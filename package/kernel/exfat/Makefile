#
# This is free software, licensed under the GNU General Public License v2.
# See /LICENSE for more information.
#

include $(TOPDIR)/rules.mk
include $(BUILD_DIR)/kernel.mk

PKG_NAME:=exfat
PKG_VERSION:=5.10.1
PKG_RELEASE:=1

PKG_SOURCE:=$(PKG_NAME)-$(PKG_VERSION).tar.gz
PKG_SOURCE_URL:=https://codeload.github.com/namjaejeon/linux-exfat-oot/tar.gz/$(PKG_VERSION)?
PKG_HASH:=0ff77dd7d39eb231d00c3c4909b9fad31ebeeb618bd6fa18fce142becc9c1f98
PKG_BUILD_DIR:=$(KERNEL_BUILD_DIR)/linux-exfat-oot-$(PKG_VERSION)

PKG_MAINTAINER:=
PKG_LICENSE:=GPL-2.0-only

include $(BUILD_DIR)/package.mk

define KernelPackage/fs-exfat
  SECTION:=kernel
  CATEGORY:=Kernel modules
  SUBMENU:=Filesystems
  TITLE:=exFAT kernel module
  URL:=https://github.com/namjaejeon/linux-exfat-oot
  FILES:=$(PKG_BUILD_DIR)/exfat.ko
  AUTOLOAD:=$(call AutoProbe,exfat)
  DEPENDS:= +kmod-nls-base
endef

define kernelPackage/exfat/description
  This package provides the kernel module for exfat.
endef
MAKE_OPTS:= \
	$(KERNEL_MAKE_FLAGS) \
        M="$(PKG_BUILD_DIR)" \
	ARCH="$(LINUX_KARCH)" \
        CROSS_COMPILE="$(TARGET_CROSS)" \
        SUBDIRS="$(PKG_BUILD_DIR)"

define Build/Compile
	$(MAKE) -C "$(LINUX_DIR)" \
	$(MAKE_OPTS) \
	EXTRA_CFLAGS="" \
	CONFIG_EXFAT_FS=m \
	modules
endef

$(eval $(call KernelPackage,fs-exfat))
