#
# This is free software, licensed under the GNU General Public License v2.
# See /LICENSE for more information.
#

include $(TOPDIR)/rules.mk

PKG_NAME:=libcap-ng
PKG_VERSION:=0.7.9
PKG_RELEASE:=1

PKG_SOURCE:=$(PKG_NAME)-$(PKG_VERSION).tar.gz
PKG_SOURCE_URL:=http://people.redhat.com/sgrubb/libcap-ng
PKG_HASH:=4a1532bcf3731aade40936f6d6a586ed5a66ca4c7455e1338d1f6c3e09221328
PKG_INSTALL:=1

PKG_MAINTAINER:=Thomas Petazzoni <thomas.petazzoni@bootlin.com>

PKG_FIXUP:=autoreconf

include $(BUILD_DIR)/package.mk

define Package/libcap-ng
  SECTION:=libs
  CATEGORY:=Libraries
  TITLE:=POSIX capabilities programming library
  URL:=http://people.redhat.com/sgrubb/libcap-ng/
endef

define Package/libcap-ng/description
	The libcap-ng library is intended to make programming with
	posix capabilities much easier than the traditional libcap
	library. It includes utilities that can analyse all currently
	running applications and print out any capabilities and
	whether or not it has an open ended bounding set.
endef #'

CONFIGURE_ARGS += --without-python
CONFIGURE_VARS += ac_cv_prog_swig_found=no

define Build/InstallDev
	$(INSTALL_DIR) $(1)/usr/include
	$(CP) $(PKG_INSTALL_DIR)/usr/include/* $(1)/usr/include/
	$(INSTALL_DIR) $(1)/usr/lib/pkgconfig
	$(INSTALL_DATA) $(PKG_INSTALL_DIR)/usr/lib/pkgconfig/libcap-ng.pc $(1)/usr/lib/pkgconfig/
	$(INSTALL_DIR) $(1)/usr/lib
	$(CP) $(PKG_INSTALL_DIR)/usr/lib/* $(1)/usr/lib/
endef

define Package/libcap-ng/install
	$(INSTALL_DIR) $(1)/usr/lib
	$(CP) $(PKG_INSTALL_DIR)/usr/lib/libcap-ng.so.* $(1)/usr/lib/
endef

$(eval $(call BuildPackage,libcap-ng))
