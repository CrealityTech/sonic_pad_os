#
# Copyright (C) 2015-2016 Allwinner
#
# This is free software, licensed under the GNU General Public License v2.
# See /build/LICENSE for more information.

define KernelPackage/xenomai
  SUBMENU:=$(XENOMAI_MENU)
  TITLE:=xenomai support
  DEPENDS:=@PACKAGE_xenomai
  FILES:=$(LINUX_DIR)/drivers/xenomai/testing/xeno_klat.ko
  FILES+=$(LINUX_DIR)/drivers/xenomai/testing/xeno_rtdmtest.ko
  AUTOLOAD:=$(call AutoProbe, xeno_klat xeno_rtdmtest)
endef

define KernelPackage/xenomai/description
  Kernel modules for xenomai support
endef

$(eval $(call KernelPackage,xenomai))

define KernelPackage/touchscreen-atmel-mxt
  SUBMENU:=$(INPUT_MODULES_MENU)
  TITLE:=Atmel MXT  support
  DEPENDS:= +kmod-input-core
  KCONFIG:= \
	CONFIG_INPUT_TOUCHSCREEN \
	CONFIG_INPUT_TOUCHSCREEN_ATMEL_MXT
  FILES:=$(LINUX_DIR)/drivers/input/touchscreen/atmel_mxt_ts.ko
  AUTOLOAD:=$(call AutoProbe,atmel_mxt_ts.ko)
endef

define KernelPackage/touchscreen-atmel-mxt/description
 Enable support for Atmel MXT touchscreen port.
endef

$(eval $(call KernelPackage,touchscreen-atmel-mxt))

define KernelPackage/xradio-xr819
  SUBMENU:=$(WIRELESS_MENU)
  TITLE:=xr819 support
  DEPENDS:=+kmod-cfg80211
  FILES:=$(LINUX_DIR)/drivers/net/wireless/xradio/wlan/xradio_core.ko
  FILES+=$(LINUX_DIR)/drivers/net/wireless/xradio/wlan/xradio_wlan.ko
  FILES+=$(LINUX_DIR)/drivers/net/wireless/xradio/umac/xradio_mac.ko
  AUTOLOAD:=$(call AutoProbe, xradio_mac xradio_core xradio_wlan)
endef

define KernelPackage/xradio-xr819/description
 Kernel modules for Allwinnertech XR819  support
endef

$(eval $(call KernelPackage,xradio-xr819))

define KernelPackage/pwm_leds
  SUBMENU:=$(LEDS_MENU)
  TITLE:=Kernel modules-sc3866r for PWM-LEDS
  FILES:=$(LINUX_DIR)/drivers/misc/leds/pwm_leds.ko
  KCONFIG:=CONFIG_MISC_PWM_LEDS
  AUTOLOAD:=$(call AutoLoad,50,pwm_leds.ko)
endef
$(eval $(call KernelPackage,pwm_leds))

define KernelPackage/sunxi-keyboard
  SUBMENU:=$(INPUT_MODULES_MENU)
  TITLE:=Kernel modules-sc3866r for ADC0-KEYS
  FILES:=$(LINUX_DIR)/drivers/input/keyboard/sunxi-keyboard.ko
  KCONFIG:= \
  	CONFIG_KEYBOARD_SUNXI=m
  AUTOLOAD:=$(call AutoLoad,48,sunxi-keyboard.ko)
endef
$(eval $(call KernelPackage,sunxi-keyboard))

define KernelPackage/net-broadcom
  SUBMENU:=$(WIRELESS_MENU)
  TITLE:=broadcom wifi driver support
  DEPENDS:=
  FILES:=$(LINUX_DIR)/drivers/net/wireless/bcmdhd/bcmdhd.ko
  AUTOLOAD:=$(call AutoProbe,bcmdhd)
endef

define KernelPackage/net-broadcom/description
 Kernel modules for Broadcom AP6212/AP6212A/AP6255/AP6335/AP6356S  support
endef
$(eval $(call KernelPackage,net-broadcom))

define KernelPackage/net-rtl8723ds
  SUBMENU:=$(WIRELESS_MENU)
  TITLE:=RTL8723DS support (staging)
  DEPENDS:=@USB_SUPPORT +@DRIVER_WEXT_SUPPORT +r8723ds-firmware
#  KCONFIG:=\
#	CONFIG_STAGING=y \
#	CONFIG_R8723DS \
#	CONFIG_23DS_AP_MODE=y \
#	CONFIG_23DS_P2P=n
  FILES:=$(LINUX_DIR)/drivers/net/wireless/rtl8723ds/8723ds.ko
  AUTOLOAD:=$(call AutoProbe,8723ds)
endef

define KernelPackage/net-rtl8723ds/description
 Kernel modules for RealTek RTL8723DS support
endef

$(eval $(call KernelPackage,net-rtl8723ds))
