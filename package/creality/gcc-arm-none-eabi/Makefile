#
# Copyright (C) 2011 OpenWrt.org
#
# This is free software, licensed under the GNU General Public License v2.
# See /LICENSE for more information.
#

include $(TOPDIR)/rules.mk
include ../version.mk

PKG_NAME:=gcc-arm-none-eabi
PKG_VERSION:=6-2022-q4-major
PKG_RELEASE:=1

PKG_SOURCE:=$(PKG_NAME)-$(PKG_VERSION)-linux.tar.bz2

include $(BUILD_DIR)/package.mk

define Package/$(PKG_NAME)
  TITLE:=$(PKG_NAME)
  SECTION:=base
  CATEGORY:=Creality
  DEPENDS:=+libstdcpp
endef

define Package/$(PKG_NAME)/description
  gcc-arm-none-eabi toolchain run on aarch64 TinaLinux.
endef

define Build/Prepare
	rm -rf $(PKG_BUILD_DIR)/
	mkdir -p $(PKG_BUILD_DIR)/
	$(TAR) -xjf $(DL_DIR)/$(PKG_SOURCE) -C $(PKG_BUILD_DIR)
endef

define Build/Compile

endef

define Package/$(PKG_NAME)/install
	$(INSTALL_DIR) $(1)/usr/share/
	$(CP) -uvd $(PKG_BUILD_DIR)/$(PKG_NAME)-$(PKG_VERSION) $(1)/usr/share/$(PKG_NAME)
endef

$(eval $(call BuildPackage,$(PKG_NAME)))
