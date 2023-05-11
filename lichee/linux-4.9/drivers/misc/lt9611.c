/*
 *  lt9611.c - Linux kernel module for
 * 	Lontium LT9611
 *
 *  Copyright (c) 2022 wh21 <wuhui@creality.com>
 *
 *  Based on code written by
 *  	xhguo
 */
/******************************************************************************/
/*******************************************************************************
History:
V9.0 20200612
1. Add CEC interrupter.

V10 20200612
1. update txpll power on sequence.
2. update hpd source from hpd and rx dc det.

V11 20200806
1. change hpd source from hpd only(0x8258 = 0x0a).
2. pcr_m --;

V12 20201009
1. CEC compliance Test.

V13. 20201024
1. support U3.

********************************************************************************/

#include <linux/module.h>
#include <linux/slab.h>
#include <linux/i2c.h>
#include <linux/mutex.h>
#include <linux/delay.h>
#include <linux/of_gpio.h>
#include <linux/gpio.h>
#include <linux/sunxi-gpio.h>
#include <linux/irq.h>
#include <linux/interrupt.h>
#include <linux/uaccess.h>

#include "lt9611.h"

#define LT9611_DRV_NAME	"lt9611"

struct lt9611_data {
	struct i2c_client *client;
	struct mutex lock;
	int reset_gpio;
	u8 rst_active_level;
	int power_gpio;
	u8 pwr_active_level;
	int irq_gpio;
	spinlock_t irq_lock;
	struct work_struct work;
	struct workqueue_struct *lt9611_wq;
};

struct Lontium_IC_Mode lt9611 = {
single_port_mipi_A, //mipi_port_cnt; //single_port_mipi_A or single_port_mipi_B or dual_port_mipi
lane_cnt_4,       //mipi_lane_cnt; //1 or 2 or 4
dsi,              //mipi_mode;     //dsi or csi
Burst_Mode,
audio_i2s,       //audio_out      //audio_i2s or audio_spdif       //hdmi_coupling_mode;//ac_mode or dc_mode
dc_mode,         //hdmi_coupling_mode;//ac_mode or dc_mode
hdcp_disable,    //hdcp_encryption //hdcp_enable or hdcp_disable
HDMI,            //HDMI or DVI
RGB888           //RGB888 or YUV422
};

//DTV 									      // hfp, hs, hbp,hact,htotal,vfp, vs, vbp,vact,vtotal, h_polary, v_polary, vic, pclk_khz
struct video_timing video_640x480_60Hz     ={16, 96,  48, 640,   800, 10,  2,  33, 480,   525, 0, 0, 1, AR_4_3, 25000};
struct video_timing video_720x480_60Hz     ={16, 62,  60, 720,   858,  9,  6,  30, 480,   525, 0, 0, 2, AR_4_3, 27000};
struct video_timing video_720x576_50Hz     ={12, 64,  68, 720,   864,  5,  5,  39, 576,   625, 0, 0, 17,AR_4_3, 27000};

struct video_timing video_1280x720_60Hz    ={110,40, 220,1280,  1650,  5,  5,  20, 720,   750, 1, 1, 4, AR_16_9,74250};
struct video_timing video_1280x720_50Hz    ={440,40, 220,1280,  1980,  5,  5,  20, 720,   750, 1, 1, 19,AR_16_9,74250};
struct video_timing video_1280x720_30Hz    ={1760, 40, 220,1280,  3300,  5,  5,  20, 720, 750,   1,1,0, AR_16_9,74250};

struct video_timing video_1920x1080_60Hz   ={88, 44, 148,1920,  2200,  4,  5,  36, 1080,  1125, 1,1,16,AR_16_9,148500};
struct video_timing video_1920x1080_50Hz   ={528, 44, 148,1920, 2640,  4,  5,  36, 1080, 1125, 1,1,31,AR_16_9,148500};
struct video_timing video_1920x1080_30Hz   ={88, 44, 148,1920,  2200,  4,  5,  36, 1080,  1125, 1,1,34,AR_16_9,74250};
struct video_timing video_1920x1080_25Hz   ={528, 44, 148,1920,  2640,  4,  5,  36, 1080, 1125, 1,1,33,AR_16_9,74250};
struct video_timing video_1920x1080_24Hz   ={638, 44, 148,1920,  2750,  4,  5,  36, 1080, 1125, 1,1,32,AR_16_9,74250};

struct video_timing video_3840x2160_30Hz   ={176,88, 296,3840,  4400,  8,  10, 72, 2160, 2250, 1,1,95,AR_16_9, 297000};
struct video_timing video_3840x2160_25Hz   ={1056,88, 296,3840,  5280,  8,  10, 72, 2160, 2250,1,1,94,AR_16_9, 297000};
struct video_timing video_3840x2160_24Hz   ={1276,88, 296,3840,  5500,  8,  10, 72, 2160, 2250,1,1,93,AR_16_9,297000};
/*
struct video_timing video_2560x1080_60Hz   ={248,44, 148,2560,2720,  4,  5, 11, 1080,1100, 1,1,0,AR_16_9,198000};
//VESA-DMT
struct video_timing video_800x600_60Hz     ={48, 128, 88, 800, 1056,  1,  4,  23, 600, 628,     1,1,0,AR_16_9,40000};
struct video_timing video_1024x768_60Hz    ={24, 136, 160, 1024, 1344,  3,  6,  29, 768, 806,   0,0,0,AR_16_9,65000};
//struct video_timing video_1280x768_60Hz    ={48, 32,   80, 1280, 1440,  3,  7,  12, 768, 790,   1,0,0,AR_16_9,68250}; //reduced blank
struct video_timing video_1280x768_60Hz    ={64, 128,   192, 1280, 1664,  3,  7,  20, 768, 798,   0,1,0,AR_16_9,79500}; //Normal
//20190328
//struct video_timing video_1280x800_60Hz   ={72, 128, 200, 1280, 1680,  3,  6,  22, 800, 831, 0,1,0,AR_16_9,83500};
struct video_timing video_1280x800_60Hz   ={28, 32, 100, 1280, 1440,  2,  6,  15, 800, 823, 0,0,0,AR_16_9,71000};
struct video_timing video_1280x960_60Hz   ={96, 112, 312, 1280, 1800,  1,  3,  36, 960, 1000, 1,1,0,AR_16_9,108000};

struct video_timing video_1280x800_30Hz   ={72, 128, 200, 1280, 1680,  3,  6,  22, 800, 831, 0,1,0,AR_16_9,41750};
struct video_timing video_1280x960_30Hz   ={96, 112, 312, 1280, 1800,  1,  3,  36, 960, 1000, 1,1,0,AR_16_9,54000};

struct video_timing video_1280x1024_60Hz   ={48, 112, 248, 1280, 1688,  1,  3,  38, 1024, 1066, 1,1,0,AR_16_9,108000};
struct video_timing video_1600x1200_60Hz   ={64, 192, 304, 1600, 2160,  1,  3,  46, 1200, 1250, 1,1,0,AR_16_9,162000};
//struct video_timing video_1680x1050_60Hz   ={48, 32,   80, 1680, 1840,  3,  6,  21, 1050, 1080, 1,0,0,AR_16_9,119000}; //reduced blank
struct video_timing video_1680x1050_60Hz   ={104, 176,   280, 1680, 2240,  3,  6,  30, 1050, 1089, 0,1,0,AR_16_9,146250};//Normal

struct video_timing video_1280x720_30Hz    ={1760, 40, 220,1280,  3300,  5,  5,  20, 720, 750,   1,1,0, AR_16_9,74250};
struct video_timing video_1024x600_60Hz    ={60,60, 100,1024,  1154,  2,  5, 10, 600, 617,      1,1,0,AR_16_9,34000};

struct video_timing video_1366x768_60Hz    ={14, 56,  64,1366,  1500,  1,  3,  28, 768,  800,   1,1,0, AR_16_9,74000};

struct video_timing video_2560x1440_50Hz   ={176,272, 448,2560,  3456,  3,  5, 36, 1440, 1484,  1,1,0, AR_16_9,256250};
struct video_timing video_2560x1440_60Hz   ={48,32, 80,2560,  2720,  3,  5, 33, 1440, 1481,     1,1,0, AR_16_9,241500};

//20180731								        //hfp, hs, hbp,hact,htotal,vfp, vs, vbp,vact,vtotal, pclk_khz
struct video_timing video_2560x1600_60Hz   ={48,32, 80,2560,  2720,  3,  6, 37, 1600, 1646,     1,1,0,AR_16_9,268500};

//VR timing
struct video_timing video_3840x1080_60Hz   ={176,88, 296,3840,  4400,  4,  5,  36, 1080, 1125,   1,1,0, AR_16_9,297000};
struct video_timing video_2560x720_60Hz    ={220,80, 440,2560,  3300,  5,  5,  20, 720,   750, 1, 1, 0, AR_16_9,148500};

//mipi panel resolution
struct video_timing video_1080x1920_60Hz  ={  4,  5,  36, 1080, 1125, 88, 44, 148,1920,  2200,  1,1,0,AR_16_9,148500};
struct video_timing video_720x1280_60Hz    ={  5,  5,  20, 720,   750, 110,40, 220,1280,  1650,  1,1,0,AR_16_9,74250};
struct video_timing video_540x960_60Hz      ={30, 10, 30, 540, 610,  10,  10,  10, 960, 990,      1,1,0,AR_16_9,33500};
*/

//others
struct video_timing video_1200x1920_60Hz    ={180,60, 160, 1200, 1600,  35, 10, 25, 1920, 1990, 1, 1, 0, AR_16_9,191040}; 
struct video_timing video_1920x720_60Hz     ={88, 44, 148, 1920, 2200,  5,  5,  20, 720,  750,  1, 1, 0, AR_16_9,100000}; 


struct video_timing *video;
struct cec_msg lt9611_cec_msg = {0};

static u8 CEC_RxData_Buff[16];
static u8 CEC_TxData_Buff[18];

u8 pcr_m;
bool flag_cec_data_received = 0;

u8 Sink_EDID[256];
bool Tx_HPD=0;
bool irq_task_flag = 0;

enum VideoFormat Video_Format;

/*
 * sysfs layer
 */

/* access registers */

static ssize_t lt9611_store_read(struct device *dev,
		struct device_attribute *attr, const char *buf, size_t count)
{
	struct i2c_client *client = to_i2c_client(dev);
	unsigned long val;
	int ret;

	ret = kstrtoul(buf, 16, &val);
	if (ret)
		return ret;

	dev_info(&client->dev, "read reg[0x%x]: 0x%02x\n", (u8)val, HDMI_ReadI2C_Byte(client, (u8)val));

	return count;
}

static DEVICE_ATTR(read, S_IWUSR | S_IRUGO,
		   NULL, lt9611_store_read);

static ssize_t lt9611_store_write(struct device *dev,
		struct device_attribute *attr, const char *buf, size_t count)
{
	struct i2c_client *client = to_i2c_client(dev);
	unsigned int reg;
	unsigned int val;
	int ret;

	ret = sscanf(buf, "%02x %02x", &reg, &val);
	if (ret != 2)
		return ret;

	dev_info(&client->dev, "write reg[0x%x]: 0x%02x\n", (u8)reg, (u8)val);
	HDMI_WriteI2C_Byte(client, (u8)reg, (u8)val);

	return count;
}

static DEVICE_ATTR(write, S_IWUSR | S_IRUGO,
		   NULL, lt9611_store_write);

static struct attribute *lt9611_attributes[] = {
	&dev_attr_read.attr,
	&dev_attr_write.attr,
	NULL
};

static const struct attribute_group lt9611_attr_group = {
	.attrs = lt9611_attributes,
};

static int lt9611_parse_dts(struct i2c_client *client)
{
	struct lt9611_data *data = i2c_get_clientdata(client);
	struct device *dev = &client->dev;
	struct device_node *np = dev->of_node;
	struct gpio_config cfg;
	int ret = 0;

	data->reset_gpio = of_get_named_gpio_flags(np, "reset_gpio", 0,
			(enum of_gpio_flags *)&cfg);
	data->rst_active_level = 0;
	if (!gpio_is_valid(data->reset_gpio)) {
		dev_err(dev, "%s: get reset_gpio err!\n", __func__);
	} else {
		if (gpio_request(data->reset_gpio, "lt9611-rst") < 0) {
			dev_err(dev, "reset gpio request fail!\n");
			ret = -EINVAL;
			goto rst_pin_err;
		}
		gpio_direction_output(data->reset_gpio, data->rst_active_level);
	}

	data->power_gpio = of_get_named_gpio_flags(np, "power_gpio", 0,
			(enum of_gpio_flags *)&cfg);
	data->pwr_active_level = 1;
	if (!gpio_is_valid(data->power_gpio)) {
		dev_err(dev, "%s: get power_gpio err!\n", __func__);
	} else {
		if (gpio_request(data->power_gpio, "lt9611-pwr") < 0) {
			dev_err(dev, "power gpio request fail!\n");
			ret = -EINVAL;
			goto pwr_pin_err;
		}
		gpio_direction_output(data->power_gpio, !data->pwr_active_level);
	}

	data->irq_gpio = of_get_named_gpio_flags(np, "irq_gpio", 0,
			(enum of_gpio_flags *)&cfg);
	if (!gpio_is_valid(data->irq_gpio)) {
		dev_err(dev, "%s: get irq_gpio err!\n", __func__);
	} else {
		if (gpio_request(data->irq_gpio, "lt9611-irq") < 0) {
			dev_err(dev, "irq gpio request fail!\n");
			ret = -EINVAL;
			goto irq_pin_err;
		}
		gpio_direction_input(data->irq_gpio);
	}

	return 0;

irq_pin_err:
	if (gpio_is_valid(data->power_gpio))
		gpio_free(data->power_gpio);
pwr_pin_err:
	if (gpio_is_valid(data->reset_gpio))
		gpio_free(data->reset_gpio);
rst_pin_err:

	return ret;
}

static void lt9611_reset(struct i2c_client *client)
{
	struct lt9611_data *data = i2c_get_clientdata(client);

	if (gpio_is_valid(data->power_gpio)) {
		gpio_set_value(data->power_gpio, !data->pwr_active_level);
		mdelay(20);
		gpio_set_value(data->power_gpio, data->pwr_active_level);
		mdelay(20);
	}
	if (gpio_is_valid(data->reset_gpio)) {
		gpio_set_value(data->reset_gpio, !data->rst_active_level);
		mdelay(20);
		gpio_set_value(data->reset_gpio, data->rst_active_level);
		mdelay(20);
		gpio_set_value(data->reset_gpio, !data->rst_active_level);
		mdelay(20);
	}
}

static u8 HDMI_ReadI2C_Byte(struct i2c_client *client, u8 reg)
{
	int ret;
	u8 read_cmd[3] = { 0 };
	u8 cmd_len = 0;
	u8 ret_val = 0;

	read_cmd[0] = reg;
	cmd_len = 1;

	if (client == NULL || client->adapter == NULL) {
		pr_err("HDMI_ReadI2C_Byte client or client->adapter is NULL\n");
		return -1;
	}

	ret = i2c_master_send(client, read_cmd, cmd_len);
	if (ret != cmd_len) {
		pr_err("HDMI_ReadI2C_Byte error1->[REG-0x%02x]\n", reg);
		return -1;
	}

	ret = i2c_master_recv(client, &ret_val, 1);
	if (ret != 1) {
		pr_err("HDMI_ReadI2C_Byte error2->[REG-0x%02x], ret=%d\n", reg, ret);
		return -1;
	}

	return ret_val;

}

static bool HDMI_WriteI2C_Byte(struct i2c_client *client, u8 reg, u8 val)
{
	int ret = 0;
	u8 write_cmd[2] = { 0 };

	write_cmd[0] = reg;
	write_cmd[1] = val;

	if (client == NULL || client->adapter == NULL) {
		pr_err("HDMI_WriteI2C_Byte client or client->adapter is NULL\n");
		return -1;
	}

	ret = i2c_master_send(client, write_cmd, 2);
	if (ret != 2) {
		pr_err("HDMI_WriteI2C_Byte error->[REG-0x%02x,val-0x%02x]\n", reg,
		       val);
		return -1;
	}

	return ret;
}


static u32 LT9611_Chip_ID(struct i2c_client *client)
{
	struct device *dev = &client->dev;
	u8 id0, id1, id2;

	HDMI_WriteI2C_Byte(client, 0xFF,0x80);
	HDMI_WriteI2C_Byte(client, 0xee,0x01);
	id0 = HDMI_ReadI2C_Byte(client, 0x00);
	id1 = HDMI_ReadI2C_Byte(client, 0x01);
	id2  = HDMI_ReadI2C_Byte(client, 0x02);
	dev_info(dev, "LT9611 ring Chip ID = 0x%02x, 0x%02x, 0x%02x \n", id0, id1, id2);

	HDMI_WriteI2C_Byte(client, 0xFF,0x81);
	HDMI_WriteI2C_Byte(client, 0x01,0x18); //sel xtal clock
	HDMI_WriteI2C_Byte(client, 0xFF,0x80);

	return (id2 << 16) | (id1 << 8) | id0;
}

static void LT9611_RST_PD_Init(struct i2c_client *client)
{
	/* power consumption for standby */
	HDMI_WriteI2C_Byte(client, 0xFF,0x81);
	HDMI_WriteI2C_Byte(client, 0x02,0x48);
	HDMI_WriteI2C_Byte(client, 0x23,0x80);
	HDMI_WriteI2C_Byte(client, 0x30,0x00);
	HDMI_WriteI2C_Byte(client, 0x01,0x00); /* i2c stop work */
}

static void LT9611_LowPower_mode(struct i2c_client *client, bool on)
{
	struct device *dev = &client->dev;
	/* only hpd irq is working for low power consumption */
	/* 1.8V: 15 mA */
	if(on)
	{
		HDMI_WriteI2C_Byte(client, 0xFF,0x81);
		HDMI_WriteI2C_Byte(client, 0x02,0x49);
		HDMI_WriteI2C_Byte(client, 0x23,0x80);
		HDMI_WriteI2C_Byte(client, 0x30,0x00); //0x00 --> 0xc0, tx phy and clk can not power down, otherwise dc det don't work.

		HDMI_WriteI2C_Byte(client, 0xff,0x80);
		HDMI_WriteI2C_Byte(client, 0x11,0x0a);
		dev_info(dev, "LT9611_LowPower_mode: enter low power mode\n");
	}
	else
	{
		HDMI_WriteI2C_Byte(client, 0xFF,0x81);
		HDMI_WriteI2C_Byte(client, 0x02,0x12);
		HDMI_WriteI2C_Byte(client, 0x23,0x40);
		HDMI_WriteI2C_Byte(client, 0x30,0xea);

		HDMI_WriteI2C_Byte(client, 0xff,0x80);
		HDMI_WriteI2C_Byte(client, 0x11,0xfa);
		dev_info(dev, "LT9611_LowPower_mode: exit low power mode\n");
	}
}

static void LT9611_System_Init(struct i2c_client *client)  //dsren
{
	HDMI_WriteI2C_Byte(client, 0xFF,0x82);
	HDMI_WriteI2C_Byte(client, 0x51,0x11);
	//Timer for Frequency meter
	HDMI_WriteI2C_Byte(client, 0xFF,0x82);
	HDMI_WriteI2C_Byte(client, 0x1b,0x69); //Timer 2
	HDMI_WriteI2C_Byte(client, 0x1c,0x78);
	HDMI_WriteI2C_Byte(client, 0xcb,0x69); //Timer 1
	HDMI_WriteI2C_Byte(client, 0xcc,0x78);

	/*power consumption for work*/
	HDMI_WriteI2C_Byte(client, 0xff,0x80); 
	HDMI_WriteI2C_Byte(client, 0x04,0xf0);
	HDMI_WriteI2C_Byte(client, 0x06,0xf0);
	HDMI_WriteI2C_Byte(client, 0x0a,0x80);
	HDMI_WriteI2C_Byte(client, 0x0b,0x46); //csc clk
	HDMI_WriteI2C_Byte(client, 0x0d,0xef);
	HDMI_WriteI2C_Byte(client, 0x11,0xfa);
}

static void LT9611_MIPI_Input_Analog(struct i2c_client *client)//xuxi
{
	//mipi mode
	HDMI_WriteI2C_Byte(client, 0xff,0x81);
	HDMI_WriteI2C_Byte(client, 0x06,0x60); //port A rx current
	HDMI_WriteI2C_Byte(client, 0x07,0x3f); //eq
	HDMI_WriteI2C_Byte(client, 0x08,0x3f); //eq

	HDMI_WriteI2C_Byte(client, 0x0a,0xfe); //port A ldo voltage set
	HDMI_WriteI2C_Byte(client, 0x0b,0xbf); //enable port A lprx

	HDMI_WriteI2C_Byte(client, 0x11,0x60); //port B rx current
	HDMI_WriteI2C_Byte(client, 0x12,0x3f); //eq
	HDMI_WriteI2C_Byte(client, 0x13,0x3f); //eq
	HDMI_WriteI2C_Byte(client, 0x15,0xfe); //port B ldo voltage set
	HDMI_WriteI2C_Byte(client, 0x16,0xbf); //enable port B lprx

	HDMI_WriteI2C_Byte(client, 0x1c,0x03); //PortA clk lane no-LP mode.
	HDMI_WriteI2C_Byte(client, 0x20,0x03); //PortB clk lane no-LP mode.
}

static void LT9611_MIPI_Input_Digtal(struct i2c_client *client) //weiguo
{
	struct device *dev = &client->dev;
	u8 lanes = lt9611.mipi_lane_cnt;

	dev_info(dev, "LT9611_MIPI_Input_Digtal: lt9611 set mipi lanes = %d\n", lanes);

	HDMI_WriteI2C_Byte(client, 0xff,0x82); 
	HDMI_WriteI2C_Byte(client, 0x4f,0x80);    //[7] = Select ad_txpll_d_clk.
	HDMI_WriteI2C_Byte(client, 0x50,0x10);
	
	HDMI_WriteI2C_Byte(client, 0xff,0x83); 
	HDMI_WriteI2C_Byte(client, 0x00,lanes); 
	HDMI_WriteI2C_Byte(client, 0x02,0x0a); //settle
	HDMI_WriteI2C_Byte(client, 0x06,0x0a); //settle

	if( (lt9611.mipi_port_cnt == single_port_mipi_A)
		|| (lt9611.mipi_port_cnt == single_port_mipi_B) )//(P10) //single_port_mipi
	{
		 HDMI_WriteI2C_Byte(client, 0x0a,0x00); //1=dual_lr, 0=dual_en
		 dev_info(dev, "LT9611_MIPI_Input_Digtal: lt9611 set mipi ports = 1\n");
  	}
	else    //dual_port_mipi
	{
		HDMI_WriteI2C_Byte(client, 0x0a,0x03); //1=dual_lr, 0=dual_en
		dev_info(dev, "LT9611_MIPI_Input_Digtal: lt9611 set mipi port = 2\n");
	}

#if 1
	if(lt9611.mipi_mode == csi)
	{
		dev_info(dev, "LT9611_MIPI_Input_Digtal: LT9611.mipi_mode = csi\n");

		HDMI_WriteI2C_Byte(client, 0xff,0x83); 
		HDMI_WriteI2C_Byte(client, 0x08,0x10); //csi_en
		HDMI_WriteI2C_Byte(client, 0x2c,0x40); //csi_sel

		if(lt9611.input_color_space == RGB888)
		{
			HDMI_WriteI2C_Byte(client, 0xff,0x83);
			HDMI_WriteI2C_Byte(client, 0x1c,0x01); 
		}    
	}
	else {
		dev_info(dev, "LT9611_MIPI_Input_Digtal: LT9611.mipi_mode = dsi\n");

		if(lt9611.input_color_space == RGB888)
		{
			HDMI_WriteI2C_Byte(client, 0xff,0x83);
			HDMI_WriteI2C_Byte(client, 0x1c,0x04);	//RGB_SWAP[2:0] = RBG
		}
	}
#endif

	if(lt9611.mipi_port_cnt == single_port_mipi_B)
	{
		HDMI_WriteI2C_Byte(client,  0xff, 0x82);
		HDMI_WriteI2C_Byte(client,  0x50, 0x14);   //signal port from portB

		HDMI_WriteI2C_Byte(client,  0xff, 0x83);
		HDMI_WriteI2C_Byte(client,  0x03, 0x40);     //signal port from portB
	}
}

static void LT9611_Video_Check(struct i2c_client *client) //dsren
{
	struct device *dev = &client->dev;
#if 1
	u8 mipi_video_format=0x00;
	u16 h_act, h_act_a ,h_act_b, v_act,v_tal;
	u16 h_total_sysclk;

	HDMI_WriteI2C_Byte(client, 0xff,0x82); // top video check module
	h_total_sysclk = HDMI_ReadI2C_Byte(client, 0x86);
	h_total_sysclk = (h_total_sysclk << 8) + HDMI_ReadI2C_Byte(client, 0x87);
	dev_info(dev, "-----------------------------------------------------------------------------\n");
	dev_info(dev, "LT9611_Video_Check: h_total_sysclk = %d\n",h_total_sysclk);

	v_act=HDMI_ReadI2C_Byte(client, 0x82);
	v_act=(v_act << 8) + HDMI_ReadI2C_Byte(client, 0x83);
	v_tal=HDMI_ReadI2C_Byte(client, 0x6c);
	v_tal=(v_tal << 8) + HDMI_ReadI2C_Byte(client, 0x6d);
	
	HDMI_WriteI2C_Byte(client, 0xff,0x83);
	h_act_a = HDMI_ReadI2C_Byte(client, 0x82);
	h_act_a = (h_act_a << 8) + HDMI_ReadI2C_Byte(client, 0x83);
	
	h_act_b = HDMI_ReadI2C_Byte(client, 0x86);
	h_act_b =(h_act_b << 8) + HDMI_ReadI2C_Byte(client, 0x87);


	if(lt9611.input_color_space==YUV422)
	{
		dev_info(dev, "LT9611_Video_Check: lt9611.input_color_space = YUV422\n");
		h_act_a /= 2; 
		h_act_b /= 2;
	}
	else if(lt9611.input_color_space==RGB888)
	{
	  	dev_info(dev, "LT9611_Video_Check: lt9611.input_color_space = RGB888\n");
	   	h_act_a /= 3; 
		h_act_b /= 3;
	}

	mipi_video_format = HDMI_ReadI2C_Byte(client, 0x88);

	dev_info(dev, "LT9611_Video_Check: h_act_a, h_act_b, v_act, v_tal: %d, %d, %d, %d, \n",h_act_a, h_act_b, v_act, v_tal);
	dev_info(dev, "LT9611_Video_Check: mipi_video_format: 0x%02x\n", mipi_video_format);
					 
	if(lt9611.mipi_port_cnt == dual_port_mipi)//if(0)//(P10 == 0) //dual port.
		h_act = h_act_a + h_act_b;
	else
		h_act = h_act_a;


	dev_info(dev, "LT9611_Video_Check: Video_Format = ");
///////////////////////formate detect///////////////////////////////////

//DTV
	if((h_act == video_640x480_60Hz.hact)
		&& (v_act == video_640x480_60Hz.vact))
	{
		dev_info(dev, " video_640x480_60Hz \n");
		Video_Format = video_640x480_60Hz_vic1;
		video = &video_640x480_60Hz;
	}
	else if((h_act == (video_720x480_60Hz.hact))
		&& (v_act == video_720x480_60Hz.vact))
	{
		dev_info(dev, " video_720x480_60Hz \n");
		Video_Format=video_720x480_60Hz_vic3;
		video = &video_720x480_60Hz;
	}

	else if((h_act == (video_720x576_50Hz.hact))
		&&(v_act == video_720x576_50Hz.vact))
	{
		dev_info(dev, " video_720x576_50Hz \n");
		Video_Format=video_720x576_50Hz_vic;
		video = &video_720x576_50Hz;
	}

	else if((h_act == video_1280x720_60Hz.hact)
		&& (v_act == video_1280x720_60Hz.vact))
	{
		if(h_total_sysclk < 630)
		{
			dev_info(dev, " video_1280x720_60Hz \n");
			Video_Format = video_1280x720_60Hz_vic4;
			video = &video_1280x720_60Hz;
		}
		else if(h_total_sysclk < 750)
		{
			dev_info(dev, " video_1280x720_50Hz \n");
			Video_Format = video_1280x720_50Hz_vic;
			video = &video_1280x720_50Hz;
		}
		else if(h_total_sysclk < 1230)
		{
			dev_info(dev, " video_1280x720_30Hz \n");
			Video_Format = video_1280x720_30Hz_vic;
			video = &video_1280x720_30Hz;
		}
	}

	else if((h_act == video_1920x1080_60Hz.hact)
		&& (v_act == video_1920x1080_60Hz.vact))//1080P
	{
		if(h_total_sysclk < 430)
		{
			dev_info(dev, " video_1920x1080_60Hz \n");
			Video_Format = video_1920x1080_60Hz_vic16;
			video = &video_1920x1080_60Hz;
		}

		else if(h_total_sysclk < 510)
		{
			dev_info(dev, " video_1920x1080_50Hz \n");
			Video_Format = video_1920x1080_50Hz_vic;
			video = &video_1920x1080_50Hz;
		}

		else if(h_total_sysclk < 830)
		{
			dev_info(dev, " video_1920x1080_30Hz \n");
			Video_Format=video_1920x1080_30Hz_vic;
			video = &video_1920x1080_30Hz;
		}

		else if(h_total_sysclk < 980)
		{
			dev_info(dev, " video_1920x1080_25Hz \n");
			Video_Format=video_1920x1080_25Hz_vic;
			video = &video_1920x1080_25Hz;
		}

		else if(h_total_sysclk < 1030)
		{
			dev_info(dev, " video_1920x1080_24Hz \n");
			Video_Format=video_1920x1080_24Hz_vic;
			video = &video_1920x1080_24Hz;
		}
	}
	else if((h_act == video_3840x2160_30Hz.hact)
		&& (v_act == video_3840x2160_30Hz.vact)) //2160P
	{
		if(h_total_sysclk < 430)
		{
			dev_info(dev, " video_3840x2160_30Hz \n");
			Video_Format = video_3840x2160_30Hz_vic;
			video = &video_3840x2160_30Hz;
		}
		else if(h_total_sysclk < 490)
		{
			dev_info(dev, " video_3840x2160_25Hz \n");
			Video_Format = video_3840x2160_25Hz_vic;
			video = &video_3840x2160_25Hz;
		}
		else if(h_total_sysclk < 520)
		{
			dev_info(dev, " video_3840x2160_24Hz \n");
			Video_Format = video_3840x2160_24Hz_vic;
			video = &video_3840x2160_24Hz;
		}
	}

//DMT
/*
	else if((h_act==(video_800x600_60Hz.hact))&&(v_act==video_800x600_60Hz.vact))
		{
			dev_info(dev, " video_800x600_60Hz ");
			Video_Format = video_other;
			video = &video_800x600_60Hz;
		}
	else if((h_act==(video_1024x768_60Hz.hact))&&(v_act==video_1024x768_60Hz.vact))
		{
			dev_info(dev, " video_1024x768_60Hz ");
			Video_Format = video_other;
			video = &video_1024x768_60Hz;
		}
	else if((h_act==(video_1280x768_60Hz.hact))&&(v_act==video_1280x768_60Hz.vact))
		{
			dev_info(dev, " video_1280x768_60Hz ");
			Video_Format = video_other;
			video = &video_1280x768_60Hz;
		}

	else if((h_act==(video_800x600_60Hz.hact))&&(v_act==video_800x600_60Hz.vact))
		{
			dev_info(dev, " video_800x600_60Hz ");
			Video_Format = video_other;
			video = &video_800x600_60Hz;
		}
	else if((h_act==(video_1024x768_60Hz.hact))&&(v_act==video_1024x768_60Hz.vact))
		{
			dev_info(dev, " video_1024x768_60Hz ");
			Video_Format = video_other;
			video = &video_1024x768_60Hz;
		}
	else if((h_act==(video_1280x800_60Hz.hact))&&(v_act==video_1280x800_60Hz.vact))
		{
			if(h_total_sysclk < 560)
			{
				dev_info(dev, " video_1280x800_60Hz ");
				Video_Format = video_other;
				video = &video_1280x800_60Hz;
			}
			else
			{
				dev_info(dev, " video_1280x800_30Hz ");
				Video_Format = video_other;
				video = &video_1280x800_30Hz;
			}
		}

	else if((h_act==(video_1280x960_60Hz.hact))&&(v_act==video_1280x960_60Hz.vact))
		{

			if(h_total_sysclk < 460)
			{
				dev_info(dev, " video_1280x960_60Hz ");
				Video_Format = video_other;
				video = &video_1280x960_60Hz;
			}
			else
			{
				dev_info(dev, " video_1280x960_30Hz ");
				Video_Format = video_other;
				video = &video_1280x960_30Hz;
			}
		}

	else if((h_act==(video_1280x1024_60Hz.hact))&&(v_act==video_1280x1024_60Hz.vact))
		{
			dev_info(dev, " video_1280x1024_60Hz ");
			Video_Format = video_other;
			video = &video_1280x1024_60Hz;
		}

	else if((h_act==(video_1600x1200_60Hz.hact))&&(v_act==video_1600x1200_60Hz.vact))
		{
			dev_info(dev, " video_1600x1200_60Hz ");
			Video_Format = video_other;
			video = &video_1600x1200_60Hz;
		}

	else if((h_act==(video_1680x1050_60Hz.hact))&&(v_act==video_1680x1050_60Hz.vact))
		{
			dev_info(dev, " video_1680x1050_60Hz ");
			Video_Format = video_other;
			video = &video_1680x1050_60Hz;
		}
											  

	else if((h_act==video_1366x768_60Hz.hact)&&
		 (v_act==video_1366x768_60Hz.vact))
	  {
			  dev_info(dev, " video_1366x768_60Hz ");
				Video_Format=video_1366x768_60Hz_vic;
				video = &video_1366x768_60Hz;
	  }

	 else if((h_act==video_2560x1440_60Hz.hact)&&
		 (v_act==video_2560x1440_60Hz.vact))
		{
			if(h_total_sysclk < 310)
			{
			dev_info(dev, " video_2560x1440_60Hz ");
			Video_Format = video_2560x1440_60Hz_vic;
			video = &video_2560x1440_60Hz;
			}

			else if(h_total_sysclk < 370)
			{
		   	dev_info(dev, " video_2560x1440_50Hz ");
			Video_Format = video_other;
			video = &video_2560x1440_50Hz;
			}
	    }

	else if((h_act== video_720x1280_60Hz.hact)&&
		 (v_act== video_720x1280_60Hz.vact))
		{
			dev_info(dev, " video_720x1080_60Hz ");
			Video_Format=video_720x1280_60Hz_vic;
			video = &video_720x1280_60Hz;

	  }

	else if((h_act==video_1080x1920_60Hz.hact)&&
		 (v_act==video_1080x1920_60Hz.vact))//&&
		{
				dev_info(dev, " video_1080x1920_60Hz ");
				Video_Format=video_1080x1920_60Hz_vic;
				video = &video_1080x1920_60Hz;
	    }
			
	else if((h_act==video_3840x1080_60Hz.hact)&&
		 (v_act==video_3840x1080_60Hz.vact))//&&
		{
			dev_info(dev, " video_3840x1080_60Hz ");
			Video_Format = video_3840x1080_60Hz_vic;
			video = &video_3840x1080_60Hz;
	  }	
	else if((h_act==video_1024x600_60Hz.hact)&&
		 (v_act==video_1024x600_60Hz.vact))//&&
		{

			dev_info(dev, " video_1024x600_60Hz ");
			Video_Format = video_1024x600_60Hz_vic;
			video = &video_1024x600_60Hz;
	  }	
	else if((h_act==video_1280x800_60Hz.hact)&&
		 (v_act==video_1280x800_60Hz.vact))//&&
		{
			dev_info(dev, " video_1280x800_60Hz ");
			Video_Format = video_1280x800_60Hz_vic;
			video = &video_1280x800_60Hz;
	  }  
//mipi
	else if((h_act==video_540x960_60Hz.hact)&&
		 (v_act==video_540x960_60Hz.vact))//&&
		{
			
			dev_info(dev, " video_540x960_60Hz ");
			Video_Format = video_540x960_60Hz_vic;
			video = &video_540x960_60Hz;
	  }  
	   
	else if((h_act==video_2560x1600_60Hz.hact)&&
		 (v_act==video_2560x1600_60Hz.vact))//&&
		{
			
			dev_info(dev, " video_2560x1600_60Hz ");
			Video_Format = video_2560x1600_60Hz_vic;
			video = &video_2560x1600_60Hz;
	  }  

	else if((h_act==video_2560x1080_60Hz.hact)&&
		 (v_act==video_2560x1080_60Hz.vact))//&&
		{
			dev_info(dev, " video_2560x1080_60Hz ");
			Video_Format = video_2560x1080_60Hz_vic;
			video = &video_2560x1080_60Hz;
	    }  	
		
	else if((h_act==video_2560x720_60Hz.hact)&&
		 (v_act==video_2560x720_60Hz.vact))//&&
		{
			dev_info(dev, " video_2560x720_60Hz ");
			Video_Format = video_2560x720_60Hz_vic;
			video = &video_2560x720_60Hz;
	    } 
*/
	else if((h_act == video_1200x1920_60Hz.hact)
		&& (v_act == video_1200x1920_60Hz.vact))//&&
	{
		dev_info(dev, " video_1200x1920_60Hz \n");
		Video_Format = video_other;
		video = &video_1200x1920_60Hz;
	} 	
	else if((h_act == video_1920x720_60Hz.hact)
		&& (v_act == video_1920x720_60Hz.vact))//&&
	{
		dev_info(dev, " video_1920x720_60Hz \n");
		Video_Format = video_other;
		video = &video_1920x720_60Hz;
	} 	

	else
	{
		Video_Format = video_none;
		video = NULL;
		dev_info(dev, " unknown video format \n");
	}
	dev_info(dev, "-----------------------------------------------------------------------------");	
#endif

}


void LT9611_MIPI_Video_Timing(struct i2c_client *client, struct video_timing *video_format) //weiguo
{
	HDMI_WriteI2C_Byte(client, 0xff, 0x83);
	HDMI_WriteI2C_Byte(client, 0x0d, (u8)(video_format->vtotal / 256));
	HDMI_WriteI2C_Byte(client, 0x0e, (u8)(video_format->vtotal % 256));//vtotal
	HDMI_WriteI2C_Byte(client, 0x0f, (u8)(video_format->vact / 256));
	HDMI_WriteI2C_Byte(client, 0x10, (u8)(video_format->vact % 256));  //vactive
	HDMI_WriteI2C_Byte(client, 0x11, (u8)(video_format->htotal / 256));
	HDMI_WriteI2C_Byte(client, 0x12, (u8)(video_format->htotal % 256));//htotal
	HDMI_WriteI2C_Byte(client, 0x13, (u8)(video_format->hact / 256));
	HDMI_WriteI2C_Byte(client, 0x14, (u8)(video_format->hact % 256)); //hactive
	HDMI_WriteI2C_Byte(client, 0x15, (u8)(video_format->vs % 256));   //vsa
	HDMI_WriteI2C_Byte(client, 0x16, (u8)(video_format->hs % 256));   //hsa
	HDMI_WriteI2C_Byte(client, 0x17, (u8)(video_format->vfp % 256));  //vfp
	HDMI_WriteI2C_Byte(client, 0x18, (u8)((video_format->vs + video_format->vbp) % 256));  //vss
	HDMI_WriteI2C_Byte(client, 0x19, (u8)(video_format->hfp % 256));  //hfp
	HDMI_WriteI2C_Byte(client, 0x1a, (u8)(((video_format->hfp / 256) << 4)+(video_format->hs + video_format->hbp) / 256));   //20180901
	HDMI_WriteI2C_Byte(client, 0x1b, (u8)((video_format->hs + video_format->hbp) % 256));  //hss
}

static void LT9611_MIPI_Pcr(struct i2c_client *client, struct video_timing *video_format) //weiguo
{
	struct device *dev = &client->dev;
	u8 POL;
	u16 hact;

	if (NULL == video_format) {
		dev_err(dev, "%s: unknown video format\n", __func__);
		return;
	}

	hact = video_format->hact;
	POL = (video_format->h_polarity) * 0x02 + (video_format->v_polarity);
	POL = ~POL;
	POL &= 0x03;

	HDMI_WriteI2C_Byte(client, 0xff,0x83); 

	if(lt9611.mipi_port_cnt == dual_port_mipi)// if(P10 == 0) //dual port.
	{
		hact = (hact>>2);
		hact += 0x50;

		hact = (0x3e0>hact ? hact:0x3e0);

		HDMI_WriteI2C_Byte(client, 0x0b,(u8)(hact >> 8)); //vsync mode
		HDMI_WriteI2C_Byte(client, 0x0c,(u8)hact); //=1/4 hact
		//hact -=0x40; 
		HDMI_WriteI2C_Byte(client, 0x48,(u8)(hact >> 8)); //de mode delay
		HDMI_WriteI2C_Byte(client, 0x49,(u8)(hact)); //
	}

	else
	{
		HDMI_WriteI2C_Byte(client, 0x0b,0x01); //vsync read delay(reference value)
		HDMI_WriteI2C_Byte(client, 0x0c,0x10); //

		HDMI_WriteI2C_Byte(client, 0x48,0x00); //de mode delay
		HDMI_WriteI2C_Byte(client, 0x49,0x81); //=1/4 hact
	}

	/* stage 1 */
	HDMI_WriteI2C_Byte(client, 0x21,0x4a); //bit[3:0] step[11:8]
	//HDMI_WriteI2C_Byte(client, 0x22,0x40);//step[7:0]

	HDMI_WriteI2C_Byte(client, 0x24,0x71); //bit[7:4]v/h/de mode; line for clk stb[11:8]
	HDMI_WriteI2C_Byte(client, 0x25,0x30); //line for clk stb[7:0]

	HDMI_WriteI2C_Byte(client, 0x2a,0x01); //clk stable in

	/* stage 2 */
	HDMI_WriteI2C_Byte(client, 0x4a,0x40); //offset //0x10
	HDMI_WriteI2C_Byte(client, 0x1d,(0x10|POL)); //PCR de mode step setting.

	/* MK limit */

	switch(Video_Format)
	{
	case video_3840x1080_60Hz_vic:
	case video_3840x2160_30Hz_vic:
	case video_3840x2160_25Hz_vic:
	case video_3840x2160_24Hz_vic:
	case video_2560x1600_60Hz_vic:
	case video_2560x1440_60Hz_vic:
	case video_2560x1080_60Hz_vic:
	break;
	case video_1920x1080_60Hz_vic16:
	case video_1920x1080_30Hz_vic:		
	case video_1280x720_60Hz_vic4:
	case video_1280x720_30Hz_vic:
	break;
	case video_720x480_60Hz_vic3:
	case video_640x480_60Hz_vic1:

		HDMI_WriteI2C_Byte(client, 0xff,0x83);
		HDMI_WriteI2C_Byte(client, 0x0b,0x02);
		HDMI_WriteI2C_Byte(client, 0x0c,0x40);
		HDMI_WriteI2C_Byte(client, 0x48,0x01);
		HDMI_WriteI2C_Byte(client, 0x49,0x10);
		HDMI_WriteI2C_Byte(client, 0x24,0x70);
		HDMI_WriteI2C_Byte(client, 0x25,0x80);
		HDMI_WriteI2C_Byte(client, 0x2a,0x10);
		HDMI_WriteI2C_Byte(client, 0x2b,0x80);
		HDMI_WriteI2C_Byte(client, 0x23,0x28);  //
		HDMI_WriteI2C_Byte(client, 0x4a,0x10);
		HDMI_WriteI2C_Byte(client, 0x1d,0xf3);  //

		dev_info(dev, "LT9611_MIPI_Pcr: 640x480_60Hz\n");
	break;

	case video_540x960_60Hz_vic:
	case video_1024x600_60Hz_vic:
		HDMI_WriteI2C_Byte(client, 0x24,0x70); //bit[7:4]v/h/de mode; line for clk stb[11:8]
		HDMI_WriteI2C_Byte(client, 0x25,0x80); //line for clk stb[7:0]
		HDMI_WriteI2C_Byte(client, 0x2a,0x10); //clk stable in

		/* stage 2 */
		//HDMI_WriteI2C_Byte(client, 0x23,0x04); //pcr h mode step
		//HDMI_WriteI2C_Byte(client, 0x4a,0x10); //offset //0x10
		HDMI_WriteI2C_Byte(client, 0x1d,0xf0); //PCR de mode step setting.
	break;

	default: break;
	}

	LT9611_MIPI_Video_Timing(client, video);

	HDMI_WriteI2C_Byte(client, 0xff,0x83);
	HDMI_WriteI2C_Byte(client, 0x26,pcr_m);

	HDMI_WriteI2C_Byte(client, 0xff,0x80);
	HDMI_WriteI2C_Byte(client, 0x11,0x5a); //Pcr reset
	HDMI_WriteI2C_Byte(client, 0x11,0xfa);
}

static void LT9611_PLL(struct i2c_client *client, struct video_timing *video_format) //zhangzhichun
{
	struct device *dev = &client->dev;
	u32 pclk;
	u8 pll_lock_flag, cal_done_flag, band_out;
	u8 hdmi_post_div;
	u8 i;

	if (NULL == video_format) {
		dev_err(dev, "%s: unknown video format\n", __func__);
		return;
	}

	pclk = video_format->pclk_khz;
	dev_info(dev, "LT9611_PLL: set rx pll = %d\n", pclk);
	
	HDMI_WriteI2C_Byte(client, 0xff,0x81);
	HDMI_WriteI2C_Byte(client, 0x23,0x40); //Enable LDO and disable PD
	HDMI_WriteI2C_Byte(client, 0x24,0x62); //0x62, LG25UM58 issue, 20180824
	HDMI_WriteI2C_Byte(client, 0x25,0x80); //pre-divider
	HDMI_WriteI2C_Byte(client, 0x26,0x55);
	HDMI_WriteI2C_Byte(client, 0x2c,0x37);
	//HDMI_WriteI2C_Byte(client, 0x2d,0x99); //txpll_divx_set&da_txpll_freq_set
	//HDMI_WriteI2C_Byte(client, 0x2e,0x01);
	HDMI_WriteI2C_Byte(client, 0x2f,0x01);
	//HDMI_WriteI2C_Byte(client, 0x26,0x55);
	HDMI_WriteI2C_Byte(client, 0x27,0x66);
	HDMI_WriteI2C_Byte(client, 0x28,0x88);

	HDMI_WriteI2C_Byte(client, 0x2a,0x20); //for U3
	
	if(pclk > 150000)
	{
		HDMI_WriteI2C_Byte(client, 0x2d,0x88);
		hdmi_post_div = 0x01;
	}
	else if(pclk > 80000)
	{
		HDMI_WriteI2C_Byte(client, 0x2d,0x99);
		hdmi_post_div = 0x02;
	}
	else
	{
		HDMI_WriteI2C_Byte(client, 0x2d,0xaa); //0xaa
		hdmi_post_div = 0x04;
	}

	pcr_m = (u8)((pclk*5*hdmi_post_div) / 27000);
	pcr_m --;
	dev_info(dev, "LT9611_PLL: pcr_m = 0x%02x, hdmi_post_div = %d\n", pcr_m, hdmi_post_div); //Hex
		
	HDMI_WriteI2C_Byte(client, 0xff,0x83);
	HDMI_WriteI2C_Byte(client, 0x2d,0x40); //M up limit
	HDMI_WriteI2C_Byte(client, 0x31,0x08); //M down limit
	HDMI_WriteI2C_Byte(client, 0x26,0x80 | pcr_m); /* fixed M is to let pll locked*/

	pclk = pclk / 2;
	HDMI_WriteI2C_Byte(client, 0xff,0x82);     //13.5M
	HDMI_WriteI2C_Byte(client, 0xe3, pclk / 65536);
	pclk = pclk % 65536;
	HDMI_WriteI2C_Byte(client, 0xe4, pclk / 256);
	HDMI_WriteI2C_Byte(client, 0xe5, pclk % 256);

	HDMI_WriteI2C_Byte(client, 0xde,0x20); // pll cal en, start calibration
	HDMI_WriteI2C_Byte(client, 0xde,0xe0);

	HDMI_WriteI2C_Byte(client, 0xff,0x80);
	HDMI_WriteI2C_Byte(client, 0x11,0x5a); /* Pcr clk reset */
	HDMI_WriteI2C_Byte(client, 0x11,0xfa);
	HDMI_WriteI2C_Byte(client, 0x16,0xf2); /* pll cal digital reset */ 
	HDMI_WriteI2C_Byte(client, 0x18,0xdc); /* pll analog reset */
	HDMI_WriteI2C_Byte(client, 0x18,0xfc);
	HDMI_WriteI2C_Byte(client, 0x16,0xf3); /*start calibration*/ 
   
	/* pll lock status */
	for(i = 0; i < 6 ; i++)
	{   
		HDMI_WriteI2C_Byte(client, 0xff,0x80);	
		HDMI_WriteI2C_Byte(client, 0x16,0xe3); /* pll lock logic reset */
		HDMI_WriteI2C_Byte(client, 0x16,0xf3);
		HDMI_WriteI2C_Byte(client, 0xff,0x82);
		cal_done_flag = HDMI_ReadI2C_Byte(client, 0xe7);
		band_out = HDMI_ReadI2C_Byte(client, 0xe6);
		pll_lock_flag = HDMI_ReadI2C_Byte(client, 0x15);

		if((pll_lock_flag & 0x80) && (cal_done_flag & 0x80) && (band_out != 0xff))
		{
			dev_info(dev, "LT9611_PLL: HDMI pll lockedband out: 0x%02x\n", band_out);
			break;
		}
		else
		{
			HDMI_WriteI2C_Byte(client, 0xff,0x80);
			HDMI_WriteI2C_Byte(client, 0x11,0x5a); /* Pcr clk reset */
			HDMI_WriteI2C_Byte(client, 0x11,0xfa);
			HDMI_WriteI2C_Byte(client, 0x16,0xf2); /* pll cal digital reset */ 
			HDMI_WriteI2C_Byte(client, 0x18,0xdc); /* pll analog reset */
			HDMI_WriteI2C_Byte(client, 0x18,0xfc);
			HDMI_WriteI2C_Byte(client, 0x16,0xf3); /*start calibration*/ 
			dev_info(dev, "LT9611_PLL: HDMI pll unlocked, reset pll\n");
		}
	}
}

static void LT9611_HDMI_TX_Phy(struct i2c_client *client) //xyji
{
	struct device *dev = &client->dev;

	HDMI_WriteI2C_Byte(client, 0xff,0x81);
	HDMI_WriteI2C_Byte(client, 0x30,0x6a);

	if(lt9611.hdmi_coupling_mode==ac_mode)
	{
		HDMI_WriteI2C_Byte(client, 0x31,0x73); //DC: 0x44, AC:0x73
		dev_info(dev, "LT9611_HDMI_TX_Phy: AC couple\n");
  	}
	else //lt9611.hdmi_coupling_mode==dc_mode
	{
		HDMI_WriteI2C_Byte(client, 0x31,0x44);
		dev_info(dev, "LT9611_HDMI_TX_Phy: DC couple\n");
	}

	HDMI_WriteI2C_Byte(client, 0x32,0x4a);
	HDMI_WriteI2C_Byte(client, 0x33,0x0b);
	HDMI_WriteI2C_Byte(client, 0x34,0x00);
	HDMI_WriteI2C_Byte(client, 0x35,0x00);
	HDMI_WriteI2C_Byte(client, 0x36,0x00);
	HDMI_WriteI2C_Byte(client, 0x37,0x44);
	HDMI_WriteI2C_Byte(client, 0x3f,0x0f);
	HDMI_WriteI2C_Byte(client, 0x40,0x98); //clk swing
	HDMI_WriteI2C_Byte(client, 0x41,0x98); //D0 swing
	HDMI_WriteI2C_Byte(client, 0x42,0x98); //D1 swing
	HDMI_WriteI2C_Byte(client, 0x43,0x98); //D2 swing
	HDMI_WriteI2C_Byte(client, 0x44,0x0a);
}

#if 0
void LT9611_HDMI_TX_Phy(void) //xyji
{
  // AC mode with VL pass
	HDMI_WriteI2C_Byte(client, 0xff,0x81);
	HDMI_WriteI2C_Byte(client, 0x30,0xff);
	HDMI_WriteI2C_Byte(client, 0x31,0x73); //DC: 0x44, AC:0x73 
	HDMI_WriteI2C_Byte(client, 0x32,0xfe);
	HDMI_WriteI2C_Byte(client, 0x33,0x73);
	HDMI_WriteI2C_Byte(client, 0x34,0x03);
	HDMI_WriteI2C_Byte(client, 0x35,0x03);
	HDMI_WriteI2C_Byte(client, 0x36,0x03);
	HDMI_WriteI2C_Byte(client, 0x37,0x44);
	HDMI_WriteI2C_Byte(client, 0x3f,0x0f);
	HDMI_WriteI2C_Byte(client, 0x40,0xa0); //clk swing
	HDMI_WriteI2C_Byte(client, 0x41,0xa0); //D0 swing
	HDMI_WriteI2C_Byte(client, 0x42,0xa0); //D1 swing
	HDMI_WriteI2C_Byte(client, 0x43,0xa0); //D2 swing
	HDMI_WriteI2C_Byte(client, 0x44,0x0a);
}
#endif


static void LT9611_HDCP_Init(struct i2c_client *client) //luodexing
{
	HDMI_WriteI2C_Byte(client, 0xff,0x85); 
	HDMI_WriteI2C_Byte(client, 0x07,0x1f);
	HDMI_WriteI2C_Byte(client, 0x13,0xfe);// [7]=force_hpd, [6]=force_rsen, [5]=vsync_pol, [4]=hsync_pol,
	                              // [3]=hdmi_mode, [2]=no_accs_when_rdy, [1]=skip_wt_hdmi
	HDMI_WriteI2C_Byte(client, 0x17,0x0f);// [7]=ri_short_read, [3]=sync_pol_mode, [2]=srm_chk_done, 
	                              // [1]=bksv_srm_pass, [0]=ksv_list_vld
	HDMI_WriteI2C_Byte(client, 0x15,0x05);
	//HDMI_WriteI2C_Byte(client, 0x15,0x65);// [7]=key_ddc_st_sel, [6]=tx_hdcp_en,[5]=tx_auth_en, [4]=tx_re_auth
}

static void LT9611_HDCP_Enable(struct i2c_client *client) //luodexing
{
	struct device *dev = &client->dev;

	HDMI_WriteI2C_Byte(client, 0xff,0x80); 
	HDMI_WriteI2C_Byte(client, 0x14,0x7f);
	HDMI_WriteI2C_Byte(client, 0x14,0xff); 	
	HDMI_WriteI2C_Byte(client, 0xff,0x85); 
	HDMI_WriteI2C_Byte(client, 0x15,0x01); //disable HDCP
	HDMI_WriteI2C_Byte(client, 0x15,0x71); //enable HDCP
	HDMI_WriteI2C_Byte(client, 0x15,0x65); //enable HDCP
	dev_info(dev, "LT9611_HDCP_Enable: On\n");
}

static void LT9611_HDCP_Disable(struct i2c_client *client) //luodexing
{
	HDMI_WriteI2C_Byte(client, 0xff,0x85); 
	HDMI_WriteI2C_Byte(client, 0x15,0x45); //enable HDCP
}

static void LT9611_HDMI_Out_Enable(struct i2c_client *client) //dsren
{
	struct device *dev = &client->dev;

	HDMI_WriteI2C_Byte(client, 0xff,0x81);
	HDMI_WriteI2C_Byte(client, 0x23,0x40);

	HDMI_WriteI2C_Byte(client, 0xff,0x82);
	HDMI_WriteI2C_Byte(client, 0xde,0x20);
	HDMI_WriteI2C_Byte(client, 0xde,0xe0);

	HDMI_WriteI2C_Byte(client, 0xff,0x80); 
	HDMI_WriteI2C_Byte(client, 0x18,0xdc); /* txpll sw rst */
	HDMI_WriteI2C_Byte(client, 0x18,0xfc);
	HDMI_WriteI2C_Byte(client, 0x16,0xf1); /* txpll calibration rest */ 
	HDMI_WriteI2C_Byte(client, 0x16,0xf3);

	HDMI_WriteI2C_Byte(client, 0x11,0x5a); //Pcr reset
	HDMI_WriteI2C_Byte(client, 0x11,0xfa);

	HDMI_WriteI2C_Byte(client, 0xff,0x81);
	HDMI_WriteI2C_Byte(client, 0x30,0xea);

	if(lt9611.hdcp_encryption == hdcp_enable)
	{
		LT9611_HDCP_Enable(client);
	}
	else
	{
		LT9611_HDCP_Disable(client);
	}

	dev_info(dev, "LT9611_HDMI_Out_Enable\n");
}

static void LT9611_HDMI_Out_Disable(struct i2c_client *client) //dsren
{
	struct device *dev = &client->dev;

	HDMI_WriteI2C_Byte(client, 0xff,0x81);
	HDMI_WriteI2C_Byte(client, 0x30,0x00); /* Txphy PD */
	HDMI_WriteI2C_Byte(client, 0x23,0x80); /* Txpll PD */ 
	dev_info(dev, "LT9611_HDMI_Out_Disable\n");
	LT9611_HDCP_Disable(client);
}

static void LT9611_HDMI_TX_Digital(struct i2c_client *client, struct video_timing *video_format) //dsren
{
	struct device *dev = &client->dev;

	if (NULL == video_format) {
		dev_err(dev, "%s: unknown video format\n", __func__);
		return;
	}

	//bool hdmi_mode = lt9611->hdmi_mode;
	u8 VIC = video_format->vic;
	u8 AR = video_format->aspact_ratio;
	u8 pb0,pb2,pb4;
	u8 infoFrame_en;

	infoFrame_en = (AIF_PKT_EN|AVI_PKT_EN|SPD_PKT_EN);
	//MPEG_PKT_EN,AIF_PKT_EN,SPD_PKT_EN,AVI_PKT_EN,UD0_PKT_EN,UD1_PKT_EN
	dev_info(dev, "LT9611_HDMI_TX_Digital: infoFrame_en = 0x%02x\n",infoFrame_en);

	pb2 =  (AR<<4) + 0x08;
	pb4 =  VIC;

	pb0 = ((pb2 + pb4) <= 0x5f)?(0x5f - pb2 - pb4):(0x15f - pb2 - pb4);

	HDMI_WriteI2C_Byte(client, 0xff,0x82);
	if (lt9611.hdmi_mode == HDMI)
	{
		HDMI_WriteI2C_Byte(client, 0xd6,0x8e); //sync polarity
		dev_info(dev, "LT9611_HDMI_TX_Digital: HMDI mode = HDMI\n");
	}
	else if(lt9611.hdmi_mode == DVI)
	{
		HDMI_WriteI2C_Byte(client, 0xd6,0x0e); //sync polarity
		dev_info(dev, "LT9611_HDMI_TX_Digital: HMDI mode = DVI\n");
	}

	if (lt9611.audio_out == audio_i2s)
	{
		HDMI_WriteI2C_Byte(client, 0xd7,0x04);
	}

	if (lt9611.audio_out == audio_spdif)
	{
		HDMI_WriteI2C_Byte(client, 0xd7,0x80); 
	}

	//AVI
	HDMI_WriteI2C_Byte(client, 0xff,0x84);
	HDMI_WriteI2C_Byte(client, 0x43,pb0);   //AVI_PB0

	//HDMI_WriteI2C_Byte(client, 0x44,0x10);//AVI_PB1
	HDMI_WriteI2C_Byte(client, 0x45,pb2);  //AVI_PB2
	HDMI_WriteI2C_Byte(client, 0x47,pb4);   //AVI_PB4

	HDMI_WriteI2C_Byte(client, 0xff,0x84);
	HDMI_WriteI2C_Byte(client, 0x10,0x02); //data iland
	HDMI_WriteI2C_Byte(client, 0x12,0x64); //act_h_blank

	//VS_IF, 4k 30hz need send VS_IF packet.
	if (VIC == 95)
	{
		HDMI_WriteI2C_Byte(client, 0xff,0x84);
		HDMI_WriteI2C_Byte(client, 0x3d,infoFrame_en|UD0_PKT_EN); //UD1 infoframe enable //revise on 20200715

		HDMI_WriteI2C_Byte(client, 0x74,0x81);  //HB0
		HDMI_WriteI2C_Byte(client, 0x75,0x01);  //HB1
		HDMI_WriteI2C_Byte(client, 0x76,0x05);  //HB2
		HDMI_WriteI2C_Byte(client, 0x77,0x49);  //PB0
		HDMI_WriteI2C_Byte(client, 0x78,0x03);  //PB1
		HDMI_WriteI2C_Byte(client, 0x79,0x0c);  //PB2
		HDMI_WriteI2C_Byte(client, 0x7a,0x00);  //PB3
		HDMI_WriteI2C_Byte(client, 0x7b,0x20);  //PB4
		HDMI_WriteI2C_Byte(client, 0x7c,0x01);  //PB5
	}
	else
	{
		HDMI_WriteI2C_Byte(client, 0xff,0x84);
		HDMI_WriteI2C_Byte(client, 0x3d,infoFrame_en); //UD1 infoframe enable
	}

	if (infoFrame_en && SPD_PKT_EN)
	{
		HDMI_WriteI2C_Byte(client, 0xff,0x84);
		HDMI_WriteI2C_Byte(client, 0xc0,0x83);  //HB0
		HDMI_WriteI2C_Byte(client, 0xc1,0x01);  //HB1
		HDMI_WriteI2C_Byte(client, 0xc2,0x19);  //HB2

		HDMI_WriteI2C_Byte(client, 0xc3,0x00);  //PB0
		HDMI_WriteI2C_Byte(client, 0xc4,0x01);  //PB1
		HDMI_WriteI2C_Byte(client, 0xc5,0x02);  //PB2
		HDMI_WriteI2C_Byte(client, 0xc6,0x03);  //PB3
		HDMI_WriteI2C_Byte(client, 0xc7,0x04);  //PB4
		HDMI_WriteI2C_Byte(client, 0xc8,0x00);  //PB5
	}				
}

static void LT9611_CSC(struct i2c_client *client) 
{
	struct device *dev = &client->dev;

   	if(lt9611.input_color_space == YUV422)
	{
		HDMI_WriteI2C_Byte(client, 0xff,0x82);
		HDMI_WriteI2C_Byte(client, 0xb9,0x18); 
		dev_info(dev, "LT9611_CSC: Ypbpr 422 to RGB888\n");
	}
}


static void LT9611_Audio_Init(struct i2c_client *client) //sujin
{
	struct device *dev = &client->dev;

#if 1
	if(lt9611.audio_out == audio_i2s)
	{
		dev_info(dev, "Audio inut = I2S 2ch\n");

		HDMI_WriteI2C_Byte(client, 0xff,0x84);
		HDMI_WriteI2C_Byte(client, 0x06,0x08);
		HDMI_WriteI2C_Byte(client, 0x07,0x10);

		//48K sampling frequency	
		HDMI_WriteI2C_Byte(client, 0x0f,0x2b); //0x2b: 48K, 0xab:96K
		HDMI_WriteI2C_Byte(client, 0x34,0xd4); //CTS_N 20180823 0xd5: sclk = 32fs, 0xd4: sclk = 64fs blck/FS

		HDMI_WriteI2C_Byte(client, 0x35,0x00); // N value = 6144
		HDMI_WriteI2C_Byte(client, 0x36,0x18); 
		HDMI_WriteI2C_Byte(client, 0x37,0x00); 

		//96K sampling frequency	
		//HDMI_WriteI2C_Byte(client, 0x0f,0xab); //0x2b: 48K, 0xab:96K
		//HDMI_WriteI2C_Byte(client, 0x34,0xd4); //CTS_N 20180823 0xd5: sclk = 32fs, 0xd4: sclk = 64fs

		//HDMI_WriteI2C_Byte(client, 0x35,0x00); // N value = 12288
		//HDMI_WriteI2C_Byte(client, 0x36,0x30);
		//HDMI_WriteI2C_Byte(client, 0x37,0x00); 


		//44.1K sampling frequency
		/*
		HDMI_WriteI2C_Byte(client, 0x0f,0x0b); //0x2b: 48K, 0xab:96K
		HDMI_WriteI2C_Byte(client, 0x34,0xd4); //CTS_N 20180823 0xd5: sclk = 32fs, 0xd4: sclk = 64fs

		HDMI_WriteI2C_Byte(client, 0x35,0x00); // N value = 6272
		HDMI_WriteI2C_Byte(client, 0x36,0x18); 
		HDMI_WriteI2C_Byte(client, 0x37,0x80); 
		*/
	}

	if(lt9611.audio_out == audio_spdif)
	{
		dev_info(dev, "Audio inut = SPDIF\n");

		HDMI_WriteI2C_Byte(client, 0xff,0x84);
		HDMI_WriteI2C_Byte(client, 0x06,0x0c);
		HDMI_WriteI2C_Byte(client, 0x07,0x10);	

		HDMI_WriteI2C_Byte(client, 0x34,0xd4); //CTS_N
	}
#endif
}


static void LT9611_Read_EDID(struct i2c_client *client) //luodexing
{
	struct device *dev = &client->dev;

#ifdef _enable_read_edid_

	u8 i,j, edid_data;
	u8 extended_flag = 00;

	memset(Sink_EDID,0,sizeof Sink_EDID);

	HDMI_WriteI2C_Byte(client, 0xff,0x85);
	//HDMI_WriteI2C_Byte(client, 0x02,0x0a); //I2C 100K
	HDMI_WriteI2C_Byte(client, 0x03,0xc9);
	HDMI_WriteI2C_Byte(client, 0x04,0xa0); //0xA0 is EDID device address
	HDMI_WriteI2C_Byte(client, 0x05,0x00); //0x00 is EDID offset address
	HDMI_WriteI2C_Byte(client, 0x06,0x20); //length for read
	HDMI_WriteI2C_Byte(client, 0x14,0x7f);

	for(i=0;i < 8;i++) // block 0 & 1
	{
		HDMI_WriteI2C_Byte(client, 0x05,i*32); //0x00 is EDID offset address
		HDMI_WriteI2C_Byte(client, 0x07,0x36);
		HDMI_WriteI2C_Byte(client, 0x07,0x34); //0x31
		HDMI_WriteI2C_Byte(client, 0x07,0x37); //0x37
		mdelay(5); // wait 5ms for reading edid data.
		if(HDMI_ReadI2C_Byte(client, 0x40) & 0x02) //KEY_DDC_ACCS_DONE=1
		{
			if(HDMI_ReadI2C_Byte(client, 0x40)&0x50)//DDC No Ack or Abitration lost
			{
				dev_info(dev, "read edid failed: no ack\n");
				goto end;
			}
			else
			{
				printk("HDMI read EDID block%d: \n", i);
				for(j=0; j<32; j++)
				{
					edid_data = HDMI_ReadI2C_Byte(client, 0x83);
					Sink_EDID[i*32+j]= edid_data; // write edid data to Sink_EDID[];
					if((i == 3)&&( j == 30))
					{
						extended_flag = edid_data & 0x03;
					}
					printk("%02x,",edid_data);
				}
				printk("\n");

				if(i == 3)
				{
					if(extended_flag < 1) //no block 1, stop reading edid. 
					{
						goto end; 
					}
				}
			}
		}
		else
		{
			dev_info(dev, "read edid failed: accs not done\n");
			goto end;
		}
	}

	if(extended_flag < 2) //no block 2, stop reading edid. 
	{
		goto end; 
	}

	for(i=0;i< 8;i++) //  // block 2 & 3
	{	
		HDMI_WriteI2C_Byte(client, 0x05,i*32); //0x00 is EDID offset address
		HDMI_WriteI2C_Byte(client, 0x07,0x76); //0x31
		HDMI_WriteI2C_Byte(client, 0x07,0x74); //0x31
		HDMI_WriteI2C_Byte(client, 0x07,0x77); //0x37
		mdelay(5); // wait 5ms for reading edid data.
		if(HDMI_ReadI2C_Byte(client, 0x40)&0x02) //KEY_DDC_ACCS_DONE=1
		{
			if(HDMI_ReadI2C_Byte(client, 0x40)&0x50)//DDC No Ack or Abitration lost
			{
				dev_info(dev, "read edid failed: no ack\n");
				goto end;
			}
			else
			{
				printk("HDMI read EDID 1 block%d: \n", i);
				for(j=0; j<32; j++)
				{
					edid_data = HDMI_ReadI2C_Byte(client, 0x83);
					//Sink_EDID[256+i*32+j]= edid_data; // write edid data to Sink_EDID[];
					printk("%02x,",edid_data);
				}
				printk("\n");

				if(i == 3)
				{
					if(extended_flag < 3) //no block 1, stop reading edid. 
					{
						goto end; 
					}
				}
			}
		}
		else
		{
			dev_info(dev, "read edid failed: accs not done\n");
			goto end;
		}
	}
end:
	HDMI_WriteI2C_Byte(client, 0x03,0xc2);
	HDMI_WriteI2C_Byte(client, 0x07,0x1f);
#endif
}

static void LT9611_load_hdcp_key(struct i2c_client *client) //luodexing
{
	struct device *dev = &client->dev;

	HDMI_WriteI2C_Byte(client, 0xff,0x85);
	HDMI_WriteI2C_Byte(client, 0x00,0x85);
	//HDMI_WriteI2C_Byte(client, 0x02,0x0a); //I2C 100K
	HDMI_WriteI2C_Byte(client, 0x03,0xc0);
	HDMI_WriteI2C_Byte(client, 0x03,0xc3);
	HDMI_WriteI2C_Byte(client, 0x04,0xa0); //0xA0 is eeprom device address
	HDMI_WriteI2C_Byte(client, 0x05,0x00); //0x00 is eeprom offset address
	HDMI_WriteI2C_Byte(client, 0x06,0x20); //length for read
	HDMI_WriteI2C_Byte(client, 0x14,0xff);

	HDMI_WriteI2C_Byte(client, 0x07,0x11); //0x31
	HDMI_WriteI2C_Byte(client, 0x07,0x17); //0x37
	mdelay(50); // wait 5ms for loading key.

	//dev_info(dev, "LT9611_load_hdcp_key: 0x%02bx",HDMI_ReadI2C_Byte(client, 0x40));

	if((HDMI_ReadI2C_Byte(client, 0x40)&0x81) == 0x81)
	{
		dev_info(dev, "LT9611_load_hdcp_key: external key valid\n");
	}
	else
	{
		dev_info(dev, "LT9611_load_hdcp_key: external key unvalid, using internal test key!\n");
	}
            
	HDMI_WriteI2C_Byte(client, 0x03,0xc2);
	HDMI_WriteI2C_Byte(client, 0x07,0x1f);      
}

static bool LT9611_Get_HPD_Status(struct i2c_client *client)
{
	u8 reg_825e;
	HDMI_WriteI2C_Byte(client, 0xff,0x82);
	reg_825e = HDMI_ReadI2C_Byte(client, 0x5e);

	if((reg_825e & 0x04) == 0x04) //20200727
	{
		//Tx_HPD=1;
		return 1;
	}
	else
	{
		//Tx_HPD=0;
		return 0;
	}
}


//CEC: start
static void LT9611_HDMI_CEC_ON(struct i2c_client *client, bool enable)
{
	if(enable){
		/* cec init */
		HDMI_WriteI2C_Byte(client, 0xff, 0x80);
		HDMI_WriteI2C_Byte(client, 0x0d, 0xff);
		HDMI_WriteI2C_Byte(client, 0x15, 0xf1); //reset cec logic
		HDMI_WriteI2C_Byte(client, 0x15, 0xf9); 
		HDMI_WriteI2C_Byte(client, 0xff, 0x86); 
		HDMI_WriteI2C_Byte(client, 0xfe, 0xa5); //clk div
	}
	else{
		HDMI_WriteI2C_Byte(client, 0xff, 0x80);
		HDMI_WriteI2C_Byte(client, 0x15, 0xf1); 
	}
}

static void lt9611_cec_logical_reset(struct i2c_client *client)
{
	HDMI_WriteI2C_Byte(client, 0xff, 0x80);
	HDMI_WriteI2C_Byte(client, 0x15, 0xf1); //reset cec logic
	HDMI_WriteI2C_Byte(client, 0x15, 0xf9);
}


static void lt9611_cec_msg_set_logical_address(struct i2c_client *client, struct cec_msg *cec_msg)
{
	struct device *dev = &client->dev;
	u8 logical_address;
/*
    0xf8, 0xf7   //Register
    0x00, 0x01,  //LA 0
    0x00, 0x02,  //LA 1
    0x00, 0x03,  //LA 2
    0x00, 0x04,  //LA 3
    0x00, 0x10,  //LA 4
    0x00, 0x20,  //LA 5
    0x00, 0x30,  //LA 6
    0x00, 0x40,  //LA 7
    0x01, 0x00,  //LA 8
    0x02, 0x00,  //LA 9
    0x03, 0x00,  //LA 10
    0x04, 0x00,  //LA 11
    0x10, 0x00,  //LA 12
    0x20, 0x00,  //LA 13
    0x30, 0x00,  //LA 14
    0x40, 0x00	 //LA 15
*/


	if(!cec_msg->la_allocation_done)
		logical_address = 15;
	else
		logical_address = cec_msg->logical_address;

	if(logical_address > 15)
	{
		dev_info(dev, "LA error!\n");
		return;
	}

	HDMI_WriteI2C_Byte(client, 0xff, 0x86);
		
	switch(logical_address) {
	case 0: 
		HDMI_WriteI2C_Byte(client, 0xf7, 0x01);
		HDMI_WriteI2C_Byte(client, 0xf8, 0x00);
	break;

	case 1: 
		HDMI_WriteI2C_Byte(client, 0xf7, 0x02);
		HDMI_WriteI2C_Byte(client, 0xf8, 0x00);
	break;

	case 2: 
		HDMI_WriteI2C_Byte(client, 0xf7, 0x03);
		HDMI_WriteI2C_Byte(client, 0xf8, 0x00);
	break;

	case 3: 
		HDMI_WriteI2C_Byte(client, 0xf7, 0x04);
		HDMI_WriteI2C_Byte(client, 0xf8, 0x00);
	break;

	case 4: 
		HDMI_WriteI2C_Byte(client, 0xf7, 0x10);
		HDMI_WriteI2C_Byte(client, 0xf8, 0x00);
	break;

	case 5: 
		HDMI_WriteI2C_Byte(client, 0xf7, 0x20);
		HDMI_WriteI2C_Byte(client, 0xf8, 0x00);
	break;

	case 6: 
		HDMI_WriteI2C_Byte(client, 0xf7, 0x30);
		HDMI_WriteI2C_Byte(client, 0xf8, 0x00);
	break;

	case 7: 
		HDMI_WriteI2C_Byte(client, 0xf7, 0x40);
		HDMI_WriteI2C_Byte(client, 0xf8, 0x00);
	break;

	case 8: 
		HDMI_WriteI2C_Byte(client, 0xf7, 0x00);
		HDMI_WriteI2C_Byte(client, 0xf8, 0x01);
	break;

	case 9: 
		HDMI_WriteI2C_Byte(client, 0xf7, 0x00);
		HDMI_WriteI2C_Byte(client, 0xf8, 0x02);
	break;

	case 10: 
		HDMI_WriteI2C_Byte(client, 0xf7, 0x00);
		HDMI_WriteI2C_Byte(client, 0xf8, 0x03);
	break;

	case 11: 
		HDMI_WriteI2C_Byte(client, 0xf7, 0x00);
		HDMI_WriteI2C_Byte(client, 0xf8, 0x04);
	break;

	case 12: 
		HDMI_WriteI2C_Byte(client, 0xf7, 0x00);
		HDMI_WriteI2C_Byte(client, 0xf8, 0x10);
	break;

	case 13: 
		HDMI_WriteI2C_Byte(client, 0xf7, 0x00);
		HDMI_WriteI2C_Byte(client, 0xf8, 0x20);
	break;

	case 14: 
		HDMI_WriteI2C_Byte(client, 0xf7, 0x00);
		HDMI_WriteI2C_Byte(client, 0xf8, 0x30);
	break;

	case 15: 
		HDMI_WriteI2C_Byte(client, 0xf7, 0x00);
		HDMI_WriteI2C_Byte(client, 0xf8, 0x40);
	break;

	default: break;   
	}

}


static int do_checksum(const unsigned char *x, u8 len)
{
	unsigned char check = x[len];
	unsigned char sum = 0;
	int i;

	pr_info(" Checksum: 0x%02x\n", check);

	for (i = 0; i < len; i++)
		sum += x[i];

	if ((unsigned char)(check + sum) != 0) {
		pr_info(" (should be 0x%02x)\n", -sum & 0xff);
		return 0;
	}

	pr_info(" (valid)\n");
	return 1;
}


static int lt9611_parse_physical_address(struct cec_msg *cec_msg, u8 *edid) // parse edid data from edid.
{
	int version;
	int offset = 0;
	int offset_d = 0;
	int tag_code;
	u16 physical_address;

	version = edid[0x81];
	offset_d = edid[0x82];

	if(!do_checksum(edid, 255))
		return 0; //prase_physical_address fail.

	if (version < 3)
		return 0; //prase_physical_address fail.

	if (offset_d < 5)
		return 0; //prase_physical_address fail.

	tag_code = (edid[0x84 + offset] & 0xe0)>>5;

	while(tag_code != 0x03){
		if((edid[0x84 + offset] & 0x1f) == 0 )
			return 0;
		offset += edid[0x84 + offset] & 0x1f;
		offset++;

		if(offset > (offset_d - 4))
			return 0;

		tag_code = (edid[0x84 + offset] & 0xe0)>>5;
	}
    
    	pr_info("\nvsdb: 0x%02x,0x%02x,0x%02x", edid[0x84 + offset],edid[0x85 + offset],edid[0x86 + offset]);

	if((edid[0x84 + offset + 1] == 0x03) &&
	(edid[0x84 + offset + 2] == 0x0c) &&
	(edid[0x84 + offset + 3] == 0x00))
	{
		physical_address = edid[0x84 + offset + 4];
		physical_address = (physical_address << 8) + edid[0x84 + offset + 5];

		cec_msg->physical_address = physical_address; 
		pr_info("prase physical address success! 0x%x\n",physical_address);
		return 1;
	}

	return 0;      
}

static void lt9611_hdmi_cec_read(struct i2c_client *client, struct cec_msg *cec_msg) // transfer cec msg from LT9611 regisrer to rx_buffer.
{
	u8 size, i;
	HDMI_WriteI2C_Byte(client, 0xff, 0x86); 
	HDMI_WriteI2C_Byte(client, 0xf5, 0x01); //lock rx data buff
	size = HDMI_ReadI2C_Byte(client, 0xd3);
	cec_msg->rx_data_buff[0] = size;
	//dev_info(dev, "cec rec: ");
	for(i = 1;i<= size; i++)
	{
		cec_msg->rx_data_buff[i] = HDMI_ReadI2C_Byte(client, 0xd3 + i);
		//pr_info("0x%02bx, ",cec_msg->rx_data_buff[i]);
	}
	HDMI_WriteI2C_Byte(client, 0xf5, 0x00); //unlock rx data buff
}


static void lt9611_hdmi_cec_write(struct i2c_client *client, struct cec_msg *cec_msg)// send cec msg which is in tx_buffer.
{
	u8 size, i;
	size = cec_msg->tx_data_buff[0];
	cec_msg->retries_times = 0;

	HDMI_WriteI2C_Byte(client, 0xff, 0x86);
	HDMI_WriteI2C_Byte(client, 0xf5, 0x01); //lock rx data buff
	HDMI_WriteI2C_Byte(client, 0xf4, size);   
	for(i = 0;i<= size; i++)
	{
		HDMI_WriteI2C_Byte(client, 0xe4+i, cec_msg->tx_data_buff[1+i]);  
	}
	HDMI_WriteI2C_Byte(client, 0xf9, 0x03); //start send msg
	mdelay(25*i); //wait HDMI
	HDMI_WriteI2C_Byte(client, 0xf5, 0x00); //unlock rx data buff
}



static void lt9611_cec_msg_write_demo(struct i2c_client *client) //for debug
{
	CEC_TxData_Buff[0] = 0x05; //data counter to be send
	CEC_TxData_Buff[1] = 0x40; //first cec data(in spec, sender id = 0x05, 
								 //receiver id = 0x00
	CEC_TxData_Buff[2] = 0x84; //second cec data(in spec, it is opcode =0x84)
	CEC_TxData_Buff[3] = 0x10; //parameter of opcode
	CEC_TxData_Buff[4] = 0x00; //parameter of opcode
	CEC_TxData_Buff[5] = 0x05; //parameter of opcode

	//lt9611_hdmi_cec_write(client, CEC_TxData_Buff);
}

static void lt9611_broad_cast_demo(struct i2c_client *client) // for debug
{
	CEC_TxData_Buff[0] = 0x05; //data counter to be send
	CEC_TxData_Buff[1] = 0x40; //first cec data(in spec, sender id = 0x05, 
							 //receiver id = 0x00
	CEC_TxData_Buff[2] = 0x84; //second cec data(in spec, it is opcode =0x84)
	CEC_TxData_Buff[3] = 0x10; //parameter of opcode
	CEC_TxData_Buff[4] = 0x00; //parameter of opcode
	CEC_TxData_Buff[5] = 0x05; //parameter of opcode

	//lt9611_hdmi_cec_write(client, CEC_TxData_Buff);
}


static void lt9611_cec_la_allocation(struct i2c_client *client, struct cec_msg *cec_msg) //polling  logical address.
{
	u8 logical_address;

	logical_address = cec_msg->logical_address;
	cec_msg->tx_data_buff[0] = 0x01; //data counter to be send
	cec_msg->tx_data_buff[1] = (logical_address<<4)|logical_address; 
	                     //first cec data(in spec, sender id = 0x04, 
							     //receiver id = 0x04;
	lt9611_hdmi_cec_write(client, cec_msg);
}

static void lt9611_cec_report_physical_address(struct i2c_client *client, struct cec_msg *cec_msg) // report physical address.
{
	cec_msg->tx_data_buff[0] = 0x05; //data counter to be send
	cec_msg->tx_data_buff[1] = (cec_msg->logical_address<<4)|0x0f; 
	                            //first cec data([7:4]=initiator ;[7:4]= destintion)
	cec_msg->tx_data_buff[2] = 0x84; //opcode
	cec_msg->tx_data_buff[3] = (u8)(cec_msg->physical_address>>8); //parameter of opcode
	cec_msg->tx_data_buff[4] = (u8)(cec_msg->physical_address); //parameter of opcode
	cec_msg->tx_data_buff[5] = 0x04; //device type = playback device

	//pr_info("\nPA:%bx, %bx",cec_msg->tx_data_buff[3],cec_msg->tx_data_buff[4]);
	                                
	lt9611_hdmi_cec_write(client, cec_msg);
}



static void lt9611_cec_menu_activate(struct i2c_client *client, struct cec_msg *cec_msg) // report physical address.
{
        cec_msg->tx_data_buff[0] = 0x04; //data counter to be send
        cec_msg->tx_data_buff[1] = (cec_msg->logical_address<<4)|cec_msg->destintion; 
                                    //first cec data([7:4]=initiator ;[7:4]= destintion)
        cec_msg->tx_data_buff[2] = 0x8e; //opcode
        cec_msg->tx_data_buff[3] = 0x00; //parameter of opcode
 

        //pr_info("\nPA:%bx, %bx",cec_msg->tx_data_buff[3],cec_msg->tx_data_buff[4]);
                                        
        lt9611_hdmi_cec_write(client, cec_msg);
}

static void lt9611_cec_feature_abort(struct i2c_client *client, struct cec_msg *cec_msg, u8 reason)// report feature abort 
{
	cec_msg->tx_data_buff[0] = 0x03; //data counter to be send
	cec_msg->tx_data_buff[1] = (cec_msg->logical_address<<4)|cec_msg->destintion; 
	                                 //first cec data([7:4]=initiator ;[7:4]= destintion) 
	cec_msg->tx_data_buff[2] = 0x00; //opcode
	cec_msg->tx_data_buff[3] = reason; //parameter1 of opcode

	lt9611_hdmi_cec_write(client, cec_msg);
}

static void lt9611_cec_frame_retransmission(struct i2c_client *client, struct cec_msg *cec_msg) 
{
	if(cec_msg->retries_times < 5){
		HDMI_WriteI2C_Byte(client, 0xff, 0x86);   
		HDMI_WriteI2C_Byte(client, 0xf9, 0x02);
		HDMI_WriteI2C_Byte(client, 0xf9, 0x03);   //start send msg
	}
	cec_msg->retries_times ++; 
}

#if 0
void lt9611_cec_device_polling(struct cec_msg *cec_msg)
{
    static u8 i;
    if(!cec_msg->la_allocation_done)
        {
            cec_msg->tx_data_buff[0] = 0x01; //data counter to be send
            cec_msg->tx_data_buff[1] = i;    //first cec data(in spec, sender id = 0x04, 
                  					         //receiver id = 0x04;
            lt9611_hdmi_cec_write(cec_msg);
            if(i >13 )
            cec_msg->la_allocation_done = 1;
            (i>13)?(i=0):(i++);
        }
}

void lt9611_cec_msg_tx_handle(struct cec_msg *cec_msg)
{
    u8 cec_status;
    u8 header;
    u8 opcode;
    u8 i;
    cec_status = cec_msg ->cec_status;
    
//    if( cec_msg ->send_msg_done) //There is no tx msg to be handled 
//        return; 

    if(cec_status&CEC_ERROR_INITIATOR){
        dev_info(dev, "\nCEC_ERROR_INITIATOR.");
        lt9611_cec_logical_reset();
        return;
        }

    if(cec_status&CEC_ARB_LOST){
        dev_info(dev, "\nCEC_ARB_LOST."); //lost arbitration
        return;
        }

    if(cec_status&(CEC_SEND_DONE|CEC_NACK|CEC_ERROR_FOLLOWER)) do {

        dev_info(dev, "\ntx_date: ");
        for(i = 0; i < cec_msg->tx_data_buff[0]; i++)
            dev_info(dev, "0x%02bx, ",cec_msg->tx_data_buff[i + 1]);


        if(cec_status&CEC_SEND_DONE)
            dev_info(dev, "CEC_SEND_DONE >>");
        if(cec_status&CEC_NACK)
            dev_info(dev, "NACK >>"); 
              
        header = cec_msg ->tx_data_buff[1];

        if((header == 0x44)||(header == 0x88)||(header == 0xbb)) //logical address allocation
        {
            if(cec_status&CEC_NACK){
            cec_msg ->logical_address = header&0x0f;
            dev_info(dev, "la_allocation_done.");
            lt9611_cec_msg_set_logical_address(cec_msg);
            lt9611_cec_report_physical_address(cec_msg);
            }

            if(cec_status&CEC_SEND_DONE){
            
                if(cec_msg->logical_address == 4)// go to next la.
                    cec_msg->logical_address = 8;
                else if(cec_msg->logical_address == 8)// go to next la. 
                    cec_msg->logical_address = 11; 
                else if(cec_msg->logical_address == 11) // go to next la. 
                    cec_msg->logical_address = 4;

                lt9611_cec_la_allocation(cec_msg);     
            }

            break;
        }

        if(cec_status&(CEC_NACK|CEC_ERROR_FOLLOWER)){
            lt9611_cec_frame_retransmission(cec_msg);        
        }

        if(cec_msg ->tx_data_buff[0] < 2)  //check tx data length
            break;

   
        opcode = cec_msg ->tx_data_buff[2];
        if(opcode == 0x84){
              cec_msg ->report_physical_address_done = 1;
              dev_info(dev, "report_physical_address.");
        } 

        if(opcode == 0x00){
              dev_info(dev, "feature abort"); 
        } 

        } while (0);
}


void lt9611_cec_msg_rx_parse(struct cec_msg *cec_msg)
{
    u8 cec_status;
    u8 header;
    u8 opcode;
    u8 initiator;
    u8 destintion;
    u8 i;
    cec_status = cec_msg ->cec_status;

//    if( cec_msg ->parse_msg_done) //There is no Rx msg to be parsed 
//        return; 

    if(cec_status&CEC_ERROR_FOLLOWER){
        dev_info(dev, "\nCEC_ERROR_FOLLOWER.");
        return;
        }

    if(!(cec_status&CEC_REC_DATA)){
        return;
        }
        
    lt9611_hdmi_cec_read(&lt9611_cec_msg);

    if(cec_msg ->rx_data_buff[0] < 1) //check rx data length
        return; 

    dev_info(dev, "\nrx_date: ");
    for(i = 0; i < cec_msg->rx_data_buff[0]; i++)
    dev_info(dev, "0x%02bx, ",cec_msg->rx_data_buff[i + 1]);

    dev_info(dev, "parse <<");
    header = cec_msg ->rx_data_buff[1]; 
    destintion = header&0x0f;
    initiator = (header&0xf0) >> 4;
    //cec_msg ->parse_msg_done = 1;

    if(header == 0x4f){
       dev_info(dev, "lt9611 broadcast msg.");
        }
    
    if(cec_msg ->rx_data_buff[0] < 2) //check rx data length
        return; 
    
    opcode = cec_msg ->rx_data_buff[2];

// CECT 12 Invalid Msg Tests
if((header&0x0f) == 0x0f){ 
    if((opcode == 0x00)||
        (opcode == 0x83)||
        (opcode == 0x8e)||
        (opcode == 0x90)||
        (opcode == 0xff)){
        dev_info(dev, "Invalid msg, destination address error"); //these msg should not be broadcast msg, but they do.
        return;
    }
}else{
    if((opcode == 0x84)||
        (opcode == 0x84)||
        (opcode == 0x84)){
        dev_info(dev, "Invalid msg, destination address error"); //these msg should be broadcast msg, but they not.
        return;
    }
}
        
    if(opcode == 0xff) //abort
    {
        dev_info(dev, "abort.");
        if(destintion == 0x0f) //ignor broadcast abort msg.
            return; 
        cec_msg ->destintion = initiator;
        lt9611_cec_feature_abort(cec_msg, CEC_ABORT_REASON_0);
    }

    if(opcode == 0x83) //give physical address
    {
        dev_info(dev, "give physical address.");
        lt9611_cec_report_physical_address(cec_msg);
    }

    if(opcode == 0x90) //report power status
    {
        dev_info(dev, "report power status.");
        if(cec_msg ->rx_data_buff[0] < 3) {
            dev_info(dev, "<error:parameters missing");
            return; //parameters missing, ignor this msg.
        }
    }

    if(opcode == 0x8e) //menu status
    {
        dev_info(dev, "menu status.");
        if(cec_msg ->rx_data_buff[0] < 3) {
            dev_info(dev, "<error:parameters missing");
            return; //parameters missing, ignor this msg.
        }
    }

    if(opcode == 0x00) //feature abort
    {
        dev_info(dev, "feature abort.");
        if(cec_msg ->rx_data_buff[0] < 3) {
            dev_info(dev, "<error:parameters missing");
            return; //parameters missing, ignor this msg.
        }
    }

    if(opcode == 0x9e) //cec version
    {
        dev_info(dev, "cec version.");
        if(cec_msg ->rx_data_buff[0] < 3) {
            dev_info(dev, "<error:parameters missing");
            return; //parameters missing, ignor this msg.
        }
    }

    if(opcode == 0x84) //report physical address
    {
        dev_info(dev, "report physical address.");
        if(cec_msg ->rx_data_buff[0] < 5) {
            dev_info(dev, "<error:parameters missing");
            return; //parameters missing, ignor this msg.
        }
    }

    if(opcode == 0x86) //set stream path
    {
        dev_info(dev, "set stream path.");
        if(cec_msg ->rx_data_buff[0] < 4) {
            dev_info(dev, "<error:parameters missing");
            return; //parameters missing, ignor this msg.
        }
        lt9611_cec_report_physical_address(cec_msg);
        Timer0_Delay1ms(120);
        lt9611_cec_menu_activate(cec_msg);
    }
}

void lt9611_cec_msg_init(struct cec_msg *cec_msg)
{
    LT9611_HDMI_CEC_ON(1);
    cec_msg->physical_address = 0x2000;
    cec_msg->logical_address = 4;
    cec_msg->report_physical_address_done = 0;
    cec_msg->la_allocation_done = 0;
    lt9611_cec_msg_set_logical_address(cec_msg);
}
#endif

//CEC: end

/////////////////////////////////////////////////////////////
//These function for debug: start
/////////////////////////////////////////////////////////////
static void LT9611_Frequency_Meter_Byte_Clk(struct i2c_client *client)
{
	struct device *dev = &client->dev;
	u8 temp;
	u32 reg=0x00;

	/* port A byte clk meter */
	HDMI_WriteI2C_Byte(client, 0xff,0x82); 
	HDMI_WriteI2C_Byte(client, 0xc7,0x03); //PortA
	mdelay(50);
	temp = HDMI_ReadI2C_Byte(client, 0xcd);
	if((temp&0x60)==0x60) /* clk stable */
	{
		reg = (u32)(temp&0x0f)*65536;
		temp = HDMI_ReadI2C_Byte(client, 0xce);
		reg = reg + (u16)temp*256;
		temp = HDMI_ReadI2C_Byte(client, 0xcf);
		reg = reg + temp;
		dev_info(dev, "port A byte clk = %d\n",reg);
	}
	else /* clk unstable */
		dev_info(dev, "port A byte clk unstable\n");

	
	/* port B byte clk meter */
	HDMI_WriteI2C_Byte(client, 0xff,0x82); 
	HDMI_WriteI2C_Byte(client, 0xc7,0x04); 
	mdelay(50);
	temp = HDMI_ReadI2C_Byte(client, 0xcd);
	if((temp&0x60)==0x60) /* clk stable */
	{
		reg = (u32)(temp & 0x0f) * 65536;
		temp = HDMI_ReadI2C_Byte(client, 0xce);
		reg =reg + (u16)temp * 256;
		temp = HDMI_ReadI2C_Byte(client, 0xcf);
		reg = reg + temp;
		dev_info(dev, "port B byte clk = %d\n",reg);
	}
	else /* clk unstable */
		dev_info(dev, "port B byte clk unstable\n");
}

static void LT9611_Htotal_Sysclk(struct i2c_client *client)
{
	struct device *dev = &client->dev;
#ifdef _htotal_stable_check_
	u16 reg;
	u8 loopx;
	for(loopx = 0; loopx < 10; loopx++)
	{
		HDMI_WriteI2C_Byte(client, 0xff,0x82);
		reg = HDMI_ReadI2C_Byte(client, 0x86);
		reg = reg * 256 + HDMI_ReadI2C_Byte(client, 0x87);
		dev_info(dev, "Htotal_Sysclk = %d\n", reg);
		//printdec_u32(reg);
	}
#endif
}
static void LT9611_Pcr_MK_Print(struct i2c_client *client)
{
#ifdef _pcr_mk_printf_
	u8 loopx;
	for(loopx = 0; loopx < 8; loopx++)
	{
		HDMI_WriteI2C_Byte(client, 0xff,0x83); 
		printk("M:0x%02x", HDMI_ReadI2C_Byte(client, 0x97));
		printk(" 0x%02x", HDMI_ReadI2C_Byte(client, 0xb4));
		printk(" 0x%02x", HDMI_ReadI2C_Byte(client, 0xb5));
		printk(" 0x%02x", HDMI_ReadI2C_Byte(client, 0xb6));
		printk(" 0x%02x \n", HDMI_ReadI2C_Byte(client, 0xb7));
		mdelay(1000);
	}
#endif
}

static void LT9611_Dphy_debug(struct i2c_client *client)
{
	struct device *dev = &client->dev;
#ifdef _mipi_Dphy_debug_
	u8 temp;
	
	HDMI_WriteI2C_Byte(client, 0xff,0x83);
	temp = HDMI_ReadI2C_Byte(client, 0xbc);
	if(temp == 0x55)
		dev_info(dev, "port A lane PN is right\n");
	else
		dev_info(dev, "port A lane PN error 0x83bc = 0x%02x\n",temp);
	
	temp = HDMI_ReadI2C_Byte(client, 0x99);
	if(temp == 0xb8)
		dev_info(dev, "port A lane 0 sot right \n");
	else
		dev_info(dev, "port A lane 0 sot error = 0x%02x\n",temp);

	temp = HDMI_ReadI2C_Byte(client, 0x9b);
	if(temp == 0xb8)
		dev_info(dev, "port A lane 1 sot right \n");
	else
		dev_info(dev, "port A lane 1 sot error = 0x%02x\n",temp);
	
	temp = HDMI_ReadI2C_Byte(client, 0x9d);
	if(temp == 0xb8)
		dev_info(dev, "port A lane 2 sot right \n");
	else
		dev_info(dev, "port A lane 2 sot error = 0x%02x\n",temp);
	
	temp = HDMI_ReadI2C_Byte(client, 0x9f);
	if(temp == 0xb8)
		dev_info(dev, "port A lane 3 sot right \n");
	else
		dev_info(dev, "port A lane 3 sot error = 0x%02x\n",temp);
	
	dev_info(dev, "port A lane 0 settle = 0x%02x\n",HDMI_ReadI2C_Byte(client, 0x98));
	dev_info(dev, "port A lane 1 settle = 0x%02x\n",HDMI_ReadI2C_Byte(client, 0x9a));
 	dev_info(dev, "port A lane 2 settle = 0x%02x\n",HDMI_ReadI2C_Byte(client, 0x9c));
 	dev_info(dev, "port A lane 3 settle = 0x%02x\n",HDMI_ReadI2C_Byte(client, 0x9e));

#endif
}

/////////////////////////////////////////////////////////////
//These function for debug: end
/////////////////////////////////////////////////////////////


static void LT9611_IRQ_Init(struct i2c_client *client) //dsren
{            
	//int hpd interrupt
	HDMI_WriteI2C_Byte(client, 0xff,0x82);
	//HDMI_WriteI2C_Byte(client, 0x10,0x00); //Output low level active;
	HDMI_WriteI2C_Byte(client, 0x58,0x0a); //Det HPD 0x0a --> 0x08 20200727
	HDMI_WriteI2C_Byte(client, 0x59,0x80); //HPD debounce width

	//intial vid change interrupt
	HDMI_WriteI2C_Byte(client, 0x9e,0xf7);
}

static void LT9611_Globe_Interrupts(struct i2c_client *client, bool on)
{
	if(on)
	{
		HDMI_WriteI2C_Byte(client, 0xff,0x81);
		HDMI_WriteI2C_Byte(client, 0x51,0x10); //hardware mode irq pin out
	}
	else
	{
		HDMI_WriteI2C_Byte(client, 0xff,0x81); //software mode irq pin out = 1;
		HDMI_WriteI2C_Byte(client, 0x51,0x30); 
	}

}

static void LT9611_Enable_Interrupts(struct i2c_client *client, u8 interrupts, bool on)
{
	struct device *dev = &client->dev;

	if(interrupts == HPD_INTERRUPT_ENABLE)
	{
		if(on)
		{
			HDMI_WriteI2C_Byte(client, 0xff,0x82);
			HDMI_WriteI2C_Byte(client, 0x07,0xff); //clear3
			HDMI_WriteI2C_Byte(client, 0x07,0x3f); //clear3
			HDMI_WriteI2C_Byte(client, 0x03,0x3f); //mask3  //Tx_det
			dev_info(dev, "LT9611_Enable_Interrupts: hpd_irq_enable\n");
		}
		else
		{
			HDMI_WriteI2C_Byte(client, 0xff,0x82);
			HDMI_WriteI2C_Byte(client, 0x07,0xff); //clear3
			HDMI_WriteI2C_Byte(client, 0x03,0xff); //mask3  //Tx_det
			dev_info(dev, "LT9611_Enable_Interrupts: hpd_irq_disable\n");
		}
	}

	if(interrupts == VID_CHG_INTERRUPT_ENABLE)
	{
		if(on)
		{
			HDMI_WriteI2C_Byte(client, 0xff,0x82);
			HDMI_WriteI2C_Byte(client, 0x9e,0xff); //clear vid chk irq
			HDMI_WriteI2C_Byte(client, 0x9e,0xf7); 
			HDMI_WriteI2C_Byte(client, 0x04,0xff); //clear0
			HDMI_WriteI2C_Byte(client, 0x04,0xfe); //clear0
			HDMI_WriteI2C_Byte(client, 0x00,0xfe); //mask0 vid_chk_IRQ
			dev_info(dev, "LT9611_Enable_Interrupts: vid_chg_irq_enable\n");
		}
		else
		{
			HDMI_WriteI2C_Byte(client, 0xff,0x82);
			HDMI_WriteI2C_Byte(client, 0x04,0xff); //clear0
			HDMI_WriteI2C_Byte(client, 0x00,0xff); //mask0 vid_chk_IRQ
			dev_info(dev, "LT9611_Enable_Interrupts: vid_chg_irq_disable\n");
		}
	}
	if(interrupts == CEC_INTERRUPT_ENABLE)
	{
		if(on)
		{
			HDMI_WriteI2C_Byte(client, 0xff, 0x86); 
			HDMI_WriteI2C_Byte(client, 0xfa, 0x00); //cec interrup mask
			HDMI_WriteI2C_Byte(client, 0xfc, 0x7f); //cec irq clr
			HDMI_WriteI2C_Byte(client, 0xfc, 0x00); 

			/* cec irq init */
			HDMI_WriteI2C_Byte(client, 0xff, 0x82);
			HDMI_WriteI2C_Byte(client, 0x01, 0x7f); //mask bit[7]
			HDMI_WriteI2C_Byte(client, 0x05, 0xff); //clr bit[7]
			HDMI_WriteI2C_Byte(client, 0x05, 0x7f);
		}
		else
		{
			HDMI_WriteI2C_Byte(client, 0xff, 0x86); 
			HDMI_WriteI2C_Byte(client, 0xfa, 0xff); //cec interrup mask
			HDMI_WriteI2C_Byte(client, 0xfc, 0x7f); //cec irq clr

			/* cec irq init */
			HDMI_WriteI2C_Byte(client, 0xff, 0x82);
			HDMI_WriteI2C_Byte(client, 0x01, 0xff); //mask bit[7]
			HDMI_WriteI2C_Byte(client, 0x05, 0xff); //clr bit[7]
		}
	}
}

static void LT9611_HDP_Interrupt_Handle(struct i2c_client *client)
{
	struct device *dev = &client->dev;
	Tx_HPD = LT9611_Get_HPD_Status(client);

	HDMI_WriteI2C_Byte(client, 0xff,0x82); 
	HDMI_WriteI2C_Byte(client, 0x07,0xff); //clear3
	HDMI_WriteI2C_Byte(client, 0x07,0x3f); //clear3
	 
	if(Tx_HPD)
	{
		dev_info(dev, "LT9611_HDP_Interrupt_Handle: HDMI connected\n");
		LT9611_LowPower_mode(client, 0);
		LT9611_Enable_Interrupts(client, VID_CHG_INTERRUPT_ENABLE, 1);
		mdelay(100);
		LT9611_Read_EDID(client);	
#ifdef cec_on
		lt9611_parse_physical_address(&lt9611_cec_msg, Sink_EDID);
		lt9611_cec_la_allocation(client, &lt9611_cec_msg);
#endif

		LT9611_Video_Check(client);
		if(Video_Format != video_none)
		{
			LT9611_PLL(client, video);
			LT9611_MIPI_Pcr(client, video);
			LT9611_HDMI_TX_Digital(client, video);
			LT9611_HDMI_Out_Enable(client);
		}
		else
		{
			LT9611_HDMI_Out_Disable(client);
			dev_info(dev, "LT9611_HDP_Interrupt_Handle: no mipi video, disable hdmi output\n");
		}
	}
	else
	{     
		dev_info(dev, "LT9611_HDP_Interrupt_Handle: HDMI disconnected\n");
		LT9611_Enable_Interrupts(client, VID_CHG_INTERRUPT_ENABLE, 0);
		LT9611_LowPower_mode(client, 1);
#ifdef cec_on
		lt9611_cec_msg_init(&lt9611_cec_msg);
#endif
	}
}

static void LT9611_Vid_Chg_Interrupt_Handle(struct i2c_client *client)
{
	struct device *dev = &client->dev;

	dev_info(dev, "LT9611_Vid_Chg_Interrupt_Handle: \n");
#if 1
	HDMI_WriteI2C_Byte(client, 0xff,0x82); 
	HDMI_WriteI2C_Byte(client, 0x9e,0xff); //clear vid chk irq
	HDMI_WriteI2C_Byte(client, 0x9e,0xf7); 

	HDMI_WriteI2C_Byte(client, 0x04,0xff); //clear0 irq
	HDMI_WriteI2C_Byte(client, 0x04,0xfe); 
#endif
	//mdelay(100);
	LT9611_Video_Check(client);

	if(Video_Format != video_none)
	{
		LT9611_PLL(client, video);
		LT9611_MIPI_Pcr(client, video);
		LT9611_HDMI_TX_Digital(client, video);
		LT9611_HDMI_Out_Enable(client);
		//LT9611_HDCP_Enable(client);
	}
	else
	{
		//dev_info(dev, "LT9611_Vid_Chg_Interrupt_Handle: no mipi video");
		LT9611_HDMI_Out_Disable(client);
	}	
}

static void lt9611_cec_msg_Interrupt_Handle(struct i2c_client *client, struct cec_msg *cec_msg)
{
	struct device *dev = &client->dev;
	u8 cec_status;

	HDMI_WriteI2C_Byte(client, 0xff,0x86);
	cec_status = HDMI_ReadI2C_Byte(client, 0xd2) ;

	cec_msg->cec_status = cec_status;
	dev_info(dev, "\nIRQ cec_status: 0x%02x\n",cec_status);

	HDMI_WriteI2C_Byte(client, 0xff, 0x86); 
	HDMI_WriteI2C_Byte(client, 0xfc, 0x7f); //cec irq clr
	HDMI_WriteI2C_Byte(client, 0xfc, 0x00);

	HDMI_WriteI2C_Byte(client, 0xff,0x82); 
	HDMI_WriteI2C_Byte(client, 0x05,0xff); //clear3
	HDMI_WriteI2C_Byte(client, 0x05,0x7f); //clear3

//        lt9611_cec_msg_tx_handle(cec_msg);
//        lt9611_cec_msg_rx_parse(cec_msg);
}


/////////////////////////////////////////////////////////////
//These function for Pattern output: start
/////////////////////////////////////////////////////////////
static void LT9611_pattern_gcm(struct i2c_client *client, struct video_timing *video_format)
{
	u8 POL;
	POL = (video_format->h_polarity) * 0x10 + (video_format->v_polarity) * 0x20;
	POL = ~POL;
	POL &= 0x30;

	HDMI_WriteI2C_Byte(client, 0xff,0x82);
	HDMI_WriteI2C_Byte(client, 0xa3,(u8)((video_format->hs + video_format->hbp) / 256));//de_delay
	HDMI_WriteI2C_Byte(client, 0xa4,(u8)((video_format->hs + video_format->hbp) % 256));
	HDMI_WriteI2C_Byte(client, 0xa5,(u8)((video_format->vs + video_format->vbp) % 256));//de_top
	HDMI_WriteI2C_Byte(client, 0xa6,(u8)(video_format->hact / 256));
	HDMI_WriteI2C_Byte(client, 0xa7,(u8)(video_format->hact % 256));  //de_cnt
	HDMI_WriteI2C_Byte(client, 0xa8,(u8)(video_format->vact / 256));
	HDMI_WriteI2C_Byte(client, 0xa9,(u8)(video_format->vact % 256));  //de_line
	HDMI_WriteI2C_Byte(client, 0xaa,(u8)(video_format->htotal / 256));
	HDMI_WriteI2C_Byte(client, 0xab,(u8)(video_format->htotal % 256));//htotal
	HDMI_WriteI2C_Byte(client, 0xac,(u8)(video_format->vtotal / 256));
	HDMI_WriteI2C_Byte(client, 0xad,(u8)(video_format->vtotal % 256));//vtotal
	HDMI_WriteI2C_Byte(client, 0xae,(u8)(video_format->hs / 256));
	HDMI_WriteI2C_Byte(client, 0xaf,(u8)(video_format->hs % 256));    //hvsa
	HDMI_WriteI2C_Byte(client, 0xb0,(u8)(video_format->vs % 256));    //vsa

	HDMI_WriteI2C_Byte(client, 0x47,(u8)(POL|0x07));  //sync polarity

}

static void LT9611_pattern_pixel_clk(struct i2c_client *client, struct video_timing *video_format)
{
	struct device *dev = &client->dev;
	u32 pclk;
	pclk = video_format->pclk_khz;

	dev_info(dev, "set pixel clk = %d\n", pclk);
	//printdec_u32(pclk); //Dec
	
	HDMI_WriteI2C_Byte(client, 0xff,0x83);
	HDMI_WriteI2C_Byte(client, 0x2d,0x50);
	
	if(pclk == 297000)
	{
		HDMI_WriteI2C_Byte(client, 0x26,0xb6);
		HDMI_WriteI2C_Byte(client, 0x27,0xf0);
	}
	if(pclk == 148500)
	{
		HDMI_WriteI2C_Byte(client, 0x26,0xb7);
	}
	if(pclk == 74250)
	{
		HDMI_WriteI2C_Byte(client, 0x26,0x9c);
	}
	HDMI_WriteI2C_Byte(client, 0xff,0x80);
	HDMI_WriteI2C_Byte(client, 0x11,0x5a); //Pcr reset
	HDMI_WriteI2C_Byte(client, 0x11,0xfa);
}

static void LT9611_pattern_en(struct i2c_client *client)
{
	HDMI_WriteI2C_Byte(client, 0xff,0x82); 
	HDMI_WriteI2C_Byte(client, 0x4f,0x80);    //[7] = Select ad_txpll_d_clk.
	HDMI_WriteI2C_Byte(client, 0x50,0x20);
}
/*
void LT9611_pattern(void)
{
	//DTV
	//video = &video_640x480_60Hz;
	//video = &video_720x480_60Hz;
	//video = &video_1280x720_60Hz;
	video = &video_1920x1080_60Hz;
	//video = &video_3840x2160_30Hz;

	//DMT
	//video = &video_1024x600_60Hz;
	//video = &video_1024x600_60Hz;
	//video = &video_1280x800_60Hz;

	//video = &video_1920x1080_30Hz;

	LT9611_Chip_ID();
	LT9611_System_Init(); 
	LT9611_pattern_en();
	LT9611_PLL(video);
	LT9611_pattern_gcm(video);
	
	LT9611_HDMI_TX_Digital(video);
	LT9611_HDMI_TX_Phy();

	LT9611_Pcr_MK_Print();

#if 0	
	//Audio pattern
	HDMI_WriteI2C_Byte(client, 0xff,0x82);
	HDMI_WriteI2C_Byte(client, 0xd6,0x8c);
	HDMI_WriteI2C_Byte(client, 0xd7,0x06); //sync polarity
		
	HDMI_WriteI2C_Byte(client, 0xff,0x84);
	HDMI_WriteI2C_Byte(client, 0x06,0x0c);
	HDMI_WriteI2C_Byte(client, 0x07,0x10);
	HDMI_WriteI2C_Byte(client, 0x16,0x01);
	
	HDMI_WriteI2C_Byte(client, 0x34,0xd4); //CTS_N
#endif
    LT9611_Audio_Init();
	LT9611_HDCP_Init();
	LT9611_load_hdcp_key();
   
    LT9611_Read_EDID();
	LT9611_HDMI_Out_Enable();
    LT9611_HDMI_CEC_ON(1);
	lt9611_cec_msg_set_logical_address();

	while(1)
	{
		lt9611_cec_msg_write_demo();
	}
}
*/
/////////////////////////////////////////////////////////////
//These function for debug: end
/////////////////////////////////////////////////////////////


static int lt9611_init_client(struct i2c_client *client)
{
	struct device *dev = &client->dev;

	LT9611_System_Init(client); 
	
	//LT9611_RST_PD_Init(client); 
	LT9611_MIPI_Input_Analog(client); 
	LT9611_MIPI_Input_Digtal(client);

	mdelay(1000);
	LT9611_Video_Check(client);
	
	LT9611_PLL(client, video);
	LT9611_MIPI_Pcr(client, video); //pcr setup

	LT9611_Audio_Init(client);
	LT9611_CSC(client);
	LT9611_HDCP_Init(client);
	LT9611_HDMI_TX_Digital(client, video);
	LT9611_HDMI_TX_Phy(client);

	LT9611_IRQ_Init(client);;
	//LT9611_Read_EDID(client);

	//LT9611_HDMI_CEC_ON(client, 1);
	//lt9611_cec_msg_set_logical_address(client);
	//lt9611_cec_msg_init(client, &lt9611_cec_msg);	
	LT9611_Enable_Interrupts(client, HPD_INTERRUPT_ENABLE, 1);
	LT9611_Enable_Interrupts(client, VID_CHG_INTERRUPT_ENABLE, 0);
	LT9611_Enable_Interrupts(client, CEC_INTERRUPT_ENABLE, 1);
	
	LT9611_Frequency_Meter_Byte_Clk(client);
	LT9611_Dphy_debug(client);
	LT9611_Htotal_Sysclk(client);
	LT9611_Pcr_MK_Print(client);

	dev_info(dev, "===============LT9611 Initial End==================\n");
	mdelay(200); //HPD have debounce, wait HPD irq.
	//while(1);
	LT9611_HDP_Interrupt_Handle(client);
	//lt9611_cec_msg_write_demo(client);

	return 0;
}

static void LT9611_IRQ_Task(struct work_struct *work)
{
	struct lt9611_data *data = container_of(work, struct lt9611_data, work);
	struct i2c_client *client = data->client;
	unsigned long irqflags;
	u8 irq_flag3;
	u8 irq_flag0;
	u8 irq_flag1;

	//dev_info(&client->dev, "LT9611_IRQ_Task :IRQ Task\n");

	HDMI_WriteI2C_Byte(client, 0xff,0x82); 

	irq_flag0=HDMI_ReadI2C_Byte(client, 0x0c);
	irq_flag1=HDMI_ReadI2C_Byte(client, 0x0d);
	irq_flag3=HDMI_ReadI2C_Byte(client, 0x0f);

	if((irq_flag1 & 0x80) == 0x80)
	{
		lt9611_cec_msg_Interrupt_Handle(client, &lt9611_cec_msg);
	}

	if(irq_flag3 & 0xc0)   //HPD interrupt
	{
		LT9611_HDP_Interrupt_Handle(client);
	}

	if(irq_flag0 & 0x01) //vid_chk
	{
		LT9611_Vid_Chg_Interrupt_Handle(client);
	}

	spin_lock_irqsave(&data->irq_lock, irqflags);
	enable_irq(client->irq);
	spin_unlock_irqrestore(&data->irq_lock, irqflags);
}

static irqreturn_t lt9611_irq_handler(int irq, void *dev_id)
{
	struct lt9611_data *data = dev_id;
	struct i2c_client *client = data->client;
	unsigned long irqflags;

	//dev_info(&client->dev, "lt9611_irq_handler :IRQ Task\n");
	spin_lock_irqsave(&data->irq_lock, irqflags);
	disable_irq_nosync(client->irq);
	spin_unlock_irqrestore(&data->irq_lock, irqflags);

	queue_work(data->lt9611_wq, &data->work);

	return IRQ_HANDLED;
}

/*
 * I2C layer
 */

static int lt9611_probe(struct i2c_client *client,
				    const struct i2c_device_id *id)
{
	struct i2c_adapter *adapter = to_i2c_adapter(client->dev.parent);
	struct lt9611_data *data;
	int err = 0;

	if (!i2c_check_functionality(adapter, I2C_FUNC_SMBUS_BYTE))
		return -EIO;

	data = kzalloc(sizeof(struct lt9611_data), GFP_KERNEL);
	if (!data)
		return -ENOMEM;

	data->client = client;
	i2c_set_clientdata(client, data);
	mutex_init(&data->lock);

	err  = lt9611_parse_dts(client);
	if (err)
		goto exit_kfree;

	lt9611_reset(client);
	if (0xFFFFFF == LT9611_Chip_ID(client) || 0x0 == LT9611_Chip_ID(client)) {
		dev_err(&client->dev, "get wrong chip id of lt9611\n");
		goto exit_dts;
	}
	/* initialize the LT9611 chip */
	err = lt9611_init_client(client);
	if (err)
		goto exit_dts;

	/* interrupt handle */
	if (!client->irq && gpio_is_valid(data->irq_gpio)) {
		client->irq = gpio_to_irq(data->irq_gpio);
	}

	if (!client->irq) {
		dev_err(&client->dev, "no valid irq for lt9611\n");
		goto exit_dts;
	}

	spin_lock_init(&data->irq_lock);

	INIT_WORK(&data->work, LT9611_IRQ_Task);

	data->lt9611_wq = create_singlethread_workqueue("lt9611_wq");
	if (!data->lt9611_wq) {
		dev_err(&client->dev, "create workqueue fail!\n");
		goto exit_dts;
	}

	err = devm_request_threaded_irq(&client->dev, client->irq,
					NULL, lt9611_irq_handler,
					IRQF_TRIGGER_FALLING | IRQF_ONESHOT,
					client->name, data);
	if (err) {
		dev_err(&client->dev, "request irq for lt9611 failed : %d\n", err);
		goto exit_wq;
	}

	/* register sysfs hooks */
	err = sysfs_create_group(&client->dev.kobj, &lt9611_attr_group);
	if (err)
		goto exit_irq;

	return 0;

exit_irq:
	disable_irq_nosync(client->irq);
	devm_free_irq(&client->dev, client->irq, data);

exit_wq:
	if (data->lt9611_wq)
		destroy_workqueue(data->lt9611_wq);

exit_dts:
	if (gpio_is_valid(data->power_gpio))
		gpio_free(data->power_gpio);
	if (gpio_is_valid(data->reset_gpio))
		gpio_free(data->reset_gpio);
	if (gpio_is_valid(data->irq_gpio))
		gpio_free(data->irq_gpio);

exit_kfree:
	kfree(data);
	return err;
}

static int lt9611_remove(struct i2c_client *client)
{
	struct lt9611_data *data = i2c_get_clientdata(client);

	sysfs_remove_group(&client->dev.kobj, &lt9611_attr_group);

	disable_irq_nosync(client->irq);
	devm_free_irq(&client->dev, client->irq, data);

	if (data->lt9611_wq)
		destroy_workqueue(data->lt9611_wq);

	if (gpio_is_valid(data->power_gpio))
		gpio_free(data->power_gpio);
	if (gpio_is_valid(data->reset_gpio))
		gpio_free(data->reset_gpio);
	if (gpio_is_valid(data->irq_gpio))
		gpio_free(data->irq_gpio);

	kfree(i2c_get_clientdata(client));
	i2c_set_clientdata(client, NULL);

	return 0;
}

#ifdef CONFIG_PM_SLEEP
static int lt9611_suspend(struct device *dev)
{
	struct i2c_client *client = to_i2c_client(dev);
	struct lt9611_data *data = i2c_get_clientdata(client);

	dev_info(&client->dev, "%s ...\n", __func__);

	return 0;
}

static int lt9611_resume(struct device *dev)
{
	struct i2c_client *client = to_i2c_client(dev);
	struct lt9611_data *data = i2c_get_clientdata(client);

	dev_info(&client->dev, "%s ...\n", __func__);

	return 0;
}

static SIMPLE_DEV_PM_OPS(lt9611_pm_ops, lt9611_suspend, lt9611_resume);
#define LT9611_PM_OPS (&lt9611_pm_ops)

#else
#define LT9611_PM_OPS NULL
#endif /* CONFIG_PM_SLEEP */

static const struct i2c_device_id lt9611_id[] = {
	{ "lt9611", 0 },
	{}
};
MODULE_DEVICE_TABLE(i2c, lt9611_id);

static const struct of_device_id lt9611_dt_ids[] = {
	{.compatible = "lontium,lt9611"},
	{},
};
MODULE_DEVICE_TABLE(of, lt9611_dt_ids);

static struct i2c_driver lt9611_driver = {
	.driver = {
		.name	= LT9611_DRV_NAME,
		.pm	= LT9611_PM_OPS,
		.of_match_table = lt9611_dt_ids,
	},
	.probe	= lt9611_probe,
	.remove	= lt9611_remove,
	.id_table = lt9611_id,
};

module_i2c_driver(lt9611_driver);

MODULE_AUTHOR("wh21 <wuhui@creality.com>");
MODULE_DESCRIPTION("LT9611 MIPI CSI/DSI to HDMI1.4 Bridge driver");
