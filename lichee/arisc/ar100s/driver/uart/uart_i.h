/*
*********************************************************************************************************
*                                                AR100 SYSTEM
*                                     AR100 Software System Develop Kits
*                                                uart module
*
*                                    (c) Copyright 2012-2016, Sunny China
*                                             All Rights Reserved
*
* File    : uart_i.h
* By      : Sunny
* Version : v1.0
* Date    : 2012-5-4
* Descript: uart module internal header.
* Update  : date                auther      ver     notes
*           2012-5-4 9:34:40    Sunny       1.0     Create this file.
*********************************************************************************************************
*/

#ifndef __UART_I_H__
#define __UART_I_H__

#include "include.h"

/* uart regiters list */
#define UART_REG_ADDR   (R_UART_REG_BASE)
#define UART_REG_RBR    (UART_REG_ADDR + 0x00)
#define UART_REG_THR    (UART_REG_ADDR + 0x00)
#define UART_REG_DLL    (UART_REG_ADDR + 0x00)
#define UART_REG_DLH    (UART_REG_ADDR + 0x04)
#define UART_REG_IER    (UART_REG_ADDR + 0x04)
#define UART_REG_IIR    (UART_REG_ADDR + 0x08)
#define UART_REG_FCR    (UART_REG_ADDR + 0x08)
#define UART_REG_LCR    (UART_REG_ADDR + 0x0c)
#define UART_REG_MCR    (UART_REG_ADDR + 0x10)
#define UART_REG_LSR    (UART_REG_ADDR + 0x14)
#define UART_REG_MSR    (UART_REG_ADDR + 0x18)
#define UART_REG_SCH    (UART_REG_ADDR + 0x1c)
#define UART_REG_USR    (UART_REG_ADDR + 0x7c)
#define UART_REG_TFL    (UART_REG_ADDR + 0x80)
#define UART_REG_RFL    (UART_REG_ADDR + 0x84)
#define UART_REG_HALT   (UART_REG_ADDR + 0xa4)
#endif  /*__UART_I_H__ */
