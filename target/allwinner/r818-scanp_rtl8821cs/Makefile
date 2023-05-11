#
# Copyright (C) 2014 OpenWrt.org
#
# This is free software, licensed under the GNU General Public License v2.
# See /LICENSE for more information.
#
include $(TOPDIR)/rules.mk

ARCH:=aarch64
BOARD:=r818-scanp_rtl8821cs
BOARDNAME:=r818-scanp_rtl8821cs
FEATURES:=fpu ramdisk
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

$(eval $(call BuildTarget))
