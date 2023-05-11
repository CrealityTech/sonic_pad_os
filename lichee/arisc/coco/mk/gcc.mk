ifeq ($(ARCH),openrisc)
CC$(sm)		:= or32-elf-gcc
CPP$(sm)	:= or32-elf-cpp
LD$(sm)		:= or32-elf-ld
AR$(sm)		:= or32-elf-ar
NM$(sm)		:= or32-elf-nm
OBJCOPY$(sm)	:= or32-elf-objcopy
OBJDUMP$(sm)	:= or32-elf-objdump
READELF$(sm)	:= or32-elf-readelf
endif

ifeq ($(ARCH),d10)
CC$(sm)		:= nds32le-elf-gcc
CPP$(sm)	:= nds32le-elf-cpp
LD$(sm)		:= nds32le-elf-ld
AR$(sm)		:= nds32le-elf-ar
NM$(sm)		:= nds32le-elf-nm
OBJCOPY$(sm)	:= nds32le-elf-objcopy
OBJDUMP$(sm)	:= nds32le-elf-objdump
READELF$(sm)	:= nds32le-elf-readelf
endif
nostdinc$(sm)	:= -nostdinc -isystem $(shell $(CC$(sm)) \
			-print-file-name=include 2> /dev/null)

# Get location of libgcc from gcc
libgcc$(sm)  	:= $(shell $(CC$(sm)) $(CFLAGS$(arch-bits-$(sm))) $(comp-cflags$(sm)) \
			-print-libgcc-file-name 2> /dev/null)

# Define these to something to discover accidental use
CC		:= false
CPP		:= false
LD		:= false
AR		:= false
NM		:= false
OBJCOPY		:= false
OBJDUMP		:= false
READELF		:= false
nostdinc	:= --bad-nostdinc-variable
libgcc  	:= --bad-libgcc-variable


