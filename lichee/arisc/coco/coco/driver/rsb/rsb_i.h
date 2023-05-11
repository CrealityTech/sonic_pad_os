/*
*********************************************************************************************************
*                                                AR100 SYSTEM
*                                     AR100 Software System Develop Kits
*                                                 rsb module
*
*                                    (c) Copyright 2012-2016, Sunny China
*                                             All Rights Reserved
*
* File    : rsb_i.h
* By      : Sunny
* Version : v1.0
* Date    : 2012-5-22
* Descript: rsb internal header.
* Update  : date                auther      ver     notes
*           2012-5-22 9:46:18   Sunny       1.0     Create this file.
*********************************************************************************************************
*/

#ifndef __RSB_I_H__
#define __RSB_I_H__

#include "ibase.h"
#include "driver.h"

struct rsb_ops {
	s32 (*read_block)(rsb_device *dev, rsb_paras_t *paras);
	s32 (*write_block)(rsb_device *dev, rsb_paras_t *paras);
	s32 (*set_rtsa)(rsb_device *dev, u32 saddr ,u32 rtsaddr);
};

/* rsb transfer data type */
typedef enum arisc_rsb_bits_ops {
	RSB_CLR_BITS,
	RSB_SET_BITS
} arisc_rsb_bits_ops_e;


/* RSB SHIFT */
#define RSB_RTSADDR_SHIFT       (16)//runtime slave address shift
#define RSB_SADDR_SHIFT         (0)//Slave Address shift

/* RSB command */
#define RSB_CMD_BYTE_WRITE      (0x4E)//(0x27)//Byte write
#define RSB_CMD_HWORD_WRITE     (0x59)//(0x2c)//Half word write
#define RSB_CMD_WORD_WRITE      (0x63)//(0x31)//Word write
#define RSB_CMD_BYTE_READ       (0x8B)//(0x45)//Byte read
#define RSB_CMD_HWORD_READ      (0x9C)//(0x4e)//Half word read
#define RSB_CMD_WORD_READ       (0xA6)//(0x53)//Word read
#define RSB_CMD_SET_RTSADDR     (0xE8)//(0x74)//Set Run-time Address

//rsb control bit field
#define RSB_SOFT_RST            (1U << 0)
#define RSB_GLB_INTEN           (1U << 1)
#define RSB_ABT_TRANS           (1U << 6)
#define RSB_START_TRANS         (1U << 7)

//rsb state bit field
#define RSB_TOVER_INT           (1U << 0)
#define RSB_TERR_INT            (1U << 1)
#define RSB_LBSY_INT            (1U << 2)

//rsb or twi pmu mode bit field
#define RSB_PMU_INIT            (1U << 31)
#define NTWI_PMU_INIT           (0U << 31)

#endif  //__RSB_I_H__
