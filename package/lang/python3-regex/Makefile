#
# Copyright (C) 2007-2014 OpenWrt.org
#
# This is free software, licensed under the GNU General Public License v2.
# See /LICENSE for more information.
#

include $(TOPDIR)/rules.mk

PKG_NAME:=python3-regex
PKG_VERSION:=2020.10.28
PKG_RELEASE:=1
PKG_LICENSE:=MIT

PKG_SOURCE:=regex-$(PKG_VERSION).tar.gz
PKG_SOURCE_URL:=https://files.pythonhosted.org/packages/source/r/regex/
PKG_HASH:=dd3e6547ecf842a29cf25123fbf8d2461c53c8d37aa20d87ecee130c89b7079b

PKG_BUILD_DEPENDS:=+python3/host

PKG_BUILD_DIR:=$(COMPILE_DIR)/regex-$(PKG_VERSION)

include $(BUILD_DIR)/package.mk
$(call include_mk, python3-package.mk)

define Package/$(PKG_NAME)
  SUBMENU:=Python
  SECTION:=lang
  CATEGORY:=Languages
  TITLE:=python3 regex
  DEPENDS:=+python3
  URL:=https://pypi.org/project/regex/
endef

define Package/$(PKG_NAME)/description
Alternative regular expression module, to replace re.
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
