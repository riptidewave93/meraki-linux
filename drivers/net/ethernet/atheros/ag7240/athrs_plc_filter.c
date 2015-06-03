/*
 * athrs_plc_filter.c
 *
 * Copyright (c) 2012, Qualcomm Atheros, Inc.
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

#include <linux/kernel.h>
#include <linux/etherdevice.h>
#include "athrs_mac.h"
#include "athrs_plc_filter.h"

#define PLC_ETHER_TYPE		(0x88E1)
#define PLC_MGMT_ADDR		{0x00, 0xB0, 0x52, 0x00, 0x00, 0x01} /* Not used, but kept here for reference */

int
athr_plc_dmac_change(void *mac, struct sk_buff *skb)
{
    struct ethhdr *eh;

    eh = (struct ethhdr *) skb->data;

    /* The PLC filter checks if the packet's DA is L2 broadcast address,
     * and if EtherType is PLC message type: 0x88E1.
     * If both conditions are true, the filter changes the DA to the
     * local interface's DA, thus avoiding flooding this packet
     * to all other interfaces.
     */
    if( unlikely( is_broadcast_ether_addr( eh->h_dest ) ) ) {
        unsigned short ethertype;

    	/* Check VLAN tag */
        if(unlikely(skb->data[0x0C] == 0x81 && skb->data[0x0D] == 0x00)) {
            ethertype = *(unsigned short *)(skb->data + 0x10);
        } else {
            ethertype = eh->h_proto;
        }

        if(unlikely(ethertype == __constant_htons( PLC_ETHER_TYPE ))){
            struct net_device *dev;

            dev = ((athr_gmac_t *)mac)->mac_dev;
            memcpy(eh->h_dest, dev->dev_addr, ETH_ALEN);
        }
    }

    return 0;
}

