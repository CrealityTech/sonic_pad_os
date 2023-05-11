#include "ibase.h"
#include "task.h"
#include "task_handle.h"
#include "msgmanager.h"
#include "dram.h"

__TASK_INITIAL(dram_dvfs_task, DEFAULT_TASK_PRIORITY, TASK_GID_PSCI, ARISC_DRAM_DVFS_REQ);

static s32 dram_dvfs_task_work(task_struct *task)
{
	unsigned int paras[5] = {0};
	unsigned int *para_p = paras;

	get_paras_form_msg((struct list_head *)task->msgparalist, (void **)(&para_p), 2 * sizeof(int));

	mdfs_dfs(dram_para, paras[0]);

	return OK;
}

s32 dramfreq_init(void)
{
	add_task(&dram_dvfs_task, (__taskCBK_t)dram_dvfs_task_work);

	return OK;
}
