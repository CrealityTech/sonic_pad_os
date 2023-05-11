#
# Copyright (C) 2014 OpenWrt.org
#
# This is free software, licensed under the GNU General Public License v2.
# See /LICENSE for more information.
#
include $(TOPDIR)/rules.mk

ARCH:=aarch64
BOARD:=r329-ubidemo
BOARDNAME:=r329-ubidemo
FEATURES:=fpu ramdisk usb audio nand ubifs
CFLAGS:=-Os -pipe -fno-caller-saves
MAINTAINER:=Allwinner

KERNEL_PATCHVER:=4.9
UBOOT_PATCHVER:=2018

DEVICE_TYPE:=developerboard

include $(BUILD_DIR)/target.mk

KERNELNAME:=Image.gz dtbs

ifneq ($(CONFIG_KERNEL_KERNEL_LZO),)
KERNELNAME:=Image.lzo dtbs
endif

ifneq ($(CONFIG_TARGET_ROOTFS_UBIFS),)
NAND_SUPER_PAGESIZE := 4096
NAND_SUPER_BLOCKSIZE := $(shell expr 256 \* 1024)
ifneq ($(wildcard $(DEVICE_PLATFORM_DIR)/sys_partition.fex),)
SYS_PART := $(DEVICE_PLATFORM_DIR)/sys_partition.fex
else
$(error not found $(DEVICE_PLATFORM_DIR)/sys_partition.fex)
endif
ROOTFS_TOTAL_SIZE := $(shell awk '$$3=="rootfs"{while ($$1 != "size") getline; print $$3 * 512}' $(SYS_PART))
UBI_LEB_SIZE := $(shell expr $(NAND_SUPER_BLOCKSIZE) - $(NAND_SUPER_PAGESIZE))
UBI_LEB_CNT := $(shell expr $(ROOTFS_TOTAL_SIZE) / $(UBI_LEB_SIZE))
UBIFS_OPTS := -m $(NAND_SUPER_PAGESIZE) -R 0 -e $(UBI_LEB_SIZE) -c $(UBI_LEB_CNT)
endif

$(eval $(call BuildTarget))
