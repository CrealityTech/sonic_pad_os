#
# Copyright (C) 2007-2014 OpenWrt.org
#
# This is free software, licensed under the GNU General Public License v2.
# See /LICENSE for more information.
#

include $(TOPDIR)/rules.mk

PKG_NAME:=python-backports.functools_lru_cache
PKG_VERSION:=1.6.4
PKG_RELEASE:=1
PKG_LICENSE:=MIT

PKG_SOURCE:=backports.functools_lru_cache-$(PKG_VERSION).tar.gz
PKG_SOURCE_SUBDIR:=backports.functools_lru_cache-$(PKG_VERSION)
PKG_SOURCE_URL:=https://pypi.python.org/packages/source/b/backports.functools_lru_cache
PKG_MD5SUM:=8fed424f30bf9554235aa02997b7574c
PKG_SOURCE_VERSION:=$(PKG_VERSION)

PKG_BUILD_DEPENDS:=python

PKG_BUILD_DIR:=$(COMPILE_DIR)/$(PKG_NAME)-$(PKG_VERSION)/backports.functools_lru_cache-$(PKG_VERSION)

include $(BUILD_DIR)/package.mk
$(call include_mk, python-package.mk)

define Package/$(PKG_NAME)
  SUBMENU:=Python
  SECTION:=lang
  CATEGORY:=Languages
  TITLE:=backports.functools_lru_cache
  DEPENDS:=+python
  URL:=https://github.com/jaraco/backports.functools_lru_cache
endef

define Package/$(PKG_NAME)/description
  Backport of functools.lru_cache from Python 3.3 as published at ActiveState.
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
