include $(TOPDIR)/rules.mk

PKG_NAME:=btmanager
PKG_VERSION:=0.0.0
PKG_RELEASE:=1
PKG_BUILD_DIR := $(COMPILE_DIR)/$(PKG_NAME)

PKG_CONFIG_DEPENDS:= \
	CONFIG_USES_BCMDHD \
	CONFIG_USES_XRADIO \
	CONFIG_USES_REALTEK \
	CONFIG_USES_ESP8089

include $(BUILD_DIR)/package.mk

#BT_EXT_CFLAG +=-DDEBUG_LOG_LEVEL=2

define Package/$(PKG_NAME)/Default
  SECTION:=utils
  CATEGORY:=Allwinner
  SUBMENU:=btmanager
endef

define Package/$(PKG_NAME)-core
  $(call Package/btmanager/Default)
  TITLE:=bluetooth manager core
  DEPENDS:=+alsa-lib +bluez-daemon +glib2 +sbc +dbus +bluez-alsa +libjson-c
  KCONFIG:=\
	  CONFIG_HFP_OVER_PCM=y
endef

define Package/$(PKG_NAME)-demo
  $(call Package/btmanager/Default)
  TITLE:=Tina btmanager app demo
  DEPENDS := +btmanager-core +libjson-c
endef

define Package/$(PKG_NAME)-core/description
	bluetooth manager
endef

define Package/$(PKG_NAME)-demo/description
	Tina btmanager app demo
endef

define Build/Prepare
	mkdir -p $(PKG_BUILD_DIR)
	$(CP) -r ./src $(PKG_BUILD_DIR)/
	$(CP) -r ./demo $(PKG_BUILD_DIR)/
endef

TARGET_LDFLAGS += $(if $(CONFIG_USE_GLIBC),-lpthread -lz -lrt -lm)

define Build/Configure
endef
ifeq ($(CONFIG_PACKAGE_btmanager-core),y)
define Build/Compile/$(PKG_NAME)-core
	$(MAKE) -C $(PKG_BUILD_DIR)/src/ \
		PKG_BUILD_DIR="$(PKG_BUILD_DIR)" \
		ARCH="$(TARGET_ARCH)" \
		AR="$(TARGET_AR)" \
		CC="$(TARGET_CC)" \
		CXX="$(TARGET_CXX)" \
		CFLAGS="$(TARGET_CFLAGS)" \
		BT_EXT_CFLAG="$(BT_EXT_CFLAG)" \
		LDFLAGS="$(TARGET_LDFLAGS)" \
		STAGING_DIR="$(STAGING_DIR)" \
		CONFIG_LIBC="$(CONFIG_LIBC)" \
		CPU="$(ARCH)" \
		CONFIG_PREFIX="$(PKG_INSTALL_DIR)" \
		all
endef
endif

ifeq ($(CONFIG_PACKAGE_btmanager-demo),y)
define Build/Compile/$(PKG_NAME)-demo
	$(MAKE) -C $(PKG_BUILD_DIR)/demo/ \
		ARCH="$(TARGET_ARCH)" \
		AR="$(TARGET_AR)" \
		CC="$(TARGET_CC)" \
		CXX="$(TARGET_CXX)" \
		CFLAGS="$(TARGET_CFLAGS)" \
		LDFLAGS="$(TARGET_LDFLAGS)" \
		CONFIG_PREFIX="$(PKG_INSTALL_DIR)" \
		all

endef
endif
define Build/Compile
	$(Build/Compile/$(PKG_NAME)-core)
	$(Build/Compile/$(PKG_NAME)-demo)
endef

define Build/InstallDev
	$(INSTALL_DIR) $(1)/lib
	$(INSTALL_DIR) $(1)/usr/include
	$(INSTALL_DATA) $(PKG_INSTALL_DIR)/usr/lib/libbtmg.so $(1)/lib
endef

define Package/$(PKG_NAME)-core/install
	$(INSTALL_DIR) $(1)/lib
	$(INSTALL_DIR) $(1)/etc/bluetooth
	$(INSTALL_DATA) $(PKG_INSTALL_DIR)/usr/lib/libbtmg.so $(1)/lib
ifeq ($(CONFIG_USES_XRADIO), y)
	$(INSTALL_BIN) ./config/xradio_bt_init.sh $(1)/etc/bluetooth/bt_init.sh
endif
ifeq ($(CONFIG_USES_REALTEK), y)
	$(INSTALL_BIN) ./config/realtek_bt_init.sh $(1)/etc/bluetooth/bt_init.sh
endif
	$(INSTALL_DATA) ./config/bluetooth.json $(1)/etc/bluetooth/bluetooth.json
endef

define Package/$(PKG_NAME)-demo/install
	$(INSTALL_DIR) $(1)/usr/bin
	$(INSTALL_BIN) $(PKG_INSTALL_DIR)/usr/bin/bt_test $(1)/usr/bin
	$(INSTALL_BIN) $(PKG_INSTALL_DIR)/usr/bin/bt_gatt_server_test $(1)/usr/bin
	$(INSTALL_BIN) $(PKG_INSTALL_DIR)/usr/bin/bt_gatt_client_test $(1)/usr/bin
endef
$(eval $(call BuildPackage,$(PKG_NAME)-core))
$(eval $(call BuildPackage,$(PKG_NAME)-demo))
