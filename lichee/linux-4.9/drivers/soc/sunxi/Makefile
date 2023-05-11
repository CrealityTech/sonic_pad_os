obj-$(CONFIG_SUNXI_SRAM) +=	sunxi_sram.o
obj-y +=        sunxi-sid.o
ifeq ($(strip $(CONFIG_ARCH_SUN8IW8)     \
	      $(CONFIG_ARCH_SUN8IW19)    \
	      $(CONFIG_ARCH_SUN8IW16)    \
	      $(CONFIG_ARCH_SUN50IW10)   \
	      $(CONFIG_ARCH_SUN50IW11)),)
obj-$(CONFIG_PM) += pm.o
endif
obj-y += arisc/
obj-y += sunxi-dump.o
obj-m += vf-test.o
obj-y += vf-table.o
obj-$(CONFIG_GPU_PM_DOMAINS) += pm_domains.o
