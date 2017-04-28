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

#ifndef _ATHR_VLAN_IGMP_H
#define _ATHR_VLAN_IGMP_H

#include "athrs_ioctl.h" /*mac_addr_t*/

#if defined(CONFIG_ATHRS16_PHY) || defined(CONFIG_AR7242_S16_PHY) || \
    defined(CONFIG_AR7240_S26_PHY) || defined(CONFIG_ATHRS26_PHY) || defined (CONFIG_ATHRS27_PHY)
/* S26 CSR Registers */
#define PORT_STATUS_REGISTER0                0x0100 
#define PORT_STATUS_REGISTER1                0x0200
#define PORT_STATUS_REGISTER2                0x0300
#define PORT_STATUS_REGISTER3                0x0400
#define PORT_STATUS_REGISTER4                0x0500
#define PORT_STATUS_REGISTER5                0x0600

#define PORT_BASE_VLAN_REGISTER0             0x0108 
#define PORT_BASE_VLAN_REGISTER1             0x0208
#define PORT_BASE_VLAN_REGISTER2             0x0308
#define PORT_BASE_VLAN_REGISTER3             0x0408
#define PORT_BASE_VLAN_REGISTER4             0x0508
#define PORT_BASE_VLAN_REGISTER5             0x0608

#define RATE_LIMIT_REGISTER0                 0x010C
#define RATE_LIMIT_REGISTER1                 0x020C
#define RATE_LIMIT_REGISTER2                 0x030C
#define RATE_LIMIT_REGISTER3                 0x040C
#define RATE_LIMIT_REGISTER4                 0x050C
#define RATE_LIMIT_REGISTER5                 0x060C

#define PORT_CONTROL_REGISTER0               0x0104
#define PORT_CONTROL_REGISTER1               0x0204
#define PORT_CONTROL_REGISTER2               0x0304
#define PORT_CONTROL_REGISTER3               0x0404
#define PORT_CONTROL_REGISTER4               0x0504
#define PORT_CONTROL_REGISTER5               0x0604

#define RATE_LIMIT1_REGISTER0                0x011c
#define RATE_LIMIT2_REGISTER0                0x0120

/* SWITCH QOS REGISTERS */
#define ATHR_CTRL_PORT_0             0x104
#define ATHR_CPU_PORT                0x0078
#define ATHR_IP_PRI_MAP0             0x0060
#define ATHR_IP_PRI_MAP1             0x0064
#define ATHR_IP_PRI_MAP2             0x0068

#ifdef CONFIG_ATHRS27_PHY
#define ATHR_PRI_CTRL_PORT_0             0x114 /* CPU PORT */
#define ATHR_PRI_CTRL_PORT_1             0x214
#define ATHR_PRI_CTRL_PORT_2             0x314
#define ATHR_PRI_CTRL_PORT_3             0x414
#define ATHR_PRI_CTRL_PORT_4             0x514
#define ATHR_PRI_CTRL_PORT_5             0x614
#else
#define ATHR_PRI_CTRL_PORT_0             0x110 /* CPU PORT */
#define ATHR_PRI_CTRL_PORT_1             0x210
#define ATHR_PRI_CTRL_PORT_2             0x310
#define ATHR_PRI_CTRL_PORT_3             0x410
#define ATHR_PRI_CTRL_PORT_4             0x510
#define ATHR_PRI_CTRL_PORT_5             0x610
#endif

#define ATHR_QOS_PORT_0			0x110 /* CPU PORT */
#define ATHR_QOS_PORT_1			0x210
#define ATHR_QOS_PORT_2			0x310
#define ATHR_QOS_PORT_3			0x410
#define ATHR_QOS_PORT_4			0x510

#define ATHR_TOS_PRI_EN                      (1 << 16)
#define ATHR_VLAN_PRI_EN                     (1 << 17)
#define ATHR_DA_PRI_EN                       (1 << 18)
#define ATHR_PORT_PRI_EN                     (1 << 19)
#define ATHR_HDR_EN              (1 << 11)
#define ATHR_CPU_EN              (1 << 8 )

#define ATHR_ENABLE_TOS                 (1 << 16)

#define ATHR_QOS_MODE_REGISTER          0x030
#define ATHR_QOS_FIXED_PRIORITY        ((0 << 31) | (0 << 28))
#define ATHR_QOS_WEIGHTED              ((1 << 31) | (0 << 28)) /* Fixed weight 8,4,2,1 */
#define ATHR_QOS_MIXED                 ((1 << 31) | (1 << 28)) /* Q3 for managment; Q2,Q1,Q0 - 4,2,1 */

#ifndef BOOL
#define BOOL    int
#endif

typedef enum {
    PORT_EG_UNMODIFIED = 0,  /**<  egress transmit packets unmodified */
    PORT_EG_UNTAGGED,        /**<  egress transmit packets without vlan tag */
    PORT_EG_TAGGED,          /**<  egress transmit packets with vlan tag */
} port_1q_egmode_t;

extern void set_packet_inspection_flag(int flag);

uint32_t athr_ioctl_vlan_addports(uint32_t vlan_id, uint32_t port_id);
uint32_t athr_ioctl_vlan_delports(uint32_t vlan_id, uint32_t port_id);
uint32_t athr_port_egvlanmode_set(uint32_t port_id, port_1q_egmode_t mode);
uint32_t athr_port_default_vid_set(uint32_t port_id, uint32_t vlan_id);
uint32_t athr_port_igmps_status_set(uint32_t port_id, BOOL enable);
uint32_t athr_ioctl_enable_vlan(void);
uint32_t athr_ioctl_disable_vlan(void);
uint32_t athr_fdb_add(mac_addr_t addr, uint32_t port_bmap, BOOL sa_drop);
uint32_t athr_fdb_del(mac_addr_t addr);
void athr_clear_multi(void);
void athr_set_multi(void);
uint32_t athr_ioctl_vlan_qos(uint32_t vlan_id, uint32_t vlan_pri, uint32_t *port_map);
uint32_t athr_fdb_add_qos(u_int8_t addr[], uint32_t prio,uint32_t portno);
#endif
#endif
