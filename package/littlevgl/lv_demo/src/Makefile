#TARGET := lv_demo
#
##LVGL_DIR ?= ./
#LVGL_DIR ?= ${shell pwd}
#include $(LVGL_DIR)/lvgl/lvgl.mk
#include $(LVGL_DIR)/lv_drivers/lv_drivers.mk
#include $(LVGL_DIR)/lv_examples/lv_examples.mk
#
#CFLAGS += -I$(LVGL_DIR)/
#MAINSRC = ./main.c
#SRCS = $(CSRCS) $(MAINSRC)
#OBJS = $(patsubst %.c, %.o, $(SRCS))
#
#LIBS :=
#
#$(OBJS):%.o:%.c
#	$(CC) $(CFLAGS) $(LDFLAGS)  -c $< -o $@
#
#$(TARGET):$(OBJS)
#	$(CC) $(CFLAGS) $(LDFLAGS) $(LIBS) -o $@ $^
#
#all: $(TARGET) install
#
#clean:
#	-rm $(OBJS) $(TARGET)
#
#install:
#	mkdir -p $(INSTALL_PREFIX)/usr/lib
#	cp $(TARGET) $(INSTALL_PREFIX)/usr/lib
#


TARGET = lv_demo

SRCDIRS = $(shell find . -maxdepth 5 -type d)

INCLUDES += $(foreach dir,$(SRCDIRS),-I$(dir))  \


SRCS = $(foreach dir,$(SRCDIRS),$(wildcard $(dir)/*.c))
OBJS = $(SRCS:.c=.o)
#LIB +=

%.o: %.c
	$(CC) $(INCLUDES) $(LIB) -c -o $@ $<

$(TARGET): $(OBJS)
	$(CC) -o $@ $(OBJS) $(LIB)

all:install
install:$(TARGET)

clean:
	-rm -rf $(OBJS) $(TARGET)