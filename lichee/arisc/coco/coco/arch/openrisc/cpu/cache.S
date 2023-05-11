/*
*********************************************************************************************************
*                                                AR100 SYSTEM
*                                     AR100 Software System Develop Kits
*                                                CACHE OPERATIONS
*
*                                    (c) Copyright 2012-2016, Superm China
*                                             All Rights Reserved
*
* File    : cache.S
* By      : Superm
* Version : v1.0
* Date    : 2013-04-10
* Descript: Assembly cache operations.
* Update  : date                auther      ver     notes
*           2013-04-10 15:50:27 Superm      1.0     Create this file.
*********************************************************************************************************
*/
#include "spr_defines.h"
#include "cache.h"

#define ENTRY(symbol)    \
		.align  4       ;\
		.global symbol  ;\
symbol:

ENTRY(icache_coherent)
	l.sw 0xfffffffc(r1),r9
	l.sw 0xfffffff8(r1),r3
	l.sw 0xfffffff4(r1),r4
	l.sw 0xfffffff0(r1),r5
	//memory Syncronization
	l.msync
	//Context Syncronization
	l.csync
	//invalidate i-cache
	l.addi  r3, r0, ICACHE_BLOCK_SIZE
	l.addi  r4,  r0, 0
	l.addi  r5,  r0, ICACHE_SIZE
invalidate_block:
	l.mtspr r0, r4, SPR_ICBIR
	l.sfne  r4, r5
	l.bf    invalidate_block
	l.add   r4, r4, r3
	//pipeline Syncronization
	l.psync
	l.lwz r9,0xfffffffc(r1)
	l.lwz r3,0xfffffff8(r1)
	l.lwz r4,0xfffffff4(r1)
	l.lwz r5,0xfffffff0(r1)
	l.jr    r9
	l.nop   0x0
