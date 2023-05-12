ifeq ($(CFG_BUILD_LEGACY_TA),y)
link-script$(sm) = $(ta-dev-kit-dir$(sm))/src/legacy_ta.ld.S
else
link-script$(sm) = $(ta-dev-kit-dir$(sm))/src/ta.ld.S
endif
link-script-pp$(sm) = $(link-out-dir$(sm))/ta.lds
link-script-dep$(sm) = $(link-out-dir$(sm))/.ta.ld.d

SIGN ?= $(ta-dev-kit-dir$(sm))/scripts/sign.py
ifeq ($(CFG_SUNXI_SIGN_KEY_V2),y)
TA_SIGN_KEY ?= $(TA_DEV_KIT_DIR)/keys/default_ta_v2.pem
else
TA_SIGN_KEY ?= $(TA_DEV_KIT_DIR)/keys/default_ta.pem
endif

all: $(link-out-dir$(sm))/$(user-ta-uuid).dmp \
	$(link-out-dir$(sm))/$(user-ta-uuid).stripped.elf \
	$(link-out-dir$(sm))/$(user-ta-uuid).ta
cleanfiles += $(link-out-dir$(sm))/$(user-ta-uuid).elf
cleanfiles += $(link-out-dir$(sm))/$(user-ta-uuid).dmp
cleanfiles += $(link-out-dir$(sm))/$(user-ta-uuid).map
cleanfiles += $(link-out-dir$(sm))/$(user-ta-uuid).stripped.elf
cleanfiles += $(link-out-dir$(sm))/$(user-ta-uuid).ta
cleanfiles += $(link-script-pp$(sm)) $(link-script-dep$(sm))

link-ldflags  = -e__ta_entry -pie
link-ldflags += -T $(link-script-pp$(sm))
link-ldflags += -Map=$(link-out-dir$(sm))/$(user-ta-uuid).map
link-ldflags += --sort-section=alignment
link-ldflags += -z max-page-size=4096 # OP-TEE always uses 4K alignment
link-ldflags += --as-needed # Do not add dependency on unused shlib
link-ldflags += $(link-ldflags$(sm))

ifeq ($(CFG_TA_FTRACE_SUPPORT),y)
$(link-out-dir$(sm))/dyn_list:
	@$(cmd-echo-silent) '  GEN     $@'
	$(q)mkdir -p $(dir $@)
	$(q)echo "{__ftrace_info;};" >$@
link-ldflags += --dynamic-list $(link-out-dir$(sm))/dyn_list
ftracedep = $(link-out-dir$(sm))/dyn_list
cleanfiles += $(link-out-dir$(sm))/dyn_list
endif

link-ldadd = $(TA_PRIVATE_FLAGS)
link-ldadd += $(user-ta-ldadd) $(addprefix -L,$(libdirs))
link-ldadd += --start-group $(addprefix -l,$(libnames)) --end-group
ldargs-$(user-ta-uuid).elf := $(link-ldflags) $(objs) $(link-ldadd)


link-script-cppflags-$(sm) := \
	$(filter-out $(CPPFLAGS_REMOVE) $(cppflags-remove), \
		$(nostdinc$(sm)) $(CPPFLAGS) \
		$(addprefix -I,$(incdirs$(sm)) $(link-out-dir$(sm))) \
		$(cppflags$(sm)))

-include $(link-script-dep$(sm))

link-script-pp-makefiles$(sm) = $(filter-out %.d %.cmd,$(MAKEFILE_LIST))

ifeq ($(CFG_SUNXI_TA_ENCRYPT_SUPPORT)$(SUNXI_TA_ENCRYPT),yy)
TA_AES_KEY ?= $(TA_DEV_KIT_DIR)/keys/ta_aes_key.bin
ifeq ($(USING_DERIVE_KEY),y)
TA_ENCRYPT_FLAGS += --using-derive-key
endif
endif

define gen-link-t
$(link-script-pp$(sm)): $(link-script$(sm)) $(conf-file) $(link-script-pp-makefiles$(sm))
	@$(cmd-echo-silent) '  CPP     $$@'
	$(q)mkdir -p $$(dir $$@)
	$(q)$(CPP$(sm)) -P -MT $$@ -MD -MF $(link-script-dep$(sm)) \
		$(link-script-cppflags-$(sm)) $$< -o $$@

$(link-out-dir$(sm))/$(user-ta-uuid).elf: $(objs) $(libdeps) \
					  $(link-script-pp$(sm)) \
					  $(ftracedep) \
					  $(additional-link-deps)
	@$(cmd-echo-silent) '  LD      $$@'
	$(q)$(LD$(sm)) $(ldargs-$(user-ta-uuid).elf) -o $$@

$(link-out-dir$(sm))/$(user-ta-uuid).dmp: \
			$(link-out-dir$(sm))/$(user-ta-uuid).elf
	@$(cmd-echo-silent) '  OBJDUMP $$@'
	$(q)$(OBJDUMP$(sm)) -l -x -d $$< > $$@

$(link-out-dir$(sm))/$(user-ta-uuid).stripped.elf: \
			$(link-out-dir$(sm))/$(user-ta-uuid).elf
	@$(cmd-echo-silent) '  OBJCOPY $$@'
	$(q)$(OBJCOPY$(sm)) --strip-unneeded $$< $$@

ifeq ($(CFG_SUNXI_TA_ENCRYPT_SUPPORT)$(SUNXI_TA_ENCRYPT),yy)
$(link-out-dir$(sm))/$(user-ta-uuid).ta: \
			$(link-out-dir$(sm))/$(user-ta-uuid).stripped.elf \
			$(TA_SIGN_KEY)
	@echo '  SIGN AND AES ENCRYPT    $$@'
	$(q)$(SIGN) --key $(TA_SIGN_KEY) --uuid $(user-ta-uuid) \
		--in $$< --out $$@ --aes_key $(TA_AES_KEY) \
		$(TA_ENCRYPT_FLAGS)
else
$(link-out-dir$(sm))/$(user-ta-uuid).ta: \
			$(link-out-dir$(sm))/$(user-ta-uuid).stripped.elf \
			$(TA_SIGN_KEY)
	@echo '  SIGN    $$@'
	$(q)$(SIGN) --key $(TA_SIGN_KEY) --uuid $(user-ta-uuid) \
		--in $$< --out $$@
endif
endef

$(eval $(call gen-link-t))

additional-link-deps :=
