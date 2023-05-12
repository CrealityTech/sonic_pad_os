#
# Copyright (C) 2015-2016 OpenWrt.org
#
# This is free software, licensed under the GNU General Public License v2.
# See /LICENSE for more information.
#

include $(TOPDIR)/rules.mk

PKG_NAME:=python3-urllib3
PKG_VERSION:=1.26.3
PKG_RELEASE:=2

PKG_SOURCE:=urllib3-$(PKG_VERSION).tar.gz
PKG_SOURCE_URL:=https://files.pythonhosted.org/packages/source/u/urllib3/
PKG_MD5SUM:=4181b82b66744045ee1ddbc1527f6401

PKG_BUILD_DEPENDS:=python3 python3-setuptools
PKG_BUILD_DIR:=$(COMPILE_DIR)/$(PKG_NAME)/$(PKG_VERSION)/urllib3-$(PKG_VERSION)

PKG_LICENSE:=MIT
PKG_LICENSE_FILES:=LICENSE
PKG_MAINTAINER:=Jeffery To <jeffery.to@gmail.com>

include $(BUILD_DIR)/package.mk
$(call include_mk, python3-package.mk)

define Package/$(PKG_NAME)
	SECTION:=lang
	CATEGORY:=Languages
	SUBMENU:=Python
	TITLE:=$(PKG_NAME)
	URL:=http://urllib3.readthedocs.org/
	DEPENDS:=+python3
endef

define Package/$(PKG_NAME)/description
urllib3 is a powerful, user-friendly HTTP client for Python. Much of the Python ecosystem already uses urllib3 and you should too.
endef

define Build/Compile
	$(call Build/Compile/Py3Mod,,install --prefix="/usr" --root="$(PKG_INSTALL_DIR)")
endef

$(eval $(call Py3Package,$(PKG_NAME)))
$(eval $(call BuildPackage,$(PKG_NAME)))
