#
# Copyright (C) 2008-2015 OpenWrt.org
#
# This is free software, licensed under the GNU General Public License v2.
# See /LICENSE for more information.
#

include $(TOPDIR)/rules.mk

PKG_NAME:=python3-pyyaml
PKG_VERSION:=5.3.1
PKG_RELEASE:=1

PKG_SOURCE:=PyYAML-$(PKG_VERSION).tar.gz
PKG_SOURCE_URL:=https://files.pythonhosted.org/packages/source/P/PyYAML
PKG_MD5SUM:=d3590b85917362e837298e733321962b

PKG_LICENSE:=MIT
PKG_LICENSE_FILES:=COPYING
PKG_MAINTAINER:=Nicolas Thill <nico@openwrt.org>

PKG_BUILD_DEPENDS:=+python3

PKG_BUILD_DIR:=$(COMPILE_DIR)/$(PKG_NAME)-$(PKG_VERSION)/PyYAML-$(PKG_VERSION)

include $(BUILD_DIR)/package.mk
$(call include_mk, python3-package.mk)

define Package/$(PKG_NAME)
  SUBMENU:=Python
  SECTION:=lang
  CATEGORY:=Languages
  TITLE:=YAML parser and emitter for Python
  URL:=http://pyyaml.org/wiki/PyYAML
  DEPENDS:=+python3 +libyaml
endef

define Build/Compile
	$(call Build/Compile/Py3Mod,,\
		--with-libyaml install --prefix="$(PKG_INSTALL_DIR)/usr" \
	)
endef

$(eval $(call Py3Package,$(PKG_NAME)))
$(eval $(call BuildPackage,$(PKG_NAME)))
