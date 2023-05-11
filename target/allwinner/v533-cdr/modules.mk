#
# Copyright (C) 2015-2016 Allwinner
#
# This is free software, licensed under the GNU General Public License v2.
# See /build/LICENSE for more information.

define KernelPackage/vin-v4l2
  SUBMENU:=$(VIDEO_MENU)
  TITLE:=Video input support (staging)
  DEPENDS:=
  FILES:=$(LINUX_DIR)/drivers/media/v4l2-core/videobuf2-core.ko
  FILES+=$(LINUX_DIR)/drivers/media/v4l2-core/videobuf2-dma-contig.ko
  FILES+=$(LINUX_DIR)/drivers/media/v4l2-core/videobuf2-memops.ko
  FILES+=$(LINUX_DIR)/drivers/media/v4l2-core/videobuf2-v4l2.ko
  FILES+=$(LINUX_DIR)/drivers/media/platform/sunxi-vin/vin_io.ko
  FILES+=$(LINUX_DIR)/drivers/media/platform/sunxi-vin/modules/sensor/gc2053_mipi.ko
  FILES+=$(LINUX_DIR)/drivers/media/platform/sunxi-vin/modules/sensor/tp9950.ko
  FILES+=$(LINUX_DIR)/drivers/media/platform/sunxi-vin/vin_v4l2.ko
  AUTOLOAD:=$(call AutoProbe,videobuf2-core videobuf2-dma-contig videobuf2-memops videobuf2-v4l2 vin_io gc2053_mipi tp9950 vin_v4l2)
endef

define KernelPackage/vin_v4l2/description
 Kernel modules for video input support
endef

$(eval $(call KernelPackage,vin-v4l2))

define KernelPackage/EISE-ISE
  SUBMENU:=$(VIDEO_MENU)
  TITLE:=Video ISE&EISE support (staging)
  DEPENDS:=
  UTOLOAD:=$(call AutoProbe,sunxi_ise sunxi_eise)
endef

define KernelPackage/EISE-ISE/description
 Kernel modules for video ISE&EISE support
endef

$(eval $(call KernelPackage,EISE-ISE))

define KernelPackage/sunxi-rf-wlan
  SUBMENU:=$(WIRELESS_MENU)
  TITLE:=sunxi rfkill wlan support (staging)
  DEPENDS:=
  KCONFIG:= CONFIG_RFKILL \
	  CONFIG_SUNXI_RFKILL
  FILES:=$(LINUX_DIR)/drivers/misc/sunxi-rf/sunxi-wlan.ko
  AUTOLOAD:=$(call AutoProbe, sunxi-rf-wlan)
endef

define KernelPackage/sunxi-rf-wlan/description
 Kernel modules for sunx-wlan support
endef

$(eval $(call KernelPackage,sunxi-rf-wlan))

define KernelPackage/net-xr819
  SUBMENU:=$(WIRELESS_MENU)
  TITLE:=xr819 support (staging)
  DEPENDS:= +kmod-sunxi-rf-wlan +kmod-cfg80211
  FILES:=$(LINUX_DIR)/drivers/net/wireless/xr819/wlan/xradio_core.ko
  FILES+=$(LINUX_DIR)/drivers/net/wireless/xr819/wlan/xradio_wlan.ko
  FILES+=$(LINUX_DIR)/drivers/net/wireless/xr819/umac/xradio_mac.ko
  AUTOLOAD:=$(call AutoProbe, xradio_mac xradio_core xradio_wlan)
endef

define KernelPackage/net-xr819/description
 Kernel modules for xr819 support
endef

$(eval $(call KernelPackage,net-xr819))

define KernelPackage/hmac
  SUBMENU:=$(WIRELESS_MENU)
  TITLE:=hmac support (staging)
  DEPENDS:=
  FILES:=$(LINUX_DIR)/crypto/hmac.ko
  AUTOLOAD:=$(call AutoProbe,hmac)
endef

define KernelPackage/hmac/description
 Kernel modules for hmac support
endef

$(eval $(call KernelPackage,hmac))

define KernelPackage/da380
  SUBMENU:=$(INPUT_MODULES_MENU)
  TITLE:=G-sensor da380 support
  KCONFIG:= CONFIG_SENSORS_DA380
  DEPENDS:= +kmod-input-core +kmod-input-evdev
  FILES+=$(LINUX_DIR)/drivers/input/sensor/da380/da380.ko
  AUTOLOAD:=$(call AutoProbe,da380)
endef

define KernelPackage/da380/description
 Support G-sensor DA380 module
endef

$(eval $(call KernelPackage,da380))
