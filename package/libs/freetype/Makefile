#
# Copyright (C) 2006-2015 OpenWrt.org
#
# This is free software, licensed under the GNU General Public License v2.
# See /LICENSE for more information.
#

include $(TOPDIR)/rules.mk

PKG_NAME:=freetype
PKG_VERSION:=2.10.4
PKG_RELEASE:=1

PKG_SOURCE:=$(PKG_NAME)-$(PKG_VERSION).tar.xz
PKG_SOURCE_URL:=@SF/freetype
PKG_HASH:=86a854d8905b19698bbc8f23b860bc104246ce4854dcea8e3b0fb21284f75784

PKG_MAINTAINER:=Val Kulkov <val.kulkov@gmail.com>
PKG_LICENSE:=FTL GPL-2.0-only MIT ZLIB GPL-3.0-or-later
PKG_LICENSE_FILES:=docs/LICENSE.TXT docs/FTL.TXT docs/GPLv2.TXT src/bdf/README src/pcf/README src/gzip/zlib.h builds/unix/config.sub b    uilds/unix/config.guess builds/unix/libtool
PKG_CPE_ID:=cpe:/a:freetype:freetype2

PKG_FIXUP:=autoreconf
PKG_LIBTOOL_PATHS:=builds/unix

include $(BUILD_DIR)/host-build.mk
include $(BUILD_DIR)/package.mk

define Package/libfreetype
  SECTION:=libs
  CATEGORY:=Libraries
  TITLE:=A free, high-quality and portable font engine
  URL:=http://www.freetype.org/
  DEPENDS:=+zlib +libbz2
endef

define Package/libfreetype/description
 The FreeType project is a team of volunteers who develop free,
 portable and high-quality software solutions for digital typography.
 They specifically target embedded systems and focus on bringing small,
 efficient and ubiquitous products.
endef

TARGET_CFLAGS += $(FPIC)

CONFIGURE_ARGS += \
	--enable-freetype-config \
	--enable-shared \
	--enable-static \
	--with-bzip2=yes \
	--with-zlib=yes \
	--with-png=no \

define Build/Compile
	$(MAKE) -C $(PKG_BUILD_DIR) DESTDIR="$(PKG_INSTALL_DIR)" all install
endef

define Build/InstallDev
	$(INSTALL_DIR) $(1)/bin
	$(INSTALL_BIN) $(PKG_INSTALL_DIR)/usr/bin/freetype-config $(1)/bin/
	$(INSTALL_DIR) $(2)/bin
	$(INSTALL_BIN) $(PKG_INSTALL_DIR)/usr/bin/freetype-config $(2)/bin/
	$(INSTALL_DIR) $(1)/usr/include
	$(CP) $(PKG_INSTALL_DIR)/usr/include/freetype2 $(1)/usr/include/
	$(INSTALL_DIR) $(1)/usr/lib
	$(CP) $(PKG_INSTALL_DIR)/usr/lib/libfreetype.{a,so*} $(1)/usr/lib/
	$(INSTALL_DIR) $(1)/usr/lib/pkgconfig
	$(CP) $(PKG_INSTALL_DIR)/usr/lib/pkgconfig/freetype2.pc $(1)/usr/lib/pkgconfig/

	$(SED) \
		's,/usr/include,$$$${prefix}/include,g; \
		 s,/usr/lib,$$$${exec_prefix}/lib,g' \
		 $(1)/usr/lib/pkgconfig/freetype2.pc
endef


define Package/libfreetype/install
	$(INSTALL_DIR) $(1)/usr/lib
	$(CP) $(PKG_INSTALL_DIR)/usr/lib/libfreetype.so.* $(1)/usr/lib/
endef

$(eval $(call HostBuild))
$(eval $(call BuildPackage,libfreetype))
