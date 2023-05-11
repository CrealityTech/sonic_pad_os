/*
*********************************************************************************************************
*                                                AR100 SYSTEM
*                                     AR100 Software System Develop Kits
*                                                 pin module
*
*                                    (c) Copyright 2012-2016, Sunny China
*                                             All Rights Reserved
*
* File    : pin.h
* By      : Sunny
* Version : v1.0
* Date    : 2012-5-4
* Descript: pin management module public header.
* Update  : date                auther      ver     notes
*           2012-5-4 13:57:15   Sunny       1.0     Create this file.
*********************************************************************************************************
*/

#ifndef __PIN_H__
#define __PIN_H__
/*
------------------------------------------------------------------------------
pin groups define
------------------------------------------------------------------------------
*/
#define PIN_GRP_PL (0x1)
#define PIN_GRP_PM (0x2)

/*
------------------------------------------------------------------------------
pin pull status
------------------------------------------------------------------------------
*/
typedef enum pin_pull {
	PIN_PULL_DISABLE    =   0x00,
	PIN_PULL_UP     =   0x01,
	PIN_PULL_DOWN       =   0x02,
	PIN_PULL_RESERVED   =   0x03,
	PIN_PULL_DEFAULT    =   0xFF
} pin_pull_e;

/*
------------------------------------------------------------------------------
pin multi drive level
------------------------------------------------------------------------------
*/
typedef enum pin_multi_drive {
	PIN_MULTI_DRIVE_0       =   0x00,
	PIN_MULTI_DRIVE_1       =   0x01,
	PIN_MULTI_DRIVE_2       =   0x02,
	PIN_MULTI_DRIVE_3       =   0x03,
	PIN_MULTI_DRIVE_DEFAULT     =   0xFF
} pin_multi_drive_e;

/*
------------------------------------------------------------------------------
pin data bit status
------------------------------------------------------------------------------
*/
typedef enum pin_data_status {
	PIN_DATA_LOW     = 0x00,
	PIN_DATA_HIGH    = 0x01,
	PIN_DATA_DEFAULT = 0XFF
} pin_data_status_e;

/*
------------------------------------------------------------------------------
pin multi select functions
------------------------------------------------------------------------------
*/
typedef enum pin_multi_select {
	PIN_MULTI_SEL_INPUT  = 0x0,
	PIN_MULTI_SEL_OUTPUT = 0x1,
} pin_multi_select_t;

/*
------------------------------------------------------------------------------
pin int trigger mode
------------------------------------------------------------------------------
*/
typedef enum pin_int_trigger_mode {
	PIN_INT_POSITIVE_EDGE   =   0x0,
	PIN_INT_NEGATIVE_EDGE   =   0x1,
	PIN_INT_HIGN_LEVEL      =   0x2,
	PIN_INT_LOW_LEVEL       =   0x3,
	PIN_INT_DOUBLE_EDGE     =   0x4
} pin_int_trigger_mode_e;

/*
------------------------------------------------------------------------------
pin status
------------------------------------------------------------------------------
*/
typedef struct pin_status {
	u32 multi_sel;
	u32 pull;
	u32 driving;
	u32 data;
} pin_status_t;

/*
------------------------------------------------------------------------------
PIN management module interfaces
------------------------------------------------------------------------------
*/
s32 pin_init(void);
s32 pin_exit(void);
s32 pin_set_status(u32 pin_grp, u32 pin_num, struct pin_status *pstatus);
s32 pin_set_multi_sel(u32 pin_grp, u32 pin_num, u32 multi_sel);
s32 pin_set_pull(u32 pin_grp, u32 pin_num, u32 pull);
s32 pin_set_drive(u32 pin_grp, u32 pin_num, u32 drive);
s32 pin_write_data(u32 pin_grp, u32 pin_num, u32 data);
u32 pin_read_data(u32 pin_grp, u32 pin_num);

s32 pin_enable_int(u32 pin_grp, u32 pin_num);
s32 pin_disable_int(u32 pin_grp, u32 pin_num);
u32 pin_query_pending(u32 pin_grp, u32 pin_num);
s32 pin_clear_pending(u32 pin_grp, u32 pin_num);
s32 pin_standby_init(u32 gpio_enable_bitmap);
s32 pin_standby_exit(void);
extern u32 is_wanted_gpio_int(u32 pin_grp, u32 gpio_int_en);

#endif /* __PIN_H__ */
