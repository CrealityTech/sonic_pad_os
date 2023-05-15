#
# Copyright (C) 2011 OpenWrt.org
#
# This is free software, licensed under the GNU General Public License v2.
# See /LICENSE for more information.
#

include $(TOPDIR)/rules.mk

PKG_NAME:=brightness
PKG_VERSION:=1.2
PKG_RELEASE:=1

PKG_BUILD_DIR := $(COMPILE_DIR)/$(PKG_NAME)
PKG_BUILD_DEPENDS:=libuapi

include $(BUILD_DIR)/package.mk

define Package/$(PKG_NAME)
  TITLE:=brightness adjust
  SECTION:=base
  CATEGORY:=Creality
  DEPENDS:=+libuapi
endef

define Package/$(PKG_NAME)/description
	adjust backlight brightness
endef

define Build/Prepare
	$(CP) -r ./src/* $(PKG_BUILD_DIR)/
endef

define Build/Compile
	$(MAKE) -C $(PKG_BUILD_DIR)/ \
		ARCH="$(TARGET_ARCH)" \
		AR="$(TARGET_AR)" \
		CC="$(TARGET_CC)" \
		CXX="$(TARGET_CXX)" \
		CFLAGS="$(TARGET_CFLAGS) -I$(STAGING_DIR)/usr/include" \
		LDFLAGS="$(TARGET_LDFLAGS)" \
		TARGET_BOARD="$(TARGET_BOARD_PLATFORM)" \
		all
endef

define Package/$(PKG_NAME)/install
	$(INSTALL_DIR) $(1)/usr/bin
	$(INSTALL_BIN) $(PKG_BUILD_DIR)/brightness $(1)/usr/bin/
endef

$(eval $(call BuildPackage,$(PKG_NAME)))
