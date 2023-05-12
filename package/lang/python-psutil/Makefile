#
# Copyright (C) 2007-2014 OpenWrt.org
#
# This is free software, licensed under the GNU General Public License v2.
# See /LICENSE for more information.
#

include $(TOPDIR)/rules.mk

PKG_NAME:=python-psutil
PKG_VERSION:=5.7.2
PKG_RELEASE:=1
PKG_LICENSE:=MIT

PKG_SOURCE:=psutil-$(PKG_VERSION).tar.gz
PKG_SOURCE_SUBDIR:=psutil-$(PKG_VERSION)
PKG_SOURCE_URL:=https://pypi.python.org/packages/source/p/psutil
PKG_HASH:=90990af1c3c67195c44c9a889184f84f5b2320dce3ee3acbd054e3ba0b4a7beb

PKG_BUILD_DEPENDS:=+python/host

PKG_BUILD_DIR:=$(COMPILE_DIR)/$(PKG_NAME)-$(PKG_VERSION)/psutil-$(PKG_VERSION)

include $(BUILD_DIR)/package.mk
$(call include_mk, python-package.mk)

define Package/$(PKG_NAME)
  SUBMENU:=Python
  SECTION:=lang
  CATEGORY:=Languages
  TITLE:=python psutil
  DEPENDS:=+python
  URL:=https://pypi.org/project/psutil/
endef

define Package/$(PKG_NAME)/description
  Cross-platform lib for process and system monitoring in Python.
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
