/*
*********************************************************************************************************
*                                                AR100 SYSTEM
*                                     AR100 Software System Develop Kits
*                                                 cpu module
*
*                                    (c) Copyright 2012-2016, Sunny China
*                                             All Rights Reserved
*
* File    : exception.c
* By      : Sunny
* Version : v1.0
* Date    : 2012-5-14
* Descript: cpu exceptions.
* Update  : date                auther      ver     notes
*           2012-5-14 10:05:15  Sunny       1.0     Create this file.
*********************************************************************************************************
*/
#include <nds32_intrinsic.h>

#include "ibase.h"
#include "driver.h"
#include "arch.h"
//the string of exceptions
const char *exception_strings[] =
{
	"An unknown",                           // 0
	"A reset",                              // 1
	"A Bus Error",                          // 2
	"A Data Page Fault",                    // 3
	"An Instruction Page Fault",            // 4
	"A Tick-Timer",                         // 5
	"An Alignment",                         // 6
	"An Illegal Instruction",               // 7
	"An External Interrupt",                // 8
	"A D-TLB Miss",                         // 9
	"An I-TLB Miss",                        // a
	"A Range",                              // b
	"A System Call",                        // c
	"A Floating-Point",                     // d
	"A Trap",                               // e
	"A Reserved",                           // f
};

volatile int d10_hw0_mask = 0;
//the C-language entry of exception handler
void cpu_exception_handler_entry_c(int isr_type)
{
	u32 rdata;
    //2.enable HW0
	isr_type = isr_type;

	rdata = __nds32__mfsr(NDS32_SR_INT_MASK);
	rdata |= 0x00000;
	__nds32__mtsr(rdata, NDS32_SR_INT_MASK);

	d10_hw0_mask = 1;
//	printk("isr_type %d\n",isr_type);
	interrupt_entry();

	rdata = __nds32__mfsr(NDS32_SR_INT_MASK);
	rdata |= 0x10001;
	__nds32__mtsr(rdata, NDS32_SR_INT_MASK);

	d10_hw0_mask = 0;
}
