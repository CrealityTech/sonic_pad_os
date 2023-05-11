#ifndef __WAKEUP_SOURCE_H__
#define __WAKEUP_SOURCE_H__

#define WAKESOURE_IRQ_HANDLER   0
#define WAKESOURE_LOOP          1

#define NO_WAKESOURCE           0x0

/* wakeup source 'levels' field */
#define WAKESOURE_ROOT          (0x0 << 30)
#define WAKESOURE_SECOND        (0x1 << 30)
#define WAKESOURE_THIRD         (0x2 << 30)
#define WAKESOURE_TIMER         (0x3 << 30)

/*
 * wakeup source 'root irq number' field
 * just the same as r_intc's interrupt source macro in irqnum_config.h,
 * no need to add another one here.
 */

/* wakeup source 'secondary irq number' field */
#if 0
#define CPUS_WAKEUP_LOWBATT     (0 << 10)  /* pmu wakeup source start */
#define CPUS_WAKEUP_USB         (1 << 10)
#define CPUS_WAKEUP_AC          (2 << 10)
#define CPUS_WAKEUP_ASCEND      (3 << 10)
#define CPUS_WAKEUP_DESCEND     (4 << 10)
#define CPUS_WAKEUP_SHORT_KEY   (5 << 10)
#define CPUS_WAKEUP_LONG_KEY    (6 << 10)
#define CPUS_WAKEUP_BAT_TEMP    (7 << 10)
#define CPUS_WAKEUP_FULLBATT    (8 << 10)
#define CPUS_WAKEUP_GPIO        (9 << 10)  /* pmu wakeup source end */
#endif

enum wakeup_source {
	WAKESOURCE_LOOP_AXP,
	WAKESOURCE_LOOP_PL,
	WAKESOURCE_LOOP_USBMOUCE,

	WAKESOURCE_LOOP_MAX,
};

#ifdef CFG_STANDBY_SERVICE
s32 wakeup_timer_init(void);
s32 set_wakeup_src(struct message *pmessage);
s32 clear_wakeup_src(struct message *pmessage);
void wakeup_timer_start(void);
void wakeup_timer_stop(void);
#else
static inline s32 wakeup_timer_init(void) { return -1; }
static inline s32 set_wakeup_src(struct message *pmessage) { return -1; }
static inline s32 clear_wakeup_src(struct message *pmessage) { return -1; }
static inline void wakeup_timer_start(void) { return; }
static inline void  wakeup_timer_stop(void) { return; }
#endif

#endif

