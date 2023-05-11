/*
*********************************************************************************************************
*                                                AR100 SYSTEM
*                                     AR100 Software System Develop Kits
*                                                 CPU module
*
*                                    (c) Copyright 2012-2016, Sunny China
*                                             All Rights Reserved
*
* File    : cpu.S
* By      : Sunny
* Version : v1.0
* Date    : 2012-5-2
* Descript: cpu module operations.
* Update  : date                auther      ver     notes
*           2012-5-2 16:24:36   Sunny       1.0     Create this file.
*********************************************************************************************************
*/

//#include "cpu_i.h"
#include "spr_defines.h"
#include "cache.h"

//from openrisc GCC 4.5.1 version, The "red zone" is reduced to 128 bytes.
//by sunny at 2012-5-2 16:40:25.
#define RED_ZONE                (128)
#define EXCEPTION_STACK_SIZE    (RED_ZONE + 128)

//the byte size of stack
#define SYS_STACK_SIZE          (2048)

// Really goes to configurable interrupt handler
	.global _start
#define EXCEPTION_HANDLER(no)               \
	l.addi  r1, r1, -EXCEPTION_STACK_SIZE;  \
	l.sw    0x04(r1), r3;                   \
	l.sw    0x08(r1), r4;                   \
	l.sw    0x0c(r1), r5;                   \
	l.addi  r3, r0, no;                     \
	l.mfspr r4, r0, SPR_EPCR_BASE;          \
	l.addi  r5, r1, 0x0;                    \
	l.j     cpu_exception_handler_entry;    \
	l.nop

	.macro load32i reg const
	l.movhi \reg,hi(\const)
	l.ori   \reg,\reg,lo(\const)
	.endm

	//-------------------------------------
	//          [ statck section ]
	//-------------------------------------
	.section .stack, "aw", @nobits
.space  SYS_STACK_SIZE
_stack:


	//-------------------------------------
	//          [ exceptions ]
	//-------------------------------------
	.section .vectors, "ax"

_start:
	//[ 0x100: RESET exception ]
	.org 0x100
	l.j reset_handler
	l.nop

	//[ 0x200: BUS exception ]
	.org 0x200
	l.j bus_error_exception_handler
	l.nop

	//[ 0x300: Data Page Fault exception ]
	.org 0x300
	l.j data_page_fault_exception_handler
	l.nop

	//[ 0x400: Insn Page Fault exception ]
	.org 0x400
	l.j insn_page_fault_exception_handler
	l.nop

	//[ 0x500: Timer exception ]
	.org 0x500
	l.j timer_exception_handler
	l.nop

	//[ 0x600: Aligment exception ]
	.org 0x600
	l.j aligment_exception_handler
	l.nop

	//[ 0x700: Illegal insn exception ]
	.org 0x700
	l.j illegal_exception_handler
	l.nop

	//[ 0x800: External interrupt exception ]
	.org 0x800
	l.j irq_exception_handler
	l.nop

	//[ 0x900: DTLB miss exception ]
	.org 0x900
	l.j dtlb_miss_exception_handler
	l.nop

	//[ 0xa00: ITLB miss exception ]
	.org 0xa00
	l.j itlb_miss_exception_handler
	l.nop

	//[ 0xb00: Range exception ]
	.org 0xb00
	l.j range_exception_handler
	l.nop

	//[ 0xc00: Syscall exception ]
	.org 0xc00
	l.j syscall_exception_handler
	l.nop

	//[ 0xd00: FPU exception ]
	.org 0xd00
	l.j fpu_exception_handler
	l.nop

	//[ 0xe00: Trap exception ]
	.org 0xe00
	l.j trap_exception_handler
	l.nop

	//======[ entry ]=====
	.section .text
reset_handler:
	//clear general registers
	l.movhi r1,  0
	l.movhi r2,  0
	l.movhi r3,  0
	l.movhi r4,  0
	l.movhi r5,  0
	l.movhi r6,  0
	l.movhi r7,  0
	l.movhi r8,  0
	l.movhi r9,  0
	l.movhi r10, 0
	l.movhi r11, 0
	l.movhi r12, 0
	l.movhi r13, 0
	l.movhi r14, 0
	l.movhi r15, 0
	l.movhi r16, 0
	l.movhi r17, 0
	l.movhi r18, 0
	l.movhi r19, 0
	l.movhi r20, 0
	l.movhi r21, 0
	l.movhi r22, 0
	l.movhi r23, 0
	l.movhi r24, 0
	l.movhi r25, 0
	l.movhi r26, 0
	l.movhi r27, 0
	l.movhi r28, 0
	l.movhi r29, 0
	l.movhi r30, 0
	l.movhi r31, 0

	//Clear status register, set supervisor mode and enable Little Endian
	l.ori r1, r0, SPR_SR_SM
	l.ori r1, r1, SPR_SR_LEE
	l.mtspr r0, r1, SPR_SR

	//Clear timer
	l.mtspr r0, r0, SPR_TTMR

	//Stack initilization
	load32i r1, stack_base_addr
	l.addi  r2, r0, -3
	l.and   r1, r1, r2

	//Cache initialize,
	//enable i-cache, d-cache can't been used.
	//by sunny at 2012-5-16 19:48:51.
	//...
	//disable i-cache
	l.mfspr r6, r0, SPR_SR
	l.addi  r5, r0, -1
	l.xori  r5, r5, SPR_SR_ICE
	l.and   r5, r6, r5
	l.mtspr r0, r5, SPR_SR

	//invalidate i-cache
	l.addi  r14, r0, ICACHE_BLOCK_SIZE
	l.addi  r6,  r0, 0
	l.addi  r5,  r0, ICACHE_SIZE
invalid_block:
	l.mtspr r0, r6, SPR_ICBIR
	l.sfne  r6, r5
	l.bf    invalid_block
	l.add   r6, r6, r14

	//enable i-cache
//  l.mfspr r6, r0, SPR_SR
//  l.ori   r6, r6, SPR_SR_ICE
//  l.mtspr r0, r6, SPR_SR
	l.nop
	l.nop
	l.nop
	l.nop
	l.nop
	l.nop
	l.nop
	l.nop
	l.nop
	l.nop
	l.nop

	//Clear BSS
	load32i     r5, bss_start_addr
	load32i     r6, bss_end_addr
1:
	l.sw    (0)(r5), r0
	l.sfltu r5, r6
	l.bf    1b
	l.addi  r5, r5, 4

	//jump to main program entry point
	l.jal   startup_entry
	l.nop

	//If program exits, call exit routine
	l.addi  r3, r11, 0
//	l.jal   exit
	l.nop

	//[ 0x200: BUS exception ]
bus_error_exception_handler:
	EXCEPTION_HANDLER(2)

	//[ 0x300: Data Page Fault exception ]
data_page_fault_exception_handler:
	EXCEPTION_HANDLER(3)

	//[ 0x400: Insn Page Fault exception ]
insn_page_fault_exception_handler:
	EXCEPTION_HANDLER(4)

	//[ 0x500: Timer exception ]
timer_exception_handler:
	EXCEPTION_HANDLER(5)

	//[ 0x600: Aligment exception ]
aligment_exception_handler:
	EXCEPTION_HANDLER(6)

	//[ 0x700: Illegal insn exception ]
illegal_exception_handler:
	EXCEPTION_HANDLER(7)

	//[ 0x800: External interrupt exception ]
irq_exception_handler:
	EXCEPTION_HANDLER(8)

	//[ 0x900: DTLB miss exception ]
dtlb_miss_exception_handler:
	EXCEPTION_HANDLER(9)

	//[ 0xa00: ITLB miss exception ]
itlb_miss_exception_handler:
	EXCEPTION_HANDLER(10)

	//[ 0xb00: Range exception ]
range_exception_handler:
	EXCEPTION_HANDLER(11)

	//[ 0xc00: Syscall exception ]
syscall_exception_handler:
	EXCEPTION_HANDLER(12)

	//[ 0xd00: FPU exception ]
fpu_exception_handler:
	EXCEPTION_HANDLER(13)

	//[ 0xe00: Trap exception ]
trap_exception_handler:
	EXCEPTION_HANDLER(14)

	//------------------------------------------------------
	//      [ the entry of exception handler ]
	//------------------------------------------------------
cpu_exception_handler_entry:
	l.sw    0x00(r1), r2
	l.sw    0x10(r1), r6
	l.sw    0x14(r1), r7
	l.sw    0x18(r1), r8
	l.sw    0x1c(r1), r9
	l.sw    0x20(r1), r10
	l.sw    0x24(r1), r11
	l.sw    0x28(r1), r12
	l.sw    0x2c(r1), r13
	l.sw    0x30(r1), r14
	l.sw    0x34(r1), r15
	l.sw    0x38(r1), r16
	l.sw    0x3c(r1), r17
	l.sw    0x40(r1), r18
	l.sw    0x44(r1), r19
	l.sw    0x48(r1), r20
	l.sw    0x4c(r1), r21
	l.sw    0x50(r1), r22
	l.sw    0x54(r1), r23
	l.sw    0x58(r1), r24
	l.sw    0x5c(r1), r25
	l.sw    0x60(r1), r26
	l.sw    0x64(r1), r27
	l.sw    0x68(r1), r28
	l.sw    0x6c(r1), r29
	l.sw    0x70(r1), r30
	l.sw    0x74(r1), r31

	l.jal   cpu_exception_handler_entry_c
	l.nop

	l.lwz    r2,  0x00(r1)
	l.lwz    r3,  0x04(r1)
	l.lwz    r4,  0x08(r1)
	l.lwz    r5,  0x0c(r1)
	l.lwz    r6,  0x10(r1)
	l.lwz    r7,  0x14(r1)
	l.lwz    r8,  0x18(r1)
	l.lwz    r9,  0x1c(r1)
	l.lwz    r10, 0x20(r1)
	l.lwz    r11, 0x24(r1)
	l.lwz    r12, 0x28(r1)
	l.lwz    r13, 0x2c(r1)
	l.lwz    r14, 0x30(r1)
	l.lwz    r15, 0x34(r1)
	l.lwz    r16, 0x38(r1)
	l.lwz    r17, 0x3c(r1)
	l.lwz    r18, 0x40(r1)
	l.lwz    r19, 0x44(r1)
	l.lwz    r20, 0x48(r1)
	l.lwz    r21, 0x4c(r1)
	l.lwz    r22, 0x50(r1)
	l.lwz    r23, 0x54(r1)
	l.lwz    r24, 0x58(r1)
	l.lwz    r25, 0x5c(r1)
	l.lwz    r26, 0x60(r1)
	l.lwz    r27, 0x64(r1)
	l.lwz    r28, 0x68(r1)
	l.lwz    r29, 0x6c(r1)
	l.lwz    r30, 0x70(r1)
	l.lwz    r31, 0x74(r1)

	l.addi  r1, r1, EXCEPTION_STACK_SIZE

	l.rfe
	l.nop
