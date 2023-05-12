#
# Copyright (C) 2007-2014 OpenWrt.org
#
# This is free software, licensed under the GNU General Public License v2.
# See /LICENSE for more information.
#

include $(TOPDIR)/rules.mk

PKG_NAME:=python-kiwisolver
PKG_VERSION:=1.1.0
PKG_RELEASE:=1
PKG_LICENSE:=BSD

PKG_SOURCE:=kiwisolver-$(PKG_VERSION).tar.gz
PKG_SOURCE_SUBDIR:=kiwisolver-$(PKG_VERSION)
PKG_SOURCE_URL:=https://pypi.python.org/packages/source/k/kiwisolver
PKG_MD5SUM:=fc8a614367f7ba0d34a02fd08c535afc
PKG_SOURCE_VERSION:=$(PKG_VERSION)

PKG_BUILD_DEPENDS:=python

PKG_BUILD_DIR:=$(COMPILE_DIR)/python-kiwisolver-$(PKG_VERSION)/kiwisolver-$(PKG_VERSION)

include $(BUILD_DIR)/package.mk
$(call include_mk, python-package.mk)

define Package/$(PKG_NAME)
  SUBMENU:=Python
  SECTION:=lang
  CATEGORY:=Languages
  TITLE:=python kiwisolver
  DEPENDS:=+python +libstdcpp
  URL:=https://github.com/nucleic/kiwi
endef

define Package/$(PKG_NAME)/description
  Kiwi is an efficient C++ implementation of the Cassowary constraint solving algorithm. Kiwi is an implementation of the algorithm based on the seminal Cassowary paper. It is not a refactoring of the original C++ solver. Kiwi has been designed from the ground up to be lightweight and fast. Kiwi ranges from 10x to 500x faster than the original Cassowary solver with typical use cases gaining a 40x improvement. Memory savings are consistently > 5x.
endef

define Build/Compile
	$(call Build/Compile/PyMod,,install --prefix="/usr" --root="$(PKG_INSTALL_DIR)")
endef

define PyPackage/$(PKG_NAME)/install
	$(INSTALL_DIR) $(1)/usr/lib
	$(CP) $(PKG_INSTALL_DIR)/usr/lib/* $(1)/usr/lib
endef

$(eval $(call PyPackage,$(PKG_NAME)))
$(eval $(call BuildPackage,$(PKG_NAME)))
