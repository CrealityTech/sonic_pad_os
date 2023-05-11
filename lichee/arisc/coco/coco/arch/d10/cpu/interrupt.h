#ifndef INTERRUPT_H
#define INTERRUPT_H

	.macro gie_disable
		setgie.d
		dsb
	.endm

	.macro gie_enable
		setgie.e
	.endm

	/* align $sp to 8B boundary */
	.macro align_sp_8 R0="$r2", R1="$r3"
		move    \R0, $sp        !keep original $sp to be pushed
#if !defined(__NDS32_ISA_V3M__) || 1 //could be optimized out
 #ifndef __NDS32_EXT_PERF__
		andi    \R1, $sp, #4    ! R1 = $sp.bit2 // 0 or 4
		subri   \R1, \R1, #4    ! R1 = 4 - R1   // 4 or 0
		sub     $sp, $sp, \R1   ! $sp -= R1     //-4 or 0
		push    \R0
 #else
		addi    $sp, $sp, #-4   ! $sp -= 4
		bclr    $sp, $sp, #2    ! $sp.bit2 = 0
		swi     \R0, [$sp]
 #endif
#endif
	.endm

	.macro push_d0d1 R0="$r2", R1="$r3", R2="$r4", R3="$r5"
#ifdef __NDS32_DX_REGS__
		mfusr   \R0, $d1.lo
		mfusr   \R1, $d1.hi
		mfusr   \R2, $d0.lo
		mfusr   \R3, $d0.hi
		pushm   \R0, \R3
#endif
	.endm

	.macro pop_d0d1 R0="$r2", R1="$r3", R2="$r4", R3="$r5"
#ifdef __NDS32_DX_REGS__
		popm    \R0, \R3
		mtusr   \R0, $d1.lo
		mtusr   \R1, $d1.hi
		mtusr   \R2, $d0.lo
		mtusr   \R3, $d0.hi
#endif
	.endm

	.macro push_ifc_lp R0="$r2"
#ifdef __NDS32_EXT_IFC__
		mfusr   \R0, $IFC_LP
		push    \R0
#endif
	.endm

	.macro pop_ifc_lp R0="$r2"
#ifdef __NDS32_EXT_IFC__
	pop     \R0
	mtusr   \R0, $IFC_LP
#endif
	.endm

	.macro push_zol R0="$r2", R1="$r3", R2="$r4"
#ifdef CFG_HWZOL
		mfusr   \R0, $LB
		mfusr   \R1, $LE
		mfusr   \R2, $LC
		pushm   \R0, \R2
#endif
	.endm

	.macro pop_zol R0="$r2", R1="$r3", R2="$r4"
#ifdef CFG_HWZOL
		popm    \R0, \R2
		mtusr   \R0, $LB
		mtusr   \R1, $LE
		mtusr   \R2, $LC
#endif
	.endm

#ifdef __NDS32_ABI_2FP_PLUS__
	/* New FPU ABI */
	.macro SAVE_FPU_REGS_00
		addi    $sp, $sp, -8
		fsdi.bi $fd2, [$sp], -8
		fsdi.bi $fd1, [$sp], -8
		fsdi    $fd0, [$sp+0]
	.endm

	.macro SAVE_FPU_REGS_01
		SAVE_FPU_REGS_00
	.endm

	.macro SAVE_FPU_REGS_02
		addi    $sp, $sp, -8
		fsdi.bi $fd15, [$sp], -8
		fsdi.bi $fd14, [$sp], -8
		fsdi.bi $fd13, [$sp], -8
		fsdi.bi $fd12, [$sp], -8
		fsdi.bi $fd11, [$sp], -8
		fsdi.bi $fd2, [$sp], -8
		fsdi.bi $fd1, [$sp], -8
		fsdi    $fd0, [$sp+0]
	.endm

	.macro SAVE_FPU_REGS_03
		addi    $sp, $sp, -8
		fsdi.bi $fd23, [$sp], -8
		fsdi.bi $fd22, [$sp], -8
		fsdi.bi $fd21, [$sp], -8
		fsdi.bi $fd20, [$sp], -8
		fsdi.bi $fd19, [$sp], -8
		fsdi.bi $fd18, [$sp], -8
		fsdi.bi $fd17, [$sp], -8
		fsdi.bi $fd16, [$sp], -8
		fsdi.bi $fd15, [$sp], -8
		fsdi.bi $fd14, [$sp], -8
		fsdi.bi $fd13, [$sp], -8
		fsdi.bi $fd12, [$sp], -8
		fsdi.bi $fd11, [$sp], -8
		fsdi.bi $fd2, [$sp], -8
		fsdi.bi $fd1, [$sp], -8
		fsdi    $fd0, [$sp+0]
	.endm
#else
	.macro SAVE_FPU_REGS_00
		addi    $sp, $sp, -8
		fsdi.bi $fd2, [$sp], -8
		fsdi.bi $fd1, [$sp], -8
		fsdi    $fd0, [$sp+0]
	.endm

	.macro SAVE_FPU_REGS_01
		addi    $sp, $sp, -8
		fsdi.bi $fd6, [$sp], -8
		fsdi.bi $fd4, [$sp], -8
		fsdi.bi $fd2, [$sp], -8
		fsdi.bi $fd1, [$sp], -8
		fsdi    $fd0, [$sp+0]
	.endm

	.macro SAVE_FPU_REGS_02
		addi    $sp, $sp, -8
		fsdi.bi $fd14, [$sp], -8
		fsdi.bi $fd12, [$sp], -8
		fsdi.bi $fd10, [$sp], -8
		fsdi.bi $fd8, [$sp], -8
		fsdi.bi $fd6, [$sp], -8
		fsdi.bi $fd4, [$sp], -8
		fsdi.bi $fd2, [$sp], -8
		fsdi.bi $fd1, [$sp], -8
		fsdi    $fd0, [$sp+0]
	.endm

	.macro SAVE_FPU_REGS_03
		addi    $sp, $sp, -8
		fsdi.bi $fd30, [$sp], -8
		fsdi.bi $fd28, [$sp], -8
		fsdi.bi $fd26, [$sp], -8
		fsdi.bi $fd24, [$sp], -8
		fsdi.bi $fd22, [$sp], -8
		fsdi.bi $fd20, [$sp], -8
		fsdi.bi $fd18, [$sp], -8
		fsdi.bi $fd16, [$sp], -8
		fsdi.bi $fd14, [$sp], -8
		fsdi.bi $fd12, [$sp], -8
		fsdi.bi $fd10, [$sp], -8
		fsdi.bi $fd8, [$sp], -8
		fsdi.bi $fd6, [$sp], -8
		fsdi.bi $fd4, [$sp], -8
		fsdi.bi $fd2, [$sp], -8
		fsdi.bi $fd1, [$sp], -8
		fsdi    $fd0, [$sp+0]
	.endm
#endif

	.macro push_fpu
#if defined(__NDS32_EXT_FPU_CONFIG_0__)
		SAVE_FPU_REGS_00
#elif defined(__NDS32_EXT_FPU_CONFIG_1__)
		SAVE_FPU_REGS_01
#elif defined(__NDS32_EXT_FPU_CONFIG_2__)
		SAVE_FPU_REGS_02
#elif defined(__NDS32_EXT_FPU_CONFIG_3__)
		SAVE_FPU_REGS_03
#else
#endif
	.endm

#ifdef __NDS32_ABI_2FP_PLUS__
        /* New FPU ABI */
	.macro RESTORE_FPU_REGS_00
		fldi.bi $fd0, [$sp], 8
		fldi.bi $fd1, [$sp], 8
		fldi.bi $fd2, [$sp], 8
	.endm

	.macro RESTORE_FPU_REGS_01
		RESTORE_FPU_REGS_00
	.endm

	.macro RESTORE_FPU_REGS_02
		fldi.bi $fd0, [$sp], 8
		fldi.bi $fd1, [$sp], 8
		fldi.bi $fd2, [$sp], 8
		fldi.bi $fd11, [$sp], 8
		fldi.bi $fd12, [$sp], 8
		fldi.bi $fd13, [$sp], 8
		fldi.bi $fd14, [$sp], 8
		fldi.bi $fd15, [$sp], 8
	.endm

	.macro RESTORE_FPU_REGS_03
		fldi.bi $fd0, [$sp], 8
		fldi.bi $fd1, [$sp], 8
		fldi.bi $fd2, [$sp], 8
		fldi.bi $fd11, [$sp], 8
		fldi.bi $fd12, [$sp], 8
		fldi.bi $fd13, [$sp], 8
		fldi.bi $fd14, [$sp], 8
		fldi.bi $fd15, [$sp], 8
		fldi.bi $fd16, [$sp], 8
		fldi.bi $fd17, [$sp], 8
		fldi.bi $fd18, [$sp], 8
		fldi.bi $fd19, [$sp], 8
		fldi.bi $fd20, [$sp], 8
		fldi.bi $fd21, [$sp], 8
		fldi.bi $fd22, [$sp], 8
		fldi.bi $fd23, [$sp], 8
	.endm
#else
	.macro RESTORE_FPU_REGS_00
		fldi.bi $fd0, [$sp], 8
		fldi.bi $fd1, [$sp], 8
		fldi.bi $fd2, [$sp], 8
	.endm

	.macro RESTORE_FPU_REGS_01
		fldi.bi $fd0, [$sp], 8
		fldi.bi $fd1, [$sp], 8
		fldi.bi $fd2, [$sp], 8
		fldi.bi $fd4, [$sp], 8
		fldi.bi $fd6, [$sp], 8
	.endm

	.macro RESTORE_FPU_REGS_02
		fldi.bi $fd0, [$sp], 8
		fldi.bi $fd1, [$sp], 8
		fldi.bi $fd2, [$sp], 8
		fldi.bi $fd4, [$sp], 8
		fldi.bi $fd6, [$sp], 8
		fldi.bi $fd8, [$sp], 8
		fldi.bi $fd10, [$sp], 8
		fldi.bi $fd12, [$sp], 8
		fldi.bi $fd14, [$sp], 8
	.endm

	.macro RESTORE_FPU_REGS_03
		fldi.bi $fd0, [$sp], 8
		fldi.bi $fd1, [$sp], 8
		fldi.bi $fd2, [$sp], 8
		fldi.bi $fd4, [$sp], 8
		fldi.bi $fd6, [$sp], 8
		fldi.bi $fd8, [$sp], 8
		fldi.bi $fd10, [$sp], 8
		fldi.bi $fd12, [$sp], 8
		fldi.bi $fd14, [$sp], 8
		fldi.bi $fd16, [$sp], 8
		fldi.bi $fd18, [$sp], 8
		fldi.bi $fd20, [$sp], 8
		fldi.bi $fd22, [$sp], 8
		fldi.bi $fd24, [$sp], 8
		fldi.bi $fd26, [$sp], 8
		fldi.bi $fd28, [$sp], 8
		fldi.bi $fd30, [$sp], 8
	.endm
#endif

	.macro pop_fpu
#if defined(__NDS32_EXT_FPU_CONFIG_0__)
		RESTORE_FPU_REGS_00
#elif defined(__NDS32_EXT_FPU_CONFIG_1__)
		RESTORE_FPU_REGS_01
#elif defined(__NDS32_EXT_FPU_CONFIG_2__)
		RESTORE_FPU_REGS_02
#elif defined(__NDS32_EXT_FPU_CONFIG_3__)
		RESTORE_FPU_REGS_03
#else
#endif
	.endm


	.macro SAVE_ALL
		pushm   $r0, $r5
		pushm   $r15,$r30
		push_d0d1
		push_ifc_lp
		push_zol
		mfsr    $r0, $IPC
		mfsr    $r1, $IPSW
		pushm   $r0, $r1

		/* Descend interrupt level */
		mfsr    $r0, $PSW
#ifdef CFG_HWZOL
		/* Also enable ZOL (PSW.AEN) */
		xori    $r0, $r0, #((1 << 13) | (1 << 1))
#else
		addi    $r0, $r0, #-2
#endif
		mtsr    $r0, $PSW
		dsb

		align_sp_8
		push_fpu
	.endm

	.macro RESTORE_ALL
		pop_fpu
		lwi     $sp, [$sp]
		popm    $r0, $r1
		mtsr    $r0, $IPC
		mtsr    $r1, $IPSW
		pop_zol
		pop_ifc_lp
		pop_d0d1
		popm    $r15,$r30
		popm    $r0, $r5
	.endm

	.macro SYSCALL_SAVE_ALL
		pushm   $r1, $r9                    ! $r0 for return value,
		pushm   $r15,$r30                   ! $r0 ~ $r5 are arguments, and
		push_d0d1   "$r6","$r7","$r8","$r9" ! saved them in case ABI are not followed
		push_ifc_lp "$r6"                   ! $r6 ~ $r9 are clobber-list
		push_zol    "$r6","$r7","$r8"
		mfsr    $r6, $IPC
		addi    $r6, $r6, 4
		mfsr    $r7, $IPSW
		pushm   $r6, $r7                    ! save $ipc, $ipsw

		/* Descend interrupt level */
		mfsr    $r6, $PSW
#ifdef CFG_HWZOL
		/* Also enable ZOL (PSW.AEN) */
		xori    $r6, $r6, #((1 << 13) | (1 << 1))
#else
		addi    $r6, $r6, #-2
#endif
		mtsr    $r6, $PSW
		dsb

		align_sp_8 "$r6","$r7"
		push_fpu
	.endm

	.macro SYSCALL_RESTORE_ALL
		pop_fpu
		lwi     $sp, [$sp]      ! restore align_sp_8
		popm    $r6, $r7        ! restore $ipc, $ipsw
		mtsr    $r6, $IPC
		mtsr    $r7, $IPSW
		pop_zol    "$r6","$r7","$r8"
		pop_ifc_lp "$r6"
		pop_d0d1   "$r6","$r7","$r8","$r9"
		popm    $r15,$r30
		popm    $r1, $r9
	.endm

	.macro SAVE_ALL_HW0
		pushm   $r0, $r5
		pushm   $r15,$r30
		push_d0d1
		push_ifc_lp
		push_zol

		align_sp_8
		push_fpu
	.endm

	.macro RESTORE_ALL_HW0
		pop_fpu
		lwi     $sp, [$sp]
		pop_zol
		pop_ifc_lp
		pop_d0d1
		popm    $r15,$r30
		popm    $r0, $r5
	.endm

	.macro SAVE_ALL_HW      !should not touch $r0
		/* push caller-saved gpr */
		pushm	$r1, $r5
		pushm	$r15, $r30
		push_d0d1
		push_ifc_lp
		push_zol

#if !(defined(__NDS32_ISA_V3M__) || defined(__NDS32_ISA_V3__))
		/* push $INT_MASK */
		mfsr    $r1, $INT_MASK
		push    $r1             ! push $INT_MASK

		/* disable low priority and
		 * enable high priority interrupt */
		ori $r1, $r1, 0x3f  ! r1= {mask[31:6],000000}
		li  $r2, 1
		sll $r2, $r2, $r0   ! 1<<r0
		subri $r2, $r2, 64  ! 64 - (1 << r0)
		sub $r1, $r1, $r2   ! {mask[31:6],000000} - (64- (1<<r0))

		mtsr $r1, $INT_MASK
#endif

		/* push $IPC, $IPSW */
		mfsr    $r1, $IPC
		mfsr    $r2, $IPSW
		pushm   $r1, $r2

		/* You can use -1 if you want to
		 * descend interrupt level and enable gie or
		 * you can enable gie when you finish your top
		 * half isr. */
		/* Descend interrupt level */
		mfsr    $r1, $PSW
#ifdef CFG_HWZOL
		/* Also enable ZOL (PSW.AEN) */
		xori    $r1, $r1, #((1 << 13) | (1 << 1))
#else
		addi    $r1, $r1, #-2
#endif
		mtsr    $r1, $PSW

		align_sp_8
		push_fpu
	.endm

	.macro RESTORE_ALL_HW
		/* pop fpu*/
		pop_fpu
		lwi $sp, [$sp]

		/* pop $IPC, $IPSW*/
		popm    $r0, $r1
		gie_disable
		mtsr    $r0, $IPC
		mtsr    $r1, $IPSW

#if !(defined(__NDS32_ISA_V3M__) || defined(__NDS32_ISA_V3__))
		/* pop $INT_MASK */
		pop     $r0             ! get $INT_MASK
		mtsr    $r0, $INT_MASK  ! restore $INT_MASK
#endif
		pop_zol
		pop_ifc_lp
		/* pop d0d1 */
		pop_d0d1

		/* pop gpr */
		popm    $r15,$r30
		popm    $r1, $r5
		pop     $r0
	.endm

#endif //#ifndef INTERRUPT_H
