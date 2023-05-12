#
# Copyright (C) 2007-2014 OpenWrt.org
#
# This is free software, licensed under the GNU General Public License v2.
# See /LICENSE for more information.
#

include $(TOPDIR)/rules.mk

PKG_NAME:=python-greenlet
PKG_VERSION:=0.4.15
PKG_RELEASE:=1
PKG_LICENSE:=BSD

PKG_SOURCE:=greenlet-$(PKG_VERSION).tar.gz
PKG_SOURCE_SUBDIR:=greenlet-$(PKG_VERSION)
PKG_SOURCE_URL:=https://pypi.python.org/packages/source/g/greenlet
PKG_MD5SUM:=10fa304f673fc18b28fa6d8c6658cb80
PKG_SOURCE_VERSION:=$(PKG_VERSION)

PKG_BUILD_DEPENDS:=python

PKG_BUILD_DIR:=$(COMPILE_DIR)/$(PKG_NAME)-$(PKG_VERSION)/greenlet-$(PKG_VERSION)

include $(BUILD_DIR)/package.mk
$(call include_mk, python-package.mk)

define Package/$(PKG_NAME)
  SUBMENU:=Python
  SECTION:=lang
  CATEGORY:=Languages
  TITLE:=python greenlet
  DEPENDS:=+python
  URL:=https://github.com/python-greenlet/greenlet
endef

define Package/$(PKG_NAME)/description
  The greenlet package is a spin-off of Stackless, a version of CPython that supports micro-threads called “tasklets”. Tasklets run pseudo-concurrently (typically in a single or a few OS-level threads) and are synchronized with data exchanges on “channels”.
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
