#
# Copyright (C) 2015-2016 Allwinner
#
# This is free software, licensed under the GNU General Public License v2.
# See /build/LICENSE for more information.
define KernelPackage/net-broadcom
  SUBMENU:=$(WIRELESS_MENU)
  TITLE:=broadcom(ap6212/ap6335/ap6255...) support
  FILES:=$(LINUX_DIR)/drivers/net/wireless/bcmdhd/bcmdhd.ko
  AUTOLOAD:=$(call AutoProbe,bcmdhd,1)
endef

define KernelPackage/net-broadcom/description
 Kernel modules for Broadcom AP6212/AP6335/AP6255...  support
endef

$(eval $(call KernelPackage,net-broadcom))

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

define KernelPackage/xradio-xr819
  SUBMENU:=$(WIRELESS_MENU)
  TITLE:=xr819 support
  DEPENDS:=+kmod-cfg80211
  FILES:=$(LINUX_DIR)/drivers/net/wireless/xr819/wlan/xradio_core.ko
  FILES+=$(LINUX_DIR)/drivers/net/wireless/xr819/wlan/xradio_wlan.ko
  FILES+=$(LINUX_DIR)/drivers/net/wireless/xr819/umac/xradio_mac.ko
  AUTOLOAD:=$(call AutoProbe, xradio_mac xradio_core xradio_wlan)
endef

define KernelPackage/xradio-xr819/description
 Kernel modules for Allwinnertech XR819  support
endef

$(eval $(call KernelPackage,xradio-xr819))

