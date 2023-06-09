#
# Copyright (C) 2015-2016 OpenWrt.org
#
# This is free software, licensed under the GNU General Public License v2.
# See /LICENSE for more information.
#

HOST_PYTHON_DIR:=$(OUT_DIR)/host
HOST_PYTHON_INC_DIR:=$(HOST_PYTHON_DIR)/include/python$(PYTHON_VERSION)
HOST_PYTHON_LIB_DIR:=$(HOST_PYTHON_DIR)/lib/python$(PYTHON_VERSION)

HOST_PYTHON_PKG_DIR:=/lib/python$(PYTHON_VERSION)/site-packages

HOST_PYTHON_BIN:=$(HOST_PYTHON_DIR)/bin/python$(PYTHON_VERSION)

HOST_PYTHONPATH:=$(HOST_PYTHON_LIB_DIR):$(OUT_DIR)/host/$(HOST_PYTHON_PKG_DIR)

define HostPython
	if [ "$(strip $(3))" == "HOST" ]; then \
		export PYTHONPATH="$(HOST_PYTHONPATH)"; \
		export PYTHONDONTWRITEBYTECODE=0; \
	else \
		export PYTHONPATH="$(PYTHONPATH)"; \
		export PYTHONDONTWRITEBYTECODE=1; \
		export _python_sysroot="$(STAGING_DIR)"; \
		export _python_prefix="/usr"; \
		export _python_exec_prefix="/usr"; \
	fi; \
	export PYTHONOPTIMIZE=""; \
	$(1) \
	$(HOST_PYTHON_BIN) $(2);
endef

# $(1) => build subdir
# $(2) => additional arguments to setup.py
# $(3) => additional variables
define Build/Compile/HostPyMod
	$(call HostPython, \
		cd $(HOST_BUILD_DIR)/$(strip $(1)); \
		CC="$(HOSTCC)" \
		CCSHARED="$(HOSTCC) $(HOST_FPIC)" \
		CXX="$(HOSTCXX)" \
		LD="$(HOSTCC)" \
		LDSHARED="$(HOSTCC) -shared" \
		CFLAGS="$(HOST_CFLAGS)" \
		CPPFLAGS="$(HOST_CPPFLAGS) -I$(HOST_PYTHON_INC_DIR)" \
		LDFLAGS="$(HOST_LDFLAGS) -lpython$(PYTHON_VERSION) -Wl$(comma)-rpath=$(OUT_DIR)/host/lib" \
		_PYTHON_HOST_PLATFORM=linux2 \
		$(3) \
		, \
		./setup.py $(2) \
		, \
		HOST \
	)
endef

# Note: I shamelessly copied this from Yousong's logic (from python-packages);
HOST_PYTHON_PIP:=$(OUT_DIR)/host/bin/pip$(PYTHON_VERSION)

# $(1) => packages to install
define Build/Compile/HostPyPipInstall
    $(call locked, \
        ARCH="$(HOST_ARCH)" \
        CC="$(HOSTCC)" \
        CCSHARED="$(HOSTCC) $(HOST_FPIC)" \
        CXX="$(HOSTCXX)" \
        LD="$(HOSTCC)" \
        LDSHARED="$(HOSTCC) -shared" \
        CFLAGS="$(HOST_CFLAGS)" \
        CPPFLAGS="$(HOST_CPPFLAGS) -I$(HOST_PYTHON_INC_DIR)" \
        LDFLAGS="$(HOST_LDFLAGS) -lpython$(PYTHON_VERSION) -Wl$(comma)-rpath=$(OUT_DIR)/host/lib" \
        $(HOST_PYTHON_PIP) \
            --disable-pip-version-check \
            --cache-dir "$(TOPDIR)/tmp/pip2.7-cache" \
            install \
            --no-binary :all: \
            $(1), \
        pip \
    )
endef
