#
# Copyright (C) 2011 OpenWrt.org
#
# This is free software, licensed under the GNU General Public License v2.
# See /LICENSE for more information.
#

include $(TOPDIR)/rules.mk
include ../version.mk

PKG_NAME:=mainsail
PKG_VERSION:=1.0.5_mainsail
PKG_RELEASE:=1

PKG_SOURCE:=$(PKG_NAME)-$(PKG_VERSION).tar.gz
PKG_SOURCE_SUBDIR:=$(PKG_NAME)-$(PKG_VERSION)/$(PKG_PROJECT_NAME)
PKG_SOURCE_PROTO:=git
PKG_SOURCE_VERSION:=V$(PKG_VERSION)

include $(BUILD_DIR)/package.mk

define Package/$(PKG_NAME)
  TITLE:=mainsail, the klipper UI
  SECTION:=base
  CATEGORY:=Creality
  DEPENDS:=+moonraker
endef

define Package/$(PKG_NAME)/description
  Makes Klipper more accessible by adding a lightweight,
  responsive web user interface, centred around an intuitive
  and consistent design philosophy.
endef

define Build/Prepare
	rm -rf $(PKG_BUILD_DIR)/
	mkdir -p $(PKG_BUILD_DIR)/
	$(TAR) -xzf $(DL_DIR)/$(PKG_SOURCE) -C $(PKG_BUILD_DIR)  --strip-components 1
endef

define Build/Compile

endef

define Package/$(PKG_NAME)/install
	$(INSTALL_DIR) $(1)/usr/share/
	$(CP) -uvd $(PKG_BUILD_DIR)/dist $(1)/usr/share/mainsail
endef

$(eval $(call BuildPackage,$(PKG_NAME)))
