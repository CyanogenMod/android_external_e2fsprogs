ifneq ($(TARGET_SIMULATOR),true)
ifeq ($(TARGET_ARCH),x86)
include $(call all-subdir-makefiles)
else
ifeq ($(USE_E2FSPROGS),true)
include $(call all-subdir-makefiles)
endif
endif
endif
