LOCAL_PATH := $(call my-dir)
include $(CLEAR_VARS)
LOCAL_SRC_FILES := \
                   trace.c

LOCAL_C_INCLUDES += $(LOCAL_PATH)/../bluez/inc \
LOCAL_C_INCLUDES += $(LOCAL_PATH)/../platform/inc \
                    $(mesh_INCLUDE)

LOCAL_CFLAGS += $(mesh_CFLAGS) -Wno-pointer-to-int-cast
LOCAL_CFLAGS += -fPIC

LOCAL_LDLIBS += -llog
LOCAL_MODULE    := trace
include $(BUILD_STATIC_LIBRARY)
