#
# Copyright (C) 2006-2016 OpenWrt.org
#
# This is free software, licensed under the GNU General Public License v2.
# See /LICENSE for more information.
#

include $(TOPDIR)/rules.mk

PKG_NAME:=bluez
PKG_VERSION:=5.54
PKG_RELEASE:=1

PKG_SOURCE:=$(PKG_NAME)-$(PKG_VERSION).tar.xz
PKG_SOURCE_URL:=@KERNEL/linux/bluetooth/
PKG_HASH:=ebedfb359f62957940822f1d0b39fcee30422380e435608dad06bb3913d5ebba

PKG_LICENSE:=GPL-2.0-or-later
PKG_LICENSE_FILES:=COPYING
PKG_MAINTAINER:=
PKG_CPE_ID:=cpe:/a:bluez:bluez

PKG_BUILD_PARALLEL:=1
PKG_INSTALL:=1

include $(BUILD_DIR)/package.mk
include $(BUILD_DIR)/nls.mk

define Package/bluez/Default
  TITLE:=Bluetooth
  URL:=http://www.bluez.org/
endef

define Package/bluez-libs
$(call Package/bluez/Default)
  SECTION:=libs
  CATEGORY:=Libraries
  TITLE+= library
  DEPENDS:=+libpthread
endef

define Package/bluez-utils
$(call Package/bluez/Default)
  SECTION:=utils
  CATEGORY:=Utilities
  TITLE+= utilities
  DEPENDS:=+bluez-libs
endef

define Package/bluez-utils-extra
$(call Package/bluez/Default)
  SECTION:=utils
  CATEGORY:=Utilities
  TITLE+= additional utilities
  DEPENDS:=+bluez-libs +libpthread +librt +glib2 +libncurses +libreadline $(INTL_DEPENDS) $(ICONV_DEPENDS) +dbus +libjson-c
endef

define Package/bluez-daemon
$(call Package/bluez/Default)
  SECTION:=utils
  CATEGORY:=Utilities
  TITLE+= daemon
  DEPENDS:=+bluez-libs +bluez-utils +bluez-utils-extra +ell +libical +glib2 +libncurses +libreadline +dbus +libjson-c +libical $(INTL_DEPENDS) $(ICONV_DEPENDS)
endef

define Package/bluez-daemon/config
config XR829_BT
    bool "Enable xr829 extra config"
    default n
config XR829_BT_LPM
	depends on XR829_BT
    bool "Enable xr829 lpm"
    default n
endef


define Package/bluez-daemon/conffiles
/etc/bluetooth/main.conf
/etc/bluetooth/network.conf
/etc/bluetooth/input.conf
/etc/bluetooth/keys
/etc/config/bluetooth
endef

TARGET_CFLAGS += -D_GNU_SOURCE -ffunction-sections -fdata-sections
TARGET_LDFLAGS += -Wl,--gc-sections

ifeq ($(CONFIG_XR829_BT_LPM),y)
TARGET_CPPFLAGS=-DCONFIG_XR829_BT_LPM
endif

PKG_CONFIG_DEPENDS +=CONFIG_XR829_BT
PKG_CONFIG_DEPENDS +=CONFIG_XR829_BT_LPM

TARGET_LDFLAGS += $(if $(CONFIG_USE_GLIBC),-lpthread)

CONFIGURE_ARGS += \
	--enable-static \
	--enable-shared \
	--enable-client \
	--enable-datafiles \
	--enable-experimental \
	--enable-library \
	--enable-monitor \
	--enable-obex \
	--enable-threads \
	--enable-tools \
	--disable-android \
	--disable-cups \
	--disable-manpages \
	--disable-sixaxis \
	--disable-systemd \
	--disable-test \
	--disable-udev \
	--enable-deprecated \
	--enable-mesh \
	--localstatedir=/etc

define Build/InstallDev
	$(INSTALL_DIR) $(1)/usr/include
	$(CP) $(PKG_INSTALL_DIR)/usr/include/bluetooth $(1)/usr/include/
	$(INSTALL_DIR) $(1)/usr/include/src/shared
	$(CP) $(PKG_BUILD_DIR)/src/shared/*.h $(1)/usr/include/src/shared
	$(CP) $(PKG_BUILD_DIR)/monitor/bt.h $(1)/usr/include/src/shared
	$(INSTALL_DIR) $(1)/usr/include/lib
	$(CP) $(PKG_BUILD_DIR)/lib/*.h $(1)/usr/include/lib
	$(INSTALL_DIR) $(1)/usr/lib
	$(CP) $(PKG_INSTALL_DIR)/usr/lib/libbluetooth.{a,so*} $(1)/usr/lib/
	$(CP) $(PKG_BUILD_DIR)/src/.libs/*.a $(1)/usr/lib
	$(CP) $(PKG_BUILD_DIR)/lib/.libs/*.a $(1)/usr/lib
	$(INSTALL_DIR) $(1)/usr/lib/pkgconfig
	$(CP) $(PKG_INSTALL_DIR)/usr/lib/pkgconfig/bluez.pc $(1)/usr/lib/pkgconfig/
endef

define Package/bluez-libs/install
	$(INSTALL_DIR) $(1)/usr/lib
	$(CP) $(PKG_INSTALL_DIR)/usr/lib/libbluetooth.so.* $(1)/usr/lib/
endef

define Package/bluez-utils/install
	$(INSTALL_DIR) $(1)/usr/bin
	$(INSTALL_BIN) $(PKG_BUILD_DIR)/tools/bdaddr $(1)/usr/bin/
	$(INSTALL_BIN) $(PKG_INSTALL_DIR)/usr/bin/bccmd $(1)/usr/bin/
	$(INSTALL_BIN) $(PKG_INSTALL_DIR)/usr/bin/bluemoon $(1)/usr/bin/
	$(INSTALL_BIN) $(PKG_INSTALL_DIR)/usr/bin/btattach $(1)/usr/bin/
	$(INSTALL_BIN) $(PKG_INSTALL_DIR)/usr/bin/btmon $(1)/usr/bin/
	$(INSTALL_BIN) $(PKG_INSTALL_DIR)/usr/bin/ciptool $(1)/usr/bin/
	$(INSTALL_BIN) $(PKG_INSTALL_DIR)/usr/bin/hciattach $(1)/usr/bin/
	$(INSTALL_BIN) $(PKG_INSTALL_DIR)/usr/bin/hciconfig $(1)/usr/bin/
	$(INSTALL_BIN) $(PKG_INSTALL_DIR)/usr/bin/hcidump $(1)/usr/bin/
	$(INSTALL_BIN) $(PKG_INSTALL_DIR)/usr/bin/hcitool $(1)/usr/bin/
	$(INSTALL_BIN) $(PKG_INSTALL_DIR)/usr/bin/hex2hcd $(1)/usr/bin/
	$(INSTALL_BIN) $(PKG_INSTALL_DIR)/usr/bin/l2ping $(1)/usr/bin/
	$(INSTALL_BIN) $(PKG_INSTALL_DIR)/usr/bin/l2test $(1)/usr/bin/
	$(INSTALL_BIN) $(PKG_INSTALL_DIR)/usr/bin/rctest $(1)/usr/bin/
	$(INSTALL_BIN) $(PKG_INSTALL_DIR)/usr/bin/rfcomm $(1)/usr/bin/
endef

define Package/bluez-utils-extra/install
	$(INSTALL_DIR) $(1)/usr/bin
	$(INSTALL_BIN) $(PKG_BUILD_DIR)/tools/btmgmt $(1)/usr/bin/
	$(INSTALL_BIN) $(PKG_INSTALL_DIR)/usr/bin/mpris-proxy $(1)/usr/bin/
	$(INSTALL_BIN) $(PKG_INSTALL_DIR)/usr/bin/sdptool $(1)/usr/bin/
	$(INSTALL_BIN) $(PKG_BUILD_DIR)/attrib/gatttool $(1)/usr/bin/
endef

define Package/bluez-daemon/install
	$(INSTALL_DIR) $(1)/usr/bin
	$(INSTALL_BIN) $(PKG_INSTALL_DIR)/usr/lib/bluetooth/bluetoothd $(1)/usr/bin/
	$(INSTALL_BIN) $(PKG_INSTALL_DIR)/usr/bin/bluetoothctl $(1)/usr/bin/
	$(INSTALL_BIN) $(PKG_INSTALL_DIR)/usr/lib/bluetooth/obexd $(1)/usr/bin/
	$(CP) $(PKG_INSTALL_DIR)/usr/lib/bluetooth/bluetooth-meshd $(1)/usr/bin/
	$(INSTALL_BIN) $(PKG_INSTALL_DIR)/usr/bin/meshctl $(1)/usr/bin/
	$(INSTALL_DIR) $(1)/etc/dbus-1/system.d/
	$(INSTALL_DATA) ./files/bluetooth-mesh-adapter.conf $(1)/etc/dbus-1/system.d/bluetooth-mesh-adapter.conf
	$(INSTALL_DIR) $(1)/etc/bluetooth/mesh
	$(INSTALL_DATA) ./files/local_node.json $(1)/etc/bluetooth/mesh/local_node.json
	$(INSTALL_DATA) ./files/prov_db.json $(1)/etc/bluetooth/mesh/prov_db.json
	$(INSTALL_DIR) $(1)/etc/lib/bluetooth/mesh
	$(INSTALL_DATA) ./files/local_node.json $(1)/etc/lib/bluetooth/mesh/local_node.json
	$(INSTALL_DATA) ./files/prov_db.json $(1)/etc/lib/bluetooth/mesh/prov_db.json
	$(INSTALL_DATA) $(PKG_BUILD_DIR)/mesh/bluetooth-mesh.conf $(1)/etc/dbus-1/system.d/bluetooth-mesh.conf
	$(INSTALL_DATA) $(PKG_BUILD_DIR)/src/bluetooth.conf $(1)/etc/dbus-1/system.d/bluetooth.conf
	$(INSTALL_DIR) $(1)/etc/bluetooth
	$(INSTALL_DIR) $(1)/etc/bluetooth/keys
	$(INSTALL_DATA) $(PKG_BUILD_DIR)/profiles/network/network.conf $(1)/etc/bluetooth/network.conf
	$(INSTALL_DATA) $(PKG_BUILD_DIR)/profiles/input/input.conf $(1)/etc/bluetooth/input.conf
	$(INSTALL_BIN) ./files/bluetoothd.init $(1)/etc/bluetooth/bluetoothd
	$(INSTALL_DATA) ./files/main.conf $(1)/etc/bluetooth/main.conf

ifeq ($(CONFIG_XR829_BT),y)
	$(INSTALL_BIN) ./files/hcidump_xr $(1)/usr/bin
endif

endef

$(eval $(call BuildPackage,bluez-libs))
$(eval $(call BuildPackage,bluez-utils))
$(eval $(call BuildPackage,bluez-utils-extra))
$(eval $(call BuildPackage,bluez-daemon))
