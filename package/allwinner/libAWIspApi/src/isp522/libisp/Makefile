TARGET := libisp.so
SUBLIB_DIR = ./..

commonSources:= \
	isp.c \
	isp_events/events.c \
    isp_tuning/isp_tuning.c \
    isp_manage/isp_manage.c \
    isp_manage/isp_helper.c

local_CFLAGS := \
	-I./include/V4l2Camera \
    -I./include/device \
    -I./include \
    -I./isp_dev \
    -I./isp_tuning \
    -I./

SUBDIRS = ./isp_dev ./iniparser ./isp_cfg ./isp_math

WHOLE_STATIC_LIB := \
	libisp_ae.a \
	libisp_af.a \
	libisp_afs.a \
	libisp_awb.a \
	libisp_base.a \
	libisp_gtm.a \
	libisp_iso.a \
	libisp_md.a \
	libisp_pltm.a \
	libisp_rolloff.a

WHOLE_DYNAMIC_LIB := \
	libisp_ae.so \
	libisp_af.so \
	libisp_afs.so \
	libisp_awb.so \
	libisp_base.so \
	libisp_gtm.so \
	libisp_iso.so \
	libisp_md.so \
	libisp_pltm.so \
	libisp_rolloff.so

OBJS = $(patsubst %.c, %.o, $(commonSources))

TARGET_awTuningApp := awTuningApp
OBJ_TOOLS := tuning_app/awTuningApp.o tuning_app/log_handle.o tuning_app/socket_protocol.o tuning_app/thread_pool.o \
		tuning_app/server/capture_image.o tuning_app/server/isp_handle.o tuning_app/server/mini_shell.o \
		tuning_app/server/server.o tuning_app/server/server_api.o tuning_app/server/server_core.o \
		tuning_app/server/register_opt.o tuning_app/server/raw_flow_opt.o \
		isp.o isp_events/events.o isp_tuning/isp_tuning.o isp_manage/isp_manage.o isp_manage/isp_helper.o

INIPARSER_TARGET := libiniparser.a
ISP_INI_TARGET := libisp_ini.so
ISP_DEV_TARGET := libisp_dev.a
ISP_MATH_TARGET := libisp_math.so

WHOLE_LIB := $(WHOLE_DYNAMIC_LIB)


$(TARGET):$(OBJS) $(INIPARSER_TARGET) $(ISP_INI_TARGET) $(ISP_DEV_TARGET) $(ISP_MATH_TARGET) $(WHOLE_LIB)
	$(CC) $(CFLAGS) $(LDFLAGS) $(LIBS) $(local_CFLAGS) -o $@ $^ -fPIC -shared

$(OBJS):%.o:%.c
	$(CC) $(CFLAGS) $(LDFLAGS) $(local_CFLAGS) -c $< -o $@ -fPIC -g

$(INIPARSER_TARGET):
	+$(MAKE) -C ./iniparser all

$(ISP_INI_TARGET):
	+$(MAKE) -C ./isp_cfg all

$(ISP_DEV_TARGET):
	+$(MAKE) -C ./isp_dev all

$(ISP_MATH_TARGET):
	+$(MAKE) -C ./isp_math all

$(WHOLE_LIB):
	+$(MAKE) -C ./out all CPU_VARIANT="$(CPU_VARIANT)" C_LIB_TYPE="$(C_LIB_TYPE)"

$(TARGET_awTuningApp): $(OBJ_TOOLS)
	$(CC) $(CFLAGS) -o $@ $< $(LDFLAGS) $(LIBS) -fPIC -shared -g -L./ -lisp_dev -lisp_base -lisp_math -lisp_ae -lisp_af -lisp_afs -lisp_awb \
			-lisp_md -lisp_iso -lisp_gtm -lisp_pltm -lisp_rolloff -lisp_ini -liniparser  -lpthread -lrt -lm

all: $(TARGET)  install

clean:
	-rm $(OBJS) $(TARGET)
install:
	mkdir -p $(INSTALL_PREFIX)/usr/lib
	install -m0755 $(TARGET) $(INSTALL_PREFIX)/usr/lib
	install -m0755 *.so $(SUBLIB_DIR)
