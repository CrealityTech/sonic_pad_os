/*
*********************************************************************************************************
*                                                AR100 SYSTEM
*                                     AR100 Software System Develop Kits
*                                                 AVS module
*
*                                    (c) Copyright 2017-2021, HuSan China
*                                             All Rights Reserved
*
* File    : dvfs.c
* By      : lvtsanhu@allwinnertech.com
* Version : v1.0
* Date    : 2017-12-30
* Descript: AVS module.
* Update  : date                auther      ver     notes
*           2018-4-4  11:21:55  HuSan       1.0     Create this file.
*           2018-6-22 13:42:42  Frank       1.1     Update the support of dvfs.
*********************************************************************************************************
*/

#include "ibase.h"
#include "head_para.h"
#include "task.h"
#include "task_handle.h"
#include "msgmanager.h"
#include "cpucfg.h"
#include "psci.h"
#include "wakeup_source.h"
#include "notifier.h"
#include "softtimer.h"
#include "dram.h"
#include "driver.h"
#include "dvfs_service.h"

static int vf_nums;
#if AVS_USED
#define VF_TABLE_SIZE (sizeof(struct freq_voltage_avs) * DVFS_VF_TABLE_MAX)
//record the current freq and voltage
struct freq_voltage_avs current_vf;
struct freq_voltage_avs vf_table[DVFS_VF_TABLE_MAX];
u32 cpu_output;
u32 min_p_value = 65535;

#else	//NONE AVS
//record the current freq and voltage
#define VF_TABLE_SIZE (sizeof(struct freq_voltage) * DVFS_VF_TABLE_MAX)
struct freq_voltage current_vf;
struct freq_voltage vf_table[DVFS_VF_TABLE_MAX];
u32 cpu_output;
#endif

extern struct arisc_para arisc_para;
static ccu_device *ccmu;

__TASK_INITIAL(psci_set_dvfs_task, DEFAULT_TASK_PRIORITY,TASK_GID_PSCI, ARISC_CPUX_DVFS_REQ);
__TASK_INITIAL(psci_set_dvfs_table_task, DEFAULT_TASK_PRIORITY,TASK_GID_PSCI, ARISC_CPUX_DVFS_CFG_VF_REQ);


#if AVS_USED
/*
*********************************************************************************************************
*                                       GET CURRENT P-SENSOR VALUE
*
* Description:  get current p-sensor value module.
*
* Arguments  :  none.
*
* Returns    :  return the current p-sensor u32 value.
*********************************************************************************************************
*/
static u32 get_p_parameter(void)
{
	u32 rdata;
	u8 times;
	u32 count_all=0;

	/*set delay cycle*/
	writel((P_SENSOR_DELAY | P_SENSOR_MAX_TIME), PS_CTRL_REG);

	/*select device*/
	rdata = readl(PS_CTRL_REG) | (0 << 8);
	writel(rdata, PS_CTRL_REG);

	/*select OSC: */
	rdata = (readl(PS_CTRL_REG) &~ (0x3 << 4)) | (1 << 4);
	writel(rdata, PS_CTRL_REG);

	for(times =0; times < 16; times++) {

		writel(readl(PS_CTRL_REG) | (1 << 0), PS_CTRL_REG);
		do {
			rdata = readl(PS_CTRL_REG) & (1 << 7);
		} while(!rdata);

		writel(readl(PS_CTRL_REG) | (1 << 7), PS_CTRL_REG);

		count_all += readl(PS_CNT_REG);

		writel(readl(PS_CTRL_REG) &~ (1 << 0), PS_CTRL_REG);
	}

	return ((u32)(count_all/16));
}



/*
*********************************************************************************************************
*                                       SET FREQ
*
* Description:  set the frequency of a specical module.
*
* Arguments  :  freq    : the frequency which we want to set.
*
* Returns    :  OK if set frequency succeeded, others if failed.
*********************************************************************************************************
*/
s32 dvfs_set_freq(u32 freq)
{
	struct freq_voltage_avs *vf_inf;
	u32 current_voltemp = 0;
	u32 cur_p = 0;
	u32 step_flag = 0;
	u32 step_value = 1;
	u32 steps = 0;
	u32 mdelay = 0;

	/* initialize message
	 *
	 * |paras[0]|
	 * |freq    |
	 */

	/* switch to stable clk src */
	ccmu_set_freq(ccmu, CPU_AXI_CFG_REG, CCMU_SRC_STABLE);

	//freq base on khz
	freq = freq * 1000;

	//find target voltage and p
	vf_inf = &vf_table[0];

	INF("target_freq = %d\n", freq);
	if (freq >= vf_inf->freq)
		freq = vf_inf->freq;
	else if (freq <= vf_table[vf_nums - 1].freq)
		freq = vf_table[vf_nums - 1].freq;

	while((vf_inf + 1)->freq >= freq)
	{
		INF("f:%u,v:%u\n", vf_inf->freq, vf_inf->voltage);
		vf_inf++;
	}
	INF("target_freq = %d, vf_inf->freq = %d, current_vf.voltage = %d, vf_inf->voltage %d\n",
			freq, vf_inf->freq, current_vf.voltage, vf_inf->voltage);

	/* try to increase voltage first */
	if (vf_inf->voltage > current_vf.voltage)
	{
		INF("current_vf.freq:%u < target freq:%u\n", current_vf.freq, freq);
		INF("Increasing freq process, starting increasing voltage.\n");

		//calc adjust voltage steps, DCDC3 voltage step is 20mV
		steps = (vf_inf->voltage - current_vf.voltage) / 20;

		//SOC->CPU0_VDD = PMU->DCDC3.
		if (pmu_set_voltage(cpu_output, vf_inf->voltage) != OK)
		{
			//dvfs failed, feedback the failed result to cpu0.
			ERR("InVoltFail\n");
			return -EFAIL;
		}

		mdelay = ((steps * PMU_DCDC3_STEP_DELAY_US) + 1000 - 1) / 1000;
		time_mdelay(mdelay);

		//update current voltage
		current_vf.voltage = pmu_get_voltage(cpu_output);
		if (current_vf.voltage != vf_inf->voltage)
		{
			//if current_vf.voltage != target voltage
			//just return FAIL, and not continue scaling voltage
			ERR("cv %u != tv %u\n", current_vf.voltage, vf_inf->voltage);
			ERR("cur_volt != targ_volt\n");
			return -EFAIL;
		}
	}

	//try to adjust CPU0 target freq
	if (freq != current_vf.freq)
	{
		INF("change: current_vf.freq:%u < target freq:%u\n", current_vf.freq, freq);
		//try to increase axi divide ratio
		if (vf_inf->axi_div > current_vf.axi_div)
		{
			//the axi_div of target freq should be increase first,
			//this is to avoid axi bus clock beyond limition.
			ccmu_set_m_div(CPU_AXI_CFG_REG, vf_inf->axi_div);
			current_vf.axi_div = ccmu_get_m_div(CPU_AXI_CFG_REG);
		}
		//adjust PLL1 clock, this interface internal will wait PLL1 stable
		ccu_set_sclk_freq(freq);
		current_vf.freq = ccu_get_sclk_freq();

		//try to decrease axi divide ratio
		if (vf_inf->axi_div < current_vf.axi_div)
		{
			//the axi_div of target freq should be increase first,
			//this is to avoid axi bus clock beyond limition.
			ccmu_set_m_div(CPU_AXI_CFG_REG, vf_inf->axi_div);
			current_vf.axi_div = ccmu_get_m_div(CPU_AXI_CFG_REG);
		}
	}

	//try to decrease CPU0_VDD
	if (vf_inf->voltage < current_vf.voltage)
	{
		INF("current_vf.freq:%u < target freq:%u\n", current_vf.freq, freq);
		INF("Decrease freq process, starting decreasing voltage.\n");
		//SOC->CPU0_VDD = PMU->DCDC3.
		if (pmu_set_voltage(cpu_output, vf_inf->voltage) != OK)
		{
			//dvfs failed, feedback the failed result to cpu0.
			ERR("dvfs try to decrease voltage failed\n");
			return -EFAIL;
		}

		//add 1m delay to wait dvm scaling voltage
		time_mdelay(1);

		//update current voltage
		current_vf.voltage = pmu_get_voltage(cpu_output);
		if (current_vf.voltage != vf_inf->voltage)
		{
			//if current_vf.voltage != target voltage
			//just return FAIL, and not continue scaling voltage
			ERR("cv %u != tv %u\n", current_vf.voltage, vf_inf->voltage);
			ERR("cur_volt != targ_volt\n");
			return -EFAIL;
		}
	}

	current_voltemp = current_vf.voltage;
	cur_p = get_p_parameter();
	INF("c_p=%u,c_v=%u,c_f=%u\n", cur_p, current_vf.voltage, current_vf.freq);


	while (cur_p > (vf_inf->pval+ RADIUS_P))
	{
		step_value = 1;
		INF("cur_p:%u > target_p:%u + R:%u\n", cur_p, vf_inf->pval, RADIUS_P);
		if (step_flag == 0)
		{
			step_flag = 1;
			step_value = (cur_p - vf_inf->pval) / P_STEP;
			if (step_value == 0)
				step_value = 1;
		}
		current_voltemp -= VOLT_STEP * step_value;
		if (current_voltemp < MIN_VOLTAGE)
		{
			if (pmu_set_voltage(cpu_output, MIN_VOLTAGE) != OK)
			{
				ERR("InVoltFail\n");
				return -EFAIL;
			}
			time_udelay(PMU_DCDC3_STEP_DELAY_US);
			current_vf.voltage = pmu_get_voltage(cpu_output);
			if (current_vf.voltage != MIN_VOLTAGE)
			{
				//if current_vf.voltage != target voltage
				//just return FAIL, and not continue scaling voltage
				ERR("cur_volt != targ_volt\n");
				return -EFAIL;
			}
			INF("AVS:set min_volt:%u ok!\n", MIN_VOLTAGE);
			return OK;
		}
		if (pmu_set_voltage(cpu_output, current_voltemp) != OK)
		{
			ERR("InVoltFail\n");
			return -EFAIL;
		}
		time_udelay(PMU_DCDC3_STEP_DELAY_US);
		current_vf.voltage = pmu_get_voltage(cpu_output);
		if (current_vf.voltage != current_voltemp)
		{
			//if current_vf.voltage != target voltage
			//just return FAIL, and not continue scaling voltage
			ERR("current voltage != target voltage\n");
			return -EFAIL;
		}
		cur_p = get_p_parameter();

		INF("AVS:cur_p=%u,cur_vol=%u\n", cur_p, current_vf.voltage);

	}

	/* switch to stable cpu_c0_pll */
	ccmu_set_freq(ccmu, CPU_AXI_CFG_REG, CCMU_SRC_PLL);

	INF("DVFS succeed, freq = %u, voltage = %u, axi_div = %u, pval = %u\n",
		current_vf.freq, current_vf.voltage, current_vf.axi_div, current_vf.pval);
	//dvfs succeeded, feedback the succeeded result to cpu0.

	return OK;
}
#else	//NONE AVS

/*
**********************************************************************************************************
**                                       SET FREQ
**
** Description:  set the frequency of a specical module.
**
** Arguments  :  freq    : the frequency which we want to set.
**
** Returns    :  OK if set frequency succeeded, others if failed.
**********************************************************************************************************
**/
s32 dvfs_set_freq(u32 freq)
{
	struct freq_voltage *vf_inf;
	u32 current_voltage = 0;
	/* initialize message
	 *
	 * |paras[0]|
	 * |freq    |
	 */

	//switch to stable clk src
	ccmu_set_freq(ccmu, CPU_AXI_CFG_REG, CCMU_SRC_STABLE);
	//writel((readl(0x03001500) & (0xf8ffffff)) | (4 << 24), 0x03001500);

	freq = freq * 1000;

	//find target voltage
	vf_inf = &vf_table[0];

	if (freq >= vf_inf->freq)
		freq = vf_inf->freq;
	else if (freq <= vf_table[vf_nums - 1].freq)
		freq = vf_table[vf_nums - 1].freq;

	while((vf_inf + 1)->freq >= freq)
	{
		INF("f:%u,v:%u\n", vf_inf->freq, vf_inf->voltage);
		vf_inf++;
	}
	INF("target_freq = %d, vf_inf->freq = %d, current_vf.voltage = %d, vf_inf->voltage %d\n",
			freq, vf_inf->freq, current_vf.voltage, vf_inf->voltage);

	//try to increase voltage first.
	if (vf_inf->voltage > current_vf.voltage)
	{
		//calc adjust voltage steps, DCDC3 voltage step is 20mV
		u32 steps = (vf_inf->voltage - current_vf.voltage) / 20;
		u32 mdelay;

		//SOC->CPU0_VDD = PMU->DCDC3.
		if (pmu_set_voltage(cpu_output, vf_inf->voltage) != OK)
		{
			//dvfs failed, feedback the failed result to cpu0.
			ERR("InVoltFail\n");
			return -EFAIL;
		}

		mdelay = ((steps * PMU_DCDC3_STEP_DELAY_US) + 1000 - 1) / 1000;
		time_mdelay(mdelay);

		//update current voltage
		current_voltage = pmu_get_voltage(cpu_output);
		if (current_voltage != vf_inf->voltage)
		{
			//if current voltage != target voltage
			//just return FAIL, and not continue scaling voltage
			ERR("current voltage != target voltage\n");
			return -EFAIL;
		}
		INF("current voltage = %d,  target voltage = %d\n", current_voltage, vf_inf->voltage);
	}

	//try to adjust CPU0 target freq
	if (freq != current_vf.freq)
	{
		//try to increase axi divide ratio
		if (vf_inf->axi_div > current_vf.axi_div)
		{
			//the axi_div of target freq should be increase first,
			//this is to avoid axi bus clock beyond limition.
			ccmu_set_m_div(CPU_AXI_CFG_REG, vf_inf->axi_div);
			current_vf.axi_div = ccmu_get_m_div(CPU_AXI_CFG_REG);
		}
		//adjust PLL1 clock, this interface internal will wait PLL1 stable
		//ccu_set_sclk_freq(STABLE_FREQ);
		ccu_set_sclk_freq(freq);
		current_vf.freq = ccu_get_sclk_freq();

		//try to decrease axi divide ratio
		if (vf_inf->axi_div < current_vf.axi_div)
		{
			//the axi_div of target freq should be increase first,
			//this is to avoid axi bus clock beyond limition.
			ccmu_set_m_div(CPU_AXI_CFG_REG, vf_inf->axi_div);
			current_vf.axi_div = ccmu_get_m_div(CPU_AXI_CFG_REG);
		}
	}

	//try to decrease CPU0_VDD
	if (vf_inf->voltage < current_vf.voltage)
	{
		//SOC->CPU0_VDD = PMU->DCDC3.
		if (pmu_set_voltage(cpu_output, vf_inf->voltage) != OK)
		{
			//dvfs failed, feedback the failed result to cpu0.
			ERR("dvfs try to decrease voltage failed\n");
			return -EFAIL;
		}

		//add 1m delay to wait dvm scaling voltage
		time_mdelay(1);

		//update current voltage
		current_voltage = pmu_get_voltage(cpu_output);
		if (current_voltage != vf_inf->voltage)
		{
			//if current voltage != target voltage
			//just return FAIL, and not continue scaling voltage
			ERR("cv %u != tv %u\n", current_voltage, vf_inf->voltage);
			return -EFAIL;
		}
		INF("current voltage = %d,  target voltage = %d\n", current_voltage, vf_inf->voltage);
	}

	if (vf_inf->voltage != current_vf.voltage)
	{
		current_vf.voltage = current_voltage;
	}

	//switch to stable cpu_c0_pll
	ccmu_set_freq(ccmu, CPU_AXI_CFG_REG, CCMU_SRC_PLL);
	//writel((readl(0x03001500) & (0xf8ffffff)) | (3 << 24), 0x03001500);
	//dvfs succeeded, feedback the succeeded result to cpu0.
	LOG("DVFS succeed, freq = %u, voltage = %u, axi_div = %u\n",
		current_vf.freq, current_vf.voltage, current_vf.axi_div);
	return OK;
}
#endif

s32 dvfs_config_vf_table(u32 vf_num, u32 vf_addr)
{
	u32 vf_size;
	int i = 0;
#if  AVS_USED
	vf_size = vf_num * sizeof(struct freq_voltage_avs);
#else
	vf_size = vf_num * sizeof(struct freq_voltage);
#endif
	memset(vf_table, 0, VF_TABLE_SIZE);
	memcpy(vf_table, (void *)vf_addr, vf_size);
	vf_nums = vf_num;
	for (i = 0; i < DVFS_VF_TABLE_MAX; i++)
		LOG("%s:table[%d].freq = %d, .vol = %d, .axi_div = %d\n",
			__func__, i, vf_table[i].freq, vf_table[i].voltage, vf_table[i].axi_div);
/*#if  AVS_USED
		LOG("%s:table[%d].freq = %d, .vol = %d, .axi_div = %d, .pval = %d\n",
			__func__, i, vf_table[i].freq, vf_table[i].voltage, vf_table[i].axi_div, vf_table[i].pval);
#endif*/
	return OK;
}

static s32 psci_set_dvfs_task_work(task_struct *task)
{
	unsigned int paras[5] = {0};
	unsigned int *para_p = NULL;
	unsigned int target_freq = 0;

	para_p = paras;
	get_paras_form_msg((struct list_head *)task->msgparalist, (void **)(&para_p), 2 * sizeof(int));

	/*for (i = 0; i < 2; i++) {
		LOG("%s:para[%d] = %d\n", __func__, i, paras[i]);
	}*/
	target_freq = paras[0];
	dvfs_set_freq(target_freq);
	return OK;
}

static s32 psci_set_dvfs_table_task_work(task_struct *task)
{
	unsigned int paras[5] = {0};
	unsigned int *para_p = NULL;
	unsigned int vf_num, vf_addr;

	para_p = paras;
	get_paras_form_msg((struct list_head *)task->msgparalist, (void **)(&para_p), 3 * sizeof(int));

	/*for (i = 0; i < 3; i++) {
		LOG("%s:para[%d] = %d\n", __func__, i, paras[i]);
	}*/
	vf_num  = paras[1];
	vf_addr = paras[2];
	dvfs_config_vf_table(vf_num, vf_addr);

	return OK;
}
/*
*********************************************************************************************************
*                                       INIT DVFS
*
* Description:  initialize dvfs module.
*
* Arguments  :  none.
*
* Returns    :  OK if initialize succeeded, others if failed.
*********************************************************************************************************
*/
s32 dvfs_init(void)
{
	u32 i;

	//initialize current freq.
	current_vf.freq = ccu_get_sclk_freq();

	/**
	 * SOC->CPU0_VDD = PMU->DCDC3
	 * parse vdd-cpu power ID from arisc para
	 */
	for (i = 0; i < 32; i++) {
		if (!!(arisc_para.vdd_cpua & (0x1 << i)))
			break;
	}
	cpu_output = i;
	LOG("dvfs_cpu_output:%d\n", cpu_output);

	current_vf.voltage = pmu_get_voltage(cpu_output);
	//initialize axi bus divide ratio
	current_vf.axi_div = ccmu_get_m_div(CPU_AXI_CFG_REG);
#if AVS_USED
	//vf_table = arisc_para.vf;
#else	//NONE AVS
#endif

	INF("freq   :%d\n", current_vf.freq);
	INF("voltage:%d\n", current_vf.voltage);
	INF("axi_div:%d\n", current_vf.axi_div);

	ccmu = get_ccmu_device();

	add_task(&psci_set_dvfs_task, (__taskCBK_t)psci_set_dvfs_task_work);
	add_task(&psci_set_dvfs_table_task, (__taskCBK_t)psci_set_dvfs_table_task_work);
	return OK;
}

s32 dvfs_exit(void)
{
	return OK;
}
