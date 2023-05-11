 /******************************************************
  ** file name:  init-default.c
  ** Modify:	 gaojiangtao
  ** date:		 2018/1/4
  ** version:	 V0.2
  ****************************************************/

#include <nds32_intrinsic.h>
#include "config.h"
#include "ibase.h"
#include "arch.h"
//#include "assert_demos.h"

//#include <stdio.h>
//#include <stdlib.h>

/* It will use Default_Handler if you don't have one */
#pragma weak tlb_exception_handler   = Default_Handler
#pragma weak error_exception_handler = Default_Handler
#pragma weak syscall_handler         = Default_Handler
#pragma weak HW0_ISR   = Default_Handler
#pragma weak HW1_ISR   = Default_Handler
#pragma weak HW2_ISR   = Default_Handler
#pragma weak HW3_ISR   = Default_Handler
#pragma weak HW4_ISR   = Default_Handler
#pragma weak HW5_ISR   = Default_Handler
#pragma weak SW0_ISR   = Default_Handler
#pragma weak VEP0_ISR  = Default_Handler
#pragma weak VEP1_ISR  = Default_Handler
#pragma weak VEP2_ISR  = Default_Handler
#pragma weak VEP3_ISR  = Default_Handler
#pragma weak VEP4_ISR  = Default_Handler
#pragma weak VEP5_ISR  = Default_Handler
#pragma weak VEP6_ISR  = Default_Handler
#pragma weak VEP7_ISR  = Default_Handler
#pragma weak VEP8_ISR  = Default_Handler
#pragma weak VEP9_ISR  = Default_Handler
#pragma weak VEP10_ISR = Default_Handler
#pragma weak VEP11_ISR = Default_Handler
#pragma weak VEP12_ISR = Default_Handler
#pragma weak VEP13_ISR = Default_Handler
#pragma weak VEP14_ISR = Default_Handler
#pragma weak VEP15_ISR = Default_Handler
#pragma weak VEP16_ISR = Default_Handler
#pragma weak VEP17_ISR = Default_Handler
#pragma weak VEP18_ISR = Default_Handler
#pragma weak VEP19_ISR = Default_Handler
#pragma weak VEP20_ISR = Default_Handler
#pragma weak VEP21_ISR = Default_Handler
#pragma weak VEP22_ISR = Default_Handler
#pragma weak VEP23_ISR = Default_Handler
#pragma weak VEP24_ISR = Default_Handler
#pragma weak VEP25_ISR = Default_Handler
#pragma weak VEP26_ISR = Default_Handler
#pragma weak VEP27_ISR = Default_Handler
#pragma weak VEP28_ISR = Default_Handler
#pragma weak VEP29_ISR = Default_Handler
#pragma weak VEP30_ISR = Default_Handler
#pragma weak VEP31_ISR = Default_Handler
#pragma weak VEP32_ISR = Default_Handler
#pragma weak VEP33_ISR = Default_Handler
#pragma weak VEP34_ISR = Default_Handler
#pragma weak VEP35_ISR = Default_Handler
#pragma weak VEP36_ISR = Default_Handler
#pragma weak VEP37_ISR = Default_Handler
#pragma weak VEP38_ISR = Default_Handler
#pragma weak VEP39_ISR = Default_Handler
#pragma weak VEP40_ISR = Default_Handler
#pragma weak VEP41_ISR = Default_Handler
#pragma weak VEP42_ISR = Default_Handler
#pragma weak VEP43_ISR = Default_Handler
#pragma weak VEP44_ISR = Default_Handler
#pragma weak VEP45_ISR = Default_Handler
#pragma weak VEP46_ISR = Default_Handler
#pragma weak VEP47_ISR = Default_Handler
#pragma weak VEP48_ISR = Default_Handler
#pragma weak VEP49_ISR = Default_Handler
#pragma weak VEP50_ISR = Default_Handler
#pragma weak VEP51_ISR = Default_Handler
#pragma weak VEP52_ISR = Default_Handler
#pragma weak VEP53_ISR = Default_Handler
#pragma weak VEP54_ISR = Default_Handler
#pragma weak VEP55_ISR = Default_Handler
#pragma weak VEP56_ISR = Default_Handler
#pragma weak VEP57_ISR = Default_Handler
#pragma weak VEP58_ISR = Default_Handler
#pragma weak VEP59_ISR = Default_Handler
#pragma weak VEP60_ISR = Default_Handler
#pragma weak VEP61_ISR = Default_Handler
#pragma weak VEP62_ISR = Default_Handler
#pragma weak VEP63_ISR = Default_Handler

extern int uart_puts(const char *);

__attribute__((unused))
static void Default_Handler(int isr_typ)
{
	cpu_exception_handler_entry_c(isr_typ);
}

extern volatile int d10_hw0_mask;
static int int_disable_ref = 0;
void cpu_enable_int(signed int arg)
{
	arg = arg;
	if(d10_hw0_mask)
		return ;

	int_disable_ref--;
	if(int_disable_ref > 0)
		return;

       __nds32__setgie_en();
}

signed int cpu_disable_int(void)
{
	if(d10_hw0_mask)
		return 0;

	int_disable_ref++;
	if(int_disable_ref > 1)
		return 0;

       __nds32__setgie_dis();
       __nds32__dsb();

       return 0;
}



//#pragma weak __soc_init = __null_function
//
//void __null_function()
//{;
//}

extern char __bss_end;
extern char __bss_start;
static void __c_init(void)
{
#define MEMCPY(des, src, n) __builtin_memcpy ((des), (src), (n))
#define MEMSET(s, c, n) __builtin_memset ((s), (c), (n))
	int size;

      /* Clear bss section */
	size = &__bss_end - &__bss_start;
	MEMSET(&__bss_start, 0, size);
	return;
}

static void __cpu_init(void)
{
	unsigned int tmp;

	/* turn on BTB */
	tmp = 0x0;
	__nds32__mtsr(tmp, NDS32_SR_MISC_CTL);

	/* disable all hardware interrupts */
	__nds32__mtsr(0x0, NDS32_SR_INT_MASK);
	if (__nds32__mfsr(NDS32_SR_IVB) & 0x01)
		__nds32__mtsr(0x0, NDS32_SR_INT_MASK);

	/* set IVIC, vector size: 4 bytes, base: 0x0
	 * If we use v3/v3m toolchain and want to use
	 * assembly version please don't use USE_C_EXT
	 * in CFLAGS */
	__nds32__mtsr(0x0, NDS32_SR_IVB);

	/* Set PSW INTL to 0 */
	tmp = __nds32__mfsr(NDS32_SR_PSW);
	tmp = tmp & 0xfffffff9;
	/* Set PSW CPL to 7 to allow any priority */
	tmp = tmp | 0x70008;
	__nds32__mtsr_dsb(tmp, NDS32_SR_PSW);
	/* Check interrupt priority programmable*
	* IVB.PROG_PRI_LVL
	*      0: Fixed priority       -- no exist ir18 1r19
	*      1: Programmable priority
	*/
	if (__nds32__mfsr(NDS32_SR_IVB) & 0x01) {
		/* Set PPL2FIX_EN to 0 to enable Programmable
	 	* Priority Level */
		__nds32__mtsr(0x0, NDS32_SR_INT_CTRL);
		/* Check IVIC numbers (IVB.NIVIC) */
		if ((__nds32__mfsr(NDS32_SR_IVB) & 0x0E)>>1 == 5) {	// 32IVIC
				/* set priority HW9: 0, HW13: 1, HW19: 2,
			* HW#-: 0 */
			__nds32__mtsr(0x04000000, NDS32_SR_INT_PRI);
			__nds32__mtsr(0x00000080, NDS32_SR_INT_PRI2);
		} else {
			/* set priority HW0: 0, HW1: 1, HW2: 2, HW3: 3
			* HW4-: 0 */
			__nds32__mtsr(0x000000e4, NDS32_SR_INT_PRI);
		}
	}

	return;
}

void cpu_init(void)
{
	unsigned int rdata;

	cpu_enable_int(0);

	//1.set priority to lowest
	__nds32__mtsr(0x1 << 0, NDS32_SR_INT_PRI);

      //2.enable HW0
	rdata = __nds32__mfsr(NDS32_SR_INT_MASK);
	rdata |= 0x10001;
	__nds32__mtsr(rdata, NDS32_SR_INT_MASK);

      //3.Interrupt pending register, write 1 to clear
	__nds32__mtsr(0xFFFFFFFF, NDS32_SR_INT_PEND2);
}

void __init(void)
{
/*----------------------------------------------------------
   !!  Users should NOT add any code before this comment  !!
------------------------------------------------------------*/
	__cpu_init();
	__c_init();     //copy data section, clean bss


//	__soc_init();

	/* Double check ZOL supporting */
	/*
	 * Check whether the CPU configured with ZOL supported.
	 * The MSC_CFG.MSC_EXT($cr4) indicates MSC_CFG2 register exist
	 * and MSC_CFG2($cr7) bit 5 indicates ZOL supporting.
	 */
}



#ifdef CFG_GCOV
void abort(void)
{
	fflush(stdout);
	exit(1);
}
#endif
