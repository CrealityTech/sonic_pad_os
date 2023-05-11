// SPDX-License-Identifier: GPL-2.0+
/*
 *  drivers/dsp/dsp.c
 *
 * Copyright (c) 2020 Allwinner.
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program;
 */

#include <asm/arch-sunxi/cpu_ncat_v2.h>
#include <asm/io.h>
#include <common.h>
#include "elf.h"
#include "fdt_support.h"
#include <sys_config.h>

#include "dsp_i.h"

#define ROUND_DOWN(a, b) ((a) & ~((b)-1))
#define ROUND_UP(a,   b) (((a) + (b)-1) & ~((b)-1))

#define ROUND_DOWN_CACHE(a) ROUND_DOWN(a, CONFIG_SYS_CACHELINE_SIZE)
#define ROUND_UP_CACHE(a)   ROUND_UP(a, CONFIG_SYS_CACHELINE_SIZE)

struct vaddr_range {
	unsigned long vstart;
	unsigned long vend;
	unsigned long offset;
};

/*
 * dsp need to remap addresses for some addr.
 */
static struct vaddr_range addr_mapping[] = {
	{ 0x10000000, 0x1fffffff, 0x30000000 },
	{ 0x30000000, 0x3fffffff, 0x10000000 },
};

static unsigned long __va_to_pa(unsigned long vaddr)
{
	unsigned long paddr = vaddr;
	int i;
	int size = sizeof(addr_mapping) / sizeof(struct vaddr_range);

	for (i = 0; i < size; i++) {
		if (vaddr >= addr_mapping[i].vstart
				&& vaddr <= addr_mapping[i].vend) {
			paddr += addr_mapping[i].offset;
			break;
		}
	}

	return paddr;
}


static void sunxi_dsp_set_runstall(u32 dsp_id, u32 value)
{
	u32 reg_val;

	if (dsp_id == 0) { /* DSP0 */
		reg_val = readl(DSP0_CFG_BASE + DSP_CTRL_REG0);
		reg_val &= ~(1 << BIT_RUN_STALL);
		reg_val |= (value << BIT_RUN_STALL);
		writel(reg_val, DSP0_CFG_BASE + DSP_CTRL_REG0);
	}
}

static int update_reset_vec(u32 img_addr, u32 *run_addr)
{
	Elf32_Ehdr *ehdr; /* Elf header structure pointer */

	ehdr = (Elf32_Ehdr *)img_addr;

	if (!*run_addr)
		*run_addr = ehdr->e_entry;

	return 0;
}


static int load_image(u32 img_addr)
{
	Elf32_Ehdr *ehdr; /* Elf header structure pointer */
	Elf32_Phdr *phdr; /* Program header structure pointer */
	int i;
	void *dst = NULL;
	void *src = NULL;
	ehdr = (Elf32_Ehdr *)img_addr;
	phdr = (Elf32_Phdr *)(img_addr + ehdr->e_phoff);

	/* Load each program header */
	for (i = 0; i < ehdr->e_phnum; ++i) {

		//remap addresses
		dst = (void *)__va_to_pa((unsigned long)phdr->p_paddr);
		src = (void *)img_addr + phdr->p_offset;
		debug("Loading phdr %i to 0x%p (%i bytes)\n",
		      i, dst, phdr->p_filesz);
		if (phdr->p_filesz)
			memcpy(dst, src, phdr->p_filesz);
		if (phdr->p_filesz != phdr->p_memsz)
			memset(dst + phdr->p_filesz, 0x00,
			       phdr->p_memsz - phdr->p_filesz);

		flush_cache(ROUND_DOWN_CACHE((unsigned long)dst),
			    ROUND_UP_CACHE(phdr->p_filesz));
		++phdr;
	}

	return 0;
}

static void dsp_freq_default_set(void)
{
	u32 reg = SUNXI_CCM_BASE + CCMU_DSP_CLK_REG;
	u32 val = 0;

	val = DSP_CLK_SRC_PERI2X | DSP_CLK_FACTOR_M(2)
		| (1 << BIT_DSP_SCLK_GATING);
	writel(val, reg);
}

static void sram_remap_set(void)
{
	u32 val = 0;

	val = readl(SUNXI_SRAMC_BASE + SRAMC_SRAM_REMAP_REG);
	val &= ~(1 << BIT_SRAM_REMAP_ENABLE);
	writel(val, SUNXI_SRAMC_BASE + SRAMC_SRAM_REMAP_REG);
}

int sunxi_dsp_init(u32 img_addr, u32 run_ddr, u32 dsp_id)
{
	u32 reg_val;

	/* update run addr */
	update_reset_vec(img_addr, &run_ddr);

	dsp_freq_default_set();

	sram_remap_set();

	printf("DSP%d booting from 0x%x...\n", dsp_id, run_ddr);

	if (dsp_id == 0) { /* DSP0 */
		/* clock gating */
		reg_val = readl(SUNXI_CCM_BASE + CCMU_DSP_BGR_REG);
		reg_val |= (1 << BIT_DSP0_CFG_GATING);
		writel(reg_val, SUNXI_CCM_BASE + CCMU_DSP_BGR_REG);

		/* reset */
		reg_val = readl(SUNXI_CCM_BASE + CCMU_DSP_BGR_REG);
		reg_val |= (1 << BIT_DSP0_CFG_RST);
		reg_val |= (1 << BIT_DSP0_DBG_RST);
		writel(reg_val, SUNXI_CCM_BASE + CCMU_DSP_BGR_REG);

		/* set external Reset Vector if needed */
		if (run_ddr != DSP_DEFAULT_RST_VEC) {
			writel(run_ddr, DSP0_CFG_BASE + DSP_ALT_RESET_VEC_REG);

			reg_val = readl(DSP0_CFG_BASE + DSP_CTRL_REG0);
			reg_val |= (1 << BIT_START_VEC_SEL);
			writel(reg_val, DSP0_CFG_BASE + DSP_CTRL_REG0);
		}

		/* set runstall */
		sunxi_dsp_set_runstall(dsp_id, 1);

		/* set dsp clken */
		reg_val = readl(DSP0_CFG_BASE + DSP_CTRL_REG0);
		reg_val |= (1 << BIT_DSP_CLKEN);
		writel(reg_val, DSP0_CFG_BASE + DSP_CTRL_REG0);

		/* de-assert dsp0 */
		reg_val = readl(SUNXI_CCM_BASE + CCMU_DSP_BGR_REG);
		reg_val |= (1 << BIT_DSP0_RST);
		writel(reg_val, SUNXI_CCM_BASE + CCMU_DSP_BGR_REG);

		/* load image to ram */
		load_image(img_addr);

		/* clear runstall */
		sunxi_dsp_set_runstall(dsp_id, 0);
	}

	return 0;
}
