#
# Copyright (C) 2012 OpenWrt.org
#
# This is free software, licensed under the GNU General Public License v2.
# See /LICENSE for more information.
#

include $(TOPDIR)/rules.mk
include $(BUILD_DIR)/kernel.mk

PKG_NAME:=awrpaf
PKG_VERSION:=1
PKG_RELEASE:=1

PKG_BUILD_DIR := $(COMPILE_DIR)/$(PKG_NAME)
include $(BUILD_DIR)/package.mk

define Package/libawrpaf
  SECTION:=utils
  CATEGORY:=Libraries
  TITLE:= Allwinner RPAF library
endef

define Package/libawrpaf/description
  Remote Process Audio Framework library
  used for communication with DSP
endef

define Package/awrpaf_demo
  SECTION:=utils
  CATEGORY:=Allwinner
  TITLE:= RPAF demo
  DEPENDS:= +libawrpaf
endef

define Package/awrpaf_demo/description
  RPAF demo
endef

define Package/awrpaf_plugin
  SECTION:=utils
  CATEGORY:=Allwinner
  TITLE:= RPAF ALSA plugin
  DEPENDS:= +libawrpaf +alsa-lib
endef

define Package/awrpaf_plugin/description
  RPAF ALSA plugin
  used for config pcm stream algo
endef

define Build/Prepare
	$(CP) -r ./libawrpaf $(PKG_BUILD_DIR)/
	$(CP) -r ./demo $(PKG_BUILD_DIR)/
	$(CP) -r ./awrpaf-plugin $(PKG_BUILD_DIR)/
	$(CP) -r ./conf $(PKG_BUILD_DIR)/
endef

TARGET_CFLAGS+=-Werror

define Build/Compile/libawrpaf
	$(MAKE) -C $(PKG_BUILD_DIR)/libawrpaf \
		ARCH="$(TARGET_ARCH)" \
		AR="$(TARGET_AR)" \
		CC="$(TARGET_CC)" \
		CXX="$(TARGET_CXX)" \
		CFLAGS="$(TARGET_CFLAGS)" \
		LDFLAGS="$(TARGET_LDFLAGS)" \
		CONFIG_PREFIX="$(PKG_INSTALL_DIR)" \
		all
endef

define Build/Compile/awrpaf_demo
	$(MAKE) -C $(PKG_BUILD_DIR)/demo \
		ARCH="$(TARGET_ARCH)" \
		AR="$(TARGET_AR)" \
		CC="$(TARGET_CC)" \
		CXX="$(TARGET_CXX)" \
		CFLAGS="$(TARGET_CFLAGS)" \
		LDFLAGS="$(TARGET_LDFLAGS)" \
		CONFIG_PREFIX="$(PKG_INSTALL_DIR)" \
		all
endef

define Build/Compile/awrpaf_plugin
	$(MAKE) -C $(PKG_BUILD_DIR)/awrpaf-plugin \
		ARCH="$(TARGET_ARCH)" \
		AR="$(TARGET_AR)" \
		CC="$(TARGET_CC)" \
		CXX="$(TARGET_CXX)" \
		CFLAGS="$(TARGET_CFLAGS)" \
		LDFLAGS="$(TARGET_LDFLAGS)" \
		CONFIG_PREFIX="$(PKG_INSTALL_DIR)" \
		all
endef

define Build/Compile
	$(Build/Compile/libawrpaf)
ifeq ($(CONFIG_PACKAGE_awrpaf_demo),y)
	$(Build/Compile/awrpaf_demo)
endif
ifeq ($(CONFIG_PACKAGE_awrpaf_plugin),y)
	$(Build/Compile/awrpaf_plugin)
endif
endef

define Build/InstallDev
	$(INSTALL_DIR) $(1)/usr/lib
	$(INSTALL_BIN) $(PKG_INSTALL_DIR)/usr/lib/*.so $(1)/usr/lib/

	$(INSTALL_DIR) $(1)/usr/include
	$(INSTALL_DATA) $(PKG_INSTALL_DIR)/usr/include/*.h $(1)/usr/include
endef

define Package/libawrpaf/install
	$(INSTALL_DIR) $(1)/usr/lib
	$(INSTALL_BIN) $(PKG_INSTALL_DIR)/usr/lib/libawrpaf.so $(1)/usr/lib/
endef

define Package/awrpaf_demo/install
	$(INSTALL_DIR) $(1)/usr/bin
	$(INSTALL_BIN) $(PKG_INSTALL_DIR)/usr/bin/awrpaf_demo $(1)/usr/bin/
endef

define Package/awrpaf_plugin/install
	$(INSTALL_DIR) $(1)/usr/lib/alsa-lib/
	$(INSTALL_BIN) $(PKG_INSTALL_DIR)/usr/lib/libasound_module_pcm_awrpaf.so $(1)/usr/lib/alsa-lib/
	$(INSTALL_DIR) $(1)/usr/share/alsa/alsa.conf.d/
	$(INSTALL_DATA) $(PKG_BUILD_DIR)/conf/30-awrpaf.conf $(1)/usr/share/alsa/alsa.conf.d/
endef

$(eval $(call BuildPackage,libawrpaf))
$(eval $(call BuildPackage,awrpaf_demo))
$(eval $(call BuildPackage,awrpaf_plugin))
