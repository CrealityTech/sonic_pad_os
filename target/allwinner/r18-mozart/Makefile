#
# Copyright (C) 2014 OpenWrt.org
#
# This is free software, licensed under the GNU General Public License v2.
# See /LICENSE for more information.
#
include $(TOPDIR)/rules.mk

ARCH:=aarch64
BOARD:=r18-mozart
BOARDNAME:=r18-mozart
FEATURES:=fpu ramdisk
CFLAGS:=-Os -pipe -fno-caller-saves
MAINTAINER:=Henrisk

KERNEL_PATCHVER:=4.4
UBOOT_PATCHVER:=2014.07

DEVICE_TYPE:=developerboard

include $(BUILD_DIR)/target.mk

define Target/Description
	Build multi-platform images for the ARMv8 instruction set architecture
endef

KERNELNAME:=Image.gz dtbs

$(eval $(call BuildTarget))
