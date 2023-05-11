##==============================================================================
##
##  crt0.S
##
##  nds32 startup code
##
##==============================================================================
##
## Copyright (c) 1995, 1996, 1997, 1998 Cygnus Solutions
##
## The authors hereby grant permission to use, copy, modify, distribute,
## and license this software and its documentation for any purpose, provided
## that existing copyright notices are retained in all copies and that this
## notice is included verbatim in any distributions. No written agreement,
## license, or royalty fee is required for any of the authorized uses.
## Modifications to this software may be copyrighted by their authors
## and need not follow the licensing terms described here, provided that
## the new terms are clearly indicated on the first page of each file where
## they apply.
##

#include <nds32_init.inc>
#include "config.h"
#include "interrupt.h"

#define SYS_STACK_SIZE          (2048)
##------------------------------------------------------------------------------
	.macro vector name
	.align 2
	j OS_Trap_\name
	.endm

	.macro OS_Trap_Interrupt_HW num
OS_Trap_Interrupt_HW\num:
	push $r0
	li  $r0, \num
	b   common_ISR_wrapper
	.endm


	.section .nds32_init, "ax"
!========================================================================
! Vector table
!========================================================================
	.align 2
exception_vector:
	j _start                    !  (0) Trap Reset
	vector TLB_Fill             !  (1) Trap TLB fill
	vector PTE_Not_Present      !  (2) Trap PTE not present
	vector TLB_Misc             !  (3) Trap TLB misc
	vector TLB_VLPT_Miss        !  (4) Trap TLB VLPT miss
	vector Machine_Error        !  (5) Trap Machine error
	vector Debug_Related        !  (6) Trap Debug related
	vector General_Exception    !  (7) Trap General exception
	vector Syscall              !  (8) Syscall

	/* HW interrupt vector layout */
	j OS_Trap_Interrupt_HW0     !  (9) Interrupt HW0
	j OS_Trap_Interrupt_HW1     ! (10) Interrupt HW1
	j OS_Trap_Interrupt_HW2     ! (11) Interrupt HW2
	j OS_Trap_Interrupt_HW3     ! (12) Interrupt HW3
	j OS_Trap_Interrupt_HW4     ! (13) Interrupt HW4
	j OS_Trap_Interrupt_HW5     ! (14) Interrupt HW5
	j OS_Trap_SWI               ! (15) Interrupt SW0


	.align  2
exception_vector_end:
exception_handler:
	.long tlb_exception_handler
	.long error_exception_handler
	.long syscall_handler
ISR_TABLE:
	.long HW0_ISR
!!	.long intc_handler			!HW1_ISR///////////////////////////////////
	.long HW2_ISR
	.long HW3_ISR
	.long HW4_ISR
	.long HW5_ISR
/*
 * exception handlers
 */

/*----------------------------------------------------------------------
   Since N903 doesn't have MMU, make them share the common error handler.
----------------------------------------------------------------------*/
OS_Trap_TLB_Fill:
/*
	SAVE_ALL
	li  $r0, #0x1
	b   tlb_exception_handler
*/

OS_Trap_PTE_Not_Present:
/*
	SAVE_ALL
	li  $r0, #0x2
	b   tlb_exception_handler
*/

OS_Trap_TLB_Misc:
/*
	SAVE_ALL
	li  $r0, #0x3
	b   tlb_exception_handler
*/

OS_Trap_TLB_VLPT_Miss:
1:  b   1b
	SAVE_ALL
//	li  $r0, #0x4
	b   tlb_exception_handler

OS_Trap_Machine_Error:
1:  b   1b
	SAVE_ALL
	li  $r0, #0x5
	b   error_exception_handler

OS_Trap_Debug_Related:
1:  b   1b
	SAVE_ALL
	li  $r0, #0x6
	b   error_exception_handler

OS_Trap_General_Exception:
1:  b   1b
	SAVE_ALL
	li  $r0, #0x7
	b   error_exception_handler

OS_Trap_Syscall:
	SYSCALL_SAVE_ALL
	bal syscall_handler
	SYSCALL_RESTORE_ALL
	iret

OS_Trap_Interrupt_HW0:
/*
	SAVE_ALL_HW
	la  $r1, ISR_TABLE
	lw  $r1, [$r1+$r0<<2]
	jral $r1
	RESTORE_ALL_HW
	iret
*/
	SAVE_ALL_HW0
	li  $r0, 0x0
	la  $r1, HW0_ISR
	jral $r1
	RESTORE_ALL_HW0
	iret
common_ISR_wrapper:
	SAVE_ALL_HW
!	la  $r1, ISR_TABLE
!	lw  $r1, [$r1+$r0<<2]
!	jral $r1
!!	jal intc_handler  //
	RESTORE_ALL_HW
	iret

OS_Trap_Interrupt_HW1:
	push $r0
	li   $r0, 0x01
	b    common_ISR_wrapper

OS_Trap_Interrupt_HW2:
	push $r0
	li  $r0, 0x02
	b   common_ISR_wrapper

OS_Trap_Interrupt_HW3:
	push $r0
	li  $r0, 0x03
	b   common_ISR_wrapper

OS_Trap_Interrupt_HW4:
	push $r0
	li  $r0, 0x04
	b   common_ISR_wrapper

OS_Trap_Interrupt_HW5:
	push $r0
	li  $r0, 0x05
	b   common_ISR_wrapper

OS_Trap_SWI:
	SAVE_ALL
	li  $r0, 0x0f
	la  $r1, SW0_ISR
	jral $r1
	RESTORE_ALL
	iret


OS_Trap_Interrupt_UNDEF:
	push $r0
	li  $r0, 0xff
	b   common_ISR_wrapper

	/* Other HW interrupt handlers */


##------------------------------------------------------------------------------
## Startup code

#ifdef CFG_LLINIT
#ifdef CFG_REMAP
#define LED_VALUE (0x66) //Burn
#else
#define LED_VALUE (0x11) //xIp
#endif
#else
#define LED_VALUE (0x00) //lOad
#endif

	.section .text
	.global _start
	/*Gjt Add*/
	.global _DLM_BASE
	.global _ILM_BASE
	/*Gjt Add*/

	.weak _call_exit
	.weak _SDA_BASE_
	.func _start
	.type _start, @function

	.align 2
_start:
REG_INT:
	li		$r1, 0
	li		$r2, 0
	li		$r3, 0
	li		$r4, 0
	li		$r5, 0
	li		$r6, 0
	li		$r7, 0
	li		$r8, 0
	li		$r9, 0
	li		$r10, 0
	li		$r15, 0
	li		$r28, 0
	li		$r29, 0
	li		$r30, 0
	li		$r31, 0
	mfsr	$r0, $MSC_CFG
	srli 	$r0, $r0, 10
	andi	$r0, $r0, 1
	bnez	$r0, SKIP_RDREG				!Check if Reduced Register is implented
	li		$r11, 0
	li		$r12, 0
	li		$r13, 0
	li		$r14, 0
	li		$r15, 0
	li		$r16, 0
	li		$r17, 0
	li		$r18, 0
	li		$r19, 0
	li		$r20, 0
	li		$r21, 0
	li		$r22, 0
	li		$r23, 0
	li		$r24, 0
	li		$r25, 0
	li		$r26, 0
	li		$r27, 0

SKIP_RDREG:
	mfsr	$r0, $MSC_CFG
	srli	$r0, $r0, 16
	andi	$r0, $r0, 1
	bnez	$r0, SKIP_D0D1
	mtusr	$r1, $d0.lo
	mtusr	$r1, $d0.hi
	mtusr	$r1, $d1.lo
	mtusr	$r1, $d1.hi

SKIP_D0D1:
	nop
	!************************** Begin of do-not-modify **************************
	! Please don't modify this code
	! Initialize the registers used by the compiler
	nds32_init		! NDS32 startup initial macro in <nds32_init.inc>

#ifdef CFG_DEBUG
	mfsr $r0, $MSC_CFG

#ifndef CONFIG_NO_NDS32_EXT_EX9
	li  $r1, (1 << 24)      ! EIT
	and $r2, $r0, $r1
1:	beqz $r2, 1b
#endif

#ifdef __NDS32_EXT_IFC__
	li  $r1, (1 << 19)      ! IFC
	and $r2, $r0, $r1
1:	beqz $r2, 1b
#endif

#endif //#ifdef CFG_DEBUG

	!*************************** End of do-not-modify ***************************

	!LED LED_VALUE           ! light LED

test_ilm:
	isb

test_dlm:
	isb

end_of_test_dlm:
	nop

#ifdef CFG_LLINIT
	bal _nds32_init_mem
#endif
	bal __init
#ifdef CFG_GCOV
	bal _crt_init
	bal startup_entry
	jal exit
#else

	bal startup_entry
1:  b   1b
#endif

	.size _start, .-_start

	.section .stack, "aw", @nobits
.space  SYS_STACK_SIZE
_stack:

	.end
