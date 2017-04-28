
#ifndef _ATHEROS_GPIO_H_
#define _ATHEROS_GPIO_H_

#ifndef ATHEROS_GPIO_BASE
#define ATHEROS_GPIO_BASE 0
#endif

#define ATHEROS_NUM_GPIOS 23

#ifdef CONFIG_GPIOLIB

#define gpio_get_value		__gpio_get_value
#define gpio_set_value		__gpio_set_value
#define gpio_cansleep		__gpio_cansleep
#define gpio_to_irq		__gpio_to_irq

#include <asm-generic/gpio.h>

#else

extern void ath_gpio_out_val(int gpio, int val);
extern int ath_gpio_in_val(int gpio);
extern int ath_gpio_config_output(int gpio, int val);
extern int ath_gpio_config_input(int gpio);


static inline int gpio_direction_input(int gpio)
{
	return ath_gpio_config_input(gpio);
}

static inline int gpio_direction_output(int gpio, int v)
{
	return ath_gpio_config_output(gpio, v);
}

static inline int gpio_get_value(int gpio)
{
	return ath_gpio_in_val(gpio);
}

static inline void gpio_set_value(int gpio, int v)
{
	ath_gpio_out_val(gpio, v);
}

static inline int gpio_cansleep(int gpio)
{
	return 0;
}

static inline int gpio_to_irq(int gpio)
{
	return ATH_GPIO_IRQn(gpio);
}

static inline int gpio_set_value_cansleep(unsigned gpio, int val)
{
	return 0;
}

static inline int gpio_is_valid(unsigned gpio)
{
	return (gpio - ATHEROS_GPIO_BASE) < ATHEROS_NUM_GPIOS;
}

static inline int gpio_request(unsigned gpio, const char *label)
{
	return 0;
}

static inline void gpio_free(unsigned gpio)
{
}


#endif //CONFIG_GPIOLIB
#endif //_ATHEROS_GPIO_H_


