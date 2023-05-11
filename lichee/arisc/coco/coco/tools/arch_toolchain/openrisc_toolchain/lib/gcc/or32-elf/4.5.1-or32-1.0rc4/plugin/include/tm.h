#ifndef GCC_TM_H
#define GCC_TM_H
#ifndef OR32_LIBC_DEFAULT
# define OR32_LIBC_DEFAULT or32_libc_newlib
#endif
#ifdef IN_GCC
# include "options.h"
# include "config/or32/or32.h"
# include "config/dbxelf.h"
# include "config/elfos.h"
# include "config/newlib-stdint.h"
# include "config/or32/elf.h"
# include "defaults.h"
#endif
#if defined IN_GCC && !defined GENERATOR_FILE && !defined USED_FOR_TARGET
# include "insn-constants.h"
# include "insn-flags.h"
#endif
#endif /* GCC_TM_H */
