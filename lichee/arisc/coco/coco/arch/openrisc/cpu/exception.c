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

#include "ibase.h"
#include "driver.h"
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


//the C-language entry of exception handler
void cpu_exception_handler_entry_c(u32 exception, u32 epc, u32 *pstack);
void cpu_exception_handler_entry_c(u32 exception, u32 epc, u32 *pstack)
{
	u32 index;

	if (exception == 8)
	{
		//ar100 support UserIRQ
		interrupt_entry();
		return;
	}

	//dump debug infortions
	printk("exception [%x, %s] coming, [epc = %x]\n",
		exception, exception_strings[exception], epc);
	printk("register list:\n");
	for (index = 2; index < 32; index++)
	{
		printk("register%x: %x\n", index, pstack[index - 2]);
	}
	printk("cpu abort enter...\n");
	asm("l.nop");
	while(1);
}
