LOCAL_PATH := $(call my-dir)
include $(CLEAR_VARS)
LOCAL_SRC_FILES  := \
                    ftl.c

LOCAL_C_INCLUDES := \
                    $(mesh_INCLUDE) \
                    $(LOCAL_PATH)/../trace \
                    $(LOCAL_PATH)/../platform/inc

LOCAL_CFLAGS += $(mesh_CFLAGS) -Wno-pointer-to-int-cast

LOCAL_CFLAGS += -fPIC

LOCAL_LDLIBS += -llog
LOCAL_MODULE    := ftl
include $(BUILD_STATIC_LIBRARY)
