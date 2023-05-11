#include "includes.h"
#include "mem.h"


int mem_gpio_save(void)
{

	mem_reg_save(system_back->gpio_back.gpio_reg_back, \
	             (const unsigned int *)(SUNXI_PIO_PBASE), GPIO_REG_LENGTH, 1);

	time_mdelay_irqdisable(1);
	return OK;
}

int mem_gpio_restore(void)
{

	mem_reg_restore((unsigned int *)(SUNXI_PIO_PBASE), \
	                (const unsigned int *)system_back->gpio_back.gpio_reg_back, GPIO_REG_LENGTH, 1);

	return OK;
}

