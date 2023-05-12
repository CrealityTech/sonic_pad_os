LOCAL_PATH := $(call my-dir)
include $(CLEAR_VARS)
LOCAL_SRC_FILES  := \
                    ./app_cmd.c \
                    ./app_handler.c \
                    ./mesh_app.c \


LOCAL_C_INCLUDES := $(LOCAL_PATH)/../../bluez/inc \
                    $(LOCAL_PATH)/../../gap/inc \
                    $(LOCAL_PATH)/../../trace \
                    $(LOCAL_PATH)/../../platform/inc \
                    $(LOCAL_PATH)/../../ftl \
                    $(LOCAL_PATH)/../../lib/cmd \
                    $(LOCAL_PATH)/../../lib/common \
                    $(LOCAL_PATH)/../../lib/gap \
                    $(LOCAL_PATH)/../../lib/inc \
                    $(LOCAL_PATH)/../../lib/profile \
                    $(LOCAL_PATH)/../../lib/platform \
                    $(LOCAL_PATH)/../../lib/model \
                    $(LOCAL_PATH)/../../lib/model/realtek \
                    $(LOCAL_PATH)/../provisioner \
                    $(LOCAL_PATH)/


LOCAL_CFLAGS += \
                -Wno-pointer-to-int-cast \
                -Wno-unused-parameter \
                -Wno-unused-variable \
                -Wno-unused-function \
                -Wno-missing-field-initializers \
                -Wno-typedef-redefinition

LOCAL_CFLAGS += -fPIC \
                -std=c99

#LOCAL_LDLIBS += -ldl
#LOCAL_LDLIBS += -llog
LOCAL_MODULE := mesh_thread_test

LOCAL_SHARED_LIBRARIES :=   libmesh-lib \
                            libmesh-wrapper \
                            libmesh-api \
                            liblog
include $(BUILD_EXECUTABLE)
