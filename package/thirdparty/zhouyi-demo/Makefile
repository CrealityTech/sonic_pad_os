include $(TOPDIR)/rules.mk

PKG_NAME:=zhouyi-demo
PKG_VERSION:=r1p0-eac0
PKG_RELEASE:=1

include $(BUILD_DIR)/package.mk

define Package/$(PKG_NAME)
  CATEGORY:=Thirdparty
  TITLE:=Zhouyi AIPU Demo
  DEPENDS:=+zhouyi-lib +libstdcpp
endef

define Package/$(PKG_NAME)/description
  ARM CHINA Zhouyi AIPU demo
endef

define Build/Prepare
	mkdir -p $(PKG_BUILD_DIR)
	$(CP) ./files/* $(PKG_BUILD_DIR)
endef

define Package/$(PKG_NAME)/config
	menu "Select demo cases"
		depends on PACKAGE_zhouyi-demo

	config ZHOUYI_DEMO_CASE_RESNET_50
		bool "ResNet50"
		default y

	config ZHOUYI_DEMO_CASE_TFLITE_INCEPTION_V3
		bool "TFLite Inception V3"
		default y

	endmenu
endef

PKG_CONFIG_DEPENDS := \
	CONFIG_ZHOUYI_DEMO_CASE_RESNET_50 \
	CONFIG_ZHOUYI_DEMO_CASE_TFLITE_INCEPTION_V3 \

ZHOUYI_DEMO_CASES += $(if $(filter $(CONFIG_ZHOUYI_DEMO_CASE_RESNET_50),y),resnet_50)
ZHOUYI_DEMO_CASES += $(if $(filter $(CONFIG_ZHOUYI_DEMO_CASE_TFLITE_INCEPTION_V3),y),tflite_inception_v3)

define Build/Compile
	$(MAKE) -C $(PKG_BUILD_DIR)/src \
		ARCH="$(TARGET_ARCH)" \
		AR="$(TARGET_AR)" \
		CXX="$(TARGET_CXX)" \
		CXXFLAGS="$(TARGET_CXXFLAGS)" \
		LDFLAGS="$(TARGET_LDFLAGS)" \
		INSTALL_PREFIX="$(PKG_INSTALL_DIR)" \
		all
endef

define Package/$(PKG_NAME)/install
	$(INSTALL_DIR) $(1)/etc/zhouyi
	$(CP) $(PKG_BUILD_DIR)/zhouyi-demo-run.sh $(1)/etc/zhouyi
	$(INSTALL_DIR) $(1)/etc/zhouyi/cases
	for case in $(ZHOUYI_DEMO_CASES); do \
		$(CP) $(PKG_BUILD_DIR)/cases/$$$$case $(1)/etc/zhouyi/cases; \
	done
	$(INSTALL_DIR) $(1)/usr/bin
	$(INSTALL_BIN) $(PKG_INSTALL_DIR)/usr/bin/* $(1)/usr/bin
endef

$(eval $(call BuildPackage,$(PKG_NAME)))
