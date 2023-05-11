#ifndef __HWSPINLOCK_LIB_H__
#define __HWSPINLOCK_LIB_H__


#include "lib_inc.h"

#define SPINLOCK_SYSTATUS_REG		0x00
#define SPINLOCK_STATUS_REG		0x10
#define SPINLOCK_VER_REG		0x60
#define SPINLOCK_LOCK_BASE_REG		0x100


#define MAX_SPINLOCK_NUM	32


#define SPINLOCK_TAKEN		1
#define SPINLOCK_FREE		0


u32 Spinlock_LockReg(u32 lock_id);
u32 Spinlock_ReadLock(u32 reg);
void Spinlock_WriteLock(u32 reg,u32 value);
u32 Spinlock_ReadLockNum(void);
u32 Spinlock_ReadStatus(void);








#endif


