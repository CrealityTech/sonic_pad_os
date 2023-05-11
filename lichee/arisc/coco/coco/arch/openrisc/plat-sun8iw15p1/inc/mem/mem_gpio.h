#ifndef __MEM_GPIO_H__
#define __MEM_GPIO_H__


#define GPIO_REG_LENGTH ((0x348 + 0x4) >> 2)

struct gpio_state{
	unsigned int gpio_reg_back[GPIO_REG_LENGTH];
};


int mem_gpio_save(void);

int mem_gpio_restore(void);
#endif /* __MEM_GPIO_SUN8IW12_H__ */
