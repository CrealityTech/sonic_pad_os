# read config
-include .config

LICHEE_ARISC_DEFCONFIG ?= sun50iw10p1_defconfig
LICHEE_ARISC_DEFDIR ?= ar100s
MAKEFLAGS += --no-print-directory

all:
	@echo "arisc defconfig: generate ${LICHEE_ARISC_DEFDIR}/.config by ${LICHEE_ARISC_DEFCONFIG}"
	@make -C $(LICHEE_ARISC_DEFDIR) $(LICHEE_ARISC_DEFCONFIG)
	@make -C $(LICHEE_ARISC_DEFDIR)




