LOCAL_PATH := $(call my-dir)
include $(CLEAR_VARS)
LOCAL_SRC_FILES := \
                    ./src/hci_imp.c \
                    ./src/os_msg.c \
                    ./src/os_sched.c \
                    ./src/os_sync.c \
                    ./src/os_timer.c \
                    ./src/data_uart.c

LOCAL_C_INCLUDES := $(LOCAL_PATH)/../trace
LOCAL_C_INCLUDES += $(LOCAL_PATH)/inc
LOCAL_C_INCLUDES += $(LOCAL_PATH)/../bluez/inc
LOCAL_C_INCLUDES += $(LOCAL_PATH)/../
LOCAL_C_INCLUDES += $(LOCAL_PATH)/../ftl \
                    $(mesh_INCLUDE)

#LOCAL_CFLAGS += -Wno-pointer-to-int-cast
#LOCAL_CFLAGS += -Wunused-variable
#LOCAL_CFLAGS += -fPIC
LOCAL_CFLAGS += $(mesh_CFLAGS)

LOCAL_SHARED_LIBRARIES := \
                          libcutils \
                          libutils \
                          liblog

#LOCAL_LDLIBS := -Wl,-Bsymbolic,-Bsymbolic-functions
LOCAL_MODULE := platform
LOCAL_MODULE_TAGS := optional

#LOCAL_MODULE_CLASS := SHARED_LIBRARIE
LOCAL_MULTILIB := 32

include $(BUILD_STATIC_LIBRARY)
