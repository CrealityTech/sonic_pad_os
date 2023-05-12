#
# Copyright (C) 2007-2016 OpenWrt.org
#
# This is free software, licensed under the GNU General Public License v2.
# See /LICENSE for more information.
#

include $(TOPDIR)/rules.mk

PKG_NAME:=libgpu-opencl
PKG_RELEASE:=1

PKG_BUILD_DIR:=$(COMPILE_DIR)/$(PKG_NAME)

#PKG_BUILD_PARALLEL:=1
#PKG_INSTALL:=1

include $(BUILD_DIR)/package.mk

ifeq ($(TARGET_PLATFORM), $(filter $(TARGET_PLATFORM), mr813 r818))
    GPU_TYPE:=ge8300
endif

define Package/$(PKG_NAME)
  SECTION:=libs
  CATEGORY:=Libraries
  SUBMENU:=GPU Libraries
  TITLE:=GPU Library $(GPU_TYPE) opencl
ifeq ($(GPU_TYPE), $(filter $(GPU_TYPE), ge8300))
  DEPENDS:=+kmod-ge8300-km-opencl +libstdcpp +zlib
endif
endef

define Package/$(PKG_NAME)/description
endef

define Build/Prepare
endef

ifeq ($(CONFIG_WESTON_DRM),y)
    WINDOW_SYSTEM_TYPE:=wayland
else
    WINDOW_SYSTEM_TYPE:=fbdev
endif

GPU_3RDPARTY_INCLUDE_DIR:=$(GPU_TYPE)/3rdparty/include/khronos
GPU_INCLUDE_DIR:=$(GPU_TYPE)/$(WINDOW_SYSTEM_TYPE)/include
ifeq ($(CONFIG_COMPLILE_KERNEL64_USER64),y)
GPU_LIB_DIR:=$(GPU_TYPE)/$(WINDOW_SYSTEM_TYPE)/$(CONFIG_LIBC)/lib64
else ifeq ($(TARGET_ARCH),aarch64)
GPU_LIB_DIR:=$(GPU_TYPE)/$(WINDOW_SYSTEM_TYPE)/$(CONFIG_LIBC)/lib64
else ifeq ($(TARGET_ARCH),arm)
GPU_LIB_DIR:=$(GPU_TYPE)/$(WINDOW_SYSTEM_TYPE)/$(CONFIG_LIBC)/lib
else
$(error Invalid TARGET_ARCH $(TARGET_ARCH))
endif

$(warning $(GPU_INCLUDE_DIR))

# This library only supports CONFIG_NO_STRIP or CONFIG_USE_STRIP,
# but not CONFIG_USE_SSTRIP. Therefore overwrite the original strip
# settings in $(TOPDIR)/rules.mk.
ifneq ($(CONFIG_NO_STRIP),)
  RSTRIP:=:
  STRIP:=:
else
  STRIP:=$(TARGET_CROSS)strip $(call qstrip,$(CONFIG_STRIP_ARGS))
  RSTRIP= \
    export CROSS="$(TARGET_CROSS)" \
                $(if $(PKG_BUILD_ID),KEEP_BUILD_ID=1) \
                $(if $(CONFIG_KERNEL_KALLSYMS),NO_RENAME=1) \
                $(if $(CONFIG_KERNEL_PROFILING),KEEP_SYMBOLS=1); \
    NM="$(TARGET_CROSS)nm" \
    STRIP="$(STRIP)" \
    STRIP_KMOD="$(SCRIPT_DIR)/strip-kmod.sh" \
    PATCHELF="$(STAGING_DIR_HOST)/bin/patchelf" \
    $(SCRIPT_DIR)/rstrip.sh
endif


define Build/Compile

endef

define Build/InstallDev
	$(INSTALL_DIR) $(1)/usr/lib 
	$(INSTALL_DIR) $(1)/usr/include
	$(CP) -v $(GPU_3RDPARTY_INCLUDE_DIR)/* $(1)/usr/include
	$(CP) -uvd $(GPU_LIB_DIR)/* $(1)/usr/lib
	$(INSTALL_DIR) $(1)/usr/lib/pkgconfig
	$(CP) -v pkgconfig/*.pc $(1)/usr/lib/pkgconfig
ifeq ($(GPU_TYPE), $(filter $(GPU_TYPE), ge8300))
	$(INSTALL_DIR) $(1)/usr/bin
	$(CP) -uv $(GPU_LIB_DIR)/../bin/* $(1)/usr/bin
	$(INSTALL_DIR) $(1)/lib/firmware
	$(CP) $(GPU_LIB_DIR)/firmware/* $(1)/lib/firmware
endif
ifeq ($(WINDOW_SYSTEM_TYPE),wayland)
       $(CP) -v $(GPU_TYPE)/wayland/pkgconfig/*.pc $(1)/usr/lib/pkgconfig
endif
endef

define Package/$(PKG_NAME)/install
	$(INSTALL_DIR) $(1)/usr/lib
	$(CP) -uvd $(GPU_LIB_DIR)/* $(1)/usr/lib
ifeq ($(GPU_TYPE), $(filter $(GPU_TYPE), ge8300))
	$(INSTALL_DIR) $(1)/usr/bin
	$(CP) -uv $(GPU_LIB_DIR)/../bin/* $(1)/usr/bin
	$(INSTALL_DIR) $(1)/lib/firmware
	$(CP) $(GPU_LIB_DIR)/firmware/* $(1)/lib/firmware
endif
endef

$(eval $(call BuildPackage,$(PKG_NAME)))
