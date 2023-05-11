#
# Copyright (C) 2015-2016 Allwinner
#
# This is free software, licensed under the GNU General Public License v2.
# See /build/LICENSE for more information.

#define KernelPackage/sunxi-vfe
#  SUBMENU:=$(VIDEO_MENU)
#  TITLE:=sunxi-vfe support
#  FILES:=$(LINUX_DIR)/drivers/media/v4l2-core/videobuf2-core.ko
#  FILES+=$(LINUX_DIR)/drivers/media/v4l2-core/videobuf2-memops.ko
#  FILES+=$(LINUX_DIR)/drivers/media/v4l2-core/videobuf2-dma-contig.ko
#  FILES+=$(LINUX_DIR)/drivers/media/platform/sunxi-vfe/vfe_io.ko
#  FILES+=$(LINUX_DIR)/drivers/media/platform/sunxi-vfe/device/gc0308.ko
#  FILES+=$(LINUX_DIR)/drivers/media/platform/sunxi-vfe/vfe_v4l2.ko
#  AUTOLOAD:=$(call AutoLoad,90,videobuf2-core videobuf2-memops videobuf2-dma-contig vfe_io gc0308 vfe_v4l2)
#endef
#
#define KernelPackage/sunxi-vfe/description
#  Kernel modules for sunxi-vfe support
#endef
#
#$(eval $(call KernelPackage,sunxi-vfe))
#

