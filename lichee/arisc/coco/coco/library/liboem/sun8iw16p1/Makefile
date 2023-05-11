ifneq ($(wildcard $(dir)/arisc-private/Makefile),)
m := $(shell : > .lib)
obj-y += arisc-private/

__build:$(dir)/libcoco_$(CFG_DDR_TYPE).a

$(info use source)
else
$(info use lib)
m := $(shell echo -L$(dir) -lcoco_$(CFG_DDR_TYPE) > .lib)
endif

$(dir)/libcoco_$(CFG_DDR_TYPE).a:$(in-target)
	$(Q)$(AR) rcs $@ $(in-target)

