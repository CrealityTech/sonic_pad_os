/*
 * Copyright 2000-2019
 * SPDX-License-Identifier: GPL-2.0
 */
	.arch armv7-a
	.eabi_attribute 20, 1
	.eabi_attribute 21, 1
	.eabi_attribute 23, 3
	.eabi_attribute 24, 1
	.eabi_attribute 25, 1
	.eabi_attribute 26, 2
	.eabi_attribute 30, 4
	.eabi_attribute 34, 0
	.eabi_attribute 18, 2
	.file	"nftl.c"
	.section	.text._nand_sector_read,"ax",%progbits
	.align	1
	.global	_nand_sector_read
	.syntax unified
	.thumb
	.thumb_func
	.fpu softvfp
	.type	_nand_sector_read, %function
_nand_sector_read:
	@ args = 0, pretend = 0, frame = 0
	@ frame_needed = 0, uses_anonymous_args = 0
	@ link register save eliminated.
	movs	r0, #0
	bx	lr
	.size	_nand_sector_read, .-_nand_sector_read
	.section	.text._nand_flush_sector_write_cache,"ax",%progbits
	.align	1
	.global	_nand_flush_sector_write_cache
	.syntax unified
	.thumb
	.thumb_func
	.fpu softvfp
	.type	_nand_flush_sector_write_cache, %function
_nand_flush_sector_write_cache:
	@ args = 0, pretend = 0, frame = 0
	@ frame_needed = 0, uses_anonymous_args = 0
	@ link register save eliminated.
	movs	r0, #0
	bx	lr
	.size	_nand_flush_sector_write_cache, .-_nand_flush_sector_write_cache
	.section	.text._shutdown_op,"ax",%progbits
	.align	1
	.global	_shutdown_op
	.syntax unified
	.thumb
	.thumb_func
	.fpu softvfp
	.type	_shutdown_op, %function
_shutdown_op:
	@ args = 0, pretend = 0, frame = 0
	@ frame_needed = 0, uses_anonymous_args = 0
	@ link register save eliminated.
	movs	r0, #0
	bx	lr
	.size	_shutdown_op, .-_shutdown_op
	.section	.text._nftl_nand_erase_superblk,"ax",%progbits
	.align	1
	.global	_nftl_nand_erase_superblk
	.syntax unified
	.thumb
	.thumb_func
	.fpu softvfp
	.type	_nftl_nand_erase_superblk, %function
_nftl_nand_erase_superblk:
	@ args = 0, pretend = 0, frame = 16
	@ frame_needed = 0, uses_anonymous_args = 0
	push	{r0, r1, r2, r3, r4, lr}
	ldr	r3, [r0]
	ldr	r0, [r3, #4]
	ldrh	r3, [r1, #2]
	strh	r3, [sp, #2]	@ movhi
	ldrh	r3, [r1]
	strh	r3, [sp]	@ movhi
	ldrh	r3, [r1, #4]
	strh	r3, [sp, #4]	@ movhi
	ldr	r3, [r1, #8]
	str	r3, [sp, #8]
	ldr	r3, [r1, #12]
	mov	r1, sp
	str	r3, [sp, #12]
	ldr	r3, [r0, #84]
	blx	r3
	add	sp, sp, #20
	@ sp needed
	ldr	pc, [sp], #4
	.size	_nftl_nand_erase_superblk, .-_nftl_nand_erase_superblk
	.section	.text._nftl_nand_is_blk_good,"ax",%progbits
	.align	1
	.global	_nftl_nand_is_blk_good
	.syntax unified
	.thumb
	.thumb_func
	.fpu softvfp
	.type	_nftl_nand_is_blk_good, %function
_nftl_nand_is_blk_good:
	@ args = 0, pretend = 0, frame = 16
	@ frame_needed = 0, uses_anonymous_args = 0
	push	{r0, r1, r2, r3, r4, lr}
	ldr	r3, [r0]
	ldr	r0, [r3, #4]
	ldrh	r3, [r1, #2]
	strh	r3, [sp, #2]	@ movhi
	ldrh	r3, [r1]
	strh	r3, [sp]	@ movhi
	ldrh	r3, [r1, #4]
	strh	r3, [sp, #4]	@ movhi
	ldr	r3, [r1, #8]
	str	r3, [sp, #8]
	ldr	r3, [r1, #12]
	mov	r1, sp
	str	r3, [sp, #12]
	ldr	r3, [r0, #96]
	blx	r3
	add	sp, sp, #20
	@ sp needed
	ldr	pc, [sp], #4
	.size	_nftl_nand_is_blk_good, .-_nftl_nand_is_blk_good
	.section	.text._nftl_nand_mark_bad_blk,"ax",%progbits
	.align	1
	.global	_nftl_nand_mark_bad_blk
	.syntax unified
	.thumb
	.thumb_func
	.fpu softvfp
	.type	_nftl_nand_mark_bad_blk, %function
_nftl_nand_mark_bad_blk:
	@ args = 0, pretend = 0, frame = 16
	@ frame_needed = 0, uses_anonymous_args = 0
	push	{r0, r1, r2, r3, r4, lr}
	ldr	r3, [r0]
	ldr	r0, [r3, #4]
	ldrh	r3, [r1, #2]
	strh	r3, [sp, #2]	@ movhi
	ldrh	r3, [r1]
	strh	r3, [sp]	@ movhi
	ldrh	r3, [r1, #4]
	strh	r3, [sp, #4]	@ movhi
	ldr	r3, [r1, #8]
	str	r3, [sp, #8]
	ldr	r3, [r1, #12]
	mov	r1, sp
	str	r3, [sp, #12]
	ldr	r3, [r0, #100]
	blx	r3
	add	sp, sp, #20
	@ sp needed
	ldr	pc, [sp], #4
	.size	_nftl_nand_mark_bad_blk, .-_nftl_nand_mark_bad_blk
	.section	.text.dumphex,"ax",%progbits
	.align	1
	.syntax unified
	.thumb
	.thumb_func
	.fpu softvfp
	.type	dumphex, %function
dumphex:
	@ args = 0, pretend = 0, frame = 0
	@ frame_needed = 0, uses_anonymous_args = 0
	push	{r4, r5, r6, r7, r8, lr}
	mov	r6, r1
	ldr	r7, .L10
	sub	sp, sp, #56
	mov	r4, r0
	movs	r5, #0
	ldr	r8, .L10+8
.L8:
	cmp	r5, r6
	add	r4, r4, #16
	bcc	.L9
	ldr	r0, .L10+4
	add	sp, sp, #56
	@ sp needed
	pop	{r4, r5, r6, r7, r8, lr}
	b	NAND_Print
.L9:
	mov	r1, r5
	mov	r0, r7
	bl	NAND_Print
	ldrb	r0, [r4, #-1]	@ zero_extendqisi2
	adds	r5, r5, #16
	ldrb	r3, [r4, #-14]	@ zero_extendqisi2
	ldrb	r2, [r4, #-15]	@ zero_extendqisi2
	ldrb	r1, [r4, #-16]	@ zero_extendqisi2
	str	r0, [sp, #48]
	ldrb	r0, [r4, #-2]	@ zero_extendqisi2
	str	r0, [sp, #44]
	ldrb	r0, [r4, #-3]	@ zero_extendqisi2
	str	r0, [sp, #40]
	ldrb	r0, [r4, #-4]	@ zero_extendqisi2
	str	r0, [sp, #36]
	ldrb	r0, [r4, #-5]	@ zero_extendqisi2
	str	r0, [sp, #32]
	ldrb	r0, [r4, #-6]	@ zero_extendqisi2
	str	r0, [sp, #28]
	ldrb	r0, [r4, #-7]	@ zero_extendqisi2
	str	r0, [sp, #24]
	ldrb	r0, [r4, #-8]	@ zero_extendqisi2
	str	r0, [sp, #20]
	ldrb	r0, [r4, #-9]	@ zero_extendqisi2
	str	r0, [sp, #16]
	ldrb	r0, [r4, #-10]	@ zero_extendqisi2
	str	r0, [sp, #12]
	ldrb	r0, [r4, #-11]	@ zero_extendqisi2
	str	r0, [sp, #8]
	ldrb	r0, [r4, #-12]	@ zero_extendqisi2
	str	r0, [sp, #4]
	ldrb	r0, [r4, #-13]	@ zero_extendqisi2
	str	r0, [sp]
	mov	r0, r8
	bl	NAND_Print
	b	.L8
.L11:
	.align	2
.L10:
	.word	.LC5
	.word	.LC7
	.word	.LC6
	.size	dumphex, .-dumphex
	.section	.text._nftl_nand_read_page,"ax",%progbits
	.align	1
	.global	_nftl_nand_read_page
	.syntax unified
	.thumb
	.thumb_func
	.fpu softvfp
	.type	_nftl_nand_read_page, %function
_nftl_nand_read_page:
	@ args = 0, pretend = 0, frame = 16
	@ frame_needed = 0, uses_anonymous_args = 0
	push	{r4, r5, lr}
	mov	r4, r0
	ldr	r2, [r4, #1968]
	sub	sp, sp, #20
	ldr	r3, [r0]
	ldr	r0, [r3, #4]
	ldrd	r3, r5, [r2, #80]
	adds	r3, r3, #1
	adc	r5, r5, #0
	strd	r3, r5, [r2, #80]
	ldrh	r3, [r1, #2]
	strh	r3, [sp, #2]	@ movhi
	ldrh	r3, [r1]
	strh	r3, [sp]	@ movhi
	ldrh	r3, [r1, #4]
	strh	r3, [sp, #4]	@ movhi
	ldr	r3, [r1, #8]
	str	r3, [sp, #8]
	ldr	r3, [r1, #12]
	mov	r1, sp
	str	r3, [sp, #12]
	ldr	r3, [r0, #88]
	blx	r3
	adds	r3, r0, #2
	itttt	eq
	ldreq	r2, [r4, #1968]
	ldreq	r3, [r2, #132]
	addeq	r3, r3, #1
	streq	r3, [r2, #132]
	add	sp, sp, #20
	@ sp needed
	pop	{r4, r5, pc}
	.size	_nftl_nand_read_page, .-_nftl_nand_read_page
	.section	.text._nftl_nand_write_page,"ax",%progbits
	.align	1
	.global	_nftl_nand_write_page
	.syntax unified
	.thumb
	.thumb_func
	.fpu softvfp
	.type	_nftl_nand_write_page, %function
_nftl_nand_write_page:
	@ args = 0, pretend = 0, frame = 16
	@ frame_needed = 0, uses_anonymous_args = 0
	push	{r4, r5, lr}
	mov	r4, r0
	ldr	r3, [r0]
	sub	sp, sp, #20
	ldr	r2, [r4, #1968]
	ldr	r0, [r3, #4]
	ldrd	r3, r5, [r2, #88]
	adds	r3, r3, #1
	adc	r5, r5, #0
	strd	r3, r5, [r2, #88]
	ldrh	r3, [r1, #2]
	strh	r3, [sp, #2]	@ movhi
	ldrh	r3, [r1]
	strh	r3, [sp]	@ movhi
	ldrh	r3, [r1, #4]
	strh	r3, [sp, #4]	@ movhi
	ldr	r3, [r1, #8]
	str	r3, [sp, #8]
	ldr	r3, [r1, #12]
	mov	r1, sp
	str	r3, [sp, #12]
	ldr	r3, [r0, #92]
	blx	r3
	cbz	r0, .L14
	ldr	r2, [r4, #1968]
	ldr	r3, [r2, #136]
	adds	r3, r3, #1
	str	r3, [r2, #136]
.L14:
	add	sp, sp, #20
	@ sp needed
	pop	{r4, r5, pc}
	.size	_nftl_nand_write_page, .-_nftl_nand_write_page
	.section	.text._nand_sector_write,"ax",%progbits
	.align	1
	.global	_nand_sector_write
	.syntax unified
	.thumb
	.thumb_func
	.fpu softvfp
	.type	_nand_sector_write, %function
_nand_sector_write:
	@ args = 0, pretend = 0, frame = 0
	@ frame_needed = 0, uses_anonymous_args = 0
	@ link register save eliminated.
	movs	r0, #0
	bx	lr
	.size	_nand_sector_write, .-_nand_sector_write
	.section	.text.nftl_malloc,"ax",%progbits
	.align	1
	.global	nftl_malloc
	.syntax unified
	.thumb
	.thumb_func
	.fpu softvfp
	.type	nftl_malloc, %function
nftl_malloc:
	@ args = 0, pretend = 0, frame = 0
	@ frame_needed = 0, uses_anonymous_args = 0
	cmp	r0, #1572864
	push	{r4, lr}
	mov	r4, r0
	bls	.L21
	mov	r1, r0
	ldr	r0, .L22
	bl	NAND_Print
.L21:
	mov	r0, r4
	pop	{r4, lr}
	b	NAND_Malloc
.L23:
	.align	2
.L22:
	.word	.LC8
	.size	nftl_malloc, .-nftl_malloc
	.section	.text.nftl_free,"ax",%progbits
	.align	1
	.global	nftl_free
	.syntax unified
	.thumb
	.thumb_func
	.fpu softvfp
	.type	nftl_free, %function
nftl_free:
	@ args = 0, pretend = 0, frame = 0
	@ frame_needed = 0, uses_anonymous_args = 0
	@ link register save eliminated.
	movs	r1, #0
	b	NAND_Free
	.size	nftl_free, .-nftl_free
	.section	.text.add_nftl_blk_list,"ax",%progbits
	.align	1
	.global	add_nftl_blk_list
	.syntax unified
	.thumb
	.thumb_func
	.fpu softvfp
	.type	add_nftl_blk_list, %function
add_nftl_blk_list:
	@ args = 0, pretend = 0, frame = 0
	@ frame_needed = 0, uses_anonymous_args = 0
	@ link register save eliminated.
	movs	r3, #0
	str	r3, [r1, #8]
.L26:
	ldr	r3, [r0, #8]
	cbnz	r3, .L27
	str	r1, [r0, #8]
	bx	lr
.L27:
	mov	r0, r3
	b	.L26
	.size	add_nftl_blk_list, .-add_nftl_blk_list
	.section	.text.del_last_nftl_blk,"ax",%progbits
	.align	1
	.global	del_last_nftl_blk
	.syntax unified
	.thumb
	.thumb_func
	.fpu softvfp
	.type	del_last_nftl_blk, %function
del_last_nftl_blk:
	@ args = 0, pretend = 0, frame = 0
	@ frame_needed = 0, uses_anonymous_args = 0
	@ link register save eliminated.
.L29:
	ldr	r3, [r0, #8]
	cbz	r3, .L28
	ldr	r2, [r3, #8]
	cbnz	r2, .L32
	str	r2, [r0, #8]
.L28:
	mov	r0, r3
	bx	lr
.L32:
	mov	r0, r3
	b	.L29
	.size	del_last_nftl_blk, .-del_last_nftl_blk
	.section	.text.nftl_support_boot_check_crc_from_dts,"ax",%progbits
	.align	1
	.global	nftl_support_boot_check_crc_from_dts
	.syntax unified
	.thumb
	.thumb_func
	.fpu softvfp
	.type	nftl_support_boot_check_crc_from_dts, %function
nftl_support_boot_check_crc_from_dts:
	@ args = 0, pretend = 0, frame = 0
	@ frame_needed = 0, uses_anonymous_args = 0
	push	{r3, lr}
	bl	nand_get_support_boot_check_crc
	adds	r0, r0, #0
	it	ne
	movne	r0, #1
	pop	{r3, pc}
	.size	nftl_support_boot_check_crc_from_dts, .-nftl_support_boot_check_crc_from_dts
	.section	.text._nftl_nand_read_history_page,"ax",%progbits
	.align	1
	.global	_nftl_nand_read_history_page
	.syntax unified
	.thumb
	.thumb_func
	.fpu softvfp
	.type	_nftl_nand_read_history_page, %function
_nftl_nand_read_history_page:
	@ args = 0, pretend = 0, frame = 16
	@ frame_needed = 0, uses_anonymous_args = 0
	push	{r4, r5, lr}
	mov	r4, r0
	ldr	r2, [r4, #1968]
	sub	sp, sp, #20
	ldr	r3, [r0]
	ldr	r0, [r3, #4]
	ldrd	r3, r5, [r2, #80]
	adds	r3, r3, #1
	adc	r5, r5, #0
	strd	r3, r5, [r2, #80]
	ldrh	r3, [r1, #2]
	strh	r3, [sp, #2]	@ movhi
	ldrh	r3, [r1]
	strh	r3, [sp]	@ movhi
	ldrh	r3, [r1, #4]
	strh	r3, [sp, #4]	@ movhi
	ldr	r3, [r1, #8]
	str	r3, [sp, #8]
	ldr	r3, [r1, #12]
	mov	r1, sp
	str	r3, [sp, #12]
	ldr	r3, [r0, #88]
	blx	r3
	adds	r3, r0, #2
	itttt	eq
	ldreq	r2, [r4, #1968]
	ldreq	r3, [r2, #132]
	addeq	r3, r3, #1
	streq	r3, [r2, #132]
	add	sp, sp, #20
	@ sp needed
	pop	{r4, r5, pc}
	.size	_nftl_nand_read_history_page, .-_nftl_nand_read_history_page
	.section	.text.print_all_nftl_blk,"ax",%progbits
	.align	1
	.global	print_all_nftl_blk
	.syntax unified
	.thumb
	.thumb_func
	.fpu softvfp
	.type	print_all_nftl_blk, %function
print_all_nftl_blk:
	@ args = 0, pretend = 0, frame = 0
	@ frame_needed = 0, uses_anonymous_args = 0
	push	{r3, r4, r5, lr}
	ldr	r4, .L39
	ldr	r5, .L39+4
.L37:
	ldr	r3, [r4, #8]
	cbnz	r3, .L38
	pop	{r3, r4, r5, pc}
.L38:
	ldr	r1, [r3]
	mov	r0, r5
	bl	NAND_Print_DBG
	ldr	r4, [r4, #8]
	b	.L37
.L40:
	.align	2
.L39:
	.word	.LANCHOR0
	.word	.LC9
	.size	print_all_nftl_blk, .-print_all_nftl_blk
	.section	.text.get_nftl_num,"ax",%progbits
	.align	1
	.global	get_nftl_num
	.syntax unified
	.thumb
	.thumb_func
	.fpu softvfp
	.type	get_nftl_num, %function
get_nftl_num:
	@ args = 0, pretend = 0, frame = 0
	@ frame_needed = 0, uses_anonymous_args = 0
	@ link register save eliminated.
	ldr	r3, .L44
	movs	r0, #0
.L42:
	ldr	r3, [r3, #8]
	cbnz	r3, .L43
	bx	lr
.L43:
	adds	r0, r0, #1
	b	.L42
.L45:
	.align	2
.L44:
	.word	.LANCHOR0
	.size	get_nftl_num, .-get_nftl_num
	.section	.text.get_nftl_cap,"ax",%progbits
	.align	1
	.global	get_nftl_cap
	.syntax unified
	.thumb
	.thumb_func
	.fpu softvfp
	.type	get_nftl_cap, %function
get_nftl_cap:
	@ args = 0, pretend = 0, frame = 0
	@ frame_needed = 0, uses_anonymous_args = 0
	@ link register save eliminated.
	ldr	r3, .L49
	movs	r0, #0
.L47:
	ldr	r3, [r3, #8]
	cbnz	r3, .L48
	bx	lr
.L48:
	ldr	r2, [r3]
	add	r0, r0, r2
	b	.L47
.L50:
	.align	2
.L49:
	.word	.LANCHOR0
	.size	get_nftl_cap, .-get_nftl_cap
	.section	.text.get_first_nftl_cap,"ax",%progbits
	.align	1
	.global	get_first_nftl_cap
	.syntax unified
	.thumb
	.thumb_func
	.fpu softvfp
	.type	get_first_nftl_cap, %function
get_first_nftl_cap:
	@ args = 0, pretend = 0, frame = 0
	@ frame_needed = 0, uses_anonymous_args = 0
	@ link register save eliminated.
	ldr	r3, .L52
	ldr	r3, [r3, #8]
	ldr	r0, [r3]
	bx	lr
.L53:
	.align	2
.L52:
	.word	.LANCHOR0
	.size	get_first_nftl_cap, .-get_first_nftl_cap
	.section	.text.nftl_read,"ax",%progbits
	.align	1
	.global	nftl_read
	.syntax unified
	.thumb
	.thumb_func
	.fpu softvfp
	.type	nftl_read, %function
nftl_read:
	@ args = 0, pretend = 0, frame = 0
	@ frame_needed = 0, uses_anonymous_args = 0
	mov	r3, r2
	push	{r4, r5, r6, lr}
	mov	r2, r1
	cbnz	r1, .L63
.L55:
	mov	r0, r2
	pop	{r4, r5, r6, pc}
.L58:
	ldr	r4, [r4, #8]
	subs	r0, r0, r1
	cbnz	r4, .L57
	mov	r1, r0
	ldr	r0, .L64
	bl	NAND_Print
	movs	r2, #1
	b	.L55
.L63:
	ldr	r1, .L64+4
	ldr	r4, [r1, #8]
.L57:
	ldr	r1, [r4]
	cmp	r1, r0
	bls	.L58
	ldr	r5, [r4, #44]
	mov	r1, r0
	mov	r0, r4
	mov	ip, r5
	pop	{r4, r5, r6, lr}
	bx	ip
.L65:
	.align	2
.L64:
	.word	.LC10
	.word	.LANCHOR0
	.size	nftl_read, .-nftl_read
	.section	.text.nftl_read_history,"ax",%progbits
	.align	1
	.global	nftl_read_history
	.syntax unified
	.thumb
	.thumb_func
	.fpu softvfp
	.type	nftl_read_history, %function
nftl_read_history:
	@ args = 0, pretend = 0, frame = 0
	@ frame_needed = 0, uses_anonymous_args = 0
	push	{r4, r5, r6, r7, r8, lr}
	mov	r6, r0
	mov	r8, r2
	mov	r5, r1
	cbz	r1, .L67
	ldr	r3, .L74
	ldr	r0, .L74+4
	ldr	r4, [r3, #8]
	ldr	r1, [r4]
	bl	NAND_Print
.L68:
	ldr	r1, [r4]
	cmp	r1, r6
	bls	.L69
	ldr	r7, [r4, #40]
	mov	r3, r8
	mov	r2, r5
	mov	r1, r6
	mov	r0, r4
	mov	ip, r7
	pop	{r4, r5, r6, r7, r8, lr}
	bx	ip
.L69:
	ldr	r4, [r4, #8]
	subs	r6, r6, r1
	cmp	r4, #0
	bne	.L68
	mov	r2, r5
	movs	r5, #1
	mov	r1, r6
	ldr	r0, .L74+8
	bl	NAND_Print
.L67:
	mov	r0, r5
	pop	{r4, r5, r6, r7, r8, pc}
.L75:
	.align	2
.L74:
	.word	.LANCHOR0
	.word	.LC11
	.word	.LC10
	.size	nftl_read_history, .-nftl_read_history
	.section	.text.nftl_write,"ax",%progbits
	.align	1
	.global	nftl_write
	.syntax unified
	.thumb
	.thumb_func
	.fpu softvfp
	.type	nftl_write, %function
nftl_write:
	@ args = 0, pretend = 0, frame = 0
	@ frame_needed = 0, uses_anonymous_args = 0
	mov	r3, r2
	push	{r4, r5, r6, lr}
	mov	r2, r1
	cbnz	r1, .L85
.L77:
	mov	r0, r2
	pop	{r4, r5, r6, pc}
.L80:
	ldr	r4, [r4, #8]
	subs	r0, r0, r1
	cbnz	r4, .L79
	mov	r1, r0
	ldr	r0, .L86
	bl	NAND_Print
	movs	r2, #1
	b	.L77
.L85:
	ldr	r1, .L86+4
	ldr	r4, [r1, #8]
.L79:
	ldr	r1, [r4]
	cmp	r1, r0
	bls	.L80
	ldr	r5, [r4, #48]
	mov	r1, r0
	mov	r0, r4
	mov	ip, r5
	pop	{r4, r5, r6, lr}
	bx	ip
.L87:
	.align	2
.L86:
	.word	.LC10
	.word	.LANCHOR0
	.size	nftl_write, .-nftl_write
	.section	.text.nftl_discard,"ax",%progbits
	.align	1
	.global	nftl_discard
	.syntax unified
	.thumb
	.thumb_func
	.fpu softvfp
	.type	nftl_discard, %function
nftl_discard:
	@ args = 0, pretend = 0, frame = 0
	@ frame_needed = 0, uses_anonymous_args = 0
	push	{r4, lr}
	mov	r2, r1
	cbnz	r1, .L97
.L89:
	mov	r0, r2
	pop	{r4, pc}
.L92:
	ldr	r3, [r3, #8]
	subs	r0, r0, r1
	cbnz	r3, .L91
	mov	r1, r0
	ldr	r0, .L98
	bl	NAND_Print
	movs	r2, #1
	b	.L89
.L97:
	ldr	r3, .L98+4
	ldr	r3, [r3, #8]
.L91:
	ldr	r1, [r3]
	cmp	r1, r0
	bls	.L92
	ldr	r4, [r3, #56]
	mov	r1, r0
	mov	r0, r3
	mov	r3, r4
	pop	{r4, lr}
	bx	r3
.L99:
	.align	2
.L98:
	.word	.LC10
	.word	.LANCHOR0
	.size	nftl_discard, .-nftl_discard
	.section	.text.nftl_flush_write_cache,"ax",%progbits
	.align	1
	.global	nftl_flush_write_cache
	.syntax unified
	.thumb
	.thumb_func
	.fpu softvfp
	.type	nftl_flush_write_cache, %function
nftl_flush_write_cache:
	@ args = 0, pretend = 0, frame = 0
	@ frame_needed = 0, uses_anonymous_args = 0
	ldr	r3, .L103
	push	{r4, lr}
	ldr	r4, [r3, #8]
.L101:
	cbnz	r4, .L102
	mov	r0, r4
	pop	{r4, pc}
.L102:
	ldr	r3, [r4, #52]
	mov	r0, r4
	movw	r1, #65535
	blx	r3
	ldr	r4, [r4, #8]
	b	.L101
.L104:
	.align	2
.L103:
	.word	.LANCHOR0
	.size	nftl_flush_write_cache, .-nftl_flush_write_cache
	.section	.text.add_free_list,"ax",%progbits
	.align	1
	.global	add_free_list
	.syntax unified
	.thumb
	.thumb_func
	.fpu softvfp
	.type	add_free_list, %function
add_free_list:
	@ args = 0, pretend = 0, frame = 0
	@ frame_needed = 0, uses_anonymous_args = 0
	adds	r0, r0, #152
	push	{r4, lr}
.L106:
	ldr	r3, [r0, #20]
	cbnz	r3, .L108
	str	r1, [r0, #20]
	str	r3, [r1, #20]
	b	.L110
.L108:
	ldrh	r2, [r3, #6]
	ldrh	r4, [r1, #6]
	cmp	r4, r2
	bhi	.L109
	str	r1, [r3, #24]
	str	r3, [r1, #20]
	str	r1, [r0, #20]
.L110:
	str	r0, [r1, #24]
	movs	r0, #0
	pop	{r4, pc}
.L109:
	mov	r0, r3
	b	.L106
	.size	add_free_list, .-add_free_list
	.section	.text.del_free_list,"ax",%progbits
	.align	1
	.global	del_free_list
	.syntax unified
	.thumb
	.thumb_func
	.fpu softvfp
	.type	del_free_list, %function
del_free_list:
	@ args = 0, pretend = 0, frame = 0
	@ frame_needed = 0, uses_anonymous_args = 0
	@ link register save eliminated.
	mov	r3, r0
	ldr	r0, [r0, #172]
	cbz	r0, .L111
	ldr	r2, [r0, #20]
	str	r2, [r3, #172]
	ldr	r2, [r0, #20]
	str	r2, [r3, #172]
	ldr	r2, [r0, #20]
	cbz	r2, .L111
	adds	r3, r3, #152
	str	r3, [r2, #24]
.L111:
	bx	lr
	.size	del_free_list, .-del_free_list
	.section	.text.del_free_list_by_block,"ax",%progbits
	.align	1
	.global	del_free_list_by_block
	.syntax unified
	.thumb
	.thumb_func
	.fpu softvfp
	.type	del_free_list_by_block, %function
del_free_list_by_block:
	@ args = 0, pretend = 0, frame = 0
	@ frame_needed = 0, uses_anonymous_args = 0
	@ link register save eliminated.
	ldr	r0, [r0, #172]
.L119:
	cbnz	r0, .L122
	bx	lr
.L122:
	cmp	r0, r1
	ldr	r3, [r0, #20]
	bne	.L123
	ldr	r2, [r0, #24]
	str	r3, [r2, #20]
	ldr	r3, [r0, #20]
	cbz	r3, .L120
	str	r2, [r3, #24]
.L120:
	movs	r3, #0
	strd	r3, r3, [r0, #20]
	bx	lr
.L123:
	mov	r0, r3
	b	.L119
	.size	del_free_list_by_block, .-del_free_list_by_block
	.section	.text.is_block_in_free_list,"ax",%progbits
	.align	1
	.global	is_block_in_free_list
	.syntax unified
	.thumb
	.thumb_func
	.fpu softvfp
	.type	is_block_in_free_list, %function
is_block_in_free_list:
	@ args = 0, pretend = 0, frame = 0
	@ frame_needed = 0, uses_anonymous_args = 0
	@ link register save eliminated.
	ldr	r0, [r0, #172]
.L128:
	cbnz	r0, .L130
	bx	lr
.L130:
	cmp	r0, r1
	beq	.L131
	ldr	r0, [r0, #20]
	b	.L128
.L131:
	movs	r0, #1
	bx	lr
	.size	is_block_in_free_list, .-is_block_in_free_list
	.section	.text.print_free_list,"ax",%progbits
	.align	1
	.global	print_free_list
	.syntax unified
	.thumb
	.thumb_func
	.fpu softvfp
	.type	print_free_list, %function
print_free_list:
	@ args = 0, pretend = 0, frame = 0
	@ frame_needed = 0, uses_anonymous_args = 0
	ldr	r3, [r0, #356]
	push	{r4, r5, r6, lr}
	mov	r4, r0
	cbz	r3, .L132
	ldr	r0, .L137
	bl	NAND_Print_DBG
	ldr	r5, .L137+4
	ldr	r4, [r4, #172]
	ldr	r6, .L137+8
.L134:
	cbnz	r4, .L135
.L132:
	pop	{r4, r5, r6, pc}
.L135:
	ldrh	r2, [r4, #6]
	mov	r0, r5
	ldrh	r1, [r4]
	bl	NAND_Print_DBG
	ldr	r1, [r4, #8]
	mov	r0, r6
	bl	NAND_Print_DBG
	ldrh	r1, [r4, #2]
	ldr	r0, .L137+12
	bl	NAND_Print_DBG
	ldr	r4, [r4, #20]
	b	.L134
.L138:
	.align	2
.L137:
	.word	.LC12
	.word	.LC13
	.word	.LC14
	.word	.LC15
	.size	print_free_list, .-print_free_list
	.section	.text.add_block_invalid_list,"ax",%progbits
	.align	1
	.global	add_block_invalid_list
	.syntax unified
	.thumb
	.thumb_func
	.fpu softvfp
	.type	add_block_invalid_list, %function
add_block_invalid_list:
	@ args = 0, pretend = 0, frame = 0
	@ frame_needed = 0, uses_anonymous_args = 0
	adds	r0, r0, #188
	push	{r4, lr}
.L140:
	ldr	r3, [r0, #12]
	cbnz	r3, .L142
	str	r1, [r0, #12]
	str	r3, [r1, #12]
	b	.L144
.L142:
	ldrh	r2, [r3, #4]
	ldrh	r4, [r1, #4]
	cmp	r4, r2
	bcc	.L143
	str	r1, [r3, #16]
	str	r3, [r1, #12]
	str	r1, [r0, #12]
.L144:
	str	r0, [r1, #16]
	movs	r0, #0
	pop	{r4, pc}
.L143:
	mov	r0, r3
	b	.L140
	.size	add_block_invalid_list, .-add_block_invalid_list
	.section	.text.del_block_invalid_list,"ax",%progbits
	.align	1
	.global	del_block_invalid_list
	.syntax unified
	.thumb
	.thumb_func
	.fpu softvfp
	.type	del_block_invalid_list, %function
del_block_invalid_list:
	@ args = 0, pretend = 0, frame = 0
	@ frame_needed = 0, uses_anonymous_args = 0
	@ link register save eliminated.
	mov	r3, r0
	ldr	r0, [r0, #200]
	cbz	r0, .L145
	ldr	r2, [r0, #12]
	str	r2, [r3, #200]
	ldr	r2, [r0, #12]
	cbz	r2, .L145
	adds	r3, r3, #188
	str	r3, [r2, #16]
.L145:
	bx	lr
	.size	del_block_invalid_list, .-del_block_invalid_list
	.section	.text.del_block_invalid_list_by_block,"ax",%progbits
	.align	1
	.global	del_block_invalid_list_by_block
	.syntax unified
	.thumb
	.thumb_func
	.fpu softvfp
	.type	del_block_invalid_list_by_block, %function
del_block_invalid_list_by_block:
	@ args = 0, pretend = 0, frame = 0
	@ frame_needed = 0, uses_anonymous_args = 0
	@ link register save eliminated.
	ldr	r0, [r0, #200]
.L153:
	cbnz	r0, .L156
	bx	lr
.L156:
	cmp	r0, r1
	ldr	r3, [r0, #12]
	bne	.L157
	ldr	r2, [r0, #16]
	str	r3, [r2, #12]
	ldr	r3, [r0, #12]
	cbz	r3, .L154
	str	r2, [r3, #16]
.L154:
	movs	r3, #0
	strd	r3, r3, [r0, #12]
	bx	lr
.L157:
	mov	r0, r3
	b	.L153
	.size	del_block_invalid_list_by_block, .-del_block_invalid_list_by_block
	.section	.text.is_block_in_invalid_list,"ax",%progbits
	.align	1
	.global	is_block_in_invalid_list
	.syntax unified
	.thumb
	.thumb_func
	.fpu softvfp
	.type	is_block_in_invalid_list, %function
is_block_in_invalid_list:
	@ args = 0, pretend = 0, frame = 0
	@ frame_needed = 0, uses_anonymous_args = 0
	@ link register save eliminated.
	ldr	r0, [r0, #200]
.L162:
	cbnz	r0, .L164
	bx	lr
.L164:
	cmp	r0, r1
	beq	.L165
	ldr	r0, [r0, #12]
	b	.L162
.L165:
	movs	r0, #1
	bx	lr
	.size	is_block_in_invalid_list, .-is_block_in_invalid_list
	.section	.text.print_block_invalid_list,"ax",%progbits
	.align	1
	.global	print_block_invalid_list
	.syntax unified
	.thumb
	.thumb_func
	.fpu softvfp
	.type	print_block_invalid_list, %function
print_block_invalid_list:
	@ args = 0, pretend = 0, frame = 0
	@ frame_needed = 0, uses_anonymous_args = 0
	ldr	r3, [r0, #356]
	push	{r4, r5, r6, lr}
	mov	r4, r0
	cbz	r3, .L166
	ldr	r0, .L171
	bl	NAND_Print_DBG
	ldr	r5, .L171+4
	ldr	r4, [r4, #200]
	ldr	r6, .L171+8
.L168:
	cbnz	r4, .L169
.L166:
	pop	{r4, r5, r6, pc}
.L169:
	ldrh	r2, [r4, #6]
	mov	r0, r5
	ldrh	r1, [r4]
	bl	NAND_Print_DBG
	ldrh	r2, [r4, #4]
	mov	r0, r6
	ldr	r1, [r4, #8]
	bl	NAND_Print_DBG
	ldrh	r1, [r4, #2]
	ldr	r0, .L171+12
	bl	NAND_Print_DBG
	ldr	r4, [r4, #12]
	b	.L168
.L172:
	.align	2
.L171:
	.word	.LC16
	.word	.LC13
	.word	.LC17
	.word	.LC15
	.size	print_block_invalid_list, .-print_block_invalid_list
	.section	.text.add_block_count_list,"ax",%progbits
	.align	1
	.global	add_block_count_list
	.syntax unified
	.thumb
	.thumb_func
	.fpu softvfp
	.type	add_block_count_list, %function
add_block_count_list:
	@ args = 0, pretend = 0, frame = 0
	@ frame_needed = 0, uses_anonymous_args = 0
	adds	r0, r0, #224
	push	{r4, lr}
.L174:
	ldr	r3, [r0, #28]
	cbnz	r3, .L176
	str	r1, [r0, #28]
	str	r3, [r1, #28]
	b	.L178
.L176:
	ldr	r4, [r3, #8]
	ldr	r2, [r1, #8]
	cmp	r4, r2
	blt	.L177
	str	r1, [r3, #32]
	str	r3, [r1, #28]
	str	r1, [r0, #28]
.L178:
	str	r0, [r1, #32]
	movs	r0, #0
	pop	{r4, pc}
.L177:
	mov	r0, r3
	b	.L174
	.size	add_block_count_list, .-add_block_count_list
	.section	.text.del_block_count_list,"ax",%progbits
	.align	1
	.global	del_block_count_list
	.syntax unified
	.thumb
	.thumb_func
	.fpu softvfp
	.type	del_block_count_list, %function
del_block_count_list:
	@ args = 0, pretend = 0, frame = 0
	@ frame_needed = 0, uses_anonymous_args = 0
	@ link register save eliminated.
	mov	r3, r0
	adds	r0, r0, #224
	ldr	r2, [r3, #252]
	cbz	r2, .L179
	ldr	r2, [r2, #28]
	str	r2, [r3, #252]
.L179:
	bx	lr
	.size	del_block_count_list, .-del_block_count_list
	.section	.text.del_block_count_by_block,"ax",%progbits
	.align	1
	.global	del_block_count_by_block
	.syntax unified
	.thumb
	.thumb_func
	.fpu softvfp
	.type	del_block_count_by_block, %function
del_block_count_by_block:
	@ args = 0, pretend = 0, frame = 0
	@ frame_needed = 0, uses_anonymous_args = 0
	@ link register save eliminated.
	ldr	r0, [r0, #252]
.L185:
	cbnz	r0, .L188
	bx	lr
.L188:
	cmp	r0, r1
	ldr	r3, [r0, #28]
	bne	.L189
	ldr	r2, [r0, #32]
	str	r3, [r2, #28]
	ldr	r3, [r0, #28]
	cbz	r3, .L186
	str	r2, [r3, #32]
.L186:
	movs	r3, #0
	strd	r3, r3, [r0, #28]
	bx	lr
.L189:
	mov	r0, r3
	b	.L185
	.size	del_block_count_by_block, .-del_block_count_by_block
	.section	.text.print_block_count_list,"ax",%progbits
	.align	1
	.global	print_block_count_list
	.syntax unified
	.thumb
	.thumb_func
	.fpu softvfp
	.type	print_block_count_list, %function
print_block_count_list:
	@ args = 0, pretend = 0, frame = 0
	@ frame_needed = 0, uses_anonymous_args = 0
	ldr	r3, [r0, #356]
	push	{r4, r5, r6, lr}
	mov	r4, r0
	cbz	r3, .L193
	ldr	r0, .L198
	bl	NAND_Print_DBG
	ldr	r5, .L198+4
	ldr	r4, [r4, #252]
	ldr	r6, .L198+8
.L195:
	cbnz	r4, .L196
.L193:
	pop	{r4, r5, r6, pc}
.L196:
	ldrh	r1, [r4]
	mov	r0, r5
	bl	NAND_Print_DBG
	ldr	r1, [r4, #8]
	mov	r0, r6
	bl	NAND_Print_DBG
	ldr	r4, [r4, #28]
	b	.L195
.L199:
	.align	2
.L198:
	.word	.LC18
	.word	.LC19
	.word	.LC20
	.size	print_block_count_list, .-print_block_count_list
	.section	.text.put_phy_block_to_free_list,"ax",%progbits
	.align	1
	.global	put_phy_block_to_free_list
	.syntax unified
	.thumb
	.thumb_func
	.fpu softvfp
	.type	put_phy_block_to_free_list, %function
put_phy_block_to_free_list:
	@ args = 0, pretend = 0, frame = 0
	@ frame_needed = 0, uses_anonymous_args = 0
	movs	r2, #0
	push	{r3, r4, r5, lr}
	strd	r2, r2, [r1, #20]
	mov	r5, r0
	bl	add_free_list
	mov	r4, r0
	cbnz	r0, .L201
	ldrh	r3, [r5, #22]
	adds	r3, r3, #1
	strh	r3, [r5, #22]	@ movhi
.L200:
	mov	r0, r4
	pop	{r3, r4, r5, pc}
.L201:
	ldr	r0, .L203
	bl	NAND_Print_DBG
	b	.L200
.L204:
	.align	2
.L203:
	.word	.LC21
	.size	put_phy_block_to_free_list, .-put_phy_block_to_free_list
	.section	.text.out_phy_block_from_free_list,"ax",%progbits
	.align	1
	.global	out_phy_block_from_free_list
	.syntax unified
	.thumb
	.thumb_func
	.fpu softvfp
	.type	out_phy_block_from_free_list, %function
out_phy_block_from_free_list:
	@ args = 0, pretend = 0, frame = 0
	@ frame_needed = 0, uses_anonymous_args = 0
	push	{r3, lr}
	mov	r1, r0
	bl	del_free_list
	cbz	r0, .L205
	movs	r3, #0
	strd	r3, r3, [r0, #20]
	ldrh	r3, [r1, #22]
	cbz	r3, .L205
	subs	r3, r3, #1
	strh	r3, [r1, #22]	@ movhi
.L205:
	pop	{r3, pc}
	.size	out_phy_block_from_free_list, .-out_phy_block_from_free_list
	.section	.text.get_max_erase_time_from_free_list,"ax",%progbits
	.align	1
	.global	get_max_erase_time_from_free_list
	.syntax unified
	.thumb
	.thumb_func
	.fpu softvfp
	.type	get_max_erase_time_from_free_list, %function
get_max_erase_time_from_free_list:
	@ args = 0, pretend = 0, frame = 0
	@ frame_needed = 0, uses_anonymous_args = 0
	ldr	r3, [r0, #172]
	movs	r0, #0
	mov	r2, r0
	push	{r4, lr}
.L213:
	cbnz	r3, .L215
	cbz	r2, .L217
	ldrh	r0, [r2, #6]
.L216:
	pop	{r4, pc}
.L215:
	ldrh	r4, [r3, #6]
	cmp	r4, r1
	bcs	.L214
	cmp	r4, r0
	itt	cs
	movcs	r0, r4
	movcs	r2, r3
.L214:
	ldr	r3, [r3, #20]
	b	.L213
.L217:
	movw	r0, #65535
	b	.L216
	.size	get_max_erase_time_from_free_list, .-get_max_erase_time_from_free_list
	.section	.text.get_block_max_erase_time_from_free_list,"ax",%progbits
	.align	1
	.global	get_block_max_erase_time_from_free_list
	.syntax unified
	.thumb
	.thumb_func
	.fpu softvfp
	.type	get_block_max_erase_time_from_free_list, %function
get_block_max_erase_time_from_free_list:
	@ args = 0, pretend = 0, frame = 0
	@ frame_needed = 0, uses_anonymous_args = 0
	movs	r2, #0
	ldr	r3, [r0, #172]
	mov	r0, r2
	push	{r4, lr}
.L219:
	cbnz	r3, .L221
	pop	{r4, pc}
.L221:
	ldrh	r4, [r3, #6]
	cmp	r4, r1
	bcs	.L220
	cmp	r4, r2
	itt	cs
	movcs	r2, r4
	movcs	r0, r3
.L220:
	ldr	r3, [r3, #20]
	b	.L219
	.size	get_block_max_erase_time_from_free_list, .-get_block_max_erase_time_from_free_list
	.section	.text.get_block_max_erase_time_from_invalid_list,"ax",%progbits
	.align	1
	.global	get_block_max_erase_time_from_invalid_list
	.syntax unified
	.thumb
	.thumb_func
	.fpu softvfp
	.type	get_block_max_erase_time_from_invalid_list, %function
get_block_max_erase_time_from_invalid_list:
	@ args = 0, pretend = 0, frame = 0
	@ frame_needed = 0, uses_anonymous_args = 0
	movs	r2, #0
	ldr	r3, [r0, #200]
	mov	r0, r2
	push	{r4, lr}
.L223:
	cbnz	r3, .L225
	pop	{r4, pc}
.L225:
	ldrh	r4, [r3, #6]
	cmp	r4, r1
	bcs	.L224
	cmp	r4, r2
	itt	cs
	movcs	r2, r4
	movcs	r0, r3
.L224:
	ldr	r3, [r3, #12]
	b	.L223
	.size	get_block_max_erase_time_from_invalid_list, .-get_block_max_erase_time_from_invalid_list
	.section	.text.get_block_min_erase_time_from_invalid_list,"ax",%progbits
	.align	1
	.global	get_block_min_erase_time_from_invalid_list
	.syntax unified
	.thumb
	.thumb_func
	.fpu softvfp
	.type	get_block_min_erase_time_from_invalid_list, %function
get_block_min_erase_time_from_invalid_list:
	@ args = 0, pretend = 0, frame = 0
	@ frame_needed = 0, uses_anonymous_args = 0
	ldr	r3, [r0, #4]
	push	{r4, r5, r6, lr}
	ldrh	r5, [r3, #16]
	ldr	r3, [r0, #200]
	movs	r0, #0
.L227:
	cbnz	r3, .L229
	pop	{r4, r5, r6, pc}
.L229:
	ldrh	r4, [r3, #6]
	cmp	r4, r1
	bcs	.L228
	ldr	r6, [r3, #8]
	cmp	r6, r2
	bcs	.L228
	cmp	r4, r5
	itt	ls
	movls	r5, r4
	movls	r0, r3
.L228:
	ldr	r3, [r3, #12]
	b	.L227
	.size	get_block_min_erase_time_from_invalid_list, .-get_block_min_erase_time_from_invalid_list
	.section	.text.get_block_max_count_from_invalid_list,"ax",%progbits
	.align	1
	.global	get_block_max_count_from_invalid_list
	.syntax unified
	.thumb
	.thumb_func
	.fpu softvfp
	.type	get_block_max_count_from_invalid_list, %function
get_block_max_count_from_invalid_list:
	@ args = 0, pretend = 0, frame = 0
	@ frame_needed = 0, uses_anonymous_args = 0
	@ link register save eliminated.
	movs	r2, #0
	ldr	r3, [r0, #200]
	mov	r0, r2
.L231:
	cbnz	r3, .L233
	bx	lr
.L233:
	ldr	r1, [r3, #8]
	cmp	r1, r2
	itt	cs
	movcs	r0, r3
	movcs	r2, r1
	ldr	r3, [r3, #12]
	b	.L231
	.size	get_block_max_count_from_invalid_list, .-get_block_max_count_from_invalid_list
	.section	.text.get_block_min_count_from_invalid_list,"ax",%progbits
	.align	1
	.global	get_block_min_count_from_invalid_list
	.syntax unified
	.thumb
	.thumb_func
	.fpu softvfp
	.type	get_block_min_count_from_invalid_list, %function
get_block_min_count_from_invalid_list:
	@ args = 0, pretend = 0, frame = 0
	@ frame_needed = 0, uses_anonymous_args = 0
	@ link register save eliminated.
	ldr	r3, [r0, #200]
	mov	r2, #-1
	movs	r0, #0
.L235:
	cbnz	r3, .L237
	bx	lr
.L237:
	ldr	r1, [r3, #8]
	cmp	r1, r2
	itt	ls
	movls	r0, r3
	movls	r2, r1
	ldr	r3, [r3, #12]
	b	.L235
	.size	get_block_min_count_from_invalid_list, .-get_block_min_count_from_invalid_list
	.section	.text.out_phy_block_from_free_list_by_block,"ax",%progbits
	.align	1
	.global	out_phy_block_from_free_list_by_block
	.syntax unified
	.thumb
	.thumb_func
	.fpu softvfp
	.type	out_phy_block_from_free_list_by_block, %function
out_phy_block_from_free_list_by_block:
	@ args = 0, pretend = 0, frame = 0
	@ frame_needed = 0, uses_anonymous_args = 0
	push	{r4, lr}
	mov	r4, r0
	bl	del_free_list_by_block
	cbz	r0, .L238
	movs	r3, #0
	strd	r3, r3, [r0, #20]
	ldrh	r3, [r4, #22]
	cbz	r3, .L238
	subs	r3, r3, #1
	strh	r3, [r4, #22]	@ movhi
.L238:
	pop	{r4, pc}
	.size	out_phy_block_from_free_list_by_block, .-out_phy_block_from_free_list_by_block
	.section	.text.out_phy_block_from_free_list_by_erase_time,"ax",%progbits
	.align	1
	.global	out_phy_block_from_free_list_by_erase_time
	.syntax unified
	.thumb
	.thumb_func
	.fpu softvfp
	.type	out_phy_block_from_free_list_by_erase_time, %function
out_phy_block_from_free_list_by_erase_time:
	@ args = 0, pretend = 0, frame = 0
	@ frame_needed = 0, uses_anonymous_args = 0
	@ link register save eliminated.
	ldr	r3, [r0, #172]
	push	{r4, r5}
	movs	r4, #0
	mov	r2, r4
.L246:
	cbnz	r3, .L248
	cbz	r2, .L245
	mov	r1, r2
	pop	{r4, r5}
	b	out_phy_block_from_free_list_by_block
.L248:
	ldrh	r5, [r3, #6]
	cmp	r5, r1
	bcs	.L247
	cmp	r5, r4
	itt	cs
	movcs	r4, r5
	movcs	r2, r3
.L247:
	ldr	r3, [r3, #20]
	b	.L246
.L245:
	mov	r0, r2
	pop	{r4, r5}
	bx	lr
	.size	out_phy_block_from_free_list_by_erase_time, .-out_phy_block_from_free_list_by_erase_time
	.section	.text.put_phy_block_to_invalid_page_list,"ax",%progbits
	.align	1
	.global	put_phy_block_to_invalid_page_list
	.syntax unified
	.thumb
	.thumb_func
	.fpu softvfp
	.type	put_phy_block_to_invalid_page_list, %function
put_phy_block_to_invalid_page_list:
	@ args = 0, pretend = 0, frame = 0
	@ frame_needed = 0, uses_anonymous_args = 0
	movs	r2, #0
	push	{r4, lr}
	strd	r2, r2, [r1, #12]
	bl	add_block_invalid_list
	mov	r4, r0
	cbz	r0, .L250
	ldr	r0, .L255
	bl	NAND_Print_DBG
.L250:
	mov	r0, r4
	pop	{r4, pc}
.L256:
	.align	2
.L255:
	.word	.LC22
	.size	put_phy_block_to_invalid_page_list, .-put_phy_block_to_invalid_page_list
	.section	.text.out_phy_block_from_invalid_page_list,"ax",%progbits
	.align	1
	.global	out_phy_block_from_invalid_page_list
	.syntax unified
	.thumb
	.thumb_func
	.fpu softvfp
	.type	out_phy_block_from_invalid_page_list, %function
out_phy_block_from_invalid_page_list:
	@ args = 0, pretend = 0, frame = 0
	@ frame_needed = 0, uses_anonymous_args = 0
	push	{r3, lr}
	bl	del_block_invalid_list
	cbz	r0, .L257
	movs	r3, #0
	strd	r3, r3, [r0, #12]
.L257:
	pop	{r3, pc}
	.size	out_phy_block_from_invalid_page_list, .-out_phy_block_from_invalid_page_list
	.section	.text.out_phy_block_from_invalid_page_list_by_block,"ax",%progbits
	.align	1
	.global	out_phy_block_from_invalid_page_list_by_block
	.syntax unified
	.thumb
	.thumb_func
	.fpu softvfp
	.type	out_phy_block_from_invalid_page_list_by_block, %function
out_phy_block_from_invalid_page_list_by_block:
	@ args = 0, pretend = 0, frame = 0
	@ frame_needed = 0, uses_anonymous_args = 0
	push	{r3, lr}
	bl	del_block_invalid_list_by_block
	cbz	r0, .L262
	movs	r3, #0
	strd	r3, r3, [r0, #12]
.L262:
	pop	{r3, pc}
	.size	out_phy_block_from_invalid_page_list_by_block, .-out_phy_block_from_invalid_page_list_by_block
	.section	.text.phy_block_from_invalid_page_incr,"ax",%progbits
	.align	1
	.global	phy_block_from_invalid_page_incr
	.syntax unified
	.thumb
	.thumb_func
	.fpu softvfp
	.type	phy_block_from_invalid_page_incr, %function
phy_block_from_invalid_page_incr:
	@ args = 0, pretend = 0, frame = 0
	@ frame_needed = 0, uses_anonymous_args = 0
	push	{r3, r4, r5, lr}
	mov	r4, r1
	ldrh	r2, [r1, #4]
	mov	r5, r0
	ldr	r1, [r1, #12]
	ldr	r3, [r0, #4]
	adds	r2, r2, #1
	uxth	r2, r2
	ldrh	r3, [r3, #20]
	cbnz	r1, .L268
	ldr	r1, [r4, #16]
	cbnz	r1, .L268
	cmp	r2, r3
	strh	r2, [r4, #4]	@ movhi
	bls	.L270
	ldrh	r1, [r4]
	ldr	r0, .L282
	bl	NAND_Print_DBG
.L270:
	movs	r0, #0
	pop	{r3, r4, r5, pc}
.L268:
	cmp	r2, r3
	strh	r2, [r4, #4]	@ movhi
	bls	.L271
	ldr	r0, .L282+4
	bl	NAND_Print_DBG
.L271:
	ldr	r3, [r4, #16]
	adds	r5, r5, #188
	cmp	r3, r5
	beq	.L270
	ldrh	r1, [r4, #4]
.L273:
	ldrh	r2, [r3, #4]
	cmp	r2, r1
	bcs	.L270
	ldr	r2, [r3, #16]
	str	r2, [r4, #16]
	ldr	r2, [r3, #16]
	str	r4, [r2, #12]
	ldr	r2, [r4, #12]
	str	r2, [r3, #12]
	ldr	r2, [r4, #12]
	cbz	r2, .L274
	str	r3, [r2, #16]
.L274:
	str	r3, [r4, #12]
	str	r4, [r3, #16]
	ldr	r3, [r4, #16]
	ldr	r2, [r3, #16]
	cmp	r2, #0
	bne	.L273
	b	.L270
.L283:
	.align	2
.L282:
	.word	.LC23
	.word	.LC24
	.size	phy_block_from_invalid_page_incr, .-phy_block_from_invalid_page_incr
	.section	.text.adjust_invaild_list,"ax",%progbits
	.align	1
	.global	adjust_invaild_list
	.syntax unified
	.thumb
	.thumb_func
	.fpu softvfp
	.type	adjust_invaild_list, %function
adjust_invaild_list:
	@ args = 0, pretend = 0, frame = 0
	@ frame_needed = 0, uses_anonymous_args = 0
	push	{r4, lr}
	mov	r4, r0
.L285:
	ldr	r3, [r4, #200]
	cbnz	r3, .L287
.L286:
	movs	r0, #0
	pop	{r4, pc}
.L287:
	ldr	r1, [r4, #4]
	ldrh	r2, [r3, #4]
	ldrh	r3, [r1, #20]
	cmp	r2, r3
	bne	.L286
	mov	r0, r4
	bl	out_phy_block_from_invalid_page_list
	mov	r1, r0
	mov	r0, r4
	bl	put_phy_block_to_free_list
	b	.L285
	.size	adjust_invaild_list, .-adjust_invaild_list
	.section	.text.dump_buf,"ax",%progbits
	.align	1
	.global	dump_buf
	.syntax unified
	.thumb
	.thumb_func
	.fpu softvfp
	.type	dump_buf, %function
dump_buf:
	@ args = 0, pretend = 0, frame = 0
	@ frame_needed = 0, uses_anonymous_args = 0
	push	{r4, r5, r6, r7, r8, lr}
	movs	r4, #0
	ldr	r7, .L293
	mov	r6, r0
	mov	r5, r1
	ldr	r8, .L293+8
	ldr	r0, .L293+4
	bl	NAND_Print
.L289:
	cmp	r4, r5
	bne	.L292
	ldr	r0, .L293+4
	pop	{r4, r5, r6, r7, r8, lr}
	b	NAND_Print
.L292:
	lsls	r2, r4, #28
	bne	.L290
	mov	r1, r4
	mov	r0, r7
	bl	NAND_Print
.L290:
	ldrb	r1, [r6, r4]	@ zero_extendqisi2
	mov	r0, r8
	adds	r4, r4, #1
	bl	NAND_Print
	lsls	r3, r4, #28
	bne	.L289
	ldr	r0, .L293+4
	bl	NAND_Print
	b	.L289
.L294:
	.align	2
.L293:
	.word	.LC5
	.word	.LC7
	.word	.LC25
	.size	dump_buf, .-dump_buf
	.global	__aeabi_uidiv
	.section	.text.init_zone_after_first_scan,"ax",%progbits
	.align	1
	.global	init_zone_after_first_scan
	.syntax unified
	.thumb
	.thumb_func
	.fpu softvfp
	.type	init_zone_after_first_scan, %function
init_zone_after_first_scan:
	@ args = 0, pretend = 0, frame = 0
	@ frame_needed = 0, uses_anonymous_args = 0
	push	{r4, r5, r6, lr}
	mov	r5, r1
	ldr	r6, [r0, #4]
	mov	r4, r0
	ldr	r0, [r0, #24]
	ldrb	r1, [r6, #8]	@ zero_extendqisi2
	bl	__aeabi_uidiv
	ldrh	r1, [r6, #20]
	bl	__aeabi_uidiv
	mov	r1, r5
	mov	r6, r0
	mov	r2, r0
	ldr	r0, .L302
	bl	NAND_Print_DBG
	cmp	r6, r5
	bcc	.L296
	ldr	r0, .L302+4
	mov	r2, r6
	mov	r1, r5
.L301:
	bl	NAND_Print
	movs	r0, #1
.L295:
	pop	{r4, r5, r6, pc}
.L296:
	ldr	r1, [r4, #4]
	subs	r2, r5, r6
	ldrb	r0, [r1, #8]	@ zero_extendqisi2
	ldrh	r3, [r1, #20]
	muls	r3, r0, r3
	muls	r3, r2, r3
	str	r3, [r4, #32]
	ldr	r3, [r4, #1972]
	ldrh	r1, [r3, #12]
	subs	r1, r1, #1
	cmp	r2, r1
	bcs	.L298
	mov	r2, r6
	mov	r1, r5
	ldr	r0, .L302+8
	b	.L301
.L298:
	ldr	r0, .L302+12
	umull	r0, r1, r2, r0
	ubfx	r0, r1, #2, #16
	ldrh	r1, [r3, #14]
	strh	r0, [r4, #380]	@ movhi
	cmp	r0, r1
	ldrh	r0, [r3, #18]
	it	cc
	strhcc	r1, [r4, #380]	@ movhi
	ldrh	r1, [r3, #20]
	muls	r0, r2, r0
	bl	__aeabi_uidiv
	ldrh	r3, [r4, #380]
	strh	r0, [r4, #382]	@ movhi
	uxth	r0, r0
	adds	r2, r3, #1
	cmp	r2, r0
	mov	r0, #0
	itt	ge
	addge	r3, r3, #2
	strhge	r3, [r4, #382]	@ movhi
	movs	r3, #1
	str	r3, [r4, #384]
	b	.L295
.L303:
	.align	2
.L302:
	.word	.LC26
	.word	.LC27
	.word	.LC28
	.word	-858993459
	.size	init_zone_after_first_scan, .-init_zone_after_first_scan
	.section	.text.check_bit_nums,"ax",%progbits
	.align	1
	.global	check_bit_nums
	.syntax unified
	.thumb
	.thumb_func
	.fpu softvfp
	.type	check_bit_nums, %function
check_bit_nums:
	@ args = 0, pretend = 0, frame = 0
	@ frame_needed = 0, uses_anonymous_args = 0
	push	{r4, r5, r6, lr}
	movs	r2, #0
	mov	r4, r0
.L305:
	subs	r3, r4, r0
	cmp	r3, r1
	blt	.L308
	mov	r0, r2
	pop	{r4, r5, r6, pc}
.L308:
	ldrb	r6, [r4], #1	@ zero_extendqisi2
	movs	r5, #8
	movs	r3, #1
.L307:
	tst	r6, r3
	lsl	r3, r3, #1
	it	ne
	addne	r2, r2, #1
	subs	r5, r5, #1
	uxtb	r3, r3
	bne	.L307
	b	.L305
	.size	check_bit_nums, .-check_bit_nums
	.section	.text.is_all_bit_ff,"ax",%progbits
	.align	1
	.global	is_all_bit_ff
	.syntax unified
	.thumb
	.thumb_func
	.fpu softvfp
	.type	is_all_bit_ff, %function
is_all_bit_ff:
	@ args = 0, pretend = 0, frame = 0
	@ frame_needed = 0, uses_anonymous_args = 0
	push	{r4, lr}
	rsb	r4, r2, r1, lsl #3
	bl	check_bit_nums
	cmp	r4, r0
	ite	gt
	movgt	r0, #0
	movle	r0, #1
	pop	{r4, pc}
	.size	is_all_bit_ff, .-is_all_bit_ff
	.section	.text.is_all_byte_ff,"ax",%progbits
	.align	1
	.global	is_all_byte_ff
	.syntax unified
	.thumb
	.thumb_func
	.fpu softvfp
	.type	is_all_byte_ff, %function
is_all_byte_ff:
	@ args = 0, pretend = 0, frame = 0
	@ frame_needed = 0, uses_anonymous_args = 0
	push	{r4, r5, lr}
	subs	r2, r1, r2
	mov	r4, r0
	movs	r3, #0
.L315:
	subs	r5, r4, r0
	cmp	r1, r5
	bgt	.L317
	cmp	r3, r2
	ite	lt
	movlt	r0, #0
	movge	r0, #1
	pop	{r4, r5, pc}
.L317:
	ldrb	r5, [r4], #1	@ zero_extendqisi2
	cmp	r5, #255
	it	eq
	addeq	r3, r3, #1
	b	.L315
	.size	is_all_byte_ff, .-is_all_byte_ff
	.section	.text.do_write_error_in_build_list,"ax",%progbits
	.align	1
	.global	do_write_error_in_build_list
	.syntax unified
	.thumb
	.thumb_func
	.fpu softvfp
	.type	do_write_error_in_build_list, %function
do_write_error_in_build_list:
	@ args = 0, pretend = 0, frame = 16
	@ frame_needed = 0, uses_anonymous_args = 0
	push	{r4, r5, r6, r7, r8, r10, fp, lr}
	mov	r8, r3
	ldr	r3, [r0, #4]
	mov	r7, r2
	sub	sp, sp, #24
	movs	r5, #0
	ldrh	r2, [r2]
	mov	r10, r1
	ldrh	r3, [r3, #28]
	add	r1, sp, #8
	strh	r5, [sp, #8]	@ movhi
	mov	r4, r0
	strh	r2, [sp, #10]	@ movhi
	strh	r3, [sp, #12]	@ movhi
	strd	r5, r5, [sp, #16]
	ldr	r3, [r0, #1976]
	blx	r3
	ldrh	r3, [r7, #6]
	strh	r5, [r7, #4]	@ movhi
	adds	r3, r3, #1
	strh	r3, [r7, #6]	@ movhi
	cmp	r0, #0
	beq	.L323
	ldr	r0, .L328
	bl	NAND_Print
	ldr	r3, [r4, #1996]
	add	r1, sp, #8
	mov	r0, r4
	blx	r3
	movs	r0, #1
.L318:
	add	sp, sp, #24
	@ sp needed
	pop	{r4, r5, r6, r7, r8, r10, fp, pc}
.L322:
	uxth	r3, r6
	mov	r2, r7
	mov	r1, r10
	ldr	r5, [r4, #1988]
	str	r3, [sp]
	mov	r0, r4
	ldr	r3, [r4, #284]
	blx	r5
	cbz	r0, .L321
	mov	r0, fp
	bl	NAND_Print
.L321:
	adds	r6, r6, #1
.L319:
	cmp	r8, r6
	bhi	.L322
	ldr	r3, [r4, #4]
	movs	r5, #0
	ldrh	r2, [r10]
	add	r1, sp, #8
	strh	r5, [sp, #8]	@ movhi
	mov	r0, r4
	ldrh	r3, [r3, #28]
	strh	r2, [sp, #10]	@ movhi
	strd	r5, r5, [sp, #16]
	strh	r3, [sp, #12]	@ movhi
	ldr	r3, [r4, #1996]
	blx	r3
	cmp	r8, #0
	bne	.L324
	str	r7, [r4, #292]
	mov	r0, r8
	strh	r8, [r4, #320]	@ movhi
	b	.L318
.L323:
	mov	r6, r0
	ldr	fp, .L328+4
	b	.L319
.L324:
	mov	r0, r5
	b	.L318
.L329:
	.align	2
.L328:
	.word	.LC29
	.word	.LC30
	.size	do_write_error_in_build_list, .-do_write_error_in_build_list
	.section	.text.init_nrc,"ax",%progbits
	.align	1
	.global	init_nrc
	.syntax unified
	.thumb
	.thumb_func
	.fpu softvfp
	.type	init_nrc, %function
init_nrc:
	@ args = 0, pretend = 0, frame = 0
	@ frame_needed = 0, uses_anonymous_args = 0
	push	{r3, r4, r5, r6, r7, lr}
	mov	r5, r0
	movs	r0, #16
	mov	r6, r1
	bl	NAND_Malloc
	mov	r4, r0
	cbnz	r0, .L331
	movw	r2, #1399
	ldr	r1, .L351
	ldr	r0, .L351+4
	bl	NAND_Print
.L350:
	mov	r0, #-1
	b	.L330
.L331:
	movs	r2, #16
	movs	r1, #0
	bl	NAND_Memset
	ldr	r3, [r5, #4]
	ldrh	r5, [r3, #20]
	ldrh	r0, [r3, #26]
	adds	r0, r0, #64
	muls	r0, r5, r0
	bl	nftl_malloc
	str	r0, [r4, #8]
	cbnz	r0, .L333
	ldr	r1, .L351
	mov	r2, #1408
	ldr	r0, .L351+8
.L349:
.L334:
	bl	NAND_Print
	ldr	r0, [r4, #8]
	cbz	r0, .L337
	bl	nftl_free
.L337:
	ldr	r0, [r4, #12]
	cbz	r0, .L338
	bl	nftl_free
.L338:
	ldr	r0, [r4, #4]
	cbz	r0, .L339
	bl	nftl_free
.L339:
	mov	r0, r4
	bl	nftl_free
	movs	r3, #0
	str	r3, [r6]
	b	.L350
.L333:
	lsls	r7, r5, #2
	mov	r0, r7
	bl	nftl_malloc
	str	r0, [r4, #12]
	cbnz	r0, .L335
	movw	r2, #1413
	ldr	r1, .L351
	ldr	r0, .L351+12
	b	.L349
.L335:
	mov	r2, r7
	movs	r1, #255
	bl	NAND_Memset
	mov	r0, r5
	bl	nftl_malloc
	str	r0, [r4, #4]
	cbnz	r0, .L336
	movw	r2, #1419
	ldr	r1, .L351
	ldr	r0, .L351+16
	b	.L349
.L336:
	mov	r2, r5
	movs	r1, #0
	bl	NAND_Memset
	movs	r0, #0
	str	r4, [r6]
.L330:
	pop	{r3, r4, r5, r6, r7, pc}
.L352:
	.align	2
.L351:
	.word	.LANCHOR1
	.word	.LC31
	.word	.LC32
	.word	.LC33
	.word	.LC34
	.size	init_nrc, .-init_nrc
	.section	.text.free_nrc,"ax",%progbits
	.align	1
	.global	free_nrc
	.syntax unified
	.thumb
	.thumb_func
	.fpu softvfp
	.type	free_nrc, %function
free_nrc:
	@ args = 0, pretend = 0, frame = 0
	@ frame_needed = 0, uses_anonymous_args = 0
	push	{r4, lr}
	mov	r4, r0
	cbz	r0, .L354
	ldr	r0, [r0, #8]
	bl	nftl_free
	ldr	r0, [r4, #4]
	bl	nftl_free
	ldr	r0, [r4, #12]
	bl	nftl_free
	mov	r0, r4
	bl	nftl_free
.L354:
	movs	r0, #0
	pop	{r4, pc}
	.size	free_nrc, .-free_nrc
	.section	.text.nftl_check_pattern,"ax",%progbits
	.align	1
	.global	nftl_check_pattern
	.syntax unified
	.thumb
	.thumb_func
	.fpu softvfp
	.type	nftl_check_pattern, %function
nftl_check_pattern:
	@ args = 0, pretend = 0, frame = 0
	@ frame_needed = 0, uses_anonymous_args = 0
	mov	r3, r0
	push	{r4, lr}
.L359:
	subs	r4, r3, r0
	cmp	r2, r4
	bgt	.L361
	movs	r0, #0
.L358:
	pop	{r4, pc}
.L361:
	ldrb	r4, [r3], #1	@ zero_extendqisi2
	cmp	r4, r1
	beq	.L359
	movs	r0, #1
	b	.L358
	.size	nftl_check_pattern, .-nftl_check_pattern
	.section	.text.is_last_phy_block,"ax",%progbits
	.align	1
	.global	is_last_phy_block
	.syntax unified
	.thumb
	.thumb_func
	.fpu softvfp
	.type	is_last_phy_block, %function
is_last_phy_block:
	@ args = 0, pretend = 0, frame = 0
	@ frame_needed = 0, uses_anonymous_args = 0
	@ link register save eliminated.
	ldr	r3, [r1, #4]
	ldrh	r0, [r0]
	ldrh	r3, [r3, #24]
	subs	r3, r3, #1
	subs	r3, r3, r0
	rsbs	r0, r3, #0
	adcs	r0, r0, r3
	bx	lr
	.size	is_last_phy_block, .-is_last_phy_block
	.section	.text.get_vaild_blocks,"ax",%progbits
	.align	1
	.global	get_vaild_blocks
	.syntax unified
	.thumb
	.thumb_func
	.fpu softvfp
	.type	get_vaild_blocks, %function
get_vaild_blocks:
	@ args = 0, pretend = 0, frame = 16
	@ frame_needed = 0, uses_anonymous_args = 0
	push	{r0, r1, r2, r3, r4, r5, r6, r7, r8, lr}
	movs	r4, #36
	ldr	r3, [r0, #4]
	sub	r8, r2, r1
	mov	r7, r0
	mov	r5, r1
	mov	r6, r8
	ldr	r3, [r3, #32]
	mla	r4, r4, r1, r3
.L365:
	cmp	r5, r8
	bcc	.L368
.L364:
	mov	r0, r6
	add	sp, sp, #16
	@ sp needed
	pop	{r4, r5, r6, r7, r8, pc}
.L368:
	ldrh	r3, [r4]
	mov	r1, sp
	mov	r0, r7
	strh	r3, [sp, #2]	@ movhi
	ldr	r3, [r7, #1992]
	blx	r3
	cmp	r0, #159
	bne	.L366
	subs	r6, r6, #1
	beq	.L364
.L366:
	mov	r1, r7
	mov	r0, r4
	bl	is_last_phy_block
	cmp	r0, #1
	beq	.L364
	adds	r4, r4, #36
	adds	r5, r5, #1
	b	.L365
	.size	get_vaild_blocks, .-get_vaild_blocks
	.section	.text.is_phy_block_valid,"ax",%progbits
	.align	1
	.global	is_phy_block_valid
	.syntax unified
	.thumb
	.thumb_func
	.fpu softvfp
	.type	is_phy_block_valid, %function
is_phy_block_valid:
	@ args = 0, pretend = 0, frame = 0
	@ frame_needed = 0, uses_anonymous_args = 0
	@ link register save eliminated.
	ldr	r3, [r1, #4]
	ldrh	r0, [r0]
	ldrh	r3, [r3, #24]
	cmp	r0, r3
	ite	cs
	movcs	r0, #0
	movcc	r0, #1
	bx	lr
	.size	is_phy_block_valid, .-is_phy_block_valid
	.section	.text.get_phy_block_addr,"ax",%progbits
	.align	1
	.global	get_phy_block_addr
	.syntax unified
	.thumb
	.thumb_func
	.fpu softvfp
	.type	get_phy_block_addr, %function
get_phy_block_addr:
	@ args = 0, pretend = 0, frame = 0
	@ frame_needed = 0, uses_anonymous_args = 0
	@ link register save eliminated.
	ldr	r3, [r0, #4]
	movs	r0, #36
	ldr	r3, [r3, #32]
	mla	r0, r0, r1, r3
	bx	lr
	.size	get_phy_block_addr, .-get_phy_block_addr
	.section	.text.check_cross_talk,"ax",%progbits
	.align	1
	.global	check_cross_talk
	.syntax unified
	.thumb
	.thumb_func
	.fpu softvfp
	.type	check_cross_talk, %function
check_cross_talk:
	@ args = 0, pretend = 0, frame = 32
	@ frame_needed = 0, uses_anonymous_args = 0
	push	{r4, r5, r6, r7, lr}
	movs	r4, #0
	sub	sp, sp, #36
	mov	r5, r0
	mov	r6, r1
	mov	r7, r4
.L378:
	ldr	r3, [r5, #4]
	ldrh	r2, [r3, #20]
	cmp	r2, r4
	bls	.L380
	ldrh	r3, [r3, #28]
	add	r1, sp, #16
	ldrh	r2, [r6]
	mov	r0, r5
	strh	r4, [sp, #16]	@ movhi
	strh	r3, [sp, #20]	@ movhi
	strh	r2, [sp, #18]	@ movhi
	str	r7, [sp, #24]
	str	sp, [sp, #28]
	ldr	r3, [r5, #1980]
	blx	r3
	cbnz	r0, .L379
	ldrb	r3, [sp, #14]	@ zero_extendqisi2
	cmp	r3, #99
	bhi	.L379
	ldr	r2, [r5, #1968]
	ldr	r1, [r2, #168]
	cmp	r1, r3
	bhi	.L379
	str	r3, [r2, #168]
.L380:
	movs	r0, #0
	add	sp, sp, #36
	@ sp needed
	pop	{r4, r5, r6, r7, pc}
.L379:
	adds	r4, r4, #1
	b	.L378
	.size	check_cross_talk, .-check_cross_talk
	.section	.text.nftl_get_bad_block_cnt,"ax",%progbits
	.align	1
	.global	nftl_get_bad_block_cnt
	.syntax unified
	.thumb
	.thumb_func
	.fpu softvfp
	.type	nftl_get_bad_block_cnt, %function
nftl_get_bad_block_cnt:
	@ args = 0, pretend = 0, frame = 0
	@ frame_needed = 0, uses_anonymous_args = 0
	@ link register save eliminated.
	ldr	r3, [r0, #12]
	ldrh	r0, [r3, #20]
	bx	lr
	.size	nftl_get_bad_block_cnt, .-nftl_get_bad_block_cnt
	.section	.text.nand_dbg_phy_read,"ax",%progbits
	.align	1
	.global	nand_dbg_phy_read
	.syntax unified
	.thumb
	.thumb_func
	.fpu softvfp
	.type	nand_dbg_phy_read, %function
nand_dbg_phy_read:
	@ args = 0, pretend = 0, frame = 16
	@ frame_needed = 0, uses_anonymous_args = 0
	push	{r4, r5, r6, r7, lr}
	mov	r5, r0
	sub	sp, sp, #76
	mov	r0, #4096
	mov	r6, r1
	mov	r7, r2
	bl	NAND_Malloc
	mov	r4, r0
	cbnz	r0, .L384
	ldr	r1, .L385
	ldr	r0, .L385+4
	bl	NAND_Print
.L384:
	add	r3, sp, #56
	mov	r2, r7
	strd	r4, r3, [sp]
	mov	r1, r6
	movs	r3, #8
	mov	r0, r5
	bl	page_read
	mov	r5, r0
	ldrb	r0, [sp, #71]	@ zero_extendqisi2
	ldrb	r3, [sp, #58]	@ zero_extendqisi2
	ldrb	r2, [sp, #57]	@ zero_extendqisi2
	str	r0, [sp, #48]
	ldrb	r0, [sp, #70]	@ zero_extendqisi2
	ldrb	r1, [sp, #56]	@ zero_extendqisi2
	str	r0, [sp, #44]
	ldrb	r0, [sp, #69]	@ zero_extendqisi2
	str	r0, [sp, #40]
	ldrb	r0, [sp, #68]	@ zero_extendqisi2
	str	r0, [sp, #36]
	ldrb	r0, [sp, #67]	@ zero_extendqisi2
	str	r0, [sp, #32]
	ldrb	r0, [sp, #66]	@ zero_extendqisi2
	str	r0, [sp, #28]
	ldrb	r0, [sp, #65]	@ zero_extendqisi2
	str	r0, [sp, #24]
	ldrb	r0, [sp, #64]	@ zero_extendqisi2
	str	r0, [sp, #20]
	ldrb	r0, [sp, #63]	@ zero_extendqisi2
	str	r0, [sp, #16]
	ldrb	r0, [sp, #62]	@ zero_extendqisi2
	str	r0, [sp, #12]
	ldrb	r0, [sp, #61]	@ zero_extendqisi2
	str	r0, [sp, #8]
	ldrb	r0, [sp, #60]	@ zero_extendqisi2
	str	r0, [sp, #4]
	ldrb	r0, [sp, #59]	@ zero_extendqisi2
	str	r0, [sp]
	ldr	r0, .L385+8
	bl	NAND_Print_DBG
	ldrb	r0, [r4, #10]	@ zero_extendqisi2
	ldrb	r3, [r4, #2]	@ zero_extendqisi2
	ldrb	r2, [r4, #1]	@ zero_extendqisi2
	ldrb	r1, [r4]	@ zero_extendqisi2
	str	r0, [sp, #28]
	ldrb	r0, [r4, #9]	@ zero_extendqisi2
	str	r0, [sp, #24]
	ldrb	r0, [r4, #8]	@ zero_extendqisi2
	str	r0, [sp, #20]
	ldrb	r0, [r4, #7]	@ zero_extendqisi2
	str	r0, [sp, #16]
	ldrb	r0, [r4, #6]	@ zero_extendqisi2
	str	r0, [sp, #12]
	ldrb	r0, [r4, #5]	@ zero_extendqisi2
	str	r0, [sp, #8]
	ldrb	r0, [r4, #4]	@ zero_extendqisi2
	str	r0, [sp, #4]
	ldrb	r0, [r4, #3]	@ zero_extendqisi2
	str	r0, [sp]
	ldr	r0, .L385+12
	bl	NAND_Print_DBG
	mov	r0, r4
	bl	nftl_free
	mov	r0, r5
	add	sp, sp, #76
	@ sp needed
	pop	{r4, r5, r6, r7, pc}
.L386:
	.align	2
.L385:
	.word	.LANCHOR2
	.word	.LC35
	.word	.LC36
	.word	.LC37
	.size	nand_dbg_phy_read, .-nand_dbg_phy_read
	.section	.text.nand_dbg_zone_phy_read,"ax",%progbits
	.align	1
	.global	nand_dbg_zone_phy_read
	.syntax unified
	.thumb
	.thumb_func
	.fpu softvfp
	.type	nand_dbg_zone_phy_read, %function
nand_dbg_zone_phy_read:
	@ args = 0, pretend = 0, frame = 32
	@ frame_needed = 0, uses_anonymous_args = 0
	push	{r4, r5, r6, r7, lr}
	mov	r5, r0
	sub	sp, sp, #92
	mov	r0, #4096
	mov	r7, r1
	mov	r6, r2
	bl	NAND_Malloc
	mov	r4, r0
	cbnz	r0, .L388
	ldr	r1, .L389
	ldr	r0, .L389+4
	bl	NAND_Print
.L388:
	ldr	r3, [r5, #4]
	movs	r1, #36
	muls	r1, r7, r1
	mov	r0, r5
	ldr	r3, [r3, #32]
	ldrh	r3, [r3, r1]
	add	r1, sp, #56
	strh	r6, [sp, #56]	@ movhi
	str	r4, [sp, #64]
	strh	r3, [sp, #58]	@ movhi
	movs	r3, #8
	strh	r3, [sp, #60]	@ movhi
	add	r3, sp, #72
	str	r3, [sp, #68]
	ldr	r3, [r5, #1980]
	blx	r3
	mov	r5, r0
	ldrb	r0, [sp, #87]	@ zero_extendqisi2
	ldrb	r3, [sp, #74]	@ zero_extendqisi2
	ldrb	r2, [sp, #73]	@ zero_extendqisi2
	str	r0, [sp, #48]
	ldrb	r0, [sp, #86]	@ zero_extendqisi2
	ldrb	r1, [sp, #72]	@ zero_extendqisi2
	str	r0, [sp, #44]
	ldrb	r0, [sp, #85]	@ zero_extendqisi2
	str	r0, [sp, #40]
	ldrb	r0, [sp, #84]	@ zero_extendqisi2
	str	r0, [sp, #36]
	ldrb	r0, [sp, #83]	@ zero_extendqisi2
	str	r0, [sp, #32]
	ldrb	r0, [sp, #82]	@ zero_extendqisi2
	str	r0, [sp, #28]
	ldrb	r0, [sp, #81]	@ zero_extendqisi2
	str	r0, [sp, #24]
	ldrb	r0, [sp, #80]	@ zero_extendqisi2
	str	r0, [sp, #20]
	ldrb	r0, [sp, #79]	@ zero_extendqisi2
	str	r0, [sp, #16]
	ldrb	r0, [sp, #78]	@ zero_extendqisi2
	str	r0, [sp, #12]
	ldrb	r0, [sp, #77]	@ zero_extendqisi2
	str	r0, [sp, #8]
	ldrb	r0, [sp, #76]	@ zero_extendqisi2
	str	r0, [sp, #4]
	ldrb	r0, [sp, #75]	@ zero_extendqisi2
	str	r0, [sp]
	ldr	r0, .L389+8
	bl	NAND_Print_DBG
	ldrb	r0, [r4, #10]	@ zero_extendqisi2
	ldrb	r3, [r4, #2]	@ zero_extendqisi2
	ldrb	r2, [r4, #1]	@ zero_extendqisi2
	ldrb	r1, [r4]	@ zero_extendqisi2
	str	r0, [sp, #28]
	ldrb	r0, [r4, #9]	@ zero_extendqisi2
	str	r0, [sp, #24]
	ldrb	r0, [r4, #8]	@ zero_extendqisi2
	str	r0, [sp, #20]
	ldrb	r0, [r4, #7]	@ zero_extendqisi2
	str	r0, [sp, #16]
	ldrb	r0, [r4, #6]	@ zero_extendqisi2
	str	r0, [sp, #12]
	ldrb	r0, [r4, #5]	@ zero_extendqisi2
	str	r0, [sp, #8]
	ldrb	r0, [r4, #4]	@ zero_extendqisi2
	str	r0, [sp, #4]
	ldrb	r0, [r4, #3]	@ zero_extendqisi2
	str	r0, [sp]
	ldr	r0, .L389+12
	bl	NAND_Print_DBG
	mov	r0, r4
	bl	nftl_free
	mov	r0, r5
	add	sp, sp, #92
	@ sp needed
	pop	{r4, r5, r6, r7, pc}
.L390:
	.align	2
.L389:
	.word	.LANCHOR3
	.word	.LC35
	.word	.LC36
	.word	.LC37
	.size	nand_dbg_zone_phy_read, .-nand_dbg_zone_phy_read
	.section	.text.nand_dbg_zone_phy_write,"ax",%progbits
	.align	1
	.global	nand_dbg_zone_phy_write
	.syntax unified
	.thumb
	.thumb_func
	.fpu softvfp
	.type	nand_dbg_zone_phy_write, %function
nand_dbg_zone_phy_write:
	@ args = 0, pretend = 0, frame = 32
	@ frame_needed = 0, uses_anonymous_args = 0
	push	{r4, r5, r6, r7, r8, lr}
	mov	r5, r0
	sub	sp, sp, #32
	mov	r0, #8192
	mov	r6, r1
	mov	r7, r2
	bl	NAND_Malloc
	mov	r4, r0
	cbnz	r0, .L392
	ldr	r1, .L393
	ldr	r0, .L393+4
	bl	NAND_Print
.L392:
	ldr	r3, [r5, #4]
	movs	r2, #36
	muls	r6, r2, r6
	mov	r0, r4
	mov	r2, #4096
	movs	r1, #165
	ldr	r8, [r3, #32]
	bl	NAND_Memset
	ldr	r2, .L393+8
	mov	r3, #-1
	strh	r3, [sp, #24]	@ movhi
	mov	r1, sp
	mov	r0, r5
	strd	r3, r2, [sp, #16]
	movw	r3, #65535
	strb	r3, [sp, #26]
	ldr	r3, [r5, #4]
	ldrh	r2, [r8, r6]
	strh	r7, [sp]	@ movhi
	ldrh	r3, [r3, #28]
	strh	r2, [sp, #2]	@ movhi
	str	r4, [sp, #8]
	strh	r3, [sp, #4]	@ movhi
	add	r3, sp, #16
	str	r3, [sp, #12]
	ldr	r3, [r5, #1984]
	blx	r3
	mov	r5, r0
	mov	r0, r4
	bl	nftl_free
	mov	r0, r5
	add	sp, sp, #32
	@ sp needed
	pop	{r4, r5, r6, r7, r8, pc}
.L394:
	.align	2
.L393:
	.word	.LANCHOR4
	.word	.LC38
	.word	-5921281
	.size	nand_dbg_zone_phy_write, .-nand_dbg_zone_phy_write
	.section	.text.nand_dbg_phy_write,"ax",%progbits
	.align	1
	.global	nand_dbg_phy_write
	.syntax unified
	.thumb
	.thumb_func
	.fpu softvfp
	.type	nand_dbg_phy_write, %function
nand_dbg_phy_write:
	@ args = 0, pretend = 0, frame = 16
	@ frame_needed = 0, uses_anonymous_args = 0
	push	{r4, r5, r6, r7, lr}
	mov	r5, r0
	sub	sp, sp, #28
	mov	r0, #8192
	mov	r6, r1
	mov	r7, r2
	bl	NAND_Malloc
	mov	r4, r0
	cbnz	r0, .L396
	ldr	r1, .L397
	ldr	r0, .L397+4
	bl	NAND_Print
.L396:
	mov	r0, r4
	mov	r2, #4096
	movs	r1, #165
	bl	NAND_Memset
	ldr	r2, .L397+8
	mov	r3, #-1
	strh	r3, [sp, #16]	@ movhi
	mov	r1, r6
	mov	r0, r5
	strd	r3, r2, [sp, #8]
	movw	r3, #65535
	strb	r3, [sp, #18]
	add	r3, sp, #8
	strd	r4, r3, [sp]
	mov	r2, r7
	movs	r3, #8
	bl	page_write
	mov	r5, r0
	mov	r0, r4
	bl	nftl_free
	mov	r0, r5
	add	sp, sp, #28
	@ sp needed
	pop	{r4, r5, r6, r7, pc}
.L398:
	.align	2
.L397:
	.word	.LANCHOR5
	.word	.LC38
	.word	-5921281
	.size	nand_dbg_phy_write, .-nand_dbg_phy_write
	.section	.text.nand_dbg_phy_erase,"ax",%progbits
	.align	1
	.global	nand_dbg_phy_erase
	.syntax unified
	.thumb
	.thumb_func
	.fpu softvfp
	.type	nand_dbg_phy_erase, %function
nand_dbg_phy_erase:
	@ args = 0, pretend = 0, frame = 0
	@ frame_needed = 0, uses_anonymous_args = 0
	@ link register save eliminated.
	b	block_erase
	.size	nand_dbg_phy_erase, .-nand_dbg_phy_erase
	.section	.text.nand_dbg_single_phy_erase,"ax",%progbits
	.align	1
	.global	nand_dbg_single_phy_erase
	.syntax unified
	.thumb
	.thumb_func
	.fpu softvfp
	.type	nand_dbg_single_phy_erase, %function
nand_dbg_single_phy_erase:
	@ args = 0, pretend = 0, frame = 0
	@ frame_needed = 0, uses_anonymous_args = 0
	@ link register save eliminated.
	b	nand_physic_erase_block
	.size	nand_dbg_single_phy_erase, .-nand_dbg_single_phy_erase
	.section	.text.clean_block_list,"ax",%progbits
	.align	1
	.global	clean_block_list
	.syntax unified
	.thumb
	.thumb_func
	.fpu softvfp
	.type	clean_block_list, %function
clean_block_list:
	@ args = 0, pretend = 0, frame = 0
	@ frame_needed = 0, uses_anonymous_args = 0
	push	{r4, r5, r6, lr}
	mov	r5, r0
	movs	r4, #0
	mov	r6, #-1
.L403:
	mov	r0, r5
	bl	out_phy_block_from_free_list
	cbnz	r0, .L406
	mov	r1, r0
	mov	r4, #-1
.L402:
	mov	r0, r5
	bl	out_phy_block_from_invalid_page_list
	cbz	r0, .L401
	strh	r1, [r0, #4]	@ movhi
	strh	r1, [r0, #2]	@ movhi
	strd	r4, r1, [r0, #8]
	strd	r1, r1, [r0, #16]
	strd	r1, r1, [r0, #24]
	str	r1, [r0, #32]
	b	.L402
.L406:
	strh	r4, [r0, #4]	@ movhi
	strh	r4, [r0, #2]	@ movhi
	strd	r6, r4, [r0, #8]
	strd	r4, r4, [r0, #16]
	strd	r4, r4, [r0, #24]
	str	r4, [r0, #32]
	b	.L403
.L401:
	pop	{r4, r5, r6, pc}
	.size	clean_block_list, .-clean_block_list
	.section	.text.nand_clean_zone_table2,"ax",%progbits
	.align	1
	.global	nand_clean_zone_table2
	.syntax unified
	.thumb
	.thumb_func
	.fpu softvfp
	.type	nand_clean_zone_table2, %function
nand_clean_zone_table2:
	@ args = 0, pretend = 0, frame = 0
	@ frame_needed = 0, uses_anonymous_args = 0
	ldr	r3, [r0, #4]
	push	{r4, r5, r6, r7, r8, r10, fp, lr}
	mov	r4, r0
	ldr	r0, [r0, #24]
	add	r8, r4, #52
	mov	r5, r4
	ldrb	r1, [r3, #8]	@ zero_extendqisi2
	bl	__aeabi_uidiv
	lsls	r6, r0, #2
	ldr	fp, .L416+4
	mov	r2, r0
	ldrb	r1, [r4, #16]	@ zero_extendqisi2
	lsrs	r7, r6, #20
	ldr	r0, .L416
	add	r10, r8, r7, lsl #2
	bl	NAND_Print_DBG
.L408:
	cmp	r8, r10
	bne	.L409
	ubfx	r6, r6, #0, #20
	cbz	r6, .L410
	add	r7, r4, r7, lsl #2
	movs	r1, #255
	mov	r2, r6
	ldr	r0, [r7, #52]
	bl	NAND_Memset
	mov	r1, r6
	ldr	r0, .L416+4
	bl	NAND_Print_DBG
.L410:
	movs	r3, #0
	movw	r2, #65535
	strh	r2, [r4, #320]	@ movhi
	strh	r2, [r4, #352]	@ movhi
	mov	r2, r3
	str	r3, [r4, #292]
	str	r3, [r4, #324]
	strd	r3, r3, [r4, #172]
	strd	r3, r3, [r4, #164]
	strd	r3, r3, [r4, #180]
	strd	r3, r3, [r4, #208]
	strd	r3, r3, [r4, #200]
	strd	r3, r3, [r4, #216]
	strd	r3, r3, [r4, #244]
	strd	r3, r3, [r4, #236]
	strd	r3, r3, [r4, #252]
	str	r3, [r4, #20]
	strh	r3, [r4, #392]	@ movhi
.L411:
	strh	r3, [r5, #396]	@ movhi
	adds	r3, r3, #1
	cmp	r3, #10
	strh	r2, [r5, #398]	@ movhi
	strd	r2, r2, [r5, #400]
	add	r5, r5, #16
	str	r2, [r5, #392]
	bne	.L411
	movs	r3, #255
	strh	r2, [r4, #394]	@ movhi
	strd	r3, r2, [r4, #556]
	movs	r0, #0
	strd	r2, r2, [r4, #564]
	pop	{r4, r5, r6, r7, r8, r10, fp, pc}
.L409:
	ldr	r0, [r8], #4
	mov	r2, #1048576
	movs	r1, #255
	bl	NAND_Memset
	mov	r1, #1048576
	mov	r0, fp
	bl	NAND_Print_DBG
	b	.L408
.L417:
	.align	2
.L416:
	.word	.LC39
	.word	.LC40
	.size	nand_clean_zone_table2, .-nand_clean_zone_table2
	.section	.text.nftl_sector_cache_init,"ax",%progbits
	.align	1
	.global	nftl_sector_cache_init
	.syntax unified
	.thumb
	.thumb_func
	.fpu softvfp
	.type	nftl_sector_cache_init, %function
nftl_sector_cache_init:
	@ args = 0, pretend = 0, frame = 0
	@ frame_needed = 0, uses_anonymous_args = 0
	push	{r4, r5, r6, lr}
	movs	r3, #16
	movs	r5, #0
	mov	r4, r0
	str	r3, [r0, #1620]
	str	r5, [r0, #1624]
	mov	r0, #8192
	bl	NAND_Malloc
	str	r0, [r4, #1624]
	cbnz	r0, .L419
	ldr	r1, .L423
	ldr	r0, .L423+4
	bl	NAND_Print
	movs	r0, #1
.L418:
	pop	{r4, r5, r6, pc}
.L419:
	mov	r2, r5
	str	r5, [r4, #1960]
	str	r5, [r4, #1964]
	mov	r3, r4
	movs	r6, #255
	mov	r5, #-1
	mov	r1, r2
.L421:
	strh	r2, [r3, #1628]	@ movhi
	add	r4, r0, r2, lsl #9
	adds	r2, r2, #1
	strh	r6, [r3, #1630]	@ movhi
	cmp	r2, #16
	str	r5, [r3, #1632]
	str	r1, [r3, #1640]
	add	r3, r3, #20
	str	r1, [r3, #1624]
	str	r4, [r3, #1616]
	bne	.L421
	movs	r0, #0
	b	.L418
.L424:
	.align	2
.L423:
	.word	.LANCHOR6
	.word	.LC41
	.size	nftl_sector_cache_init, .-nftl_sector_cache_init
	.section	.text.nftl_sector_cache_exit,"ax",%progbits
	.align	1
	.global	nftl_sector_cache_exit
	.syntax unified
	.thumb
	.thumb_func
	.fpu softvfp
	.type	nftl_sector_cache_exit, %function
nftl_sector_cache_exit:
	@ args = 0, pretend = 0, frame = 0
	@ frame_needed = 0, uses_anonymous_args = 0
	push	{r3, lr}
	ldr	r0, [r0, #1624]
	bl	nftl_free
	movs	r0, #0
	pop	{r3, pc}
	.size	nftl_sector_cache_exit, .-nftl_sector_cache_exit
	.section	.text.search_from_sector_cache_write_list,"ax",%progbits
	.align	1
	.global	search_from_sector_cache_write_list
	.syntax unified
	.thumb
	.thumb_func
	.fpu softvfp
	.type	search_from_sector_cache_write_list, %function
search_from_sector_cache_write_list:
	@ args = 0, pretend = 0, frame = 0
	@ frame_needed = 0, uses_anonymous_args = 0
	@ link register save eliminated.
	ldr	r0, [r0, #340]
.L427:
	cbnz	r0, .L429
.L426:
	bx	lr
.L429:
	ldr	r3, [r0, #4]
	cmp	r3, r1
	beq	.L426
	ldr	r0, [r0, #12]
	b	.L427
	.size	search_from_sector_cache_write_list, .-search_from_sector_cache_write_list
	.section	.text.get_empty_sector_cahce_node,"ax",%progbits
	.align	1
	.global	get_empty_sector_cahce_node
	.syntax unified
	.thumb
	.thumb_func
	.fpu softvfp
	.type	get_empty_sector_cahce_node, %function
get_empty_sector_cahce_node:
	@ args = 0, pretend = 0, frame = 0
	@ frame_needed = 0, uses_anonymous_args = 0
	movs	r3, #0
	movs	r1, #20
	push	{r4, lr}
.L433:
	mul	r2, r1, r3
	adds	r4, r0, r2
	ldrh	r4, [r4, #10]
	cmp	r4, #255
	bne	.L431
	adds	r2, r2, #8
	add	r0, r0, r2
.L430:
	pop	{r4, pc}
.L431:
	adds	r3, r3, #1
	cmp	r3, #16
	bne	.L433
	movs	r0, #0
	b	.L430
	.size	get_empty_sector_cahce_node, .-get_empty_sector_cahce_node
	.section	.text.add_to_sector_cache_write_list_tail,"ax",%progbits
	.align	1
	.global	add_to_sector_cache_write_list_tail
	.syntax unified
	.thumb
	.thumb_func
	.fpu softvfp
	.type	add_to_sector_cache_write_list_tail, %function
add_to_sector_cache_write_list_tail:
	@ args = 0, pretend = 0, frame = 0
	@ frame_needed = 0, uses_anonymous_args = 0
	@ link register save eliminated.
	add	r2, r0, #328
.L436:
	ldr	r3, [r2, #12]
	cbnz	r3, .L437
	str	r1, [r2, #12]
	strd	r3, r2, [r1, #12]
	movs	r2, #1
	strh	r2, [r1, #2]	@ movhi
	ldrh	r2, [r0, #2]
	adds	r2, r2, #1
	strh	r2, [r0, #2]	@ movhi
	mov	r0, r3
	bx	lr
.L437:
	mov	r2, r3
	b	.L436
	.size	add_to_sector_cache_write_list_tail, .-add_to_sector_cache_write_list_tail
	.section	.text.del_from_sector_cache_write_list,"ax",%progbits
	.align	1
	.global	del_from_sector_cache_write_list
	.syntax unified
	.thumb
	.thumb_func
	.fpu softvfp
	.type	del_from_sector_cache_write_list, %function
del_from_sector_cache_write_list:
	@ args = 0, pretend = 0, frame = 0
	@ frame_needed = 0, uses_anonymous_args = 0
	@ link register save eliminated.
	ldrd	r3, r2, [r1, #12]
	str	r3, [r2, #12]
	ldr	r3, [r1, #12]
	cbz	r3, .L439
	str	r2, [r3, #16]
.L439:
	movs	r3, #0
	strd	r3, r3, [r1, #12]
	movs	r3, #255
	strh	r3, [r1, #2]	@ movhi
	ldrh	r3, [r0, #2]
	subs	r3, r3, #1
	strh	r3, [r0, #2]	@ movhi
	mov	r0, r1
	bx	lr
	.size	del_from_sector_cache_write_list, .-del_from_sector_cache_write_list
	.section	.text.set_spare_data,"ax",%progbits
	.align	1
	.global	set_spare_data
	.syntax unified
	.thumb
	.thumb_func
	.fpu softvfp
	.type	set_spare_data, %function
set_spare_data:
	@ args = 0, pretend = 0, frame = 0
	@ frame_needed = 0, uses_anonymous_args = 0
	push	{r4, r5, r6, lr}
	add	r6, r3, #536870912
	movs	r4, #0
	add	r2, r2, r0
	subs	r6, r6, #1
.L444:
	cmp	r3, r4
	bhi	.L445
	pop	{r4, r5, r6, pc}
.L445:
	subs	r5, r6, r4
	lsls	r5, r5, #3
	lsr	r5, r1, r5
	strb	r5, [r2, r4]
	adds	r4, r4, #1
	b	.L444
	.size	set_spare_data, .-set_spare_data
	.section	.text.get_spare_data,"ax",%progbits
	.align	1
	.global	get_spare_data
	.syntax unified
	.thumb
	.thumb_func
	.fpu softvfp
	.type	get_spare_data, %function
get_spare_data:
	@ args = 0, pretend = 0, frame = 0
	@ frame_needed = 0, uses_anonymous_args = 0
	movs	r3, #0
	push	{r4, lr}
	add	r1, r1, r0
	mov	r4, r3
.L447:
	cmp	r2, r3
	bhi	.L448
	mov	r0, r4
	pop	{r4, pc}
.L448:
	ldrb	r0, [r1, r3]	@ zero_extendqisi2
	adds	r3, r3, #1
	orr	r4, r0, r4, lsl #8
	b	.L447
	.size	get_spare_data, .-get_spare_data
	.section	.text.set_oob_logic_page,"ax",%progbits
	.align	1
	.global	set_oob_logic_page
	.syntax unified
	.thumb
	.thumb_func
	.fpu softvfp
	.type	set_oob_logic_page, %function
set_oob_logic_page:
	@ args = 4, pretend = 0, frame = 0
	@ frame_needed = 0, uses_anonymous_args = 0
	push	{r3, r4, r5, lr}
	mov	r4, r1
	mov	r5, r3
	movs	r3, #255
	mov	r1, r2
	mov	r0, r4
	strb	r3, [r4]
	movs	r2, #1
	movs	r3, #4
	bl	set_spare_data
	movs	r3, #2
	movs	r2, #5
	ldrh	r1, [sp, #16]
	bl	set_spare_data
	movs	r3, #4
	movs	r2, #7
	mov	r1, r5
	bl	set_spare_data
	ldrb	r3, [r4, #1]	@ zero_extendqisi2
	orn	r3, r3, #63
	strb	r3, [r4, #1]
	pop	{r3, r4, r5, pc}
	.size	set_oob_logic_page, .-set_oob_logic_page
	.section	.text.set_oob_special_page,"ax",%progbits
	.align	1
	.global	set_oob_special_page
	.syntax unified
	.thumb
	.thumb_func
	.fpu softvfp
	.type	set_oob_special_page, %function
set_oob_special_page:
	@ args = 4, pretend = 0, frame = 0
	@ frame_needed = 0, uses_anonymous_args = 0
	push	{r4, r5, r6, lr}
	mov	r4, r1
	ldrh	r6, [sp, #16]
	mov	r5, r3
	movs	r3, #255
	mov	r1, r2
	strb	r3, [r4]
	mov	r0, r4
	movs	r3, #4
	movs	r2, #1
	bl	set_spare_data
	mov	r1, r6
	movs	r3, #2
	movs	r2, #5
	bl	set_spare_data
	mov	r1, r5
	movs	r3, #4
	movs	r2, #7
	pop	{r4, r5, r6, lr}
	b	set_spare_data
	.size	set_oob_special_page, .-set_oob_special_page
	.section	.text.mark_free_block,"ax",%progbits
	.align	1
	.global	mark_free_block
	.syntax unified
	.thumb
	.thumb_func
	.fpu softvfp
	.type	mark_free_block, %function
mark_free_block:
	@ args = 0, pretend = 0, frame = 32
	@ frame_needed = 0, uses_anonymous_args = 0
	push	{r4, r5, r6, r7, lr}
	sub	sp, sp, #44
	add	r6, sp, #24
	mov	r4, r0
	mov	r5, r1
	mov	r0, r6
	movs	r2, #16
	movs	r1, #165
	bl	NAND_Memset
	ldr	r3, [r4, #4]
	movs	r7, #0
	ldrh	r1, [r5]
	mov	r0, r4
	strh	r7, [sp, #8]	@ movhi
	ldrh	r2, [r3, #28]
	ldr	r3, [r4, #284]
	strh	r1, [sp, #10]	@ movhi
	mov	r1, r6
	strh	r2, [sp, #12]	@ movhi
	mov	r2, #1431655765
	strd	r3, r6, [sp, #16]
	ldrh	r3, [r5, #6]
	str	r3, [sp]
	ldr	r3, [r5, #8]
	bl	set_oob_special_page
	ldr	r3, [r4, #1984]
	add	r1, sp, #8
	mov	r0, r4
	blx	r3
	ldr	r3, [r4, #4]
	movs	r0, #1
	ldrh	r1, [r5]
	strh	r0, [sp, #8]	@ movhi
	mov	r0, r4
	ldrh	r2, [r3, #28]
	ldr	r3, [r4, #284]
	strh	r1, [sp, #10]	@ movhi
	mov	r1, r6
	strh	r2, [sp, #12]	@ movhi
	mov	r2, #1431655765
	strd	r3, r6, [sp, #16]
	ldrh	r3, [r5, #6]
	str	r3, [sp]
	ldr	r3, [r5, #8]
	bl	set_oob_special_page
	ldr	r3, [r4, #1984]
	add	r1, sp, #8
	mov	r0, r4
	blx	r3
	ldr	r3, [r4, #4]
	ldrh	r0, [r5]
	ldr	r2, [r4, #284]
	ldrh	r1, [r3, #28]
	ldrh	r3, [r3, #20]
	strh	r0, [sp, #10]	@ movhi
	mov	r0, r4
	strh	r1, [sp, #12]	@ movhi
	mov	r1, r6
	subs	r3, r3, #2
	strd	r2, r6, [sp, #16]
	strh	r3, [sp, #8]	@ movhi
	mov	r2, #1431655765
	ldrh	r3, [r5, #6]
	str	r3, [sp]
	ldr	r3, [r5, #8]
	bl	set_oob_special_page
	ldr	r3, [r4, #1984]
	add	r1, sp, #8
	mov	r0, r4
	blx	r3
	ldr	r3, [r4, #4]
	ldrh	r0, [r5]
	ldr	r2, [r4, #284]
	ldrh	r1, [r3, #28]
	ldrh	r3, [r3, #20]
	strh	r0, [sp, #10]	@ movhi
	mov	r0, r4
	strh	r1, [sp, #12]	@ movhi
	mov	r1, r6
	subs	r3, r3, #1
	strd	r2, r6, [sp, #16]
	strh	r3, [sp, #8]	@ movhi
	mov	r2, #1431655765
	ldrh	r3, [r5, #6]
	str	r3, [sp]
	ldr	r3, [r5, #8]
	bl	set_oob_special_page
	ldr	r3, [r4, #1984]
	add	r1, sp, #8
	mov	r0, r4
	blx	r3
	mov	r0, r7
	add	sp, sp, #44
	@ sp needed
	pop	{r4, r5, r6, r7, pc}
	.size	mark_free_block, .-mark_free_block
	.section	.text.erase_block,"ax",%progbits
	.align	1
	.global	erase_block
	.syntax unified
	.thumb
	.thumb_func
	.fpu softvfp
	.type	erase_block, %function
erase_block:
	@ args = 0, pretend = 0, frame = 16
	@ frame_needed = 0, uses_anonymous_args = 0
	push	{r0, r1, r2, r3, r4, r5, r6, r7, r8, lr}
	mov	r5, r0
	mov	r4, r1
	ldr	r0, .L455
	mov	r8, r2
	movs	r6, #0
	ldrh	r1, [r1]
	bl	NAND_Print
	ldr	r3, [r5, #4]
	mov	r1, sp
	ldrh	r2, [r4]
	mov	r0, r5
	strh	r6, [sp]	@ movhi
	ldrh	r3, [r3, #28]
	strh	r2, [sp, #2]	@ movhi
	strd	r6, r6, [sp, #8]
	strh	r3, [sp, #4]	@ movhi
	ldr	r3, [r5, #1976]
	blx	r3
	mov	r7, r0
	cbz	r0, .L453
	ldrh	r1, [r4]
	ldr	r0, .L455+4
	bl	NAND_Print
	ldr	r3, [r5, #1996]
	mov	r1, sp
	mov	r0, r5
	blx	r3
	movs	r3, #255
	strh	r6, [r4, #4]	@ movhi
	strh	r3, [r4, #2]	@ movhi
	ldrh	r3, [r5, #20]
	adds	r3, r3, #1
	strh	r3, [r5, #20]	@ movhi
.L452:
	mov	r0, r7
	add	sp, sp, #16
	@ sp needed
	pop	{r4, r5, r6, r7, r8, pc}
.L453:
	mov	r3, #-1
	mov	r1, r4
	str	r3, [r4, #8]
	mov	r0, r5
	strh	r8, [r4, #6]	@ movhi
	bl	put_phy_block_to_free_list
	mov	r1, r4
	mov	r0, r5
	bl	mark_free_block
	ldr	r3, [r5, #4]
	ldrh	r3, [r3, #20]
	strh	r7, [r4, #2]	@ movhi
	strh	r3, [r4, #4]	@ movhi
	b	.L452
.L456:
	.align	2
.L455:
	.word	.LC42
	.word	.LC43
	.size	erase_block, .-erase_block
	.section	.text.adjust_free_list,"ax",%progbits
	.align	1
	.global	adjust_free_list
	.syntax unified
	.thumb
	.thumb_func
	.fpu softvfp
	.type	adjust_free_list, %function
adjust_free_list:
	@ args = 0, pretend = 0, frame = 0
	@ frame_needed = 0, uses_anonymous_args = 0
	push	{r3, r4, r5, lr}
	mov	r5, r0
	bl	out_phy_block_from_free_list
	mov	r4, r0
	cbnz	r0, .L458
	ldr	r0, .L459
	bl	NAND_Print
.L458:
	ldrh	r2, [r4, #6]
	mov	r1, r4
	mov	r0, r5
	adds	r2, r2, #1
	uxth	r2, r2
	bl	erase_block
	mov	r1, r4
	mov	r0, r5
	bl	out_phy_block_from_free_list_by_block
	str	r0, [r5, #48]
	ldrh	r2, [r0, #6]
	ldrh	r1, [r0]
	ldr	r0, .L459+4
	bl	NAND_Print
	movs	r0, #0
	pop	{r3, r4, r5, pc}
.L460:
	.align	2
.L459:
	.word	.LC44
	.word	.LC45
	.size	adjust_free_list, .-adjust_free_list
	.section	.text.nand_dbg_zone_erase,"ax",%progbits
	.align	1
	.global	nand_dbg_zone_erase
	.syntax unified
	.thumb
	.thumb_func
	.fpu softvfp
	.type	nand_dbg_zone_erase, %function
nand_dbg_zone_erase:
	@ args = 0, pretend = 0, frame = 0
	@ frame_needed = 0, uses_anonymous_args = 0
	push	{r3, r4, r5, lr}
	mov	r5, r2
	ldr	r3, [r0, #4]
	movs	r2, #36
	mov	r4, r0
	ldr	r3, [r3, #32]
	mla	r1, r2, r1, r3
	bl	out_phy_block_from_free_list_by_block
	mov	r2, r5
	mov	r1, r0
	mov	r0, r4
	pop	{r3, r4, r5, lr}
	b	erase_block
	.size	nand_dbg_zone_erase, .-nand_dbg_zone_erase
	.section	.text.torture_block_and_mark,"ax",%progbits
	.align	1
	.global	torture_block_and_mark
	.syntax unified
	.thumb
	.thumb_func
	.fpu softvfp
	.type	torture_block_and_mark, %function
torture_block_and_mark:
	@ args = 0, pretend = 0, frame = 32
	@ frame_needed = 0, uses_anonymous_args = 0
	push	{r4, r5, r6, r7, r8, r10, lr}
	sub	sp, sp, #44
	add	r7, sp, #8
	mov	r4, r0
	mov	r5, r1
	movs	r2, #15
	movs	r1, #0
	mov	r0, r7
	bl	memset
	ldrh	r1, [r5]
	ldr	r0, .L478
	bl	NAND_Print
	ldr	r3, [r4, #4]
	mov	r0, r4
	ldrh	r1, [r5]
	ldrh	r2, [r3, #28]
	movs	r3, #0
	strh	r3, [sp, #24]	@ movhi
	strh	r1, [sp, #26]	@ movhi
	add	r1, sp, #24
	strd	r3, r3, [sp, #32]
	strh	r2, [sp, #28]	@ movhi
	ldr	r3, [r4, #1976]
	blx	r3
	mov	r6, r0
	cbz	r0, .L463
	ldrh	r1, [r5]
	ldr	r0, .L478+4
.L475:
	bl	NAND_Print
	b	.L464
.L463:
	ldrh	r3, [r5, #6]
	movs	r1, #165
	ldr	r0, [r4, #284]
	mov	r8, r6
	adds	r3, r3, #1
	strh	r3, [r5, #6]	@ movhi
	ldr	r3, [r4, #4]
	ldrh	r2, [r3, #26]
	bl	NAND_Memset
	movs	r2, #15
	movs	r1, #165
	mov	r0, r7
	bl	NAND_Memset
.L466:
	ldr	r3, [r4, #4]
	mov	r0, r4
	ldrh	r1, [r5]
	strh	r8, [sp, #24]	@ movhi
	ldrh	r2, [r3, #28]
	ldr	r3, [r4, #284]
	strh	r1, [sp, #26]	@ movhi
	add	r1, sp, #24
	strh	r2, [sp, #28]	@ movhi
	strd	r3, r7, [sp, #32]
	ldr	r3, [r4, #1984]
	blx	r3
	mov	r6, r0
	cbz	r0, .L465
	ldrh	r2, [r5]
	mov	r3, r8
	ldr	r1, .L478+8
	ldr	r0, .L478+12
.L476:
	bl	NAND_Print
.L464:
	ldr	r3, [r4, #1996]
	add	r1, sp, #24
	mov	r0, r4
	blx	r3
	movs	r3, #0
	strh	r3, [r5, #4]	@ movhi
	movs	r3, #255
	strh	r3, [r5, #2]	@ movhi
	ldrh	r3, [r4, #20]
	adds	r3, r3, #1
	strh	r3, [r4, #20]	@ movhi
	b	.L462
.L465:
	add	r8, r8, #1
	cmp	r8, #3
	bne	.L466
	mov	r8, r0
.L470:
	ldr	r3, [r4, #4]
	movs	r1, #0
	ldr	r0, [r4, #284]
	ldrh	r2, [r3, #26]
	bl	NAND_Memset
	movs	r2, #15
	movs	r1, #0
	mov	r0, r7
	bl	NAND_Memset
	ldr	r3, [r4, #4]
	mov	r0, r4
	ldrh	r1, [r5]
	strh	r8, [sp, #24]	@ movhi
	ldrh	r2, [r3, #28]
	ldr	r3, [r4, #284]
	strh	r1, [sp, #26]	@ movhi
	add	r1, sp, #24
	strh	r2, [sp, #28]	@ movhi
	strd	r3, r7, [sp, #32]
	ldr	r3, [r4, #1980]
	blx	r3
	mov	r6, r0
	cbz	r0, .L467
	mov	r3, r8
	ldrh	r2, [r5]
	ldr	r1, .L478+8
	ldr	r0, .L478+16
	b	.L476
.L467:
	ldr	r10, [r4, #4]
	movs	r1, #165
	ldr	r0, [r4, #284]
	ldrh	r2, [r10, #26]
	bl	nftl_check_pattern
	mov	r6, r0
	cbz	r0, .L468
	ldrh	r2, [r5]
	movs	r3, #165
	ldr	r1, .L478+8
	ldr	r0, .L478+20
	str	r3, [sp]
	mov	r3, r8
.L477:
	bl	NAND_Print
	b	.L464
.L468:
	movs	r2, #15
	movs	r1, #165
	mov	r0, r7
	bl	nftl_check_pattern
	mov	r6, r0
	cbz	r0, .L469
	movs	r3, #165
	ldrh	r2, [r5]
	str	r3, [sp]
	mov	r3, r8
	ldr	r1, .L478+8
	ldr	r0, .L478+24
	b	.L477
.L469:
	add	r8, r8, #1
	cmp	r8, #3
	bne	.L470
	ldrh	r3, [r10, #28]
	add	r1, sp, #24
	ldrh	r2, [r5]
	strh	r0, [sp, #24]	@ movhi
	strh	r3, [sp, #28]	@ movhi
	strd	r0, r0, [sp, #32]
	mov	r0, r4
	strh	r2, [sp, #26]	@ movhi
	ldr	r3, [r4, #1976]
	blx	r3
	mov	r6, r0
	cbz	r0, .L471
	ldrh	r1, [r5]
	ldr	r0, .L478+28
	b	.L475
.L471:
	ldrh	r3, [r5, #6]
	mov	r1, r5
	mov	r0, r4
	adds	r3, r3, #1
	strh	r3, [r5, #6]	@ movhi
	mov	r3, #-1
	str	r3, [r5, #8]
	bl	put_phy_block_to_free_list
	mov	r1, r5
	mov	r0, r4
	bl	mark_free_block
	ldr	r3, [r4, #4]
	ldrh	r3, [r3, #20]
	strh	r6, [r5, #2]	@ movhi
	strh	r3, [r5, #4]	@ movhi
.L462:
	mov	r0, r6
	add	sp, sp, #44
	@ sp needed
	pop	{r4, r5, r6, r7, r8, r10, pc}
.L479:
	.align	2
.L478:
	.word	.LC46
	.word	.LC43
	.word	.LANCHOR7
	.word	.LC47
	.word	.LC48
	.word	.LC49
	.word	.LC50
	.word	.LC51
	.size	torture_block_and_mark, .-torture_block_and_mark
	.section	.text.set_no_use_page,"ax",%progbits
	.align	1
	.global	set_no_use_page
	.syntax unified
	.thumb
	.thumb_func
	.fpu softvfp
	.type	set_no_use_page, %function
set_no_use_page:
	@ args = 0, pretend = 0, frame = 0
	@ frame_needed = 0, uses_anonymous_args = 0
	movs	r3, #255
	push	{r4, lr}
	mov	r4, r1
	strb	r3, [r1]
	movs	r3, #1
	movs	r1, #170
	mov	r2, r3
	mov	r0, r4
	bl	set_spare_data
	movs	r3, #1
	movs	r2, #2
	movs	r1, #187
	pop	{r4, lr}
	b	set_spare_data
	.size	set_no_use_page, .-set_no_use_page
	.section	.text.set_start_block_flag,"ax",%progbits
	.align	1
	.global	set_start_block_flag
	.syntax unified
	.thumb
	.thumb_func
	.fpu softvfp
	.type	set_start_block_flag, %function
set_start_block_flag:
	@ args = 0, pretend = 0, frame = 0
	@ frame_needed = 0, uses_anonymous_args = 0
	movs	r3, #255
	push	{r4, r5, r6, lr}
	mov	r4, r1
	strb	r3, [r1]
	movs	r3, #1
	mov	r5, r2
	mov	r0, r4
	mov	r2, r3
	movs	r1, #170
	bl	set_spare_data
	movs	r3, #1
	movs	r2, #2
	movs	r1, #238
	bl	set_spare_data
	mov	r1, r5
	movs	r3, #4
	movs	r2, #3
	pop	{r4, r5, r6, lr}
	b	set_spare_data
	.size	set_start_block_flag, .-set_start_block_flag
	.section	.text.set_end_block_flag,"ax",%progbits
	.align	1
	.global	set_end_block_flag
	.syntax unified
	.thumb
	.thumb_func
	.fpu softvfp
	.type	set_end_block_flag, %function
set_end_block_flag:
	@ args = 0, pretend = 0, frame = 0
	@ frame_needed = 0, uses_anonymous_args = 0
	movs	r3, #255
	push	{r4, lr}
	mov	r4, r1
	strb	r3, [r1]
	movs	r3, #1
	movs	r1, #170
	mov	r2, r3
	mov	r0, r4
	bl	set_spare_data
	movs	r3, #1
	movs	r2, #2
	movs	r1, #204
	pop	{r4, lr}
	b	set_spare_data
	.size	set_end_block_flag, .-set_end_block_flag
	.section	.text.build_zone_list_first,"ax",%progbits
	.align	1
	.global	build_zone_list_first
	.syntax unified
	.thumb
	.thumb_func
	.fpu softvfp
	.type	build_zone_list_first, %function
build_zone_list_first:
	@ args = 0, pretend = 0, frame = 32
	@ frame_needed = 0, uses_anonymous_args = 0
	push	{r4, r5, r6, r7, r8, r10, lr}
	mov	r4, r0
	sub	sp, sp, #36
	ldr	r0, .L509
	mov	r10, r1
	bl	NAND_Print_DBG
	ldr	r3, [r4, #24]
	ldr	r5, [r4, #40]
	cmp	r3, #0
	beq	.L504
	ldr	r6, [r4, #4]
	ldr	r0, [r4, #32]
	ldrb	r1, [r6, #8]	@ zero_extendqisi2
	add	r0, r0, r3
	bl	__aeabi_uidiv
	ldrh	r1, [r6, #20]
	bl	__aeabi_uidiv
	mov	r8, r0
.L484:
	movs	r7, #0
	strh	r7, [r4, #18]	@ movhi
.L485:
	ldrh	r3, [r4, #18]
	cmp	r3, r8
	bcs	.L495
	ldr	r3, [r4, #4]
	mov	r0, r4
	ldrh	r1, [r5]
	str	r5, [r4, #44]
	ldrh	r2, [r3, #28]
	ldr	r3, [r4, #284]
	strh	r1, [sp, #18]	@ movhi
	add	r1, sp, #16
	strh	r7, [sp, #16]	@ movhi
	str	r3, [sp, #24]
	strh	r2, [sp, #20]	@ movhi
	str	sp, [sp, #28]
	ldr	r3, [r4, #1992]
	blx	r3
	cmp	r0, #1
	bne	.L486
	ldr	r3, [r4, #1976]
	add	r1, sp, #16
	mov	r0, r4
	blx	r3
	mov	r6, r0
	cbz	r0, .L487
	movs	r3, #255
	add	r1, sp, #16
	strh	r3, [r5, #2]	@ movhi
	mov	r0, r4
	ldr	r3, [r4, #1996]
	blx	r3
.L488:
	movw	r3, #65535
	str	r7, [r5, #20]
	strh	r3, [r5, #4]	@ movhi
	mov	r1, r4
	strd	r7, r7, [r5, #12]
	mov	r0, r5
	str	r5, [r4, #44]
	bl	is_last_phy_block
	cmp	r0, #1
	bne	.L491
.L495:
	ldrh	r1, [r4, #18]
	cmp	r1, #1
	bhi	.L492
	ldr	r0, .L509+4
	bl	NAND_Print
	movs	r0, #1
	b	.L483
.L504:
	mov	r8, #-1
	b	.L484
.L487:
	ldrh	r3, [r4, #18]
	adds	r3, r3, #1
	uxth	r3, r3
	cmp	r3, #1
	strh	r3, [r4, #18]	@ movhi
	strh	r0, [r5, #2]	@ movhi
	strh	r0, [r5, #6]	@ movhi
	bne	.L488
	movs	r2, #16
	movs	r1, #255
	mov	r0, sp
	bl	NAND_Memset
	mov	r2, r10
	mov	r1, sp
	mov	r0, r4
	bl	set_start_block_flag
	ldr	r3, [r4, #4]
	ldrh	r1, [r5]
	strh	r6, [sp, #16]	@ movhi
	ldrh	r2, [r3, #28]
	ldr	r3, [r4, #284]
	strh	r1, [sp, #18]	@ movhi
	strh	r2, [sp, #20]	@ movhi
	str	r3, [sp, #24]
	str	sp, [sp, #28]
.L489:
	ldr	r3, [r4, #4]
	ldrh	r3, [r3, #20]
	cmp	r3, r6
	bhi	.L490
	add	r3, r5, #36
	str	r3, [r4, #40]
	b	.L488
.L490:
	ldr	r3, [r4, #1984]
	add	r1, sp, #16
	mov	r0, r4
	adds	r6, r6, #1
	blx	r3
	ldrh	r3, [sp, #16]
	adds	r3, r3, #1
	strh	r3, [sp, #16]	@ movhi
	b	.L489
.L486:
	movs	r3, #255
	strh	r3, [r5, #2]	@ movhi
	b	.L488
.L491:
	adds	r5, r5, #36
	b	.L485
.L492:
	cmp	r8, #-1
	add	r1, r1, #-1
	strh	r1, [r4, #18]	@ movhi
	bne	.L496
.L498:
	movs	r0, #0
.L483:
	add	sp, sp, #36
	@ sp needed
	pop	{r4, r5, r6, r7, r8, r10, pc}
.L496:
	mov	r1, r4
	mov	r0, r5
	bl	is_last_phy_block
	cmp	r0, #1
	beq	.L498
	movs	r7, #0
.L499:
	ldr	r2, [r4, #4]
	ldrh	r3, [r5]
	ldrh	r1, [r2, #24]
	cmp	r1, r3
	bls	.L498
	ldrh	r1, [r2, #28]
	mov	r0, r4
	ldr	r2, [r4, #284]
	strh	r3, [sp, #18]	@ movhi
	strh	r1, [sp, #20]	@ movhi
	add	r1, sp, #16
	strh	r7, [sp, #16]	@ movhi
	str	r2, [sp, #24]
	str	sp, [sp, #28]
	ldr	r3, [r4, #1992]
	blx	r3
	cmp	r0, #1
	bne	.L500
	ldr	r3, [r4, #1976]
	add	r1, sp, #16
	mov	r0, r4
	blx	r3
	mov	r6, r0
	cbz	r0, .L501
	movs	r3, #255
	add	r1, sp, #16
	strh	r3, [r5, #2]	@ movhi
	mov	r0, r4
	ldr	r3, [r4, #1996]
	blx	r3
.L502:
	adds	r5, r5, #36
	b	.L499
.L501:
	movs	r2, #16
	mov	r0, sp
	movs	r1, #255
	bl	NAND_Memset
	mov	r1, sp
	mov	r0, r4
	bl	set_end_block_flag
	ldr	r3, [r4, #4]
	mov	r0, r4
	ldrh	r1, [r5]
	strh	r6, [sp, #16]	@ movhi
	ldrh	r2, [r3, #28]
	ldr	r3, [r4, #284]
	strh	r1, [sp, #18]	@ movhi
	add	r1, sp, #16
	strh	r2, [sp, #20]	@ movhi
	str	r3, [sp, #24]
	str	sp, [sp, #28]
	ldr	r3, [r4, #1984]
	blx	r3
	b	.L498
.L500:
	movs	r3, #255
	strh	r3, [r5, #2]	@ movhi
	b	.L502
.L510:
	.align	2
.L509:
	.word	.LC52
	.word	.LC53
	.size	build_zone_list_first, .-build_zone_list_first
	.section	.text.get_bad_flag_from_oob,"ax",%progbits
	.align	1
	.global	get_bad_flag_from_oob
	.syntax unified
	.thumb
	.thumb_func
	.fpu softvfp
	.type	get_bad_flag_from_oob, %function
get_bad_flag_from_oob:
	@ args = 0, pretend = 0, frame = 0
	@ frame_needed = 0, uses_anonymous_args = 0
	push	{r3, lr}
	movs	r2, #4
	movs	r1, #1
	bl	get_spare_data
	adds	r0, r0, #0
	it	ne
	movne	r0, #1
	pop	{r3, pc}
	.size	get_bad_flag_from_oob, .-get_bad_flag_from_oob
	.section	.text.get_block_used_count_from_oob,"ax",%progbits
	.align	1
	.global	get_block_used_count_from_oob
	.syntax unified
	.thumb
	.thumb_func
	.fpu softvfp
	.type	get_block_used_count_from_oob, %function
get_block_used_count_from_oob:
	@ args = 0, pretend = 0, frame = 0
	@ frame_needed = 0, uses_anonymous_args = 0
	@ link register save eliminated.
	movs	r2, #4
	movs	r1, #7
	b	get_spare_data
	.size	get_block_used_count_from_oob, .-get_block_used_count_from_oob
	.section	.text.get_used_block_count,"ax",%progbits
	.align	1
	.global	get_used_block_count
	.syntax unified
	.thumb
	.thumb_func
	.fpu softvfp
	.type	get_used_block_count, %function
get_used_block_count:
	@ args = 0, pretend = 0, frame = 32
	@ frame_needed = 0, uses_anonymous_args = 0
	ldrh	r2, [r1]
	movs	r3, #0
	push	{r4, lr}
	sub	sp, sp, #32
	add	r4, sp, #16
	strh	r3, [sp]	@ movhi
	mov	r1, sp
	strh	r2, [sp, #2]	@ movhi
	strh	r3, [sp, #4]	@ movhi
	str	r3, [sp, #8]
	str	r4, [sp, #12]
	ldr	r3, [r0, #1980]
	blx	r3
	mov	r0, r4
	bl	get_block_used_count_from_oob
	add	sp, sp, #32
	@ sp needed
	pop	{r4, pc}
	.size	get_used_block_count, .-get_used_block_count
	.section	.text.get_logic_page_from_oob,"ax",%progbits
	.align	1
	.global	get_logic_page_from_oob
	.syntax unified
	.thumb
	.thumb_func
	.fpu softvfp
	.type	get_logic_page_from_oob, %function
get_logic_page_from_oob:
	@ args = 0, pretend = 0, frame = 0
	@ frame_needed = 0, uses_anonymous_args = 0
	push	{r3, lr}
	movs	r2, #4
	movs	r1, #1
	bl	get_spare_data
	bic	r0, r0, #-268435456
	pop	{r3, pc}
	.size	get_logic_page_from_oob, .-get_logic_page_from_oob
	.section	.text._nftl_nand_read_logic_history_page,"ax",%progbits
	.align	1
	.global	_nftl_nand_read_logic_history_page
	.syntax unified
	.thumb
	.thumb_func
	.fpu softvfp
	.type	_nftl_nand_read_logic_history_page, %function
_nftl_nand_read_logic_history_page:
	@ args = 0, pretend = 0, frame = 56
	@ frame_needed = 0, uses_anonymous_args = 0
	push	{r4, r5, r6, r7, r8, r10, fp, lr}
	sub	sp, sp, #56
	movs	r2, #16
	mov	r5, r0
	add	r0, sp, #24
	str	r1, [sp, #16]
	movs	r1, #0
	bl	memset
	ldr	r3, [r5, #4]
	ldrb	r6, [r3, #8]	@ zero_extendqisi2
	lsls	r6, r6, #9
	mov	r0, r6
	bl	nftl_malloc
	mov	r8, r0
	mov	r0, r6
	bl	nftl_malloc
	mov	fp, r0
	cmp	r8, #0
	bne	.L516
.L533:
	ldr	r1, .L534
	ldr	r0, .L534+4
	bl	NAND_Print
	movs	r0, #1
.L515:
	add	sp, sp, #56
	@ sp needed
	pop	{r4, r5, r6, r7, r8, r10, fp, pc}
.L516:
	cbnz	r0, .L518
	mov	r0, r8
	bl	nftl_free
	b	.L533
.L518:
	ldrh	r2, [r5, #18]
	movs	r1, #0
	ldrh	r3, [r5, #20]
	mov	r0, r8
	mov	r10, #0
	mov	r7, r10
	adds	r3, r2, r3
	mov	r2, r6
	str	r3, [sp, #8]
	bl	NAND_Memset
	mov	r2, r6
	movs	r1, #0
	mov	r0, fp
	bl	NAND_Memset
	movs	r2, #16
	movs	r1, #0
	add	r0, sp, #24
	bl	NAND_Memset
	lsrs	r3, r6, #9
	str	r3, [sp, #20]
.L519:
	ldr	r3, [sp, #8]
	cmp	r3, r7
	bne	.L527
	mov	r1, r3
	ldr	r0, .L534+8
	bl	NAND_Print
	mov	r0, r8
	bl	nftl_free
	mov	r0, fp
	bl	nftl_free
	movs	r0, #0
	b	.L515
.L527:
	mov	r1, r7
	ldr	r0, .L534+12
	bl	NAND_Print
	uxth	r3, r7
	movs	r4, #0
	strh	r4, [sp, #40]	@ movhi
	add	r1, sp, #40
	str	r3, [sp, #4]
	mov	r0, r5
	ldrh	r3, [sp, #4]
	strh	r4, [sp, #44]	@ movhi
	str	r4, [sp, #48]
	strh	r3, [sp, #42]	@ movhi
	add	r3, sp, #24
	str	r3, [sp, #52]
	ldr	r3, [r5, #1992]
	blx	r3
	cmp	r0, #1
	beq	.L520
	mov	r1, r7
	ldr	r0, .L534+16
	bl	NAND_Print
.L521:
	adds	r7, r7, #1
	b	.L519
.L526:
	movs	r2, #16
	movs	r1, #0
	add	r0, sp, #24
	bl	NAND_Memset
	uxth	r3, r4
	add	r1, sp, #40
	mov	r0, r5
	str	r3, [sp, #12]
	ldrh	r3, [sp, #12]
	strh	r3, [sp, #40]	@ movhi
	ldrh	r3, [sp, #4]
	strh	r3, [sp, #42]	@ movhi
	movs	r3, #0
	strh	r3, [sp, #44]	@ movhi
	str	r3, [sp, #48]
	add	r3, sp, #24
	str	r3, [sp, #52]
	ldr	r3, [r5, #1980]
	blx	r3
	adds	r0, r0, #2
	bne	.L522
	ldr	r0, .L534+20
	mov	r2, r4
	mov	r1, r7
.L532:
	bl	NAND_Print
.L523:
	adds	r4, r4, #1
.L520:
	ldr	r3, [r5, #4]
	ldrh	r3, [r3, #20]
	cmp	r3, r4
	bgt	.L526
	b	.L521
.L522:
	add	r0, sp, #24
	bl	get_logic_page_from_oob
	ldr	r3, [sp, #16]
	cmp	r0, r3
	bne	.L523
	ldrh	r3, [sp, #12]
	add	r1, sp, #40
	str	r8, [sp, #48]
	mov	r0, r5
	strh	r3, [sp, #40]	@ movhi
	ldrh	r3, [sp, #4]
	strh	r3, [sp, #42]	@ movhi
	ldrh	r3, [sp, #20]
	strh	r3, [sp, #44]	@ movhi
	add	r3, sp, #24
	str	r3, [sp, #52]
	ldr	r3, [r5, #1980]
	blx	r3
	adds	r0, r0, #2
	bne	.L524
	mov	r2, r4
	mov	r1, r7
	ldr	r0, .L534+24
	b	.L532
.L524:
	add	r3, r10, #1
	str	r3, [sp, #12]
	cmp	r10, #0
	beq	.L525
	mov	r2, r10
	ldr	r1, [sp, #16]
	ldr	r0, .L534+28
	bl	NAND_Print
	mov	r1, r6
	mov	r0, fp
	bl	dumphex
	ldr	r0, .L534+32
	bl	NAND_Print
	movs	r1, #16
	add	r0, sp, #24
	bl	dumphex
.L525:
	mov	r2, r6
	mov	r1, r8
	mov	r0, fp
	bl	NAND_Memcpy
	ldr	r10, [sp, #12]
	b	.L523
.L535:
	.align	2
.L534:
	.word	.LANCHOR8
	.word	.LC54
	.word	.LC61
	.word	.LC55
	.word	.LC56
	.word	.LC57
	.word	.LC58
	.word	.LC59
	.word	.LC60
	.size	_nftl_nand_read_logic_history_page, .-_nftl_nand_read_logic_history_page
	.section	.text._nand_read_history,"ax",%progbits
	.align	1
	.global	_nand_read_history
	.syntax unified
	.thumb
	.thumb_func
	.fpu softvfp
	.type	_nand_read_history, %function
_nand_read_history:
	@ args = 0, pretend = 0, frame = 0
	@ frame_needed = 0, uses_anonymous_args = 0
	push	{r4, r5, r6, lr}
	mov	r6, r3
	ldr	r4, [r0, #12]
	add	r2, r2, r1
	mov	r5, r1
	ldr	r3, [r4, #24]
	cmp	r2, r3
	bhi	.L540
	ldr	r3, [r4, #4]
	mov	r0, r5
	ldrb	r1, [r3, #8]	@ zero_extendqisi2
	bl	__aeabi_uidiv
	mov	r2, r6
	mov	r1, r0
	mov	r0, r4
	pop	{r4, r5, r6, lr}
	b	_nftl_nand_read_logic_history_page
.L540:
	ldr	r0, .L541
	bl	NAND_Print
	movs	r0, #1
	pop	{r4, r5, r6, pc}
.L542:
	.align	2
.L541:
	.word	.LC62
	.size	_nand_read_history, .-_nand_read_history
	.section	.text.get_special_data_from_oob,"ax",%progbits
	.align	1
	.global	get_special_data_from_oob
	.syntax unified
	.thumb
	.thumb_func
	.fpu softvfp
	.type	get_special_data_from_oob, %function
get_special_data_from_oob:
	@ args = 0, pretend = 0, frame = 0
	@ frame_needed = 0, uses_anonymous_args = 0
	@ link register save eliminated.
	movs	r2, #4
	movs	r1, #1
	b	get_spare_data
	.size	get_special_data_from_oob, .-get_special_data_from_oob
	.section	.text._nftl_nand_copy_page,"ax",%progbits
	.align	1
	.global	_nftl_nand_copy_page
	.syntax unified
	.thumb
	.thumb_func
	.fpu softvfp
	.type	_nftl_nand_copy_page, %function
_nftl_nand_copy_page:
	@ args = 4, pretend = 0, frame = 48
	@ frame_needed = 0, uses_anonymous_args = 0
	push	{r4, r5, r6, r7, r8, lr}
	sub	sp, sp, #56
	mov	r8, r2
	ldrh	r2, [sp, #80]
	mov	r6, r1
	add	r5, sp, #8
	ldrh	r1, [r1]
	mov	r4, r0
	strh	r2, [sp, #24]	@ movhi
	strh	r2, [sp, #40]	@ movhi
	ldr	r2, [r0, #4]
	strh	r1, [sp, #26]	@ movhi
	ldrh	r1, [r8]
	ldrh	r2, [r2, #28]
	str	r3, [sp, #32]
	strh	r1, [sp, #42]	@ movhi
	add	r1, sp, #24
	strh	r2, [sp, #28]	@ movhi
	strh	r2, [sp, #44]	@ movhi
	str	r3, [sp, #48]
	str	r5, [sp, #36]
	ldr	r3, [r0, #1980]
	str	r5, [sp, #52]
	blx	r3
	cmp	r0, #10
	ite	ne
	movne	r7, r0
	moveq	r7, #0
	mov	r0, r5
	bl	get_special_data_from_oob
	ldrh	r3, [r8, #6]
	mov	r2, r0
	mov	r1, r5
	mov	r0, r4
	adds	r3, r3, #1
	uxth	r3, r3
	str	r3, [sp]
	ldr	r3, [r6, #8]
	bl	set_oob_special_page
	ldr	r3, [r4, #1984]
	add	r1, sp, #40
	mov	r0, r4
	blx	r3
	orr	r0, r7, r0, lsl #8
	add	sp, sp, #56
	@ sp needed
	pop	{r4, r5, r6, r7, r8, pc}
	.size	_nftl_nand_copy_page, .-_nftl_nand_copy_page
	.section	.text.get_erase_count_from_oob,"ax",%progbits
	.align	1
	.global	get_erase_count_from_oob
	.syntax unified
	.thumb
	.thumb_func
	.fpu softvfp
	.type	get_erase_count_from_oob, %function
get_erase_count_from_oob:
	@ args = 0, pretend = 0, frame = 0
	@ frame_needed = 0, uses_anonymous_args = 0
	push	{r3, lr}
	movs	r2, #2
	movs	r1, #5
	bl	get_spare_data
	uxth	r0, r0
	pop	{r3, pc}
	.size	get_erase_count_from_oob, .-get_erase_count_from_oob
	.section	.text.is_ftl_start_flag_page,"ax",%progbits
	.align	1
	.global	is_ftl_start_flag_page
	.syntax unified
	.thumb
	.thumb_func
	.fpu softvfp
	.type	is_ftl_start_flag_page, %function
is_ftl_start_flag_page:
	@ args = 0, pretend = 0, frame = 0
	@ frame_needed = 0, uses_anonymous_args = 0
	@ link register save eliminated.
	ldrb	r3, [r0, #1]	@ zero_extendqisi2
	cmp	r3, #170
	bne	.L549
	ldrb	r0, [r0, #2]	@ zero_extendqisi2
	sub	r3, r0, #238
	rsbs	r0, r3, #0
	adcs	r0, r0, r3
	bx	lr
.L549:
	movs	r0, #0
	bx	lr
	.size	is_ftl_start_flag_page, .-is_ftl_start_flag_page
	.section	.text.is_no_use_device,"ax",%progbits
	.align	1
	.global	is_no_use_device
	.syntax unified
	.thumb
	.thumb_func
	.fpu softvfp
	.type	is_no_use_device, %function
is_no_use_device:
	@ args = 0, pretend = 0, frame = 32
	@ frame_needed = 0, uses_anonymous_args = 0
	ldr	r3, [r0, #24]
	push	{r4, r5, r6, r7, r8, lr}
	mov	r4, r0
	sub	sp, sp, #32
	mov	r6, r1
	ldr	r5, [r0, #40]
	cmp	r3, #0
	beq	.L560
	ldr	r7, [r0, #4]
	ldr	r0, [r0, #32]
	ldrb	r1, [r7, #8]	@ zero_extendqisi2
	add	r0, r0, r3
	bl	__aeabi_uidiv
	ldrh	r1, [r7, #20]
	bl	__aeabi_uidiv
	mov	r7, r0
.L551:
	movs	r3, #0
	mov	r8, r3
	strh	r3, [r4, #18]	@ movhi
.L552:
	ldrh	r3, [r4, #18]
	cmp	r3, r7
	bcs	.L567
	ldr	r3, [r4, #4]
	mov	r0, r4
	ldrh	r1, [r5]
	str	r5, [r4, #44]
	ldrh	r2, [r3, #28]
	ldr	r3, [r4, #284]
	strh	r1, [sp, #18]	@ movhi
	add	r1, sp, #16
	strh	r8, [sp, #16]	@ movhi
	str	r3, [sp, #24]
	strh	r2, [sp, #20]	@ movhi
	str	sp, [sp, #28]
	ldr	r3, [r4, #1992]
	blx	r3
	cmp	r0, #1
	bne	.L553
	ldr	r3, [r4, #1980]
	add	r1, sp, #16
	mov	r0, r4
	blx	r3
	mov	r2, r0
	mov	r0, sp
	bl	is_ftl_start_flag_page
	cmp	r0, #1
	beq	.L554
.L567:
	movs	r0, #1
.L550:
	add	sp, sp, #32
	@ sp needed
	pop	{r4, r5, r6, r7, r8, pc}
.L560:
	mov	r7, #-1
	b	.L551
.L554:
	cbz	r2, .L556
	cmp	r2, #10
	bne	.L567
.L556:
	movs	r2, #4
	movs	r1, #3
	mov	r0, sp
	bl	get_spare_data
	cmp	r0, r6
	beq	.L558
	ldr	r0, .L568
	bl	NAND_Print
	b	.L567
.L558:
	adds	r5, r5, #36
	movs	r0, #0
	str	r5, [r4, #40]
	b	.L550
.L553:
	movs	r3, #255
	mov	r1, r4
	strh	r3, [r5, #2]	@ movhi
	mov	r0, r5
	ldrh	r3, [r4, #18]
	adds	r3, r3, #1
	strh	r3, [r4, #18]	@ movhi
	bl	is_last_phy_block
	cmp	r0, #1
	beq	.L567
	adds	r5, r5, #36
	b	.L552
.L569:
	.align	2
.L568:
	.word	.LC63
	.size	is_no_use_device, .-is_no_use_device
	.section	.text.is_ftl_end_flag_page,"ax",%progbits
	.align	1
	.global	is_ftl_end_flag_page
	.syntax unified
	.thumb
	.thumb_func
	.fpu softvfp
	.type	is_ftl_end_flag_page, %function
is_ftl_end_flag_page:
	@ args = 0, pretend = 0, frame = 0
	@ frame_needed = 0, uses_anonymous_args = 0
	@ link register save eliminated.
	ldrb	r3, [r0, #1]	@ zero_extendqisi2
	cmp	r3, #170
	bne	.L572
	ldrb	r0, [r0, #2]	@ zero_extendqisi2
	sub	r3, r0, #204
	rsbs	r0, r3, #0
	adcs	r0, r0, r3
	bx	lr
.L572:
	movs	r0, #0
	bx	lr
	.size	is_ftl_end_flag_page, .-is_ftl_end_flag_page
	.section	.text.is_ftl_special_data,"ax",%progbits
	.align	1
	.global	is_ftl_special_data
	.syntax unified
	.thumb
	.thumb_func
	.fpu softvfp
	.type	is_ftl_special_data, %function
is_ftl_special_data:
	@ args = 0, pretend = 0, frame = 0
	@ frame_needed = 0, uses_anonymous_args = 0
	@ link register save eliminated.
	ldrb	r0, [r0, #1]	@ zero_extendqisi2
	sub	r3, r0, #170
	rsbs	r0, r3, #0
	adcs	r0, r0, r3
	bx	lr
	.size	is_ftl_special_data, .-is_ftl_special_data
	.section	.text.is_ftl_logic_page_data,"ax",%progbits
	.align	1
	.global	is_ftl_logic_page_data
	.syntax unified
	.thumb
	.thumb_func
	.fpu softvfp
	.type	is_ftl_logic_page_data, %function
is_ftl_logic_page_data:
	@ args = 0, pretend = 0, frame = 0
	@ frame_needed = 0, uses_anonymous_args = 0
	@ link register save eliminated.
	ldrb	r0, [r0, #1]	@ zero_extendqisi2
	and	r0, r0, #240
	sub	r3, r0, #192
	rsbs	r0, r3, #0
	adcs	r0, r0, r3
	bx	lr
	.size	is_ftl_logic_page_data, .-is_ftl_logic_page_data
	.section	.text.is_nouse_page,"ax",%progbits
	.align	1
	.global	is_nouse_page
	.syntax unified
	.thumb
	.thumb_func
	.fpu softvfp
	.type	is_nouse_page, %function
is_nouse_page:
	@ args = 0, pretend = 0, frame = 0
	@ frame_needed = 0, uses_anonymous_args = 0
	subs	r2, r0, #1
	add	r1, r0, #9
	movs	r3, #0
	push	{r4, lr}
.L577:
	ldrb	r4, [r2, #1]!	@ zero_extendqisi2
	cmp	r4, #255
	it	eq
	addeq	r3, r3, #1
	cmp	r2, r1
	bne	.L577
	cmp	r3, #8
	bls	.L579
	ldrb	r0, [r0]	@ zero_extendqisi2
	sub	r3, r0, #255
	rsbs	r0, r3, #0
	adcs	r0, r0, r3
.L575:
	pop	{r4, pc}
.L579:
	movs	r0, #0
	b	.L575
	.size	is_nouse_page, .-is_nouse_page
	.section	.text.block_last_page_ecc_error,"ax",%progbits
	.align	1
	.global	block_last_page_ecc_error
	.syntax unified
	.thumb
	.thumb_func
	.fpu softvfp
	.type	block_last_page_ecc_error, %function
block_last_page_ecc_error:
	@ args = 0, pretend = 0, frame = 32
	@ frame_needed = 0, uses_anonymous_args = 0
	push	{r4, r5, r6, r7, r8, r10, fp, lr}
	mov	r10, r0
	sub	sp, sp, #32
	ldr	r0, .L613
	mov	r4, r1
	bl	NAND_Print
	mov	r0, #4096
	bl	NAND_Malloc
	mov	r5, r0
	cbnz	r0, .L582
	ldr	r1, .L613+4
	ldr	r0, .L613+8
	bl	NAND_Print
.L582:
	ldr	r7, .L613+4
	movs	r6, #0
	ldr	r8, .L613+28
.L583:
	ldr	r2, [r10, #4]
	ldrh	r1, [r2, #20]
	cmp	r1, r6
	bgt	.L585
	movs	r6, #0
	mov	r7, r6
.L586:
	ldr	r2, [r10, #4]
	uxth	r8, r7
	ldrh	ip, [r4]
	ldr	r1, [r10, #284]
	ldrh	r0, [r2, #28]
	ldrh	r2, [r2, #20]
	cmp	r2, r7
	ble	.L598
	strh	ip, [sp, #18]	@ movhi
	strh	r0, [sp, #20]	@ movhi
	mov	r0, r10
	str	r1, [sp, #24]
	add	r1, sp, #16
	ldr	r2, [r10, #1980]
	strh	r8, [sp, #16]	@ movhi
	str	sp, [sp, #28]
	blx	r2
	mov	fp, r0
	mov	r0, sp
	bl	is_nouse_page
	cmp	r0, #1
	bne	.L587
.L598:
	ldr	r2, [r10, #4]
	ldrh	r0, [r4]
	strh	r8, [sp, #16]	@ movhi
	ldrh	r1, [r2, #28]
	ldr	r2, [r10, #284]
	strh	r0, [sp, #18]	@ movhi
	mov	r0, r10
	strh	r1, [sp, #20]	@ movhi
	add	r1, sp, #16
	str	r2, [sp, #24]
	str	sp, [sp, #28]
	ldr	r2, [r10, #1976]
	blx	r2
	cmp	r0, #0
	bne	.L588
.L589:
	movs	r7, #0
.L601:
	cmp	r7, r6
	ldr	r2, [r10, #4]
	beq	.L600
	ldrh	r0, [r4]
	ldrh	r1, [r2, #28]
	ldr	r2, [r5, r7, lsl #2]
	strh	r7, [sp, #16]	@ movhi
	adds	r7, r7, #1
	strh	r0, [sp, #18]	@ movhi
	mov	r0, r10
	strh	r1, [sp, #20]	@ movhi
	add	r1, sp, #16
	str	r2, [sp, #24]
	str	sp, [sp, #28]
	ldr	r2, [r10, #1984]
	blx	r2
	b	.L601
.L585:
	ldrh	r0, [r2, #26]
	bl	nftl_malloc
	str	r0, [r5, r6, lsl #2]
	cbnz	r0, .L584
	mov	r1, r7
	mov	r0, r8
	bl	NAND_Print
.L584:
	adds	r6, r6, #1
	b	.L583
.L587:
	mov	r0, sp
	bl	is_ftl_logic_page_data
	cmp	r0, #1
	bne	.L590
	cmp	fp, #0
	beq	.L591
	cmp	fp, #10
	bne	.L592
.L591:
	ldr	r2, [r10, #4]
	adds	r6, r6, #1
	ldr	r1, [r10, #284]
	ldr	r0, [r5, r7, lsl #2]
	ldrh	r2, [r2, #26]
	bl	NAND_Memcpy
.L592:
	adds	r7, r7, #1
	b	.L586
.L590:
	ldrb	r1, [sp, #1]	@ zero_extendqisi2
	cmp	r1, #170
	bne	.L593
	cmp	fp, #0
	bne	.L611
.L594:
	ldr	r2, [r10, #4]
	adds	r6, r6, #1
	ldr	r1, [r10, #284]
	ldr	r0, [r5, r7, lsl #2]
	ldrh	r2, [r2, #26]
	bl	NAND_Memcpy
	b	.L596
.L593:
	ldr	r0, .L613+12
.L612:
	bl	NAND_Print
	b	.L592
.L611:
	cmp	fp, #10
	beq	.L594
.L596:
	ldr	r0, .L613+16
	b	.L612
.L588:
	ldr	r2, [r10, #1996]
	add	r1, sp, #16
	mov	r0, r10
	blx	r2
	mov	r0, r10
	bl	out_phy_block_from_free_list
	mov	r4, r0
	cbnz	r0, .L599
	ldr	r0, .L613+20
	bl	NAND_Print
.L600:
	movs	r6, #0
.L603:
	ldr	r2, [r10, #4]
	ldrh	r2, [r2, #20]
	cmp	r2, r6
	bgt	.L604
	mov	r0, r5
	bl	nftl_free
	ldr	r0, .L613+24
	bl	NAND_Print
	mov	r0, r4
	add	sp, sp, #32
	@ sp needed
	pop	{r4, r5, r6, r7, r8, r10, fp, pc}
.L599:
	ldr	r2, [r10, #4]
	ldrh	r0, [r0]
	strh	r8, [sp, #16]	@ movhi
	ldrh	r1, [r2, #28]
	ldr	r2, [r10, #284]
	strh	r0, [sp, #18]	@ movhi
	mov	r0, r10
	strh	r1, [sp, #20]	@ movhi
	add	r1, sp, #16
	str	r2, [sp, #24]
	str	sp, [sp, #28]
	ldr	r2, [r10, #1976]
	blx	r2
	b	.L589
.L604:
	ldr	r0, [r5, r6, lsl #2]
	adds	r6, r6, #1
	bl	nftl_free
	b	.L603
.L614:
	.align	2
.L613:
	.word	.LC64
	.word	.LANCHOR9
	.word	.LC65
	.word	.LC67
	.word	.LC68
	.word	.LC69
	.word	.LC70
	.word	.LC66
	.size	block_last_page_ecc_error, .-block_last_page_ecc_error
	.section	.text.is_fill_page,"ax",%progbits
	.align	1
	.global	is_fill_page
	.syntax unified
	.thumb
	.thumb_func
	.fpu softvfp
	.type	is_fill_page, %function
is_fill_page:
	@ args = 0, pretend = 0, frame = 0
	@ frame_needed = 0, uses_anonymous_args = 0
	push	{r3, lr}
	bl	get_special_data_from_oob
	sub	r3, r0, #1431655765
	rsbs	r0, r3, #0
	adcs	r0, r0, r3
	pop	{r3, pc}
	.size	is_fill_page, .-is_fill_page
	.section	.text.is_phy_mapping_page,"ax",%progbits
	.align	1
	.global	is_phy_mapping_page
	.syntax unified
	.thumb
	.thumb_func
	.fpu softvfp
	.type	is_phy_mapping_page, %function
is_phy_mapping_page:
	@ args = 0, pretend = 0, frame = 0
	@ frame_needed = 0, uses_anonymous_args = 0
	push	{r3, lr}
	bl	get_special_data_from_oob
	ldr	r3, .L617
	subs	r3, r0, r3
	rsbs	r0, r3, #0
	adcs	r0, r0, r3
	pop	{r3, pc}
.L618:
	.align	2
.L617:
	.word	-1431633921
	.size	is_phy_mapping_page, .-is_phy_mapping_page
	.section	.text.is_function_info_page,"ax",%progbits
	.align	1
	.global	is_function_info_page
	.syntax unified
	.thumb
	.thumb_func
	.fpu softvfp
	.type	is_function_info_page, %function
is_function_info_page:
	@ args = 0, pretend = 0, frame = 0
	@ frame_needed = 0, uses_anonymous_args = 0
	push	{r3, lr}
	bl	get_special_data_from_oob
	lsrs	r0, r0, #16
	ldr	r3, .L620
	lsls	r0, r0, #16
	subs	r3, r0, r3
	rsbs	r0, r3, #0
	adcs	r0, r0, r3
	pop	{r3, pc}
.L621:
	.align	2
.L620:
	.word	-1441726464
	.size	is_function_info_page, .-is_function_info_page
	.section	.text.is_power_down_info_page,"ax",%progbits
	.align	1
	.global	is_power_down_info_page
	.syntax unified
	.thumb
	.thumb_func
	.fpu softvfp
	.type	is_power_down_info_page, %function
is_power_down_info_page:
	@ args = 0, pretend = 0, frame = 0
	@ frame_needed = 0, uses_anonymous_args = 0
	push	{r3, lr}
	bl	get_special_data_from_oob
	ldr	r3, .L623
	subs	r3, r0, r3
	rsbs	r0, r3, #0
	adcs	r0, r0, r3
	pop	{r3, pc}
.L624:
	.align	2
.L623:
	.word	-1441713357
	.size	is_power_down_info_page, .-is_power_down_info_page
	.section	.text.get_logic_page_map,"ax",%progbits
	.align	1
	.global	get_logic_page_map
	.syntax unified
	.thumb
	.thumb_func
	.fpu softvfp
	.type	get_logic_page_map, %function
get_logic_page_map:
	@ args = 0, pretend = 0, frame = 0
	@ frame_needed = 0, uses_anonymous_args = 0
	push	{r3, lr}
	mov	r2, r1
	ldr	r3, [r0, #28]
	cmp	r3, r1
	bhi	.L626
	ldr	r1, .L628
	ldr	r0, .L628+4
	bl	NAND_Print
	movs	r0, #0
.L625:
	pop	{r3, pc}
.L626:
	lsrs	r1, r1, #18
	add	r0, r0, r1, lsl #2
	rsb	r1, r1, r1, lsl #14
	ldr	r0, [r0, #52]
	add	r2, r2, r1, lsl #18
	add	r0, r0, r2, lsl #2
	b	.L625
.L629:
	.align	2
.L628:
	.word	.LANCHOR10
	.word	.LC71
	.size	get_logic_page_map, .-get_logic_page_map
	.section	.text.recover_logic_page_mapping,"ax",%progbits
	.align	1
	.global	recover_logic_page_mapping
	.syntax unified
	.thumb
	.thumb_func
	.fpu softvfp
	.type	recover_logic_page_mapping, %function
recover_logic_page_mapping:
	@ args = 0, pretend = 0, frame = 8
	@ frame_needed = 0, uses_anonymous_args = 0
	push	{r0, r1, r4, r5, r6, r7, r8, r10, fp, lr}
	mov	r6, r0
	mov	r4, r1
	mov	fp, r2
	mov	r10, r3
	movs	r5, #0
.L631:
	cmp	r5, r10
	bne	.L641
	movs	r0, #0
	add	sp, sp, #8
	@ sp needed
	pop	{r4, r5, r6, r7, r8, r10, fp, pc}
.L641:
	ldr	r3, [fp, r5, lsl #2]
	ldr	r2, [r6, #28]
	cmp	r2, r3
	bls	.L632
	mov	r1, r3
	mov	r0, r6
	bl	get_logic_page_map
	mov	r7, r0
	cbnz	r0, .L633
	ldr	r0, .L645
	bl	NAND_Print
	ldrh	r3, [r4, #4]
	adds	r3, r3, #1
	strh	r3, [r4, #4]	@ movhi
.L634:
	adds	r5, r5, #1
	b	.L631
.L633:
	ldrh	r3, [r0, #2]
	movw	r2, #65535
	cmp	r3, r2
	beq	.L636
	ldr	r2, [r6, #4]
	mov	r8, #36
	mul	r8, r8, r3
	ldr	r2, [r2, #32]
	add	r3, r2, r8
	cmp	r4, r3
	beq	.L637
	mov	r1, r3
	mov	r0, r6
	str	r2, [sp, #4]
	str	r3, [sp]
	bl	phy_block_from_invalid_page_incr
	ldrd	r3, r2, [sp]
.L638:
	ldr	r0, [r6, #4]
	ldrh	r1, [r3, #4]
	ldrh	r3, [r0, #20]
	cmp	r1, r3
	bls	.L636
	ldrh	r1, [r2, r8]
	ldr	r0, .L645+4
	bl	NAND_Print
.L636:
	ldrh	r3, [r7]
	bfi	r3, r5, #0, #12
	strh	r3, [r7]	@ movhi
	ldrh	r3, [r4]
	strh	r3, [r7, #2]	@ movhi
	b	.L634
.L637:
	ldrh	r1, [r4, #4]
	adds	r1, r1, #1
	strh	r1, [r4, #4]	@ movhi
	b	.L638
.L632:
	adds	r2, r3, #1
	beq	.L640
	mov	r2, r5
	ldrh	r1, [r4]
	ldr	r0, .L645+8
	bl	NAND_Print
.L640:
	ldrh	r3, [r4, #4]
	ldr	r2, [r6, #4]
	adds	r3, r3, #1
	ldrh	r2, [r2, #20]
	uxth	r3, r3
	strh	r3, [r4, #4]	@ movhi
	cmp	r2, r3
	bcs	.L634
	ldr	r0, .L645+12
	bl	NAND_Print
	b	.L634
.L646:
	.align	2
.L645:
	.word	.LC72
	.word	.LC73
	.word	.LC74
	.word	.LC75
	.size	recover_logic_page_mapping, .-recover_logic_page_mapping
	.section	.text.malloc_logic_page_map,"ax",%progbits
	.align	1
	.global	malloc_logic_page_map
	.syntax unified
	.thumb
	.thumb_func
	.fpu softvfp
	.type	malloc_logic_page_map, %function
malloc_logic_page_map:
	@ args = 0, pretend = 0, frame = 0
	@ frame_needed = 0, uses_anonymous_args = 0
	push	{r4, r5, r6, r7, r8, lr}
	add	r7, r0, #52
	mov	r4, r1
	mov	r6, r0
	add	r2, r0, #152
	mov	r3, r7
	movs	r1, #0
.L648:
	str	r1, [r3], #4
	cmp	r3, r2
	bne	.L648
	ldr	r8, .L660+12
	movs	r5, #0
.L652:
	cmp	r4, r8
	bls	.L649
	mov	r0, #1048576
	bl	NAND_Malloc
	str	r0, [r7], #4
	cbnz	r0, .L650
	ldr	r0, .L660
	mov	r1, r5
.L659:
	bl	NAND_Print
	mvn	r0, #1
.L647:
	pop	{r4, r5, r6, r7, r8, pc}
.L650:
	mov	r2, #1048576
	movs	r1, #255
	adds	r5, r5, #1
	sub	r4, r4, #262144
	bl	NAND_Memset
	cmp	r5, #25
	bne	.L652
	cbz	r4, .L655
	mov	r1, r4
	ldr	r0, .L660+4
	b	.L659
.L649:
	lsls	r4, r4, #2
	mov	r0, r4
	bl	nftl_malloc
	add	r3, r6, r5, lsl #2
	str	r0, [r3, #52]
	cbnz	r0, .L654
	mov	r1, r5
	ldr	r0, .L660+8
	b	.L659
.L654:
	mov	r2, r4
	movs	r1, #255
	bl	NAND_Memset
	movs	r0, #0
	b	.L647
.L655:
	mov	r0, r4
	b	.L647
.L661:
	.align	2
.L660:
	.word	.LC76
	.word	.LC78
	.word	.LC77
	.word	262143
	.size	malloc_logic_page_map, .-malloc_logic_page_map
	.section	.text.zone_param_init,"ax",%progbits
	.align	1
	.global	zone_param_init
	.syntax unified
	.thumb
	.thumb_func
	.fpu softvfp
	.type	zone_param_init, %function
zone_param_init:
	@ args = 0, pretend = 0, frame = 0
	@ frame_needed = 0, uses_anonymous_args = 0
	push	{r3, r4, r5, r6, r7, lr}
	mov	r7, r3
	ldr	r3, [r0, #4]
	mov	r4, r0
	mov	r5, r1
	mov	r6, r2
	ldrh	r0, [r3, #26]
	bl	nftl_malloc
	str	r0, [r4, #284]
	cbnz	r0, .L663
	ldr	r1, .L674
	ldr	r0, .L674+4
.L672:
	bl	NAND_Print
	b	.L673
.L663:
	ldr	r3, [r4, #4]
	ldrh	r0, [r3, #26]
	bl	nftl_malloc
	str	r0, [r4, #288]
	cbnz	r0, .L665
	ldr	r1, .L674
	ldr	r0, .L674+8
	b	.L672
.L665:
	ldr	r2, [r4, #4]
	movs	r1, #36
	strh	r5, [r4, #36]	@ movhi
	mov	r0, r6
	str	r6, [r4, #24]
	ldr	r3, [r2, #32]
	str	r7, [r4, #32]
	mla	r5, r1, r5, r3
	movs	r3, #0
	strh	r3, [r4, #18]	@ movhi
	str	r5, [r4, #40]
	strh	r3, [r4, #20]	@ movhi
	strh	r3, [r4, #22]	@ movhi
	str	r3, [r4, #44]
	ldrb	r1, [r2, #8]	@ zero_extendqisi2
	bl	__aeabi_uidiv
	mov	r1, r0
	mov	r0, r4
	bl	malloc_logic_page_map
	mov	r5, r0
	cbz	r0, .L666
	ldr	r0, .L674+12
	bl	NAND_Print
.L673:
	mvn	r5, #1
.L662:
	mov	r0, r5
	pop	{r3, r4, r5, r6, r7, pc}
.L666:
	ldr	r3, [r4, #4]
	ldrh	r0, [r3, #20]
	mov	r3, #320
	str	r3, [r4, #316]
	lsls	r0, r0, #2
	str	r0, [r4, #308]
	add	r0, r0, r3
	str	r0, [r4, #300]
	bl	nftl_malloc
	mov	r3, r0
	str	r0, [r4, #296]
	cbnz	r0, .L667
	ldr	r1, .L674
	ldr	r0, .L674+16
	b	.L672
.L667:
	ldr	r2, [r4, #308]
	movs	r1, #255
	str	r0, [r4, #304]
	add	r3, r3, r2
	ldr	r2, [r4, #300]
	str	r3, [r4, #312]
	str	r3, [r4, #1968]
	bl	NAND_Memset
	ldr	r3, [r4, #308]
	ldr	r0, [r4, #300]
	str	r3, [r4, #340]
	str	r0, [r4, #332]
	bl	nftl_malloc
	mov	r3, r0
	str	r0, [r4, #328]
	cbnz	r0, .L668
	ldr	r1, .L674
	ldr	r0, .L674+20
	b	.L672
.L668:
	ldr	r2, [r4, #340]
	movs	r1, #255
	str	r0, [r4, #336]
	add	r3, r3, r2
	ldr	r2, [r4, #332]
	str	r3, [r4, #344]
	bl	NAND_Memset
	ldr	r3, [r4, #308]
	ldr	r0, [r4, #300]
	str	r3, [r4, #272]
	ldr	r3, [r4, #316]
	str	r0, [r4, #264]
	str	r3, [r4, #280]
	bl	nftl_malloc
	str	r0, [r4, #260]
	cbnz	r0, .L669
	ldr	r1, .L674
	ldr	r0, .L674+24
	b	.L672
.L669:
	ldr	r3, [r4, #272]
	mov	r1, r5
	str	r0, [r4, #268]
	str	r5, [r4, #356]
	add	r0, r0, r3
	ldr	r3, [r4, #4]
	str	r0, [r4, #276]
	str	r5, [r4, #376]
	ldrh	r2, [r3, #16]
	strd	r5, r5, [r4, #360]
	strd	r5, r5, [r4, #368]
	strd	r5, r5, [r4, #576]
	lsrs	r3, r2, #1
	str	r5, [r4, #292]
	str	r5, [r4, #324]
	strh	r3, [r4, #572]	@ movhi
	ldr	r3, .L674+28
	strd	r5, r5, [r4, #172]
	strd	r5, r5, [r4, #164]
	strd	r5, r5, [r4, #180]
	umull	r2, r3, r2, r3
	mov	r2, r4
	strd	r5, r5, [r4, #208]
	strd	r5, r5, [r4, #200]
	strd	r5, r5, [r4, #216]
	lsrs	r3, r3, #2
	strh	r3, [r4, #574]	@ movhi
	movw	r3, #65535
	strh	r3, [r4, #320]	@ movhi
	strh	r3, [r4, #352]	@ movhi
	mov	r3, r5
	strd	r5, r5, [r4, #244]
	strd	r5, r5, [r4, #236]
	strd	r5, r5, [r4, #252]
	strh	r5, [r4, #392]	@ movhi
.L670:
	strh	r1, [r2, #396]	@ movhi
	adds	r1, r1, #1
	cmp	r1, #10
	strh	r3, [r2, #398]	@ movhi
	strd	r3, r3, [r2, #400]
	add	r2, r2, #16
	str	r3, [r2, #392]
	bne	.L670
	movs	r2, #255
	strh	r3, [r4, #394]	@ movhi
	strd	r2, r3, [r4, #556]
	strd	r3, r3, [r4, #564]
	b	.L662
.L675:
	.align	2
.L674:
	.word	.LANCHOR11
	.word	.LC79
	.word	.LC80
	.word	.LC81
	.word	.LC82
	.word	.LC83
	.word	.LC84
	.word	-1431655765
	.size	zone_param_init, .-zone_param_init
	.section	.text.free_logic_page_map,"ax",%progbits
	.align	1
	.global	free_logic_page_map
	.syntax unified
	.thumb
	.thumb_func
	.fpu softvfp
	.type	free_logic_page_map, %function
free_logic_page_map:
	@ args = 0, pretend = 0, frame = 0
	@ frame_needed = 0, uses_anonymous_args = 0
	push	{r3, r4, r5, lr}
	add	r5, r0, #52
	add	r4, r0, #152
.L678:
	ldr	r0, [r5], #4
	cbz	r0, .L677
	bl	nftl_free
.L677:
	cmp	r5, r4
	bne	.L678
	movs	r0, #0
	pop	{r3, r4, r5, pc}
	.size	free_logic_page_map, .-free_logic_page_map
	.section	.text.zone_param_exit,"ax",%progbits
	.align	1
	.global	zone_param_exit
	.syntax unified
	.thumb
	.thumb_func
	.fpu softvfp
	.type	zone_param_exit, %function
zone_param_exit:
	@ args = 0, pretend = 0, frame = 0
	@ frame_needed = 0, uses_anonymous_args = 0
	push	{r4, lr}
	mov	r4, r0
	bl	free_logic_page_map
	ldr	r0, [r4, #260]
	bl	nftl_free
	ldr	r0, [r4, #296]
	bl	nftl_free
	ldr	r0, [r4, #328]
	bl	nftl_free
	ldr	r0, [r4, #284]
	bl	nftl_free
	ldr	r0, [r4, #288]
	bl	nftl_free
	movs	r0, #0
	pop	{r4, pc}
	.size	zone_param_exit, .-zone_param_exit
	.section	.text.set_physic_op_par,"ax",%progbits
	.align	1
	.global	set_physic_op_par
	.syntax unified
	.thumb
	.thumb_func
	.fpu softvfp
	.type	set_physic_op_par, %function
set_physic_op_par:
	@ args = 8, pretend = 0, frame = 0
	@ frame_needed = 0, uses_anonymous_args = 0
	@ link register save eliminated.
	strh	r3, [r0, #4]	@ movhi
	ldr	r3, [sp]
	strh	r2, [r0]	@ movhi
	strh	r1, [r0, #2]	@ movhi
	str	r3, [r0, #8]
	ldr	r3, [sp, #4]
	str	r3, [r0, #12]
	bx	lr
	.size	set_physic_op_par, .-set_physic_op_par
	.section	.text.nftl_get_boot_cnt,"ax",%progbits
	.align	1
	.global	nftl_get_boot_cnt
	.syntax unified
	.thumb
	.thumb_func
	.fpu softvfp
	.type	nftl_get_boot_cnt, %function
nftl_get_boot_cnt:
	@ args = 0, pretend = 0, frame = 0
	@ frame_needed = 0, uses_anonymous_args = 0
	@ link register save eliminated.
	ldr	r3, [r0, #1968]
	ldr	r0, [r3, #140]
	bx	lr
	.size	nftl_get_boot_cnt, .-nftl_get_boot_cnt
	.section	.text.nftl_get_zone_cap,"ax",%progbits
	.align	1
	.global	nftl_get_zone_cap
	.syntax unified
	.thumb
	.thumb_func
	.fpu softvfp
	.type	nftl_get_zone_cap, %function
nftl_get_zone_cap:
	@ args = 0, pretend = 0, frame = 0
	@ frame_needed = 0, uses_anonymous_args = 0
	@ link register save eliminated.
	ldr	r0, [r0, #24]
	bx	lr
	.size	nftl_get_zone_cap, .-nftl_get_zone_cap
	.section	.text.nftl_get_zone_priv,"ax",%progbits
	.align	1
	.global	nftl_get_zone_priv
	.syntax unified
	.thumb
	.thumb_func
	.fpu softvfp
	.type	nftl_get_zone_priv, %function
nftl_get_zone_priv:
	@ args = 0, pretend = 0, frame = 0
	@ frame_needed = 0, uses_anonymous_args = 0
	@ link register save eliminated.
	ldr	r0, [r0]
	bx	lr
	.size	nftl_get_zone_priv, .-nftl_get_zone_priv
	.section	.text.nftl_get_zone_write_cache_nums,"ax",%progbits
	.align	1
	.global	nftl_get_zone_write_cache_nums
	.syntax unified
	.thumb
	.thumb_func
	.fpu softvfp
	.type	nftl_get_zone_write_cache_nums, %function
nftl_get_zone_write_cache_nums:
	@ args = 0, pretend = 0, frame = 0
	@ frame_needed = 0, uses_anonymous_args = 0
	@ link register save eliminated.
	ldrh	r0, [r0, #588]
	bx	lr
	.size	nftl_get_zone_write_cache_nums, .-nftl_get_zone_write_cache_nums
	.section	.text.print_smart,"ax",%progbits
	.align	1
	.global	print_smart
	.syntax unified
	.thumb
	.thumb_func
	.fpu softvfp
	.type	print_smart, %function
print_smart:
	@ args = 0, pretend = 0, frame = 0
	@ frame_needed = 0, uses_anonymous_args = 0
	ldr	r3, [r0, #356]
	push	{r4, lr}
	cmp	r3, #0
	beq	.L689
	ldr	r4, [r0, #1968]
	ldr	r0, .L691
	ldr	r1, [r4, #112]
	bl	NAND_Print_DBG
	ldrd	r2, [r4, #64]
	ldr	r0, .L691+4
	bl	NAND_Print_DBG
	ldrd	r2, [r4, #72]
	ldr	r0, .L691+8
	bl	NAND_Print_DBG
	ldrd	r2, [r4, #80]
	ldr	r0, .L691+12
	bl	NAND_Print_DBG
	ldrd	r2, [r4, #88]
	ldr	r0, .L691+16
	bl	NAND_Print_DBG
	ldrd	r2, [r4, #96]
	ldr	r0, .L691+20
	bl	NAND_Print_DBG
	ldrd	r2, [r4, #104]
	ldr	r0, .L691+24
	bl	NAND_Print_DBG
	ldr	r1, [r4, #116]
	ldr	r0, .L691+28
	bl	NAND_Print_DBG
	ldr	r1, [r4, #120]
	ldr	r0, .L691+32
	bl	NAND_Print_DBG
	ldr	r1, [r4, #124]
	ldr	r0, .L691+36
	bl	NAND_Print_DBG
	ldr	r1, [r4, #128]
	ldr	r0, .L691+40
	bl	NAND_Print_DBG
	ldr	r1, [r4, #180]
	ldr	r0, .L691+44
	bl	NAND_Print_DBG
	ldr	r1, [r4, #132]
	ldr	r0, .L691+48
	bl	NAND_Print_DBG
	ldr	r1, [r4, #136]
	ldr	r0, .L691+52
	bl	NAND_Print_DBG
	ldr	r1, [r4, #140]
	ldr	r0, .L691+56
	bl	NAND_Print_DBG
	ldr	r1, [r4, #144]
	ldr	r0, .L691+60
	bl	NAND_Print_DBG
	ldr	r1, [r4, #148]
	ldr	r0, .L691+64
	bl	NAND_Print_DBG
	ldr	r1, [r4, #152]
	ldr	r0, .L691+68
	bl	NAND_Print_DBG
	ldr	r1, [r4, #156]
	ldr	r0, .L691+72
	bl	NAND_Print_DBG
	ldr	r1, [r4, #160]
	ldr	r0, .L691+76
	bl	NAND_Print_DBG
	ldr	r1, [r4, #164]
	ldr	r0, .L691+80
	bl	NAND_Print_DBG
	ldr	r1, [r4, #168]
	ldr	r0, .L691+84
	bl	NAND_Print_DBG
	ldr	r1, [r4, #172]
	ldr	r0, .L691+88
	bl	NAND_Print_DBG
	ldr	r1, [r4, #176]
	ldr	r0, .L691+92
	pop	{r4, lr}
	b	NAND_Print_DBG
.L689:
	pop	{r4, pc}
.L692:
	.align	2
.L691:
	.word	.LC85
	.word	.LC86
	.word	.LC87
	.word	.LC88
	.word	.LC89
	.word	.LC90
	.word	.LC91
	.word	.LC92
	.word	.LC93
	.word	.LC94
	.word	.LC95
	.word	.LC96
	.word	.LC97
	.word	.LC98
	.word	.LC99
	.word	.LC100
	.word	.LC101
	.word	.LC102
	.word	.LC103
	.word	.LC104
	.word	.LC105
	.word	.LC106
	.word	.LC107
	.word	.LC108
	.size	print_smart, .-print_smart
	.section	.text.init_smart_info,"ax",%progbits
	.align	1
	.global	init_smart_info
	.syntax unified
	.thumb
	.thumb_func
	.fpu softvfp
	.type	init_smart_info, %function
init_smart_info:
	@ args = 0, pretend = 0, frame = 32
	@ frame_needed = 0, uses_anonymous_args = 0
	push	{r4, r5, r6, lr}
	mov	r2, #320
	mov	r4, r0
	sub	sp, sp, #32
	mov	r5, r1
	movs	r1, #0
	ldr	r0, [r0, #1968]
	bl	NAND_Memset
	ldr	r3, [r4, #1968]
	ldr	r6, .L707
	ldr	r2, [r3, #140]
	str	r6, [r3, #112]
	adds	r2, r2, #1
	str	r2, [r3, #140]
	cmp	r5, #0
	beq	.L694
	ldr	r3, [r4, #4]
	ldrh	r0, [r5]
	ldr	r2, [r4, #284]
	ldrh	r1, [r3, #28]
	ldrh	r3, [r3, #20]
	strh	r0, [sp, #18]	@ movhi
	mov	r0, r4
	strh	r1, [sp, #20]	@ movhi
	add	r1, sp, #16
	subs	r3, r3, #1
	str	r2, [sp, #24]
	strh	r3, [sp, #16]	@ movhi
	str	sp, [sp, #28]
	ldr	r3, [r4, #1980]
	blx	r3
	mov	r5, r0
	mov	r0, sp
	bl	is_phy_mapping_page
	cmp	r0, #1
	bne	.L696
	cbz	r5, .L697
	cmp	r5, #10
	bne	.L696
.L697:
	ldr	r3, [r4, #308]
	ldr	r1, [r4, #284]
	add	r1, r1, r3
	ldr	r3, [r1, #112]
	cmp	r3, r6
	bne	.L696
	mov	r2, #320
	ldr	r0, [r4, #1968]
	bl	NAND_Memcpy
	ldr	r3, [r4, #1968]
	ldr	r2, [r3, #140]
	adds	r2, r2, #1
	str	r2, [r3, #140]
	ldr	r2, [r3, #164]
	adds	r2, r2, #1
	itt	eq
	moveq	r2, #0
	streq	r2, [r3, #164]
	ldr	r2, [r3, #172]
	adds	r2, r2, #1
	itt	eq
	moveq	r2, #0
	streq	r2, [r3, #172]
	ldr	r2, [r3, #176]
	adds	r2, r2, #1
	itt	eq
	moveq	r2, #0
	streq	r2, [r3, #176]
.L696:
	mov	r0, r4
	bl	print_smart
.L694:
	movs	r0, #0
	add	sp, sp, #32
	@ sp needed
	pop	{r4, r5, r6, pc}
.L708:
	.align	2
.L707:
	.word	-1431655935
	.size	init_smart_info, .-init_smart_info
	.section	.text.print_nftl_zone,"ax",%progbits
	.align	1
	.global	print_nftl_zone
	.syntax unified
	.thumb
	.thumb_func
	.fpu softvfp
	.type	print_nftl_zone, %function
print_nftl_zone:
	@ args = 0, pretend = 0, frame = 0
	@ frame_needed = 0, uses_anonymous_args = 0
	push	{r0, r1, r2, r3, r4, lr}
	mov	r4, r0
	ldr	r3, [r0, #356]
	cmp	r3, #0
	beq	.L709
	ldr	r0, .L715
	bl	NAND_Print_DBG
	ldr	r3, [r4, #4]
	ldr	r0, .L715+4
	ldrh	r1, [r3, #24]
	bl	NAND_Print_DBG
	ldr	r3, [r4, #4]
	ldr	r0, .L715+8
	ldrh	r1, [r3, #26]
	bl	NAND_Print_DBG
	ldr	r3, [r4, #4]
	ldr	r0, .L715+12
	ldrh	r1, [r3, #20]
	bl	NAND_Print_DBG
	ldr	r3, [r4, #4]
	ldr	r0, .L715+16
	ldrh	r1, [r3, #16]
	bl	NAND_Print_DBG
	ldr	r3, [r4, #4]
	ldr	r0, .L715+20
	ldrh	r1, [r3, #30]
	bl	NAND_Print_DBG
	ldr	r1, [r4, #356]
	ldr	r0, .L715+24
	bl	NAND_Print_DBG
	ldrb	r1, [r4, #16]	@ zero_extendqisi2
	ldr	r0, .L715+28
	bl	NAND_Print_DBG
	ldrb	r1, [r4, #17]	@ zero_extendqisi2
	ldr	r0, .L715+32
	bl	NAND_Print_DBG
	ldrh	r1, [r4, #18]
	ldr	r0, .L715+36
	bl	NAND_Print_DBG
	ldrh	r1, [r4, #20]
	ldr	r0, .L715+40
	bl	NAND_Print_DBG
	ldr	r1, [r4, #24]
	ldr	r0, .L715+44
	bl	NAND_Print_DBG
	ldr	r1, [r4, #32]
	ldr	r0, .L715+48
	bl	NAND_Print_DBG
	ldrh	r1, [r4, #22]
	ldr	r0, .L715+52
	bl	NAND_Print_DBG
	ldrh	r1, [r4, #380]
	ldr	r0, .L715+56
	bl	NAND_Print_DBG
	ldrh	r1, [r4, #382]
	ldr	r0, .L715+60
	bl	NAND_Print_DBG
	ldrh	r1, [r4, #388]
	ldr	r0, .L715+64
	bl	NAND_Print_DBG
	ldrh	r1, [r4, #384]
	ldr	r0, .L715+68
	bl	NAND_Print_DBG
	ldrh	r1, [r4, #394]
	ldr	r0, .L715+72
	bl	NAND_Print_DBG
	ldr	r3, [r4, #40]
	ldr	r0, .L715+76
	ldrh	r1, [r3]
	bl	NAND_Print_DBG
	ldr	r3, [r4, #44]
	ldr	r0, .L715+80
	ldrh	r1, [r3]
	bl	NAND_Print_DBG
	add	r3, r4, #272
	ldm	r3, {r0, r1, r2}
	add	r3, r4, #260
	stm	sp, {r0, r1, r2}
	ldr	r0, .L715+84
	ldm	r3, {r1, r2, r3}
	bl	NAND_Print_DBG
	add	r3, r4, #308
	ldm	r3, {r0, r1, r2}
	add	r3, r4, #296
	stm	sp, {r0, r1, r2}
	ldr	r0, .L715+88
	ldm	r3, {r1, r2, r3}
	bl	NAND_Print_DBG
	ldr	r1, [r4, #292]
	ldr	r0, .L715+92
	bl	NAND_Print_DBG
	ldr	r3, [r4, #292]
	ldr	r0, .L715+96
	ldrh	r1, [r3]
	bl	NAND_Print_DBG
	ldrh	r1, [r4, #320]
	ldr	r0, .L715+100
	bl	NAND_Print_DBG
	ldr	r3, [r4, #292]
	ldr	r0, .L715+104
	ldr	r1, [r3, #8]
	bl	NAND_Print_DBG
	ldr	r3, [r4, #48]
	ldr	r0, .L715+108
	ldrh	r2, [r3, #6]
	ldrh	r1, [r3]
	bl	NAND_Print_DBG
	ldrb	r3, [r4, #17]	@ zero_extendqisi2
	lsls	r3, r3, #31
	bpl	.L711
	add	r3, r4, #340
	ldm	r3, {r0, r1, r2}
	add	r3, r4, #328
	stm	sp, {r0, r1, r2}
	ldr	r0, .L715+112
	ldm	r3, {r1, r2, r3}
	bl	NAND_Print_DBG
	ldr	r1, [r4, #324]
	ldr	r0, .L715+116
	bl	NAND_Print_DBG
	ldr	r3, [r4, #324]
	ldr	r0, .L715+120
	ldrh	r1, [r3]
	bl	NAND_Print_DBG
	ldrh	r1, [r4, #352]
	ldr	r0, .L715+124
	bl	NAND_Print_DBG
	ldr	r3, [r4, #324]
	ldr	r0, .L715+128
	ldr	r1, [r3, #8]
	bl	NAND_Print_DBG
.L711:
	ldr	r1, [r4, #360]
	ldr	r0, .L715+132
	bl	NAND_Print_DBG
	ldr	r1, [r4, #284]
	ldr	r0, .L715+136
	bl	NAND_Print_DBG
	ldr	r1, [r4, #376]
	ldr	r0, .L715+140
	bl	NAND_Print_DBG
	ldrh	r1, [r4, #584]
	ldr	r0, .L715+144
	bl	NAND_Print_DBG
	ldr	r1, [r4, #368]
	ldr	r0, .L715+148
	bl	NAND_Print_DBG
	ldr	r1, [r4, #372]
	ldr	r0, .L715+152
	bl	NAND_Print_DBG
	ldr	r3, [r4, #1972]
	ldr	r0, .L715+156
	ldrh	r1, [r3]
	bl	NAND_Print_DBG
	ldr	r3, [r4, #1972]
	ldr	r0, .L715+160
	ldrh	r1, [r3, #2]
	bl	NAND_Print_DBG
	ldr	r3, [r4, #1972]
	ldr	r0, .L715+164
	ldrh	r1, [r3, #4]
	bl	NAND_Print_DBG
	ldr	r3, [r4, #1972]
	ldr	r0, .L715+168
	ldrh	r1, [r3, #6]
	bl	NAND_Print_DBG
	ldr	r3, [r4, #1972]
	ldr	r0, .L715+172
	ldrh	r1, [r3, #10]
	bl	NAND_Print_DBG
	ldr	r3, [r4, #1972]
	ldr	r0, .L715+176
	ldrh	r1, [r3, #12]
	bl	NAND_Print_DBG
	ldr	r3, [r4, #1972]
	ldr	r0, .L715+180
	ldrh	r1, [r3, #14]
	bl	NAND_Print_DBG
	ldr	r3, [r4, #1972]
	ldr	r0, .L715+184
	ldrh	r1, [r3, #16]
	bl	NAND_Print_DBG
	ldr	r3, [r4, #1972]
	ldr	r0, .L715+188
	ldrh	r1, [r3, #18]
	bl	NAND_Print_DBG
	ldr	r3, [r4, #1972]
	ldr	r0, .L715+192
	ldrh	r1, [r3, #20]
	bl	NAND_Print_DBG
	ldr	r3, [r4, #1972]
	ldr	r0, .L715+196
	ldrh	r1, [r3, #22]
	bl	NAND_Print_DBG
	ldr	r3, [r4, #1972]
	ldr	r0, .L715+200
	ldrh	r1, [r3, #24]
	bl	NAND_Print_DBG
	ldr	r3, [r4, #1972]
	ldr	r0, .L715+204
	ldrh	r1, [r3, #26]
	bl	NAND_Print_DBG
	ldr	r3, [r4, #1972]
	ldr	r0, .L715+208
	ldr	r1, [r3, #32]
	bl	NAND_Print_DBG
	mov	r0, r4
	add	sp, sp, #16
	@ sp needed
	pop	{r4, lr}
	b	print_smart
.L709:
	add	sp, sp, #16
	@ sp needed
	pop	{r4, pc}
.L716:
	.align	2
.L715:
	.word	.LC109
	.word	.LC110
	.word	.LC111
	.word	.LC112
	.word	.LC113
	.word	.LC114
	.word	.LC115
	.word	.LC116
	.word	.LC117
	.word	.LC118
	.word	.LC119
	.word	.LC120
	.word	.LC121
	.word	.LC122
	.word	.LC123
	.word	.LC124
	.word	.LC125
	.word	.LC126
	.word	.LC127
	.word	.LC128
	.word	.LC129
	.word	.LC130
	.word	.LC131
	.word	.LC132
	.word	.LC133
	.word	.LC134
	.word	.LC135
	.word	.LC136
	.word	.LC137
	.word	.LC138
	.word	.LC139
	.word	.LC140
	.word	.LC141
	.word	.LC142
	.word	.LC143
	.word	.LC144
	.word	.LC145
	.word	.LC146
	.word	.LC147
	.word	.LC148
	.word	.LC149
	.word	.LC150
	.word	.LC151
	.word	.LC152
	.word	.LC153
	.word	.LC154
	.word	.LC155
	.word	.LC156
	.word	.LC157
	.word	.LC158
	.word	.LC159
	.word	.LC160
	.word	.LC161
	.size	print_nftl_zone, .-print_nftl_zone
	.section	.text.nftl_set_zone_test,"ax",%progbits
	.align	1
	.global	nftl_set_zone_test
	.syntax unified
	.thumb
	.thumb_func
	.fpu softvfp
	.type	nftl_set_zone_test, %function
nftl_set_zone_test:
	@ args = 0, pretend = 0, frame = 0
	@ frame_needed = 0, uses_anonymous_args = 0
	@ link register save eliminated.
	str	r1, [r0, #356]
	b	print_nftl_zone
	.size	nftl_set_zone_test, .-nftl_set_zone_test
	.section	.text.nand_crc16,"ax",%progbits
	.align	1
	.global	nand_crc16
	.syntax unified
	.thumb
	.thumb_func
	.fpu softvfp
	.type	nand_crc16, %function
nand_crc16:
	@ args = 0, pretend = 0, frame = 0
	@ frame_needed = 0, uses_anonymous_args = 0
	push	{r4, lr}
	add	r2, r2, r1
	ldr	r4, .L721
.L719:
	cmp	r2, r1
	bne	.L720
	pop	{r4, pc}
.L720:
	ldrb	r3, [r1], #1	@ zero_extendqisi2
	eors	r3, r3, r0
	uxtb	r3, r3
	ldrh	r3, [r4, r3, lsl #1]
	eor	r0, r3, r0, lsr #8
	b	.L719
.L722:
	.align	2
.L721:
	.word	.LANCHOR12
	.size	nand_crc16, .-nand_crc16
	.section	.text.crc_wrong,"ax",%progbits
	.align	1
	.syntax unified
	.thumb
	.thumb_func
	.fpu softvfp
	.type	crc_wrong, %function
crc_wrong:
	@ args = 0, pretend = 0, frame = 0
	@ frame_needed = 0, uses_anonymous_args = 0
	push	{r3, r4, r5, r6, r7, lr}
	mov	r6, r0
	mov	r0, r2
	mov	r7, r1
	mov	r5, r2
	bl	is_nouse_page
	cbnz	r0, .L728
	ldr	r3, [r6, #4]
	mov	r0, r5
	ldrb	r4, [r3, #8]	@ zero_extendqisi2
	bl	is_phy_mapping_page
	cbnz	r0, .L725
	lsls	r4, r4, #9
.L726:
	mov	r2, r4
	mov	r1, r7
	movw	r0, #65535
	bl	nand_crc16
	ldrb	r3, [r5, #12]	@ zero_extendqisi2
	mov	r1, r0
	ldrb	r2, [r5, #13]	@ zero_extendqisi2
	orr	r2, r2, r3, lsl #8
	cmp	r0, r2
	beq	.L728
	mov	r3, r4
	ldr	r0, .L729
	bl	NAND_Print
	movs	r0, #1
.L723:
	pop	{r3, r4, r5, r6, r7, pc}
.L725:
	ldr	r4, [r6, #308]
	b	.L726
.L728:
	movs	r0, #0
	b	.L723
.L730:
	.align	2
.L729:
	.word	.LC162
	.size	crc_wrong, .-crc_wrong
	.section	.text.nand_power_off_ecc_error,"ax",%progbits
	.align	1
	.global	nand_power_off_ecc_error
	.syntax unified
	.thumb
	.thumb_func
	.fpu softvfp
	.type	nand_power_off_ecc_error, %function
nand_power_off_ecc_error:
	@ args = 0, pretend = 0, frame = 0
	@ frame_needed = 0, uses_anonymous_args = 0
	push	{r4, r5, lr}
	mov	r4, r2
	ldrb	r5, [r2]	@ zero_extendqisi2
	sub	sp, sp, #60
	cmp	r5, #255
	beq	.L732
	ldr	r0, .L743
.L742:
	bl	NAND_Print
	movs	r3, #1
	movw	r2, #3063
	ldr	r1, .L743+4
	ldr	r0, .L743+8
	bl	NAND_Print
	ldr	r0, .L743+12
	bl	NAND_Print
	ldrb	r0, [r4, #15]	@ zero_extendqisi2
	ldrb	r3, [r4, #2]	@ zero_extendqisi2
	ldrb	r2, [r4, #1]	@ zero_extendqisi2
	str	r0, [sp, #48]
	ldrb	r0, [r4, #14]	@ zero_extendqisi2
	ldrb	r1, [r4]	@ zero_extendqisi2
	str	r0, [sp, #44]
	ldrb	r0, [r4, #13]	@ zero_extendqisi2
	str	r0, [sp, #40]
	ldrb	r0, [r4, #12]	@ zero_extendqisi2
	str	r0, [sp, #36]
	ldrb	r0, [r4, #11]	@ zero_extendqisi2
	str	r0, [sp, #32]
	ldrb	r0, [r4, #10]	@ zero_extendqisi2
	str	r0, [sp, #28]
	ldrb	r0, [r4, #9]	@ zero_extendqisi2
	str	r0, [sp, #24]
	ldrb	r0, [r4, #8]	@ zero_extendqisi2
	str	r0, [sp, #20]
	ldrb	r0, [r4, #7]	@ zero_extendqisi2
	str	r0, [sp, #16]
	ldrb	r0, [r4, #6]	@ zero_extendqisi2
	str	r0, [sp, #12]
	ldrb	r0, [r4, #5]	@ zero_extendqisi2
	str	r0, [sp, #8]
	ldrb	r0, [r4, #4]	@ zero_extendqisi2
	str	r0, [sp, #4]
	ldrb	r0, [r4, #3]	@ zero_extendqisi2
	str	r0, [sp]
	ldr	r0, .L743+16
	bl	NAND_Print
	movs	r0, #1
	b	.L736
.L732:
	ldr	r3, [r0, #1972]
	ldrh	r3, [r3, #28]
	cbz	r3, .L734
	bl	crc_wrong
	cbnz	r0, .L735
.L738:
	movs	r0, #0
.L736:
	add	sp, sp, #60
	@ sp needed
	pop	{r4, r5, pc}
.L735:
	ldr	r0, .L743+20
	b	.L742
.L734:
	ldrb	r3, [r2, #11]	@ zero_extendqisi2
	cmp	r3, #165
	bne	.L737
	ldrb	r3, [r2, #15]	@ zero_extendqisi2
	cmp	r3, #165
	beq	.L738
.L737:
	ldr	r0, .L743+24
	b	.L742
.L744:
	.align	2
.L743:
	.word	.LC163
	.word	.LANCHOR13
	.word	.LC166
	.word	.LC167
	.word	.LC168
	.word	.LC164
	.word	.LC165
	.size	nand_power_off_ecc_error, .-nand_power_off_ecc_error
	.section	.text.recover_phy_page_mapping,"ax",%progbits
	.align	1
	.global	recover_phy_page_mapping
	.syntax unified
	.thumb
	.thumb_func
	.fpu softvfp
	.type	recover_phy_page_mapping, %function
recover_phy_page_mapping:
	@ args = 0, pretend = 0, frame = 32
	@ frame_needed = 0, uses_anonymous_args = 0
	push	{r4, r5, r6, r7, r8, r10, fp, lr}
	mov	r8, r2
	ldr	r10, .L779+20
	movs	r4, #0
	sub	sp, sp, #32
	mov	r5, r0
	ldr	fp, .L779+24
	mov	r7, r1
	ldr	r2, [r0, #308]
	movs	r1, #255
	mov	r0, r8
	bl	NAND_Memset
.L746:
	ldr	r3, [r5, #4]
	ldrh	r2, [r3, #20]
	cmp	r2, r4
	ble	.L749
	ldrh	r2, [r3, #28]
	mov	r0, r5
	ldrh	r1, [r7]
	ldr	r3, [r5, #284]
	strh	r2, [sp, #20]	@ movhi
	strh	r1, [sp, #18]	@ movhi
	add	r1, sp, #16
	str	r3, [sp, #24]
	strh	r4, [sp, #16]	@ movhi
	ldr	r3, [r5, #1980]
	str	sp, [sp, #28]
	blx	r3
	mov	r6, r0
	mov	r0, sp
	bl	is_nouse_page
	cmp	r0, #1
	bne	.L747
	cbnz	r6, .L748
.L750:
	mov	r2, r4
	ldrh	r1, [r7]
	ldr	r0, .L779
	bl	NAND_Print
.L749:
	mov	r0, r4
	b	.L745
.L748:
	cmp	r6, #10
	beq	.L750
.L747:
	mov	r0, sp
	bl	is_ftl_logic_page_data
	cmp	r0, #1
	bne	.L751
	cbz	r6, .L752
	cmp	r6, #10
	bne	.L753
.L752:
	mov	r2, sp
	ldr	r1, [r5, #284]
	mov	r0, r5
	bl	nand_power_off_ecc_error
	cbz	r0, .L754
	mov	r2, r4
	ldrh	r1, [r7]
	ldr	r0, .L779+4
	bl	NAND_Print
.L753:
	mov	r2, r4
	ldrh	r1, [r7]
	ldr	r0, .L779+8
	bl	NAND_Print
	movw	r0, #65535
.L745:
	add	sp, sp, #32
	@ sp needed
	pop	{r4, r5, r6, r7, r8, r10, fp, pc}
.L754:
	mov	r0, sp
	bl	get_logic_page_from_oob
	str	r0, [r8, r4, lsl #2]
	mov	r3, r0
	ldr	r2, [r5, #28]
	cmp	r0, r2
	bcc	.L755
	mov	r2, r4
	ldrh	r1, [r7]
	ldr	r0, .L779+12
	bl	NAND_Print
	mov	r3, #-1
	str	r3, [r8, r4, lsl #2]
.L755:
	cbz	r6, .L760
	cmp	r6, #10
	bne	.L753
.L760:
	adds	r4, r4, #1
	b	.L746
.L751:
	ldrb	r3, [sp, #1]	@ zero_extendqisi2
	cmp	r3, #170
	bne	.L756
	ldr	r0, .L779+16
	mov	r1, r4
.L778:
	bl	NAND_Print
	b	.L755
.L756:
	mov	r0, sp
	bl	is_power_down_info_page
	cmp	r0, #1
	bne	.L757
	cbz	r6, .L758
	cmp	r6, #10
	bne	.L753
.L758:
	mov	r0, sp
	bl	get_special_data_from_oob
	mov	r1, r4
	str	r0, [r8, r4, lsl #2]
	mov	r0, fp
	b	.L778
.L757:
	mov	r1, r4
	mov	r0, r10
	b	.L778
.L780:
	.align	2
.L779:
	.word	.LC169
	.word	.LC170
	.word	.LC175
	.word	.LC171
	.word	.LC172
	.word	.LC174
	.word	.LC173
	.size	recover_phy_page_mapping, .-recover_phy_page_mapping
	.section	.text.recover_block_phy_page_mapping,"ax",%progbits
	.align	1
	.global	recover_block_phy_page_mapping
	.syntax unified
	.thumb
	.thumb_func
	.fpu softvfp
	.type	recover_block_phy_page_mapping, %function
recover_block_phy_page_mapping:
	@ args = 0, pretend = 0, frame = 32
	@ frame_needed = 0, uses_anonymous_args = 0
	push	{r4, r5, r6, r7, r8, r10, fp, lr}
	mov	r10, r2
	movs	r4, #0
	ldr	r2, [r0, #308]
	sub	sp, sp, #40
	mov	r5, r0
	mov	r7, r1
	mov	r0, r10
	movs	r1, #255
	bl	NAND_Memset
.L782:
	ldr	r2, [r5, #4]
	ldrh	r3, [r2, #20]
	subs	r3, r3, #1
	cmp	r3, r4
	bgt	.L793
	movs	r0, #0
	add	sp, sp, #40
	@ sp needed
	pop	{r4, r5, r6, r7, r8, r10, fp, pc}
.L793:
	ldr	r3, [r5, #1972]
	uxth	r0, r4
	ldrh	r1, [r7]
	add	r6, sp, #8
	ldrh	r3, [r3, #28]
	cmp	r3, #0
	beq	.L783
	ldr	r3, [r5, #284]
	ldrh	r2, [r2, #28]
	strh	r0, [sp, #24]	@ movhi
	strh	r1, [sp, #26]	@ movhi
	strh	r2, [sp, #28]	@ movhi
.L804:
	mov	fp, #0
	strd	r3, r6, [sp, #32]
.L787:
	ldr	r3, [r5, #1980]
	add	r1, sp, #24
	mov	r0, r5
	blx	r3
	subs	r8, r0, #0
	bge	.L785
	mov	r3, r4
	ldrh	r2, [r7]
	mov	r1, r8
	ldr	r0, .L805
	bl	NAND_Print
	cmp	fp, #0
	beq	.L794
	ldrh	r2, [r7]
	mov	r3, r4
	str	fp, [sp]
	mov	r1, r8
	ldr	r0, .L805+4
	bl	NAND_Print
.L785:
	mov	r0, r6
	bl	is_ftl_logic_page_data
	cmp	r0, #1
	bne	.L788
	cmp	r8, #0
	beq	.L789
	cmp	r8, #10
	bne	.L790
.L789:
	mov	r2, r6
	ldr	r1, [r5, #284]
	mov	r0, r5
	bl	nand_power_off_ecc_error
	cbz	r0, .L791
	ldr	r0, .L805+8
	bl	NAND_Print
.L790:
	adds	r4, r4, #1
	b	.L782
.L783:
	strh	r0, [sp, #24]	@ movhi
	strh	r1, [sp, #26]	@ movhi
	strh	r3, [sp, #28]	@ movhi
	b	.L804
.L794:
	mov	fp, #1
	b	.L787
.L791:
	mov	r0, r6
	bl	get_logic_page_from_oob
	str	r0, [r10, r4, lsl #2]
	mov	r3, r0
	ldr	r2, [r5, #28]
	cmp	r0, r2
	bcc	.L790
	mov	r2, r4
	ldrh	r1, [r7]
	ldr	r0, .L805+12
	bl	NAND_Print
	mov	r3, #-1
	str	r3, [r10, r4, lsl #2]
	b	.L790
.L788:
	mov	r0, r6
	bl	is_nouse_page
	cmp	r0, #1
	beq	.L790
	mov	r2, r4
	ldrh	r1, [r7]
	ldr	r0, .L805+16
	bl	NAND_Print
	b	.L790
.L806:
	.align	2
.L805:
	.word	.LC176
	.word	.LC177
	.word	.LC178
	.word	.LC171
	.word	.LC179
	.size	recover_block_phy_page_mapping, .-recover_block_phy_page_mapping
	.section	.text.get_used_page_num,"ax",%progbits
	.align	1
	.global	get_used_page_num
	.syntax unified
	.thumb
	.thumb_func
	.fpu softvfp
	.type	get_used_page_num, %function
get_used_page_num:
	@ args = 0, pretend = 0, frame = 40
	@ frame_needed = 0, uses_anonymous_args = 0
	push	{r4, r5, r6, r7, r8, r10, fp, lr}
	mov	r6, r0
	mov	fp, r2
	mov	r5, r3
	sub	sp, sp, #104
	movs	r4, #0
	str	r1, [sp, #64]
	str	r4, [r2]
.L808:
	ldr	r3, [r6, #4]
	ldrh	r2, [r3, #20]
	cmp	r2, r4
	bhi	.L819
.L807:
	mov	r0, r4
	add	sp, sp, #104
	@ sp needed
	pop	{r4, r5, r6, r7, r8, r10, fp, pc}
.L819:
	cmp	r5, #0
	beq	.L809
	ldr	r2, [r5, #4]
	ldrb	r2, [r2, r4]	@ zero_extendqisi2
	cmp	r2, #0
	bne	.L809
	ldrh	r7, [r3, #26]
	ldr	r2, [r5, #8]
	add	r8, r7, #64
	mla	r8, r4, r8, r2
	add	r7, r7, r8
.L810:
	ldr	r2, [sp, #64]
	uxth	r0, r4
	ldrh	r1, [r2]
	ldr	r2, [r6, #1972]
	ldrh	r2, [r2, #28]
	cmp	r2, #0
	beq	.L811
	ldrh	r3, [r3, #28]
	strh	r0, [sp, #88]	@ movhi
	strh	r1, [sp, #90]	@ movhi
	strh	r3, [sp, #92]	@ movhi
	str	r8, [sp, #96]
.L857:
	str	r7, [sp, #100]
	add	r1, sp, #88
	ldr	r3, [r6, #1980]
	mov	r0, r6
	blx	r3
	mov	r10, r0
	cbz	r0, .L813
	cmp	r0, #10
	beq	.L813
	ldr	r3, [sp, #64]
	mov	r2, r4
	ldr	r0, .L859
	ldrh	r1, [r3]
	bl	NAND_Print
	ldr	r3, [fp]
	adds	r3, r3, #1
	str	r3, [fp]
.L813:
	mov	r0, r7
	bl	is_nouse_page
	cmp	r0, #1
	beq	.L830
	adds	r3, r4, #1
	cmp	r10, #0
	beq	.L815
	cmp	r10, #10
	bne	.L816
.L815:
	cmp	r8, #0
	beq	.L816
	mov	r2, r7
	mov	r1, r8
	mov	r0, r6
	str	r3, [sp, #68]
	bl	nand_power_off_ecc_error
	ldr	r3, [sp, #68]
	cbz	r0, .L818
	ldr	r0, .L859+4
	bl	NAND_Print
	ldr	r3, [sp, #68]
.L816:
	mov	r4, r3
	b	.L808
.L809:
	ldrb	r2, [r3, #8]	@ zero_extendqisi2
	cmp	r2, #8
	ittee	eq
	ldreq	r8, [r6, #284]
	addeq	r7, sp, #72
	addne	r7, sp, #72
	movne	r8, #0
	b	.L810
.L811:
	strh	r0, [sp, #88]	@ movhi
	strh	r1, [sp, #90]	@ movhi
	strh	r2, [sp, #92]	@ movhi
	str	r2, [sp, #96]
	b	.L857
.L818:
	ldr	r2, [r5, #4]
	movs	r1, #1
	strb	r1, [r2, r4]
	ldr	r2, [r5]
	cmp	r2, r3
	it	cc
	strcc	r3, [r5]
	b	.L816
.L827:
	cmp	r5, #0
	beq	.L832
	ldr	r2, [r5, #4]
	ldrb	r2, [r2, r7]	@ zero_extendqisi2
	cmp	r2, #0
	bne	.L832
	ldrh	r2, [r3, #26]
	ldr	r1, [r5, #8]
	add	r8, r2, #64
	mla	r8, r7, r8, r1
	add	fp, r8, r2
.L820:
	ldr	r1, [r6, #1972]
	uxth	ip, r7
	ldr	r2, [sp, #64]
	ldrh	r1, [r1, #28]
	ldrh	r0, [r2]
	cmp	r1, #0
	beq	.L821
	ldrh	r3, [r3, #28]
	strh	ip, [sp, #88]	@ movhi
	strh	r0, [sp, #90]	@ movhi
	strh	r3, [sp, #92]	@ movhi
	str	r8, [sp, #96]
.L858:
	ldr	r3, [r6, #1980]
	add	r1, sp, #88
	str	fp, [sp, #100]
	mov	r0, r6
	blx	r3
	str	r0, [sp, #68]
	mov	r0, fp
	bl	is_nouse_page
	cmp	r0, #1
	add	r10, r7, #1
	beq	.L824
	ldrb	r0, [fp, #15]	@ zero_extendqisi2
	ldr	r2, [sp, #64]
	ldrb	r3, [fp]	@ zero_extendqisi2
	ldrh	r1, [r2]
	str	r0, [sp, #56]
	ldrb	r0, [fp, #14]	@ zero_extendqisi2
	str	r0, [sp, #52]
	ldrb	r0, [fp, #13]	@ zero_extendqisi2
	str	r0, [sp, #48]
	ldrb	r0, [fp, #12]	@ zero_extendqisi2
	str	r0, [sp, #44]
	ldrb	r0, [fp, #11]	@ zero_extendqisi2
	str	r0, [sp, #40]
	ldrb	r0, [fp, #10]	@ zero_extendqisi2
	str	r0, [sp, #36]
	ldrb	r0, [fp, #9]	@ zero_extendqisi2
	str	r0, [sp, #32]
	ldrb	r0, [fp, #8]	@ zero_extendqisi2
	str	r0, [sp, #28]
	ldrb	r0, [fp, #7]	@ zero_extendqisi2
	str	r0, [sp, #24]
	ldrb	r0, [fp, #6]	@ zero_extendqisi2
	str	r0, [sp, #20]
	ldrb	r0, [fp, #5]	@ zero_extendqisi2
	str	r0, [sp, #16]
	ldrb	r0, [fp, #4]	@ zero_extendqisi2
	str	r0, [sp, #12]
	ldrb	r0, [fp, #3]	@ zero_extendqisi2
	str	r0, [sp, #8]
	ldrb	r0, [fp, #2]	@ zero_extendqisi2
	str	r0, [sp, #4]
	ldrb	r2, [fp, #1]	@ zero_extendqisi2
	ldr	r0, .L859+8
	str	r2, [sp]
	mov	r2, r7
	bl	NAND_Print
	ldr	r3, [sp, #68]
	cbz	r3, .L825
	cmp	r3, #10
	bne	.L824
.L825:
	cmp	r8, #0
	beq	.L824
	ldr	r3, [r5, #4]
	movs	r2, #1
	strb	r2, [r3, r7]
	ldr	r3, [r5]
	cmp	r3, r10
	it	cc
	strcc	r10, [r5]
.L824:
	mov	r7, r10
.L814:
	ldr	r3, [r6, #4]
	ldrh	r2, [r3, #20]
	cmp	r2, r7
	bhi	.L827
	b	.L807
.L832:
	add	fp, sp, #72
	mov	r8, #0
	b	.L820
.L821:
	strh	ip, [sp, #88]	@ movhi
	strh	r0, [sp, #90]	@ movhi
	strh	r1, [sp, #92]	@ movhi
	str	r1, [sp, #96]
	b	.L858
.L830:
	mov	r7, r4
	b	.L814
.L860:
	.align	2
.L859:
	.word	.LC180
	.word	.LC181
	.word	.LC182
	.size	get_used_page_num, .-get_used_page_num
	.section	.text.get_used_page_num_no_crosstalk,"ax",%progbits
	.align	1
	.global	get_used_page_num_no_crosstalk
	.syntax unified
	.thumb
	.thumb_func
	.fpu softvfp
	.type	get_used_page_num_no_crosstalk, %function
get_used_page_num_no_crosstalk:
	@ args = 0, pretend = 0, frame = 48
	@ frame_needed = 0, uses_anonymous_args = 0
	push	{r4, r5, r6, r7, r8, r10, fp, lr}
	mov	r6, r3
	movs	r3, #0
	mov	r8, r0
	mov	r10, r3
	mov	fp, r3
	mov	r4, #-1
	mov	r5, r3
	sub	sp, sp, #48
	str	r3, [r2]
	str	r1, [sp, #4]
.L862:
	ldr	r2, [r8, #4]
	ldrh	r3, [r2, #20]
	cmp	r3, r5
	bhi	.L871
	adds	r2, r4, #1
	bne	.L869
	mov	r4, r3
.L872:
	mov	r0, r4
	add	sp, sp, #48
	@ sp needed
	pop	{r4, r5, r6, r7, r8, r10, fp, pc}
.L871:
	cbz	r6, .L863
	ldr	r3, [r6, #4]
	ldrb	r3, [r3, r5]	@ zero_extendqisi2
	cbnz	r3, .L863
	ldrh	r7, [r2, #26]
	ldr	r3, [r6, #8]
	add	r1, r7, #64
	mla	r3, r5, r1, r3
	add	r7, r7, r3
.L864:
	ldr	r1, [sp, #4]
	mov	r0, r8
	ldrh	r2, [r2, #28]
	str	r3, [sp, #40]
	ldrh	r1, [r1]
	strh	r2, [sp, #36]	@ movhi
	strh	r5, [sp, #32]	@ movhi
	strh	r1, [sp, #34]	@ movhi
	add	r1, sp, #32
	str	r7, [sp, #44]
	ldr	r2, [r8, #1980]
	str	r3, [sp, #8]
	blx	r2
	ldr	r3, [sp, #8]
	cbz	r0, .L865
	cmp	r0, #10
	beq	.L865
	ldr	r3, [sp, #4]
	cmp	r4, #-1
	it	eq
	moveq	r4, r5
	mov	r2, r5
	ldr	r0, .L892
	ldrh	r1, [r3]
	bl	NAND_Print
.L866:
	adds	r5, r5, #1
	b	.L862
.L863:
	ldrb	r3, [r2, #8]	@ zero_extendqisi2
	cmp	r3, #4
	ittee	eq
	ldreq	r3, [r8, #284]
	addeq	r7, sp, #16
	addne	r7, sp, #16
	movne	r3, #0
	b	.L864
.L865:
	mov	r0, r7
	str	r3, [sp, #8]
	bl	is_nouse_page
	cmp	r0, #1
	ldr	r3, [sp, #8]
	bne	.L867
	add	r10, r10, #1
	cmp	r4, #-1
	it	eq
	moveq	r4, r5
	cmp	r10, #3
	bne	.L866
.L869:
	cmp	fp, r4
	beq	.L872
	ldr	r1, [sp, #4]
	mov	r3, r4
	mov	r2, fp
	ldr	r0, .L892+4
	ldrh	r1, [r1]
	bl	NAND_Print
	ldr	r2, [r8, #1968]
	ldr	r3, [r2, #168]
	adds	r3, r3, #1
	str	r3, [r2, #168]
	movs	r3, #0
.L873:
	ldr	r2, [r6]
	cmp	r2, r3
	bls	.L872
	ldr	r2, [r6, #4]
	ldrb	r2, [r2, r3]	@ zero_extendqisi2
	cbz	r2, .L874
	ldr	r2, [r8, #4]
	ldr	r1, [r6, #8]
	ldrh	r0, [r2, #26]
	add	r1, r1, r0
	add	r2, r0, #64
	mla	r2, r3, r2, r1
	ldr	r1, [r8, #1968]
	ldr	r1, [r1, #168]
	strb	r1, [r2, #14]
.L874:
	adds	r3, r3, #1
	b	.L873
.L867:
	mov	r2, r7
	mov	r1, r3
	mov	r0, r8
	bl	nand_power_off_ecc_error
	cbz	r0, .L870
	ldr	r0, .L892+8
	bl	NAND_Print
	b	.L866
.L870:
	adds	r1, r4, #1
	add	fp, fp, #1
	bne	.L866
	ldr	r2, [r6, #4]
	movs	r1, #1
	adds	r3, r5, #1
	mov	r0, r7
	str	r3, [sp, #12]
	strb	r1, [r2, r5]
	ldr	r2, [r6, #12]
	str	r2, [sp, #8]
	bl	get_logic_page_from_oob
	ldr	r2, [sp, #8]
	ldr	r3, [sp, #12]
	str	r0, [r2, r5, lsl #2]
	ldr	r2, [r6]
	cmp	r2, r3
	it	cc
	strcc	r3, [r6]
	b	.L866
.L893:
	.align	2
.L892:
	.word	.LC183
	.word	.LC185
	.word	.LC184
	.size	get_used_page_num_no_crosstalk, .-get_used_page_num_no_crosstalk
	.section	.text.do_write_error,"ax",%progbits
	.align	1
	.global	do_write_error
	.syntax unified
	.thumb
	.thumb_func
	.fpu softvfp
	.type	do_write_error, %function
do_write_error:
	@ args = 0, pretend = 0, frame = 16
	@ frame_needed = 0, uses_anonymous_args = 0
	push	{r4, r5, r6, r7, r8, r10, fp, lr}
	mov	r4, r0
	ldr	r10, .L916+4
	sub	sp, sp, #24
	mov	r6, r1
	ldr	fp, .L916+8
.L895:
	mov	r0, r4
	bl	out_phy_block_from_free_list
	mov	r5, r0
	cbnz	r0, .L896
.L900:
	ldr	r0, .L916
	bl	NAND_Print
	movs	r0, #1
.L894:
	add	sp, sp, #24
	@ sp needed
	pop	{r4, r5, r6, r7, r8, r10, fp, pc}
.L896:
	ldr	r3, [r4, #4]
	movs	r7, #0
	ldrh	r2, [r0]
	add	r1, sp, #8
	strh	r7, [sp, #8]	@ movhi
	mov	r0, r4
	ldrh	r3, [r3, #28]
	strh	r2, [sp, #10]	@ movhi
	strd	r7, r7, [sp, #16]
	strh	r3, [sp, #12]	@ movhi
	ldr	r3, [r4, #1976]
	blx	r3
	ldrh	r3, [r5, #6]
	strh	r7, [r5, #4]	@ movhi
	adds	r3, r3, #1
	strh	r3, [r5, #6]	@ movhi
	cbz	r0, .L898
	ldr	r3, [r4, #1996]
	add	r1, sp, #8
	mov	r0, r4
	blx	r3
	ldrh	r1, [r5]
	mov	r0, fp
	bl	NAND_Print
	ldrh	r2, [r4, #380]
	ldrh	r3, [r4, #22]
	cmp	r3, r2, lsr #2
	bhi	.L895
	b	.L900
.L898:
	ldr	r3, [r6]
	mov	r8, r0
	ldr	r3, [r3, #8]
	adds	r3, r3, #1
	str	r3, [r5, #8]
.L901:
	ldrh	r3, [r6, #28]
	ldr	r1, [r6]
	cmp	r3, r8
	bhi	.L904
.L902:
	ldrh	r3, [r1, #4]
	movs	r7, #0
	ldrh	r2, [r1]
	mov	r0, r4
	strh	r7, [sp, #8]	@ movhi
	strh	r3, [r5, #4]	@ movhi
	ldr	r3, [r4, #4]
	strh	r2, [sp, #10]	@ movhi
	ldrh	r2, [r1, #6]
	ldrh	r3, [r3, #28]
	strd	r7, r7, [sp, #16]
	adds	r2, r2, #1
	uxth	r2, r2
	strh	r3, [sp, #12]	@ movhi
	bl	erase_block
	ldr	r3, [r6]
	ldrh	r3, [r3, #2]
	strh	r3, [r5, #2]	@ movhi
	str	r5, [r6]
.L905:
	ldrh	r3, [r6, #28]
	cmp	r3, r7
	bhi	.L909
	movs	r0, #0
	b	.L894
.L904:
	ldr	r3, [r6, #12]
	ldr	r3, [r3, r8, lsl #2]
	adds	r3, r3, #1
	beq	.L902
	uxth	r3, r8
	ldr	r7, [r4, #1988]
	mov	r2, r5
	str	r3, [sp]
	mov	r0, r4
	ldr	r3, [r4, #284]
	blx	r7
	cbz	r0, .L903
	ldrh	r1, [r5]
	mov	r0, r10
	bl	NAND_Print
	ldr	r3, [r4, #4]
	mov	r0, r4
	ldrh	r1, [r5]
	ldrh	r2, [r3, #28]
	movs	r3, #0
	strh	r3, [sp, #8]	@ movhi
	strh	r1, [sp, #10]	@ movhi
	add	r1, sp, #8
	strd	r3, r3, [sp, #16]
	strh	r2, [sp, #12]	@ movhi
	ldr	r3, [r4, #1996]
	blx	r3
	b	.L895
.L903:
	add	r8, r8, #1
	b	.L901
.L909:
	ldr	r3, [r6, #12]
	ldr	r1, [r3, r7, lsl #2]
	adds	r3, r1, #1
	beq	.L907
	mov	r0, r4
	bl	get_logic_page_map
	cbz	r0, .L907
	ldr	r3, [r6]
	ldrh	r3, [r3]
	strh	r3, [r0, #2]	@ movhi
	ldrh	r3, [r0]
	bfi	r3, r7, #0, #12
	strh	r3, [r0]	@ movhi
.L907:
	adds	r7, r7, #1
	b	.L905
.L917:
	.align	2
.L916:
	.word	.LC186
	.word	.LC188
	.word	.LC187
	.size	do_write_error, .-do_write_error
	.section	.text.write_phy_data_to_current_block,"ax",%progbits
	.align	1
	.global	write_phy_data_to_current_block
	.syntax unified
	.thumb
	.thumb_func
	.fpu softvfp
	.type	write_phy_data_to_current_block, %function
write_phy_data_to_current_block:
	@ args = 4, pretend = 0, frame = 40
	@ frame_needed = 0, uses_anonymous_args = 0
	push	{r4, r5, r6, r7, r8, r10, fp, lr}
	sub	sp, sp, #48
	add	r7, sp, #16
	mov	r4, r0
	mov	fp, r2
	mov	r0, r7
	movs	r2, #16
	mov	r5, r1
	movs	r1, #165
	str	r3, [sp, #12]
	ldr	r6, [sp, #80]
	mov	r8, #3
	bl	NAND_Memset
	ldr	r3, [r4, #4]
	mov	r1, fp
	movw	r0, #65535
	ldrb	r2, [r3, #8]	@ zero_extendqisi2
	ldr	r3, .L935
	cmp	r6, r3
	itet	ne
	lslne	r2, r2, #9
	ldrheq	r2, [r4, #308]
	uxthne	r2, r2
	bl	nand_crc16
	lsrs	r3, r0, #8
	strb	r0, [sp, #29]
	strb	r3, [sp, #28]
.L921:
	ldr	r0, [r5]
	ldr	lr, [r4, #4]
	ldr	r3, .L935
	ldrh	r1, [r0]
	ldrh	r2, [r5, #28]
	ldrh	ip, [lr, #28]
	cmp	r6, r3
	strh	r1, [sp, #34]	@ movhi
	strh	r2, [sp, #32]	@ movhi
	strh	ip, [sp, #36]	@ movhi
	strd	fp, r7, [sp, #40]
	bne	.L922
	ldrh	r3, [r0, #6]
	mov	r2, r6
	str	r3, [sp]
	ldr	r3, [r0, #8]
.L934:
	mov	r1, r7
	mov	r0, r4
	bl	set_oob_special_page
	b	.L923
.L922:
	cmp	r6, #192
	bne	.L924
	ldrh	r3, [lr, #20]
	subs	r3, r3, #1
	cmp	r2, r3
	bne	.L925
	ldr	r0, .L935+4
	bl	NAND_Print_DBG
.L925:
	ldr	r3, [r5]
	mov	r1, r7
	mov	r0, r4
	ldrh	r2, [r3, #6]
	str	r2, [sp]
	ldr	r2, [sp, #12]
	ldr	r3, [r3, #8]
	bl	set_oob_logic_page
.L923:
	ldr	r3, [r5]
	mov	r10, #1
	add	r1, sp, #32
	mov	r0, r4
	strh	r10, [r3, #2]	@ movhi
	ldr	r3, [r4, #1984]
	blx	r3
	cbz	r0, .L918
	subs	r8, r8, #1
	beq	.L928
	ldrh	r1, [sp, #34]
	ldr	r0, .L935+8
	bl	NAND_Print_DBG
	mov	r1, r5
	mov	r0, r4
	bl	do_write_error
	cmp	r0, #0
	beq	.L921
	ldr	r0, .L935+12
	bl	NAND_Print
	mov	r0, r10
.L918:
	add	sp, sp, #48
	@ sp needed
	pop	{r4, r5, r6, r7, r8, r10, fp, pc}
.L924:
	ldr	r3, .L935+16
	cmp	r6, r3
	bne	.L923
	ldrh	r3, [lr, #20]
	subs	r3, r3, #1
	cmp	r2, r3
	bne	.L926
	ldr	r0, .L935+20
	bl	NAND_Print_DBG
.L926:
	ldr	r3, [r5]
	ldrh	r2, [r3, #6]
	str	r2, [sp]
	ldr	r2, .L935+16
	ldr	r3, [r3, #8]
	b	.L934
.L928:
	mov	r0, r8
	b	.L918
.L936:
	.align	2
.L935:
	.word	-1431633921
	.word	.LC189
	.word	.LC191
	.word	.LC192
	.word	-1441713357
	.word	.LC190
	.size	write_phy_data_to_current_block, .-write_phy_data_to_current_block
	.section	.text.write_logic_data_to_current_block,"ax",%progbits
	.align	1
	.global	write_logic_data_to_current_block
	.syntax unified
	.thumb
	.thumb_func
	.fpu softvfp
	.type	write_logic_data_to_current_block, %function
write_logic_data_to_current_block:
	@ args = 4, pretend = 0, frame = 0
	@ frame_needed = 0, uses_anonymous_args = 0
	cmp	r2, #192
	push	{r0, r1, r4, r5, r6, r7, r8, r10, fp, lr}
	mov	r8, r0
	mov	r4, r1
	mov	fp, r3
	bne	.L938
	mov	r1, r3
	bl	get_logic_page_map
	mov	r10, r0
	cbnz	r0, .L939
.L945:
	movs	r7, #1
	ldr	r0, .L961
	bl	NAND_Print_DBG
.L937:
	mov	r0, r7
	add	sp, sp, #8
	@ sp needed
	pop	{r4, r5, r6, r7, r8, r10, fp, pc}
.L939:
	ldrh	r3, [r0, #2]
	movw	r2, #65535
	cmp	r3, r2
	beq	.L953
	ldr	r2, [r8, #4]
	movs	r5, #36
	ldr	r1, [r2, #32]
	mla	r5, r5, r3, r1
	ldr	r3, [r4]
	cmp	r3, r5
	bne	.L952
	ldrh	r3, [r5, #4]
	ldrh	r2, [r2, #20]
	adds	r3, r3, #1
	uxth	r3, r3
	cmp	r2, r3
	strh	r3, [r5, #4]	@ movhi
	bcs	.L953
	ldrh	r1, [r4, #28]
	ldr	r0, .L961+4
	bl	NAND_Print_DBG
.L953:
	movs	r6, #0
	b	.L941
.L952:
	movs	r6, #1
.L941:
	movs	r3, #192
	ldr	r2, [sp, #40]
	str	r3, [sp]
	mov	r1, r4
	mov	r3, fp
	mov	r0, r8
	bl	write_phy_data_to_current_block
	mov	r7, r0
	cbz	r0, .L942
	ldr	r0, .L961+8
.L960:
	bl	NAND_Print
	b	.L937
.L942:
	ldr	r3, [r4]
	ldrh	r1, [r4, #28]
	ldr	r2, [r8, #364]
	ldrh	r3, [r3]
	ubfx	r0, r1, #0, #12
	adds	r1, r1, #1
	clz	r2, r2
	strh	r3, [r10, #2]	@ movhi
	lsrs	r2, r2, #5
	ldrh	r3, [r10]
	bfi	r3, r0, #0, #12
	strh	r3, [r10]	@ movhi
	ubfx	r3, r3, #8, #8
	bfi	r3, r2, #4, #1
	strb	r3, [r10, #1]
	ldr	r3, [r4, #12]
	str	fp, [r3, r0, lsl #2]
	strh	r1, [r4, #28]	@ movhi
.L943:
	cmp	r6, #0
	beq	.L948
	mov	r1, r5
	mov	r0, r8
	bl	phy_block_from_invalid_page_incr
	b	.L948
.L938:
	ldr	r6, .L961+12
	cmp	r2, r6
	bne	.L944
	mov	r1, r3
	bl	get_logic_page_map
	mov	r7, r0
	cmp	r0, #0
	beq	.L945
	ldrh	r3, [r0, #2]
	movw	r2, #65535
	cmp	r3, r2
	beq	.L956
	ldr	r2, [r8, #4]
	movs	r5, #36
	ldr	r1, [r2, #32]
	mla	r5, r5, r3, r1
	ldr	r3, [r4]
	cmp	r3, r5
	bne	.L955
	ldrh	r3, [r0]
	mov	r0, #-1
	ldr	r1, [r4, #12]
	ldrh	r2, [r2, #20]
	ubfx	r3, r3, #0, #12
	str	r0, [r1, r3, lsl #2]
	ldrh	r3, [r5, #4]
	adds	r3, r3, #1
	uxth	r3, r3
	cmp	r2, r3
	strh	r3, [r5, #4]	@ movhi
	bcs	.L956
	ldrh	r1, [r4, #28]
	ldr	r0, .L961+16
	bl	NAND_Print
.L956:
	movs	r6, #0
	b	.L946
.L955:
	movs	r6, #1
.L946:
	movw	r3, #65535
	movw	r2, #4095
	strh	r3, [r7, #2]	@ movhi
	ldrh	r3, [r7]
	bfi	r3, r2, #0, #12
	strh	r3, [r7]	@ movhi
	ubfx	r3, r3, #8, #8
	orr	r3, r3, #16
	strb	r3, [r7, #1]
	b	.L943
.L944:
	ldr	r5, .L961+20
	cmp	r2, r5
	bne	.L948
	ldr	r6, [r1]
	ldrh	r5, [r6, #4]
	adds	r5, r5, #1
	strh	r5, [r6, #4]	@ movhi
	str	r2, [sp]
	ldr	r2, [sp, #40]
	bl	write_phy_data_to_current_block
	mov	r7, r0
	cbz	r0, .L949
	ldr	r0, .L961+24
	b	.L960
.L949:
	ldrh	r3, [r4, #28]
	adds	r3, r3, #1
	strh	r3, [r4, #28]	@ movhi
.L948:
	mov	r0, r8
	movs	r7, #0
	bl	adjust_invaild_list
	b	.L937
.L962:
	.align	2
.L961:
	.word	.LC193
	.word	.LC194
	.word	.LC195
	.word	-1441722095
	.word	.LC196
	.word	-1441713357
	.word	.LC197
	.size	write_logic_data_to_current_block, .-write_logic_data_to_current_block
	.section	.text.write_data_to_page,"ax",%progbits
	.align	1
	.global	write_data_to_page
	.syntax unified
	.thumb
	.thumb_func
	.fpu softvfp
	.type	write_data_to_page, %function
write_data_to_page:
	@ args = 0, pretend = 0, frame = 0
	@ frame_needed = 0, uses_anonymous_args = 0
	push	{r0, r1, r4, r5, r6, r7, r8, lr}
	mov	r6, r1
	str	r3, [sp]
	mov	r7, r2
	mov	r8, r3
	mov	r3, r2
	mov	r2, r1
	add	r1, r0, #292
	mov	r4, r0
	bl	write_logic_data_to_current_block
	mov	r5, r0
	cbz	r0, .L964
.L967:
	movs	r5, #1
	ldr	r0, .L969
	bl	NAND_Print
.L963:
	mov	r0, r5
	add	sp, sp, #8
	@ sp needed
	pop	{r4, r5, r6, r7, r8, pc}
.L964:
	ldrb	r3, [r4, #17]	@ zero_extendqisi2
	lsls	r3, r3, #31
	bpl	.L963
	str	r8, [sp]
	mov	r3, r7
	mov	r2, r6
	add	r1, r4, #324
	mov	r0, r4
	bl	write_logic_data_to_current_block
	cmp	r0, #0
	beq	.L963
	b	.L967
.L970:
	.align	2
.L969:
	.word	.LC198
	.size	write_data_to_page, .-write_data_to_page
	.section	.text._nftl_discard_logic_page,"ax",%progbits
	.align	1
	.global	_nftl_discard_logic_page
	.syntax unified
	.thumb
	.thumb_func
	.fpu softvfp
	.type	_nftl_discard_logic_page, %function
_nftl_discard_logic_page:
	@ args = 0, pretend = 0, frame = 0
	@ frame_needed = 0, uses_anonymous_args = 0
	@ link register save eliminated.
	mov	r2, r1
	movs	r3, #0
	ldr	r1, .L972
	b	write_data_to_page
.L973:
	.align	2
.L972:
	.word	-1441722095
	.size	_nftl_discard_logic_page, .-_nftl_discard_logic_page
	.section	.text.get_new_current_block,"ax",%progbits
	.align	1
	.global	get_new_current_block
	.syntax unified
	.thumb
	.thumb_func
	.fpu softvfp
	.type	get_new_current_block, %function
get_new_current_block:
	@ args = 0, pretend = 0, frame = 0
	@ frame_needed = 0, uses_anonymous_args = 0
	push	{r3, r4, r5, r6, r7, lr}
	mov	r4, r1
	ldr	r3, [r1]
	mov	r6, r0
	ldr	r2, [r1, #16]
	movs	r1, #255
	ldr	r0, [r4, #12]
	ldr	r5, [r3, #8]
	bl	NAND_Memset
	ldrh	r3, [r6, #578]
	cbz	r3, .L975
	ldr	r3, [r6, #4]
	mov	r0, r6
	ldrh	r1, [r3, #16]
	bl	out_phy_block_from_free_list_by_erase_time
	str	r0, [r4]
	cbz	r0, .L976
	ldrh	r2, [r0, #6]
	ldrh	r1, [r0]
	ldr	r0, .L983
	bl	NAND_Print_DBG
.L976:
	ldr	r1, [r4]
	cbnz	r1, .L977
	ldr	r0, .L983+4
	bl	NAND_Print
	movs	r0, #1
.L974:
	pop	{r3, r4, r5, r6, r7, pc}
.L975:
	mov	r0, r6
	bl	out_phy_block_from_free_list
	str	r0, [r4]
	b	.L976
.L977:
	adds	r2, r5, #1
	movs	r7, #0
	strh	r7, [r4, #28]	@ movhi
	str	r2, [r1, #8]
	ldrb	r3, [r6, #17]	@ zero_extendqisi2
	ands	r0, r3, #1
	ittt	ne
	addne	r2, r5, #2
	movne	r0, r7
	strne	r2, [r1, #8]
	b	.L974
.L984:
	.align	2
.L983:
	.word	.LC199
	.word	.LC200
	.size	get_new_current_block, .-get_new_current_block
	.section	.text.new_block_init_for_write,"ax",%progbits
	.align	1
	.global	new_block_init_for_write
	.syntax unified
	.thumb
	.thumb_func
	.fpu softvfp
	.type	new_block_init_for_write, %function
new_block_init_for_write:
	@ args = 0, pretend = 0, frame = 16
	@ frame_needed = 0, uses_anonymous_args = 0
	ldr	r3, [r0, #4]
	push	{r4, r5, r6, r7, lr}
	mov	r7, r2
	ldrh	r2, [r1]
	sub	sp, sp, #20
	movs	r6, #0
	mov	r5, r1
	ldrh	r3, [r3, #28]
	mov	r1, sp
	strh	r2, [sp, #2]	@ movhi
	mov	r4, r0
	strh	r6, [sp]	@ movhi
	strh	r3, [sp, #4]	@ movhi
	strd	r6, r6, [sp, #8]
	ldr	r3, [r0, #1976]
	blx	r3
	ldrh	r3, [r5, #6]
	ldr	r2, [r4, #376]
	strh	r6, [r5, #4]	@ movhi
	adds	r3, r3, #1
	uxth	r3, r3
	cmp	r3, r2
	strh	r3, [r5, #6]	@ movhi
	bls	.L986
	ldr	r2, [r4, #4]
	ldrh	r2, [r2, #16]
	cmp	r2, r3
	it	hi
	strhi	r3, [r4, #376]
.L986:
	str	r7, [r5, #8]
	cbz	r0, .L985
	ldr	r3, [r4, #1996]
	mov	r1, sp
	mov	r0, r4
	blx	r3
	ldr	r0, .L991
	bl	NAND_Print
	movs	r0, #1
.L985:
	add	sp, sp, #20
	@ sp needed
	pop	{r4, r5, r6, r7, pc}
.L992:
	.align	2
.L991:
	.word	.LC201
	.size	new_block_init_for_write, .-new_block_init_for_write
	.section	.text.get_current_used_block,"ax",%progbits
	.align	1
	.global	get_current_used_block
	.syntax unified
	.thumb
	.thumb_func
	.fpu softvfp
	.type	get_current_used_block, %function
get_current_used_block:
	@ args = 0, pretend = 0, frame = 0
	@ frame_needed = 0, uses_anonymous_args = 0
	push	{r0, r1, r2, r4, r5, r6, r7, r8, r10, lr}
	mov	r4, r0
	mov	r5, r2
	mov	r6, r1
	cmp	r1, #0
	beq	.L994
	ldr	r3, [r0, #292]
	cmp	r3, r1
	beq	.L995
	movw	r2, #1013
	ldr	r1, .L1028
	ldr	r0, .L1028+4
	bl	NAND_Print
.L995:
	ldr	r3, [r4, #292]
	ldrh	r2, [r4, #320]
	ldr	r0, .L1028+8
	ldrh	r1, [r3]
	bl	NAND_Print_DBG
	ldrh	r3, [r4, #320]
	mov	r0, r4
	ldr	r2, [r4, #304]
	ldr	r1, [r4, #292]
	bl	recover_logic_page_mapping
	ldr	r3, [r4, #292]
	ldrh	r1, [r4, #320]
	ldrh	r2, [r3, #4]
	cmp	r1, r2
	bcs	.L996
	ldr	r0, .L1028+12
	bl	NAND_Print
.L996:
	ldrb	r3, [r4, #17]	@ zero_extendqisi2
	lsls	r3, r3, #31
	bpl	.L1026
	ldr	r6, .L1028+16
	ldr	r8, .L1028+44
.L997:
	ldr	r7, [r4, #324]
	cbz	r7, .L999
.L1008:
	ldr	r2, [r4, #336]
	mov	r0, r4
	ldr	r1, [r4, #324]
	bl	recover_phy_page_mapping
	uxth	r3, r0
	movw	r2, #65535
	cmp	r3, r2
	strh	r3, [r4, #352]	@ movhi
	bne	.L1024
	ldr	r0, .L1028+20
.L1025:
	bl	NAND_Print
	ldr	r1, [r4, #324]
	mov	r0, r4
	ldrh	r2, [r1, #6]
	adds	r2, r2, #1
	uxth	r2, r2
	bl	erase_block
	movs	r3, #0
	str	r3, [r4, #324]
	b	.L997
.L999:
	mov	r0, r4
	bl	out_phy_block_from_free_list
	mov	r1, r0
	str	r0, [r4, #324]
	cbnz	r0, .L1002
.L1027:
	movs	r0, #1
	b	.L993
.L1002:
	ldr	r3, [r4, #292]
	strh	r7, [r4, #352]	@ movhi
	ldr	r2, [r3, #8]
	adds	r2, r2, #1
	str	r2, [r0, #8]
	mov	r0, r4
	bl	new_block_init_for_write
	cbz	r0, .L1003
	mov	r0, r8
	bl	NAND_Print
	str	r7, [r4, #324]
	b	.L997
.L1003:
	ldrh	r10, [r4, #320]
	mov	r5, r0
.L1005:
	cmp	r5, r10
	ldr	r2, [r4, #324]
	bcs	.L1008
	uxth	r3, r5
	ldr	r7, [r4, #1988]
	ldr	r1, [r4, #292]
	mov	r0, r4
	str	r3, [sp]
	ldr	r3, [r4, #284]
	blx	r7
	cbz	r0, .L1006
	mov	r0, r6
	bl	NAND_Print
.L1006:
	adds	r5, r5, #1
	b	.L1005
.L1024:
	ldr	r3, [r4, #292]
	ldr	r1, [r4, #324]
	ldr	r2, [r3, #8]
	uxth	r3, r0
	mov	r0, r4
	adds	r2, r2, #1
	str	r2, [r1, #8]
	ldr	r2, [r4, #336]
	bl	recover_logic_page_mapping
	ldrh	r1, [r4, #352]
	ldrh	r2, [r4, #320]
	cmp	r1, r2
	bls	.L1009
	ldr	r0, .L1028+24
	bl	NAND_Print
	ldrh	r2, [r4, #320]
	ldrh	r1, [r4, #352]
	ldr	r0, .L1028+28
	bl	NAND_Print
.L1026:
	movs	r0, #0
.L993:
	add	sp, sp, #12
	@ sp needed
	pop	{r4, r5, r6, r7, r8, r10, pc}
.L1009:
	bcs	.L1026
	ldr	r0, .L1028+32
	b	.L1025
.L994:
	ldr	r0, .L1028+36
	bl	NAND_Print_DBG
	ldr	r3, [r4, #172]
	cbz	r3, .L1011
	mov	r0, r4
	bl	out_phy_block_from_free_list
	str	r0, [r4, #292]
	cmp	r0, #0
	beq	.L1027
	adds	r5, r5, #1
	strh	r6, [r4, #320]	@ movhi
	str	r5, [r0, #8]
	ldrb	r0, [r4, #17]	@ zero_extendqisi2
	ands	r0, r0, #1
	beq	.L1013
	mov	r0, r4
	bl	out_phy_block_from_free_list
	str	r0, [r4, #324]
	cmp	r0, #0
	beq	.L1027
	ldr	r3, [r4, #292]
	strh	r6, [r4, #352]	@ movhi
	ldr	r3, [r3, #8]
	adds	r3, r3, #1
	str	r3, [r0, #8]
	b	.L1026
.L1013:
	str	r0, [r4, #324]
	b	.L993
.L1011:
	ldr	r0, .L1028+40
	bl	NAND_Print
	mov	r0, r4
	bl	print_free_list
	mov	r0, r4
	bl	print_block_invalid_list
	b	.L1027
.L1029:
	.align	2
.L1028:
	.word	.LANCHOR14
	.word	.LC202
	.word	.LC203
	.word	.LC204
	.word	.LC206
	.word	.LC207
	.word	.LC208
	.word	.LC209
	.word	.LC210
	.word	.LC211
	.word	.LC212
	.word	.LC205
	.size	get_current_used_block, .-get_current_used_block
	.section	.text.cross_talk_rebuild_current_block,"ax",%progbits
	.align	1
	.global	cross_talk_rebuild_current_block
	.syntax unified
	.thumb
	.thumb_func
	.fpu softvfp
	.type	cross_talk_rebuild_current_block, %function
cross_talk_rebuild_current_block:
	@ args = 0, pretend = 0, frame = 40
	@ frame_needed = 0, uses_anonymous_args = 0
	push	{r4, r5, r6, r7, r8, r10, fp, lr}
	sub	sp, sp, #56
	mov	r6, r1
	movs	r7, #0
	add	r1, sp, #56
	mov	r5, r0
	str	r7, [r1, #-20]!
	mov	r4, r2
	bl	init_nrc
	cbz	r0, .L1031
	ldr	r0, [sp, #36]
	mov	r4, r7
	bl	free_nrc
	ldr	r0, .L1051
	bl	NAND_Print
.L1032:
	mov	r0, r4
	add	sp, sp, #56
	@ sp needed
	pop	{r4, r5, r6, r7, r8, r10, fp, pc}
.L1031:
	ldr	r3, [sp, #36]
	add	r2, sp, #28
	mov	r1, r6
	mov	r0, r5
	bl	get_used_page_num
	mov	r10, r0
	cbnz	r4, .L1033
	ldr	r3, [r6, #8]
	mov	r2, r0
	ldrh	r1, [r6]
	ldr	r0, .L1051+4
	bl	NAND_Print
	ldr	r0, [sp, #36]
	ldr	r3, [r0]
	cmp	r3, #0
	bne	.L1046
	bl	free_nrc
	ldr	r0, .L1051+8
	bl	NAND_Print
	ldrh	r2, [r6, #6]
	mov	r1, r6
	mov	r0, r5
	adds	r2, r2, #1
	uxth	r2, r2
	bl	erase_block
	b	.L1032
.L1033:
	ldr	r3, [sp, #36]
	add	r2, sp, #32
	mov	r1, r4
	mov	r0, r5
	bl	get_used_page_num
	ldr	r3, [r4, #8]
	mov	fp, r0
	ldrh	r1, [r6]
	mov	r2, r10
	strd	r0, r3, [sp, #4]
	ldrh	r3, [r4]
	ldr	r0, .L1051+12
	str	r3, [sp]
	ldr	r3, [r6, #8]
	bl	NAND_Print
	ldr	r0, [sp, #36]
	ldr	r3, [r0]
	cbnz	r3, .L1035
	str	r3, [sp, #20]
	bl	free_nrc
	ldr	r0, .L1051+16
	bl	NAND_Print
	ldrh	r2, [r6, #6]
	mov	r1, r6
	mov	r0, r5
	adds	r2, r2, #1
	uxth	r2, r2
	bl	erase_block
	ldrh	r2, [r4, #6]
	mov	r1, r4
	mov	r0, r5
	adds	r2, r2, #1
	uxth	r2, r2
	bl	erase_block
	ldr	r3, [sp, #20]
	mov	r4, r3
	b	.L1032
.L1035:
	ldrh	r7, [r4, #6]
	cmp	r10, fp
	ldrh	r8, [r6, #6]
	ittt	ls
	movls	r2, r7
	movls	r7, r8
	movls	r8, r2
	ldr	r2, [sp, #28]
	cbnz	r2, .L1037
	ldr	r2, [sp, #32]
	cbz	r2, .L1038
.L1037:
	adds	r7, r7, #20
	add	r8, r8, #20
.L1038:
	cmp	r10, fp
	add	r7, r7, #1
	uxth	r7, r7
	mov	r2, fp
	mov	r1, r10
	bcs	.L1039
	ldr	r0, .L1051+20
	bl	NAND_Print
	mov	r1, r6
	mov	r6, r4
	mov	r2, r7
	mov	r0, r5
	bl	erase_block
.L1034:
	ldr	r7, .L1051+24
.L1042:
	mov	r0, r5
	bl	out_phy_block_from_free_list
	mov	r4, r0
	cbnz	r0, .L1040
	ldr	r0, .L1051+28
	bl	NAND_Print
	b	.L1032
.L1039:
	ldr	r0, .L1051+32
	bl	NAND_Print
	mov	r2, r7
	mov	r1, r4
	mov	r0, r5
	bl	erase_block
	b	.L1034
.L1046:
	mov	r8, r4
	b	.L1034
.L1040:
	ldr	r2, [r6, #8]
	mov	r1, r0
	adds	r2, r2, #1
	str	r2, [r0, #8]
	mov	r0, r5
	bl	new_block_init_for_write
	cmp	r0, #0
	beq	.L1047
	mov	r0, r7
	bl	NAND_Print
	b	.L1042
.L1045:
	ldr	r3, [r0, #4]
	ldrb	r3, [r3, r10]	@ zero_extendqisi2
	cmp	r3, #0
	beq	.L1043
	ldr	r2, [r5, #4]
	ldr	r0, [r0, #8]
	ldrh	r1, [r2, #26]
	ldrh	r2, [r2, #28]
	add	r3, r1, #64
	mla	r3, r10, r3, r0
	ldrh	r0, [r4]
	strh	r7, [sp, #40]	@ movhi
	strh	r2, [sp, #44]	@ movhi
	adds	r7, r7, #1
	strh	r0, [sp, #42]	@ movhi
	add	r1, r1, r3
	mov	r0, r1
	strd	r3, r1, [sp, #48]
	str	r1, [sp, #20]
	bl	get_special_data_from_oob
	ldrh	r3, [r4, #6]
	mov	r2, r0
	ldr	r1, [sp, #20]
	mov	r0, r5
	str	r3, [sp]
	ldr	r3, [r4, #8]
	bl	set_oob_special_page
	ldr	r3, [r5, #1984]
	add	r1, sp, #40
	mov	r0, r5
	blx	r3
.L1044:
	add	r10, r10, #1
.L1041:
	ldr	r0, [sp, #36]
	ldr	r2, [r0]
	cmp	r2, r10
	bhi	.L1045
	add	r2, r8, #1
	mov	r1, r6
	uxth	r2, r2
	mov	r0, r5
	bl	erase_block
	ldr	r0, [sp, #36]
	bl	free_nrc
	b	.L1032
.L1043:
	mov	r1, r10
	mov	r0, fp
	bl	NAND_Print
	b	.L1044
.L1047:
	mov	r7, r0
	mov	r10, r0
	ldr	fp, .L1051+36
	b	.L1041
.L1052:
	.align	2
.L1051:
	.word	.LC213
	.word	.LC214
	.word	.LC215
	.word	.LC216
	.word	.LC217
	.word	.LC218
	.word	.LC221
	.word	.LC220
	.word	.LC219
	.word	.LC222
	.size	cross_talk_rebuild_current_block, .-cross_talk_rebuild_current_block
	.section	.text.no_cross_talk_rebuild_current_block,"ax",%progbits
	.align	1
	.global	no_cross_talk_rebuild_current_block
	.syntax unified
	.thumb
	.thumb_func
	.fpu softvfp
	.type	no_cross_talk_rebuild_current_block, %function
no_cross_talk_rebuild_current_block:
	@ args = 0, pretend = 0, frame = 56
	@ frame_needed = 0, uses_anonymous_args = 0
	push	{r4, r5, r6, r7, r8, r10, fp, lr}
	sub	sp, sp, #72
	mov	r6, r1
	movs	r7, #0
	add	r1, sp, #32
	mov	r5, r0
	mov	r4, r2
	strd	r7, r7, [sp, #32]
	bl	init_nrc
	cbz	r0, .L1054
	ldr	r0, .L1087
	bl	NAND_Print
	ldr	r0, [sp, #32]
	bl	free_nrc
.L1085:
	movs	r4, #0
	b	.L1055
.L1054:
	ldr	r3, [sp, #32]
	add	r2, sp, #24
	mov	r1, r6
	mov	r0, r5
	bl	get_used_page_num_no_crosstalk
	mov	r8, r0
	cbnz	r4, .L1056
	ldr	r3, [r6, #8]
	mov	r2, r0
	ldrh	r1, [r6]
	ldr	r0, .L1087+4
	bl	NAND_Print
	ldr	r7, [sp, #32]
	ldr	r3, [r7]
	cbz	r3, .L1084
	ldr	r8, .L1087+40
.L1057:
	mov	r0, r5
	bl	out_phy_block_from_free_list
	mov	r4, r0
	cbnz	r0, .L1058
.L1066:
	ldr	r0, .L1087+8
.L1086:
	bl	NAND_Print
	b	.L1085
.L1084:
	mov	r0, r7
	bl	free_nrc
	ldr	r0, .L1087+12
	bl	NAND_Print
	ldrh	r2, [r6, #6]
	mov	r1, r6
	mov	r0, r5
	adds	r2, r2, #1
	uxth	r2, r2
	bl	erase_block
.L1055:
	mov	r0, r4
	add	sp, sp, #72
	@ sp needed
	pop	{r4, r5, r6, r7, r8, r10, fp, pc}
.L1058:
	ldr	r2, [r6, #8]
	mov	r1, r0
	str	r2, [r0, #8]
	mov	r0, r5
	bl	new_block_init_for_write
	cmp	r0, #0
	beq	.L1059
	mov	r0, r8
	bl	NAND_Print
	b	.L1057
.L1056:
	add	r1, sp, #36
	mov	r0, r5
	bl	init_nrc
	cbz	r0, .L1060
	ldr	r0, [sp, #32]
	bl	free_nrc
	ldr	r0, [sp, #36]
	bl	free_nrc
	ldr	r0, .L1087+16
	b	.L1086
.L1060:
	ldr	r3, [sp, #36]
	add	r2, sp, #28
	mov	r1, r4
	mov	r0, r5
	bl	get_used_page_num_no_crosstalk
	ldr	r3, [r4, #8]
	mov	r10, r0
	ldrh	r1, [r6]
	mov	r2, r8
	strd	r0, r3, [sp, #4]
	ldrh	r3, [r4]
	ldr	r0, .L1087+20
	str	r3, [sp]
	ldr	r3, [r6, #8]
	bl	NAND_Print
	ldr	r7, [sp, #36]
	ldr	fp, [r7]
	cmp	fp, #0
	bne	.L1061
	ldr	r0, [sp, #32]
	bl	free_nrc
	ldr	r0, [sp, #36]
	bl	free_nrc
	ldr	r0, .L1087+24
	bl	NAND_Print
	ldrh	r2, [r6, #6]
	mov	r1, r6
	mov	r0, r5
	adds	r2, r2, #1
	uxth	r2, r2
	bl	erase_block
	ldrh	r2, [r4, #6]
	mov	r1, r4
	mov	r0, r5
	adds	r2, r2, #1
	uxth	r2, r2
	bl	erase_block
	b	.L1085
.L1061:
	cmp	r8, r10
	bcs	.L1062
	mov	r3, fp
	mov	r2, r10
	mov	r1, r8
	ldr	r0, .L1087+28
	bl	NAND_Print
	mov	r3, r6
	mov	r6, r4
	mov	r4, r3
.L1063:
	ldr	r3, [r5, #4]
	mov	r8, #0
	ldrh	r2, [r4]
	add	r1, sp, #40
	strh	r8, [sp, #40]	@ movhi
	mov	r0, r5
	ldrh	r3, [r3, #28]
	strh	r2, [sp, #42]	@ movhi
	strd	r8, r8, [sp, #48]
	strh	r3, [sp, #44]	@ movhi
	ldr	r3, [r5, #1976]
	blx	r3
	cbz	r0, .L1064
	ldrh	r1, [r4]
	ldr	r0, .L1087+32
	bl	NAND_Print
	ldr	r3, [r5, #1996]
	add	r1, sp, #40
	mov	r0, r5
	blx	r3
	movs	r3, #255
	strh	r8, [r6, #4]	@ movhi
	strh	r3, [r6, #2]	@ movhi
	ldr	r8, .L1087+40
	ldrh	r3, [r5, #20]
	adds	r3, r3, #1
	strh	r3, [r5, #20]	@ movhi
.L1065:
	mov	r0, r5
	bl	out_phy_block_from_free_list
	mov	r4, r0
	cmp	r0, #0
	beq	.L1066
	ldr	r2, [r0, #8]
	mov	r1, r0
	mov	r0, r5
	bl	new_block_init_for_write
	cbz	r0, .L1064
	mov	r0, r8
	bl	NAND_Print
	b	.L1065
.L1062:
	ldr	r7, [sp, #32]
	mov	r2, r10
	mov	r1, r8
	ldr	r0, .L1087+36
	ldr	r3, [r7]
	bl	NAND_Print
	b	.L1063
.L1064:
	ldr	r3, [r6, #8]
	str	r3, [r4, #8]
.L1059:
	ldr	fp, .L1087+44
	mov	r10, #0
	mov	r8, r10
.L1067:
	ldr	r2, [r7]
	cmp	r2, r8
	bhi	.L1070
	ldrh	r2, [r6, #6]
	mov	r1, r6
	mov	r0, r5
	adds	r2, r2, #1
	uxth	r2, r2
	bl	erase_block
	ldr	r2, [r7]
	ldr	r1, [r7, #12]
	ldr	r0, [r5, #304]
	strh	r2, [r5, #320]	@ movhi
	str	r4, [r5, #292]
	lsls	r2, r2, #2
	bl	NAND_Memcpy
	ldr	r0, [sp, #32]
	bl	free_nrc
	ldr	r0, [sp, #36]
	bl	free_nrc
	b	.L1055
.L1070:
	ldr	r3, [r7, #4]
	ldrb	r3, [r3, r8]	@ zero_extendqisi2
	cbz	r3, .L1068
	ldr	r2, [r5, #4]
	ldr	r0, [r7, #8]
	ldrh	r1, [r2, #26]
	ldrh	r2, [r2, #28]
	add	r3, r1, #64
	mla	r3, r8, r3, r0
	ldrh	r0, [r4]
	strh	r10, [sp, #56]	@ movhi
	strh	r2, [sp, #60]	@ movhi
	add	r10, r10, #1
	strh	r0, [sp, #58]	@ movhi
	add	r1, r1, r3
	mov	r0, r1
	strd	r3, r1, [sp, #64]
	str	r1, [sp, #20]
	bl	get_special_data_from_oob
	ldrh	r3, [r4, #6]
	mov	r2, r0
	ldr	r1, [sp, #20]
	mov	r0, r5
	str	r3, [sp]
	ldr	r3, [r4, #8]
	bl	set_oob_special_page
	ldr	r3, [r5, #1984]
	add	r1, sp, #56
	mov	r0, r5
	blx	r3
.L1069:
	add	r8, r8, #1
	b	.L1067
.L1068:
	mov	r1, r8
	mov	r0, fp
	bl	NAND_Print
	b	.L1069
.L1088:
	.align	2
.L1087:
	.word	.LC213
	.word	.LC223
	.word	.LC220
	.word	.LC224
	.word	.LC217
	.word	.LC225
	.word	.LC226
	.word	.LC218
	.word	.LC227
	.word	.LC219
	.word	.LC221
	.word	.LC222
	.size	no_cross_talk_rebuild_current_block, .-no_cross_talk_rebuild_current_block
	.section	.text.current_block_ecc_error,"ax",%progbits
	.align	1
	.global	current_block_ecc_error
	.syntax unified
	.thumb
	.thumb_func
	.fpu softvfp
	.type	current_block_ecc_error, %function
current_block_ecc_error:
	@ args = 0, pretend = 0, frame = 48
	@ frame_needed = 0, uses_anonymous_args = 0
	push	{r4, r5, r6, r7, r8, r10, fp, lr}
	mov	r4, r0
	ldr	r5, .L1109
	sub	sp, sp, #48
	mov	r8, r1
.L1090:
	mov	r0, r4
	bl	out_phy_block_from_free_list
	mov	r7, r0
	cbnz	r0, .L1091
	ldr	r0, .L1109+4
	bl	NAND_Print
.L1089:
	mov	r0, r7
	add	sp, sp, #48
	@ sp needed
	pop	{r4, r5, r6, r7, r8, r10, fp, pc}
.L1091:
	ldr	r2, [r8, #8]
	mov	r1, r0
	str	r2, [r0, #8]
	mov	r0, r4
	bl	new_block_init_for_write
	mov	r6, r0
	cbz	r0, .L1093
	mov	r0, r5
	bl	NAND_Print
	b	.L1090
.L1093:
	ldr	fp, .L1109+32
	mov	r5, r6
	ldrh	r1, [r8]
	ldr	r0, .L1109+8
	bl	NAND_Print
.L1094:
	ldr	r3, [r4, #4]
	ldrh	r2, [r3, #20]
	cmp	r2, r5
	bgt	.L1101
	ldrh	r2, [r8, #6]
	mov	r1, r8
	mov	r0, r4
	adds	r2, r2, #1
	uxth	r2, r2
	bl	erase_block
	mov	r2, r6
	mov	r1, r5
	ldr	r0, .L1109+12
	bl	NAND_Print_DBG
	b	.L1089
.L1101:
	ldrh	r2, [r3, #28]
	mov	r0, r4
	ldrh	r1, [r8]
	ldr	r3, [r4, #284]
	strh	r2, [sp, #20]	@ movhi
	strh	r1, [sp, #18]	@ movhi
	add	r1, sp, #16
	str	r3, [sp, #24]
	strh	r5, [sp, #16]	@ movhi
	ldr	r3, [r4, #1980]
	str	sp, [sp, #28]
	blx	r3
	mov	r10, r0
	mov	r0, sp
	bl	is_ftl_logic_page_data
	cmp	r0, #1
	bne	.L1095
	cmp	r10, #0
	beq	.L1096
	cmp	r10, #10
	bne	.L1097
.L1096:
	mov	r2, sp
	ldr	r1, [r4, #284]
	mov	r0, r4
	bl	nand_power_off_ecc_error
	cbz	r0, .L1098
	ldr	r0, .L1109+16
	bl	NAND_Print
.L1099:
	adds	r5, r5, #1
	b	.L1094
.L1098:
	mov	r2, r10
	mov	r1, r5
	ldr	r0, .L1109+20
	bl	NAND_Print
	ldr	r3, [r4, #4]
	mov	r0, r4
	ldrh	r1, [r7]
	strh	r6, [sp, #32]	@ movhi
	adds	r6, r6, #1
	ldrh	r2, [r3, #28]
	ldr	r3, [r4, #284]
	strh	r1, [sp, #34]	@ movhi
	add	r1, sp, #32
	strh	r2, [sp, #36]	@ movhi
	str	r3, [sp, #40]
	str	sp, [sp, #44]
	ldr	r3, [r4, #1984]
	blx	r3
	b	.L1099
.L1097:
	ldr	r0, .L1109+24
	mov	r1, r5
.L1108:
	bl	NAND_Print
	b	.L1099
.L1095:
	ldrb	r3, [sp, #1]	@ zero_extendqisi2
	cmp	r3, #170
	bne	.L1100
	mov	r1, r5
	mov	r0, fp
	b	.L1108
.L1100:
	mov	r0, sp
	bl	is_nouse_page
	cmp	r0, #1
	beq	.L1099
	mov	r1, r5
	ldr	r0, .L1109+28
	b	.L1108
.L1110:
	.align	2
.L1109:
	.word	.LC210
	.word	.LC69
	.word	.LC228
	.word	.LC234
	.word	.LC229
	.word	.LC230
	.word	.LC231
	.word	.LC233
	.word	.LC232
	.size	current_block_ecc_error, .-current_block_ecc_error
	.section	.text.do_before_write,"ax",%progbits
	.align	1
	.global	do_before_write
	.syntax unified
	.thumb
	.thumb_func
	.fpu softvfp
	.type	do_before_write, %function
do_before_write:
	@ args = 0, pretend = 0, frame = 0
	@ frame_needed = 0, uses_anonymous_args = 0
	push	{r3, r4, r5, lr}
	mov	r4, r0
	add	r5, r0, #292
.L1112:
	ldrh	r3, [r4, #320]
	cbz	r3, .L1115
.L1119:
	ldrb	r3, [r4, #17]	@ zero_extendqisi2
	lsls	r0, r3, #31
	bpl	.L1117
	add	r5, r4, #324
.L1116:
	ldrh	r3, [r4, #352]
	cbnz	r3, .L1117
	ldr	r1, [r4, #324]
	mov	r0, r4
	ldr	r2, [r1, #8]
	bl	new_block_init_for_write
	cbnz	r0, .L1121
.L1117:
	movs	r0, #0
.L1111:
	pop	{r3, r4, r5, pc}
.L1115:
	ldr	r1, [r4, #292]
	mov	r0, r4
	ldr	r2, [r1, #8]
	bl	new_block_init_for_write
	cmp	r0, #0
	beq	.L1119
	ldr	r2, [r4, #292]
	mov	r0, r4
	ldr	r3, [r2, #8]
	subs	r1, r3, #1
	str	r1, [r2, #8]
	ldrb	r1, [r4, #17]	@ zero_extendqisi2
	lsls	r1, r1, #31
	mov	r1, r5
	itt	mi
	submi	r3, r3, #2
	strmi	r3, [r2, #8]
	bl	get_new_current_block
	cmp	r0, #0
	beq	.L1112
	ldr	r0, .L1134
.L1133:
	bl	NAND_Print
	movs	r0, #1
	b	.L1111
.L1121:
	ldr	r2, [r4, #324]
	mov	r0, r4
	ldr	r3, [r2, #8]
	subs	r1, r3, #1
	str	r1, [r2, #8]
	ldrb	r1, [r4, #17]	@ zero_extendqisi2
	lsls	r1, r1, #31
	mov	r1, r5
	itt	mi
	submi	r3, r3, #2
	strmi	r3, [r2, #8]
	bl	get_new_current_block
	cmp	r0, #0
	beq	.L1116
	ldr	r0, .L1134+4
	b	.L1133
.L1135:
	.align	2
.L1134:
	.word	.LC235
	.word	.LC236
	.size	do_before_write, .-do_before_write
	.section	.text.check_invalid_page_right,"ax",%progbits
	.align	1
	.global	check_invalid_page_right
	.syntax unified
	.thumb
	.thumb_func
	.fpu softvfp
	.type	check_invalid_page_right, %function
check_invalid_page_right:
	@ args = 0, pretend = 0, frame = 0
	@ frame_needed = 0, uses_anonymous_args = 0
	push	{r4, r5, r6, r7, r8, r10, fp, lr}
	movs	r4, #0
	ldr	fp, .L1145+4
	mov	r8, r0
	mov	r10, r1
	mov	r7, r2
	mov	r6, r3
	mov	r5, r4
.L1137:
	cmp	r6, r5
	bhi	.L1142
	ldrh	r2, [r7, #4]
	cmp	r2, r4
	beq	.L1144
	mov	r3, r6
	mov	r1, r4
	ldr	r0, .L1145
	bl	NAND_Print
	movs	r0, #1
.L1136:
	pop	{r4, r5, r6, r7, r8, r10, fp, pc}
.L1142:
	ldr	r3, [r10, #8]
	ldr	r1, [r3, r5, lsl #2]
	adds	r3, r1, #1
	beq	.L1138
	mov	r0, r8
	bl	get_logic_page_map
	cbnz	r0, .L1139
	mov	r0, fp
	bl	NAND_Print_DBG
.L1138:
	adds	r4, r4, #1
	b	.L1140
.L1139:
	ldrh	r2, [r0, #2]
	ldrh	r3, [r7]
	cmp	r2, r3
	bne	.L1138
	ldrh	r3, [r0]
	ubfx	r3, r3, #0, #12
	cmp	r3, r5
	bne	.L1138
.L1140:
	adds	r5, r5, #1
	b	.L1137
.L1144:
	movs	r0, #0
	b	.L1136
.L1146:
	.align	2
.L1145:
	.word	.LC238
	.word	.LC237
	.size	check_invalid_page_right, .-check_invalid_page_right
	.section	.text.write_phy_page_map_to_current_block,"ax",%progbits
	.align	1
	.global	write_phy_page_map_to_current_block
	.syntax unified
	.thumb
	.thumb_func
	.fpu softvfp
	.type	write_phy_page_map_to_current_block, %function
write_phy_page_map_to_current_block:
	@ args = 0, pretend = 0, frame = 0
	@ frame_needed = 0, uses_anonymous_args = 0
	add	r3, r0, #324
	push	{r0, r1, r4, r5, r6, lr}
	cmp	r1, r3
	mov	r4, r0
	mov	r5, r1
	bne	.L1148
	ldr	r1, [r0, #312]
	mov	r2, #320
	ldr	r0, [r0, #344]
	bl	NAND_Memcpy
.L1148:
	ldr	r3, .L1151
	mov	r1, r5
	ldr	r2, [r5, #4]
	mov	r0, r4
	str	r3, [sp]
	movs	r3, #0
	bl	write_phy_data_to_current_block
	mov	r6, r0
	cbz	r0, .L1149
	ldr	r0, .L1151+4
	bl	NAND_Print
.L1147:
	mov	r0, r6
	add	sp, sp, #8
	@ sp needed
	pop	{r4, r5, r6, pc}
.L1149:
	mov	r1, r5
	ldrh	r3, [r5, #28]
	ldr	r2, [r1], #4
	adds	r3, r3, #1
	ldrh	r0, [r2, #4]
	uxth	r3, r3
	strh	r3, [r5, #28]	@ movhi
	adds	r0, r0, #1
	strh	r0, [r2, #4]	@ movhi
	mov	r0, r4
	bl	check_invalid_page_right
	ldr	r3, [r4, #4]
	ldrh	r2, [r5, #28]
	ldrh	r3, [r3, #20]
	cmp	r2, r3
	bne	.L1147
	ldr	r1, [r5]
	mov	r0, r4
	bl	put_phy_block_to_invalid_page_list
	mov	r0, r4
	bl	adjust_invaild_list
	b	.L1147
.L1152:
	.align	2
.L1151:
	.word	-1431633921
	.word	.LC239
	.size	write_phy_page_map_to_current_block, .-write_phy_page_map_to_current_block
	.section	.text.nftl_ops_init,"ax",%progbits
	.align	1
	.global	nftl_ops_init
	.syntax unified
	.thumb
	.thumb_func
	.fpu softvfp
	.type	nftl_ops_init, %function
nftl_ops_init:
	@ args = 0, pretend = 0, frame = 0
	@ frame_needed = 0, uses_anonymous_args = 0
	@ link register save eliminated.
	ldr	r3, .L1154
	str	r3, [r0, #1976]
	ldr	r3, .L1154+4
	str	r3, [r0, #1980]
	ldr	r3, .L1154+8
	str	r3, [r0, #1984]
	ldr	r3, .L1154+12
	str	r3, [r0, #1992]
	ldr	r3, .L1154+16
	str	r3, [r0, #1996]
	ldr	r3, .L1154+20
	str	r3, [r0, #1988]
	ldr	r3, .L1154+24
	str	r3, [r0, #2004]
	ldr	r3, .L1154+28
	str	r3, [r0, #2008]
	ldr	r3, .L1154+32
	str	r3, [r0, #2000]
	ldr	r3, .L1154+36
	str	r3, [r0, #2012]
	bx	lr
.L1155:
	.align	2
.L1154:
	.word	_nftl_nand_erase_superblk
	.word	_nftl_nand_read_page
	.word	_nftl_nand_write_page
	.word	_nftl_nand_is_blk_good
	.word	_nftl_nand_mark_bad_blk
	.word	_nftl_nand_copy_page
	.word	_nftl_nand_read_logic_page
	.word	_nftl_nand_read_logic_history_page
	.word	_nftl_nand_write_logic_page
	.word	_nftl_discard_logic_page
	.size	nftl_ops_init, .-nftl_ops_init
	.section	.text.current_block_at_last_page,"ax",%progbits
	.align	1
	.global	current_block_at_last_page
	.syntax unified
	.thumb
	.thumb_func
	.fpu softvfp
	.type	current_block_at_last_page, %function
current_block_at_last_page:
	@ args = 0, pretend = 0, frame = 0
	@ frame_needed = 0, uses_anonymous_args = 0
	@ link register save eliminated.
	ldr	r3, [r0, #4]
	ldrh	r0, [r1, #28]
	ldrh	r3, [r3, #20]
	subs	r3, r3, #1
	subs	r3, r3, r0
	rsbs	r0, r3, #0
	adcs	r0, r0, r3
	bx	lr
	.size	current_block_at_last_page, .-current_block_at_last_page
	.section	.text.do_after_write,"ax",%progbits
	.align	1
	.global	do_after_write
	.syntax unified
	.thumb
	.thumb_func
	.fpu softvfp
	.type	do_after_write, %function
do_after_write:
	@ args = 0, pretend = 0, frame = 0
	@ frame_needed = 0, uses_anonymous_args = 0
	push	{r4, r5, r6, lr}
	add	r6, r0, #292
	mov	r1, r6
	mov	r4, r0
	bl	current_block_at_last_page
	cmp	r0, #1
	mov	r5, r0
	bne	.L1165
	mov	r0, r4
	bl	write_phy_page_map_to_current_block
	cbz	r0, .L1159
	ldr	r0, .L1176
.L1175:
	bl	NAND_Print
.L1157:
	mov	r0, r5
	pop	{r4, r5, r6, pc}
.L1159:
	ldrb	r3, [r4, #17]	@ zero_extendqisi2
	lsls	r2, r3, #31
	bpl	.L1161
	add	r1, r4, #324
	mov	r0, r4
	bl	write_phy_page_map_to_current_block
	cbz	r0, .L1161
	ldr	r0, .L1176
	bl	NAND_Print
.L1161:
	mov	r1, r6
	mov	r0, r4
	bl	get_new_current_block
	cbz	r0, .L1163
	ldr	r0, .L1176+4
	b	.L1175
.L1163:
	ldrb	r3, [r4, #17]	@ zero_extendqisi2
	lsls	r3, r3, #31
	bpl	.L1164
	add	r1, r4, #324
	mov	r0, r4
	bl	get_new_current_block
	cbz	r0, .L1164
	ldr	r0, .L1176+8
	b	.L1175
.L1164:
	ldr	r3, [r4, #368]
	adds	r3, r3, #1
	str	r3, [r4, #368]
.L1165:
	movs	r5, #0
	b	.L1157
.L1177:
	.align	2
.L1176:
	.word	.LC240
	.word	.LC241
	.word	.LC242
	.size	do_after_write, .-do_after_write
	.section	.text.nand_write_data_page,"ax",%progbits
	.align	1
	.global	nand_write_data_page
	.syntax unified
	.thumb
	.thumb_func
	.fpu softvfp
	.type	nand_write_data_page, %function
nand_write_data_page:
	@ args = 0, pretend = 0, frame = 0
	@ frame_needed = 0, uses_anonymous_args = 0
	push	{r4, r5, r6, r7, r8, lr}
	mov	r6, r1
	add	r1, r0, #292
	mov	r5, r0
	mov	r7, r2
	mov	r8, r3
	bl	current_block_at_last_page
	cmp	r0, #1
	mov	r4, r0
	bne	.L1179
	mov	r0, r5
	bl	do_after_write
	cbz	r0, .L1179
	ldr	r0, .L1190
	bl	NAND_Print
.L1178:
	mov	r0, r4
	pop	{r4, r5, r6, r7, r8, pc}
.L1179:
	mov	r0, r5
	bl	do_before_write
	cbz	r0, .L1181
	ldr	r0, .L1190+4
.L1189:
	bl	NAND_Print
	movs	r4, #1
	b	.L1178
.L1181:
	mov	r3, r8
	mov	r2, r7
	mov	r1, r6
	mov	r0, r5
	bl	write_data_to_page
	cbz	r0, .L1182
	ldr	r0, .L1190+8
	b	.L1189
.L1182:
	mov	r0, r5
	bl	do_after_write
	mov	r4, r0
	cmp	r0, #0
	beq	.L1178
	ldr	r0, .L1190+12
	b	.L1189
.L1191:
	.align	2
.L1190:
	.word	.LC243
	.word	.LC244
	.word	.LC245
	.word	.LC246
	.size	nand_write_data_page, .-nand_write_data_page
	.section	.text.nand_write_logic_page_no_gc,"ax",%progbits
	.align	1
	.global	nand_write_logic_page_no_gc
	.syntax unified
	.thumb
	.thumb_func
	.fpu softvfp
	.type	nand_write_logic_page_no_gc, %function
nand_write_logic_page_no_gc:
	@ args = 0, pretend = 0, frame = 0
	@ frame_needed = 0, uses_anonymous_args = 0
	@ link register save eliminated.
	mov	r3, r2
	mov	r2, r1
	movs	r1, #192
	b	nand_write_data_page
	.size	nand_write_logic_page_no_gc, .-nand_write_logic_page_no_gc
	.section	.text.current_block_at_first_page,"ax",%progbits
	.align	1
	.global	current_block_at_first_page
	.syntax unified
	.thumb
	.thumb_func
	.fpu softvfp
	.type	current_block_at_first_page, %function
current_block_at_first_page:
	@ args = 0, pretend = 0, frame = 0
	@ frame_needed = 0, uses_anonymous_args = 0
	@ link register save eliminated.
	ldrh	r0, [r1, #28]
	clz	r0, r0
	lsrs	r0, r0, #5
	bx	lr
	.size	current_block_at_first_page, .-current_block_at_first_page
	.section	.text.fill_no_use_data_to_page,"ax",%progbits
	.align	1
	.global	fill_no_use_data_to_page
	.syntax unified
	.thumb
	.thumb_func
	.fpu softvfp
	.type	fill_no_use_data_to_page, %function
fill_no_use_data_to_page:
	@ args = 0, pretend = 0, frame = 0
	@ frame_needed = 0, uses_anonymous_args = 0
	push	{r3, r4, r5, lr}
	add	r5, r0, #292
	mov	r1, r5
	mov	r4, r0
	bl	current_block_at_last_page
	cbz	r0, .L1195
	mov	r0, r4
	bl	write_phy_page_map_to_current_block
	cbz	r0, .L1196
	ldr	r0, .L1224
.L1222:
	bl	NAND_Print
	movs	r0, #1
	b	.L1194
.L1196:
	ldrb	r3, [r4, #17]	@ zero_extendqisi2
	lsls	r1, r3, #31
	bpl	.L1199
	add	r1, r4, #324
	mov	r0, r4
	bl	write_phy_page_map_to_current_block
	cbz	r0, .L1199
	ldr	r0, .L1224
	bl	NAND_Print
.L1199:
	mov	r1, r5
	mov	r0, r4
	bl	get_new_current_block
	cbz	r0, .L1201
	ldr	r0, .L1224+4
	b	.L1222
.L1201:
	ldrb	r3, [r4, #17]	@ zero_extendqisi2
	lsls	r2, r3, #31
	bmi	.L1202
.L1203:
	movs	r0, #0
.L1194:
	pop	{r3, r4, r5, pc}
.L1202:
	add	r1, r4, #324
	mov	r0, r4
	bl	get_new_current_block
	cmp	r0, #0
	beq	.L1203
	ldr	r0, .L1224+8
	b	.L1222
.L1195:
	ldrh	r3, [r4, #320]
	cmp	r3, #0
	beq	.L1203
	mov	r0, r4
	bl	write_phy_page_map_to_current_block
	cbz	r0, .L1204
	ldr	r0, .L1224
	bl	NAND_Print
.L1204:
	ldrb	r3, [r4, #17]	@ zero_extendqisi2
	lsls	r3, r3, #31
	bmi	.L1205
.L1223:
	movs	r0, #8
	b	.L1194
.L1205:
	add	r1, r4, #324
	mov	r0, r4
	bl	write_phy_page_map_to_current_block
	cmp	r0, #0
	beq	.L1223
	ldr	r0, .L1224
	bl	NAND_Print
	b	.L1223
.L1225:
	.align	2
.L1224:
	.word	.LC240
	.word	.LC241
	.word	.LC242
	.size	fill_no_use_data_to_page, .-fill_no_use_data_to_page
	.section	.text.fill_no_use_data_to_current_block,"ax",%progbits
	.align	1
	.global	fill_no_use_data_to_current_block
	.syntax unified
	.thumb
	.thumb_func
	.fpu softvfp
	.type	fill_no_use_data_to_current_block, %function
fill_no_use_data_to_current_block:
	@ args = 0, pretend = 0, frame = 0
	@ frame_needed = 0, uses_anonymous_args = 0
	push	{r3, r4, r5, lr}
	mov	r5, r0
	movs	r4, #0
.L1227:
	ldr	r3, [r5, #4]
	ldrh	r3, [r3, #20]
	cmp	r3, r4
	bgt	.L1229
	movs	r0, #1
.L1226:
	pop	{r3, r4, r5, pc}
.L1229:
	mov	r0, r5
	bl	fill_no_use_data_to_page
	cmp	r0, #0
	beq	.L1226
	adds	r4, r4, #1
	b	.L1227
	.size	fill_no_use_data_to_current_block, .-fill_no_use_data_to_current_block
	.section	.text.fill_no_use_page_to_current_block,"ax",%progbits
	.align	1
	.global	fill_no_use_page_to_current_block
	.syntax unified
	.thumb
	.thumb_func
	.fpu softvfp
	.type	fill_no_use_page_to_current_block, %function
fill_no_use_page_to_current_block:
	@ args = 0, pretend = 0, frame = 0
	@ frame_needed = 0, uses_anonymous_args = 0
	push	{r4, r5, r6, lr}
	mov	r6, r0
	mov	r5, r1
	movs	r4, #0
.L1234:
	cmp	r4, r5
	blt	.L1236
	movs	r0, #1
.L1233:
	pop	{r4, r5, r6, pc}
.L1236:
	mov	r0, r6
	bl	fill_no_use_data_to_page
	cmp	r0, #0
	beq	.L1233
	adds	r4, r4, #1
	b	.L1234
	.size	fill_no_use_page_to_current_block, .-fill_no_use_page_to_current_block
	.section	.text.nftl_write_end,"ax",%progbits
	.align	1
	.global	nftl_write_end
	.syntax unified
	.thumb
	.thumb_func
	.fpu softvfp
	.type	nftl_write_end, %function
nftl_write_end:
	@ args = 0, pretend = 0, frame = 0
	@ frame_needed = 0, uses_anonymous_args = 0
	ldr	r3, .L1243
	push	{r4, lr}
	ldr	r4, [r3, #8]
.L1241:
	cbnz	r4, .L1242
	mov	r0, r4
	pop	{r4, pc}
.L1242:
	ldr	r0, [r4, #12]
	movs	r1, #8
	bl	fill_no_use_page_to_current_block
	ldr	r4, [r4, #8]
	b	.L1241
.L1244:
	.align	2
.L1243:
	.word	.LANCHOR0
	.size	nftl_write_end, .-nftl_write_end
	.section	.text.nand_before_shutdown,"ax",%progbits
	.align	1
	.global	nand_before_shutdown
	.syntax unified
	.thumb
	.thumb_func
	.fpu softvfp
	.type	nand_before_shutdown, %function
nand_before_shutdown:
	@ args = 0, pretend = 0, frame = 0
	@ frame_needed = 0, uses_anonymous_args = 0
	@ link register save eliminated.
	movs	r1, #8
	b	fill_no_use_page_to_current_block
	.size	nand_before_shutdown, .-nand_before_shutdown
	.section	.text.nftl_cache_init,"ax",%progbits
	.align	1
	.global	nftl_cache_init
	.syntax unified
	.thumb
	.thumb_func
	.fpu softvfp
	.type	nftl_cache_init, %function
nftl_cache_init:
	@ args = 0, pretend = 0, frame = 0
	@ frame_needed = 0, uses_anonymous_args = 0
	ldr	r3, [r0, #1972]
	push	{r4, r5, r6, r7, r8, lr}
	mov	r4, r0
	mov	r6, r0
	movs	r7, #255
	mov	r8, #-1
	movs	r5, #0
	ldrh	r3, [r3, #22]
	strh	r5, [r0, #586]	@ movhi
	strh	r5, [r0, #588]	@ movhi
	strh	r3, [r0, #584]	@ movhi
	strh	r5, [r0, #590]	@ movhi
	str	r5, [r0, #1568]
	str	r5, [r0, #1572]
	str	r5, [r0, #1576]
	str	r5, [r0, #1580]
	str	r5, [r0, #1600]
	str	r5, [r0, #1604]
	str	r5, [r0, #1608]
	str	r5, [r0, #1612]
.L1247:
	ldr	r3, [r4, #4]
	ldrh	r0, [r3, #26]
	ldrh	r3, [r4, #584]
	cmp	r3, r5
	bhi	.L1250
	bl	nftl_malloc
	str	r0, [r4, #1616]
	cbnz	r0, .L1251
	ldr	r1, .L1253
	ldr	r0, .L1253+4
	b	.L1252
.L1250:
	movs	r3, #0
	strh	r5, [r6, #592]	@ movhi
	strh	r7, [r6, #594]	@ movhi
	str	r8, [r6, #600]
	strd	r3, r3, [r6, #608]
	strd	r3, r3, [r6, #616]
	adds	r6, r6, #32
	strh	r3, [r6, #564]	@ movhi
	strh	r3, [r6, #566]	@ movhi
	bl	nftl_malloc
	str	r0, [r6, #572]
	cbnz	r0, .L1248
	ldr	r1, .L1253
	ldr	r0, .L1253+8
.L1252:
	bl	NAND_Print
	movs	r0, #1
.L1246:
	pop	{r4, r5, r6, r7, r8, pc}
.L1248:
	adds	r5, r5, #1
	b	.L1247
.L1251:
	movs	r0, #0
	b	.L1246
.L1254:
	.align	2
.L1253:
	.word	.LANCHOR15
	.word	.LC248
	.word	.LC247
	.size	nftl_cache_init, .-nftl_cache_init
	.section	.text.nftl_cache_exit,"ax",%progbits
	.align	1
	.global	nftl_cache_exit
	.syntax unified
	.thumb
	.thumb_func
	.fpu softvfp
	.type	nftl_cache_exit, %function
nftl_cache_exit:
	@ args = 0, pretend = 0, frame = 0
	@ frame_needed = 0, uses_anonymous_args = 0
	push	{r3, r4, r5, lr}
	mov	r5, r0
	movs	r4, #0
.L1256:
	ldrh	r3, [r5, #584]
	cmp	r3, r4
	bgt	.L1257
	ldr	r0, [r5, #1616]
	bl	nftl_free
	movs	r0, #0
	pop	{r3, r4, r5, pc}
.L1257:
	add	r3, r5, r4, lsl #5
	adds	r4, r4, #1
	ldr	r0, [r3, #604]
	bl	nftl_free
	b	.L1256
	.size	nftl_cache_exit, .-nftl_cache_exit
	.section	.text.nftl_cache_clean,"ax",%progbits
	.align	1
	.global	nftl_cache_clean
	.syntax unified
	.thumb
	.thumb_func
	.fpu softvfp
	.type	nftl_cache_clean, %function
nftl_cache_clean:
	@ args = 0, pretend = 0, frame = 0
	@ frame_needed = 0, uses_anonymous_args = 0
	ldr	r3, [r0, #1972]
	add	r0, r0, #592
	push	{r4, r5, lr}
	movs	r4, #255
	mov	r5, #-1
	ldrh	r1, [r3, #22]
	movs	r3, #0
	strh	r3, [r0, #-6]	@ movhi
	mov	r2, r3
	strh	r3, [r0, #-4]	@ movhi
	strh	r1, [r0, #-8]	@ movhi
	strh	r3, [r0, #-2]	@ movhi
	str	r3, [r0, #976]
	str	r3, [r0, #980]
	str	r3, [r0, #984]
	str	r3, [r0, #988]
	str	r3, [r0, #1008]
	str	r3, [r0, #1012]
	str	r3, [r0, #1016]
	str	r3, [r0, #1020]
.L1259:
	cmp	r1, r3
	add	r0, r0, #32
	bhi	.L1260
	movs	r0, #0
	pop	{r4, r5, pc}
.L1260:
	strh	r3, [r0, #-32]	@ movhi
	adds	r3, r3, #1
	strh	r4, [r0, #-30]	@ movhi
	str	r5, [r0, #-24]
	strd	r2, r2, [r0, #-16]
	strd	r2, r2, [r0, #-8]
	strh	r2, [r0, #-28]	@ movhi
	strh	r2, [r0, #-26]	@ movhi
	b	.L1259
	.size	nftl_cache_clean, .-nftl_cache_clean
	.section	.text.search_from_cache_read_list,"ax",%progbits
	.align	1
	.global	search_from_cache_read_list
	.syntax unified
	.thumb
	.thumb_func
	.fpu softvfp
	.type	search_from_cache_read_list, %function
search_from_cache_read_list:
	@ args = 0, pretend = 0, frame = 0
	@ frame_needed = 0, uses_anonymous_args = 0
	@ link register save eliminated.
	ldr	r0, [r0, #984]
.L1262:
	cbnz	r0, .L1264
.L1261:
	bx	lr
.L1264:
	ldr	r3, [r0, #8]
	cmp	r3, r1
	beq	.L1261
	ldr	r0, [r0, #16]
	b	.L1262
	.size	search_from_cache_read_list, .-search_from_cache_read_list
	.section	.text.search_from_cache_write_list,"ax",%progbits
	.align	1
	.global	search_from_cache_write_list
	.syntax unified
	.thumb
	.thumb_func
	.fpu softvfp
	.type	search_from_cache_write_list, %function
search_from_cache_write_list:
	@ args = 0, pretend = 0, frame = 0
	@ frame_needed = 0, uses_anonymous_args = 0
	@ link register save eliminated.
	ldr	r0, [r0, #1024]
.L1266:
	cbnz	r0, .L1268
.L1265:
	bx	lr
.L1268:
	ldr	r3, [r0, #8]
	cmp	r3, r1
	beq	.L1265
	ldr	r0, [r0, #24]
	b	.L1266
	.size	search_from_cache_write_list, .-search_from_cache_write_list
	.section	.text.add_to_cache_read_list_tail,"ax",%progbits
	.align	1
	.global	add_to_cache_read_list_tail
	.syntax unified
	.thumb
	.thumb_func
	.fpu softvfp
	.type	add_to_cache_read_list_tail, %function
add_to_cache_read_list_tail:
	@ args = 0, pretend = 0, frame = 0
	@ frame_needed = 0, uses_anonymous_args = 0
	add	r3, r0, #968
	push	{r4, lr}
.L1270:
	ldr	r4, [r3, #16]
	cbnz	r4, .L1273
	str	r1, [r3, #16]
	strd	r4, r3, [r1, #16]
	strh	r2, [r1, #2]	@ movhi
	cbnz	r2, .L1271
	ldrh	r3, [r0, #2]
	adds	r3, r3, #1
	strh	r3, [r0, #2]	@ movhi
.L1272:
	movs	r0, #0
	pop	{r4, pc}
.L1273:
	mov	r3, r4
	b	.L1270
.L1271:
	ldrh	r3, [r0, #6]
	adds	r3, r3, #1
	strh	r3, [r0, #6]	@ movhi
	b	.L1272
	.size	add_to_cache_read_list_tail, .-add_to_cache_read_list_tail
	.section	.text.add_to_cache_write_list_tail,"ax",%progbits
	.align	1
	.global	add_to_cache_write_list_tail
	.syntax unified
	.thumb
	.thumb_func
	.fpu softvfp
	.type	add_to_cache_write_list_tail, %function
add_to_cache_write_list_tail:
	@ args = 0, pretend = 0, frame = 0
	@ frame_needed = 0, uses_anonymous_args = 0
	@ link register save eliminated.
	add	r2, r0, #1000
.L1275:
	ldr	r3, [r2, #24]
	cbnz	r3, .L1276
	str	r1, [r2, #24]
	strd	r3, r2, [r1, #24]
	movs	r2, #1
	strh	r2, [r1, #2]	@ movhi
	ldrh	r2, [r0, #4]
	adds	r2, r2, #1
	strh	r2, [r0, #4]	@ movhi
	mov	r0, r3
	bx	lr
.L1276:
	mov	r2, r3
	b	.L1275
	.size	add_to_cache_write_list_tail, .-add_to_cache_write_list_tail
	.section	.text.del_from_cache_read_list,"ax",%progbits
	.align	1
	.global	del_from_cache_read_list
	.syntax unified
	.thumb
	.thumb_func
	.fpu softvfp
	.type	del_from_cache_read_list, %function
del_from_cache_read_list:
	@ args = 0, pretend = 0, frame = 0
	@ frame_needed = 0, uses_anonymous_args = 0
	@ link register save eliminated.
	ldrd	r3, r2, [r1, #16]
	str	r3, [r2, #16]
	ldr	r3, [r1, #16]
	cbz	r3, .L1278
	str	r2, [r3, #20]
.L1278:
	ldrh	r3, [r1, #2]
	cbnz	r3, .L1279
	ldrh	r3, [r0, #2]
	subs	r3, r3, #1
	strh	r3, [r0, #2]	@ movhi
.L1280:
	movs	r3, #255
	mov	r0, r1
	strh	r3, [r1, #2]	@ movhi
	movs	r3, #0
	strd	r3, r3, [r1, #16]
	bx	lr
.L1279:
	ldrh	r3, [r0, #6]
	subs	r3, r3, #1
	strh	r3, [r0, #6]	@ movhi
	b	.L1280
	.size	del_from_cache_read_list, .-del_from_cache_read_list
	.section	.text.del_from_cache_write_list,"ax",%progbits
	.align	1
	.global	del_from_cache_write_list
	.syntax unified
	.thumb
	.thumb_func
	.fpu softvfp
	.type	del_from_cache_write_list, %function
del_from_cache_write_list:
	@ args = 0, pretend = 0, frame = 0
	@ frame_needed = 0, uses_anonymous_args = 0
	@ link register save eliminated.
	ldrd	r3, r2, [r1, #24]
	str	r3, [r2, #24]
	ldr	r3, [r1, #24]
	cbz	r3, .L1285
	str	r2, [r3, #28]
.L1285:
	movs	r3, #0
	strd	r3, r3, [r1, #24]
	movs	r3, #255
	strh	r3, [r1, #2]	@ movhi
	ldrh	r3, [r0, #4]
	subs	r3, r3, #1
	strh	r3, [r0, #4]	@ movhi
	mov	r0, r1
	bx	lr
	.size	del_from_cache_write_list, .-del_from_cache_write_list
	.section	.text.get_empty_cahce_node,"ax",%progbits
	.align	1
	.global	get_empty_cahce_node
	.syntax unified
	.thumb
	.thumb_func
	.fpu softvfp
	.type	get_empty_cahce_node, %function
get_empty_cahce_node:
	@ args = 0, pretend = 0, frame = 0
	@ frame_needed = 0, uses_anonymous_args = 0
	@ link register save eliminated.
	ldrh	r2, [r0]
	movs	r1, #0
	push	{r4}
.L1290:
	uxth	r3, r1
	cmp	r2, r3
	bhi	.L1292
	ldrh	r3, [r0, #6]
	ldr	r1, [r0, #984]
	cmp	r3, #9
	bhi	.L1293
.L1294:
	cbz	r1, .L1289
	ldrh	r3, [r1, #2]
	cbz	r3, .L1301
	ldr	r1, [r1, #16]
	b	.L1294
.L1292:
	adds	r3, r1, #1
	add	r4, r0, r3, lsl #5
	ldrh	r4, [r4, #-22]
	cmp	r4, #255
	bne	.L1299
	add	r1, r0, r1, lsl #5
	adds	r1, r1, #8
.L1289:
	mov	r0, r1
	pop	{r4}
	bx	lr
.L1299:
	mov	r1, r3
	b	.L1290
.L1295:
	ldr	r1, [r1, #16]
.L1293:
	cmp	r1, #0
	beq	.L1289
	ldrh	r3, [r1, #2]
	cmp	r3, #2
	bne	.L1295
.L1301:
	pop	{r4}
	b	del_from_cache_read_list
	.size	get_empty_cahce_node, .-get_empty_cahce_node
	.section	.text.nand_cache_read,"ax",%progbits
	.align	1
	.global	nand_cache_read
	.syntax unified
	.thumb
	.thumb_func
	.fpu softvfp
	.type	nand_cache_read, %function
nand_cache_read:
	@ args = 0, pretend = 0, frame = 16
	@ frame_needed = 0, uses_anonymous_args = 0
	push	{r0, r1, r2, r3, r4, r5, r6, r7, r8, r10, fp, lr}
	mov	r4, r0
	str	r1, [sp, #4]
	mov	r7, r3
	cbnz	r3, .L1303
	ldr	r0, .L1313
	bl	NAND_Print
.L1311:
	mov	fp, #0
.L1302:
	mov	r0, fp
	add	sp, sp, #16
	@ sp needed
	pop	{r4, r5, r6, r7, r8, r10, fp, pc}
.L1303:
	lsrs	r3, r2, #8
	uxtb	r2, r2
	lsl	r8, r3, #9
	ldr	r3, [r0, #1972]
	lsls	r5, r2, #9
	ldrh	r3, [r3]
	str	r3, [sp, #12]
	cbz	r3, .L1305
	ldr	r6, [r0, #604]
	ldr	r3, [r0, #2004]
	mov	r2, r6
	blx	r3
	mov	r2, r5
	mov	fp, r0
	add	r1, r6, r8
	mov	r0, r7
	bl	NAND_Memcpy
	cmp	fp, #10
	bne	.L1302
	ldr	r2, [r4, #1968]
	mov	r0, r4
	ldr	r1, [sp, #4]
	ldr	r3, [r2, #116]
	adds	r3, r3, #1
	str	r3, [r2, #116]
	mov	r2, r6
	ldr	r3, [r4, #2000]
	blx	r3
	b	.L1311
.L1305:
	add	r10, r0, #584
	mov	r0, r10
	bl	search_from_cache_write_list
	mov	fp, r0
	cbz	r0, .L1306
	ldr	r3, [r0, #12]
	ldrh	r6, [r0, #6]
	str	r3, [sp, #8]
	ldrh	r3, [r0, #4]
	lsls	r6, r6, #9
	ldr	r2, [sp, #8]
	uxth	r6, r6
	lsls	r3, r3, #9
	add	r10, r2, r8
	uxth	r3, r3
	add	r3, r3, r6
	cmp	r3, r8
	bcc	.L1307
	add	r2, r8, r5
	cmp	r3, r2
	bcc	.L1307
.L1312:
	mov	r2, r5
	mov	r1, r10
	mov	r0, r7
	bl	NAND_Memcpy
	b	.L1311
.L1307:
	ldr	r3, [r4, #2004]
	mov	r0, r4
	ldr	r2, [r4, #1616]
	ldr	r1, [sp, #4]
	blx	r3
	ldr	r3, [r4, #4]
	ldr	r1, [r4, #1616]
	ldrh	r2, [r3, #26]
	ldr	r3, [sp, #8]
	add	r1, r1, r6
	subs	r2, r2, r6
	add	r3, r3, r6
	mov	r0, r3
	bl	NAND_Memcpy
	ldr	r3, [r4, #4]
	ldrb	r3, [r3, #8]	@ zero_extendqisi2
	strh	r3, [fp, #6]	@ movhi
	b	.L1312
.L1306:
	ldr	r1, [sp, #4]
	mov	r0, r10
	bl	search_from_cache_read_list
	mov	r6, r0
	cbz	r0, .L1308
	ldrh	r4, [r0, #2]
	cbnz	r4, .L1309
	mov	r1, r0
	mov	r0, r10
	bl	del_from_cache_read_list
	ldr	r1, [r6, #12]
	mov	r2, r5
	mov	r0, r7
	add	r1, r1, r8
	bl	NAND_Memcpy
	mov	r2, r4
	mov	r1, r6
	mov	r0, r10
	bl	add_to_cache_read_list_tail
	b	.L1311
.L1309:
	ldr	r1, [r0, #12]
	mov	r2, r5
	mov	r0, r7
	add	r1, r1, r8
	bl	NAND_Memcpy
	b	.L1302
.L1308:
	mov	r0, r10
	bl	get_empty_cahce_node
	cbnz	r0, .L1310
	ldrh	r3, [r4, #590]
	mov	fp, #1
	ldrh	r2, [r4, #586]
	ldrh	r1, [r4, #588]
	ldr	r0, .L1313+4
	bl	NAND_Print
	b	.L1302
.L1310:
	ldr	r2, [sp, #4]
	ldr	r1, [sp, #4]
	ldr	r3, [r4, #2004]
	str	r2, [r0, #8]
	ldr	r2, [r0, #12]
	strd	r2, r0, [sp, #8]
	mov	r0, r4
	blx	r3
	ldr	r3, [sp, #12]
	mov	fp, r0
	ldr	r2, [r4, #4]
	mov	r0, r10
	strh	r6, [r3, #4]	@ movhi
	mov	r1, r3
	ldrb	r2, [r2, #8]	@ zero_extendqisi2
	strh	r2, [r3, #6]	@ movhi
	mov	r2, r6
	bl	add_to_cache_read_list_tail
	ldr	r3, [sp, #8]
	mov	r2, r5
	mov	r0, r7
	add	r1, r3, r8
	bl	NAND_Memcpy
	cmp	fp, #10
	bne	.L1302
	ldr	r2, [r4, #1968]
	mov	r0, r4
	mov	fp, r6
	ldr	r3, [r2, #116]
	adds	r3, r3, #1
	str	r3, [r2, #116]
	ldr	r3, [r4, #2000]
	ldrd	r1, r2, [sp, #4]
	blx	r3
	b	.L1302
.L1314:
	.align	2
.L1313:
	.word	.LC249
	.word	.LC250
	.size	nand_cache_read, .-nand_cache_read
	.global	__aeabi_uidivmod
	.section	.text.nand_op,"ax",%progbits
	.align	1
	.global	nand_op
	.syntax unified
	.thumb
	.thumb_func
	.fpu softvfp
	.type	nand_op, %function
nand_op:
	@ args = 4, pretend = 0, frame = 0
	@ frame_needed = 0, uses_anonymous_args = 0
	push	{r4, r5, r6, r7, r8, r10, fp, lr}
	mov	r6, r1
	ldr	r10, [sp, #32]
	mov	r4, r3
	cbz	r3, .L1316
	ldr	r1, [r1, #24]
	add	r3, r3, r2
	cmp	r3, r1
	bls	.L1317
	movs	r4, #1
	ldr	r0, .L1330
	bl	NAND_Print
.L1316:
	mov	r0, r4
	pop	{r4, r5, r6, r7, r8, r10, fp, pc}
.L1317:
	ldr	r1, .L1330+4
	ldr	r3, .L1330+8
	cmp	r0, #0
	ite	ne
	movne	r8, r1
	moveq	r8, r3
	ldr	r3, [r6, #4]
	mov	r0, r2
	ldrb	r5, [r3, #8]	@ zero_extendqisi2
	mov	r1, r5
	bl	__aeabi_uidivmod
	subs	r5, r5, r1
	mov	fp, r0
	uxth	r5, r5
	mov	r3, r10
	cmp	r4, r5
	itet	ls
	uxthls	r5, r4
	subhi	r7, r4, r5
	movls	r7, #0
	orr	r2, r5, r1, lsl #8
	mov	r1, r0
	mov	r0, r6
	uxth	r2, r2
	blx	r8
	mov	r4, r0
	cmp	r7, #0
	beq	.L1316
.L1321:
	ldr	r3, [r6, #4]
	add	fp, fp, #1
	add	r10, r10, r5, lsl #9
	ldrb	r3, [r3, #8]	@ zero_extendqisi2
	cmp	r7, r3
	bhi	.L1322
	mov	r3, r10
	uxth	r2, r7
	mov	r1, fp
	mov	r0, r6
	blx	r8
	orrs	r4, r4, r0
	b	.L1316
.L1322:
	uxth	r5, r3
	subs	r7, r7, r3
	mov	r1, fp
	mov	r3, r10
	mov	r2, r5
	mov	r0, r6
	blx	r8
	orrs	r4, r4, r0
	b	.L1321
.L1331:
	.align	2
.L1330:
	.word	.LC62
	.word	nand_cache_write
	.word	nand_cache_read
	.size	nand_op, .-nand_op
	.section	.text.__nand_read,"ax",%progbits
	.align	1
	.global	__nand_read
	.syntax unified
	.thumb
	.thumb_func
	.fpu softvfp
	.type	__nand_read, %function
__nand_read:
	@ args = 0, pretend = 0, frame = 0
	@ frame_needed = 0, uses_anonymous_args = 0
	push	{r0, r1, r4, r5, r6, r7, fp, lr}
	mov	r7, r3
	ldr	r3, [r0, #356]
	mov	r4, r0
	mov	r6, r1
	mov	r5, r2
	cmp	r3, #3
	bne	.L1333
	ldr	r0, .L1334
	bl	NAND_Print_DBG
.L1333:
	ldr	r1, [r4, #1968]
	movs	r0, #0
	ldrd	r2, [r1, #64]
	adds	fp, r2, r5
	mov	r2, r6
	adc	ip, r3, #0
	mov	r3, r5
	strd	fp, [r1, #64]
	mov	r1, r4
	str	r7, [sp]
	bl	nand_op
	add	sp, sp, #8
	@ sp needed
	pop	{r4, r5, r6, r7, fp, pc}
.L1335:
	.align	2
.L1334:
	.word	.LC251
	.size	__nand_read, .-__nand_read
	.section	.text._nand_read,"ax",%progbits
	.align	1
	.global	_nand_read
	.syntax unified
	.thumb
	.thumb_func
	.fpu softvfp
	.type	_nand_read, %function
_nand_read:
	@ args = 0, pretend = 0, frame = 0
	@ frame_needed = 0, uses_anonymous_args = 0
	@ link register save eliminated.
	ldr	r0, [r0, #12]
	b	__nand_read
	.size	_nand_read, .-_nand_read
	.section	.text.nand_sector_cache_read,"ax",%progbits
	.align	1
	.global	nand_sector_cache_read
	.syntax unified
	.thumb
	.thumb_func
	.fpu softvfp
	.type	nand_sector_cache_read, %function
nand_sector_cache_read:
	@ args = 0, pretend = 0, frame = 0
	@ frame_needed = 0, uses_anonymous_args = 0
	push	{r4, r5, r6, lr}
	mov	r4, r0
	addw	r0, r0, #1620
	mov	r5, r2
	bl	search_from_sector_cache_write_list
	cbz	r0, .L1338
	ldr	r1, [r0, #8]
	mov	r2, #512
	mov	r0, r5
	bl	NAND_Memcpy
	movs	r0, #0
.L1337:
	pop	{r4, r5, r6, pc}
.L1338:
	mov	r3, r2
	mov	r0, r4
	movs	r2, #1
	bl	__nand_read
	cmp	r0, #0
	beq	.L1337
	ldr	r0, .L1343
	bl	NAND_Print
	movs	r0, #1
	b	.L1337
.L1344:
	.align	2
.L1343:
	.word	.LC252
	.size	nand_sector_cache_read, .-nand_sector_cache_read
	.section	.text.__nand_sector_read,"ax",%progbits
	.align	1
	.global	__nand_sector_read
	.syntax unified
	.thumb
	.thumb_func
	.fpu softvfp
	.type	__nand_sector_read, %function
__nand_sector_read:
	@ args = 0, pretend = 0, frame = 0
	@ frame_needed = 0, uses_anonymous_args = 0
	push	{r4, r5, r6, r7, r8, lr}
	mov	r8, r0
	mov	r4, r1
	mov	r6, r3
	adds	r7, r1, r2
	movs	r5, #0
.L1346:
	cmp	r4, r7
	bne	.L1347
	mov	r0, r5
	pop	{r4, r5, r6, r7, r8, pc}
.L1347:
	mov	r2, r6
	mov	r1, r4
	mov	r0, r8
	adds	r4, r4, #1
	bl	nand_sector_cache_read
	add	r6, r6, #512
	orrs	r5, r5, r0
	b	.L1346
	.size	__nand_sector_read, .-__nand_sector_read
	.section	.text.__nand_write,"ax",%progbits
	.align	1
	.global	__nand_write
	.syntax unified
	.thumb
	.thumb_func
	.fpu softvfp
	.type	__nand_write, %function
__nand_write:
	@ args = 0, pretend = 0, frame = 0
	@ frame_needed = 0, uses_anonymous_args = 0
	push	{r0, r1, r4, r5, r6, r7, fp, lr}
	mov	r7, r3
	ldr	r3, [r0, #356]
	mov	r4, r0
	mov	r6, r1
	mov	r5, r2
	cmp	r3, #3
	bne	.L1349
	ldr	r0, .L1350
	bl	NAND_Print_DBG
.L1349:
	ldr	r1, [r4, #1968]
	movs	r0, #1
	ldrd	r2, [r1, #72]
	adds	fp, r2, r5
	mov	r2, r6
	adc	ip, r3, #0
	mov	r3, r5
	strd	fp, [r1, #72]
	mov	r1, r4
	str	r7, [sp]
	bl	nand_op
	add	sp, sp, #8
	@ sp needed
	pop	{r4, r5, r6, r7, fp, pc}
.L1351:
	.align	2
.L1350:
	.word	.LC253
	.size	__nand_write, .-__nand_write
	.section	.text._nand_write,"ax",%progbits
	.align	1
	.global	_nand_write
	.syntax unified
	.thumb
	.thumb_func
	.fpu softvfp
	.type	_nand_write, %function
_nand_write:
	@ args = 0, pretend = 0, frame = 0
	@ frame_needed = 0, uses_anonymous_args = 0
	@ link register save eliminated.
	ldr	r0, [r0, #12]
	b	__nand_write
	.size	_nand_write, .-_nand_write
	.section	.text.write_data_to_nand,"ax",%progbits
	.align	1
	.global	write_data_to_nand
	.syntax unified
	.thumb
	.thumb_func
	.fpu softvfp
	.type	write_data_to_nand, %function
write_data_to_nand:
	@ args = 0, pretend = 0, frame = 0
	@ frame_needed = 0, uses_anonymous_args = 0
	push	{r4, r5, r6, lr}
	mov	r6, r0
	mov	r0, #512
	bl	NAND_Malloc
	mov	r4, r0
	cbnz	r0, .L1354
	ldr	r1, .L1357
	ldr	r0, .L1357+4
	bl	NAND_Print
.L1354:
	movs	r5, #0
	mov	r2, #512
	movs	r1, #170
	mov	r0, r4
	bl	NAND_Memset
.L1355:
	mov	r1, r5
	mov	r3, r4
	movs	r2, #1
	mov	r0, r6
	adds	r5, r5, #1
	bl	__nand_write
	cmp	r5, #38912
	bne	.L1355
	mov	r0, r4
	bl	nftl_free
	movs	r0, #0
	pop	{r4, r5, r6, pc}
.L1358:
	.align	2
.L1357:
	.word	.LANCHOR16
	.word	.LC254
	.size	write_data_to_nand, .-write_data_to_nand
	.section	.text.nand_sector_cache_write,"ax",%progbits
	.align	1
	.global	nand_sector_cache_write
	.syntax unified
	.thumb
	.thumb_func
	.fpu softvfp
	.type	nand_sector_cache_write, %function
nand_sector_cache_write:
	@ args = 0, pretend = 0, frame = 0
	@ frame_needed = 0, uses_anonymous_args = 0
	push	{r4, r5, r6, r7, r8, lr}
	addw	r5, r0, #1620
	mov	r6, r0
	mov	r0, r5
	mov	r8, r1
	mov	r7, r2
	bl	search_from_sector_cache_write_list
	mov	r4, r0
	cbz	r0, .L1360
	mov	r1, r0
	mov	r0, r5
	bl	del_from_sector_cache_write_list
.L1368:
	mov	r2, #512
	mov	r1, r7
	ldr	r0, [r4, #8]
	bl	NAND_Memcpy
	mov	r1, r4
	mov	r0, r5
	bl	add_to_sector_cache_write_list_tail
	movs	r0, #0
	b	.L1359
.L1360:
	ldrh	r3, [r6, #1622]
	cmp	r3, #7
	bls	.L1362
	ldr	r1, [r6, #1960]
	mov	r0, r5
	bl	del_from_sector_cache_write_list
	movs	r2, #1
	mov	r4, r0
	ldr	r3, [r0, #8]
	ldr	r1, [r0, #4]
	mov	r0, r6
	bl	__nand_write
	cbz	r0, .L1363
	ldr	r0, .L1369
.L1367:
	bl	NAND_Print
	movs	r0, #1
.L1359:
	pop	{r4, r5, r6, r7, r8, pc}
.L1362:
	mov	r0, r5
	bl	get_empty_sector_cahce_node
	mov	r4, r0
	cbnz	r0, .L1363
	ldr	r0, .L1369+4
	b	.L1367
.L1363:
	str	r8, [r4, #4]
	b	.L1368
.L1370:
	.align	2
.L1369:
	.word	.LC255
	.word	.LC256
	.size	nand_sector_cache_write, .-nand_sector_cache_write
	.section	.text.__nand_sector_write,"ax",%progbits
	.align	1
	.global	__nand_sector_write
	.syntax unified
	.thumb
	.thumb_func
	.fpu softvfp
	.type	__nand_sector_write, %function
__nand_sector_write:
	@ args = 0, pretend = 0, frame = 0
	@ frame_needed = 0, uses_anonymous_args = 0
	push	{r4, r5, r6, r7, r8, lr}
	mov	r8, r0
	mov	r4, r1
	mov	r6, r3
	adds	r7, r1, r2
	movs	r5, #0
.L1372:
	cmp	r4, r7
	bne	.L1373
	mov	r0, r5
	pop	{r4, r5, r6, r7, r8, pc}
.L1373:
	mov	r2, r6
	mov	r1, r4
	mov	r0, r8
	adds	r4, r4, #1
	bl	nand_sector_cache_write
	add	r6, r6, #512
	orrs	r5, r5, r0
	b	.L1372
	.size	__nand_sector_write, .-__nand_sector_write
	.section	.text.__nand_flush_sector_write_cache,"ax",%progbits
	.align	1
	.global	__nand_flush_sector_write_cache
	.syntax unified
	.thumb
	.thumb_func
	.fpu softvfp
	.type	__nand_flush_sector_write_cache, %function
__nand_flush_sector_write_cache:
	@ args = 0, pretend = 0, frame = 0
	@ frame_needed = 0, uses_anonymous_args = 0
	push	{r3, r4, r5, r6, r7, lr}
	mov	r5, r0
	mov	r6, r1
	addw	r7, r0, #1620
	movs	r4, #0
.L1375:
	ldr	r1, [r5, #1960]
	cbz	r1, .L1376
	mov	r0, r7
	bl	del_from_sector_cache_write_list
	movs	r2, #1
	ldr	r3, [r0, #8]
	ldr	r1, [r0, #4]
	mov	r0, r5
	bl	__nand_write
	subs	r6, r6, #1
	mov	r4, r0
	bne	.L1375
.L1376:
	cbz	r4, .L1374
	ldr	r0, .L1385
	bl	NAND_Print
.L1374:
	mov	r0, r4
	pop	{r3, r4, r5, r6, r7, pc}
.L1386:
	.align	2
.L1385:
	.word	.LC257
	.size	__nand_flush_sector_write_cache, .-__nand_flush_sector_write_cache
	.section	.text.flush_write_cache_to_nand,"ax",%progbits
	.align	1
	.global	flush_write_cache_to_nand
	.syntax unified
	.thumb
	.thumb_func
	.fpu softvfp
	.type	flush_write_cache_to_nand, %function
flush_write_cache_to_nand:
	@ args = 0, pretend = 0, frame = 16
	@ frame_needed = 0, uses_anonymous_args = 0
	push	{r4, r5, r6, r7, r8, r10, lr}
	sub	sp, sp, #20
	mov	r4, r0
	movs	r2, #8
	mov	r8, r1
	mov	r0, sp
	ldr	r1, .L1414
	bl	memcpy
	movs	r2, #8
	ldr	r1, .L1414+4
	add	r0, sp, r2
	bl	memcpy
	ldr	r1, [r4, #1608]
	cbnz	r1, .L1388
	ldr	r0, .L1414+8
	bl	NAND_Print
.L1389:
	movs	r0, #0
	add	sp, sp, #20
	@ sp needed
	pop	{r4, r5, r6, r7, r8, r10, pc}
.L1388:
	add	r7, r4, #584
	mov	r0, r7
	bl	del_from_cache_write_list
	mov	r5, r0
	cmp	r0, #0
	beq	.L1389
	ldr	r3, [r4, #4]
	ldrh	r2, [r0, #6]
	ldrd	r1, r6, [r0, #8]
	ldrb	r3, [r3, #8]	@ zero_extendqisi2
	cmp	r2, r3
	beq	.L1391
	ldr	r3, [r4, #2004]
	mov	r0, r4
	ldr	r2, [r4, #1616]
	blx	r3
	mov	r10, r0
	ldrh	r0, [r5, #6]
	ldr	r3, [r4, #4]
	ldr	r1, [r4, #1616]
	lsls	r0, r0, #9
	ldrh	r2, [r3, #26]
	uxth	r0, r0
	subs	r2, r2, r0
	add	r1, r1, r0
	add	r0, r0, r6
	bl	NAND_Memcpy
	cmp	r10, #10
	bne	.L1391
	ldrh	r2, [r4, #590]
	ldr	r1, [r5, #8]
	ldr	r0, .L1414+12
	bl	NAND_Print
.L1391:
	movs	r3, #0
	mov	r2, r6
	strh	r3, [r5, #4]	@ movhi
	mov	r0, r4
	ldr	r3, [r4, #4]
	ldr	r1, [r5, #8]
	ldrb	r3, [r3, #8]	@ zero_extendqisi2
	strh	r3, [r5, #6]	@ movhi
	ldr	r3, [r4, #2000]
	blx	r3
	cbz	r0, .L1393
	ldr	r0, .L1414+16
	bl	NAND_Print
.L1393:
	ldr	r10, [r4, #4]
	movs	r2, #8
	mov	r1, sp
	mov	r0, r10
	bl	NAND_Memcmp
	tst	r0, #255
	bne	.L1394
.L1397:
	cmp	r8, #1
	bne	.L1398
	ldrh	r1, [r4, #320]
	ldr	r0, .L1414+20
	bl	NAND_Print
	ldrh	r0, [r4, #320]
	cbz	r0, .L1398
	subs	r0, r0, #1
	bl	nand_phy_get_page_type
	cmp	r0, #2
	bne	.L1398
	ldr	r0, .L1414+24
	bl	NAND_Print
	ldr	r3, [r4, #2000]
	mov	r2, r6
	ldr	r1, [r5, #8]
	mov	r0, r4
	blx	r3
	cbz	r0, .L1398
	ldr	r0, .L1414+16
	bl	NAND_Print
	b	.L1398
.L1394:
	movs	r2, #8
	mov	r0, r10
	add	r1, sp, r2
	bl	NAND_Memcmp
	tst	r0, #255
	beq	.L1397
.L1398:
	movs	r2, #2
	mov	r1, r5
	mov	r0, r7
	bl	add_to_cache_read_list_tail
	b	.L1389
.L1415:
	.align	2
.L1414:
	.word	.LANCHOR17
	.word	.LANCHOR17+8
	.word	.LC258
	.word	.LC259
	.word	.LC260
	.word	.LC261
	.word	.LC262
	.size	flush_write_cache_to_nand, .-flush_write_cache_to_nand
	.section	.text.nand_cache_write,"ax",%progbits
	.align	1
	.global	nand_cache_write
	.syntax unified
	.thumb
	.thumb_func
	.fpu softvfp
	.type	nand_cache_write, %function
nand_cache_write:
	@ args = 0, pretend = 0, frame = 16
	@ frame_needed = 0, uses_anonymous_args = 0
	push	{r0, r1, r2, r3, r4, r5, r6, r7, r8, r10, fp, lr}
	mov	r4, r0
	mov	r6, r1
	str	r3, [sp, #4]
	cbnz	r3, .L1417
.L1455:
	movs	r0, #0
	b	.L1416
.L1417:
	lsls	r3, r2, #1
	lsl	r8, r2, #9
	and	r10, r3, #65024
	ldr	r3, [r0, #1972]
	uxth	r8, r8
	ldrh	r5, [r3]
	cbz	r5, .L1419
	ldr	r5, [r0, #604]
	ldr	r3, [r0, #2004]
	mov	r2, r5
	blx	r3
	mov	r2, r8
	ldr	r1, [sp, #4]
	add	r0, r5, r10
	bl	NAND_Memcpy
	ldr	r3, [r4, #2000]
	mov	r2, r5
	mov	r1, r6
	mov	r0, r4
	add	sp, sp, #16
	@ sp needed
	pop	{r4, r5, r6, r7, r8, r10, fp, lr}
	bx	r3
.L1419:
	add	r5, r0, #584
	ldrh	r0, [r3, #2]
	cbnz	r0, .L1420
	ldr	r0, [r4, #4]
	ldrb	r7, [r0, #8]	@ zero_extendqisi2
	cmp	r7, r2
	bne	.L1420
	ldrh	r3, [r3, #26]
	cbnz	r3, .L1420
	mov	r0, r5
	bl	search_from_cache_write_list
	mov	r1, r0
	cbz	r0, .L1421
	mov	r0, r5
	bl	del_from_cache_write_list
.L1421:
	mov	r1, r6
	mov	r0, r5
	bl	search_from_cache_read_list
	mov	r1, r0
	cbz	r0, .L1422
	mov	r0, r5
	bl	del_from_cache_read_list
.L1422:
	ldr	r3, [r4, #2000]
	mov	r1, r6
	ldr	r2, [sp, #4]
	mov	r0, r4
	blx	r3
	cmp	r0, #0
	beq	.L1455
	mov	r1, r7
	ldr	r0, .L1457
	bl	NAND_Print
.L1454:
	movs	r0, #1
.L1416:
	add	sp, sp, #16
	@ sp needed
	pop	{r4, r5, r6, r7, r8, r10, fp, pc}
.L1420:
	mov	r1, r6
	mov	r0, r5
	bl	search_from_cache_write_list
	mov	r7, r0
	cmp	r0, #0
	beq	.L1424
	mov	r1, r0
	mov	r0, r5
	bl	del_from_cache_write_list
	ldr	r3, [r7, #12]
	ldr	r2, [r4, #4]
	str	r3, [sp, #8]
	ldrb	r2, [r2, #8]	@ zero_extendqisi2
	ldrh	r3, [r7, #6]
	cmp	r2, r3
	beq	.L1426
	lsl	fp, r3, #9
	uxth	fp, fp
	cmp	r10, fp
	bne	.L1427
	add	r3, r3, r8, lsr #9
	strh	r3, [r7, #6]	@ movhi
.L1426:
	ldrd	r1, r3, [sp, #4]
	mov	r2, r8
	add	r3, r3, r10
	mov	r0, r3
	bl	NAND_Memcpy
	mov	r1, r7
	mov	r0, r5
	bl	add_to_cache_write_list_tail
	mov	r1, r6
	mov	r0, r5
	bl	search_from_cache_read_list
	mov	r1, r0
	cmp	r0, #0
	beq	.L1455
	mov	r0, r5
	bl	del_from_cache_read_list
	b	.L1455
.L1427:
	ldr	r3, [r4, #2004]
	mov	r1, r6
	ldr	r2, [r4, #1616]
	mov	r0, r4
	blx	r3
	ldr	r2, [r4, #4]
	ldr	r3, [sp, #8]
	ldr	r1, [r4, #1616]
	ldrh	r2, [r2, #26]
	str	r0, [sp, #12]
	add	r0, r3, fp
	add	r1, r1, fp
	sub	r2, r2, fp
	bl	NAND_Memcpy
	ldr	r2, [r4, #4]
	ldr	r3, [sp, #12]
	ldrb	r2, [r2, #8]	@ zero_extendqisi2
	cmp	r3, #10
	strh	r2, [r7, #6]	@ movhi
	bne	.L1426
	mov	r1, r6
	ldr	r0, .L1457+4
	bl	NAND_Print
	b	.L1426
.L1424:
	mov	r0, r5
	bl	search_from_cache_read_list
	mov	fp, r0
	cbz	r0, .L1429
	ldr	r0, [r0, #12]
	mov	r2, r8
	ldr	r1, [sp, #4]
	add	r0, r0, r10
	bl	NAND_Memcpy
	ldr	r3, [r4, #4]
	mov	r1, fp
	strh	r7, [fp, #4]	@ movhi
	mov	r0, r5
	ldrb	r3, [r3, #8]	@ zero_extendqisi2
	strh	r3, [fp, #6]	@ movhi
	bl	del_from_cache_read_list
	mov	r0, r5
	bl	add_to_cache_write_list_tail
	ldr	r3, [r4, #1972]
	ldrh	r2, [r4, #588]
	ldrh	r3, [r3, #24]
	cmp	r2, r3
	bcc	.L1455
	mov	r1, r7
	mov	r0, r4
	bl	flush_write_cache_to_nand
	cmp	r0, #0
	beq	.L1455
	ldr	r0, .L1457+8
.L1456:
	bl	NAND_Print
	b	.L1454
.L1430:
	movs	r1, #0
	mov	r0, r4
	bl	flush_write_cache_to_nand
	cbz	r0, .L1429
	ldr	r0, .L1457+12
	b	.L1456
.L1429:
	ldr	r3, [r4, #1972]
	ldrh	r2, [r4, #588]
	ldrh	r3, [r3, #24]
	cmp	r2, r3
	bcs	.L1430
	mov	r0, r5
	bl	get_empty_cahce_node
	mov	r7, r0
	cbnz	r0, .L1431
	ldrh	r3, [r4, #590]
	ldrh	r2, [r4, #586]
	ldrh	r1, [r4, #588]
	ldr	r0, .L1457+16
	bl	NAND_Print
	b	.L1454
.L1431:
	str	r6, [r0, #8]
	ldr	fp, [r0, #12]
	cmp	r10, #0
	bne	.L1432
	lsr	r3, r8, #9
	strh	r10, [r0, #4]	@ movhi
.L1453:
	ldr	r1, [sp, #4]
	add	r0, fp, r10
	strh	r3, [r7, #6]	@ movhi
	mov	r2, r8
	bl	NAND_Memcpy
	mov	r1, r7
	mov	r0, r5
	bl	add_to_cache_write_list_tail
	b	.L1455
.L1432:
	ldr	r3, [r4, #2004]
	mov	r2, fp
	mov	r1, r6
	mov	r0, r4
	blx	r3
	cmp	r0, #10
	bne	.L1434
	mov	r1, r6
	ldr	r0, .L1457+20
	bl	NAND_Print
.L1434:
	movs	r3, #0
	strh	r3, [r7, #4]	@ movhi
	ldr	r3, [r4, #4]
	ldrb	r3, [r3, #8]	@ zero_extendqisi2
	b	.L1453
.L1458:
	.align	2
.L1457:
	.word	.LC263
	.word	.LC264
	.word	.LC265
	.word	.LC266
	.word	.LC250
	.word	.LC267
	.size	nand_cache_write, .-nand_cache_write
	.section	.text.__nand_flush_write_cache,"ax",%progbits
	.align	1
	.global	__nand_flush_write_cache
	.syntax unified
	.thumb
	.thumb_func
	.fpu softvfp
	.type	__nand_flush_write_cache, %function
__nand_flush_write_cache:
	@ args = 0, pretend = 0, frame = 0
	@ frame_needed = 0, uses_anonymous_args = 0
	push	{r3, r4, r5, r6, r7, lr}
	movs	r4, #0
	mov	r6, r0
	mov	r7, r1
	mov	r5, r4
.L1460:
	ldr	r3, [r6, #1608]
	cbz	r3, .L1461
	adds	r4, r4, #1
	movs	r1, #1
	mov	r0, r6
	bl	flush_write_cache_to_nand
	cmp	r7, r4
	orr	r5, r5, r0
	bne	.L1460
.L1461:
	cbz	r5, .L1463
	ldr	r0, .L1474
	bl	NAND_Print
.L1463:
	cbz	r4, .L1459
	ldr	r3, [r6, #356]
	cbz	r3, .L1459
	mov	r2, r4
	ldrb	r1, [r6, #16]	@ zero_extendqisi2
	ldr	r0, .L1474+4
	bl	NAND_Print_DBG
.L1459:
	mov	r0, r5
	pop	{r3, r4, r5, r6, r7, pc}
.L1475:
	.align	2
.L1474:
	.word	.LC257
	.word	.LC268
	.size	__nand_flush_write_cache, .-__nand_flush_write_cache
	.section	.text._nand_flush_write_cache,"ax",%progbits
	.align	1
	.global	_nand_flush_write_cache
	.syntax unified
	.thumb
	.thumb_func
	.fpu softvfp
	.type	_nand_flush_write_cache, %function
_nand_flush_write_cache:
	@ args = 0, pretend = 0, frame = 0
	@ frame_needed = 0, uses_anonymous_args = 0
	@ link register save eliminated.
	ldr	r0, [r0, #12]
	b	__nand_flush_write_cache
	.size	_nand_flush_write_cache, .-_nand_flush_write_cache
	.section	.text.nand_clean_zone_table,"ax",%progbits
	.align	1
	.global	nand_clean_zone_table
	.syntax unified
	.thumb
	.thumb_func
	.fpu softvfp
	.type	nand_clean_zone_table, %function
nand_clean_zone_table:
	@ args = 0, pretend = 0, frame = 0
	@ frame_needed = 0, uses_anonymous_args = 0
	push	{r4, lr}
	mov	r1, #1000
	mov	r4, r0
	bl	__nand_flush_write_cache
	mov	r0, r4
	bl	clean_block_list
	mov	r0, r4
	bl	nand_clean_zone_table2
	mov	r0, r4
	bl	nftl_cache_exit
	movs	r0, #0
	pop	{r4, pc}
	.size	nand_clean_zone_table, .-nand_clean_zone_table
	.section	.text.nand_discard,"ax",%progbits
	.align	1
	.global	nand_discard
	.syntax unified
	.thumb
	.thumb_func
	.fpu softvfp
	.type	nand_discard, %function
nand_discard:
	@ args = 0, pretend = 0, frame = 0
	@ frame_needed = 0, uses_anonymous_args = 0
	push	{r4, r5, r6, r7, r8, lr}
	mov	r6, r0
	ldr	r3, [r6, #24]
	mov	r4, r2
	adds	r2, r1, r2
	mov	r0, r1
	cmp	r2, r3
	bls	.L1479
	ldr	r0, .L1500
	bl	NAND_Print
	movs	r0, #1
.L1478:
	pop	{r4, r5, r6, r7, r8, pc}
.L1479:
	ldr	r1, [r6, #1968]
	ldrd	r2, [r1, #96]
	adds	r7, r2, r4
	adc	r8, r3, #0
	ldr	r3, [r6, #4]
	strd	r7, [r1, #96]
	ldrb	r7, [r3, #8]	@ zero_extendqisi2
	mov	r1, r7
	bl	__aeabi_uidivmod
	mov	r5, r0
	cbz	r1, .L1481
	subs	r1, r7, r1
	adds	r5, r5, #1
	uxth	r1, r1
	cmp	r1, r4
	bhi	.L1489
	subs	r4, r4, r1
.L1482:
	mov	r1, r7
	mov	r0, r4
	bl	__aeabi_uidiv
	cbz	r0, .L1483
	add	r7, r6, #584
	adds	r4, r5, r0
.L1488:
	ldr	r3, [r6, #1972]
	ldrh	r3, [r3]
	cbnz	r3, .L1485
	mov	r1, r5
	mov	r0, r7
	bl	search_from_cache_write_list
	mov	r1, r0
	cbz	r0, .L1486
	mov	r0, r7
	bl	del_from_cache_write_list
.L1486:
	mov	r1, r5
	mov	r0, r7
	bl	search_from_cache_read_list
	mov	r1, r0
	cbz	r0, .L1485
	mov	r0, r7
	bl	del_from_cache_read_list
.L1485:
	mov	r1, r5
	ldr	r3, [r6, #2012]
	mov	r0, r6
	adds	r5, r5, #1
	blx	r3
	ldr	r3, [r6, #4]
	movs	r1, #0
	ldr	ip, [r6, #1968]
	ldrb	r0, [r3, #8]	@ zero_extendqisi2
	ldrd	r3, r2, [ip, #104]
	adds	r3, r3, r0
	adc	r2, r1, r2
	cmp	r5, r4
	strd	r3, r2, [ip, #104]
	bne	.L1488
.L1483:
	movs	r0, #0
	b	.L1478
.L1481:
	cmp	r7, r4
	it	hi
	movhi	r4, #0
	b	.L1482
.L1489:
	movs	r4, #0
	b	.L1482
.L1501:
	.align	2
.L1500:
	.word	.LC269
	.size	nand_discard, .-nand_discard
	.section	.text.__nand_discard,"ax",%progbits
	.align	1
	.global	__nand_discard
	.syntax unified
	.thumb
	.thumb_func
	.fpu softvfp
	.type	__nand_discard, %function
__nand_discard:
	@ args = 0, pretend = 0, frame = 0
	@ frame_needed = 0, uses_anonymous_args = 0
	ldr	r3, [r0, #356]
	push	{r4, r5, r6, lr}
	mov	r4, r0
	mov	r5, r1
	mov	r6, r2
	cmp	r3, #3
	bne	.L1503
	ldr	r0, .L1504
	bl	NAND_Print_DBG
.L1503:
	mov	r2, r6
	mov	r1, r5
	mov	r0, r4
	pop	{r4, r5, r6, lr}
	b	nand_discard
.L1505:
	.align	2
.L1504:
	.word	.LC270
	.size	__nand_discard, .-__nand_discard
	.section	.text._nand_discard,"ax",%progbits
	.align	1
	.global	_nand_discard
	.syntax unified
	.thumb
	.thumb_func
	.fpu softvfp
	.type	_nand_discard, %function
_nand_discard:
	@ args = 0, pretend = 0, frame = 0
	@ frame_needed = 0, uses_anonymous_args = 0
	@ link register save eliminated.
	ldr	r0, [r0, #12]
	b	__nand_discard
	.size	_nand_discard, .-_nand_discard
	.section	.text.__shutdown_op,"ax",%progbits
	.align	1
	.global	__shutdown_op
	.syntax unified
	.thumb
	.thumb_func
	.fpu softvfp
	.type	__shutdown_op, %function
__shutdown_op:
	@ args = 0, pretend = 0, frame = 0
	@ frame_needed = 0, uses_anonymous_args = 0
	@ link register save eliminated.
	movs	r0, #0
	bx	lr
	.size	__shutdown_op, .-__shutdown_op
	.section	.text.print_logic_page_map,"ax",%progbits
	.align	1
	.global	print_logic_page_map
	.syntax unified
	.thumb
	.thumb_func
	.fpu softvfp
	.type	print_logic_page_map, %function
print_logic_page_map:
	@ args = 0, pretend = 0, frame = 0
	@ frame_needed = 0, uses_anonymous_args = 0
	ldr	r3, [r0, #4]
	push	{r0, r1, r4, r5, r6, r7, r8, lr}
	mov	r5, r0
	ldr	r7, .L1513
	movs	r4, #0
	ldr	r8, .L1513+4
	ldrb	r1, [r3, #8]	@ zero_extendqisi2
	ldr	r0, [r0, #24]
	bl	__aeabi_uidiv
	mov	r6, r0
.L1509:
	cmp	r4, r6
	bne	.L1512
	movs	r0, #0
	add	sp, sp, #8
	@ sp needed
	pop	{r4, r5, r6, r7, r8, pc}
.L1512:
	mov	r1, r4
	mov	r0, r5
	bl	get_logic_page_map
	cbnz	r0, .L1510
	mov	r0, r8
	bl	NAND_Print
.L1511:
	adds	r4, r4, #1
	b	.L1509
.L1510:
	ldrh	r2, [r0, #2]
	movw	r3, #65535
	cmp	r2, r3
	beq	.L1511
	ldrb	r1, [r0, #1]	@ zero_extendqisi2
	ldrh	r3, [r0]
	mov	r0, r7
	ubfx	r1, r1, #4, #1
	str	r1, [sp]
	ubfx	r3, r3, #0, #12
	mov	r1, r4
	bl	NAND_Print_DBG
	b	.L1511
.L1514:
	.align	2
.L1513:
	.word	.LC272
	.word	.LC271
	.size	print_logic_page_map, .-print_logic_page_map
	.section	.text.get_last_nftl,"ax",%progbits
	.align	1
	.global	get_last_nftl
	.syntax unified
	.thumb
	.thumb_func
	.fpu softvfp
	.type	get_last_nftl, %function
get_last_nftl:
	@ args = 0, pretend = 0, frame = 0
	@ frame_needed = 0, uses_anonymous_args = 0
	@ link register save eliminated.
	mov	r3, r0
.L1516:
	cbnz	r3, .L1517
	bx	lr
.L1517:
	mov	r0, r3
	ldr	r3, [r3, #8]
	b	.L1516
	.size	get_last_nftl, .-get_last_nftl
	.section	.text.get_nftl_need_read_claim,"ax",%progbits
	.align	1
	.global	get_nftl_need_read_claim
	.syntax unified
	.thumb
	.thumb_func
	.fpu softvfp
	.type	get_nftl_need_read_claim, %function
get_nftl_need_read_claim:
	@ args = 0, pretend = 0, frame = 0
	@ frame_needed = 0, uses_anonymous_args = 0
	@ link register save eliminated.
	mov	r2, r0
	mov	r3, r0
.L1519:
	cbnz	r3, .L1520
	ldr	r3, [r2, #12]
	ldr	r3, [r3, #1968]
	ldr	r3, [r3, #172]
.L1521:
	cbnz	r0, .L1523
.L1518:
	bx	lr
.L1520:
	mov	r2, r3
	ldr	r3, [r3, #8]
	b	.L1519
.L1523:
	ldr	r2, [r0, #12]
	ldrb	r2, [r2, #16]	@ zero_extendqisi2
	cmp	r2, r3
	beq	.L1518
	ldr	r0, [r0, #8]
	b	.L1521
	.size	get_nftl_need_read_claim, .-get_nftl_need_read_claim
	.section	.text.get_nftl_read_claim_page_no,"ax",%progbits
	.align	1
	.global	get_nftl_read_claim_page_no
	.syntax unified
	.thumb
	.thumb_func
	.fpu softvfp
	.type	get_nftl_read_claim_page_no, %function
get_nftl_read_claim_page_no:
	@ args = 0, pretend = 0, frame = 0
	@ frame_needed = 0, uses_anonymous_args = 0
	@ link register save eliminated.
	mov	r3, r0
.L1525:
	cbnz	r3, .L1526
	ldr	r3, [r0, #12]
	ldr	r3, [r3, #1968]
	ldr	r0, [r3, #176]
	bx	lr
.L1526:
	mov	r0, r3
	ldr	r3, [r3, #8]
	b	.L1525
	.size	get_nftl_read_claim_page_no, .-get_nftl_read_claim_page_no
	.section	.text.clean_read_reclaim_complete_flag,"ax",%progbits
	.align	1
	.global	clean_read_reclaim_complete_flag
	.syntax unified
	.thumb
	.thumb_func
	.fpu softvfp
	.type	clean_read_reclaim_complete_flag, %function
clean_read_reclaim_complete_flag:
	@ args = 0, pretend = 0, frame = 0
	@ frame_needed = 0, uses_anonymous_args = 0
	@ link register save eliminated.
.L1528:
	cbnz	r0, .L1529
	bx	lr
.L1529:
	ldr	r2, [r0, #12]
	ldr	r0, [r0, #8]
	ldr	r3, [r2, #364]
	clz	r3, r3
	lsrs	r3, r3, #5
	str	r3, [r2, #364]
	b	.L1528
	.size	clean_read_reclaim_complete_flag, .-clean_read_reclaim_complete_flag
	.section	.text.set_nftl_read_claim_complete,"ax",%progbits
	.align	1
	.global	set_nftl_read_claim_complete
	.syntax unified
	.thumb
	.thumb_func
	.fpu softvfp
	.type	set_nftl_read_claim_complete, %function
set_nftl_read_claim_complete:
	@ args = 0, pretend = 0, frame = 0
	@ frame_needed = 0, uses_anonymous_args = 0
	push	{r3, r4, r5, lr}
	movs	r3, #1
	ldr	r5, [r1, #12]
	mov	r4, r0
	str	r3, [r5, #360]
	mov	r3, r0
.L1531:
	cbnz	r3, .L1532
	ldr	r1, [r1, #8]
	ldr	r2, [r4, #12]
	ldr	r2, [r2, #1968]
	cbnz	r1, .L1533
	str	r3, [r2, #172]
	bl	clean_read_reclaim_complete_flag
.L1534:
	ldr	r3, [r4, #12]
	movs	r0, #0
	ldr	r3, [r3, #1968]
	str	r0, [r3, #176]
	pop	{r3, r4, r5, pc}
.L1532:
	mov	r4, r3
	ldr	r3, [r3, #8]
	b	.L1531
.L1533:
	ldrb	r3, [r5, #16]	@ zero_extendqisi2
	adds	r3, r3, #1
	str	r3, [r2, #172]
	b	.L1534
	.size	set_nftl_read_claim_complete, .-set_nftl_read_claim_complete
	.section	.text.set_nftl_read_claim_flag,"ax",%progbits
	.align	1
	.global	set_nftl_read_claim_flag
	.syntax unified
	.thumb
	.thumb_func
	.fpu softvfp
	.type	set_nftl_read_claim_flag, %function
set_nftl_read_claim_flag:
	@ args = 0, pretend = 0, frame = 0
	@ frame_needed = 0, uses_anonymous_args = 0
	@ link register save eliminated.
.L1536:
	cbnz	r0, .L1537
	bx	lr
.L1537:
	ldr	r3, [r0, #12]
	ldr	r0, [r0, #8]
	ldr	r3, [r3, #1968]
	strd	r1, r2, [r3, #172]
	b	.L1536
	.size	set_nftl_read_claim_flag, .-set_nftl_read_claim_flag
	.section	.text.get_cold_logic_page,"ax",%progbits
	.align	1
	.global	get_cold_logic_page
	.syntax unified
	.thumb
	.thumb_func
	.fpu softvfp
	.type	get_cold_logic_page, %function
get_cold_logic_page:
	@ args = 0, pretend = 0, frame = 0
	@ frame_needed = 0, uses_anonymous_args = 0
	push	{r4, r5, r6, r7, r8, lr}
	mov	r5, r0
	ldr	r8, .L1544
	mov	r6, r2
	mov	r4, r1
	movw	r7, #65535
.L1539:
	cmp	r4, r6
	bcc	.L1543
	mov	r4, #-1
	b	.L1538
.L1543:
	mov	r1, r4
	mov	r0, r5
	bl	get_logic_page_map
	cbnz	r0, .L1540
	mov	r0, r8
	bl	NAND_Print
.L1541:
	adds	r4, r4, #1
	b	.L1539
.L1540:
	ldrh	r3, [r0, #2]
	cmp	r3, r7
	beq	.L1541
	ldrb	r3, [r0, #1]	@ zero_extendqisi2
	ldr	r2, [r5, #364]
	ubfx	r1, r3, #4, #1
	cmp	r1, r2
	beq	.L1541
	bfi	r3, r2, #4, #1
	strb	r3, [r0, #1]
.L1538:
	mov	r0, r4
	pop	{r4, r5, r6, r7, r8, pc}
.L1545:
	.align	2
.L1544:
	.word	.LC271
	.size	get_cold_logic_page, .-get_cold_logic_page
	.section	.text.read_reclaim,"ax",%progbits
	.align	1
	.global	read_reclaim
	.syntax unified
	.thumb
	.thumb_func
	.fpu softvfp
	.type	read_reclaim, %function
read_reclaim:
	@ args = 0, pretend = 0, frame = 0
	@ frame_needed = 0, uses_anonymous_args = 0
	push	{r3, r4, r5, r6, r7, lr}
	mov	r4, r1
	mov	r7, r2
	mov	r6, r0
	bl	get_nftl_read_claim_page_no
	ldr	r5, [r1, #12]
	mov	r1, r0
	ldr	r2, [r5, #28]
	mov	r0, r5
	bl	get_cold_logic_page
	adds	r3, r0, #1
	mov	r5, r0
	bne	.L1547
	mov	r1, r4
	mov	r0, r6
	pop	{r3, r4, r5, r6, r7, lr}
	b	set_nftl_read_claim_complete
.L1547:
	ldr	r0, [r4, #12]
	mov	r3, r7
	mov	r1, r5
	ldr	r2, [r0, #4]
	ldrb	r2, [r2, #8]	@ zero_extendqisi2
	bl	nand_cache_read
	ldr	r3, [r4, #12]
	mov	r2, r5
	mov	r0, r6
	ldrb	r1, [r3, #16]	@ zero_extendqisi2
	bl	set_nftl_read_claim_flag
	movs	r0, #0
	pop	{r3, r4, r5, r6, r7, pc}
	.size	read_reclaim, .-read_reclaim
	.section	.text.nftl_zone_release,"ax",%progbits
	.align	1
	.global	nftl_zone_release
	.syntax unified
	.thumb
	.thumb_func
	.fpu softvfp
	.type	nftl_zone_release, %function
nftl_zone_release:
	@ args = 0, pretend = 0, frame = 0
	@ frame_needed = 0, uses_anonymous_args = 0
	push	{r4, lr}
	mov	r4, r0
	bl	nftl_cache_exit
	mov	r0, r4
	bl	zone_param_exit
	ldr	r3, [r4, #4]
	ldr	r0, [r3, #32]
	bl	nftl_free
	ldr	r0, [r4, #4]
	bl	nftl_free
	mov	r0, r4
	pop	{r4, lr}
	b	nftl_free
	.size	nftl_zone_release, .-nftl_zone_release
	.section	.text.nftl_exit,"ax",%progbits
	.align	1
	.global	nftl_exit
	.syntax unified
	.thumb
	.thumb_func
	.fpu softvfp
	.type	nftl_exit, %function
nftl_exit:
	@ args = 0, pretend = 0, frame = 0
	@ frame_needed = 0, uses_anonymous_args = 0
	push	{r4, lr}
	mov	r4, r0
	ldr	r0, [r0, #4]
	bl	free_nand_partition
	ldr	r0, [r4, #12]
	bl	nftl_zone_release
	ldr	r0, [r4, #16]
	bl	nftl_free
	movs	r0, #0
	pop	{r4, pc}
	.size	nftl_exit, .-nftl_exit
	.section	.text.nand_ftl_exit,"ax",%progbits
	.align	1
	.global	nand_ftl_exit
	.syntax unified
	.thumb
	.thumb_func
	.fpu softvfp
	.type	nand_ftl_exit, %function
nand_ftl_exit:
	@ args = 0, pretend = 0, frame = 0
	@ frame_needed = 0, uses_anonymous_args = 0
	push	{r3, r4, r5, lr}
	ldr	r5, .L1553
	ldr	r0, .L1553
	bl	del_last_nftl_blk
	mov	r4, r0
.L1551:
	mov	r0, r4
	cbnz	r4, .L1552
	pop	{r3, r4, r5, pc}
.L1552:
	bl	nftl_exit
	mov	r0, r4
	bl	nftl_free
	mov	r0, r5
	bl	del_last_nftl_blk
	mov	r4, r0
	b	.L1551
.L1554:
	.align	2
.L1553:
	.word	.LANCHOR0
	.size	nand_ftl_exit, .-nand_ftl_exit
	.section	.text.get_empty_gc_node,"ax",%progbits
	.align	1
	.global	get_empty_gc_node
	.syntax unified
	.thumb
	.thumb_func
	.fpu softvfp
	.type	get_empty_gc_node, %function
get_empty_gc_node:
	@ args = 0, pretend = 0, frame = 0
	@ frame_needed = 0, uses_anonymous_args = 0
	@ link register save eliminated.
	movs	r3, #0
.L1558:
	lsls	r2, r3, #4
	adds	r1, r0, r2
	ldrh	r1, [r1, #6]
	cbnz	r1, .L1556
	adds	r2, r2, #4
	add	r0, r0, r2
	bx	lr
.L1556:
	adds	r3, r3, #1
	cmp	r3, #10
	bne	.L1558
	movs	r0, #0
	bx	lr
	.size	get_empty_gc_node, .-get_empty_gc_node
	.section	.text.search_gc_node,"ax",%progbits
	.align	1
	.global	search_gc_node
	.syntax unified
	.thumb
	.thumb_func
	.fpu softvfp
	.type	search_gc_node, %function
search_gc_node:
	@ args = 0, pretend = 0, frame = 0
	@ frame_needed = 0, uses_anonymous_args = 0
	@ link register save eliminated.
	ldr	r0, [r0, #172]
.L1561:
	cbnz	r0, .L1563
.L1560:
	bx	lr
.L1563:
	ldr	r3, [r0, #4]
	cmp	r3, r1
	beq	.L1560
	ldr	r0, [r0, #8]
	b	.L1561
	.size	search_gc_node, .-search_gc_node
	.section	.text.add_to_gc_list_tail,"ax",%progbits
	.align	1
	.global	add_to_gc_list_tail
	.syntax unified
	.thumb
	.thumb_func
	.fpu softvfp
	.type	add_to_gc_list_tail, %function
add_to_gc_list_tail:
	@ args = 0, pretend = 0, frame = 0
	@ frame_needed = 0, uses_anonymous_args = 0
	@ link register save eliminated.
	add	r2, r0, #164
.L1565:
	ldr	r3, [r2, #8]
	cbnz	r3, .L1566
	str	r1, [r2, #8]
	strd	r3, r2, [r1, #8]
	ldrh	r2, [r0]
	adds	r2, r2, #1
	strh	r2, [r0]	@ movhi
	mov	r0, r3
	bx	lr
.L1566:
	mov	r2, r3
	b	.L1565
	.size	add_to_gc_list_tail, .-add_to_gc_list_tail
	.section	.text.del_from_gc_list,"ax",%progbits
	.align	1
	.global	del_from_gc_list
	.syntax unified
	.thumb
	.thumb_func
	.fpu softvfp
	.type	del_from_gc_list, %function
del_from_gc_list:
	@ args = 0, pretend = 0, frame = 0
	@ frame_needed = 0, uses_anonymous_args = 0
	@ link register save eliminated.
	ldrd	r3, r2, [r1, #8]
	str	r3, [r2, #8]
	ldr	r3, [r1, #8]
	cbz	r3, .L1568
	str	r2, [r3, #12]
.L1568:
	movs	r3, #0
	strd	r3, r3, [r1, #8]
	ldrh	r3, [r0]
	subs	r3, r3, #1
	strh	r3, [r0]	@ movhi
	mov	r0, r1
	bx	lr
	.size	del_from_gc_list, .-del_from_gc_list
	.section	.text.add_prio_gc,"ax",%progbits
	.align	1
	.global	add_prio_gc
	.syntax unified
	.thumb
	.thumb_func
	.fpu softvfp
	.type	add_prio_gc, %function
add_prio_gc:
	@ args = 0, pretend = 0, frame = 0
	@ frame_needed = 0, uses_anonymous_args = 0
	push	{r3, r4, r5, r6, r7, lr}
	add	r5, r0, #392
	mov	r0, r5
	mov	r7, r1
	mov	r6, r2
	bl	search_gc_node
	mov	r4, r0
	cbnz	r0, .L1575
	mov	r0, r5
	bl	get_empty_gc_node
	mov	r1, r0
	cbz	r0, .L1575
	str	r7, [r0, #4]
	strh	r6, [r0, #2]	@ movhi
	strd	r4, r4, [r0, #8]
	mov	r0, r5
	bl	add_to_gc_list_tail
	mov	r0, r4
.L1572:
	pop	{r3, r4, r5, r6, r7, pc}
.L1575:
	movs	r0, #1
	b	.L1572
	.size	add_prio_gc, .-add_prio_gc
	.section	.text.second_scan_all_blocks,"ax",%progbits
	.align	1
	.global	second_scan_all_blocks
	.syntax unified
	.thumb
	.thumb_func
	.fpu softvfp
	.type	second_scan_all_blocks, %function
second_scan_all_blocks:
	@ args = 0, pretend = 0, frame = 48
	@ frame_needed = 0, uses_anonymous_args = 0
	ldr	r3, .L1659
	ldr	r2, .L1659+4
	push	{r4, r5, r6, r7, r8, r10, fp, lr}
	mov	r4, r0
	ldr	r3, [r3]
	sub	sp, sp, #104
	ldr	r3, [r3, #752]
	cmp	r3, r2
	beq	.L1577
	subs	r3, r3, #1
	cmp	r3, #1
	bhi	.L1612
.L1577:
	ldr	r3, [r4, #4]
	ldr	r0, [r4, #24]
	ldrb	r1, [r3, #8]	@ zero_extendqisi2
	bl	__aeabi_uidiv
	ldr	r5, [r4, #252]
	mov	r3, #-1
	str	r0, [r4, #28]
	str	r3, [sp, #64]
	mov	r10, r5
.L1579:
	cmp	r10, #0
	bne	.L1580
	cmp	r5, #0
	beq	.L1613
	ldr	r3, [r4, #1968]
	mov	r7, r10
	ldr	r2, [r5, #8]
	mov	r6, r10
	mov	r8, r10
	str	r10, [sp, #68]
	str	r2, [r3, #160]
.L1596:
	ldr	r2, [r4, #308]
	lsrs	r3, r2, #9
	ubfx	r2, r2, #0, #9
	cbz	r2, .L1583
	adds	r3, r3, #1
.L1583:
	ldr	r2, [r4, #4]
	ldrh	r0, [r5]
	ldr	r1, [r4, #284]
	ldrh	r2, [r2, #20]
	strh	r3, [sp, #92]	@ movhi
	add	r3, sp, #72
	strh	r0, [sp, #90]	@ movhi
	mov	r0, r4
	subs	r2, r2, #1
	str	r1, [sp, #96]
	str	r3, [sp, #100]
	add	r1, sp, #88
	ldr	r3, [r4, #1980]
	strh	r2, [sp, #88]	@ movhi
	blx	r3
	ldr	r3, [r4, #1972]
	mov	fp, r0
	ldrh	r3, [r3, #28]
	cbz	r3, .L1584
	add	r2, sp, #72
	ldr	r1, [r4, #284]
	mov	r0, r4
	bl	crc_wrong
	cmp	r0, #0
	it	ne
	mvnne	fp, #1
.L1584:
	ldr	r3, [r5, #8]
	ldr	r1, [r4, #376]
	ldr	r2, [r4, #1968]
	str	r3, [sp, #60]
	ldrh	r3, [r5, #6]
	cmp	r3, r1
	bls	.L1585
	movw	r1, #49999
	cmp	r3, r1
	itt	ls
	strls	r3, [r4, #376]
	strls	r3, [r2, #148]
.L1585:
	ldr	r1, [r2, #152]
	add	r0, sp, #72
	cmp	r3, r1
	it	cc
	strcc	r3, [r2, #152]
	bl	is_phy_mapping_page
	cmp	r0, #1
	bne	.L1587
	cmp	fp, #0
	beq	.L1588
	cmp	fp, #10
	bne	.L1587
.L1588:
	ldrh	r3, [r5, #4]
	mov	r10, r5
	ldr	r2, [r4, #284]
	mov	r1, r5
	mov	r0, r4
	adds	r3, r3, #1
	strh	r3, [r5, #4]	@ movhi
	ldr	r3, [r4, #4]
	ldrh	r3, [r3, #20]
	subs	r3, r3, #1
	bl	recover_logic_page_mapping
.L1653:
	mov	r8, r5
	b	.L1589
.L1580:
	ldr	r3, [r10, #8]
	ldr	r10, [r10, #28]
	str	r3, [sp, #64]
	ldr	r2, [sp, #64]
	ldr	r3, [r4, #1968]
	str	r2, [r3, #156]
	b	.L1579
.L1587:
	add	r0, sp, #72
	bl	is_nouse_page
	cmp	r0, #1
	bne	.L1590
	cbz	r6, .L1615
	ldrh	r1, [r6]
	cbnz	r7, .L1591
	ldrh	r2, [r5]
	ldr	r0, .L1659+8
	bl	NAND_Print
	ldrb	r3, [r4, #17]	@ zero_extendqisi2
	lsls	r0, r3, #31
	bmi	.L1617
	ldr	r3, [r6, #8]
	ldr	r0, [r5, #8]
	cmp	r3, r0
	beq	.L1617
	ldrh	r2, [r5]
	ldrh	r1, [r6]
	str	r0, [sp]
	ldr	r0, .L1659+12
	bl	NAND_Print
	ldr	r2, [r4, #304]
	mov	r1, r6
	mov	r0, r4
	bl	recover_phy_page_mapping
	ldr	r3, [r4, #4]
	mov	r1, r6
	ldr	r2, [r4, #304]
	mov	r0, r4
	ldrh	r3, [r3, #20]
	subs	r3, r3, #1
	bl	recover_logic_page_mapping
	movs	r2, #3
	mov	r1, r6
	mov	r0, r4
	bl	add_prio_gc
.L1615:
	mov	r6, r5
	b	.L1589
.L1591:
	ldr	r2, [r6, #8]
	ldr	r0, .L1659+16
	bl	NAND_Print
	ldr	r2, [r7, #8]
	ldrh	r1, [r7]
	ldr	r0, .L1659+20
	bl	NAND_Print
	ldr	r2, [sp, #60]
	ldrh	r1, [r5]
	ldr	r0, .L1659+24
	bl	NAND_Print
	mov	r1, r6
	mov	r6, r7
	movs	r2, #3
	mov	r0, r4
	bl	add_prio_gc
.L1617:
	mov	r7, r5
	b	.L1589
.L1590:
	ldrb	r0, [sp, #87]	@ zero_extendqisi2
	ldrb	r2, [sp, #73]	@ zero_extendqisi2
	ldrb	r1, [sp, #72]	@ zero_extendqisi2
	str	r0, [sp, #48]
	ldrb	r0, [sp, #86]	@ zero_extendqisi2
	ldrb	r3, [sp, #74]	@ zero_extendqisi2
	str	r0, [sp, #44]
	ldrb	r0, [sp, #85]	@ zero_extendqisi2
	str	r0, [sp, #40]
	ldrb	r0, [sp, #84]	@ zero_extendqisi2
	str	r0, [sp, #36]
	ldrb	r0, [sp, #83]	@ zero_extendqisi2
	str	r0, [sp, #32]
	ldrb	r0, [sp, #82]	@ zero_extendqisi2
	str	r0, [sp, #28]
	ldrb	r0, [sp, #81]	@ zero_extendqisi2
	str	r0, [sp, #24]
	ldrb	r0, [sp, #80]	@ zero_extendqisi2
	str	r0, [sp, #20]
	ldrb	r0, [sp, #79]	@ zero_extendqisi2
	str	r0, [sp, #16]
	ldrb	r0, [sp, #78]	@ zero_extendqisi2
	str	r0, [sp, #12]
	ldrb	r0, [sp, #77]	@ zero_extendqisi2
	str	r0, [sp, #8]
	ldrb	r0, [sp, #76]	@ zero_extendqisi2
	str	r0, [sp, #4]
	ldrb	r0, [sp, #75]	@ zero_extendqisi2
	str	r0, [sp]
	ldr	r0, .L1659+28
	bl	NAND_Print
	ldrh	r2, [r5]
	ldrb	r1, [r4, #16]	@ zero_extendqisi2
	cmp	fp, #0
	beq	.L1592
	cmp	fp, #10
	bne	.L1593
.L1592:
	ldr	r0, .L1659+32
.L1652:
	bl	NAND_Print
	ldrb	r3, [r4, #17]	@ zero_extendqisi2
	lsls	r1, r3, #31
	bpl	.L1595
	ldrd	r2, r3, [sp, #60]
	cmp	r3, r2
	bne	.L1595
	ldrh	r2, [r5, #6]
	mov	r1, r5
	mov	r0, r4
	adds	r2, r2, #1
	uxth	r2, r2
	bl	erase_block
	str	r5, [sp, #68]
.L1589:
	ldr	r5, [r5, #28]
	cmp	r5, #0
	bne	.L1596
.L1581:
	mov	r1, r10
	mov	r0, r4
	bl	init_smart_info
	ldr	r5, [r4, #252]
.L1597:
	cmp	r5, #0
	bne	.L1599
	mov	r0, r4
	bl	adjust_invaild_list
	cmp	r8, #0
	beq	.L1600
	cbz	r6, .L1600
	ldr	r1, [r8, #8]
	ldr	r2, [r6, #8]
	adds	r3, r1, #1
	cmp	r3, r2
	beq	.L1600
	ldr	r0, .L1659+36
	bl	NAND_Print
.L1600:
	movs	r3, #0
	str	r3, [r4, #292]
	cmp	r6, #0
	bne	.L1601
	cmp	r7, #0
	bne	.L1602
	ldr	r0, .L1659+40
	bl	NAND_Print
	ldrb	r3, [r4, #17]	@ zero_extendqisi2
	lsls	r2, r3, #31
	bpl	.L1603
	cmp	r8, #0
	beq	.L1604
.L1657:
	movs	r2, #3
	mov	r1, r8
	mov	r0, r4
	bl	add_prio_gc
	b	.L1604
.L1593:
	ldr	r0, .L1659+44
	b	.L1652
.L1595:
	movs	r3, #170
	ldr	r2, [r4, #304]
	strh	r3, [r5, #2]	@ movhi
	mov	r1, r5
	ldrh	r3, [r5, #4]
	mov	r0, r4
	adds	r3, r3, #1
	strh	r3, [r5, #4]	@ movhi
	bl	recover_block_phy_page_mapping
	ldr	r3, [r4, #4]
	mov	r1, r5
	ldr	r2, [r4, #304]
	mov	r0, r4
	ldrh	r3, [r3, #20]
	subs	r3, r3, #1
	bl	recover_logic_page_mapping
	movs	r2, #3
	mov	r1, r5
	mov	r0, r4
	bl	add_prio_gc
	b	.L1653
.L1660:
	.align	2
.L1659:
	.word	g_nreg_base
	.word	196609
	.word	.LC273
	.word	.LC274
	.word	.LC275
	.word	.LC276
	.word	.LC277
	.word	.LC278
	.word	.LC279
	.word	.LC281
	.word	.LC282
	.word	.LC280
.L1613:
	mov	r3, #-1
	mov	r7, r10
	mov	r6, r10
	mov	r8, r10
	str	r10, [sp, #68]
	str	r3, [sp, #60]
	b	.L1581
.L1599:
	cmp	r6, r5
	beq	.L1598
	cmp	r7, r5
	beq	.L1598
	ldr	r3, [sp, #68]
	cmp	r3, r5
	beq	.L1598
	mov	r1, r5
	mov	r0, r4
	bl	put_phy_block_to_invalid_page_list
.L1598:
	ldr	r5, [r5, #28]
	b	.L1597
.L1603:
	cmp	r8, #0
	beq	.L1604
	ldrh	r3, [r8, #2]
	cmp	r3, #170
	beq	.L1657
.L1604:
	ldr	r2, [sp, #60]
	mov	r1, r7
	mov	r0, r4
	bl	get_current_used_block
	mov	r5, r0
	cmp	r0, #0
	beq	.L1611
	ldr	r0, .L1661
	bl	NAND_Print
	b	.L1658
.L1601:
	cbnz	r7, .L1602
	ldrb	r3, [r4, #17]	@ zero_extendqisi2
	lsls	r3, r3, #31
	bpl	.L1605
	ldr	r2, [r6, #8]
	ldrh	r1, [r6]
	ldr	r0, .L1661+4
	bl	NAND_Print_DBG
	cmp	r8, #0
	beq	.L1606
	ldrh	r3, [r8, #2]
	cmp	r3, #170
	bne	.L1606
	movs	r2, #3
	mov	r1, r8
	mov	r0, r4
	bl	add_prio_gc
.L1606:
	movs	r2, #0
.L1655:
	mov	r1, r6
	mov	r0, r4
	bl	cross_talk_rebuild_current_block
	mov	r7, r0
	cmp	r0, #0
	bne	.L1604
.L1658:
	movs	r5, #1
.L1576:
	mov	r0, r5
	add	sp, sp, #104
	@ sp needed
	pop	{r4, r5, r6, r7, r8, r10, fp, pc}
.L1605:
	cmp	r8, #0
	beq	.L1607
	ldrh	r3, [r8, #2]
	cmp	r3, #170
	bne	.L1607
	movs	r2, #3
	mov	r1, r8
	mov	r0, r4
	bl	add_prio_gc
.L1607:
	movs	r2, #0
.L1656:
	mov	r1, r6
	mov	r0, r4
	bl	no_cross_talk_rebuild_current_block
	mov	r7, r0
	b	.L1604
.L1602:
	ldrb	r3, [r4, #17]	@ zero_extendqisi2
	ands	r5, r3, #1
	beq	.L1608
	mov	r2, r7
	b	.L1655
.L1608:
	ldr	r2, [r6, #8]
	ldr	r3, [r7, #8]
	cmp	r2, r3
	bne	.L1610
	ldrh	r1, [r6]
	ldr	r0, .L1661+8
	bl	NAND_Print
	ldr	r2, [r7, #8]
	ldrh	r1, [r7]
	ldr	r0, .L1661+12
	bl	NAND_Print_DBG
	mov	r2, r7
	b	.L1656
.L1610:
	ldr	r0, .L1661+16
	bl	NAND_Print
	ldr	r2, [r8, #8]
	ldrh	r1, [r8]
	ldr	r0, .L1661+20
	bl	NAND_Print
	ldr	r2, [r6, #8]
	ldrh	r1, [r6]
	ldr	r0, .L1661+8
	bl	NAND_Print
	ldr	r2, [r7, #8]
	ldrh	r1, [r7]
	ldr	r0, .L1661+12
	bl	NAND_Print_DBG
	ldr	r2, [r4, #304]
	mov	r1, r6
	mov	r0, r4
	bl	recover_block_phy_page_mapping
	ldr	r3, [r4, #4]
	mov	r1, r6
	ldr	r2, [r4, #304]
	mov	r0, r4
	ldrh	r3, [r3, #20]
	subs	r3, r3, #1
	bl	recover_logic_page_mapping
	movs	r2, #3
	mov	r1, r6
	mov	r0, r4
	bl	add_prio_gc
	mov	r1, r7
	ldr	r2, [r4, #304]
	mov	r0, r4
	bl	recover_block_phy_page_mapping
	ldr	r3, [r4, #4]
	mov	r1, r7
	ldr	r2, [r4, #304]
	mov	r0, r4
	ldrh	r3, [r3, #20]
	subs	r3, r3, #1
	bl	recover_logic_page_mapping
	mov	r1, r7
	movs	r2, #3
	mov	r0, r4
	mov	r7, r5
	bl	add_prio_gc
	b	.L1604
.L1611:
	mov	r0, r4
	bl	adjust_invaild_list
	ldr	r2, [r4, #1968]
	ldr	r3, [r4, #292]
	ldrh	r1, [r4, #22]
	ldr	r0, .L1661+24
	str	r3, [r4, #48]
	ldr	r3, [r2, #160]
	str	r3, [sp, #4]
	ldr	r3, [r2, #156]
	str	r3, [sp]
	ldrd	r2, r3, [r2, #148]
	bl	NAND_Print_DBG
	b	.L1576
.L1612:
	mov	r5, #-1
	b	.L1576
.L1662:
	.align	2
.L1661:
	.word	.LC288
	.word	.LC283
	.word	.LC284
	.word	.LC285
	.word	.LC286
	.word	.LC287
	.word	.LC289
	.size	second_scan_all_blocks, .-second_scan_all_blocks
	.section	.text.check_static_wear_leveling,"ax",%progbits
	.align	1
	.global	check_static_wear_leveling
	.syntax unified
	.thumb
	.thumb_func
	.fpu softvfp
	.type	check_static_wear_leveling, %function
check_static_wear_leveling:
	@ args = 0, pretend = 0, frame = 0
	@ frame_needed = 0, uses_anonymous_args = 0
	push	{r3, r4, r5, r6, r7, r8, r10, lr}
	mov	r4, r0
	ldr	r3, [r0, #580]
	cbz	r3, .L1664
.L1666:
	movs	r0, #1
.L1663:
	pop	{r3, r4, r5, r6, r7, r8, r10, pc}
.L1664:
	ldr	r3, [r0, #200]
	cmp	r3, #0
	beq	.L1666
	ldr	r3, [r0, #172]
	cmp	r3, #0
	beq	.L1666
	ldr	r3, [r0, #292]
	ldrh	r7, [r0, #18]
	ldr	r8, [r3, #8]
	cmp	r8, r7
	blt	.L1666
	ldr	r3, [r0, #4]
	ldrh	r5, [r3, #16]
	mov	r1, r5
	bl	get_block_max_erase_time_from_free_list
	cmp	r0, #0
	beq	.L1666
	ldrh	r6, [r0, #6]
	ldr	r0, .L1684
	umull	r0, r1, r5, r0
	ubfx	r10, r1, #2, #16
	cmp	r6, r10
	bcc	.L1666
	sub	r2, r8, r7
	mov	r1, r5
	mov	r0, r4
	bl	get_block_min_erase_time_from_invalid_list
	mov	r7, r0
	cmp	r0, #0
	beq	.L1666
	ldrh	r8, [r0, #6]
	cmp	r6, r8
	bls	.L1666
	lsls	r2, r6, #1
	adds	r3, r2, r6
	cmp	r3, r5, lsl #1
	ble	.L1667
	ldr	r2, .L1684+4
.L1683:
	umull	r2, r3, r5, r2
	lsrs	r3, r3, #3
	strh	r3, [r4, #574]	@ movhi
.L1668:
	ldrh	r2, [r4, #574]
	sub	r3, r6, r8
	cmp	r3, r2
	blt	.L1666
	mov	r2, r8
	mov	r1, r6
	ldr	r0, .L1684+8
	bl	NAND_Print_DBG
	ldrh	r1, [r7]
	ldr	r0, .L1684+12
	bl	NAND_Print_DBG
	ldrh	r1, [r4, #18]
	ldr	r0, .L1684+16
	bl	NAND_Print_DBG
	str	r7, [r4, #580]
	movs	r0, #0
	b	.L1663
.L1667:
	cmp	r5, r2
	bge	.L1669
	ldr	r2, .L1684
	b	.L1683
.L1669:
	cmp	r5, r3
	bge	.L1670
	movs	r1, #7
	mov	r0, r5
	bl	__aeabi_uidiv
	strh	r0, [r4, #574]	@ movhi
	b	.L1668
.L1670:
	strh	r10, [r4, #574]	@ movhi
	b	.L1668
.L1685:
	.align	2
.L1684:
	.word	-858993459
	.word	-1431655765
	.word	.LC290
	.word	.LC291
	.word	.LC292
	.size	check_static_wear_leveling, .-check_static_wear_leveling
	.section	.text.prio_gc_one,"ax",%progbits
	.align	1
	.global	prio_gc_one
	.syntax unified
	.thumb
	.thumb_func
	.fpu softvfp
	.type	prio_gc_one, %function
prio_gc_one:
	@ args = 0, pretend = 0, frame = 0
	@ frame_needed = 0, uses_anonymous_args = 0
	ldr	r3, [r0, #4]
	cmp	r2, #1
	push	{r4, lr}
	ldr	r4, [r3, #32]
	mov	r3, #36
	mla	r1, r3, r1, r4
	beq	.L1687
	cmp	r2, #2
	it	ne
	movne	r2, #3
.L1687:
	uxth	r2, r2
	bl	add_prio_gc
	movs	r0, #0
	pop	{r4, pc}
	.size	prio_gc_one, .-prio_gc_one
	.section	.text.get_total_invalid_page,"ax",%progbits
	.align	1
	.global	get_total_invalid_page
	.syntax unified
	.thumb
	.thumb_func
	.fpu softvfp
	.type	get_total_invalid_page, %function
get_total_invalid_page:
	@ args = 0, pretend = 0, frame = 0
	@ frame_needed = 0, uses_anonymous_args = 0
	@ link register save eliminated.
	add	r3, r0, #188
	movs	r0, #0
.L1694:
	ldr	r3, [r3, #12]
	cbnz	r3, .L1695
	bx	lr
.L1695:
	ldrh	r2, [r3, #4]
	add	r0, r0, r2
	b	.L1694
	.size	get_total_invalid_page, .-get_total_invalid_page
	.section	.text.is_current_blcok,"ax",%progbits
	.align	1
	.global	is_current_blcok
	.syntax unified
	.thumb
	.thumb_func
	.fpu softvfp
	.type	is_current_blcok, %function
is_current_blcok:
	@ args = 0, pretend = 0, frame = 0
	@ frame_needed = 0, uses_anonymous_args = 0
	@ link register save eliminated.
	ldrb	r3, [r0, #17]	@ zero_extendqisi2
	lsls	r2, r3, #31
	bpl	.L1697
	ldr	r3, [r0, #324]
	cmp	r1, r3
	beq	.L1699
.L1697:
	ldr	r0, [r0, #292]
	subs	r3, r0, r1
	rsbs	r0, r3, #0
	adcs	r0, r0, r3
	bx	lr
.L1699:
	movs	r0, #1
	bx	lr
	.size	is_current_blcok, .-is_current_blcok
	.section	.text.garbage_collect_first,"ax",%progbits
	.align	1
	.global	garbage_collect_first
	.syntax unified
	.thumb
	.thumb_func
	.fpu softvfp
	.type	garbage_collect_first, %function
garbage_collect_first:
	@ args = 0, pretend = 0, frame = 40
	@ frame_needed = 0, uses_anonymous_args = 0
	push	{r4, r5, r6, r7, r8, r10, fp, lr}
	sub	sp, sp, #96
	mov	r5, r1
	add	r7, sp, #64
	mov	r4, r0
	mov	r6, r2
	movs	r1, #90
	movs	r2, #16
	mov	r0, r7
	bl	NAND_Memset
	cbnz	r5, .L1701
	mov	r0, r4
	bl	out_phy_block_from_invalid_page_list
.L1741:
	mov	r5, r0
	cbnz	r0, .L1704
	ldr	r0, .L1742
	bl	NAND_Print
	movs	r0, #8
.L1700:
	add	sp, sp, #96
	@ sp needed
	pop	{r4, r5, r6, r7, r8, r10, fp, pc}
.L1701:
	mov	r1, r5
	mov	r0, r4
	bl	is_current_blcok
	cmp	r0, #1
	beq	.L1719
	mov	r0, r4
	bl	out_phy_block_from_invalid_page_list_by_block
	b	.L1741
.L1704:
	ldr	r2, [r4, #1968]
	ldrh	r0, [r0]
	ldr	r3, [r2, #120]
	strh	r0, [sp, #82]	@ movhi
	mov	r0, r4
	adds	r3, r3, #1
	str	r3, [r2, #120]
	ldr	r3, [r4, #4]
	ldr	r2, [r4, #284]
	ldrh	r1, [r3, #28]
	ldrh	r3, [r3, #20]
	strd	r2, r7, [sp, #88]
	strh	r1, [sp, #84]	@ movhi
	add	r1, sp, #80
	subs	r3, r3, #1
	strh	r3, [sp, #80]	@ movhi
	ldr	r3, [r4, #1980]
	blx	r3
	mov	r10, r0
	mov	r0, r7
	bl	is_phy_mapping_page
	cmp	r0, #1
	mov	r8, r0
	beq	.L1705
	ldr	r0, .L1742+4
	mov	r8, #1
	bl	NAND_Print_DBG
	ldr	r3, [r4, #4]
	ldrh	r1, [r5]
	ldr	r0, .L1742+8
	ldrh	r2, [r3, #20]
	ldrh	r3, [r3, #28]
	subs	r2, r2, #1
	str	r3, [sp]
	movs	r3, #0
	bl	NAND_Print_DBG
	ldrb	r0, [sp, #79]	@ zero_extendqisi2
	ldrb	r3, [sp, #67]	@ zero_extendqisi2
	ldrb	r2, [sp, #66]	@ zero_extendqisi2
	str	r0, [sp, #44]
	ldrb	r0, [sp, #78]	@ zero_extendqisi2
	ldrb	r1, [sp, #65]	@ zero_extendqisi2
	str	r0, [sp, #40]
	ldrb	r0, [sp, #77]	@ zero_extendqisi2
	str	r0, [sp, #36]
	ldrb	r0, [sp, #76]	@ zero_extendqisi2
	str	r0, [sp, #32]
	ldrb	r0, [sp, #75]	@ zero_extendqisi2
	str	r0, [sp, #28]
	ldrb	r0, [sp, #74]	@ zero_extendqisi2
	str	r0, [sp, #24]
	ldrb	r0, [sp, #73]	@ zero_extendqisi2
	str	r0, [sp, #20]
	ldrb	r0, [sp, #72]	@ zero_extendqisi2
	str	r0, [sp, #16]
	ldrb	r0, [sp, #71]	@ zero_extendqisi2
	str	r0, [sp, #12]
	ldrb	r0, [sp, #70]	@ zero_extendqisi2
	str	r0, [sp, #8]
	ldrb	r0, [sp, #69]	@ zero_extendqisi2
	str	r0, [sp, #4]
	ldrb	r0, [sp, #68]	@ zero_extendqisi2
	str	r0, [sp]
	ldr	r0, .L1742+12
	bl	NAND_Print_DBG
	ldrh	r2, [r5]
	ldrb	r1, [r4, #16]	@ zero_extendqisi2
	ldr	r0, .L1742+16
	bl	NAND_Print_DBG
	ldr	r2, [r4, #268]
	mov	r1, r5
	mov	r0, r4
	bl	recover_block_phy_page_mapping
.L1706:
	ldr	r3, [r4, #4]
	mov	r2, r5
	add	r1, r4, #260
	mov	r0, r4
	ldrh	r3, [r3, #20]
	bl	check_invalid_page_right
	cmp	r0, #0
	beq	.L1708
	mov	r8, #1
	movw	r6, #65535
	ldr	r0, .L1742+20
	bl	NAND_Print
.L1710:
	mov	r10, #0
.L1709:
	ldr	r3, [r4, #4]
	ldrh	r3, [r3, #20]
	subs	r3, r3, #1
	cmp	r3, r10
	bhi	.L1717
.L1716:
	ldr	r3, [r4, #4]
	ldrh	r3, [r3, #20]
	subs	r3, r3, #1
	cmp	r3, r10
	bne	.L1718
	movs	r6, #0
	strh	r6, [r4, #386]	@ movhi
	cmp	r8, #0
	beq	.L1718
	ldr	r0, .L1742+24
	bl	NAND_Print
	ldrh	r2, [r5, #6]
	mov	r1, r5
	mov	r0, r4
	adds	r2, r2, #1
	uxth	r2, r2
	bl	erase_block
.L1719:
	movs	r0, #0
	b	.L1700
.L1705:
	cmp	r10, #0
	beq	.L1707
	cmp	r10, #10
	beq	.L1707
	ldrh	r1, [r5]
	ldr	r0, .L1742+28
	bl	NAND_Print_DBG
	ldr	r2, [r4, #268]
	mov	r1, r5
	mov	r0, r4
	bl	recover_block_phy_page_mapping
	b	.L1706
.L1707:
	ldr	r2, [r4, #272]
	mov	r8, #0
	ldr	r1, [r4, #284]
	ldr	r0, [r4, #268]
	bl	NAND_Memcpy
	b	.L1706
.L1708:
	movw	r3, #65535
	cmp	r8, #0
	it	ne
	movne	r6, r3
	b	.L1710
.L1717:
	ldr	r3, [r4, #268]
	ldr	r3, [r3, r10, lsl #2]
	adds	r2, r3, #1
	beq	.L1711
	mov	r1, r3
	mov	r0, r4
	str	r3, [sp, #60]
	bl	get_logic_page_map
	mov	fp, r0
	cmp	r0, #0
	beq	.L1711
	ldrh	r2, [r0, #2]
	ldrh	r1, [r5]
	cmp	r1, r2
	bne	.L1711
	ldrh	r2, [r0]
	ubfx	r2, r2, #0, #12
	cmp	r2, r10
	bne	.L1711
	ldr	r0, [r4, #1968]
	ldr	ip, [r4, #4]
	strh	r1, [sp, #82]	@ movhi
	add	r1, sp, #80
	ldr	r2, [r0, #124]
	strh	r10, [sp, #80]	@ movhi
	adds	r2, r2, #1
	str	r2, [r0, #124]
	ldrh	r0, [ip, #28]
	ldr	r2, [r4, #288]
	strh	r0, [sp, #84]	@ movhi
	mov	r0, r4
	strd	r2, r7, [sp, #88]
	ldr	r2, [r4, #1980]
	blx	r2
	str	r0, [sp, #56]
	mov	r0, r7
	bl	get_logic_page_from_oob
	ldrd	r2, r3, [sp, #56]
	cbz	r2, .L1712
	cmp	r2, #10
	bne	.L1713
.L1712:
	cmp	r3, r0
	beq	.L1714
	ldrh	r1, [r5]
	mov	r2, r10
	str	r3, [sp]
	mov	r3, r0
	ldr	r0, .L1742+32
	mov	r8, #1
	bl	NAND_Print_DBG
	ldrb	r0, [sp, #79]	@ zero_extendqisi2
	movw	r6, #65535
	ldrb	r3, [sp, #66]	@ zero_extendqisi2
	ldrb	r2, [sp, #65]	@ zero_extendqisi2
	str	r0, [sp, #48]
	ldrb	r0, [sp, #78]	@ zero_extendqisi2
	ldrb	r1, [sp, #64]	@ zero_extendqisi2
	str	r0, [sp, #44]
	ldrb	r0, [sp, #77]	@ zero_extendqisi2
	str	r0, [sp, #40]
	ldrb	r0, [sp, #76]	@ zero_extendqisi2
	str	r0, [sp, #36]
	ldrb	r0, [sp, #75]	@ zero_extendqisi2
	str	r0, [sp, #32]
	ldrb	r0, [sp, #74]	@ zero_extendqisi2
	str	r0, [sp, #28]
	ldrb	r0, [sp, #73]	@ zero_extendqisi2
	str	r0, [sp, #24]
	ldrb	r0, [sp, #72]	@ zero_extendqisi2
	str	r0, [sp, #20]
	ldrb	r0, [sp, #71]	@ zero_extendqisi2
	str	r0, [sp, #16]
	ldrb	r0, [sp, #70]	@ zero_extendqisi2
	str	r0, [sp, #12]
	ldrb	r0, [sp, #69]	@ zero_extendqisi2
	str	r0, [sp, #8]
	ldrb	r0, [sp, #68]	@ zero_extendqisi2
	str	r0, [sp, #4]
	ldrb	r0, [sp, #67]	@ zero_extendqisi2
	str	r0, [sp]
	ldr	r0, .L1742+36
	bl	NAND_Print
	ldrh	r3, [fp]
	movw	r2, #4095
	strh	r6, [fp, #2]	@ movhi
	bfi	r3, r2, #0, #12
	strh	r3, [fp]	@ movhi
	ldrh	r3, [r5, #4]
	adds	r3, r3, #1
	strh	r3, [r5, #4]	@ movhi
.L1715:
	subs	r6, r6, #1
	uxth	r6, r6
	cmp	r6, #0
	beq	.L1716
.L1711:
	add	r10, r10, #1
	b	.L1709
.L1714:
	ldr	r2, [r4, #288]
	mov	r1, r3
	mov	r0, r4
	bl	nand_write_logic_page_no_gc
	cmp	r0, #0
	beq	.L1715
	ldr	r0, .L1742+40
	bl	NAND_Print
	movs	r0, #1
	b	.L1700
.L1713:
	ldrh	r3, [r5, #4]
	mov	r8, #1
	movw	r6, #65535
	adds	r3, r3, #1
	strh	r3, [r5, #4]	@ movhi
	b	.L1715
.L1718:
	mov	r1, r5
	mov	r0, r4
	bl	put_phy_block_to_invalid_page_list
	mov	r0, r4
	bl	adjust_invaild_list
	b	.L1719
.L1743:
	.align	2
.L1742:
	.word	.LC293
	.word	.LC294
	.word	.LC295
	.word	.LC296
	.word	.LC297
	.word	.LC299
	.word	.LC303
	.word	.LC298
	.word	.LC300
	.word	.LC301
	.word	.LC302
	.size	garbage_collect_first, .-garbage_collect_first
	.section	.text.gc_one,"ax",%progbits
	.align	1
	.global	gc_one
	.syntax unified
	.thumb
	.thumb_func
	.fpu softvfp
	.type	gc_one, %function
gc_one:
	@ args = 0, pretend = 0, frame = 0
	@ frame_needed = 0, uses_anonymous_args = 0
	ldr	r3, [r0, #200]
	push	{r4, lr}
	cbnz	r3, .L1745
.L1747:
	movs	r4, #8
.L1744:
	mov	r0, r4
	pop	{r4, pc}
.L1745:
	ldrh	r3, [r3, #4]
	cmp	r3, r1
	bls	.L1747
	movw	r2, #65535
	movs	r1, #0
	bl	garbage_collect_first
	cmp	r0, #8
	mov	r4, r0
	beq	.L1747
	cbnz	r0, .L1748
	ldr	r0, .L1752
	bl	NAND_Print_DBG
	b	.L1744
.L1748:
	ldr	r0, .L1752+4
	movs	r4, #1
	bl	NAND_Print
	b	.L1744
.L1753:
	.align	2
.L1752:
	.word	.LC304
	.word	.LC305
	.size	gc_one, .-gc_one
	.section	.text.gc_all_cnt,"ax",%progbits
	.align	1
	.global	gc_all_cnt
	.syntax unified
	.thumb
	.thumb_func
	.fpu softvfp
	.type	gc_all_cnt, %function
gc_all_cnt:
	@ args = 0, pretend = 0, frame = 0
	@ frame_needed = 0, uses_anonymous_args = 0
	push	{r4, r5, r6, lr}
	mov	r5, r0
	mov	r6, r1
	mov	r4, r2
.L1755:
	cbz	r4, .L1759
	mov	r1, r6
	mov	r0, r5
	bl	gc_one
	cmp	r0, #8
	add	r4, r4, #-1
	bne	.L1755
.L1759:
	movs	r0, #0
	pop	{r4, r5, r6, pc}
	.size	gc_all_cnt, .-gc_all_cnt
	.section	.text.gc_all_base_on_free_blks,"ax",%progbits
	.align	1
	.global	gc_all_base_on_free_blks
	.syntax unified
	.thumb
	.thumb_func
	.fpu softvfp
	.type	gc_all_base_on_free_blks, %function
gc_all_base_on_free_blks:
	@ args = 0, pretend = 0, frame = 0
	@ frame_needed = 0, uses_anonymous_args = 0
	ldrh	r3, [r0, #22]
	ldrh	r1, [r0, #380]
	push	{r4, r5, r6, lr}
	mov	r5, r0
	cmp	r3, r1
	bcc	.L1760
	ldrh	r2, [r0, #382]
	add	r2, r2, r2, lsl #1
	cmp	r2, r3
	bcc	.L1760
	ldr	r6, .L1765
	ldrb	r0, [r6, #10]	@ zero_extendqisi2
	ldrh	r4, [r6, #8]
	muls	r4, r0, r4
	ldr	r0, [r6, #4]
	muls	r0, r4, r0
	cmp	r0, #264192
	bcs	.L1760
	ldr	r0, [r5, #4]
	subs	r3, r3, r1
	subs	r1, r2, r1
	ldrh	r4, [r0, #20]
	lsrs	r4, r4, #1
	subs	r0, r4, #3
	muls	r0, r3, r0
	bl	__aeabi_uidiv
	adds	r1, r4, r0
	movs	r2, #3
	mov	r0, r5
	uxth	r1, r1
	pop	{r4, r5, r6, lr}
	b	gc_all_cnt
.L1760:
	movs	r0, #0
	pop	{r4, r5, r6, pc}
.L1766:
	.align	2
.L1765:
	.word	.LANCHOR18
	.size	gc_all_base_on_free_blks, .-gc_all_base_on_free_blks
	.section	.text.gc_all,"ax",%progbits
	.align	1
	.global	gc_all
	.syntax unified
	.thumb
	.thumb_func
	.fpu softvfp
	.type	gc_all, %function
gc_all:
	@ args = 0, pretend = 0, frame = 0
	@ frame_needed = 0, uses_anonymous_args = 0
	push	{r3, r4, r5, lr}
	mov	r4, r0
	mov	r5, r1
.L1768:
	mov	r1, r5
	mov	r0, r4
	bl	gc_one
	cmp	r0, #0
	beq	.L1768
	movs	r0, #0
	pop	{r3, r4, r5, pc}
	.size	gc_all, .-gc_all
	.section	.text.gc_all_enhance,"ax",%progbits
	.align	1
	.global	gc_all_enhance
	.syntax unified
	.thumb
	.thumb_func
	.fpu softvfp
	.type	gc_all_enhance, %function
gc_all_enhance:
	@ args = 0, pretend = 0, frame = 0
	@ frame_needed = 0, uses_anonymous_args = 0
	@ link register save eliminated.
	ldrh	r1, [r0, #22]
	ldrh	r2, [r0, #380]
	push	{r4}
	cmp	r2, r1
	bhi	.L1771
	ldrh	r2, [r0, #382]
	add	r2, r2, r2, lsl #1
	cmp	r2, r1
	bcc	.L1771
	ldr	r4, .L1776
	ldrb	r2, [r4, #10]	@ zero_extendqisi2
	ldrh	r1, [r4, #8]
	muls	r1, r2, r1
	ldr	r2, [r4, #4]
	muls	r2, r1, r2
	cmp	r2, #264192
	bcs	.L1771
	ldr	r3, [r0, #4]
	pop	{r4}
	ldrh	r1, [r3, #20]
	lsrs	r1, r1, #1
	b	gc_all
.L1771:
	movs	r0, #0
	pop	{r4}
	bx	lr
.L1777:
	.align	2
.L1776:
	.word	.LANCHOR18
	.size	gc_all_enhance, .-gc_all_enhance
	.section	.text.gc_win_block,"ax",%progbits
	.align	1
	.global	gc_win_block
	.syntax unified
	.thumb
	.thumb_func
	.fpu softvfp
	.type	gc_win_block, %function
gc_win_block:
	@ args = 0, pretend = 0, frame = 0
	@ frame_needed = 0, uses_anonymous_args = 0
	push	{r3, r4, r5, r6, r7, lr}
	mov	r7, r0
	ldr	r3, [r0, #4]
	movs	r5, #0
	ldrh	r6, [r3, #20]
	muls	r6, r1, r6
.L1779:
	cmp	r5, r6
	blt	.L1781
	movs	r4, #0
	b	.L1778
.L1781:
	ldr	r4, [r7, #200]
	cbz	r4, .L1778
	ldrh	r3, [r4, #4]
	movs	r1, #1
	mov	r0, r7
	add	r5, r5, r3
	subs	r5, r5, #1
	bl	gc_one
	mov	r4, r0
	cmp	r0, #0
	beq	.L1779
	cmp	r0, #1
	bne	.L1778
	ldr	r0, .L1785
	bl	NAND_Print_DBG
.L1778:
	mov	r0, r4
	pop	{r3, r4, r5, r6, r7, pc}
.L1786:
	.align	2
.L1785:
	.word	.LC306
	.size	gc_win_block, .-gc_win_block
	.section	.text.garbage_collect,"ax",%progbits
	.align	1
	.global	garbage_collect
	.syntax unified
	.thumb
	.thumb_func
	.fpu softvfp
	.type	garbage_collect, %function
garbage_collect:
	@ args = 0, pretend = 0, frame = 0
	@ frame_needed = 0, uses_anonymous_args = 0
	ldrh	r3, [r0, #380]
	ldrh	r1, [r0, #22]
	push	{r4, lr}
	mov	r4, r0
	cmp	r3, r1
	itt	cs
	movcs	r3, #0
	strhcs	r3, [r0, #384]	@ movhi
	ldrh	r0, [r0, #384]
	cbz	r0, .L1789
.L1806:
	movs	r0, #0
	b	.L1787
.L1789:
	ldrh	r3, [r4, #382]
	cmp	r3, r1
	bls	.L1791
	ldrh	r3, [r4, #386]
	cbnz	r3, .L1793
	ldr	r3, [r4, #200]
	cbnz	r3, .L1794
	ldr	r0, .L1808
	bl	NAND_Print
.L1807:
	movs	r0, #1
.L1787:
	pop	{r4, pc}
.L1794:
	ldrh	r2, [r3, #4]
	ldr	r3, [r4, #4]
	ldrh	r3, [r3, #20]
	cmp	r2, r3, lsr #1
	bcc	.L1795
	movs	r3, #4
.L1805:
	strh	r3, [r4, #388]	@ movhi
	ldr	r3, [r4, #1972]
	ldrh	r2, [r4, #22]
	ldrh	r3, [r3, #16]
	cmp	r2, r3
	itt	ls
	movwls	r3, #65535
	strhls	r3, [r4, #388]	@ movhi
.L1793:
	ldrh	r2, [r4, #388]
	movw	r3, #65535
	cmp	r2, r3
	beq	.L1800
	movs	r1, #0
	mov	r0, r4
	bl	garbage_collect_first
	cmp	r0, #0
	beq	.L1806
	ldrh	r1, [r4, #22]
	ldr	r0, .L1808+4
	bl	NAND_Print
	b	.L1807
.L1795:
	cmp	r2, r3, lsr #2
	bcc	.L1797
	movs	r3, #8
	b	.L1805
.L1797:
	cmp	r2, r3, lsr #3
	bcc	.L1798
	movs	r3, #16
	b	.L1805
.L1798:
	ldr	r0, .L1808+8
	bl	NAND_Print_DBG
	movw	r3, #65535
	b	.L1805
.L1800:
	movs	r1, #1
	mov	r0, r4
	bl	gc_win_block
	b	.L1806
.L1791:
	movs	r3, #1
	strh	r0, [r4, #388]	@ movhi
	str	r3, [r4, #384]
	b	.L1787
.L1809:
	.align	2
.L1808:
	.word	.LC307
	.word	.LC309
	.word	.LC308
	.size	garbage_collect, .-garbage_collect
	.section	.text.do_prio_gc,"ax",%progbits
	.align	1
	.global	do_prio_gc
	.syntax unified
	.thumb
	.thumb_func
	.fpu softvfp
	.type	do_prio_gc, %function
do_prio_gc:
	@ args = 0, pretend = 0, frame = 0
	@ frame_needed = 0, uses_anonymous_args = 0
	ldr	r1, [r0, #564]
	push	{r3, r4, r5, r6, r7, lr}
	mov	r6, r0
	cmp	r1, #0
	beq	.L1823
	add	r4, r0, #392
	mov	r0, r4
	bl	del_from_gc_list
	ldr	r7, [r0, #4]
	mov	r5, r0
	mov	r0, r6
	mov	r1, r7
	bl	is_current_blcok
	cmp	r0, #1
	bne	.L1812
	mov	r1, r5
	mov	r0, r4
	bl	add_to_gc_list_tail
.L1824:
	movs	r4, #0
.L1810:
	mov	r0, r4
	pop	{r3, r4, r5, r6, r7, pc}
.L1812:
	cbnz	r7, .L1813
	ldr	r0, .L1827
	bl	NAND_Print_DBG
	strh	r7, [r5, #2]	@ movhi
	strh	r7, [r6, #394]	@ movhi
	str	r7, [r5, #4]
	b	.L1824
.L1813:
	mov	r0, r6
	bl	is_block_in_invalid_list
	ldrh	r2, [r5, #2]
	mov	r4, r0
	cbnz	r0, .L1814
	ldrh	r1, [r7]
	ldr	r0, .L1827+4
.L1825:
	bl	NAND_Print_DBG
	b	.L1826
.L1814:
	cmp	r2, #5
	strh	r2, [r6, #394]	@ movhi
	bne	.L1815
	movs	r4, #0
	movs	r2, #4
	mov	r0, r6
	bl	garbage_collect_first
.L1826:
	strh	r4, [r5, #2]	@ movhi
	strh	r4, [r6, #394]	@ movhi
	str	r4, [r5, #4]
	b	.L1810
.L1815:
	movw	r2, #65535
	mov	r0, r6
	bl	garbage_collect_first
	ldr	r3, [r5, #4]
	mov	r4, r0
	cbz	r0, .L1816
	ldrh	r1, [r3]
	movs	r4, #1
	ldrh	r2, [r5, #2]
	ldr	r0, .L1827+8
	bl	NAND_Print
	movs	r3, #0
	strh	r3, [r5, #2]	@ movhi
	strh	r3, [r6, #394]	@ movhi
	str	r3, [r5, #4]
	b	.L1810
.L1816:
	mov	r1, r3
	mov	r0, r6
	bl	is_block_in_free_list
	cbnz	r0, .L1817
	ldrh	r2, [r5, #2]
	ldrh	r1, [r3]
	ldr	r0, .L1827+12
	b	.L1825
.L1817:
	mov	r0, r6
	bl	out_phy_block_from_free_list_by_block
	mov	r1, r0
	cbnz	r0, .L1818
	ldr	r3, [r5, #4]
	ldrh	r2, [r5, #2]
	ldr	r0, .L1827+16
	ldrh	r1, [r3]
	b	.L1825
.L1818:
	ldrh	r2, [r0, #6]
	mov	r0, r6
	adds	r2, r2, #1
	uxth	r2, r2
	bl	erase_block
	movs	r3, #0
	strh	r3, [r5, #2]	@ movhi
	strh	r3, [r6, #394]	@ movhi
	str	r3, [r5, #4]
	b	.L1810
.L1823:
	mov	r4, r1
	b	.L1810
.L1828:
	.align	2
.L1827:
	.word	.LC310
	.word	.LC311
	.word	.LC312
	.word	.LC313
	.word	.LC314
	.size	do_prio_gc, .-do_prio_gc
	.section	.text._nftl_nand_read_logic_page,"ax",%progbits
	.align	1
	.global	_nftl_nand_read_logic_page
	.syntax unified
	.thumb
	.thumb_func
	.fpu softvfp
	.type	_nftl_nand_read_logic_page, %function
_nftl_nand_read_logic_page:
	@ args = 0, pretend = 0, frame = 32
	@ frame_needed = 0, uses_anonymous_args = 0
	push	{r4, r5, r6, r7, lr}
	sub	sp, sp, #36
	mov	r4, r0
	mov	r7, r1
	mov	r6, r2
	bl	get_logic_page_map
	mov	r5, r0
	cbnz	r0, .L1830
	movs	r6, #1
	ldr	r0, .L1834
	bl	NAND_Print_DBG
.L1829:
	mov	r0, r6
	add	sp, sp, #36
	@ sp needed
	pop	{r4, r5, r6, r7, pc}
.L1830:
	ldrh	r2, [r0, #2]
	movw	r1, #65535
	ldr	r3, [r4, #4]
	cmp	r2, r1
	bne	.L1832
	mov	r0, r6
	ldrh	r2, [r3, #26]
	movs	r1, #255
	movs	r6, #0
	bl	NAND_Memset
	b	.L1829
.L1832:
	ldrb	r1, [r0, #1]	@ zero_extendqisi2
	ldr	r0, [r4, #364]
	bfi	r1, r0, #4, #1
	mov	r0, r4
	strb	r1, [r5, #1]
	ldrh	r1, [r3, #28]
	ldrh	r3, [r5]
	str	r6, [sp, #24]
	strh	r1, [sp, #20]	@ movhi
	add	r1, sp, #16
	ubfx	r3, r3, #0, #12
	strh	r2, [sp, #18]	@ movhi
	strh	r3, [sp, #16]	@ movhi
	str	sp, [sp, #28]
	ldr	r3, [r4, #1980]
	blx	r3
	adds	r3, r0, #2
	mov	r6, r0
	bne	.L1833
	ldr	r3, [r4, #4]
	movs	r0, #36
	ldrh	r1, [r5, #2]
	movs	r2, #1
	ldr	r3, [r3, #32]
	mla	r1, r0, r1, r3
	mov	r0, r4
	bl	add_prio_gc
	mov	r1, r7
	ldr	r0, .L1834+4
	bl	NAND_Print
.L1833:
	mov	r0, r4
	bl	do_prio_gc
	b	.L1829
.L1835:
	.align	2
.L1834:
	.word	.LC237
	.word	.LC315
	.size	_nftl_nand_read_logic_page, .-_nftl_nand_read_logic_page
	.section	.text.prio_gc_all,"ax",%progbits
	.align	1
	.global	prio_gc_all
	.syntax unified
	.thumb
	.thumb_func
	.fpu softvfp
	.type	prio_gc_all, %function
prio_gc_all:
	@ args = 0, pretend = 0, frame = 0
	@ frame_needed = 0, uses_anonymous_args = 0
	push	{r4, r5, r6, lr}
	mov	r6, r0
	movs	r5, #10
	movs	r4, #0
.L1837:
	mov	r0, r6
	bl	do_prio_gc
	subs	r5, r5, #1
	orr	r4, r4, r0
	bne	.L1837
	mov	r0, r4
	pop	{r4, r5, r6, pc}
	.size	prio_gc_all, .-prio_gc_all
	.section	.text.done_static_wear_leveling,"ax",%progbits
	.align	1
	.global	done_static_wear_leveling
	.syntax unified
	.thumb
	.thumb_func
	.fpu softvfp
	.type	done_static_wear_leveling, %function
done_static_wear_leveling:
	@ args = 0, pretend = 0, frame = 0
	@ frame_needed = 0, uses_anonymous_args = 0
	push	{r4, r5, r6, lr}
	mov	r4, r0
	ldr	r5, [r0, #580]
	cbz	r5, .L1845
	mov	r1, r5
	bl	is_current_blcok
	cmp	r0, #1
	bne	.L1841
	movs	r3, #0
	str	r3, [r4, #580]
.L1845:
	movs	r5, #2
	b	.L1839
.L1841:
	mov	r0, r4
	bl	is_block_in_invalid_list
	ldrh	r1, [r5]
	mov	r6, r0
	cbnz	r0, .L1842
	ldr	r0, .L1846
	bl	NAND_Print_DBG
	str	r6, [r4, #580]
	b	.L1845
.L1842:
	ldrh	r2, [r5, #6]
	movs	r5, #1
	ldr	r0, .L1846+4
	bl	NAND_Print_DBG
	ldr	r2, [r4, #1968]
	mov	r0, r4
	strh	r5, [r4, #578]	@ movhi
	ldr	r3, [r2, #128]
	add	r3, r3, r5
	str	r3, [r2, #128]
	bl	fill_no_use_data_to_current_block
	movw	r2, #65535
	ldr	r1, [r4, #580]
	mov	r0, r4
	bl	garbage_collect_first
	ldr	r2, [r4, #580]
	mov	r3, r0
	cbz	r0, .L1843
	ldrh	r1, [r2]
	ldr	r0, .L1846+8
	bl	NAND_Print
	movs	r3, #0
	str	r3, [r4, #580]
.L1839:
	mov	r0, r5
	pop	{r4, r5, r6, pc}
.L1843:
	strh	r0, [r4, #578]	@ movhi
	mov	r1, r2
	mov	r0, r4
	bl	is_block_in_free_list
	mov	r5, r0
	cbnz	r0, .L1844
	ldrh	r1, [r2]
	ldr	r0, .L1846+12
	bl	NAND_Print
	str	r5, [r4, #580]
	b	.L1839
.L1844:
	str	r3, [r4, #580]
	mov	r5, r3
	b	.L1839
.L1847:
	.align	2
.L1846:
	.word	.LC316
	.word	.LC317
	.word	.LC318
	.word	.LC319
	.size	done_static_wear_leveling, .-done_static_wear_leveling
	.section	.text.do_static_wear_leveling,"ax",%progbits
	.align	1
	.global	do_static_wear_leveling
	.syntax unified
	.thumb
	.thumb_func
	.fpu softvfp
	.type	do_static_wear_leveling, %function
do_static_wear_leveling:
	@ args = 0, pretend = 0, frame = 0
	@ frame_needed = 0, uses_anonymous_args = 0
	ldr	r3, [r0, #1972]
	push	{r4, lr}
	mov	r4, r0
	ldrh	r3, [r3, #6]
	cbnz	r3, .L1849
.L1851:
	movs	r0, #0
.L1848:
	pop	{r4, pc}
.L1849:
	ldrh	r3, [r0, #384]
	cbz	r3, .L1852
	bl	check_static_wear_leveling
	cmp	r0, #0
	bne	.L1851
	mov	r0, r4
	pop	{r4, lr}
	b	done_static_wear_leveling
.L1852:
	movs	r0, #1
	b	.L1848
	.size	do_static_wear_leveling, .-do_static_wear_leveling
	.section	.text.static_wear_leveling,"ax",%progbits
	.align	1
	.global	static_wear_leveling
	.syntax unified
	.thumb
	.thumb_func
	.fpu softvfp
	.type	static_wear_leveling, %function
static_wear_leveling:
	@ args = 0, pretend = 0, frame = 0
	@ frame_needed = 0, uses_anonymous_args = 0
	ldrh	r2, [r0, #22]
	ldrh	r3, [r0, #380]
	push	{r4, lr}
	mov	r4, r0
	cmp	r2, r3
	bls	.L1855
	ldr	r3, [r0, #368]
	cmp	r3, #19
	bls	.L1855
	ldr	r2, [r0, #1968]
	ldr	r3, [r2, #180]
	adds	r3, r3, #1
	str	r3, [r2, #180]
	bl	get_block_min_count_from_invalid_list
	movs	r2, #5
	mov	r1, r0
	mov	r0, r4
	bl	add_prio_gc
	cbnz	r0, .L1857
	ldr	r3, [r4, #372]
	adds	r3, r3, #4
	str	r3, [r4, #372]
.L1857:
	ldr	r3, [r4, #4]
	ldr	r2, [r4, #372]
	ldrh	r3, [r3, #20]
	cmp	r2, r3
	itt	cs
	movcs	r3, #0
	strdcs	r3, r3, [r4, #368]
.L1855:
	movs	r0, #0
	pop	{r4, pc}
	.size	static_wear_leveling, .-static_wear_leveling
	.section	.text.power_on_gc,"ax",%progbits
	.align	1
	.global	power_on_gc
	.syntax unified
	.thumb
	.thumb_func
	.fpu softvfp
	.type	power_on_gc, %function
power_on_gc:
	@ args = 0, pretend = 0, frame = 0
	@ frame_needed = 0, uses_anonymous_args = 0
	push	{r4, r5, r6, r7, r8, r10, lr}
	mov	r4, r0
	ldrh	r6, [r0, #22]
	sub	sp, sp, #20
	ldrh	r7, [r0, #380]
	ldr	r3, [r0, #4]
	cmp	r6, r7
	ldrh	r5, [r3, #20]
	bcc	.L1859
	ldrb	r8, [r3, #8]	@ zero_extendqisi2
	ldrh	r10, [r0, #382]
	ldr	r0, [r0, #32]
	mov	r1, r8
	bl	__aeabi_uidiv
	mov	r1, r5
	bl	__aeabi_uidiv
	mov	r1, r8
	str	r0, [sp, #8]
	ldr	r0, [r4, #24]
	bl	__aeabi_uidiv
	mov	r1, r5
	bl	__aeabi_uidiv
	ldrh	r3, [r4, #18]
	mov	r2, r7
	str	r0, [sp, #4]
	mov	r1, r6
	ldr	r0, .L1869
	str	r3, [sp]
	mov	r3, r10
	bl	NAND_Print_DBG
.L1860:
	movs	r0, #0
	add	sp, sp, #20
	@ sp needed
	pop	{r4, r5, r6, r7, r8, r10, pc}
.L1859:
	subs	r5, r5, #10
	movs	r7, #0
	uxth	r5, r5
.L1861:
	ldrh	r2, [r4, #22]
	uxth	r6, r7
	ldrh	r3, [r4, #380]
	cmp	r2, r3
	bcs	.L1864
	mov	r1, r5
	mov	r0, r4
	bl	gc_one
	cmp	r0, #8
	add	r7, r7, #1
	bne	.L1861
.L1864:
	ldr	r3, [r4, #4]
	ldr	r7, .L1869+4
	ldrh	r5, [r3, #20]
	lsrs	r5, r5, #3
.L1862:
	ldrh	r1, [r4, #22]
	cmp	r1, #4
	bhi	.L1867
	mov	r0, r7
	bl	NAND_Print_DBG
	mov	r1, r5
	mov	r0, r4
	bl	gc_one
	cmp	r0, #8
	bne	.L1865
.L1867:
	mov	r1, r6
	ldr	r0, .L1869+8
	bl	NAND_Print_DBG
	b	.L1860
.L1865:
	adds	r6, r6, #1
	uxth	r6, r6
	b	.L1862
.L1870:
	.align	2
.L1869:
	.word	.LC320
	.word	.LC321
	.word	.LC322
	.size	power_on_gc, .-power_on_gc
	.section	.text.all_time_gc,"ax",%progbits
	.align	1
	.global	all_time_gc
	.syntax unified
	.thumb
	.thumb_func
	.fpu softvfp
	.type	all_time_gc, %function
all_time_gc:
	@ args = 0, pretend = 0, frame = 0
	@ frame_needed = 0, uses_anonymous_args = 0
	ldrh	r1, [r0, #22]
	ldrh	r2, [r0, #382]
	push	{r3, lr}
	cmp	r1, r2
	bcs	.L1872
	ldrh	r2, [r0, #384]
	cbz	r2, .L1872
	ldr	r3, [r0, #4]
	ldrh	r1, [r3, #20]
	subs	r1, r1, #3
	uxth	r1, r1
	bl	gc_one
.L1872:
	movs	r0, #0
	pop	{r3, pc}
	.size	all_time_gc, .-all_time_gc
	.section	.text._nftl_nand_write_logic_page,"ax",%progbits
	.align	1
	.global	_nftl_nand_write_logic_page
	.syntax unified
	.thumb
	.thumb_func
	.fpu softvfp
	.type	_nftl_nand_write_logic_page, %function
_nftl_nand_write_logic_page:
	@ args = 0, pretend = 0, frame = 0
	@ frame_needed = 0, uses_anonymous_args = 0
	push	{r4, r5, r6, lr}
	mov	r5, r0
	mov	r6, r1
	bl	nand_write_logic_page_no_gc
	mov	r4, r0
	cbz	r0, .L1877
	movs	r4, #1
	mov	r1, r6
	ldr	r0, .L1879
	bl	NAND_Print
.L1876:
	mov	r0, r4
	pop	{r4, r5, r6, pc}
.L1877:
	mov	r0, r5
	bl	garbage_collect
	mov	r0, r5
	bl	all_time_gc
	mov	r0, r5
	bl	do_prio_gc
	b	.L1876
.L1880:
	.align	2
.L1879:
	.word	.LC323
	.size	_nftl_nand_write_logic_page, .-_nftl_nand_write_logic_page
	.section	.text.shutdown_on_gc,"ax",%progbits
	.align	1
	.global	shutdown_on_gc
	.syntax unified
	.thumb
	.thumb_func
	.fpu softvfp
	.type	shutdown_on_gc, %function
shutdown_on_gc:
	@ args = 0, pretend = 0, frame = 0
	@ frame_needed = 0, uses_anonymous_args = 0
	ldrh	r2, [r0, #382]
	push	{r4, r5, lr}
	sub	sp, sp, #20
	ldrh	r3, [r0, #384]
	mov	r4, r0
	str	r2, [sp, #8]
	ldrh	r2, [r0, #380]
	ldr	r5, .L1889
	ldr	r1, .L1889+4
	str	r2, [sp, #4]
	ldrh	r2, [r0, #22]
	str	r2, [sp]
	ldr	r2, [r0, #200]
	ldr	r0, .L1889+8
	bl	NAND_Print_DBG
.L1882:
	ldrh	r1, [r4, #22]
	ldrh	r3, [r4, #382]
.L1885:
	cmp	r1, r3
	bhi	.L1884
	ldr	r2, [r4, #200]
	cbnz	r2, .L1883
	ldrh	r2, [r4, #380]
	ldr	r0, .L1889+12
	bl	NAND_Print
.L1884:
	movs	r0, #0
	add	sp, sp, #20
	@ sp needed
	pop	{r4, r5, pc}
.L1883:
	ldrh	r2, [r4, #384]
	cmp	r2, #0
	beq	.L1885
	movs	r1, #0
	mov	r0, r4
	bl	gc_one
	ldrh	r3, [r4, #382]
	mov	r0, r5
	ldrh	r2, [r4, #380]
	ldrh	r1, [r4, #22]
	bl	NAND_Print_DBG
	b	.L1882
.L1890:
	.align	2
.L1889:
	.word	.LC326
	.word	.LANCHOR19
	.word	.LC324
	.word	.LC325
	.size	shutdown_on_gc, .-shutdown_on_gc
	.section	.text.nftl_get_gc_info,"ax",%progbits
	.align	1
	.global	nftl_get_gc_info
	.syntax unified
	.thumb
	.thumb_func
	.fpu softvfp
	.type	nftl_get_gc_info, %function
nftl_get_gc_info:
	@ args = 0, pretend = 0, frame = 0
	@ frame_needed = 0, uses_anonymous_args = 0
	push	{r4, r5, r6, r7, r8, r10, fp, lr}
	mov	r8, r2
	movs	r2, #0
	mov	r5, r0
	mov	r7, r1
	sub	sp, sp, #32
	ldr	ip, [r0, #4]
	mov	r6, r2
	ldr	r4, [r0, #200]
	mov	r1, r2
	mov	r0, r2
.L1892:
	cbnz	r4, .L1894
	ldr	ip, [r5, #1968]
	mov	fp, #10
	ldrh	r3, [r5, #22]
	strd	r1, r0, [sp, #16]
	mov	r1, r8
	strd	r6, r2, [sp, #24]
	mov	r0, r7
	ldr	r2, [ip, #124]
	ldr	r10, .L1905+4
	str	r2, [sp, #12]
	ldr	r2, [ip, #120]
	str	r2, [sp, #8]
	ldrh	r2, [r5, #382]
	str	r2, [sp, #4]
	ldrh	r2, [r5, #380]
	str	r2, [sp]
	ldr	r2, .L1905
	bl	NAND_Snprintf
	ldr	r5, [r5, #200]
	mov	r6, r0
.L1895:
	sub	r1, r8, r6
	adds	r0, r7, r6
	cmp	r1, #1
	bne	.L1898
.L1896:
	movs	r3, #10
	strb	r3, [r0, #-1]
	mov	r0, r6
	add	sp, sp, #32
	@ sp needed
	pop	{r4, r5, r6, r7, r8, r10, fp, pc}
.L1894:
	ldrh	r3, [r4, #4]
	adds	r0, r0, #1
	ldrh	lr, [ip, #20]
	ldr	r4, [r4, #12]
	add	r2, r2, r3
	cmp	r3, lr, lsr #1
	itt	hi
	addhi	r6, r6, r3
	addhi	r1, r1, #1
	b	.L1892
.L1898:
	cmp	r5, #0
	beq	.L1896
	ldrh	r2, [r5, #4]
	cmp	r2, #0
	beq	.L1896
	ldrh	r3, [r5]
	adds	r4, r4, #1
	str	r2, [sp]
	mov	r2, r10
	bl	NAND_Snprintf
	lsls	r3, r4, #29
	add	r6, r6, r0
	itt	eq
	addeq	r3, r7, r6
	strbeq	fp, [r3, #-1]
	ldr	r5, [r5, #12]
	b	.L1895
.L1906:
	.align	2
.L1905:
	.word	.LC327
	.word	.LC328
	.size	nftl_get_gc_info, .-nftl_get_gc_info
	.section	.text.is_on_panic,"ax",%progbits
	.align	1
	.global	is_on_panic
	.syntax unified
	.thumb
	.thumb_func
	.fpu softvfp
	.type	is_on_panic, %function
is_on_panic:
	@ args = 0, pretend = 0, frame = 0
	@ frame_needed = 0, uses_anonymous_args = 0
	@ link register save eliminated.
	ldr	r3, .L1908
	ldr	r0, [r3]
	adds	r0, r0, #0
	it	ne
	movne	r0, #1
	bx	lr
.L1909:
	.align	2
.L1908:
	.word	.LANCHOR20
	.size	is_on_panic, .-is_on_panic
	.section	.text.panic_mark_enable,"ax",%progbits
	.align	1
	.global	panic_mark_enable
	.syntax unified
	.thumb
	.thumb_func
	.fpu softvfp
	.type	panic_mark_enable, %function
panic_mark_enable:
	@ args = 0, pretend = 0, frame = 0
	@ frame_needed = 0, uses_anonymous_args = 0
	@ link register save eliminated.
	ldr	r3, .L1911
	movs	r2, #1
	str	r2, [r3]
	bx	lr
.L1912:
	.align	2
.L1911:
	.word	.LANCHOR21
	.size	panic_mark_enable, .-panic_mark_enable
	.section	.text.is_panic_enable,"ax",%progbits
	.align	1
	.global	is_panic_enable
	.syntax unified
	.thumb
	.thumb_func
	.fpu softvfp
	.type	is_panic_enable, %function
is_panic_enable:
	@ args = 0, pretend = 0, frame = 0
	@ frame_needed = 0, uses_anonymous_args = 0
	@ link register save eliminated.
	ldr	r3, .L1914
	ldr	r0, [r3]
	adds	r0, r0, #0
	it	ne
	movne	r0, #1
	bx	lr
.L1915:
	.align	2
.L1914:
	.word	.LANCHOR21
	.size	is_panic_enable, .-is_panic_enable
	.section	.text.panic_init_part,"ax",%progbits
	.align	1
	.global	panic_init_part
	.syntax unified
	.thumb
	.thumb_func
	.fpu softvfp
	.type	panic_init_part, %function
panic_init_part:
	@ args = 0, pretend = 0, frame = 0
	@ frame_needed = 0, uses_anonymous_args = 0
	push	{r3, r4, r5, lr}
	mov	r3, r2
	cbz	r0, .L1920
	cbz	r2, .L1920
	ldr	r5, .L1921
	ldr	r4, [r5, #8]
	cbnz	r4, .L1920
	str	r2, [r5, #8]
	ldr	r2, .L1921+4
	strd	r0, r1, [r5]
	movs	r5, #1
	str	r5, [r2]
	mov	r2, r1
	mov	r1, r0
	ldr	r0, .L1921+8
	bl	NAND_Print
	mov	r0, r4
.L1916:
	pop	{r3, r4, r5, pc}
.L1920:
	mov	r0, #-1
	b	.L1916
.L1922:
	.align	2
.L1921:
	.word	.LANCHOR22
	.word	.LANCHOR21
	.word	.LC329
	.size	panic_init_part, .-panic_init_part
	.section	.text.panic_backup_block,"ax",%progbits
	.align	1
	.global	panic_backup_block
	.syntax unified
	.thumb
	.thumb_func
	.fpu softvfp
	.type	panic_backup_block, %function
panic_backup_block:
	@ args = 0, pretend = 0, frame = 0
	@ frame_needed = 0, uses_anonymous_args = 0
	ldr	r3, .L1934
	push	{r4, r5, r6, r7, r8, r10, fp, lr}
	mov	r8, r0
	ldr	r0, [r3]
	cbz	r0, .L1923
	ldr	r6, .L1934+4
	movs	r5, #0
	ldr	fp, .L1934+16
.L1927:
	ldr	r4, [r6, r5, lsl #2]
	lsl	r10, r5, #2
	cbz	r4, .L1925
	ldrh	r1, [r4]
	ldr	r0, .L1934+8
	bl	NAND_Print
	movs	r0, #0
.L1923:
	pop	{r4, r5, r6, r7, r8, r10, fp, pc}
.L1925:
	mov	r0, r8
	bl	out_phy_block_from_free_list
	mov	r7, r0
	cbz	r0, .L1926
	ldrh	r1, [r0]
	adds	r5, r5, #1
	mov	r0, fp
	bl	NAND_Print
	cmp	r5, #3
	str	r7, [r10, r6]
	bne	.L1927
	ldr	r3, [r8, #4]
	mov	r0, r4
	ldrh	r3, [r3, #20]
	add	r3, r3, r3, lsl #1
	str	r3, [r6, #12]
	b	.L1923
.L1926:
	ldr	r0, .L1934+12
	bl	NAND_Print
	mov	r0, #-1
	b	.L1923
.L1935:
	.align	2
.L1934:
	.word	.LANCHOR21
	.word	.LANCHOR23
	.word	.LC330
	.word	.LC332
	.word	.LC331
	.size	panic_backup_block, .-panic_backup_block
	.section	.text.panic_read,"ax",%progbits
	.align	1
	.global	panic_read
	.syntax unified
	.thumb
	.thumb_func
	.fpu softvfp
	.type	panic_read, %function
panic_read:
	@ args = 0, pretend = 0, frame = 0
	@ frame_needed = 0, uses_anonymous_args = 0
	push	{r4, r5, r6, r7, r8, r10, fp, lr}
	mov	r8, r3
	ldr	r3, .L1942
	mov	r6, r0
	mov	r5, r2
	ldr	r3, [r3]
	cbz	r3, .L1940
	ldr	r3, .L1942+4
	movs	r2, #1
	mov	r4, r1
	sub	fp, r8, r1
	str	r2, [r3]
	bic	r3, r5, #32640
	bic	r3, r3, #127
	add	r10, r3, r1
.L1938:
	cmp	r4, r10
	add	r3, fp, r4
	bne	.L1939
	ubfx	r2, r5, #0, #15
	bic	r5, r5, #32640
	bic	r5, r5, #127
	add	r3, r8, r5
	cbz	r2, .L1941
	ldr	r5, [r6, #44]
	mov	r1, r4
	mov	r0, r6
	blx	r5
	movs	r0, #0
.L1936:
	pop	{r4, r5, r6, r7, r8, r10, fp, pc}
.L1939:
	mov	r1, r4
	ldr	r7, [r6, #44]
	mov	r2, #32768
	mov	r0, r6
	blx	r7
	add	r4, r4, #32768
	b	.L1938
.L1940:
	mov	r0, #-1
	b	.L1936
.L1941:
	mov	r0, r2
	b	.L1936
.L1943:
	.align	2
.L1942:
	.word	.LANCHOR21
	.word	.LANCHOR20
	.size	panic_read, .-panic_read
	.section	.text.panic_write,"ax",%progbits
	.align	1
	.global	panic_write
	.syntax unified
	.thumb
	.thumb_func
	.fpu softvfp
	.type	panic_write, %function
panic_write:
	@ args = 0, pretend = 0, frame = 48
	@ frame_needed = 0, uses_anonymous_args = 0
	push	{r4, r5, r6, r7, r8, r10, fp, lr}
	sub	sp, sp, #56
	str	r3, [sp, #16]
	mov	fp, r2
	ldr	r3, .L1957
	str	r1, [sp, #12]
	ldr	r3, [r3]
	cbz	r3, .L1955
	ldr	r8, [r0, #12]
	movs	r2, #1
	ldr	r3, .L1957+4
	add	r0, fp, #-1
	ldr	r7, .L1957+8
	ldr	r5, [r8, #4]
	str	r2, [r3]
	ldrb	r1, [r5, #8]	@ zero_extendqisi2
	add	r0, r0, r1
	bl	__aeabi_uidiv
	ldrh	r1, [r5, #20]
	mov	r10, r0
	subs	r0, r1, #1
	add	r0, r0, r10
	bl	__aeabi_uidiv
	mov	r3, r10
	str	r0, [sp]
	mov	r2, fp
	ldr	r1, [sp, #12]
	ldr	r0, .L1957+12
	bl	NAND_Print
	ldr	r3, [r7, #12]
	subs	r3, r3, #3
	cmp	r3, r10
	bcs	.L1946
	ldrb	r1, [r5, #8]	@ zero_extendqisi2
	ldr	r0, .L1957+16
	muls	r1, r3, r1
	bl	NAND_Print
.L1955:
	mov	r0, #-1
	b	.L1944
.L1946:
	movs	r2, #16
	movs	r1, #165
	add	r0, sp, #24
	mov	r4, r7
	bl	NAND_Memset
	ldr	r3, [r7, #16]
	ldr	r6, [r7, r3, lsl #2]
.L1947:
	ldr	r3, [r7, #20]
	cmp	r10, #0
	bne	.L1950
	ldr	fp, .L1957+24
	movs	r7, #3
	add	r10, sp, #24
	movs	r3, #98
	strb	r3, [sp, #26]
.L1954:
	ldr	r2, [r4, #20]
	cmp	r2, #0
	bne	.L1951
.L1952:
	movs	r0, #0
.L1944:
	add	sp, sp, #56
	@ sp needed
	pop	{r4, r5, r6, r7, r8, r10, fp, pc}
.L1950:
	ldrh	r2, [r5, #20]
	cmp	r3, r2
	bcc	.L1948
	ldr	r3, [r7, #16]
	adds	r3, r3, #1
	str	r3, [r7, #16]
	ldr	r6, [r7, r3, lsl #2]
	movs	r3, #0
	str	r3, [r7, #20]
.L1948:
	ldr	r3, [r4, #20]
	ldrh	r1, [r6]
	cbnz	r3, .L1949
	ldrh	r2, [r5, #28]
	mov	r0, r8
	strh	r3, [sp, #40]	@ movhi
	strh	r1, [sp, #42]	@ movhi
	add	r1, sp, #40
	strh	r2, [sp, #44]	@ movhi
	strd	r3, r3, [sp, #48]
	ldr	r2, [r8, #1976]
	str	r3, [sp, #20]
	blx	r2
	ldrh	r2, [r6, #6]
	ldr	r3, [sp, #20]
	adds	r2, r2, #1
	strh	r2, [r6, #6]	@ movhi
	strh	r3, [r6, #4]	@ movhi
.L1949:
	ldr	r2, [r4, #20]
	add	r10, r10, #-1
	ldrh	r1, [r6]
	ldr	r0, .L1957+20
	bl	NAND_Print
	movw	r3, #43775
	movs	r2, #5
	strh	r3, [sp, #24]	@ movhi
	movs	r3, #99
	strb	r3, [sp, #26]
	add	r0, sp, #24
	ldrb	r3, [r8, #16]	@ zero_extendqisi2
	strb	r3, [sp, #27]
	ldr	r3, [r4, #16]
	strb	r3, [sp, #28]
	movs	r3, #2
	ldrh	r1, [r6, #6]
	bl	set_spare_data
	movs	r3, #4
	movs	r2, #7
	ldr	r1, [sp, #12]
	add	r0, sp, #24
	bl	set_spare_data
	ldrb	r1, [r5, #8]	@ zero_extendqisi2
	movs	r3, #4
	movs	r2, #11
	add	r0, sp, #24
	cmp	r1, fp
	it	cs
	movcs	r1, fp
	bl	set_spare_data
	ldrh	r3, [r5, #28]
	mov	r0, r8
	ldrh	r2, [r6]
	ldr	r1, [r4, #20]
	strh	r3, [sp, #44]	@ movhi
	ldr	r3, [sp, #16]
	strh	r1, [sp, #40]	@ movhi
	add	r1, sp, #40
	strh	r2, [sp, #42]	@ movhi
	str	r3, [sp, #48]
	add	r3, sp, #24
	str	r3, [sp, #52]
	ldr	r3, [r8, #1984]
	blx	r3
	ldr	r2, [sp, #16]
	ldrh	r3, [r5, #26]
	adds	r3, r2, r3
	ldr	r2, [sp, #12]
	str	r3, [sp, #16]
	ldrb	r3, [r5, #8]	@ zero_extendqisi2
	cmp	r3, fp
	it	cs
	movcs	r3, fp
	add	r2, r2, r3
	sub	fp, fp, r3
	ldr	r3, [r4, #12]
	str	r2, [sp, #12]
	subs	r3, r3, #1
	str	r3, [r4, #12]
	ldr	r3, [r4, #20]
	adds	r3, r3, #1
	str	r3, [r4, #20]
	b	.L1947
.L1951:
	ldrh	r3, [r5, #20]
	cmp	r2, r3
	bcs	.L1952
	ldrh	r1, [r6]
	mov	r0, fp
	bl	NAND_Print
	ldrh	r3, [r5, #28]
	mov	r0, r8
	ldrh	r2, [r6]
	ldr	r1, [r4, #20]
	strh	r3, [sp, #44]	@ movhi
	ldr	r3, [sp, #16]
	strh	r1, [sp, #40]	@ movhi
	add	r1, sp, #40
	strh	r2, [sp, #42]	@ movhi
	strd	r3, r10, [sp, #48]
	ldr	r3, [r8, #1984]
	blx	r3
	ldr	r2, [r4, #12]
	ldr	r3, [r4, #20]
	subs	r2, r2, #1
	str	r2, [r4, #12]
	adds	r3, r3, #1
	ldrh	r2, [r5, #20]
	str	r3, [r4, #20]
	cmp	r3, r2
	bcc	.L1953
	ldr	r3, [r4, #16]
	adds	r3, r3, #1
	str	r3, [r4, #16]
	ldr	r6, [r4, r3, lsl #2]
	movs	r3, #0
	str	r3, [r4, #20]
.L1953:
	subs	r7, r7, #1
	bne	.L1954
	b	.L1952
.L1958:
	.align	2
.L1957:
	.word	.LANCHOR21
	.word	.LANCHOR20
	.word	.LANCHOR23
	.word	.LC333
	.word	.LC334
	.word	.LC335
	.word	.LC336
	.size	panic_write, .-panic_write
	.section	.text.is_panic_data_page,"ax",%progbits
	.align	1
	.global	is_panic_data_page
	.syntax unified
	.thumb
	.thumb_func
	.fpu softvfp
	.type	is_panic_data_page, %function
is_panic_data_page:
	@ args = 0, pretend = 0, frame = 0
	@ frame_needed = 0, uses_anonymous_args = 0
	@ link register save eliminated.
	ldrb	r3, [r0, #1]	@ zero_extendqisi2
	cmp	r3, #170
	bne	.L1961
	ldrb	r0, [r0, #2]	@ zero_extendqisi2
	sub	r3, r0, #99
	rsbs	r0, r3, #0
	adcs	r0, r0, r3
	bx	lr
.L1961:
	movs	r0, #0
	bx	lr
	.size	is_panic_data_page, .-is_panic_data_page
	.section	.text.is_panic_dummy_page,"ax",%progbits
	.align	1
	.global	is_panic_dummy_page
	.syntax unified
	.thumb
	.thumb_func
	.fpu softvfp
	.type	is_panic_dummy_page, %function
is_panic_dummy_page:
	@ args = 0, pretend = 0, frame = 0
	@ frame_needed = 0, uses_anonymous_args = 0
	@ link register save eliminated.
	ldrb	r3, [r0, #1]	@ zero_extendqisi2
	cmp	r3, #170
	bne	.L1964
	ldrb	r0, [r0, #2]	@ zero_extendqisi2
	sub	r3, r0, #98
	rsbs	r0, r3, #0
	adcs	r0, r0, r3
	bx	lr
.L1964:
	movs	r0, #0
	bx	lr
	.size	is_panic_dummy_page, .-is_panic_dummy_page
	.section	.text.panic_recover_info,"ax",%progbits
	.align	1
	.global	panic_recover_info
	.syntax unified
	.thumb
	.thumb_func
	.fpu softvfp
	.type	panic_recover_info, %function
panic_recover_info:
	@ args = 0, pretend = 0, frame = 0
	@ frame_needed = 0, uses_anonymous_args = 0
	push	{r3, r4, r5, lr}
	mov	r5, r1
	ldrb	r4, [r0, #4]	@ zero_extendqisi2
	cmp	r4, #2
	bhi	.L1966
	ldrh	r1, [r1]
	ldr	r0, .L1968
	bl	NAND_Print
	ldr	r3, .L1968+4
	movs	r0, #0
	str	r5, [r3, r4, lsl #2]
.L1965:
	pop	{r3, r4, r5, pc}
.L1966:
	mov	r1, r4
	ldr	r0, .L1968+8
	bl	NAND_Print
	mov	r0, #-1
	b	.L1965
.L1969:
	.align	2
.L1968:
	.word	.LC337
	.word	.LANCHOR23
	.word	.LC338
	.size	panic_recover_info, .-panic_recover_info
	.section	.text.first_scan_all_blocks,"ax",%progbits
	.align	1
	.global	first_scan_all_blocks
	.syntax unified
	.thumb
	.thumb_func
	.fpu softvfp
	.type	first_scan_all_blocks, %function
first_scan_all_blocks:
	@ args = 0, pretend = 0, frame = 32
	@ frame_needed = 0, uses_anonymous_args = 0
	push	{r4, r5, r6, r7, r8, r10, fp, lr}
	sub	sp, sp, #88
	ldr	r4, [r0, #40]
	mov	r5, r0
	movs	r2, #5
	add	r0, sp, #72
	ldr	r1, .L2024
	add	r6, sp, #88
	bl	memcpy
	movs	r3, #47
	ldr	r1, .L2024+4
	strh	r3, [r6, #-32]!	@ movhi
	mov	r0, r6
	bl	nand_fdt_get_string
	cmp	r0, #0
	beq	.L1971
	movs	r2, #3
	add	r1, sp, #72
	bl	NAND_Memcmp
	mov	r7, r0
	cbnz	r0, .L1971
	ldr	r0, .L2024+8
	bl	NAND_Print_DBG
	strh	r7, [r5, #18]	@ movhi
.L1972:
	ldr	r2, [r5, #4]
	mov	r8, #0
	ldrh	r3, [r4]
	ldrh	r2, [r2, #24]
	cmp	r2, r3
	bls	.L2017
	strh	r3, [sp, #74]	@ movhi
	add	r1, sp, #72
	strh	r8, [sp, #72]	@ movhi
	mov	r0, r5
	strh	r8, [sp, #76]	@ movhi
	strd	r8, r6, [sp, #80]
	ldr	r3, [r5, #1992]
	blx	r3
	cmp	r0, #1
	bne	.L1974
	ldr	r3, [r5, #1980]
	add	r1, sp, #72
	mov	r0, r5
	blx	r3
	str	r4, [r5, #44]
	mov	r10, r0
	mov	r0, r6
	bl	get_block_used_count_from_oob
	mov	fp, r0
	mov	r0, r6
	bl	is_ftl_end_flag_page
	cmp	r0, #1
	bne	.L1975
	ldrh	r1, [r4]
	ldr	r0, .L2024+12
	bl	NAND_Print
.L2017:
	mov	r0, r8
	b	.L1970
.L1971:
	mov	r0, #-1
.L1970:
	add	sp, sp, #88
	@ sp needed
	pop	{r4, r5, r6, r7, r8, r10, fp, pc}
.L1975:
	mov	r0, r6
	bl	is_panic_data_page
	cmp	r0, #1
	bne	.L1976
	ldrb	r2, [sp, #59]	@ zero_extendqisi2
	ldrb	r3, [r5, #16]	@ zero_extendqisi2
	cmp	r2, r3
	bne	.L1977
	mov	r1, r4
	mov	r0, r6
	bl	panic_recover_info
.L1977:
	adds	r4, r4, #36
	b	.L1972
.L1976:
	mov	r0, r6
	bl	is_nouse_page
	cmp	r0, #1
	bne	.L1978
	ldrh	r3, [r5, #18]
	mov	r1, r4
	mov	r0, r5
	adds	r3, r3, #1
	strh	r3, [r5, #18]	@ movhi
	strh	r8, [r4, #2]	@ movhi
	strh	r8, [r4, #6]	@ movhi
	strh	r8, [r4, #4]	@ movhi
	bl	put_phy_block_to_free_list
	b	.L1977
.L1978:
	mov	r0, r6
	bl	is_ftl_logic_page_data
	cmp	r0, #1
	beq	.L1979
	mov	r0, r6
	bl	is_function_info_page
	cmp	r0, #1
	bne	.L1980
.L1979:
	cmp	r10, #0
	beq	.L1981
	cmp	r10, #10
	bne	.L1980
.L1981:
	ldrh	r3, [r5, #18]
	mov	r8, #1
	mov	r0, r6
	adds	r3, r3, #1
	strh	r3, [r5, #18]	@ movhi
	strh	r8, [r4, #2]	@ movhi
	bl	get_erase_count_from_oob
	movs	r3, #0
	strh	r0, [r4, #6]	@ movhi
	mov	r7, r0
	strh	r3, [r4, #4]	@ movhi
	str	fp, [r4, #8]
	mov	r1, r4
	strd	r3, r3, [r4, #28]
	mov	r0, r5
	bl	add_block_count_list
	cmp	r10, #10
	bne	.L1977
	ldr	r3, [r5, #1972]
	ldrh	r3, [r3, #4]
	cmp	r3, #0
	beq	.L1977
	mov	r2, r8
	mov	r0, r5
	bl	add_prio_gc
	ldr	r0, .L2024+16
	bl	NAND_Print_DBG
	b	.L1977
.L1980:
	mov	r0, r6
	bl	is_ftl_start_flag_page
	cmp	r0, #1
	bne	.L1982
	ldr	r0, .L2024+20
.L2016:
	bl	NAND_Print
	b	.L1977
.L1982:
	mov	r0, r6
	bl	is_fill_page
	cmp	r0, #1
	bne	.L1983
	ldrh	r3, [r5, #18]
	mov	r0, r6
	adds	r3, r3, #1
	strh	r3, [r5, #18]	@ movhi
	movs	r3, #0
	strh	r3, [r4, #2]	@ movhi
	bl	get_erase_count_from_oob
	ldr	r3, [r5, #4]
	mov	r1, r4
	strh	r0, [r4, #6]	@ movhi
	mov	r7, r0
	mov	r0, r5
	ldrh	r3, [r3, #20]
	strh	r3, [r4, #4]	@ movhi
	bl	put_phy_block_to_free_list
	ldrh	r1, [r4]
	ldr	r0, .L2024+24
	bl	NAND_Print_DBG
	b	.L1977
.L1983:
	ldrb	r0, [sp, #71]	@ zero_extendqisi2
	ldrh	r1, [r4]
	ldrb	r3, [sp, #57]	@ zero_extendqisi2
	str	r0, [sp, #52]
	ldrb	r0, [sp, #70]	@ zero_extendqisi2
	ldrb	r2, [sp, #56]	@ zero_extendqisi2
	str	r0, [sp, #48]
	ldrb	r0, [sp, #69]	@ zero_extendqisi2
	str	r0, [sp, #44]
	ldrb	r0, [sp, #68]	@ zero_extendqisi2
	str	r0, [sp, #40]
	ldrb	r0, [sp, #67]	@ zero_extendqisi2
	str	r0, [sp, #36]
	ldrb	r0, [sp, #66]	@ zero_extendqisi2
	str	r0, [sp, #32]
	ldrb	r0, [sp, #65]	@ zero_extendqisi2
	str	r0, [sp, #28]
	ldrb	r0, [sp, #64]	@ zero_extendqisi2
	str	r0, [sp, #24]
	ldrb	r0, [sp, #63]	@ zero_extendqisi2
	str	r0, [sp, #20]
	ldrb	r0, [sp, #62]	@ zero_extendqisi2
	str	r0, [sp, #16]
	ldrb	r0, [sp, #61]	@ zero_extendqisi2
	str	r0, [sp, #12]
	ldrb	r0, [sp, #60]	@ zero_extendqisi2
	str	r0, [sp, #8]
	ldrb	r0, [sp, #59]	@ zero_extendqisi2
	str	r0, [sp, #4]
	ldrb	r0, [sp, #58]	@ zero_extendqisi2
	str	r0, [sp]
	ldr	r0, .L2024+28
	bl	NAND_Print
	uxth	r2, r7
	adds	r3, r2, #1
	strh	r3, [r4, #6]	@ movhi
	cmp	r10, #0
	beq	.L1984
	cmp	r10, #10
	bne	.L1985
.L1984:
	ldrb	r3, [sp, #56]	@ zero_extendqisi2
	cmp	r3, #255
	beq	.L1986
	ldrh	r1, [r4]
	ldr	r0, .L2024+32
	bl	NAND_Print
	movs	r3, #255
	strh	r3, [r4, #2]	@ movhi
.L2015:
	ldrh	r3, [r5, #20]
	adds	r3, r3, #1
	strh	r3, [r5, #20]	@ movhi
	b	.L1977
.L1986:
	adds	r2, r2, #2
.L2022:
	uxth	r2, r2
	mov	r1, r4
	mov	r0, r5
	bl	erase_block
.L2019:
	cmp	r0, #0
	bne	.L1977
	ldrh	r3, [r5, #18]
	adds	r3, r3, #1
	strh	r3, [r5, #18]	@ movhi
	b	.L1977
.L1985:
	ldrh	r1, [r4]
	ldr	r0, .L2024+36
	bl	NAND_Print
	ldr	r3, [r5, #4]
	add	r1, sp, #72
	ldrh	r2, [r4]
	mov	r0, r5
	ldrh	r3, [r3, #20]
	strh	r2, [sp, #74]	@ movhi
	subs	r3, r3, #1
	strh	r3, [sp, #72]	@ movhi
	movs	r3, #0
	strh	r3, [sp, #76]	@ movhi
	strd	r3, r6, [sp, #80]
	ldr	r3, [r5, #1980]
	blx	r3
	ldrh	r8, [r4]
	mov	fp, r0
	cbz	r0, .L1987
	cmp	r0, #10
	bne	.L1988
.L1987:
	mov	r0, r6
	bl	is_phy_mapping_page
	cmp	r0, #1
	mov	r10, r0
	bne	.L1989
	mov	r1, r8
	ldr	r0, .L2024+40
	bl	NAND_Print
	mov	r0, r6
	bl	get_block_used_count_from_oob
	ldrh	r3, [r5, #18]
	mov	r8, r0
	mov	r0, r6
	adds	r3, r3, #1
	strh	r3, [r5, #18]	@ movhi
	strh	r10, [r4, #2]	@ movhi
	bl	get_erase_count_from_oob
	movs	r3, #0
	strh	r0, [r4, #6]	@ movhi
	mov	r7, r0
	strh	r3, [r4, #4]	@ movhi
	str	r8, [r4, #8]
	mov	r1, r4
	strd	r3, r3, [r4, #28]
	mov	r0, r5
	bl	add_block_count_list
	cmp	fp, #10
	bne	.L1977
	ldr	r3, [r5, #1972]
	ldrh	r3, [r3, #4]
	cmp	r3, #0
	beq	.L1977
	mov	r2, r10
	mov	r0, r5
	bl	add_prio_gc
	ldr	r0, .L2024+44
	b	.L2016
.L1989:
	mov	r0, r6
	bl	is_nouse_page
	cmp	r0, #1
	mov	r1, r8
	ite	eq
	ldreq	r0, .L2024+48
	ldrne	r0, .L2024+52
	bl	NAND_Print
	ldrh	r2, [r4, #6]
	adds	r2, r2, #1
	b	.L2022
.L2025:
	.align	2
.L2024:
	.word	.LANCHOR17+16
	.word	.LANCHOR24
	.word	.LC339
	.word	.LC340
	.word	.LC341
	.word	.LC342
	.word	.LC343
	.word	.LC344
	.word	.LC345
	.word	.LC346
	.word	.LC347
	.word	.LC348
	.word	.LC349
	.word	.LC350
.L1988:
	mov	r1, r8
	ldr	r0, .L2026
	bl	NAND_Print
	ldrb	r0, [sp, #71]	@ zero_extendqisi2
	ldrh	r1, [r4]
	ldrb	r3, [sp, #57]	@ zero_extendqisi2
	str	r0, [sp, #52]
	ldrb	r0, [sp, #70]	@ zero_extendqisi2
	ldrb	r2, [sp, #56]	@ zero_extendqisi2
	str	r0, [sp, #48]
	ldrb	r0, [sp, #69]	@ zero_extendqisi2
	str	r0, [sp, #44]
	ldrb	r0, [sp, #68]	@ zero_extendqisi2
	str	r0, [sp, #40]
	ldrb	r0, [sp, #67]	@ zero_extendqisi2
	str	r0, [sp, #36]
	ldrb	r0, [sp, #66]	@ zero_extendqisi2
	str	r0, [sp, #32]
	ldrb	r0, [sp, #65]	@ zero_extendqisi2
	str	r0, [sp, #28]
	ldrb	r0, [sp, #64]	@ zero_extendqisi2
	str	r0, [sp, #24]
	ldrb	r0, [sp, #63]	@ zero_extendqisi2
	str	r0, [sp, #20]
	ldrb	r0, [sp, #62]	@ zero_extendqisi2
	str	r0, [sp, #16]
	ldrb	r0, [sp, #61]	@ zero_extendqisi2
	str	r0, [sp, #12]
	ldrb	r0, [sp, #60]	@ zero_extendqisi2
	str	r0, [sp, #8]
	ldrb	r0, [sp, #59]	@ zero_extendqisi2
	str	r0, [sp, #4]
	ldrb	r0, [sp, #58]	@ zero_extendqisi2
	str	r0, [sp]
	ldr	r0, .L2026+4
	bl	NAND_Print
	mov	r1, r4
	mov	r0, r5
	bl	torture_block_and_mark
	b	.L2019
.L1974:
	ldrh	r1, [r4]
	ldr	r0, .L2026+8
	bl	NAND_Print
	movs	r3, #255
	strh	r8, [r4, #6]	@ movhi
	strh	r3, [r4, #2]	@ movhi
	b	.L2015
.L2027:
	.align	2
.L2026:
	.word	.LC351
	.word	.LC352
	.word	.LC353
	.size	first_scan_all_blocks, .-first_scan_all_blocks
	.section	.text.build_zone_list,"ax",%progbits
	.align	1
	.global	build_zone_list
	.syntax unified
	.thumb
	.thumb_func
	.fpu softvfp
	.type	build_zone_list, %function
build_zone_list:
	@ args = 0, pretend = 0, frame = 0
	@ frame_needed = 0, uses_anonymous_args = 0
	push	{r3, r4, r5, lr}
	mov	r4, r0
	ldr	r0, .L2033
	bl	NAND_Print_DBG
	mov	r0, r4
	bl	first_scan_all_blocks
	ldrh	r1, [r4, #18]
	mov	r0, r4
	bl	init_zone_after_first_scan
	cbz	r0, .L2029
	ldr	r0, .L2033+4
.L2032:
	movs	r5, #1
	bl	NAND_Print
.L2028:
	mov	r0, r5
	pop	{r3, r4, r5, pc}
.L2029:
	mov	r0, r4
	bl	second_scan_all_blocks
	mov	r5, r0
	cbz	r0, .L2031
	ldr	r0, .L2033+8
	b	.L2032
.L2031:
	mov	r0, r4
	bl	power_on_gc
	mov	r0, r4
	bl	print_nftl_zone
	ldr	r0, .L2033+12
	bl	NAND_Print
	mov	r0, r4
	bl	panic_backup_block
	b	.L2028
.L2034:
	.align	2
.L2033:
	.word	.LC354
	.word	.LC355
	.word	.LC356
	.word	.LC357
	.size	build_zone_list, .-build_zone_list
	.section	.text.nand_find_zone_table,"ax",%progbits
	.align	1
	.global	nand_find_zone_table
	.syntax unified
	.thumb
	.thumb_func
	.fpu softvfp
	.type	nand_find_zone_table, %function
nand_find_zone_table:
	@ args = 0, pretend = 0, frame = 0
	@ frame_needed = 0, uses_anonymous_args = 0
	push	{r3, r4, r5, lr}
	mov	r4, r0
	bl	clean_block_list
	mov	r0, r4
	bl	nand_clean_zone_table2
	ldr	r0, .L2036
	bl	NAND_Print_DBG
	ldr	r5, .L2036+4
	mov	r0, r4
	bl	build_zone_list
	ldr	r0, .L2036+8
	bl	NAND_Print_DBG
	mov	r0, r4
	bl	nftl_cache_init
	mov	r0, r5
	bl	NAND_Print_DBG
	mov	r0, r4
	bl	print_block_invalid_list
	mov	r0, r4
	bl	print_free_list
	mov	r0, r5
	bl	NAND_Print_DBG
	ldr	r0, .L2036+12
	bl	NAND_Print_DBG
	movs	r0, #0
	pop	{r3, r4, r5, pc}
.L2037:
	.align	2
.L2036:
	.word	.LC358
	.word	.LC360
	.word	.LC359
	.word	.LC361
	.size	nand_find_zone_table, .-nand_find_zone_table
	.section	.text.nand_check_table,"ax",%progbits
	.align	1
	.global	nand_check_table
	.syntax unified
	.thumb
	.thumb_func
	.fpu softvfp
	.type	nand_check_table, %function
nand_check_table:
	@ args = 0, pretend = 0, frame = 0
	@ frame_needed = 0, uses_anonymous_args = 0
	push	{r3, r4, r5, lr}
	mov	r1, #1000
	ldr	r4, .L2039
	mov	r5, r0
	bl	__nand_flush_write_cache
	mov	r0, r4
	bl	NAND_Print_DBG
	mov	r0, r5
	bl	print_nftl_zone
	mov	r0, r4
	bl	NAND_Print_DBG
	mov	r0, r5
	bl	nand_clean_zone_table
	mov	r0, r4
	bl	NAND_Print_DBG
	mov	r0, r5
	bl	nand_find_zone_table
	mov	r0, r5
	bl	write_data_to_nand
	mov	r0, r4
	bl	NAND_Print_DBG
	movs	r0, #0
	pop	{r3, r4, r5, pc}
.L2040:
	.align	2
.L2039:
	.word	.LC362
	.size	nand_check_table, .-nand_check_table
	.global	__aeabi_idiv
	.section	.text.nftl_start,"ax",%progbits
	.align	1
	.global	nftl_start
	.syntax unified
	.thumb
	.thumb_func
	.fpu softvfp
	.type	nftl_start, %function
nftl_start:
	@ args = 40, pretend = 0, frame = 40
	@ frame_needed = 0, uses_anonymous_args = 0
	push	{r4, r5, r6, r7, r8, r10, fp, lr}
	sub	sp, sp, #40
	ldr	r3, [sp, #92]
	mov	r6, r2
	ldr	r2, [sp, #76]
	ldr	r7, [sp, #88]
	lsr	r10, r3, #9
	ldr	r3, [sp, #72]
	strd	r0, r1, [sp, #28]
	mov	r0, #2016
	lsrs	r7, r7, #9
	lsrs	r3, r3, #9
	orr	r3, r3, r2, lsl #23
	ldr	r2, [sp, #84]
	str	r3, [sp, #20]
	ldr	r3, [sp, #80]
	lsrs	r3, r3, #9
	orr	r3, r3, r2, lsl #23
	str	r3, [sp, #24]
	bl	NAND_Malloc
	mov	r4, r0
	str	r0, [r6]
	cbz	r0, .L2042
	lsl	r3, r10, #9
	uxth	r3, r3
	str	r3, [sp, #16]
	movs	r3, #2
.L2065:
.L2043:
	mov	r2, #2016
	movs	r1, #0
	mov	r0, r4
	str	r3, [sp, #12]
	bl	NAND_Memset
	ldr	r3, [sp, #28]
	movs	r0, #36
	str	r3, [r4]
	ldr	r3, [sp, #32]
	str	r3, [r4, #1972]
	bl	NAND_Malloc
	str	r0, [r4, #4]
	cbnz	r0, .L2045
	ldr	r1, .L2067
	ldr	r0, .L2067+4
	b	.L2066
.L2042:
	ldr	r1, .L2067
	ldr	r0, .L2067+8
.L2066:
	mvn	r5, #1
	bl	NAND_Print
.L2041:
	mov	r0, r5
	add	sp, sp, #40
	@ sp needed
	pop	{r4, r5, r6, r7, r8, r10, fp, pc}
.L2045:
	movs	r2, #4
	movs	r1, #0
	bl	NAND_Memset
	ldr	r3, .L2067+12
	movs	r2, #8
	ldr	r0, [r4, #4]
	ldr	r3, [r3]
	ldr	r1, [r3, #8]
	add	r1, r1, r2
	bl	NAND_Memcpy
	ldr	r5, [r4, #4]
	mov	r1, r7
	ldrh	r3, [sp, #16]
	ldr	r0, [sp, #24]
	strh	r3, [r5, #26]	@ movhi
	ldr	r3, [sp, #96]
	strb	r3, [r5, #10]
	bl	__aeabi_uidiv
	ldr	r2, [r4, #1972]
	mov	r8, r0
	ldrh	r2, [r2, #12]
	cmp	r2, r0
	bcs	.L2055
	uxth	r3, r0
	mov	r1, r10
	mov	r0, r7
	str	r3, [sp, #4]
	mov	fp, #36
	ldrh	r3, [sp, #4]
	strh	r3, [r5, #24]	@ movhi
	bl	__aeabi_uidiv
	uxtb	r2, r10
	add	r3, r8, #1
	strh	r0, [r5, #20]	@ movhi
	strb	r2, [r5, #8]
	strh	r2, [r5, #28]	@ movhi
	movw	r2, #3000
	strh	r2, [r5, #16]	@ movhi
	mul	r2, fp, r3
	str	r3, [sp, #8]
	mov	r0, r2
	str	r2, [sp, #36]
	bl	nftl_malloc
	ldr	r3, [r4, #4]
	str	r0, [r5, #32]
	ldr	r2, [sp, #36]
	ldr	r0, [r3, #32]
	cbnz	r0, .L2046
	ldr	r1, .L2067
	ldr	r0, .L2067+16
	b	.L2066
.L2046:
	subs	r2, r2, #36
	movs	r1, #255
	bl	NAND_Memset
	movs	r1, #0
	mov	ip, #-1
	mov	r2, r1
.L2047:
	ldr	r3, [sp, #8]
	cmp	r3, r1
	bhi	.L2048
	ldr	r0, [r6]
	bl	nftl_ops_init
	mov	r1, r7
	ldr	r0, [sp, #20]
	bl	__aeabi_uidiv
	ldr	r3, [sp, #4]
	subs	r0, r3, r0
	ldr	r3, [r4, #1972]
	uxth	r2, r0
	ldrh	r3, [r3, #12]
	strh	r2, [r4, #22]	@ movhi
	cmp	r3, r2
	bls	.L2049
	mov	r1, r2
	ldr	r0, .L2067+20
	b	.L2066
.L2048:
	ldr	r3, [r4, #4]
	mul	r0, fp, r1
	ldr	lr, [r3, #32]
	add	r3, lr, r0
	strh	r1, [lr, r0]	@ movhi
	strh	r2, [r3, #4]	@ movhi
	adds	r1, r1, #1
	strh	r2, [r3, #2]	@ movhi
	str	ip, [r3, #8]
	strh	r2, [r3, #6]	@ movhi
	strd	r2, r2, [r3, #12]
	strd	r2, r2, [r3, #20]
	strd	r2, r2, [r3, #28]
	b	.L2047
.L2049:
	mov	r1, r8
	ldr	r0, .L2067+24
	bl	NAND_Print_DBG
	ldrh	r3, [r4, #22]
	movs	r1, #0
	ldr	r0, [r6]
	sub	r2, r8, r3
	muls	r2, r7, r2
	muls	r3, r7, r3
	bl	zone_param_init
	mov	r5, r0
	cbz	r0, .L2050
	ldr	r0, .L2067+28
	bl	NAND_Print
	b	.L2041
.L2050:
	ldr	r3, [sp, #104]
	ldr	r2, [r4, #1972]
	strb	r3, [r4, #16]
	ldrh	r3, [r2, #26]
	cbz	r3, .L2051
	ldrb	r3, [r4, #17]	@ zero_extendqisi2
	orr	r3, r3, #1
	strb	r3, [r4, #17]
.L2051:
	ldrh	r3, [r2, #10]
	cbz	r3, .L2052
	mov	r1, r8
	ldr	r0, [r6]
	bl	is_no_use_device
	cmp	r0, #1
	bne	.L2052
	mov	r1, r8
	ldr	r0, [r6]
	bl	build_zone_list_first
	mov	r8, r0
	cbz	r0, .L2052
	ldr	r0, .L2067+32
	bl	NAND_Print
.L2054:
	mov	r5, r8
	b	.L2041
.L2052:
	ldr	r0, [r6]
	bl	build_zone_list
	mov	r8, r0
	cbz	r0, .L2053
	ldr	r0, .L2067+36
	bl	NAND_Print
	ldr	r3, [sp, #12]
	cmp	r3, #1
	beq	.L2054
	ldr	r0, .L2067+40
	bl	NAND_Print
	movs	r3, #1
	b	.L2065
.L2053:
	ldr	r0, [r6]
	bl	nftl_cache_init
	b	.L2041
.L2055:
	mov	r5, #-1
	b	.L2041
.L2068:
	.align	2
.L2067:
	.word	.LANCHOR25
	.word	.LC364
	.word	.LC363
	.word	g_nsi
	.word	.LC365
	.word	.LC366
	.word	.LC367
	.word	.LC368
	.word	.LC369
	.word	.LC370
	.word	.LC371
	.size	nftl_start, .-nftl_start
	.section	.text.nftl_initialize,"ax",%progbits
	.align	1
	.global	nftl_initialize
	.syntax unified
	.thumb
	.thumb_func
	.fpu softvfp
	.type	nftl_initialize, %function
nftl_initialize:
	@ args = 0, pretend = 0, frame = 16
	@ frame_needed = 0, uses_anonymous_args = 0
	push	{r4, r5, r6, r7, lr}
	sub	sp, sp, #60
	ldr	r5, [r0, #4]
	mov	r4, r0
	mov	r6, r1
	movs	r2, #9
	ldr	r1, .L2091
	add	r0, sp, #44
	bl	memcpy
	add	r0, sp, #56
	movs	r3, #47
	strh	r3, [r0, #-16]!	@ movhi
	ldr	r1, .L2091+4
	bl	nand_fdt_get_string
	cbz	r0, .L2083
	movs	r2, #9
	add	r1, sp, #44
	bl	NAND_Memcmp
	mov	r7, r0
	cbnz	r0, .L2083
	ldr	r3, .L2091+8
	ldr	r2, .L2091+12
	ldr	r1, .L2091+16
	str	r3, [sp, #4]
	ldr	r3, .L2091+20
	ldr	r0, .L2091+24
	str	r3, [sp]
	ldr	r3, .L2091+28
	bl	NAND_Print
	ldr	r0, .L2091+32
	bl	nftl_get_nand_phyinfo
	ldrh	r3, [r5, #34]
	cmp	r3, #15
	bhi	.L2087
.L2083:
	mov	r6, #-1
	b	.L2069
.L2087:
	movs	r0, #36
	bl	NAND_Malloc
	str	r0, [r4, #16]
	cbnz	r0, .L2073
	movs	r6, #1
	ldr	r1, .L2091+36
	ldr	r0, .L2091+40
	bl	NAND_Print
.L2069:
	mov	r0, r6
	add	sp, sp, #60
	@ sp needed
	pop	{r4, r5, r6, r7, pc}
.L2073:
	str	r7, [r0, #8]
	mov	r2, #65536
	ldr	r7, [r5, #80]
	mov	r3, #65537
	strd	r2, r3, [r0]
	movs	r1, #3
	ldr	r2, [r7, #36]
	ldrh	r3, [r2, #2478]
	strh	r3, [r0, #12]	@ movhi
	ldrh	r3, [r2, #2480]
	strh	r1, [r0, #20]	@ movhi
	strh	r3, [r0, #16]	@ movhi
	movs	r3, #8
	strh	r3, [r0, #14]	@ movhi
	movs	r3, #2
	strh	r3, [r0, #18]	@ movhi
	ldr	r3, .L2091+44
	str	r3, [r0, #32]
	ldrh	r3, [r2, #2474]
	cmp	r3, #0
	bne	.L2074
.L2088:
	movs	r3, #30
	strh	r1, [r0, #24]	@ movhi
	strh	r3, [r0, #22]	@ movhi
.L2075:
	ldrh	r3, [r7, #2]
	strh	r3, [r0, #26]	@ movhi
	cmp	r3, #0
	beq	.L2078
	movs	r3, #3
.L2090:
	ldr	r1, [r2, #2492]
	strh	r3, [r0, #24]	@ movhi
	ldrh	r3, [r0, #22]
	ldr	r7, .L2091+48
	sub	r2, r1, #86016
	sub	r2, r2, #384
	cmp	r3, #30
	itt	hi
	movhi	r3, #30
	strhhi	r3, [r0, #22]	@ movhi
	movs	r3, #1
	cmp	r2, r7
	strh	r3, [r0, #24]	@ movhi
	it	ls
	strls	r1, [r0, #32]
	strh	r3, [r0, #28]	@ movhi
	bl	nftl_support_boot_check_crc_from_dts
	cbnz	r0, .L2082
	ldr	r3, [r4, #16]
	strh	r0, [r3, #28]	@ movhi
.L2082:
	movs	r3, #0
	str	r5, [sp, #28]
	strd	r6, r3, [sp, #32]
	mov	r0, r4
	ldrh	r3, [r5, #34]
	ldr	r1, [r4, #16]
	str	r3, [sp, #24]
	ldrh	r3, [r5, #40]
	str	r3, [sp, #20]
	ldr	r3, [r5, #44]
	str	r3, [sp, #16]
	ldrd	r2, [r5, #72]
	strd	r2, [sp, #8]
	ldrd	r2, [r5, #64]
	strd	r2, [sp]
	add	r2, r4, #12
	bl	nftl_start
	mov	r6, r0
	cmp	r0, #0
	bne	.L2069
	ldr	r3, [r5, #80]
	ldr	r0, .L2091+52
	ldr	r2, [r3, #36]
	ldr	r3, [r4, #12]
	ldr	r1, [r2, #2484]
	ldr	r3, [r3, #4]
	ldr	r2, [r2, #2488]
	strh	r1, [r3, #16]	@ movhi
	uxth	r1, r1
	strh	r2, [r3, #30]	@ movhi
	bl	NAND_Print_DBG
	ldr	r3, [r4, #12]
	ldr	r3, [r3, #24]
	str	r6, [r4, #8]
	str	r3, [r4]
	ldr	r3, .L2091+56
	str	r3, [r4, #44]
	ldr	r3, .L2091+60
	str	r3, [r4, #40]
	ldr	r3, .L2091+64
	str	r3, [r4, #48]
	ldr	r3, .L2091+68
	str	r3, [r4, #56]
	ldr	r3, .L2091+72
	str	r3, [r4, #52]
	ldr	r3, .L2091+76
	str	r3, [r4, #60]
	ldr	r3, .L2091+80
	str	r3, [r4, #64]
	ldr	r3, .L2091+84
	str	r3, [r4, #68]
	ldr	r3, .L2091+88
	str	r3, [r4, #72]
	b	.L2069
.L2074:
	cmp	r3, #255
	bne	.L2076
	movs	r3, #1
	strh	r3, [r0]	@ movhi
	movs	r3, #50
	strh	r3, [r0, #22]	@ movhi
	movs	r3, #40
.L2089:
	strh	r3, [r0, #24]	@ movhi
	b	.L2075
.L2076:
	cmp	r3, #10
	bls	.L2077
	movs	r3, #10
	strh	r3, [r2, #2474]	@ movhi
	b	.L2088
.L2077:
	add	r3, r3, r3, lsl #2
	add	r1, r3, r3, lsl #2
	lsls	r3, r3, #3
	lsls	r1, r1, #1
	strh	r1, [r0, #22]	@ movhi
	b	.L2089
.L2078:
	movs	r3, #1
	b	.L2090
.L2092:
	.align	2
.L2091:
	.word	.LANCHOR17+21
	.word	.LANCHOR26
	.word	.LC376
	.word	.LC373
	.word	.LC374
	.word	.LC377
	.word	.LC375
	.word	.LC372
	.word	.LANCHOR18
	.word	.LANCHOR27
	.word	.LC378
	.word	1296000
	.word	7692600
	.word	.LC379
	.word	_nand_read
	.word	_nand_read_history
	.word	_nand_write
	.word	_nand_discard
	.word	_nand_flush_write_cache
	.word	_shutdown_op
	.word	_nand_sector_read
	.word	_nand_sector_write
	.word	_nand_flush_sector_write_cache
	.size	nftl_initialize, .-nftl_initialize
	.section	.text.panic_recover_data,"ax",%progbits
	.align	1
	.global	panic_recover_data
	.syntax unified
	.thumb
	.thumb_func
	.fpu softvfp
	.type	panic_recover_data, %function
panic_recover_data:
	@ args = 0, pretend = 0, frame = 40
	@ frame_needed = 0, uses_anonymous_args = 0
	push	{r4, r5, r6, r7, r8, r10, fp, lr}
	sub	sp, sp, #40
	ldr	r4, .L2112
	ldr	r5, [r4]
	cbnz	r5, .L2094
	ldr	r0, .L2112+4
	bl	NAND_Print
.L2111:
	movs	r0, #0
	b	.L2093
.L2094:
	ldr	r6, [r0, #12]
	ldr	r0, .L2112+8
	ldr	r7, [r6, #4]
	bl	NAND_Print
	ldrh	r0, [r7, #26]
	bl	NAND_Malloc
	mov	fp, r0
	cbnz	r0, .L2096
	mov	r2, #308
	ldr	r1, .L2112+12
	ldr	r0, .L2112+16
	bl	NAND_Print
	mov	r0, #-1
.L2093:
	add	sp, sp, #40
	@ sp needed
	pop	{r4, r5, r6, r7, r8, r10, fp, pc}
.L2096:
	ldrh	r2, [r7, #26]
	movs	r1, #0
	bl	NAND_Memset
	movs	r3, #0
.L2110:
	str	r3, [r4, #16]
	ldr	r3, [r4, #16]
	cmp	r3, #2
	bhi	.L2104
	ldr	r3, [r4, r3, lsl #2]
	str	r3, [sp]
	movs	r3, #0
.L2109:
	str	r3, [r4, #20]
	ldrh	r2, [r7, #20]
	ldr	r3, [r4, #20]
	cmp	r3, r2
	bcc	.L2105
	ldr	r3, [r4, #16]
	adds	r3, r3, #1
	b	.L2110
.L2105:
	ldr	r2, [sp]
	add	r5, sp, #24
	mov	r0, r6
	ldrh	r1, [r2]
	ldrh	r2, [r7, #28]
	strh	r3, [sp, #8]	@ movhi
	strh	r1, [sp, #10]	@ movhi
	add	r1, sp, #8
	ldr	r3, [r6, #1980]
	strh	r2, [sp, #12]	@ movhi
	str	fp, [sp, #16]
	str	r5, [sp, #20]
	blx	r3
	mov	r0, r5
	bl	is_panic_dummy_page
	ldr	r3, [sp]
	ldrh	r8, [r3]
	cbz	r0, .L2099
	ldr	r2, [r4, #20]
	mov	r1, r8
	ldr	r0, .L2112+20
	bl	NAND_Print
.L2100:
	ldr	r3, [r4, #20]
	adds	r3, r3, #1
	b	.L2109
.L2099:
	mov	r0, r5
	bl	is_panic_data_page
	ldr	r10, [r4, #20]
	cbnz	r0, .L2101
	ldr	r3, [r4, #16]
	mov	r2, r10
	mov	r1, r8
	cbnz	r3, .L2102
	cmp	r10, #0
	bne	.L2102
	ldr	r0, .L2112+24
	bl	NAND_Print
.L2103:
	movs	r1, #0
	mov	r0, fp
	bl	NAND_Free
	b	.L2111
.L2102:
	ldr	r0, .L2112+28
	bl	NAND_Print
.L2104:
	ldr	r3, [r4, #16]
	cmp	r3, #0
	blt	.L2103
	ldr	r1, [r4, r3, lsl #2]
	mov	r0, r6
	ldrh	r2, [r1, #6]
	adds	r2, r2, #1
	uxth	r2, r2
	bl	erase_block
	ldr	r3, [r4, #16]
	subs	r3, r3, #1
	str	r3, [r4, #16]
	b	.L2104
.L2101:
	movs	r2, #4
	movs	r1, #7
	mov	r0, r5
	bl	get_spare_data
	movs	r2, #4
	str	r0, [sp, #4]
	movs	r1, #11
	mov	r0, r5
	bl	get_spare_data
	mov	r2, r10
	mov	r5, r0
	mov	r1, r8
	ldr	r3, [sp, #4]
	ldr	r0, .L2112+32
	bl	NAND_Print
	mov	r2, fp
	mov	r1, r5
	ldr	r0, [sp, #4]
	bl	nftl_write
	b	.L2100
.L2113:
	.align	2
.L2112:
	.word	.LANCHOR23
	.word	.LC380
	.word	.LC381
	.word	.LANCHOR28
	.word	.LC382
	.word	.LC383
	.word	.LC384
	.word	.LC385
	.word	.LC386
	.size	panic_recover_data, .-panic_recover_data
	.section	.text.nftl_add,"ax",%progbits
	.align	1
	.global	nftl_add
	.syntax unified
	.thumb
	.thumb_func
	.fpu softvfp
	.type	nftl_add, %function
nftl_add:
	@ args = 0, pretend = 0, frame = 0
	@ frame_needed = 0, uses_anonymous_args = 0
	push	{r3, r4, r5, lr}
	mov	r5, r0
	movs	r0, #76
	bl	NAND_Malloc
	mov	r4, r0
	cbnz	r0, .L2115
	ldr	r1, .L2118
	ldr	r0, .L2118+4
	bl	NAND_Print
.L2114:
	mov	r0, r4
	pop	{r3, r4, r5, pc}
.L2115:
	mov	r0, r5
	bl	build_nand_partition
	ldrh	r1, [r5]
	str	r0, [r4, #4]
	mov	r0, r4
	bl	nftl_initialize
	cbz	r0, .L2117
	ldr	r0, .L2118+8
	movs	r4, #0
	bl	NAND_Print
	b	.L2114
.L2117:
	mov	r1, r4
	ldr	r0, .L2118+12
	bl	add_nftl_blk_list
	mov	r0, r4
	bl	panic_recover_data
	ldr	r0, .L2118+16
	bl	NAND_Print_DBG
	b	.L2114
.L2119:
	.align	2
.L2118:
	.word	.LANCHOR29
	.word	.LC387
	.word	.LC388
	.word	.LANCHOR0
	.word	.LC389
	.size	nftl_add, .-nftl_add
	.section	.text.nftl_build_all,"ax",%progbits
	.align	1
	.global	nftl_build_all
	.syntax unified
	.thumb
	.thumb_func
	.fpu softvfp
	.type	nftl_build_all, %function
nftl_build_all:
	@ args = 0, pretend = 0, frame = 0
	@ frame_needed = 0, uses_anonymous_args = 0
	push	{r4, lr}
	movs	r2, #0
	ldr	r4, [r0, #56]
	ldr	r3, .L2125
	str	r2, [r3, #8]
.L2121:
	mov	r0, r4
	cbz	r4, .L2120
	bl	nftl_add
	cbnz	r0, .L2122
	ldr	r0, .L2125+4
	bl	NAND_Print
	movs	r0, #1
.L2120:
	pop	{r4, pc}
.L2122:
	ldr	r4, [r4, #48]
	b	.L2121
.L2126:
	.align	2
.L2125:
	.word	.LANCHOR0
	.word	.LC390
	.size	nftl_build_all, .-nftl_build_all
	.section	.text.nftl_build_one,"ax",%progbits
	.align	1
	.global	nftl_build_one
	.syntax unified
	.thumb
	.thumb_func
	.fpu softvfp
	.type	nftl_build_one, %function
nftl_build_one:
	@ args = 0, pretend = 0, frame = 0
	@ frame_needed = 0, uses_anonymous_args = 0
	push	{r3, r4, r5, lr}
	mov	r5, r1
	ldr	r4, [r0, #56]
	cbnz	r1, .L2129
	ldr	r3, .L2133
	str	r1, [r3, #8]
.L2129:
	cbnz	r4, .L2132
	mov	r0, r4
	b	.L2127
.L2132:
	ldrh	r3, [r4]
	cmp	r3, r5
	bne	.L2130
	mov	r0, r4
	bl	nftl_add
	cbnz	r0, .L2130
	ldr	r0, .L2133+4
	bl	NAND_Print
	movs	r0, #1
.L2127:
	pop	{r3, r4, r5, pc}
.L2130:
	ldr	r4, [r4, #48]
	b	.L2129
.L2134:
	.align	2
.L2133:
	.word	.LANCHOR0
	.word	.LC390
	.size	nftl_build_one, .-nftl_build_one
	.section	.text.hynix16nm_vender_set_param,"ax",%progbits
	.align	1
	.global	hynix16nm_vender_set_param
	.syntax unified
	.thumb
	.thumb_func
	.fpu softvfp
	.type	hynix16nm_vender_set_param, %function
hynix16nm_vender_set_param:
	@ args = 0, pretend = 0, frame = 8
	@ frame_needed = 0, uses_anonymous_args = 0
	push	{r4, r5, r6, r7, r8, r10, fp, lr}
	mov	r5, r1
	sub	sp, sp, #24
	mov	r4, r2
	mov	r7, r3
	mov	r6, r0
	movs	r3, #54
	add	r10, sp, #24
	mov	r8, r5
	add	r7, r7, r4
	mov	fp, #1
	strb	r3, [r10, #-1]!
	bl	nand_enable_chip
	ldr	r0, [r6, #260]
	bl	ndfc_disable_randomize
.L2136:
	cmp	r4, r7
	bne	.L2137
	mov	r0, r6
	bl	nand_disable_chip
	ldrb	r0, [r5, #3]	@ zero_extendqisi2
	ldrb	r3, [r5, #2]	@ zero_extendqisi2
	ldrb	r2, [r5, #1]	@ zero_extendqisi2
	str	r0, [sp]
	ldrb	r1, [r5]	@ zero_extendqisi2
	ldr	r0, .L2138
	bl	NAND_Print_DBG
	movs	r0, #0
	add	sp, sp, #24
	@ sp needed
	pop	{r4, r5, r6, r7, r8, r10, fp, pc}
.L2137:
	mov	r3, r4
	str	r8, [sp]
	movs	r2, #0
	mov	r1, r10
	strd	fp, fp, [sp, #4]
	mov	r0, r6
	bl	set_cmd_with_nand_bus
	movs	r2, #0
	movs	r1, #22
	mov	r0, r6
	adds	r4, r4, #1
	bl	set_one_cmd
	add	r8, r8, #1
	b	.L2136
.L2139:
	.align	2
.L2138:
	.word	.LC391
	.size	hynix16nm_vender_set_param, .-hynix16nm_vender_set_param
	.section	.text.hynix16nm_set_readretry,"ax",%progbits
	.align	1
	.global	hynix16nm_set_readretry
	.syntax unified
	.thumb
	.thumb_func
	.fpu softvfp
	.type	hynix16nm_set_readretry, %function
hynix16nm_set_readretry:
	@ args = 0, pretend = 0, frame = 8
	@ frame_needed = 0, uses_anonymous_args = 0
	ldr	r3, .L2145
	push	{r0, r1, r4, lr}
	ldr	r1, [r0, #176]
	ldrb	r3, [r3]	@ zero_extendqisi2
	cmp	r1, r3
	bhi	.L2144
	ldr	r3, .L2145+4
	lsls	r1, r1, #2
	movs	r2, #0
	ldrb	r3, [r3]	@ zero_extendqisi2
.L2142:
	cmp	r3, r2
	bhi	.L2143
	ldr	r2, .L2145+8
	mov	r1, sp
	bl	hynix16nm_vender_set_param
.L2140:
	add	sp, sp, #8
	@ sp needed
	pop	{r4, pc}
.L2143:
	adds	r4, r1, r2
	add	r4, r4, r0
	ldrb	r4, [r4, #48]	@ zero_extendqisi2
	strb	r4, [sp, r2]
	adds	r2, r2, #1
	b	.L2142
.L2144:
	mvn	r0, #73
	b	.L2140
.L2146:
	.align	2
.L2145:
	.word	.LANCHOR30
	.word	.LANCHOR31
	.word	.LANCHOR32
	.size	hynix16nm_set_readretry, .-hynix16nm_set_readretry
	.section	.text.hynix16nm_read_page_end,"ax",%progbits
	.align	1
	.global	hynix16nm_read_page_end
	.syntax unified
	.thumb
	.thumb_func
	.fpu softvfp
	.type	hynix16nm_read_page_end, %function
hynix16nm_read_page_end:
	@ args = 0, pretend = 0, frame = 0
	@ frame_needed = 0, uses_anonymous_args = 0
	ldr	r3, .L2160
	push	{r0, r1, r2, r4, r5, r6, r7, r8, r10, lr}
	mov	r6, r0
	ldr	r1, [r0]
	ldr	r0, [r3]
	bl	nci_get_from_nsi
	mov	r5, r0
	mov	r0, r6
	bl	generic_read_page_end_not_retry
	adds	r3, r0, #2
	mov	r4, r0
	bne	.L2147
	ldr	r7, .L2160+4
	mov	r10, #0
	mov	r8, r10
	ldr	r0, .L2160+8
	bl	NAND_Print_DBG
.L2149:
	ldrb	r2, [r7]	@ zero_extendqisi2
	cmp	r2, r10
	ble	.L2153
	ldr	r3, [r5, #176]
	mov	r0, r5
	adds	r3, r3, #1
	cmp	r3, r2
	it	hi
	movhi	r3, r8
	str	r3, [r5, #176]
	bl	hynix16nm_set_readretry
	mov	r4, r0
	cbz	r0, .L2152
	movs	r3, #0
	mov	r0, r5
	str	r3, [r5, #176]
	bl	hynix16nm_set_readretry
.L2153:
	movs	r3, #0
	mov	r0, r5
	str	r3, [r5, #176]
	bl	hynix16nm_set_readretry
.L2147:
	mov	r0, r4
	add	sp, sp, #12
	@ sp needed
	pop	{r4, r5, r6, r7, r8, r10, pc}
.L2152:
	mov	r0, r6
	bl	generic_read_page_start
	mov	r4, r0
	mov	r0, r6
	bl	generic_read_page_end_not_retry
	orrs	r4, r4, r0
	cmp	r4, #10
	beq	.L2154
	cbnz	r4, .L2155
.L2154:
	ldr	r3, [r5, #176]
	movs	r4, #10
	ldr	r1, [r5, #260]
	ldr	r0, .L2160+12
	str	r3, [sp, #4]
	ldr	r3, [r6, #8]
	str	r3, [sp]
	ldr	r3, [r6, #4]
	ldr	r2, [r5, #20]
	ldr	r1, [r1, #8]
	bl	NAND_Print_DBG
	b	.L2153
.L2155:
	add	r10, r10, #1
	b	.L2149
.L2161:
	.align	2
.L2160:
	.word	g_nsi
	.word	.LANCHOR30
	.word	.LC392
	.word	.LC393
	.size	hynix16nm_read_page_end, .-hynix16nm_read_page_end
	.section	.text.hynix16nm_get_special_info,"ax",%progbits
	.align	1
	.global	hynix16nm_get_special_info
	.syntax unified
	.thumb
	.thumb_func
	.fpu softvfp
	.type	hynix16nm_get_special_info, %function
hynix16nm_get_special_info:
	@ args = 0, pretend = 0, frame = 8
	@ frame_needed = 0, uses_anonymous_args = 0
	push	{r0, r1, r4, r5, r6, r7, r8, r10, fp, lr}
	mov	r4, r0
	ldr	r0, .L2175
	mov	r8, r1
	mov	r7, r2
	mov	r10, r3
	bl	NAND_Print_DBG
	mov	r0, sp
	bl	physic_info_get_offset
	cbz	r0, .L2169
	ldr	r0, .L2175+4
	bl	NAND_Print
	movs	r0, #1
.L2162:
	add	sp, sp, #8
	@ sp needed
	pop	{r4, r5, r6, r7, r8, r10, fp, pc}
.L2169:
	ldr	fp, .L2175+8
	movs	r5, #4
	add	r6, r4, #9728
.L2163:
	mov	r1, r5
	mov	r0, fp
	bl	NAND_Print_DBG
	ldrb	r3, [r6]	@ zero_extendqisi2
	cmp	r3, #165
	beq	.L2165
	mov	r3, r4
	add	r2, sp, #4
	ldr	r1, [sp]
	mov	r0, r5
	bl	physic_info_get_one_copy
.L2165:
	ldrb	r3, [r6]	@ zero_extendqisi2
	cmp	r3, #165
	bne	.L2166
	add	r2, r4, #9728
	add	r1, r4, #10752
	adds	r2, r2, #2
	movs	r3, #0
.L2167:
	ldrb	r0, [r2], #1	@ zero_extendqisi2
	cmp	r2, r1
	add	r3, r3, r0
	uxtb	r3, r3
	bne	.L2167
	movw	r2, #9729
	ldrb	r2, [r4, r2]	@ zero_extendqisi2
	cmp	r2, r3
	bne	.L2168
	mla	r1, r7, r10, r4
	mov	r0, r8
	mov	r2, r7
	add	r1, r1, #9728
	adds	r1, r1, #2
	bl	NAND_Memcpy
	movs	r0, #0
	b	.L2162
.L2168:
	movs	r3, #0
	strb	r3, [r6]
.L2166:
	adds	r5, r5, #1
	cmp	r5, #51
	bne	.L2163
	mov	r0, #-1
	b	.L2162
.L2176:
	.align	2
.L2175:
	.word	.LC394
	.word	.LC395
	.word	.LC396
	.size	hynix16nm_get_special_info, .-hynix16nm_get_special_info
	.section	.text.hynix16nm_write_page_FF,"ax",%progbits
	.align	1
	.global	hynix16nm_write_page_FF
	.syntax unified
	.thumb
	.thumb_func
	.fpu softvfp
	.type	hynix16nm_write_page_FF, %function
hynix16nm_write_page_FF:
	@ args = 0, pretend = 0, frame = 16
	@ frame_needed = 0, uses_anonymous_args = 0
	ldr	r3, .L2196
	push	{r4, r5, r6, r7, r8, r10, fp, lr}
	mov	r10, r0
	sub	sp, sp, #24
	mov	r6, r1
	ldr	r1, [r0]
	ldr	r0, [r3]
	bl	nci_get_from_nsi
	ldr	r4, [r0, #260]
	mov	r5, r0
	ldr	r2, [r0, #20]
	ldr	r1, [r10, #4]
	ldr	r3, [r4, #12]
	add	r7, r4, #32
	ldr	ip, [r0, #24]
	cmp	r2, r3
	bcs	.L2178
	cmp	r1, ip
	bcc	.L2179
.L2178:
	strd	r1, ip, [sp]
	ldr	r1, [r4, #8]
	mvn	r4, #107
	ldr	r0, .L2196+4
	bl	NAND_Print
.L2177:
	mov	r0, r4
	add	sp, sp, #24
	@ sp needed
	pop	{r4, r5, r6, r7, r8, r10, fp, pc}
.L2179:
	bl	nand_read_chip_status_ready
	mov	r0, r5
	bl	nand_enable_chip
	ldr	r0, [r5, #260]
	bl	ndfc_disable_randomize
	ldr	r0, [r5, #260]
	bl	ndfc_disable_ecc
	mov	r0, r7
	bl	ndfc_clean_cmd_seq
	cmp	r6, #8
	beq	.L2188
	cmp	r6, #16
	beq	.L2189
	movs	r3, #0
	str	r3, [sp, #12]
.L2193:
	str	r3, [sp, #8]
	add	r3, r4, #56
	str	r3, [sp, #16]
	mov	r8, #0
	ldr	r3, [sp, #8]
	mov	r6, r8
	subs	r3, r3, #1
	str	r3, [sp, #20]
.L2182:
	ldr	r3, [sp, #8]
	lsl	fp, r6, #10
	cmp	r6, r3
	bne	.L2186
	movs	r3, #0
	movs	r1, #16
	str	r3, [r4, #32]
	movs	r3, #1
	strd	r3, r1, [r4, #40]
	mov	r1, r7
	str	r3, [r4, #48]
	str	r3, [r4, #116]
	ldr	r0, [r5, #260]
	bl	ndfc_execute_cmd
	mov	r0, r5
	bl	nand_read_chip_status_ready
	mov	r4, r0
	cbz	r0, .L2187
	ldr	r0, .L2196+8
	bl	NAND_Print
.L2187:
	mov	r0, r5
	bl	nand_disable_chip
	b	.L2177
.L2188:
	mov	r3, #832
	str	r3, [sp, #12]
	movs	r3, #9
	b	.L2193
.L2189:
	mov	r3, #640
	str	r3, [sp, #12]
	movs	r3, #18
	b	.L2193
.L2186:
	ldr	r1, [r10, #16]
	cbnz	r6, .L2183
	movs	r3, #1
	movs	r0, #128
	mov	r2, #1024
	strd	r3, r0, [r4, #40]
	str	r3, [r4, #48]
	str	r3, [r4, #88]
	strd	r3, r3, [r4, #80]
	str	r1, [r4, #100]
	str	r2, [r4, #92]
	str	r3, [r4, #76]
	ldrd	r1, r2, [r10, #4]
	str	r6, [r4, #32]
	ldr	r0, [r5, #36]
	bl	get_row_addr
	movs	r3, #5
	mov	r8, r0
	str	r3, [r4, #72]
	mov	r2, r0
	ldr	r3, [sp, #16]
	movs	r1, #2
	str	r3, [sp]
	movs	r3, #3
.L2194:
	mov	r0, fp
	adds	r6, r6, #1
	bl	fill_cmd_addr
	mov	r1, r7
	ldr	r0, [r5, #260]
	bl	ndfc_execute_cmd
	b	.L2182
.L2183:
	ldr	r3, [sp, #20]
	add	r1, r1, fp
	movs	r2, #133
	str	r1, [r4, #100]
	str	r2, [r4, #44]
	movs	r2, #1
	cmp	r3, r6
	str	r2, [r4, #40]
	ite	eq
	ldreq	r1, [sp, #12]
	movne	r1, #1024
	str	r2, [r4, #48]
	movs	r3, #0
	str	r2, [r4, #88]
	strd	r2, r2, [r4, #80]
	str	r2, [r4, #76]
	ldr	r2, [sp, #16]
	str	r1, [r4, #92]
	movs	r1, #2
	str	r3, [r4, #32]
	str	r1, [r4, #72]
	str	r2, [sp]
	mov	r2, r8
	b	.L2194
.L2197:
	.align	2
.L2196:
	.word	g_nsi
	.word	.LC397
	.word	.LC398
	.size	hynix16nm_write_page_FF, .-hynix16nm_write_page_FF
	.section	.text.hynix16nm_vender_get_param,"ax",%progbits
	.align	1
	.global	hynix16nm_vender_get_param
	.syntax unified
	.thumb
	.thumb_func
	.fpu softvfp
	.type	hynix16nm_vender_get_param, %function
hynix16nm_vender_get_param:
	@ args = 0, pretend = 0, frame = 8
	@ frame_needed = 0, uses_anonymous_args = 0
	push	{r4, r5, r6, r7, r8, r10, lr}
	mov	r4, r2
	sub	sp, sp, #20
	mov	r7, r3
	mov	r6, r0
	movs	r3, #55
	add	r8, sp, #16
	add	r7, r7, r4
	mov	r10, #1
	mov	r5, r1
	strb	r3, [r8, #-1]!
	bl	nand_enable_chip
	ldr	r0, [r6, #260]
	bl	ndfc_disable_randomize
.L2199:
	cmp	r4, r7
	bne	.L2200
	mov	r0, r6
	bl	nand_disable_chip
	movs	r0, #0
	add	sp, sp, #20
	@ sp needed
	pop	{r4, r5, r6, r7, r8, r10, pc}
.L2200:
	mov	r3, r5
	mov	r2, r4
	str	r10, [sp]
	mov	r1, r8
	mov	r0, r6
	adds	r4, r4, #1
	bl	get_data_with_nand_bus_one_cmd
	adds	r5, r5, #1
	b	.L2199
	.size	hynix16nm_vender_get_param, .-hynix16nm_vender_get_param
	.section	.text.hynix16nm_major_check_byte,"ax",%progbits
	.align	1
	.global	hynix16nm_major_check_byte
	.syntax unified
	.thumb
	.thumb_func
	.fpu softvfp
	.type	hynix16nm_major_check_byte, %function
hynix16nm_major_check_byte:
	@ args = 8, pretend = 0, frame = 8
	@ frame_needed = 0, uses_anonymous_args = 0
	push	{r0, r1, r4, r5, r6, r7, r8, r10, fp, lr}
	mov	r10, r0
	ldr	r8, [sp, #44]
	mov	r5, r2
	mov	fp, r3
	str	r1, [sp, #4]
	cmp	r2, r8, lsr #1
	bcs	.L2220
	mov	r2, r8
	mov	r1, r5
	ldr	r0, .L2223
	bl	NAND_Print
	mvn	r0, #106
	movs	r3, #255
	strb	r3, [r10]
.L2201:
	add	sp, sp, #8
	@ sp needed
	pop	{r4, r5, r6, r7, r8, r10, fp, pc}
.L2206:
	ldrb	lr, [r0], #1	@ zero_extendqisi2
	tst	lr, r1
	ite	ne
	addne	r2, r2, #1
	addeq	r3, r3, #1
.L2217:
	cmp	ip, r0
	bne	.L2206
	cmp	r5, r2
	bcs	.L2207
.L2222:
	orrs	r7, r7, r1
	uxtb	r7, r7
.L2208:
	adds	r6, r6, #1
.L2215:
	adds	r4, r4, #1
	cmp	r4, #8
	bne	.L2202
	cmp	r6, #8
	iteet	ne
	movne	r3, #255
	strbeq	r7, [r10]
	moveq	r0, #0
	strbne	r3, [r10]
	it	ne
	mvnne	r0, #79
	b	.L2201
.L2213:
	ldrb	lr, [r0], #1	@ zero_extendqisi2
	tst	lr, r1
	ite	ne
	addne	r3, r3, #1
	addeq	r2, r2, #1
	b	.L2210
.L2220:
	movs	r6, #0
	mov	r7, r6
	mov	r4, r6
.L2202:
	movs	r1, #1
	movs	r3, #0
	lsls	r1, r1, r4
	mov	r0, fp
	add	ip, r8, fp
	mov	r2, r3
	b	.L2217
.L2207:
	cmp	r5, r3
	bcc	.L2208
	ldr	r0, [sp, #4]
	cmp	r0, #1
	bne	.L2209
	ldr	r0, [sp, #40]
	movs	r3, #0
	mov	r2, r3
	add	ip, r8, r0
.L2210:
	cmp	r0, ip
	bne	.L2213
	cmp	r5, r2
	bcc	.L2222
	cmp	r5, r3
	bcc	.L2208
.L2209:
	mov	r1, r4
	ldr	r0, .L2223+4
	bl	NAND_Print
	mov	r1, r4
	ldr	r0, .L2223+8
	bl	NAND_Print
	b	.L2215
.L2224:
	.align	2
.L2223:
	.word	.LC399
	.word	.LC400
	.word	.LC401
	.size	hynix16nm_major_check_byte, .-hynix16nm_major_check_byte
	.section	.text.hynix16nm_get_read_retry_cfg,"ax",%progbits
	.align	1
	.global	hynix16nm_get_read_retry_cfg
	.syntax unified
	.thumb
	.thumb_func
	.fpu softvfp
	.type	hynix16nm_get_read_retry_cfg, %function
hynix16nm_get_read_retry_cfg:
	@ args = 0, pretend = 0, frame = 64
	@ frame_needed = 0, uses_anonymous_args = 0
	push	{r4, r5, r6, r7, r8, r10, fp, lr}
	sub	sp, sp, #72
	add	r4, sp, #8
	mov	fp, r0
	mov	r5, r1
	mov	r8, r2
	movs	r1, #0
	movs	r2, #32
	mov	r0, r4
	add	r7, sp, #40
	mov	r10, r3
	bl	memset
	movs	r2, #32
	movs	r1, #0
	mov	r0, r7
	bl	memset
	movs	r3, #0
.L2226:
	ldrb	r2, [r10, r3]	@ zero_extendqisi2
	strb	r2, [r4, r3]
	adds	r3, r3, #1
	cmp	r3, #8
	bne	.L2226
	strd	r7, r3, [sp]
	movs	r2, #4
	mov	r3, r4
	movs	r1, #0
	mov	r0, fp
	bl	hynix16nm_major_check_byte
	subs	r6, r0, #0
	bge	.L2227
	ldr	r0, .L2248
.L2247:
	bl	NAND_Print
.L2225:
	mov	r0, r6
	add	sp, sp, #72
	@ sp needed
	pop	{r4, r5, r6, r7, r8, r10, fp, pc}
.L2227:
	ldrb	r1, [fp]	@ zero_extendqisi2
	ldr	r0, .L2248+4
	bl	NAND_Print_DBG
	add	r2, r10, #7
	movs	r3, #0
.L2229:
	ldrb	r1, [r2, #1]!	@ zero_extendqisi2
	strb	r1, [r4, r3]
	adds	r3, r3, #1
	cmp	r3, #8
	bne	.L2229
	strd	r7, r3, [sp]
	movs	r2, #4
	mov	r3, r4
	movs	r1, #0
	mov	r0, r5
	bl	hynix16nm_major_check_byte
	subs	r6, r0, #0
	bge	.L2230
	ldr	r0, .L2248+8
	b	.L2247
.L2230:
	ldrb	r1, [r5]	@ zero_extendqisi2
	ldr	r0, .L2248+12
	bl	NAND_Print_DBG
	ldrb	r1, [fp]	@ zero_extendqisi2
	ldrb	r2, [r5]	@ zero_extendqisi2
	cmp	r1, #8
	bne	.L2231
	cmp	r2, #4
	beq	.L2238
.L2231:
	ldr	r0, .L2248+16
	mvn	r6, #80
	bl	NAND_Print
	b	.L2225
.L2234:
	adds	r5, r5, #1
	cmp	r5, #32
	bne	.L2232
	movs	r6, #0
	b	.L2235
.L2238:
	movs	r5, #0
.L2232:
	add	r2, r10, r5
	movs	r3, #0
.L2233:
	ldrb	r1, [r2, #16]	@ zero_extendqisi2
	adds	r2, r2, #64
	strb	r1, [r4, r3]
	ldrb	r1, [r2, #-16]	@ zero_extendqisi2
	strb	r1, [r7, r3]
	adds	r3, r3, #1
	cmp	r3, #8
	bne	.L2233
	strd	r7, r3, [sp]
	movs	r2, #4
	mov	r3, r4
	movs	r1, #1
	add	r0, r8, r5
	bl	hynix16nm_major_check_byte
	subs	r6, r0, #0
	bge	.L2234
	mov	r1, r5
	ldr	r0, .L2248+20
	bl	NAND_Print
.L2235:
	ldr	r7, .L2248+24
	movs	r4, #0
	ldr	r5, .L2248+28
.L2237:
	lsls	r3, r4, #29
	bne	.L2236
	cbz	r4, .L2236
	mov	r0, r7
	bl	NAND_Print_DBG
.L2236:
	ldrb	r1, [r8, r4]	@ zero_extendqisi2
	mov	r0, r5
	adds	r4, r4, #1
	bl	NAND_Print_DBG
	cmp	r4, #32
	bne	.L2237
	ldr	r0, .L2248+24
	bl	NAND_Print_DBG
	b	.L2225
.L2249:
	.align	2
.L2248:
	.word	.LC402
	.word	.LC403
	.word	.LC404
	.word	.LC405
	.word	.LC406
	.word	.LC407
	.word	.LC7
	.word	.LC25
	.size	hynix16nm_get_read_retry_cfg, .-hynix16nm_get_read_retry_cfg
	.section	.text.hynix16nm_read_otp_info_hynix,"ax",%progbits
	.align	1
	.global	hynix16nm_read_otp_info_hynix
	.syntax unified
	.thumb
	.thumb_func
	.fpu softvfp
	.type	hynix16nm_read_otp_info_hynix, %function
hynix16nm_read_otp_info_hynix:
	@ args = 0, pretend = 0, frame = 16
	@ frame_needed = 0, uses_anonymous_args = 0
	push	{r4, r5, r6, r7, r8, r10, fp, lr}
	mov	r4, r0
	sub	sp, sp, #32
	mov	r8, r1
	bl	nand_enable_chip
	ldr	r0, [r4, #260]
	bl	ndfc_disable_randomize
	ldr	r2, [r4, #20]
	ldr	r3, [r4, #260]
	ldr	r0, .L2255
	add	r3, r3, r2, lsl #2
	ldr	r2, [r3, #1148]
	ldr	r1, [r3, #1116]
	bl	NAND_Print_DBG
	cmp	r8, #0
	bne	.L2251
	mov	r0, r4
	bl	nand_disable_chip
	ldr	r0, .L2255+4
	bl	NAND_Print
	mvn	r0, #78
.L2250:
	add	sp, sp, #32
	@ sp needed
	pop	{r4, r5, r6, r7, r8, r10, fp, pc}
.L2251:
	movs	r2, #1
	movs	r1, #255
	mov	r0, r4
	movs	r6, #1
	bl	set_one_cmd
	add	r7, sp, #20
	movs	r3, #82
	movs	r5, #56
	mov	fp, #54
	strb	r3, [sp, #20]
	movs	r2, #0
	add	r3, sp, #24
	add	r1, sp, #16
	strd	r6, r6, [sp, #4]
	mov	r0, r4
	str	r7, [sp]
	mov	r10, #48
	strb	r5, [sp, #24]
	strb	fp, [sp, #16]
	bl	set_cmd_with_nand_bus
	movs	r2, #0
	movs	r1, #22
	mov	r0, r4
	bl	set_one_cmd
	movs	r2, #0
	movs	r1, #23
	mov	r0, r4
	bl	set_one_cmd
	movs	r2, #0
	movs	r1, #4
	mov	r0, r4
	bl	set_one_cmd
	movs	r2, #0
	movs	r1, #25
	mov	r0, r4
	bl	set_one_cmd
	movs	r2, #0
	mov	r0, r4
	mov	r1, r2
	bl	set_one_cmd
	movs	r1, #0
	mov	r0, r4
	bl	set_one_addr
	movs	r1, #0
	mov	r0, r4
	bl	set_one_addr
	movs	r1, #0
	mov	r0, r4
	bl	set_one_addr
	movs	r1, #2
	mov	r0, r4
	bl	set_one_addr
	movs	r1, #0
	mov	r0, r4
	bl	set_one_addr
	mov	r3, #528
	movs	r2, #0
	str	r3, [sp]
	add	r1, sp, #16
	mov	r3, r8
	mov	r0, r4
	strb	r10, [sp, #16]
	bl	get_data_with_nand_bus_one_cmd
	mov	r2, r6
	movs	r1, #255
	mov	r0, r4
	bl	set_one_cmd
	strb	r5, [sp, #24]
	movs	r5, #0
	add	r3, sp, #24
	mov	r2, r5
	add	r1, sp, #16
	strd	r6, r6, [sp, #4]
	str	r7, [sp]
	mov	r0, r4
	strb	fp, [sp, #16]
	strb	r5, [sp, #20]
	bl	set_cmd_with_nand_bus
	mov	r2, r5
	movs	r1, #22
	mov	r0, r4
	bl	set_one_cmd
	mov	r2, r5
	mov	r1, r5
	mov	r0, r4
	bl	set_one_cmd
	mov	r1, r5
	mov	r0, r4
	bl	set_one_addr
	mov	r1, r5
	mov	r0, r4
	bl	set_one_addr
	mov	r1, r5
	mov	r0, r4
	bl	set_one_addr
	mov	r1, r5
	mov	r0, r4
	bl	set_one_addr
	mov	r1, r5
	mov	r0, r4
	bl	set_one_addr
	mov	r2, r6
	mov	r1, r10
	mov	r0, r4
	bl	set_one_cmd
	ldrb	r1, [r8]	@ zero_extendqisi2
	ldrb	r2, [r8, #1]	@ zero_extendqisi2
	cmp	r1, #8
	bne	.L2253
	cmp	r2, #8
	beq	.L2254
.L2253:
	ldr	r0, .L2255+8
	bl	NAND_Print
	mov	r0, r4
	bl	nand_disable_chip
	mvn	r0, #95
	b	.L2250
.L2254:
	mov	r0, r4
	bl	nand_disable_chip
	mov	r0, r5
	b	.L2250
.L2256:
	.align	2
.L2255:
	.word	.LC408
	.word	.LC409
	.word	.LC410
	.size	hynix16nm_read_otp_info_hynix, .-hynix16nm_read_otp_info_hynix
	.section	.text.hynix16nm_get_rr_value_otp_hynix,"ax",%progbits
	.align	1
	.global	hynix16nm_get_rr_value_otp_hynix
	.syntax unified
	.thumb
	.thumb_func
	.fpu softvfp
	.type	hynix16nm_get_rr_value_otp_hynix, %function
hynix16nm_get_rr_value_otp_hynix:
	@ args = 0, pretend = 0, frame = 8
	@ frame_needed = 0, uses_anonymous_args = 0
	push	{r0, r1, r4, r5, r6, lr}
	mov	r4, r0
	mov	r0, #528
	bl	nand_get_temp_buf
	mov	r5, r0
	cbnz	r0, .L2258
	mvn	r6, #77
	ldr	r0, .L2262
	bl	NAND_Print
.L2257:
	mov	r0, r6
	add	sp, sp, #8
	@ sp needed
	pop	{r4, r5, r6, pc}
.L2258:
	mov	r1, r0
	mov	r0, r4
	bl	hynix16nm_read_otp_info_hynix
	cmp	r0, #0
	bge	.L2260
	ldr	r3, [r4, #260]
	ldr	r2, [r4, #20]
	ldr	r0, .L2262+4
	ldr	r1, [r3, #8]
	bl	NAND_Print
.L2260:
	mov	r3, r5
	add	r2, r4, #48
	add	r1, sp, #7
	add	r0, sp, #6
	bl	hynix16nm_get_read_retry_cfg
	subs	r6, r0, #0
	bge	.L2261
	ldr	r3, [r4, #260]
	ldr	r2, [r4, #20]
	ldr	r0, .L2262+8
	ldr	r1, [r3, #8]
	bl	NAND_Print
.L2261:
	mov	r1, #528
	mov	r0, r5
	bl	nand_free_temp_buf
	b	.L2257
.L2263:
	.align	2
.L2262:
	.word	.LC411
	.word	.LC412
	.word	.LC413
	.size	hynix16nm_get_rr_value_otp_hynix, .-hynix16nm_get_rr_value_otp_hynix
	.section	.text.hynix16nm_save_otp_value,"ax",%progbits
	.align	1
	.global	hynix16nm_save_otp_value
	.syntax unified
	.thumb
	.thumb_func
	.fpu softvfp
	.type	hynix16nm_save_otp_value, %function
hynix16nm_save_otp_value:
	@ args = 0, pretend = 0, frame = 0
	@ frame_needed = 0, uses_anonymous_args = 0
	@ link register save eliminated.
	subs	r3, r1, #1
	adds	r0, r0, #48
	adds	r1, r1, #31
.L2265:
	ldrb	r2, [r3, #1]!	@ zero_extendqisi2
	cmp	r3, r1
	strb	r2, [r0], #1
	bne	.L2265
	bx	lr
	.size	hynix16nm_save_otp_value, .-hynix16nm_save_otp_value
	.section	.text.hynix16nm_get_otp_param,"ax",%progbits
	.align	1
	.global	hynix16nm_get_otp_param
	.syntax unified
	.thumb
	.thumb_func
	.fpu softvfp
	.type	hynix16nm_get_otp_param, %function
hynix16nm_get_otp_param:
	@ args = 0, pretend = 0, frame = 0
	@ frame_needed = 0, uses_anonymous_args = 0
	push	{r3, r4, r5, r6, r7, lr}
	mov	r4, r0
	ldr	r7, .L2274
	mov	r5, r1
	movs	r6, #0
.L2268:
	mov	r1, r6
	mov	r0, r7
	bl	NAND_Print_DBG
	mov	r0, r4
	adds	r6, r6, #1
	bl	hynix16nm_get_rr_value_otp_hynix
	cmp	r0, #0
	bne	.L2268
	mov	r0, r4
	mov	r1, r5
	adds	r4, r4, #32
.L2269:
	mov	r2, r1
	movs	r3, #0
.L2270:
	adds	r5, r0, r3
	adds	r3, r3, #1
	ldrb	r5, [r5, #48]	@ zero_extendqisi2
	cmp	r3, #4
	strb	r5, [r2], #1
	bne	.L2270
	adds	r0, r0, #4
	adds	r1, r1, #4
	cmp	r0, r4
	bne	.L2269
	movs	r0, #0
	pop	{r3, r4, r5, r6, r7, pc}
.L2275:
	.align	2
.L2274:
	.word	.LC414
	.size	hynix16nm_get_otp_param, .-hynix16nm_get_otp_param
	.section	.text.hynix16nm_set_default_param,"ax",%progbits
	.align	1
	.global	hynix16nm_set_default_param
	.syntax unified
	.thumb
	.thumb_func
	.fpu softvfp
	.type	hynix16nm_set_default_param, %function
hynix16nm_set_default_param:
	@ args = 0, pretend = 0, frame = 8
	@ frame_needed = 0, uses_anonymous_args = 0
	push	{r0, r1, r2, r4, r5, r6, r7, lr}
	add	r1, r0, #48
	ldr	r5, .L2281
	movs	r2, #0
	ldrb	r3, [r5]	@ zero_extendqisi2
.L2277:
	cmp	r3, r2
	bhi	.L2278
	ldr	r7, .L2281+4
	mov	r1, sp
	ldr	r2, .L2281+8
	movs	r4, #0
	bl	hynix16nm_vender_set_param
	mov	r6, r0
	ldr	r0, .L2281+12
	bl	NAND_Print_DBG
.L2279:
	ldrb	r3, [r5]	@ zero_extendqisi2
	cmp	r3, r4
	bhi	.L2280
	ldr	r0, .L2281+16
	bl	NAND_Print_DBG
	mov	r0, r6
	add	sp, sp, #12
	@ sp needed
	pop	{r4, r5, r6, r7, pc}
.L2278:
	ldrb	r4, [r1], #1	@ zero_extendqisi2
	strb	r4, [sp, r2]
	adds	r2, r2, #1
	b	.L2277
.L2280:
	ldrb	r1, [sp, r4]	@ zero_extendqisi2
	mov	r0, r7
	bl	NAND_Print_DBG
	adds	r4, r4, #1
	b	.L2279
.L2282:
	.align	2
.L2281:
	.word	.LANCHOR31
	.word	.LC416
	.word	.LANCHOR32
	.word	.LC415
	.word	.LC7
	.size	hynix16nm_set_default_param, .-hynix16nm_set_default_param
	.section	.text.hynix16nm_init_readretry_param,"ax",%progbits
	.align	1
	.global	hynix16nm_init_readretry_param
	.syntax unified
	.thumb
	.thumb_func
	.fpu softvfp
	.type	hynix16nm_init_readretry_param, %function
hynix16nm_init_readretry_param:
	@ args = 0, pretend = 0, frame = 64
	@ frame_needed = 0, uses_anonymous_args = 0
	ldr	r3, [r0, #264]
	push	{r4, r5, r6, r7, r8, r10, lr}
	mov	r4, r0
	sub	sp, sp, #68
	ldr	r0, [r3, #16]
	lsls	r0, r0, #9
	bl	nand_get_temp_buf
	mov	r6, r0
	cmp	r0, #0
	beq	.L2290
	mov	r0, #32768
	bl	nand_get_temp_buf
	mov	r7, r0
	cbnz	r0, .L2285
	ldr	r3, [r4, #264]
	mov	r0, r6
	ldr	r1, [r3, #16]
	lsls	r1, r1, #9
	bl	nand_free_temp_buf
	ldr	r0, .L2292
	bl	NAND_Print
	mvn	r0, #75
.L2283:
	add	sp, sp, #68
	@ sp needed
	pop	{r4, r5, r6, r7, r8, r10, pc}
.L2285:
	ldr	r3, [r4, #16]
	movs	r2, #32
	mov	r1, r6
	bl	hynix16nm_get_special_info
	ldr	r3, [r4, #260]
	mov	r5, r0
	cbnz	r0, .L2286
	ldr	r8, .L2292+16
	mov	r2, r0
	ldr	r10, .L2292+20
	ldr	r1, [r3, #8]
	ldr	r0, .L2292+4
	bl	NAND_Print_DBG
.L2288:
	ldrb	r1, [r6, r5]	@ zero_extendqisi2
	mov	r0, r8
	bl	NAND_Print_DBG
	and	r3, r5, #7
	cmp	r3, #7
	bne	.L2287
	mov	r0, r10
	bl	NAND_Print_DBG
.L2287:
	adds	r5, r5, #1
	cmp	r5, #32
	bne	.L2288
	mov	r0, r4
	mov	r1, r6
	bl	hynix16nm_save_otp_value
	mov	r0, r4
	bl	hynix16nm_set_default_param
.L2289:
	mov	r1, #32768
	mov	r0, r7
	bl	nand_free_temp_buf
	ldr	r3, [r4, #264]
	mov	r0, r6
	ldr	r1, [r3, #16]
	lsls	r1, r1, #9
	bl	nand_free_temp_buf
	movs	r0, #0
	b	.L2283
.L2286:
	ldr	r1, [r3, #8]
	ldr	r0, .L2292+8
	bl	NAND_Print_DBG
	mov	r1, sp
	mov	r0, r4
	bl	hynix16nm_get_otp_param
	mov	r0, r4
	bl	hynix16nm_set_default_param
	ldr	r3, [r4, #260]
	movs	r2, #0
	ldr	r0, .L2292+12
	ldr	r1, [r3, #8]
	bl	NAND_Print_DBG
	b	.L2289
.L2290:
	mvn	r0, #76
	b	.L2283
.L2293:
	.align	2
.L2292:
	.word	.LC417
	.word	.LC418
	.word	.LC420
	.word	.LC421
	.word	.LC419
	.word	.LC7
	.size	hynix16nm_init_readretry_param, .-hynix16nm_init_readretry_param
	.section	.text.hynix16nm_readretry_init,"ax",%progbits
	.align	1
	.global	hynix16nm_readretry_init
	.syntax unified
	.thumb
	.thumb_func
	.fpu softvfp
	.type	hynix16nm_readretry_init, %function
hynix16nm_readretry_init:
	@ args = 0, pretend = 0, frame = 0
	@ frame_needed = 0, uses_anonymous_args = 0
	ldr	r3, [r0, #264]
	ldr	r2, .L2299
	push	{r4, lr}
	ldr	r3, [r3, #52]
	ubfx	r1, r3, #16, #8
	strb	r1, [r2]
	lsrs	r4, r3, #8
	ldr	r2, .L2299+4
	cmp	r1, #4
	strb	r4, [r2]
	ldr	r2, .L2299+8
	strb	r3, [r2]
	bne	.L2295
	ldr	r3, .L2299+12
	movs	r2, #56
	add	r1, r0, #176
	strb	r2, [r3]
	movs	r2, #57
	strb	r2, [r3, #1]
	movs	r2, #58
	strb	r2, [r3, #2]
	movs	r2, #59
	strb	r2, [r3, #3]
	add	r2, r0, #48
	movs	r3, #0
	str	r3, [r0, #176]
.L2296:
	strb	r3, [r2], #1
	cmp	r2, r1
	bne	.L2296
	bl	hynix16nm_init_readretry_param
	movs	r0, #0
.L2294:
	pop	{r4, pc}
.L2295:
	ldr	r0, .L2299+16
	bl	NAND_Print
	mvn	r0, #74
	b	.L2294
.L2300:
	.align	2
.L2299:
	.word	.LANCHOR33
	.word	.LANCHOR30
	.word	.LANCHOR31
	.word	.LANCHOR32
	.word	.LC422
	.size	hynix16nm_readretry_init, .-hynix16nm_readretry_init
	.section	.text.hynix16nm_readretry_exit,"ax",%progbits
	.align	1
	.global	hynix16nm_readretry_exit
	.syntax unified
	.thumb
	.thumb_func
	.fpu softvfp
	.type	hynix16nm_readretry_exit, %function
hynix16nm_readretry_exit:
	@ args = 0, pretend = 0, frame = 0
	@ frame_needed = 0, uses_anonymous_args = 0
	push	{r4, lr}
	mov	r4, r0
	ldr	r0, .L2302
	bl	NAND_Print_DBG
	mov	r0, r4
	bl	hynix16nm_set_default_param
	movs	r0, #0
	str	r0, [r4, #176]
	pop	{r4, pc}
.L2303:
	.align	2
.L2302:
	.word	.LC423
	.size	hynix16nm_readretry_exit, .-hynix16nm_readretry_exit
	.section	.text.hynix16nm_special_init,"ax",%progbits
	.align	1
	.global	hynix16nm_special_init
	.syntax unified
	.thumb
	.thumb_func
	.fpu softvfp
	.type	hynix16nm_special_init, %function
hynix16nm_special_init:
	@ args = 0, pretend = 0, frame = 0
	@ frame_needed = 0, uses_anonymous_args = 0
	push	{r3, r4, r5, lr}
	movs	r4, #0
	ldr	r3, .L2309
	ldr	r3, [r3]
	ldr	r5, [r3, #8]
.L2305:
	cbnz	r5, .L2306
	ldr	r3, .L2309+4
	ldrb	r1, [r3]	@ zero_extendqisi2
	ldr	r3, .L2309+8
	ldrb	r2, [r3]	@ zero_extendqisi2
	ldr	r3, .L2309+12
	ldrb	r3, [r3]	@ zero_extendqisi2
	cbnz	r4, .L2307
	ldr	r0, .L2309+16
	ldr	r5, .L2309+20
	str	r5, [r0]
	ldr	r0, .L2309+24
	bl	NAND_Print_DBG
.L2304:
	mov	r0, r4
	pop	{r3, r4, r5, pc}
.L2306:
	mov	r0, r5
	bl	hynix16nm_readretry_init
	ldr	r5, [r5]
	orrs	r4, r4, r0
	b	.L2305
.L2307:
	ldr	r0, .L2309+28
	bl	NAND_Print
	b	.L2304
.L2310:
	.align	2
.L2309:
	.word	g_nsi
	.word	.LANCHOR33
	.word	.LANCHOR30
	.word	.LANCHOR31
	.word	df_read_page_end
	.word	hynix16nm_read_page_end
	.word	.LC424
	.word	.LC425
	.size	hynix16nm_special_init, .-hynix16nm_special_init
	.section	.text.hynix16nm_special_exit,"ax",%progbits
	.align	1
	.global	hynix16nm_special_exit
	.syntax unified
	.thumb
	.thumb_func
	.fpu softvfp
	.type	hynix16nm_special_exit, %function
hynix16nm_special_exit:
	@ args = 0, pretend = 0, frame = 0
	@ frame_needed = 0, uses_anonymous_args = 0
	ldr	r3, .L2314
	push	{r4, lr}
	ldr	r3, [r3]
	ldr	r4, [r3, #8]
.L2312:
	cbnz	r4, .L2313
	ldr	r0, .L2314+4
	bl	NAND_Print_DBG
	mov	r0, r4
	pop	{r4, pc}
.L2313:
	mov	r0, r4
	bl	hynix16nm_readretry_exit
	ldr	r4, [r4]
	b	.L2312
.L2315:
	.align	2
.L2314:
	.word	g_nsi
	.word	.LC426
	.size	hynix16nm_special_exit, .-hynix16nm_special_exit
	.section	.text.samsung_vender_set_param,"ax",%progbits
	.align	1
	.global	samsung_vender_set_param
	.syntax unified
	.thumb
	.thumb_func
	.fpu softvfp
	.type	samsung_vender_set_param, %function
samsung_vender_set_param:
	@ args = 0, pretend = 0, frame = 0
	@ frame_needed = 0, uses_anonymous_args = 0
	push	{r4, r5, lr}
	sub	sp, sp, #20
	str	r3, [sp, #8]
	movs	r3, #1
	mov	r4, r1
	strd	r1, r3, [sp]
	mov	r3, r2
	ldr	r1, .L2317
	movs	r2, #0
	bl	set_cmd_with_nand_bus
	mov	r5, r0
	ldrb	r0, [r4, #3]	@ zero_extendqisi2
	ldrb	r3, [r4, #2]	@ zero_extendqisi2
	ldrb	r2, [r4, #1]	@ zero_extendqisi2
	str	r0, [sp]
	ldrb	r1, [r4]	@ zero_extendqisi2
	ldr	r0, .L2317+4
	bl	NAND_Print_DBG
	mov	r0, r5
	add	sp, sp, #20
	@ sp needed
	pop	{r4, r5, pc}
.L2318:
	.align	2
.L2317:
	.word	.LANCHOR34
	.word	.LC391
	.size	samsung_vender_set_param, .-samsung_vender_set_param
	.section	.text.samsung_set_readretry,"ax",%progbits
	.align	1
	.global	samsung_set_readretry
	.syntax unified
	.thumb
	.thumb_func
	.fpu softvfp
	.type	samsung_set_readretry, %function
samsung_set_readretry:
	@ args = 0, pretend = 0, frame = 0
	@ frame_needed = 0, uses_anonymous_args = 0
	push	{r3, r4, r5, lr}
	mov	r4, r0
	bl	nand_enable_chip
	ldr	r0, [r4, #260]
	bl	ndfc_disable_randomize
	ldr	r0, [r4, #176]
	movs	r3, #4
	ldr	r1, .L2324
	ldr	r2, .L2324+4
	add	r1, r1, r0, lsl #2
	mov	r0, r4
	bl	samsung_vender_set_param
	mov	r5, r0
	cbz	r0, .L2320
	ldr	r1, [r4, #176]
	ldr	r0, .L2324+8
	bl	NAND_Print
.L2320:
	mov	r0, r4
	bl	nand_disable_chip
	mov	r0, r5
	pop	{r3, r4, r5, pc}
.L2325:
	.align	2
.L2324:
	.word	.LANCHOR36
	.word	.LANCHOR35
	.word	.LC427
	.size	samsung_set_readretry, .-samsung_set_readretry
	.section	.text.samsung_read_page_end,"ax",%progbits
	.align	1
	.global	samsung_read_page_end
	.syntax unified
	.thumb
	.thumb_func
	.fpu softvfp
	.type	samsung_read_page_end, %function
samsung_read_page_end:
	@ args = 0, pretend = 0, frame = 0
	@ frame_needed = 0, uses_anonymous_args = 0
	ldr	r3, .L2336
	push	{r0, r1, r4, r5, r6, r7, r8, lr}
	mov	r6, r0
	ldr	r1, [r0]
	ldr	r0, [r3]
	bl	nci_get_from_nsi
	mov	r5, r0
	mov	r0, r6
	bl	generic_read_page_end_not_retry
	adds	r3, r0, #2
	mov	r4, r0
	bne	.L2326
	ldr	r8, .L2336+12
	movs	r7, #1
	ldr	r0, .L2336+4
	bl	NAND_Print_DBG
.L2328:
	ldrb	r3, [r8]	@ zero_extendqisi2
	cmp	r3, r7
	ble	.L2331
	str	r7, [r5, #176]
	mov	r0, r5
	bl	samsung_set_readretry
	mov	r4, r0
	cbnz	r0, .L2329
	mov	r0, r6
	bl	generic_read_page_start
	mov	r4, r0
	mov	r0, r6
	bl	generic_read_page_end_not_retry
	orrs	r4, r4, r0
	cmp	r4, #10
	beq	.L2330
	cbnz	r4, .L2329
.L2330:
	ldr	r3, [r5, #176]
	movs	r4, #10
	ldr	r1, [r5, #260]
	ldr	r0, .L2336+8
	str	r3, [sp, #4]
	ldr	r3, [r6, #8]
	str	r3, [sp]
	ldr	r3, [r6, #4]
	ldr	r2, [r5, #20]
	ldr	r1, [r1, #8]
	bl	NAND_Print_DBG
.L2331:
	movs	r3, #0
	mov	r0, r5
	str	r3, [r5, #176]
	bl	samsung_set_readretry
.L2326:
	mov	r0, r4
	add	sp, sp, #8
	@ sp needed
	pop	{r4, r5, r6, r7, r8, pc}
.L2329:
	adds	r7, r7, #1
	b	.L2328
.L2337:
	.align	2
.L2336:
	.word	g_nsi
	.word	.LC428
	.word	.LC429
	.word	.LANCHOR37
	.size	samsung_read_page_end, .-samsung_read_page_end
	.section	.text.samsung_readretry_init,"ax",%progbits
	.align	1
	.global	samsung_readretry_init
	.syntax unified
	.thumb
	.thumb_func
	.fpu softvfp
	.type	samsung_readretry_init, %function
samsung_readretry_init:
	@ args = 0, pretend = 0, frame = 0
	@ frame_needed = 0, uses_anonymous_args = 0
	movs	r3, #0
	push	{r4, lr}
	str	r3, [r0, #176]
	movs	r1, #15
	ldr	r3, [r0, #264]
	ldr	r4, .L2341
	ldr	r0, .L2341+4
	ldrh	r3, [r3, #54]
	strb	r3, [r4]
	ldr	r3, .L2341+8
	strb	r1, [r3]
	bl	NAND_Print_DBG
	ldrb	r3, [r4]	@ zero_extendqisi2
	cmp	r3, #32
	ite	ne
	mvnne	r0, #99
	moveq	r0, #0
	pop	{r4, pc}
.L2342:
	.align	2
.L2341:
	.word	.LANCHOR38
	.word	.LC430
	.word	.LANCHOR37
	.size	samsung_readretry_init, .-samsung_readretry_init
	.section	.text.samsung_readretry_exit,"ax",%progbits
	.align	1
	.global	samsung_readretry_exit
	.syntax unified
	.thumb
	.thumb_func
	.fpu softvfp
	.type	samsung_readretry_exit, %function
samsung_readretry_exit:
	@ args = 0, pretend = 0, frame = 0
	@ frame_needed = 0, uses_anonymous_args = 0
	@ link register save eliminated.
	movs	r0, #0
	bx	lr
	.size	samsung_readretry_exit, .-samsung_readretry_exit
	.section	.text.samsung_special_init,"ax",%progbits
	.align	1
	.global	samsung_special_init
	.syntax unified
	.thumb
	.thumb_func
	.fpu softvfp
	.type	samsung_special_init, %function
samsung_special_init:
	@ args = 0, pretend = 0, frame = 0
	@ frame_needed = 0, uses_anonymous_args = 0
	push	{r3, r4, r5, lr}
	movs	r4, #0
	ldr	r3, .L2349
	ldr	r3, [r3]
	ldr	r5, [r3, #8]
.L2345:
	cbnz	r5, .L2346
	ldr	r3, .L2349+4
	ldrb	r1, [r3]	@ zero_extendqisi2
	ldr	r3, .L2349+8
	ldrb	r2, [r3]	@ zero_extendqisi2
	cbnz	r4, .L2347
	ldr	r0, .L2349+12
	ldr	r3, .L2349+16
	str	r0, [r3]
	ldr	r0, .L2349+20
	bl	NAND_Print_DBG
.L2344:
	mov	r0, r4
	pop	{r3, r4, r5, pc}
.L2346:
	mov	r0, r5
	bl	samsung_readretry_init
	ldr	r5, [r5]
	orrs	r4, r4, r0
	b	.L2345
.L2347:
	ldr	r0, .L2349+24
	bl	NAND_Print
	b	.L2344
.L2350:
	.align	2
.L2349:
	.word	g_nsi
	.word	.LANCHOR38
	.word	.LANCHOR37
	.word	samsung_read_page_end
	.word	df_read_page_end
	.word	.LC431
	.word	.LC432
	.size	samsung_special_init, .-samsung_special_init
	.section	.text.samsung_special_exit,"ax",%progbits
	.align	1
	.global	samsung_special_exit
	.syntax unified
	.thumb
	.thumb_func
	.fpu softvfp
	.type	samsung_special_exit, %function
samsung_special_exit:
	@ args = 0, pretend = 0, frame = 0
	@ frame_needed = 0, uses_anonymous_args = 0
	ldr	r3, .L2354
	push	{r4, lr}
	ldr	r3, [r3]
	ldr	r4, [r3, #8]
.L2352:
	cbnz	r4, .L2353
	ldr	r0, .L2354+4
	bl	NAND_Print_DBG
	mov	r0, r4
	pop	{r4, pc}
.L2353:
	ldr	r4, [r4]
	b	.L2352
.L2355:
	.align	2
.L2354:
	.word	g_nsi
	.word	.LC433
	.size	samsung_special_exit, .-samsung_special_exit
	.section	.text.hynix26nm_vender_set_param,"ax",%progbits
	.align	1
	.global	hynix26nm_vender_set_param
	.syntax unified
	.thumb
	.thumb_func
	.fpu softvfp
	.type	hynix26nm_vender_set_param, %function
hynix26nm_vender_set_param:
	@ args = 0, pretend = 0, frame = 0
	@ frame_needed = 0, uses_anonymous_args = 0
	@ link register save eliminated.
	b	hynix16nm_vender_set_param
	.size	hynix26nm_vender_set_param, .-hynix26nm_vender_set_param
	.section	.text.hynix26nm_set_readretry,"ax",%progbits
	.align	1
	.global	hynix26nm_set_readretry
	.syntax unified
	.thumb
	.thumb_func
	.fpu softvfp
	.type	hynix26nm_set_readretry, %function
hynix26nm_set_readretry:
	@ args = 0, pretend = 0, frame = 8
	@ frame_needed = 0, uses_anonymous_args = 0
	ldr	r3, .L2366
	push	{r0, r1, r4, r5, r6, r7, r8, lr}
	ldr	r1, [r0, #176]
	ldrb	r3, [r3]	@ zero_extendqisi2
	cmp	r1, r3
	bhi	.L2365
	ldr	r4, .L2366+4
	movs	r6, #0
	ldr	r7, .L2366+8
	add	r5, sp, #4
	ldrb	r3, [r4]	@ zero_extendqisi2
	add	r7, r7, r1, lsl #3
.L2359:
	cmp	r3, r6
	bhi	.L2360
	subs	r2, r1, #2
	subs	r1, r1, #5
	cmp	r2, #4
	ldr	r8, .L2366+24
	it	ls
	movls	r2, #0
	mov	r6, #0
	it	ls
	strbls	r2, [sp, #4]
	cmp	r1, #1
	it	ls
	movls	r2, #0
	mov	r1, r5
	it	ls
	strbls	r2, [sp, #5]
	ldr	r2, .L2366+12
	bl	hynix16nm_vender_set_param
	mov	r7, r0
	ldr	r0, .L2366+16
	bl	NAND_Print_DBG
.L2363:
	ldrb	r3, [r4]	@ zero_extendqisi2
	cmp	r3, r6
	bhi	.L2364
	ldr	r0, .L2366+20
	bl	NAND_Print_DBG
.L2357:
	mov	r0, r7
	add	sp, sp, #8
	@ sp needed
	pop	{r4, r5, r6, r7, r8, pc}
.L2360:
	adds	r2, r0, r6
	ldrh	ip, [r7, r6, lsl #1]
	ldrb	r2, [r2, #48]	@ zero_extendqisi2
	add	r2, r2, ip
	sxth	r2, r2
	usat	r2, #8, r2
	strb	r2, [r5, r6]
	adds	r6, r6, #1
	b	.L2359
.L2364:
	ldrb	r1, [r5, r6]	@ zero_extendqisi2
	mov	r0, r8
	bl	NAND_Print_DBG
	adds	r6, r6, #1
	b	.L2363
.L2365:
	mvn	r7, #52
	b	.L2357
.L2367:
	.align	2
.L2366:
	.word	.LANCHOR39
	.word	.LANCHOR40
	.word	.LANCHOR41
	.word	.LANCHOR42
	.word	.LC434
	.word	.LC7
	.word	.LC435
	.size	hynix26nm_set_readretry, .-hynix26nm_set_readretry
	.section	.text.hynix26nm_read_page_end,"ax",%progbits
	.align	1
	.global	hynix26nm_read_page_end
	.syntax unified
	.thumb
	.thumb_func
	.fpu softvfp
	.type	hynix26nm_read_page_end, %function
hynix26nm_read_page_end:
	@ args = 0, pretend = 0, frame = 0
	@ frame_needed = 0, uses_anonymous_args = 0
	ldr	r3, .L2381
	push	{r0, r1, r2, r4, r5, r6, r7, r8, r10, lr}
	mov	r6, r0
	ldr	r1, [r0]
	ldr	r0, [r3]
	bl	nci_get_from_nsi
	mov	r5, r0
	mov	r0, r6
	bl	generic_read_page_end_not_retry
	adds	r3, r0, #2
	mov	r4, r0
	bne	.L2368
	ldr	r7, .L2381+4
	mov	r10, #0
	mov	r8, r10
	ldr	r0, .L2381+8
	bl	NAND_Print_DBG
.L2370:
	ldrb	r2, [r7]	@ zero_extendqisi2
	cmp	r2, r10
	ble	.L2374
	ldr	r3, [r5, #176]
	mov	r0, r5
	adds	r3, r3, #1
	cmp	r3, r2
	it	hi
	movhi	r3, r8
	str	r3, [r5, #176]
	bl	hynix26nm_set_readretry
	mov	r4, r0
	cbz	r0, .L2373
	movs	r3, #0
	mov	r0, r5
	str	r3, [r5, #176]
	bl	hynix26nm_set_readretry
.L2374:
	movs	r3, #0
	mov	r0, r5
	str	r3, [r5, #176]
	bl	hynix26nm_set_readretry
.L2368:
	mov	r0, r4
	add	sp, sp, #12
	@ sp needed
	pop	{r4, r5, r6, r7, r8, r10, pc}
.L2373:
	mov	r0, r6
	bl	generic_read_page_start
	mov	r4, r0
	mov	r0, r6
	bl	generic_read_page_end_not_retry
	orrs	r4, r4, r0
	cmp	r4, #10
	beq	.L2375
	cbnz	r4, .L2376
.L2375:
	ldr	r3, [r5, #176]
	movs	r4, #10
	ldr	r1, [r5, #260]
	ldr	r0, .L2381+12
	str	r3, [sp, #4]
	ldr	r3, [r6, #8]
	str	r3, [sp]
	ldr	r3, [r6, #4]
	ldr	r2, [r5, #20]
	ldr	r1, [r1, #8]
	bl	NAND_Print_DBG
	b	.L2374
.L2376:
	add	r10, r10, #1
	b	.L2370
.L2382:
	.align	2
.L2381:
	.word	g_nsi
	.word	.LANCHOR39
	.word	.LC436
	.word	.LC437
	.size	hynix26nm_read_page_end, .-hynix26nm_read_page_end
	.section	.text.hynix26nm_vender_get_param,"ax",%progbits
	.align	1
	.global	hynix26nm_vender_get_param
	.syntax unified
	.thumb
	.thumb_func
	.fpu softvfp
	.type	hynix26nm_vender_get_param, %function
hynix26nm_vender_get_param:
	@ args = 0, pretend = 0, frame = 0
	@ frame_needed = 0, uses_anonymous_args = 0
	@ link register save eliminated.
	b	hynix16nm_vender_get_param
	.size	hynix26nm_vender_get_param, .-hynix26nm_vender_get_param
	.section	.text.hynix26nm_lsb_init,"ax",%progbits
	.align	1
	.global	hynix26nm_lsb_init
	.syntax unified
	.thumb
	.thumb_func
	.fpu softvfp
	.type	hynix26nm_lsb_init, %function
hynix26nm_lsb_init:
	@ args = 0, pretend = 0, frame = 0
	@ frame_needed = 0, uses_anonymous_args = 0
	@ link register save eliminated.
	ldr	r3, [r0, #264]
	movs	r0, #0
	ldr	r2, .L2385
	ldr	r3, [r3, #52]
	lsrs	r1, r3, #16
	strb	r1, [r2]
	lsrs	r1, r3, #8
	ldr	r2, .L2385+4
	strb	r1, [r2]
	ldr	r2, .L2385+8
	strb	r3, [r2]
	movs	r2, #5
	ldr	r3, .L2385+12
	strb	r2, [r3]
	movs	r2, #164
	ldr	r3, .L2385+16
	strb	r2, [r3]
	movs	r2, #165
	strb	r2, [r3, #1]
	movs	r2, #176
	strb	r2, [r3, #2]
	movs	r2, #177
	strb	r2, [r3, #3]
	movs	r2, #201
	strb	r2, [r3, #4]
	movs	r2, #37
	ldr	r3, .L2385+20
	strb	r2, [r3]
	strb	r2, [r3, #1]
	strb	r2, [r3, #2]
	strb	r2, [r3, #3]
	movs	r2, #1
	strb	r2, [r3, #4]
	bx	lr
.L2386:
	.align	2
.L2385:
	.word	.LANCHOR43
	.word	.LANCHOR39
	.word	.LANCHOR40
	.word	.LANCHOR44
	.word	.LANCHOR45
	.word	.LANCHOR46
	.size	hynix26nm_lsb_init, .-hynix26nm_lsb_init
	.section	.text.hynix26nm_lsb_enable,"ax",%progbits
	.align	1
	.global	hynix26nm_lsb_enable
	.syntax unified
	.thumb
	.thumb_func
	.fpu softvfp
	.type	hynix26nm_lsb_enable, %function
hynix26nm_lsb_enable:
	@ args = 0, pretend = 0, frame = 8
	@ frame_needed = 0, uses_anonymous_args = 0
	push	{r0, r1, r4, r5, r6, lr}
	mov	r5, r0
	ldr	r4, .L2390
	ldr	r2, .L2390+4
	ldrb	r3, [r4]	@ zero_extendqisi2
	ldr	r1, .L2390+8
	bl	hynix16nm_vender_get_param
	ldrb	r3, [r4]	@ zero_extendqisi2
	movs	r2, #0
	ldr	r0, .L2390+8
	ldr	r4, .L2390+12
.L2388:
	cmp	r3, r2
	bhi	.L2389
	ldr	r2, .L2390+4
	mov	r1, sp
	mov	r0, r5
	bl	hynix16nm_vender_set_param
	movs	r0, #0
	add	sp, sp, #8
	@ sp needed
	pop	{r4, r5, r6, pc}
.L2389:
	ldrb	r1, [r2, r0]	@ zero_extendqisi2
	ldrb	r6, [r2, r4]	@ zero_extendqisi2
	add	r1, r1, r6
	strb	r1, [sp, r2]
	adds	r2, r2, #1
	b	.L2388
.L2391:
	.align	2
.L2390:
	.word	.LANCHOR44
	.word	.LANCHOR45
	.word	.LANCHOR47
	.word	.LANCHOR46
	.size	hynix26nm_lsb_enable, .-hynix26nm_lsb_enable
	.section	.text.hynix26nm_lsb_disable,"ax",%progbits
	.align	1
	.global	hynix26nm_lsb_disable
	.syntax unified
	.thumb
	.thumb_func
	.fpu softvfp
	.type	hynix26nm_lsb_disable, %function
hynix26nm_lsb_disable:
	@ args = 0, pretend = 0, frame = 0
	@ frame_needed = 0, uses_anonymous_args = 0
	push	{r3, lr}
	ldr	r3, .L2393
	ldr	r2, .L2393+4
	ldrb	r3, [r3]	@ zero_extendqisi2
	ldr	r1, .L2393+8
	bl	hynix16nm_vender_set_param
	movs	r0, #0
	pop	{r3, pc}
.L2394:
	.align	2
.L2393:
	.word	.LANCHOR44
	.word	.LANCHOR45
	.word	.LANCHOR47
	.size	hynix26nm_lsb_disable, .-hynix26nm_lsb_disable
	.section	.text.hynix26nm_lsb_exit,"ax",%progbits
	.align	1
	.global	hynix26nm_lsb_exit
	.syntax unified
	.thumb
	.thumb_func
	.fpu softvfp
	.type	hynix26nm_lsb_exit, %function
hynix26nm_lsb_exit:
	@ args = 0, pretend = 0, frame = 0
	@ frame_needed = 0, uses_anonymous_args = 0
	push	{r4, lr}
	mov	r4, r0
	bl	nand_enable_chip
	ldr	r0, [r4, #260]
	bl	ndfc_disable_randomize
	movs	r2, #0
	mov	r0, r4
	mov	r1, r2
	bl	set_one_cmd
	movs	r1, #0
	mov	r0, r4
	bl	set_one_addr
	movs	r1, #0
	mov	r0, r4
	bl	set_one_addr
	movs	r1, #0
	mov	r0, r4
	bl	set_one_addr
	movs	r1, #0
	mov	r0, r4
	bl	set_one_addr
	movs	r1, #0
	mov	r0, r4
	bl	set_one_addr
	movs	r2, #1
	movs	r1, #48
	mov	r0, r4
	bl	set_one_cmd
	ldr	r0, .L2396
	bl	NAND_Print_DBG
	mov	r0, r4
	bl	nand_disable_chip
	movs	r0, #0
	pop	{r4, pc}
.L2397:
	.align	2
.L2396:
	.word	.LC438
	.size	hynix26nm_lsb_exit, .-hynix26nm_lsb_exit
	.section	.text.hynix26nm_getdefaultparam,"ax",%progbits
	.align	1
	.global	hynix26nm_getdefaultparam
	.syntax unified
	.thumb
	.thumb_func
	.fpu softvfp
	.type	hynix26nm_getdefaultparam, %function
hynix26nm_getdefaultparam:
	@ args = 0, pretend = 0, frame = 0
	@ frame_needed = 0, uses_anonymous_args = 0
	push	{r4, r5, r6, lr}
	add	r4, r0, #48
	ldr	r5, .L2401
	mov	r6, r1
	mov	r1, r4
	ldr	r2, .L2401+4
	ldrb	r3, [r5]	@ zero_extendqisi2
	bl	hynix16nm_vender_get_param
	mov	r3, r6
.L2399:
	ldrb	r1, [r5]	@ zero_extendqisi2
	subs	r2, r3, r6
	cmp	r1, r2
	bgt	.L2400
	pop	{r4, r5, r6, pc}
.L2400:
	ldrb	r2, [r4], #1	@ zero_extendqisi2
	strb	r2, [r3], #1
	b	.L2399
.L2402:
	.align	2
.L2401:
	.word	.LANCHOR40
	.word	.LANCHOR42
	.size	hynix26nm_getdefaultparam, .-hynix26nm_getdefaultparam
	.section	.text.hynix26nm_setdefaultparam,"ax",%progbits
	.align	1
	.global	hynix26nm_setdefaultparam
	.syntax unified
	.thumb
	.thumb_func
	.fpu softvfp
	.type	hynix26nm_setdefaultparam, %function
hynix26nm_setdefaultparam:
	@ args = 0, pretend = 0, frame = 8
	@ frame_needed = 0, uses_anonymous_args = 0
	push	{r0, r1, r2, r4, r5, r6, r7, lr}
	add	r1, r0, #48
	ldr	r5, .L2408
	movs	r2, #0
	ldrb	r3, [r5]	@ zero_extendqisi2
.L2404:
	cmp	r3, r2
	bgt	.L2405
	ldr	r7, .L2408+4
	mov	r1, sp
	ldr	r2, .L2408+8
	movs	r4, #0
	bl	hynix16nm_vender_set_param
	mov	r6, r0
	ldr	r0, .L2408+12
	bl	NAND_Print_DBG
.L2406:
	ldrb	r3, [r5]	@ zero_extendqisi2
	cmp	r3, r4
	bgt	.L2407
	ldr	r0, .L2408+16
	bl	NAND_Print_DBG
	mov	r0, r6
	add	sp, sp, #12
	@ sp needed
	pop	{r4, r5, r6, r7, pc}
.L2405:
	ldrb	r4, [r1], #1	@ zero_extendqisi2
	strb	r4, [sp, r2]
	adds	r2, r2, #1
	b	.L2404
.L2407:
	ldrb	r1, [sp, r4]	@ zero_extendqisi2
	mov	r0, r7
	bl	NAND_Print_DBG
	adds	r4, r4, #1
	b	.L2406
.L2409:
	.align	2
.L2408:
	.word	.LANCHOR40
	.word	.LC416
	.word	.LANCHOR42
	.word	.LC415
	.word	.LC7
	.size	hynix26nm_setdefaultparam, .-hynix26nm_setdefaultparam
	.section	.text.hynix26nm_get_default_param,"ax",%progbits
	.align	1
	.global	hynix26nm_get_default_param
	.syntax unified
	.thumb
	.thumb_func
	.fpu softvfp
	.type	hynix26nm_get_default_param, %function
hynix26nm_get_default_param:
	@ args = 0, pretend = 0, frame = 8
	@ frame_needed = 0, uses_anonymous_args = 0
	push	{r4, lr}
	sub	sp, sp, #24
	add	r1, sp, #20
	mov	r4, r0
	bl	hynix26nm_getdefaultparam
	ldrb	r2, [sp, #23]	@ zero_extendqisi2
	ldr	r1, [r4, #260]
	ldrb	r3, [sp, #20]	@ zero_extendqisi2
	str	r2, [sp, #8]
	ldrb	r2, [sp, #22]	@ zero_extendqisi2
	ldr	r0, .L2411
	str	r2, [sp, #4]
	ldrb	r2, [sp, #21]	@ zero_extendqisi2
	str	r2, [sp]
	ldr	r2, [r4, #20]
	ldr	r1, [r1, #8]
	bl	NAND_Print_DBG
	mov	r0, r4
	bl	hynix26nm_setdefaultparam
	movs	r0, #0
	add	sp, sp, #24
	@ sp needed
	pop	{r4, pc}
.L2412:
	.align	2
.L2411:
	.word	.LC439
	.size	hynix26nm_get_default_param, .-hynix26nm_get_default_param
	.section	.text.hynix26nm_readretry_init,"ax",%progbits
	.align	1
	.global	hynix26nm_readretry_init
	.syntax unified
	.thumb
	.thumb_func
	.fpu softvfp
	.type	hynix26nm_readretry_init, %function
hynix26nm_readretry_init:
	@ args = 0, pretend = 0, frame = 0
	@ frame_needed = 0, uses_anonymous_args = 0
	push	{r3, r4, r5, r6, r7, lr}
	ldr	r3, [r0, #264]
	ldr	r4, .L2428
	ldr	r2, [r3, #52]
	ldr	r3, .L2428+4
	ubfx	r1, r2, #16, #8
	strb	r1, [r3]
	lsrs	r3, r2, #8
	strb	r3, [r4]
	ldr	r4, .L2428+8
	strb	r2, [r4]
	cbnz	r1, .L2414
	ldr	r4, .L2428+12
	movs	r5, #172
	uxtb	r3, r3
	ldr	r6, .L2428+16
	uxtb	r2, r2
	strb	r5, [r4]
	movs	r5, #173
	strb	r5, [r4, #1]
	movs	r5, #174
	strb	r5, [r4, #2]
	movs	r5, #175
	strb	r5, [r4, #3]
	adds	r3, r3, #1
	ldr	r5, .L2428+20
.L2415:
	cmp	r3, r1
	bhi	.L2425
.L2418:
	add	r2, r0, #48
	add	r1, r0, #176
	movs	r3, #0
	str	r3, [r0, #176]
.L2424:
	strb	r3, [r2], #1
	cmp	r1, r2
	bne	.L2424
	bl	hynix26nm_get_default_param
	movs	r0, #0
	b	.L2413
.L2416:
	ldrh	r7, [r5, r4, lsl #1]
	strh	r7, [r6, r4, lsl #1]	@ movhi
	adds	r4, r4, #1
.L2417:
	cmp	r4, r2
	bcc	.L2416
	adds	r1, r1, #1
	adds	r5, r5, #8
	adds	r6, r6, #8
	b	.L2415
.L2425:
	movs	r4, #0
	b	.L2417
.L2414:
	cmp	r1, #1
	bne	.L2419
	ldr	r1, .L2428+12
	movs	r4, #167
	uxtb	r3, r3
	ldr	r5, .L2428+24
	ldr	r6, .L2428+16
	uxtb	r2, r2
	strb	r4, [r1]
	movs	r4, #173
	strb	r4, [r1, #1]
	movs	r4, #174
	strb	r4, [r1, #2]
	movs	r4, #175
	strb	r4, [r1, #3]
	adds	r3, r3, #1
	movs	r4, #0
.L2420:
	cmp	r3, r4
	bls	.L2418
	movs	r1, #0
	b	.L2422
.L2421:
	ldrh	r7, [r5, r1, lsl #1]
	strh	r7, [r6, r1, lsl #1]	@ movhi
	adds	r1, r1, #1
.L2422:
	cmp	r1, r2
	bcc	.L2421
	adds	r4, r4, #1
	adds	r5, r5, #8
	adds	r6, r6, #8
	b	.L2420
.L2419:
	ldr	r0, .L2428+28
	bl	NAND_Print
	mvn	r0, #53
.L2413:
	pop	{r3, r4, r5, r6, r7, pc}
.L2429:
	.align	2
.L2428:
	.word	.LANCHOR39
	.word	.LANCHOR43
	.word	.LANCHOR40
	.word	.LANCHOR42
	.word	.LANCHOR41
	.word	.LANCHOR48
	.word	.LANCHOR49
	.word	.LC422
	.size	hynix26nm_readretry_init, .-hynix26nm_readretry_init
	.section	.text.hynix26nm_readretry_exit,"ax",%progbits
	.align	1
	.global	hynix26nm_readretry_exit
	.syntax unified
	.thumb
	.thumb_func
	.fpu softvfp
	.type	hynix26nm_readretry_exit, %function
hynix26nm_readretry_exit:
	@ args = 0, pretend = 0, frame = 0
	@ frame_needed = 0, uses_anonymous_args = 0
	push	{r4, lr}
	mov	r4, r0
	ldr	r0, .L2431
	bl	NAND_Print_DBG
	mov	r0, r4
	bl	hynix26nm_setdefaultparam
	movs	r0, #0
	str	r0, [r4, #176]
	pop	{r4, pc}
.L2432:
	.align	2
.L2431:
	.word	.LC440
	.size	hynix26nm_readretry_exit, .-hynix26nm_readretry_exit
	.section	.text.hynix26nm_special_init,"ax",%progbits
	.align	1
	.global	hynix26nm_special_init
	.syntax unified
	.thumb
	.thumb_func
	.fpu softvfp
	.type	hynix26nm_special_init, %function
hynix26nm_special_init:
	@ args = 0, pretend = 0, frame = 0
	@ frame_needed = 0, uses_anonymous_args = 0
	push	{r3, r4, r5, lr}
	movs	r4, #0
	ldr	r3, .L2438
	ldr	r3, [r3]
	ldr	r5, [r3, #8]
.L2434:
	cbnz	r5, .L2435
	ldr	r3, .L2438+4
	ldrb	r1, [r3]	@ zero_extendqisi2
	ldr	r3, .L2438+8
	ldrb	r2, [r3]	@ zero_extendqisi2
	ldr	r3, .L2438+12
	ldrb	r3, [r3]	@ zero_extendqisi2
	cbnz	r4, .L2436
	ldr	r0, .L2438+16
	ldr	r5, .L2438+20
	str	r5, [r0]
	ldr	r0, .L2438+24
	bl	NAND_Print_DBG
.L2433:
	mov	r0, r4
	pop	{r3, r4, r5, pc}
.L2435:
	mov	r0, r5
	bl	hynix26nm_readretry_init
	ldr	r5, [r5]
	orrs	r4, r4, r0
	b	.L2434
.L2436:
	ldr	r0, .L2438+28
	bl	NAND_Print
	b	.L2433
.L2439:
	.align	2
.L2438:
	.word	g_nsi
	.word	.LANCHOR43
	.word	.LANCHOR39
	.word	.LANCHOR40
	.word	df_read_page_end
	.word	hynix26nm_read_page_end
	.word	.LC441
	.word	.LC442
	.size	hynix26nm_special_init, .-hynix26nm_special_init
	.section	.text.hynix26nm_special_exit,"ax",%progbits
	.align	1
	.global	hynix26nm_special_exit
	.syntax unified
	.thumb
	.thumb_func
	.fpu softvfp
	.type	hynix26nm_special_exit, %function
hynix26nm_special_exit:
	@ args = 0, pretend = 0, frame = 0
	@ frame_needed = 0, uses_anonymous_args = 0
	ldr	r3, .L2443
	push	{r4, lr}
	ldr	r3, [r3]
	ldr	r4, [r3, #8]
.L2441:
	cbnz	r4, .L2442
	ldr	r0, .L2443+4
	bl	NAND_Print_DBG
	mov	r0, r4
	pop	{r4, pc}
.L2442:
	mov	r0, r4
	bl	hynix26nm_readretry_exit
	ldr	r4, [r4]
	b	.L2441
.L2444:
	.align	2
.L2443:
	.word	g_nsi
	.word	.LC443
	.size	hynix26nm_special_exit, .-hynix26nm_special_exit
	.section	.text.sandisk_A19_dsp_on,"ax",%progbits
	.align	1
	.global	sandisk_A19_dsp_on
	.syntax unified
	.thumb
	.thumb_func
	.fpu softvfp
	.type	sandisk_A19_dsp_on, %function
sandisk_A19_dsp_on:
	@ args = 0, pretend = 0, frame = 0
	@ frame_needed = 0, uses_anonymous_args = 0
	ldr	r3, .L2446
	push	{r4, lr}
	ldr	r1, [r0]
	ldr	r0, [r3]
	bl	nci_get_from_nsi
	mov	r4, r0
	bl	nand_enable_chip
	ldr	r0, [r4, #260]
	bl	ndfc_disable_randomize
	movs	r2, #0
	movs	r1, #38
	mov	r0, r4
	bl	set_one_cmd
	mov	r0, r4
	bl	nand_disable_chip
	movs	r0, #0
	pop	{r4, pc}
.L2447:
	.align	2
.L2446:
	.word	g_nsi
	.size	sandisk_A19_dsp_on, .-sandisk_A19_dsp_on
	.section	.text.sandisk_A19_cmd25,"ax",%progbits
	.align	1
	.global	sandisk_A19_cmd25
	.syntax unified
	.thumb
	.thumb_func
	.fpu softvfp
	.type	sandisk_A19_cmd25, %function
sandisk_A19_cmd25:
	@ args = 0, pretend = 0, frame = 0
	@ frame_needed = 0, uses_anonymous_args = 0
	ldr	r3, .L2449
	push	{r4, lr}
	ldr	r1, [r0]
	ldr	r0, [r3]
	bl	nci_get_from_nsi
	mov	r4, r0
	bl	nand_enable_chip
	ldr	r0, [r4, #260]
	bl	ndfc_disable_randomize
	movs	r2, #0
	movs	r1, #37
	mov	r0, r4
	bl	set_one_cmd
	mov	r0, r4
	bl	nand_disable_chip
	movs	r0, #0
	pop	{r4, pc}
.L2450:
	.align	2
.L2449:
	.word	g_nsi
	.size	sandisk_A19_cmd25, .-sandisk_A19_cmd25
	.section	.text.sandisk_A19_set_lmflgfix_next,"ax",%progbits
	.align	1
	.global	sandisk_A19_set_lmflgfix_next
	.syntax unified
	.thumb
	.thumb_func
	.fpu softvfp
	.type	sandisk_A19_set_lmflgfix_next, %function
sandisk_A19_set_lmflgfix_next:
	@ args = 0, pretend = 0, frame = 8
	@ frame_needed = 0, uses_anonymous_args = 0
	ldr	r3, .L2456
	push	{r4, r5, r6, r7, r8, lr}
	mov	r5, r1
	sub	sp, sp, #24
	ldr	r1, [r0]
	add	r7, sp, #22
	ldr	r0, [r3]
	bl	nci_get_from_nsi
	mov	r4, r0
	bl	nand_enable_chip
	ldr	r0, [r4, #260]
	bl	ndfc_disable_randomize
	movs	r2, #0
	movs	r1, #92
	mov	r0, r4
	bl	set_one_cmd
	movs	r2, #0
	movs	r1, #197
	mov	r0, r4
	bl	set_one_cmd
	movs	r3, #0
	movs	r2, #1
	strb	r3, [sp, #21]
	movs	r3, #85
	strb	r3, [sp, #23]
	add	r1, sp, #23
	add	r3, sp, #21
	strd	r2, r2, [sp, #4]
	str	r7, [sp]
	mov	r0, r4
	strb	r2, [sp, #22]
	bl	set_cmd_with_nand_bus
	ldr	r3, .L2456+4
	ldrb	r3, [r3]	@ zero_extendqisi2
	cmp	r3, #52
	bne	.L2452
	movs	r3, #34
.L2455:
	strb	r5, [sp, #22]
	movs	r5, #1
	strb	r3, [sp, #21]
	mov	r8, #85
	add	r3, sp, #21
	mov	r2, r5
	add	r1, sp, #23
	strd	r5, r5, [sp, #4]
	str	r7, [sp]
	mov	r0, r4
	movs	r6, #0
	strb	r8, [sp, #23]
	bl	set_cmd_with_nand_bus
	add	r3, sp, #21
	mov	r2, r5
	add	r1, sp, #23
	strd	r5, r5, [sp, #4]
	str	r7, [sp]
	mov	r0, r4
	strb	r6, [sp, #21]
	strb	r6, [sp, #22]
	strb	r8, [sp, #23]
	bl	set_cmd_with_nand_bus
	mov	r0, r4
	bl	nand_disable_chip
	mov	r0, r6
	add	sp, sp, #24
	@ sp needed
	pop	{r4, r5, r6, r7, r8, pc}
.L2452:
	cmp	r3, #53
	ite	eq
	moveq	r3, #37
	movne	r3, #35
	b	.L2455
.L2457:
	.align	2
.L2456:
	.word	g_nsi
	.word	.LANCHOR50
	.size	sandisk_A19_set_lmflgfix_next, .-sandisk_A19_set_lmflgfix_next
	.section	.text.sandisk_A19_readretry_init,"ax",%progbits
	.align	1
	.global	sandisk_A19_readretry_init
	.syntax unified
	.thumb
	.thumb_func
	.fpu softvfp
	.type	sandisk_A19_readretry_init, %function
sandisk_A19_readretry_init:
	@ args = 0, pretend = 0, frame = 0
	@ frame_needed = 0, uses_anonymous_args = 0
	ldr	r3, [r0, #264]
	ldr	r2, .L2461
	push	{r4, lr}
	movs	r4, #0
	ldr	r3, [r3, #52]
	str	r4, [r0, #176]
	ubfx	r1, r3, #16, #8
	ldr	r0, .L2461+4
	strb	r1, [r2]
	lsrs	r2, r3, #8
	strb	r2, [r0]
	ldr	r0, .L2461+8
	strb	r3, [r0]
	sub	r3, r1, #50
	cmp	r3, #3
	bls	.L2459
	ldr	r0, .L2461+12
	bl	NAND_Print
	mvn	r0, #98
.L2458:
	pop	{r4, pc}
.L2459:
	uxtb	r1, r2
	ldr	r0, .L2461+16
	bl	NAND_Print_DBG
	mov	r0, r4
	b	.L2458
.L2462:
	.align	2
.L2461:
	.word	.LANCHOR50
	.word	.LANCHOR51
	.word	.LANCHOR52
	.word	.LC444
	.word	.LC445
	.size	sandisk_A19_readretry_init, .-sandisk_A19_readretry_init
	.section	.text.sandisk_A19_readretry_exit,"ax",%progbits
	.align	1
	.global	sandisk_A19_readretry_exit
	.syntax unified
	.thumb
	.thumb_func
	.fpu softvfp
	.type	sandisk_A19_readretry_exit, %function
sandisk_A19_readretry_exit:
	@ args = 0, pretend = 0, frame = 0
	@ frame_needed = 0, uses_anonymous_args = 0
	@ link register save eliminated.
	movs	r0, #0
	bx	lr
	.size	sandisk_A19_readretry_exit, .-sandisk_A19_readretry_exit
	.section	.text.sandisk_A19_read_retry_clock_save,"ax",%progbits
	.align	1
	.global	sandisk_A19_read_retry_clock_save
	.syntax unified
	.thumb
	.thumb_func
	.fpu softvfp
	.type	sandisk_A19_read_retry_clock_save, %function
sandisk_A19_read_retry_clock_save:
	@ args = 0, pretend = 0, frame = 0
	@ frame_needed = 0, uses_anonymous_args = 0
	push	{r4, lr}
	mov	r4, r0
	ldr	r3, .L2465
	ldr	r0, [r0, #260]
	ldr	r3, [r3]
	ldr	r2, .L2465+4
	ldr	r1, .L2465+8
	ldr	r0, [r0, #8]
	blx	r3
	ldr	r0, [r4, #260]
	movs	r2, #20
	ldr	r3, .L2465+12
	movs	r1, #10
	ldr	r0, [r0, #8]
	ldr	r3, [r3]
	blx	r3
	movs	r0, #0
	pop	{r4, pc}
.L2466:
	.align	2
.L2465:
	.word	NAND_GetClk
	.word	.LANCHOR53
	.word	.LANCHOR54
	.word	NAND_SetClk
	.size	sandisk_A19_read_retry_clock_save, .-sandisk_A19_read_retry_clock_save
	.section	.text.sandisk_A19_read_retry_clock_recover,"ax",%progbits
	.align	1
	.global	sandisk_A19_read_retry_clock_recover
	.syntax unified
	.thumb
	.thumb_func
	.fpu softvfp
	.type	sandisk_A19_read_retry_clock_recover, %function
sandisk_A19_read_retry_clock_recover:
	@ args = 0, pretend = 0, frame = 0
	@ frame_needed = 0, uses_anonymous_args = 0
	push	{r3, lr}
	ldr	r0, [r0, #260]
	ldr	r3, .L2468
	ldr	r2, .L2468+4
	ldr	r1, .L2468+8
	ldr	r3, [r3]
	ldr	r2, [r2]
	ldr	r1, [r1]
	ldr	r0, [r0, #8]
	blx	r3
	movs	r0, #0
	pop	{r3, pc}
.L2469:
	.align	2
.L2468:
	.word	NAND_SetClk
	.word	.LANCHOR53
	.word	.LANCHOR54
	.size	sandisk_A19_read_retry_clock_recover, .-sandisk_A19_read_retry_clock_recover
	.section	.text.sandisk_A19_vender_set_param,"ax",%progbits
	.align	1
	.global	sandisk_A19_vender_set_param
	.syntax unified
	.thumb
	.thumb_func
	.fpu softvfp
	.type	sandisk_A19_vender_set_param, %function
sandisk_A19_vender_set_param:
	@ args = 0, pretend = 0, frame = 0
	@ frame_needed = 0, uses_anonymous_args = 0
	push	{r0, r1, r2, r3, r4, lr}
	movs	r4, #1
	movs	r3, #4
	str	r1, [sp]
	strd	r3, r4, [sp, #4]
	mov	r3, r2
	ldr	r1, .L2471
	mov	r2, r4
	bl	set_cmd_with_nand_bus
	add	sp, sp, #16
	@ sp needed
	pop	{r4, pc}
.L2472:
	.align	2
.L2471:
	.word	.LANCHOR55
	.size	sandisk_A19_vender_set_param, .-sandisk_A19_vender_set_param
	.section	.text.sandisk_A19_set_readretry,"ax",%progbits
	.align	1
	.global	sandisk_A19_set_readretry
	.syntax unified
	.thumb
	.thumb_func
	.fpu softvfp
	.type	sandisk_A19_set_readretry, %function
sandisk_A19_set_readretry:
	@ args = 0, pretend = 0, frame = 0
	@ frame_needed = 0, uses_anonymous_args = 0
	push	{r3, r4, r5, lr}
	mov	r4, r0
	bl	nand_enable_chip
	ldr	r0, [r4, #260]
	bl	ndfc_disable_randomize
	ldr	r3, .L2478
	ldr	r1, [r4, #176]
	ldrb	r3, [r3]	@ zero_extendqisi2
	cmp	r3, #52
	bne	.L2474
	ldr	r3, .L2478+4
	add	r1, r3, r1, lsl #2
.L2475:
	movs	r3, #1
	ldr	r2, .L2478+8
	mov	r0, r4
	bl	sandisk_A19_vender_set_param
	movs	r2, #0
	mov	r5, r0
	movs	r1, #93
	mov	r0, r4
	bl	set_one_cmd
	mov	r0, r4
	bl	nand_disable_chip
	mov	r0, r5
	pop	{r3, r4, r5, pc}
.L2474:
	cmp	r3, #53
	lsl	r1, r1, #2
	ite	eq
	ldreq	r3, .L2478+12
	ldrne	r3, .L2478+16
	add	r1, r1, r3
	b	.L2475
.L2479:
	.align	2
.L2478:
	.word	.LANCHOR50
	.word	.LANCHOR56
	.word	.LANCHOR59
	.word	.LANCHOR57
	.word	.LANCHOR58
	.size	sandisk_A19_set_readretry, .-sandisk_A19_set_readretry
	.section	.text.sandisk_A19_exit_readretry,"ax",%progbits
	.align	1
	.global	sandisk_A19_exit_readretry
	.syntax unified
	.thumb
	.thumb_func
	.fpu softvfp
	.type	sandisk_A19_exit_readretry, %function
sandisk_A19_exit_readretry:
	@ args = 0, pretend = 0, frame = 8
	@ frame_needed = 0, uses_anonymous_args = 0
	push	{r0, r1, r2, r4, r5, lr}
	movs	r3, #0
	mov	r4, r0
	add	r5, sp, #8
	str	r3, [r5, #-4]!
	bl	nand_enable_chip
	ldr	r0, [r4, #260]
	bl	ndfc_disable_randomize
	movs	r2, #1
	movs	r1, #255
	mov	r0, r4
	bl	set_one_cmd
	mov	r1, r5
	movs	r3, #1
	ldr	r2, .L2481
	mov	r0, r4
	bl	sandisk_A19_vender_set_param
	mov	r5, r0
	mov	r0, r4
	bl	nand_disable_chip
	mov	r0, r5
	add	sp, sp, #12
	@ sp needed
	pop	{r4, r5, pc}
.L2482:
	.align	2
.L2481:
	.word	.LANCHOR59
	.size	sandisk_A19_exit_readretry, .-sandisk_A19_exit_readretry
	.section	.text.sandisk_A19_read_page_end,"ax",%progbits
	.align	1
	.global	sandisk_A19_read_page_end
	.syntax unified
	.thumb
	.thumb_func
	.fpu softvfp
	.type	sandisk_A19_read_page_end, %function
sandisk_A19_read_page_end:
	@ args = 0, pretend = 0, frame = 0
	@ frame_needed = 0, uses_anonymous_args = 0
	ldr	r3, .L2531
	push	{r0, r1, r4, r5, r6, r7, r8, r10, fp, lr}
	mov	r4, r0
	ldr	r1, [r0]
	ldr	r0, [r3]
	bl	nci_get_from_nsi
	mov	r5, r0
	mov	r0, r4
	bl	generic_read_page_end_not_retry
	adds	r3, r0, #2
	mov	r6, r0
	bne	.L2483
	ldr	r8, .L2531+28
	movs	r7, #0
	ldr	r0, .L2531+4
	bl	NAND_Print_DBG
	mov	r10, r8
.L2485:
	ldrb	r3, [r8]	@ zero_extendqisi2
	cmp	r3, r7
	bgt	.L2491
	ldr	r7, .L2531+8
	ldrb	r3, [r7]	@ zero_extendqisi2
	cmp	r3, #51
	beq	.L2492
	ldr	fp, .L2531+12
	mov	r8, #0
.L2493:
	ldrb	r3, [r10]	@ zero_extendqisi2
	cmp	r3, r8
	bgt	.L2499
	ldr	fp, .L2531+12
	mov	r8, #0
.L2500:
	ldrb	r3, [r10]	@ zero_extendqisi2
	cmp	r3, r8
	bgt	.L2506
.L2492:
	ldr	fp, .L2531+12
	mov	r8, #0
.L2507:
	ldrb	r3, [r10]	@ zero_extendqisi2
	cmp	r3, r8
	bgt	.L2521
.L2522:
	movs	r3, #0
	mov	r0, r5
	str	r3, [r5, #176]
	bl	sandisk_A19_exit_readretry
.L2483:
	mov	r0, r6
	add	sp, sp, #8
	@ sp needed
	pop	{r4, r5, r6, r7, r8, r10, fp, pc}
.L2491:
	str	r7, [r5, #176]
	mov	r0, r5
	ldrh	r1, [r4, #8]
	bl	sandisk_A19_set_readretry
	mov	r6, r0
	cbz	r0, .L2486
	ldr	r0, .L2531+12
	bl	NAND_Print_DBG
.L2487:
	adds	r7, r7, #1
	b	.L2485
.L2486:
	mov	r0, r4
	bl	generic_read_page_start
	mov	r6, r0
	mov	r0, r4
	bl	generic_read_page_end_not_retry
	orrs	r6, r6, r0
	ldr	r0, [r4, #20]
	bl	is_nouse_page
	cmp	r0, #1
	bne	.L2488
	ldr	r3, [r5, #176]
	mvn	r6, #1
	ldr	r1, [r5, #260]
	ldr	r0, .L2531+16
	str	r3, [sp, #4]
	ldr	r3, [r4, #8]
	str	r3, [sp]
	ldr	r3, [r4, #4]
	ldr	r2, [r5, #20]
	ldr	r1, [r1, #8]
	bl	NAND_Print_DBG
	b	.L2487
.L2488:
	cmp	r6, #10
	beq	.L2519
	cmp	r6, #0
	bne	.L2487
.L2520:
	ldr	r3, [r5, #176]
	movs	r6, #10
	ldr	r1, [r5, #260]
	ldr	r0, .L2531+20
	str	r3, [sp, #4]
	ldr	r3, [r4, #8]
	str	r3, [sp]
	ldr	r3, [r4, #4]
	ldr	r2, [r5, #20]
	ldr	r1, [r1, #8]
	bl	NAND_Print_DBG
	b	.L2522
.L2499:
	str	r8, [r5, #176]
	mov	r0, r5
	ldrh	r1, [r4, #8]
	bl	sandisk_A19_set_readretry
	mov	r6, r0
	cbz	r0, .L2494
	mov	r0, fp
	bl	NAND_Print_DBG
.L2495:
	add	r8, r8, #1
	b	.L2493
.L2494:
	mov	r0, r4
	bl	sandisk_A19_dsp_on
	mov	r0, r4
	bl	generic_read_page_start
	mov	r6, r0
	mov	r0, r4
	bl	generic_read_page_end_not_retry
	orrs	r6, r6, r0
	ldr	r0, [r4, #20]
	bl	is_nouse_page
	cmp	r0, #1
	bne	.L2496
	ldr	r3, [r5, #176]
	mvn	r6, #1
	ldr	r1, [r5, #260]
	ldr	r0, .L2531+16
	str	r3, [sp, #4]
	ldr	r3, [r4, #8]
	str	r3, [sp]
	ldr	r3, [r4, #4]
	ldr	r2, [r5, #20]
	ldr	r1, [r1, #8]
	bl	NAND_Print_DBG
	b	.L2495
.L2496:
	cmp	r6, #10
	beq	.L2519
	cmp	r6, #0
	bne	.L2495
	b	.L2520
.L2506:
	str	r8, [r5, #176]
	mov	r0, r5
	ldrh	r1, [r4, #8]
	bl	sandisk_A19_set_readretry
	mov	r6, r0
	cbz	r0, .L2501
	mov	r0, fp
	bl	NAND_Print_DBG
.L2502:
	add	r8, r8, #1
	b	.L2500
.L2501:
	mov	r0, r4
	bl	sandisk_A19_cmd25
	mov	r0, r4
	bl	generic_read_page_start
	mov	r6, r0
	mov	r0, r4
	bl	generic_read_page_end_not_retry
	orrs	r6, r6, r0
	ldr	r0, [r4, #20]
	bl	is_nouse_page
	cmp	r0, #1
	bne	.L2503
	ldr	r3, [r5, #176]
	mvn	r6, #1
	ldr	r1, [r5, #260]
	ldr	r0, .L2531+16
	str	r3, [sp, #4]
	ldr	r3, [r4, #8]
	str	r3, [sp]
	ldr	r3, [r4, #4]
	ldr	r2, [r5, #20]
	ldr	r1, [r1, #8]
	bl	NAND_Print_DBG
	b	.L2502
.L2503:
	cmp	r6, #10
	beq	.L2519
	cmp	r6, #0
	bne	.L2502
	b	.L2520
.L2521:
	str	r8, [r5, #176]
	mov	r0, r5
	ldrh	r1, [r4, #8]
	bl	sandisk_A19_set_readretry
	mov	r6, r0
	cbz	r0, .L2508
	mov	r0, fp
	bl	NAND_Print_DBG
.L2509:
	add	r8, r8, #1
	b	.L2507
.L2508:
	ldrb	r3, [r7]	@ zero_extendqisi2
	cmp	r3, #52
	bne	.L2510
	movs	r1, #198
.L2529:
	mov	r0, r4
	bl	sandisk_A19_set_lmflgfix_next
	mov	r0, r4
	bl	sandisk_A19_cmd25
	ldrb	r3, [r7]	@ zero_extendqisi2
	cmp	r3, #50
	beq	.L2513
	subs	r3, r3, #52
	cmp	r3, #1
	bhi	.L2514
.L2513:
	mov	r0, r4
	bl	sandisk_A19_dsp_on
.L2514:
	mov	r0, r4
	bl	generic_read_page_start
	mov	r6, r0
	mov	r0, r4
	bl	generic_read_page_end_not_retry
	ldrb	r3, [r7]	@ zero_extendqisi2
	orrs	r6, r6, r0
	cmp	r3, #52
	bne	.L2515
	movs	r1, #194
.L2530:
	mov	r0, r4
	bl	sandisk_A19_set_lmflgfix_next
	ldr	r0, [r4, #20]
	bl	is_nouse_page
	cmp	r0, #1
	bne	.L2518
	ldr	r3, [r5, #176]
	mvn	r6, #1
	ldr	r1, [r5, #260]
	ldr	r0, .L2531+16
	str	r3, [sp, #4]
	ldr	r3, [r4, #8]
	str	r3, [sp]
	ldr	r3, [r4, #4]
	ldr	r2, [r5, #20]
	ldr	r1, [r1, #8]
	bl	NAND_Print_DBG
	b	.L2509
.L2510:
	cmp	r3, #53
	ite	eq
	moveq	r1, #26
	movne	r1, #192
	b	.L2529
.L2515:
	cmp	r3, #53
	ite	eq
	moveq	r1, #10
	movne	r1, #64
	b	.L2530
.L2518:
	cmp	r6, #10
	beq	.L2519
	cmp	r6, #0
	bne	.L2509
	b	.L2520
.L2519:
	ldr	r3, [r5, #176]
	ldr	r1, [r5, #260]
	ldr	r0, .L2531+24
	str	r3, [sp, #4]
	ldr	r3, [r4, #8]
	str	r3, [sp]
	ldr	r3, [r4, #4]
	ldr	r2, [r5, #20]
	ldr	r1, [r1, #8]
	bl	NAND_Print_DBG
	b	.L2522
.L2532:
	.align	2
.L2531:
	.word	g_nsi
	.word	.LC446
	.word	.LANCHOR50
	.word	.LC447
	.word	.LC448
	.word	.LC449
	.word	.LC450
	.word	.LANCHOR51
	.size	sandisk_A19_read_page_end, .-sandisk_A19_read_page_end
	.section	.text.sandisk_A19_special_init,"ax",%progbits
	.align	1
	.global	sandisk_A19_special_init
	.syntax unified
	.thumb
	.thumb_func
	.fpu softvfp
	.type	sandisk_A19_special_init, %function
sandisk_A19_special_init:
	@ args = 0, pretend = 0, frame = 0
	@ frame_needed = 0, uses_anonymous_args = 0
	push	{r3, r4, r5, lr}
	movs	r4, #0
	ldr	r3, .L2538
	ldr	r3, [r3]
	ldr	r5, [r3, #8]
.L2534:
	cbnz	r5, .L2535
	ldr	r3, .L2538+4
	ldrb	r1, [r3]	@ zero_extendqisi2
	ldr	r3, .L2538+8
	ldrb	r2, [r3]	@ zero_extendqisi2
	ldr	r3, .L2538+12
	ldrb	r3, [r3]	@ zero_extendqisi2
	cbnz	r4, .L2536
	ldr	r0, .L2538+16
	ldr	r5, .L2538+20
	str	r5, [r0]
	ldr	r0, .L2538+24
	bl	NAND_Print_DBG
.L2533:
	mov	r0, r4
	pop	{r3, r4, r5, pc}
.L2535:
	mov	r0, r5
	bl	sandisk_A19_readretry_init
	ldr	r5, [r5]
	orrs	r4, r4, r0
	b	.L2534
.L2536:
	ldr	r0, .L2538+28
	bl	NAND_Print
	b	.L2533
.L2539:
	.align	2
.L2538:
	.word	g_nsi
	.word	.LANCHOR50
	.word	.LANCHOR51
	.word	.LANCHOR52
	.word	df_read_page_end
	.word	sandisk_A19_read_page_end
	.word	.LC451
	.word	.LC452
	.size	sandisk_A19_special_init, .-sandisk_A19_special_init
	.section	.text.sandisk_A19_special_exit,"ax",%progbits
	.align	1
	.global	sandisk_A19_special_exit
	.syntax unified
	.thumb
	.thumb_func
	.fpu softvfp
	.type	sandisk_A19_special_exit, %function
sandisk_A19_special_exit:
	@ args = 0, pretend = 0, frame = 0
	@ frame_needed = 0, uses_anonymous_args = 0
	ldr	r3, .L2551
	push	{r4, r5, r6, lr}
	movs	r6, #0
	ldr	r3, [r3]
	ldr	r4, [r3, #8]
.L2541:
	cbnz	r4, .L2544
	ldr	r0, .L2551+4
	mov	r5, r4
	bl	NAND_Print_DBG
	b	.L2540
.L2544:
	ldrb	r3, [r4, #254]	@ zero_extendqisi2
	cbz	r3, .L2542
	str	r6, [r4, #196]
	mov	r0, r4
	bl	sandisk_toggle_vendor_specific_setting
	mov	r5, r0
	cbz	r0, .L2542
	ldr	r1, .L2551+8
	ldr	r0, .L2551+12
	bl	NAND_Print
.L2540:
	mov	r0, r5
	pop	{r4, r5, r6, pc}
.L2542:
	ldr	r4, [r4]
	b	.L2541
.L2552:
	.align	2
.L2551:
	.word	g_nsi
	.word	.LC454
	.word	.LANCHOR60
	.word	.LC453
	.size	sandisk_A19_special_exit, .-sandisk_A19_special_exit
	.section	.text.sandisk_A19_check_bad_block_first_burn,"ax",%progbits
	.align	1
	.global	sandisk_A19_check_bad_block_first_burn
	.syntax unified
	.thumb
	.thumb_func
	.fpu softvfp
	.type	sandisk_A19_check_bad_block_first_burn, %function
sandisk_A19_check_bad_block_first_burn:
	@ args = 0, pretend = 0, frame = 0
	@ frame_needed = 0, uses_anonymous_args = 0
	ldr	r3, .L2557
	push	{r0, r1, r2, r4, r5, r6, r7, r8, r10, lr}
	mov	r8, r0
	ldr	r1, [r0]
	ldr	r0, [r3]
	bl	nci_get_from_nsi
	ldr	r4, [r0, #260]
	mov	r5, r0
	ldr	r2, [r0, #20]
	ldr	r1, [r8, #4]
	ldr	r3, [r4, #12]
	add	r10, r4, #32
	ldr	r0, [r0, #24]
	cmp	r2, r3
	bcs	.L2554
	cmp	r1, r0
	bcc	.L2555
.L2554:
	strd	r1, r0, [sp]
	ldr	r1, [r4, #8]
	mvn	r4, #9
	ldr	r0, .L2557+4
	bl	NAND_Print
.L2553:
	mov	r0, r4
	add	sp, sp, #12
	@ sp needed
	pop	{r4, r5, r6, r7, r8, r10, pc}
.L2555:
	mov	r0, r4
	movs	r6, #1
	bl	ndfc_disable_randomize
	mov	r0, r5
	movs	r7, #0
	bl	nand_read_chip_status_ready
	mov	r0, r5
	bl	nand_enable_chip
	mov	r0, r10
	bl	ndfc_clean_cmd_seq
	movs	r3, #162
	ldrd	r1, r2, [r8, #4]
	strd	r6, r3, [r4, #40]
	movs	r3, #128
	strd	r6, r3, [r4, #104]
	str	r7, [r4, #32]
	strd	r6, r7, [r4, #48]
	strd	r6, r7, [r4, #112]
	ldr	r0, [r5, #36]
	bl	get_row_addr
	movs	r3, #5
	mov	r2, r0
	str	r3, [r4, #136]
	add	r3, r4, #120
	str	r3, [sp]
	movs	r1, #2
	movs	r3, #3
	mov	r0, r7
	bl	fill_cmd_addr
	movs	r3, #16
	strd	r6, r6, [r4, #176]
	strd	r6, r3, [r4, #168]
	mov	r1, r10
	ldr	r0, [r5, #260]
	bl	ndfc_execute_cmd
	mov	r0, r5
	bl	nand_read_chip_status_ready
	mov	r4, r0
	mov	r0, r5
	bl	nand_disable_chip
	b	.L2553
.L2558:
	.align	2
.L2557:
	.word	g_nsi
	.word	.LC397
	.size	sandisk_A19_check_bad_block_first_burn, .-sandisk_A19_check_bad_block_first_burn
	.section	.text.generic_special_init,"ax",%progbits
	.align	1
	.global	generic_special_init
	.syntax unified
	.thumb
	.thumb_func
	.fpu softvfp
	.type	generic_special_init, %function
generic_special_init:
	@ args = 0, pretend = 0, frame = 0
	@ frame_needed = 0, uses_anonymous_args = 0
	push	{r3, lr}
	ldr	r2, .L2560
	ldr	r3, .L2560+4
	ldr	r0, .L2560+8
	str	r2, [r3]
	bl	NAND_Print_DBG
	movs	r0, #0
	pop	{r3, pc}
.L2561:
	.align	2
.L2560:
	.word	generic_read_page_end_not_retry
	.word	df_read_page_end
	.word	.LC455
	.size	generic_special_init, .-generic_special_init
	.section	.text.generic_special_exit,"ax",%progbits
	.align	1
	.global	generic_special_exit
	.syntax unified
	.thumb
	.thumb_func
	.fpu softvfp
	.type	generic_special_exit, %function
generic_special_exit:
	@ args = 0, pretend = 0, frame = 0
	@ frame_needed = 0, uses_anonymous_args = 0
	push	{r3, lr}
	ldr	r0, .L2563
	bl	NAND_Print_DBG
	movs	r0, #0
	pop	{r3, pc}
.L2564:
	.align	2
.L2563:
	.word	.LC456
	.size	generic_special_exit, .-generic_special_exit
	.section	.text.toshiba_read_retry_clock_save,"ax",%progbits
	.align	1
	.global	toshiba_read_retry_clock_save
	.syntax unified
	.thumb
	.thumb_func
	.fpu softvfp
	.type	toshiba_read_retry_clock_save, %function
toshiba_read_retry_clock_save:
	@ args = 0, pretend = 0, frame = 0
	@ frame_needed = 0, uses_anonymous_args = 0
	push	{r4, lr}
	mov	r4, r0
	ldr	r3, .L2566
	ldr	r0, [r0, #260]
	ldr	r3, [r3]
	ldr	r2, .L2566+4
	ldr	r1, .L2566+8
	ldr	r0, [r0, #8]
	blx	r3
	ldr	r0, [r4, #260]
	movs	r2, #20
	ldr	r3, .L2566+12
	movs	r1, #10
	ldr	r0, [r0, #8]
	ldr	r3, [r3]
	blx	r3
	movs	r0, #0
	pop	{r4, pc}
.L2567:
	.align	2
.L2566:
	.word	NAND_GetClk
	.word	.LANCHOR61
	.word	.LANCHOR62
	.word	NAND_SetClk
	.size	toshiba_read_retry_clock_save, .-toshiba_read_retry_clock_save
	.section	.text.toshiba_read_retry_clock_recover,"ax",%progbits
	.align	1
	.global	toshiba_read_retry_clock_recover
	.syntax unified
	.thumb
	.thumb_func
	.fpu softvfp
	.type	toshiba_read_retry_clock_recover, %function
toshiba_read_retry_clock_recover:
	@ args = 0, pretend = 0, frame = 0
	@ frame_needed = 0, uses_anonymous_args = 0
	push	{r3, lr}
	ldr	r0, [r0, #260]
	ldr	r3, .L2569
	ldr	r2, .L2569+4
	ldr	r1, .L2569+8
	ldr	r3, [r3]
	ldr	r2, [r2]
	ldr	r1, [r1]
	ldr	r0, [r0, #8]
	blx	r3
	movs	r0, #0
	pop	{r3, pc}
.L2570:
	.align	2
.L2569:
	.word	NAND_SetClk
	.word	.LANCHOR61
	.word	.LANCHOR62
	.size	toshiba_read_retry_clock_recover, .-toshiba_read_retry_clock_recover
	.section	.text.toshiba_vender_set_param,"ax",%progbits
	.align	1
	.global	toshiba_vender_set_param
	.syntax unified
	.thumb
	.thumb_func
	.fpu softvfp
	.type	toshiba_vender_set_param, %function
toshiba_vender_set_param:
	@ args = 0, pretend = 0, frame = 0
	@ frame_needed = 0, uses_anonymous_args = 0
	push	{r0, r1, r2, r3, r4, r5, r6, r7, r8, r10, fp, lr}
	mov	r10, r3
	ldr	r7, .L2580
	mov	r6, r0
	mov	r4, r1
	mov	r8, r2
	ldrb	r3, [r7]	@ zero_extendqisi2
	cmp	r3, #18
	bne	.L2578
	mov	fp, #1
	mov	r3, r2
	strd	r1, fp, [sp]
	movs	r2, #0
	str	r10, [sp, #8]
	ldr	r1, .L2580+4
	bl	set_cmd_with_nand_bus
	ldrb	r3, [r4]	@ zero_extendqisi2
	mov	r5, r0
	cbnz	r3, .L2572
	ldrb	r3, [r4, #1]	@ zero_extendqisi2
	cbnz	r3, .L2572
	ldrb	r3, [r4, #2]	@ zero_extendqisi2
	cbnz	r3, .L2572
	ldrb	r3, [r4, #3]	@ zero_extendqisi2
	cbnz	r3, .L2572
	ldr	r3, .L2580+8
	ldr	r3, [r3]
	cbnz	r3, .L2572
.L2579:
	ldr	r3, .L2580+12
	movs	r2, #1
	mov	r0, r6
	ldrb	r1, [r3]	@ zero_extendqisi2
	bl	set_one_cmd
	orrs	r5, r5, r0
.L2571:
	mov	r0, r5
	add	sp, sp, #16
	@ sp needed
	pop	{r4, r5, r6, r7, r8, r10, fp, pc}
.L2578:
	movs	r5, #0
.L2572:
	ldrb	r3, [r7]	@ zero_extendqisi2
	cmp	r3, #17
	bne	.L2574
	mov	fp, #1
	mov	r3, r8
	str	r10, [sp, #8]
	movs	r2, #0
	strd	r4, fp, [sp]
	mov	r0, r6
	ldr	r1, .L2580+4
	bl	set_cmd_with_nand_bus
	ldrb	r3, [r4]	@ zero_extendqisi2
	orrs	r5, r5, r0
	cmp	r3, #0
	bne	.L2575
	ldrb	r3, [r4, #1]	@ zero_extendqisi2
	cbnz	r3, .L2574
	ldrb	r3, [r4, #2]	@ zero_extendqisi2
	cbnz	r3, .L2574
	ldrb	r3, [r4, #3]	@ zero_extendqisi2
	cmp	r3, #0
	beq	.L2579
.L2574:
	ldrb	r3, [r7]	@ zero_extendqisi2
	cmp	r3, #16
	bne	.L2576
	ldrb	r3, [r4]	@ zero_extendqisi2
	cmp	r3, #255
	bne	.L2577
	ldrb	r3, [r4, #1]	@ zero_extendqisi2
	cmp	r3, #255
	bne	.L2577
	ldrb	r3, [r4, #2]	@ zero_extendqisi2
	cmp	r3, #255
	bne	.L2577
	ldrb	r3, [r4, #3]	@ zero_extendqisi2
	cmp	r3, #255
	beq	.L2579
.L2577:
	movs	r3, #1
	str	r10, [sp, #8]
	strd	r4, r3, [sp]
	movs	r2, #0
	mov	r3, r8
	ldr	r1, .L2580+16
	mov	r0, r6
	bl	set_cmd_with_nand_bus
	orrs	r5, r5, r0
.L2576:
	ldr	r7, .L2580+20
	movs	r2, #0
	mov	r0, r6
	ldrb	r1, [r7]	@ zero_extendqisi2
	bl	set_one_cmd
	ldrb	r1, [r7, #1]	@ zero_extendqisi2
	orrs	r5, r5, r0
	movs	r2, #0
	mov	r0, r6
	bl	set_one_cmd
	orrs	r5, r5, r0
	ldrb	r0, [r4, #3]	@ zero_extendqisi2
	ldrb	r3, [r4, #2]	@ zero_extendqisi2
	ldrb	r2, [r4, #1]	@ zero_extendqisi2
	str	r0, [sp]
	ldrb	r1, [r4]	@ zero_extendqisi2
	ldr	r0, .L2580+24
	bl	NAND_Print_DBG
	b	.L2571
.L2575:
	cmp	r3, #2
	bne	.L2574
	ldrb	r2, [r4, #1]	@ zero_extendqisi2
	cmp	r2, #0
	bne	.L2574
	ldrb	r3, [r4, #2]	@ zero_extendqisi2
	cmp	r3, #126
	bne	.L2574
	ldrb	r3, [r4, #3]	@ zero_extendqisi2
	cmp	r3, #124
	bne	.L2574
	ldr	r3, .L2580+28
	mov	r0, r6
	ldrb	r1, [r3]	@ zero_extendqisi2
	bl	set_one_cmd
	orrs	r5, r5, r0
	b	.L2574
.L2581:
	.align	2
.L2580:
	.word	.LANCHOR63
	.word	.LANCHOR64
	.word	.LANCHOR65
	.word	.LANCHOR66
	.word	.LANCHOR68
	.word	.LANCHOR69
	.word	.LC457
	.word	.LANCHOR67
	.size	toshiba_vender_set_param, .-toshiba_vender_set_param
	.section	.text.toshiba_vender_pre_condition,"ax",%progbits
	.align	1
	.global	toshiba_vender_pre_condition
	.syntax unified
	.thumb
	.thumb_func
	.fpu softvfp
	.type	toshiba_vender_pre_condition, %function
toshiba_vender_pre_condition:
	@ args = 0, pretend = 0, frame = 0
	@ frame_needed = 0, uses_anonymous_args = 0
	push	{r0, r1, r2, r3, r4, lr}
	movs	r3, #0
	movs	r2, #2
	str	r3, [sp]
	strd	r3, r2, [sp, #4]
	mov	r2, r3
	ldr	r1, .L2587
	bl	set_cmd_with_nand_bus
	mov	r4, r0
	cbz	r0, .L2582
	ldr	r0, .L2587+4
	bl	NAND_Print
.L2582:
	mov	r0, r4
	add	sp, sp, #16
	@ sp needed
	pop	{r4, pc}
.L2588:
	.align	2
.L2587:
	.word	.LANCHOR70
	.word	.LC458
	.size	toshiba_vender_pre_condition, .-toshiba_vender_pre_condition
	.section	.text.toshiba_set_readretry,"ax",%progbits
	.align	1
	.global	toshiba_set_readretry
	.syntax unified
	.thumb
	.thumb_func
	.fpu softvfp
	.type	toshiba_set_readretry, %function
toshiba_set_readretry:
	@ args = 0, pretend = 0, frame = 0
	@ frame_needed = 0, uses_anonymous_args = 0
	push	{r4, r5, r6, lr}
	mov	r4, r0
	ldr	r6, .L2602
	ldrb	r2, [r6]	@ zero_extendqisi2
	subs	r2, r2, #16
	cmp	r2, #2
	bhi	.L2597
	bl	toshiba_read_retry_clock_save
	mov	r0, r4
	bl	nand_enable_chip
	ldr	r0, [r4, #260]
	bl	ndfc_disable_randomize
	ldr	r3, [r4, #176]
	cbnz	r3, .L2591
	mov	r0, r4
	bl	toshiba_vender_pre_condition
.L2591:
	ldr	r0, [r4, #260]
	bl	ndfc_is_toggle_interface
	ldr	r5, .L2602+4
	cbz	r0, .L2592
	ldr	r0, [r4, #260]
	bl	ndfc_set_legacy_interface
	movs	r3, #1
	str	r3, [r5]
.L2592:
	ldrb	r2, [r6]	@ zero_extendqisi2
	ldr	r1, [r4, #176]
	cmp	r2, #17
	lsl	r3, r1, #2
	bne	.L2593
	add	r3, r3, r1
	ldr	r1, .L2602+8
.L2601:
	ldr	r2, .L2602+12
	add	r1, r1, r3
	movs	r3, #5
.L2594:
	mov	r0, r4
	bl	toshiba_vender_set_param
	ldr	r3, [r5]
	mov	r6, r0
	cmp	r3, #1
	bne	.L2596
	ldr	r0, [r4, #260]
	bl	ndfc_set_toggle_interface
.L2596:
	mov	r0, r4
	bl	nand_disable_chip
	mov	r0, r4
	bl	toshiba_read_retry_clock_recover
.L2589:
	mov	r0, r6
	pop	{r4, r5, r6, pc}
.L2593:
	cmp	r2, #18
	bne	.L2595
	add	r3, r3, r1
	ldr	r1, .L2602+16
	b	.L2601
.L2595:
	ldr	r1, .L2602+20
	ldr	r2, .L2602+24
	add	r1, r1, r3
	movs	r3, #4
	b	.L2594
.L2597:
	mvn	r6, #88
	b	.L2589
.L2603:
	.align	2
.L2602:
	.word	.LANCHOR63
	.word	.LANCHOR73
	.word	.LANCHOR74
	.word	.LANCHOR71
	.word	.LANCHOR75
	.word	.LANCHOR76
	.word	.LANCHOR72
	.size	toshiba_set_readretry, .-toshiba_set_readretry
	.section	.text.toshiba_readretry_init,"ax",%progbits
	.align	1
	.global	toshiba_readretry_init
	.syntax unified
	.thumb
	.thumb_func
	.fpu softvfp
	.type	toshiba_readretry_init, %function
toshiba_readretry_init:
	@ args = 0, pretend = 0, frame = 0
	@ frame_needed = 0, uses_anonymous_args = 0
	ldr	r3, [r0, #264]
	ldr	r2, .L2605
	push	{r4, lr}
	movs	r4, #0
	ldr	r3, [r3, #52]
	str	r4, [r0, #176]
	ldr	r0, .L2605+4
	lsrs	r1, r3, #16
	strb	r1, [r2]
	lsrs	r1, r3, #8
	ldr	r2, .L2605+8
	strb	r1, [r2]
	uxtb	r1, r1
	ldr	r2, .L2605+12
	strb	r3, [r2]
	bl	NAND_Print_DBG
	mov	r0, r4
	pop	{r4, pc}
.L2606:
	.align	2
.L2605:
	.word	.LANCHOR63
	.word	.LC459
	.word	.LANCHOR77
	.word	.LANCHOR78
	.size	toshiba_readretry_init, .-toshiba_readretry_init
	.section	.text.toshiba_readretry_exit,"ax",%progbits
	.align	1
	.global	toshiba_readretry_exit
	.syntax unified
	.thumb
	.thumb_func
	.fpu softvfp
	.type	toshiba_readretry_exit, %function
toshiba_readretry_exit:
	@ args = 0, pretend = 0, frame = 0
	@ frame_needed = 0, uses_anonymous_args = 0
	@ link register save eliminated.
	movs	r0, #0
	bx	lr
	.size	toshiba_readretry_exit, .-toshiba_readretry_exit
	.section	.text.toshiba_exit_readretry,"ax",%progbits
	.align	1
	.global	toshiba_exit_readretry
	.syntax unified
	.thumb
	.thumb_func
	.fpu softvfp
	.type	toshiba_exit_readretry, %function
toshiba_exit_readretry:
	@ args = 0, pretend = 0, frame = 0
	@ frame_needed = 0, uses_anonymous_args = 0
	push	{r4, r5, r6, lr}
	mov	r4, r0
	ldr	r5, .L2619
	ldrb	r2, [r5]	@ zero_extendqisi2
	subs	r2, r2, #16
	cmp	r2, #2
	bhi	.L2613
	bl	nand_enable_chip
	ldr	r0, [r4, #260]
	bl	ndfc_disable_randomize
	ldr	r0, [r4, #260]
	bl	ndfc_is_toggle_interface
	ldr	r6, .L2619+4
	cbz	r0, .L2610
	ldr	r0, [r4, #260]
	bl	ndfc_set_legacy_interface
	movs	r3, #1
	str	r3, [r6]
.L2610:
	ldrb	r3, [r5]	@ zero_extendqisi2
	cmp	r3, #17
	beq	.L2614
	ldr	r1, .L2619+8
	cmp	r3, #18
	ldr	r0, .L2619+12
	it	eq
	moveq	r3, #5
	ldr	r2, .L2619+16
	itt	ne
	movne	r3, #0
	movne	r2, r1
	ldr	r1, .L2619+20
	it	ne
	movne	r1, r0
.L2611:
	mov	r0, r4
	bl	toshiba_vender_set_param
	ldr	r3, [r6]
	mov	r5, r0
	cmp	r3, #1
	bne	.L2612
	ldr	r0, [r4, #260]
	bl	ndfc_set_toggle_interface
.L2612:
	mov	r0, r4
	bl	nand_disable_chip
.L2608:
	mov	r0, r5
	pop	{r4, r5, r6, pc}
.L2614:
	movs	r3, #5
	ldr	r2, .L2619+16
	ldr	r1, .L2619+24
	b	.L2611
.L2613:
	mvn	r5, #87
	b	.L2608
.L2620:
	.align	2
.L2619:
	.word	.LANCHOR63
	.word	.LANCHOR73
	.word	.LANCHOR72
	.word	.LANCHOR76+24
	.word	.LANCHOR71
	.word	.LANCHOR75+50
	.word	.LANCHOR74+35
	.size	toshiba_exit_readretry, .-toshiba_exit_readretry
	.section	.text.toshiba_read_page_end,"ax",%progbits
	.align	1
	.global	toshiba_read_page_end
	.syntax unified
	.thumb
	.thumb_func
	.fpu softvfp
	.type	toshiba_read_page_end, %function
toshiba_read_page_end:
	@ args = 0, pretend = 0, frame = 0
	@ frame_needed = 0, uses_anonymous_args = 0
	ldr	r3, .L2631
	push	{r0, r1, r2, r4, r5, r6, r7, r8, r10, lr}
	mov	r6, r0
	ldr	r1, [r0]
	ldr	r0, [r3]
	bl	nci_get_from_nsi
	mov	r5, r0
	mov	r0, r6
	bl	generic_read_page_end_not_retry
	adds	r3, r0, #2
	mov	r4, r0
	bne	.L2621
	ldr	r0, .L2631+4
	movs	r7, #0
	bl	NAND_Print_DBG
	ldr	r3, .L2631+8
	movs	r2, #1
	ldr	r10, .L2631+16
	mov	r8, r3
	str	r2, [r3]
.L2623:
	ldrb	r3, [r10]	@ zero_extendqisi2
	cmp	r3, r7
	ble	.L2626
	str	r7, [r5, #176]
	mov	r0, r5
	bl	toshiba_set_readretry
	mov	r4, r0
	cbnz	r0, .L2624
	mov	r0, r6
	bl	generic_read_page_start
	mov	r4, r0
	mov	r0, r6
	bl	generic_read_page_end_not_retry
	orrs	r4, r4, r0
	cmp	r4, #10
	beq	.L2625
	cbnz	r4, .L2624
.L2625:
	ldr	r3, [r5, #176]
	movs	r4, #10
	ldr	r1, [r5, #260]
	ldr	r0, .L2631+12
	str	r3, [sp, #4]
	ldr	r3, [r6, #8]
	str	r3, [sp]
	ldr	r3, [r6, #4]
	ldr	r2, [r5, #20]
	ldr	r1, [r1, #8]
	bl	NAND_Print_DBG
.L2626:
	movs	r3, #0
	mov	r0, r5
	str	r3, [r5, #176]
	str	r3, [r8]
	bl	toshiba_exit_readretry
.L2621:
	mov	r0, r4
	add	sp, sp, #12
	@ sp needed
	pop	{r4, r5, r6, r7, r8, r10, pc}
.L2624:
	adds	r7, r7, #1
	b	.L2623
.L2632:
	.align	2
.L2631:
	.word	g_nsi
	.word	.LC460
	.word	.LANCHOR65
	.word	.LC461
	.word	.LANCHOR77
	.size	toshiba_read_page_end, .-toshiba_read_page_end
	.section	.text.toshiba_special_init,"ax",%progbits
	.align	1
	.global	toshiba_special_init
	.syntax unified
	.thumb
	.thumb_func
	.fpu softvfp
	.type	toshiba_special_init, %function
toshiba_special_init:
	@ args = 0, pretend = 0, frame = 0
	@ frame_needed = 0, uses_anonymous_args = 0
	push	{r3, r4, r5, lr}
	movs	r4, #0
	ldr	r3, .L2638
	ldr	r3, [r3]
	ldr	r5, [r3, #8]
.L2634:
	cbnz	r5, .L2635
	ldr	r3, .L2638+4
	ldrb	r1, [r3]	@ zero_extendqisi2
	ldr	r3, .L2638+8
	ldrb	r2, [r3]	@ zero_extendqisi2
	ldr	r3, .L2638+12
	ldrb	r3, [r3]	@ zero_extendqisi2
	cbnz	r4, .L2636
	ldr	r0, .L2638+16
	ldr	r5, .L2638+20
	str	r5, [r0]
	ldr	r0, .L2638+24
	bl	NAND_Print_DBG
.L2633:
	mov	r0, r4
	pop	{r3, r4, r5, pc}
.L2635:
	mov	r0, r5
	bl	toshiba_readretry_init
	ldr	r5, [r5]
	orrs	r4, r4, r0
	b	.L2634
.L2636:
	ldr	r0, .L2638+28
	bl	NAND_Print
	b	.L2633
.L2639:
	.align	2
.L2638:
	.word	g_nsi
	.word	.LANCHOR63
	.word	.LANCHOR77
	.word	.LANCHOR78
	.word	df_read_page_end
	.word	toshiba_read_page_end
	.word	.LC462
	.word	.LC463
	.size	toshiba_special_init, .-toshiba_special_init
	.section	.text.toshiba_special_exit,"ax",%progbits
	.align	1
	.global	toshiba_special_exit
	.syntax unified
	.thumb
	.thumb_func
	.fpu softvfp
	.type	toshiba_special_exit, %function
toshiba_special_exit:
	@ args = 0, pretend = 0, frame = 0
	@ frame_needed = 0, uses_anonymous_args = 0
	ldr	r3, .L2643
	push	{r4, lr}
	ldr	r3, [r3]
	ldr	r4, [r3, #8]
.L2641:
	cbnz	r4, .L2642
	ldr	r0, .L2643+4
	bl	NAND_Print_DBG
	mov	r0, r4
	pop	{r4, pc}
.L2642:
	ldr	r4, [r4]
	b	.L2641
.L2644:
	.align	2
.L2643:
	.word	g_nsi
	.word	.LC464
	.size	toshiba_special_exit, .-toshiba_special_exit
	.section	.text.micron_intel_set_feature,"ax",%progbits
	.align	1
	.global	micron_intel_set_feature
	.syntax unified
	.thumb
	.thumb_func
	.fpu softvfp
	.type	micron_intel_set_feature, %function
micron_intel_set_feature:
	@ args = 0, pretend = 0, frame = 16
	@ frame_needed = 0, uses_anonymous_args = 0
	push	{r4, r5, r6, r7, lr}
	sub	sp, sp, #20
	movs	r4, #0
	mov	r7, r0
	mov	r6, r2
	mov	r5, r1
	strb	r1, [sp, #7]
	add	r1, sp, #7
	strb	r2, [sp, #8]
	add	r2, sp, #8
	strb	r4, [sp, #9]
	strb	r4, [sp, #10]
	strb	r4, [sp, #11]
	bl	nand_set_feature
	add	r2, sp, #12
	add	r1, sp, #7
	mov	r0, r7
	bl	nand_get_feature
	ldrb	r2, [sp, #12]	@ zero_extendqisi2
	ldrb	r3, [sp, #8]	@ zero_extendqisi2
	cmp	r2, r3
	beq	.L2647
	mov	r2, r6
	mov	r1, r5
	ldr	r0, .L2648
	bl	NAND_Print
	mvn	r0, #71
.L2645:
	add	sp, sp, #20
	@ sp needed
	pop	{r4, r5, r6, r7, pc}
.L2647:
	mov	r0, r4
	b	.L2645
.L2649:
	.align	2
.L2648:
	.word	.LC465
	.size	micron_intel_set_feature, .-micron_intel_set_feature
	.section	.text.micron_set_readretry,"ax",%progbits
	.align	1
	.global	micron_set_readretry
	.syntax unified
	.thumb
	.thumb_func
	.fpu softvfp
	.type	micron_set_readretry, %function
micron_set_readretry:
	@ args = 0, pretend = 0, frame = 0
	@ frame_needed = 0, uses_anonymous_args = 0
	push	{r3, r4, r5, r6, r7, lr}
	mov	r4, r0
	bl	nand_enable_chip
	ldr	r0, [r4, #260]
	bl	ndfc_disable_randomize
	ldr	r3, .L2661
	ldrb	r3, [r3]	@ zero_extendqisi2
	sub	r2, r3, #64
	cmp	r2, #2
	bls	.L2651
	cmp	r3, #80
	bne	.L2659
.L2651:
	ldrb	r6, [r4, #176]	@ zero_extendqisi2
	ldr	r7, .L2661+4
	cmp	r6, #255
	it	eq
	moveq	r6, #0
	cmp	r3, #80
	bne	.L2654
	ldr	r3, [r7]
	ldrb	r3, [r3, r6]	@ zero_extendqisi2
	cbz	r3, .L2655
.L2657:
	ldr	r3, [r4, #176]
	cmp	r3, #255
	bne	.L2654
	movs	r2, #0
	movs	r1, #137
	mov	r0, r4
	bl	micron_intel_set_feature
	movs	r2, #0
	mov	r5, r0
	movs	r1, #147
	mov	r0, r4
	bl	micron_intel_set_feature
	orrs	r5, r5, r0
	b	.L2652
.L2655:
	movs	r2, #1
	movs	r1, #147
	mov	r0, r4
	bl	micron_intel_set_feature
	mov	r5, r0
	cmp	r0, #0
	beq	.L2657
.L2652:
	ldr	r3, [r4, #176]
	mov	r0, r4
	cmp	r3, #255
	itt	eq
	moveq	r3, #0
	streq	r3, [r4, #176]
	bl	nand_disable_chip
	mov	r0, r5
	pop	{r3, r4, r5, r6, r7, pc}
.L2654:
	ldr	r3, [r7]
	movs	r1, #137
	mov	r0, r4
	ldrb	r2, [r3, r6]	@ zero_extendqisi2
	bl	micron_intel_set_feature
	mov	r5, r0
	b	.L2652
.L2659:
	movs	r5, #0
	b	.L2652
.L2662:
	.align	2
.L2661:
	.word	.LANCHOR79
	.word	.LANCHOR80
	.size	micron_set_readretry, .-micron_set_readretry
	.section	.text.micron_read_page_end,"ax",%progbits
	.align	1
	.global	micron_read_page_end
	.syntax unified
	.thumb
	.thumb_func
	.fpu softvfp
	.type	micron_read_page_end, %function
micron_read_page_end:
	@ args = 0, pretend = 0, frame = 0
	@ frame_needed = 0, uses_anonymous_args = 0
	ldr	r3, .L2673
	push	{r0, r1, r4, r5, r6, r7, r8, lr}
	mov	r6, r0
	ldr	r1, [r0]
	ldr	r0, [r3]
	bl	nci_get_from_nsi
	mov	r5, r0
	mov	r0, r6
	bl	generic_read_page_end_not_retry
	adds	r3, r0, #2
	mov	r4, r0
	bne	.L2663
	ldr	r8, .L2673+12
	movs	r7, #0
	ldr	r0, .L2673+4
	bl	NAND_Print_DBG
.L2665:
	ldrb	r3, [r8]	@ zero_extendqisi2
	cmp	r3, r7
	ble	.L2668
	str	r7, [r5, #176]
	mov	r0, r5
	bl	micron_set_readretry
	mov	r4, r0
	cbnz	r0, .L2666
	mov	r0, r6
	bl	generic_read_page_start
	mov	r4, r0
	mov	r0, r6
	bl	generic_read_page_end_not_retry
	orrs	r4, r4, r0
	cmp	r4, #10
	beq	.L2667
	cbnz	r4, .L2666
.L2667:
	ldr	r3, [r5, #176]
	movs	r4, #10
	ldr	r1, [r5, #260]
	ldr	r0, .L2673+8
	str	r3, [sp, #4]
	ldr	r3, [r6, #8]
	str	r3, [sp]
	ldr	r3, [r6, #4]
	ldr	r2, [r5, #20]
	ldr	r1, [r1, #8]
	bl	NAND_Print_DBG
.L2668:
	movs	r3, #255
	mov	r0, r5
	str	r3, [r5, #176]
	bl	micron_set_readretry
.L2663:
	mov	r0, r4
	add	sp, sp, #8
	@ sp needed
	pop	{r4, r5, r6, r7, r8, pc}
.L2666:
	adds	r7, r7, #1
	b	.L2665
.L2674:
	.align	2
.L2673:
	.word	g_nsi
	.word	.LC466
	.word	.LC467
	.word	.LANCHOR81
	.size	micron_read_page_end, .-micron_read_page_end
	.section	.text.micron_readretry_init,"ax",%progbits
	.align	1
	.global	micron_readretry_init
	.syntax unified
	.thumb
	.thumb_func
	.fpu softvfp
	.type	micron_readretry_init, %function
micron_readretry_init:
	@ args = 0, pretend = 0, frame = 0
	@ frame_needed = 0, uses_anonymous_args = 0
	push	{r3, r4, r5, r6, r7, r8, r10, lr}
	movs	r7, #0
	ldr	r3, [r0, #264]
	movs	r1, #137
	mov	r6, r0
	ldr	r10, .L2683+28
	ldr	r3, [r3, #52]
	ldr	r5, .L2683
	ldr	r8, .L2683+32
	ldr	r4, .L2683+4
	lsrs	r2, r3, #16
	lsrs	r3, r3, #8
	str	r7, [r0, #176]
	strb	r2, [r10]
	mov	r2, r7
	strb	r3, [r4]
	str	r8, [r5]
	bl	micron_intel_set_feature
	ldrb	r3, [r10]	@ zero_extendqisi2
	cmp	r3, #65
	bne	.L2676
	movs	r3, #12
	str	r8, [r5]
	strb	r3, [r4]
.L2677:
	ldrb	r1, [r4]	@ zero_extendqisi2
	ldr	r0, .L2683+8
	bl	NAND_Print_DBG
	movs	r0, #0
	b	.L2675
.L2676:
	cmp	r3, #64
	bne	.L2678
	ldr	r3, .L2683+12
	str	r3, [r5]
	movs	r3, #10
.L2682:
	strb	r3, [r4]
	b	.L2677
.L2678:
	cmp	r3, #80
	bne	.L2679
	ldr	r3, .L2683+16
	mov	r2, r7
	movs	r1, #147
	mov	r0, r6
	str	r3, [r5]
	movs	r3, #7
	strb	r3, [r4]
	bl	micron_intel_set_feature
	b	.L2677
.L2679:
	cmp	r3, #66
	bne	.L2680
	ldr	r3, .L2683+20
	str	r3, [r5]
	movs	r3, #18
	b	.L2682
.L2680:
	ldr	r0, .L2683+24
	bl	NAND_Print
	mvn	r0, #70
.L2675:
	pop	{r3, r4, r5, r6, r7, r8, r10, pc}
.L2684:
	.align	2
.L2683:
	.word	.LANCHOR80
	.word	.LANCHOR81
	.word	.LC469
	.word	.LANCHOR83
	.word	.LANCHOR84
	.word	.LANCHOR85
	.word	.LC468
	.word	.LANCHOR79
	.word	.LANCHOR82
	.size	micron_readretry_init, .-micron_readretry_init
	.section	.text.micron_readretry_exit,"ax",%progbits
	.align	1
	.global	micron_readretry_exit
	.syntax unified
	.thumb
	.thumb_func
	.fpu softvfp
	.type	micron_readretry_exit, %function
micron_readretry_exit:
	@ args = 0, pretend = 0, frame = 0
	@ frame_needed = 0, uses_anonymous_args = 0
	@ link register save eliminated.
	ldr	r3, .L2686
	movs	r0, #0
	strb	r0, [r3]
	ldr	r3, .L2686+4
	strb	r0, [r3]
	bx	lr
.L2687:
	.align	2
.L2686:
	.word	.LANCHOR79
	.word	.LANCHOR81
	.size	micron_readretry_exit, .-micron_readretry_exit
	.section	.text.micron_special_init,"ax",%progbits
	.align	1
	.global	micron_special_init
	.syntax unified
	.thumb
	.thumb_func
	.fpu softvfp
	.type	micron_special_init, %function
micron_special_init:
	@ args = 0, pretend = 0, frame = 0
	@ frame_needed = 0, uses_anonymous_args = 0
	push	{r3, r4, r5, lr}
	movs	r4, #0
	ldr	r3, .L2693
	ldr	r3, [r3]
	ldr	r5, [r3, #8]
.L2689:
	cbnz	r5, .L2690
	ldr	r3, .L2693+4
	ldrb	r1, [r3]	@ zero_extendqisi2
	ldr	r3, .L2693+8
	ldrb	r2, [r3]	@ zero_extendqisi2
	cbnz	r4, .L2691
	ldr	r0, .L2693+12
	ldr	r3, .L2693+16
	str	r0, [r3]
	ldr	r0, .L2693+20
	bl	NAND_Print_DBG
.L2688:
	mov	r0, r4
	pop	{r3, r4, r5, pc}
.L2690:
	mov	r0, r5
	bl	micron_readretry_init
	ldr	r5, [r5]
	orrs	r4, r4, r0
	b	.L2689
.L2691:
	ldr	r0, .L2693+24
	bl	NAND_Print
	b	.L2688
.L2694:
	.align	2
.L2693:
	.word	g_nsi
	.word	.LANCHOR79
	.word	.LANCHOR81
	.word	micron_read_page_end
	.word	df_read_page_end
	.word	.LC470
	.word	.LC471
	.size	micron_special_init, .-micron_special_init
	.section	.text.micron_special_exit,"ax",%progbits
	.align	1
	.global	micron_special_exit
	.syntax unified
	.thumb
	.thumb_func
	.fpu softvfp
	.type	micron_special_exit, %function
micron_special_exit:
	@ args = 0, pretend = 0, frame = 0
	@ frame_needed = 0, uses_anonymous_args = 0
	push	{r3, r4, r5, lr}
	movs	r5, #0
	ldr	r3, .L2702
	ldr	r2, .L2702+4
	ldr	r3, [r3]
	ldrb	r1, [r2]	@ zero_extendqisi2
	ldr	r0, [r3, #8]
	ldr	r3, .L2702+8
	ldrb	r4, [r3]	@ zero_extendqisi2
.L2696:
	cbnz	r0, .L2697
	cbz	r5, .L2698
	strb	r1, [r2]
	strb	r4, [r3]
.L2698:
	ldr	r0, .L2702+12
	bl	NAND_Print_DBG
	movs	r0, #0
	pop	{r3, r4, r5, pc}
.L2697:
	movs	r1, #0
	ldr	r0, [r0]
	movs	r5, #1
	mov	r4, r1
	b	.L2696
.L2703:
	.align	2
.L2702:
	.word	g_nsi
	.word	.LANCHOR79
	.word	.LANCHOR81
	.word	.LC472
	.size	micron_special_exit, .-micron_special_exit
	.section	.text.sandisk_read_retry_clock_save,"ax",%progbits
	.align	1
	.global	sandisk_read_retry_clock_save
	.syntax unified
	.thumb
	.thumb_func
	.fpu softvfp
	.type	sandisk_read_retry_clock_save, %function
sandisk_read_retry_clock_save:
	@ args = 0, pretend = 0, frame = 0
	@ frame_needed = 0, uses_anonymous_args = 0
	push	{r4, lr}
	mov	r4, r0
	ldr	r3, .L2705
	ldr	r0, [r0, #260]
	ldr	r3, [r3]
	ldr	r2, .L2705+4
	ldr	r1, .L2705+8
	ldr	r0, [r0, #8]
	blx	r3
	ldr	r0, [r4, #260]
	movs	r2, #20
	ldr	r3, .L2705+12
	movs	r1, #10
	ldr	r0, [r0, #8]
	ldr	r3, [r3]
	blx	r3
	movs	r0, #0
	pop	{r4, pc}
.L2706:
	.align	2
.L2705:
	.word	NAND_GetClk
	.word	.LANCHOR86
	.word	.LANCHOR87
	.word	NAND_SetClk
	.size	sandisk_read_retry_clock_save, .-sandisk_read_retry_clock_save
	.section	.text.sandisk_vender_set_param,"ax",%progbits
	.align	1
	.global	sandisk_vender_set_param
	.syntax unified
	.thumb
	.thumb_func
	.fpu softvfp
	.type	sandisk_vender_set_param, %function
sandisk_vender_set_param:
	@ args = 0, pretend = 0, frame = 0
	@ frame_needed = 0, uses_anonymous_args = 0
	push	{r4, r5, r6, r7, r8, r10, lr}
	mov	r6, r2
	ldr	r2, .L2711
	sub	sp, sp, #20
	mov	r5, r0
	mov	r7, r1
	movs	r4, #0
	ldrb	r3, [r2]	@ zero_extendqisi2
	mov	r8, r2
	subs	r3, r3, #48
	cmp	r3, #1
	bhi	.L2710
	movs	r3, #2
	mov	r2, r4
	strd	r4, r3, [sp, #4]
	mov	r3, r4
	ldr	r1, .L2711+4
	str	r4, [sp]
	bl	set_cmd_with_nand_bus
	movs	r3, #1
	movs	r2, #3
	strd	r3, r2, [sp, #4]
	mov	r10, r0
	str	r7, [sp]
	mov	r3, r6
	mov	r2, r4
	ldr	r1, .L2711+8
	mov	r0, r5
	bl	set_cmd_with_nand_bus
	orr	r0, r10, r0
.L2708:
	ldrb	r3, [r8]	@ zero_extendqisi2
	cmp	r3, #50
	bne	.L2707
	movs	r3, #4
	movs	r2, #1
	strd	r3, r2, [sp, #4]
	mov	r0, r5
	str	r7, [sp]
	mov	r3, r6
	ldr	r1, .L2711+12
	bl	set_cmd_with_nand_bus
.L2707:
	add	sp, sp, #20
	@ sp needed
	pop	{r4, r5, r6, r7, r8, r10, pc}
.L2710:
	mov	r0, r4
	b	.L2708
.L2712:
	.align	2
.L2711:
	.word	.LANCHOR88
	.word	.LANCHOR89
	.word	.LANCHOR90
	.word	.LANCHOR91
	.size	sandisk_vender_set_param, .-sandisk_vender_set_param
	.section	.text.sandisk_read_retry_clock_recover,"ax",%progbits
	.align	1
	.global	sandisk_read_retry_clock_recover
	.syntax unified
	.thumb
	.thumb_func
	.fpu softvfp
	.type	sandisk_read_retry_clock_recover, %function
sandisk_read_retry_clock_recover:
	@ args = 0, pretend = 0, frame = 0
	@ frame_needed = 0, uses_anonymous_args = 0
	push	{r3, lr}
	ldr	r0, [r0, #260]
	ldr	r3, .L2714
	ldr	r2, .L2714+4
	ldr	r1, .L2714+8
	ldr	r3, [r3]
	ldr	r2, [r2]
	ldr	r1, [r1]
	ldr	r0, [r0, #8]
	blx	r3
	movs	r0, #0
	pop	{r3, pc}
.L2715:
	.align	2
.L2714:
	.word	NAND_SetClk
	.word	.LANCHOR86
	.word	.LANCHOR87
	.size	sandisk_read_retry_clock_recover, .-sandisk_read_retry_clock_recover
	.section	.text.sandisk_end_one_read_retry,"ax",%progbits
	.align	1
	.global	sandisk_end_one_read_retry
	.syntax unified
	.thumb
	.thumb_func
	.fpu softvfp
	.type	sandisk_end_one_read_retry, %function
sandisk_end_one_read_retry:
	@ args = 0, pretend = 0, frame = 0
	@ frame_needed = 0, uses_anonymous_args = 0
	ldr	r3, .L2723
	push	{r4, r5, lr}
	mov	r4, r0
	sub	sp, sp, #20
	ldrb	r3, [r3]	@ zero_extendqisi2
	subs	r3, r3, #48
	cmp	r3, #1
	bhi	.L2717
	bl	nand_enable_chip
	ldr	r0, [r4, #260]
	bl	ndfc_disable_randomize
	ldr	r0, [r4, #260]
	bl	ndfc_is_toggle_interface
	ldr	r5, .L2723+4
	cbz	r0, .L2718
	ldr	r0, [r4, #260]
	bl	ndfc_set_legacy_interface
	movs	r3, #1
	str	r3, [r5]
.L2718:
	movs	r3, #0
	movs	r2, #1
	strd	r3, r2, [sp, #4]
	mov	r0, r4
	str	r3, [sp]
	mov	r2, r3
	ldr	r1, .L2723+8
	bl	set_cmd_with_nand_bus
	ldr	r3, [r5]
	cmp	r3, #1
	bne	.L2719
	ldr	r0, [r4, #260]
	bl	ndfc_set_toggle_interface
.L2719:
	mov	r0, r4
	bl	nand_disable_chip
.L2717:
	movs	r0, #0
	add	sp, sp, #20
	@ sp needed
	pop	{r4, r5, pc}
.L2724:
	.align	2
.L2723:
	.word	.LANCHOR88
	.word	.LANCHOR92
	.word	.LANCHOR93
	.size	sandisk_end_one_read_retry, .-sandisk_end_one_read_retry
	.section	.text.sandisk_set_readretry,"ax",%progbits
	.align	1
	.global	sandisk_set_readretry
	.syntax unified
	.thumb
	.thumb_func
	.fpu softvfp
	.type	sandisk_set_readretry, %function
sandisk_set_readretry:
	@ args = 0, pretend = 0, frame = 0
	@ frame_needed = 0, uses_anonymous_args = 0
	push	{r4, r5, r6, lr}
	mov	r4, r0
	mov	r6, r1
	ldr	r5, .L2750
	bl	sandisk_read_retry_clock_save
	mov	r0, r4
	bl	nand_enable_chip
	ldr	r0, [r4, #260]
	bl	ndfc_disable_randomize
	ldr	r0, [r4, #260]
	bl	ndfc_is_toggle_interface
	cbz	r0, .L2726
	ldr	r0, [r4, #260]
	bl	ndfc_set_legacy_interface
	movs	r3, #1
	str	r3, [r5]
.L2726:
	ldr	r3, .L2750+4
	ldrb	r3, [r3]	@ zero_extendqisi2
	cmp	r3, #48
	bne	.L2727
	cmp	r6, #255
	ldr	r3, [r4, #176]
	ldr	r2, .L2750+8
	beq	.L2728
	cbz	r6, .L2729
	lsls	r1, r6, #31
	bpl	.L2728
.L2729:
	ldr	r1, .L2750+12
	add	r3, r3, r3, lsl #1
	add	r1, r1, r3
	movs	r3, #16
.L2747:
	strb	r3, [r2]
	movs	r3, #3
	ldr	r2, .L2750+16
.L2749:
	mov	r0, r4
	bl	sandisk_vender_set_param
	movs	r2, #0
	mov	r6, r0
	movs	r1, #182
.L2748:
	mov	r0, r4
	bl	set_one_cmd
.L2731:
	ldr	r3, [r5]
	cmp	r3, #1
	bne	.L2733
	ldr	r0, [r4, #260]
	bl	ndfc_set_toggle_interface
.L2733:
	mov	r0, r4
	bl	nand_disable_chip
	mov	r0, r4
	bl	sandisk_read_retry_clock_recover
	mov	r0, r6
	pop	{r4, r5, r6, pc}
.L2728:
	ldr	r1, .L2750+20
	add	r3, r3, r3, lsl #1
	add	r1, r1, r3
	movs	r3, #20
	b	.L2747
.L2727:
	cmp	r3, #49
	bne	.L2732
	ldr	r1, [r4, #176]
	movs	r3, #3
	ldr	r0, .L2750+24
	ldr	r2, .L2750+28
	add	r1, r1, r1, lsl #1
	add	r1, r1, r0
	b	.L2749
.L2732:
	cmp	r3, #50
	bne	.L2734
	ldr	r0, [r4, #176]
	movs	r3, #1
	ldr	r1, .L2750+32
	ldr	r2, .L2750+36
	add	r1, r1, r0, lsl #2
	mov	r0, r4
	bl	sandisk_vender_set_param
	movs	r2, #0
	mov	r6, r0
	movs	r1, #93
	b	.L2748
.L2734:
	movs	r6, #0
	b	.L2731
.L2751:
	.align	2
.L2750:
	.word	.LANCHOR92
	.word	.LANCHOR88
	.word	.LANCHOR95
	.word	.LANCHOR94
	.word	.LANCHOR97
	.word	.LANCHOR96
	.word	.LANCHOR99
	.word	.LANCHOR98
	.word	.LANCHOR101
	.word	.LANCHOR100
	.size	sandisk_set_readretry, .-sandisk_set_readretry
	.section	.text.sandisk_activation_seq,"ax",%progbits
	.align	1
	.global	sandisk_activation_seq
	.syntax unified
	.thumb
	.thumb_func
	.fpu softvfp
	.type	sandisk_activation_seq, %function
sandisk_activation_seq:
	@ args = 0, pretend = 0, frame = 24
	@ frame_needed = 0, uses_anonymous_args = 0
	push	{r4, r5, r6, r7, r8, lr}
	mov	r8, #9
	sub	sp, sp, #40
	mov	r6, r0
	mov	r2, r8
	add	r0, sp, #16
	ldr	r1, .L2757
	movs	r4, #0
	bl	memcpy
	mov	r0, r6
	strd	r4, r4, [sp, #28]
	strb	r4, [sp, #36]
	add	r7, sp, #28
	bl	nand_enable_chip
	ldr	r0, [r6, #260]
	bl	ndfc_disable_randomize
	movs	r3, #2
	mov	r2, r4
	strd	r4, r3, [sp, #4]
	mov	r0, r6
	mov	r3, r4
	ldr	r1, .L2757+4
	str	r4, [sp]
	bl	set_cmd_with_nand_bus
	movs	r3, #1
	mov	r5, r0
	strd	r7, r3, [sp]
	mov	r2, r4
	str	r8, [sp, #8]
	add	r1, sp, #16
	ldr	r3, .L2757+8
	mov	r0, r6
	bl	set_cmd_with_nand_bus
	orrs	r5, r5, r0
	mov	r0, r6
	bl	nand_disable_chip
	cbz	r5, .L2752
	mov	r1, r5
	ldr	r0, .L2757+12
	bl	NAND_Print
.L2752:
	mov	r0, r5
	add	sp, sp, #40
	@ sp needed
	pop	{r4, r5, r6, r7, r8, pc}
.L2758:
	.align	2
.L2757:
	.word	.LANCHOR17+30
	.word	.LANCHOR89
	.word	.LANCHOR102
	.word	.LC473
	.size	sandisk_activation_seq, .-sandisk_activation_seq
	.section	.text.sandisk_readretry_init,"ax",%progbits
	.align	1
	.global	sandisk_readretry_init
	.syntax unified
	.thumb
	.thumb_func
	.fpu softvfp
	.type	sandisk_readretry_init, %function
sandisk_readretry_init:
	@ args = 0, pretend = 0, frame = 0
	@ frame_needed = 0, uses_anonymous_args = 0
	push	{r3, r4, r5, lr}
	movs	r2, #0
	ldr	r3, [r0, #264]
	str	r2, [r0, #176]
	ldr	r4, [r3, #52]
	ldr	r2, .L2763
	ldr	r3, .L2763+4
	ubfx	r1, r4, #16, #8
	lsrs	r5, r4, #8
	strb	r1, [r2]
	subs	r1, r1, #48
	strb	r5, [r3]
	cmp	r1, #1
	ldr	r5, .L2763+8
	strb	r4, [r5]
	mov	r5, r2
	mov	r4, r3
	bhi	.L2760
	bl	sandisk_activation_seq
.L2760:
	ldrb	r3, [r5]	@ zero_extendqisi2
	subs	r3, r3, #48
	cmp	r3, #2
	bhi	.L2762
	ldrb	r1, [r4]	@ zero_extendqisi2
	ldr	r0, .L2763+12
	bl	NAND_Print_DBG
	movs	r0, #0
.L2759:
	pop	{r3, r4, r5, pc}
.L2762:
	mvn	r0, #98
	b	.L2759
.L2764:
	.align	2
.L2763:
	.word	.LANCHOR88
	.word	.LANCHOR95
	.word	.LANCHOR103
	.word	.LC474
	.size	sandisk_readretry_init, .-sandisk_readretry_init
	.section	.text.sandisk_readretry_exit,"ax",%progbits
	.align	1
	.global	sandisk_readretry_exit
	.syntax unified
	.thumb
	.thumb_func
	.fpu softvfp
	.type	sandisk_readretry_exit, %function
sandisk_readretry_exit:
	@ args = 0, pretend = 0, frame = 0
	@ frame_needed = 0, uses_anonymous_args = 0
	@ link register save eliminated.
	movs	r0, #0
	bx	lr
	.size	sandisk_readretry_exit, .-sandisk_readretry_exit
	.section	.text.sandisk_exit_readretry,"ax",%progbits
	.align	1
	.global	sandisk_exit_readretry
	.syntax unified
	.thumb
	.thumb_func
	.fpu softvfp
	.type	sandisk_exit_readretry, %function
sandisk_exit_readretry:
	@ args = 0, pretend = 0, frame = 8
	@ frame_needed = 0, uses_anonymous_args = 0
	push	{r0, r1, r4, r5, r6, lr}
	movs	r3, #0
	mov	r4, r0
	str	r3, [sp, #4]
	bl	nand_enable_chip
	ldr	r0, [r4, #260]
	bl	ndfc_disable_randomize
	ldr	r0, [r4, #260]
	bl	ndfc_is_toggle_interface
	ldr	r6, .L2777
	cbz	r0, .L2767
	ldr	r0, [r4, #260]
	bl	ndfc_set_legacy_interface
	movs	r3, #1
	str	r3, [r6]
.L2767:
	ldr	r3, .L2777+4
	ldrb	r3, [r3]	@ zero_extendqisi2
	cmp	r3, #48
	bne	.L2768
	ldr	r2, .L2777+8
	movs	r3, #3
.L2776:
	add	r1, sp, #4
	mov	r0, r4
	bl	sandisk_vender_set_param
	mov	r5, r0
.L2769:
	ldr	r3, [r6]
	cmp	r3, #1
	bne	.L2771
	ldr	r0, [r4, #260]
	bl	ndfc_set_toggle_interface
.L2771:
	mov	r0, r4
	bl	nand_disable_chip
	mov	r0, r5
	add	sp, sp, #8
	@ sp needed
	pop	{r4, r5, r6, pc}
.L2768:
	cmp	r3, #49
	bne	.L2770
	movs	r3, #3
	ldr	r2, .L2777+12
	b	.L2776
.L2770:
	cmp	r3, #50
	bne	.L2772
	movs	r2, #1
	movs	r1, #255
	mov	r0, r4
	bl	set_one_cmd
	movs	r3, #1
	ldr	r2, .L2777+16
	b	.L2776
.L2772:
	movs	r5, #0
	b	.L2769
.L2778:
	.align	2
.L2777:
	.word	.LANCHOR92
	.word	.LANCHOR88
	.word	.LANCHOR97
	.word	.LANCHOR98
	.word	.LANCHOR100
	.size	sandisk_exit_readretry, .-sandisk_exit_readretry
	.section	.text.sandisk_read_page_end,"ax",%progbits
	.align	1
	.global	sandisk_read_page_end
	.syntax unified
	.thumb
	.thumb_func
	.fpu softvfp
	.type	sandisk_read_page_end, %function
sandisk_read_page_end:
	@ args = 0, pretend = 0, frame = 0
	@ frame_needed = 0, uses_anonymous_args = 0
	ldr	r3, .L2790
	push	{r0, r1, r2, r4, r5, r6, r7, r8, r10, lr}
	mov	r6, r0
	ldr	r1, [r0]
	ldr	r0, [r3]
	bl	nci_get_from_nsi
	mov	r5, r0
	mov	r0, r6
	bl	generic_read_page_end_not_retry
	adds	r3, r0, #2
	mov	r4, r0
	bne	.L2779
	ldr	r8, .L2790+12
	movs	r7, #0
	ldr	r10, .L2790+16
	ldr	r0, .L2790+4
	bl	NAND_Print_DBG
.L2781:
	ldrb	r3, [r8]	@ zero_extendqisi2
	cmp	r3, r7
	ble	.L2785
	str	r7, [r5, #176]
	mov	r0, r5
	ldrh	r1, [r6, #8]
	bl	sandisk_set_readretry
	mov	r4, r0
	cbnz	r0, .L2782
	mov	r0, r6
	bl	generic_read_page_start
	mov	r4, r0
	mov	r0, r6
	bl	generic_read_page_end_not_retry
	orrs	r4, r4, r0
	mov	r0, r5
	bl	sandisk_end_one_read_retry
	ldr	r0, [r6, #20]
	bl	is_nouse_page
	cmp	r0, #1
	bne	.L2783
	ldr	r3, [r5, #176]
	mvn	r4, #1
	ldr	r1, [r5, #260]
	mov	r0, r10
	str	r3, [sp, #4]
	ldr	r3, [r6, #8]
	str	r3, [sp]
	ldr	r3, [r6, #4]
	ldr	r2, [r5, #20]
	ldr	r1, [r1, #8]
	bl	NAND_Print_DBG
.L2782:
	adds	r7, r7, #1
	b	.L2781
.L2783:
	cmp	r4, #10
	beq	.L2784
	cmp	r4, #0
	bne	.L2782
.L2784:
	ldr	r3, [r5, #176]
	movs	r4, #10
	ldr	r1, [r5, #260]
	ldr	r0, .L2790+8
	str	r3, [sp, #4]
	ldr	r3, [r6, #8]
	str	r3, [sp]
	ldr	r3, [r6, #4]
	ldr	r2, [r5, #20]
	ldr	r1, [r1, #8]
	bl	NAND_Print_DBG
.L2785:
	movs	r3, #0
	mov	r0, r5
	str	r3, [r5, #176]
	bl	sandisk_exit_readretry
.L2779:
	mov	r0, r4
	add	sp, sp, #12
	@ sp needed
	pop	{r4, r5, r6, r7, r8, r10, pc}
.L2791:
	.align	2
.L2790:
	.word	g_nsi
	.word	.LC475
	.word	.LC476
	.word	.LANCHOR95
	.word	.LC448
	.size	sandisk_read_page_end, .-sandisk_read_page_end
	.section	.text.sandisk_special_init,"ax",%progbits
	.align	1
	.global	sandisk_special_init
	.syntax unified
	.thumb
	.thumb_func
	.fpu softvfp
	.type	sandisk_special_init, %function
sandisk_special_init:
	@ args = 0, pretend = 0, frame = 0
	@ frame_needed = 0, uses_anonymous_args = 0
	push	{r3, r4, r5, lr}
	movs	r4, #0
	ldr	r3, .L2797
	ldr	r3, [r3]
	ldr	r5, [r3, #8]
.L2793:
	cbnz	r5, .L2794
	ldr	r3, .L2797+4
	ldrb	r1, [r3]	@ zero_extendqisi2
	ldr	r3, .L2797+8
	ldrb	r2, [r3]	@ zero_extendqisi2
	ldr	r3, .L2797+12
	ldrb	r3, [r3]	@ zero_extendqisi2
	cbnz	r4, .L2795
	ldr	r0, .L2797+16
	ldr	r5, .L2797+20
	str	r5, [r0]
	ldr	r0, .L2797+24
	bl	NAND_Print_DBG
.L2792:
	mov	r0, r4
	pop	{r3, r4, r5, pc}
.L2794:
	mov	r0, r5
	bl	sandisk_readretry_init
	ldr	r5, [r5]
	orrs	r4, r4, r0
	b	.L2793
.L2795:
	ldr	r0, .L2797+28
	bl	NAND_Print
	b	.L2792
.L2798:
	.align	2
.L2797:
	.word	g_nsi
	.word	.LANCHOR88
	.word	.LANCHOR95
	.word	.LANCHOR103
	.word	df_read_page_end
	.word	sandisk_read_page_end
	.word	.LC477
	.word	.LC478
	.size	sandisk_special_init, .-sandisk_special_init
	.section	.text.sandisk_special_exit,"ax",%progbits
	.align	1
	.global	sandisk_special_exit
	.syntax unified
	.thumb
	.thumb_func
	.fpu softvfp
	.type	sandisk_special_exit, %function
sandisk_special_exit:
	@ args = 0, pretend = 0, frame = 0
	@ frame_needed = 0, uses_anonymous_args = 0
	ldr	r3, .L2802
	push	{r4, lr}
	ldr	r3, [r3]
	ldr	r4, [r3, #8]
.L2800:
	cbnz	r4, .L2801
	ldr	r0, .L2802+4
	bl	NAND_Print_DBG
	mov	r0, r4
	pop	{r4, pc}
.L2801:
	ldr	r4, [r4]
	b	.L2800
.L2803:
	.align	2
.L2802:
	.word	g_nsi
	.word	.LC479
	.size	sandisk_special_exit, .-sandisk_special_exit
	.section	.text.hynix20nm_vender_set_param,"ax",%progbits
	.align	1
	.global	hynix20nm_vender_set_param
	.syntax unified
	.thumb
	.thumb_func
	.fpu softvfp
	.type	hynix20nm_vender_set_param, %function
hynix20nm_vender_set_param:
	@ args = 0, pretend = 0, frame = 8
	@ frame_needed = 0, uses_anonymous_args = 0
	push	{r4, r5, r6, r7, r8, r10, lr}
	mov	r4, r2
	sub	sp, sp, #28
	mov	r7, r3
	mov	r6, r0
	movs	r3, #54
	add	r8, sp, #24
	add	r7, r7, r4
	mov	r10, #1
	mov	r5, r1
	strb	r3, [r8, #-1]!
	bl	nand_enable_chip
	ldr	r0, [r6, #260]
	bl	ndfc_disable_randomize
.L2805:
	cmp	r4, r7
	bne	.L2806
	mov	r0, r6
	bl	nand_disable_chip
	movs	r0, #0
	add	sp, sp, #28
	@ sp needed
	pop	{r4, r5, r6, r7, r8, r10, pc}
.L2806:
	mov	r3, r4
	str	r5, [sp]
	movs	r2, #0
	mov	r1, r8
	strd	r10, r10, [sp, #4]
	mov	r0, r6
	bl	set_cmd_with_nand_bus
	movs	r2, #0
	movs	r1, #22
	mov	r0, r6
	adds	r4, r4, #1
	bl	set_one_cmd
	adds	r5, r5, #1
	b	.L2805
	.size	hynix20nm_vender_set_param, .-hynix20nm_vender_set_param
	.section	.text.hynix20nm_set_readretry,"ax",%progbits
	.align	1
	.global	hynix20nm_set_readretry
	.syntax unified
	.thumb
	.thumb_func
	.fpu softvfp
	.type	hynix20nm_set_readretry, %function
hynix20nm_set_readretry:
	@ args = 0, pretend = 0, frame = 8
	@ frame_needed = 0, uses_anonymous_args = 0
	ldr	r3, .L2814
	push	{r0, r1, r2, r4, r5, r6, r7, lr}
	ldr	r1, [r0, #176]
	ldrb	r3, [r3]	@ zero_extendqisi2
	cmp	r1, r3
	bhi	.L2813
	ldr	r5, .L2814+4
	lsls	r1, r1, #3
	movs	r2, #0
	ldrb	r3, [r5]	@ zero_extendqisi2
.L2809:
	cmp	r3, r2
	bhi	.L2810
	ldr	r7, .L2814+8
	mov	r1, sp
	ldr	r2, .L2814+12
	movs	r4, #0
	bl	hynix20nm_vender_set_param
	mov	r6, r0
	ldr	r0, .L2814+16
	bl	NAND_Print_DBG
.L2811:
	ldrb	r3, [r5]	@ zero_extendqisi2
	cmp	r3, r4
	bhi	.L2812
	ldr	r0, .L2814+20
	bl	NAND_Print_DBG
.L2807:
	mov	r0, r6
	add	sp, sp, #12
	@ sp needed
	pop	{r4, r5, r6, r7, pc}
.L2810:
	adds	r4, r1, r2
	add	r4, r4, r0
	ldrb	r4, [r4, #48]	@ zero_extendqisi2
	strb	r4, [sp, r2]
	adds	r2, r2, #1
	b	.L2809
.L2812:
	ldrb	r1, [sp, r4]	@ zero_extendqisi2
	mov	r0, r7
	bl	NAND_Print_DBG
	adds	r4, r4, #1
	b	.L2811
.L2813:
	mvn	r6, #91
	b	.L2807
.L2815:
	.align	2
.L2814:
	.word	.LANCHOR104
	.word	.LANCHOR105
	.word	.LC435
	.word	.LANCHOR106
	.word	.LC480
	.word	.LC7
	.size	hynix20nm_set_readretry, .-hynix20nm_set_readretry
	.section	.text.hynix20nm_read_page_end,"ax",%progbits
	.align	1
	.global	hynix20nm_read_page_end
	.syntax unified
	.thumb
	.thumb_func
	.fpu softvfp
	.type	hynix20nm_read_page_end, %function
hynix20nm_read_page_end:
	@ args = 0, pretend = 0, frame = 0
	@ frame_needed = 0, uses_anonymous_args = 0
	ldr	r3, .L2832
	push	{r0, r1, r4, r5, r6, r7, r8, r10, fp, lr}
	mov	r6, r0
	ldr	r1, [r0]
	ldr	r0, [r3]
	bl	nci_get_from_nsi
	mov	r5, r0
	mov	r0, r6
	bl	generic_read_page_end_not_retry
	adds	r3, r0, #2
	mov	r4, r0
	bne	.L2816
	ldr	r8, .L2832+16
	movs	r7, #0
	ldr	r10, .L2832+20
	ldr	r0, .L2832+4
	bl	NAND_Print_DBG
.L2818:
	ldrb	r2, [r8]	@ zero_extendqisi2
	cmp	r2, r7
	ble	.L2822
	ldr	r3, [r5, #176]
	mov	r0, r5
	adds	r3, r3, #1
	cmp	r3, r2
	it	hi
	movhi	r3, #0
	str	r3, [r5, #176]
	bl	hynix20nm_set_readretry
	mov	r4, r0
	cbz	r0, .L2821
	movs	r3, #0
	mov	r0, r5
	str	r3, [r5, #176]
	bl	hynix20nm_set_readretry
.L2822:
	movs	r3, #0
	mov	r0, r5
	str	r3, [r5, #176]
	bl	hynix20nm_set_readretry
.L2816:
	mov	r0, r4
	add	sp, sp, #8
	@ sp needed
	pop	{r4, r5, r6, r7, r8, r10, fp, pc}
.L2821:
	mov	r0, r6
	bl	generic_read_page_start
	mov	r4, r0
	mov	r0, r6
	bl	generic_read_page_end_not_retry
	ldr	fp, [r6, #20]
	orrs	r4, r4, r0
	mov	r0, fp
	bl	is_nouse_page
	cmp	r0, #1
	bne	.L2823
	ldr	r3, [r5, #176]
	mvn	r4, #1
	ldr	r1, [r5, #260]
	mov	r0, r10
	str	r3, [sp, #4]
	ldr	r3, [r6, #8]
	str	r3, [sp]
	ldr	r3, [r6, #4]
	ldr	r2, [r5, #20]
	ldr	r1, [r1, #8]
	bl	NAND_Print_DBG
.L2824:
	adds	r7, r7, #1
	b	.L2818
.L2823:
	cmp	r4, #10
	bne	.L2825
.L2826:
	ldrb	r0, [fp, #4]	@ zero_extendqisi2
	movs	r4, #10
	ldrb	r3, [fp, #3]	@ zero_extendqisi2
	ldrb	r2, [fp, #2]	@ zero_extendqisi2
	ldrb	r1, [fp, #1]	@ zero_extendqisi2
	str	r0, [sp]
	ldr	r0, .L2832+8
	bl	NAND_Print_DBG
	ldr	r3, [r5, #176]
	ldr	r1, [r5, #260]
	ldr	r0, .L2832+12
	str	r3, [sp, #4]
	ldr	r3, [r6, #8]
	str	r3, [sp]
	ldr	r3, [r6, #4]
	ldr	r2, [r5, #20]
	ldr	r1, [r1, #8]
	bl	NAND_Print_DBG
	b	.L2822
.L2825:
	cmp	r4, #0
	bne	.L2824
	b	.L2826
.L2833:
	.align	2
.L2832:
	.word	g_nsi
	.word	.LC481
	.word	.LC482
	.word	.LC483
	.word	.LANCHOR104
	.word	.LC448
	.size	hynix20nm_read_page_end, .-hynix20nm_read_page_end
	.section	.text.hynix20nm_vender_get_param,"ax",%progbits
	.align	1
	.global	hynix20nm_vender_get_param
	.syntax unified
	.thumb
	.thumb_func
	.fpu softvfp
	.type	hynix20nm_vender_get_param, %function
hynix20nm_vender_get_param:
	@ args = 0, pretend = 0, frame = 8
	@ frame_needed = 0, uses_anonymous_args = 0
	push	{r4, r5, r6, r7, r8, r10, lr}
	mov	r7, r3
	sub	sp, sp, #20
	mov	r4, r2
	mov	r6, r0
	mov	r5, r1
	add	r7, r7, r4
	mov	r10, #1
	bl	nand_enable_chip
	ldr	r0, [r6, #260]
	add	r8, sp, #16
	bl	ndfc_disable_randomize
	movs	r3, #55
	strb	r3, [r8, #-1]!
.L2835:
	cmp	r4, r7
	bne	.L2836
	mov	r0, r6
	bl	nand_disable_chip
	movs	r0, #0
	add	sp, sp, #20
	@ sp needed
	pop	{r4, r5, r6, r7, r8, r10, pc}
.L2836:
	mov	r3, r5
	mov	r2, r4
	str	r10, [sp]
	mov	r1, r8
	mov	r0, r6
	adds	r4, r4, #1
	bl	get_data_with_nand_bus_one_cmd
	adds	r5, r5, #1
	b	.L2835
	.size	hynix20nm_vender_get_param, .-hynix20nm_vender_get_param
	.section	.text.hynix20nm_lsb_init,"ax",%progbits
	.align	1
	.global	hynix20nm_lsb_init
	.syntax unified
	.thumb
	.thumb_func
	.fpu softvfp
	.type	hynix20nm_lsb_init, %function
hynix20nm_lsb_init:
	@ args = 0, pretend = 0, frame = 0
	@ frame_needed = 0, uses_anonymous_args = 0
	@ link register save eliminated.
	ldr	r3, [r0, #264]
	ldr	r1, .L2841
	ldr	r2, [r3, #52]
	ubfx	r3, r2, #16, #8
	strb	r3, [r1]
	lsrs	r0, r2, #8
	ldr	r1, .L2841+4
	cmp	r3, #2
	strb	r0, [r1]
	ldr	r1, .L2841+8
	strb	r2, [r1]
	bne	.L2838
	ldr	r3, .L2841+12
	movs	r2, #4
	strb	r2, [r3]
	movs	r2, #176
	ldr	r3, .L2841+16
	strb	r2, [r3]
	movs	r2, #177
	strb	r2, [r3, #1]
	movs	r2, #160
	strb	r2, [r3, #2]
	movs	r2, #161
.L2840:
	strb	r2, [r3, #3]
	movs	r2, #10
	ldr	r3, .L2841+20
	strb	r2, [r3]
	strb	r2, [r3, #1]
	strb	r2, [r3, #2]
	strb	r2, [r3, #3]
.L2839:
	movs	r0, #0
	bx	lr
.L2838:
	cmp	r3, #3
	bne	.L2839
	ldr	r3, .L2841+12
	movs	r2, #4
	strb	r2, [r3]
	movs	r2, #160
	ldr	r3, .L2841+16
	strb	r2, [r3]
	movs	r2, #161
	strb	r2, [r3, #1]
	movs	r2, #167
	strb	r2, [r3, #2]
	movs	r2, #168
	b	.L2840
.L2842:
	.align	2
.L2841:
	.word	.LANCHOR107
	.word	.LANCHOR104
	.word	.LANCHOR105
	.word	.LANCHOR108
	.word	.LANCHOR109
	.word	.LANCHOR110
	.size	hynix20nm_lsb_init, .-hynix20nm_lsb_init
	.section	.text.hynix20nm_lsb_enable,"ax",%progbits
	.align	1
	.global	hynix20nm_lsb_enable
	.syntax unified
	.thumb
	.thumb_func
	.fpu softvfp
	.type	hynix20nm_lsb_enable, %function
hynix20nm_lsb_enable:
	@ args = 0, pretend = 0, frame = 8
	@ frame_needed = 0, uses_anonymous_args = 0
	push	{r0, r1, r2, r4, r5, r6, r7, lr}
	mov	r5, r0
	ldr	r4, .L2846
	ldr	r2, .L2846+4
	ldrb	r3, [r4]	@ zero_extendqisi2
	ldr	r1, .L2846+8
	bl	hynix20nm_vender_get_param
	ldr	r6, .L2846+12
	movs	r2, #0
	ldrb	r3, [r4]	@ zero_extendqisi2
	add	r1, sp, #4
	ldr	r4, .L2846+8
.L2844:
	cmp	r3, r2
	bhi	.L2845
	ldr	r2, .L2846+4
	mov	r0, r5
	bl	hynix20nm_vender_set_param
	movs	r0, #0
	add	sp, sp, #12
	@ sp needed
	pop	{r4, r5, r6, r7, pc}
.L2845:
	ldrb	r0, [r2, r4]	@ zero_extendqisi2
	ldrb	r7, [r2, r6]	@ zero_extendqisi2
	add	r0, r0, r7
	strb	r0, [r1, r2]
	adds	r2, r2, #1
	b	.L2844
.L2847:
	.align	2
.L2846:
	.word	.LANCHOR108
	.word	.LANCHOR109
	.word	.LANCHOR111
	.word	.LANCHOR110
	.size	hynix20nm_lsb_enable, .-hynix20nm_lsb_enable
	.section	.text.hynix20nm_lsb_disable,"ax",%progbits
	.align	1
	.global	hynix20nm_lsb_disable
	.syntax unified
	.thumb
	.thumb_func
	.fpu softvfp
	.type	hynix20nm_lsb_disable, %function
hynix20nm_lsb_disable:
	@ args = 0, pretend = 0, frame = 0
	@ frame_needed = 0, uses_anonymous_args = 0
	push	{r3, lr}
	ldr	r3, .L2849
	ldr	r2, .L2849+4
	ldrb	r3, [r3]	@ zero_extendqisi2
	ldr	r1, .L2849+8
	bl	hynix20nm_vender_set_param
	movs	r0, #0
	pop	{r3, pc}
.L2850:
	.align	2
.L2849:
	.word	.LANCHOR108
	.word	.LANCHOR109
	.word	.LANCHOR111
	.size	hynix20nm_lsb_disable, .-hynix20nm_lsb_disable
	.section	.text.hynix20nm_lsb_exit,"ax",%progbits
	.align	1
	.global	hynix20nm_lsb_exit
	.syntax unified
	.thumb
	.thumb_func
	.fpu softvfp
	.type	hynix20nm_lsb_exit, %function
hynix20nm_lsb_exit:
	@ args = 0, pretend = 0, frame = 0
	@ frame_needed = 0, uses_anonymous_args = 0
	push	{r4, lr}
	mov	r4, r0
	bl	nand_enable_chip
	ldr	r0, [r4, #260]
	bl	ndfc_disable_randomize
	movs	r2, #0
	mov	r0, r4
	mov	r1, r2
	bl	set_one_cmd
	movs	r1, #0
	mov	r0, r4
	bl	set_one_addr
	movs	r1, #0
	mov	r0, r4
	bl	set_one_addr
	movs	r1, #0
	mov	r0, r4
	bl	set_one_addr
	movs	r1, #0
	mov	r0, r4
	bl	set_one_addr
	movs	r1, #0
	mov	r0, r4
	bl	set_one_addr
	movs	r2, #1
	movs	r1, #48
	mov	r0, r4
	bl	set_one_cmd
	ldr	r0, .L2852
	bl	NAND_Print_DBG
	mov	r0, r4
	bl	nand_disable_chip
	movs	r0, #0
	pop	{r4, pc}
.L2853:
	.align	2
.L2852:
	.word	.LC484
	.size	hynix20nm_lsb_exit, .-hynix20nm_lsb_exit
	.section	.text.hynix20nm_vender_get_param_otp_hynix,"ax",%progbits
	.align	1
	.global	hynix20nm_vender_get_param_otp_hynix
	.syntax unified
	.thumb
	.thumb_func
	.fpu softvfp
	.type	hynix20nm_vender_get_param_otp_hynix, %function
hynix20nm_vender_get_param_otp_hynix:
	@ args = 0, pretend = 0, frame = 16
	@ frame_needed = 0, uses_anonymous_args = 0
	push	{r4, r5, r6, r7, lr}
	movs	r3, #0
	sub	sp, sp, #36
	mov	r4, r0
	strh	r3, [sp, #28]	@ movhi
	ldr	r3, .L2872
	ldrb	r3, [r3]	@ zero_extendqisi2
	cmp	r3, #2
	bne	.L2855
	movw	r3, #52479
	strh	r3, [sp, #24]	@ movhi
	movw	r3, #19776
	strh	r3, [sp, #28]	@ movhi
.L2856:
	mov	r0, #1024
	movs	r6, #1
	bl	nand_get_temp_buf
	mov	r5, r0
	mov	r0, r4
	bl	nand_enable_chip
	ldr	r0, [r4, #260]
	bl	ndfc_disable_randomize
	movs	r2, #1
	movs	r1, #255
	mov	r0, r4
	bl	set_one_cmd
	movs	r3, #54
	movs	r2, #0
	strb	r3, [sp, #20]
	add	r3, sp, #28
	str	r3, [sp]
	add	r1, sp, #20
	add	r3, sp, #24
	strd	r6, r6, [sp, #4]
	mov	r0, r4
	bl	set_cmd_with_nand_bus
	movs	r2, #0
	add	r3, sp, #29
	mov	r1, r2
	str	r3, [sp]
	strd	r6, r6, [sp, #4]
	add	r3, sp, #25
	mov	r0, r4
	bl	set_cmd_with_nand_bus
	movs	r2, #0
	movs	r1, #22
	mov	r0, r4
	bl	set_one_cmd
	movs	r2, #0
	movs	r1, #23
	mov	r0, r4
	bl	set_one_cmd
	movs	r2, #0
	movs	r1, #4
	mov	r0, r4
	bl	set_one_cmd
	movs	r2, #0
	movs	r1, #25
	mov	r0, r4
	bl	set_one_cmd
	movs	r2, #0
	mov	r0, r4
	mov	r1, r2
	bl	set_one_cmd
	movs	r1, #0
	mov	r0, r4
	bl	set_one_addr
	movs	r1, #0
	mov	r0, r4
	bl	set_one_addr
	movs	r1, #0
	mov	r0, r4
	bl	set_one_addr
	movs	r1, #2
	mov	r0, r4
	bl	set_one_addr
	movs	r1, #0
	mov	r0, r4
	bl	set_one_addr
	movs	r3, #48
	movs	r2, #0
	strb	r3, [sp, #20]
	movs	r3, #2
	str	r3, [sp]
	add	r1, sp, #20
	mov	r3, r5
	mov	r0, r4
	bl	get_data_with_nand_bus_one_cmd
	ldrb	r1, [r5]	@ zero_extendqisi2
	ldrb	r2, [r5, #1]	@ zero_extendqisi2
	cmp	r1, #8
	bne	.L2858
	cmp	r2, #8
	beq	.L2867
.L2858:
	mvn	r6, #95
	ldr	r0, .L2872+4
	bl	NAND_Print
.L2859:
	mov	r3, #1024
	movs	r2, #0
	str	r3, [sp]
	mov	r1, r2
	mov	r3, r5
	mov	r0, r4
	bl	get_data_with_nand_bus_one_cmd
	movs	r1, #0
	add	ip, r4, #112
.L2860:
	add	r2, r5, r1, lsl #7
	add	r3, r4, #48
.L2862:
	ldrb	r7, [r2], #1	@ zero_extendqisi2
	strb	r7, [r3], #1
	ldrb	r0, [r2, #63]	@ zero_extendqisi2
	add	r0, r0, r7
	cmp	r0, #255
	bne	.L2861
	cmp	r3, ip
	bne	.L2862
	ldr	r0, .L2872+8
	bl	NAND_Print_DBG
	b	.L2864
.L2855:
	cmp	r3, #3
	bne	.L2866
	movw	r3, #45230
	strh	r3, [sp, #24]	@ movhi
	movs	r3, #77
	strb	r3, [sp, #29]
	b	.L2856
.L2867:
	movs	r6, #0
	b	.L2859
.L2866:
	mvn	r6, #96
	b	.L2854
.L2861:
	adds	r1, r1, #1
	cmp	r1, #8
	bne	.L2860
	mvn	r6, #94
	ldr	r0, .L2872+12
	bl	NAND_Print_DBG
.L2864:
	movs	r2, #1
	movs	r1, #255
	mov	r0, r4
	bl	set_one_cmd
	movs	r1, #56
	movs	r2, #1
	mov	r0, r4
	bl	set_one_cmd
	mov	r0, r4
	bl	nand_disable_chip
	mov	r1, #1024
	mov	r0, r5
	bl	nand_free_temp_buf
.L2854:
	mov	r0, r6
	add	sp, sp, #36
	@ sp needed
	pop	{r4, r5, r6, r7, pc}
.L2873:
	.align	2
.L2872:
	.word	.LANCHOR107
	.word	.LC410
	.word	.LC486
	.word	.LC485
	.size	hynix20nm_vender_get_param_otp_hynix, .-hynix20nm_vender_get_param_otp_hynix
	.section	.text.hynix20nm_saveotpvalue,"ax",%progbits
	.align	1
	.global	hynix20nm_saveotpvalue
	.syntax unified
	.thumb
	.thumb_func
	.fpu softvfp
	.type	hynix20nm_saveotpvalue, %function
hynix20nm_saveotpvalue:
	@ args = 0, pretend = 0, frame = 0
	@ frame_needed = 0, uses_anonymous_args = 0
	@ link register save eliminated.
	subs	r3, r1, #1
	adds	r0, r0, #48
	adds	r1, r1, #63
.L2875:
	ldrb	r2, [r3, #1]!	@ zero_extendqisi2
	cmp	r3, r1
	strb	r2, [r0], #1
	bne	.L2875
	bx	lr
	.size	hynix20nm_saveotpvalue, .-hynix20nm_saveotpvalue
	.section	.text.hynix20nm_getotpparam,"ax",%progbits
	.align	1
	.global	hynix20nm_getotpparam
	.syntax unified
	.thumb
	.thumb_func
	.fpu softvfp
	.type	hynix20nm_getotpparam, %function
hynix20nm_getotpparam:
	@ args = 0, pretend = 0, frame = 0
	@ frame_needed = 0, uses_anonymous_args = 0
	push	{r4, r5, r6, r7, r8, lr}
	mov	r4, r0
	ldr	r8, .L2887+4
	mov	r5, r1
	movs	r6, #0
.L2880:
	mov	r1, r6
	mov	r0, r8
	bl	NAND_Print_DBG
	mov	r0, r4
	bl	hynix20nm_vender_get_param_otp_hynix
	mov	r7, r0
	cbnz	r0, .L2878
	mov	r0, r4
	mov	r1, r5
	add	r2, r4, #64
.L2879:
	movs	r3, #0
.L2882:
	adds	r4, r0, r3
	ldrb	r4, [r4, #48]	@ zero_extendqisi2
	strb	r4, [r1, r3]
	adds	r3, r3, #1
	cmp	r3, #8
	bne	.L2882
	adds	r0, r0, #8
	adds	r1, r1, #8
	cmp	r2, r0
	bne	.L2879
.L2877:
	mov	r0, r7
	pop	{r4, r5, r6, r7, r8, pc}
.L2878:
	adds	r6, r6, #1
	cmp	r6, #5
	bne	.L2880
	ldr	r0, .L2887
	bl	NAND_Print
	b	.L2877
.L2888:
	.align	2
.L2887:
	.word	.LC487
	.word	.LC414
	.size	hynix20nm_getotpparam, .-hynix20nm_getotpparam
	.section	.text.hynix20nm_set_default_param,"ax",%progbits
	.align	1
	.global	hynix20nm_set_default_param
	.syntax unified
	.thumb
	.thumb_func
	.fpu softvfp
	.type	hynix20nm_set_default_param, %function
hynix20nm_set_default_param:
	@ args = 0, pretend = 0, frame = 8
	@ frame_needed = 0, uses_anonymous_args = 0
	push	{r0, r1, r2, r4, r5, r6, r7, lr}
	add	r1, r0, #48
	ldr	r5, .L2894
	movs	r2, #0
	ldrb	r3, [r5]	@ zero_extendqisi2
.L2890:
	cmp	r3, r2
	bhi	.L2891
	ldr	r7, .L2894+4
	mov	r1, sp
	ldr	r2, .L2894+8
	movs	r4, #0
	bl	hynix20nm_vender_set_param
	mov	r6, r0
	ldr	r0, .L2894+12
	bl	NAND_Print_DBG
.L2892:
	ldrb	r3, [r5]	@ zero_extendqisi2
	cmp	r3, r4
	bhi	.L2893
	ldr	r0, .L2894+16
	bl	NAND_Print_DBG
	mov	r0, r6
	add	sp, sp, #12
	@ sp needed
	pop	{r4, r5, r6, r7, pc}
.L2891:
	ldrb	r4, [r1], #1	@ zero_extendqisi2
	strb	r4, [sp, r2]
	adds	r2, r2, #1
	b	.L2890
.L2893:
	ldrb	r1, [sp, r4]	@ zero_extendqisi2
	mov	r0, r7
	bl	NAND_Print_DBG
	adds	r4, r4, #1
	b	.L2892
.L2895:
	.align	2
.L2894:
	.word	.LANCHOR105
	.word	.LC416
	.word	.LANCHOR106
	.word	.LC415
	.word	.LC7
	.size	hynix20nm_set_default_param, .-hynix20nm_set_default_param
	.section	.text.hynix20nm_init_readretry_param,"ax",%progbits
	.align	1
	.global	hynix20nm_init_readretry_param
	.syntax unified
	.thumb
	.thumb_func
	.fpu softvfp
	.type	hynix20nm_init_readretry_param, %function
hynix20nm_init_readretry_param:
	@ args = 0, pretend = 0, frame = 64
	@ frame_needed = 0, uses_anonymous_args = 0
	ldr	r3, [r0, #264]
	push	{r4, r5, r6, r7, r8, lr}
	mov	r4, r0
	sub	sp, sp, #64
	ldr	r0, [r3, #16]
	lsls	r0, r0, #9
	bl	nand_get_temp_buf
	mov	r6, r0
	cbnz	r0, .L2897
	mvn	r5, #93
	ldr	r0, .L2905
	bl	NAND_Print_DBG
.L2896:
	mov	r0, r5
	add	sp, sp, #64
	@ sp needed
	pop	{r4, r5, r6, r7, r8, pc}
.L2897:
	mov	r0, #32768
	bl	nand_get_temp_buf
	cbnz	r0, .L2899
	ldr	r3, [r4, #264]
	mov	r0, r6
	mvn	r5, #75
	ldr	r1, [r3, #16]
	lsls	r1, r1, #9
	bl	nand_free_temp_buf
	ldr	r0, .L2905+4
	bl	NAND_Print
	b	.L2896
.L2899:
	ldr	r3, [r4, #16]
	movs	r2, #64
	mov	r1, r6
	bl	hynix16nm_get_special_info
	ldr	r3, [r4, #260]
	mov	r5, r0
	cbnz	r0, .L2900
	ldr	r7, .L2905+8
	ldr	r8, .L2905+24
	ldr	r2, [r4, #16]
	ldr	r1, [r3, #8]
	ldr	r0, .L2905+12
	bl	NAND_Print_DBG
.L2902:
	ldrb	r1, [r6, r5]	@ zero_extendqisi2
	mov	r0, r7
	bl	NAND_Print_DBG
	and	r3, r5, #7
	cmp	r3, #7
	bne	.L2901
	mov	r0, r8
	bl	NAND_Print_DBG
.L2901:
	adds	r5, r5, #1
	cmp	r5, #64
	bne	.L2902
	mov	r1, r6
	mov	r0, r4
	bl	hynix20nm_saveotpvalue
	mov	r0, r4
	bl	hynix20nm_set_default_param
	mov	r5, r0
.L2903:
	ldr	r3, [r4, #264]
	mov	r0, r6
	ldr	r1, [r3, #16]
	lsls	r1, r1, #9
	bl	nand_free_temp_buf
	b	.L2896
.L2900:
	ldr	r1, [r3, #8]
	ldr	r0, .L2905+16
	bl	NAND_Print_DBG
	mov	r1, sp
	mov	r0, r4
	bl	hynix20nm_getotpparam
	mov	r5, r0
	cmp	r0, #0
	bne	.L2903
	mov	r0, r4
	bl	hynix20nm_set_default_param
	ldr	r3, [r4, #260]
	ldr	r0, .L2905+20
	ldr	r1, [r3, #8]
	bl	NAND_Print_DBG
	b	.L2903
.L2906:
	.align	2
.L2905:
	.word	.LC488
	.word	.LC417
	.word	.LC419
	.word	.LC489
	.word	.LC420
	.word	.LC490
	.word	.LC7
	.size	hynix20nm_init_readretry_param, .-hynix20nm_init_readretry_param
	.section	.text.hynix20nm_readretry_init,"ax",%progbits
	.align	1
	.global	hynix20nm_readretry_init
	.syntax unified
	.thumb
	.thumb_func
	.fpu softvfp
	.type	hynix20nm_readretry_init, %function
hynix20nm_readretry_init:
	@ args = 0, pretend = 0, frame = 0
	@ frame_needed = 0, uses_anonymous_args = 0
	ldr	r3, [r0, #264]
	ldr	r2, .L2916
	push	{r4, lr}
	ldr	r3, [r3, #52]
	ubfx	r1, r3, #16, #8
	strb	r1, [r2]
	lsrs	r4, r3, #8
	ldr	r2, .L2916+4
	cmp	r1, #2
	strb	r4, [r2]
	ldr	r2, .L2916+8
	strb	r3, [r2]
	bne	.L2908
	ldr	r3, .L2916+12
	movs	r2, #204
	strb	r2, [r3]
	movs	r2, #191
	strb	r2, [r3, #1]
	movs	r2, #170
	strb	r2, [r3, #2]
	movs	r2, #171
	strb	r2, [r3, #3]
	movs	r2, #205
	strb	r2, [r3, #4]
	movs	r2, #173
	strb	r2, [r3, #5]
	movs	r2, #174
	strb	r2, [r3, #6]
	movs	r2, #175
.L2915:
	strb	r2, [r3, #7]
	add	r1, r0, #176
	add	r2, r0, #48
	movs	r3, #0
	str	r3, [r0, #176]
.L2912:
	strb	r3, [r2], #1
	cmp	r2, r1
	bne	.L2912
	pop	{r4, lr}
	b	hynix20nm_init_readretry_param
.L2908:
	cmp	r1, #3
	bne	.L2910
	ldr	r3, .L2916+12
	movs	r2, #176
	strb	r2, [r3]
	movs	r2, #177
	strb	r2, [r3, #1]
	movs	r2, #178
	strb	r2, [r3, #2]
	movs	r2, #179
	strb	r2, [r3, #3]
	movs	r2, #180
	strb	r2, [r3, #4]
	movs	r2, #181
	strb	r2, [r3, #5]
	movs	r2, #182
	strb	r2, [r3, #6]
	movs	r2, #183
	b	.L2915
.L2910:
	ldr	r0, .L2916+16
	bl	NAND_Print
	mvn	r0, #92
	pop	{r4, pc}
.L2917:
	.align	2
.L2916:
	.word	.LANCHOR107
	.word	.LANCHOR104
	.word	.LANCHOR105
	.word	.LANCHOR106
	.word	.LC422
	.size	hynix20nm_readretry_init, .-hynix20nm_readretry_init
	.section	.text.hynix20nm_readretry_exit,"ax",%progbits
	.align	1
	.global	hynix20nm_readretry_exit
	.syntax unified
	.thumb
	.thumb_func
	.fpu softvfp
	.type	hynix20nm_readretry_exit, %function
hynix20nm_readretry_exit:
	@ args = 0, pretend = 0, frame = 0
	@ frame_needed = 0, uses_anonymous_args = 0
	push	{r4, lr}
	mov	r4, r0
	ldr	r0, .L2919
	bl	NAND_Print_DBG
	mov	r0, r4
	bl	hynix20nm_set_default_param
	movs	r0, #0
	str	r0, [r4, #176]
	pop	{r4, pc}
.L2920:
	.align	2
.L2919:
	.word	.LC491
	.size	hynix20nm_readretry_exit, .-hynix20nm_readretry_exit
	.section	.text.hynix20nm_special_init,"ax",%progbits
	.align	1
	.global	hynix20nm_special_init
	.syntax unified
	.thumb
	.thumb_func
	.fpu softvfp
	.type	hynix20nm_special_init, %function
hynix20nm_special_init:
	@ args = 0, pretend = 0, frame = 0
	@ frame_needed = 0, uses_anonymous_args = 0
	push	{r3, r4, r5, lr}
	movs	r4, #0
	ldr	r3, .L2926
	ldr	r3, [r3]
	ldr	r5, [r3, #8]
.L2922:
	cbnz	r5, .L2923
	ldr	r3, .L2926+4
	ldrb	r1, [r3]	@ zero_extendqisi2
	ldr	r3, .L2926+8
	ldrb	r2, [r3]	@ zero_extendqisi2
	ldr	r3, .L2926+12
	ldrb	r3, [r3]	@ zero_extendqisi2
	cbnz	r4, .L2924
	ldr	r0, .L2926+16
	ldr	r5, .L2926+20
	str	r5, [r0]
	ldr	r0, .L2926+24
	bl	NAND_Print_DBG
.L2921:
	mov	r0, r4
	pop	{r3, r4, r5, pc}
.L2923:
	mov	r0, r5
	bl	hynix20nm_readretry_init
	ldr	r5, [r5]
	orrs	r4, r4, r0
	b	.L2922
.L2924:
	ldr	r0, .L2926+28
	bl	NAND_Print
	b	.L2921
.L2927:
	.align	2
.L2926:
	.word	g_nsi
	.word	.LANCHOR107
	.word	.LANCHOR104
	.word	.LANCHOR105
	.word	df_read_page_end
	.word	hynix20nm_read_page_end
	.word	.LC492
	.word	.LC493
	.size	hynix20nm_special_init, .-hynix20nm_special_init
	.section	.text.hynix20nm_special_exit,"ax",%progbits
	.align	1
	.global	hynix20nm_special_exit
	.syntax unified
	.thumb
	.thumb_func
	.fpu softvfp
	.type	hynix20nm_special_exit, %function
hynix20nm_special_exit:
	@ args = 0, pretend = 0, frame = 0
	@ frame_needed = 0, uses_anonymous_args = 0
	ldr	r3, .L2931
	push	{r4, lr}
	ldr	r3, [r3]
	ldr	r4, [r3, #8]
.L2929:
	cbnz	r4, .L2930
	ldr	r0, .L2931+4
	bl	NAND_Print_DBG
	mov	r0, r4
	pop	{r4, pc}
.L2930:
	mov	r0, r4
	bl	hynix20nm_readretry_exit
	ldr	r4, [r4]
	b	.L2929
.L2932:
	.align	2
.L2931:
	.word	g_nsi
	.word	.LC494
	.size	hynix20nm_special_exit, .-hynix20nm_special_exit
	.global	hynix20nm_lsb_mode_reg_num
	.global	hynix20nm_lsb_mode_val
	.global	hynix20nm_lsb_mode_default_val
	.global	hynix20nm_lsb_mode_reg_adr
	.global	hynix20nm_read_retry_reg_adr_2
	.global	hynix20nm_read_retry_reg_adr_1
	.global	hynix20nm_read_retry_reg_adr
	.global	hynix20nm_read_retry_reg_num
	.global	hynix20nm_read_retry_cycle
	.global	hynix20nm_read_retry_mode
	.global	sandisk_1y
	.global	sandisk_read_retry_reg_adr_1y
	.global	sandisk_read_retry_cmd_1y
	.global	sandisk_read_retry_exit_cmd_1y
	.global	sandisk_read_retry_enable_cmd_1y
	.global	sandisk_24
	.global	sandisk_read_retry_reg_adr_24
	.global	sandisk_read_retry_cmd_24
	.global	sandisk_19_high
	.global	sandisk_19_low
	.global	sandisk_read_retry_reg_adr_19
	.global	sandisk_read_retry_cmd_19
	.global	sandisk_read_retry_disable_cmd_19
	.global	sandisk_read_retry_enable_cmd_19
	.global	sandisk_acti_start_addr
	.global	sandisk_acti_start_cmd
	.global	sandisk_read_retry_reg_cnt
	.global	sandisk_read_retry_cycle
	.global	sandisk_read_retry_mode
	.global	micron_p1_4
	.global	micron_p1_3
	.global	micron_p1_2
	.global	micron_p1_1
	.global	micron_read_retry_cycle
	.global	micron_read_retry_mode
	.global	toshiba_read_retry_exit_cmd
	.global	toshiba_read_retry_last_cmd
	.global	toshiba_p1_3
	.global	toshiba_p1_2
	.global	toshiba_read_retry_addr_2
	.global	toshiba_read_retry_cmd_2
	.global	toshiba_p1_1
	.global	toshiba_read_retry_addr_1
	.global	toshiba_read_retry_cmd_1
	.global	toshiba_read_retry_end_cmd
	.global	toshiba_read_retry_start_cmd
	.global	toshiba_read_retry_reg_cnt
	.global	toshiba_read_retry_cycle
	.global	toshiba_read_retry_mode
	.global	sandisk_A19_1z_8g
	.global	sandisk_A19_1z_16g
	.global	sandisk_A19_1y
	.global	sandisk_A19_read_retry_reg_adr_1y
	.global	sandisk_A19_read_retry_cmd_1y
	.global	sandisk_A19_read_retry_exit_cmd_1y
	.global	sandisk_A19_read_retry_enable_cmd_1y
	.global	sandisk_A19_read_retry_reg_cnt
	.global	sandisk_A19_read_retry_cycle
	.global	sandisk_A19_read_retry_mode
	.global	para1
	.global	para0
	.global	hynix26nm_lsb_mode_reg_num
	.global	hynix26nm_lsb_mode_val
	.global	hynix26nm_lsb_mode_default_val
	.global	hynix26nm_lsb_mode_reg_adr
	.global	hynix26nm_read_retry_val
	.global	hynix26nm_read_retry_reg_adr
	.global	hynix26nm_read_retry_reg_num
	.global	hynix26nm_read_retry_cycle
	.global	hynix26nm_read_retry_mode
	.global	samsung_p1
	.global	samsung_read_retry_cmd
	.global	samsung_read_retry_addr
	.global	samsung_read_retry_cycle
	.global	samsung_read_retry_mode
	.global	hynix16nm_read_retry_reg_adr
	.global	hynix16nm_read_retry_reg_num
	.global	hynix16nm_read_retry_cycle
	.global	hynix16nm_read_retry_mode
	.global	nftl_blk_head
	.global	phyinfo
	.global	mod
	.global	com
	.section	.rodata
	.set	.LANCHOR17,. + 0
.LC2:
	.byte	44
	.byte	100
	.byte	68
	.byte	50
	.byte	-91
	.byte	-1
	.byte	-1
	.byte	-1
.LC3:
	.byte	44
	.byte	-124
	.byte	68
	.byte	50
	.byte	-86
	.byte	4
	.byte	0
	.byte	0
.LC1:
	.byte	115
	.byte	117
	.byte	110
	.byte	120
	.byte	105
.LC0:
	.byte	97
	.byte	108
	.byte	108
	.byte	119
	.byte	105
	.byte	110
	.byte	110
	.byte	101
	.byte	114
.LC4:
	.byte	83
	.byte	83
	.byte	83
	.byte	83
	.byte	83
	.byte	83
	.byte	83
	.byte	83
	.byte	83
	.section	.bss.hynix16nm_read_retry_cycle,"aw",%nobits
	.set	.LANCHOR30,. + 0
	.type	hynix16nm_read_retry_cycle, %object
	.size	hynix16nm_read_retry_cycle, 1
hynix16nm_read_retry_cycle:
	.space	1
	.section	.bss.hynix16nm_read_retry_mode,"aw",%nobits
	.set	.LANCHOR33,. + 0
	.type	hynix16nm_read_retry_mode, %object
	.size	hynix16nm_read_retry_mode, 1
hynix16nm_read_retry_mode:
	.space	1
	.section	.bss.hynix16nm_read_retry_reg_adr,"aw",%nobits
	.set	.LANCHOR32,. + 0
	.type	hynix16nm_read_retry_reg_adr, %object
	.size	hynix16nm_read_retry_reg_adr, 4
hynix16nm_read_retry_reg_adr:
	.space	4
	.section	.bss.hynix16nm_read_retry_reg_num,"aw",%nobits
	.set	.LANCHOR31,. + 0
	.type	hynix16nm_read_retry_reg_num, %object
	.size	hynix16nm_read_retry_reg_num, 1
hynix16nm_read_retry_reg_num:
	.space	1
	.section	.bss.hynix20nm_lsb_mode_default_val,"aw",%nobits
	.set	.LANCHOR111,. + 0
	.type	hynix20nm_lsb_mode_default_val, %object
	.size	hynix20nm_lsb_mode_default_val, 4
hynix20nm_lsb_mode_default_val:
	.space	4
	.section	.bss.hynix20nm_lsb_mode_reg_adr,"aw",%nobits
	.set	.LANCHOR109,. + 0
	.type	hynix20nm_lsb_mode_reg_adr, %object
	.size	hynix20nm_lsb_mode_reg_adr, 4
hynix20nm_lsb_mode_reg_adr:
	.space	4
	.section	.bss.hynix20nm_lsb_mode_reg_num,"aw",%nobits
	.set	.LANCHOR108,. + 0
	.type	hynix20nm_lsb_mode_reg_num, %object
	.size	hynix20nm_lsb_mode_reg_num, 1
hynix20nm_lsb_mode_reg_num:
	.space	1
	.section	.bss.hynix20nm_lsb_mode_val,"aw",%nobits
	.set	.LANCHOR110,. + 0
	.type	hynix20nm_lsb_mode_val, %object
	.size	hynix20nm_lsb_mode_val, 4
hynix20nm_lsb_mode_val:
	.space	4
	.section	.bss.hynix20nm_read_retry_cycle,"aw",%nobits
	.set	.LANCHOR104,. + 0
	.type	hynix20nm_read_retry_cycle, %object
	.size	hynix20nm_read_retry_cycle, 1
hynix20nm_read_retry_cycle:
	.space	1
	.section	.bss.hynix20nm_read_retry_mode,"aw",%nobits
	.set	.LANCHOR107,. + 0
	.type	hynix20nm_read_retry_mode, %object
	.size	hynix20nm_read_retry_mode, 1
hynix20nm_read_retry_mode:
	.space	1
	.section	.bss.hynix20nm_read_retry_reg_adr,"aw",%nobits
	.set	.LANCHOR106,. + 0
	.type	hynix20nm_read_retry_reg_adr, %object
	.size	hynix20nm_read_retry_reg_adr, 8
hynix20nm_read_retry_reg_adr:
	.space	8
	.section	.bss.hynix20nm_read_retry_reg_num,"aw",%nobits
	.set	.LANCHOR105,. + 0
	.type	hynix20nm_read_retry_reg_num, %object
	.size	hynix20nm_read_retry_reg_num, 1
hynix20nm_read_retry_reg_num:
	.space	1
	.section	.bss.hynix26nm_lsb_mode_default_val,"aw",%nobits
	.set	.LANCHOR47,. + 0
	.type	hynix26nm_lsb_mode_default_val, %object
	.size	hynix26nm_lsb_mode_default_val, 5
hynix26nm_lsb_mode_default_val:
	.space	5
	.section	.bss.hynix26nm_lsb_mode_reg_adr,"aw",%nobits
	.set	.LANCHOR45,. + 0
	.type	hynix26nm_lsb_mode_reg_adr, %object
	.size	hynix26nm_lsb_mode_reg_adr, 5
hynix26nm_lsb_mode_reg_adr:
	.space	5
	.section	.bss.hynix26nm_lsb_mode_reg_num,"aw",%nobits
	.set	.LANCHOR44,. + 0
	.type	hynix26nm_lsb_mode_reg_num, %object
	.size	hynix26nm_lsb_mode_reg_num, 1
hynix26nm_lsb_mode_reg_num:
	.space	1
	.section	.bss.hynix26nm_lsb_mode_val,"aw",%nobits
	.set	.LANCHOR46,. + 0
	.type	hynix26nm_lsb_mode_val, %object
	.size	hynix26nm_lsb_mode_val, 5
hynix26nm_lsb_mode_val:
	.space	5
	.section	.bss.hynix26nm_read_retry_cycle,"aw",%nobits
	.set	.LANCHOR39,. + 0
	.type	hynix26nm_read_retry_cycle, %object
	.size	hynix26nm_read_retry_cycle, 1
hynix26nm_read_retry_cycle:
	.space	1
	.section	.bss.hynix26nm_read_retry_mode,"aw",%nobits
	.set	.LANCHOR43,. + 0
	.type	hynix26nm_read_retry_mode, %object
	.size	hynix26nm_read_retry_mode, 1
hynix26nm_read_retry_mode:
	.space	1
	.section	.bss.hynix26nm_read_retry_reg_adr,"aw",%nobits
	.set	.LANCHOR42,. + 0
	.type	hynix26nm_read_retry_reg_adr, %object
	.size	hynix26nm_read_retry_reg_adr, 4
hynix26nm_read_retry_reg_adr:
	.space	4
	.section	.bss.hynix26nm_read_retry_reg_num,"aw",%nobits
	.set	.LANCHOR40,. + 0
	.type	hynix26nm_read_retry_reg_num, %object
	.size	hynix26nm_read_retry_reg_num, 1
hynix26nm_read_retry_reg_num:
	.space	1
	.section	.bss.hynix26nm_read_retry_val,"aw",%nobits
	.align	1
	.set	.LANCHOR41,. + 0
	.type	hynix26nm_read_retry_val, %object
	.size	hynix26nm_read_retry_val, 56
hynix26nm_read_retry_val:
	.space	56
	.section	.bss.micron_p1,"aw",%nobits
	.align	2
	.set	.LANCHOR80,. + 0
	.type	micron_p1, %object
	.size	micron_p1, 4
micron_p1:
	.space	4
	.section	.bss.micron_read_retry_cycle,"aw",%nobits
	.set	.LANCHOR81,. + 0
	.type	micron_read_retry_cycle, %object
	.size	micron_read_retry_cycle, 1
micron_read_retry_cycle:
	.space	1
	.section	.bss.micron_read_retry_mode,"aw",%nobits
	.set	.LANCHOR79,. + 0
	.type	micron_read_retry_mode, %object
	.size	micron_read_retry_mode, 1
micron_read_retry_mode:
	.space	1
	.section	.bss.nand_on_panic,"aw",%nobits
	.align	2
	.set	.LANCHOR20,. + 0
	.type	nand_on_panic, %object
	.size	nand_on_panic, 4
nand_on_panic:
	.space	4
	.section	.bss.nftl_blk_head,"aw",%nobits
	.align	2
	.set	.LANCHOR0,. + 0
	.type	nftl_blk_head, %object
	.size	nftl_blk_head, 76
nftl_blk_head:
	.space	76
	.section	.bss.panic_enable,"aw",%nobits
	.align	2
	.set	.LANCHOR21,. + 0
	.type	panic_enable, %object
	.size	panic_enable, 4
panic_enable:
	.space	4
	.section	.bss.panic_info,"aw",%nobits
	.align	2
	.set	.LANCHOR23,. + 0
	.type	panic_info, %object
	.size	panic_info, 24
panic_info:
	.space	24
	.section	.bss.panic_part,"aw",%nobits
	.align	2
	.set	.LANCHOR22,. + 0
	.type	panic_part, %object
	.size	panic_part, 12
panic_part:
	.space	12
	.section	.bss.phyinfo,"aw",%nobits
	.align	2
	.set	.LANCHOR18,. + 0
	.type	phyinfo, %object
	.size	phyinfo, 20
phyinfo:
	.space	20
	.section	.bss.samsung_read_retry_cycle,"aw",%nobits
	.set	.LANCHOR37,. + 0
	.type	samsung_read_retry_cycle, %object
	.size	samsung_read_retry_cycle, 1
samsung_read_retry_cycle:
	.space	1
	.section	.bss.samsung_read_retry_mode,"aw",%nobits
	.set	.LANCHOR38,. + 0
	.type	samsung_read_retry_mode, %object
	.size	samsung_read_retry_mode, 1
samsung_read_retry_mode:
	.space	1
	.section	.bss.sandisk_A19_read_retry_cycle,"aw",%nobits
	.set	.LANCHOR51,. + 0
	.type	sandisk_A19_read_retry_cycle, %object
	.size	sandisk_A19_read_retry_cycle, 1
sandisk_A19_read_retry_cycle:
	.space	1
	.section	.bss.sandisk_A19_read_retry_mode,"aw",%nobits
	.set	.LANCHOR50,. + 0
	.type	sandisk_A19_read_retry_mode, %object
	.size	sandisk_A19_read_retry_mode, 1
sandisk_A19_read_retry_mode:
	.space	1
	.section	.bss.sandisk_A19_read_retry_reg_cnt,"aw",%nobits
	.set	.LANCHOR52,. + 0
	.type	sandisk_A19_read_retry_reg_cnt, %object
	.size	sandisk_A19_read_retry_reg_cnt, 1
sandisk_A19_read_retry_reg_cnt:
	.space	1
	.section	.bss.sandisk_A19_sclk0_bak,"aw",%nobits
	.align	2
	.set	.LANCHOR54,. + 0
	.type	sandisk_A19_sclk0_bak, %object
	.size	sandisk_A19_sclk0_bak, 4
sandisk_A19_sclk0_bak:
	.space	4
	.section	.bss.sandisk_A19_sclk1_bak,"aw",%nobits
	.align	2
	.set	.LANCHOR53,. + 0
	.type	sandisk_A19_sclk1_bak, %object
	.size	sandisk_A19_sclk1_bak, 4
sandisk_A19_sclk1_bak:
	.space	4
	.section	.bss.sandisk_read_retry_cycle,"aw",%nobits
	.set	.LANCHOR95,. + 0
	.type	sandisk_read_retry_cycle, %object
	.size	sandisk_read_retry_cycle, 1
sandisk_read_retry_cycle:
	.space	1
	.section	.bss.sandisk_read_retry_mode,"aw",%nobits
	.set	.LANCHOR88,. + 0
	.type	sandisk_read_retry_mode, %object
	.size	sandisk_read_retry_mode, 1
sandisk_read_retry_mode:
	.space	1
	.section	.bss.sandisk_read_retry_reg_cnt,"aw",%nobits
	.set	.LANCHOR103,. + 0
	.type	sandisk_read_retry_reg_cnt, %object
	.size	sandisk_read_retry_reg_cnt, 1
sandisk_read_retry_reg_cnt:
	.space	1
	.section	.bss.sandisk_sclk0_bak,"aw",%nobits
	.align	2
	.set	.LANCHOR87,. + 0
	.type	sandisk_sclk0_bak, %object
	.size	sandisk_sclk0_bak, 4
sandisk_sclk0_bak:
	.space	4
	.section	.bss.sandisk_sclk1_bak,"aw",%nobits
	.align	2
	.set	.LANCHOR86,. + 0
	.type	sandisk_sclk1_bak, %object
	.size	sandisk_sclk1_bak, 4
sandisk_sclk1_bak:
	.space	4
	.section	.bss.sandisk_toggle_mode_flag,"aw",%nobits
	.align	2
	.set	.LANCHOR92,. + 0
	.type	sandisk_toggle_mode_flag, %object
	.size	sandisk_toggle_mode_flag, 4
sandisk_toggle_mode_flag:
	.space	4
	.section	.bss.toshiba_read_retry_cycle,"aw",%nobits
	.set	.LANCHOR77,. + 0
	.type	toshiba_read_retry_cycle, %object
	.size	toshiba_read_retry_cycle, 1
toshiba_read_retry_cycle:
	.space	1
	.section	.bss.toshiba_read_retry_mode,"aw",%nobits
	.set	.LANCHOR63,. + 0
	.type	toshiba_read_retry_mode, %object
	.size	toshiba_read_retry_mode, 1
toshiba_read_retry_mode:
	.space	1
	.section	.bss.toshiba_read_retry_reg_cnt,"aw",%nobits
	.set	.LANCHOR78,. + 0
	.type	toshiba_read_retry_reg_cnt, %object
	.size	toshiba_read_retry_reg_cnt, 1
toshiba_read_retry_reg_cnt:
	.space	1
	.section	.bss.toshiba_retry_flag,"aw",%nobits
	.align	2
	.set	.LANCHOR65,. + 0
	.type	toshiba_retry_flag, %object
	.size	toshiba_retry_flag, 4
toshiba_retry_flag:
	.space	4
	.section	.bss.toshiba_sclk0_bak,"aw",%nobits
	.align	2
	.set	.LANCHOR62,. + 0
	.type	toshiba_sclk0_bak, %object
	.size	toshiba_sclk0_bak, 4
toshiba_sclk0_bak:
	.space	4
	.section	.bss.toshiba_sclk1_bak,"aw",%nobits
	.align	2
	.set	.LANCHOR61,. + 0
	.type	toshiba_sclk1_bak, %object
	.size	toshiba_sclk1_bak, 4
toshiba_sclk1_bak:
	.space	4
	.section	.bss.toshiba_toggle_mode_flag,"aw",%nobits
	.align	2
	.set	.LANCHOR73,. + 0
	.type	toshiba_toggle_mode_flag, %object
	.size	toshiba_toggle_mode_flag, 4
toshiba_toggle_mode_flag:
	.space	4
	.section	.data.hynix20nm_read_retry_reg_adr_1,"aw",%progbits
	.type	hynix20nm_read_retry_reg_adr_1, %object
	.size	hynix20nm_read_retry_reg_adr_1, 8
hynix20nm_read_retry_reg_adr_1:
	.byte	-52
	.byte	-65
	.byte	-86
	.byte	-85
	.byte	-51
	.byte	-83
	.byte	-82
	.byte	-81
	.section	.data.hynix20nm_read_retry_reg_adr_2,"aw",%progbits
	.type	hynix20nm_read_retry_reg_adr_2, %object
	.size	hynix20nm_read_retry_reg_adr_2, 8
hynix20nm_read_retry_reg_adr_2:
	.byte	-80
	.byte	-79
	.byte	-78
	.byte	-77
	.byte	-76
	.byte	-75
	.byte	-74
	.byte	-73
	.section	.data.micron_p1_1,"aw",%progbits
	.set	.LANCHOR83,. + 0
	.type	micron_p1_1, %object
	.size	micron_p1_1, 10
micron_p1_1:
	.byte	0
	.byte	0
	.byte	0
	.byte	1
	.byte	2
	.byte	3
	.byte	4
	.byte	5
	.byte	6
	.byte	7
	.section	.data.micron_p1_2,"aw",%progbits
	.set	.LANCHOR82,. + 0
	.type	micron_p1_2, %object
	.size	micron_p1_2, 12
micron_p1_2:
	.byte	0
	.byte	0
	.byte	0
	.byte	1
	.byte	2
	.byte	3
	.byte	4
	.byte	5
	.byte	6
	.byte	7
	.byte	8
	.byte	12
	.section	.data.micron_p1_3,"aw",%progbits
	.set	.LANCHOR84,. + 0
	.type	micron_p1_3, %object
	.size	micron_p1_3, 7
micron_p1_3:
	.byte	1
	.byte	2
	.byte	3
	.byte	0
	.byte	1
	.byte	2
	.byte	3
	.section	.data.micron_p1_4,"aw",%progbits
	.set	.LANCHOR85,. + 0
	.type	micron_p1_4, %object
	.size	micron_p1_4, 18
micron_p1_4:
	.byte	0
	.byte	0
	.byte	0
	.byte	1
	.byte	2
	.byte	3
	.byte	4
	.byte	5
	.byte	6
	.byte	7
	.byte	8
	.byte	9
	.byte	10
	.byte	11
	.byte	12
	.byte	13
	.byte	14
	.byte	15
	.section	.data.samsung_p1,"aw",%progbits
	.set	.LANCHOR36,. + 0
	.type	samsung_p1, %object
	.size	samsung_p1, 60
samsung_p1:
	.byte	0
	.byte	0
	.byte	0
	.byte	0
	.byte	5
	.byte	10
	.byte	0
	.byte	0
	.byte	40
	.byte	0
	.byte	-20
	.byte	-40
	.byte	-19
	.byte	-11
	.byte	-19
	.byte	-26
	.byte	10
	.byte	15
	.byte	5
	.byte	0
	.byte	15
	.byte	10
	.byte	-5
	.byte	-20
	.byte	-24
	.byte	-17
	.byte	-24
	.byte	-36
	.byte	-15
	.byte	-5
	.byte	-2
	.byte	-16
	.byte	10
	.byte	0
	.byte	-5
	.byte	-20
	.byte	-48
	.byte	-30
	.byte	-48
	.byte	-62
	.byte	20
	.byte	15
	.byte	-5
	.byte	-20
	.byte	-24
	.byte	-5
	.byte	-24
	.byte	-36
	.byte	30
	.byte	20
	.byte	-5
	.byte	-20
	.byte	-5
	.byte	-1
	.byte	-5
	.byte	-8
	.byte	7
	.byte	12
	.byte	2
	.byte	0
	.section	.data.samsung_read_retry_addr,"aw",%progbits
	.set	.LANCHOR35,. + 0
	.type	samsung_read_retry_addr, %object
	.size	samsung_read_retry_addr, 4
samsung_read_retry_addr:
	.byte	-89
	.byte	-92
	.byte	-91
	.byte	-90
	.section	.data.samsung_read_retry_cmd,"aw",%progbits
	.set	.LANCHOR34,. + 0
	.type	samsung_read_retry_cmd, %object
	.size	samsung_read_retry_cmd, 4
samsung_read_retry_cmd:
	.byte	-95
	.byte	-95
	.byte	-95
	.byte	-95
	.section	.data.sandisk_19_high,"aw",%progbits
	.set	.LANCHOR96,. + 0
	.type	sandisk_19_high, %object
	.size	sandisk_19_high, 60
sandisk_19_high:
	.byte	0
	.byte	-16
	.byte	0
	.byte	15
	.byte	-32
	.byte	0
	.byte	15
	.byte	-48
	.byte	0
	.byte	14
	.byte	-32
	.byte	0
	.byte	14
	.byte	-48
	.byte	0
	.byte	13
	.byte	-16
	.byte	0
	.byte	13
	.byte	-32
	.byte	0
	.byte	13
	.byte	-48
	.byte	0
	.byte	1
	.byte	16
	.byte	0
	.byte	2
	.byte	32
	.byte	0
	.byte	2
	.byte	16
	.byte	0
	.byte	3
	.byte	32
	.byte	0
	.byte	15
	.byte	0
	.byte	0
	.byte	14
	.byte	-16
	.byte	0
	.byte	13
	.byte	-64
	.byte	0
	.byte	15
	.byte	-16
	.byte	0
	.byte	1
	.byte	0
	.byte	0
	.byte	2
	.byte	0
	.byte	0
	.byte	13
	.byte	-80
	.byte	0
	.byte	12
	.byte	-96
	.byte	0
	.section	.data.sandisk_19_low,"aw",%progbits
	.set	.LANCHOR94,. + 0
	.type	sandisk_19_low, %object
	.size	sandisk_19_low, 48
sandisk_19_low:
	.byte	-16
	.byte	0
	.byte	-16
	.byte	-32
	.byte	0
	.byte	-32
	.byte	-48
	.byte	0
	.byte	-48
	.byte	16
	.byte	0
	.byte	16
	.byte	32
	.byte	0
	.byte	32
	.byte	48
	.byte	0
	.byte	48
	.byte	-64
	.byte	0
	.byte	-48
	.byte	0
	.byte	0
	.byte	16
	.byte	0
	.byte	0
	.byte	32
	.byte	16
	.byte	0
	.byte	32
	.byte	-80
	.byte	0
	.byte	-48
	.byte	-96
	.byte	0
	.byte	-48
	.byte	-112
	.byte	0
	.byte	-48
	.byte	-80
	.byte	0
	.byte	-64
	.byte	-96
	.byte	0
	.byte	-64
	.byte	-112
	.byte	0
	.byte	-64
	.section	.data.sandisk_1y,"aw",%progbits
	.set	.LANCHOR101,. + 0
	.type	sandisk_1y, %object
	.size	sandisk_1y, 76
sandisk_1y:
	.byte	4
	.byte	0
	.byte	124
	.byte	124
	.byte	8
	.byte	0
	.byte	4
	.byte	4
	.byte	124
	.byte	124
	.byte	0
	.byte	124
	.byte	124
	.byte	124
	.byte	0
	.byte	0
	.byte	12
	.byte	0
	.byte	120
	.byte	120
	.byte	16
	.byte	0
	.byte	8
	.byte	8
	.byte	124
	.byte	124
	.byte	120
	.byte	120
	.byte	124
	.byte	124
	.byte	4
	.byte	4
	.byte	124
	.byte	124
	.byte	120
	.byte	116
	.byte	20
	.byte	4
	.byte	4
	.byte	0
	.byte	120
	.byte	120
	.byte	0
	.byte	124
	.byte	120
	.byte	120
	.byte	120
	.byte	120
	.byte	120
	.byte	120
	.byte	120
	.byte	116
	.byte	120
	.byte	120
	.byte	4
	.byte	0
	.byte	120
	.byte	116
	.byte	120
	.byte	116
	.byte	120
	.byte	116
	.byte	116
	.byte	112
	.byte	120
	.byte	116
	.byte	120
	.byte	120
	.byte	120
	.byte	112
	.byte	120
	.byte	116
	.byte	120
	.byte	112
	.byte	112
	.byte	112
	.section	.data.sandisk_24,"aw",%progbits
	.set	.LANCHOR99,. + 0
	.type	sandisk_24, %object
	.size	sandisk_24, 27
sandisk_24:
	.byte	0
	.byte	-16
	.byte	0
	.byte	0
	.byte	-32
	.byte	0
	.byte	-1
	.byte	-16
	.byte	-16
	.byte	-18
	.byte	-32
	.byte	-32
	.byte	-34
	.byte	-48
	.byte	-48
	.byte	-51
	.byte	-64
	.byte	-64
	.byte	1
	.byte	0
	.byte	0
	.byte	2
	.byte	0
	.byte	0
	.byte	3
	.byte	0
	.byte	0
	.section	.data.sandisk_A19_1y,"aw",%progbits
	.set	.LANCHOR58,. + 0
	.type	sandisk_A19_1y, %object
	.size	sandisk_A19_1y, 124
sandisk_A19_1y:
	.byte	124
	.byte	0
	.byte	0
	.byte	124
	.byte	4
	.byte	0
	.byte	124
	.byte	120
	.byte	120
	.byte	0
	.byte	120
	.byte	116
	.byte	8
	.byte	124
	.byte	0
	.byte	124
	.byte	0
	.byte	124
	.byte	124
	.byte	120
	.byte	124
	.byte	124
	.byte	120
	.byte	116
	.byte	0
	.byte	124
	.byte	116
	.byte	112
	.byte	0
	.byte	120
	.byte	0
	.byte	124
	.byte	0
	.byte	120
	.byte	124
	.byte	120
	.byte	0
	.byte	120
	.byte	120
	.byte	116
	.byte	0
	.byte	120
	.byte	116
	.byte	112
	.byte	0
	.byte	120
	.byte	112
	.byte	108
	.byte	0
	.byte	4
	.byte	4
	.byte	0
	.byte	0
	.byte	4
	.byte	0
	.byte	124
	.byte	12
	.byte	4
	.byte	124
	.byte	120
	.byte	12
	.byte	4
	.byte	120
	.byte	116
	.byte	16
	.byte	8
	.byte	0
	.byte	124
	.byte	16
	.byte	8
	.byte	4
	.byte	0
	.byte	120
	.byte	116
	.byte	120
	.byte	116
	.byte	120
	.byte	116
	.byte	116
	.byte	112
	.byte	120
	.byte	116
	.byte	112
	.byte	108
	.byte	120
	.byte	116
	.byte	108
	.byte	104
	.byte	120
	.byte	112
	.byte	120
	.byte	116
	.byte	120
	.byte	112
	.byte	116
	.byte	112
	.byte	120
	.byte	112
	.byte	108
	.byte	104
	.byte	120
	.byte	112
	.byte	112
	.byte	108
	.byte	120
	.byte	108
	.byte	112
	.byte	108
	.byte	120
	.byte	108
	.byte	108
	.byte	104
	.byte	120
	.byte	108
	.byte	104
	.byte	100
	.byte	116
	.byte	104
	.byte	108
	.byte	104
	.byte	116
	.byte	104
	.byte	104
	.byte	100
	.section	.data.sandisk_A19_1z_16g,"aw",%progbits
	.set	.LANCHOR56,. + 0
	.type	sandisk_A19_1z_16g, %object
	.size	sandisk_A19_1z_16g, 128
sandisk_A19_1z_16g:
	.byte	124
	.byte	0
	.byte	4
	.byte	0
	.byte	4
	.byte	0
	.byte	0
	.byte	124
	.byte	120
	.byte	0
	.byte	8
	.byte	4
	.byte	8
	.byte	0
	.byte	124
	.byte	120
	.byte	124
	.byte	124
	.byte	4
	.byte	0
	.byte	0
	.byte	124
	.byte	0
	.byte	124
	.byte	120
	.byte	124
	.byte	8
	.byte	4
	.byte	4
	.byte	124
	.byte	116
	.byte	120
	.byte	4
	.byte	4
	.byte	4
	.byte	0
	.byte	8
	.byte	4
	.byte	0
	.byte	124
	.byte	0
	.byte	4
	.byte	8
	.byte	4
	.byte	12
	.byte	4
	.byte	124
	.byte	120
	.byte	124
	.byte	4
	.byte	12
	.byte	8
	.byte	120
	.byte	120
	.byte	4
	.byte	0
	.byte	124
	.byte	120
	.byte	0
	.byte	124
	.byte	116
	.byte	120
	.byte	112
	.byte	120
	.byte	8
	.byte	8
	.byte	4
	.byte	0
	.byte	4
	.byte	8
	.byte	8
	.byte	4
	.byte	12
	.byte	8
	.byte	12
	.byte	8
	.byte	0
	.byte	0
	.byte	12
	.byte	8
	.byte	4
	.byte	0
	.byte	120
	.byte	116
	.byte	124
	.byte	124
	.byte	120
	.byte	116
	.byte	120
	.byte	120
	.byte	120
	.byte	116
	.byte	8
	.byte	8
	.byte	0
	.byte	124
	.byte	116
	.byte	116
	.byte	0
	.byte	124
	.byte	120
	.byte	116
	.byte	124
	.byte	120
	.byte	112
	.byte	116
	.byte	116
	.byte	116
	.byte	124
	.byte	116
	.byte	112
	.byte	112
	.byte	12
	.byte	12
	.byte	8
	.byte	4
	.byte	16
	.byte	12
	.byte	12
	.byte	8
	.byte	112
	.byte	112
	.byte	112
	.byte	116
	.byte	116
	.byte	112
	.byte	112
	.byte	112
	.section	.data.sandisk_A19_1z_8g,"aw",%progbits
	.set	.LANCHOR57,. + 0
	.type	sandisk_A19_1z_8g, %object
	.size	sandisk_A19_1z_8g, 124
sandisk_A19_1z_8g:
	.byte	4
	.byte	4
	.byte	120
	.byte	120
	.byte	4
	.byte	4
	.byte	124
	.byte	116
	.byte	0
	.byte	0
	.byte	4
	.byte	120
	.byte	4
	.byte	4
	.byte	124
	.byte	124
	.byte	0
	.byte	0
	.byte	0
	.byte	124
	.byte	0
	.byte	0
	.byte	0
	.byte	116
	.byte	8
	.byte	8
	.byte	4
	.byte	120
	.byte	124
	.byte	124
	.byte	4
	.byte	120
	.byte	124
	.byte	124
	.byte	0
	.byte	124
	.byte	4
	.byte	4
	.byte	124
	.byte	112
	.byte	124
	.byte	124
	.byte	116
	.byte	116
	.byte	0
	.byte	0
	.byte	120
	.byte	112
	.byte	12
	.byte	12
	.byte	8
	.byte	120
	.byte	120
	.byte	120
	.byte	124
	.byte	124
	.byte	4
	.byte	4
	.byte	8
	.byte	4
	.byte	120
	.byte	120
	.byte	8
	.byte	120
	.byte	124
	.byte	124
	.byte	120
	.byte	112
	.byte	120
	.byte	120
	.byte	112
	.byte	108
	.byte	0
	.byte	0
	.byte	116
	.byte	108
	.byte	8
	.byte	8
	.byte	0
	.byte	116
	.byte	124
	.byte	124
	.byte	120
	.byte	108
	.byte	0
	.byte	0
	.byte	4
	.byte	4
	.byte	116
	.byte	116
	.byte	116
	.byte	108
	.byte	120
	.byte	120
	.byte	124
	.byte	112
	.byte	12
	.byte	12
	.byte	0
	.byte	116
	.byte	4
	.byte	4
	.byte	12
	.byte	8
	.byte	120
	.byte	120
	.byte	124
	.byte	116
	.byte	120
	.byte	120
	.byte	112
	.byte	104
	.byte	8
	.byte	8
	.byte	0
	.byte	112
	.byte	16
	.byte	16
	.byte	12
	.byte	120
	.byte	0
	.byte	0
	.byte	12
	.byte	8
	.section	.data.sandisk_A19_read_retry_cmd_1y,"aw",%progbits
	.set	.LANCHOR55,. + 0
	.type	sandisk_A19_read_retry_cmd_1y, %object
	.size	sandisk_A19_read_retry_cmd_1y, 1
sandisk_A19_read_retry_cmd_1y:
	.byte	-17
	.section	.data.sandisk_A19_read_retry_enable_cmd_1y,"aw",%progbits
	.type	sandisk_A19_read_retry_enable_cmd_1y, %object
	.size	sandisk_A19_read_retry_enable_cmd_1y, 1
sandisk_A19_read_retry_enable_cmd_1y:
	.byte	93
	.section	.data.sandisk_A19_read_retry_exit_cmd_1y,"aw",%progbits
	.type	sandisk_A19_read_retry_exit_cmd_1y, %object
	.size	sandisk_A19_read_retry_exit_cmd_1y, 1
sandisk_A19_read_retry_exit_cmd_1y:
	.byte	-1
	.section	.data.sandisk_A19_read_retry_reg_adr_1y,"aw",%progbits
	.set	.LANCHOR59,. + 0
	.type	sandisk_A19_read_retry_reg_adr_1y, %object
	.size	sandisk_A19_read_retry_reg_adr_1y, 1
sandisk_A19_read_retry_reg_adr_1y:
	.byte	17
	.section	.data.sandisk_acti_start_addr,"aw",%progbits
	.set	.LANCHOR102,. + 0
	.type	sandisk_acti_start_addr, %object
	.size	sandisk_acti_start_addr, 9
sandisk_acti_start_addr:
	.byte	4
	.byte	5
	.byte	6
	.byte	7
	.byte	8
	.byte	9
	.byte	10
	.byte	11
	.byte	12
	.section	.data.sandisk_acti_start_cmd,"aw",%progbits
	.set	.LANCHOR89,. + 0
	.type	sandisk_acti_start_cmd, %object
	.size	sandisk_acti_start_cmd, 2
sandisk_acti_start_cmd:
	.byte	59
	.byte	-71
	.section	.data.sandisk_read_retry_cmd_19,"aw",%progbits
	.set	.LANCHOR90,. + 0
	.type	sandisk_read_retry_cmd_19, %object
	.size	sandisk_read_retry_cmd_19, 3
sandisk_read_retry_cmd_19:
	.byte	83
	.byte	83
	.byte	83
	.section	.data.sandisk_read_retry_cmd_1y,"aw",%progbits
	.set	.LANCHOR91,. + 0
	.type	sandisk_read_retry_cmd_1y, %object
	.size	sandisk_read_retry_cmd_1y, 1
sandisk_read_retry_cmd_1y:
	.byte	-17
	.section	.data.sandisk_read_retry_cmd_24,"aw",%progbits
	.type	sandisk_read_retry_cmd_24, %object
	.size	sandisk_read_retry_cmd_24, 3
sandisk_read_retry_cmd_24:
	.byte	83
	.byte	83
	.byte	83
	.section	.data.sandisk_read_retry_disable_cmd_19,"aw",%progbits
	.set	.LANCHOR93,. + 0
	.type	sandisk_read_retry_disable_cmd_19, %object
	.size	sandisk_read_retry_disable_cmd_19, 1
sandisk_read_retry_disable_cmd_19:
	.byte	-42
	.section	.data.sandisk_read_retry_enable_cmd_19,"aw",%progbits
	.type	sandisk_read_retry_enable_cmd_19, %object
	.size	sandisk_read_retry_enable_cmd_19, 1
sandisk_read_retry_enable_cmd_19:
	.byte	-74
	.section	.data.sandisk_read_retry_enable_cmd_1y,"aw",%progbits
	.type	sandisk_read_retry_enable_cmd_1y, %object
	.size	sandisk_read_retry_enable_cmd_1y, 1
sandisk_read_retry_enable_cmd_1y:
	.byte	93
	.section	.data.sandisk_read_retry_exit_cmd_1y,"aw",%progbits
	.type	sandisk_read_retry_exit_cmd_1y, %object
	.size	sandisk_read_retry_exit_cmd_1y, 1
sandisk_read_retry_exit_cmd_1y:
	.byte	-1
	.section	.data.sandisk_read_retry_reg_adr_19,"aw",%progbits
	.set	.LANCHOR97,. + 0
	.type	sandisk_read_retry_reg_adr_19, %object
	.size	sandisk_read_retry_reg_adr_19, 9
sandisk_read_retry_reg_adr_19:
	.byte	4
	.byte	5
	.byte	7
	.space	6
	.section	.data.sandisk_read_retry_reg_adr_1y,"aw",%progbits
	.set	.LANCHOR100,. + 0
	.type	sandisk_read_retry_reg_adr_1y, %object
	.size	sandisk_read_retry_reg_adr_1y, 1
sandisk_read_retry_reg_adr_1y:
	.byte	17
	.section	.data.sandisk_read_retry_reg_adr_24,"aw",%progbits
	.set	.LANCHOR98,. + 0
	.type	sandisk_read_retry_reg_adr_24, %object
	.size	sandisk_read_retry_reg_adr_24, 9
sandisk_read_retry_reg_adr_24:
	.byte	4
	.byte	5
	.byte	7
	.space	6
	.section	.data.toshiba_p1_1,"aw",%progbits
	.set	.LANCHOR76,. + 0
	.type	toshiba_p1_1, %object
	.size	toshiba_p1_1, 28
toshiba_p1_1:
	.byte	0
	.byte	0
	.byte	0
	.byte	0
	.byte	4
	.byte	4
	.byte	4
	.byte	4
	.byte	124
	.byte	124
	.byte	124
	.byte	124
	.byte	120
	.byte	120
	.byte	120
	.byte	120
	.byte	116
	.byte	116
	.byte	116
	.byte	116
	.byte	8
	.byte	8
	.byte	8
	.byte	8
	.byte	-1
	.byte	-1
	.byte	-1
	.byte	-1
	.section	.data.toshiba_p1_2,"aw",%progbits
	.set	.LANCHOR74,. + 0
	.type	toshiba_p1_2, %object
	.size	toshiba_p1_2, 40
toshiba_p1_2:
	.byte	4
	.byte	4
	.byte	124
	.byte	126
	.byte	0
	.byte	0
	.byte	124
	.byte	120
	.byte	120
	.byte	0
	.byte	124
	.byte	118
	.byte	116
	.byte	114
	.byte	0
	.byte	8
	.byte	8
	.byte	0
	.byte	0
	.byte	0
	.byte	11
	.byte	126
	.byte	118
	.byte	116
	.byte	0
	.byte	16
	.byte	118
	.byte	114
	.byte	112
	.byte	0
	.byte	2
	.byte	0
	.byte	126
	.byte	124
	.byte	0
	.byte	0
	.byte	0
	.byte	0
	.byte	0
	.byte	0
	.section	.data.toshiba_p1_3,"aw",%progbits
	.set	.LANCHOR75,. + 0
	.type	toshiba_p1_3, %object
	.size	toshiba_p1_3, 55
toshiba_p1_3:
	.byte	0
	.byte	0
	.byte	0
	.byte	0
	.byte	0
	.byte	2
	.byte	4
	.byte	2
	.byte	0
	.byte	0
	.byte	124
	.byte	0
	.byte	124
	.byte	124
	.byte	0
	.byte	122
	.byte	0
	.byte	122
	.byte	122
	.byte	0
	.byte	120
	.byte	2
	.byte	120
	.byte	122
	.byte	0
	.byte	126
	.byte	4
	.byte	126
	.byte	122
	.byte	0
	.byte	118
	.byte	4
	.byte	118
	.byte	120
	.byte	0
	.byte	4
	.byte	4
	.byte	4
	.byte	118
	.byte	0
	.byte	6
	.byte	10
	.byte	6
	.byte	2
	.byte	0
	.byte	116
	.byte	124
	.byte	116
	.byte	118
	.byte	0
	.byte	0
	.byte	0
	.byte	0
	.byte	0
	.byte	0
	.section	.data.toshiba_read_retry_addr_1,"aw",%progbits
	.set	.LANCHOR72,. + 0
	.type	toshiba_read_retry_addr_1, %object
	.size	toshiba_read_retry_addr_1, 4
toshiba_read_retry_addr_1:
	.byte	4
	.byte	5
	.byte	6
	.byte	7
	.section	.data.toshiba_read_retry_addr_2,"aw",%progbits
	.set	.LANCHOR71,. + 0
	.type	toshiba_read_retry_addr_2, %object
	.size	toshiba_read_retry_addr_2, 5
toshiba_read_retry_addr_2:
	.byte	4
	.byte	5
	.byte	6
	.byte	7
	.byte	13
	.section	.data.toshiba_read_retry_cmd_1,"aw",%progbits
	.set	.LANCHOR68,. + 0
	.type	toshiba_read_retry_cmd_1, %object
	.size	toshiba_read_retry_cmd_1, 4
toshiba_read_retry_cmd_1:
	.byte	85
	.byte	85
	.byte	85
	.byte	85
	.section	.data.toshiba_read_retry_cmd_2,"aw",%progbits
	.set	.LANCHOR64,. + 0
	.type	toshiba_read_retry_cmd_2, %object
	.size	toshiba_read_retry_cmd_2, 5
toshiba_read_retry_cmd_2:
	.byte	85
	.byte	85
	.byte	85
	.byte	85
	.byte	85
	.section	.data.toshiba_read_retry_end_cmd,"aw",%progbits
	.set	.LANCHOR69,. + 0
	.type	toshiba_read_retry_end_cmd, %object
	.size	toshiba_read_retry_end_cmd, 2
toshiba_read_retry_end_cmd:
	.byte	38
	.byte	93
	.section	.data.toshiba_read_retry_exit_cmd,"aw",%progbits
	.set	.LANCHOR66,. + 0
	.type	toshiba_read_retry_exit_cmd, %object
	.size	toshiba_read_retry_exit_cmd, 1
toshiba_read_retry_exit_cmd:
	.byte	-1
	.section	.data.toshiba_read_retry_last_cmd,"aw",%progbits
	.set	.LANCHOR67,. + 0
	.type	toshiba_read_retry_last_cmd, %object
	.size	toshiba_read_retry_last_cmd, 1
toshiba_read_retry_last_cmd:
	.byte	-77
	.section	.data.toshiba_read_retry_start_cmd,"aw",%progbits
	.set	.LANCHOR70,. + 0
	.type	toshiba_read_retry_start_cmd, %object
	.size	toshiba_read_retry_start_cmd, 2
toshiba_read_retry_start_cmd:
	.byte	92
	.byte	-59
	.section	.rodata.__FUNCTION__.7025,"a",%progbits
	.set	.LANCHOR13,. + 0
	.type	__FUNCTION__.7025, %object
	.size	__FUNCTION__.7025, 25
__FUNCTION__.7025:
	.ascii	"nand_power_off_ecc_error\000"
	.section	.rodata.__func__.5931,"a",%progbits
	.set	.LANCHOR27,. + 0
	.type	__func__.5931, %object
	.size	__func__.5931, 16
__func__.5931:
	.ascii	"nftl_initialize\000"
	.section	.rodata.__func__.6116,"a",%progbits
	.set	.LANCHOR29,. + 0
	.type	__func__.6116, %object
	.size	__func__.6116, 9
__func__.6116:
	.ascii	"nftl_add\000"
	.section	.rodata.__func__.6463,"a",%progbits
	.set	.LANCHOR11,. + 0
	.type	__func__.6463, %object
	.size	__func__.6463, 16
__func__.6463:
	.ascii	"zone_param_init\000"
	.section	.rodata.__func__.6611,"a",%progbits
	.set	.LANCHOR14,. + 0
	.type	__func__.6611, %object
	.size	__func__.6611, 23
__func__.6611:
	.ascii	"get_current_used_block\000"
	.section	.rodata.__func__.6677,"a",%progbits
	.set	.LANCHOR1,. + 0
	.type	__func__.6677, %object
	.size	__func__.6677, 9
__func__.6677:
	.ascii	"init_nrc\000"
	.section	.rodata.__func__.6762,"a",%progbits
	.set	.LANCHOR7,. + 0
	.type	__func__.6762, %object
	.size	__func__.6762, 23
__func__.6762:
	.ascii	"torture_block_and_mark\000"
	.section	.rodata.__func__.6885,"a",%progbits
	.set	.LANCHOR9,. + 0
	.type	__func__.6885, %object
	.size	__func__.6885, 26
__func__.6885:
	.ascii	"block_last_page_ecc_error\000"
	.section	.rodata.__func__.6917,"a",%progbits
	.set	.LANCHOR2,. + 0
	.type	__func__.6917, %object
	.size	__func__.6917, 18
__func__.6917:
	.ascii	"nand_dbg_phy_read\000"
	.section	.rodata.__func__.6928,"a",%progbits
	.set	.LANCHOR3,. + 0
	.type	__func__.6928, %object
	.size	__func__.6928, 23
__func__.6928:
	.ascii	"nand_dbg_zone_phy_read\000"
	.section	.rodata.__func__.6939,"a",%progbits
	.set	.LANCHOR4,. + 0
	.type	__func__.6939, %object
	.size	__func__.6939, 24
__func__.6939:
	.ascii	"nand_dbg_zone_phy_write\000"
	.section	.rodata.__func__.6948,"a",%progbits
	.set	.LANCHOR5,. + 0
	.type	__func__.6948, %object
	.size	__func__.6948, 19
__func__.6948:
	.ascii	"nand_dbg_phy_write\000"
	.section	.rodata.__func__.7001,"a",%progbits
	.set	.LANCHOR16,. + 0
	.type	__func__.7001, %object
	.size	__func__.7001, 19
__func__.7001:
	.ascii	"write_data_to_nand\000"
	.section	.rodata.__func__.7032,"a",%progbits
	.set	.LANCHOR6,. + 0
	.type	__func__.7032, %object
	.size	__func__.7032, 23
__func__.7032:
	.ascii	"nftl_sector_cache_init\000"
	.section	.rodata.__func__.7249,"a",%progbits
	.set	.LANCHOR10,. + 0
	.type	__func__.7249, %object
	.size	__func__.7249, 19
__func__.7249:
	.ascii	"get_logic_page_map\000"
	.section	.rodata.__func__.7349,"a",%progbits
	.set	.LANCHOR8,. + 0
	.type	__func__.7349, %object
	.size	__func__.7349, 35
__func__.7349:
	.ascii	"_nftl_nand_read_logic_history_page\000"
	.section	.rodata.__func__.7517,"a",%progbits
	.set	.LANCHOR15,. + 0
	.type	__func__.7517, %object
	.size	__func__.7517, 16
__func__.7517:
	.ascii	"nftl_cache_init\000"
	.section	.rodata.__func__.7805,"a",%progbits
	.set	.LANCHOR25,. + 0
	.type	__func__.7805, %object
	.size	__func__.7805, 11
__func__.7805:
	.ascii	"nftl_start\000"
	.section	.rodata.__func__.7995,"a",%progbits
	.set	.LANCHOR19,. + 0
	.type	__func__.7995, %object
	.size	__func__.7995, 15
__func__.7995:
	.ascii	"shutdown_on_gc\000"
	.section	.rodata.__func__.8107,"a",%progbits
	.set	.LANCHOR28,. + 0
	.type	__func__.8107, %object
	.size	__func__.8107, 19
__func__.8107:
	.ascii	"panic_recover_data\000"
	.section	.rodata.__func__.8915,"a",%progbits
	.set	.LANCHOR60,. + 0
	.type	__func__.8915, %object
	.size	__func__.8915, 25
__func__.8915:
	.ascii	"sandisk_A19_special_exit\000"
	.section	.rodata.__nand_discard.str1.1,"aMS",%progbits,1
.LC270:
	.ascii	"[ND]dicard sector:0x%x,len:0x%x\012\000"
	.section	.rodata.__nand_flush_sector_write_cache.str1.1,"aMS",%progbits,1
.LC257:
	.ascii	"[NE]nand_flush_write_cache write error\012\000"
	.section	.rodata.__nand_flush_write_cache.str1.1,"aMS",%progbits,1
.LC268:
	.ascii	"[ND]flush cache %d %d!\012\000"
	.section	.rodata.__nand_read.str1.1,"aMS",%progbits,1
.LC251:
	.ascii	"[ND]read sector:0x%x,len:0x%x\012\000"
	.section	.rodata.__nand_write.str1.1,"aMS",%progbits,1
.LC253:
	.ascii	"[ND]write sector:0x%x,len:0x%x\012\000"
	.section	.rodata._nand_read_history.str1.1,"aMS",%progbits,1
.LC62:
	.ascii	"[NE]paramter error. beyond reality capacity\012\000"
	.section	.rodata._nftl_nand_read_logic_history_page.str1.1,"aMS",%progbits,1
.LC54:
	.ascii	"[NE]%s malloc buffer fail\012\000"
.LC55:
	.ascii	"scan super block@%d\012\000"
.LC56:
	.ascii	"block@%d is bad\012\000"
.LC57:
	.ascii	"[NE]read sb@%u sp@%u err, maybe read a bad block\012"
	.ascii	"\000"
.LC58:
	.ascii	"[NE]read lb@%u lp@%u err\012\000"
.LC59:
	.ascii	"page@%u history%d data:\012\000"
.LC60:
	.ascii	"oob:\012\000"
.LC61:
	.ascii	"scan end total super blocks:%d\012\000"
	.section	.rodata._nftl_nand_read_logic_page.str1.1,"aMS",%progbits,1
.LC315:
	.ascii	"[NE]read a page: %d READ_RECLAIM!\012\000"
	.section	.rodata._nftl_nand_write_logic_page.str1.1,"aMS",%progbits,1
.LC323:
	.ascii	"[NE]error nand_write_logic_page_no_gc page_no:%u\012"
	.ascii	"\000"
	.section	.rodata.adjust_free_list.str1.1,"aMS",%progbits,1
.LC44:
	.ascii	"[NE]adjust free list fail!!\012\000"
.LC45:
	.ascii	"[NI]adjust free list : %d %d!!\012\000"
	.section	.rodata.block_last_page_ecc_error.str1.1,"aMS",%progbits,1
.LC64:
	.ascii	"[NE]do block_last_page_ecc_error!!\012\000"
.LC65:
	.ascii	"[NE]%s:malloc fail 1!\012\000"
.LC66:
	.ascii	"[NE]%s:malloc fail 2!\012\000"
.LC67:
	.ascii	"[NE]current_block_ecc_error unkown page data!!\012\000"
.LC68:
	.ascii	"[NE]current_block_ecc_error invalid page data!!\012"
	.ascii	"\000"
.LC69:
	.ascii	"[NE]current_block_ecc_error no free block!!!\012\000"
.LC70:
	.ascii	"[NE]do block_last_page_ecc_error end!!\012\000"
	.section	.rodata.build_zone_list.str1.1,"aMS",%progbits,1
.LC354:
	.ascii	"[ND]first\012\000"
.LC355:
	.ascii	"[NE]init_zone_after_first_scan error\012\000"
.LC356:
	.ascii	"[NE]second_scan_all_blocks error\012\000"
.LC357:
	.ascii	"[NI]nftl ok!\012\000"
	.section	.rodata.build_zone_list_first.str1.1,"aMS",%progbits,1
.LC52:
	.ascii	"[ND]build_zone_list_first.\012\000"
.LC53:
	.ascii	"[NE]no block %d\012\000"
	.section	.rodata.check_invalid_page_right.str1.1,"aMS",%progbits,1
.LC237:
	.ascii	"[ND]logic_page_map_temp NULL!!\012\000"
.LC238:
	.ascii	"[NE]invaild_page num error:[%d] [%d]  used_page:[%d"
	.ascii	"]!!\012\000"
	.section	.rodata.check_static_wear_leveling.str1.1,"aMS",%progbits,1
.LC290:
	.ascii	"[ND]swl: static WL need to do(%d : %d)\012\000"
.LC291:
	.ascii	"[ND]swl: min_ec_blk_no = %d,\012\000"
.LC292:
	.ascii	"[ND]swl: zone->blocks = %d)\012\000"
	.section	.rodata.com,"a",%progbits
	.set	.LANCHOR26,. + 0
	.type	com, %object
	.size	com, 11
com:
	.byte	99
	.byte	111
	.byte	109
	.byte	112
	.byte	97
	.byte	116
	.byte	105
	.byte	98
	.byte	108
	.byte	101
	.byte	0
	.section	.rodata.crc_wrong.str1.1,"aMS",%progbits,1
.LC162:
	.ascii	"[NE]exception case wb %04x %04x, len: %d\012\000"
	.section	.rodata.cross_talk_rebuild_current_block.str1.1,"aMS",%progbits,1
.LC213:
	.ascii	"[NE]cross_talk_rebuild_current_block fail 1\012\000"
.LC214:
	.ascii	"[NE]cross_talk_rebuild_current_block!! %d,%d,%d!\012"
	.ascii	"\000"
.LC215:
	.ascii	"[NE]cross_talk_rebuild_current_block fail 2\012\000"
.LC216:
	.ascii	"[NE]cross_talk_rebuild_current_block!! %d,%d,%d,%d,"
	.ascii	"%d,%d!\012\000"
.LC217:
	.ascii	"[NE]cross_talk_rebuild_current_block fail 3\012\000"
.LC218:
	.ascii	"[NE]corss talk rebuild 1 %d %d %d!\012\000"
.LC219:
	.ascii	"[NE]corss talk rebuild 2 %d %d %d!\012\000"
.LC220:
	.ascii	"[NE]get free block fail!\012\000"
.LC221:
	.ascii	"[NE]something is error here 14!\012\000"
.LC222:
	.ascii	"[NE]page lost %d %d\012\000"
	.section	.rodata.current_block_ecc_error.str1.1,"aMS",%progbits,1
.LC228:
	.ascii	"[NE]do current_block_ecc_error %d!!\012\000"
.LC229:
	.ascii	"[NE]power off ecc error 2.\012\000"
.LC230:
	.ascii	"[NE]current_block_ecc ok page %d %d!!\012\000"
.LC231:
	.ascii	"[NE]current_block_ecc_error page %d!!\012\000"
.LC232:
	.ascii	"[NE]current_block_ecc_error invalid page data %d!!\012"
	.ascii	"\000"
.LC233:
	.ascii	"[NE]current_block_ecc_error unkown page data %d!!\012"
	.ascii	"\000"
.LC234:
	.ascii	"[ND]do current_block_ecc_error end %d %d!!\012\000"
	.section	.rodata.do_after_write.str1.1,"aMS",%progbits,1
.LC240:
	.ascii	"[NE]write_phy_page_map_to_current_block error!\012\000"
.LC241:
	.ascii	"[NE]get_new_current_block error1!\012\000"
.LC242:
	.ascii	"[NE]get_new_current_block error2!\012\000"
	.section	.rodata.do_before_write.str1.1,"aMS",%progbits,1
.LC235:
	.ascii	"[NE]get_new_current_block error3!\012\000"
.LC236:
	.ascii	"[NE]get_new_current_block error4!\012\000"
	.section	.rodata.do_prio_gc.str1.1,"aMS",%progbits,1
.LC310:
	.ascii	"[ND]no block to prio gc\012\000"
.LC311:
	.ascii	"[ND]no need gc:%d, type:%d\012\000"
.LC312:
	.ascii	"[NE]do_prio_gc garbage_collect_first fail:%d, type:"
	.ascii	"%d\012\000"
.LC313:
	.ascii	"[ND]prio gc fail1:%d, type:%d\012\000"
.LC314:
	.ascii	"[ND]prio gc fail2:%d, type:%d\012\000"
	.section	.rodata.do_write_error.str1.1,"aMS",%progbits,1
.LC186:
	.ascii	"[NE]no free block\012\000"
.LC187:
	.ascii	"[NE]do_write_error erase error sb:%d!\012\000"
.LC188:
	.ascii	"[NE]do_write_error sb@%d nftl_nand_copy_page error!"
	.ascii	"\012\000"
	.section	.rodata.do_write_error_in_build_list.str1.1,"aMS",%progbits,1
.LC29:
	.ascii	"[NE]NAND EVENT:do_write_error erase error1!\012\000"
.LC30:
	.ascii	"[NE]NAND EVENT:do_write_error erase error2!\012\000"
	.section	.rodata.done_static_wear_leveling.str1.1,"aMS",%progbits,1
.LC316:
	.ascii	"[ND]swl: the block(%d) for static WL is not in inva"
	.ascii	"lid_list\012\000"
.LC317:
	.ascii	"[ND]swl: done_static_wear_leveling(min_ec) block:%d"
	.ascii	", erase_count:%d\012\000"
.LC318:
	.ascii	"[NE]swl: garbage_collect_first fail:%d\012\000"
.LC319:
	.ascii	"[NE]swl: last->check fail:%d\012\000"
	.section	.rodata.dump_buf.str1.1,"aMS",%progbits,1
.LC25:
	.ascii	"%02x \000"
	.section	.rodata.dumphex.str1.1,"aMS",%progbits,1
.LC5:
	.ascii	"%08x: \000"
.LC6:
	.ascii	"%02x %02x %02x %02x %02x %02x %02x %02x %02x %02x %"
	.ascii	"02x %02x %02x %02x %02x %02x\012\000"
.LC7:
	.ascii	"\012\000"
	.section	.rodata.erase_block.str1.1,"aMS",%progbits,1
.LC42:
	.ascii	"[NI]erase_block block %d!\012\000"
.LC43:
	.ascii	"[NE]erase_block block fail %d!\012\000"
	.section	.rodata.first_scan_all_blocks.str1.1,"aMS",%progbits,1
.LC339:
	.ascii	"[ND]fsab\012\000"
.LC340:
	.ascii	"[NI]ftl end block! %d!\012\000"
.LC341:
	.ascii	"[ND]READ_RECLAIM!\012\000"
.LC342:
	.ascii	"[NI]start page , do nothing\012\000"
.LC343:
	.ascii	"[ND]fill block %d\012\000"
.LC344:
	.ascii	"[NI]b@%d p@0 unknow spare data:%x,%x,%x,%x,%x,%x,%x"
	.ascii	",%x,%x,%x,%x,%x,%x,%x,%x,%x.\012\000"
.LC345:
	.ascii	"[NE]nftl bad block: %d\012\000"
.LC346:
	.ascii	"[NE]nftl b@%d p@0 ecc error\012\000"
.LC347:
	.ascii	"[NE]b@%d last page ok !\012\000"
.LC348:
	.ascii	"[NI]READ_RECLAIM!\012\000"
.LC349:
	.ascii	"[NE]last page blank ! %d!\012\000"
.LC350:
	.ascii	"[NE]last page unkown ! %d!\012\000"
.LC351:
	.ascii	"[NE]b@%d first page last page ecc error\012\000"
.LC352:
	.ascii	"[NE]b@%d last page spare data %x,%x,%x,%x,%x,%x,%x,"
	.ascii	"%x,%x,%x,%x,%x,%x,%x,%x,%x\012\000"
.LC353:
	.ascii	"[NI]nftl bad block: %d\012\000"
	.section	.rodata.flush_write_cache_to_nand.str1.1,"aMS",%progbits,1
.LC258:
	.ascii	"[NE]flush write cache to nand no cache!\012\000"
.LC259:
	.ascii	"[NE]ECC_LIMIT happened 10! page:%d cache: %d!\012\000"
.LC260:
	.ascii	"[NE]flush write cache to nand error!\012\000"
.LC261:
	.ascii	"[NE]fwctn %d\012\000"
.LC262:
	.ascii	"[NE]fwctn w+\012\000"
	.section	.rodata.garbage_collect.str1.1,"aMS",%progbits,1
.LC307:
	.ascii	"[NE]garbage_collect phy_block null\012\000"
.LC308:
	.ascii	"[ND]garbage too frequency! %d:%d\012\000"
.LC309:
	.ascii	"[NE]garbage_collect_first wrong fbn:%d\012\000"
	.section	.rodata.garbage_collect_first.str1.1,"aMS",%progbits,1
.LC293:
	.ascii	"[NI]garbage_collect_first none!!\012\000"
.LC294:
	.ascii	"[ND]garbage_collect_first wrong!!\012\000"
.LC295:
	.ascii	"[ND]garbage_collect_first cannot find PHY_MAPPING_P"
	.ascii	"AGE%d %d %d %d!!\012\000"
.LC296:
	.ascii	"[ND]garbage_collect_first cannot find PHY_MAPPING_P"
	.ascii	"AGE%x %x %x %x %x %x %x %x %x %x %x %x %x %x %x %x."
	.ascii	"\012\000"
.LC297:
	.ascii	"[ND][NE]garbage_collect_first rebuild %d,%d!!\012\000"
.LC298:
	.ascii	"[ND]garbage_collect_first PHY_MAPPING_PAGEecc err b"
	.ascii	"lock:%d !!\012\000"
.LC299:
	.ascii	"[NE]garbage_collect_first invaild_page num error!!\012"
	.ascii	"\000"
.LC300:
	.ascii	"[ND]garbage_collect_first read a logic page error%d"
	.ascii	" %d %d %d spare data:\012\000"
.LC301:
	.ascii	"%x %x %x %x %x %x %x %x %x %x %x %x %x %x %x %x\012"
	.ascii	"\000"
.LC302:
	.ascii	"[NE]nand_write_logic_page_no_gc gc fail!!\012\000"
.LC303:
	.ascii	"[NE]erase ecc error block\012\000"
	.section	.rodata.gc_one.str1.1,"aMS",%progbits,1
.LC304:
	.ascii	"[ND]gc_one ok!\012\000"
.LC305:
	.ascii	"[NE]gc_one error!\012\000"
	.section	.rodata.gc_win_block.str1.1,"aMS",%progbits,1
.LC306:
	.ascii	"[ND][NE] gc_win_block gc_one error \012\000"
	.section	.rodata.generic_special_exit.str1.1,"aMS",%progbits,1
.LC456:
	.ascii	" generic special exit \012\000"
	.section	.rodata.generic_special_init.str1.1,"aMS",%progbits,1
.LC455:
	.ascii	" generic special init \012\000"
	.section	.rodata.get_current_used_block.str1.1,"aMS",%progbits,1
.LC202:
	.ascii	"%s %d:\012\000"
.LC203:
	.ascii	"[ND]recover %d %d\012\000"
.LC204:
	.ascii	"[NE]page used:%d invalid page count:%d error\012\000"
.LC205:
	.ascii	"[NE]new_block_init_for_write error here!\012\000"
.LC206:
	.ascii	"[NE]something is error here 10!\012\000"
.LC207:
	.ascii	"[NE]something is error here 11!\012\000"
.LC208:
	.ascii	"[NE]assist block page used:%d current block page us"
	.ascii	"ed:%d something is error here 12.\012\000"
.LC209:
	.ascii	"[NE]assist block page used:%d current block page us"
	.ascii	"ed:%d something is error here 17.\012\000"
.LC210:
	.ascii	"[NE]something is error here 16!\012\000"
.LC211:
	.ascii	"[ND]get a new free block\012\000"
.LC212:
	.ascii	"[NE]no free block to use!\012\000"
	.section	.rodata.get_logic_page_map.str1.1,"aMS",%progbits,1
.LC71:
	.ascii	"[NE]%s:logic_page:0x%x >= total_pages:0x%x\012\000"
	.section	.rodata.get_new_current_block.str1.1,"aMS",%progbits,1
.LC199:
	.ascii	"[ND]swl: new_cur_blk->alloc ok(max_ec): block_no = "
	.ascii	"%d; ec = %d; \012\000"
.LC200:
	.ascii	"[NE]no free block!!!!\012\000"
	.section	.rodata.get_used_page_num.str1.1,"aMS",%progbits,1
.LC180:
	.ascii	"[NE]get_used_page_num ecc error block:%d page:%d!\012"
	.ascii	"\000"
.LC181:
	.ascii	"[NE]power off ecc error 4!!\012\000"
.LC182:
	.ascii	"[NE]sb@%d sp@%d get_used_page_num used spare data %"
	.ascii	"x %x %x %x %x %x %x %x %x %x %x %x %x %x %x %x\012\000"
	.section	.rodata.get_used_page_num_no_crosstalk.str1.1,"aMS",%progbits,1
.LC183:
	.ascii	"[NE]gupnnc ecc err b:%d p:%d.\012\000"
.LC184:
	.ascii	"[NE]gupnnc power off ecc err.\012\000"
.LC185:
	.ascii	"[NE]cross talk block:%d total_good_num %d good_page"
	.ascii	"_num: %d!\012\000"
	.section	.rodata.hynix16nm_get_otp_param.str1.1,"aMS",%progbits,1
.LC414:
	.ascii	"_vender_get_param_otp_hynix time %d!\012\000"
	.section	.rodata.hynix16nm_get_read_retry_cfg.str1.1,"aMS",%progbits,1
.LC402:
	.ascii	"_get_read_retry_parameters, get rr count failed!\012"
	.ascii	"\000"
.LC403:
	.ascii	"rr cnt: %d\012\000"
.LC404:
	.ascii	"_get_read_retry_parameters, get rr reg count failed"
	.ascii	"!\012\000"
.LC405:
	.ascii	"rr reg cnt: %d\012\000"
.LC406:
	.ascii	"read retry value from otp error: rr_cnt %d rr_reg_c"
	.ascii	"nt %d!\012\000"
.LC407:
	.ascii	"_get_read_retry_parameters, get the %d-th byte of r"
	.ascii	"r table failed!\012\000"
	.section	.rodata.hynix16nm_get_rr_value_otp_hynix.str1.1,"aMS",%progbits,1
.LC411:
	.ascii	"otp_info_hynix_16nm : allocate memory fail\012\000"
.LC412:
	.ascii	"m1 CH %d chip %d get otp info failed!\012\000"
.LC413:
	.ascii	"m1 CH %d chip %d get read retry cfg from otp info f"
	.ascii	"ailed!\012\000"
	.section	.rodata.hynix16nm_get_special_info.str1.1,"aMS",%progbits,1
.LC394:
	.ascii	"get_hynix_special_info start!!\012\000"
.LC395:
	.ascii	"hynix_special_info:can't find uboot head\012\000"
.LC396:
	.ascii	"physic info start_block %d\012\000"
	.section	.rodata.hynix16nm_init_readretry_param.str1.1,"aMS",%progbits,1
.LC417:
	.ascii	"[PHY_GetDefaultParam]:readbuf malloc fail\012\000"
.LC418:
	.ascii	"ch %d, chip %d Read Retry value Table from uboot\012"
	.ascii	"\000"
.LC419:
	.ascii	"0x%x \000"
.LC420:
	.ascii	"[PHY_DBG] ch %d, can't get right otp value from nan"
	.ascii	"d otp blocks, then use otp command\012\000"
.LC421:
	.ascii	"[PHY_DBG] repair ch %d chip %d otp value end\012\000"
	.section	.rodata.hynix16nm_major_check_byte.str1.1,"aMS",%progbits,1
.LC399:
	.ascii	"_major_check_byte, wrong input para, level %d, len "
	.ascii	"%d\012\000"
.LC400:
	.ascii	"%d:  '1'-'0' : %d - %d\012\000"
.LC401:
	.ascii	"get bit %d failed!\012\000"
	.section	.rodata.hynix16nm_read_otp_info_hynix.str1.1,"aMS",%progbits,1
.LC408:
	.ascii	"start get read retry param from: ce %d, rb %d...\012"
	.ascii	"\000"
.LC409:
	.ascii	"invalid buffer for otp info!\012\000"
.LC410:
	.ascii	"hynix OTP RegCount value error: 0x%x, 0x%x \012\000"
	.section	.rodata.hynix16nm_read_page_end.str1.1,"aMS",%progbits,1
.LC392:
	.ascii	"m1 retry!\012\000"
.LC393:
	.ascii	"m1 ReadRetry ok! ch =%d, chip = %d  block = %d, pag"
	.ascii	"e = %d, RetryCount = %d  \012\000"
	.section	.rodata.hynix16nm_readretry_exit.str1.1,"aMS",%progbits,1
.LC423:
	.ascii	"hynix16nm_readretry_exit. \012\000"
	.section	.rodata.hynix16nm_readretry_init.str1.1,"aMS",%progbits,1
.LC422:
	.ascii	"NFC_ReadRetryInit, unknown read retry mode 0x%x\012"
	.ascii	"\000"
	.section	.rodata.hynix16nm_set_default_param.str1.1,"aMS",%progbits,1
.LC415:
	.ascii	"set retry default value: \000"
.LC416:
	.ascii	" %x\000"
	.section	.rodata.hynix16nm_special_exit.str1.1,"aMS",%progbits,1
.LC426:
	.ascii	" hynix16nm_special_exit \012\000"
	.section	.rodata.hynix16nm_special_init.str1.1,"aMS",%progbits,1
.LC424:
	.ascii	" hynix16nm_special_init hynix16nm_read_retry_mode: "
	.ascii	"%d hynix16nm_read_retry_cycle:%d hynix16nm_read_ret"
	.ascii	"ry_reg_num:%d\012\000"
.LC425:
	.ascii	" hynix16nm_special_init error hynix16nm_read_retry_"
	.ascii	"mode: %d hynix16nm_read_retry_cycle:%d hynix16nm_re"
	.ascii	"ad_retry_reg_num:%d\012\000"
	.section	.rodata.hynix16nm_vender_set_param.str1.1,"aMS",%progbits,1
.LC391:
	.ascii	"rr value %x %x %x %x!\012\000"
	.section	.rodata.hynix16nm_write_page_FF.str1.1,"aMS",%progbits,1
.LC397:
	.ascii	"fatal err -0, wrong input parameter, ch: %d  chip: "
	.ascii	"%d/%d  block: %d/%d \012\000"
.LC398:
	.ascii	"m1 write page FF wrong\012\000"
	.section	.rodata.hynix20nm_getotpparam.str1.1,"aMS",%progbits,1
.LC487:
	.ascii	"_vender_get_param_otp_hynix error!\012\000"
	.section	.rodata.hynix20nm_init_readretry_param.str1.1,"aMS",%progbits,1
.LC488:
	.ascii	"hynix20nm get default param error !\012\000"
.LC489:
	.ascii	"ch %d, chip %d Read Retry value Table from uboot:\012"
	.ascii	"\000"
.LC490:
	.ascii	"[PHY_DBG] repair ch %d otp value end\012\000"
	.section	.rodata.hynix20nm_lsb_exit.str1.1,"aMS",%progbits,1
.LC484:
	.ascii	"hynix20nm_lsb_exit\012\000"
	.section	.rodata.hynix20nm_read_page_end.str1.1,"aMS",%progbits,1
.LC481:
	.ascii	"hynix20nm retry!\012\000"
.LC482:
	.ascii	"sdata:0x%x %x %x %x \000"
.LC483:
	.ascii	"hynix20nm ReadRetry ok! ch =%d, chip = %d  block = "
	.ascii	"%d, page = %d, RetryCount = %d  \012\000"
	.section	.rodata.hynix20nm_readretry_exit.str1.1,"aMS",%progbits,1
.LC491:
	.ascii	"hynix20nm_readretry_exit. \012\000"
	.section	.rodata.hynix20nm_set_readretry.str1.1,"aMS",%progbits,1
.LC480:
	.ascii	"retry para:\000"
	.section	.rodata.hynix20nm_special_exit.str1.1,"aMS",%progbits,1
.LC494:
	.ascii	" hynix20nm_special_exit \012\000"
	.section	.rodata.hynix20nm_special_init.str1.1,"aMS",%progbits,1
.LC492:
	.ascii	" hynix20nm_special_init hynix20nm_read_retry_mode: "
	.ascii	"%d hynix20nm_read_retry_cycle:%d hynix20nm_read_ret"
	.ascii	"ry_reg_num:%d\012\000"
.LC493:
	.ascii	" hynix20nm_special_init error hynix20nm_read_retry_"
	.ascii	"mode: %d hynix20nm_read_retry_cycle:%d hynix20nm_re"
	.ascii	"ad_retry_reg_num:%d\012\000"
	.section	.rodata.hynix20nm_vender_get_param_otp_hynix.str1.1,"aMS",%progbits,1
.LC485:
	.ascii	"otp copy failed!\012\000"
.LC486:
	.ascii	"otp copy %d is ok!\012\000"
	.section	.rodata.hynix26nm_get_default_param.str1.1,"aMS",%progbits,1
.LC439:
	.ascii	"hynix26nm_get_default_param: ch: %d, chip: %d, valu"
	.ascii	"e: 0x%x 0x%x 0x%x 0x%x \012\000"
	.section	.rodata.hynix26nm_lsb_exit.str1.1,"aMS",%progbits,1
.LC438:
	.ascii	"hynix26nm_lsb_exit\012\000"
	.section	.rodata.hynix26nm_read_page_end.str1.1,"aMS",%progbits,1
.LC436:
	.ascii	"m0 retry!\012\000"
.LC437:
	.ascii	"hynix26nm ReadRetry ok! ch =%d, chip = %d  block = "
	.ascii	"%d, page = %d, RetryCount = %d  \012\000"
	.section	.rodata.hynix26nm_readretry_exit.str1.1,"aMS",%progbits,1
.LC440:
	.ascii	"hynix26nm_readretry_exit. \012\000"
	.section	.rodata.hynix26nm_set_readretry.str1.1,"aMS",%progbits,1
.LC434:
	.ascii	"hynix26nm retry param:\000"
.LC435:
	.ascii	" %x \000"
	.section	.rodata.hynix26nm_special_exit.str1.1,"aMS",%progbits,1
.LC443:
	.ascii	" hynix26nm_special_exit \012\000"
	.section	.rodata.hynix26nm_special_init.str1.1,"aMS",%progbits,1
.LC441:
	.ascii	" hynix26nm_special_init hynix26nm_read_retry_mode: "
	.ascii	"%d hynix26nm_read_retry_cycle:%d hynix26nm_read_ret"
	.ascii	"ry_reg_num:%d\012\000"
.LC442:
	.ascii	" hynix26nm_special_init error hynix26nm_read_retry_"
	.ascii	"mode: %d hynix26nm_read_retry_cycle:%d hynix26nm_re"
	.ascii	"ad_retry_reg_num:%d\012\000"
	.section	.rodata.init_nrc.str1.1,"aMS",%progbits,1
.LC31:
	.ascii	"%s %d:fail 1\012\000"
.LC32:
	.ascii	"%s %d:fail 2\012\000"
.LC33:
	.ascii	"%s %d:fail 3\012\000"
.LC34:
	.ascii	"%s %d:fail 4\012\000"
	.section	.rodata.init_zone_after_first_scan.str1.1,"aMS",%progbits,1
.LC26:
	.ascii	"[ND]before second %d %d.\012\000"
.LC27:
	.ascii	"[NE]only have blocks:%u,logic cap need blocks:%u th"
	.ascii	"is zone not enough data block!!\012\000"
.LC28:
	.ascii	"[NE]this zone not enough free block %d , %d!!\012\000"
	.section	.rodata.is_no_use_device.str1.1,"aMS",%progbits,1
.LC63:
	.ascii	"[NE]mtd size changed !\012\000"
	.section	.rodata.malloc_logic_page_map.str1.1,"aMS",%progbits,1
.LC76:
	.ascii	"[NE]%s:malloc fail for logic_page_map %d\012\000"
.LC77:
	.ascii	"[NE]%s: malloc fail for logic_page_map %d\012\000"
.LC78:
	.ascii	"[NE]zone_param_init malloc_logic_page_map error %d\012"
	.ascii	"\000"
	.section	.rodata.micron_intel_set_feature.str1.1,"aMS",%progbits,1
.LC465:
	.ascii	"set feature(addr %d) p0: %d readretry Configuration"
	.ascii	" failed!\012\000"
	.section	.rodata.micron_read_page_end.str1.1,"aMS",%progbits,1
.LC466:
	.ascii	"micron retry!\012\000"
.LC467:
	.ascii	"micron ReadRetry ok! ch =%d, chip = %d  block = %d,"
	.ascii	" page = %d, RetryCount = %d  \012\000"
	.section	.rodata.micron_readretry_init.str1.1,"aMS",%progbits,1
.LC468:
	.ascii	"micron read retry cannot support!\012\000"
.LC469:
	.ascii	"micron read retry conut: %d !\012\000"
	.section	.rodata.micron_special_exit.str1.1,"aMS",%progbits,1
.LC472:
	.ascii	" micron_special_exit \012\000"
	.section	.rodata.micron_special_init.str1.1,"aMS",%progbits,1
.LC470:
	.ascii	" micron_special_init micron_read_retry_mode:%d micr"
	.ascii	"on_read_retry_cycle:%d \012\000"
.LC471:
	.ascii	" micron_special_init error micron_read_retry_mode:%"
	.ascii	"d micron_read_retry_cycle:%d \012\000"
	.section	.rodata.mod,"a",%progbits
	.set	.LANCHOR24,. + 0
	.type	mod, %object
	.size	mod, 6
mod:
	.byte	109
	.byte	111
	.byte	100
	.byte	101
	.byte	108
	.byte	0
	.section	.rodata.nand_cache_read.str1.1,"aMS",%progbits,1
.LC249:
	.ascii	"[NI]nand_cache_read error buf is  NULL!\012\000"
.LC250:
	.ascii	"[NE]error1 node %d,%d,%d!\012\000"
	.section	.rodata.nand_cache_write.str1.1,"aMS",%progbits,1
.LC263:
	.ascii	"[NE]error1 bitmap %x!\012\000"
.LC264:
	.ascii	"[NE]ECC_LIMIT happened 12! page:%d!\012\000"
.LC265:
	.ascii	"[NE]error1 flush write cache to nand\012\000"
.LC266:
	.ascii	"[NE]error2 flush write cache to nand\012\000"
.LC267:
	.ascii	"[NI]ECC_LIMIT happened 11! page:%d!\012\000"
	.section	.rodata.nand_check_table.str1.1,"aMS",%progbits,1
.LC362:
	.ascii	"[ND]==========================\012\000"
	.section	.rodata.nand_clean_zone_table2.str1.1,"aMS",%progbits,1
.LC39:
	.ascii	"[ND]zoneNO:%d total_pages: %d\012\000"
.LC40:
	.ascii	"[ND]NAND_Memset bytes: %d\012\000"
	.section	.rodata.nand_crc16_table,"a",%progbits
	.align	1
	.set	.LANCHOR12,. + 0
	.type	nand_crc16_table, %object
	.size	nand_crc16_table, 512
nand_crc16_table:
	.short	0
	.short	-16191
	.short	-15999
	.short	320
	.short	-15615
	.short	960
	.short	640
	.short	-15807
	.short	-14847
	.short	1728
	.short	1920
	.short	-14527
	.short	1280
	.short	-14911
	.short	-15231
	.short	1088
	.short	-13311
	.short	3264
	.short	3456
	.short	-12991
	.short	3840
	.short	-12351
	.short	-12671
	.short	3648
	.short	2560
	.short	-13631
	.short	-13439
	.short	2880
	.short	-14079
	.short	2496
	.short	2176
	.short	-14271
	.short	-10239
	.short	6336
	.short	6528
	.short	-9919
	.short	6912
	.short	-9279
	.short	-9599
	.short	6720
	.short	7680
	.short	-8511
	.short	-8319
	.short	8000
	.short	-8959
	.short	7616
	.short	7296
	.short	-9151
	.short	5120
	.short	-11071
	.short	-10879
	.short	5440
	.short	-10495
	.short	6080
	.short	5760
	.short	-10687
	.short	-11775
	.short	4800
	.short	4992
	.short	-11455
	.short	4352
	.short	-11839
	.short	-12159
	.short	4160
	.short	-4095
	.short	12480
	.short	12672
	.short	-3775
	.short	13056
	.short	-3135
	.short	-3455
	.short	12864
	.short	13824
	.short	-2367
	.short	-2175
	.short	14144
	.short	-2815
	.short	13760
	.short	13440
	.short	-3007
	.short	15360
	.short	-831
	.short	-639
	.short	15680
	.short	-255
	.short	16320
	.short	16000
	.short	-447
	.short	-1535
	.short	15040
	.short	15232
	.short	-1215
	.short	14592
	.short	-1599
	.short	-1919
	.short	14400
	.short	10240
	.short	-5951
	.short	-5759
	.short	10560
	.short	-5375
	.short	11200
	.short	10880
	.short	-5567
	.short	-4607
	.short	11968
	.short	12160
	.short	-4287
	.short	11520
	.short	-4671
	.short	-4991
	.short	11328
	.short	-7167
	.short	9408
	.short	9600
	.short	-6847
	.short	9984
	.short	-6207
	.short	-6527
	.short	9792
	.short	8704
	.short	-7487
	.short	-7295
	.short	9024
	.short	-7935
	.short	8640
	.short	8320
	.short	-8127
	.short	-24575
	.short	24768
	.short	24960
	.short	-24255
	.short	25344
	.short	-23615
	.short	-23935
	.short	25152
	.short	26112
	.short	-22847
	.short	-22655
	.short	26432
	.short	-23295
	.short	26048
	.short	25728
	.short	-23487
	.short	27648
	.short	-21311
	.short	-21119
	.short	27968
	.short	-20735
	.short	28608
	.short	28288
	.short	-20927
	.short	-22015
	.short	27328
	.short	27520
	.short	-21695
	.short	26880
	.short	-22079
	.short	-22399
	.short	26688
	.short	30720
	.short	-18239
	.short	-18047
	.short	31040
	.short	-17663
	.short	31680
	.short	31360
	.short	-17855
	.short	-16895
	.short	32448
	.short	32640
	.short	-16575
	.short	32000
	.short	-16959
	.short	-17279
	.short	31808
	.short	-19455
	.short	29888
	.short	30080
	.short	-19135
	.short	30464
	.short	-18495
	.short	-18815
	.short	30272
	.short	29184
	.short	-19775
	.short	-19583
	.short	29504
	.short	-20223
	.short	29120
	.short	28800
	.short	-20415
	.short	20480
	.short	-28479
	.short	-28287
	.short	20800
	.short	-27903
	.short	21440
	.short	21120
	.short	-28095
	.short	-27135
	.short	22208
	.short	22400
	.short	-26815
	.short	21760
	.short	-27199
	.short	-27519
	.short	21568
	.short	-25599
	.short	23744
	.short	23936
	.short	-25279
	.short	24320
	.short	-24639
	.short	-24959
	.short	24128
	.short	23040
	.short	-25919
	.short	-25727
	.short	23360
	.short	-26367
	.short	22976
	.short	22656
	.short	-26559
	.short	-30719
	.short	18624
	.short	18816
	.short	-30399
	.short	19200
	.short	-29759
	.short	-30079
	.short	19008
	.short	19968
	.short	-28991
	.short	-28799
	.short	20288
	.short	-29439
	.short	19904
	.short	19584
	.short	-29631
	.short	17408
	.short	-31551
	.short	-31359
	.short	17728
	.short	-30975
	.short	18368
	.short	18048
	.short	-31167
	.short	-32255
	.short	17088
	.short	17280
	.short	-31935
	.short	16640
	.short	-32319
	.short	-32639
	.short	16448
	.section	.rodata.nand_dbg_phy_read.str1.1,"aMS",%progbits,1
.LC35:
	.ascii	"[NE]%s:malloc 4096 fail!\012\000"
.LC36:
	.ascii	"[ND]spare: %x %x %x %x %x %x %x %x %x %x %x %x %x %"
	.ascii	"x %x %x\000"
.LC37:
	.ascii	"[ND]buf:%x %x %x %x %x %x %x %x %x %x %x.\012\000"
	.section	.rodata.nand_dbg_zone_phy_write.str1.1,"aMS",%progbits,1
.LC38:
	.ascii	"[NE]%s:malloc 8192 fail!\012\000"
	.section	.rodata.nand_discard.str1.1,"aMS",%progbits,1
.LC269:
	.ascii	"[NE]nand_discard paramter error!\012\000"
	.section	.rodata.nand_find_zone_table.str1.1,"aMS",%progbits,1
.LC358:
	.ascii	"[ND]build_zone_list start\012\000"
.LC359:
	.ascii	"[ND]nftl_cache_init start\012\000"
.LC360:
	.ascii	"[ND]=================================2============="
	.ascii	"==========================\012\000"
.LC361:
	.ascii	"[ND]nand_find_zone_table end\012\000"
	.section	.rodata.nand_power_off_ecc_error.str1.1,"aMS",%progbits,1
.LC163:
	.ascii	"[NE]exception case bad block\012\000"
.LC164:
	.ascii	"[NE]exception case wbc\012\000"
.LC165:
	.ascii	"[NE]exception case gd\012\000"
.LC166:
	.ascii	"[NE]%s %d, flag = %d\012\000"
.LC167:
	.ascii	"[NE]OOB :\000"
.LC168:
	.ascii	"%.2x %.2x %.2x %.2x %.2x %.2x %.2x %.2x %.2x %.2x %"
	.ascii	".2x %.2x %.2x %.2x %.2x %.2x\012\000"
	.section	.rodata.nand_sector_cache_read.str1.1,"aMS",%progbits,1
.LC252:
	.ascii	"[NE]nand_sector_cache_read 1!\012\000"
	.section	.rodata.nand_sector_cache_write.str1.1,"aMS",%progbits,1
.LC255:
	.ascii	"[NE]nand_sector_cache_write 1!\012\000"
.LC256:
	.ascii	"[NE]nand_sector_cache_write 2!\012\000"
	.section	.rodata.nand_write_data_page.str1.1,"aMS",%progbits,1
.LC243:
	.ascii	"[NE]do after write error1!\012\000"
.LC244:
	.ascii	"[NE]do_before_write error!\012\000"
.LC245:
	.ascii	"[NE]write data to page error!\012\000"
.LC246:
	.ascii	"[NE]do after write error2!\012\000"
	.section	.rodata.new_block_init_for_write.str1.1,"aMS",%progbits,1
.LC201:
	.ascii	"[NE]new_block_init_for_write erase error!\012\000"
	.section	.rodata.nftl_add.str1.1,"aMS",%progbits,1
.LC387:
	.ascii	"[NE]%s:malloc fail for nftl_blk\012\000"
.LC388:
	.ascii	"[NE]nftl_initialize failed\012\000"
.LC389:
	.ascii	"[ND]nftl_add ok\012\000"
	.section	.rodata.nftl_build_all.str1.1,"aMS",%progbits,1
.LC390:
	.ascii	"[NE]nftl_build_all error!\012\000"
	.section	.rodata.nftl_cache_init.str1.1,"aMS",%progbits,1
.LC247:
	.ascii	"[NE]%s:malloc fail for cache_node!\012\000"
.LC248:
	.ascii	"[NE]%s:malloc fail for cache_page_buf!\012\000"
	.section	.rodata.nftl_get_gc_info.str1.1,"aMS",%progbits,1
.LC327:
	.ascii	"%d %d %d %d %d %d/%d %d/%d\012\000"
.LC328:
	.ascii	"%d:%d \000"
	.section	.rodata.nftl_initialize.str1.1,"aMS",%progbits,1
.LC372:
	.ascii	"000\000"
.LC373:
	.ascii	"0\000"
.LC374:
	.ascii	"5\000"
.LC375:
	.ascii	"sunxi nftl version %s.%s.%s  %s-%s\012\000"
.LC376:
	.ascii	"09:50\000"
.LC377:
	.ascii	"2020.12.24\000"
.LC378:
	.ascii	"[NE]%s:malloc fail for nftl_cfg\012\000"
.LC379:
	.ascii	"[ND]max_erase_times = %i\012\000"
	.section	.rodata.nftl_malloc.str1.1,"aMS",%progbits,1
.LC8:
	.ascii	"[NI]malloc size is large %d!\012\000"
	.section	.rodata.nftl_read.str1.1,"aMS",%progbits,1
.LC10:
	.ascii	"[NE]parameter error %d,%d !\012\000"
	.section	.rodata.nftl_read_history.str1.1,"aMS",%progbits,1
.LC11:
	.ascii	"awa1543 nftl_logic_size:%u\012\000"
	.section	.rodata.nftl_sector_cache_init.str1.1,"aMS",%progbits,1
.LC41:
	.ascii	"[NE]%s:malloc fail for cache_buf!\012\000"
	.section	.rodata.nftl_start.str1.1,"aMS",%progbits,1
.LC363:
	.ascii	"[NE]%s: malloc fail for zone\012\000"
.LC364:
	.ascii	"[NE]%s: malloc fail for nand_chip\012\000"
.LC365:
	.ascii	"[NE]%s: malloc fail for nand_block_info\012\000"
.LC366:
	.ascii	"[NE]not enough blocks %d!\012\000"
.LC367:
	.ascii	"[ND]nftl start:%d,%d\012\000"
.LC368:
	.ascii	"[NE]zone_param_init fail!\012\000"
.LC369:
	.ascii	"[NE]build_zone_list_first fail!\012\000"
.LC370:
	.ascii	"[NE]build_zone_list fail!\012\000"
.LC371:
	.ascii	"[NI]try again\012\000"
	.section	.rodata.no_cross_talk_rebuild_current_block.str1.1,"aMS",%progbits,1
.LC223:
	.ascii	"[NI]nctrcb1 %d,%d,%d!\012\000"
.LC224:
	.ascii	"[NE]no_cross_talk_rebuild_current_block fail 2\012\000"
.LC225:
	.ascii	"[NI]nctrcb2 %d,%d,%d,%d,%d,%d!\012\000"
.LC226:
	.ascii	"[NE]no_cross_talk_rebuild_current_block fail 4\012\000"
.LC227:
	.ascii	"[NE]no_cross_talk_rebuild_current_block erase block"
	.ascii	" fail %d!\012\000"
	.section	.rodata.panic_backup_block.str1.1,"aMS",%progbits,1
.LC330:
	.ascii	"[ND] reserved panic block before %d!\012\000"
.LC331:
	.ascii	"[NE] reserved panic block %d!\012\000"
.LC332:
	.ascii	"[NE] no reserved panic block!!! quit!\012\000"
	.section	.rodata.panic_init_part.str1.1,"aMS",%progbits,1
.LC329:
	.ascii	"[ND] panic part %s off %u size %u\012\000"
	.section	.rodata.panic_recover_data.str1.1,"aMS",%progbits,1
.LC380:
	.ascii	"[ND] not find panic data, no need to recovery!\012\000"
.LC381:
	.ascii	"[ND] recovery panic data start!\012\000"
.LC382:
	.ascii	"[NE] %s(%d): malloc failed\012\000"
.LC383:
	.ascii	"[ND] ignore blk %d page %d as dummy page\012\000"
.LC384:
	.ascii	"[ND] exit recovery panic block\012\000"
.LC385:
	.ascii	"[ND] invalid blk %d page %d, finish recovery\012\000"
.LC386:
	.ascii	"[ND] move blk %d page %d to logic addr %d\012\000"
	.section	.rodata.panic_recover_info.str1.1,"aMS",%progbits,1
.LC337:
	.ascii	"[ND] find panic block : %d\012\000"
.LC338:
	.ascii	"[NE] invalid panic block index %d\012\000"
	.section	.rodata.panic_write.str1.1,"aMS",%progbits,1
.LC333:
	.ascii	"[ND] write panic data : logic addr: %d, sectors %d,"
	.ascii	" pages %d, blocks %d\012\000"
.LC334:
	.ascii	"[NE] too much data for panic-write, %d sectors left"
	.ascii	" over\012\000"
.LC335:
	.ascii	"[ND] write panic data block: %d page %d !\012\000"
.LC336:
	.ascii	"[ND] write dummy data block: %d page %d !\012\000"
	.section	.rodata.para0,"a",%progbits
	.align	1
	.set	.LANCHOR48,. + 0
	.type	para0, %object
	.size	para0, 56
para0:
	.short	0
	.short	0
	.short	0
	.short	0
	.short	0
	.short	6
	.short	10
	.short	6
	.short	0
	.short	-3
	.short	-7
	.short	-8
	.short	0
	.short	-6
	.short	-13
	.short	-15
	.short	0
	.short	-11
	.short	-20
	.short	-23
	.short	0
	.short	0
	.short	-26
	.short	-30
	.short	0
	.short	0
	.short	-32
	.short	-37
	.section	.rodata.para1,"a",%progbits
	.align	1
	.set	.LANCHOR49,. + 0
	.type	para1, %object
	.size	para1, 56
para1:
	.short	0
	.short	0
	.short	0
	.short	0
	.short	0
	.short	6
	.short	10
	.short	6
	.short	0
	.short	-3
	.short	-7
	.short	-8
	.short	0
	.short	-6
	.short	-13
	.short	-15
	.short	0
	.short	-9
	.short	-20
	.short	-23
	.short	0
	.short	0
	.short	-26
	.short	-30
	.short	0
	.short	0
	.short	-32
	.short	-37
	.section	.rodata.phy_block_from_invalid_page_incr.str1.1,"aMS",%progbits,1
.LC23:
	.ascii	"[ND]invalid_page_count more than pages per blk 0 %d"
	.ascii	" %d!!\012\000"
.LC24:
	.ascii	"[ND]invalid_page_count more than pages per blk 1!!\012"
	.ascii	"\000"
	.section	.rodata.power_on_gc.str1.1,"aMS",%progbits,1
.LC320:
	.ascii	"[ND]not power on gc [%d, %d, %d], [%d %d %d]!\012\000"
.LC321:
	.ascii	"[ND]emergency power on gc %d !\012\000"
.LC322:
	.ascii	"[ND]power on gc %d!\012\000"
	.section	.rodata.print_all_nftl_blk.str1.1,"aMS",%progbits,1
.LC9:
	.ascii	"[ND]cap: 0x%08x.\012\000"
	.section	.rodata.print_block_count_list.str1.1,"aMS",%progbits,1
.LC18:
	.ascii	"[ND]block_used_count list: \012\000"
.LC19:
	.ascii	"[ND]block NO: %d \000"
.LC20:
	.ascii	"[ND]block_used_count: %d \012\000"
	.section	.rodata.print_block_invalid_list.str1.1,"aMS",%progbits,1
.LC16:
	.ascii	"[ND]invalid_page_count list: \012\000"
.LC17:
	.ascii	"[ND]count:%4d; invalid:%3d;\000"
	.section	.rodata.print_free_list.str1.1,"aMS",%progbits,1
.LC12:
	.ascii	"[ND]free list: \012\000"
.LC13:
	.ascii	"[ND]block NO:%4d; erase:%3d; \000"
.LC14:
	.ascii	"[ND]count:%4d; \000"
.LC15:
	.ascii	"[ND]info:0x%x\012\000"
	.section	.rodata.print_logic_page_map.str1.1,"aMS",%progbits,1
.LC271:
	.ascii	"[NI]page_map NULL!!\012\000"
.LC272:
	.ascii	"[ND]%4d:<%4d %3d><%d>\012\000"
	.section	.rodata.print_nftl_zone.str1.1,"aMS",%progbits,1
.LC109:
	.ascii	"[ND]nand3.x \012\000"
.LC110:
	.ascii	"[ND]zone->nand_chip->blk_per_chip: %d \012\000"
.LC111:
	.ascii	"[ND]zone->nand_chip->bytes_per_page: %d \012\000"
.LC112:
	.ascii	"[ND]zone->nand_chip->pages_per_blk: %d \012\000"
.LC113:
	.ascii	"[ND]zone->nand_chip->max_erase_times: %d \012\000"
.LC114:
	.ascii	"[ND]zone->nand_chip->support_read_reclaim: %d \012\000"
.LC115:
	.ascii	"[ND]zone->test: %d \012\000"
.LC116:
	.ascii	"[ND]zone->zone_no: %d \012\000"
.LC117:
	.ascii	"[ND]zone->zone_attr: %d \012\000"
.LC118:
	.ascii	"[ND]zone->blocks: %d \012\000"
.LC119:
	.ascii	"[ND]zone->bad_block: %d \012\000"
.LC120:
	.ascii	"[ND]zone->logic_cap_in_sects: %d \012\000"
.LC121:
	.ascii	"[ND]zone->backup_cap_in_sects: %d \012\000"
.LC122:
	.ascii	"[ND]zone->free_block_num: %d \012\000"
.LC123:
	.ascii	"[ND]zone->gc_strategy.start_gc_free_blocks: %d \012"
	.ascii	"\000"
.LC124:
	.ascii	"[ND]zone->gc_strategy.stop_gc_free_blocks: %d \012\000"
.LC125:
	.ascii	"[ND]zone->gc_strategy.gc_page: %d \012\000"
.LC126:
	.ascii	"[ND]zone->gc_strategy.process: %d \012\000"
.LC127:
	.ascii	"[ND]zone->prio_gc.prio_type : %d \012\000"
.LC128:
	.ascii	"[ND]zone->zone_start_phy_block->phy_block.Block_NO:"
	.ascii	" %d \012\000"
.LC129:
	.ascii	"[ND]zone->zone_end_phy_block->phy_block.Block_NO: %"
	.ascii	"d \012\000"
.LC130:
	.ascii	"[ND]zone->zone_phy_page_map_for_gc: %x \012\000"
.LC131:
	.ascii	"[ND]zone->current_block.user_info: %x \012\000"
.LC132:
	.ascii	"[ND]zone->current_block.block_info: %x \012\000"
.LC133:
	.ascii	"[ND]zone->current_block.block_info->phy_block.Block"
	.ascii	"_NO: %d \012\000"
.LC134:
	.ascii	"[ND]zone->current_block.page_used: %d \012\000"
.LC135:
	.ascii	"[ND]zone->current_block.block_info->block_used_coun"
	.ascii	"t: %d \012\000"
.LC136:
	.ascii	"[ND]zone->recv_free_block: %d %d!!\012\000"
.LC137:
	.ascii	"[ND]zone->assist_block.user_info: %x \012\000"
.LC138:
	.ascii	"[ND]zone->assist_block.block_info: %x \012\000"
.LC139:
	.ascii	"[ND]zone->assist_block.block_info->phy_block.Block_"
	.ascii	"NO: %d \012\000"
.LC140:
	.ascii	"[ND]zone->assist_block.page_used: %d \012\000"
.LC141:
	.ascii	"[ND]zone->assist_block.block_info->block_used_count"
	.ascii	": %d \012\000"
.LC142:
	.ascii	"[ND]zone->read_reclaim_complete: %x \012\000"
.LC143:
	.ascii	"[ND]zone->temp_page_buf: %x \012\000"
.LC144:
	.ascii	"[ND]zone->max_erase_num: %d \012\000"
.LC145:
	.ascii	"[ND]zone->cache.cache_totals: %x \012\000"
.LC146:
	.ascii	"[ND]zone->wl_gc_count: %d \012\000"
.LC147:
	.ascii	"[ND]zone->wl_gc_page_num: %d \012\000"
.LC148:
	.ascii	"[ND]zone->cfg->nftl_dont_use_cache: %x \012\000"
.LC149:
	.ascii	"[ND]zone->cfg->nftl_use_cache_sort: %x \012\000"
.LC150:
	.ascii	"[ND]zone->cfg->nftl_support_gc_read_reclaim: %x \012"
	.ascii	"\000"
.LC151:
	.ascii	"[ND]zone->cfg->nftl_support_wear_leveling: %x \012\000"
.LC152:
	.ascii	"[ND]zone->cfg->nftl_need_erase: %x \012\000"
.LC153:
	.ascii	"[ND]zone->cfg->nftl_min_free_block_num: %x \012\000"
.LC154:
	.ascii	"[ND]zone->cfg->nftl_gc_threshold_free_block_num: %x"
	.ascii	" \012\000"
.LC155:
	.ascii	"[ND]zone->cfg->nftl_min_free_block: %x \012\000"
.LC156:
	.ascii	"[ND]zone->cfg->nftl_gc_threshold_ratio_numerator: %"
	.ascii	"x \012\000"
.LC157:
	.ascii	"[ND]zone->cfg->nftl_gc_threshold_ratio_denominator:"
	.ascii	" %x \012\000"
.LC158:
	.ascii	"[ND]zone->cfg->nftl_max_cache_num: %d \012\000"
.LC159:
	.ascii	"[ND]zone->cfg->nftl_max_cache_write_num: %d \012\000"
.LC160:
	.ascii	"[ND]zone->cfg->nftl_cross_talk: %x \012\000"
.LC161:
	.ascii	"[ND]zone->cfg->nftl_read_claim_interval: %d \012\000"
	.section	.rodata.print_smart.str1.1,"aMS",%progbits,1
.LC85:
	.ascii	"[ND]smart->version:%x;\012\000"
.LC86:
	.ascii	"[ND]smart->total_recv_read_sectors:%lld;\012\000"
.LC87:
	.ascii	"[ND]smart->total_recv_write_sectors:%lld;\012\000"
.LC88:
	.ascii	"[ND]smart->total_real_read_pages:%lld;\012\000"
.LC89:
	.ascii	"[ND]smart->total_real_write_pages:%lld;\012\000"
.LC90:
	.ascii	"[ND]smart->total_recv_discard_sectors:%lld;\012\000"
.LC91:
	.ascii	"[ND]smart->total_real_discard_sectors:%lld;\012\000"
.LC92:
	.ascii	"[ND]smart->total_recv_read_claim_pages:%d;\012\000"
.LC93:
	.ascii	"[ND]smart->total_gc_times:%d;\012\000"
.LC94:
	.ascii	"[ND]smart->total_gc_pages:%d;\012\000"
.LC95:
	.ascii	"[ND]smart->total_wl_times:%d;\012\000"
.LC96:
	.ascii	"[ND]smart->total_wl_gc_times:%d;\012\000"
.LC97:
	.ascii	"[ND]smart->total_real_read_error_pages:%d;\012\000"
.LC98:
	.ascii	"[ND]smart->total_real_write_error_pages:%d;\012\000"
.LC99:
	.ascii	"[ND]smart->total_normal_power_cycles:%x;\012\000"
.LC100:
	.ascii	"[ND]smart->total_unusual_power_cycles:%x;\012\000"
.LC101:
	.ascii	"[ND]smart->max_block_erase_times:%d;\012\000"
.LC102:
	.ascii	"[ND]smart->min_block_erase_times:%d;\012\000"
.LC103:
	.ascii	"[ND]smart->max_block_used_counter:%d;\012\000"
.LC104:
	.ascii	"[ND]smart->min_block_used_counter:%d;\012\000"
.LC105:
	.ascii	"[ND]smart->read_reclaim_utc:%d;\012\000"
.LC106:
	.ascii	"[ND]smart->cross_talk_times:%d;\012\000"
.LC107:
	.ascii	"[ND]smart->read_reclaim_zone_no:%d;\012\000"
.LC108:
	.ascii	"[ND]smart->read_reclaim_page_no:%d;\012\000"
	.section	.rodata.put_phy_block_to_free_list.str1.1,"aMS",%progbits,1
.LC21:
	.ascii	"[ND]free_list error !\012\000"
	.section	.rodata.put_phy_block_to_invalid_page_list.str1.1,"aMS",%progbits,1
.LC22:
	.ascii	"[ND]add invalid_list error !\012\000"
	.section	.rodata.recover_block_phy_page_mapping.str1.1,"aMS",%progbits,1
.LC176:
	.ascii	"[NE]ret:%d read fail %d %d,read one more time!!\012"
	.ascii	"\000"
.LC177:
	.ascii	"[NE]ret:%d read fail %d %d,%d times,don't read more"
	.ascii	"!!\012\000"
.LC178:
	.ascii	"[NE]power off ecc error 3!!\012\000"
.LC179:
	.ascii	"[NE]recover sb@%d sp@%d phy page mapping error.\012"
	.ascii	"\000"
	.section	.rodata.recover_logic_page_mapping.str1.1,"aMS",%progbits,1
.LC72:
	.ascii	"[NI]logic_page_map is NULL\012\000"
.LC73:
	.ascii	"[NE]invalid_page_count more than pages_per_blk :%d\012"
	.ascii	"\000"
.LC74:
	.ascii	"[NE]inv P b:%d p:%d,lp %x\012\000"
.LC75:
	.ascii	"[NE]build invalid_page_count more than pages_per_bl"
	.ascii	"k\012\000"
	.section	.rodata.recover_phy_page_mapping.str1.1,"aMS",%progbits,1
.LC169:
	.ascii	"[NE]sb@%d sp@%d recover_phy_page_mapping no used pa"
	.ascii	"ge\012\000"
.LC170:
	.ascii	"[NE]sb@%d sp@%d power off ecc error1.\012\000"
.LC171:
	.ascii	"[NE]invalid logic_page block:%d page:%d,logic_page "
	.ascii	"0x%x.\012\000"
.LC172:
	.ascii	"[NE]rppm invalid page %d!!\012\000"
.LC173:
	.ascii	"[NI]power_down page %d!!\012\000"
.LC174:
	.ascii	"[NE]recover_phy_page_mapping unkown page data %d!!\012"
	.ascii	"\000"
.LC175:
	.ascii	"[NE]recover_phy_page_mapping ecc error block:%d;pag"
	.ascii	"e:%d.\012\000"
	.section	.rodata.samsung_read_page_end.str1.1,"aMS",%progbits,1
.LC428:
	.ascii	"samsung retry!\012\000"
.LC429:
	.ascii	"samsung ReadRetry ok! ch =%d, chip = %d  block = %d"
	.ascii	", page = %d, RetryCount = %d  \012\000"
	.section	.rodata.samsung_readretry_init.str1.1,"aMS",%progbits,1
.LC430:
	.ascii	"samsung read retry conut: %d !\012\000"
	.section	.rodata.samsung_set_readretry.str1.1,"aMS",%progbits,1
.LC427:
	.ascii	"samsung set readretry error ! %x \012\000"
	.section	.rodata.samsung_special_exit.str1.1,"aMS",%progbits,1
.LC433:
	.ascii	" samsung_special_exit \012\000"
	.section	.rodata.samsung_special_init.str1.1,"aMS",%progbits,1
.LC431:
	.ascii	" samsung_special_init samsung_read_retry_mode:%d sa"
	.ascii	"msung_read_retry_cycle :%d \012\000"
.LC432:
	.ascii	" samsung_special_init error samsung_read_retry_mode"
	.ascii	":%d samsung_read_retry_cycle :%d \012\000"
	.section	.rodata.sandisk_A19_read_page_end.str1.1,"aMS",%progbits,1
.LC446:
	.ascii	"sandisk_A19 retry!\012\000"
.LC447:
	.ascii	"sandisk_A19 set readretry error\012\000"
.LC448:
	.ascii	"retry spare all 0xff! ch =%d, chip = %d  block = %d"
	.ascii	", page = %d, RetryCount = %d  \012\000"
.LC449:
	.ascii	"sandisk_A19 ReadRetry ok! ch =%d, chip = %d  block "
	.ascii	"= %d, page = %d, RetryCount = %d  \012\000"
.LC450:
	.ascii	"sandisk_A19 ReadRetry ok with ecc limit! ch =%d, ch"
	.ascii	"ip = %d  block = %d, page = %d, RetryCount = %d  \012"
	.ascii	"\000"
	.section	.rodata.sandisk_A19_readretry_init.str1.1,"aMS",%progbits,1
.LC444:
	.ascii	"sandisk_A19 retry mode err : %d !\012\000"
.LC445:
	.ascii	"sandisk A19 read retry conut: %d !\012\000"
	.section	.rodata.sandisk_A19_special_exit.str1.1,"aMS",%progbits,1
.LC453:
	.ascii	"%s sandisk toggle vendor specific setting failed!\012"
	.ascii	"\000"
.LC454:
	.ascii	" sandisk_A19_special_exit \012\000"
	.section	.rodata.sandisk_A19_special_init.str1.1,"aMS",%progbits,1
.LC451:
	.ascii	" sandisk_A19_special_init sandisk_A19_read_retry_mo"
	.ascii	"de:%d sandisk_A19_read_retry_cycle :%d sandisk_A19_"
	.ascii	"read_retry_reg_cnt %d \012\000"
.LC452:
	.ascii	" sandisk_A19_special_init error sandisk_A19_read_re"
	.ascii	"try_mode:%d sandisk_A19_read_retry_cycle :%d sandis"
	.ascii	"k_A19_read_retry_reg_cnt %d \012\000"
	.section	.rodata.sandisk_activation_seq.str1.1,"aMS",%progbits,1
.LC473:
	.ascii	"sandisk activation seq fail 0x%x\012\000"
	.section	.rodata.sandisk_read_page_end.str1.1,"aMS",%progbits,1
.LC475:
	.ascii	"sandisk retry!\012\000"
.LC476:
	.ascii	"sandisk ReadRetry ok! ch =%d, chip = %d  block = %d"
	.ascii	", page = %d, RetryCount = %d  \012\000"
	.section	.rodata.sandisk_readretry_init.str1.1,"aMS",%progbits,1
.LC474:
	.ascii	"sandisk read retry conut: %d !\012\000"
	.section	.rodata.sandisk_special_exit.str1.1,"aMS",%progbits,1
.LC479:
	.ascii	" sandisk_special_exit \012\000"
	.section	.rodata.sandisk_special_init.str1.1,"aMS",%progbits,1
.LC477:
	.ascii	" sandisk_special_init sandisk_read_retry_mode:%d sa"
	.ascii	"ndisk_read_retry_cycle :%d sandisk_read_retry_reg_c"
	.ascii	"nt %d \012\000"
.LC478:
	.ascii	" sandisk_special_init error sandisk_read_retry_mode"
	.ascii	":%d sandisk_read_retry_cycle :%d sandisk_read_retry"
	.ascii	"_reg_cnt %d \012\000"
	.section	.rodata.second_scan_all_blocks.str1.1,"aMS",%progbits,1
.LC273:
	.ascii	"[NI]NAND_EVENT: 2 current used block found %d,%d!\012"
	.ascii	"\000"
.LC274:
	.ascii	"[NE]no crosstalk:2 block used count diff %d,%d %d,%"
	.ascii	"d !\012\000"
.LC275:
	.ascii	"[NE]NAND_EVENT: muti current used block found1 %d %"
	.ascii	"d!\012\000"
.LC276:
	.ascii	"[NE]NAND_EVENT: muti current used block found2 %d %"
	.ascii	"d!\012\000"
.LC277:
	.ascii	"[NE]NAND_EVENT: muti current used block found3 %d %"
	.ascii	"d!\012\000"
.LC278:
	.ascii	"[NE]NAND_EVENT: last page spare data %x,%x,%x,%x,%x"
	.ascii	",%x,%x %x %x %x %x %x %x %x %x %x!!\012\000"
.LC279:
	.ascii	"[NI]zone@%d b@%d last page unkown ecc ok.\012\000"
.LC280:
	.ascii	"[NE]zone@%d b@%d last page ecc error.\012\000"
.LC281:
	.ascii	"[NE]something strange %d,%d!!\012\000"
.LC282:
	.ascii	"[NI]all block full!!\012\000"
.LC283:
	.ascii	"[ND]corss talk rebuild 0 %d %d!!\012\000"
.LC284:
	.ascii	"[NE]1st current block:%d 1st current block uc:%d mu"
	.ascii	"ti current used block 1st.\012\000"
.LC285:
	.ascii	"[ND]2nd current block:%d 2nd current block uc:%d mu"
	.ascii	"ti current used block 2nd.\012\000"
.LC286:
	.ascii	"[NE]something is error here 7!\012\000"
.LC287:
	.ascii	"[NE]p_phy_block_info_last %d %d.\012\000"
.LC288:
	.ascii	"[NE]get_current_used_block fail.\012\000"
.LC289:
	.ascii	"[ND]free block nums is %d, aec: %d, iec: %d, a_buc:"
	.ascii	" %d, i_buc: %d!\012\000"
	.section	.rodata.shutdown_on_gc.str1.1,"aMS",%progbits,1
.LC324:
	.ascii	"[ND]%s %p %d %d %d %\012\000"
.LC325:
	.ascii	"[NI]shutdown_on_gc end! [%d %d %d]\012\000"
.LC326:
	.ascii	"[ND]shutdown_on_gc one ok! [%d %d %d]\012\000"
	.section	.rodata.torture_block_and_mark.str1.1,"aMS",%progbits,1
.LC46:
	.ascii	"[NI]torture b@%d.\012\000"
.LC47:
	.ascii	"[NE]%s write b@%d p@%d err\012\000"
.LC48:
	.ascii	"[NE]%s read b@%d p@%d err\012\000"
.LC49:
	.ascii	"[NE]%s b@%d p@%d patt %02x checking main data fail\012"
	.ascii	"\000"
.LC50:
	.ascii	"[NE]%s b@%d p@%d patt %02x checking spare data fail"
	.ascii	"\012\000"
.LC51:
	.ascii	"[NE][NE]erase_block block fail %d!\012\000"
	.section	.rodata.toshiba_read_page_end.str1.1,"aMS",%progbits,1
.LC460:
	.ascii	"toshiba retry!\012\000"
.LC461:
	.ascii	"toshiba ReadRetry ok! ch =%d, chip = %d  block = %d"
	.ascii	", page = %d, RetryCount = %d  \012\000"
	.section	.rodata.toshiba_readretry_init.str1.1,"aMS",%progbits,1
.LC459:
	.ascii	"toshiba read retry conut: %d !\012\000"
	.section	.rodata.toshiba_special_exit.str1.1,"aMS",%progbits,1
.LC464:
	.ascii	" toshiba_special_exit \012\000"
	.section	.rodata.toshiba_special_init.str1.1,"aMS",%progbits,1
.LC462:
	.ascii	" toshiba_special_init toshiba_read_retry_mode :%d t"
	.ascii	"oshiba_read_retry_cycle :%d toshiba_read_retry_reg_"
	.ascii	"cnt :%d \012\000"
.LC463:
	.ascii	" toshiba_special_init error toshiba_read_retry_mode"
	.ascii	" :%d toshiba_read_retry_cycle :%d toshiba_read_retr"
	.ascii	"y_reg_cnt :%d \012\000"
	.section	.rodata.toshiba_vender_pre_condition.str1.1,"aMS",%progbits,1
.LC458:
	.ascii	"toshiba vender_pre_condition error!\012\000"
	.section	.rodata.toshiba_vender_set_param.str1.1,"aMS",%progbits,1
.LC457:
	.ascii	"toshiba rr value 3 %x %x %x %x!\012\000"
	.section	.rodata.write_data_to_nand.str1.1,"aMS",%progbits,1
.LC254:
	.ascii	"[NE][NE]%s:malloc 512 fail!\012\000"
	.section	.rodata.write_data_to_page.str1.1,"aMS",%progbits,1
.LC198:
	.ascii	"[NE]write logic data to current block error\012\000"
	.section	.rodata.write_logic_data_to_current_block.str1.1,"aMS",%progbits,1
.LC193:
	.ascii	"[ND]logic_page_map NULL!!\012\000"
.LC194:
	.ascii	"[ND]invalid_page_count more than pages_per_blk at w"
	.ascii	"rite logic data to current block:%d!!\012\000"
.LC195:
	.ascii	"[NE]write_phy_data_to_current_block error2\012\000"
.LC196:
	.ascii	"[NE]invalid_page_count more than pages_per_blk at w"
	.ascii	"rite logic data to current block:%d!!\012\000"
.LC197:
	.ascii	"[NE]write phy data to current block error2\012\000"
	.section	.rodata.write_phy_data_to_current_block.str1.1,"aMS",%progbits,1
.LC189:
	.ascii	"[ND]last block can not used to logic page %d!!\012\000"
.LC190:
	.ascii	"[ND]last block can not used to power down page %d!!"
	.ascii	"\012\000"
.LC191:
	.ascii	"[ND]write_phy_data_to_current_block write error %d!"
	.ascii	"\012\000"
.LC192:
	.ascii	"[NE]do_write_error error\012\000"
	.section	.rodata.write_phy_page_map_to_current_block.str1.1,"aMS",%progbits,1
.LC239:
	.ascii	"[NE]write_phy_data_to_current_block error1\012\000"
	.section	.rodata.zone_param_init.str1.1,"aMS",%progbits,1
.LC79:
	.ascii	"[NE]%s:malloc fail for temp_page_buf\012\000"
.LC80:
	.ascii	"[NE]%s:malloc fail for logic_page_buf\012\000"
.LC81:
	.ascii	"[NE]zone_param_init error2\012\000"
.LC82:
	.ascii	"[NE]%s:malloc fail for current_block.user_info.buf\012"
	.ascii	"\000"
.LC83:
	.ascii	"[NE]%s:malloc fail for assist_block.user_info.buf\012"
	.ascii	"\000"
.LC84:
	.ascii	"[NE]%s:malloc fail for zone_phy_page_map_for_gc.buf"
	.ascii	"\012\000"
	.ident	"GCC: (Linaro GCC 7.2-2017.11) 7.2.1 20171011"
	.section	.note.GNU-stack,"",%progbits
