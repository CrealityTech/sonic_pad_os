link-out-dir = $(out-dir)/coco

link-script = $(platform-dir)/kern.ld.S
link-script-pp = $(link-out-dir)/kern.ld
link-script-dep = $(link-out-dir)/.kern.ld.d

link-ldflags  = $(LDFLAGS)
link-ldflags += -T $(link-script-pp) -Map=$(link-out-dir)/coco.map
link-ldflags += --sort-section=alignment
link-ldflags += --fatal-warnings

link-ldadd  = $(LDADD)
link-ldadd += $(addprefix -L,$(libdirs))
link-ldadd += $(addprefix -l,$(libnames))
link-objs := $(objs)
link-objs := $(filter-out $(out-dir)/core/arch/arm/kernel/link_dummies.o, \
		  $(objs))
ldargs-coco.elf := $(link-ldflags) $(link-objs) $(link-ldadd) $(libgcccore)
#$(link-out-dir)/version.o \

link-script-cppflags := -DASM=1  \
	$(filter-out $(CPPFLAGS_REMOVE) $(cppflags-remove), \
		$(nostdinccore) $(CPPFLAGS) \
		$(addprefix -I,$(incdirscore) $(link-out-dir)) \
		$(cppflagscore))

ldargs-all_objs := -T $(platform-dir)/ram.ld -e 0x100\
		$(link-objs) $(link-ldadd) $(libgcccore)
cleanfiles += $(link-out-dir)/all_objs.o
$(link-out-dir)/all_objs.o: $(objs) $(libdeps) $(MAKEFILE_LIST)
	$(cmd-echo-silent) '  LD      $@'
	echo $(link-script-cppflags)
	$(q)$(LDcore) $(ldargs-all_objs) -o $@

#link-script-extra-deps += $(link-out-dir)/text_unpaged.ld.S
#link-script-extra-deps += $(link-out-dir)/rodata_unpaged.ld.S
#link-script-extra-deps += $(link-out-dir)/text_init.ld.S
#link-script-extra-deps += $(link-out-dir)/rodata_init.ld.S
link-script-extra-deps += $(conf-file)
cleanfiles += $(link-script-pp) $(link-script-dep)
103 $(link-script-pp): $(link-script) $(link-script-extra-deps)
	@$(cmd-echo-silent) '  CPP     $@'
	@mkdir -p $(dir $@)
	$(q)$(CPPcore) -Wp,-P,-MT,$@,-MD,$(link-script-dep) \
		$(link-script-cppflags) $< > $@

all: $(link-out-dir)/coco.elf
cleanfiles += $(link-out-dir)/coco.elf $(link-out-dir)/coco.map
$(link-out-dir)/coco.elf: $(link-objs) $(libdeps) $(link-script-pp)
	@$(cmd-echo-silent) '  LD      $@'
	$(q)$(LDcore) $(ldargs-coco.elf) -o $@

all: $(link-out-dir)/coco.dmp
cleanfiles += $(link-out-dir)/coco.dmp
$(link-out-dir)/coco.dmp: $(link-out-dir)/coco.elf
	@$(cmd-echo-silent) '  OBJDUMP $@'
	$(q)$(OBJDUMPcore) -l -x -d $< > $@

all: $(link-out-dir)/scp.bin
cleanfiles += $(link-out-dir)/scp.bin
cleanfiles += $(link-out-dir)/coco.raw
$(link-out-dir)/coco.raw: $(link-out-dir)/coco.elf
	$(Q)$(OBJCOPYcore) -O binary $< $@

$(link-out-dir)/scp.bin: $(link-out-dir)/coco.raw
	be2le $< $@
