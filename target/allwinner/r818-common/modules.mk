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
  TITLE:=sun50iw10 sound support
  KCONFIG:= \
	CONFIG_SND_SUNXI_SOC=y \
	CONFIG_SND_SUNXI_SOC_RWFUNC=y \
	CONFIG_SND_SUN50IW10_CODEC \
	CONFIG_SND_SUNXI_SOC_SUN50IW10_CODEC \
	CONFIG_SUNXI_AUDIO_DEBUG=n

  FILES:=$(LINUX_DIR)/sound/soc/sunxi/sun50iw10-sndcodec.ko
  FILES+=$(LINUX_DIR)/sound/soc/sunxi/sun50iw10-codec.ko
  AUTOLOAD:=$(call AutoLoad,30,sun50iw10-sndcodec sun50iw10-codec)
endef

define KernelPackage/sunxi-sound/description
  Kernel modules for sun50iw10-sound support
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

define KernelPackage/net-rtl8723ds
  SUBMENU:=$(WIRELESS_MENU)
  TITLE:=RTL8723DS support (staging)
  DEPENDS:= +r8723ds-firmware +@IPV6 +@USES_REALTEK +@PACKAGE_realtek-rftest +@PACKAGE_rtk_hciattach
  FILES:=$(LINUX_DIR)/drivers/net/wireless/rtl8723ds/8723ds.ko
  KCONFIG:=\
	CONFIG_RTL8723DS=m \
	CONFIG_BT=y \
	CONFIG_BT_BREDR=y \
	CONFIG_BT_RFCOMM=y \
	CONFIG_BT_RFCOMM_TTY=y \
	CONFIG_BT_DEBUGFS=y \
	CONFIG_BT_HCIUART_RTL3WIRE=y \
	CONFIG_BT_HCIUART=y \
	CONFIG_BT_HCIUART_H4=y \
	CONFIG_HFP_OVER_PCM=y \
	CONFIG_RFKILL=y \
	CONFIG_RFKILL_PM=y \
	CONFIG_RFKILL_GPIO=y

  AUTOLOAD:=$(call AutoProbe,8723ds)
endef

define KernelPackage/net-rtl8723ds/description
 Kernel modules for RealTek RTL8723DS support
endef

$(eval $(call KernelPackage,net-rtl8723ds))

define KernelPackage/net-rtl8821cs
  SUBMENU:=$(WIRELESS_MENU)
  TITLE:=RTL8821CS support (staging)
  DEPENDS:= +rtl8821cs-firmware +@IPV6 +@USES_REALTEK +@PACKAGE_realtek-rftest +@PACKAGE_rtk_hciattach
  FILES:=$(LINUX_DIR)/drivers/net/wireless/rtl8821cs/8821cs.ko
  KCONFIG:=\
	CONFIG_RTL8821CS=m \
	CONFIG_BT=y \
	CONFIG_BT_BREDR=y \
	CONFIG_BT_RFCOMM=y \
	CONFIG_BT_RFCOMM_TTY=y \
	CONFIG_BT_DEBUGFS=y \
	CONFIG_BT_HCIUART_RTL3WIRE=y \
	CONFIG_BT_HCIUART=y \
	CONFIG_BT_HCIUART_H4=y \
	CONFIG_HFP_OVER_PCM=y \
	CONFIG_RFKILL=y \
	CONFIG_RFKILL_PM=y \
	CONFIG_RFKILL_GPIO=y
  AUTOLOAD:=$(call AutoProbe,8821cs)
endef

define KernelPackage/net-rtl8821cs/description
 Kernel modules for RealTek RTL8821CS support
endef

$(eval $(call KernelPackage,net-rtl8821cs))

define KernelPackage/net-xr829-40M
  SUBMENU:=$(WIRELESS_MENU)
  TITLE:=xr829 support (staging)
  DEPENDS:= +xr829-firmware +@IPV6 +@XR829_USE_40M_SDD +@USES_XRADIO +@PACKAGE_xr829-rftest
  FILES:=$(LINUX_DIR)/drivers/net/wireless/xr829/wlan/xradio_core.ko
  FILES+=$(LINUX_DIR)/drivers/net/wireless/xr829/wlan/xradio_wlan.ko
  FILES+=$(LINUX_DIR)/drivers/net/wireless/xr829/umac/xradio_mac.ko
  KCONFIG:=\
	CONFIG_XR829_WLAN=m \
	CONFIG_XRMAC=m \
	CONFIG_XRMAC_RC_MINSTREL=y \
	CONFIG_XRMAC_RC_MINSTREL_HT=y \
	CONFIG_XRMAC_RC_DEBUGFS=y \
	CONFIG_XRADIO=m \
	CONFIG_XRADIO_SDIO=y \
	CONFIG_XRADIO_NON_POWER_OF_TWO_BLOCKSIZES=y \
	CONFIG_XRADIO_USE_GPIO_IRQ=y \
	CONFIG_XRADIO_CFG80211=y \
	CONFIG_XRADIO_DEBUG=y \
	CONFIG_XRADIO_ETF=y \
	CONFIG_XRADIO_DEBUGFS=y \
	CONFIG_BT=y \
	CONFIG_BT_BREDR=y \
	CONFIG_BT_RFCOMM=y \
	CONFIG_BT_RFCOMM_TTY=y \
	CONFIG_BT_DEBUGFS=y \
	CONFIG_XR_BT_LPM=y \
	CONFIG_XR_BT_FDI=y \
	CONFIG_BT_HCIUART=y \
	CONFIG_BT_HCIUART_H4=y \
	CONFIG_HFP_OVER_PCM=y \
	CONFIG_RFKILL=y \
	CONFIG_RFKILL_PM=y \
	CONFIG_RFKILL_GPIO=y

  AUTOLOAD:=$(call AutoProbe, xradio_mac xradio_core xradio_wlan)
endef

define KernelPackage/net-xr829-40M/description
 Kernel modules for xr829 support
endef

$(eval $(call KernelPackage,net-xr829-40M))


define KernelPackage/net-xr829
  SUBMENU:=$(WIRELESS_MENU)
  TITLE:=xr829 support (staging)
  DEPENDS:= +xr829-firmware +@IPV6 +@USES_XRADIO +@PACKAGE_xr829-rftest +@PACKAGE_xr829-rftest
  FILES:=$(LINUX_DIR)/drivers/net/wireless/xr829/wlan/xr829_core.ko
  FILES+=$(LINUX_DIR)/drivers/net/wireless/xr829/wlan/xr829_wlan.ko
  FILES+=$(LINUX_DIR)/drivers/net/wireless/xr829/umac/xr829_mac.ko
  KCONFIG:=\
	CONFIG_XR829_WLAN=m \
	CONFIG_XRMAC=m \
	CONFIG_XRMAC_RC_MINSTREL=y \
	CONFIG_XRMAC_RC_MINSTREL_HT=y \
	CONFIG_XRMAC_RC_DEBUGFS=y \
	CONFIG_XRADIO=m \
	CONFIG_XRADIO_SDIO=y \
	CONFIG_XRADIO_NON_POWER_OF_TWO_BLOCKSIZES=y \
	CONFIG_XRADIO_USE_GPIO_IRQ=y \
	CONFIG_XRADIO_CFG80211=y \
	CONFIG_XRADIO_DEBUG=y \
	CONFIG_XRADIO_ETF=y \
	CONFIG_XRADIO_DEBUGFS=y \
	CONFIG_BT=y \
	CONFIG_BT_BREDR=y \
	CONFIG_BT_RFCOMM=y \
	CONFIG_BT_RFCOMM_TTY=y \
	CONFIG_BT_DEBUGFS=y \
	CONFIG_XR_BT_LPM=y \
	CONFIG_XR_BT_FDI=y \
	CONFIG_BT_HCIUART=y \
	CONFIG_BT_HCIUART_H4=y \
	CONFIG_HFP_OVER_PCM=y \
	CONFIG_RFKILL=y \
	CONFIG_RFKILL_PM=y \
	CONFIG_RFKILL_GPIO=y

# AUTOLOAD:=$(call AutoProbe, xr829_mac xr829_core xr829_wlan)
# FILES+=$(LINUX_DIR)/drivers/net/wireless/xr829/xr829.ko
# AUTOLOAD:=$(call AutoProbe, xr829)
endef

define KernelPackage/net-xr829/description
 Kernel modules for xr829 support
endef

$(eval $(call KernelPackage,net-xr829))

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

define KernelPackage/net-xr819
  SUBMENU:=$(WIRELESS_MENU)
  TITLE:=xr819 support (staging)
  DEPENDS:= +xr819-firmware +@IPV6
  FILES:=$(LINUX_DIR)/drivers/net/wireless/xr819/wlan/xr819_core.ko
  FILES+=$(LINUX_DIR)/drivers/net/wireless/xr819/wlan/xr819_wlan.ko
  FILES+=$(LINUX_DIR)/drivers/net/wireless/xr819/umac/xr819_mac.ko
# AUTOLOAD:=$(call AutoProbe, xr819_mac xr819_core xr819_wlan)
# FILES+=$(LINUX_DIR)/drivers/net/wireless/xr819/xr819.ko
# AUTOLOAD:=$(call AutoProbe, xr819)
endef

define KernelPackage/net-xr819/description
 Kernel modules for xr819 support
endef

$(eval $(call KernelPackage,net-xr819))
