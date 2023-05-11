#
# Copyright (C) 2015-2016 Allwinner
#
# This is free software, licensed under the GNU General Public License v2.
# See /build/LICENSE for more information.
#
define KernelPackage/sunxi-vin
  SUBMENU:=$(VIDEO_MENU)
  TITLE:=sunxi-vin support
  FILES:=$(LINUX_DIR)/drivers/media/v4l2-core/videobuf2-core.ko
  FILES+=$(LINUX_DIR)/drivers/media/v4l2-core/videobuf2-memops.ko
  FILES+=$(LINUX_DIR)/drivers/media/v4l2-core/videobuf2-dma-contig.ko
  FILES+=$(LINUX_DIR)/drivers/media/v4l2-core/videobuf2-v4l2.ko
  FILES+=$(LINUX_DIR)/drivers/media/platform/sunxi-vin/vin_io.ko
  FILES+=$(LINUX_DIR)/drivers/media/platform/sunxi-vin/modules/sensor/imx278_mipi.ko
  FILES+=$(LINUX_DIR)/drivers/media/platform/sunxi-vin/modules/sensor/imx386_2lane_mipi.ko
  FILES+=$(LINUX_DIR)/drivers/media/platform/sunxi-vin/vin_v4l2.ko
  KCONFIG:=\
    CONFIG_V4L_PLATFORM_DRIVERS=y \
    CONFIG_MEDIA_CONTROLLER=y \
    CONFIG_MEDIA_SUBDRV_AUTOSELECT=y \
    CONFIG_VIDEO_SUNXI_VIN \
    CONFIG_CSI_VIN \
    CONFIG_CCI
  AUTOLOAD:=$(call AutoLoad,90,videobuf2-core videobuf2-memops videobuf2-dma-contig videobuf2-v4l2 vin_io imx278_mipi imx386_2lane_mipi vin_v4l2)
endef

define KernelPackage/sunxi-vin/description
  Kernel modules for sunxi-vin support
endef

$(eval $(call KernelPackage,sunxi-vin))

define KernelPackage/sunxi-uvc
  SUBMENU:=$(VIDEO_MENU)
  TITLE:=sunxi-uvc support
  FILES:=$(LINUX_DIR)/drivers/media/v4l2-core/videobuf2-core.ko
  FILES+=$(LINUX_DIR)/drivers/media/v4l2-core/videobuf2-v4l2.ko
  FILES+=$(LINUX_DIR)/drivers/media/v4l2-core/videobuf2-memops.ko
  FILES+=$(LINUX_DIR)/drivers/media/v4l2-core/videobuf2-vmalloc.ko
  FILES+=$(LINUX_DIR)/drivers/media/usb/uvc/uvcvideo.ko
  KCONFIG:= \
    CONFIG_MEDIA_USB_SUPPORT=y \
    CONFIG_USB_VIDEO_CLASS \
    CONFIG_USB_VIDEO_CLASS_INPUT_EVDEV
  AUTOLOAD:=$(call AutoLoad,95,videobuf2-core videobuf2-v4l2 videobuf2-memops videobuf2_vmalloc uvcvideo)
endef

define KernelPackage/sunxi-uvc/description
  Kernel modules for sunxi-uvc support
endef

$(eval $(call KernelPackage,sunxi-uvc))

define KernelPackage/sunxi-sound
  SUBMENU:=$(SOUND_MENU)
  DEPENDS:=+kmod-sound-core
  TITLE:=sun8iw18 sound support
  KCONFIG:= \
	CONFIG_SND_SUNXI_SOC=y \
	CONFIG_SND_SUNXI_SOC_DAUDIO \
	CONFIG_SND_SUNXI_SOC_RWFUNC=y \
	CONFIG_SND_SUN8IW18_CODEC \
	CONFIG_SND_SUNXI_SOC_SUN8IW18_CODEC \
	CONFIG_SND_SUNXI_SOC_SUNXI_DAUDIO \
	CONFIG_SUNXI_AUDIO_DEBUG=n

  FILES:=$(LINUX_DIR)/sound/soc/sunxi/sun8iw18-sndcodec.ko
  FILES+=$(LINUX_DIR)/sound/soc/sunxi/sun8iw18-codec.ko
  FILES+=$(LINUX_DIR)/sound/soc/sunxi/sunxi-snddaudio.ko
  FILES+=$(LINUX_DIR)/sound/soc/sunxi/snd-soc-sunxi-daudio.ko
  AUTOLOAD:=$(call AutoLoad,30,sun8iw18-sndcodec sun8iw18-codec sunxi-snddaudio snd-soc-sunxi-daudio)
endef

define KernelPackage/sunxi-sound/description
  Kernel modules for sun8iw18-sound support
endef

$(eval $(call KernelPackage,sunxi-sound))

define KernelPackage/leds-sunxi
  SUBMENU:=$(LEDS_MENU)
  TITLE:=leds-sunxi support
  FILES:=$(LINUX_DIR)/drivers/leds/leds-sunxi.ko
  KCONFIG:=CONFIG_LEDS_SUNXI
  AUTOLOAD:=$(call AutoLoad,60,leds-sunxi)
endef

define KernelPackage/leds-sunxi/description
  Kernel modules for leds sunxi support
endef

$(eval $(call KernelPackage,leds-sunxi))

define KernelPackage/ledtrig-doubleflash
  SUBMENU:=$(LEDS_MENU)
  TITLE:=LED DoubleFlash Trigger
  KCONFIG:=CONFIG_LEDS_TRIGGER_DOUBLEFLASH
  FILES:=$(LINUX_DIR)/drivers/leds/ledtrig-doubleflash.ko
  AUTOLOAD:=$(call AutoLoad,50,ledtrig-doubleflash)
endef

define KernelPackage/ledtrig-doubleflash/description
 Kernel module that allows LEDs to be controlled by a programmable doubleflash
 via sysfs
endef

$(eval $(call KernelPackage,ledtrig-doubleflash))

define KernelPackage/net-xr829
  SUBMENU:=$(WIRELESS_MENU)
  TITLE:=xr829 support (staging)
  DEPENDS:=
  FILES:=$(LINUX_DIR)/drivers/net/wireless/xr829/wlan/xradio_core.ko
  FILES+=$(LINUX_DIR)/drivers/net/wireless/xr829/wlan/xradio_wlan.ko
  FILES+=$(LINUX_DIR)/drivers/net/wireless/xr829/umac/xradio_mac.ko
  AUTOLOAD:=$(call AutoProbe, xradio_mac xradio_core xradio_wlan)
# FILES+=$(LINUX_DIR)/drivers/net/wireless/xr829/xr829.ko
# AUTOLOAD:=$(call AutoProbe, xr829)
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
