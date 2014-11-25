LOCAL_PATH := $(call my-dir)

libext2fs_src_files := \
	ext2_err.c \
	alloc.c \
	alloc_sb.c \
	alloc_stats.c \
	alloc_tables.c \
	badblocks.c \
	bb_inode.c \
	bitmaps.c \
	bitops.c \
	blkmap64_ba.c \
	blkmap64_rb.c \
	blknum.c \
	block.c \
	bmap.c \
	check_desc.c \
	crc16.c \
	csum.c \
	closefs.c \
	dblist.c \
	dblist_dir.c \
	dirblock.c \
	dirhash.c \
	dir_iterate.c \
	dupfs.c \
	expanddir.c \
	ext_attr.c \
	extent.c \
	fileio.c \
	finddev.c \
	flushb.c \
	freefs.c \
	gen_bitmap.c \
	gen_bitmap64.c \
	get_pathname.c \
	getsize.c \
	getsectsize.c \
	i_block.c \
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
	mmp.c \
	mkdir.c \
	mkjournal.c \
	native.c \
	newdir.c \
	openfs.c \
	progress.c \
	punch.c \
	rbtree.c \
	read_bb.c \
	read_bb_file.c \
	res_gdt.c \
	rw_bitmaps.c \
	swapfs.c \
	tdb.c \
	undo_io.c \
	unix_io.c \
	unlink.c \
	valid_blk.c \
	version.c

# get rid of this?!
libext2fs_src_files += test_io.c

libext2fs_shared_libraries := \
	libext2_com_err \
	libext2_uuid \
	libext2_blkid \
	libext2_e2p

libext2fs_system_shared_libraries := libc

libext2fs_c_includes := external/e2fsprogs/lib

libext2fs_cflags := -O2 -g -W -Wall \
	-DHAVE_UNISTD_H \
	-DHAVE_ERRNO_H \
	-DHAVE_NETINET_IN_H \
	-DHAVE_SYS_IOCTL_H \
	-DHAVE_SYS_MMAN_H \
	-DHAVE_SYS_MOUNT_H \
	-DHAVE_SYS_RESOURCE_H \
	-DHAVE_SYS_SELECT_H \
	-DHAVE_SYS_STAT_H \
	-DHAVE_SYS_TYPES_H \
	-DHAVE_STDLIB_H \
	-DHAVE_STRDUP \
	-DHAVE_MMAP \
	-DHAVE_UTIME_H \
	-DHAVE_GETPAGESIZE \
	-DHAVE_EXT2_IOCTLS \
	-DHAVE_TYPE_SSIZE_T \
	-DHAVE_SYS_TIME_H \
        -DHAVE_SYS_PARAM_H \
	-DHAVE_SYSCONF

libext2fs_cflags_linux := \
	-DHAVE_LINUX_FD_H \
	-DHAVE_SYS_PRCTL_H \
	-DHAVE_LSEEK64 \
	-DHAVE_LSEEK64_PROTOTYPE \
	-DHAVE_SETMNTENT \
	-DHAVE_MNTENT_H

include $(CLEAR_VARS)

LOCAL_SRC_FILES := $(libext2fs_src_files)
LOCAL_SYSTEM_SHARED_LIBRARIES := $(libext2fs_system_shared_libraries)
LOCAL_SHARED_LIBRARIES := $(libext2fs_shared_libraries)
LOCAL_C_INCLUDES := $(libext2fs_c_includes)
LOCAL_CFLAGS := $(libext2fs_cflags) $(libext2fs_cflags_linux)
LOCAL_PRELINK_MODULE := false
LOCAL_MODULE := libext2fs
LOCAL_MODULE_TAGS := optional

include $(BUILD_SHARED_LIBRARY)

include $(CLEAR_VARS)

LOCAL_SRC_FILES := $(libext2fs_src_files)
LOCAL_STATIC_LIBRARIES := $(libext2fs_system_shared_libraries) $(libext2fs_shared_libraries)
LOCAL_C_INCLUDES := $(libext2fs_c_includes)
LOCAL_CFLAGS := $(libext2fs_cflags) $(libext2fs_cflags_linux)
LOCAL_PRELINK_MODULE := false
LOCAL_MODULE := libext2fs
LOCAL_MODULE_TAGS := optional

include $(BUILD_STATIC_LIBRARY)

include $(CLEAR_VARS)

LOCAL_SRC_FILES := $(libext2fs_src_files)
LOCAL_SHARED_LIBRARIES := $(addsuffix _host, $(libext2fs_shared_libraries))
LOCAL_C_INCLUDES := $(libext2fs_c_includes)
ifeq ($(HOST_OS),linux)
LOCAL_CFLAGS := $(libext2fs_cflags) $(libext2fs_cflags_linux)
else
LOCAL_CFLAGS := $(libext2fs_cflags)
endif
LOCAL_MODULE := libext2fs_host
LOCAL_MODULE_TAGS := optional

include $(BUILD_HOST_SHARED_LIBRARY)
