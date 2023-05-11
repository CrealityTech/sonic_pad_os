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
