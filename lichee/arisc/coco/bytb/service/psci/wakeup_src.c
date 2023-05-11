#include "includes.h"
#include "task.h"
#include "task_handle.h"
#include "msgmanager.h"
#include "cpucfg.h"
#include "psci.h"
#include "wakeup_source.h"
#include "softtimer.h"


__TASK_INITIAL(set_wakeup_src_task,DEFAULT_TASK_PRIORITY,TASK_GID_PSCI,ARISC_SET_WAKEUP_SRC);
__TASK_INITIAL(clear_wakeup_src_task,DEFAULT_TASK_PRIORITY,TASK_GID_PSCI,ARISC_CLEAR_WAKEUP_SRC);


extern struct softtimer wakeup_timer;
extern u32 wakeup_source;

static int group_irq_init(void *parg)
{
	u32 *p = (u32 *)parg;
	u32 wakeup_root_irq = p[0];

	s32 group_irq_num = CPUX_IRQ_MAPTO_CPUS_GRP(wakeup_root_irq);

	/* this is a group irq, we need to set group config, otherwise do nothing. */
	if (group_irq_num != FAIL)
		ioctl(INTC_DEVICE_ID, INTC_SET_GRP_CONFIG, group_irq_num, TRUE);

	return OK;
}

static int group_irq_exit(void *parg)
{
	u32 *p = (u32 *)parg;
	u32 wakeup_root_irq = p[0];

	s32 group_irq_num = CPUX_IRQ_MAPTO_CPUS_GRP(wakeup_root_irq);

	/* this is a group irq, we need to set group config, otherwise do nothing. */
	if (group_irq_num != FAIL)
		ioctl(INTC_DEVICE_ID, INTC_SET_GRP_CONFIG, group_irq_num, FALSE);

	return OK;
}
static int irq_wakesource_init(s32 irq_num,__pCBK_t init,void *init_parg ,__pISR_t handler, void *parg)
{
	if(init != NULL)
		(*init)(init_parg);

	if(handler != NULL)
		ioctl(INTC_DEVICE_ID, INTC_INSTALL_ISR, irq_num, (u32)handler, (u32)parg);

	ioctl(INTC_DEVICE_ID, INTC_ENABLE_IRQ, irq_num);
	ioctl(INTC_DEVICE_ID, INTC_SET_MASK, irq_num, FALSE);

	return OK;
}

static int irq_wakesource_exit(s32 irq_num,__pCBK_t exit,void *exit_parg ,__pISR_t handler, void *parg)
{
	if(exit != NULL)
		(*exit)(exit_parg);

	if(handler != NULL)
		ioctl(INTC_DEVICE_ID, INTC_INSTALL_ISR, irq_num, (u32)handler, (u32)parg);

	ioctl(INTC_DEVICE_ID, INTC_SET_MASK, irq_num, TRUE);

	return OK;
}

static s32 set_wakeup_src(task_struct *task)
{
	struct msgblock_head *head;
	struct msgblock *block;
	struct list_head *hnext;

	u32 wakeup_src_type;
	u32 wakeup_root_irq;
	u32 wakeup_secondary_irq;
	u32 wakeup_thrid_irq;
	u32 time_to_wakeup;

	s32 irq_no;
	u32 para[3];

	hnext = task->msgparalist->next;

	head = container_of(task->msgparalist,struct msgblock_head,list);
	block = container_of(hnext,struct msgblock,list);

	wakeup_src_type = (block->paras[0] >> 30) & 0x3;
	wakeup_root_irq = block->paras[0] & 0x3FF;
	wakeup_secondary_irq = (block->paras[0] >> 10) & 0x3FF;
	wakeup_thrid_irq = (block->paras[0] >> 20) & 0x3FF;

	BYTD_LOG("wakeup_root_irq: %d\n", wakeup_root_irq);

	if(wakeup_src_type == 0x3) {
		time_to_wakeup =  block->paras[0] & 0x3fffffff;
		wakeup_timer.ticks = ms_to_ticks(time_to_wakeup);
	}
	else {
		irq_no = CPUX_IRQ_MAPTO_CPUS(wakeup_root_irq);
		if(irq_no == -1){
			BYTD_ERR("%s(%d) irq_no error, root_irq %x\n",
				__func__, __LINE__, wakeup_root_irq);
			return -1;
		}

		/**
		 * 1.we should clear wakeup_source here to avoid
		 * missing or remain of wakeup event.
		 * 2.FIXME: modify wakeup_source only in process context
		 */
		if (wakeup_source == wakeup_root_irq) {
			wakeup_source = NO_WAKESOURCE;
		}

		switch (irq_no)
		{
			case INTC_R_PL_EINT_IRQ:
				{
					para[0] = wakeup_secondary_irq;
					para[1] = GPIO_GP_PL;
					para[2] = (u32)default_wakeup_handler;
					irq_wakesource_init(irq_no, default_pin_wakeup_init, &para[0],
							NULL, NULL);
					break;
				}
#ifdef CFG_PIN_M_USED
			case INTC_R_PM_EINT_IRQ:
				{
					para[0] = wakeup_secondary_irq;
					para[1] = GPIO_GP_PM;
					para[2] = (u32)default_wakeup_handler;
					irq_wakesource_init(irq_no, pin_wakeup_init, &para[0],
							NULL, NULL);
					break;
				}
#endif

#ifdef CFG_CIR_WAKEUP
			case INTC_R_CIR_IRQ:
				{
					irq_wakesource_init(irq_no,cir_wakeup_init,NULL,
							cir_wakeup_handler,NULL);
					break;
				}
#endif
			case INTC_R_NMI_IRQ:
				{
					irq_wakesource_init(irq_no, NULL, NULL,
							default_wakeup_handler, NULL);
					break;
				}
			default:
				{
					para[0] = wakeup_root_irq;
					para[1] = wakeup_secondary_irq;
					para[2] = wakeup_thrid_irq;
					irq_wakesource_init(irq_no, group_irq_init, &para[0],
							default_wakeup_handler, NULL);
					break;
				}
		}
	}

	return OK;
}


static s32 clear_wakeup_src(task_struct *task)
{
	struct msgblock_head *head;
	struct msgblock *block;
	struct list_head *hnext;

	u32 wakeup_src_type;
	u32 wakeup_root_irq;
	u32 wakeup_secondary_irq;
	u32 wakeup_thrid_irq;
	u32 time_to_wakeup;

	s32 irq_no;
	u32 para[3];

	hnext = task->msgparalist->next;

	head = container_of(task->msgparalist,struct msgblock_head,list);
	block = container_of(hnext,struct msgblock,list);

	wakeup_src_type = (block->paras[0] >> 30) & 0x3;
	wakeup_root_irq = block->paras[0] & 0x3FF;
	wakeup_secondary_irq = (block->paras[0] >> 10) & 0x3FF;
	wakeup_thrid_irq = (block->paras[0] >> 20) & 0x3FF;

	if(wakeup_src_type == 0x3) {
		time_to_wakeup =  block->paras[0] & 0x3fffffff;
		wakeup_timer.ticks = ms_to_ticks(time_to_wakeup);
	}
	else {
		irq_no = CPUX_IRQ_MAPTO_CPUS(wakeup_root_irq);
		if(irq_no == -1){
			BYTD_ERR("%s(%d) irq_no error, root_irq %x\n",
				__func__, __LINE__, wakeup_root_irq);
			return -1;
		}
		switch (irq_no)
		{
			case INTC_R_PL_EINT_IRQ:
				{
					para[0] = wakeup_secondary_irq;
					para[1] = GPIO_GP_PL;
					para[2] = (u32)NULL;
					irq_wakesource_exit(irq_no, default_pin_wakeup_exit, &para[0],
							NULL, NULL);
					break;
				}
#ifdef CFG_PIN_M_USED
			case INTC_R_PM_EINT_IRQ:
				{
					para[0] = wakeup_secondary_irq;
					para[1] = GPIO_GP_PM;
					para[2] = (u32)default_wakeup_handler;
					irq_wakesource_init(irq_no, pin_wakeup_init, &para[0],
							NULL, NULL);
					break;
				}
#endif

#ifdef CFG_CIR_WAKEUP
			case INTC_R_CIR_IRQ:
				{
					irq_wakesource_init(irq_no,cir_wakeup_init,NULL,
							cir_wakeup_handler,NULL);
					break;
				}
#endif
			case INTC_R_NMI_IRQ:
				{
					irq_wakesource_exit(irq_no, NULL, NULL,
							default_wakeup_handler, NULL);
					break;
				}
			default:
				{
					para[0] = wakeup_root_irq;
					para[1] = wakeup_secondary_irq;
					para[2] = wakeup_thrid_irq;
					irq_wakesource_exit(irq_no, group_irq_exit, &para[0],
							default_wakeup_handler, NULL);
					break;
				}
		}
	}

	return OK;
}

int set_wakeup_src_init(void)
{
	create_task((u32)(&set_wakeup_src_task),(u32)set_wakeup_src);
	create_task((u32)(&clear_wakeup_src_task),(u32)clear_wakeup_src);

	return OK;
}



