#
# Copyright (C) 2014-2016 OpenWrt.org
#
# This is free software, licensed under the GNU General Public License v2.
# See /LICENSE for more information.
#

include $(TOPDIR)/rules.mk

PKG_NAME:=optee-client-2.5
PKG_VERSION:=2.5.0
PKG_RELEASE:=1

PKG_SOURCE:=optee_client-$(PKG_VERSION).tar.gz
URL_FILE:=2.5.0.tar.gz
PKG_SOURCE_URL:=https://github.com/OP-TEE/optee_client/archive/
PKG_MD5SUM:=fd2dffe4c6fd428920019b407ded99bc

PKG_BUILD_DIR:=$(COMPILE_DIR)/optee_client-$(PKG_VERSION)

PKG_LICENSE:=BSD 2-Clause
PKG_LICENSE_FILES:=LICENSE

include $(BUILD_DIR)/package.mk

define Package/$(PKG_NAME)
  SECTION:=utils
  CATEGORY:=Security
  SUBMENU:=OPTEE
  DEPENDS:=+liballwinner-base @OPTEE_VERSION_2_5
  TITLE:=optee-client
endef

define Package/$(PKG_NAME)/description
This component provides the TEE Client API as defined by the GlobalPlatform TEE standard.
endef

define Build/InstallDev
	$(INSTALL_DIR) $(1)/usr/
	$(INSTALL_DIR) $(1)/usr/include/
	$(INSTALL_DIR) $(1)/usr/sbin
	$(INSTALL_DIR) $(1)/usr/lib

	$(CP) $(PKG_BUILD_DIR)/out/export/include/* $(1)/usr/include
	$(CP) $(PKG_BUILD_DIR)/out/export/bin/*	$(1)/usr/sbin
	$(CP) $(PKG_BUILD_DIR)/out/export/lib/* $(1)/usr/lib
endef

define Package/$(PKG_NAME)/install
	$(INSTALL_DIR) $(1)/usr/sbin
	$(INSTALL_DIR) $(1)/usr/lib
	$(INSTALL_DIR) $(1)/etc
	$(INSTALL_DIR) $(1)/etc/init.d

	$(CP) ./files/tee-supplicant.init $(1)/etc/init.d/tee-supplicant
	$(CP) $(PKG_BUILD_DIR)/out/export/bin/* $(1)/usr/sbin
	$(CP) $(PKG_BUILD_DIR)/out/export/lib/* $(1)/usr/lib
endef

$(eval $(call BuildPackage,$(PKG_NAME)))
