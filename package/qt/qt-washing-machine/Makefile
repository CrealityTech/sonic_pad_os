#
# Copyright (C) 2007-2016 OpenWrt.org
#
# This is free software, licensed under the GNU General Public License v2.
# See /LICENSE for more information.
#

include $(TOPDIR)/rules.mk

PKG_NAME:=qt-washing-machine
PKG_VERSION:=1.0
PKG_RELEASE:=1

PKG_BUILD_DIR:=$(COMPILE_DIR)/$(PKG_NAME)
HAVE_SOURCE := yes

include $(BUILD_DIR)/package.mk
include $(BUILD_DIR)/cmake.mk

define Package/$(PKG_NAME)
  SECTION:=Qt
  CATEGORY:=Qt
  DEPENDS:= +qt5-gui +qt5-core +qt5-qml +qt5-quick-2 +qt5-quick-window +qt5-drivers-touchscreen +qt5-quick-controls-2
  TITLE:=qt-washing-machine
  MAINTAINER:=anruliu<anruliu@allwinnertech.com>
endef

define Package/$(PKG_NAME)/description
  The qt washing-machine application for Demo
endef

define Build/Prepare
	mkdir -p $(PKG_BUILD_DIR)/
	$(CP) -r ./src/* $(PKG_BUILD_DIR)/
endef

define Package/$(PKG_NAME)/install
	$(INSTALL_DIR) $(1)/usr/bin
	$(CP) $(PKG_BUILD_DIR)/qt-washing-machine $(1)/usr/bin

#       add setup env
	$(INSTALL_DIR) $(1)/etc/profile.d
	$(INSTALL_BIN) ./files/qtsetupenv.sh $(1)/etc/profile.d

	$(INSTALL_DIR) $(1)/usr/share/fonts
	$(INSTALL_BIN) ./files/fzcircle.ttf $(1)/usr/share/fonts
endef

$(eval $(call BuildPackage,$(PKG_NAME)))
