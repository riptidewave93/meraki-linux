/*
 * arch/arm/plat-ambarella/generic/adc.c
 *
 * Author: Anthony Ginger <hfjiang@ambarella.com>
 *
 * Copyright (C) 2004-2009, Ambarella, Inc.
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

#include <linux/kernel.h>
#include <linux/init.h>
#include <linux/platform_device.h>
#include <linux/bootmem.h>
#include <linux/delay.h>
#include <linux/irq.h>
#include <linux/dma-mapping.h>
#include <linux/moduleparam.h>
#include <linux/proc_fs.h>
#include <asm/uaccess.h>
#include <asm/system_info.h>
#include <linux/module.h>

#include <mach/hardware.h>
#include <plat/adc.h>

/* ==========================================================================*/
#ifdef MODULE_PARAM_PREFIX
#undef MODULE_PARAM_PREFIX
#endif
#define MODULE_PARAM_PREFIX	"ambarella_config."

static atomic_t use_count;

static DEFINE_MUTEX(adc_lock);

struct ambarella_adc_controller ambarella_platform_adc_controller0;
/* ==========================================================================*/
#if ((CHIP_REV == A5) || (CHIP_REV == A6) || (CHIP_REV == A5S) ||\
     (CHIP_REV == A7) || (CHIP_REV == A5L) || (CHIP_REV == AMB_S2))
#undef ADC_ONE_SHOT
#define ADC_ONE_SHOT
#else
#define ADC_ONE_SHOT
#endif	/* CHIP_REV */

#ifndef CONFIG_AMBARELLA_ADC_WAIT_COUNTER_LIMIT
#define CONFIG_AMBARELLA_ADC_WAIT_COUNTER_LIMIT	(100000)
#endif


/* ==========================================================================*/
u32 ambarella_adc_get_instances(void)
{
	return ADC_NUM_CHANNELS;
}

static inline u32 ambarella_adc_get_channel_inline(u32 channel_id)
{
	u32					adc_data = 0;

	mutex_lock(&adc_lock);

#if defined(ADC_ONE_SHOT)
	amba_setbitsl(ADC_CONTROL_REG, ADC_CONTROL_START);
#if ((CHIP_REV == A5) || (CHIP_REV == A6) || (CHIP_REV == A5S) || \
     (CHIP_REV == A7) || (CHIP_REV == A5L))
	while (amba_tstbitsl(ADC_CONTROL_REG, ADC_CONTROL_STATUS) == 0x0) {
		msleep(1);
	}
#elif (CHIP_REV == AMB_S2)
	while (amba_tstbitsl(ADC_STATUS_REG, ADC_CONTROL_STATUS) == 0x0);
#else
	while (amba_tstbitsl(ADC_CONTROL_REG, ADC_CONTROL_STATUS) == 0x0);
#endif
#endif

	switch(channel_id) {
#if (ADC_NUM_CHANNELS == 8)
	case 0:
		adc_data = (amba_readl(ADC_DATA0_REG) + 0x8000) & 0xffff;
		break;
	case 1:
		adc_data = (amba_readl(ADC_DATA1_REG) + 0x8000) & 0xffff;
		break;
	case 2:
		adc_data = (amba_readl(ADC_DATA2_REG) + 0x8000) & 0xffff;
		break;
	case 3:
		adc_data = (amba_readl(ADC_DATA3_REG) + 0x8000) & 0xffff;
		break;
	case 4:
		adc_data = (amba_readl(ADC_DATA4_REG) + 0x8000) & 0xffff;
		break;
	case 5:
		adc_data = (amba_readl(ADC_DATA5_REG) + 0x8000) & 0xffff;
		break;
	case 6:
		adc_data = (amba_readl(ADC_DATA6_REG) + 0x8000) & 0xffff;
		break;
	case 7:
		adc_data = (amba_readl(ADC_DATA7_REG) + 0x8000) & 0xffff;
		break;
#else
	case 0:
		adc_data = amba_readl(ADC_DATA0_REG);
		break;
	case 1:
		adc_data = amba_readl(ADC_DATA1_REG);
		break;
	case 2:
		adc_data = amba_readl(ADC_DATA2_REG);
		break;
	case 3:
		adc_data = amba_readl(ADC_DATA3_REG);
		break;
#if (ADC_NUM_CHANNELS >= 6)
	case 4:
		adc_data = amba_readl(ADC_DATA4_REG);
		break;
	case 5:
		adc_data = amba_readl(ADC_DATA5_REG);
		break;
#endif
#if (ADC_NUM_CHANNELS >= 10)
	case 6:
		adc_data = amba_readl(ADC_DATA6_REG);
		break;
	case 7:
		adc_data = amba_readl(ADC_DATA7_REG);
		break;
	case 8:
		adc_data = amba_readl(ADC_DATA8_REG);
		break;
	case 9:
		adc_data = amba_readl(ADC_DATA9_REG);
		break;
#endif
#if (ADC_NUM_CHANNELS >= 12)
	case 10:
		adc_data = amba_readl(ADC_DATA10_REG);
		break;
	case 11:
		adc_data = amba_readl(ADC_DATA11_REG);
		break;
#endif
#endif
	default:
		pr_warning("%s: invalid adc channel id %d!\n",
			__func__, channel_id);
		break;
	}
	pr_debug("%s: channel[%d] = %d.\n", __func__, channel_id, adc_data);

	mutex_unlock(&adc_lock);

	return adc_data;
}

void ambarella_adc_get_array(u32 *adc_data, u32 *array_size)
{
	int					i;

	if (unlikely(*array_size > ADC_NUM_CHANNELS)) {
		pr_err("%s: array_size should be %d, not %d!\n",
			__func__, ADC_NUM_CHANNELS, *array_size);
		return;
	}

	mutex_lock(&adc_lock);

#if defined(ADC_ONE_SHOT)
	amba_setbitsl(ADC_CONTROL_REG, ADC_CONTROL_START);
#if ((CHIP_REV == A5) || (CHIP_REV == A6) || (CHIP_REV == A5S) || \
     (CHIP_REV == A7) || (CHIP_REV == A5L))
	while (amba_tstbitsl(ADC_CONTROL_REG, ADC_CONTROL_STATUS) == 0x0) {
		msleep(1);
	}
#elif (CHIP_REV == AMB_S2)
	while (amba_tstbitsl(ADC_STATUS_REG, ADC_CONTROL_STATUS) == 0x0);
#else
	while (amba_tstbitsl(ADC_CONTROL_REG, ADC_CONTROL_STATUS) == 0x0);
#endif
#endif

#if (ADC_NUM_CHANNELS == 8)
	adc_data[0] = (amba_readl(ADC_DATA0_REG) + 0x8000) & 0xffff;
	adc_data[1] = (amba_readl(ADC_DATA1_REG) + 0x8000) & 0xffff;
	adc_data[2] = (amba_readl(ADC_DATA2_REG) + 0x8000) & 0xffff;
	adc_data[3] = (amba_readl(ADC_DATA3_REG) + 0x8000) & 0xffff;
	adc_data[4] = (amba_readl(ADC_DATA4_REG) + 0x8000) & 0xffff;
	adc_data[5] = (amba_readl(ADC_DATA5_REG) + 0x8000) & 0xffff;
	adc_data[6] = (amba_readl(ADC_DATA6_REG) + 0x8000) & 0xffff;
	adc_data[7] = (amba_readl(ADC_DATA7_REG) + 0x8000) & 0xffff;
#else
	adc_data[0] = amba_readl(ADC_DATA0_REG);
	adc_data[1] = amba_readl(ADC_DATA1_REG);
	adc_data[2] = amba_readl(ADC_DATA2_REG);
	adc_data[3] = amba_readl(ADC_DATA3_REG);
#if (ADC_NUM_CHANNELS >= 6)
	adc_data[4] = amba_readl(ADC_DATA4_REG);
	adc_data[5] = amba_readl(ADC_DATA5_REG);
#endif
#if (ADC_NUM_CHANNELS >= 10)
	adc_data[6] = amba_readl(ADC_DATA6_REG);
	adc_data[7] = amba_readl(ADC_DATA7_REG);
	adc_data[8] = amba_readl(ADC_DATA8_REG);
	adc_data[9] = amba_readl(ADC_DATA9_REG);
#endif
#if (ADC_NUM_CHANNELS >= 12)
	adc_data[10] = amba_readl(ADC_DATA10_REG);
	adc_data[11] = amba_readl(ADC_DATA11_REG);
#endif
#endif
	mutex_unlock(&adc_lock);

	for (i = 0; i < ADC_NUM_CHANNELS; i++)
		pr_debug("%s: channel[%d] = %d.\n", __func__, i, adc_data[i]);
}
EXPORT_SYMBOL(ambarella_adc_get_array);

#if (CHIP_REV == AMB_S2)
void ambarella_adc_set_slot_ctrl(u8 slot_id, u32 slot_value)
{
	switch (slot_id) {
	case 0:
		amba_writel(ADC_SLOT_CTRL_0_REG, slot_value);
		break;
	case 1:
		amba_writel(ADC_SLOT_CTRL_1_REG, slot_value);
		break;
	case 2:
		amba_writel(ADC_SLOT_CTRL_2_REG, slot_value);
		break;
	case 3:
		amba_writel(ADC_SLOT_CTRL_3_REG, slot_value);
		break;
	case 4:
		amba_writel(ADC_SLOT_CTRL_4_REG, slot_value);
		break;
	case 5:
		amba_writel(ADC_SLOT_CTRL_5_REG, slot_value);
		break;
	case 6:
		amba_writel(ADC_SLOT_CTRL_6_REG, slot_value);
		break;
	case 7:
		amba_writel(ADC_SLOT_CTRL_7_REG, slot_value);
		break;
	}
}

static int ambarella_adc_set_fifo_ctrl(u32 fifo_id, u16 fifo_cid)
{
	u32 reg=0;
	u32 reg_val=0;

	switch(fifo_id) {
	case 0:
		reg = ADC_FIFO_CTRL_0_REG;
		reg_val = ADC_FIFO_TH
			| (fifo_cid << ADC_FIFO_ID_SHIFT)
			| ADC_FIFO_DEPTH;
		break;
	case 1:
		reg=ADC_FIFO_CTRL_1_REG;
		break;
	case 2:
		reg=ADC_FIFO_CTRL_2_REG;
		break;
	case 3:
		reg=ADC_FIFO_CTRL_3_REG;
		break;
	default:
		pr_err("%s: invalid fifo NO = %d.\n",
			__func__, fifo_id);
		return -1;
	}

	amba_writel(reg, reg_val);

	amba_writel(ADC_FIFO_CTRL_REG, ADC_FIFO_CONTROL_CLEAR);
	return 0;
}

void ambarella_adc_set_config(void)
{
	int i = 0;
	int slot_num_reg = 0;
	amba_setbitsl(ADC_CONTROL_REG, ADC_CONTROL_ENABLE);
	udelay(3);

	ambarella_platform_adc_controller0.adc_slot_num = 1;
	ambarella_platform_adc_controller0.adc_slot_period = 60;
	ambarella_platform_adc_controller0.adc_slot_ctrl[0] = 0xfff;
	slot_num_reg = ambarella_platform_adc_controller0.adc_slot_num-1;
	amba_writel(ADC_SLOT_NUM_REG, slot_num_reg);
	amba_writel(ADC_SLOT_PERIOD_REG, ambarella_platform_adc_controller0.adc_slot_period);
	for(i=0;i <= slot_num_reg; i++)
		ambarella_adc_set_slot_ctrl(i, ambarella_platform_adc_controller0.adc_slot_ctrl[i]);

	//temporary config fifo_0 with channel 3
	ambarella_adc_set_fifo_ctrl(0,3);
}
#endif

void ambarella_adc_start(void)
{
	mutex_lock(&adc_lock);

#if defined(ADC16_CTRL_REG)
	amba_clrbitsl(ADC16_CTRL_REG, 0x2);
#endif

#if (CHIP_REV == A5L)
	amba_writel(ADC_CONTROL_REG, 0x0);

	amba_writel(ADC_DATA0_REG, 0);
	amba_writel(ADC_DATA1_REG, 0);
	amba_writel(ADC_DATA2_REG, 0);
	amba_writel(ADC_DATA3_REG, 0);

	amba_writel(ADC_ENABLE_REG, 0x0);
#endif

#if (CHIP_REV == A5 || CHIP_REV == A6)
	/* SCALER_ADC_REG (default=4) */
	/* clk_au = 27MHz/2 */
	rct_set_adc_clk_freq_hz(PLL_CLK_13_5MHZ);

	/* ADC Analog (lowest power) */
	amba_writel(ADC16_CTRL_REG, 0x031cff);

	/* ADC reset */
	amba_writel(ADC_RESET_REG, 0x1);

	/* Fix nonlinearity */
	amba_writel(ADC16_CTRL_REG, 0x00031c00);
#endif

#if (CHIP_REV == A5S)
	/* stop conversion */
	amba_writel(ADC_CONTROL_REG, 0x0);

	amba_writel(ADC_DATA0_REG, 0);
	amba_writel(ADC_DATA1_REG, 0);
	amba_writel(ADC_DATA2_REG, 0);
	amba_writel(ADC_DATA3_REG, 0);

	amba_writel(ADC_ENABLE_REG, 0x0);
#endif

#if (CHIP_REV == A7)
	/* stop conversion */
	amba_writel(ADC_CONTROL_REG, 0x0);

	amba_writel(ADC_DATA0_REG, 0);
	amba_writel(ADC_DATA1_REG, 0);
	amba_writel(ADC_DATA2_REG, 0);
	amba_writel(ADC_DATA3_REG, 0);

	amba_writel(ADC_ENABLE_REG, 0x1);
	amba_writel(ADC_CONTROL_REG, 0x1);
#elif (CHIP_REV != AMB_S2)
#if (CHIP_REV != I1)
	if (amba_readl(ADC_ENABLE_REG) != 0) {
		pr_err("%s: ADC_ENABLE_REG = %d.\n",
			__func__, amba_readl(ADC_ENABLE_REG));
		mutex_unlock(&adc_lock);
		return;
	}
#endif
	amba_writel(ADC_ENABLE_REG, 0x1);
#endif

#if (CHIP_REV == AMB_S2)
	amba_setbitsl(ADC_CONTROL_REG, ADC_CONTROL_CLEAR);
#endif

#ifdef ADC_ONE_SHOT
	/* ADC control mode, single */
	amba_clrbitsl(ADC_CONTROL_REG, ADC_CONTROL_MODE);
#else
	/* ADC control mode, continuous */
	amba_setbitsl(ADC_CONTROL_REG, ADC_CONTROL_MODE);

	/* start conversion */
	amba_setbitsl(ADC_CONTROL_REG, ADC_CONTROL_START);
	while (amba_tstbitsl(ADC_CONTROL_REG, ADC_CONTROL_STATUS) == 0x0);
#endif

#if (CHIP_REV == AMB_S2)
	ambarella_adc_set_config();
#endif

	mutex_unlock(&adc_lock);
}

void ambarella_adc_stop(void)
{
	mutex_lock(&adc_lock);
#ifndef ADC_ONE_SHOT
	amba_writel(ADC_CONTROL_REG, 0x0);
#endif
#if (CHIP_REV != AMB_S2)
	amba_writel(ADC_ENABLE_REG, 0x0);
#else
	amba_clrbitsl(ADC_CONTROL_REG, ADC_CONTROL_ENABLE);
#endif
#if (CHIP_REV == A7)
	amba_writel(ADC_CONTROL_REG, 0x0);
#endif
#if defined(ADC16_CTRL_REG)
	amba_setbitsl(ADC16_CTRL_REG, 0x2);
#endif
	mutex_unlock(&adc_lock);
}

int __init ambarella_init_adc(void)
{
	int					retval = 0;
	atomic_set(&use_count, -1);
	return retval;
}

u32 adc_is_irq_supported(void)
{
#if (ADC_SUPPORT_THRESHOLD_INT == 1)
#ifndef ADC_ONE_SHOT
	return 1;
#else
	return 0;
#endif
#else
	return 0;
#endif
}

void adc_set_irq_threshold(u32 ch, u32 h_level, u32 l_level)
{
#if (ADC_SUPPORT_THRESHOLD_INT == 1)
	u32 irq_control_address = 0;
	u32 value = ADC_EN_HI(!!h_level) | ADC_EN_LO(!!l_level) |
		ADC_VAL_HI(h_level) | ADC_VAL_LO(l_level);

	switch (ch) {
	case 0:
		irq_control_address = ADC_CHAN0_INTR_REG;
		break;
	case 1:
		irq_control_address = ADC_CHAN1_INTR_REG;
		break;
	case 2:
		irq_control_address = ADC_CHAN2_INTR_REG;
		break;
	case 3:
		irq_control_address = ADC_CHAN3_INTR_REG;
		break;
#if (ADC_NUM_CHANNELS >= 6)
	case 4:
		irq_control_address = ADC_CHAN4_INTR_REG;
		break;
	case 5:
		irq_control_address = ADC_CHAN5_INTR_REG;
		break;
#endif
#if (ADC_NUM_CHANNELS >= 8)
	case 6:
		irq_control_address = ADC_CHAN6_INTR_REG;
		break;
	case 7:
		irq_control_address = ADC_CHAN7_INTR_REG;
		break;
#endif
#if (ADC_NUM_CHANNELS >= 10)
	case 8:
		irq_control_address = ADC_CHAN8_INTR_REG;
		break;
	case 9:
		irq_control_address = ADC_CHAN9_INTR_REG;
		break;
#endif
#if (ADC_NUM_CHANNELS >= 12)
	case 10:
		irq_control_address = ADC_CHAN10_INTR_REG;
		break;
	case 11:
		irq_control_address = ADC_CHAN11_INTR_REG;
		break;
#endif
	default:
		printk("Don't support %d channels\n", ch);
		return;
	}
	amba_writel(irq_control_address, value);
	pr_err("%s: set ch[%d] h[%d], l[%d], 0x%08X!\n",
		__func__, ch, h_level, l_level, value);
#endif
}

u32 amb_temper_curve(u32 adc_data)
{
	return ((((1083 * adc_data * 315) / 4096) - 81900) / 1000);// Tj=(108.3 * adc_value * 3.15 / 4096) - 81.9
}

u32 ambarella_adc_open(void)
{
	if(atomic_inc_and_test(&use_count))
		ambarella_adc_start();
	return 0;
}

u32 ambarella_adc_close(void)
{
	if(atomic_read(&use_count) < 0)
		return -1;

	if(atomic_dec_return(&use_count) == (-1)){
		ambarella_adc_stop();
	}
	return 0;
}

/* ==========================================================================*/
struct ambarella_adc_pm_info {
#if defined(ADC16_CTRL_REG)
	u32 adc_rct_reg;
#endif
	u32 adc_control_reg;
	u32 adc_enable_reg;
	u32 adc_chan0_intr_reg;
	u32 adc_chan1_intr_reg;
	u32 adc_chan2_intr_reg;
	u32 adc_chan3_intr_reg;
#if (ADC_NUM_CHANNELS >= 6)
	u32 adc_chan4_intr_reg;
	u32 adc_chan5_intr_reg;
#endif
#if (ADC_NUM_CHANNELS >= 8)
	u32 adc_chan6_intr_reg;
	u32 adc_chan7_intr_reg;
#endif
#if (ADC_NUM_CHANNELS >= 10)
	u32 adc_chan8_intr_reg;
	u32 adc_chan9_intr_reg;
#endif
#if (ADC_NUM_CHANNELS >= 12)
	u32 adc_chan10_intr_reg;
	u32 adc_chan11_intr_reg;
#endif
};

struct ambarella_adc_pm_info ambarella_adc_pm;

u32 ambarella_adc_suspend(u32 level)
{
#if defined(ADC16_CTRL_REG)
	ambarella_adc_pm.adc_rct_reg = amba_readl(ADC16_CTRL_REG);
#endif
	ambarella_adc_pm.adc_control_reg = amba_readl(ADC_CONTROL_REG);
	ambarella_adc_pm.adc_enable_reg = amba_readl(ADC_ENABLE_REG);
	ambarella_adc_pm.adc_chan0_intr_reg = amba_readl(ADC_CHAN0_INTR_REG);
	ambarella_adc_pm.adc_chan1_intr_reg = amba_readl(ADC_CHAN1_INTR_REG);
	ambarella_adc_pm.adc_chan2_intr_reg = amba_readl(ADC_CHAN2_INTR_REG);
	ambarella_adc_pm.adc_chan3_intr_reg = amba_readl(ADC_CHAN3_INTR_REG);
#if (ADC_NUM_CHANNELS >= 6)
	ambarella_adc_pm.adc_chan4_intr_reg = amba_readl(ADC_CHAN4_INTR_REG);
	ambarella_adc_pm.adc_chan5_intr_reg = amba_readl(ADC_CHAN5_INTR_REG);
#endif
#if (ADC_NUM_CHANNELS >= 8)
	ambarella_adc_pm.adc_chan6_intr_reg = amba_readl(ADC_CHAN6_INTR_REG);
	ambarella_adc_pm.adc_chan7_intr_reg = amba_readl(ADC_CHAN7_INTR_REG);
#endif
#if (ADC_NUM_CHANNELS >= 10)
	ambarella_adc_pm.adc_chan8_intr_reg = amba_readl(ADC_CHAN8_INTR_REG);
	ambarella_adc_pm.adc_chan9_intr_reg = amba_readl(ADC_CHAN9_INTR_REG);
#endif
#if (ADC_NUM_CHANNELS >= 12)
	ambarella_adc_pm.adc_chan10_intr_reg = amba_readl(ADC_CHAN10_INTR_REG);
	ambarella_adc_pm.adc_chan11_intr_reg = amba_readl(ADC_CHAN11_INTR_REG);
#endif
	return 0;
}

u32 ambarella_adc_resume(u32 level)
{
#if defined(ADC16_CTRL_REG)
	amba_writel(ADC16_CTRL_REG, ambarella_adc_pm.adc_rct_reg);
#endif
	if (ambarella_adc_pm.adc_enable_reg & 0x01) {
		amba_writel(ADC_ENABLE_REG, ambarella_adc_pm.adc_enable_reg);

		if ((amba_readl(ADC_CONTROL_REG) & 0xfffffffc) != 0) {
			amba_writel(ADC_CONTROL_REG, 0x0);
			while ((amba_readl(ADC_CONTROL_REG) & ADC_CONTROL_STATUS) == 0x0);
		}
		if ((ambarella_adc_pm.adc_control_reg & 0xfffffffe) != 0) {
			amba_writel(ADC_CONTROL_REG, ambarella_adc_pm.adc_control_reg & 0xfffffffe);
			while ((amba_readl(ADC_CONTROL_REG) & ADC_CONTROL_STATUS) == 0x0);
		}

		amba_writel(ADC_CHAN0_INTR_REG, ambarella_adc_pm.adc_chan0_intr_reg);
		amba_writel(ADC_CHAN1_INTR_REG, ambarella_adc_pm.adc_chan1_intr_reg);
		amba_writel(ADC_CHAN2_INTR_REG, ambarella_adc_pm.adc_chan2_intr_reg);
		amba_writel(ADC_CHAN3_INTR_REG, ambarella_adc_pm.adc_chan3_intr_reg);
#if (ADC_NUM_CHANNELS >= 6)
		amba_writel(ADC_CHAN4_INTR_REG, ambarella_adc_pm.adc_chan4_intr_reg);
		amba_writel(ADC_CHAN5_INTR_REG, ambarella_adc_pm.adc_chan5_intr_reg);
#endif
#if (ADC_NUM_CHANNELS >= 8)
		amba_writel(ADC_CHAN6_INTR_REG, ambarella_adc_pm.adc_chan6_intr_reg);
		amba_writel(ADC_CHAN7_INTR_REG, ambarella_adc_pm.adc_chan7_intr_reg);
#endif
#if (ADC_NUM_CHANNELS >= 10)
		amba_writel(ADC_CHAN8_INTR_REG, ambarella_adc_pm.adc_chan8_intr_reg);
		amba_writel(ADC_CHAN9_INTR_REG, ambarella_adc_pm.adc_chan9_intr_reg);
#endif
#if (ADC_NUM_CHANNELS >= 12)
		amba_writel(ADC_CHAN8_INTR_REG, ambarella_adc_pm.adc_chan10_intr_reg);
		amba_writel(ADC_CHAN9_INTR_REG, ambarella_adc_pm.adc_chan11_intr_reg);
#endif
	}

	return 0;
}

/* ==========================================================================*/
struct ambarella_adc_controller ambarella_platform_adc_controller0 = {
	.open			= ambarella_adc_open,
	.close			= ambarella_adc_close,
	.read_channels		= ambarella_adc_get_array,
	.is_irq_supported	= adc_is_irq_supported,
	.set_irq_threshold	= adc_set_irq_threshold,
	.reset			= ambarella_adc_start,
	.stop			= ambarella_adc_stop,
	.get_channel_num	= ambarella_adc_get_instances,

	.scan_delay		= 20,
};
#if defined(CONFIG_AMBARELLA_SYS_ADC_CALL)
AMBA_ADC_PARAM_CALL(ambarella_platform_adc_controller0, 0644);
#endif

struct resource ambarella_adc_resources[] = {
	[0] = {
		.start	= ADC_BASE,
		.end	= ADC_BASE + 0x0FFF,
		.name	= "registers",
		.flags	= IORESOURCE_MEM,
	},
#if (CHIP_REV == A5S || CHIP_REV == A7)
	[1] = {
		.start	= ADC_LEVEL_IRQ,
		.end	= ADC_LEVEL_IRQ,
		.name	= "adc-level-irq",
		.flags	= IORESOURCE_IRQ,
	},
#endif
};

#if 0
struct platform_device ambarella_adc0 = {
	.name		= "ambarella-adc",
	.id		= -1,
	.resource	= ambarella_adc_resources,
	.num_resources	= ARRAY_SIZE(ambarella_adc_resources),
	.dev		= {
		.platform_data		= &ambarella_platform_adc_controller0,
		.dma_mask		= &ambarella_dmamask,
		.coherent_dma_mask	= DMA_BIT_MASK(32),
	}
};
#endif

struct ambarella_adc_controller ambarella_platform_adc_temper_controller0 = {
	.open			= ambarella_adc_open,
	.close			= ambarella_adc_close,
	.read_channel		= ambarella_adc_get_channel_inline,
	.reset			= ambarella_adc_start,
	.stop			= ambarella_adc_stop,
	.get_channel_num	= ambarella_adc_get_instances,
	.temper_curve		= NULL,
	.adc_temper_channel	= -1,
};

struct platform_device ambarella_adc_temper = {
	.name		= "ambarella-adc-temper",
	.id		= -1,
	.dev		= {
		.platform_data		= &ambarella_platform_adc_temper_controller0,
		.dma_mask			= &ambarella_dmamask,
		.coherent_dma_mask	= DMA_BIT_MASK(32),
	}
};

struct platform_device sc_adc_dn = {
	.name		= "adc_dn",
	.id		= -1,
	.resource	= ambarella_adc_resources,
	.num_resources	= ARRAY_SIZE(ambarella_adc_resources),
	.dev		= {
		.platform_data		= &ambarella_platform_adc_controller0,
		.dma_mask		= &ambarella_dmamask,
		.coherent_dma_mask	= DMA_BIT_MASK(32),
	}
};
