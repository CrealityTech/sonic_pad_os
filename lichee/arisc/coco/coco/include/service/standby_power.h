#ifndef __STANDBY_POWER_H__
#define __STANDBY_POWER_H__

#ifdef CFG_STANDBY_POWER
void standby_powerdown(void);
void standby_powerup(void);
#else
static inline void standby_powerdown(void) {}
static inline void standby_powerup(void) {}
#endif

#endif

