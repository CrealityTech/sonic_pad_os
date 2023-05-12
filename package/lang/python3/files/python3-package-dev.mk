#
# Copyright (C) 2006-2016 OpenWrt.org
#
# This is free software, licensed under the GNU General Public License v2.
# See /LICENSE for more information.
#

define Package/python3-dev
$(call Package/python3/Default)
  TITLE:=Python $(PYTHON3_VERSION) development files
  DEPENDS:=+python3
endef

define Py3Package/python3-dev/install
	$(INSTALL_DIR) $(1)/usr/bin
	$(CP) $(PKG_INSTALL_DIR)/usr/bin/python$(PYTHON3_VERSION)$(PYTHON3_ABIFLAGS)-config $(1)/usr/bin
	$(LN) python$(PYTHON3_VERSION)$(PYTHON3_ABIFLAGS)-config $(1)/usr/bin/python3-config
	$(LN) python$(PYTHON_VERSION)/config-$(PYTHON_VERSION)$(PYTHON3_ABIFLAGS)/libpython$(PYTHON3_VERSION)$(PYTHON3_ABIFLAGS).a $(1)/usr/lib/libpython$(PYTHON3_VERSION)$(PYTHON3_ABIFLAGS).a
endef

$(eval $(call Py3BasePackage,python3-dev, \
    /usr/lib/python$(PYTHON_VERSION)/config-$(PYTHON_VERSION)$(PYTHON3_ABIFLAGS) \
    /usr/include/python$(PYTHON_VERSION)$(PYTHON3_ABIFLAGS) \
    /usr/lib/pkgconfig \
))
