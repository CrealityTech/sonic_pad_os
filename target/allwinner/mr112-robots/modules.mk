#
# Copyright (C) 2015-2016 Allwinner
#
# This is free software, licensed under the GNU General Public License v2.
# See /build/LICENSE for more information.

define KernelPackage/net-rtl8723ds
  SUBMENU:=$(WIRELESS_MENU)
  TITLE:=RTL8723DS support (staging)
  DEPENDS:= +r8723ds-firmware
  FILES:=$(LINUX_DIR)/drivers/net/wireless/rtl8723ds/8723ds.ko
  AUTOLOAD:=$(call AutoProbe,8723ds)
endef

define KernelPackage/net-rtl8723ds/description
 Kernel modules for RealTek RTL8723DS support
endef

$(eval $(call KernelPackage,net-rtl8723ds))

define KernelPackage/net-rtl8189fs
  SUBMENU:=$(WIRELESS_MENU)
  TITLE:=RTL8189FS support (staging)
  DEPENDS:=
  FILES:=$(LINUX_DIR)/drivers/net/wireless/rtl8189fs/8189fs.ko
  AUTOLOAD:=$(call AutoProbe,8189fs)
endef

define KernelPackage/net-rtl8189fs/description
 Kernel modules for RealTek RTL8189FS support
endef

$(eval $(call KernelPackage,net-rtl8189fs))

define KernelPackage/net-xr829
  SUBMENU:=$(WIRELESS_MENU)
  TITLE:=xr829 support (staging)
  DEPENDS:=
  FILES:=$(LINUX_DIR)/drivers/net/wireless/xr829/wlan/xradio_core.ko
  FILES+=$(LINUX_DIR)/drivers/net/wireless/xr829/wlan/xradio_wlan.ko
  FILES+=$(LINUX_DIR)/drivers/net/wireless/xr829/umac/xradio_mac.ko
  AUTOLOAD:=$(call AutoProbe, xradio_mac xradio_core xradio_wlan)
endef

define KernelPackage/net-xr829/description
 Kernel modules for xr829 support
endef

$(eval $(call KernelPackage,net-xr829))

define KernelPackage/net-xrbtlpm
  SUBMENU:=$(WIRELESS_MENU)
  TITLE:=xradio bt lpm support (staging)
  DEPENDS:=
  FILES:=$(LINUX_DIR)/drivers/bluetooth/xradio_btlpm.ko
  AUTOLOAD:=$(call AutoProbe,xradio_btlpm)
endef

define KernelPackage/net-xrbtlpm/description
 Kernel modules for xradio bt lpm support
endef

$(eval $(call KernelPackage,net-xrbtlpm))
