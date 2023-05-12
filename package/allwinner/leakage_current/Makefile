include $(TOPDIR)/rules.mk

PKG_NAME:=leakage_current
PKG_VERSION:=1.0.0
PKG_RELEASE:=1

include $(BUILD_DIR)/package.mk

define Package/$(PKG_NAME)
  SECTION:=utils
  CATEGORY:=Allwinner
  TITLE:=A tool to check leakage current
endef

define Build/Prepare
	mkdir -p $(PKG_BUILD_DIR)
	$(CP) ./files/* $(PKG_BUILD_DIR)
endef

TOOLCHAIN_LIBC_STRING:=$(shell echo -n $(CONFIG_TOOLCHAIN_LIBC))

define Build/Compile
	$(MAKE) -C $(PKG_BUILD_DIR) \
		ARCH="$(TARGET_ARCH)" \
		AR="$(TARGET_AR)" \
		CC="$(TARGET_CC)" \
		CFLAGS="$(TARGET_CFLAGS)" \
		LDFLAGS="$(TARGET_LDFLAGS)" \
		INSTALL_PREFIX="$(PKG_INSTALL_DIR)" \
		CPU_VARIANT="$(TARGET_CPU_VARIANT)" \
		TOOLCHAIN_LIBC="$(TOOLCHAIN_LIBC_STRING)" \
		all
endef

define Package/$(PKG_NAME)/install
	$(INSTALL_DIR) $(1)/usr/bin
	$(INSTALL_BIN) $(PKG_INSTALL_DIR)/usr/bin/* $(1)/usr/bin/
	$(INSTALL_DIR) $(1)/usr/lib
	$(INSTALL_BIN) ./files/lib/$(TARGET_CPU_VARIANT)/$(TOOLCHAIN_LIBC_STRING)/*.so $(1)/usr/lib/
endef

$(eval $(call BuildPackage,$(PKG_NAME)))
