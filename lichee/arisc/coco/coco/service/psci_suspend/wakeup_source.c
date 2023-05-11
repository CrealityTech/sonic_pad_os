#include "ibase.h"
#include "task.h"
#include "task_handle.h"
#include "msgmanager.h"
#include "cpucfg.h"
#include "psci.h"
#include "wakeup_source.h"

struct wakeup_loop_handle loop_handle;

int loop_wakesource_init(u32 loop_num,__pCBK_t handler, void *parg, u32 irq_num)
{
	loop_handle.handle[loop_num] = (__pCBK_t)handler;
	loop_handle.parg[loop_num] =  parg;

	if(irq_num != 0) {
		interrupt_enable(irq_num);
		interrupt_set_mask(irq_num,1);
	}

	return OK;
}

int loop_wakesource_exit(u32 irq_num)
{
	if(irq_num != 0) {
		interrupt_disable(irq_num);
		interrupt_set_mask(irq_num,0);
	}

	return OK;
}



int wakesource_loop(void)
{
	int i = 0;

	for(i = 0; i < WAKESOURCE_LOOP_MAX; i++) {
		if(loop_handle.handle[i] != NULL)
			loop_handle.handle[i](loop_handle.parg[i]);
	}

	return OK;
}

