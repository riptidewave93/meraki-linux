/*
 *  Meraki Z1 board register definitions
 *
 *  Copyright (C) 2013 Dean Thomas <dean@meraki.com>
 *
 */

#ifndef __ASM_MACH_MERAKI_Z1_H
#define __ASM_MACH_MERAKI_Z1_H

#include <linux/types.h>
#include <linux/init.h>
#include <linux/io.h>
#include <linux/bitops.h>

/* GPIO Support (LED's, button, nu801 tri-color, xlna) */
#define MERAKI_Z1_GPIO_BTN_RESET	12

#define MERAKI_Z1_GPIO_NU801_CKI        14
#define MERAKI_Z1_GPIO_NU801_SDI        15

#define MERAKI_Z1_GPIO_LED_POWER	17

#define MERAKI_Z1_GPIO_XLNA1		18
#define MERAKI_Z1_GPIO_XLNA2		19

#define MERAKI_Z1_NUM_GPIOS		23
#define MERAKI_Z1_GPIO_OUT_FUNCTION0	0x1804002c

/* GPIO Button Definitions */
#define MERAKI_Z1_KEYS_POLL_INTERVAL    	20      /* msecs */
#define MERAKI_Z1_KEYS_DEBOUNCE_INTERVAL	(3 * MERAKI_Z1_KEYS_POLL_INTERVAL)

/* WMAC Constants */

#define MERAKI_Z1_WMAC_CALDATA_OFFSET	0x1000

/* PCI Constants */

#define MERAKI_Z1_PCIE_CALDATA_OFFSET	0x15000

#endif /* __ASM_MACH_MERAKI_Z1_H */
