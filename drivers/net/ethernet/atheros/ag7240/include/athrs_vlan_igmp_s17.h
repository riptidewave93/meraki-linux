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

#ifndef _ATHR_VLAN_IGMP_S17_H
#define _ATHR_VLAN_IGMP_S17_H

#define S17DEVID 0

#if 1
#define S17DEBUG printk
#else
#define S17DEBUG
#endif

typedef enum {
    A_FALSE,
    A_TRUE
}a_bool_t;

typedef enum {
    FAL_EG_UNMODIFIED = 0,  /**<  egress transmit packets unmodified */
    FAL_EG_UNTAGGED,        /**<   egress transmit packets without vlan tag*/
    FAL_EG_TAGGED,          /**<  egress transmit packets with vlan tag     */
    FAL_EG_HYBRID,          /**<  egress transmit packets in hybrid tag mode     */
    FAL_EG_UNTOUCHED,
    FAL_EG_MODE_BUTT
} fal_pt_1q_egmode_t;

typedef enum {
    FAL_1Q_DISABLE = 0, /**<  802.1q mode disbale, port based vlan */
    FAL_1Q_SECURE,      /**<   secure mode, packets which vid isn't in vlan table or source port isn't in vlan port member will be discarded.*/
    FAL_1Q_CHECK,       /**<   check mode, packets which vid isn't in vlan table will be discarded, packets which source port isn't in vlan port member will forward base on vlan port member*/
    FAL_1Q_FALLBACK,    /**<   fallback mode, packets which vid isn't in vlan table will forwarded base on port vlan, packet's which source port isn't in vlan port member will forward base on vlan port member.*/
    FAL_1Q_MODE_BUTT
} fal_pt_1qmode_t;


typedef struct {
    uint16_t vid;     /**<  vlan entry id */
    uint16_t fid;     /**<   filter data base id*/
    uint32_t mem_ports;       /**<  member port bit map */
    uint32_t tagged_ports;    /**<   bit map of tagged infomation for member port*/
    uint32_t untagged_ports;  /**<   bit map of untagged infomation for member port*/
    uint32_t unmodify_ports;/**<   bit map of unmodified infomation for member port*/
    uint32_t u_ports;
    uint16_t   learn_dis;   /**< disable address learning*/
    uint16_t   vid_pri_en;  /**<   enable 802.1p*/
    uint8_t  vid_pri;     /**<   vlaue of 802.1p when enable vid_pri_en*/
} fal_vlan_t;

typedef enum {
    FAL_MAC_FRWRD = 0,      /**<   packets are normally forwarded */
    FAL_MAC_DROP,           /**<   packets are dropped */
    FAL_MAC_CPY_TO_CPU,     /**<   packets are copyed to cpu */
    FAL_MAC_RDT_TO_CPU      /**<   packets are redirected to cpu */
} fal_fwd_cmd_t;


/* interfaces for vlan */
extern int isis_vlan_create(uint32_t dev_id, uint32_t vlan_id);
extern int isis_vlan_find(uint32_t dev_id, uint32_t vlan_id, fal_vlan_t * p_vlan);
extern int isis_vlan_member_add(uint32_t dev_id, uint32_t vlan_id, uint32_t port_id, fal_pt_1q_egmode_t port_info);
extern int isis_vlan_member_del(uint32_t dev_id, uint32_t vlan_id, uint32_t port_id);
extern int isis_port_egvlanmode_set(uint32_t dev_id, uint32_t port_id, fal_pt_1q_egmode_t port_egvlanmode);
extern int isis_port_default_cvid_set(uint32_t dev_id, uint32_t port_id, uint32_t vid);
extern int isis_port_1qmode_set(uint32_t dev_id, uint32_t port_id, fal_pt_1qmode_t port_1qmode);
/* interfaces for igmp snooping */
extern int isis_port_igmps_status_set(uint32_t dev_id, uint32_t port_id, a_bool_t enable);
extern int isis_igmp_mld_cmd_set(uint32_t dev_id, fal_fwd_cmd_t cmd);
extern int isis_port_igmp_mld_join_set(uint32_t dev_id, uint32_t port_id, a_bool_t enable);
extern int isis_port_igmp_mld_leave_set(uint32_t dev_id, uint32_t port_id, a_bool_t enable);
extern int isis_igmp_mld_rp_set(uint32_t dev_id, uint32_t pts);
extern int isis_igmp_mld_entry_creat_set(uint32_t dev_id, a_bool_t enable);
extern int isis_igmp_mld_entry_v3_set(uint32_t dev_id, a_bool_t enable);
    
extern int athr_gmac_get_link_st(athr_gmac_t *mac, int *link, int *fdx, athr_phy_speed_t *speed,int phyUnit);

typedef enum {
    PORT_EG_UNMODIFIED = 0,  /**<  egress transmit packets unmodified */
    PORT_EG_UNTAGGED,        /**<  egress transmit packets without vlan tag */
    PORT_EG_TAGGED,          /**<  egress transmit packets with vlan tag */
} port_1q_egmode_t;

#endif //_ATHR_VLAN_IGMP_S17_H
