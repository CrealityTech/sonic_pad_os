#
# This is free software, licensed under the GNU General Public License v2.
# See /LICENSE for more information.
#

include $(TOPDIR)/rules.mk

PKG_NAME:=libsemanage
PKG_VERSION:=2.9
PKG_RELEASE:=1

PKG_SOURCE:=$(PKG_NAME)-$(PKG_VERSION).tar.gz
PKG_SOURCE_URL:=https://github.com/SELinuxProject/selinux/releases/download/20190315
PKG_HASH:=2576349d344492e73b468059767268dec1dabd8c35f3c7222c3ec2448737bc1c
HOST_BUILD_DEPENDS:=audit/host libselinux/host bzip2/host

PKG_MAINTAINER:=Thomas Petazzoni <thomas.petazzoni@bootlin.com>

include $(BUILD_DIR)/package.mk

define Package/libsemanage
  SECTION:=libs
  DEPENDS:=+libaudit +libselinux +libbz2
  CATEGORY:=Libraries
  TITLE:=SELinux policy management library
  URL:=http://selinuxproject.org/page/Main_Page
endef

define Package/libsemanage/description
	libsemanage is the policy management library. It uses
	libsepol for binary policy manipulation and libselinux for
	interacting with the SELinux system. It also exec's helper
	programs for loading policy and for checking whether the
	file_contexts configuration is valid (load_policy and
	setfiles from policycoreutils) presently, although this may
	change at least for the bootstrapping case (for rpm).
endef #'

include $(BUILD_DIR)/host-build.mk

HOST_MAKE_FLAGS += \
	PREFIX=$(HOST_BUILD_PREFIX) \
	SHLIBDIR=$(HOST_BUILD_PREFIX)/lib \
	LDFLAGS="$(HOST_LDFLAGS)" \
	CFLAGS="$(HOST_CPPFLAGS)"

define Host/Install
	PREFIX=$(HOST_BUILD_PREFIX) \
	SHLIBDIR=$(HOST_BUILD_PREFIX)/lib \
		$(MAKE) -C $(HOST_BUILD_DIR) install
endef

define Build/Configure
endef

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
	$(INSTALL_DATA) $(PKG_INSTALL_DIR)/usr/lib/pkgconfig/libsemanage.pc $(1)/usr/lib/pkgconfig/
	$(INSTALL_DIR) $(1)/usr/lib
	$(CP) $(PKG_INSTALL_DIR)/usr/lib/* $(1)/usr/lib/
endef

define Package/libsemanage/install
	$(INSTALL_DIR) $(1)/usr/lib
	$(CP) $(PKG_INSTALL_DIR)/usr/lib/libsemanage.so.* $(1)/usr/lib/
endef

$(eval $(call HostBuild))
$(eval $(call BuildPackage,libsemanage))
