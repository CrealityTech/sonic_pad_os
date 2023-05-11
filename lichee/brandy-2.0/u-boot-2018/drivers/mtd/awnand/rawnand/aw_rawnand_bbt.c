/**
 * SPDX-License-Identifier: GPL-2.0+
 * aw_rawnand_bbt.c
 *
 * (C) Copyright 2020 - 2021
 * Allwinner Technology Co., Ltd. <www.allwinnertech.com>
 * cuizhikui <cuizhikui@allwinnertech.com>
 *
 */

#include <common.h>
#include <linux/mtd/mtd.h>
#include <linux/mtd/aw-rawnand.h>


/**
 * aw_rawnand_chip_check_badblock_bbt - check bad block from bbt
 * @mtd: MTD device structure
 * @block: block offset from device start block
 * @return: %BBT_B_BAD the block is bad, %BBT_B_GOOD the block is good,
 *		%BBT_B_INVALID the block in bbt can't to know bad or good
 * */
static int aw_rawnand_chip_check_badblock_bbt(struct mtd_info *mtd, int block)
{
	struct aw_nand_chip *chip = awnand_mtd_to_chip(mtd);
	int ret = 0;

	int pos_byte = 0;
	int pos_bit = 0;

	pos_byte = block >> 3;
	pos_bit = block & 0x7;


	if (chip->bbtd[pos_byte] & (1 << pos_bit)) {
		if (chip->bbt[pos_byte] & (1 << pos_bit))
			ret = BBT_B_BAD;
		else
			ret = BBT_B_GOOD;
	} else
		ret = BBT_B_INVALID;

	return ret;
}


/**
 * aw_rawnand_chip_updata_bbt - update bbt&bbtd
 * @mtd : MTD device structure
 * @block: block offset device start
 * @flag: %BBT_B_GOOD mark the block is good in bbt,
 *	%BBT_B_BAD mark the block is bad in bbt
 * */
void aw_rawnand_chip_update_bbt(struct mtd_info *mtd, int block, int flag)
{
	struct aw_nand_chip *chip = awnand_mtd_to_chip(mtd);
	int pos_byte = 0;
	int pos_bit = 0;

	if (unlikely(!chip->bbt || !chip->bbtd)) {
		awrawnand_warn("bbt or bbtd is null, nothing to do\n");
		return;
	}

	pos_byte = block >> 3;
	pos_bit = block & 0x7;

	chip->bbtd[pos_byte] |= (1 << pos_bit);
	if (flag == BBT_B_GOOD)
		chip->bbt[pos_byte] &= ~(1 << pos_bit);
	else
		chip->bbt[pos_byte] |= (1 << pos_bit);

	return;
}
/**
 * aw_rawnand_chip_block_bad - read bad block marker from the chip
 * @mtd: MTD device structure
 * @block: block offset from device start block
 * */
int aw_rawnand_chip_block_bad(struct mtd_info *mtd, int block)
{
	struct aw_nand_chip *chip = awnand_mtd_to_chip(mtd);

	int ret = 0;
	int page = 0;
	uint8_t spare[chip->avalid_sparesize];
	int snd_flag = 0;

	ret = aw_rawnand_chip_check_badblock_bbt(mtd, block);
	if (ret == BBT_B_GOOD) {
		printf("block@%d bbt good\n", block);
		return 0;
	} else if (ret == BBT_B_BAD) {
		printf("block@%d bbt bad\n", block);
		return 1;
	} else {
		awrawnand_dbg(
		"block @%d in bbt is invalid , need to read block mark flag\n", block);
	}


	if (chip->badblock_mark_pos & FIRST_PAGE)
		page = block << chip->pages_per_blk_shift;
	else if (chip->badblock_mark_pos & LAST_PAGE) {
		page = (block << chip->pages_per_blk_shift) +
			chip->pages_per_blk_mask;
	} else {
		awrawnand_err("Unknow the block mark use default mark pos(first page)\n");
	}

	ret = chip->read_page_spare(mtd, chip, spare, chip->avalid_sparesize, page);
	if (ret != ECC_ERR) {
		if (spare[0] != 0xFF) {
			ret = 1;
			awrawnand_info("page@%d oob:%02x %02x %02x %02x %02x %02x %02x %02x\n", page, spare[0],
					spare[1], spare[2], spare[3], spare[4], spare[5], spare[6], spare[7]);
			aw_rawnand_chip_update_bbt(mtd, block, BBT_B_BAD);
			goto out;
		}
	}

	if ((chip->badblock_mark_pos == FIRST_TWO_PAGES)) {
		page++;
		snd_flag = 1;
	} else if ((chip->badblock_mark_pos == LAST_TWO_PAGES)) {
		page--;
		snd_flag = 1;
	}

	if (snd_flag) {
		ret = chip->read_page_spare(mtd, chip, spare, chip->avalid_sparesize, page);
		if (ret != ECC_ERR) {
			if (spare[0] != 0xFF) {
				ret = 1;
				printf("page@%d:%02x %02x %02x %02x %02x %02x %02x %02x %02x %02x\n", page, spare[0],
			spare[1], spare[2], spare[3], spare[4], spare[5], spare[6], spare[7], spare[8], spare[9]);
				aw_rawnand_chip_update_bbt(mtd, block, BBT_B_BAD);
			}
		}
	}

out:
	return ret;
}

/**
 * aw_rawnand_chip_simu_block_bad - read bad block marker from the chip
 * @mtd: MTD device structure
 * @block: simu block offset from device start simu block
 * */
int aw_rawnand_chip_simu_block_bad(struct mtd_info *mtd, int block)
{
	struct aw_nand_chip *chip = awnand_mtd_to_chip(mtd);

	int ret = 0;
	int page = 0;
	uint8_t spare[chip->avalid_sparesize];
	int snd_flag = 0;


	ret = aw_rawnand_chip_check_badblock_bbt(mtd, block);
	if (ret == BBT_B_GOOD) {
		printf("block@%d bbt good\n", block);
		return 0;
	} else if (ret == BBT_B_BAD) {
		printf("block@%d bbt bad\n", block);
		return 1;
	} else {
		awrawnand_dbg(
		"block @%d in bbt is invalid , need to read block mark flag\n", block);
	}

	int real_blkA = (block << 1);
	int real_blkB = real_blkA + 1;
	int real_blk = real_blkA;

	int plane = 0;
	/*check planeA & planeB*/
	for (plane = 0; plane < 2; plane++) {
		if (chip->badblock_mark_pos & FIRST_PAGE)
			page = real_blk << chip->simu_pages_per_blk_shift;
		else if (chip->badblock_mark_pos & LAST_PAGE) {
			page = (real_blk << chip->simu_pages_per_blk_shift) +
				chip->simu_pages_per_blk_mask;
		} else {
			awrawnand_err("Unknow the block mark use default mark pos(first page)\n");
		}

		ret = chip->read_page_spare(mtd, chip, spare, chip->avalid_sparesize, page);
		if (ret != ECC_ERR) {
			if (spare[0] != 0xFF) {
				ret = 1;
				awrawnand_info("page@%d oob:%02x %02x %02x %02x %02x %02x %02x %02x\n", page, spare[0],
						spare[1], spare[2], spare[3], spare[4], spare[5], spare[6], spare[7]);
				aw_rawnand_chip_update_bbt(mtd, block, BBT_B_BAD);
				break;
			}
		}

		if ((chip->badblock_mark_pos == FIRST_TWO_PAGES)) {
			page++;
			snd_flag = 1;
		} else if ((chip->badblock_mark_pos == LAST_TWO_PAGES)) {
			page--;
			snd_flag = 1;
		}

		if (snd_flag) {
			ret = chip->read_page_spare(mtd, chip, spare, chip->avalid_sparesize, page);
			if (ret != ECC_ERR) {
				if (spare[0] != 0xFF) {
					ret = 1;
					printf("page@%d:%02x %02x %02x %02x %02x %02x %02x %02x %02x %02x\n", page, spare[0],
							spare[1], spare[2], spare[3], spare[4], spare[5], spare[6], spare[7], spare[8], spare[9]);
					aw_rawnand_chip_update_bbt(mtd, block, BBT_B_BAD);
					break;
				}
			}
		}
		real_blk = real_blkB;
	}

	return ret;
}

/**
 * aw_rawnand_chip_block_markbad - mark a block bad in mark pos and update bbt&bbtd
 * @mtd: MTD device structure
 * @block: block offset from device start
 * */
int aw_rawnand_chip_block_markbad(struct mtd_info *mtd, int block)
{
	struct aw_nand_chip *chip = awnand_mtd_to_chip(mtd);
	uint8_t *mdata = NULL;
	uint8_t *spare = NULL;
	int ret = 0;
	int page = 0;
	int bound = (chip->chips * chip->chipsize) >> chip->erase_shift;
	int pagesize = chip->pagesize;
	int sparesize = chip->avalid_sparesize;

	if (unlikely(block > bound)) {
		ret = -EINVAL;
		awrawnand_err("block@%d is exceed boundary@%d\n", block, bound);
		goto out;
	}

	page = block << chip->pages_per_blk_shift;

	mdata = kzalloc(pagesize,  GFP_KERNEL);
	if (!mdata) {
		awrawnand_err("kzalloc mdata fail\n");
		ret = -ENOMEM;
		goto out;
	}

	spare = kzalloc(sparesize, GFP_KERNEL);
	if (!spare) {
		awrawnand_err("kzalloc spare fail\n");
		ret = -ENOMEM;
		goto out;
	}

	ret = chip->erase(mtd, page);
	if (ret) {
		awrawnand_err("erase block@%d fail\n", block);
		ret = -EIO;
		goto out;
	}

	if (chip->badblock_mark_pos == FIRST_PAGE)
		ret = chip->write_page(mtd, chip, mdata, pagesize, spare, sparesize, page);
	else if (chip->badblock_mark_pos == LAST_PAGE) {
		page += chip->pages_per_blk_mask;
		ret = chip->write_page(mtd, chip, mdata, pagesize, spare, sparesize, page);
	} else if (chip->badblock_mark_pos == FIRST_TWO_PAGES) {
		ret = chip->write_page(mtd, chip, mdata, pagesize, spare, sparesize, page);
		page++;
		ret |= chip->write_page(mtd, chip, mdata, pagesize, spare, sparesize, page);
	} else if (chip->badblock_mark_pos == LAST_TWO_PAGES) {
		page += chip->pages_per_blk_mask;
		ret = chip->write_page(mtd, chip, mdata, pagesize, spare, sparesize, page);
		page--;
		ret |= chip->write_page(mtd, chip, mdata, pagesize, spare, sparesize, page);
	} else{
		awrawnand_err("Unknow the block mark use default mark pos(first page)\n");
		ret = chip->write_page(mtd, chip, mdata, pagesize, spare, sparesize, page);
	}

	aw_rawnand_chip_update_bbt(mtd, block, BBT_B_BAD);

out:
	return ret;
}



/**
 * aw_rawnand_chip_simu_block_markbad - mark a simu block bad in mark pos and update bbt&bbtd
 * @mtd: MTD device structure
 * @block: simu block offset from device start simu block
 * */
int aw_rawnand_chip_simu_block_markbad(struct mtd_info *mtd, int block)
{
	struct aw_nand_chip *chip = awnand_mtd_to_chip(mtd);
	uint8_t *mdata = NULL;
	uint8_t *spare = NULL;
	int ret = 0;
	int page = 0;
	int bound = (chip->chips * chip->chipsize) >> chip->simu_erase_shift;
	int pagesize = chip->pagesize;
	int sparesize = chip->avalid_sparesize;

	if (unlikely(block > bound)) {
		ret = -EINVAL;
		awrawnand_err("block@%d is exceed boundary@%d\n", block, bound);
		goto out;
	}

	int real_blkA = (block << 1);
	int real_blkB = real_blkA + 1;
	int real_blk = real_blkA;

	int plane = 0;
	/*mark planeA & planeB*/

	mdata = kzalloc(pagesize,  GFP_KERNEL);
	if (!mdata) {
		awrawnand_err("kzalloc mdata fail\n");
		ret = -ENOMEM;
		goto out;
	}

	spare = kzalloc(sparesize, GFP_KERNEL);
	if (!spare) {
		awrawnand_err("kzalloc spare fail\n");
		ret = -ENOMEM;
		goto out;
	}

	for (plane = 0; plane < 2; plane++) {
		page = real_blk << chip->simu_pages_per_blk_shift;

		ret = chip->erase(mtd, page);
		if (ret) {
			awrawnand_err("erase block@%d fail\n", block);
			ret = -EIO;
			goto out;
		}

		if (chip->badblock_mark_pos == FIRST_PAGE)
			ret = chip->write_page(mtd, chip, mdata, pagesize, spare, sparesize, page);
		else if (chip->badblock_mark_pos == LAST_PAGE) {
			page += chip->pages_per_blk_mask;
			ret = chip->write_page(mtd, chip, mdata, pagesize, spare, sparesize, page);
		} else if (chip->badblock_mark_pos == FIRST_TWO_PAGES) {
			ret = chip->write_page(mtd, chip, mdata, pagesize, spare, sparesize, page);
			page++;
			ret |= chip->write_page(mtd, chip, mdata, pagesize, spare, sparesize, page);
		} else if (chip->badblock_mark_pos == LAST_TWO_PAGES) {
			page += chip->pages_per_blk_mask;
			ret = chip->write_page(mtd, chip, mdata, pagesize, spare, sparesize, page);
			page--;
			ret |= chip->write_page(mtd, chip, mdata, pagesize, spare, sparesize, page);
		} else{
			awrawnand_err("Unknow the block mark use default mark pos(first page)\n");
			ret = chip->write_page(mtd, chip, mdata, pagesize, spare, sparesize, page);
		}
		real_blk = real_blkB;

	}

	aw_rawnand_chip_update_bbt(mtd, block, BBT_B_BAD);

out:
	return ret;
}

int aw_rawnand_chip_scan_bbt(struct mtd_info *mtd)
{
	struct aw_nand_chip *chip = awnand_mtd_to_chip(mtd);

#if SIMULATE_MULTIPLANE
	int total_blocks = chip->chipsize >> chip->erase_shift;
#else
	int total_blocks = chip->simu_chipsize >> chip->simu_erase_shift;
#endif

	int b = 0;
	int c = 0;
	int pos_byte = 0;
	int pos_bit = 0;
	uint32_t time_start = get_timer(0);

	for (c = 0; c < chip->chips; c++) {
		chip->select_chip(mtd, c);
		for (b = 0; b < total_blocks; b++) {
			pos_byte = b >> 3;
			pos_bit = b & 0x7;

#if SIMULATE_MULTIPLANE
			if (chip->block_bad(mtd, b)) {
				chip->block_markbad(mtd, b);
#else
			if (chip->simu_block_bad(mtd, b)) {
				chip->simu_block_markbad(mtd, b);
#endif
			} else {
				chip->bbt[pos_byte] &= ~(1 << pos_bit);
			}
			chip->bbtd[pos_byte] |= (1 << pos_bit);
		}
		chip->select_chip(mtd, -1);
	}
	printf("c@%d b@%d times@%lu\n", c, b, get_timer(time_start));

	return 0;
}
