LOCAL_PATH := $(call my-dir)

include $(CLEAR_VARS)
LOCAL_MODULE       := fstab.qcom_ramdisk
LOCAL_MODULE_STEM  := fstab.qcom
LOCAL_MODULE_TAGS  := optional
LOCAL_MODULE_CLASS := ETC
ifeq ($(TARGET_IS_VAB),true)
LOCAL_SRC_FILES    := etc/fstab_AB.qcom
LOCAL_MODULE_PATH  := $(TARGET_VENDOR_RAMDISK_OUT)/first_stage_ramdisk
else
ifneq ($(TARGET_IS_LEGACY),true)
LOCAL_SRC_FILES    := etc/fstab_dynamic.qcom
else
LOCAL_SRC_FILES    := etc/fstab_legacy.qcom
endif
LOCAL_MODULE_PATH  := $(TARGET_RAMDISK_OUT)
endif
include $(BUILD_PREBUILT)

# prebuilt_root doesn't cover recovery ramdisk, that sucks

include $(CLEAR_VARS)
LOCAL_MODULE       := init.recovery.qcom.rc
LOCAL_MODULE_TAGS  := optional
LOCAL_MODULE_CLASS := ETC
LOCAL_SRC_FILES    := etc/init.recovery.qcom.rc
LOCAL_MODULE_PATH  := $(TARGET_ROOT_OUT)
include $(BUILD_PREBUILT)
