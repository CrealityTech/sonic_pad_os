#ifndef __MEM_INCLUDE_H__
#define __MEM_INCLUDE_H__

#ifdef CFG_STANDBY_MEM
extern int mem_linux_save(void *arg);
extern int mem_linux_restore(void *arg);
#else
static inline int mem_linux_save(void *arg) { return -1; }
static inline int mem_linux_restore(void *arg) { return -1; }
#endif

#endif /* __MEM_INCLUDE_H__ */
