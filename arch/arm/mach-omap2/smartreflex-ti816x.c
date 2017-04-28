/*
 * SmartReflex Voltage Control driver
 *
 * Copyright (C) 2011 Texas Instruments, Inc. - http://www.ti.com/
 * Author: AnilKumar Ch <anilkumar@ti.com>
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License as
 * published by the Free Software Foundation version 2.
 *
 * This program is distributed "as is" WITHOUT ANY WARRANTY of any
 * kind, whether express or implied; without even the implied warranty
 * of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 */


#include <linux/kernel.h>
#include <linux/init.h>
#include <linux/module.h>
#include <linux/interrupt.h>
#include <linux/clk.h>
#include <linux/io.h>
#include <linux/debugfs.h>
#include <linux/slab.h>
#include <linux/regulator/consumer.h>

#include <plat/common.h>
#include <plat/smartreflex.h>

#include "control.h"

#define MARGIN_PERCEN_ERROR	0
#define CLK_NAME_LEN		40
#define MAX_SENSORS_PER_VD	2

struct ti816x_sr_sensor {
	u32			irq;
	u32			irq_status;
	u32			efuse_offs;
	u32			nvalue;
	u32			e2v_gain;
	u32			err_weight;
	u32			err_minlimit;
	u32			err_maxlimit;
	u32			senn_en;
	u32			senp_en;
	char			*name;
	void __iomem		*base;
	struct clk		*fck;
	struct timer_list	timer;
};

struct ti816x_sr {
	u32				autocomp_active;
	u32				sens_per_vd;
	u32				irq_delay;
	u32				ip_type;
	int				init_volt_mv;
	int				uvoltage_step_size;
	struct regulator		*reg;
	struct work_struct		work;
	struct sr_platform_data		*sr_data;
	struct ti816x_sr_sensor		sen[MAX_SENSORS_PER_VD];
	struct platform_device		*pdev;
};

static inline void sr_write_reg(struct ti816x_sr *sr, int offset, u32 value,
					u32 srid)
{
	__raw_writel(value, sr->sen[srid].base + offset);
}

static inline void sr_modify_reg(struct ti816x_sr *sr, int offset, u32 mask,
				u32 value, u32 srid)
{
	u32 reg_val;

	reg_val = __raw_readl(sr->sen[srid].base + offset);
	reg_val &= ~mask;
	reg_val |= (value&mask);

	__raw_writel(reg_val, sr->sen[srid].base + offset);
}

static inline u32 sr_read_reg(struct ti816x_sr *sr, int offset, u32 srid)
{
	return __raw_readl(sr->sen[srid].base + offset);
}

/* get_errvolt - get error voltage from SR error register
 * @sr:		contains SR driver data
 * @srid:	contains the srid, specify whether it is HVT or SVT
 *
 * Read the error from SENSOR error register and then convert
 * to voltage delta, return value is the voltage delta in micro
 * volt.
 */
static int get_errvolt(struct ti816x_sr *sr, s32 srid)
{
	u32 senerror_reg;
	int uvoltage;
	s8 terror;

	senerror_reg = sr_read_reg(sr, SENERROR_V2, srid);
	senerror_reg = (senerror_reg & 0x0000FF00);
	terror = (s8)(senerror_reg >> 8);

	/* converting binary to percentage error */
	uvoltage = (int)((terror * 25) >> 5);

	uvoltage = (uvoltage + MARGIN_PERCEN_ERROR) *
				sr->uvoltage_step_size;
	uvoltage = uvoltage * sr->sen[srid].e2v_gain;

	/* converting percentage to value by dividing 100 */
	uvoltage = uvoltage/100;

	return uvoltage;
}

/* set_voltage - Schedule task for setting the voltage
 * @work:	pointer to the work structure
 *
 * Voltage is set based on previous voltage and corresponding
 * voltage changes from two sensors. Make sure that the device
 * voltage is such that it satisfies the sensor requesting for
 * the higher voltage.
 *
 * Generic voltage regulator set voltage is used for changing
 * the voltage to new value, both minimum and maximum voltage
 * values are same in this case
 *
 * Disabling the module before changing the voltage, this is
 * needed for not generating interrupt during voltage change,
 * enabling after voltage change. This will also take care of
 * resetting the nCount registers.
 */
static void set_voltage(struct work_struct *work)
{
	struct ti816x_sr *sr;
	int prev_volt, new_volt;
	s32 hvt_dvolt, svt_dvolt;

	sr = container_of(work, struct ti816x_sr, work);

	/* Get the current voltage from GPIO */
	prev_volt = regulator_get_voltage(sr->reg);

	hvt_dvolt = get_errvolt(sr, SRHVT);
	svt_dvolt = get_errvolt(sr, SRSVT);

	if (hvt_dvolt > svt_dvolt)
		new_volt = prev_volt + hvt_dvolt;
	else
		new_volt = prev_volt + svt_dvolt;

	/* Clear the counter, SR module disable */
	sr_modify_reg(sr, SRCONFIG, SRCONFIG_SRENABLE,
			~SRCONFIG_SRENABLE, SRHVT);
	sr_modify_reg(sr, SRCONFIG, SRCONFIG_SRENABLE,
			~SRCONFIG_SRENABLE, SRSVT);

	regulator_set_voltage(sr->reg, new_volt, new_volt);

	/* Restart the module after voltage set */
	sr_modify_reg(sr, SRCONFIG, SRCONFIG_SRENABLE,
			SRCONFIG_SRENABLE, SRHVT);
	sr_modify_reg(sr, SRCONFIG, SRCONFIG_SRENABLE,
			SRCONFIG_SRENABLE, SRSVT);
}

/* irq_sr_htimer - sr HVT timer callback
 * @data:	data contains the SR driver structure
 *
 * While servicing the HVT IRQ, the timer gets added by HVT IRQ
 * with some time delay and this is called once the timer elapses.
 * This will re-enable the HVT interrupt
 */
static void irq_sr_htimer(unsigned long data)
{
	struct ti816x_sr *sr;

	sr = (struct ti816x_sr *)data;

	/* Enable the interrupt */
	sr_modify_reg(sr, IRQENABLE_SET, IRQENABLE_MCUBOUNDSINT,
			IRQENABLE_MCUBOUNDSINT, SRHVT);
}

/* irq_sr_stimer - sr SVT timer callback
 * @data:	data contains the SR driver structure
 *
 * While servicing the SVT IRQ, the timer gets added by SVT IRQ
 * with some time delay and this is called once the timer elapses.
 * This will re-enable the SVT interrupt
 */
static void irq_sr_stimer(unsigned long data)
{
	struct ti816x_sr *sr;

	sr = (struct ti816x_sr *)data;

	/* Enable the interrupt */
	sr_modify_reg(sr, IRQENABLE_SET, IRQENABLE_MCUBOUNDSINT,
			IRQENABLE_MCUBOUNDSINT, SRSVT);
}

/* sr_class2_irq - sr irq handling
 * @irq:	Number of the irq serviced
 * @data:	data contains the SR driver structure
 *
 * Smartreflex IRQ handling for class2 IP, once the IRQ handler
 * is here then disable the interrupt and re-enable after some
 * time. This is the work around for handling both interrupts,
 * while one got satisfied with the voltage change but not the
 * other. The same logic helps the case where PMIC cannot set
 * the exact voltage requested by SR IP
 *
 * Schedule work only if both interrupts are serviced
 *
 * Note that same irq handler is used for both the interrupts,
 * needed for decision making for voltage change
 */
static irqreturn_t sr_class2_irq(int irq, void *data)
{
	u32 srid;
	struct ti816x_sr *sr = (struct ti816x_sr *)data;

	if (irq == TI81XX_IRQ_SMRFLX0)
		srid = SRHVT;
	else
		srid = SRSVT;

	sr->sen[srid].irq_status = 1;

	/* Clear MCUBounds Interrupt */
	sr_modify_reg(sr, IRQSTATUS, IRQSTATUS_MCBOUNDSINT,
			IRQSTATUS_MCBOUNDSINT, srid);

	/* Disable the interrupt and enable after timer expires */
	sr_modify_reg(sr, IRQENABLE_CLR, IRQENABLE_MCUBOUNDSINT,
			IRQENABLE_MCUBOUNDSINT, srid);

	if (!timer_pending(&sr->sen[srid].timer)) {
		sr->sen[srid].timer.data = (unsigned long)sr;
		if (srid == SRHVT)
			sr->sen[srid].timer.function = irq_sr_htimer;
		else
			sr->sen[srid].timer.function = irq_sr_stimer;

		sr->sen[srid].timer.expires = jiffies +
				msecs_to_jiffies(sr->irq_delay);
		add_timer(&sr->sen[srid].timer);
	} else {
		/* Timer of this interrupt should not be pending
		 * while system is running
		 */
		BUG();
	}

	if ((sr->sen[SRHVT].irq_status == 1) &&
			(sr->sen[SRSVT].irq_status == 1)) {
		schedule_work(&sr->work);
		sr->sen[SRHVT].irq_status = 0;
		sr->sen[SRSVT].irq_status = 0;
	}

	return IRQ_HANDLED;
}

static int sr_clk_enable(struct ti816x_sr *sr, u32 srid)
{
	if (clk_enable(sr->sen[srid].fck) != 0) {
		dev_err(&sr->pdev->dev, "%s: Could not enable sr_fck\n",
					__func__);
		return -EINVAL;
	}

	return 0;
}

static int sr_clk_disable(struct ti816x_sr *sr, u32 srid)
{
	clk_disable(sr->sen[srid].fck);

	return 0;
}

static inline int sr_set_nvalues(struct ti816x_sr *sr, u32 srid)
{
	/* Read nTarget value form EFUSE register*/
	sr->sen[srid].nvalue = __raw_readl(TI81XX_CTRL_REGADDR
			(sr->sen[srid].efuse_offs)) & 0xFFFFFF;

	return 0;
}

/* sr_configure - Configure SR module to work in Error generator mode
 * @sr:		contains SR driver data
 * @srid:	contains the srid, specify whether it is HVT or SVT
 *
 * Configure the corresponding values to SR module registers for
 * operating SR module in Error Generator mode.
 */
static void sr_configure(struct ti816x_sr *sr, u32 srid)
{
	/* Configuring the SR module with clock length, enabling the
	 * error generator, enable SR module, enable individual N and P
	 * sensors
	 */
	sr_write_reg(sr, SRCONFIG, (SRCLKLENGTH_125MHZ_SYSCLK |
		SRCONFIG_SENENABLE | SRCONFIG_ERRGEN_EN |
		(sr->sen[srid].senn_en << SRCONFIG_SENNENABLE_V2_SHIFT) |
		(sr->sen[srid].senp_en << SRCONFIG_SENPENABLE_V2_SHIFT)),
		srid);

	/* Configuring the Error Generator */
	sr_modify_reg(sr, ERRCONFIG_V2, (SR_ERRWEIGHT_MASK |
		SR_ERRMAXLIMIT_MASK | SR_ERRMINLIMIT_MASK),
		((sr->sen[srid].err_weight << ERRCONFIG_ERRWEIGHT_SHIFT) |
		(sr->sen[srid].err_maxlimit << ERRCONFIG_ERRMAXLIMIT_SHIFT) |
		(sr->sen[srid].err_minlimit << ERRCONFIG_ERRMINLIMIT_SHIFT)),
		srid);
}

/* sr_enable - Enable SR module
 * @sr:		contains SR driver data
 * @srid:	contains the srid, specify whether it is HVT or SVT
 *
 * Enable SR module by writing nTarget values to corresponding SR
 * NVALUERECIPROCAL register, enable the interrupt and enable SR
 */
static void sr_enable(struct ti816x_sr *sr, u32 srid)
{
	/* Check if SR is already enabled. If yes do nothing */
	if (sr_read_reg(sr, SRCONFIG, srid) & SRCONFIG_SRENABLE)
		return;

	if (sr->sen[srid].nvalue == 0)
		dev_err(&sr->pdev->dev, "%s: OPP doesn't support SmartReflex\n",
				__func__);

	/* Writing the nReciprocal value to the register */
	sr_write_reg(sr, NVALUERECIPROCAL, sr->sen[srid].nvalue, srid);

	/* Enable the interrupt */
	sr_modify_reg(sr, IRQENABLE_SET, IRQENABLE_MCUBOUNDSINT,
				IRQENABLE_MCUBOUNDSINT, srid);

	/* SRCONFIG - enable SR */
	sr_modify_reg(sr, SRCONFIG, SRCONFIG_SRENABLE,
				SRCONFIG_SRENABLE, srid);
}

/* sr_disable - Disable SR module
 * @sr:		contains SR driver data
 * @srid:	contains the srid, specify whether it is HVT or SVT
 *
 * Disable SR module by disabling the interrupt and Smartrefelx module
 */
static void sr_disable(struct ti816x_sr *sr, u32 srid)
{
	/* Disable the interrupt */
	sr_modify_reg(sr, IRQENABLE_CLR, IRQENABLE_MCUBOUNDSINT,
				IRQENABLE_MCUBOUNDSINT, srid);

	/* SRCONFIG - disable SR */
	sr_modify_reg(sr, SRCONFIG, SRCONFIG_SRENABLE,
				~SRCONFIG_SRENABLE, srid);
}

/* sr_start_vddautocomp - Start VDD auto compensation
 * @sr:		contains SR driver data
 *
 * This is the starting point for AVS enable from user space.
 */
static void sr_start_vddautocomp(struct ti816x_sr *sr)
{
	int i;
	int ret;

	if ((sr->sen[SRHVT].nvalue == 0) || (sr->sen[SRSVT].nvalue == 0)) {
		dev_err(&sr->pdev->dev, "SR module not enabled, nTarget"
					" values are not found\n");
		return;
	}

	if (sr->autocomp_active == 1) {
		dev_warn(&sr->pdev->dev, "SR VDD autocomp is already active\n");
		return;
	}

	ret = regulator_enable(sr->reg);
	if (ret) {
		dev_err(&sr->pdev->dev, "Failed to enable supply: %d\n", ret);
		return;
	}

	for (i = 0; i < sr->sens_per_vd; i++) {
		init_timer(&sr->sen[i].timer);
		sr_clk_enable(sr, i);
		sr_configure(sr, i);
		sr_enable(sr, i);
	}

	sr->autocomp_active = 1;
}

/* sr_stop_vddautocomp - Stop VDD auto compensation
 * @sr:		contains SR driver data
 *
 * This is the ending point during SR disable from user space.
 */
static void sr_stop_vddautocomp(struct ti816x_sr *sr)
{
	int i;

	if (sr->autocomp_active == 0) {
		dev_warn(&sr->pdev->dev, "SR VDD autocomp is not active\n");
		return;
	}

	cancel_work_sync(&sr->work);

	regulator_disable(sr->reg);
	for (i = 0; i < sr->sens_per_vd; i++) {
		sr_disable(sr, i);
		del_timer_sync(&sr->sen[i].timer);
		sr_clk_disable(sr, i);
	}

	regulator_set_voltage(sr->reg, sr->init_volt_mv, sr->init_volt_mv);
	sr->autocomp_active = 0;
}

/* ti816x_sr_autocomp_show - Store user input value and stop SR
 * @data:		contains SR driver data
 * @val:		pointer to store autocomp_active status
 *
 * This is the Debug Fs enteries to show whether SR is enabled
 * or disabled
 */
static int ti816x_sr_autocomp_show(void *data, u64 *val)
{
	struct ti816x_sr *sr_info = (struct ti816x_sr *) data;

	*val = (u64) sr_info->autocomp_active;

	return 0;
}

/* ti816x_sr_autocomp_store - Store user input and start SR
 * @data:		contains SR driver data
 * @val:		contains the value pased by user
 *
 * This is the Debug Fs enteries to store user input and
 * enable smartreflex.
 */
static int ti816x_sr_autocomp_store(void *data, u64 val)
{
	struct ti816x_sr *sr_info = (struct ti816x_sr *) data;

	/* Sanity check */
	if (val && (val != 1)) {
		dev_warn(&sr_info->pdev->dev, "%s: Invalid argument %llu\n",
				__func__, val);
		return -EINVAL;
	}

	if (!val)
		sr_stop_vddautocomp(sr_info);
	else
		sr_start_vddautocomp(sr_info);

	return 0;
}

DEFINE_SIMPLE_ATTRIBUTE(sr_fops, ti816x_sr_autocomp_show,
		ti816x_sr_autocomp_store, "%llu\n");

/* sr_curr_volt_show - Show current voltage value
 * @data:		contains SR driver data
 * @val:		pointer to store current voltage value
 *
 * Read the current voltage value and display the same on console
 * This is used in debugfs entries
 */
static int sr_curr_volt_show(void *data, u64 *val)
{
	struct ti816x_sr *sr_info = (struct ti816x_sr *) data;

	*val = (u64) regulator_get_voltage(sr_info->reg);

	return 0;
}

DEFINE_SIMPLE_ATTRIBUTE(curr_volt_fops, sr_curr_volt_show,
		NULL, "%llu\n");

#ifdef CONFIG_DEBUG_FS
/* sr_debugfs_entries - Create debugfs entries
 * @sr_info:		contains SR driver data
 *
 * Create debugfs entries, which is exposed to user for knowing
 * the current status. Some of the parameters can change during
 * run time
 */
static int sr_debugfs_entries(struct ti816x_sr *sr_info)
{
	struct dentry *dbg_dir, *sen_dir;
	int i;

	dbg_dir = debugfs_create_dir("smartreflex", NULL);
	if (IS_ERR(dbg_dir)) {
		dev_err(&sr_info->pdev->dev, "%s: Unable to create debugfs"
				" directory\n", __func__);
		return PTR_ERR(dbg_dir);
	}

	(void) debugfs_create_file("autocomp", S_IRUGO | S_IWUGO, dbg_dir,
				(void *)sr_info, &sr_fops);
	(void) debugfs_create_u32("initial_voltage", S_IRUGO, dbg_dir,
				&sr_info->init_volt_mv);
	(void) debugfs_create_file("current_voltage", S_IRUGO, dbg_dir,
				(void *)sr_info, &curr_volt_fops);
	(void) debugfs_create_u32("interrupt_delay", S_IRUGO | S_IWUGO,
				dbg_dir, &sr_info->irq_delay);

	for (i = 0; i < sr_info->sens_per_vd; i++) {

		sen_dir = debugfs_create_dir(sr_info->sen[i].name, dbg_dir);
		if (IS_ERR(sen_dir)) {
			dev_err(&sr_info->pdev->dev, "%s: Unable to create"
				" debugfs directory\n", __func__);
			return PTR_ERR(sen_dir);
		}

		(void) debugfs_create_x32("err2voltgain", S_IRUGO,
					sen_dir, &sr_info->sen[i].e2v_gain);
		(void) debugfs_create_x32("nvalue", S_IRUGO,
					sen_dir, &sr_info->sen[i].nvalue);
	}
	return 0;
}
#else
static int sr_debugfs_entries(struct ti816x_sr *sr_info)
{
	return 0;
}
#endif

static int __init ti816x_sr_probe(struct platform_device *pdev)
{
	struct ti816x_sr *sr_info;
	struct ti816x_sr_platform_data *pdata;
	struct resource *res[MAX_SENSORS_PER_VD];
	char *name[MAX_SENSORS_PER_VD];
	int irq;
	int ret;
	int i;

	sr_info = kzalloc(sizeof(struct ti816x_sr), GFP_KERNEL);
	if (!sr_info) {
		dev_err(&pdev->dev, "%s: unable to allocate sr_info\n",
					__func__);
		return -ENOMEM;
	}

	pdata = pdev->dev.platform_data;
	if (!pdata) {
		dev_err(&pdev->dev, "%s: platform data missing\n", __func__);
		ret = -EINVAL;
		goto err_free_sr_info;
	}

	sr_info->pdev = pdev;
	sr_info->sen[SRHVT].name = "sr_hvt";
	sr_info->sen[SRSVT].name = "sr_svt";
	sr_info->ip_type = pdata->ip_type;
	sr_info->irq_delay = pdata->irq_delay;
	sr_info->sens_per_vd = pdata->no_of_sens/pdata->no_of_vds;
	sr_info->uvoltage_step_size = pdata->vstep_size_uv;
	sr_info->autocomp_active = false;

	/* Reading nTarget Values */
	for (i = 0; i < sr_info->sens_per_vd; i++) {
		sr_info->sen[i].efuse_offs = pdata->sr_sdata[i].efuse_offs;
		sr_set_nvalues(sr_info, i);
	}

	if ((sr_info->sen[SRHVT].nvalue == 0) ||
			(sr_info->sen[SRSVT].nvalue == 0)) {
		dev_err(&pdev->dev, "Driver is not initialized,"
				" nTarget values are not found\n");
		ret = 0;
		goto err_free_sr_info;
	}

	INIT_WORK(&sr_info->work, set_voltage);

	for (i = 0; i < sr_info->sens_per_vd; i++) {

		name[i] = kzalloc(CLK_NAME_LEN + 1, GFP_KERNEL);
		/* resources */
		res[i] = platform_get_resource_byname(pdev, IORESOURCE_MEM,
					sr_info->sen[i].name);
		if (!res[i]) {
			dev_err(&pdev->dev, "%s: no mem resource\n", __func__);
			ret = -ENOENT;
			goto err_free_mem;
		}

		irq = platform_get_irq_byname(pdev, sr_info->sen[i].name);
		if (irq < 0) {
			dev_err(&pdev->dev, "Can't get interrupt resource\n");
			ret = irq;
			goto err_free_mem;
		}
		sr_info->sen[i].irq = irq;

		res[i] = request_mem_region(res[i]->start,
				resource_size(res[i]), pdev->name);
		if (!res[i]) {
			dev_err(&pdev->dev, "can't request mem region\n");
			ret = -EBUSY;
			goto err_free_reg;
		}

		sr_info->sen[i].base = ioremap(res[i]->start,
				resource_size(res[i]));
		if (!sr_info->sen[i].base) {
			dev_err(&pdev->dev, "%s: ioremap fail\n", __func__);
			ret = -ENOMEM;
			goto err_release_mem;
		}

		strcat(name[i], sr_info->sen[i].name);
		strcat(name[i], "_fck");

		sr_info->sen[i].fck = clk_get(NULL, name[i]);
		if (IS_ERR(sr_info->sen[i].fck)) {
			dev_err(&pdev->dev, "%s: Could not get sr fck\n",
						__func__);
			ret = PTR_ERR(sr_info->sen[i].fck);
			goto err_unmap;
		}

		ret = request_irq(sr_info->sen[i].irq, sr_class2_irq,
			IRQF_DISABLED, sr_info->sen[i].name, (void *)sr_info);
		if (ret) {
			dev_err(&pdev->dev, "%s: Could not install SR ISR\n",
						__func__);
			goto err_put_clock;
		}

		sr_info->sen[i].e2v_gain = pdata->sr_sdata[i].e2v_gain;
		sr_info->sen[i].err_weight = pdata->sr_sdata[i].err_weight;
		sr_info->sen[i].err_minlimit = pdata->sr_sdata[i].err_minlimit;
		sr_info->sen[i].err_maxlimit = pdata->sr_sdata[i].err_maxlimit;
		sr_info->sen[i].senn_en = pdata->sr_sdata[i].senn_mod;
		sr_info->sen[i].senp_en = pdata->sr_sdata[i].senp_mod;
	}

	/* debugfs entries */
	ret = sr_debugfs_entries(sr_info);
	if (ret)
		dev_warn(&pdev->dev, "%s: Debugfs entries are not created\n",
						__func__);

	sr_info->reg = regulator_get(NULL, pdata->vd_name);
	if (IS_ERR(sr_info->reg))
		goto err_free_irq;

	/* Read current GPIO value and voltage */
	sr_info->init_volt_mv = regulator_get_voltage(sr_info->reg);

	platform_set_drvdata(pdev, sr_info);

	dev_info(&pdev->dev, "Driver initialized\n");

	if (pdata->enable_on_init)
		sr_start_vddautocomp(sr_info);

	return ret;

err_free_irq:
	for (i = 0; i < sr_info->sens_per_vd; i++)
		free_irq(sr_info->sen[i].irq, (void *)sr_info);

err_put_clock:
	if (i == 1)
		free_irq(sr_info->sen[i-1].irq, pdev);
	for (i = 0; i < sr_info->sens_per_vd; i++)
		clk_put(sr_info->sen[i].fck);

err_unmap:
	if (i == 1)
		clk_put(sr_info->sen[i-1].fck);
	for (i = 0; i < sr_info->sens_per_vd; i++)
		iounmap(sr_info->sen[i].base);

err_release_mem:
	if (i == 1)
		iounmap(sr_info->sen[i-1].base);
	for (i = 0; i < sr_info->sens_per_vd; i++)
		release_mem_region(res[i]->start, resource_size(res[i]));

err_free_reg:
	if (i == 1)
		release_mem_region(res[i-1]->start, resource_size(res[i-1]));

err_free_mem:
	if (i == 1)
		kfree(name[i-1]);
	for (i = 0; i < sr_info->sens_per_vd; i++)
		kfree(name[i]);

err_free_sr_info:
	kfree(sr_info);
	return ret;
}

static int __devexit ti816x_sr_remove(struct platform_device *pdev)
{
	struct ti816x_sr *sr_info;
	struct resource *res[MAX_SENSORS_PER_VD];
	int irq;
	int i;

	sr_info = dev_get_drvdata(&pdev->dev);
	if (!sr_info) {
		dev_err(&pdev->dev, "%s: sr_info missing\n", __func__);
		return -EINVAL;
	}

	if (sr_info->autocomp_active)
		sr_stop_vddautocomp(sr_info);

	for (i = 0; i < sr_info->sens_per_vd; i++) {
		clk_put(sr_info->sen[i].fck);

		iounmap(sr_info->sen[i].base);
		res[i] = platform_get_resource_byname(pdev,
				IORESOURCE_MEM, sr_info->sen[i].name);

		release_mem_region(res[i]->start, resource_size(res[i]));

		irq = platform_get_irq_byname(pdev, sr_info->sen[i].name);
		free_irq(irq, (void *)sr_info);
	}

	kfree(sr_info);

	return 0;
}

static struct platform_driver smartreflex_driver = {
	.driver		= {
		.name	= "smartreflex",
		.owner	= THIS_MODULE,
	},
	.remove		= ti816x_sr_remove,
};

static int __init sr_init(void)
{
	int ret;

	ret = platform_driver_probe(&smartreflex_driver, ti816x_sr_probe);
	if (ret) {
		pr_err("%s: platform driver register failed\n", __func__);
		return ret;
	}

	return 0;
}

static void __exit sr_exit(void)
{
	platform_driver_unregister(&smartreflex_driver);
}
late_initcall(sr_init);
module_exit(sr_exit);

MODULE_DESCRIPTION("TI816X Smartreflex Class2 Driver");
MODULE_LICENSE("GPL");
MODULE_ALIAS("platform:" DRIVER_NAME);
MODULE_AUTHOR("Texas Instruments Inc");
