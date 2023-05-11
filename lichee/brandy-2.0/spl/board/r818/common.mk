
#
#config file for sun50iw10
#
SUPPORT_BOARD=a100 a133 t509 a53 mr813 r818 b810
ARCH = arm
PLATFORM = sun50iw10p1

CFG_BOOT0_RUN_ADDR=0x20000
CFG_SYS_INIT_RAM_SIZE=0x10000
CFG_FES1_RUN_ADDR=0x28000
CFG_SBOOT_RUN_ADDR=0x20480

#power
CFG_SUNXI_POWER=y
CFG_SUNXI_TWI=y
CFG_SUNXI_PMIC=y
CFG_AXP81X_POWER=y
CFG_AXP858_POWER=y
CFG_AXP806_POWER=y
CFG_AXP2202_POWER=y

#chipid
CFG_SUNXI_CHIPID=y

#key
CFG_SUNXI_PHY_KEY=y
CFG_LRADC_KEY=y
CFG_GPADC_KEY=y

#sunxi nsi
CFG_SUNXI_NSI=y

#sunxi selet dram para
CFG_SUNXI_SELECT_DRAM_PARA=y

#sunxi smc
CFG_SUNXI_SMC_30=y