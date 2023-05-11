#
# Copyright (C) 2014 OpenWrt.org
#
# This is free software, licensed under the GNU General Public License v2.
# See /LICENSE for more information.
#
include $(TOPDIR)/rules.mk

ARCH:=arm
BOARD:=t7-chimera
BOARDNAME:=t7-chimera
FEATURES:=fpu usb ext4 display rtc
MAINTAINER:=xudong
CPU_TYPE:=cortex-a7
CPU_SUBTYPE:=neon

KERNEL_PATCHVER:=4.9
UBOOT_PATCHVER:=2014.07

DEVICE_TYPE:=developerboard

include $(BUILD_DIR)/target.mk

define Target/Description
	Build multi-platform images for the ARMv7 instruction set architecture
endef

KERNELNAME:=zImage dtbs

ifneq ($(CONFIG_KERNEL_KERNEL_LZO),)
KERNELNAME:=Image.lzo dtbs
endif

$(eval $(call BuildTarget))
