/*
 * include/cfgs.h
 *
 * Descript: system configure header.
 * Copyright (C) 2012-2016 AllWinnertech Ltd.
 * Author: superm <superm@allwinnertech.com>
 *
 */
#ifndef __PLATFORM_CFGS_H__
#define __PLATFORM_CFGS_H__

#define ARISC_USE_DRAM_CODE_PBASE       (0x48100000)
#define ARISC_USE_DRAM_CODE_SIZE        (0x00003000)
#define ARISC_USE_DRAM_DATA_PBASE       (ARISC_USE_DRAM_CODE_PBASE + ARISC_USE_DRAM_CODE_SIZE)
#define ARISC_USE_DRAM_DATA_SIZE        (0x00002000)
#define ARISC_PARA_ADDR                 (0x48105000)

#define DDR2_FPGA_S2C_16B

#define IR_CHECK_ADDR_CODE

#define AXP_TRANS_BYTE_MAX              (8)	/* the max number of pmu transfer byte */
#define PMU_DCDC3_STEP_DELAY_US         (40)	/* reference from pmu spec */

#define TWI_CLOCK_FREQ                  (200 * 1000)	/* the twi source clock freq */
#define TWI_SCK_FREQ                  (400000)	//the p2wi SCK freq(2M)
#define TWI_SDAODLY                   (1)	//SDA output delay
#define TWI_TRANS_BYTE_MAX            (8)	//the max number of P2WI transfer byte

#define RSB_RTSADDR_AW1660		(0x34)
#define RSB_RTSADDR_AW1736		(0x36)
#define RSB_RTSADDR_AXP2202		(0x34)
#define RSB_RTSADDR_AW1657		(0x36)

/* system time tick, 100 ticks/second */
#define TICK_PER_SEC                    (100)

#define RSB_SCK_FREQ                    (3000000)	/* the rsb SCK freq(3M) */
#define RSB_SDAODLY                     (1)	/* SDA output delay */

/* config pmu communication mode */
#define RSB_USED                (0)	/* config here */

/* uart config */
#define UART_BAUDRATE                   (115200 / 2)

/* services define */

/* devices define */
#define RC5_USED		(1)

#define ARISC_DTS_BASE (0x48100000)
#define ARISC_DTS_SIZE (0x00100000)

#endif /* __SUN50IW2_CFGS_H__ */
