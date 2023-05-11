/*
*********************************************************************************************************
*                                                AR100 SYSTEM
*                                     AR100 Software System Develop Kits
*                                                 twi module
*
*                                    (c) Copyright 2012-2016, Sunny China
*                                             All Rights Reserved
*
* File    : twi.h
* By      : Sunny
* Version : v1.0
* Date    : 2012-5-18
* Descript: twi controller public header.
* Update  : date                auther      ver     notes
*           2012-5-18 9:40:41   Sunny       1.0     Create this file.
*********************************************************************************************************
*/

#ifndef __TWI_H__
#define __TWI_H__

/*
*********************************************************************************************************
*                                       INITIALIZE TWI
*
* Description:  initialize twi module.
*
* Arguments  :  none.
*
* Returns    :  OK if initialize twi succeeded, others if failed.
*********************************************************************************************************
*/
s32 twi_init(void);

/*
*********************************************************************************************************
*                                           EXIT TWI
*
* Description:  exit twi module.
*
* Arguments  :  none.
*
* Returns    :  OK if exit twi succeeded, others if failed.
*********************************************************************************************************
*/
s32 twi_exit(void);

/*
*********************************************************************************************************
*                                           READ DATA BY TWI
*
* Description:  read data by twi.
*
* Arguments  :  addr    : the point of address, size is specified by len.
*               data    : the point of data, size is specified by len.
*               len     : the length of data.
*
* Returns    :  OK if read data succeeded, others if failed.
*********************************************************************************************************
*/
s32 twi_read(u32 devaddr, u8 *addr, u8 *data, u32 len);

/*
*********************************************************************************************************
*                                           WRITE DATA BY TWI
*
* Description:  write data by twi.
*
* Arguments  :  addr    : the point of address, size is specified by len.
*               data    : the point of data, size is specified by len.
*               len     : the length of data.
*
* Returns    :  OK if write data succeeded, others if failed.
*********************************************************************************************************
*/
s32 twi_write(u32 devaddr, u8 *addr, u8 *data, u32 len);

bool is_twi_lock(void);

extern int twi_send_clk_9pulse(void);
extern s32 twi_get_status(void);
extern s32 twi_clkchangecb(u32 command, u32 freq);
s32 twi_standby_init(void);
s32 twi_standby_exit(void);

#endif /* __TWI_H__ */
