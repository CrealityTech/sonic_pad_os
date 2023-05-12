LOCAL_PATH := $(call my-dir)
include $(CLEAR_VARS)
LOCAL_SRC_FILES  := $(call all-c-files-under, src)

LOCAL_C_INCLUDES += $(LOCAL_PATH)/inc
LOCAL_C_INCLUDES += $(LOCAL_PATH)
LOCAL_C_INCLUDES += $(LOCAL_PATH)/../trace \

LOCAL_CFLAGS += $(mesh_CFLAGS)

LOCAL_LDLIBS += -llog
LOCAL_MODULE    := bluez
include $(BUILD_STATIC_LIBRARY)
