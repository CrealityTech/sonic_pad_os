#ifndef __LOOPBACK_H__
#define __LOOPBACK_H__

#ifdef CFG_LOOP_TASK
void loop_back_init(void);
#else
static inline void loop_back_init(void) {}
#endif

#endif

