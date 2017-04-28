/*
 * arch/arm/plat-ambarella/generic/pll_hal.c
 *
 * Author: Cao Rongrong, <rrcao@ambarella.com>
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
#include <linux/proc_fs.h>
#include <linux/cpu.h>
#include <linux/export.h>

#include <asm/uaccess.h>
#include <asm/localtimer.h>

#include <mach/hardware.h>

#include <plat/timer.h>
#include <plat/ambcache.h>
#include <plat/event.h>

/* ==========================================================================*/
#define AMBPLL_MAX_CMD_LENGTH			(32)

/* ==========================================================================*/
struct ambarella_pll_info {
	amb_operating_mode_t operating_mode;
};

#if (CHIP_REV == I1 || CHIP_REV == AMB_S2)
struct ambarella_pll_vidcap_info {
	char *name;
	unsigned int vidcap;
};
#endif

struct ambarella_pll_mode_info {
	char *name;
	unsigned int mode;
};

struct ambarella_pll_performance_info {
	char *name;
	unsigned int performance;
};

/* ==========================================================================*/
static inline unsigned long cpufreq_scale_copy(unsigned long old,
	u_int div, u_int mult)
{
#if BITS_PER_LONG == 32

	u64 result = ((u64) old) * ((u64) mult);
	do_div(result, div);
	return (unsigned long) result;

#elif BITS_PER_LONG == 64

	unsigned long result = old * ((u64) mult);
	result /= div;
	return result;

#endif
};

extern unsigned long loops_per_jiffy;
static inline unsigned int ambarella_adjust_jiffies(unsigned long val,
	unsigned int oldfreq, unsigned int newfreq)
{
	if (((val == AMBA_EVENT_PRE_CPUFREQ) && (oldfreq < newfreq)) ||
		((val == AMBA_EVENT_POST_CPUFREQ) && (oldfreq != newfreq))) {
		loops_per_jiffy = cpufreq_scale_copy(loops_per_jiffy,
			oldfreq, newfreq);

		return newfreq;
	}

	return oldfreq;
}

/* ==========================================================================*/
static struct proc_dir_entry *mode_file = NULL;
static struct proc_dir_entry *performance_file = NULL;
static struct ambarella_pll_info pll_info;

#if (CHIP_REV == A5S)

static struct ambarella_pll_mode_info mode_list[] = {
	{"3d_playback", AMB_OPERATING_MODE_3D_PLAYBACK},
	{"preview", AMB_OPERATING_MODE_PREVIEW},
	{"still_capture", AMB_OPERATING_MODE_STILL_CAPTURE},
	{"capture_nopreview", AMB_OPERATING_MODE_CAPTURE_NOPREVIEW},
	{"capture", AMB_OPERATING_MODE_CAPTURE},
	{"playback", AMB_OPERATING_MODE_PLAYBACK},
	{"display_and_arm", AMB_OPERATING_MODE_DISPLAY_AND_ARM},
	{"standby", AMB_OPERATING_MODE_STANDBY},
	{"lcd_bypass", AMB_OPERATING_MODE_LCD_BYPASS},
	{"still_preview", AMB_OPERATING_MODE_STILL_PREVIEW},
	{"lowpower", AMB_OPERATING_MODE_LOW_POWER},
	{"raw", AMB_OPERATING_MODE_RAW},
	{"ipcam", AMB_OPERATING_MODE_IP_CAM},
	{"ipcam_HP", AMB_OPERATING_MODE_IP_CAM_HP},
	{"ipcam_HP1_372MHZ", AMB_OPERATING_MODE_IP_CAM_HP1},
	{"ipcam_HP2_384MHZ", AMB_OPERATING_MODE_IP_CAM_HP2},
	{"ipcam_HP3_396MHZ", AMB_OPERATING_MODE_IP_CAM_HP3},
};

static struct ambarella_pll_performance_info performance_list[] = {
	{"480P30", AMB_PERFORMANCE_480P30},
	{"720P30", AMB_PERFORMANCE_720P30},
	{"720P60", AMB_PERFORMANCE_720P60},
	{"1080I60_LowPower", AMB_PERFORMANCE_1080I60_LP},
	{"1080I60", AMB_PERFORMANCE_1080I60},
	{"1080P30", AMB_PERFORMANCE_1080P30},
	{"1080P50", AMB_PERFORMANCE_1080P50},
	{"1080P60", AMB_PERFORMANCE_1080P60},
	{"2160P60", AMB_PERFORMANCE_2160P60},
};

#elif (CHIP_REV == A7L)

static struct ambarella_pll_mode_info mode_list[] = {
	{"preview", AMB_OPERATING_MODE_PREVIEW},
	{"still_capture", AMB_OPERATING_MODE_STILL_CAPTURE},
	{"capture", AMB_OPERATING_MODE_CAPTURE},
	{"playback", AMB_OPERATING_MODE_PLAYBACK},
	{"display_and_arm", AMB_OPERATING_MODE_DISPLAY_AND_ARM},
	{"standby", AMB_OPERATING_MODE_STANDBY},
	{"lcd_bypass", AMB_OPERATING_MODE_LCD_BYPASS},
	{"still_preview", AMB_OPERATING_MODE_STILL_PREVIEW},
	{"lowpower", AMB_OPERATING_MODE_LOW_POWER},
	{"audio_playback", AMB_OPERATING_MODE_AUDIO_PLAYBACK},
	{"audio_capture", AMB_OPERATING_MODE_AUDIO_CAPTURE},
};

static struct ambarella_pll_performance_info performance_list[] = {
	{"480P30", AMB_PERFORMANCE_480P30},
	{"720P30", AMB_PERFORMANCE_720P30},
	{"720P60", AMB_PERFORMANCE_720P60},
	{"1080I60", AMB_PERFORMANCE_1080I60},
	{"1080P30", AMB_PERFORMANCE_1080P30},
	{"1080P60", AMB_PERFORMANCE_1080P60},
	{"2160P60", AMB_PERFORMANCE_2160P60},
};

#elif (CHIP_REV == AMB_S2)
static struct proc_dir_entry *vidcap_file = NULL;

static struct ambarella_pll_mode_info mode_list[] = {
	{"preview", AMB_OPERATING_MODE_PREVIEW},
	{"still_capture", AMB_OPERATING_MODE_STILL_CAPTURE},
	{"capture", AMB_OPERATING_MODE_CAPTURE},
	{"playback", AMB_OPERATING_MODE_PLAYBACK},
	{"display_and_arm", AMB_OPERATING_MODE_DISPLAY_AND_ARM},
	{"standby", AMB_OPERATING_MODE_STANDBY},
	{"lcd_bypass", AMB_OPERATING_MODE_LCD_BYPASS},
	{"still_preview", AMB_OPERATING_MODE_STILL_PREVIEW},
	{"lowpower", AMB_OPERATING_MODE_LOW_POWER},
	{"ipcam", AMB_OPERATING_MODE_IP_CAM},
};

static struct ambarella_pll_performance_info performance_list[] = {
	{"480P30", AMB_PERFORMANCE_480P30},
	{"720P30", AMB_PERFORMANCE_720P30},
	{"720P60", AMB_PERFORMANCE_720P60},
	{"1080I60", AMB_PERFORMANCE_1080I60},
	{"1080P30", AMB_PERFORMANCE_1080P30},
	{"1080P60", AMB_PERFORMANCE_1080P60},
	{"4KP30", AMB_PERFORMANCE_4KP30},
};

static struct ambarella_pll_vidcap_info vidcap_list[] = {
	{"4kp30", AMB_VIDCAP_4KP30},
	{"4096x2176_60fps", AMB_VIDCAP_4096X2176_60FPS},
	{"4096x3575", AMB_VIDCAP_4096X3575},
	{"4000x2250", AMB_VIDCAP_4000X2250},
	{"2304x1296", AMB_VIDCAP_2304X1296},
	{"1984x1116", AMB_VIDCAP_1984X1116},
	{"2048x1536", AMB_VIDCAP_2048X1536},
	{"1312x984", AMB_VIDCAP_1312X984},
	{"1536x384", AMB_VIDCAP_1536X384},
	{"1536x384_small_vb", AMB_VIDCAP_1536X384_SMALL_VB},
};

#elif (CHIP_REV == I1)
static struct proc_dir_entry *vidcap_file = NULL;

static struct ambarella_pll_mode_info mode_list[] = {
	{"preview", AMB_OPERATING_MODE_PREVIEW},
	{"still_capture", AMB_OPERATING_MODE_STILL_CAPTURE},
	{"capture", AMB_OPERATING_MODE_CAPTURE},
	{"playback", AMB_OPERATING_MODE_PLAYBACK},
	{"display_and_arm", AMB_OPERATING_MODE_DISPLAY_AND_ARM},
	{"standby", AMB_OPERATING_MODE_STANDBY},
	{"lcd_bypass", AMB_OPERATING_MODE_LCD_BYPASS},
	{"still_preview", AMB_OPERATING_MODE_STILL_PREVIEW},
	{"lowpower", AMB_OPERATING_MODE_LOW_POWER},
	{"lowpower_cortex", AMB_OPERATING_MODE_LOW_POWER_WITH_CORTEX},
	{"auido_playback", AMB_OPERATING_MODE_AUDIO_PLAYBACK},
	{"auido_capture", AMB_OPERATING_MODE_AUDIO_CAPTURE},
	{"duplex", AMB_OPERATING_MODE_DUPLEX},
	{"nvr", AMB_OPERATING_MODE_NVR},
};

static struct ambarella_pll_performance_info performance_list[] = {
	{"480P30", AMB_PERFORMANCE_480P30},
	{"720P30", AMB_PERFORMANCE_720P30},
	{"720P60", AMB_PERFORMANCE_720P60},
	{"1080I60", AMB_PERFORMANCE_1080I60},
	{"1080P30", AMB_PERFORMANCE_1080P30},
	{"1080P60", AMB_PERFORMANCE_1080P60},
	{"2160P60", AMB_PERFORMANCE_2160P60},
};

static struct ambarella_pll_vidcap_info vidcap_list[] = {
	{"2304x1296_60fps", AMB_VIDCAP_2304X1296_60FPS},
	{"1296x1787", AMB_VIDCAP_1296X1787},
	{"2048x1536", AMB_VIDCAP_2048X1536},
	{"2240x1260", AMB_VIDCAP_2240X1260},
	{"1984x1116", AMB_VIDCAP_1984X1116},
	{"2112x1188", AMB_VIDCAP_2112X1188},
	{"1312x984", AMB_VIDCAP_1312X984},
	{"1088x816", AMB_VIDCAP_1088X816},
};

#elif (CHIP_REV == A8)

static struct ambarella_pll_mode_info mode_list[] = {
	{"preview", AMB_OPERATING_MODE_PREVIEW},
	{"still_capture", AMB_OPERATING_MODE_STILL_CAPTURE},
	{"capture", AMB_OPERATING_MODE_CAPTURE},
	{"playback", AMB_OPERATING_MODE_PLAYBACK},
	{"display_and_arm", AMB_OPERATING_MODE_DISPLAY_AND_ARM},
	{"standby", AMB_OPERATING_MODE_STANDBY},
	{"still_preview", AMB_OPERATING_MODE_STILL_PREVIEW},
	{"lowpower", AMB_OPERATING_MODE_LOW_POWER},
	{"auido_playback", AMB_OPERATING_MODE_AUDIO_PLAYBACK},
	{"auido_capture", AMB_OPERATING_MODE_AUDIO_CAPTURE},
};

static struct ambarella_pll_performance_info performance_list[] = {
	{"480P30", AMB_PERFORMANCE_480P30},
	{"720P30", AMB_PERFORMANCE_720P30},
	{"720P60", AMB_PERFORMANCE_720P60},
	{"1080I60", AMB_PERFORMANCE_1080I60},
	{"1080P30", AMB_PERFORMANCE_1080P30},
	{"1080P60", AMB_PERFORMANCE_1080P60},
};
#endif

/* ==========================================================================*/
static int ambarella_pll_proc_read(char *page, char **start,
	off_t off, int count, int *eof, void *data)
{
	amb_hal_success_t			result;
	amb_operating_mode_t			operating_mode;
	int					retlen = 0;
	int					i;

	result = amb_get_operating_mode(HAL_BASE_VP, &operating_mode);
	if(result != AMB_HAL_SUCCESS){
		pr_err("%s: amb_get_operating_mode failed(%d)\n",
			__func__, result);
		retlen = -EPERM;
		goto ambarella_pll_proc_read_exit;
	}

	if (off != 0) {
		retlen = 0;
		goto ambarella_pll_proc_read_exit;
	}

#if (CHIP_REV == I1 || CHIP_REV == AMB_S2)
	retlen += scnprintf((page + retlen), (count - retlen),
			"\nPossible Vidcap:\n");
	for (i = 0; i < ARRAY_SIZE(vidcap_list); i++) {
		retlen += scnprintf((page + retlen), (count - retlen),
				"\t%s\n", vidcap_list[i].name);
	}
#endif
	retlen += scnprintf((page + retlen), (count - retlen),
			"\nPossible Mode:\n");
	for (i = 0; i < ARRAY_SIZE(mode_list); i++) {
		retlen += scnprintf((page + retlen), (count - retlen),
				"\t%s\n", mode_list[i].name);
	}

	retlen += scnprintf((page + retlen), (count - retlen),
			"\nPossible Performance:\n");
	for (i = 0; i < ARRAY_SIZE(performance_list); i++) {
		retlen += scnprintf((page + retlen), (count - retlen),
				"\t%s\n", performance_list[i].name);
	}

	retlen += scnprintf((page + retlen), (count - retlen),
			"\nPLL Information:\n"
#if (CHIP_REV == I1 || CHIP_REV == AMB_S2)
			"\tVidcap:\t\t%s\n"
#endif
			"\tPerformance:\t%s\n"
			"\tMode:\t\t%s\n"
#if (CHIP_REV != A8)
			"\tUSB:\t\t%s\n"
#endif
			"\tHDMI:\t\t%s\n"
			"\tDualStream:\t%s\n"
#if (CHIP_REV == A5S)
			"\tHDpreview:\t%s\n"
#endif
#if (CHIP_REV == I1 || CHIP_REV == AMB_S2)
			"\tDigitalGamma:\t%s\n"
#endif
			"\tARM:\t\t%u Hz\n"
			"\tDram:\t\t%u Hz\n"
			"\tiDSP:\t\t%u Hz\n"
			"\tCore:\t\t%u Hz\n"
#if (CHIP_REV == I1 || CHIP_REV == AMB_S2 || CHIP_REV == A8)
			"\tCortex:\t\t%u Hz\n"
			"\tAXI:\t\t%u Hz\n"
#endif
#if (CHIP_REV == I1)
			"\tDDD:\t\t%u Hz\n"
#endif
			"\tAHB:\t\t%u Hz\n"
			"\tAPB:\t\t%u Hz\n"
			"\tVOUT:\t\t%u Hz\n"
#if (CHIP_REV != A8)
			"\tVOUT2:\t\t%u Hz\n"
#endif
			"\tVIN:\t\t%u Hz\n"
			"\tHDMI:\t\t%u Hz\n"
			"\tAudio:\t\t%u Hz\n"
			"\tADC:\t\t%u Hz\n"
			"\tSSI:\t\t%u Hz\n"
#if (CHIP_REV != A8)
			"\tSSI2:\t\t%u Hz\n"
#endif
			"\tUART:\t\t%u Hz\n"
#if (CHIP_REV == I1 || CHIP_REV == AMB_S2)
			"\tGTX:\t\t%u Hz\n"
#endif
#if (CHIP_REV == I1)
			"\tSDXC:\t\t%u Hz\n"
#endif
#if (CHIP_REV == A7L || CHIP_REV == AMB_S2)
			"\tSDIO:\t\t%u Hz\n"
#endif
			"\tSD:\t\t%u Hz\n\n",
#if (CHIP_REV == I1 || CHIP_REV == AMB_S2)
			vidcap_list[operating_mode.vidcap_size].name,
#endif
			performance_list[operating_mode.performance].name,
			mode_list[operating_mode.mode].name,
#if (CHIP_REV != A8)
			operating_mode.usb_state ? "On" : "Off",
#endif
			operating_mode.hdmi_state ? "On" : "Off",
			operating_mode.dual_stream_state ? "On" : "Off",
#if (CHIP_REV == A5S)
			operating_mode.hd_preview_state ? "On" : "Off",
#endif
#if (CHIP_REV == I1 || CHIP_REV == AMB_S2)
			operating_mode.amb_digital_gamma_mode ? "On" : "Off",
#endif
			amb_get_arm_clock_frequency(HAL_BASE_VP),
			amb_get_ddr_clock_frequency(HAL_BASE_VP),
			amb_get_idsp_clock_frequency(HAL_BASE_VP),
			get_core_bus_freq_hz(),
#if (CHIP_REV == I1 || CHIP_REV == AMB_S2 || CHIP_REV == A8)
			amb_get_cortex_clock_frequency(HAL_BASE_VP),
			amb_get_axi_clock_frequency(HAL_BASE_VP),
#endif
#if (CHIP_REV == I1)
			amb_get_3d_clock_frequency(HAL_BASE_VP),
#endif
			get_ahb_bus_freq_hz(),
			get_apb_bus_freq_hz(),
			amb_get_vout_clock_frequency(HAL_BASE_VP),
#if (CHIP_REV != A8)
			amb_get_lcd_clock_frequency(HAL_BASE_VP),
#endif
			amb_get_sensor_clock_frequency(HAL_BASE_VP),
			amb_get_hdmi_clock_frequency(HAL_BASE_VP),
			amb_get_audio_clock_frequency(HAL_BASE_VP),
			amb_get_adc_clock_frequency(HAL_BASE_VP),
			amb_get_ssi_clock_frequency(HAL_BASE_VP),
#if (CHIP_REV != A8)
			amb_get_ssi2_clock_frequency(HAL_BASE_VP),
#endif
			amb_get_uart_clock_frequency(HAL_BASE_VP),
#if (CHIP_REV == I1 || CHIP_REV == AMB_S2)
			amb_get_gtx_clock_frequency(HAL_BASE_VP),
#endif
#if (CHIP_REV == I1)
			amb_get_sdxc_clock_frequency(HAL_BASE_VP),
#endif
#if (CHIP_REV == A7L || CHIP_REV == AMB_S2)
			amb_get_sdio_clock_frequency(HAL_BASE_VP),
#endif
			amb_get_sd_clock_frequency(HAL_BASE_VP));

	*eof = 1;

ambarella_pll_proc_read_exit:
	return retlen;
}

int ambarella_set_operating_mode(amb_operating_mode_t *popmode)
{
	int					retval = 0;
	amb_hal_success_t			result = AMB_HAL_SUCCESS;
	unsigned int				oldfreq, newfreq;
	unsigned long				flags;

	retval = notifier_to_errno(
		ambarella_set_event(AMBA_EVENT_PRE_CPUFREQ, NULL));
	if (retval) {
		pr_err("%s: AMBA_EVENT_PRE_CPUFREQ failed(%d)\n",
			__func__, retval);
	}

	disable_nonboot_cpus();
	local_irq_save(flags);

	retval = notifier_to_errno(
		ambarella_set_raw_event(AMBA_EVENT_PRE_CPUFREQ, NULL));
	if (retval) {
		pr_err("%s: AMBA_EVENT_PRE_CPUFREQ failed(%d)\n",
			__func__, retval);
	}

#if defined(CONFIG_PLAT_AMBARELLA_CORTEX)
	oldfreq = (unsigned int)amb_get_cortex_clock_frequency(HAL_BASE_VP);
#else
	oldfreq = (unsigned int)amb_get_arm_clock_frequency(HAL_BASE_VP);
#endif

	ambarella_timer_suspend(1);
	ambcache_pli_range(HAL_BASE_VP, ambarella_hal_get_size());
	result = amb_set_operating_mode(HAL_BASE_VP, popmode);
	if (result != AMB_HAL_SUCCESS) {
		pr_err("%s: amb_set_operating_mode failed(%d)\n",
			__func__, result);
		retval = -EPERM;
	}
	ambarella_timer_resume(1);

#if defined(CONFIG_PLAT_AMBARELLA_CORTEX)
	newfreq = (unsigned int)amb_get_cortex_clock_frequency(HAL_BASE_VP);
#else
	newfreq = (unsigned int)amb_get_arm_clock_frequency(HAL_BASE_VP);
#endif
	ambarella_adjust_jiffies(AMBA_EVENT_POST_CPUFREQ, oldfreq, newfreq);

	retval = notifier_to_errno(
		ambarella_set_raw_event(AMBA_EVENT_POST_CPUFREQ, NULL));
	if (retval) {
		pr_err("%s: AMBA_EVENT_POST_CPUFREQ failed(%d)\n",
			__func__, retval);
	}

	local_irq_restore(flags);
	enable_nonboot_cpus();

	retval = notifier_to_errno(
		ambarella_set_event(AMBA_EVENT_POST_CPUFREQ, NULL));
	if (retval) {
		pr_err("%s: AMBA_EVENT_POST_CPUFREQ failed(%d)\n",
			__func__, retval);
	}

	result = amb_get_operating_mode(HAL_BASE_VP, popmode);
	if (result != AMB_HAL_SUCCESS) {
		pr_err("%s: amb_get_operating_mode failed(%d)\n",
			__func__, result);
		retval = -EPERM;
	}

	return retval;
}

static int ambarella_mode_proc_write(struct file *file,
	const char __user *buffer, unsigned long count, void *data)
{
	int					retval = 0;
	char					str[AMBPLL_MAX_CMD_LENGTH];
	int					mode, i;
	amb_hal_success_t			result;

	i = (count < AMBPLL_MAX_CMD_LENGTH) ? count : AMBPLL_MAX_CMD_LENGTH;
	if (copy_from_user(str, buffer, i)) {
		pr_err("%s: copy_from_user fail!\n", __func__);
		retval = -EFAULT;
		goto ambarella_mode_proc_write_exit;
	}
	str[i - 1] = 0;

	for (i = 0; i < ARRAY_SIZE(mode_list); i++) {
		if (strlen(str) == strlen(mode_list[i].name)
			&& strcmp(str, mode_list[i].name) == 0) {
			mode = mode_list[i].mode;
			break;
		}
	}
	if (i >= ARRAY_SIZE(mode_list)) {
		pr_err("%s: invalid mode (%s)!\n", __func__, str);
		retval = -EINVAL;
		goto ambarella_mode_proc_write_exit;
	}

	result = amb_get_operating_mode(HAL_BASE_VP, &pll_info.operating_mode);
	if(result != AMB_HAL_SUCCESS){
		pr_err("%s: amb_get_operating_mode failed(%d)\n",
			__func__, result);
		retval = -EPERM;
		goto ambarella_mode_proc_write_exit;
	}

	if (pll_info.operating_mode.mode != mode) {
		pll_info.operating_mode.mode = mode;
		retval = ambarella_set_operating_mode(&pll_info.operating_mode);
	}
	if (!retval)
		retval = count;

ambarella_mode_proc_write_exit:
	return retval;
}

static int ambarella_performance_proc_write(struct file *file,
	const char __user *buffer, unsigned long count, void *data)
{
	int					retval = 0;
	char					str[AMBPLL_MAX_CMD_LENGTH];
	int					performance, i;
	amb_hal_success_t			result;

	i = (count < AMBPLL_MAX_CMD_LENGTH) ? count : AMBPLL_MAX_CMD_LENGTH;
	if (copy_from_user(str, buffer, i)) {
		pr_err("%s: copy_from_user fail!\n", __func__);
		retval = -EFAULT;
		goto ambarella_performance_proc_write_exit;
	}
	str[i - 1] = 0;

	for (i = 0; i < ARRAY_SIZE(performance_list); i++) {
		if (strlen(str) == strlen(performance_list[i].name)
			&& strcmp(str, performance_list[i].name) == 0) {
			performance = performance_list[i].performance;
			break;
		}
	}
	if (i >= ARRAY_SIZE(performance_list)){
		pr_err("\n%s: invalid performance (%s)!\n", __func__, str);
		retval = -EINVAL;
		goto ambarella_performance_proc_write_exit;
	}

	result = amb_get_operating_mode(HAL_BASE_VP, &pll_info.operating_mode);
	if(result != AMB_HAL_SUCCESS){
		pr_err("%s: amb_get_operating_mode failed(%d)\n",
			__func__, result);
		retval = -EPERM;
		goto ambarella_performance_proc_write_exit;
	}

	if (pll_info.operating_mode.performance != performance) {
		pll_info.operating_mode.performance = performance;
		retval = ambarella_set_operating_mode(&pll_info.operating_mode);
	}
	if (!retval)
		retval = count;

ambarella_performance_proc_write_exit:
	return retval;
}

#if (CHIP_REV == I1 || CHIP_REV == AMB_S2)
static int ambarella_vidcap_proc_write(struct file *file,
	const char __user *buffer, unsigned long count, void *data)
{
	int					retval = 0;
	char					str[AMBPLL_MAX_CMD_LENGTH];
	int					vidcap, i;
	amb_hal_success_t			result;

	i = (count < AMBPLL_MAX_CMD_LENGTH) ? count : AMBPLL_MAX_CMD_LENGTH;
	if (copy_from_user(str, buffer, i)) {
		pr_err("%s: copy_from_user fail!\n", __func__);
		retval = -EFAULT;
		goto ambarella_vidcap_proc_write_exit;
	}
	str[i - 1] = 0;

	for (i = 0; i < ARRAY_SIZE(vidcap_list); i++) {
		if (strlen(str) == strlen(vidcap_list[i].name)
			&& strcmp(str, vidcap_list[i].name) == 0) {
			vidcap = vidcap_list[i].vidcap;
			break;
		}
	}
	if (i >= ARRAY_SIZE(vidcap_list)){
		pr_err("\n%s: invalid vidcap (%s)!\n", __func__, str);
		retval = -EINVAL;
		goto ambarella_vidcap_proc_write_exit;
	}

	result = amb_get_operating_mode(HAL_BASE_VP, &pll_info.operating_mode);
	if(result != AMB_HAL_SUCCESS){
		pr_err("%s: amb_get_operating_mode failed(%d)\n",
			__func__, result);
		retval = -EPERM;
		goto ambarella_vidcap_proc_write_exit;
	}

	if (pll_info.operating_mode.vidcap_size != vidcap) {
		pll_info.operating_mode.vidcap_size = vidcap;
		retval = ambarella_set_operating_mode(&pll_info.operating_mode);
	}
	if (!retval)
		retval = count;

ambarella_vidcap_proc_write_exit:
	return retval;
}
#endif


static int __init ambarella_init_pll_hal(void)
{
	amb_hal_success_t			result;
	int					retval = 0;

	result = amb_get_operating_mode(HAL_BASE_VP, &pll_info.operating_mode);
	if(result != AMB_HAL_SUCCESS){
		pr_err("%s: get operating mode failed(%d)\n",__func__, result);
		retval = -EPERM;
		goto ambarella_init_pll_hal_exit;
	}

#if (CHIP_REV == I1 || CHIP_REV == AMB_S2)
	vidcap_file = create_proc_entry("vidcap", S_IRUGO | S_IWUSR,
		get_ambarella_proc_dir());
	if (vidcap_file == NULL) {
		retval = -ENOMEM;
		pr_err("%s: create proc file (vidcap) fail!\n", __func__);
		goto ambarella_init_pll_hal_exit;
	} else {
		vidcap_file->read_proc = ambarella_pll_proc_read;
		vidcap_file->write_proc = ambarella_vidcap_proc_write;
	}
#endif

	mode_file = create_proc_entry("mode", S_IRUGO | S_IWUSR,
		get_ambarella_proc_dir());
	if (mode_file == NULL) {
		retval = -ENOMEM;
		pr_err("%s: create proc file (mode) fail!\n", __func__);
		goto ambarella_init_pll_hal_exit;
	} else {
		mode_file->read_proc = ambarella_pll_proc_read;
		mode_file->write_proc = ambarella_mode_proc_write;
	}

	performance_file = create_proc_entry("performance", S_IRUGO | S_IWUSR,
		get_ambarella_proc_dir());
	if (performance_file == NULL) {
		retval = -ENOMEM;
		pr_err("%s: create proc file (performance) fail!\n", __func__);
		goto ambarella_init_pll_hal_exit;
	} else {
		performance_file->read_proc = ambarella_pll_proc_read;
		performance_file->write_proc = ambarella_performance_proc_write;
	}

ambarella_init_pll_hal_exit:
	return retval;
}

int __init ambarella_init_pll(void)
{
	int					retval = 0;

	retval = ambarella_init_pll_hal();

	return retval;
}

/* ==========================================================================*/
#if (CHIP_REV == A5S)
u32 get_core_bus_freq_hz(void)
{
	return (u32)amb_get_core_clock_frequency(HAL_BASE_VP);
}

u32 get_ahb_bus_freq_hz(void)
{
	return get_core_bus_freq_hz();
}

u32 get_apb_bus_freq_hz(void)
{
	return get_core_bus_freq_hz() >> 1;
}
#else
u32 get_core_bus_freq_hz(void)
{
	return (u32)amb_get_core_clock_frequency(HAL_BASE_VP);
}

u32 get_ahb_bus_freq_hz(void)
{
	return (u32)amb_get_ahb_clock_frequency(HAL_BASE_VP);
}

u32 get_apb_bus_freq_hz(void)
{
	return (u32)amb_get_apb_clock_frequency(HAL_BASE_VP);
}
#endif
EXPORT_SYMBOL(get_core_bus_freq_hz);
EXPORT_SYMBOL(get_ahb_bus_freq_hz);
EXPORT_SYMBOL(get_apb_bus_freq_hz);

