include $(TOPDIR)/rules.mk

PKG_NAME:=nbench-byte
PKG_VERSION:=2.2.3

PKG_SOURCE:=$(PKG_NAME)-$(PKG_VERSION).tar.gz
PKG_SOURCE_SUBDIR:=$(PKG_NAME)-$(PKG_VERSION)

PKG_SOURCE_VERSION:=v$(PKG_VERSION)

include $(BUILD_DIR)/package.mk

define Package/$(PKG_NAME)
  SECTION:=utils
  CATEGORY:=Utilities
  TITLE:=nbench-byte
  URL:=http://www.math.utah.edu/~mayer/linux/bmark.html
endef

define Package/$(PKG_NAME)/description
This release is based on beta release 2 of BYTE Magazine's BYTEmark benchmark program (previously known as BYTE's Native Mode Benchmarks). This document covers the Native Mode (a.k.a. Algorithm Level) tests; benchmarks designed to expose the capabilities of a system's CPU, FPU, and memory system.
endef

TARGET_CFLAGS += -lc -lgcc_eh $(if $(findstring glibc,$(CONFIG_LIBC)),-static)

define Package/$(PKG_NAME)/install
	$(INSTALL_DIR) $(1)/usr/bin
	$(INSTALL_BIN) $(PKG_BUILD_DIR)/nbench $(1)/usr/bin

	$(INSTALL_DIR) $(1)/etc
	$(INSTALL_BIN) $(PKG_BUILD_DIR)/NNET.DAT $(1)/etc
endef

$(eval $(call BuildPackage,$(PKG_NAME)))
