#include "include.h"
#include "mem_pmu.h"

/* backup pmu interrupt enable registers */
static u8 int_enable_regs[5];


s32 mem_pmu_standby_init(struct super_standby_para *para)
{
	mem_printk("mem pmu standby\n");
	u8  devaddr[5];
	u8  regaddr[5];
	u8  enable[5];
	u32 len = 5;
	pmu_paras_t pmu_para;
	u32 event = para->event;
//	u32 gpio_bitmap = para->gpio_enable_bitmap;

	devaddr[0] = RSB_RTSADDR_AW1760;
	devaddr[1] = RSB_RTSADDR_AW1760;
	devaddr[2] = RSB_RTSADDR_AW1760;
	devaddr[3] = RSB_RTSADDR_AW1736;
	devaddr[4] = RSB_RTSADDR_AW1760;

	//read pmu irq enable registers
	regaddr[0] = AW1760_IRQ_ENABLE1;
	regaddr[1] = AW1760_IRQ_ENABLE2;
	regaddr[2] = AW1760_IRQ_ENABLE3;
	regaddr[3] = AW1736_IRQ_ENABLE2;
	regaddr[4] = AW1760_IRQ_ENABLE5;

	pmu_para.devaddr = devaddr;
	pmu_para.regaddr = regaddr;
	pmu_para.data = enable;
	pmu_para.len = len;
	mem_long_jump((mem_long_jump_fn)pmu_reg_read_para, (u32)&pmu_para);
	//pmu_reg_read_para(&pmu_para);

	//backup enable registers
	mem_memcpy(int_enable_regs, enable, len);

	//clear enable array firstly
	mem_memset(enable, 0, len);

	//initialize pmu wakeup events
	if (event & CPUS_WAKEUP_LOWBATT) {
		//AW1760_IRQ_ENABLE1,
		//BIT7 : battery capacity drop to waring level2
		//BIT6 : battery capacity drop to waring level1
		enable[0] |= (0x1 << 6) | (0x1 << 7);
	}
	if (event & CPUS_WAKEUP_BAT_TEMP) {
		//AW1760_IRQ_ENABLE2,
		//BIT7 : battery over temperature in charge mode
		//BIT6 : quit battery over temperatur in charge mode.
		//BIT5 : battery under temperature in charge mode.
		//BIT4 : quit battery under temperature in charge mode.
		//BIT3 : battery over temperature in work mode.
		//BIT2 : quit battery over temperature in work mode.
		//BIT1 : battery under temperature in work mode.
		//BIT0 : quit battery under temperature in work mode.
		enable[1] |= 0xff ;
	}
	if (event & CPUS_WAKEUP_USB) {
		//AW1760_IRQ_ENABLE3,
		//BIT7 :vbus insert.
		//BIT6 :vbus insert.
		enable[2] |= (0x1 << 6) | (0x1 << 7);
	}
	if (event & CPUS_WAKEUP_LONG_KEY) {
		//AW1736_IRQ_ENABLE2,
		//BIT3 : long key.
		enable[3] |= (0x1 << 0);
	}
	if (event & CPUS_WAKEUP_SHORT_KEY) {
		//AW1736_IRQ_ENABLE2,
		//BIT3 : short key.
		enable[3] |= (0x1 << 1);
	}
	if (event & CPUS_WAKEUP_DESCEND) {
		//AW1736_IRQ_ENABLE2,
		//BIT3 : POK negative.
		enable[3] |= (0x1 << 3);
	}
	if (event & CPUS_WAKEUP_ASCEND) {
		//AW1736_IRQ_ENABLE2,
		//BIT6 : POK postive.
		enable[3] |= (0x1 << 4);
	}
	if (event & CPUS_WAKEUP_FULLBATT)
	{
		//AW1760_IRQ_ENABLE5,
		//BIT4 : battery is full/charge done.
		enable[4] |= (0x1 << 6);
	}

	//write pmu enable registers
	mem_long_jump((mem_long_jump_fn)pmu_reg_write_para, (u32)&pmu_para);
	//pmu_reg_write_para(&pmu_para);

	return OK;
}

s32 mem_pmu_standby_exit(void)
{
	u8 devaddr[5];
	u8 regaddr[5];
	u32 len = 5;
	pmu_paras_t pmu_para;

	//restore pmu irq enable registers
	devaddr[0] = RSB_RTSADDR_AW1760;
	devaddr[1] = RSB_RTSADDR_AW1760;
	devaddr[2] = RSB_RTSADDR_AW1760;
	devaddr[3] = RSB_RTSADDR_AW1736;
	devaddr[4] = RSB_RTSADDR_AW1760;

	regaddr[0] = AW1760_IRQ_ENABLE1;
	regaddr[1] = AW1760_IRQ_ENABLE2;
	regaddr[2] = AW1760_IRQ_ENABLE3;
	regaddr[3] = AW1736_IRQ_ENABLE2;
	regaddr[4] = AW1760_IRQ_ENABLE5;

	pmu_para.devaddr = devaddr;
	pmu_para.regaddr = regaddr;
	pmu_para.data = int_enable_regs;
	pmu_para.len = len;
	mem_long_jump((mem_long_jump_fn)pmu_reg_write_para, (u32)&pmu_para);
	//pmu_reg_write_para(&pmu_para);

	return OK;
}

