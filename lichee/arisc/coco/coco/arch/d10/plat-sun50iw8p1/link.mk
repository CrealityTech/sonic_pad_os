link-out-dir = $(out-dir)/coco

link-script = $(platform-dir)/kern.ld.S
link-script-pp = $(link-out-dir)/kern.ld
link-script-dep = $(link-out-dir)/.kern.ld.d

OBJS := $(objs)

CUR_DIR := $(shell pwd)

LD_PATH  = $(CUR_DIR)

#LDSCRIPT = $(LD_PATH)/d10.ld

#SRCDIRS = $(shell find ./../ -maxdepth 6 -type d)

#INCLUDES := $(INCLUDES) \
#	$(foreach dir,$(SRCDIRS),-I$(dir))

#TLDSAG_OUTOOLINCLUDES := $(TOOLINCLUDES) \
#	$(foreach dir,$(TOOLPATCH),-I$(dir))

AFLAGS =
CEXTLDFLAGS += -Wl,--mno-ex9
CEXTLDFLAGS += -lgcov

$(CROSS_COMPILE)gcc -O0 -nostartfiles -static -mcmodel=medium -T"$(LDSAG_OUT)" -Wl,--gc-sections -mvh  $(OBJS) $(USER_OBJS) $(LIBS)^M
LDFLAGS = -Os -nostartfiles -static -T $(link-script-pp) -Wl,-Map,link.map -mrelax
OBJCOPYFLAGS = -O binary -R .note -R .comment -S
NMFLAGS = -B -n
TARGET = $(link-out-dir)

cleanfiles += $(link-script-pp) $(link-script-dep)
$(link-script-pp): $(link-script) $(link-script-extra-deps)
	@$(cmd-echo-silent) '  CPP     $@'
	@mkdir -p $(dir $@)
	$(q)$(CPPcore) -Wp,-P,-MT,$@,-MD,$(link-script-dep) \
		$(link-script-cppflags) $< > $@


all: $(TARGET).elf

$(TARGET).elf: $(OBJS) $(link-script-pp)
	@ $(CCcore) $(LDFLAGS) $(OBJS) $(CEXTLDFLAGS) -o $(TARGET).elf
	$(OBJDUMPcore) -x -d $(TARGET).elf > $(TARGET).s
	$(OBJCOPYcore) $(OBJCOPYFLAGS) $(TARGET).elf $(TARGET).bin
	$(NMcore) $(NMFLAGS) $(TARGET).elf > $(TARGET).map
#	hexdump -v -e '/1 "%02x" "\n"' $(TARGET).bin > rama2.hex











