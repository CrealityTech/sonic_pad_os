#
# Copyright (C) 2015-2016 Allwinner
#
# This is free software, licensed under the GNU General Public License v2.
# See /build/LICENSE for more information.

define KernelPackage/net-rtl8189fs
  SUBMENU:=$(WIRELESS_MENU)
  TITLE:=RTL8189FS support (staging)
  FILES:=$(LINUX_DIR)/drivers/net/wireless/rtl8189fs/8189fs.ko
  DEPENDS:=+@IPV6
  KCONFIG:=\
  CONFIG_RTL8189FS=m
  AUTOLOAD:=$(call AutoProbe,8189fs)
endef

define KernelPackage/net-rtl8189fs/description
 Kernel modules for RealTek RTL8189FS support
endef

$(eval $(call KernelPackage,net-rtl8189fs))

define KernelPackage/net-rtl8189es
  SUBMENU:=$(WIRELESS_MENU)
  TITLE:=RTL8189FS support (staging)
  FILES:=$(LINUX_DIR)/drivers/net/wireless/rtl8189es/8189es.ko
  DEPENDS:=+@IPV6
  KCONFIG:=\
  CONFIG_RTL8189ES=m
  AUTOLOAD:=$(call AutoProbe,8189es)
endef

define KernelPackage/net-rtl8189es/description
 Kernel modules for RealTek RTL8189ES support
endef

$(eval $(call KernelPackage,net-rtl8189es))
