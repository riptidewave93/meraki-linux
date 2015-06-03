#ifndef __XLNA_H__
#define __XLNA_H__

/*
 * Definitions for XLNA enabling driver for atheros 9344
 *
 * Justin Delegard
 * Copyright (c) 2012, Meraki, Inc.
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License version 2 as
 * published by the Free Software Foundation.
 *
 */

struct xlna_platform_data {
    unsigned gpio0;
    unsigned gpio1;
    int enabled;
};

#endif /* __XLNA_H__ */
