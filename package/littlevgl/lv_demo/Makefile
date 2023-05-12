include $(TOPDIR)/rules.mk
include $(BUILD_DIR)/kernel.mk
include $(BUILD_DIR)/package.mk

PKG_NAME:=lv_demo
PKG_VERSION:=6.0
PKG_RELEASE:=1

PKG_BUILD_DIR := $(COMPILE_DIR)/$(PKG_NAME)

define Package/$(PKG_NAME)
  SECTION:=gui
  CATEGORY:=littlevgl
  TITLE:=a lvgl demo
  MAINTAINER:=huangyixiu <huangyixiu@allwinnertech.com>
endef

define Package/$(PKG_NAME)/Default
endef

define Package/$(PKG_NAME)/description
  a lvgl demo
endef

#$(CP) ./../lv_drivers $(PKG_BUILD_DIR)/src/
#$(CP) ./../lv_examples $(PKG_BUILD_DIR)/src/

define Build/Prepare
	$(INSTALL_DIR) $(PKG_BUILD_DIR)/
	$(CP) ./src $(PKG_BUILD_DIR)/
	$(CP) ./../lvgl $(PKG_BUILD_DIR)/src/
	$(CP) ./../lv_drivers $(PKG_BUILD_DIR)/src/
	$(CP) ./../lv_examples $(PKG_BUILD_DIR)/src/
endef

define Build/Configure
endef

define Build/Compile
	$(MAKE) -C $(PKG_BUILD_DIR)/src\
		ARCH="$(TARGET_ARCH)" \
		AR="$(TARGET_AR)" \
		CC="$(TARGET_CC)" \
		CXX="$(TARGET_CXX)" \
		CFLAGS="$(TARGET_CFLAGS)" \
		LDFLAGS="$(TARGET_LDFLAGS)" \
		INSTALL_PREFIX="$(PKG_INSTALL_DIR)" \
		all
endef

#define Build/InstallDev
#	$(INSTALL_DIR) $(1)/usr/lib
#endef

define Package/$(PKG_NAME)/install
	$(INSTALL_DIR) $(1)/usr/bin/
	$(INSTALL_BIN) $(PKG_BUILD_DIR)/src/$(PKG_NAME) $(1)/usr/bin/
endef

$(eval $(call BuildPackage,$(PKG_NAME)))
