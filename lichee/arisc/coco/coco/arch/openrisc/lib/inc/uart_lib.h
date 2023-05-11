/*
*********************************************************************************************************
*                                                AR100 SYSTEM
*                                     AR100 Software System Develop Kits
*
*
*                                    (c) Copyright 2018-2024, Luojie China
*)                                             All Rights Reserved
*
* File    : uart_lib.h
* By      : LuoJie
* Version : v1.0
* Date    : 2018-04-11 17:15
* Descript: .
* Update  :auther      ver     notes
*          Luojie      1.0     Create this file.
*********************************************************************************************************
*/

#ifndef __UART_LIB_H__
#define __UART_LIB_H__

#include "lib_inc.h"

typedef volatile struct Uart_Configuration {
	u32 UART_BaudRate;
	u32 UART_WordLength;
	u32 UART_StopBit;
	u32 UART_Parity;
	u32 UART_HardwareFlowCtr;
	u32 UART_UartMode;
} UART_InitConfig;


typedef enum uart_ability
{
	UART_DISABLE   = 0x0,      //reset valid status
	UART_ENABLE  = 0x1,      //reset invalid status
} uart_ability_e;

typedef enum uart_clk_status
{
	UART_Assert  =	0,
	UART_Deassert = 1,
} uart_clk_status_e;

typedef enum uart_gating_status
{
	UART_GatingMask = 0,
	UART_GatingPass = 1,
} uart_gating_status_e;

extern struct R_UART __R_UART;

#define UART_PIN_NUM		2

#define UART0 __R_UART.UART[0]

/*
 * UART config
 */

#define BaudRate_115200		115200
#define BaudRate_9600		9600

#define WordLength_5		0
#define WordLength_6		1
#define WordLength_7		2
#define WordLength_8		3

#define StopBit_1		0
#define StopBit_1_5		1
#define StopBit_2		2

#define Parity_Odd		1
#define Parity_Even		2
#define Parity_Disable		0

#define FlowCtr_Auto		1
#define FlowCtr_Disable		0

#define UartMode_Uart		0
#define UartMode_IrDaSIR	1
#define UartMode_485		2
#define UartMode_LoopBack	3

#define DmaCtr_Mode0	0
#define DmaCtr_Mode1	1


#define IS_UART_BUSY(X)		(GET_UART0_STATUS >> 0) & 0x1
#define IS_TX_FIFO_NOT_FULL(X)	(GET_UART0_STATUS >> 1) & 0x1
#define IS_TX_FIFO_EMPTY(X)	(GET_UART0_STATUS >> 2) & 0x1
#define IS_RX_FIFO_NOT_EMPTY(X) (GET_UART0_STATUS >> 3) & 0x1
#define IS_RX_FIFO_FULL(X)	(GET_UART0_STATUS >> 4) & 0x1
/*
#define IS_UART_BUSY(X)                 X.status->bits.busy
#define IS_TX_FIFO_NOT_FULL(X)		X.status->bits.tfnf
#define IS_TX_FIFO_EMPTY(X)		X.status->bits.tfe
#define IS_RX_FIFO_NOT_EMPTY(X)		X.status->bits.rfne
#define IS_RX_FIFO_FULL(X)		X.status->bits.rff
*/

#define MODEM_DCD	0x80
#define MODEM_RI	0x40
#define MODEM_DSR	0x20
#define MODEM_CTS	0x10
#define MODEM_DDCD	0x8
#define MODEM_TERI	0x4
#define MODEM_DDSR	0x2
#define MODEM_DCTS	0x1
#define MODEM_ALL	0xFF

#define LINE_FIFOEER	0x80
#define LINE_TEMT	0x40
#define LINE_THRE	0x20
#define LINE_BI		0x10
#define	LINE_FE		0x8
#define LINE_PE		0x4
#define LINE_OE		0x2
#define LINE_DR		0x1
#define LINE_ALL	0xFF


#define ACTIVE_RBR	0x0
#define ARTIVE_DLL	0x1

#define CHAR_1_FIFO		0x0
#define FIFO_QUARTER_FULL	0x1
#define FIFO_HALF_FULL		0x2
#define FIFO_2_LESS		0x3

#define XFIFO_RESET	0x4
#define RFIFO_RESET	0x2
#define ENABLE_FIFO	0x1


extern void UART_DefInit(UART_InitConfig *config);
extern void UART_Config(UART_TypedDef *UARTx, UART_InitConfig *config);
extern void UART_Reg_Init(UART_TypedDef *UARTx, u32 reg_base);
extern s32 UART_SetBaudRate(UART_TypedDef *Uartx,u32 Rate);
extern u32 UART_GetBaudRate(UART_TypedDef *UARTx);
extern void UART_IRQ_Request(UART_TypedDef *Uartx, __pCBK_t phdle, void *parg);
extern s32 UART_SendData(UART_TypedDef *Uartx, u16 Data);
extern s32 UART_ReceiveData(UART_TypedDef *UARTx,char *buf);
extern u32 UART_ModemStatus(UART_TypedDef *UARTx, u32 modem);
extern u32 UART_LineStatus(UART_TypedDef *UARTx, u32 line);
extern void UART_IRQ_Config(UART_TypedDef *UARTx, u32 irq, u32 state);

#endif  //__UART_I_H__

