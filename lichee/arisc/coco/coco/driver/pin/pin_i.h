/*
 *********************************************************************************************************
 *                                                AR100 SYSTEM
 *                                     AR100 Software System Develop Kits
 *
 *
 *                                    (c) Copyright 2018-2024, Luojie China
 *)                                             All Rights Reserved
 *
 * File    : pin.h
 * By      : LuoJie
 * Version : v1.0
 * Date    : 2018-04-12 15:42
 * Descript: .
 * Update  :auther      ver     notes
 *          Luojie      1.0     Create this file.
 *********************************************************************************************************
 */

#ifndef __PIN_I_H__
#define __PIN_I_H__

#include <ibase.h>
#include <driver.h>


typedef struct {
	u32 irq_no;
	pin_typedef pin[PL_NUM];
}pl_port;

typedef struct {
	u32 irq_no;
	pin_typedef pin[PM_NUM];
}pm_port;


pl_port gpio_l_dev;
pm_port gpio_m_dev;

#endif  //__PIN_I_H__
