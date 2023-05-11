#ifndef __IRQNUM_CONFIG_H__
#define __IRQNUM_CONFIG_H__

/**
 *interrupt source
 */
#define INTC_R_NMI_IRQ          0
#define INTC_R_TIMER0_IRQ       1
#define INTC_R_TIMER1_IRQ       2
#define INTC_R_ALM0_IRQ         9
#define INTC_R_ALM1_IRQ         10
#define INTC_R_WDOG_IRQ         11
#define INTC_R_PL_EINT_IRQ      14
#define INTC_R_TH_IRQ		17
#define INTC_R_UART_IRQ         21
#define INTC_R_TWI0_IRQ         26
#define INTC_R_TWI1_IRQ         27
#define INTC_R_RSB_IRQ          31
#define INTC_R_CIR_IRQ          32
#define INTC_R_OWC_IRQ          35
#define INTC_R_PWCTRL_IRQ		37
#define INTC_R_GIC_OUT_IRQ        38
#define INTC_R_CPUIDLE_IRQ		40
#define INTC_R_SPINLOCK_IRQ       64
#define INTC_R_M_BOX_IRQ          65
#define INTC_R_DMA_IRQ            66
#define INTC_R_GINT0_IRQ          67
#define INTC_R_GINT1_IRQ          68
#define INTC_R_GINT2_IRQ          69
#define INTC_R_GINT3_IRQ          70
#define INTC_R_GINT4_IRQ          71
#define INTC_R_GINT5_IRQ          72
#define INTC_R_GINT6_IRQ          73
#define INTC_R_GINT7_IRQ          74
#define INTC_R_GINT8_IRQ          75
#define INTC_R_GINT9_IRQ          76
#define INTC_R_GINT10_IRQ         77
#define INTC_R_GINT11_IRQ         78
/**
 *the max interrupt source number
 */
#define IRQ_R_SOUCE_MAX           80

#define GIC_SRC_SPI               32

static u32 irq_table[][2] = {
	/* cpus irq num          cpux irq num   */
	{INTC_R_NMI_IRQ,       136},		/* external nmi */
	{INTC_R_TIMER0_IRQ,    137},		/* r_timer0 */
	{INTC_R_TIMER1_IRQ,    138},		/* r_timer1 */
	{INTC_R_ALM0_IRQ,      139},		/* r_alarm0 */
	{INTC_R_WDOG_IRQ,      141},		/* r_wdog */
	{INTC_R_PL_EINT_IRQ,   142},		/* r_gpiol */
	{INTC_R_UART_IRQ,      143},		/* r_uart0 */
	{INTC_R_TWI0_IRQ,      144},		/* r_twi0 */
	{INTC_R_TWI1_IRQ,      145},		/* r_twi1 */
	{INTC_R_RSB_IRQ,       146},		/* r_rsb */
	{INTC_R_CIR_IRQ,       147},		/* r_ir */
	{INTC_R_OWC_IRQ,       149},		/* r_owc */
	{INTC_R_CPUIDLE_IRQ,   148},		/* r_cpu_idle */
	{INTC_R_GINT11_IRQ,     97},		/* g_int8, usb0 ehci */
	{INTC_R_GINT11_IRQ,     98},		/* g_int8, usb0 ohci */
};

static u32 grp_irq_table[][2] = {
	/* cpux irq num   group irq num */
	{97,     65},		/* g_int8, usb0 ehci */
	{98,     66},		/* g_int8, usb0 ohci */
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

	for (cpux_irq = 0; cpux_irq < ARRAY_SIZE(grp_irq_table); cpux_irq++) {
		if (grp_irq_table[cpux_irq][0] == (intno + GIC_SRC_SPI))
			return grp_irq_table[cpux_irq][1];
	}

	return FAIL;
}

#endif
