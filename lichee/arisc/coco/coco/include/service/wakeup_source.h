#ifndef __WAKEUP_SOURCE_H__
#define __WAKEUP_SOURCE_H__

#define WAKESOURE_IRQ_HANDLER	0
#define WAKESOURE_LOOP		1

#define NO_WAKESOURCE		0x0

/* wakeup source 'levels' field */
#define WAKESOURE_ROOT		(0x0 << 30)
#define WAKESOURE_SECOND	(0x1 << 30)
#define WAKESOURE_THIRD		(0x2 << 30)
#define WAKESOURE_TIMER		(0x3 << 30)

/* wakeup source 'root irq number' field
* just the same as r_intc's interrupt source macro in irqnum_config.h,
* no need to add another one here.
*/

/* wakeup source 'secondary irq number' field */
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

enum wakeup_source {
	WAKESOURCE_LOOP_AXP,
	WAKESOURCE_LOOP_PL,
	WAKESOURCE_LOOP_USBMOUCE,

	WAKESOURCE_LOOP_MAX,
};


struct wakeup_loop_handle {
	__pCBK_t handle[WAKESOURCE_LOOP_MAX];
	void *parg[WAKESOURCE_LOOP_MAX];
};


int loop_wakesource_init(u32 loop_num,__pCBK_t handler, void *parg, u32 irq_num);
int loop_wakesource_exit(u32 irq_num);

//int irq_wakesource_init(u32 irq_num,__pISR_t handler, void *parg);
//int irq_wakesource_exit(u32 irq_num);
int wakesource_loop(void);

#endif

