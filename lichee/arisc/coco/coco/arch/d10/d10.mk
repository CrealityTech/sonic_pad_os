CFG_KERN_LINKER_ARCH ?= d10


core-platform-subdirs += \
	$(addprefix $(arch-dir)/, cpu) $(platform-dir)

core-platform-cppflags>-+= -I$(arch-dir)/include

INCLUDE = -I$(arch-dir)/cpu
platform-cflags-generic ?= -Os -nostartfiles -static -mcmodel=medium -nostdlib  -Wall -Werror  $(INCLUDES) -Wl,--gc-sections -mvh
platform-aflags-generic ?= -g  --stats

platform-cflags-optimization ?=  -Os

platform-cflags-debug-info ?= -g3
platform-aflags-debug-info ?=

core-platform-cflags += $(platform-cflags-optimization)
core-platform-cflags += $(platform-cflags-generic)
core-platform-cflags += $(platform-cflags-debug-info)

core-platform-aflags += $(platform-aflags-generic)
core-platform-aflags += $(platform-aflags-debug-info)


