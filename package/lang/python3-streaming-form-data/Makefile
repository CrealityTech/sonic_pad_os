#
# Copyright (C) 2015-2016 OpenWrt.org
#
# This is free software, licensed under the GNU General Public License v2.
# See /LICENSE for more information.
#

include $(TOPDIR)/rules.mk

PKG_NAME:=python3-streaming-form-data
PKG_VERSION:=1.8.1
PKG_RELEASE:=1

PKG_SOURCE:=streaming-form-data-$(PKG_VERSION).tar.gz
PKG_SOURCE_URL:=https://files.pythonhosted.org/packages/source/s/streaming-form-data
PKG_MD5SUM:=c7517e0e1f1390bbf5dd3fa91dd5f58f

PKG_BUILD_DEPENDS:=python3

PKG_BUILD_DIR:=$(COMPILE_DIR)/$(PKG_NAME)-$(PKG_VERSION)/streaming-form-data-$(PKG_VERSION)

include $(BUILD_DIR)/host-build.mk
include $(BUILD_DIR)/package.mk
$(call include_mk, python3-package.mk)
$(call include_mk, python3-host.mk)

define Package/$(PKG_NAME)
	SECTION:=lang
	CATEGORY:=Languages
	SUBMENU:=Python
	TITLE:=$(PKG_NAME)
	URL:=https://github.com/siddhantgoel/streaming-form-data
	DEPENDS:=+python3
endef

define Package/$(PKG_NAME)/description
  streaming_form_data provides a Python parser for parsing multipart/form-data input chunks (the encoding used when submitting data over HTTP through HTML forms).
endef

define Build/Compile
	$(call Build/Compile/Py3Mod, ,install --prefix="/usr" --root="$(PKG_INSTALL_DIR)")
endef

$(eval $(call Py3Package,$(PKG_NAME)))
$(eval $(call BuildPackage,$(PKG_NAME)))
