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

#include <linux/version.h>
#if (LINUX_VERSION_CODE >= KERNEL_VERSION(2,6,31))
#include <linux/autoconf.h>
#else
#include <linux/config.h>
#endif
#include <linux/types.h>
#include <linux/netdevice.h>
#include <linux/etherdevice.h>
#include <linux/delay.h>
#include <linux/if_pppox.h>
#include <linux/igmp.h>
#include <linux/ip.h>

#include "athrs_mac.h"
#include "athrs_phy.h"
#include "athrs_ioctl.h"

#include "athrs_vlan_igmp_s17.h"
#if HYBRID_SWITCH_PORT6_USED
#define ATHR_MAX_PORT_ID 7 /* lan ports and cpu port, exclude cpu port */
#define ATHR_MAX_PORTS_ID 127 /* The maxsize is 0x111_1111 */
#else
#define ATHR_MAX_PORT_ID 6 /* lan ports and cpu port, exclude cpu port */
#define ATHR_MAX_PORTS_ID 63 /* The maxsize is 0x11_1111 */
#endif

#define ATHR_OK   0
#define ATHR_ERR  1
#define ATHR_FULL 2

#define ATHR_ATU_FUNC_REG       0x60C

#define AT_BUSY                 (1 << 31)
#define AT_FULL_VIO             (1 << 12)
#define AT_FUNC(x)              ((x) & 0x0f)

#define ARL_FLUSH_ALL           0x0001
#define ARL_LOAD_ENTRY          0x0010
#define ARL_FLUSH_ALL_UNLOCK    0x0100

#if HYBRID_LINK_CHANGE_EVENT
extern athr_hybrid_lce_t athr_hybrid_lce[HYBRID_MAX_VLAN];
#endif
uint32_t
athr_port_igmps_status_set(uint32_t port_id, BOOL enable)
{
    static int flag = 0;
    int ret = 0;

    if(port_id > ATHR_MAX_PORT_ID){
        printk("%s, port_id %d is wrong !\n", __func__, port_id);
        return -1;
    }
#if 0
    printk("***port_id %d, enable %d\n", port_id, enable);
    if((1 == enable) && ((flag&(0x1<<port_id)) == 0)){
        isis_port_igmps_status_set(S17DEVID, port_id, A_TRUE);
        isis_port_igmp_mld_join_set(S17DEVID, port_id, A_TRUE);
        isis_port_igmp_mld_leave_set(S17DEVID, port_id, A_TRUE);
        if( 0 == flag ){
            isis_igmp_mld_entry_creat_set(S17DEVID, A_TRUE);
            isis_igmp_mld_entry_v3_set(S17DEVID, A_TRUE);
            isis_igmp_mld_rp_set(S17DEVID, 0x3f);
            isis_igmp_mld_cmd_set(S17DEVID,FAL_MAC_CPY_TO_CPU);
        }
        flag |= 0x1<<port_id;
    }else if((0 == enable) && ((flag&(0x1<<port_id)) != 0)){
        isis_port_igmps_status_set(S17DEVID, port_id, A_FALSE);
        isis_port_igmp_mld_join_set(S17DEVID, port_id, A_FALSE);
        isis_port_igmp_mld_leave_set(S17DEVID, port_id, A_FALSE);
        flag &= ~(0x1<<port_id);
        if( 0 == flag ){
            isis_igmp_mld_entry_creat_set(S17DEVID, A_FALSE);
            isis_igmp_mld_entry_v3_set(S17DEVID, A_FALSE);
            isis_igmp_mld_rp_set(S17DEVID, 0x0);
            isis_igmp_mld_cmd_set(S17DEVID,FAL_MAC_FRWRD);
        }
    }else
        ;
#endif
    return ret;
}

uint32_t
athr_ioctl_vlan_addports(uint32_t vlan_id, uint32_t port_id)
{
    fal_vlan_t vlan;
    uint32_t port;
    fal_pt_1q_egmode_t info;
    int ret = 0;
    int i;
#if HYBRID_LINK_CHANGE_EVENT
    int index=-1;
#endif
    if(port_id > ATHR_MAX_PORTS_ID){
        printk("%s, port_id %d is wrong !\n", __func__, port_id);
        return -1;
    }

    ret = isis_vlan_find(S17DEVID, vlan_id, &vlan);
    if(ret != 0){
        S17DEBUG("%s, vlanId %x does not exist\n", __func__, vlan_id);
        ret = isis_vlan_create(S17DEVID, vlan_id);
        if(ret != 0){
            printk("%s, Create s17 vlanID %d Error ! \n", __func__, vlan_id);
        }else{
            S17DEBUG("%s, Create s17 vlanID %d OK ! \n", __func__, vlan_id);
        }
    }else{
        S17DEBUG("%s, vlanId %x exists, the member is %x\n", __func__, vlan_id, vlan.mem_ports);
    }

#if HYBRID_LINK_CHANGE_EVENT
    for(i=0; i<HYBRID_MAX_VLAN; i++){
        if(athr_hybrid_lce[i].vlanid == vlan_id){
            index = i;
            break;
        }
    }
#endif

    /* translate the port bitmap to port number */
    for(i=0; i<ATHR_MAX_PORT_ID; i++){
        if(((port_id >> i)&0x1) != 0){
#if HYBRID_LINK_CHANGE_EVENT
            if((index != -1) && (i > 0)){
                if(!(athr_hybrid_lce[index].vlanmap & ((0x1 << (i-1)))))
                    athr_hybrid_lce[index].count++;
                athr_hybrid_lce[index].vlanmap |= (0x1 << (i-1));
                if(i == HYBRID_PLC_PORT){
                    athr_hybrid_lce[index].vlanmap &= ~(0x1 << (i-1));
                    athr_hybrid_lce[index].p6used = 1;
                }
            }
#endif
            ret = isis_vlan_member_add(S17DEVID, vlan_id, i, i ? FAL_EG_UNTAGGED:FAL_EG_TAGGED);
            if(ret < 0)
               return ret;
        }
    }
    
    return ret;
}

uint32_t
athr_ioctl_vlan_delports(uint32_t vlan_id, uint32_t port_id)
{
    fal_vlan_t vlan;
    uint32_t port;
    int ret = 0;
    int i;
#if HYBRID_LINK_CHANGE_EVENT
    int index=-1;
#endif

    if(port_id > ATHR_MAX_PORTS_ID){
        printk("%s, port_id %d is wrong !\n", __func__, port_id);
        return -1;
    }

    ret = isis_vlan_find(S17DEVID, vlan_id, &vlan);
    if(ret != 0){
        S17DEBUG("%s, vlanId %x does not exist\n", __func__, vlan_id);
        return -1;
    }
#if HYBRID_LINK_CHANGE_EVENT
    for(i=0; i<HYBRID_MAX_VLAN; i++){
        if(athr_hybrid_lce[i].vlanid == vlan_id){
            index = i;
            break;
        }
    }
#endif
    /* translate the port bitmap to port number */
    for(i=0; i<ATHR_MAX_PORT_ID; i++){
        if(((port_id >> i)&0x1) != 0){
#if HYBRID_LINK_CHANGE_EVENT
            if(index != -1 && i > 0){
                if((athr_hybrid_lce[index].vlanmap & ((0x1 << (i-1)))))
                    athr_hybrid_lce[index].count--;
                athr_hybrid_lce[index].vlanmap &= ~(0x1 << (i-1));
            }
#endif
            ret = isis_vlan_member_del(S17DEVID, vlan_id, i);
            if(ret < 0)
               return ret;
        }
    }
    
    return ret;
}

uint32_t
athr_port_egvlanmode_set(uint32_t port_id, port_1q_egmode_t mode)
{
    int ret = 0;
    fal_pt_1q_egmode_t port_egvlanmode;

    if(port_id > ATHR_MAX_PORTS_ID){
        printk("%s, port_id %d is wrong !\n", __func__, port_id);
        return -1;
    }

    switch(mode){
        case PORT_EG_UNTAGGED:
            port_egvlanmode = FAL_EG_UNTAGGED;
            break;
        case PORT_EG_TAGGED:
            port_egvlanmode = FAL_EG_TAGGED;
            break;
        default:
            port_egvlanmode = FAL_EG_UNMODIFIED;
            break;
    }

    ret = isis_port_egvlanmode_set(S17DEVID, port_id, port_egvlanmode);

    return ret;
}

uint32_t
athr_port_default_vid_set(uint32_t port_id, uint32_t vlan_id)
{
    int ret = 0;

    if(port_id > ATHR_MAX_PORTS_ID){
        printk("%s, port_id %d is wrong !\n", __func__, port_id);
        return -1;
    }

    ret = isis_port_default_cvid_set(S17DEVID, port_id, vlan_id);

    return ret;
}

uint32_t
athr_ioctl_enable_vlan(void)
{
    int port = 0;
    int ret = 0;
    for(port = 0; port < ATHR_MAX_PORT_ID; port ++)
        ret = isis_port_1qmode_set(S17DEVID, port, FAL_1Q_SECURE);

    return ret;
}

uint32_t
athr_ioctl_disable_vlan(void)
{
    int port = 0;
    int ret = 0;
    for(port = 0; port < ATHR_MAX_PORT_ID; port ++)
        ret = isis_port_1qmode_set(S17DEVID, port, FAL_1Q_DISABLE);
    return ret;
}

static uint32_t
_athr_fdb_commit(athr_phy_ops_t *phy, uint32_t op)
{
    uint32_t busy = 1, i = 0x1000;
    uint32_t reg0, full_vio;

    reg0 = phy->read_mac_reg(ATHR_ATU_FUNC_REG);
    reg0 |= AT_FUNC(op) | AT_BUSY;
    phy->write_mac_reg(ATHR_ATU_FUNC_REG, reg0);

    while (busy && --i) {
        reg0 = phy->read_mac_reg(ATHR_ATU_FUNC_REG);
        busy = reg0 & AT_BUSY;
        udelay(5);
    }

    if (0 == i) {
        printk("Flush ARL table failed!\n");
        return ATHR_ERR;
    }

    full_vio = reg0 & AT_FULL_VIO; 
    if (full_vio) {
        /* must clear AT_FULL_VOI bit */
        reg0 = AT_FULL_VIO;
        phy->write_mac_reg(ATHR_ATU_FUNC_REG, reg0);

        if (ARL_LOAD_ENTRY == op) {
            return ATHR_FULL;
        } else {
            return ATHR_ERR;
        }
    }

    return ATHR_OK;
}

uint32_t
athr_fdb_add(mac_addr_t addr, uint32_t port_bmap, BOOL sa_drop)
{
    int ret = 0;
    return ret;
}

uint32_t
athr_fdb_del(mac_addr_t addr)
{
    int ret = 0;
    return ret;
}

uint32_t
athr_fdb_flush(athr_phy_ops_t *phy, BOOL f_static)
{
    if (f_static) {
        return _athr_fdb_commit(phy, ARL_FLUSH_ALL);
    } else {
        return _athr_fdb_commit(phy, ARL_FLUSH_ALL_UNLOCK);
    }
}

void
athr_clear_multi(void)
{
    int ret = 0;
    return ret;
}

void
athr_set_multi(void)
{
    int ret = 0;
    return ret;
}

uint32_t
athr_ioctl_vlan_qos(uint32_t vlan_id, uint32_t vlan_pri, uint32_t *port_map)
{
    int ret = 0;
    return ret;
}

uint32_t
athr_fdb_add_qos(u_int8_t addr[], uint32_t prio,uint32_t portno)
{
    int ret = 0;
    return ret;
}

uint32_t
athr_vlan_query(uint32_t vlan_id, uint32_t * port_bmap)
{
    int ret = 0;
    return ret;
}

void
athr_set_vlan_phy_ops(void *arg)
{
    return;
}

int set_mac_qops(void *mac){
    return 0;
}

int athr_config_qos(struct eth_cfg_params *ethcfg, int cmd)
{
    return 0;
}

extern int ignore_packet_inspection;
void 
athr_filter_igmp_pkg(athr_gmac_t *mac, struct sk_buff *skb)
{
   /* 
     * 01:00:5e:X:X:X is a multicast MAC address.
     */
    mac_addr_t addr;

    if (mac_has_flag(mac,ATHR_S26_HEADER) && (mac->mac_unit == 1))
    {
        int offset = 14;

#if defined(CONFIG_IPV6)
        if(ignore_packet_inspection)
        {
            if(unlikely((skb->data[2]==0x33)&&(skb->data[3]==0x33))){ /* multicast mac address */
            /* 
             * If vlan, calculate the offset of the vlan header. 
             */
            if((skb->data[14]==0x81)&&(skb->data[15]==0x00)){
                offset +=4;             
            }
            /* 
             * If pppoe exists, calculate the offset of the pppoe header. 
             */
            if((skb->data[offset]==0x88)&&((skb->data[offset+1]==0x63)|(skb->data[offset+1]==0x64))){
                offset += sizeof(struct pppoe_hdr);             
            }
            /* 
             * Parse IP layer.  
             */
            if((skb->data[offset]==0x86)&&(skb->data[offset+1]==0xdd)){
                offset +=2;
                offset += sizeof(struct ipv6hdr);
                
                if((skb->data[4]==0xff)&&(skb->data[offset]==0x87)){ /* Neighbor solicitation */
                    addr.uc[0] = 0x33;
                    addr.uc[1] = 0x33;
                    addr.uc[2] = 0xff;
                    addr.uc[3] = skb->data[5];
                    addr.uc[4] = skb->data[6];
                    addr.uc[5] = skb->data[7];
                    athr_fdb_add(addr, 0x1f, 0);
                    printk("athr_mac::S26_ARL_ADD,mac:[%x.%x.%x.%x.%x.%x] port[%x] drop %d\n",
                        addr.uc[0],addr.uc[1],addr.uc[2],addr.uc[3],addr.uc[4],addr.uc[5],0x1f,0);
                }else if(skb->data[offset]==0x3a){ 
                    /* 0x3a for ICMPv6 */
                    offset += 8;
                    if((skb->data[offset]==0x82)|| (skb->data[offset]==0x8f)||(skb->data[offset]==0x83)||(skb->data[offset]==0x84)){
                        /* 0x82 Multicast Listener Query Message(V2), 0x8f Multicast Listener Report Message(v2) */
                        /* 0x83 Multicast Listener Report Message(V1), 0x84 Multicast Listener Listen done Message */
                       struct icmp6hdr *igh;
                        igh =(struct icmp6hdr *)(skb->data + offset); 
                        igh->icmp6_cksum = 0x7d50|(skb->data[0] & 0xf);
                    }
                }
            }
        }
     }
#endif
        if(ignore_packet_inspection){
            if(unlikely((skb->data[2]==0x01)&&(skb->data[3]==0x00)&&(skb->data[4]==0x5e))){
    
                /* 
                 * If vlan, calculate the offset of the vlan header. 
                 */
                if((skb->data[14]==0x81)&&(skb->data[15]==0x00)){
                    offset +=4;             
                }

                /* 
                 * If pppoe exists, calculate the offset of the pppoe header. 
                 */
                if((skb->data[offset]==0x88)&&((skb->data[offset+1]==0x63)||(skb->data[offset+1]==0x64))){
                    offset += sizeof(struct pppoe_hdr);             
                }

                /* 
                 * Parse IP layer.  
                 */
                if((skb->data[offset]==0x08)&&(skb->data[offset+1]==0x00)){
                    struct iphdr * iph;
                    struct igmphdr *igh;

                    offset +=2;
                    iph =(struct iphdr *)(skb->data+offset);
                    offset += (iph->ihl<<2);
                    igh =(struct igmphdr *)(skb->data + offset); 

                    /* 
                     * Sanity Check and insert port information into igmp checksum. 
                     */
                    if((iph->ihl>4)&&(iph->version==4)&&(iph->protocol==2)){
                        igh->csum = 0x7d50|(skb->data[0] & 0xf);
                        printk("[eth1] Type %x port %x.\n",igh->type, skb->data[0] & 0xf);
                    }

                }
            }
        }
     }
}
void 
athr_add_igmp_pkg(athr_gmac_t *mac, struct sk_buff *skb)
{
#if defined(CONFIG_IPV6)
    mac_addr_t addr;
    if(ignore_packet_inspection)
    {
        if ((mac->mac_unit == 1))
        {
            if(unlikely((skb->data[0]==0x33)&&(skb->data[1]==0x33)&&(skb->data[2]==0xff))){ /* neighbor solicit mac address */
                    addr.uc[0] = skb->data[0];
                    addr.uc[1] = skb->data[1];
                    addr.uc[2] = skb->data[2];
                    addr.uc[3] = skb->data[3];
                    addr.uc[4] = skb->data[4];
                    addr.uc[5] = skb->data[5];
                    athr_fdb_add(addr, 0x1f, 0);
                    printk("athr_add_igmp_pkg::S26_ARL_ADD,mac:[%x.%x.%x.%x.%x.%x] port[%x] drop %d\n",
                        addr.uc[0],addr.uc[1],addr.uc[2],addr.uc[3],addr.uc[4],addr.uc[5],0x1f,0);
            }
        }
    }
#endif
}

int athr_vlan_ctrls(struct net_device *dev, struct ifreq *ifr, int cmd)
{
    struct eth_cfg_params *ethcfg = (struct eth_cfg_params *) ifr->ifr_data;
    int ret = 0;
    int tmp = 0;
    athr_gmac_t *mac = (athr_gmac_t *) ATHR_MAC_PRIV(dev);
    athr_phy_ops_t *phy = mac->phy;
    athr_gmac_t *mac0 = (athr_gmac_t *)phy->arg_ad[0];
    struct arl_struct *arl = (struct arl_struct *) (&ethcfg->vlanid);
    unsigned int vlan_value = ethcfg->val;
    unsigned short vlan_id = vlan_value >> 16;
    unsigned short mode = vlan_value >> 16;
    unsigned short vlan_port = vlan_value & 0xff;
    unsigned int flag = 0;
    uint32_t ar7240_revid;
    cmd = ethcfg->cmd;

    switch (cmd) {
        case ATHR_PACKET_FLAG:
            set_packet_inspection_flag(vlan_value);
            break;

        case ATHR_VLAN_ADDPORTS:
            if (vlan_id > 4095)
                return -EINVAL;

            printk("athr_gmac::ADD_PORT vid = %d ports=%x.\n", vlan_id,
                   vlan_port);
#if HYBRID_LINK_CHANGE_EVENT
            for(tmp=0; tmp<HYBRID_MAX_VLAN; tmp++){
                if(athr_hybrid_lce[tmp].vlanid == vlan_id)
                    break;
            }

            if(tmp == HYBRID_MAX_VLAN){
                for(tmp=0; tmp<HYBRID_MAX_VLAN; tmp++){
                    if(athr_hybrid_lce[tmp].vlanid == 0){
                        athr_hybrid_lce[tmp].vlanid = vlan_id;
                        athr_hybrid_lce[tmp].vlanmap = 0;
                        athr_hybrid_lce[tmp].count = 0;
                        sprintf(athr_hybrid_lce[tmp].name, "%s.%d\0", dev->name, vlan_id);
                        break;
                    }  
                }
            }
            if(tmp == HYBRID_MAX_VLAN){
                printk("Warning: vlans are more then 5\n");
            }
#endif

            ret = athr_ioctl_vlan_addports(vlan_id, vlan_port);
            /*for(tmp=0; tmp<HYBRID_MAX_VLAN; tmp++){
                if(athr_hybrid_lce[tmp].vlanid != 0)
                    printk("**** name %s, vlanid %d, map %x, count %d, p6used %d\n", athr_hybrid_lce[tmp].name,
                            athr_hybrid_lce[tmp].vlanid, athr_hybrid_lce[tmp].vlanmap, athr_hybrid_lce[tmp].count,
                            athr_hybrid_lce[tmp].p6used);
            }*/

            break;

        case ATHR_VLAN_DELPORTS:
            if (vlan_id > 4095)
                return -EINVAL;

            printk("athr_gmac::vlan_DEL_PORT vid = %d ports=%x.\n", vlan_id,
                   vlan_port);

            ret = athr_ioctl_vlan_delports(vlan_id, vlan_port);
            break;

        case ATHR_VLAN_SETTAGMODE:
            ret = athr_port_egvlanmode_set(vlan_port, mode);
            break;

        case ATHR_VLAN_SETDEFAULTID:
            if (vlan_id > 4095)
                return -EINVAL;

            printk("athr_gmac::VLAN_SETDEFAULTID vid = %d portno=%d.\n", vlan_id,
                   vlan_port);

            ret = athr_port_default_vid_set(vlan_port, vlan_id);
            break;

        case ATHR_VLAN_ENABLE:
            athr_ioctl_enable_vlan();
            athr_fdb_flush(phy, 1);
            break;

        case ATHR_VLAN_DISABLE:
            athr_ioctl_disable_vlan();
            athr_fdb_flush(phy, 1);
            break;

        case ATHR_IGMP_ON_OFF:
            tmp = vlan_value & (0x1 << 7);
            vlan_port &= ~(0x1 << 7);

            if (vlan_port > 4)
                return -EINVAL;

            if (tmp != 0) {
                printk("athr_gmac::Enable IGMP snooping in port no %x.\n",
                     vlan_port);
                ret = athr_port_igmps_status_set(vlan_port, 1);
            } else {
                printk("athr_gmac::Disable IGMP snooping in port no %x.\n",
                     vlan_port);
                ret = athr_port_igmps_status_set(vlan_port, 0);
            }

            break;
        case ATHR_LINK_GETSTAT: {
            int fdx, phy_up;
            athr_phy_speed_t speed;

            athr_gmac_get_link_st(mac0, &phy_up, &fdx, &speed, vlan_port);
            ifr->ifr_ifru.ifru_ivalue = (speed << 16 | fdx << 8 | phy_up);
            }
            break;
        case ATHR_ARL_ADD:
            ret = athr_fdb_add(arl->mac_addr, arl->port_map, arl->sa_drop);

            printk("athr_gmac::ARL_ADD,mac:[%x.%x.%x.%x.%x.%x] port[%x] drop %d\n",
                 arl->mac_addr.uc[0], arl->mac_addr.uc[1], arl->mac_addr.uc[2],
                 arl->mac_addr.uc[3], arl->mac_addr.uc[4], arl->mac_addr.uc[5],
                 arl->port_map, arl->sa_drop);
            break;
        case ATHR_ARL_DEL:
            ret = athr_fdb_del(arl->mac_addr);

            printk("athr_gmac::ARL_DEL mac:[%x.%x.%x.%x.%x.%x].\n",
                   arl->mac_addr.uc[0], arl->mac_addr.uc[1], arl->mac_addr.uc[2],
                   arl->mac_addr.uc[3], arl->mac_addr.uc[4], arl->mac_addr.uc[5]);
            break;
        case ATHR_MCAST_CLR:
            if (!vlan_value)
                athr_clear_multi();
            else
                athr_set_multi();

            printk("athr_gmac::_MCAST_CLR --- %s.\n",
                   vlan_value ? "enable Multicast" : "disable Multicast");
            break;
        default:
            break;
    }

    return ret;
}
