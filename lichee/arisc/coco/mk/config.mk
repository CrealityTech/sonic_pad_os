# Default configuration values for OP-TEE core (all platforms).
#
# Platform-specific overrides are in core/arch/arm32/plat-*/conf.mk.
# Some subsystem-specific defaults are not here but rather in */sub.mk.
#
# Configuration values may be assigned from multiple sources.
# From higher to lower priority:
#
#   1. Make arguments ('make CFG_FOO=bar...')
#   2. The file specified by $(CFG_OPTEE_CONFIG) (if defined)
#   3. The environment ('CFG_FOO=bar make...')
#   4. The platform-specific configuration file: core/arch/arm32/plat-*/conf.mk
#   5. This file
#   6. Subsystem-specific makefiles (*/sub.mk)
#
# Actual values used during the build are output to $(out-dir)/conf.mk
# (CFG_* variables only).

# Cross-compiler prefix and suffix
ifeq ($(ARCH),openrisc)
CROSS_COMPILE ?= or32-elf-
endif

ifeq ($(ARCH),d10)
CROSS_COMPILE ?= nds32le-elf-
endif
COMPILER ?= gcc


CFG_DEBUG_LOG = y
CFG_DEBUG_WRN = y
CFG_DEBUG_ERR = y
#CFG_DEBUG_INF = y
