/*
*********************************************************************************************************
*                                                AR100 SYSTEM
*                                     AR100 Software System Develop Kits
*                                                TIME DELAY
*
*                                    (c) Copyright 2012-2016, Sunny China
*                                             All Rights Reserved
*
* File    : time_cdelay.S
* By      : Sunny
* Version : v1.0
* Date    : 2012-12-25
* Descript: Assembly time_cdelay.
* Update  : date                auther      ver     notes
*           2012-12-25 15:50:27 Sunny       1.0     Create this file.
*********************************************************************************************************
*/
#define ENTRY(symbol)    \
		.align  4       ;\
		.global symbol  ;\
symbol:

ENTRY(time_cdelay)
delay_loop:
	l.sfnei r3, 0x0
	l.bf    delay_loop
	l.addi  r3, r3, 0xffffffff
	l.addi  r3, r3, 0x1
	l.jr    r9
	l.nop   0x0
