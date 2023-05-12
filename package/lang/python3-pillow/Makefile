#
# Copyright (C) 2007-2014 OpenWrt.org
#
# This is free software, licensed under the GNU General Public License v2.
# See /LICENSE for more information.
#

include $(TOPDIR)/rules.mk

PKG_NAME:=python3-pillow
PKG_VERSION:=8.3.2
PKG_RELEASE:=1
PKG_LICENSE:=MIT

PKG_SOURCE:=Pillow-$(PKG_VERSION).tar.gz
PKG_SOURCE_URL:=https://files.pythonhosted.org/packages/source/P/Pillow/
PKG_HASH:=dde3f3ed8d00c72631bc19cbfff8ad3b6215062a5eed402381ad365f82f0c18c

PKG_BUILD_DEPENDS:=+python3/host

PKG_BUILD_DIR:=$(COMPILE_DIR)/$(PKG_NAME)-$(PKG_VERSION)/Pillow-$(PKG_VERSION)

include $(BUILD_DIR)/package.mk
$(call include_mk, python3-package.mk)

define Package/$(PKG_NAME)
  SUBMENU:=Python
  SECTION:=lang
  CATEGORY:=Languages
  TITLE:=The friendly PIL fork
  DEPENDS:=+python3 +libjpeg +libfreetype +libtiff +zlib
  URL:=https://python-pillow.org/
endef

define Package/$(PKG_NAME)/description
The friendly PIL fork
endef
PYTHON3_PKG_SETUP_GLOBAL_ARGS := build_ext \
        --enable-zlib \
        --enable-jpeg \
        --enable-tiff \
        --enable-freetype \
        --disable-lcms \
        --disable-webp \
        --disable-webpmux \
        --disable-jpeg2000 \
        --disable-imagequant \
        --disable-platform-guessing	

define Build/Compile
	$(call Build/Compile/Py3Mod, ,install --prefix="/usr" --root="$(PKG_INSTALL_DIR)" \
			$(PYTHON3_PKG_SETUP_GLOBAL_ARGS))
endef

define Py3Package/$(PKG_NAME)/install
	$(INSTALL_DIR) $(1)/usr/lib
	$(CP) $(PKG_INSTALL_DIR)/usr/lib/* $(1)/usr/lib
endef

$(eval $(call Py3Package,$(PKG_NAME)))
$(eval $(call BuildPackage,$(PKG_NAME)))
