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


#define TWI_CLOCK_FREQ                  (200 * 1000)	/* the twi source clock freq */
#define TICK_PER_SEC                    (100)

/* uart config */
#define UART_BAUDRATE                   (115200 / 2)


/* devices define */
#define ARISC_DTS_BASE (0x44000000)
#define ARISC_DTS_SIZE (0x00100000)

#endif /* __SUN50IW2_CFGS_H__ */
