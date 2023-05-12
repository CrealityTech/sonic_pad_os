#
# Copyright (C) 2015-2016 OpenWrt.org
#
# This is free software, licensed under the GNU General Public License v2.
# See /LICENSE for more information.
#

include $(TOPDIR)/rules.mk

PKG_NAME:=python3-pycparser
PKG_VERSION:=2.20
PKG_RELEASE:=2

PKG_SOURCE:=pycparser-$(PKG_VERSION).tar.gz
PKG_SOURCE_URL:=https://files.pythonhosted.org/packages/source/p/pycparser
PKG_MD5SUM:=b8f88de737db8c346ee8d31c07c7a25a

PKG_BUILD_DEPENDS:=python3 python3-setuptools
HOST_PYTHON3_PACKAGE_BUILD_DEPENDS:="ply==3.11"

PKG_LICENSE:=BSD-3-Clause
PKG_LICENSE_FILES:=LICENSE
PKG_MAINTAINER:=Jeffery To <jeffery.to@gmail.com>

include $(BUILD_DIR)/package.mk
$(call include_mk, python3-package.mk)

PKG_BUILD_DIR:=$(COMPILE_DIR)/$(PKG_NAME)-$(PKG_VERSION)/pycparser-$(PKG_VERSION)

define Package/$(PKG_NAME)
	SECTION:=lang
	CATEGORY:=Languages
	SUBMENU:=Python
	TITLE:=python3 pycparser
	URL:=http://pycparser.readthedocs.org/
	DEPENDS:=+python3
endef

define Package/$(PKG_NAME)/description
pycparser is a complete parser of the C language, written in pure Python using the PLY parsing library. It parses C code into an AST and can serve as a front-end for C compilers or analysis tools.
endef

define Build/Compile
	$(if $(HOST_PYTHON3_PACKAGE_BUILD_DEPENDS),
		$(call Build/Compile/HostPy3PipInstall,$(HOST_PYTHON3_PACKAGE_BUILD_DEPENDS))
	)
	$(call Build/Compile/Py3Mod,,install --prefix="/usr" --root="$(PKG_INSTALL_DIR)")
endef

$(eval $(call Py3Package,$(PKG_NAME)))
$(eval $(call BuildPackage,$(PKG_NAME)))
