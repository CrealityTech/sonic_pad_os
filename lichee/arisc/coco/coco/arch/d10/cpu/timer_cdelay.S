
#define ENTRY(symbol)    \
		.align  4       ;\
		.global symbol  ;\
symbol:

ENTRY(time_cdelay)

timer_cdelay:
    subi333 $r0,$r0,#0x1
    bnez38 $r0,timer_cdelay
    ret5 $lp
/*
00002f46<dddelay>:
   2f46:	fc 01       	push25 $r6,#8    ! {$r6, $fp, $gp, $lp}
   2f48:	f0 81       	swi37.sp $r0,[+#0x4]
   2f4a:	f0 01       	lwi37.sp $r0,[+#0x4]
   2f4c:	9e 41       	subi333 $r1,$r0,#0x1
   2f4e:	f1 81       	swi37.sp $r1,[+#0x4]
   2f50:	c8 fd       	bnez38 $r0,2f4a <dddelay+0x4>
   2f52:	fc 81       	pop25 $r6,#8    ! {$r6, $fp, $gp, $lp}
   */
