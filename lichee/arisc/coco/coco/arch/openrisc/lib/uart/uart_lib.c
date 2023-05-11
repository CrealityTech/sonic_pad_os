#include "lib_inc.h"

void UART_DefInit(UART_InitConfig *config)
{
	UART_InitConfig *uart_config = config;

	uart_config->UART_BaudRate		= BaudRate_115200;
	uart_config->UART_WordLength		= WordLength_8;
	uart_config->UART_StopBit		= StopBit_1;
	uart_config->UART_Parity		= Parity_Disable;
	uart_config->UART_HardwareFlowCtr	= FlowCtr_Disable;
	uart_config->UART_UartMode		= UartMode_Uart;

}

void UART_Reg_Init(UART_TypedDef *UARTx, u32 reg_base)
{
	UARTx->receive			= (uart_rbr_t *)(reg_base + R_UART_RBR);
	UARTx->trans_hold		= (uart_thr_t *)(reg_base + R_UART_THR);
	UARTx->div_latch_low		= (uart_dll_t *)(reg_base + R_UART_DLL);
	UARTx->div_latch_high		= (uart_dlh_t *)(reg_base + R_UART_DLH);
	UARTx->int_enable		= (uart_ier_t *)(reg_base + R_UART_IER);
	UARTx->int_identity		= (uart_iir_t *)(reg_base + R_UART_IIR);
	UARTx->fifo_ctrl		= (uart_fcr_t *)(reg_base + R_UART_FCR);
	UARTx->line_ctrl		= (uart_lcr_t *)(reg_base + R_UART_LCR);
	UARTx->modem_ctrl		= (uart_mcr_t *)(reg_base + R_UART_MCR);
	UARTx->line_status		= (uart_lsr_t *)(reg_base + R_UART_LSR);
	UARTx->modem_status		= (uart_msr_t *)(reg_base + R_UART_MSR);
	UARTx->scratch			= (uart_sch_t *)(reg_base + R_UART_SCH);
	UARTx->status			= (uart_usr_t *)(reg_base + R_UART_USR);
	UARTx->trans_fifo_lvl		= (uart_tfl_t *)(reg_base + R_UART_TFL);
	UARTx->receive_fifo_lvl		= (uart_rfl_t *)(reg_base + R_UART_RFL);
	UARTx->DMA_handshake		= (uart_hsk_t *)(reg_base + R_UART_HSK);
	UARTx->halt_tx_reg		= (uart_halt_t *)(reg_base + R_UART_HALT);
	UARTx->debug_dll		= (uart_dbg_dll_t *)(reg_base + R_UART_DBG_DLL);
	UARTx->debug_dlh		= (uart_dbg_dlh_t *)(reg_base + R_UART_DBG_DLH);
	UARTx->rs485_ctrl_status	= (uart_485_ctrl_t *)(reg_base + R_UART_485_CTL);
	UARTx->rs485_addr_match		= (uart_485_addr_t *)(reg_base + R_UART_485_ADDR);
	UARTx->rs485_bus_idle_chk	= (uart_485_bus_idle_chk_t *)(reg_base + R_UART_BUS_IDLE);
	UARTx->tx_delay			= (uart_tx_delay_t *)(reg_base + R_UART_TX_DLY);
}


void UART_Config(UART_TypedDef *UARTx, UART_InitConfig *config)
{
	u32 divisor;
	u32 apbs_clk;

	if(config->UART_UartMode != UartMode_Uart) {
//		ERR("only support uart mode for now\n");
		return ;
	}
	UARTx->modem_ctrl->bits.uart_function = UartMode_Uart;

	apbs_clk = Get_APBS2_Freq();

	//计算 div， 四舍五入。
	divisor = (apbs_clk + (config->UART_BaudRate << 3))/(config->UART_BaudRate << 4);
	UARTx->line_ctrl->bits.dlab = ARTIVE_DLL;

	UARTx->div_latch_low->reg = (divisor & 0xff);
	UARTx->div_latch_high->reg = ((divisor >> 8) & 0xff);

	UARTx->halt_tx_reg->reg = 0;
	UARTx->line_ctrl->bits.dls = config->UART_WordLength;
	UARTx->line_ctrl->bits.dlab = ACTIVE_RBR;

	UARTx->modem_ctrl->bits.afce = config->UART_HardwareFlowCtr;

	if(config->UART_StopBit == StopBit_1)
	      	UARTx->line_ctrl->bits.stop = StopBit_1;
	else if(config->UART_StopBit == StopBit_1_5) {
		UARTx->line_ctrl->bits.stop = StopBit_1_5;
		UARTx->line_ctrl->bits.dls = WordLength_5;
		config->UART_WordLength = WordLength_5;
	}
	else if(config->UART_StopBit == StopBit_2)
		UARTx->line_ctrl->bits.stop = 1;

	if(config->UART_Parity != Parity_Disable) {
		if(config->UART_Parity == Parity_Odd)
			UARTx->line_ctrl->bits.eps = 0;
		else if (config->UART_Parity == Parity_Even)
			UARTx->line_ctrl->bits.eps = 0x1;
		else
			UARTx->line_ctrl->bits.eps = 0x2;

	}
	else
		UARTx->line_ctrl->bits.pen = Parity_Disable;

	if(config->UART_HardwareFlowCtr) {
		UARTx->fifo_ctrl->bits.rt = FIFO_HALF_FULL;
		UARTx->fifo_ctrl->bits.tft = FIFO_HALF_FULL;
		UARTx->fifo_ctrl->reg |= (XFIFO_RESET | RFIFO_RESET | ENABLE_FIFO);
	}
	else
		UARTx->fifo_ctrl->reg = (XFIFO_RESET | RFIFO_RESET | ENABLE_FIFO);

#ifdef CFG_SHELL_USED
	UARTx->int_enable->bits.erbfi  = 1;
#endif
}

s32 UART_SetBaudRate(UART_TypedDef *UARTx,u32 Rate)
{
	u32 divisor;
	u32 apbs_clk;

	while (UARTx->trans_fifo_lvl->reg)
		continue;


	apbs_clk = Get_APBS2_Freq();
	divisor = (apbs_clk + (Rate << 3))/(Rate << 4);

	if(divisor == 0)
		divisor = 1;

	UARTx->halt_tx_reg->bits.chcfg_at_busy = 1;
	UARTx->line_ctrl->bits.dlab = 1;
	UARTx->div_latch_low->reg= (divisor & 0xff);
	UARTx->div_latch_high->reg = ((divisor >> 8) & 0xff);
	UARTx->line_ctrl->bits.dlab = 0;

	UARTx->halt_tx_reg->bits.change_update = 1;
	while(UARTx->halt_tx_reg->bits.change_update);
		;

	UARTx->halt_tx_reg->bits.change_update = 1;
	UARTx->halt_tx_reg->bits.chcfg_at_busy = 0;

	return OK;
}


void UART_IRQ_Request(UART_TypedDef *UARTx, __pCBK_t phdle, void *parg)
{
	UARTx->phandler = phdle;
	UARTx->parg	= parg;
}


/*
 * UART_SendData() - put out a charset.
 *
 * @Data: the charset which we want to put out.
 * @return: OK if put out charset succeeded, others if failed.
 * @note: put out a charset.
 */

s32 UART_SendData(UART_TypedDef *UARTx, u16 Data)
{

	while(!(IS_TX_FIFO_NOT_FULL(UARTx)))
		;

	UARTx->trans_hold->reg = Data & 0xff;

	return OK;
}


/*
 * UART_ReceiveData() - get a charset.
 *
 * @return: the charset we read from uart.
 * @note: get a charset.
 */
s32 UART_ReceiveData(UART_TypedDef *UARTx,char *buf)
{
	u32 count = 0;

	while(UARTx->receive_fifo_lvl->reg) {
		*buf++ = (char)(UARTx->receive->reg);
		count++;
	}

	return count;
}

u32 UART_ModemStatus(UART_TypedDef *UARTx, u32 modem)
{
	return (UARTx->modem_status->reg) & modem;
}

u32 UART_LineStatus(UART_TypedDef *UARTx, u32 line)
{
	return (UARTx->modem_status->reg) & line;
}

void UART_IRQ_Config(UART_TypedDef *UARTx, u32 irq, u32 state)
{
	if(state == UART_ENABLE)
		UARTx->int_enable->reg |= irq;
	else if(state == UART_DISABLE)
		UARTx->int_enable->reg &= (~irq);
}


