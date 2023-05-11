/**
 *********************************************************************************************************
 *                                                AR100 SYSTEM
 *                                     AR100 Software System Develop Kits
 *                                                pmu module
 *
 *                                    (c) Copyright 2012-2016, Sunny China
 *                                             All Rights Reserved
 *
 * File    : pmu.c
 * By      : Sunny
 * Version : v1.0
 * Date    : 2012-5-22
 * Descript: power management unit.
 * Update  : date                auther      ver     notes
 *           2012-5-22 13:33:03  Sunny       1.0     Create this file.
 *********************************************************************************************************
 */

#include "ibase.h"
#include "driver.h"
#include "head_para.h"
#include "standby_power.h"

extern struct arisc_para arisc_para;
extern u32 axp_power_max;

static pin_typedef *vddsys_pinctrlx;

/**
 * here we use PL11 as vdd-sys power control pin,
 * by hard-coded instead of parsing arisc_para.
 * because vdd-sys controlled by pin is not common so far,
 * maybe we'll fix it later.
 */
static void pinctrl_powerdown(void)
{
	/* set PL11 as output function */
	pin_config vddsys_pinctrl_config = {0};

	vddsys_pinctrl_config.pin_grp = GPIO_GP_PL;
	vddsys_pinctrl_config.pin_fn  = GPIO_FN_1;
	vddsys_pinctrl_config.pin_driving = GPIO_MULTI_DRIVE_1;
	vddsys_pinctrl_config.pin_num = GPIO_PIN_11;
	vddsys_pinctrl_config.pin_exti_ctrl = GPIO_EXTI_DISABLE;

	pin_set(&vddsys_pinctrl_config, &vddsys_pinctrlx);

	/* powerdown by setting the pin output low level */
	pin_data_write(vddsys_pinctrlx, 0);

	LOG("pinctrl powerdown\n");
}

static void pinctrl_powerup(void)
{
	/**
	 * powerup by setting the pin disable, which means Hi-Z state.
	 * this absolutely depends on the board-level design.
	 */
	pin_disable(vddsys_pinctrlx);

	/* delay 5ms wait for power supply stable */
	time_mdelay(5);

	LOG("pinctrl powerup\n");
}

void standby_powerdown(void)
{
	u32 type;
	u32 standby_power;

	standby_power = arisc_para.vdd_cpua | arisc_para.vdd_cpub |
						arisc_para.vdd_sys | arisc_para.vcc_pll;
	LOG("standby_powerdown: 0x%x\n", standby_power);

	/* vdd-cpua powerdown */
	for (type = 0; type < axp_power_max; type++) {
		if ((arisc_para.vdd_cpua >> type) & 0x1)
			pmu_set_voltage_state(type, POWER_VOL_OFF);
	}

	/* vdd-cpub powerdown */
	for (type = 0; type < axp_power_max; type++) {
		if ((arisc_para.vdd_cpub >> type) & 0x1)
			pmu_set_voltage_state(type, POWER_VOL_OFF);
	}

	/* vcc-pll powerdown */
	for (type = 0; type < axp_power_max; type++) {
		if ((arisc_para.vcc_pll >> type) & 0x1)
			pmu_set_voltage_state(type, POWER_VOL_OFF);
	}

	/* vdd-sys powerdown */
	for (type = 0; type < axp_power_max; type++) {
		if ((arisc_para.vdd_sys >> type) & 0x1)
			pmu_set_voltage_state(type, POWER_VOL_OFF);
	}

	/**
	 * this may mean vdd_sys is controlled by gpio, but not by pmu.
	 * so we need to set a pin to power down vdd_sys.
	 */
	if (arisc_para.vdd_sys == 0)
		pinctrl_powerdown();

}

void standby_powerup(void)
{
	u32 type;

	/**
	 * this may mean vdd_sys is controlled by gpio, but not by pmu.
	 * so we need to set a pin to power up vdd_sys.
	 */
	if (arisc_para.vdd_sys == 0)
		pinctrl_powerup();

	/* vdd-sys powerup */
	for (type = 0; type < axp_power_max; type++) {
		if ((arisc_para.vdd_sys >> type) & 0x1)
			pmu_set_voltage_state(type, POWER_VOL_ON);
	}

	/* vcc-pll powerup */
	for (type = 0; type < axp_power_max; type++) {
		if ((arisc_para.vcc_pll >> type) & 0x1)
			pmu_set_voltage_state(type, POWER_VOL_ON);
	}

	/* vdd-cpub powerup */
	for (type = 0; type < axp_power_max; type++) {
		if ((arisc_para.vdd_cpub >> type) & 0x1)
			pmu_set_voltage_state(type, POWER_VOL_ON);
	}

	/* vdd-cpua powerup */
	for (type = 0; type < axp_power_max; type++) {
		if ((arisc_para.vdd_cpua >> type) & 0x1)
			pmu_set_voltage_state(type, POWER_VOL_ON);
	}

}
