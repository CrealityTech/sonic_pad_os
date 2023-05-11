/*
*********************************************************************************************************
*                                                AR100 SYSTEM
*                                     AR100 Software System Develop Kits
*
*
*                                    (c) Copyright 2018-2024, Luojie China
*)                                             All Rights Reserved
*
* File    : uart.c
* By      : LuoJie
* Version : v1.0
* Date    : 2018-04-11 16:43
* Descript: .
* Update  :auther      ver     notes
*          Luojie      1.0     Create this file.
*********************************************************************************************************
*/
#include "uart_i.h"

uart_device suart;

static volatile u32 uart_pin_not_used;

extern int arisc_para_get_uart_pin_info(void);

uart_device *query_uart(void)
{
	return &suart;
}


/*
 * uart_putc() - exit uart.
 *
 * @return: OK if exit uart succeeded, others if failed.
 * @note: exit uart.
 */
s32 uart_exit(uart_device *uart)
{
	uart->dev.dev_lock = 1;

//	ccu_unreg_mclk_cb(CCU_MOD_CLK_R_UART, uart->clkchangecb);

//	pin_set_multi_sel(PIN_GRP_PL, 2, 7); //PL2, select as uart_tx
//	pin_set_multi_sel(PIN_GRP_PL, 3, 7); //PL3, select as uart_rx

	return OK;
}

/*
 * uart_putc() - put out a charset.
 *
 * @ch: the charset which we want to put out.
 * @return: OK if put out charset succeeded, others if failed.
 * @note: put out a charset.
 */
s32 uart_putc(uart_device *uart,char ch)
{
	if (uart->dev.dev_lock == 1 || uart_pin_not_used == 1)
	        return -EACCES;
	else
		return UART_SendData(&uart->UARTx,ch);
}

/*
 * uart_getc() - get a charset.
 *
 * @return: the charset we read from uart.
 * @note: get a charset.
 */
s32 uart_get(uart_device *uart,char *buf)
{
	if (uart->dev.dev_lock == 1 || uart_pin_not_used == 1)
	        return -EACCES;
	else {
		return 	UART_ReceiveData(&uart->UARTx,buf);
	}
}

/*
 * uart_puts() - put out a string.
 *
 * @string: the string which we want to put out.
 * @return: OK if put out string succeeded, others if failed.
 * @note: put out a string.
 */
s32 uart_puts(uart_device *uart,const char *string)
{
	ASSERT(string != NULL);

	if (uart->dev.dev_lock == 1 || uart_pin_not_used == 1)
	        return -EACCES;

	while (*string != '\0') {
		if(uart->flow_ctrl) {
			if(UART_ModemStatus(&uart->UARTx,MODEM_ALL) & MODEM_CTS) {
				u32 poll_time = 0xffffff;
				uart->flow_cts_count++;
				while((!UART_LineStatus(&uart->UARTx,LINE_THRE)) && poll_time--)
					;
			}
		}

		if (*string == '\n') /* if current character is '\n', insert output with '\r' */
			UART_SendData(&uart->UARTx,'\r');
		UART_SendData(&uart->UARTx,*string++);
	}

	return OK;
}

s32 uart_set_baudrate(uart_device *uart,u32 baud_rate)
{
	if (uart_pin_not_used == 1)
		return -EACCES;

	uart->baud_rate = baud_rate;

	return UART_SetBaudRate(&uart->UARTx,baud_rate);
}

s32 uart_get_baudrate(uart_device *uart)
{
	return uart->baud_rate;
}

s32 uart_set_lock(uart_device *uart,u32 lock)
{
	while (!IS_TX_FIFO_EMPTY(uart))
		continue;

	uart->dev.dev_lock = lock;

	return OK;
}


static s32 uart_clkchangecb(u32 command, void *parg __attribute__ ((__unused__)))
{
	if (uart_pin_not_used == 1)
		return -EACCES;

	switch(command) {
		case CCU_APBS2_CLK_CLKCHG_REQ:
		{
			//clock will be change
			LOG("uart source clock change request\n");
			//wait uart transmit fifo empty
			//lock uart
			uart_set_lock(&suart,1);
			return OK;
		}
		case CCU_ABPS2_CLK_CLKCHG_DONE:
		{
			//reconfig uart current baudrate
			uart_set_lock(&suart,0);
			uart_set_baudrate(&suart,BaudRate_115200);
			LOG("uart buadrate change done\n");

			return OK;
		}
		default:
		break;
	}

	return OK;
}


static s32 uart_suspend(arisc_device *dev __attribute__ ((__unused__)))
{
	return OK;
}

static s32 uart_resume(arisc_device *dev __attribute__ ((__unused__)))
{
	return OK;
}



arisc_driver uart_dri = {
	.suspend = uart_suspend,
	.resume  = uart_resume,
};

const struct uart_ops arisc_uart_ops = {
	.putc = uart_putc,
	.get = uart_get,
	.puts = uart_puts,
	.set_baudrate = uart_set_baudrate,
	.get_baudrate = uart_get_baudrate,
	.set_lock = uart_set_lock,
//	.suspend_early = uart_suspend_early,
//	.resume_early = uart_resume_early,
};

static void uart_pin_init(uart_device *uart)
{
	pin_config uart_pin_config = {0};

	uart_pin_config.pin_grp = GPIO_GP_PL;
	uart_pin_config.pin_fn  = GPIO_FN_2;
	uart_pin_config.pin_driving = GPIO_MULTI_DRIVE_1;
	uart_pin_config.pin_pull = GPIO_PULL_UP;

	uart_pin_config.pin_num = GPIO_PIN_2;
	pin_set(&uart_pin_config,&(uart->uart_tx));

	uart_pin_config.pin_num = GPIO_PIN_3;
	pin_set(&uart_pin_config,&(uart->uart_rx));
}


s32 uart_init(void)
{
	UART_InitConfig uart_config;

	uart_pin_not_used = !arisc_para_get_uart_pin_info();
	if (uart_pin_not_used == 1)
		return -EACCES;

	suart.dev.hw_ver = 0x10;
	suart.dev.reg_base = UART_REG_BASE;
	suart.dev.dev_lock = 1;
	suart.dev.dev_id = UART_DEVICE_ID;
	suart.dev.dri = &uart_dri;
	suart.dev.irq_no = INTC_R_UART_IRQ;
	suart.ops = &arisc_uart_ops;

	suart.clkchangecb = uart_clkchangecb;

//	if (dev_match(&uart.dev,R_UART_REG_BASE + R_UART_VER)) {
		prcm_reset_clk(get_prcm_device(),UART_BUS_GATING_REG);
		uart_pin_init(&suart);
		UART_Reg_Init(&(suart.UARTx), suart.dev.reg_base);
		UART_DefInit(&uart_config);
		UART_Config(&(suart.UARTx), &uart_config);

		suart.baud_rate = uart_config.UART_BaudRate;
		suart.flow_ctrl = uart_config.UART_HardwareFlowCtr;
		list_add_tail(&suart.dev.list,&(dev_list));
		suart.dev.dev_lock = 0;
//		install_isr(uart.irq_no, uart.handler, (void *)(&uart));
		//这里需要插入通知链。
		ccu_apbs2_reg_mclk_cb(uart_clkchangecb,NULL);
		return OK;
//	}
//	else
//		uart_exit(&uart);

	return FAIL;
}


