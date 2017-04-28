
#ifndef __AM_IPP_GPIO_H__
#define __AM_IPP_GPIO_H__

#include <linux/types.h>
#include <linux/notifier.h>
#include <linux/spinlock.h>
#include <linux/io.h>
#include <linux/of.h>
#include <linux/of_gpio.h>
#include <linux/gpio.h>

struct ipp_gpio_regs {
    u8  pre[0xa90];
    u32 mpa_gpio_int_lvl;
    u32 mpa_gpio;
    u32 mpa_gpio_in;
    u8  pre_ioctl[0xd4];
    u32 gpio_ds_ioctl0;
    u32 gpio_ds_ioctl1;
    u32 gpio_ds_ioctl2;
    u32 gpio_ds_ioctl3;
};

struct ipp_gpio_chip {
    struct of_mm_gpio_chip mm_gc;
    spinlock_t lock;
};

#define GPIO_DS_MASK_VAL(gpio)		(0x0000001E << (gpio*8))

#endif //__AM_IPP_GPIO_H__
