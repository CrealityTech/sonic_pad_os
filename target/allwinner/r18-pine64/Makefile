#
# Copyright (C) 2014 OpenWrt.org
#
# This is free software, licensed under the GNU General Public License v2.
# See /LICENSE for more information.
#
include $(TOPDIR)/rules.mk

ARCH:=aarch64
BOARD:=r18-pine64
BOARDNAME:=r18-pine64
FEATURES:=fpu ramdisk
CFLAGS:=-Os -pipe -fno-caller-saves
MAINTAINER:=TraceWong

KERNEL_PATCHVER:=4.4
UBOOT_PATCHVER:=2014.07

DEVICE_TYPE:=developerboard

include $(BUILD_DIR)/target.mk

define Target/Description
	Build multi-platform images for the ARMv8 instruction set architecture
endef

KERNELNAME:=Image.gz dtbs

ifneq ($(CONFIG_KERNEL_KERNEL_LZO),)
KERNELNAME:=Image.lzo dtbs
endif

$(eval $(call BuildTarget))
