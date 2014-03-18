/*
 * Copyright (C) 2013 Alejandro Martinez Ruiz <alex@nowcomputing.com>
 *
 * This file may be redistributed under the terms of the
 * GNU Lesser General Public License
 *
 * Initial Android port
 * March 2014 Ketut P. Kumajaya <ketut.kumajaya@gmail.com>
 */

#include <stddef.h>
#include <string.h>

#include "blkidP.h"
#include "uuid/uuid.h"
#include "probe.h"

#define le16_to_cpu(x) blkid_le16(x)

typedef __u8  uint8_t;
typedef __u16 uint16_t;
typedef __u32 uint32_t;
typedef __u64 uint64_t;

typedef struct blkid_probe* blkid_probe;

#define F2FS_UUID_SIZE		16
#define F2FS_LABEL_SIZE		512

struct f2fs_super_block {				/* According to version 1.1 */
/* 0x00 */	__u32	magic;				/* Magic Number */
/* 0x04 */	__u16	major_ver;			/* Major Version */
/* 0x06 */	__u16	minor_ver;			/* Minor Version */
/* 0x08 */	__u32	log_sectorsize;			/* log2 sector size in bytes */
/* 0x0C */	__u32	log_sectors_per_block;		/* log2 # of sectors per block */
/* 0x10 */	__u32	log_blocksize;			/* log2 block size in bytes */
/* 0x14 */	__u32	log_blocks_per_seg;		/* log2 # of blocks per segment */
/* 0x18 */	__u32	segs_per_sec;			/* # of segments per section */
/* 0x1C */	__u32	secs_per_zone;			/* # of sections per zone */
/* 0x20 */	__u32	checksum_offset;		/* checksum offset inside super block */
/* 0x24 */	__u64	block_count;			/* total # of user blocks */
/* 0x2C */	__u32	section_count;			/* total # of sections */
/* 0x30 */	__u32	segment_count;			/* total # of segments */
/* 0x34 */	__u32	segment_count_ckpt;		/* # of segments for checkpoint */
/* 0x38 */	__u32	segment_count_sit;		/* # of segments for SIT */
/* 0x3C */	__u32	segment_count_nat;		/* # of segments for NAT */
/* 0x40 */	__u32	segment_count_ssa;		/* # of segments for SSA */
/* 0x44 */	__u32	segment_count_main;		/* # of segments for main area */
/* 0x48 */	__u32	segment0_blkaddr;		/* start block address of segment 0 */
/* 0x4C */	__u32	cp_blkaddr;			/* start block address of checkpoint */
/* 0x50 */	__u32	sit_blkaddr;			/* start block address of SIT */
/* 0x54 */	__u32	nat_blkaddr;			/* start block address of NAT */
/* 0x58 */	__u32	ssa_blkaddr;			/* start block address of SSA */
/* 0x5C */	__u32	main_blkaddr;			/* start block address of main area */
/* 0x60 */	__u32	root_ino;			/* root inode number */
/* 0x64 */	__u32	node_ino;			/* node inode number */
/* 0x68 */	__u32	meta_ino;			/* meta inode number */
/* 0x6C */	__u8	uuid[F2FS_UUID_SIZE];		/* 128-bit uuid for volume */
/* 0x7C */	__u16	volume_name[F2FS_LABEL_SIZE];	/* volume name */
#if 0
/* 0x47C */	__u32	extension_count;		/* # of extensions below */
/* 0x480 */	__u8	extension_list[64][8];		/* extension array */
#endif
} __attribute__((packed));

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

int probe_f2fs(struct blkid_probe *probe,
		      struct blkid_magic *id __BLKID_ATTR((unused)),
		      unsigned char *buf)
{
	struct f2fs_super_block *sb;
	uint16_t major, minor;
	char uuid_str[37];

	sb = (struct f2fs_super_block *) buf;
	if (!sb)
		return -1;

	major = le16_to_cpu(sb->major_ver);
	minor = le16_to_cpu(sb->minor_ver);

	/* For version 1.0 we cannot know the correct sb structure */
	if (major == 1 && minor == 0)
		return 0;

	if (*((unsigned char *) sb->volume_name)) {
		unsigned char utf8_label[512];
		unicode_16le_to_utf8(utf8_label, sizeof(utf8_label),
			(unsigned char *) sb->volume_name, sizeof(sb->volume_name));
		blkid_set_tag(probe->dev, "LABEL", utf8_label, 0);
	}

	uuid_unparse(sb->uuid, uuid_str);
	blkid_set_tag(probe->dev, "UUID", uuid_str, sizeof(uuid_str));
	return 0;
}
