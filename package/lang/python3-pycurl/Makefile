#
# Copyright (C) 2007-2014 OpenWrt.org
#
# This is free software, licensed under the GNU General Public License v2.
# See /LICENSE for more information.
#

include $(TOPDIR)/rules.mk

PKG_NAME:=python3-pycurl
PKG_VERSION:=7.43.0.3
PKG_RELEASE:=1
PKG_LICENSE:=MIT

PKG_SOURCE:=pycurl-$(PKG_VERSION).tar.gz
PKG_SOURCE_URL:=https://files.pythonhosted.org/packages/source/p/pycurl
PKG_HASH:=aa975c19b79b6aa6c0518c0cc2ae33528900478f0b500531dbcdbf05beec584c

PKG_BUILD_DEPENDS:=+python3/host

PKG_BUILD_DIR:=$(COMPILE_DIR)/$(PKG_NAME)-$(PKG_VERSION)/pycurl-$(PKG_VERSION)

include $(BUILD_DIR)/package.mk
$(call include_mk, python3-package.mk)

define Package/$(PKG_NAME)
  SUBMENU:=Python
  SECTION:=lang
  CATEGORY:=Languages
  TITLE:=Python module interface to the cURL library
  DEPENDS:=+python3 +libcurl
  URL:=http://pycurl.io/
endef

define Package/$(PKG_NAME)/description
PycURL is a Python interface to libcurl, the multiprotocol file transfer library.
endef

PYTHON_PKG_SETUP_ARGS:=
PYTHON3_PKG_SETUP_ARGS:=

ifdef CONFIG_LIBCURL_OPENSSL
  PYTHON_PKG_SETUP_ARGS+=--with-openssl
  PYTHON3_PKG_SETUP_ARGS+=--with-openssl
endif

ifdef CONFIG_LIBCURL_GNUTLS
  PYTHON_PKG_SETUP_ARGS+=--with-gnutls
  PYTHON3_PKG_SETUP_ARGS+=--with-gnutls
endif

ifdef CONFIG_LIBCURL_MBEDTLS
  PYTHON_PKG_SETUP_ARGS+=--with-mbedtls
  PYTHON3_PKG_SETUP_ARGS+=--with-mbedtls
endif

ifdef CONFIG_LIBCURL_WOLFSSL
  PYTHON_PKG_SETUP_ARGS+=--with-wolfssl
  PYTHON3_PKG_SETUP_ARGS+=--with-wolfssl
endif

define Build/Compile
	$(call Build/Compile/Py3Mod, \
		 , \
		install --root="$(PKG_INSTALL_DIR)" --prefix="/usr" \
		$(PYTHON3_PKG_SETUP_ARGS) \
	)
endef

define Py3Package/$(PKG_NAME)/install
	$(INSTALL_DIR) $(1)/usr/lib
	$(CP) $(PKG_INSTALL_DIR)/usr/lib/* $(1)/usr/lib
endef

$(eval $(call Py3Package,$(PKG_NAME)))
$(eval $(call BuildPackage,$(PKG_NAME)))
