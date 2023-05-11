#include "ibase.h"
#include "task.h"
#include "task_handle.h"
#include "msgmanager.h"
#include "cpucfg.h"
#include "psci.h"
#include "wakeup_source.h"
#include "standby_dram_crc.h"
#include "standby_power.h"
#include "notifier.h"
#include "softtimer.h"
#include "dram.h"

__TASK_INITIAL(psci_suspend_late_task,DEFAULT_TASK_PRIORITY,TASK_GID_PSCI,DEFAULT_TASK_HANDLE);
__TASK_INITIAL(psci_resume_task,DEFAULT_TASK_PRIORITY,TASK_GID_PSCI,DEFAULT_TASK_HANDLE);
__TASK_INITIAL(psci_waitwakeup_task,DEFAULT_TASK_PRIORITY,TASK_GID_PSCI,DEFAULT_TASK_HANDLE);

__TASK_INITIAL(wakeuptimer_task,DEFAULT_TASK_PRIORITY,0,DEFAULT_TASK_HANDLE);

struct notifier *wakeup_notify = NULL;
struct softtimer wakeup_timer;
u32 wakeup_source= 0;
u32 before_crc;
u32 after_crc;

extern task_struct psci_resume_finish_task;
static ccu_device *prcm;
static ccu_device *ccmu;

extern struct notifier *hosc_notifier_list;
extern struct notifier *apbs2_notifier_head;

#if 0
u32 dram_crc_enable  = 1;
u32 dram_crc_src     = 0x40000000;
u32 dram_crc_len     = (1024 * 1024);

static s32 standby_set_dram_crc_paras(u32 enable, u32 src, u32 len)
{
	dram_crc_enable = enable;
	dram_crc_src    = src;
	dram_crc_len    = len;

	return OK;
}

static s32 standby_dram_crc_enable(void)
{
	return dram_crc_enable;
}

static u32 standby_dram_crc(void)
{
	u32 *pdata = (u32 *)(dram_crc_src);
	u32 crc = 0;

	printk("crc begin src:%x len:%x\n", dram_crc_src, dram_crc_len);
	while (pdata < (u32 *)(dram_crc_src + dram_crc_len))
	{
		crc += *pdata;
		pdata++;
	}
	printk("crc finish...\n");

	return crc;
}

#endif

u32 pll_periph0 = 0;
u32 mbus = 0;

static s32 device_suspend(void)
{
	arisc_device *dev;
	u32 ret = OK;

	list_for_each_entry(dev, &dev_list, list) {
		if (dev->dev_id == HWMSGBOX_DEVICE_ID || dev->dev_id == HWSPINLOCK_DEVICE_ID) {
			if (dev->dri->suspend != NULL) {
				ret = dev->dri->suspend(dev);
				if(ret != OK) {
					LOG("%s fail, dev_id: %d\n", __func__, dev->dev_id);
					break;
				}
			}
		}
	}

	return ret;
}

static s32 device_resume(void)
{
	arisc_device *dev;
	u32 ret = OK;

	list_for_each_entry(dev, &dev_list, list) {
		if (dev->dev_id == HWMSGBOX_DEVICE_ID || dev->dev_id == HWSPINLOCK_DEVICE_ID) {
			if (dev->dri->resume != NULL) {
				ret = dev->dri->resume(dev);
				if(ret != OK) {
					LOG("%s fail, dev_id: %d\n", __func__, dev->dev_id);
					break;
				}
			}
		}
	}

	return ret;
}

static void dram_suspend(void)
{
	if (standby_dram_crc_enable())
	{
		before_crc = standby_dram_crc();
	}
	LOG("before_crc: 0x%x\n", before_crc);

	pll_periph0 = readl(CCMU_REG_BASE + PLL_PERI0_CTRL_REG);
	mbus = readl(CCMU_REG_BASE + MBUS_CLK_REG);
	dram_power_save_process();
}


static void dram_resume(void)
{
	//restore dram controller and transing area.
	INF("power-up dram\n");
	writel(readl(CCMU_REG_BASE + PLL_PERI0_CTRL_REG) & (~(1 << 31)), CCMU_REG_BASE + PLL_PERI0_CTRL_REG); //disable pll_periph0 firstly
	writel((pll_periph0 & (~(0x1 << 31))), CCMU_REG_BASE + PLL_PERI0_CTRL_REG); //set pll factor but not enable pll
	time_udelay(100); //delay 100us for factor to be effective
	writel((readl(CCMU_REG_BASE + PLL_PERI0_CTRL_REG) | (0x1 << 31)), CCMU_REG_BASE + PLL_PERI0_CTRL_REG); //enable pll
	time_udelay(20);
	//mbus default clk src is 24MHz, switch to pll_periph0(x2),
	//so before increase mbus freq, should set div firstly.
	//by Superm Wu at 2015-09-18
#ifdef CFG_EVB_FLATFORM
	writel(mbus&0x7, CCMU_REG_BASE + MBUS_CLK_REG);
	time_udelay(200);
	writel(mbus&((0x3 << 24) | 0x7),CCMU_REG_BASE + MBUS_CLK_REG);
	time_udelay(20);
	writel((readl(CCMU_REG_BASE + MBUS_CLK_REG) | (0x1 << 31)), CCMU_REG_BASE + MBUS_CLK_REG);
	time_udelay(10000);
	dram_power_up_process();

	// calc dram checksum
	if (standby_dram_crc_enable())
	{
		LOG("dram crc check...\n");
		after_crc = standby_dram_crc();
		if(after_crc != before_crc)
		{
			//save_state_flag(REC_SSTANDBY | REC_DRAM_DBG | 0xf);
			ERR("dram crc error: 0x%x\n", after_crc);
			ERR("---->>>>LOOP<<<<----\n");
			while(1);
		}
	}

#else
	dram_power_up_process();
#endif
}

static void hosc_off(void)
{
	u32 value;

	if(!(readl(RTC_XO_CTRL) & (1 << 3))) {
		//disable 24mhosc
		value = (readl(R_PRCM_REG_BASE + PLL_CONTROL_REG1) | (0xa7 << 24));
		writel(value, R_PRCM_REG_BASE + PLL_CONTROL_REG1);
		value = (readl(R_PRCM_REG_BASE + PLL_CONTROL_REG1) | (0xa7 << 24));
		value &= (~(0x1 << 2));
		writel(value, R_PRCM_REG_BASE + PLL_CONTROL_REG1);
	} else {

		//disable DCXO24mhosc
		value = readl(RTC_XO_CTRL);
		value &= (~(0x1 << 1));
		writel(value, RTC_XO_CTRL);
	}
	//wait 20 cycles
	time_cdelay(20);

	//power-off pll ldo
	value = (readl(R_PRCM_REG_BASE + PLL_CONTROL_REG1) | (0xa7 << 24));
	writel(value, R_PRCM_REG_BASE + PLL_CONTROL_REG1);
	value = (readl(R_PRCM_REG_BASE + PLL_CONTROL_REG1) | (0xa7 << 24));
	value &= (~(0x1 << 0));
	writel(value, R_PRCM_REG_BASE + PLL_CONTROL_REG1);

}

static void hosc_on(void)
{
	u32 value = 0;

	//power-on pll ldo
	value = (readl(R_PRCM_REG_BASE + PLL_CONTROL_REG1) | (0xa7 << 24));
	writel(value, R_PRCM_REG_BASE + PLL_CONTROL_REG1);
	value = (readl(R_PRCM_REG_BASE + PLL_CONTROL_REG1) | (0xa7 << 24));
	value |= (0x1 << 0);
	writel(value, R_PRCM_REG_BASE + PLL_CONTROL_REG1);

	//wait 2ms for power-on ready
	time_mdelay(2);

	if(!(readl(RTC_XO_CTRL) & (1 << 3))) {
		//enable 24mhosc
		value = (readl(R_PRCM_REG_BASE + PLL_CONTROL_REG1) | (0xa7 << 24));
		writel(value, R_PRCM_REG_BASE + PLL_CONTROL_REG1);
		value = (readl(R_PRCM_REG_BASE + PLL_CONTROL_REG1) | (0xa7 << 24));
		value |= (0x1 << 2);
		writel(value, R_PRCM_REG_BASE + PLL_CONTROL_REG1);
	} else {
		//enable DCXO24mhosc
		value = readl(RTC_XO_CTRL);
		value |= (0x1 << 1);
		writel(value, RTC_XO_CTRL);
	}
	//wait 2ms for 24m hosc ready
	time_mdelay(10);
	//adjust pll voltage to 1.45v
	value = (readl(R_PRCM_REG_BASE + PLL_CONTROL_REG1) | (0xa7 << 24));
	writel(value, R_PRCM_REG_BASE + PLL_CONTROL_REG1);
	value = (readl(R_PRCM_REG_BASE + PLL_CONTROL_REG1) | (0xa7 << 24));
	value &= (~(0x7 << 16));
	value |= (0x4 << 16);
	writel(value, R_PRCM_REG_BASE + PLL_CONTROL_REG1);

	//wait 2ms for voltage ready
	time_mdelay(2);
}

static void system_suspend(void)
{
	u32 value = 0;

	value = readl(R_PRCM_REG_BASE + VDD_SYS_POWEROFF_GATING_REG);
	value |= ((0x1 << 3));
	writel(value,R_PRCM_REG_BASE + VDD_SYS_POWEROFF_GATING_REG);

	value = readl(R_PRCM_REG_BASE + VDD_SYS_POWEROFF_GATING_REG);
	value |= ((0x1 << 2));
	writel(value,R_PRCM_REG_BASE + VDD_SYS_POWEROFF_GATING_REG);

	value = readl(R_PRCM_REG_BASE + GPU_POWEROFF_GATING_REG);
	value &= (~(0x1 << 0));
	writel(value,R_PRCM_REG_BASE + GPU_POWEROFF_GATING_REG);

	value = readl(R_PRCM_REG_BASE + VDD_SYS_PWOER_RESET_REG);
	value &= (~(0x1 << 0));
	writel(value,R_PRCM_REG_BASE + VDD_SYS_PWOER_RESET_REG);

}

static void system_resume(void)
{
	u32 value = 0;

	value = readl(R_PRCM_REG_BASE + VDD_SYS_POWEROFF_GATING_REG);
	value &= (~(0x1 << 3));
	writel(value,R_PRCM_REG_BASE + VDD_SYS_POWEROFF_GATING_REG);

	value = readl(R_PRCM_REG_BASE + VDD_SYS_POWEROFF_GATING_REG);
	value &= (~(0x1 << 2));
	writel(value,R_PRCM_REG_BASE + VDD_SYS_POWEROFF_GATING_REG);

	time_mdelay(1);
	value = readl(R_PRCM_REG_BASE + GPU_POWEROFF_GATING_REG);
	value |= ((0x1 << 0));
	writel(value,R_PRCM_REG_BASE + GPU_POWEROFF_GATING_REG);
	time_mdelay(1);

	value = readl(R_PRCM_REG_BASE + VDD_SYS_PWOER_RESET_REG);
	value |= ((0x1 << 0));
	writel(value,R_PRCM_REG_BASE + VDD_SYS_PWOER_RESET_REG);

}
static void clk_suspend(void)
{
	prcm = get_prcm_device();
	ccmu = get_ccmu_device();

	prcm_set_src(prcm,CPUS_CONFIG_REG,PRCM_SRC_IOSC);
	prcm_set_div(prcm,CPUS_CONFIG_REG,1);
	prcm->freq = 16000000;

	ccmu_set_freq(ccmu,CPU_AXI_CFG_REG,CCMU_SRC_LOSC);
	ccmu_set_freq(ccmu,PSI_AHB1_AHB2_CFG_REG,CCMU_SRC_LOSC);
	ccmu_set_freq(ccmu,AHB3_CFG_REG,CCMU_SRC_LOSC);
	ccmu_set_freq(ccmu,APB1_CFG_REG,CCMU_SRC_LOSC);
	ccmu_set_freq(ccmu,APB2_CFG_REG,CCMU_SRC_LOSC);

	ccmu_disbale_clk(ccmu,PLL_CPUX_CTRL_REG);
	ccmu_disbale_clk(ccmu,PLL_PERI0_CTRL_REG);
	ccmu_disbale_clk(ccmu,PLL_PERI1_CTRL_REG);
	ccmu_disbale_clk(ccmu,PLL_DDR_CTRL_REG);
	ccmu_disbale_clk(ccmu,PLL_GPU_CTRL_REG);
	ccmu_disbale_clk(ccmu,PLL_VIDEO0_CTRL_REG);
	ccmu_disbale_clk(ccmu,PLL_VIDEO1_CTRL_REG);
	ccmu_disbale_clk(ccmu,PLL_VE_CTRL_REG);
	ccmu_disbale_clk(ccmu,PLL_DE_CTRL_REG);
	ccmu_disbale_clk(ccmu,PLL_AUDIO_CTRL_REG);

	notifier_notify(&hosc_notifier_list,CCU_HOSC_WILL_OFF_NOTIFY);

	notifier_notify(&apbs2_notifier_head,CCU_APBS2_CLK_CLKCHG_REQ);
	prcm_set_src(prcm,APBS2_CONFIG_REG,PRCM_SRC_IOSC);
	prcm_set_div(prcm,APBS2_CONFIG_REG,1);
	notifier_notify(&apbs2_notifier_head,CCU_ABPS2_CLK_CLKCHG_DONE);

	hosc_off();
}

static void clk_resume(void)
{
	prcm = get_prcm_device();
	ccmu = get_ccmu_device();

	hosc_on();
	notifier_notify(&hosc_notifier_list,CCU_HOSC_ON_READY_NOTIFY);

	ccmu_set_freq(ccmu,CPU_AXI_CFG_REG,CCMU_SRC_HOSC);
	ccmu_set_m_div(CPU_AXI_CFG_REG, 1);

	ccmu_set_freq(ccmu,PSI_AHB1_AHB2_CFG_REG,CCMU_SRC_HOSC);
	ccmu_set_freq(ccmu,AHB3_CFG_REG,CCMU_SRC_HOSC);
	ccmu_set_freq(ccmu,APB1_CFG_REG,CCMU_SRC_HOSC);
	ccmu_set_freq(ccmu,APB2_CFG_REG,CCMU_SRC_HOSC);

	prcm_set_src(prcm,CPUS_CONFIG_REG,PRCM_SRC_HOSC);
	prcm_set_div(prcm,CPUS_CONFIG_REG,1);
	prcm->freq = 24000000;

	notifier_notify(&apbs2_notifier_head,CCU_APBS2_CLK_CLKCHG_REQ);
	prcm_set_src(prcm,APBS2_CONFIG_REG,PRCM_SRC_HOSC);
	prcm_set_div(prcm,APBS2_CONFIG_REG,1);
	notifier_notify(&apbs2_notifier_head,CCU_ABPS2_CLK_CLKCHG_DONE);

}

static void dm_suspend(void)
{
	standby_powerdown();
}

static void dm_resume(void)
{
	standby_powerup();
}

static s32 psci_suspend_late(task_struct * task __attribute__ ((__unused__)))
{
	save_state_flag(REC_ESTANDBY | REC_ENTER_INIT | 0x3);

	device_suspend();
	save_state_flag(REC_ESTANDBY | REC_ENTER_INIT | 0x4);

	dram_suspend();
	save_state_flag(REC_ESTANDBY | REC_ENTER_INIT | 0x5);

	cpucfg_cpu_suspend_late();
	save_state_flag(REC_ESTANDBY | REC_ENTER_INIT | 0x6);

	clk_suspend();
	save_state_flag(REC_ESTANDBY | REC_ENTER_INIT | 0x7);

	system_suspend();
	save_state_flag(REC_ESTANDBY | REC_ENTER_INIT | 0x8);

	dm_suspend();
	save_state_flag(REC_ESTANDBY | REC_ENTER_INIT | 0x9);

	start_softtimer(&wakeup_timer,SOFTTIMER_SINGLE);
	save_state_flag(REC_ESTANDBY | REC_ENTER_INIT | 0xa);

	resume_task(&psci_waitwakeup_task);
	psci_waitwakeup_task.para = task->para;
	LOG("psci_suspend susseed,wait wakeup\n");
	save_state_flag(REC_ESTANDBY | REC_AFTER_INIT);

	return OK;
}

static s32 psci_waitwakeup(task_struct * task __attribute__ ((__unused__)))
{
//	wakesource_loop();

	if (wakeup_source != NO_WAKESOURCE) {
		stop_softtimer(&wakeup_timer);
		task->keep_run = 0;
		resume_task(&psci_resume_task);
		psci_resume_task.para = task->para;
		LOG("wakeup: 0x%x!\n", wakeup_source);
		save_state_flag(REC_ESTANDBY | REC_BEFORE_EXIT);
	} else if (wakeup_source == NO_WAKESOURCE) {
		task->keep_run = 1;
		save_state_flag(REC_ESTANDBY | REC_WAIT_WAKEUP);
	}

	return OK;
}

static s32 psci_resume(task_struct * task __attribute__ ((__unused__)))
{

	save_state_flag(REC_ESTANDBY | REC_ENTER_EXIT);

	dm_resume();
	save_state_flag(REC_ESTANDBY | REC_ENTER_EXIT | 0x1);

	system_resume();
	save_state_flag(REC_ESTANDBY | REC_ENTER_EXIT | 0x2);

	clk_resume();
	save_state_flag(REC_ESTANDBY | REC_ENTER_EXIT | 0x3);

	cpucfg_cpu_resume_early();
	save_state_flag(REC_ESTANDBY | REC_ENTER_EXIT | 0x4);

	dram_resume();
	save_state_flag(REC_ESTANDBY | REC_ENTER_EXIT | 0x5);

	device_resume();
	save_state_flag(REC_ESTANDBY | REC_ENTER_EXIT | 0x6);

	resume_task(&psci_resume_finish_task);
	psci_resume_finish_task.para = task->para;
	save_state_flag(REC_ESTANDBY | REC_ENTER_EXIT | 0x7);

	prcm_set_freq(prcm,CPUS_CONFIG_REG,200000000);
	prcm->freq = 200000000;
	save_state_flag(REC_ESTANDBY | REC_ENTER_EXIT | 0x8);

	LOG("psci_resume\n");
	return OK;
}

static s32 wakeup_source_monitor(u32 message, void *aux)
{
	u32 *wakup_s = (u32 *)aux;

	if(message != NO_WAKESOURCE)
		*wakup_s |= message;

	return OK;
}

static void wakeuptimer(task_struct * task __attribute__ ((__unused__)))
{
	notifier_notify(&wakeup_notify,WAKESOURE_TIMER);
}

s32 default_wakeup_handler(void *parg __attribute__ ((__unused__)), u32 intno)
{
	/**
	 *when wakeup event happen, we should mask the r_intc interrupt here first,
	 *otherwise cpus can't go out of the interrupt handler and the resume process can't go ahead.
	 */
	interrupt_set_mask(intno, TRUE);

	notifier_notify(&wakeup_notify,CPUS_IRQ_MAPTO_CPUX(intno));

	save_state_flag(REC_ESTANDBY | REC_WAIT_WAKEUP | (CPUS_IRQ_MAPTO_CPUX(intno) & 0xff));

	return OK;
}

int psci_suspend_core_init(void)
{
	add_task(&psci_suspend_late_task,(__taskCBK_t)psci_suspend_late);
	add_task(&psci_resume_task,(__taskCBK_t)psci_resume);
	add_task(&psci_waitwakeup_task,(__taskCBK_t)psci_waitwakeup);

	wakeup_source = NO_WAKESOURCE;
	notifier_insert(&wakeup_notify,wakeup_source_monitor,&wakeup_source);

	add_task(&wakeuptimer_task,(__taskCBK_t)wakeuptimer);

	wakeup_timer.ticks = 0;
	wakeup_timer.expires = 0;
	wakeup_timer.task = &wakeuptimer_task;
	wakeup_timer.arg = NULL;

	add_softtimer(&wakeup_timer);

	standby_dram_crc_init();

	return OK;
}

