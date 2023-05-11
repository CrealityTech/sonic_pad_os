#
# Copyright (C) 2015-2016 Allwinner
#
# This is free software, licensed under the GNU General Public License v2.
# See /build/LICENSE for more information.

define KernelPackage/net-rtl8189fs
  SUBMENU:=$(WIRELESS_MENU)
  TITLE:=RTL8189FS support (staging)
  DEPENDS:=
  FILES:=$(LINUX_DIR)/drivers/net/wireless/rtl8189fs/8189fs.ko
  AUTOLOAD:=$(call AutoProbe,8189fs)
endef

define KernelPackage/net-rtl8189es/description
 Kernel modules for RealTek RTL8189FS support
endef

$(eval $(call KernelPackage,net-rtl8189fs))
