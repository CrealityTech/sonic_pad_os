#
# Copyright (C) 2015-2016 Allwinner
#
# This is free software, licensed under the GNU General Public License v2.
# See /build/LICENSE for more information.
define KernelPackage/sunxi-vfe
  SUBMENU:=$(VIDEO_MENU)
  TITLE:=sunxi-vfe support
  FILES:=$(LINUX_DIR)/drivers/media/v4l2-core/videobuf2-core.ko
  FILES+=$(LINUX_DIR)/drivers/media/v4l2-core/videobuf2-memops.ko
  FILES+=$(LINUX_DIR)/drivers/media/v4l2-core/videobuf2-dma-contig.ko
  FILES+=$(LINUX_DIR)/drivers/media/v4l2-core/videobuf2-v4l2.ko
  FILES+=$(LINUX_DIR)/drivers/media/platform/sunxi-vfe/vfe_io.ko
  FILES+=$(LINUX_DIR)/drivers/media/platform/sunxi-vfe/device/ov5640.ko
  FILES+=$(LINUX_DIR)/drivers/media/platform/sunxi-vfe/vfe_v4l2.ko
  KCONFIG:=\
    CONFIG_VIDEO_SUNXI_VFE \
    CONFIG_CSI_VFE
  AUTOLOAD:=$(call AutoLoad,90,videobuf2-core videobuf2-memops videobuf2-dma-contig videobuf2-v4l2 vfe_io ov5640 vfe_v4l2)
endef

define KernelPackage/sunxi-vfe/description
  Kernel modules for sunxi-vfe support
endef

$(eval $(call KernelPackage,sunxi-vfe))

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

define KernelPackage/sunxi-drm
  SUBMENU:=$(VIDEO_MENU)
  TITLE:=sunxi-drm support
  DEPENDS:=
  KCONFIG:=\
    CONFIG_DRM=y \
    CONFIG_DRM_FBDEV_EMULATION=y \
    CONFIG_DRM_SUNXI=m \
    CONFIG_DRM_BRIDGE=y \
    CONFIG_DRM_KMS_FB_HELPER=y \
    CONFIG_DRM_KMS_HELPER=y \
    CONFIG_DRM_LOAD_EDID_FIRMWARE=n \
    CONFIG_DRM_VGEM=n \
    CONFIG_DRM_UDL=n \
    CONFIG_DRM_NXP_PTN3460=n \
    CONFIG_DRM_PARADE_PS8622=n \
    CONFIG_DRM_I2C_ADV7511=n \
    CONFIG_DRM_I2C_CH7006=n \
    CONFIG_DRM_I2C_SIL164=n \
    CONFIG_DRM_I2C_NXP_TDA998X=n \
    CONFIG_FB_CONSOLE_SUNXI=n \
    CONFIG_DISP2_SUNXI=n
  FILES+=$(LINUX_DIR)/drivers/gpu/drm/sunxi/sunxidrm.ko
  AUTOLOAD:=$(call AutoLoad,10,sunxidrm,1)
endef
define KernelPackage/sunxi-drm/description
  Kernel modules for sunxi-drm support
endef
$(eval $(call KernelPackage,sunxi-drm))

define KernelPackage/sunxi-disp
  SUBMENU:=$(VIDEO_MENU)
  TITLE:=sunxi-disp support
  KCONFIG:=\
    CONFIG_DISP2_SUNXI=m \
    CONFIG_DISP2_SUNXI_BOOT_COLORBAR=y \
    CONFIG_DISP2_SUNXI_DEBUG=y \
    CONFIG_HDMI_DISP2_SUNXI=m \
    CONFIG_TV_DISP2_SUNXI=n \
    CONFIG_FB=y \
    CONFIG_FB_CFB_COPYAREA=y \
    CONFIG_FB_CFB_FILLRECT=y \
    CONFIG_FB_CFB_IMAGEBLIT=y \
    CONFIG_FB_CMDLINE=y \
    CONFIG_FB_CONSOLE_SUNXI=y \
    CONFIG_FRAMEBUFFER_CONSOLE=n \
    CONFIG_SUNXI_TRANSFORM=y \
    CONFIG_VDPO_DISP2_SUNXI=n \
    CONFIG_EDP_DISP2_SUNXI=n \
    CONFIG_DISP2_SUNXI_COMPOSER=n \
    CONFIG_FRAMEBUFFER_CONSOLE_DETECT_PRIMARY=n \
    CONFIG_FRAMEBUFFER_CONSOLE_ROTATION=n \
    CONFIG_DRM=n
ifeq ($(KERNEL),4.4)
  FILES+=$(LINUX_DIR)/drivers/video/fbdev/sunxi/disp2/disp/disp.ko
else
  FILES+=$(LINUX_DIR)/drivers/video/sunxi/disp2/disp/disp.ko
endif
  AUTOLOAD:=$(call AutoLoad,10,disp,1)
endef

define KernelPackage/sunxi-disp/description
  Kernel modules for sunxi-disp support
endef

$(eval $(call KernelPackage,sunxi-disp))

define KernelPackage/sunxi-hdmi
  SUBMENU:=$(VIDEO_MENU)
  TITLE:=sunxi-hdmi support
  DEPENDS:=+kmod-sunxi-disp
ifeq ($(KERNEL),4.4)
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
