/*
 *  Copyright (c) 2008, Atheros Communications Inc.
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
 *   
 */

#ifndef _ATHR_NF_ACL_H
#define _ATHR_NF_ACL_H

#include "athrs_hw_acl.h"

#define R_HEAD 0xaa
#define R_LINK 0xbb
#define R_END  0xee
#define TYPE_OF(x) ( x & 0xff)
#define KC_OFF(x)  (((x & 0xff00) >> 8))
#define ATHR_ETH_WAN_IF "eth0"

typedef struct
{
        int cmd;
        int operand0;
        int operand1;
        int dir;
        int proto;
        int kc_offset;
        int action;
        int qol;
        int is_last;
        int is_and;
        struct list_head list;

}athr_nf_cmd_t; 

typedef struct {

   uint8_t cmds[5];
   uint8_t pad[3];
   uint32_t operand0;
   uint32_t operand1;
   int cmd_no;             /* No of cmds per rule. Max is 4 */
   int rem_bits;           /* Total no of bits per rule */
   int dir;
   int action;
   int type;
   int nep;
   struct list_head list;

}athr_acl_rmap_t;

typedef struct {

   athr_nf_cmd_t *cmd_tb;
   int dir;
   char name[20];
   struct list_head list;
   int kc_offset; /* Kernel entry offset */
   int no_cmds;   /* no of cmds in this chain */
   int qol; /* Lenght of the chain */
}athr_nf_chain_t;

typedef struct
{
    int   (*create_cmd_chain)  (char *name,struct ipt_ip *ip,int *iptb_dir,int action,int off);
    void* (*get_cmd_chain)     (char *name,int dir,int off);
    int   (*nf_parse_cmds)     (char *name,void *mtpar,int cmdno,int dir,int action);

}athr_hw_acl_ops_t;


#define set_offset(_off,_idx)           \
({                                      \
         _off = (64 - _idx) / 8;        \
         if ((_off) / 4)                \
               _off = 7 - (_off % 4);   \
         else                           \
               _off = 3 - _off;         \
})


static inline void list_insert(struct list_head *old,
                                struct list_head *new)
{
        if (old->prev != NULL)
            old->prev->next = new;
  
        new->next = old;
        new->prev = old->prev;
        old->prev = new;
}

#define ATHRS_ACL_LIST_ITERATE_AND_FREE(pos, head, freep)       \
    freep = list_entry((head)->next, typeof(*pos), list);       \
    list_for_each_entry(pos, head, list) {                      \
        if (freep != pos) {                                     \
            kfree(freep);                                       \
        }                                                       \
        freep = pos;                                            \
    }                                                           \
    kfree(freep);

#endif
