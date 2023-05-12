#
# Copyright (C) 2018 Allwinner Technology Limited. All rights reserved
#
# This is free software, licensed under the GNU General Public License v2.
# See /LICENSE for more information.
#
# Author: Albert Yu <yuxyun@allwinnertech.com>
#

include $(TOPDIR)/rules.mk
include $(BUILD_DIR)/kernel.mk
-include $(TINA_BUILD_TOP)/.config

ifeq ($(CONFIG_COMPLILE_KERNEL64_USER32),y)
LINUX_KARCH=arm64
TARGET_CROSS=aarch64-openwrt-linux-
endif

MAKE_OPTS:=\
	CROSS_COMPILE=$(TARGET_CROSS) \
	KDIR=$(LINUX_DIR) \
	KERNELDIR=$(LINUX_DIR) \
	LICHEE_PLATFORM=linux \
	LICHEE_KDIR=$(LINUX_DIR)

ifeq ($(TARGET_ARCH), arm)
MAKE_OPTS+=\
	ARCH=arm
else ifeq ($(TARGET_ARCH), aarch64)
MAKE_OPTS+=\
        ARCH=arm64
else
	$(error Invalid TARGET_ARCH)
endif

#Make sure TARGET_PLATFORM is null to avoid build error for Mali Utgard Device Driver
TARGET_PLATFORM=
GPU_NEW_DIR := $(LINUX_DIR)/modules/gpu/mali-utgard
GPU_OLD_DIR := $(GPU_NEW_DIR)/kernel_mode
ifeq ($(TARGET_LINUX_VERSION), $(filter $(TARGET_LINUX_VERSION),3.4))
	MAKE_OPTS+=USING_DT=0
else
	MAKE_OPTS+=USING_DT=1
endif
#For Mali Midgard platforms
ifeq ($(TARGET_PLATFORM), $(filter $(TARGET_PLATFORM), mr813))
PKG_NAME:=ge8300-km-opencl
#GPU_BUILD_DIR:=$(LINUX_DIR)/modules/gpu/img-rgx/linux/rogue_km
GPU_BUILD_DIR:=$(LINUX_DIR)/modules/gpu/
ifeq ($(CPU_ARCH), arm)
GPU_KO_DIR:=$(GPU_BUILD_DIR)/img-rgx/linux/rogue_km/binary_sunxi_linux_release/target_armv7-a/pvrsrvkm.ko
else
GPU_KO_DIR:=$(GPU_BUILD_DIR)/img-rgx/linux/rogue_km/binary_sunxi_linux_nullws_release/target_aarch64/pvrsrvkm.ko
endif
endif

PKG_RELEASE:=2

include $(BUILD_DIR)/package.mk

define KernelPackage/ge8300-km-opencl
	SUBMENU:=Video Support
	TITLE:=GE8300 Device Driver
	DEPENDS:= +kmod-sunxi-disp
	FILES:=$(GPU_KO_DIR)
	AUTOLOAD:=$(call AutoLoad,20,pvrsrvkm,1)
	KCONFIG:=
endef

define Build/Configure
endef

define Build/Compile
	$(MAKE) -j -C $(GPU_BUILD_DIR) $(MAKE_OPTS)
endef

ifeq ($(TARGET_PLATFORM), $(filter $(TARGET_PLATFORM), mr813))
$(eval $(call KernelPackage,ge8300-km-opencl))
endif
