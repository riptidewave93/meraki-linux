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
#include <linux/module.h>
#include <linux/irq.h>
#include <linux/io.h>

#include <asm/io.h>

#include <mach/io_map.h>
#include <mach/iproc_regs.h>
#include <mach/gpio.h>
#include <mach/gpio_cfg.h>

#if (defined(CONFIG_MACH_NS) || defined(CONFIG_MACH_NSP))
int iproc_gpio_set_config(struct iproc_gpio_chip *chip,
			      unsigned int off, unsigned int cfg)
{
    unsigned long aux_sel;
    unsigned int aux_sel_reg;
    unsigned int base, pin;
    void __iomem *cca = IOMEM(IPROC_CCA_CORE_REG_VA);
    u8 maj = ((readl(cca) & 0xc0000) >> 18);

    //the two gpio controlls have inverted aux mode bit logic
    int set_bit = 0;

    base = 0;
    pin = 1 << off;
    if (chip->id == IPROC_GPIO_CCA_ID) {
        aux_sel_reg = IPROC_GPIO_CCA_CTRL0;
        base = (unsigned int)chip->dmu_ioaddr;
        switch(off) {
            case 26 ... 27:
                //the above pins have inverted logic on chip revsion B
                if (maj)
                    cfg = (cfg == IPROC_GPIO_AUX_FUN ? IPROC_GPIO_GENERAL : IPROC_GPIO_AUX_FUN);
        }
        set_bit = (cfg != IPROC_GPIO_AUX_FUN);
    } else if (chip->id == IPROC_GPIO_CCB_ID) {
        aux_sel_reg = IPROC_GPIO_CCB_AUX_SEL;
        base = (unsigned int)chip->ioaddr;
        set_bit = (cfg == IPROC_GPIO_AUX_FUN);
    } else {
        return -EINVAL;
    }
    aux_sel = __raw_readl(base + aux_sel_reg);

    if (set_bit)
        aux_sel |= pin;
    else
        aux_sel &= ~pin;

    __raw_writel(aux_sel, base + aux_sel_reg);
    return 0;
}


unsigned iproc_gpio_get_config(struct iproc_gpio_chip *chip,
				   unsigned int off)
{
    unsigned long aux_sel;
    unsigned int aux_sel_reg;
    unsigned int base, pin;
    void __iomem *cca = IOMEM(IPROC_CCA_CORE_REG_VA);
    u8 maj = ((readl(cca) & 0xc0000) >> 18);

    base = 0;
    pin = 0;
    if (chip->id == IPROC_GPIO_CCA_ID) {

        aux_sel_reg = IPROC_GPIO_CCA_CTRL0;
        base = (unsigned int)chip->dmu_ioaddr;
        if(off <= 17 || (off >= 21 && off <= 25)){
            pin = 1 << off;
        //For revision B, the remaining GPIOs are reveresed
        } else if (off >= 18 && off <= 31) {
            if (!maj)
                pin = 1 << off;
        } else {
            return IPROC_GPIO_GENERAL;
        }
    }
    if (chip->id == IPROC_GPIO_CCB_ID) {
        aux_sel_reg = IPROC_GPIO_CCB_AUX_SEL;
        base = (unsigned int)chip->ioaddr;
        pin = 1 << off;
    }

    aux_sel = __raw_readl(base + aux_sel_reg);

    if (aux_sel & pin) {
        if (chip->id == IPROC_GPIO_CCA_ID) {
            return IPROC_GPIO_GENERAL;
        } else {
            return IPROC_GPIO_AUX_FUN;
        }
    } else {
        if (chip->id == IPROC_GPIO_CCA_ID) {
            return IPROC_GPIO_AUX_FUN;
        } else {
            return IPROC_GPIO_GENERAL;
        }        
    }        
}


int iproc_gpio_setpull_updown(struct iproc_gpio_chip *chip,
			    unsigned int off, iproc_gpio_pull_t pull)
{
    unsigned int base;
    base = 0;

    if (chip->id == IPROC_GPIO_CCA_ID) {
        unsigned long pull_up, pull_down;

        base = (unsigned int)chip->dmu_ioaddr;

        pull_up = __raw_readl(base + IPROC_GPIO_CCA_PULL_UP);
        pull_down = __raw_readl(base + IPROC_GPIO_CCA_PULL_DOWN);

        switch (pull) {
            case IPROC_GPIO_PULL_UP:
                pull_up |= (1 << off);
                pull_down &= ~(1 << off);
                break;
            case IPROC_GPIO_PULL_DOWN:
                pull_up &= ~(1 << off);
                pull_down |= (1 << off);
                break;
            case IPROC_GPIO_PULL_NONE:
                pull_up &= ~(1 << off);
                pull_down &= ~(1 << off);
                break;
            default:
                return -EINVAL;                                
        }
        __raw_writel(pull_up, base + IPROC_GPIO_CCA_PULL_UP);
        __raw_writel(pull_down, base + IPROC_GPIO_CCA_PULL_DOWN);
    }

    if (chip->id == IPROC_GPIO_CCB_ID) {
        unsigned long pad_res, res_en;        

        base = (unsigned int)chip->ioaddr;

        pad_res = __raw_readl(base + IPROC_GPIO_CCB_PAD_RES);
        res_en = __raw_readl(base + IPROC_GPIO_CCB_RES_EN);

        switch (pull) {
            case IPROC_GPIO_PULL_UP:
                pad_res |= (1 << off);
                res_en |= (1 << off);
                break;
            case IPROC_GPIO_PULL_DOWN:
                pad_res &= ~(1 << off);
                res_en |= (1 << off);
                break;
            case IPROC_GPIO_PULL_NONE:           
                res_en &= ~(1 << off);
                break;                
            default:
                return -EINVAL;
        }
        __raw_writel(pad_res, base + IPROC_GPIO_CCB_PAD_RES);
        __raw_writel(res_en, base + IPROC_GPIO_CCB_RES_EN);
    }
    return 0;
}


iproc_gpio_pull_t iproc_gpio_getpull_updown(struct iproc_gpio_chip *chip,
					unsigned int off)
{

    unsigned int base;
    base = 0;

    if (chip->id == IPROC_GPIO_CCA_ID) {
        unsigned long pull_up, pull_down;

        base = (unsigned int)chip->dmu_ioaddr;

        pull_up = __raw_readl(base + IPROC_GPIO_CCA_PULL_UP);
        pull_down = __raw_readl(base + IPROC_GPIO_CCA_PULL_DOWN);
        pull_up &= 1 << off;
        pull_down &= 1 << off;

        if (pull_up ^ pull_down) {
            if (pull_up) {
                return IPROC_GPIO_PULL_UP;
            } else {
                return IPROC_GPIO_PULL_DOWN;
            }                
        } else if(!pull_up) {
            return IPROC_GPIO_PULL_NONE;
        }
    }

    if (chip->id == IPROC_GPIO_CCB_ID) {
        unsigned long pad_res, res_en;        

        base = (unsigned int)chip->ioaddr;

        pad_res = __raw_readl(base + IPROC_GPIO_CCB_PAD_RES);
        res_en = __raw_readl(base + IPROC_GPIO_CCB_RES_EN);

        pad_res &= 1 << off;
        res_en &= 1 << off;

        if (res_en) {
            if (pad_res) {
                return IPROC_GPIO_PULL_UP;
            } else {
                return IPROC_GPIO_PULL_DOWN;
            }
        } else {
            return IPROC_GPIO_PULL_NONE;
        }
    }    
    return IPROC_GPIO_PULL_NONE;
}
#endif

iproc_gpio_pull_t iproc_gpio_getpull(unsigned int pin)
{
	struct iproc_gpio_chip *chip = iproc_gpiolib_getchip(pin);
	unsigned long flags;
	int offset, ret;

	if (!chip)
		return -EINVAL;

	offset = pin - chip->chip.base;

	iproc_gpio_lock(chip, flags);
    if (chip->config){	
	    ret = (chip->config->get_pull)(chip, offset);
    }
	iproc_gpio_unlock(chip, flags);

	return ret;

}
EXPORT_SYMBOL(iproc_gpio_getpull);


int iproc_gpio_setpull(unsigned int pin, iproc_gpio_pull_t pull)
{
	struct iproc_gpio_chip *chip = iproc_gpiolib_getchip(pin);
	unsigned long flags;
	int offset, ret;

	if (!chip)
		return -EINVAL;

	offset = pin - chip->chip.base;

	iproc_gpio_lock(chip, flags);
    if (chip->config){
	    ret = (chip->config->set_pull)(chip, offset, pull);
    }
	iproc_gpio_unlock(chip, flags);

	return ret;

}
EXPORT_SYMBOL(iproc_gpio_setpull);

unsigned iproc_gpio_getcfg(unsigned int pin)
{
    struct iproc_gpio_chip *chip = iproc_gpiolib_getchip(pin);
    unsigned long flags;
    int offset;
	unsigned ret = 0;

	if (!chip)
		return -EINVAL;

    offset = pin - chip->chip.base;
    iproc_gpio_lock(chip, flags);
    if (chip->config){
        ret = (chip->config->get_config)(chip, offset);
    }
    iproc_gpio_unlock(chip, flags);
        
    return ret;
}

EXPORT_SYMBOL(iproc_gpio_getcfg);

int iproc_gpio_cfgpin(unsigned int pin, unsigned int config)
{
	struct iproc_gpio_chip *chip = iproc_gpiolib_getchip(pin);
	unsigned long flags;
	int offset;
	int ret = 0;

	if (!chip)
		return -EINVAL;

	offset = pin - chip->chip.base;
	iproc_gpio_lock(chip, flags);
    if (chip->config){
        (chip->config->set_config)(chip, offset, config);
    }
	iproc_gpio_unlock(chip, flags);

	return ret;


}
EXPORT_SYMBOL(iproc_gpio_cfgpin);
