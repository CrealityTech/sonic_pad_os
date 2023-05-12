include $(TOPDIR)/rules.mk

PKG_NAME:=zhouyi-onnx-lib
PKG_VERSION:=r1p1-dev3
PKG_RELEASE:=1

include $(BUILD_DIR)/package.mk

define Package/$(PKG_NAME)
  CATEGORY:=Thirdparty
  TITLE:=Zhouyi AIPU ONNX Runtime Library
  DEPENDS:=+libstdcpp
endef

define Package/$(PKG_NAME)/description
  ARM CHINA Zhouyi AIPU UMD (User Mode Driver) ONNX Runtime library
endef

define Build/Prepare
	mkdir -p $(PKG_BUILD_DIR)
	$(CP) ./files/* $(PKG_BUILD_DIR)
endef

define Build/Compile
	:
endef

define Build/InstallDev
	$(INSTALL_DIR) $(1)/usr/include
	$(CP) $(PKG_BUILD_DIR)/include/* $(1)/usr/include
	$(INSTALL_DIR) $(1)/usr/lib
	$(CP) $(PKG_BUILD_DIR)/libs/* $(1)/usr/lib
endef

define Package/$(PKG_NAME)/install
	$(INSTALL_DIR) $(1)/usr/lib
	$(CP) $(PKG_BUILD_DIR)/libs/*.so* $(1)/usr/lib
endef

$(eval $(call BuildPackage,$(PKG_NAME)))
