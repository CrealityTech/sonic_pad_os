ifeq (,$(shlibuuid))
$(error SHLIBUUID not set)
endif
link-out-dir = $(out-dir)

SIGN ?= $(TA_DEV_KIT_DIR)/scripts/sign.py
ifeq ($(CFG_SUNXI_SIGN_KEY_V2),y)
TA_SIGN_KEY ?= $(TA_DEV_KIT_DIR)/keys/default_ta_v2.pem
else
TA_SIGN_KEY ?= $(TA_DEV_KIT_DIR)/keys/default_ta.pem
endif

all: $(link-out-dir)/$(shlibname).so $(link-out-dir)/$(shlibname).dmp \
	$(link-out-dir)/$(shlibname).stripped.so \
	$(link-out-dir)/$(shlibuuid).elf \
	$(link-out-dir)/$(shlibuuid).ta

cleanfiles += $(link-out-dir)/$(shlibname).so
cleanfiles += $(link-out-dir)/$(shlibname).dmp
cleanfiles += $(link-out-dir)/$(shlibname).stripped.so
cleanfiles += $(link-out-dir)/$(shlibuuid).elf
cleanfiles += $(link-out-dir)/$(shlibuuid).ta

shlink-ldflags  = $(LDFLAGS)
shlink-ldflags += -shared -z max-page-size=4096
shlink-ldflags += --as-needed # Do not add dependency on unused shlib

shlink-ldadd  = $(LDADD)
shlink-ldadd += $(addprefix -L,$(libdirs))
shlink-ldadd += --start-group $(addprefix -l,$(libnames)) --end-group
ldargs-$(shlibname).so := $(shlink-ldflags) $(objs) $(shlink-ldadd)


$(link-out-dir)/$(shlibname).so: $(objs) $(libdeps)
	@$(cmd-echo-silent) '  LD      $@'
	$(q)$(LD$(sm)) $(ldargs-$(shlibname).so) --soname=$(shlibuuid) -o $@

$(link-out-dir)/$(shlibname).dmp: $(link-out-dir)/$(shlibname).so
	@$(cmd-echo-silent) '  OBJDUMP $@'
	$(q)$(OBJDUMP$(sm)) -l -x -d $< > $@

$(link-out-dir)/$(shlibname).stripped.so: $(link-out-dir)/$(shlibname).so
	@$(cmd-echo-silent) '  OBJCOPY $@'
	$(q)$(OBJCOPY$(sm)) --strip-unneeded $< $@

$(link-out-dir)/$(shlibuuid).elf: $(link-out-dir)/$(shlibname).so
	@$(cmd-echo-silent) '  LN      $@'
	$(q)ln -sf $(<F) $@

ifeq ($(CFG_SUNXI_TA_ENCRYPT_SUPPORT)$(SUNXI_TA_ENCRYPT),yy)
TA_AES_KEY ?= $(TA_DEV_KIT_DIR)/keys/ta_aes_key.bin
ifeq ($(USING_DERIVE_KEY),y)
TA_ENCRYPT_FLAGS += --using-derive-key
endif
$(link-out-dir)/$(shlibuuid).ta: $(link-out-dir)/$(shlibname).stripped.so \
				$(TA_SIGN_KEY)
	@echo '  SIGN    $@'
	$(q)$(SIGN) --key $(TA_SIGN_KEY) --uuid $(shlibuuid) --ta-version 0 \
		--in $< --out $@ --aes_key $(TA_AES_KEY) \
		$(TA_ENCRYPT_FLAGS)
else
$(link-out-dir)/$(shlibuuid).ta: $(link-out-dir)/$(shlibname).stripped.so \
				$(TA_SIGN_KEY)
	@$(cmd-echo-silent) '  SIGN    $@'
	$(q)$(SIGN) --key $(TA_SIGN_KEY) --uuid $(shlibuuid) \
		--in $< --out $@
endif
