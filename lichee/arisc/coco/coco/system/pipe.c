#include "ibase.h"
#include "ioctl.h"
#include "includes.h"
#include "syscall.h"
#include "hwspinlock.h"
#include "task.h"
#include "msgmanager.h"

struct hwspinlock *simsyscall;

s32 __ioctl(u32 devid, u32 requset, va_list args)
{
	arisc_device *dev;
	u32 ret = 0;

	list_for_each_entry(dev,&dev_list,list ) {
		if(dev->dev_id == devid)
			ret = dev->dri->iorequset(requset,args);
	}

	return ret;
}


s32 __div(s32 value, s32 di)
{
	return value/di;
}

static s32 msg_ops(u32 *parg)
{
	u32 type;

	type = parg[0];

	switch (type) {
		case MSG_ALLOCK:
			parg[1] = (u32)allock_msg(parg[1]);
			break;
		case MSG_FREE:
			parg[1] = (u32)allock_msg(parg[1]);
			break;
		case MSG_SETPARAS:
			set_paras_to_msg((struct list_head *)parg[1], (void *)parg[2], parg[3]);
			break;
		case MSG_GETPARAS:
			get_paras_form_msg((struct list_head *)parg[1], (void **)parg[2], parg[3]);
			break;
		case MSG_RESOLVE:
			parg[1] = (u32)resolve_msg(parg[1]);
			break;
		case FEEDBACK_RESOLVE:
			parg[1] = (u32)resolve_msg(parg[1]);
			break;
		default:
			break;
	}

	return OK;
}

static s32 task_ops(u32 *parg)
{
	u32 type;

	type = parg[0];

	switch (type) {
		case TASK_CREATE:
			add_task((task_struct *)parg[1], (__taskCBK_t)parg[2]);
			break;
		case TASK_SLEEP:
			suspend_task((task_struct *)parg[1]);
			break;
		case TASK_WAKEUP:
			resume_task((task_struct *)parg[1]);
			break;
		case TASK_INMSG:
			inherit_msg((task_struct *)parg[1],(task_struct *)parg[2]);
			break;
		default:
			break;
	}
	return OK;
}


static s32 system_ops(u32 *parg)
{
	u32 type;

	type = parg[0];

	switch (type) {
		case SYSTEM_MDELAY:
			time_mdelay(parg[1]);
			break;
		case SYSTEM_MS2TICKS:
			parg[1] = mecs_to_ticks(parg[1]);
			break;
		default:
			break;
	}
	return OK;
}


static s32 simsyscall_handler(void *par)
{
	u32 type;
	u32 *parg = (u32 *)par;

	type = parg[0];

	if((type & 0xff000000) == MSG_OPS) {
		msg_ops(parg);
	}
	else if((type & 0xff000000) == TASK_OPS) {
		task_ops(parg);
	}
	else if((type & 0xff000000) == SYSTEM_OPS) {
		system_ops(parg);
	}

	Spinlock_ClearPending(simsyscall->id);
	return OK;
}

void simsyscall_init(void)
{
	simsyscall = query_spinlock(SIMSYSCLL_SPINLOCK);
	spinlock_set_irqhandler(simsyscall,simsyscall_handler);
	Spinlock_SetIrqEnable(simsyscall->id);
	LOG("simsyscall->id %d\n", simsyscall->id);
}

void simsyscall_suspend(void)
{
	simsyscall = query_spinlock(SIMSYSCLL_SPINLOCK);
	Spinlock_SetIrqDisable(simsyscall->id);
}

void simsyscall_resume(void)
{
	simsyscall = query_spinlock(SIMSYSCLL_SPINLOCK);
	Spinlock_SetIrqEnable(simsyscall->id);
}


void load_bytb(void)
{
	cococall fn = bytb_init;

	fn = (cococall)((u32)(fn) & (~(0xf0000000)));
	fn = (cococall)((u32)(fn) | (0x40000000));

	(*fn)();

	return;
}


