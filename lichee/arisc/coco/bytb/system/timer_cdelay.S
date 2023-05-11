#define ENTRY(symbol)    \
		.align  4       ;\
		.global symbol  ;\
symbol:

ENTRY(__time_cdelay)
delay_loop:
	l.sfnei r3, 0x0
	l.bf    delay_loop
	l.addi  r3, r3, 0xffffffff
	l.addi  r3, r3, 0x1
	l.jr    r9
	l.nop   0x0
