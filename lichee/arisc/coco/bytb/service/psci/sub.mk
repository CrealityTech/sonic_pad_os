srcs-y += psci.c
srcs-y += suspend.c
srcs-y += cpu_depend.c
srcs-y += wakeup_src.c

subdirs-$(CFG_SUN8IW12P1) += $(PLATFORM)_mem
subdirs-$(CFG_SUN8IW15P1) += $(PLATFORM)_mem
subdirs-$(CFG_SUN8IW16P1) += $(PLATFORM)_mem

