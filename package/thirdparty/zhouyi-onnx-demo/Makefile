include $(TOPDIR)/rules.mk

PKG_NAME:=zhouyi-onnx-demo
PKG_VERSION:=r1p1-dev3
PKG_RELEASE:=1

include $(BUILD_DIR)/package.mk
include $(BUILD_DIR)/cmake.mk

define Package/$(PKG_NAME)
  CATEGORY:=Thirdparty
  TITLE:=Zhouyi AIPU ONNX Demo
  DEPENDS:=+zhouyi-onnx-lib +libstdcpp
endef

define Package/$(PKG_NAME)/description
  ARM CHINA Zhouyi AIPU ONNX demo
endef

define Build/Prepare
	mkdir -p $(PKG_BUILD_DIR)
	$(CP) ./files/* $(PKG_BUILD_DIR)
endef

CMAKE_SOURCE_DIR:=$(PKG_BUILD_DIR)/src

define Package/$(PKG_NAME)/config
	menu "Select demo cases"
		depends on PACKAGE_zhouyi-onnx-demo

	config ZHOUYI_ONNX_DEMO_CASE_WAVENET
		bool "WaveNet"
		default y

	config ZHOUYI_ONNX_DEMO_CASE_RNN
		bool "RNN"
		default y

	config ZHOUYI_ONNX_DEMO_CASE_DEEPVOICE
		bool "DeepVoice"
		default y

	config ZHOUYI_ONNX_DEMO_CASE_CUSTOM_OP_AIPU
		bool "Custom OP (AIPU)"
		default y

	config ZHOUYI_ONNX_DEMO_CASE_CUSTOM_OP_CPU
		bool "Custom OP (CPU)"
		default y

	endmenu
endef

PKG_CONFIG_DEPENDS := \
	CONFIG_ZHOUYI_ONNX_DEMO_CASE_WAVENET \
	CONFIG_ZHOUYI_ONNX_DEMO_CASE_RNN \
	CONFIG_ZHOUYI_ONNX_DEMO_CASE_DEEPVOICE \
	CONFIG_ZHOUYI_ONNX_DEMO_CASE_CUSTOM_OP_AIPU \
	CONFIG_ZHOUYI_ONNX_DEMO_CASE_CUSTOM_OP_CPU \

ZHOUYI_ONNX_DEMO_CASES += $(if $(filter $(CONFIG_ZHOUYI_ONNX_DEMO_CASE_WAVENET),y),WaveNet)
ZHOUYI_ONNX_DEMO_CASES += $(if $(filter $(CONFIG_ZHOUYI_ONNX_DEMO_CASE_RNN),y),RNN)
ZHOUYI_ONNX_DEMO_CASES += $(if $(filter $(CONFIG_ZHOUYI_ONNX_DEMO_CASE_DEEPVOICE),y),DeepVoice)
ZHOUYI_ONNX_DEMO_CASES += $(if $(filter $(CONFIG_ZHOUYI_ONNX_DEMO_CASE_CUSTOM_OP_AIPU),y),custom_op_aipu)
ZHOUYI_ONNX_DEMO_CASES += $(if $(filter $(CONFIG_ZHOUYI_ONNX_DEMO_CASE_CUSTOM_OP_CPU),y),custom_op_cpu)

define Package/$(PKG_NAME)/install
	$(INSTALL_DIR) $(1)/etc/zhouyi-onnx
	$(CP) $(PKG_BUILD_DIR)/zhouyi-onnx-demo-run.sh $(1)/etc/zhouyi-onnx
	$(INSTALL_DIR) $(1)/etc/zhouyi-onnx/cases
	for case in $(ZHOUYI_ONNX_DEMO_CASES); do \
		$(CP) $(PKG_BUILD_DIR)/cases/$$$$case $(1)/etc/zhouyi-onnx/cases; \
	done
	$(INSTALL_DIR) $(1)/usr/bin
	$(INSTALL_BIN) $(PKG_INSTALL_DIR)/usr/bin/* $(1)/usr/bin
endef

$(eval $(call BuildPackage,$(PKG_NAME)))
