include $(TOPDIR)/rules.mk

PKG_VERSION_MAJOR:=3
PKG_VERSION_MINOR:=2.25

PKG_NAME:=zhouyi-lib
PKG_VERSION:=r1p0-eac0-$(PKG_VERSION_MAJOR).$(PKG_VERSION_MINOR)
PKG_RELEASE:=1

include $(BUILD_DIR)/package.mk

define Package/$(PKG_NAME)
  CATEGORY:=Thirdparty
  TITLE:=Zhouyi AIPU UMD Library
  DEPENDS:=+libstdcpp
endef

define Package/$(PKG_NAME)/description
  ARM CHINA Zhouyi AIPU UMD (User Mode Driver) library
endef

define Build/Prepare
	mkdir -p $(PKG_BUILD_DIR)
	$(CP) ./files/* $(PKG_BUILD_DIR)
endef

define Build/Compile
	$(MAKE) -C $(PKG_BUILD_DIR) \
		ARCH="$(TARGET_ARCH)" \
		AR="$(TARGET_AR)" \
		CXX="$(TARGET_CXX)" \
		CXXFLAGS="$(TARGET_CXXFLAGS)" \
		LDFLAGS="$(TARGET_LDFLAGS)" \
		INSTALL_PREFIX="$(PKG_INSTALL_DIR)" \
		MAJOR="$(PKG_VERSION_MAJOR)" \
		MINOR="$(PKG_VERSION_MINOR)" \
		all
endef

define Build/InstallDev
	$(INSTALL_DIR) $(1)/usr/include
	$(CP) $(PKG_INSTALL_DIR)/usr/include/* $(1)/usr/include
	$(INSTALL_DIR) $(1)/usr/lib
	$(CP) $(PKG_INSTALL_DIR)/usr/lib/* $(1)/usr/lib
endef

define Package/$(PKG_NAME)/install
	$(INSTALL_DIR) $(1)/usr/lib
	$(CP) $(PKG_INSTALL_DIR)/usr/lib/*.so* $(1)/usr/lib
endef

$(eval $(call BuildPackage,$(PKG_NAME)))
