/*SPDX-Licensen-Identifier: GPL-2.0*/

	.arch armv8-a
	.file	"nftl.c"
	.global	com
	.section	.rodata
	.align	3
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
	.global	mod
	.align	3
	.type	mod, %object
	.size	mod, 6
mod:
	.byte	109
	.byte	111
	.byte	100
	.byte	101
	.byte	108
	.byte	0
	.global	nftl_blk_head
	.bss
	.align	3
	.type	nftl_blk_head, %object
	.size	nftl_blk_head, 136
nftl_blk_head:
	.zero	136
	.section	.rodata
	.align	3
.LC3:
	.string	"[NE]nftl_build_all error!\n"
	.text
	.align	2
	.global	nftl_build_all
	.type	nftl_build_all, %function
nftl_build_all:
	stp	x29, x30, [sp, -48]!
	add	x29, sp, 0
	str	x0, [x29, 24]
	ldr	x0, [x29, 24]
	ldr	x0, [x0, 72]
	str	x0, [x29, 40]
	adrp	x0, nftl_blk_head
	add	x0, x0, :lo12:nftl_blk_head
	str	xzr, [x0, 16]
	b	.L2
.L5:
	ldr	x0, [x29, 40]
	bl	nftl_add
	cmp	x0, 0
	bne	.L3
	adrp	x0, .LC3
	add	x0, x0, :lo12:.LC3
	bl	nand_print
	mov	w0, 1
	b	.L4
.L3:
	ldr	x0, [x29, 40]
	ldr	x0, [x0, 64]
	str	x0, [x29, 40]
.L2:
	ldr	x0, [x29, 40]
	cmp	x0, 0
	bne	.L5
	mov	w0, 0
.L4:
	ldp	x29, x30, [sp], 48
	ret
	.size	nftl_build_all, .-nftl_build_all
	.align	2
	.global	nftl_build_one
	.type	nftl_build_one, %function
nftl_build_one:
	stp	x29, x30, [sp, -48]!
	add	x29, sp, 0
	str	x0, [x29, 24]
	str	w1, [x29, 20]
	ldr	x0, [x29, 24]
	ldr	x0, [x0, 72]
	str	x0, [x29, 40]
	ldr	w0, [x29, 20]
	cmp	w0, 0
	bne	.L8
	adrp	x0, nftl_blk_head
	add	x0, x0, :lo12:nftl_blk_head
	str	xzr, [x0, 16]
	b	.L8
.L11:
	ldr	x0, [x29, 40]
	ldrh	w0, [x0]
	mov	w1, w0
	ldr	w0, [x29, 20]
	cmp	w1, w0
	bne	.L9
	ldr	x0, [x29, 40]
	bl	nftl_add
	cmp	x0, 0
	bne	.L9
	adrp	x0, .LC3
	add	x0, x0, :lo12:.LC3
	bl	nand_print
	mov	w0, 1
	b	.L10
.L9:
	ldr	x0, [x29, 40]
	ldr	x0, [x0, 64]
	str	x0, [x29, 40]
.L8:
	ldr	x0, [x29, 40]
	cmp	x0, 0
	bne	.L11
	mov	w0, 0
.L10:
	ldp	x29, x30, [sp], 48
	ret
	.size	nftl_build_one, .-nftl_build_one
	.section	.rodata
	.align	3
.LC4:
	.string	"[NE]%s:malloc fail for nftl_blk\n"
	.align	3
.LC5:
	.string	"[NE]nftl_initialize failed\n"
	.align	3
.LC6:
	.string	"[ND]nftl_add ok\n"
	.text
	.align	2
	.global	nftl_add
	.type	nftl_add, %function
nftl_add:
	stp	x29, x30, [sp, -48]!
	add	x29, sp, 0
	str	x0, [x29, 24]
	mov	w0, 136
	bl	nftl_malloc
	str	x0, [x29, 40]
	ldr	x0, [x29, 40]
	cmp	x0, 0
	bne	.L13
	adrp	x0, __func__.5194
	add	x1, x0, :lo12:__func__.5194
	adrp	x0, .LC4
	add	x0, x0, :lo12:.LC4
	bl	nand_print
	mov	x0, 0
	b	.L14
.L13:
	ldr	x0, [x29, 24]
	bl	build_nand_partition
	mov	x1, x0
	ldr	x0, [x29, 40]
	str	x1, [x0, 8]
	ldr	x0, [x29, 24]
	ldrh	w0, [x0]
	mov	w1, w0
	ldr	x0, [x29, 40]
	bl	nftl_initialize
	cmp	w0, 0
	beq	.L15
	adrp	x0, .LC5
	add	x0, x0, :lo12:.LC5
	bl	nand_print
	mov	x0, 0
	b	.L14
.L15:
	adrp	x0, nftl_blk_head
	add	x0, x0, :lo12:nftl_blk_head
	ldr	x1, [x29, 40]
	bl	add_nftl_blk_list
	ldr	x0, [x29, 40]
	bl	panic_recover_data
	adrp	x0, .LC6
	add	x0, x0, :lo12:.LC6
	bl	nand_print_dbg
	ldr	x0, [x29, 40]
.L14:
	ldp	x29, x30, [sp], 48
	ret
	.size	nftl_add, .-nftl_add
	.align	2
	.global	nand_ftl_exit
	.type	nand_ftl_exit, %function
nand_ftl_exit:
	stp	x29, x30, [sp, -32]!
	add	x29, sp, 0
	adrp	x0, nftl_blk_head
	add	x0, x0, :lo12:nftl_blk_head
	bl	del_last_nftl_blk
	str	x0, [x29, 24]
	b	.L17
.L18:
	ldr	x0, [x29, 24]
	bl	nftl_exit
	ldr	x0, [x29, 24]
	bl	nftl_free
	adrp	x0, nftl_blk_head
	add	x0, x0, :lo12:nftl_blk_head
	bl	del_last_nftl_blk
	str	x0, [x29, 24]
.L17:
	ldr	x0, [x29, 24]
	cmp	x0, 0
	bne	.L18
	mov	w0, 0
	ldp	x29, x30, [sp], 32
	ret
	.size	nand_ftl_exit, .-nand_ftl_exit
	.section	.rodata
	.align	3
.LC7:
	.string	"[ND]cap: 0x%08x.\n"
	.text
	.align	2
	.global	print_all_nftl_blk
	.type	print_all_nftl_blk, %function
print_all_nftl_blk:
	stp	x29, x30, [sp, -32]!
	add	x29, sp, 0
	adrp	x0, nftl_blk_head
	add	x0, x0, :lo12:nftl_blk_head
	str	x0, [x29, 24]
	b	.L21
.L22:
	ldr	x0, [x29, 24]
	ldr	x0, [x0, 16]
	ldr	w1, [x0]
	adrp	x0, .LC7
	add	x0, x0, :lo12:.LC7
	bl	nand_print_dbg
	ldr	x0, [x29, 24]
	ldr	x0, [x0, 16]
	str	x0, [x29, 24]
.L21:
	ldr	x0, [x29, 24]
	ldr	x0, [x0, 16]
	cmp	x0, 0
	bne	.L22
	nop
	ldp	x29, x30, [sp], 32
	ret
	.size	print_all_nftl_blk, .-print_all_nftl_blk
	.align	2
	.global	get_nftl_num
	.type	get_nftl_num, %function
get_nftl_num:
	sub	sp, sp, #16
	str	wzr, [sp, 12]
	adrp	x0, nftl_blk_head
	add	x0, x0, :lo12:nftl_blk_head
	str	x0, [sp]
	b	.L24
.L25:
	ldr	w0, [sp, 12]
	add	w0, w0, 1
	str	w0, [sp, 12]
	ldr	x0, [sp]
	ldr	x0, [x0, 16]
	str	x0, [sp]
.L24:
	ldr	x0, [sp]
	ldr	x0, [x0, 16]
	cmp	x0, 0
	bne	.L25
	ldr	w0, [sp, 12]
	add	sp, sp, 16
	ret
	.size	get_nftl_num, .-get_nftl_num
	.align	2
	.global	get_nftl_cap
	.type	get_nftl_cap, %function
get_nftl_cap:
	sub	sp, sp, #16
	str	wzr, [sp, 12]
	adrp	x0, nftl_blk_head
	add	x0, x0, :lo12:nftl_blk_head
	str	x0, [sp]
	b	.L28
.L29:
	ldr	x0, [sp]
	ldr	x0, [x0, 16]
	ldr	w0, [x0]
	ldr	w1, [sp, 12]
	add	w0, w1, w0
	str	w0, [sp, 12]
	ldr	x0, [sp]
	ldr	x0, [x0, 16]
	str	x0, [sp]
.L28:
	ldr	x0, [sp]
	ldr	x0, [x0, 16]
	cmp	x0, 0
	bne	.L29
	ldr	w0, [sp, 12]
	add	sp, sp, 16
	ret
	.size	get_nftl_cap, .-get_nftl_cap
	.align	2
	.global	get_first_nftl_cap
	.type	get_first_nftl_cap, %function
get_first_nftl_cap:
	sub	sp, sp, #16
	adrp	x0, nftl_blk_head
	add	x0, x0, :lo12:nftl_blk_head
	str	x0, [sp, 8]
	ldr	x0, [sp, 8]
	ldr	x0, [x0, 16]
	ldr	w0, [x0]
	add	sp, sp, 16
	ret
	.size	get_first_nftl_cap, .-get_first_nftl_cap
	.section	.rodata
	.align	3
.LC8:
	.string	"[NE]parameter error %d,%d !\n"
	.text
	.align	2
	.global	nftl_read
	.type	nftl_read, %function
nftl_read:
	stp	x29, x30, [sp, -48]!
	add	x29, sp, 0
	str	w0, [x29, 28]
	str	w1, [x29, 24]
	str	x2, [x29, 16]
	adrp	x0, nftl_blk_head
	add	x0, x0, :lo12:nftl_blk_head
	ldr	x0, [x0, 16]
	str	x0, [x29, 40]
	ldr	w0, [x29, 24]
	cmp	w0, 0
	bne	.L36
	mov	w0, 0
	b	.L35
.L37:
	ldr	x0, [x29, 40]
	ldr	w0, [x0]
	ldr	w1, [x29, 28]
	sub	w0, w1, w0
	str	w0, [x29, 28]
	ldr	x0, [x29, 40]
	ldr	x0, [x0, 16]
	str	x0, [x29, 40]
	ldr	x0, [x29, 40]
	cmp	x0, 0
	bne	.L36
	adrp	x0, .LC8
	add	x0, x0, :lo12:.LC8
	ldr	w2, [x29, 24]
	ldr	w1, [x29, 28]
	bl	nand_print
	mov	w0, 1
	b	.L35
.L36:
	ldr	x0, [x29, 40]
	ldr	w1, [x0]
	ldr	w0, [x29, 28]
	cmp	w1, w0
	bls	.L37
	ldr	x0, [x29, 40]
	ldr	x4, [x0, 72]
	ldr	x3, [x29, 16]
	ldr	w2, [x29, 24]
	ldr	w1, [x29, 28]
	ldr	x0, [x29, 40]
	blr	x4
.L35:
	ldp	x29, x30, [sp], 48
	ret
	.size	nftl_read, .-nftl_read
	.align	2
	.global	nftl_write
	.type	nftl_write, %function
nftl_write:
	stp	x29, x30, [sp, -48]!
	add	x29, sp, 0
	str	w0, [x29, 28]
	str	w1, [x29, 24]
	str	x2, [x29, 16]
	adrp	x0, nftl_blk_head
	add	x0, x0, :lo12:nftl_blk_head
	ldr	x0, [x0, 16]
	str	x0, [x29, 40]
	ldr	w0, [x29, 24]
	cmp	w0, 0
	bne	.L41
	mov	w0, 0
	b	.L40
.L42:
	ldr	x0, [x29, 40]
	ldr	w0, [x0]
	ldr	w1, [x29, 28]
	sub	w0, w1, w0
	str	w0, [x29, 28]
	ldr	x0, [x29, 40]
	ldr	x0, [x0, 16]
	str	x0, [x29, 40]
	ldr	x0, [x29, 40]
	cmp	x0, 0
	bne	.L41
	adrp	x0, .LC8
	add	x0, x0, :lo12:.LC8
	ldr	w2, [x29, 24]
	ldr	w1, [x29, 28]
	bl	nand_print
	mov	w0, 1
	b	.L40
.L41:
	ldr	x0, [x29, 40]
	ldr	w1, [x0]
	ldr	w0, [x29, 28]
	cmp	w1, w0
	bls	.L42
	ldr	x0, [x29, 40]
	ldr	x4, [x0, 80]
	ldr	x3, [x29, 16]
	ldr	w2, [x29, 24]
	ldr	w1, [x29, 28]
	ldr	x0, [x29, 40]
	blr	x4
.L40:
	ldp	x29, x30, [sp], 48
	ret
	.size	nftl_write, .-nftl_write
	.align	2
	.global	nftl_discard
	.type	nftl_discard, %function
nftl_discard:
	stp	x29, x30, [sp, -48]!
	add	x29, sp, 0
	str	w0, [x29, 28]
	str	w1, [x29, 24]
	adrp	x0, nftl_blk_head
	add	x0, x0, :lo12:nftl_blk_head
	ldr	x0, [x0, 16]
	str	x0, [x29, 40]
	ldr	w0, [x29, 24]
	cmp	w0, 0
	bne	.L46
	mov	w0, 0
	b	.L45
.L47:
	ldr	x0, [x29, 40]
	ldr	w0, [x0]
	ldr	w1, [x29, 28]
	sub	w0, w1, w0
	str	w0, [x29, 28]
	ldr	x0, [x29, 40]
	ldr	x0, [x0, 16]
	str	x0, [x29, 40]
	ldr	x0, [x29, 40]
	cmp	x0, 0
	bne	.L46
	adrp	x0, .LC8
	add	x0, x0, :lo12:.LC8
	ldr	w2, [x29, 24]
	ldr	w1, [x29, 28]
	bl	nand_print
	mov	w0, 1
	b	.L45
.L46:
	ldr	x0, [x29, 40]
	ldr	w1, [x0]
	ldr	w0, [x29, 28]
	cmp	w1, w0
	bls	.L47
	ldr	x0, [x29, 40]
	ldr	x3, [x0, 96]
	ldr	w2, [x29, 24]
	ldr	w1, [x29, 28]
	ldr	x0, [x29, 40]
	blr	x3
.L45:
	ldp	x29, x30, [sp], 48
	ret
	.size	nftl_discard, .-nftl_discard
	.align	2
	.global	nftl_flush_write_cache
	.type	nftl_flush_write_cache, %function
nftl_flush_write_cache:
	stp	x29, x30, [sp, -32]!
	add	x29, sp, 0
	adrp	x0, nftl_blk_head
	add	x0, x0, :lo12:nftl_blk_head
	ldr	x0, [x0, 16]
	str	x0, [x29, 24]
	b	.L49
.L50:
	ldr	x0, [x29, 24]
	ldr	x2, [x0, 88]
	mov	w1, 65535
	ldr	x0, [x29, 24]
	blr	x2
	ldr	x0, [x29, 24]
	ldr	x0, [x0, 16]
	str	x0, [x29, 24]
.L49:
	ldr	x0, [x29, 24]
	cmp	x0, 0
	bne	.L50
	mov	w0, 0
	ldp	x29, x30, [sp], 32
	ret
	.size	nftl_flush_write_cache, .-nftl_flush_write_cache
	.align	2
	.global	nftl_write_end
	.type	nftl_write_end, %function
nftl_write_end:
	stp	x29, x30, [sp, -32]!
	add	x29, sp, 0
	adrp	x0, nftl_blk_head
	add	x0, x0, :lo12:nftl_blk_head
	ldr	x0, [x0, 16]
	str	x0, [x29, 24]
	b	.L53
.L54:
	ldr	x0, [x29, 24]
	ldr	x0, [x0, 24]
	mov	w1, 8
	bl	fill_no_use_page_to_current_block
	ldr	x0, [x29, 24]
	ldr	x0, [x0, 16]
	str	x0, [x29, 24]
.L53:
	ldr	x0, [x29, 24]
	cmp	x0, 0
	bne	.L54
	mov	w0, 0
	ldp	x29, x30, [sp], 32
	ret
	.size	nftl_write_end, .-nftl_write_end
	.global	phyinfo
	.bss
	.align	3
	.type	phyinfo, %object
	.size	phyinfo, 20
phyinfo:
	.zero	20
	.section	.rodata
	.align	3
.LC9:
	.string	"[NE]malloc size too large %d!\n"
	.text
	.align	2
	.global	nftl_malloc
	.type	nftl_malloc, %function
nftl_malloc:
	stp	x29, x30, [sp, -32]!
	add	x29, sp, 0
	str	w0, [x29, 28]
	ldr	w0, [x29, 28]
	cmp	w0, 1572864
	bls	.L57
	adrp	x0, .LC9
	add	x0, x0, :lo12:.LC9
	ldr	w1, [x29, 28]
	bl	nand_print
.L57:
	ldr	w0, [x29, 28]
	bl	nand_malloc
	ldp	x29, x30, [sp], 32
	ret
	.size	nftl_malloc, .-nftl_malloc
	.align	2
	.global	nftl_free
	.type	nftl_free, %function
nftl_free:
	stp	x29, x30, [sp, -32]!
	add	x29, sp, 0
	str	x0, [x29, 24]
	ldr	x0, [x29, 24]
	bl	nand_free
	nop
	ldp	x29, x30, [sp], 32
	ret
	.size	nftl_free, .-nftl_free
	.align	2
	.global	add_nftl_blk_list
	.type	add_nftl_blk_list, %function
add_nftl_blk_list:
	sub	sp, sp, #32
	str	x0, [sp, 8]
	str	x1, [sp]
	ldr	x0, [sp, 8]
	str	x0, [sp, 24]
	ldr	x0, [sp]
	str	xzr, [x0, 16]
	b	.L61
.L62:
	ldr	x0, [sp, 24]
	ldr	x0, [x0, 16]
	str	x0, [sp, 24]
.L61:
	ldr	x0, [sp, 24]
	ldr	x0, [x0, 16]
	cmp	x0, 0
	bne	.L62
	ldr	x0, [sp, 24]
	ldr	x1, [sp]
	str	x1, [x0, 16]
	nop
	add	sp, sp, 32
	ret
	.size	add_nftl_blk_list, .-add_nftl_blk_list
	.align	2
	.global	del_last_nftl_blk
	.type	del_last_nftl_blk, %function
del_last_nftl_blk:
	sub	sp, sp, #32
	str	x0, [sp, 8]
	str	xzr, [sp, 16]
	ldr	x0, [sp, 8]
	str	x0, [sp, 24]
	b	.L64
.L67:
	ldr	x0, [sp, 24]
	ldr	x0, [x0, 16]
	str	x0, [sp, 16]
	ldr	x0, [sp, 16]
	ldr	x0, [x0, 16]
	cmp	x0, 0
	bne	.L65
	ldr	x0, [sp, 24]
	str	xzr, [x0, 16]
	ldr	x0, [sp, 16]
	b	.L66
.L65:
	ldr	x0, [sp, 24]
	ldr	x0, [x0, 16]
	str	x0, [sp, 24]
.L64:
	ldr	x0, [sp, 24]
	ldr	x0, [x0, 16]
	cmp	x0, 0
	bne	.L67
	mov	x0, 0
.L66:
	add	sp, sp, 32
	ret
	.size	del_last_nftl_blk, .-del_last_nftl_blk
	.align	2
	.global	nftl_support_boot_check_crc_from_dts
	.type	nftl_support_boot_check_crc_from_dts, %function
nftl_support_boot_check_crc_from_dts:
	stp	x29, x30, [sp, -16]!
	add	x29, sp, 0
	bl	nand_get_support_boot_check_crc
	cmp	w0, 0
	cset	w0, ne
	uxtb	w0, w0
	ldp	x29, x30, [sp], 16
	ret
	.size	nftl_support_boot_check_crc_from_dts, .-nftl_support_boot_check_crc_from_dts
	.section	.rodata
	.align	3
.LC10:
	.string	"17:53"
	.align	3
.LC11:
	.string	"2020.09.28"
	.align	3
.LC12:
	.string	"006"
	.align	3
.LC13:
	.string	"7"
	.align	3
.LC14:
	.string	"3"
	.align	3
.LC15:
	.string	"sunxi nftl version %s.%s.%s  %s-%s\n"
	.text
	.align	2
	.type	nftl_version, %function
nftl_version:
	stp	x29, x30, [sp, -16]!
	add	x29, sp, 0
	adrp	x0, .LC10
	add	x5, x0, :lo12:.LC10
	adrp	x0, .LC11
	add	x4, x0, :lo12:.LC11
	adrp	x0, .LC12
	add	x3, x0, :lo12:.LC12
	adrp	x0, .LC13
	add	x2, x0, :lo12:.LC13
	adrp	x0, .LC14
	add	x1, x0, :lo12:.LC14
	adrp	x0, .LC15
	add	x0, x0, :lo12:.LC15
	bl	nand_print
	nop
	ldp	x29, x30, [sp], 16
	ret
	.size	nftl_version, .-nftl_version
	.align	2
	.global	get_nftl_version
	.type	get_nftl_version, %function
get_nftl_version:
	stp	x29, x30, [sp, -64]!
	add	x29, sp, 0
	str	x0, [x29, 56]
	str	x1, [x29, 48]
	str	x2, [x29, 40]
	str	x3, [x29, 32]
	str	x4, [x29, 24]
	adrp	x0, .LC14
	add	x0, x0, :lo12:.LC14
	mov	w2, 2
	mov	x1, x0
	ldr	x0, [x29, 56]
	bl	nand_memcpy
	adrp	x0, .LC13
	add	x0, x0, :lo12:.LC13
	mov	w2, 2
	mov	x1, x0
	ldr	x0, [x29, 48]
	bl	nand_memcpy
	adrp	x0, .LC12
	add	x0, x0, :lo12:.LC12
	mov	w2, 4
	mov	x1, x0
	ldr	x0, [x29, 40]
	bl	nand_memcpy
	adrp	x0, .LC11
	add	x0, x0, :lo12:.LC11
	mov	w2, 11
	mov	x1, x0
	ldr	x0, [x29, 32]
	bl	nand_memcpy
	adrp	x0, .LC10
	add	x0, x0, :lo12:.LC10
	mov	w2, 6
	mov	x1, x0
	ldr	x0, [x29, 24]
	bl	nand_memcpy
	nop
	ldp	x29, x30, [sp], 64
	ret
	.size	get_nftl_version, .-get_nftl_version
	.section	.rodata
	.align	3
.LC16:
	.string	"[NE]%s:malloc fail for nftl_cfg\n"
	.align	3
.LC17:
	.string	"[ND]max_erase_times = %i\n"
	.text
	.align	2
	.global	nftl_initialize
	.type	nftl_initialize, %function
nftl_initialize:
	sub	sp, sp, #112
	stp	x29, x30, [sp, 32]
	add	x29, sp, 32
	str	x0, [x29, 24]
	str	w1, [x29, 20]
	ldr	x0, [x29, 24]
	ldr	x0, [x0, 8]
	str	x0, [x29, 72]
	str	xzr, [x29, 64]
	adrp	x0, .LC0
	add	x1, x0, :lo12:.LC0
	add	x0, x29, 48
	ldr	x2, [x1]
	str	x2, [x0]
	ldrb	w1, [x1, 8]
	strb	w1, [x0, 8]
	mov	w0, 47
	strb	w0, [x29, 40]
	strb	wzr, [x29, 41]
	adrp	x0, com
	add	x1, x0, :lo12:com
	add	x0, x29, 40
	mov	x2, x1
	mov	x1, x0
	mov	x0, 0
	bl	nand_fdt_get_prop_string_by_path
	str	x0, [x29, 64]
	ldr	x0, [x29, 64]
	cmp	x0, 0
	bne	.L73
	mov	w0, -1
	b	.L78
.L73:
	add	x0, x29, 48
	mov	x2, 9
	mov	x1, x0
	ldr	x0, [x29, 64]
	bl	nand_memcmp
	cmp	w0, 0
	beq	.L75
	mov	w0, -1
	b	.L78
.L75:
	bl	nftl_version
	adrp	x0, phyinfo
	add	x0, x0, :lo12:phyinfo
	bl	nftl_get_nand_phyinfo
	ldr	x0, [x29, 72]
	ldrh	w0, [x0, 34]
	cmp	w0, 15
	bhi	.L90
	mov	w0, -1
	b	.L78
.L90:
	mov	w0, 36
	bl	nftl_malloc
	mov	x1, x0
	ldr	x0, [x29, 24]
	str	x1, [x0, 32]
	ldr	x0, [x29, 24]
	ldr	x0, [x0, 32]
	cmp	x0, 0
	bne	.L79
	adrp	x0, __func__.5346
	add	x1, x0, :lo12:__func__.5346
	adrp	x0, .LC16
	add	x0, x0, :lo12:.LC16
	bl	nand_print
	mov	w0, 1
	b	.L78
.L79:
	ldr	x0, [x29, 24]
	ldr	x0, [x0, 32]
	strh	wzr, [x0]
	ldr	x0, [x29, 24]
	ldr	x0, [x0, 32]
	mov	w1, 1
	strh	w1, [x0, 2]
	ldr	x0, [x29, 24]
	ldr	x0, [x0, 32]
	mov	w1, 1
	strh	w1, [x0, 4]
	ldr	x0, [x29, 24]
	ldr	x0, [x0, 32]
	mov	w1, 1
	strh	w1, [x0, 6]
	ldr	x0, [x29, 24]
	ldr	x0, [x0, 32]
	strh	wzr, [x0, 8]
	ldr	x0, [x29, 24]
	ldr	x0, [x0, 32]
	strh	wzr, [x0, 10]
	ldr	x0, [x29, 24]
	ldr	x0, [x0, 32]
	ldr	x1, [x29, 72]
	ldr	x1, [x1, 80]
	ldr	x1, [x1, 40]
	ldrh	w1, [x1, 2498]
	strh	w1, [x0, 12]
	ldr	x0, [x29, 24]
	ldr	x0, [x0, 32]
	ldr	x1, [x29, 72]
	ldr	x1, [x1, 80]
	ldr	x1, [x1, 40]
	ldrh	w1, [x1, 2500]
	strh	w1, [x0, 16]
	ldr	x0, [x29, 24]
	ldr	x0, [x0, 32]
	mov	w1, 8
	strh	w1, [x0, 14]
	ldr	x0, [x29, 24]
	ldr	x0, [x0, 32]
	mov	w1, 2
	strh	w1, [x0, 18]
	ldr	x0, [x29, 24]
	ldr	x0, [x0, 32]
	mov	w1, 3
	strh	w1, [x0, 20]
	ldr	x0, [x29, 24]
	ldr	x0, [x0, 32]
	mov	w1, 50816
	movk	w1, 0x13, lsl 16
	str	w1, [x0, 32]
	ldr	x0, [x29, 72]
	ldr	x0, [x0, 80]
	ldr	x0, [x0, 40]
	ldrh	w0, [x0, 2494]
	cmp	w0, 0
	bne	.L80
	ldr	x0, [x29, 24]
	ldr	x0, [x0, 32]
	mov	w1, 30
	strh	w1, [x0, 22]
	ldr	x0, [x29, 24]
	ldr	x0, [x0, 32]
	mov	w1, 3
	strh	w1, [x0, 24]
	b	.L81
.L80:
	ldr	x0, [x29, 72]
	ldr	x0, [x0, 80]
	ldr	x0, [x0, 40]
	ldrh	w0, [x0, 2494]
	cmp	w0, 255
	bne	.L82
	ldr	x0, [x29, 24]
	ldr	x0, [x0, 32]
	mov	w1, 1
	strh	w1, [x0]
	ldr	x0, [x29, 24]
	ldr	x0, [x0, 32]
	mov	w1, 50
	strh	w1, [x0, 22]
	ldr	x0, [x29, 24]
	ldr	x0, [x0, 32]
	mov	w1, 40
	strh	w1, [x0, 24]
	b	.L81
.L82:
	ldr	x0, [x29, 72]
	ldr	x0, [x0, 80]
	ldr	x0, [x0, 40]
	ldrh	w0, [x0, 2494]
	cmp	w0, 10
	bls	.L83
	ldr	x0, [x29, 72]
	ldr	x0, [x0, 80]
	ldr	x0, [x0, 40]
	mov	w1, 10
	strh	w1, [x0, 2494]
	ldr	x0, [x29, 24]
	ldr	x0, [x0, 32]
	mov	w1, 30
	strh	w1, [x0, 22]
	ldr	x0, [x29, 24]
	ldr	x0, [x0, 32]
	mov	w1, 3
	strh	w1, [x0, 24]
	b	.L81
.L83:
	ldr	x0, [x29, 24]
	ldr	x0, [x0, 32]
	ldr	x1, [x29, 72]
	ldr	x1, [x1, 80]
	ldr	x1, [x1, 40]
	ldrh	w2, [x1, 2494]
	mov	w1, 50
	mul	w1, w2, w1
	uxth	w1, w1
	strh	w1, [x0, 22]
	ldr	x0, [x29, 24]
	ldr	x1, [x0, 32]
	ldr	x0, [x29, 72]
	ldr	x0, [x0, 80]
	ldr	x0, [x0, 40]
	ldrh	w0, [x0, 2494]
	mov	w2, w0
	mov	w0, w2
	ubfiz	w0, w0, 2, 14
	add	w0, w0, w2
	ubfiz	w0, w0, 3, 13
	uxth	w0, w0
	strh	w0, [x1, 24]
.L81:
	ldr	x0, [x29, 24]
	ldr	x0, [x0, 32]
	ldr	x1, [x29, 72]
	ldr	x1, [x1, 80]
	ldrh	w1, [x1, 2]
	strh	w1, [x0, 26]
	ldr	x0, [x29, 24]
	ldr	x0, [x0, 32]
	ldrh	w0, [x0, 26]
	cmp	w0, 0
	beq	.L84
	ldr	x0, [x29, 24]
	ldr	x0, [x0, 32]
	mov	w1, 3
	strh	w1, [x0, 24]
	b	.L85
.L84:
	ldr	x0, [x29, 24]
	ldr	x0, [x0, 32]
	mov	w1, 1
	strh	w1, [x0, 24]
.L85:
	ldr	x0, [x29, 24]
	ldr	x0, [x0, 32]
	ldrh	w0, [x0, 22]
	cmp	w0, 30
	bls	.L86
	ldr	x0, [x29, 24]
	ldr	x0, [x0, 32]
	mov	w1, 30
	strh	w1, [x0, 22]
.L86:
	ldr	x0, [x29, 24]
	ldr	x0, [x0, 32]
	mov	w1, 1
	strh	w1, [x0, 24]
	ldr	x0, [x29, 72]
	ldr	x0, [x0, 80]
	ldr	x0, [x0, 40]
	ldr	w1, [x0, 2512]
	mov	w0, 45752
	movk	w0, 0x76, lsl 16
	cmp	w1, w0
	bhi	.L87
	ldr	x0, [x29, 72]
	ldr	x0, [x0, 80]
	ldr	x0, [x0, 40]
	ldr	w1, [x0, 2512]
	mov	w0, 20863
	movk	w0, 0x1, lsl 16
	cmp	w1, w0
	bls	.L87
	ldr	x0, [x29, 24]
	ldr	x0, [x0, 32]
	ldr	x1, [x29, 72]
	ldr	x1, [x1, 80]
	ldr	x1, [x1, 40]
	ldr	w1, [x1, 2512]
	str	w1, [x0, 32]
.L87:
	ldr	x0, [x29, 24]
	ldr	x0, [x0, 32]
	mov	w1, 1
	strh	w1, [x0, 28]
	bl	nftl_support_boot_check_crc_from_dts
	cmp	w0, 0
	bne	.L88
	ldr	x0, [x29, 24]
	ldr	x0, [x0, 32]
	strh	wzr, [x0, 28]
.L88:
	ldr	x0, [x29, 24]
	ldr	x8, [x0, 32]
	ldr	x0, [x29, 24]
	add	x2, x0, 24
	ldr	x0, [x29, 72]
	ldr	x3, [x0, 64]
	ldr	x0, [x29, 72]
	ldr	x4, [x0, 72]
	ldr	x0, [x29, 72]
	ldr	w5, [x0, 44]
	ldr	x0, [x29, 72]
	ldrh	w0, [x0, 40]
	mov	w6, w0
	ldr	x0, [x29, 72]
	ldrh	w0, [x0, 34]
	mov	w7, w0
	ldr	x0, [x29, 72]
	strb	wzr, [sp, 16]
	ldr	w1, [x29, 20]
	str	w1, [sp, 8]
	str	x0, [sp]
	mov	x1, x8
	ldr	x0, [x29, 24]
	bl	nftl_start
	str	w0, [x29, 60]
	ldr	w0, [x29, 60]
	cmp	w0, 0
	beq	.L89
	ldr	w0, [x29, 60]
	b	.L78
.L89:
	ldr	x0, [x29, 24]
	ldr	x0, [x0, 24]
	ldr	x0, [x0, 8]
	ldr	x1, [x29, 72]
	ldr	x1, [x1, 80]
	ldr	x1, [x1, 40]
	ldr	w1, [x1, 2504]
	uxth	w1, w1
	strh	w1, [x0, 16]
	ldr	x0, [x29, 24]
	ldr	x0, [x0, 24]
	ldr	x0, [x0, 8]
	ldr	x1, [x29, 72]
	ldr	x1, [x1, 80]
	ldr	x1, [x1, 40]
	ldr	w1, [x1, 2508]
	uxth	w1, w1
	strh	w1, [x0, 30]
	ldr	x0, [x29, 24]
	ldr	x0, [x0, 24]
	ldr	x0, [x0, 8]
	ldrh	w0, [x0, 16]
	mov	w1, w0
	adrp	x0, .LC17
	add	x0, x0, :lo12:.LC17
	bl	nand_print_dbg
	ldr	x0, [x29, 24]
	ldr	x0, [x0, 24]
	bl	nftl_get_zone_cap
	mov	w1, w0
	ldr	x0, [x29, 24]
	str	w1, [x0]
	ldr	x0, [x29, 24]
	adrp	x1, _nand_read
	add	x1, x1, :lo12:_nand_read
	str	x1, [x0, 72]
	ldr	x0, [x29, 24]
	adrp	x1, _nand_write
	add	x1, x1, :lo12:_nand_write
	str	x1, [x0, 80]
	ldr	x0, [x29, 24]
	adrp	x1, _nand_discard
	add	x1, x1, :lo12:_nand_discard
	str	x1, [x0, 96]
	ldr	x0, [x29, 24]
	adrp	x1, _nand_flush_write_cache
	add	x1, x1, :lo12:_nand_flush_write_cache
	str	x1, [x0, 88]
	ldr	x0, [x29, 24]
	adrp	x1, _shutdown_op
	add	x1, x1, :lo12:_shutdown_op
	str	x1, [x0, 104]
	ldr	x0, [x29, 24]
	adrp	x1, _nand_sector_read
	add	x1, x1, :lo12:_nand_sector_read
	str	x1, [x0, 112]
	ldr	x0, [x29, 24]
	adrp	x1, _nand_sector_write
	add	x1, x1, :lo12:_nand_sector_write
	str	x1, [x0, 120]
	ldr	x0, [x29, 24]
	adrp	x1, _nand_flush_sector_write_cache
	add	x1, x1, :lo12:_nand_flush_sector_write_cache
	str	x1, [x0, 128]
	ldr	x0, [x29, 24]
	str	xzr, [x0, 16]
	mov	w0, 0
.L78:
	add	sp, x29, 0
	ldp	x29, x30, [sp], 80
	ret
	.size	nftl_initialize, .-nftl_initialize
	.section	.rodata
	.align	3
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
	.text
	.align	2
	.global	nftl_exit
	.type	nftl_exit, %function
nftl_exit:
	stp	x29, x30, [sp, -32]!
	add	x29, sp, 0
	str	x0, [x29, 24]
	ldr	x0, [x29, 24]
	ldr	x0, [x0, 8]
	bl	free_nand_partition
	ldr	x0, [x29, 24]
	ldr	x0, [x0, 24]
	bl	nftl_zone_release
	ldr	x0, [x29, 24]
	ldr	x0, [x0, 32]
	bl	nftl_free
	mov	w0, 0
	ldp	x29, x30, [sp], 32
	ret
	.size	nftl_exit, .-nftl_exit
	.align	2
	.global	_nand_read
	.type	_nand_read, %function
_nand_read:
	stp	x29, x30, [sp, -64]!
	add	x29, sp, 0
	str	x0, [x29, 40]
	str	w1, [x29, 36]
	str	w2, [x29, 32]
	str	x3, [x29, 24]
	str	wzr, [x29, 60]
	ldr	x0, [x29, 40]
	ldr	x0, [x0, 24]
	ldr	x3, [x29, 24]
	ldr	w2, [x29, 32]
	ldr	w1, [x29, 36]
	bl	__nand_read
	str	w0, [x29, 60]
	ldr	w0, [x29, 60]
	ldp	x29, x30, [sp], 64
	ret
	.size	_nand_read, .-_nand_read
	.align	2
	.global	_nand_write
	.type	_nand_write, %function
_nand_write:
	stp	x29, x30, [sp, -64]!
	add	x29, sp, 0
	str	x0, [x29, 40]
	str	w1, [x29, 36]
	str	w2, [x29, 32]
	str	x3, [x29, 24]
	str	wzr, [x29, 60]
	ldr	x0, [x29, 40]
	ldr	x0, [x0, 24]
	ldr	x3, [x29, 24]
	ldr	w2, [x29, 32]
	ldr	w1, [x29, 36]
	bl	__nand_write
	str	w0, [x29, 60]
	ldr	w0, [x29, 60]
	ldp	x29, x30, [sp], 64
	ret
	.size	_nand_write, .-_nand_write
	.align	2
	.global	_nand_discard
	.type	_nand_discard, %function
_nand_discard:
	stp	x29, x30, [sp, -48]!
	add	x29, sp, 0
	str	x0, [x29, 24]
	str	w1, [x29, 20]
	str	w2, [x29, 16]
	str	wzr, [x29, 44]
	ldr	x0, [x29, 24]
	ldr	x0, [x0, 24]
	ldr	w2, [x29, 16]
	ldr	w1, [x29, 20]
	bl	__nand_discard
	str	w0, [x29, 44]
	ldr	w0, [x29, 44]
	ldp	x29, x30, [sp], 48
	ret
	.size	_nand_discard, .-_nand_discard
	.align	2
	.global	_nand_flush_write_cache
	.type	_nand_flush_write_cache, %function
_nand_flush_write_cache:
	stp	x29, x30, [sp, -48]!
	add	x29, sp, 0
	str	x0, [x29, 24]
	str	w1, [x29, 20]
	str	wzr, [x29, 44]
	ldr	x0, [x29, 24]
	ldr	x0, [x0, 24]
	ldr	w1, [x29, 20]
	bl	__nand_flush_write_cache
	str	w0, [x29, 44]
	ldr	w0, [x29, 44]
	ldp	x29, x30, [sp], 48
	ret
	.size	_nand_flush_write_cache, .-_nand_flush_write_cache
	.align	2
	.global	_nand_sector_read
	.type	_nand_sector_read, %function
_nand_sector_read:
	sub	sp, sp, #48
	str	x0, [sp, 24]
	str	w1, [sp, 20]
	str	w2, [sp, 16]
	str	x3, [sp, 8]
	str	wzr, [sp, 44]
	ldr	w0, [sp, 44]
	add	sp, sp, 48
	ret
	.size	_nand_sector_read, .-_nand_sector_read
	.align	2
	.global	_nand_sector_write
	.type	_nand_sector_write, %function
_nand_sector_write:
	sub	sp, sp, #48
	str	x0, [sp, 24]
	str	w1, [sp, 20]
	str	w2, [sp, 16]
	str	x3, [sp, 8]
	str	wzr, [sp, 44]
	ldr	w0, [sp, 44]
	add	sp, sp, 48
	ret
	.size	_nand_sector_write, .-_nand_sector_write
	.align	2
	.global	_nand_flush_sector_write_cache
	.type	_nand_flush_sector_write_cache, %function
_nand_flush_sector_write_cache:
	sub	sp, sp, #32
	str	x0, [sp, 8]
	str	w1, [sp, 4]
	str	wzr, [sp, 28]
	ldr	w0, [sp, 28]
	add	sp, sp, 32
	ret
	.size	_nand_flush_sector_write_cache, .-_nand_flush_sector_write_cache
	.align	2
	.global	_shutdown_op
	.type	_shutdown_op, %function
_shutdown_op:
	stp	x29, x30, [sp, -32]!
	add	x29, sp, 0
	str	x0, [x29, 24]
	ldr	x0, [x29, 24]
	ldr	x0, [x0, 24]
	bl	__shutdown_op
	ldp	x29, x30, [sp], 32
	ret
	.size	_shutdown_op, .-_shutdown_op
	.align	2
	.global	_nftl_nand_erase_superblk
	.type	_nftl_nand_erase_superblk, %function
_nftl_nand_erase_superblk:
	stp	x29, x30, [sp, -80]!
	add	x29, sp, 0
	str	x0, [x29, 24]
	str	x1, [x29, 16]
	ldr	x0, [x29, 24]
	bl	nftl_get_zone_priv
	str	x0, [x29, 72]
	ldr	x0, [x29, 72]
	ldr	x0, [x0, 8]
	str	x0, [x29, 64]
	ldr	x0, [x29, 16]
	ldrh	w0, [x0, 2]
	strh	w0, [x29, 42]
	ldr	x0, [x29, 16]
	ldrh	w0, [x0]
	strh	w0, [x29, 40]
	ldr	x0, [x29, 16]
	ldrh	w0, [x0, 4]
	strh	w0, [x29, 44]
	ldr	x0, [x29, 16]
	ldr	x0, [x0, 8]
	str	x0, [x29, 48]
	ldr	x0, [x29, 16]
	ldr	x0, [x0, 16]
	str	x0, [x29, 56]
	ldr	x0, [x29, 64]
	ldr	x2, [x0, 88]
	ldr	x0, [x29, 72]
	ldr	x0, [x0, 8]
	add	x1, x29, 40
	blr	x2
	ldp	x29, x30, [sp], 80
	ret
	.size	_nftl_nand_erase_superblk, .-_nftl_nand_erase_superblk
	.align	2
	.global	_nftl_nand_read_page
	.type	_nftl_nand_read_page, %function
_nftl_nand_read_page:
	stp	x29, x30, [sp, -80]!
	add	x29, sp, 0
	str	x0, [x29, 24]
	str	x1, [x29, 16]
	ldr	x0, [x29, 24]
	bl	nftl_get_zone_priv
	str	x0, [x29, 72]
	ldr	x0, [x29, 72]
	ldr	x0, [x0, 8]
	str	x0, [x29, 64]
	ldr	x0, [x29, 24]
	ldr	x0, [x0, 3440]
	ldr	x1, [x0, 80]
	add	x1, x1, 1
	str	x1, [x0, 80]
	ldr	x0, [x29, 16]
	ldrh	w0, [x0, 2]
	strh	w0, [x29, 34]
	ldr	x0, [x29, 16]
	ldrh	w0, [x0]
	strh	w0, [x29, 32]
	ldr	x0, [x29, 16]
	ldrh	w0, [x0, 4]
	strh	w0, [x29, 36]
	ldr	x0, [x29, 16]
	ldr	x0, [x0, 8]
	str	x0, [x29, 40]
	ldr	x0, [x29, 16]
	ldr	x0, [x0, 16]
	str	x0, [x29, 48]
	ldr	x0, [x29, 64]
	ldr	x2, [x0, 96]
	ldr	x0, [x29, 72]
	ldr	x0, [x0, 8]
	add	x1, x29, 32
	blr	x2
	str	w0, [x29, 60]
	ldr	w0, [x29, 60]
	cmn	w0, #2
	bne	.L112
	ldr	x0, [x29, 24]
	ldr	x0, [x0, 3440]
	ldr	w1, [x0, 132]
	add	w1, w1, 1
	str	w1, [x0, 132]
.L112:
	ldr	w0, [x29, 60]
	ldp	x29, x30, [sp], 80
	ret
	.size	_nftl_nand_read_page, .-_nftl_nand_read_page
	.align	2
	.global	_nftl_nand_write_page
	.type	_nftl_nand_write_page, %function
_nftl_nand_write_page:
	stp	x29, x30, [sp, -80]!
	add	x29, sp, 0
	str	x0, [x29, 24]
	str	x1, [x29, 16]
	ldr	x0, [x29, 24]
	bl	nftl_get_zone_priv
	str	x0, [x29, 72]
	ldr	x0, [x29, 72]
	ldr	x0, [x0, 8]
	str	x0, [x29, 64]
	ldr	x0, [x29, 24]
	ldr	x0, [x0, 3440]
	ldr	x1, [x0, 88]
	add	x1, x1, 1
	str	x1, [x0, 88]
	ldr	x0, [x29, 16]
	ldrh	w0, [x0, 2]
	strh	w0, [x29, 34]
	ldr	x0, [x29, 16]
	ldrh	w0, [x0]
	strh	w0, [x29, 32]
	ldr	x0, [x29, 16]
	ldrh	w0, [x0, 4]
	strh	w0, [x29, 36]
	ldr	x0, [x29, 16]
	ldr	x0, [x0, 8]
	str	x0, [x29, 40]
	ldr	x0, [x29, 16]
	ldr	x0, [x0, 16]
	str	x0, [x29, 48]
	ldr	x0, [x29, 64]
	ldr	x2, [x0, 104]
	ldr	x0, [x29, 72]
	ldr	x0, [x0, 8]
	add	x1, x29, 32
	blr	x2
	str	w0, [x29, 60]
	ldr	w0, [x29, 60]
	cmp	w0, 0
	beq	.L115
	ldr	x0, [x29, 24]
	ldr	x0, [x0, 3440]
	ldr	w1, [x0, 136]
	add	w1, w1, 1
	str	w1, [x0, 136]
.L115:
	ldr	w0, [x29, 60]
	ldp	x29, x30, [sp], 80
	ret
	.size	_nftl_nand_write_page, .-_nftl_nand_write_page
	.align	2
	.global	_nftl_nand_is_blk_good
	.type	_nftl_nand_is_blk_good, %function
_nftl_nand_is_blk_good:
	stp	x29, x30, [sp, -80]!
	add	x29, sp, 0
	str	x0, [x29, 24]
	str	x1, [x29, 16]
	ldr	x0, [x29, 24]
	bl	nftl_get_zone_priv
	str	x0, [x29, 72]
	ldr	x0, [x29, 72]
	ldr	x0, [x0, 8]
	str	x0, [x29, 64]
	ldr	x0, [x29, 16]
	ldrh	w0, [x0, 2]
	strh	w0, [x29, 42]
	ldr	x0, [x29, 16]
	ldrh	w0, [x0]
	strh	w0, [x29, 40]
	ldr	x0, [x29, 16]
	ldrh	w0, [x0, 4]
	strh	w0, [x29, 44]
	ldr	x0, [x29, 16]
	ldr	x0, [x0, 8]
	str	x0, [x29, 48]
	ldr	x0, [x29, 16]
	ldr	x0, [x0, 16]
	str	x0, [x29, 56]
	ldr	x0, [x29, 64]
	ldr	x2, [x0, 112]
	ldr	x0, [x29, 72]
	ldr	x0, [x0, 8]
	add	x1, x29, 40
	blr	x2
	ldp	x29, x30, [sp], 80
	ret
	.size	_nftl_nand_is_blk_good, .-_nftl_nand_is_blk_good
	.align	2
	.global	_nftl_nand_mark_bad_blk
	.type	_nftl_nand_mark_bad_blk, %function
_nftl_nand_mark_bad_blk:
	stp	x29, x30, [sp, -80]!
	add	x29, sp, 0
	str	x0, [x29, 24]
	str	x1, [x29, 16]
	ldr	x0, [x29, 24]
	bl	nftl_get_zone_priv
	str	x0, [x29, 72]
	ldr	x0, [x29, 72]
	ldr	x0, [x0, 8]
	str	x0, [x29, 64]
	ldr	x0, [x29, 16]
	ldrh	w0, [x0, 2]
	strh	w0, [x29, 42]
	ldr	x0, [x29, 16]
	ldrh	w0, [x0]
	strh	w0, [x29, 40]
	ldr	x0, [x29, 16]
	ldrh	w0, [x0, 4]
	strh	w0, [x29, 44]
	ldr	x0, [x29, 16]
	ldr	x0, [x0, 8]
	str	x0, [x29, 48]
	ldr	x0, [x29, 16]
	ldr	x0, [x0, 16]
	str	x0, [x29, 56]
	ldr	x0, [x29, 64]
	ldr	x2, [x0, 120]
	ldr	x0, [x29, 72]
	ldr	x0, [x0, 8]
	add	x1, x29, 40
	blr	x2
	ldp	x29, x30, [sp], 80
	ret
	.size	_nftl_nand_mark_bad_blk, .-_nftl_nand_mark_bad_blk
	.local	panic_info
	.comm	panic_info,40,8
	.local	panic_part
	.comm	panic_part,16,8
	.local	nand_on_panic
	.comm	nand_on_panic,4,4
	.local	panic_enable
	.comm	panic_enable,4,4
	.align	2
	.type	panic_mark_panic, %function
panic_mark_panic:
	adrp	x0, nand_on_panic
	add	x0, x0, :lo12:nand_on_panic
	mov	w1, 1
	str	w1, [x0]
	nop
	ret
	.size	panic_mark_panic, .-panic_mark_panic
	.align	2
	.global	is_on_panic
	.type	is_on_panic, %function
is_on_panic:
	adrp	x0, nand_on_panic
	add	x0, x0, :lo12:nand_on_panic
	ldr	w0, [x0]
	cmp	w0, 0
	cset	w0, ne
	uxtb	w0, w0
	ret
	.size	is_on_panic, .-is_on_panic
	.align	2
	.global	panic_mark_enable
	.type	panic_mark_enable, %function
panic_mark_enable:
	adrp	x0, panic_enable
	add	x0, x0, :lo12:panic_enable
	mov	w1, 1
	str	w1, [x0]
	nop
	ret
	.size	panic_mark_enable, .-panic_mark_enable
	.align	2
	.global	is_panic_enable
	.type	is_panic_enable, %function
is_panic_enable:
	adrp	x0, panic_enable
	add	x0, x0, :lo12:panic_enable
	ldr	w0, [x0]
	cmp	w0, 0
	cset	w0, ne
	uxtb	w0, w0
	ret
	.size	is_panic_enable, .-is_panic_enable
	.section	.rodata
	.align	3
.LC18:
	.string	"[ND] panic part %s off %u size %u\n"
	.text
	.align	2
	.global	panic_init_part
	.type	panic_init_part, %function
panic_init_part:
	stp	x29, x30, [sp, -32]!
	add	x29, sp, 0
	str	x0, [x29, 24]
	str	w1, [x29, 20]
	str	w2, [x29, 16]
	ldr	x0, [x29, 24]
	cmp	x0, 0
	beq	.L128
	ldr	w0, [x29, 16]
	cmp	w0, 0
	bne	.L129
.L128:
	mov	w0, -1
	b	.L130
.L129:
	adrp	x0, panic_part
	add	x0, x0, :lo12:panic_part
	ldr	w0, [x0, 12]
	cmp	w0, 0
	beq	.L131
	mov	w0, -1
	b	.L130
.L131:
	adrp	x0, panic_part
	add	x0, x0, :lo12:panic_part
	ldr	x1, [x29, 24]
	str	x1, [x0]
	adrp	x0, panic_part
	add	x0, x0, :lo12:panic_part
	ldr	w1, [x29, 20]
	str	w1, [x0, 8]
	adrp	x0, panic_part
	add	x0, x0, :lo12:panic_part
	ldr	w1, [x29, 16]
	str	w1, [x0, 12]
	bl	panic_mark_enable
	adrp	x0, .LC18
	add	x0, x0, :lo12:.LC18
	ldr	w3, [x29, 16]
	ldr	w2, [x29, 20]
	ldr	x1, [x29, 24]
	bl	nand_print
	mov	w0, 0
.L130:
	ldp	x29, x30, [sp], 32
	ret
	.size	panic_init_part, .-panic_init_part
	.section	.rodata
	.align	3
.LC19:
	.string	"[ND] reserved panic block before %d!\n"
	.align	3
.LC20:
	.string	"[NE] reserved panic block %d!\n"
	.align	3
.LC21:
	.string	"[NE] no reserved panic block!!! quit!\n"
	.text
	.align	2
	.global	panic_backup_block
	.type	panic_backup_block, %function
panic_backup_block:
	stp	x29, x30, [sp, -64]!
	add	x29, sp, 0
	str	x0, [x29, 24]
	bl	is_panic_enable
	cmp	w0, 0
	bne	.L133
	mov	w0, 0
	b	.L134
.L133:
	str	wzr, [x29, 60]
	b	.L135
.L139:
	adrp	x0, panic_info
	add	x0, x0, :lo12:panic_info
	ldrsw	x1, [x29, 60]
	ldr	x0, [x0, x1, lsl 3]
	str	x0, [x29, 48]
	ldr	x0, [x29, 48]
	cmp	x0, 0
	beq	.L136
	ldr	x0, [x29, 48]
	ldrh	w0, [x0]
	mov	w1, w0
	adrp	x0, .LC19
	add	x0, x0, :lo12:.LC19
	bl	nand_print
	mov	w0, 0
	b	.L134
.L136:
	ldr	x0, [x29, 24]
	bl	out_phy_block_from_free_list
	str	x0, [x29, 48]
	ldr	x0, [x29, 48]
	cmp	x0, 0
	beq	.L137
	ldr	x0, [x29, 48]
	ldrh	w0, [x0]
	mov	w1, w0
	adrp	x0, .LC20
	add	x0, x0, :lo12:.LC20
	bl	nand_print
	adrp	x0, panic_info
	add	x0, x0, :lo12:panic_info
	ldrsw	x1, [x29, 60]
	ldr	x2, [x29, 48]
	str	x2, [x0, x1, lsl 3]
	b	.L140
.L137:
	adrp	x0, .LC21
	add	x0, x0, :lo12:.LC21
	bl	nand_print
	mov	w0, -1
	b	.L134
.L140:
	ldr	w0, [x29, 60]
	add	w0, w0, 1
	str	w0, [x29, 60]
.L135:
	ldr	w0, [x29, 60]
	cmp	w0, 2
	ble	.L139
	ldr	x0, [x29, 24]
	ldr	x0, [x0, 8]
	str	x0, [x29, 40]
	ldr	x0, [x29, 40]
	ldrh	w0, [x0, 20]
	mov	w1, w0
	mov	w0, w1
	lsl	w0, w0, 1
	add	w0, w0, w1
	mov	w1, w0
	adrp	x0, panic_info
	add	x0, x0, :lo12:panic_info
	str	w1, [x0, 24]
	mov	w0, 0
.L134:
	ldp	x29, x30, [sp], 64
	ret
	.size	panic_backup_block, .-panic_backup_block
	.align	2
	.global	panic_read
	.type	panic_read, %function
panic_read:
	stp	x29, x30, [sp, -64]!
	add	x29, sp, 0
	str	x0, [x29, 40]
	str	w1, [x29, 36]
	str	w2, [x29, 32]
	str	x3, [x29, 24]
	ldr	x0, [x29, 24]
	str	x0, [x29, 56]
	bl	is_panic_enable
	cmp	w0, 0
	bne	.L142
	mov	w0, -1
	b	.L143
.L142:
	bl	panic_mark_panic
	b	.L144
.L145:
	ldr	x0, [x29, 40]
	ldr	x4, [x0, 72]
	ldr	x3, [x29, 56]
	mov	w2, 32768
	ldr	w1, [x29, 36]
	ldr	x0, [x29, 40]
	blr	x4
	ldr	x0, [x29, 56]
	add	x0, x0, 32768
	str	x0, [x29, 56]
	ldr	w0, [x29, 36]
	add	w0, w0, 32768
	str	w0, [x29, 36]
	ldr	w0, [x29, 32]
	sub	w0, w0, #32768
	str	w0, [x29, 32]
.L144:
	ldr	w1, [x29, 32]
	mov	w0, 32767
	cmp	w1, w0
	bhi	.L145
	ldr	w0, [x29, 32]
	cmp	w0, 0
	beq	.L146
	ldr	x0, [x29, 40]
	ldr	x4, [x0, 72]
	ldr	x3, [x29, 56]
	ldr	w2, [x29, 32]
	ldr	w1, [x29, 36]
	ldr	x0, [x29, 40]
	blr	x4
.L146:
	mov	w0, 0
.L143:
	ldp	x29, x30, [sp], 64
	ret
	.size	panic_read, .-panic_read
	.section	.rodata
	.align	3
.LC22:
	.string	"[ND] write panic data : logic addr: %d, sectors %d, pages %d, blocks %d\n"
	.align	3
.LC23:
	.string	"[NE] too much data for panic-write, %d sectors left over\n"
	.align	3
.LC24:
	.string	"[ND] write panic data block: %d page %d !\n"
	.align	3
.LC25:
	.string	"[ND] write dummy data block: %d page %d !\n"
	.text
	.align	2
	.global	panic_write
	.type	panic_write, %function
panic_write:
	stp	x29, x30, [sp, -144]!
	add	x29, sp, 0
	str	x0, [x29, 40]
	str	w1, [x29, 36]
	str	w2, [x29, 32]
	str	x3, [x29, 24]
	adrp	x0, panic_info
	add	x0, x0, :lo12:panic_info
	str	x0, [x29, 112]
	bl	is_panic_enable
	cmp	w0, 0
	bne	.L148
	mov	w0, -1
	b	.L159
.L148:
	bl	panic_mark_panic
	ldr	x0, [x29, 40]
	ldr	x0, [x0, 24]
	str	x0, [x29, 104]
	ldr	x0, [x29, 104]
	ldr	x0, [x0, 8]
	str	x0, [x29, 96]
	ldr	x0, [x29, 96]
	ldrb	w0, [x0, 8]
	mov	w1, w0
	ldr	w0, [x29, 32]
	add	w0, w1, w0
	sub	w0, w0, #1
	ldr	x1, [x29, 96]
	ldrb	w1, [x1, 8]
	udiv	w0, w0, w1
	str	w0, [x29, 124]
	ldr	x0, [x29, 96]
	ldrh	w0, [x0, 20]
	mov	w1, w0
	ldr	w0, [x29, 124]
	add	w0, w1, w0
	sub	w0, w0, #1
	ldr	x1, [x29, 96]
	ldrh	w1, [x1, 20]
	udiv	w0, w0, w1
	str	w0, [x29, 92]
	adrp	x0, .LC22
	add	x0, x0, :lo12:.LC22
	ldr	w4, [x29, 92]
	ldr	w3, [x29, 124]
	ldr	w2, [x29, 32]
	ldr	w1, [x29, 36]
	bl	nand_print
	ldr	x0, [x29, 112]
	ldr	w0, [x0, 24]
	sub	w1, w0, #3
	ldr	w0, [x29, 124]
	cmp	w1, w0
	bcs	.L150
	ldr	x0, [x29, 112]
	ldr	w0, [x0, 24]
	sub	w0, w0, #3
	ldr	x1, [x29, 96]
	ldrb	w1, [x1, 8]
	mul	w1, w0, w1
	adrp	x0, .LC23
	add	x0, x0, :lo12:.LC23
	bl	nand_print
	mov	w0, -1
	b	.L159
.L150:
	add	x0, x29, 72
	mov	w2, 16
	mov	w1, -91
	bl	nand_memset
	ldr	x0, [x29, 24]
	str	x0, [x29, 128]
	ldr	x0, [x29, 112]
	ldr	w1, [x0, 28]
	ldr	x0, [x29, 112]
	uxtw	x1, w1
	ldr	x0, [x0, x1, lsl 3]
	str	x0, [x29, 136]
	b	.L151
.L154:
	ldr	x0, [x29, 112]
	ldr	w0, [x0, 32]
	ldr	x1, [x29, 96]
	ldrh	w1, [x1, 20]
	cmp	w0, w1
	bcc	.L152
	ldr	x0, [x29, 112]
	ldr	w0, [x0, 28]
	add	w1, w0, 1
	ldr	x0, [x29, 112]
	str	w1, [x0, 28]
	ldr	x0, [x29, 112]
	ldr	w1, [x0, 28]
	ldr	x0, [x29, 112]
	uxtw	x1, w1
	ldr	x0, [x0, x1, lsl 3]
	str	x0, [x29, 136]
	ldr	x0, [x29, 112]
	str	wzr, [x0, 32]
.L152:
	ldr	x0, [x29, 112]
	ldr	w0, [x0, 32]
	cmp	w0, 0
	bne	.L153
	ldr	x0, [x29, 136]
	ldrh	w1, [x0]
	ldr	x0, [x29, 96]
	ldrh	w2, [x0, 28]
	add	x0, x29, 48
	mov	x5, 0
	mov	x4, 0
	mov	w3, w2
	mov	w2, 0
	bl	set_physic_op_par
	ldr	x0, [x29, 104]
	ldr	x2, [x0, 3456]
	add	x0, x29, 48
	mov	x1, x0
	ldr	x0, [x29, 104]
	blr	x2
	ldr	x0, [x29, 136]
	ldrh	w0, [x0, 6]
	add	w0, w0, 1
	uxth	w1, w0
	ldr	x0, [x29, 136]
	strh	w1, [x0, 6]
	ldr	x0, [x29, 136]
	strh	wzr, [x0, 4]
.L153:
	ldr	x0, [x29, 136]
	ldrh	w0, [x0]
	mov	w3, w0
	ldr	x0, [x29, 112]
	ldr	w1, [x0, 32]
	adrp	x0, .LC24
	add	x0, x0, :lo12:.LC24
	mov	w2, w1
	mov	w1, w3
	bl	nand_print
	mov	w0, -1
	strb	w0, [x29, 72]
	mov	w0, -86
	strb	w0, [x29, 73]
	mov	w0, 99
	strb	w0, [x29, 74]
	ldr	x0, [x29, 104]
	ldrb	w0, [x0, 24]
	strb	w0, [x29, 75]
	ldr	x0, [x29, 112]
	ldr	w0, [x0, 28]
	uxtb	w0, w0
	strb	w0, [x29, 76]
	ldr	x0, [x29, 136]
	ldrh	w0, [x0, 6]
	mov	w1, w0
	add	x0, x29, 72
	mov	w3, 2
	mov	w2, 5
	bl	set_spare_data
	add	x0, x29, 72
	mov	w3, 4
	mov	w2, 7
	ldr	w1, [x29, 36]
	bl	set_spare_data
	ldr	x0, [x29, 96]
	ldrb	w0, [x0, 8]
	mov	w1, w0
	ldr	w0, [x29, 32]
	cmp	w1, w0
	csel	w1, w1, w0, ls
	add	x0, x29, 72
	mov	w3, 4
	mov	w2, 11
	bl	set_spare_data
	ldr	x0, [x29, 136]
	ldrh	w1, [x0]
	ldr	x0, [x29, 112]
	ldr	w0, [x0, 32]
	uxth	w2, w0
	ldr	x0, [x29, 96]
	ldrh	w3, [x0, 28]
	add	x4, x29, 72
	add	x0, x29, 48
	mov	x5, x4
	ldr	x4, [x29, 128]
	bl	set_physic_op_par
	ldr	x0, [x29, 104]
	ldr	x2, [x0, 3472]
	add	x0, x29, 48
	mov	x1, x0
	ldr	x0, [x29, 104]
	blr	x2
	ldr	x0, [x29, 96]
	ldrh	w0, [x0, 26]
	uxth	x0, w0
	ldr	x1, [x29, 128]
	add	x0, x1, x0
	str	x0, [x29, 128]
	ldr	x0, [x29, 96]
	ldrb	w0, [x0, 8]
	mov	w1, w0
	ldr	w0, [x29, 32]
	cmp	w1, w0
	csel	w0, w1, w0, ls
	ldr	w1, [x29, 36]
	add	w0, w1, w0
	str	w0, [x29, 36]
	ldr	x0, [x29, 96]
	ldrb	w0, [x0, 8]
	mov	w1, w0
	ldr	w0, [x29, 32]
	cmp	w1, w0
	csel	w0, w1, w0, ls
	ldr	w1, [x29, 32]
	sub	w0, w1, w0
	str	w0, [x29, 32]
	ldr	x0, [x29, 112]
	ldr	w0, [x0, 24]
	sub	w1, w0, #1
	ldr	x0, [x29, 112]
	str	w1, [x0, 24]
	ldr	x0, [x29, 112]
	ldr	w0, [x0, 32]
	add	w1, w0, 1
	ldr	x0, [x29, 112]
	str	w1, [x0, 32]
	ldr	w0, [x29, 124]
	sub	w0, w0, #1
	str	w0, [x29, 124]
.L151:
	ldr	w0, [x29, 124]
	cmp	w0, 0
	bne	.L154
	mov	w0, 98
	strb	w0, [x29, 74]
	str	wzr, [x29, 120]
	b	.L155
.L158:
	ldr	x0, [x29, 112]
	ldr	w0, [x0, 32]
	cmp	w0, 0
	beq	.L156
	ldr	x0, [x29, 112]
	ldr	w0, [x0, 32]
	ldr	x1, [x29, 96]
	ldrh	w1, [x1, 20]
	cmp	w0, w1
	bcs	.L156
	ldr	x0, [x29, 136]
	ldrh	w0, [x0]
	mov	w3, w0
	ldr	x0, [x29, 112]
	ldr	w1, [x0, 32]
	adrp	x0, .LC25
	add	x0, x0, :lo12:.LC25
	mov	w2, w1
	mov	w1, w3
	bl	nand_print
	ldr	x0, [x29, 136]
	ldrh	w1, [x0]
	ldr	x0, [x29, 112]
	ldr	w0, [x0, 32]
	uxth	w2, w0
	ldr	x0, [x29, 96]
	ldrh	w3, [x0, 28]
	add	x4, x29, 72
	add	x0, x29, 48
	mov	x5, x4
	ldr	x4, [x29, 128]
	bl	set_physic_op_par
	ldr	x0, [x29, 104]
	ldr	x2, [x0, 3472]
	add	x0, x29, 48
	mov	x1, x0
	ldr	x0, [x29, 104]
	blr	x2
	ldr	x0, [x29, 112]
	ldr	w0, [x0, 32]
	add	w1, w0, 1
	ldr	x0, [x29, 112]
	str	w1, [x0, 32]
	ldr	x0, [x29, 112]
	ldr	w0, [x0, 24]
	sub	w1, w0, #1
	ldr	x0, [x29, 112]
	str	w1, [x0, 24]
	ldr	x0, [x29, 112]
	ldr	w0, [x0, 32]
	ldr	x1, [x29, 96]
	ldrh	w1, [x1, 20]
	cmp	w0, w1
	bcc	.L157
	ldr	x0, [x29, 112]
	ldr	w0, [x0, 28]
	add	w1, w0, 1
	ldr	x0, [x29, 112]
	str	w1, [x0, 28]
	ldr	x0, [x29, 112]
	ldr	w1, [x0, 28]
	ldr	x0, [x29, 112]
	uxtw	x1, w1
	ldr	x0, [x0, x1, lsl 3]
	str	x0, [x29, 136]
	ldr	x0, [x29, 112]
	str	wzr, [x0, 32]
.L157:
	ldr	w0, [x29, 120]
	add	w0, w0, 1
	str	w0, [x29, 120]
.L155:
	ldr	w0, [x29, 120]
	cmp	w0, 2
	bls	.L158
.L156:
	mov	w0, 0
.L159:
	ldp	x29, x30, [sp], 144
	ret
	.size	panic_write, .-panic_write
	.align	2
	.global	is_panic_data_page
	.type	is_panic_data_page, %function
is_panic_data_page:
	sub	sp, sp, #16
	str	x0, [sp, 8]
	ldr	x0, [sp, 8]
	add	x0, x0, 1
	ldrb	w0, [x0]
	cmp	w0, 170
	bne	.L161
	ldr	x0, [sp, 8]
	add	x0, x0, 2
	ldrb	w0, [x0]
	cmp	w0, 99
	bne	.L161
	mov	w0, 1
	b	.L162
.L161:
	mov	w0, 0
.L162:
	add	sp, sp, 16
	ret
	.size	is_panic_data_page, .-is_panic_data_page
	.align	2
	.global	is_panic_dummy_page
	.type	is_panic_dummy_page, %function
is_panic_dummy_page:
	sub	sp, sp, #16
	str	x0, [sp, 8]
	ldr	x0, [sp, 8]
	add	x0, x0, 1
	ldrb	w0, [x0]
	cmp	w0, 170
	bne	.L165
	ldr	x0, [sp, 8]
	add	x0, x0, 2
	ldrb	w0, [x0]
	cmp	w0, 98
	bne	.L165
	mov	w0, 1
	b	.L166
.L165:
	mov	w0, 0
.L166:
	add	sp, sp, 16
	ret
	.size	is_panic_dummy_page, .-is_panic_dummy_page
	.section	.rodata
	.align	3
.LC26:
	.string	"[ND] find panic block : %d\n"
	.align	3
.LC27:
	.string	"[NE] invalid panic block index %d\n"
	.text
	.align	2
	.global	panic_recover_info
	.type	panic_recover_info, %function
panic_recover_info:
	stp	x29, x30, [sp, -48]!
	add	x29, sp, 0
	str	x0, [x29, 24]
	str	x1, [x29, 16]
	ldr	x0, [x29, 24]
	add	x0, x0, 4
	ldrb	w0, [x0]
	str	w0, [x29, 44]
	ldr	w0, [x29, 44]
	cmp	w0, 2
	bhi	.L169
	ldr	x0, [x29, 16]
	ldrh	w0, [x0]
	mov	w1, w0
	adrp	x0, .LC26
	add	x0, x0, :lo12:.LC26
	bl	nand_print
	adrp	x0, panic_info
	add	x0, x0, :lo12:panic_info
	ldr	w1, [x29, 44]
	ldr	x2, [x29, 16]
	str	x2, [x0, x1, lsl 3]
	mov	w0, 0
	b	.L170
.L169:
	adrp	x0, .LC27
	add	x0, x0, :lo12:.LC27
	ldr	w1, [x29, 44]
	bl	nand_print
	mov	w0, -1
.L170:
	ldp	x29, x30, [sp], 48
	ret
	.size	panic_recover_info, .-panic_recover_info
	.section	.rodata
	.align	3
.LC28:
	.string	"[ND] not find panic data, no need to recovery!\n"
	.align	3
.LC29:
	.string	"[ND] recovery panic data start!\n"
	.align	3
.LC30:
	.string	"[NE] %s(%d): malloc failed\n"
	.align	3
.LC31:
	.string	"[ND] ignore blk %d page %d as dummy page\n"
	.align	3
.LC32:
	.string	"[ND] exit recovery panic block\n"
	.align	3
.LC33:
	.string	"[ND] invalid blk %d page %d, finish recovery\n"
	.align	3
.LC34:
	.string	"[ND] move blk %d page %d to logic addr %d\n"
	.text
	.align	2
	.global	panic_recover_data
	.type	panic_recover_data, %function
panic_recover_data:
	stp	x29, x30, [sp, -128]!
	add	x29, sp, 0
	str	x0, [x29, 24]
	ldr	x0, [x29, 24]
	ldr	x0, [x0, 24]
	str	x0, [x29, 120]
	ldr	x0, [x29, 120]
	ldr	x0, [x0, 8]
	str	x0, [x29, 112]
	adrp	x0, panic_info
	add	x0, x0, :lo12:panic_info
	str	x0, [x29, 104]
	str	xzr, [x29, 96]
	ldr	x0, [x29, 104]
	ldr	x0, [x0]
	cmp	x0, 0
	bne	.L172
	adrp	x0, .LC28
	add	x0, x0, :lo12:.LC28
	bl	nand_print
	mov	w0, 0
	b	.L173
.L172:
	adrp	x0, .LC29
	add	x0, x0, :lo12:.LC29
	bl	nand_print
	ldr	x0, [x29, 112]
	ldrh	w0, [x0, 26]
	bl	nand_malloc
	str	x0, [x29, 96]
	ldr	x0, [x29, 96]
	cmp	x0, 0
	bne	.L174
	adrp	x0, __func__.5524
	add	x1, x0, :lo12:__func__.5524
	adrp	x0, .LC30
	add	x0, x0, :lo12:.LC30
	mov	w2, 308
	bl	nand_print
	mov	w0, -1
	b	.L173
.L174:
	ldr	x0, [x29, 112]
	ldrh	w0, [x0, 26]
	mov	w2, w0
	mov	w1, 0
	ldr	x0, [x29, 96]
	bl	nand_memset
	ldr	x0, [x29, 104]
	str	wzr, [x0, 28]
	b	.L175
.L184:
	ldr	x0, [x29, 104]
	ldr	w1, [x0, 28]
	ldr	x0, [x29, 104]
	uxtw	x1, w1
	ldr	x0, [x0, x1, lsl 3]
	str	x0, [x29, 88]
	ldr	x0, [x29, 104]
	str	wzr, [x0, 32]
	b	.L176
.L183:
	ldr	x0, [x29, 88]
	ldrh	w1, [x0]
	ldr	x0, [x29, 104]
	ldr	w0, [x0, 32]
	uxth	w2, w0
	ldr	x0, [x29, 112]
	ldrh	w3, [x0, 28]
	add	x4, x29, 40
	add	x0, x29, 56
	mov	x5, x4
	ldr	x4, [x29, 96]
	bl	set_physic_op_par
	ldr	x0, [x29, 120]
	ldr	x2, [x0, 3464]
	add	x0, x29, 56
	mov	x1, x0
	ldr	x0, [x29, 120]
	blr	x2
	add	x0, x29, 40
	bl	is_panic_dummy_page
	cmp	w0, 0
	beq	.L177
	ldr	x0, [x29, 88]
	ldrh	w0, [x0]
	mov	w3, w0
	ldr	x0, [x29, 104]
	ldr	w1, [x0, 32]
	adrp	x0, .LC31
	add	x0, x0, :lo12:.LC31
	mov	w2, w1
	mov	w1, w3
	bl	nand_print
	b	.L178
.L177:
	add	x0, x29, 40
	bl	is_panic_data_page
	cmp	w0, 0
	bne	.L179
	ldr	x0, [x29, 104]
	ldr	w0, [x0, 28]
	cmp	w0, 0
	bne	.L180
	ldr	x0, [x29, 104]
	ldr	w0, [x0, 32]
	cmp	w0, 0
	bne	.L180
	ldr	x0, [x29, 88]
	ldrh	w0, [x0]
	mov	w3, w0
	ldr	x0, [x29, 104]
	ldr	w1, [x0, 32]
	adrp	x0, .LC32
	add	x0, x0, :lo12:.LC32
	mov	w2, w1
	mov	w1, w3
	bl	nand_print
	b	.L181
.L180:
	ldr	x0, [x29, 88]
	ldrh	w0, [x0]
	mov	w3, w0
	ldr	x0, [x29, 104]
	ldr	w1, [x0, 32]
	adrp	x0, .LC33
	add	x0, x0, :lo12:.LC33
	mov	w2, w1
	mov	w1, w3
	bl	nand_print
	b	.L185
.L179:
	add	x0, x29, 40
	mov	w2, 4
	mov	w1, 7
	bl	get_spare_data
	str	w0, [x29, 84]
	add	x0, x29, 40
	mov	w2, 4
	mov	w1, 11
	bl	get_spare_data
	str	w0, [x29, 80]
	ldr	x0, [x29, 88]
	ldrh	w0, [x0]
	mov	w4, w0
	ldr	x0, [x29, 104]
	ldr	w1, [x0, 32]
	adrp	x0, .LC34
	add	x0, x0, :lo12:.LC34
	ldr	w3, [x29, 84]
	mov	w2, w1
	mov	w1, w4
	bl	nand_print
	ldr	x2, [x29, 96]
	ldr	w1, [x29, 80]
	ldr	w0, [x29, 84]
	bl	nftl_write
.L178:
	ldr	x0, [x29, 104]
	ldr	w0, [x0, 32]
	add	w1, w0, 1
	ldr	x0, [x29, 104]
	str	w1, [x0, 32]
.L176:
	ldr	x0, [x29, 104]
	ldr	w0, [x0, 32]
	ldr	x1, [x29, 112]
	ldrh	w1, [x1, 20]
	cmp	w0, w1
	bcc	.L183
	ldr	x0, [x29, 104]
	ldr	w0, [x0, 28]
	add	w1, w0, 1
	ldr	x0, [x29, 104]
	str	w1, [x0, 28]
.L175:
	ldr	x0, [x29, 104]
	ldr	w0, [x0, 28]
	cmp	w0, 2
	bls	.L184
.L182:
	b	.L185
.L186:
	ldr	x0, [x29, 104]
	ldr	w1, [x0, 28]
	ldr	x0, [x29, 104]
	uxtw	x1, w1
	ldr	x0, [x0, x1, lsl 3]
	str	x0, [x29, 88]
	ldr	x0, [x29, 88]
	ldrh	w0, [x0, 6]
	add	w0, w0, 1
	uxth	w0, w0
	mov	w2, w0
	ldr	x1, [x29, 88]
	ldr	x0, [x29, 120]
	bl	erase_block
	ldr	x0, [x29, 104]
	ldr	w0, [x0, 28]
	sub	w1, w0, #1
	ldr	x0, [x29, 104]
	str	w1, [x0, 28]
.L185:
	ldr	x0, [x29, 104]
	ldr	w0, [x0, 28]
	cmp	w0, 0
	bge	.L186
.L181:
	ldr	x0, [x29, 96]
	bl	nand_free
	mov	w0, 0
.L173:
	ldp	x29, x30, [sp], 128
	ret
	.size	panic_recover_data, .-panic_recover_data
	.global	samsung_read_retry_mode
	.bss
	.type	samsung_read_retry_mode, %object
	.size	samsung_read_retry_mode, 1
samsung_read_retry_mode:
	.zero	1
	.global	samsung_read_retry_cycle
	.type	samsung_read_retry_cycle, %object
	.size	samsung_read_retry_cycle, 1
samsung_read_retry_cycle:
	.zero	1
	.global	samsung_read_retry_addr
	.data
	.align	3
	.type	samsung_read_retry_addr, %object
	.size	samsung_read_retry_addr, 4
samsung_read_retry_addr:
	.byte	-89
	.byte	-92
	.byte	-91
	.byte	-90
	.global	samsung_read_retry_cmd
	.align	3
	.type	samsung_read_retry_cmd, %object
	.size	samsung_read_retry_cmd, 4
samsung_read_retry_cmd:
	.byte	-95
	.byte	-95
	.byte	-95
	.byte	-95
	.global	samsung_p1
	.align	3
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
	.section	.rodata
	.align	3
.LC35:
	.string	"rr value %x %x %x %x!\n"
	.text
	.align	2
	.global	samsung_vender_set_param
	.type	samsung_vender_set_param, %function
samsung_vender_set_param:
	stp	x29, x30, [sp, -64]!
	add	x29, sp, 0
	str	x0, [x29, 40]
	str	x1, [x29, 32]
	str	x2, [x29, 24]
	str	w3, [x29, 20]
	adrp	x0, samsung_read_retry_cmd
	add	x0, x0, :lo12:samsung_read_retry_cmd
	ldr	w6, [x29, 20]
	mov	w5, 1
	ldr	x4, [x29, 32]
	ldr	x3, [x29, 24]
	mov	w2, 0
	mov	x1, x0
	ldr	x0, [x29, 40]
	bl	set_cmd_with_nand_bus
	str	w0, [x29, 60]
	ldr	x0, [x29, 32]
	ldrb	w0, [x0]
	mov	w1, w0
	ldr	x0, [x29, 32]
	add	x0, x0, 1
	ldrb	w0, [x0]
	mov	w2, w0
	ldr	x0, [x29, 32]
	add	x0, x0, 2
	ldrb	w0, [x0]
	mov	w3, w0
	ldr	x0, [x29, 32]
	add	x0, x0, 3
	ldrb	w0, [x0]
	mov	w4, w0
	adrp	x0, .LC35
	add	x0, x0, :lo12:.LC35
	bl	nand_print_dbg
	ldr	w0, [x29, 60]
	ldp	x29, x30, [sp], 64
	ret
	.size	samsung_vender_set_param, .-samsung_vender_set_param
	.section	.rodata
	.align	3
.LC36:
	.string	"samsung set readretry error ! %x \n"
	.text
	.align	2
	.global	samsung_set_readretry
	.type	samsung_set_readretry, %function
samsung_set_readretry:
	stp	x29, x30, [sp, -48]!
	add	x29, sp, 0
	str	x0, [x29, 24]
	ldr	x0, [x29, 24]
	bl	nand_enable_chip
	ldr	x0, [x29, 24]
	ldr	x0, [x0, 272]
	bl	ndfc_disable_randomize
	ldr	x0, [x29, 24]
	ldr	w0, [x0, 184]
	uxtw	x0, w0
	lsl	x1, x0, 2
	adrp	x0, samsung_p1
	add	x0, x0, :lo12:samsung_p1
	add	x1, x1, x0
	adrp	x0, samsung_read_retry_addr
	add	x0, x0, :lo12:samsung_read_retry_addr
	mov	w3, 4
	mov	x2, x0
	ldr	x0, [x29, 24]
	bl	samsung_vender_set_param
	str	w0, [x29, 44]
	ldr	w0, [x29, 44]
	cmp	w0, 0
	beq	.L190
	ldr	x0, [x29, 24]
	ldr	w1, [x0, 184]
	adrp	x0, .LC36
	add	x0, x0, :lo12:.LC36
	bl	nand_print
.L190:
	ldr	x0, [x29, 24]
	bl	nand_disable_chip
	ldr	w0, [x29, 44]
	ldp	x29, x30, [sp], 48
	ret
	.size	samsung_set_readretry, .-samsung_set_readretry
	.section	.rodata
	.align	3
.LC37:
	.string	"samsung retry!\n"
	.align	3
.LC38:
	.string	"samsung ReadRetry ok! ch =%d, chip = %d  block = %d, page = %d, RetryCount = %d  \n"
	.text
	.align	2
	.global	samsung_read_page_end
	.type	samsung_read_page_end, %function
samsung_read_page_end:
	stp	x29, x30, [sp, -48]!
	add	x29, sp, 0
	str	x0, [x29, 24]
	str	wzr, [x29, 44]
	str	wzr, [x29, 40]
	adrp	x0, g_nsi
	add	x0, x0, :lo12:g_nsi
	ldr	x2, [x0]
	ldr	x0, [x29, 24]
	ldr	w0, [x0]
	mov	w1, w0
	mov	x0, x2
	bl	nci_get_from_nsi
	str	x0, [x29, 32]
	ldr	x0, [x29, 24]
	bl	generic_read_page_end_not_retry
	str	w0, [x29, 40]
	ldr	w0, [x29, 40]
	cmn	w0, #2
	bne	.L193
	adrp	x0, .LC37
	add	x0, x0, :lo12:.LC37
	bl	nand_print_dbg
	mov	w0, 1
	str	w0, [x29, 44]
	b	.L194
.L199:
	ldr	w1, [x29, 44]
	ldr	x0, [x29, 32]
	str	w1, [x0, 184]
	ldr	x0, [x29, 32]
	bl	samsung_set_readretry
	str	w0, [x29, 40]
	ldr	w0, [x29, 40]
	cmp	w0, 0
	bne	.L201
	ldr	x0, [x29, 24]
	bl	generic_read_page_start
	str	w0, [x29, 40]
	ldr	x0, [x29, 24]
	bl	generic_read_page_end_not_retry
	mov	w1, w0
	ldr	w0, [x29, 40]
	orr	w0, w0, w1
	str	w0, [x29, 40]
	ldr	w0, [x29, 40]
	cmp	w0, 10
	beq	.L197
	ldr	w0, [x29, 40]
	cmp	w0, 0
	bne	.L196
.L197:
	mov	w0, 10
	str	w0, [x29, 40]
	ldr	x0, [x29, 32]
	ldr	x0, [x0, 272]
	ldr	w1, [x0, 12]
	ldr	x0, [x29, 32]
	ldr	w2, [x0, 28]
	ldr	x0, [x29, 24]
	ldr	w3, [x0, 4]
	ldr	x0, [x29, 24]
	ldr	w4, [x0, 8]
	ldr	x0, [x29, 32]
	ldr	w5, [x0, 184]
	adrp	x0, .LC38
	add	x0, x0, :lo12:.LC38
	bl	nand_print_dbg
	b	.L198
.L201:
	nop
.L196:
	ldr	w0, [x29, 44]
	add	w0, w0, 1
	str	w0, [x29, 44]
.L194:
	adrp	x0, samsung_read_retry_cycle
	add	x0, x0, :lo12:samsung_read_retry_cycle
	ldrb	w0, [x0]
	mov	w1, w0
	ldr	w0, [x29, 44]
	cmp	w1, w0
	bgt	.L199
.L198:
	ldr	x0, [x29, 32]
	str	wzr, [x0, 184]
	ldr	x0, [x29, 32]
	bl	samsung_set_readretry
.L193:
	ldr	w0, [x29, 40]
	ldp	x29, x30, [sp], 48
	ret
	.size	samsung_read_page_end, .-samsung_read_page_end
	.section	.rodata
	.align	3
.LC39:
	.string	"samsung read retry conut: %d !\n"
	.text
	.align	2
	.global	samsung_readretry_init
	.type	samsung_readretry_init, %function
samsung_readretry_init:
	stp	x29, x30, [sp, -32]!
	add	x29, sp, 0
	str	x0, [x29, 24]
	ldr	x0, [x29, 24]
	str	wzr, [x0, 184]
	ldr	x0, [x29, 24]
	ldr	x0, [x0, 280]
	ldr	w0, [x0, 52]
	lsr	w0, w0, 16
	uxtb	w1, w0
	adrp	x0, samsung_read_retry_mode
	add	x0, x0, :lo12:samsung_read_retry_mode
	strb	w1, [x0]
	adrp	x0, samsung_read_retry_cycle
	add	x0, x0, :lo12:samsung_read_retry_cycle
	mov	w1, 15
	strb	w1, [x0]
	adrp	x0, samsung_read_retry_cycle
	add	x0, x0, :lo12:samsung_read_retry_cycle
	ldrb	w0, [x0]
	mov	w1, w0
	adrp	x0, .LC39
	add	x0, x0, :lo12:.LC39
	bl	nand_print_dbg
	adrp	x0, samsung_read_retry_mode
	add	x0, x0, :lo12:samsung_read_retry_mode
	ldrb	w0, [x0]
	cmp	w0, 32
	beq	.L203
	mov	w0, -100
	b	.L204
.L203:
	mov	w0, 0
.L204:
	ldp	x29, x30, [sp], 32
	ret
	.size	samsung_readretry_init, .-samsung_readretry_init
	.align	2
	.global	samsung_readretry_exit
	.type	samsung_readretry_exit, %function
samsung_readretry_exit:
	sub	sp, sp, #16
	str	x0, [sp, 8]
	mov	w0, 0
	add	sp, sp, 16
	ret
	.size	samsung_readretry_exit, .-samsung_readretry_exit
	.section	.rodata
	.align	3
.LC40:
	.string	" samsung_special_init samsung_read_retry_mode:%d samsung_read_retry_cycle :%d \n"
	.align	3
.LC41:
	.string	" samsung_special_init error samsung_read_retry_mode:%d samsung_read_retry_cycle :%d \n"
	.text
	.align	2
	.global	samsung_special_init
	.type	samsung_special_init, %function
samsung_special_init:
	stp	x29, x30, [sp, -32]!
	add	x29, sp, 0
	str	wzr, [x29, 28]
	adrp	x0, g_nsi
	add	x0, x0, :lo12:g_nsi
	ldr	x0, [x0]
	ldr	x0, [x0, 8]
	str	x0, [x29, 16]
	b	.L208
.L209:
	ldr	x0, [x29, 16]
	bl	samsung_readretry_init
	mov	w1, w0
	ldr	w0, [x29, 28]
	orr	w0, w0, w1
	str	w0, [x29, 28]
	ldr	x0, [x29, 16]
	ldr	x0, [x0]
	str	x0, [x29, 16]
.L208:
	ldr	x0, [x29, 16]
	cmp	x0, 0
	bne	.L209
	ldr	w0, [x29, 28]
	cmp	w0, 0
	bne	.L210
	adrp	x0, df_read_page_end
	add	x0, x0, :lo12:df_read_page_end
	adrp	x1, samsung_read_page_end
	add	x1, x1, :lo12:samsung_read_page_end
	str	x1, [x0]
	adrp	x0, samsung_read_retry_mode
	add	x0, x0, :lo12:samsung_read_retry_mode
	ldrb	w0, [x0]
	mov	w1, w0
	adrp	x0, samsung_read_retry_cycle
	add	x0, x0, :lo12:samsung_read_retry_cycle
	ldrb	w0, [x0]
	mov	w2, w0
	adrp	x0, .LC40
	add	x0, x0, :lo12:.LC40
	bl	nand_print_dbg
	b	.L211
.L210:
	adrp	x0, samsung_read_retry_mode
	add	x0, x0, :lo12:samsung_read_retry_mode
	ldrb	w0, [x0]
	mov	w1, w0
	adrp	x0, samsung_read_retry_cycle
	add	x0, x0, :lo12:samsung_read_retry_cycle
	ldrb	w0, [x0]
	mov	w2, w0
	adrp	x0, .LC41
	add	x0, x0, :lo12:.LC41
	bl	nand_print
.L211:
	ldr	w0, [x29, 28]
	ldp	x29, x30, [sp], 32
	ret
	.size	samsung_special_init, .-samsung_special_init
	.section	.rodata
	.align	3
.LC42:
	.string	" samsung_special_exit \n"
	.text
	.align	2
	.global	samsung_special_exit
	.type	samsung_special_exit, %function
samsung_special_exit:
	stp	x29, x30, [sp, -32]!
	add	x29, sp, 0
	adrp	x0, g_nsi
	add	x0, x0, :lo12:g_nsi
	ldr	x0, [x0]
	ldr	x0, [x0, 8]
	str	x0, [x29, 24]
	b	.L214
.L215:
	ldr	x0, [x29, 24]
	bl	samsung_readretry_exit
	ldr	x0, [x29, 24]
	ldr	x0, [x0]
	str	x0, [x29, 24]
.L214:
	ldr	x0, [x29, 24]
	cmp	x0, 0
	bne	.L215
	adrp	x0, .LC42
	add	x0, x0, :lo12:.LC42
	bl	nand_print_dbg
	mov	w0, 0
	ldp	x29, x30, [sp], 32
	ret
	.size	samsung_special_exit, .-samsung_special_exit
	.global	hynix20nm_read_retry_mode
	.bss
	.type	hynix20nm_read_retry_mode, %object
	.size	hynix20nm_read_retry_mode, 1
hynix20nm_read_retry_mode:
	.zero	1
	.global	hynix20nm_read_retry_cycle
	.type	hynix20nm_read_retry_cycle, %object
	.size	hynix20nm_read_retry_cycle, 1
hynix20nm_read_retry_cycle:
	.zero	1
	.global	hynix20nm_read_retry_reg_num
	.type	hynix20nm_read_retry_reg_num, %object
	.size	hynix20nm_read_retry_reg_num, 1
hynix20nm_read_retry_reg_num:
	.zero	1
	.global	hynix20nm_read_retry_reg_adr
	.align	3
	.type	hynix20nm_read_retry_reg_adr, %object
	.size	hynix20nm_read_retry_reg_adr, 8
hynix20nm_read_retry_reg_adr:
	.zero	8
	.global	hynix20nm_read_retry_reg_adr_1
	.data
	.align	3
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
	.global	hynix20nm_read_retry_reg_adr_2
	.align	3
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
	.global	hynix20nm_lsb_mode_reg_adr
	.bss
	.align	3
	.type	hynix20nm_lsb_mode_reg_adr, %object
	.size	hynix20nm_lsb_mode_reg_adr, 4
hynix20nm_lsb_mode_reg_adr:
	.zero	4
	.global	hynix20nm_lsb_mode_default_val
	.align	3
	.type	hynix20nm_lsb_mode_default_val, %object
	.size	hynix20nm_lsb_mode_default_val, 4
hynix20nm_lsb_mode_default_val:
	.zero	4
	.global	hynix20nm_lsb_mode_val
	.align	3
	.type	hynix20nm_lsb_mode_val, %object
	.size	hynix20nm_lsb_mode_val, 4
hynix20nm_lsb_mode_val:
	.zero	4
	.global	hynix20nm_lsb_mode_reg_num
	.type	hynix20nm_lsb_mode_reg_num, %object
	.size	hynix20nm_lsb_mode_reg_num, 1
hynix20nm_lsb_mode_reg_num:
	.zero	1
	.text
	.align	2
	.global	hynix20nm_vender_set_param
	.type	hynix20nm_vender_set_param, %function
hynix20nm_vender_set_param:
	stp	x29, x30, [sp, -64]!
	add	x29, sp, 0
	str	x0, [x29, 40]
	str	x1, [x29, 32]
	str	x2, [x29, 24]
	str	w3, [x29, 20]
	str	wzr, [x29, 56]
	mov	w0, 54
	strb	w0, [x29, 54]
	mov	w0, 22
	strb	w0, [x29, 55]
	ldr	x0, [x29, 40]
	bl	nand_enable_chip
	ldr	x0, [x29, 40]
	ldr	x0, [x0, 272]
	bl	ndfc_disable_randomize
	str	wzr, [x29, 60]
	b	.L218
.L219:
	ldr	w0, [x29, 60]
	ldr	x1, [x29, 24]
	add	x2, x1, x0
	ldr	w0, [x29, 60]
	ldr	x1, [x29, 32]
	add	x1, x1, x0
	add	x0, x29, 54
	mov	w6, 1
	mov	w5, 1
	mov	x4, x1
	mov	x3, x2
	mov	w2, 0
	mov	x1, x0
	ldr	x0, [x29, 40]
	bl	set_cmd_with_nand_bus
	mov	w2, 0
	ldrb	w1, [x29, 55]
	ldr	x0, [x29, 40]
	bl	set_one_cmd
	ldr	w0, [x29, 60]
	add	w0, w0, 1
	str	w0, [x29, 60]
.L218:
	ldr	w1, [x29, 60]
	ldr	w0, [x29, 20]
	cmp	w1, w0
	bcc	.L219
	ldr	x0, [x29, 40]
	bl	nand_disable_chip
	ldr	w0, [x29, 56]
	ldp	x29, x30, [sp], 64
	ret
	.size	hynix20nm_vender_set_param, .-hynix20nm_vender_set_param
	.section	.rodata
	.align	3
.LC43:
	.string	"retry para:"
	.align	3
.LC44:
	.string	" %x "
	.align	3
.LC45:
	.string	"\n"
	.text
	.align	2
	.global	hynix20nm_set_readretry
	.type	hynix20nm_set_readretry, %function
hynix20nm_set_readretry:
	stp	x29, x30, [sp, -48]!
	add	x29, sp, 0
	str	x0, [x29, 24]
	str	wzr, [x29, 40]
	ldr	x0, [x29, 24]
	ldr	w0, [x0, 184]
	adrp	x1, hynix20nm_read_retry_cycle
	add	x1, x1, :lo12:hynix20nm_read_retry_cycle
	ldrb	w1, [x1]
	cmp	w0, w1
	bls	.L222
	mov	w0, -92
	b	.L228
.L222:
	str	wzr, [x29, 44]
	b	.L224
.L225:
	ldr	x0, [x29, 24]
	ldr	w0, [x0, 184]
	lsl	w1, w0, 3
	ldr	w0, [x29, 44]
	add	w0, w1, w0
	ldr	x1, [x29, 24]
	uxtw	x0, w0
	add	x0, x1, x0
	ldrb	w2, [x0, 56]
	ldr	w0, [x29, 44]
	sub	x1, x29, #4048
	add	x0, x1, x0
	mov	w1, w2
	strb	w1, [x0, 4080]
	ldr	w0, [x29, 44]
	add	w0, w0, 1
	str	w0, [x29, 44]
.L224:
	adrp	x0, hynix20nm_read_retry_reg_num
	add	x0, x0, :lo12:hynix20nm_read_retry_reg_num
	ldrb	w0, [x0]
	mov	w1, w0
	ldr	w0, [x29, 44]
	cmp	w1, w0
	bhi	.L225
	adrp	x0, hynix20nm_read_retry_reg_num
	add	x0, x0, :lo12:hynix20nm_read_retry_reg_num
	ldrb	w0, [x0]
	mov	w2, w0
	adrp	x0, hynix20nm_read_retry_reg_adr
	add	x1, x0, :lo12:hynix20nm_read_retry_reg_adr
	add	x0, x29, 32
	mov	w3, w2
	mov	x2, x1
	mov	x1, x0
	ldr	x0, [x29, 24]
	bl	hynix20nm_vender_set_param
	str	w0, [x29, 40]
	adrp	x0, .LC43
	add	x0, x0, :lo12:.LC43
	bl	nand_print_dbg
	str	wzr, [x29, 44]
	b	.L226
.L227:
	ldr	w0, [x29, 44]
	sub	x1, x29, #4048
	add	x0, x1, x0
	ldrb	w0, [x0, 4080]
	mov	w1, w0
	adrp	x0, .LC44
	add	x0, x0, :lo12:.LC44
	bl	nand_print_dbg
	ldr	w0, [x29, 44]
	add	w0, w0, 1
	str	w0, [x29, 44]
.L226:
	adrp	x0, hynix20nm_read_retry_reg_num
	add	x0, x0, :lo12:hynix20nm_read_retry_reg_num
	ldrb	w0, [x0]
	mov	w1, w0
	ldr	w0, [x29, 44]
	cmp	w1, w0
	bhi	.L227
	adrp	x0, .LC45
	add	x0, x0, :lo12:.LC45
	bl	nand_print_dbg
	ldr	w0, [x29, 40]
.L228:
	ldp	x29, x30, [sp], 48
	ret
	.size	hynix20nm_set_readretry, .-hynix20nm_set_readretry
	.section	.rodata
	.align	3
.LC46:
	.string	"hynix20nm retry!\n"
	.align	3
.LC47:
	.string	"retry spare all 0xff! ch =%d, chip = %d  block = %d, page = %d, RetryCount = %d  \n"
	.align	3
.LC48:
	.string	"sdata:0x%x %x %x %x "
	.align	3
.LC49:
	.string	"hynix20nm ReadRetry ok! ch =%d, chip = %d  block = %d, page = %d, RetryCount = %d  \n"
	.text
	.align	2
	.global	hynix20nm_read_page_end
	.type	hynix20nm_read_page_end, %function
hynix20nm_read_page_end:
	stp	x29, x30, [sp, -48]!
	add	x29, sp, 0
	str	x0, [x29, 24]
	str	wzr, [x29, 44]
	str	wzr, [x29, 40]
	adrp	x0, g_nsi
	add	x0, x0, :lo12:g_nsi
	ldr	x2, [x0]
	ldr	x0, [x29, 24]
	ldr	w0, [x0]
	mov	w1, w0
	mov	x0, x2
	bl	nci_get_from_nsi
	str	x0, [x29, 32]
	ldr	x0, [x29, 24]
	bl	generic_read_page_end_not_retry
	str	w0, [x29, 40]
	ldr	w0, [x29, 40]
	cmn	w0, #2
	bne	.L230
	adrp	x0, .LC46
	add	x0, x0, :lo12:.LC46
	bl	nand_print_dbg
	str	wzr, [x29, 44]
	b	.L231
.L238:
	str	wzr, [x29, 40]
	ldr	x0, [x29, 32]
	ldr	w0, [x0, 184]
	add	w1, w0, 1
	ldr	x0, [x29, 32]
	str	w1, [x0, 184]
	ldr	x0, [x29, 32]
	ldr	w0, [x0, 184]
	adrp	x1, hynix20nm_read_retry_cycle
	add	x1, x1, :lo12:hynix20nm_read_retry_cycle
	ldrb	w1, [x1]
	cmp	w0, w1
	bls	.L232
	ldr	x0, [x29, 32]
	str	wzr, [x0, 184]
.L232:
	ldr	x0, [x29, 32]
	bl	hynix20nm_set_readretry
	str	w0, [x29, 40]
	ldr	w0, [x29, 40]
	cmp	w0, 0
	beq	.L233
	ldr	x0, [x29, 32]
	str	wzr, [x0, 184]
	ldr	x0, [x29, 32]
	bl	hynix20nm_set_readretry
	b	.L234
.L233:
	ldr	x0, [x29, 24]
	bl	generic_read_page_start
	str	w0, [x29, 40]
	ldr	x0, [x29, 24]
	bl	generic_read_page_end_not_retry
	mov	w1, w0
	ldr	w0, [x29, 40]
	orr	w0, w0, w1
	str	w0, [x29, 40]
	ldr	x0, [x29, 24]
	ldr	x0, [x0, 24]
	bl	is_nouse_page
	cmp	w0, 1
	bne	.L235
	mov	w0, -2
	str	w0, [x29, 40]
	ldr	x0, [x29, 32]
	ldr	x0, [x0, 272]
	ldr	w1, [x0, 12]
	ldr	x0, [x29, 32]
	ldr	w2, [x0, 28]
	ldr	x0, [x29, 24]
	ldr	w3, [x0, 4]
	ldr	x0, [x29, 24]
	ldr	w4, [x0, 8]
	ldr	x0, [x29, 32]
	ldr	w5, [x0, 184]
	adrp	x0, .LC47
	add	x0, x0, :lo12:.LC47
	bl	nand_print_dbg
	b	.L236
.L235:
	ldr	w0, [x29, 40]
	cmp	w0, 10
	beq	.L237
	ldr	w0, [x29, 40]
	cmp	w0, 0
	bne	.L236
.L237:
	mov	w0, 10
	str	w0, [x29, 40]
	ldr	x0, [x29, 24]
	ldr	x0, [x0, 24]
	add	x0, x0, 1
	ldrb	w0, [x0]
	mov	w1, w0
	ldr	x0, [x29, 24]
	ldr	x0, [x0, 24]
	add	x0, x0, 2
	ldrb	w0, [x0]
	mov	w2, w0
	ldr	x0, [x29, 24]
	ldr	x0, [x0, 24]
	add	x0, x0, 3
	ldrb	w0, [x0]
	mov	w3, w0
	ldr	x0, [x29, 24]
	ldr	x0, [x0, 24]
	add	x0, x0, 4
	ldrb	w0, [x0]
	mov	w4, w0
	adrp	x0, .LC48
	add	x0, x0, :lo12:.LC48
	bl	nand_print_dbg
	ldr	x0, [x29, 32]
	ldr	x0, [x0, 272]
	ldr	w1, [x0, 12]
	ldr	x0, [x29, 32]
	ldr	w2, [x0, 28]
	ldr	x0, [x29, 24]
	ldr	w3, [x0, 4]
	ldr	x0, [x29, 24]
	ldr	w4, [x0, 8]
	ldr	x0, [x29, 32]
	ldr	w5, [x0, 184]
	adrp	x0, .LC49
	add	x0, x0, :lo12:.LC49
	bl	nand_print_dbg
	b	.L234
.L236:
	ldr	w0, [x29, 44]
	add	w0, w0, 1
	str	w0, [x29, 44]
.L231:
	adrp	x0, hynix20nm_read_retry_cycle
	add	x0, x0, :lo12:hynix20nm_read_retry_cycle
	ldrb	w0, [x0]
	mov	w1, w0
	ldr	w0, [x29, 44]
	cmp	w1, w0
	bgt	.L238
.L234:
	ldr	x0, [x29, 32]
	str	wzr, [x0, 184]
	ldr	x0, [x29, 32]
	bl	hynix20nm_set_readretry
.L230:
	ldr	w0, [x29, 40]
	ldp	x29, x30, [sp], 48
	ret
	.size	hynix20nm_read_page_end, .-hynix20nm_read_page_end
	.align	2
	.global	hynix20nm_vender_get_param
	.type	hynix20nm_vender_get_param, %function
hynix20nm_vender_get_param:
	stp	x29, x30, [sp, -64]!
	add	x29, sp, 0
	str	x0, [x29, 40]
	str	x1, [x29, 32]
	str	x2, [x29, 24]
	str	w3, [x29, 20]
	str	wzr, [x29, 56]
	ldr	x0, [x29, 40]
	bl	nand_enable_chip
	ldr	x0, [x29, 40]
	ldr	x0, [x0, 272]
	bl	ndfc_disable_randomize
	mov	w0, 55
	strb	w0, [x29, 55]
	str	wzr, [x29, 60]
	b	.L241
.L242:
	ldr	w0, [x29, 60]
	ldr	x1, [x29, 24]
	add	x2, x1, x0
	ldr	w0, [x29, 60]
	ldr	x1, [x29, 32]
	add	x1, x1, x0
	add	x0, x29, 55
	mov	w4, 1
	mov	x3, x1
	mov	x1, x0
	ldr	x0, [x29, 40]
	bl	get_data_with_nand_bus_one_cmd
	ldr	w0, [x29, 60]
	add	w0, w0, 1
	str	w0, [x29, 60]
.L241:
	ldr	w1, [x29, 60]
	ldr	w0, [x29, 20]
	cmp	w1, w0
	bcc	.L242
	ldr	x0, [x29, 40]
	bl	nand_disable_chip
	ldr	w0, [x29, 56]
	ldp	x29, x30, [sp], 64
	ret
	.size	hynix20nm_vender_get_param, .-hynix20nm_vender_get_param
	.align	2
	.global	hynix20nm_lsb_init
	.type	hynix20nm_lsb_init, %function
hynix20nm_lsb_init:
	sub	sp, sp, #16
	str	x0, [sp, 8]
	ldr	x0, [sp, 8]
	ldr	x0, [x0, 280]
	ldr	w0, [x0, 52]
	lsr	w0, w0, 16
	uxtb	w1, w0
	adrp	x0, hynix20nm_read_retry_mode
	add	x0, x0, :lo12:hynix20nm_read_retry_mode
	strb	w1, [x0]
	ldr	x0, [sp, 8]
	ldr	x0, [x0, 280]
	ldr	w0, [x0, 52]
	lsr	w0, w0, 8
	uxtb	w1, w0
	adrp	x0, hynix20nm_read_retry_cycle
	add	x0, x0, :lo12:hynix20nm_read_retry_cycle
	strb	w1, [x0]
	ldr	x0, [sp, 8]
	ldr	x0, [x0, 280]
	ldr	w0, [x0, 52]
	uxtb	w1, w0
	adrp	x0, hynix20nm_read_retry_reg_num
	add	x0, x0, :lo12:hynix20nm_read_retry_reg_num
	strb	w1, [x0]
	adrp	x0, hynix20nm_read_retry_mode
	add	x0, x0, :lo12:hynix20nm_read_retry_mode
	ldrb	w0, [x0]
	cmp	w0, 2
	bne	.L245
	adrp	x0, hynix20nm_lsb_mode_reg_num
	add	x0, x0, :lo12:hynix20nm_lsb_mode_reg_num
	mov	w1, 4
	strb	w1, [x0]
	adrp	x0, hynix20nm_lsb_mode_reg_adr
	add	x0, x0, :lo12:hynix20nm_lsb_mode_reg_adr
	mov	w1, -80
	strb	w1, [x0]
	adrp	x0, hynix20nm_lsb_mode_reg_adr
	add	x0, x0, :lo12:hynix20nm_lsb_mode_reg_adr
	mov	w1, -79
	strb	w1, [x0, 1]
	adrp	x0, hynix20nm_lsb_mode_reg_adr
	add	x0, x0, :lo12:hynix20nm_lsb_mode_reg_adr
	mov	w1, -96
	strb	w1, [x0, 2]
	adrp	x0, hynix20nm_lsb_mode_reg_adr
	add	x0, x0, :lo12:hynix20nm_lsb_mode_reg_adr
	mov	w1, -95
	strb	w1, [x0, 3]
	adrp	x0, hynix20nm_lsb_mode_val
	add	x0, x0, :lo12:hynix20nm_lsb_mode_val
	mov	w1, 10
	strb	w1, [x0]
	adrp	x0, hynix20nm_lsb_mode_val
	add	x0, x0, :lo12:hynix20nm_lsb_mode_val
	mov	w1, 10
	strb	w1, [x0, 1]
	adrp	x0, hynix20nm_lsb_mode_val
	add	x0, x0, :lo12:hynix20nm_lsb_mode_val
	mov	w1, 10
	strb	w1, [x0, 2]
	adrp	x0, hynix20nm_lsb_mode_val
	add	x0, x0, :lo12:hynix20nm_lsb_mode_val
	mov	w1, 10
	strb	w1, [x0, 3]
	b	.L246
.L245:
	adrp	x0, hynix20nm_read_retry_mode
	add	x0, x0, :lo12:hynix20nm_read_retry_mode
	ldrb	w0, [x0]
	cmp	w0, 3
	bne	.L246
	adrp	x0, hynix20nm_lsb_mode_reg_num
	add	x0, x0, :lo12:hynix20nm_lsb_mode_reg_num
	mov	w1, 4
	strb	w1, [x0]
	adrp	x0, hynix20nm_lsb_mode_reg_adr
	add	x0, x0, :lo12:hynix20nm_lsb_mode_reg_adr
	mov	w1, -96
	strb	w1, [x0]
	adrp	x0, hynix20nm_lsb_mode_reg_adr
	add	x0, x0, :lo12:hynix20nm_lsb_mode_reg_adr
	mov	w1, -95
	strb	w1, [x0, 1]
	adrp	x0, hynix20nm_lsb_mode_reg_adr
	add	x0, x0, :lo12:hynix20nm_lsb_mode_reg_adr
	mov	w1, -89
	strb	w1, [x0, 2]
	adrp	x0, hynix20nm_lsb_mode_reg_adr
	add	x0, x0, :lo12:hynix20nm_lsb_mode_reg_adr
	mov	w1, -88
	strb	w1, [x0, 3]
	adrp	x0, hynix20nm_lsb_mode_val
	add	x0, x0, :lo12:hynix20nm_lsb_mode_val
	mov	w1, 10
	strb	w1, [x0]
	adrp	x0, hynix20nm_lsb_mode_val
	add	x0, x0, :lo12:hynix20nm_lsb_mode_val
	mov	w1, 10
	strb	w1, [x0, 1]
	adrp	x0, hynix20nm_lsb_mode_val
	add	x0, x0, :lo12:hynix20nm_lsb_mode_val
	mov	w1, 10
	strb	w1, [x0, 2]
	adrp	x0, hynix20nm_lsb_mode_val
	add	x0, x0, :lo12:hynix20nm_lsb_mode_val
	mov	w1, 10
	strb	w1, [x0, 3]
.L246:
	mov	w0, 0
	add	sp, sp, 16
	ret
	.size	hynix20nm_lsb_init, .-hynix20nm_lsb_init
	.align	2
	.global	hynix20nm_lsb_enable
	.type	hynix20nm_lsb_enable, %function
hynix20nm_lsb_enable:
	stp	x29, x30, [sp, -48]!
	add	x29, sp, 0
	str	x0, [x29, 24]
	adrp	x0, hynix20nm_lsb_mode_reg_num
	add	x0, x0, :lo12:hynix20nm_lsb_mode_reg_num
	ldrb	w0, [x0]
	mov	w2, w0
	adrp	x0, hynix20nm_lsb_mode_reg_adr
	add	x1, x0, :lo12:hynix20nm_lsb_mode_reg_adr
	adrp	x0, hynix20nm_lsb_mode_default_val
	add	x0, x0, :lo12:hynix20nm_lsb_mode_default_val
	mov	w3, w2
	mov	x2, x1
	mov	x1, x0
	ldr	x0, [x29, 24]
	bl	hynix20nm_vender_get_param
	str	wzr, [x29, 44]
	b	.L249
.L250:
	adrp	x0, hynix20nm_lsb_mode_default_val
	add	x1, x0, :lo12:hynix20nm_lsb_mode_default_val
	ldr	w0, [x29, 44]
	ldrb	w2, [x1, x0]
	ldr	w0, [x29, 44]
	sub	x1, x29, #4048
	add	x0, x1, x0
	mov	w1, w2
	strb	w1, [x0, 4088]
	ldr	w0, [x29, 44]
	sub	x1, x29, #4048
	add	x0, x1, x0
	ldrb	w1, [x0, 4088]
	adrp	x0, hynix20nm_lsb_mode_val
	add	x2, x0, :lo12:hynix20nm_lsb_mode_val
	ldr	w0, [x29, 44]
	ldrb	w0, [x2, x0]
	add	w0, w1, w0
	uxtb	w2, w0
	ldr	w0, [x29, 44]
	sub	x1, x29, #4048
	add	x0, x1, x0
	mov	w1, w2
	strb	w1, [x0, 4088]
	ldr	w0, [x29, 44]
	add	w0, w0, 1
	str	w0, [x29, 44]
.L249:
	adrp	x0, hynix20nm_lsb_mode_reg_num
	add	x0, x0, :lo12:hynix20nm_lsb_mode_reg_num
	ldrb	w0, [x0]
	mov	w1, w0
	ldr	w0, [x29, 44]
	cmp	w1, w0
	bhi	.L250
	adrp	x0, hynix20nm_lsb_mode_reg_num
	add	x0, x0, :lo12:hynix20nm_lsb_mode_reg_num
	ldrb	w0, [x0]
	mov	w2, w0
	adrp	x0, hynix20nm_lsb_mode_reg_adr
	add	x1, x0, :lo12:hynix20nm_lsb_mode_reg_adr
	add	x0, x29, 40
	mov	w3, w2
	mov	x2, x1
	mov	x1, x0
	ldr	x0, [x29, 24]
	bl	hynix20nm_vender_set_param
	mov	w0, 0
	ldp	x29, x30, [sp], 48
	ret
	.size	hynix20nm_lsb_enable, .-hynix20nm_lsb_enable
	.align	2
	.global	hynix20nm_lsb_disable
	.type	hynix20nm_lsb_disable, %function
hynix20nm_lsb_disable:
	stp	x29, x30, [sp, -32]!
	add	x29, sp, 0
	str	x0, [x29, 24]
	adrp	x0, hynix20nm_lsb_mode_reg_num
	add	x0, x0, :lo12:hynix20nm_lsb_mode_reg_num
	ldrb	w0, [x0]
	mov	w2, w0
	adrp	x0, hynix20nm_lsb_mode_reg_adr
	add	x1, x0, :lo12:hynix20nm_lsb_mode_reg_adr
	adrp	x0, hynix20nm_lsb_mode_default_val
	add	x0, x0, :lo12:hynix20nm_lsb_mode_default_val
	mov	w3, w2
	mov	x2, x1
	mov	x1, x0
	ldr	x0, [x29, 24]
	bl	hynix20nm_vender_set_param
	mov	w0, 0
	ldp	x29, x30, [sp], 32
	ret
	.size	hynix20nm_lsb_disable, .-hynix20nm_lsb_disable
	.section	.rodata
	.align	3
.LC50:
	.string	"hynix20nm_lsb_exit\n"
	.text
	.align	2
	.global	hynix20nm_lsb_exit
	.type	hynix20nm_lsb_exit, %function
hynix20nm_lsb_exit:
	stp	x29, x30, [sp, -48]!
	add	x29, sp, 0
	str	x0, [x29, 24]
	mov	w0, 1048575
	str	w0, [x29, 44]
	ldr	x0, [x29, 24]
	bl	nand_enable_chip
	ldr	x0, [x29, 24]
	ldr	x0, [x0, 272]
	bl	ndfc_disable_randomize
	mov	w2, 0
	mov	w1, 0
	ldr	x0, [x29, 24]
	bl	set_one_cmd
	mov	w1, 0
	ldr	x0, [x29, 24]
	bl	set_one_addr
	mov	w1, 0
	ldr	x0, [x29, 24]
	bl	set_one_addr
	mov	w1, 0
	ldr	x0, [x29, 24]
	bl	set_one_addr
	mov	w1, 0
	ldr	x0, [x29, 24]
	bl	set_one_addr
	mov	w1, 0
	ldr	x0, [x29, 24]
	bl	set_one_addr
	mov	w2, 1
	mov	w1, 48
	ldr	x0, [x29, 24]
	bl	set_one_cmd
	nop
.L255:
	ldr	w0, [x29, 44]
	sub	w1, w0, #1
	str	w1, [x29, 44]
	cmp	w0, 0
	bgt	.L255
	adrp	x0, .LC50
	add	x0, x0, :lo12:.LC50
	bl	nand_print_dbg
	ldr	x0, [x29, 24]
	bl	nand_disable_chip
	mov	w0, 0
	ldp	x29, x30, [sp], 48
	ret
	.size	hynix20nm_lsb_exit, .-hynix20nm_lsb_exit
	.section	.rodata
	.align	3
.LC51:
	.string	"hynix OTP RegCount value error: 0x%x, 0x%x \n"
	.align	3
.LC52:
	.string	"otp copy %d is ok!\n"
	.align	3
.LC53:
	.string	"otp copy failed!\n"
	.text
	.align	2
	.global	hynix20nm_vender_get_param_otp_hynix
	.type	hynix20nm_vender_get_param_otp_hynix, %function
hynix20nm_vender_get_param_otp_hynix:
	stp	x29, x30, [sp, -144]!
	add	x29, sp, 0
	str	x0, [x29, 24]
	str	wzr, [x29, 128]
	strb	wzr, [x29, 40]
	strb	wzr, [x29, 41]
	strb	wzr, [x29, 32]
	strb	wzr, [x29, 33]
	adrp	x0, hynix20nm_read_retry_mode
	add	x0, x0, :lo12:hynix20nm_read_retry_mode
	ldrb	w0, [x0]
	cmp	w0, 2
	bne	.L258
	mov	w0, -1
	strb	w0, [x29, 40]
	mov	w0, -52
	strb	w0, [x29, 41]
	mov	w0, 64
	strb	w0, [x29, 32]
	mov	w0, 77
	strb	w0, [x29, 33]
	b	.L259
.L258:
	adrp	x0, hynix20nm_read_retry_mode
	add	x0, x0, :lo12:hynix20nm_read_retry_mode
	ldrb	w0, [x0]
	cmp	w0, 3
	bne	.L260
	mov	w0, -82
	strb	w0, [x29, 40]
	mov	w0, -80
	strb	w0, [x29, 41]
	strb	wzr, [x29, 32]
	mov	w0, 77
	strb	w0, [x29, 33]
	b	.L259
.L260:
	mov	w0, -97
	b	.L273
.L259:
	mov	w0, 1024
	bl	nand_get_temp_buf
	str	x0, [x29, 120]
	ldr	x0, [x29, 24]
	bl	nand_enable_chip
	ldr	x0, [x29, 24]
	ldr	x0, [x0, 272]
	bl	ndfc_disable_randomize
	mov	w2, 1
	mov	w1, -1
	ldr	x0, [x29, 24]
	bl	set_one_cmd
	mov	w0, 54
	strb	w0, [x29, 112]
	add	x2, x29, 32
	add	x1, x29, 40
	add	x0, x29, 112
	mov	w6, 1
	mov	w5, 1
	mov	x4, x2
	mov	x3, x1
	mov	w2, 0
	mov	x1, x0
	ldr	x0, [x29, 24]
	bl	set_cmd_with_nand_bus
	add	x0, x29, 32
	add	x1, x0, 1
	add	x0, x29, 40
	add	x0, x0, 1
	mov	w6, 1
	mov	w5, 1
	mov	x4, x1
	mov	x3, x0
	mov	w2, 0
	mov	x1, 0
	ldr	x0, [x29, 24]
	bl	set_cmd_with_nand_bus
	mov	w2, 0
	mov	w1, 22
	ldr	x0, [x29, 24]
	bl	set_one_cmd
	mov	w2, 0
	mov	w1, 23
	ldr	x0, [x29, 24]
	bl	set_one_cmd
	mov	w2, 0
	mov	w1, 4
	ldr	x0, [x29, 24]
	bl	set_one_cmd
	mov	w2, 0
	mov	w1, 25
	ldr	x0, [x29, 24]
	bl	set_one_cmd
	mov	w2, 0
	mov	w1, 0
	ldr	x0, [x29, 24]
	bl	set_one_cmd
	mov	w1, 0
	ldr	x0, [x29, 24]
	bl	set_one_addr
	mov	w1, 0
	ldr	x0, [x29, 24]
	bl	set_one_addr
	mov	w1, 0
	ldr	x0, [x29, 24]
	bl	set_one_addr
	mov	w1, 2
	ldr	x0, [x29, 24]
	bl	set_one_addr
	mov	w1, 0
	ldr	x0, [x29, 24]
	bl	set_one_addr
	mov	w0, 48
	strb	w0, [x29, 112]
	add	x0, x29, 112
	mov	w4, 2
	ldr	x3, [x29, 120]
	mov	x2, 0
	mov	x1, x0
	ldr	x0, [x29, 24]
	bl	get_data_with_nand_bus_one_cmd
	ldr	x0, [x29, 120]
	ldrb	w0, [x0]
	cmp	w0, 8
	bne	.L262
	ldr	x0, [x29, 120]
	add	x0, x0, 1
	ldrb	w0, [x0]
	cmp	w0, 8
	beq	.L263
.L262:
	ldr	x0, [x29, 120]
	ldrb	w0, [x0]
	mov	w1, w0
	ldr	x0, [x29, 120]
	add	x0, x0, 1
	ldrb	w0, [x0]
	mov	w2, w0
	adrp	x0, .LC51
	add	x0, x0, :lo12:.LC51
	bl	nand_print
	mov	w0, -96
	str	w0, [x29, 128]
.L263:
	mov	w4, 1024
	ldr	x3, [x29, 120]
	mov	x2, 0
	mov	x1, 0
	ldr	x0, [x29, 24]
	bl	get_data_with_nand_bus_one_cmd
	str	wzr, [x29, 136]
	b	.L264
.L271:
	str	wzr, [x29, 132]
	str	wzr, [x29, 140]
	b	.L265
.L268:
	ldr	w0, [x29, 136]
	lsl	w0, w0, 7
	uxtw	x1, w0
	ldr	w0, [x29, 140]
	add	x0, x1, x0
	ldr	x1, [x29, 120]
	add	x0, x1, x0
	ldrb	w2, [x0]
	ldr	x1, [x29, 24]
	ldr	w0, [x29, 140]
	add	x0, x1, x0
	mov	w1, w2
	strb	w1, [x0, 56]
	ldr	w0, [x29, 136]
	lsl	w0, w0, 7
	uxtw	x1, w0
	ldr	w0, [x29, 140]
	add	x0, x1, x0
	add	x0, x0, 64
	ldr	x1, [x29, 120]
	add	x0, x1, x0
	ldrb	w2, [x0]
	ldr	w0, [x29, 140]
	sub	x1, x29, #3952
	add	x0, x1, x0
	mov	w1, w2
	strb	w1, [x0, 4000]
	ldr	x1, [x29, 24]
	ldr	w0, [x29, 140]
	add	x0, x1, x0
	ldrb	w0, [x0, 56]
	mov	w2, w0
	ldr	w0, [x29, 140]
	sub	x1, x29, #3952
	add	x0, x1, x0
	ldrb	w0, [x0, 4000]
	add	w0, w2, w0
	cmp	w0, 255
	beq	.L266
	mov	w0, 1
	str	w0, [x29, 132]
	b	.L267
.L266:
	ldr	w0, [x29, 140]
	add	w0, w0, 1
	str	w0, [x29, 140]
.L265:
	ldr	w0, [x29, 140]
	cmp	w0, 63
	bls	.L268
.L267:
	ldr	w0, [x29, 132]
	cmp	w0, 0
	bne	.L269
	adrp	x0, .LC52
	add	x0, x0, :lo12:.LC52
	ldr	w1, [x29, 136]
	bl	nand_print_dbg
	b	.L270
.L269:
	ldr	w0, [x29, 136]
	add	w0, w0, 1
	str	w0, [x29, 136]
.L264:
	ldr	w0, [x29, 136]
	cmp	w0, 7
	bls	.L271
.L270:
	ldr	w0, [x29, 132]
	cmp	w0, 0
	beq	.L272
	adrp	x0, .LC53
	add	x0, x0, :lo12:.LC53
	bl	nand_print_dbg
	mov	w0, -95
	str	w0, [x29, 128]
.L272:
	mov	w2, 1
	mov	w1, -1
	ldr	x0, [x29, 24]
	bl	set_one_cmd
	mov	w2, 1
	mov	w1, 56
	ldr	x0, [x29, 24]
	bl	set_one_cmd
	ldr	x0, [x29, 24]
	bl	nand_disable_chip
	ldr	x0, [x29, 120]
	bl	nand_free_temp_buf
	ldr	w0, [x29, 128]
.L273:
	ldp	x29, x30, [sp], 144
	ret
	.size	hynix20nm_vender_get_param_otp_hynix, .-hynix20nm_vender_get_param_otp_hynix
	.align	2
	.global	hynix20nm_saveotpvalue
	.type	hynix20nm_saveotpvalue, %function
hynix20nm_saveotpvalue:
	sub	sp, sp, #32
	str	x0, [sp, 8]
	str	x1, [sp]
	str	wzr, [sp, 28]
	b	.L275
.L276:
	ldr	w0, [sp, 28]
	ldr	x1, [sp]
	add	x0, x1, x0
	ldrb	w2, [x0]
	ldr	x1, [sp, 8]
	ldr	w0, [sp, 28]
	add	x0, x1, x0
	mov	w1, w2
	strb	w1, [x0, 56]
	ldr	w0, [sp, 28]
	add	w0, w0, 1
	str	w0, [sp, 28]
.L275:
	ldr	w0, [sp, 28]
	cmp	w0, 63
	bls	.L276
	nop
	add	sp, sp, 32
	ret
	.size	hynix20nm_saveotpvalue, .-hynix20nm_saveotpvalue
	.section	.rodata
	.align	3
.LC54:
	.string	"_vender_get_param_otp_hynix time %d!\n"
	.align	3
.LC55:
	.string	"_vender_get_param_otp_hynix error!\n"
	.text
	.align	2
	.global	hynix20nm_getotpparam
	.type	hynix20nm_getotpparam, %function
hynix20nm_getotpparam:
	stp	x29, x30, [sp, -48]!
	add	x29, sp, 0
	str	x0, [x29, 24]
	str	x1, [x29, 16]
	str	wzr, [x29, 32]
	b	.L278
.L281:
	adrp	x0, .LC54
	add	x0, x0, :lo12:.LC54
	ldr	w1, [x29, 32]
	bl	nand_print_dbg
	ldr	x0, [x29, 24]
	bl	hynix20nm_vender_get_param_otp_hynix
	str	w0, [x29, 44]
	ldr	w0, [x29, 44]
	cmp	w0, 0
	beq	.L288
	ldr	w0, [x29, 32]
	add	w0, w0, 1
	str	w0, [x29, 32]
.L278:
	ldr	w0, [x29, 32]
	cmp	w0, 4
	bls	.L281
	b	.L280
.L288:
	nop
.L280:
	ldr	w0, [x29, 44]
	cmp	w0, 0
	beq	.L282
	adrp	x0, .LC55
	add	x0, x0, :lo12:.LC55
	bl	nand_print
	ldr	w0, [x29, 44]
	b	.L283
.L282:
	str	wzr, [x29, 40]
	b	.L284
.L287:
	str	wzr, [x29, 36]
	b	.L285
.L286:
	ldr	w0, [x29, 40]
	lsl	w1, w0, 3
	ldr	w0, [x29, 36]
	add	w0, w1, w0
	uxtw	x0, w0
	ldr	x1, [x29, 16]
	add	x0, x1, x0
	ldr	w1, [x29, 40]
	lsl	w2, w1, 3
	ldr	w1, [x29, 36]
	add	w1, w2, w1
	ldr	x2, [x29, 24]
	uxtw	x1, w1
	add	x1, x2, x1
	ldrb	w1, [x1, 56]
	strb	w1, [x0]
	ldr	w0, [x29, 36]
	add	w0, w0, 1
	str	w0, [x29, 36]
.L285:
	ldr	w0, [x29, 36]
	cmp	w0, 7
	bls	.L286
	ldr	w0, [x29, 40]
	add	w0, w0, 1
	str	w0, [x29, 40]
.L284:
	ldr	w0, [x29, 40]
	cmp	w0, 7
	bls	.L287
	mov	w0, 0
.L283:
	ldp	x29, x30, [sp], 48
	ret
	.size	hynix20nm_getotpparam, .-hynix20nm_getotpparam
	.section	.rodata
	.align	3
.LC56:
	.string	"set retry default value: "
	.align	3
.LC57:
	.string	" %x"
	.text
	.align	2
	.global	hynix20nm_set_default_param
	.type	hynix20nm_set_default_param, %function
hynix20nm_set_default_param:
	stp	x29, x30, [sp, -48]!
	add	x29, sp, 0
	str	x0, [x29, 24]
	str	wzr, [x29, 40]
	str	wzr, [x29, 44]
	b	.L290
.L291:
	ldr	x1, [x29, 24]
	ldr	w0, [x29, 44]
	add	x0, x1, x0
	ldrb	w2, [x0, 56]
	ldr	w0, [x29, 44]
	sub	x1, x29, #4048
	add	x0, x1, x0
	mov	w1, w2
	strb	w1, [x0, 4080]
	ldr	w0, [x29, 44]
	add	w0, w0, 1
	str	w0, [x29, 44]
.L290:
	adrp	x0, hynix20nm_read_retry_reg_num
	add	x0, x0, :lo12:hynix20nm_read_retry_reg_num
	ldrb	w0, [x0]
	mov	w1, w0
	ldr	w0, [x29, 44]
	cmp	w1, w0
	bhi	.L291
	adrp	x0, hynix20nm_read_retry_reg_num
	add	x0, x0, :lo12:hynix20nm_read_retry_reg_num
	ldrb	w0, [x0]
	mov	w2, w0
	adrp	x0, hynix20nm_read_retry_reg_adr
	add	x1, x0, :lo12:hynix20nm_read_retry_reg_adr
	add	x0, x29, 32
	mov	w3, w2
	mov	x2, x1
	mov	x1, x0
	ldr	x0, [x29, 24]
	bl	hynix20nm_vender_set_param
	str	w0, [x29, 40]
	adrp	x0, .LC56
	add	x0, x0, :lo12:.LC56
	bl	nand_print_dbg
	str	wzr, [x29, 44]
	b	.L292
.L293:
	ldr	w0, [x29, 44]
	sub	x1, x29, #4048
	add	x0, x1, x0
	ldrb	w0, [x0, 4080]
	mov	w1, w0
	adrp	x0, .LC57
	add	x0, x0, :lo12:.LC57
	bl	nand_print_dbg
	ldr	w0, [x29, 44]
	add	w0, w0, 1
	str	w0, [x29, 44]
.L292:
	adrp	x0, hynix20nm_read_retry_reg_num
	add	x0, x0, :lo12:hynix20nm_read_retry_reg_num
	ldrb	w0, [x0]
	mov	w1, w0
	ldr	w0, [x29, 44]
	cmp	w1, w0
	bhi	.L293
	adrp	x0, .LC45
	add	x0, x0, :lo12:.LC45
	bl	nand_print_dbg
	ldr	w0, [x29, 40]
	ldp	x29, x30, [sp], 48
	ret
	.size	hynix20nm_set_default_param, .-hynix20nm_set_default_param
	.section	.rodata
	.align	3
.LC58:
	.string	"hynix20nm get default param error !\n"
	.align	3
.LC59:
	.string	"[PHY_GetDefaultParam]:readbuf malloc fail\n"
	.align	3
.LC60:
	.string	"ch %d, chip %d Read Retry value Table from uboot:\n"
	.align	3
.LC61:
	.string	"0x%x "
	.align	3
.LC62:
	.string	"[PHY_DBG] ch %d, can't get right otp value from nand otp blocks, then use otp command\n"
	.align	3
.LC63:
	.string	"[PHY_DBG] repair ch %d otp value end\n"
	.text
	.align	2
	.global	hynix20nm_init_readretry_param
	.type	hynix20nm_init_readretry_param, %function
hynix20nm_init_readretry_param:
	stp	x29, x30, [sp, -128]!
	add	x29, sp, 0
	str	x0, [x29, 24]
	str	wzr, [x29, 124]
	str	xzr, [x29, 112]
	str	xzr, [x29, 104]
	str	wzr, [x29, 120]
	str	wzr, [x29, 100]
	ldr	x0, [x29, 24]
	ldr	x0, [x0, 280]
	ldr	w0, [x0, 20]
	lsl	w0, w0, 9
	bl	nand_get_temp_buf
	str	x0, [x29, 112]
	ldr	x0, [x29, 112]
	cmp	x0, 0
	bne	.L296
	adrp	x0, .LC58
	add	x0, x0, :lo12:.LC58
	bl	nand_print_dbg
	mov	w0, -94
	b	.L305
.L296:
	mov	w0, 32768
	bl	nand_get_temp_buf
	str	x0, [x29, 104]
	ldr	x0, [x29, 104]
	cmp	x0, 0
	bne	.L298
	ldr	x0, [x29, 112]
	bl	nand_free_temp_buf
	adrp	x0, .LC59
	add	x0, x0, :lo12:.LC59
	bl	nand_print
	mov	w0, -76
	b	.L305
.L298:
	str	wzr, [x29, 100]
	ldr	x0, [x29, 24]
	ldr	w0, [x0, 24]
	mov	w3, w0
	mov	w2, 64
	ldr	x1, [x29, 112]
	ldr	x0, [x29, 104]
	bl	hynix16nm_get_special_info
	str	w0, [x29, 100]
	ldr	w0, [x29, 100]
	cmp	w0, 0
	bne	.L299
	ldr	x0, [x29, 24]
	ldr	x0, [x0, 272]
	ldr	w1, [x0, 12]
	ldr	x0, [x29, 24]
	ldr	w2, [x0, 24]
	adrp	x0, .LC60
	add	x0, x0, :lo12:.LC60
	bl	nand_print_dbg
	str	wzr, [x29, 124]
	b	.L300
.L302:
	ldr	w0, [x29, 124]
	ldr	x1, [x29, 112]
	add	x0, x1, x0
	ldrb	w0, [x0]
	mov	w1, w0
	adrp	x0, .LC61
	add	x0, x0, :lo12:.LC61
	bl	nand_print_dbg
	ldr	w0, [x29, 124]
	and	w0, w0, 7
	cmp	w0, 7
	bne	.L301
	adrp	x0, .LC45
	add	x0, x0, :lo12:.LC45
	bl	nand_print_dbg
.L301:
	ldr	w0, [x29, 124]
	add	w0, w0, 1
	str	w0, [x29, 124]
.L300:
	ldr	w0, [x29, 124]
	cmp	w0, 63
	bls	.L302
	ldr	x1, [x29, 112]
	ldr	x0, [x29, 24]
	bl	hynix20nm_saveotpvalue
	ldr	x0, [x29, 24]
	bl	hynix20nm_set_default_param
	str	w0, [x29, 120]
	b	.L303
.L299:
	ldr	x0, [x29, 24]
	ldr	x0, [x0, 272]
	ldr	w1, [x0, 12]
	adrp	x0, .LC62
	add	x0, x0, :lo12:.LC62
	bl	nand_print_dbg
	add	x0, x29, 32
	mov	x1, x0
	ldr	x0, [x29, 24]
	bl	hynix20nm_getotpparam
	str	w0, [x29, 120]
	ldr	w0, [x29, 120]
	cmp	w0, 0
	bne	.L306
	ldr	x0, [x29, 24]
	bl	hynix20nm_set_default_param
	ldr	x0, [x29, 24]
	ldr	x0, [x0, 272]
	ldr	w1, [x0, 12]
	adrp	x0, .LC63
	add	x0, x0, :lo12:.LC63
	bl	nand_print_dbg
	b	.L303
.L306:
	nop
.L303:
	ldr	x0, [x29, 112]
	bl	nand_free_temp_buf
	ldr	w0, [x29, 120]
.L305:
	ldp	x29, x30, [sp], 128
	ret
	.size	hynix20nm_init_readretry_param, .-hynix20nm_init_readretry_param
	.section	.rodata
	.align	3
.LC64:
	.string	"NFC_ReadRetryInit, unknown read retry mode 0x%x\n"
	.text
	.align	2
	.global	hynix20nm_readretry_init
	.type	hynix20nm_readretry_init, %function
hynix20nm_readretry_init:
	stp	x29, x30, [sp, -48]!
	add	x29, sp, 0
	str	x0, [x29, 24]
	ldr	x0, [x29, 24]
	ldr	x0, [x0, 280]
	ldr	w0, [x0, 52]
	lsr	w0, w0, 16
	uxtb	w1, w0
	adrp	x0, hynix20nm_read_retry_mode
	add	x0, x0, :lo12:hynix20nm_read_retry_mode
	strb	w1, [x0]
	ldr	x0, [x29, 24]
	ldr	x0, [x0, 280]
	ldr	w0, [x0, 52]
	lsr	w0, w0, 8
	uxtb	w1, w0
	adrp	x0, hynix20nm_read_retry_cycle
	add	x0, x0, :lo12:hynix20nm_read_retry_cycle
	strb	w1, [x0]
	ldr	x0, [x29, 24]
	ldr	x0, [x0, 280]
	ldr	w0, [x0, 52]
	uxtb	w1, w0
	adrp	x0, hynix20nm_read_retry_reg_num
	add	x0, x0, :lo12:hynix20nm_read_retry_reg_num
	strb	w1, [x0]
	adrp	x0, hynix20nm_read_retry_mode
	add	x0, x0, :lo12:hynix20nm_read_retry_mode
	ldrb	w0, [x0]
	cmp	w0, 2
	bne	.L308
	adrp	x0, hynix20nm_read_retry_reg_adr
	add	x0, x0, :lo12:hynix20nm_read_retry_reg_adr
	mov	w1, -52
	strb	w1, [x0]
	adrp	x0, hynix20nm_read_retry_reg_adr
	add	x0, x0, :lo12:hynix20nm_read_retry_reg_adr
	mov	w1, -65
	strb	w1, [x0, 1]
	adrp	x0, hynix20nm_read_retry_reg_adr
	add	x0, x0, :lo12:hynix20nm_read_retry_reg_adr
	mov	w1, -86
	strb	w1, [x0, 2]
	adrp	x0, hynix20nm_read_retry_reg_adr
	add	x0, x0, :lo12:hynix20nm_read_retry_reg_adr
	mov	w1, -85
	strb	w1, [x0, 3]
	adrp	x0, hynix20nm_read_retry_reg_adr
	add	x0, x0, :lo12:hynix20nm_read_retry_reg_adr
	mov	w1, -51
	strb	w1, [x0, 4]
	adrp	x0, hynix20nm_read_retry_reg_adr
	add	x0, x0, :lo12:hynix20nm_read_retry_reg_adr
	mov	w1, -83
	strb	w1, [x0, 5]
	adrp	x0, hynix20nm_read_retry_reg_adr
	add	x0, x0, :lo12:hynix20nm_read_retry_reg_adr
	mov	w1, -82
	strb	w1, [x0, 6]
	adrp	x0, hynix20nm_read_retry_reg_adr
	add	x0, x0, :lo12:hynix20nm_read_retry_reg_adr
	mov	w1, -81
	strb	w1, [x0, 7]
	b	.L309
.L308:
	adrp	x0, hynix20nm_read_retry_mode
	add	x0, x0, :lo12:hynix20nm_read_retry_mode
	ldrb	w0, [x0]
	cmp	w0, 3
	bne	.L310
	adrp	x0, hynix20nm_read_retry_reg_adr
	add	x0, x0, :lo12:hynix20nm_read_retry_reg_adr
	mov	w1, -80
	strb	w1, [x0]
	adrp	x0, hynix20nm_read_retry_reg_adr
	add	x0, x0, :lo12:hynix20nm_read_retry_reg_adr
	mov	w1, -79
	strb	w1, [x0, 1]
	adrp	x0, hynix20nm_read_retry_reg_adr
	add	x0, x0, :lo12:hynix20nm_read_retry_reg_adr
	mov	w1, -78
	strb	w1, [x0, 2]
	adrp	x0, hynix20nm_read_retry_reg_adr
	add	x0, x0, :lo12:hynix20nm_read_retry_reg_adr
	mov	w1, -77
	strb	w1, [x0, 3]
	adrp	x0, hynix20nm_read_retry_reg_adr
	add	x0, x0, :lo12:hynix20nm_read_retry_reg_adr
	mov	w1, -76
	strb	w1, [x0, 4]
	adrp	x0, hynix20nm_read_retry_reg_adr
	add	x0, x0, :lo12:hynix20nm_read_retry_reg_adr
	mov	w1, -75
	strb	w1, [x0, 5]
	adrp	x0, hynix20nm_read_retry_reg_adr
	add	x0, x0, :lo12:hynix20nm_read_retry_reg_adr
	mov	w1, -74
	strb	w1, [x0, 6]
	adrp	x0, hynix20nm_read_retry_reg_adr
	add	x0, x0, :lo12:hynix20nm_read_retry_reg_adr
	mov	w1, -73
	strb	w1, [x0, 7]
	b	.L309
.L310:
	adrp	x0, hynix20nm_read_retry_mode
	add	x0, x0, :lo12:hynix20nm_read_retry_mode
	ldrb	w0, [x0]
	mov	w1, w0
	adrp	x0, .LC64
	add	x0, x0, :lo12:.LC64
	bl	nand_print
	mov	w0, -93
	b	.L311
.L309:
	ldr	x0, [x29, 24]
	str	wzr, [x0, 184]
	str	wzr, [x29, 44]
	b	.L312
.L313:
	ldr	x1, [x29, 24]
	ldrsw	x0, [x29, 44]
	add	x0, x1, x0
	strb	wzr, [x0, 56]
	ldr	w0, [x29, 44]
	add	w0, w0, 1
	str	w0, [x29, 44]
.L312:
	ldr	w0, [x29, 44]
	cmp	w0, 127
	ble	.L313
	ldr	x0, [x29, 24]
	bl	hynix20nm_init_readretry_param
	str	w0, [x29, 40]
	ldr	w0, [x29, 40]
.L311:
	ldp	x29, x30, [sp], 48
	ret
	.size	hynix20nm_readretry_init, .-hynix20nm_readretry_init
	.section	.rodata
	.align	3
.LC65:
	.string	"hynix20nm_readretry_exit. \n"
	.text
	.align	2
	.global	hynix20nm_readretry_exit
	.type	hynix20nm_readretry_exit, %function
hynix20nm_readretry_exit:
	stp	x29, x30, [sp, -32]!
	add	x29, sp, 0
	str	x0, [x29, 24]
	adrp	x0, .LC65
	add	x0, x0, :lo12:.LC65
	bl	nand_print_dbg
	ldr	x0, [x29, 24]
	bl	hynix20nm_set_default_param
	ldr	x0, [x29, 24]
	str	wzr, [x0, 184]
	mov	w0, 0
	ldp	x29, x30, [sp], 32
	ret
	.size	hynix20nm_readretry_exit, .-hynix20nm_readretry_exit
	.section	.rodata
	.align	3
.LC66:
	.string	" hynix20nm_special_init hynix20nm_read_retry_mode: %d hynix20nm_read_retry_cycle:%d hynix20nm_read_retry_reg_num:%d\n"
	.align	3
.LC67:
	.string	" hynix20nm_special_init error hynix20nm_read_retry_mode: %d hynix20nm_read_retry_cycle:%d hynix20nm_read_retry_reg_num:%d\n"
	.text
	.align	2
	.global	hynix20nm_special_init
	.type	hynix20nm_special_init, %function
hynix20nm_special_init:
	stp	x29, x30, [sp, -32]!
	add	x29, sp, 0
	str	wzr, [x29, 28]
	adrp	x0, g_nsi
	add	x0, x0, :lo12:g_nsi
	ldr	x0, [x0]
	ldr	x0, [x0, 8]
	str	x0, [x29, 16]
	b	.L317
.L318:
	ldr	x0, [x29, 16]
	bl	hynix20nm_readretry_init
	mov	w1, w0
	ldr	w0, [x29, 28]
	orr	w0, w0, w1
	str	w0, [x29, 28]
	ldr	x0, [x29, 16]
	ldr	x0, [x0]
	str	x0, [x29, 16]
.L317:
	ldr	x0, [x29, 16]
	cmp	x0, 0
	bne	.L318
	ldr	w0, [x29, 28]
	cmp	w0, 0
	bne	.L319
	adrp	x0, df_read_page_end
	add	x0, x0, :lo12:df_read_page_end
	adrp	x1, hynix20nm_read_page_end
	add	x1, x1, :lo12:hynix20nm_read_page_end
	str	x1, [x0]
	adrp	x0, hynix20nm_read_retry_mode
	add	x0, x0, :lo12:hynix20nm_read_retry_mode
	ldrb	w0, [x0]
	mov	w1, w0
	adrp	x0, hynix20nm_read_retry_cycle
	add	x0, x0, :lo12:hynix20nm_read_retry_cycle
	ldrb	w0, [x0]
	mov	w2, w0
	adrp	x0, hynix20nm_read_retry_reg_num
	add	x0, x0, :lo12:hynix20nm_read_retry_reg_num
	ldrb	w0, [x0]
	mov	w3, w0
	adrp	x0, .LC66
	add	x0, x0, :lo12:.LC66
	bl	nand_print_dbg
	b	.L320
.L319:
	adrp	x0, hynix20nm_read_retry_mode
	add	x0, x0, :lo12:hynix20nm_read_retry_mode
	ldrb	w0, [x0]
	mov	w1, w0
	adrp	x0, hynix20nm_read_retry_cycle
	add	x0, x0, :lo12:hynix20nm_read_retry_cycle
	ldrb	w0, [x0]
	mov	w2, w0
	adrp	x0, hynix20nm_read_retry_reg_num
	add	x0, x0, :lo12:hynix20nm_read_retry_reg_num
	ldrb	w0, [x0]
	mov	w3, w0
	adrp	x0, .LC67
	add	x0, x0, :lo12:.LC67
	bl	nand_print
.L320:
	ldr	w0, [x29, 28]
	ldp	x29, x30, [sp], 32
	ret
	.size	hynix20nm_special_init, .-hynix20nm_special_init
	.section	.rodata
	.align	3
.LC68:
	.string	" hynix20nm_special_exit \n"
	.text
	.align	2
	.global	hynix20nm_special_exit
	.type	hynix20nm_special_exit, %function
hynix20nm_special_exit:
	stp	x29, x30, [sp, -32]!
	add	x29, sp, 0
	adrp	x0, g_nsi
	add	x0, x0, :lo12:g_nsi
	ldr	x0, [x0]
	ldr	x0, [x0, 8]
	str	x0, [x29, 24]
	b	.L323
.L324:
	ldr	x0, [x29, 24]
	bl	hynix20nm_readretry_exit
	ldr	x0, [x29, 24]
	ldr	x0, [x0]
	str	x0, [x29, 24]
.L323:
	ldr	x0, [x29, 24]
	cmp	x0, 0
	bne	.L324
	adrp	x0, .LC68
	add	x0, x0, :lo12:.LC68
	bl	nand_print_dbg
	mov	w0, 0
	ldp	x29, x30, [sp], 32
	ret
	.size	hynix20nm_special_exit, .-hynix20nm_special_exit
	.global	micron_read_retry_mode
	.bss
	.type	micron_read_retry_mode, %object
	.size	micron_read_retry_mode, 1
micron_read_retry_mode:
	.zero	1
	.global	micron_read_retry_cycle
	.type	micron_read_retry_cycle, %object
	.size	micron_read_retry_cycle, 1
micron_read_retry_cycle:
	.zero	1
	.global	micron_p1_1
	.data
	.align	3
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
	.global	micron_p1_2
	.align	3
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
	.global	micron_p1_3
	.align	3
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
	.global	micron_p1_4
	.align	3
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
	.local	micron_p1
	.comm	micron_p1,8,8
	.section	.rodata
	.align	3
.LC69:
	.string	"set feature(addr %d) p0: %d readretry Configuration failed!\n"
	.text
	.align	2
	.global	micron_intel_set_feature
	.type	micron_intel_set_feature, %function
micron_intel_set_feature:
	stp	x29, x30, [sp, -48]!
	add	x29, sp, 0
	str	x0, [x29, 24]
	strb	w1, [x29, 23]
	strb	w2, [x29, 22]
	ldrb	w0, [x29, 23]
	strb	w0, [x29, 47]
	ldrb	w0, [x29, 22]
	strb	w0, [x29, 40]
	strb	wzr, [x29, 41]
	strb	wzr, [x29, 42]
	strb	wzr, [x29, 43]
	add	x1, x29, 40
	add	x0, x29, 23
	mov	x2, x1
	mov	x1, x0
	ldr	x0, [x29, 24]
	bl	nand_set_feature
	add	x1, x29, 32
	add	x0, x29, 23
	mov	x2, x1
	mov	x1, x0
	ldr	x0, [x29, 24]
	bl	nand_get_feature
	ldrb	w1, [x29, 32]
	ldrb	w0, [x29, 40]
	cmp	w1, w0
	beq	.L327
	ldrb	w1, [x29, 47]
	ldrb	w2, [x29, 22]
	adrp	x0, .LC69
	add	x0, x0, :lo12:.LC69
	bl	nand_print
	mov	w0, -72
	b	.L329
.L327:
	mov	w0, 0
.L329:
	ldp	x29, x30, [sp], 48
	ret
	.size	micron_intel_set_feature, .-micron_intel_set_feature
	.align	2
	.global	micron_set_readretry
	.type	micron_set_readretry, %function
micron_set_readretry:
	stp	x29, x30, [sp, -48]!
	add	x29, sp, 0
	str	x0, [x29, 24]
	str	wzr, [x29, 44]
	ldr	x0, [x29, 24]
	bl	nand_enable_chip
	ldr	x0, [x29, 24]
	ldr	x0, [x0, 272]
	bl	ndfc_disable_randomize
	adrp	x0, micron_read_retry_mode
	add	x0, x0, :lo12:micron_read_retry_mode
	ldrb	w0, [x0]
	cmp	w0, 64
	beq	.L331
	adrp	x0, micron_read_retry_mode
	add	x0, x0, :lo12:micron_read_retry_mode
	ldrb	w0, [x0]
	cmp	w0, 65
	beq	.L331
	adrp	x0, micron_read_retry_mode
	add	x0, x0, :lo12:micron_read_retry_mode
	ldrb	w0, [x0]
	cmp	w0, 80
	beq	.L331
	adrp	x0, micron_read_retry_mode
	add	x0, x0, :lo12:micron_read_retry_mode
	ldrb	w0, [x0]
	cmp	w0, 66
	bne	.L338
.L331:
	ldr	x0, [x29, 24]
	ldr	w0, [x0, 184]
	strb	w0, [x29, 43]
	ldrb	w0, [x29, 43]
	cmp	w0, 255
	bne	.L333
	strb	wzr, [x29, 43]
.L333:
	adrp	x0, micron_read_retry_mode
	add	x0, x0, :lo12:micron_read_retry_mode
	ldrb	w0, [x0]
	cmp	w0, 80
	bne	.L334
	adrp	x0, micron_p1
	add	x0, x0, :lo12:micron_p1
	ldr	x1, [x0]
	ldrb	w0, [x29, 43]
	add	x0, x1, x0
	ldrb	w0, [x0]
	cmp	w0, 0
	bne	.L335
	mov	w2, 1
	mov	w1, -109
	ldr	x0, [x29, 24]
	bl	micron_intel_set_feature
	str	w0, [x29, 44]
	ldr	w0, [x29, 44]
	cmp	w0, 0
	bne	.L339
.L335:
	ldr	x0, [x29, 24]
	ldr	w0, [x0, 184]
	cmp	w0, 255
	bne	.L334
	mov	w2, 0
	mov	w1, -119
	ldr	x0, [x29, 24]
	bl	micron_intel_set_feature
	str	w0, [x29, 44]
	mov	w2, 0
	mov	w1, -109
	ldr	x0, [x29, 24]
	bl	micron_intel_set_feature
	mov	w1, w0
	ldr	w0, [x29, 44]
	orr	w0, w0, w1
	str	w0, [x29, 44]
	b	.L332
.L334:
	adrp	x0, micron_p1
	add	x0, x0, :lo12:micron_p1
	ldr	x1, [x0]
	ldrb	w0, [x29, 43]
	add	x0, x1, x0
	ldrb	w0, [x0]
	mov	w2, w0
	mov	w1, -119
	ldr	x0, [x29, 24]
	bl	micron_intel_set_feature
	str	w0, [x29, 44]
	b	.L332
.L338:
	nop
	b	.L332
.L339:
	nop
.L332:
	ldr	x0, [x29, 24]
	ldr	w0, [x0, 184]
	cmp	w0, 255
	bne	.L336
	ldr	x0, [x29, 24]
	str	wzr, [x0, 184]
.L336:
	ldr	x0, [x29, 24]
	bl	nand_disable_chip
	ldr	w0, [x29, 44]
	ldp	x29, x30, [sp], 48
	ret
	.size	micron_set_readretry, .-micron_set_readretry
	.section	.rodata
	.align	3
.LC70:
	.string	"micron retry!\n"
	.align	3
.LC71:
	.string	"micron ReadRetry ok! ch =%d, chip = %d  block = %d, page = %d, RetryCount = %d  \n"
	.text
	.align	2
	.global	micron_read_page_end
	.type	micron_read_page_end, %function
micron_read_page_end:
	stp	x29, x30, [sp, -48]!
	add	x29, sp, 0
	str	x0, [x29, 24]
	str	wzr, [x29, 44]
	str	wzr, [x29, 40]
	adrp	x0, g_nsi
	add	x0, x0, :lo12:g_nsi
	ldr	x2, [x0]
	ldr	x0, [x29, 24]
	ldr	w0, [x0]
	mov	w1, w0
	mov	x0, x2
	bl	nci_get_from_nsi
	str	x0, [x29, 32]
	ldr	x0, [x29, 24]
	bl	generic_read_page_end_not_retry
	str	w0, [x29, 40]
	ldr	w0, [x29, 40]
	cmn	w0, #2
	bne	.L341
	adrp	x0, .LC70
	add	x0, x0, :lo12:.LC70
	bl	nand_print_dbg
	str	wzr, [x29, 44]
	b	.L342
.L347:
	ldr	w1, [x29, 44]
	ldr	x0, [x29, 32]
	str	w1, [x0, 184]
	ldr	x0, [x29, 32]
	bl	micron_set_readretry
	str	w0, [x29, 40]
	ldr	w0, [x29, 40]
	cmp	w0, 0
	bne	.L349
	ldr	x0, [x29, 24]
	bl	generic_read_page_start
	str	w0, [x29, 40]
	ldr	x0, [x29, 24]
	bl	generic_read_page_end_not_retry
	mov	w1, w0
	ldr	w0, [x29, 40]
	orr	w0, w0, w1
	str	w0, [x29, 40]
	ldr	w0, [x29, 40]
	cmp	w0, 10
	beq	.L345
	ldr	w0, [x29, 40]
	cmp	w0, 0
	bne	.L344
.L345:
	mov	w0, 10
	str	w0, [x29, 40]
	ldr	x0, [x29, 32]
	ldr	x0, [x0, 272]
	ldr	w1, [x0, 12]
	ldr	x0, [x29, 32]
	ldr	w2, [x0, 28]
	ldr	x0, [x29, 24]
	ldr	w3, [x0, 4]
	ldr	x0, [x29, 24]
	ldr	w4, [x0, 8]
	ldr	x0, [x29, 32]
	ldr	w5, [x0, 184]
	adrp	x0, .LC71
	add	x0, x0, :lo12:.LC71
	bl	nand_print_dbg
	b	.L346
.L349:
	nop
.L344:
	ldr	w0, [x29, 44]
	add	w0, w0, 1
	str	w0, [x29, 44]
.L342:
	adrp	x0, micron_read_retry_cycle
	add	x0, x0, :lo12:micron_read_retry_cycle
	ldrb	w0, [x0]
	mov	w1, w0
	ldr	w0, [x29, 44]
	cmp	w1, w0
	bgt	.L347
.L346:
	ldr	x0, [x29, 32]
	mov	w1, 255
	str	w1, [x0, 184]
	ldr	x0, [x29, 32]
	bl	micron_set_readretry
.L341:
	ldr	w0, [x29, 40]
	ldp	x29, x30, [sp], 48
	ret
	.size	micron_read_page_end, .-micron_read_page_end
	.section	.rodata
	.align	3
.LC72:
	.string	"micron read retry cannot support!\n"
	.align	3
.LC73:
	.string	"micron read retry conut: %d !\n"
	.text
	.align	2
	.global	micron_readretry_init
	.type	micron_readretry_init, %function
micron_readretry_init:
	stp	x29, x30, [sp, -32]!
	add	x29, sp, 0
	str	x0, [x29, 24]
	ldr	x0, [x29, 24]
	str	wzr, [x0, 184]
	adrp	x0, micron_read_retry_mode
	add	x0, x0, :lo12:micron_read_retry_mode
	strb	wzr, [x0]
	adrp	x0, micron_p1
	add	x0, x0, :lo12:micron_p1
	adrp	x1, micron_p1_2
	add	x1, x1, :lo12:micron_p1_2
	str	x1, [x0]
	ldr	x0, [x29, 24]
	ldr	x0, [x0, 280]
	ldr	w0, [x0, 52]
	lsr	w0, w0, 16
	uxtb	w1, w0
	adrp	x0, micron_read_retry_mode
	add	x0, x0, :lo12:micron_read_retry_mode
	strb	w1, [x0]
	ldr	x0, [x29, 24]
	ldr	x0, [x0, 280]
	ldr	w0, [x0, 52]
	lsr	w0, w0, 8
	uxtb	w1, w0
	adrp	x0, micron_read_retry_cycle
	add	x0, x0, :lo12:micron_read_retry_cycle
	strb	w1, [x0]
	mov	w2, 0
	mov	w1, -119
	ldr	x0, [x29, 24]
	bl	micron_intel_set_feature
	adrp	x0, micron_read_retry_mode
	add	x0, x0, :lo12:micron_read_retry_mode
	ldrb	w0, [x0]
	cmp	w0, 65
	bne	.L351
	adrp	x0, micron_read_retry_cycle
	add	x0, x0, :lo12:micron_read_retry_cycle
	mov	w1, 12
	strb	w1, [x0]
	adrp	x0, micron_p1
	add	x0, x0, :lo12:micron_p1
	adrp	x1, micron_p1_2
	add	x1, x1, :lo12:micron_p1_2
	str	x1, [x0]
	b	.L352
.L351:
	adrp	x0, micron_read_retry_mode
	add	x0, x0, :lo12:micron_read_retry_mode
	ldrb	w0, [x0]
	cmp	w0, 64
	bne	.L353
	adrp	x0, micron_p1
	add	x0, x0, :lo12:micron_p1
	adrp	x1, micron_p1_1
	add	x1, x1, :lo12:micron_p1_1
	str	x1, [x0]
	adrp	x0, micron_read_retry_cycle
	add	x0, x0, :lo12:micron_read_retry_cycle
	mov	w1, 10
	strb	w1, [x0]
	b	.L352
.L353:
	adrp	x0, micron_read_retry_mode
	add	x0, x0, :lo12:micron_read_retry_mode
	ldrb	w0, [x0]
	cmp	w0, 80
	bne	.L354
	adrp	x0, micron_p1
	add	x0, x0, :lo12:micron_p1
	adrp	x1, micron_p1_3
	add	x1, x1, :lo12:micron_p1_3
	str	x1, [x0]
	adrp	x0, micron_read_retry_cycle
	add	x0, x0, :lo12:micron_read_retry_cycle
	mov	w1, 7
	strb	w1, [x0]
	mov	w2, 0
	mov	w1, -109
	ldr	x0, [x29, 24]
	bl	micron_intel_set_feature
	b	.L352
.L354:
	adrp	x0, micron_read_retry_mode
	add	x0, x0, :lo12:micron_read_retry_mode
	ldrb	w0, [x0]
	cmp	w0, 66
	bne	.L355
	adrp	x0, micron_p1
	add	x0, x0, :lo12:micron_p1
	adrp	x1, micron_p1_4
	add	x1, x1, :lo12:micron_p1_4
	str	x1, [x0]
	adrp	x0, micron_read_retry_cycle
	add	x0, x0, :lo12:micron_read_retry_cycle
	mov	w1, 18
	strb	w1, [x0]
	b	.L352
.L355:
	adrp	x0, .LC72
	add	x0, x0, :lo12:.LC72
	bl	nand_print
	mov	w0, -71
	b	.L356
.L352:
	adrp	x0, micron_read_retry_cycle
	add	x0, x0, :lo12:micron_read_retry_cycle
	ldrb	w0, [x0]
	mov	w1, w0
	adrp	x0, .LC73
	add	x0, x0, :lo12:.LC73
	bl	nand_print_dbg
	mov	w0, 0
.L356:
	ldp	x29, x30, [sp], 32
	ret
	.size	micron_readretry_init, .-micron_readretry_init
	.align	2
	.global	micron_readretry_exit
	.type	micron_readretry_exit, %function
micron_readretry_exit:
	sub	sp, sp, #16
	str	x0, [sp, 8]
	adrp	x0, micron_read_retry_mode
	add	x0, x0, :lo12:micron_read_retry_mode
	strb	wzr, [x0]
	adrp	x0, micron_read_retry_cycle
	add	x0, x0, :lo12:micron_read_retry_cycle
	strb	wzr, [x0]
	mov	w0, 0
	add	sp, sp, 16
	ret
	.size	micron_readretry_exit, .-micron_readretry_exit
	.section	.rodata
	.align	3
.LC74:
	.string	" micron_special_init micron_read_retry_mode:%d micron_read_retry_cycle:%d \n"
	.align	3
.LC75:
	.string	" micron_special_init error micron_read_retry_mode:%d micron_read_retry_cycle:%d \n"
	.text
	.align	2
	.global	micron_special_init
	.type	micron_special_init, %function
micron_special_init:
	stp	x29, x30, [sp, -32]!
	add	x29, sp, 0
	str	wzr, [x29, 28]
	adrp	x0, g_nsi
	add	x0, x0, :lo12:g_nsi
	ldr	x0, [x0]
	ldr	x0, [x0, 8]
	str	x0, [x29, 16]
	b	.L360
.L361:
	ldr	x0, [x29, 16]
	bl	micron_readretry_init
	mov	w1, w0
	ldr	w0, [x29, 28]
	orr	w0, w0, w1
	str	w0, [x29, 28]
	ldr	x0, [x29, 16]
	ldr	x0, [x0]
	str	x0, [x29, 16]
.L360:
	ldr	x0, [x29, 16]
	cmp	x0, 0
	bne	.L361
	ldr	w0, [x29, 28]
	cmp	w0, 0
	bne	.L362
	adrp	x0, df_read_page_end
	add	x0, x0, :lo12:df_read_page_end
	adrp	x1, micron_read_page_end
	add	x1, x1, :lo12:micron_read_page_end
	str	x1, [x0]
	adrp	x0, micron_read_retry_mode
	add	x0, x0, :lo12:micron_read_retry_mode
	ldrb	w0, [x0]
	mov	w1, w0
	adrp	x0, micron_read_retry_cycle
	add	x0, x0, :lo12:micron_read_retry_cycle
	ldrb	w0, [x0]
	mov	w2, w0
	adrp	x0, .LC74
	add	x0, x0, :lo12:.LC74
	bl	nand_print_dbg
	b	.L363
.L362:
	adrp	x0, micron_read_retry_mode
	add	x0, x0, :lo12:micron_read_retry_mode
	ldrb	w0, [x0]
	mov	w1, w0
	adrp	x0, micron_read_retry_cycle
	add	x0, x0, :lo12:micron_read_retry_cycle
	ldrb	w0, [x0]
	mov	w2, w0
	adrp	x0, .LC75
	add	x0, x0, :lo12:.LC75
	bl	nand_print
.L363:
	ldr	w0, [x29, 28]
	ldp	x29, x30, [sp], 32
	ret
	.size	micron_special_init, .-micron_special_init
	.section	.rodata
	.align	3
.LC76:
	.string	" micron_special_exit \n"
	.text
	.align	2
	.global	micron_special_exit
	.type	micron_special_exit, %function
micron_special_exit:
	stp	x29, x30, [sp, -32]!
	add	x29, sp, 0
	adrp	x0, g_nsi
	add	x0, x0, :lo12:g_nsi
	ldr	x0, [x0]
	ldr	x0, [x0, 8]
	str	x0, [x29, 24]
	b	.L366
.L367:
	ldr	x0, [x29, 24]
	bl	micron_readretry_exit
	ldr	x0, [x29, 24]
	ldr	x0, [x0]
	str	x0, [x29, 24]
.L366:
	ldr	x0, [x29, 24]
	cmp	x0, 0
	bne	.L367
	adrp	x0, .LC76
	add	x0, x0, :lo12:.LC76
	bl	nand_print_dbg
	mov	w0, 0
	ldp	x29, x30, [sp], 32
	ret
	.size	micron_special_exit, .-micron_special_exit
	.global	toshiba_read_retry_mode
	.bss
	.type	toshiba_read_retry_mode, %object
	.size	toshiba_read_retry_mode, 1
toshiba_read_retry_mode:
	.zero	1
	.global	toshiba_read_retry_cycle
	.type	toshiba_read_retry_cycle, %object
	.size	toshiba_read_retry_cycle, 1
toshiba_read_retry_cycle:
	.zero	1
	.global	toshiba_read_retry_reg_cnt
	.type	toshiba_read_retry_reg_cnt, %object
	.size	toshiba_read_retry_reg_cnt, 1
toshiba_read_retry_reg_cnt:
	.zero	1
	.global	toshiba_read_retry_start_cmd
	.data
	.align	3
	.type	toshiba_read_retry_start_cmd, %object
	.size	toshiba_read_retry_start_cmd, 2
toshiba_read_retry_start_cmd:
	.byte	92
	.byte	-59
	.global	toshiba_read_retry_end_cmd
	.align	3
	.type	toshiba_read_retry_end_cmd, %object
	.size	toshiba_read_retry_end_cmd, 2
toshiba_read_retry_end_cmd:
	.byte	38
	.byte	93
	.global	toshiba_read_retry_cmd_1
	.align	3
	.type	toshiba_read_retry_cmd_1, %object
	.size	toshiba_read_retry_cmd_1, 4
toshiba_read_retry_cmd_1:
	.byte	85
	.byte	85
	.byte	85
	.byte	85
	.global	toshiba_read_retry_addr_1
	.align	3
	.type	toshiba_read_retry_addr_1, %object
	.size	toshiba_read_retry_addr_1, 4
toshiba_read_retry_addr_1:
	.byte	4
	.byte	5
	.byte	6
	.byte	7
	.global	toshiba_p1_1
	.align	3
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
	.global	toshiba_read_retry_cmd_2
	.align	3
	.type	toshiba_read_retry_cmd_2, %object
	.size	toshiba_read_retry_cmd_2, 5
toshiba_read_retry_cmd_2:
	.byte	85
	.byte	85
	.byte	85
	.byte	85
	.byte	85
	.global	toshiba_read_retry_addr_2
	.align	3
	.type	toshiba_read_retry_addr_2, %object
	.size	toshiba_read_retry_addr_2, 5
toshiba_read_retry_addr_2:
	.byte	4
	.byte	5
	.byte	6
	.byte	7
	.byte	13
	.global	toshiba_p1_2
	.align	3
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
	.global	toshiba_p1_3
	.align	3
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
	.global	toshiba_read_retry_last_cmd
	.align	3
	.type	toshiba_read_retry_last_cmd, %object
	.size	toshiba_read_retry_last_cmd, 1
toshiba_read_retry_last_cmd:
	.byte	-77
	.global	toshiba_read_retry_exit_cmd
	.align	3
	.type	toshiba_read_retry_exit_cmd, %object
	.size	toshiba_read_retry_exit_cmd, 1
toshiba_read_retry_exit_cmd:
	.byte	-1
	.local	toshiba_toggle_mode_flag
	.comm	toshiba_toggle_mode_flag,4,4
	.local	toshiba_retry_flag
	.comm	toshiba_retry_flag,4,4
	.local	toshiba_sclk0_bak
	.comm	toshiba_sclk0_bak,4,4
	.local	toshiba_sclk1_bak
	.comm	toshiba_sclk1_bak,4,4
	.text
	.align	2
	.global	toshiba_read_retry_clock_save
	.type	toshiba_read_retry_clock_save, %function
toshiba_read_retry_clock_save:
	stp	x29, x30, [sp, -32]!
	add	x29, sp, 0
	str	x0, [x29, 24]
	ldr	x0, [x29, 24]
	ldr	x0, [x0, 272]
	ldr	w1, [x0, 12]
	adrp	x0, toshiba_sclk1_bak
	add	x3, x0, :lo12:toshiba_sclk1_bak
	adrp	x0, toshiba_sclk0_bak
	add	x2, x0, :lo12:toshiba_sclk0_bak
	adrp	x0, aw_ndfc
	add	x0, x0, :lo12:aw_ndfc
	bl	nand_get_clk
	ldr	x0, [x29, 24]
	ldr	x0, [x0, 272]
	ldr	w1, [x0, 12]
	adrp	x0, aw_ndfc
	add	x0, x0, :lo12:aw_ndfc
	mov	w3, 20
	mov	w2, 10
	bl	nand_set_clk
	mov	w0, 0
	ldp	x29, x30, [sp], 32
	ret
	.size	toshiba_read_retry_clock_save, .-toshiba_read_retry_clock_save
	.align	2
	.global	toshiba_read_retry_clock_recover
	.type	toshiba_read_retry_clock_recover, %function
toshiba_read_retry_clock_recover:
	stp	x29, x30, [sp, -32]!
	add	x29, sp, 0
	str	x0, [x29, 24]
	ldr	x0, [x29, 24]
	ldr	x0, [x0, 272]
	ldr	w1, [x0, 12]
	adrp	x0, toshiba_sclk0_bak
	add	x0, x0, :lo12:toshiba_sclk0_bak
	ldr	w2, [x0]
	adrp	x0, toshiba_sclk1_bak
	add	x0, x0, :lo12:toshiba_sclk1_bak
	ldr	w3, [x0]
	adrp	x0, aw_ndfc
	add	x0, x0, :lo12:aw_ndfc
	bl	nand_set_clk
	mov	w0, 0
	ldp	x29, x30, [sp], 32
	ret
	.size	toshiba_read_retry_clock_recover, .-toshiba_read_retry_clock_recover
	.section	.rodata
	.align	3
.LC77:
	.string	"toshiba rr value 3 %x %x %x %x!\n"
	.text
	.align	2
	.global	toshiba_vender_set_param
	.type	toshiba_vender_set_param, %function
toshiba_vender_set_param:
	stp	x29, x30, [sp, -64]!
	add	x29, sp, 0
	str	x0, [x29, 40]
	str	x1, [x29, 32]
	str	x2, [x29, 24]
	str	w3, [x29, 20]
	str	wzr, [x29, 60]
	adrp	x0, toshiba_read_retry_mode
	add	x0, x0, :lo12:toshiba_read_retry_mode
	ldrb	w0, [x0]
	cmp	w0, 18
	bne	.L374
	adrp	x0, toshiba_read_retry_cmd_2
	add	x0, x0, :lo12:toshiba_read_retry_cmd_2
	ldr	w6, [x29, 20]
	mov	w5, 1
	ldr	x4, [x29, 32]
	ldr	x3, [x29, 24]
	mov	w2, 0
	mov	x1, x0
	ldr	x0, [x29, 40]
	bl	set_cmd_with_nand_bus
	mov	w1, w0
	ldr	w0, [x29, 60]
	orr	w0, w0, w1
	str	w0, [x29, 60]
	ldr	x0, [x29, 32]
	ldrb	w0, [x0]
	cmp	w0, 0
	bne	.L374
	ldr	x0, [x29, 32]
	add	x0, x0, 1
	ldrb	w0, [x0]
	cmp	w0, 0
	bne	.L374
	ldr	x0, [x29, 32]
	add	x0, x0, 2
	ldrb	w0, [x0]
	cmp	w0, 0
	bne	.L374
	ldr	x0, [x29, 32]
	add	x0, x0, 3
	ldrb	w0, [x0]
	cmp	w0, 0
	bne	.L374
	adrp	x0, toshiba_retry_flag
	add	x0, x0, :lo12:toshiba_retry_flag
	ldr	w0, [x0]
	cmp	w0, 0
	bne	.L374
	adrp	x0, toshiba_read_retry_exit_cmd
	add	x0, x0, :lo12:toshiba_read_retry_exit_cmd
	ldrb	w0, [x0]
	mov	w2, 1
	mov	w1, w0
	ldr	x0, [x29, 40]
	bl	set_one_cmd
	mov	w1, w0
	ldr	w0, [x29, 60]
	orr	w0, w0, w1
	str	w0, [x29, 60]
	ldr	w0, [x29, 60]
	b	.L375
.L374:
	adrp	x0, toshiba_read_retry_mode
	add	x0, x0, :lo12:toshiba_read_retry_mode
	ldrb	w0, [x0]
	cmp	w0, 17
	bne	.L376
	adrp	x0, toshiba_read_retry_cmd_2
	add	x0, x0, :lo12:toshiba_read_retry_cmd_2
	ldr	w6, [x29, 20]
	mov	w5, 1
	ldr	x4, [x29, 32]
	ldr	x3, [x29, 24]
	mov	w2, 0
	mov	x1, x0
	ldr	x0, [x29, 40]
	bl	set_cmd_with_nand_bus
	mov	w1, w0
	ldr	w0, [x29, 60]
	orr	w0, w0, w1
	str	w0, [x29, 60]
	ldr	x0, [x29, 32]
	ldrb	w0, [x0]
	cmp	w0, 0
	bne	.L377
	ldr	x0, [x29, 32]
	add	x0, x0, 1
	ldrb	w0, [x0]
	cmp	w0, 0
	bne	.L377
	ldr	x0, [x29, 32]
	add	x0, x0, 2
	ldrb	w0, [x0]
	cmp	w0, 0
	bne	.L377
	ldr	x0, [x29, 32]
	add	x0, x0, 3
	ldrb	w0, [x0]
	cmp	w0, 0
	bne	.L377
	adrp	x0, toshiba_read_retry_exit_cmd
	add	x0, x0, :lo12:toshiba_read_retry_exit_cmd
	ldrb	w0, [x0]
	mov	w2, 1
	mov	w1, w0
	ldr	x0, [x29, 40]
	bl	set_one_cmd
	mov	w1, w0
	ldr	w0, [x29, 60]
	orr	w0, w0, w1
	str	w0, [x29, 60]
	ldr	w0, [x29, 60]
	b	.L375
.L377:
	ldr	x0, [x29, 32]
	ldrb	w0, [x0]
	cmp	w0, 2
	bne	.L376
	ldr	x0, [x29, 32]
	add	x0, x0, 1
	ldrb	w0, [x0]
	cmp	w0, 0
	bne	.L376
	ldr	x0, [x29, 32]
	add	x0, x0, 2
	ldrb	w0, [x0]
	cmp	w0, 126
	bne	.L376
	ldr	x0, [x29, 32]
	add	x0, x0, 3
	ldrb	w0, [x0]
	cmp	w0, 124
	bne	.L376
	adrp	x0, toshiba_read_retry_last_cmd
	add	x0, x0, :lo12:toshiba_read_retry_last_cmd
	ldrb	w0, [x0]
	mov	w2, 0
	mov	w1, w0
	ldr	x0, [x29, 40]
	bl	set_one_cmd
	mov	w1, w0
	ldr	w0, [x29, 60]
	orr	w0, w0, w1
	str	w0, [x29, 60]
.L376:
	adrp	x0, toshiba_read_retry_mode
	add	x0, x0, :lo12:toshiba_read_retry_mode
	ldrb	w0, [x0]
	cmp	w0, 16
	bne	.L378
	ldr	x0, [x29, 32]
	ldrb	w0, [x0]
	cmp	w0, 255
	bne	.L379
	ldr	x0, [x29, 32]
	add	x0, x0, 1
	ldrb	w0, [x0]
	cmp	w0, 255
	bne	.L379
	ldr	x0, [x29, 32]
	add	x0, x0, 2
	ldrb	w0, [x0]
	cmp	w0, 255
	bne	.L379
	ldr	x0, [x29, 32]
	add	x0, x0, 3
	ldrb	w0, [x0]
	cmp	w0, 255
	bne	.L379
	adrp	x0, toshiba_read_retry_exit_cmd
	add	x0, x0, :lo12:toshiba_read_retry_exit_cmd
	ldrb	w0, [x0]
	mov	w2, 1
	mov	w1, w0
	ldr	x0, [x29, 40]
	bl	set_one_cmd
	mov	w1, w0
	ldr	w0, [x29, 60]
	orr	w0, w0, w1
	str	w0, [x29, 60]
	ldr	w0, [x29, 60]
	b	.L375
.L379:
	adrp	x0, toshiba_read_retry_cmd_1
	add	x0, x0, :lo12:toshiba_read_retry_cmd_1
	ldr	w6, [x29, 20]
	mov	w5, 1
	ldr	x4, [x29, 32]
	ldr	x3, [x29, 24]
	mov	w2, 0
	mov	x1, x0
	ldr	x0, [x29, 40]
	bl	set_cmd_with_nand_bus
	mov	w1, w0
	ldr	w0, [x29, 60]
	orr	w0, w0, w1
	str	w0, [x29, 60]
.L378:
	adrp	x0, toshiba_read_retry_end_cmd
	add	x0, x0, :lo12:toshiba_read_retry_end_cmd
	ldrb	w0, [x0]
	mov	w2, 0
	mov	w1, w0
	ldr	x0, [x29, 40]
	bl	set_one_cmd
	mov	w1, w0
	ldr	w0, [x29, 60]
	orr	w0, w0, w1
	str	w0, [x29, 60]
	adrp	x0, toshiba_read_retry_end_cmd
	add	x0, x0, :lo12:toshiba_read_retry_end_cmd
	ldrb	w0, [x0, 1]
	mov	w2, 0
	mov	w1, w0
	ldr	x0, [x29, 40]
	bl	set_one_cmd
	mov	w1, w0
	ldr	w0, [x29, 60]
	orr	w0, w0, w1
	str	w0, [x29, 60]
	ldr	x0, [x29, 32]
	ldrb	w0, [x0]
	mov	w1, w0
	ldr	x0, [x29, 32]
	add	x0, x0, 1
	ldrb	w0, [x0]
	mov	w2, w0
	ldr	x0, [x29, 32]
	add	x0, x0, 2
	ldrb	w0, [x0]
	mov	w3, w0
	ldr	x0, [x29, 32]
	add	x0, x0, 3
	ldrb	w0, [x0]
	mov	w4, w0
	adrp	x0, .LC77
	add	x0, x0, :lo12:.LC77
	bl	nand_print_dbg
	ldr	w0, [x29, 60]
.L375:
	ldp	x29, x30, [sp], 64
	ret
	.size	toshiba_vender_set_param, .-toshiba_vender_set_param
	.section	.rodata
	.align	3
.LC78:
	.string	"toshiba vender_pre_condition error!\n"
	.text
	.align	2
	.global	toshiba_vender_pre_condition
	.type	toshiba_vender_pre_condition, %function
toshiba_vender_pre_condition:
	stp	x29, x30, [sp, -48]!
	add	x29, sp, 0
	str	x0, [x29, 24]
	adrp	x0, toshiba_read_retry_start_cmd
	add	x0, x0, :lo12:toshiba_read_retry_start_cmd
	mov	w6, 2
	mov	w5, 0
	mov	x4, 0
	mov	x3, 0
	mov	w2, 0
	mov	x1, x0
	ldr	x0, [x29, 24]
	bl	set_cmd_with_nand_bus
	str	w0, [x29, 44]
	ldr	w0, [x29, 44]
	cmp	w0, 0
	beq	.L381
	adrp	x0, .LC78
	add	x0, x0, :lo12:.LC78
	bl	nand_print
	ldr	w0, [x29, 44]
	b	.L382
.L381:
	mov	w0, 0
.L382:
	ldp	x29, x30, [sp], 48
	ret
	.size	toshiba_vender_pre_condition, .-toshiba_vender_pre_condition
	.align	2
	.global	toshiba_set_readretry
	.type	toshiba_set_readretry, %function
toshiba_set_readretry:
	stp	x29, x30, [sp, -64]!
	add	x29, sp, 0
	str	x0, [x29, 24]
	adrp	x0, toshiba_read_retry_mode
	add	x0, x0, :lo12:toshiba_read_retry_mode
	ldrb	w0, [x0]
	cmp	w0, 16
	beq	.L384
	adrp	x0, toshiba_read_retry_mode
	add	x0, x0, :lo12:toshiba_read_retry_mode
	ldrb	w0, [x0]
	cmp	w0, 17
	beq	.L384
	adrp	x0, toshiba_read_retry_mode
	add	x0, x0, :lo12:toshiba_read_retry_mode
	ldrb	w0, [x0]
	cmp	w0, 18
	beq	.L384
	mov	w0, -89
	b	.L385
.L384:
	ldr	x0, [x29, 24]
	bl	toshiba_read_retry_clock_save
	ldr	x0, [x29, 24]
	bl	nand_enable_chip
	ldr	x0, [x29, 24]
	ldr	x0, [x0, 272]
	bl	ndfc_disable_randomize
	ldr	x0, [x29, 24]
	ldr	w0, [x0, 184]
	cmp	w0, 0
	bne	.L386
	ldr	x0, [x29, 24]
	bl	toshiba_vender_pre_condition
.L386:
	ldr	x0, [x29, 24]
	ldr	x0, [x0, 272]
	bl	ndfc_is_toggle_interface
	cmp	w0, 0
	beq	.L387
	ldr	x0, [x29, 24]
	ldr	x0, [x0, 272]
	bl	ndfc_set_legacy_interface
	adrp	x0, toshiba_toggle_mode_flag
	add	x0, x0, :lo12:toshiba_toggle_mode_flag
	mov	w1, 1
	str	w1, [x0]
.L387:
	adrp	x0, toshiba_read_retry_mode
	add	x0, x0, :lo12:toshiba_read_retry_mode
	ldrb	w0, [x0]
	cmp	w0, 17
	bne	.L388
	ldr	x0, [x29, 24]
	ldr	w0, [x0, 184]
	uxtw	x1, w0
	mov	x0, x1
	lsl	x0, x0, 2
	add	x0, x0, x1
	adrp	x1, toshiba_p1_2
	add	x1, x1, :lo12:toshiba_p1_2
	add	x0, x0, x1
	str	x0, [x29, 56]
	adrp	x0, toshiba_read_retry_addr_2
	add	x0, x0, :lo12:toshiba_read_retry_addr_2
	str	x0, [x29, 48]
	mov	w0, 5
	str	w0, [x29, 44]
	b	.L389
.L388:
	adrp	x0, toshiba_read_retry_mode
	add	x0, x0, :lo12:toshiba_read_retry_mode
	ldrb	w0, [x0]
	cmp	w0, 18
	bne	.L390
	ldr	x0, [x29, 24]
	ldr	w0, [x0, 184]
	uxtw	x1, w0
	mov	x0, x1
	lsl	x0, x0, 2
	add	x0, x0, x1
	adrp	x1, toshiba_p1_3
	add	x1, x1, :lo12:toshiba_p1_3
	add	x0, x0, x1
	str	x0, [x29, 56]
	adrp	x0, toshiba_read_retry_addr_2
	add	x0, x0, :lo12:toshiba_read_retry_addr_2
	str	x0, [x29, 48]
	mov	w0, 5
	str	w0, [x29, 44]
	b	.L389
.L390:
	ldr	x0, [x29, 24]
	ldr	w0, [x0, 184]
	uxtw	x0, w0
	lsl	x1, x0, 2
	adrp	x0, toshiba_p1_1
	add	x0, x0, :lo12:toshiba_p1_1
	add	x0, x1, x0
	str	x0, [x29, 56]
	adrp	x0, toshiba_read_retry_addr_1
	add	x0, x0, :lo12:toshiba_read_retry_addr_1
	str	x0, [x29, 48]
	mov	w0, 4
	str	w0, [x29, 44]
.L389:
	ldr	w3, [x29, 44]
	ldr	x2, [x29, 48]
	ldr	x1, [x29, 56]
	ldr	x0, [x29, 24]
	bl	toshiba_vender_set_param
	str	w0, [x29, 40]
	adrp	x0, toshiba_toggle_mode_flag
	add	x0, x0, :lo12:toshiba_toggle_mode_flag
	ldr	w0, [x0]
	cmp	w0, 1
	bne	.L391
	ldr	x0, [x29, 24]
	ldr	x0, [x0, 272]
	bl	ndfc_set_toggle_interface
.L391:
	ldr	x0, [x29, 24]
	bl	nand_disable_chip
	ldr	x0, [x29, 24]
	bl	toshiba_read_retry_clock_recover
	ldr	w0, [x29, 40]
.L385:
	ldp	x29, x30, [sp], 64
	ret
	.size	toshiba_set_readretry, .-toshiba_set_readretry
	.section	.rodata
	.align	3
.LC79:
	.string	"toshiba retry!\n"
	.align	3
.LC80:
	.string	"toshiba ReadRetry ok! ch =%d, chip = %d  block = %d, page = %d, RetryCount = %d  \n"
	.text
	.align	2
	.global	toshiba_read_page_end
	.type	toshiba_read_page_end, %function
toshiba_read_page_end:
	stp	x29, x30, [sp, -48]!
	add	x29, sp, 0
	str	x0, [x29, 24]
	str	wzr, [x29, 44]
	str	wzr, [x29, 40]
	adrp	x0, g_nsi
	add	x0, x0, :lo12:g_nsi
	ldr	x2, [x0]
	ldr	x0, [x29, 24]
	ldr	w0, [x0]
	mov	w1, w0
	mov	x0, x2
	bl	nci_get_from_nsi
	str	x0, [x29, 32]
	ldr	x0, [x29, 24]
	bl	generic_read_page_end_not_retry
	str	w0, [x29, 40]
	ldr	w0, [x29, 40]
	cmn	w0, #2
	bne	.L393
	adrp	x0, .LC79
	add	x0, x0, :lo12:.LC79
	bl	nand_print_dbg
	adrp	x0, toshiba_retry_flag
	add	x0, x0, :lo12:toshiba_retry_flag
	mov	w1, 1
	str	w1, [x0]
	str	wzr, [x29, 44]
	b	.L394
.L399:
	ldr	w1, [x29, 44]
	ldr	x0, [x29, 32]
	str	w1, [x0, 184]
	ldr	x0, [x29, 32]
	bl	toshiba_set_readretry
	str	w0, [x29, 40]
	ldr	w0, [x29, 40]
	cmp	w0, 0
	bne	.L401
	ldr	x0, [x29, 24]
	bl	generic_read_page_start
	str	w0, [x29, 40]
	ldr	x0, [x29, 24]
	bl	generic_read_page_end_not_retry
	mov	w1, w0
	ldr	w0, [x29, 40]
	orr	w0, w0, w1
	str	w0, [x29, 40]
	ldr	w0, [x29, 40]
	cmp	w0, 10
	beq	.L397
	ldr	w0, [x29, 40]
	cmp	w0, 0
	bne	.L396
.L397:
	mov	w0, 10
	str	w0, [x29, 40]
	ldr	x0, [x29, 32]
	ldr	x0, [x0, 272]
	ldr	w1, [x0, 12]
	ldr	x0, [x29, 32]
	ldr	w2, [x0, 28]
	ldr	x0, [x29, 24]
	ldr	w3, [x0, 4]
	ldr	x0, [x29, 24]
	ldr	w4, [x0, 8]
	ldr	x0, [x29, 32]
	ldr	w5, [x0, 184]
	adrp	x0, .LC80
	add	x0, x0, :lo12:.LC80
	bl	nand_print_dbg
	b	.L398
.L401:
	nop
.L396:
	ldr	w0, [x29, 44]
	add	w0, w0, 1
	str	w0, [x29, 44]
.L394:
	adrp	x0, toshiba_read_retry_cycle
	add	x0, x0, :lo12:toshiba_read_retry_cycle
	ldrb	w0, [x0]
	mov	w1, w0
	ldr	w0, [x29, 44]
	cmp	w1, w0
	bgt	.L399
.L398:
	adrp	x0, toshiba_retry_flag
	add	x0, x0, :lo12:toshiba_retry_flag
	str	wzr, [x0]
	ldr	x0, [x29, 32]
	str	wzr, [x0, 184]
	ldr	x0, [x29, 32]
	bl	toshiba_exit_readretry
.L393:
	ldr	w0, [x29, 40]
	ldp	x29, x30, [sp], 48
	ret
	.size	toshiba_read_page_end, .-toshiba_read_page_end
	.section	.rodata
	.align	3
.LC81:
	.string	"toshiba read retry conut: %d !\n"
	.text
	.align	2
	.global	toshiba_readretry_init
	.type	toshiba_readretry_init, %function
toshiba_readretry_init:
	stp	x29, x30, [sp, -32]!
	add	x29, sp, 0
	str	x0, [x29, 24]
	ldr	x0, [x29, 24]
	str	wzr, [x0, 184]
	ldr	x0, [x29, 24]
	ldr	x0, [x0, 280]
	ldr	w0, [x0, 52]
	lsr	w0, w0, 16
	uxtb	w1, w0
	adrp	x0, toshiba_read_retry_mode
	add	x0, x0, :lo12:toshiba_read_retry_mode
	strb	w1, [x0]
	ldr	x0, [x29, 24]
	ldr	x0, [x0, 280]
	ldr	w0, [x0, 52]
	lsr	w0, w0, 8
	uxtb	w1, w0
	adrp	x0, toshiba_read_retry_cycle
	add	x0, x0, :lo12:toshiba_read_retry_cycle
	strb	w1, [x0]
	ldr	x0, [x29, 24]
	ldr	x0, [x0, 280]
	ldr	w0, [x0, 52]
	uxtb	w1, w0
	adrp	x0, toshiba_read_retry_reg_cnt
	add	x0, x0, :lo12:toshiba_read_retry_reg_cnt
	strb	w1, [x0]
	adrp	x0, toshiba_read_retry_cycle
	add	x0, x0, :lo12:toshiba_read_retry_cycle
	ldrb	w0, [x0]
	mov	w1, w0
	adrp	x0, .LC81
	add	x0, x0, :lo12:.LC81
	bl	nand_print_dbg
	mov	w0, 0
	ldp	x29, x30, [sp], 32
	ret
	.size	toshiba_readretry_init, .-toshiba_readretry_init
	.align	2
	.global	toshiba_readretry_exit
	.type	toshiba_readretry_exit, %function
toshiba_readretry_exit:
	sub	sp, sp, #16
	str	x0, [sp, 8]
	mov	w0, 0
	add	sp, sp, 16
	ret
	.size	toshiba_readretry_exit, .-toshiba_readretry_exit
	.align	2
	.global	toshiba_exit_readretry
	.type	toshiba_exit_readretry, %function
toshiba_exit_readretry:
	stp	x29, x30, [sp, -64]!
	add	x29, sp, 0
	str	x0, [x29, 24]
	adrp	x0, toshiba_read_retry_mode
	add	x0, x0, :lo12:toshiba_read_retry_mode
	ldrb	w0, [x0]
	cmp	w0, 16
	beq	.L407
	adrp	x0, toshiba_read_retry_mode
	add	x0, x0, :lo12:toshiba_read_retry_mode
	ldrb	w0, [x0]
	cmp	w0, 17
	beq	.L407
	adrp	x0, toshiba_read_retry_mode
	add	x0, x0, :lo12:toshiba_read_retry_mode
	ldrb	w0, [x0]
	cmp	w0, 18
	beq	.L407
	mov	w0, -88
	b	.L408
.L407:
	ldr	x0, [x29, 24]
	bl	nand_enable_chip
	ldr	x0, [x29, 24]
	ldr	x0, [x0, 272]
	bl	ndfc_disable_randomize
	ldr	x0, [x29, 24]
	ldr	x0, [x0, 272]
	bl	ndfc_is_toggle_interface
	cmp	w0, 0
	beq	.L409
	ldr	x0, [x29, 24]
	ldr	x0, [x0, 272]
	bl	ndfc_set_legacy_interface
	adrp	x0, toshiba_toggle_mode_flag
	add	x0, x0, :lo12:toshiba_toggle_mode_flag
	mov	w1, 1
	str	w1, [x0]
.L409:
	adrp	x0, toshiba_read_retry_mode
	add	x0, x0, :lo12:toshiba_read_retry_mode
	ldrb	w0, [x0]
	cmp	w0, 17
	bne	.L410
	adrp	x0, toshiba_p1_2+35
	add	x0, x0, :lo12:toshiba_p1_2+35
	str	x0, [x29, 56]
	adrp	x0, toshiba_read_retry_addr_2
	add	x0, x0, :lo12:toshiba_read_retry_addr_2
	str	x0, [x29, 48]
	mov	w0, 5
	str	w0, [x29, 44]
	b	.L411
.L410:
	adrp	x0, toshiba_read_retry_mode
	add	x0, x0, :lo12:toshiba_read_retry_mode
	ldrb	w0, [x0]
	cmp	w0, 18
	bne	.L412
	adrp	x0, toshiba_p1_3+50
	add	x0, x0, :lo12:toshiba_p1_3+50
	str	x0, [x29, 56]
	adrp	x0, toshiba_read_retry_addr_2
	add	x0, x0, :lo12:toshiba_read_retry_addr_2
	str	x0, [x29, 48]
	mov	w0, 5
	str	w0, [x29, 44]
	b	.L411
.L412:
	adrp	x0, toshiba_p1_1+24
	add	x0, x0, :lo12:toshiba_p1_1+24
	str	x0, [x29, 56]
	adrp	x0, toshiba_read_retry_addr_1
	add	x0, x0, :lo12:toshiba_read_retry_addr_1
	str	x0, [x29, 48]
	str	wzr, [x29, 44]
.L411:
	ldr	w3, [x29, 44]
	ldr	x2, [x29, 48]
	ldr	x1, [x29, 56]
	ldr	x0, [x29, 24]
	bl	toshiba_vender_set_param
	str	w0, [x29, 40]
	adrp	x0, toshiba_toggle_mode_flag
	add	x0, x0, :lo12:toshiba_toggle_mode_flag
	ldr	w0, [x0]
	cmp	w0, 1
	bne	.L413
	ldr	x0, [x29, 24]
	ldr	x0, [x0, 272]
	bl	ndfc_set_toggle_interface
.L413:
	ldr	x0, [x29, 24]
	bl	nand_disable_chip
	ldr	w0, [x29, 40]
.L408:
	ldp	x29, x30, [sp], 64
	ret
	.size	toshiba_exit_readretry, .-toshiba_exit_readretry
	.section	.rodata
	.align	3
.LC82:
	.string	" toshiba_special_init toshiba_read_retry_mode :%d toshiba_read_retry_cycle :%d toshiba_read_retry_reg_cnt :%d \n"
	.align	3
.LC83:
	.string	" toshiba_special_init error toshiba_read_retry_mode :%d toshiba_read_retry_cycle :%d toshiba_read_retry_reg_cnt :%d \n"
	.text
	.align	2
	.global	toshiba_special_init
	.type	toshiba_special_init, %function
toshiba_special_init:
	stp	x29, x30, [sp, -32]!
	add	x29, sp, 0
	str	wzr, [x29, 28]
	adrp	x0, g_nsi
	add	x0, x0, :lo12:g_nsi
	ldr	x0, [x0]
	ldr	x0, [x0, 8]
	str	x0, [x29, 16]
	b	.L415
.L416:
	ldr	x0, [x29, 16]
	bl	toshiba_readretry_init
	mov	w1, w0
	ldr	w0, [x29, 28]
	orr	w0, w0, w1
	str	w0, [x29, 28]
	ldr	x0, [x29, 16]
	ldr	x0, [x0]
	str	x0, [x29, 16]
.L415:
	ldr	x0, [x29, 16]
	cmp	x0, 0
	bne	.L416
	ldr	w0, [x29, 28]
	cmp	w0, 0
	bne	.L417
	adrp	x0, df_read_page_end
	add	x0, x0, :lo12:df_read_page_end
	adrp	x1, toshiba_read_page_end
	add	x1, x1, :lo12:toshiba_read_page_end
	str	x1, [x0]
	adrp	x0, toshiba_read_retry_mode
	add	x0, x0, :lo12:toshiba_read_retry_mode
	ldrb	w0, [x0]
	mov	w1, w0
	adrp	x0, toshiba_read_retry_cycle
	add	x0, x0, :lo12:toshiba_read_retry_cycle
	ldrb	w0, [x0]
	mov	w2, w0
	adrp	x0, toshiba_read_retry_reg_cnt
	add	x0, x0, :lo12:toshiba_read_retry_reg_cnt
	ldrb	w0, [x0]
	mov	w3, w0
	adrp	x0, .LC82
	add	x0, x0, :lo12:.LC82
	bl	nand_print_dbg
	b	.L418
.L417:
	adrp	x0, toshiba_read_retry_mode
	add	x0, x0, :lo12:toshiba_read_retry_mode
	ldrb	w0, [x0]
	mov	w1, w0
	adrp	x0, toshiba_read_retry_cycle
	add	x0, x0, :lo12:toshiba_read_retry_cycle
	ldrb	w0, [x0]
	mov	w2, w0
	adrp	x0, toshiba_read_retry_reg_cnt
	add	x0, x0, :lo12:toshiba_read_retry_reg_cnt
	ldrb	w0, [x0]
	mov	w3, w0
	adrp	x0, .LC83
	add	x0, x0, :lo12:.LC83
	bl	nand_print
.L418:
	ldr	w0, [x29, 28]
	ldp	x29, x30, [sp], 32
	ret
	.size	toshiba_special_init, .-toshiba_special_init
	.section	.rodata
	.align	3
.LC84:
	.string	" toshiba_special_exit \n"
	.text
	.align	2
	.global	toshiba_special_exit
	.type	toshiba_special_exit, %function
toshiba_special_exit:
	stp	x29, x30, [sp, -32]!
	add	x29, sp, 0
	adrp	x0, g_nsi
	add	x0, x0, :lo12:g_nsi
	ldr	x0, [x0]
	ldr	x0, [x0, 8]
	str	x0, [x29, 24]
	b	.L421
.L422:
	ldr	x0, [x29, 24]
	bl	toshiba_readretry_exit
	ldr	x0, [x29, 24]
	ldr	x0, [x0]
	str	x0, [x29, 24]
.L421:
	ldr	x0, [x29, 24]
	cmp	x0, 0
	bne	.L422
	adrp	x0, .LC84
	add	x0, x0, :lo12:.LC84
	bl	nand_print_dbg
	mov	w0, 0
	ldp	x29, x30, [sp], 32
	ret
	.size	toshiba_special_exit, .-toshiba_special_exit
	.global	hynix26nm_read_retry_mode
	.bss
	.type	hynix26nm_read_retry_mode, %object
	.size	hynix26nm_read_retry_mode, 1
hynix26nm_read_retry_mode:
	.zero	1
	.global	hynix26nm_read_retry_cycle
	.type	hynix26nm_read_retry_cycle, %object
	.size	hynix26nm_read_retry_cycle, 1
hynix26nm_read_retry_cycle:
	.zero	1
	.global	hynix26nm_read_retry_reg_num
	.type	hynix26nm_read_retry_reg_num, %object
	.size	hynix26nm_read_retry_reg_num, 1
hynix26nm_read_retry_reg_num:
	.zero	1
	.global	hynix26nm_read_retry_reg_adr
	.align	3
	.type	hynix26nm_read_retry_reg_adr, %object
	.size	hynix26nm_read_retry_reg_adr, 4
hynix26nm_read_retry_reg_adr:
	.zero	4
	.global	hynix26nm_read_retry_val
	.align	3
	.type	hynix26nm_read_retry_val, %object
	.size	hynix26nm_read_retry_val, 56
hynix26nm_read_retry_val:
	.zero	56
	.global	hynix26nm_lsb_mode_reg_adr
	.align	3
	.type	hynix26nm_lsb_mode_reg_adr, %object
	.size	hynix26nm_lsb_mode_reg_adr, 5
hynix26nm_lsb_mode_reg_adr:
	.zero	5
	.global	hynix26nm_lsb_mode_default_val
	.align	3
	.type	hynix26nm_lsb_mode_default_val, %object
	.size	hynix26nm_lsb_mode_default_val, 5
hynix26nm_lsb_mode_default_val:
	.zero	5
	.global	hynix26nm_lsb_mode_val
	.align	3
	.type	hynix26nm_lsb_mode_val, %object
	.size	hynix26nm_lsb_mode_val, 5
hynix26nm_lsb_mode_val:
	.zero	5
	.global	hynix26nm_lsb_mode_reg_num
	.type	hynix26nm_lsb_mode_reg_num, %object
	.size	hynix26nm_lsb_mode_reg_num, 1
hynix26nm_lsb_mode_reg_num:
	.zero	1
	.global	para0
	.section	.rodata
	.align	3
	.type	para0, %object
	.size	para0, 56
para0:
	.hword	0
	.hword	0
	.hword	0
	.hword	0
	.hword	0
	.hword	6
	.hword	10
	.hword	6
	.hword	0
	.hword	-3
	.hword	-7
	.hword	-8
	.hword	0
	.hword	-6
	.hword	-13
	.hword	-15
	.hword	0
	.hword	-11
	.hword	-20
	.hword	-23
	.hword	0
	.hword	0
	.hword	-26
	.hword	-30
	.hword	0
	.hword	0
	.hword	-32
	.hword	-37
	.global	para1
	.align	3
	.type	para1, %object
	.size	para1, 56
para1:
	.hword	0
	.hword	0
	.hword	0
	.hword	0
	.hword	0
	.hword	6
	.hword	10
	.hword	6
	.hword	0
	.hword	-3
	.hword	-7
	.hword	-8
	.hword	0
	.hword	-6
	.hword	-13
	.hword	-15
	.hword	0
	.hword	-9
	.hword	-20
	.hword	-23
	.hword	0
	.hword	0
	.hword	-26
	.hword	-30
	.hword	0
	.hword	0
	.hword	-32
	.hword	-37
	.text
	.align	2
	.global	hynix26nm_vender_set_param
	.type	hynix26nm_vender_set_param, %function
hynix26nm_vender_set_param:
	stp	x29, x30, [sp, -64]!
	add	x29, sp, 0
	str	x0, [x29, 40]
	str	x1, [x29, 32]
	str	x2, [x29, 24]
	str	w3, [x29, 20]
	str	wzr, [x29, 56]
	mov	w0, 54
	strb	w0, [x29, 54]
	mov	w0, 22
	strb	w0, [x29, 55]
	ldr	x0, [x29, 40]
	bl	nand_enable_chip
	ldr	x0, [x29, 40]
	ldr	x0, [x0, 272]
	bl	ndfc_disable_randomize
	str	wzr, [x29, 60]
	b	.L425
.L426:
	ldr	w0, [x29, 60]
	ldr	x1, [x29, 24]
	add	x2, x1, x0
	ldr	w0, [x29, 60]
	ldr	x1, [x29, 32]
	add	x1, x1, x0
	add	x0, x29, 54
	mov	w6, 1
	mov	w5, 1
	mov	x4, x1
	mov	x3, x2
	mov	w2, 0
	mov	x1, x0
	ldr	x0, [x29, 40]
	bl	set_cmd_with_nand_bus
	mov	w2, 0
	ldrb	w1, [x29, 55]
	ldr	x0, [x29, 40]
	bl	set_one_cmd
	ldr	w0, [x29, 60]
	add	w0, w0, 1
	str	w0, [x29, 60]
.L425:
	ldr	w1, [x29, 60]
	ldr	w0, [x29, 20]
	cmp	w1, w0
	bcc	.L426
	ldr	x0, [x29, 40]
	bl	nand_disable_chip
	ldr	x0, [x29, 32]
	ldrb	w0, [x0]
	mov	w1, w0
	ldr	x0, [x29, 32]
	add	x0, x0, 1
	ldrb	w0, [x0]
	mov	w2, w0
	ldr	x0, [x29, 32]
	add	x0, x0, 2
	ldrb	w0, [x0]
	mov	w3, w0
	ldr	x0, [x29, 32]
	add	x0, x0, 3
	ldrb	w0, [x0]
	mov	w4, w0
	adrp	x0, .LC35
	add	x0, x0, :lo12:.LC35
	bl	nand_print_dbg
	ldr	w0, [x29, 56]
	ldp	x29, x30, [sp], 64
	ret
	.size	hynix26nm_vender_set_param, .-hynix26nm_vender_set_param
	.section	.rodata
	.align	3
.LC85:
	.string	"hynix26nm retry param:"
	.text
	.align	2
	.global	hynix26nm_set_readretry
	.type	hynix26nm_set_readretry, %function
hynix26nm_set_readretry:
	stp	x29, x30, [sp, -64]!
	add	x29, sp, 0
	str	x0, [x29, 24]
	str	wzr, [x29, 52]
	ldr	x0, [x29, 24]
	ldr	w0, [x0, 184]
	adrp	x1, hynix26nm_read_retry_cycle
	add	x1, x1, :lo12:hynix26nm_read_retry_cycle
	ldrb	w1, [x1]
	cmp	w0, w1
	bls	.L429
	mov	w0, -53
	b	.L441
.L429:
	str	wzr, [x29, 60]
	b	.L431
.L435:
	ldr	x1, [x29, 24]
	ldr	w0, [x29, 60]
	add	x0, x1, x0
	ldrb	w0, [x0, 56]
	strh	w0, [x29, 50]
	ldr	x0, [x29, 24]
	ldr	w2, [x0, 184]
	adrp	x0, hynix26nm_read_retry_val
	add	x0, x0, :lo12:hynix26nm_read_retry_val
	ldr	w1, [x29, 60]
	uxtw	x2, w2
	lsl	x2, x2, 2
	add	x1, x2, x1
	ldrsh	w0, [x0, x1, lsl 1]
	uxth	w1, w0
	ldrh	w0, [x29, 50]
	add	w0, w1, w0
	uxth	w0, w0
	strh	w0, [x29, 58]
	ldrsh	w0, [x29, 58]
	cmp	w0, 255
	ble	.L432
	mov	w0, 255
	strh	w0, [x29, 58]
	b	.L433
.L432:
	ldrsh	w0, [x29, 58]
	cmp	w0, 0
	bge	.L434
	strh	wzr, [x29, 58]
	b	.L433
.L434:
	ldrh	w0, [x29, 58]
	and	w0, w0, 255
	strh	w0, [x29, 58]
.L433:
	ldrh	w0, [x29, 58]
	uxtb	w2, w0
	ldr	w0, [x29, 60]
	sub	x1, x29, #4032
	add	x0, x1, x0
	mov	w1, w2
	strb	w1, [x0, 4072]
	ldr	w0, [x29, 60]
	add	w0, w0, 1
	str	w0, [x29, 60]
.L431:
	adrp	x0, hynix26nm_read_retry_reg_num
	add	x0, x0, :lo12:hynix26nm_read_retry_reg_num
	ldrb	w0, [x0]
	mov	w1, w0
	ldr	w0, [x29, 60]
	cmp	w1, w0
	bhi	.L435
	ldr	x0, [x29, 24]
	ldr	w0, [x0, 184]
	cmp	w0, 1
	bls	.L436
	ldr	x0, [x29, 24]
	ldr	w0, [x0, 184]
	cmp	w0, 6
	bhi	.L436
	strb	wzr, [x29, 40]
.L436:
	ldr	x0, [x29, 24]
	ldr	w0, [x0, 184]
	cmp	w0, 5
	beq	.L437
	ldr	x0, [x29, 24]
	ldr	w0, [x0, 184]
	cmp	w0, 6
	bne	.L438
.L437:
	strb	wzr, [x29, 41]
.L438:
	adrp	x0, hynix26nm_read_retry_reg_num
	add	x0, x0, :lo12:hynix26nm_read_retry_reg_num
	ldrb	w0, [x0]
	mov	w2, w0
	adrp	x0, hynix26nm_read_retry_reg_adr
	add	x1, x0, :lo12:hynix26nm_read_retry_reg_adr
	add	x0, x29, 40
	mov	w3, w2
	mov	x2, x1
	mov	x1, x0
	ldr	x0, [x29, 24]
	bl	hynix26nm_vender_set_param
	str	w0, [x29, 52]
	adrp	x0, .LC85
	add	x0, x0, :lo12:.LC85
	bl	nand_print_dbg
	str	wzr, [x29, 60]
	b	.L439
.L440:
	ldr	w0, [x29, 60]
	sub	x1, x29, #4032
	add	x0, x1, x0
	ldrb	w0, [x0, 4072]
	mov	w1, w0
	adrp	x0, .LC44
	add	x0, x0, :lo12:.LC44
	bl	nand_print_dbg
	ldr	w0, [x29, 60]
	add	w0, w0, 1
	str	w0, [x29, 60]
.L439:
	adrp	x0, hynix26nm_read_retry_reg_num
	add	x0, x0, :lo12:hynix26nm_read_retry_reg_num
	ldrb	w0, [x0]
	mov	w1, w0
	ldr	w0, [x29, 60]
	cmp	w1, w0
	bhi	.L440
	adrp	x0, .LC45
	add	x0, x0, :lo12:.LC45
	bl	nand_print_dbg
	ldr	w0, [x29, 52]
.L441:
	ldp	x29, x30, [sp], 64
	ret
	.size	hynix26nm_set_readretry, .-hynix26nm_set_readretry
	.section	.rodata
	.align	3
.LC86:
	.string	"m0 retry!\n"
	.align	3
.LC87:
	.string	"hynix26nm ReadRetry ok! ch =%d, chip = %d  block = %d, page = %d, RetryCount = %d  \n"
	.text
	.align	2
	.global	hynix26nm_read_page_end
	.type	hynix26nm_read_page_end, %function
hynix26nm_read_page_end:
	stp	x29, x30, [sp, -48]!
	add	x29, sp, 0
	str	x0, [x29, 24]
	str	wzr, [x29, 44]
	str	wzr, [x29, 40]
	adrp	x0, g_nsi
	add	x0, x0, :lo12:g_nsi
	ldr	x2, [x0]
	ldr	x0, [x29, 24]
	ldr	w0, [x0]
	mov	w1, w0
	mov	x0, x2
	bl	nci_get_from_nsi
	str	x0, [x29, 32]
	ldr	x0, [x29, 24]
	bl	generic_read_page_end_not_retry
	str	w0, [x29, 40]
	ldr	w0, [x29, 40]
	cmn	w0, #2
	bne	.L443
	adrp	x0, .LC86
	add	x0, x0, :lo12:.LC86
	bl	nand_print_dbg
	str	wzr, [x29, 44]
	b	.L444
.L450:
	str	wzr, [x29, 40]
	ldr	x0, [x29, 32]
	ldr	w0, [x0, 184]
	add	w1, w0, 1
	ldr	x0, [x29, 32]
	str	w1, [x0, 184]
	ldr	x0, [x29, 32]
	ldr	w0, [x0, 184]
	adrp	x1, hynix26nm_read_retry_cycle
	add	x1, x1, :lo12:hynix26nm_read_retry_cycle
	ldrb	w1, [x1]
	cmp	w0, w1
	bls	.L445
	ldr	x0, [x29, 32]
	str	wzr, [x0, 184]
.L445:
	ldr	x0, [x29, 32]
	bl	hynix26nm_set_readretry
	str	w0, [x29, 40]
	ldr	w0, [x29, 40]
	cmp	w0, 0
	beq	.L446
	ldr	x0, [x29, 32]
	str	wzr, [x0, 184]
	ldr	x0, [x29, 32]
	bl	hynix26nm_set_readretry
	b	.L447
.L446:
	ldr	x0, [x29, 24]
	bl	generic_read_page_start
	str	w0, [x29, 40]
	ldr	x0, [x29, 24]
	bl	generic_read_page_end_not_retry
	mov	w1, w0
	ldr	w0, [x29, 40]
	orr	w0, w0, w1
	str	w0, [x29, 40]
	ldr	w0, [x29, 40]
	cmp	w0, 10
	beq	.L448
	ldr	w0, [x29, 40]
	cmp	w0, 0
	bne	.L449
.L448:
	mov	w0, 10
	str	w0, [x29, 40]
	ldr	x0, [x29, 32]
	ldr	x0, [x0, 272]
	ldr	w1, [x0, 12]
	ldr	x0, [x29, 32]
	ldr	w2, [x0, 28]
	ldr	x0, [x29, 24]
	ldr	w3, [x0, 4]
	ldr	x0, [x29, 24]
	ldr	w4, [x0, 8]
	ldr	x0, [x29, 32]
	ldr	w5, [x0, 184]
	adrp	x0, .LC87
	add	x0, x0, :lo12:.LC87
	bl	nand_print_dbg
	b	.L447
.L449:
	ldr	w0, [x29, 44]
	add	w0, w0, 1
	str	w0, [x29, 44]
.L444:
	adrp	x0, hynix26nm_read_retry_cycle
	add	x0, x0, :lo12:hynix26nm_read_retry_cycle
	ldrb	w0, [x0]
	mov	w1, w0
	ldr	w0, [x29, 44]
	cmp	w1, w0
	bgt	.L450
.L447:
	ldr	x0, [x29, 32]
	str	wzr, [x0, 184]
	ldr	x0, [x29, 32]
	bl	hynix26nm_set_readretry
.L443:
	ldr	w0, [x29, 40]
	ldp	x29, x30, [sp], 48
	ret
	.size	hynix26nm_read_page_end, .-hynix26nm_read_page_end
	.align	2
	.global	hynix26nm_vender_get_param
	.type	hynix26nm_vender_get_param, %function
hynix26nm_vender_get_param:
	stp	x29, x30, [sp, -64]!
	add	x29, sp, 0
	str	x0, [x29, 40]
	str	x1, [x29, 32]
	str	x2, [x29, 24]
	str	w3, [x29, 20]
	str	wzr, [x29, 56]
	mov	w0, 55
	strb	w0, [x29, 55]
	ldr	x0, [x29, 40]
	bl	nand_enable_chip
	ldr	x0, [x29, 40]
	ldr	x0, [x0, 272]
	bl	ndfc_disable_randomize
	str	wzr, [x29, 60]
	b	.L453
.L454:
	ldr	w0, [x29, 60]
	ldr	x1, [x29, 24]
	add	x2, x1, x0
	ldr	w0, [x29, 60]
	ldr	x1, [x29, 32]
	add	x1, x1, x0
	add	x0, x29, 55
	mov	w4, 1
	mov	x3, x1
	mov	x1, x0
	ldr	x0, [x29, 40]
	bl	get_data_with_nand_bus_one_cmd
	ldr	w0, [x29, 60]
	add	w0, w0, 1
	str	w0, [x29, 60]
.L453:
	ldr	w1, [x29, 60]
	ldr	w0, [x29, 20]
	cmp	w1, w0
	bcc	.L454
	ldr	x0, [x29, 40]
	bl	nand_disable_chip
	ldr	w0, [x29, 56]
	ldp	x29, x30, [sp], 64
	ret
	.size	hynix26nm_vender_get_param, .-hynix26nm_vender_get_param
	.align	2
	.global	hynix26nm_lsb_init
	.type	hynix26nm_lsb_init, %function
hynix26nm_lsb_init:
	sub	sp, sp, #16
	str	x0, [sp, 8]
	ldr	x0, [sp, 8]
	ldr	x0, [x0, 280]
	ldr	w0, [x0, 52]
	lsr	w0, w0, 16
	uxtb	w1, w0
	adrp	x0, hynix26nm_read_retry_mode
	add	x0, x0, :lo12:hynix26nm_read_retry_mode
	strb	w1, [x0]
	ldr	x0, [sp, 8]
	ldr	x0, [x0, 280]
	ldr	w0, [x0, 52]
	lsr	w0, w0, 8
	uxtb	w1, w0
	adrp	x0, hynix26nm_read_retry_cycle
	add	x0, x0, :lo12:hynix26nm_read_retry_cycle
	strb	w1, [x0]
	ldr	x0, [sp, 8]
	ldr	x0, [x0, 280]
	ldr	w0, [x0, 52]
	uxtb	w1, w0
	adrp	x0, hynix26nm_read_retry_reg_num
	add	x0, x0, :lo12:hynix26nm_read_retry_reg_num
	strb	w1, [x0]
	adrp	x0, hynix26nm_lsb_mode_reg_num
	add	x0, x0, :lo12:hynix26nm_lsb_mode_reg_num
	mov	w1, 5
	strb	w1, [x0]
	adrp	x0, hynix26nm_lsb_mode_reg_adr
	add	x0, x0, :lo12:hynix26nm_lsb_mode_reg_adr
	mov	w1, -92
	strb	w1, [x0]
	adrp	x0, hynix26nm_lsb_mode_reg_adr
	add	x0, x0, :lo12:hynix26nm_lsb_mode_reg_adr
	mov	w1, -91
	strb	w1, [x0, 1]
	adrp	x0, hynix26nm_lsb_mode_reg_adr
	add	x0, x0, :lo12:hynix26nm_lsb_mode_reg_adr
	mov	w1, -80
	strb	w1, [x0, 2]
	adrp	x0, hynix26nm_lsb_mode_reg_adr
	add	x0, x0, :lo12:hynix26nm_lsb_mode_reg_adr
	mov	w1, -79
	strb	w1, [x0, 3]
	adrp	x0, hynix26nm_lsb_mode_reg_adr
	add	x0, x0, :lo12:hynix26nm_lsb_mode_reg_adr
	mov	w1, -55
	strb	w1, [x0, 4]
	adrp	x0, hynix26nm_lsb_mode_val
	add	x0, x0, :lo12:hynix26nm_lsb_mode_val
	mov	w1, 37
	strb	w1, [x0]
	adrp	x0, hynix26nm_lsb_mode_val
	add	x0, x0, :lo12:hynix26nm_lsb_mode_val
	mov	w1, 37
	strb	w1, [x0, 1]
	adrp	x0, hynix26nm_lsb_mode_val
	add	x0, x0, :lo12:hynix26nm_lsb_mode_val
	mov	w1, 37
	strb	w1, [x0, 2]
	adrp	x0, hynix26nm_lsb_mode_val
	add	x0, x0, :lo12:hynix26nm_lsb_mode_val
	mov	w1, 37
	strb	w1, [x0, 3]
	adrp	x0, hynix26nm_lsb_mode_val
	add	x0, x0, :lo12:hynix26nm_lsb_mode_val
	mov	w1, 1
	strb	w1, [x0, 4]
	mov	w0, 0
	add	sp, sp, 16
	ret
	.size	hynix26nm_lsb_init, .-hynix26nm_lsb_init
	.align	2
	.global	hynix26nm_lsb_enable
	.type	hynix26nm_lsb_enable, %function
hynix26nm_lsb_enable:
	stp	x29, x30, [sp, -48]!
	add	x29, sp, 0
	str	x0, [x29, 24]
	adrp	x0, hynix26nm_lsb_mode_reg_num
	add	x0, x0, :lo12:hynix26nm_lsb_mode_reg_num
	ldrb	w0, [x0]
	mov	w2, w0
	adrp	x0, hynix26nm_lsb_mode_reg_adr
	add	x1, x0, :lo12:hynix26nm_lsb_mode_reg_adr
	adrp	x0, hynix26nm_lsb_mode_default_val
	add	x0, x0, :lo12:hynix26nm_lsb_mode_default_val
	mov	w3, w2
	mov	x2, x1
	mov	x1, x0
	ldr	x0, [x29, 24]
	bl	hynix26nm_vender_get_param
	str	wzr, [x29, 44]
	b	.L459
.L460:
	adrp	x0, hynix26nm_lsb_mode_default_val
	add	x1, x0, :lo12:hynix26nm_lsb_mode_default_val
	ldr	w0, [x29, 44]
	ldrb	w2, [x1, x0]
	ldr	w0, [x29, 44]
	sub	x1, x29, #4048
	add	x0, x1, x0
	mov	w1, w2
	strb	w1, [x0, 4080]
	ldr	w0, [x29, 44]
	sub	x1, x29, #4048
	add	x0, x1, x0
	ldrb	w1, [x0, 4080]
	adrp	x0, hynix26nm_lsb_mode_val
	add	x2, x0, :lo12:hynix26nm_lsb_mode_val
	ldr	w0, [x29, 44]
	ldrb	w0, [x2, x0]
	add	w0, w1, w0
	uxtb	w2, w0
	ldr	w0, [x29, 44]
	sub	x1, x29, #4048
	add	x0, x1, x0
	mov	w1, w2
	strb	w1, [x0, 4080]
	ldr	w0, [x29, 44]
	add	w0, w0, 1
	str	w0, [x29, 44]
.L459:
	adrp	x0, hynix26nm_lsb_mode_reg_num
	add	x0, x0, :lo12:hynix26nm_lsb_mode_reg_num
	ldrb	w0, [x0]
	mov	w1, w0
	ldr	w0, [x29, 44]
	cmp	w1, w0
	bhi	.L460
	adrp	x0, hynix26nm_lsb_mode_reg_num
	add	x0, x0, :lo12:hynix26nm_lsb_mode_reg_num
	ldrb	w0, [x0]
	mov	w2, w0
	adrp	x0, hynix26nm_lsb_mode_reg_adr
	add	x1, x0, :lo12:hynix26nm_lsb_mode_reg_adr
	add	x0, x29, 32
	mov	w3, w2
	mov	x2, x1
	mov	x1, x0
	ldr	x0, [x29, 24]
	bl	hynix26nm_vender_set_param
	mov	w0, 0
	ldp	x29, x30, [sp], 48
	ret
	.size	hynix26nm_lsb_enable, .-hynix26nm_lsb_enable
	.align	2
	.global	hynix26nm_lsb_disable
	.type	hynix26nm_lsb_disable, %function
hynix26nm_lsb_disable:
	stp	x29, x30, [sp, -32]!
	add	x29, sp, 0
	str	x0, [x29, 24]
	adrp	x0, hynix26nm_lsb_mode_reg_num
	add	x0, x0, :lo12:hynix26nm_lsb_mode_reg_num
	ldrb	w0, [x0]
	mov	w2, w0
	adrp	x0, hynix26nm_lsb_mode_reg_adr
	add	x1, x0, :lo12:hynix26nm_lsb_mode_reg_adr
	adrp	x0, hynix26nm_lsb_mode_default_val
	add	x0, x0, :lo12:hynix26nm_lsb_mode_default_val
	mov	w3, w2
	mov	x2, x1
	mov	x1, x0
	ldr	x0, [x29, 24]
	bl	hynix26nm_vender_set_param
	mov	w0, 0
	ldp	x29, x30, [sp], 32
	ret
	.size	hynix26nm_lsb_disable, .-hynix26nm_lsb_disable
	.section	.rodata
	.align	3
.LC88:
	.string	"hynix26nm_lsb_exit\n"
	.text
	.align	2
	.global	hynix26nm_lsb_exit
	.type	hynix26nm_lsb_exit, %function
hynix26nm_lsb_exit:
	stp	x29, x30, [sp, -48]!
	add	x29, sp, 0
	str	x0, [x29, 24]
	mov	w0, 1048575
	str	w0, [x29, 44]
	ldr	x0, [x29, 24]
	bl	nand_enable_chip
	ldr	x0, [x29, 24]
	ldr	x0, [x0, 272]
	bl	ndfc_disable_randomize
	mov	w2, 0
	mov	w1, 0
	ldr	x0, [x29, 24]
	bl	set_one_cmd
	mov	w1, 0
	ldr	x0, [x29, 24]
	bl	set_one_addr
	mov	w1, 0
	ldr	x0, [x29, 24]
	bl	set_one_addr
	mov	w1, 0
	ldr	x0, [x29, 24]
	bl	set_one_addr
	mov	w1, 0
	ldr	x0, [x29, 24]
	bl	set_one_addr
	mov	w1, 0
	ldr	x0, [x29, 24]
	bl	set_one_addr
	mov	w2, 1
	mov	w1, 48
	ldr	x0, [x29, 24]
	bl	set_one_cmd
	nop
.L465:
	ldr	w0, [x29, 44]
	sub	w1, w0, #1
	str	w1, [x29, 44]
	cmp	w0, 0
	bgt	.L465
	adrp	x0, .LC88
	add	x0, x0, :lo12:.LC88
	bl	nand_print_dbg
	ldr	x0, [x29, 24]
	bl	nand_disable_chip
	mov	w0, 0
	ldp	x29, x30, [sp], 48
	ret
	.size	hynix26nm_lsb_exit, .-hynix26nm_lsb_exit
	.align	2
	.global	hynix26nm_getdefaultparam
	.type	hynix26nm_getdefaultparam, %function
hynix26nm_getdefaultparam:
	stp	x29, x30, [sp, -48]!
	add	x29, sp, 0
	str	x0, [x29, 24]
	str	x1, [x29, 16]
	str	wzr, [x29, 40]
	ldr	x0, [x29, 24]
	add	x1, x0, 56
	adrp	x0, hynix26nm_read_retry_reg_num
	add	x0, x0, :lo12:hynix26nm_read_retry_reg_num
	ldrb	w0, [x0]
	mov	w2, w0
	adrp	x0, hynix26nm_read_retry_reg_adr
	add	x0, x0, :lo12:hynix26nm_read_retry_reg_adr
	mov	w3, w2
	mov	x2, x0
	ldr	x0, [x29, 24]
	bl	hynix26nm_vender_get_param
	str	w0, [x29, 40]
	str	wzr, [x29, 44]
	b	.L468
.L469:
	ldrsw	x0, [x29, 44]
	ldr	x1, [x29, 16]
	add	x0, x1, x0
	ldr	x2, [x29, 24]
	ldrsw	x1, [x29, 44]
	add	x1, x2, x1
	ldrb	w1, [x1, 56]
	strb	w1, [x0]
	ldr	w0, [x29, 44]
	add	w0, w0, 1
	str	w0, [x29, 44]
.L468:
	adrp	x0, hynix26nm_read_retry_reg_num
	add	x0, x0, :lo12:hynix26nm_read_retry_reg_num
	ldrb	w0, [x0]
	mov	w1, w0
	ldr	w0, [x29, 44]
	cmp	w1, w0
	bgt	.L469
	ldr	w0, [x29, 40]
	ldp	x29, x30, [sp], 48
	ret
	.size	hynix26nm_getdefaultparam, .-hynix26nm_getdefaultparam
	.align	2
	.global	hynix26nm_setdefaultparam
	.type	hynix26nm_setdefaultparam, %function
hynix26nm_setdefaultparam:
	stp	x29, x30, [sp, -48]!
	add	x29, sp, 0
	str	x0, [x29, 24]
	str	wzr, [x29, 40]
	str	wzr, [x29, 44]
	b	.L472
.L473:
	ldr	x1, [x29, 24]
	ldrsw	x0, [x29, 44]
	add	x0, x1, x0
	ldrb	w2, [x0, 56]
	ldrsw	x0, [x29, 44]
	sub	x1, x29, #4048
	add	x0, x1, x0
	mov	w1, w2
	strb	w1, [x0, 4080]
	ldr	w0, [x29, 44]
	add	w0, w0, 1
	str	w0, [x29, 44]
.L472:
	adrp	x0, hynix26nm_read_retry_reg_num
	add	x0, x0, :lo12:hynix26nm_read_retry_reg_num
	ldrb	w0, [x0]
	mov	w1, w0
	ldr	w0, [x29, 44]
	cmp	w1, w0
	bgt	.L473
	adrp	x0, hynix26nm_read_retry_reg_num
	add	x0, x0, :lo12:hynix26nm_read_retry_reg_num
	ldrb	w0, [x0]
	mov	w2, w0
	adrp	x0, hynix26nm_read_retry_reg_adr
	add	x1, x0, :lo12:hynix26nm_read_retry_reg_adr
	add	x0, x29, 32
	mov	w3, w2
	mov	x2, x1
	mov	x1, x0
	ldr	x0, [x29, 24]
	bl	hynix26nm_vender_set_param
	str	w0, [x29, 40]
	adrp	x0, .LC56
	add	x0, x0, :lo12:.LC56
	bl	nand_print_dbg
	str	wzr, [x29, 44]
	b	.L474
.L475:
	ldrsw	x0, [x29, 44]
	sub	x1, x29, #4048
	add	x0, x1, x0
	ldrb	w0, [x0, 4080]
	mov	w1, w0
	adrp	x0, .LC57
	add	x0, x0, :lo12:.LC57
	bl	nand_print_dbg
	ldr	w0, [x29, 44]
	add	w0, w0, 1
	str	w0, [x29, 44]
.L474:
	adrp	x0, hynix26nm_read_retry_reg_num
	add	x0, x0, :lo12:hynix26nm_read_retry_reg_num
	ldrb	w0, [x0]
	mov	w1, w0
	ldr	w0, [x29, 44]
	cmp	w1, w0
	bgt	.L475
	adrp	x0, .LC45
	add	x0, x0, :lo12:.LC45
	bl	nand_print_dbg
	ldr	w0, [x29, 40]
	ldp	x29, x30, [sp], 48
	ret
	.size	hynix26nm_setdefaultparam, .-hynix26nm_setdefaultparam
	.section	.rodata
	.align	3
.LC89:
	.string	"hynix26nm_get_default_param: ch: %d, chip: %d, value: 0x%x 0x%x 0x%x 0x%x \n"
	.text
	.align	2
	.global	hynix26nm_get_default_param
	.type	hynix26nm_get_default_param, %function
hynix26nm_get_default_param:
	stp	x29, x30, [sp, -48]!
	add	x29, sp, 0
	str	x0, [x29, 24]
	add	x0, x29, 40
	mov	x1, x0
	ldr	x0, [x29, 24]
	bl	hynix26nm_getdefaultparam
	ldr	x0, [x29, 24]
	ldr	x0, [x0, 272]
	ldr	w1, [x0, 12]
	ldr	x0, [x29, 24]
	ldr	w2, [x0, 28]
	ldrb	w0, [x29, 40]
	mov	w3, w0
	ldrb	w0, [x29, 41]
	mov	w4, w0
	ldrb	w0, [x29, 42]
	mov	w5, w0
	ldrb	w0, [x29, 43]
	mov	w6, w0
	adrp	x0, .LC89
	add	x0, x0, :lo12:.LC89
	bl	nand_print_dbg
	ldr	x0, [x29, 24]
	bl	hynix26nm_setdefaultparam
	mov	w0, 0
	ldp	x29, x30, [sp], 48
	ret
	.size	hynix26nm_get_default_param, .-hynix26nm_get_default_param
	.align	2
	.global	hynix26nm_readretry_init
	.type	hynix26nm_readretry_init, %function
hynix26nm_readretry_init:
	stp	x29, x30, [sp, -48]!
	add	x29, sp, 0
	str	x0, [x29, 24]
	ldr	x0, [x29, 24]
	ldr	x0, [x0, 280]
	ldr	w0, [x0, 52]
	lsr	w0, w0, 16
	uxtb	w1, w0
	adrp	x0, hynix26nm_read_retry_mode
	add	x0, x0, :lo12:hynix26nm_read_retry_mode
	strb	w1, [x0]
	ldr	x0, [x29, 24]
	ldr	x0, [x0, 280]
	ldr	w0, [x0, 52]
	lsr	w0, w0, 8
	uxtb	w1, w0
	adrp	x0, hynix26nm_read_retry_cycle
	add	x0, x0, :lo12:hynix26nm_read_retry_cycle
	strb	w1, [x0]
	ldr	x0, [x29, 24]
	ldr	x0, [x0, 280]
	ldr	w0, [x0, 52]
	uxtb	w1, w0
	adrp	x0, hynix26nm_read_retry_reg_num
	add	x0, x0, :lo12:hynix26nm_read_retry_reg_num
	strb	w1, [x0]
	adrp	x0, hynix26nm_read_retry_mode
	add	x0, x0, :lo12:hynix26nm_read_retry_mode
	ldrb	w0, [x0]
	cmp	w0, 0
	bne	.L480
	adrp	x0, hynix26nm_read_retry_reg_adr
	add	x0, x0, :lo12:hynix26nm_read_retry_reg_adr
	mov	w1, -84
	strb	w1, [x0]
	adrp	x0, hynix26nm_read_retry_reg_adr
	add	x0, x0, :lo12:hynix26nm_read_retry_reg_adr
	mov	w1, -83
	strb	w1, [x0, 1]
	adrp	x0, hynix26nm_read_retry_reg_adr
	add	x0, x0, :lo12:hynix26nm_read_retry_reg_adr
	mov	w1, -82
	strb	w1, [x0, 2]
	adrp	x0, hynix26nm_read_retry_reg_adr
	add	x0, x0, :lo12:hynix26nm_read_retry_reg_adr
	mov	w1, -81
	strb	w1, [x0, 3]
	str	wzr, [x29, 44]
	b	.L481
.L484:
	str	wzr, [x29, 40]
	b	.L482
.L483:
	adrp	x0, para0
	add	x0, x0, :lo12:para0
	ldr	w1, [x29, 40]
	ldr	w2, [x29, 44]
	lsl	x2, x2, 2
	add	x1, x2, x1
	ldrsh	w3, [x0, x1, lsl 1]
	adrp	x0, hynix26nm_read_retry_val
	add	x0, x0, :lo12:hynix26nm_read_retry_val
	ldr	w1, [x29, 40]
	ldr	w2, [x29, 44]
	lsl	x2, x2, 2
	add	x1, x2, x1
	mov	w2, w3
	strh	w2, [x0, x1, lsl 1]
	ldr	w0, [x29, 40]
	add	w0, w0, 1
	str	w0, [x29, 40]
.L482:
	adrp	x0, hynix26nm_read_retry_reg_num
	add	x0, x0, :lo12:hynix26nm_read_retry_reg_num
	ldrb	w0, [x0]
	mov	w1, w0
	ldr	w0, [x29, 40]
	cmp	w1, w0
	bhi	.L483
	ldr	w0, [x29, 44]
	add	w0, w0, 1
	str	w0, [x29, 44]
.L481:
	adrp	x0, hynix26nm_read_retry_cycle
	add	x0, x0, :lo12:hynix26nm_read_retry_cycle
	ldrb	w0, [x0]
	add	w0, w0, 1
	mov	w1, w0
	ldr	w0, [x29, 44]
	cmp	w1, w0
	bhi	.L484
	b	.L485
.L480:
	adrp	x0, hynix26nm_read_retry_mode
	add	x0, x0, :lo12:hynix26nm_read_retry_mode
	ldrb	w0, [x0]
	cmp	w0, 1
	bne	.L486
	adrp	x0, hynix26nm_read_retry_reg_adr
	add	x0, x0, :lo12:hynix26nm_read_retry_reg_adr
	mov	w1, -89
	strb	w1, [x0]
	adrp	x0, hynix26nm_read_retry_reg_adr
	add	x0, x0, :lo12:hynix26nm_read_retry_reg_adr
	mov	w1, -83
	strb	w1, [x0, 1]
	adrp	x0, hynix26nm_read_retry_reg_adr
	add	x0, x0, :lo12:hynix26nm_read_retry_reg_adr
	mov	w1, -82
	strb	w1, [x0, 2]
	adrp	x0, hynix26nm_read_retry_reg_adr
	add	x0, x0, :lo12:hynix26nm_read_retry_reg_adr
	mov	w1, -81
	strb	w1, [x0, 3]
	str	wzr, [x29, 44]
	b	.L487
.L490:
	str	wzr, [x29, 40]
	b	.L488
.L489:
	adrp	x0, para1
	add	x0, x0, :lo12:para1
	ldr	w1, [x29, 40]
	ldr	w2, [x29, 44]
	lsl	x2, x2, 2
	add	x1, x2, x1
	ldrsh	w3, [x0, x1, lsl 1]
	adrp	x0, hynix26nm_read_retry_val
	add	x0, x0, :lo12:hynix26nm_read_retry_val
	ldr	w1, [x29, 40]
	ldr	w2, [x29, 44]
	lsl	x2, x2, 2
	add	x1, x2, x1
	mov	w2, w3
	strh	w2, [x0, x1, lsl 1]
	ldr	w0, [x29, 40]
	add	w0, w0, 1
	str	w0, [x29, 40]
.L488:
	adrp	x0, hynix26nm_read_retry_reg_num
	add	x0, x0, :lo12:hynix26nm_read_retry_reg_num
	ldrb	w0, [x0]
	mov	w1, w0
	ldr	w0, [x29, 40]
	cmp	w1, w0
	bhi	.L489
	ldr	w0, [x29, 44]
	add	w0, w0, 1
	str	w0, [x29, 44]
.L487:
	adrp	x0, hynix26nm_read_retry_cycle
	add	x0, x0, :lo12:hynix26nm_read_retry_cycle
	ldrb	w0, [x0]
	add	w0, w0, 1
	mov	w1, w0
	ldr	w0, [x29, 44]
	cmp	w1, w0
	bhi	.L490
	b	.L485
.L486:
	adrp	x0, hynix26nm_read_retry_mode
	add	x0, x0, :lo12:hynix26nm_read_retry_mode
	ldrb	w0, [x0]
	mov	w1, w0
	adrp	x0, .LC64
	add	x0, x0, :lo12:.LC64
	bl	nand_print
	mov	w0, -54
	b	.L491
.L485:
	ldr	x0, [x29, 24]
	str	wzr, [x0, 184]
	str	wzr, [x29, 44]
	b	.L492
.L493:
	ldr	x1, [x29, 24]
	ldr	w0, [x29, 44]
	add	x0, x1, x0
	strb	wzr, [x0, 56]
	ldr	w0, [x29, 44]
	add	w0, w0, 1
	str	w0, [x29, 44]
.L492:
	ldr	w0, [x29, 44]
	cmp	w0, 127
	bls	.L493
	ldr	x0, [x29, 24]
	bl	hynix26nm_get_default_param
	mov	w0, 0
.L491:
	ldp	x29, x30, [sp], 48
	ret
	.size	hynix26nm_readretry_init, .-hynix26nm_readretry_init
	.section	.rodata
	.align	3
.LC90:
	.string	"hynix26nm_readretry_exit. \n"
	.text
	.align	2
	.global	hynix26nm_readretry_exit
	.type	hynix26nm_readretry_exit, %function
hynix26nm_readretry_exit:
	stp	x29, x30, [sp, -32]!
	add	x29, sp, 0
	str	x0, [x29, 24]
	adrp	x0, .LC90
	add	x0, x0, :lo12:.LC90
	bl	nand_print_dbg
	ldr	x0, [x29, 24]
	bl	hynix26nm_setdefaultparam
	ldr	x0, [x29, 24]
	str	wzr, [x0, 184]
	mov	w0, 0
	ldp	x29, x30, [sp], 32
	ret
	.size	hynix26nm_readretry_exit, .-hynix26nm_readretry_exit
	.section	.rodata
	.align	3
.LC91:
	.string	" hynix26nm_special_init hynix26nm_read_retry_mode: %d hynix26nm_read_retry_cycle:%d hynix26nm_read_retry_reg_num:%d\n"
	.align	3
.LC92:
	.string	" hynix26nm_special_init error hynix26nm_read_retry_mode: %d hynix26nm_read_retry_cycle:%d hynix26nm_read_retry_reg_num:%d\n"
	.text
	.align	2
	.global	hynix26nm_special_init
	.type	hynix26nm_special_init, %function
hynix26nm_special_init:
	stp	x29, x30, [sp, -32]!
	add	x29, sp, 0
	str	wzr, [x29, 28]
	adrp	x0, g_nsi
	add	x0, x0, :lo12:g_nsi
	ldr	x0, [x0]
	ldr	x0, [x0, 8]
	str	x0, [x29, 16]
	b	.L497
.L498:
	ldr	x0, [x29, 16]
	bl	hynix26nm_readretry_init
	mov	w1, w0
	ldr	w0, [x29, 28]
	orr	w0, w0, w1
	str	w0, [x29, 28]
	ldr	x0, [x29, 16]
	ldr	x0, [x0]
	str	x0, [x29, 16]
.L497:
	ldr	x0, [x29, 16]
	cmp	x0, 0
	bne	.L498
	ldr	w0, [x29, 28]
	cmp	w0, 0
	bne	.L499
	adrp	x0, df_read_page_end
	add	x0, x0, :lo12:df_read_page_end
	adrp	x1, hynix26nm_read_page_end
	add	x1, x1, :lo12:hynix26nm_read_page_end
	str	x1, [x0]
	adrp	x0, hynix26nm_read_retry_mode
	add	x0, x0, :lo12:hynix26nm_read_retry_mode
	ldrb	w0, [x0]
	mov	w1, w0
	adrp	x0, hynix26nm_read_retry_cycle
	add	x0, x0, :lo12:hynix26nm_read_retry_cycle
	ldrb	w0, [x0]
	mov	w2, w0
	adrp	x0, hynix26nm_read_retry_reg_num
	add	x0, x0, :lo12:hynix26nm_read_retry_reg_num
	ldrb	w0, [x0]
	mov	w3, w0
	adrp	x0, .LC91
	add	x0, x0, :lo12:.LC91
	bl	nand_print_dbg
	b	.L500
.L499:
	adrp	x0, hynix26nm_read_retry_mode
	add	x0, x0, :lo12:hynix26nm_read_retry_mode
	ldrb	w0, [x0]
	mov	w1, w0
	adrp	x0, hynix26nm_read_retry_cycle
	add	x0, x0, :lo12:hynix26nm_read_retry_cycle
	ldrb	w0, [x0]
	mov	w2, w0
	adrp	x0, hynix26nm_read_retry_reg_num
	add	x0, x0, :lo12:hynix26nm_read_retry_reg_num
	ldrb	w0, [x0]
	mov	w3, w0
	adrp	x0, .LC92
	add	x0, x0, :lo12:.LC92
	bl	nand_print
.L500:
	ldr	w0, [x29, 28]
	ldp	x29, x30, [sp], 32
	ret
	.size	hynix26nm_special_init, .-hynix26nm_special_init
	.section	.rodata
	.align	3
.LC93:
	.string	" hynix26nm_special_exit \n"
	.text
	.align	2
	.global	hynix26nm_special_exit
	.type	hynix26nm_special_exit, %function
hynix26nm_special_exit:
	stp	x29, x30, [sp, -32]!
	add	x29, sp, 0
	adrp	x0, g_nsi
	add	x0, x0, :lo12:g_nsi
	ldr	x0, [x0]
	ldr	x0, [x0, 8]
	str	x0, [x29, 24]
	b	.L503
.L504:
	ldr	x0, [x29, 24]
	bl	hynix26nm_readretry_exit
	ldr	x0, [x29, 24]
	ldr	x0, [x0]
	str	x0, [x29, 24]
.L503:
	ldr	x0, [x29, 24]
	cmp	x0, 0
	bne	.L504
	adrp	x0, .LC93
	add	x0, x0, :lo12:.LC93
	bl	nand_print_dbg
	mov	w0, 0
	ldp	x29, x30, [sp], 32
	ret
	.size	hynix26nm_special_exit, .-hynix26nm_special_exit
	.global	sandisk_A19_read_retry_mode
	.bss
	.type	sandisk_A19_read_retry_mode, %object
	.size	sandisk_A19_read_retry_mode, 1
sandisk_A19_read_retry_mode:
	.zero	1
	.global	sandisk_A19_read_retry_cycle
	.type	sandisk_A19_read_retry_cycle, %object
	.size	sandisk_A19_read_retry_cycle, 1
sandisk_A19_read_retry_cycle:
	.zero	1
	.global	sandisk_A19_read_retry_reg_cnt
	.type	sandisk_A19_read_retry_reg_cnt, %object
	.size	sandisk_A19_read_retry_reg_cnt, 1
sandisk_A19_read_retry_reg_cnt:
	.zero	1
	.global	sandisk_A19_read_retry_enable_cmd_1y
	.data
	.align	3
	.type	sandisk_A19_read_retry_enable_cmd_1y, %object
	.size	sandisk_A19_read_retry_enable_cmd_1y, 1
sandisk_A19_read_retry_enable_cmd_1y:
	.byte	93
	.global	sandisk_A19_read_retry_exit_cmd_1y
	.align	3
	.type	sandisk_A19_read_retry_exit_cmd_1y, %object
	.size	sandisk_A19_read_retry_exit_cmd_1y, 1
sandisk_A19_read_retry_exit_cmd_1y:
	.byte	-1
	.global	sandisk_A19_read_retry_cmd_1y
	.align	3
	.type	sandisk_A19_read_retry_cmd_1y, %object
	.size	sandisk_A19_read_retry_cmd_1y, 1
sandisk_A19_read_retry_cmd_1y:
	.byte	-17
	.global	sandisk_A19_read_retry_reg_adr_1y
	.align	3
	.type	sandisk_A19_read_retry_reg_adr_1y, %object
	.size	sandisk_A19_read_retry_reg_adr_1y, 1
sandisk_A19_read_retry_reg_adr_1y:
	.byte	17
	.global	sandisk_A19_1y
	.align	3
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
	.global	sandisk_A19_1z_16g
	.align	3
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
	.global	sandisk_A19_1z_8g
	.align	3
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
	.local	sandisk_A19_sclk0_bak
	.comm	sandisk_A19_sclk0_bak,4,4
	.local	sandisk_A19_sclk1_bak
	.comm	sandisk_A19_sclk1_bak,4,4
	.text
	.align	2
	.global	sandisk_A19_dsp_on
	.type	sandisk_A19_dsp_on, %function
sandisk_A19_dsp_on:
	stp	x29, x30, [sp, -48]!
	add	x29, sp, 0
	str	x0, [x29, 24]
	adrp	x0, g_nsi
	add	x0, x0, :lo12:g_nsi
	ldr	x2, [x0]
	ldr	x0, [x29, 24]
	ldr	w0, [x0]
	mov	w1, w0
	mov	x0, x2
	bl	nci_get_from_nsi
	str	x0, [x29, 40]
	ldr	x0, [x29, 40]
	bl	nand_enable_chip
	ldr	x0, [x29, 40]
	ldr	x0, [x0, 272]
	bl	ndfc_disable_randomize
	mov	w2, 0
	mov	w1, 38
	ldr	x0, [x29, 40]
	bl	set_one_cmd
	ldr	x0, [x29, 40]
	bl	nand_disable_chip
	mov	w0, 0
	ldp	x29, x30, [sp], 48
	ret
	.size	sandisk_A19_dsp_on, .-sandisk_A19_dsp_on
	.align	2
	.global	sandisk_A19_cmd25
	.type	sandisk_A19_cmd25, %function
sandisk_A19_cmd25:
	stp	x29, x30, [sp, -48]!
	add	x29, sp, 0
	str	x0, [x29, 24]
	adrp	x0, g_nsi
	add	x0, x0, :lo12:g_nsi
	ldr	x2, [x0]
	ldr	x0, [x29, 24]
	ldr	w0, [x0]
	mov	w1, w0
	mov	x0, x2
	bl	nci_get_from_nsi
	str	x0, [x29, 40]
	ldr	x0, [x29, 40]
	bl	nand_enable_chip
	ldr	x0, [x29, 40]
	ldr	x0, [x0, 272]
	bl	ndfc_disable_randomize
	mov	w2, 0
	mov	w1, 37
	ldr	x0, [x29, 40]
	bl	set_one_cmd
	ldr	x0, [x29, 40]
	bl	nand_disable_chip
	mov	w0, 0
	ldp	x29, x30, [sp], 48
	ret
	.size	sandisk_A19_cmd25, .-sandisk_A19_cmd25
	.align	2
	.global	sandisk_A19_set_lmflgfix_next
	.type	sandisk_A19_set_lmflgfix_next, %function
sandisk_A19_set_lmflgfix_next:
	stp	x29, x30, [sp, -48]!
	add	x29, sp, 0
	str	x0, [x29, 24]
	strb	w1, [x29, 23]
	adrp	x0, g_nsi
	add	x0, x0, :lo12:g_nsi
	ldr	x2, [x0]
	ldr	x0, [x29, 24]
	ldr	w0, [x0]
	mov	w1, w0
	mov	x0, x2
	bl	nci_get_from_nsi
	str	x0, [x29, 40]
	ldr	x0, [x29, 40]
	bl	nand_enable_chip
	ldr	x0, [x29, 40]
	ldr	x0, [x0, 272]
	bl	ndfc_disable_randomize
	mov	w2, 0
	mov	w1, 92
	ldr	x0, [x29, 40]
	bl	set_one_cmd
	mov	w2, 0
	mov	w1, -59
	ldr	x0, [x29, 40]
	bl	set_one_cmd
	strb	wzr, [x29, 39]
	mov	w0, 1
	strb	w0, [x29, 38]
	mov	w0, 85
	strb	w0, [x29, 37]
	add	x2, x29, 38
	add	x1, x29, 39
	add	x0, x29, 37
	mov	w6, 1
	mov	w5, 1
	mov	x4, x2
	mov	x3, x1
	mov	w2, 1
	mov	x1, x0
	ldr	x0, [x29, 40]
	bl	set_cmd_with_nand_bus
	adrp	x0, sandisk_A19_read_retry_mode
	add	x0, x0, :lo12:sandisk_A19_read_retry_mode
	ldrb	w0, [x0]
	cmp	w0, 52
	bne	.L511
	mov	w0, 34
	strb	w0, [x29, 39]
	b	.L512
.L511:
	adrp	x0, sandisk_A19_read_retry_mode
	add	x0, x0, :lo12:sandisk_A19_read_retry_mode
	ldrb	w0, [x0]
	cmp	w0, 53
	bne	.L513
	mov	w0, 37
	strb	w0, [x29, 39]
	b	.L512
.L513:
	mov	w0, 35
	strb	w0, [x29, 39]
.L512:
	ldrb	w0, [x29, 23]
	strb	w0, [x29, 38]
	mov	w0, 85
	strb	w0, [x29, 37]
	add	x2, x29, 38
	add	x1, x29, 39
	add	x0, x29, 37
	mov	w6, 1
	mov	w5, 1
	mov	x4, x2
	mov	x3, x1
	mov	w2, 1
	mov	x1, x0
	ldr	x0, [x29, 40]
	bl	set_cmd_with_nand_bus
	strb	wzr, [x29, 39]
	strb	wzr, [x29, 38]
	mov	w0, 85
	strb	w0, [x29, 37]
	add	x2, x29, 38
	add	x1, x29, 39
	add	x0, x29, 37
	mov	w6, 1
	mov	w5, 1
	mov	x4, x2
	mov	x3, x1
	mov	w2, 1
	mov	x1, x0
	ldr	x0, [x29, 40]
	bl	set_cmd_with_nand_bus
	ldr	x0, [x29, 40]
	bl	nand_disable_chip
	mov	w0, 0
	ldp	x29, x30, [sp], 48
	ret
	.size	sandisk_A19_set_lmflgfix_next, .-sandisk_A19_set_lmflgfix_next
	.section	.rodata
	.align	3
.LC94:
	.string	"sandisk_A19 retry!\n"
	.align	3
.LC95:
	.string	"sandisk_A19 set readretry error\n"
	.align	3
.LC96:
	.string	"sandisk_A19 ReadRetry ok! ch =%d, chip = %d  block = %d, page = %d, RetryCount = %d  \n"
	.align	3
.LC97:
	.string	"sandisk_A19 ReadRetry ok with ecc limit! ch =%d, chip = %d  block = %d, page = %d, RetryCount = %d  \n"
	.text
	.align	2
	.global	sandisk_A19_read_page_end
	.type	sandisk_A19_read_page_end, %function
sandisk_A19_read_page_end:
	stp	x29, x30, [sp, -48]!
	add	x29, sp, 0
	str	x0, [x29, 24]
	str	wzr, [x29, 44]
	str	wzr, [x29, 40]
	adrp	x0, g_nsi
	add	x0, x0, :lo12:g_nsi
	ldr	x2, [x0]
	ldr	x0, [x29, 24]
	ldr	w0, [x0]
	mov	w1, w0
	mov	x0, x2
	bl	nci_get_from_nsi
	str	x0, [x29, 32]
	ldr	x0, [x29, 24]
	bl	generic_read_page_end_not_retry
	str	w0, [x29, 40]
	ldr	w0, [x29, 40]
	cmn	w0, #2
	bne	.L516
	adrp	x0, .LC94
	add	x0, x0, :lo12:.LC94
	bl	nand_print_dbg
	str	wzr, [x29, 44]
	b	.L517
.L525:
	ldr	w1, [x29, 44]
	ldr	x0, [x29, 32]
	str	w1, [x0, 184]
	ldr	x0, [x29, 24]
	ldr	w0, [x0, 8]
	uxth	w0, w0
	mov	w1, w0
	ldr	x0, [x29, 32]
	bl	sandisk_A19_set_readretry
	str	w0, [x29, 40]
	ldr	w0, [x29, 40]
	cmp	w0, 0
	beq	.L518
	adrp	x0, .LC95
	add	x0, x0, :lo12:.LC95
	bl	nand_print_dbg
	b	.L519
.L518:
	ldr	x0, [x29, 24]
	bl	generic_read_page_start
	str	w0, [x29, 40]
	ldr	x0, [x29, 24]
	bl	generic_read_page_end_not_retry
	mov	w1, w0
	ldr	w0, [x29, 40]
	orr	w0, w0, w1
	str	w0, [x29, 40]
	ldr	x0, [x29, 24]
	ldr	x0, [x0, 24]
	bl	is_nouse_page
	cmp	w0, 1
	bne	.L520
	mov	w0, -2
	str	w0, [x29, 40]
	ldr	x0, [x29, 32]
	ldr	x0, [x0, 272]
	ldr	w1, [x0, 12]
	ldr	x0, [x29, 32]
	ldr	w2, [x0, 28]
	ldr	x0, [x29, 24]
	ldr	w3, [x0, 4]
	ldr	x0, [x29, 24]
	ldr	w4, [x0, 8]
	ldr	x0, [x29, 32]
	ldr	w5, [x0, 184]
	adrp	x0, .LC47
	add	x0, x0, :lo12:.LC47
	bl	nand_print_dbg
	b	.L519
.L520:
	ldr	w0, [x29, 40]
	cmp	w0, 10
	beq	.L521
	ldr	w0, [x29, 40]
	cmp	w0, 0
	bne	.L519
.L521:
	ldr	w0, [x29, 40]
	cmp	w0, 0
	bne	.L522
	ldr	x0, [x29, 32]
	ldr	x0, [x0, 272]
	ldr	w1, [x0, 12]
	ldr	x0, [x29, 32]
	ldr	w2, [x0, 28]
	ldr	x0, [x29, 24]
	ldr	w3, [x0, 4]
	ldr	x0, [x29, 24]
	ldr	w4, [x0, 8]
	ldr	x0, [x29, 32]
	ldr	w5, [x0, 184]
	adrp	x0, .LC96
	add	x0, x0, :lo12:.LC96
	bl	nand_print_dbg
	b	.L523
.L522:
	ldr	x0, [x29, 32]
	ldr	x0, [x0, 272]
	ldr	w1, [x0, 12]
	ldr	x0, [x29, 32]
	ldr	w2, [x0, 28]
	ldr	x0, [x29, 24]
	ldr	w3, [x0, 4]
	ldr	x0, [x29, 24]
	ldr	w4, [x0, 8]
	ldr	x0, [x29, 32]
	ldr	w5, [x0, 184]
	adrp	x0, .LC97
	add	x0, x0, :lo12:.LC97
	bl	nand_print_dbg
.L523:
	mov	w0, 10
	str	w0, [x29, 40]
	b	.L524
.L519:
	ldr	w0, [x29, 44]
	add	w0, w0, 1
	str	w0, [x29, 44]
.L517:
	adrp	x0, sandisk_A19_read_retry_cycle
	add	x0, x0, :lo12:sandisk_A19_read_retry_cycle
	ldrb	w0, [x0]
	mov	w1, w0
	ldr	w0, [x29, 44]
	cmp	w1, w0
	bgt	.L525
	adrp	x0, sandisk_A19_read_retry_mode
	add	x0, x0, :lo12:sandisk_A19_read_retry_mode
	ldrb	w0, [x0]
	cmp	w0, 51
	beq	.L561
	str	wzr, [x29, 44]
	b	.L528
.L535:
	ldr	w1, [x29, 44]
	ldr	x0, [x29, 32]
	str	w1, [x0, 184]
	ldr	x0, [x29, 24]
	ldr	w0, [x0, 8]
	uxth	w0, w0
	mov	w1, w0
	ldr	x0, [x29, 32]
	bl	sandisk_A19_set_readretry
	str	w0, [x29, 40]
	ldr	w0, [x29, 40]
	cmp	w0, 0
	beq	.L529
	adrp	x0, .LC95
	add	x0, x0, :lo12:.LC95
	bl	nand_print_dbg
	b	.L530
.L529:
	ldr	x0, [x29, 24]
	bl	sandisk_A19_dsp_on
	ldr	x0, [x29, 24]
	bl	generic_read_page_start
	str	w0, [x29, 40]
	ldr	x0, [x29, 24]
	bl	generic_read_page_end_not_retry
	mov	w1, w0
	ldr	w0, [x29, 40]
	orr	w0, w0, w1
	str	w0, [x29, 40]
	ldr	x0, [x29, 24]
	ldr	x0, [x0, 24]
	bl	is_nouse_page
	cmp	w0, 1
	bne	.L531
	mov	w0, -2
	str	w0, [x29, 40]
	ldr	x0, [x29, 32]
	ldr	x0, [x0, 272]
	ldr	w1, [x0, 12]
	ldr	x0, [x29, 32]
	ldr	w2, [x0, 28]
	ldr	x0, [x29, 24]
	ldr	w3, [x0, 4]
	ldr	x0, [x29, 24]
	ldr	w4, [x0, 8]
	ldr	x0, [x29, 32]
	ldr	w5, [x0, 184]
	adrp	x0, .LC47
	add	x0, x0, :lo12:.LC47
	bl	nand_print_dbg
	b	.L530
.L531:
	ldr	w0, [x29, 40]
	cmp	w0, 10
	beq	.L532
	ldr	w0, [x29, 40]
	cmp	w0, 0
	bne	.L530
.L532:
	ldr	w0, [x29, 40]
	cmp	w0, 0
	bne	.L533
	ldr	x0, [x29, 32]
	ldr	x0, [x0, 272]
	ldr	w1, [x0, 12]
	ldr	x0, [x29, 32]
	ldr	w2, [x0, 28]
	ldr	x0, [x29, 24]
	ldr	w3, [x0, 4]
	ldr	x0, [x29, 24]
	ldr	w4, [x0, 8]
	ldr	x0, [x29, 32]
	ldr	w5, [x0, 184]
	adrp	x0, .LC96
	add	x0, x0, :lo12:.LC96
	bl	nand_print_dbg
	b	.L534
.L533:
	ldr	x0, [x29, 32]
	ldr	x0, [x0, 272]
	ldr	w1, [x0, 12]
	ldr	x0, [x29, 32]
	ldr	w2, [x0, 28]
	ldr	x0, [x29, 24]
	ldr	w3, [x0, 4]
	ldr	x0, [x29, 24]
	ldr	w4, [x0, 8]
	ldr	x0, [x29, 32]
	ldr	w5, [x0, 184]
	adrp	x0, .LC97
	add	x0, x0, :lo12:.LC97
	bl	nand_print_dbg
.L534:
	mov	w0, 10
	str	w0, [x29, 40]
	b	.L524
.L530:
	ldr	w0, [x29, 44]
	add	w0, w0, 1
	str	w0, [x29, 44]
.L528:
	adrp	x0, sandisk_A19_read_retry_cycle
	add	x0, x0, :lo12:sandisk_A19_read_retry_cycle
	ldrb	w0, [x0]
	mov	w1, w0
	ldr	w0, [x29, 44]
	cmp	w1, w0
	bgt	.L535
	str	wzr, [x29, 44]
	b	.L536
.L543:
	ldr	w1, [x29, 44]
	ldr	x0, [x29, 32]
	str	w1, [x0, 184]
	ldr	x0, [x29, 24]
	ldr	w0, [x0, 8]
	uxth	w0, w0
	mov	w1, w0
	ldr	x0, [x29, 32]
	bl	sandisk_A19_set_readretry
	str	w0, [x29, 40]
	ldr	w0, [x29, 40]
	cmp	w0, 0
	beq	.L537
	adrp	x0, .LC95
	add	x0, x0, :lo12:.LC95
	bl	nand_print_dbg
	b	.L538
.L537:
	ldr	x0, [x29, 24]
	bl	sandisk_A19_cmd25
	ldr	x0, [x29, 24]
	bl	generic_read_page_start
	str	w0, [x29, 40]
	ldr	x0, [x29, 24]
	bl	generic_read_page_end_not_retry
	mov	w1, w0
	ldr	w0, [x29, 40]
	orr	w0, w0, w1
	str	w0, [x29, 40]
	ldr	x0, [x29, 24]
	ldr	x0, [x0, 24]
	bl	is_nouse_page
	cmp	w0, 1
	bne	.L539
	mov	w0, -2
	str	w0, [x29, 40]
	ldr	x0, [x29, 32]
	ldr	x0, [x0, 272]
	ldr	w1, [x0, 12]
	ldr	x0, [x29, 32]
	ldr	w2, [x0, 28]
	ldr	x0, [x29, 24]
	ldr	w3, [x0, 4]
	ldr	x0, [x29, 24]
	ldr	w4, [x0, 8]
	ldr	x0, [x29, 32]
	ldr	w5, [x0, 184]
	adrp	x0, .LC47
	add	x0, x0, :lo12:.LC47
	bl	nand_print_dbg
	b	.L538
.L539:
	ldr	w0, [x29, 40]
	cmp	w0, 10
	beq	.L540
	ldr	w0, [x29, 40]
	cmp	w0, 0
	bne	.L538
.L540:
	ldr	w0, [x29, 40]
	cmp	w0, 0
	bne	.L541
	ldr	x0, [x29, 32]
	ldr	x0, [x0, 272]
	ldr	w1, [x0, 12]
	ldr	x0, [x29, 32]
	ldr	w2, [x0, 28]
	ldr	x0, [x29, 24]
	ldr	w3, [x0, 4]
	ldr	x0, [x29, 24]
	ldr	w4, [x0, 8]
	ldr	x0, [x29, 32]
	ldr	w5, [x0, 184]
	adrp	x0, .LC96
	add	x0, x0, :lo12:.LC96
	bl	nand_print_dbg
	b	.L542
.L541:
	ldr	x0, [x29, 32]
	ldr	x0, [x0, 272]
	ldr	w1, [x0, 12]
	ldr	x0, [x29, 32]
	ldr	w2, [x0, 28]
	ldr	x0, [x29, 24]
	ldr	w3, [x0, 4]
	ldr	x0, [x29, 24]
	ldr	w4, [x0, 8]
	ldr	x0, [x29, 32]
	ldr	w5, [x0, 184]
	adrp	x0, .LC97
	add	x0, x0, :lo12:.LC97
	bl	nand_print_dbg
.L542:
	mov	w0, 10
	str	w0, [x29, 40]
	b	.L524
.L538:
	ldr	w0, [x29, 44]
	add	w0, w0, 1
	str	w0, [x29, 44]
.L536:
	adrp	x0, sandisk_A19_read_retry_cycle
	add	x0, x0, :lo12:sandisk_A19_read_retry_cycle
	ldrb	w0, [x0]
	mov	w1, w0
	ldr	w0, [x29, 44]
	cmp	w1, w0
	bgt	.L543
	b	.L527
.L561:
	nop
.L527:
	str	wzr, [x29, 44]
	b	.L544
.L559:
	ldr	w1, [x29, 44]
	ldr	x0, [x29, 32]
	str	w1, [x0, 184]
	ldr	x0, [x29, 24]
	ldr	w0, [x0, 8]
	uxth	w0, w0
	mov	w1, w0
	ldr	x0, [x29, 32]
	bl	sandisk_A19_set_readretry
	str	w0, [x29, 40]
	ldr	w0, [x29, 40]
	cmp	w0, 0
	beq	.L545
	adrp	x0, .LC95
	add	x0, x0, :lo12:.LC95
	bl	nand_print_dbg
	b	.L546
.L545:
	adrp	x0, sandisk_A19_read_retry_mode
	add	x0, x0, :lo12:sandisk_A19_read_retry_mode
	ldrb	w0, [x0]
	cmp	w0, 52
	bne	.L547
	mov	w1, -58
	ldr	x0, [x29, 24]
	bl	sandisk_A19_set_lmflgfix_next
	b	.L548
.L547:
	adrp	x0, sandisk_A19_read_retry_mode
	add	x0, x0, :lo12:sandisk_A19_read_retry_mode
	ldrb	w0, [x0]
	cmp	w0, 53
	bne	.L549
	mov	w1, 26
	ldr	x0, [x29, 24]
	bl	sandisk_A19_set_lmflgfix_next
	b	.L548
.L549:
	mov	w1, -64
	ldr	x0, [x29, 24]
	bl	sandisk_A19_set_lmflgfix_next
.L548:
	ldr	x0, [x29, 24]
	bl	sandisk_A19_cmd25
	adrp	x0, sandisk_A19_read_retry_mode
	add	x0, x0, :lo12:sandisk_A19_read_retry_mode
	ldrb	w0, [x0]
	cmp	w0, 50
	beq	.L550
	adrp	x0, sandisk_A19_read_retry_mode
	add	x0, x0, :lo12:sandisk_A19_read_retry_mode
	ldrb	w0, [x0]
	cmp	w0, 52
	beq	.L550
	adrp	x0, sandisk_A19_read_retry_mode
	add	x0, x0, :lo12:sandisk_A19_read_retry_mode
	ldrb	w0, [x0]
	cmp	w0, 53
	bne	.L551
.L550:
	ldr	x0, [x29, 24]
	bl	sandisk_A19_dsp_on
.L551:
	ldr	x0, [x29, 24]
	bl	generic_read_page_start
	str	w0, [x29, 40]
	ldr	x0, [x29, 24]
	bl	generic_read_page_end_not_retry
	mov	w1, w0
	ldr	w0, [x29, 40]
	orr	w0, w0, w1
	str	w0, [x29, 40]
	adrp	x0, sandisk_A19_read_retry_mode
	add	x0, x0, :lo12:sandisk_A19_read_retry_mode
	ldrb	w0, [x0]
	cmp	w0, 52
	bne	.L552
	mov	w1, -62
	ldr	x0, [x29, 24]
	bl	sandisk_A19_set_lmflgfix_next
	b	.L553
.L552:
	adrp	x0, sandisk_A19_read_retry_mode
	add	x0, x0, :lo12:sandisk_A19_read_retry_mode
	ldrb	w0, [x0]
	cmp	w0, 53
	bne	.L554
	mov	w1, 10
	ldr	x0, [x29, 24]
	bl	sandisk_A19_set_lmflgfix_next
	b	.L553
.L554:
	mov	w1, 64
	ldr	x0, [x29, 24]
	bl	sandisk_A19_set_lmflgfix_next
.L553:
	ldr	x0, [x29, 24]
	ldr	x0, [x0, 24]
	bl	is_nouse_page
	cmp	w0, 1
	bne	.L555
	mov	w0, -2
	str	w0, [x29, 40]
	ldr	x0, [x29, 32]
	ldr	x0, [x0, 272]
	ldr	w1, [x0, 12]
	ldr	x0, [x29, 32]
	ldr	w2, [x0, 28]
	ldr	x0, [x29, 24]
	ldr	w3, [x0, 4]
	ldr	x0, [x29, 24]
	ldr	w4, [x0, 8]
	ldr	x0, [x29, 32]
	ldr	w5, [x0, 184]
	adrp	x0, .LC47
	add	x0, x0, :lo12:.LC47
	bl	nand_print_dbg
	b	.L546
.L555:
	ldr	w0, [x29, 40]
	cmp	w0, 10
	beq	.L556
	ldr	w0, [x29, 40]
	cmp	w0, 0
	bne	.L546
.L556:
	ldr	w0, [x29, 40]
	cmp	w0, 0
	bne	.L557
	ldr	x0, [x29, 32]
	ldr	x0, [x0, 272]
	ldr	w1, [x0, 12]
	ldr	x0, [x29, 32]
	ldr	w2, [x0, 28]
	ldr	x0, [x29, 24]
	ldr	w3, [x0, 4]
	ldr	x0, [x29, 24]
	ldr	w4, [x0, 8]
	ldr	x0, [x29, 32]
	ldr	w5, [x0, 184]
	adrp	x0, .LC96
	add	x0, x0, :lo12:.LC96
	bl	nand_print_dbg
	b	.L558
.L557:
	ldr	x0, [x29, 32]
	ldr	x0, [x0, 272]
	ldr	w1, [x0, 12]
	ldr	x0, [x29, 32]
	ldr	w2, [x0, 28]
	ldr	x0, [x29, 24]
	ldr	w3, [x0, 4]
	ldr	x0, [x29, 24]
	ldr	w4, [x0, 8]
	ldr	x0, [x29, 32]
	ldr	w5, [x0, 184]
	adrp	x0, .LC97
	add	x0, x0, :lo12:.LC97
	bl	nand_print_dbg
.L558:
	mov	w0, 10
	str	w0, [x29, 40]
	b	.L524
.L546:
	ldr	w0, [x29, 44]
	add	w0, w0, 1
	str	w0, [x29, 44]
.L544:
	adrp	x0, sandisk_A19_read_retry_cycle
	add	x0, x0, :lo12:sandisk_A19_read_retry_cycle
	ldrb	w0, [x0]
	mov	w1, w0
	ldr	w0, [x29, 44]
	cmp	w1, w0
	bgt	.L559
.L524:
	ldr	x0, [x29, 32]
	str	wzr, [x0, 184]
	ldr	x0, [x29, 32]
	bl	sandisk_A19_exit_readretry
.L516:
	ldr	w0, [x29, 40]
	ldp	x29, x30, [sp], 48
	ret
	.size	sandisk_A19_read_page_end, .-sandisk_A19_read_page_end
	.section	.rodata
	.align	3
.LC98:
	.string	"sandisk_A19 retry mode err : %d !\n"
	.align	3
.LC99:
	.string	"sandisk A19 read retry conut: %d !\n"
	.text
	.align	2
	.global	sandisk_A19_readretry_init
	.type	sandisk_A19_readretry_init, %function
sandisk_A19_readretry_init:
	stp	x29, x30, [sp, -32]!
	add	x29, sp, 0
	str	x0, [x29, 24]
	ldr	x0, [x29, 24]
	str	wzr, [x0, 184]
	ldr	x0, [x29, 24]
	ldr	x0, [x0, 280]
	ldr	w0, [x0, 52]
	lsr	w0, w0, 16
	uxtb	w1, w0
	adrp	x0, sandisk_A19_read_retry_mode
	add	x0, x0, :lo12:sandisk_A19_read_retry_mode
	strb	w1, [x0]
	ldr	x0, [x29, 24]
	ldr	x0, [x0, 280]
	ldr	w0, [x0, 52]
	lsr	w0, w0, 8
	uxtb	w1, w0
	adrp	x0, sandisk_A19_read_retry_cycle
	add	x0, x0, :lo12:sandisk_A19_read_retry_cycle
	strb	w1, [x0]
	ldr	x0, [x29, 24]
	ldr	x0, [x0, 280]
	ldr	w0, [x0, 52]
	uxtb	w1, w0
	adrp	x0, sandisk_A19_read_retry_reg_cnt
	add	x0, x0, :lo12:sandisk_A19_read_retry_reg_cnt
	strb	w1, [x0]
	adrp	x0, sandisk_A19_read_retry_mode
	add	x0, x0, :lo12:sandisk_A19_read_retry_mode
	ldrb	w0, [x0]
	cmp	w0, 50
	beq	.L563
	adrp	x0, sandisk_A19_read_retry_mode
	add	x0, x0, :lo12:sandisk_A19_read_retry_mode
	ldrb	w0, [x0]
	cmp	w0, 51
	beq	.L563
	adrp	x0, sandisk_A19_read_retry_mode
	add	x0, x0, :lo12:sandisk_A19_read_retry_mode
	ldrb	w0, [x0]
	cmp	w0, 52
	beq	.L563
	adrp	x0, sandisk_A19_read_retry_mode
	add	x0, x0, :lo12:sandisk_A19_read_retry_mode
	ldrb	w0, [x0]
	cmp	w0, 53
	beq	.L563
	adrp	x0, sandisk_A19_read_retry_mode
	add	x0, x0, :lo12:sandisk_A19_read_retry_mode
	ldrb	w0, [x0]
	mov	w1, w0
	adrp	x0, .LC98
	add	x0, x0, :lo12:.LC98
	bl	nand_print
	mov	w0, -99
	b	.L564
.L563:
	adrp	x0, sandisk_A19_read_retry_cycle
	add	x0, x0, :lo12:sandisk_A19_read_retry_cycle
	ldrb	w0, [x0]
	mov	w1, w0
	adrp	x0, .LC99
	add	x0, x0, :lo12:.LC99
	bl	nand_print_dbg
	mov	w0, 0
.L564:
	ldp	x29, x30, [sp], 32
	ret
	.size	sandisk_A19_readretry_init, .-sandisk_A19_readretry_init
	.align	2
	.global	sandisk_A19_readretry_exit
	.type	sandisk_A19_readretry_exit, %function
sandisk_A19_readretry_exit:
	sub	sp, sp, #16
	str	x0, [sp, 8]
	mov	w0, 0
	add	sp, sp, 16
	ret
	.size	sandisk_A19_readretry_exit, .-sandisk_A19_readretry_exit
	.align	2
	.global	sandisk_A19_read_retry_clock_save
	.type	sandisk_A19_read_retry_clock_save, %function
sandisk_A19_read_retry_clock_save:
	stp	x29, x30, [sp, -32]!
	add	x29, sp, 0
	str	x0, [x29, 24]
	ldr	x0, [x29, 24]
	ldr	x0, [x0, 272]
	ldr	w1, [x0, 12]
	adrp	x0, sandisk_A19_sclk1_bak
	add	x3, x0, :lo12:sandisk_A19_sclk1_bak
	adrp	x0, sandisk_A19_sclk0_bak
	add	x2, x0, :lo12:sandisk_A19_sclk0_bak
	adrp	x0, aw_ndfc
	add	x0, x0, :lo12:aw_ndfc
	bl	nand_get_clk
	ldr	x0, [x29, 24]
	ldr	x0, [x0, 272]
	ldr	w1, [x0, 12]
	adrp	x0, aw_ndfc
	add	x0, x0, :lo12:aw_ndfc
	mov	w3, 20
	mov	w2, 10
	bl	nand_set_clk
	mov	w0, 0
	ldp	x29, x30, [sp], 32
	ret
	.size	sandisk_A19_read_retry_clock_save, .-sandisk_A19_read_retry_clock_save
	.align	2
	.global	sandisk_A19_read_retry_clock_recover
	.type	sandisk_A19_read_retry_clock_recover, %function
sandisk_A19_read_retry_clock_recover:
	stp	x29, x30, [sp, -32]!
	add	x29, sp, 0
	str	x0, [x29, 24]
	ldr	x0, [x29, 24]
	ldr	x0, [x0, 272]
	ldr	w1, [x0, 12]
	adrp	x0, sandisk_A19_sclk0_bak
	add	x0, x0, :lo12:sandisk_A19_sclk0_bak
	ldr	w2, [x0]
	adrp	x0, sandisk_A19_sclk1_bak
	add	x0, x0, :lo12:sandisk_A19_sclk1_bak
	ldr	w3, [x0]
	adrp	x0, aw_ndfc
	add	x0, x0, :lo12:aw_ndfc
	bl	nand_set_clk
	mov	w0, 0
	ldp	x29, x30, [sp], 32
	ret
	.size	sandisk_A19_read_retry_clock_recover, .-sandisk_A19_read_retry_clock_recover
	.align	2
	.global	sandisk_A19_vender_set_param
	.type	sandisk_A19_vender_set_param, %function
sandisk_A19_vender_set_param:
	stp	x29, x30, [sp, -64]!
	add	x29, sp, 0
	str	x0, [x29, 40]
	str	x1, [x29, 32]
	str	x2, [x29, 24]
	str	w3, [x29, 20]
	str	wzr, [x29, 60]
	adrp	x0, sandisk_A19_read_retry_cmd_1y
	add	x0, x0, :lo12:sandisk_A19_read_retry_cmd_1y
	mov	w6, 1
	mov	w5, 4
	ldr	x4, [x29, 32]
	ldr	x3, [x29, 24]
	mov	w2, 1
	mov	x1, x0
	ldr	x0, [x29, 40]
	bl	set_cmd_with_nand_bus
	str	w0, [x29, 60]
	ldr	w0, [x29, 60]
	ldp	x29, x30, [sp], 64
	ret
	.size	sandisk_A19_vender_set_param, .-sandisk_A19_vender_set_param
	.align	2
	.global	sandisk_A19_set_readretry
	.type	sandisk_A19_set_readretry, %function
sandisk_A19_set_readretry:
	stp	x29, x30, [sp, -64]!
	add	x29, sp, 0
	str	x0, [x29, 24]
	strh	w1, [x29, 22]
	str	wzr, [x29, 52]
	ldr	x0, [x29, 24]
	bl	nand_enable_chip
	ldr	x0, [x29, 24]
	ldr	x0, [x0, 272]
	bl	ndfc_disable_randomize
	adrp	x0, sandisk_A19_read_retry_mode
	add	x0, x0, :lo12:sandisk_A19_read_retry_mode
	ldrb	w0, [x0]
	cmp	w0, 52
	bne	.L574
	ldr	x0, [x29, 24]
	ldr	w0, [x0, 184]
	uxtw	x0, w0
	lsl	x1, x0, 2
	adrp	x0, sandisk_A19_1z_16g
	add	x0, x0, :lo12:sandisk_A19_1z_16g
	add	x0, x1, x0
	str	x0, [x29, 56]
	b	.L575
.L574:
	adrp	x0, sandisk_A19_read_retry_mode
	add	x0, x0, :lo12:sandisk_A19_read_retry_mode
	ldrb	w0, [x0]
	cmp	w0, 53
	bne	.L576
	ldr	x0, [x29, 24]
	ldr	w0, [x0, 184]
	uxtw	x0, w0
	lsl	x1, x0, 2
	adrp	x0, sandisk_A19_1z_8g
	add	x0, x0, :lo12:sandisk_A19_1z_8g
	add	x0, x1, x0
	str	x0, [x29, 56]
	b	.L575
.L576:
	ldr	x0, [x29, 24]
	ldr	w0, [x0, 184]
	uxtw	x0, w0
	lsl	x1, x0, 2
	adrp	x0, sandisk_A19_1y
	add	x0, x0, :lo12:sandisk_A19_1y
	add	x0, x1, x0
	str	x0, [x29, 56]
.L575:
	adrp	x0, sandisk_A19_read_retry_reg_adr_1y
	add	x0, x0, :lo12:sandisk_A19_read_retry_reg_adr_1y
	str	x0, [x29, 40]
	mov	w0, 1
	str	w0, [x29, 36]
	ldr	w3, [x29, 36]
	ldr	x2, [x29, 40]
	ldr	x1, [x29, 56]
	ldr	x0, [x29, 24]
	bl	sandisk_A19_vender_set_param
	str	w0, [x29, 52]
	mov	w2, 0
	mov	w1, 93
	ldr	x0, [x29, 24]
	bl	set_one_cmd
	ldr	x0, [x29, 24]
	bl	nand_disable_chip
	ldr	w0, [x29, 52]
	ldp	x29, x30, [sp], 64
	ret
	.size	sandisk_A19_set_readretry, .-sandisk_A19_set_readretry
	.align	2
	.global	sandisk_A19_exit_readretry
	.type	sandisk_A19_exit_readretry, %function
sandisk_A19_exit_readretry:
	stp	x29, x30, [sp, -64]!
	add	x29, sp, 0
	str	x0, [x29, 24]
	str	wzr, [x29, 60]
	strb	wzr, [x29, 40]
	strb	wzr, [x29, 41]
	strb	wzr, [x29, 42]
	strb	wzr, [x29, 43]
	ldr	x0, [x29, 24]
	bl	nand_enable_chip
	ldr	x0, [x29, 24]
	ldr	x0, [x0, 272]
	bl	ndfc_disable_randomize
	mov	w2, 1
	mov	w1, -1
	ldr	x0, [x29, 24]
	bl	set_one_cmd
	adrp	x0, sandisk_A19_read_retry_reg_adr_1y
	add	x0, x0, :lo12:sandisk_A19_read_retry_reg_adr_1y
	str	x0, [x29, 48]
	mov	w0, 1
	str	w0, [x29, 44]
	add	x0, x29, 40
	ldr	w3, [x29, 44]
	ldr	x2, [x29, 48]
	mov	x1, x0
	ldr	x0, [x29, 24]
	bl	sandisk_A19_vender_set_param
	str	w0, [x29, 60]
	ldr	x0, [x29, 24]
	bl	nand_disable_chip
	ldr	w0, [x29, 60]
	ldp	x29, x30, [sp], 64
	ret
	.size	sandisk_A19_exit_readretry, .-sandisk_A19_exit_readretry
	.section	.rodata
	.align	3
.LC100:
	.string	" sandisk_A19_special_init sandisk_A19_read_retry_mode:%d sandisk_A19_read_retry_cycle :%d sandisk_A19_read_retry_reg_cnt %d \n"
	.align	3
.LC101:
	.string	" sandisk_A19_special_init error sandisk_A19_read_retry_mode:%d sandisk_A19_read_retry_cycle :%d sandisk_A19_read_retry_reg_cnt %d \n"
	.text
	.align	2
	.global	sandisk_A19_special_init
	.type	sandisk_A19_special_init, %function
sandisk_A19_special_init:
	stp	x29, x30, [sp, -32]!
	add	x29, sp, 0
	str	wzr, [x29, 28]
	adrp	x0, g_nsi
	add	x0, x0, :lo12:g_nsi
	ldr	x0, [x0]
	ldr	x0, [x0, 8]
	str	x0, [x29, 16]
	b	.L581
.L582:
	ldr	x0, [x29, 16]
	bl	sandisk_A19_readretry_init
	mov	w1, w0
	ldr	w0, [x29, 28]
	orr	w0, w0, w1
	str	w0, [x29, 28]
	ldr	x0, [x29, 16]
	ldr	x0, [x0]
	str	x0, [x29, 16]
.L581:
	ldr	x0, [x29, 16]
	cmp	x0, 0
	bne	.L582
	ldr	w0, [x29, 28]
	cmp	w0, 0
	bne	.L583
	adrp	x0, df_read_page_end
	add	x0, x0, :lo12:df_read_page_end
	adrp	x1, sandisk_A19_read_page_end
	add	x1, x1, :lo12:sandisk_A19_read_page_end
	str	x1, [x0]
	adrp	x0, sandisk_A19_read_retry_mode
	add	x0, x0, :lo12:sandisk_A19_read_retry_mode
	ldrb	w0, [x0]
	mov	w1, w0
	adrp	x0, sandisk_A19_read_retry_cycle
	add	x0, x0, :lo12:sandisk_A19_read_retry_cycle
	ldrb	w0, [x0]
	mov	w2, w0
	adrp	x0, sandisk_A19_read_retry_reg_cnt
	add	x0, x0, :lo12:sandisk_A19_read_retry_reg_cnt
	ldrb	w0, [x0]
	mov	w3, w0
	adrp	x0, .LC100
	add	x0, x0, :lo12:.LC100
	bl	nand_print_dbg
	b	.L584
.L583:
	adrp	x0, sandisk_A19_read_retry_mode
	add	x0, x0, :lo12:sandisk_A19_read_retry_mode
	ldrb	w0, [x0]
	mov	w1, w0
	adrp	x0, sandisk_A19_read_retry_cycle
	add	x0, x0, :lo12:sandisk_A19_read_retry_cycle
	ldrb	w0, [x0]
	mov	w2, w0
	adrp	x0, sandisk_A19_read_retry_reg_cnt
	add	x0, x0, :lo12:sandisk_A19_read_retry_reg_cnt
	ldrb	w0, [x0]
	mov	w3, w0
	adrp	x0, .LC101
	add	x0, x0, :lo12:.LC101
	bl	nand_print
.L584:
	ldr	w0, [x29, 28]
	ldp	x29, x30, [sp], 32
	ret
	.size	sandisk_A19_special_init, .-sandisk_A19_special_init
	.section	.rodata
	.align	3
.LC102:
	.string	"%s sandisk toggle vendor specific setting failed!\n"
	.align	3
.LC103:
	.string	" sandisk_A19_special_exit \n"
	.text
	.align	2
	.global	sandisk_A19_special_exit
	.type	sandisk_A19_special_exit, %function
sandisk_A19_special_exit:
	stp	x29, x30, [sp, -32]!
	add	x29, sp, 0
	adrp	x0, g_nsi
	add	x0, x0, :lo12:g_nsi
	ldr	x0, [x0]
	ldr	x0, [x0, 8]
	str	x0, [x29, 24]
	b	.L587
.L590:
	ldr	x0, [x29, 24]
	bl	sandisk_A19_readretry_exit
	ldr	x0, [x29, 24]
	ldrb	w0, [x0, 262]
	cmp	w0, 0
	beq	.L588
	ldr	x0, [x29, 24]
	str	wzr, [x0, 204]
	ldr	x0, [x29, 24]
	bl	sandisk_toggle_vendor_specific_setting
	str	w0, [x29, 20]
	ldr	w0, [x29, 20]
	cmp	w0, 0
	beq	.L588
	adrp	x0, __func__.6409
	add	x1, x0, :lo12:__func__.6409
	adrp	x0, .LC102
	add	x0, x0, :lo12:.LC102
	bl	nand_print
	ldr	w0, [x29, 20]
	b	.L589
.L588:
	ldr	x0, [x29, 24]
	ldr	x0, [x0]
	str	x0, [x29, 24]
.L587:
	ldr	x0, [x29, 24]
	cmp	x0, 0
	bne	.L590
	adrp	x0, .LC103
	add	x0, x0, :lo12:.LC103
	bl	nand_print_dbg
	mov	w0, 0
.L589:
	ldp	x29, x30, [sp], 32
	ret
	.size	sandisk_A19_special_exit, .-sandisk_A19_special_exit
	.section	.rodata
	.align	3
.LC104:
	.string	"fatal err -0, wrong input parameter, ch: %d  chip: %d/%d  block: %d/%d \n"
	.text
	.align	2
	.global	sandisk_A19_check_bad_block_first_burn
	.type	sandisk_A19_check_bad_block_first_burn, %function
sandisk_A19_check_bad_block_first_burn:
	stp	x29, x30, [sp, -80]!
	add	x29, sp, 0
	str	x0, [x29, 24]
	str	wzr, [x29, 76]
	str	wzr, [x29, 72]
	adrp	x0, g_nsi
	add	x0, x0, :lo12:g_nsi
	ldr	x2, [x0]
	ldr	x0, [x29, 24]
	ldr	w0, [x0]
	mov	w1, w0
	mov	x0, x2
	bl	nci_get_from_nsi
	str	x0, [x29, 64]
	ldr	x0, [x29, 64]
	ldr	x0, [x0, 272]
	str	x0, [x29, 56]
	ldr	x0, [x29, 64]
	ldr	x0, [x0, 272]
	add	x0, x0, 40
	str	x0, [x29, 48]
	ldr	x0, [x29, 64]
	ldr	w1, [x0, 28]
	ldr	x0, [x29, 56]
	ldr	w0, [x0, 16]
	cmp	w1, w0
	bcs	.L592
	ldr	x0, [x29, 24]
	ldr	w1, [x0, 4]
	ldr	x0, [x29, 64]
	ldr	w0, [x0, 32]
	cmp	w1, w0
	bcc	.L593
.L592:
	ldr	x0, [x29, 56]
	ldr	w1, [x0, 12]
	ldr	x0, [x29, 64]
	ldr	w2, [x0, 28]
	ldr	x0, [x29, 56]
	ldr	w3, [x0, 16]
	ldr	x0, [x29, 24]
	ldr	w4, [x0, 4]
	ldr	x0, [x29, 64]
	ldr	w5, [x0, 32]
	adrp	x0, .LC104
	add	x0, x0, :lo12:.LC104
	bl	nand_print
	mov	w0, -10
	b	.L594
.L593:
	ldr	x0, [x29, 64]
	ldr	x0, [x0, 272]
	bl	ndfc_disable_randomize
	ldr	x0, [x29, 64]
	bl	nand_read_chip_status_ready
	ldr	x0, [x29, 64]
	bl	nand_enable_chip
	ldr	x0, [x29, 48]
	bl	ndfc_clean_cmd_seq
	ldr	x0, [x29, 48]
	str	wzr, [x0]
	ldr	x0, [x29, 48]
	mov	w1, 162
	str	w1, [x0, 12]
	ldr	x0, [x29, 48]
	mov	w1, 1
	str	w1, [x0, 8]
	ldr	x0, [x29, 48]
	mov	w1, 1
	str	w1, [x0, 16]
	ldr	x0, [x29, 48]
	str	wzr, [x0, 20]
	ldr	x0, [x29, 48]
	mov	w1, 128
	str	w1, [x0, 84]
	ldr	x0, [x29, 48]
	mov	w1, 1
	str	w1, [x0, 80]
	ldr	x0, [x29, 48]
	mov	w1, 1
	str	w1, [x0, 88]
	ldr	x0, [x29, 48]
	str	wzr, [x0, 92]
	ldr	x0, [x29, 64]
	ldr	w3, [x0, 44]
	ldr	x0, [x29, 24]
	ldr	w1, [x0, 4]
	ldr	x0, [x29, 24]
	ldr	w0, [x0, 8]
	mov	w2, w0
	mov	w0, w3
	bl	get_row_addr
	str	w0, [x29, 76]
	ldr	x0, [x29, 48]
	mov	w1, 5
	str	w1, [x0, 112]
	ldr	x0, [x29, 48]
	add	x0, x0, 96
	mov	x4, x0
	mov	w3, 3
	ldr	w2, [x29, 76]
	mov	w1, 2
	ldr	w0, [x29, 72]
	bl	fill_cmd_addr
	ldr	x0, [x29, 48]
	mov	w1, 16
	str	w1, [x0, 156]
	ldr	x0, [x29, 48]
	mov	w1, 1
	str	w1, [x0, 152]
	ldr	x0, [x29, 48]
	mov	w1, 1
	str	w1, [x0, 160]
	ldr	x0, [x29, 48]
	mov	w1, 1
	str	w1, [x0, 164]
	ldr	x0, [x29, 64]
	ldr	x0, [x0, 272]
	ldr	x1, [x29, 48]
	bl	ndfc_execute_cmd
	str	w0, [x29, 44]
	ldr	x0, [x29, 64]
	bl	nand_read_chip_status_ready
	str	w0, [x29, 44]
	ldr	x0, [x29, 64]
	bl	nand_disable_chip
	ldr	w0, [x29, 44]
.L594:
	ldp	x29, x30, [sp], 80
	ret
	.size	sandisk_A19_check_bad_block_first_burn, .-sandisk_A19_check_bad_block_first_burn
	.section	.rodata
	.align	3
.LC105:
	.string	" generic special init \n"
	.text
	.align	2
	.global	generic_special_init
	.type	generic_special_init, %function
generic_special_init:
	stp	x29, x30, [sp, -16]!
	add	x29, sp, 0
	adrp	x0, df_read_page_end
	add	x0, x0, :lo12:df_read_page_end
	adrp	x1, generic_read_page_end_not_retry
	add	x1, x1, :lo12:generic_read_page_end_not_retry
	str	x1, [x0]
	adrp	x0, .LC105
	add	x0, x0, :lo12:.LC105
	bl	nand_print_dbg
	mov	w0, 0
	ldp	x29, x30, [sp], 16
	ret
	.size	generic_special_init, .-generic_special_init
	.section	.rodata
	.align	3
.LC106:
	.string	" generic special exit \n"
	.text
	.align	2
	.global	generic_special_exit
	.type	generic_special_exit, %function
generic_special_exit:
	stp	x29, x30, [sp, -16]!
	add	x29, sp, 0
	adrp	x0, .LC106
	add	x0, x0, :lo12:.LC106
	bl	nand_print_dbg
	mov	w0, 0
	ldp	x29, x30, [sp], 16
	ret
	.size	generic_special_exit, .-generic_special_exit
	.global	hynix16nm_read_retry_mode
	.bss
	.type	hynix16nm_read_retry_mode, %object
	.size	hynix16nm_read_retry_mode, 1
hynix16nm_read_retry_mode:
	.zero	1
	.global	hynix16nm_read_retry_cycle
	.type	hynix16nm_read_retry_cycle, %object
	.size	hynix16nm_read_retry_cycle, 1
hynix16nm_read_retry_cycle:
	.zero	1
	.global	hynix16nm_read_retry_reg_num
	.type	hynix16nm_read_retry_reg_num, %object
	.size	hynix16nm_read_retry_reg_num, 1
hynix16nm_read_retry_reg_num:
	.zero	1
	.global	hynix16nm_read_retry_reg_adr
	.align	3
	.type	hynix16nm_read_retry_reg_adr, %object
	.size	hynix16nm_read_retry_reg_adr, 4
hynix16nm_read_retry_reg_adr:
	.zero	4
	.text
	.align	2
	.global	hynix16nm_vender_set_param
	.type	hynix16nm_vender_set_param, %function
hynix16nm_vender_set_param:
	stp	x29, x30, [sp, -64]!
	add	x29, sp, 0
	str	x0, [x29, 40]
	str	x1, [x29, 32]
	str	x2, [x29, 24]
	str	w3, [x29, 20]
	str	wzr, [x29, 56]
	mov	w0, 54
	strb	w0, [x29, 54]
	mov	w0, 22
	strb	w0, [x29, 55]
	ldr	x0, [x29, 40]
	bl	nand_enable_chip
	ldr	x0, [x29, 40]
	ldr	x0, [x0, 272]
	bl	ndfc_disable_randomize
	str	wzr, [x29, 60]
	b	.L600
.L601:
	ldr	w0, [x29, 60]
	ldr	x1, [x29, 24]
	add	x2, x1, x0
	ldr	w0, [x29, 60]
	ldr	x1, [x29, 32]
	add	x1, x1, x0
	add	x0, x29, 54
	mov	w6, 1
	mov	w5, 1
	mov	x4, x1
	mov	x3, x2
	mov	w2, 0
	mov	x1, x0
	ldr	x0, [x29, 40]
	bl	set_cmd_with_nand_bus
	mov	w2, 0
	ldrb	w1, [x29, 55]
	ldr	x0, [x29, 40]
	bl	set_one_cmd
	ldr	w0, [x29, 60]
	add	w0, w0, 1
	str	w0, [x29, 60]
.L600:
	ldr	w1, [x29, 60]
	ldr	w0, [x29, 20]
	cmp	w1, w0
	bcc	.L601
	ldr	x0, [x29, 40]
	bl	nand_disable_chip
	ldr	x0, [x29, 32]
	ldrb	w0, [x0]
	mov	w1, w0
	ldr	x0, [x29, 32]
	add	x0, x0, 1
	ldrb	w0, [x0]
	mov	w2, w0
	ldr	x0, [x29, 32]
	add	x0, x0, 2
	ldrb	w0, [x0]
	mov	w3, w0
	ldr	x0, [x29, 32]
	add	x0, x0, 3
	ldrb	w0, [x0]
	mov	w4, w0
	adrp	x0, .LC35
	add	x0, x0, :lo12:.LC35
	bl	nand_print_dbg
	ldr	w0, [x29, 56]
	ldp	x29, x30, [sp], 64
	ret
	.size	hynix16nm_vender_set_param, .-hynix16nm_vender_set_param
	.align	2
	.global	hynix16nm_set_readretry
	.type	hynix16nm_set_readretry, %function
hynix16nm_set_readretry:
	stp	x29, x30, [sp, -48]!
	add	x29, sp, 0
	str	x0, [x29, 24]
	str	wzr, [x29, 40]
	ldr	x0, [x29, 24]
	ldr	w0, [x0, 184]
	adrp	x1, hynix16nm_read_retry_cycle
	add	x1, x1, :lo12:hynix16nm_read_retry_cycle
	ldrb	w1, [x1]
	cmp	w0, w1
	bls	.L604
	mov	w0, -74
	b	.L608
.L604:
	str	wzr, [x29, 44]
	b	.L606
.L607:
	ldr	x0, [x29, 24]
	ldr	w0, [x0, 184]
	lsl	w1, w0, 2
	ldr	w0, [x29, 44]
	add	w0, w1, w0
	ldr	x1, [x29, 24]
	uxtw	x0, w0
	add	x0, x1, x0
	ldrb	w2, [x0, 56]
	ldr	w0, [x29, 44]
	sub	x1, x29, #4048
	add	x0, x1, x0
	mov	w1, w2
	strb	w1, [x0, 4080]
	ldr	w0, [x29, 44]
	add	w0, w0, 1
	str	w0, [x29, 44]
.L606:
	adrp	x0, hynix16nm_read_retry_reg_num
	add	x0, x0, :lo12:hynix16nm_read_retry_reg_num
	ldrb	w0, [x0]
	mov	w1, w0
	ldr	w0, [x29, 44]
	cmp	w1, w0
	bhi	.L607
	adrp	x0, hynix16nm_read_retry_reg_num
	add	x0, x0, :lo12:hynix16nm_read_retry_reg_num
	ldrb	w0, [x0]
	mov	w2, w0
	adrp	x0, hynix16nm_read_retry_reg_adr
	add	x1, x0, :lo12:hynix16nm_read_retry_reg_adr
	add	x0, x29, 32
	mov	w3, w2
	mov	x2, x1
	mov	x1, x0
	ldr	x0, [x29, 24]
	bl	hynix16nm_vender_set_param
	str	w0, [x29, 40]
	ldr	w0, [x29, 40]
.L608:
	ldp	x29, x30, [sp], 48
	ret
	.size	hynix16nm_set_readretry, .-hynix16nm_set_readretry
	.section	.rodata
	.align	3
.LC107:
	.string	"get_hynix_special_info start!!\n"
	.align	3
.LC108:
	.string	"hynix_special_info:can't find uboot head\n"
	.align	3
.LC109:
	.string	"physic info start_block %d\n"
	.text
	.align	2
	.global	hynix16nm_get_special_info
	.type	hynix16nm_get_special_info, %function
hynix16nm_get_special_info:
	stp	x29, x30, [sp, -96]!
	add	x29, sp, 0
	str	x0, [x29, 40]
	str	x1, [x29, 32]
	str	w2, [x29, 28]
	str	w3, [x29, 24]
	adrp	x0, .LC107
	add	x0, x0, :lo12:.LC107
	bl	nand_print_dbg
	add	x0, x29, 60
	bl	physic_info_get_offset
	str	w0, [x29, 76]
	ldr	w0, [x29, 76]
	cmp	w0, 0
	beq	.L610
	adrp	x0, .LC108
	add	x0, x0, :lo12:.LC108
	bl	nand_print
	mov	w0, 1
	b	.L620
.L610:
	mov	w0, -1
	str	w0, [x29, 88]
	mov	w0, 4
	str	w0, [x29, 84]
	b	.L612
.L619:
	adrp	x0, .LC109
	add	x0, x0, :lo12:.LC109
	ldr	w1, [x29, 84]
	bl	nand_print_dbg
	ldr	x0, [x29, 40]
	str	x0, [x29, 64]
	ldr	x0, [x29, 64]
	add	x0, x0, 8192
	ldrb	w0, [x0, 1536]
	cmp	w0, 165
	beq	.L613
	ldr	w0, [x29, 60]
	add	x1, x29, 56
	ldr	x3, [x29, 40]
	mov	x2, x1
	mov	w1, w0
	ldr	w0, [x29, 84]
	bl	physic_info_get_one_copy
.L613:
	ldr	x0, [x29, 40]
	str	x0, [x29, 64]
	ldr	x0, [x29, 64]
	add	x0, x0, 8192
	ldrb	w0, [x0, 1536]
	cmp	w0, 165
	bne	.L614
	strb	wzr, [x29, 83]
	mov	w0, 2
	str	w0, [x29, 92]
	b	.L615
.L616:
	ldr	x1, [x29, 64]
	ldr	w0, [x29, 92]
	add	x0, x1, x0
	add	x0, x0, 8192
	ldrb	w1, [x0, 1536]
	ldrb	w0, [x29, 83]
	add	w0, w1, w0
	strb	w0, [x29, 83]
	ldr	w0, [x29, 92]
	add	w0, w0, 1
	str	w0, [x29, 92]
.L615:
	ldr	w0, [x29, 92]
	cmp	w0, 1023
	bls	.L616
	ldr	x0, [x29, 64]
	add	x0, x0, 8192
	ldrb	w0, [x0, 1537]
	ldrb	w1, [x29, 83]
	cmp	w1, w0
	bne	.L617
	str	wzr, [x29, 88]
	ldr	w1, [x29, 24]
	ldr	w0, [x29, 28]
	mul	w0, w1, w0
	add	w0, w0, 2
	uxtw	x0, w0
	add	x0, x0, 8192
	add	x0, x0, 1536
	ldr	x1, [x29, 64]
	add	x0, x1, x0
	ldr	w2, [x29, 28]
	mov	x1, x0
	ldr	x0, [x29, 32]
	bl	nand_memcpy
	b	.L618
.L617:
	ldr	x0, [x29, 64]
	add	x0, x0, 8192
	strb	wzr, [x0, 1536]
.L614:
	ldr	w0, [x29, 84]
	add	w0, w0, 1
	str	w0, [x29, 84]
.L612:
	ldr	w0, [x29, 84]
	cmp	w0, 50
	bls	.L619
.L618:
	ldr	w0, [x29, 88]
.L620:
	ldp	x29, x30, [sp], 96
	ret
	.size	hynix16nm_get_special_info, .-hynix16nm_get_special_info
	.section	.rodata
	.align	3
.LC110:
	.string	"m1 retry!\n"
	.align	3
.LC111:
	.string	"m1 ReadRetry ok! ch =%d, chip = %d  block = %d, page = %d, RetryCount = %d  \n"
	.text
	.align	2
	.global	hynix16nm_read_page_end
	.type	hynix16nm_read_page_end, %function
hynix16nm_read_page_end:
	stp	x29, x30, [sp, -48]!
	add	x29, sp, 0
	str	x0, [x29, 24]
	str	wzr, [x29, 44]
	str	wzr, [x29, 40]
	adrp	x0, g_nsi
	add	x0, x0, :lo12:g_nsi
	ldr	x2, [x0]
	ldr	x0, [x29, 24]
	ldr	w0, [x0]
	mov	w1, w0
	mov	x0, x2
	bl	nci_get_from_nsi
	str	x0, [x29, 32]
	ldr	x0, [x29, 24]
	bl	generic_read_page_end_not_retry
	str	w0, [x29, 40]
	ldr	w0, [x29, 40]
	cmn	w0, #2
	bne	.L622
	adrp	x0, .LC110
	add	x0, x0, :lo12:.LC110
	bl	nand_print_dbg
	str	wzr, [x29, 44]
	b	.L623
.L629:
	str	wzr, [x29, 40]
	ldr	x0, [x29, 32]
	ldr	w0, [x0, 184]
	add	w1, w0, 1
	ldr	x0, [x29, 32]
	str	w1, [x0, 184]
	ldr	x0, [x29, 32]
	ldr	w0, [x0, 184]
	adrp	x1, hynix16nm_read_retry_cycle
	add	x1, x1, :lo12:hynix16nm_read_retry_cycle
	ldrb	w1, [x1]
	cmp	w0, w1
	bls	.L624
	ldr	x0, [x29, 32]
	str	wzr, [x0, 184]
.L624:
	ldr	x0, [x29, 32]
	bl	hynix16nm_set_readretry
	str	w0, [x29, 40]
	ldr	w0, [x29, 40]
	cmp	w0, 0
	beq	.L625
	ldr	x0, [x29, 32]
	str	wzr, [x0, 184]
	ldr	x0, [x29, 32]
	bl	hynix16nm_set_readretry
	b	.L626
.L625:
	ldr	x0, [x29, 24]
	bl	generic_read_page_start
	str	w0, [x29, 40]
	ldr	x0, [x29, 24]
	bl	generic_read_page_end_not_retry
	mov	w1, w0
	ldr	w0, [x29, 40]
	orr	w0, w0, w1
	str	w0, [x29, 40]
	ldr	w0, [x29, 40]
	cmp	w0, 10
	beq	.L627
	ldr	w0, [x29, 40]
	cmp	w0, 0
	bne	.L628
.L627:
	mov	w0, 10
	str	w0, [x29, 40]
	ldr	x0, [x29, 32]
	ldr	x0, [x0, 272]
	ldr	w1, [x0, 12]
	ldr	x0, [x29, 32]
	ldr	w2, [x0, 28]
	ldr	x0, [x29, 24]
	ldr	w3, [x0, 4]
	ldr	x0, [x29, 24]
	ldr	w4, [x0, 8]
	ldr	x0, [x29, 32]
	ldr	w5, [x0, 184]
	adrp	x0, .LC111
	add	x0, x0, :lo12:.LC111
	bl	nand_print_dbg
	b	.L626
.L628:
	ldr	w0, [x29, 44]
	add	w0, w0, 1
	str	w0, [x29, 44]
.L623:
	adrp	x0, hynix16nm_read_retry_cycle
	add	x0, x0, :lo12:hynix16nm_read_retry_cycle
	ldrb	w0, [x0]
	mov	w1, w0
	ldr	w0, [x29, 44]
	cmp	w1, w0
	bgt	.L629
.L626:
	ldr	x0, [x29, 32]
	str	wzr, [x0, 184]
	ldr	x0, [x29, 32]
	bl	hynix16nm_set_readretry
.L622:
	ldr	w0, [x29, 40]
	ldp	x29, x30, [sp], 48
	ret
	.size	hynix16nm_read_page_end, .-hynix16nm_read_page_end
	.section	.rodata
	.align	3
.LC112:
	.string	"m1 write page FF wrong\n"
	.text
	.align	2
	.global	hynix16nm_write_page_FF
	.type	hynix16nm_write_page_FF, %function
hynix16nm_write_page_FF:
	stp	x29, x30, [sp, -80]!
	add	x29, sp, 0
	str	x0, [x29, 24]
	str	w1, [x29, 20]
	str	wzr, [x29, 76]
	str	wzr, [x29, 72]
	str	wzr, [x29, 68]
	str	wzr, [x29, 56]
	str	wzr, [x29, 64]
	str	wzr, [x29, 60]
	adrp	x0, g_nsi
	add	x0, x0, :lo12:g_nsi
	ldr	x2, [x0]
	ldr	x0, [x29, 24]
	ldr	w0, [x0]
	mov	w1, w0
	mov	x0, x2
	bl	nci_get_from_nsi
	str	x0, [x29, 48]
	ldr	x0, [x29, 48]
	ldr	x0, [x0, 272]
	str	x0, [x29, 40]
	ldr	x0, [x29, 48]
	ldr	x0, [x0, 272]
	add	x0, x0, 40
	str	x0, [x29, 32]
	ldr	x0, [x29, 48]
	ldr	w1, [x0, 28]
	ldr	x0, [x29, 40]
	ldr	w0, [x0, 16]
	cmp	w1, w0
	bcs	.L632
	ldr	x0, [x29, 24]
	ldr	w1, [x0, 4]
	ldr	x0, [x29, 48]
	ldr	w0, [x0, 32]
	cmp	w1, w0
	bcc	.L633
.L632:
	ldr	x0, [x29, 40]
	ldr	w1, [x0, 12]
	ldr	x0, [x29, 48]
	ldr	w2, [x0, 28]
	ldr	x0, [x29, 40]
	ldr	w3, [x0, 16]
	ldr	x0, [x29, 24]
	ldr	w4, [x0, 4]
	ldr	x0, [x29, 48]
	ldr	w5, [x0, 32]
	adrp	x0, .LC104
	add	x0, x0, :lo12:.LC104
	bl	nand_print
	mov	w0, -108
	b	.L634
.L633:
	ldr	x0, [x29, 48]
	bl	nand_read_chip_status_ready
	ldr	x0, [x29, 48]
	bl	nand_enable_chip
	ldr	x0, [x29, 48]
	ldr	x0, [x0, 272]
	bl	ndfc_disable_randomize
	ldr	x0, [x29, 48]
	ldr	x0, [x0, 272]
	bl	ndfc_disable_ecc
	ldr	x0, [x29, 32]
	bl	ndfc_clean_cmd_seq
	ldr	w0, [x29, 20]
	cmp	w0, 8
	bne	.L635
	mov	w0, 9
	str	w0, [x29, 72]
	mov	w0, 832
	str	w0, [x29, 68]
	b	.L636
.L635:
	ldr	w0, [x29, 20]
	cmp	w0, 16
	bne	.L636
	mov	w0, 18
	str	w0, [x29, 72]
	mov	w0, 640
	str	w0, [x29, 68]
.L636:
	str	wzr, [x29, 76]
	b	.L637
.L641:
	ldr	w0, [x29, 76]
	cmp	w0, 0
	bne	.L638
	ldr	x0, [x29, 32]
	str	wzr, [x0]
	ldr	x0, [x29, 32]
	mov	w1, 128
	str	w1, [x0, 12]
	ldr	x0, [x29, 32]
	mov	w1, 1
	str	w1, [x0, 8]
	ldr	x0, [x29, 32]
	mov	w1, 1
	str	w1, [x0, 16]
	ldr	x0, [x29, 32]
	mov	w1, 1
	str	w1, [x0, 56]
	ldr	x0, [x29, 32]
	mov	w1, 1
	str	w1, [x0, 48]
	ldr	x0, [x29, 32]
	mov	w1, 1
	str	w1, [x0, 52]
	ldr	x0, [x29, 24]
	ldr	x1, [x0, 16]
	ldr	x0, [x29, 32]
	str	x1, [x0, 72]
	ldr	x0, [x29, 32]
	mov	w1, 1024
	str	w1, [x0, 60]
	ldr	x0, [x29, 32]
	mov	w1, 1
	str	w1, [x0, 44]
	ldr	x0, [x29, 48]
	ldr	w3, [x0, 44]
	ldr	x0, [x29, 24]
	ldr	w1, [x0, 4]
	ldr	x0, [x29, 24]
	ldr	w0, [x0, 8]
	mov	w2, w0
	mov	w0, w3
	bl	get_row_addr
	str	w0, [x29, 64]
	ldr	x0, [x29, 32]
	mov	w1, 5
	str	w1, [x0, 40]
	ldr	x0, [x29, 32]
	add	x0, x0, 24
	mov	x4, x0
	mov	w3, 3
	ldr	w2, [x29, 64]
	mov	w1, 2
	ldr	w0, [x29, 60]
	bl	fill_cmd_addr
	ldr	x0, [x29, 48]
	ldr	x0, [x0, 272]
	ldr	x1, [x29, 32]
	bl	ndfc_execute_cmd
	str	w0, [x29, 56]
	b	.L639
.L638:
	ldr	w0, [x29, 72]
	sub	w1, w0, #1
	ldr	w0, [x29, 76]
	cmp	w1, w0
	bne	.L640
	ldr	x0, [x29, 32]
	str	wzr, [x0]
	ldr	x0, [x29, 32]
	mov	w1, 133
	str	w1, [x0, 12]
	ldr	x0, [x29, 32]
	mov	w1, 1
	str	w1, [x0, 8]
	ldr	x0, [x29, 32]
	mov	w1, 1
	str	w1, [x0, 16]
	ldr	x0, [x29, 32]
	mov	w1, 1
	str	w1, [x0, 56]
	ldr	x0, [x29, 32]
	mov	w1, 1
	str	w1, [x0, 48]
	ldr	x0, [x29, 32]
	mov	w1, 1
	str	w1, [x0, 52]
	ldr	x0, [x29, 24]
	ldr	x1, [x0, 16]
	ldr	w0, [x29, 76]
	lsl	w0, w0, 10
	uxtw	x0, w0
	add	x1, x1, x0
	ldr	x0, [x29, 32]
	str	x1, [x0, 72]
	ldr	x0, [x29, 32]
	ldr	w1, [x29, 68]
	str	w1, [x0, 60]
	ldr	x0, [x29, 32]
	mov	w1, 1
	str	w1, [x0, 44]
	ldr	x0, [x29, 32]
	mov	w1, 2
	str	w1, [x0, 40]
	ldr	x0, [x29, 32]
	add	x0, x0, 24
	mov	x4, x0
	mov	w3, 0
	ldr	w2, [x29, 64]
	mov	w1, 2
	ldr	w0, [x29, 60]
	bl	fill_cmd_addr
	ldr	x0, [x29, 48]
	ldr	x0, [x0, 272]
	ldr	x1, [x29, 32]
	bl	ndfc_execute_cmd
	str	w0, [x29, 56]
	b	.L639
.L640:
	ldr	x0, [x29, 32]
	str	wzr, [x0]
	ldr	x0, [x29, 32]
	mov	w1, 133
	str	w1, [x0, 12]
	ldr	x0, [x29, 32]
	mov	w1, 1
	str	w1, [x0, 8]
	ldr	x0, [x29, 32]
	mov	w1, 1
	str	w1, [x0, 16]
	ldr	x0, [x29, 32]
	mov	w1, 1
	str	w1, [x0, 56]
	ldr	x0, [x29, 32]
	mov	w1, 1
	str	w1, [x0, 48]
	ldr	x0, [x29, 32]
	mov	w1, 1
	str	w1, [x0, 52]
	ldr	x0, [x29, 24]
	ldr	x1, [x0, 16]
	ldr	w0, [x29, 76]
	lsl	w0, w0, 10
	uxtw	x0, w0
	add	x1, x1, x0
	ldr	x0, [x29, 32]
	str	x1, [x0, 72]
	ldr	x0, [x29, 32]
	mov	w1, 1024
	str	w1, [x0, 60]
	ldr	x0, [x29, 32]
	mov	w1, 1
	str	w1, [x0, 44]
	ldr	x0, [x29, 32]
	mov	w1, 2
	str	w1, [x0, 40]
	ldr	x0, [x29, 32]
	add	x0, x0, 24
	mov	x4, x0
	mov	w3, 0
	ldr	w2, [x29, 64]
	mov	w1, 2
	ldr	w0, [x29, 60]
	bl	fill_cmd_addr
	ldr	x0, [x29, 48]
	ldr	x0, [x0, 272]
	ldr	x1, [x29, 32]
	bl	ndfc_execute_cmd
	str	w0, [x29, 56]
.L639:
	ldr	w0, [x29, 60]
	add	w0, w0, 1024
	str	w0, [x29, 60]
	ldr	w0, [x29, 76]
	add	w0, w0, 1
	str	w0, [x29, 76]
.L637:
	ldr	w1, [x29, 76]
	ldr	w0, [x29, 72]
	cmp	w1, w0
	bcc	.L641
	ldr	x0, [x29, 32]
	str	wzr, [x0]
	ldr	x0, [x29, 32]
	mov	w1, 16
	str	w1, [x0, 12]
	ldr	x0, [x29, 32]
	mov	w1, 1
	str	w1, [x0, 8]
	ldr	x0, [x29, 32]
	mov	w1, 1
	str	w1, [x0, 16]
	ldr	x0, [x29, 32]
	mov	w1, 1
	str	w1, [x0, 92]
	ldr	x0, [x29, 48]
	ldr	x0, [x0, 272]
	ldr	x1, [x29, 32]
	bl	ndfc_execute_cmd
	str	w0, [x29, 56]
	ldr	x0, [x29, 48]
	bl	nand_read_chip_status_ready
	str	w0, [x29, 56]
	ldr	w0, [x29, 56]
	cmp	w0, 0
	beq	.L642
	adrp	x0, .LC112
	add	x0, x0, :lo12:.LC112
	bl	nand_print
.L642:
	ldr	x0, [x29, 48]
	bl	nand_disable_chip
	ldr	w0, [x29, 56]
.L634:
	ldp	x29, x30, [sp], 80
	ret
	.size	hynix16nm_write_page_FF, .-hynix16nm_write_page_FF
	.align	2
	.global	hynix16nm_vender_get_param
	.type	hynix16nm_vender_get_param, %function
hynix16nm_vender_get_param:
	stp	x29, x30, [sp, -64]!
	add	x29, sp, 0
	str	x0, [x29, 40]
	str	x1, [x29, 32]
	str	x2, [x29, 24]
	str	w3, [x29, 20]
	str	wzr, [x29, 56]
	mov	w0, 55
	strb	w0, [x29, 55]
	ldr	x0, [x29, 40]
	bl	nand_enable_chip
	ldr	x0, [x29, 40]
	ldr	x0, [x0, 272]
	bl	ndfc_disable_randomize
	str	wzr, [x29, 60]
	b	.L644
.L645:
	ldr	w0, [x29, 60]
	ldr	x1, [x29, 24]
	add	x2, x1, x0
	ldr	w0, [x29, 60]
	ldr	x1, [x29, 32]
	add	x1, x1, x0
	add	x0, x29, 55
	mov	w4, 1
	mov	x3, x1
	mov	x1, x0
	ldr	x0, [x29, 40]
	bl	get_data_with_nand_bus_one_cmd
	ldr	w0, [x29, 60]
	add	w0, w0, 1
	str	w0, [x29, 60]
.L644:
	ldr	w1, [x29, 60]
	ldr	w0, [x29, 20]
	cmp	w1, w0
	bcc	.L645
	ldr	x0, [x29, 40]
	bl	nand_disable_chip
	ldr	w0, [x29, 56]
	ldp	x29, x30, [sp], 64
	ret
	.size	hynix16nm_vender_get_param, .-hynix16nm_vender_get_param
	.section	.rodata
	.align	3
.LC113:
	.string	"_major_check_byte, wrong input para, level %d, len %d\n"
	.align	3
.LC114:
	.string	"%d:  '1'-'0' : %d - %d\n"
	.align	3
.LC115:
	.string	"get bit %d failed!\n"
	.text
	.align	2
	.global	hynix16nm_major_check_byte
	.type	hynix16nm_major_check_byte, %function
hynix16nm_major_check_byte:
	stp	x29, x30, [sp, -96]!
	add	x29, sp, 0
	str	x0, [x29, 56]
	str	w1, [x29, 52]
	str	w2, [x29, 48]
	str	x3, [x29, 40]
	str	x4, [x29, 32]
	str	w5, [x29, 28]
	strb	wzr, [x29, 71]
	ldr	w0, [x29, 28]
	lsr	w1, w0, 1
	ldr	w0, [x29, 48]
	cmp	w1, w0
	bls	.L648
	adrp	x0, .LC113
	add	x0, x0, :lo12:.LC113
	ldr	w2, [x29, 28]
	ldr	w1, [x29, 48]
	bl	nand_print
	ldr	x0, [x29, 56]
	mov	w1, -1
	strb	w1, [x0]
	mov	w0, -107
	b	.L649
.L648:
	str	wzr, [x29, 72]
	str	wzr, [x29, 92]
	b	.L650
.L665:
	str	wzr, [x29, 84]
	str	wzr, [x29, 80]
	str	wzr, [x29, 76]
	str	wzr, [x29, 88]
	b	.L651
.L654:
	ldr	w0, [x29, 88]
	ldr	x1, [x29, 40]
	add	x0, x1, x0
	ldrb	w0, [x0]
	mov	w1, w0
	ldr	w0, [x29, 92]
	lsr	w0, w1, w0
	and	w0, w0, 1
	cmp	w0, 0
	beq	.L652
	ldr	w0, [x29, 84]
	add	w0, w0, 1
	str	w0, [x29, 84]
	b	.L653
.L652:
	ldr	w0, [x29, 80]
	add	w0, w0, 1
	str	w0, [x29, 80]
.L653:
	ldr	w0, [x29, 88]
	add	w0, w0, 1
	str	w0, [x29, 88]
.L651:
	ldr	w1, [x29, 88]
	ldr	w0, [x29, 28]
	cmp	w1, w0
	bcc	.L654
	ldr	w1, [x29, 84]
	ldr	w0, [x29, 48]
	cmp	w1, w0
	bls	.L655
	ldr	w0, [x29, 92]
	mov	w1, 1
	lsl	w0, w1, w0
	uxtb	w1, w0
	ldrb	w0, [x29, 71]
	orr	w0, w1, w0
	strb	w0, [x29, 71]
	mov	w0, 1
	str	w0, [x29, 76]
.L655:
	ldr	w1, [x29, 80]
	ldr	w0, [x29, 48]
	cmp	w1, w0
	bls	.L656
	mov	w0, 1
	str	w0, [x29, 76]
.L656:
	ldr	w0, [x29, 76]
	cmp	w0, 0
	bne	.L657
	ldr	w0, [x29, 52]
	cmp	w0, 1
	bne	.L657
	str	wzr, [x29, 84]
	str	wzr, [x29, 80]
	str	wzr, [x29, 76]
	str	wzr, [x29, 88]
	b	.L658
.L661:
	ldr	w0, [x29, 88]
	ldr	x1, [x29, 32]
	add	x0, x1, x0
	ldrb	w0, [x0]
	mov	w1, w0
	ldr	w0, [x29, 92]
	lsr	w0, w1, w0
	and	w0, w0, 1
	cmp	w0, 0
	beq	.L659
	ldr	w0, [x29, 80]
	add	w0, w0, 1
	str	w0, [x29, 80]
	b	.L660
.L659:
	ldr	w0, [x29, 84]
	add	w0, w0, 1
	str	w0, [x29, 84]
.L660:
	ldr	w0, [x29, 88]
	add	w0, w0, 1
	str	w0, [x29, 88]
.L658:
	ldr	w1, [x29, 88]
	ldr	w0, [x29, 28]
	cmp	w1, w0
	bcc	.L661
	ldr	w1, [x29, 80]
	ldr	w0, [x29, 48]
	cmp	w1, w0
	bls	.L662
	mov	w0, 1
	str	w0, [x29, 76]
.L662:
	ldr	w1, [x29, 84]
	ldr	w0, [x29, 48]
	cmp	w1, w0
	bls	.L657
	ldr	w0, [x29, 92]
	mov	w1, 1
	lsl	w0, w1, w0
	uxtb	w1, w0
	ldrb	w0, [x29, 71]
	orr	w0, w1, w0
	strb	w0, [x29, 71]
	mov	w0, 1
	str	w0, [x29, 76]
.L657:
	ldr	w0, [x29, 76]
	cmp	w0, 0
	beq	.L663
	ldr	w0, [x29, 72]
	add	w0, w0, 1
	str	w0, [x29, 72]
	b	.L664
.L663:
	adrp	x0, .LC114
	add	x0, x0, :lo12:.LC114
	ldr	w3, [x29, 80]
	ldr	w2, [x29, 84]
	ldr	w1, [x29, 92]
	bl	nand_print
	adrp	x0, .LC115
	add	x0, x0, :lo12:.LC115
	ldr	w1, [x29, 92]
	bl	nand_print
.L664:
	ldr	w0, [x29, 92]
	add	w0, w0, 1
	str	w0, [x29, 92]
.L650:
	ldr	w0, [x29, 92]
	cmp	w0, 7
	bls	.L665
	ldr	w0, [x29, 72]
	cmp	w0, 8
	bne	.L666
	ldr	x0, [x29, 56]
	ldrb	w1, [x29, 71]
	strb	w1, [x0]
	mov	w0, 0
	b	.L649
.L666:
	ldr	x0, [x29, 56]
	mov	w1, -1
	strb	w1, [x0]
	mov	w0, -80
.L649:
	ldp	x29, x30, [sp], 96
	ret
	.size	hynix16nm_major_check_byte, .-hynix16nm_major_check_byte
	.section	.rodata
	.align	3
.LC116:
	.string	"_get_read_retry_parameters, get rr count failed!\n"
	.align	3
.LC117:
	.string	"rr cnt: %d\n"
	.align	3
.LC118:
	.string	"_get_read_retry_parameters, get rr reg count failed!\n"
	.align	3
.LC119:
	.string	"rr reg cnt: %d\n"
	.align	3
.LC120:
	.string	"read retry value from otp error: rr_cnt %d rr_reg_cnt %d!\n"
	.align	3
.LC121:
	.string	"_get_read_retry_parameters, get the %d-th byte of rr table failed!\n"
	.align	3
.LC122:
	.string	"%02x "
	.text
	.align	2
	.global	hynix16nm_get_read_retry_cfg
	.type	hynix16nm_get_read_retry_cfg, %function
hynix16nm_get_read_retry_cfg:
	stp	x29, x30, [sp, -144]!
	add	x29, sp, 0
	str	x0, [x29, 40]
	str	x1, [x29, 32]
	str	x2, [x29, 24]
	str	x3, [x29, 16]
	str	wzr, [x29, 140]
	str	wzr, [x29, 136]
	add	x0, x29, 88
	stp	xzr, xzr, [x0]
	add	x0, x29, 104
	stp	xzr, xzr, [x0]
	add	x0, x29, 56
	stp	xzr, xzr, [x0]
	add	x0, x29, 72
	stp	xzr, xzr, [x0]
	mov	w0, 32
	str	w0, [x29, 120]
	str	wzr, [x29, 132]
	b	.L668
.L669:
	ldr	w0, [x29, 132]
	ldr	x1, [x29, 16]
	add	x0, x1, x0
	ldrb	w2, [x0]
	ldr	w0, [x29, 132]
	sub	x1, x29, #3952
	add	x0, x1, x0
	mov	w1, w2
	strb	w1, [x0, 4040]
	ldr	w0, [x29, 132]
	add	w0, w0, 1
	str	w0, [x29, 132]
.L668:
	ldr	w0, [x29, 132]
	cmp	w0, 7
	bls	.L669
	add	x1, x29, 56
	add	x0, x29, 88
	mov	w5, 8
	mov	x4, x1
	mov	x3, x0
	mov	w2, 4
	mov	w1, 0
	ldr	x0, [x29, 40]
	bl	hynix16nm_major_check_byte
	str	w0, [x29, 136]
	ldr	w0, [x29, 136]
	cmp	w0, 0
	bge	.L670
	adrp	x0, .LC116
	add	x0, x0, :lo12:.LC116
	bl	nand_print
	ldr	w0, [x29, 136]
	b	.L688
.L670:
	ldr	x0, [x29, 40]
	ldrb	w0, [x0]
	mov	w1, w0
	adrp	x0, .LC117
	add	x0, x0, :lo12:.LC117
	bl	nand_print_dbg
	str	wzr, [x29, 132]
	b	.L672
.L673:
	ldr	w0, [x29, 132]
	add	w0, w0, 8
	uxtw	x0, w0
	ldr	x1, [x29, 16]
	add	x0, x1, x0
	ldrb	w2, [x0]
	ldr	w0, [x29, 132]
	sub	x1, x29, #3952
	add	x0, x1, x0
	mov	w1, w2
	strb	w1, [x0, 4040]
	ldr	w0, [x29, 132]
	add	w0, w0, 1
	str	w0, [x29, 132]
.L672:
	ldr	w0, [x29, 132]
	cmp	w0, 7
	bls	.L673
	add	x1, x29, 56
	add	x0, x29, 88
	mov	w5, 8
	mov	x4, x1
	mov	x3, x0
	mov	w2, 4
	mov	w1, 0
	ldr	x0, [x29, 32]
	bl	hynix16nm_major_check_byte
	str	w0, [x29, 136]
	ldr	w0, [x29, 136]
	cmp	w0, 0
	bge	.L674
	adrp	x0, .LC118
	add	x0, x0, :lo12:.LC118
	bl	nand_print
	ldr	w0, [x29, 136]
	b	.L688
.L674:
	ldr	x0, [x29, 32]
	ldrb	w0, [x0]
	mov	w1, w0
	adrp	x0, .LC119
	add	x0, x0, :lo12:.LC119
	bl	nand_print_dbg
	ldr	x0, [x29, 40]
	ldrb	w0, [x0]
	cmp	w0, 8
	bne	.L675
	ldr	x0, [x29, 32]
	ldrb	w0, [x0]
	cmp	w0, 4
	beq	.L676
.L675:
	ldr	x0, [x29, 40]
	ldrb	w0, [x0]
	mov	w1, w0
	ldr	x0, [x29, 32]
	ldrb	w0, [x0]
	mov	w2, w0
	adrp	x0, .LC120
	add	x0, x0, :lo12:.LC120
	bl	nand_print
	mov	w0, -81
	b	.L688
.L676:
	str	wzr, [x29, 128]
	b	.L677
.L682:
	str	wzr, [x29, 124]
	b	.L678
.L679:
	ldr	w0, [x29, 124]
	sub	x1, x29, #3952
	add	x0, x1, x0
	strb	wzr, [x0, 4040]
	ldr	w0, [x29, 124]
	sub	x1, x29, #3952
	add	x0, x1, x0
	strb	wzr, [x0, 4008]
	ldr	w1, [x29, 124]
	ldr	w0, [x29, 120]
	mul	w0, w1, w0
	add	w0, w0, 8
	lsl	w1, w0, 1
	ldr	w0, [x29, 128]
	add	w0, w1, w0
	uxtw	x0, w0
	ldr	x1, [x29, 16]
	add	x0, x1, x0
	ldrb	w2, [x0]
	ldr	w0, [x29, 124]
	sub	x1, x29, #3952
	add	x0, x1, x0
	mov	w1, w2
	strb	w1, [x0, 4040]
	ldr	w1, [x29, 124]
	ldr	w0, [x29, 120]
	mul	w0, w1, w0
	add	w0, w0, 8
	lsl	w1, w0, 1
	ldr	w0, [x29, 120]
	add	w1, w1, w0
	ldr	w0, [x29, 128]
	add	w0, w1, w0
	uxtw	x0, w0
	ldr	x1, [x29, 16]
	add	x0, x1, x0
	ldrb	w2, [x0]
	ldr	w0, [x29, 124]
	sub	x1, x29, #3952
	add	x0, x1, x0
	mov	w1, w2
	strb	w1, [x0, 4008]
	ldr	w0, [x29, 124]
	add	w0, w0, 1
	str	w0, [x29, 124]
.L678:
	ldr	w0, [x29, 124]
	cmp	w0, 7
	bls	.L679
	ldr	w0, [x29, 128]
	ldr	x1, [x29, 24]
	add	x0, x1, x0
	add	x2, x29, 56
	add	x1, x29, 88
	mov	w5, 8
	mov	x4, x2
	mov	x3, x1
	mov	w2, 4
	mov	w1, 1
	bl	hynix16nm_major_check_byte
	str	w0, [x29, 136]
	ldr	w0, [x29, 136]
	cmp	w0, 0
	bge	.L680
	adrp	x0, .LC121
	add	x0, x0, :lo12:.LC121
	ldr	w1, [x29, 128]
	bl	nand_print
	ldr	w0, [x29, 136]
	str	w0, [x29, 140]
	b	.L681
.L680:
	ldr	w0, [x29, 128]
	add	w0, w0, 1
	str	w0, [x29, 128]
.L677:
	ldr	w1, [x29, 128]
	ldr	w0, [x29, 120]
	cmp	w1, w0
	bcc	.L682
.L681:
	str	wzr, [x29, 128]
	b	.L683
.L685:
	ldr	w0, [x29, 128]
	and	w0, w0, 7
	cmp	w0, 0
	bne	.L684
	ldr	w0, [x29, 128]
	cmp	w0, 0
	beq	.L684
	adrp	x0, .LC45
	add	x0, x0, :lo12:.LC45
	bl	nand_print_dbg
.L684:
	ldr	w0, [x29, 128]
	ldr	x1, [x29, 24]
	add	x0, x1, x0
	ldrb	w0, [x0]
	mov	w1, w0
	adrp	x0, .LC122
	add	x0, x0, :lo12:.LC122
	bl	nand_print_dbg
	ldr	w0, [x29, 128]
	add	w0, w0, 1
	str	w0, [x29, 128]
.L683:
	ldr	w1, [x29, 128]
	ldr	w0, [x29, 120]
	cmp	w1, w0
	bcc	.L685
	adrp	x0, .LC45
	add	x0, x0, :lo12:.LC45
	bl	nand_print_dbg
	ldr	w0, [x29, 140]
	cmp	w0, 0
	beq	.L686
	ldr	w0, [x29, 140]
	str	w0, [x29, 136]
	b	.L687
.L686:
	str	wzr, [x29, 136]
.L687:
	ldr	w0, [x29, 136]
.L688:
	ldp	x29, x30, [sp], 144
	ret
	.size	hynix16nm_get_read_retry_cfg, .-hynix16nm_get_read_retry_cfg
	.section	.rodata
	.align	3
.LC123:
	.string	"start get read retry param from: ce %d, rb %d...\n"
	.align	3
.LC124:
	.string	"invalid buffer for otp info!\n"
	.text
	.align	2
	.global	hynix16nm_read_otp_info_hynix
	.type	hynix16nm_read_otp_info_hynix, %function
hynix16nm_read_otp_info_hynix:
	stp	x29, x30, [sp, -80]!
	add	x29, sp, 0
	str	x0, [x29, 24]
	str	x1, [x29, 16]
	ldr	x0, [x29, 24]
	bl	nand_enable_chip
	ldr	x0, [x29, 24]
	ldr	x0, [x0, 272]
	bl	ndfc_disable_randomize
	ldr	x0, [x29, 24]
	ldr	x0, [x0, 272]
	ldr	x1, [x29, 24]
	ldr	w1, [x1, 28]
	uxtw	x1, w1
	add	x1, x1, 316
	ldr	w3, [x0, x1, lsl 2]
	ldr	x0, [x29, 24]
	ldr	x0, [x0, 272]
	ldr	x1, [x29, 24]
	ldr	w1, [x1, 28]
	uxtw	x1, w1
	add	x1, x1, 324
	ldr	w1, [x0, x1, lsl 2]
	adrp	x0, .LC123
	add	x0, x0, :lo12:.LC123
	mov	w2, w1
	mov	w1, w3
	bl	nand_print_dbg
	ldr	x0, [x29, 16]
	str	x0, [x29, 72]
	ldr	x0, [x29, 72]
	cmp	x0, 0
	bne	.L690
	ldr	x0, [x29, 24]
	bl	nand_disable_chip
	adrp	x0, .LC124
	add	x0, x0, :lo12:.LC124
	bl	nand_print
	mov	w0, -79
	b	.L694
.L690:
	mov	w2, 1
	mov	w1, -1
	ldr	x0, [x29, 24]
	bl	set_one_cmd
	mov	w0, 54
	strb	w0, [x29, 48]
	mov	w0, 56
	strb	w0, [x29, 56]
	mov	w0, 82
	strb	w0, [x29, 40]
	add	x2, x29, 40
	add	x1, x29, 56
	add	x0, x29, 48
	mov	w6, 1
	mov	w5, 1
	mov	x4, x2
	mov	x3, x1
	mov	w2, 0
	mov	x1, x0
	ldr	x0, [x29, 24]
	bl	set_cmd_with_nand_bus
	mov	w2, 0
	mov	w1, 22
	ldr	x0, [x29, 24]
	bl	set_one_cmd
	mov	w2, 0
	mov	w1, 23
	ldr	x0, [x29, 24]
	bl	set_one_cmd
	mov	w2, 0
	mov	w1, 4
	ldr	x0, [x29, 24]
	bl	set_one_cmd
	mov	w2, 0
	mov	w1, 25
	ldr	x0, [x29, 24]
	bl	set_one_cmd
	mov	w2, 0
	mov	w1, 0
	ldr	x0, [x29, 24]
	bl	set_one_cmd
	mov	w1, 0
	ldr	x0, [x29, 24]
	bl	set_one_addr
	mov	w1, 0
	ldr	x0, [x29, 24]
	bl	set_one_addr
	mov	w1, 0
	ldr	x0, [x29, 24]
	bl	set_one_addr
	mov	w1, 2
	ldr	x0, [x29, 24]
	bl	set_one_addr
	mov	w1, 0
	ldr	x0, [x29, 24]
	bl	set_one_addr
	mov	w0, 48
	strb	w0, [x29, 48]
	add	x0, x29, 48
	mov	w4, 528
	ldr	x3, [x29, 72]
	mov	x2, 0
	mov	x1, x0
	ldr	x0, [x29, 24]
	bl	get_data_with_nand_bus_one_cmd
	mov	w2, 1
	mov	w1, -1
	ldr	x0, [x29, 24]
	bl	set_one_cmd
	mov	w0, 54
	strb	w0, [x29, 48]
	mov	w0, 56
	strb	w0, [x29, 56]
	strb	wzr, [x29, 40]
	add	x2, x29, 40
	add	x1, x29, 56
	add	x0, x29, 48
	mov	w6, 1
	mov	w5, 1
	mov	x4, x2
	mov	x3, x1
	mov	w2, 0
	mov	x1, x0
	ldr	x0, [x29, 24]
	bl	set_cmd_with_nand_bus
	mov	w2, 0
	mov	w1, 22
	ldr	x0, [x29, 24]
	bl	set_one_cmd
	mov	w2, 0
	mov	w1, 0
	ldr	x0, [x29, 24]
	bl	set_one_cmd
	mov	w1, 0
	ldr	x0, [x29, 24]
	bl	set_one_addr
	mov	w1, 0
	ldr	x0, [x29, 24]
	bl	set_one_addr
	mov	w1, 0
	ldr	x0, [x29, 24]
	bl	set_one_addr
	mov	w1, 0
	ldr	x0, [x29, 24]
	bl	set_one_addr
	mov	w1, 0
	ldr	x0, [x29, 24]
	bl	set_one_addr
	mov	w2, 1
	mov	w1, 48
	ldr	x0, [x29, 24]
	bl	set_one_cmd
	ldr	x0, [x29, 72]
	ldrb	w0, [x0]
	cmp	w0, 8
	bne	.L692
	ldr	x0, [x29, 72]
	add	x0, x0, 1
	ldrb	w0, [x0]
	cmp	w0, 8
	beq	.L693
.L692:
	ldr	x0, [x29, 72]
	ldrb	w0, [x0]
	mov	w1, w0
	ldr	x0, [x29, 72]
	add	x0, x0, 1
	ldrb	w0, [x0]
	mov	w2, w0
	adrp	x0, .LC51
	add	x0, x0, :lo12:.LC51
	bl	nand_print
	ldr	x0, [x29, 24]
	bl	nand_disable_chip
	mov	w0, -96
	str	w0, [x29, 68]
	ldr	w0, [x29, 68]
	b	.L694
.L693:
	ldr	x0, [x29, 24]
	bl	nand_disable_chip
	mov	w0, 0
.L694:
	ldp	x29, x30, [sp], 80
	ret
	.size	hynix16nm_read_otp_info_hynix, .-hynix16nm_read_otp_info_hynix
	.section	.rodata
	.align	3
.LC125:
	.string	"otp_info_hynix_16nm : allocate memory fail\n"
	.align	3
.LC126:
	.string	"m1 CH %d chip %d get otp info failed!\n"
	.align	3
.LC127:
	.string	"m1 CH %d chip %d get read retry cfg from otp info failed!\n"
	.text
	.align	2
	.global	hynix16nm_get_rr_value_otp_hynix
	.type	hynix16nm_get_rr_value_otp_hynix, %function
hynix16nm_get_rr_value_otp_hynix:
	stp	x29, x30, [sp, -64]!
	add	x29, sp, 0
	str	x0, [x29, 24]
	str	wzr, [x29, 60]
	mov	w0, 528
	bl	nand_get_temp_buf
	str	x0, [x29, 48]
	ldr	x0, [x29, 48]
	cmp	x0, 0
	bne	.L696
	adrp	x0, .LC125
	add	x0, x0, :lo12:.LC125
	bl	nand_print
	mov	w0, -78
	b	.L700
.L696:
	ldr	x1, [x29, 48]
	ldr	x0, [x29, 24]
	bl	hynix16nm_read_otp_info_hynix
	str	w0, [x29, 60]
	ldr	w0, [x29, 60]
	cmp	w0, 0
	bge	.L698
	ldr	x0, [x29, 24]
	ldr	x0, [x0, 272]
	ldr	w1, [x0, 12]
	ldr	x0, [x29, 24]
	ldr	w2, [x0, 28]
	adrp	x0, .LC126
	add	x0, x0, :lo12:.LC126
	bl	nand_print
.L698:
	ldr	x0, [x29, 24]
	add	x2, x0, 56
	add	x1, x29, 46
	add	x0, x29, 47
	ldr	x3, [x29, 48]
	bl	hynix16nm_get_read_retry_cfg
	str	w0, [x29, 60]
	ldr	w0, [x29, 60]
	cmp	w0, 0
	bge	.L699
	ldr	x0, [x29, 24]
	ldr	x0, [x0, 272]
	ldr	w1, [x0, 12]
	ldr	x0, [x29, 24]
	ldr	w2, [x0, 28]
	adrp	x0, .LC127
	add	x0, x0, :lo12:.LC127
	bl	nand_print
.L699:
	ldr	x0, [x29, 48]
	bl	nand_free_temp_buf
	ldr	w0, [x29, 60]
.L700:
	ldp	x29, x30, [sp], 64
	ret
	.size	hynix16nm_get_rr_value_otp_hynix, .-hynix16nm_get_rr_value_otp_hynix
	.align	2
	.global	hynix16nm_save_otp_value
	.type	hynix16nm_save_otp_value, %function
hynix16nm_save_otp_value:
	sub	sp, sp, #32
	str	x0, [sp, 8]
	str	x1, [sp]
	str	wzr, [sp, 28]
	b	.L702
.L703:
	ldr	w0, [sp, 28]
	ldr	x1, [sp]
	add	x0, x1, x0
	ldrb	w2, [x0]
	ldr	x1, [sp, 8]
	ldr	w0, [sp, 28]
	add	x0, x1, x0
	mov	w1, w2
	strb	w1, [x0, 56]
	ldr	w0, [sp, 28]
	add	w0, w0, 1
	str	w0, [sp, 28]
.L702:
	ldr	w0, [sp, 28]
	cmp	w0, 31
	bls	.L703
	nop
	add	sp, sp, 32
	ret
	.size	hynix16nm_save_otp_value, .-hynix16nm_save_otp_value
	.align	2
	.global	hynix16nm_get_otp_param
	.type	hynix16nm_get_otp_param, %function
hynix16nm_get_otp_param:
	stp	x29, x30, [sp, -64]!
	add	x29, sp, 0
	str	x0, [x29, 24]
	str	x1, [x29, 16]
	str	wzr, [x29, 52]
	str	wzr, [x29, 48]
	b	.L705
.L707:
	adrp	x0, .LC54
	add	x0, x0, :lo12:.LC54
	ldr	w1, [x29, 52]
	bl	nand_print_dbg
	ldr	x0, [x29, 24]
	bl	hynix16nm_get_rr_value_otp_hynix
	str	w0, [x29, 44]
	ldr	w0, [x29, 44]
	cmp	w0, 0
	bne	.L706
	mov	w0, 1
	str	w0, [x29, 48]
.L706:
	ldr	w0, [x29, 52]
	add	w0, w0, 1
	str	w0, [x29, 52]
.L705:
	ldr	w0, [x29, 48]
	cmp	w0, 0
	beq	.L707
	str	wzr, [x29, 60]
	b	.L708
.L711:
	str	wzr, [x29, 56]
	b	.L709
.L710:
	ldr	w0, [x29, 60]
	lsl	w1, w0, 2
	ldr	w0, [x29, 56]
	add	w0, w1, w0
	uxtw	x0, w0
	ldr	x1, [x29, 16]
	add	x0, x1, x0
	ldr	w1, [x29, 60]
	lsl	w2, w1, 2
	ldr	w1, [x29, 56]
	add	w1, w2, w1
	ldr	x2, [x29, 24]
	uxtw	x1, w1
	add	x1, x2, x1
	ldrb	w1, [x1, 56]
	strb	w1, [x0]
	ldr	w0, [x29, 56]
	add	w0, w0, 1
	str	w0, [x29, 56]
.L709:
	ldr	w0, [x29, 56]
	cmp	w0, 3
	bls	.L710
	ldr	w0, [x29, 60]
	add	w0, w0, 1
	str	w0, [x29, 60]
.L708:
	ldr	w0, [x29, 60]
	cmp	w0, 7
	bls	.L711
	mov	w0, 0
	ldp	x29, x30, [sp], 64
	ret
	.size	hynix16nm_get_otp_param, .-hynix16nm_get_otp_param
	.align	2
	.global	hynix16nm_set_default_param
	.type	hynix16nm_set_default_param, %function
hynix16nm_set_default_param:
	stp	x29, x30, [sp, -48]!
	add	x29, sp, 0
	str	x0, [x29, 24]
	str	wzr, [x29, 40]
	str	wzr, [x29, 44]
	b	.L714
.L715:
	ldr	x1, [x29, 24]
	ldr	w0, [x29, 44]
	add	x0, x1, x0
	ldrb	w2, [x0, 56]
	ldr	w0, [x29, 44]
	sub	x1, x29, #4048
	add	x0, x1, x0
	mov	w1, w2
	strb	w1, [x0, 4080]
	ldr	w0, [x29, 44]
	add	w0, w0, 1
	str	w0, [x29, 44]
.L714:
	adrp	x0, hynix16nm_read_retry_reg_num
	add	x0, x0, :lo12:hynix16nm_read_retry_reg_num
	ldrb	w0, [x0]
	mov	w1, w0
	ldr	w0, [x29, 44]
	cmp	w1, w0
	bhi	.L715
	adrp	x0, hynix16nm_read_retry_reg_num
	add	x0, x0, :lo12:hynix16nm_read_retry_reg_num
	ldrb	w0, [x0]
	mov	w2, w0
	adrp	x0, hynix16nm_read_retry_reg_adr
	add	x1, x0, :lo12:hynix16nm_read_retry_reg_adr
	add	x0, x29, 32
	mov	w3, w2
	mov	x2, x1
	mov	x1, x0
	ldr	x0, [x29, 24]
	bl	hynix16nm_vender_set_param
	str	w0, [x29, 40]
	adrp	x0, .LC56
	add	x0, x0, :lo12:.LC56
	bl	nand_print_dbg
	str	wzr, [x29, 44]
	b	.L716
.L717:
	ldr	w0, [x29, 44]
	sub	x1, x29, #4048
	add	x0, x1, x0
	ldrb	w0, [x0, 4080]
	mov	w1, w0
	adrp	x0, .LC57
	add	x0, x0, :lo12:.LC57
	bl	nand_print_dbg
	ldr	w0, [x29, 44]
	add	w0, w0, 1
	str	w0, [x29, 44]
.L716:
	adrp	x0, hynix16nm_read_retry_reg_num
	add	x0, x0, :lo12:hynix16nm_read_retry_reg_num
	ldrb	w0, [x0]
	mov	w1, w0
	ldr	w0, [x29, 44]
	cmp	w1, w0
	bhi	.L717
	adrp	x0, .LC45
	add	x0, x0, :lo12:.LC45
	bl	nand_print_dbg
	ldr	w0, [x29, 40]
	ldp	x29, x30, [sp], 48
	ret
	.size	hynix16nm_set_default_param, .-hynix16nm_set_default_param
	.section	.rodata
	.align	3
.LC128:
	.string	"ch %d, chip %d Read Retry value Table from uboot\n"
	.align	3
.LC129:
	.string	"[PHY_DBG] repair ch %d chip %d otp value end\n"
	.text
	.align	2
	.global	hynix16nm_init_readretry_param
	.type	hynix16nm_init_readretry_param, %function
hynix16nm_init_readretry_param:
	stp	x29, x30, [sp, -176]!
	add	x29, sp, 0
	str	x0, [x29, 24]
	str	wzr, [x29, 172]
	str	xzr, [x29, 160]
	str	wzr, [x29, 156]
	str	xzr, [x29, 144]
	add	x0, x29, 40
	stp	xzr, xzr, [x0]
	add	x0, x29, 56
	stp	xzr, xzr, [x0]
	str	xzr, [x29, 72]
	ldr	x0, [x29, 24]
	ldr	x0, [x0, 280]
	ldr	w0, [x0, 20]
	lsl	w0, w0, 9
	bl	nand_get_temp_buf
	str	x0, [x29, 160]
	ldr	x0, [x29, 160]
	cmp	x0, 0
	bne	.L720
	mov	w0, -77
	b	.L728
.L720:
	mov	w0, 32768
	bl	nand_get_temp_buf
	str	x0, [x29, 144]
	ldr	x0, [x29, 144]
	cmp	x0, 0
	bne	.L722
	ldr	x0, [x29, 160]
	bl	nand_free_temp_buf
	adrp	x0, .LC59
	add	x0, x0, :lo12:.LC59
	bl	nand_print
	mov	w0, -76
	b	.L728
.L722:
	str	wzr, [x29, 156]
	ldr	x0, [x29, 24]
	ldr	w0, [x0, 24]
	mov	w3, w0
	mov	w2, 32
	ldr	x1, [x29, 160]
	ldr	x0, [x29, 144]
	bl	hynix16nm_get_special_info
	str	w0, [x29, 156]
	ldr	w0, [x29, 156]
	cmp	w0, 0
	bne	.L723
	ldr	x0, [x29, 24]
	ldr	x0, [x0, 272]
	ldr	w1, [x0, 12]
	ldr	w2, [x29, 40]
	adrp	x0, .LC128
	add	x0, x0, :lo12:.LC128
	bl	nand_print_dbg
	str	wzr, [x29, 172]
	b	.L724
.L726:
	ldr	w0, [x29, 172]
	ldr	x1, [x29, 160]
	add	x0, x1, x0
	ldrb	w0, [x0]
	mov	w1, w0
	adrp	x0, .LC61
	add	x0, x0, :lo12:.LC61
	bl	nand_print_dbg
	ldr	w0, [x29, 172]
	and	w0, w0, 7
	cmp	w0, 7
	bne	.L725
	adrp	x0, .LC45
	add	x0, x0, :lo12:.LC45
	bl	nand_print_dbg
.L725:
	ldr	w0, [x29, 172]
	add	w0, w0, 1
	str	w0, [x29, 172]
.L724:
	ldr	w0, [x29, 172]
	cmp	w0, 31
	bls	.L726
	ldr	x1, [x29, 160]
	ldr	x0, [x29, 24]
	bl	hynix16nm_save_otp_value
	ldr	x0, [x29, 24]
	bl	hynix16nm_set_default_param
	b	.L727
.L723:
	ldr	x0, [x29, 24]
	ldr	x0, [x0, 272]
	ldr	w1, [x0, 12]
	adrp	x0, .LC62
	add	x0, x0, :lo12:.LC62
	bl	nand_print_dbg
	add	x0, x29, 80
	mov	x1, x0
	ldr	x0, [x29, 24]
	bl	hynix16nm_get_otp_param
	ldr	x0, [x29, 24]
	bl	hynix16nm_set_default_param
	ldr	x0, [x29, 24]
	ldr	x0, [x0, 272]
	ldr	w1, [x0, 12]
	ldr	w2, [x29, 40]
	adrp	x0, .LC129
	add	x0, x0, :lo12:.LC129
	bl	nand_print_dbg
	nop
.L727:
	ldr	x0, [x29, 144]
	bl	nand_free_temp_buf
	ldr	x0, [x29, 160]
	bl	nand_free_temp_buf
	mov	w0, 0
.L728:
	ldp	x29, x30, [sp], 176
	ret
	.size	hynix16nm_init_readretry_param, .-hynix16nm_init_readretry_param
	.align	2
	.global	hynix16nm_readretry_init
	.type	hynix16nm_readretry_init, %function
hynix16nm_readretry_init:
	stp	x29, x30, [sp, -48]!
	add	x29, sp, 0
	str	x0, [x29, 24]
	ldr	x0, [x29, 24]
	ldr	x0, [x0, 280]
	ldr	w0, [x0, 52]
	lsr	w0, w0, 16
	uxtb	w1, w0
	adrp	x0, hynix16nm_read_retry_mode
	add	x0, x0, :lo12:hynix16nm_read_retry_mode
	strb	w1, [x0]
	ldr	x0, [x29, 24]
	ldr	x0, [x0, 280]
	ldr	w0, [x0, 52]
	lsr	w0, w0, 8
	uxtb	w1, w0
	adrp	x0, hynix16nm_read_retry_cycle
	add	x0, x0, :lo12:hynix16nm_read_retry_cycle
	strb	w1, [x0]
	ldr	x0, [x29, 24]
	ldr	x0, [x0, 280]
	ldr	w0, [x0, 52]
	uxtb	w1, w0
	adrp	x0, hynix16nm_read_retry_reg_num
	add	x0, x0, :lo12:hynix16nm_read_retry_reg_num
	strb	w1, [x0]
	adrp	x0, hynix16nm_read_retry_mode
	add	x0, x0, :lo12:hynix16nm_read_retry_mode
	ldrb	w0, [x0]
	cmp	w0, 4
	bne	.L730
	adrp	x0, hynix16nm_read_retry_reg_adr
	add	x0, x0, :lo12:hynix16nm_read_retry_reg_adr
	mov	w1, 56
	strb	w1, [x0]
	adrp	x0, hynix16nm_read_retry_reg_adr
	add	x0, x0, :lo12:hynix16nm_read_retry_reg_adr
	mov	w1, 57
	strb	w1, [x0, 1]
	adrp	x0, hynix16nm_read_retry_reg_adr
	add	x0, x0, :lo12:hynix16nm_read_retry_reg_adr
	mov	w1, 58
	strb	w1, [x0, 2]
	adrp	x0, hynix16nm_read_retry_reg_adr
	add	x0, x0, :lo12:hynix16nm_read_retry_reg_adr
	mov	w1, 59
	strb	w1, [x0, 3]
	ldr	x0, [x29, 24]
	str	wzr, [x0, 184]
	str	wzr, [x29, 44]
	b	.L733
.L730:
	adrp	x0, hynix16nm_read_retry_mode
	add	x0, x0, :lo12:hynix16nm_read_retry_mode
	ldrb	w0, [x0]
	mov	w1, w0
	adrp	x0, .LC64
	add	x0, x0, :lo12:.LC64
	bl	nand_print
	mov	w0, -75
	b	.L732
.L734:
	ldr	x1, [x29, 24]
	ldr	w0, [x29, 44]
	add	x0, x1, x0
	strb	wzr, [x0, 56]
	ldr	w0, [x29, 44]
	add	w0, w0, 1
	str	w0, [x29, 44]
.L733:
	ldr	w0, [x29, 44]
	cmp	w0, 127
	bls	.L734
	ldr	x0, [x29, 24]
	bl	hynix16nm_init_readretry_param
	mov	w0, 0
.L732:
	ldp	x29, x30, [sp], 48
	ret
	.size	hynix16nm_readretry_init, .-hynix16nm_readretry_init
	.section	.rodata
	.align	3
.LC130:
	.string	"hynix16nm_readretry_exit. \n"
	.text
	.align	2
	.global	hynix16nm_readretry_exit
	.type	hynix16nm_readretry_exit, %function
hynix16nm_readretry_exit:
	stp	x29, x30, [sp, -32]!
	add	x29, sp, 0
	str	x0, [x29, 24]
	adrp	x0, .LC130
	add	x0, x0, :lo12:.LC130
	bl	nand_print_dbg
	ldr	x0, [x29, 24]
	bl	hynix16nm_set_default_param
	ldr	x0, [x29, 24]
	str	wzr, [x0, 184]
	mov	w0, 0
	ldp	x29, x30, [sp], 32
	ret
	.size	hynix16nm_readretry_exit, .-hynix16nm_readretry_exit
	.section	.rodata
	.align	3
.LC131:
	.string	" hynix16nm_special_init hynix16nm_read_retry_mode: %d hynix16nm_read_retry_cycle:%d hynix16nm_read_retry_reg_num:%d\n"
	.align	3
.LC132:
	.string	" hynix16nm_special_init error hynix16nm_read_retry_mode: %d hynix16nm_read_retry_cycle:%d hynix16nm_read_retry_reg_num:%d\n"
	.text
	.align	2
	.global	hynix16nm_special_init
	.type	hynix16nm_special_init, %function
hynix16nm_special_init:
	stp	x29, x30, [sp, -32]!
	add	x29, sp, 0
	str	wzr, [x29, 28]
	adrp	x0, g_nsi
	add	x0, x0, :lo12:g_nsi
	ldr	x0, [x0]
	ldr	x0, [x0, 8]
	str	x0, [x29, 16]
	b	.L738
.L739:
	ldr	x0, [x29, 16]
	bl	hynix16nm_readretry_init
	mov	w1, w0
	ldr	w0, [x29, 28]
	orr	w0, w0, w1
	str	w0, [x29, 28]
	ldr	x0, [x29, 16]
	ldr	x0, [x0]
	str	x0, [x29, 16]
.L738:
	ldr	x0, [x29, 16]
	cmp	x0, 0
	bne	.L739
	ldr	w0, [x29, 28]
	cmp	w0, 0
	bne	.L740
	adrp	x0, df_read_page_end
	add	x0, x0, :lo12:df_read_page_end
	adrp	x1, hynix16nm_read_page_end
	add	x1, x1, :lo12:hynix16nm_read_page_end
	str	x1, [x0]
	adrp	x0, hynix16nm_read_retry_mode
	add	x0, x0, :lo12:hynix16nm_read_retry_mode
	ldrb	w0, [x0]
	mov	w1, w0
	adrp	x0, hynix16nm_read_retry_cycle
	add	x0, x0, :lo12:hynix16nm_read_retry_cycle
	ldrb	w0, [x0]
	mov	w2, w0
	adrp	x0, hynix16nm_read_retry_reg_num
	add	x0, x0, :lo12:hynix16nm_read_retry_reg_num
	ldrb	w0, [x0]
	mov	w3, w0
	adrp	x0, .LC131
	add	x0, x0, :lo12:.LC131
	bl	nand_print_dbg
	b	.L741
.L740:
	adrp	x0, hynix16nm_read_retry_mode
	add	x0, x0, :lo12:hynix16nm_read_retry_mode
	ldrb	w0, [x0]
	mov	w1, w0
	adrp	x0, hynix16nm_read_retry_cycle
	add	x0, x0, :lo12:hynix16nm_read_retry_cycle
	ldrb	w0, [x0]
	mov	w2, w0
	adrp	x0, hynix16nm_read_retry_reg_num
	add	x0, x0, :lo12:hynix16nm_read_retry_reg_num
	ldrb	w0, [x0]
	mov	w3, w0
	adrp	x0, .LC132
	add	x0, x0, :lo12:.LC132
	bl	nand_print
.L741:
	ldr	w0, [x29, 28]
	ldp	x29, x30, [sp], 32
	ret
	.size	hynix16nm_special_init, .-hynix16nm_special_init
	.section	.rodata
	.align	3
.LC133:
	.string	" hynix16nm_special_exit \n"
	.text
	.align	2
	.global	hynix16nm_special_exit
	.type	hynix16nm_special_exit, %function
hynix16nm_special_exit:
	stp	x29, x30, [sp, -32]!
	add	x29, sp, 0
	adrp	x0, g_nsi
	add	x0, x0, :lo12:g_nsi
	ldr	x0, [x0]
	ldr	x0, [x0, 8]
	str	x0, [x29, 24]
	b	.L744
.L745:
	ldr	x0, [x29, 24]
	bl	hynix16nm_readretry_exit
	ldr	x0, [x29, 24]
	ldr	x0, [x0]
	str	x0, [x29, 24]
.L744:
	ldr	x0, [x29, 24]
	cmp	x0, 0
	bne	.L745
	adrp	x0, .LC133
	add	x0, x0, :lo12:.LC133
	bl	nand_print_dbg
	mov	w0, 0
	ldp	x29, x30, [sp], 32
	ret
	.size	hynix16nm_special_exit, .-hynix16nm_special_exit
	.global	sandisk_read_retry_mode
	.bss
	.type	sandisk_read_retry_mode, %object
	.size	sandisk_read_retry_mode, 1
sandisk_read_retry_mode:
	.zero	1
	.global	sandisk_read_retry_cycle
	.type	sandisk_read_retry_cycle, %object
	.size	sandisk_read_retry_cycle, 1
sandisk_read_retry_cycle:
	.zero	1
	.global	sandisk_read_retry_reg_cnt
	.type	sandisk_read_retry_reg_cnt, %object
	.size	sandisk_read_retry_reg_cnt, 1
sandisk_read_retry_reg_cnt:
	.zero	1
	.global	sandisk_acti_start_cmd
	.data
	.align	3
	.type	sandisk_acti_start_cmd, %object
	.size	sandisk_acti_start_cmd, 2
sandisk_acti_start_cmd:
	.byte	59
	.byte	-71
	.global	sandisk_acti_start_addr
	.align	3
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
	.global	sandisk_read_retry_enable_cmd_19
	.align	3
	.type	sandisk_read_retry_enable_cmd_19, %object
	.size	sandisk_read_retry_enable_cmd_19, 1
sandisk_read_retry_enable_cmd_19:
	.byte	-74
	.global	sandisk_read_retry_disable_cmd_19
	.align	3
	.type	sandisk_read_retry_disable_cmd_19, %object
	.size	sandisk_read_retry_disable_cmd_19, 1
sandisk_read_retry_disable_cmd_19:
	.byte	-42
	.global	sandisk_read_retry_cmd_19
	.align	3
	.type	sandisk_read_retry_cmd_19, %object
	.size	sandisk_read_retry_cmd_19, 3
sandisk_read_retry_cmd_19:
	.byte	83
	.byte	83
	.byte	83
	.global	sandisk_read_retry_reg_adr_19
	.align	3
	.type	sandisk_read_retry_reg_adr_19, %object
	.size	sandisk_read_retry_reg_adr_19, 9
sandisk_read_retry_reg_adr_19:
	.byte	4
	.byte	5
	.byte	7
	.zero	6
	.global	sandisk_19_low
	.align	3
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
	.global	sandisk_19_high
	.align	3
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
	.global	sandisk_read_retry_cmd_24
	.align	3
	.type	sandisk_read_retry_cmd_24, %object
	.size	sandisk_read_retry_cmd_24, 3
sandisk_read_retry_cmd_24:
	.byte	83
	.byte	83
	.byte	83
	.global	sandisk_read_retry_reg_adr_24
	.align	3
	.type	sandisk_read_retry_reg_adr_24, %object
	.size	sandisk_read_retry_reg_adr_24, 9
sandisk_read_retry_reg_adr_24:
	.byte	4
	.byte	5
	.byte	7
	.zero	6
	.global	sandisk_24
	.align	3
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
	.global	sandisk_read_retry_enable_cmd_1y
	.align	3
	.type	sandisk_read_retry_enable_cmd_1y, %object
	.size	sandisk_read_retry_enable_cmd_1y, 1
sandisk_read_retry_enable_cmd_1y:
	.byte	93
	.global	sandisk_read_retry_exit_cmd_1y
	.align	3
	.type	sandisk_read_retry_exit_cmd_1y, %object
	.size	sandisk_read_retry_exit_cmd_1y, 1
sandisk_read_retry_exit_cmd_1y:
	.byte	-1
	.global	sandisk_read_retry_cmd_1y
	.align	3
	.type	sandisk_read_retry_cmd_1y, %object
	.size	sandisk_read_retry_cmd_1y, 1
sandisk_read_retry_cmd_1y:
	.byte	-17
	.global	sandisk_read_retry_reg_adr_1y
	.align	3
	.type	sandisk_read_retry_reg_adr_1y, %object
	.size	sandisk_read_retry_reg_adr_1y, 1
sandisk_read_retry_reg_adr_1y:
	.byte	17
	.global	sandisk_1y
	.align	3
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
	.local	sandisk_sclk0_bak
	.comm	sandisk_sclk0_bak,4,4
	.local	sandisk_sclk1_bak
	.comm	sandisk_sclk1_bak,4,4
	.local	sandisk_toggle_mode_flag
	.comm	sandisk_toggle_mode_flag,4,4
	.text
	.align	2
	.global	sandisk_read_retry_clock_save
	.type	sandisk_read_retry_clock_save, %function
sandisk_read_retry_clock_save:
	stp	x29, x30, [sp, -32]!
	add	x29, sp, 0
	str	x0, [x29, 24]
	ldr	x0, [x29, 24]
	ldr	x0, [x0, 272]
	ldr	w1, [x0, 12]
	adrp	x0, sandisk_sclk1_bak
	add	x3, x0, :lo12:sandisk_sclk1_bak
	adrp	x0, sandisk_sclk0_bak
	add	x2, x0, :lo12:sandisk_sclk0_bak
	adrp	x0, aw_ndfc
	add	x0, x0, :lo12:aw_ndfc
	bl	nand_get_clk
	ldr	x0, [x29, 24]
	ldr	x0, [x0, 272]
	ldr	w1, [x0, 12]
	adrp	x0, aw_ndfc
	add	x0, x0, :lo12:aw_ndfc
	mov	w3, 20
	mov	w2, 10
	bl	nand_set_clk
	mov	w0, 0
	ldp	x29, x30, [sp], 32
	ret
	.size	sandisk_read_retry_clock_save, .-sandisk_read_retry_clock_save
	.align	2
	.global	sandisk_vender_set_param
	.type	sandisk_vender_set_param, %function
sandisk_vender_set_param:
	stp	x29, x30, [sp, -64]!
	add	x29, sp, 0
	str	x0, [x29, 40]
	str	x1, [x29, 32]
	str	x2, [x29, 24]
	str	w3, [x29, 20]
	str	wzr, [x29, 60]
	adrp	x0, sandisk_read_retry_mode
	add	x0, x0, :lo12:sandisk_read_retry_mode
	ldrb	w0, [x0]
	cmp	w0, 48
	beq	.L750
	adrp	x0, sandisk_read_retry_mode
	add	x0, x0, :lo12:sandisk_read_retry_mode
	ldrb	w0, [x0]
	cmp	w0, 49
	bne	.L751
.L750:
	adrp	x0, sandisk_acti_start_cmd
	add	x0, x0, :lo12:sandisk_acti_start_cmd
	mov	w6, 2
	mov	w5, 0
	mov	x4, 0
	mov	x3, 0
	mov	w2, 0
	mov	x1, x0
	ldr	x0, [x29, 40]
	bl	set_cmd_with_nand_bus
	str	w0, [x29, 60]
	adrp	x0, sandisk_read_retry_cmd_19
	add	x0, x0, :lo12:sandisk_read_retry_cmd_19
	mov	w6, 3
	mov	w5, 1
	ldr	x4, [x29, 32]
	ldr	x3, [x29, 24]
	mov	w2, 0
	mov	x1, x0
	ldr	x0, [x29, 40]
	bl	set_cmd_with_nand_bus
	mov	w1, w0
	ldr	w0, [x29, 60]
	orr	w0, w0, w1
	str	w0, [x29, 60]
.L751:
	adrp	x0, sandisk_read_retry_mode
	add	x0, x0, :lo12:sandisk_read_retry_mode
	ldrb	w0, [x0]
	cmp	w0, 50
	bne	.L752
	adrp	x0, sandisk_read_retry_cmd_1y
	add	x0, x0, :lo12:sandisk_read_retry_cmd_1y
	mov	w6, 1
	mov	w5, 4
	ldr	x4, [x29, 32]
	ldr	x3, [x29, 24]
	mov	w2, 1
	mov	x1, x0
	ldr	x0, [x29, 40]
	bl	set_cmd_with_nand_bus
	str	w0, [x29, 60]
.L752:
	ldr	w0, [x29, 60]
	ldp	x29, x30, [sp], 64
	ret
	.size	sandisk_vender_set_param, .-sandisk_vender_set_param
	.align	2
	.global	sandisk_read_retry_clock_recover
	.type	sandisk_read_retry_clock_recover, %function
sandisk_read_retry_clock_recover:
	stp	x29, x30, [sp, -32]!
	add	x29, sp, 0
	str	x0, [x29, 24]
	ldr	x0, [x29, 24]
	ldr	x0, [x0, 272]
	ldr	w1, [x0, 12]
	adrp	x0, sandisk_sclk0_bak
	add	x0, x0, :lo12:sandisk_sclk0_bak
	ldr	w2, [x0]
	adrp	x0, sandisk_sclk1_bak
	add	x0, x0, :lo12:sandisk_sclk1_bak
	ldr	w3, [x0]
	adrp	x0, aw_ndfc
	add	x0, x0, :lo12:aw_ndfc
	bl	nand_set_clk
	mov	w0, 0
	ldp	x29, x30, [sp], 32
	ret
	.size	sandisk_read_retry_clock_recover, .-sandisk_read_retry_clock_recover
	.align	2
	.global	sandisk_end_one_read_retry
	.type	sandisk_end_one_read_retry, %function
sandisk_end_one_read_retry:
	stp	x29, x30, [sp, -32]!
	add	x29, sp, 0
	str	x0, [x29, 24]
	adrp	x0, sandisk_read_retry_mode
	add	x0, x0, :lo12:sandisk_read_retry_mode
	ldrb	w0, [x0]
	cmp	w0, 48
	beq	.L757
	adrp	x0, sandisk_read_retry_mode
	add	x0, x0, :lo12:sandisk_read_retry_mode
	ldrb	w0, [x0]
	cmp	w0, 49
	beq	.L757
	mov	w0, 0
	b	.L758
.L757:
	ldr	x0, [x29, 24]
	bl	nand_enable_chip
	ldr	x0, [x29, 24]
	ldr	x0, [x0, 272]
	bl	ndfc_disable_randomize
	ldr	x0, [x29, 24]
	ldr	x0, [x0, 272]
	bl	ndfc_is_toggle_interface
	cmp	w0, 0
	beq	.L759
	ldr	x0, [x29, 24]
	ldr	x0, [x0, 272]
	bl	ndfc_set_legacy_interface
	adrp	x0, sandisk_toggle_mode_flag
	add	x0, x0, :lo12:sandisk_toggle_mode_flag
	mov	w1, 1
	str	w1, [x0]
.L759:
	adrp	x0, sandisk_read_retry_disable_cmd_19
	add	x0, x0, :lo12:sandisk_read_retry_disable_cmd_19
	mov	w6, 1
	mov	w5, 0
	mov	x4, 0
	mov	x3, 0
	mov	w2, 0
	mov	x1, x0
	ldr	x0, [x29, 24]
	bl	set_cmd_with_nand_bus
	adrp	x0, sandisk_toggle_mode_flag
	add	x0, x0, :lo12:sandisk_toggle_mode_flag
	ldr	w0, [x0]
	cmp	w0, 1
	bne	.L760
	ldr	x0, [x29, 24]
	ldr	x0, [x0, 272]
	bl	ndfc_set_toggle_interface
.L760:
	ldr	x0, [x29, 24]
	bl	nand_disable_chip
	mov	w0, 0
.L758:
	ldp	x29, x30, [sp], 32
	ret
	.size	sandisk_end_one_read_retry, .-sandisk_end_one_read_retry
	.align	2
	.global	sandisk_set_readretry
	.type	sandisk_set_readretry, %function
sandisk_set_readretry:
	stp	x29, x30, [sp, -64]!
	add	x29, sp, 0
	str	x0, [x29, 24]
	strh	w1, [x29, 22]
	str	wzr, [x29, 60]
	ldr	x0, [x29, 24]
	bl	sandisk_read_retry_clock_save
	ldr	x0, [x29, 24]
	bl	nand_enable_chip
	ldr	x0, [x29, 24]
	ldr	x0, [x0, 272]
	bl	ndfc_disable_randomize
	ldr	x0, [x29, 24]
	ldr	x0, [x0, 272]
	bl	ndfc_is_toggle_interface
	cmp	w0, 0
	beq	.L762
	ldr	x0, [x29, 24]
	ldr	x0, [x0, 272]
	bl	ndfc_set_legacy_interface
	adrp	x0, sandisk_toggle_mode_flag
	add	x0, x0, :lo12:sandisk_toggle_mode_flag
	mov	w1, 1
	str	w1, [x0]
.L762:
	adrp	x0, sandisk_read_retry_mode
	add	x0, x0, :lo12:sandisk_read_retry_mode
	ldrb	w0, [x0]
	cmp	w0, 48
	bne	.L763
	ldrh	w0, [x29, 22]
	cmp	w0, 255
	beq	.L764
	ldrh	w0, [x29, 22]
	cmp	w0, 0
	beq	.L765
	ldrh	w0, [x29, 22]
	and	w0, w0, 1
	uxth	w0, w0
	cmp	w0, 0
	beq	.L764
.L765:
	ldr	x0, [x29, 24]
	ldr	w0, [x0, 184]
	uxtw	x1, w0
	mov	x0, x1
	lsl	x0, x0, 1
	add	x0, x0, x1
	adrp	x1, sandisk_19_low
	add	x1, x1, :lo12:sandisk_19_low
	add	x0, x0, x1
	str	x0, [x29, 48]
	adrp	x0, sandisk_read_retry_reg_adr_19
	add	x0, x0, :lo12:sandisk_read_retry_reg_adr_19
	str	x0, [x29, 40]
	mov	w0, 3
	str	w0, [x29, 36]
	adrp	x0, sandisk_read_retry_cycle
	add	x0, x0, :lo12:sandisk_read_retry_cycle
	mov	w1, 16
	strb	w1, [x0]
	b	.L766
.L764:
	ldr	x0, [x29, 24]
	ldr	w0, [x0, 184]
	uxtw	x1, w0
	mov	x0, x1
	lsl	x0, x0, 1
	add	x0, x0, x1
	adrp	x1, sandisk_19_high
	add	x1, x1, :lo12:sandisk_19_high
	add	x0, x0, x1
	str	x0, [x29, 48]
	adrp	x0, sandisk_read_retry_reg_adr_19
	add	x0, x0, :lo12:sandisk_read_retry_reg_adr_19
	str	x0, [x29, 40]
	mov	w0, 3
	str	w0, [x29, 36]
	adrp	x0, sandisk_read_retry_cycle
	add	x0, x0, :lo12:sandisk_read_retry_cycle
	mov	w1, 20
	strb	w1, [x0]
.L766:
	ldr	w3, [x29, 36]
	ldr	x2, [x29, 40]
	ldr	x1, [x29, 48]
	ldr	x0, [x29, 24]
	bl	sandisk_vender_set_param
	str	w0, [x29, 60]
	mov	w2, 0
	mov	w1, -74
	ldr	x0, [x29, 24]
	bl	set_one_cmd
	b	.L767
.L763:
	adrp	x0, sandisk_read_retry_mode
	add	x0, x0, :lo12:sandisk_read_retry_mode
	ldrb	w0, [x0]
	cmp	w0, 49
	bne	.L768
	ldr	x0, [x29, 24]
	ldr	w0, [x0, 184]
	uxtw	x1, w0
	mov	x0, x1
	lsl	x0, x0, 1
	add	x0, x0, x1
	adrp	x1, sandisk_24
	add	x1, x1, :lo12:sandisk_24
	add	x0, x0, x1
	str	x0, [x29, 48]
	adrp	x0, sandisk_read_retry_reg_adr_24
	add	x0, x0, :lo12:sandisk_read_retry_reg_adr_24
	str	x0, [x29, 40]
	mov	w0, 3
	str	w0, [x29, 36]
	ldr	w3, [x29, 36]
	ldr	x2, [x29, 40]
	ldr	x1, [x29, 48]
	ldr	x0, [x29, 24]
	bl	sandisk_vender_set_param
	str	w0, [x29, 60]
	mov	w2, 0
	mov	w1, -74
	ldr	x0, [x29, 24]
	bl	set_one_cmd
	b	.L767
.L768:
	adrp	x0, sandisk_read_retry_mode
	add	x0, x0, :lo12:sandisk_read_retry_mode
	ldrb	w0, [x0]
	cmp	w0, 50
	bne	.L767
	ldr	x0, [x29, 24]
	ldr	w0, [x0, 184]
	uxtw	x0, w0
	lsl	x1, x0, 2
	adrp	x0, sandisk_1y
	add	x0, x0, :lo12:sandisk_1y
	add	x0, x1, x0
	str	x0, [x29, 48]
	adrp	x0, sandisk_read_retry_reg_adr_1y
	add	x0, x0, :lo12:sandisk_read_retry_reg_adr_1y
	str	x0, [x29, 40]
	mov	w0, 1
	str	w0, [x29, 36]
	ldr	w3, [x29, 36]
	ldr	x2, [x29, 40]
	ldr	x1, [x29, 48]
	ldr	x0, [x29, 24]
	bl	sandisk_vender_set_param
	str	w0, [x29, 60]
	mov	w2, 0
	mov	w1, 93
	ldr	x0, [x29, 24]
	bl	set_one_cmd
.L767:
	adrp	x0, sandisk_toggle_mode_flag
	add	x0, x0, :lo12:sandisk_toggle_mode_flag
	ldr	w0, [x0]
	cmp	w0, 1
	bne	.L769
	ldr	x0, [x29, 24]
	ldr	x0, [x0, 272]
	bl	ndfc_set_toggle_interface
.L769:
	ldr	x0, [x29, 24]
	bl	nand_disable_chip
	ldr	x0, [x29, 24]
	bl	sandisk_read_retry_clock_recover
	ldr	w0, [x29, 60]
	ldp	x29, x30, [sp], 64
	ret
	.size	sandisk_set_readretry, .-sandisk_set_readretry
	.section	.rodata
	.align	3
.LC134:
	.string	"sandisk retry!\n"
	.align	3
.LC135:
	.string	"sandisk ReadRetry ok! ch =%d, chip = %d  block = %d, page = %d, RetryCount = %d  \n"
	.text
	.align	2
	.global	sandisk_read_page_end
	.type	sandisk_read_page_end, %function
sandisk_read_page_end:
	stp	x29, x30, [sp, -48]!
	add	x29, sp, 0
	str	x0, [x29, 24]
	str	wzr, [x29, 44]
	str	wzr, [x29, 40]
	adrp	x0, g_nsi
	add	x0, x0, :lo12:g_nsi
	ldr	x2, [x0]
	ldr	x0, [x29, 24]
	ldr	w0, [x0]
	mov	w1, w0
	mov	x0, x2
	bl	nci_get_from_nsi
	str	x0, [x29, 32]
	ldr	x0, [x29, 24]
	bl	generic_read_page_end_not_retry
	str	w0, [x29, 40]
	ldr	w0, [x29, 40]
	cmn	w0, #2
	bne	.L772
	adrp	x0, .LC134
	add	x0, x0, :lo12:.LC134
	bl	nand_print_dbg
	str	wzr, [x29, 44]
	b	.L773
.L779:
	ldr	w1, [x29, 44]
	ldr	x0, [x29, 32]
	str	w1, [x0, 184]
	ldr	x0, [x29, 24]
	ldr	w0, [x0, 8]
	uxth	w0, w0
	mov	w1, w0
	ldr	x0, [x29, 32]
	bl	sandisk_set_readretry
	str	w0, [x29, 40]
	ldr	w0, [x29, 40]
	cmp	w0, 0
	bne	.L781
	ldr	x0, [x29, 24]
	bl	generic_read_page_start
	str	w0, [x29, 40]
	ldr	x0, [x29, 24]
	bl	generic_read_page_end_not_retry
	mov	w1, w0
	ldr	w0, [x29, 40]
	orr	w0, w0, w1
	str	w0, [x29, 40]
	ldr	x0, [x29, 32]
	bl	sandisk_end_one_read_retry
	ldr	x0, [x29, 24]
	ldr	x0, [x0, 24]
	bl	is_nouse_page
	cmp	w0, 1
	bne	.L776
	mov	w0, -2
	str	w0, [x29, 40]
	ldr	x0, [x29, 32]
	ldr	x0, [x0, 272]
	ldr	w1, [x0, 12]
	ldr	x0, [x29, 32]
	ldr	w2, [x0, 28]
	ldr	x0, [x29, 24]
	ldr	w3, [x0, 4]
	ldr	x0, [x29, 24]
	ldr	w4, [x0, 8]
	ldr	x0, [x29, 32]
	ldr	w5, [x0, 184]
	adrp	x0, .LC47
	add	x0, x0, :lo12:.LC47
	bl	nand_print_dbg
	b	.L775
.L776:
	ldr	w0, [x29, 40]
	cmp	w0, 10
	beq	.L777
	ldr	w0, [x29, 40]
	cmp	w0, 0
	bne	.L775
.L777:
	mov	w0, 10
	str	w0, [x29, 40]
	ldr	x0, [x29, 32]
	ldr	x0, [x0, 272]
	ldr	w1, [x0, 12]
	ldr	x0, [x29, 32]
	ldr	w2, [x0, 28]
	ldr	x0, [x29, 24]
	ldr	w3, [x0, 4]
	ldr	x0, [x29, 24]
	ldr	w4, [x0, 8]
	ldr	x0, [x29, 32]
	ldr	w5, [x0, 184]
	adrp	x0, .LC135
	add	x0, x0, :lo12:.LC135
	bl	nand_print_dbg
	b	.L778
.L781:
	nop
.L775:
	ldr	w0, [x29, 44]
	add	w0, w0, 1
	str	w0, [x29, 44]
.L773:
	adrp	x0, sandisk_read_retry_cycle
	add	x0, x0, :lo12:sandisk_read_retry_cycle
	ldrb	w0, [x0]
	mov	w1, w0
	ldr	w0, [x29, 44]
	cmp	w1, w0
	bgt	.L779
.L778:
	ldr	x0, [x29, 32]
	str	wzr, [x0, 184]
	ldr	x0, [x29, 32]
	bl	sandisk_exit_readretry
.L772:
	ldr	w0, [x29, 40]
	ldp	x29, x30, [sp], 48
	ret
	.size	sandisk_read_page_end, .-sandisk_read_page_end
	.section	.rodata
	.align	3
.LC136:
	.string	"sandisk activation seq fail 0x%x\n"
	.text
	.align	2
	.global	sandisk_activation_seq
	.type	sandisk_activation_seq, %function
sandisk_activation_seq:
	stp	x29, x30, [sp, -64]!
	add	x29, sp, 0
	str	x0, [x29, 24]
	adrp	x0, .LC1
	add	x1, x0, :lo12:.LC1
	add	x0, x29, 48
	ldr	x2, [x1]
	str	x2, [x0]
	ldrb	w1, [x1, 8]
	strb	w1, [x0, 8]
	strb	wzr, [x29, 32]
	strb	wzr, [x29, 33]
	strb	wzr, [x29, 34]
	strb	wzr, [x29, 35]
	strb	wzr, [x29, 36]
	strb	wzr, [x29, 37]
	strb	wzr, [x29, 38]
	strb	wzr, [x29, 39]
	strb	wzr, [x29, 40]
	ldr	x0, [x29, 24]
	bl	nand_enable_chip
	ldr	x0, [x29, 24]
	ldr	x0, [x0, 272]
	bl	ndfc_disable_randomize
	adrp	x0, sandisk_acti_start_cmd
	add	x0, x0, :lo12:sandisk_acti_start_cmd
	mov	w6, 2
	mov	w5, 0
	mov	x4, 0
	mov	x3, 0
	mov	w2, 0
	mov	x1, x0
	ldr	x0, [x29, 24]
	bl	set_cmd_with_nand_bus
	str	w0, [x29, 60]
	add	x2, x29, 32
	adrp	x0, sandisk_acti_start_addr
	add	x1, x0, :lo12:sandisk_acti_start_addr
	add	x0, x29, 48
	mov	w6, 9
	mov	w5, 1
	mov	x4, x2
	mov	x3, x1
	mov	w2, 0
	mov	x1, x0
	ldr	x0, [x29, 24]
	bl	set_cmd_with_nand_bus
	mov	w1, w0
	ldr	w0, [x29, 60]
	orr	w0, w0, w1
	str	w0, [x29, 60]
	ldr	x0, [x29, 24]
	bl	nand_disable_chip
	ldr	w0, [x29, 60]
	cmp	w0, 0
	beq	.L783
	adrp	x0, .LC136
	add	x0, x0, :lo12:.LC136
	ldr	w1, [x29, 60]
	bl	nand_print
.L783:
	ldr	w0, [x29, 60]
	ldp	x29, x30, [sp], 64
	ret
	.size	sandisk_activation_seq, .-sandisk_activation_seq
	.section	.rodata
	.align	3
.LC1:
	.byte	83
	.byte	83
	.byte	83
	.byte	83
	.byte	83
	.byte	83
	.byte	83
	.byte	83
	.byte	83
	.text
	.section	.rodata
	.align	3
.LC137:
	.string	"sandisk read retry conut: %d !\n"
	.text
	.align	2
	.global	sandisk_readretry_init
	.type	sandisk_readretry_init, %function
sandisk_readretry_init:
	stp	x29, x30, [sp, -32]!
	add	x29, sp, 0
	str	x0, [x29, 24]
	ldr	x0, [x29, 24]
	str	wzr, [x0, 184]
	ldr	x0, [x29, 24]
	ldr	x0, [x0, 280]
	ldr	w0, [x0, 52]
	lsr	w0, w0, 16
	uxtb	w1, w0
	adrp	x0, sandisk_read_retry_mode
	add	x0, x0, :lo12:sandisk_read_retry_mode
	strb	w1, [x0]
	ldr	x0, [x29, 24]
	ldr	x0, [x0, 280]
	ldr	w0, [x0, 52]
	lsr	w0, w0, 8
	uxtb	w1, w0
	adrp	x0, sandisk_read_retry_cycle
	add	x0, x0, :lo12:sandisk_read_retry_cycle
	strb	w1, [x0]
	ldr	x0, [x29, 24]
	ldr	x0, [x0, 280]
	ldr	w0, [x0, 52]
	uxtb	w1, w0
	adrp	x0, sandisk_read_retry_reg_cnt
	add	x0, x0, :lo12:sandisk_read_retry_reg_cnt
	strb	w1, [x0]
	adrp	x0, sandisk_read_retry_mode
	add	x0, x0, :lo12:sandisk_read_retry_mode
	ldrb	w0, [x0]
	cmp	w0, 48
	beq	.L786
	adrp	x0, sandisk_read_retry_mode
	add	x0, x0, :lo12:sandisk_read_retry_mode
	ldrb	w0, [x0]
	cmp	w0, 49
	bne	.L787
.L786:
	ldr	x0, [x29, 24]
	bl	sandisk_activation_seq
.L787:
	adrp	x0, sandisk_read_retry_mode
	add	x0, x0, :lo12:sandisk_read_retry_mode
	ldrb	w0, [x0]
	cmp	w0, 48
	beq	.L788
	adrp	x0, sandisk_read_retry_mode
	add	x0, x0, :lo12:sandisk_read_retry_mode
	ldrb	w0, [x0]
	cmp	w0, 49
	beq	.L788
	adrp	x0, sandisk_read_retry_mode
	add	x0, x0, :lo12:sandisk_read_retry_mode
	ldrb	w0, [x0]
	cmp	w0, 50
	beq	.L788
	mov	w0, -99
	b	.L789
.L788:
	adrp	x0, sandisk_read_retry_cycle
	add	x0, x0, :lo12:sandisk_read_retry_cycle
	ldrb	w0, [x0]
	mov	w1, w0
	adrp	x0, .LC137
	add	x0, x0, :lo12:.LC137
	bl	nand_print_dbg
	mov	w0, 0
.L789:
	ldp	x29, x30, [sp], 32
	ret
	.size	sandisk_readretry_init, .-sandisk_readretry_init
	.align	2
	.global	sandisk_readretry_exit
	.type	sandisk_readretry_exit, %function
sandisk_readretry_exit:
	sub	sp, sp, #16
	str	x0, [sp, 8]
	mov	w0, 0
	add	sp, sp, 16
	ret
	.size	sandisk_readretry_exit, .-sandisk_readretry_exit
	.align	2
	.global	sandisk_exit_readretry
	.type	sandisk_exit_readretry, %function
sandisk_exit_readretry:
	stp	x29, x30, [sp, -64]!
	add	x29, sp, 0
	str	x0, [x29, 24]
	str	wzr, [x29, 60]
	strb	wzr, [x29, 40]
	strb	wzr, [x29, 41]
	strb	wzr, [x29, 42]
	strb	wzr, [x29, 43]
	ldr	x0, [x29, 24]
	bl	nand_enable_chip
	ldr	x0, [x29, 24]
	ldr	x0, [x0, 272]
	bl	ndfc_disable_randomize
	ldr	x0, [x29, 24]
	ldr	x0, [x0, 272]
	bl	ndfc_is_toggle_interface
	cmp	w0, 0
	beq	.L793
	ldr	x0, [x29, 24]
	ldr	x0, [x0, 272]
	bl	ndfc_set_legacy_interface
	adrp	x0, sandisk_toggle_mode_flag
	add	x0, x0, :lo12:sandisk_toggle_mode_flag
	mov	w1, 1
	str	w1, [x0]
.L793:
	adrp	x0, sandisk_read_retry_mode
	add	x0, x0, :lo12:sandisk_read_retry_mode
	ldrb	w0, [x0]
	cmp	w0, 48
	bne	.L794
	adrp	x0, sandisk_read_retry_reg_adr_19
	add	x0, x0, :lo12:sandisk_read_retry_reg_adr_19
	str	x0, [x29, 48]
	mov	w0, 3
	str	w0, [x29, 44]
	add	x0, x29, 40
	ldr	w3, [x29, 44]
	ldr	x2, [x29, 48]
	mov	x1, x0
	ldr	x0, [x29, 24]
	bl	sandisk_vender_set_param
	str	w0, [x29, 60]
	b	.L795
.L794:
	adrp	x0, sandisk_read_retry_mode
	add	x0, x0, :lo12:sandisk_read_retry_mode
	ldrb	w0, [x0]
	cmp	w0, 49
	bne	.L796
	adrp	x0, sandisk_read_retry_reg_adr_24
	add	x0, x0, :lo12:sandisk_read_retry_reg_adr_24
	str	x0, [x29, 48]
	mov	w0, 3
	str	w0, [x29, 44]
	add	x0, x29, 40
	ldr	w3, [x29, 44]
	ldr	x2, [x29, 48]
	mov	x1, x0
	ldr	x0, [x29, 24]
	bl	sandisk_vender_set_param
	str	w0, [x29, 60]
	b	.L795
.L796:
	adrp	x0, sandisk_read_retry_mode
	add	x0, x0, :lo12:sandisk_read_retry_mode
	ldrb	w0, [x0]
	cmp	w0, 50
	bne	.L795
	mov	w2, 1
	mov	w1, -1
	ldr	x0, [x29, 24]
	bl	set_one_cmd
	adrp	x0, sandisk_read_retry_reg_adr_1y
	add	x0, x0, :lo12:sandisk_read_retry_reg_adr_1y
	str	x0, [x29, 48]
	mov	w0, 1
	str	w0, [x29, 44]
	add	x0, x29, 40
	ldr	w3, [x29, 44]
	ldr	x2, [x29, 48]
	mov	x1, x0
	ldr	x0, [x29, 24]
	bl	sandisk_vender_set_param
	str	w0, [x29, 60]
.L795:
	adrp	x0, sandisk_toggle_mode_flag
	add	x0, x0, :lo12:sandisk_toggle_mode_flag
	ldr	w0, [x0]
	cmp	w0, 1
	bne	.L797
	ldr	x0, [x29, 24]
	ldr	x0, [x0, 272]
	bl	ndfc_set_toggle_interface
.L797:
	ldr	x0, [x29, 24]
	bl	nand_disable_chip
	ldr	w0, [x29, 60]
	ldp	x29, x30, [sp], 64
	ret
	.size	sandisk_exit_readretry, .-sandisk_exit_readretry
	.section	.rodata
	.align	3
.LC138:
	.string	" sandisk_special_init sandisk_read_retry_mode:%d sandisk_read_retry_cycle :%d sandisk_read_retry_reg_cnt %d \n"
	.align	3
.LC139:
	.string	" sandisk_special_init error sandisk_read_retry_mode:%d sandisk_read_retry_cycle :%d sandisk_read_retry_reg_cnt %d \n"
	.text
	.align	2
	.global	sandisk_special_init
	.type	sandisk_special_init, %function
sandisk_special_init:
	stp	x29, x30, [sp, -32]!
	add	x29, sp, 0
	str	wzr, [x29, 28]
	adrp	x0, g_nsi
	add	x0, x0, :lo12:g_nsi
	ldr	x0, [x0]
	ldr	x0, [x0, 8]
	str	x0, [x29, 16]
	b	.L800
.L801:
	ldr	x0, [x29, 16]
	bl	sandisk_readretry_init
	mov	w1, w0
	ldr	w0, [x29, 28]
	orr	w0, w0, w1
	str	w0, [x29, 28]
	ldr	x0, [x29, 16]
	ldr	x0, [x0]
	str	x0, [x29, 16]
.L800:
	ldr	x0, [x29, 16]
	cmp	x0, 0
	bne	.L801
	ldr	w0, [x29, 28]
	cmp	w0, 0
	bne	.L802
	adrp	x0, df_read_page_end
	add	x0, x0, :lo12:df_read_page_end
	adrp	x1, sandisk_read_page_end
	add	x1, x1, :lo12:sandisk_read_page_end
	str	x1, [x0]
	adrp	x0, sandisk_read_retry_mode
	add	x0, x0, :lo12:sandisk_read_retry_mode
	ldrb	w0, [x0]
	mov	w1, w0
	adrp	x0, sandisk_read_retry_cycle
	add	x0, x0, :lo12:sandisk_read_retry_cycle
	ldrb	w0, [x0]
	mov	w2, w0
	adrp	x0, sandisk_read_retry_reg_cnt
	add	x0, x0, :lo12:sandisk_read_retry_reg_cnt
	ldrb	w0, [x0]
	mov	w3, w0
	adrp	x0, .LC138
	add	x0, x0, :lo12:.LC138
	bl	nand_print_dbg
	b	.L803
.L802:
	adrp	x0, sandisk_read_retry_mode
	add	x0, x0, :lo12:sandisk_read_retry_mode
	ldrb	w0, [x0]
	mov	w1, w0
	adrp	x0, sandisk_read_retry_cycle
	add	x0, x0, :lo12:sandisk_read_retry_cycle
	ldrb	w0, [x0]
	mov	w2, w0
	adrp	x0, sandisk_read_retry_reg_cnt
	add	x0, x0, :lo12:sandisk_read_retry_reg_cnt
	ldrb	w0, [x0]
	mov	w3, w0
	adrp	x0, .LC139
	add	x0, x0, :lo12:.LC139
	bl	nand_print
.L803:
	ldr	w0, [x29, 28]
	ldp	x29, x30, [sp], 32
	ret
	.size	sandisk_special_init, .-sandisk_special_init
	.section	.rodata
	.align	3
.LC140:
	.string	" sandisk_special_exit \n"
	.text
	.align	2
	.global	sandisk_special_exit
	.type	sandisk_special_exit, %function
sandisk_special_exit:
	stp	x29, x30, [sp, -32]!
	add	x29, sp, 0
	adrp	x0, g_nsi
	add	x0, x0, :lo12:g_nsi
	ldr	x0, [x0]
	ldr	x0, [x0, 8]
	str	x0, [x29, 24]
	b	.L806
.L807:
	ldr	x0, [x29, 24]
	bl	sandisk_readretry_exit
	ldr	x0, [x29, 24]
	ldr	x0, [x0]
	str	x0, [x29, 24]
.L806:
	ldr	x0, [x29, 24]
	cmp	x0, 0
	bne	.L807
	adrp	x0, .LC140
	add	x0, x0, :lo12:.LC140
	bl	nand_print_dbg
	mov	w0, 0
	ldp	x29, x30, [sp], 32
	ret
	.size	sandisk_special_exit, .-sandisk_special_exit
	.section	.rodata
	.align	3
.LC141:
	.string	"[NE]%s:malloc fail for cache_buf!\n"
	.text
	.align	2
	.global	nftl_sector_cache_init
	.type	nftl_sector_cache_init, %function
nftl_sector_cache_init:
	stp	x29, x30, [sp, -64]!
	add	x29, sp, 0
	str	x0, [x29, 24]
	ldr	x0, [x29, 24]
	add	x0, x0, 2880
	str	x0, [x29, 40]
	ldr	x0, [x29, 40]
	mov	w1, 16
	strh	w1, [x0]
	ldr	x0, [x29, 40]
	strh	wzr, [x0, 2]
	ldr	x0, [x29, 40]
	str	xzr, [x0, 8]
	mov	w0, 8192
	bl	nftl_malloc
	mov	x1, x0
	ldr	x0, [x29, 40]
	str	x1, [x0, 8]
	ldr	x0, [x29, 40]
	ldr	x0, [x0, 8]
	cmp	x0, 0
	bne	.L810
	adrp	x0, __func__.6755
	add	x1, x0, :lo12:__func__.6755
	adrp	x0, .LC141
	add	x0, x0, :lo12:.LC141
	bl	nand_print
	mov	w0, 1
	b	.L811
.L810:
	ldr	x0, [x29, 40]
	str	xzr, [x0, 544]
	ldr	x0, [x29, 40]
	str	xzr, [x0, 552]
	ldr	x0, [x29, 40]
	ldr	x0, [x0, 8]
	str	x0, [x29, 48]
	str	wzr, [x29, 60]
	b	.L812
.L813:
	ldr	w0, [x29, 60]
	uxth	w2, w0
	ldr	x1, [x29, 40]
	ldr	w0, [x29, 60]
	lsl	x0, x0, 5
	add	x0, x1, x0
	add	x0, x0, 16
	mov	w1, w2
	strh	w1, [x0]
	ldr	x1, [x29, 40]
	ldr	w0, [x29, 60]
	lsl	x0, x0, 5
	add	x0, x1, x0
	add	x0, x0, 18
	mov	w1, 255
	strh	w1, [x0]
	ldr	x1, [x29, 40]
	ldr	w0, [x29, 60]
	lsl	x0, x0, 5
	add	x0, x1, x0
	add	x0, x0, 20
	mov	w1, -1
	str	w1, [x0]
	ldr	x1, [x29, 40]
	ldr	w0, [x29, 60]
	add	x0, x0, 1
	lsl	x0, x0, 5
	add	x0, x1, x0
	str	xzr, [x0]
	ldr	x1, [x29, 40]
	ldr	w0, [x29, 60]
	add	x0, x0, 1
	lsl	x0, x0, 5
	add	x0, x1, x0
	add	x0, x0, 8
	str	xzr, [x0]
	ldr	x1, [x29, 40]
	ldr	w0, [x29, 60]
	lsl	x0, x0, 5
	add	x0, x1, x0
	add	x0, x0, 24
	ldr	x1, [x29, 48]
	str	x1, [x0]
	ldr	x0, [x29, 48]
	add	x0, x0, 512
	str	x0, [x29, 48]
	ldr	w0, [x29, 60]
	add	w0, w0, 1
	str	w0, [x29, 60]
.L812:
	ldr	w0, [x29, 60]
	cmp	w0, 15
	bls	.L813
	mov	w0, 0
.L811:
	ldp	x29, x30, [sp], 64
	ret
	.size	nftl_sector_cache_init, .-nftl_sector_cache_init
	.align	2
	.global	nftl_sector_cache_exit
	.type	nftl_sector_cache_exit, %function
nftl_sector_cache_exit:
	stp	x29, x30, [sp, -48]!
	add	x29, sp, 0
	str	x0, [x29, 24]
	ldr	x0, [x29, 24]
	add	x0, x0, 2880
	str	x0, [x29, 40]
	ldr	x0, [x29, 40]
	ldr	x0, [x0, 8]
	bl	nftl_free
	mov	w0, 0
	ldp	x29, x30, [sp], 48
	ret
	.size	nftl_sector_cache_exit, .-nftl_sector_cache_exit
	.align	2
	.global	search_from_sector_cache_write_list
	.type	search_from_sector_cache_write_list, %function
search_from_sector_cache_write_list:
	sub	sp, sp, #32
	str	x0, [sp, 8]
	str	w1, [sp, 4]
	ldr	x0, [sp, 8]
	add	x0, x0, 528
	str	x0, [sp, 24]
	ldr	x0, [sp, 24]
	ldr	x0, [x0, 16]
	str	x0, [sp, 24]
	b	.L817
.L820:
	ldr	x0, [sp, 24]
	ldr	w1, [x0, 4]
	ldr	w0, [sp, 4]
	cmp	w1, w0
	bne	.L818
	ldr	x0, [sp, 24]
	b	.L819
.L818:
	ldr	x0, [sp, 24]
	ldr	x0, [x0, 16]
	str	x0, [sp, 24]
.L817:
	ldr	x0, [sp, 24]
	cmp	x0, 0
	bne	.L820
	mov	x0, 0
.L819:
	add	sp, sp, 32
	ret
	.size	search_from_sector_cache_write_list, .-search_from_sector_cache_write_list
	.align	2
	.global	get_empty_sector_cahce_node
	.type	get_empty_sector_cahce_node, %function
get_empty_sector_cahce_node:
	sub	sp, sp, #32
	str	x0, [sp, 8]
	strh	wzr, [sp, 30]
	b	.L822
.L825:
	ldrh	w0, [sp, 30]
	ldr	x1, [sp, 8]
	sxtw	x0, w0
	lsl	x0, x0, 5
	add	x0, x1, x0
	add	x0, x0, 18
	ldrh	w0, [x0]
	cmp	w0, 255
	bne	.L823
	ldrh	w0, [sp, 30]
	sxtw	x0, w0
	lsl	x0, x0, 5
	add	x0, x0, 16
	ldr	x1, [sp, 8]
	add	x0, x1, x0
	b	.L824
.L823:
	ldrh	w0, [sp, 30]
	add	w0, w0, 1
	strh	w0, [sp, 30]
.L822:
	ldrh	w0, [sp, 30]
	cmp	w0, 15
	bls	.L825
	mov	x0, 0
.L824:
	add	sp, sp, 32
	ret
	.size	get_empty_sector_cahce_node, .-get_empty_sector_cahce_node
	.align	2
	.global	add_to_sector_cache_write_list_tail
	.type	add_to_sector_cache_write_list_tail, %function
add_to_sector_cache_write_list_tail:
	sub	sp, sp, #32
	str	x0, [sp, 8]
	str	x1, [sp]
	ldr	x0, [sp, 8]
	add	x0, x0, 528
	str	x0, [sp, 24]
	b	.L827
.L828:
	ldr	x0, [sp, 24]
	ldr	x0, [x0, 16]
	str	x0, [sp, 24]
.L827:
	ldr	x0, [sp, 24]
	ldr	x0, [x0, 16]
	cmp	x0, 0
	bne	.L828
	ldr	x0, [sp, 24]
	ldr	x1, [sp]
	str	x1, [x0, 16]
	ldr	x0, [sp]
	str	xzr, [x0, 16]
	ldr	x0, [sp]
	ldr	x1, [sp, 24]
	str	x1, [x0, 24]
	ldr	x0, [sp]
	mov	w1, 1
	strh	w1, [x0, 2]
	ldr	x0, [sp, 8]
	ldrh	w0, [x0, 2]
	add	w0, w0, 1
	uxth	w1, w0
	ldr	x0, [sp, 8]
	strh	w1, [x0, 2]
	mov	w0, 0
	add	sp, sp, 32
	ret
	.size	add_to_sector_cache_write_list_tail, .-add_to_sector_cache_write_list_tail
	.align	2
	.global	del_from_sector_cache_write_list
	.type	del_from_sector_cache_write_list, %function
del_from_sector_cache_write_list:
	sub	sp, sp, #32
	str	x0, [sp, 8]
	str	x1, [sp]
	ldr	x0, [sp]
	ldr	x0, [x0, 24]
	str	x0, [sp, 24]
	ldr	x0, [sp]
	ldr	x1, [x0, 16]
	ldr	x0, [sp, 24]
	str	x1, [x0, 16]
	ldr	x0, [sp]
	ldr	x0, [x0, 16]
	cmp	x0, 0
	beq	.L831
	ldr	x0, [sp]
	ldr	x0, [x0, 16]
	ldr	x1, [sp, 24]
	str	x1, [x0, 24]
.L831:
	ldr	x0, [sp]
	str	xzr, [x0, 16]
	ldr	x0, [sp]
	str	xzr, [x0, 24]
	ldr	x0, [sp]
	mov	w1, 255
	strh	w1, [x0, 2]
	ldr	x0, [sp, 8]
	ldrh	w0, [x0, 2]
	sub	w0, w0, #1
	uxth	w1, w0
	ldr	x0, [sp, 8]
	strh	w1, [x0, 2]
	ldr	x0, [sp]
	add	sp, sp, 32
	ret
	.size	del_from_sector_cache_write_list, .-del_from_sector_cache_write_list
	.align	2
	.global	__nand_sector_read
	.type	__nand_sector_read, %function
__nand_sector_read:
	stp	x29, x30, [sp, -80]!
	add	x29, sp, 0
	str	x0, [x29, 40]
	str	w1, [x29, 36]
	str	w2, [x29, 32]
	str	x3, [x29, 24]
	str	wzr, [x29, 76]
	ldr	w0, [x29, 36]
	str	w0, [x29, 68]
	ldr	x0, [x29, 24]
	str	x0, [x29, 56]
	str	wzr, [x29, 72]
	b	.L834
.L835:
	ldr	x2, [x29, 56]
	ldr	w1, [x29, 68]
	ldr	x0, [x29, 40]
	bl	nand_sector_cache_read
	mov	w1, w0
	ldr	w0, [x29, 76]
	orr	w0, w0, w1
	str	w0, [x29, 76]
	ldr	w0, [x29, 68]
	add	w0, w0, 1
	str	w0, [x29, 68]
	ldr	x0, [x29, 56]
	add	x0, x0, 512
	str	x0, [x29, 56]
	ldr	w0, [x29, 72]
	add	w0, w0, 1
	str	w0, [x29, 72]
.L834:
	ldr	w1, [x29, 72]
	ldr	w0, [x29, 32]
	cmp	w1, w0
	bcc	.L835
	ldr	w0, [x29, 76]
	ldp	x29, x30, [sp], 80
	ret
	.size	__nand_sector_read, .-__nand_sector_read
	.align	2
	.global	__nand_sector_write
	.type	__nand_sector_write, %function
__nand_sector_write:
	stp	x29, x30, [sp, -80]!
	add	x29, sp, 0
	str	x0, [x29, 40]
	str	w1, [x29, 36]
	str	w2, [x29, 32]
	str	x3, [x29, 24]
	str	wzr, [x29, 76]
	ldr	w0, [x29, 36]
	str	w0, [x29, 68]
	ldr	x0, [x29, 24]
	str	x0, [x29, 56]
	str	wzr, [x29, 72]
	b	.L838
.L839:
	ldr	x2, [x29, 56]
	ldr	w1, [x29, 68]
	ldr	x0, [x29, 40]
	bl	nand_sector_cache_write
	mov	w1, w0
	ldr	w0, [x29, 76]
	orr	w0, w0, w1
	str	w0, [x29, 76]
	ldr	w0, [x29, 68]
	add	w0, w0, 1
	str	w0, [x29, 68]
	ldr	x0, [x29, 56]
	add	x0, x0, 512
	str	x0, [x29, 56]
	ldr	w0, [x29, 72]
	add	w0, w0, 1
	str	w0, [x29, 72]
.L838:
	ldr	w1, [x29, 72]
	ldr	w0, [x29, 32]
	cmp	w1, w0
	bcc	.L839
	ldr	w0, [x29, 76]
	ldp	x29, x30, [sp], 80
	ret
	.size	__nand_sector_write, .-__nand_sector_write
	.section	.rodata
	.align	3
.LC142:
	.string	"[NE]nand_sector_cache_read 1!\n"
	.text
	.align	2
	.global	nand_sector_cache_read
	.type	nand_sector_cache_read, %function
nand_sector_cache_read:
	stp	x29, x30, [sp, -80]!
	add	x29, sp, 0
	str	x0, [x29, 40]
	str	w1, [x29, 36]
	str	x2, [x29, 24]
	mov	w0, 512
	str	w0, [x29, 76]
	ldr	x0, [x29, 40]
	add	x0, x0, 2880
	str	x0, [x29, 64]
	ldr	w1, [x29, 36]
	ldr	x0, [x29, 64]
	bl	search_from_sector_cache_write_list
	str	x0, [x29, 56]
	ldr	x0, [x29, 56]
	cmp	x0, 0
	beq	.L842
	ldr	x0, [x29, 56]
	ldr	x0, [x0, 8]
	ldr	w2, [x29, 76]
	mov	x1, x0
	ldr	x0, [x29, 24]
	bl	nand_memcpy
	mov	w0, 0
	b	.L843
.L842:
	ldr	x3, [x29, 24]
	mov	w2, 1
	ldr	w1, [x29, 36]
	ldr	x0, [x29, 40]
	bl	__nand_read
	str	w0, [x29, 52]
	ldr	w0, [x29, 52]
	cmp	w0, 0
	beq	.L844
	adrp	x0, .LC142
	add	x0, x0, :lo12:.LC142
	bl	nand_print
	mov	w0, 1
	b	.L843
.L844:
	ldr	w0, [x29, 52]
.L843:
	ldp	x29, x30, [sp], 80
	ret
	.size	nand_sector_cache_read, .-nand_sector_cache_read
	.section	.rodata
	.align	3
.LC143:
	.string	"[NE]nand_sector_cache_write 1!\n"
	.align	3
.LC144:
	.string	"[NE]nand_sector_cache_write 2!\n"
	.text
	.align	2
	.global	nand_sector_cache_write
	.type	nand_sector_cache_write, %function
nand_sector_cache_write:
	stp	x29, x30, [sp, -80]!
	add	x29, sp, 0
	str	x0, [x29, 40]
	str	w1, [x29, 36]
	str	x2, [x29, 24]
	str	wzr, [x29, 68]
	strh	wzr, [x29, 66]
	ldr	x0, [x29, 40]
	add	x0, x0, 2880
	str	x0, [x29, 56]
	mov	w0, 512
	strh	w0, [x29, 66]
	ldr	w1, [x29, 36]
	ldr	x0, [x29, 56]
	bl	search_from_sector_cache_write_list
	str	x0, [x29, 72]
	ldr	x0, [x29, 72]
	cmp	x0, 0
	beq	.L846
	ldr	x1, [x29, 72]
	ldr	x0, [x29, 56]
	bl	del_from_sector_cache_write_list
	ldr	x0, [x29, 72]
	ldr	x0, [x0, 8]
	ldrh	w1, [x29, 66]
	mov	w2, w1
	ldr	x1, [x29, 24]
	bl	nand_memcpy
	ldr	x1, [x29, 72]
	ldr	x0, [x29, 56]
	bl	add_to_sector_cache_write_list_tail
	mov	w0, 0
	b	.L847
.L846:
	ldr	x0, [x29, 56]
	ldrh	w0, [x0, 2]
	cmp	w0, 7
	bls	.L848
	ldr	x0, [x29, 56]
	ldr	x0, [x0, 544]
	mov	x1, x0
	ldr	x0, [x29, 56]
	bl	del_from_sector_cache_write_list
	str	x0, [x29, 72]
	ldr	x0, [x29, 72]
	ldr	w1, [x0, 4]
	ldr	x0, [x29, 72]
	ldr	x0, [x0, 8]
	mov	x3, x0
	mov	w2, 1
	ldr	x0, [x29, 40]
	bl	__nand_write
	str	w0, [x29, 68]
	ldr	w0, [x29, 68]
	cmp	w0, 0
	beq	.L849
	adrp	x0, .LC143
	add	x0, x0, :lo12:.LC143
	bl	nand_print
	mov	w0, 1
	b	.L847
.L848:
	ldr	x0, [x29, 56]
	bl	get_empty_sector_cahce_node
	str	x0, [x29, 72]
	ldr	x0, [x29, 72]
	cmp	x0, 0
	bne	.L849
	adrp	x0, .LC144
	add	x0, x0, :lo12:.LC144
	bl	nand_print
	mov	w0, 1
	b	.L847
.L849:
	ldr	x0, [x29, 72]
	ldr	w1, [x29, 36]
	str	w1, [x0, 4]
	ldr	x0, [x29, 72]
	ldr	x0, [x0, 8]
	ldrh	w1, [x29, 66]
	mov	w2, w1
	ldr	x1, [x29, 24]
	bl	nand_memcpy
	ldr	x1, [x29, 72]
	ldr	x0, [x29, 56]
	bl	add_to_sector_cache_write_list_tail
	mov	w0, 0
.L847:
	ldp	x29, x30, [sp], 80
	ret
	.size	nand_sector_cache_write, .-nand_sector_cache_write
	.section	.rodata
	.align	3
.LC145:
	.string	"[NE]nand_flush_write_cache write error\n"
	.text
	.align	2
	.global	__nand_flush_sector_write_cache
	.type	__nand_flush_sector_write_cache, %function
__nand_flush_sector_write_cache:
	stp	x29, x30, [sp, -64]!
	add	x29, sp, 0
	str	x0, [x29, 24]
	str	w1, [x29, 20]
	ldr	x0, [x29, 24]
	add	x0, x0, 2880
	str	x0, [x29, 48]
	str	wzr, [x29, 60]
	ldr	x0, [x29, 48]
	ldrh	w0, [x0, 2]
	cmp	w0, 0
	beq	.L851
	mov	w0, 1
	str	w0, [x29, 44]
	b	.L853
.L851:
	str	wzr, [x29, 44]
	b	.L853
.L855:
	ldr	x0, [x29, 48]
	ldr	x0, [x0, 544]
	mov	x1, x0
	ldr	x0, [x29, 48]
	bl	del_from_sector_cache_write_list
	str	x0, [x29, 32]
	ldr	x0, [x29, 32]
	ldr	w1, [x0, 4]
	ldr	x0, [x29, 32]
	ldr	x0, [x0, 8]
	mov	x3, x0
	mov	w2, 1
	ldr	x0, [x29, 24]
	bl	__nand_write
	str	w0, [x29, 60]
	ldr	w0, [x29, 20]
	sub	w0, w0, #1
	str	w0, [x29, 20]
	ldr	w0, [x29, 20]
	cmp	w0, 0
	beq	.L859
.L853:
	ldr	x0, [x29, 48]
	ldr	x0, [x0, 544]
	cmp	x0, 0
	bne	.L855
	b	.L854
.L859:
	nop
.L854:
	ldr	w0, [x29, 60]
	cmp	w0, 0
	beq	.L858
	adrp	x0, .LC145
	add	x0, x0, :lo12:.LC145
	bl	nand_print
.L858:
	ldr	w0, [x29, 60]
	ldp	x29, x30, [sp], 64
	ret
	.size	__nand_flush_sector_write_cache, .-__nand_flush_sector_write_cache
	.section	.rodata
	.align	3
	.type	nand_crc16_table, %object
	.size	nand_crc16_table, 512
nand_crc16_table:
	.hword	0
	.hword	-16191
	.hword	-15999
	.hword	320
	.hword	-15615
	.hword	960
	.hword	640
	.hword	-15807
	.hword	-14847
	.hword	1728
	.hword	1920
	.hword	-14527
	.hword	1280
	.hword	-14911
	.hword	-15231
	.hword	1088
	.hword	-13311
	.hword	3264
	.hword	3456
	.hword	-12991
	.hword	3840
	.hword	-12351
	.hword	-12671
	.hword	3648
	.hword	2560
	.hword	-13631
	.hword	-13439
	.hword	2880
	.hword	-14079
	.hword	2496
	.hword	2176
	.hword	-14271
	.hword	-10239
	.hword	6336
	.hword	6528
	.hword	-9919
	.hword	6912
	.hword	-9279
	.hword	-9599
	.hword	6720
	.hword	7680
	.hword	-8511
	.hword	-8319
	.hword	8000
	.hword	-8959
	.hword	7616
	.hword	7296
	.hword	-9151
	.hword	5120
	.hword	-11071
	.hword	-10879
	.hword	5440
	.hword	-10495
	.hword	6080
	.hword	5760
	.hword	-10687
	.hword	-11775
	.hword	4800
	.hword	4992
	.hword	-11455
	.hword	4352
	.hword	-11839
	.hword	-12159
	.hword	4160
	.hword	-4095
	.hword	12480
	.hword	12672
	.hword	-3775
	.hword	13056
	.hword	-3135
	.hword	-3455
	.hword	12864
	.hword	13824
	.hword	-2367
	.hword	-2175
	.hword	14144
	.hword	-2815
	.hword	13760
	.hword	13440
	.hword	-3007
	.hword	15360
	.hword	-831
	.hword	-639
	.hword	15680
	.hword	-255
	.hword	16320
	.hword	16000
	.hword	-447
	.hword	-1535
	.hword	15040
	.hword	15232
	.hword	-1215
	.hword	14592
	.hword	-1599
	.hword	-1919
	.hword	14400
	.hword	10240
	.hword	-5951
	.hword	-5759
	.hword	10560
	.hword	-5375
	.hword	11200
	.hword	10880
	.hword	-5567
	.hword	-4607
	.hword	11968
	.hword	12160
	.hword	-4287
	.hword	11520
	.hword	-4671
	.hword	-4991
	.hword	11328
	.hword	-7167
	.hword	9408
	.hword	9600
	.hword	-6847
	.hword	9984
	.hword	-6207
	.hword	-6527
	.hword	9792
	.hword	8704
	.hword	-7487
	.hword	-7295
	.hword	9024
	.hword	-7935
	.hword	8640
	.hword	8320
	.hword	-8127
	.hword	-24575
	.hword	24768
	.hword	24960
	.hword	-24255
	.hword	25344
	.hword	-23615
	.hword	-23935
	.hword	25152
	.hword	26112
	.hword	-22847
	.hword	-22655
	.hword	26432
	.hword	-23295
	.hword	26048
	.hword	25728
	.hword	-23487
	.hword	27648
	.hword	-21311
	.hword	-21119
	.hword	27968
	.hword	-20735
	.hword	28608
	.hword	28288
	.hword	-20927
	.hword	-22015
	.hword	27328
	.hword	27520
	.hword	-21695
	.hword	26880
	.hword	-22079
	.hword	-22399
	.hword	26688
	.hword	30720
	.hword	-18239
	.hword	-18047
	.hword	31040
	.hword	-17663
	.hword	31680
	.hword	31360
	.hword	-17855
	.hword	-16895
	.hword	32448
	.hword	32640
	.hword	-16575
	.hword	32000
	.hword	-16959
	.hword	-17279
	.hword	31808
	.hword	-19455
	.hword	29888
	.hword	30080
	.hword	-19135
	.hword	30464
	.hword	-18495
	.hword	-18815
	.hword	30272
	.hword	29184
	.hword	-19775
	.hword	-19583
	.hword	29504
	.hword	-20223
	.hword	29120
	.hword	28800
	.hword	-20415
	.hword	20480
	.hword	-28479
	.hword	-28287
	.hword	20800
	.hword	-27903
	.hword	21440
	.hword	21120
	.hword	-28095
	.hword	-27135
	.hword	22208
	.hword	22400
	.hword	-26815
	.hword	21760
	.hword	-27199
	.hword	-27519
	.hword	21568
	.hword	-25599
	.hword	23744
	.hword	23936
	.hword	-25279
	.hword	24320
	.hword	-24639
	.hword	-24959
	.hword	24128
	.hword	23040
	.hword	-25919
	.hword	-25727
	.hword	23360
	.hword	-26367
	.hword	22976
	.hword	22656
	.hword	-26559
	.hword	-30719
	.hword	18624
	.hword	18816
	.hword	-30399
	.hword	19200
	.hword	-29759
	.hword	-30079
	.hword	19008
	.hword	19968
	.hword	-28991
	.hword	-28799
	.hword	20288
	.hword	-29439
	.hword	19904
	.hword	19584
	.hword	-29631
	.hword	17408
	.hword	-31551
	.hword	-31359
	.hword	17728
	.hword	-30975
	.hword	18368
	.hword	18048
	.hword	-31167
	.hword	-32255
	.hword	17088
	.hword	17280
	.hword	-31935
	.hword	16640
	.hword	-32319
	.hword	-32639
	.hword	16448
	.text
	.align	2
	.type	nand_crc16_byte, %function
nand_crc16_byte:
	sub	sp, sp, #16
	strh	w0, [sp, 14]
	strb	w1, [sp, 13]
	ldrh	w0, [sp, 14]
	lsr	w0, w0, 8
	uxth	w1, w0
	ldrh	w2, [sp, 14]
	ldrb	w0, [sp, 13]
	eor	w0, w2, w0
	and	w2, w0, 255
	adrp	x0, nand_crc16_table
	add	x0, x0, :lo12:nand_crc16_table
	sxtw	x2, w2
	ldrh	w0, [x0, x2, lsl 1]
	eor	w0, w1, w0
	uxth	w0, w0
	add	sp, sp, 16
	ret
	.size	nand_crc16_byte, .-nand_crc16_byte
	.align	2
	.global	nand_crc16
	.type	nand_crc16, %function
nand_crc16:
	stp	x29, x30, [sp, -32]!
	add	x29, sp, 0
	strh	w0, [x29, 30]
	str	x1, [x29, 16]
	str	w2, [x29, 24]
	b	.L863
.L864:
	ldr	x0, [x29, 16]
	add	x1, x0, 1
	str	x1, [x29, 16]
	ldrb	w0, [x0]
	mov	w1, w0
	ldrh	w0, [x29, 30]
	bl	nand_crc16_byte
	strh	w0, [x29, 30]
.L863:
	ldr	w0, [x29, 24]
	sub	w1, w0, #1
	str	w1, [x29, 24]
	cmp	w0, 0
	bne	.L864
	ldrh	w0, [x29, 30]
	ldp	x29, x30, [sp], 32
	ret
	.size	nand_crc16, .-nand_crc16
	.section	.rodata
	.align	3
.LC146:
	.string	"[NE]error nand_write_logic_page_no_gc page_no:%u\n"
	.text
	.align	2
	.global	_nftl_nand_write_logic_page
	.type	_nftl_nand_write_logic_page, %function
_nftl_nand_write_logic_page:
	stp	x29, x30, [sp, -48]!
	add	x29, sp, 0
	str	x0, [x29, 40]
	str	w1, [x29, 36]
	str	x2, [x29, 24]
	ldr	x2, [x29, 24]
	ldr	w1, [x29, 36]
	ldr	x0, [x29, 40]
	bl	nand_write_logic_page_no_gc
	cmp	w0, 0
	beq	.L867
	adrp	x0, .LC146
	add	x0, x0, :lo12:.LC146
	ldr	w1, [x29, 36]
	bl	nand_print
	mov	w0, 1
	b	.L868
.L867:
	ldr	x0, [x29, 40]
	bl	garbage_collect
	ldr	x0, [x29, 40]
	bl	all_time_gc
	ldr	x0, [x29, 40]
	bl	do_prio_gc
	mov	w0, 0
.L868:
	ldp	x29, x30, [sp], 48
	ret
	.size	_nftl_nand_write_logic_page, .-_nftl_nand_write_logic_page
	.section	.rodata
	.align	3
.LC147:
	.string	"[ND]logic_page_map_temp NULL!!\n"
	.align	3
.LC148:
	.string	"[NE]read a page: %d READ_RECLAIM!\n"
	.text
	.align	2
	.global	_nftl_nand_read_logic_page
	.type	_nftl_nand_read_logic_page, %function
_nftl_nand_read_logic_page:
	stp	x29, x30, [sp, -112]!
	add	x29, sp, 0
	str	x0, [x29, 40]
	str	w1, [x29, 36]
	str	x2, [x29, 24]
	ldr	w1, [x29, 36]
	ldr	x0, [x29, 40]
	bl	get_logic_page_map
	str	x0, [x29, 104]
	ldr	x0, [x29, 104]
	cmp	x0, 0
	bne	.L870
	adrp	x0, .LC147
	add	x0, x0, :lo12:.LC147
	bl	nand_print_dbg
	mov	w0, 1
	b	.L874
.L870:
	ldr	x0, [x29, 104]
	ldrh	w1, [x0, 2]
	mov	w0, 65535
	cmp	w1, w0
	bne	.L872
	ldr	x0, [x29, 40]
	ldr	x0, [x0, 8]
	ldrh	w0, [x0, 26]
	mov	w2, w0
	mov	w1, -1
	ldr	x0, [x29, 24]
	bl	nand_memset
	mov	w0, 0
	b	.L874
.L872:
	ldr	x0, [x29, 40]
	ldr	w0, [x0, 664]
	uxtb	w0, w0
	and	w0, w0, 1
	uxtb	w2, w0
	ldr	x1, [x29, 104]
	ldrb	w0, [x1, 1]
	bfi	w0, w2, 4, 1
	strb	w0, [x1, 1]
	ldr	x0, [x29, 104]
	ldrh	w1, [x0, 2]
	ldr	x0, [x29, 104]
	ldrh	w0, [x0]
	ubfx	x0, x0, 0, 12
	uxth	w0, w0
	mov	w6, w0
	ldr	x0, [x29, 40]
	ldr	x0, [x0, 8]
	ldrh	w2, [x0, 28]
	add	x3, x29, 72
	add	x0, x29, 48
	mov	x5, x3
	ldr	x4, [x29, 24]
	mov	w3, w2
	mov	w2, w6
	bl	set_physic_op_par
	ldr	x0, [x29, 40]
	ldr	x2, [x0, 3464]
	add	x0, x29, 48
	mov	x1, x0
	ldr	x0, [x29, 40]
	blr	x2
	str	w0, [x29, 100]
	ldr	w0, [x29, 100]
	cmn	w0, #2
	bne	.L873
	ldr	x0, [x29, 104]
	ldrh	w0, [x0, 2]
	mov	w1, w0
	ldr	x0, [x29, 40]
	bl	get_phy_block_addr
	str	x0, [x29, 88]
	mov	w2, 1
	ldr	x1, [x29, 88]
	ldr	x0, [x29, 40]
	bl	add_prio_gc
	adrp	x0, .LC148
	add	x0, x0, :lo12:.LC148
	ldr	w1, [x29, 36]
	bl	nand_print
.L873:
	ldr	x0, [x29, 40]
	bl	do_prio_gc
	ldr	w0, [x29, 100]
.L874:
	ldp	x29, x30, [sp], 112
	ret
	.size	_nftl_nand_read_logic_page, .-_nftl_nand_read_logic_page
	.align	2
	.global	_nftl_discard_logic_page
	.type	_nftl_discard_logic_page, %function
_nftl_discard_logic_page:
	stp	x29, x30, [sp, -48]!
	add	x29, sp, 0
	str	x0, [x29, 24]
	str	w1, [x29, 20]
	mov	x3, 0
	ldr	w2, [x29, 20]
	mov	w1, 4369
	movk	w1, 0xaa11, lsl 16
	ldr	x0, [x29, 24]
	bl	write_data_to_page
	str	w0, [x29, 44]
	ldr	w0, [x29, 44]
	ldp	x29, x30, [sp], 48
	ret
	.size	_nftl_discard_logic_page, .-_nftl_discard_logic_page
	.section	.rodata
	.align	3
.LC149:
	.string	"[NE]no free block\n"
	.align	3
.LC150:
	.string	"[NE]do_write_error erase error sb:%d!\n"
	.align	3
.LC151:
	.string	"[NE]do_write_error sb@%d nftl_nand_copy_page error!\n"
	.text
	.align	2
	.global	do_write_error
	.type	do_write_error, %function
do_write_error:
	stp	x29, x30, [sp, -96]!
	add	x29, sp, 0
	str	x0, [x29, 24]
	str	x1, [x29, 16]
.L878:
	ldr	x0, [x29, 24]
	bl	out_phy_block_from_free_list
	str	x0, [x29, 80]
	ldr	x0, [x29, 80]
	cmp	x0, 0
	bne	.L879
	adrp	x0, .LC149
	add	x0, x0, :lo12:.LC149
	bl	nand_print
	mov	w0, 1
	b	.L891
.L879:
	ldr	x0, [x29, 80]
	ldrh	w1, [x0]
	ldr	x0, [x29, 24]
	ldr	x0, [x0, 8]
	ldrh	w2, [x0, 28]
	add	x0, x29, 40
	mov	x5, 0
	mov	x4, 0
	mov	w3, w2
	mov	w2, 0
	bl	set_physic_op_par
	ldr	x0, [x29, 24]
	ldr	x2, [x0, 3456]
	add	x0, x29, 40
	mov	x1, x0
	ldr	x0, [x29, 24]
	blr	x2
	str	w0, [x29, 76]
	ldr	x0, [x29, 80]
	strh	wzr, [x0, 4]
	ldr	x0, [x29, 80]
	ldrh	w0, [x0, 6]
	add	w0, w0, 1
	uxth	w1, w0
	ldr	x0, [x29, 80]
	strh	w1, [x0, 6]
	ldr	w0, [x29, 76]
	cmp	w0, 0
	beq	.L881
	ldr	x0, [x29, 24]
	ldr	x2, [x0, 3496]
	add	x0, x29, 40
	mov	x1, x0
	ldr	x0, [x29, 24]
	blr	x2
	ldr	x0, [x29, 80]
	ldrh	w0, [x0]
	mov	w1, w0
	adrp	x0, .LC150
	add	x0, x0, :lo12:.LC150
	bl	nand_print
	ldr	x0, [x29, 24]
	ldrh	w1, [x0, 30]
	ldr	x0, [x29, 24]
	ldrh	w0, [x0, 680]
	lsr	w0, w0, 2
	uxth	w0, w0
	cmp	w1, w0
	bls	.L882
	b	.L878
.L882:
	adrp	x0, .LC149
	add	x0, x0, :lo12:.LC149
	bl	nand_print
	mov	w0, 1
	b	.L891
.L881:
	ldr	x0, [x29, 16]
	ldr	x0, [x0]
	ldr	w0, [x0, 8]
	add	w1, w0, 1
	ldr	x0, [x29, 80]
	str	w1, [x0, 8]
	str	wzr, [x29, 92]
	b	.L883
.L887:
	ldr	x0, [x29, 16]
	ldr	x1, [x0, 24]
	ldr	w0, [x29, 92]
	lsl	x0, x0, 2
	add	x0, x1, x0
	ldr	w0, [x0]
	str	w0, [x29, 72]
	ldr	w0, [x29, 72]
	cmn	w0, #1
	beq	.L892
	ldr	x0, [x29, 24]
	ldr	x5, [x0, 3480]
	ldr	x0, [x29, 16]
	ldr	x1, [x0]
	ldr	x0, [x29, 24]
	ldr	x0, [x0, 512]
	ldr	w2, [x29, 92]
	uxth	w2, w2
	mov	w4, w2
	mov	x3, x0
	ldr	x2, [x29, 80]
	ldr	x0, [x29, 24]
	blr	x5
	str	w0, [x29, 76]
	ldr	w0, [x29, 76]
	cmp	w0, 0
	beq	.L885
	ldr	x0, [x29, 80]
	ldrh	w0, [x0]
	mov	w1, w0
	adrp	x0, .LC151
	add	x0, x0, :lo12:.LC151
	bl	nand_print
	ldr	x0, [x29, 80]
	ldrh	w1, [x0]
	ldr	x0, [x29, 24]
	ldr	x0, [x0, 8]
	ldrh	w2, [x0, 28]
	add	x0, x29, 40
	mov	x5, 0
	mov	x4, 0
	mov	w3, w2
	mov	w2, 0
	bl	set_physic_op_par
	ldr	x0, [x29, 24]
	ldr	x2, [x0, 3496]
	add	x0, x29, 40
	mov	x1, x0
	ldr	x0, [x29, 24]
	blr	x2
	b	.L878
.L885:
	ldr	w0, [x29, 92]
	add	w0, w0, 1
	str	w0, [x29, 92]
.L883:
	ldr	x0, [x29, 16]
	ldrh	w0, [x0, 56]
	mov	w1, w0
	ldr	w0, [x29, 92]
	cmp	w1, w0
	bhi	.L887
	b	.L886
.L892:
	nop
.L886:
	ldr	x0, [x29, 16]
	ldr	x0, [x0]
	ldrh	w1, [x0, 4]
	ldr	x0, [x29, 80]
	strh	w1, [x0, 4]
	ldr	x0, [x29, 16]
	ldr	x0, [x0]
	ldrh	w1, [x0]
	ldr	x0, [x29, 24]
	ldr	x0, [x0, 8]
	ldrh	w2, [x0, 28]
	add	x0, x29, 40
	mov	x5, 0
	mov	x4, 0
	mov	w3, w2
	mov	w2, 0
	bl	set_physic_op_par
	ldr	x0, [x29, 16]
	ldr	x1, [x0]
	ldr	x0, [x29, 16]
	ldr	x0, [x0]
	ldrh	w0, [x0, 6]
	add	w0, w0, 1
	uxth	w0, w0
	mov	w2, w0
	ldr	x0, [x29, 24]
	bl	erase_block
	ldr	x0, [x29, 16]
	ldr	x0, [x0]
	ldrh	w1, [x0, 2]
	ldr	x0, [x29, 80]
	strh	w1, [x0, 2]
	ldr	x0, [x29, 16]
	ldr	x1, [x29, 80]
	str	x1, [x0]
	str	wzr, [x29, 92]
	b	.L888
.L890:
	ldr	x0, [x29, 16]
	ldr	x1, [x0, 24]
	ldr	w0, [x29, 92]
	lsl	x0, x0, 2
	add	x0, x1, x0
	ldr	w0, [x0]
	str	w0, [x29, 72]
	ldr	w0, [x29, 72]
	cmn	w0, #1
	beq	.L889
	ldr	w1, [x29, 72]
	ldr	x0, [x29, 24]
	bl	get_logic_page_map
	str	x0, [x29, 64]
	ldr	x0, [x29, 64]
	cmp	x0, 0
	beq	.L889
	ldr	x0, [x29, 16]
	ldr	x0, [x0]
	ldrh	w1, [x0]
	ldr	x0, [x29, 64]
	strh	w1, [x0, 2]
	ldr	w0, [x29, 92]
	uxth	w0, w0
	and	w0, w0, 4095
	uxth	w2, w0
	ldr	x1, [x29, 64]
	ldrh	w0, [x1]
	bfi	w0, w2, 0, 12
	strh	w0, [x1]
.L889:
	ldr	w0, [x29, 92]
	add	w0, w0, 1
	str	w0, [x29, 92]
.L888:
	ldr	x0, [x29, 16]
	ldrh	w0, [x0, 56]
	mov	w1, w0
	ldr	w0, [x29, 92]
	cmp	w1, w0
	bhi	.L890
	mov	w0, 0
.L891:
	ldp	x29, x30, [sp], 96
	ret
	.size	do_write_error, .-do_write_error
	.section	.rodata
	.align	3
.LC152:
	.string	"[NE]do after write error1!\n"
	.align	3
.LC153:
	.string	"[NE]do_before_write error!\n"
	.align	3
.LC154:
	.string	"[NE]write data to page error!\n"
	.align	3
.LC155:
	.string	"[NE]do after write error2!\n"
	.text
	.align	2
	.global	nand_write_data_page
	.type	nand_write_data_page, %function
nand_write_data_page:
	stp	x29, x30, [sp, -48]!
	add	x29, sp, 0
	str	x0, [x29, 40]
	str	w1, [x29, 36]
	str	w2, [x29, 32]
	str	x3, [x29, 24]
	ldr	x0, [x29, 40]
	add	x0, x0, 528
	mov	x1, x0
	ldr	x0, [x29, 40]
	bl	current_block_at_last_page
	cmp	w0, 1
	bne	.L894
	ldr	x0, [x29, 40]
	bl	do_after_write
	cmp	w0, 0
	beq	.L894
	adrp	x0, .LC152
	add	x0, x0, :lo12:.LC152
	bl	nand_print
	mov	w0, 1
	b	.L895
.L894:
	ldr	x0, [x29, 40]
	bl	do_before_write
	cmp	w0, 0
	beq	.L896
	adrp	x0, .LC153
	add	x0, x0, :lo12:.LC153
	bl	nand_print
	mov	w0, 1
	b	.L895
.L896:
	ldr	x3, [x29, 24]
	ldr	w2, [x29, 32]
	ldr	w1, [x29, 36]
	ldr	x0, [x29, 40]
	bl	write_data_to_page
	cmp	w0, 0
	beq	.L897
	adrp	x0, .LC154
	add	x0, x0, :lo12:.LC154
	bl	nand_print
	mov	w0, 1
	b	.L895
.L897:
	ldr	x0, [x29, 40]
	bl	do_after_write
	cmp	w0, 0
	beq	.L898
	adrp	x0, .LC155
	add	x0, x0, :lo12:.LC155
	bl	nand_print
	mov	w0, 1
	b	.L895
.L898:
	mov	w0, 0
.L895:
	ldp	x29, x30, [sp], 48
	ret
	.size	nand_write_data_page, .-nand_write_data_page
	.align	2
	.global	nand_write_logic_page_no_gc
	.type	nand_write_logic_page_no_gc, %function
nand_write_logic_page_no_gc:
	stp	x29, x30, [sp, -64]!
	add	x29, sp, 0
	str	x0, [x29, 40]
	str	w1, [x29, 36]
	str	x2, [x29, 24]
	ldr	x3, [x29, 24]
	ldr	w2, [x29, 36]
	mov	w1, 192
	ldr	x0, [x29, 40]
	bl	nand_write_data_page
	str	w0, [x29, 60]
	ldr	w0, [x29, 60]
	ldp	x29, x30, [sp], 64
	ret
	.size	nand_write_logic_page_no_gc, .-nand_write_logic_page_no_gc
	.section	.rodata
	.align	3
.LC156:
	.string	"[NE]get_new_current_block error3!\n"
	.align	3
.LC157:
	.string	"[NE]get_new_current_block error4!\n"
	.text
	.align	2
	.global	do_before_write
	.type	do_before_write, %function
do_before_write:
	stp	x29, x30, [sp, -32]!
	add	x29, sp, 0
	str	x0, [x29, 24]
	b	.L902
.L907:
	ldr	x0, [x29, 24]
	ldr	x1, [x0, 528]
	ldr	x0, [x29, 24]
	ldr	x0, [x0, 528]
	ldr	w0, [x0, 8]
	mov	w2, w0
	ldr	x0, [x29, 24]
	bl	new_block_init_for_write
	cmp	w0, 0
	beq	.L913
	ldr	x0, [x29, 24]
	ldr	x0, [x0, 528]
	ldr	w1, [x0, 8]
	sub	w1, w1, #1
	str	w1, [x0, 8]
	ldr	x0, [x29, 24]
	ldrb	w0, [x0, 25]
	and	w0, w0, 1
	cmp	w0, 0
	beq	.L904
	ldr	x0, [x29, 24]
	ldr	x0, [x0, 528]
	ldr	w1, [x0, 8]
	sub	w1, w1, #1
	str	w1, [x0, 8]
.L904:
	ldr	x0, [x29, 24]
	add	x0, x0, 528
	mov	x1, x0
	ldr	x0, [x29, 24]
	bl	get_new_current_block
	cmp	w0, 0
	beq	.L902
	adrp	x0, .LC156
	add	x0, x0, :lo12:.LC156
	bl	nand_print
	mov	w0, 1
	b	.L905
.L902:
	ldr	x0, [x29, 24]
	add	x0, x0, 528
	mov	x1, x0
	ldr	x0, [x29, 24]
	bl	current_block_at_first_page
	cmp	w0, 1
	beq	.L907
	b	.L906
.L913:
	nop
.L906:
	ldr	x0, [x29, 24]
	ldrb	w0, [x0, 25]
	and	w0, w0, 1
	cmp	w0, 0
	beq	.L908
	b	.L909
.L912:
	ldr	x0, [x29, 24]
	ldr	x1, [x0, 592]
	ldr	x0, [x29, 24]
	ldr	x0, [x0, 592]
	ldr	w0, [x0, 8]
	mov	w2, w0
	ldr	x0, [x29, 24]
	bl	new_block_init_for_write
	cmp	w0, 0
	beq	.L914
	ldr	x0, [x29, 24]
	ldr	x0, [x0, 592]
	ldr	w1, [x0, 8]
	sub	w1, w1, #1
	str	w1, [x0, 8]
	ldr	x0, [x29, 24]
	ldrb	w0, [x0, 25]
	and	w0, w0, 1
	cmp	w0, 0
	beq	.L911
	ldr	x0, [x29, 24]
	ldr	x0, [x0, 592]
	ldr	w1, [x0, 8]
	sub	w1, w1, #1
	str	w1, [x0, 8]
.L911:
	ldr	x0, [x29, 24]
	add	x0, x0, 592
	mov	x1, x0
	ldr	x0, [x29, 24]
	bl	get_new_current_block
	cmp	w0, 0
	beq	.L909
	adrp	x0, .LC157
	add	x0, x0, :lo12:.LC157
	bl	nand_print
	mov	w0, 1
	b	.L905
.L909:
	ldr	x0, [x29, 24]
	add	x0, x0, 592
	mov	x1, x0
	ldr	x0, [x29, 24]
	bl	current_block_at_first_page
	cmp	w0, 0
	bne	.L912
	b	.L908
.L914:
	nop
.L908:
	mov	w0, 0
.L905:
	ldp	x29, x30, [sp], 32
	ret
	.size	do_before_write, .-do_before_write
	.section	.rodata
	.align	3
.LC158:
	.string	"[NE]write_phy_page_map_to_current_block error!\n"
	.align	3
.LC159:
	.string	"[NE]get_new_current_block error1!\n"
	.align	3
.LC160:
	.string	"[NE]get_new_current_block error2!\n"
	.text
	.align	2
	.global	do_after_write
	.type	do_after_write, %function
do_after_write:
	stp	x29, x30, [sp, -32]!
	add	x29, sp, 0
	str	x0, [x29, 24]
	ldr	x0, [x29, 24]
	add	x0, x0, 528
	mov	x1, x0
	ldr	x0, [x29, 24]
	bl	current_block_at_last_page
	cmp	w0, 1
	bne	.L916
	ldr	x0, [x29, 24]
	add	x0, x0, 528
	mov	x1, x0
	ldr	x0, [x29, 24]
	bl	write_phy_page_map_to_current_block
	cmp	w0, 0
	beq	.L917
	adrp	x0, .LC158
	add	x0, x0, :lo12:.LC158
	bl	nand_print
	mov	w0, 1
	b	.L918
.L917:
	ldr	x0, [x29, 24]
	ldrb	w0, [x0, 25]
	and	w0, w0, 1
	cmp	w0, 0
	beq	.L919
	ldr	x0, [x29, 24]
	add	x0, x0, 592
	mov	x1, x0
	ldr	x0, [x29, 24]
	bl	write_phy_page_map_to_current_block
	cmp	w0, 0
	beq	.L919
	adrp	x0, .LC158
	add	x0, x0, :lo12:.LC158
	bl	nand_print
.L919:
	ldr	x0, [x29, 24]
	add	x0, x0, 528
	mov	x1, x0
	ldr	x0, [x29, 24]
	bl	get_new_current_block
	cmp	w0, 0
	beq	.L920
	adrp	x0, .LC159
	add	x0, x0, :lo12:.LC159
	bl	nand_print
	mov	w0, 1
	b	.L918
.L920:
	ldr	x0, [x29, 24]
	ldrb	w0, [x0, 25]
	and	w0, w0, 1
	cmp	w0, 0
	beq	.L921
	ldr	x0, [x29, 24]
	add	x0, x0, 592
	mov	x1, x0
	ldr	x0, [x29, 24]
	bl	get_new_current_block
	cmp	w0, 0
	beq	.L921
	adrp	x0, .LC160
	add	x0, x0, :lo12:.LC160
	bl	nand_print
	mov	w0, 1
	b	.L918
.L921:
	ldr	x0, [x29, 24]
	ldr	w0, [x0, 668]
	add	w1, w0, 1
	ldr	x0, [x29, 24]
	str	w1, [x0, 668]
.L916:
	mov	w0, 0
.L918:
	ldp	x29, x30, [sp], 32
	ret
	.size	do_after_write, .-do_after_write
	.section	.rodata
	.align	3
.LC161:
	.string	"[NE]write logic data to current block error\n"
	.text
	.align	2
	.global	write_data_to_page
	.type	write_data_to_page, %function
write_data_to_page:
	stp	x29, x30, [sp, -48]!
	add	x29, sp, 0
	str	x0, [x29, 40]
	str	w1, [x29, 36]
	str	w2, [x29, 32]
	str	x3, [x29, 24]
	ldr	x0, [x29, 40]
	add	x0, x0, 528
	ldr	x4, [x29, 24]
	ldr	w3, [x29, 32]
	ldr	w2, [x29, 36]
	mov	x1, x0
	ldr	x0, [x29, 40]
	bl	write_logic_data_to_current_block
	cmp	w0, 0
	beq	.L923
	adrp	x0, .LC161
	add	x0, x0, :lo12:.LC161
	bl	nand_print
	mov	w0, 1
	b	.L924
.L923:
	ldr	x0, [x29, 40]
	ldrb	w0, [x0, 25]
	and	w0, w0, 1
	cmp	w0, 0
	beq	.L925
	ldr	x0, [x29, 40]
	add	x0, x0, 592
	ldr	x4, [x29, 24]
	ldr	w3, [x29, 32]
	ldr	w2, [x29, 36]
	mov	x1, x0
	ldr	x0, [x29, 40]
	bl	write_logic_data_to_current_block
	cmp	w0, 0
	beq	.L925
	adrp	x0, .LC161
	add	x0, x0, :lo12:.LC161
	bl	nand_print
	mov	w0, 1
	b	.L924
.L925:
	mov	w0, 0
.L924:
	ldp	x29, x30, [sp], 48
	ret
	.size	write_data_to_page, .-write_data_to_page
	.section	.rodata
	.align	3
.LC162:
	.string	"[ND]last block can not used to logic page %d!!\n"
	.align	3
.LC163:
	.string	"[ND]last block can not used to power down page %d!!\n"
	.align	3
.LC164:
	.string	"[ND]write_phy_data_to_current_block write error %d!\n"
	.align	3
.LC165:
	.string	"[NE]do_write_error error\n"
	.text
	.align	2
	.global	write_phy_data_to_current_block
	.type	write_phy_data_to_current_block, %function
write_phy_data_to_current_block:
	stp	x29, x30, [sp, -112]!
	add	x29, sp, 0
	str	x0, [x29, 40]
	str	x1, [x29, 32]
	str	x2, [x29, 24]
	str	w3, [x29, 20]
	str	w4, [x29, 16]
	strh	wzr, [x29, 104]
	strh	wzr, [x29, 106]
	add	x0, x29, 80
	mov	w2, 16
	mov	w1, -91
	bl	nand_memset
	ldr	x0, [x29, 40]
	ldr	x0, [x0, 8]
	ldrb	w0, [x0, 8]
	uxth	w0, w0
	ubfiz	w0, w0, 9, 7
	strh	w0, [x29, 106]
	ldr	w1, [x29, 16]
	mov	w0, -1431633921
	cmp	w1, w0
	bne	.L927
	ldr	x0, [x29, 40]
	ldr	w0, [x0, 560]
	strh	w0, [x29, 106]
.L927:
	ldrh	w0, [x29, 106]
	mov	w2, w0
	ldr	x1, [x29, 24]
	mov	w0, -1
	bl	nand_crc16
	strh	w0, [x29, 104]
	ldrh	w0, [x29, 104]
	lsr	w0, w0, 8
	uxth	w0, w0
	uxtb	w0, w0
	strb	w0, [x29, 92]
	ldrh	w0, [x29, 104]
	uxtb	w0, w0
	strb	w0, [x29, 93]
	str	wzr, [x29, 108]
.L928:
	ldr	x0, [x29, 32]
	ldr	x0, [x0]
	ldrh	w1, [x0]
	ldr	x0, [x29, 32]
	ldrh	w2, [x0, 56]
	ldr	x0, [x29, 40]
	ldr	x0, [x0, 8]
	ldrh	w3, [x0, 28]
	add	x4, x29, 80
	add	x0, x29, 56
	mov	x5, x4
	ldr	x4, [x29, 24]
	bl	set_physic_op_par
	ldr	w1, [x29, 16]
	mov	w0, -1431633921
	cmp	w1, w0
	bne	.L929
	ldr	x0, [x29, 32]
	ldr	x0, [x0]
	ldr	w0, [x0, 8]
	mov	w2, w0
	ldr	x0, [x29, 32]
	ldr	x0, [x0]
	ldrh	w1, [x0, 6]
	add	x0, x29, 80
	mov	w4, w1
	mov	w3, w2
	mov	w2, -1431633921
	mov	x1, x0
	ldr	x0, [x29, 40]
	bl	set_oob_special_page
	b	.L930
.L929:
	ldr	w0, [x29, 16]
	cmp	w0, 192
	bne	.L931
	ldr	x0, [x29, 32]
	ldrh	w0, [x0, 56]
	mov	w1, w0
	ldr	x0, [x29, 40]
	ldr	x0, [x0, 8]
	ldrh	w0, [x0, 20]
	sub	w0, w0, #1
	cmp	w1, w0
	bne	.L932
	ldr	x0, [x29, 32]
	ldr	x0, [x0]
	ldrh	w0, [x0]
	mov	w1, w0
	adrp	x0, .LC162
	add	x0, x0, :lo12:.LC162
	bl	nand_print_dbg
.L932:
	ldr	x0, [x29, 32]
	ldr	x0, [x0]
	ldr	w0, [x0, 8]
	mov	w2, w0
	ldr	x0, [x29, 32]
	ldr	x0, [x0]
	ldrh	w1, [x0, 6]
	add	x0, x29, 80
	mov	w4, w1
	mov	w3, w2
	ldr	w2, [x29, 20]
	mov	x1, x0
	ldr	x0, [x29, 40]
	bl	set_oob_logic_page
	b	.L930
.L931:
	ldr	w1, [x29, 16]
	mov	w0, 13107
	movk	w0, 0xaa11, lsl 16
	cmp	w1, w0
	bne	.L930
	ldr	x0, [x29, 32]
	ldrh	w0, [x0, 56]
	mov	w1, w0
	ldr	x0, [x29, 40]
	ldr	x0, [x0, 8]
	ldrh	w0, [x0, 20]
	sub	w0, w0, #1
	cmp	w1, w0
	bne	.L933
	ldr	x0, [x29, 32]
	ldr	x0, [x0]
	ldrh	w0, [x0]
	mov	w1, w0
	adrp	x0, .LC163
	add	x0, x0, :lo12:.LC163
	bl	nand_print_dbg
.L933:
	ldr	x0, [x29, 32]
	ldr	x0, [x0]
	ldr	w0, [x0, 8]
	mov	w2, w0
	ldr	x0, [x29, 32]
	ldr	x0, [x0]
	ldrh	w1, [x0, 6]
	add	x0, x29, 80
	mov	w4, w1
	mov	w3, w2
	mov	w2, 13107
	movk	w2, 0xaa11, lsl 16
	mov	x1, x0
	ldr	x0, [x29, 40]
	bl	set_oob_special_page
.L930:
	ldr	x0, [x29, 32]
	ldr	x0, [x0]
	mov	w1, 1
	strh	w1, [x0, 2]
	ldr	x0, [x29, 40]
	ldr	x2, [x0, 3472]
	add	x0, x29, 56
	mov	x1, x0
	ldr	x0, [x29, 40]
	blr	x2
	str	w0, [x29, 100]
	ldr	w0, [x29, 100]
	cmp	w0, 0
	beq	.L934
	ldr	w0, [x29, 108]
	cmp	w0, 1
	bgt	.L934
	ldrh	w0, [x29, 58]
	mov	w1, w0
	adrp	x0, .LC164
	add	x0, x0, :lo12:.LC164
	bl	nand_print_dbg
	ldr	x1, [x29, 32]
	ldr	x0, [x29, 40]
	bl	do_write_error
	cmp	w0, 0
	bne	.L935
	ldr	w0, [x29, 108]
	add	w0, w0, 1
	str	w0, [x29, 108]
	b	.L928
.L935:
	adrp	x0, .LC165
	add	x0, x0, :lo12:.LC165
	bl	nand_print
	mov	w0, 1
	b	.L937
.L934:
	mov	w0, 0
.L937:
	ldp	x29, x30, [sp], 112
	ret
	.size	write_phy_data_to_current_block, .-write_phy_data_to_current_block
	.section	.rodata
	.align	3
.LC166:
	.string	"[NE]write_phy_data_to_current_block error1\n"
	.text
	.align	2
	.global	write_phy_page_map_to_current_block
	.type	write_phy_page_map_to_current_block, %function
write_phy_page_map_to_current_block:
	stp	x29, x30, [sp, -48]!
	add	x29, sp, 0
	str	x0, [x29, 24]
	str	x1, [x29, 16]
	ldr	x0, [x29, 24]
	add	x1, x0, 592
	ldr	x0, [x29, 16]
	cmp	x1, x0
	bne	.L939
	ldr	x0, [x29, 24]
	ldr	x3, [x0, 632]
	ldr	x0, [x29, 24]
	ldr	x0, [x0, 568]
	mov	w2, 320
	mov	x1, x0
	mov	x0, x3
	bl	nand_memcpy
.L939:
	ldr	x0, [x29, 16]
	ldr	x0, [x0, 8]
	mov	w4, -1431633921
	mov	w3, 0
	mov	x2, x0
	ldr	x1, [x29, 16]
	ldr	x0, [x29, 24]
	bl	write_phy_data_to_current_block
	str	w0, [x29, 44]
	ldr	w0, [x29, 44]
	cmp	w0, 0
	beq	.L940
	adrp	x0, .LC166
	add	x0, x0, :lo12:.LC166
	bl	nand_print
	ldr	w0, [x29, 44]
	b	.L941
.L940:
	ldr	x0, [x29, 16]
	ldrh	w0, [x0, 56]
	add	w0, w0, 1
	uxth	w1, w0
	ldr	x0, [x29, 16]
	strh	w1, [x0, 56]
	ldr	x0, [x29, 16]
	ldr	x0, [x0]
	ldrh	w1, [x0, 4]
	add	w1, w1, 1
	uxth	w1, w1
	strh	w1, [x0, 4]
	ldr	x0, [x29, 16]
	add	x1, x0, 8
	ldr	x0, [x29, 16]
	ldr	x2, [x0]
	ldr	x0, [x29, 16]
	ldrh	w0, [x0, 56]
	mov	w3, w0
	ldr	x0, [x29, 24]
	bl	check_invalid_page_right
	ldr	x0, [x29, 16]
	ldrh	w1, [x0, 56]
	ldr	x0, [x29, 24]
	ldr	x0, [x0, 8]
	ldrh	w0, [x0, 20]
	cmp	w1, w0
	bne	.L942
	ldr	x0, [x29, 16]
	ldr	x0, [x0]
	mov	x1, x0
	ldr	x0, [x29, 24]
	bl	put_phy_block_to_invalid_page_list
	ldr	x0, [x29, 24]
	bl	adjust_invaild_list
.L942:
	mov	w0, 0
.L941:
	ldp	x29, x30, [sp], 48
	ret
	.size	write_phy_page_map_to_current_block, .-write_phy_page_map_to_current_block
	.section	.rodata
	.align	3
.LC167:
	.string	"[ND]logic_page_map NULL!!\n"
	.align	3
.LC168:
	.string	"[ND]invalid_page_count more than pages_per_blk at write logic data to current block:%d!!\n"
	.align	3
.LC169:
	.string	"[NE]write_phy_data_to_current_block error2\n"
	.align	3
.LC170:
	.string	"[NE]invalid_page_count more than pages_per_blk at write logic data to current block:%d!!\n"
	.align	3
.LC171:
	.string	"[NE]write phy data to current block error2\n"
	.text
	.align	2
	.global	write_logic_data_to_current_block
	.type	write_logic_data_to_current_block, %function
write_logic_data_to_current_block:
	stp	x29, x30, [sp, -80]!
	add	x29, sp, 0
	str	x0, [x29, 40]
	str	x1, [x29, 32]
	str	w2, [x29, 28]
	str	w3, [x29, 24]
	str	x4, [x29, 16]
	str	wzr, [x29, 76]
	ldr	w0, [x29, 28]
	cmp	w0, 192
	bne	.L944
	ldr	w1, [x29, 24]
	ldr	x0, [x29, 40]
	bl	get_logic_page_map
	str	x0, [x29, 56]
	ldr	x0, [x29, 56]
	cmp	x0, 0
	bne	.L945
	adrp	x0, .LC167
	add	x0, x0, :lo12:.LC167
	bl	nand_print_dbg
	mov	w0, 1
	b	.L946
.L945:
	ldr	x0, [x29, 56]
	ldrh	w1, [x0, 2]
	mov	w0, 65535
	cmp	w1, w0
	beq	.L947
	ldr	x0, [x29, 56]
	ldrh	w0, [x0, 2]
	mov	w1, w0
	ldr	x0, [x29, 40]
	bl	get_phy_block_addr
	str	x0, [x29, 64]
	ldr	x0, [x29, 32]
	ldr	x1, [x0]
	ldr	x0, [x29, 64]
	cmp	x1, x0
	bne	.L948
	ldr	x0, [x29, 64]
	ldrh	w0, [x0, 4]
	add	w0, w0, 1
	uxth	w1, w0
	ldr	x0, [x29, 64]
	strh	w1, [x0, 4]
	ldr	x0, [x29, 64]
	ldrh	w1, [x0, 4]
	ldr	x0, [x29, 40]
	ldr	x0, [x0, 8]
	ldrh	w0, [x0, 20]
	cmp	w1, w0
	bls	.L947
	ldr	x0, [x29, 32]
	ldrh	w0, [x0, 56]
	mov	w1, w0
	adrp	x0, .LC168
	add	x0, x0, :lo12:.LC168
	bl	nand_print_dbg
	b	.L947
.L948:
	mov	w0, 1
	str	w0, [x29, 76]
.L947:
	mov	w4, 192
	ldr	w3, [x29, 24]
	ldr	x2, [x29, 16]
	ldr	x1, [x29, 32]
	ldr	x0, [x29, 40]
	bl	write_phy_data_to_current_block
	str	w0, [x29, 52]
	ldr	w0, [x29, 52]
	cmp	w0, 0
	beq	.L950
	adrp	x0, .LC169
	add	x0, x0, :lo12:.LC169
	bl	nand_print
	ldr	w0, [x29, 52]
	b	.L946
.L950:
	ldr	x0, [x29, 32]
	ldr	x0, [x0]
	ldrh	w1, [x0]
	ldr	x0, [x29, 56]
	strh	w1, [x0, 2]
	ldr	x0, [x29, 32]
	ldrh	w0, [x0, 56]
	and	w0, w0, 4095
	uxth	w2, w0
	ldr	x1, [x29, 56]
	ldrh	w0, [x1]
	bfi	w0, w2, 0, 12
	strh	w0, [x1]
	ldr	x0, [x29, 40]
	ldr	w0, [x0, 664]
	cmp	w0, 0
	cset	w0, eq
	uxtb	w2, w0
	ldr	x1, [x29, 56]
	ldrb	w0, [x1, 1]
	bfi	w0, w2, 4, 1
	strb	w0, [x1, 1]
	ldr	x0, [x29, 32]
	ldr	x1, [x0, 24]
	ldr	x0, [x29, 56]
	ldrh	w0, [x0]
	ubfx	x0, x0, 0, 12
	uxth	w0, w0
	uxth	x0, w0
	lsl	x0, x0, 2
	add	x0, x1, x0
	ldr	w1, [x29, 24]
	str	w1, [x0]
	ldr	x0, [x29, 32]
	ldrh	w0, [x0, 56]
	add	w0, w0, 1
	uxth	w1, w0
	ldr	x0, [x29, 32]
	strh	w1, [x0, 56]
	b	.L951
.L944:
	ldr	w1, [x29, 28]
	mov	w0, 4369
	movk	w0, 0xaa11, lsl 16
	cmp	w1, w0
	bne	.L952
	ldr	w1, [x29, 24]
	ldr	x0, [x29, 40]
	bl	get_logic_page_map
	str	x0, [x29, 56]
	ldr	x0, [x29, 56]
	cmp	x0, 0
	bne	.L953
	adrp	x0, .LC167
	add	x0, x0, :lo12:.LC167
	bl	nand_print_dbg
	mov	w0, 1
	b	.L946
.L953:
	ldr	x0, [x29, 56]
	ldrh	w1, [x0, 2]
	mov	w0, 65535
	cmp	w1, w0
	beq	.L954
	ldr	x0, [x29, 56]
	ldrh	w0, [x0, 2]
	mov	w1, w0
	ldr	x0, [x29, 40]
	bl	get_phy_block_addr
	str	x0, [x29, 64]
	ldr	x0, [x29, 32]
	ldr	x1, [x0]
	ldr	x0, [x29, 64]
	cmp	x1, x0
	bne	.L955
	ldr	x0, [x29, 32]
	ldr	x1, [x0, 24]
	ldr	x0, [x29, 56]
	ldrh	w0, [x0]
	ubfx	x0, x0, 0, 12
	uxth	w0, w0
	uxth	x0, w0
	lsl	x0, x0, 2
	add	x0, x1, x0
	mov	w1, -1
	str	w1, [x0]
	ldr	x0, [x29, 64]
	ldrh	w0, [x0, 4]
	add	w0, w0, 1
	uxth	w1, w0
	ldr	x0, [x29, 64]
	strh	w1, [x0, 4]
	ldr	x0, [x29, 64]
	ldrh	w1, [x0, 4]
	ldr	x0, [x29, 40]
	ldr	x0, [x0, 8]
	ldrh	w0, [x0, 20]
	cmp	w1, w0
	bls	.L954
	ldr	x0, [x29, 32]
	ldrh	w0, [x0, 56]
	mov	w1, w0
	adrp	x0, .LC170
	add	x0, x0, :lo12:.LC170
	bl	nand_print
	b	.L954
.L955:
	mov	w0, 1
	str	w0, [x29, 76]
.L954:
	ldr	x0, [x29, 56]
	mov	w1, -1
	strh	w1, [x0, 2]
	ldr	x0, [x29, 56]
	ldrh	w1, [x0]
	orr	w1, w1, 4095
	strh	w1, [x0]
	ldr	x0, [x29, 56]
	ldrb	w1, [x0, 1]
	orr	w1, w1, 16
	strb	w1, [x0, 1]
	b	.L951
.L952:
	ldr	w1, [x29, 28]
	mov	w0, 13107
	movk	w0, 0xaa11, lsl 16
	cmp	w1, w0
	bne	.L951
	ldr	x0, [x29, 32]
	ldr	x0, [x0]
	ldrh	w1, [x0, 4]
	add	w1, w1, 1
	uxth	w1, w1
	strh	w1, [x0, 4]
	mov	w4, 13107
	movk	w4, 0xaa11, lsl 16
	ldr	w3, [x29, 24]
	ldr	x2, [x29, 16]
	ldr	x1, [x29, 32]
	ldr	x0, [x29, 40]
	bl	write_phy_data_to_current_block
	str	w0, [x29, 52]
	ldr	w0, [x29, 52]
	cmp	w0, 0
	beq	.L957
	adrp	x0, .LC171
	add	x0, x0, :lo12:.LC171
	bl	nand_print
	ldr	w0, [x29, 52]
	b	.L946
.L957:
	ldr	x0, [x29, 32]
	ldrh	w0, [x0, 56]
	add	w0, w0, 1
	uxth	w1, w0
	ldr	x0, [x29, 32]
	strh	w1, [x0, 56]
.L951:
	ldr	w0, [x29, 76]
	cmp	w0, 1
	bne	.L958
	ldr	x1, [x29, 64]
	ldr	x0, [x29, 40]
	bl	phy_block_from_invalid_page_incr
.L958:
	ldr	x0, [x29, 40]
	bl	adjust_invaild_list
	mov	w0, 0
.L946:
	ldp	x29, x30, [sp], 80
	ret
	.size	write_logic_data_to_current_block, .-write_logic_data_to_current_block
	.section	.rodata
	.align	3
.LC172:
	.string	"[ND]swl: new_cur_blk->alloc ok(max_ec): block_no = %d; ec = %d; \n"
	.align	3
.LC173:
	.string	"[NE]no free block!!!!\n"
	.text
	.align	2
	.global	get_new_current_block
	.type	get_new_current_block, %function
get_new_current_block:
	stp	x29, x30, [sp, -48]!
	add	x29, sp, 0
	str	x0, [x29, 24]
	str	x1, [x29, 16]
	ldr	x0, [x29, 16]
	ldr	x0, [x0]
	ldr	w0, [x0, 8]
	str	w0, [x29, 44]
	ldr	x0, [x29, 16]
	ldr	x3, [x0, 24]
	ldr	x0, [x29, 16]
	ldr	w0, [x0, 32]
	mov	w2, w0
	mov	w1, -1
	mov	x0, x3
	bl	nand_memset
	ldr	x0, [x29, 24]
	ldrh	w0, [x0, 1062]
	cmp	w0, 0
	beq	.L960
	ldr	x0, [x29, 24]
	ldr	x0, [x0, 8]
	ldrh	w0, [x0, 16]
	mov	w1, w0
	ldr	x0, [x29, 24]
	bl	out_phy_block_from_free_list_by_erase_time
	mov	x1, x0
	ldr	x0, [x29, 16]
	str	x1, [x0]
	ldr	x0, [x29, 16]
	ldr	x0, [x0]
	cmp	x0, 0
	beq	.L962
	ldr	x0, [x29, 16]
	ldr	x0, [x0]
	ldrh	w0, [x0]
	mov	w1, w0
	ldr	x0, [x29, 16]
	ldr	x0, [x0]
	ldrh	w0, [x0, 6]
	mov	w2, w0
	adrp	x0, .LC172
	add	x0, x0, :lo12:.LC172
	bl	nand_print_dbg
	b	.L962
.L960:
	ldr	x0, [x29, 24]
	bl	out_phy_block_from_free_list
	mov	x1, x0
	ldr	x0, [x29, 16]
	str	x1, [x0]
.L962:
	ldr	x0, [x29, 16]
	ldr	x0, [x0]
	cmp	x0, 0
	bne	.L963
	adrp	x0, .LC173
	add	x0, x0, :lo12:.LC173
	bl	nand_print
	mov	w0, 1
	b	.L964
.L963:
	ldr	x0, [x29, 16]
	strh	wzr, [x0, 56]
	ldr	x0, [x29, 16]
	ldr	x0, [x0]
	ldr	w1, [x29, 44]
	add	w1, w1, 1
	str	w1, [x0, 8]
	ldr	x0, [x29, 24]
	ldrb	w0, [x0, 25]
	and	w0, w0, 1
	cmp	w0, 0
	beq	.L965
	ldr	x0, [x29, 16]
	ldr	x0, [x0]
	ldr	w1, [x0, 8]
	add	w1, w1, 1
	str	w1, [x0, 8]
.L965:
	mov	w0, 0
.L964:
	ldp	x29, x30, [sp], 48
	ret
	.size	get_new_current_block, .-get_new_current_block
	.section	.rodata
	.align	3
.LC174:
	.string	"[NE]new_block_init_for_write erase error!\n"
	.text
	.align	2
	.global	new_block_init_for_write
	.type	new_block_init_for_write, %function
new_block_init_for_write:
	stp	x29, x30, [sp, -80]!
	add	x29, sp, 0
	str	x0, [x29, 40]
	str	x1, [x29, 32]
	str	w2, [x29, 28]
	ldr	x0, [x29, 32]
	ldrh	w1, [x0]
	ldr	x0, [x29, 40]
	ldr	x0, [x0, 8]
	ldrh	w2, [x0, 28]
	add	x0, x29, 48
	mov	x5, 0
	mov	x4, 0
	mov	w3, w2
	mov	w2, 0
	bl	set_physic_op_par
	ldr	x0, [x29, 40]
	ldr	x2, [x0, 3456]
	add	x0, x29, 48
	mov	x1, x0
	ldr	x0, [x29, 40]
	blr	x2
	str	w0, [x29, 76]
	ldr	x0, [x29, 32]
	strh	wzr, [x0, 4]
	ldr	x0, [x29, 32]
	ldrh	w0, [x0, 6]
	add	w0, w0, 1
	uxth	w1, w0
	ldr	x0, [x29, 32]
	strh	w1, [x0, 6]
	ldr	x0, [x29, 32]
	ldrh	w0, [x0, 6]
	mov	w1, w0
	ldr	x0, [x29, 40]
	ldr	w0, [x0, 676]
	cmp	w1, w0
	bls	.L967
	ldr	x0, [x29, 32]
	ldrh	w1, [x0, 6]
	ldr	x0, [x29, 40]
	ldr	x0, [x0, 8]
	ldrh	w0, [x0, 16]
	cmp	w1, w0
	bcs	.L967
	ldr	x0, [x29, 32]
	ldrh	w0, [x0, 6]
	mov	w1, w0
	ldr	x0, [x29, 40]
	str	w1, [x0, 676]
.L967:
	ldr	w1, [x29, 28]
	ldr	x0, [x29, 32]
	str	w1, [x0, 8]
	ldr	w0, [x29, 76]
	cmp	w0, 0
	beq	.L968
	ldr	x0, [x29, 40]
	ldr	x2, [x0, 3496]
	add	x0, x29, 48
	mov	x1, x0
	ldr	x0, [x29, 40]
	blr	x2
	adrp	x0, .LC174
	add	x0, x0, :lo12:.LC174
	bl	nand_print
	mov	w0, 1
	b	.L970
.L968:
	mov	w0, 0
.L970:
	ldp	x29, x30, [sp], 80
	ret
	.size	new_block_init_for_write, .-new_block_init_for_write
	.section	.rodata
	.align	3
.LC175:
	.string	"[NE]invaild_page num error:[%d] [%d]  used_page:[%d]!!\n"
	.text
	.align	2
	.global	check_invalid_page_right
	.type	check_invalid_page_right, %function
check_invalid_page_right:
	stp	x29, x30, [sp, -80]!
	add	x29, sp, 0
	str	x0, [x29, 40]
	str	x1, [x29, 32]
	str	x2, [x29, 24]
	strh	w3, [x29, 22]
	str	wzr, [x29, 72]
	str	wzr, [x29, 76]
	b	.L972
.L978:
	ldr	x0, [x29, 32]
	ldr	x1, [x0, 16]
	ldr	w0, [x29, 76]
	lsl	x0, x0, 2
	add	x0, x1, x0
	ldr	w0, [x0]
	str	w0, [x29, 68]
	ldr	w0, [x29, 68]
	cmn	w0, #1
	beq	.L973
	ldr	w1, [x29, 68]
	ldr	x0, [x29, 40]
	bl	get_logic_page_map
	str	x0, [x29, 56]
	ldr	x0, [x29, 56]
	cmp	x0, 0
	bne	.L974
	adrp	x0, .LC147
	add	x0, x0, :lo12:.LC147
	bl	nand_print_dbg
	ldr	w0, [x29, 72]
	add	w0, w0, 1
	str	w0, [x29, 72]
	b	.L975
.L974:
	ldr	x0, [x29, 56]
	ldrh	w1, [x0, 2]
	ldr	x0, [x29, 24]
	ldrh	w0, [x0]
	cmp	w1, w0
	bne	.L976
	ldr	x0, [x29, 56]
	ldrh	w0, [x0]
	ubfx	x0, x0, 0, 12
	uxth	w0, w0
	mov	w1, w0
	ldr	w0, [x29, 76]
	cmp	w1, w0
	beq	.L975
.L976:
	ldr	w0, [x29, 72]
	add	w0, w0, 1
	str	w0, [x29, 72]
	b	.L975
.L973:
	ldr	w0, [x29, 72]
	add	w0, w0, 1
	str	w0, [x29, 72]
.L975:
	ldr	w0, [x29, 76]
	add	w0, w0, 1
	str	w0, [x29, 76]
.L972:
	ldrh	w1, [x29, 22]
	ldr	w0, [x29, 76]
	cmp	w1, w0
	bhi	.L978
	ldr	x0, [x29, 24]
	ldrh	w0, [x0, 4]
	mov	w1, w0
	ldr	w0, [x29, 72]
	cmp	w1, w0
	beq	.L979
	ldr	x0, [x29, 24]
	ldrh	w0, [x0, 4]
	mov	w2, w0
	ldrh	w1, [x29, 22]
	adrp	x0, .LC175
	add	x0, x0, :lo12:.LC175
	mov	w3, w1
	ldr	w1, [x29, 72]
	bl	nand_print
	mov	w0, 1
	b	.L980
.L979:
	mov	w0, 0
.L980:
	ldp	x29, x30, [sp], 80
	ret
	.size	check_invalid_page_right, .-check_invalid_page_right
	.align	2
	.global	nftl_ops_init
	.type	nftl_ops_init, %function
nftl_ops_init:
	sub	sp, sp, #16
	str	x0, [sp, 8]
	ldr	x0, [sp, 8]
	adrp	x1, _nftl_nand_erase_superblk
	add	x1, x1, :lo12:_nftl_nand_erase_superblk
	str	x1, [x0, 3456]
	ldr	x0, [sp, 8]
	adrp	x1, _nftl_nand_read_page
	add	x1, x1, :lo12:_nftl_nand_read_page
	str	x1, [x0, 3464]
	ldr	x0, [sp, 8]
	adrp	x1, _nftl_nand_write_page
	add	x1, x1, :lo12:_nftl_nand_write_page
	str	x1, [x0, 3472]
	ldr	x0, [sp, 8]
	adrp	x1, _nftl_nand_is_blk_good
	add	x1, x1, :lo12:_nftl_nand_is_blk_good
	str	x1, [x0, 3488]
	ldr	x0, [sp, 8]
	adrp	x1, _nftl_nand_mark_bad_blk
	add	x1, x1, :lo12:_nftl_nand_mark_bad_blk
	str	x1, [x0, 3496]
	ldr	x0, [sp, 8]
	adrp	x1, _nftl_nand_copy_page
	add	x1, x1, :lo12:_nftl_nand_copy_page
	str	x1, [x0, 3480]
	ldr	x0, [sp, 8]
	adrp	x1, _nftl_nand_read_logic_page
	add	x1, x1, :lo12:_nftl_nand_read_logic_page
	str	x1, [x0, 3512]
	ldr	x0, [sp, 8]
	adrp	x1, _nftl_nand_write_logic_page
	add	x1, x1, :lo12:_nftl_nand_write_logic_page
	str	x1, [x0, 3504]
	ldr	x0, [sp, 8]
	adrp	x1, _nftl_discard_logic_page
	add	x1, x1, :lo12:_nftl_discard_logic_page
	str	x1, [x0, 3520]
	nop
	add	sp, sp, 16
	ret
	.size	nftl_ops_init, .-nftl_ops_init
	.align	2
	.global	current_block_at_last_page
	.type	current_block_at_last_page, %function
current_block_at_last_page:
	sub	sp, sp, #16
	str	x0, [sp, 8]
	str	x1, [sp]
	ldr	x0, [sp]
	ldrh	w0, [x0, 56]
	mov	w1, w0
	ldr	x0, [sp, 8]
	ldr	x0, [x0, 8]
	ldrh	w0, [x0, 20]
	sub	w0, w0, #1
	cmp	w1, w0
	bne	.L984
	mov	w0, 1
	b	.L985
.L984:
	mov	w0, 0
.L985:
	add	sp, sp, 16
	ret
	.size	current_block_at_last_page, .-current_block_at_last_page
	.align	2
	.global	current_block_at_first_page
	.type	current_block_at_first_page, %function
current_block_at_first_page:
	sub	sp, sp, #16
	str	x0, [sp, 8]
	str	x1, [sp]
	ldr	x0, [sp]
	ldrh	w0, [x0, 56]
	cmp	w0, 0
	bne	.L987
	mov	w0, 1
	b	.L988
.L987:
	mov	w0, 0
.L988:
	add	sp, sp, 16
	ret
	.size	current_block_at_first_page, .-current_block_at_first_page
	.align	2
	.global	fill_no_use_data_to_page
	.type	fill_no_use_data_to_page, %function
fill_no_use_data_to_page:
	stp	x29, x30, [sp, -32]!
	add	x29, sp, 0
	str	x0, [x29, 24]
	ldr	x0, [x29, 24]
	add	x0, x0, 528
	mov	x1, x0
	ldr	x0, [x29, 24]
	bl	current_block_at_last_page
	cmp	w0, 0
	beq	.L990
	ldr	x0, [x29, 24]
	add	x0, x0, 528
	mov	x1, x0
	ldr	x0, [x29, 24]
	bl	write_phy_page_map_to_current_block
	cmp	w0, 0
	beq	.L991
	adrp	x0, .LC158
	add	x0, x0, :lo12:.LC158
	bl	nand_print
	mov	w0, 1
	b	.L992
.L991:
	ldr	x0, [x29, 24]
	ldrb	w0, [x0, 25]
	and	w0, w0, 1
	cmp	w0, 0
	beq	.L993
	ldr	x0, [x29, 24]
	add	x0, x0, 592
	mov	x1, x0
	ldr	x0, [x29, 24]
	bl	write_phy_page_map_to_current_block
	cmp	w0, 0
	beq	.L993
	adrp	x0, .LC158
	add	x0, x0, :lo12:.LC158
	bl	nand_print
.L993:
	ldr	x0, [x29, 24]
	add	x0, x0, 528
	mov	x1, x0
	ldr	x0, [x29, 24]
	bl	get_new_current_block
	cmp	w0, 0
	beq	.L994
	adrp	x0, .LC159
	add	x0, x0, :lo12:.LC159
	bl	nand_print
	mov	w0, 1
	b	.L992
.L994:
	ldr	x0, [x29, 24]
	ldrb	w0, [x0, 25]
	and	w0, w0, 1
	cmp	w0, 0
	beq	.L995
	ldr	x0, [x29, 24]
	add	x0, x0, 592
	mov	x1, x0
	ldr	x0, [x29, 24]
	bl	get_new_current_block
	cmp	w0, 0
	beq	.L995
	adrp	x0, .LC160
	add	x0, x0, :lo12:.LC160
	bl	nand_print
	mov	w0, 1
	b	.L992
.L995:
	mov	w0, 0
	b	.L992
.L990:
	ldr	x0, [x29, 24]
	add	x0, x0, 528
	mov	x1, x0
	ldr	x0, [x29, 24]
	bl	current_block_at_first_page
	cmp	w0, 0
	beq	.L996
	mov	w0, 0
	b	.L992
.L996:
	ldr	x0, [x29, 24]
	add	x0, x0, 528
	mov	x1, x0
	ldr	x0, [x29, 24]
	bl	write_phy_page_map_to_current_block
	cmp	w0, 0
	beq	.L997
	adrp	x0, .LC158
	add	x0, x0, :lo12:.LC158
	bl	nand_print
.L997:
	ldr	x0, [x29, 24]
	ldrb	w0, [x0, 25]
	and	w0, w0, 1
	cmp	w0, 0
	beq	.L998
	ldr	x0, [x29, 24]
	add	x0, x0, 592
	mov	x1, x0
	ldr	x0, [x29, 24]
	bl	write_phy_page_map_to_current_block
	cmp	w0, 0
	beq	.L998
	adrp	x0, .LC158
	add	x0, x0, :lo12:.LC158
	bl	nand_print
.L998:
	mov	w0, 8
.L992:
	ldp	x29, x30, [sp], 32
	ret
	.size	fill_no_use_data_to_page, .-fill_no_use_data_to_page
	.align	2
	.global	fill_no_use_data_to_current_block
	.type	fill_no_use_data_to_current_block, %function
fill_no_use_data_to_current_block:
	stp	x29, x30, [sp, -48]!
	add	x29, sp, 0
	str	x0, [x29, 24]
	str	wzr, [x29, 44]
	b	.L1000
.L1003:
	ldr	x0, [x29, 24]
	bl	fill_no_use_data_to_page
	str	w0, [x29, 40]
	ldr	w0, [x29, 40]
	cmp	w0, 0
	bne	.L1001
	mov	w0, 0
	b	.L1002
.L1001:
	ldr	w0, [x29, 44]
	add	w0, w0, 1
	str	w0, [x29, 44]
.L1000:
	ldr	x0, [x29, 24]
	ldr	x0, [x0, 8]
	ldrh	w0, [x0, 20]
	mov	w1, w0
	ldr	w0, [x29, 44]
	cmp	w1, w0
	bgt	.L1003
	mov	w0, 1
.L1002:
	ldp	x29, x30, [sp], 48
	ret
	.size	fill_no_use_data_to_current_block, .-fill_no_use_data_to_current_block
	.align	2
	.global	fill_no_use_page_to_current_block
	.type	fill_no_use_page_to_current_block, %function
fill_no_use_page_to_current_block:
	stp	x29, x30, [sp, -48]!
	add	x29, sp, 0
	str	x0, [x29, 24]
	str	w1, [x29, 20]
	str	wzr, [x29, 44]
	b	.L1005
.L1008:
	ldr	x0, [x29, 24]
	bl	fill_no_use_data_to_page
	str	w0, [x29, 40]
	ldr	w0, [x29, 40]
	cmp	w0, 0
	bne	.L1006
	mov	w0, 0
	b	.L1007
.L1006:
	ldr	w0, [x29, 44]
	add	w0, w0, 1
	str	w0, [x29, 44]
.L1005:
	ldr	w1, [x29, 44]
	ldr	w0, [x29, 20]
	cmp	w1, w0
	blt	.L1008
	mov	w0, 1
.L1007:
	ldp	x29, x30, [sp], 48
	ret
	.size	fill_no_use_page_to_current_block, .-fill_no_use_page_to_current_block
	.align	2
	.global	nand_before_shutdown
	.type	nand_before_shutdown, %function
nand_before_shutdown:
	stp	x29, x30, [sp, -32]!
	add	x29, sp, 0
	str	x0, [x29, 24]
	mov	w1, 8
	ldr	x0, [x29, 24]
	bl	fill_no_use_page_to_current_block
	ldp	x29, x30, [sp], 32
	ret
	.size	nand_before_shutdown, .-nand_before_shutdown
	.align	2
	.global	nftl_get_boot_cnt
	.type	nftl_get_boot_cnt, %function
nftl_get_boot_cnt:
	sub	sp, sp, #32
	str	x0, [sp, 8]
	ldr	x0, [sp, 8]
	str	x0, [sp, 24]
	ldr	x0, [sp, 24]
	ldr	x0, [x0, 3440]
	ldr	w0, [x0, 140]
	add	sp, sp, 32
	ret
	.size	nftl_get_boot_cnt, .-nftl_get_boot_cnt
	.align	2
	.global	nftl_get_zone_cap
	.type	nftl_get_zone_cap, %function
nftl_get_zone_cap:
	sub	sp, sp, #32
	str	x0, [sp, 8]
	ldr	x0, [sp, 8]
	str	x0, [sp, 24]
	ldr	x0, [sp, 24]
	ldr	w0, [x0, 32]
	add	sp, sp, 32
	ret
	.size	nftl_get_zone_cap, .-nftl_get_zone_cap
	.align	2
	.global	nftl_set_zone_test
	.type	nftl_set_zone_test, %function
nftl_set_zone_test:
	stp	x29, x30, [sp, -48]!
	add	x29, sp, 0
	str	x0, [x29, 24]
	str	w1, [x29, 20]
	ldr	x0, [x29, 24]
	str	x0, [x29, 40]
	ldr	x0, [x29, 40]
	ldr	w1, [x29, 20]
	str	w1, [x0, 656]
	ldr	x0, [x29, 40]
	bl	print_nftl_zone
	nop
	ldp	x29, x30, [sp], 48
	ret
	.size	nftl_set_zone_test, .-nftl_set_zone_test
	.align	2
	.global	nftl_get_zone_priv
	.type	nftl_get_zone_priv, %function
nftl_get_zone_priv:
	sub	sp, sp, #32
	str	x0, [sp, 8]
	ldr	x0, [sp, 8]
	str	x0, [sp, 24]
	ldr	x0, [sp, 24]
	ldr	x0, [x0]
	add	sp, sp, 32
	ret
	.size	nftl_get_zone_priv, .-nftl_get_zone_priv
	.align	2
	.global	nftl_get_zone_write_cache_nums
	.type	nftl_get_zone_write_cache_nums, %function
nftl_get_zone_write_cache_nums:
	sub	sp, sp, #32
	str	x0, [sp, 8]
	ldr	x0, [sp, 8]
	str	x0, [sp, 24]
	ldr	x0, [sp, 24]
	ldrh	w0, [x0, 1076]
	add	sp, sp, 32
	ret
	.size	nftl_get_zone_write_cache_nums, .-nftl_get_zone_write_cache_nums
	.section	.rodata
	.align	3
.LC176:
	.string	"[ND]smart->version:%x;\n"
	.align	3
.LC177:
	.string	"[ND]smart->total_recv_read_sectors:%lld;\n"
	.align	3
.LC178:
	.string	"[ND]smart->total_recv_write_sectors:%lld;\n"
	.align	3
.LC179:
	.string	"[ND]smart->total_real_read_pages:%lld;\n"
	.align	3
.LC180:
	.string	"[ND]smart->total_real_write_pages:%lld;\n"
	.align	3
.LC181:
	.string	"[ND]smart->total_recv_discard_sectors:%lld;\n"
	.align	3
.LC182:
	.string	"[ND]smart->total_real_discard_sectors:%lld;\n"
	.align	3
.LC183:
	.string	"[ND]smart->total_recv_read_claim_pages:%d;\n"
	.align	3
.LC184:
	.string	"[ND]smart->total_gc_times:%d;\n"
	.align	3
.LC185:
	.string	"[ND]smart->total_gc_pages:%d;\n"
	.align	3
.LC186:
	.string	"[ND]smart->total_wl_times:%d;\n"
	.align	3
.LC187:
	.string	"[ND]smart->total_wl_gc_times:%d;\n"
	.align	3
.LC188:
	.string	"[ND]smart->total_real_read_error_pages:%d;\n"
	.align	3
.LC189:
	.string	"[ND]smart->total_real_write_error_pages:%d;\n"
	.align	3
.LC190:
	.string	"[ND]smart->total_normal_power_cycles:%x;\n"
	.align	3
.LC191:
	.string	"[ND]smart->total_unusual_power_cycles:%x;\n"
	.align	3
.LC192:
	.string	"[ND]smart->max_block_erase_times:%d;\n"
	.align	3
.LC193:
	.string	"[ND]smart->min_block_erase_times:%d;\n"
	.align	3
.LC194:
	.string	"[ND]smart->max_block_used_counter:%d;\n"
	.align	3
.LC195:
	.string	"[ND]smart->min_block_used_counter:%d;\n"
	.align	3
.LC196:
	.string	"[ND]smart->read_reclaim_utc:%d;\n"
	.align	3
.LC197:
	.string	"[ND]smart->cross_talk_times:%d;\n"
	.align	3
.LC198:
	.string	"[ND]smart->read_reclaim_zone_no:%d;\n"
	.align	3
.LC199:
	.string	"[ND]smart->read_reclaim_page_no:%d;\n"
	.text
	.align	2
	.global	print_smart
	.type	print_smart, %function
print_smart:
	stp	x29, x30, [sp, -48]!
	add	x29, sp, 0
	str	x0, [x29, 24]
	ldr	x0, [x29, 24]
	str	x0, [x29, 40]
	ldr	x0, [x29, 40]
	ldr	w0, [x0, 656]
	cmp	w0, 0
	beq	.L1024
	ldr	x0, [x29, 40]
	ldr	x0, [x0, 3440]
	str	x0, [x29, 32]
	ldr	x0, [x29, 32]
	ldr	w1, [x0, 112]
	adrp	x0, .LC176
	add	x0, x0, :lo12:.LC176
	bl	nand_print_dbg
	ldr	x0, [x29, 32]
	ldr	x1, [x0, 64]
	adrp	x0, .LC177
	add	x0, x0, :lo12:.LC177
	bl	nand_print_dbg
	ldr	x0, [x29, 32]
	ldr	x1, [x0, 72]
	adrp	x0, .LC178
	add	x0, x0, :lo12:.LC178
	bl	nand_print_dbg
	ldr	x0, [x29, 32]
	ldr	x1, [x0, 80]
	adrp	x0, .LC179
	add	x0, x0, :lo12:.LC179
	bl	nand_print_dbg
	ldr	x0, [x29, 32]
	ldr	x1, [x0, 88]
	adrp	x0, .LC180
	add	x0, x0, :lo12:.LC180
	bl	nand_print_dbg
	ldr	x0, [x29, 32]
	ldr	x1, [x0, 96]
	adrp	x0, .LC181
	add	x0, x0, :lo12:.LC181
	bl	nand_print_dbg
	ldr	x0, [x29, 32]
	ldr	x1, [x0, 104]
	adrp	x0, .LC182
	add	x0, x0, :lo12:.LC182
	bl	nand_print_dbg
	ldr	x0, [x29, 32]
	ldr	w1, [x0, 116]
	adrp	x0, .LC183
	add	x0, x0, :lo12:.LC183
	bl	nand_print_dbg
	ldr	x0, [x29, 32]
	ldr	w1, [x0, 120]
	adrp	x0, .LC184
	add	x0, x0, :lo12:.LC184
	bl	nand_print_dbg
	ldr	x0, [x29, 32]
	ldr	w1, [x0, 124]
	adrp	x0, .LC185
	add	x0, x0, :lo12:.LC185
	bl	nand_print_dbg
	ldr	x0, [x29, 32]
	ldr	w1, [x0, 128]
	adrp	x0, .LC186
	add	x0, x0, :lo12:.LC186
	bl	nand_print_dbg
	ldr	x0, [x29, 32]
	ldr	w1, [x0, 180]
	adrp	x0, .LC187
	add	x0, x0, :lo12:.LC187
	bl	nand_print_dbg
	ldr	x0, [x29, 32]
	ldr	w1, [x0, 132]
	adrp	x0, .LC188
	add	x0, x0, :lo12:.LC188
	bl	nand_print_dbg
	ldr	x0, [x29, 32]
	ldr	w1, [x0, 136]
	adrp	x0, .LC189
	add	x0, x0, :lo12:.LC189
	bl	nand_print_dbg
	ldr	x0, [x29, 32]
	ldr	w1, [x0, 140]
	adrp	x0, .LC190
	add	x0, x0, :lo12:.LC190
	bl	nand_print_dbg
	ldr	x0, [x29, 32]
	ldr	w1, [x0, 144]
	adrp	x0, .LC191
	add	x0, x0, :lo12:.LC191
	bl	nand_print_dbg
	ldr	x0, [x29, 32]
	ldr	w1, [x0, 148]
	adrp	x0, .LC192
	add	x0, x0, :lo12:.LC192
	bl	nand_print_dbg
	ldr	x0, [x29, 32]
	ldr	w1, [x0, 152]
	adrp	x0, .LC193
	add	x0, x0, :lo12:.LC193
	bl	nand_print_dbg
	ldr	x0, [x29, 32]
	ldr	w1, [x0, 156]
	adrp	x0, .LC194
	add	x0, x0, :lo12:.LC194
	bl	nand_print_dbg
	ldr	x0, [x29, 32]
	ldr	w1, [x0, 160]
	adrp	x0, .LC195
	add	x0, x0, :lo12:.LC195
	bl	nand_print_dbg
	ldr	x0, [x29, 32]
	ldr	w1, [x0, 164]
	adrp	x0, .LC196
	add	x0, x0, :lo12:.LC196
	bl	nand_print_dbg
	ldr	x0, [x29, 32]
	ldr	w1, [x0, 168]
	adrp	x0, .LC197
	add	x0, x0, :lo12:.LC197
	bl	nand_print_dbg
	ldr	x0, [x29, 32]
	ldr	w1, [x0, 172]
	adrp	x0, .LC198
	add	x0, x0, :lo12:.LC198
	bl	nand_print_dbg
	ldr	x0, [x29, 32]
	ldr	w1, [x0, 176]
	adrp	x0, .LC199
	add	x0, x0, :lo12:.LC199
	bl	nand_print_dbg
	b	.L1021
.L1024:
	nop
.L1021:
	ldp	x29, x30, [sp], 48
	ret
	.size	print_smart, .-print_smart
	.section	.rodata
	.align	3
.LC200:
	.string	"%08x: "
	.text
	.align	2
	.global	dump_buf
	.type	dump_buf, %function
dump_buf:
	stp	x29, x30, [sp, -48]!
	add	x29, sp, 0
	str	x0, [x29, 24]
	str	w1, [x29, 20]
	str	wzr, [x29, 44]
	adrp	x0, .LC45
	add	x0, x0, :lo12:.LC45
	bl	nand_print
	str	wzr, [x29, 44]
	b	.L1026
.L1029:
	ldr	w0, [x29, 44]
	and	w0, w0, 15
	cmp	w0, 0
	bne	.L1027
	adrp	x0, .LC200
	add	x0, x0, :lo12:.LC200
	ldr	w1, [x29, 44]
	bl	nand_print
.L1027:
	ldrsw	x0, [x29, 44]
	ldr	x1, [x29, 24]
	add	x0, x1, x0
	ldrb	w0, [x0]
	mov	w1, w0
	adrp	x0, .LC122
	add	x0, x0, :lo12:.LC122
	bl	nand_print
	ldr	w0, [x29, 44]
	add	w0, w0, 1
	and	w0, w0, 15
	cmp	w0, 0
	bne	.L1028
	adrp	x0, .LC45
	add	x0, x0, :lo12:.LC45
	bl	nand_print
.L1028:
	ldr	w0, [x29, 44]
	add	w0, w0, 1
	str	w0, [x29, 44]
.L1026:
	ldr	w1, [x29, 44]
	ldr	w0, [x29, 20]
	cmp	w1, w0
	bcc	.L1029
	adrp	x0, .LC45
	add	x0, x0, :lo12:.LC45
	bl	nand_print
	nop
	ldp	x29, x30, [sp], 48
	ret
	.size	dump_buf, .-dump_buf
	.section	.rodata
	.align	3
.LC201:
	.string	"[NE]exception case wb %04x %04x, len: %d\n"
	.text
	.align	2
	.type	crc_wrong, %function
crc_wrong:
	stp	x29, x30, [sp, -64]!
	add	x29, sp, 0
	str	x0, [x29, 40]
	str	x1, [x29, 32]
	str	x2, [x29, 24]
	strh	wzr, [x29, 58]
	strh	wzr, [x29, 56]
	str	wzr, [x29, 60]
	ldr	x0, [x29, 40]
	ldr	x0, [x0, 8]
	ldrb	w0, [x0, 8]
	lsl	w0, w0, 9
	str	w0, [x29, 60]
	ldr	x0, [x29, 24]
	bl	is_nouse_page
	cmp	w0, 0
	beq	.L1031
	mov	w0, 0
	b	.L1032
.L1031:
	ldr	x0, [x29, 24]
	bl	is_phy_mapping_page
	cmp	w0, 0
	beq	.L1033
	ldr	x0, [x29, 40]
	ldr	w0, [x0, 560]
	str	w0, [x29, 60]
.L1033:
	ldr	w2, [x29, 60]
	ldr	x1, [x29, 32]
	mov	w0, -1
	bl	nand_crc16
	strh	w0, [x29, 58]
	ldr	x0, [x29, 24]
	add	x0, x0, 13
	ldrb	w0, [x0]
	sxth	w1, w0
	ldr	x0, [x29, 24]
	add	x0, x0, 12
	ldrb	w0, [x0]
	lsl	w0, w0, 8
	sxth	w0, w0
	orr	w0, w1, w0
	sxth	w0, w0
	strh	w0, [x29, 56]
	ldrh	w1, [x29, 58]
	ldrh	w0, [x29, 56]
	cmp	w1, w0
	beq	.L1034
	ldrh	w1, [x29, 58]
	ldrh	w2, [x29, 56]
	adrp	x0, .LC201
	add	x0, x0, :lo12:.LC201
	ldr	w3, [x29, 60]
	bl	nand_print
	mov	w0, 1
	b	.L1032
.L1034:
	mov	w0, 0
.L1032:
	ldp	x29, x30, [sp], 64
	ret
	.size	crc_wrong, .-crc_wrong
	.section	.rodata
	.align	3
.LC202:
	.string	"[NE]%s:malloc fail for temp_page_buf\n"
	.align	3
.LC203:
	.string	"[NE]%s:malloc fail for logic_page_buf\n"
	.align	3
.LC204:
	.string	"[NE]zone_param_init error2\n"
	.align	3
.LC205:
	.string	"[NE]%s:malloc fail for current_block.user_info.buf\n"
	.align	3
.LC206:
	.string	"[NE]%s:malloc fail for assist_block.user_info.buf\n"
	.align	3
.LC207:
	.string	"[NE]%s:malloc fail for zone_phy_page_map_for_gc.buf\n"
	.text
	.align	2
	.global	zone_param_init
	.type	zone_param_init, %function
zone_param_init:
	stp	x29, x30, [sp, -64]!
	add	x29, sp, 0
	str	x0, [x29, 40]
	strh	w1, [x29, 38]
	str	w2, [x29, 32]
	str	w3, [x29, 28]
	ldr	x0, [x29, 40]
	ldr	x0, [x0, 8]
	ldrh	w0, [x0, 26]
	bl	nftl_malloc
	mov	x1, x0
	ldr	x0, [x29, 40]
	str	x1, [x0, 512]
	ldr	x0, [x29, 40]
	ldr	x0, [x0, 512]
	cmp	x0, 0
	bne	.L1036
	adrp	x0, __func__.7098
	add	x1, x0, :lo12:__func__.7098
	adrp	x0, .LC202
	add	x0, x0, :lo12:.LC202
	bl	nand_print
	mov	w0, -2
	b	.L1037
.L1036:
	ldr	x0, [x29, 40]
	ldr	x0, [x0, 8]
	ldrh	w0, [x0, 26]
	bl	nftl_malloc
	mov	x1, x0
	ldr	x0, [x29, 40]
	str	x1, [x0, 520]
	ldr	x0, [x29, 40]
	ldr	x0, [x0, 520]
	cmp	x0, 0
	bne	.L1038
	adrp	x0, __func__.7098
	add	x1, x0, :lo12:__func__.7098
	adrp	x0, .LC203
	add	x0, x0, :lo12:.LC203
	bl	nand_print
	mov	w0, -2
	b	.L1037
.L1038:
	ldr	x0, [x29, 40]
	ldr	w1, [x29, 32]
	str	w1, [x0, 32]
	ldr	x0, [x29, 40]
	ldr	w1, [x29, 28]
	str	w1, [x0, 40]
	ldr	x0, [x29, 40]
	ldrh	w1, [x29, 38]
	strh	w1, [x0, 44]
	ldrh	w1, [x29, 38]
	ldr	x0, [x29, 40]
	bl	get_phy_block_addr
	mov	x1, x0
	ldr	x0, [x29, 40]
	str	x1, [x0, 48]
	ldr	x0, [x29, 40]
	strh	wzr, [x0, 26]
	ldr	x0, [x29, 40]
	strh	wzr, [x0, 28]
	ldr	x0, [x29, 40]
	strh	wzr, [x0, 30]
	ldr	x0, [x29, 40]
	str	xzr, [x0, 56]
	ldr	x0, [x29, 40]
	ldr	w0, [x0, 32]
	ldr	x1, [x29, 40]
	ldr	x1, [x1, 8]
	ldrb	w1, [x1, 8]
	udiv	w0, w0, w1
	str	w0, [x29, 56]
	ldr	w1, [x29, 56]
	ldr	x0, [x29, 40]
	bl	malloc_logic_page_map
	cmp	w0, 0
	beq	.L1039
	adrp	x0, .LC204
	add	x0, x0, :lo12:.LC204
	bl	nand_print
	mov	w0, -2
	b	.L1037
.L1039:
	ldr	x0, [x29, 40]
	ldr	x0, [x0, 8]
	ldrh	w0, [x0, 20]
	lsl	w0, w0, 2
	mov	w1, w0
	ldr	x0, [x29, 40]
	str	w1, [x0, 560]
	ldr	x0, [x29, 40]
	mov	w1, 320
	str	w1, [x0, 576]
	ldr	x0, [x29, 40]
	ldr	w1, [x0, 560]
	ldr	x0, [x29, 40]
	ldr	w0, [x0, 576]
	add	w1, w1, w0
	ldr	x0, [x29, 40]
	str	w1, [x0, 544]
	ldr	x0, [x29, 40]
	ldr	w0, [x0, 544]
	bl	nftl_malloc
	mov	x1, x0
	ldr	x0, [x29, 40]
	str	x1, [x0, 536]
	ldr	x0, [x29, 40]
	ldr	x0, [x0, 536]
	cmp	x0, 0
	bne	.L1040
	adrp	x0, __func__.7098
	add	x1, x0, :lo12:__func__.7098
	adrp	x0, .LC205
	add	x0, x0, :lo12:.LC205
	bl	nand_print
	mov	w0, -2
	b	.L1037
.L1040:
	ldr	x0, [x29, 40]
	ldr	x1, [x0, 536]
	ldr	x0, [x29, 40]
	str	x1, [x0, 552]
	ldr	x0, [x29, 40]
	ldr	x1, [x0, 536]
	ldr	x0, [x29, 40]
	ldr	w0, [x0, 560]
	uxtw	x0, w0
	add	x1, x1, x0
	ldr	x0, [x29, 40]
	str	x1, [x0, 568]
	ldr	x0, [x29, 40]
	ldr	x1, [x0, 568]
	ldr	x0, [x29, 40]
	str	x1, [x0, 3440]
	ldr	x0, [x29, 40]
	ldr	x3, [x0, 536]
	ldr	x0, [x29, 40]
	ldr	w0, [x0, 544]
	mov	w2, w0
	mov	w1, -1
	mov	x0, x3
	bl	nand_memset
	ldr	x0, [x29, 40]
	ldr	w1, [x0, 560]
	ldr	x0, [x29, 40]
	str	w1, [x0, 624]
	ldr	x0, [x29, 40]
	ldr	w1, [x0, 576]
	ldr	x0, [x29, 40]
	str	w1, [x0, 576]
	ldr	x0, [x29, 40]
	ldr	w1, [x0, 544]
	ldr	x0, [x29, 40]
	str	w1, [x0, 608]
	ldr	x0, [x29, 40]
	ldr	w0, [x0, 608]
	bl	nftl_malloc
	mov	x1, x0
	ldr	x0, [x29, 40]
	str	x1, [x0, 600]
	ldr	x0, [x29, 40]
	ldr	x0, [x0, 600]
	cmp	x0, 0
	bne	.L1041
	adrp	x0, __func__.7098
	add	x1, x0, :lo12:__func__.7098
	adrp	x0, .LC206
	add	x0, x0, :lo12:.LC206
	bl	nand_print
	mov	w0, -2
	b	.L1037
.L1041:
	ldr	x0, [x29, 40]
	ldr	x1, [x0, 600]
	ldr	x0, [x29, 40]
	str	x1, [x0, 616]
	ldr	x0, [x29, 40]
	ldr	x1, [x0, 600]
	ldr	x0, [x29, 40]
	ldr	w0, [x0, 624]
	uxtw	x0, w0
	add	x1, x1, x0
	ldr	x0, [x29, 40]
	str	x1, [x0, 632]
	ldr	x0, [x29, 40]
	ldr	x3, [x0, 600]
	ldr	x0, [x29, 40]
	ldr	w0, [x0, 608]
	mov	w2, w0
	mov	w1, -1
	mov	x0, x3
	bl	nand_memset
	ldr	x0, [x29, 40]
	ldr	w1, [x0, 560]
	ldr	x0, [x29, 40]
	str	w1, [x0, 488]
	ldr	x0, [x29, 40]
	ldr	w1, [x0, 576]
	ldr	x0, [x29, 40]
	str	w1, [x0, 504]
	ldr	x0, [x29, 40]
	ldr	w1, [x0, 544]
	ldr	x0, [x29, 40]
	str	w1, [x0, 472]
	ldr	x0, [x29, 40]
	ldr	w0, [x0, 472]
	bl	nftl_malloc
	mov	x1, x0
	ldr	x0, [x29, 40]
	str	x1, [x0, 464]
	ldr	x0, [x29, 40]
	ldr	x0, [x0, 464]
	cmp	x0, 0
	bne	.L1042
	adrp	x0, __func__.7098
	add	x1, x0, :lo12:__func__.7098
	adrp	x0, .LC207
	add	x0, x0, :lo12:.LC207
	bl	nand_print
	mov	w0, -2
	b	.L1037
.L1042:
	ldr	x0, [x29, 40]
	ldr	x1, [x0, 464]
	ldr	x0, [x29, 40]
	str	x1, [x0, 480]
	ldr	x0, [x29, 40]
	ldr	x1, [x0, 464]
	ldr	x0, [x29, 40]
	ldr	w0, [x0, 488]
	uxtw	x0, w0
	add	x1, x1, x0
	ldr	x0, [x29, 40]
	str	x1, [x0, 496]
	ldr	x0, [x29, 40]
	str	wzr, [x0, 656]
	ldr	x0, [x29, 40]
	str	wzr, [x0, 676]
	ldr	x0, [x29, 40]
	str	wzr, [x0, 660]
	ldr	x0, [x29, 40]
	str	wzr, [x0, 664]
	ldr	x0, [x29, 40]
	str	wzr, [x0, 668]
	ldr	x0, [x29, 40]
	str	wzr, [x0, 672]
	ldr	x0, [x29, 40]
	ldr	x0, [x0, 8]
	ldrh	w0, [x0, 16]
	lsr	w0, w0, 1
	uxth	w1, w0
	ldr	x0, [x29, 40]
	strh	w1, [x0, 1056]
	ldr	x0, [x29, 40]
	ldr	x0, [x0, 8]
	ldrh	w1, [x0, 16]
	mov	w0, 43691
	movk	w0, 0xaaaa, lsl 16
	umull	x0, w1, w0
	lsr	x0, x0, 32
	lsr	w0, w0, 2
	uxth	w1, w0
	ldr	x0, [x29, 40]
	strh	w1, [x0, 1058]
	ldr	x0, [x29, 40]
	strh	wzr, [x0, 1060]
	ldr	x0, [x29, 40]
	strh	wzr, [x0, 1062]
	ldr	x0, [x29, 40]
	str	xzr, [x0, 1064]
	ldr	x0, [x29, 40]
	str	xzr, [x0, 528]
	ldr	x0, [x29, 40]
	mov	w1, -1
	strh	w1, [x0, 584]
	ldr	x0, [x29, 40]
	str	xzr, [x0, 592]
	ldr	x0, [x29, 40]
	mov	w1, -1
	strh	w1, [x0, 648]
	ldr	x0, [x29, 40]
	str	xzr, [x0, 304]
	ldr	x0, [x29, 40]
	str	xzr, [x0, 312]
	ldr	x0, [x29, 40]
	str	xzr, [x0, 288]
	ldr	x0, [x29, 40]
	str	xzr, [x0, 296]
	ldr	x0, [x29, 40]
	str	xzr, [x0, 320]
	ldr	x0, [x29, 40]
	str	xzr, [x0, 328]
	ldr	x0, [x29, 40]
	str	xzr, [x0, 368]
	ldr	x0, [x29, 40]
	str	xzr, [x0, 376]
	ldr	x0, [x29, 40]
	str	xzr, [x0, 352]
	ldr	x0, [x29, 40]
	str	xzr, [x0, 360]
	ldr	x0, [x29, 40]
	str	xzr, [x0, 384]
	ldr	x0, [x29, 40]
	str	xzr, [x0, 392]
	ldr	x0, [x29, 40]
	str	xzr, [x0, 432]
	ldr	x0, [x29, 40]
	str	xzr, [x0, 440]
	ldr	x0, [x29, 40]
	str	xzr, [x0, 416]
	ldr	x0, [x29, 40]
	str	xzr, [x0, 424]
	ldr	x0, [x29, 40]
	str	xzr, [x0, 448]
	ldr	x0, [x29, 40]
	str	xzr, [x0, 456]
	ldr	x0, [x29, 40]
	strh	wzr, [x0, 696]
	str	wzr, [x29, 60]
	b	.L1043
.L1044:
	ldr	w0, [x29, 60]
	uxth	w2, w0
	ldr	x1, [x29, 40]
	ldr	w0, [x29, 60]
	lsl	x0, x0, 5
	add	x0, x1, x0
	add	x0, x0, 704
	mov	w1, w2
	strh	w1, [x0]
	ldr	x1, [x29, 40]
	ldr	w0, [x29, 60]
	lsl	x0, x0, 5
	add	x0, x1, x0
	add	x0, x0, 706
	strh	wzr, [x0]
	ldr	x1, [x29, 40]
	ldr	w0, [x29, 60]
	lsl	x0, x0, 5
	add	x0, x1, x0
	add	x0, x0, 712
	str	xzr, [x0]
	ldr	x1, [x29, 40]
	ldr	w0, [x29, 60]
	add	x0, x0, 22
	lsl	x0, x0, 5
	add	x0, x1, x0
	add	x0, x0, 16
	str	xzr, [x0]
	ldr	x1, [x29, 40]
	ldr	w0, [x29, 60]
	add	x0, x0, 22
	lsl	x0, x0, 5
	add	x0, x1, x0
	add	x0, x0, 24
	str	xzr, [x0]
	ldr	w0, [x29, 60]
	add	w0, w0, 1
	str	w0, [x29, 60]
.L1043:
	ldr	w0, [x29, 60]
	cmp	w0, 9
	bls	.L1044
	ldr	x0, [x29, 40]
	mov	w1, 255
	strh	w1, [x0, 1024]
	ldr	x0, [x29, 40]
	strh	wzr, [x0, 698]
	ldr	x0, [x29, 40]
	strh	wzr, [x0, 1026]
	ldr	x0, [x29, 40]
	str	xzr, [x0, 1032]
	ldr	x0, [x29, 40]
	str	xzr, [x0, 1040]
	ldr	x0, [x29, 40]
	str	xzr, [x0, 1048]
	mov	w0, 0
.L1037:
	ldp	x29, x30, [sp], 64
	ret
	.size	zone_param_init, .-zone_param_init
	.align	2
	.global	zone_param_exit
	.type	zone_param_exit, %function
zone_param_exit:
	stp	x29, x30, [sp, -32]!
	add	x29, sp, 0
	str	x0, [x29, 24]
	ldr	x0, [x29, 24]
	bl	free_logic_page_map
	ldr	x0, [x29, 24]
	ldr	x0, [x0, 464]
	bl	nftl_free
	ldr	x0, [x29, 24]
	ldr	x0, [x0, 536]
	bl	nftl_free
	ldr	x0, [x29, 24]
	ldr	x0, [x0, 600]
	bl	nftl_free
	ldr	x0, [x29, 24]
	ldr	x0, [x0, 512]
	bl	nftl_free
	ldr	x0, [x29, 24]
	ldr	x0, [x0, 520]
	bl	nftl_free
	mov	w0, 0
	ldp	x29, x30, [sp], 32
	ret
	.size	zone_param_exit, .-zone_param_exit
	.section	.rodata
	.align	3
.LC208:
	.string	"[NE]mtd size changed !\n"
	.text
	.align	2
	.global	is_no_use_device
	.type	is_no_use_device, %function
is_no_use_device:
	stp	x29, x30, [sp, -96]!
	add	x29, sp, 0
	str	x0, [x29, 24]
	str	w1, [x29, 20]
	ldr	x0, [x29, 24]
	ldr	x0, [x0, 48]
	str	x0, [x29, 80]
	ldr	x0, [x29, 24]
	ldr	w0, [x0, 32]
	cmp	w0, 0
	beq	.L1048
	ldr	x0, [x29, 24]
	ldr	w1, [x0, 32]
	ldr	x0, [x29, 24]
	ldr	w0, [x0, 40]
	add	w0, w1, w0
	ldr	x1, [x29, 24]
	ldr	x1, [x1, 8]
	ldrb	w1, [x1, 8]
	udiv	w0, w0, w1
	str	w0, [x29, 92]
	ldr	x0, [x29, 24]
	ldr	x0, [x0, 8]
	ldrh	w0, [x0, 20]
	mov	w1, w0
	ldr	w0, [x29, 92]
	udiv	w0, w0, w1
	str	w0, [x29, 92]
	b	.L1049
.L1048:
	mov	w0, -1
	str	w0, [x29, 92]
.L1049:
	ldr	x0, [x29, 24]
	strh	wzr, [x0, 26]
	b	.L1050
.L1058:
	ldr	x0, [x29, 24]
	ldr	x1, [x29, 80]
	str	x1, [x0, 56]
	ldr	x0, [x29, 80]
	ldrh	w1, [x0]
	ldr	x0, [x29, 24]
	ldr	x0, [x0, 8]
	ldrh	w2, [x0, 28]
	ldr	x0, [x29, 24]
	ldr	x3, [x0, 512]
	add	x4, x29, 56
	add	x0, x29, 32
	mov	x5, x4
	mov	x4, x3
	mov	w3, w2
	mov	w2, 0
	bl	set_physic_op_par
	ldr	x0, [x29, 24]
	ldr	x2, [x0, 3488]
	add	x0, x29, 32
	mov	x1, x0
	ldr	x0, [x29, 24]
	blr	x2
	cmp	w0, 1
	bne	.L1051
	ldr	x0, [x29, 24]
	ldr	x2, [x0, 3464]
	add	x0, x29, 32
	mov	x1, x0
	ldr	x0, [x29, 24]
	blr	x2
	str	w0, [x29, 76]
	add	x0, x29, 56
	bl	is_ftl_start_flag_page
	cmp	w0, 1
	bne	.L1052
	ldr	w0, [x29, 76]
	cmp	w0, 0
	beq	.L1053
	ldr	w0, [x29, 76]
	cmp	w0, 10
	bne	.L1052
.L1053:
	add	x0, x29, 56
	mov	w2, 4
	mov	w1, 3
	bl	get_spare_data
	str	w0, [x29, 72]
	ldr	w1, [x29, 72]
	ldr	w0, [x29, 20]
	cmp	w1, w0
	beq	.L1054
	adrp	x0, .LC208
	add	x0, x0, :lo12:.LC208
	bl	nand_print
	mov	w0, 1
	b	.L1059
.L1054:
	ldr	x0, [x29, 80]
	add	x1, x0, 64
	ldr	x0, [x29, 24]
	str	x1, [x0, 48]
	mov	w0, 0
	b	.L1059
.L1052:
	mov	w0, 1
	b	.L1059
.L1051:
	ldr	x0, [x29, 80]
	mov	w1, 255
	strh	w1, [x0, 2]
	ldr	x0, [x29, 24]
	ldrh	w0, [x0, 26]
	add	w0, w0, 1
	uxth	w1, w0
	ldr	x0, [x29, 24]
	strh	w1, [x0, 26]
	ldr	x1, [x29, 24]
	ldr	x0, [x29, 80]
	bl	is_last_phy_block
	cmp	w0, 1
	beq	.L1060
	ldr	x0, [x29, 80]
	add	x0, x0, 64
	str	x0, [x29, 80]
.L1050:
	ldr	x0, [x29, 24]
	ldrh	w0, [x0, 26]
	mov	w1, w0
	ldr	w0, [x29, 92]
	cmp	w1, w0
	bcc	.L1058
	b	.L1057
.L1060:
	nop
.L1057:
	mov	w0, 1
.L1059:
	ldp	x29, x30, [sp], 96
	ret
	.size	is_no_use_device, .-is_no_use_device
	.align	2
	.global	get_vaild_blocks
	.type	get_vaild_blocks, %function
get_vaild_blocks:
	stp	x29, x30, [sp, -80]!
	add	x29, sp, 0
	str	x0, [x29, 24]
	str	w1, [x29, 20]
	str	w2, [x29, 16]
	ldr	x0, [x29, 24]
	ldr	x0, [x0, 8]
	ldr	x1, [x0, 32]
	ldr	w0, [x29, 20]
	lsl	x0, x0, 6
	add	x0, x1, x0
	str	x0, [x29, 64]
	ldr	w1, [x29, 16]
	ldr	w0, [x29, 20]
	sub	w0, w1, w0
	str	w0, [x29, 60]
	ldr	w0, [x29, 60]
	str	w0, [x29, 76]
	ldr	w0, [x29, 20]
	str	w0, [x29, 72]
	b	.L1062
.L1067:
	ldr	x0, [x29, 64]
	ldrh	w0, [x0]
	strh	w0, [x29, 34]
	ldr	x0, [x29, 24]
	ldr	x2, [x0, 3488]
	add	x0, x29, 32
	mov	x1, x0
	ldr	x0, [x29, 24]
	blr	x2
	cmp	w0, 159
	bne	.L1063
	ldr	w0, [x29, 76]
	sub	w0, w0, #1
	str	w0, [x29, 76]
	ldr	w0, [x29, 76]
	cmp	w0, 0
	bne	.L1063
	mov	w0, 0
	b	.L1068
.L1063:
	ldr	x1, [x29, 24]
	ldr	x0, [x29, 64]
	bl	is_last_phy_block
	cmp	w0, 1
	beq	.L1069
	ldr	x0, [x29, 64]
	add	x0, x0, 64
	str	x0, [x29, 64]
	ldr	w0, [x29, 72]
	add	w0, w0, 1
	str	w0, [x29, 72]
.L1062:
	ldr	w1, [x29, 72]
	ldr	w0, [x29, 60]
	cmp	w1, w0
	bcc	.L1067
	b	.L1066
.L1069:
	nop
.L1066:
	ldr	w0, [x29, 76]
.L1068:
	ldp	x29, x30, [sp], 80
	ret
	.size	get_vaild_blocks, .-get_vaild_blocks
	.section	.rodata
	.align	3
.LC209:
	.string	"[ND]build_zone_list_first.\n"
	.align	3
.LC210:
	.string	"[NE]no block %d\n"
	.text
	.align	2
	.global	build_zone_list_first
	.type	build_zone_list_first, %function
build_zone_list_first:
	stp	x29, x30, [sp, -96]!
	add	x29, sp, 0
	str	x0, [x29, 24]
	str	w1, [x29, 20]
	adrp	x0, .LC209
	add	x0, x0, :lo12:.LC209
	bl	nand_print_dbg
	ldr	x0, [x29, 24]
	ldr	x0, [x0, 48]
	str	x0, [x29, 80]
	ldr	x0, [x29, 24]
	ldr	w0, [x0, 32]
	cmp	w0, 0
	beq	.L1071
	ldr	x0, [x29, 24]
	ldr	w1, [x0, 32]
	ldr	x0, [x29, 24]
	ldr	w0, [x0, 40]
	add	w0, w1, w0
	ldr	x1, [x29, 24]
	ldr	x1, [x1, 8]
	ldrb	w1, [x1, 8]
	udiv	w0, w0, w1
	str	w0, [x29, 92]
	ldr	x0, [x29, 24]
	ldr	x0, [x0, 8]
	ldrh	w0, [x0, 20]
	mov	w1, w0
	ldr	w0, [x29, 92]
	udiv	w0, w0, w1
	str	w0, [x29, 92]
	b	.L1072
.L1071:
	mov	w0, -1
	str	w0, [x29, 92]
.L1072:
	ldr	x0, [x29, 24]
	strh	wzr, [x0, 26]
	b	.L1073
.L1082:
	ldr	x0, [x29, 24]
	ldr	x1, [x29, 80]
	str	x1, [x0, 56]
	ldr	x0, [x29, 80]
	ldrh	w1, [x0]
	ldr	x0, [x29, 24]
	ldr	x0, [x0, 8]
	ldrh	w2, [x0, 28]
	ldr	x0, [x29, 24]
	ldr	x3, [x0, 512]
	add	x4, x29, 56
	add	x0, x29, 32
	mov	x5, x4
	mov	x4, x3
	mov	w3, w2
	mov	w2, 0
	bl	set_physic_op_par
	ldr	x0, [x29, 24]
	ldr	x2, [x0, 3488]
	add	x0, x29, 32
	mov	x1, x0
	ldr	x0, [x29, 24]
	blr	x2
	cmp	w0, 1
	bne	.L1074
	ldr	x0, [x29, 24]
	ldr	x2, [x0, 3456]
	add	x0, x29, 32
	mov	x1, x0
	ldr	x0, [x29, 24]
	blr	x2
	str	w0, [x29, 76]
	ldr	w0, [x29, 76]
	cmp	w0, 0
	beq	.L1075
	ldr	x0, [x29, 80]
	mov	w1, 255
	strh	w1, [x0, 2]
	ldr	x0, [x29, 24]
	ldr	x2, [x0, 3496]
	add	x0, x29, 32
	mov	x1, x0
	ldr	x0, [x29, 24]
	blr	x2
	b	.L1079
.L1075:
	ldr	x0, [x29, 24]
	ldrh	w0, [x0, 26]
	add	w0, w0, 1
	uxth	w1, w0
	ldr	x0, [x29, 24]
	strh	w1, [x0, 26]
	ldr	x0, [x29, 80]
	strh	wzr, [x0, 2]
	ldr	x0, [x29, 80]
	strh	wzr, [x0, 6]
	ldr	x0, [x29, 24]
	ldrh	w0, [x0, 26]
	cmp	w0, 1
	bne	.L1079
	add	x0, x29, 56
	mov	w2, 16
	mov	w1, -1
	bl	nand_memset
	add	x0, x29, 56
	ldr	w2, [x29, 20]
	mov	x1, x0
	ldr	x0, [x29, 24]
	bl	set_start_block_flag
	ldr	x0, [x29, 80]
	ldrh	w1, [x0]
	ldr	x0, [x29, 24]
	ldr	x0, [x0, 8]
	ldrh	w2, [x0, 28]
	ldr	x0, [x29, 24]
	ldr	x3, [x0, 512]
	add	x4, x29, 56
	add	x0, x29, 32
	mov	x5, x4
	mov	x4, x3
	mov	w3, w2
	mov	w2, 0
	bl	set_physic_op_par
	str	wzr, [x29, 88]
	b	.L1077
.L1078:
	ldr	x0, [x29, 24]
	ldr	x2, [x0, 3472]
	add	x0, x29, 32
	mov	x1, x0
	ldr	x0, [x29, 24]
	blr	x2
	ldrh	w0, [x29, 32]
	add	w0, w0, 1
	uxth	w0, w0
	strh	w0, [x29, 32]
	ldr	w0, [x29, 88]
	add	w0, w0, 1
	str	w0, [x29, 88]
.L1077:
	ldr	x0, [x29, 24]
	ldr	x0, [x0, 8]
	ldrh	w0, [x0, 20]
	mov	w1, w0
	ldr	w0, [x29, 88]
	cmp	w1, w0
	bhi	.L1078
	ldr	x0, [x29, 80]
	add	x1, x0, 64
	ldr	x0, [x29, 24]
	str	x1, [x0, 48]
	b	.L1079
.L1074:
	ldr	x0, [x29, 80]
	mov	w1, 255
	strh	w1, [x0, 2]
.L1079:
	ldr	x0, [x29, 80]
	mov	w1, -1
	strh	w1, [x0, 4]
	ldr	x0, [x29, 80]
	str	xzr, [x0, 32]
	ldr	x0, [x29, 80]
	str	xzr, [x0, 16]
	ldr	x0, [x29, 80]
	str	xzr, [x0, 24]
	ldr	x0, [x29, 24]
	ldr	x1, [x29, 80]
	str	x1, [x0, 56]
	ldr	x1, [x29, 24]
	ldr	x0, [x29, 80]
	bl	is_last_phy_block
	cmp	w0, 1
	beq	.L1096
	ldr	x0, [x29, 80]
	add	x0, x0, 64
	str	x0, [x29, 80]
.L1073:
	ldr	x0, [x29, 24]
	ldrh	w0, [x0, 26]
	mov	w1, w0
	ldr	w0, [x29, 92]
	cmp	w1, w0
	bcc	.L1082
	b	.L1081
.L1096:
	nop
.L1081:
	ldr	x0, [x29, 24]
	ldrh	w0, [x0, 26]
	cmp	w0, 1
	bls	.L1083
	ldr	x0, [x29, 24]
	ldrh	w0, [x0, 26]
	sub	w0, w0, #1
	uxth	w1, w0
	ldr	x0, [x29, 24]
	strh	w1, [x0, 26]
	ldr	w0, [x29, 92]
	cmn	w0, #1
	bne	.L1086
	b	.L1095
.L1083:
	ldr	x0, [x29, 24]
	ldrh	w0, [x0, 26]
	mov	w1, w0
	adrp	x0, .LC210
	add	x0, x0, :lo12:.LC210
	bl	nand_print
	mov	w0, 1
	b	.L1094
.L1095:
	mov	w0, 0
	b	.L1094
.L1086:
	ldr	x1, [x29, 24]
	ldr	x0, [x29, 80]
	bl	is_last_phy_block
	cmp	w0, 1
	bne	.L1088
	mov	w0, 0
	b	.L1094
.L1093:
	ldr	x0, [x29, 80]
	ldrh	w1, [x0]
	ldr	x0, [x29, 24]
	ldr	x0, [x0, 8]
	ldrh	w2, [x0, 28]
	ldr	x0, [x29, 24]
	ldr	x3, [x0, 512]
	add	x4, x29, 56
	add	x0, x29, 32
	mov	x5, x4
	mov	x4, x3
	mov	w3, w2
	mov	w2, 0
	bl	set_physic_op_par
	ldr	x0, [x29, 24]
	ldr	x2, [x0, 3488]
	add	x0, x29, 32
	mov	x1, x0
	ldr	x0, [x29, 24]
	blr	x2
	cmp	w0, 1
	bne	.L1089
	ldr	x0, [x29, 24]
	ldr	x2, [x0, 3456]
	add	x0, x29, 32
	mov	x1, x0
	ldr	x0, [x29, 24]
	blr	x2
	str	w0, [x29, 76]
	ldr	w0, [x29, 76]
	cmp	w0, 0
	beq	.L1090
	ldr	x0, [x29, 80]
	mov	w1, 255
	strh	w1, [x0, 2]
	ldr	x0, [x29, 24]
	ldr	x2, [x0, 3496]
	add	x0, x29, 32
	mov	x1, x0
	ldr	x0, [x29, 24]
	blr	x2
	b	.L1091
.L1090:
	add	x0, x29, 56
	mov	w2, 16
	mov	w1, -1
	bl	nand_memset
	add	x0, x29, 56
	mov	x1, x0
	ldr	x0, [x29, 24]
	bl	set_end_block_flag
	ldr	x0, [x29, 80]
	ldrh	w1, [x0]
	ldr	x0, [x29, 24]
	ldr	x0, [x0, 8]
	ldrh	w2, [x0, 28]
	ldr	x0, [x29, 24]
	ldr	x3, [x0, 512]
	add	x4, x29, 56
	add	x0, x29, 32
	mov	x5, x4
	mov	x4, x3
	mov	w3, w2
	mov	w2, 0
	bl	set_physic_op_par
	ldr	x0, [x29, 24]
	ldr	x2, [x0, 3472]
	add	x0, x29, 32
	mov	x1, x0
	ldr	x0, [x29, 24]
	blr	x2
	b	.L1092
.L1089:
	ldr	x0, [x29, 80]
	mov	w1, 255
	strh	w1, [x0, 2]
.L1091:
	ldr	x0, [x29, 80]
	add	x0, x0, 64
	str	x0, [x29, 80]
.L1088:
	ldr	x0, [x29, 80]
	ldrh	w1, [x0]
	ldr	x0, [x29, 24]
	ldr	x0, [x0, 8]
	ldrh	w0, [x0, 24]
	cmp	w1, w0
	bcc	.L1093
.L1092:
	mov	w0, 0
.L1094:
	ldp	x29, x30, [sp], 96
	ret
	.size	build_zone_list_first, .-build_zone_list_first
	.section	.rodata
	.align	3
.LC211:
	.string	"[ND]first\n"
	.align	3
.LC212:
	.string	"[NE]init_zone_after_first_scan error\n"
	.align	3
.LC213:
	.string	"[NE]second_scan_all_blocks error\n"
	.align	3
.LC214:
	.string	"[NI]nftl ok!\n"
	.text
	.align	2
	.global	build_zone_list
	.type	build_zone_list, %function
build_zone_list:
	stp	x29, x30, [sp, -48]!
	add	x29, sp, 0
	str	x0, [x29, 24]
	adrp	x0, .LC211
	add	x0, x0, :lo12:.LC211
	bl	nand_print_dbg
	ldr	x0, [x29, 24]
	bl	first_scan_all_blocks
	cmp	w0, 0
	beq	.L1098
	mov	w0, 1
	b	.L1099
.L1098:
	ldr	x0, [x29, 24]
	ldrh	w0, [x0, 26]
	mov	w1, w0
	ldr	x0, [x29, 24]
	bl	init_zone_after_first_scan
	str	w0, [x29, 44]
	ldr	w0, [x29, 44]
	cmp	w0, 0
	beq	.L1100
	adrp	x0, .LC212
	add	x0, x0, :lo12:.LC212
	bl	nand_print
	mov	w0, 1
	b	.L1099
.L1100:
	ldr	x0, [x29, 24]
	bl	second_scan_all_blocks
	str	w0, [x29, 44]
	ldr	w0, [x29, 44]
	cmp	w0, 0
	beq	.L1101
	adrp	x0, .LC213
	add	x0, x0, :lo12:.LC213
	bl	nand_print
	mov	w0, 1
	b	.L1099
.L1101:
	ldr	x0, [x29, 24]
	bl	power_on_gc
	str	w0, [x29, 44]
	ldr	x0, [x29, 24]
	bl	print_nftl_zone
	adrp	x0, .LC214
	add	x0, x0, :lo12:.LC214
	bl	nand_print
	ldr	x0, [x29, 24]
	bl	panic_backup_block
	mov	w0, 0
.L1099:
	ldp	x29, x30, [sp], 48
	ret
	.size	build_zone_list, .-build_zone_list
	.section	.rodata
	.align	3
.LC215:
	.string	"[ND]fsab\n"
	.align	3
.LC216:
	.string	"[NI]ftl end block! %d!\n"
	.align	3
.LC217:
	.string	"[ND]READ_RECLAIM!\n"
	.align	3
.LC218:
	.string	"[NI]start page , do nothing\n"
	.align	3
.LC219:
	.string	"[ND]fill block %d\n"
	.align	3
.LC220:
	.string	"[NI]b@%d p@0 unknow spare data:%x,%x,%x,%x,%x,%x,%x,%x,%x,%x,%x,%x,%x,%x,%x,%x.\n"
	.align	3
.LC221:
	.string	"[NE]nftl bad block: %d\n"
	.align	3
.LC222:
	.string	"[NE]nftl b@%d p@0 ecc error\n"
	.align	3
.LC223:
	.string	"[NE]b@%d last page ok !\n"
	.align	3
.LC224:
	.string	"[NI]READ_RECLAIM!\n"
	.align	3
.LC225:
	.string	"[NE]last page blank ! %d!\n"
	.align	3
.LC226:
	.string	"[NE]last page unkown ! %d!\n"
	.align	3
.LC227:
	.string	"[NE]b@%d first page last page ecc error\n"
	.align	3
.LC228:
	.string	"[NE]b@%d last page spare data %x,%x,%x,%x,%x,%x,%x,%x,%x,%x,%x,%x,%x,%x,%x,%x\n"
	.align	3
.LC229:
	.string	"[NI]nftl bad block: %d\n"
	.text
	.align	2
	.global	first_scan_all_blocks
	.type	first_scan_all_blocks, %function
first_scan_all_blocks:
	sub	sp, sp, #208
	stp	x29, x30, [sp, 80]
	add	x29, sp, 80
	str	x0, [x29, 24]
	str	wzr, [x29, 124]
	ldr	x0, [x29, 24]
	ldr	x0, [x0, 48]
	str	x0, [x29, 112]
	str	xzr, [x29, 104]
	adrp	x0, .LC2
	add	x1, x0, :lo12:.LC2
	add	x0, x29, 48
	ldr	w2, [x1]
	str	w2, [x0]
	ldr	w1, [x1, 1]
	str	w1, [x0, 1]
	mov	w0, 47
	strb	w0, [x29, 40]
	strb	wzr, [x29, 41]
	adrp	x0, mod
	add	x1, x0, :lo12:mod
	add	x0, x29, 40
	mov	x2, x1
	mov	x1, x0
	mov	x0, 0
	bl	nand_fdt_get_prop_string_by_path
	str	x0, [x29, 104]
	ldr	x0, [x29, 104]
	cmp	x0, 0
	bne	.L1103
	mov	w0, -1
	b	.L1134
.L1103:
	add	x0, x29, 48
	mov	x2, 3
	mov	x1, x0
	ldr	x0, [x29, 104]
	bl	nand_memcmp
	cmp	w0, 0
	beq	.L1105
	mov	w0, -1
	b	.L1134
.L1105:
	adrp	x0, .LC215
	add	x0, x0, :lo12:.LC215
	bl	nand_print_dbg
	ldr	x0, [x29, 24]
	strh	wzr, [x0, 26]
	b	.L1106
.L1133:
	ldr	x0, [x29, 112]
	ldrh	w1, [x0]
	add	x2, x29, 80
	add	x0, x29, 56
	mov	x5, x2
	mov	x4, 0
	mov	w3, 0
	mov	w2, 0
	bl	set_physic_op_par
	ldr	x0, [x29, 24]
	ldr	x2, [x0, 3488]
	add	x0, x29, 56
	mov	x1, x0
	ldr	x0, [x29, 24]
	blr	x2
	cmp	w0, 1
	bne	.L1108
	ldr	x0, [x29, 24]
	ldr	x2, [x0, 3464]
	add	x0, x29, 56
	mov	x1, x0
	ldr	x0, [x29, 24]
	blr	x2
	str	w0, [x29, 100]
	ldr	x0, [x29, 24]
	ldr	x1, [x29, 112]
	str	x1, [x0, 56]
	add	x0, x29, 80
	bl	get_block_used_count_from_oob
	str	w0, [x29, 96]
	add	x0, x29, 80
	bl	is_ftl_end_flag_page
	cmp	w0, 1
	bne	.L1109
	ldr	x0, [x29, 112]
	ldrh	w0, [x0]
	mov	w1, w0
	adrp	x0, .LC216
	add	x0, x0, :lo12:.LC216
	bl	nand_print
	b	.L1110
.L1109:
	add	x0, x29, 80
	bl	is_panic_data_page
	cmp	w0, 1
	bne	.L1111
	ldrb	w1, [x29, 83]
	ldr	x0, [x29, 24]
	ldrb	w0, [x0, 24]
	cmp	w1, w0
	bne	.L1132
	add	x0, x29, 80
	ldr	x1, [x29, 112]
	bl	panic_recover_info
	b	.L1132
.L1111:
	add	x0, x29, 80
	bl	is_nouse_page
	cmp	w0, 1
	bne	.L1114
	ldr	x0, [x29, 24]
	ldrh	w0, [x0, 26]
	add	w0, w0, 1
	uxth	w1, w0
	ldr	x0, [x29, 24]
	strh	w1, [x0, 26]
	ldr	x0, [x29, 112]
	strh	wzr, [x0, 2]
	ldr	x0, [x29, 112]
	strh	wzr, [x0, 6]
	ldr	x0, [x29, 112]
	strh	wzr, [x0, 4]
	ldr	x1, [x29, 112]
	ldr	x0, [x29, 24]
	bl	put_phy_block_to_free_list
	b	.L1132
.L1114:
	add	x0, x29, 80
	bl	is_ftl_logic_page_data
	cmp	w0, 1
	beq	.L1115
	add	x0, x29, 80
	bl	is_function_info_page
	cmp	w0, 1
	bne	.L1116
.L1115:
	ldr	w0, [x29, 100]
	cmp	w0, 0
	beq	.L1117
	ldr	w0, [x29, 100]
	cmp	w0, 10
	bne	.L1116
.L1117:
	ldr	x0, [x29, 24]
	ldrh	w0, [x0, 26]
	add	w0, w0, 1
	uxth	w1, w0
	ldr	x0, [x29, 24]
	strh	w1, [x0, 26]
	ldr	x0, [x29, 112]
	mov	w1, 1
	strh	w1, [x0, 2]
	add	x0, x29, 80
	bl	get_erase_count_from_oob
	uxth	w1, w0
	ldr	x0, [x29, 112]
	strh	w1, [x0, 6]
	ldr	x0, [x29, 112]
	ldrh	w0, [x0, 6]
	str	w0, [x29, 124]
	ldr	x0, [x29, 112]
	strh	wzr, [x0, 4]
	ldr	w1, [x29, 96]
	ldr	x0, [x29, 112]
	str	w1, [x0, 8]
	ldr	x0, [x29, 112]
	str	xzr, [x0, 48]
	ldr	x0, [x29, 112]
	str	xzr, [x0, 56]
	ldr	x1, [x29, 112]
	ldr	x0, [x29, 24]
	bl	add_block_count_list
	ldr	w0, [x29, 100]
	cmp	w0, 10
	bne	.L1135
	ldr	x0, [x29, 24]
	ldr	x0, [x0, 3448]
	ldrh	w0, [x0, 4]
	cmp	w0, 0
	beq	.L1135
	mov	w2, 1
	ldr	x1, [x29, 112]
	ldr	x0, [x29, 24]
	bl	add_prio_gc
	adrp	x0, .LC217
	add	x0, x0, :lo12:.LC217
	bl	nand_print_dbg
	b	.L1135
.L1116:
	add	x0, x29, 80
	bl	is_ftl_start_flag_page
	cmp	w0, 1
	bne	.L1119
	adrp	x0, .LC218
	add	x0, x0, :lo12:.LC218
	bl	nand_print
	b	.L1132
.L1119:
	add	x0, x29, 80
	bl	is_fill_page
	cmp	w0, 1
	bne	.L1120
	ldr	x0, [x29, 24]
	ldrh	w0, [x0, 26]
	add	w0, w0, 1
	uxth	w1, w0
	ldr	x0, [x29, 24]
	strh	w1, [x0, 26]
	ldr	x0, [x29, 112]
	strh	wzr, [x0, 2]
	add	x0, x29, 80
	bl	get_erase_count_from_oob
	uxth	w1, w0
	ldr	x0, [x29, 112]
	strh	w1, [x0, 6]
	ldr	x0, [x29, 112]
	ldrh	w0, [x0, 6]
	str	w0, [x29, 124]
	ldr	x0, [x29, 24]
	ldr	x0, [x0, 8]
	ldrh	w1, [x0, 20]
	ldr	x0, [x29, 112]
	strh	w1, [x0, 4]
	ldr	x1, [x29, 112]
	ldr	x0, [x29, 24]
	bl	put_phy_block_to_free_list
	ldr	x0, [x29, 112]
	ldrh	w0, [x0]
	mov	w1, w0
	adrp	x0, .LC219
	add	x0, x0, :lo12:.LC219
	bl	nand_print_dbg
	b	.L1132
.L1120:
	ldr	x0, [x29, 112]
	ldrh	w0, [x0]
	mov	w11, w0
	ldrb	w0, [x29, 80]
	mov	w12, w0
	ldrb	w0, [x29, 81]
	mov	w13, w0
	ldrb	w0, [x29, 82]
	mov	w14, w0
	ldrb	w0, [x29, 83]
	mov	w15, w0
	ldrb	w0, [x29, 84]
	mov	w16, w0
	ldrb	w0, [x29, 85]
	mov	w17, w0
	ldrb	w0, [x29, 86]
	mov	w1, w0
	ldrb	w0, [x29, 87]
	mov	w2, w0
	ldrb	w0, [x29, 88]
	mov	w3, w0
	ldrb	w0, [x29, 89]
	mov	w4, w0
	ldrb	w0, [x29, 90]
	mov	w5, w0
	ldrb	w0, [x29, 91]
	mov	w6, w0
	ldrb	w0, [x29, 92]
	mov	w7, w0
	ldrb	w0, [x29, 93]
	mov	w8, w0
	ldrb	w0, [x29, 94]
	mov	w9, w0
	ldrb	w0, [x29, 95]
	mov	w10, w0
	adrp	x0, .LC220
	add	x0, x0, :lo12:.LC220
	str	w10, [sp, 72]
	str	w9, [sp, 64]
	str	w8, [sp, 56]
	str	w7, [sp, 48]
	str	w6, [sp, 40]
	str	w5, [sp, 32]
	str	w4, [sp, 24]
	str	w3, [sp, 16]
	str	w2, [sp, 8]
	str	w1, [sp]
	mov	w7, w17
	mov	w6, w16
	mov	w5, w15
	mov	w4, w14
	mov	w3, w13
	mov	w2, w12
	mov	w1, w11
	bl	nand_print
	ldr	w0, [x29, 124]
	uxth	w0, w0
	add	w0, w0, 1
	uxth	w1, w0
	ldr	x0, [x29, 112]
	strh	w1, [x0, 6]
	ldr	w0, [x29, 100]
	cmp	w0, 0
	beq	.L1121
	ldr	w0, [x29, 100]
	cmp	w0, 10
	bne	.L1122
.L1121:
	ldrb	w0, [x29, 80]
	cmp	w0, 255
	beq	.L1123
	ldr	x0, [x29, 112]
	ldrh	w0, [x0]
	mov	w1, w0
	adrp	x0, .LC221
	add	x0, x0, :lo12:.LC221
	bl	nand_print
	ldr	x0, [x29, 112]
	mov	w1, 255
	strh	w1, [x0, 2]
	ldr	x0, [x29, 24]
	ldrh	w0, [x0, 28]
	add	w0, w0, 1
	uxth	w1, w0
	ldr	x0, [x29, 24]
	strh	w1, [x0, 28]
	b	.L1136
.L1123:
	ldr	x0, [x29, 112]
	ldrh	w0, [x0, 6]
	add	w0, w0, 1
	uxth	w0, w0
	mov	w2, w0
	ldr	x1, [x29, 112]
	ldr	x0, [x29, 24]
	bl	erase_block
	cmp	w0, 0
	bne	.L1136
	ldr	x0, [x29, 24]
	ldrh	w0, [x0, 26]
	add	w0, w0, 1
	uxth	w1, w0
	ldr	x0, [x29, 24]
	strh	w1, [x0, 26]
	b	.L1136
.L1122:
	ldr	x0, [x29, 112]
	ldrh	w0, [x0]
	mov	w1, w0
	adrp	x0, .LC222
	add	x0, x0, :lo12:.LC222
	bl	nand_print
	ldr	x0, [x29, 112]
	ldrh	w1, [x0]
	ldr	x0, [x29, 24]
	ldr	x0, [x0, 8]
	ldrh	w0, [x0, 20]
	sub	w0, w0, #1
	uxth	w2, w0
	add	x3, x29, 80
	add	x0, x29, 56
	mov	x5, x3
	mov	x4, 0
	mov	w3, 0
	bl	set_physic_op_par
	ldr	x0, [x29, 24]
	ldr	x2, [x0, 3464]
	add	x0, x29, 56
	mov	x1, x0
	ldr	x0, [x29, 24]
	blr	x2
	str	w0, [x29, 100]
	ldr	w0, [x29, 100]
	cmp	w0, 0
	beq	.L1125
	ldr	w0, [x29, 100]
	cmp	w0, 10
	bne	.L1126
.L1125:
	add	x0, x29, 80
	bl	is_phy_mapping_page
	cmp	w0, 1
	bne	.L1127
	ldr	x0, [x29, 112]
	ldrh	w0, [x0]
	mov	w1, w0
	adrp	x0, .LC223
	add	x0, x0, :lo12:.LC223
	bl	nand_print
	add	x0, x29, 80
	bl	get_block_used_count_from_oob
	str	w0, [x29, 96]
	ldr	x0, [x29, 24]
	ldrh	w0, [x0, 26]
	add	w0, w0, 1
	uxth	w1, w0
	ldr	x0, [x29, 24]
	strh	w1, [x0, 26]
	ldr	x0, [x29, 112]
	mov	w1, 1
	strh	w1, [x0, 2]
	add	x0, x29, 80
	bl	get_erase_count_from_oob
	uxth	w1, w0
	ldr	x0, [x29, 112]
	strh	w1, [x0, 6]
	ldr	x0, [x29, 112]
	ldrh	w0, [x0, 6]
	str	w0, [x29, 124]
	ldr	x0, [x29, 112]
	strh	wzr, [x0, 4]
	ldr	w1, [x29, 96]
	ldr	x0, [x29, 112]
	str	w1, [x0, 8]
	ldr	x0, [x29, 112]
	str	xzr, [x0, 48]
	ldr	x0, [x29, 112]
	str	xzr, [x0, 56]
	ldr	x1, [x29, 112]
	ldr	x0, [x29, 24]
	bl	add_block_count_list
	ldr	w0, [x29, 100]
	cmp	w0, 10
	bne	.L1137
	ldr	x0, [x29, 24]
	ldr	x0, [x0, 3448]
	ldrh	w0, [x0, 4]
	cmp	w0, 0
	beq	.L1137
	mov	w2, 1
	ldr	x1, [x29, 112]
	ldr	x0, [x29, 24]
	bl	add_prio_gc
	adrp	x0, .LC224
	add	x0, x0, :lo12:.LC224
	bl	nand_print
	b	.L1137
.L1127:
	add	x0, x29, 80
	bl	is_nouse_page
	cmp	w0, 1
	bne	.L1130
	ldr	x0, [x29, 112]
	ldrh	w0, [x0]
	mov	w1, w0
	adrp	x0, .LC225
	add	x0, x0, :lo12:.LC225
	bl	nand_print
	ldr	x0, [x29, 112]
	ldrh	w0, [x0, 6]
	add	w0, w0, 1
	uxth	w0, w0
	mov	w2, w0
	ldr	x1, [x29, 112]
	ldr	x0, [x29, 24]
	bl	erase_block
	cmp	w0, 0
	bne	.L1137
	ldr	x0, [x29, 24]
	ldrh	w0, [x0, 26]
	add	w0, w0, 1
	uxth	w1, w0
	ldr	x0, [x29, 24]
	strh	w1, [x0, 26]
	b	.L1137
.L1130:
	ldr	x0, [x29, 112]
	ldrh	w0, [x0]
	mov	w1, w0
	adrp	x0, .LC226
	add	x0, x0, :lo12:.LC226
	bl	nand_print
	ldr	x0, [x29, 112]
	ldrh	w0, [x0, 6]
	add	w0, w0, 1
	uxth	w0, w0
	mov	w2, w0
	ldr	x1, [x29, 112]
	ldr	x0, [x29, 24]
	bl	erase_block
	cmp	w0, 0
	bne	.L1137
	ldr	x0, [x29, 24]
	ldrh	w0, [x0, 26]
	add	w0, w0, 1
	uxth	w1, w0
	ldr	x0, [x29, 24]
	strh	w1, [x0, 26]
	b	.L1137
.L1126:
	ldr	x0, [x29, 112]
	ldrh	w0, [x0]
	mov	w1, w0
	adrp	x0, .LC227
	add	x0, x0, :lo12:.LC227
	bl	nand_print
	ldr	x0, [x29, 112]
	ldrh	w0, [x0]
	mov	w11, w0
	ldrb	w0, [x29, 80]
	mov	w12, w0
	ldrb	w0, [x29, 81]
	mov	w13, w0
	ldrb	w0, [x29, 82]
	mov	w14, w0
	ldrb	w0, [x29, 83]
	mov	w15, w0
	ldrb	w0, [x29, 84]
	mov	w16, w0
	ldrb	w0, [x29, 85]
	mov	w17, w0
	ldrb	w0, [x29, 86]
	mov	w1, w0
	ldrb	w0, [x29, 87]
	mov	w2, w0
	ldrb	w0, [x29, 88]
	mov	w3, w0
	ldrb	w0, [x29, 89]
	mov	w4, w0
	ldrb	w0, [x29, 90]
	mov	w5, w0
	ldrb	w0, [x29, 91]
	mov	w6, w0
	ldrb	w0, [x29, 92]
	mov	w7, w0
	ldrb	w0, [x29, 93]
	mov	w8, w0
	ldrb	w0, [x29, 94]
	mov	w9, w0
	ldrb	w0, [x29, 95]
	mov	w10, w0
	adrp	x0, .LC228
	add	x0, x0, :lo12:.LC228
	str	w10, [sp, 72]
	str	w9, [sp, 64]
	str	w8, [sp, 56]
	str	w7, [sp, 48]
	str	w6, [sp, 40]
	str	w5, [sp, 32]
	str	w4, [sp, 24]
	str	w3, [sp, 16]
	str	w2, [sp, 8]
	str	w1, [sp]
	mov	w7, w17
	mov	w6, w16
	mov	w5, w15
	mov	w4, w14
	mov	w3, w13
	mov	w2, w12
	mov	w1, w11
	bl	nand_print
	ldr	x1, [x29, 112]
	ldr	x0, [x29, 24]
	bl	torture_block_and_mark
	cmp	w0, 0
	bne	.L1132
	ldr	x0, [x29, 24]
	ldrh	w0, [x0, 26]
	add	w0, w0, 1
	uxth	w1, w0
	ldr	x0, [x29, 24]
	strh	w1, [x0, 26]
	b	.L1132
.L1108:
	ldr	x0, [x29, 112]
	ldrh	w0, [x0]
	mov	w1, w0
	adrp	x0, .LC229
	add	x0, x0, :lo12:.LC229
	bl	nand_print
	ldr	x0, [x29, 112]
	mov	w1, 255
	strh	w1, [x0, 2]
	ldr	x0, [x29, 112]
	strh	wzr, [x0, 6]
	ldr	x0, [x29, 24]
	ldrh	w0, [x0, 28]
	add	w0, w0, 1
	uxth	w1, w0
	ldr	x0, [x29, 24]
	strh	w1, [x0, 28]
	b	.L1132
.L1135:
	nop
	b	.L1132
.L1136:
	nop
	b	.L1132
.L1137:
	nop
.L1132:
	ldr	x0, [x29, 112]
	add	x0, x0, 64
	str	x0, [x29, 112]
.L1106:
	ldr	x0, [x29, 112]
	ldrh	w1, [x0]
	ldr	x0, [x29, 24]
	ldr	x0, [x0, 8]
	ldrh	w0, [x0, 24]
	cmp	w1, w0
	bcc	.L1133
.L1110:
	mov	w0, 0
.L1134:
	add	sp, x29, 0
	ldp	x29, x30, [sp], 128
	ret
	.size	first_scan_all_blocks, .-first_scan_all_blocks
	.section	.rodata
	.align	3
.LC2:
	.byte	115
	.byte	117
	.byte	110
	.byte	120
	.byte	105
	.text
	.section	.rodata
	.align	3
.LC230:
	.string	"[ND]before second %d %d.\n"
	.align	3
.LC231:
	.string	"[NE]only have blocks:%u,logic cap need blocks:%u this zone not enough data block!!\n"
	.align	3
.LC232:
	.string	"[NE]this zone not enough free block %d , %d!!\n"
	.text
	.align	2
	.global	init_zone_after_first_scan
	.type	init_zone_after_first_scan, %function
init_zone_after_first_scan:
	stp	x29, x30, [sp, -48]!
	add	x29, sp, 0
	str	x0, [x29, 24]
	str	w1, [x29, 20]
	ldr	x0, [x29, 24]
	ldr	w0, [x0, 32]
	ldr	x1, [x29, 24]
	ldr	x1, [x1, 8]
	ldrb	w1, [x1, 8]
	udiv	w0, w0, w1
	str	w0, [x29, 44]
	ldr	x0, [x29, 24]
	ldr	x0, [x0, 8]
	ldrh	w0, [x0, 20]
	mov	w1, w0
	ldr	w0, [x29, 44]
	udiv	w0, w0, w1
	str	w0, [x29, 44]
	adrp	x0, .LC230
	add	x0, x0, :lo12:.LC230
	ldr	w2, [x29, 44]
	ldr	w1, [x29, 20]
	bl	nand_print_dbg
	ldr	w1, [x29, 20]
	ldr	w0, [x29, 44]
	cmp	w1, w0
	bhi	.L1139
	adrp	x0, .LC231
	add	x0, x0, :lo12:.LC231
	ldr	w2, [x29, 44]
	ldr	w1, [x29, 20]
	bl	nand_print
	mov	w0, 1
	b	.L1140
.L1139:
	ldr	w1, [x29, 20]
	ldr	w0, [x29, 44]
	sub	w0, w1, w0
	ldr	x1, [x29, 24]
	ldr	x1, [x1, 8]
	ldrb	w1, [x1, 8]
	mul	w0, w0, w1
	ldr	x1, [x29, 24]
	ldr	x1, [x1, 8]
	ldrh	w1, [x1, 20]
	mul	w1, w0, w1
	ldr	x0, [x29, 24]
	str	w1, [x0, 40]
	ldr	w1, [x29, 20]
	ldr	w0, [x29, 44]
	sub	w0, w1, w0
	ldr	x1, [x29, 24]
	ldr	x1, [x1, 3448]
	ldrh	w1, [x1, 12]
	sub	w1, w1, #1
	cmp	w0, w1
	bcs	.L1141
	adrp	x0, .LC232
	add	x0, x0, :lo12:.LC232
	ldr	w2, [x29, 44]
	ldr	w1, [x29, 20]
	bl	nand_print
	mov	w0, 1
	b	.L1140
.L1141:
	ldr	w1, [x29, 20]
	ldr	w0, [x29, 44]
	sub	w1, w1, w0
	mov	w0, 52429
	movk	w0, 0xcccc, lsl 16
	umull	x0, w1, w0
	lsr	x0, x0, 32
	lsr	w0, w0, 2
	uxth	w1, w0
	ldr	x0, [x29, 24]
	strh	w1, [x0, 680]
	ldr	x0, [x29, 24]
	ldrh	w1, [x0, 680]
	ldr	x0, [x29, 24]
	ldr	x0, [x0, 3448]
	ldrh	w0, [x0, 14]
	cmp	w1, w0
	bcs	.L1142
	ldr	x0, [x29, 24]
	ldr	x0, [x0, 3448]
	ldrh	w1, [x0, 14]
	ldr	x0, [x29, 24]
	strh	w1, [x0, 680]
.L1142:
	ldr	w1, [x29, 20]
	ldr	w0, [x29, 44]
	sub	w0, w1, w0
	ldr	x1, [x29, 24]
	ldr	x1, [x1, 3448]
	ldrh	w1, [x1, 18]
	mul	w0, w0, w1
	ldr	x1, [x29, 24]
	ldr	x1, [x1, 3448]
	ldrh	w1, [x1, 20]
	udiv	w0, w0, w1
	uxth	w1, w0
	ldr	x0, [x29, 24]
	strh	w1, [x0, 682]
	ldr	x0, [x29, 24]
	ldrh	w0, [x0, 682]
	mov	w1, w0
	ldr	x0, [x29, 24]
	ldrh	w0, [x0, 680]
	add	w0, w0, 2
	cmp	w1, w0
	bge	.L1143
	ldr	x0, [x29, 24]
	ldrh	w0, [x0, 680]
	add	w0, w0, 2
	uxth	w1, w0
	ldr	x0, [x29, 24]
	strh	w1, [x0, 682]
.L1143:
	ldr	x0, [x29, 24]
	mov	w1, 1
	strh	w1, [x0, 684]
	ldr	x0, [x29, 24]
	strh	wzr, [x0, 686]
	mov	w0, 0
.L1140:
	ldp	x29, x30, [sp], 48
	ret
	.size	init_zone_after_first_scan, .-init_zone_after_first_scan
	.section	.rodata
	.align	3
.LC233:
	.string	"[NI]NAND_EVENT: 2 current used block found %d,%d!\n"
	.align	3
.LC234:
	.string	"[NE]no crosstalk:2 block used count diff %d,%d %d,%d !\n"
	.align	3
.LC235:
	.string	"[NE]NAND_EVENT: muti current used block found1 %d %d!\n"
	.align	3
.LC236:
	.string	"[NE]NAND_EVENT: muti current used block found2 %d %d!\n"
	.align	3
.LC237:
	.string	"[NE]NAND_EVENT: muti current used block found3 %d %d!\n"
	.align	3
.LC238:
	.string	"[NE]NAND_EVENT: last page spare data %x,%x,%x,%x,%x,%x,%x %x %x %x %x %x %x %x %x %x!!\n"
	.align	3
.LC239:
	.string	"[NI]zone@%d b@%d last page unkown ecc ok.\n"
	.align	3
.LC240:
	.string	"[NE]zone@%d b@%d last page ecc error.\n"
	.align	3
.LC241:
	.string	"[NE]something strange %d,%d!!\n"
	.align	3
.LC242:
	.string	"[NI]all block full!!\n"
	.align	3
.LC243:
	.string	"[ND]corss talk rebuild 0 %d %d!!\n"
	.align	3
.LC244:
	.string	"[NE]1st current block:%d 1st current block uc:%d muti current used block 1st.\n"
	.align	3
.LC245:
	.string	"[ND]2nd current block:%d 2nd current block uc:%d muti current used block 2nd.\n"
	.align	3
.LC246:
	.string	"[NE]something is error here 7!\n"
	.align	3
.LC247:
	.string	"[NE]p_phy_block_info_last %d %d.\n"
	.align	3
.LC248:
	.string	"[NE]get_current_used_block fail.\n"
	.align	3
.LC249:
	.string	"[ND]free block nums is %d, aec: %d, iec: %d, a_buc: %d, i_buc: %d!\n"
	.text
	.align	2
	.global	second_scan_all_blocks
	.type	second_scan_all_blocks, %function
second_scan_all_blocks:
	sub	sp, sp, #256
	stp	x29, x30, [sp, 80]
	add	x29, sp, 80
	str	x0, [x29, 24]
	str	wzr, [x29, 160]
	str	xzr, [x29, 112]
	str	xzr, [x29, 104]
	str	xzr, [x29, 128]
	str	xzr, [x29, 136]
	str	xzr, [x29, 120]
	str	wzr, [x29, 100]
	adrp	x0, g_nreg_base
	add	x0, x0, :lo12:g_nreg_base
	ldr	x0, [x0]
	str	x0, [x29, 88]
	mov	w0, 188
	str	w0, [x29, 84]
	mov	w0, 1
	str	w0, [x29, 80]
	mov	w0, 2
	str	w0, [x29, 76]
	mov	w0, 1
	movk	w0, 0x3, lsl 16
	str	w0, [x29, 72]
	ldr	w0, [x29, 84]
	lsl	x0, x0, 2
	ldr	x1, [x29, 88]
	add	x0, x1, x0
	ldr	w0, [x0]
	str	w0, [x29, 100]
	ldr	w1, [x29, 100]
	ldr	w0, [x29, 72]
	cmp	w1, w0
	beq	.L1145
	ldr	w1, [x29, 100]
	ldr	w0, [x29, 80]
	cmp	w1, w0
	beq	.L1145
	ldr	w1, [x29, 100]
	ldr	w0, [x29, 76]
	cmp	w1, w0
	beq	.L1145
	mov	w0, -1
	b	.L1187
.L1145:
	mov	w0, -1
	str	w0, [x29, 172]
	ldr	x0, [x29, 24]
	ldr	w0, [x0, 32]
	ldr	x1, [x29, 24]
	ldr	x1, [x1, 8]
	ldrb	w1, [x1, 8]
	udiv	w1, w0, w1
	ldr	x0, [x29, 24]
	str	w1, [x0, 36]
	mov	w0, -1
	str	w0, [x29, 164]
	ldr	x0, [x29, 24]
	ldr	x0, [x0, 448]
	str	x0, [x29, 152]
	b	.L1147
.L1148:
	ldr	x0, [x29, 152]
	ldr	w0, [x0, 8]
	str	w0, [x29, 164]
	ldr	x0, [x29, 24]
	ldr	x0, [x0, 3440]
	ldr	x1, [x29, 152]
	ldr	w1, [x1, 8]
	str	w1, [x0, 156]
	ldr	x0, [x29, 152]
	ldr	x0, [x0, 48]
	str	x0, [x29, 152]
.L1147:
	ldr	x0, [x29, 152]
	cmp	x0, 0
	bne	.L1148
	ldr	x0, [x29, 24]
	ldr	x0, [x0, 448]
	cmp	x0, 0
	beq	.L1149
	ldr	x0, [x29, 24]
	ldr	x0, [x0, 3440]
	ldr	x1, [x29, 24]
	ldr	x1, [x1, 448]
	ldr	w1, [x1, 8]
	str	w1, [x0, 160]
.L1149:
	ldr	x0, [x29, 24]
	ldr	x0, [x0, 448]
	str	x0, [x29, 152]
	b	.L1150
.L1168:
	ldr	x0, [x29, 24]
	ldr	w0, [x0, 560]
	and	w0, w0, 511
	cmp	w0, 0
	beq	.L1151
	ldr	x0, [x29, 24]
	ldr	w0, [x0, 560]
	lsr	w0, w0, 9
	add	w0, w0, 1
	str	w0, [x29, 160]
	b	.L1152
.L1151:
	ldr	x0, [x29, 24]
	ldr	w0, [x0, 560]
	lsr	w0, w0, 9
	str	w0, [x29, 160]
.L1152:
	ldr	x0, [x29, 152]
	ldrh	w1, [x0]
	ldr	x0, [x29, 24]
	ldr	x0, [x0, 8]
	ldrh	w0, [x0, 20]
	sub	w0, w0, #1
	uxth	w2, w0
	ldr	w0, [x29, 160]
	uxth	w3, w0
	ldr	x0, [x29, 24]
	ldr	x4, [x0, 512]
	add	x5, x29, 56
	add	x0, x29, 32
	bl	set_physic_op_par
	ldr	x0, [x29, 24]
	ldr	x2, [x0, 3464]
	add	x0, x29, 32
	mov	x1, x0
	ldr	x0, [x29, 24]
	blr	x2
	str	w0, [x29, 168]
	ldr	x0, [x29, 24]
	ldr	x0, [x0, 3448]
	ldrh	w0, [x0, 28]
	cmp	w0, 0
	beq	.L1153
	ldr	x0, [x29, 24]
	ldr	x0, [x0, 512]
	add	x1, x29, 56
	mov	x2, x1
	mov	x1, x0
	ldr	x0, [x29, 24]
	bl	crc_wrong
	cmp	w0, 0
	beq	.L1153
	mov	w0, -2
	str	w0, [x29, 168]
.L1153:
	ldr	x0, [x29, 152]
	ldr	w0, [x0, 8]
	str	w0, [x29, 172]
	ldr	x0, [x29, 152]
	ldrh	w0, [x0, 6]
	mov	w1, w0
	ldr	x0, [x29, 24]
	ldr	w0, [x0, 676]
	cmp	w1, w0
	bls	.L1154
	ldr	x0, [x29, 152]
	ldrh	w1, [x0, 6]
	mov	w0, 49999
	cmp	w1, w0
	bhi	.L1154
	ldr	x0, [x29, 152]
	ldrh	w0, [x0, 6]
	mov	w1, w0
	ldr	x0, [x29, 24]
	str	w1, [x0, 676]
	ldr	x0, [x29, 24]
	ldr	x0, [x0, 3440]
	ldr	x1, [x29, 24]
	ldr	w1, [x1, 676]
	str	w1, [x0, 148]
.L1154:
	ldr	x0, [x29, 152]
	ldrh	w0, [x0, 6]
	mov	w1, w0
	ldr	x0, [x29, 24]
	ldr	x0, [x0, 3440]
	ldr	w0, [x0, 152]
	cmp	w1, w0
	bcs	.L1155
	ldr	x0, [x29, 24]
	ldr	x0, [x0, 3440]
	ldr	x1, [x29, 152]
	ldrh	w1, [x1, 6]
	str	w1, [x0, 152]
.L1155:
	add	x0, x29, 56
	bl	is_phy_mapping_page
	cmp	w0, 1
	bne	.L1156
	ldr	w0, [x29, 168]
	cmp	w0, 0
	beq	.L1157
	ldr	w0, [x29, 168]
	cmp	w0, 10
	bne	.L1156
.L1157:
	ldr	x0, [x29, 152]
	str	x0, [x29, 128]
	ldr	x0, [x29, 152]
	ldrh	w0, [x0, 4]
	add	w0, w0, 1
	uxth	w1, w0
	ldr	x0, [x29, 152]
	strh	w1, [x0, 4]
	ldr	x0, [x29, 24]
	ldr	x1, [x0, 512]
	ldr	x0, [x29, 24]
	ldr	x0, [x0, 8]
	ldrh	w0, [x0, 20]
	sub	w0, w0, #1
	mov	w3, w0
	mov	x2, x1
	ldr	x1, [x29, 152]
	ldr	x0, [x29, 24]
	bl	recover_logic_page_mapping
	ldr	x0, [x29, 152]
	str	x0, [x29, 120]
	b	.L1158
.L1156:
	add	x0, x29, 56
	bl	is_nouse_page
	cmp	w0, 1
	bne	.L1159
	ldr	x0, [x29, 112]
	cmp	x0, 0
	bne	.L1160
	ldr	x0, [x29, 152]
	str	x0, [x29, 112]
	b	.L1158
.L1160:
	ldr	x0, [x29, 104]
	cmp	x0, 0
	bne	.L1162
	ldr	x0, [x29, 152]
	str	x0, [x29, 104]
	ldr	x0, [x29, 112]
	ldrh	w0, [x0]
	mov	w1, w0
	ldr	x0, [x29, 104]
	ldrh	w0, [x0]
	mov	w2, w0
	adrp	x0, .LC233
	add	x0, x0, :lo12:.LC233
	bl	nand_print
	ldr	x0, [x29, 24]
	ldrb	w0, [x0, 25]
	and	w0, w0, 1
	cmp	w0, 0
	bne	.L1158
	ldr	x0, [x29, 112]
	ldr	w1, [x0, 8]
	ldr	x0, [x29, 104]
	ldr	w0, [x0, 8]
	cmp	w1, w0
	beq	.L1158
	ldr	x0, [x29, 112]
	ldrh	w0, [x0]
	mov	w5, w0
	ldr	x0, [x29, 104]
	ldrh	w0, [x0]
	mov	w6, w0
	ldr	x0, [x29, 112]
	ldr	w1, [x0, 8]
	ldr	x0, [x29, 104]
	ldr	w2, [x0, 8]
	adrp	x0, .LC234
	add	x0, x0, :lo12:.LC234
	mov	w4, w2
	mov	w3, w1
	mov	w2, w6
	mov	w1, w5
	bl	nand_print
	ldr	x0, [x29, 24]
	ldr	x0, [x0, 552]
	mov	x2, x0
	ldr	x1, [x29, 112]
	ldr	x0, [x29, 24]
	bl	recover_phy_page_mapping
	ldr	x0, [x29, 24]
	ldr	x1, [x0, 552]
	ldr	x0, [x29, 24]
	ldr	x0, [x0, 8]
	ldrh	w0, [x0, 20]
	sub	w0, w0, #1
	mov	w3, w0
	mov	x2, x1
	ldr	x1, [x29, 112]
	ldr	x0, [x29, 24]
	bl	recover_logic_page_mapping
	mov	w2, 3
	ldr	x1, [x29, 112]
	ldr	x0, [x29, 24]
	bl	add_prio_gc
	ldr	x0, [x29, 104]
	str	x0, [x29, 112]
	str	xzr, [x29, 104]
	b	.L1158
.L1162:
	ldr	x0, [x29, 112]
	ldrh	w0, [x0]
	mov	w3, w0
	ldr	x0, [x29, 112]
	ldr	w1, [x0, 8]
	adrp	x0, .LC235
	add	x0, x0, :lo12:.LC235
	mov	w2, w1
	mov	w1, w3
	bl	nand_print
	ldr	x0, [x29, 104]
	ldrh	w0, [x0]
	mov	w3, w0
	ldr	x0, [x29, 104]
	ldr	w1, [x0, 8]
	adrp	x0, .LC236
	add	x0, x0, :lo12:.LC236
	mov	w2, w1
	mov	w1, w3
	bl	nand_print
	ldr	x0, [x29, 152]
	ldrh	w0, [x0]
	mov	w1, w0
	adrp	x0, .LC237
	add	x0, x0, :lo12:.LC237
	ldr	w2, [x29, 172]
	bl	nand_print
	ldr	x0, [x29, 112]
	str	x0, [x29, 144]
	ldr	x0, [x29, 104]
	str	x0, [x29, 112]
	ldr	x0, [x29, 152]
	str	x0, [x29, 104]
	mov	w2, 3
	ldr	x1, [x29, 144]
	ldr	x0, [x29, 24]
	bl	add_prio_gc
	b	.L1158
.L1159:
	ldrb	w0, [x29, 56]
	mov	w10, w0
	ldrb	w0, [x29, 57]
	mov	w11, w0
	ldrb	w0, [x29, 58]
	mov	w12, w0
	ldrb	w0, [x29, 59]
	mov	w13, w0
	ldrb	w0, [x29, 60]
	mov	w14, w0
	ldrb	w0, [x29, 61]
	mov	w15, w0
	ldrb	w0, [x29, 62]
	mov	w16, w0
	ldrb	w0, [x29, 63]
	mov	w1, w0
	ldrb	w0, [x29, 64]
	mov	w2, w0
	ldrb	w0, [x29, 65]
	mov	w3, w0
	ldrb	w0, [x29, 66]
	mov	w4, w0
	ldrb	w0, [x29, 67]
	mov	w5, w0
	ldrb	w0, [x29, 68]
	mov	w6, w0
	ldrb	w0, [x29, 69]
	mov	w7, w0
	ldrb	w0, [x29, 70]
	mov	w8, w0
	ldrb	w0, [x29, 71]
	mov	w9, w0
	adrp	x0, .LC238
	add	x0, x0, :lo12:.LC238
	str	w9, [sp, 64]
	str	w8, [sp, 56]
	str	w7, [sp, 48]
	str	w6, [sp, 40]
	str	w5, [sp, 32]
	str	w4, [sp, 24]
	str	w3, [sp, 16]
	str	w2, [sp, 8]
	str	w1, [sp]
	mov	w7, w16
	mov	w6, w15
	mov	w5, w14
	mov	w4, w13
	mov	w3, w12
	mov	w2, w11
	mov	w1, w10
	bl	nand_print
	ldr	w0, [x29, 168]
	cmp	w0, 0
	beq	.L1164
	ldr	w0, [x29, 168]
	cmp	w0, 10
	bne	.L1165
.L1164:
	ldr	x0, [x29, 24]
	ldrb	w0, [x0, 24]
	mov	w1, w0
	ldr	x0, [x29, 152]
	ldrh	w0, [x0]
	mov	w2, w0
	adrp	x0, .LC239
	add	x0, x0, :lo12:.LC239
	bl	nand_print
	b	.L1166
.L1165:
	ldr	x0, [x29, 24]
	ldrb	w0, [x0, 24]
	mov	w1, w0
	ldr	x0, [x29, 152]
	ldrh	w0, [x0]
	mov	w2, w0
	adrp	x0, .LC240
	add	x0, x0, :lo12:.LC240
	bl	nand_print
.L1166:
	ldr	x0, [x29, 24]
	ldrb	w0, [x0, 25]
	and	w0, w0, 1
	cmp	w0, 0
	beq	.L1167
	ldr	w1, [x29, 164]
	ldr	w0, [x29, 172]
	cmp	w1, w0
	bne	.L1167
	ldr	x0, [x29, 152]
	ldrh	w0, [x0, 6]
	add	w0, w0, 1
	uxth	w0, w0
	mov	w2, w0
	ldr	x1, [x29, 152]
	ldr	x0, [x29, 24]
	bl	erase_block
	ldr	x0, [x29, 152]
	str	x0, [x29, 136]
	b	.L1158
.L1167:
	ldr	x0, [x29, 152]
	str	x0, [x29, 128]
	ldr	x0, [x29, 152]
	mov	w1, 170
	strh	w1, [x0, 2]
	ldr	x0, [x29, 152]
	ldrh	w0, [x0, 4]
	add	w0, w0, 1
	uxth	w1, w0
	ldr	x0, [x29, 152]
	strh	w1, [x0, 4]
	ldr	x0, [x29, 24]
	ldr	x0, [x0, 552]
	mov	x2, x0
	ldr	x1, [x29, 152]
	ldr	x0, [x29, 24]
	bl	recover_block_phy_page_mapping
	ldr	x0, [x29, 24]
	ldr	x1, [x0, 552]
	ldr	x0, [x29, 24]
	ldr	x0, [x0, 8]
	ldrh	w0, [x0, 20]
	sub	w0, w0, #1
	mov	w3, w0
	mov	x2, x1
	ldr	x1, [x29, 152]
	ldr	x0, [x29, 24]
	bl	recover_logic_page_mapping
	mov	w2, 3
	ldr	x1, [x29, 152]
	ldr	x0, [x29, 24]
	bl	add_prio_gc
.L1158:
	ldr	x0, [x29, 152]
	ldr	x0, [x0, 48]
	str	x0, [x29, 152]
.L1150:
	ldr	x0, [x29, 152]
	cmp	x0, 0
	bne	.L1168
	ldr	x1, [x29, 120]
	ldr	x0, [x29, 24]
	bl	init_smart_info
	ldr	x0, [x29, 24]
	ldr	x0, [x0, 448]
	str	x0, [x29, 144]
	b	.L1169
.L1171:
	ldr	x1, [x29, 144]
	ldr	x0, [x29, 112]
	cmp	x1, x0
	beq	.L1170
	ldr	x1, [x29, 144]
	ldr	x0, [x29, 104]
	cmp	x1, x0
	beq	.L1170
	ldr	x1, [x29, 144]
	ldr	x0, [x29, 136]
	cmp	x1, x0
	beq	.L1170
	ldr	x1, [x29, 144]
	ldr	x0, [x29, 24]
	bl	put_phy_block_to_invalid_page_list
.L1170:
	ldr	x0, [x29, 144]
	ldr	x0, [x0, 48]
	str	x0, [x29, 144]
.L1169:
	ldr	x0, [x29, 144]
	cmp	x0, 0
	bne	.L1171
	ldr	x0, [x29, 24]
	bl	adjust_invaild_list
	ldr	x0, [x29, 128]
	cmp	x0, 0
	beq	.L1172
	ldr	x0, [x29, 112]
	cmp	x0, 0
	beq	.L1172
	ldr	x0, [x29, 128]
	ldr	w0, [x0, 8]
	add	w1, w0, 1
	ldr	x0, [x29, 112]
	ldr	w0, [x0, 8]
	cmp	w1, w0
	beq	.L1172
	ldr	x0, [x29, 128]
	ldr	w1, [x0, 8]
	ldr	x0, [x29, 112]
	ldr	w2, [x0, 8]
	adrp	x0, .LC241
	add	x0, x0, :lo12:.LC241
	bl	nand_print
.L1172:
	ldr	x0, [x29, 24]
	str	xzr, [x0, 528]
	ldr	x0, [x29, 112]
	cmp	x0, 0
	bne	.L1173
	ldr	x0, [x29, 104]
	cmp	x0, 0
	bne	.L1173
	adrp	x0, .LC242
	add	x0, x0, :lo12:.LC242
	bl	nand_print
	str	xzr, [x29, 152]
	ldr	x0, [x29, 24]
	ldrb	w0, [x0, 25]
	and	w0, w0, 1
	cmp	w0, 0
	beq	.L1174
	ldr	x0, [x29, 128]
	cmp	x0, 0
	beq	.L1188
	mov	w2, 3
	ldr	x1, [x29, 128]
	ldr	x0, [x29, 24]
	bl	add_prio_gc
	b	.L1188
.L1174:
	ldr	x0, [x29, 128]
	cmp	x0, 0
	beq	.L1188
	ldr	x0, [x29, 128]
	ldrh	w0, [x0, 2]
	cmp	w0, 170
	bne	.L1188
	mov	w2, 3
	ldr	x1, [x29, 128]
	ldr	x0, [x29, 24]
	bl	add_prio_gc
	b	.L1188
.L1173:
	ldr	x0, [x29, 104]
	cmp	x0, 0
	bne	.L1178
	ldr	x0, [x29, 112]
	str	x0, [x29, 152]
	ldr	x0, [x29, 24]
	ldrb	w0, [x0, 25]
	and	w0, w0, 1
	cmp	w0, 0
	beq	.L1179
	ldr	x0, [x29, 112]
	ldrh	w0, [x0]
	mov	w3, w0
	ldr	x0, [x29, 112]
	ldr	w1, [x0, 8]
	adrp	x0, .LC243
	add	x0, x0, :lo12:.LC243
	mov	w2, w1
	mov	w1, w3
	bl	nand_print_dbg
	ldr	x0, [x29, 128]
	cmp	x0, 0
	beq	.L1180
	ldr	x0, [x29, 128]
	ldrh	w0, [x0, 2]
	cmp	w0, 170
	bne	.L1180
	mov	w2, 3
	ldr	x1, [x29, 128]
	ldr	x0, [x29, 24]
	bl	add_prio_gc
.L1180:
	mov	x2, 0
	ldr	x1, [x29, 112]
	ldr	x0, [x29, 24]
	bl	cross_talk_rebuild_current_block
	str	x0, [x29, 152]
	ldr	x0, [x29, 152]
	cmp	x0, 0
	bne	.L1177
	mov	w0, 1
	b	.L1187
.L1179:
	ldr	x0, [x29, 128]
	cmp	x0, 0
	beq	.L1182
	ldr	x0, [x29, 128]
	ldrh	w0, [x0, 2]
	cmp	w0, 170
	bne	.L1182
	mov	w2, 3
	ldr	x1, [x29, 128]
	ldr	x0, [x29, 24]
	bl	add_prio_gc
.L1182:
	ldr	x0, [x29, 24]
	ldrb	w0, [x0, 24]
	cmp	w0, 0
	bne	.L1183
	ldr	x0, [x29, 112]
	str	x0, [x29, 152]
	b	.L1177
.L1183:
	mov	x2, 0
	ldr	x1, [x29, 112]
	ldr	x0, [x29, 24]
	bl	no_cross_talk_rebuild_current_block
	str	x0, [x29, 152]
	b	.L1177
.L1178:
	ldr	x0, [x29, 24]
	ldrb	w0, [x0, 25]
	and	w0, w0, 1
	cmp	w0, 0
	beq	.L1184
	ldr	x2, [x29, 104]
	ldr	x1, [x29, 112]
	ldr	x0, [x29, 24]
	bl	cross_talk_rebuild_current_block
	str	x0, [x29, 152]
	ldr	x0, [x29, 152]
	cmp	x0, 0
	bne	.L1177
	mov	w0, 1
	b	.L1187
.L1184:
	ldr	x0, [x29, 112]
	ldr	w1, [x0, 8]
	ldr	x0, [x29, 104]
	ldr	w0, [x0, 8]
	cmp	w1, w0
	bne	.L1185
	ldr	x0, [x29, 112]
	ldrh	w0, [x0]
	mov	w3, w0
	ldr	x0, [x29, 112]
	ldr	w1, [x0, 8]
	adrp	x0, .LC244
	add	x0, x0, :lo12:.LC244
	mov	w2, w1
	mov	w1, w3
	bl	nand_print
	ldr	x0, [x29, 104]
	ldrh	w0, [x0]
	mov	w3, w0
	ldr	x0, [x29, 104]
	ldr	w1, [x0, 8]
	adrp	x0, .LC245
	add	x0, x0, :lo12:.LC245
	mov	w2, w1
	mov	w1, w3
	bl	nand_print_dbg
	ldr	x2, [x29, 104]
	ldr	x1, [x29, 112]
	ldr	x0, [x29, 24]
	bl	no_cross_talk_rebuild_current_block
	str	x0, [x29, 152]
	b	.L1177
.L1185:
	adrp	x0, .LC246
	add	x0, x0, :lo12:.LC246
	bl	nand_print
	ldr	x0, [x29, 128]
	ldrh	w0, [x0]
	mov	w3, w0
	ldr	x0, [x29, 128]
	ldr	w1, [x0, 8]
	adrp	x0, .LC247
	add	x0, x0, :lo12:.LC247
	mov	w2, w1
	mov	w1, w3
	bl	nand_print
	ldr	x0, [x29, 112]
	ldrh	w0, [x0]
	mov	w3, w0
	ldr	x0, [x29, 112]
	ldr	w1, [x0, 8]
	adrp	x0, .LC244
	add	x0, x0, :lo12:.LC244
	mov	w2, w1
	mov	w1, w3
	bl	nand_print
	ldr	x0, [x29, 104]
	ldrh	w0, [x0]
	mov	w3, w0
	ldr	x0, [x29, 104]
	ldr	w1, [x0, 8]
	adrp	x0, .LC245
	add	x0, x0, :lo12:.LC245
	mov	w2, w1
	mov	w1, w3
	bl	nand_print_dbg
	ldr	x0, [x29, 24]
	ldr	x0, [x0, 552]
	mov	x2, x0
	ldr	x1, [x29, 112]
	ldr	x0, [x29, 24]
	bl	recover_block_phy_page_mapping
	ldr	x0, [x29, 24]
	ldr	x1, [x0, 552]
	ldr	x0, [x29, 24]
	ldr	x0, [x0, 8]
	ldrh	w0, [x0, 20]
	sub	w0, w0, #1
	mov	w3, w0
	mov	x2, x1
	ldr	x1, [x29, 112]
	ldr	x0, [x29, 24]
	bl	recover_logic_page_mapping
	mov	w2, 3
	ldr	x1, [x29, 112]
	ldr	x0, [x29, 24]
	bl	add_prio_gc
	ldr	x0, [x29, 24]
	ldr	x0, [x0, 552]
	mov	x2, x0
	ldr	x1, [x29, 104]
	ldr	x0, [x29, 24]
	bl	recover_block_phy_page_mapping
	ldr	x0, [x29, 24]
	ldr	x1, [x0, 552]
	ldr	x0, [x29, 24]
	ldr	x0, [x0, 8]
	ldrh	w0, [x0, 20]
	sub	w0, w0, #1
	mov	w3, w0
	mov	x2, x1
	ldr	x1, [x29, 104]
	ldr	x0, [x29, 24]
	bl	recover_logic_page_mapping
	mov	w2, 3
	ldr	x1, [x29, 104]
	ldr	x0, [x29, 24]
	bl	add_prio_gc
	b	.L1177
.L1188:
	nop
.L1177:
	ldr	w2, [x29, 172]
	ldr	x1, [x29, 152]
	ldr	x0, [x29, 24]
	bl	get_current_used_block
	str	w0, [x29, 168]
	ldr	w0, [x29, 168]
	cmp	w0, 0
	beq	.L1186
	adrp	x0, .LC248
	add	x0, x0, :lo12:.LC248
	bl	nand_print
	mov	w0, 1
	b	.L1187
.L1186:
	ldr	x0, [x29, 24]
	bl	adjust_invaild_list
	ldr	x0, [x29, 24]
	ldr	x1, [x0, 528]
	ldr	x0, [x29, 24]
	str	x1, [x0, 64]
	ldr	x0, [x29, 24]
	ldrh	w0, [x0, 30]
	mov	w6, w0
	ldr	x0, [x29, 24]
	ldr	x0, [x0, 3440]
	ldr	w1, [x0, 148]
	ldr	x0, [x29, 24]
	ldr	x0, [x0, 3440]
	ldr	w2, [x0, 152]
	ldr	x0, [x29, 24]
	ldr	x0, [x0, 3440]
	ldr	w3, [x0, 156]
	ldr	x0, [x29, 24]
	ldr	x0, [x0, 3440]
	ldr	w4, [x0, 160]
	adrp	x0, .LC249
	add	x0, x0, :lo12:.LC249
	mov	w5, w4
	mov	w4, w3
	mov	w3, w2
	mov	w2, w1
	mov	w1, w6
	bl	nand_print_dbg
	mov	w0, 0
.L1187:
	add	sp, x29, 0
	ldp	x29, x30, [sp], 176
	ret
	.size	second_scan_all_blocks, .-second_scan_all_blocks
	.align	2
	.global	check_bit_nums
	.type	check_bit_nums, %function
check_bit_nums:
	sub	sp, sp, #32
	str	x0, [sp, 8]
	str	w1, [sp, 4]
	str	wzr, [sp, 20]
	str	wzr, [sp, 28]
	b	.L1190
.L1194:
	mov	w0, 1
	strb	w0, [sp, 19]
	str	wzr, [sp, 24]
	b	.L1191
.L1193:
	ldrsw	x0, [sp, 28]
	ldr	x1, [sp, 8]
	add	x0, x1, x0
	ldrb	w1, [x0]
	ldrb	w0, [sp, 19]
	and	w0, w1, w0
	uxtb	w0, w0
	cmp	w0, 0
	beq	.L1192
	ldr	w0, [sp, 20]
	add	w0, w0, 1
	str	w0, [sp, 20]
.L1192:
	ldrb	w0, [sp, 19]
	ubfiz	w0, w0, 1, 7
	strb	w0, [sp, 19]
	ldr	w0, [sp, 24]
	add	w0, w0, 1
	str	w0, [sp, 24]
.L1191:
	ldr	w0, [sp, 24]
	cmp	w0, 7
	ble	.L1193
	ldr	w0, [sp, 28]
	add	w0, w0, 1
	str	w0, [sp, 28]
.L1190:
	ldr	w1, [sp, 28]
	ldr	w0, [sp, 4]
	cmp	w1, w0
	blt	.L1194
	ldr	w0, [sp, 20]
	add	sp, sp, 32
	ret
	.size	check_bit_nums, .-check_bit_nums
	.align	2
	.global	is_all_bit_ff
	.type	is_all_bit_ff, %function
is_all_bit_ff:
	stp	x29, x30, [sp, -48]!
	add	x29, sp, 0
	str	x0, [x29, 24]
	str	w1, [x29, 20]
	str	w2, [x29, 16]
	ldr	w0, [x29, 20]
	lsl	w0, w0, 3
	str	w0, [x29, 44]
	ldr	w1, [x29, 44]
	ldr	w0, [x29, 16]
	sub	w0, w1, w0
	str	w0, [x29, 44]
	ldr	w1, [x29, 20]
	ldr	x0, [x29, 24]
	bl	check_bit_nums
	str	w0, [x29, 40]
	ldr	w1, [x29, 40]
	ldr	w0, [x29, 44]
	cmp	w1, w0
	blt	.L1197
	mov	w0, 1
	b	.L1198
.L1197:
	mov	w0, 0
.L1198:
	ldp	x29, x30, [sp], 48
	ret
	.size	is_all_bit_ff, .-is_all_bit_ff
	.align	2
	.global	is_all_byte_ff
	.type	is_all_byte_ff, %function
is_all_byte_ff:
	sub	sp, sp, #32
	str	x0, [sp, 8]
	str	w1, [sp, 4]
	str	w2, [sp]
	str	wzr, [sp, 28]
	ldr	w1, [sp, 4]
	ldr	w0, [sp]
	sub	w0, w1, w0
	str	w0, [sp, 20]
	str	wzr, [sp, 24]
	b	.L1200
.L1202:
	ldrsw	x0, [sp, 24]
	ldr	x1, [sp, 8]
	add	x0, x1, x0
	ldrb	w0, [x0]
	cmp	w0, 255
	bne	.L1201
	ldr	w0, [sp, 28]
	add	w0, w0, 1
	str	w0, [sp, 28]
.L1201:
	ldr	w0, [sp, 24]
	add	w0, w0, 1
	str	w0, [sp, 24]
.L1200:
	ldr	w1, [sp, 24]
	ldr	w0, [sp, 4]
	cmp	w1, w0
	blt	.L1202
	ldr	w1, [sp, 28]
	ldr	w0, [sp, 20]
	cmp	w1, w0
	blt	.L1203
	mov	w0, 1
	b	.L1204
.L1203:
	mov	w0, 0
.L1204:
	add	sp, sp, 32
	ret
	.size	is_all_byte_ff, .-is_all_byte_ff
	.section	.rodata
	.align	3
.LC250:
	.string	"[NE]current_block ecc error %d!\n"
	.align	3
.LC251:
	.string	"[NE]error! no free block!\n"
	.align	3
.LC252:
	.string	"[ND]recover %d %d\n"
	.align	3
.LC253:
	.string	"[NE]page used:%d invalid page count:%d error\n"
	.align	3
.LC254:
	.string	"[NE]new_block_init_for_write error here!\n"
	.align	3
.LC255:
	.string	"[NE]something is error here 10!\n"
	.align	3
.LC256:
	.string	"[NE]something is error here 11!\n"
	.align	3
.LC257:
	.string	"[NE]assist block page used:%d current block page used:%d something is error here 12.\n"
	.align	3
.LC258:
	.string	"[NE]assist block page used:%d current block page used:%d something is error here 17.\n"
	.align	3
.LC259:
	.string	"[NE]something is error here 16!\n"
	.align	3
.LC260:
	.string	"[ND]get a new free block\n"
	.align	3
.LC261:
	.string	"[NE]no free block to use!\n"
	.text
	.align	2
	.global	get_current_used_block
	.type	get_current_used_block, %function
get_current_used_block:
	stp	x29, x30, [sp, -64]!
	add	x29, sp, 0
	str	x0, [x29, 40]
	str	x1, [x29, 32]
	str	w2, [x29, 28]
	str	wzr, [x29, 56]
	ldr	x0, [x29, 40]
	ldr	x0, [x0, 528]
	cmp	x0, 0
	beq	.L1206
	mov	w0, 0
	b	.L1207
.L1206:
	ldr	x0, [x29, 32]
	cmp	x0, 0
	beq	.L1208
.L1209:
	ldr	x0, [x29, 40]
	ldr	x0, [x0, 552]
	mov	x2, x0
	ldr	x1, [x29, 32]
	ldr	x0, [x29, 40]
	bl	recover_phy_page_mapping
	uxth	w1, w0
	ldr	x0, [x29, 40]
	strh	w1, [x0, 584]
	ldr	x0, [x29, 40]
	ldrh	w1, [x0, 584]
	mov	w0, 65535
	cmp	w1, w0
	bne	.L1210
	mov	w0, 1
	str	w0, [x29, 56]
	ldr	x0, [x29, 32]
	ldrh	w0, [x0]
	mov	w1, w0
	adrp	x0, .LC250
	add	x0, x0, :lo12:.LC250
	bl	nand_print
	ldr	x1, [x29, 32]
	ldr	x0, [x29, 40]
	bl	current_block_ecc_error
	str	x0, [x29, 32]
	ldr	x0, [x29, 32]
	cmp	x0, 0
	beq	.L1211
	b	.L1209
.L1211:
	adrp	x0, .LC251
	add	x0, x0, :lo12:.LC251
	bl	nand_print
	mov	w0, 1
	b	.L1207
.L1210:
	ldr	x0, [x29, 40]
	ldr	x1, [x29, 32]
	str	x1, [x0, 528]
	ldr	x0, [x29, 40]
	ldr	x0, [x0, 528]
	ldrh	w0, [x0]
	mov	w1, w0
	ldr	x0, [x29, 40]
	ldrh	w0, [x0, 584]
	mov	w2, w0
	adrp	x0, .LC252
	add	x0, x0, :lo12:.LC252
	bl	nand_print_dbg
	ldr	x0, [x29, 40]
	ldr	x1, [x0, 528]
	ldr	x0, [x29, 40]
	ldr	x2, [x0, 552]
	ldr	x0, [x29, 40]
	ldrh	w0, [x0, 584]
	mov	w3, w0
	ldr	x0, [x29, 40]
	bl	recover_logic_page_mapping
	ldr	x0, [x29, 40]
	ldrh	w1, [x0, 584]
	ldr	x0, [x29, 40]
	ldr	x0, [x0, 528]
	ldrh	w0, [x0, 4]
	cmp	w1, w0
	bcs	.L1212
	ldr	x0, [x29, 40]
	ldrh	w0, [x0, 584]
	mov	w1, w0
	ldr	x0, [x29, 40]
	ldr	x0, [x0, 528]
	ldrh	w0, [x0, 4]
	mov	w2, w0
	adrp	x0, .LC253
	add	x0, x0, :lo12:.LC253
	bl	nand_print
.L1212:
	ldr	x0, [x29, 40]
	ldrb	w0, [x0, 25]
	and	w0, w0, 1
	cmp	w0, 0
	beq	.L1213
.L1214:
	ldr	x0, [x29, 40]
	ldr	x0, [x0, 592]
	cmp	x0, 0
	bne	.L1215
	ldr	x0, [x29, 40]
	bl	out_phy_block_from_free_list
	mov	x1, x0
	ldr	x0, [x29, 40]
	str	x1, [x0, 592]
	ldr	x0, [x29, 40]
	ldr	x0, [x0, 592]
	cmp	x0, 0
	bne	.L1216
	mov	w0, 1
	b	.L1207
.L1216:
	ldr	x0, [x29, 40]
	strh	wzr, [x0, 648]
	ldr	x0, [x29, 40]
	ldr	x0, [x0, 592]
	ldr	x1, [x29, 40]
	ldr	x1, [x1, 528]
	ldr	w1, [x1, 8]
	add	w1, w1, 1
	str	w1, [x0, 8]
	ldr	x0, [x29, 40]
	ldr	x1, [x0, 592]
	ldr	x0, [x29, 40]
	ldr	x0, [x0, 592]
	ldr	w0, [x0, 8]
	mov	w2, w0
	ldr	x0, [x29, 40]
	bl	new_block_init_for_write
	cmp	w0, 0
	beq	.L1217
	adrp	x0, .LC254
	add	x0, x0, :lo12:.LC254
	bl	nand_print
	ldr	x0, [x29, 40]
	str	xzr, [x0, 592]
	b	.L1214
.L1217:
	ldr	x0, [x29, 40]
	ldrh	w0, [x0, 584]
	str	w0, [x29, 52]
	str	wzr, [x29, 60]
	b	.L1218
.L1220:
	ldr	x0, [x29, 40]
	ldr	x5, [x0, 3480]
	ldr	x0, [x29, 40]
	ldr	x1, [x0, 528]
	ldr	x0, [x29, 40]
	ldr	x2, [x0, 592]
	ldr	x0, [x29, 40]
	ldr	x0, [x0, 512]
	ldr	w3, [x29, 60]
	uxth	w3, w3
	mov	w4, w3
	mov	x3, x0
	ldr	x0, [x29, 40]
	blr	x5
	str	w0, [x29, 48]
	ldr	w0, [x29, 48]
	cmp	w0, 0
	beq	.L1219
	adrp	x0, .LC255
	add	x0, x0, :lo12:.LC255
	bl	nand_print
.L1219:
	ldr	w0, [x29, 60]
	add	w0, w0, 1
	str	w0, [x29, 60]
.L1218:
	ldr	w1, [x29, 60]
	ldr	w0, [x29, 52]
	cmp	w1, w0
	bcc	.L1220
.L1215:
	ldr	x0, [x29, 40]
	ldr	x1, [x0, 592]
	ldr	x0, [x29, 40]
	ldr	x0, [x0, 616]
	mov	x2, x0
	ldr	x0, [x29, 40]
	bl	recover_phy_page_mapping
	uxth	w1, w0
	ldr	x0, [x29, 40]
	strh	w1, [x0, 648]
	ldr	x0, [x29, 40]
	ldrh	w1, [x0, 648]
	mov	w0, 65535
	cmp	w1, w0
	bne	.L1221
	adrp	x0, .LC256
	add	x0, x0, :lo12:.LC256
	bl	nand_print
	ldr	x0, [x29, 40]
	ldr	x1, [x0, 592]
	ldr	x0, [x29, 40]
	ldr	x0, [x0, 592]
	ldrh	w0, [x0, 6]
	add	w0, w0, 1
	uxth	w0, w0
	mov	w2, w0
	ldr	x0, [x29, 40]
	bl	erase_block
	ldr	x0, [x29, 40]
	str	xzr, [x0, 592]
	b	.L1214
.L1221:
	ldr	x0, [x29, 40]
	ldr	x0, [x0, 592]
	ldr	x1, [x29, 40]
	ldr	x1, [x1, 528]
	ldr	w1, [x1, 8]
	add	w1, w1, 1
	str	w1, [x0, 8]
	ldr	x0, [x29, 40]
	ldr	x1, [x0, 592]
	ldr	x0, [x29, 40]
	ldr	x2, [x0, 616]
	ldr	x0, [x29, 40]
	ldrh	w0, [x0, 648]
	mov	w3, w0
	ldr	x0, [x29, 40]
	bl	recover_logic_page_mapping
	ldr	x0, [x29, 40]
	ldrh	w1, [x0, 648]
	ldr	x0, [x29, 40]
	ldrh	w0, [x0, 584]
	cmp	w1, w0
	bls	.L1222
	ldr	x0, [x29, 40]
	ldrh	w0, [x0, 648]
	mov	w1, w0
	ldr	x0, [x29, 40]
	ldrh	w0, [x0, 584]
	mov	w2, w0
	adrp	x0, .LC257
	add	x0, x0, :lo12:.LC257
	bl	nand_print
	ldr	w0, [x29, 56]
	cmp	w0, 0
	beq	.L1223
	ldr	x0, [x29, 40]
	ldr	x1, [x0, 528]
	ldr	x0, [x29, 40]
	ldr	x0, [x0, 528]
	ldrh	w0, [x0, 6]
	add	w0, w0, 1
	uxth	w0, w0
	mov	w2, w0
	ldr	x0, [x29, 40]
	bl	erase_block
	ldr	x0, [x29, 40]
	ldr	x1, [x0, 592]
	ldr	x0, [x29, 40]
	str	x1, [x0, 528]
	ldr	x0, [x29, 40]
	ldrh	w1, [x0, 648]
	ldr	x0, [x29, 40]
	strh	w1, [x0, 584]
	ldr	x0, [x29, 40]
	ldr	x3, [x0, 536]
	ldr	x0, [x29, 40]
	ldr	x1, [x0, 600]
	ldr	x0, [x29, 40]
	ldr	w0, [x0, 544]
	mov	w2, w0
	mov	x0, x3
	bl	nand_memcpy
	ldr	x0, [x29, 40]
	str	xzr, [x0, 592]
	b	.L1214
.L1223:
	ldr	x0, [x29, 40]
	ldrh	w0, [x0, 648]
	mov	w1, w0
	ldr	x0, [x29, 40]
	ldrh	w0, [x0, 584]
	mov	w2, w0
	adrp	x0, .LC258
	add	x0, x0, :lo12:.LC258
	bl	nand_print
	b	.L1213
.L1222:
	ldr	x0, [x29, 40]
	ldrh	w1, [x0, 648]
	ldr	x0, [x29, 40]
	ldrh	w0, [x0, 584]
	cmp	w1, w0
	bcs	.L1213
	adrp	x0, .LC259
	add	x0, x0, :lo12:.LC259
	bl	nand_print
	ldr	x0, [x29, 40]
	ldr	x1, [x0, 592]
	ldr	x0, [x29, 40]
	ldr	x0, [x0, 592]
	ldrh	w0, [x0, 6]
	add	w0, w0, 1
	uxth	w0, w0
	mov	w2, w0
	ldr	x0, [x29, 40]
	bl	erase_block
	ldr	x0, [x29, 40]
	str	xzr, [x0, 592]
	b	.L1214
.L1208:
	adrp	x0, .LC260
	add	x0, x0, :lo12:.LC260
	bl	nand_print_dbg
	ldr	x0, [x29, 40]
	ldr	x0, [x0, 304]
	cmp	x0, 0
	beq	.L1224
	ldr	x0, [x29, 40]
	bl	out_phy_block_from_free_list
	mov	x1, x0
	ldr	x0, [x29, 40]
	str	x1, [x0, 528]
	ldr	x0, [x29, 40]
	ldr	x0, [x0, 528]
	cmp	x0, 0
	bne	.L1225
	mov	w0, 1
	b	.L1207
.L1225:
	ldr	x0, [x29, 40]
	strh	wzr, [x0, 584]
	ldr	x0, [x29, 40]
	ldr	x0, [x0, 528]
	ldr	w1, [x29, 28]
	add	w1, w1, 1
	str	w1, [x0, 8]
	ldr	x0, [x29, 40]
	ldrb	w0, [x0, 25]
	and	w0, w0, 1
	cmp	w0, 0
	beq	.L1226
	ldr	x0, [x29, 40]
	bl	out_phy_block_from_free_list
	mov	x1, x0
	ldr	x0, [x29, 40]
	str	x1, [x0, 592]
	ldr	x0, [x29, 40]
	ldr	x0, [x0, 592]
	cmp	x0, 0
	bne	.L1227
	mov	w0, 1
	b	.L1207
.L1227:
	ldr	x0, [x29, 40]
	strh	wzr, [x0, 648]
	ldr	x0, [x29, 40]
	ldr	x0, [x0, 592]
	ldr	x1, [x29, 40]
	ldr	x1, [x1, 528]
	ldr	w1, [x1, 8]
	add	w1, w1, 1
	str	w1, [x0, 8]
	b	.L1213
.L1226:
	ldr	x0, [x29, 40]
	str	xzr, [x0, 592]
	b	.L1213
.L1224:
	adrp	x0, .LC261
	add	x0, x0, :lo12:.LC261
	bl	nand_print
	ldr	x0, [x29, 40]
	bl	print_free_list
	ldr	x0, [x29, 40]
	bl	print_block_invalid_list
	mov	w0, 1
	b	.L1207
.L1213:
	mov	w0, 0
.L1207:
	ldp	x29, x30, [sp], 64
	ret
	.size	get_current_used_block, .-get_current_used_block
	.section	.rodata
	.align	3
.LC262:
	.string	"[NE]sb@%d sp@%d recover_phy_page_mapping no used page\n"
	.align	3
.LC263:
	.string	"[NE]sb@%d sp@%d power off ecc error1.\n"
	.align	3
.LC264:
	.string	"[NE]invalid logic_page block:%d page:%d,logic_page 0x%x.\n"
	.align	3
.LC265:
	.string	"[NE]rppm invalid page %d!!\n"
	.align	3
.LC266:
	.string	"[NI]power_down page %d!!\n"
	.align	3
.LC267:
	.string	"[NE]recover_phy_page_mapping unkown page data %d!!\n"
	.align	3
.LC268:
	.string	"[NE]recover_phy_page_mapping ecc error block:%d;page:%d.\n"
	.text
	.align	2
	.global	recover_phy_page_mapping
	.type	recover_phy_page_mapping, %function
recover_phy_page_mapping:
	stp	x29, x30, [sp, -112]!
	add	x29, sp, 0
	str	x19, [sp, 16]
	str	x0, [x29, 56]
	str	x1, [x29, 48]
	str	x2, [x29, 40]
	ldr	x0, [x29, 56]
	ldr	w0, [x0, 560]
	mov	w2, w0
	mov	w1, -1
	ldr	x0, [x29, 40]
	bl	nand_memset
	str	wzr, [x29, 108]
	b	.L1230
.L1245:
	ldr	x0, [x29, 48]
	ldrh	w1, [x0]
	ldr	w0, [x29, 108]
	uxth	w2, w0
	ldr	x0, [x29, 56]
	ldr	x0, [x0, 8]
	ldrh	w3, [x0, 28]
	ldr	x0, [x29, 56]
	ldr	x4, [x0, 512]
	add	x5, x29, 88
	add	x0, x29, 64
	bl	set_physic_op_par
	ldr	x0, [x29, 56]
	ldr	x2, [x0, 3464]
	add	x0, x29, 64
	mov	x1, x0
	ldr	x0, [x29, 56]
	blr	x2
	str	w0, [x29, 104]
	add	x0, x29, 88
	bl	is_nouse_page
	cmp	w0, 1
	bne	.L1231
	ldr	w0, [x29, 104]
	cmp	w0, 0
	beq	.L1232
	ldr	w0, [x29, 104]
	cmp	w0, 10
	bne	.L1231
.L1232:
	ldr	x0, [x29, 48]
	ldrh	w0, [x0]
	mov	w1, w0
	adrp	x0, .LC262
	add	x0, x0, :lo12:.LC262
	ldr	w2, [x29, 108]
	bl	nand_print
	b	.L1233
.L1231:
	add	x0, x29, 88
	bl	is_ftl_logic_page_data
	cmp	w0, 1
	bne	.L1234
	ldr	w0, [x29, 104]
	cmp	w0, 0
	beq	.L1235
	ldr	w0, [x29, 104]
	cmp	w0, 10
	bne	.L1238
.L1235:
	ldr	x0, [x29, 56]
	ldr	x0, [x0, 512]
	add	x1, x29, 88
	mov	x2, x1
	mov	x1, x0
	ldr	x0, [x29, 56]
	bl	nand_power_off_ecc_error
	cmp	w0, 0
	beq	.L1237
	ldr	x0, [x29, 48]
	ldrh	w0, [x0]
	mov	w1, w0
	adrp	x0, .LC263
	add	x0, x0, :lo12:.LC263
	ldr	w2, [x29, 108]
	bl	nand_print
	mov	w0, -2
	str	w0, [x29, 104]
	b	.L1238
.L1237:
	ldrsw	x0, [x29, 108]
	lsl	x0, x0, 2
	ldr	x1, [x29, 40]
	add	x19, x1, x0
	add	x0, x29, 88
	bl	get_logic_page_from_oob
	str	w0, [x19]
	ldrsw	x0, [x29, 108]
	lsl	x0, x0, 2
	ldr	x1, [x29, 40]
	add	x0, x1, x0
	ldr	w1, [x0]
	ldr	x0, [x29, 56]
	ldr	w0, [x0, 36]
	cmp	w1, w0
	bcc	.L1238
	ldr	x0, [x29, 48]
	ldrh	w0, [x0]
	mov	w4, w0
	ldrsw	x0, [x29, 108]
	lsl	x0, x0, 2
	ldr	x1, [x29, 40]
	add	x0, x1, x0
	ldr	w1, [x0]
	adrp	x0, .LC264
	add	x0, x0, :lo12:.LC264
	mov	w3, w1
	ldr	w2, [x29, 108]
	mov	w1, w4
	bl	nand_print
	ldrsw	x0, [x29, 108]
	lsl	x0, x0, 2
	ldr	x1, [x29, 40]
	add	x0, x1, x0
	mov	w1, -1
	str	w1, [x0]
	b	.L1238
.L1234:
	add	x0, x29, 88
	bl	is_ftl_special_data
	cmp	w0, 1
	bne	.L1239
	adrp	x0, .LC265
	add	x0, x0, :lo12:.LC265
	ldr	w1, [x29, 108]
	bl	nand_print
	b	.L1238
.L1239:
	add	x0, x29, 88
	bl	is_power_down_info_page
	cmp	w0, 1
	bne	.L1240
	ldr	w0, [x29, 104]
	cmp	w0, 0
	beq	.L1241
	ldr	w0, [x29, 104]
	cmp	w0, 10
	bne	.L1238
.L1241:
	ldrsw	x0, [x29, 108]
	lsl	x0, x0, 2
	ldr	x1, [x29, 40]
	add	x19, x1, x0
	add	x0, x29, 88
	bl	get_special_data_from_oob
	str	w0, [x19]
	adrp	x0, .LC266
	add	x0, x0, :lo12:.LC266
	ldr	w1, [x29, 108]
	bl	nand_print
	b	.L1238
.L1240:
	adrp	x0, .LC267
	add	x0, x0, :lo12:.LC267
	ldr	w1, [x29, 108]
	bl	nand_print
.L1238:
	ldr	w0, [x29, 104]
	cmp	w0, 0
	beq	.L1243
	ldr	w0, [x29, 104]
	cmp	w0, 10
	beq	.L1243
	ldr	x0, [x29, 48]
	ldrh	w0, [x0]
	mov	w1, w0
	adrp	x0, .LC268
	add	x0, x0, :lo12:.LC268
	ldr	w2, [x29, 108]
	bl	nand_print
	mov	w0, 65535
	b	.L1247
.L1243:
	ldr	w0, [x29, 108]
	add	w0, w0, 1
	str	w0, [x29, 108]
.L1230:
	ldr	x0, [x29, 56]
	ldr	x0, [x0, 8]
	ldrh	w0, [x0, 20]
	mov	w1, w0
	ldr	w0, [x29, 108]
	cmp	w1, w0
	bgt	.L1245
.L1233:
	ldr	x0, [x29, 56]
	ldr	x0, [x0, 8]
	ldrh	w0, [x0, 20]
	sub	w1, w0, #2
	ldr	w0, [x29, 108]
	cmp	w1, w0
	ldr	w0, [x29, 108]
.L1247:
	ldr	x19, [sp, 16]
	ldp	x29, x30, [sp], 112
	ret
	.size	recover_phy_page_mapping, .-recover_phy_page_mapping
	.section	.rodata
	.align	3
.LC269:
	.string	"[NE]ret:%d read fail %d %d,read one more time!!\n"
	.align	3
.LC270:
	.string	"[NE]ret:%d read fail %d %d,%d times,don't read more!!\n"
	.align	3
.LC271:
	.string	"[NE]power off ecc error 3!!\n"
	.align	3
.LC272:
	.string	"[NE]recover sb@%d sp@%d phy page mapping error.\n"
	.text
	.align	2
	.global	recover_block_phy_page_mapping
	.type	recover_block_phy_page_mapping, %function
recover_block_phy_page_mapping:
	stp	x29, x30, [sp, -128]!
	add	x29, sp, 0
	str	x19, [sp, 16]
	str	x0, [x29, 56]
	str	x1, [x29, 48]
	str	x2, [x29, 40]
	str	wzr, [x29, 116]
	ldr	x0, [x29, 56]
	ldr	w0, [x0, 560]
	mov	w2, w0
	mov	w1, -1
	ldr	x0, [x29, 40]
	bl	nand_memset
	str	wzr, [x29, 124]
	b	.L1249
.L1263:
	ldr	x0, [x29, 56]
	ldr	x0, [x0, 3448]
	ldrh	w0, [x0, 28]
	cmp	w0, 0
	beq	.L1250
	ldr	x0, [x29, 48]
	ldrh	w1, [x0]
	ldr	w0, [x29, 124]
	uxth	w2, w0
	ldr	x0, [x29, 56]
	ldr	x0, [x0, 8]
	ldrh	w3, [x0, 28]
	ldr	x0, [x29, 56]
	ldr	x4, [x0, 512]
	add	x5, x29, 96
	add	x0, x29, 72
	bl	set_physic_op_par
	b	.L1251
.L1250:
	ldr	x0, [x29, 48]
	ldrh	w1, [x0]
	ldr	w0, [x29, 124]
	uxth	w2, w0
	add	x3, x29, 96
	add	x0, x29, 72
	mov	x5, x3
	mov	x4, 0
	mov	w3, 0
	bl	set_physic_op_par
.L1251:
	str	wzr, [x29, 116]
	b	.L1252
.L1257:
	ldr	x0, [x29, 56]
	ldr	x2, [x0, 3464]
	add	x0, x29, 72
	mov	x1, x0
	ldr	x0, [x29, 56]
	blr	x2
	str	w0, [x29, 120]
	ldr	w0, [x29, 120]
	cmp	w0, 0
	bge	.L1265
	ldr	x0, [x29, 48]
	ldrh	w0, [x0]
	mov	w1, w0
	adrp	x0, .LC269
	add	x0, x0, :lo12:.LC269
	ldr	w3, [x29, 124]
	mov	w2, w1
	ldr	w1, [x29, 120]
	bl	nand_print
	ldr	w0, [x29, 116]
	cmp	w0, 1
	bne	.L1256
	ldr	x0, [x29, 48]
	ldrh	w0, [x0]
	mov	w1, w0
	adrp	x0, .LC270
	add	x0, x0, :lo12:.LC270
	ldr	w4, [x29, 116]
	ldr	w3, [x29, 124]
	mov	w2, w1
	ldr	w1, [x29, 120]
	bl	nand_print
.L1256:
	ldr	w0, [x29, 116]
	add	w0, w0, 1
	str	w0, [x29, 116]
.L1252:
	ldr	w0, [x29, 116]
	cmp	w0, 1
	ble	.L1257
	b	.L1255
.L1265:
	nop
.L1255:
	add	x0, x29, 96
	bl	is_ftl_logic_page_data
	cmp	w0, 1
	bne	.L1258
	ldr	w0, [x29, 120]
	cmp	w0, 0
	beq	.L1259
	ldr	w0, [x29, 120]
	cmp	w0, 10
	bne	.L1262
.L1259:
	ldr	x0, [x29, 56]
	ldr	x0, [x0, 512]
	add	x1, x29, 96
	mov	x2, x1
	mov	x1, x0
	ldr	x0, [x29, 56]
	bl	nand_power_off_ecc_error
	cmp	w0, 0
	beq	.L1261
	adrp	x0, .LC271
	add	x0, x0, :lo12:.LC271
	bl	nand_print
	b	.L1262
.L1261:
	ldrsw	x0, [x29, 124]
	lsl	x0, x0, 2
	ldr	x1, [x29, 40]
	add	x19, x1, x0
	add	x0, x29, 96
	bl	get_logic_page_from_oob
	str	w0, [x19]
	ldrsw	x0, [x29, 124]
	lsl	x0, x0, 2
	ldr	x1, [x29, 40]
	add	x0, x1, x0
	ldr	w1, [x0]
	ldr	x0, [x29, 56]
	ldr	w0, [x0, 36]
	cmp	w1, w0
	bcc	.L1262
	ldr	x0, [x29, 48]
	ldrh	w0, [x0]
	mov	w4, w0
	ldrsw	x0, [x29, 124]
	lsl	x0, x0, 2
	ldr	x1, [x29, 40]
	add	x0, x1, x0
	ldr	w1, [x0]
	adrp	x0, .LC264
	add	x0, x0, :lo12:.LC264
	mov	w3, w1
	ldr	w2, [x29, 124]
	mov	w1, w4
	bl	nand_print
	ldrsw	x0, [x29, 124]
	lsl	x0, x0, 2
	ldr	x1, [x29, 40]
	add	x0, x1, x0
	mov	w1, -1
	str	w1, [x0]
	b	.L1262
.L1258:
	add	x0, x29, 96
	bl	is_nouse_page
	cmp	w0, 1
	beq	.L1262
	ldr	x0, [x29, 48]
	ldrh	w0, [x0]
	mov	w1, w0
	adrp	x0, .LC272
	add	x0, x0, :lo12:.LC272
	ldr	w2, [x29, 124]
	bl	nand_print
.L1262:
	ldr	w0, [x29, 124]
	add	w0, w0, 1
	str	w0, [x29, 124]
.L1249:
	ldr	x0, [x29, 56]
	ldr	x0, [x0, 8]
	ldrh	w0, [x0, 20]
	sub	w1, w0, #1
	ldr	w0, [x29, 124]
	cmp	w1, w0
	bgt	.L1263
	mov	w0, 0
	ldr	x19, [sp, 16]
	ldp	x29, x30, [sp], 128
	ret
	.size	recover_block_phy_page_mapping, .-recover_block_phy_page_mapping
	.section	.rodata
	.align	3
.LC273:
	.string	"[NI]logic_page_map is NULL\n"
	.align	3
.LC274:
	.string	"[NE]invalid_page_count more than pages_per_blk :%d\n"
	.align	3
.LC275:
	.string	"[NE]invalid_page data block:%d page:%d,logic_page 0x%x!!\n"
	.align	3
.LC276:
	.string	"[NE]build invalid_page_count more than pages_per_blk!!\n"
	.text
	.align	2
	.global	recover_logic_page_mapping
	.type	recover_logic_page_mapping, %function
recover_logic_page_mapping:
	stp	x29, x30, [sp, -80]!
	add	x29, sp, 0
	str	x0, [x29, 40]
	str	x1, [x29, 32]
	str	x2, [x29, 24]
	str	w3, [x29, 20]
	str	wzr, [x29, 76]
	b	.L1267
.L1275:
	ldr	w0, [x29, 76]
	lsl	x0, x0, 2
	ldr	x1, [x29, 24]
	add	x0, x1, x0
	ldr	w0, [x0]
	str	w0, [x29, 72]
	ldr	x0, [x29, 40]
	ldr	w1, [x0, 36]
	ldr	w0, [x29, 72]
	cmp	w1, w0
	bls	.L1268
	ldr	w1, [x29, 72]
	ldr	x0, [x29, 40]
	bl	get_logic_page_map
	str	x0, [x29, 64]
	ldr	x0, [x29, 64]
	cmp	x0, 0
	bne	.L1269
	adrp	x0, .LC273
	add	x0, x0, :lo12:.LC273
	bl	nand_print
	ldr	x0, [x29, 32]
	ldrh	w0, [x0, 4]
	add	w0, w0, 1
	uxth	w1, w0
	ldr	x0, [x29, 32]
	strh	w1, [x0, 4]
	b	.L1270
.L1269:
	ldr	x0, [x29, 64]
	ldrh	w1, [x0, 2]
	mov	w0, 65535
	cmp	w1, w0
	beq	.L1271
	ldr	x0, [x29, 64]
	ldrh	w0, [x0, 2]
	mov	w1, w0
	ldr	x0, [x29, 40]
	bl	get_phy_block_addr
	str	x0, [x29, 56]
	ldr	x1, [x29, 56]
	ldr	x0, [x29, 32]
	cmp	x1, x0
	beq	.L1272
	ldr	x1, [x29, 56]
	ldr	x0, [x29, 40]
	bl	phy_block_from_invalid_page_incr
	b	.L1273
.L1272:
	ldr	x0, [x29, 56]
	ldrh	w0, [x0, 4]
	add	w0, w0, 1
	uxth	w1, w0
	ldr	x0, [x29, 56]
	strh	w1, [x0, 4]
.L1273:
	ldr	x0, [x29, 56]
	ldrh	w1, [x0, 4]
	ldr	x0, [x29, 40]
	ldr	x0, [x0, 8]
	ldrh	w0, [x0, 20]
	cmp	w1, w0
	bls	.L1271
	ldr	x0, [x29, 56]
	ldrh	w0, [x0]
	mov	w1, w0
	adrp	x0, .LC274
	add	x0, x0, :lo12:.LC274
	bl	nand_print
.L1271:
	ldr	w0, [x29, 76]
	uxth	w0, w0
	and	w0, w0, 4095
	uxth	w2, w0
	ldr	x1, [x29, 64]
	ldrh	w0, [x1]
	bfi	w0, w2, 0, 12
	strh	w0, [x1]
	ldr	x0, [x29, 32]
	ldrh	w1, [x0]
	ldr	x0, [x29, 64]
	strh	w1, [x0, 2]
	b	.L1270
.L1268:
	ldr	w0, [x29, 72]
	cmn	w0, #1
	beq	.L1274
	ldr	x0, [x29, 32]
	ldrh	w0, [x0]
	mov	w1, w0
	adrp	x0, .LC275
	add	x0, x0, :lo12:.LC275
	ldr	w3, [x29, 72]
	ldr	w2, [x29, 76]
	bl	nand_print
.L1274:
	ldr	x0, [x29, 32]
	ldrh	w0, [x0, 4]
	add	w0, w0, 1
	uxth	w1, w0
	ldr	x0, [x29, 32]
	strh	w1, [x0, 4]
	ldr	x0, [x29, 32]
	ldrh	w1, [x0, 4]
	ldr	x0, [x29, 40]
	ldr	x0, [x0, 8]
	ldrh	w0, [x0, 20]
	cmp	w1, w0
	bls	.L1270
	adrp	x0, .LC276
	add	x0, x0, :lo12:.LC276
	bl	nand_print
.L1270:
	ldr	w0, [x29, 76]
	add	w0, w0, 1
	str	w0, [x29, 76]
.L1267:
	ldr	w1, [x29, 76]
	ldr	w0, [x29, 20]
	cmp	w1, w0
	bcc	.L1275
	mov	w0, 0
	ldp	x29, x30, [sp], 80
	ret
	.size	recover_logic_page_mapping, .-recover_logic_page_mapping
	.section	.rodata
	.align	3
.LC277:
	.string	"[NE]NAND EVENT:do_write_error erase error1!\n"
	.align	3
.LC278:
	.string	"[NE]NAND EVENT:do_write_error erase error2!\n"
	.text
	.align	2
	.global	do_write_error_in_build_list
	.type	do_write_error_in_build_list, %function
do_write_error_in_build_list:
	stp	x29, x30, [sp, -96]!
	add	x29, sp, 0
	str	x0, [x29, 40]
	str	x1, [x29, 32]
	str	x2, [x29, 24]
	strh	w3, [x29, 22]
	ldr	x0, [x29, 24]
	str	x0, [x29, 80]
	ldr	x0, [x29, 80]
	ldrh	w1, [x0]
	ldr	x0, [x29, 40]
	ldr	x0, [x0, 8]
	ldrh	w2, [x0, 28]
	add	x0, x29, 48
	mov	x5, 0
	mov	x4, 0
	mov	w3, w2
	mov	w2, 0
	bl	set_physic_op_par
	ldr	x0, [x29, 40]
	ldr	x2, [x0, 3456]
	add	x0, x29, 48
	mov	x1, x0
	ldr	x0, [x29, 40]
	blr	x2
	str	w0, [x29, 76]
	ldr	x0, [x29, 80]
	strh	wzr, [x0, 4]
	ldr	x0, [x29, 80]
	ldrh	w0, [x0, 6]
	add	w0, w0, 1
	uxth	w1, w0
	ldr	x0, [x29, 80]
	strh	w1, [x0, 6]
	ldr	w0, [x29, 76]
	cmp	w0, 0
	beq	.L1278
	adrp	x0, .LC277
	add	x0, x0, :lo12:.LC277
	bl	nand_print
	ldr	x0, [x29, 40]
	ldr	x2, [x0, 3496]
	add	x0, x29, 48
	mov	x1, x0
	ldr	x0, [x29, 40]
	blr	x2
	mov	w0, 1
	b	.L1284
.L1278:
	str	wzr, [x29, 92]
	b	.L1280
.L1282:
	ldr	x0, [x29, 40]
	ldr	x5, [x0, 3480]
	ldr	x0, [x29, 40]
	ldr	x0, [x0, 512]
	ldr	w1, [x29, 92]
	uxth	w1, w1
	mov	w4, w1
	mov	x3, x0
	ldr	x2, [x29, 80]
	ldr	x1, [x29, 32]
	ldr	x0, [x29, 40]
	blr	x5
	str	w0, [x29, 76]
	ldr	w0, [x29, 76]
	cmp	w0, 0
	beq	.L1281
	adrp	x0, .LC278
	add	x0, x0, :lo12:.LC278
	bl	nand_print
.L1281:
	ldr	w0, [x29, 92]
	add	w0, w0, 1
	str	w0, [x29, 92]
.L1280:
	ldrh	w1, [x29, 22]
	ldr	w0, [x29, 92]
	cmp	w1, w0
	bhi	.L1282
	ldr	x0, [x29, 32]
	ldrh	w1, [x0]
	ldr	x0, [x29, 40]
	ldr	x0, [x0, 8]
	ldrh	w2, [x0, 28]
	add	x0, x29, 48
	mov	x5, 0
	mov	x4, 0
	mov	w3, w2
	mov	w2, 0
	bl	set_physic_op_par
	ldr	x0, [x29, 40]
	ldr	x2, [x0, 3496]
	add	x0, x29, 48
	mov	x1, x0
	ldr	x0, [x29, 40]
	blr	x2
	ldrh	w0, [x29, 22]
	cmp	w0, 0
	bne	.L1283
	ldr	x0, [x29, 40]
	ldr	x1, [x29, 24]
	str	x1, [x0, 528]
	ldr	x0, [x29, 40]
	strh	wzr, [x0, 584]
.L1283:
	mov	w0, 0
.L1284:
	ldp	x29, x30, [sp], 96
	ret
	.size	do_write_error_in_build_list, .-do_write_error_in_build_list
	.section	.rodata
	.align	3
.LC279:
	.string	"%s %d:fail 1\n"
	.align	3
.LC280:
	.string	"%s %d:fail 2\n"
	.align	3
.LC281:
	.string	"%s %d:fail 3\n"
	.align	3
.LC282:
	.string	"%s %d:fail 4\n"
	.align	3
.LC283:
	.string	"[NE]pages_per_blk: %d, %p, %p, %p, %p\n"
	.text
	.align	2
	.global	init_nrc
	.type	init_nrc, %function
init_nrc:
	stp	x29, x30, [sp, -48]!
	add	x29, sp, 0
	str	x0, [x29, 24]
	str	x1, [x29, 16]
	str	wzr, [x29, 44]
	str	xzr, [x29, 32]
	mov	w0, 32
	bl	nftl_malloc
	str	x0, [x29, 32]
	ldr	x0, [x29, 32]
	cmp	x0, 0
	bne	.L1286
	adrp	x0, __func__.7312
	add	x1, x0, :lo12:__func__.7312
	adrp	x0, .LC279
	add	x0, x0, :lo12:.LC279
	mov	w2, 1408
	bl	nand_print
	mov	w0, -1
	b	.L1287
.L1286:
	mov	w2, 32
	mov	w1, 0
	ldr	x0, [x29, 32]
	bl	nand_memset
	ldr	x0, [x29, 24]
	ldr	x0, [x0, 8]
	ldrh	w0, [x0, 20]
	str	w0, [x29, 44]
	ldr	x0, [x29, 24]
	ldr	x0, [x0, 8]
	ldrh	w0, [x0, 26]
	add	w1, w0, 64
	ldr	w0, [x29, 44]
	mul	w0, w1, w0
	bl	nftl_malloc
	mov	x1, x0
	ldr	x0, [x29, 32]
	str	x1, [x0, 16]
	ldr	x0, [x29, 32]
	ldr	x0, [x0, 16]
	cmp	x0, 0
	bne	.L1288
	adrp	x0, __func__.7312
	add	x1, x0, :lo12:__func__.7312
	adrp	x0, .LC280
	add	x0, x0, :lo12:.LC280
	mov	w2, 1417
	bl	nand_print
	b	.L1289
.L1288:
	ldrsw	x0, [x29, 44]
	lsl	w0, w0, 2
	bl	nftl_malloc
	mov	x1, x0
	ldr	x0, [x29, 32]
	str	x1, [x0, 24]
	ldr	x0, [x29, 32]
	ldr	x0, [x0, 24]
	cmp	x0, 0
	bne	.L1290
	adrp	x0, __func__.7312
	add	x1, x0, :lo12:__func__.7312
	adrp	x0, .LC281
	add	x0, x0, :lo12:.LC281
	mov	w2, 1422
	bl	nand_print
	b	.L1289
.L1290:
	ldr	x0, [x29, 32]
	ldr	x0, [x0, 24]
	ldrsw	x1, [x29, 44]
	lsl	w1, w1, 2
	mov	w2, w1
	mov	w1, -1
	bl	nand_memset
	ldr	w0, [x29, 44]
	bl	nftl_malloc
	mov	x1, x0
	ldr	x0, [x29, 32]
	str	x1, [x0, 8]
	ldr	x0, [x29, 32]
	ldr	x0, [x0, 8]
	cmp	x0, 0
	bne	.L1291
	adrp	x0, __func__.7312
	add	x1, x0, :lo12:__func__.7312
	adrp	x0, .LC282
	add	x0, x0, :lo12:.LC282
	mov	w2, 1428
	bl	nand_print
	b	.L1289
.L1291:
	ldr	x0, [x29, 32]
	ldr	x0, [x0, 8]
	ldr	w1, [x29, 44]
	mov	w2, w1
	mov	w1, 0
	bl	nand_memset
	ldr	x0, [x29, 16]
	ldr	x1, [x29, 32]
	str	x1, [x0]
	ldr	x0, [x29, 24]
	ldr	x0, [x0, 8]
	ldrh	w0, [x0, 20]
	mov	w6, w0
	ldr	x0, [x29, 32]
	ldr	x1, [x0, 16]
	ldr	x0, [x29, 32]
	ldr	x2, [x0, 24]
	ldr	x0, [x29, 32]
	ldr	x3, [x0, 8]
	adrp	x0, .LC283
	add	x0, x0, :lo12:.LC283
	mov	x5, x3
	mov	x4, x2
	mov	x3, x1
	ldr	x2, [x29, 32]
	mov	w1, w6
	bl	nand_print
	mov	w0, 0
	b	.L1287
.L1289:
	ldr	x0, [x29, 32]
	ldr	x0, [x0, 16]
	cmp	x0, 0
	beq	.L1292
	ldr	x0, [x29, 32]
	ldr	x0, [x0, 16]
	bl	nftl_free
.L1292:
	ldr	x0, [x29, 32]
	ldr	x0, [x0, 24]
	cmp	x0, 0
	beq	.L1293
	ldr	x0, [x29, 32]
	ldr	x0, [x0, 24]
	bl	nftl_free
.L1293:
	ldr	x0, [x29, 32]
	ldr	x0, [x0, 8]
	cmp	x0, 0
	beq	.L1294
	ldr	x0, [x29, 32]
	ldr	x0, [x0, 8]
	bl	nftl_free
.L1294:
	ldr	x0, [x29, 32]
	bl	nftl_free
	ldr	x0, [x29, 16]
	str	xzr, [x0]
	mov	w0, -1
.L1287:
	ldp	x29, x30, [sp], 48
	ret
	.size	init_nrc, .-init_nrc
	.align	2
	.global	free_nrc
	.type	free_nrc, %function
free_nrc:
	stp	x29, x30, [sp, -32]!
	add	x29, sp, 0
	str	x0, [x29, 24]
	ldr	x0, [x29, 24]
	cmp	x0, 0
	bne	.L1296
	mov	w0, 0
	b	.L1297
.L1296:
	ldr	x0, [x29, 24]
	ldr	x0, [x0, 16]
	bl	nftl_free
	ldr	x0, [x29, 24]
	ldr	x0, [x0, 8]
	bl	nftl_free
	ldr	x0, [x29, 24]
	ldr	x0, [x0, 24]
	bl	nftl_free
	ldr	x0, [x29, 24]
	bl	nftl_free
	mov	w0, 0
.L1297:
	ldp	x29, x30, [sp], 32
	ret
	.size	free_nrc, .-free_nrc
	.section	.rodata
	.align	3
.LC284:
	.string	"[NE]cross_talk_rebuild_current_block fail 1\n"
	.align	3
.LC285:
	.string	"[NE]cross_talk_rebuild_current_block!! %d,%d,%d!\n"
	.align	3
.LC286:
	.string	"[NE]cross_talk_rebuild_current_block fail 2\n"
	.align	3
.LC287:
	.string	"[NE]cross_talk_rebuild_current_block!! %d,%d,%d,%d,%d,%d!\n"
	.align	3
.LC288:
	.string	"[NE]cross_talk_rebuild_current_block fail 3\n"
	.align	3
.LC289:
	.string	"[NE]corss talk rebuild 1 %d %d %d!\n"
	.align	3
.LC290:
	.string	"[NE]corss talk rebuild 2 %d %d %d!\n"
	.align	3
.LC291:
	.string	"[NE]get free block fail!\n"
	.align	3
.LC292:
	.string	"[NE]something is error here 14!\n"
	.align	3
.LC293:
	.string	"[NE]page lost %d %d\n"
	.text
	.align	2
	.global	cross_talk_rebuild_current_block
	.type	cross_talk_rebuild_current_block, %function
cross_talk_rebuild_current_block:
	stp	x29, x30, [sp, -176]!
	add	x29, sp, 0
	str	x0, [x29, 40]
	str	x1, [x29, 32]
	str	x2, [x29, 24]
	str	wzr, [x29, 160]
	str	xzr, [x29, 56]
	str	xzr, [x29, 144]
	str	xzr, [x29, 136]
	add	x0, x29, 56
	mov	x1, x0
	ldr	x0, [x29, 40]
	bl	init_nrc
	cmp	w0, 0
	beq	.L1299
	ldr	x0, [x29, 56]
	bl	free_nrc
	adrp	x0, .LC284
	add	x0, x0, :lo12:.LC284
	bl	nand_print
	mov	x0, 0
	b	.L1317
.L1299:
	ldr	x1, [x29, 56]
	add	x0, x29, 112
	mov	x3, x1
	mov	x2, x0
	ldr	x1, [x29, 32]
	ldr	x0, [x29, 40]
	bl	get_used_page_num
	str	w0, [x29, 132]
	ldr	x0, [x29, 24]
	cmp	x0, 0
	bne	.L1301
	ldr	x0, [x29, 32]
	ldrh	w0, [x0]
	mov	w4, w0
	ldr	x0, [x29, 32]
	ldr	w1, [x0, 8]
	adrp	x0, .LC285
	add	x0, x0, :lo12:.LC285
	mov	w3, w1
	ldr	w2, [x29, 132]
	mov	w1, w4
	bl	nand_print
	ldr	x0, [x29, 32]
	str	x0, [x29, 152]
	ldr	x0, [x29, 32]
	ldrh	w0, [x0, 6]
	str	w0, [x29, 164]
	ldr	x0, [x29, 56]
	ldr	w0, [x0]
	cmp	w0, 0
	bne	.L1302
	ldr	x0, [x29, 56]
	bl	free_nrc
	adrp	x0, .LC286
	add	x0, x0, :lo12:.LC286
	bl	nand_print
	ldr	x0, [x29, 32]
	ldrh	w0, [x0, 6]
	add	w0, w0, 1
	uxth	w0, w0
	mov	w2, w0
	ldr	x1, [x29, 32]
	ldr	x0, [x29, 40]
	bl	erase_block
	mov	x0, 0
	b	.L1317
.L1302:
	ldr	w0, [x29, 112]
	cmp	w0, 0
	beq	.L1304
	ldr	w0, [x29, 164]
	add	w0, w0, 20
	str	w0, [x29, 164]
	b	.L1304
.L1301:
	ldr	x1, [x29, 56]
	add	x0, x29, 108
	mov	x3, x1
	mov	x2, x0
	ldr	x1, [x29, 24]
	ldr	x0, [x29, 40]
	bl	get_used_page_num
	str	w0, [x29, 128]
	ldr	x0, [x29, 32]
	ldrh	w0, [x0]
	mov	w7, w0
	ldr	x0, [x29, 32]
	ldr	w1, [x0, 8]
	ldr	x0, [x29, 24]
	ldrh	w0, [x0]
	mov	w3, w0
	ldr	x0, [x29, 24]
	ldr	w2, [x0, 8]
	adrp	x0, .LC287
	add	x0, x0, :lo12:.LC287
	mov	w6, w2
	ldr	w5, [x29, 128]
	mov	w4, w3
	mov	w3, w1
	ldr	w2, [x29, 132]
	mov	w1, w7
	bl	nand_print
	ldr	x0, [x29, 56]
	ldr	w0, [x0]
	cmp	w0, 0
	bne	.L1305
	ldr	x0, [x29, 56]
	bl	free_nrc
	adrp	x0, .LC288
	add	x0, x0, :lo12:.LC288
	bl	nand_print
	ldr	x0, [x29, 32]
	ldrh	w0, [x0, 6]
	add	w0, w0, 1
	uxth	w0, w0
	mov	w2, w0
	ldr	x1, [x29, 32]
	ldr	x0, [x29, 40]
	bl	erase_block
	ldr	x0, [x29, 24]
	ldrh	w0, [x0, 6]
	add	w0, w0, 1
	uxth	w0, w0
	mov	w2, w0
	ldr	x1, [x29, 24]
	ldr	x0, [x29, 40]
	bl	erase_block
	mov	x0, 0
	b	.L1317
.L1305:
	ldr	w1, [x29, 128]
	ldr	w0, [x29, 132]
	cmp	w1, w0
	bcc	.L1306
	ldr	x0, [x29, 32]
	ldrh	w0, [x0, 6]
	str	w0, [x29, 164]
	ldr	x0, [x29, 24]
	ldrh	w0, [x0, 6]
	str	w0, [x29, 160]
	b	.L1307
.L1306:
	ldr	x0, [x29, 24]
	ldrh	w0, [x0, 6]
	str	w0, [x29, 164]
	ldr	x0, [x29, 32]
	ldrh	w0, [x0, 6]
	str	w0, [x29, 160]
.L1307:
	ldr	w0, [x29, 112]
	cmp	w0, 0
	bne	.L1308
	ldr	w0, [x29, 108]
	cmp	w0, 0
	beq	.L1309
.L1308:
	ldr	w0, [x29, 164]
	add	w0, w0, 20
	str	w0, [x29, 164]
	ldr	w0, [x29, 160]
	add	w0, w0, 20
	str	w0, [x29, 160]
.L1309:
	ldr	w1, [x29, 128]
	ldr	w0, [x29, 132]
	cmp	w1, w0
	bls	.L1310
	ldr	x0, [x29, 56]
	ldr	w1, [x0]
	adrp	x0, .LC289
	add	x0, x0, :lo12:.LC289
	mov	w3, w1
	ldr	w2, [x29, 128]
	ldr	w1, [x29, 132]
	bl	nand_print
	ldr	w0, [x29, 164]
	uxth	w0, w0
	add	w0, w0, 1
	uxth	w0, w0
	mov	w2, w0
	ldr	x1, [x29, 32]
	ldr	x0, [x29, 40]
	bl	erase_block
	ldr	x0, [x29, 24]
	str	x0, [x29, 152]
	b	.L1304
.L1310:
	ldr	x0, [x29, 56]
	ldr	w1, [x0]
	adrp	x0, .LC290
	add	x0, x0, :lo12:.LC290
	mov	w3, w1
	ldr	w2, [x29, 128]
	ldr	w1, [x29, 132]
	bl	nand_print
	ldr	w0, [x29, 164]
	uxth	w0, w0
	add	w0, w0, 1
	uxth	w0, w0
	mov	w2, w0
	ldr	x1, [x29, 24]
	ldr	x0, [x29, 40]
	bl	erase_block
	ldr	x0, [x29, 32]
	str	x0, [x29, 152]
.L1304:
	ldr	x0, [x29, 40]
	bl	out_phy_block_from_free_list
	str	x0, [x29, 120]
	ldr	x0, [x29, 120]
	cmp	x0, 0
	bne	.L1311
	adrp	x0, .LC291
	add	x0, x0, :lo12:.LC291
	bl	nand_print
	mov	x0, 0
	b	.L1317
.L1311:
	ldr	x0, [x29, 152]
	ldr	w0, [x0, 8]
	add	w1, w0, 1
	ldr	x0, [x29, 120]
	str	w1, [x0, 8]
	ldr	x0, [x29, 120]
	ldr	w0, [x0, 8]
	mov	w2, w0
	ldr	x1, [x29, 120]
	ldr	x0, [x29, 40]
	bl	new_block_init_for_write
	cmp	w0, 0
	beq	.L1312
	adrp	x0, .LC292
	add	x0, x0, :lo12:.LC292
	bl	nand_print
	b	.L1304
.L1312:
	str	wzr, [x29, 172]
	str	wzr, [x29, 168]
	b	.L1313
.L1316:
	ldr	x0, [x29, 56]
	ldr	x1, [x0, 8]
	ldr	w0, [x29, 172]
	add	x0, x1, x0
	ldrb	w0, [x0]
	cmp	w0, 0
	beq	.L1314
	ldr	x0, [x29, 56]
	ldr	x1, [x0, 16]
	ldr	x0, [x29, 40]
	ldr	x0, [x0, 8]
	ldrh	w0, [x0, 26]
	add	w0, w0, 64
	mov	w2, w0
	ldr	w0, [x29, 172]
	mul	w0, w2, w0
	uxtw	x0, w0
	add	x0, x1, x0
	str	x0, [x29, 136]
	ldr	x0, [x29, 40]
	ldr	x0, [x0, 8]
	ldrh	w0, [x0, 26]
	uxth	x0, w0
	ldr	x1, [x29, 136]
	add	x0, x1, x0
	str	x0, [x29, 144]
	ldr	x0, [x29, 120]
	ldrh	w1, [x0]
	ldr	w0, [x29, 168]
	uxth	w2, w0
	ldr	x0, [x29, 40]
	ldr	x0, [x0, 8]
	ldrh	w3, [x0, 28]
	add	x0, x29, 64
	ldr	x5, [x29, 144]
	ldr	x4, [x29, 136]
	bl	set_physic_op_par
	ldr	x0, [x29, 144]
	bl	get_special_data_from_oob
	str	w0, [x29, 116]
	ldr	x0, [x29, 120]
	ldr	w0, [x0, 8]
	mov	w1, w0
	ldr	x0, [x29, 120]
	ldrh	w0, [x0, 6]
	mov	w4, w0
	mov	w3, w1
	ldr	w2, [x29, 116]
	ldr	x1, [x29, 144]
	ldr	x0, [x29, 40]
	bl	set_oob_special_page
	ldr	x0, [x29, 40]
	ldr	x2, [x0, 3472]
	add	x0, x29, 64
	mov	x1, x0
	ldr	x0, [x29, 40]
	blr	x2
	ldr	w0, [x29, 168]
	add	w0, w0, 1
	str	w0, [x29, 168]
	b	.L1315
.L1314:
	ldr	x0, [x29, 56]
	ldr	w1, [x0]
	adrp	x0, .LC293
	add	x0, x0, :lo12:.LC293
	mov	w2, w1
	ldr	w1, [x29, 172]
	bl	nand_print
.L1315:
	ldr	w0, [x29, 172]
	add	w0, w0, 1
	str	w0, [x29, 172]
.L1313:
	ldr	x0, [x29, 56]
	ldr	w1, [x0]
	ldr	w0, [x29, 172]
	cmp	w1, w0
	bhi	.L1316
	ldr	w0, [x29, 160]
	uxth	w0, w0
	add	w0, w0, 1
	uxth	w0, w0
	mov	w2, w0
	ldr	x1, [x29, 152]
	ldr	x0, [x29, 40]
	bl	erase_block
	ldr	x0, [x29, 56]
	bl	free_nrc
	ldr	x0, [x29, 120]
.L1317:
	ldp	x29, x30, [sp], 176
	ret
	.size	cross_talk_rebuild_current_block, .-cross_talk_rebuild_current_block
	.section	.rodata
	.align	3
.LC294:
	.string	"[NI]nctrcb1 %d,%d,%d!\n"
	.align	3
.LC295:
	.string	"[NE]no_cross_talk_rebuild_current_block fail 2\n"
	.align	3
.LC296:
	.string	"[NI]nctrcb2 %d,%d,%d,%d,%d,%d!\n"
	.align	3
.LC297:
	.string	"[NE]no_cross_talk_rebuild_current_block fail 4\n"
	.align	3
.LC298:
	.string	"[NE]no_cross_talk_rebuild_current_block erase block fail %d!\n"
	.text
	.align	2
	.global	no_cross_talk_rebuild_current_block
	.type	no_cross_talk_rebuild_current_block, %function
no_cross_talk_rebuild_current_block:
	stp	x29, x30, [sp, -192]!
	add	x29, sp, 0
	str	x0, [x29, 40]
	str	x1, [x29, 32]
	str	x2, [x29, 24]
	str	xzr, [x29, 152]
	str	xzr, [x29, 144]
	str	xzr, [x29, 64]
	str	xzr, [x29, 56]
	str	xzr, [x29, 160]
	add	x0, x29, 64
	mov	x1, x0
	ldr	x0, [x29, 40]
	bl	init_nrc
	cmp	w0, 0
	beq	.L1319
	ldr	x0, [x29, 64]
	bl	free_nrc
	adrp	x0, .LC284
	add	x0, x0, :lo12:.LC284
	bl	nand_print
	mov	x0, 0
	b	.L1337
.L1319:
	ldr	x1, [x29, 64]
	add	x0, x29, 124
	mov	x3, x1
	mov	x2, x0
	ldr	x1, [x29, 32]
	ldr	x0, [x29, 40]
	bl	get_used_page_num_no_crosstalk
	str	w0, [x29, 140]
	ldr	x0, [x29, 24]
	cmp	x0, 0
	bne	.L1321
	ldr	x0, [x29, 32]
	ldrh	w0, [x0]
	mov	w4, w0
	ldr	x0, [x29, 32]
	ldr	w1, [x0, 8]
	adrp	x0, .LC294
	add	x0, x0, :lo12:.LC294
	mov	w3, w1
	ldr	w2, [x29, 140]
	mov	w1, w4
	bl	nand_print
	ldr	x0, [x29, 32]
	str	x0, [x29, 176]
	ldr	x0, [x29, 64]
	ldr	w0, [x0]
	cmp	w0, 0
	bne	.L1322
	ldr	x0, [x29, 64]
	bl	free_nrc
	adrp	x0, .LC295
	add	x0, x0, :lo12:.LC295
	bl	nand_print
	ldr	x0, [x29, 32]
	ldrh	w0, [x0, 6]
	add	w0, w0, 1
	uxth	w0, w0
	mov	w2, w0
	ldr	x1, [x29, 32]
	ldr	x0, [x29, 40]
	bl	erase_block
	mov	x0, 0
	b	.L1337
.L1322:
	ldr	x0, [x29, 64]
	str	x0, [x29, 160]
.L1323:
	ldr	x0, [x29, 40]
	bl	out_phy_block_from_free_list
	str	x0, [x29, 168]
	ldr	x0, [x29, 168]
	cmp	x0, 0
	bne	.L1324
	adrp	x0, .LC291
	add	x0, x0, :lo12:.LC291
	bl	nand_print
	mov	x0, 0
	b	.L1337
.L1324:
	ldr	x0, [x29, 176]
	ldr	w1, [x0, 8]
	ldr	x0, [x29, 168]
	str	w1, [x0, 8]
	ldr	x0, [x29, 168]
	ldr	w0, [x0, 8]
	mov	w2, w0
	ldr	x1, [x29, 168]
	ldr	x0, [x29, 40]
	bl	new_block_init_for_write
	cmp	w0, 0
	beq	.L1325
	adrp	x0, .LC292
	add	x0, x0, :lo12:.LC292
	bl	nand_print
	b	.L1323
.L1321:
	add	x0, x29, 56
	mov	x1, x0
	ldr	x0, [x29, 40]
	bl	init_nrc
	cmp	w0, 0
	beq	.L1326
	ldr	x0, [x29, 64]
	bl	free_nrc
	ldr	x0, [x29, 56]
	bl	free_nrc
	adrp	x0, .LC288
	add	x0, x0, :lo12:.LC288
	bl	nand_print
	mov	x0, 0
	b	.L1337
.L1326:
	ldr	x1, [x29, 56]
	add	x0, x29, 120
	mov	x3, x1
	mov	x2, x0
	ldr	x1, [x29, 24]
	ldr	x0, [x29, 40]
	bl	get_used_page_num_no_crosstalk
	str	w0, [x29, 136]
	ldr	x0, [x29, 32]
	ldrh	w0, [x0]
	mov	w7, w0
	ldr	x0, [x29, 32]
	ldr	w1, [x0, 8]
	ldr	x0, [x29, 24]
	ldrh	w0, [x0]
	mov	w3, w0
	ldr	x0, [x29, 24]
	ldr	w2, [x0, 8]
	adrp	x0, .LC296
	add	x0, x0, :lo12:.LC296
	mov	w6, w2
	ldr	w5, [x29, 136]
	mov	w4, w3
	mov	w3, w1
	ldr	w2, [x29, 140]
	mov	w1, w7
	bl	nand_print
	ldr	x0, [x29, 56]
	ldr	w0, [x0]
	cmp	w0, 0
	bne	.L1327
	ldr	x0, [x29, 64]
	bl	free_nrc
	ldr	x0, [x29, 56]
	bl	free_nrc
	adrp	x0, .LC297
	add	x0, x0, :lo12:.LC297
	bl	nand_print
	ldr	x0, [x29, 32]
	ldrh	w0, [x0, 6]
	add	w0, w0, 1
	uxth	w0, w0
	mov	w2, w0
	ldr	x1, [x29, 32]
	ldr	x0, [x29, 40]
	bl	erase_block
	ldr	x0, [x29, 24]
	ldrh	w0, [x0, 6]
	add	w0, w0, 1
	uxth	w0, w0
	mov	w2, w0
	ldr	x1, [x29, 24]
	ldr	x0, [x29, 40]
	bl	erase_block
	mov	x0, 0
	b	.L1337
.L1327:
	ldr	w1, [x29, 136]
	ldr	w0, [x29, 140]
	cmp	w1, w0
	bls	.L1328
	ldr	x0, [x29, 24]
	str	x0, [x29, 176]
	ldr	x0, [x29, 32]
	str	x0, [x29, 168]
	ldr	x0, [x29, 56]
	str	x0, [x29, 160]
	ldr	x0, [x29, 160]
	ldr	w1, [x0]
	adrp	x0, .LC289
	add	x0, x0, :lo12:.LC289
	mov	w3, w1
	ldr	w2, [x29, 136]
	ldr	w1, [x29, 140]
	bl	nand_print
	b	.L1329
.L1328:
	ldr	x0, [x29, 32]
	str	x0, [x29, 176]
	ldr	x0, [x29, 24]
	str	x0, [x29, 168]
	ldr	x0, [x29, 64]
	str	x0, [x29, 160]
	ldr	x0, [x29, 160]
	ldr	w1, [x0]
	adrp	x0, .LC290
	add	x0, x0, :lo12:.LC290
	mov	w3, w1
	ldr	w2, [x29, 136]
	ldr	w1, [x29, 140]
	bl	nand_print
.L1329:
	ldr	x0, [x29, 168]
	ldrh	w1, [x0]
	ldr	x0, [x29, 40]
	ldr	x0, [x0, 8]
	ldrh	w2, [x0, 28]
	add	x0, x29, 96
	mov	x5, 0
	mov	x4, 0
	mov	w3, w2
	mov	w2, 0
	bl	set_physic_op_par
	ldr	x0, [x29, 40]
	ldr	x2, [x0, 3456]
	add	x0, x29, 96
	mov	x1, x0
	ldr	x0, [x29, 40]
	blr	x2
	str	w0, [x29, 132]
	ldr	w0, [x29, 132]
	cmp	w0, 0
	beq	.L1330
	ldr	x0, [x29, 168]
	ldrh	w0, [x0]
	mov	w1, w0
	adrp	x0, .LC298
	add	x0, x0, :lo12:.LC298
	bl	nand_print
	ldr	x0, [x29, 40]
	ldr	x2, [x0, 3496]
	add	x0, x29, 96
	mov	x1, x0
	ldr	x0, [x29, 40]
	blr	x2
	ldr	x0, [x29, 176]
	strh	wzr, [x0, 4]
	ldr	x0, [x29, 176]
	mov	w1, 255
	strh	w1, [x0, 2]
	ldr	x0, [x29, 40]
	ldrh	w0, [x0, 28]
	add	w0, w0, 1
	uxth	w1, w0
	ldr	x0, [x29, 40]
	strh	w1, [x0, 28]
.L1331:
	ldr	x0, [x29, 40]
	bl	out_phy_block_from_free_list
	str	x0, [x29, 168]
	ldr	x0, [x29, 168]
	cmp	x0, 0
	bne	.L1332
	adrp	x0, .LC291
	add	x0, x0, :lo12:.LC291
	bl	nand_print
	mov	x0, 0
	b	.L1337
.L1332:
	ldr	x0, [x29, 168]
	ldr	w0, [x0, 8]
	mov	w2, w0
	ldr	x1, [x29, 168]
	ldr	x0, [x29, 40]
	bl	new_block_init_for_write
	cmp	w0, 0
	beq	.L1330
	adrp	x0, .LC292
	add	x0, x0, :lo12:.LC292
	bl	nand_print
	b	.L1331
.L1330:
	ldr	x0, [x29, 176]
	ldr	w1, [x0, 8]
	ldr	x0, [x29, 168]
	str	w1, [x0, 8]
.L1325:
	str	wzr, [x29, 188]
	str	wzr, [x29, 184]
	b	.L1333
.L1336:
	ldr	x0, [x29, 160]
	ldr	x1, [x0, 8]
	ldr	w0, [x29, 188]
	add	x0, x1, x0
	ldrb	w0, [x0]
	cmp	w0, 0
	beq	.L1334
	ldr	x0, [x29, 160]
	ldr	x1, [x0, 16]
	ldr	x0, [x29, 40]
	ldr	x0, [x0, 8]
	ldrh	w0, [x0, 26]
	add	w0, w0, 64
	mov	w2, w0
	ldr	w0, [x29, 188]
	mul	w0, w2, w0
	uxtw	x0, w0
	add	x0, x1, x0
	str	x0, [x29, 144]
	ldr	x0, [x29, 40]
	ldr	x0, [x0, 8]
	ldrh	w0, [x0, 26]
	uxth	x0, w0
	ldr	x1, [x29, 144]
	add	x0, x1, x0
	str	x0, [x29, 152]
	ldr	x0, [x29, 168]
	ldrh	w1, [x0]
	ldr	w0, [x29, 184]
	uxth	w2, w0
	ldr	x0, [x29, 40]
	ldr	x0, [x0, 8]
	ldrh	w3, [x0, 28]
	add	x0, x29, 72
	ldr	x5, [x29, 152]
	ldr	x4, [x29, 144]
	bl	set_physic_op_par
	ldr	x0, [x29, 152]
	bl	get_special_data_from_oob
	str	w0, [x29, 128]
	ldr	x0, [x29, 168]
	ldr	w0, [x0, 8]
	mov	w1, w0
	ldr	x0, [x29, 168]
	ldrh	w0, [x0, 6]
	mov	w4, w0
	mov	w3, w1
	ldr	w2, [x29, 128]
	ldr	x1, [x29, 152]
	ldr	x0, [x29, 40]
	bl	set_oob_special_page
	ldr	x0, [x29, 40]
	ldr	x2, [x0, 3472]
	add	x0, x29, 72
	mov	x1, x0
	ldr	x0, [x29, 40]
	blr	x2
	ldr	w0, [x29, 184]
	add	w0, w0, 1
	str	w0, [x29, 184]
	b	.L1335
.L1334:
	ldr	x0, [x29, 160]
	ldr	w1, [x0]
	adrp	x0, .LC293
	add	x0, x0, :lo12:.LC293
	mov	w2, w1
	ldr	w1, [x29, 188]
	bl	nand_print
.L1335:
	ldr	w0, [x29, 188]
	add	w0, w0, 1
	str	w0, [x29, 188]
.L1333:
	ldr	x0, [x29, 160]
	ldr	w1, [x0]
	ldr	w0, [x29, 188]
	cmp	w1, w0
	bhi	.L1336
	ldr	x0, [x29, 176]
	ldrh	w0, [x0, 6]
	add	w0, w0, 1
	uxth	w0, w0
	mov	w2, w0
	ldr	x1, [x29, 176]
	ldr	x0, [x29, 40]
	bl	erase_block
	ldr	x0, [x29, 64]
	bl	free_nrc
	ldr	x0, [x29, 56]
	bl	free_nrc
	ldr	x0, [x29, 168]
.L1337:
	ldp	x29, x30, [sp], 192
	ret
	.size	no_cross_talk_rebuild_current_block, .-no_cross_talk_rebuild_current_block
	.section	.rodata
	.align	3
.LC299:
	.string	"[NI]erase_block block %d!\n"
	.align	3
.LC300:
	.string	"[NE]erase_block block fail %d!\n"
	.text
	.align	2
	.global	erase_block
	.type	erase_block, %function
erase_block:
	stp	x29, x30, [sp, -80]!
	add	x29, sp, 0
	str	x0, [x29, 40]
	str	x1, [x29, 32]
	strh	w2, [x29, 30]
	ldr	x0, [x29, 32]
	ldrh	w0, [x0]
	mov	w1, w0
	adrp	x0, .LC299
	add	x0, x0, :lo12:.LC299
	bl	nand_print
	ldr	x0, [x29, 32]
	ldrh	w1, [x0]
	ldr	x0, [x29, 40]
	ldr	x0, [x0, 8]
	ldrh	w2, [x0, 28]
	add	x0, x29, 48
	mov	x5, 0
	mov	x4, 0
	mov	w3, w2
	mov	w2, 0
	bl	set_physic_op_par
	ldr	x0, [x29, 40]
	ldr	x2, [x0, 3456]
	add	x0, x29, 48
	mov	x1, x0
	ldr	x0, [x29, 40]
	blr	x2
	str	w0, [x29, 76]
	ldr	w0, [x29, 76]
	cmp	w0, 0
	beq	.L1339
	ldr	x0, [x29, 32]
	ldrh	w0, [x0]
	mov	w1, w0
	adrp	x0, .LC300
	add	x0, x0, :lo12:.LC300
	bl	nand_print
	ldr	x0, [x29, 40]
	ldr	x2, [x0, 3496]
	add	x0, x29, 48
	mov	x1, x0
	ldr	x0, [x29, 40]
	blr	x2
	ldr	x0, [x29, 32]
	strh	wzr, [x0, 4]
	ldr	x0, [x29, 32]
	mov	w1, 255
	strh	w1, [x0, 2]
	ldr	x0, [x29, 40]
	ldrh	w0, [x0, 28]
	add	w0, w0, 1
	uxth	w1, w0
	ldr	x0, [x29, 40]
	strh	w1, [x0, 28]
	b	.L1340
.L1339:
	ldr	x0, [x29, 32]
	ldrh	w1, [x29, 30]
	strh	w1, [x0, 6]
	ldr	x0, [x29, 32]
	mov	w1, -1
	str	w1, [x0, 8]
	ldr	x1, [x29, 32]
	ldr	x0, [x29, 40]
	bl	put_phy_block_to_free_list
	ldr	x1, [x29, 32]
	ldr	x0, [x29, 40]
	bl	mark_free_block
	ldr	x0, [x29, 40]
	ldr	x0, [x0, 8]
	ldrh	w1, [x0, 20]
	ldr	x0, [x29, 32]
	strh	w1, [x0, 4]
	ldr	x0, [x29, 32]
	strh	wzr, [x0, 2]
.L1340:
	ldr	w0, [x29, 76]
	ldp	x29, x30, [sp], 80
	ret
	.size	erase_block, .-erase_block
	.align	2
	.global	nftl_check_pattern
	.type	nftl_check_pattern, %function
nftl_check_pattern:
	sub	sp, sp, #32
	str	x0, [sp, 8]
	strb	w1, [sp, 7]
	str	w2, [sp]
	str	wzr, [sp, 28]
	b	.L1343
.L1346:
	ldrsw	x0, [sp, 28]
	ldr	x1, [sp, 8]
	add	x0, x1, x0
	ldrb	w0, [x0]
	ldrb	w1, [sp, 7]
	cmp	w1, w0
	beq	.L1344
	mov	w0, 1
	b	.L1345
.L1344:
	ldr	w0, [sp, 28]
	add	w0, w0, 1
	str	w0, [sp, 28]
.L1343:
	ldr	w1, [sp, 28]
	ldr	w0, [sp]
	cmp	w1, w0
	blt	.L1346
	mov	w0, 0
.L1345:
	add	sp, sp, 32
	ret
	.size	nftl_check_pattern, .-nftl_check_pattern
	.data
	.align	3
	.type	patterns, %object
	.size	patterns, 1
patterns:
	.byte	-91
	.section	.rodata
	.align	3
.LC301:
	.string	"[NI]torture b@%d.\n"
	.align	3
.LC302:
	.string	"[NE]%s write b@%d p@%d err\n"
	.align	3
.LC303:
	.string	"[NE]%s read b@%d p@%d err\n"
	.align	3
.LC304:
	.string	"[NE]%s b@%d p@%d patt %02x checking main data fail\n"
	.align	3
.LC305:
	.string	"[NE]%s b@%d p@%d patt %02x checking spare data fail\n"
	.align	3
.LC306:
	.string	"[NE][NE]erase_block block fail %d!\n"
	.text
	.align	2
	.global	torture_block_and_mark
	.type	torture_block_and_mark, %function
torture_block_and_mark:
	stp	x29, x30, [sp, -96]!
	add	x29, sp, 0
	str	x0, [x29, 24]
	str	x1, [x29, 16]
	str	wzr, [x29, 92]
	str	wzr, [x29, 88]
	str	wzr, [x29, 80]
	str	wzr, [x29, 84]
	mov	w0, 3
	str	w0, [x29, 76]
	str	xzr, [x29, 56]
	str	wzr, [x29, 64]
	strh	wzr, [x29, 68]
	strb	wzr, [x29, 70]
	mov	w0, 1
	str	w0, [x29, 80]
	ldr	x0, [x29, 16]
	ldrh	w0, [x0]
	mov	w1, w0
	adrp	x0, .LC301
	add	x0, x0, :lo12:.LC301
	bl	nand_print
	ldr	x0, [x29, 16]
	ldrh	w1, [x0]
	ldr	x0, [x29, 24]
	ldr	x0, [x0, 8]
	ldrh	w2, [x0, 28]
	add	x0, x29, 32
	mov	x5, 0
	mov	x4, 0
	mov	w3, w2
	mov	w2, 0
	bl	set_physic_op_par
	ldr	x0, [x29, 24]
	ldr	x2, [x0, 3456]
	add	x0, x29, 32
	mov	x1, x0
	ldr	x0, [x29, 24]
	blr	x2
	str	w0, [x29, 92]
	ldr	w0, [x29, 92]
	cmp	w0, 0
	beq	.L1348
	ldr	x0, [x29, 16]
	ldrh	w0, [x0]
	mov	w1, w0
	adrp	x0, .LC300
	add	x0, x0, :lo12:.LC300
	bl	nand_print
	b	.L1349
.L1348:
	ldr	x0, [x29, 16]
	ldrh	w0, [x0, 6]
	add	w0, w0, 1
	uxth	w1, w0
	ldr	x0, [x29, 16]
	strh	w1, [x0, 6]
	str	wzr, [x29, 88]
	b	.L1350
.L1359:
	ldr	x0, [x29, 24]
	ldr	x3, [x0, 512]
	adrp	x0, patterns
	add	x1, x0, :lo12:patterns
	ldrsw	x0, [x29, 88]
	ldrb	w0, [x1, x0]
	mov	w1, w0
	ldr	x0, [x29, 24]
	ldr	x0, [x0, 8]
	ldrh	w0, [x0, 26]
	uxth	x0, w0
	mov	x2, x0
	mov	x0, x3
	bl	memset
	adrp	x0, patterns
	add	x1, x0, :lo12:patterns
	ldrsw	x0, [x29, 88]
	ldrb	w0, [x1, x0]
	mov	w1, w0
	add	x0, x29, 56
	mov	x2, 15
	bl	memset
	str	wzr, [x29, 84]
	b	.L1351
.L1353:
	ldr	x0, [x29, 16]
	ldrh	w1, [x0]
	ldr	w0, [x29, 84]
	uxth	w2, w0
	ldr	x0, [x29, 24]
	ldr	x0, [x0, 8]
	ldrh	w3, [x0, 28]
	ldr	x0, [x29, 24]
	ldr	x4, [x0, 512]
	add	x5, x29, 56
	add	x0, x29, 32
	bl	set_physic_op_par
	ldr	x0, [x29, 24]
	ldr	x2, [x0, 3472]
	add	x0, x29, 32
	mov	x1, x0
	ldr	x0, [x29, 24]
	blr	x2
	str	w0, [x29, 92]
	ldr	w0, [x29, 92]
	cmp	w0, 0
	beq	.L1352
	ldr	x0, [x29, 16]
	ldrh	w0, [x0]
	mov	w2, w0
	adrp	x0, __func__.7402
	add	x1, x0, :lo12:__func__.7402
	adrp	x0, .LC302
	add	x0, x0, :lo12:.LC302
	ldr	w3, [x29, 84]
	bl	nand_print
	b	.L1349
.L1352:
	ldr	w0, [x29, 84]
	add	w0, w0, 1
	str	w0, [x29, 84]
.L1351:
	ldr	w1, [x29, 84]
	ldr	w0, [x29, 76]
	cmp	w1, w0
	blt	.L1353
	str	wzr, [x29, 84]
	b	.L1354
.L1358:
	ldr	x0, [x29, 24]
	ldr	x3, [x0, 512]
	ldr	x0, [x29, 24]
	ldr	x0, [x0, 8]
	ldrh	w0, [x0, 26]
	uxth	x0, w0
	mov	x2, x0
	mov	w1, 0
	mov	x0, x3
	bl	memset
	add	x0, x29, 56
	mov	x2, 15
	mov	w1, 0
	bl	memset
	ldr	x0, [x29, 16]
	ldrh	w1, [x0]
	ldr	w0, [x29, 84]
	uxth	w2, w0
	ldr	x0, [x29, 24]
	ldr	x0, [x0, 8]
	ldrh	w3, [x0, 28]
	ldr	x0, [x29, 24]
	ldr	x4, [x0, 512]
	add	x5, x29, 56
	add	x0, x29, 32
	bl	set_physic_op_par
	ldr	x0, [x29, 24]
	ldr	x2, [x0, 3464]
	add	x0, x29, 32
	mov	x1, x0
	ldr	x0, [x29, 24]
	blr	x2
	str	w0, [x29, 92]
	ldr	w0, [x29, 92]
	cmp	w0, 0
	beq	.L1355
	ldr	x0, [x29, 16]
	ldrh	w0, [x0]
	mov	w2, w0
	adrp	x0, __func__.7402
	add	x1, x0, :lo12:__func__.7402
	adrp	x0, .LC303
	add	x0, x0, :lo12:.LC303
	ldr	w3, [x29, 84]
	bl	nand_print
	b	.L1349
.L1355:
	ldr	x0, [x29, 24]
	ldr	x3, [x0, 512]
	adrp	x0, patterns
	add	x1, x0, :lo12:patterns
	ldrsw	x0, [x29, 88]
	ldrb	w1, [x1, x0]
	ldr	x0, [x29, 24]
	ldr	x0, [x0, 8]
	ldrh	w0, [x0, 26]
	mov	w2, w0
	mov	x0, x3
	bl	nftl_check_pattern
	str	w0, [x29, 92]
	ldr	w0, [x29, 92]
	cmp	w0, 0
	beq	.L1356
	ldr	x0, [x29, 16]
	ldrh	w0, [x0]
	mov	w2, w0
	adrp	x0, patterns
	add	x1, x0, :lo12:patterns
	ldrsw	x0, [x29, 88]
	ldrb	w0, [x1, x0]
	mov	w3, w0
	adrp	x0, __func__.7402
	add	x1, x0, :lo12:__func__.7402
	adrp	x0, .LC304
	add	x0, x0, :lo12:.LC304
	mov	w4, w3
	ldr	w3, [x29, 84]
	bl	nand_print
	b	.L1349
.L1356:
	adrp	x0, patterns
	add	x1, x0, :lo12:patterns
	ldrsw	x0, [x29, 88]
	ldrb	w1, [x1, x0]
	add	x0, x29, 56
	mov	w2, 15
	bl	nftl_check_pattern
	str	w0, [x29, 92]
	ldr	w0, [x29, 92]
	cmp	w0, 0
	beq	.L1357
	ldr	x0, [x29, 16]
	ldrh	w0, [x0]
	mov	w2, w0
	adrp	x0, patterns
	add	x1, x0, :lo12:patterns
	ldrsw	x0, [x29, 88]
	ldrb	w0, [x1, x0]
	mov	w3, w0
	adrp	x0, __func__.7402
	add	x1, x0, :lo12:__func__.7402
	adrp	x0, .LC305
	add	x0, x0, :lo12:.LC305
	mov	w4, w3
	ldr	w3, [x29, 84]
	bl	nand_print
	b	.L1349
.L1357:
	ldr	w0, [x29, 84]
	add	w0, w0, 1
	str	w0, [x29, 84]
.L1354:
	ldr	w1, [x29, 84]
	ldr	w0, [x29, 76]
	cmp	w1, w0
	blt	.L1358
	ldr	w0, [x29, 88]
	add	w0, w0, 1
	str	w0, [x29, 88]
.L1350:
	ldr	w1, [x29, 88]
	ldr	w0, [x29, 80]
	cmp	w1, w0
	blt	.L1359
	ldr	x0, [x29, 16]
	ldrh	w1, [x0]
	ldr	x0, [x29, 24]
	ldr	x0, [x0, 8]
	ldrh	w2, [x0, 28]
	add	x0, x29, 32
	mov	x5, 0
	mov	x4, 0
	mov	w3, w2
	mov	w2, 0
	bl	set_physic_op_par
	ldr	x0, [x29, 24]
	ldr	x2, [x0, 3456]
	add	x0, x29, 32
	mov	x1, x0
	ldr	x0, [x29, 24]
	blr	x2
	str	w0, [x29, 92]
	ldr	w0, [x29, 92]
	cmp	w0, 0
	beq	.L1360
	ldr	x0, [x29, 16]
	ldrh	w0, [x0]
	mov	w1, w0
	adrp	x0, .LC306
	add	x0, x0, :lo12:.LC306
	bl	nand_print
	b	.L1349
.L1360:
	ldr	x0, [x29, 16]
	ldrh	w0, [x0, 6]
	add	w0, w0, 1
	uxth	w1, w0
	ldr	x0, [x29, 16]
	strh	w1, [x0, 6]
	ldr	x0, [x29, 16]
	mov	w1, -1
	str	w1, [x0, 8]
	ldr	x1, [x29, 16]
	ldr	x0, [x29, 24]
	bl	put_phy_block_to_free_list
	ldr	x1, [x29, 16]
	ldr	x0, [x29, 24]
	bl	mark_free_block
	ldr	x0, [x29, 24]
	ldr	x0, [x0, 8]
	ldrh	w1, [x0, 20]
	ldr	x0, [x29, 16]
	strh	w1, [x0, 4]
	ldr	x0, [x29, 16]
	strh	wzr, [x0, 2]
	mov	w0, 0
	b	.L1362
.L1349:
	ldr	x0, [x29, 24]
	ldr	x2, [x0, 3496]
	add	x0, x29, 32
	mov	x1, x0
	ldr	x0, [x29, 24]
	blr	x2
	ldr	x0, [x29, 16]
	strh	wzr, [x0, 4]
	ldr	x0, [x29, 16]
	mov	w1, 255
	strh	w1, [x0, 2]
	ldr	x0, [x29, 24]
	ldrh	w0, [x0, 28]
	add	w0, w0, 1
	uxth	w1, w0
	ldr	x0, [x29, 24]
	strh	w1, [x0, 28]
	ldr	w0, [x29, 92]
.L1362:
	ldp	x29, x30, [sp], 96
	ret
	.size	torture_block_and_mark, .-torture_block_and_mark
	.align	2
	.global	mark_free_block
	.type	mark_free_block, %function
mark_free_block:
	stp	x29, x30, [sp, -80]!
	add	x29, sp, 0
	str	x0, [x29, 24]
	str	x1, [x29, 16]
	add	x0, x29, 40
	mov	w2, 16
	mov	w1, -91
	bl	nand_memset
	ldr	x0, [x29, 16]
	ldrh	w1, [x0]
	ldr	x0, [x29, 24]
	ldr	x0, [x0, 8]
	ldrh	w2, [x0, 28]
	ldr	x0, [x29, 24]
	ldr	x3, [x0, 512]
	add	x4, x29, 40
	add	x0, x29, 56
	mov	x5, x4
	mov	x4, x3
	mov	w3, w2
	mov	w2, 0
	bl	set_physic_op_par
	ldr	x0, [x29, 16]
	ldr	w0, [x0, 8]
	mov	w2, w0
	ldr	x0, [x29, 16]
	ldrh	w1, [x0, 6]
	add	x0, x29, 40
	mov	w4, w1
	mov	w3, w2
	mov	w2, 1431655765
	mov	x1, x0
	ldr	x0, [x29, 24]
	bl	set_oob_special_page
	ldr	x0, [x29, 24]
	ldr	x2, [x0, 3472]
	add	x0, x29, 56
	mov	x1, x0
	ldr	x0, [x29, 24]
	blr	x2
	ldr	x0, [x29, 16]
	ldrh	w1, [x0]
	ldr	x0, [x29, 24]
	ldr	x0, [x0, 8]
	ldrh	w2, [x0, 28]
	ldr	x0, [x29, 24]
	ldr	x3, [x0, 512]
	add	x4, x29, 40
	add	x0, x29, 56
	mov	x5, x4
	mov	x4, x3
	mov	w3, w2
	mov	w2, 1
	bl	set_physic_op_par
	ldr	x0, [x29, 16]
	ldr	w0, [x0, 8]
	mov	w2, w0
	ldr	x0, [x29, 16]
	ldrh	w1, [x0, 6]
	add	x0, x29, 40
	mov	w4, w1
	mov	w3, w2
	mov	w2, 1431655765
	mov	x1, x0
	ldr	x0, [x29, 24]
	bl	set_oob_special_page
	ldr	x0, [x29, 24]
	ldr	x2, [x0, 3472]
	add	x0, x29, 56
	mov	x1, x0
	ldr	x0, [x29, 24]
	blr	x2
	ldr	x0, [x29, 16]
	ldrh	w1, [x0]
	ldr	x0, [x29, 24]
	ldr	x0, [x0, 8]
	ldrh	w0, [x0, 20]
	sub	w0, w0, #2
	uxth	w2, w0
	ldr	x0, [x29, 24]
	ldr	x0, [x0, 8]
	ldrh	w3, [x0, 28]
	ldr	x0, [x29, 24]
	ldr	x4, [x0, 512]
	add	x5, x29, 40
	add	x0, x29, 56
	bl	set_physic_op_par
	ldr	x0, [x29, 16]
	ldr	w0, [x0, 8]
	mov	w2, w0
	ldr	x0, [x29, 16]
	ldrh	w1, [x0, 6]
	add	x0, x29, 40
	mov	w4, w1
	mov	w3, w2
	mov	w2, 1431655765
	mov	x1, x0
	ldr	x0, [x29, 24]
	bl	set_oob_special_page
	ldr	x0, [x29, 24]
	ldr	x2, [x0, 3472]
	add	x0, x29, 56
	mov	x1, x0
	ldr	x0, [x29, 24]
	blr	x2
	ldr	x0, [x29, 16]
	ldrh	w1, [x0]
	ldr	x0, [x29, 24]
	ldr	x0, [x0, 8]
	ldrh	w0, [x0, 20]
	sub	w0, w0, #1
	uxth	w2, w0
	ldr	x0, [x29, 24]
	ldr	x0, [x0, 8]
	ldrh	w3, [x0, 28]
	ldr	x0, [x29, 24]
	ldr	x4, [x0, 512]
	add	x5, x29, 40
	add	x0, x29, 56
	bl	set_physic_op_par
	ldr	x0, [x29, 16]
	ldr	w0, [x0, 8]
	mov	w2, w0
	ldr	x0, [x29, 16]
	ldrh	w1, [x0, 6]
	add	x0, x29, 40
	mov	w4, w1
	mov	w3, w2
	mov	w2, 1431655765
	mov	x1, x0
	ldr	x0, [x29, 24]
	bl	set_oob_special_page
	ldr	x0, [x29, 24]
	ldr	x2, [x0, 3472]
	add	x0, x29, 56
	mov	x1, x0
	ldr	x0, [x29, 24]
	blr	x2
	mov	w0, 0
	ldp	x29, x30, [sp], 80
	ret
	.size	mark_free_block, .-mark_free_block
	.align	2
	.global	is_last_phy_block
	.type	is_last_phy_block, %function
is_last_phy_block:
	sub	sp, sp, #16
	str	x0, [sp, 8]
	str	x1, [sp]
	ldr	x0, [sp, 8]
	ldrh	w0, [x0]
	mov	w1, w0
	ldr	x0, [sp]
	ldr	x0, [x0, 8]
	ldrh	w0, [x0, 24]
	sub	w0, w0, #1
	cmp	w1, w0
	bne	.L1366
	mov	w0, 1
	b	.L1367
.L1366:
	mov	w0, 0
.L1367:
	add	sp, sp, 16
	ret
	.size	is_last_phy_block, .-is_last_phy_block
	.align	2
	.global	is_phy_block_valid
	.type	is_phy_block_valid, %function
is_phy_block_valid:
	sub	sp, sp, #16
	str	x0, [sp, 8]
	str	x1, [sp]
	ldr	x0, [sp, 8]
	ldrh	w1, [x0]
	ldr	x0, [sp]
	ldr	x0, [x0, 8]
	ldrh	w0, [x0, 24]
	cmp	w1, w0
	bcs	.L1369
	mov	w0, 1
	b	.L1370
.L1369:
	mov	w0, 0
.L1370:
	add	sp, sp, 16
	ret
	.size	is_phy_block_valid, .-is_phy_block_valid
	.align	2
	.global	get_phy_block_addr
	.type	get_phy_block_addr, %function
get_phy_block_addr:
	sub	sp, sp, #16
	str	x0, [sp, 8]
	strh	w1, [sp, 6]
	ldr	x0, [sp, 8]
	ldr	x0, [x0, 8]
	ldr	x1, [x0, 32]
	ldrh	w0, [sp, 6]
	lsl	x0, x0, 6
	add	x0, x1, x0
	add	sp, sp, 16
	ret
	.size	get_phy_block_addr, .-get_phy_block_addr
	.section	.rodata
	.align	3
.LC307:
	.string	"[NE]get_used_page_num ecc error block:%d page:%d!\n"
	.align	3
.LC308:
	.string	"[NE]power off ecc error 4!!\n"
	.align	3
.LC309:
	.string	"[NE]sb@%d sp@%d get_used_page_num used spare data %x %x %x %x %x %x %x %x %x %x %x %x %x %x %x %x\n"
	.text
	.align	2
	.global	get_used_page_num
	.type	get_used_page_num, %function
get_used_page_num:
	sub	sp, sp, #224
	stp	x29, x30, [sp, 96]
	add	x29, sp, 96
	str	x0, [x29, 40]
	str	x1, [x29, 32]
	str	x2, [x29, 24]
	str	x3, [x29, 16]
	ldr	x0, [x29, 24]
	str	wzr, [x0]
	str	wzr, [x29, 120]
	str	wzr, [x29, 124]
	b	.L1374
.L1386:
	ldr	x0, [x29, 16]
	cmp	x0, 0
	beq	.L1375
	ldr	x0, [x29, 16]
	ldr	x1, [x0, 8]
	ldr	w0, [x29, 124]
	add	x0, x1, x0
	ldrb	w0, [x0]
	cmp	w0, 0
	bne	.L1375
	ldr	x0, [x29, 16]
	ldr	x1, [x0, 16]
	ldr	x0, [x29, 40]
	ldr	x0, [x0, 8]
	ldrh	w0, [x0, 26]
	add	w0, w0, 64
	mov	w2, w0
	ldr	w0, [x29, 124]
	mul	w0, w2, w0
	uxtw	x0, w0
	add	x0, x1, x0
	str	x0, [x29, 112]
	ldr	x0, [x29, 40]
	ldr	x0, [x0, 8]
	ldrh	w0, [x0, 26]
	uxth	x0, w0
	ldr	x1, [x29, 112]
	add	x0, x1, x0
	str	x0, [x29, 104]
	b	.L1376
.L1375:
	str	xzr, [x29, 112]
	ldr	x0, [x29, 40]
	ldr	x0, [x0, 8]
	ldrb	w0, [x0, 8]
	cmp	w0, 8
	bne	.L1377
	ldr	x0, [x29, 40]
	ldr	x0, [x0, 512]
	str	x0, [x29, 112]
.L1377:
	add	x0, x29, 80
	str	x0, [x29, 104]
.L1376:
	ldr	x0, [x29, 40]
	ldr	x0, [x0, 3448]
	ldrh	w0, [x0, 28]
	cmp	w0, 0
	beq	.L1378
	ldr	x0, [x29, 32]
	ldrh	w1, [x0]
	ldr	w0, [x29, 124]
	uxth	w2, w0
	ldr	x0, [x29, 40]
	ldr	x0, [x0, 8]
	ldrh	w3, [x0, 28]
	add	x0, x29, 56
	ldr	x5, [x29, 104]
	ldr	x4, [x29, 112]
	bl	set_physic_op_par
	b	.L1379
.L1378:
	ldr	x0, [x29, 32]
	ldrh	w1, [x0]
	ldr	w0, [x29, 124]
	uxth	w2, w0
	add	x0, x29, 56
	ldr	x5, [x29, 104]
	mov	x4, 0
	mov	w3, 0
	bl	set_physic_op_par
.L1379:
	ldr	x0, [x29, 40]
	ldr	x2, [x0, 3464]
	add	x0, x29, 56
	mov	x1, x0
	ldr	x0, [x29, 40]
	blr	x2
	str	w0, [x29, 100]
	ldr	w0, [x29, 100]
	cmp	w0, 0
	beq	.L1380
	ldr	w0, [x29, 100]
	cmp	w0, 10
	beq	.L1380
	ldr	x0, [x29, 32]
	ldrh	w0, [x0]
	mov	w1, w0
	adrp	x0, .LC307
	add	x0, x0, :lo12:.LC307
	ldr	w2, [x29, 124]
	bl	nand_print
	ldr	x0, [x29, 24]
	ldr	w0, [x0]
	add	w1, w0, 1
	ldr	x0, [x29, 24]
	str	w1, [x0]
.L1380:
	ldr	x0, [x29, 104]
	bl	is_nouse_page
	cmp	w0, 1
	beq	.L1397
	ldr	w0, [x29, 100]
	cmp	w0, 0
	beq	.L1383
	ldr	w0, [x29, 100]
	cmp	w0, 10
	bne	.L1384
.L1383:
	ldr	x0, [x29, 112]
	cmp	x0, 0
	beq	.L1384
	ldr	x2, [x29, 104]
	ldr	x1, [x29, 112]
	ldr	x0, [x29, 40]
	bl	nand_power_off_ecc_error
	cmp	w0, 0
	beq	.L1385
	adrp	x0, .LC308
	add	x0, x0, :lo12:.LC308
	bl	nand_print
	b	.L1384
.L1385:
	ldr	x0, [x29, 16]
	ldr	x1, [x0, 8]
	ldr	w0, [x29, 124]
	add	x0, x1, x0
	mov	w1, 1
	strb	w1, [x0]
	ldr	x0, [x29, 16]
	ldr	w1, [x0]
	ldr	w0, [x29, 124]
	add	w0, w0, 1
	cmp	w1, w0
	bcs	.L1384
	ldr	w0, [x29, 124]
	add	w1, w0, 1
	ldr	x0, [x29, 16]
	str	w1, [x0]
.L1384:
	ldr	w0, [x29, 124]
	add	w0, w0, 1
	str	w0, [x29, 124]
.L1374:
	ldr	x0, [x29, 40]
	ldr	x0, [x0, 8]
	ldrh	w0, [x0, 20]
	mov	w1, w0
	ldr	w0, [x29, 124]
	cmp	w1, w0
	bhi	.L1386
	b	.L1382
.L1397:
	nop
.L1382:
	ldr	w0, [x29, 124]
	str	w0, [x29, 96]
	b	.L1387
.L1395:
	ldr	x0, [x29, 16]
	cmp	x0, 0
	beq	.L1388
	ldr	x0, [x29, 16]
	ldr	x1, [x0, 8]
	ldr	w0, [x29, 124]
	add	x0, x1, x0
	ldrb	w0, [x0]
	cmp	w0, 0
	bne	.L1388
	ldr	x0, [x29, 16]
	ldr	x1, [x0, 16]
	ldr	x0, [x29, 40]
	ldr	x0, [x0, 8]
	ldrh	w0, [x0, 26]
	add	w0, w0, 64
	mov	w2, w0
	ldr	w0, [x29, 124]
	mul	w0, w2, w0
	uxtw	x0, w0
	add	x0, x1, x0
	str	x0, [x29, 112]
	ldr	x0, [x29, 40]
	ldr	x0, [x0, 8]
	ldrh	w0, [x0, 26]
	uxth	x0, w0
	ldr	x1, [x29, 112]
	add	x0, x1, x0
	str	x0, [x29, 104]
	b	.L1389
.L1388:
	str	xzr, [x29, 112]
	add	x0, x29, 80
	str	x0, [x29, 104]
.L1389:
	ldr	x0, [x29, 40]
	ldr	x0, [x0, 3448]
	ldrh	w0, [x0, 28]
	cmp	w0, 0
	beq	.L1390
	ldr	x0, [x29, 32]
	ldrh	w1, [x0]
	ldr	w0, [x29, 124]
	uxth	w2, w0
	ldr	x0, [x29, 40]
	ldr	x0, [x0, 8]
	ldrh	w3, [x0, 28]
	add	x0, x29, 56
	ldr	x5, [x29, 104]
	ldr	x4, [x29, 112]
	bl	set_physic_op_par
	b	.L1391
.L1390:
	ldr	x0, [x29, 32]
	ldrh	w1, [x0]
	ldr	w0, [x29, 124]
	uxth	w2, w0
	add	x0, x29, 56
	ldr	x5, [x29, 104]
	mov	x4, 0
	mov	w3, 0
	bl	set_physic_op_par
.L1391:
	ldr	x0, [x29, 40]
	ldr	x2, [x0, 3464]
	add	x0, x29, 56
	mov	x1, x0
	ldr	x0, [x29, 40]
	blr	x2
	str	w0, [x29, 100]
	ldr	x0, [x29, 104]
	bl	is_nouse_page
	cmp	w0, 1
	bne	.L1392
	ldr	w0, [x29, 120]
	add	w0, w0, 1
	str	w0, [x29, 120]
	b	.L1393
.L1392:
	ldr	x0, [x29, 32]
	ldrh	w0, [x0]
	mov	w12, w0
	ldr	x0, [x29, 104]
	ldrb	w0, [x0]
	mov	w13, w0
	ldr	x0, [x29, 104]
	add	x0, x0, 1
	ldrb	w0, [x0]
	mov	w14, w0
	ldr	x0, [x29, 104]
	add	x0, x0, 2
	ldrb	w0, [x0]
	mov	w15, w0
	ldr	x0, [x29, 104]
	add	x0, x0, 3
	ldrb	w0, [x0]
	mov	w16, w0
	ldr	x0, [x29, 104]
	add	x0, x0, 4
	ldrb	w0, [x0]
	mov	w17, w0
	ldr	x0, [x29, 104]
	add	x0, x0, 5
	ldrb	w0, [x0]
	mov	w1, w0
	ldr	x0, [x29, 104]
	add	x0, x0, 6
	ldrb	w0, [x0]
	mov	w2, w0
	ldr	x0, [x29, 104]
	add	x0, x0, 7
	ldrb	w0, [x0]
	mov	w3, w0
	ldr	x0, [x29, 104]
	add	x0, x0, 8
	ldrb	w0, [x0]
	mov	w4, w0
	ldr	x0, [x29, 104]
	add	x0, x0, 9
	ldrb	w0, [x0]
	mov	w5, w0
	ldr	x0, [x29, 104]
	add	x0, x0, 10
	ldrb	w0, [x0]
	mov	w6, w0
	ldr	x0, [x29, 104]
	add	x0, x0, 11
	ldrb	w0, [x0]
	mov	w7, w0
	ldr	x0, [x29, 104]
	add	x0, x0, 12
	ldrb	w0, [x0]
	mov	w8, w0
	ldr	x0, [x29, 104]
	add	x0, x0, 13
	ldrb	w0, [x0]
	mov	w9, w0
	ldr	x0, [x29, 104]
	add	x0, x0, 14
	ldrb	w0, [x0]
	mov	w10, w0
	ldr	x0, [x29, 104]
	add	x0, x0, 15
	ldrb	w0, [x0]
	mov	w11, w0
	adrp	x0, .LC309
	add	x0, x0, :lo12:.LC309
	str	w11, [sp, 80]
	str	w10, [sp, 72]
	str	w9, [sp, 64]
	str	w8, [sp, 56]
	str	w7, [sp, 48]
	str	w6, [sp, 40]
	str	w5, [sp, 32]
	str	w4, [sp, 24]
	str	w3, [sp, 16]
	str	w2, [sp, 8]
	str	w1, [sp]
	mov	w7, w17
	mov	w6, w16
	mov	w5, w15
	mov	w4, w14
	mov	w3, w13
	ldr	w2, [x29, 124]
	mov	w1, w12
	bl	nand_print
	ldr	w0, [x29, 100]
	cmp	w0, 0
	beq	.L1394
	ldr	w0, [x29, 100]
	cmp	w0, 10
	bne	.L1393
.L1394:
	ldr	x0, [x29, 112]
	cmp	x0, 0
	beq	.L1393
	ldr	x0, [x29, 16]
	ldr	x1, [x0, 8]
	ldr	w0, [x29, 124]
	add	x0, x1, x0
	mov	w1, 1
	strb	w1, [x0]
	ldr	x0, [x29, 16]
	ldr	w1, [x0]
	ldr	w0, [x29, 124]
	add	w0, w0, 1
	cmp	w1, w0
	bcs	.L1393
	ldr	w0, [x29, 124]
	add	w1, w0, 1
	ldr	x0, [x29, 16]
	str	w1, [x0]
.L1393:
	ldr	w0, [x29, 124]
	add	w0, w0, 1
	str	w0, [x29, 124]
.L1387:
	ldr	x0, [x29, 40]
	ldr	x0, [x0, 8]
	ldrh	w0, [x0, 20]
	mov	w1, w0
	ldr	w0, [x29, 124]
	cmp	w1, w0
	bhi	.L1395
	ldr	w0, [x29, 96]
	add	sp, x29, 0
	ldp	x29, x30, [sp], 128
	ret
	.size	get_used_page_num, .-get_used_page_num
	.section	.rodata
	.align	3
.LC310:
	.string	"[NE]gupnnc ecc err b:%d p:%d!\n"
	.align	3
.LC311:
	.string	"[NE]gupnnc power off ecc err!!\n"
	.align	3
.LC312:
	.string	"[NE]cross talk block:%d total_good_num %d good_page_num: %d!\n"
	.text
	.align	2
	.global	get_used_page_num_no_crosstalk
	.type	get_used_page_num_no_crosstalk, %function
get_used_page_num_no_crosstalk:
	stp	x29, x30, [sp, -160]!
	add	x29, sp, 0
	str	x19, [sp, 16]
	str	x0, [x29, 56]
	str	x1, [x29, 48]
	str	x2, [x29, 40]
	str	x3, [x29, 32]
	mov	w0, -1
	str	w0, [x29, 132]
	str	wzr, [x29, 128]
	str	wzr, [x29, 124]
	ldr	x0, [x29, 40]
	str	wzr, [x0]
	str	wzr, [x29, 156]
	b	.L1399
.L1410:
	ldr	x0, [x29, 32]
	cmp	x0, 0
	beq	.L1400
	ldr	x0, [x29, 32]
	ldr	x1, [x0, 8]
	ldr	w0, [x29, 156]
	add	x0, x1, x0
	ldrb	w0, [x0]
	cmp	w0, 0
	bne	.L1400
	ldr	x0, [x29, 32]
	ldr	x1, [x0, 16]
	ldr	x0, [x29, 56]
	ldr	x0, [x0, 8]
	ldrh	w0, [x0, 26]
	add	w0, w0, 64
	mov	w2, w0
	ldr	w0, [x29, 156]
	mul	w0, w2, w0
	uxtw	x0, w0
	add	x0, x1, x0
	str	x0, [x29, 144]
	ldr	x0, [x29, 56]
	ldr	x0, [x0, 8]
	ldrh	w0, [x0, 26]
	uxth	x0, w0
	ldr	x1, [x29, 144]
	add	x0, x1, x0
	str	x0, [x29, 136]
	b	.L1401
.L1400:
	str	xzr, [x29, 144]
	ldr	x0, [x29, 56]
	ldr	x0, [x0, 8]
	ldrb	w0, [x0, 8]
	cmp	w0, 4
	bne	.L1402
	ldr	x0, [x29, 56]
	ldr	x0, [x0, 512]
	str	x0, [x29, 144]
.L1402:
	add	x0, x29, 96
	str	x0, [x29, 136]
.L1401:
	ldr	x0, [x29, 48]
	ldrh	w1, [x0]
	ldr	w0, [x29, 156]
	uxth	w2, w0
	ldr	x0, [x29, 56]
	ldr	x0, [x0, 8]
	ldrh	w3, [x0, 28]
	add	x0, x29, 72
	ldr	x5, [x29, 136]
	ldr	x4, [x29, 144]
	bl	set_physic_op_par
	ldr	x0, [x29, 56]
	ldr	x2, [x0, 3464]
	add	x0, x29, 72
	mov	x1, x0
	ldr	x0, [x29, 56]
	blr	x2
	str	w0, [x29, 120]
	ldr	w0, [x29, 120]
	cmp	w0, 0
	beq	.L1403
	ldr	w0, [x29, 120]
	cmp	w0, 10
	beq	.L1403
	ldr	x0, [x29, 48]
	ldrh	w0, [x0]
	mov	w1, w0
	adrp	x0, .LC310
	add	x0, x0, :lo12:.LC310
	ldr	w2, [x29, 156]
	bl	nand_print
	ldr	w0, [x29, 132]
	cmn	w0, #1
	bne	.L1405
	ldr	w0, [x29, 156]
	str	w0, [x29, 132]
	b	.L1405
.L1403:
	ldr	x0, [x29, 136]
	bl	is_nouse_page
	cmp	w0, 1
	bne	.L1406
	ldr	w0, [x29, 132]
	cmn	w0, #1
	bne	.L1407
	ldr	w0, [x29, 156]
	str	w0, [x29, 132]
.L1407:
	ldr	w0, [x29, 124]
	add	w0, w0, 1
	str	w0, [x29, 124]
	ldr	w0, [x29, 124]
	cmp	w0, 3
	bne	.L1405
	b	.L1408
.L1406:
	ldr	x2, [x29, 136]
	ldr	x1, [x29, 144]
	ldr	x0, [x29, 56]
	bl	nand_power_off_ecc_error
	cmp	w0, 0
	beq	.L1409
	adrp	x0, .LC311
	add	x0, x0, :lo12:.LC311
	bl	nand_print
	b	.L1405
.L1409:
	ldr	w0, [x29, 128]
	add	w0, w0, 1
	str	w0, [x29, 128]
	ldr	w0, [x29, 132]
	cmn	w0, #1
	bne	.L1405
	ldr	x0, [x29, 32]
	ldr	x1, [x0, 8]
	ldr	w0, [x29, 156]
	add	x0, x1, x0
	mov	w1, 1
	strb	w1, [x0]
	ldr	x0, [x29, 32]
	ldr	x1, [x0, 24]
	ldr	w0, [x29, 156]
	lsl	x0, x0, 2
	add	x19, x1, x0
	ldr	x0, [x29, 136]
	bl	get_logic_page_from_oob
	str	w0, [x19]
	ldr	x0, [x29, 32]
	ldr	w1, [x0]
	ldr	w0, [x29, 156]
	add	w0, w0, 1
	cmp	w1, w0
	bcs	.L1405
	ldr	w0, [x29, 156]
	add	w1, w0, 1
	ldr	x0, [x29, 32]
	str	w1, [x0]
.L1405:
	ldr	w0, [x29, 156]
	add	w0, w0, 1
	str	w0, [x29, 156]
.L1399:
	ldr	x0, [x29, 56]
	ldr	x0, [x0, 8]
	ldrh	w0, [x0, 20]
	mov	w1, w0
	ldr	w0, [x29, 156]
	cmp	w1, w0
	bhi	.L1410
.L1408:
	ldr	w0, [x29, 132]
	cmn	w0, #1
	bne	.L1411
	ldr	x0, [x29, 56]
	ldr	x0, [x0, 8]
	ldrh	w0, [x0, 20]
	str	w0, [x29, 132]
	b	.L1412
.L1411:
	ldr	w1, [x29, 128]
	ldr	w0, [x29, 132]
	cmp	w1, w0
	beq	.L1412
	ldr	x0, [x29, 48]
	ldrh	w0, [x0]
	mov	w1, w0
	adrp	x0, .LC312
	add	x0, x0, :lo12:.LC312
	ldr	w3, [x29, 132]
	ldr	w2, [x29, 128]
	bl	nand_print
	ldr	x0, [x29, 56]
	ldr	x0, [x0, 3440]
	ldr	w1, [x0, 168]
	add	w1, w1, 1
	str	w1, [x0, 168]
	str	wzr, [x29, 156]
	b	.L1413
.L1415:
	ldr	x0, [x29, 32]
	ldr	x1, [x0, 8]
	ldr	w0, [x29, 156]
	add	x0, x1, x0
	ldrb	w0, [x0]
	cmp	w0, 0
	beq	.L1414
	ldr	x0, [x29, 32]
	ldr	x1, [x0, 16]
	ldr	x0, [x29, 56]
	ldr	x0, [x0, 8]
	ldrh	w0, [x0, 26]
	add	w0, w0, 64
	mov	w2, w0
	ldr	w0, [x29, 156]
	mul	w0, w2, w0
	uxtw	x2, w0
	ldr	x0, [x29, 56]
	ldr	x0, [x0, 8]
	ldrh	w0, [x0, 26]
	uxth	x0, w0
	add	x0, x2, x0
	add	x0, x1, x0
	str	x0, [x29, 136]
	ldr	x0, [x29, 136]
	add	x0, x0, 14
	ldr	x1, [x29, 56]
	ldr	x1, [x1, 3440]
	ldr	w1, [x1, 168]
	uxtb	w1, w1
	strb	w1, [x0]
.L1414:
	ldr	w0, [x29, 156]
	add	w0, w0, 1
	str	w0, [x29, 156]
.L1413:
	ldr	x0, [x29, 32]
	ldr	w1, [x0]
	ldr	w0, [x29, 156]
	cmp	w1, w0
	bhi	.L1415
.L1412:
	ldr	w0, [x29, 132]
	str	w0, [x29, 116]
	ldr	w0, [x29, 116]
	ldr	x19, [sp, 16]
	ldp	x29, x30, [sp], 160
	ret
	.size	get_used_page_num_no_crosstalk, .-get_used_page_num_no_crosstalk
	.align	2
	.global	check_cross_talk
	.type	check_cross_talk, %function
check_cross_talk:
	stp	x29, x30, [sp, -112]!
	add	x29, sp, 0
	str	x0, [x29, 24]
	str	x1, [x29, 16]
	ldr	x0, [x29, 24]
	ldr	x0, [x0, 512]
	str	x0, [x29, 96]
	str	wzr, [x29, 108]
	b	.L1418
.L1421:
	str	xzr, [x29, 96]
	add	x0, x29, 64
	str	x0, [x29, 88]
	ldr	x0, [x29, 16]
	ldrh	w1, [x0]
	ldr	w0, [x29, 108]
	uxth	w2, w0
	ldr	x0, [x29, 24]
	ldr	x0, [x0, 8]
	ldrh	w3, [x0, 28]
	add	x0, x29, 40
	ldr	x5, [x29, 88]
	ldr	x4, [x29, 96]
	bl	set_physic_op_par
	ldr	x0, [x29, 24]
	ldr	x2, [x0, 3464]
	add	x0, x29, 40
	mov	x1, x0
	ldr	x0, [x29, 24]
	blr	x2
	str	w0, [x29, 84]
	ldr	w0, [x29, 84]
	cmp	w0, 0
	bne	.L1419
	ldr	x0, [x29, 88]
	add	x0, x0, 14
	ldrb	w0, [x0]
	cmp	w0, 99
	bhi	.L1419
	ldr	x0, [x29, 24]
	ldr	x0, [x0, 3440]
	ldr	w0, [x0, 168]
	ldr	x1, [x29, 88]
	add	x1, x1, 14
	ldrb	w1, [x1]
	cmp	w0, w1
	bhi	.L1419
	ldr	x0, [x29, 24]
	ldr	x0, [x0, 3440]
	ldr	x1, [x29, 88]
	add	x1, x1, 14
	ldrb	w1, [x1]
	str	w1, [x0, 168]
	b	.L1420
.L1419:
	ldr	w0, [x29, 108]
	add	w0, w0, 1
	str	w0, [x29, 108]
.L1418:
	ldr	x0, [x29, 24]
	ldr	x0, [x0, 8]
	ldrh	w0, [x0, 20]
	mov	w1, w0
	ldr	w0, [x29, 108]
	cmp	w1, w0
	bhi	.L1421
.L1420:
	mov	w0, 0
	ldp	x29, x30, [sp], 112
	ret
	.size	check_cross_talk, .-check_cross_talk
	.align	2
	.global	init_smart_info
	.type	init_smart_info, %function
init_smart_info:
	stp	x29, x30, [sp, -128]!
	add	x29, sp, 0
	str	x0, [x29, 24]
	str	x1, [x29, 16]
	ldr	x0, [x29, 24]
	ldr	x0, [x0, 3440]
	mov	w2, 320
	mov	w1, 0
	bl	nand_memset
	ldr	x0, [x29, 24]
	ldr	x0, [x0, 3440]
	mov	w1, 43521
	movk	w1, 0xaaaa, lsl 16
	str	w1, [x0, 112]
	ldr	x0, [x29, 24]
	ldr	x0, [x0, 3440]
	ldr	w1, [x0, 140]
	add	w1, w1, 1
	str	w1, [x0, 140]
	ldr	x0, [x29, 16]
	cmp	x0, 0
	bne	.L1424
	mov	w0, 0
	b	.L1430
.L1424:
	ldr	x0, [x29, 16]
	ldrh	w1, [x0]
	ldr	x0, [x29, 24]
	ldr	x0, [x0, 8]
	ldrh	w0, [x0, 20]
	sub	w0, w0, #1
	uxth	w2, w0
	ldr	x0, [x29, 24]
	ldr	x0, [x0, 8]
	ldrh	w3, [x0, 28]
	ldr	x0, [x29, 24]
	ldr	x4, [x0, 512]
	add	x5, x29, 104
	add	x0, x29, 80
	bl	set_physic_op_par
	ldr	x0, [x29, 24]
	ldr	x2, [x0, 3464]
	add	x0, x29, 80
	mov	x1, x0
	ldr	x0, [x29, 24]
	blr	x2
	str	w0, [x29, 124]
	add	x0, x29, 104
	bl	is_phy_mapping_page
	cmp	w0, 1
	bne	.L1426
	ldr	w0, [x29, 124]
	cmp	w0, 0
	beq	.L1427
	ldr	w0, [x29, 124]
	cmp	w0, 10
	bne	.L1426
.L1427:
	ldr	x0, [x29, 24]
	ldr	x1, [x0, 512]
	ldr	x0, [x29, 24]
	ldr	w0, [x0, 560]
	uxtw	x0, w0
	add	x0, x1, x0
	str	x0, [x29, 64]
	ldr	x0, [x29, 64]
	ldr	w1, [x0, 112]
	mov	w0, 43521
	movk	w0, 0xaaaa, lsl 16
	cmp	w1, w0
	bne	.L1426
	ldr	x0, [x29, 24]
	ldr	x0, [x0, 3440]
	ldr	x1, [x29, 64]
	mov	w2, 320
	bl	nand_memcpy
	ldr	x0, [x29, 24]
	ldr	x0, [x0, 3440]
	ldr	w1, [x0, 140]
	add	w1, w1, 1
	str	w1, [x0, 140]
	ldr	x0, [x29, 24]
	ldr	x0, [x0, 3440]
	ldr	w0, [x0, 164]
	cmn	w0, #1
	bne	.L1428
	ldr	x0, [x29, 24]
	ldr	x0, [x0, 3440]
	str	wzr, [x0, 164]
.L1428:
	ldr	x0, [x29, 24]
	ldr	x0, [x0, 3440]
	ldr	w0, [x0, 172]
	cmn	w0, #1
	bne	.L1429
	ldr	x0, [x29, 24]
	ldr	x0, [x0, 3440]
	str	wzr, [x0, 172]
.L1429:
	ldr	x0, [x29, 24]
	ldr	x0, [x0, 3440]
	ldr	w0, [x0, 176]
	cmn	w0, #1
	bne	.L1426
	ldr	x0, [x29, 24]
	ldr	x0, [x0, 3440]
	str	wzr, [x0, 176]
.L1426:
	ldr	x0, [x29, 24]
	bl	print_smart
	mov	w0, 0
.L1430:
	ldp	x29, x30, [sp], 128
	ret
	.size	init_smart_info, .-init_smart_info
	.align	2
	.global	get_used_block_count
	.type	get_used_block_count, %function
get_used_block_count:
	stp	x29, x30, [sp, -80]!
	add	x29, sp, 0
	str	x0, [x29, 24]
	str	x1, [x29, 16]
	ldr	x0, [x29, 16]
	ldrh	w1, [x0]
	add	x2, x29, 32
	add	x0, x29, 48
	mov	x5, x2
	mov	x4, 0
	mov	w3, 0
	mov	w2, 0
	bl	set_physic_op_par
	ldr	x0, [x29, 24]
	ldr	x2, [x0, 3464]
	add	x0, x29, 48
	mov	x1, x0
	ldr	x0, [x29, 24]
	blr	x2
	add	x0, x29, 32
	bl	get_block_used_count_from_oob
	str	w0, [x29, 76]
	ldr	w0, [x29, 76]
	ldp	x29, x30, [sp], 80
	ret
	.size	get_used_block_count, .-get_used_block_count
	.section	.rodata
	.align	3
.LC313:
	.string	"[NE]current_block_ecc_error no free block!!!\n"
	.align	3
.LC314:
	.string	"[NE]do current_block_ecc_error %d!!\n"
	.align	3
.LC315:
	.string	"[NE]power off ecc error 2.\n"
	.align	3
.LC316:
	.string	"[NE]current_block_ecc ok page %d %d!!\n"
	.align	3
.LC317:
	.string	"[NE]current_block_ecc_error page %d!!\n"
	.align	3
.LC318:
	.string	"[NE]current_block_ecc_error invalid page data %d!!\n"
	.align	3
.LC319:
	.string	"[NE]current_block_ecc_error unkown page data %d!!\n"
	.align	3
.LC320:
	.string	"[ND]do current_block_ecc_error end %d %d!!\n"
	.text
	.align	2
	.global	current_block_ecc_error
	.type	current_block_ecc_error, %function
current_block_ecc_error:
	stp	x29, x30, [sp, -128]!
	add	x29, sp, 0
	str	x0, [x29, 24]
	str	x1, [x29, 16]
.L1434:
	ldr	x0, [x29, 24]
	bl	out_phy_block_from_free_list
	str	x0, [x29, 112]
	ldr	x0, [x29, 112]
	cmp	x0, 0
	bne	.L1435
	adrp	x0, .LC313
	add	x0, x0, :lo12:.LC313
	bl	nand_print
	mov	x0, 0
	b	.L1450
.L1435:
	ldr	x0, [x29, 16]
	ldr	w1, [x0, 8]
	ldr	x0, [x29, 112]
	str	w1, [x0, 8]
	ldr	x0, [x29, 112]
	ldr	w0, [x0, 8]
	mov	w2, w0
	ldr	x1, [x29, 112]
	ldr	x0, [x29, 24]
	bl	new_block_init_for_write
	cmp	w0, 0
	beq	.L1437
	adrp	x0, .LC259
	add	x0, x0, :lo12:.LC259
	bl	nand_print
	b	.L1434
.L1437:
	ldr	x0, [x29, 16]
	ldrh	w0, [x0]
	mov	w1, w0
	adrp	x0, .LC314
	add	x0, x0, :lo12:.LC314
	bl	nand_print
	str	wzr, [x29, 124]
	str	wzr, [x29, 120]
	b	.L1438
.L1447:
	ldr	x0, [x29, 16]
	ldrh	w1, [x0]
	ldr	w0, [x29, 124]
	uxth	w2, w0
	ldr	x0, [x29, 24]
	ldr	x0, [x0, 8]
	ldrh	w3, [x0, 28]
	ldr	x0, [x29, 24]
	ldr	x4, [x0, 512]
	add	x5, x29, 88
	add	x0, x29, 64
	bl	set_physic_op_par
	ldr	x0, [x29, 24]
	ldr	x2, [x0, 3464]
	add	x0, x29, 64
	mov	x1, x0
	ldr	x0, [x29, 24]
	blr	x2
	str	w0, [x29, 108]
	add	x0, x29, 88
	bl	is_ftl_logic_page_data
	cmp	w0, 1
	bne	.L1439
	ldr	w0, [x29, 108]
	cmp	w0, 0
	beq	.L1440
	ldr	w0, [x29, 108]
	cmp	w0, 10
	bne	.L1441
.L1440:
	ldr	x0, [x29, 24]
	ldr	x0, [x0, 512]
	add	x1, x29, 88
	mov	x2, x1
	mov	x1, x0
	ldr	x0, [x29, 24]
	bl	nand_power_off_ecc_error
	cmp	w0, 0
	beq	.L1442
	adrp	x0, .LC315
	add	x0, x0, :lo12:.LC315
	bl	nand_print
	b	.L1445
.L1442:
	adrp	x0, .LC316
	add	x0, x0, :lo12:.LC316
	ldr	w2, [x29, 108]
	ldr	w1, [x29, 124]
	bl	nand_print
	ldr	x0, [x29, 112]
	ldrh	w1, [x0]
	ldr	w0, [x29, 120]
	uxth	w2, w0
	ldr	x0, [x29, 24]
	ldr	x0, [x0, 8]
	ldrh	w3, [x0, 28]
	ldr	x0, [x29, 24]
	ldr	x4, [x0, 512]
	add	x5, x29, 88
	add	x0, x29, 40
	bl	set_physic_op_par
	ldr	x0, [x29, 24]
	ldr	x2, [x0, 3472]
	add	x0, x29, 40
	mov	x1, x0
	ldr	x0, [x29, 24]
	blr	x2
	ldr	w0, [x29, 120]
	add	w0, w0, 1
	str	w0, [x29, 120]
	b	.L1445
.L1441:
	adrp	x0, .LC317
	add	x0, x0, :lo12:.LC317
	ldr	w1, [x29, 124]
	bl	nand_print
	b	.L1445
.L1439:
	add	x0, x29, 88
	bl	is_ftl_special_data
	cmp	w0, 1
	bne	.L1446
	adrp	x0, .LC318
	add	x0, x0, :lo12:.LC318
	ldr	w1, [x29, 124]
	bl	nand_print
	b	.L1445
.L1446:
	add	x0, x29, 88
	bl	is_nouse_page
	cmp	w0, 1
	beq	.L1445
	adrp	x0, .LC319
	add	x0, x0, :lo12:.LC319
	ldr	w1, [x29, 124]
	bl	nand_print
.L1445:
	ldr	w0, [x29, 124]
	add	w0, w0, 1
	str	w0, [x29, 124]
.L1438:
	ldr	x0, [x29, 24]
	ldr	x0, [x0, 8]
	ldrh	w0, [x0, 20]
	mov	w1, w0
	ldr	w0, [x29, 124]
	cmp	w1, w0
	bgt	.L1447
	ldr	x0, [x29, 24]
	ldrb	w0, [x0, 25]
	and	w0, w0, 1
	cmp	w0, 0
	beq	.L1448
	ldr	x0, [x29, 16]
	ldrh	w0, [x0, 6]
	add	w0, w0, 1
	uxth	w0, w0
	mov	w2, w0
	ldr	x1, [x29, 16]
	ldr	x0, [x29, 24]
	bl	erase_block
	b	.L1449
.L1448:
	ldr	x0, [x29, 16]
	ldrh	w0, [x0, 6]
	add	w0, w0, 1
	uxth	w0, w0
	mov	w2, w0
	ldr	x1, [x29, 16]
	ldr	x0, [x29, 24]
	bl	erase_block
.L1449:
	adrp	x0, .LC320
	add	x0, x0, :lo12:.LC320
	ldr	w2, [x29, 120]
	ldr	w1, [x29, 124]
	bl	nand_print_dbg
	ldr	x0, [x29, 112]
.L1450:
	ldp	x29, x30, [sp], 128
	ret
	.size	current_block_ecc_error, .-current_block_ecc_error
	.section	.rodata
	.align	3
.LC321:
	.string	"[NE]do block_last_page_ecc_error!!\n"
	.align	3
.LC322:
	.string	"[NE]%s:malloc fail 1!\n"
	.align	3
.LC323:
	.string	"[NE]%s:malloc fail 2!\n"
	.align	3
.LC324:
	.string	"[NE]current_block_ecc_error invalid page data!!\n"
	.align	3
.LC325:
	.string	"[NE]current_block_ecc_error unkown page data!!\n"
	.align	3
.LC326:
	.string	"[NE]do block_last_page_ecc_error end!!\n"
	.text
	.align	2
	.global	block_last_page_ecc_error
	.type	block_last_page_ecc_error, %function
block_last_page_ecc_error:
	stp	x29, x30, [sp, -112]!
	add	x29, sp, 0
	str	x19, [sp, 16]
	str	x0, [x29, 40]
	str	x1, [x29, 32]
	adrp	x0, .LC321
	add	x0, x0, :lo12:.LC321
	bl	nand_print
	str	wzr, [x29, 104]
	mov	w0, 8192
	bl	nftl_malloc
	str	x0, [x29, 96]
	ldr	x0, [x29, 96]
	cmp	x0, 0
	bne	.L1452
	adrp	x0, __func__.7526
	add	x1, x0, :lo12:__func__.7526
	adrp	x0, .LC322
	add	x0, x0, :lo12:.LC322
	bl	nand_print
.L1452:
	str	wzr, [x29, 108]
	b	.L1453
.L1455:
	ldrsw	x0, [x29, 108]
	lsl	x0, x0, 3
	ldr	x1, [x29, 96]
	add	x19, x1, x0
	ldr	x0, [x29, 40]
	ldr	x0, [x0, 8]
	ldrh	w0, [x0, 26]
	bl	nftl_malloc
	str	x0, [x19]
	ldrsw	x0, [x29, 108]
	lsl	x0, x0, 3
	ldr	x1, [x29, 96]
	add	x0, x1, x0
	ldr	x0, [x0]
	cmp	x0, 0
	bne	.L1454
	adrp	x0, __func__.7526
	add	x1, x0, :lo12:__func__.7526
	adrp	x0, .LC323
	add	x0, x0, :lo12:.LC323
	bl	nand_print
.L1454:
	ldr	w0, [x29, 108]
	add	w0, w0, 1
	str	w0, [x29, 108]
.L1453:
	ldr	x0, [x29, 40]
	ldr	x0, [x0, 8]
	ldrh	w0, [x0, 20]
	mov	w1, w0
	ldr	w0, [x29, 108]
	cmp	w1, w0
	bgt	.L1455
	str	wzr, [x29, 108]
	b	.L1456
.L1466:
	ldr	x0, [x29, 32]
	ldrh	w1, [x0]
	ldr	w0, [x29, 108]
	uxth	w2, w0
	ldr	x0, [x29, 40]
	ldr	x0, [x0, 8]
	ldrh	w3, [x0, 28]
	ldr	x0, [x29, 40]
	ldr	x4, [x0, 512]
	add	x5, x29, 72
	add	x0, x29, 48
	bl	set_physic_op_par
	ldr	x0, [x29, 40]
	ldr	x2, [x0, 3464]
	add	x0, x29, 48
	mov	x1, x0
	ldr	x0, [x29, 40]
	blr	x2
	str	w0, [x29, 92]
	add	x0, x29, 72
	bl	is_nouse_page
	cmp	w0, 1
	beq	.L1475
	add	x0, x29, 72
	bl	is_ftl_logic_page_data
	cmp	w0, 1
	bne	.L1459
	ldr	w0, [x29, 92]
	cmp	w0, 0
	beq	.L1460
	ldr	w0, [x29, 92]
	cmp	w0, 10
	bne	.L1462
.L1460:
	ldrsw	x0, [x29, 108]
	lsl	x0, x0, 3
	ldr	x1, [x29, 96]
	add	x0, x1, x0
	ldr	x3, [x0]
	ldr	x0, [x29, 40]
	ldr	x1, [x0, 512]
	ldr	x0, [x29, 40]
	ldr	x0, [x0, 8]
	ldrh	w0, [x0, 26]
	mov	w2, w0
	mov	x0, x3
	bl	nand_memcpy
	ldr	w0, [x29, 104]
	add	w0, w0, 1
	str	w0, [x29, 104]
	b	.L1462
.L1459:
	add	x0, x29, 72
	bl	is_ftl_special_data
	cmp	w0, 1
	bne	.L1463
	ldr	w0, [x29, 92]
	cmp	w0, 0
	beq	.L1464
	ldr	w0, [x29, 92]
	cmp	w0, 10
	bne	.L1465
.L1464:
	ldrsw	x0, [x29, 108]
	lsl	x0, x0, 3
	ldr	x1, [x29, 96]
	add	x0, x1, x0
	ldr	x3, [x0]
	ldr	x0, [x29, 40]
	ldr	x1, [x0, 512]
	ldr	x0, [x29, 40]
	ldr	x0, [x0, 8]
	ldrh	w0, [x0, 26]
	mov	w2, w0
	mov	x0, x3
	bl	nand_memcpy
	ldr	w0, [x29, 104]
	add	w0, w0, 1
	str	w0, [x29, 104]
.L1465:
	adrp	x0, .LC324
	add	x0, x0, :lo12:.LC324
	bl	nand_print
	b	.L1462
.L1463:
	adrp	x0, .LC325
	add	x0, x0, :lo12:.LC325
	bl	nand_print
.L1462:
	ldr	w0, [x29, 108]
	add	w0, w0, 1
	str	w0, [x29, 108]
.L1456:
	ldr	x0, [x29, 40]
	ldr	x0, [x0, 8]
	ldrh	w0, [x0, 20]
	mov	w1, w0
	ldr	w0, [x29, 108]
	cmp	w1, w0
	bgt	.L1466
	b	.L1458
.L1475:
	nop
.L1458:
	ldr	x0, [x29, 32]
	ldrh	w1, [x0]
	ldr	w0, [x29, 108]
	uxth	w2, w0
	ldr	x0, [x29, 40]
	ldr	x0, [x0, 8]
	ldrh	w3, [x0, 28]
	ldr	x0, [x29, 40]
	ldr	x4, [x0, 512]
	add	x5, x29, 72
	add	x0, x29, 48
	bl	set_physic_op_par
	ldr	x0, [x29, 40]
	ldr	x2, [x0, 3456]
	add	x0, x29, 48
	mov	x1, x0
	ldr	x0, [x29, 40]
	blr	x2
	str	w0, [x29, 92]
	ldr	w0, [x29, 92]
	cmp	w0, 0
	beq	.L1467
	ldr	x0, [x29, 40]
	ldr	x2, [x0, 3496]
	add	x0, x29, 48
	mov	x1, x0
	ldr	x0, [x29, 40]
	blr	x2
	ldr	x0, [x29, 40]
	bl	out_phy_block_from_free_list
	str	x0, [x29, 32]
	ldr	x0, [x29, 32]
	cmp	x0, 0
	bne	.L1468
	adrp	x0, .LC313
	add	x0, x0, :lo12:.LC313
	bl	nand_print
	str	xzr, [x29, 32]
	b	.L1469
.L1468:
	ldr	x0, [x29, 32]
	ldrh	w1, [x0]
	ldr	w0, [x29, 108]
	uxth	w2, w0
	ldr	x0, [x29, 40]
	ldr	x0, [x0, 8]
	ldrh	w3, [x0, 28]
	ldr	x0, [x29, 40]
	ldr	x4, [x0, 512]
	add	x5, x29, 72
	add	x0, x29, 48
	bl	set_physic_op_par
	ldr	x0, [x29, 40]
	ldr	x2, [x0, 3456]
	add	x0, x29, 48
	mov	x1, x0
	ldr	x0, [x29, 40]
	blr	x2
.L1467:
	str	wzr, [x29, 108]
	b	.L1470
.L1471:
	ldr	x0, [x29, 32]
	ldrh	w6, [x0]
	ldr	w0, [x29, 108]
	uxth	w2, w0
	ldr	x0, [x29, 40]
	ldr	x0, [x0, 8]
	ldrh	w3, [x0, 28]
	ldrsw	x0, [x29, 108]
	lsl	x0, x0, 3
	ldr	x1, [x29, 96]
	add	x0, x1, x0
	ldr	x1, [x0]
	add	x4, x29, 72
	add	x0, x29, 48
	mov	x5, x4
	mov	x4, x1
	mov	w1, w6
	bl	set_physic_op_par
	ldr	x0, [x29, 40]
	ldr	x2, [x0, 3472]
	add	x0, x29, 48
	mov	x1, x0
	ldr	x0, [x29, 40]
	blr	x2
	ldr	w0, [x29, 108]
	add	w0, w0, 1
	str	w0, [x29, 108]
.L1470:
	ldr	w1, [x29, 108]
	ldr	w0, [x29, 104]
	cmp	w1, w0
	blt	.L1471
.L1469:
	str	wzr, [x29, 108]
	b	.L1472
.L1473:
	ldrsw	x0, [x29, 108]
	lsl	x0, x0, 3
	ldr	x1, [x29, 96]
	add	x0, x1, x0
	ldr	x0, [x0]
	bl	nftl_free
	ldr	w0, [x29, 108]
	add	w0, w0, 1
	str	w0, [x29, 108]
.L1472:
	ldr	x0, [x29, 40]
	ldr	x0, [x0, 8]
	ldrh	w0, [x0, 20]
	mov	w1, w0
	ldr	w0, [x29, 108]
	cmp	w1, w0
	bgt	.L1473
	ldr	x0, [x29, 96]
	bl	nftl_free
	adrp	x0, .LC326
	add	x0, x0, :lo12:.LC326
	bl	nand_print
	ldr	x0, [x29, 32]
	ldr	x19, [sp, 16]
	ldp	x29, x30, [sp], 112
	ret
	.size	block_last_page_ecc_error, .-block_last_page_ecc_error
	.section	.rodata
	.align	3
.LC327:
	.string	"[NE]adjust free list fail!!\n"
	.align	3
.LC328:
	.string	"[NI]adjust free list : %d %d!!\n"
	.text
	.align	2
	.global	adjust_free_list
	.type	adjust_free_list, %function
adjust_free_list:
	stp	x29, x30, [sp, -48]!
	add	x29, sp, 0
	str	x0, [x29, 24]
	ldr	x0, [x29, 24]
	bl	out_phy_block_from_free_list
	str	x0, [x29, 40]
	ldr	x0, [x29, 40]
	cmp	x0, 0
	bne	.L1477
	adrp	x0, .LC327
	add	x0, x0, :lo12:.LC327
	bl	nand_print
.L1477:
	ldr	x0, [x29, 40]
	ldrh	w0, [x0, 6]
	add	w0, w0, 1
	uxth	w0, w0
	mov	w2, w0
	ldr	x1, [x29, 40]
	ldr	x0, [x29, 24]
	bl	erase_block
	ldr	x1, [x29, 40]
	ldr	x0, [x29, 24]
	bl	out_phy_block_from_free_list_by_block
	str	x0, [x29, 40]
	ldr	x0, [x29, 24]
	ldr	x1, [x29, 40]
	str	x1, [x0, 64]
	ldr	x0, [x29, 40]
	ldrh	w0, [x0]
	mov	w1, w0
	ldr	x0, [x29, 40]
	ldrh	w0, [x0, 6]
	mov	w2, w0
	adrp	x0, .LC328
	add	x0, x0, :lo12:.LC328
	bl	nand_print
	mov	w0, 0
	ldp	x29, x30, [sp], 48
	ret
	.size	adjust_free_list, .-adjust_free_list
	.align	2
	.global	nftl_get_bad_block_cnt
	.type	nftl_get_bad_block_cnt, %function
nftl_get_bad_block_cnt:
	sub	sp, sp, #16
	str	x0, [sp, 8]
	ldr	x0, [sp, 8]
	ldr	x0, [x0, 24]
	ldrh	w0, [x0, 28]
	add	sp, sp, 16
	ret
	.size	nftl_get_bad_block_cnt, .-nftl_get_bad_block_cnt
	.section	.rodata
	.align	3
.LC329:
	.string	"[ND]nand3.x \n"
	.align	3
.LC330:
	.string	"[ND]zone->nand_chip->blk_per_chip: %d \n"
	.align	3
.LC331:
	.string	"[ND]zone->nand_chip->bytes_per_page: %d \n"
	.align	3
.LC332:
	.string	"[ND]zone->nand_chip->pages_per_blk: %d \n"
	.align	3
.LC333:
	.string	"[ND]zone->nand_chip->max_erase_times: %d \n"
	.align	3
.LC334:
	.string	"[ND]zone->nand_chip->support_read_reclaim: %d \n"
	.align	3
.LC335:
	.string	"[ND]zone->test: %d \n"
	.align	3
.LC336:
	.string	"[ND]zone->zone_no: %d \n"
	.align	3
.LC337:
	.string	"[ND]zone->zone_attr: %d \n"
	.align	3
.LC338:
	.string	"[ND]zone->blocks: %d \n"
	.align	3
.LC339:
	.string	"[ND]zone->bad_block: %d \n"
	.align	3
.LC340:
	.string	"[ND]zone->logic_cap_in_sects: %d \n"
	.align	3
.LC341:
	.string	"[ND]zone->backup_cap_in_sects: %d \n"
	.align	3
.LC342:
	.string	"[ND]zone->free_block_num: %d \n"
	.align	3
.LC343:
	.string	"[ND]zone->gc_strategy.start_gc_free_blocks: %d \n"
	.align	3
.LC344:
	.string	"[ND]zone->gc_strategy.stop_gc_free_blocks: %d \n"
	.align	3
.LC345:
	.string	"[ND]zone->gc_strategy.gc_page: %d \n"
	.align	3
.LC346:
	.string	"[ND]zone->gc_strategy.process: %d \n"
	.align	3
.LC347:
	.string	"[ND]zone->prio_gc.prio_type : %d \n"
	.align	3
.LC348:
	.string	"[ND]zone->zone_start_phy_block->phy_block.Block_NO: %d \n"
	.align	3
.LC349:
	.string	"[ND]zone->zone_end_phy_block->phy_block.Block_NO: %d \n"
	.align	3
.LC350:
	.string	"[ND]zone->zone_phy_page_map_for_gc: %x \n"
	.align	3
.LC351:
	.string	"[ND]zone->current_block.user_info: %x \n"
	.align	3
.LC352:
	.string	"[ND]zone->current_block.block_info: %x \n"
	.align	3
.LC353:
	.string	"[ND]zone->current_block.block_info->phy_block.Block_NO: %d \n"
	.align	3
.LC354:
	.string	"[ND]zone->current_block.page_used: %d \n"
	.align	3
.LC355:
	.string	"[ND]zone->current_block.block_info->block_used_count: %d \n"
	.align	3
.LC356:
	.string	"[ND]zone->recv_free_block: %d %d!!\n"
	.align	3
.LC357:
	.string	"[ND]zone->assist_block.user_info: %x \n"
	.align	3
.LC358:
	.string	"[ND]zone->assist_block.block_info: %x \n"
	.align	3
.LC359:
	.string	"[ND]zone->assist_block.block_info->phy_block.Block_NO: %d \n"
	.align	3
.LC360:
	.string	"[ND]zone->assist_block.page_used: %d \n"
	.align	3
.LC361:
	.string	"[ND]zone->assist_block.block_info->block_used_count: %d \n"
	.align	3
.LC362:
	.string	"[ND]zone->read_reclaim_complete: %x \n"
	.align	3
.LC363:
	.string	"[ND]zone->temp_page_buf: %x \n"
	.align	3
.LC364:
	.string	"[ND]zone->max_erase_num: %d \n"
	.align	3
.LC365:
	.string	"[ND]zone->cache.cache_totals: %x \n"
	.align	3
.LC366:
	.string	"[ND]zone->wl_gc_count: %d \n"
	.align	3
.LC367:
	.string	"[ND]zone->wl_gc_page_num: %d \n"
	.align	3
.LC368:
	.string	"[ND]zone->cfg->nftl_dont_use_cache: %x \n"
	.align	3
.LC369:
	.string	"[ND]zone->cfg->nftl_use_cache_sort: %x \n"
	.align	3
.LC370:
	.string	"[ND]zone->cfg->nftl_support_gc_read_reclaim: %x \n"
	.align	3
.LC371:
	.string	"[ND]zone->cfg->nftl_support_wear_leveling: %x \n"
	.align	3
.LC372:
	.string	"[ND]zone->cfg->nftl_need_erase: %x \n"
	.align	3
.LC373:
	.string	"[ND]zone->cfg->nftl_min_free_block_num: %x \n"
	.align	3
.LC374:
	.string	"[ND]zone->cfg->nftl_gc_threshold_free_block_num: %x \n"
	.align	3
.LC375:
	.string	"[ND]zone->cfg->nftl_min_free_block: %x \n"
	.align	3
.LC376:
	.string	"[ND]zone->cfg->nftl_gc_threshold_ratio_numerator: %x \n"
	.align	3
.LC377:
	.string	"[ND]zone->cfg->nftl_gc_threshold_ratio_denominator: %x \n"
	.align	3
.LC378:
	.string	"[ND]zone->cfg->nftl_max_cache_num: %d \n"
	.align	3
.LC379:
	.string	"[ND]zone->cfg->nftl_max_cache_write_num: %d \n"
	.align	3
.LC380:
	.string	"[ND]zone->cfg->nftl_cross_talk: %x \n"
	.align	3
.LC381:
	.string	"[ND]zone->cfg->nftl_read_claim_interval: %d \n"
	.text
	.align	2
	.global	print_nftl_zone
	.type	print_nftl_zone, %function
print_nftl_zone:
	stp	x29, x30, [sp, -80]!
	add	x29, sp, 0
	str	x0, [x29, 72]
	ldr	x0, [x29, 72]
	ldr	w0, [x0, 656]
	cmp	w0, 0
	beq	.L1485
	adrp	x0, .LC329
	add	x0, x0, :lo12:.LC329
	bl	nand_print_dbg
	ldr	x0, [x29, 72]
	ldr	x0, [x0, 8]
	ldrh	w0, [x0, 24]
	mov	w1, w0
	adrp	x0, .LC330
	add	x0, x0, :lo12:.LC330
	bl	nand_print_dbg
	ldr	x0, [x29, 72]
	ldr	x0, [x0, 8]
	ldrh	w0, [x0, 26]
	mov	w1, w0
	adrp	x0, .LC331
	add	x0, x0, :lo12:.LC331
	bl	nand_print_dbg
	ldr	x0, [x29, 72]
	ldr	x0, [x0, 8]
	ldrh	w0, [x0, 20]
	mov	w1, w0
	adrp	x0, .LC332
	add	x0, x0, :lo12:.LC332
	bl	nand_print_dbg
	ldr	x0, [x29, 72]
	ldr	x0, [x0, 8]
	ldrh	w0, [x0, 16]
	mov	w1, w0
	adrp	x0, .LC333
	add	x0, x0, :lo12:.LC333
	bl	nand_print_dbg
	ldr	x0, [x29, 72]
	ldr	x0, [x0, 8]
	ldrh	w0, [x0, 30]
	mov	w1, w0
	adrp	x0, .LC334
	add	x0, x0, :lo12:.LC334
	bl	nand_print_dbg
	ldr	x0, [x29, 72]
	ldr	w1, [x0, 656]
	adrp	x0, .LC335
	add	x0, x0, :lo12:.LC335
	bl	nand_print_dbg
	ldr	x0, [x29, 72]
	ldrb	w0, [x0, 24]
	mov	w1, w0
	adrp	x0, .LC336
	add	x0, x0, :lo12:.LC336
	bl	nand_print_dbg
	ldr	x0, [x29, 72]
	ldrb	w0, [x0, 25]
	mov	w1, w0
	adrp	x0, .LC337
	add	x0, x0, :lo12:.LC337
	bl	nand_print_dbg
	ldr	x0, [x29, 72]
	ldrh	w0, [x0, 26]
	mov	w1, w0
	adrp	x0, .LC338
	add	x0, x0, :lo12:.LC338
	bl	nand_print_dbg
	ldr	x0, [x29, 72]
	ldrh	w0, [x0, 28]
	mov	w1, w0
	adrp	x0, .LC339
	add	x0, x0, :lo12:.LC339
	bl	nand_print_dbg
	ldr	x0, [x29, 72]
	ldr	w1, [x0, 32]
	adrp	x0, .LC340
	add	x0, x0, :lo12:.LC340
	bl	nand_print_dbg
	ldr	x0, [x29, 72]
	ldr	w1, [x0, 40]
	adrp	x0, .LC341
	add	x0, x0, :lo12:.LC341
	bl	nand_print_dbg
	ldr	x0, [x29, 72]
	ldrh	w0, [x0, 30]
	mov	w1, w0
	adrp	x0, .LC342
	add	x0, x0, :lo12:.LC342
	bl	nand_print_dbg
	ldr	x0, [x29, 72]
	ldrh	w0, [x0, 680]
	mov	w1, w0
	adrp	x0, .LC343
	add	x0, x0, :lo12:.LC343
	bl	nand_print_dbg
	ldr	x0, [x29, 72]
	ldrh	w0, [x0, 682]
	mov	w1, w0
	adrp	x0, .LC344
	add	x0, x0, :lo12:.LC344
	bl	nand_print_dbg
	ldr	x0, [x29, 72]
	ldrh	w0, [x0, 688]
	mov	w1, w0
	adrp	x0, .LC345
	add	x0, x0, :lo12:.LC345
	bl	nand_print_dbg
	ldr	x0, [x29, 72]
	ldrh	w0, [x0, 684]
	mov	w1, w0
	adrp	x0, .LC346
	add	x0, x0, :lo12:.LC346
	bl	nand_print_dbg
	ldr	x0, [x29, 72]
	ldrh	w0, [x0, 698]
	mov	w1, w0
	adrp	x0, .LC347
	add	x0, x0, :lo12:.LC347
	bl	nand_print_dbg
	ldr	x0, [x29, 72]
	ldr	x0, [x0, 48]
	ldrh	w0, [x0]
	mov	w1, w0
	adrp	x0, .LC348
	add	x0, x0, :lo12:.LC348
	bl	nand_print_dbg
	ldr	x0, [x29, 72]
	ldr	x0, [x0, 56]
	ldrh	w0, [x0]
	mov	w1, w0
	adrp	x0, .LC349
	add	x0, x0, :lo12:.LC349
	bl	nand_print_dbg
	ldr	x0, [x29, 72]
	add	x0, x0, 512
	sub	x1, x0, #48
	add	x0, x29, 16
	ldp	x2, x3, [x1]
	stp	x2, x3, [x0]
	ldp	x2, x3, [x1, 16]
	stp	x2, x3, [x0, 16]
	ldp	x1, x2, [x1, 32]
	stp	x1, x2, [x0, 32]
	add	x1, x29, 16
	adrp	x0, .LC350
	add	x0, x0, :lo12:.LC350
	bl	nand_print_dbg
	ldr	x0, [x29, 72]
	add	x0, x0, 512
	add	x1, x0, 24
	add	x0, x29, 16
	ldp	x2, x3, [x1]
	stp	x2, x3, [x0]
	ldp	x2, x3, [x1, 16]
	stp	x2, x3, [x0, 16]
	ldp	x1, x2, [x1, 32]
	stp	x1, x2, [x0, 32]
	add	x1, x29, 16
	adrp	x0, .LC351
	add	x0, x0, :lo12:.LC351
	bl	nand_print_dbg
	ldr	x0, [x29, 72]
	ldr	x1, [x0, 528]
	adrp	x0, .LC352
	add	x0, x0, :lo12:.LC352
	bl	nand_print_dbg
	ldr	x0, [x29, 72]
	ldr	x0, [x0, 528]
	ldrh	w0, [x0]
	mov	w1, w0
	adrp	x0, .LC353
	add	x0, x0, :lo12:.LC353
	bl	nand_print_dbg
	ldr	x0, [x29, 72]
	ldrh	w0, [x0, 584]
	mov	w1, w0
	adrp	x0, .LC354
	add	x0, x0, :lo12:.LC354
	bl	nand_print_dbg
	ldr	x0, [x29, 72]
	ldr	x0, [x0, 528]
	ldr	w1, [x0, 8]
	adrp	x0, .LC355
	add	x0, x0, :lo12:.LC355
	bl	nand_print_dbg
	ldr	x0, [x29, 72]
	ldr	x0, [x0, 64]
	ldrh	w0, [x0]
	mov	w1, w0
	ldr	x0, [x29, 72]
	ldr	x0, [x0, 64]
	ldrh	w0, [x0, 6]
	mov	w2, w0
	adrp	x0, .LC356
	add	x0, x0, :lo12:.LC356
	bl	nand_print_dbg
	ldr	x0, [x29, 72]
	ldrb	w0, [x0, 25]
	and	w0, w0, 1
	cmp	w0, 0
	beq	.L1484
	ldr	x0, [x29, 72]
	add	x0, x0, 512
	add	x1, x0, 88
	add	x0, x29, 16
	ldp	x2, x3, [x1]
	stp	x2, x3, [x0]
	ldp	x2, x3, [x1, 16]
	stp	x2, x3, [x0, 16]
	ldp	x1, x2, [x1, 32]
	stp	x1, x2, [x0, 32]
	add	x1, x29, 16
	adrp	x0, .LC357
	add	x0, x0, :lo12:.LC357
	bl	nand_print_dbg
	ldr	x0, [x29, 72]
	ldr	x1, [x0, 592]
	adrp	x0, .LC358
	add	x0, x0, :lo12:.LC358
	bl	nand_print_dbg
	ldr	x0, [x29, 72]
	ldr	x0, [x0, 592]
	ldrh	w0, [x0]
	mov	w1, w0
	adrp	x0, .LC359
	add	x0, x0, :lo12:.LC359
	bl	nand_print_dbg
	ldr	x0, [x29, 72]
	ldrh	w0, [x0, 648]
	mov	w1, w0
	adrp	x0, .LC360
	add	x0, x0, :lo12:.LC360
	bl	nand_print_dbg
	ldr	x0, [x29, 72]
	ldr	x0, [x0, 592]
	ldr	w1, [x0, 8]
	adrp	x0, .LC361
	add	x0, x0, :lo12:.LC361
	bl	nand_print_dbg
.L1484:
	ldr	x0, [x29, 72]
	ldr	w1, [x0, 660]
	adrp	x0, .LC362
	add	x0, x0, :lo12:.LC362
	bl	nand_print_dbg
	ldr	x0, [x29, 72]
	ldr	x1, [x0, 512]
	adrp	x0, .LC363
	add	x0, x0, :lo12:.LC363
	bl	nand_print_dbg
	ldr	x0, [x29, 72]
	ldr	w1, [x0, 676]
	adrp	x0, .LC364
	add	x0, x0, :lo12:.LC364
	bl	nand_print_dbg
	ldr	x0, [x29, 72]
	ldrh	w0, [x0, 1072]
	mov	w1, w0
	adrp	x0, .LC365
	add	x0, x0, :lo12:.LC365
	bl	nand_print_dbg
	ldr	x0, [x29, 72]
	ldr	w1, [x0, 668]
	adrp	x0, .LC366
	add	x0, x0, :lo12:.LC366
	bl	nand_print_dbg
	ldr	x0, [x29, 72]
	ldr	w1, [x0, 672]
	adrp	x0, .LC367
	add	x0, x0, :lo12:.LC367
	bl	nand_print_dbg
	ldr	x0, [x29, 72]
	ldr	x0, [x0, 3448]
	ldrh	w0, [x0]
	mov	w1, w0
	adrp	x0, .LC368
	add	x0, x0, :lo12:.LC368
	bl	nand_print_dbg
	ldr	x0, [x29, 72]
	ldr	x0, [x0, 3448]
	ldrh	w0, [x0, 2]
	mov	w1, w0
	adrp	x0, .LC369
	add	x0, x0, :lo12:.LC369
	bl	nand_print_dbg
	ldr	x0, [x29, 72]
	ldr	x0, [x0, 3448]
	ldrh	w0, [x0, 4]
	mov	w1, w0
	adrp	x0, .LC370
	add	x0, x0, :lo12:.LC370
	bl	nand_print_dbg
	ldr	x0, [x29, 72]
	ldr	x0, [x0, 3448]
	ldrh	w0, [x0, 6]
	mov	w1, w0
	adrp	x0, .LC371
	add	x0, x0, :lo12:.LC371
	bl	nand_print_dbg
	ldr	x0, [x29, 72]
	ldr	x0, [x0, 3448]
	ldrh	w0, [x0, 10]
	mov	w1, w0
	adrp	x0, .LC372
	add	x0, x0, :lo12:.LC372
	bl	nand_print_dbg
	ldr	x0, [x29, 72]
	ldr	x0, [x0, 3448]
	ldrh	w0, [x0, 12]
	mov	w1, w0
	adrp	x0, .LC373
	add	x0, x0, :lo12:.LC373
	bl	nand_print_dbg
	ldr	x0, [x29, 72]
	ldr	x0, [x0, 3448]
	ldrh	w0, [x0, 14]
	mov	w1, w0
	adrp	x0, .LC374
	add	x0, x0, :lo12:.LC374
	bl	nand_print_dbg
	ldr	x0, [x29, 72]
	ldr	x0, [x0, 3448]
	ldrh	w0, [x0, 16]
	mov	w1, w0
	adrp	x0, .LC375
	add	x0, x0, :lo12:.LC375
	bl	nand_print_dbg
	ldr	x0, [x29, 72]
	ldr	x0, [x0, 3448]
	ldrh	w0, [x0, 18]
	mov	w1, w0
	adrp	x0, .LC376
	add	x0, x0, :lo12:.LC376
	bl	nand_print_dbg
	ldr	x0, [x29, 72]
	ldr	x0, [x0, 3448]
	ldrh	w0, [x0, 20]
	mov	w1, w0
	adrp	x0, .LC377
	add	x0, x0, :lo12:.LC377
	bl	nand_print_dbg
	ldr	x0, [x29, 72]
	ldr	x0, [x0, 3448]
	ldrh	w0, [x0, 22]
	mov	w1, w0
	adrp	x0, .LC378
	add	x0, x0, :lo12:.LC378
	bl	nand_print_dbg
	ldr	x0, [x29, 72]
	ldr	x0, [x0, 3448]
	ldrh	w0, [x0, 24]
	mov	w1, w0
	adrp	x0, .LC379
	add	x0, x0, :lo12:.LC379
	bl	nand_print_dbg
	ldr	x0, [x29, 72]
	ldr	x0, [x0, 3448]
	ldrh	w0, [x0, 26]
	mov	w1, w0
	adrp	x0, .LC380
	add	x0, x0, :lo12:.LC380
	bl	nand_print_dbg
	ldr	x0, [x29, 72]
	ldr	x0, [x0, 3448]
	ldr	w1, [x0, 32]
	adrp	x0, .LC381
	add	x0, x0, :lo12:.LC381
	bl	nand_print_dbg
	ldr	x0, [x29, 72]
	bl	print_smart
	b	.L1481
.L1485:
	nop
.L1481:
	ldp	x29, x30, [sp], 80
	ret
	.size	print_nftl_zone, .-print_nftl_zone
	.section	.rodata
	.align	3
.LC382:
	.string	"[NE]%s:malloc 4096 fail!\n"
	.align	3
.LC383:
	.string	"[ND]spare: %x %x %x %x %x %x %x %x %x %x %x %x %x %x %x %x"
	.align	3
.LC384:
	.string	"[ND]buf:%x %x %x %x %x %x %x %x %x %x %x.\n"
	.text
	.align	2
	.global	nand_dbg_phy_read
	.type	nand_dbg_phy_read, %function
nand_dbg_phy_read:
	sub	sp, sp, #144
	stp	x29, x30, [sp, 80]
	add	x29, sp, 80
	strh	w0, [x29, 30]
	strh	w1, [x29, 28]
	strh	w2, [x29, 26]
	mov	w0, 4096
	bl	nftl_malloc
	str	x0, [x29, 56]
	ldr	x0, [x29, 56]
	cmp	x0, 0
	bne	.L1487
	adrp	x0, __func__.7558
	add	x1, x0, :lo12:__func__.7558
	adrp	x0, .LC382
	add	x0, x0, :lo12:.LC382
	bl	nand_print
.L1487:
	add	x0, x29, 32
	mov	x5, x0
	ldr	x4, [x29, 56]
	mov	w3, 8
	ldrh	w2, [x29, 26]
	ldrh	w1, [x29, 28]
	ldrh	w0, [x29, 30]
	bl	page_read
	str	w0, [x29, 52]
	ldrb	w0, [x29, 32]
	mov	w10, w0
	ldrb	w0, [x29, 33]
	mov	w11, w0
	ldrb	w0, [x29, 34]
	mov	w12, w0
	ldrb	w0, [x29, 35]
	mov	w13, w0
	ldrb	w0, [x29, 36]
	mov	w14, w0
	ldrb	w0, [x29, 37]
	mov	w15, w0
	ldrb	w0, [x29, 38]
	mov	w16, w0
	ldrb	w0, [x29, 39]
	mov	w1, w0
	ldrb	w0, [x29, 40]
	mov	w2, w0
	ldrb	w0, [x29, 41]
	mov	w3, w0
	ldrb	w0, [x29, 42]
	mov	w4, w0
	ldrb	w0, [x29, 43]
	mov	w5, w0
	ldrb	w0, [x29, 44]
	mov	w6, w0
	ldrb	w0, [x29, 45]
	mov	w7, w0
	ldrb	w0, [x29, 46]
	mov	w8, w0
	ldrb	w0, [x29, 47]
	mov	w9, w0
	adrp	x0, .LC383
	add	x0, x0, :lo12:.LC383
	str	w9, [sp, 64]
	str	w8, [sp, 56]
	str	w7, [sp, 48]
	str	w6, [sp, 40]
	str	w5, [sp, 32]
	str	w4, [sp, 24]
	str	w3, [sp, 16]
	str	w2, [sp, 8]
	str	w1, [sp]
	mov	w7, w16
	mov	w6, w15
	mov	w5, w14
	mov	w4, w13
	mov	w3, w12
	mov	w2, w11
	mov	w1, w10
	bl	nand_print_dbg
	ldr	x0, [x29, 56]
	ldrb	w0, [x0]
	mov	w8, w0
	ldr	x0, [x29, 56]
	add	x0, x0, 1
	ldrb	w0, [x0]
	mov	w9, w0
	ldr	x0, [x29, 56]
	add	x0, x0, 2
	ldrb	w0, [x0]
	mov	w10, w0
	ldr	x0, [x29, 56]
	add	x0, x0, 3
	ldrb	w0, [x0]
	mov	w11, w0
	ldr	x0, [x29, 56]
	add	x0, x0, 4
	ldrb	w0, [x0]
	mov	w5, w0
	ldr	x0, [x29, 56]
	add	x0, x0, 5
	ldrb	w0, [x0]
	mov	w6, w0
	ldr	x0, [x29, 56]
	add	x0, x0, 6
	ldrb	w0, [x0]
	mov	w7, w0
	ldr	x0, [x29, 56]
	add	x0, x0, 7
	ldrb	w0, [x0]
	mov	w1, w0
	ldr	x0, [x29, 56]
	add	x0, x0, 8
	ldrb	w0, [x0]
	mov	w2, w0
	ldr	x0, [x29, 56]
	add	x0, x0, 9
	ldrb	w0, [x0]
	mov	w3, w0
	ldr	x0, [x29, 56]
	add	x0, x0, 10
	ldrb	w0, [x0]
	mov	w4, w0
	adrp	x0, .LC384
	add	x0, x0, :lo12:.LC384
	str	w4, [sp, 24]
	str	w3, [sp, 16]
	str	w2, [sp, 8]
	str	w1, [sp]
	mov	w4, w11
	mov	w3, w10
	mov	w2, w9
	mov	w1, w8
	bl	nand_print_dbg
	ldr	x0, [x29, 56]
	bl	nftl_free
	ldr	w0, [x29, 52]
	add	sp, x29, 0
	ldp	x29, x30, [sp], 64
	ret
	.size	nand_dbg_phy_read, .-nand_dbg_phy_read
	.align	2
	.global	nand_dbg_zone_phy_read
	.type	nand_dbg_zone_phy_read, %function
nand_dbg_zone_phy_read:
	sub	sp, sp, #176
	stp	x29, x30, [sp, 80]
	add	x29, sp, 80
	str	x0, [x29, 24]
	strh	w1, [x29, 22]
	strh	w2, [x29, 20]
	mov	w0, 4096
	bl	nftl_malloc
	str	x0, [x29, 88]
	ldr	x0, [x29, 88]
	cmp	x0, 0
	bne	.L1490
	adrp	x0, __func__.7569
	add	x1, x0, :lo12:__func__.7569
	adrp	x0, .LC382
	add	x0, x0, :lo12:.LC382
	bl	nand_print
.L1490:
	ldrh	w1, [x29, 22]
	ldr	x0, [x29, 24]
	bl	get_phy_block_addr
	str	x0, [x29, 80]
	ldr	x0, [x29, 80]
	ldrh	w1, [x0]
	add	x2, x29, 32
	add	x0, x29, 48
	mov	x5, x2
	ldr	x4, [x29, 88]
	mov	w3, 8
	ldrh	w2, [x29, 20]
	bl	set_physic_op_par
	ldr	x0, [x29, 24]
	ldr	x2, [x0, 3464]
	add	x0, x29, 48
	mov	x1, x0
	ldr	x0, [x29, 24]
	blr	x2
	str	w0, [x29, 76]
	ldrb	w0, [x29, 32]
	mov	w10, w0
	ldrb	w0, [x29, 33]
	mov	w11, w0
	ldrb	w0, [x29, 34]
	mov	w12, w0
	ldrb	w0, [x29, 35]
	mov	w13, w0
	ldrb	w0, [x29, 36]
	mov	w14, w0
	ldrb	w0, [x29, 37]
	mov	w15, w0
	ldrb	w0, [x29, 38]
	mov	w16, w0
	ldrb	w0, [x29, 39]
	mov	w1, w0
	ldrb	w0, [x29, 40]
	mov	w2, w0
	ldrb	w0, [x29, 41]
	mov	w3, w0
	ldrb	w0, [x29, 42]
	mov	w4, w0
	ldrb	w0, [x29, 43]
	mov	w5, w0
	ldrb	w0, [x29, 44]
	mov	w6, w0
	ldrb	w0, [x29, 45]
	mov	w7, w0
	ldrb	w0, [x29, 46]
	mov	w8, w0
	ldrb	w0, [x29, 47]
	mov	w9, w0
	adrp	x0, .LC383
	add	x0, x0, :lo12:.LC383
	str	w9, [sp, 64]
	str	w8, [sp, 56]
	str	w7, [sp, 48]
	str	w6, [sp, 40]
	str	w5, [sp, 32]
	str	w4, [sp, 24]
	str	w3, [sp, 16]
	str	w2, [sp, 8]
	str	w1, [sp]
	mov	w7, w16
	mov	w6, w15
	mov	w5, w14
	mov	w4, w13
	mov	w3, w12
	mov	w2, w11
	mov	w1, w10
	bl	nand_print_dbg
	ldr	x0, [x29, 88]
	ldrb	w0, [x0]
	mov	w8, w0
	ldr	x0, [x29, 88]
	add	x0, x0, 1
	ldrb	w0, [x0]
	mov	w9, w0
	ldr	x0, [x29, 88]
	add	x0, x0, 2
	ldrb	w0, [x0]
	mov	w10, w0
	ldr	x0, [x29, 88]
	add	x0, x0, 3
	ldrb	w0, [x0]
	mov	w11, w0
	ldr	x0, [x29, 88]
	add	x0, x0, 4
	ldrb	w0, [x0]
	mov	w5, w0
	ldr	x0, [x29, 88]
	add	x0, x0, 5
	ldrb	w0, [x0]
	mov	w6, w0
	ldr	x0, [x29, 88]
	add	x0, x0, 6
	ldrb	w0, [x0]
	mov	w7, w0
	ldr	x0, [x29, 88]
	add	x0, x0, 7
	ldrb	w0, [x0]
	mov	w1, w0
	ldr	x0, [x29, 88]
	add	x0, x0, 8
	ldrb	w0, [x0]
	mov	w2, w0
	ldr	x0, [x29, 88]
	add	x0, x0, 9
	ldrb	w0, [x0]
	mov	w3, w0
	ldr	x0, [x29, 88]
	add	x0, x0, 10
	ldrb	w0, [x0]
	mov	w4, w0
	adrp	x0, .LC384
	add	x0, x0, :lo12:.LC384
	str	w4, [sp, 24]
	str	w3, [sp, 16]
	str	w2, [sp, 8]
	str	w1, [sp]
	mov	w4, w11
	mov	w3, w10
	mov	w2, w9
	mov	w1, w8
	bl	nand_print_dbg
	ldr	x0, [x29, 88]
	bl	nftl_free
	ldr	w0, [x29, 76]
	add	sp, x29, 0
	ldp	x29, x30, [sp], 96
	ret
	.size	nand_dbg_zone_phy_read, .-nand_dbg_zone_phy_read
	.section	.rodata
	.align	3
.LC385:
	.string	"[NE]%s:malloc 8192 fail!\n"
	.text
	.align	2
	.global	nand_dbg_zone_phy_write
	.type	nand_dbg_zone_phy_write, %function
nand_dbg_zone_phy_write:
	stp	x29, x30, [sp, -96]!
	add	x29, sp, 0
	str	x0, [x29, 24]
	strh	w1, [x29, 22]
	strh	w2, [x29, 20]
	mov	w0, 8192
	bl	nftl_malloc
	str	x0, [x29, 88]
	ldr	x0, [x29, 88]
	cmp	x0, 0
	bne	.L1493
	adrp	x0, __func__.7580
	add	x1, x0, :lo12:__func__.7580
	adrp	x0, .LC385
	add	x0, x0, :lo12:.LC385
	bl	nand_print
.L1493:
	ldrh	w1, [x29, 22]
	ldr	x0, [x29, 24]
	bl	get_phy_block_addr
	str	x0, [x29, 80]
	mov	w2, 4096
	mov	w1, -91
	ldr	x0, [x29, 88]
	bl	nand_memset
	mov	w0, -1
	strb	w0, [x29, 32]
	mov	w0, -1
	strb	w0, [x29, 33]
	mov	w0, -1
	strb	w0, [x29, 34]
	mov	w0, -1
	strb	w0, [x29, 35]
	mov	w0, -1
	strb	w0, [x29, 36]
	mov	w0, -91
	strb	w0, [x29, 37]
	mov	w0, -91
	strb	w0, [x29, 38]
	mov	w0, -1
	strb	w0, [x29, 39]
	mov	w0, -1
	strb	w0, [x29, 40]
	mov	w0, -1
	strb	w0, [x29, 41]
	mov	w0, -1
	strb	w0, [x29, 42]
	ldr	x0, [x29, 80]
	ldrh	w1, [x0]
	ldr	x0, [x29, 24]
	ldr	x0, [x0, 8]
	ldrh	w2, [x0, 28]
	add	x3, x29, 32
	add	x0, x29, 48
	mov	x5, x3
	ldr	x4, [x29, 88]
	mov	w3, w2
	ldrh	w2, [x29, 20]
	bl	set_physic_op_par
	ldr	x0, [x29, 24]
	ldr	x2, [x0, 3472]
	add	x0, x29, 48
	mov	x1, x0
	ldr	x0, [x29, 24]
	blr	x2
	str	w0, [x29, 76]
	ldr	x0, [x29, 88]
	bl	nftl_free
	ldr	w0, [x29, 76]
	ldp	x29, x30, [sp], 96
	ret
	.size	nand_dbg_zone_phy_write, .-nand_dbg_zone_phy_write
	.align	2
	.global	nand_dbg_phy_write
	.type	nand_dbg_phy_write, %function
nand_dbg_phy_write:
	stp	x29, x30, [sp, -64]!
	add	x29, sp, 0
	strh	w0, [x29, 30]
	strh	w1, [x29, 28]
	strh	w2, [x29, 26]
	mov	w0, 8192
	bl	nftl_malloc
	str	x0, [x29, 56]
	ldr	x0, [x29, 56]
	cmp	x0, 0
	bne	.L1496
	adrp	x0, __func__.7589
	add	x1, x0, :lo12:__func__.7589
	adrp	x0, .LC385
	add	x0, x0, :lo12:.LC385
	bl	nand_print
.L1496:
	mov	w2, 4096
	mov	w1, -91
	ldr	x0, [x29, 56]
	bl	nand_memset
	mov	w0, -1
	strb	w0, [x29, 32]
	mov	w0, -1
	strb	w0, [x29, 33]
	mov	w0, -1
	strb	w0, [x29, 34]
	mov	w0, -1
	strb	w0, [x29, 35]
	mov	w0, -1
	strb	w0, [x29, 36]
	mov	w0, -91
	strb	w0, [x29, 37]
	mov	w0, -91
	strb	w0, [x29, 38]
	mov	w0, -1
	strb	w0, [x29, 39]
	mov	w0, -1
	strb	w0, [x29, 40]
	mov	w0, -1
	strb	w0, [x29, 41]
	mov	w0, -1
	strb	w0, [x29, 42]
	add	x0, x29, 32
	mov	x5, x0
	ldr	x4, [x29, 56]
	mov	w3, 8
	ldrh	w2, [x29, 26]
	ldrh	w1, [x29, 28]
	ldrh	w0, [x29, 30]
	bl	page_write
	str	w0, [x29, 52]
	ldr	x0, [x29, 56]
	bl	nftl_free
	ldr	w0, [x29, 52]
	ldp	x29, x30, [sp], 64
	ret
	.size	nand_dbg_phy_write, .-nand_dbg_phy_write
	.align	2
	.global	nand_dbg_zone_erase
	.type	nand_dbg_zone_erase, %function
nand_dbg_zone_erase:
	stp	x29, x30, [sp, -48]!
	add	x29, sp, 0
	str	x0, [x29, 24]
	strh	w1, [x29, 22]
	strh	w2, [x29, 20]
	ldrh	w1, [x29, 22]
	ldr	x0, [x29, 24]
	bl	get_phy_block_addr
	str	x0, [x29, 40]
	ldr	x1, [x29, 40]
	ldr	x0, [x29, 24]
	bl	out_phy_block_from_free_list_by_block
	str	x0, [x29, 40]
	ldrh	w2, [x29, 20]
	ldr	x1, [x29, 40]
	ldr	x0, [x29, 24]
	bl	erase_block
	str	w0, [x29, 36]
	ldr	w0, [x29, 36]
	ldp	x29, x30, [sp], 48
	ret
	.size	nand_dbg_zone_erase, .-nand_dbg_zone_erase
	.align	2
	.global	nand_dbg_phy_erase
	.type	nand_dbg_phy_erase, %function
nand_dbg_phy_erase:
	stp	x29, x30, [sp, -48]!
	add	x29, sp, 0
	strh	w0, [x29, 30]
	strh	w1, [x29, 28]
	ldrh	w1, [x29, 28]
	ldrh	w0, [x29, 30]
	bl	block_erase
	str	w0, [x29, 44]
	ldr	w0, [x29, 44]
	ldp	x29, x30, [sp], 48
	ret
	.size	nand_dbg_phy_erase, .-nand_dbg_phy_erase
	.align	2
	.global	nand_dbg_single_phy_erase
	.type	nand_dbg_single_phy_erase, %function
nand_dbg_single_phy_erase:
	stp	x29, x30, [sp, -48]!
	add	x29, sp, 0
	strh	w0, [x29, 30]
	strh	w1, [x29, 28]
	ldrh	w0, [x29, 30]
	ldrh	w1, [x29, 28]
	bl	nand_physic_erase_block
	str	w0, [x29, 44]
	ldr	w0, [x29, 44]
	ldp	x29, x30, [sp], 48
	ret
	.size	nand_dbg_single_phy_erase, .-nand_dbg_single_phy_erase
	.align	2
	.global	clean_block_list
	.type	clean_block_list, %function
clean_block_list:
	stp	x29, x30, [sp, -48]!
	add	x29, sp, 0
	str	x0, [x29, 24]
.L1507:
	ldr	x0, [x29, 24]
	bl	out_phy_block_from_free_list
	str	x0, [x29, 40]
	ldr	x0, [x29, 40]
	cmp	x0, 0
	bne	.L1505
	b	.L1506
.L1505:
	ldr	x0, [x29, 40]
	strh	wzr, [x0, 4]
	ldr	x0, [x29, 40]
	strh	wzr, [x0, 2]
	ldr	x0, [x29, 40]
	mov	w1, -1
	str	w1, [x0, 8]
	ldr	x0, [x29, 40]
	str	xzr, [x0, 16]
	ldr	x0, [x29, 40]
	str	xzr, [x0, 24]
	ldr	x0, [x29, 40]
	str	xzr, [x0, 32]
	ldr	x0, [x29, 40]
	str	xzr, [x0, 40]
	ldr	x0, [x29, 40]
	str	xzr, [x0, 48]
	ldr	x0, [x29, 40]
	str	xzr, [x0, 56]
	b	.L1507
.L1506:
	ldr	x0, [x29, 24]
	bl	out_phy_block_from_invalid_page_list
	str	x0, [x29, 40]
	ldr	x0, [x29, 40]
	cmp	x0, 0
	beq	.L1512
	ldr	x0, [x29, 40]
	strh	wzr, [x0, 4]
	ldr	x0, [x29, 40]
	strh	wzr, [x0, 2]
	ldr	x0, [x29, 40]
	mov	w1, -1
	str	w1, [x0, 8]
	ldr	x0, [x29, 40]
	str	xzr, [x0, 16]
	ldr	x0, [x29, 40]
	str	xzr, [x0, 24]
	ldr	x0, [x29, 40]
	str	xzr, [x0, 32]
	ldr	x0, [x29, 40]
	str	xzr, [x0, 40]
	ldr	x0, [x29, 40]
	str	xzr, [x0, 48]
	ldr	x0, [x29, 40]
	str	xzr, [x0, 56]
	b	.L1506
.L1512:
	nop
	nop
	ldp	x29, x30, [sp], 48
	ret
	.size	clean_block_list, .-clean_block_list
	.section	.rodata
	.align	3
.LC386:
	.string	"[ND]zoneNO:%d total_pages: %d\n"
	.align	3
.LC387:
	.string	"[ND]nand_memset bytes: %d\n"
	.text
	.align	2
	.global	nand_clean_zone_table2
	.type	nand_clean_zone_table2, %function
nand_clean_zone_table2:
	stp	x29, x30, [sp, -64]!
	add	x29, sp, 0
	str	x0, [x29, 24]
	ldr	x0, [x29, 24]
	ldr	w0, [x0, 32]
	ldr	x1, [x29, 24]
	ldr	x1, [x1, 8]
	ldrb	w1, [x1, 8]
	udiv	w0, w0, w1
	str	w0, [x29, 56]
	ldr	w0, [x29, 56]
	lsl	w0, w0, 2
	str	w0, [x29, 52]
	ldr	x0, [x29, 24]
	ldrb	w0, [x0, 24]
	mov	w1, w0
	adrp	x0, .LC386
	add	x0, x0, :lo12:.LC386
	ldr	w2, [x29, 56]
	bl	nand_print_dbg
	ldr	w0, [x29, 52]
	lsr	w0, w0, 20
	str	w0, [x29, 48]
	str	wzr, [x29, 60]
	b	.L1514
.L1515:
	ldr	x1, [x29, 24]
	ldr	w0, [x29, 60]
	add	x0, x0, 8
	lsl	x0, x0, 3
	add	x0, x1, x0
	ldr	x0, [x0, 8]
	mov	w2, 1048576
	mov	w1, -1
	bl	nand_memset
	adrp	x0, .LC387
	add	x0, x0, :lo12:.LC387
	mov	w1, 1048576
	bl	nand_print_dbg
	ldr	w0, [x29, 60]
	add	w0, w0, 1
	str	w0, [x29, 60]
.L1514:
	ldr	w1, [x29, 60]
	ldr	w0, [x29, 48]
	cmp	w1, w0
	bcc	.L1515
	ldr	w0, [x29, 52]
	and	w0, w0, 1048575
	str	w0, [x29, 44]
	ldr	w0, [x29, 44]
	cmp	w0, 0
	beq	.L1516
	ldr	x1, [x29, 24]
	ldr	w0, [x29, 60]
	add	x0, x0, 8
	lsl	x0, x0, 3
	add	x0, x1, x0
	ldr	x0, [x0, 8]
	ldr	w2, [x29, 44]
	mov	w1, -1
	bl	nand_memset
	adrp	x0, .LC387
	add	x0, x0, :lo12:.LC387
	ldr	w1, [x29, 44]
	bl	nand_print_dbg
.L1516:
	ldr	x0, [x29, 24]
	str	xzr, [x0, 528]
	ldr	x0, [x29, 24]
	mov	w1, -1
	strh	w1, [x0, 584]
	ldr	x0, [x29, 24]
	str	xzr, [x0, 592]
	ldr	x0, [x29, 24]
	mov	w1, -1
	strh	w1, [x0, 648]
	ldr	x0, [x29, 24]
	str	xzr, [x0, 304]
	ldr	x0, [x29, 24]
	str	xzr, [x0, 312]
	ldr	x0, [x29, 24]
	str	xzr, [x0, 288]
	ldr	x0, [x29, 24]
	str	xzr, [x0, 296]
	ldr	x0, [x29, 24]
	str	xzr, [x0, 320]
	ldr	x0, [x29, 24]
	str	xzr, [x0, 328]
	ldr	x0, [x29, 24]
	str	xzr, [x0, 368]
	ldr	x0, [x29, 24]
	str	xzr, [x0, 376]
	ldr	x0, [x29, 24]
	str	xzr, [x0, 352]
	ldr	x0, [x29, 24]
	str	xzr, [x0, 360]
	ldr	x0, [x29, 24]
	str	xzr, [x0, 384]
	ldr	x0, [x29, 24]
	str	xzr, [x0, 392]
	ldr	x0, [x29, 24]
	str	xzr, [x0, 432]
	ldr	x0, [x29, 24]
	str	xzr, [x0, 440]
	ldr	x0, [x29, 24]
	str	xzr, [x0, 416]
	ldr	x0, [x29, 24]
	str	xzr, [x0, 424]
	ldr	x0, [x29, 24]
	str	xzr, [x0, 448]
	ldr	x0, [x29, 24]
	str	xzr, [x0, 456]
	ldr	x0, [x29, 24]
	strh	wzr, [x0, 28]
	ldr	x0, [x29, 24]
	strh	wzr, [x0, 30]
	ldr	x0, [x29, 24]
	strh	wzr, [x0, 696]
	str	wzr, [x29, 60]
	b	.L1517
.L1518:
	ldr	w0, [x29, 60]
	uxth	w2, w0
	ldr	x1, [x29, 24]
	ldr	w0, [x29, 60]
	lsl	x0, x0, 5
	add	x0, x1, x0
	add	x0, x0, 704
	mov	w1, w2
	strh	w1, [x0]
	ldr	x1, [x29, 24]
	ldr	w0, [x29, 60]
	lsl	x0, x0, 5
	add	x0, x1, x0
	add	x0, x0, 706
	strh	wzr, [x0]
	ldr	x1, [x29, 24]
	ldr	w0, [x29, 60]
	lsl	x0, x0, 5
	add	x0, x1, x0
	add	x0, x0, 712
	str	xzr, [x0]
	ldr	x1, [x29, 24]
	ldr	w0, [x29, 60]
	add	x0, x0, 22
	lsl	x0, x0, 5
	add	x0, x1, x0
	add	x0, x0, 16
	str	xzr, [x0]
	ldr	x1, [x29, 24]
	ldr	w0, [x29, 60]
	add	x0, x0, 22
	lsl	x0, x0, 5
	add	x0, x1, x0
	add	x0, x0, 24
	str	xzr, [x0]
	ldr	w0, [x29, 60]
	add	w0, w0, 1
	str	w0, [x29, 60]
.L1517:
	ldr	w0, [x29, 60]
	cmp	w0, 9
	bls	.L1518
	ldr	x0, [x29, 24]
	mov	w1, 255
	strh	w1, [x0, 1024]
	ldr	x0, [x29, 24]
	strh	wzr, [x0, 698]
	ldr	x0, [x29, 24]
	strh	wzr, [x0, 1026]
	ldr	x0, [x29, 24]
	str	xzr, [x0, 1032]
	ldr	x0, [x29, 24]
	str	xzr, [x0, 1040]
	ldr	x0, [x29, 24]
	str	xzr, [x0, 1048]
	mov	w0, 0
	ldp	x29, x30, [sp], 64
	ret
	.size	nand_clean_zone_table2, .-nand_clean_zone_table2
	.align	2
	.global	nand_clean_zone_table
	.type	nand_clean_zone_table, %function
nand_clean_zone_table:
	stp	x29, x30, [sp, -48]!
	add	x29, sp, 0
	str	x0, [x29, 24]
	ldr	x0, [x29, 24]
	str	x0, [x29, 40]
	mov	w1, 1000
	ldr	x0, [x29, 40]
	bl	__nand_flush_write_cache
	ldr	x0, [x29, 40]
	bl	clean_block_list
	ldr	x0, [x29, 40]
	bl	nand_clean_zone_table2
	ldr	x0, [x29, 40]
	bl	nftl_cache_exit
	mov	w0, 0
	ldp	x29, x30, [sp], 48
	ret
	.size	nand_clean_zone_table, .-nand_clean_zone_table
	.section	.rodata
	.align	3
.LC388:
	.string	"[ND]build_zone_list start\n"
	.align	3
.LC389:
	.string	"[ND]nftl_cache_init start\n"
	.align	3
.LC390:
	.string	"[ND]=================================2=======================================\n"
	.align	3
.LC391:
	.string	"[ND]nand_find_zone_table end\n"
	.text
	.align	2
	.global	nand_find_zone_table
	.type	nand_find_zone_table, %function
nand_find_zone_table:
	stp	x29, x30, [sp, -48]!
	add	x29, sp, 0
	str	x0, [x29, 24]
	ldr	x0, [x29, 24]
	str	x0, [x29, 40]
	ldr	x0, [x29, 40]
	bl	clean_block_list
	ldr	x0, [x29, 40]
	bl	nand_clean_zone_table2
	adrp	x0, .LC388
	add	x0, x0, :lo12:.LC388
	bl	nand_print_dbg
	ldr	x0, [x29, 40]
	bl	build_zone_list
	adrp	x0, .LC389
	add	x0, x0, :lo12:.LC389
	bl	nand_print_dbg
	ldr	x0, [x29, 40]
	bl	nftl_cache_init
	adrp	x0, .LC390
	add	x0, x0, :lo12:.LC390
	bl	nand_print_dbg
	ldr	x0, [x29, 40]
	bl	print_block_invalid_list
	ldr	x0, [x29, 40]
	bl	print_free_list
	adrp	x0, .LC390
	add	x0, x0, :lo12:.LC390
	bl	nand_print_dbg
	adrp	x0, .LC391
	add	x0, x0, :lo12:.LC391
	bl	nand_print_dbg
	mov	w0, 0
	ldp	x29, x30, [sp], 48
	ret
	.size	nand_find_zone_table, .-nand_find_zone_table
	.section	.rodata
	.align	3
.LC392:
	.string	"[NE][NE]%s:malloc 512 fail!\n"
	.text
	.align	2
	.global	write_data_to_nand
	.type	write_data_to_nand, %function
write_data_to_nand:
	stp	x29, x30, [sp, -48]!
	add	x29, sp, 0
	str	x0, [x29, 24]
	mov	w0, 512
	bl	nftl_malloc
	str	x0, [x29, 32]
	ldr	x0, [x29, 32]
	cmp	x0, 0
	bne	.L1525
	adrp	x0, __func__.7642
	add	x1, x0, :lo12:__func__.7642
	adrp	x0, .LC392
	add	x0, x0, :lo12:.LC392
	bl	nand_print
.L1525:
	mov	w2, 512
	mov	w1, -86
	ldr	x0, [x29, 32]
	bl	nand_memset
	str	wzr, [x29, 44]
	b	.L1526
.L1527:
	ldr	w0, [x29, 44]
	ldr	x3, [x29, 32]
	mov	w2, 1
	mov	w1, w0
	ldr	x0, [x29, 24]
	bl	__nand_write
	ldr	w0, [x29, 44]
	add	w0, w0, 1
	str	w0, [x29, 44]
.L1526:
	ldr	w1, [x29, 44]
	mov	w0, 38911
	cmp	w1, w0
	ble	.L1527
	ldr	x0, [x29, 32]
	bl	nftl_free
	mov	w0, 0
	ldp	x29, x30, [sp], 48
	ret
	.size	write_data_to_nand, .-write_data_to_nand
	.section	.rodata
	.align	3
.LC393:
	.string	"[ND]==========================\n"
	.text
	.align	2
	.global	nand_check_table
	.type	nand_check_table, %function
nand_check_table:
	stp	x29, x30, [sp, -48]!
	add	x29, sp, 0
	str	x0, [x29, 24]
	ldr	x0, [x29, 24]
	str	x0, [x29, 40]
	mov	w1, 1000
	ldr	x0, [x29, 40]
	bl	__nand_flush_write_cache
	adrp	x0, .LC393
	add	x0, x0, :lo12:.LC393
	bl	nand_print_dbg
	ldr	x0, [x29, 40]
	bl	print_nftl_zone
	adrp	x0, .LC393
	add	x0, x0, :lo12:.LC393
	bl	nand_print_dbg
	ldr	x0, [x29, 40]
	bl	nand_clean_zone_table
	adrp	x0, .LC393
	add	x0, x0, :lo12:.LC393
	bl	nand_print_dbg
	ldr	x0, [x29, 40]
	bl	nand_find_zone_table
	ldr	x0, [x29, 40]
	bl	write_data_to_nand
	adrp	x0, .LC393
	add	x0, x0, :lo12:.LC393
	bl	nand_print_dbg
	mov	w0, 0
	ldp	x29, x30, [sp], 48
	ret
	.size	nand_check_table, .-nand_check_table
	.section	.rodata
	.align	3
.LC394:
	.string	"%.2x "
	.text
	.align	2
	.type	_DumpHexData, %function
_DumpHexData:
	stp	x29, x30, [sp, -48]!
	add	x29, sp, 0
	str	x0, [x29, 24]
	str	w1, [x29, 20]
	ldr	x0, [x29, 24]
	str	x0, [x29, 40]
	str	wzr, [x29, 36]
	b	.L1532
.L1534:
	ldr	x0, [x29, 40]
	add	x1, x0, 1
	str	x1, [x29, 40]
	ldrb	w0, [x0]
	mov	w1, w0
	adrp	x0, .LC394
	add	x0, x0, :lo12:.LC394
	bl	nand_print
	ldr	w0, [x29, 36]
	negs	w1, w0
	and	w0, w0, 15
	and	w1, w1, 15
	csneg	w0, w0, w1, mi
	cmp	w0, 15
	bne	.L1533
	adrp	x0, .LC45
	add	x0, x0, :lo12:.LC45
	bl	nand_print
.L1533:
	ldr	w0, [x29, 36]
	add	w0, w0, 1
	str	w0, [x29, 36]
.L1532:
	ldr	w1, [x29, 36]
	ldr	w0, [x29, 20]
	cmp	w1, w0
	blt	.L1534
	adrp	x0, .LC45
	add	x0, x0, :lo12:.LC45
	bl	nand_print
	nop
	ldp	x29, x30, [sp], 48
	ret
	.size	_DumpHexData, .-_DumpHexData
	.section	.rodata
	.align	3
.LC395:
	.string	"[NE]exception case bad block\n"
	.align	3
.LC396:
	.string	"[NE]exception case wbc\n"
	.align	3
.LC397:
	.string	"[NE]exception case gd\n"
	.align	3
.LC398:
	.string	"[NE]%s %d, flag = %d\n"
	.align	3
.LC399:
	.string	"[NE]OOB :"
	.text
	.align	2
	.global	nand_power_off_ecc_error
	.type	nand_power_off_ecc_error, %function
nand_power_off_ecc_error:
	stp	x29, x30, [sp, -64]!
	add	x29, sp, 0
	str	x0, [x29, 40]
	str	x1, [x29, 32]
	str	x2, [x29, 24]
	str	wzr, [x29, 60]
	ldr	x0, [x29, 24]
	ldrb	w0, [x0]
	cmp	w0, 255
	beq	.L1537
	adrp	x0, .LC395
	add	x0, x0, :lo12:.LC395
	bl	nand_print
	mov	w0, 1
	str	w0, [x29, 60]
	b	.L1538
.L1537:
	ldr	x0, [x29, 40]
	ldr	x0, [x0, 3448]
	ldrh	w0, [x0, 28]
	cmp	w0, 0
	beq	.L1539
	ldr	x2, [x29, 24]
	ldr	x1, [x29, 32]
	ldr	x0, [x29, 40]
	bl	crc_wrong
	cmp	w0, 0
	beq	.L1538
	adrp	x0, .LC396
	add	x0, x0, :lo12:.LC396
	bl	nand_print
	mov	w0, 1
	str	w0, [x29, 60]
	b	.L1538
.L1539:
	ldr	x0, [x29, 24]
	add	x0, x0, 11
	ldrb	w0, [x0]
	cmp	w0, 165
	bne	.L1540
	ldr	x0, [x29, 24]
	add	x0, x0, 15
	ldrb	w0, [x0]
	cmp	w0, 165
	beq	.L1538
.L1540:
	adrp	x0, .LC397
	add	x0, x0, :lo12:.LC397
	bl	nand_print
	mov	w0, 1
	str	w0, [x29, 60]
	nop
.L1538:
	ldr	w0, [x29, 60]
	cmp	w0, 1
	bne	.L1541
	adrp	x0, __FUNCTION__.7666
	add	x1, x0, :lo12:__FUNCTION__.7666
	adrp	x0, .LC398
	add	x0, x0, :lo12:.LC398
	ldr	w3, [x29, 60]
	mov	w2, 3046
	bl	nand_print
	adrp	x0, .LC399
	add	x0, x0, :lo12:.LC399
	bl	nand_print
	mov	w1, 16
	ldr	x0, [x29, 24]
	bl	_DumpHexData
.L1541:
	ldr	w0, [x29, 60]
	ldp	x29, x30, [sp], 64
	ret
	.size	nand_power_off_ecc_error, .-nand_power_off_ecc_error
	.align	2
	.global	add_free_list
	.type	add_free_list, %function
add_free_list:
	sub	sp, sp, #32
	str	x0, [sp, 8]
	str	x1, [sp]
	ldr	x0, [sp, 8]
	add	x0, x0, 272
	str	x0, [sp, 24]
	b	.L1544
.L1547:
	ldr	x0, [sp, 24]
	ldr	x0, [x0, 32]
	ldrh	w1, [x0, 6]
	ldr	x0, [sp]
	ldrh	w0, [x0, 6]
	cmp	w1, w0
	bcc	.L1545
	ldr	x0, [sp, 24]
	ldr	x0, [x0, 32]
	ldr	x1, [sp]
	str	x1, [x0, 40]
	ldr	x0, [sp, 24]
	ldr	x1, [x0, 32]
	ldr	x0, [sp]
	str	x1, [x0, 32]
	ldr	x0, [sp, 24]
	ldr	x1, [sp]
	str	x1, [x0, 32]
	ldr	x0, [sp]
	ldr	x1, [sp, 24]
	str	x1, [x0, 40]
	mov	w0, 0
	b	.L1546
.L1545:
	ldr	x0, [sp, 24]
	ldr	x0, [x0, 32]
	str	x0, [sp, 24]
.L1544:
	ldr	x0, [sp, 24]
	ldr	x0, [x0, 32]
	cmp	x0, 0
	bne	.L1547
	ldr	x0, [sp, 24]
	ldr	x1, [sp]
	str	x1, [x0, 32]
	ldr	x0, [sp]
	str	xzr, [x0, 32]
	ldr	x0, [sp]
	ldr	x1, [sp, 24]
	str	x1, [x0, 40]
	mov	w0, 0
.L1546:
	add	sp, sp, 32
	ret
	.size	add_free_list, .-add_free_list
	.align	2
	.global	del_free_list
	.type	del_free_list, %function
del_free_list:
	sub	sp, sp, #32
	str	x0, [sp, 8]
	ldr	x0, [sp, 8]
	add	x0, x0, 272
	str	x0, [sp, 24]
	ldr	x0, [sp, 24]
	ldr	x0, [x0, 32]
	str	x0, [sp, 24]
	ldr	x0, [sp, 24]
	cmp	x0, 0
	beq	.L1549
	ldr	x0, [sp, 24]
	ldr	x1, [x0, 32]
	ldr	x0, [sp, 8]
	str	x1, [x0, 304]
	ldr	x0, [sp, 24]
	ldr	x1, [x0, 32]
	ldr	x0, [sp, 8]
	str	x1, [x0, 304]
	ldr	x0, [sp, 24]
	ldr	x0, [x0, 32]
	cmp	x0, 0
	beq	.L1549
	ldr	x0, [sp, 24]
	ldr	x0, [x0, 32]
	ldr	x1, [sp, 8]
	add	x1, x1, 272
	str	x1, [x0, 40]
.L1549:
	ldr	x0, [sp, 24]
	add	sp, sp, 32
	ret
	.size	del_free_list, .-del_free_list
	.align	2
	.global	del_free_list_by_block
	.type	del_free_list_by_block, %function
del_free_list_by_block:
	sub	sp, sp, #32
	str	x0, [sp, 8]
	str	x1, [sp]
	ldr	x0, [sp, 8]
	add	x0, x0, 272
	str	x0, [sp, 24]
	ldr	x0, [sp, 24]
	ldr	x0, [x0, 32]
	str	x0, [sp, 24]
	b	.L1552
.L1556:
	ldr	x1, [sp, 24]
	ldr	x0, [sp]
	cmp	x1, x0
	bne	.L1553
	ldr	x0, [sp, 24]
	ldr	x0, [x0, 40]
	ldr	x1, [sp, 24]
	ldr	x1, [x1, 32]
	str	x1, [x0, 32]
	ldr	x0, [sp, 24]
	ldr	x0, [x0, 32]
	cmp	x0, 0
	beq	.L1554
	ldr	x0, [sp, 24]
	ldr	x0, [x0, 32]
	ldr	x1, [sp, 24]
	ldr	x1, [x1, 40]
	str	x1, [x0, 40]
.L1554:
	ldr	x0, [sp, 24]
	str	xzr, [x0, 40]
	ldr	x0, [sp, 24]
	str	xzr, [x0, 32]
	b	.L1555
.L1553:
	ldr	x0, [sp, 24]
	ldr	x0, [x0, 32]
	str	x0, [sp, 24]
.L1552:
	ldr	x0, [sp, 24]
	cmp	x0, 0
	bne	.L1556
.L1555:
	ldr	x0, [sp, 24]
	add	sp, sp, 32
	ret
	.size	del_free_list_by_block, .-del_free_list_by_block
	.align	2
	.global	is_block_in_free_list
	.type	is_block_in_free_list, %function
is_block_in_free_list:
	sub	sp, sp, #32
	str	x0, [sp, 8]
	str	x1, [sp]
	ldr	x0, [sp, 8]
	add	x0, x0, 272
	str	x0, [sp, 24]
	ldr	x0, [sp, 24]
	ldr	x0, [x0, 32]
	str	x0, [sp, 24]
	b	.L1559
.L1562:
	ldr	x1, [sp, 24]
	ldr	x0, [sp]
	cmp	x1, x0
	bne	.L1560
	mov	w0, 1
	b	.L1561
.L1560:
	ldr	x0, [sp, 24]
	ldr	x0, [x0, 32]
	str	x0, [sp, 24]
.L1559:
	ldr	x0, [sp, 24]
	cmp	x0, 0
	bne	.L1562
	mov	w0, 0
.L1561:
	add	sp, sp, 32
	ret
	.size	is_block_in_free_list, .-is_block_in_free_list
	.section	.rodata
	.align	3
.LC400:
	.string	"[ND]free list: \n"
	.align	3
.LC401:
	.string	"[ND]block NO:%4d; erase:%3d; "
	.align	3
.LC402:
	.string	"[ND]count:%4d; "
	.align	3
.LC403:
	.string	"[ND]info:0x%x\n"
	.text
	.align	2
	.global	print_free_list
	.type	print_free_list, %function
print_free_list:
	stp	x29, x30, [sp, -48]!
	add	x29, sp, 0
	str	x0, [x29, 24]
	ldr	x0, [x29, 24]
	add	x0, x0, 272
	str	x0, [x29, 40]
	ldr	x0, [x29, 24]
	ldr	w0, [x0, 656]
	cmp	w0, 0
	beq	.L1568
	adrp	x0, .LC400
	add	x0, x0, :lo12:.LC400
	bl	nand_print_dbg
	ldr	x0, [x29, 40]
	ldr	x0, [x0, 32]
	str	x0, [x29, 40]
	b	.L1566
.L1567:
	ldr	x0, [x29, 40]
	ldrh	w0, [x0]
	mov	w1, w0
	ldr	x0, [x29, 40]
	ldrh	w0, [x0, 6]
	mov	w2, w0
	adrp	x0, .LC401
	add	x0, x0, :lo12:.LC401
	bl	nand_print_dbg
	ldr	x0, [x29, 40]
	ldr	w1, [x0, 8]
	adrp	x0, .LC402
	add	x0, x0, :lo12:.LC402
	bl	nand_print_dbg
	ldr	x0, [x29, 40]
	ldrh	w0, [x0, 2]
	mov	w1, w0
	adrp	x0, .LC403
	add	x0, x0, :lo12:.LC403
	bl	nand_print_dbg
	ldr	x0, [x29, 40]
	ldr	x0, [x0, 32]
	str	x0, [x29, 40]
.L1566:
	ldr	x0, [x29, 40]
	cmp	x0, 0
	bne	.L1567
	b	.L1563
.L1568:
	nop
.L1563:
	ldp	x29, x30, [sp], 48
	ret
	.size	print_free_list, .-print_free_list
	.align	2
	.global	add_block_invalid_list
	.type	add_block_invalid_list, %function
add_block_invalid_list:
	sub	sp, sp, #32
	str	x0, [sp, 8]
	str	x1, [sp]
	ldr	x0, [sp, 8]
	add	x0, x0, 336
	str	x0, [sp, 24]
	b	.L1570
.L1573:
	ldr	x0, [sp, 24]
	ldr	x0, [x0, 16]
	ldrh	w1, [x0, 4]
	ldr	x0, [sp]
	ldrh	w0, [x0, 4]
	cmp	w1, w0
	bhi	.L1571
	ldr	x0, [sp, 24]
	ldr	x0, [x0, 16]
	ldr	x1, [sp]
	str	x1, [x0, 24]
	ldr	x0, [sp, 24]
	ldr	x1, [x0, 16]
	ldr	x0, [sp]
	str	x1, [x0, 16]
	ldr	x0, [sp, 24]
	ldr	x1, [sp]
	str	x1, [x0, 16]
	ldr	x0, [sp]
	ldr	x1, [sp, 24]
	str	x1, [x0, 24]
	mov	w0, 0
	b	.L1572
.L1571:
	ldr	x0, [sp, 24]
	ldr	x0, [x0, 16]
	str	x0, [sp, 24]
.L1570:
	ldr	x0, [sp, 24]
	ldr	x0, [x0, 16]
	cmp	x0, 0
	bne	.L1573
	ldr	x0, [sp, 24]
	ldr	x1, [sp]
	str	x1, [x0, 16]
	ldr	x0, [sp]
	str	xzr, [x0, 16]
	ldr	x0, [sp]
	ldr	x1, [sp, 24]
	str	x1, [x0, 24]
	mov	w0, 0
.L1572:
	add	sp, sp, 32
	ret
	.size	add_block_invalid_list, .-add_block_invalid_list
	.align	2
	.global	del_block_invalid_list
	.type	del_block_invalid_list, %function
del_block_invalid_list:
	sub	sp, sp, #32
	str	x0, [sp, 8]
	ldr	x0, [sp, 8]
	add	x0, x0, 336
	str	x0, [sp, 24]
	ldr	x0, [sp, 24]
	ldr	x0, [x0, 16]
	str	x0, [sp, 24]
	ldr	x0, [sp, 24]
	cmp	x0, 0
	beq	.L1575
	ldr	x0, [sp, 24]
	ldr	x1, [x0, 16]
	ldr	x0, [sp, 8]
	str	x1, [x0, 352]
	ldr	x0, [sp, 24]
	ldr	x0, [x0, 16]
	cmp	x0, 0
	beq	.L1575
	ldr	x0, [sp, 24]
	ldr	x0, [x0, 16]
	ldr	x1, [sp, 8]
	add	x1, x1, 336
	str	x1, [x0, 24]
.L1575:
	ldr	x0, [sp, 24]
	add	sp, sp, 32
	ret
	.size	del_block_invalid_list, .-del_block_invalid_list
	.align	2
	.global	del_block_invalid_list_by_block
	.type	del_block_invalid_list_by_block, %function
del_block_invalid_list_by_block:
	sub	sp, sp, #32
	str	x0, [sp, 8]
	str	x1, [sp]
	ldr	x0, [sp, 8]
	add	x0, x0, 336
	str	x0, [sp, 24]
	ldr	x0, [sp, 24]
	ldr	x0, [x0, 16]
	str	x0, [sp, 24]
	b	.L1578
.L1582:
	ldr	x1, [sp, 24]
	ldr	x0, [sp]
	cmp	x1, x0
	bne	.L1579
	ldr	x0, [sp, 24]
	ldr	x0, [x0, 24]
	ldr	x1, [sp, 24]
	ldr	x1, [x1, 16]
	str	x1, [x0, 16]
	ldr	x0, [sp, 24]
	ldr	x0, [x0, 16]
	cmp	x0, 0
	beq	.L1580
	ldr	x0, [sp, 24]
	ldr	x0, [x0, 16]
	ldr	x1, [sp, 24]
	ldr	x1, [x1, 24]
	str	x1, [x0, 24]
.L1580:
	ldr	x0, [sp, 24]
	str	xzr, [x0, 24]
	ldr	x0, [sp, 24]
	str	xzr, [x0, 16]
	b	.L1581
.L1579:
	ldr	x0, [sp, 24]
	ldr	x0, [x0, 16]
	str	x0, [sp, 24]
.L1578:
	ldr	x0, [sp, 24]
	cmp	x0, 0
	bne	.L1582
.L1581:
	ldr	x0, [sp, 24]
	add	sp, sp, 32
	ret
	.size	del_block_invalid_list_by_block, .-del_block_invalid_list_by_block
	.align	2
	.global	is_block_in_invalid_list
	.type	is_block_in_invalid_list, %function
is_block_in_invalid_list:
	sub	sp, sp, #32
	str	x0, [sp, 8]
	str	x1, [sp]
	ldr	x0, [sp, 8]
	add	x0, x0, 336
	str	x0, [sp, 24]
	ldr	x0, [sp, 24]
	ldr	x0, [x0, 16]
	str	x0, [sp, 24]
	b	.L1585
.L1588:
	ldr	x1, [sp, 24]
	ldr	x0, [sp]
	cmp	x1, x0
	bne	.L1586
	mov	w0, 1
	b	.L1587
.L1586:
	ldr	x0, [sp, 24]
	ldr	x0, [x0, 16]
	str	x0, [sp, 24]
.L1585:
	ldr	x0, [sp, 24]
	cmp	x0, 0
	bne	.L1588
	mov	w0, 0
.L1587:
	add	sp, sp, 32
	ret
	.size	is_block_in_invalid_list, .-is_block_in_invalid_list
	.section	.rodata
	.align	3
.LC404:
	.string	"[ND]invalid_page_count list: \n"
	.align	3
.LC405:
	.string	"[ND]count:%4d; invalid:%3d;"
	.text
	.align	2
	.global	print_block_invalid_list
	.type	print_block_invalid_list, %function
print_block_invalid_list:
	stp	x29, x30, [sp, -48]!
	add	x29, sp, 0
	str	x0, [x29, 24]
	ldr	x0, [x29, 24]
	add	x0, x0, 336
	str	x0, [x29, 40]
	ldr	x0, [x29, 24]
	ldr	w0, [x0, 656]
	cmp	w0, 0
	beq	.L1594
	adrp	x0, .LC404
	add	x0, x0, :lo12:.LC404
	bl	nand_print_dbg
	ldr	x0, [x29, 40]
	ldr	x0, [x0, 16]
	str	x0, [x29, 40]
	b	.L1592
.L1593:
	ldr	x0, [x29, 40]
	ldrh	w0, [x0]
	mov	w1, w0
	ldr	x0, [x29, 40]
	ldrh	w0, [x0, 6]
	mov	w2, w0
	adrp	x0, .LC401
	add	x0, x0, :lo12:.LC401
	bl	nand_print_dbg
	ldr	x0, [x29, 40]
	ldr	w1, [x0, 8]
	ldr	x0, [x29, 40]
	ldrh	w0, [x0, 4]
	mov	w2, w0
	adrp	x0, .LC405
	add	x0, x0, :lo12:.LC405
	bl	nand_print_dbg
	ldr	x0, [x29, 40]
	ldrh	w0, [x0, 2]
	mov	w1, w0
	adrp	x0, .LC403
	add	x0, x0, :lo12:.LC403
	bl	nand_print_dbg
	ldr	x0, [x29, 40]
	ldr	x0, [x0, 16]
	str	x0, [x29, 40]
.L1592:
	ldr	x0, [x29, 40]
	cmp	x0, 0
	bne	.L1593
	b	.L1589
.L1594:
	nop
.L1589:
	ldp	x29, x30, [sp], 48
	ret
	.size	print_block_invalid_list, .-print_block_invalid_list
	.align	2
	.global	add_block_count_list
	.type	add_block_count_list, %function
add_block_count_list:
	sub	sp, sp, #32
	str	x0, [sp, 8]
	str	x1, [sp]
	ldr	x0, [sp, 8]
	add	x0, x0, 400
	str	x0, [sp, 24]
	b	.L1596
.L1599:
	ldr	x0, [sp, 24]
	ldr	x0, [x0, 48]
	ldr	w1, [x0, 8]
	ldr	x0, [sp]
	ldr	w0, [x0, 8]
	cmp	w1, w0
	blt	.L1597
	ldr	x0, [sp, 24]
	ldr	x0, [x0, 48]
	ldr	x1, [sp]
	str	x1, [x0, 56]
	ldr	x0, [sp, 24]
	ldr	x1, [x0, 48]
	ldr	x0, [sp]
	str	x1, [x0, 48]
	ldr	x0, [sp, 24]
	ldr	x1, [sp]
	str	x1, [x0, 48]
	ldr	x0, [sp]
	ldr	x1, [sp, 24]
	str	x1, [x0, 56]
	mov	w0, 0
	b	.L1598
.L1597:
	ldr	x0, [sp, 24]
	ldr	x0, [x0, 48]
	str	x0, [sp, 24]
.L1596:
	ldr	x0, [sp, 24]
	ldr	x0, [x0, 48]
	cmp	x0, 0
	bne	.L1599
	ldr	x0, [sp, 24]
	ldr	x1, [sp]
	str	x1, [x0, 48]
	ldr	x0, [sp]
	str	xzr, [x0, 48]
	ldr	x0, [sp]
	ldr	x1, [sp, 24]
	str	x1, [x0, 56]
	mov	w0, 0
.L1598:
	add	sp, sp, 32
	ret
	.size	add_block_count_list, .-add_block_count_list
	.align	2
	.global	del_block_count_list
	.type	del_block_count_list, %function
del_block_count_list:
	sub	sp, sp, #32
	str	x0, [sp, 8]
	ldr	x0, [sp, 8]
	add	x0, x0, 400
	str	x0, [sp, 24]
	ldr	x0, [sp, 24]
	ldr	x0, [x0, 48]
	cmp	x0, 0
	beq	.L1601
	ldr	x0, [sp, 24]
	ldr	x0, [x0, 48]
	ldr	x1, [x0, 48]
	ldr	x0, [sp, 24]
	str	x1, [x0, 48]
.L1601:
	ldr	x0, [sp, 24]
	add	sp, sp, 32
	ret
	.size	del_block_count_list, .-del_block_count_list
	.align	2
	.global	del_block_count_by_block
	.type	del_block_count_by_block, %function
del_block_count_by_block:
	sub	sp, sp, #32
	str	x0, [sp, 8]
	str	x1, [sp]
	ldr	x0, [sp, 8]
	add	x0, x0, 400
	str	x0, [sp, 24]
	ldr	x0, [sp, 24]
	ldr	x0, [x0, 48]
	str	x0, [sp, 24]
	b	.L1604
.L1608:
	ldr	x1, [sp, 24]
	ldr	x0, [sp]
	cmp	x1, x0
	bne	.L1605
	ldr	x0, [sp, 24]
	ldr	x0, [x0, 56]
	ldr	x1, [sp, 24]
	ldr	x1, [x1, 48]
	str	x1, [x0, 48]
	ldr	x0, [sp, 24]
	ldr	x0, [x0, 48]
	cmp	x0, 0
	beq	.L1606
	ldr	x0, [sp, 24]
	ldr	x0, [x0, 48]
	ldr	x1, [sp, 24]
	ldr	x1, [x1, 56]
	str	x1, [x0, 56]
.L1606:
	ldr	x0, [sp, 24]
	str	xzr, [x0, 56]
	ldr	x0, [sp, 24]
	str	xzr, [x0, 48]
	b	.L1607
.L1605:
	ldr	x0, [sp, 24]
	ldr	x0, [x0, 48]
	str	x0, [sp, 24]
.L1604:
	ldr	x0, [sp, 24]
	cmp	x0, 0
	bne	.L1608
.L1607:
	ldr	x0, [sp, 24]
	add	sp, sp, 32
	ret
	.size	del_block_count_by_block, .-del_block_count_by_block
	.section	.rodata
	.align	3
.LC406:
	.string	"[ND]block_used_count list: \n"
	.align	3
.LC407:
	.string	"[ND]block NO: %d "
	.align	3
.LC408:
	.string	"[ND]block_used_count: %d \n"
	.text
	.align	2
	.global	print_block_count_list
	.type	print_block_count_list, %function
print_block_count_list:
	stp	x29, x30, [sp, -48]!
	add	x29, sp, 0
	str	x0, [x29, 24]
	ldr	x0, [x29, 24]
	add	x0, x0, 400
	str	x0, [x29, 40]
	ldr	x0, [x29, 24]
	ldr	w0, [x0, 656]
	cmp	w0, 0
	beq	.L1615
	adrp	x0, .LC406
	add	x0, x0, :lo12:.LC406
	bl	nand_print_dbg
	ldr	x0, [x29, 40]
	ldr	x0, [x0, 48]
	str	x0, [x29, 40]
	b	.L1613
.L1614:
	ldr	x0, [x29, 40]
	ldrh	w0, [x0]
	mov	w1, w0
	adrp	x0, .LC407
	add	x0, x0, :lo12:.LC407
	bl	nand_print_dbg
	ldr	x0, [x29, 40]
	ldr	w1, [x0, 8]
	adrp	x0, .LC408
	add	x0, x0, :lo12:.LC408
	bl	nand_print_dbg
	ldr	x0, [x29, 40]
	ldr	x0, [x0, 48]
	str	x0, [x29, 40]
.L1613:
	ldr	x0, [x29, 40]
	cmp	x0, 0
	bne	.L1614
	b	.L1610
.L1615:
	nop
.L1610:
	ldp	x29, x30, [sp], 48
	ret
	.size	print_block_count_list, .-print_block_count_list
	.section	.rodata
	.align	3
.LC409:
	.string	"[ND]free_list error !\n"
	.text
	.align	2
	.global	put_phy_block_to_free_list
	.type	put_phy_block_to_free_list, %function
put_phy_block_to_free_list:
	stp	x29, x30, [sp, -48]!
	add	x29, sp, 0
	str	x0, [x29, 24]
	str	x1, [x29, 16]
	ldr	x0, [x29, 16]
	str	xzr, [x0, 32]
	ldr	x0, [x29, 16]
	str	xzr, [x0, 40]
	ldr	x1, [x29, 16]
	ldr	x0, [x29, 24]
	bl	add_free_list
	str	w0, [x29, 44]
	ldr	w0, [x29, 44]
	cmp	w0, 0
	bne	.L1617
	ldr	x0, [x29, 24]
	ldrh	w0, [x0, 30]
	add	w0, w0, 1
	uxth	w1, w0
	ldr	x0, [x29, 24]
	strh	w1, [x0, 30]
	b	.L1618
.L1617:
	adrp	x0, .LC409
	add	x0, x0, :lo12:.LC409
	bl	nand_print_dbg
.L1618:
	ldr	w0, [x29, 44]
	ldp	x29, x30, [sp], 48
	ret
	.size	put_phy_block_to_free_list, .-put_phy_block_to_free_list
	.align	2
	.global	out_phy_block_from_free_list
	.type	out_phy_block_from_free_list, %function
out_phy_block_from_free_list:
	stp	x29, x30, [sp, -48]!
	add	x29, sp, 0
	str	x0, [x29, 24]
	ldr	x0, [x29, 24]
	bl	del_free_list
	str	x0, [x29, 40]
	ldr	x0, [x29, 40]
	cmp	x0, 0
	beq	.L1621
	ldr	x0, [x29, 40]
	str	xzr, [x0, 32]
	ldr	x0, [x29, 40]
	str	xzr, [x0, 40]
	ldr	x0, [x29, 24]
	ldrh	w0, [x0, 30]
	cmp	w0, 0
	beq	.L1621
	ldr	x0, [x29, 24]
	ldrh	w0, [x0, 30]
	sub	w0, w0, #1
	uxth	w1, w0
	ldr	x0, [x29, 24]
	strh	w1, [x0, 30]
.L1621:
	ldr	x0, [x29, 40]
	ldp	x29, x30, [sp], 48
	ret
	.size	out_phy_block_from_free_list, .-out_phy_block_from_free_list
	.align	2
	.global	out_phy_block_from_free_list_by_erase_time
	.type	out_phy_block_from_free_list_by_erase_time, %function
out_phy_block_from_free_list_by_erase_time:
	stp	x29, x30, [sp, -64]!
	add	x29, sp, 0
	str	x0, [x29, 24]
	strh	w1, [x29, 22]
	str	xzr, [x29, 56]
	strh	wzr, [x29, 54]
	ldr	x0, [x29, 24]
	add	x0, x0, 272
	str	x0, [x29, 40]
	ldr	x0, [x29, 40]
	ldr	x0, [x0, 32]
	str	x0, [x29, 40]
	b	.L1624
.L1626:
	ldr	x0, [x29, 40]
	ldrh	w0, [x0, 6]
	ldrh	w1, [x29, 22]
	cmp	w1, w0
	bls	.L1625
	ldr	x0, [x29, 40]
	ldrh	w0, [x0, 6]
	ldrh	w1, [x29, 54]
	cmp	w1, w0
	bhi	.L1625
	ldr	x0, [x29, 40]
	ldrh	w0, [x0, 6]
	strh	w0, [x29, 54]
	ldr	x0, [x29, 40]
	str	x0, [x29, 56]
.L1625:
	ldr	x0, [x29, 40]
	ldr	x0, [x0, 32]
	str	x0, [x29, 40]
.L1624:
	ldr	x0, [x29, 40]
	cmp	x0, 0
	bne	.L1626
	ldr	x0, [x29, 56]
	cmp	x0, 0
	beq	.L1627
	ldr	x1, [x29, 56]
	ldr	x0, [x29, 24]
	bl	out_phy_block_from_free_list_by_block
	str	x0, [x29, 56]
.L1627:
	ldr	x0, [x29, 56]
	ldp	x29, x30, [sp], 64
	ret
	.size	out_phy_block_from_free_list_by_erase_time, .-out_phy_block_from_free_list_by_erase_time
	.align	2
	.global	get_max_erase_time_from_free_list
	.type	get_max_erase_time_from_free_list, %function
get_max_erase_time_from_free_list:
	sub	sp, sp, #48
	str	x0, [sp, 8]
	strh	w1, [sp, 6]
	str	xzr, [sp, 40]
	strh	wzr, [sp, 38]
	ldr	x0, [sp, 8]
	add	x0, x0, 272
	str	x0, [sp, 24]
	ldr	x0, [sp, 24]
	ldr	x0, [x0, 32]
	str	x0, [sp, 24]
	b	.L1630
.L1632:
	ldr	x0, [sp, 24]
	ldrh	w0, [x0, 6]
	ldrh	w1, [sp, 6]
	cmp	w1, w0
	bls	.L1631
	ldr	x0, [sp, 24]
	ldrh	w0, [x0, 6]
	ldrh	w1, [sp, 38]
	cmp	w1, w0
	bhi	.L1631
	ldr	x0, [sp, 24]
	ldrh	w0, [x0, 6]
	strh	w0, [sp, 38]
	ldr	x0, [sp, 24]
	str	x0, [sp, 40]
.L1631:
	ldr	x0, [sp, 24]
	ldr	x0, [x0, 32]
	str	x0, [sp, 24]
.L1630:
	ldr	x0, [sp, 24]
	cmp	x0, 0
	bne	.L1632
	ldr	x0, [sp, 40]
	cmp	x0, 0
	beq	.L1633
	ldr	x0, [sp, 40]
	ldrh	w0, [x0, 6]
	b	.L1634
.L1633:
	mov	w0, 65535
.L1634:
	add	sp, sp, 48
	ret
	.size	get_max_erase_time_from_free_list, .-get_max_erase_time_from_free_list
	.align	2
	.global	get_block_max_erase_time_from_free_list
	.type	get_block_max_erase_time_from_free_list, %function
get_block_max_erase_time_from_free_list:
	sub	sp, sp, #48
	str	x0, [sp, 8]
	strh	w1, [sp, 6]
	str	xzr, [sp, 40]
	strh	wzr, [sp, 38]
	ldr	x0, [sp, 8]
	add	x0, x0, 272
	str	x0, [sp, 24]
	ldr	x0, [sp, 24]
	ldr	x0, [x0, 32]
	str	x0, [sp, 24]
	b	.L1636
.L1638:
	ldr	x0, [sp, 24]
	ldrh	w0, [x0, 6]
	ldrh	w1, [sp, 6]
	cmp	w1, w0
	bls	.L1637
	ldr	x0, [sp, 24]
	ldrh	w0, [x0, 6]
	ldrh	w1, [sp, 38]
	cmp	w1, w0
	bhi	.L1637
	ldr	x0, [sp, 24]
	ldrh	w0, [x0, 6]
	strh	w0, [sp, 38]
	ldr	x0, [sp, 24]
	str	x0, [sp, 40]
.L1637:
	ldr	x0, [sp, 24]
	ldr	x0, [x0, 32]
	str	x0, [sp, 24]
.L1636:
	ldr	x0, [sp, 24]
	cmp	x0, 0
	bne	.L1638
	ldr	x0, [sp, 40]
	cmp	x0, 0
	beq	.L1639
	ldr	x0, [sp, 40]
	b	.L1640
.L1639:
	mov	x0, 0
.L1640:
	add	sp, sp, 48
	ret
	.size	get_block_max_erase_time_from_free_list, .-get_block_max_erase_time_from_free_list
	.align	2
	.global	get_block_max_erase_time_from_invalid_list
	.type	get_block_max_erase_time_from_invalid_list, %function
get_block_max_erase_time_from_invalid_list:
	sub	sp, sp, #48
	str	x0, [sp, 8]
	strh	w1, [sp, 6]
	str	xzr, [sp, 40]
	strh	wzr, [sp, 38]
	ldr	x0, [sp, 8]
	add	x0, x0, 336
	str	x0, [sp, 24]
	ldr	x0, [sp, 24]
	ldr	x0, [x0, 16]
	str	x0, [sp, 24]
	b	.L1642
.L1644:
	ldr	x0, [sp, 24]
	ldrh	w0, [x0, 6]
	ldrh	w1, [sp, 6]
	cmp	w1, w0
	bls	.L1643
	ldr	x0, [sp, 24]
	ldrh	w0, [x0, 6]
	ldrh	w1, [sp, 38]
	cmp	w1, w0
	bhi	.L1643
	ldr	x0, [sp, 24]
	ldrh	w0, [x0, 6]
	strh	w0, [sp, 38]
	ldr	x0, [sp, 24]
	str	x0, [sp, 40]
.L1643:
	ldr	x0, [sp, 24]
	ldr	x0, [x0, 16]
	str	x0, [sp, 24]
.L1642:
	ldr	x0, [sp, 24]
	cmp	x0, 0
	bne	.L1644
	ldr	x0, [sp, 40]
	cmp	x0, 0
	beq	.L1645
	ldr	x0, [sp, 40]
	b	.L1646
.L1645:
	mov	x0, 0
.L1646:
	add	sp, sp, 48
	ret
	.size	get_block_max_erase_time_from_invalid_list, .-get_block_max_erase_time_from_invalid_list
	.align	2
	.global	get_block_min_erase_time_from_invalid_list
	.type	get_block_min_erase_time_from_invalid_list, %function
get_block_min_erase_time_from_invalid_list:
	sub	sp, sp, #48
	str	x0, [sp, 8]
	strh	w1, [sp, 6]
	str	w2, [sp]
	str	xzr, [sp, 40]
	ldr	x0, [sp, 8]
	ldr	x0, [x0, 8]
	ldrh	w0, [x0, 16]
	strh	w0, [sp, 38]
	ldr	x0, [sp, 8]
	add	x0, x0, 336
	str	x0, [sp, 24]
	ldr	x0, [sp, 24]
	ldr	x0, [x0, 16]
	str	x0, [sp, 24]
	b	.L1648
.L1650:
	ldr	x0, [sp, 24]
	ldrh	w0, [x0, 6]
	ldrh	w1, [sp, 6]
	cmp	w1, w0
	bls	.L1649
	ldr	x0, [sp, 24]
	ldr	w0, [x0, 8]
	mov	w1, w0
	ldr	w0, [sp]
	cmp	w1, w0
	bcs	.L1649
	ldr	x0, [sp, 24]
	ldrh	w0, [x0, 6]
	ldrh	w1, [sp, 38]
	cmp	w1, w0
	bcc	.L1649
	ldr	x0, [sp, 24]
	ldrh	w0, [x0, 6]
	strh	w0, [sp, 38]
	ldr	x0, [sp, 24]
	str	x0, [sp, 40]
.L1649:
	ldr	x0, [sp, 24]
	ldr	x0, [x0, 16]
	str	x0, [sp, 24]
.L1648:
	ldr	x0, [sp, 24]
	cmp	x0, 0
	bne	.L1650
	ldr	x0, [sp, 40]
	cmp	x0, 0
	beq	.L1651
	ldr	x0, [sp, 40]
	b	.L1652
.L1651:
	mov	x0, 0
.L1652:
	add	sp, sp, 48
	ret
	.size	get_block_min_erase_time_from_invalid_list, .-get_block_min_erase_time_from_invalid_list
	.align	2
	.global	get_block_max_count_from_invalid_list
	.type	get_block_max_count_from_invalid_list, %function
get_block_max_count_from_invalid_list:
	sub	sp, sp, #48
	str	x0, [sp, 8]
	str	xzr, [sp, 40]
	str	wzr, [sp, 36]
	ldr	x0, [sp, 8]
	add	x0, x0, 336
	str	x0, [sp, 24]
	ldr	x0, [sp, 24]
	ldr	x0, [x0, 16]
	str	x0, [sp, 24]
	b	.L1654
.L1656:
	ldr	x0, [sp, 24]
	ldr	w0, [x0, 8]
	mov	w1, w0
	ldr	w0, [sp, 36]
	cmp	w1, w0
	bcc	.L1655
	ldr	x0, [sp, 24]
	ldr	w0, [x0, 8]
	str	w0, [sp, 36]
	ldr	x0, [sp, 24]
	str	x0, [sp, 40]
.L1655:
	ldr	x0, [sp, 24]
	ldr	x0, [x0, 16]
	str	x0, [sp, 24]
.L1654:
	ldr	x0, [sp, 24]
	cmp	x0, 0
	bne	.L1656
	ldr	x0, [sp, 40]
	cmp	x0, 0
	beq	.L1657
	ldr	x0, [sp, 40]
	b	.L1658
.L1657:
	mov	x0, 0
.L1658:
	add	sp, sp, 48
	ret
	.size	get_block_max_count_from_invalid_list, .-get_block_max_count_from_invalid_list
	.align	2
	.global	get_block_min_count_from_invalid_list
	.type	get_block_min_count_from_invalid_list, %function
get_block_min_count_from_invalid_list:
	sub	sp, sp, #48
	str	x0, [sp, 8]
	str	xzr, [sp, 40]
	mov	w0, -1
	str	w0, [sp, 36]
	ldr	x0, [sp, 8]
	add	x0, x0, 336
	str	x0, [sp, 24]
	ldr	x0, [sp, 24]
	ldr	x0, [x0, 16]
	str	x0, [sp, 24]
	b	.L1660
.L1662:
	ldr	x0, [sp, 24]
	ldr	w0, [x0, 8]
	mov	w1, w0
	ldr	w0, [sp, 36]
	cmp	w1, w0
	bhi	.L1661
	ldr	x0, [sp, 24]
	ldr	w0, [x0, 8]
	str	w0, [sp, 36]
	ldr	x0, [sp, 24]
	str	x0, [sp, 40]
.L1661:
	ldr	x0, [sp, 24]
	ldr	x0, [x0, 16]
	str	x0, [sp, 24]
.L1660:
	ldr	x0, [sp, 24]
	cmp	x0, 0
	bne	.L1662
	ldr	x0, [sp, 40]
	cmp	x0, 0
	beq	.L1663
	ldr	x0, [sp, 40]
	b	.L1664
.L1663:
	mov	x0, 0
.L1664:
	add	sp, sp, 48
	ret
	.size	get_block_min_count_from_invalid_list, .-get_block_min_count_from_invalid_list
	.align	2
	.global	out_phy_block_from_free_list_by_block
	.type	out_phy_block_from_free_list_by_block, %function
out_phy_block_from_free_list_by_block:
	stp	x29, x30, [sp, -48]!
	add	x29, sp, 0
	str	x0, [x29, 24]
	str	x1, [x29, 16]
	ldr	x1, [x29, 16]
	ldr	x0, [x29, 24]
	bl	del_free_list_by_block
	str	x0, [x29, 40]
	ldr	x0, [x29, 40]
	cmp	x0, 0
	beq	.L1666
	ldr	x0, [x29, 40]
	str	xzr, [x0, 32]
	ldr	x0, [x29, 40]
	str	xzr, [x0, 40]
	ldr	x0, [x29, 24]
	ldrh	w0, [x0, 30]
	cmp	w0, 0
	beq	.L1667
	ldr	x0, [x29, 24]
	ldrh	w0, [x0, 30]
	sub	w0, w0, #1
	uxth	w1, w0
	ldr	x0, [x29, 24]
	strh	w1, [x0, 30]
.L1667:
	ldr	x0, [x29, 40]
	b	.L1668
.L1666:
	ldr	x0, [x29, 40]
.L1668:
	ldp	x29, x30, [sp], 48
	ret
	.size	out_phy_block_from_free_list_by_block, .-out_phy_block_from_free_list_by_block
	.section	.rodata
	.align	3
.LC410:
	.string	"[ND]add invalid_list error !\n"
	.text
	.align	2
	.global	put_phy_block_to_invalid_page_list
	.type	put_phy_block_to_invalid_page_list, %function
put_phy_block_to_invalid_page_list:
	stp	x29, x30, [sp, -48]!
	add	x29, sp, 0
	str	x0, [x29, 24]
	str	x1, [x29, 16]
	ldr	x0, [x29, 16]
	str	xzr, [x0, 16]
	ldr	x0, [x29, 16]
	str	xzr, [x0, 24]
	ldr	x1, [x29, 16]
	ldr	x0, [x29, 24]
	bl	add_block_invalid_list
	str	w0, [x29, 44]
	ldr	w0, [x29, 44]
	cmp	w0, 0
	beq	.L1670
	adrp	x0, .LC410
	add	x0, x0, :lo12:.LC410
	bl	nand_print_dbg
.L1670:
	ldr	w0, [x29, 44]
	ldp	x29, x30, [sp], 48
	ret
	.size	put_phy_block_to_invalid_page_list, .-put_phy_block_to_invalid_page_list
	.align	2
	.global	out_phy_block_from_invalid_page_list
	.type	out_phy_block_from_invalid_page_list, %function
out_phy_block_from_invalid_page_list:
	stp	x29, x30, [sp, -48]!
	add	x29, sp, 0
	str	x0, [x29, 24]
	ldr	x0, [x29, 24]
	bl	del_block_invalid_list
	str	x0, [x29, 40]
	ldr	x0, [x29, 40]
	cmp	x0, 0
	beq	.L1673
	ldr	x0, [x29, 40]
	str	xzr, [x0, 16]
	ldr	x0, [x29, 40]
	str	xzr, [x0, 24]
	ldr	x0, [x29, 40]
	b	.L1674
.L1673:
	ldr	x0, [x29, 40]
.L1674:
	ldp	x29, x30, [sp], 48
	ret
	.size	out_phy_block_from_invalid_page_list, .-out_phy_block_from_invalid_page_list
	.align	2
	.global	out_phy_block_from_invalid_page_list_by_block
	.type	out_phy_block_from_invalid_page_list_by_block, %function
out_phy_block_from_invalid_page_list_by_block:
	stp	x29, x30, [sp, -48]!
	add	x29, sp, 0
	str	x0, [x29, 24]
	str	x1, [x29, 16]
	ldr	x1, [x29, 16]
	ldr	x0, [x29, 24]
	bl	del_block_invalid_list_by_block
	str	x0, [x29, 40]
	ldr	x0, [x29, 40]
	cmp	x0, 0
	beq	.L1676
	ldr	x0, [x29, 40]
	str	xzr, [x0, 16]
	ldr	x0, [x29, 40]
	str	xzr, [x0, 24]
	ldr	x0, [x29, 40]
	b	.L1677
.L1676:
	ldr	x0, [x29, 40]
.L1677:
	ldp	x29, x30, [sp], 48
	ret
	.size	out_phy_block_from_invalid_page_list_by_block, .-out_phy_block_from_invalid_page_list_by_block
	.section	.rodata
	.align	3
.LC411:
	.string	"[ND]invalid_page_count more than pages per blk 0 %d %d!!\n"
	.align	3
.LC412:
	.string	"[ND]invalid_page_count more than pages per blk 1!!\n"
	.text
	.align	2
	.global	phy_block_from_invalid_page_incr
	.type	phy_block_from_invalid_page_incr, %function
phy_block_from_invalid_page_incr:
	stp	x29, x30, [sp, -48]!
	add	x29, sp, 0
	str	x0, [x29, 24]
	str	x1, [x29, 16]
	ldr	x0, [x29, 16]
	ldr	x0, [x0, 16]
	cmp	x0, 0
	bne	.L1679
	ldr	x0, [x29, 16]
	ldr	x0, [x0, 24]
	cmp	x0, 0
	bne	.L1679
	ldr	x0, [x29, 16]
	ldrh	w0, [x0, 4]
	add	w0, w0, 1
	uxth	w1, w0
	ldr	x0, [x29, 16]
	strh	w1, [x0, 4]
	ldr	x0, [x29, 16]
	ldrh	w1, [x0, 4]
	ldr	x0, [x29, 24]
	ldr	x0, [x0, 8]
	ldrh	w0, [x0, 20]
	cmp	w1, w0
	bls	.L1680
	ldr	x0, [x29, 16]
	ldrh	w0, [x0]
	mov	w1, w0
	ldr	x0, [x29, 16]
	ldrh	w0, [x0, 4]
	mov	w2, w0
	adrp	x0, .LC411
	add	x0, x0, :lo12:.LC411
	bl	nand_print_dbg
.L1680:
	mov	w0, 0
	b	.L1681
.L1679:
	ldr	x0, [x29, 16]
	ldrh	w0, [x0, 4]
	add	w0, w0, 1
	uxth	w1, w0
	ldr	x0, [x29, 16]
	strh	w1, [x0, 4]
	ldr	x0, [x29, 16]
	ldrh	w1, [x0, 4]
	ldr	x0, [x29, 24]
	ldr	x0, [x0, 8]
	ldrh	w0, [x0, 20]
	cmp	w1, w0
	bls	.L1682
	adrp	x0, .LC412
	add	x0, x0, :lo12:.LC412
	bl	nand_print_dbg
.L1682:
	ldr	x0, [x29, 16]
	ldr	x0, [x0, 24]
	str	x0, [x29, 40]
	ldr	x0, [x29, 16]
	str	x0, [x29, 32]
	ldr	x0, [x29, 24]
	add	x1, x0, 336
	ldr	x0, [x29, 40]
	cmp	x1, x0
	bne	.L1684
	mov	w0, 0
	b	.L1681
.L1687:
	ldr	x0, [x29, 40]
	ldr	x1, [x0, 24]
	ldr	x0, [x29, 32]
	str	x1, [x0, 24]
	ldr	x0, [x29, 40]
	ldr	x0, [x0, 24]
	ldr	x1, [x29, 32]
	str	x1, [x0, 16]
	ldr	x0, [x29, 32]
	ldr	x1, [x0, 16]
	ldr	x0, [x29, 40]
	str	x1, [x0, 16]
	ldr	x0, [x29, 32]
	ldr	x0, [x0, 16]
	cmp	x0, 0
	beq	.L1685
	ldr	x0, [x29, 32]
	ldr	x0, [x0, 16]
	ldr	x1, [x29, 40]
	str	x1, [x0, 24]
.L1685:
	ldr	x0, [x29, 32]
	ldr	x1, [x29, 40]
	str	x1, [x0, 16]
	ldr	x0, [x29, 40]
	ldr	x1, [x29, 32]
	str	x1, [x0, 24]
	ldr	x0, [x29, 32]
	ldr	x0, [x0, 24]
	str	x0, [x29, 40]
	ldr	x0, [x29, 40]
	ldr	x0, [x0, 24]
	cmp	x0, 0
	beq	.L1688
.L1684:
	ldr	x0, [x29, 40]
	ldrh	w1, [x0, 4]
	ldr	x0, [x29, 32]
	ldrh	w0, [x0, 4]
	cmp	w1, w0
	bcc	.L1687
	b	.L1686
.L1688:
	nop
.L1686:
	mov	w0, 0
.L1681:
	ldp	x29, x30, [sp], 48
	ret
	.size	phy_block_from_invalid_page_incr, .-phy_block_from_invalid_page_incr
	.align	2
	.global	adjust_invaild_list
	.type	adjust_invaild_list, %function
adjust_invaild_list:
	stp	x29, x30, [sp, -48]!
	add	x29, sp, 0
	str	x0, [x29, 24]
	b	.L1690
.L1693:
	ldr	x0, [x29, 24]
	ldr	x0, [x0, 352]
	str	x0, [x29, 40]
	ldr	x0, [x29, 40]
	ldrh	w1, [x0, 4]
	ldr	x0, [x29, 24]
	ldr	x0, [x0, 8]
	ldrh	w0, [x0, 20]
	cmp	w1, w0
	bne	.L1695
	ldr	x0, [x29, 24]
	bl	out_phy_block_from_invalid_page_list
	str	x0, [x29, 40]
	ldr	x1, [x29, 40]
	ldr	x0, [x29, 24]
	bl	put_phy_block_to_free_list
.L1690:
	ldr	x0, [x29, 24]
	ldr	x0, [x0, 352]
	cmp	x0, 0
	bne	.L1693
	b	.L1692
.L1695:
	nop
.L1692:
	mov	w0, 0
	ldp	x29, x30, [sp], 48
	ret
	.size	adjust_invaild_list, .-adjust_invaild_list
	.section	.rodata
	.align	3
.LC413:
	.string	"[NE]%s:malloc fail for cache_node!\n"
	.align	3
.LC414:
	.string	"[NE]%s:malloc fail for cache_page_buf!\n"
	.text
	.align	2
	.global	nftl_cache_init
	.type	nftl_cache_init, %function
nftl_cache_init:
	stp	x29, x30, [sp, -48]!
	add	x29, sp, 0
	str	x0, [x29, 24]
	ldr	x0, [x29, 24]
	add	x0, x0, 1072
	str	x0, [x29, 32]
	ldr	x0, [x29, 24]
	ldr	x0, [x0, 3448]
	ldrh	w1, [x0, 22]
	ldr	x0, [x29, 32]
	strh	w1, [x0]
	ldr	x0, [x29, 32]
	strh	wzr, [x0, 2]
	ldr	x0, [x29, 32]
	strh	wzr, [x0, 4]
	ldr	x0, [x29, 32]
	strh	wzr, [x0, 6]
	ldr	x0, [x29, 32]
	str	xzr, [x0, 1712]
	ldr	x0, [x29, 32]
	str	xzr, [x0, 1720]
	ldr	x0, [x29, 32]
	str	xzr, [x0, 1728]
	ldr	x0, [x29, 32]
	str	xzr, [x0, 1736]
	ldr	x0, [x29, 32]
	str	xzr, [x0, 1768]
	ldr	x0, [x29, 32]
	str	xzr, [x0, 1776]
	ldr	x0, [x29, 32]
	str	xzr, [x0, 1784]
	ldr	x0, [x29, 32]
	str	xzr, [x0, 1792]
	str	wzr, [x29, 44]
	b	.L1697
.L1700:
	ldr	w0, [x29, 44]
	uxth	w3, w0
	ldr	x2, [x29, 32]
	ldr	w1, [x29, 44]
	mov	x0, x1
	lsl	x0, x0, 3
	sub	x0, x0, x1
	lsl	x0, x0, 3
	add	x0, x2, x0
	add	x0, x0, 8
	mov	w1, w3
	strh	w1, [x0]
	ldr	x2, [x29, 32]
	ldr	w1, [x29, 44]
	mov	x0, x1
	lsl	x0, x0, 3
	sub	x0, x0, x1
	lsl	x0, x0, 3
	add	x0, x2, x0
	add	x0, x0, 10
	mov	w1, 255
	strh	w1, [x0]
	ldr	x2, [x29, 32]
	ldr	w1, [x29, 44]
	mov	x0, x1
	lsl	x0, x0, 3
	sub	x0, x0, x1
	lsl	x0, x0, 3
	add	x0, x2, x0
	add	x0, x0, 16
	mov	w1, -1
	str	w1, [x0]
	ldr	x2, [x29, 32]
	ldr	w1, [x29, 44]
	mov	x0, x1
	lsl	x0, x0, 3
	sub	x0, x0, x1
	lsl	x0, x0, 3
	add	x0, x2, x0
	add	x0, x0, 32
	str	xzr, [x0]
	ldr	x2, [x29, 32]
	ldr	w1, [x29, 44]
	mov	x0, x1
	lsl	x0, x0, 3
	sub	x0, x0, x1
	lsl	x0, x0, 3
	add	x0, x2, x0
	add	x0, x0, 40
	str	xzr, [x0]
	ldr	x2, [x29, 32]
	ldr	w1, [x29, 44]
	mov	x0, x1
	lsl	x0, x0, 3
	sub	x0, x0, x1
	lsl	x0, x0, 3
	add	x0, x2, x0
	add	x0, x0, 48
	str	xzr, [x0]
	ldr	x2, [x29, 32]
	ldr	w1, [x29, 44]
	mov	x0, x1
	lsl	x0, x0, 3
	sub	x0, x0, x1
	lsl	x0, x0, 3
	add	x0, x2, x0
	add	x0, x0, 56
	str	xzr, [x0]
	ldr	x2, [x29, 32]
	ldr	w1, [x29, 44]
	mov	x0, x1
	lsl	x0, x0, 3
	sub	x0, x0, x1
	lsl	x0, x0, 3
	add	x0, x2, x0
	add	x0, x0, 12
	strh	wzr, [x0]
	ldr	x2, [x29, 32]
	ldr	w1, [x29, 44]
	mov	x0, x1
	lsl	x0, x0, 3
	sub	x0, x0, x1
	lsl	x0, x0, 3
	add	x0, x2, x0
	add	x0, x0, 14
	strh	wzr, [x0]
	ldr	x0, [x29, 24]
	ldr	x0, [x0, 8]
	ldrh	w0, [x0, 26]
	bl	nftl_malloc
	mov	x3, x0
	ldr	x2, [x29, 32]
	ldr	w1, [x29, 44]
	mov	x0, x1
	lsl	x0, x0, 3
	sub	x0, x0, x1
	lsl	x0, x0, 3
	add	x0, x2, x0
	add	x0, x0, 24
	str	x3, [x0]
	ldr	x2, [x29, 32]
	ldr	w1, [x29, 44]
	mov	x0, x1
	lsl	x0, x0, 3
	sub	x0, x0, x1
	lsl	x0, x0, 3
	add	x0, x2, x0
	add	x0, x0, 24
	ldr	x0, [x0]
	cmp	x0, 0
	bne	.L1698
	adrp	x0, __func__.7884
	add	x1, x0, :lo12:__func__.7884
	adrp	x0, .LC413
	add	x0, x0, :lo12:.LC413
	bl	nand_print
	mov	w0, 1
	b	.L1699
.L1698:
	ldr	w0, [x29, 44]
	add	w0, w0, 1
	str	w0, [x29, 44]
.L1697:
	ldr	x0, [x29, 32]
	ldrh	w0, [x0]
	mov	w1, w0
	ldr	w0, [x29, 44]
	cmp	w1, w0
	bhi	.L1700
	ldr	x0, [x29, 24]
	ldr	x0, [x0, 8]
	ldrh	w0, [x0, 26]
	bl	nftl_malloc
	mov	x1, x0
	ldr	x0, [x29, 32]
	str	x1, [x0, 1800]
	ldr	x0, [x29, 32]
	ldr	x0, [x0, 1800]
	cmp	x0, 0
	bne	.L1701
	adrp	x0, __func__.7884
	add	x1, x0, :lo12:__func__.7884
	adrp	x0, .LC414
	add	x0, x0, :lo12:.LC414
	bl	nand_print
	mov	w0, 1
	b	.L1699
.L1701:
	mov	w0, 0
.L1699:
	ldp	x29, x30, [sp], 48
	ret
	.size	nftl_cache_init, .-nftl_cache_init
	.align	2
	.global	nftl_cache_exit
	.type	nftl_cache_exit, %function
nftl_cache_exit:
	stp	x29, x30, [sp, -48]!
	add	x29, sp, 0
	str	x0, [x29, 24]
	ldr	x0, [x29, 24]
	add	x0, x0, 1072
	str	x0, [x29, 32]
	str	wzr, [x29, 44]
	b	.L1703
.L1704:
	ldr	x2, [x29, 32]
	ldrsw	x1, [x29, 44]
	mov	x0, x1
	lsl	x0, x0, 3
	sub	x0, x0, x1
	lsl	x0, x0, 3
	add	x0, x2, x0
	add	x0, x0, 24
	ldr	x0, [x0]
	bl	nftl_free
	ldr	w0, [x29, 44]
	add	w0, w0, 1
	str	w0, [x29, 44]
.L1703:
	ldr	x0, [x29, 32]
	ldrh	w0, [x0]
	mov	w1, w0
	ldr	w0, [x29, 44]
	cmp	w1, w0
	bgt	.L1704
	ldr	x0, [x29, 32]
	ldr	x0, [x0, 1800]
	bl	nftl_free
	mov	w0, 0
	ldp	x29, x30, [sp], 48
	ret
	.size	nftl_cache_exit, .-nftl_cache_exit
	.align	2
	.global	nftl_cache_clean
	.type	nftl_cache_clean, %function
nftl_cache_clean:
	sub	sp, sp, #32
	str	x0, [sp, 8]
	ldr	x0, [sp, 8]
	add	x0, x0, 1072
	str	x0, [sp, 16]
	ldr	x0, [sp, 8]
	ldr	x0, [x0, 3448]
	ldrh	w1, [x0, 22]
	ldr	x0, [sp, 16]
	strh	w1, [x0]
	ldr	x0, [sp, 16]
	strh	wzr, [x0, 2]
	ldr	x0, [sp, 16]
	strh	wzr, [x0, 4]
	ldr	x0, [sp, 16]
	strh	wzr, [x0, 6]
	ldr	x0, [sp, 16]
	str	xzr, [x0, 1712]
	ldr	x0, [sp, 16]
	str	xzr, [x0, 1720]
	ldr	x0, [sp, 16]
	str	xzr, [x0, 1728]
	ldr	x0, [sp, 16]
	str	xzr, [x0, 1736]
	ldr	x0, [sp, 16]
	str	xzr, [x0, 1768]
	ldr	x0, [sp, 16]
	str	xzr, [x0, 1776]
	ldr	x0, [sp, 16]
	str	xzr, [x0, 1784]
	ldr	x0, [sp, 16]
	str	xzr, [x0, 1792]
	str	wzr, [sp, 28]
	b	.L1707
.L1708:
	ldr	w0, [sp, 28]
	uxth	w3, w0
	ldr	x2, [sp, 16]
	ldr	w1, [sp, 28]
	mov	x0, x1
	lsl	x0, x0, 3
	sub	x0, x0, x1
	lsl	x0, x0, 3
	add	x0, x2, x0
	add	x0, x0, 8
	mov	w1, w3
	strh	w1, [x0]
	ldr	x2, [sp, 16]
	ldr	w1, [sp, 28]
	mov	x0, x1
	lsl	x0, x0, 3
	sub	x0, x0, x1
	lsl	x0, x0, 3
	add	x0, x2, x0
	add	x0, x0, 10
	mov	w1, 255
	strh	w1, [x0]
	ldr	x2, [sp, 16]
	ldr	w1, [sp, 28]
	mov	x0, x1
	lsl	x0, x0, 3
	sub	x0, x0, x1
	lsl	x0, x0, 3
	add	x0, x2, x0
	add	x0, x0, 16
	mov	w1, -1
	str	w1, [x0]
	ldr	x2, [sp, 16]
	ldr	w1, [sp, 28]
	mov	x0, x1
	lsl	x0, x0, 3
	sub	x0, x0, x1
	lsl	x0, x0, 3
	add	x0, x2, x0
	add	x0, x0, 32
	str	xzr, [x0]
	ldr	x2, [sp, 16]
	ldr	w1, [sp, 28]
	mov	x0, x1
	lsl	x0, x0, 3
	sub	x0, x0, x1
	lsl	x0, x0, 3
	add	x0, x2, x0
	add	x0, x0, 40
	str	xzr, [x0]
	ldr	x2, [sp, 16]
	ldr	w1, [sp, 28]
	mov	x0, x1
	lsl	x0, x0, 3
	sub	x0, x0, x1
	lsl	x0, x0, 3
	add	x0, x2, x0
	add	x0, x0, 48
	str	xzr, [x0]
	ldr	x2, [sp, 16]
	ldr	w1, [sp, 28]
	mov	x0, x1
	lsl	x0, x0, 3
	sub	x0, x0, x1
	lsl	x0, x0, 3
	add	x0, x2, x0
	add	x0, x0, 56
	str	xzr, [x0]
	ldr	x2, [sp, 16]
	ldr	w1, [sp, 28]
	mov	x0, x1
	lsl	x0, x0, 3
	sub	x0, x0, x1
	lsl	x0, x0, 3
	add	x0, x2, x0
	add	x0, x0, 12
	strh	wzr, [x0]
	ldr	x2, [sp, 16]
	ldr	w1, [sp, 28]
	mov	x0, x1
	lsl	x0, x0, 3
	sub	x0, x0, x1
	lsl	x0, x0, 3
	add	x0, x2, x0
	add	x0, x0, 14
	strh	wzr, [x0]
	ldr	w0, [sp, 28]
	add	w0, w0, 1
	str	w0, [sp, 28]
.L1707:
	ldr	x0, [sp, 16]
	ldrh	w0, [x0]
	mov	w1, w0
	ldr	w0, [sp, 28]
	cmp	w1, w0
	bhi	.L1708
	mov	w0, 0
	add	sp, sp, 32
	ret
	.size	nftl_cache_clean, .-nftl_cache_clean
	.align	2
	.global	search_from_cache_read_list
	.type	search_from_cache_read_list, %function
search_from_cache_read_list:
	sub	sp, sp, #32
	str	x0, [sp, 8]
	str	w1, [sp, 4]
	ldr	x0, [sp, 8]
	add	x0, x0, 1688
	str	x0, [sp, 24]
	ldr	x0, [sp, 24]
	ldr	x0, [x0, 24]
	str	x0, [sp, 24]
	b	.L1711
.L1714:
	ldr	x0, [sp, 24]
	ldr	w1, [x0, 8]
	ldr	w0, [sp, 4]
	cmp	w1, w0
	bne	.L1712
	ldr	x0, [sp, 24]
	b	.L1713
.L1712:
	ldr	x0, [sp, 24]
	ldr	x0, [x0, 24]
	str	x0, [sp, 24]
.L1711:
	ldr	x0, [sp, 24]
	cmp	x0, 0
	bne	.L1714
	mov	x0, 0
.L1713:
	add	sp, sp, 32
	ret
	.size	search_from_cache_read_list, .-search_from_cache_read_list
	.align	2
	.global	search_from_cache_write_list
	.type	search_from_cache_write_list, %function
search_from_cache_write_list:
	sub	sp, sp, #32
	str	x0, [sp, 8]
	str	w1, [sp, 4]
	ldr	x0, [sp, 8]
	add	x0, x0, 1744
	str	x0, [sp, 24]
	ldr	x0, [sp, 24]
	ldr	x0, [x0, 40]
	str	x0, [sp, 24]
	b	.L1716
.L1719:
	ldr	x0, [sp, 24]
	ldr	w1, [x0, 8]
	ldr	w0, [sp, 4]
	cmp	w1, w0
	bne	.L1717
	ldr	x0, [sp, 24]
	b	.L1718
.L1717:
	ldr	x0, [sp, 24]
	ldr	x0, [x0, 40]
	str	x0, [sp, 24]
.L1716:
	ldr	x0, [sp, 24]
	cmp	x0, 0
	bne	.L1719
	mov	x0, 0
.L1718:
	add	sp, sp, 32
	ret
	.size	search_from_cache_write_list, .-search_from_cache_write_list
	.align	2
	.global	add_to_cache_read_list_tail
	.type	add_to_cache_read_list_tail, %function
add_to_cache_read_list_tail:
	sub	sp, sp, #48
	str	x0, [sp, 24]
	str	x1, [sp, 16]
	str	w2, [sp, 12]
	ldr	x0, [sp, 24]
	add	x0, x0, 1688
	str	x0, [sp, 40]
	b	.L1721
.L1722:
	ldr	x0, [sp, 40]
	ldr	x0, [x0, 24]
	str	x0, [sp, 40]
.L1721:
	ldr	x0, [sp, 40]
	ldr	x0, [x0, 24]
	cmp	x0, 0
	bne	.L1722
	ldr	x0, [sp, 40]
	ldr	x1, [sp, 16]
	str	x1, [x0, 24]
	ldr	x0, [sp, 16]
	str	xzr, [x0, 24]
	ldr	x0, [sp, 16]
	ldr	x1, [sp, 40]
	str	x1, [x0, 32]
	ldr	w0, [sp, 12]
	uxth	w1, w0
	ldr	x0, [sp, 16]
	strh	w1, [x0, 2]
	ldr	w0, [sp, 12]
	cmp	w0, 0
	bne	.L1723
	ldr	x0, [sp, 24]
	ldrh	w0, [x0, 2]
	add	w0, w0, 1
	uxth	w1, w0
	ldr	x0, [sp, 24]
	strh	w1, [x0, 2]
	b	.L1724
.L1723:
	ldr	x0, [sp, 24]
	ldrh	w0, [x0, 6]
	add	w0, w0, 1
	uxth	w1, w0
	ldr	x0, [sp, 24]
	strh	w1, [x0, 6]
.L1724:
	mov	w0, 0
	add	sp, sp, 48
	ret
	.size	add_to_cache_read_list_tail, .-add_to_cache_read_list_tail
	.align	2
	.global	add_to_cache_write_list_tail
	.type	add_to_cache_write_list_tail, %function
add_to_cache_write_list_tail:
	sub	sp, sp, #32
	str	x0, [sp, 8]
	str	x1, [sp]
	ldr	x0, [sp, 8]
	add	x0, x0, 1744
	str	x0, [sp, 24]
	b	.L1727
.L1728:
	ldr	x0, [sp, 24]
	ldr	x0, [x0, 40]
	str	x0, [sp, 24]
.L1727:
	ldr	x0, [sp, 24]
	ldr	x0, [x0, 40]
	cmp	x0, 0
	bne	.L1728
	ldr	x0, [sp, 24]
	ldr	x1, [sp]
	str	x1, [x0, 40]
	ldr	x0, [sp]
	str	xzr, [x0, 40]
	ldr	x0, [sp]
	ldr	x1, [sp, 24]
	str	x1, [x0, 48]
	ldr	x0, [sp]
	mov	w1, 1
	strh	w1, [x0, 2]
	ldr	x0, [sp, 8]
	ldrh	w0, [x0, 4]
	add	w0, w0, 1
	uxth	w1, w0
	ldr	x0, [sp, 8]
	strh	w1, [x0, 4]
	mov	w0, 0
	add	sp, sp, 32
	ret
	.size	add_to_cache_write_list_tail, .-add_to_cache_write_list_tail
	.align	2
	.global	del_from_cache_read_list
	.type	del_from_cache_read_list, %function
del_from_cache_read_list:
	sub	sp, sp, #32
	str	x0, [sp, 8]
	str	x1, [sp]
	ldr	x0, [sp]
	ldr	x0, [x0, 32]
	str	x0, [sp, 24]
	ldr	x0, [sp]
	ldr	x1, [x0, 24]
	ldr	x0, [sp, 24]
	str	x1, [x0, 24]
	ldr	x0, [sp]
	ldr	x0, [x0, 24]
	cmp	x0, 0
	beq	.L1731
	ldr	x0, [sp]
	ldr	x0, [x0, 24]
	ldr	x1, [sp, 24]
	str	x1, [x0, 32]
.L1731:
	ldr	x0, [sp]
	ldrh	w0, [x0, 2]
	cmp	w0, 0
	bne	.L1732
	ldr	x0, [sp, 8]
	ldrh	w0, [x0, 2]
	sub	w0, w0, #1
	uxth	w1, w0
	ldr	x0, [sp, 8]
	strh	w1, [x0, 2]
	b	.L1733
.L1732:
	ldr	x0, [sp, 8]
	ldrh	w0, [x0, 6]
	sub	w0, w0, #1
	uxth	w1, w0
	ldr	x0, [sp, 8]
	strh	w1, [x0, 6]
.L1733:
	ldr	x0, [sp]
	mov	w1, 255
	strh	w1, [x0, 2]
	ldr	x0, [sp]
	str	xzr, [x0, 24]
	ldr	x0, [sp]
	str	xzr, [x0, 32]
	ldr	x0, [sp]
	add	sp, sp, 32
	ret
	.size	del_from_cache_read_list, .-del_from_cache_read_list
	.align	2
	.global	del_from_cache_write_list
	.type	del_from_cache_write_list, %function
del_from_cache_write_list:
	sub	sp, sp, #32
	str	x0, [sp, 8]
	str	x1, [sp]
	ldr	x0, [sp]
	ldr	x0, [x0, 48]
	str	x0, [sp, 24]
	ldr	x0, [sp]
	ldr	x1, [x0, 40]
	ldr	x0, [sp, 24]
	str	x1, [x0, 40]
	ldr	x0, [sp]
	ldr	x0, [x0, 40]
	cmp	x0, 0
	beq	.L1736
	ldr	x0, [sp]
	ldr	x0, [x0, 40]
	ldr	x1, [sp, 24]
	str	x1, [x0, 48]
.L1736:
	ldr	x0, [sp]
	str	xzr, [x0, 40]
	ldr	x0, [sp]
	str	xzr, [x0, 48]
	ldr	x0, [sp]
	mov	w1, 255
	strh	w1, [x0, 2]
	ldr	x0, [sp, 8]
	ldrh	w0, [x0, 4]
	sub	w0, w0, #1
	uxth	w1, w0
	ldr	x0, [sp, 8]
	strh	w1, [x0, 4]
	ldr	x0, [sp]
	add	sp, sp, 32
	ret
	.size	del_from_cache_write_list, .-del_from_cache_write_list
	.align	2
	.global	get_empty_cahce_node
	.type	get_empty_cahce_node, %function
get_empty_cahce_node:
	stp	x29, x30, [sp, -48]!
	add	x29, sp, 0
	str	x0, [x29, 24]
	str	xzr, [x29, 32]
	strh	wzr, [x29, 46]
	b	.L1739
.L1742:
	ldrh	w0, [x29, 46]
	ldr	x2, [x29, 24]
	sxtw	x1, w0
	mov	x0, x1
	lsl	x0, x0, 3
	sub	x0, x0, x1
	lsl	x0, x0, 3
	add	x0, x2, x0
	add	x0, x0, 10
	ldrh	w0, [x0]
	cmp	w0, 255
	bne	.L1740
	ldrh	w0, [x29, 46]
	sxtw	x1, w0
	mov	x0, x1
	lsl	x0, x0, 3
	sub	x0, x0, x1
	lsl	x0, x0, 3
	ldr	x1, [x29, 24]
	add	x0, x1, x0
	add	x0, x0, 8
	b	.L1741
.L1740:
	ldrh	w0, [x29, 46]
	add	w0, w0, 1
	strh	w0, [x29, 46]
.L1739:
	ldr	x0, [x29, 24]
	ldrh	w0, [x0]
	ldrh	w1, [x29, 46]
	cmp	w1, w0
	bcc	.L1742
	ldr	x0, [x29, 24]
	ldrh	w0, [x0, 6]
	cmp	w0, 9
	bls	.L1743
	ldr	x0, [x29, 24]
	add	x0, x0, 1688
	str	x0, [x29, 32]
	ldr	x0, [x29, 32]
	ldr	x0, [x0, 24]
	str	x0, [x29, 32]
	b	.L1744
.L1746:
	ldr	x0, [x29, 32]
	ldrh	w0, [x0, 2]
	cmp	w0, 2
	bne	.L1745
	ldr	x1, [x29, 32]
	ldr	x0, [x29, 24]
	bl	del_from_cache_read_list
	b	.L1741
.L1745:
	ldr	x0, [x29, 32]
	ldr	x0, [x0, 24]
	str	x0, [x29, 32]
.L1744:
	ldr	x0, [x29, 32]
	cmp	x0, 0
	bne	.L1746
	b	.L1747
.L1743:
	ldr	x0, [x29, 24]
	add	x0, x0, 1688
	str	x0, [x29, 32]
	ldr	x0, [x29, 32]
	ldr	x0, [x0, 24]
	str	x0, [x29, 32]
	b	.L1748
.L1750:
	ldr	x0, [x29, 32]
	ldrh	w0, [x0, 2]
	cmp	w0, 0
	bne	.L1749
	ldr	x1, [x29, 32]
	ldr	x0, [x29, 24]
	bl	del_from_cache_read_list
	b	.L1741
.L1749:
	ldr	x0, [x29, 32]
	ldr	x0, [x0, 24]
	str	x0, [x29, 32]
.L1748:
	ldr	x0, [x29, 32]
	cmp	x0, 0
	bne	.L1750
.L1747:
	mov	x0, 0
.L1741:
	ldp	x29, x30, [sp], 48
	ret
	.size	get_empty_cahce_node, .-get_empty_cahce_node
	.section	.rodata
	.align	3
.LC415:
	.string	"[ND]read sector:0x%x,len:0x%x\n"
	.text
	.align	2
	.global	__nand_read
	.type	__nand_read, %function
__nand_read:
	stp	x29, x30, [sp, -64]!
	add	x29, sp, 0
	str	x0, [x29, 40]
	str	w1, [x29, 36]
	str	w2, [x29, 32]
	str	x3, [x29, 24]
	ldr	x0, [x29, 40]
	ldr	w0, [x0, 656]
	cmp	w0, 3
	bne	.L1752
	adrp	x0, .LC415
	add	x0, x0, :lo12:.LC415
	ldr	w2, [x29, 32]
	ldr	w1, [x29, 36]
	bl	nand_print_dbg
.L1752:
	ldr	x0, [x29, 40]
	ldr	x0, [x0, 3440]
	ldr	x1, [x29, 40]
	ldr	x1, [x1, 3440]
	ldr	x2, [x1, 64]
	ldr	w1, [x29, 32]
	add	x1, x2, x1
	str	x1, [x0, 64]
	ldr	x4, [x29, 24]
	ldr	w3, [x29, 32]
	ldr	w2, [x29, 36]
	ldr	x1, [x29, 40]
	mov	w0, 0
	bl	nand_op
	str	w0, [x29, 60]
	ldr	w0, [x29, 60]
	ldp	x29, x30, [sp], 64
	ret
	.size	__nand_read, .-__nand_read
	.section	.rodata
	.align	3
.LC416:
	.string	"[ND]write sector:0x%x,len:0x%x\n"
	.text
	.align	2
	.global	__nand_write
	.type	__nand_write, %function
__nand_write:
	stp	x29, x30, [sp, -64]!
	add	x29, sp, 0
	str	x0, [x29, 40]
	str	w1, [x29, 36]
	str	w2, [x29, 32]
	str	x3, [x29, 24]
	ldr	x0, [x29, 40]
	ldr	w0, [x0, 656]
	cmp	w0, 3
	bne	.L1755
	adrp	x0, .LC416
	add	x0, x0, :lo12:.LC416
	ldr	w2, [x29, 32]
	ldr	w1, [x29, 36]
	bl	nand_print_dbg
.L1755:
	ldr	x0, [x29, 40]
	ldr	x0, [x0, 3440]
	ldr	x1, [x29, 40]
	ldr	x1, [x1, 3440]
	ldr	x2, [x1, 72]
	ldr	w1, [x29, 32]
	add	x1, x2, x1
	str	x1, [x0, 72]
	ldr	x4, [x29, 24]
	ldr	w3, [x29, 32]
	ldr	w2, [x29, 36]
	ldr	x1, [x29, 40]
	mov	w0, 1
	bl	nand_op
	str	w0, [x29, 60]
	ldr	w0, [x29, 60]
	ldp	x29, x30, [sp], 64
	ret
	.size	__nand_write, .-__nand_write
	.section	.rodata
	.align	3
.LC417:
	.string	"[ND]dicard sector:0x%x,len:0x%x\n"
	.text
	.align	2
	.global	__nand_discard
	.type	__nand_discard, %function
__nand_discard:
	stp	x29, x30, [sp, -48]!
	add	x29, sp, 0
	str	x0, [x29, 24]
	str	w1, [x29, 20]
	str	w2, [x29, 16]
	ldr	x0, [x29, 24]
	ldr	w0, [x0, 656]
	cmp	w0, 3
	bne	.L1758
	adrp	x0, .LC417
	add	x0, x0, :lo12:.LC417
	ldr	w2, [x29, 16]
	ldr	w1, [x29, 20]
	bl	nand_print_dbg
.L1758:
	ldr	w2, [x29, 16]
	ldr	w1, [x29, 20]
	ldr	x0, [x29, 24]
	bl	nand_discard
	str	w0, [x29, 44]
	ldr	w0, [x29, 44]
	ldp	x29, x30, [sp], 48
	ret
	.size	__nand_discard, .-__nand_discard
	.section	.rodata
	.align	3
.LC418:
	.string	"[NE]paramter error. beyond reality capacity\n"
	.text
	.align	2
	.global	nand_op
	.type	nand_op, %function
nand_op:
	stp	x29, x30, [sp, -96]!
	add	x29, sp, 0
	strb	w0, [x29, 47]
	str	x1, [x29, 32]
	str	w2, [x29, 40]
	str	w3, [x29, 28]
	str	x4, [x29, 16]
	str	wzr, [x29, 72]
	ldr	w0, [x29, 28]
	cmp	w0, 0
	bne	.L1761
	mov	w0, 0
	b	.L1762
.L1761:
	ldr	w1, [x29, 40]
	ldr	w0, [x29, 28]
	add	w1, w1, w0
	ldr	x0, [x29, 32]
	ldr	w0, [x0, 32]
	cmp	w1, w0
	bls	.L1763
	adrp	x0, .LC418
	add	x0, x0, :lo12:.LC418
	bl	nand_print
	mov	w0, 1
	b	.L1762
.L1763:
	ldrb	w0, [x29, 47]
	cmp	w0, 0
	bne	.L1764
	adrp	x0, nand_cache_read
	add	x0, x0, :lo12:nand_cache_read
	str	x0, [x29, 64]
	b	.L1765
.L1764:
	adrp	x0, nand_cache_write
	add	x0, x0, :lo12:nand_cache_write
	str	x0, [x29, 64]
.L1765:
	ldr	x0, [x29, 32]
	ldr	x0, [x0, 8]
	ldrb	w0, [x0, 8]
	mov	w1, w0
	ldr	w0, [x29, 40]
	udiv	w0, w0, w1
	str	w0, [x29, 60]
	ldr	w0, [x29, 60]
	str	w0, [x29, 76]
	ldr	x0, [x29, 32]
	ldr	x0, [x0, 8]
	ldrb	w0, [x0, 8]
	mov	w1, w0
	ldr	w0, [x29, 40]
	udiv	w2, w0, w1
	mul	w1, w2, w1
	sub	w0, w0, w1
	strh	w0, [x29, 58]
	ldr	x0, [x29, 32]
	ldr	x0, [x0, 8]
	ldrb	w0, [x0, 8]
	uxth	w1, w0
	ldrh	w0, [x29, 58]
	sub	w0, w1, w0
	strh	w0, [x29, 94]
	ldrh	w1, [x29, 94]
	ldr	w0, [x29, 28]
	cmp	w1, w0
	bcc	.L1766
	ldr	w0, [x29, 28]
	strh	w0, [x29, 94]
	str	wzr, [x29, 28]
	b	.L1767
.L1766:
	ldrh	w0, [x29, 94]
	ldr	w1, [x29, 28]
	sub	w0, w1, w0
	str	w0, [x29, 28]
.L1767:
	ldr	x0, [x29, 16]
	str	x0, [x29, 80]
	ldrh	w0, [x29, 94]
	strh	w0, [x29, 92]
	ldrh	w0, [x29, 58]
	lsl	w0, w0, 8
	sxth	w1, w0
	ldrsh	w0, [x29, 94]
	orr	w0, w1, w0
	sxth	w0, w0
	strh	w0, [x29, 56]
	ldr	x4, [x29, 64]
	ldr	x3, [x29, 80]
	ldrh	w2, [x29, 56]
	ldr	w1, [x29, 76]
	ldr	x0, [x29, 32]
	blr	x4
	str	w0, [x29, 72]
	ldr	w0, [x29, 28]
	cmp	w0, 0
	bne	.L1769
	ldr	w0, [x29, 72]
	b	.L1762
.L1770:
	ldr	x0, [x29, 32]
	ldr	x0, [x0, 8]
	ldrb	w0, [x0, 8]
	strh	w0, [x29, 94]
	ldr	w0, [x29, 76]
	add	w0, w0, 1
	str	w0, [x29, 76]
	ldrh	w0, [x29, 92]
	lsl	w0, w0, 9
	sxtw	x0, w0
	ldr	x1, [x29, 80]
	add	x0, x1, x0
	str	x0, [x29, 80]
	ldrh	w0, [x29, 94]
	strh	w0, [x29, 56]
	ldr	x0, [x29, 32]
	ldr	x0, [x0, 8]
	ldrb	w0, [x0, 8]
	mov	w1, w0
	ldr	w0, [x29, 28]
	sub	w0, w0, w1
	str	w0, [x29, 28]
	ldr	x0, [x29, 32]
	ldr	x0, [x0, 8]
	ldrb	w0, [x0, 8]
	strh	w0, [x29, 92]
	ldr	x4, [x29, 64]
	ldr	x3, [x29, 80]
	ldrh	w2, [x29, 56]
	ldr	w1, [x29, 76]
	ldr	x0, [x29, 32]
	blr	x4
	mov	w1, w0
	ldr	w0, [x29, 72]
	orr	w0, w0, w1
	str	w0, [x29, 72]
.L1769:
	ldr	x0, [x29, 32]
	ldr	x0, [x0, 8]
	ldrb	w0, [x0, 8]
	mov	w1, w0
	ldr	w0, [x29, 28]
	cmp	w1, w0
	bcc	.L1770
	ldr	w0, [x29, 28]
	strh	w0, [x29, 94]
	ldr	w0, [x29, 76]
	add	w0, w0, 1
	str	w0, [x29, 76]
	ldrh	w0, [x29, 92]
	lsl	w0, w0, 9
	sxtw	x0, w0
	ldr	x1, [x29, 80]
	add	x0, x1, x0
	str	x0, [x29, 80]
	ldrh	w0, [x29, 94]
	strh	w0, [x29, 56]
	ldr	x4, [x29, 64]
	ldr	x3, [x29, 80]
	ldrh	w2, [x29, 56]
	ldr	w1, [x29, 76]
	ldr	x0, [x29, 32]
	blr	x4
	mov	w1, w0
	ldr	w0, [x29, 72]
	orr	w0, w0, w1
	str	w0, [x29, 72]
	ldr	w0, [x29, 72]
.L1762:
	ldp	x29, x30, [sp], 96
	ret
	.size	nand_op, .-nand_op
	.section	.rodata
	.align	3
.LC419:
	.string	"[NI]nand_cache_read error buf is  NULL!\n"
	.align	3
.LC420:
	.string	"[NE]error1 node %d,%d,%d!\n"
	.text
	.align	2
	.global	nand_cache_read
	.type	nand_cache_read, %function
nand_cache_read:
	stp	x29, x30, [sp, -96]!
	add	x29, sp, 0
	str	x0, [x29, 40]
	str	w1, [x29, 36]
	strh	w2, [x29, 34]
	str	x3, [x29, 24]
	ldr	x0, [x29, 24]
	cmp	x0, 0
	bne	.L1772
	adrp	x0, .LC419
	add	x0, x0, :lo12:.LC419
	bl	nand_print
	mov	w0, 0
	b	.L1773
.L1772:
	str	wzr, [x29, 92]
	ldrh	w0, [x29, 34]
	lsr	w0, w0, 8
	uxth	w0, w0
	str	w0, [x29, 88]
	ldrh	w0, [x29, 34]
	and	w0, w0, 255
	str	w0, [x29, 84]
	ldr	w0, [x29, 88]
	lsl	w0, w0, 9
	str	w0, [x29, 88]
	ldr	w0, [x29, 84]
	lsl	w0, w0, 9
	str	w0, [x29, 84]
	ldr	x0, [x29, 40]
	add	x0, x0, 1072
	str	x0, [x29, 72]
	ldr	x0, [x29, 40]
	ldr	x0, [x0, 3448]
	ldrh	w0, [x0]
	cmp	w0, 0
	beq	.L1774
	ldr	x0, [x29, 72]
	ldr	x0, [x0, 24]
	str	x0, [x29, 64]
	ldr	x0, [x29, 40]
	ldr	x3, [x0, 3512]
	ldr	x2, [x29, 64]
	ldr	w1, [x29, 36]
	ldr	x0, [x29, 40]
	blr	x3
	str	w0, [x29, 92]
	ldr	w0, [x29, 88]
	ldr	x1, [x29, 64]
	add	x0, x1, x0
	ldr	w2, [x29, 84]
	mov	x1, x0
	ldr	x0, [x29, 24]
	bl	nand_memcpy
	ldr	w0, [x29, 92]
	cmp	w0, 10
	bne	.L1775
	ldr	x0, [x29, 40]
	ldr	x0, [x0, 3440]
	ldr	w1, [x0, 116]
	add	w1, w1, 1
	str	w1, [x0, 116]
	ldr	x0, [x29, 40]
	ldr	x3, [x0, 3504]
	ldr	x2, [x29, 64]
	ldr	w1, [x29, 36]
	ldr	x0, [x29, 40]
	blr	x3
	str	wzr, [x29, 92]
.L1775:
	ldr	w0, [x29, 92]
	b	.L1773
.L1774:
	ldr	w1, [x29, 36]
	ldr	x0, [x29, 72]
	bl	search_from_cache_write_list
	str	x0, [x29, 56]
	ldr	x0, [x29, 56]
	cmp	x0, 0
	beq	.L1776
	ldr	x0, [x29, 56]
	ldr	x0, [x0, 16]
	str	x0, [x29, 64]
	ldr	x0, [x29, 56]
	ldrh	w0, [x0, 4]
	ubfiz	w0, w0, 9, 7
	strh	w0, [x29, 54]
	ldr	x0, [x29, 56]
	ldrh	w0, [x0, 6]
	ubfiz	w0, w0, 9, 7
	strh	w0, [x29, 52]
	ldrh	w1, [x29, 54]
	ldrh	w0, [x29, 52]
	add	w0, w1, w0
	mov	w1, w0
	ldr	w0, [x29, 88]
	cmp	w1, w0
	bcc	.L1777
	ldr	w1, [x29, 88]
	ldr	w0, [x29, 84]
	add	w0, w1, w0
	ldrh	w2, [x29, 54]
	ldrh	w1, [x29, 52]
	add	w1, w2, w1
	cmp	w0, w1
	bhi	.L1777
	ldr	w0, [x29, 88]
	ldr	x1, [x29, 64]
	add	x0, x1, x0
	ldr	w2, [x29, 84]
	mov	x1, x0
	ldr	x0, [x29, 24]
	bl	nand_memcpy
	mov	w0, 0
	b	.L1773
.L1777:
	ldr	x0, [x29, 40]
	ldr	x3, [x0, 3512]
	ldr	x0, [x29, 72]
	ldr	x0, [x0, 1800]
	mov	x2, x0
	ldr	w1, [x29, 36]
	ldr	x0, [x29, 40]
	blr	x3
	str	w0, [x29, 92]
	ldrh	w0, [x29, 52]
	ldr	x1, [x29, 64]
	add	x3, x1, x0
	ldr	x0, [x29, 72]
	ldr	x1, [x0, 1800]
	ldrh	w0, [x29, 52]
	add	x1, x1, x0
	ldr	x0, [x29, 40]
	ldr	x0, [x0, 8]
	ldrh	w0, [x0, 26]
	mov	w2, w0
	ldrh	w0, [x29, 52]
	sub	w0, w2, w0
	mov	w2, w0
	mov	x0, x3
	bl	nand_memcpy
	ldr	x0, [x29, 40]
	ldr	x0, [x0, 8]
	ldrb	w0, [x0, 8]
	uxth	w1, w0
	ldr	x0, [x29, 56]
	strh	w1, [x0, 6]
	ldr	w0, [x29, 88]
	ldr	x1, [x29, 64]
	add	x0, x1, x0
	ldr	w2, [x29, 84]
	mov	x1, x0
	ldr	x0, [x29, 24]
	bl	nand_memcpy
	mov	w0, 0
	b	.L1773
.L1776:
	ldr	w1, [x29, 36]
	ldr	x0, [x29, 72]
	bl	search_from_cache_read_list
	str	x0, [x29, 56]
	ldr	x0, [x29, 56]
	cmp	x0, 0
	beq	.L1778
	ldr	x0, [x29, 56]
	ldrh	w0, [x0, 2]
	cmp	w0, 0
	bne	.L1779
	ldr	x1, [x29, 56]
	ldr	x0, [x29, 72]
	bl	del_from_cache_read_list
	ldr	x0, [x29, 56]
	ldr	x0, [x0, 16]
	str	x0, [x29, 64]
	ldr	w0, [x29, 88]
	ldr	x1, [x29, 64]
	add	x0, x1, x0
	ldr	w2, [x29, 84]
	mov	x1, x0
	ldr	x0, [x29, 24]
	bl	nand_memcpy
	mov	w2, 0
	ldr	x1, [x29, 56]
	ldr	x0, [x29, 72]
	bl	add_to_cache_read_list_tail
	b	.L1780
.L1779:
	ldr	x0, [x29, 56]
	ldr	x0, [x0, 16]
	str	x0, [x29, 64]
	ldr	w0, [x29, 88]
	ldr	x1, [x29, 64]
	add	x0, x1, x0
	ldr	w2, [x29, 84]
	mov	x1, x0
	ldr	x0, [x29, 24]
	bl	nand_memcpy
.L1780:
	mov	w0, 0
	b	.L1773
.L1778:
	ldr	x0, [x29, 72]
	bl	get_empty_cahce_node
	str	x0, [x29, 56]
	ldr	x0, [x29, 56]
	cmp	x0, 0
	bne	.L1781
	ldr	x0, [x29, 72]
	ldrh	w0, [x0, 4]
	mov	w1, w0
	ldr	x0, [x29, 72]
	ldrh	w0, [x0, 2]
	mov	w2, w0
	ldr	x0, [x29, 72]
	ldrh	w0, [x0, 6]
	mov	w3, w0
	adrp	x0, .LC420
	add	x0, x0, :lo12:.LC420
	bl	nand_print
	mov	w0, 1
	b	.L1773
.L1781:
	ldr	x0, [x29, 56]
	ldr	w1, [x29, 36]
	str	w1, [x0, 8]
	ldr	x0, [x29, 56]
	ldr	x0, [x0, 16]
	str	x0, [x29, 64]
	ldr	x0, [x29, 40]
	ldr	x3, [x0, 3512]
	ldr	x2, [x29, 64]
	ldr	w1, [x29, 36]
	ldr	x0, [x29, 40]
	blr	x3
	str	w0, [x29, 92]
	ldr	x0, [x29, 56]
	strh	wzr, [x0, 4]
	ldr	x0, [x29, 40]
	ldr	x0, [x0, 8]
	ldrb	w0, [x0, 8]
	uxth	w1, w0
	ldr	x0, [x29, 56]
	strh	w1, [x0, 6]
	mov	w2, 0
	ldr	x1, [x29, 56]
	ldr	x0, [x29, 72]
	bl	add_to_cache_read_list_tail
	ldr	w0, [x29, 88]
	ldr	x1, [x29, 64]
	add	x0, x1, x0
	ldr	w2, [x29, 84]
	mov	x1, x0
	ldr	x0, [x29, 24]
	bl	nand_memcpy
	ldr	w0, [x29, 92]
	cmp	w0, 10
	bne	.L1782
	ldr	x0, [x29, 40]
	ldr	x0, [x0, 3440]
	ldr	w1, [x0, 116]
	add	w1, w1, 1
	str	w1, [x0, 116]
	ldr	x0, [x29, 40]
	ldr	x3, [x0, 3504]
	ldr	x2, [x29, 64]
	ldr	w1, [x29, 36]
	ldr	x0, [x29, 40]
	blr	x3
	str	wzr, [x29, 92]
.L1782:
	ldr	w0, [x29, 92]
.L1773:
	ldp	x29, x30, [sp], 96
	ret
	.size	nand_cache_read, .-nand_cache_read
	.section	.rodata
	.align	3
.LC421:
	.string	"[NE]error1 bitmap %x!\n"
	.align	3
.LC422:
	.string	"[NE]ECC_LIMIT happened 12! page:%d!\n"
	.align	3
.LC423:
	.string	"[NE]error1 flush write cache to nand\n"
	.align	3
.LC424:
	.string	"[NE]error2 flush write cache to nand\n"
	.align	3
.LC425:
	.string	"[NI]ECC_LIMIT happened 11! page:%d!\n"
	.text
	.align	2
	.global	nand_cache_write
	.type	nand_cache_write, %function
nand_cache_write:
	stp	x29, x30, [sp, -96]!
	add	x29, sp, 0
	str	x0, [x29, 40]
	str	w1, [x29, 36]
	strh	w2, [x29, 34]
	str	x3, [x29, 24]
	ldr	x0, [x29, 24]
	cmp	x0, 0
	bne	.L1784
	mov	w0, 0
	b	.L1785
.L1784:
	ldrh	w0, [x29, 34]
	lsr	w0, w0, 8
	strh	w0, [x29, 94]
	ldrh	w0, [x29, 34]
	and	w0, w0, 255
	strh	w0, [x29, 92]
	ldrh	w0, [x29, 94]
	ubfiz	w0, w0, 9, 7
	strh	w0, [x29, 94]
	ldrh	w0, [x29, 92]
	ubfiz	w0, w0, 9, 7
	strh	w0, [x29, 92]
	str	wzr, [x29, 88]
	ldr	x0, [x29, 40]
	add	x0, x0, 1072
	str	x0, [x29, 80]
	ldr	x0, [x29, 40]
	ldr	x0, [x0, 3448]
	ldrh	w0, [x0]
	cmp	w0, 0
	beq	.L1786
	ldr	x0, [x29, 80]
	ldr	x0, [x0, 24]
	str	x0, [x29, 72]
	ldr	x0, [x29, 40]
	ldr	x3, [x0, 3512]
	ldr	x2, [x29, 72]
	ldr	w1, [x29, 36]
	ldr	x0, [x29, 40]
	blr	x3
	ldrh	w0, [x29, 94]
	ldr	x1, [x29, 72]
	add	x0, x1, x0
	ldrh	w1, [x29, 92]
	mov	w2, w1
	ldr	x1, [x29, 24]
	bl	nand_memcpy
	ldr	x0, [x29, 40]
	ldr	x3, [x0, 3504]
	ldr	x2, [x29, 72]
	ldr	w1, [x29, 36]
	ldr	x0, [x29, 40]
	blr	x3
	str	w0, [x29, 88]
	ldr	w0, [x29, 88]
	b	.L1785
.L1786:
	ldr	x0, [x29, 40]
	ldr	x0, [x0, 3448]
	ldrh	w0, [x0, 2]
	cmp	w0, 0
	bne	.L1787
	ldr	x0, [x29, 40]
	ldr	x0, [x0, 8]
	ldrb	w0, [x0, 8]
	uxth	w0, w0
	ldrh	w1, [x29, 34]
	cmp	w1, w0
	bne	.L1787
	ldr	x0, [x29, 40]
	ldr	x0, [x0, 3448]
	ldrh	w0, [x0, 26]
	cmp	w0, 0
	bne	.L1787
	ldr	w1, [x29, 36]
	ldr	x0, [x29, 80]
	bl	search_from_cache_write_list
	str	x0, [x29, 64]
	ldr	x0, [x29, 64]
	cmp	x0, 0
	beq	.L1788
	ldr	x1, [x29, 64]
	ldr	x0, [x29, 80]
	bl	del_from_cache_write_list
.L1788:
	ldr	w1, [x29, 36]
	ldr	x0, [x29, 80]
	bl	search_from_cache_read_list
	str	x0, [x29, 64]
	ldr	x0, [x29, 64]
	cmp	x0, 0
	beq	.L1789
	ldr	x1, [x29, 64]
	ldr	x0, [x29, 80]
	bl	del_from_cache_read_list
.L1789:
	ldr	x0, [x29, 40]
	ldr	x3, [x0, 3504]
	ldr	x2, [x29, 24]
	ldr	w1, [x29, 36]
	ldr	x0, [x29, 40]
	blr	x3
	str	w0, [x29, 88]
	ldr	w0, [x29, 88]
	cmp	w0, 0
	beq	.L1790
	ldrh	w1, [x29, 34]
	adrp	x0, .LC421
	add	x0, x0, :lo12:.LC421
	bl	nand_print
	mov	w0, 1
	b	.L1785
.L1790:
	mov	w0, 0
	b	.L1785
.L1787:
	ldr	w1, [x29, 36]
	ldr	x0, [x29, 80]
	bl	search_from_cache_write_list
	str	x0, [x29, 64]
	ldr	x0, [x29, 64]
	cmp	x0, 0
	beq	.L1791
	ldr	x1, [x29, 64]
	ldr	x0, [x29, 80]
	bl	del_from_cache_write_list
	ldr	x0, [x29, 64]
	ldr	x0, [x0, 16]
	str	x0, [x29, 72]
	ldr	x0, [x29, 64]
	ldrh	w0, [x0, 6]
	ubfiz	w0, w0, 9, 7
	strh	w0, [x29, 62]
	ldr	x0, [x29, 64]
	ldrh	w1, [x0, 6]
	ldr	x0, [x29, 40]
	ldr	x0, [x0, 8]
	ldrb	w0, [x0, 8]
	uxth	w0, w0
	cmp	w1, w0
	beq	.L1792
	ldrh	w1, [x29, 94]
	ldrh	w0, [x29, 62]
	cmp	w1, w0
	bne	.L1793
	ldr	x0, [x29, 64]
	ldrh	w1, [x0, 6]
	ldrh	w0, [x29, 92]
	lsr	w0, w0, 9
	uxth	w0, w0
	add	w0, w1, w0
	uxth	w1, w0
	ldr	x0, [x29, 64]
	strh	w1, [x0, 6]
	b	.L1792
.L1793:
	ldr	x0, [x29, 40]
	ldr	x3, [x0, 3512]
	ldr	x0, [x29, 80]
	ldr	x0, [x0, 1800]
	mov	x2, x0
	ldr	w1, [x29, 36]
	ldr	x0, [x29, 40]
	blr	x3
	str	w0, [x29, 88]
	ldrh	w0, [x29, 62]
	ldr	x1, [x29, 72]
	add	x3, x1, x0
	ldr	x0, [x29, 80]
	ldr	x1, [x0, 1800]
	ldrh	w0, [x29, 62]
	add	x1, x1, x0
	ldr	x0, [x29, 40]
	ldr	x0, [x0, 8]
	ldrh	w0, [x0, 26]
	mov	w2, w0
	ldrh	w0, [x29, 62]
	sub	w0, w2, w0
	mov	w2, w0
	mov	x0, x3
	bl	nand_memcpy
	ldr	x0, [x29, 40]
	ldr	x0, [x0, 8]
	ldrb	w0, [x0, 8]
	uxth	w1, w0
	ldr	x0, [x29, 64]
	strh	w1, [x0, 6]
	ldr	w0, [x29, 88]
	cmp	w0, 10
	bne	.L1792
	adrp	x0, .LC422
	add	x0, x0, :lo12:.LC422
	ldr	w1, [x29, 36]
	bl	nand_print
.L1792:
	ldrh	w0, [x29, 94]
	ldr	x1, [x29, 72]
	add	x0, x1, x0
	ldrh	w1, [x29, 92]
	mov	w2, w1
	ldr	x1, [x29, 24]
	bl	nand_memcpy
	ldr	x1, [x29, 64]
	ldr	x0, [x29, 80]
	bl	add_to_cache_write_list_tail
	ldr	w1, [x29, 36]
	ldr	x0, [x29, 80]
	bl	search_from_cache_read_list
	str	x0, [x29, 64]
	ldr	x0, [x29, 64]
	cmp	x0, 0
	beq	.L1794
	ldr	x1, [x29, 64]
	ldr	x0, [x29, 80]
	bl	del_from_cache_read_list
.L1794:
	mov	w0, 0
	b	.L1785
.L1791:
	ldr	w1, [x29, 36]
	ldr	x0, [x29, 80]
	bl	search_from_cache_read_list
	str	x0, [x29, 64]
	ldr	x0, [x29, 64]
	cmp	x0, 0
	beq	.L1797
	ldr	x0, [x29, 64]
	ldr	x0, [x0, 16]
	str	x0, [x29, 72]
	ldrh	w0, [x29, 94]
	ldr	x1, [x29, 72]
	add	x0, x1, x0
	ldrh	w1, [x29, 92]
	mov	w2, w1
	ldr	x1, [x29, 24]
	bl	nand_memcpy
	ldr	x0, [x29, 64]
	strh	wzr, [x0, 4]
	ldr	x0, [x29, 40]
	ldr	x0, [x0, 8]
	ldrb	w0, [x0, 8]
	uxth	w1, w0
	ldr	x0, [x29, 64]
	strh	w1, [x0, 6]
	ldr	x1, [x29, 64]
	ldr	x0, [x29, 80]
	bl	del_from_cache_read_list
	ldr	x1, [x29, 64]
	ldr	x0, [x29, 80]
	bl	add_to_cache_write_list_tail
	ldr	x0, [x29, 80]
	ldrh	w1, [x0, 4]
	ldr	x0, [x29, 40]
	ldr	x0, [x0, 3448]
	ldrh	w0, [x0, 24]
	cmp	w1, w0
	bcc	.L1796
	mov	w1, 0
	ldr	x0, [x29, 40]
	bl	flush_write_cache_to_nand
	cmp	w0, 0
	beq	.L1796
	adrp	x0, .LC423
	add	x0, x0, :lo12:.LC423
	bl	nand_print
	mov	w0, 1
	b	.L1785
.L1796:
	mov	w0, 0
	b	.L1785
.L1798:
	mov	w1, 0
	ldr	x0, [x29, 40]
	bl	flush_write_cache_to_nand
	cmp	w0, 0
	beq	.L1797
	adrp	x0, .LC424
	add	x0, x0, :lo12:.LC424
	bl	nand_print
	mov	w0, 1
	b	.L1785
.L1797:
	ldr	x0, [x29, 80]
	ldrh	w1, [x0, 4]
	ldr	x0, [x29, 40]
	ldr	x0, [x0, 3448]
	ldrh	w0, [x0, 24]
	cmp	w1, w0
	bcs	.L1798
	ldr	x0, [x29, 80]
	bl	get_empty_cahce_node
	str	x0, [x29, 64]
	ldr	x0, [x29, 64]
	cmp	x0, 0
	bne	.L1799
	ldr	x0, [x29, 80]
	ldrh	w0, [x0, 4]
	mov	w1, w0
	ldr	x0, [x29, 80]
	ldrh	w0, [x0, 2]
	mov	w2, w0
	ldr	x0, [x29, 80]
	ldrh	w0, [x0, 6]
	mov	w3, w0
	adrp	x0, .LC420
	add	x0, x0, :lo12:.LC420
	bl	nand_print
	mov	w0, 1
	b	.L1785
.L1799:
	ldr	x0, [x29, 64]
	ldr	w1, [x29, 36]
	str	w1, [x0, 8]
	ldr	x0, [x29, 64]
	ldr	x0, [x0, 16]
	str	x0, [x29, 72]
	ldrh	w0, [x29, 94]
	cmp	w0, 0
	bne	.L1800
	ldr	x0, [x29, 64]
	strh	wzr, [x0, 4]
	ldrh	w0, [x29, 92]
	lsr	w0, w0, 9
	uxth	w1, w0
	ldr	x0, [x29, 64]
	strh	w1, [x0, 6]
	b	.L1801
.L1800:
	ldr	x0, [x29, 40]
	ldr	x3, [x0, 3512]
	ldr	x2, [x29, 72]
	ldr	w1, [x29, 36]
	ldr	x0, [x29, 40]
	blr	x3
	str	w0, [x29, 88]
	ldr	w0, [x29, 88]
	cmp	w0, 10
	bne	.L1802
	adrp	x0, .LC425
	add	x0, x0, :lo12:.LC425
	ldr	w1, [x29, 36]
	bl	nand_print
.L1802:
	ldr	x0, [x29, 64]
	strh	wzr, [x0, 4]
	ldr	x0, [x29, 40]
	ldr	x0, [x0, 8]
	ldrb	w0, [x0, 8]
	uxth	w1, w0
	ldr	x0, [x29, 64]
	strh	w1, [x0, 6]
.L1801:
	ldrh	w0, [x29, 94]
	ldr	x1, [x29, 72]
	add	x0, x1, x0
	ldrh	w1, [x29, 92]
	mov	w2, w1
	ldr	x1, [x29, 24]
	bl	nand_memcpy
	ldr	x1, [x29, 64]
	ldr	x0, [x29, 80]
	bl	add_to_cache_write_list_tail
	mov	w0, 0
.L1785:
	ldp	x29, x30, [sp], 96
	ret
	.size	nand_cache_write, .-nand_cache_write
	.section	.rodata
	.align	3
.LC426:
	.string	"[NE]flush write cache to nand no cache!\n"
	.align	3
.LC427:
	.string	"[NE]ECC_LIMIT happened 10! page:%d cache: %d!\n"
	.align	3
.LC428:
	.string	"[NE]flush write cache to nand error!\n"
	.text
	.align	2
	.global	flush_write_cache_to_nand
	.type	flush_write_cache_to_nand, %function
flush_write_cache_to_nand:
	stp	x29, x30, [sp, -112]!
	add	x29, sp, 0
	str	x0, [x29, 24]
	str	w1, [x29, 20]
	str	xzr, [x29, 96]
	mov	w0, 1
	strb	w0, [x29, 111]
	mov	w0, 44
	strb	w0, [x29, 48]
	mov	w0, 100
	strb	w0, [x29, 49]
	mov	w0, 68
	strb	w0, [x29, 50]
	mov	w0, 50
	strb	w0, [x29, 51]
	mov	w0, -91
	strb	w0, [x29, 52]
	mov	w0, -1
	strb	w0, [x29, 53]
	mov	w0, -1
	strb	w0, [x29, 54]
	mov	w0, -1
	strb	w0, [x29, 55]
	mov	w0, 44
	strb	w0, [x29, 40]
	mov	w0, -124
	strb	w0, [x29, 41]
	mov	w0, 68
	strb	w0, [x29, 42]
	mov	w0, 50
	strb	w0, [x29, 43]
	mov	w0, -86
	strb	w0, [x29, 44]
	mov	w0, 4
	strb	w0, [x29, 45]
	strb	wzr, [x29, 46]
	strb	wzr, [x29, 47]
	str	wzr, [x29, 92]
	ldr	x0, [x29, 24]
	add	x0, x0, 1072
	str	x0, [x29, 80]
	ldr	x0, [x29, 80]
	ldr	x0, [x0, 1784]
	cmp	x0, 0
	bne	.L1804
	adrp	x0, .LC426
	add	x0, x0, :lo12:.LC426
	bl	nand_print
	mov	w0, 0
	b	.L1811
.L1804:
	ldr	x0, [x29, 80]
	ldr	x0, [x0, 1784]
	mov	x1, x0
	ldr	x0, [x29, 80]
	bl	del_from_cache_write_list
	str	x0, [x29, 72]
	ldr	x0, [x29, 72]
	cmp	x0, 0
	bne	.L1806
	mov	w0, 0
	b	.L1811
.L1806:
	ldr	x0, [x29, 72]
	ldr	x0, [x0, 16]
	str	x0, [x29, 64]
	ldr	x0, [x29, 72]
	ldrh	w1, [x0, 6]
	ldr	x0, [x29, 24]
	ldr	x0, [x0, 8]
	ldrb	w0, [x0, 8]
	uxth	w0, w0
	cmp	w1, w0
	beq	.L1807
	ldr	x0, [x29, 24]
	ldr	x3, [x0, 3512]
	ldr	x0, [x29, 72]
	ldr	w1, [x0, 8]
	ldr	x0, [x29, 80]
	ldr	x0, [x0, 1800]
	mov	x2, x0
	ldr	x0, [x29, 24]
	blr	x3
	str	w0, [x29, 92]
	ldr	x0, [x29, 72]
	ldrh	w0, [x0, 6]
	ubfiz	w0, w0, 9, 7
	strh	w0, [x29, 62]
	ldrh	w0, [x29, 62]
	ldr	x1, [x29, 64]
	add	x3, x1, x0
	ldr	x0, [x29, 80]
	ldr	x1, [x0, 1800]
	ldrh	w0, [x29, 62]
	add	x1, x1, x0
	ldr	x0, [x29, 24]
	ldr	x0, [x0, 8]
	ldrh	w0, [x0, 26]
	mov	w2, w0
	ldrh	w0, [x29, 62]
	sub	w0, w2, w0
	mov	w2, w0
	mov	x0, x3
	bl	nand_memcpy
	ldr	w0, [x29, 92]
	cmp	w0, 10
	bne	.L1807
	ldr	x0, [x29, 72]
	ldr	w1, [x0, 8]
	ldr	x0, [x29, 80]
	ldrh	w0, [x0, 6]
	mov	w2, w0
	adrp	x0, .LC427
	add	x0, x0, :lo12:.LC427
	bl	nand_print
.L1807:
	ldr	x0, [x29, 72]
	strh	wzr, [x0, 4]
	ldr	x0, [x29, 24]
	ldr	x0, [x0, 8]
	ldrb	w0, [x0, 8]
	uxth	w1, w0
	ldr	x0, [x29, 72]
	strh	w1, [x0, 6]
	ldr	x0, [x29, 24]
	ldr	x3, [x0, 3504]
	ldr	x0, [x29, 72]
	ldr	w0, [x0, 8]
	ldr	x2, [x29, 64]
	mov	w1, w0
	ldr	x0, [x29, 24]
	blr	x3
	str	w0, [x29, 92]
	ldr	w0, [x29, 92]
	cmp	w0, 0
	beq	.L1808
	adrp	x0, .LC428
	add	x0, x0, :lo12:.LC428
	bl	nand_print
.L1808:
	ldr	x0, [x29, 24]
	ldr	x0, [x0, 8]
	str	x0, [x29, 96]
	add	x0, x29, 48
	mov	x2, 8
	mov	x1, x0
	ldr	x0, [x29, 96]
	bl	nand_memcmp
	strb	w0, [x29, 111]
	ldrb	w0, [x29, 111]
	cmp	w0, 0
	beq	.L1809
	add	x0, x29, 40
	mov	x2, 8
	mov	x1, x0
	ldr	x0, [x29, 96]
	bl	nand_memcmp
	strb	w0, [x29, 111]
.L1809:
	ldrb	w0, [x29, 111]
	cmp	w0, 0
	bne	.L1810
	ldr	x0, [x29, 24]
	ldrh	w0, [x0, 584]
	cmp	w0, 0
	beq	.L1810
	ldr	x0, [x29, 24]
	ldrh	w0, [x0, 584]
	sub	w0, w0, #1
	bl	nand_phy_get_page_type
	cmp	w0, 2
	bne	.L1810
	ldr	x0, [x29, 24]
	ldr	x3, [x0, 3504]
	ldr	x0, [x29, 72]
	ldr	w0, [x0, 8]
	ldr	x2, [x29, 64]
	mov	w1, w0
	ldr	x0, [x29, 24]
	blr	x3
	str	w0, [x29, 92]
	ldr	w0, [x29, 92]
	cmp	w0, 0
	beq	.L1810
	adrp	x0, .LC428
	add	x0, x0, :lo12:.LC428
	bl	nand_print
.L1810:
	mov	w2, 2
	ldr	x1, [x29, 72]
	ldr	x0, [x29, 80]
	bl	add_to_cache_read_list_tail
	mov	w0, 0
.L1811:
	ldp	x29, x30, [sp], 112
	ret
	.size	flush_write_cache_to_nand, .-flush_write_cache_to_nand
	.section	.rodata
	.align	3
.LC429:
	.string	"[ND]flush cache %d %d!\n"
	.text
	.align	2
	.global	__nand_flush_write_cache
	.type	__nand_flush_write_cache, %function
__nand_flush_write_cache:
	stp	x29, x30, [sp, -48]!
	add	x29, sp, 0
	str	x0, [x29, 24]
	str	w1, [x29, 20]
	str	wzr, [x29, 40]
	ldr	x0, [x29, 24]
	add	x0, x0, 1072
	str	x0, [x29, 32]
	str	wzr, [x29, 44]
	b	.L1813
.L1815:
	ldr	w0, [x29, 40]
	add	w0, w0, 1
	str	w0, [x29, 40]
	mov	w1, 1
	ldr	x0, [x29, 24]
	bl	flush_write_cache_to_nand
	mov	w1, w0
	ldr	w0, [x29, 44]
	orr	w0, w0, w1
	str	w0, [x29, 44]
	ldr	w0, [x29, 20]
	sub	w0, w0, #1
	str	w0, [x29, 20]
	ldr	w0, [x29, 20]
	cmp	w0, 0
	beq	.L1819
.L1813:
	ldr	x0, [x29, 32]
	ldr	x0, [x0, 1784]
	cmp	x0, 0
	bne	.L1815
	b	.L1814
.L1819:
	nop
.L1814:
	ldr	w0, [x29, 44]
	cmp	w0, 0
	beq	.L1816
	adrp	x0, .LC145
	add	x0, x0, :lo12:.LC145
	bl	nand_print
.L1816:
	ldr	w0, [x29, 40]
	cmp	w0, 0
	beq	.L1817
	ldr	x0, [x29, 24]
	ldr	w0, [x0, 656]
	cmp	w0, 0
	beq	.L1817
	ldr	x0, [x29, 24]
	ldrb	w0, [x0, 24]
	mov	w1, w0
	adrp	x0, .LC429
	add	x0, x0, :lo12:.LC429
	ldr	w2, [x29, 40]
	bl	nand_print_dbg
.L1817:
	ldr	w0, [x29, 44]
	ldp	x29, x30, [sp], 48
	ret
	.size	__nand_flush_write_cache, .-__nand_flush_write_cache
	.section	.rodata
	.align	3
.LC430:
	.string	"[NE]nand_discard paramter error!\n"
	.text
	.align	2
	.global	nand_discard
	.type	nand_discard, %function
nand_discard:
	stp	x29, x30, [sp, -64]!
	add	x29, sp, 0
	str	x0, [x29, 24]
	str	w1, [x29, 20]
	str	w2, [x29, 16]
	str	wzr, [x29, 56]
	ldr	x0, [x29, 24]
	add	x0, x0, 1072
	str	x0, [x29, 48]
	ldr	w1, [x29, 20]
	ldr	w0, [x29, 16]
	add	w1, w1, w0
	ldr	x0, [x29, 24]
	ldr	w0, [x0, 32]
	cmp	w1, w0
	bls	.L1821
	adrp	x0, .LC430
	add	x0, x0, :lo12:.LC430
	bl	nand_print
	mov	w0, 1
	b	.L1822
.L1821:
	ldr	x0, [x29, 24]
	ldr	x0, [x0, 3440]
	ldr	x1, [x29, 24]
	ldr	x1, [x1, 3440]
	ldr	x2, [x1, 96]
	ldr	w1, [x29, 16]
	add	x1, x2, x1
	str	x1, [x0, 96]
	ldr	x0, [x29, 24]
	ldr	x0, [x0, 8]
	ldrb	w0, [x0, 8]
	mov	w1, w0
	ldr	w0, [x29, 20]
	udiv	w0, w0, w1
	str	w0, [x29, 60]
	ldr	x0, [x29, 24]
	ldr	x0, [x0, 8]
	ldrb	w0, [x0, 8]
	mov	w1, w0
	ldr	w0, [x29, 20]
	udiv	w2, w0, w1
	mul	w1, w2, w1
	sub	w0, w0, w1
	strh	w0, [x29, 46]
	ldr	x0, [x29, 24]
	ldr	x0, [x0, 8]
	ldrb	w0, [x0, 8]
	uxth	w1, w0
	ldrh	w0, [x29, 46]
	sub	w0, w1, w0
	strh	w0, [x29, 44]
	ldrh	w0, [x29, 46]
	cmp	w0, 0
	beq	.L1823
	ldr	w0, [x29, 60]
	add	w0, w0, 1
	str	w0, [x29, 60]
	ldrh	w1, [x29, 44]
	ldr	w0, [x29, 16]
	cmp	w1, w0
	bls	.L1824
	str	wzr, [x29, 16]
	b	.L1826
.L1824:
	ldrh	w0, [x29, 44]
	ldr	w1, [x29, 16]
	sub	w0, w1, w0
	str	w0, [x29, 16]
	b	.L1826
.L1823:
	ldr	x0, [x29, 24]
	ldr	x0, [x0, 8]
	ldrb	w0, [x0, 8]
	mov	w1, w0
	ldr	w0, [x29, 16]
	cmp	w1, w0
	bls	.L1826
	str	wzr, [x29, 16]
.L1826:
	ldr	x0, [x29, 24]
	ldr	x0, [x0, 8]
	ldrb	w0, [x0, 8]
	mov	w1, w0
	ldr	w0, [x29, 16]
	udiv	w0, w0, w1
	str	w0, [x29, 16]
	ldr	w0, [x29, 16]
	cmp	w0, 0
	bne	.L1828
	mov	w0, 0
	b	.L1822
.L1831:
	ldr	x0, [x29, 24]
	ldr	x0, [x0, 3448]
	ldrh	w0, [x0]
	cmp	w0, 0
	bne	.L1829
	ldr	w1, [x29, 60]
	ldr	x0, [x29, 48]
	bl	search_from_cache_write_list
	str	x0, [x29, 32]
	ldr	x0, [x29, 32]
	cmp	x0, 0
	beq	.L1830
	ldr	x1, [x29, 32]
	ldr	x0, [x29, 48]
	bl	del_from_cache_write_list
.L1830:
	ldr	w1, [x29, 60]
	ldr	x0, [x29, 48]
	bl	search_from_cache_read_list
	str	x0, [x29, 32]
	ldr	x0, [x29, 32]
	cmp	x0, 0
	beq	.L1829
	ldr	x1, [x29, 32]
	ldr	x0, [x29, 48]
	bl	del_from_cache_read_list
.L1829:
	ldr	x0, [x29, 24]
	ldr	x2, [x0, 3520]
	ldr	w1, [x29, 60]
	ldr	x0, [x29, 24]
	blr	x2
	ldr	w0, [x29, 16]
	sub	w0, w0, #1
	str	w0, [x29, 16]
	ldr	w0, [x29, 60]
	add	w0, w0, 1
	str	w0, [x29, 60]
	ldr	x0, [x29, 24]
	ldr	x0, [x0, 3440]
	ldr	x1, [x29, 24]
	ldr	x1, [x1, 3440]
	ldr	x2, [x1, 104]
	ldr	x1, [x29, 24]
	ldr	x1, [x1, 8]
	ldrb	w1, [x1, 8]
	uxtb	x1, w1
	add	x1, x2, x1
	str	x1, [x0, 104]
.L1828:
	ldr	w0, [x29, 16]
	cmp	w0, 0
	bne	.L1831
	ldr	w0, [x29, 56]
.L1822:
	ldp	x29, x30, [sp], 64
	ret
	.size	nand_discard, .-nand_discard
	.align	2
	.global	__shutdown_op
	.type	__shutdown_op, %function
__shutdown_op:
	sub	sp, sp, #32
	str	x0, [sp, 8]
	str	wzr, [sp, 28]
	ldr	w0, [sp, 28]
	add	sp, sp, 32
	ret
	.size	__shutdown_op, .-__shutdown_op
	.section	.rodata
	.align	3
.LC431:
	.string	"[NI]page_map NULL!!\n"
	.align	3
.LC432:
	.string	"[ND]%4d:<%4d %3d><%d>\n"
	.text
	.align	2
	.global	print_logic_page_map
	.type	print_logic_page_map, %function
print_logic_page_map:
	stp	x29, x30, [sp, -48]!
	add	x29, sp, 0
	str	x0, [x29, 24]
	str	w1, [x29, 20]
	ldr	x0, [x29, 24]
	ldr	w0, [x0, 32]
	ldr	x1, [x29, 24]
	ldr	x1, [x1, 8]
	ldrb	w1, [x1, 8]
	udiv	w0, w0, w1
	str	w0, [x29, 40]
	str	wzr, [x29, 44]
	b	.L1835
.L1838:
	ldr	w1, [x29, 44]
	ldr	x0, [x29, 24]
	bl	get_logic_page_map
	str	x0, [x29, 32]
	ldr	x0, [x29, 32]
	cmp	x0, 0
	bne	.L1836
	adrp	x0, .LC431
	add	x0, x0, :lo12:.LC431
	bl	nand_print
	b	.L1837
.L1836:
	ldr	x0, [x29, 32]
	ldrh	w1, [x0, 2]
	mov	w0, 65535
	cmp	w1, w0
	beq	.L1837
	ldr	x0, [x29, 32]
	ldrh	w0, [x0, 2]
	mov	w1, w0
	ldr	x0, [x29, 32]
	ldrh	w0, [x0]
	ubfx	x0, x0, 0, 12
	uxth	w0, w0
	mov	w2, w0
	ldr	x0, [x29, 32]
	ldrb	w0, [x0, 1]
	ubfx	x0, x0, 4, 1
	uxtb	w0, w0
	mov	w3, w0
	adrp	x0, .LC432
	add	x0, x0, :lo12:.LC432
	mov	w4, w3
	mov	w3, w2
	mov	w2, w1
	ldr	w1, [x29, 44]
	bl	nand_print_dbg
.L1837:
	ldr	w0, [x29, 44]
	add	w0, w0, 1
	str	w0, [x29, 44]
.L1835:
	ldr	w1, [x29, 44]
	ldr	w0, [x29, 40]
	cmp	w1, w0
	bcc	.L1838
	mov	w0, 0
	ldp	x29, x30, [sp], 48
	ret
	.size	print_logic_page_map, .-print_logic_page_map
	.align	2
	.global	get_last_nftl
	.type	get_last_nftl, %function
get_last_nftl:
	sub	sp, sp, #32
	str	x0, [sp, 8]
	ldr	x0, [sp, 8]
	str	x0, [sp, 24]
	ldr	x0, [sp, 8]
	str	x0, [sp, 16]
	b	.L1841
.L1842:
	ldr	x0, [sp, 24]
	str	x0, [sp, 16]
	ldr	x0, [sp, 24]
	ldr	x0, [x0, 16]
	str	x0, [sp, 24]
.L1841:
	ldr	x0, [sp, 24]
	cmp	x0, 0
	bne	.L1842
	ldr	x0, [sp, 16]
	add	sp, sp, 32
	ret
	.size	get_last_nftl, .-get_last_nftl
	.align	2
	.global	get_nftl_need_read_claim
	.type	get_nftl_need_read_claim, %function
get_nftl_need_read_claim:
	stp	x29, x30, [sp, -64]!
	add	x29, sp, 0
	str	x0, [x29, 24]
	ldr	x0, [x29, 24]
	str	x0, [x29, 56]
	str	wzr, [x29, 52]
	ldr	x0, [x29, 24]
	bl	get_last_nftl
	str	x0, [x29, 40]
	ldr	x0, [x29, 40]
	ldr	x0, [x0, 24]
	ldr	x0, [x0, 3440]
	ldr	w0, [x0, 172]
	str	w0, [x29, 52]
	ldr	x0, [x29, 24]
	str	x0, [x29, 56]
	b	.L1845
.L1848:
	ldr	x0, [x29, 56]
	ldr	x0, [x0, 24]
	ldrb	w0, [x0, 24]
	mov	w1, w0
	ldr	w0, [x29, 52]
	cmp	w1, w0
	beq	.L1850
	ldr	x0, [x29, 56]
	ldr	x0, [x0, 16]
	str	x0, [x29, 56]
.L1845:
	ldr	x0, [x29, 56]
	cmp	x0, 0
	bne	.L1848
	b	.L1847
.L1850:
	nop
.L1847:
	ldr	x0, [x29, 56]
	ldp	x29, x30, [sp], 64
	ret
	.size	get_nftl_need_read_claim, .-get_nftl_need_read_claim
	.align	2
	.global	get_nftl_read_claim_page_no
	.type	get_nftl_read_claim_page_no, %function
get_nftl_read_claim_page_no:
	stp	x29, x30, [sp, -48]!
	add	x29, sp, 0
	str	x0, [x29, 24]
	ldr	x0, [x29, 24]
	bl	get_last_nftl
	str	x0, [x29, 40]
	ldr	x0, [x29, 40]
	ldr	x0, [x0, 24]
	ldr	x0, [x0, 3440]
	ldr	w0, [x0, 176]
	ldp	x29, x30, [sp], 48
	ret
	.size	get_nftl_read_claim_page_no, .-get_nftl_read_claim_page_no
	.align	2
	.global	clean_read_reclaim_complete_flag
	.type	clean_read_reclaim_complete_flag, %function
clean_read_reclaim_complete_flag:
	sub	sp, sp, #32
	str	x0, [sp, 8]
	ldr	x0, [sp, 8]
	str	x0, [sp, 24]
	b	.L1854
.L1855:
	ldr	x0, [sp, 24]
	ldr	x0, [x0, 24]
	ldr	x1, [sp, 24]
	ldr	x1, [x1, 24]
	ldr	w1, [x1, 664]
	cmp	w1, 0
	cset	w1, eq
	uxtb	w1, w1
	str	w1, [x0, 664]
	ldr	x0, [sp, 24]
	ldr	x0, [x0, 16]
	str	x0, [sp, 24]
.L1854:
	ldr	x0, [sp, 24]
	cmp	x0, 0
	bne	.L1855
	nop
	add	sp, sp, 32
	ret
	.size	clean_read_reclaim_complete_flag, .-clean_read_reclaim_complete_flag
	.align	2
	.global	set_nftl_read_claim_complete
	.type	set_nftl_read_claim_complete, %function
set_nftl_read_claim_complete:
	stp	x29, x30, [sp, -48]!
	add	x29, sp, 0
	str	x0, [x29, 24]
	str	x1, [x29, 16]
	ldr	x0, [x29, 16]
	ldr	x0, [x0, 24]
	mov	w1, 1
	str	w1, [x0, 660]
	ldr	x0, [x29, 24]
	bl	get_last_nftl
	str	x0, [x29, 40]
	ldr	x0, [x29, 16]
	ldr	x0, [x0, 16]
	cmp	x0, 0
	bne	.L1857
	ldr	x0, [x29, 40]
	ldr	x0, [x0, 24]
	ldr	x0, [x0, 3440]
	str	wzr, [x0, 172]
	ldr	x0, [x29, 24]
	bl	clean_read_reclaim_complete_flag
	b	.L1858
.L1857:
	ldr	x0, [x29, 40]
	ldr	x0, [x0, 24]
	ldr	x0, [x0, 3440]
	ldr	x1, [x29, 16]
	ldr	x1, [x1, 24]
	ldrb	w1, [x1, 24]
	add	w1, w1, 1
	str	w1, [x0, 172]
.L1858:
	ldr	x0, [x29, 40]
	ldr	x0, [x0, 24]
	ldr	x0, [x0, 3440]
	str	wzr, [x0, 176]
	mov	w0, 0
	ldp	x29, x30, [sp], 48
	ret
	.size	set_nftl_read_claim_complete, .-set_nftl_read_claim_complete
	.align	2
	.global	set_nftl_read_claim_flag
	.type	set_nftl_read_claim_flag, %function
set_nftl_read_claim_flag:
	sub	sp, sp, #32
	str	x0, [sp, 8]
	str	w1, [sp, 4]
	str	w2, [sp]
	ldr	x0, [sp, 8]
	str	x0, [sp, 24]
	b	.L1861
.L1862:
	ldr	x0, [sp, 24]
	ldr	x0, [x0, 24]
	ldr	x0, [x0, 3440]
	ldr	w1, [sp, 4]
	str	w1, [x0, 172]
	ldr	x0, [sp, 24]
	ldr	x0, [x0, 24]
	ldr	x0, [x0, 3440]
	ldr	w1, [sp]
	str	w1, [x0, 176]
	ldr	x0, [sp, 24]
	ldr	x0, [x0, 16]
	str	x0, [sp, 24]
.L1861:
	ldr	x0, [sp, 24]
	cmp	x0, 0
	bne	.L1862
	mov	w0, 0
	add	sp, sp, 32
	ret
	.size	set_nftl_read_claim_flag, .-set_nftl_read_claim_flag
	.align	2
	.global	get_cold_logic_page
	.type	get_cold_logic_page, %function
get_cold_logic_page:
	stp	x29, x30, [sp, -48]!
	add	x29, sp, 0
	str	x0, [x29, 24]
	str	w1, [x29, 20]
	str	w2, [x29, 16]
	ldr	w0, [x29, 20]
	str	w0, [x29, 44]
	b	.L1865
.L1869:
	ldr	w1, [x29, 44]
	ldr	x0, [x29, 24]
	bl	get_logic_page_map
	str	x0, [x29, 32]
	ldr	x0, [x29, 32]
	cmp	x0, 0
	bne	.L1866
	adrp	x0, .LC431
	add	x0, x0, :lo12:.LC431
	bl	nand_print
	b	.L1867
.L1866:
	ldr	x0, [x29, 32]
	ldrh	w1, [x0, 2]
	mov	w0, 65535
	cmp	w1, w0
	beq	.L1867
	ldr	x0, [x29, 32]
	ldrb	w0, [x0, 1]
	ubfx	x0, x0, 4, 1
	uxtb	w0, w0
	mov	w1, w0
	ldr	x0, [x29, 24]
	ldr	w0, [x0, 664]
	cmp	w1, w0
	beq	.L1867
	ldr	x0, [x29, 24]
	ldr	w0, [x0, 664]
	uxtb	w0, w0
	and	w0, w0, 1
	uxtb	w2, w0
	ldr	x1, [x29, 32]
	ldrb	w0, [x1, 1]
	bfi	w0, w2, 4, 1
	strb	w0, [x1, 1]
	ldr	w0, [x29, 44]
	b	.L1868
.L1867:
	ldr	w0, [x29, 44]
	add	w0, w0, 1
	str	w0, [x29, 44]
.L1865:
	ldr	w1, [x29, 44]
	ldr	w0, [x29, 16]
	cmp	w1, w0
	bcc	.L1869
	mov	w0, -1
.L1868:
	ldp	x29, x30, [sp], 48
	ret
	.size	get_cold_logic_page, .-get_cold_logic_page
	.align	2
	.global	read_reclaim
	.type	read_reclaim, %function
read_reclaim:
	stp	x29, x30, [sp, -64]!
	add	x29, sp, 0
	str	x0, [x29, 40]
	str	x1, [x29, 32]
	str	x2, [x29, 24]
	str	wzr, [x29, 60]
	ldr	x0, [x29, 40]
	bl	get_nftl_read_claim_page_no
	str	w0, [x29, 56]
	ldr	x0, [x29, 32]
	ldr	x3, [x0, 24]
	ldr	x0, [x29, 32]
	ldr	x0, [x0, 24]
	ldr	w0, [x0, 36]
	mov	w2, w0
	ldr	w1, [x29, 56]
	mov	x0, x3
	bl	get_cold_logic_page
	str	w0, [x29, 52]
	ldr	w0, [x29, 52]
	cmn	w0, #1
	bne	.L1871
	ldr	x1, [x29, 32]
	ldr	x0, [x29, 40]
	bl	set_nftl_read_claim_complete
	str	w0, [x29, 60]
	b	.L1872
.L1871:
	ldr	x0, [x29, 32]
	ldr	x4, [x0, 24]
	ldr	x0, [x29, 32]
	ldr	x0, [x0, 24]
	ldr	x0, [x0, 8]
	ldrb	w0, [x0, 8]
	uxth	w0, w0
	ldr	x3, [x29, 24]
	mov	w2, w0
	ldr	w1, [x29, 52]
	mov	x0, x4
	bl	nand_cache_read
	ldr	x0, [x29, 32]
	ldr	x0, [x0, 24]
	ldrb	w0, [x0, 24]
	ldr	w2, [x29, 52]
	mov	w1, w0
	ldr	x0, [x29, 40]
	bl	set_nftl_read_claim_flag
.L1872:
	ldr	w0, [x29, 60]
	ldp	x29, x30, [sp], 64
	ret
	.size	read_reclaim, .-read_reclaim
	.local	gc_src_type
	.comm	gc_src_type,4,4
	.section	.rodata
	.align	3
.LC433:
	.string	"[NE]garbage_collect phy_block null\n"
	.align	3
.LC434:
	.string	"[ND]garbage too frequency! %d:%d\n"
	.align	3
.LC435:
	.string	"[NE]garbage_collect_first wrong fbn:%d\n"
	.text
	.align	2
	.global	garbage_collect
	.type	garbage_collect, %function
garbage_collect:
	stp	x29, x30, [sp, -48]!
	add	x29, sp, 0
	str	x0, [x29, 24]
	ldr	x0, [x29, 24]
	ldrh	w1, [x0, 30]
	ldr	x0, [x29, 24]
	ldrh	w0, [x0, 680]
	cmp	w1, w0
	bhi	.L1875
	ldr	x0, [x29, 24]
	strh	wzr, [x0, 684]
.L1875:
	adrp	x0, gc_src_type
	add	x0, x0, :lo12:gc_src_type
	mov	w1, 3
	str	w1, [x0]
	ldr	x0, [x29, 24]
	ldrh	w0, [x0, 684]
	cmp	w0, 0
	bne	.L1876
	ldr	x0, [x29, 24]
	ldrh	w1, [x0, 30]
	ldr	x0, [x29, 24]
	ldrh	w0, [x0, 682]
	cmp	w1, w0
	bcs	.L1877
	ldr	x0, [x29, 24]
	ldrh	w0, [x0, 686]
	cmp	w0, 0
	bne	.L1878
	ldr	x0, [x29, 24]
	ldr	x0, [x0, 352]
	str	x0, [x29, 40]
	ldr	x0, [x29, 40]
	cmp	x0, 0
	bne	.L1879
	adrp	x0, .LC433
	add	x0, x0, :lo12:.LC433
	bl	nand_print
	mov	w0, 1
	b	.L1880
.L1879:
	ldr	x0, [x29, 40]
	ldrh	w1, [x0, 4]
	ldr	x0, [x29, 24]
	ldr	x0, [x0, 8]
	ldrh	w0, [x0, 20]
	lsr	w0, w0, 1
	uxth	w0, w0
	cmp	w1, w0
	bcc	.L1881
	ldr	x0, [x29, 24]
	mov	w1, 4
	strh	w1, [x0, 688]
	b	.L1882
.L1881:
	ldr	x0, [x29, 40]
	ldrh	w1, [x0, 4]
	ldr	x0, [x29, 24]
	ldr	x0, [x0, 8]
	ldrh	w0, [x0, 20]
	lsr	w0, w0, 2
	uxth	w0, w0
	cmp	w1, w0
	bcc	.L1883
	ldr	x0, [x29, 24]
	mov	w1, 8
	strh	w1, [x0, 688]
	b	.L1882
.L1883:
	ldr	x0, [x29, 40]
	ldrh	w1, [x0, 4]
	ldr	x0, [x29, 24]
	ldr	x0, [x0, 8]
	ldrh	w0, [x0, 20]
	lsr	w0, w0, 3
	uxth	w0, w0
	cmp	w1, w0
	bcc	.L1884
	ldr	x0, [x29, 24]
	mov	w1, 16
	strh	w1, [x0, 688]
	b	.L1882
.L1884:
	ldr	x0, [x29, 24]
	ldrh	w0, [x0, 30]
	mov	w1, w0
	ldr	x0, [x29, 40]
	ldrh	w0, [x0, 4]
	mov	w2, w0
	adrp	x0, .LC434
	add	x0, x0, :lo12:.LC434
	bl	nand_print_dbg
	ldr	x0, [x29, 24]
	mov	w1, -1
	strh	w1, [x0, 688]
.L1882:
	ldr	x0, [x29, 24]
	ldrh	w1, [x0, 30]
	ldr	x0, [x29, 24]
	ldr	x0, [x0, 3448]
	ldrh	w0, [x0, 16]
	cmp	w1, w0
	bhi	.L1878
	ldr	x0, [x29, 24]
	mov	w1, -1
	strh	w1, [x0, 688]
.L1878:
	ldr	x0, [x29, 24]
	ldrh	w0, [x0, 688]
	strh	w0, [x29, 38]
	ldrh	w1, [x29, 38]
	mov	w0, 65535
	cmp	w1, w0
	beq	.L1885
	ldrh	w2, [x29, 38]
	mov	x1, 0
	ldr	x0, [x29, 24]
	bl	garbage_collect_first
	cmp	w0, 0
	beq	.L1876
	ldr	x0, [x29, 24]
	ldrh	w0, [x0, 30]
	mov	w1, w0
	adrp	x0, .LC435
	add	x0, x0, :lo12:.LC435
	bl	nand_print
	mov	w0, 1
	b	.L1880
.L1885:
	mov	w1, 1
	ldr	x0, [x29, 24]
	bl	gc_win_block
	b	.L1876
.L1877:
	ldr	x0, [x29, 24]
	mov	w1, 1
	strh	w1, [x0, 684]
	ldr	x0, [x29, 24]
	strh	wzr, [x0, 686]
	ldr	x0, [x29, 24]
	strh	wzr, [x0, 688]
.L1876:
	mov	w0, 0
.L1880:
	ldp	x29, x30, [sp], 48
	ret
	.size	garbage_collect, .-garbage_collect
	.section	.rodata
	.align	3
.LC436:
	.string	"[NI]garbage_collect_first none!!\n"
	.align	3
.LC437:
	.string	"[ND]garbage_collect_first wrong!!\n"
	.align	3
.LC438:
	.string	"[ND]garbage_collect_first cannot find PHY_MAPPING_PAGE%d %d %d %d!!\n"
	.align	3
.LC439:
	.string	"[ND]garbage_collect_first cannot find PHY_MAPPING_PAGE%x %x %x %x %x %x %x %x %x %x %x %x %x %x %x %x.\n"
	.align	3
.LC440:
	.string	"[ND][NE]garbage_collect_first rebuild %d,%d!!\n"
	.align	3
.LC441:
	.string	"[ND]garbage_collect_first PHY_MAPPING_PAGEecc err block:%d !!\n"
	.align	3
.LC442:
	.string	"[NE]garbage_collect_first invaild_page num error!!\n"
	.align	3
.LC443:
	.string	"[ND]garbage_collect_first read a logic page error%d %d %d %d spare data:\n"
	.align	3
.LC444:
	.string	"%x %x %x %x %x %x %x %x %x %x %x %x %x %x %x %x\n"
	.align	3
.LC445:
	.string	"[NE]nand_write_logic_page_no_gc gc fail!!\n"
	.align	3
.LC446:
	.string	"[NE]erase ecc error block\n"
	.text
	.align	2
	.global	garbage_collect_first
	.type	garbage_collect_first, %function
garbage_collect_first:
	sub	sp, sp, #224
	stp	x29, x30, [sp, 80]
	add	x29, sp, 80
	str	x0, [x29, 40]
	str	x1, [x29, 32]
	strh	w2, [x29, 30]
	str	wzr, [x29, 116]
	str	wzr, [x29, 132]
	str	wzr, [x29, 112]
	add	x0, x29, 72
	mov	w2, 16
	mov	w1, 90
	bl	nand_memset
	ldr	x0, [x29, 32]
	cmp	x0, 0
	bne	.L1888
	ldr	x0, [x29, 40]
	bl	out_phy_block_from_invalid_page_list
	str	x0, [x29, 120]
	b	.L1889
.L1888:
	ldr	x1, [x29, 32]
	ldr	x0, [x29, 40]
	bl	is_current_blcok
	cmp	w0, 1
	bne	.L1890
	mov	w0, 0
	b	.L1915
.L1890:
	ldr	x1, [x29, 32]
	ldr	x0, [x29, 40]
	bl	out_phy_block_from_invalid_page_list_by_block
	str	x0, [x29, 120]
.L1889:
	ldr	x0, [x29, 120]
	cmp	x0, 0
	bne	.L1892
	adrp	x0, .LC436
	add	x0, x0, :lo12:.LC436
	bl	nand_print
	mov	w0, 8
	b	.L1915
.L1892:
	ldr	x0, [x29, 40]
	ldr	x0, [x0, 3440]
	ldr	w1, [x0, 120]
	add	w1, w1, 1
	str	w1, [x0, 120]
	ldr	x0, [x29, 120]
	ldrh	w1, [x0]
	ldr	x0, [x29, 40]
	ldr	x0, [x0, 8]
	ldrh	w0, [x0, 20]
	sub	w0, w0, #1
	uxth	w2, w0
	ldr	x0, [x29, 40]
	ldr	x0, [x0, 8]
	ldrh	w3, [x0, 28]
	ldr	x0, [x29, 40]
	ldr	x4, [x0, 512]
	add	x5, x29, 72
	add	x0, x29, 48
	bl	set_physic_op_par
	ldr	x0, [x29, 40]
	ldr	x2, [x0, 3464]
	add	x0, x29, 48
	mov	x1, x0
	ldr	x0, [x29, 40]
	blr	x2
	str	w0, [x29, 136]
	add	x0, x29, 72
	bl	is_phy_mapping_page
	cmp	w0, 1
	beq	.L1893
	adrp	x0, .LC437
	add	x0, x0, :lo12:.LC437
	bl	nand_print_dbg
	ldr	x0, [x29, 120]
	ldrh	w0, [x0]
	mov	w5, w0
	ldr	x0, [x29, 40]
	ldr	x0, [x0, 8]
	ldrh	w0, [x0, 20]
	sub	w1, w0, #1
	ldr	x0, [x29, 40]
	ldr	x0, [x0, 8]
	ldrh	w0, [x0, 28]
	mov	w2, w0
	adrp	x0, .LC438
	add	x0, x0, :lo12:.LC438
	mov	w4, w2
	ldr	w3, [x29, 112]
	mov	w2, w1
	mov	w1, w5
	bl	nand_print_dbg
	ldrb	w0, [x29, 73]
	mov	w9, w0
	ldrb	w0, [x29, 74]
	mov	w10, w0
	ldrb	w0, [x29, 75]
	mov	w11, w0
	ldrb	w0, [x29, 76]
	mov	w12, w0
	ldrb	w0, [x29, 77]
	mov	w13, w0
	ldrb	w0, [x29, 78]
	mov	w14, w0
	ldrb	w0, [x29, 79]
	mov	w15, w0
	ldrb	w0, [x29, 80]
	mov	w1, w0
	ldrb	w0, [x29, 81]
	mov	w2, w0
	ldrb	w0, [x29, 82]
	mov	w3, w0
	ldrb	w0, [x29, 83]
	mov	w4, w0
	ldrb	w0, [x29, 84]
	mov	w5, w0
	ldrb	w0, [x29, 85]
	mov	w6, w0
	ldrb	w0, [x29, 86]
	mov	w7, w0
	ldrb	w0, [x29, 87]
	mov	w8, w0
	adrp	x0, .LC439
	add	x0, x0, :lo12:.LC439
	str	w8, [sp, 56]
	str	w7, [sp, 48]
	str	w6, [sp, 40]
	str	w5, [sp, 32]
	str	w4, [sp, 24]
	str	w3, [sp, 16]
	str	w2, [sp, 8]
	str	w1, [sp]
	mov	w7, w15
	mov	w6, w14
	mov	w5, w13
	mov	w4, w12
	mov	w3, w11
	mov	w2, w10
	mov	w1, w9
	bl	nand_print_dbg
	ldr	x0, [x29, 40]
	ldrb	w0, [x0, 24]
	mov	w1, w0
	ldr	x0, [x29, 120]
	ldrh	w0, [x0]
	mov	w2, w0
	adrp	x0, .LC440
	add	x0, x0, :lo12:.LC440
	bl	nand_print_dbg
	ldr	x0, [x29, 40]
	ldr	x0, [x0, 480]
	mov	x2, x0
	ldr	x1, [x29, 120]
	ldr	x0, [x29, 40]
	bl	recover_block_phy_page_mapping
	mov	w0, 1
	str	w0, [x29, 132]
	b	.L1894
.L1893:
	ldr	w0, [x29, 136]
	cmp	w0, 0
	beq	.L1895
	ldr	w0, [x29, 136]
	cmp	w0, 10
	beq	.L1895
	ldr	x0, [x29, 120]
	ldrh	w0, [x0]
	mov	w1, w0
	adrp	x0, .LC441
	add	x0, x0, :lo12:.LC441
	bl	nand_print_dbg
	ldr	x0, [x29, 40]
	ldr	x0, [x0, 480]
	mov	x2, x0
	ldr	x1, [x29, 120]
	ldr	x0, [x29, 40]
	bl	recover_block_phy_page_mapping
	mov	w0, 1
	str	w0, [x29, 132]
	b	.L1894
.L1895:
	ldr	x0, [x29, 40]
	ldr	x3, [x0, 480]
	ldr	x0, [x29, 40]
	ldr	x1, [x0, 512]
	ldr	x0, [x29, 40]
	ldr	w0, [x0, 488]
	mov	w2, w0
	mov	x0, x3
	bl	nand_memcpy
.L1894:
	ldr	x0, [x29, 40]
	add	x1, x0, 464
	ldr	x0, [x29, 40]
	ldr	x0, [x0, 8]
	ldrh	w0, [x0, 20]
	mov	w3, w0
	ldr	x2, [x29, 120]
	ldr	x0, [x29, 40]
	bl	check_invalid_page_right
	str	w0, [x29, 136]
	ldr	w0, [x29, 136]
	cmp	w0, 0
	beq	.L1896
	adrp	x0, .LC442
	add	x0, x0, :lo12:.LC442
	bl	nand_print
	mov	w0, 1
	str	w0, [x29, 132]
.L1896:
	ldr	w0, [x29, 132]
	cmp	w0, 1
	bne	.L1897
	mov	w0, -1
	strh	w0, [x29, 30]
.L1897:
	str	wzr, [x29, 140]
	b	.L1898
.L1911:
	ldr	x0, [x29, 40]
	ldr	x1, [x0, 480]
	ldr	w0, [x29, 140]
	lsl	x0, x0, 2
	add	x0, x1, x0
	ldr	w0, [x0]
	str	w0, [x29, 108]
	ldr	w0, [x29, 108]
	cmn	w0, #1
	beq	.L1916
	ldr	w1, [x29, 108]
	ldr	x0, [x29, 40]
	bl	get_logic_page_map
	str	x0, [x29, 96]
	ldr	x0, [x29, 96]
	cmp	x0, 0
	beq	.L1917
	ldr	x0, [x29, 96]
	ldrh	w1, [x0, 2]
	ldr	x0, [x29, 120]
	ldrh	w0, [x0]
	cmp	w1, w0
	bne	.L1918
	ldr	x0, [x29, 96]
	ldrh	w0, [x0]
	ubfx	x0, x0, 0, 12
	uxth	w0, w0
	mov	w1, w0
	ldr	w0, [x29, 140]
	cmp	w1, w0
	bne	.L1919
	ldr	x0, [x29, 40]
	ldr	x0, [x0, 3440]
	ldr	w1, [x0, 124]
	add	w1, w1, 1
	str	w1, [x0, 124]
	ldr	x0, [x29, 120]
	ldrh	w1, [x0]
	ldr	w0, [x29, 140]
	uxth	w2, w0
	ldr	x0, [x29, 40]
	ldr	x0, [x0, 8]
	ldrh	w3, [x0, 28]
	ldr	x0, [x29, 40]
	ldr	x4, [x0, 520]
	add	x5, x29, 72
	add	x0, x29, 48
	bl	set_physic_op_par
	str	wzr, [x29, 92]
	ldr	w0, [x29, 92]
	cmp	w0, 1
	bhi	.L1904
	ldr	x0, [x29, 40]
	ldr	x2, [x0, 3464]
	add	x0, x29, 48
	mov	x1, x0
	ldr	x0, [x29, 40]
	blr	x2
	str	w0, [x29, 136]
	nop
.L1904:
	add	x0, x29, 72
	bl	get_logic_page_from_oob
	str	w0, [x29, 116]
	ldr	w0, [x29, 136]
	cmp	w0, 0
	beq	.L1905
	ldr	w0, [x29, 136]
	cmp	w0, 10
	bne	.L1906
.L1905:
	ldr	w1, [x29, 116]
	ldr	w0, [x29, 108]
	cmp	w1, w0
	beq	.L1907
	ldr	x0, [x29, 120]
	ldrh	w0, [x0]
	mov	w1, w0
	adrp	x0, .LC443
	add	x0, x0, :lo12:.LC443
	ldr	w4, [x29, 108]
	ldr	w3, [x29, 116]
	ldr	w2, [x29, 140]
	bl	nand_print_dbg
	ldrb	w0, [x29, 72]
	mov	w10, w0
	ldrb	w0, [x29, 73]
	mov	w11, w0
	ldrb	w0, [x29, 74]
	mov	w12, w0
	ldrb	w0, [x29, 75]
	mov	w13, w0
	ldrb	w0, [x29, 76]
	mov	w14, w0
	ldrb	w0, [x29, 77]
	mov	w15, w0
	ldrb	w0, [x29, 78]
	mov	w16, w0
	ldrb	w0, [x29, 79]
	mov	w1, w0
	ldrb	w0, [x29, 80]
	mov	w2, w0
	ldrb	w0, [x29, 81]
	mov	w3, w0
	ldrb	w0, [x29, 82]
	mov	w4, w0
	ldrb	w0, [x29, 83]
	mov	w5, w0
	ldrb	w0, [x29, 84]
	mov	w6, w0
	ldrb	w0, [x29, 85]
	mov	w7, w0
	ldrb	w0, [x29, 86]
	mov	w8, w0
	ldrb	w0, [x29, 87]
	mov	w9, w0
	adrp	x0, .LC444
	add	x0, x0, :lo12:.LC444
	str	w9, [sp, 64]
	str	w8, [sp, 56]
	str	w7, [sp, 48]
	str	w6, [sp, 40]
	str	w5, [sp, 32]
	str	w4, [sp, 24]
	str	w3, [sp, 16]
	str	w2, [sp, 8]
	str	w1, [sp]
	mov	w7, w16
	mov	w6, w15
	mov	w5, w14
	mov	w4, w13
	mov	w3, w12
	mov	w2, w11
	mov	w1, w10
	bl	nand_print
	ldr	x0, [x29, 96]
	mov	w1, -1
	strh	w1, [x0, 2]
	ldr	x0, [x29, 96]
	ldrh	w1, [x0]
	orr	w1, w1, 4095
	strh	w1, [x0]
	mov	w0, 1
	str	w0, [x29, 132]
	mov	w0, -1
	strh	w0, [x29, 30]
	ldr	x0, [x29, 120]
	ldrh	w0, [x0, 4]
	add	w0, w0, 1
	uxth	w1, w0
	ldr	x0, [x29, 120]
	strh	w1, [x0, 4]
	b	.L1920
.L1907:
	ldr	x0, [x29, 40]
	ldr	x0, [x0, 520]
	mov	x2, x0
	ldr	w1, [x29, 108]
	ldr	x0, [x29, 40]
	bl	nand_write_logic_page_no_gc
	str	w0, [x29, 136]
	ldr	w0, [x29, 136]
	cmp	w0, 0
	beq	.L1920
	adrp	x0, .LC445
	add	x0, x0, :lo12:.LC445
	bl	nand_print
	mov	w0, 1
	b	.L1915
.L1906:
	ldr	x0, [x29, 120]
	ldrh	w0, [x0, 4]
	add	w0, w0, 1
	uxth	w1, w0
	ldr	x0, [x29, 120]
	strh	w1, [x0, 4]
	mov	w0, 1
	str	w0, [x29, 132]
	mov	w0, -1
	strh	w0, [x29, 30]
	b	.L1909
.L1920:
	nop
.L1909:
	ldrh	w0, [x29, 30]
	sub	w0, w0, #1
	strh	w0, [x29, 30]
	ldrh	w0, [x29, 30]
	cmp	w0, 0
	beq	.L1921
	b	.L1900
.L1916:
	nop
	b	.L1900
.L1917:
	nop
	b	.L1900
.L1918:
	nop
	b	.L1900
.L1919:
	nop
.L1900:
	ldr	w0, [x29, 140]
	add	w0, w0, 1
	str	w0, [x29, 140]
.L1898:
	ldr	x0, [x29, 40]
	ldr	x0, [x0, 8]
	ldrh	w0, [x0, 20]
	sub	w0, w0, #1
	mov	w1, w0
	ldr	w0, [x29, 140]
	cmp	w1, w0
	bhi	.L1911
	b	.L1910
.L1921:
	nop
.L1910:
	ldr	x0, [x29, 40]
	ldr	x0, [x0, 8]
	ldrh	w0, [x0, 20]
	sub	w0, w0, #1
	mov	w1, w0
	ldr	w0, [x29, 140]
	cmp	w1, w0
	bne	.L1912
	ldr	x0, [x29, 40]
	strh	wzr, [x0, 686]
.L1912:
	ldr	w0, [x29, 132]
	cmp	w0, 1
	bne	.L1913
	ldr	x0, [x29, 40]
	ldr	x0, [x0, 8]
	ldrh	w0, [x0, 20]
	sub	w0, w0, #1
	mov	w1, w0
	ldr	w0, [x29, 140]
	cmp	w1, w0
	bne	.L1913
	adrp	x0, .LC446
	add	x0, x0, :lo12:.LC446
	bl	nand_print
	ldr	x0, [x29, 120]
	ldrh	w0, [x0, 6]
	add	w0, w0, 1
	uxth	w0, w0
	mov	w2, w0
	ldr	x1, [x29, 120]
	ldr	x0, [x29, 40]
	bl	erase_block
	b	.L1914
.L1913:
	ldr	x1, [x29, 120]
	ldr	x0, [x29, 40]
	bl	put_phy_block_to_invalid_page_list
	ldr	x0, [x29, 40]
	bl	adjust_invaild_list
.L1914:
	mov	w0, 0
.L1915:
	add	sp, x29, 0
	ldp	x29, x30, [sp], 144
	ret
	.size	garbage_collect_first, .-garbage_collect_first
	.align	2
	.global	get_empty_gc_node
	.type	get_empty_gc_node, %function
get_empty_gc_node:
	sub	sp, sp, #32
	str	x0, [sp, 8]
	strh	wzr, [sp, 30]
	b	.L1923
.L1926:
	ldrh	w0, [sp, 30]
	ldr	x1, [sp, 8]
	sxtw	x0, w0
	lsl	x0, x0, 5
	add	x0, x1, x0
	add	x0, x0, 10
	ldrh	w0, [x0]
	cmp	w0, 0
	bne	.L1924
	ldrh	w0, [sp, 30]
	sxtw	x0, w0
	lsl	x0, x0, 5
	ldr	x1, [sp, 8]
	add	x0, x1, x0
	add	x0, x0, 8
	b	.L1925
.L1924:
	ldrh	w0, [sp, 30]
	add	w0, w0, 1
	strh	w0, [sp, 30]
.L1923:
	ldrh	w0, [sp, 30]
	cmp	w0, 9
	bls	.L1926
	mov	x0, 0
.L1925:
	add	sp, sp, 32
	ret
	.size	get_empty_gc_node, .-get_empty_gc_node
	.align	2
	.global	search_gc_node
	.type	search_gc_node, %function
search_gc_node:
	sub	sp, sp, #32
	str	x0, [sp, 8]
	str	x1, [sp]
	ldr	x0, [sp, 8]
	add	x0, x0, 328
	str	x0, [sp, 24]
	ldr	x0, [sp, 24]
	ldr	x0, [x0, 16]
	str	x0, [sp, 24]
	b	.L1928
.L1931:
	ldr	x0, [sp, 24]
	ldr	x1, [x0, 8]
	ldr	x0, [sp]
	cmp	x1, x0
	bne	.L1929
	ldr	x0, [sp, 24]
	b	.L1930
.L1929:
	ldr	x0, [sp, 24]
	ldr	x0, [x0, 16]
	str	x0, [sp, 24]
.L1928:
	ldr	x0, [sp, 24]
	cmp	x0, 0
	bne	.L1931
	mov	x0, 0
.L1930:
	add	sp, sp, 32
	ret
	.size	search_gc_node, .-search_gc_node
	.align	2
	.global	add_to_gc_list_tail
	.type	add_to_gc_list_tail, %function
add_to_gc_list_tail:
	sub	sp, sp, #32
	str	x0, [sp, 8]
	str	x1, [sp]
	ldr	x0, [sp, 8]
	add	x0, x0, 328
	str	x0, [sp, 24]
	b	.L1933
.L1934:
	ldr	x0, [sp, 24]
	ldr	x0, [x0, 16]
	str	x0, [sp, 24]
.L1933:
	ldr	x0, [sp, 24]
	ldr	x0, [x0, 16]
	cmp	x0, 0
	bne	.L1934
	ldr	x0, [sp, 24]
	ldr	x1, [sp]
	str	x1, [x0, 16]
	ldr	x0, [sp]
	str	xzr, [x0, 16]
	ldr	x0, [sp]
	ldr	x1, [sp, 24]
	str	x1, [x0, 24]
	ldr	x0, [sp, 8]
	ldrh	w0, [x0]
	add	w0, w0, 1
	uxth	w1, w0
	ldr	x0, [sp, 8]
	strh	w1, [x0]
	mov	w0, 0
	add	sp, sp, 32
	ret
	.size	add_to_gc_list_tail, .-add_to_gc_list_tail
	.align	2
	.global	del_from_gc_list
	.type	del_from_gc_list, %function
del_from_gc_list:
	sub	sp, sp, #32
	str	x0, [sp, 8]
	str	x1, [sp]
	ldr	x0, [sp]
	ldr	x0, [x0, 24]
	str	x0, [sp, 24]
	ldr	x0, [sp]
	ldr	x1, [x0, 16]
	ldr	x0, [sp, 24]
	str	x1, [x0, 16]
	ldr	x0, [sp]
	ldr	x0, [x0, 16]
	cmp	x0, 0
	beq	.L1937
	ldr	x0, [sp]
	ldr	x0, [x0, 16]
	ldr	x1, [sp, 24]
	str	x1, [x0, 24]
.L1937:
	ldr	x0, [sp]
	str	xzr, [x0, 16]
	ldr	x0, [sp]
	str	xzr, [x0, 24]
	ldr	x0, [sp, 8]
	ldrh	w0, [x0]
	sub	w0, w0, #1
	uxth	w1, w0
	ldr	x0, [sp, 8]
	strh	w1, [x0]
	ldr	x0, [sp]
	add	sp, sp, 32
	ret
	.size	del_from_gc_list, .-del_from_gc_list
	.align	2
	.global	add_prio_gc
	.type	add_prio_gc, %function
add_prio_gc:
	stp	x29, x30, [sp, -64]!
	add	x29, sp, 0
	str	x0, [x29, 40]
	str	x1, [x29, 32]
	strh	w2, [x29, 30]
	ldr	x0, [x29, 40]
	add	x0, x0, 696
	ldr	x1, [x29, 32]
	bl	search_gc_node
	str	x0, [x29, 56]
	ldr	x0, [x29, 56]
	cmp	x0, 0
	beq	.L1940
	mov	w0, 1
	b	.L1941
.L1940:
	ldr	x0, [x29, 40]
	add	x0, x0, 696
	bl	get_empty_gc_node
	str	x0, [x29, 56]
	ldr	x0, [x29, 56]
	cmp	x0, 0
	bne	.L1942
	mov	w0, 1
	b	.L1941
.L1942:
	ldr	x0, [x29, 56]
	ldr	x1, [x29, 32]
	str	x1, [x0, 8]
	ldr	x0, [x29, 56]
	ldrh	w1, [x29, 30]
	strh	w1, [x0, 2]
	ldr	x0, [x29, 56]
	str	xzr, [x0, 16]
	ldr	x0, [x29, 56]
	str	xzr, [x0, 24]
	ldr	x0, [x29, 40]
	add	x0, x0, 696
	ldr	x1, [x29, 56]
	bl	add_to_gc_list_tail
	mov	w0, 0
.L1941:
	ldp	x29, x30, [sp], 64
	ret
	.size	add_prio_gc, .-add_prio_gc
	.section	.rodata
	.align	3
.LC447:
	.string	"[ND]no block to prio gc\n"
	.align	3
.LC448:
	.string	"[ND]no need gc:%d, type:%d\n"
	.align	3
.LC449:
	.string	"[NE]do_prio_gc garbage_collect_first fail:%d, type:%d\n"
	.align	3
.LC450:
	.string	"[ND]prio gc fail1:%d, type:%d\n"
	.align	3
.LC451:
	.string	"[ND]prio gc fail2:%d, type:%d\n"
	.text
	.align	2
	.global	do_prio_gc
	.type	do_prio_gc, %function
do_prio_gc:
	stp	x29, x30, [sp, -64]!
	add	x29, sp, 0
	str	x0, [x29, 24]
	ldr	x0, [x29, 24]
	ldr	x0, [x0, 1040]
	cmp	x0, 0
	bne	.L1944
	mov	w0, 0
	b	.L1945
.L1944:
	ldr	x0, [x29, 24]
	add	x2, x0, 696
	ldr	x0, [x29, 24]
	ldr	x0, [x0, 1040]
	mov	x1, x0
	mov	x0, x2
	bl	del_from_gc_list
	str	x0, [x29, 56]
	ldr	x0, [x29, 56]
	ldr	x0, [x0, 8]
	mov	x1, x0
	ldr	x0, [x29, 24]
	bl	is_current_blcok
	cmp	w0, 1
	bne	.L1946
	ldr	x0, [x29, 24]
	add	x0, x0, 696
	ldr	x1, [x29, 56]
	bl	add_to_gc_list_tail
	mov	w0, 0
	b	.L1945
.L1946:
	ldr	x0, [x29, 56]
	ldr	x0, [x0, 8]
	cmp	x0, 0
	bne	.L1947
	adrp	x0, .LC447
	add	x0, x0, :lo12:.LC447
	bl	nand_print_dbg
	ldr	x0, [x29, 56]
	strh	wzr, [x0, 2]
	ldr	x0, [x29, 24]
	strh	wzr, [x0, 698]
	ldr	x0, [x29, 56]
	str	xzr, [x0, 8]
	mov	w0, 0
	b	.L1945
.L1947:
	ldr	x0, [x29, 56]
	ldr	x0, [x0, 8]
	mov	x1, x0
	ldr	x0, [x29, 24]
	bl	is_block_in_invalid_list
	cmp	w0, 0
	bne	.L1948
	ldr	x0, [x29, 56]
	ldr	x0, [x0, 8]
	ldrh	w0, [x0]
	mov	w1, w0
	ldr	x0, [x29, 56]
	ldrh	w0, [x0, 2]
	mov	w2, w0
	adrp	x0, .LC448
	add	x0, x0, :lo12:.LC448
	bl	nand_print_dbg
	ldr	x0, [x29, 56]
	strh	wzr, [x0, 2]
	ldr	x0, [x29, 24]
	strh	wzr, [x0, 698]
	ldr	x0, [x29, 56]
	str	xzr, [x0, 8]
	mov	w0, 0
	b	.L1945
.L1948:
	ldr	x0, [x29, 56]
	ldrh	w1, [x0, 2]
	ldr	x0, [x29, 24]
	strh	w1, [x0, 698]
	ldr	x0, [x29, 56]
	ldrh	w0, [x0, 2]
	cmp	w0, 5
	bne	.L1949
	ldr	x0, [x29, 56]
	ldr	x0, [x0, 8]
	mov	w2, 4
	mov	x1, x0
	ldr	x0, [x29, 24]
	bl	garbage_collect_first
	str	w0, [x29, 52]
	ldr	x0, [x29, 56]
	strh	wzr, [x0, 2]
	ldr	x0, [x29, 24]
	strh	wzr, [x0, 698]
	ldr	x0, [x29, 56]
	str	xzr, [x0, 8]
	mov	w0, 0
	b	.L1945
.L1949:
	ldr	x0, [x29, 56]
	ldr	x0, [x0, 8]
	mov	w2, -1
	mov	x1, x0
	ldr	x0, [x29, 24]
	bl	garbage_collect_first
	str	w0, [x29, 52]
	ldr	w0, [x29, 52]
	cmp	w0, 0
	beq	.L1950
	ldr	x0, [x29, 56]
	ldr	x0, [x0, 8]
	ldrh	w0, [x0]
	mov	w1, w0
	ldr	x0, [x29, 56]
	ldrh	w0, [x0, 2]
	mov	w2, w0
	adrp	x0, .LC449
	add	x0, x0, :lo12:.LC449
	bl	nand_print
	ldr	x0, [x29, 56]
	strh	wzr, [x0, 2]
	ldr	x0, [x29, 24]
	strh	wzr, [x0, 698]
	ldr	x0, [x29, 56]
	str	xzr, [x0, 8]
	mov	w0, 1
	b	.L1945
.L1950:
	ldr	x0, [x29, 56]
	ldr	x0, [x0, 8]
	mov	x1, x0
	ldr	x0, [x29, 24]
	bl	is_block_in_free_list
	cmp	w0, 0
	bne	.L1951
	ldr	x0, [x29, 56]
	ldr	x0, [x0, 8]
	ldrh	w0, [x0]
	mov	w1, w0
	ldr	x0, [x29, 56]
	ldrh	w0, [x0, 2]
	mov	w2, w0
	adrp	x0, .LC450
	add	x0, x0, :lo12:.LC450
	bl	nand_print_dbg
	ldr	x0, [x29, 56]
	strh	wzr, [x0, 2]
	ldr	x0, [x29, 24]
	strh	wzr, [x0, 698]
	ldr	x0, [x29, 56]
	str	xzr, [x0, 8]
	mov	w0, 0
	b	.L1945
.L1951:
	ldr	x0, [x29, 56]
	ldr	x0, [x0, 8]
	mov	x1, x0
	ldr	x0, [x29, 24]
	bl	out_phy_block_from_free_list_by_block
	str	x0, [x29, 40]
	ldr	x0, [x29, 40]
	cmp	x0, 0
	bne	.L1952
	ldr	x0, [x29, 56]
	ldr	x0, [x0, 8]
	ldrh	w0, [x0]
	mov	w1, w0
	ldr	x0, [x29, 56]
	ldrh	w0, [x0, 2]
	mov	w2, w0
	adrp	x0, .LC451
	add	x0, x0, :lo12:.LC451
	bl	nand_print_dbg
	ldr	x0, [x29, 56]
	strh	wzr, [x0, 2]
	ldr	x0, [x29, 24]
	strh	wzr, [x0, 698]
	ldr	x0, [x29, 56]
	str	xzr, [x0, 8]
	mov	w0, 0
	b	.L1945
.L1952:
	ldr	x0, [x29, 24]
	ldrh	w0, [x0, 698]
	cmp	w0, 1
	bne	.L1953
	ldr	x0, [x29, 40]
	ldrh	w0, [x0, 6]
	add	w0, w0, 1
	uxth	w0, w0
	mov	w2, w0
	ldr	x1, [x29, 40]
	ldr	x0, [x29, 24]
	bl	erase_block
	b	.L1954
.L1953:
	ldr	x0, [x29, 24]
	ldrh	w0, [x0, 698]
	cmp	w0, 2
	bne	.L1955
	ldr	x0, [x29, 40]
	ldrh	w0, [x0, 6]
	add	w0, w0, 1
	uxth	w0, w0
	mov	w2, w0
	ldr	x1, [x29, 40]
	ldr	x0, [x29, 24]
	bl	erase_block
	b	.L1954
.L1955:
	ldr	x0, [x29, 24]
	ldrh	w0, [x0, 698]
	cmp	w0, 4
	bne	.L1956
	ldr	x0, [x29, 40]
	ldrh	w0, [x0, 6]
	add	w0, w0, 1
	uxth	w0, w0
	mov	w2, w0
	ldr	x1, [x29, 40]
	ldr	x0, [x29, 24]
	bl	erase_block
	b	.L1954
.L1956:
	ldr	x0, [x29, 40]
	ldrh	w0, [x0, 6]
	add	w0, w0, 1
	uxth	w0, w0
	mov	w2, w0
	ldr	x1, [x29, 40]
	ldr	x0, [x29, 24]
	bl	erase_block
.L1954:
	ldr	x0, [x29, 56]
	strh	wzr, [x0, 2]
	ldr	x0, [x29, 24]
	strh	wzr, [x0, 698]
	ldr	x0, [x29, 56]
	str	xzr, [x0, 8]
	mov	w0, 0
.L1945:
	ldp	x29, x30, [sp], 64
	ret
	.size	do_prio_gc, .-do_prio_gc
	.align	2
	.global	prio_gc_all
	.type	prio_gc_all, %function
prio_gc_all:
	stp	x29, x30, [sp, -48]!
	add	x29, sp, 0
	str	x0, [x29, 24]
	str	wzr, [x29, 40]
	str	wzr, [x29, 44]
	b	.L1958
.L1959:
	ldr	x0, [x29, 24]
	bl	do_prio_gc
	mov	w1, w0
	ldr	w0, [x29, 40]
	orr	w0, w0, w1
	str	w0, [x29, 40]
	ldr	w0, [x29, 44]
	add	w0, w0, 1
	str	w0, [x29, 44]
.L1958:
	ldr	w0, [x29, 44]
	cmp	w0, 9
	bls	.L1959
	ldr	w0, [x29, 40]
	ldp	x29, x30, [sp], 48
	ret
	.size	prio_gc_all, .-prio_gc_all
	.section	.rodata
	.align	3
.LC452:
	.string	"[ND]swl: static WL need to do(%d : %d)\n"
	.align	3
.LC453:
	.string	"[ND]swl: min_ec_blk_no = %d,\n"
	.align	3
.LC454:
	.string	"[ND]swl: zone->blocks = %d)\n"
	.text
	.align	2
	.global	check_static_wear_leveling
	.type	check_static_wear_leveling, %function
check_static_wear_leveling:
	stp	x29, x30, [sp, -64]!
	add	x29, sp, 0
	str	x0, [x29, 24]
	str	xzr, [x29, 56]
	str	xzr, [x29, 48]
	mov	w0, -1
	strh	w0, [x29, 46]
	strh	wzr, [x29, 44]
	ldr	x0, [x29, 24]
	ldr	x0, [x0, 1064]
	cmp	x0, 0
	beq	.L1962
	mov	w0, 1
	b	.L1963
.L1962:
	ldr	x0, [x29, 24]
	ldr	x0, [x0, 352]
	cmp	x0, 0
	beq	.L1964
	ldr	x0, [x29, 24]
	ldr	x0, [x0, 304]
	cmp	x0, 0
	bne	.L1965
.L1964:
	mov	w0, 1
	b	.L1963
.L1965:
	ldr	x0, [x29, 24]
	ldr	x0, [x0, 528]
	ldr	w0, [x0, 8]
	ldr	x1, [x29, 24]
	ldrh	w1, [x1, 26]
	cmp	w0, w1
	bge	.L1966
	mov	w0, 1
	b	.L1963
.L1966:
	ldr	x0, [x29, 24]
	ldr	x0, [x0, 8]
	ldrh	w0, [x0, 16]
	mov	w1, w0
	ldr	x0, [x29, 24]
	bl	get_block_max_erase_time_from_free_list
	str	x0, [x29, 48]
	ldr	x0, [x29, 48]
	cmp	x0, 0
	bne	.L1967
	mov	w0, 1
	b	.L1963
.L1967:
	ldr	x0, [x29, 48]
	ldrh	w0, [x0, 6]
	strh	w0, [x29, 44]
	ldr	x0, [x29, 24]
	ldr	x0, [x0, 8]
	ldrh	w1, [x0, 16]
	mov	w0, 52429
	movk	w0, 0xcccc, lsl 16
	umull	x0, w1, w0
	lsr	x0, x0, 32
	lsr	w0, w0, 2
	uxth	w0, w0
	ldrh	w1, [x29, 44]
	cmp	w1, w0
	bcs	.L1968
	mov	w0, 1
	b	.L1963
.L1968:
	ldr	x0, [x29, 24]
	ldr	x0, [x0, 8]
	ldrh	w3, [x0, 16]
	ldr	x0, [x29, 24]
	ldr	x0, [x0, 528]
	ldr	w0, [x0, 8]
	ldr	x1, [x29, 24]
	ldrh	w1, [x1, 26]
	sub	w0, w0, w1
	mov	w2, w0
	mov	w1, w3
	ldr	x0, [x29, 24]
	bl	get_block_min_erase_time_from_invalid_list
	str	x0, [x29, 56]
	ldr	x0, [x29, 56]
	cmp	x0, 0
	bne	.L1969
	mov	w0, 1
	b	.L1963
.L1969:
	ldr	x0, [x29, 56]
	ldrh	w0, [x0, 6]
	strh	w0, [x29, 46]
	ldrh	w1, [x29, 44]
	ldrh	w0, [x29, 46]
	cmp	w1, w0
	bhi	.L1970
	mov	w0, 1
	b	.L1963
.L1970:
	ldr	x0, [x29, 24]
	ldr	x0, [x0, 8]
	ldrh	w0, [x0, 16]
	lsl	w2, w0, 1
	ldrh	w1, [x29, 44]
	mov	w0, w1
	lsl	w0, w0, 1
	add	w0, w0, w1
	cmp	w2, w0
	bge	.L1971
	ldr	x0, [x29, 24]
	ldr	x0, [x0, 8]
	ldrh	w1, [x0, 16]
	mov	w0, 43691
	movk	w0, 0xaaaa, lsl 16
	umull	x0, w1, w0
	lsr	x0, x0, 32
	lsr	w0, w0, 3
	uxth	w1, w0
	ldr	x0, [x29, 24]
	strh	w1, [x0, 1058]
	b	.L1972
.L1971:
	ldr	x0, [x29, 24]
	ldr	x0, [x0, 8]
	ldrh	w0, [x0, 16]
	mov	w1, w0
	ldrh	w0, [x29, 44]
	lsl	w0, w0, 1
	cmp	w1, w0
	bge	.L1973
	ldr	x0, [x29, 24]
	ldr	x0, [x0, 8]
	ldrh	w1, [x0, 16]
	mov	w0, 52429
	movk	w0, 0xcccc, lsl 16
	umull	x0, w1, w0
	lsr	x0, x0, 32
	lsr	w0, w0, 3
	uxth	w1, w0
	ldr	x0, [x29, 24]
	strh	w1, [x0, 1058]
	b	.L1972
.L1973:
	ldr	x0, [x29, 24]
	ldr	x0, [x0, 8]
	ldrh	w0, [x0, 16]
	mov	w2, w0
	ldrh	w1, [x29, 44]
	mov	w0, w1
	lsl	w0, w0, 1
	add	w0, w0, w1
	cmp	w2, w0
	bge	.L1974
	ldr	x0, [x29, 24]
	ldr	x0, [x0, 8]
	ldrh	w1, [x0, 16]
	mov	w0, 18725
	movk	w0, 0x2492, lsl 16
	umull	x0, w1, w0
	lsr	x0, x0, 32
	sub	w1, w1, w0
	lsr	w1, w1, 1
	add	w0, w1, w0
	lsr	w0, w0, 2
	uxth	w1, w0
	ldr	x0, [x29, 24]
	strh	w1, [x0, 1058]
	b	.L1972
.L1974:
	ldr	x0, [x29, 24]
	ldr	x0, [x0, 8]
	ldrh	w1, [x0, 16]
	mov	w0, 52429
	movk	w0, 0xcccc, lsl 16
	umull	x0, w1, w0
	lsr	x0, x0, 32
	lsr	w0, w0, 2
	uxth	w1, w0
	ldr	x0, [x29, 24]
	strh	w1, [x0, 1058]
.L1972:
	ldrh	w1, [x29, 44]
	ldrh	w0, [x29, 46]
	sub	w0, w1, w0
	ldr	x1, [x29, 24]
	ldrh	w1, [x1, 1058]
	cmp	w0, w1
	bge	.L1975
	mov	w0, 1
	b	.L1963
.L1975:
	ldrh	w1, [x29, 44]
	ldrh	w2, [x29, 46]
	adrp	x0, .LC452
	add	x0, x0, :lo12:.LC452
	bl	nand_print_dbg
	ldr	x0, [x29, 56]
	ldrh	w0, [x0]
	mov	w1, w0
	adrp	x0, .LC453
	add	x0, x0, :lo12:.LC453
	bl	nand_print_dbg
	ldr	x0, [x29, 24]
	ldrh	w0, [x0, 26]
	mov	w1, w0
	adrp	x0, .LC454
	add	x0, x0, :lo12:.LC454
	bl	nand_print_dbg
	ldr	x0, [x29, 24]
	ldr	x1, [x29, 56]
	str	x1, [x0, 1064]
	mov	w0, 0
.L1963:
	ldp	x29, x30, [sp], 64
	ret
	.size	check_static_wear_leveling, .-check_static_wear_leveling
	.section	.rodata
	.align	3
.LC455:
	.string	"[ND]swl: the block(%d) for static WL is not in invalid_list\n"
	.align	3
.LC456:
	.string	"[ND]swl: done_static_wear_leveling(min_ec) block:%d, erase_count:%d\n"
	.align	3
.LC457:
	.string	"[NE]swl: garbage_collect_first fail:%d\n"
	.align	3
.LC458:
	.string	"[NE]swl: last->check fail:%d\n"
	.text
	.align	2
	.global	done_static_wear_leveling
	.type	done_static_wear_leveling, %function
done_static_wear_leveling:
	stp	x29, x30, [sp, -48]!
	add	x29, sp, 0
	str	x0, [x29, 24]
	str	wzr, [x29, 44]
	ldr	x0, [x29, 24]
	ldr	x0, [x0, 1064]
	cmp	x0, 0
	bne	.L1977
	mov	w0, 2
	b	.L1978
.L1977:
	ldr	x0, [x29, 24]
	ldr	x0, [x0, 1064]
	mov	x1, x0
	ldr	x0, [x29, 24]
	bl	is_current_blcok
	cmp	w0, 1
	bne	.L1979
	ldr	x0, [x29, 24]
	str	xzr, [x0, 1064]
	mov	w0, 2
	b	.L1978
.L1979:
	ldr	x0, [x29, 24]
	ldr	x0, [x0, 1064]
	mov	x1, x0
	ldr	x0, [x29, 24]
	bl	is_block_in_invalid_list
	cmp	w0, 0
	bne	.L1980
	ldr	x0, [x29, 24]
	ldr	x0, [x0, 1064]
	ldrh	w0, [x0]
	mov	w1, w0
	adrp	x0, .LC455
	add	x0, x0, :lo12:.LC455
	bl	nand_print_dbg
	ldr	x0, [x29, 24]
	str	xzr, [x0, 1064]
	mov	w0, 2
	b	.L1978
.L1980:
	ldr	x0, [x29, 24]
	ldr	x0, [x0, 1064]
	ldrh	w0, [x0]
	mov	w1, w0
	ldr	x0, [x29, 24]
	ldr	x0, [x0, 1064]
	ldrh	w0, [x0, 6]
	mov	w2, w0
	adrp	x0, .LC456
	add	x0, x0, :lo12:.LC456
	bl	nand_print_dbg
	ldr	x0, [x29, 24]
	mov	w1, 1
	strh	w1, [x0, 1062]
	ldr	x0, [x29, 24]
	ldr	x0, [x0, 3440]
	ldr	w1, [x0, 128]
	add	w1, w1, 1
	str	w1, [x0, 128]
	ldr	x0, [x29, 24]
	bl	fill_no_use_data_to_current_block
	ldr	x0, [x29, 24]
	ldr	x0, [x0, 1064]
	mov	w2, -1
	mov	x1, x0
	ldr	x0, [x29, 24]
	bl	garbage_collect_first
	str	w0, [x29, 44]
	ldr	w0, [x29, 44]
	cmp	w0, 0
	beq	.L1981
	ldr	x0, [x29, 24]
	ldr	x0, [x0, 1064]
	ldrh	w0, [x0]
	mov	w1, w0
	adrp	x0, .LC457
	add	x0, x0, :lo12:.LC457
	bl	nand_print
	ldr	x0, [x29, 24]
	str	xzr, [x0, 1064]
	mov	w0, 1
	b	.L1978
.L1981:
	ldr	x0, [x29, 24]
	strh	wzr, [x0, 1062]
	ldr	x0, [x29, 24]
	ldr	x0, [x0, 1064]
	mov	x1, x0
	ldr	x0, [x29, 24]
	bl	is_block_in_free_list
	cmp	w0, 0
	bne	.L1982
	ldr	x0, [x29, 24]
	ldr	x0, [x0, 1064]
	ldrh	w0, [x0]
	mov	w1, w0
	adrp	x0, .LC458
	add	x0, x0, :lo12:.LC458
	bl	nand_print
	ldr	x0, [x29, 24]
	str	xzr, [x0, 1064]
	mov	w0, 0
	b	.L1978
.L1982:
	ldr	x0, [x29, 24]
	str	xzr, [x0, 1064]
	mov	w0, 0
.L1978:
	ldp	x29, x30, [sp], 48
	ret
	.size	done_static_wear_leveling, .-done_static_wear_leveling
	.align	2
	.global	do_static_wear_leveling
	.type	do_static_wear_leveling, %function
do_static_wear_leveling:
	stp	x29, x30, [sp, -32]!
	add	x29, sp, 0
	str	x0, [x29, 24]
	ldr	x0, [x29, 24]
	ldr	x0, [x0, 3448]
	ldrh	w0, [x0, 6]
	cmp	w0, 0
	bne	.L1984
	mov	w0, 0
	b	.L1985
.L1984:
	ldr	x0, [x29, 24]
	ldrh	w0, [x0, 684]
	cmp	w0, 0
	bne	.L1986
	mov	w0, 1
	b	.L1985
.L1986:
	ldr	x0, [x29, 24]
	bl	check_static_wear_leveling
	cmp	w0, 0
	bne	.L1987
	ldr	x0, [x29, 24]
	bl	done_static_wear_leveling
	b	.L1985
.L1987:
	mov	w0, 0
.L1985:
	ldp	x29, x30, [sp], 32
	ret
	.size	do_static_wear_leveling, .-do_static_wear_leveling
	.align	2
	.global	prio_gc_one
	.type	prio_gc_one, %function
prio_gc_one:
	stp	x29, x30, [sp, -48]!
	add	x29, sp, 0
	str	x0, [x29, 24]
	strh	w1, [x29, 22]
	str	w2, [x29, 16]
	ldrh	w1, [x29, 22]
	ldr	x0, [x29, 24]
	bl	get_phy_block_addr
	str	x0, [x29, 40]
	ldr	w0, [x29, 16]
	cmp	w0, 1
	bne	.L1989
	mov	w0, 1
	str	w0, [x29, 16]
	b	.L1990
.L1989:
	ldr	w0, [x29, 16]
	cmp	w0, 2
	bne	.L1991
	mov	w0, 2
	str	w0, [x29, 16]
	b	.L1990
.L1991:
	ldr	w0, [x29, 16]
	cmp	w0, 3
	bne	.L1992
	mov	w0, 3
	str	w0, [x29, 16]
	b	.L1990
.L1992:
	mov	w0, 3
	str	w0, [x29, 16]
.L1990:
	ldr	w0, [x29, 16]
	uxth	w0, w0
	mov	w2, w0
	ldr	x1, [x29, 40]
	ldr	x0, [x29, 24]
	bl	add_prio_gc
	mov	w0, 0
	ldp	x29, x30, [sp], 48
	ret
	.size	prio_gc_one, .-prio_gc_one
	.section	.rodata
	.align	3
.LC459:
	.string	"[ND]gc_one ok! from: %d [%d %d %d] @%s %d\n"
	.align	3
.LC460:
	.string	"[NE][NE]gc_one error!\n"
	.text
	.align	2
	.global	gc_one
	.type	gc_one, %function
gc_one:
	stp	x29, x30, [sp, -96]!
	add	x29, sp, 0
	stp	x19, x20, [sp, 16]
	stp	x21, x22, [sp, 32]
	str	x23, [sp, 48]
	str	x0, [x29, 72]
	strh	w1, [x29, 70]
	ldr	x0, [x29, 72]
	ldr	x0, [x0, 352]
	str	x0, [x29, 88]
	ldr	x0, [x29, 88]
	cmp	x0, 0
	bne	.L1995
	mov	w0, 8
	b	.L1996
.L1995:
	ldr	x0, [x29, 88]
	ldrh	w0, [x0, 4]
	ldrh	w1, [x29, 70]
	cmp	w1, w0
	bcs	.L1997
	mov	w2, -1
	mov	x1, 0
	ldr	x0, [x29, 72]
	bl	garbage_collect_first
	str	w0, [x29, 84]
	ldr	w0, [x29, 84]
	cmp	w0, 8
	bne	.L1998
	mov	w0, 8
	b	.L1996
.L1998:
	ldr	w0, [x29, 84]
	cmp	w0, 0
	bne	.L1999
	adrp	x0, gc_src_type
	add	x0, x0, :lo12:gc_src_type
	ldr	w19, [x0]
	ldr	x0, [x29, 72]
	ldrh	w0, [x0, 30]
	mov	w20, w0
	ldr	x0, [x29, 72]
	ldrh	w0, [x0, 680]
	mov	w21, w0
	ldr	x0, [x29, 72]
	ldrh	w0, [x0, 682]
	mov	w22, w0
	bl	nand_get_cur_task_name
	mov	x23, x0
	bl	nand_get_cur_task_pid
	mov	w1, w0
	adrp	x0, .LC459
	add	x0, x0, :lo12:.LC459
	mov	w6, w1
	mov	x5, x23
	mov	w4, w22
	mov	w3, w21
	mov	w2, w20
	mov	w1, w19
	bl	nand_print_dbg
	mov	w0, 0
	b	.L1996
.L1999:
	adrp	x0, .LC460
	add	x0, x0, :lo12:.LC460
	bl	nand_print
	mov	w0, 1
	b	.L1996
.L1997:
	mov	w0, 8
.L1996:
	ldp	x19, x20, [sp, 16]
	ldp	x21, x22, [sp, 32]
	ldr	x23, [sp, 48]
	ldp	x29, x30, [sp], 96
	ret
	.size	gc_one, .-gc_one
	.align	2
	.global	gc_all_cnt
	.type	gc_all_cnt, %function
gc_all_cnt:
	stp	x29, x30, [sp, -48]!
	add	x29, sp, 0
	str	x0, [x29, 24]
	strh	w1, [x29, 22]
	str	w2, [x29, 16]
	adrp	x0, gc_src_type
	add	x0, x0, :lo12:gc_src_type
	mov	w1, 255
	str	w1, [x0]
	b	.L2001
.L2003:
	ldrh	w1, [x29, 22]
	ldr	x0, [x29, 24]
	bl	gc_one
	str	w0, [x29, 44]
	ldr	w0, [x29, 44]
	cmp	w0, 8
	beq	.L2005
.L2001:
	ldr	w0, [x29, 16]
	sub	w1, w0, #1
	str	w1, [x29, 16]
	cmp	w0, 0
	bne	.L2003
	b	.L2002
.L2005:
	nop
.L2002:
	mov	w0, 0
	ldp	x29, x30, [sp], 48
	ret
	.size	gc_all_cnt, .-gc_all_cnt
	.align	2
	.global	gc_all
	.type	gc_all, %function
gc_all:
	stp	x29, x30, [sp, -48]!
	add	x29, sp, 0
	str	x0, [x29, 24]
	strh	w1, [x29, 22]
	adrp	x0, gc_src_type
	add	x0, x0, :lo12:gc_src_type
	mov	w1, 255
	str	w1, [x0]
.L2009:
	ldrh	w1, [x29, 22]
	ldr	x0, [x29, 24]
	bl	gc_one
	str	w0, [x29, 44]
	ldr	w0, [x29, 44]
	cmp	w0, 0
	bne	.L2012
	b	.L2009
.L2012:
	nop
	mov	w0, 0
	ldp	x29, x30, [sp], 48
	ret
	.size	gc_all, .-gc_all
	.align	2
	.global	gc_all_enhance
	.type	gc_all_enhance, %function
gc_all_enhance:
	stp	x29, x30, [sp, -64]!
	add	x29, sp, 0
	str	x0, [x29, 24]
	adrp	x0, phyinfo
	add	x0, x0, :lo12:phyinfo
	str	x0, [x29, 56]
	ldr	x0, [x29, 24]
	ldrh	w0, [x0, 30]
	str	w0, [x29, 52]
	ldr	x0, [x29, 24]
	ldrh	w0, [x0, 680]
	mov	w1, w0
	ldr	w0, [x29, 52]
	cmp	w1, w0
	bls	.L2014
	mov	w0, 0
	b	.L2015
.L2014:
	ldr	x0, [x29, 24]
	ldrh	w0, [x0, 682]
	mov	w1, w0
	mov	w0, w1
	lsl	w0, w0, 1
	add	w0, w0, w1
	mov	w1, w0
	ldr	w0, [x29, 52]
	cmp	w1, w0
	bcs	.L2016
	mov	w0, 0
	b	.L2015
.L2016:
	ldr	x0, [x29, 56]
	ldr	w0, [x0, 4]
	ldr	x1, [x29, 56]
	ldrh	w1, [x1, 8]
	mul	w0, w0, w1
	uxtw	x0, w0
	str	x0, [x29, 40]
	ldr	x0, [x29, 56]
	ldrb	w0, [x0, 10]
	uxtb	x0, w0
	ldr	x1, [x29, 40]
	mul	x0, x1, x0
	str	x0, [x29, 40]
	ldr	x0, [x29, 40]
	lsr	x0, x0, 11
	str	x0, [x29, 40]
	ldr	x0, [x29, 40]
	cmp	x0, 128
	bls	.L2017
	mov	w0, 0
	b	.L2015
.L2017:
	ldr	x0, [x29, 24]
	ldr	x0, [x0, 8]
	ldrh	w0, [x0, 20]
	lsr	w0, w0, 1
	uxth	w0, w0
	mov	w1, w0
	ldr	x0, [x29, 24]
	bl	gc_all
.L2015:
	ldp	x29, x30, [sp], 64
	ret
	.size	gc_all_enhance, .-gc_all_enhance
	.align	2
	.global	gc_all_base_on_free_blks
	.type	gc_all_base_on_free_blks, %function
gc_all_base_on_free_blks:
	stp	x29, x30, [sp, -80]!
	add	x29, sp, 0
	str	x0, [x29, 24]
	adrp	x0, phyinfo
	add	x0, x0, :lo12:phyinfo
	str	x0, [x29, 72]
	ldr	x0, [x29, 24]
	ldr	x0, [x0, 8]
	str	x0, [x29, 64]
	ldr	x0, [x29, 24]
	ldrh	w0, [x0, 30]
	str	w0, [x29, 60]
	ldr	x0, [x29, 24]
	ldrh	w0, [x0, 680]
	mov	w1, w0
	ldr	w0, [x29, 60]
	cmp	w1, w0
	bls	.L2019
	mov	w0, 0
	b	.L2020
.L2019:
	ldr	x0, [x29, 24]
	ldrh	w0, [x0, 682]
	mov	w1, w0
	mov	w0, w1
	lsl	w0, w0, 1
	add	w0, w0, w1
	mov	w1, w0
	ldr	w0, [x29, 60]
	cmp	w1, w0
	bcs	.L2021
	mov	w0, 0
	b	.L2020
.L2021:
	ldr	x0, [x29, 72]
	ldr	w0, [x0, 4]
	ldr	x1, [x29, 72]
	ldrh	w1, [x1, 8]
	mul	w0, w0, w1
	uxtw	x0, w0
	str	x0, [x29, 48]
	ldr	x0, [x29, 72]
	ldrb	w0, [x0, 10]
	uxtb	x0, w0
	ldr	x1, [x29, 48]
	mul	x0, x1, x0
	str	x0, [x29, 48]
	ldr	x0, [x29, 48]
	lsr	x0, x0, 11
	str	x0, [x29, 48]
	ldr	x0, [x29, 48]
	cmp	x0, 128
	bls	.L2022
	mov	w0, 0
	b	.L2020
.L2022:
	ldr	x0, [x29, 64]
	ldrh	w0, [x0, 20]
	lsr	w0, w0, 1
	uxth	w0, w0
	sub	w0, w0, #3
	str	w0, [x29, 44]
	ldr	x0, [x29, 24]
	ldrh	w0, [x0, 680]
	mov	w1, w0
	ldr	w0, [x29, 60]
	sub	w0, w0, w1
	ldr	w1, [x29, 44]
	mul	w0, w1, w0
	str	w0, [x29, 44]
	ldr	x0, [x29, 24]
	ldrh	w0, [x0, 682]
	mov	w1, w0
	mov	w0, w1
	lsl	w0, w0, 1
	add	w0, w0, w1
	ldr	x1, [x29, 24]
	ldrh	w1, [x1, 680]
	sub	w0, w0, w1
	mov	w1, w0
	ldr	w0, [x29, 44]
	udiv	w0, w0, w1
	str	w0, [x29, 44]
	ldr	x0, [x29, 64]
	ldrh	w0, [x0, 20]
	lsr	w0, w0, 1
	uxth	w0, w0
	mov	w1, w0
	ldr	w0, [x29, 44]
	add	w0, w0, w1
	str	w0, [x29, 44]
	ldr	w0, [x29, 44]
	uxth	w0, w0
	mov	w2, 3
	mov	w1, w0
	ldr	x0, [x29, 24]
	bl	gc_all_cnt
.L2020:
	ldp	x29, x30, [sp], 80
	ret
	.size	gc_all_base_on_free_blks, .-gc_all_base_on_free_blks
	.section	.rodata
	.align	3
.LC461:
	.string	"[ND][NE] gc_win_block gc_one error \n"
	.text
	.align	2
	.global	gc_win_block
	.type	gc_win_block, %function
gc_win_block:
	stp	x29, x30, [sp, -48]!
	add	x29, sp, 0
	str	x0, [x29, 24]
	strh	w1, [x29, 22]
	str	wzr, [x29, 44]
	ldrh	w0, [x29, 22]
	ldr	x1, [x29, 24]
	ldr	x1, [x1, 8]
	ldrh	w1, [x1, 20]
	mul	w0, w0, w1
	str	w0, [x29, 40]
	b	.L2024
.L2028:
	ldr	x0, [x29, 24]
	ldr	x0, [x0, 352]
	cmp	x0, 0
	bne	.L2025
	mov	w0, 0
	b	.L2026
.L2025:
	ldr	x0, [x29, 24]
	ldr	x0, [x0, 352]
	ldrh	w0, [x0, 4]
	mov	w1, w0
	ldr	w0, [x29, 44]
	add	w0, w0, w1
	str	w0, [x29, 44]
	ldr	w0, [x29, 44]
	sub	w0, w0, #1
	str	w0, [x29, 44]
	mov	w1, 1
	ldr	x0, [x29, 24]
	bl	gc_one
	str	w0, [x29, 36]
	ldr	w0, [x29, 36]
	cmp	w0, 0
	beq	.L2024
	ldr	w0, [x29, 36]
	cmp	w0, 1
	bne	.L2027
	adrp	x0, .LC461
	add	x0, x0, :lo12:.LC461
	bl	nand_print_dbg
.L2027:
	ldr	w0, [x29, 36]
	b	.L2026
.L2024:
	ldr	w1, [x29, 44]
	ldr	w0, [x29, 40]
	cmp	w1, w0
	blt	.L2028
	mov	w0, 0
.L2026:
	ldp	x29, x30, [sp], 48
	ret
	.size	gc_win_block, .-gc_win_block
	.align	2
	.global	get_total_invalid_page
	.type	get_total_invalid_page, %function
get_total_invalid_page:
	sub	sp, sp, #32
	str	x0, [sp, 8]
	str	wzr, [sp, 20]
	ldr	x0, [sp, 8]
	add	x0, x0, 336
	str	x0, [sp, 24]
	b	.L2030
.L2031:
	ldr	x0, [sp, 24]
	ldr	x0, [x0, 16]
	ldrh	w0, [x0, 4]
	mov	w1, w0
	ldr	w0, [sp, 20]
	add	w0, w0, w1
	str	w0, [sp, 20]
	ldr	x0, [sp, 24]
	ldr	x0, [x0, 16]
	str	x0, [sp, 24]
.L2030:
	ldr	x0, [sp, 24]
	ldr	x0, [x0, 16]
	cmp	x0, 0
	bne	.L2031
	ldr	w0, [sp, 20]
	add	sp, sp, 32
	ret
	.size	get_total_invalid_page, .-get_total_invalid_page
	.align	2
	.global	is_current_blcok
	.type	is_current_blcok, %function
is_current_blcok:
	sub	sp, sp, #32
	str	x0, [sp, 8]
	str	x1, [sp]
	str	wzr, [sp, 28]
	ldr	x0, [sp, 8]
	ldrb	w0, [x0, 25]
	and	w0, w0, 1
	cmp	w0, 0
	beq	.L2034
	ldr	x0, [sp, 8]
	ldr	x1, [x0, 592]
	ldr	x0, [sp]
	cmp	x1, x0
	beq	.L2035
	ldr	x0, [sp, 8]
	ldr	x1, [x0, 528]
	ldr	x0, [sp]
	cmp	x1, x0
	bne	.L2037
.L2035:
	mov	w0, 1
	str	w0, [sp, 28]
	b	.L2037
.L2034:
	ldr	x0, [sp, 8]
	ldr	x1, [x0, 528]
	ldr	x0, [sp]
	cmp	x1, x0
	bne	.L2037
	mov	w0, 1
	str	w0, [sp, 28]
.L2037:
	ldr	w0, [sp, 28]
	add	sp, sp, 32
	ret
	.size	is_current_blcok, .-is_current_blcok
	.align	2
	.global	static_wear_leveling
	.type	static_wear_leveling, %function
static_wear_leveling:
	stp	x29, x30, [sp, -48]!
	add	x29, sp, 0
	str	x0, [x29, 24]
	ldr	x0, [x29, 24]
	ldrh	w1, [x0, 30]
	ldr	x0, [x29, 24]
	ldrh	w0, [x0, 680]
	cmp	w1, w0
	bhi	.L2040
	mov	w0, 0
	b	.L2041
.L2040:
	ldr	x0, [x29, 24]
	ldr	w0, [x0, 668]
	cmp	w0, 19
	bls	.L2042
	ldr	x0, [x29, 24]
	ldr	x0, [x0, 3440]
	ldr	w1, [x0, 180]
	add	w1, w1, 1
	str	w1, [x0, 180]
	ldr	x0, [x29, 24]
	bl	get_block_min_count_from_invalid_list
	str	x0, [x29, 40]
	mov	w2, 5
	ldr	x1, [x29, 40]
	ldr	x0, [x29, 24]
	bl	add_prio_gc
	str	w0, [x29, 36]
	ldr	w0, [x29, 36]
	cmp	w0, 0
	bne	.L2043
	ldr	x0, [x29, 24]
	ldr	w0, [x0, 672]
	add	w1, w0, 4
	ldr	x0, [x29, 24]
	str	w1, [x0, 672]
.L2043:
	ldr	x0, [x29, 24]
	ldr	w0, [x0, 672]
	ldr	x1, [x29, 24]
	ldr	x1, [x1, 8]
	ldrh	w1, [x1, 20]
	cmp	w0, w1
	bcc	.L2042
	ldr	x0, [x29, 24]
	str	wzr, [x0, 668]
	ldr	x0, [x29, 24]
	str	wzr, [x0, 672]
.L2042:
	mov	w0, 0
.L2041:
	ldp	x29, x30, [sp], 48
	ret
	.size	static_wear_leveling, .-static_wear_leveling
	.section	.rodata
	.align	3
.LC462:
	.string	"[ND]not power on gc [%d, %d, %d], [%d %d %d]!\n"
	.align	3
.LC463:
	.string	"[ND]emergency power on gc %d !\n"
	.align	3
.LC464:
	.string	"[ND]power on gc %d!\n"
	.text
	.align	2
	.global	power_on_gc
	.type	power_on_gc, %function
power_on_gc:
	stp	x29, x30, [sp, -48]!
	add	x29, sp, 0
	str	x0, [x29, 24]
	ldr	x0, [x29, 24]
	ldrh	w1, [x0, 30]
	ldr	x0, [x29, 24]
	ldrh	w0, [x0, 680]
	cmp	w1, w0
	bcc	.L2045
	ldr	x0, [x29, 24]
	ldrh	w0, [x0, 30]
	mov	w7, w0
	ldr	x0, [x29, 24]
	ldrh	w0, [x0, 680]
	mov	w8, w0
	ldr	x0, [x29, 24]
	ldrh	w0, [x0, 682]
	mov	w3, w0
	ldr	x0, [x29, 24]
	ldrh	w0, [x0, 26]
	mov	w4, w0
	ldr	x0, [x29, 24]
	ldr	w0, [x0, 32]
	ldr	x1, [x29, 24]
	ldr	x1, [x1, 8]
	ldrb	w1, [x1, 8]
	udiv	w0, w0, w1
	ldr	x1, [x29, 24]
	ldr	x1, [x1, 8]
	ldrh	w1, [x1, 20]
	udiv	w2, w0, w1
	ldr	x0, [x29, 24]
	ldr	w0, [x0, 40]
	ldr	x1, [x29, 24]
	ldr	x1, [x1, 8]
	ldrb	w1, [x1, 8]
	udiv	w0, w0, w1
	ldr	x1, [x29, 24]
	ldr	x1, [x1, 8]
	ldrh	w1, [x1, 20]
	udiv	w1, w0, w1
	adrp	x0, .LC462
	add	x0, x0, :lo12:.LC462
	mov	w6, w1
	mov	w5, w2
	mov	w2, w8
	mov	w1, w7
	bl	nand_print_dbg
	mov	w0, 0
	b	.L2046
.L2045:
	adrp	x0, gc_src_type
	add	x0, x0, :lo12:gc_src_type
	mov	w1, 2
	str	w1, [x0]
	strh	wzr, [x29, 46]
	ldr	x0, [x29, 24]
	ldr	x0, [x0, 8]
	ldrh	w0, [x0, 20]
	sub	w0, w0, #10
	strh	w0, [x29, 44]
	b	.L2047
.L2050:
	ldrh	w1, [x29, 44]
	ldr	x0, [x29, 24]
	bl	gc_one
	str	w0, [x29, 40]
	ldr	w0, [x29, 40]
	cmp	w0, 8
	beq	.L2055
	ldrh	w0, [x29, 46]
	add	w0, w0, 1
	strh	w0, [x29, 46]
.L2047:
	ldr	x0, [x29, 24]
	ldrh	w1, [x0, 30]
	ldr	x0, [x29, 24]
	ldrh	w0, [x0, 680]
	cmp	w1, w0
	bcc	.L2050
	b	.L2049
.L2055:
	nop
.L2049:
	ldr	x0, [x29, 24]
	ldr	x0, [x0, 8]
	ldrh	w0, [x0, 20]
	lsr	w0, w0, 3
	strh	w0, [x29, 44]
	b	.L2051
.L2054:
	ldr	x0, [x29, 24]
	ldrh	w0, [x0, 30]
	mov	w1, w0
	adrp	x0, .LC463
	add	x0, x0, :lo12:.LC463
	bl	nand_print_dbg
	ldrh	w1, [x29, 44]
	ldr	x0, [x29, 24]
	bl	gc_one
	str	w0, [x29, 40]
	ldr	w0, [x29, 40]
	cmp	w0, 8
	beq	.L2056
	ldrh	w0, [x29, 46]
	add	w0, w0, 1
	strh	w0, [x29, 46]
.L2051:
	ldr	x0, [x29, 24]
	ldrh	w0, [x0, 30]
	cmp	w0, 4
	bls	.L2054
	b	.L2053
.L2056:
	nop
.L2053:
	ldrh	w1, [x29, 46]
	adrp	x0, .LC464
	add	x0, x0, :lo12:.LC464
	bl	nand_print_dbg
	mov	w0, 0
.L2046:
	ldp	x29, x30, [sp], 48
	ret
	.size	power_on_gc, .-power_on_gc
	.align	2
	.global	all_time_gc
	.type	all_time_gc, %function
all_time_gc:
	stp	x29, x30, [sp, -48]!
	add	x29, sp, 0
	str	x0, [x29, 24]
	ldr	x0, [x29, 24]
	ldr	x0, [x0, 8]
	ldrh	w0, [x0, 20]
	sub	w0, w0, #3
	strh	w0, [x29, 46]
	ldr	x0, [x29, 24]
	ldrh	w1, [x0, 30]
	ldr	x0, [x29, 24]
	ldrh	w0, [x0, 682]
	cmp	w1, w0
	bcs	.L2058
	ldr	x0, [x29, 24]
	ldrh	w0, [x0, 684]
	cmp	w0, 0
	beq	.L2058
	adrp	x0, gc_src_type
	add	x0, x0, :lo12:gc_src_type
	mov	w1, 1
	str	w1, [x0]
	ldrh	w1, [x29, 46]
	ldr	x0, [x29, 24]
	bl	gc_one
.L2058:
	mov	w0, 0
	ldp	x29, x30, [sp], 48
	ret
	.size	all_time_gc, .-all_time_gc
	.section	.rodata
	.align	3
.LC465:
	.string	"[ND]%s %p %d %d %d %\n"
	.align	3
.LC466:
	.string	"[NI]shutdown_on_gc end! [%d %d %d]\n"
	.align	3
.LC467:
	.string	"[ND]shutdown_on_gc one ok! [%d %d %d]\n"
	.text
	.align	2
	.global	shutdown_on_gc
	.type	shutdown_on_gc, %function
shutdown_on_gc:
	stp	x29, x30, [sp, -48]!
	add	x29, sp, 0
	str	x0, [x29, 24]
	ldr	x0, [x29, 24]
	ldr	x0, [x0, 352]
	str	x0, [x29, 40]
	adrp	x0, gc_src_type
	add	x0, x0, :lo12:gc_src_type
	mov	w1, 4
	str	w1, [x0]
	ldr	x0, [x29, 24]
	ldrh	w0, [x0, 684]
	mov	w2, w0
	ldr	x0, [x29, 24]
	ldrh	w0, [x0, 30]
	mov	w3, w0
	ldr	x0, [x29, 24]
	ldrh	w0, [x0, 680]
	mov	w4, w0
	ldr	x0, [x29, 24]
	ldrh	w0, [x0, 682]
	mov	w5, w0
	adrp	x0, __func__.8331
	add	x1, x0, :lo12:__func__.8331
	adrp	x0, .LC465
	add	x0, x0, :lo12:.LC465
	mov	w6, w5
	mov	w5, w4
	mov	w4, w3
	mov	w3, w2
	ldr	x2, [x29, 40]
	bl	nand_print_dbg
	b	.L2061
.L2064:
	ldr	x0, [x29, 24]
	ldr	x0, [x0, 352]
	str	x0, [x29, 40]
	ldr	x0, [x29, 40]
	cmp	x0, 0
	bne	.L2062
	ldr	x0, [x29, 24]
	ldrh	w0, [x0, 30]
	mov	w1, w0
	ldr	x0, [x29, 24]
	ldrh	w0, [x0, 680]
	mov	w2, w0
	ldr	x0, [x29, 24]
	ldrh	w0, [x0, 682]
	mov	w3, w0
	adrp	x0, .LC466
	add	x0, x0, :lo12:.LC466
	bl	nand_print
	b	.L2063
.L2062:
	ldr	x0, [x29, 24]
	ldrh	w0, [x0, 684]
	cmp	w0, 0
	beq	.L2061
	mov	w1, 0
	ldr	x0, [x29, 24]
	bl	gc_one
	ldr	x0, [x29, 24]
	ldrh	w0, [x0, 30]
	mov	w1, w0
	ldr	x0, [x29, 24]
	ldrh	w0, [x0, 680]
	mov	w2, w0
	ldr	x0, [x29, 24]
	ldrh	w0, [x0, 682]
	mov	w3, w0
	adrp	x0, .LC467
	add	x0, x0, :lo12:.LC467
	bl	nand_print_dbg
.L2061:
	ldr	x0, [x29, 24]
	ldrh	w1, [x0, 30]
	ldr	x0, [x29, 24]
	ldrh	w0, [x0, 682]
	cmp	w1, w0
	bls	.L2064
.L2063:
	mov	w0, 0
	ldp	x29, x30, [sp], 48
	ret
	.size	shutdown_on_gc, .-shutdown_on_gc
	.section	.rodata
	.align	3
.LC468:
	.string	"%d %d %d %d %d %d/%d %d/%d\n"
	.align	3
.LC469:
	.string	"%08d:%04d "
	.text
	.align	2
	.global	nftl_get_gc_info
	.type	nftl_get_gc_info, %function
nftl_get_gc_info:
	sub	sp, sp, #128
	stp	x29, x30, [sp, 32]
	add	x29, sp, 32
	str	x0, [x29, 40]
	str	x1, [x29, 32]
	str	w2, [x29, 28]
	str	wzr, [x29, 92]
	ldr	x0, [x29, 40]
	ldr	x0, [x0, 8]
	str	x0, [x29, 56]
	str	wzr, [x29, 76]
	ldr	w0, [x29, 76]
	str	w0, [x29, 80]
	ldr	w0, [x29, 80]
	str	w0, [x29, 88]
	ldr	w0, [x29, 88]
	str	w0, [x29, 84]
	ldr	x0, [x29, 40]
	ldr	x0, [x0, 352]
	str	x0, [x29, 64]
	b	.L2067
.L2069:
	ldr	x0, [x29, 64]
	ldrh	w1, [x0, 4]
	ldr	x0, [x29, 56]
	ldrh	w0, [x0, 20]
	lsr	w0, w0, 1
	uxth	w0, w0
	cmp	w1, w0
	bls	.L2068
	ldr	x0, [x29, 64]
	ldrh	w0, [x0, 4]
	mov	w1, w0
	ldr	w0, [x29, 80]
	add	w0, w0, w1
	str	w0, [x29, 80]
	ldr	w0, [x29, 88]
	add	w0, w0, 1
	str	w0, [x29, 88]
.L2068:
	ldr	w0, [x29, 84]
	add	w0, w0, 1
	str	w0, [x29, 84]
	ldr	x0, [x29, 64]
	ldrh	w0, [x0, 4]
	mov	w1, w0
	ldr	w0, [x29, 76]
	add	w0, w0, w1
	str	w0, [x29, 76]
	ldr	x0, [x29, 64]
	ldr	x0, [x0, 16]
	str	x0, [x29, 64]
.L2067:
	ldr	x0, [x29, 64]
	cmp	x0, 0
	bne	.L2069
	ldrsw	x0, [x29, 92]
	ldr	x1, [x29, 32]
	add	x8, x1, x0
	ldr	w1, [x29, 28]
	ldr	w0, [x29, 92]
	sub	w0, w1, w0
	mov	w9, w0
	ldr	x0, [x29, 40]
	ldrh	w0, [x0, 30]
	mov	w10, w0
	ldr	x0, [x29, 40]
	ldrh	w0, [x0, 680]
	mov	w4, w0
	ldr	x0, [x29, 40]
	ldrh	w0, [x0, 682]
	mov	w5, w0
	ldr	x0, [x29, 40]
	ldr	x0, [x0, 3440]
	ldr	w2, [x0, 120]
	ldr	x0, [x29, 40]
	ldr	x0, [x0, 3440]
	ldr	w3, [x0, 124]
	adrp	x0, .LC468
	add	x1, x0, :lo12:.LC468
	ldr	w0, [x29, 76]
	str	w0, [sp, 24]
	ldr	w0, [x29, 80]
	str	w0, [sp, 16]
	ldr	w0, [x29, 84]
	str	w0, [sp, 8]
	ldr	w0, [x29, 88]
	str	w0, [sp]
	mov	w7, w3
	mov	w6, w2
	mov	w3, w10
	mov	x2, x1
	mov	w1, w9
	mov	x0, x8
	bl	nand_snprint
	str	w0, [x29, 92]
	str	wzr, [x29, 88]
	ldr	x0, [x29, 40]
	ldr	x0, [x0, 352]
	str	x0, [x29, 64]
	b	.L2070
.L2073:
	ldr	x0, [x29, 64]
	cmp	x0, 0
	beq	.L2071
	ldr	x0, [x29, 64]
	ldrh	w0, [x0, 4]
	cmp	w0, 0
	beq	.L2071
	ldrsw	x0, [x29, 92]
	ldr	x1, [x29, 32]
	add	x5, x1, x0
	ldr	w1, [x29, 28]
	ldr	w0, [x29, 92]
	sub	w0, w1, w0
	mov	w1, w0
	ldr	x0, [x29, 64]
	ldrh	w0, [x0]
	mov	w2, w0
	ldr	x0, [x29, 64]
	ldrh	w0, [x0, 4]
	mov	w3, w0
	adrp	x0, .LC469
	add	x0, x0, :lo12:.LC469
	mov	w4, w3
	mov	w3, w2
	mov	x2, x0
	mov	x0, x5
	bl	nand_snprint
	mov	w1, w0
	ldr	w0, [x29, 92]
	add	w0, w0, w1
	str	w0, [x29, 92]
	ldr	w0, [x29, 88]
	add	w0, w0, 1
	str	w0, [x29, 88]
	ldr	w0, [x29, 88]
	and	w0, w0, 7
	cmp	w0, 0
	bne	.L2072
	ldrsw	x0, [x29, 92]
	sub	x0, x0, #1
	ldr	x1, [x29, 32]
	add	x0, x1, x0
	mov	w1, 10
	strb	w1, [x0]
.L2072:
	ldr	x0, [x29, 64]
	ldr	x0, [x0, 16]
	str	x0, [x29, 64]
.L2070:
	ldr	w1, [x29, 28]
	ldr	w0, [x29, 92]
	sub	w0, w1, w0
	cmp	w0, 1
	bne	.L2073
.L2071:
	ldrsw	x0, [x29, 92]
	sub	x0, x0, #1
	ldr	x1, [x29, 32]
	add	x0, x1, x0
	mov	w1, 10
	strb	w1, [x0]
	ldr	w0, [x29, 92]
	add	sp, x29, 0
	ldp	x29, x30, [sp], 96
	ret
	.size	nftl_get_gc_info, .-nftl_get_gc_info
	.align	2
	.global	_nftl_nand_copy_page
	.type	_nftl_nand_copy_page, %function
_nftl_nand_copy_page:
	stp	x29, x30, [sp, -144]!
	add	x29, sp, 0
	str	x0, [x29, 56]
	str	x1, [x29, 48]
	str	x2, [x29, 40]
	str	x3, [x29, 32]
	strh	w4, [x29, 30]
	ldr	x0, [x29, 48]
	ldrh	w1, [x0]
	add	x0, x29, 88
	mov	x5, 0
	mov	x4, 0
	mov	w3, 0
	ldrh	w2, [x29, 30]
	bl	set_physic_op_par
	ldr	x0, [x29, 40]
	ldrh	w1, [x0]
	add	x0, x29, 64
	mov	x5, 0
	mov	x4, 0
	mov	w3, 0
	ldrh	w2, [x29, 30]
	bl	set_physic_op_par
	str	wzr, [x29, 136]
	ldr	x0, [x29, 56]
	ldr	x0, [x0, 8]
	ldrh	w0, [x0, 28]
	strh	w0, [x29, 92]
	ldr	x0, [x29, 56]
	ldr	x0, [x0, 8]
	ldrh	w0, [x0, 28]
	strh	w0, [x29, 68]
	ldr	x0, [x29, 32]
	str	x0, [x29, 96]
	ldr	x0, [x29, 32]
	str	x0, [x29, 72]
	add	x0, x29, 112
	str	x0, [x29, 104]
	add	x0, x29, 112
	str	x0, [x29, 80]
	ldr	x0, [x29, 56]
	ldr	x2, [x0, 3464]
	add	x0, x29, 88
	mov	x1, x0
	ldr	x0, [x29, 56]
	blr	x2
	str	w0, [x29, 140]
	ldr	w0, [x29, 140]
	cmp	w0, 10
	bne	.L2076
	str	wzr, [x29, 140]
.L2076:
	add	x0, x29, 112
	bl	get_special_data_from_oob
	str	w0, [x29, 132]
	ldr	x0, [x29, 48]
	ldr	w0, [x0, 8]
	mov	w2, w0
	ldr	x0, [x29, 40]
	ldrh	w0, [x0, 6]
	add	w0, w0, 1
	uxth	w1, w0
	add	x0, x29, 112
	mov	w4, w1
	mov	w3, w2
	ldr	w2, [x29, 132]
	mov	x1, x0
	ldr	x0, [x29, 56]
	bl	set_oob_special_page
	ldr	x0, [x29, 56]
	ldr	x2, [x0, 3472]
	add	x0, x29, 64
	mov	x1, x0
	ldr	x0, [x29, 56]
	blr	x2
	str	w0, [x29, 128]
	ldr	w0, [x29, 128]
	lsl	w1, w0, 8
	ldr	w0, [x29, 140]
	orr	w0, w1, w0
	str	w0, [x29, 136]
	ldr	w0, [x29, 136]
	ldp	x29, x30, [sp], 144
	ret
	.size	_nftl_nand_copy_page, .-_nftl_nand_copy_page
	.align	2
	.global	set_spare_data
	.type	set_spare_data, %function
set_spare_data:
	sub	sp, sp, #32
	str	x0, [sp, 8]
	str	w1, [sp, 4]
	strb	w2, [sp, 3]
	strb	w3, [sp, 2]
	str	wzr, [sp, 28]
	b	.L2079
.L2080:
	ldrb	w1, [sp, 2]
	ldr	w0, [sp, 28]
	sub	w0, w1, w0
	str	w0, [sp, 24]
	ldr	w0, [sp, 24]
	sub	w0, w0, #1
	str	w0, [sp, 24]
	ldr	w0, [sp, 24]
	lsl	w0, w0, 3
	str	w0, [sp, 24]
	ldrb	w1, [sp, 3]
	ldr	w0, [sp, 28]
	add	w0, w1, w0
	uxtw	x0, w0
	ldr	x1, [sp, 8]
	add	x0, x1, x0
	ldr	w1, [sp, 24]
	ldr	w2, [sp, 4]
	lsr	w1, w2, w1
	uxtb	w1, w1
	strb	w1, [x0]
	ldr	w0, [sp, 28]
	add	w0, w0, 1
	str	w0, [sp, 28]
.L2079:
	ldrb	w1, [sp, 2]
	ldr	w0, [sp, 28]
	cmp	w1, w0
	bhi	.L2080
	nop
	add	sp, sp, 32
	ret
	.size	set_spare_data, .-set_spare_data
	.align	2
	.global	get_spare_data
	.type	get_spare_data, %function
get_spare_data:
	sub	sp, sp, #32
	str	x0, [sp, 8]
	strb	w1, [sp, 7]
	strb	w2, [sp, 6]
	str	wzr, [sp, 24]
	str	wzr, [sp, 28]
	b	.L2082
.L2083:
	ldr	w0, [sp, 28]
	lsl	w0, w0, 8
	str	w0, [sp, 28]
	ldrb	w1, [sp, 7]
	ldr	w0, [sp, 24]
	add	w0, w1, w0
	uxtw	x0, w0
	ldr	x1, [sp, 8]
	add	x0, x1, x0
	ldrb	w0, [x0]
	mov	w1, w0
	ldr	w0, [sp, 28]
	orr	w0, w0, w1
	str	w0, [sp, 28]
	ldr	w0, [sp, 24]
	add	w0, w0, 1
	str	w0, [sp, 24]
.L2082:
	ldrb	w1, [sp, 6]
	ldr	w0, [sp, 24]
	cmp	w1, w0
	bhi	.L2083
	ldr	w0, [sp, 28]
	add	sp, sp, 32
	ret
	.size	get_spare_data, .-get_spare_data
	.align	2
	.global	set_oob_logic_page
	.type	set_oob_logic_page, %function
set_oob_logic_page:
	stp	x29, x30, [sp, -48]!
	add	x29, sp, 0
	str	x0, [x29, 40]
	str	x1, [x29, 32]
	str	w2, [x29, 28]
	str	w3, [x29, 24]
	strh	w4, [x29, 22]
	ldr	x0, [x29, 32]
	mov	w1, -1
	strb	w1, [x0]
	mov	w3, 4
	mov	w2, 1
	ldr	w1, [x29, 28]
	ldr	x0, [x29, 32]
	bl	set_spare_data
	ldrh	w0, [x29, 22]
	mov	w3, 2
	mov	w2, 5
	mov	w1, w0
	ldr	x0, [x29, 32]
	bl	set_spare_data
	mov	w3, 4
	mov	w2, 7
	ldr	w1, [x29, 24]
	ldr	x0, [x29, 32]
	bl	set_spare_data
	ldr	x0, [x29, 32]
	add	x0, x0, 1
	ldr	x1, [x29, 32]
	add	x1, x1, 1
	ldrb	w1, [x1]
	orr	w1, w1, -64
	uxtb	w1, w1
	strb	w1, [x0]
	nop
	ldp	x29, x30, [sp], 48
	ret
	.size	set_oob_logic_page, .-set_oob_logic_page
	.align	2
	.global	set_oob_special_page
	.type	set_oob_special_page, %function
set_oob_special_page:
	stp	x29, x30, [sp, -48]!
	add	x29, sp, 0
	str	x0, [x29, 40]
	str	x1, [x29, 32]
	str	w2, [x29, 28]
	str	w3, [x29, 24]
	strh	w4, [x29, 22]
	ldr	x0, [x29, 32]
	mov	w1, -1
	strb	w1, [x0]
	mov	w3, 4
	mov	w2, 1
	ldr	w1, [x29, 28]
	ldr	x0, [x29, 32]
	bl	set_spare_data
	ldrh	w0, [x29, 22]
	mov	w3, 2
	mov	w2, 5
	mov	w1, w0
	ldr	x0, [x29, 32]
	bl	set_spare_data
	mov	w3, 4
	mov	w2, 7
	ldr	w1, [x29, 24]
	ldr	x0, [x29, 32]
	bl	set_spare_data
	nop
	ldp	x29, x30, [sp], 48
	ret
	.size	set_oob_special_page, .-set_oob_special_page
	.align	2
	.global	set_no_use_page
	.type	set_no_use_page, %function
set_no_use_page:
	stp	x29, x30, [sp, -32]!
	add	x29, sp, 0
	str	x0, [x29, 24]
	str	x1, [x29, 16]
	ldr	x0, [x29, 16]
	mov	w1, -1
	strb	w1, [x0]
	mov	w3, 1
	mov	w2, 1
	mov	w1, 170
	ldr	x0, [x29, 16]
	bl	set_spare_data
	mov	w3, 1
	mov	w2, 2
	mov	w1, 187
	ldr	x0, [x29, 16]
	bl	set_spare_data
	nop
	ldp	x29, x30, [sp], 32
	ret
	.size	set_no_use_page, .-set_no_use_page
	.align	2
	.global	set_start_block_flag
	.type	set_start_block_flag, %function
set_start_block_flag:
	stp	x29, x30, [sp, -48]!
	add	x29, sp, 0
	str	x0, [x29, 40]
	str	x1, [x29, 32]
	str	w2, [x29, 28]
	ldr	x0, [x29, 32]
	mov	w1, -1
	strb	w1, [x0]
	mov	w3, 1
	mov	w2, 1
	mov	w1, 170
	ldr	x0, [x29, 32]
	bl	set_spare_data
	mov	w3, 1
	mov	w2, 2
	mov	w1, 238
	ldr	x0, [x29, 32]
	bl	set_spare_data
	mov	w3, 4
	mov	w2, 3
	ldr	w1, [x29, 28]
	ldr	x0, [x29, 32]
	bl	set_spare_data
	nop
	ldp	x29, x30, [sp], 48
	ret
	.size	set_start_block_flag, .-set_start_block_flag
	.align	2
	.global	set_end_block_flag
	.type	set_end_block_flag, %function
set_end_block_flag:
	stp	x29, x30, [sp, -32]!
	add	x29, sp, 0
	str	x0, [x29, 24]
	str	x1, [x29, 16]
	ldr	x0, [x29, 16]
	mov	w1, -1
	strb	w1, [x0]
	mov	w3, 1
	mov	w2, 1
	mov	w1, 170
	ldr	x0, [x29, 16]
	bl	set_spare_data
	mov	w3, 1
	mov	w2, 2
	mov	w1, 204
	ldr	x0, [x29, 16]
	bl	set_spare_data
	nop
	ldp	x29, x30, [sp], 32
	ret
	.size	set_end_block_flag, .-set_end_block_flag
	.align	2
	.global	get_bad_flag_from_oob
	.type	get_bad_flag_from_oob, %function
get_bad_flag_from_oob:
	stp	x29, x30, [sp, -48]!
	add	x29, sp, 0
	str	x0, [x29, 24]
	mov	w2, 4
	mov	w1, 1
	ldr	x0, [x29, 24]
	bl	get_spare_data
	str	w0, [x29, 44]
	ldr	w0, [x29, 44]
	cmp	w0, 0
	bne	.L2091
	mov	w0, 0
	b	.L2092
.L2091:
	mov	w0, 1
.L2092:
	ldp	x29, x30, [sp], 48
	ret
	.size	get_bad_flag_from_oob, .-get_bad_flag_from_oob
	.align	2
	.global	get_block_used_count_from_oob
	.type	get_block_used_count_from_oob, %function
get_block_used_count_from_oob:
	stp	x29, x30, [sp, -48]!
	add	x29, sp, 0
	str	x0, [x29, 24]
	mov	w2, 4
	mov	w1, 7
	ldr	x0, [x29, 24]
	bl	get_spare_data
	str	w0, [x29, 44]
	ldr	w0, [x29, 44]
	ldp	x29, x30, [sp], 48
	ret
	.size	get_block_used_count_from_oob, .-get_block_used_count_from_oob
	.align	2
	.global	get_logic_page_from_oob
	.type	get_logic_page_from_oob, %function
get_logic_page_from_oob:
	stp	x29, x30, [sp, -48]!
	add	x29, sp, 0
	str	x0, [x29, 24]
	mov	w2, 4
	mov	w1, 1
	ldr	x0, [x29, 24]
	bl	get_spare_data
	str	w0, [x29, 44]
	ldr	w0, [x29, 44]
	and	w0, w0, 268435455
	str	w0, [x29, 44]
	ldr	w0, [x29, 44]
	ldp	x29, x30, [sp], 48
	ret
	.size	get_logic_page_from_oob, .-get_logic_page_from_oob
	.align	2
	.global	get_special_data_from_oob
	.type	get_special_data_from_oob, %function
get_special_data_from_oob:
	stp	x29, x30, [sp, -48]!
	add	x29, sp, 0
	str	x0, [x29, 24]
	mov	w2, 4
	mov	w1, 1
	ldr	x0, [x29, 24]
	bl	get_spare_data
	str	w0, [x29, 44]
	ldr	w0, [x29, 44]
	ldp	x29, x30, [sp], 48
	ret
	.size	get_special_data_from_oob, .-get_special_data_from_oob
	.align	2
	.global	get_erase_count_from_oob
	.type	get_erase_count_from_oob, %function
get_erase_count_from_oob:
	stp	x29, x30, [sp, -48]!
	add	x29, sp, 0
	str	x0, [x29, 24]
	mov	w2, 2
	mov	w1, 5
	ldr	x0, [x29, 24]
	bl	get_spare_data
	strh	w0, [x29, 46]
	ldrh	w0, [x29, 46]
	ldp	x29, x30, [sp], 48
	ret
	.size	get_erase_count_from_oob, .-get_erase_count_from_oob
	.align	2
	.global	is_ftl_start_flag_page
	.type	is_ftl_start_flag_page, %function
is_ftl_start_flag_page:
	sub	sp, sp, #16
	str	x0, [sp, 8]
	ldr	x0, [sp, 8]
	add	x0, x0, 1
	ldrb	w0, [x0]
	cmp	w0, 170
	bne	.L2102
	ldr	x0, [sp, 8]
	add	x0, x0, 2
	ldrb	w0, [x0]
	cmp	w0, 238
	bne	.L2102
	mov	w0, 1
	b	.L2103
.L2102:
	mov	w0, 0
.L2103:
	add	sp, sp, 16
	ret
	.size	is_ftl_start_flag_page, .-is_ftl_start_flag_page
	.align	2
	.global	is_ftl_end_flag_page
	.type	is_ftl_end_flag_page, %function
is_ftl_end_flag_page:
	sub	sp, sp, #16
	str	x0, [sp, 8]
	ldr	x0, [sp, 8]
	add	x0, x0, 1
	ldrb	w0, [x0]
	cmp	w0, 170
	bne	.L2105
	ldr	x0, [sp, 8]
	add	x0, x0, 2
	ldrb	w0, [x0]
	cmp	w0, 204
	bne	.L2105
	mov	w0, 1
	b	.L2106
.L2105:
	mov	w0, 0
.L2106:
	add	sp, sp, 16
	ret
	.size	is_ftl_end_flag_page, .-is_ftl_end_flag_page
	.align	2
	.global	is_ftl_special_data
	.type	is_ftl_special_data, %function
is_ftl_special_data:
	sub	sp, sp, #16
	str	x0, [sp, 8]
	ldr	x0, [sp, 8]
	add	x0, x0, 1
	ldrb	w0, [x0]
	cmp	w0, 170
	bne	.L2108
	mov	w0, 1
	b	.L2109
.L2108:
	mov	w0, 0
.L2109:
	add	sp, sp, 16
	ret
	.size	is_ftl_special_data, .-is_ftl_special_data
	.align	2
	.global	is_ftl_logic_page_data
	.type	is_ftl_logic_page_data, %function
is_ftl_logic_page_data:
	sub	sp, sp, #16
	str	x0, [sp, 8]
	ldr	x0, [sp, 8]
	add	x0, x0, 1
	ldrb	w0, [x0]
	and	w0, w0, 240
	cmp	w0, 192
	bne	.L2111
	mov	w0, 1
	b	.L2112
.L2111:
	mov	w0, 0
.L2112:
	add	sp, sp, 16
	ret
	.size	is_ftl_logic_page_data, .-is_ftl_logic_page_data
	.align	2
	.global	is_nouse_page
	.type	is_nouse_page, %function
is_nouse_page:
	sub	sp, sp, #32
	str	x0, [sp, 8]
	str	wzr, [sp, 24]
	str	wzr, [sp, 28]
	b	.L2114
.L2116:
	ldr	w0, [sp, 28]
	ldr	x1, [sp, 8]
	add	x0, x1, x0
	ldrb	w0, [x0]
	cmp	w0, 255
	bne	.L2115
	ldr	w0, [sp, 24]
	add	w0, w0, 1
	str	w0, [sp, 24]
.L2115:
	ldr	w0, [sp, 28]
	add	w0, w0, 1
	str	w0, [sp, 28]
.L2114:
	ldr	w0, [sp, 28]
	cmp	w0, 9
	bls	.L2116
	ldr	w0, [sp, 24]
	cmp	w0, 8
	bls	.L2117
	ldr	x0, [sp, 8]
	ldrb	w0, [x0]
	cmp	w0, 255
	bne	.L2117
	mov	w0, 1
	b	.L2118
.L2117:
	mov	w0, 0
.L2118:
	add	sp, sp, 32
	ret
	.size	is_nouse_page, .-is_nouse_page
	.align	2
	.global	is_fill_page
	.type	is_fill_page, %function
is_fill_page:
	stp	x29, x30, [sp, -48]!
	add	x29, sp, 0
	str	x0, [x29, 24]
	ldr	x0, [x29, 24]
	bl	get_special_data_from_oob
	str	w0, [x29, 44]
	ldr	w1, [x29, 44]
	mov	w0, 1431655765
	cmp	w1, w0
	bne	.L2120
	mov	w0, 1
	b	.L2121
.L2120:
	mov	w0, 0
.L2121:
	ldp	x29, x30, [sp], 48
	ret
	.size	is_fill_page, .-is_fill_page
	.align	2
	.global	is_phy_mapping_page
	.type	is_phy_mapping_page, %function
is_phy_mapping_page:
	stp	x29, x30, [sp, -48]!
	add	x29, sp, 0
	str	x0, [x29, 24]
	ldr	x0, [x29, 24]
	bl	get_special_data_from_oob
	str	w0, [x29, 44]
	ldr	w1, [x29, 44]
	mov	w0, -1431633921
	cmp	w1, w0
	bne	.L2123
	mov	w0, 1
	b	.L2124
.L2123:
	mov	w0, 0
.L2124:
	ldp	x29, x30, [sp], 48
	ret
	.size	is_phy_mapping_page, .-is_phy_mapping_page
	.align	2
	.global	is_function_info_page
	.type	is_function_info_page, %function
is_function_info_page:
	stp	x29, x30, [sp, -48]!
	add	x29, sp, 0
	str	x0, [x29, 24]
	ldr	x0, [x29, 24]
	bl	get_special_data_from_oob
	str	w0, [x29, 44]
	ldr	w0, [x29, 44]
	and	w1, w0, -65536
	mov	w0, -1441726464
	cmp	w1, w0
	bne	.L2126
	mov	w0, 1
	b	.L2127
.L2126:
	mov	w0, 0
.L2127:
	ldp	x29, x30, [sp], 48
	ret
	.size	is_function_info_page, .-is_function_info_page
	.align	2
	.global	is_power_down_info_page
	.type	is_power_down_info_page, %function
is_power_down_info_page:
	stp	x29, x30, [sp, -48]!
	add	x29, sp, 0
	str	x0, [x29, 24]
	ldr	x0, [x29, 24]
	bl	get_special_data_from_oob
	str	w0, [x29, 44]
	ldr	w1, [x29, 44]
	mov	w0, 13107
	movk	w0, 0xaa11, lsl 16
	cmp	w1, w0
	bne	.L2129
	mov	w0, 1
	b	.L2130
.L2129:
	mov	w0, 0
.L2130:
	ldp	x29, x30, [sp], 48
	ret
	.size	is_power_down_info_page, .-is_power_down_info_page
	.section	.rodata
	.align	3
.LC470:
	.string	"[NE]%s:logic_page:0x%x >= total_pages:0x%x\n"
	.text
	.align	2
	.global	get_logic_page_map
	.type	get_logic_page_map, %function
get_logic_page_map:
	stp	x29, x30, [sp, -64]!
	add	x29, sp, 0
	str	x0, [x29, 24]
	str	w1, [x29, 20]
	ldr	x0, [x29, 24]
	ldr	w1, [x0, 36]
	ldr	w0, [x29, 20]
	cmp	w1, w0
	bhi	.L2132
	ldr	x0, [x29, 24]
	ldr	w2, [x0, 36]
	adrp	x0, __func__.8479
	add	x1, x0, :lo12:__func__.8479
	adrp	x0, .LC470
	add	x0, x0, :lo12:.LC470
	mov	w3, w2
	ldr	w2, [x29, 20]
	bl	nand_print
	str	xzr, [x29, 48]
	ldr	x0, [x29, 48]
	b	.L2133
.L2132:
	mov	w0, 262144
	str	w0, [x29, 44]
	str	wzr, [x29, 60]
	b	.L2134
.L2135:
	ldr	w0, [x29, 60]
	add	w0, w0, 1
	str	w0, [x29, 60]
	ldr	w1, [x29, 20]
	ldr	w0, [x29, 44]
	sub	w0, w1, w0
	str	w0, [x29, 20]
.L2134:
	ldr	w1, [x29, 20]
	ldr	w0, [x29, 44]
	cmp	w1, w0
	bcs	.L2135
	ldr	x1, [x29, 24]
	ldr	w0, [x29, 60]
	add	x0, x0, 8
	lsl	x0, x0, 3
	add	x0, x1, x0
	ldr	x0, [x0, 8]
	str	x0, [x29, 32]
	ldr	w0, [x29, 20]
	lsl	x0, x0, 2
	ldr	x1, [x29, 32]
	add	x0, x1, x0
	str	x0, [x29, 48]
	ldr	x0, [x29, 48]
.L2133:
	ldp	x29, x30, [sp], 64
	ret
	.size	get_logic_page_map, .-get_logic_page_map
	.section	.rodata
	.align	3
.LC471:
	.string	"[NE]%s:malloc fail for logic_page_map %d\n"
	.align	3
.LC472:
	.string	"[NE]%s: malloc fail for logic_page_map %d\n"
	.align	3
.LC473:
	.string	"[NE]zone_param_init malloc_logic_page_map error %d\n"
	.text
	.align	2
	.global	malloc_logic_page_map
	.type	malloc_logic_page_map, %function
malloc_logic_page_map:
	stp	x29, x30, [sp, -48]!
	add	x29, sp, 0
	str	x0, [x29, 24]
	str	w1, [x29, 20]
	mov	w0, 262144
	str	w0, [x29, 40]
	str	wzr, [x29, 44]
	b	.L2137
.L2138:
	ldr	x1, [x29, 24]
	ldr	w0, [x29, 44]
	add	x0, x0, 8
	lsl	x0, x0, 3
	add	x0, x1, x0
	str	xzr, [x0, 8]
	ldr	w0, [x29, 44]
	add	w0, w0, 1
	str	w0, [x29, 44]
.L2137:
	ldr	w0, [x29, 44]
	cmp	w0, 24
	bls	.L2138
	str	wzr, [x29, 44]
	b	.L2139
.L2146:
	ldr	w1, [x29, 20]
	ldr	w0, [x29, 40]
	cmp	w1, w0
	bcc	.L2140
	ldr	w0, [x29, 40]
	lsl	w0, w0, 2
	str	w0, [x29, 36]
	ldr	w0, [x29, 36]
	bl	nftl_malloc
	mov	x2, x0
	ldr	x1, [x29, 24]
	ldr	w0, [x29, 44]
	add	x0, x0, 8
	lsl	x0, x0, 3
	add	x0, x1, x0
	str	x2, [x0, 8]
	ldr	x1, [x29, 24]
	ldr	w0, [x29, 44]
	add	x0, x0, 8
	lsl	x0, x0, 3
	add	x0, x1, x0
	ldr	x0, [x0, 8]
	cmp	x0, 0
	bne	.L2141
	adrp	x0, .LC471
	add	x0, x0, :lo12:.LC471
	ldr	w1, [x29, 44]
	bl	nand_print
	mov	w0, -2
	b	.L2142
.L2141:
	ldr	w1, [x29, 20]
	ldr	w0, [x29, 40]
	sub	w0, w1, w0
	str	w0, [x29, 20]
	ldr	x1, [x29, 24]
	ldr	w0, [x29, 44]
	add	x0, x0, 8
	lsl	x0, x0, 3
	add	x0, x1, x0
	ldr	x0, [x0, 8]
	ldr	w2, [x29, 36]
	mov	w1, -1
	bl	nand_memset
	b	.L2148
.L2140:
	ldr	w0, [x29, 20]
	lsl	w0, w0, 2
	str	w0, [x29, 36]
	ldr	w0, [x29, 36]
	bl	nftl_malloc
	mov	x2, x0
	ldr	x1, [x29, 24]
	ldr	w0, [x29, 44]
	add	x0, x0, 8
	lsl	x0, x0, 3
	add	x0, x1, x0
	str	x2, [x0, 8]
	ldr	x1, [x29, 24]
	ldr	w0, [x29, 44]
	add	x0, x0, 8
	lsl	x0, x0, 3
	add	x0, x1, x0
	ldr	x0, [x0, 8]
	cmp	x0, 0
	bne	.L2144
	adrp	x0, .LC472
	add	x0, x0, :lo12:.LC472
	ldr	w1, [x29, 44]
	bl	nand_print
	mov	w0, -2
	b	.L2142
.L2144:
	ldr	x1, [x29, 24]
	ldr	w0, [x29, 44]
	add	x0, x0, 8
	lsl	x0, x0, 3
	add	x0, x1, x0
	ldr	x0, [x0, 8]
	ldr	w2, [x29, 36]
	mov	w1, -1
	bl	nand_memset
	str	wzr, [x29, 20]
	b	.L2145
.L2148:
	ldr	w0, [x29, 44]
	add	w0, w0, 1
	str	w0, [x29, 44]
.L2139:
	ldr	w0, [x29, 44]
	cmp	w0, 24
	bls	.L2146
.L2145:
	ldr	w0, [x29, 20]
	cmp	w0, 0
	beq	.L2147
	adrp	x0, .LC473
	add	x0, x0, :lo12:.LC473
	ldr	w1, [x29, 20]
	bl	nand_print
	mov	w0, -2
	b	.L2142
.L2147:
	mov	w0, 0
.L2142:
	ldp	x29, x30, [sp], 48
	ret
	.size	malloc_logic_page_map, .-malloc_logic_page_map
	.align	2
	.global	free_logic_page_map
	.type	free_logic_page_map, %function
free_logic_page_map:
	stp	x29, x30, [sp, -48]!
	add	x29, sp, 0
	str	x0, [x29, 24]
	str	wzr, [x29, 44]
	b	.L2150
.L2152:
	ldr	x1, [x29, 24]
	ldr	w0, [x29, 44]
	add	x0, x0, 8
	lsl	x0, x0, 3
	add	x0, x1, x0
	ldr	x0, [x0, 8]
	cmp	x0, 0
	beq	.L2151
	ldr	x1, [x29, 24]
	ldr	w0, [x29, 44]
	add	x0, x0, 8
	lsl	x0, x0, 3
	add	x0, x1, x0
	ldr	x0, [x0, 8]
	bl	nftl_free
.L2151:
	ldr	w0, [x29, 44]
	add	w0, w0, 1
	str	w0, [x29, 44]
.L2150:
	ldr	w0, [x29, 44]
	cmp	w0, 24
	bls	.L2152
	mov	w0, 0
	ldp	x29, x30, [sp], 48
	ret
	.size	free_logic_page_map, .-free_logic_page_map
	.align	2
	.global	set_physic_op_par
	.type	set_physic_op_par, %function
set_physic_op_par:
	sub	sp, sp, #32
	str	x0, [sp, 24]
	strh	w1, [sp, 22]
	strh	w2, [sp, 20]
	strh	w3, [sp, 18]
	str	x4, [sp, 8]
	str	x5, [sp]
	ldr	x0, [sp, 24]
	ldrh	w1, [sp, 20]
	strh	w1, [x0]
	ldr	x0, [sp, 24]
	ldrh	w1, [sp, 22]
	strh	w1, [x0, 2]
	ldr	x0, [sp, 24]
	ldrh	w1, [sp, 18]
	strh	w1, [x0, 4]
	ldr	x0, [sp, 24]
	ldr	x1, [sp, 8]
	str	x1, [x0, 8]
	ldr	x0, [sp, 24]
	ldr	x1, [sp]
	str	x1, [x0, 16]
	nop
	add	sp, sp, 32
	ret
	.size	set_physic_op_par, .-set_physic_op_par
	.section	.rodata
	.align	3
.LC474:
	.string	"[NE]%s: malloc fail for zone\n"
	.align	3
.LC475:
	.string	"[NE]%s: malloc fail for nand_chip\n"
	.align	3
.LC476:
	.string	"[NE]%s: malloc fail for nand_block_info\n"
	.align	3
.LC477:
	.string	"[NE]not enough blocks %d!\n"
	.align	3
.LC478:
	.string	"[ND]nftl start:%d,%d\n"
	.align	3
.LC479:
	.string	"[NE]zone_param_init fail!\n"
	.align	3
.LC480:
	.string	"[NE]build_zone_list_first fail!\n"
	.align	3
.LC481:
	.string	"[NE]build_zone_list fail!\n"
	.align	3
.LC482:
	.string	"[NI]try again\n"
	.text
	.align	2
	.global	nftl_start
	.type	nftl_start, %function
nftl_start:
	stp	x29, x30, [sp, -160]!
	add	x29, sp, 0
	str	x19, [sp, 16]
	str	x0, [x29, 88]
	str	x1, [x29, 80]
	str	x2, [x29, 72]
	str	x3, [x29, 64]
	str	x4, [x29, 56]
	str	w5, [x29, 52]
	str	w6, [x29, 48]
	str	w7, [x29, 44]
	str	wzr, [x29, 152]
	ldr	x0, [x29, 64]
	lsr	x0, x0, 9
	str	x0, [x29, 64]
	ldr	x0, [x29, 56]
	lsr	x0, x0, 9
	str	x0, [x29, 56]
	ldr	w0, [x29, 52]
	lsr	w0, w0, 9
	str	w0, [x29, 52]
	ldr	w0, [x29, 48]
	lsr	w0, w0, 9
	str	w0, [x29, 48]
	ldr	x0, [x29, 64]
	str	w0, [x29, 148]
	ldr	x0, [x29, 56]
	str	w0, [x29, 144]
	ldr	w0, [x29, 52]
	str	w0, [x29, 140]
	ldr	w0, [x29, 48]
	str	w0, [x29, 136]
	mov	w0, 3528
	bl	nftl_malloc
	mov	x1, x0
	ldr	x0, [x29, 72]
	str	x1, [x0]
	ldr	x0, [x29, 72]
	ldr	x0, [x0]
	str	x0, [x29, 128]
	ldr	x0, [x29, 128]
	cmp	x0, 0
	bne	.L2156
	adrp	x0, __func__.8536
	add	x1, x0, :lo12:__func__.8536
	adrp	x0, .LC474
	add	x0, x0, :lo12:.LC474
	bl	nand_print
	mov	w0, -2
	b	.L2157
.L2156:
	ldr	w0, [x29, 152]
	add	w0, w0, 1
	str	w0, [x29, 152]
	mov	w2, 3528
	mov	w1, 0
	ldr	x0, [x29, 128]
	bl	nand_memset
	ldr	x0, [x29, 128]
	ldr	x1, [x29, 88]
	str	x1, [x0]
	ldr	x0, [x29, 128]
	ldr	x1, [x29, 80]
	str	x1, [x0, 3448]
	mov	w0, 40
	bl	nftl_malloc
	mov	x1, x0
	ldr	x0, [x29, 128]
	str	x1, [x0, 8]
	ldr	x0, [x29, 128]
	ldr	x0, [x0, 8]
	cmp	x0, 0
	bne	.L2158
	adrp	x0, __func__.8536
	add	x1, x0, :lo12:__func__.8536
	adrp	x0, .LC475
	add	x0, x0, :lo12:.LC475
	bl	nand_print
	mov	w0, -2
	b	.L2157
.L2158:
	ldr	x0, [x29, 128]
	ldr	x0, [x0, 8]
	mov	w2, 8
	mov	w1, 0
	bl	nand_memset
	ldr	x0, [x29, 128]
	ldr	x0, [x0, 8]
	mov	x3, x0
	adrp	x0, g_nsi
	add	x0, x0, :lo12:g_nsi
	ldr	x0, [x0]
	ldr	x0, [x0, 8]
	add	x0, x0, 16
	mov	w2, 8
	mov	x1, x0
	mov	x0, x3
	bl	nand_memcpy
	ldr	x0, [x29, 128]
	ldr	x0, [x0, 8]
	ldr	w1, [x29, 136]
	uxth	w1, w1
	ubfiz	w1, w1, 9, 7
	uxth	w1, w1
	strh	w1, [x0, 26]
	ldr	x0, [x29, 128]
	ldr	x0, [x0, 8]
	ldr	w1, [x29, 44]
	uxtb	w1, w1
	strb	w1, [x0, 10]
	ldr	w1, [x29, 144]
	ldr	w0, [x29, 140]
	udiv	w0, w1, w0
	str	w0, [x29, 124]
	ldr	x0, [x29, 128]
	ldr	x0, [x0, 3448]
	ldrh	w0, [x0, 12]
	mov	w1, w0
	ldr	w0, [x29, 124]
	cmp	w1, w0
	bcc	.L2159
	mov	w0, -1
	b	.L2157
.L2159:
	ldr	x0, [x29, 128]
	ldr	x0, [x0, 8]
	ldr	w1, [x29, 124]
	uxth	w1, w1
	strh	w1, [x0, 24]
	ldr	x0, [x29, 128]
	ldr	x0, [x0, 8]
	ldr	w2, [x29, 140]
	ldr	w1, [x29, 136]
	udiv	w1, w2, w1
	uxth	w1, w1
	strh	w1, [x0, 20]
	ldr	x0, [x29, 128]
	ldr	x0, [x0, 8]
	ldr	w1, [x29, 136]
	uxtb	w1, w1
	strb	w1, [x0, 8]
	ldr	x0, [x29, 128]
	ldr	x0, [x0, 8]
	ldr	x1, [x29, 128]
	ldr	x1, [x1, 8]
	ldrb	w1, [x1, 8]
	uxth	w1, w1
	strh	w1, [x0, 28]
	ldr	x0, [x29, 128]
	ldr	x0, [x0, 8]
	mov	w1, 3000
	strh	w1, [x0, 16]
	ldr	w0, [x29, 124]
	str	w0, [x29, 120]
	ldr	x0, [x29, 128]
	ldr	x19, [x0, 8]
	ldr	w0, [x29, 120]
	add	w0, w0, 1
	lsl	w0, w0, 6
	bl	nftl_malloc
	str	x0, [x19, 32]
	ldr	x0, [x29, 128]
	ldr	x0, [x0, 8]
	ldr	x0, [x0, 32]
	cmp	x0, 0
	bne	.L2160
	adrp	x0, __func__.8536
	add	x1, x0, :lo12:__func__.8536
	adrp	x0, .LC476
	add	x0, x0, :lo12:.LC476
	bl	nand_print
	mov	w0, -2
	b	.L2157
.L2160:
	ldr	x0, [x29, 128]
	ldr	x0, [x0, 8]
	ldr	x3, [x0, 32]
	ldr	w0, [x29, 120]
	lsl	w0, w0, 6
	mov	w2, w0
	mov	w1, -1
	mov	x0, x3
	bl	nand_memset
	str	wzr, [x29, 156]
	b	.L2161
.L2162:
	ldr	x0, [x29, 128]
	ldr	x0, [x0, 8]
	ldr	x1, [x0, 32]
	ldr	w0, [x29, 156]
	lsl	x0, x0, 6
	add	x0, x1, x0
	ldr	w1, [x29, 156]
	uxth	w1, w1
	strh	w1, [x0]
	ldr	x0, [x29, 128]
	ldr	x0, [x0, 8]
	ldr	x1, [x0, 32]
	ldr	w0, [x29, 156]
	lsl	x0, x0, 6
	add	x0, x1, x0
	strh	wzr, [x0, 4]
	ldr	x0, [x29, 128]
	ldr	x0, [x0, 8]
	ldr	x1, [x0, 32]
	ldr	w0, [x29, 156]
	lsl	x0, x0, 6
	add	x0, x1, x0
	strh	wzr, [x0, 2]
	ldr	x0, [x29, 128]
	ldr	x0, [x0, 8]
	ldr	x1, [x0, 32]
	ldr	w0, [x29, 156]
	lsl	x0, x0, 6
	add	x0, x1, x0
	mov	w1, -1
	str	w1, [x0, 8]
	ldr	x0, [x29, 128]
	ldr	x0, [x0, 8]
	ldr	x1, [x0, 32]
	ldr	w0, [x29, 156]
	lsl	x0, x0, 6
	add	x0, x1, x0
	strh	wzr, [x0, 6]
	ldr	x0, [x29, 128]
	ldr	x0, [x0, 8]
	ldr	x1, [x0, 32]
	ldr	w0, [x29, 156]
	lsl	x0, x0, 6
	add	x0, x1, x0
	str	xzr, [x0, 16]
	ldr	x0, [x29, 128]
	ldr	x0, [x0, 8]
	ldr	x1, [x0, 32]
	ldr	w0, [x29, 156]
	lsl	x0, x0, 6
	add	x0, x1, x0
	str	xzr, [x0, 24]
	ldr	x0, [x29, 128]
	ldr	x0, [x0, 8]
	ldr	x1, [x0, 32]
	ldr	w0, [x29, 156]
	lsl	x0, x0, 6
	add	x0, x1, x0
	str	xzr, [x0, 32]
	ldr	x0, [x29, 128]
	ldr	x0, [x0, 8]
	ldr	x1, [x0, 32]
	ldr	w0, [x29, 156]
	lsl	x0, x0, 6
	add	x0, x1, x0
	str	xzr, [x0, 40]
	ldr	x0, [x29, 128]
	ldr	x0, [x0, 8]
	ldr	x1, [x0, 32]
	ldr	w0, [x29, 156]
	lsl	x0, x0, 6
	add	x0, x1, x0
	str	xzr, [x0, 48]
	ldr	x0, [x29, 128]
	ldr	x0, [x0, 8]
	ldr	x1, [x0, 32]
	ldr	w0, [x29, 156]
	lsl	x0, x0, 6
	add	x0, x1, x0
	str	xzr, [x0, 56]
	ldr	w0, [x29, 156]
	add	w0, w0, 1
	str	w0, [x29, 156]
.L2161:
	ldr	w0, [x29, 120]
	add	w1, w0, 1
	ldr	w0, [x29, 156]
	cmp	w1, w0
	bhi	.L2162
	ldr	x0, [x29, 72]
	ldr	x0, [x0]
	bl	nftl_ops_init
	ldr	w1, [x29, 148]
	ldr	w0, [x29, 140]
	udiv	w0, w1, w0
	str	w0, [x29, 116]
	ldr	w0, [x29, 124]
	uxth	w1, w0
	ldr	w0, [x29, 116]
	uxth	w0, w0
	sub	w0, w1, w0
	uxth	w1, w0
	ldr	x0, [x29, 128]
	strh	w1, [x0, 30]
	ldr	x0, [x29, 128]
	ldrh	w1, [x0, 30]
	ldr	x0, [x29, 128]
	ldr	x0, [x0, 3448]
	ldrh	w0, [x0, 12]
	cmp	w1, w0
	bcs	.L2163
	ldr	x0, [x29, 128]
	ldrh	w0, [x0, 30]
	mov	w1, w0
	adrp	x0, .LC477
	add	x0, x0, :lo12:.LC477
	bl	nand_print
	mov	w0, -2
	b	.L2157
.L2163:
	ldr	x0, [x29, 128]
	ldrh	w0, [x0, 30]
	mov	w1, w0
	adrp	x0, .LC478
	add	x0, x0, :lo12:.LC478
	mov	w2, w1
	ldr	w1, [x29, 124]
	bl	nand_print_dbg
	ldr	w0, [x29, 140]
	str	w0, [x29, 112]
	ldr	x0, [x29, 72]
	ldr	x4, [x0]
	ldr	x0, [x29, 128]
	ldrh	w0, [x0, 30]
	mov	w1, w0
	ldr	w0, [x29, 124]
	sub	w1, w0, w1
	ldr	w0, [x29, 112]
	mul	w1, w1, w0
	ldr	x0, [x29, 128]
	ldrh	w0, [x0, 30]
	mov	w2, w0
	ldr	w0, [x29, 112]
	mul	w0, w2, w0
	mov	w3, w0
	mov	w2, w1
	mov	w1, 0
	mov	x0, x4
	bl	zone_param_init
	str	w0, [x29, 108]
	ldr	w0, [x29, 108]
	cmp	w0, 0
	beq	.L2164
	adrp	x0, .LC479
	add	x0, x0, :lo12:.LC479
	bl	nand_print
	ldr	w0, [x29, 108]
	b	.L2157
.L2164:
	ldr	w0, [x29, 168]
	uxtb	w1, w0
	ldr	x0, [x29, 128]
	strb	w1, [x0, 24]
	ldr	x0, [x29, 128]
	ldr	x0, [x0, 3448]
	ldrh	w0, [x0, 26]
	cmp	w0, 0
	beq	.L2165
	ldr	x0, [x29, 128]
	ldrb	w0, [x0, 25]
	orr	w0, w0, 1
	uxtb	w1, w0
	ldr	x0, [x29, 128]
	strb	w1, [x0, 25]
.L2165:
	ldr	x0, [x29, 128]
	ldr	x0, [x0, 3448]
	ldrh	w0, [x0, 10]
	cmp	w0, 0
	beq	.L2166
	ldr	x0, [x29, 72]
	ldr	x0, [x0]
	ldr	w1, [x29, 124]
	bl	is_no_use_device
	cmp	w0, 1
	bne	.L2166
	ldr	x0, [x29, 72]
	ldr	x0, [x0]
	ldr	w1, [x29, 124]
	bl	build_zone_list_first
	str	w0, [x29, 108]
	ldr	w0, [x29, 108]
	cmp	w0, 0
	beq	.L2166
	adrp	x0, .LC480
	add	x0, x0, :lo12:.LC480
	bl	nand_print
	ldr	w0, [x29, 108]
	b	.L2157
.L2166:
	ldr	x0, [x29, 72]
	ldr	x0, [x0]
	bl	build_zone_list
	str	w0, [x29, 108]
	ldr	w0, [x29, 108]
	cmp	w0, 0
	beq	.L2167
	adrp	x0, .LC481
	add	x0, x0, :lo12:.LC481
	bl	nand_print
	ldr	w0, [x29, 152]
	cmp	w0, 1
	bhi	.L2168
	adrp	x0, .LC482
	add	x0, x0, :lo12:.LC482
	bl	nand_print
	b	.L2156
.L2168:
	ldr	w0, [x29, 108]
	b	.L2157
.L2167:
	ldr	x0, [x29, 72]
	ldr	x0, [x0]
	bl	nftl_cache_init
	mov	w0, 0
.L2157:
	ldr	x19, [sp, 16]
	ldp	x29, x30, [sp], 160
	ret
	.size	nftl_start, .-nftl_start
	.align	2
	.global	nftl_zone_release
	.type	nftl_zone_release, %function
nftl_zone_release:
	stp	x29, x30, [sp, -32]!
	add	x29, sp, 0
	str	x0, [x29, 24]
	ldr	x0, [x29, 24]
	bl	nftl_cache_exit
	ldr	x0, [x29, 24]
	bl	zone_param_exit
	ldr	x0, [x29, 24]
	ldr	x0, [x0, 8]
	ldr	x0, [x0, 32]
	bl	nftl_free
	ldr	x0, [x29, 24]
	ldr	x0, [x0, 8]
	bl	nftl_free
	ldr	x0, [x29, 24]
	bl	nftl_free
	nop
	ldp	x29, x30, [sp], 32
	ret
	.size	nftl_zone_release, .-nftl_zone_release
	.section	.rodata
	.align	3
	.type	__func__.5194, %object
	.size	__func__.5194, 9
__func__.5194:
	.string	"nftl_add"
	.align	3
	.type	__func__.5346, %object
	.size	__func__.5346, 16
__func__.5346:
	.string	"nftl_initialize"
	.align	3
	.type	__func__.5524, %object
	.size	__func__.5524, 19
__func__.5524:
	.string	"panic_recover_data"
	.align	3
	.type	__func__.6409, %object
	.size	__func__.6409, 25
__func__.6409:
	.string	"sandisk_A19_special_exit"
	.align	3
	.type	__func__.6755, %object
	.size	__func__.6755, 23
__func__.6755:
	.string	"nftl_sector_cache_init"
	.align	3
	.type	__func__.7098, %object
	.size	__func__.7098, 16
__func__.7098:
	.string	"zone_param_init"
	.align	3
	.type	__func__.7312, %object
	.size	__func__.7312, 9
__func__.7312:
	.string	"init_nrc"
	.align	3
	.type	__func__.7402, %object
	.size	__func__.7402, 23
__func__.7402:
	.string	"torture_block_and_mark"
	.align	3
	.type	__func__.7526, %object
	.size	__func__.7526, 26
__func__.7526:
	.string	"block_last_page_ecc_error"
	.align	3
	.type	__func__.7558, %object
	.size	__func__.7558, 18
__func__.7558:
	.string	"nand_dbg_phy_read"
	.align	3
	.type	__func__.7569, %object
	.size	__func__.7569, 23
__func__.7569:
	.string	"nand_dbg_zone_phy_read"
	.align	3
	.type	__func__.7580, %object
	.size	__func__.7580, 24
__func__.7580:
	.string	"nand_dbg_zone_phy_write"
	.align	3
	.type	__func__.7589, %object
	.size	__func__.7589, 19
__func__.7589:
	.string	"nand_dbg_phy_write"
	.align	3
	.type	__func__.7642, %object
	.size	__func__.7642, 19
__func__.7642:
	.string	"write_data_to_nand"
	.align	3
	.type	__FUNCTION__.7666, %object
	.size	__FUNCTION__.7666, 25
__FUNCTION__.7666:
	.string	"nand_power_off_ecc_error"
	.align	3
	.type	__func__.7884, %object
	.size	__func__.7884, 16
__func__.7884:
	.string	"nftl_cache_init"
	.align	3
	.type	__func__.8331, %object
	.size	__func__.8331, 15
__func__.8331:
	.string	"shutdown_on_gc"
	.align	3
	.type	__func__.8479, %object
	.size	__func__.8479, 19
__func__.8479:
	.string	"get_logic_page_map"
	.align	3
	.type	__func__.8536, %object
	.size	__func__.8536, 11
__func__.8536:
	.string	"nftl_start"
	.ident	"GCC: (Linaro GCC 5.3-2016.05) 5.3.1 20160412"
	.section	.note.GNU-stack,"",@progbits
