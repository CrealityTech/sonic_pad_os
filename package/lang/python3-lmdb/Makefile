#
# Copyright (C) 2015-2016 OpenWrt.org
#
# This is free software, licensed under the GNU General Public License v2.
# See /LICENSE for more information.
#

include $(TOPDIR)/rules.mk

PKG_NAME:=python3-lmdb
PKG_VERSION:=1.2.1
PKG_RELEASE:=1

PKG_SOURCE:=lmdb-$(PKG_VERSION).tar.gz
PKG_SOURCE_URL:=https://files.pythonhosted.org/packages/source/l/lmdb
PKG_MD5SUM:=b8f4a2cd1748e8a5f5e352d1cb3d4350

PKG_BUILD_DEPENDS:=python3

PKG_BUILD_DIR:=$(COMPILE_DIR)/$(PKG_NAME)-$(PKG_VERSION)/lmdb-$(PKG_VERSION)

include $(BUILD_DIR)/host-build.mk
include $(BUILD_DIR)/package.mk
$(call include_mk, python3-package.mk)
$(call include_mk, python3-host.mk)

define Package/$(PKG_NAME)
	SECTION:=lang
	CATEGORY:=Languages
	SUBMENU:=Python
	TITLE:=$(PKG_NAME)
	URL:=https://github.com/jnwatson/py-lmdb/
	DEPENDS:=+python3
endef

define Package/$(PKG_NAME)/description
  Universal Python binding for the LMDB 'Lightning' Database
endef

define Build/Compile
	$(call Build/Compile/Py3Mod, ,install --prefix="/usr" --root="$(PKG_INSTALL_DIR)")
endef

$(eval $(call Py3Package,$(PKG_NAME)))
$(eval $(call BuildPackage,$(PKG_NAME)))
