include $(TOPDIR)/rules.mk
include $(BUILD_DIR)/kernel.mk
include $(BUILD_DIR)/package.mk

PKG_NAME:=smartva
PKG_VERSION:=1.0
PKG_RELEASE:=1

PKG_BUILD_DIR := $(COMPILE_DIR)/$(PKG_NAME)

define Package/$(PKG_NAME)
  SECTION:=gui
  CATEGORY:=littlevgl
  DEPENDS:= +libcedarx +alsa-utils +wifimanager
  TITLE:=smartva for pdc-pd1
  MAINTAINER:=huangyixiu <huangyixiu@allwinnertech.com>
endef

define Package/$(PKG_NAME)/Default
endef

define Package/$(PKG_NAME)/description
  smartva for pdc-pd1
endef

define Package/$(PKG_NAME)/config
  source "$(SOURCE)/Config.in"
endef

ifeq ($(CONFIG_lvgl),y)
CONFIGURE_PRJ += ./../lvgl
endif

ifeq ($(CONFIG_lv_drivers),y)
CONFIGURE_PRJ += ./../lv_drivers
endif

ifeq ($(CONFIG_lv_examples),y)
CONFIGURE_PRJ += ./../lv_examples
endif

CONFIGURE_PRJ += ./src/common

#ifeq ($(CONFIG_lv_story),y)
CONFIGURE_PRJ += ./src/story
CONFIGURE_APP += ./src/story
CONFIGURE_BIN += story_bin
#endif

define Build/Prepare
	$(INSTALL_DIR) $(PKG_BUILD_DIR)/
	$(INSTALL_DIR) $(PKG_BUILD_DIR)/src
	$(CP) $(CONFIGURE_PRJ) $(PKG_BUILD_DIR)/src/
	$(CP) $(CONFIGURE_APP)/config/* $(PKG_BUILD_DIR)/src/
	$(CP) $(CONFIGURE_APP)/Makefile $(PKG_BUILD_DIR)/src/

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

define Package/$(PKG_NAME)/install
	$(INSTALL_DIR) $(1)/usr/bin/
	$(INSTALL_DIR) $(1)/usr/res/
	$(INSTALL_DIR) $(1)/usr/res/image/

	$(INSTALL_BIN) $(PKG_BUILD_DIR)/src/$(CONFIGURE_BIN) $(1)/usr/bin/
	$(INSTALL_BIN) $(PKG_BUILD_DIR)/src/story/designer/image/res/* $(1)/usr/res/image/
endef

$(eval $(call BuildPackage,$(PKG_NAME)))
