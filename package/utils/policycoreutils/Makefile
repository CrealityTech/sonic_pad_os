#
# This is free software, licensed under the GNU General Public License v2.
# See /LICENSE for more information.
#

include $(TOPDIR)/rules.mk

PKG_NAME:=policycoreutils
PKG_VERSION:=2.9
PKG_RELEASE:=1

PKG_SOURCE:=$(PKG_NAME)-$(PKG_VERSION).tar.gz
PKG_SOURCE_URL:=https://github.com/SELinuxProject/selinux/releases/download/20190315
PKG_HASH:=c53c344f28007b3c0742bd958751e9b5d2385898adeb8aec6281ae57342f0f7b
PKG_INSTALL:=1
HOST_BUILD_DEPENDS:=libsemanage/host

PKG_MAINTAINER:=Thomas Petazzoni <thomas.petazzoni@bootlin.com>

include $(BUILD_DIR)/package.mk

define Package/policycoreutils
  SECTION:=utils
  CATEGORY:=Utilities
  DEPENDS:= +libsemanage +libcap-ng
  TITLE:=SELinux policy utilities
  URL:=http://selinuxproject.org/page/Main_Page
endef

define Package/policycoreutils/description
	Policycoreutils is a collection of policy utilities
	(originally the "core" set of utilities needed to use
	SELinux, although it has grown a bit over time), which have
	different dependencies.  sestatus, secon, run_init, and
	newrole only use libselinux. load_policy and setfiles only
	use libselinux and libsepol. semodule and semanage use
	libsemanage (and thus bring in dependencies on libsepol and
	libselinux as well). setsebool uses libselinux to make
	non-persistent boolean changes (via the kernel interface)
	and uses libsemanage to make persistent boolean changes.
endef

include $(BUILD_DIR)/host-build.mk

HOST_LDFLAGS += -Wl,-rpath="$(HOST_BUILD_PREFIX)/lib:$(HOST_BUILD_PREFIX)/usr/lib"
HOST_MAKE_FLAGS += \
	PREFIX=$(HOST_BUILD_PREFIX) \
	SBINDIR=$(HOST_BUILD_PREFIX)/sbin \
	ETCDIR=$(HOST_BUILD_PREFIX)/etc \
	CFLAGS="$(HOST_CPPFLAGS)" \
	LDFLAGS="$(HOST_LDFLAGS)"

define Host/Install
	$(MAKE) -C $(HOST_BUILD_DIR) \
		PREFIX=$(HOST_BUILD_PREFIX) \
		SBINDIR=$(HOST_BUILD_PREFIX)/sbin \
		ETCDIR=$(HOST_BUILD_PREFIX)/etc \
		install
endef

TARGET_LDFLAGS += $(if $(CONFIG_USE_GLIBC),-ldl)

define Package/policycoreutils/install
	$(INSTALL_DIR) $(1)/usr/bin
	$(CP) $(PKG_INSTALL_DIR)/usr/bin/* $(1)/usr/bin/
	$(INSTALL_DIR) $(1)/usr/sbin
	$(CP) $(PKG_INSTALL_DIR)/usr/sbin/* $(1)/usr/sbin/
	$(INSTALL_DIR) $(1)/sbin
	$(CP) $(PKG_INSTALL_DIR)/sbin/* $(1)/sbin/
endef

$(eval $(call HostBuild))
$(eval $(call BuildPackage,policycoreutils))
