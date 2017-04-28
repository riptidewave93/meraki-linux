/*
 * arch/arm/mach-ambarella/cpufreq-dev.c
 *
 * Author: Cao Rongrong <rrcao@ambarella.com>
 *
 * History:
 *	2011/11/04 - [Cao Rongrong] Created file
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

#include <linux/platform_device.h>
#include <linux/cpufreq.h>
#include <linux/regulator/consumer.h>
#include <linux/err.h>
#include <mach/hardware.h>
#include <mach/board.h>
#include <plat/cpufreq.h>

/* Frequency table index must be sequential starting at 0 */
static struct cpufreq_frequency_table i1_freq_table[] = {
	{ 0, 360000 },	/* INTPROG=14 */
	{ 1, 480000 },	/* INTPROG=19 */
	{ 2, 600000 },	/* INTPROG=24 */
	{ 3, 768000 },	/* INTPROG=31 */
	{ 4, 888000 },	/* INTPROG=36 */
//	{ 5, 1008000 },	/* INTPROG=41 */
	{ 6, CPUFREQ_TABLE_END },
};


#ifdef CONFIG_REGULATOR
static struct regulator *vdd_axi = NULL;
/* CPU power table, it correspond with the frequency table.  */
static unsigned int voltage_table[] = {
	[0] = 1200000,
	[1] = 1200000,
	[2] = 1200000,
	[3] = 1300000,
	[4] = 1300000,
	[5] = 1300000,
};

static int ambarella_cpu_set_voltage(unsigned int idx)
{
	int rval;

	if (!vdd_axi)
		return -ENODEV;

	rval = regulator_set_voltage(vdd_axi,
			voltage_table[i1_freq_table[idx].index],
			voltage_table[i1_freq_table[idx].index]);
	if (rval != 0) {
		pr_err("cpufreq: Failed to set VDD_AXI for %dkHz: %d\n",
		       i1_freq_table[idx].frequency, rval);
	}

	return rval;
}

static int ambarella_cpu_regulator_init(void)
{
	vdd_axi = regulator_get(NULL, "cpu_vcc");
	if (IS_ERR(vdd_axi)) {
		pr_err("Unable to obtain voltage regulator for VDD_AXI;"
			" voltage scaling unsupported\n");
		return PTR_ERR(vdd_axi);
	}

	return 0;
}

static int ambarella_cpu_regulator_exit(void)
{
	if (vdd_axi)
		regulator_put(vdd_axi);

	vdd_axi = NULL;

	return 0;
}
#endif

static struct ambarella_cpufreq_config i1evk_cpufreq_info = {
	.freq_table = i1_freq_table,
#ifdef CONFIG_REGULATOR
	.init = ambarella_cpu_regulator_init,
	.set_voltage = ambarella_cpu_set_voltage,
	.exit = ambarella_cpu_regulator_exit,
#endif
};

struct platform_device i1evk_cpufreq_device = {
	.name			= "cpufreq-ambarella",
	.id = -1,
	.dev = {
		.platform_data	= &i1evk_cpufreq_info,
	},
};

