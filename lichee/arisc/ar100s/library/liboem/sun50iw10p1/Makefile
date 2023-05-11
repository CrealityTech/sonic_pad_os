ifneq ($(wildcard $(dir)/arisc-private/Makefile),)
m := $(shell : > .lib)
obj-y += arisc-private/

__build:$(dir)/libar100s.a

$(info use source)
else
$(info use lib)
m := $(shell echo -L$(dir) -lar100s > .lib)
endif

$(dir)/libar100s.a:$(in-target)
	$(Q)$(AR) rcs $@ $(in-target)

