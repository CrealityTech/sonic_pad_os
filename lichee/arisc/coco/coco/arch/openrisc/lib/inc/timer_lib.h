/*
*********************************************************************************************************
*                                                AR100 SYSTEM
*                                     AR100 Software System Develop Kits
*
*
*                                    (c) Copyright 2018-2024, Luojie China
*)                                             All Rights Reserved
*
* File    : timer_lib.h
* By      : LuoJie
* Version : v1.0
* Date    : 2018-04-11 17:14
* Descript: .
* Update  :auther      ver     notes
*          Luojie      1.0     Create this file.
*********************************************************************************************************
*/
#ifndef __TIMER_LIB_H__
#define __TIMER_LIB_H__

#include "lib_inc.h"

typedef volatile struct {
	u32	Timer_Mode;
	u32	Timer_PreScale;
	u32	Timer_ClkSrc;
} TIM_InitConfig;


#define TIM_Mode_Cycle		0x0
#define TIM_Mode_Single		0x1

#define TIM_PreScale_1		0
#define TIM_PreScale_2		1
#define TIM_PreScale_4		2
#define TIM_PreScale_8		3
#define TIM_PreScale_16		4
#define TIM_PreScale_32		5
#define TIM_PreScale_64		6
#define TIM_PreScale_128	7

#define TIM_ClkSrc_LOSC		0
#define TIM_ClkSrc_HOSC		1


void Timer_DefInit(TIM_InitConfig *config);
s32 Timer_RegInit(TIM_TypedDef **TIMx, u32 reg_base);
void Timer_Config(TIM_TypedDef *TIMx, TIM_InitConfig *config);
void Timer_Set_InValue(TIM_TypedDef *TIMx,u32 value);
void Timer_Start(TIM_TypedDef *TIMx);
void Timer_Stop(TIM_TypedDef *TIMx);
s32 Timer_IRQEnable(TIM_TypedDef *TIMx);
s32 Timer_IRQDisable(TIM_TypedDef *TIMx);
u32 Timer_GetIRQStatus(TIM_TypedDef *TIMx);
u32 Timer_ClearPending(TIM_TypedDef *TIMx);
u32 Timer_GetId(TIM_TypedDef *TIMx);
u32 Timer_SetMode(TIM_TypedDef *TIMx, u32 mode);
u32 Timer_GetMsTick(TIM_TypedDef *TIMx);
void Timer_SetSrc(TIM_TypedDef *TIMx, u32 CLK_SRC);
u32 Timer_Get_InValue(TIM_TypedDef *TIMx);


#endif
