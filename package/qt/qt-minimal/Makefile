#
# Copyright (C) 2007-2016 OpenWrt.org
#
# This is free software, licensed under the GNU General Public License v2.
# See /LICENSE for more information.
#

include $(TOPDIR)/rules.mk

PKG_NAME:=qt-minimal
PKG_VERSION:=1.0
PKG_RELEASE:=1

PKG_BUILD_DIR:=$(COMPILE_DIR)/$(PKG_NAME)
HAVE_SOURCE := yes

include $(BUILD_DIR)/package.mk
include $(BUILD_DIR)/cmake.mk

define Package/$(PKG_NAME)
  SECTION:=Qt
  CATEGORY:=Qt
  DEPENDS:= +qt5-widgets +qt5-gui +qt5-webengine +qt5-webview
  TITLE:=qt-minimal
  MAINTAINER:=anruliu<anruliu@allwinnertech.com>
endef

define Package/$(PKG_NAME)/description
  The qt browser application for Demo
endef

define Build/Prepare
	mkdir -p $(PKG_BUILD_DIR)/
	$(CP) -r ./src/* $(PKG_BUILD_DIR)/
endef

define Build/Compile
endef

define Package/$(PKG_NAME)/install
	$(INSTALL_DIR) $(1)/usr/bin
	$(INSTALL_DIR) $(1)/etc/init.d
	$(CP) $(PKG_BUILD_DIR)/qt-minimal $(1)/usr/bin
	$(CP) ./files/browser.init $(1)/etc/init.d/browser
	$(INSTALL_DIR) $(1)/etc/config
	$(CP) ./files/browser.config $(1)/etc/config/browser
	$(INSTALL_DIR) $(1)/usr/share/script/
	$(CP) ./files/rotate.sh $(1)/usr/share/script/
endef

$(eval $(call BuildPackage,$(PKG_NAME)))
