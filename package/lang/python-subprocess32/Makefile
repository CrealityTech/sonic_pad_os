#
# Copyright (C) 2007-2014 OpenWrt.org
#
# This is free software, licensed under the GNU General Public License v2.
# See /LICENSE for more information.
#

include $(TOPDIR)/rules.mk

PKG_NAME:=python-subprocess32
PKG_VERSION:=3.5.4
PKG_RELEASE:=1
PKG_LICENSE:=BSD

PKG_SOURCE:=subprocess32-$(PKG_VERSION).tar.gz
PKG_SOURCE_SUBDIR:=subprocess32-$(PKG_VERSION)
PKG_SOURCE_URL:=https://pypi.python.org/packages/source/s/subprocess32
PKG_MD5SUM:=748844d560be5f0c707931cba1cdb48b
PKG_SOURCE_VERSION:=$(PKG_VERSION)

PKG_BUILD_DEPENDS:=python

PKG_BUILD_DIR:=$(COMPILE_DIR)/python-subprocess32-$(PKG_VERSION)/subprocess32-$(PKG_VERSION)

include $(BUILD_DIR)/package.mk
$(call include_mk, python-package.mk)

define Package/$(PKG_NAME)
  SUBMENU:=Python
  SECTION:=lang
  CATEGORY:=Languages
  TITLE:=python subprocess32
  DEPENDS:=+python
  URL:=https://github.com/google/python-subprocess32
endef

define Package/$(PKG_NAME)/description
  This is a backport of the Python 3 subprocess module for use on Python 2. This code has not been tested on Windows or other non-POSIX platforms.
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
