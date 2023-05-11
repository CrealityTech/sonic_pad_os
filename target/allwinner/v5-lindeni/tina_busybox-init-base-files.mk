############################################################################################
# 			v316-sdv busybox-init-base-files for tina(OpenWrt) Linux
#
#	v316-sdv busybox-init-base-files will generate shell script according to some 
# environmental variables. so tina_busybox-init-base-files.mk is needed.
#
# Version: v1.0
# Date   : 2019-7-19
# Author : PDC-PD5
############################################################################################
TARGET_DIR := $(CURDIR)/busybox-init-base-files
HOOKS := $(CURDIR)/busybox-init-base-files_generate/rootfs_hook_squash.sh
all:
	@echo ==================================================
	@echo $(CURDIR)/tina_busybox-init-basefiles.mk is called to generate shell scripts
	@echo ==================================================
	(${HOOKS} ${TARGET_DIR} >/dev/null) || { \
		echo "Execute the ${HOOKS} is failed"; \
		exit 1; \
	}
	@echo generate shell scripts done!

clean:
	@echo ==================================================
	@echo $(CURDIR)/tina_busybox-init-basefiles.mk is called to clean shell scripts
	@echo ==================================================
	@echo clean shell scripts done!
