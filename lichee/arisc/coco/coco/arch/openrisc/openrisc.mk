CFG_KERN_LINKER_ARCH ?= openrisc


core-platform-subdirs += \
	$(addprefix $(arch-dir)/,lib cpu) $(platform-dir)

core-platform-cppflags>-+= -I$(arch-dir)/inc

platform-cflags-generic ?= -g -nostdlib  -Wall
platform-aflags-generic ?= -g  --stats

platform-cflags-optimization ?=  -Os

platform-cflags-debug-info ?= -g3
platform-aflags-debug-info ?=

core-platform-cflags += $(platform-cflags-optimization)
core-platform-cflags += $(platform-cflags-generic)
core-platform-cflags += $(platform-cflags-debug-info)

core-platform-aflags += $(platform-aflags-generic)
core-platform-aflags += $(platform-aflags-debug-info)

arch-bits-core := 32
core-platform-cflags += $(arm32-platform-cflags)
core-platform-cflags += $(arm32-platform-cflags-no-hard-float)

core-platform-cflags += $(arm32-platform-cflags-generic)
core-platform-aflags += $(core_arm32-platform-aflags)
core-platform-aflags += $(arm32-platform-aflags)

