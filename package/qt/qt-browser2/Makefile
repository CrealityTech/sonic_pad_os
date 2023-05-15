#
# Copyright (C) 2007-2016 OpenWrt.org
#
# This is free software, licensed under the GNU General Public License v2.
# See /LICENSE for more information.
#

include $(TOPDIR)/rules.mk
include $(BUILD_DIR)/package.mk

PKG_NAME:=qt-browser2
PKG_SOURCE:=${PKG_NAME}-prebuilt.tar.bz2
PKG_RELEASE:=1

define Package/$(PKG_NAME)
  SECTION:=Qt
  CATEGORY:=Qt
  DEPENDS:= +qt5-widgets +qt5-gui +qt5-webengine +qt5-webview +qt5-concurrent \
            +qt5-drivers-virtualkeyboard +qt5-quick-virtualkeyboard +qt5-virtualkeyboard \
            +qt5-quick-2 +qt5-quick-controls +qt5-quick-controls-2 +qt5-quick-window +qt5-quick-layouts \
            +qt5-drivers-imageformats +opencv
  TITLE:=qt-browser
endef

define Package/$(PKG_NAME)/description
  The qt browser application
endef

define Build/Prepare
	rm -rf $(PKG_BUILD_DIR)/
	mkdir -p $(PKG_BUILD_DIR)/
	$(TAR) -xjf $(DL_DIR)/$(PKG_SOURCE) -C $(PKG_BUILD_DIR)
endef

define Build/Compile

endef

define Package/$(PKG_NAME)/install
	$(CP) -uvd $(PKG_BUILD_DIR)/etc/ $(1)/
	$(CP) -uvd $(PKG_BUILD_DIR)/usr/ $(1)/
endef

$(eval $(call BuildPackage,$(PKG_NAME)))
