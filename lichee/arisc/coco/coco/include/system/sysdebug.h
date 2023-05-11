#ifndef __SYSDEBUG_H__
#define __SYSDEBUG_H__

#include <sys/errno.h>

#ifdef CFG_SYSTEM_DEBUG
int sysdebug_init(void);
#else
static inline int sysdebug_init(void)
{
	return -ENOSYS;
}
#endif

#endif

