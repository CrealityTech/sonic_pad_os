/*
 * (C) Copyright 2013-2016
 * Allwinner Technology Co., Ltd. <www.allwinnertech.com>
 *
 * SPDX-License-Identifier:     GPL-2.0+
 */

#include "asm/arch-sunxi/ce_2.3.h"
#include <common.h>
#include <asm/io.h>
#include <asm/arch/ce.h>
#include <memalign.h>
#include "ss_op.h"

#define ALG_SHA256 (0x13)
#define ALG_SHA512 (0x15)
#define ALG_RSA	   (0x20)
#define ALG_MD5	   (0x10)
#define ALG_TRANG  (0x1C)

/*check if task_queue size is cache_line align*/
#define STATIC_CHECK(condition) extern u8 checkFailAt##__LINE__[-!(condition)];
STATIC_CHECK(sizeof(struct other_task_descriptor) ==
	     ALIGN(sizeof(struct other_task_descriptor), CACHE_LINE_SIZE))
STATIC_CHECK(sizeof(struct hash_task_descriptor) ==
	     ALIGN(sizeof(struct hash_task_descriptor), CACHE_LINE_SIZE))

static void __rsa_padding(u8 *dst_buf, u8 *src_buf, u32 data_len, u32 group_len)
{
	int i = 0;

	memset(dst_buf, 0, group_len);
	for (i = group_len - data_len; i < group_len; i++) {
		dst_buf[i] = src_buf[group_len - 1 - i];
	}
}

void sunxi_ss_open(void)
{
	ss_open();
}

void sunxi_ss_close(void)
{
	ss_close();
}

int sunxi_md5_calc(u8 *dst_addr, u32 dst_len, u8 *src_addr, u32 src_len)
{
	u32 src_align_len					     = 0;
	u32 total_bit_len					     = 0;
	struct hash_task_descriptor task0 __aligned(CACHE_LINE_SIZE) = { 0 };
	/* sha256  2word, sha512 4word*/
	ALLOC_CACHE_ALIGN_BUFFER(u8, p_sign, CACHE_LINE_SIZE);

	memset(p_sign, 0, CACHE_LINE_SIZE);

	/* CE2.0 */
	src_align_len = ALIGN(src_len, 4);

	total_bit_len	     = src_len << 3;

	memset(&task0, 0, sizeof(task0));
	task0.ctrl = (CHANNEL_0 << CHN) | (0x1 << LPKG) | (0x0 << DLAV) |
		     (0x1 << IE);
	task0.cmd = (ALG_MD5);
	memcpy(task0.data_toal_len_addr, &total_bit_len, 4);
	memcpy(task0.sg[0].source_addr, &src_addr, 4);
	task0.sg[0].source_len = src_len;
	memcpy(task0.sg[0].dest_addr, &p_sign, 4);
	task0.sg[0].dest_len = dst_len;

	flush_cache((u32)&task0, sizeof(task0));
	flush_cache((u32)p_sign, CACHE_LINE_SIZE);
	flush_cache((u32)src_addr, src_align_len);

	ss_set_drq((u32)&task0);
	ss_irq_enable(CHANNEL_0);
	ss_ctrl_start(HASH_RBG_TRPE);
	ss_wait_finish(CHANNEL_0);
	ss_pending_clear(CHANNEL_0);
	ss_ctrl_stop();
	ss_irq_disable(CHANNEL_0);
	ss_set_drq(0);
	if (ss_check_err(CHANNEL_0)) {
		printf("SS %s fail 0x%x\n", __func__, ss_check_err(CHANNEL_0));
		return -1;
	}

	invalidate_dcache_range((ulong)p_sign, (ulong)p_sign + CACHE_LINE_SIZE);
	/*copy data*/
	memcpy(dst_addr, p_sign, dst_len);
	return 0;
}

int sunxi_sha_calc(u8 *dst_addr, u32 dst_len, u8 *src_addr, u32 src_len)
{
	u32 total_bit_len					     = 0;
	struct hash_task_descriptor task0 __aligned(CACHE_LINE_SIZE) = { 0 };
	/* sha256  2word, sha512 4word*/
	ALLOC_CACHE_ALIGN_BUFFER(u8, p_sign, CACHE_LINE_SIZE);
	u32 md_size = 32;
	memset(p_sign, 0, CACHE_LINE_SIZE);

	memset((void *)&task0, 0x00, sizeof(task0));
	total_bit_len = src_len * 8;
	task0.ctrl    = (CHANNEL_0 << CHN) | (0x1 << LPKG) | (0x0 << DLAV) |
		     (0x1 << IE);
	task0.cmd = (SUNXI_SHA256 << 0);
	memcpy(task0.data_toal_len_addr, &total_bit_len, 4);
	memcpy(task0.sg[0].source_addr, &src_addr, 4);
	task0.sg[0].source_len = src_len;
	memcpy(task0.sg[0].dest_addr, &p_sign, 4);
	task0.sg[0].dest_len = md_size;

	flush_cache(((u32)&task0), ALIGN(sizeof(task0), CACHE_LINE_SIZE));
	flush_cache((u32)p_sign, CACHE_LINE_SIZE);
	flush_cache(((u32)src_addr), ALIGN(src_len, CACHE_LINE_SIZE));

	ss_set_drq((u32)&task0);
	ss_irq_enable(CHANNEL_0);
	ss_ctrl_start(HASH_RBG_TRPE);
	ss_wait_finish(CHANNEL_0);
	ss_pending_clear(CHANNEL_0);
	ss_ctrl_stop();
	ss_irq_disable(CHANNEL_0);
	ss_set_drq(0);
	if (ss_check_err(CHANNEL_0)) {
		printf("SS %s fail 0x%x\n", __func__, ss_check_err(CHANNEL_0));
		return -1;
	}

	invalidate_dcache_range((ulong)p_sign,
				((ulong)p_sign) + CACHE_LINE_SIZE);
	/*copy data*/
	memcpy(dst_addr, p_sign, md_size);
	return 0;
}

s32 sunxi_normal_rsa(u8 *n_addr, u32 n_len, u8 *e_addr, u32 e_len, u8 *dst_addr,
		     u32 dst_len, u8 *src_addr, u32 src_len)
{
	const u32 TEMP_BUFF_LEN = ((2048 >> 3) + CACHE_LINE_SIZE);

	u32 mod_bit_size	 = 2048;
	u32 mod_size_len_inbytes = mod_bit_size / 8;
	struct other_task_descriptor task0 __aligned(CACHE_LINE_SIZE) = { 0 };
	ALLOC_CACHE_ALIGN_BUFFER(u8, p_n, TEMP_BUFF_LEN);
	ALLOC_CACHE_ALIGN_BUFFER(u8, p_e, TEMP_BUFF_LEN);
	ALLOC_CACHE_ALIGN_BUFFER(u8, p_src, TEMP_BUFF_LEN);
	ALLOC_CACHE_ALIGN_BUFFER(u8, p_dst, TEMP_BUFF_LEN);

	memset(p_src, 0, mod_size_len_inbytes);
	memcpy(p_src, src_addr, src_len);
	memset(p_n, 0, mod_size_len_inbytes);
	memcpy(p_n, n_addr, n_len);
	memset(p_e, 0, mod_size_len_inbytes);
	memcpy(p_e, e_addr, e_len);

	memset((void *)&task0, 0x00, sizeof(task0));
	task0.task_id	     = CHANNEL_0;
	task0.common_ctl     = (ALG_RSA | (1U << 31));
	task0.asymmetric_ctl = (mod_bit_size >> 5);
	memcpy(task0.sg[0].source_addr, &p_e, 4);
	task0.sg[0].source_len = mod_size_len_inbytes;
	memcpy(task0.sg[1].source_addr, &p_n, 4);
	task0.sg[1].source_len = mod_size_len_inbytes;
	memcpy(task0.sg[2].source_addr, &p_src, 4);
	task0.sg[2].source_len = mod_size_len_inbytes;
	task0.data_len += task0.sg[0].source_len;
	task0.data_len += task0.sg[1].source_len;
	task0.data_len += task0.sg[2].source_len;

	memcpy(task0.sg[0].dest_addr, &p_dst, 4);
	task0.sg[0].dest_len = mod_size_len_inbytes;

	flush_cache((u32)&task0, sizeof(task0));
	flush_cache((u32)p_n, mod_size_len_inbytes);
	flush_cache((u32)p_e, mod_size_len_inbytes);
	flush_cache((u32)p_src, mod_size_len_inbytes);
	flush_cache((u32)p_dst, mod_size_len_inbytes);

	ss_set_drq((u32)&task0);
	ss_irq_enable(task0.task_id);
	ss_ctrl_start(ASYM_TRPE);
	ss_wait_finish(task0.task_id);
	ss_pending_clear(task0.task_id);
	ss_ctrl_stop();
	ss_irq_disable(task0.task_id);
	if (ss_check_err(task0.task_id)) {
		printf("SS %s fail 0x%x\n", __func__,
		       ss_check_err(task0.task_id));
		return -1;
	}

	invalidate_dcache_range((ulong)p_dst,
				(ulong)p_dst + mod_size_len_inbytes);
	memcpy(dst_addr, p_dst, mod_size_len_inbytes);

	return 0;
}

s32 sunxi_rsa_calc(u8 *n_addr, u32 n_len, u8 *e_addr, u32 e_len, u8 *dst_addr,
		   u32 dst_len, u8 *src_addr, u32 src_len)
{
	const u32 TEMP_BUFF_LEN = ((2048 >> 3) + CACHE_LINE_SIZE);

	u32 mod_bit_size	 = 2048;
	u32 mod_size_len_inbytes = mod_bit_size / 8;
	struct other_task_descriptor task0 __aligned(CACHE_LINE_SIZE) = { 0 };
	ALLOC_CACHE_ALIGN_BUFFER(u8, p_n, TEMP_BUFF_LEN);
	ALLOC_CACHE_ALIGN_BUFFER(u8, p_e, TEMP_BUFF_LEN);
	ALLOC_CACHE_ALIGN_BUFFER(u8, p_src, TEMP_BUFF_LEN);
	ALLOC_CACHE_ALIGN_BUFFER(u8, p_dst, TEMP_BUFF_LEN);

	__rsa_padding(p_src, src_addr, src_len, mod_size_len_inbytes);
	__rsa_padding(p_n, n_addr, n_len, mod_size_len_inbytes);
	/*
	 * e usually 0x010001, so rsa padding, aka little-end ->
	 * big-end transfer is useless like, still do this rsa
	 * padding here, in case e is no longer 0x010001 some day
	 */
	memset(p_e, 0, mod_size_len_inbytes);
	__rsa_padding(p_e, e_addr, e_len, e_len);
	//memcpy(p_e, e_addr, e_len);

	memset((void *)&task0, 0x00, sizeof(task0));
	task0.task_id	     = CHANNEL_0;
	task0.common_ctl     = (ALG_RSA | (1U << 31));
	task0.asymmetric_ctl = (mod_bit_size >> 5);
	memcpy(task0.sg[0].source_addr, &p_e, 4);
	task0.sg[0].source_len = mod_size_len_inbytes;
	memcpy(task0.sg[1].source_addr, &p_n, 4);
	task0.sg[1].source_len = mod_size_len_inbytes;
	memcpy(task0.sg[2].source_addr, &p_src, 4);
	task0.sg[2].source_len = mod_size_len_inbytes;
	task0.data_len += task0.sg[0].source_len;
	task0.data_len += task0.sg[1].source_len;
	task0.data_len += task0.sg[2].source_len;

	memcpy(task0.sg[0].dest_addr, &p_dst, 4);
	task0.sg[0].dest_len = mod_size_len_inbytes;

	flush_cache((u32)&task0, sizeof(task0));
	flush_cache((u32)p_n, mod_size_len_inbytes);
	flush_cache((u32)p_e, mod_size_len_inbytes);
	flush_cache((u32)p_src, mod_size_len_inbytes);
	flush_cache((u32)p_dst, mod_size_len_inbytes);

	ss_set_drq((u32)&task0);
	ss_irq_enable(task0.task_id);
	ss_ctrl_start(ASYM_TRPE);
	ss_wait_finish(task0.task_id);
	ss_pending_clear(task0.task_id);
	ss_ctrl_stop();
	ss_irq_disable(task0.task_id);
	if (ss_check_err(task0.task_id)) {
		printf("SS %s fail 0x%x\n", __func__,
		       ss_check_err(task0.task_id));
		return -1;
	}

	invalidate_dcache_range((ulong)p_dst,
				(ulong)p_dst + mod_size_len_inbytes);
	__rsa_padding(dst_addr, p_dst, mod_bit_size / 64, mod_bit_size / 64);

	return 0;
}

int sunxi_aes_with_hardware(uint8_t *dst_addr, uint8_t *src_addr, int len,
			    uint8_t *key, uint32_t key_len,
			    uint32_t symmetric_ctl, uint8_t dir)
{
	uint8_t __aligned(64) iv[16]			 = { 0 };
	struct other_task_descriptor task0 __aligned(64) = { 0 };
	uint32_t cts_size, destination_len;
	ALLOC_CACHE_ALIGN_BUFFER(uint8_t, key_map, 64);

	memset(key_map, 0, 64);
	if (key) {
		memcpy(key_map, key, key_len);
	}
	memset((void *)&task0, 0x00, sizeof(task0));

	cts_size	= ALIGN(len, 4);
	destination_len = cts_size;

	task0.task_id	    = 0;
	task0.common_ctl    = (1U << 31) | (dir << 8) | _SUNXI_AES_CFG;
	task0.symmetric_ctl = symmetric_ctl;
	memcpy(task0.key_addr, &key_map, 4);
	memcpy(task0.iv_addr, &iv, 4);
	task0.data_len = len;
	memcpy(task0.sg[0].source_addr, &src_addr, 4);
	task0.sg[0].source_len = cts_size;
	memcpy(task0.sg[0].dest_addr, &dst_addr, 4);
	task0.sg[0].dest_len = destination_len;

	//flush&clean cache
	flush_cache((u32)iv, ALIGN(sizeof(iv), CACHE_LINE_SIZE));
	flush_cache((u32)&task0, sizeof(task0));
	flush_cache((u32)key_map, ALIGN(sizeof(key_map), CACHE_LINE_SIZE));
	flush_cache((u32)src_addr, ALIGN(len, CACHE_LINE_SIZE));
	flush_cache((u32)dst_addr, ALIGN(len, CACHE_LINE_SIZE));

	ss_set_drq(((u32)&task0));
	ss_irq_enable(CHANNEL_0);
	ss_ctrl_start(SYMM_TRPE);
	ss_wait_finish(CHANNEL_0);
	ss_pending_clear(CHANNEL_0);
	ss_ctrl_stop();
	ss_irq_disable(CHANNEL_0);
	ss_set_drq(0);
	if (ss_check_err(CHANNEL_0)) {
		printf("SS %s fail 0x%x\n", __func__, ss_check_err(CHANNEL_0));
		return -1;
	}

	invalidate_dcache_range((ulong)dst_addr, ((ulong)dst_addr) + len);

	return 0;
}

int sunxi_trng_gen(u8 *rng_buf, u32 rng_byte)
{
	struct hash_task_descriptor task0 __aligned(CACHE_LINE_SIZE) = { 0 };
	/* sha256  2word, sha512 4word*/
	ALLOC_CACHE_ALIGN_BUFFER(u8, p_sign, CACHE_LINE_SIZE);
#define RNG_LEN 32
	memset(p_sign, 0, CACHE_LINE_SIZE);

	memset(&task0, 0, sizeof(task0));
	task0.ctrl = (CHANNEL_0 << CHN) | (0x1 << LPKG) | (0x0 << DLAV) |
		     (0x1 << IE);
	task0.cmd = (SUNXI_TRNG<<8)|(SUNXI_SHA256 << 0);
	memcpy(task0.sg[0].dest_addr, &p_sign, 4);
	task0.sg[0].dest_len = RNG_LEN;

	flush_cache(((u32)&task0), ALIGN(sizeof(task0), CACHE_LINE_SIZE));
	flush_cache((u32)p_sign, CACHE_LINE_SIZE);

	ss_set_drq((u32)&task0);
	ss_irq_enable(CHANNEL_0);
	ss_ctrl_start(HASH_RBG_TRPE);
	ss_wait_finish(CHANNEL_0);
	ss_pending_clear(CHANNEL_0);
	ss_ctrl_stop();
	ss_irq_disable(CHANNEL_0);
	ss_set_drq(0);
	if (ss_check_err(CHANNEL_0)) {
		printf("SS %s fail 0x%x\n", __func__, ss_check_err(CHANNEL_0));
		return -1;
	}

	invalidate_dcache_range((ulong)p_sign,
				((ulong)p_sign) + CACHE_LINE_SIZE);
	/*copy data*/
	memcpy(rng_buf, p_sign, RNG_LEN);
	return 0;
}

int sunxi_create_rssk(u8 *rssk_buf, u32 rssk_byte)
{
	return sunxi_trng_gen(rssk_buf, rssk_byte);
}
