LOCAL_PATH := $(call my-dir)
include $(CLEAR_VARS)

LOCAL_SRC_FILES := \
	cache.c \
	dev.c \
	devname.c \
	devno.c \
	getsize.c \
	llseek.c \
	probe.c \
	read.c \
	resolve.c \
	save.c \
	tag.c \
	version.c \


LOCAL_MODULE := libext2_blkid
LOCAL_MODULE_TAGS:= eng
LOCAL_SYSTEM_SHARED_LIBRARIES := libext2_uuid libc

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
