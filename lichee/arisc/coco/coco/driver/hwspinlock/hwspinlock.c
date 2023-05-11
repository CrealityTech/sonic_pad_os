/*
*********************************************************************************************************
*                                                AR100 SYSTEM
*                                     AR100 Software System Develop Kits
*
*
*                                    (c) Copyright 2018-2024, Luojie China
*                                             All Rights Reserved
*
* File    : hwspinlock.c
* By      : LuoJie
* Version : v1.0
* Date    : 2018-06-05 16:32
* Descript: .
* Update  :auther      ver     notes
*          Luojie      1.0     Create this file.
*********************************************************************************************************
*/
#include "hwspinlock_i.h"

extern void simsyscall_suspend(void);
extern void simsyscall_resume(void);


static struct hwspinlock_device spinlock = {
	.hwspinlock_count = 0,
	.hwspinlock_num = MAX_SPINLOCK_NUM,
	.ref = {0},
};

//static struct hwspinlock locklock[8];

s32 hwspin_lock_timeout(struct hwspinlock *lock, u32 timeout)
{
	/* convert to cpu clcyes */
	u32 expire = timeout * 2000000;

	/* disable cpu interrupt, save cpsr to cpsr-table. */
	lock->cpsr = cpu_disable_int();

	if (spinlock.dev.suspend_status) {
		return -EACCES;
	}

	/* try to take spinlock */
	while (Spinlock_ReadLock(lock->reg) == SPINLOCK_TAKEN) {
		/* spinlock is busy */
		if (expire == 0) {
			ERR("lock to%x \n", (u32)lock);
			cpu_enable_int(lock->cpsr);
			return -ETIMEOUT;
		}
		expire--;
	}

	/* spinlock is been taken */
	lock->locked = 1;
	return OK;
}


s32 hwspin_unlock(struct hwspinlock *lock)
{
	if(spinlock.dev.suspend_status) {
		cpu_enable_int(lock->cpsr);

		return FAIL;
	}

	Spinlock_WriteLock(lock->reg,0);

	lock->locked = 0;
	cpu_enable_int(lock->cpsr);

	return OK;
}

struct hwspinlock *query_spinlock(u32 id)
{
	struct hwspinlock *p ;

	if(spinlock.hwspinlock_count >= spinlock.hwspinlock_num)
		return NULL;
	if(spinlock.ref[id] == 0) {
		p = &(spinlock.locklock[id]);
		p->reg =Spinlock_LockReg(id);
		p->id = id;
		spinlock.hwspinlock_count++;
		spinlock.ref[id]++;
		p->locked = 0;
	}else {
		p = &(spinlock.locklock[id]);
		spinlock.ref[id]++;
	}

	return p;
}

void release_spinlock(u32 id)
{
	struct hwspinlock *p ;

	spinlock.ref[id]--;
	if(spinlock.ref[id] == 0) {
		p = &(spinlock.locklock[id]);
		p->id = 0;
		spinlock.hwspinlock_count--;
		p->locked = 1;
	}
}

void spinlock_set_irqhandler(struct hwspinlock *lock,__pCBK_t pcb)
{
	lock->lock_handler = pcb;
}

s32 spinlock_suspend(arisc_device *dev)
{
	dev = dev;

	simsyscall_suspend();

	ccmu_reset_clk_mask(get_ccmu_device(),SPINLOCK_BUS_GATING_REG);

	return OK;
}

s32 spinlock_resume(arisc_device *dev)
{
	dev = dev;

	ccmu_reset_clk(get_ccmu_device(),SPINLOCK_BUS_GATING_REG);

	simsyscall_resume();

	return OK;
}

static int spinlock_ioctl(u32 request,va_list args)
{
	u32 spinlock_id = 0;
	struct hwspinlock *lock;
	u32 timeout;

	switch (request) {
		case REQUEST_SPINLOCK:
			spinlock_id = (u32)va_arg(args, unsigned int);
			return (u32)query_spinlock(spinlock_id);
		case RELEASE_SPINLOCK:
			spinlock_id = (u32)va_arg(args, unsigned int);
			release_spinlock(spinlock_id);
			break;
		case SPINLOCK_TRYLOCK_TIMEOUT:
			lock = (struct hwspinlock *)va_arg(args, unsigned int);
			timeout = (u32)va_arg(args, unsigned int);
			return hwspin_lock_timeout(lock,timeout);
		case SPINLOCK_UNLOCK:
			lock = (struct hwspinlock *)va_arg(args, unsigned int);
			return hwspin_unlock(lock);
		default:
			break;
	}

	return OK;
}

static s32 spinlock_handler(void *parg, u32 intno)
{
	struct hwspinlock_device *dev;
	int lock_id = 0;
	struct hwspinlock *lock;

	intno = intno;
	dev = (struct hwspinlock_device *)parg;

	for(lock_id = 0 ;lock_id < 32 ;lock_id++) {
		if(Spinlock_GetPending(lock_id) && Spinlock_GetIrqEnable(lock_id)) {
			lock = query_spinlock(lock_id);
			if(lock->lock_handler != NULL)
				lock->lock_handler(lock->data);
			else
				ERR("spinlock: not lock handler\n");
			release_spinlock(lock_id);
		}
	}
	return OK;
}


arisc_driver spinlock_dri = {
	.suspend = spinlock_suspend,
	.resume  = spinlock_resume,
	.iorequset = spinlock_ioctl,
};

s32 hwspinlock_init(void)
{
	spinlock.dev.hw_ver = 0x10000;
	spinlock.dev.reg_base = HWSPINLOCK_REG_BASE;
	spinlock.dev.dev_lock = 1;
	spinlock.dev.dev_id = HWSPINLOCK_DEVICE_ID;
	spinlock.dev.irq_no = INTC_R_SPINLOCK_IRQ;

	spinlock.dev.dri = &spinlock_dri;

	ccmu_reset_clk(get_ccmu_device(),SPINLOCK_BUS_GATING_REG);

	spinlock.hwspinlock_num = Spinlock_ReadLockNum();
	spinlock.dev.dev_lock = 0;
	install_isr(spinlock.dev.irq_no, spinlock_handler, (void *)(&spinlock));
	interrupt_enable(INTC_R_SPINLOCK_IRQ);

	list_add_tail(&spinlock.dev.list,&(dev_list));

	return OK;
}




