#
# Copyright (C) 2011 OpenWrt.org
#
# This is free software, licensed under the GNU General Public License v2.
# See /LICENSE for more information.
#

include $(TOPDIR)/rules.mk
include ../version.mk

PKG_NAME:=moonraker
PKG_VERSION:=0.0.71
PKG_RELEASE:=1

PKG_PROJECT_NAME:=$(PKG_NAME)
PKG_SOURCE:=${PKG_NAME}-$(PKG_VERSION).tar.gz
PKG_SOURCE_SUBDIR:=$(PKG_NAME)-$(PKG_VERSION)/$(PKG_PROJECT_NAME)
PKG_SOURCE_PROTO:=git
PKG_SOURCE_VERSION:=V$(PKG_VERSION)

include $(BUILD_DIR)/package.mk

# copy from $(BUILD_DIR)/download.mk, keep .git dir cause we need
define DownloadMethod/git
	$(call wrap_mirror, \
		echo "Checking out files from the git repository..."; \
		mkdir -p $(TMP_DIR)/dl && \
		cd $(TMP_DIR)/dl && \
		rm -rf $(SUBDIR) && \
		[ \! -d $(SUBDIR) ] && \
		git clone $(URL) $(SUBDIR) --recursive && \
		(cd $(SUBDIR) && git checkout $(VERSION) && git submodule update) && \
		echo "Packing checkout..." && \
		$(call dl_pack,$(TMP_DIR)/dl/$(FILE),$(SUBDIR)) && \
		mv $(TMP_DIR)/dl/$(FILE) $(DL_DIR)/ && \
		rm -rf $(SUBDIR); \
	)
endef

define Package/$(PKG_NAME)
  TITLE:=Web API Server for Klipper
  SECTION:=base
  CATEGORY:=Creality
  DEPENDS:=+python3 +python3-pillow +python3-lmdb +python3-streaming-form-data +libsodium +ip-full
endef

define Package/$(PKG_NAME)/description
  Moonraker is a Python 3 based web server that exposes APIs with which client applications may use to interact with Klipper.
  Communcation between the Klippy host and Moonraker is done over a Unix Domain Socket. Tornado is used to provide Moonraker's server functionality.
endef

define Build/Prepare
	rm -rf $(PKG_BUILD_DIR)/
	mkdir -p $(PKG_BUILD_DIR)/$(PKG_NAME)
	$(TAR) -xzf $(DL_DIR)/$(PKG_SOURCE) -C $(PKG_BUILD_DIR)/ --strip-components 1
	$(TAR) -xzf ./files/moonraker-env.tar.gz -C $(PKG_BUILD_DIR)
endef

define Build/Compile

endef

define Package/$(PKG_NAME)/install
	$(INSTALL_DIR) $(1)/usr/share
	$(INSTALL_DIR) $(1)/etc/init.d
	$(CP) -uvd $(PKG_BUILD_DIR)/moonraker-env $(1)/usr/share
	$(CP) -uvd $(PKG_BUILD_DIR)/$(PKG_NAME) $(1)/usr/share
	$(INSTALL_BIN) ./files/moonraker_service.init $(1)/etc/init.d/moonraker_service
	$(INSTALL_BIN) ./files/moonraker_service2.init $(1)/etc/init.d/moonraker_service.2
	$(INSTALL_BIN) ./files/moonraker_service3.init $(1)/etc/init.d/moonraker_service.3
	$(INSTALL_BIN) ./files/moonraker_service4.init $(1)/etc/init.d/moonraker_service.4
endef

$(eval $(call BuildPackage,$(PKG_NAME)))
