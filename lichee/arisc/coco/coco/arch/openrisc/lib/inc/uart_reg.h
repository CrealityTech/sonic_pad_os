/*
 *********************************************************************************************************
 *                                                AR100 SYSTEM
 *                                     AR100 Software System Develop Kits
 *
 *
 *                                    (c) Copyright 2018-2024, Luojie China
 *)                                             All Rights Reserved
 *
 * File    : uart_reg.h
 * By      : LuoJie
 * Version : v1.0
 * Date    : 2018-04-13 11:22
 * Descript: .
 * Update  :auther      ver     notes
 *          Luojie      1.0     Create this file.
 *********************************************************************************************************
 */
#ifndef __UART_REG_H__
#define __UART_REG_H__

#include "lib_inc.h"

#define R_UART_NUM		1

#define UART_NUM(x) x > R_UART_NUM ? 1 : 0

#define UART_REG_BASE   (R_UART_REG_BASE)

#define R_UART_RBR	 0x0000
#define R_UART_THR	 0x0000
#define R_UART_DLL	 0x0000
#define R_UART_DLH	 0x0004
#define R_UART_IER	 0x0004
#define R_UART_IIR	 0x0008
#define R_UART_FCR	 0x0008
#define R_UART_LCR	 0x000c
#define R_UART_MCR	 0x0010
#define R_UART_LSR	 0x0014
#define R_UART_MSR	0x0018
#define R_UART_SCH	 0x001c
#define R_UART_USR	 0x007c
#define R_UART_TFL	 0x0080
#define R_UART_RFL	 0x0084
#define R_UART_HSK	 0x0088
#define R_UART_HALT	 0x00a4
#define R_UART_DBG_DLL	 0x00b0
#define R_UART_DBG_DLH	 0x00b4
#define R_UART_485_CTL	 0x00c0
#define R_UART_485_ADDR	 0x00c4
#define R_UART_BUS_IDLE	 0x00c8
#define R_UART_TX_DLY	 0x00cc
#define R_UART_VER	 0x00d0

#define GET_UART0_STATUS	readl(UART_REG_BASE + R_UART_USR)

typedef struct {
	u32 reg;
}uart_rbr_t;

typedef struct {
	u32 reg;
}uart_thr_t;

typedef struct {
	u32 reg;
}uart_dll_t;

typedef struct {
	u32 reg;
}uart_dlh_t;

typedef union {
	u32 reg;
	struct {
		u32 reserved1:24;
		u32 ptime:1;
		u32 reserved0:2;
		u32 rs485_int_en:1;
		u32 edssi:1;
		u32 elsi:1;
		u32 etbei:1;
		u32 erbfi:1;
	}bits;
}uart_ier_t;

typedef union {
	u32 reg;
	struct {
		u32 reserved:24;
		u32 feflag:2;
		u32 reserved0:2;
		u32 iid:4;
	}bits;
}uart_iir_t;

typedef union {
	u32 reg;
	struct {
		u32 reserved:24;
		u32 rt:2;
		u32 tft:2;
		u32 dmam:1;
		u32 xfifor:1;
		u32 rfifoe:1;
		u32 fifoe:1;
	}bits;
}uart_fcr_t;

typedef union {
	u32 reg;
	struct {
		u32 reserved:24;
		u32 dlab:1;
		u32 bc:1;
		u32 eps:2;
		u32 pen:1;
		u32 stop:1;
		u32 dls:2;
	}bits;
}uart_lcr_t;

typedef union {
	u32 reg;
	struct {
		u32 reserved1:24;
		u32 uart_function:2;
		u32 afce:1;
		u32 loop:1;
		u32 reserved0:2;
		u32 rts:1;
		u32 dtr:1;
	}bits;
}uart_mcr_t;

typedef union {
	u32 reg;
	struct {
		u32 reserved:24;
		u32 fifoerr:1;
		u32 temt:1;
		u32 thre:1;
		u32 bi:1;
		u32 fe:1;
		u32 pe:1;
		u32 oe:1;
		u32 dr:1;
	}bits;
}uart_lsr_t;

typedef union {
	u32 reg;
	struct {
		u32 reserved1:24;
		u32 dcd:1;
		u32 ri:1;
		u32 dsr:1;
		u32 cts:1;
		u32 ddcd:1;
		u32 teri:1;
		u32 ddsr:1;
		u32 dcts:1;
	}bits;
}uart_msr_t;

typedef struct {
	u32 reg;
}uart_sch_t;

typedef union {
	u32 reg;
	struct {
		u32 reserved1:27;
		u32 rff:1;
		u32 rfne:1;
		u32 tfe:1;
		u32 tfnf:1;
		u32 busy:1;
	}bits;
}uart_usr_t;

typedef struct {
	u32 reg;
}uart_tfl_t;

typedef struct {
	u32 reg;
}uart_rfl_t;

typedef struct {
	u32 reg;
}uart_hsk_t;

typedef union {
	u32 reg;
	struct {
		u32 reserved1:24;
		u32 pte:1;
		u32 dma_pte_rx:1;
		u32 sir_rx_intert:1;
		u32 sir_tx_invert:1;
		u32 reserved0:1;
		u32 change_update:1;
		u32 chcfg_at_busy:1;
		u32 halt_tx:1;
	}bits;
}uart_halt_t;

typedef struct {
	u32 reg;
}uart_dbg_dll_t;

typedef struct {
	u32 reg;
}uart_dbg_dlh_t;

typedef union {
	u32 reg;
	struct {
		u32 reserved1:25;
		u32 aad_addr_f:1;
		u32 rs485_addr_det_f:1;
		u32 reserved0:1;
		u32 rx_bf_addr:1;
		u32 rx_af_addr:1;
		u32 rs485_slave_mode_sel:2;
	}bits;
}uart_485_ctrl_t;

typedef struct {
	u32 reg;
}uart_485_addr_t;

typedef union {
	u32 reg;
	struct {
		u32 reserved1:24;
		u32 bus_idle_chk_en:1;
		u32 bus_status:1;
		u32 adj_time0:6;
	}bits;
}uart_485_bus_idle_chk_t;

typedef struct {
	u32 reg;
}uart_tx_delay_t;

typedef struct {
	u32 reg;
}uart_ver_t;

// 这个typedef 非常重要，UART_typedef 前必需用volatile修饰，不然某些情况下
// 编译器会将对结构体成员函数的赋值操作优化掉 :
//	UARTx->line_ctrl->bits.dlab = 1;
//	UARTx->div_latch_low->reg = (divisor & 0xff);
//	UARTx->div_latch_high->reg = ((divisor >> 8) & 0xff);
//	UARTx->halt_tx_reg->reg = 0;
//
// 上面的赋值语句中，dlab = 1，会被优化掉。			--luojie
typedef volatile struct module_uart {
	uart_rbr_t			*receive;
	uart_thr_t			*trans_hold;
	uart_dll_t			*div_latch_low;
	uart_dlh_t			*div_latch_high;
	uart_ier_t			*int_enable;
	uart_iir_t			*int_identity;
	uart_fcr_t			*fifo_ctrl;
	uart_lcr_t			*line_ctrl;
	uart_mcr_t			*modem_ctrl;
	uart_lsr_t			*line_status;
	uart_msr_t			*modem_status;
	uart_sch_t			*scratch;
	uart_usr_t			*status;
	uart_tfl_t			*trans_fifo_lvl;
	uart_rfl_t			*receive_fifo_lvl;
	uart_hsk_t			*DMA_handshake;
	uart_halt_t			*halt_tx_reg;
	uart_dbg_dll_t			*debug_dll;
	uart_dbg_dlh_t			*debug_dlh;
	uart_485_ctrl_t			*rs485_ctrl_status;
	uart_485_addr_t			*rs485_addr_match;
	uart_485_bus_idle_chk_t		*rs485_bus_idle_chk;
	uart_tx_delay_t			*tx_delay;
	uart_ver_t			*version;
	__pCBK_t			phandler;
	void				*parg;
} UART_TypedDef;

typedef struct R_UART {
	struct module_uart	UART[R_UART_NUM];
	u32			irq_no[R_UART_NUM];
	__pISR_t		phandler;
} R_UART;


#endif

