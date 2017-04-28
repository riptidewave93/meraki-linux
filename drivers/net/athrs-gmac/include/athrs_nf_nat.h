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
#ifndef __ATHRS_NF_NAT_H
#define __ATHRS_NF_NAT_H


static uint8_t athrs_wan_if_mac_addr[ETH_ALEN] = { 0, 0, 0, 0, 0, 0 };
uint32_t athrs_wan_if_ip_addr;

extern int athrs_nat_hwtbl_ops(void *args, int portdir, int cmd);
extern int athrs_nat_hwtbl_lkup(void *args, int portdir, int cmd);
extern int athrs_nat_hwtbl_lkup_frag(void *args, int portdir, int cmd);
//extern int arp_req_get_nodev(struct arpreq *areq);
extern int arp_req_get(struct arpreq *areq, struct net_device *dev);
extern void athrs_nat_op_lock_init(void);

static int athr_nat_netdev_event_handler(struct notifier_block *this,
                                           unsigned long event, void *ptr);

static int athr_nat_inetaddr_event_handler(struct notifier_block *this,
                                           unsigned long event, void *ptr);

int athrs_nat_add_del_ops(uint32_t key[2], uint32_t info[4], int portdir, int cmd,
				uint8_t pkt_type);
static struct notifier_block athr_nat_inetaddr_notifier = {
            .notifier_call = athr_nat_inetaddr_event_handler,
};

static struct notifier_block athr_nat_netdev_notifier = {
            .notifier_call = athr_nat_netdev_event_handler,
};

#define ATH_WAN_INTERFACE "eth0"
#define ATHR_HW_NAT_EGRESS_MAX_ENTRIES 512
#define ATHR_HW_NAT_INGRESS_MAX_ENTRIES 512

#endif /* __ATHRS_NF_NAT_H */
