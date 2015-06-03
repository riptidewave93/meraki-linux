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

#include "athrs_hw_accels.h"

#ifdef CONFIG_ATHRS_GMAC_HW_ACL
#include "athrs_gmac_nf_acl.h"
#endif

extern athr_acl_ops_t *hw_acl_api;
extern int athr_nf_match_invalid;

static int athr_add_match(struct ipt_entry_match *m ,struct xt_mtchk_param *par,
                          struct ipt_entry_target *t, int i, char *name)
{
    struct ipt_ip *iptb_ip = (void *)par->entryinfo;

    par->match     = m->u.kernel.match;
    par->matchinfo = m->data;

    if (strcmp(t->u.kernel.target->name,
                            IPT_STANDARD_TARGET) == 0)
    {
        struct ipt_standard_target *st =  (void *)t; /* get verdict */

        if(par->match) {
            int dir = -1;
            int action = 0;

            if (par->hook_mask & 0x2)
                dir = 0x2; /* IN */

            if (par->hook_mask & 0x4)
                dir = 0x1; /* OUT */

            if (par->hook_mask & 0x8)
                dir = 0x1; /* FORWARD */

            action = (-st->verdict -1);

            hw_acl_api ? hw_acl_api->create_chain(name, iptb_ip, &dir, action, i) :0;
            hw_acl_api ? hw_acl_api->parse_chain(name, par, i, dir, action) : 0;

        }
        return 0;
    }
    return -1;
}

static int athr_add_entry(struct ipt_entry *e, char *name,
                          int *i, int *cmds_added)
{
    struct ipt_entry_target *t;
    int ret;
    struct xt_mtchk_param mtpar;

    mtpar.table     = name;
    mtpar.entryinfo = &e->ip;
    mtpar.hook_mask = e->comefrom;
    mtpar.family    = NFPROTO_IPV4;

    t = ipt_get_target(e);
    (*i)++;

    athr_nf_match_invalid = 0;

    ret = IPT_MATCH_ITERATE(e, athr_add_match, &mtpar, t, *i, name);
    hw_acl_api ? hw_acl_api->add_cmds(cmds_added) : 0;

    return ret;
}

void athr_nf_parse_tables(void *entry0, unsigned int size, char *name)
{
    unsigned int i;
    int cmds_added = 0;

    i = 0;
    IPT_ENTRY_ITERATE(entry0, size, athr_add_entry, name, &i, &cmds_added);

    if (!cmds_added)    /* iptables -F */
        hw_acl_api ? hw_acl_api->flush_entries(1) : 0;
}


