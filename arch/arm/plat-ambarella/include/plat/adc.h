/*
 * arch/arm/plat-ambarella/include/plat/adc.h
 *
 * Author: Anthony Ginger <hfjiang@ambarella.com>
 *
 * Copyright (C) 2004-2010, Ambarella, Inc.
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA 02111-1307 USA
 *
 */

#ifndef __PLAT_AMBARELLA_ADC_H__
#define __PLAT_AMBARELLA_ADC_H__

/* ==========================================================================*/
#if (CHIP_REV == A5)
#define ADC_EN_HI(x)			((x) << 21)
#define ADC_EN_LO(x)			((x) << 20)
#else
#define ADC_EN_HI(x)			((x) << 31)
#define ADC_EN_LO(x)			((x) << 30)
#endif
#define ADC_VAL_LO(x)			((x) & 0x3ff)

#if (CHIP_REV == AMB_S2)
#define ADC_MAX_SLOT_NUMBER	8
#endif
#define ADC_CH0		(1 << 0)
#define ADC_CH1		(1 << 1)
#define ADC_CH2		(1 << 2)
#define ADC_CH3		(1 << 3)
#define ADC_CH4		(1 << 4)
#define ADC_CH5		(1 << 5)
#define ADC_CH6		(1 << 6)
#define ADC_CH7		(1 << 7)
#define ADC_CH8		(1 << 8)
#define ADC_CH9		(1 << 9)
#define ADC_CH10		(1 << 10)
#define ADC_CH11		(1 << 11)

/* ==========================================================================*/
#ifndef __ASSEMBLER__

enum ambarella_adc_mode{
	AMBA_ADC_IRQ_MODE,
	AMBA_ADC_POL_MODE
};

struct ambarella_adc_controller {
	u32				(*open)(void);
	u32				(*close)(void);
	void				(*read_channels)(u32*, u32*);
	u32				(*is_irq_supported)(void);
	void				(*set_irq_threshold)(u32, u32, u32);
	void				(*reset)(void);
	void				(*stop)(void);
	u32				(*get_channel_num)(void);
	u32				(*read_channel)(u32);
	u32				scan_delay;
	u32				(*temper_curve)(u32);
	u32				adc_temper_channel;
#if (CHIP_REV == AMB_S2)
	u8				adc_counter;
	u8				adc_slot_num;
	u16				adc_slot_period;
	u16				adc_slot_ctrl[ADC_MAX_SLOT_NUMBER];
#endif
};
#define AMBA_ADC_PARAM_CALL(arg, perm) \
	module_param_cb(adc_scan_delay, &param_ops_int, &arg.scan_delay, perm)

/* ==========================================================================*/
//extern struct platform_device		ambarella_adc0;
extern struct platform_device		ambarella_adc_temper;
extern struct platform_device 		sc_adc_dn;
/* ==========================================================================*/
extern int ambarella_init_adc(void);
extern u32 ambarella_adc_suspend(u32 level);
extern u32 ambarella_adc_resume(u32 level);
extern void ambarella_adc_get_array(u32 *adc_data, u32 *array_size);
extern u32 amb_temper_curve(u32 adc_data);

#endif /* __ASSEMBLER__ */
/* ==========================================================================*/

#endif /* __PLAT_AMBARELLA_ADC_H__ */

