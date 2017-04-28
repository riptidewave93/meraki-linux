/*
 * arch/arm/plat-ambarella/generic/uport.c
 *
 * Author: Cao Rongrong <rrcao@ambarella.com>
 *
 * Copyright (C) 2010-2016, Ambarella, Inc.
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
#include <linux/io.h>
#include <mach/hardware.h>
#include <mach/board.h>
#include <plat/uport.h>

/* AMB_S2 share the usb port between UHC and UDC
 * I1 share the usb port1 between UHC and UDC */
#if (CHIP_REV == I1) || (CHIP_REV == AMB_S2)
static u32 ambarella_usb_port_owner = 0;
#endif

void ambarella_enable_usb_port(int owner)
{
#if defined(CONFIG_PLAT_AMBARELLA_SUPPORT_HAL)
#if (CHIP_REV == A5S)
	amb_usb_interface_state_t state;

	state = amb_get_usb_interface_state(HAL_BASE_VP);
	if (state != AMB_USB_ALWAYS_ON && amb_set_usb_interface_state(HAL_BASE_VP, AMB_USB_ALWAYS_ON)
			!= AMB_HAL_SUCCESS) {
		pr_info("amb_set_usb_interface_state() failed");
	}

#elif (CHIP_REV == A7)
	amb_usb_interface_state_t state;

	state = amb_get_usb_interface_state(HAL_BASE_VP);
	if (state != AMB_USB_ON && amb_set_usb_interface_state(HAL_BASE_VP, AMB_USB_ON)
			!= AMB_HAL_SUCCESS) {
		pr_info("amb_set_usb_interface_state() failed");
	}

#elif (CHIP_REV == I1)
	amb_usb_port_state_t state;

	ambarella_usb_port_owner |= owner;

	/* We must enable usb port1 first. Note: no matter usb port1 is
	 * configured as Host or Slave, we always enable it. */
	state = amb_get_usb_port1_state(HAL_BASE_VP);
	if (state != AMB_USB_ON && amb_set_usb_port1_state(HAL_BASE_VP, AMB_USB_ON)
			!= AMB_HAL_SUCCESS) {
		pr_info("%s: amb_set_usb_port1_state fail!\n", __func__);
	}

	/* Then we enable usb port0. */
	state = amb_get_usb_port0_state(HAL_BASE_VP);
	if (state != AMB_USB_ON && amb_set_usb_port0_state(HAL_BASE_VP, AMB_USB_ON)
			!= AMB_HAL_SUCCESS) {
		pr_info("%s: amb_set_usb_port0_state fail!\n", __func__);
	}


#elif (CHIP_REV == AMB_S2)
	amb_usb_port_state_t state;

	ambarella_usb_port_owner |= owner;

	/* Enable usb port (usbphy), and it may have been enabled before. */
	state = amb_get_usb_port_state(HAL_BASE_VP);
	if (state != AMB_USB_ALWAYS_ON && amb_set_usb_port_state(HAL_BASE_VP, AMB_USB_ALWAYS_ON)
			!= AMB_HAL_SUCCESS) {
		pr_info("%s: amb_set_usb_port_state fail!\n", __func__);
	}

#elif (CHIP_REV == A7L)
	amb_usb_port_state_t state;

	/* Enable usb port (usbphy), and it may have been enabled before. */
	state = amb_get_usb_port1_state(HAL_BASE_VP);
	if (state != AMB_USB_ALWAYS_ON && amb_set_usb_port1_state(HAL_BASE_VP, AMB_USB_ALWAYS_ON)
			!= AMB_HAL_SUCCESS) {
		pr_info("%s: amb_set_usb_port_state fail!\n", __func__);
	}
#endif

#else

#if (CHIP_REV == A5S) || (CHIP_REV == A7L)
	amba_setbitsl(ANA_PWR_REG, 0x4);	/* always on */
#elif (CHIP_REV == A7)
	amba_setbitsl(ANA_PWR_REG, 0x2);	/* on */
#elif (CHIP_REV == I1)
	ambarella_usb_port_owner |= owner;
	/* We must enable usb port1 first. Note: no matter usb port1 is
	 * configured as Host or Slave, we always enable it. */
	amba_setbitsl(ANA_PWR_REG, 0x2);	/* on */
	amba_setbitsl(ANA_PWR_REG, 0x1000);	/* on */
#elif (CHIP_REV == AMB_S2)
	ambarella_usb_port_owner |= owner;
	amba_setbitsl(ANA_PWR_REG, 0x4);	/* always on */
#endif

#endif
}

void ambarella_disable_usb_port(int owner)
{
#if defined(CONFIG_PLAT_AMBARELLA_SUPPORT_HAL)
#if (CHIP_REV == A5S) || (CHIP_REV == A7)
	if (amb_set_usb_interface_state(HAL_BASE_VP, AMB_USB_OFF)
			!= AMB_HAL_SUCCESS) {
		pr_info("%s: amb_set_usb_interface_state fail!\n", __func__);
	}

#elif (CHIP_REV == I1)
	if (amb_set_usb_port0_state(HAL_BASE_VP, AMB_USB_OFF)
			!= AMB_HAL_SUCCESS) {
		pr_info("%s: amb_set_usb_port0_state fail!\n", __func__);
	}

	/* We disable usb port1 when neither UHC nor UDC use it */
	ambarella_usb_port_owner &= (~owner);
	if (ambarella_usb_port_owner == 0) {
		if (amb_set_usb_port1_state(HAL_BASE_VP, AMB_USB_OFF)
				!= AMB_HAL_SUCCESS) {
			pr_info("%s: amb_set_usb_port1_state fail!\n", __func__);
		}
	}

#elif (CHIP_REV == AMB_S2)
	/* We disable usb port when neither UHC nor UDC use it */
	ambarella_usb_port_owner &= (~owner);
	if (ambarella_usb_port_owner == 0)
		amb_set_usb_port_state(HAL_BASE_VP, AMB_USB_OFF);

#elif (CHIP_REV == A7L)
	if (amb_set_usb_port1_state(HAL_BASE_VP, AMB_USB_OFF)
			!= AMB_HAL_SUCCESS) {
		pr_info("%s: amb_set_usb_port1_state fail!\n", __func__);
	}
#endif

#else

#if (CHIP_REV == A5S) || (CHIP_REV == A7) || (CHIP_REV == A7L)
	amba_clrbitsl(ANA_PWR_REG, 0x6);
#elif (CHIP_REV == I1)
	amba_clrbitsl(ANA_PWR_REG, 0x3000);
	/* We disable usb port when neither UHC nor UDC use it */
	ambarella_usb_port_owner &= (~owner);
	if (ambarella_usb_port_owner == 0)
		amba_clrbitsl(ANA_PWR_REG, 0x6);
#elif (CHIP_REV == AMB_S2)
	/* We disable usb port when neither UHC nor UDC use it */
	ambarella_usb_port_owner &= (~owner);
	if (ambarella_usb_port_owner == 0)
		amba_clrbitsl(ANA_PWR_REG, 0x6);
#endif

#endif
}

