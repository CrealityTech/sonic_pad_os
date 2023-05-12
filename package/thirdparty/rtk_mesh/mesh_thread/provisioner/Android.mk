LOCAL_PATH := $(call my-dir)

include $(CLEAR_VARS)
LOCAL_SRC_FILES := \
                   mesh_cmd_handler.c \
                   mesh_provisioner.c \
                   mesh_blue_cmd.c \
                   mesh_lib_thread.c \
                   RTKMeshCommon.c

LOCAL_C_INCLUDES := \
                    $(LOCAL_PATH) \
                    $(LOCAL_PATH)/../../platform/inc \
                    $(LOCAL_PATH)/../../lib/inc \
                    $(LOCAL_PATH)/../../lib/gap \
                    $(LOCAL_PATH)/../../lib/cmd \
                    $(LOCAL_PATH)/../../lib/model \
                    $(LOCAL_PATH)/../../lib/model/realtek \
                    $(LOCAL_PATH)/../../lib/common \
                    $(LOCAL_PATH)/../../lib/platform \
                    $(LOCAL_PATH)/../../trace \
                    $(LOCAL_PATH)/../../bluez/inc

LOCAL_CFLAGS := \
                -std=c99 \
                -Wno-unused-parameter \
                -Wno-unused-variable \
				-Wno-unused-function \
                -Wno-typedef-redefinition

LOCAL_SHARED_LIBRARIES:= \
                        libmesh-wrapper \
                        libmesh-lib
LOCAL_CFLAGS += -fPIC

LOCAL_LDLIBS += -ldl
LOCAL_LDLIBS += -llog
LOCAL_MODULE := libmesh-api
LOCAL_MODULE_CLASS := SHARED_LIBRARIES

include $(BUILD_SHARED_LIBRARY)
