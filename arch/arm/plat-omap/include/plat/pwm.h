/*
 *    Copyright (c) 2010 Grant Erickson <marathon96@gmail.com>
 *
 *    This program is free software; you can redistribute it and/or
 *    modify it under the terms of the GNU General Public License
 *    version 2 as published by the Free Software Foundation.
 *
 *    Description:
 *      This file is defines platform-specific configuration data for
 *      the OMAP generic PWM platform driver.
 */

#ifndef _OMAP2_PWM_H
#define _OMAP2_PWM_H

// walter @ 20140711 add for ioctl
/* use 'I' as magic number I=0x49 */
#define PWM_MAGIC				'I'

/* Set the PWM duty/period */
#define PWMIOC_SET_DURATION				_IOW (PWM_MAGIC, 0, unsigned int)
#define PWMIOC_SET_PERIOD				_IOW (PWM_MAGIC, 1, unsigned int)

/* Turn on/off the PWM */
#define PWMIOC_START					_IOW (PWM_MAGIC, 2, unsigned int)
#define PWMIOC_STOP						_IOW (PWM_MAGIC, 3, unsigned int)

/* Set one shot mode/continous mode */
#define PWMIOC_SET_MODE					_IOW (PWM_MAGIC, 4, unsigned int)

/* Set SCPWM mode */
#define PWMIOC_SET_FIRST_PHASE_STATE	_IOW (PWM_MAGIC, 5, unsigned int)
#define PWMIOC_SET_INACT_OUT_STATE		_IOW (PWM_MAGIC, 6, unsigned int)

/* Not support in IPNC */
#define PWMIOC_SET_RPT_VAL				_IOW (PWM_MAGIC, 7, unsigned int)

#define PWM_MAXNR 7

/**
 * struct omap2_pwm_platform_config - OMAP platform-specific data for PWMs
 * @timer_id: the OMAP dual-mode timer ID.
 * @polarity: the polarity (active-high or -low) of the PWM.
 *
 * This identifies the OMAP dual-mode timer (dmtimer) that will be bound
 * to the PWM.
 */
struct omap2_pwm_platform_config {
	int timer_id;
	bool polarity;
};
int omap_register_pwm_config(struct omap2_pwm_platform_config * config, int size);
#endif /* _OMAP2_PWM_H */
