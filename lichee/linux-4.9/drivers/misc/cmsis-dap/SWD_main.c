#include <linux/slab.h>
#include <linux/pm.h>
#include <linux/module.h>
#include <linux/io.h>
#include <linux/of.h>
#include <linux/of_gpio.h>
#include <linux/sunxi-gpio.h>
#include <linux/gpio.h>
#include <linux/platform_device.h>
#include <linux/uaccess.h>
#include "DAP_config.h"
#include "swd_host.h"
#include "SWD_flash.h"
#include "flash_blob.h"

struct fw_file {
	struct file *fp;
	char *data;
	size_t size;
};

struct sunxi_swd {
	struct device *dev;
	struct work_struct work;
	struct workqueue_struct *wq;
	struct gpio_config swdio;
	struct gpio_config swclk;
	struct gpio_config nrst;
	int update_progress;
	bool is_pending;
};

static int swd_debug = 1;
module_param(swd_debug, int, 0644);
MODULE_PARM_DESC(swd_debug, "echo 1 > debug for SWD_DBG info output");

#define SWD_TAG "[SWD] "
#define SWD_ERR(fmt, args...) printk(SWD_TAG fmt, ##args)

#define SWD_DBG(fmt, args...) if(swd_debug) { \
								printk(SWD_TAG fmt, ##args); \
								}

#define MCU_FIRMWARE ("/lib/firmware/mcu.bin")
#define FLASH_START_ADDR 0x08000000
#define MAX_RETRAIN_CNT	10
#define MAX_RESET_CNT	10
#define NRST_ACTIVE_LEVEL	1
#define NRST_INACTIVE_LEVEL	!NRST_ACTIVE_LEVEL

static char swdio_pin_name[8];
static char swclk_pin_name[8];
static char nrst_pin_name[8];
static int swdio_gpio = -1;
static int swclk_gpio = -1;
static int nrst_gpio = -1;

void port_swd_setup(void)
{
	long unsigned int config;
	char pin_dev_type[6];

	if (swdio_gpio >= SUNXI_PL_BASE && swdio_gpio < AXP_PIN_BASE)
		sprintf(pin_dev_type, SUNXI_R_PINCTRL);
	else
		sprintf(pin_dev_type, SUNXI_PINCTRL);

	config = SUNXI_PINCFG_PACK(SUNXI_PINCFG_TYPE_FUNC, 0xFFFF);
	pin_config_get(pin_dev_type, swdio_pin_name, &config);

	if (SUNXI_PIN_OUTPUT_FUNC != SUNXI_PINCFG_UNPACK_VALUE(config)) {
		config = SUNXI_PINCFG_PACK(SUNXI_PINCFG_TYPE_FUNC, SUNXI_PIN_OUTPUT_FUNC);
		pin_config_set(pin_dev_type, swdio_pin_name, config);
	}

	config = SUNXI_PINCFG_PACK(SUNXI_PINCFG_TYPE_PUD, 0xFFFF);
	pin_config_get(pin_dev_type, swdio_pin_name, &config);

	if (SUNXI_PIN_PULL_UP != SUNXI_PINCFG_UNPACK_VALUE(config)) {
		config = SUNXI_PINCFG_PACK(SUNXI_PINCFG_TYPE_PUD, SUNXI_PIN_PULL_UP);
		pin_config_set(pin_dev_type, swdio_pin_name, config);
	}

	if (swclk_gpio >= SUNXI_PL_BASE && swclk_gpio < AXP_PIN_BASE)
		sprintf(pin_dev_type, SUNXI_R_PINCTRL);
	else
		sprintf(pin_dev_type, SUNXI_PINCTRL);

	config = SUNXI_PINCFG_PACK(SUNXI_PINCFG_TYPE_FUNC, 0xFFFF);
	pin_config_get(pin_dev_type, swclk_pin_name, &config);

	if (SUNXI_PIN_OUTPUT_FUNC != SUNXI_PINCFG_UNPACK_VALUE(config)) {
		config = SUNXI_PINCFG_PACK(SUNXI_PINCFG_TYPE_FUNC, SUNXI_PIN_OUTPUT_FUNC);
		pin_config_set(pin_dev_type, swclk_pin_name, config);
	}

	config = SUNXI_PINCFG_PACK(SUNXI_PINCFG_TYPE_PUD, 0xFFFF);
	pin_config_get(pin_dev_type, swclk_pin_name, &config);

	if (SUNXI_PIN_PULL_UP != SUNXI_PINCFG_UNPACK_VALUE(config)) {
		config = SUNXI_PINCFG_PACK(SUNXI_PINCFG_TYPE_PUD, SUNXI_PIN_PULL_UP);
		pin_config_set(pin_dev_type, swclk_pin_name, config);
	}

	if (nrst_gpio >= SUNXI_PL_BASE && nrst_gpio < AXP_PIN_BASE)
		sprintf(pin_dev_type, SUNXI_R_PINCTRL);
	else
		sprintf(pin_dev_type, SUNXI_PINCTRL);

	config = SUNXI_PINCFG_PACK(SUNXI_PINCFG_TYPE_FUNC, 0xFFFF);
	pin_config_get(pin_dev_type, nrst_pin_name, &config);

	if (SUNXI_PIN_OUTPUT_FUNC != SUNXI_PINCFG_UNPACK_VALUE(config)) {
		config = SUNXI_PINCFG_PACK(SUNXI_PINCFG_TYPE_FUNC, SUNXI_PIN_OUTPUT_FUNC);
		pin_config_set(pin_dev_type, nrst_pin_name, config);
	}

	config = SUNXI_PINCFG_PACK(SUNXI_PINCFG_TYPE_PUD, 0xFFFF);
	pin_config_get(pin_dev_type, nrst_pin_name, &config);

	if (SUNXI_PIN_PULL_UP != SUNXI_PINCFG_UNPACK_VALUE(config)) {
		config = SUNXI_PINCFG_PACK(SUNXI_PINCFG_TYPE_PUD, SUNXI_PIN_PULL_UP);
		pin_config_set(pin_dev_type, nrst_pin_name, config);
	}

}

void port_off(void)
{
	long unsigned int config;
	char pin_dev_type[6];

	if (swdio_gpio >= SUNXI_PL_BASE && swdio_gpio < AXP_PIN_BASE)
		sprintf(pin_dev_type, SUNXI_R_PINCTRL);
	else
		sprintf(pin_dev_type, SUNXI_PINCTRL);

	config = SUNXI_PINCFG_PACK(SUNXI_PINCFG_TYPE_FUNC, 0xFFFF);
	pin_config_get(pin_dev_type, swdio_pin_name, &config);

	if (SUNXI_PIN_INPUT_FUNC != SUNXI_PINCFG_UNPACK_VALUE(config)) {
		config = SUNXI_PINCFG_PACK(SUNXI_PINCFG_TYPE_FUNC, SUNXI_PIN_INPUT_FUNC);
		pin_config_set(pin_dev_type, swdio_pin_name, config);
	}

	config = SUNXI_PINCFG_PACK(SUNXI_PINCFG_TYPE_PUD, 0xFFFF);
	pin_config_get(pin_dev_type, swdio_pin_name, &config);

	if (SUNXI_PIN_PULL_DISABLE != SUNXI_PINCFG_UNPACK_VALUE(config)) {
		config = SUNXI_PINCFG_PACK(SUNXI_PINCFG_TYPE_PUD, SUNXI_PIN_PULL_DISABLE);
		pin_config_set(pin_dev_type, swdio_pin_name, config);
	}

	if (swclk_gpio >= SUNXI_PL_BASE && swclk_gpio < AXP_PIN_BASE)
		sprintf(pin_dev_type, SUNXI_R_PINCTRL);
	else
		sprintf(pin_dev_type, SUNXI_PINCTRL);

	config = SUNXI_PINCFG_PACK(SUNXI_PINCFG_TYPE_FUNC, 0xFFFF);
	pin_config_get(pin_dev_type, swclk_pin_name, &config);

	if (SUNXI_PIN_INPUT_FUNC != SUNXI_PINCFG_UNPACK_VALUE(config)) {
		config = SUNXI_PINCFG_PACK(SUNXI_PINCFG_TYPE_FUNC, SUNXI_PIN_INPUT_FUNC);
		pin_config_set(pin_dev_type, swclk_pin_name, config);
	}

	config = SUNXI_PINCFG_PACK(SUNXI_PINCFG_TYPE_PUD, 0xFFFF);
	pin_config_get(pin_dev_type, swclk_pin_name, &config);

	if (SUNXI_PIN_PULL_DISABLE != SUNXI_PINCFG_UNPACK_VALUE(config)) {
		config = SUNXI_PINCFG_PACK(SUNXI_PINCFG_TYPE_PUD, SUNXI_PIN_PULL_DISABLE);
		pin_config_set(pin_dev_type, swclk_pin_name, config);
	}

	if (nrst_gpio >= SUNXI_PL_BASE && nrst_gpio < AXP_PIN_BASE)
		sprintf(pin_dev_type, SUNXI_R_PINCTRL);
	else
		sprintf(pin_dev_type, SUNXI_PINCTRL);

	config = SUNXI_PINCFG_PACK(SUNXI_PINCFG_TYPE_FUNC, 0xFFFF);
	pin_config_get(pin_dev_type, nrst_pin_name, &config);

	if (SUNXI_PIN_INPUT_FUNC != SUNXI_PINCFG_UNPACK_VALUE(config)) {
		config = SUNXI_PINCFG_PACK(SUNXI_PINCFG_TYPE_FUNC, SUNXI_PIN_INPUT_FUNC);
		pin_config_set(pin_dev_type, nrst_pin_name, config);
	}

	config = SUNXI_PINCFG_PACK(SUNXI_PINCFG_TYPE_PUD, 0xFFFF);
	pin_config_get(pin_dev_type, nrst_pin_name, &config);

	if (SUNXI_PIN_PULL_DISABLE != SUNXI_PINCFG_UNPACK_VALUE(config)) {
		config = SUNXI_PINCFG_PACK(SUNXI_PINCFG_TYPE_PUD, SUNXI_PIN_PULL_DISABLE);
		pin_config_set(pin_dev_type, nrst_pin_name, config);
	}

}

uint32_t pin_swclk_tck_in(void)
{
	long unsigned int config;
	char pin_dev_type[6];

	if (swdio_gpio >= SUNXI_PL_BASE && swdio_gpio < AXP_PIN_BASE)
		sprintf(pin_dev_type, SUNXI_R_PINCTRL);
	else
		sprintf(pin_dev_type, SUNXI_PINCTRL);

	config = SUNXI_PINCFG_PACK(SUNXI_PINCFG_TYPE_DAT, 0xFFFF);
	pin_config_get(pin_dev_type, swdio_pin_name, &config);

	return SUNXI_PINCFG_UNPACK_VALUE(config) ? 1 : 0;
}

void pin_swclk_tck_set(void)
{
	__gpio_set_value(swclk_gpio, 1);
}

void pin_swclk_tck_clr(void)
{
	__gpio_set_value(swclk_gpio, 0);
}

uint32_t pin_swdio_tms_in(void)
{
	long unsigned int config;
	char pin_dev_type[6];

	if (swdio_gpio >= SUNXI_PL_BASE && swdio_gpio < AXP_PIN_BASE)
		sprintf(pin_dev_type, SUNXI_R_PINCTRL);
	else
		sprintf(pin_dev_type, SUNXI_PINCTRL);

	config = SUNXI_PINCFG_PACK(SUNXI_PINCFG_TYPE_DAT, 0xFFFF);
	pin_config_get(pin_dev_type, swdio_pin_name, &config);

	return SUNXI_PINCFG_UNPACK_VALUE(config) ? 1 : 0;
}

void pin_swdio_tms_set(void)
{
	__gpio_set_value(swdio_gpio, 1);
}

void pin_swdio_tms_clr(void)
{
	__gpio_set_value(swdio_gpio, 0);
}

uint32_t pin_swdio_in(void)
{
	long unsigned int config;
	char pin_dev_type[6];

	if (swdio_gpio >= SUNXI_PL_BASE && swdio_gpio < AXP_PIN_BASE)
		sprintf(pin_dev_type, SUNXI_R_PINCTRL);
	else
		sprintf(pin_dev_type, SUNXI_PINCTRL);

	config = SUNXI_PINCFG_PACK(SUNXI_PINCFG_TYPE_DAT, 0xFFFF);
	pin_config_get(pin_dev_type, swdio_pin_name, &config);

	return SUNXI_PINCFG_UNPACK_VALUE(config) ? 1 : 0;
}

void pin_swdio_out(uint32_t bit)
{
	if (bit & 1)
		__gpio_set_value(swdio_gpio, 1);
	else
		__gpio_set_value(swdio_gpio, 0);
}

void pin_swdio_out_enable(void)
{
	long unsigned int config;
	char pin_dev_type[6];

	if (swdio_gpio >= SUNXI_PL_BASE && swdio_gpio < AXP_PIN_BASE)
		sprintf(pin_dev_type, SUNXI_R_PINCTRL);
	else
		sprintf(pin_dev_type, SUNXI_PINCTRL);

	config = SUNXI_PINCFG_PACK(SUNXI_PINCFG_TYPE_FUNC, 0xFFFF);
	pin_config_get(pin_dev_type, swdio_pin_name, &config);

	if (SUNXI_PIN_OUTPUT_FUNC != SUNXI_PINCFG_UNPACK_VALUE(config)) {
		config = SUNXI_PINCFG_PACK(SUNXI_PINCFG_TYPE_FUNC, SUNXI_PIN_OUTPUT_FUNC);
		pin_config_set(pin_dev_type, swdio_pin_name, config);
	}

	config = SUNXI_PINCFG_PACK(SUNXI_PINCFG_TYPE_PUD, 0xFFFF);
	pin_config_get(pin_dev_type, swdio_pin_name, &config);

	if (SUNXI_PIN_PULL_DOWN != SUNXI_PINCFG_UNPACK_VALUE(config)) {
		config = SUNXI_PINCFG_PACK(SUNXI_PINCFG_TYPE_PUD, SUNXI_PIN_PULL_UP);
		pin_config_set(pin_dev_type, swdio_pin_name, config);
	}
}

void pin_swdio_out_disable(void)
{
	long unsigned int config;
	char pin_dev_type[6];

	if (swdio_gpio >= SUNXI_PL_BASE && swdio_gpio < AXP_PIN_BASE)
		sprintf(pin_dev_type, SUNXI_R_PINCTRL);
	else
		sprintf(pin_dev_type, SUNXI_PINCTRL);

	config = SUNXI_PINCFG_PACK(SUNXI_PINCFG_TYPE_FUNC, 0xFFFF);
	pin_config_get(pin_dev_type, swdio_pin_name, &config);

	if (SUNXI_PIN_INPUT_FUNC != SUNXI_PINCFG_UNPACK_VALUE(config)) {
		config = SUNXI_PINCFG_PACK(SUNXI_PINCFG_TYPE_FUNC, SUNXI_PIN_INPUT_FUNC);
		pin_config_set(pin_dev_type, swdio_pin_name, config);
	}

	config = SUNXI_PINCFG_PACK(SUNXI_PINCFG_TYPE_PUD, 0xFFFF);
	pin_config_get(pin_dev_type, swdio_pin_name, &config);

	if (SUNXI_PIN_PULL_UP != SUNXI_PINCFG_UNPACK_VALUE(config)) {
		config = SUNXI_PINCFG_PACK(SUNXI_PINCFG_TYPE_PUD, SUNXI_PIN_PULL_DISABLE);
		pin_config_set(pin_dev_type, swdio_pin_name, config);
	}
}

void pin_nrst_out(uint32_t bit)
{
	if (bit & 1)
		__gpio_set_value(nrst_gpio, NRST_INACTIVE_LEVEL);
	else
		__gpio_set_value(nrst_gpio, NRST_ACTIVE_LEVEL);
}

static __inline void pin_nrst_active(void)
{
	__gpio_set_value(nrst_gpio, NRST_ACTIVE_LEVEL);
}

static __inline void pin_nrst_inactive(void)
{
	__gpio_set_value(nrst_gpio, NRST_INACTIVE_LEVEL);
}

static void hw_reset_mcu(void)
{
	pin_nrst_active();
	mdelay(20);
	pin_nrst_inactive();
}

static inline void *fw_kmalloc(unsigned int len, bool isDMA)
{
	gfp_t flags = (in_atomic()) ? GFP_ATOMIC : GFP_KERNEL;

	flags = isDMA ? (flags | GFP_DMA) : flags;

	return kmalloc(len, flags);
}

struct fw_file *fw_fileopen(const char *path, int open_mode, umode_t mode)
{
	struct fw_file *fw_fp = NULL;
	if (!path)
		return NULL;

	fw_fp = fw_kmalloc(sizeof(struct fw_file), false);
	if (!fw_fp) {
		SWD_ERR("%s: fw_kmalloc failed, size = %lu!\n",
			__func__, (unsigned long)sizeof(struct fw_file));
		return NULL;
	}

	memset(fw_fp, 0, sizeof(struct fw_file));

	/* open file */
	fw_fp->fp = filp_open(path, open_mode, mode);
	if (IS_ERR(fw_fp->fp)) {
		SWD_ERR("%s: filp_open failed(%ld)\n", __func__, (long)fw_fp->fp);
		kfree(fw_fp);
		return NULL;
	}

	/* get file size. */
	if (fw_fp->fp->f_op->llseek != NULL) {
		mm_segment_t old_fs = get_fs();
		set_fs(KERNEL_DS);
		fw_fp->size = vfs_llseek(fw_fp->fp, 0, SEEK_END);
		vfs_llseek(fw_fp->fp, 0, SEEK_SET);
		set_fs(old_fs);
	} else {
		fw_fp->size = 0;
	}

	return fw_fp;
}

int fw_fileclose(const struct fw_file *fw_fp)
{
	if (fw_fp) {
		if (!IS_ERR(fw_fp->fp))
			filp_close(fw_fp->fp, NULL);
		if (fw_fp->data)
			kfree(fw_fp->data);
		kfree(fw_fp);

		return 0;
	}

	return -1;
}

int fw_fileread(const struct fw_file *fw_fp, char *buffer, int size)
{
	int ret = -1;
	if (fw_fp && buffer) {
		mm_segment_t old_fs = get_fs();
		set_fs(KERNEL_DS);
		ret = vfs_read(fw_fp->fp, buffer, size, &fw_fp->fp->f_pos);
		if (ret < 0) {
			SWD_ERR("%s: failed(%d)\n", __func__, ret);
		}
		set_fs(old_fs);
	}

	return ret;
}

static void swd_firmware(struct work_struct *work)
{
	struct sunxi_swd *swd = container_of(work, struct sunxi_swd, work);
	const struct fw_file *firmware = NULL;
	const char *fw_path = NULL;
	int ret, num_blocks, block;
	u8 retrain_cnt = MAX_RETRAIN_CNT;
	u8 reset_cnt = MAX_RESET_CNT;
	u8 *buf = NULL;
	u32 put = 0;
	struct timeval start, end;

	SWD_DBG("%s: enter\n", __func__);

	do_gettimeofday(&start);
	swd->is_pending = false;

	fw_path = MCU_FIRMWARE;

	firmware = fw_fileopen(fw_path, O_RDONLY, 0);
	if (!firmware) {
		SWD_ERR("%s: can't load firmware file %s.\n",
			__func__, fw_path);
		ret = -1;
		goto error;
	}


	buf = fw_kmalloc(DOWNLOAD_BLOCK_SIZE, true);
	if (!buf) {
		SWD_ERR("%s: can't allocate firmware buffer. \n", __func__);
		ret = -ENOMEM;
		goto error;
	}
	memset(buf, 0, DOWNLOAD_BLOCK_SIZE);

	num_blocks = (firmware->size -1) / DOWNLOAD_BLOCK_SIZE + 1;

	SWD_DBG("%s: firmware->size = %lu, num_blocks = %d\n",
		__func__, firmware->size, num_blocks);

	swd->update_progress = 0;

	SWD_DBG("%s: put %d, progress %d\n", __func__, put, swd->update_progress);

	/* swd init */
	SWD_DBG("%s: %d, swd_init_debug\n", __func__, __LINE__);
	swd_init_debug();
	SWD_DBG("%s: %d, target_flash_init\n", __func__, __LINE__);
	ret = target_flash_init(FLASH_START_ADDR);
    if (ret > 0) {
        SWD_ERR("%s: target_flash_init error %d.\n", __func__, ret);
        goto error;
    }

	/* erase whole chip */
	SWD_DBG("%s: %d, target_flash_erase_chip\n", __func__, __LINE__);
	target_flash_erase_chip();

	/* write chip */
	for (block = 0; block < num_blocks; block++) {
		size_t block_size;

		block_size = min((size_t)(firmware->size - put), (size_t)DOWNLOAD_BLOCK_SIZE);
		ret = fw_fileread(firmware, buf, block_size);
		if (ret < block_size) {
			SWD_ERR("%s: fw_fileread error %d.\n", __func__, ret);
			goto error;
		}

		if (block_size < DOWNLOAD_BLOCK_SIZE) {
			memset(&buf[block_size], 0, DOWNLOAD_BLOCK_SIZE - block_size);
		}
		SWD_DBG("%s: block_size %lu\n", __func__, block_size);

		while ( retrain_cnt && reset_cnt && ( ret = target_flash_program_page(FLASH_START_ADDR + block * DOWNLOAD_BLOCK_SIZE, buf, block_size)) != ERROR_SUCCESS ) {
			retrain_cnt--;
			SWD_ERR("%s: left retrain_cnt %d, reset_cnt %d, error no: %d, block %d, errstr: %s\n",
				__func__, retrain_cnt, reset_cnt, ret, block, error_get_string(ret));

			if (retrain_cnt == 0) {
				retrain_cnt = MAX_RETRAIN_CNT;
				reset_cnt--;
				if (0 == reset_cnt)
					goto reboot;

				SWD_DBG("%s: %d, hw_reset_mcu\n", __func__, __LINE__);
				hw_reset_mcu();

				SWD_DBG("%s: %d, swd_init_debug\n", __func__, __LINE__);
				swd_init_debug();
				SWD_DBG("%s: %d, target_flash_init\n", __func__, __LINE__);
				target_flash_init(FLASH_START_ADDR);

			} else {
				// retrain target
				JTAG2SWD();
			}
		}

		put += block_size;
		swd->update_progress = (block + 1) * 100 / num_blocks;
		SWD_DBG("%s: put %d, progress %d\n", __func__, put, swd->update_progress);
	}

reboot:
	//swd_set_target_reset(0);
	swd_set_target_state_hw(RESET_RUN);

	do_gettimeofday(&end);
	SWD_DBG("%s: %d, cost time %ld s\n", __func__, __LINE__, end.tv_sec - start.tv_sec);

error:
	if (buf)
		kfree(buf);

	if (firmware) {
		fw_fileclose(firmware);
	}

	swd->is_pending = true;
}

static ssize_t swd_update_store(struct device *dev,
	struct device_attribute *attr, const char *buf, size_t count)
{
	unsigned long data;
	int ret;
	struct sunxi_swd *swd = dev_get_drvdata(dev);
	static int pending_cnt = 0;

	data = simple_strtoul(buf, NULL, 10);

	if (1 == data) {
		if (swd->is_pending) {
			pending_cnt = 0;
			cancel_work_sync(&swd->work);
			flush_workqueue(swd->wq);
			queue_work(swd->wq, &swd->work);
/*		} else {
			SWD_ERR("%s: swd is not pending!\n", __func__);
			pending_cnt++;
			if (pending_cnt > 3) {
				// reset target
				pin_nrst_low();
				mdelay(2);
				pin_nrst_high();
				pending_cnt = 0;
			} */
		}
	} else
		SWD_ERR("%s: params error!\n", __func__);

	return count;
}

static ssize_t swd_update_progress_show(struct device *dev,
	struct device_attribute *attr, char *buf)
{
	struct sunxi_swd *swd = dev_get_drvdata(dev);

	return sprintf(buf, "%d\n", swd->update_progress);
}

static ssize_t swd_reset_store(struct device *dev,
	struct device_attribute *attr, const char *buf, size_t count)
{
	unsigned long data;
	int ret;
	struct sunxi_swd *swd = dev_get_drvdata(dev);
	static int pending_cnt = 0;

	data = simple_strtoul(buf, NULL, 10);

	if (1 == data) {
		hw_reset_mcu();
		SWD_ERR("%s: reset MCU!\n", __func__);
	} else
		SWD_ERR("%s: params error!\n", __func__);

	return count;
}

static DEVICE_ATTR(swd_update, S_IRUGO | S_IWUSR | S_IWGRP,
		NULL, swd_update_store);

static DEVICE_ATTR(update_progress, S_IRUGO | S_IWUSR | S_IWGRP,
		swd_update_progress_show, NULL);

static DEVICE_ATTR(swd_reset, S_IRUGO | S_IWUSR | S_IWGRP,
		NULL, swd_reset_store);

static struct attribute *swd_attributes[] = {
	&dev_attr_swd_update.attr,
	&dev_attr_update_progress.attr,
	&dev_attr_swd_reset.attr,
	NULL
};

static struct attribute_group swd_attribute_group = {
	.attrs = swd_attributes
};

static int swd_parse_dt(struct platform_device *pdev)
{
	struct sunxi_swd *swd = platform_get_drvdata(pdev);
	struct device *dev = &pdev->dev;
	struct device_node *np = dev->of_node;

	swd->swdio.gpio = of_get_named_gpio_flags(np, "swdio", 0,
			(enum of_gpio_flags *)(&(swd->swdio)));
	if (!gpio_is_valid(swd->swdio.gpio))
		dev_err(dev, "%s: get swdio err!\n", __func__);
	else {
		swdio_gpio = swd->swdio.gpio;
		sunxi_gpio_to_name(swd->swdio.gpio, swdio_pin_name);
	}

	swd->swclk.gpio = of_get_named_gpio_flags(np, "swclk", 0,
			(enum of_gpio_flags *)(&(swd->swclk)));
	if (!gpio_is_valid(swd->swclk.gpio))
		dev_err(dev, "%s: get swclk err!\n", __func__);
	else {
		swclk_gpio = swd->swclk.gpio;
		sunxi_gpio_to_name(swd->swclk.gpio, swclk_pin_name);
	}

	swd->nrst.gpio = of_get_named_gpio_flags(np, "nrst", 0,
			(enum of_gpio_flags *)(&(swd->nrst)));
	if (!gpio_is_valid(swd->nrst.gpio))
		dev_err(dev, "%s: get nrst err!\n", __func__);
	else {
		nrst_gpio = swd->nrst.gpio;
		sunxi_gpio_to_name(swd->nrst.gpio, nrst_pin_name);

		gpio_direction_output(nrst_gpio, NRST_INACTIVE_LEVEL);
	}

	dev_info(dev, "%s: swdio.gpio = %d [%s], swclk.gpio = %d [%s], nrst.gpio = %d [%s]\n",
		__func__, swd->swdio.gpio, swdio_pin_name, swd->swclk.gpio, swclk_pin_name, swd->nrst.gpio, nrst_pin_name);

	return 0;
}

static int swd_probe(struct platform_device *pdev)
{
	struct sunxi_swd *swd;
	struct device *dev = &pdev->dev;
	int ret = 0;

	SWD_DBG("%s: start\n", __func__);

	swd = kzalloc(sizeof(struct sunxi_swd), GFP_KERNEL);
	if (!swd) {
		dev_err(dev, "%s: kzalloc failed\n", __func__);
		ret = -ENOMEM;
	}

	platform_set_drvdata(pdev, swd);
	swd->dev = dev;

	swd_parse_dt(pdev);

	swd->wq = create_singlethread_workqueue("kworkqueue_swd");
	if (!swd->wq) {
		dev_err(dev, "%s: create sysfs swd_attribute_group failed!\n", __func__);
		goto emem;
	}
	flush_workqueue(swd->wq);

	swd->is_pending = true;
	INIT_WORK(&swd->work, swd_firmware);

	ret = sysfs_create_group(&dev->kobj, &swd_attribute_group);
	if (ret) {
		dev_err(dev, "%s: create sysfs swd_attribute_group failed!\n", __func__);
		goto ework;
	}

	SWD_DBG("%s: finish\n", __func__);

	return 0;

ework:
	destroy_workqueue(swd->wq);
emem:
	kfree(swd);

	return ret;
}

static int swd_remove(struct platform_device *pdev)
{
	struct sunxi_swd *swd = platform_get_drvdata(pdev);
	struct device *dev = &pdev->dev;

	cancel_work_sync(&swd->work);
	destroy_workqueue(swd->wq);
	kfree(swd);
	swd = NULL;

	return 0;
}

#ifdef CONFIG_PM
static int swd_suspend_noirq(struct device *dev)
{
	struct platform_device *pdev = to_platform_device(dev);
	struct sunxi_swd *swd = platform_get_drvdata(pdev);

	return 0;
}

static int swd_resume_noirq(struct device *dev)
{
	struct platform_device *pdev = to_platform_device(dev);
	struct sunxi_swd *swd = platform_get_drvdata(pdev);

	return 0;
}

static const struct dev_pm_ops swd_dev_pm_ops = {
	.suspend_noirq	 = swd_suspend_noirq,
	.resume_noirq	 = swd_resume_noirq,
};

#define SWD_DEV_PM_OPS (&swd_dev_pm_ops)
#else
#define SWD_DEV_PM_OPS NULL
#endif

static const struct of_device_id swd_dt_ids[] = {
	{.compatible = "creality,swd"},
	{},
};

static struct platform_driver swd_driver = {
	.probe = swd_probe,
	.remove = swd_remove,
	.driver = {
		.name = "swd",
		.owner	= THIS_MODULE,
		.pm	= SWD_DEV_PM_OPS,
		.of_match_table = swd_dt_ids,
	},
};

module_platform_driver(swd_driver);

MODULE_DESCRIPTION("CMSIS-DAP SWD");
MODULE_AUTHOR("wh21");
MODULE_LICENSE("GPL");
