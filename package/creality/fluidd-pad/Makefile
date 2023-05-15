#
# Copyright (C) 2007-2016 OpenWrt.org
#
# This is free software, licensed under the GNU General Public License v2.
# See /LICENSE for more information.
#

include $(TOPDIR)/rules.mk
include $(BUILD_DIR)/package.mk
include ../version.mk

PKG_NAME:=fluidd-pad
PKG_VERSION:=$(FLUIDD-PAD_MAJOR_VERSION).$(FLUIDD-PAD_MINOR_VERSION).$(FLUIDD-PAD_LTERATION_VERSION)
PKG_SOURCE:=${PKG_NAME}-${PKG_VERSION}-prebuilt.tar.bz2
PKG_RELEASE:=1

define Package/$(PKG_NAME)
  TITLE:=fluidd-pad, the klipper UI
  SECTION:=base
  CATEGORY:=Creality
  DEPENDS:=+moonraker
endef

define Package/$(PKG_NAME)/description
  fluidd-pad application
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
endef

$(eval $(call BuildPackage,$(PKG_NAME)))
