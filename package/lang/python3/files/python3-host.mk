#
# Copyright (C) 2015-2016 OpenWrt.org
#
# This is free software, licensed under the GNU General Public License v2.
# See /LICENSE for more information.
#

HOST_PYTHON3_DIR:=$(OUT_DIR)/host
HOST_PYTHON3_INC_DIR:=$(HOST_PYTHON3_DIR)/include/python$(PYTHON3_VERSION)$(PYTHON3_ABIFLAGS)
HOST_PYTHON3_LIB_DIR:=$(HOST_PYTHON3_DIR)/lib/python$(PYTHON3_VERSION)

HOST_PYTHON3_PKG_DIR:=/lib/python$(PYTHON3_VERSION)/site-packages

HOST_PYTHON3_BIN:=$(HOST_PYTHON3_DIR)/bin/python$(PYTHON3_VERSION)

HOST_PYTHON3PATH:=$(HOST_PYTHON3_LIB_DIR):$(OUT_DIR)/host/$(HOST_PYTHON3_PKG_DIR)

define HostPython3
	if [ "$(strip $(3))" == "HOST" ]; then \
		export PYTHONPATH="$(HOST_PYTHON3PATH)"; \
		export PYTHONDONTWRITEBYTECODE=0; \
	else \
		export PYTHONPATH="$(PYTHON3PATH)"; \
		export PYTHONDONTWRITEBYTECODE=1; \
		export _python_sysroot="$(OUT_DIR)"; \
		export _python_prefix="/usr"; \
		export _python_exec_prefix="/usr"; \
	fi; \
	export PYTHON3OPTIMIZE=""; \
	$(1) \
	$(HOST_PYTHON3_BIN) $(2);
endef

# $(1) => build subdir
# $(2) => additional arguments to setup.py
# $(3) => additional variables
define Build/Compile/HostPy3Mod
	$(call HostPython3, \
		cd $(HOST_BUILD_DIR)/$(strip $(1)); \
		CC="$(HOSTCC)" \
		CCSHARED="$(HOSTCC) $(HOST_FPIC)" \
		CXX="$(HOSTCXX)" \
		LD="$(HOSTCC)" \
		LDSHARED="$(HOSTCC) -shared" \
		CFLAGS="$(HOST_CFLAGS)" \
		CPPFLAGS="$(HOST_CPPFLAGS) -I$(HOST_PYTHON3_INC_DIR)" \
		LDFLAGS="$(HOST_LDFLAGS) -lpython$(PYTHON3_VERSION)$(PYTHON3_ABIFLAGS) -Wl$(comma)-rpath=$(OUT_DIR)/host/lib" \
		$(3) \
		, \
		./setup.py $(2) \
		, \
		HOST \
	)
endef

# Note: I shamelessly copied this from Yousong's logic (from python-packages);
HOST_PYTHON3_PIP:=$(OUT_DIR)/host/bin/pip$(PYTHON3_VERSION)

# $(1) => packages to install
define Build/Compile/HostPy3PipInstall
	$(call locked, \
		ARCH="$(HOST_ARCH)" \
		CC="$(HOSTCC)" \
		CCSHARED="$(HOSTCC) $(HOST_FPIC)" \
		CXX="$(HOSTCXX)" \
		LD="$(HOSTCC)" \
		LDSHARED="$(HOSTCC) -shared" \
		CFLAGS="$(HOST_CFLAGS)" \
		CPPFLAGS="$(HOST_CPPFLAGS) -I$(HOST_PYTHON3_INC_DIR)" \
		LDFLAGS="$(HOST_LDFLAGS) -lpython$(PYTHON3_VERSION)$(PYTHON3_ABIFLAGS) -Wl$(comma)-rpath=$(OUT_DIR)/host/lib" \
		$(HOST_PYTHON3_PIP) \
			--disable-pip-version-check \
			--cache-dir "$(TOPDIR)/tmp/pip-cache" \
			install \
			--no-binary :all: \
			$(1), \
		pip3 \
	)
endef

