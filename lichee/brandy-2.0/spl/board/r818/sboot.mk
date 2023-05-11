
#
#config file for sun50iw10
#
#stroage
FILE_EXIST=$(shell if [ -f $(TOPDIR)/board/$(PLATFORM)/common.mk ]; then echo yes; else echo no; fi;)
ifeq (x$(FILE_EXIST),xyes)
include $(TOPDIR)/board/$(PLATFORM)/common.mk
else
include $(TOPDIR)/board/$(CP_BOARD)/common.mk
endif

MODULE=sboot

CFG_SUNXI_MMC =y
CFG_SUNXI_NAND =y
CFG_SUNXI_CE_21 =y
CFG_SUNXI_EFUSE =y
CFG_SUNXI_SBOOT =y
CFG_SUNXI_ITEM_HASH =y
CFG_SUNXI_KEY_PROVISION =y
