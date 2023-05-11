#include "includes.h"
#include "task.h"
#include "task_handle.h"
#include "msgmanager.h"
#include "cpucfg.h"
#include "psci.h"
#include "wakeup_source.h"
#include "mem.h"
#include "ccu.h"

__TASK_INITIAL(psci_suspend_early_task,DEFAULT_TASK_PRIORITY,TASK_GID_PSCI,DEFAULT_TASK_HANDLE);
__TASK_INITIAL(psci_resume_finish_task,DEFAULT_TASK_PRIORITY,TASK_GID_PSCI,DEFAULT_TASK_HANDLE);
__TASK_INITIAL(alreadywakeup_task,DEFAULT_TASK_PRIORITY,TASK_GID_PSCI,ARISC_WAKEUP_ALREADY);

extern task_struct psci_suspend_late_task;
extern u32 wakeup_source;

/*
static void __hexdump(const char* name, char* base, u32 len)
{
	u32 i;
	printf("%s :\n", name);
	for (i=0; i<len; i+=4) {
		if (!(i&0xf))
			printf("\n0x%8p : ", base + i);
		printf("%8x ", readl(base + i));
		}
	printf("\n");
}
*/

static s32 psci_suspend_early(task_struct *task)
{
	save_state_flag(REC_ESTANDBY | REC_ENTER);

	BYTD_LOG("psci_suspend\n");

#ifdef CFG_HWCPUIDLE_USED
	cpuidle_suspend();
#endif

	cpucfg_cpu_suspend();

	save_state_flag(REC_ESTANDBY | REC_BEFORE_INIT);

	mem_sram_save();

	mem_smc_save();

	mem_smta_save();

	mem_gpio_save();

	mem_ccu_save();

	mem_tmstmp_save();

	save_state_flag(REC_ESTANDBY | REC_ENTER_INIT | 0x1);

	wakeup_task((u32)&psci_suspend_late_task);
	psci_suspend_late_task.para = task->para;

	save_state_flag(REC_ESTANDBY | REC_ENTER_INIT | 0x2);

	return OK;

}

static s32 psci_resume_finish(task_struct *task)
{
	ccu_device *prcm;

	save_state_flag(REC_ESTANDBY | REC_ENTER_EXIT | 0x9);

	prcm = (ccu_device *)ioctl(PRCM_DEVICE_ID,GET_PRCMDEVICE,0);

	save_state_flag(REC_ESTANDBY | REC_ENTER_EXIT | 0xa);

	mem_tmstmp_restore();

	mem_ccu_restore();

	mem_gpio_restore();

	mem_smta_restore();

	mem_smc_restore();

	mem_sram_restore();

	save_state_flag(REC_ESTANDBY | REC_ENTER_EXIT | 0xb);

	cpucfg_cpu_resume((u32)(task->para));

	save_state_flag(REC_ESTANDBY | REC_ENTER_EXIT | 0xc);

	ioctl(PRCM_DEVICE_ID,CPUS_SET_FREQ,200000000);
	prcm->freq = 200000000;
	BYTD_LOG("psci_resume_finish\n");

	save_state_flag(REC_ESTANDBY | REC_ENTER_EXIT | 0xd);

	return OK;
}



static s32 alreadywakeup(task_struct *task)
{
	struct msgblock_head *head;
	struct msgblock *block;
	struct list_head *hnext;

	save_state_flag(REC_ESTANDBY | REC_ENTER_EXIT | 0xe);

	hnext = task->msgparalist->next;

	head = container_of(task->msgparalist,struct msgblock_head,list);
	block = container_of(hnext,struct msgblock,list);

	cpux_ccmu_restore();
	block->paras[0] = wakeup_source;
	save_state_flag(REC_ESTANDBY | REC_ENTER_EXIT | 0xf);

	wakeup_source = NO_WAKESOURCE;

#ifdef CFG_HWCPUIDLE_USED
	/* we must do cpuidle resume until cpux send wakeup already message */
	cpuidle_resume();
#endif

	save_state_flag(REC_ESTANDBY | REC_AFTER_EXIT);

	return OK;
}


int psci_suspend_prepare_init(void)
{
	create_task((u32)(&psci_suspend_early_task),(u32)psci_suspend_early);
	create_task((u32)(&psci_resume_finish_task),(u32)psci_resume_finish);
	create_task((u32)(&alreadywakeup_task),(u32)alreadywakeup);

	set_wakeup_src_init();

	return OK;
}

