/**
 * SPDX-License-Identifier: GPL-2.0+
 * aw_rawnand_ids.c
 *
 * (C) Copyright 2020 - 2021
 * Allwinner Technology Co., Ltd. <www.allwinnertech.com>
 * cuizhikui <cuizhikui@allwinnertech.com>
 *
 */
#include <common.h>
#include <linux/mtd/aw-rawnand.h>
#include <linux/sizes.h>




struct aw_nand_flash_dev giga[] = {
	{
		.name = "GD9FU2G8F2A",
		.id = {0xc8, 0xda, 0x90, 0x95, 0x46},
		.id_len = 5,
		.dies_per_chip = 1,
		.pagesize = SZ_2K,
		.sparesize = 128,
		.pages_per_blk = 64,
		.blks_per_die = 2048,
		.access_freq = 40,
		.badblock_flag_pos = FIRST_PAGE,
		.pe_cycles = PE_CYCLES_100K,
		.options = RAWNAND_ITF_SDR | RAWNAND_NFC_RANDOM | RAWNAND_MULTI_WRITE | RAWNAND_MULTI_ERASE,
	},
};

struct rawnand_manufacture aw_manuf_tbl[] = {
	RAWNAND_MANUFACTURE(RAWNAND_MFR_GIGA, GIGA_NAME, giga),
};

AW_NAND_MANUFACTURE(aw_nand_manufs, aw_manuf_tbl);
