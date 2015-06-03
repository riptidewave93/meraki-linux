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
#ifndef __ATHRS_S27_NF_NAT_H
#define __ATHRS_S27_NF_NAT_H

#undef CONFIG_ATHRS_HW_NAT_DEBUG
#include <net/netfilter/nf_conntrack_core.h>

extern int athrs_nat_hwtbl_ops(void *args, int portdir, int cmd);
extern int athrs_nat_hwtbl_lkup(void *args, int portdir);
extern void athrs_nat_op_lock_init(void);

int athr_hw_nat_add_entry(uint32_t oldip, uint16_t oldport,
                          uint32_t newip, uint16_t newport,
                          u_int8_t protocol, int maniptype);

void athr_hw_nat_del_entry(uint32_t oldip, uint16_t oldport,
                                  u_int8_t protocol, int maniptype);

int athr_hw_nat_lookup_entry(uint32_t ipaddr, uint16_t port, u_int8_t maniptype,
                             u_int8_t protocol, uint32_t *newipaddr, uint16_t *newport);

void *athrs27_register_nat_ops(void);

struct nf_ct_hwnat {
    struct nf_conntrack_tuple_hash tuplehash;
    __be32 newip;
    __be16 newport;
    unsigned long tx_cnt, rx_cnt;
    struct nf_conn *sw_ct;
    struct nf_conntrack_tuple tuple[IP_CT_DIR_MAX];
};

#define ATH_WAN_INTERFACE "eth0"
#define ATHR_HW_NAT_EGRESS_MAX_ENTRIES 512
#define ATHR_HW_NAT_INGRESS_MAX_ENTRIES 512
#endif /* __ATHRS_S27_NF_NAT_H */
