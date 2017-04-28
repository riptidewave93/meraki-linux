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
 
#include <linux/kernel.h>
#include <linux/init.h>
#include <linux/module.h>
#include <linux/irq.h>
#include <linux/interrupt.h>
#include <linux/io.h>
#include <linux/ioport.h>
#include <linux/version.h>

#include <mach/iproc_regs.h>
#include <mach/reg_utils.h>
#include <mach/io_map.h>
#include <mach/gpio_cfg.h>
#include <mach/gpio.h>

#if (defined(CONFIG_MACH_NS) || defined(CONFIG_MACH_NSP))
static struct iproc_gpio_cfg iproc_gpiocfg = {
    .get_pull = iproc_gpio_getpull_updown,
    .set_pull = iproc_gpio_setpull_updown,
    .get_config	= iproc_gpio_get_config,
    .set_config	= iproc_gpio_set_config,    
};

#endif /*CONFIG_MACH_NS || CONFIG_MACH_NSP */

static struct resource iproc_gpio_cca_config_resource[] = {
    [0] = {
		.start	= IPROC_CCA_BASE,
		.end	= IPROC_CCA_BASE + IPROC_GPIO_REG_SIZE - 1,
		.flags	= IORESOURCE_MEM,
		.name   = "intr",
    },
#if (defined(CONFIG_MACH_NS) || defined(CONFIG_MACH_NSP))
    [1] = {
		.start	= IPROC_DMU_BASE,
		.end	= IPROC_DMU_BASE + 0x200 - 1,
		.flags	= IORESOURCE_MEM,
		.name   = "dmu",
    },
#else
    [1] = {.name = "",},
#endif
};

static struct resource iproc_gpio_resources[] = {
	[0] = {
		.start	= IPROC_GPIO_CCA_BASE,
		.end	= IPROC_GPIO_CCA_BASE + IPROC_GPIO_REG_SIZE - 1,
		.flags	= IORESOURCE_MEM,
		.child = iproc_gpio_cca_config_resource,
	},
	[1] = {
		.start	= IPROC_GPIO_CCB_BASE,
		.end	= IPROC_GPIO_CCB_BASE + IPROC_GPIO_REG_SIZE -1,
		.flags	= IORESOURCE_MEM,
	}
};
#if defined(CONFIG_MACH_NS) || defined(CONFIG_MACH_NSP)

struct iproc_gpio_chip iproc_gpios_config[] = {
    [0] = {
        .id   = IPROC_GPIO_CCA_ID,
        .config = &iproc_gpiocfg,
        .chip   = {
            .base           = 0,
            .label          = "GPIOA",
            .ngpio          = 24,
        },
        .irq_base = IPROC_GPIO_CCA_IRQ_BASE,
        .resource = &iproc_gpio_resources[0],
        .irq = IPROC_GPIO_CCA_INT,
    },
    [1] = {
        .id   = IPROC_GPIO_CCB_ID,
        .config = &iproc_gpiocfg,
        .chip   = {
            .base           = -EINVAL,
            .label          = "GPIOB",
            .ngpio          = 4,
        },
        .irq_base = IPROC_GPIO_CCB_IRQ_BASE,
        .resource = &iproc_gpio_resources[1],
        .irq = IPROC_GPIO_CCB_INT,
    },
};
/* CONFIG_MACH_NS */
#elif defined(CONFIG_MACH_HX4) || defined(CONFIG_MACH_KT2) 
/* 
 * Chip level GPIO 0-3 from CMICD, 
 * GPIO 4-11 from ChipcommonA 
 * Hence the base is 4 and the number is 8.
 */
struct iproc_gpio_chip iproc_gpios_config[] = {
    [0] = {
        .id   = IPROC_GPIO_CCA_ID,
        .chip   = {
            .base           = 4,
            .label          = "GPIOA",
            .ngpio          = 8,
        },
        .irq_base = IPROC_GPIO_CCA_IRQ_BASE,
        .resource = &iproc_gpio_resources[0],
        .irq = IPROC_GPIO_CCA_INT,
    },
};
#elif defined(CONFIG_MACH_HR2)  
/* 
 * Chip level GPIO 0-3 from CMICD, 
 * GPIO 4-15 are from ChipcommonA
 * where GPIO 8-15 are shared with MII or LED depends on strap pin
 * Hence the base is 4 and the number is 12.
 */
struct iproc_gpio_chip iproc_gpios_config[] = {
    [0] = {
        .id   = IPROC_GPIO_CCA_ID,
        .chip   = {
            .base           = 4,
            .label          = "GPIOA",
            .ngpio          = 12,
        },
        .irq_base = IPROC_GPIO_CCA_IRQ_BASE,
        .resource = &iproc_gpio_resources[0],
        .irq = IPROC_GPIO_CCA_INT,
    },
};
#else
struct iproc_gpio_chip iproc_gpios_config[] = {
    [0] = {
        .id   = IPROC_GPIO_CCA_ID,
        .chip   = {
            .base           = 0,
            .label          = "GPIOA",
            .ngpio          = 32,
        },
        .irq_base = IPROC_GPIO_CCA_IRQ_BASE,
        .resource = &iproc_gpio_resources[0],
        .irq = IPROC_GPIO_CCA_INT,
    },
    [1] = {
        .id   = IPROC_GPIO_CCB_ID,
        .chip   = {
            .base           = -EINVAL,
            .label          = "GPIOB",
            .ngpio          = 4,
        },
        .irq_base = IPROC_GPIO_CCB_IRQ_BASE,
        .resource = &iproc_gpio_resources[1],
        .irq = IPROC_GPIO_CCB_INT,
    },
};
#endif 


static struct iproc_gpio_chip *iproc_gpio_dev[MAX_NS_GPIO] = {};
static int dev = 0;

static unsigned int _iproc_gpio_readl(struct iproc_gpio_chip *chip, int reg)
{
    return readl(chip->ioaddr + reg);
}

static void _iproc_gpio_writel(struct iproc_gpio_chip *chip, unsigned int val, int reg)
{
	writel(val, chip->ioaddr + reg);
}


static int iproc_gpio_to_irq(struct iproc_gpio_chip *chip, unsigned int pin) {
    return (chip->irq_base + pin);
}

static int iproc_irq_to_gpio(struct iproc_gpio_chip *chip, unsigned int irq) {
    return (irq - chip->irq_base);
}
static void iproc_gpio_irq_ack(struct irq_data *d)
{
    unsigned int irq = d->irq;
    struct iproc_gpio_chip *ourchip = irq_get_chip_data(irq);
    int pin;

    pin = iproc_irq_to_gpio(ourchip, irq);

    if (ourchip->id == IPROC_GPIO_CCA_ID) {
        unsigned int  event_status, irq_type;

        event_status = 0;
        irq_type = irq_desc[irq].status_use_accessors & IRQ_TYPE_SENSE_MASK;
        if (irq_type & IRQ_TYPE_EDGE_BOTH) 
        {
            event_status |= (1 << pin);            
            _iproc_gpio_writel(ourchip, event_status,
                IPROC_GPIO_CCA_INT_EVENT);
        }

    }

    if (ourchip->id == IPROC_GPIO_CCB_ID) {
        unsigned int int_clear = 0;

        int_clear |= (1 << pin);
        _iproc_gpio_writel(ourchip, int_clear, IPROC_GPIO_CCB_INT_CLR);

    }
}
static void iproc_gpio_irq_unmask(struct irq_data *d)
{
    unsigned int irq = d->irq;
    struct iproc_gpio_chip *ourchip = irq_get_chip_data(irq);
    int pin;
    unsigned int int_mask, irq_type;

    pin = iproc_irq_to_gpio(ourchip, irq);
    irq_type = irq_desc[irq].status_use_accessors & IRQ_TYPE_SENSE_MASK;

    if (ourchip->id == IPROC_GPIO_CCA_ID) {
        unsigned int  event_mask; 

        event_mask = _iproc_gpio_readl(ourchip, IPROC_GPIO_CCA_INT_EVENT_MASK);
        int_mask = _iproc_gpio_readl(ourchip, IPROC_GPIO_CCA_INT_LEVEL_MASK);

        if (irq_type & IRQ_TYPE_EDGE_BOTH) {
            event_mask |= 1 << pin;
            _iproc_gpio_writel(ourchip, event_mask, 
                IPROC_GPIO_CCA_INT_EVENT_MASK);
        } else {
            int_mask |= 1 << pin;
            _iproc_gpio_writel(ourchip, int_mask, 
                IPROC_GPIO_CCA_INT_LEVEL_MASK);
        }
    }

    if (ourchip->id == IPROC_GPIO_CCB_ID) {
        int_mask = _iproc_gpio_readl(ourchip, IPROC_GPIO_CCB_INT_MASK);
        int_mask |= (1 << pin);
        _iproc_gpio_writel(ourchip, int_mask, IPROC_GPIO_CCB_INT_MASK);
    }

}
static void iproc_gpio_irq_mask(struct irq_data *d)
{
    unsigned int irq = d->irq;
    struct iproc_gpio_chip *ourchip = irq_get_chip_data(irq);
    int pin;
    unsigned int irq_type, int_mask;

    pin = iproc_irq_to_gpio(ourchip, irq);
    irq_type = irq_desc[irq].status_use_accessors & IRQ_TYPE_SENSE_MASK;

    if (ourchip->id == IPROC_GPIO_CCA_ID) {
        unsigned int  event_mask;
        
        event_mask = _iproc_gpio_readl(ourchip, IPROC_GPIO_CCA_INT_EVENT_MASK);
        int_mask = _iproc_gpio_readl(ourchip, IPROC_GPIO_CCA_INT_LEVEL_MASK);

        if (irq_type & IRQ_TYPE_EDGE_BOTH) {
            event_mask &= ~(1 << pin);
            _iproc_gpio_writel(ourchip, event_mask,
                IPROC_GPIO_CCA_INT_EVENT_MASK);
        } else {
            int_mask &= ~(1 << pin);
            _iproc_gpio_writel(ourchip, int_mask, 
                IPROC_GPIO_CCA_INT_LEVEL_MASK);
        }
    }

    if (ourchip->id == IPROC_GPIO_CCB_ID) {
        int_mask = _iproc_gpio_readl(ourchip, IPROC_GPIO_CCB_INT_MASK);
        int_mask &= ~(1 << pin);
        _iproc_gpio_writel(ourchip, int_mask,IPROC_GPIO_CCB_INT_MASK);
    }


}
static int iproc_gpio_irq_set_type(struct irq_data *d, unsigned int type)
{
    unsigned int irq = d->irq;
    struct iproc_gpio_chip *ourchip = irq_get_chip_data(irq);
    int pin;    


    pin = iproc_irq_to_gpio(ourchip, irq);

    if (ourchip->id == IPROC_GPIO_CCA_ID) {
        unsigned int  event_pol, int_pol;

        switch (type & IRQ_TYPE_SENSE_MASK) {
        case IRQ_TYPE_EDGE_RISING:
            event_pol = _iproc_gpio_readl(ourchip, IPROC_GPIO_CCA_INT_EDGE);
            event_pol &= ~(1 << pin);
            _iproc_gpio_writel(ourchip, event_pol, IPROC_GPIO_CCA_INT_EDGE);
            break;
        case IRQ_TYPE_EDGE_FALLING:
            event_pol = _iproc_gpio_readl(ourchip, IPROC_GPIO_CCA_INT_EDGE);
            event_pol |= (1 << pin);
            _iproc_gpio_writel(ourchip, event_pol, IPROC_GPIO_CCA_INT_EDGE);
            break;
        case IRQ_TYPE_LEVEL_HIGH:
            int_pol = _iproc_gpio_readl(ourchip, IPROC_GPIO_CCA_INT_LEVEL);
            int_pol &= ~(1 << pin);
            _iproc_gpio_writel(ourchip, int_pol, IPROC_GPIO_CCA_INT_LEVEL);
            break;
        case IRQ_TYPE_LEVEL_LOW:
            int_pol = _iproc_gpio_readl(ourchip,IPROC_GPIO_CCA_INT_LEVEL);
            int_pol |= (1 << pin);
            _iproc_gpio_writel(ourchip, int_pol, IPROC_GPIO_CCA_INT_LEVEL);
            break;
        default:
            printk(KERN_ERR "unsupport irq type !\n");
            return -EINVAL;
        }
    }

    if (ourchip->id == IPROC_GPIO_CCB_ID) {
        unsigned int  int_type, int_de, int_edge;
        int_type = _iproc_gpio_readl(ourchip, IPROC_GPIO_CCB_INT_TYPE);
        int_edge = _iproc_gpio_readl(ourchip, IPROC_GPIO_CCB_INT_EDGE);
        switch (type) {
            case IRQ_TYPE_EDGE_BOTH:
                int_type &= ~(1 << pin); 
                int_de = _iproc_gpio_readl(ourchip, IPROC_GPIO_CCB_INT_DE);
                int_de |= (1 << pin); 
                _iproc_gpio_writel(ourchip, int_de, IPROC_GPIO_CCB_INT_DE);
                break;
            case IRQ_TYPE_EDGE_RISING:
                int_type &= ~(1 << pin); 
                int_edge |= (1 << pin); 
                
                int_de = _iproc_gpio_readl(ourchip, IPROC_GPIO_CCB_INT_DE);
                int_de  &= ~(1 << pin); 
                _iproc_gpio_writel(ourchip, int_de, IPROC_GPIO_CCB_INT_DE);
                break;
            case IRQ_TYPE_EDGE_FALLING:
                int_type &= ~(1 << pin);
                int_edge &= ~(1 << pin);
                
                int_de = _iproc_gpio_readl(ourchip, IPROC_GPIO_CCB_INT_DE);
                int_de  &= ~(1 << pin); 
                _iproc_gpio_writel(ourchip, int_de, IPROC_GPIO_CCB_INT_DE);
                break;
            case IRQ_TYPE_LEVEL_HIGH:
                int_type |= (1 << pin); 
                int_edge |= (1 << pin); 
                break;
            case IRQ_TYPE_LEVEL_LOW:
                int_type |= (1 << pin); 
                int_edge &= ~(1 << pin); 
                break;
            default:
                printk(KERN_ERR "unsupport irq type !\n");
                return -EINVAL;
        }
        _iproc_gpio_writel(ourchip, int_type, IPROC_GPIO_CCB_INT_TYPE);
        _iproc_gpio_writel(ourchip, int_edge, IPROC_GPIO_CCB_INT_EDGE);
    }

    return 0;
}


static irqreturn_t 
iproc_gpio_irq_handler_cca(int irq, void *dev)

{
    unsigned int  val, irq_type;
    unsigned int  int_mask, int_pol, in;
    unsigned int  event_mask, event, event_pol, tmp = 0;
    int iter, g_irq;
    struct iproc_gpio_chip *ourchip = dev;


    val = readl(ourchip->intr_ioaddr + IPROC_CCA_INT_STS);
    
    if (val & IPROC_CCA_INT_F_GPIOINT) {
        int_mask = _iproc_gpio_readl(ourchip, IPROC_GPIO_CCA_INT_LEVEL_MASK);
        int_pol = _iproc_gpio_readl(ourchip, IPROC_GPIO_CCA_INT_LEVEL);
        in = _iproc_gpio_readl(ourchip, IPROC_GPIO_CCA_DIN);
        event_mask = _iproc_gpio_readl(ourchip, IPROC_GPIO_CCA_INT_EVENT_MASK);
        event = _iproc_gpio_readl(ourchip, IPROC_GPIO_CCA_INT_EVENT);
        event_pol = _iproc_gpio_readl(ourchip, IPROC_GPIO_CCA_INT_EDGE);

        for (iter = 0; iter < ourchip->chip.ngpio; iter ++) {
            g_irq = iproc_gpio_to_irq(ourchip, iter);  
            irq_type = irq_desc[g_irq].status_use_accessors & IRQ_TYPE_SENSE_MASK;
            switch(irq_type) {
                case IRQ_TYPE_EDGE_RISING:
                    tmp = event_mask;
                    tmp &= event;
                    tmp &= ~event_pol;
                    if (tmp & (1 << iter)) {
                        generic_handle_irq(g_irq);
                    }
                    break;
                case IRQ_TYPE_EDGE_FALLING:
                    tmp = event_mask;
                    tmp &= event;
                    tmp &= event_pol;
                    if (tmp & (1 << iter)) {
                        generic_handle_irq(g_irq);
                    }                
                    break;
                case IRQ_TYPE_LEVEL_LOW:
                    tmp = in ^ int_pol;
                    tmp &= int_mask;
                    tmp &= int_pol;
                    if (tmp & (1 << iter)) {
                        generic_handle_irq(g_irq);
                    }                
                    break;
                case IRQ_TYPE_LEVEL_HIGH:
                    tmp = in ^ int_pol;
                    tmp &= int_mask;
                    tmp &= ~int_pol;
                    if (tmp & (1 << iter)) {
                        generic_handle_irq(g_irq);
                    }                
                    break;
                default:                        
                    break;
            }
        }
    }else {
        return IRQ_NONE;
    }

    return IRQ_HANDLED;
}


static irqreturn_t 
iproc_gpio_irq_handler_ccb(int irq, void *dev)
{
    struct iproc_gpio_chip *ourchip = dev;
    int iter;
    unsigned int  val;

    val = _iproc_gpio_readl(ourchip, IPROC_GPIO_CCB_INT_MSTAT);
    if(!val){
        return IRQ_NONE;
    }

    for (iter = 0; iter < ourchip->chip.ngpio; iter ++) {
        if (val & (1 << iter)) {
            generic_handle_irq(iproc_gpio_to_irq(ourchip, iter));
        }
    }
    
    return IRQ_HANDLED;
}

static struct irq_chip iproc_gpio_irq_chip = {
    .name         = "IPROC-GPIO",
    .irq_ack      = (void *) iproc_gpio_irq_ack,
    .irq_mask     = (void *) iproc_gpio_irq_mask,
    .irq_unmask   = (void *) iproc_gpio_irq_unmask,
    .irq_set_type = (void *) iproc_gpio_irq_set_type,
};

struct iproc_gpio_chip *iproc_gpios[IPROC_GPIO_END];

static __init void iproc_gpiolib_track(struct iproc_gpio_chip *chip)
{
    unsigned int gpn;
    int i;

    gpn = chip->chip.base;
    for (i = 0; i < chip->chip.ngpio; i++, gpn++) {
        BUG_ON(gpn >= ARRAY_SIZE(iproc_gpios));
        iproc_gpios[gpn] = chip;
    }
}

static int iproc_gpiolib_input(struct gpio_chip *chip, unsigned gpio)
{
    struct iproc_gpio_chip *ourchip = to_iproc_gpio(chip);
    unsigned long flags;
    unsigned int  val;
    unsigned int  nBitMask = 1 << gpio;


    iproc_gpio_lock(ourchip, flags);

    val = _iproc_gpio_readl(ourchip, REGOFFSET_GPIO_EN);
    val &= ~nBitMask;
    _iproc_gpio_writel(ourchip, val, REGOFFSET_GPIO_EN);

    iproc_gpio_unlock(ourchip, flags);
    return 0;
}

static int iproc_gpiolib_output(struct gpio_chip *chip,
			      unsigned gpio, int value)
{
    struct iproc_gpio_chip *ourchip = to_iproc_gpio(chip);
    unsigned long flags;
    unsigned long val;
    unsigned int  nBitMask = 1 << gpio;

    iproc_gpio_lock(ourchip, flags);

    val = _iproc_gpio_readl(ourchip, REGOFFSET_GPIO_EN);
    val |= nBitMask;
    _iproc_gpio_writel(ourchip, val, REGOFFSET_GPIO_EN);

    val = _iproc_gpio_readl(ourchip, REGOFFSET_GPIO_DOUT);

    if (value == 0) {
        /* Set the pin to zero */
        val &= ~nBitMask;
    } else {
        /* Set the pin to 1 */
        val |= nBitMask;
    }
    _iproc_gpio_writel(ourchip, val, REGOFFSET_GPIO_DOUT);

    iproc_gpio_unlock(ourchip, flags);
    return 0;
}

static void iproc_gpiolib_set(struct gpio_chip *chip,
			    unsigned gpio, int value)
{
    struct iproc_gpio_chip *ourchip = to_iproc_gpio(chip);
    unsigned long flags;
    unsigned long val;
    unsigned int  nBitMask = 1 << gpio;

    iproc_gpio_lock(ourchip, flags);


    /* determine the GPIO pin direction 
     */ 
    val = _iproc_gpio_readl(ourchip, REGOFFSET_GPIO_EN);
    val &= nBitMask;

    /* this function only applies to output pin
     */ 
    if (!val)
        return;

    val = _iproc_gpio_readl(ourchip, REGOFFSET_GPIO_DOUT);

    if ( value == 0 ){
        /* Set the pin to zero */
        val &= ~nBitMask;
    }else{
        /* Set the pin to 1 */
        val |= nBitMask;
    }    
    _iproc_gpio_writel(ourchip, val, REGOFFSET_GPIO_DOUT);

    iproc_gpio_unlock(ourchip, flags);

}


static int iproc_gpiolib_get(struct gpio_chip *chip, unsigned gpio)
{
    struct iproc_gpio_chip *ourchip = to_iproc_gpio(chip);
    unsigned long flags;
    unsigned int val, offset;
    unsigned int  nBitMask = 1 << gpio;    

    iproc_gpio_lock(ourchip, flags);
    /* determine the GPIO pin direction 
     */ 
    offset = _iproc_gpio_readl(ourchip, REGOFFSET_GPIO_EN);
    offset &= nBitMask;

    if (offset){
        val = _iproc_gpio_readl(ourchip, REGOFFSET_GPIO_DOUT);
    } else {
        val = _iproc_gpio_readl(ourchip, REGOFFSET_GPIO_DIN);    
    }
    val >>= gpio;
    val &= 1;
    iproc_gpio_unlock(ourchip, flags);

    return val;
}
static int iproc_gpiolib_to_irq(struct gpio_chip *chip,
                unsigned offset)
{
    struct iproc_gpio_chip *ourchip = to_iproc_gpio(chip);
    return iproc_gpio_to_irq(ourchip, offset);
}

static struct of_device_id iproc_gpio_ids[] __initdata = {
	{ .compatible = "iproc,iproc-gpio", },
	{}
};

void __init  iproc_gpiolib_add(struct iproc_gpio_chip *chip)
{
    struct resource *res;
    struct gpio_chip *gc = &chip->chip;
    int ret, i;    

    BUG_ON(!gc->label);
    BUG_ON(!gc->ngpio);
    
    spin_lock_init(&chip->lock);
    
    if (!gc->direction_input)
        gc->direction_input = iproc_gpiolib_input;
    if (!gc->direction_output)
        gc->direction_output = iproc_gpiolib_output;
    if (!gc->set)
        gc->set = iproc_gpiolib_set;
    if (!gc->get)
        gc->get = iproc_gpiolib_get;
    if (!gc->to_irq)
        gc->to_irq = iproc_gpiolib_to_irq;

    gc->of_node = of_find_matching_node(NULL, iproc_gpio_ids);

    /* gpiochip_add() prints own failure message on error. */
    ret = gpiochip_add(gc);
    if (ret >= 0)
        iproc_gpiolib_track(chip);

    printk(KERN_INFO "iproc gpiochip add %s\n", gc->label);
    /* io remap */
    res = chip->resource;

    chip->ioaddr = ioremap_nocache(res->start, (res->end - res->start) + 1);
    printk(KERN_INFO "%s:ioaddr %p \n", gc->label, chip->ioaddr);
    chip->intr_ioaddr = NULL;
    chip->dmu_ioaddr = NULL;
    if(res->child){
        for (i=0; i< 2; i++){        
            if (!strcmp("intr", res->child[i].name)){
                chip->intr_ioaddr = 
                    ioremap_nocache(res->child[i].start, 
                    (res->child[i].end - res->child[i].start) + 1);
            }
            if (!strcmp("dmu", res->child[i].name)){
                chip->dmu_ioaddr = 
                    ioremap_nocache(res->child[i].start, 
                    (res->child[i].end - res->child[i].start) + 1);
            }
        }
        printk(KERN_INFO "%s:intr_ioaddr %p dmu_ioaddr %p\n",
            gc->label, chip->intr_ioaddr,chip->dmu_ioaddr);
    }


    if (chip->irq_base) {
        for (i = chip->irq_base; i < (chip->irq_base + gc->ngpio); i++) {
            irq_set_chip(i, &iproc_gpio_irq_chip);
            irq_set_chip_data(i,chip);
            irq_set_handler(i, handle_level_irq);
            set_irq_flags(i, IRQF_VALID);                            
    
        }        
        if (chip->id == IPROC_GPIO_CCA_ID ){
            unsigned int val;
            /* enable the GPIO in CCA interrupt mask */
            val = readl(chip->intr_ioaddr + IPROC_CCA_INT_MASK);
            val |= IPROC_CCA_INT_F_GPIOINT;
            writel(val, chip->intr_ioaddr + IPROC_CCA_INT_MASK);
            ret = request_irq(chip->irq, iproc_gpio_irq_handler_cca,
                IRQF_NO_SUSPEND|IRQF_SHARED, gc->label, chip);
            if (ret)
                printk(KERN_ERR "Unable to request IRQ%d: %d\n",
                    IPROC_GPIO_CCA_INT, ret);
        }
        if (chip->id == IPROC_GPIO_CCB_ID ){
            ret = request_irq(chip->irq, iproc_gpio_irq_handler_ccb,
                IRQF_NO_SUSPEND, gc->label, chip);
            if (ret)
                printk(KERN_ERR "Unable to request IRQ%d: %d\n",
                    IPROC_GPIO_CCB_INT, ret);
        }
    }
    iproc_gpio_dev[dev] = chip;
    dev++;
}

static int __init gpio_init(void)
{      
    struct iproc_gpio_chip *chip = iproc_gpios_config;
    int gpn;
    int temp_base;

#if defined(CONFIG_MACH_NS)
    /* bcm53012 support 24 gpios; bcm53010/53011 support 16 gpios */
    if ((__REG32(IPROC_IDM_REGISTER_VA + 0xd500) & 0xc) != 0x0) {
        iproc_gpios_config[0].chip.ngpio = 16;
    }
#endif
#if defined(CONFIG_MACH_NSP)
        /* bcm53025 support 32 gpios; bcm53022/53023 support 24 gpios */
        reg32_write((volatile uint32_t *)(IPROC_PCIE_AXIB0_REG_VA + PAXB_0_CONFIG_IND_ADDR_BASE), 0);

/*
	the mechanism to get the chip number does not work, always reads 22K.
	OTP must be programmed and then need to look at OTP
	for now assume 25K chip

        if ((__REG32(IPROC_PCIE_AXIB0_REG_VA + PAXB_0_CONFIG_IND_DATA_BASE) 
            & 0xffff0000) == 0x80250000) {
*/
        if (1) {
            iproc_gpios_config[0].chip.ngpio = 32;
        }
#endif


    temp_base = 0;
    for (gpn = 0; gpn < ARRAY_SIZE(iproc_gpios_config); gpn++, chip++) {
        if (gpn >= MAX_NS_GPIO){
            printk("Unavailabe to add gpiolib\n");
            return -EINVAL;
        }
            
        if (chip->chip.base == -EINVAL) {
            chip->chip.base = temp_base;            
        }

        iproc_gpiolib_add(chip);
        temp_base = chip->chip.base + chip->chip.ngpio;
    }

    return 0;
}
static void __exit gpio_exit(void)
{
    unsigned int val;
    int i=0;

    for (i = 0 ; i < MAX_NS_GPIO; i++) {        
        if(iproc_gpio_dev[i]){
            if(iproc_gpio_dev[i]->ioaddr){
                iounmap(iproc_gpio_dev[i]->ioaddr);
            }
            if(iproc_gpio_dev[i]->intr_ioaddr){
                if (iproc_gpio_dev[i]->id == IPROC_GPIO_CCA_ID ){
                  val = readl(iproc_gpio_dev[i]->intr_ioaddr + IPROC_CCA_INT_MASK);
                  val &= ~(IPROC_CCA_INT_F_GPIOINT);
                  writel(val, iproc_gpio_dev[i]->intr_ioaddr + IPROC_CCA_INT_MASK);
                }                      
                iounmap(iproc_gpio_dev[i]->intr_ioaddr);
            }
            if(iproc_gpio_dev[i]->dmu_ioaddr){
                iounmap(iproc_gpio_dev[i]->dmu_ioaddr);
            }                        
            if(iproc_gpio_dev[i]->irq_base) {
                free_irq(iproc_gpio_dev[i]->irq,iproc_gpio_dev[i]);
            }

            gpiochip_remove(&iproc_gpio_dev[i]->chip);
            iproc_gpio_dev[i] = NULL;
        }
    }
}

MODULE_DESCRIPTION("IPROC GPIO driver");
MODULE_LICENSE("GPL");

postcore_initcall(gpio_init);
module_exit(gpio_exit);
