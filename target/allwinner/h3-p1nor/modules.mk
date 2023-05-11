#
# Copyright (C) 2015-2016 Allwinner
#
# This is free software, licensed under the GNU General Public License v2.
# See /build/LICENSE for more information.

#TEST_MENU:=Test module Support
#
#define KernelPackage/sunxi-timer
#  SUBMENU:=$(TEST_MENU)
#  TITLE:=sunxi timer test support
#  DEPENDS:=@TARGET_violin_F1C200s
#KCONFIG:=CONFIG_SUNXI_TIMER_TEST
#  FILES:=$(LINUX_DIR)/drivers/char/timer_test/sunxi_timer_test.ko
#  AUTOLOAD:=$(call AutoProbe, sunxi_timer_test)
#endef
#
#define KernelPackage/sunxi-timer/description
# Kernel modules for sunxi timer test support
#endef
#
#$(eval $(call KernelPackage,sunxi-timer))

define KernelPackage/xradio-xr819
  SUBMENU:=$(WIRELESS_MENU)
  TITLE:=xr819 support
  DEPENDS:=+kmod-cfg80211
  FILES+=$(LINUX_DIR)/drivers/net/wireless/xradio/xradio_wlan.ko
endef

define KernelPackage/xradio-xr819/description
 Kernel modules for Allwinnertech XR819  support
endef

$(eval $(call KernelPackage,xradio-xr819))

define KernelPackage/xradio-xr829
  SUBMENU:=$(WIRELESS_MENU)
  TITLE:=xr829 support
  DEPENDS:=+kmod-cfg80211
  FILES+=$(LINUX_DIR)/drivers/net/wireless/xr829/wlan/xradio_core.ko
  FILES+=$(LINUX_DIR)/drivers/net/wireless/xr829/wlan/xradio_wlan.ko
  FILES+=$(LINUX_DIR)/drivers/net/wireless/xr829/umac/xradio_mac.ko
endef

define KernelPackage/xradio-xr829/description
 Kernel modules for Allwinnertech XR829  support
endef

$(eval $(call KernelPackage,xradio-xr829))

#define KernelPackage/touchscreen-icn85xx
#  SUBMENU:=$(INPUT_MODULES_MENU)
#  TITLE:= ICN85XX support
#  DEPENDS:= +kmod-input-core
#  KCONFIG:= \
#	CONFIG_INPUT_TOUCHSCREEN \
#	CONFIG_INPUT_TOUCHSCREEN_ICN85XX_TS
#  FILES:=$(LINUX_DIR)/drivers/input/touchscreen/icn85xx/icn85xx_ts.ko
#  AUTOLOAD:=$(call AutoProbe,icn85xx_ts.ko)
#endef
#
#define KernelPackage/touchscreen-icn85xx/description
# Enable support for ICN85XX touchscreen port.
#endef
#
#$(eval $(call KernelPackage,touchscreen-icn85xx))
