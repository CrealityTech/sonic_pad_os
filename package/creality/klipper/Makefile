#
# Copyright (C) 2011 OpenWrt.org
#
# This is free software, licensed under the GNU General Public License v2.
# See /LICENSE for more information.
#

include $(TOPDIR)/rules.mk
include ../version.mk

PKG_NAME:=klipper
PKG_VERSION:=0.1.98
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
  TITLE:=klipper project
  SECTION:=base
  CATEGORY:=Creality
  DEPENDS:=+python +git +python-cffi +python-greenlet +python-numpy +python-Jinja2 +python-can
endef

define Package/$(PKG_NAME)/description
  Klipper is a 3d-Printer firmware. It combines the power of a general purpose computer with one or more micro-controllers. 
endef

define Build/Prepare
	rm -rf $(PKG_BUILD_DIR)/
	mkdir -p $(PKG_BUILD_DIR)/$(PKG_NAME)
	$(TAR) -xzf $(DL_DIR)/$(PKG_SOURCE) -C $(PKG_BUILD_DIR)  --strip-components 1
	$(TAR) -xzf ./files/klippy-env.tar.gz -C $(PKG_BUILD_DIR)
	$(Build/Patch)
	# for chelper compile
	$(CP) ./files/Makefile $(PKG_BUILD_DIR)/$(PKG_NAME)/klippy/chelper/
endef

define Build/Compile
	$(MAKE) -C $(PKG_BUILD_DIR)/$(PKG_NAME)/klippy/chelper $(MAKE_FLAGS)
endef

define Package/$(PKG_NAME)/install
	$(INSTALL_DIR) $(1)/usr/bin
	$(INSTALL_DIR) $(1)/usr/share
	$(INSTALL_DIR) $(1)/etc/init.d
	$(CP) -uvd $(PKG_BUILD_DIR)/klippy-env $(1)/usr/share
	$(CP) -uvd $(PKG_BUILD_DIR)/$(PKG_NAME) $(1)/usr/share
	find $(1)/usr/share/klipper/klippy/chelper/ -iname "*.o" | xargs rm
	$(INSTALL_BIN) ./files/klipper_service.init $(1)/etc/init.d/klipper_service
	$(INSTALL_BIN) ./files/klipper_service2.init $(1)/etc/init.d/klipper_service.2
	$(INSTALL_BIN) ./files/klipper_service3.init $(1)/etc/init.d/klipper_service.3
	$(INSTALL_BIN) ./files/klipper_service4.init $(1)/etc/init.d/klipper_service.4
	$(INSTALL_BIN) ./files/klipper_service_test.init $(1)/usr/bin/klipper_service_test.init
	$(INSTALL_BIN) ./files/klipper.elf $(1)/usr/bin/klipper_mcu
	$(INSTALL_BIN) ./files/klipper_mcu.init $(1)/etc/init.d/klipper_mcu
endef

$(eval $(call BuildPackage,$(PKG_NAME)))
