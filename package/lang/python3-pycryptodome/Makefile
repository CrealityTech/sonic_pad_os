# This is free software, licensed under the GNU General Public License v2.
# See /LICENSE for more information.
#

include $(TOPDIR)/rules.mk

PKG_NAME:=python3-pycryptodome
PKG_VERSION:=3.9.7
PKG_RELEASE:=3

PKG_SOURCE:=pycryptodome-$(PKG_VERSION).tar.gz
PKG_SOURCE_URL:=https://files.pythonhosted.org/packages/source/p/pycryptodome/
PKG_HASH:=f1add21b6d179179b3c177c33d18a2186a09cc0d3af41ff5ed3f377360b869f2
PKG_CPE_ID:=cpe:/a:pycryptodome:pycryptodome

PKG_BUILD_DIR:=$(COMPILE_DIR)/$(PKG_NAME)-$(PKG_VERSION)/pycryptodome-$(PKG_VERSION)

include $(BUILD_DIR)/package.mk
$(call include_mk, python3-package.mk)

define Package/$(PKG_NAME)
  SECTION:=lang
  CATEGORY:=Languages
  SUBMENU:=Python
  TITLE:=A self-contained cryptographic library for Python
  URL:=https://www.pycryptodome.org/
  DEPENDS:=+libgmp +python3
  PROVIDES:=python3-crypto
endef

define Package/$(PKG_NAME)/description
PyCryptodome is a self-contained Python package of low-level
cryptographic primitives.
endef

define Build/Compile
	$(call Build/Compile/Py3Mod,,install --prefix="/usr" --root="$(PKG_INSTALL_DIR)")
endef

define Py3Package/$(PKG_NAME)/install
	$(INSTALL_DIR) $(1)/usr/lib
	$(CP) $(PKG_INSTALL_DIR)/usr/lib/* $(1)/usr/lib
endef

$(eval $(call Py3Package,$(PKG_NAME)))
$(eval $(call BuildPackage,$(PKG_NAME)))
