#
# Copyright (C) 2007-2016 OpenWrt.org
#
# This is free software, licensed under the GNU General Public License v2.
# See /LICENSE for more information.
#

include $(TOPDIR)/rules.mk
include $(BUILD_DIR)/package.mk

PKG_NAME:=rtsp_demo
PKG_SOURCE:=${PKG_NAME}-prebuilt.tar.bz2
PKG_RELEASE:=1

define Package/$(PKG_NAME)
  TITLE:=$(PKG_NAME)
  SECTION:=base
  CATEGORY:=Creality
  DEPENDS:=+libcedarx +live +libCrLog +libpthread +libjpeg +MJPG_STREAMER_V4L2:libv4l
endef

define Package/$(PKG_NAME)/description
  CedarX2.8 rtsp_demo
endef

define Build/Prepare
	rm -rf $(PKG_BUILD_DIR)/
	mkdir -p $(PKG_BUILD_DIR)/
	$(TAR) -xjf $(DL_DIR)/$(PKG_SOURCE) -C $(PKG_BUILD_DIR)
endef

define Build/Compile

endef

define Package/$(PKG_NAME)/install
	$(CP) -uvd $(PKG_BUILD_DIR)/usr/ $(1)/
	$(CP) -uvd $(PKG_BUILD_DIR)/etc/ $(1)/
	$(CP) -uvd $(PKG_BUILD_DIR)/www/ $(1)/
endef

$(eval $(call BuildPackage,$(PKG_NAME)))
