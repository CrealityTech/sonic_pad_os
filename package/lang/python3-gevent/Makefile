#
# Copyright (C) 2007-2014 OpenWrt.org
#
# This is free software, licensed under the GNU General Public License v2.
# See /LICENSE for more information.
#

include $(TOPDIR)/rules.mk

PKG_NAME:=python3-gevent
PKG_VERSION:=20.12.1
PKG_RELEASE:=1
PKG_LICENSE:=MIT

PKG_SOURCE:=gevent-$(PKG_VERSION).tar.gz
PKG_SOURCE_URL:=https://files.pythonhosted.org/packages/source/g/gevent/
PKG_HASH:=99b68765767bb3e2244a66b012883899a6f17c23b6dc1cd80b793df341e15f08

PKG_BUILD_DEPENDS:=+python3 python3-cython

PKG_BUILD_DIR:=$(COMPILE_DIR)/$(PKG_NAME)-$(PKG_VERSION)/gevent-$(PKG_VERSION)

include $(BUILD_DIR)/package.mk
$(call include_mk, python3-package.mk)

define Package/$(PKG_NAME)
  SUBMENU:=Python
  SECTION:=lang
  CATEGORY:=Languages
  TITLE:=gevent
  DEPENDS:=+python3 +python3-greenlet +libev +libcares
  URL:=http://www.gevent.org/
endef

define Package/$(PKG_NAME)/description
gevent is a coroutine -based Python networking library.
endef

TARGET_LDFLAGS += -Wl,-rpath-link=$(STAGING_DIR)/usr/lib,-rpath-link=$(CONFIG_TOOLCHAIN_ROOT)/lib/

define Build/Compile
	$(INSTALL_DIR) $(PKG_INSTALL_DIR)/$(PYTHON3_PKG_DIR)
	$(call HostPython3, \
		cd $(PKG_BUILD_DIR); \
		CC="$(TARGET_CC)" \
		CCSHARED="$(TARGET_CC) $(FPIC)" \
		CXX="$(TARGET_CXX)" \
		LD="$(TARGET_CC)" \
		GEVENTSETUP_EMBED_LIBEV=0 \
		GEVENTSETUP_EMBED_CARES=0 \
		LDSHARED="$(TARGET_CC) -shared" \
		CFLAGS="$(TARGET_CFLAGS)" \
		CPPFLAGS="$(TARGET_CPPFLAGS) -I$(PYTHON3_INC_DIR)" \
		LDFLAGS="$(TARGET_LDFLAGS) -lpython$(PYTHON3_VERSION)$(PYTHON3_ABIFLAGS)" \
		__PYVENV_LAUNCHER__="/usr/bin/$(PYTHON3)" \
		, \
		./setup.py install --root="$(PKG_INSTALL_DIR)" --prefix="/usr" \
	)
	find $(PKG_INSTALL_DIR) -name "*\.pyc" -o -name "*\.pyo" | xargs rm -f
endef

define Py3Package/$(PKG_NAME)/install
	$(INSTALL_DIR) $(1)/usr/lib
	$(CP) $(PKG_INSTALL_DIR)/usr/lib/* $(1)/usr/lib
endef

$(eval $(call Py3Package,$(PKG_NAME)))
$(eval $(call BuildPackage,$(PKG_NAME)))
