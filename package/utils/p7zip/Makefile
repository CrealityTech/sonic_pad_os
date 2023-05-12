include $(TOPDIR)/rules.mk

PKG_NAME:=p7zip
PKG_VERSION:=16.02
PKG_RELEASE:=1

PKG_SOURCE:=$(PKG_NAME)_$(PKG_VERSION)_src_all.tar.bz2
PKG_SOURCE_URL:=https://nchc.dl.sourceforge.net/project/p7zip/p7zip/$(PKG_VERSION)/
PKG_MD5SUM:=a0128d661cfe7cc8c121e73519c54fbf

PKG_BUILD_DIR := $(COMPILE_DIR)/$(PKG_NAME)_$(PKG_VERSION)
include $(BUILD_DIR)/package.mk

define Package/p7zip
  SECTION:=utils
  CATEGORY:=Utilities
  TITLE:=p7zip archiver
  URL:=http://http://www.7-zip.org
  DEPENDS:=+libstdcpp
endef

MAKE_FLAGS += 7z

define Package/p7zip/install
	$(INSTALL_DIR) $(1)/usr/sbin/
	$(CP) -r $(PKG_BUILD_DIR)/bin/* $(1)/usr/sbin/
endef

$(eval $(call BuildPackage,p7zip))
