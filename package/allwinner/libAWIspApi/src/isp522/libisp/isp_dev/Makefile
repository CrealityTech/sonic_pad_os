TARGET := libisp_dev.so libisp_dev.a
SUBLIB_DIR = ./..

SHARED_TARGET := libisp_dev.so
STATIC_TARGET := libisp_dev.a

commonSources:= \
	isp_v4l2_helper.c \
    isp_dev.c \
    media.c \
    video.c

local_CFLAGS := -I./ \
    -I./../include/V4l2Camera \
    -I./../include/device

OBJS = $(patsubst %.c, %.c.o, $(commonSources))

$(SHARED_TARGET):$(OBJS)
	$(CC) $(CFLAGS) $(LDFLAGS) $(LIBS) $(local_CFLAGS) -o $@ $^ -fPIC -shared

$(STATIC_TARGET):$(OBJS)
	$(AR) rc $@ $^

$(OBJS):%.c.o:%.c
	$(CC) $(CFLAGS) $(LDFLAGS) $(local_CFLAGS) -c $< -o $@ -fPIC

all: $(TARGET) install

clean:
	-rm $(OBJS) $(TARGET)
install:
	mkdir -p $(INSTALL_PREFIX)/usr/lib
	install -m0755 $(TARGET) $(INSTALL_PREFIX)/usr/lib
	install -m0755 $(TARGET) $(SUBLIB_DIR)
