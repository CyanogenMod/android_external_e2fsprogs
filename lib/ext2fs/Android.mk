LOCAL_PATH := $(call my-dir)
include $(CLEAR_VARS)

LOCAL_SRC_FILES := \
	ext2_err.c \
	alloc.c \
	alloc_sb.c \
	alloc_stats.c \
	alloc_tables.c \
	badblocks.c \
	bb_inode.c \
	bitmaps.c \
	bitops.c \
	block.c \
	bmap.c \
	check_desc.c \
	closefs.c \
	dblist.c \
	dblist_dir.c \
	dirblock.c \
	dirhash.c \
	dir_iterate.c \
	dupfs.c \
	expanddir.c \
	ext_attr.c \
	finddev.c \
	flushb.c \
	freefs.c \
	gen_bitmap.c \
	get_pathname.c \
	getsize.c \
	getsectsize.c \
	icount.c \
	ind_block.c \
	initialize.c \
	inline.c \
	inode.c \
	io_manager.c \
	ismounted.c \
	link.c \
	llseek.c \
	lookup.c \
	mkdir.c \
	mkjournal.c \
	native.c \
	newdir.c \
	openfs.c \
	read_bb.c \
	read_bb_file.c \
	res_gdt.c \
	rs_bitmap.c \
	rw_bitmaps.c \
	swapfs.c \
	tdb.c \
	unix_io.c \
	unlink.c \
	valid_blk.c \
	version.c

# get rid of this?!
LOCAL_SRC_FILES += test_io.c

LOCAL_MODULE := libext2fs
LOCAL_MODULE_TAGS := eng

LOCAL_SYSTEM_SHARED_LIBRARIES := \
	libext2_com_err \
	libext2_uuid \
	libext2_blkid \
	libext2_e2p \
	libc

LOCAL_C_INCLUDES := external/e2fsprogs/lib

LOCAL_CFLAGS := -O2 -g -W -Wall \
	-DHAVE_UNISTD_H \
	-DHAVE_ERRNO_H \
	-DHAVE_NETINET_IN_H \
	-DHAVE_SYS_IOCTL_H \
	-DHAVE_SYS_MMAN_H \
	-DHAVE_SYS_MOUNT_H \
	-DHAVE_SYS_PRCTL_H \
	-DHAVE_SYS_RESOURCE_H \
	-DHAVE_SYS_SELECT_H \
	-DHAVE_SYS_STAT_H \
	-DHAVE_SYS_TYPES_H \
	-DHAVE_STDLIB_H \
	-DHAVE_STRDUP \
	-DHAVE_MMAP \
	-DHAVE_UTIME_H \
	-DHAVE_GETPAGESIZE \
	-DHAVE_LSEEK64 \
	-DHAVE_LSEEK64_PROTOTYPE \
	-DHAVE_EXT2_IOCTLS \
	-DHAVE_LINUX_FD_H \
	-DHAVE_TYPE_SSIZE_T

LOCAL_PRELINK_MODULE := false

include $(BUILD_SHARED_LIBRARY)

