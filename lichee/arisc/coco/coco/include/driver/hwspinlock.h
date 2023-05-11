#ifndef __HWSPINLOCK_H__
#define __HWSPINLOCK_H__


struct hwspinlock {
	u32 reg;
	u32 locked;
	u32 cpsr;
	u32 id;
	void *data;
	s32 (*lock_handler)(void *parg);
};


#define REQUEST_SPINLOCK		0x0001
#define SPINLOCK_TRYLOCK_TIMEOUT	0x0002
#define SPINLOCK_UNLOCK			0x0003
#define RELEASE_SPINLOCK		0x0004

#define MSG_SPINLOCK 0
#define MSGTABLE_SPINLOCK 1
#define SIMSYSCLL_SPINLOCK 15

s32 hwspin_lock_timeout(struct hwspinlock *lock, u32 timeout);
s32 hwspin_unlock(struct hwspinlock *lock);
struct hwspinlock *query_spinlock(u32 id);
void release_spinlock(u32 id);
void spinlock_set_irqhandler(struct hwspinlock *lock,__pCBK_t pcb);
s32 spinlock_suspend(arisc_device *dev);
s32 spinlock_resume(arisc_device *dev);
s32 hwspinlock_init(void);

#endif

