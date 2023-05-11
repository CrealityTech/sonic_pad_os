#include "lib_inc.h"

static struct timerc_regs *ptimerc_regs;

void Timer_DefInit(TIM_InitConfig *config)
{
	TIM_InitConfig *timer_config = config;

	timer_config->Timer_Mode		= TIM_Mode_Cycle;
	timer_config->Timer_PreScale		= TIM_PreScale_1;
	timer_config->Timer_ClkSrc		= TIM_ClkSrc_HOSC;
}

s32 Timer_RegInit(TIM_TypedDef **TIMx,u32 reg_base)
{
	u32 timer_id;

	timer_id = ((reg_base - R_TMR_REG_BASE)/0x10) - 1;

	if(timer_id > TIMERC_TIMERS_NUMBER)
		return FAIL;

	//initialize the pointer of timer controller registers
	ptimerc_regs = (struct timerc_regs *)(R_TMR_REG_BASE);

	*TIMx = &(ptimerc_regs->timer[timer_id]);

	return OK;
}


void Timer_SetSrc(TIM_TypedDef *TIMx, u32 CLK_SRC)
{
	TIMx->control &= ~(0x3 << 2);
	TIMx->control |= (CLK_SRC << 2);
}

void Timer_Config(TIM_TypedDef *TIMx, TIM_InitConfig *config)
{
	TIMx->control &= ~(0x3 << 2);
	TIMx->control |= (config->Timer_ClkSrc << 2);

	TIMx->control &= ~(0x7 << 4);
	TIMx->control |= (config->Timer_PreScale << 4);

	TIMx->control &= ~(0x1 << 7);
	TIMx->control |= (config->Timer_Mode << 7);
}

u32 Timer_Get_InValue(TIM_TypedDef *TIMx)
{
	return	TIMx->interval_value;
}

void Timer_Set_InValue(TIM_TypedDef *TIMx,u32 value)
{
	TIMx->interval_value = value;
	TIMx->control |= (1<<1); //reload interval value to current value
	while((TIMx->control) & (1<<1))
		;
}

void Timer_Start(TIM_TypedDef *TIMx)
{
	TIMx->control |= 0x1;
}

void Timer_Stop(TIM_TypedDef *TIMx)
{
	TIMx->control &= ~0x1;
}

s32 Timer_IRQEnable(TIM_TypedDef *TIMx)
{
	u32 timer_id;
	u32 timer_addr = (u32)TIMx;

	timer_id = ((timer_addr - R_TMR_REG_BASE)/0x10) - 1;

	if(timer_id > TIMERC_TIMERS_NUMBER)
		return FAIL;

	writel(readl(R_TMR_REG_BASE + TIMER_IRQ_EN_REG) | (1 << timer_id),
				R_TMR_REG_BASE + TIMER_IRQ_EN_REG);

	return OK;
}

s32 Timer_IRQDisable(TIM_TypedDef *TIMx)
{
	u32 timer_id;
	u32 timer_addr = (u32)TIMx;

	timer_id = ((timer_addr - R_TMR_REG_BASE)/0x10) - 1;

	if(timer_id > TIMERC_TIMERS_NUMBER)
		return FAIL;

	writel(readl(R_TMR_REG_BASE + TIMER_IRQ_EN_REG) & (~(1 << timer_id)),
				R_TMR_REG_BASE + TIMER_IRQ_EN_REG);


	return OK;
}

u32 Timer_GetIRQStatus(TIM_TypedDef *TIMx)
{
	u32 timer_id;
	u32 timer_addr = (u32)TIMx;

	timer_id = ((timer_addr - R_TMR_REG_BASE)/0x10) - 1;

	if(timer_id > TIMERC_TIMERS_NUMBER)
		return FAIL;

	return (readl(R_TMR_REG_BASE + TIMER_IRQ_STA_REG) >> timer_id) & 0x1;
}

u32 Timer_ClearPending(TIM_TypedDef *TIMx)
{
	u32 timer_id;

	timer_id = Timer_GetId(TIMx);;

	if(timer_id > TIMERC_TIMERS_NUMBER)
		return FAIL;

	writel(readl(R_TMR_REG_BASE + TIMER_IRQ_STA_REG) | (1 << timer_id),
					R_TMR_REG_BASE + TIMER_IRQ_STA_REG);

	return OK;
}

u32 Timer_GetId(TIM_TypedDef *TIMx)
{
	u32 timer_addr = (u32)TIMx;

	return ((timer_addr - R_TMR_REG_BASE)/0x10) - 1;
}

u32 Timer_SetMode(TIM_TypedDef *TIMx, u32 mode)
{
	TIMx->control &= ~(0x1 << 7);
	TIMx->control |= (mode << 7);

	return OK;
}

u32 Timer_GetMsTick(TIM_TypedDef *TIMx)
{
	u32 value = 0;

	value = TIMx->control >> 2;
	value &= 0x3;

	if(value == 0)
		return 32;
	else if(value == 0x1)
		return 24000;
	else
		return 0;
}











