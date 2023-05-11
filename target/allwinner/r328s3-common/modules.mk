#
# Copyright (C) 2015-2016 Allwinner
#
# This is free software, licensed under the GNU General Public License v2.
# See /build/LICENSE for more information.
#
define KernelPackage/sunxi-sound
  SUBMENU:=$(SOUND_MENU)
  DEPENDS:=+kmod-sound-core
  TITLE:=sun8iw18 sound support
  KCONFIG:= \
	CONFIG_SND_SUNXI_SOC=y \
	CONFIG_SND_SUNXI_SOC_DAUDIO \
	CONFIG_SND_SUNXI_SOC_RWFUNC=y \
	CONFIG_SND_SUN8IW18_CODEC \
	CONFIG_SND_SUNXI_SOC_SUN8IW18_CODEC \
	CONFIG_SND_SUNXI_SOC_SUNXI_DAUDIO \
	CONFIG_SUNXI_AUDIO_DEBUG=n

  FILES:=$(LINUX_DIR)/sound/soc/sunxi/sun8iw18-sndcodec.ko
  FILES+=$(LINUX_DIR)/sound/soc/sunxi/sun8iw18-codec.ko
  FILES+=$(LINUX_DIR)/sound/soc/sunxi/sunxi-snddaudio.ko
  FILES+=$(LINUX_DIR)/sound/soc/sunxi/snd-soc-sunxi-daudio.ko
  AUTOLOAD:=$(call AutoLoad,30,sun8iw18-sndcodec sun8iw18-codec sunxi-snddaudio snd-soc-sunxi-daudio)
endef

define KernelPackage/sunxi-sound/description
  Kernel modules for sun8iw18-sound support
endef

$(eval $(call KernelPackage,sunxi-sound))

define KernelPackage/leds-sunxi
  SUBMENU:=$(LEDS_MENU)
  TITLE:=leds-sunxi support
  FILES:=$(LINUX_DIR)/drivers/leds/leds-sunxi.ko
  KCONFIG:=CONFIG_LEDS_SUNXI
  AUTOLOAD:=$(call AutoLoad,60,leds-sunxi)
endef

define KernelPackage/leds-sunxi/description
  Kernel modules for leds sunxi support
endef

$(eval $(call KernelPackage,leds-sunxi))

define KernelPackage/ledtrig-doubleflash
  SUBMENU:=$(LEDS_MENU)
  TITLE:=LED DoubleFlash Trigger
  KCONFIG:=CONFIG_LEDS_TRIGGER_DOUBLEFLASH
  FILES:=$(LINUX_DIR)/drivers/leds/ledtrig-doubleflash.ko
  AUTOLOAD:=$(call AutoLoad,50,ledtrig-doubleflash)
endef

define KernelPackage/ledtrig-doubleflash/description
 Kernel module that allows LEDs to be controlled by a programmable doubleflash
 via sysfs
endef

$(eval $(call KernelPackage,ledtrig-doubleflash))
