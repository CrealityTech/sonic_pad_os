#
# Copyright (C) 2015-2016 OpenWrt.org
#
# This is free software, licensed under the GNU General Public License v2.
# See /LICENSE for more information.
#

include $(TOPDIR)/rules.mk

PKG_NAME:=python3-attrs
PKG_VERSION:=20.3.0
PKG_RELEASE:=2

PKG_SOURCE:=attrs-$(PKG_VERSION).tar.gz
PKG_SOURCE_URL:=https://files.pythonhosted.org/packages/source/a/attrs/
PKG_MD5SUM:=4fe38f89297b2b446d83190fce189f29

PKG_BUILD_DEPENDS:=python3 python3-setuptools
PKG_BUILD_DIR:=$(COMPILE_DIR)/$(PKG_NAME)-$(PKG_VERSION)/attrs-$(PKG_VERSION)

PKG_LICENSE:=MIT
PKG_LICENSE_FILES:=LICENSE
PKG_MAINTAINER:=Jeffery To <jeffery.to@gmail.com>

include $(BUILD_DIR)/package.mk
$(call include_mk, python3-package.mk)


define Package/$(PKG_NAME)
	SECTION:=lang
	CATEGORY:=Languages
	SUBMENU:=Python
	TITLE:=python3 attrs
	URL:=http://attrs.readthedocs.org/
	DEPENDS:=
endef

define Package/$(PKG_NAME)/description
attrs is the Python package that will bring back the joy of writing classes by relieving you from the drudgery of implementing object protocols (aka dunder methods).
endef

define Build/Compile
	$(call Build/Compile/Py3Mod,,install --prefix="/usr" --root="$(PKG_INSTALL_DIR)")
endef

$(eval $(call Py3Package,$(PKG_NAME)))
$(eval $(call BuildPackage,$(PKG_NAME)))
