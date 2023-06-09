#
# Copyright (C) 2014 OpenWrt.org
#
# This is free software, licensed under the GNU General Public License v2.
# See /LICENSE for more information.
#

include $(TOPDIR)/rules.mk

PKG_NAME:=libev
PKG_VERSION:=4.33
PKG_RELEASE:=3

PKG_SOURCE:=$(PKG_NAME)-$(PKG_VERSION).tar.gz
PKG_SOURCE_URL:=http://dist.schmorp.de/libev/Attic/
PKG_MD5SUM:=a3433f23583167081bf4acdd5b01b34f
PKG_LICENSE:=BSD-2-Clause


PKG_BUILD_PARALLEL:=1
PKG_FIXUP:=autoreconf
PKG_INSTALL:=1
PKG_USE_MIPS16:=0

include $(BUILD_DIR)/package.mk

define Package/libev
  SECTION:=libs
  CATEGORY:=Libraries
  TITLE:=High-performance event loop
  URL:=http://software.schmorp.de/pkg/libev.html
endef

define Package/libev/description
 A full-featured and high-performance event loop that is loosely modelled after
 libevent, but without its limitations and bugs.
endef

TARGET_CFLAGS += $(FPIC)

CONFIGURE_ARGS += \
	--enable-shared \
	--enable-static \

define Build/InstallDev
	$(INSTALL_DIR) $(1)/usr/include/
	$(CP) $(PKG_INSTALL_DIR)/usr/include/* $(1)/usr/include/
	$(INSTALL_DIR) $(1)/usr/lib/
	$(CP) $(PKG_INSTALL_DIR)/usr/lib/libev.{a,so*} $(1)/usr/lib/
endef

define Package/libev/install
	$(INSTALL_DIR) $(1)/usr/lib
	$(CP) $(PKG_INSTALL_DIR)/usr/lib/libev.so* $(1)/usr/lib/
endef

$(eval $(call BuildPackage,libev))
