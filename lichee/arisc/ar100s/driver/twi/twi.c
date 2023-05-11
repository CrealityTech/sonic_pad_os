/*
 * drivers/twi/twi.c
 *
 * Copyright (C) 2014-2016 AllWinnertech Ltd.
 * Author: Xiafeng <Xiafeng@allwinnertech.com>
 *
 */

#include "twi_i.h"

/* #define TWI_DEBUG */

extern struct arisc_para arisc_para;
volatile bool twi_lock = FALSE;

#ifdef TWI_DEBUG
static void printreg(void)
{
	int i;

	for (i = 0; i < 9; i++)
		LOG("reg[%d]:%x\n", i, readl(TWI_SAR + (i << 2)));
}
#endif

/* clear the interrupt flag */
static inline void twi_clear_irq_flag(void)
{
	unsigned int reg_val;

	reg_val = readl(TWI_CTR);
	reg_val |= TWI_INTFLAG;
	/* start and stop bit should be 0 */
	/*reg_val &= ~(TWI_MSTART | TWI_MSTOP); */
	writel(reg_val, TWI_CTR);

	/* make sure that interrupt flag does really be cleared */
	readl(TWI_CTR);		/* read twice for delay */
	while (readl(TWI_CTR) & TWI_INTFLAG)
		;
}

static inline void twi_enable_ack(void)
{
	unsigned int reg_val;

	reg_val = readl(TWI_CTR);
	reg_val |= TWI_AACK;
	reg_val &= ~TWI_INTFLAG;
	writel(reg_val, TWI_CTR);
	readl(TWI_CTR);
	readl(TWI_CTR);
}

static inline void twi_disable_ack(void)
{
	unsigned int reg_val;

	reg_val = readl(TWI_CTR) & 0x0ff;
	reg_val &= ~TWI_AACK;
	reg_val &= ~TWI_INTFLAG;
	writel(reg_val, TWI_CTR);
}

static void twic_send_dummy_clock(void)
{
	unsigned int i = 10, timeout;

	INF("restore bus status to 0x3a\n");
	while (i--) {
		writel(0xa, TWI_LCR);
		for (timeout = 1000; timeout; timeout--)
			;
	}
	writel(0xa, TWI_LCR);
	if (readl(TWI_LCR) != 0x3a)
		ERR("bus status err: 0x%x\n", readl(TWI_LCR));

	writel(1, TWI_SRR);
}

static void twic_reset(void)
{
	/* reset twi comtroller */
	writel(1, TWI_SRR);
	INF("reset\n");
	/* wait twi reset completing */
	while (readl(TWI_SRR))
		;
}

static void twi_start(void)
{
	unsigned int value;

	value = readl(TWI_CTR);
	value |= TWI_MSTART;
	value &= ~TWI_INTFLAG;
	writel(value, TWI_CTR);
}

/*
 * twi_stop() - stop current twi transfer.
 *
 */
static u32 twi_stop(void)
{
	unsigned int timeout;
	unsigned int value;

	/* step1. set stop signal */
	value = readl(TWI_CTR);
	value |= TWI_MSTOP;
	value &= ~TWI_INTFLAG;
	writel(value, TWI_CTR);
	timeout = TWI_CHECK_TIMEOUT;

	/* step2. clear the interrupt flag */
	twi_clear_irq_flag();

	/* wait send finished */
	timeout = TWI_CHECK_TIMEOUT;
	while ((readl(TWI_CTR) & TWI_MSTOP) && (--timeout))
		;

	/* step3. check twi fsm is idle(0xf8) */
	timeout = TWI_CHECK_TIMEOUT;
	while ((0xf8 != readl(TWI_STR)) && (--timeout))
		;
	if (timeout == 0) {
		ERR("state err:%x\n", readl(TWI_STR));
		twic_reset();
		return -EFAIL;
	}

	/* step4. check twi scl & sda must high level */
	timeout = TWI_CHECK_TIMEOUT;
	while ((0x3a != readl(TWI_LCR)) && (--timeout))
		;
	if (timeout == 0) {
		ERR("lcr err:%x\n", readl(TWI_LCR));
		return -EFAIL;
	}
	INF("stop send\n");

	return OK;
}

/*
 * twic_set_sclk() - set twi clock to clk.
 *
 * @clk: the clock want to set.
 */
static void twic_set_sclk(u32 clk)
{
	u8 clk_m = 0;
	u8 clk_n = 0;
#ifdef CFG_FPGA_PLATFORM
	/* 24MHz clock source: 400KHz, clk_m = 5, clk_n = 0, 400KHz */
	clk_m = 5;
	clk_n = 0;
	goto out;
#else
	u32 sclk_real = 0;
	u32 src_clk = 0;
	u8 pow_clk_n = 1;
	u32 divider = 0;

	src_clk = ccu_get_sclk_freq(CCU_SYS_CLK_APBS2);

	divider = src_clk / clk;
	clk_m = 0;
	while (clk_n < 8) {
		clk_n++;
		pow_clk_n *= 2;
		while (clk_m < 16) {
			sclk_real = src_clk / (clk_m + 1) / pow_clk_n / 10;
			if (sclk_real <= clk)
				goto out;
			else
				clk_m++;
		}
	}
#endif

      out:
	/* set clock control register */
	writel((clk_m << 3) | clk_n, TWI_CKR);
	INF("m = %x, n = %x, req_sclk = %d, real_clk = %d\n", clk_m, clk_n, clk, sclk_real);
}

s32 twi_clkchangecb(u32 command, u32 freq)
{
	switch (command) {
	case CCU_CLK_CLKCHG_REQ:
		{
			/* check twi is busy now
			 * ...
			 * clock will be change, lock twi interface
			 */
			twi_lock = 1;
			INF("twi clk change request\n");
			return OK;
		}
	case CCU_CLK_CLKCHG_DONE:
		{
			/*
			 * clock change finish, re-config twi clock,
			 * maybe re-config rsb clock should do somethings?
			 */
			twic_set_sclk(TWI_CLOCK_FREQ);

			/* unlock twi interface */
			twi_lock = 0;
			INF("twi clk change done\n");
			return OK;
		}
	default:
		{
			break;
		}
	}

	return -EFAIL;
}

s32 twi_init(void)
{
#ifdef CFG_FPGA_PLATFORM
	/* PG[24][25] used as twi SCK and SDA under FPGA platform */

#endif

	/*
	 * initialize twi sck and sda pins
	 * judge pmu whether exist
	 */

	//if (arisc_para.power_mode != POWER_MODE_AXP) {
	//	twi_lock = TRUE;
	//	return OK;
	//}

	pin_set_multi_sel(PIN_GRP_PL, 0, 2);	/* PL0 config as TWI_SCK */
	pin_set_pull(PIN_GRP_PL, 0, PIN_PULL_UP);	/* TWI_SCK pull-up */
	pin_set_drive(PIN_GRP_PL, 0, PIN_MULTI_DRIVE_0);	/* TWI_SCK drive level 2 */

	pin_set_multi_sel(PIN_GRP_PL, 1, 2);	/* PL1 config as TWI_SDA */
	pin_set_pull(PIN_GRP_PL, 1, PIN_PULL_UP);	/* TWI_SDA pull-up */
	pin_set_drive(PIN_GRP_PL, 1, PIN_MULTI_DRIVE_0);	/* TWI_SDA drive level 2 */

	/* initialize twi clock */
	/* ccu_set_mclk_reset(CCU_MOD_CLK_R_TWI_1, CCU_CLK_RESET); */
	ccu_set_mclk_onoff(CCU_MOD_CLK_R_TWI, CCU_CLK_ON);
	ccu_set_mclk_reset(CCU_MOD_CLK_R_TWI, CCU_CLK_NRESET);
	twic_set_sclk(TWI_CLOCK_FREQ);
	writel(TWI_BUS_ENB | TWI_AACK, TWI_CTR);
	twic_reset();

	/* twi bus state is not reset value, try to restore it */
	if (readl(TWI_LCR) != 0x3a)
		twic_send_dummy_clock();

	twi_lock = 0;

#ifdef TWI_DEBUG
	printreg();
#endif
	if (readl(TWI_LCR) == 0x3a)
		LOG("init twi succeeded\n");
	else
		ERR("init twi failed!\n");

	return OK;
}

/*
 * twi_exit() - exit twi transfer.
 *
 * @return: 0 always
 */
s32 twi_exit(void)
{
	/* softreset twi module  */

	return OK;
}

/*
 * twi_byte_rw() - twi byte read and write.
 *
 * @op: operation read or write
 * @saddr: slave address
 * @baddr: byte address
 * @data: pointer to the data to be read or write
 * @return: EPDK_OK,      byte read or write successed;
 *          EPDK_FAIL,    btye read or write failed!
 */
s32 twi_byte_rw(twi_rw_type_e op, u8 saddr, u8 baddr, u8 *data, u32 len)
{
	volatile unsigned int state;
	unsigned int timeout;
	int ret = -1;
	s32 cpsr;

	if (twi_lock)
		return -EACCES;

	if (len > 4)
		return -EINVAL;

	twi_enable_ack();	/* enable ACK first */

	cpsr = cpu_disable_int();
	writel(0, TWI_EFR);
	state = (readl(TWI_STR) & 0x0ff);
	if (state != 0xf8) {
		ERR("e1:%x\n", state);
		goto stop_out;
	}

	/*
	 * control registser bitmap
	 *   7      6       5     4       3       2    1    0
	 * INT_EN  BUS_EN  START  STOP  INT_FLAG  ACK  NOT  NOT
	 */

	/* step1. Send Start */
	twi_start();

	timeout = TWI_CHECK_TIMEOUT;
	while ((!(readl(TWI_CTR) & TWI_INTFLAG)) && (--timeout))	/* wait for flg set */
		;
	if (timeout == 0) {
		ERR("to0\n");
		goto stop_out;
	}

	state = readl(TWI_STR);
	if (state != 0x08) {
		ERR("e3:%x\n", state);
		goto stop_out;
	}
	/* step2. Send Slave Address */
	writel(((saddr << 1) & 0x0fe), TWI_DTR);	/* slave address + write */
	twi_clear_irq_flag();	/* clear int flag to send saddr */

	timeout = TWI_CHECK_TIMEOUT;
	while ((!(readl(TWI_CTR) & TWI_INTFLAG)) && (--timeout))
		;
	if (timeout == 0) {
		ERR("to1\n");
		goto stop_out;
	}

	state = readl(TWI_STR);
	while (state != 0x18) {
		ERR("e5:%x\n", state);
		goto stop_out;
	}

	/* step3. Send Byte Address */
	writel(baddr, TWI_DTR);	/* slave address + write */
	twi_clear_irq_flag();	/* clear int flag to send regaddr */

	timeout = TWI_CHECK_TIMEOUT;
	while ((!(readl(TWI_CTR) & TWI_INTFLAG)) && (--timeout))
		;
	if (timeout == 0) {
		ERR("to2\n");
		goto stop_out;
	}

	state = readl(TWI_STR);
	if (state != 0x28) {
		ERR("e7:%x\n", state);
		goto stop_out;
	}

	if (op == TWI_WRITE) {
		/* step4. Send Data to be write */
		while (len--) {
			writel(*data++, TWI_DTR);	/* slave address + write */
			twi_clear_irq_flag();	/* clear int flag */

			timeout = TWI_CHECK_TIMEOUT;
			while ((!(readl(TWI_CTR) & TWI_INTFLAG)) && (timeout--))
				;
			if (timeout == -1) {
				ERR("to3\n");
				goto stop_out;
			}

			state = readl(TWI_STR);
			if (state != 0x28) {
				ERR("e9:%x\n", state);
				goto stop_out;
			}
		}
	} else {
		/* step4. Send restart for read */
		twi_start();
		twi_clear_irq_flag();	/* clear int flag to send saddr */

		timeout = TWI_CHECK_TIMEOUT;
		while ((!(readl(TWI_CTR) & TWI_INTFLAG)) && (--timeout))
			;
		if (timeout == 0) {
			ERR("to4\n");
			goto stop_out;
		}

		state = readl(TWI_STR);
		if (state != 0x10) {
			ERR("e11:%x\n", state);
			goto stop_out;
		}

		/* step5. Send Slave Address */
		writel(((saddr << 1) | 1), TWI_DTR);	/* slave address + write */
		twi_clear_irq_flag();	/* clear int flag to send saddr */

		timeout = TWI_CHECK_TIMEOUT;
		while ((!(readl(TWI_CTR) & TWI_INTFLAG)) && (--timeout))
			;
		if (timeout == 0) {
			ERR("to5\n");
			goto stop_out;
		}

		state = readl(TWI_STR);
		if (state != 0x40) {
			ERR("e13:%x\n", state);
			goto stop_out;
		}

		/* step6. Get data */
		while (len--) {
			if (len == 0)
				twi_disable_ack();

			twi_clear_irq_flag();	/* clear int flag then data come in */

			timeout = TWI_CHECK_TIMEOUT;
			while ((!(readl(TWI_CTR) & TWI_INTFLAG)) && (timeout--))
				;
			if (timeout == -1) {
				ERR("to6\n");
				goto stop_out;
			}

			*data++ = readl(TWI_DTR);
			state = readl(TWI_STR);
			if (len > 0) {
				if (state != 0x50) {
					ERR("e14:%x\n", state);
					goto stop_out;
				}
			} else {
				if (state != 0x58) {
					ERR("e15:%x\n", state);
					goto stop_out;
				}
			}
		}
	}
	ret = 0;

      stop_out:
	/* WRITE: step 5; READ: step 7 */
	twi_stop();		/* Send Stop */
#ifdef TWI_DEBUG
	INF("twi_%x r:%x d:%x\n", op, baddr, *data);
	printreg();		/* only for twi time sequence debuger */
#endif

	cpu_enable_int(cpsr);
	INF("%u,%u,%u,%u,%d\n", op, saddr, baddr, *data, ret);

	return ret;
}

s32 twi_read(u32 devaddr, u8 *addr, u8 *data, u32 len)
{
	u32 i;
	s32 ret = 0;

	for (i = 0; i < len; i++) {
		ret |= twi_byte_rw(TWI_READ, devaddr, *addr++, data++, 1);
	}

	return ret;
}

s32 twi_write(u32 devaddr, u8 *addr, u8 *data, u32 len)
{
	u32 i;
	s32 ret = 0;

	for (i = 0; i < len; i++) {
		ret |= twi_byte_rw(TWI_WRITE, devaddr, *addr++, data++, 1);
	}

	return ret;
}

bool is_twi_lock(void)
{
	return twi_lock;
}

int twi_send_clk_9pulse(void)
{
#define SCL_CTRL       (0x1 << 0x3)
#define SCL_CTRL_EN    (0x1 << 0x2)
	char cycle = 0;
	unsigned char status = 0;
	volatile unsigned int state;

	state = readl(TWI_LCR);
	/* enable scl control */
	writel(state | SCL_CTRL_EN, TWI_LCR);
	while (cycle < 9) {
		if ((readl(TWI_LCR) >> 0x4) & 0x1)
			break;
		/* twi_scl -> low */
		writel(readl(TWI_LCR) & (~SCL_CTRL), TWI_LCR);
		udelay(1000);

		/* twi_scl -> high */
		writel(readl(TWI_LCR) | SCL_CTRL, TWI_LCR);
		udelay(1000);
		cycle++;
	}

	/* if SDA is still low level. */
	if (!((readl(TWI_LCR) >> 0x4) & 0x1)) {
		status = -1;
		ERR("SDA is still low level!\n");
	}

	/* if (state & SCL_CTRL_EN) */
	/* 	return status; */
	/* else */
	/* disable scl control */
	writel(readl(TWI_LCR) & (~SCL_CTRL_EN), TWI_LCR);

	return status;
}

s32 twi_get_status(void)
{
	u32 reg_val;

	reg_val = readl(TWI_STR);
	if ((reg_val & 0xFF) != 0xF8)
		return -1;

	reg_val = readl(TWI_LCR);
	if ((reg_val & 0x30) != 0x30)
		return -1;

	return 0;
}

s32 twi_standby_init(void)
{
	pin_set_multi_sel(PIN_GRP_PL, 0, 2);	/* PL0 config as TWI_SCK */
	pin_set_pull(PIN_GRP_PL, 0, PIN_PULL_UP);	/* TWI_SCK pull-up */
	pin_set_drive(PIN_GRP_PL, 0, PIN_MULTI_DRIVE_0);	/* TWI_SCK drive level 2 */

	pin_set_multi_sel(PIN_GRP_PL, 1, 2);	/* PL1 config as TWI_SDA */
	pin_set_pull(PIN_GRP_PL, 1, PIN_PULL_UP);	/* TWI_SDA pull-up */
	pin_set_drive(PIN_GRP_PL, 1, PIN_MULTI_DRIVE_0);	/* TWI_SDA drive level 2 */

	/* initialize twi clock */
	/* ccu_set_mclk_reset(CCU_MOD_CLK_R_TWI_1, CCU_CLK_RESET); */
	ccu_set_mclk_onoff(CCU_MOD_CLK_R_TWI, CCU_CLK_ON);
	ccu_set_mclk_reset(CCU_MOD_CLK_R_TWI, CCU_CLK_NRESET);
	twic_set_sclk(TWI_CLOCK_FREQ);
	writel(TWI_BUS_ENB | TWI_AACK, TWI_CTR);
	twic_reset();

	/* twi bus state is not reset value, try to restore it */
	if (readl(TWI_LCR) != 0x3a)
		twic_send_dummy_clock();

	if (readl(TWI_LCR) == 0x3a)
		LOG("init twi succeeded\n");
	else
		ERR("init twi failed!\n");

	return OK;
}

s32 twi_standby_exit(void)
{
	return OK;
}

