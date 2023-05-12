#
# Copyright (C) 2015-2016 OpenWrt.org
#
# This is free software, licensed under the GNU General Public License v2.
# See /LICENSE for more information.
#

include $(TOPDIR)/rules.mk

PKG_NAME:=python-cffi
PKG_VERSION:=1.12.2
PKG_RELEASE:=2

PKG_SOURCE:=cffi-$(PKG_VERSION).tar.gz
PKG_SOURCE_URL:=https://pypi.python.org/packages/source/c/cffi
PKG_MD5SUM:=4d7dcb6c7c738c15d2ece9bd4c5f86da

PKG_BUILD_DEPENDS:=python python-setuptools
HOST_BUILD_DEPENDS:=libffi/host python/host python-setuptools/host python-pycparser/host

PKG_BUILD_DIR:=$(COMPILE_DIR)/$(PKG_NAME)-$(PKG_VERSION)/cffi-$(PKG_VERSION)

PKG_LICENSE:=MIT
PKG_LICENSE_FILES:=LICENSE
PKG_MAINTAINER:=Jeffery To <jeffery.to@gmail.com>

include $(BUILD_DIR)/host-build.mk
include $(BUILD_DIR)/package.mk
$(call include_mk, python-package.mk)
$(call include_mk, python-host.mk)

define Package/$(PKG_NAME)
	SECTION:=lang
	CATEGORY:=Languages
	SUBMENU:=Python
	TITLE:=python-cffi
	URL:=http://cffi.readthedocs.org/
	DEPENDS:=+libffi +python-light +python-pycparser
endef

define Package/$(PKG_NAME)/description
Foreign Function Interface for Python calling C code.
endef

define Build/Compile
	$(call Build/Compile/PyMod,,install --prefix="/usr" --root="$(PKG_INSTALL_DIR)")
endef

define Host/Compile
	$(call Build/Compile/HostPyMod,,install --prefix="" --root="$(OUT_DIR)/host")
endef

define Host/Install
endef

$(eval $(call HostBuild))

$(eval $(call PyPackage,$(PKG_NAME)))
$(eval $(call BuildPackage,$(PKG_NAME)))
