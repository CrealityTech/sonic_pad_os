/* Definitions of target machine for GNU compiler.  OpenRISC 1000 version.
   Copyright (C) 1987, 1988, 1992, 1995, 1996, 1999, 2000, 2001, 2002, 
   2003, 2004, 2005 Free Software Foundation, Inc.
   Copyright (C) 2010 Embecosm Limited
   Contributed by Damjan Lampret <damjanl@bsemi.com> in 1999.
   Major optimizations by Matjaz Breskvar <matjazb@bsemi.com> in 2005.

This file is part of GNU CC.

GNU CC is free software; you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation; either version 1, or (at your option)
any later version.

GNU CC is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with GNU CC; see the file COPYING.  If not, write to
the Free Software Foundation, 59 Temple Place - Suite 330,
Boston, MA 02111-1307, USA.  */

#ifndef _OR32_H_
#define _OR32_H_

/* Target CPU builtins */
#define TARGET_CPU_CPP_BUILTINS()		\
  do						\
    {						\
      builtin_define_std ("OR32");		\
      builtin_define_std ("or32");		\
      if (or32_libc == or32_libc_uclibc)	\
	builtin_define ("__UCLIBC__");		\
      builtin_assert ("cpu=or32");		\
      builtin_assert ("machine=or32");		\
    }						\
  while (0)

/* A string corresponding to the installation directory for target libraries
   and includes. Make it available to SPEC definitions via EXTRA_SPECS */
#define CONC_DIR(dir1, dir2) dir1 "/../../" dir2
#define TARGET_PREFIX CONC_DIR (STANDARD_EXEC_PREFIX, DEFAULT_TARGET_MACHINE)

#define EXTRA_SPECS                                   \
  { "target_prefix", TARGET_PREFIX }

#undef CPP_SPEC
#define CPP_SPEC \
  "%{!mnewlib:%{pthread:-D_XOPEN_SOURCE=700}}"

/* Make sure we pick up the crti.o, crtbegin.o, crtend.o and crtn.o files. */
#undef STARTFILE_SPEC
#define STARTFILE_SPEC \
  "%{!shared:"\
    "%{mnewlib:%(target_prefix)/lib/crt0.o} "\
    "%{!mnewlib:crt0.o%s} crti.o%s crtbegin.o%s}"

#undef ENDFILE_SPEC
#define ENDFILE_SPEC "crtend.o%s crtn.o%s"

/* Specify the newlib library path if necessary */
#undef LINK_SPEC
#define LINK_SPEC "%{mnewlib:-L%(target_prefix)/newlib}"

/* Override previous definitions (linux.h). Newlib doesn't have a profiling
   version of the library, but it does have a debugging version (libg.a) */
#undef LIB_SPEC 
#define LIB_SPEC "%{!mnewlib:"					         \
		   "%{pthread:"						 \
		     "--whole-archive -lpthread --no-whole-archive} "	 \
		   "%{!p:%{!pg:-lc}}%{p:-lc_p}%{pg:-lc_p}}               \
                  %{mnewlib:%{!g:-lc -lor32 -lboard -u free -lc}         \
                            %{g:-lg -lor32 -lboard -u free -lg}	         \
                            %{mboard=*:-L%(target_prefix)/lib/boards/%*} \
			    %{!mboard=*:-L%(target_prefix)/lib/boards/or1ksim}}"

#define TARGET_VERSION fprintf (stderr, " (OpenRISC 1000) Mask 0x%x", MASK_HARD_MUL);

/* Run-time compilation parameters selecting different hardware subsets.  */

extern int target_flags;

/* Target machine storage layout */

/* Define this if most significant bit is lowest numbered
   in instructions that operate on numbered bit-fields.
   This is not true on the or32.  */
#define BITS_BIG_ENDIAN 0

/* Define this if most significant byte of a word is the lowest numbered.  */
#define BYTES_BIG_ENDIAN 1

/* Define this if most significant word of a multiword number is numbered.  */
#define WORDS_BIG_ENDIAN 1

/* Number of bits in an addressable storage unit */
#define BITS_PER_UNIT 8

#define BITS_PER_WORD 32
#define SHORT_TYPE_SIZE 16
#define INT_TYPE_SIZE 32
#define LONG_TYPE_SIZE 32
#define LONG_LONG_TYPE_SIZE 64
#define FLOAT_TYPE_SIZE 32
#define DOUBLE_TYPE_SIZE 64
#define LONG_DOUBLE_TYPE_SIZE 64

/* Width of a word, in units (bytes).  */
#define UNITS_PER_WORD 4

/* Width in bits of a pointer.
   See also the macro `Pmode' defined below.  */
#define POINTER_SIZE 32

/* Allocation boundary (in *bits*) for storing pointers in memory.  */
#define POINTER_BOUNDARY 32

/* Allocation boundary (in *bits*) for storing arguments in argument list.  */
#define PARM_BOUNDARY 32

/* Boundary (in *bits*) on which stack pointer should be aligned.  */
#define STACK_BOUNDARY 32

/* Allocation boundary (in *bits*) for the code of a function.  */
#define FUNCTION_BOUNDARY 32

/* Alignment of field after `int : 0' in a structure.  */
#define EMPTY_FIELD_BOUNDARY 8

/* Every structure's size must be a multiple of this.  */
#define STRUCTURE_SIZE_BOUNDARY (TARGET_PADSTRUCT ? 32 : 8)

/* A bitfield declared as `int' forces `int' alignment for the struct.  */
#define PCC_BITFIELD_TYPE_MATTERS 1

/* No data type wants to be aligned rounder than this.  */
#define BIGGEST_ALIGNMENT 32

/* The best alignment to use in cases where we have a choice.  */
#define FASTEST_ALIGNMENT 32

#define ROUND_TYPE_ALIGN(STRUCT, COMPUTED, SPECIFIED)   \
  ((TREE_CODE (STRUCT) == RECORD_TYPE                   \
    || TREE_CODE (STRUCT) == UNION_TYPE                 \
    || TREE_CODE (STRUCT) == QUAL_UNION_TYPE)           \
   && !TYPE_PACKED (STRUCT)				\
   && TYPE_FIELDS (STRUCT) != 0                         \
     ? MAX (MAX ((COMPUTED), (SPECIFIED)), or32_struct_alignment (STRUCT)) \
     : MAX ((COMPUTED), (SPECIFIED)))                   \

/* Make strings word-aligned so strcpy from constants will be faster.  */
/*
#define CONSTANT_ALIGNMENT(EXP, ALIGN)  				\
  ((TREE_CODE (EXP) == STRING_CST || TREE_CODE (EXP) == CONSTRUCTOR)    \
    && (ALIGN) < FASTEST_ALIGNMENT      				\
   ? FASTEST_ALIGNMENT : (ALIGN))
*/

/* One use of this macro is to increase alignment of medium-size
   data to make it all fit in fewer cache lines.  Another is to
   cause character arrays to be word-aligned so that `strcpy' calls
   that copy constants to character arrays can be done inline.  */         
/*
#define DATA_ALIGNMENT(TYPE, ALIGN)                                     \
  ((((ALIGN) < FASTEST_ALIGNMENT)                                       \
    && (TREE_CODE (TYPE) == ARRAY_TYPE                                  \
        || TREE_CODE (TYPE) == UNION_TYPE                               \
        || TREE_CODE (TYPE) == RECORD_TYPE)) ? FASTEST_ALIGNMENT : (ALIGN))
*/ /* CHECK - btw code gets bigger with this one */
#define DATA_ALIGNMENT(TYPE, ALIGN) \
  ((ALIGN) < FASTEST_ALIGNMENT \
   ? or32_data_alignment ((TYPE), (ALIGN)) : (ALIGN))

#define LOCAL_ALIGNMENT(TYPE, ALIGN) \
  ((ALIGN) < FASTEST_ALIGNMENT \
   ? or32_data_alignment ((TYPE), (ALIGN)) : (ALIGN))

/* Define this if move instructions will actually fail to work
   when given unaligned data.  */
#define STRICT_ALIGNMENT 1 /* CHECK */

/* Align an address */
#define OR32_ALIGN(n,a) (((n) + (a) - 1) & ~((a) - 1))

/* Define if operations between registers always perform the operation
   on the full register even if a narrower mode is specified.  */
#define WORD_REGISTER_OPERATIONS  /* CHECK */
 

/* Define if loading in MODE, an integral mode narrower than BITS_PER_WORD
   will either zero-extend or sign-extend.  The value of this macro should
   be the code that says which one of the two operations is implicitly
   done, NIL if none.  */
#define LOAD_EXTEND_OP(MODE) ZERO_EXTEND

/* Define this macro if it is advisable to hold scalars in registers
   in a wider mode than that declared by the program.  In such cases,
   the value is constrained to be within the bounds of the declared
   type, but kept valid in the wider mode.  The signedness of the
   extension may differ from that of the type. */
#define PROMOTE_MODE(MODE, UNSIGNEDP, TYPE)     \
  if (GET_MODE_CLASS (MODE) == MODE_INT         \
      && GET_MODE_SIZE (MODE) < UNITS_PER_WORD) \
    (MODE) = SImode;
  /* CHECK */


/*
 * brings 0.4% improvment in static size for linux
 *
#define PROMOTE_FOR_CALL_ONLY
*/

/* Define this macro if it is as good or better to call a constant
   function address than to call an address kept in a register.  */
#define NO_FUNCTION_CSE 1 /* check */
   
/* Standard register usage.  */

/* Number of actual hardware registers.
   The hardware registers are assigned numbers for the compiler
   from 0 to just below FIRST_PSEUDO_REGISTER.
   All registers that the compiler knows about must be given numbers,
   even those that are not normally considered general registers.  */

#define OR32_LAST_ACTUAL_REG       31
#define ARG_POINTER_REGNUM     (OR32_LAST_ACTUAL_REG + 1)
#define FRAME_POINTER_REGNUM   (ARG_POINTER_REGNUM + 1)
#define OR32_LAST_INT_REG      FRAME_POINTER_REGNUM
#define OR32_FLAGS_REG         (OR32_LAST_INT_REG + 1)
#define FIRST_PSEUDO_REGISTER  (OR32_FLAGS_REG + 1)

/* 1 for registers that have pervasive standard uses
   and are not available for the register allocator.
   On the or32, these are r1 as stack pointer and 
   r2 as frame/arg pointer.  r9 is link register, r0
   is zero, r10 is linux thread */
#define FIXED_REGISTERS { \
  1, 1, 0, 0, 0, 0, 0, 0, \
  0, 1, 1, 0, 0, 0, 0, 0, \
  0, 0, 0, 0, 0, 0, 0, 0, \
  0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 1 }
/* 1 for registers not available across function calls.
   These must include the FIXED_REGISTERS and also any
   registers that can be used without being saved.
   The latter must include the registers where values are returned
   and the register where structure-value addresses are passed.
   Aside from that, you can include as many other registers as you like.  */
#define CALL_USED_REGISTERS { \
  1, 1, 0, 1, 1, 1, 1, 1, \
  1, 1, 1, 1, 1, 1, 0, 1, \
  0, 1, 0, 1, 0, 1, 0, 1, \
  0, 1, 0, 1, 0, 1, 0, 1, 1, 1, 1}

/* stack pointer: must be FIXED and CALL_USED */
/* hard frame pointer: must be call saved.  */
/* soft frame pointer / arg pointer: must be FIXED and CALL_USED */

/* Return number of consecutive hard regs needed starting at reg REGNO
   to hold something of mode MODE.
   This is ordinarily the length in words of a value of mode MODE
   but can be less for certain modes in special long registers.
   On the or32, all registers are one word long.  */
#define HARD_REGNO_NREGS(REGNO, MODE)   \
 ((GET_MODE_SIZE (MODE) + UNITS_PER_WORD - 1) / UNITS_PER_WORD)

/* Value is 1 if hard register REGNO can hold a value of machine-mode MODE. */
#define HARD_REGNO_MODE_OK(REGNO, MODE) 1

/* Value is 1 if it is a good idea to tie two pseudo registers
   when one has mode MODE1 and one has mode MODE2.
   If HARD_REGNO_MODE_OK could produce different values for MODE1 and MODE2,
   for any hard reg, then this must be 0 for correct output.  */
#define MODES_TIEABLE_P(MODE1, MODE2)  1

/* A C expression for the cost of moving data of mode mode from a register in
   class "from" to one in class "to". The classes are expressed using the
   enumeration values such as GENERAL_REGS. A value of 2 is the default; other
   values are interpreted relative to that.

   It is not required that the cost always equal 2 when "from" is the same as
   "to"; on some machines it is expensive to move between registers if they are
   not general registers.

   If reload sees an insn consisting of a single set between two hard
   registers, and if REGISTER_MOVE_COST applied to their classes returns a
   value of 2, reload does not check to ensure that the constraints of the
   insn are met. Setting a cost of other than 2 will allow reload to verify
   that the constraints are met. You should do this if the "movm" pattern's
   constraints do not allow such copying.

   JPB 31-Aug-10: This is just the default. */
#define REGISTER_MOVE_COST(mode, from, to)  2

/* A C expression for the cost of moving data of mode mode between a register
   of class "class" and memory; "in" is zero if the value is to be written to
   memory, nonzero if it is to be read in. This cost is relative to those in
   REGISTER_MOVE_COST. If moving between registers and memory is more
   expensive than between two registers, you should define this macro to
   express the relative cost.

   If you do not define this macro, GCC uses a default cost of 4 plus the cost
   of copying via a secondary reload register, if one is needed. If your
   machine requires a secondary reload register to copy between memory and a
   register of class but the reload mechanism is more complex than copying via
   an intermediate, define this macro to reflect the actual cost of the move.

   GCC defines the function "memory_move_secondary_cost" if secondary reloads
   are needed. It computes the costs due to copying via a secondary
   register. If your machine copies from memory using a secondary register in
   the conventional way but the default base value of 4 is not correct for
   your machine, define this macro to add some other value to the result of
   that function. The arguments to that function are the same as to this
   macro.

   JPB 31-Aug-10. Is this really correct? I suppose the OR32 only takes one
                  cycle, notionally, to access memory, but surely that will
                  often stall the  pipeline. Needs more investigation. */
#define MEMORY_MOVE_COST(mode, class, in)  2

/* A C expression for the cost of a branch instruction. A value of 1 is the
   default; other values are interpreted relative to that. Parameter "speed_p"
   is TRUE when the branch in question should be optimized for speed. When it
   is FALSE, BRANCH_COST should be returning value optimal for code size
   rather then performance considerations. "predictable_p" is true for well
   predictable branches. On many architectures the BRANCH_COST can be reduced
   then.

   JPB 31-Aug-10. The original code had the comment that "... this should
                  specify the cost of a branch insn; roughly the number of
                  extra insns that should be added to avoid a branch.

		  Set this to 3 on the or32 since that is roughly the average
		  cost of an unscheduled conditional branch.

		  Cost of 2 and 3 give equal and ~0.7% bigger binaries
		  respectively."

		  This seems ad-hoc. Probably we need some experiments. */
#define BRANCH_COST(speed_p, predictable_p)  2

/* Specify the registers used for certain standard purposes.
   The values of these macros are register numbers.  */

/* Register to use for pushing function arguments.  */
#define STACK_POINTER_REGNUM 1

/* Base register for access to local variables of the function.  */
#define HARD_FRAME_POINTER_REGNUM 2

/* Link register. */
#define LINK_REGNUM 9

/* Define this macro if debugging can be performed even without a frame pointer.
   If this macro is de ned, GCC will turn on the `-fomit-frame-pointer' option
   whenever `-O' is specifed.

   This should work for the OpenRISC architecture. Particularly if we
   generate DWARF2 output OK. */
#define CAN_DEBUG_WITHOUT_FP 

/* This function computes the initial size of the frame (difference between SP
   and FP) after the function prologue. */
#define INITIAL_FRAME_POINTER_OFFSET(depth)				\
  {									\
    int regno;								\
    int offset = 0;							\
  									\
    for (regno=0; regno < FIRST_PSEUDO_REGISTER;  regno++)		\
      {									\
	if (df_regs_ever_live_p (regno) && !call_used_regs[regno])	\
	  {								\
	    offset += 4;						\
	  }								\
      }									\
									\
    (depth) = ((!current_function_is_leaf				\
		|| df_regs_ever_live_p (LINK_REGNUM)) ? 4 : 0)		\
      + (frame_pointer_needed ? 4 : 0)					\
      + offset								\
      + OR32_ALIGN (crtl->outgoing_args_size, 4)			\
      + OR32_ALIGN (get_frame_size(), 4);				\
  }

/* Register in which static-chain is passed to a function.  */

#define STATIC_CHAIN_REGNUM 11

#define PROLOGUE_TMP 13
#define EPILOGUE_TMP 3

/* Register in which address to store a structure value
   is passed to a function.  */
/*#define STRUCT_VALUE_REGNUM 0*/

/* Pass address of result struct to callee as "invisible" first argument */
#define STRUCT_VALUE 0

/* -----------------------[ PHX start ]-------------------------------- */

/* Define the classes of registers for register constraints in the
   machine description.  Also define ranges of constants.

   One of the classes must always be named ALL_REGS and include all hard regs.
   If there is more than one class, another class must be named NO_REGS
   and contain no registers.

   The name GENERAL_REGS must be the name of a class (or an alias for
   another name such as ALL_REGS).  This is the class of registers
   that is allowed by "g" or "r" in a register constraint.
   Also, registers outside this class are allocated only when
   instructions express preferences for them.

   GENERAL_REGS and BASE_REGS classess are the same on or32.

   The classes must be numbered in nondecreasing order; that is,
   a larger-numbered class must never be contained completely
   in a smaller-numbered class.

   For any two classes, it is very desirable that there be another
   class that represents their union.  */
   
/* The or32 has only one kind of registers, so NO_REGS, GENERAL_REGS
   and ALL_REGS are the only classes.  */
/* JPB 26-Aug-10: Based on note from Mikhael (mirekez@gmail.com), we don't
   need CR_REGS and it is in the wrong place for later things! */
enum reg_class 
{ 
  NO_REGS,
  GENERAL_REGS,
  ALL_REGS,
  LIM_REG_CLASSES 
};

#define N_REG_CLASSES (int) LIM_REG_CLASSES

/* Give names of register classes as strings for dump file.   */
#define REG_CLASS_NAMES							\
{									\
  "NO_REGS",								\
  "GENERAL_REGS",   							\
  "ALL_REGS"								\
}

/* Define which registers fit in which classes.  This is an initializer for a
   vector of HARD_REG_SET of length N_REG_CLASSES.

   An initializer containing the contents of the register classes, as integers
   which are bit masks.  The Nth integer specifies the contents of class N.
   The way the integer MASK is interpreted is that register R is in the class
   if `MASK & (1 << R)' is 1.

   When the machine has more than 32 registers, an integer does not suffice.
   Then the integers are replaced by sub-initializers, braced groupings
   containing several integers.  Each sub-initializer must be suitable as an
   initializer for the type `HARD_REG_SET' which is defined in
   `hard-reg-set.h'.

   For the OR32 we have the minimal set. GENERAL_REGS is all except r0, which
   it permanently zero. */
#define REG_CLASS_CONTENTS						\
  {									\
    { 0x00000000, 0x00000000 },		/* NO_REGS */			\
    { 0xffffffff, 0x00000003 },		/* GENERAL_REGS */		\
    { 0xffffffff, 0x00000007 }		/* ALL_REGS */			\
  }

/* The same information, inverted:

   Return the class number of the smallest class containing reg number REGNO.
   This could be a conditional expression or could index an array.

   ??? 0 is not really a register, but a constant.  */
#define REGNO_REG_CLASS(regno)						\
  ((0 == regno) ? ALL_REGS : ((1 <= regno) && (regno <= OR32_LAST_INT_REG))		\
   ? GENERAL_REGS : NO_REGS)

/* The class value for index registers, and the one for base regs.  */
#define INDEX_REG_CLASS GENERAL_REGS
#define BASE_REG_CLASS  GENERAL_REGS

/* Given an rtx X being reloaded into a reg required to be in class CLASS,
   return the class of reg to actually use.  In general this is just CLASS;
   but on some machines in some cases it is preferable to use a more
   restrictive class.  */
#define PREFERRED_RELOAD_CLASS(X,CLASS)  (CLASS)

/* Return the maximum number of consecutive registers needed to represent mode
   MODE in a register of class CLASS.

   On the or32, this is always the size of MODE in words, since all registers
   are the same size.  */
#define CLASS_MAX_NREGS(CLASS, MODE)					\
  ((GET_MODE_SIZE (MODE) + UNITS_PER_WORD - 1) / UNITS_PER_WORD)


/* -------------------------------------------------------------------------- */
/* Stack layout; function entry, exit and calling.  */

/* Define this if pushing a word on the stack makes the stack pointer a
   smaller address.  */
#define STACK_GROWS_DOWNWARD 1

/* Define this if the nominal address of the stack frame is at the
   high-address end of the local variables; that is, each additional local
   variable allocated goes at a more negative offset in the frame.  */
#define FRAME_GROWS_DOWNWARD 1

/* Offset within stack frame to start allocating local variables at.  If
   FRAME_GROWS_DOWNWARD, this is the offset to the END of the first local
   allocated.  Otherwise, it is the offset to the BEGINNING of the first local
   allocated.  */
#define STARTING_FRAME_OFFSET 0

/* Offset of first parameter from the argument pointer register value.  */
#define FIRST_PARM_OFFSET(FNDECL) 0

/* Define this if stack space is still allocated for a parameter passed
   in a register.  The value is the number of bytes allocated to this
   area.

   No such allocation for OR32. */
/* #define REG_PARM_STACK_SPACE(FNDECL) (UNITS_PER_WORD * GP_ARG_NUM_REG) */

/* Define this if the above stack space is to be considered part of the
   space allocated by the caller.

   N/a for OR32. */
/* #define OUTGOING_REG_PARM_STACK_SPACE */   

/* Define this macro if `REG_PARM_STACK_SPACE' is defined, but the
   stack parameters don't skip the area specified by it.

   N/a for OR32. */
/* #define STACK_PARMS_IN_REG_PARM_AREA */

/* If nonzero, the maximum amount of space required for outgoing arguments
   will be computed and placed into the variable
   current_function_outgoing_args_size. No space will be pushed onto the stack
   for each call; instead, the function prologue should increase the stack
   frame size by this amount.

   Setting both PUSH_ARGS and ACCUMULATE_OUTGOING_ARGS is not proper.

   This is the approached used by OR32. */
#define ACCUMULATE_OUTGOING_ARGS 1

#define ELIMINABLE_REGS							\
{{ ARG_POINTER_REGNUM,   STACK_POINTER_REGNUM},				\
 { ARG_POINTER_REGNUM,   HARD_FRAME_POINTER_REGNUM},			\
 { FRAME_POINTER_REGNUM, STACK_POINTER_REGNUM},				\
 { FRAME_POINTER_REGNUM, HARD_FRAME_POINTER_REGNUM}}

#define INITIAL_ELIMINATION_OFFSET(FROM, TO, OFFSET) \
  (OFFSET) = or32_initial_elimination_offset ((FROM), (TO))

/* A C expression that should indicate the number of bytes of its own
   arguments that a function pops on returning, or 0 if the function pops no
   arguments and the caller must therefore pop them all after the function
   returns.

   "fundecl" is a C variable whose value is a tree node that describes the
   function in question. Normally it is a node of type FUNCTION_DECL that
   describes the declaration of the function. From this you can obtain the
   DECL_ATTRIBUTES of the function.

   "funtype" is a C variable whose value is a tree node that describes the
   function in question. Normally it is a node of type FUNCTION_TYPE that
   describes the data type of the function. From this it is possible to obtain
   the data types of the value and arguments (if known).

   When a call to a library function is being considered, "fundecl" will
   contain an identifier node for the library function. Thus, if you need to
   distinguish among various library functions, you can do so by their
   names. Note that “library function” in this context means a function used
   to perform arithmetic, whose name is known specially in the compiler and
   was not mentioned in the C code being compiled.

   "size" is the number of bytes of arguments passed on the stack. If a
   variable number of bytes is passed, it is zero, and argument popping will
   always be the re- sponsibility of the calling function.

   On the VAX, all functions always pop their arguments, so the definition of
   this macro is stack-size. On the 68000, using the standard calling
   convention, no functions pop their arguments, so the value of the macro is
   always 0 in this case. But an alternative calling convention is available
   in which functions that take a fixed number of argu- ments pop them but
   other functions (such as printf) pop nothing (the caller pops all). When
   this convention is in use, funtype is examined to determine whether a
   function takes a fixed number of arguments.

   On the OR32, no functions pop their arguments.
   JPB 29-Aug-10: Is this really correct? */
#define RETURN_POPS_ARGS(fundecl, funtype, size) 0

/* Minimum and maximum general purpose registers used to hold arguments.  */
#define GP_ARG_MIN_REG 3
#define GP_ARG_MAX_REG 8
#define GP_ARG_NUM_REG (GP_ARG_MAX_REG - GP_ARG_MIN_REG + 1) 

/* Return register */
#define GP_ARG_RETURN  11 
#define GP_ARG_RETURNH 12 

/* A C expression to create an RTX representing the place where a library
   function returns a value of mode mode.

   Note that “library function” in this context means a compiler support
   routine, used to perform arithmetic, whose name is known specially by the
   compiler and was not mentioned in the C code being compiled.

   For the OR32, return value is in R11 (GP_ARG_RETURN).  */   
#define LIBCALL_VALUE(mode)                                             \
  gen_rtx_REG(								\
	   ((GET_MODE_CLASS (mode) != MODE_INT				\
	     || GET_MODE_SIZE (mode) >= 4)				\
	    ? (mode)							\
	    : SImode),							\
	    GP_ARG_RETURN)

/* Define this if PCC uses the nonreentrant convention for returning
   structure and union values. 

   Not needed for OR32. */
/*#define PCC_STATIC_STRUCT_RETURN */

/* A C expression that is nonzero if regno is the number of a hard register in
   which the values of called function may come back.

   A register whose use for returning values is limited to serving as the
   second of a pair (for a value of type double, say) need not be recognized
   by this macro. So for most machines, this definition suffices:

       #define FUNCTION_VALUE_REGNO_P(N) ((N) == 0)

   If the machine has register windows, so that the caller and the called
   function use different registers for the return value, this macro should
   recognize only the caller's register numbers.

   For OR32, we must check if we have the return register.

   From GCC 4.6, this will be replaced by TARGET_FUNCION_VALUE_REGNO_P target
   hook function. */
#define FUNCTION_VALUE_REGNO_P(N)  ((N) == GP_ARG_RETURN)
 
/* 1 if N is a possible register number for function argument passing. */
#define FUNCTION_ARG_REGNO_P(N) \
   ((N) >= GP_ARG_MIN_REG && (N) <= GP_ARG_MAX_REG)

/* A code distinguishing the floating point format of the target
   machine.  There are three defined values: IEEE_FLOAT_FORMAT,
   VAX_FLOAT_FORMAT, and UNKNOWN_FLOAT_FORMAT.  */
#define TARGET_FLOAT_FORMAT IEEE_FLOAT_FORMAT
#define FLOAT_WORDS_BIG_ENDIAN 1

/* A C type for declaring a variable that is used as the first argument of
   FUNCTION_ARG and other related values. For some target machines, the type
   int suffices and can hold the number of bytes of argument so far.

   There is no need to record in CUMULATIVE_ARGS anything about the arguments
   that have been passed on the stack. The compiler has other variables to
   keep track of that.  For target machines on which all arguments are passed
   on the stack, there is no need to store anything in CUMULATIVE_ARGS;
   however, the data structure must exist and should not be empty, so use
   int. */
#define CUMULATIVE_ARGS int

/* A C statement (sans semicolon) for initializing the variable "cum" for the
   state at the beginning of the argument list. The variable has type
   CUMULATIVE_ARGS. The value of "fntype" is the tree node for the data type
   of the function which will receive the args, or 0 if the args are to a
   compiler support library function. For direct calls that are not libcalls,
   "fndecl" contain the declaration node of the function. "fndecl" is also set
   when INIT_CUMULATIVE_ARGS is used to find arguments for the function being
   compiled.  "n_named_args" is set to the number of named arguments,
   including a structure return address if it is passed as a parameter, when
   making a call. When processing incoming arguments, "n_named_args" is set to
   −1.

   When processing a call to a compiler support library function, "libname"
   identifies which one. It is a symbol_ref rtx which contains the name of the
   function, as a string. "libname" is 0 when an ordinary C function call is
   being processed. Thus, each time this macro is called, either "libname" or
   "fntype" is nonzero, but never both of them at once.

   For the OR32, we set "cum" to zero each time.
   JPB 29-Aug-10: Is this correct? */
#define INIT_CUMULATIVE_ARGS(cum, fntype, libname, fndecl, n_named_args) \
  (cum = 0)

/* -------------------------------------------------------------------------- */
/* Define intermediate macro to compute the size (in registers) of an argument
   for the or32.

   The OR32_ROUND_ADVANCE* macros are local to this file.  */

/* Round "size" up to a word boundary.  */
#define OR32_ROUND_ADVANCE(size)					\
  (((size) + UNITS_PER_WORD - 1) / UNITS_PER_WORD)

/* Round arg "mode"/"type" up to the next word boundary.  */
#define OR32_ROUND_ADVANCE_ARG(mode, type)				\
  ((mode) == BLKmode							\
   ? OR32_ROUND_ADVANCE (int_size_in_bytes (type))			\
   : OR32_ROUND_ADVANCE (GET_MODE_SIZE (mode)))

/* The ABI says that no rounding to even or odd words takes place.  */
#define OR32_ROUND_ADVANCE_CUM(cum, mode, type) (cum)
  
/* Update the data in "cum" to advance over an argument of mode "mode" and
   data type "type".  ("type" is null for libcalls where that information may
   not be available.)  */
#define FUNCTION_ARG_ADVANCE(cum, mode, type, named)			\
  ((cum) = (OR32_ROUND_ADVANCE_CUM ((cum), (mode), (type))		\
	    + OR32_ROUND_ADVANCE_ARG ((mode), (type))))
                       
/* Return boolean indicating if arg of type "type" and mode "mode" will be
   passed in a reg.  This includes arguments that have to be passed by
   reference as the pointer to them is passed in a reg if one is available
   (and that is what we're given).

   When passing arguments "named" is always 1.  When receiving arguments
   "named" is 1 for each argument except the last in a stdarg/varargs
   function.  In a stdarg function we want to treat the last named arg as
   named.  In a varargs function we want to treat the last named arg (which is
   `__builtin_va_alist') as unnamed.

   This macro is only used in this file.  */
#define OR32_PASS_IN_REG_P(cum, mode, type, named)			\
  ((named)                         					\
   && ((OR32_ROUND_ADVANCE_CUM ((cum), (mode), (type))			\
	+ OR32_ROUND_ADVANCE_ARG ((mode), (type))			\
	<= GP_ARG_NUM_REG)))

/* Determine where to put an argument to a function.  Value is zero to push
   the argument on the stack, or a hard register in which to store the
   argument.

   "mode" is the argument's machine mode.

   "type" is the data type of the argument (as a tree).  This is null for
    libcalls where that information may not be available.

   "cum" is a variable of type CUMULATIVE_ARGS which gives info about the
    preceding args and about the function being called.

   "named" is nonzero if this argument is a named parameter (otherwise it is
    an extra parameter matching an ellipsis).

    On the ARC the first MAX_ARC_PARM_REGS args are normally in registers and
    the rest are pushed.  */
#define FUNCTION_ARG(cum, mode, type, named)				\
  (OR32_PASS_IN_REG_P ((cum), (mode), (type), (named))			\
   ? gen_rtx_REG ((mode),						\
		  OR32_ROUND_ADVANCE_CUM ((cum), (mode), (type))	\
		  + GP_ARG_MIN_REG)					\
   : 0)

/* Output assembler code to FILE to increment profiler label # LABELNO
   for profiling a function entry.

   JPB 29-Aug-10: This patently doesn't work. It is not even OR32 code! */
#define FUNCTION_PROFILER(FILE, LABELNO)  \
   fprintf (FILE, "\tl.load32u\tr0,LP%d\n\tcall\tmcount\n", (LABELNO));

/* EXIT_IGNORE_STACK should be nonzero if, when returning from a function, the
   stack pointer does not matter.  The value is tested only in functions that
   have frame pointers.  No definition is equivalent to always zero.

   The default suffices for OR32. */
#define EXIT_IGNORE_STACK 0

/* A C expression whose value is RTL representing the location of the
   incoming return address at the beginning of any function, before the
   prologue.  This RTL is either a REG, indicating that the return
   value is saved in REG, or a MEM representing a location in
   the stack.  */
#define INCOMING_RETURN_ADDR_RTX gen_rtx_REG (Pmode, LINK_REGNUM)

#define RETURN_ADDR_RTX(COUNT, FP) \
  ((COUNT) ? NULL_RTX : get_hard_reg_initial_val (Pmode, LINK_REGNUM))

/* Addressing modes, and classification of registers for them.  */

/* #define HAVE_POST_INCREMENT */
/* #define HAVE_POST_DECREMENT */

/* #define HAVE_PRE_DECREMENT */
/* #define HAVE_PRE_INCREMENT */

/* Macros to check register numbers against specific register classes.  */
#define MAX_REGS_PER_ADDRESS 1

/* True if X is an rtx for a constant that is a valid address.

   JPB 29-Aug-10: Why is the default implementation not OK? */
#define CONSTANT_ADDRESS_P(X) 						\
  (GET_CODE (X) == LABEL_REF || GET_CODE (X) == SYMBOL_REF              \
   || GET_CODE (X) == CONST_INT || GET_CODE (X) == CONST                \
   || GET_CODE (X) == HIGH)        

/* A C expression which is nonzero if register number num is suitable for use
   as a base register in operand addresses. Like TARGET_LEGITIMATE_ADDRESS_P,
   this macro should also define a strict and a non-strict variant. Both
   variants behave the same for hard register; for pseudos, the strict variant
   will pass only those that have been allocated to a valid hard registers,
   while the non-strict variant will pass all pseudos.

   Compiler source files that want to use the strict variant of this and other
   macros define the macro REG_OK_STRICT. You should use an #ifdef
   REG_OK_STRICT conditional to define the strict variant in that case and the
   non-strict variant otherwise.

   JPB 29-Aug-10: This has been conflated with the old REG_OK_FOR_BASE_P
                  function, which is no longer part of GCC.

                  I'm not sure this is right. r0 can be a base register, just
                  it can't get set by the user. */
#ifdef REG_OK_STRICT
#define REGNO_OK_FOR_BASE_P(num)					     \
  (   ((0 < (num))             && ((num)             <= OR32_LAST_INT_REG))  \
   || ((0 < reg_renumber[num]) && (reg_renumber[num] <= OR32_LAST_INT_REG)))

#else
/* Accept an int register or a pseudo reg.

   JPB 1-Sep-10: Should this allow r0, if the strict version does not? */
#define REGNO_OK_FOR_BASE_P(num) ((num) <= OR32_LAST_INT_REG ||		\
				  (num) >= FIRST_PSEUDO_REGISTER)
#endif

/* OR32 doesn't have any indexed addressing. */
#define REG_OK_FOR_INDEX_P(X) 0
#define REGNO_OK_FOR_INDEX_P(X) 0


/* OR32 addresses do not depend on the machine mode they are being used in. */
#define GO_IF_MODE_DEPENDENT_ADDRESS(addr,label)

/* Is this suitable for an immediate operand.

   JPB 1-Sep-10: Is this correct. We can only do 16-bit immediates directly. */
#define LEGITIMATE_CONSTANT_P(x) \
  (GET_CODE(x) != CONST_DOUBLE || GET_MODE (x) == VOIDmode)

/* Specify the machine mode that this machine uses for the index in the
   tablejump instruction.  */
#define CASE_VECTOR_MODE SImode

/* Define as C expression which evaluates to nonzero if the tablejump
   instruction expects the table to contain offsets from the address of the
   table.

   Do not define this if the table should contain absolute addresses. */
/* #define CASE_VECTOR_PC_RELATIVE 1 */

/* Define this as 1 if `char' should by default be signed; else as 0.  */
#define DEFAULT_SIGNED_CHAR 1

/* This flag, if defined, says the same insns that convert to a signed fixnum
   also convert validly to an unsigned one.  */
#define FIXUNS_TRUNC_LIKE_FIX_TRUNC

/* The maximum number of bytes that a single instruction can move quickly
   between memory and registers or between two memory locations. */
#define MOVE_MAX 4

/* Define this if zero-extension is slow (more than one real instruction).  */
/* #define SLOW_ZERO_EXTEND */

/* Nonzero if access to memory by bytes is slow and undesirable.  
   For RISC chips, it means that access to memory by bytes is no
   better than access by words when possible, so grab a whole word
   and maybe make use of that.  */
#define SLOW_BYTE_ACCESS 1

/* Define if shifts truncate the shift count
   which implies one can omit a sign-extension or zero-extension
   of a shift count.  */
/* #define SHIFT_COUNT_TRUNCATED */

/* Value is 1 if truncating an integer of INPREC bits to OUTPREC bits
   is done just by pretending it is already truncated.  */
#define TRULY_NOOP_TRUNCATION(OUTPREC, INPREC) 1

/* Specify the machine mode that pointers have.
   After generation of rtl, the compiler makes no further distinction
   between pointers and any other objects of this machine mode.  */
#define Pmode SImode

/* A function address in a call instruction
   is a byte address (for indexing purposes)
   so give the MEM rtx a byte's mode.  */
#define FUNCTION_MODE SImode


/* -------------------------------------------------------------------------- */
/* Condition code stuff */

/* Given a comparison code (EQ, NE, etc.) and the first operand of a COMPARE,
   return the mode to be used for the comparison. */
#define SELECT_CC_MODE(op, x, y)					\
   ((EQ  == (op)) ? CCEQmode						\
  : (NE  == (op)) ? CCNEmode						\
  : (GEU == (op)) ? CCGEUmode						\
  : (GTU == (op)) ? CCGTUmode						\
  : (LTU == (op)) ? CCLTUmode						\
  : (LEU == (op)) ? CCLEUmode						\
  : (GE  == (op)) ? CCGEmode						\
  : (LT  == (op)) ? CCLTmode						\
  : (GT  == (op)) ? CCGTmode						\
  : (LE  == (op)) ? CCLEmode						\
  : (abort (), 0))

/* Can the condition code MODE be safely reversed?  This is safe in
   all cases on this port, because at present it doesn't use the
   trapping FP comparisons (fcmpo).  */
#define REVERSIBLE_CC_MODE(mode) 1

/* Given a condition code and a mode, return the inverse condition.

   JPB 31-Aug-10: This seems like the default. Do we even need this? */
#define REVERSE_CONDITION(code, mode) reverse_condition (code)


/* -------------------------------------------------------------------------- */
/* Control the assembler format that we output.  */

/* A C string constant describing how to begin a comment in the target
   assembler language.  The compiler assumes that the comment will end at
   the end of the line.  */
#define ASM_COMMENT_START "#"

/* Output to assembler file text saying following lines may contain character
   constants, extra white space, comments, etc.

   JPB 29-Aug-10: Default would seem to be OK here. */
#define ASM_APP_ON "#APP\n"

/* Output to assembler file text saying following lines no longer contain
   unusual constructs.

   JPB 29-Aug-10: Default would seem to be OK here. */
#define ASM_APP_OFF "#NO_APP\n"

/* Switch to the text or data segment.  */

/* Output before read-only data.  */
#define TEXT_SECTION_ASM_OP "\t.section .text"

/* Output before writable data.  */
#define DATA_SECTION_ASM_OP "\t.section .data"

/* Output before uninitialized data. */
#define BSS_SECTION_ASM_OP  "\t.section .bss"

/* How to refer to registers in assembler output.  This sequence is indexed by
   compiler's hard-register-number (see above).  */
#define REGISTER_NAMES							\
  {"r0",   "r1",  "r2",  "r3",  "r4",  "r5",  "r6",  "r7",		\
   "r8",   "r9", "r10", "r11", "r12", "r13", "r14", "r15",		\
   "r16", "r17", "r18", "r19", "r20", "r21", "r22", "r23",		\
   "r24", "r25", "r26", "r27", "r28", "r29", "r30", "r31",		\
   "argp", "frame", "cc-flag"}


/* -------------------------------------------------------------------------- */
/* Debug things for DBX (STABS)                                               */
/*                                                                            */
/* Note. Our config.gcc includes dbxelf.h, which sets up appropriate          */
/*       defaults. Choice of which debug format to use is in our elf.h        */
/* -------------------------------------------------------------------------- */

/* Don't try to use the  type-cross-reference character in DBX data.
   Also has the consequence of putting each struct, union or enum
   into a separate .stabs, containing only cross-refs to the others.  */
/* JPB 24-Aug-10: Is this really correct. Can't GDB use this info? */
#define DBX_NO_XREFS
         
/* -------------------------------------------------------------------------- */
/* Debug things for DWARF2                                                    */
/*                                                                            */
/* Note. Choice of which debug format to use is in our elf.h                  */
/* -------------------------------------------------------------------------- */

/* We support frame unwind info including for exceptions handling. This needs
   INCOMING_RETURN_ADDR_RTX to be set and OBJECT_FORMAT_ELF to be defined (in
   elfos.h). Override any default value. */
#undef  DWARF2_UNWIND_INFO
#define DWARF2_UNWIND_INFO 1

/* We want frame info produced. Note that this is superfluous if
   DWARF2_UNWIND_INFO is non-zero, but we set so this so, we can produce frame
   info even when it is zero. Override any default value. */
#undef  DWARF2_FRAME_INFO
#define DWARF2_FRAME_INFO 1

/* Macro specifying which register holds the return address */
#define DWARF_FRAME_RETURN_COLUMN DWARF_FRAME_REGNUM (LINK_REGNUM)

/* Where is the start of our stack frame in relation to the end of the
   previous stack frame at the start of a function, before the prologue */
#define INCOMING_FRAME_SP_OFFSET  0



/* This doesn't work for the OR32 assembler at present. If it did, we'd have
   more compact debug tables. */
/* #undef  DWARF2_ASM_LINE_DEBUG_INFO */
/* #define DWARF2_ASM_LINE_DEBUG_INFO 1 */

/* We don't need an alternative return address for now. */
/* DWARF_ALT_FRAME_RETURN_COLUMN */

/* We always save registers in the prologue with word alignment, so don't
   need this. */
/* DWARF_CIE_DATA_ALIGNMENT */

/* This specifies the maximum number of registers we can save in a frame. We
   could note that only SP, FP, LR, arg regs and callee saved regs come into
   this category. However this is only an efficiency thing, so for now we
   don't use it. */
/* DWARF_FRAME_REGISTERS */

/* This specifies a mapping from register numbers in .dwarf_frame to
   .eh_frame. However for us they are the same, so we don't need it. */
/* DWARF_FRAME_REGNUM */

/* Defined if the DWARF column numbers do not match register numbers. For us
   they do, so this is not needed. */
/* DWARF_REG_TO_UNWIND_COLUMN */

/* Can be used to define a register guaranteed to be zero. Only useful if zero
   is used to terminate backtraces, and not recommended for new ports, so we
   don't use it. */
/* DWARF_ZERO_REG */

/* This is the inverse function for DWARF_FRAME_REGNUM. Again not needed. */
/* DWARF2_FRAME_REG_OUT  */


/* -------------------------------------------------------------------------- */
/* Node: Label Output */

/* Globalizing directive for a label.  */
#define GLOBAL_ASM_OP "\t.global "

#define SUPPORTS_WEAK 1

/* This is how to output the definition of a user-level label named NAME,
   such as the label on a static function or variable NAME.  */
#define ASM_OUTPUT_LABEL(FILE,NAME)					\
  { assemble_name (FILE, NAME); fputs (":\n", FILE); }

/* We use -fleading-underscore to add it, when necessary.
   JPB: No prefix for global symbols */
#define USER_LABEL_PREFIX ""

/* Remove any previous definition (elfos.h).  */
#define ASM_GENERATE_INTERNAL_LABEL(LABEL, PREFIX, NUM)	\
  sprintf (LABEL, "*%s%d", PREFIX, NUM)

/* This is how to output an assembler line defining an int constant.  */
#define ASM_OUTPUT_INT(stream, value)					\
  {									\
    fprintf (stream, "\t.word\t");					\
    output_addr_const (stream, (value));				\
    fprintf (stream, "\n")}

/* This is how to output an assembler line defining a float constant.  */
#define ASM_OUTPUT_FLOAT(stream, value)					\
  { long l;								\
    REAL_VALUE_TO_TARGET_SINGLE (value,l);				\
    fprintf(stream,"\t.word\t0x%08x\t\t# float %26.7e\n", l, value); }

/* This is how to output an assembler line defining a double constant.  */
#define ASM_OUTPUT_DOUBLE(stream, value)				\
  { long l[2];								\
    REAL_VALUE_TO_TARGET_DOUBLE (value,&l[0]);				\
    fprintf(stream,"\t.word\t0x%08x,0x%08x\t# float %26.16le\n",	\
	    l[0],l[1],value); }

/* This is how to output an assembler line defining a long double constant.

   JPB 29-Aug-10: Do we really mean this. I thought long double on OR32 was
                  the same as double. */
#define ASM_OUTPUT_LONG_DOUBLE(stream, value)				\
  { long l[4];								\
    REAL_VALUE_TO_TARGET_DOUBLE (value,&l[0]);				\
    fprintf (stream,							\
	     "\t.word\t0x%08x,0x%08x,0x%08x,0x%08x\t# float %26.18lle\n", \
	     l[0],l[1],l[2],l[3],value); }

/* This is how to output an assembler line defining a short constant.  */
#define ASM_OUTPUT_SHORT(stream, value)					\
  { fprintf (stream, "\t.half\t");					\
    output_addr_const (stream, (value));				\
    fprintf (stream, "\n"); }

/* This is how to output an assembler line defining a char constant.  */
#define ASM_OUTPUT_CHAR(stream, value)					\
  { fprintf (stream, "\t.byte\t");					\
    output_addr_const (stream, (value));				\
    fprintf (stream, "\n")}

/* This is how to output an assembler line for a numeric constant byte.  */
#define ASM_OUTPUT_BYTE(stream, value)  \
  fprintf (stream, "\t.byte\t0x%02x\n", (value))

/* This is how to output an insn to push a register on the stack.
   It need not be very fast code.

    JPB 29-Aug-10: This was using l.sub (since we don't have l.subi), so it
                   was potty code. Replaced by adding immediate -1. */
#define ASM_OUTPUT_REG_PUSH(stream, regno)				\
  { fprintf (stream, "\tl.addi\tr1,-4\n");				\
    fprintf (stream, "\tl.sw\t0(r1),%s\n", reg_names[regno]); }

/* This is how to output an insn to pop a register from the stack.
   It need not be very fast code.  */
#define ASM_OUTPUT_REG_POP(stream,REGNO)				\
  { fprintf (stream, "\tl.lwz\t%s,0(r1)\n", reg_names[REGNO]);		\
    fprintf (stream, "\tl.addi\tr1,4\n"); }

/* This is how to output an element of a case-vector that is absolute.
   (The Vax does not use such vectors,
   but we must define this macro anyway.)  */
#define ASM_OUTPUT_ADDR_VEC_ELT(stream, value)				\
  fprintf (stream, "\t.word\t.L%d\n", value)

/* This is how to output an element of a case-vector that is relative.  */
#define ASM_OUTPUT_ADDR_DIFF_ELT(stream, body, value, rel)		\
  fprintf (stream, "\t.word\t.L%d-.L%d\n", value, rel)

#define JUMP_TABLES_IN_TEXT_SECTION (flag_pic)
/* ??? If we were serious about PIC, we should also use l.jal to get
   the table start address.  */

/* This is how to output an assembler line that says to advance the location
   counter to a multiple of 2**log bytes.  */
#define ASM_OUTPUT_ALIGN(stream, log)					\
  if ((log) != 0)							\
    {									\
      fprintf (stream, "\t.align\t%d\n", 1 << (log));			\
    }

/* This is how to output an assembler line that says to advance the location
   counter by "size" bytes.  */
#define ASM_OUTPUT_SKIP(stream, size)					\
  fprintf (stream, "\t.space %d\n", (size))

/* Need to split up .ascii directives to avoid breaking
   the linker. */

/* This is how to output a string.  */
#define ASM_OUTPUT_ASCII(stream, ptr, len)				\
  output_ascii_pseudo_op (stream, (const unsigned char *) (ptr), len)

/* Invoked just before function output. */
#define ASM_OUTPUT_FUNCTION_PREFIX(stream, fnname)			\
  { fputs (".proc\t", stream); assemble_name (stream, fnname);		\
    fputs ("\n", stream); }

/* This says how to output an assembler line to define a global common
   symbol. */
#define ASM_OUTPUT_COMMON(stream,name,size,rounded)			\
  { data_section ();							\
    fputs ("\t.global\t", stream);					\
    assemble_name(stream, name);					\
    fputs ("\n", stream);						\
    assemble_name (stream, name);					\
    fputs (":\n", stream);						\
    fprintf (stream, "\t.space\t%d\n", rounded); }

/* This says how to output an assembler line to define a local common
   symbol.

   JPB 29-Aug-10: I'm sure this doesn't work - we don't have a .bss directive
   like this. */
#define ASM_OUTPUT_LOCAL(stream, name, size, rounded)			\
  { fputs ("\t.bss\t", (stream));					\
    assemble_name ((stream), (name));					\
    fprintf ((stream), ",%d,%d\n", (size), (rounded)); }

/* This says how to output an assembler line to define a global common symbol
   with size "size" (in bytes) and alignment "align" (in bits).  */
#define ASM_OUTPUT_ALIGNED_COMMON(stream, name, size, align)		\
  { data_section();							\
    if ((ALIGN) > 8)							\
      {									\
	fprintf(stream, "\t.align %d\n", ((align) / BITS_PER_UNIT));	\
      }									\
    fputs("\t.global\t", stream); assemble_name(stream, name);      	\
    fputs("\n", stream);                                        	\
    assemble_name(stream, name);                                	\
    fputs (":\n", stream);						\
    fprintf(stream, "\t.space\t%d\n", size); }
  
/* This says how to output an assembler line to define a local common symbol
   with size "size" (in bytes) and alignment "align" (in bits).  */
#define ASM_OUTPUT_ALIGNED_LOCAL(stream, name, size, align)		\
  { data_section();							\
    if ((align) > 8)							\
      {									\
	fprintf(stream, "\t.align %d\n", ((align) / BITS_PER_UNIT));	\
      }									\
    assemble_name(stream, name);					\
    fputs (":\n", stream);						\
    fprintf(stream, "\t.space %d\n", size); }
                                                     
/* Store in "output" a string (made with alloca) containing an assembler-name
   for a local static variable named "name".  "labelno" is an integer which is
   different for each call.  */
#define ASM_FORMAT_PRIVATE_NAME(output, name, labelno)			\
  { (output) = (char *) alloca (strlen ((name)) + 10);			\
    sprintf ((output), "%s.%lu", (name), (unsigned long int) (labelno)); }

/* Macro for %code validation. Returns nonzero if valid.

   The acceptance of '(' is an idea taken from SPARC; output nop for %( if not
   optimizing or the slot is not filled. */
#define PRINT_OPERAND_PUNCT_VALID_P(code)  (('(' == code) || ('%' == code))

/* Print an instruction operand "x" on file "stream".  "code" is the code from
   the %-spec that requested printing this operand; if `%z3' was used to print
   operand 3, then CODE is 'z'.  */
#define PRINT_OPERAND(stream, x, code)					\
{									\
  if (code == 'r'							\
      && GET_CODE (x) == MEM						\
      && GET_CODE (XEXP (x, 0)) == REG)					\
    {									\
      fprintf (stream, "%s", reg_names[REGNO (XEXP (x, 0))]);		\
    }									\
  else if (code == '(')							\
    {									\
      if (dbr_sequence_length ())					\
	fprintf (stream, "\t# delay slot filled");			\
      else								\
	fprintf (stream, "\n\tl.nop\t\t\t# nop delay slot");		\
    }									\
  else if (code == 'C')							\
    {									\
      switch (GET_CODE (x))						\
	{								\
        case EQ:							\
	  fputs ("eq", stream);						\
	  break;							\
	case NE:							\
	  fputs ("ne", stream);						\
	  break;							\
	case GT:							\
	  fputs ("gts", stream);					\
	  break;							\
        case GE:							\
	  fputs ("ges", stream);					\
	  break;							\
	case LT:							\
	  fputs ("lts", stream);					\
	  break;							\
	case LE:							\
	  fputs ("les", stream);					\
	  break;							\
	case GTU:							\
	  fputs ("gtu", stream);					\
	  break;							\
	case GEU:							\
	  fputs ("geu", stream);					\
	  break;							\
	case LTU:							\
	  fputs ("ltu", stream);					\
	  break;							\
	case LEU:							\
	  fputs ("leu", stream);					\
	  break;							\
	default:							\
	  abort ();							\
	}								\
    }									\
  else if (code == 'H')							\
    {									\
      if (GET_CODE (x) == REG)						\
	fprintf (stream, "%s", reg_names[REGNO (x) + 1]);		\
      else								\
	abort ();							\
    }									\
  else if (code == 'J')							\
    or32_print_jump_restore (x);					\
  else if (GET_CODE (x) == REG)						\
    fprintf (stream, "%s", reg_names[REGNO (x)]);			\
  else if (GET_CODE (x) == MEM)						\
    output_address (XEXP (x, 0));					\
  else									\
    output_addr_const (stream, x);					\
}

/* Print a memory operand whose address is "addr", on file "stream".
   This uses a function in output-vax.c.  */
#define PRINT_OPERAND_ADDRESS(stream, addr)				\
{									\
  rtx offset;								\
									\
  switch (GET_CODE (addr))						\
    {									\
    case MEM:								\
      if (GET_CODE (XEXP (addr, 0)) == REG)				\
	fprintf (stream, "%s", reg_names[REGNO (addr)]);		\
      else								\
	abort ();							\
      break;								\
									\
    case REG:								\
      fprintf (stream, "0(%s)", reg_names[REGNO (addr)]);		\
      break;								\
									\
    case PLUS:								\
      offset = 0;							\
      if (GET_CODE (XEXP (addr, 0)) == REG)				\
	{								\
          offset = XEXP (addr, 1);					\
	  addr   = XEXP (addr, 0);					\
	}								\
      else if (GET_CODE (XEXP (addr, 1)) == REG)			\
	{								\
          offset = XEXP (addr, 0);					\
	  addr   = XEXP (addr, 1);					\
	}								\
      output_address (offset);						\
      fprintf (stream, "(%s)", reg_names[REGNO (addr)]);		\
      break;								\
									\
    default:								\
      output_addr_const (stream, addr);					\
    }									\
}

/* The size of the trampoline in bytes. This is a block of code followed by
   two words specifying the function address and static chain pointer. */
#define TRAMPOLINE_SIZE							\
  (or32_trampoline_code_size () + GET_MODE_SIZE (ptr_mode) * 2)

/* Alignment required for trampolines, in bits.

   For the OR32, there is no need for anything other than word alignment. */
#define TRAMPOLINE_ALIGNMENT  32

/* Mark functions for garbage collection. */
extern GTY(()) rtx or32_compare_op0;
extern GTY(()) rtx or32_compare_op1;

/* Enable parsing of #pragma pack(push,<n>) and #pragma pack(pop).  */
#define HANDLE_PRAGMA_PACK_PUSH_POP

/* GLIBC is not implemented, but we handle the selection for consistency
   with the Linux framework.  */
enum or32_libc_kind {  or32_libc_newlib, or32_libc_uclibc, or32_libc_glibc };

#endif /* _OR32_H_ */
