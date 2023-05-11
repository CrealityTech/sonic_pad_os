#
# Copyright (C) 2015-2016 Allwinner
#
# This is free software, licensed under the GNU General Public License v2.
# See /build/LICENSE for more information.

define KernelPackage/sunxi-vfe
  SUBMENU:=$(VIDEO_MENU)
  TITLE:=sunxi-vfe support
  FILES:=$(LINUX_DIR)/drivers/media/video/videobuf-core.ko
  FILES+=$(LINUX_DIR)/drivers/media/video/videobuf-dma-contig.ko
  FILES+=$(LINUX_DIR)/drivers/media/video/sunxi-vfe/csi_cci/cci.ko
  FILES+=$(LINUX_DIR)/drivers/media/video/sunxi-vfe/vfe_os.ko
  FILES+=$(LINUX_DIR)/drivers/media/video/sunxi-vfe/vfe_subdev.ko
  FILES+=$(LINUX_DIR)/drivers/media/video/sunxi-vfe/vfe_v4l2.ko
  AUTOLOAD:=$(call AutoLoad,90,videobuf-core videobuf-dma-contig cci vfe_os vfe_subdev)
  AUTOLOAD:=$(call AutoLoad,95,vfe_v4l2)
endef

define KernelPackage/sunxi-vfe/description
  Kernel modules for sunxi-vfe support
endef

$(eval $(call KernelPackage,sunxi-vfe))


define AddDepends/sunxi
  SUBMENU:=$(VIDEO_MENU)
  DEPENDS+=kmod-sunxi-vfe $(1)
endef

define AddDepends/sensor
$(AddDepends/sunxi)
endef

define KernelPackage/sensor-gc0308
  TITLE:=gc0308 support
  FILES+=$(LINUX_DIR)/drivers/media/video/sunxi-vfe/device/gc0308.ko
  AUTOLOAD:=$(call AutoLoad,92,gc0308)
  $(call AddDepends/sensor)
endef

define KernelPackage/sensor-gc0308/description
  Kernel modules for supporting gc0308 camera sensor
endef

$(eval $(call KernelPackage,sensor-gc0308))

define KernelPackage/sensor-ov5640
  TITLE:=ov5640 support
  FILES+=$(LINUX_DIR)/drivers/media/video/sunxi-vfe/device/ov5640.ko
  AUTOLOAD:=$(call AutoLoad,92,ov5640)
  $(call AddDepends/sensor)
endef

define KernelPackage/sensor-ov5640/description
  Kernel modules for supporting ov5640 camera sensor
endef

$(eval $(call KernelPackage,sensor-ov5640))

define KernelPackage/sensor-imx179
  TITLE:=imx179 support
  FILES+=$(LINUX_DIR)/drivers/media/video/sunxi-vfe/device/imx179.ko
  AUTOLOAD:=$(call AutoLoad,92,imx179)
  $(call AddDepends/sensor)
endef

define KernelPackage/sensor-imx179/description
  Kernel modules for supporting imx179 camera sensor
endef

$(eval $(call KernelPackage,sensor-imx179))

 define KernelPackage/sunxi-uvc
SUBMENU:=$(VIDEO_MENU)
TITLE:=sunxi-uvc support
FILES:=$(LINUX_DIR)/drivers/media/video/videobuf2-core.ko
FILES+=$(LINUX_DIR)/drivers/media/video/videobuf2-memops.ko
FILES+=$(LINUX_DIR)/drivers/media/video/videobuf2-vmalloc.ko
FILES+=$(LINUX_DIR)/drivers/media/video/uvc/uvcvideo.ko
KCONFIG:= \
	CONFIG_USB_VIDEO_CLASS=y \
	CONFIG_USB_VIDEO_CLASS_INPUT_EVDEV=y
AUTOLOAD:=$(call AutoLoad,95,videobuf2-core videobuf2-memops videobuf2_vmalloc uvcvideo)
endef

define KernelPackage/sunxi-uvc/description
Kernel modules for sunxi-uvc support
endef

$(eval $(call KernelPackage,sunxi-uvc))


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

define KernelPackage/net-broadcom
  SUBMENU:=$(WIRELESS_MENU)
  TITLE:=broadcom(ap6212/ap6335/ap6255...) support
  DEPENDS:=@TARGET_r58_sch
  FILES:=$(LINUX_DIR)/drivers/net/wireless/bcmdhd/bcmdhd.ko
  AUTOLOAD:=$(call AutoProbe,bcmdhd,1)
endef

define KernelPackage/net-broadcom/description
 Kernel modules for Broadcom AP6212/AP6335/AP6255...  support
endef

$(eval $(call KernelPackage,net-broadcom))

define KernelPackage/sunxi-disp
  SUBMENU:=$(VIDEO_MENU)
  TITLE:=SUNXI-DISP support
  FILES:=$(LINUX_DIR)/drivers/video/sunxi/disp2/disp/disp.ko
  AUTOLOAD:=$(call AutoLoad,10,disp,1)
endef

define KernelPackage/sunxi-disp/description
 Kernel modules for sunxi-disp support
endef

$(eval $(call KernelPackage,sunxi-disp))

define KernelPackage/sunxi-hdmi
  SUBMENU:=$(VIDEO_MENU)
  TITLE:=SUNXI-LCD support
  DEPENDS:=+kmod-sunxi-disp
  FILES:=$(LINUX_DIR)/drivers/video/sunxi/disp2/hdmi/hdmi.ko
  AUTOLOAD:=$(call AutoLoad,15,hdmi,1)
endef

define KernelPackage/sunxi-hdmi/description
 Kernel modules for sunxi-hdmi support
endef

$(eval $(call KernelPackage,sunxi-hdmi))
