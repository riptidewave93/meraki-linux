#
# Makefile for the Atheros AR71XX/AR724X/AR913X specific parts of the kernel
#
# Copyright (C) 2008-2011 Gabor Juhos <juhosg@openwrt.org>
# Copyright (C) 2008 Imre Kaloz <kaloz@openwrt.org>
#
# This program is free software; you can redistribute it and/or modify it
# under the terms of the GNU General Public License version 2 as published
# by the Free Software Foundation.

obj-y	:= prom.o setup.o irq.o common.o clock.o gpio.o

obj-$(CONFIG_EARLY_PRINTK)		+= early_printk.o
obj-$(CONFIG_PCI)			+= pci.o

#
# Devices
#
obj-y					+= dev-common.o
obj-$(CONFIG_ATH79_DEV_GPIO_BUTTONS)	+= dev-gpio-buttons.o
obj-$(CONFIG_ATH79_DEV_LEDS_GPIO)	+= dev-leds-gpio.o
obj-$(CONFIG_ATH79_DEV_SPI)		+= dev-spi.o
obj-$(CONFIG_ATH79_DEV_USB)		+= dev-usb.o
obj-$(CONFIG_ATH79_DEV_WMAC)		+= dev-wmac.o
obj-$(CONFIG_ATH79_DEV_XLNA)		+= dev-xlna.o

#
# Machines
#
obj-$(CONFIG_ATH79_MACH_AP121)		+= mach-ap121.o
obj-$(CONFIG_ATH79_MACH_AP136)		+= mach-ap136.o
obj-$(CONFIG_ATH79_MACH_AP81)		+= mach-ap81.o
obj-$(CONFIG_ATH79_MACH_DB120)		+= mach-db120.o
obj-$(CONFIG_ATH79_MACH_PB44)		+= mach-pb44.o
obj-$(CONFIG_ATH79_MACH_UBNT_XM)	+= mach-ubnt-xm.o
obj-$(CONFIG_ATH79_MACH_MERAKI)		+= mach-meraki.o
obj-$(CONFIG_ATH79_MACH_MERAKI_MR11_MR14) += mach-meraki-mr11-mr14.o
obj-$(CONFIG_ATH79_MACH_MERAKI_MR12)	+= mach-meraki-mr12.o
obj-$(CONFIG_ATH79_MACH_MERAKI_MR16)	+= mach-meraki-mr16.o
obj-$(CONFIG_ATH79_MACH_MERAKI_MR18)   += mach-meraki-mr18.o
obj-$(CONFIG_ATH79_MACH_MERAKI_Z1)	+= mach-meraki-z1.o
obj-$(CONFIG_MR18_RADIOS)	+= mach-meraki-mr18-radios.o
