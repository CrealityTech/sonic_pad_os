include $(TOPDIR)/target/allwinner/$(TARGET_BOARD)/BoardConfig.mk

CREALITY_GIT_URL_PREFIX=git@127.0.0.1

#后端主版本号
KLIPPER-BRAIN_MAJOR_VERSION:=1
#后端次版本号
KLIPPER-BRAIN_MINOR_VERSION:=9
#后端迭代版本号
KLIPPER-BRAIN_LTERATION_VERSION:=1

#前端主版本号
FLUIDD-PAD_MAJOR_VERSION:=1
#前端次版本号
FLUIDD-PAD_MINOR_VERSION:=2
#前端迭代版本号
FLUIDD-PAD_LTERATION_VERSION:=56

#拓展版本号
C-SMART-BRAIN-EXTERN_VERSION:=0

#硬件主版本
C-SMART-BRAIN-SWU_HW_VERSION=$(TARGET_BOARD_HW_MAJOR_VERSION)
#拓展版本
C-SMART-BRAIN-SWU_EXTERN_VERSION=$(C-SMART-BRAIN-EXTERN_VERSION)
#功能版本
C-SMART-BRAIN-SWU_FUNC_VERSION=$(shell expr $(KLIPPER-BRAIN_MAJOR_VERSION) + $(FLUIDD-PAD_MAJOR_VERSION) + \
											$(TARGET_BOARD_SYS_MAJOR_VERSION))
#系统版本
C-SMART-BRAIN-SWU_SYS_VERSION=$(shell expr $(KLIPPER-BRAIN_MINOR_VERSION) + $(FLUIDD-PAD_MINOR_VERSION) + \
											$(TARGET_BOARD_SYS_MINOR_VERSION))
#迭代版本
C-SMART-BRAIN-SWU_LTERATION_VERSION=$(shell expr $(KLIPPER-BRAIN_LTERATION_VERSION) + $(FLUIDD-PAD_LTERATION_VERSION))

#最终版本号
C-SMART-BRAIN-SWU_VERSION=$(C-SMART-BRAIN-SWU_HW_VERSION).$(C-SMART-BRAIN-SWU_EXTERN_VERSION).$(C-SMART-BRAIN-SWU_FUNC_VERSION).$(C-SMART-BRAIN-SWU_SYS_VERSION).$(C-SMART-BRAIN-SWU_LTERATION_VERSION)

$(info "the SWU image version :  $(C-SMART-BRAIN-SWU_VERSION)")

#modify the ota config
SWU_VERSION_FILE=$(LICHEE_CHIP_CONFIG_DIR)/configs/$(subst $(TARGET_PLATFORM)-,,$(TARGET_BOARD))/linux/env-4.9.cfg
#SW-DESC_FILE=$(TOPDIR)/target/allwinner/$(TARGET_BOARD)/swupdate/sw-description
SW-DESC-AB_FILE=$(TOPDIR)/target/allwinner/$(TARGET_BOARD)/swupdate/sw-description-ab
SW-DESC-AB-RDIFF_FILE=$(TOPDIR)/target/allwinner/$(TARGET_BOARD)/swupdate/sw-description-ab-rdiff

ifeq ($(SWU_VERSION_FILE),$(wildcard $(SWU_VERSION_FILE)))
	SWU_VERSION_LINE=$(shell sed -n '/swu_version/=' $(SWU_VERSION_FILE))
	SWU_VERSION_RET=$(shell sed -i "$(SWU_VERSION_LINE)c swu_version=$(C-SMART-BRAIN-SWU_VERSION)" $(SWU_VERSION_FILE))
$(info "the return is $(SWU_VERSION_RET)")
endif

#ifeq ($(SW-DESC_FILE),$(wildcard $(SW-DESC_FILE)))
#	SW-DESC_RET=$(shell sed -i -e "3c version = \"${C-SMART-BRAIN-SWU_VERSION}\";" \
#		-e "92c value = \"${C-SMART-BRAIN-SWU_VERSION}\";" $(SW-DESC_FILE);\
#		sed -i -e '3s/version/\tversion/g' -e '92s/value/\t\t\t\t\tvalue/g' $(SW-DESC_FILE))
#$(info "the return is $(SW-DESC_RET)")
#endif
ifeq ($(SW-DESC-AB_FILE),$(wildcard $(SW-DESC-AB_FILE)))
	SW-DESC-AB_RET=$(shell sed -i -e "3c version = \"${C-SMART-BRAIN-SWU_VERSION}\";" \
		-e "49c value = \"${C-SMART-BRAIN-SWU_VERSION}\";" \
		-e "103c value = \"${C-SMART-BRAIN-SWU_VERSION}\";" $(SW-DESC-AB_FILE);\
		sed -i -e '3s/version/\tversion/g' -e '49s/value/\t\t\t\t\tvalue/g' -e '103s/value/\t\t\t\t\tvalue/g' $(SW-DESC-AB_FILE))
$(info "the return is $(SW-DESC-AB_RET)")
endif
ifeq ($(SW-DESC-AB-RDIFF_FILE),$(wildcard $(SW-DESC-AB-RDIFF_FILE)))
	SW-DESC-AB-RDIFF_RET=$(shell sed -i -e "3c version = \"${C-SMART-BRAIN-SWU_VERSION}\";" \
		-e "55c value = \"${C-SMART-BRAIN-SWU_VERSION}\";" \
		-e "115c value = \"${C-SMART-BRAIN-SWU_VERSION}\";" $(SW-DESC-AB-RDIFF_FILE);\
		sed -i -e '3s/version/\tversion/g' -e '55s/value/\t\t\t\t\tvalue/g' -e '115s/value/\t\t\t\t\tvalue/g' $(SW-DESC-AB-RDIFF_FILE))
$(info "the return is $(SW-DESC-AB-RDIFF_RET)")
endif
