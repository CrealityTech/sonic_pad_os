include mk/cleanvars.mk

# Set current submodule (used for module specific flags compile result etc)
sm := core
sm-$(sm) := y

arch-dir	:= coco/arch/$(ARCH)
platform-dir	:= $(arch-dir)/plat-$(PLATFORM)
include mk/checkconf.mk
include $(platform-dir)/conf.mk
include $(platform-dir)/bconf.mk
include mk/config.mk
include coco/arch/$(ARCH)/$(ARCH).mk

PLATFORM_$(PLATFORM) := y
#PLATFORM_FLAVOR_$(PLATFORM_FLAVOR) := y

$(call cfg-depends-all,CFG_PAGED_USER_TA,CFG_WITH_PAGER CFG_WITH_USER_TA)

# Setup compiler for this sub module
COMPILER_$(sm)		?= $(COMPILER)
include mk/$(COMPILER_$(sm)).mk


cppflags$(sm) = -D__KERNEL__

cppflags$(sm) += -Ibytb/include
cppflags$(sm) += -Ibytb/include/service
cppflags$(sm) += -Icoco/include
cppflags$(sm) += -Icoco/include/system
cppflags$(sm) += -Icoco/include/driver
cppflags$(sm) += -Icoco/include/service
cppflags$(sm) += -Icoco/arch/openrisc/cpu
cppflags$(sm) += -Icoco/arch/openrisc/lib/inc
cppflags$(sm) += -I$(out-dir)/include/generated
cppflags$(sm) += -include $(conf-file)
cppflags$(sm) += $(core-platform-cppflags)
cflags$(sm)   += $(core-platform-cflags)

conf-file := $(out-dir)/include/generated/conf.h
conf-mk-file := $(out-dir)/conf.mk
#conf-mk-file := $(out-dir)/bconf.mk
$(conf-file): $(conf-mk-file)

cleanfiles += $(conf-file)
cleanfiles += $(conf-mk-file)

$(conf-file): FORCE
	$(call check-conf-h)

$(conf-mk-file):  FORCE
	$(call check-conf-mk)

#
# Do main source
#

subdirs = $(core-platform-subdirs) coco bytb

$(info $(core-platform-subdirs))
#core-platform-subdirs = $(core-platform-subdirs)/dram

include mk/subdir.mk

include mk/compile.mk

include $(platform-dir)/link.mk
