#include "includes.h"
#include "task.h"
#include "task_handle.h"
#include "msgmanager.h"
#include "cpucfg.h"
#include "psci.h"
#include "bytbmsg.h"

__TASK_INITIAL(psci_task,DEFAULT_TASK_PRIORITY,TASK_GID_PSCI,PSCI_CPU_OPS);
__TASK_INITIAL(sys_op_task,DEFAULT_TASK_PRIORITY,TASK_GID_PSCI,SYS_OP_REQ);


extern  task_struct psci_suspend_early_task;

static s32 psci_ops(task_struct *task)
{
	u32 mpidr;
	u32 entrypoint;
	u32 cpu_state;
	u32 cluster_state;
	u32 system_state;
	struct psci_paras pa;
	struct psci_paras *p;
	p = &pa;

	bytb_get_paras_form_msg(task->msgparalist,(void **)&p,5 * 4);

	mpidr = pa.mpidr;
	entrypoint = pa.entrypoint;
	cpu_state = pa.cpu_state;
	cluster_state = pa.cluster_state;
	system_state = pa.sys_state;

	BYTD_LOG("mpidr:%x, entrypoint:%x; cpu_state:%x, cluster_state:%x, system_state:%x\n",
		mpidr, entrypoint, cpu_state, cluster_state, system_state);

	if (cpu_state == ARISC_POWER_ON) {
		set_secondary_entry(entrypoint, mpidr);
		cpu_power_up(0, mpidr);
	} else if (cpu_state == ARISC_POWER_OFF) {
		if (entrypoint) {
			if (system_state == ARISC_POWER_OFF) {
				psci_suspend_early_task.para = (void *)entrypoint;
				wakeup_task((u32)(&psci_suspend_early_task));
			}
		} else {
			cpu_power_down(0, mpidr);
		}
	}

	return OK;
}

static s32 psci_sys_op(task_struct *task)
{
	u32 state;
	u32 pa;
	u32 *p;
	p = &pa;

	bytb_get_paras_form_msg(task->msgparalist,(void **)&p,1 * 4);

	state = pa;

	switch (state)
	{
		case arisc_system_shutdown:
		{
			save_state_flag(REC_SHUTDOWN | 0x100);
			ioctl(PMU_DEVICE_ID, IOCTL_PMU_SHUTDOWN, NULL);
			break;
		}
		case arisc_system_reset:
		case arisc_system_reboot:
		{
			save_state_flag(REC_SHUTDOWN | 0x200);
			ioctl(PMU_DEVICE_ID, IOCTL_PMU_RESET, NULL);
			break;
		}
		default:
		{
			WRN("invaid system power state (%d)\n", state);
			return -EINVAL;
		}
	}

	return OK;
}

int psci_platform_init(void)
{
	create_task((u32)(&psci_task),(u32)psci_ops);
	create_task((u32)(&sys_op_task),(u32)psci_sys_op);
	psci_suspend_prepare_init();
	return OK;
}





