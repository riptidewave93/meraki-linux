
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

#ifndef _ATHR_HW_ACL_H
#define _ATHR_HW_ACL_H

#include <linux/types.h>
#include <linux/version.h>
#include <linux/spinlock_types.h>
#include <linux/workqueue.h>
#include <asm/system.h>
#include <linux/netdevice.h>
#include <net/inet_ecn.h>
#include <linux/if_ether.h>
#include <linux/list.h>
#include <linux/netfilter.h>
#include <linux/netfilter_ipv4/ip_tables.h>
#include <linux/netfilter/x_tables.h>
#include <linux/netfilter/xt_mac.h>
#include <linux/netfilter/xt_dscp.h>
#include <linux/netfilter/xt_state.h>
#include "athrs_mac.h"

#define NF_IN_MASK                      0x2
#define NF_OUT_MASK                     0x4  
#define NF_FORWARD_MASK                  0x8 

#define ATHR_ACL_EGRESS_DIR         	0x1
#define ATHR_ACL_INGRESS_DIR   		0x2
/* Verdicts */
#define ATHR_ACL_GLOBAL_RULE_DROP  	NF_DROP
#define ATHR_ACL_GLOBAL_RULE_ACCEPT  	NF_ACCEPT


/********************************************************************
 * EGRESS BITS 
********************************************************************/
#define ATHR_EG_ACL_ENABLE		~(0x1)
//#define ATHR_EG_ACL_DISABLE		(0x1 | (1 << 31))
#define ATHR_EG_ACL_DISABLE		(0x1)
#define ATHR_EG_ACL_CMD0(x)             ((x & 0x1f) << 16)
#define ATHR_EG_ACL_NEP(x)              ((x & 0x3f) << 8)      /* Next Entry Pointer */
#define ATHR_EG_ACTION_ALLOW            (1 << 3) 
#define ATHR_EG_ACTION_REJECT           (1 << 2) 
#define ATHR_EG_ACTION_LINKED           (1 << 1) 
#define ATHR_EG_ACTION_HEAD             (1 << 0)
#define ATHR_EG_ACL_CMD4(x)             ((x  & 0x1f) << 24) 
#define ATHR_EG_ACL_CMD3(x)             ((x  & 0x1f) << 16) 
#define ATHR_EG_ACL_CMD2(x)             ((x  & 0x1f) << 8)
#define ATHR_EG_ACL_CMD1(x)             ((x  & 0x1f) << 0)
#define ATHR_EG_ACL_INIT		(1 << 14)
#define ATHR_EG_ACL_GLOBAL_RULE_VALID 	(1 << 13)
#define ATHR_EG_ACL_GLOBAL_RULE_DROP 	(1 << 12)
#define ATHR_EG_ACL_GLOBAL_RULE_ACCEPT 	~(1 << 13)
#define ATHR_EG_ACL_RULE_MAP            (1 << 11)
#define ATHR_EG_ACL_LAST_ENTRY          (1 << 10)
#define ATHR_EG_ACL_ACK_REG             (1 << 9)  /* Write or read to ACL is done */
#define ATHR_EG_ACL_TBL_WR              (1 << 8)
#define ATHR_EG_ACL_ENTRY_ADDR(x)       ((x & 0x3f) << 0)
#define ATHR_EG_SET_RULE(x)             (1 << x)

/********************************************************************
 * INGRESS 
********************************************************************/
#define ATHR_IG_ACL_ENABLE		~(0x1)
#define ATHR_IG_ACL_DISABLE		(0x1)
#define ATHR_IG_ACL_CMD0(x)             ((x & 0x1f) << 16)
#define ATHR_IG_ACL_NEP(x)              ((x & 0x7f) << 8)     /* Next Entry Pointer */
#define ATHR_IG_ACTION_ALLOW            (1 << 3) 
#define ATHR_IG_ACTION_REJECT           (1 << 2) 
#define ATHR_IG_ACTION_LINKED           (1 << 1) 
#define ATHR_IG_ACTION_HEAD             (1 << 0)
#define ATHR_IG_ACL_CMD4(x)             ((x  & 0x1f) << 24)
#define ATHR_IG_ACL_CMD3(x)             ((x  & 0x1f) << 16)
#define ATHR_IG_ACL_CMD2(x)             ((x  & 0x1f) << 8)
#define ATHR_IG_ACL_CMD1(x)             ((x  & 0x1f) << 0)
#define ATHR_IG_ACL_INIT		(1 << 14)
#define ATHR_IG_ACL_GLOBAL_RULE_VALID 	(1 << 13)
#define ATHR_IG_ACL_GLOBAL_RULE_DROP 	(1 << 12)
#define ATHR_IG_ACL_GLOBAL_RULE_ACCEPT 	~(1 << 13)
#define ATHR_IG_ACL_RULE_MAP            (1 << 11)
#define ATHR_IG_ACL_LAST_ENTRY          (1 << 10)
#define ATHR_IG_ACL_ACK_REG             (1 << 9)  /* Write or read to ACL is done */
#define ATHR_IG_ACL_TBL_WR              (1 << 8)
#define ATHR_IG_ACL_ENTRY_ADDR(x)       ((x & 0x1f) << 0)
#define ATHR_IG_SET_RULE(x)             (1 << x)
#define ATHR_IG_STATUS_DEBUG		(1 << 31)

/********************************************************
              ACL COMMANDS
* *******************************************************/

#define CMD_NOP 			0
#define CMD_DST_MAC_ADDR		1
#define CMD_SRC_MAC_ADDR		2
#define CMD_L2_ETHER_TYPE		3
#define CMD_L2_VLAN_TAG			4
#define CMD_L3_IP_DST_ADDR		5
#define CMD_L3_IP_SRC_ADDR              6
#define CMD_L3_IP_DST_PROT              7
#define CMD_L3_IP_SRC_PROT		8
#define CMD_L3_IP_TOS                   9   
#define CMD_TUI_L3_PROT			10
#define CMD_PATERN_MATCH		11
#define CMD_L4_DST_PORT			12
#define CMD_L4_SRC_PORT			13
#define CMD_L4_DST_PORT2		14
#define CMD_L4_SRC_PORT2		15
#define CMD_L4_TCP_FLAGS		16
#define CMD_L3_ICMP_TYPE		17
#define CMD_L3_ICMP_CODE		18
#define CMD_L4_CONN_STATE		19
#define CMD_OR				30
#define CMD_AND				31

#define RULE_TB_SIZE                    64
#define ACL_CMD_MAX                     23

#define QOL_8BIT  				8
#define QOL_16BIT  				16
#define QOL_32BIT  				32
#define QOL_48BIT  				48

#define ATHR_GMAC_FREE_TIMER 		0xb90001b8
#define ATHR_GMAC_TX_PAUSE              (1 << 30) 

typedef struct
{
   uint8_t cmd_type;
   uint8_t op_len;   
   uint8_t no_ops;
   uint32_t op1_mask;
   uint32_t op2_mask;
   
}athr_acl_cmd_t;

static const athr_acl_cmd_t cmdopts[] = {
  {CMD_NOP, 0, 0, 0 },
  {CMD_DST_MAC_ADDR, QOL_48BIT,  2, 0xffffffff, 0xffff },
  {CMD_SRC_MAC_ADDR, QOL_48BIT, 2, 0xffffffff, 0xffff }, 
  {CMD_L2_ETHER_TYPE, QOL_16BIT, 1, 0xffff, 0 },
  {CMD_L2_VLAN_TAG, QOL_16BIT, 1, 0x3ff, 0 },
  {CMD_L3_IP_DST_ADDR, QOL_48BIT, 2, 0xffffffff, 0x3f},
  {CMD_L3_IP_SRC_ADDR, QOL_48BIT, 2, 0xffffffff, 0x3f },
  {CMD_L3_IP_DST_PROT, QOL_48BIT, 2, 0xffffffff, 0x1ff },
  {CMD_L3_IP_SRC_PROT, QOL_48BIT, 2, 0xffffffff, 0x1ff },
  {CMD_L3_IP_TOS, QOL_16BIT, 2, 0xff, 0xff },
  {CMD_TUI_L3_PROT, QOL_8BIT, 1, 0x7, 0 },
  {CMD_PATERN_MATCH, QOL_8BIT, 1, 0xff, 0 },
  {CMD_L4_DST_PORT, QOL_32BIT, 2, 0xffff, 0xfffff },
  {CMD_L4_SRC_PORT, QOL_32BIT, 2, 0xffff, 0xffff },
  {CMD_L4_DST_PORT2, QOL_32BIT, 2,  0xffff, 0xffff },
  {CMD_L4_SRC_PORT2, QOL_32BIT, 2, 0xffff, 0xffff },
  {CMD_L4_TCP_FLAGS, QOL_16BIT, 2, 0xff, 0xff },
  {CMD_L3_ICMP_TYPE, QOL_8BIT, 1,	0xff, 0},
  {CMD_L3_ICMP_CODE, QOL_16BIT, 2,	0xff, 0xff },
  {CMD_L4_CONN_STATE, QOL_8BIT, 1,	0x1f, 0 },
  {CMD_OR, QOL_8BIT, 1, 0xff, 0 },
  {CMD_AND, QOL_8BIT,1, 0xff, 0 },

};

typedef enum 
{   RULE_IS_LINK = 1,
    RULE_IS_REJECT = 2,
    RULE_IS_ALLOW = 3,
} athr_acl_rule_t;

typedef struct 
{
  uint8_t  nep:6;
  uint8_t  cmd0:5;
  uint8_t  cmd1:5;
  uint8_t  cmd2:5;
  uint8_t  cmd3:5;
  uint8_t  cmd4:5;
  uint8_t  is_last;
  uint8_t  entry_addr;
  uint8_t  g_rule;
  uint8_t  dir;
  uint32_t operand0;
  uint32_t operand1;
  uint8_t  action;
  uint8_t  is_link;
  uint8_t  is_head;

}ath_acl_rule_t;


#define athr_acl_reg_wr athr_reg_wr
#define athr_acl_reg_rd athr_reg_rd
#define athr_acl_rmw_set athr_reg_rmw_set
#define athr_acl_rmw_clear athr_reg_rmw_clear

#define TX_ACL_ENABLE 4
#define TX_ACL_STATUS 2
#define RX_ACL_STATUS 3

#define ATHR_TX_HW_ACL_ENABLE(ds) (ds->res1 |= (1 << TX_ACL_ENABLE))
#define ATHR_TX_HW_ACL_STATUS(ds) (ds->res1 &  (1 << TX_ACL_STATUS))
#define ATHR_RX_HW_ACL_STATUS(ds) (ds->res1 &  (1 << RX_ACL_STATUS))

static void inline
eg_reg_dump(void)
{
   printk("ATHR_EG_ACL_STATUS           :%X\n",athr_acl_reg_rd(ATHR_EG_ACL_STATUS));
   printk("ATHR_EG_ACL_CMD0_AND_ACTION  :%X\n",athr_acl_reg_rd(ATHR_EG_ACL_CMD0_AND_ACTION));
   printk("ATHR_EG_ACL_CMD1234          :%X\n",athr_acl_reg_rd(ATHR_EG_ACL_CMD1234));
   printk("ATHR_EG_ACL_OPERAND0         :%X\n",athr_acl_reg_rd(ATHR_EG_ACL_OPERAND0));
   printk("ATHR_EG_ACL_OPERAND1         :%X\n",athr_acl_reg_rd(ATHR_EG_ACL_OPERAND1));
   printk("ATHR_EG_ACL_MEM_CTRL         :%X\n",athr_acl_reg_rd(ATHR_EG_ACL_MEM_CTRL));
   printk("ATHR_EG_ACL_RULE_TABLE_FIELDS:%X\n",athr_acl_reg_rd(ATHR_EG_ACL_RULE_TABLE_FIELDS));
   printk("ATHR_EG_ACL_RULE_TABLE0_LOWER:%X\n",athr_acl_reg_rd(ATHR_EG_ACL_RULE_TABLE0_LOWER));
   printk("ATHR_EG_ACL_RULE_TABLE0_UPPER:%X\n",athr_acl_reg_rd(ATHR_EG_ACL_RULE_TABLE0_UPPER));
   printk("ATHR_EG_ACL_RULE_TABLE1_LOWER:%X\n",athr_acl_reg_rd(ATHR_EG_ACL_RULE_TABLE1_LOWER));
   printk("ATHR_EG_ACL_RULE_TABLE1_UPPER:%X\n",athr_acl_reg_rd(ATHR_EG_ACL_RULE_TABLE1_UPPER));
   printk("ATHR_EG_ACL_RULE_TABLE2_LOWER:%X\n",athr_acl_reg_rd(ATHR_EG_ACL_RULE_TABLE2_LOWER));
   printk("ATHR_EG_ACL_RULE_TABLE2_UPPER:%X\n",athr_acl_reg_rd(ATHR_EG_ACL_RULE_TABLE2_UPPER));
   printk("ATHR_EG_ACL_RULE_TABLE3_LOWER:%X\n",athr_acl_reg_rd(ATHR_EG_ACL_RULE_TABLE3_LOWER));
   printk("ATHR_EG_ACL_RULE_TABLE3_UPPER:%X\n",athr_acl_reg_rd(ATHR_EG_ACL_RULE_TABLE3_UPPER));


}

static void inline
ig_reg_dump(void)
{

   printk("ATHR_IG_ACL_STATUS           :%X\n",athr_acl_reg_rd(ATHR_IG_ACL_STATUS));
   printk("ATHR_IG_ACL_CMD0_AND_ACTION  :%X\n",athr_acl_reg_rd(ATHR_IG_ACL_CMD0_AND_ACTION));
   printk("ATHR_IG_ACL_CMD1234          :%X\n",athr_acl_reg_rd(ATHR_IG_ACL_CMD1234));
   printk("ATHR_IG_ACL_OPERAND0         :%X\n",athr_acl_reg_rd(ATHR_IG_ACL_OPERAND0));
   printk("ATHR_IG_ACL_OPERAND1         :%X\n",athr_acl_reg_rd(ATHR_IG_ACL_OPERAND1));
   printk("ATHR_IG_ACL_MEM_CTRL         :%X\n",athr_acl_reg_rd(ATHR_IG_ACL_MEM_CTRL));
   printk("ATHR_IG_ACL_RULE_TABLE_FIELDS:%X\n",athr_acl_reg_rd(ATHR_IG_ACL_RULE_TABLE_FIELDS));
   printk("ATHR_IG_ACL_RULE_TABLE0_LOWER:%X\n",athr_acl_reg_rd(ATHR_IG_ACL_RULE_TABLE0_LOWER));
   printk("ATHR_IG_ACL_RULE_TABLE0_UPPER:%X\n",athr_acl_reg_rd(ATHR_IG_ACL_RULE_TABLE0_UPPER));
   printk("ATHR_IG_ACL_RULE_TABLE1_LOWER:%X\n",athr_acl_reg_rd(ATHR_IG_ACL_RULE_TABLE1_LOWER));
   printk("ATHR_IG_ACL_RULE_TABLE1_UPPER:%X\n",athr_acl_reg_rd(ATHR_IG_ACL_RULE_TABLE1_UPPER));
   printk("ATHR_IG_ACL_RULE_TABLE2_LOWER:%X\n",athr_acl_reg_rd(ATHR_IG_ACL_RULE_TABLE2_LOWER));
   printk("ATHR_IG_ACL_RULE_TABLE2_UPPER:%X\n",athr_acl_reg_rd(ATHR_IG_ACL_RULE_TABLE2_UPPER));
   printk("ATHR_IG_ACL_RULE_TABLE3_LOWER:%X\n",athr_acl_reg_rd(ATHR_IG_ACL_RULE_TABLE3_LOWER));
   printk("ATHR_IG_ACL_RULE_TABLE3_UPPER:%X\n",athr_acl_reg_rd(ATHR_IG_ACL_RULE_TABLE3_UPPER));

}

#endif /* #ifndef _ATHR_HW_ACL_H */


