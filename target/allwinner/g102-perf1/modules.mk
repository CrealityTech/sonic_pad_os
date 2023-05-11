#
# Copyright (C) 2015-2016 Allwinner
#
# This is free software, licensed under the GNU General Public License v2.
# See /build/LICENSE for more information.


define KernelPackage/xradio-xr819
  SUBMENU:=$(WIRELESS_MENU)
  TITLE:=xr819 support
  DEPENDS:=@TARGET_g102_perf1
  FILES:=$(LINUX_DIR)/drivers/net/wireless/xradio/umac/xradio_mac.ko
  FILES:=$(LINUX_DIR)/drivers/net/wireless/xradio/wlan/xradio_core.ko
  FILES+=$(LINUX_DIR)/drivers/net/wireless/xradio/wlan/xradio_wlan.ko
#  AUTOLOAD:=$(call AutoProbe, xradio_mac xradio_core xradio_wlan)
endef

define KernelPackage/xradio-xr819/description
 Kernel modules for Allwinnertech XR819  support
endef

$(eval $(call KernelPackage,xradio-xr819))

define KernelPackage/sunxi-timer-test
  SUBMENU:=$(OTHER_MENU)
  TITLE:=sunxi timer test driver
  DEPENDS:=@TARGET_g102_perf1
  FILES:=$(LINUX_DIR)/drivers/char/timer_test/sunxi_timer_test.ko
#  AUTOLOAD:=$(call AutoProbe, sunxi_timer_test)
endef

define KernelPackage/sunxi-timer-test/description
  Kernel modules for sunxi timer test driver
endef

$(eval $(call KernelPackage,sunxi-timer-test))

define KernelPackage/sunxi-vfe
  SUBMENU:=$(VIDEO_MENU)
  TITLE:=sunxi-vfe support
  DEPENDS:=@TARGET_g102_perf1
  FILES:=$(LINUX_DIR)/drivers/media/platform/sunxi-vfe/actuator/actuator.ko
  FILES+=$(LINUX_DIR)/drivers/media/platform/sunxi-vfe/actuator/ad5820_act.ko
  FILES+=$(LINUX_DIR)/drivers/media/platform/sunxi-vfe/actuator/dw9714_act.ko
  FILES+=$(LINUX_DIR)/drivers/media/platform/sunxi-vfe/actuator/ov8825_act.ko
  FILES+=$(LINUX_DIR)/drivers/media/platform/sunxi-vfe/device/ar0330.ko
  FILES+=$(LINUX_DIR)/drivers/media/platform/sunxi-vfe/device/gc0307.ko
  FILES+=$(LINUX_DIR)/drivers/media/platform/sunxi-vfe/device/gc0308.ko
  FILES+=$(LINUX_DIR)/drivers/media/platform/sunxi-vfe/device/gc0311.ko
  FILES+=$(LINUX_DIR)/drivers/media/platform/sunxi-vfe/device/gc0328c.ko
  FILES+=$(LINUX_DIR)/drivers/media/platform/sunxi-vfe/device/gc0328.ko
  FILES+=$(LINUX_DIR)/drivers/media/platform/sunxi-vfe/device/gc0329.ko
  FILES+=$(LINUX_DIR)/drivers/media/platform/sunxi-vfe/device/gc2035.ko
  FILES+=$(LINUX_DIR)/drivers/media/platform/sunxi-vfe/device/gc2155.ko
  FILES+=$(LINUX_DIR)/drivers/media/platform/sunxi-vfe/device/gc5004.ko
  FILES+=$(LINUX_DIR)/drivers/media/platform/sunxi-vfe/device/gc5004_mipi.ko
  FILES+=$(LINUX_DIR)/drivers/media/platform/sunxi-vfe/device/gt2005.ko
  FILES+=$(LINUX_DIR)/drivers/media/platform/sunxi-vfe/device/hi253.ko
  FILES+=$(LINUX_DIR)/drivers/media/platform/sunxi-vfe/device/imx214.ko
  FILES+=$(LINUX_DIR)/drivers/media/platform/sunxi-vfe/device/ov12830.ko
  FILES+=$(LINUX_DIR)/drivers/media/platform/sunxi-vfe/device/ov13850.ko
  FILES+=$(LINUX_DIR)/drivers/media/platform/sunxi-vfe/device/ov16825.ko
  FILES+=$(LINUX_DIR)/drivers/media/platform/sunxi-vfe/device/ov2640.ko
  FILES+=$(LINUX_DIR)/drivers/media/platform/sunxi-vfe/device/ov2686.ko
  FILES+=$(LINUX_DIR)/drivers/media/platform/sunxi-vfe/device/ov2710_mipi.ko
  FILES+=$(LINUX_DIR)/drivers/media/platform/sunxi-vfe/device/ov5640.ko
  FILES+=$(LINUX_DIR)/drivers/media/platform/sunxi-vfe/device/ov5647.ko
  FILES+=$(LINUX_DIR)/drivers/media/platform/sunxi-vfe/device/ov5647_mipi.ko
  FILES+=$(LINUX_DIR)/drivers/media/platform/sunxi-vfe/device/ov5648.ko
  FILES+=$(LINUX_DIR)/drivers/media/platform/sunxi-vfe/device/ov5650.ko
  FILES+=$(LINUX_DIR)/drivers/media/platform/sunxi-vfe/device/ov7736.ko
  FILES+=$(LINUX_DIR)/drivers/media/platform/sunxi-vfe/device/ov8825.ko
  FILES+=$(LINUX_DIR)/drivers/media/platform/sunxi-vfe/device/ov8850.ko
  FILES+=$(LINUX_DIR)/drivers/media/platform/sunxi-vfe/device/ov8858_4lane.ko
  FILES+=$(LINUX_DIR)/drivers/media/platform/sunxi-vfe/device/ov8858.ko
  FILES+=$(LINUX_DIR)/drivers/media/platform/sunxi-vfe/device/s5k4e1.ko
  FILES+=$(LINUX_DIR)/drivers/media/platform/sunxi-vfe/device/s5k4e1_mipi.ko
  FILES+=$(LINUX_DIR)/drivers/media/platform/sunxi-vfe/device/s5k4ec.ko
  FILES+=$(LINUX_DIR)/drivers/media/platform/sunxi-vfe/device/s5k4ec_mipi.ko
  FILES+=$(LINUX_DIR)/drivers/media/platform/sunxi-vfe/device/s5k5e2yx.ko
  FILES+=$(LINUX_DIR)/drivers/media/platform/sunxi-vfe/device/sp0718.ko
  FILES+=$(LINUX_DIR)/drivers/media/platform/sunxi-vfe/device/sp0838.ko
  FILES+=$(LINUX_DIR)/drivers/media/platform/sunxi-vfe/device/sp2518.ko
  FILES+=$(LINUX_DIR)/drivers/media/platform/sunxi-vfe/device/sp2519.ko
  FILES+=$(LINUX_DIR)/drivers/media/platform/sunxi-vfe/device/sp5408.ko
  FILES+=$(LINUX_DIR)/drivers/media/platform/sunxi-vfe/device/sp5409.ko
  FILES+=$(LINUX_DIR)/drivers/media/platform/sunxi-vfe/device/t8et5.ko
  FILES+=$(LINUX_DIR)/drivers/media/platform/sunxi-vfe/vfe_io.ko
  FILES+=$(LINUX_DIR)/drivers/media/platform/sunxi-vfe/vfe_sample.ko
  FILES+=$(LINUX_DIR)/drivers/media/platform/sunxi-vfe/vfe_v4l2.ko
  FILES+=$(LINUX_DIR)/drivers/media/v4l2-core/videobuf2-core.ko
  FILES+=$(LINUX_DIR)/drivers/media/v4l2-core/videobuf2-dma-contig.ko
  FILES+=$(LINUX_DIR)/drivers/media/v4l2-core/videobuf2-memops.ko
#  AUTOLOAD:=$(call AutoProbe, actuator ad5820_act ar0330 dw9714_act gc0307 gc0308 gc0311 gc0328c gc0328 gc0329 gc2035 gc2155 gc5004 gc5004_mipi gt2005 hi253 imx214 ov12830 ov13850 ov16825 ov2640 ov2686 ov2710_mipi ov5640 ov5647 ov5647_mipi ov5648 ov5650 ov7736 ov8825_act ov8825 ov8850 ov8858_4lane ov8858 s5k4e1 s5k4e1_mipi s5k4ec s5k4ec_mipi s5k5e2yx sp0718 sp0838 sp2518 sp2519 sp5408 sp5409 t8et5 vfe_io vfe_sample vfe_v4l2 videobuf2-core videobuf2-dma-contig videobuf2-memops)
endef

define KernelPackage/sunxi-vfe/description
  Kernel modules for sunxi-vfe support
endef

$(eval $(call KernelPackage,sunxi-vfe))

define KernelPackage/input-sw-device
  SUBMENU:=$(INPUT_MODULES_MENU)
  TITLE:=input sw device driver
  DEPENDS:=@TARGET_g102_perf1
  FILES:=$(LINUX_DIR)/drivers/input/sw-device.ko
#  AUTOLOAD:=$(call AutoProbe, sw-device)
endef

define KernelPackage/input-sw-device/description
  Kernel modules for input sw device driver
endef

$(eval $(call KernelPackage,input-sw-device))

define KernelPackage/ansi-cprng
  SUBMENU:=$(CRYPTO_MENU)
  TITLE:=Pseudo Random Number Generation
  DEPENDS:=@TARGET_g102_perf1
  FILES:=$(LINUX_DIR)/crypto/ansi_cprng.ko
#  AUTOLOAD:=$(call AutoProbe, ansi_cprng)
endef

define KernelPackage/ansi-cprng/description
  Kernel modules for Pseudo Random Number Generation for Cryptographic modules support
endef

$(eval $(call KernelPackage,ansi-cprng))
