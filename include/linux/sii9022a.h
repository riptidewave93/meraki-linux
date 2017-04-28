 /* sii9022a.h : sii9022a platform data
 *
 * Copyright (C) 2009 Texas Instruments Inc
 * Author: Deepthy Ravi <deepthy.ravi@ti.com>
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
 * Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA.
 */

#ifndef _SII9022A_H_
#define _SII9022A_H_

struct sii9022a_platform_data {
	u32 hdmi_hot_plug_gpio_intr_line; /* HDMI hot plug GPIO int line */
	u32 sync_mode; /* Sync Mode */
	u32 clk_edge;
};

#endif
