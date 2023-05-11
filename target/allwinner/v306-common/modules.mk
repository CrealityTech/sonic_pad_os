#
# Copyright (C) 2015-2016 Allwinner
#
# This is free software, licensed under the GNU General Public License v2.
# See /build/LICENSE for more information.
#
define KernelPackage/sunxi-vfe
  SUBMENU:=$(VIDEO_MENU)
  TITLE:=sunxi-vfe support
  FILES:=$(LINUX_DIR)/drivers/media/v4l2-core/videobuf2-core.ko
  FILES+=$(LINUX_DIR)/drivers/media/v4l2-core/videobuf2-memops.ko
  FILES+=$(LINUX_DIR)/drivers/media/v4l2-core/videobuf2-dma-contig.ko
  FILES+=$(LINUX_DIR)/drivers/media/v4l2-core/videobuf2-v4l2.ko
  FILES+=$(LINUX_DIR)/drivers/media/platform/sunxi-vfe/vfe_io.ko
  FILES+=$(LINUX_DIR)/drivers/media/platform/sunxi-vfe/device/gc1034_mipi.ko
  FILES+=$(LINUX_DIR)/drivers/media/platform/sunxi-vfe/vfe_v4l2.ko
  KCONFIG:=\
    CONFIG_VIDEO_SUNXI_VFE \
    CONFIG_CSI_VFE \
    CONFIG_CCI=m
  AUTOLOAD:=$(call AutoLoad,90,videobuf2-core videobuf2-memops videobuf2-dma-contig videobuf2-v4l2 vfe_io gc1034_mipi vfe_v4l2)
endef

define KernelPackage/sunxi-vfe/description
  Kernel modules for sunxi-vfe support
endef

$(eval $(call KernelPackage,sunxi-vfe))

define KernelPackage/sunxi-sound
  SUBMENU:=$(SOUND_MENU)
  DEPENDS:=+kmod-sound-core
  TITLE:=sun8iw8 sound support
  FILES:=$(LINUX_DIR)/sound/soc/sunxi/sun8iw8/sun8iw8_sndcodec_new.ko
  FILES+=$(LINUX_DIR)/sound/soc/sunxi/sun8iw8/sunxi_codec.ko
  FILES+=$(LINUX_DIR)/sound/soc/sunxi/sun8iw8/sunxi_codecdma.ko
  FILES+=$(LINUX_DIR)/sound/soc/sunxi/sun8iw8/sunxi_sndcodec.ko
  AUTOLOAD:=$(call AutoLoad,30,sun8iw8_sndcodec_new sunxi_codec sunxi_codecdma sunxi_sndcodec)
endef

define KernelPackage/sunxi-sound/description
  Kernel modules for sun8iw8-sound support
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
