#
# Copyright (C) 2007-2014 OpenWrt.org
#
# This is free software, licensed under the GNU General Public License v2.
# See /LICENSE for more information.
#

include $(TOPDIR)/rules.mk

PKG_NAME:=python3-numpy
PKG_VERSION:=1.16.6
PKG_RELEASE:=1
PKG_LICENSE:=MIT

PKG_SOURCE:=numpy-$(PKG_VERSION).zip
PKG_SOURCE_URL:=https://files.pythonhosted.org/packages/source/n/numpy/
PKG_HASH:=24647f014eef9b67a24adfcbcd4f4928349b4a0f8393b3d7fe648d4d2de3

PKG_BUILD_DEPENDS:=+python3/host

PKG_BUILD_DIR:=$(COMPILE_DIR)/$(PKG_NAME)-$(PKG_VERSION)/numpy-$(PKG_VERSION)

include $(BUILD_DIR)/package.mk
$(call include_mk, python3-package.mk)

define Package/$(PKG_NAME)
  SUBMENU:=Python
  SECTION:=lang
  CATEGORY:=Languages
  TITLE:=python3 numpy
  DEPENDS:=+python3
  URL:=https://pypi.org/project/numpy/
endef

define Package/$(PKG_NAME)/description
NumPy is the fundamental package for array computing with Python.
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
