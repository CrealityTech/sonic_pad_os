#
# Copyright (C) 2015-2016 Allwinner
#
# This is free software, licensed under the GNU General Public License v2.
# See /build/LICENSE for more information.
define KernelPackage/sunxi-vin
  SUBMENU:=$(VIDEO_MENU)
  TITLE:=sunxi-vin support
  FILES:=$(LINUX_DIR)/drivers/media/v4l2-core/videobuf2-core.ko
  FILES+=$(LINUX_DIR)/drivers/media/v4l2-core/videobuf2-memops.ko
  FILES+=$(LINUX_DIR)/drivers/media/v4l2-core/videobuf2-dma-contig.ko
  FILES+=$(LINUX_DIR)/drivers/media/v4l2-core/videobuf2-v4l2.ko
  FILES+=$(LINUX_DIR)/drivers/media/platform/sunxi-vin/vin_io.ko
  FILES+=$(LINUX_DIR)/drivers/media/platform/sunxi-vin/modules/sensor/ov5640_v1.ko
  FILES+=$(LINUX_DIR)/drivers/media/platform/sunxi-vin/vin_v4l2.ko
  KCONFIG:=\
    CONFIG_V4L_PLATFORM_DRIVERS=y \
    CONFIG_MEDIA_CONTROLLER=y \
    CONFIG_MEDIA_SUBDRV_AUTOSELECT=y \
    CONFIG_VIDEO_SUNXI_VIN \
    CONFIG_CSI_VIN \
    CONFIG_CCI
  AUTOLOAD:=$(call AutoLoad,90,videobuf2-core videobuf2-memops videobuf2-dma-contig videobuf2-v4l2 vin_io ov5640_v1 vin_v4l2)
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

define KernelPackage/sunxi-disp
  SUBMENU:=$(VIDEO_MENU)
  TITLE:=sunxi-disp support
ifeq ($(KERNEL),4.9)
  FILES+=$(LINUX_DIR)/drivers/video/fbdev/sunxi/disp2/disp/disp.ko
else
  FILES+=$(LINUX_DIR)/drivers/video/fbdev/sunxi/disp2/disp/disp.ko
endif
  AUTOLOAD:=$(call AutoLoad,10,disp,1)
endef

define KernelPackage/sunxi-disp/description
  Kernel modules for sunxi-disp support
endef

$(eval $(call KernelPackage,sunxi-disp))

define KernelPackage/sunxi-tv
  SUBMENU:=$(VIDEO_MENU)
  TITLE:=sunxi-tv support
  DEPENDS:=+kmod-sunxi-disp
ifeq ($(KERNEL),4.9)
  FILES+=$(LINUX_DIR)/drivers/video/fbdev/sunxi/disp2/tv/tv.ko
else
  FILES+=$(LINUX_DIR)/drivers/video/sunxi/disp2/tv/tv.ko
endif
  AUTOLOAD:=$(call AutoLoad,15,tv)
endef

define KernelPackage/sunxi-tv/description
  Kernel modules for sunxi-tv support
endef

$(eval $(call KernelPackage,sunxi-tv))

define KernelPackage/sunxi-hdmi
  SUBMENU:=$(VIDEO_MENU)
  TITLE:=sunxi-hdmi support
  DEPENDS:=+kmod-sunxi-disp
ifeq ($(KERNEL),4.9)
  FILES+=$(LINUX_DIR)/drivers/video/fbdev/sunxi/disp2/hdmi/hdmi.ko
else
  FILES+=$(LINUX_DIR)/drivers/video/sunxi/disp2/hdmi/hdmi.ko
endif
  AUTOLOAD:=$(call AutoLoad,15,hdmi,1)
endef

define KernelPackage/sunxi-hdmi/description
  Kernel modules for sunxi-disp support
endef

$(eval $(call KernelPackage,sunxi-hdmi))

define KernelPackage/net-broadcom
  SUBMENU:=$(WIRELESS_MENU)
  TITLE:=broadcom(ap6212/ap6335/ap6255...) support
  DEPENDS:=@LINUX_3_10
  FILES:=$(LINUX_DIR)/drivers/net/wireless/bcmdhd/bcmdhd.ko
  AUTOLOAD:=$(call AutoProbe,bcmdhd,1)
endef

define KernelPackage/net-broadcom/description
 Kernel modules for Broadcom AP6212/AP6335/AP6255...  support
endef

$(eval $(call KernelPackage,net-broadcom))
