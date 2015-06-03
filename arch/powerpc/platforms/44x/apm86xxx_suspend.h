/*
 * APM86xxx suspend support
 *
 * Copyright (c) 2010 Applied Micro Circuits Corporation.
 * Victor Gallardo (vgallardo@apm.com)
 *
 * See file CREDITS for list of people who contributed to this
 * project.
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License as
 * published by the Free Software Foundation; either version 2 of
 * the License, or (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place, Suite 330, Boston,
 * MA 02111-1307 USA
 */

#ifndef __POWERPC_PLATFORMS_44X_APM86xxx_SUSPEND_H
#define __POWERPC_PLATFORMS_44X_APM86xxx_SUSPEND_H

#include <asm/ppc44x_tlb.h>

#define PWRMODE_DOZE	0
#define PWRMODE_NAP	1
#define PWRMODE_SLEEP	2
#define PWRMODE_DSLEEP	3
#define PWRMODE_MAX	4

#define PWRMODE_PWR_ON  IPP_PWRMGMT_CMD_CPU_SET_NORMAL
#define PWRMODE_CLK_OFF IPP_PWRMGMT_CMD_CPU_CLKGATE
#define PWRMODE_PWR_OFF IPP_PWRMGMT_CMD_CPU_POWEROFF

#define WAKEUP_LAN	0
#define WAKEUP_GPIO	1
#define WAKEUP_USB	2
#define WAKEUP_RTC	3
#define WAKEUP_MAX	4

#ifndef __ASSEMBLY__

#include <asm/ipp.h>

#define MP_PROCESSOR_ID() (unsigned int)mfspr(SPRN_PIR)

#define CONFIG_PM_DEBUG_TRACE 0
#if CONFIG_PM_DEBUG_TRACE
#       define PM_TRACE(fmt, arg...) printk(KERN_INFO fmt, ## arg)
#else
#       define PM_TRACE(fmt, arg...) ((void)0)
#endif

#if defined(CONFIG_PM_DEBUG)
#       define PM_DEBUG(fmt, arg...) printk(KERN_INFO fmt, ## arg)
#else
#       define PM_DEBUG(fmt, arg...) ((void)0)
#endif
 
struct apm86xxx_pm * apm86xxx_suspend_sysfs_init(void);

void apm86xxx_standby(void);

extern void apm86xxx_suspend_deepsleep(void);
extern void apm86xxx_resume_deepsleep(void);

struct apm86xxx_pm_save
{
	unsigned int gpr[32];
	unsigned int sprg[8];
	unsigned int dec;
	unsigned int tbwl;
	unsigned int tbwu;
	unsigned int tcr;
	unsigned int usprg0;
	unsigned int cr;
	unsigned int lr;
	unsigned int msr;
	unsigned int mmucr;
	unsigned int pid;
	unsigned int ccr[2];
	unsigned int ivpr;
	unsigned int ivor[16];
	struct ppc44x_tlbentry tlb[64];
};

struct apm86xxx_pm_modes
{
        int cpu[2];
        const char *name;
};

struct apm86xxx_pm_wakeup
{
        unsigned int enabled;
        unsigned int ipp_mask;
        const char  *name;
};

struct apm86xxx_pm
{
        int    mode; /* current sleep mode */

        unsigned int sleep_timer; /* timer to transition to sleep state */
        unsigned int deepsleep_timer; /* timer to transition to deepsleep state */
        unsigned int rtc_wakeup_time; /* Encoded RTC wakeup time */
        unsigned int gpio_wakeup; /* GPIO IRQ to wakeup on */
        unsigned int vscale; /* Voltage Scale on/off */
        unsigned int iscale; /* IPP Scale on/off */
        unsigned int wakeup_ledmsk; /* LED to do wakeup measurement */
	unsigned int disable_gpio_sd;  /* Dont shutdown GPIO in deep sleep */
	int sleep_timer_changed;
	int ds_timer_changed;
	int wakeup_led_changed;
	int disable_gpio_sd_changed;

        struct apm86xxx_pm_modes  *modes;
        struct apm86xxx_pm_wakeup *wakeup;

	unsigned long tcr_save;
};

static inline const char *apm86xxx_pm_mode_type(int mode)
{
	switch (mode) {
	case PWRMODE_PWR_ON:
		return "PWR_ON";
	case PWRMODE_CLK_OFF:
		return "CLK_OFF";
	case PWRMODE_PWR_OFF:
		return "PWR_OFF";
	default:
		return "UNKNOWN";
	}
}

#endif /* __ASSEMBLY__ */

#endif	/* __POWERPC_PLATFORMS_44X_APM86xxx_SUSPEND_H */
