ifneq ($(TARGET_SIMULATOR),true)
use_e2fsprogs_module_tags := optional
include $(call all-subdir-makefiles)
endif
