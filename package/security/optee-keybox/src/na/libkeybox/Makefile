# By default we expect optee_client exported folder to be on a certain relative
# # path, but if the client specifies the OPTEE_CLIENT_EXPORT then that path will
# # be used instead.
# OPTEE_CLIENT_EXPORT ?= $(DEV_KIT_DIR)/arm-plat-$(PLATFORM)/export-ca

CC		= $(CROSS_COMPILE)gcc
CPP		= $(CROSS_COMPILE)cpp
LD		= $(CROSS_COMPILE)ld
AR		= $(CROSS_COMPILE)ar
NM		= $(CROSS_COMPILE)nm
OBJCOPY		= $(CROSS_COMPILE)objcopy
OBJDUMP		= $(CROSS_COMPILE)objdump
READELF		= $(CROSS_COMPILE)readelf

OUT_DIR ?= out
LIB_SRCS = secure_storage.c secure_storage_ioctl.c api.c local_crc32.c
LIB_OUT_DIR = $(OUT_DIR)/libkeybox
LIB_CFLAGS =
LIB_CFLAGS += -I./
objs 	:= $(patsubst %.c,$(LIB_OUT_DIR)/%.o, $(LIB_SRCS))

LIB_NAME ?= libkeybox.so
LIB_CFLAGS += --no-builtin-variables
LIB_CFLAGS += -fPIC
LIB_CFLAGS += -o $(LIB_OUT_DIR)/$(LIB_NAME)

.PHONY:all
all: $(OUT_DIR)/$(LIB_NAME)

$(LIB_OUT_DIR)/%.o: $(CURDIR)/%.c
	$(q)mkdir -p $(LIB_OUT_DIR)/
	@echo '  CC      $<'
	$(q)$(CC) $(LIB_CFLAGS) -c $< -o $@

$(OUT_DIR)/$(LIB_NAME):$(LIB_SRCS) $(objs)
	$(q)$(AR) rcs $@ $(objs)

clean:
	rm -rf $(OUT_DIR)
