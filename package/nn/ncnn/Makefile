include $(TOPDIR)/rules.mk

PKG_NAME:=ncnn
PKG_VERSION:=20200413
PKG_RELEASE:=1

PKG_SOURCE:=$(PKG_NAME)-$(PKG_VERSION).tar.gz
PKG_SOURCE_URL:=https://github.com/Tencent/ncnn/archive/v$(PKG_VERSION)
PKG_HASH:=1ed523bda96cfdf5b5619c6480987b0811e3c4ef62b5d3fe2e8bdca9b8b2fc90

PKG_LICENSE:=BSD 3 Clause
PKG_LICENSE_FILES:=LICENSE.txt

PKG_BUILD_PARALLEL:=1
PKG_JOBS:=-j16

include $(BUILD_DIR)/package.mk
include $(BUILD_DIR)/cmake.mk

define Package/$(PKG_NAME)/Default
  CATEGORY:=Neural Network
  SUBMENU:=ncnn
endef

define Package/$(PKG_NAME)/description/Default
  ncnn is a high-performance neural network inference computing framework
  optimized for mobile platforms.
endef

define Package/libncnn
  $(call Package/$(PKG_NAME)/Default)
  TITLE:=Tencent ncnn library
endef

Package/libncnn/description = $(Package/$(PKG_NAME)/description/Default)

define Package/ncnn-benchmark
  $(call Package/$(PKG_NAME)/Default)
  TITLE:=Tencent ncnn benchmark
  DEPENDS:=+libncnn +libstdcpp
endef

Package/ncnn-benchmark/description = $(Package/$(PKG_NAME)/description/Default)

define Package/libncnn/config
	if PACKAGE_libncnn

	config NCNN_VULKAN
		bool "Vulkan support"
		default n

	endif
endef

PKG_CONFIG_DEPENDS := \
	CONFIG_NCNN_VULKAN \

ifeq ($(CONFIG_NCNN_VULKAN),y)
	CMAKE_OPTIONS += -DNCNN_VULKAN=ON
endif

define Build/InstallDev
	$(INSTALL_DIR) $(1)/usr/include
	$(CP) $(PKG_INSTALL_DIR)/usr/include/* $(1)/usr/include/
	$(INSTALL_DIR) $(1)/usr/lib
	$(CP) $(PKG_INSTALL_DIR)/usr/lib/* $(1)/usr/lib/
endef

define Package/ncnn-benchmark/install
	$(INSTALL_DIR) $(1)/etc/ncnn/benchmark
	$(INSTALL_BIN) $(PKG_BUILD_DIR)/benchmark/benchncnn $(1)/etc/ncnn/benchmark
	$(CP) $(PKG_BUILD_DIR)/benchmark/*.param $(1)/etc/ncnn/benchmark
endef

$(eval $(call BuildPackage,libncnn))
$(eval $(call BuildPackage,ncnn-benchmark))
