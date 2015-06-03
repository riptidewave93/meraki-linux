/*
 *
 * VCore-III I2C
 *
 * Copyright (c) 2011 Meraki, Inc.
 * Author: Kevin Paul Herbert (kph@meraki.com)
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License version 2 as
 * published by the Free Software Foundation.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA
 * 02110-1301, USA.
 *
 * Jaguar glue wrapper.
 */

#ifdef CONFIG_VTSS_VCOREIII_JAGUAR

#define SUPPORT_VTSS_VCOREIII_JAGUAR

#define VCOREIII_TWI_SCL 14
#define VCOREIII_TWI_SDA 15

#include "i2c-vcoreiii.h"

#endif /* CONFIG_VTSS_VCOREIII_JAGUAR */