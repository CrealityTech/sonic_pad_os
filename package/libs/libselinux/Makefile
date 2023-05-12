#
# This is free software, licensed under the GNU General Public License v2.
# See /LICENSE for more information.
#

include $(TOPDIR)/rules.mk

PKG_NAME:=libselinux
PKG_VERSION:=2.9
PKG_RELEASE:=1

PKG_SOURCE:=$(PKG_NAME)-$(PKG_VERSION).tar.gz
PKG_SOURCE_URL:=https://github.com/SELinuxProject/selinux/releases/download/20190315
PKG_HASH:=1bccc8873e449587d9a2b2cf253de9b89a8291b9fbc7c59393ca9e5f5f4d2693
PKG_BUILD_DEPENDS:=PACKAGE_python-libselinux:python PACKAGE_python-libselinux:swig/host
HOST_BUILD_DEPENDS:=libsepol/host pcre/host

PKG_MAINTAINER:=Thomas Petazzoni <thomas.petazzoni@bootlin.com>

include $(BUILD_DIR)/package.mk
include ../../lang/python/files/python-package.mk

define Package/libselinux
  SECTION:=libs
  DEPENDS:=+libsepol +libpcre +musl-fts
  CATEGORY:=Libraries
  TITLE:=Runtime SELinux library
  URL:=http://selinuxproject.org/page/Main_Page
endef

define Package/python-libselinux
  TITLE:=Python bindings sur the runtime SELinux library
  SUBMENU:=Python
  SECTION:=lang
  CATEGORY:=Languages
  DEPENDS:=+python +libselinux
endef

define Package/libselinux-utils
  TITLE:=the runtime SELinux library utils
  SECTION:=libs
  CATEGORY:=Libraries
  DEPENDS:=+libselinux
endef

define Package/libselinux/description
	libselinux is the runtime SELinux library that provides
	interfaces (e.g. library functions for the SELinux kernel
	APIs like getcon(), other support functions like
	getseuserbyname()) to SELinux-aware applications. libselinux
	may use the shared libsepol to manipulate the binary policy
	if necessary (e.g. to downgrade the policy format to an
	older version supported by the kernel) when loading policy.
endef

include $(BUILD_DIR)/host-build.mk

# Needed to link libselinux utilities, which link against
# libselinux.so, which indirectly depends on libpcre.so, installed in
# $(HOST_BUILD_PREFIX).
HOST_LDFLAGS += -Wl,-rpath="$(HOST_BUILD_PREFIX)/lib"

HOST_MAKE_FLAGS += \
	PREFIX=$(HOST_BUILD_PREFIX) \
	SHLIBDIR=$(HOST_BUILD_PREFIX)/lib \
	LDFLAGS="$(HOST_LDFLAGS)"

define Host/Install
	PREFIX=$(HOST_BUILD_PREFIX) \
	SHLIBDIR=$(HOST_BUILD_PREFIX)/lib \
		$(MAKE) -C $(HOST_BUILD_DIR) install
endef

$(eval $(call HostBuild))

TARGET_LDFLAGS+=$(if $(CONFIG_USE_GLIBC),-ldl)

MAKE_FLAGS += \
	FTS_LDLIBS=-lfts \
	SHLIBDIR=/usr/lib \
	PYTHON=$(PYTHON) \
	PYINC="-I $(PYTHON_INC_DIR)"

ifdef CONFIG_PACKAGE_python-libselinux
  define Build/Compile/python-libselinux
	$(call Build/Compile/Default,swigify pywrap)
  endef

  define Build/Install/python-libselinux
	$(call Build/Install/Default,install-pywrap)
  endef
endif

define Build/Compile
	$(call Build/Compile/Default,all)
	$(Build/Compile/python-libselinux)
endef

define Build/Install
	$(call Build/Install/Default,install)
	$(Build/Install/python-libselinux)
endef

define Build/InstallDev
	$(INSTALL_DIR) $(1)/usr/include
	$(CP) $(PKG_INSTALL_DIR)/usr/include/* $(1)/usr/include/
	$(INSTALL_DIR) $(1)/usr/lib/pkgconfig
	$(INSTALL_DATA) $(PKG_INSTALL_DIR)/usr/lib/pkgconfig/libselinux.pc $(1)/usr/lib/pkgconfig/
	$(INSTALL_DIR) $(1)/usr/lib
	$(CP) $(PKG_INSTALL_DIR)/usr/lib/* $(1)/usr/lib/
endef

define Package/libselinux/install
	$(INSTALL_DIR) $(1)/usr/lib
	$(CP) $(PKG_INSTALL_DIR)/usr/lib/libselinux.so* $(1)/usr/lib/
endef

define Package/libselinux-utils/install
	$(INSTALL_DIR) $(1)/usr/sbin
	$(CP) $(PKG_INSTALL_DIR)/usr/sbin/* $(1)/usr/sbin/
endef

$(eval $(call BuildPackage,libselinux))
$(eval $(call PyPackage,python-libselinux))
$(eval $(call BuildPackage,python-libselinux))
$(eval $(call BuildPackage,libselinux-utils))
