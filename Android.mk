ifneq ($(TARGET_SIMULATOR),true)
ifeq ($(TARGET_ARCH),x86)
use_e2fsprogs_module_tags := eng
include $(call all-subdir-makefiles)
else
ifeq ($(USE_E2FSPROGS),true)
use_e2fsprogs_module_tags :=
include $(call all-subdir-makefiles)
endif
endif
endif
