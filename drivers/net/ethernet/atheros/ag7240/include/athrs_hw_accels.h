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

#ifndef _ATHRS_HW_ACCELS_H
#define _ATHRS_HW_ACCELS_H

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

#include <linux/types.h>
#include <linux/netdevice.h>

#if defined(CONFIG_ATHRS_HW_NAT) || defined(CONFIG_ATHRS_HW_ACL)
#define CONFIG_ATHRS_HW_ACCELS 1
#endif

#ifdef CONFIG_ATHRS_HW_NAT
#include "athrs_nf_nat.h"

typedef void
    (*athr_hw_nat_init) (void);

typedef void
    (*athr_hw_nat_down) (void);

typedef int
    (*athr_hw_nat_add) (athr_tuple tup1);

typedef void
    (*athr_hw_nat_del) (athr_tuple tup1);

typedef int
    (*athr_hw_nat_lkp) (athr_tuple tup1, uint32_t *new_ipaddr, uint16_t *new_port);

typedef void
    (*athr_hw_nat_set_wan_mac) (unsigned char *wan_mac);

typedef void
    (*athr_hw_nat_set_wan_ip) (uint32_t wan_ip, int wan_index);

typedef void
    (*athr_hw_nat_reg_notif) (void);

typedef void
    (*athr_hw_nat_unreg_notif) (void);

typedef int
    (*athr_hw_nat_proc_init) (struct net *);

typedef void
    (*athr_hw_nat_proc_remove) (void);

typedef void
    (*athr_hw_nat_cleanup) (void);

typedef struct {
    /* HWNAT Access */
    athr_hw_nat_init            init;
    athr_hw_nat_down            down;
    athr_hw_nat_add             add_entry;
    athr_hw_nat_del             del_entry;
    athr_hw_nat_lkp             lkp_entry;
    athr_hw_nat_set_wan_mac     set_wan_mac_addr;
    athr_hw_nat_set_wan_ip      set_wan_ip_addr;
    athr_hw_nat_reg_notif       reg_net_notifier;
    athr_hw_nat_unreg_notif     unreg_net_notifier;
    athr_hw_nat_proc_init       proc_init;
    athr_hw_nat_proc_remove     proc_remove;
    athr_hw_nat_cleanup         cleanup;
    int                         init_done;
} athr_nat_ops_t;

#endif /* #ifdef CONFIG_ATHRS_HW_NAT */


#ifdef CONFIG_ATHRS_HW_ACL
typedef void
    (*athr_hw_acl_init) (void);

typedef int
    (*athr_hw_acl_create_chain) (char *name, void *ip,
                                 int *iptb_dir, int action, int off);

typedef int
    (*athr_hw_acl_parse_chain) (char *name, void *mt_par, int cmdno,
                                int dir, int action);

typedef void
    (*athr_hw_acl_add_cmds) (int *);

typedef int
    (*athr_hw_acl_flush_entries) (int);

typedef void
    (*athr_hw_acl_cleanup) (void);

typedef struct {
    /* HWACL Access */
    athr_hw_acl_init                   init;
    athr_hw_acl_create_chain           create_chain;
    athr_hw_acl_parse_chain            parse_chain;
    athr_hw_acl_add_cmds               add_cmds;
    athr_hw_acl_flush_entries          flush_entries;
    athr_hw_acl_cleanup                cleanup;
    int                                init_done;
} athr_acl_ops_t;

#endif /* #ifdef CONFIG_ATHRS_HW_ACL */

#ifdef CONFIG_ATHRS_HW_ACCELS
extern int athr_setup_hwaccels(void *);
extern int athr_reg_hwaccels(void *);
extern int athr_setup_hwaccels_notifiers(void *);
extern int athr_clean_hwaccels(void *);

typedef struct {
#ifdef CONFIG_ATHRS_HW_NAT
    athr_nat_ops_t           *nat;
#endif
#ifdef CONFIG_ATHRS_HW_ACL
    athr_acl_ops_t           *acl;
#endif
} athr_hw_accels_t;

#define athr_register_hwaccels(mac)             \
    if (mac->mac_unit == 0) {                   \
        athr_reg_hwaccels(mac);                 \
    }

#define athr_init_hwaccels(mac)                 \
    if (mac->mac_unit == 0) {                   \
        athr_setup_hwaccels(mac);               \
    }

#define athr_register_hwaccels_notifiers(mac)   \
    athr_setup_hwaccels_notifiers(mac);

#define athr_cleanup_hwaccels(mac)              \
    if (mac->mac_unit == 0) {                   \
        athr_clean_hwaccels(mac);               \
    }

#else

#define athr_register_hwaccels(mac)                /* Do nothing */
#define athr_init_hwaccels(mac)                    /* Do nothing */
#define athr_register_hwaccels_notifiers(mac)      /* Do nothing */
#define athr_cleanup_hwaccels(mac)                 /* Do nothing */

#endif /* #ifdef CONFIG_ATHRS_HW_ACCELS */

#endif
