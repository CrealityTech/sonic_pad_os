#
# Copyright (C) 2013-2016 OpenWrt.org
#
# This is free software, licensed under the GNU General Public License v2.
# See /LICENSE for more information.
#
include $(TOPDIR)/rules.mk
include $(BUILD_DIR)/image.mk
include $(BUILD_DIR)/host.mk

FAT32_BLOCK_SIZE=1024
FAT32_BLOCKS=$(shell echo $$(($(CONFIG_SUNXI_SD_BOOT_PARTSIZE)*1024*1024/$(FAT32_BLOCK_SIZE))))

ifeq ($(CONFIG_TARGET_ROOTFS_INITRAMFS),)
define Image/BuildKernel
	mkimage -A arm -O linux -T kernel -C none \
	-a 0x41000000 -e 0x41000000 \
	-n 'AW Tina Linux-$(LINUX_VERSION)' \
	-d $(KDIR)/Image.gz $(TARGET_OUT_DIR)/$(IMG_PREFIX)-uImage

	rm -f $(TARGET_OUT_DIR)/boot.img
	mkdosfs $(TARGET_OUT_DIR)/boot.img -C $(FAT32_BLOCKS)

	mcopy -i $(TARGET_OUT_DIR)/boot.img $(TARGET_OUT_DIR)/$(IMG_PREFIX)-uImage ::uImage

endef
else
define Image/BuildKernel
	$(CP) $(KDIR)/Image.gz-initramfs $(TARGET_OUT_DIR)/$(IMG_PREFIX)-Image.gz-initramfs
	echo -ne '\x00\x00\x00\x00' >> $(TARGET_OUT_DIR)/$(IMG_PREFIX)-Image.gz-initramfs
	mkimage -A arm64 -O linux -T kernel -C none \
	-a 0x41000000 -e 0x41000000 \
	-n 'AW Tina Linux-$(LINUX_VERSION)' \
	-d $(TARGET_OUT_DIR)/$(IMG_PREFIX)-Image.gz-initramfs $(TARGET_OUT_DIR)/$(IMG_PREFIX)-uImage-initramfs
	rm -f $(TARGET_OUT_DIR)/boot.img
	mkdosfs $(TARGET_OUT_DIR)/boot.img -C $(FAT32_BLOCKS)

	mcopy -i $(TARGET_OUT_DIR)/boot.img $(TARGET_OUT_DIR)/$(IMG_PREFIX)-uImage-initramfs ::uImage
endef
endif

define Image/Build
	$(call Image/Build/$(1),$(1))
	dd if=$(KDIR)/root.$(1) of=$(TARGET_OUT_DIR)/rootfs.img bs=128k conv=sync
endef

$(eval $(call BuildImage))
