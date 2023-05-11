/* Definitions of target machine for GNU compiler, OR32 cpu.

   Copyright (C) 2010 Embecosm Limited

This file is part of GCC.

GCC is free software; you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation; either version 3, or (at your option)
any later version.

GCC is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with GCC; see the file COPYING3.  If not see
<http://www.gnu.org/licenses/>.  */

#ifndef GCC_OR32_PROTOS_H
#define GCC_OR32_PROTOS_H

/* The following are for general support. */
extern int         or32_trampoline_code_size (void);

/* The following are only needed when handling the machine definition. */
#ifdef RTX_CODE
extern void        or32_expand_prologue (void);
extern void        or32_expand_epilogue (rtx sibcall);
extern const char *or32_output_move_double (rtx *operands);
extern void        or32_expand_conditional_branch (rtx               *operands,
						   enum machine_mode  mode);
extern int         or32_emit_cmove (rtx  dest,
				    rtx  op,
				    rtx  true_cond,
				    rtx  false_cond);
extern const char *or32_output_bf (rtx * operands);
extern const char *or32_output_cmov (rtx * operands);
extern void        or32_expand_sibcall (rtx  result ATTRIBUTE_UNUSED,
					rtx  addr,
					rtx  args_size);
extern void        or32_emit_set_const32 (rtx  op0,
					  rtx  op1);
#endif

#endif
extern int or32_struct_alignment (tree);
extern int or32_data_alignment (tree, int);

extern int or32_initial_elimination_offset (int, int);
extern bool or32_save_reg_p_cached (int regno);
extern void or32_print_jump_restore (rtx jump_address);


/* For RETURN_ADDR_RTX */
extern rtx get_hard_reg_initial_val (enum machine_mode, unsigned int);
