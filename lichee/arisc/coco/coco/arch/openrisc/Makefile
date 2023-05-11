obj-y += cpu/
obj-y += lib/

CFLAGS += -Icoco/arch/openrisc/lib/inc

obj-$(CFG_SUN50IW6P1) += plat-sun50iw6p1/
CFLAGS-$(CFG_SUN50IW6P1) += -Icoco/arch/openrisc/plat-sun50iw6p1/inc
LINK-$(CFG_SUN50IW6P1) := coco/arch/openrisc/plat-sun50iw6p1/kern.ld.S
LINK_RAM-$(CFG_SUN50IW6P1) := coco/arch/openrisc/plat-sun50iw6p1/ram.ld

obj-$(CFG_SUN8IW12P1) += plat-sun8iw12p1/
CFLAGS-$(CFG_SUN8IW12P1) += -Icoco/arch/openrisc/plat-sun8iw12p1/inc
CFLAGS-$(CFG_SUN8IW12P1) += -Icoco/arch/openrisc/plat-sun8iw12p1/inc/mem
CFLAGS-$(CFG_SUN8IW12P1) += -Icoco/driver/dram/dram_sun8iw12p1
LINK-$(CFG_SUN8IW12P1) := coco/arch/openrisc/plat-sun8iw12p1/kern.ld.S
LINK_RAM-$(CFG_SUN8IW12P1) := coco/arch/openrisc/plat-sun8iw12p1/ram.ld

obj-$(CFG_SUN8IW15P1) += plat-sun8iw15p1/
CFLAGS-$(CFG_SUN8IW15P1) += -Icoco/arch/openrisc/plat-sun8iw15p1/inc
CFLAGS-$(CFG_SUN8IW15P1) += -Icoco/arch/openrisc/plat-sun8iw15p1/inc/mem
CFLAGS-$(CFG_SUN8IW15P1) += -Icoco/driver/dram/dram_sun8iw15p1
LINK-$(CFG_SUN8IW15P1) := coco/arch/openrisc/plat-sun8iw15p1/kern.ld.S
LINK_RAM-$(CFG_SUN8IW15P1) := coco/arch/openrisc/plat-sun8iw15p1/ram.ld

obj-$(CFG_SUN8IW16P1) += plat-sun8iw16p1/
CFLAGS-$(CFG_SUN8IW16P1) += -Icoco/arch/openrisc/plat-sun8iw16p1/inc
CFLAGS-$(CFG_SUN8IW16P1) += -Icoco/arch/openrisc/plat-sun8iw16p1/inc/mem
CFLAGS-$(CFG_SUN8IW16P1) += -Icoco/driver/dram/dram_sun8iw16p1
LINK-$(CFG_SUN8IW16P1) := coco/arch/openrisc/plat-sun8iw16p1/kern.ld.S
LINK_RAM-$(CFG_SUN8IW16P1) := coco/arch/openrisc/plat-sun8iw16p1/ram.ld

