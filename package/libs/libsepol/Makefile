#
# This is free software, licensed under the GNU General Public License v2.
# See /LICENSE for more information.
#

include $(TOPDIR)/rules.mk

PKG_NAME:=libsepol
PKG_VERSION:=2.9
PKG_RELEASE:=1

PKG_SOURCE:=$(PKG_NAME)-$(PKG_VERSION).tar.gz
PKG_SOURCE_URL:=https://github.com/SELinuxProject/selinux/releases/download/20190315
PKG_HASH:=a34b12b038d121e3e459b1cbaca3c9202e983137819c16baf63658390e3f1d5d

PKG_MAINTAINER:=Thomas Petazzoni <thomas.petazzoni@bootlin.com>

include $(BUILD_DIR)/package.mk

define Package/libsepol
  SECTION:=libs
  CATEGORY:=Libraries
  TITLE:=SELinux binary policy manipulation library
  URL:=http://selinuxproject.org/page/Main_Page
endef

define Package/libsepol/description
	Libsepol is the binary policy manipulation library. It doesn't
	depend upon or use any of the other SELinux components.
endef #'

include $(BUILD_DIR)/host-build.mk

HOST_MAKE_FLAGS += \
	PREFIX=$(HOST_BUILD_PREFIX) \
	SHLIBDIR=$(HOST_BUILD_PREFIX)/lib

define Host/Install
	PREFIX=$(HOST_BUILD_PREFIX) \
	SHLIBDIR=$(HOST_BUILD_PREFIX)/lib \
		$(MAKE) -C $(HOST_BUILD_DIR) install
endef

$(eval $(call HostBuild))

MAKE_FLAGS += \
	SHLIBDIR=/usr/lib

define Build/Compile
	$(call Build/Compile/Default,all)
endef

define Build/Install
	$(call Build/Install/Default,install)
endef

define Build/InstallDev
	$(INSTALL_DIR) $(1)/usr/include
	$(CP) $(PKG_INSTALL_DIR)/usr/include/* $(1)/usr/include/
	$(INSTALL_DIR) $(1)/usr/lib/pkgconfig
	$(INSTALL_DATA) $(PKG_INSTALL_DIR)/usr/lib/pkgconfig/libsepol.pc $(1)/usr/lib/pkgconfig/
	$(INSTALL_DIR) $(1)/usr/lib
	$(CP) $(PKG_INSTALL_DIR)/usr/lib/* $(1)/usr/lib/
endef

define Package/libsepol/install
	$(INSTALL_DIR) $(1)/usr/lib
	$(CP) $(PKG_INSTALL_DIR)/usr/lib/libsepol.so.* $(1)/usr/lib/
endef

$(eval $(call BuildPackage,libsepol))
