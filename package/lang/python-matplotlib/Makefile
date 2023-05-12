#
# Copyright (C) 2007-2014 OpenWrt.org
#
# This is free software, licensed under the GNU General Public License v2.
# See /LICENSE for more information.
#

include $(TOPDIR)/rules.mk

PKG_NAME:=python-matplotlib
PKG_VERSION:=2.2.3
PKG_RELEASE:=1
PKG_LICENSE:=BSD

PKG_SOURCE:=matplotlib-$(PKG_VERSION).tar.gz
PKG_SOURCE_SUBDIR:=matplotlib-$(PKG_VERSION)
PKG_SOURCE_URL:=https://pypi.python.org/packages/source/m/matplotlib
PKG_MD5SUM:=403b0bddd751d71187416f20d4cff100
PKG_SOURCE_VERSION:=$(PKG_VERSION)

PKG_BUILD_DEPENDS:=python

PKG_BUILD_DIR:=$(COMPILE_DIR)/python-matplotlib-$(PKG_VERSION)/matplotlib-$(PKG_VERSION)

include $(BUILD_DIR)/package.mk
$(call include_mk, python-package.mk)

define Package/$(PKG_NAME)
  SUBMENU:=Python
  SECTION:=lang
  CATEGORY:=Languages
  TITLE:=python matplotlib
  DEPENDS:=+python-six +python-pyparsing \
		   +python-numpy +python-dateutil \
		   +python-kiwisolver +python-subprocess32 \
		   +python-backports.functools_lru_cache \
		   +python-cycler +python-pytz \
		   +libpng +libfreetype +libstdcpp
  URL:=https://matplotlib.org/
endef

define Package/$(PKG_NAME)/description
  Matplotlib is a comprehensive library for creating static, animated, and interactive visualizations in Python. Matplotlib makes easy things easy and hard things possible.
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
