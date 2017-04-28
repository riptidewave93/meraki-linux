/*
 * Copyright (C) 2013, Broadcom Corporation. All Rights Reserved.
 * 
 * Permission to use, copy, modify, and/or distribute this software for any
 * purpose with or without fee is hereby granted, provided that the above
 * copyright notice and this permission notice appear in all copies.
 * 
 * THE SOFTWARE IS PROVIDED "AS IS" AND THE AUTHOR DISCLAIMS ALL WARRANTIES
 * WITH REGARD TO THIS SOFTWARE INCLUDING ALL IMPLIED WARRANTIES OF
 * MERCHANTABILITY AND FITNESS. IN NO EVENT SHALL THE AUTHOR BE LIABLE FOR ANY
 * SPECIAL, DIRECT, INDIRECT, OR CONSEQUENTIAL DAMAGES OR ANY DAMAGES
 * WHATSOEVER RESULTING FROM LOSS OF USE, DATA OR PROFITS, WHETHER IN AN ACTION
 * OF CONTRACT, NEGLIGENCE OR OTHER TORTIOUS ACTION, ARISING OUT OF OR IN
 * CONNECTION WITH THE USE OR PERFORMANCE OF THIS SOFTWARE.
 */

#ifndef __IPROC_PLAT_GPIO_H
#define __IPROC_PLAT_GPIO_H

#include <asm-generic/gpio.h>

#define IPROC_GPIO_REG_SIZE     (0x50)





#define REGOFFSET_GPIO_DIN          0x000 /* GPIO Data in register */
#define REGOFFSET_GPIO_DOUT         0x004 /* GPIO Data out register */
#define REGOFFSET_GPIO_EN           0x008 /* GPIO driver enable register */


#define IPROC_GPIO_CCA_ID   (0)
#define IPROC_GPIO_CCB_ID   (1)

#define IPROC_GPIO_CCA_IRQ_BASE     (IPROC_IRQ_GPIO_0)
#define IPROC_GPIO_CCB_IRQ_BASE     (IPROC_IRQ_GPIO_0 + 32)




#define IPROC_CCA_INT_F_GPIOINT     (1)

struct iproc_gpio_chip {
    int id;
    struct gpio_chip	chip;
    struct iproc_gpio_cfg	*config;
    void __iomem		*ioaddr;
    void __iomem		*intr_ioaddr;
    void __iomem		*dmu_ioaddr;
    spinlock_t		 lock;
    int irq_base;
	struct resource	* resource;
	int irq;
};


static inline struct iproc_gpio_chip *to_iproc_gpio(struct gpio_chip *gpc)
{
	return container_of(gpc, struct iproc_gpio_chip, chip);
}

#define IPROC_GPIO_END  (32 + 4)

extern struct iproc_gpio_chip *iproc_gpios[IPROC_GPIO_END];

static inline struct iproc_gpio_chip *iproc_gpiolib_getchip(unsigned int chip)
{
	return (chip < IPROC_GPIO_END) ? iproc_gpios[chip] : NULL;
}

/* locking wrappers to deal with multiple access to the same gpio bank */
#define iproc_gpio_lock(_oc, _fl) spin_lock_irqsave(&(_oc)->lock, _fl)
#define iproc_gpio_unlock(_oc, _fl) spin_unlock_irqrestore(&(_oc)->lock, _fl)

extern void iproc_gpiolib_add(struct iproc_gpio_chip *chip);
extern int iproc_gpiolib_init(void);

#define MAX_NS_GPIO 2

#endif
