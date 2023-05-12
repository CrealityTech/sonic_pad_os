#
# Copyright (C) 2015-2016 OpenWrt.org
#
# This is free software, licensed under the GNU General Public License v2.
# See /LICENSE for more information.
#

include $(TOPDIR)/rules.mk

PKG_NAME:=python3-pyrsistent
PKG_VERSION:=0.18.1
PKG_RELEASE:=1

PKG_SOURCE:=pyrsistent-$(PKG_VERSION).tar.gz
PKG_SOURCE_URL:=https://files.pythonhosted.org/packages/source/p/pyrsistent
PKG_MD5SUM:=cef3da08455664bf917dcf8cd00d49a4

PKG_BUILD_DEPENDS:=python3

PKG_BUILD_DIR:=$(COMPILE_DIR)/$(PKG_NAME)-$(PKG_VERSION)/pyrsistent-$(PKG_VERSION)

include $(BUILD_DIR)/package.mk
$(call include_mk, python3-package.mk)

define Package/$(PKG_NAME)
	SECTION:=lang
	CATEGORY:=Languages
	SUBMENU:=Python
	TITLE:=$(PKG_NAME)
	URL:=https://github.com/tobgu/pyrsistent/
	DEPENDS:=+python3
endef

define Package/$(PKG_NAME)/description
  Persistent/Immutable/Functional data structures for Python
endef

define Build/Compile
	$(call Build/Compile/Py3Mod, ,install --prefix="/usr" --root="$(PKG_INSTALL_DIR)")
endef

$(eval $(call Py3Package,$(PKG_NAME)))
$(eval $(call BuildPackage,$(PKG_NAME)))
