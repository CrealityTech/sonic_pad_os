LOCAL_PATH := $(call my-dir)
include $(CLEAR_VARS)
LOCAL_SRC_FILES := \
                   hci_mgmt.c

LOCAL_C_INCLUDES += $(LOCAL_PATH)/../bluez/inc \
                    $(mesh_INCLUDE)

LOCAL_CFLAGS += $(mesh_CFLAGS) -Wno-pointer-to-int-cast
LOCAL_CFLAGS += -fPIC

LOCAL_LDLIBS += -llog
LOCAL_MODULE    := hci_mgmt
include $(BUILD_STATIC_LIBRARY)
