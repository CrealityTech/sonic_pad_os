#
# Copyright (C) 2015-2016 Allwinner
#
# This is free software, licensed under the GNU General Public License v2.
# See /build/LICENSE for more information.

define KernelPackage/net-broadcom
  SUBMENU:=$(WIRELESS_MENU)
  TITLE:=broadcom wifi driver support
  DEPENDS:=@TARGET_r16_parrot
  FILES:=$(LINUX_DIR)/drivers/net/wireless/bcmdhd/bcmdhd.ko
  AUTOLOAD:=$(call AutoProbe,bcmdhd)
endef

define KernelPackage/net-broadcom/description
 Kernel modules for Broadcom AP6212/AP6212A/AP6255/AP6335/AP6356S  support
endef

$(eval $(call KernelPackage,net-broadcom))
