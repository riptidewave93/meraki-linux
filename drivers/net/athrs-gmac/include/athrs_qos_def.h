#ifndef _ATHRS_QOS_DEFS_H
#define _ATHRS_QOS_DEFS_H

#include "athrs_mac.h"
extern void athr_remove_hdr(athr_gmac_t *mac, struct sk_buff *skb);

static inline void
athr_gmac_rx_qos(athr_gmac_t *mac, struct sk_buff *skb)
{
    uint8_t hdr_type = 0;
    int tos;
    struct ethhdr       *eh;
    unsigned short diffs[2];

    if (mac_has_flag(mac,ATHR_S26_HEADER))
    {
        hdr_type = skb->data[1];
    }

    if (mac_has_flag(mac,ATHR_S16_HEADER) || mac_has_flag(mac,ATHR_S27_HEADER))
    {

        hdr_type = skb->data[12];
    }

    if ((hdr_type & HDR_PACKET_TYPE_MASK) == ATHR_NORMAL_PKT) {
        /*
         * Default priority
         */
        tos = AC_TO_ENET_TOS (((hdr_type >> HDR_PRIORITY_SHIFT) & HDR_PRIORITY_MASK));
#ifndef CONFIG_ATHR_VLAN_IGMP
        athr_remove_hdr(mac,skb);
#endif

        eh = (struct ethhdr *)skb->data;

#ifdef CONFIG_ATHR_VLAN_IGMP
    if (mac_has_flag(mac,ATHR_S26_HEADER) || mac_has_flag(mac,ATHR_S16_HEADER) || mac_has_flag(mac,ATHR_S27_HEADER))
        eh = (struct ethhdr *)(skb->data +2);
#endif

        if (eh->h_proto  == __constant_htons(ETHERTYPE_IP))
        {

            struct iphdr *ip = (struct iphdr *)
                    (skb->data + sizeof (struct ethhdr));

#ifdef CONFIG_ATHR_VLAN_IGMP
    if (mac_has_flag(mac,ATHR_S26_HEADER) || mac_has_flag(mac,ATHR_S16_HEADER) || mac_has_flag(mac,ATHR_S27_HEADER))
        ip = (struct iphdr *)(skb->data + sizeof (struct ethhdr)+2);
#endif
            /*
             * IP frame: exclude ECN bits 0-1 and map DSCP bits 2-7
             * from TOS byte.
             */
            tos = (tos << TOS_ECN_SHIFT ) & TOS_ECN_MASK;
            if ( tos != ip->tos ) {
                diffs[0] = htons(ip->tos) ^ 0xFFFF;
                ip->tos = ((ip->tos &  ~(TOS_ECN_MASK)) | tos);
                diffs[1] = htons(ip->tos);
                ip->check = csum_fold(csum_partial((char *)diffs,
                             sizeof(diffs),ip->check ^ 0xFFFF));
            }
            skb->priority = tos;
        }
    }
    else {
#ifndef CONFIG_ATHR_VLAN_IGMP
        athr_remove_hdr(mac,skb);
#endif        
    }
}

static inline void
__athr_gmac_tx_qos(athr_gmac_t *mac,struct sk_buff *skb)
{
    struct ethhdr       *eh;
    int                 ac = 0;

    /*
     * Select the TX based on access category
     */

    ac = ENET_AC_BE;
    /*
     * Default priority
     */
    eh = (struct ethhdr *) skb->data;

    if (eh->h_proto  == __constant_htons(ETHERTYPE_IP))
    {
        const struct iphdr *ip = (struct iphdr *)
                    (skb->data + sizeof (struct ethhdr));
        /*
         * IP frame: exclude ECN bits 0-1 and map DSCP bits 2-7
         * from TOS byte.
         */
        ac = TOS_TO_ENET_AC ((ip->tos >> TOS_ECN_SHIFT) & 0x3F);
    }
    skb->priority = ac;

    /*hdr_dump("Tx",mac->mac_unit,skb->data,ac,0);*/
}


static inline void
athr_gmac_tx_qos(athr_gmac_t *mac,struct sk_buff *skb)
{
    if ((mac_has_flag(mac,WAN_QOS_SOFT_CLASS))   
        || (mac_has_flag(mac,ATHR_SWITCH_QOS))) {
        __athr_gmac_tx_qos(mac,skb);               
    }                                            

}

static inline int
athr_gmac_check_qos_bt(athr_gmac_t *mac,struct sk_buff *skb)
{
    if (mac_has_flag(mac,WAN_QOS_SOFT_CLASS)) {         
        if (mac->tx_ac_bt & (1 << skb->priority)) {      
            mac->mac_dev->trans_start = jiffies;          
            return 1;
        }                                                
    }
    return 0;
}

 
#endif  //_ATHRS_QOS_DEFS_H
