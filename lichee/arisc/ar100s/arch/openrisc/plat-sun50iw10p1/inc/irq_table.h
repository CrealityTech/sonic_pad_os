#include "include.h"
#include "wakeup_source.h"

#define GIC_SRC_SPI               32

static u32 irq_table[][2] = {
	{ INTC_USB0_IRQ, GIC_USB0_EHCI_IRQ},
	{ INTC_USB0_IRQ, GIC_USB0_OHCI_IRQ},
	{ INTC_USB1_IRQ, GIC_USB1_EHCI_IRQ},
	{ INTC_USB1_IRQ, GIC_USB1_OHCI_IRQ},
	{ INTC_R_NMI_IRQ, GIC_R_EXTERNAL_NMI_IRQ },
	{ INTC_R_ALM0_IRQ, GIC_R_ALARM0_IRQ },
	{ INTC_R_PL_EINT_IRQ, GIC_R_GPIOL_IRQ },
};

static u32 grp_irq_table[][2] = {
	/* cpux irq num   group irq num */
	{ 0, 0 },
};

static inline s32 CPUS_IRQ_MAPTO_CPUX(u32 intno)
{
	u32 cpus_irq;

	for (cpus_irq = 0; cpus_irq < ARRAY_SIZE(irq_table); cpus_irq++) {
		if (irq_table[cpus_irq][0] == intno)
			return (irq_table[cpus_irq][1] - GIC_SRC_SPI);
	}

	return FAIL;
}

static inline s32 CPUX_IRQ_MAPTO_CPUS(u32 intno)
{
	u32 cpux_irq;

	for (cpux_irq = 0; cpux_irq < ARRAY_SIZE(irq_table); cpux_irq++) {
		if (irq_table[cpux_irq][1] == (intno + GIC_SRC_SPI))
			return irq_table[cpux_irq][0];
	}

	return FAIL;
}

static inline s32 CPUX_IRQ_MAPTO_CPUS_GRP(u32 intno)
{
	u32 cpux_irq;

	for (cpux_irq = 0; cpux_irq < ARRAY_SIZE(grp_irq_table);
	     cpux_irq++) {
		if (grp_irq_table[cpux_irq][0] == (intno + GIC_SRC_SPI))
			return grp_irq_table[cpux_irq][1];
	}

	return FAIL;
}
