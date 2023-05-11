SHELL=bash
# SUB_VER=1.0.0
PATH :=$(CURDIR)/coco/tools/arch_toolchain/openrisc_toolchain/bin:$(PATH)
PATH :=$(CURDIR)/coco/tools/bin:$(PATH)
MAKEFLAGS += --no-print-directory
LD_LIBRARY_PATH:=$(LD_LIBRARY_PATH):$(CURDIR)/coco/tools/arch_toolchain/openrisc_toolchain/lib
include scripts/Build.include

-include .config
include coco/arch/openrisc/Makefile

ifeq ($(V),1)
  quiet =
  Q =
else
  quiet=quiet_
  Q=@
endif

CC=or32-elf-gcc
LD=or32-elf-ld
AR=or32-elf-ar
OBJC=or32-elf-objcopy
OBJDUMP=or32-elf-objdump
NM=or32-elf-nm
OBJ=or32-elf-objdump
SIZE=or32-elf-size

CFLAGS += -Ibytb/include/service
CFLAGS += -Ibytb/include
CFLAGS += -Icoco/include
CFLAGS += -Icoco/include/driver
CFLAGS += -Icoco/include/service
CFLAGS += $(CFLAGS-y)
CFLAGS += -Icoco/include/system
CFLAGS += -Ibytb/include/service
CFLAGS += -Iinclude/generated
CFLAGS += -include autoconf.h
CFLAGS += -include conf.h
# CFLAGS += -D"SUB_VER=BUILD_STR($(SUB_VER))"
# CFLAGS += -D"SUB_VER=$(SUB_VER)"


# must change to platform
# CFLAGS += -Iout/openrisc-plat-sun8iw12p1/include/generated

# must add from config
# CFLAGS += -DSUB_VER=3

# DRAM and ddr type
export CC LD AR OBJCOPY OBJDUMP LD_LIBRARY_PATH
export CFLAGS
export HOSTCC := gcc
export CONFIG_ = CFG_




.PHONY : bytb coco FORCE menuconfig $(FORCE)


define filechk_version
	(echo "/*";\
	echo " * THIS IS CREATE WITH VERSION CHK";\
	echo " * DO NOT CHANGE IT MANUAL";\
	echo " */";\
	echo ;\
	echo "#ifndef _SUB_VER_";\
	echo "#define _SUB_VER_";\
	echo ;\
	echo "#define SUB_VER \"`scripts/setlocalversion`\"";\
	echo ;\
	echo "#endif /* _SUB_VER_ */";)
endef

target_name = $(OUTPUT)scp
all: $(target_name).bin

sub_dir := coco bytb

prepare := include/generated/conf.h scripts/basic/fixdep
prepare += include/generated/autoconf.h

$(addprefix $(OUTPUT),$(addsuffix /obj-in.o,$(sub_dir))): $(prepare) FORCE
	$(Q)$(MAKE) -f scripts/Makefile.build dir=$(notdir $(patsubst %/,%,$(dir $@)))

clean:
	$(Q)echo CLEAN  objects
	$(Q)find -name '*.o' -delete -o  -name '.*.cmd' -delete -o -name '.*.d' -delete
	$(Q)find -name '*.tmp' -delete
	$(Q)find -wholename './scripts/basic/fixdep' -delete
	$(Q)find -path 'include/generated' -delete
	$(Q)if test -d include;then rm include -r;fi
	$(Q)find -wholename './scp.*' -delete


include/generated/conf.h: Makefile FORCE
	$(call filechk,version)

# because old project use CFG_ so we change it
menuconfig:FORCE
	@scripts/mconf Kconfig

PHONY += savedefconfig

savedefconfig:FORCE
	$(Q)echo save defconfig to defconfig
	$(Q)scripts/conf --savedefconfig defconfig Kconfig

include/generated/autoconf.h:.config
	$(Q)mkdir -p include/generated
	$(Q)mkdir -p include/config
	$(Q)scripts/conf --silentoldconfig Kconfig

.config:
	$(Q)echo use make menuconfig to generate config file
	$(Q)echo
	$(Q)false


config_save:FORCE
	$(Q)scripts/conf --savedefconfig defconfig Kconfig
	$(Q)(read -e -i def -p '(Config file name?)' config_file;\
	if [ $${config_file} ]; \
	then \
	mv defconfig coco/arch/configs/$${config_file}_defconfig;\
       	fi;)

test:
	@echo CFLAGS_ARCH=$(CFLAGS-y)

$(notdir $(wildcard coco/arch/configs/*_defconfig)): FORCE
	$(Q)scripts/conf --defconfig=coco/arch/configs/$@ Kconfig

scripts/basic/fixdep: scripts/basic/fixdep.c
	$(Q)$(if $(OUTPUT),mkdir -p $(OUTPUT),)
	$(Q)gcc $^ -o $(if $(OUTPUT),$(OUTPUT:%/=%)/fixdep,$@)




# copy from ar100
#openrisc arch config options
MARCH_FLAGS ?=-mhard-mul -msoft-div -msoft-float

#compiler config options
CFLAGS  += -g -c -nostdlib -Wall -Werror -Os $(MARCH_FLAGS)

#linker config options
LD_FLAGS= --stats -T$(LINK_RAM-y) -e 0x100

# all: ar100.elf  ar100.raw ar100.dis  system.map ar100.bin ar100.hex

$(target_name).dis: $(target_name).elf
	$(Q)$(OBJ) -S -d $< > $@

$(target_name).raw: $(target_name).elf
	$(Q)$(OBJC) -O binary $< $@

$(target_name).elf:$(addsuffix /obj-in.o,$(addprefix $(OUTPUT),$(sub_dir))) FORCE
	@printf "\r\n\t--- Linking ---\r\n"
	$(Q)$(LD) -Map $(basename $@).map -Bstatic $(addprefix $(OUTPUT),$(addsuffix /obj-in.o,$(sub_dir))) $(LD_FLAGS) $(shell cat .lib) -o $@
	$(Q)$(NM) $@ | \
		grep -v '\(compiled\)\|\(\.o$$\)\|\( [aUw] \)\|\(\.\.ng$$\)\|\(LASH[RL]DI\)' | \
		sort >> $(basename $@).map
		$(Q)$(OBJ) -x -d -S $@ >> $(basename $@).map
	$(Q)$(SIZE) $(addprefix $(OUTPUT),$(addsuffix /obj-in.o,$(sub_dir))) $@

$(target_name).bin: $(target_name).raw
	be2le $^ $@
	@if [ $${LICHEE_BRANDY_OUT_DIR} ];then cp $@ $${LICHEE_BRANDY_OUT_DIR}; \
		echo copy $@ to $${LICHEE_BRANDY_OUT_DIR};\
		fi

$(target_name).hex: $(target_name).bin
	bin2hex -i $^ -o $@ -s 0x4000

objs_size: FORCE
	$(Q)find $(OUTPUT) -name '*.o' -exec $(SIZE) {} \+

