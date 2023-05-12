include $(TOPDIR)/rules.mk

PKG_NAME:=rtk_mesh
PKG_VERSION:=1.0
PKG_RELEASE:=1

PKG_BUILD_DIR := $(COMPILE_DIR)/$(PKG_NAME)

include $(BUILD_DIR)/package.mk

define Package/$(PKG_NAME)
  CATEGORY:=Thirdparty
  TITLE:=rtk mesh demo
  DEPENDS:=+bluez-daemon
endef

define Package/$(PKG_NAME)/description
	A test program for rtk mesh protcol
endef

define Build/Prepare
	mkdir -p $(PKG_BUILD_DIR)
	$(CP) -r ./mesh-lib $(PKG_BUILD_DIR)/
	$(CP) -r ./lib/$(ARCH)/$(CONFIG_LIBC)/libmesh-lib.so $(PKG_BUILD_DIR)/
	$(CP) -r ./mesh-wrapper $(PKG_BUILD_DIR)/
	$(CP) -r ./mesh_thread $(PKG_BUILD_DIR)/
endef

define Build/Compile
	$(MAKE) -C $(PKG_BUILD_DIR)/mesh-wrapper \
		ARCH="$(TARGET_ARCH)" \
		AR="$(TARGET_AR)" \
		CC="$(TARGET_CC)" \
		CXX="$(TARGET_CXX)" \
		CFLAGS="$(TARGET_CFLAGS)" \
		CXXFLAGS="$(TARGET_CXXFLAGS)" \
		LDFLAGS="$(TARGET_LDFLAGS)" \
		all

	$(MAKE) -C $(PKG_BUILD_DIR)/mesh_thread \
		ARCH="$(TARGET_ARCH)" \
		AR="$(TARGET_AR)" \
		CC="$(TARGET_CC)" \
		CXX="$(TARGET_CXX)" \
		CFLAGS="$(TARGET_CFLAGS)" \
		CXXFLAGS="$(TARGET_CXXFLAGS)" \
		LDFLAGS="$(TARGET_LDFLAGS)" \
		all

endef

define Package/$(PKG_NAME)/install
	$(INSTALL_DIR) $(1)/lib
	$(INSTALL_DIR) $(1)/usr/bin
	$(INSTALL_BIN) $(PKG_BUILD_DIR)/*.so $(1)/lib
	$(INSTALL_BIN) $(PKG_BUILD_DIR)/mesh_thread_provisioner $(1)/usr/bin/
endef

$(eval $(call BuildPackage,$(PKG_NAME)))
