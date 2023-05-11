/*
*********************************************************************************************************
*                                                AR100 SYSTEM
*                                     AR100 Software System Develop Kits
*                                                pmu module
*
*                                    (c) Copyright 2012-2016, Sunny China
*                                             All Rights Reserved
*
* File    : pmu.h
* By      : Sunny
* Version : v1.0
* Date    : 2012-5-8
* Descript: power management unit module public header.
* Update  : date                auther      ver     notes
*           2012-5-8 8:52:39    Sunny       1.0     Create this file.
*********************************************************************************************************
*/

#ifndef __PMU_H__
#define __PMU_H__

#define NMI_INT_TYPE_PMU (0)
#define NMI_INT_TYPE_RTC (1)
#define NMI_INT_TYPE_PMU_OFFSET (0x1 << NMI_INT_TYPE_PMU)
#define NMI_INT_TYPE_RTC_OFFSET (0x1 << NMI_INT_TYPE_RTC)

#define PMU_STEP_DELAY_US               (16)
#define PMU_DCDC3_STEP_DELAY_US         (40)
#define AXP_TRANS_BYTE_MAX              (8)             /* the max number of pmu transfer byte */


/*
*********************************************************************************************************
*                                                PMU specific
*********************************************************************************************************
*/
/* aw1736 voltage type */
enum {
	AW1736_POWER_DCDC1 = 0x0,
	AW1736_POWER_DCDC2,
	AW1736_POWER_DCDC3,
	AW1736_POWER_DCDC4,
	AW1736_POWER_DCDC5,
	AW1736_POWER_DCDC6,
	AW1736_POWER_ALDO1,
	AW1736_POWER_ALDO2,
	AW1736_POWER_ALDO3,
	AW1736_POWER_ALDO4,
	AW1736_POWER_ALDO5,
	AW1736_POWER_BLDO1,
	AW1736_POWER_BLDO2,
	AW1736_POWER_BLDO3,
	AW1736_POWER_BLDO4,
	AW1736_POWER_BLDO5,
	AW1736_POWER_CLDO1,
	AW1736_POWER_CLDO2,
	AW1736_POWER_CLDO3,
	AW1736_POWER_CLDO4,
	AW1736_POWER_CPUSLDO,
	AW1736_POWER_LDOIO0,
	AW1736_POWER_LDOIO1,
	AW1736_POWER_DC1SW,
	AW1736_POWER_RTC,
	AW1736_POWER_MAX,
};

/* aw1669 voltage type */
enum {
	AW1669_POWER_DCDC1 = 0x0,
	AW1669_POWER_DCDC2,
	AW1669_POWER_DCDC3,
	AW1669_POWER_DCDC4,
	AW1669_POWER_DCDC5,
	AW1669_POWER_RTC,
	AW1669_POWER_ALDO1,
	AW1669_POWER_ALDO2,
	AW1669_POWER_ALDO3,
	AW1669_POWER_DLDO1,
	AW1669_POWER_DLDO2,
	AW1669_POWER_ELDO1,
	AW1669_POWER_ELDO2,
	AW1669_POWER_ELDO3,
	AW1669_POWER_LDOIO0,
	AW1669_POWER_LDOIO1,
	AW1669_POWER_SWOUT,
	AW1669_POWER_DC1SW,
	AW1669_POWER_DC5LDO,
	AW1669_POWER_MAX,
};

/* aw2101 voltage type */
enum {
	AXP2101_POWER_DCDC1 = 0x0,
	AXP2101_POWER_DCDC2,
	AXP2101_POWER_DCDC3,
	AXP2101_POWER_DCDC4,
	AXP2101_POWER_DCDC5,
	AXP2101_POWER_RTC1,
	AXP2101_POWER_ALDO1,
	AXP2101_POWER_ALDO2,
	AXP2101_POWER_ALDO3,
	AXP2101_POWER_ALDO4,
	AXP2101_POWER_BLDO1,
	AXP2101_POWER_BLDO2,
	AXP2101_POWER_CPUSLDO,
	AXP2101_POWER_DLDO1,
	AXP2101_POWER_DLDO2,
	AXP2101_POWER_MAX,
};


/*
*********************************************************************************************************
*                                                PMU common
*********************************************************************************************************
*/

typedef enum pmu_ioctl_type
{
	IOCTL_PMU_READ = 0x1,
	IOCTL_PMU_WRITE,
	IOCTL_PMU_DISABLE_IRQ,
	IOCTL_PMU_ENABLE_IRQ,
	IOCTL_PMU_CLR_IRQ_STATUS,
	IOCTL_PMU_SET_IRQ_TRIGGER,
	IOCTL_PMU_SET_VOLT,
	IOCTL_PMU_GET_VOLT,
	IOCTL_PMU_SET_VOLT_STA,
	IOCTL_PMU_GET_VOLT_STA,
	IOCTL_PMU_RESET,
	IOCTL_PMU_SHUTDOWN,
} pmu_ioctl_type_e;

typedef enum power_key_type
{
	POWER_KEY_SHORT = 1,
	POWER_KEY_LONG  = 2,
	POWER_LOW_POWER = 3,
} power_key_type_e;

typedef enum power_voltage_state
{
	POWER_VOL_OFF = 0x0,
	POWER_VOL_ON  = 0x1,
} power_voltage_state_e;

typedef enum power_mode
{
	POWER_MODE_AXP = 0,
	POWER_MODE_DUMMY = 1,
	POWER_MODE_PWM = 2,
} power_mode_e;

//keep the struct word align
//by superm at 2014-2-13 15:34:09
typedef struct pmu_onoff_reg_bitmap
{
	u16 devaddr;
	u16 regaddr;
	u16 offset;
	u8  state;
	u8  dvm_st;
} pmu_onoff_reg_bitmap_t;
extern pmu_onoff_reg_bitmap_t pmu_onoff_reg_bitmap[];

typedef struct pmu_paras
{
	u8 *devaddr;
	u8 *regaddr;
	u8 *data;
	u32 len;
} pmu_paras_t;


#ifdef CFG_PMU_USED
extern s32 pmu_init(void);
extern s32 pmu_exit(void);
#else
#define pmu_init()     (0)
#define pmu_exit()     (0)
#endif

#ifdef CFG_SUN8IW15P1
extern s32 bmu_init(void);
extern s32 bmu_exit(void);
extern s32 pmu_set_boost(u32 state);
#endif

void pmu_shutdown(void);

void pmu_reset(void);

s32 pmu_set_voltage(u32 type, u32 voltage); //the voltage base on mV
s32 pmu_get_voltage(u32 type);
s32 pmu_set_voltage_state(u32 type, u32 state);

//register read and write
s32 pmu_reg_read(u8 *devaddr, u8 *regaddr, u8 *data, u32 len);
s32 pmu_reg_write(u8 *devaddr, u8 *regaddr, u8 *data, u32 len);

//s32 pmu_get_chip_id(struct message *pmessage);
s32 pmu_get_chip_id(u32 *chip_id);

//pmu standby process
s32 pmu_standby_init(u32 event, u32 gpio_bitmap);
s32 pmu_standby_exit(void);
s32 pmu_query_event(u32 *event);
int nmi_int_handler(void *parg, u32 intno);
int get_nmi_int_type(void);
u32 pmu_output_is_stable(u32 type);
s32 pmu_get_voltage_state(u32 type);
//s32 pmu_set_paras(struct message *pmessage);
s32 pmu_set_paras(u32 *paras);
//s32 set_pwr_tree(struct message *pmessage);
s32 set_pwr_tree(u32 *paras);

#ifdef CFG_PMU_CHRCUR_CRTL_USED
void pmu_contrl_batchrcur(void);
#endif

s32 pmu_get_batconsum(void);
u32 pmu_get_powerstate(u32 power_reg);

extern s32 pmu_reg_write_para(pmu_paras_t *para);
extern s32 pmu_reg_read_para(pmu_paras_t *para);

extern void pmu_sysconfig_cfg(void);
extern void pmu_set_lowpcons(void);
extern void pmu_sys_lowpcons(void);
extern s32 pmu_set_pok_time(int time);
extern void pmu_poweroff_system(void);
extern void pmu_reset_system(void);
extern int pmu_set_gpio(unsigned int id, unsigned int state);
extern s32 pmu_clear_pendings(void);
extern bool pmu_pin_detect(void);
extern u32 is_pmu_exist(void);
extern void watchdog_reset(void);
#endif  //__PMU_H__
