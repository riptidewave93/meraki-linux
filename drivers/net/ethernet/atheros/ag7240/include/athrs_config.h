/*
 * Copyright (c) 2008, Atheros Communications Inc.
 *
 * Permission to use, copy, modify, and/or distribute this software for any
 * purpose with or without fee is hereby granted, provided that the above
 * copyright notice and this permission notice appear in all copies.
 *
 * THE SOFTWARE IS PROVIDED "AS IS" AND THE AUTHOR DISCLAIMS ALL WARRANTIES
 * WITH REGARD TO THIS SOFTWARE INCLUDING ALL IMPLIED WARRANTIES OF
 * MERCHANTABILITY AND FITNESS. IN NO EVENT SHALL THE AUTHOR BE LIABLE FOR
 * ANY SPECIAL, DIRECT, INDIRECT, OR CONSEQUENTIAL DAMAGES OR ANY DAMAGES
 * WHATSOEVER RESULTING FROM LOSS OF USE, DATA OR PROFITS, WHETHER IN AN
 * ACTION OF CONTRACT, NEGLIGENCE OR OTHER TORTIOUS ACTION, ARISING OUT OF
 * OR IN CONNECTION WITH THE USE OR PERFORMANCE OF THIS SOFTWARE.
 */

#ifndef _ATHR_CONFIG_H
#define _ATHR_CONFIG_H

#include <linux/version.h>
#if (LINUX_VERSION_CODE >= KERNEL_VERSION(2,6,31))
#if LINUX_VERSION_CODE < KERNEL_VERSION(2, 6, 33)
#include <linux/autoconf.h>
#else
#include <generated/autoconf.h>
#endif
#else
#include <linux/config.h>
#endif

/*
 * This defines the interconnects between old Kconfig definitions and the new
 * definitions based on the new BSP.
 *
 * Eventually this file is meant to disappear.
 */

#ifdef CONFIG_AG7240_QOS
#define CONFIG_ATHRS_QOS 1
#endif
#ifdef CONFIG_ATHRS16_PHY_CONNECT_GE0
#define CONFIG_ATHRS16_PHY 1
#endif

#if defined(CONFIG_AR7240_S26_VLAN_IGMP) || defined (CONFIG_AR7240_S27_VLAN_IGMP) || defined (CONFIG_AG7240_QOS)
#define CONFIG_ATHR_VLAN_IGMP 1
#endif

#ifndef CONFIG_AG7240_NUMBER_TX_PKTS_1
#define CONFIG_AG7240_NUMBER_TX_PKTS_1    CONFIG_AG7240_NUMBER_TX_PKTS
#endif

#ifndef CONFIG_AG7240_NUMBER_RX_PKTS_1
#define CONFIG_AG7240_NUMBER_RX_PKTS_1    CONFIG_AG7240_NUMBER_RX_PKTS
#endif

#endif //_ATHR_CONFIG_H
