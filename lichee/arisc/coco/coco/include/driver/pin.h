#ifndef __PIN_H__
#define __PIN_H__

#include "ibase.h"
#include "driver.h"

#define PIN_GRP_PL  (0x1)
#define PIN_GRP_PM  (0x2)

#define GPIO_GP_PL	PIN_GRP_PL
#define GPIO_GP_PM	PIN_GRP_PM

#define GPIO_PIN_0	0
#define GPIO_PIN_1	1
#define GPIO_PIN_2	2
#define GPIO_PIN_3	3
#define GPIO_PIN_4	4
#define GPIO_PIN_5	5
#define GPIO_PIN_6	6
#define GPIO_PIN_7	7
#define GPIO_PIN_8	8
#define GPIO_PIN_9	9
#define GPIO_PIN_10	10
#define GPIO_PIN_11	11
#define GPIO_PIN_12	12
#define GPIO_PIN_13	13
#define GPIO_PIN_14	14
#define GPIO_PIN_15	15
#define GPIO_PIN_16	16
#define GPIO_PIN_17	17
#define GPIO_PIN_18	18
#define GPIO_PIN_19	19
#define GPIO_PIN_20	20
#define GPIO_PIN_21	21
#define GPIO_PIN_22	22
#define GPIO_PIN_23	23
#define GPIO_PIN_24	24
#define GPIO_PIN_25	25
#define GPIO_PIN_26	26
#define GPIO_PIN_27	27
#define GPIO_PIN_28	28
#define GPIO_PIN_29	29
#define GPIO_PIN_30	30
#define GPIO_PIN_31	31

#define GPIO_FN_0	0
#define GPIO_FN_1	1
#define GPIO_FN_2	2
#define GPIO_FN_3	3
#define GPIO_FN_4	4
#define GPIO_FN_5	5
#define GPIO_FN_6	6
#define GPIO_FN_7	7

#define GPIO_EXTI_DISABLE	0
#define GPIO_EXTI_ENABLE	1

#define GPIO_CLK_HOSC		1
#define GPIO_CLK_LOSC		0

#define CPUS_GPIO_PL(num)	(1 << num)
#define CPUS_GPIO_PM(num)	(1 << (num + 12))


#define GPIO_EXTI_POSITIVE_EDGE  0x0
#define GPIO_EXTI_NEGATIVE_EDGE  0x1
#define GPIO_EXTI_HIGN_LEVEL     0x2
#define GPIO_EXTI_LOW_LEVEL      0x3
#define GPIO_EXTI_DOUBLE_EDGE    0x4


typedef struct {
	u8 pin_grp;
	u8 pin_num;
	u8 pin_fn;
	u8 pin_driving;
	u8 pin_pull;
	u8 pin_exti_config;
	u8 pin_exti_ctrl;
	u8 pin_exti_pre;
	u8 pin_exti_clk;
}pin_config;

typedef struct {
	pin_config init_config;
	u8 pin_suspend;
	u8 pin_used;
	u8 pin_data;
	u8 pin_pending;
	__pISR_t exti_handler;
	void *owner;
}pin_typedef;
//------------------------------------------------------------------------------
//  pin pull status
//------------------------------------------------------------------------------
typedef enum pin_pull
{
	GPIO_PULL_DISABLE    =   0x00,
	GPIO_PULL_UP	    =   0x01,
	GPIO_PULL_DOWN       =   0x02,
	GPIO_PULL_RESERVED   =   0x03,
	GPIO_PULL_DEFAULT    =   0xFF
} pin_pull_e;

//------------------------------------------------------------------------------
//  pin multi drive level
//------------------------------------------------------------------------------
typedef enum pin_multi_drive
{
	GPIO_MULTI_DRIVE_0       =   0x00,
	GPIO_MULTI_DRIVE_1       =   0x01,
}pin_multi_drive_e;



s32 pin_set(pin_config *config,pin_typedef **owner);
void pin_disable(pin_typedef *pin);
void pin_exti_irq_request(pin_typedef *pin, __pISR_t handler);
void pin_data_write(pin_typedef *pin,u32 data);
u32 pin_data_read(pin_typedef *pin);
u32 is_wanted_gpio_int(u32 pin_grp, u32 gpio_int_en);
u32 is_cpux_gpio_pending(void);
u32 pin_query_pending(pin_typedef *pin);
u32 pin_clear_pending(pin_typedef *pin);
s32 pin_enable_int(pin_typedef *pin);
s32 pin_disable_int(pin_typedef *pin);
s32 pl_irq_handler(void *p_arg,u32 intno);
s32 pm_irq_handler(void *p_arg,u32 intno);

s32 pin_wakeup_init(void *parg);
s32 pin_wakeup_exit(void *parg);
s32 default_pin_wakeup_init(void *parg);
s32 default_pin_wakeup_exit(void *parg);
s32 pinl_init(void);
s32 pinm_init(void);

s32 pin_suspend(arisc_device *dev);
s32 pin_resume(arisc_device *dev);

void pin_exit(void);
typedef struct {
	arisc_device dev;
	arisc_driver *dri;
}pin_device;

#endif

