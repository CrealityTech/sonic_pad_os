#
# Copyright (C) 2013-2016 OpenWrt.org
#
# This is free software, licensed under the GNU General Public License v2.
# See /LICENSE for more information.
#

include $(TOPDIR)/rules.mk

ARCH:=arm
BOARD:=c600-cuckoo
BOARDNAME:=c600-cuckoo
FEATURES:=fpu usb ext4 display rtc
CPU_TYPE:=arm926ej-s
#CPU_SUBTYPE:=neon
MAINTAINER:=luke

KERNEL_PATCHVER:=3.10
UBOOT_PATCHVER:=2014.07
KERNELNAME:=zImage dtbs

include $(BUILD_DIR)/target.mk

DEFAULT_PACKAGES +=

$(eval $(call BuildTarget))
