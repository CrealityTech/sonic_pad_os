/*
*********************************************************************************************************
*                                                AR100 SYSTEM
*                                     AR100 Software System Develop Kits
*                                                 twi module
*
*                                    (c) Copyright 2012-2016, Sunny China
*                                             All Rights Reserved
*
* File    : twi_i.h
* By      : Sunny
* Version : v1.0
* Date    : 2012-5-18
* Descript: twi internal header.
* Update  : date                auther      ver     notes
*           2012-5-18 10:21:06  Sunny       1.0     Create this file.
*********************************************************************************************************
*/

#ifndef __TWI_I_H__
#define __TWI_I_H__

#include "include.h"

typedef enum twi_rw_type {
	TWI_WRITE = 0x0,
	TWI_READ = 0x1,
} twi_rw_type_e;

typedef enum arisc_twi_bits_ops {
	TWI_CLR_BITS,
	TWI_SET_BITS
} arisc_twi_bits_ops_e;

#define TWI_CHECK_TIMEOUT       0x07ff

/* twi controller registers list */
#define TWI_BASE        (R_TWI_REG_BASE)
#define TWI_SAR         (TWI_BASE + 0x00)	/* 2-Wire Slave Address Register */
#define TWI_EAR         (TWI_BASE + 0x04)	/* 2-Wire Extend Address Register */
#define TWI_DTR         (TWI_BASE + 0x08)	/* 2-Wire Data Register */
#define TWI_CTR         (TWI_BASE + 0x0c)	/* 2-Wire Control Register */
#define TWI_STR         (TWI_BASE + 0x10)	/* 2-Wire Status Register */
#define TWI_CKR         (TWI_BASE + 0x14)	/* 2-Wire Clock Register */
#define TWI_SRR         (TWI_BASE + 0x18)	/* 2-Wire Soft-Reset Register */
#define TWI_EFR         (TWI_BASE + 0x1c)	/* 2-Wire Enhance Feature Register */
#define TWI_LCR         (TWI_BASE + 0x20)	/* 2-Wire line control register */
#define TWI_DVFSR       (TWI_BASE + 0x24)	/* 2-Wire DVFS control register */

/* control register bit field */
#define TWI_INT_ENB     (1 << 7)
#define TWI_BUS_ENB     (1 << 6)
#define TWI_MSTART      (1 << 5)
#define TWI_MSTOP       (1 << 4)
#define TWI_INTFLAG     (1 << 3)
#define TWI_AACK        (1 << 2)
#define TWI_LCR_WMASK   (TWI_MSTART | TWI_MSTOP | TWI_INTFLAG)

#endif /*__TWI_I_H__ */
