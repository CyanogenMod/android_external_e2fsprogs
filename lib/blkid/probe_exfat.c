/*
 * Copyright (C) 2010 Andrew Nayenko <resver@gmail.com>
 *
 * This file may be redistributed under the terms of the
 * GNU Lesser General Public License.
 */

#include <stdio.h>
#include <string.h>
#include <math.h>
#include "blkidP.h"
#include "probe.h"

#define le32_to_cpu(x) blkid_le32(x)

typedef __u8  uint8_t;
typedef __u16 uint16_t;
typedef __u32 uint32_t;
typedef __uint64_t uint64_t;

typedef struct blkid_probe* blkid_probe;

struct exfat_super_block {
	__u8 jump[3];
	__u8 oem_name[8];
	__u8	__unused1[53];
	__u64 block_start;
	__u64 block_count;
	__u32 fat_block_start;
	__u32 fat_block_count;
	__u32 cluster_block_start;
	__u32 cluster_count;
	__u32 rootdir_cluster;
	__u8 volume_serial[4];
	struct {
		__u8 minor;
		__u8 major;
	} version;
	__u16 volume_state;
	__u8 block_bits;
	__u8 bpc_bits;
	__u8 fat_count;
	__u8 drive_no;
	__u8 allocated_percent;
} __attribute__((__packed__));

struct exfat_entry_label {
	__u8 type;
	__u8 length;
	__u8 name[30];
} __attribute__((__packed__));

#define BLOCK_SIZE(sb) (1 << (sb)->block_bits)
#define CLUSTER_SIZE(sb) (BLOCK_SIZE(sb) << (sb)->bpc_bits)
#define EXFAT_FIRST_DATA_CLUSTER 2
#define EXFAT_LAST_DATA_CLUSTER 0xffffff6
#define EXFAT_ENTRY_SIZE 32

#define EXFAT_ENTRY_EOD		0x00
#define EXFAT_ENTRY_LABEL	0x83

static blkid_loff_t block_to_offset(const struct exfat_super_block *sb,
		blkid_loff_t block)
{
	return (blkid_loff_t) block << sb->block_bits;
}

static blkid_loff_t cluster_to_block(const struct exfat_super_block *sb,
		uint32_t cluster)
{
	return le32_to_cpu(sb->cluster_block_start) +
			((blkid_loff_t) (cluster - EXFAT_FIRST_DATA_CLUSTER)
					<< sb->bpc_bits);
}

static blkid_loff_t cluster_to_offset(const struct exfat_super_block *sb,
		uint32_t cluster)
{
	return block_to_offset(sb, cluster_to_block(sb, cluster));
}

extern unsigned char *blkid_probe_get_buffer(struct blkid_probe *pr,
                          blkid_loff_t off, size_t len);

static uint32_t next_cluster(blkid_probe pr,
		const struct exfat_super_block *sb, uint32_t cluster)
{
	uint32_t *next;
	blkid_loff_t fat_offset;

	fat_offset = block_to_offset(sb, le32_to_cpu(sb->fat_block_start))
		+ (blkid_loff_t) cluster * sizeof(cluster);
	next = (uint32_t *) blkid_probe_get_buffer(pr, fat_offset,
			sizeof(uint32_t));
	if (!next)
		return 0;
	return le32_to_cpu(*next);
}

static struct exfat_entry_label *find_label(blkid_probe pr,
		const struct exfat_super_block *sb)
{
	uint32_t cluster = le32_to_cpu(sb->rootdir_cluster);
	blkid_loff_t offset = cluster_to_offset(sb, cluster);
	uint8_t *entry;

	for (;;) {
		entry = (uint8_t *) blkid_probe_get_buffer(pr, offset,
				EXFAT_ENTRY_SIZE);
		if (!entry)
			return NULL;
		if (entry[0] == EXFAT_ENTRY_EOD)
			return NULL;
		if (entry[0] == EXFAT_ENTRY_LABEL)
			return (struct exfat_entry_label *) entry;
		offset += EXFAT_ENTRY_SIZE;
		if (offset % CLUSTER_SIZE(sb) == 0) {
			cluster = next_cluster(pr, sb, cluster);
			if (cluster < EXFAT_FIRST_DATA_CLUSTER)
				return NULL;
			if (cluster > EXFAT_LAST_DATA_CLUSTER)
				return NULL;
			offset = cluster_to_offset(sb, cluster);
		}
	}
}

static void unicode_16le_to_utf8(unsigned char *str, int out_len,
				 const unsigned char *buf, int in_len)
{
	int i, j;
	unsigned int c;

	for (i = j = 0; i + 2 <= in_len; i += 2) {
		c = (buf[i+1] << 8) | buf[i];
		if (c == 0) {
			str[j] = '\0';
			break;
		} else if (c < 0x80) {
			if (j+1 >= out_len)
				break;
			str[j++] = (unsigned char) c;
		} else if (c < 0x800) {
			if (j+2 >= out_len)
				break;
			str[j++] = (unsigned char) (0xc0 | (c >> 6));
			str[j++] = (unsigned char) (0x80 | (c & 0x3f));
		} else {
			if (j+3 >= out_len)
				break;
			str[j++] = (unsigned char) (0xe0 | (c >> 12));
			str[j++] = (unsigned char) (0x80 | ((c >> 6) & 0x3f));
			str[j++] = (unsigned char) (0x80 | (c & 0x3f));
		}
	}
	str[j] = '\0';
}

int probe_exfat(struct blkid_probe *probe,
		      struct blkid_magic *id __BLKID_ATTR((unused)),
		      unsigned char *buf)
{
	struct exfat_super_block *sb;
	struct exfat_entry_label *label;
	char serno[10];

	sb = (struct exfat_super_block *) buf;

	sprintf(serno, "%02X%02X-%02X%02X",
			sb->volume_serial[3], sb->volume_serial[2],
			sb->volume_serial[1], sb->volume_serial[0]);

	blkid_set_tag(probe->dev, "UUID", serno, sizeof(serno)-1);
	label = find_label(probe, sb);
	if (label) {
		char utf8_label[128];
		unicode_16le_to_utf8(utf8_label, sizeof(utf8_label), label->name, label->length * 2);
		blkid_set_tag(probe->dev, "LABEL", utf8_label, 0);
	}
	return 0;
}
