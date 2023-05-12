#
# Copyright (C) 2013 OpenWrt.org
#
# This is free software, licensed under the GNU General Public License v2.
# See /LICENSE for more information.
#

include $(TOPDIR)/rules.mk

PKG_NAME:=python3-greenlet
PKG_VERSION:=0.4.17
PKG_RELEASE:=1

PKG_SOURCE:=greenlet-$(PKG_VERSION).tar.gz
PKG_SOURCE_SUBDIR:=greenlet-$(PKG_VERSION)
PKG_SOURCE_URL:=https://files.pythonhosted.org/packages/source/g/greenlet/
PKG_HASH:=41d8835c69a78de718e466dd0e6bfd4b46125f21a67c3ff6d76d8d8059868d6b
PKG_SOURCE_VERSION:=$(PKG_VERSION)
PKG_BUILD_DIR:=$(COMPILE_DIR)/$(PKG_NAME)-$(PKG_VERSION)/greenlet-$(PKG_VERSION)
PKG_BUILD_DEPENDS:=python3

include $(BUILD_DIR)/package.mk
$(call include_mk, python3-package.mk)

define Package/$(PKG_NAME)
  SUBMENU:=Python
  SECTION:=lang
  CATEGORY:=Languages
  TITLE:=greenlet
  URL:=https://github.com/python-greenlet/greenlet
  DEPENDS:=+python3
endef

define Package/$(PKG_NAME)/description
  The greenlet package is a spin-off of Stackless, a version of CPython
  that supports micro-threads called "tasklets". Tasklets run
  pseudo-concurrently (typically in a single or a few OS-level threads)
  and are synchronized with data exchanges on "channels".

  A "greenlet", on the other hand, is a still more primitive notion of
  micro-thread with no implicit scheduling; coroutines, in other
  words. This is useful when you want to control exactly when your code
  runs. You can build custom scheduled micro-threads on top of greenlet;
  however, it seems that greenlets are useful on their own as a way to
  make advanced control flow structures. For example, we can recreate
  generators; the difference with Python's own generators is that our
  generators can call nested functions and the nested functions can
  yield values too. Additionally, you don't need a "yield" keyword. See
  the example in tests/test_generator.py.

  Greenlets are provided as a C extension module for the regular
  unmodified interpreter.

  Greenlets are lightweight coroutines for in-process concurrent
  programming.
endef

define Build/Compile
	$(call Build/Compile/Py3Mod, ,install --prefix="/usr" --root="$(PKG_INSTALL_DIR)")
endef

define Py3Package/$(PKG_NAME)/install
	$(INSTALL_DIR) $(1)/usr/lib
	$(CP) $(PKG_INSTALL_DIR)/usr/lib/* $(1)/usr/lib
endef

$(eval $(call Py3Package,$(PKG_NAME)))
$(eval $(call BuildPackage,$(PKG_NAME)))
