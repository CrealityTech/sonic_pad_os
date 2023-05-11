#
# Copyright (C) 2015-2016 Allwinner
#
# This is free software, licensed under the GNU General Public License v2.
# See /build/LICENSE for more information.

define KernelPackage/xenomai
  SUBMENU:=$(XENOMAI_MENU)
  TITLE:=xenomai support
  DEPENDS:=@PACKAGE_xenomai
  FILES:=$(LINUX_DIR)/drivers/xenomai/testing/xeno_klat.ko
  FILES+=$(LINUX_DIR)/drivers/xenomai/testing/xeno_rtdmtest.ko
  AUTOLOAD:=$(call AutoProbe, xeno_klat xeno_rtdmtest)
endef

define KernelPackage/xenomai/description
  Kernel modules for xenomai support
endef

$(eval $(call KernelPackage,xenomai))

define KernelPackage/touchscreen-atmel-mxt
  SUBMENU:=$(INPUT_MODULES_MENU)
  TITLE:=Atmel MXT  support
  DEPENDS:= +kmod-input-core
  KCONFIG:= \
	CONFIG_INPUT_TOUCHSCREEN \
	CONFIG_INPUT_TOUCHSCREEN_ATMEL_MXT
  FILES:=$(LINUX_DIR)/drivers/input/touchscreen/atmel_mxt_ts.ko
  AUTOLOAD:=$(call AutoProbe,atmel_mxt_ts.ko)
endef

define KernelPackage/touchscreen-atmel-mxt/description
 Enable support for Atmel MXT touchscreen port.
endef

$(eval $(call KernelPackage,touchscreen-atmel-mxt))

define KernelPackage/touchscreen-icn85xx
  SUBMENU:=$(INPUT_MODULES_MENU)
  TITLE:=Atmel MXT  support
  DEPENDS:= +kmod-input-core
  KCONFIG:= \
        CONFIG_INPUT_TOUCHSCREEN \
        CONFIG_INPUT_TOUCHSCREEN_ICN85XX
  FILES:=$(LINUX_DIR)/drivers/input/touchscreen/icn85xx/icn85xx_ts.ko
  AUTOLOAD:=$(call AutoProbe,icn85xx_ts.ko)
endef

define KernelPackage/touchscreen-icn85xx/description
 Enable support for Atmel MXT touchscreen port.
endef

$(eval $(call KernelPackage,touchscreen-icn85xx))

define KernelPackage/xradio-xr819
  SUBMENU:=$(WIRELESS_MENU)
  TITLE:=xr819 support
  DEPENDS:=+kmod-cfg80211
  FILES:=$(LINUX_DIR)/drivers/net/wireless/xradio/wlan/xradio_core.ko
  FILES+=$(LINUX_DIR)/drivers/net/wireless/xradio/wlan/xradio_wlan.ko
  FILES+=$(LINUX_DIR)/drivers/net/wireless/xradio/umac/xradio_mac.ko
  AUTOLOAD:=$(call AutoProbe, xradio_mac xradio_core xradio_wlan)
endef

define KernelPackage/xradio-xr819/description
 Kernel modules for Allwinnertech XR819  support
endef

$(eval $(call KernelPackage,xradio-xr819))

define KernelPackage/sunxi-uvc
  SUBMENU:=$(VIDEO_MENU)
  TITLE:=sunxi-uvc support
  DEPENDS:=@USB_SUPPORT +kmod-usb-core +kmod-input-core +kmod-dma-buf
  FILES:=$(LINUX_DIR)/drivers/media/video/videobuf2-core.ko
  FILES+=$(LINUX_DIR)/drivers/media/video/videobuf2-memops.ko
  FILES+=$(LINUX_DIR)/drivers/media/video/videobuf2-vmalloc.ko
  FILES+=$(LINUX_DIR)/drivers/media/video/uvc/uvcvideo.ko
  KCONFIG:= \
    CONFIG_V4L_USB_DRIVERS=y \
    CONFIG_VIDEOBUF2_CORE \
    CONFIG_VIDEOBUF2_MEMOPS \
    CONFIG_VIDEOBUF2_VMALLOC \
    CONFIG_USB_VIDEO_CLASS \
    CONFIG_MEDIA_USB_SUPPORT
  AUTOLOAD:=$(call AutoProbe,videobuf2-core videobuf2-memops videobuf2_vmalloc uvcvideo)
endef

define KernelPackage/sunxi-uvc/description
Kernel modules for sunxi-uvc support
endef

$(eval $(call KernelPackage,sunxi-uvc))
