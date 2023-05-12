include $(TOPDIR)/rules.mk

PKG_NAME:=testunit
PKG_VERSION:=1
PKG_RELEASE:=1

include $(BUILD_DIR)/package.mk

define Package/$(PKG_NAME)
  MENU:=1
  SECTION:=test
  CATEGORY:=TestUnit
  TITLE:=Tina test unit
endef

define Package/$(PKG_NAME)/config
	source "$(SOURCE)/Config.in"
endef

define Package/$(PKG_NAME)/description
  TestUnit is created to simplify building test img.
endef

$(eval $(call BuildPackage,$(PKG_NAME)))
