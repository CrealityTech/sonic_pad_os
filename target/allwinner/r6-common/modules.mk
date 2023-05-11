#
# Copyright (C) 2015-2016 Allwinner
#
# This is free software, licensed under the GNU General Public License v2.
# See /build/LICENSE for more information.

define KernelPackage/sunxi-vfe
  SUBMENU:=$(VIDEO_MENU)
  TITLE:=sunxi-vfe support
  FILES:=$(LINUX_DIR)/drivers/media/v4l2-core/videobuf2-core.ko
  FILES+=$(LINUX_DIR)/drivers/media/v4l2-core/videobuf2-dma-contig.ko
  FILES+=$(LINUX_DIR)/drivers/media/v4l2-core/videobuf2-memops.ko
  FILES+=$(LINUX_DIR)/drivers/media/platform/sunxi-vfe/vfe_io.ko
  FILES+=$(LINUX_DIR)/drivers/media/platform/sunxi-vfe/device/nt99141.ko
  FILES+=$(LINUX_DIR)/drivers/media/platform/sunxi-vfe/vfe_v4l2.ko
  AUTOLOAD:=$(call AutoLoad,90,videobuf2-core videobuf2-dma-contig videobuf2-memops vfe_io nt99141 vfe_v4l2)
endef

define KernelPackage/sunxi-vfe/description
  Kernel modules for sunxi-vfe support
endef

$(eval $(call KernelPackage,sunxi-vfe))

define KernelPackage/sunxi-sound
  SUBMENU:=$(SOUND_MENU)
  TITLE:=sun3iw1 sound support
  KCONFIG:= \
	CONFIG_SND_SUNXI_SOC \
	CONFIG_SND_SUNXI_SOC_CODEC_CPU_DAI \
	CONFIG_SND_SUNXI_SOC_INTERNAL_SUN3IW1_AUDIOCODEC \
	CONFIG_SND_SUNXI_SOC_RW \
	CONFIG_SND_SUNXI_SOC_SUN3IW1_AUDIO_CODEC_MACHINE
  FILES:=$(LINUX_DIR)/sound/soc/sunxi/sunxi_cpudai.ko
  FILES+=$(LINUX_DIR)/sound/soc/sunxi/sun3iw1_codec.ko
  FILES+=$(LINUX_DIR)/sound/soc/sunxi/sun3iw1_sndcodec.ko
  AUTOLOAD:=$(call AutoLoad,30,sunxi_cpudai sun3iw1_codec sun3iw1_sndcodec)
endef

define KernelPackage/sunxi-sound/description
  Kernel modules for sun3iw1-sound support
endef

$(eval $(call KernelPackage,sunxi-sound))
