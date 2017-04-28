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

#include "athrs_gmac_hw_acl.h"


void athr_egress_acl_mac_enable_and_disable(unsigned char flag);
void athr_ingress_acl_enable_and_disable(unsigned char flag);

void display_acl_rule(ath_acl_rule_t *rule)
{
  printk("-----------------------------\n");
  printk("Type of cmd0:%x:%d\n",rule->cmd0,rule->cmd0);
  printk("Type of cmd1:%x:%d\n",rule->cmd1,rule->cmd1);
  printk("Type of cmd2:%x:%d\n",rule->cmd2,rule->cmd2);
  printk("Type of cmd3:%x:%d\n",rule->cmd3,rule->cmd3);
  printk("Type of cmd4:%x:%d\n",rule->cmd4,rule->cmd4);
  printk("cmd dir     :%x:%d\n",rule->dir,rule->dir);
  printk("action      :%x:%d \n",rule->action,rule->action);
  printk("operand0    :%x:%d\n",rule->operand0,rule->operand0);
  printk("operand1    :%x:%d\n",rule->operand1,rule->operand1);
  printk("entry addr  :%x:%d\n",rule->entry_addr,rule->entry_addr);
  printk("is last     :%x:%d\n",rule->is_last,rule->is_last);
  printk("is link     :%x:%d\n",rule->is_link,rule->is_link);
  printk("is head     :%x:%d\n",rule->is_head,rule->is_head);
  printk("NEP         :%x:%d\n",rule->nep,rule->nep);
  printk("-----------------------------\n");
}

void eg_reg_clear(void )
{
        athr_acl_reg_wr(ATHR_EG_ACL_CMD0_AND_ACTION, 0x0);
        athr_acl_reg_wr(ATHR_EG_ACL_CMD1234, 0x0);
}

void ig_reg_clear(void )
{
        athr_acl_reg_wr(ATHR_IG_ACL_CMD0_AND_ACTION, 0x0);
        athr_acl_reg_wr(ATHR_IG_ACL_CMD1234, 0x0);
}
int athr_acl_flush_tb(int dir)
{

    if (dir == ATHR_ACL_EGRESS_DIR ) {
       athr_egress_acl_mac_enable_and_disable(ATHR_EG_ACL_MAC_DISABLE);

       athr_acl_reg_wr(ATHR_EG_ACL_MEM_CTRL,ATHR_EG_ACL_INIT);
       while((athr_acl_reg_rd(ATHR_EG_ACL_MEM_CTRL) & ATHR_EG_ACL_INIT));
       athr_acl_rmw_clear(ATHR_EG_ACL_STATUS,ATHR_EG_ACL_DISABLE); 
       athr_egress_acl_mac_enable_and_disable(ATHR_EG_ACL_MAC_ENABLE);
       return 0;
    }
    if (dir == ATHR_ACL_INGRESS_DIR ) {

       athr_ingress_acl_enable_and_disable(ATHR_IG_ACL_MAC_DISABLE);
       athr_acl_reg_wr(ATHR_IG_ACL_MEM_CTRL,ATHR_IG_ACL_INIT);
       while((athr_acl_reg_rd(ATHR_IG_ACL_MEM_CTRL) & ATHR_IG_ACL_INIT));
        athr_ingress_acl_enable_and_disable(ATHR_IG_ACL_MAC_ENABLE);
       return 0;
    }

    return -1;

}
/* to disable or enable the mac whle programming the egress rule */
void athr_egress_acl_mac_enable_and_disable(unsigned char flag)
{
	int read_data=0x0;

	if(flag == ATHR_EG_ACL_MAC_ENABLE) {
            athr_acl_reg_wr(ATHR_ACL_GMAC_DMA_TX_CTRL,ATHR_GMAC_CFG1_TX_EN);
        }else if( flag == ATHR_EG_ACL_MAC_DISABLE) {
            athr_acl_reg_wr(ATHR_ACL_GMAC_DMA_TX_CTRL,DISABLE);
            read_data=athr_acl_reg_rd(ATHR_ACL_GMAC_DMA_XFIFO_DEPTH)& ATHR_GMAC_DMA_XFIFO_TX_DEPTH_MASK ;
            while(read_data)
             {
               read_data=athr_acl_reg_rd(ATHR_ACL_GMAC_DMA_XFIFO_DEPTH)& ATHR_GMAC_DMA_XFIFO_TX_DEPTH_MASK ;
             }
           athr_acl_rmw_set(ATHR_EG_ACL_STATUS,ATHR_EG_ACL_DISABLE);
        }
}
/* to disable or enable the mac whle programming the ingress rule */
void athr_ingress_acl_enable_and_disable(unsigned char flag)
{
	int read_data=0x0;

	if(flag == ATHR_IG_ACL_MAC_ENABLE) {
             athr_acl_rmw_set(ATHR_ACL_GMAC_CFG1,ATHR_GMAC_CFG1_RX_EN);
	 }else if( flag == ATHR_IG_ACL_MAC_DISABLE) {
             athr_acl_rmw_clear(ATHR_ACL_GMAC_CFG1,ATHR_GMAC_CFG1_RX_EN);
             read_data=(athr_acl_reg_rd(ATHR_ACL_GMAC_DMA_XFIFO_DEPTH) >> ATHR_GMAC_DMA_XFIFO_RX_DEPTH_MASK);
             while(read_data)
             {
               read_data= (athr_acl_reg_rd(ATHR_ACL_GMAC_DMA_XFIFO_DEPTH)>>ATHR_GMAC_DMA_XFIFO_RX_DEPTH_MASK);
             }
             athr_acl_rmw_set(ATHR_IG_ACL_STATUS,ATHR_IG_ACL_DISABLE);
	  }
}

int athr_acl_add_rule(ath_acl_rule_t *rule)
{
 
    unsigned int memctrl = 0x0;

    display_acl_rule(rule);

    if (rule->dir == ATHR_ACL_EGRESS_DIR)
    {
	eg_reg_clear();	/* Added */
         athr_egress_acl_mac_enable_and_disable(ATHR_EG_ACL_MAC_DISABLE);

	if (rule->entry_addr == 0) {
            athr_acl_reg_wr(ATHR_EG_ACL_MEM_CTRL,ATHR_EG_ACL_INIT);
            while((athr_acl_reg_rd(ATHR_EG_ACL_MEM_CTRL) & ATHR_EG_ACL_INIT));
        }


        if (rule->g_rule == ATHR_ACL_GLOBAL_RULE_DROP) {
            memctrl |= ATHR_EG_ACL_GLOBAL_RULE_DROP;
            memctrl |= ATHR_EG_ACL_GLOBAL_RULE_VALID;
        }

        athr_acl_rmw_clear(ATHR_EG_ACL_CMD0_AND_ACTION,0xf); /* clear actions */
        athr_acl_rmw_set(ATHR_EG_ACL_CMD0_AND_ACTION,ATHR_EG_SET_RULE(rule->action));

        if (rule->is_link) {
            athr_acl_rmw_set(ATHR_EG_ACL_CMD0_AND_ACTION,ATHR_EG_ACL_NEP(rule->nep));
            athr_acl_rmw_set(ATHR_EG_ACL_CMD0_AND_ACTION,ATHR_EG_ACTION_LINKED);
        }

        if (rule->is_head)
           athr_acl_rmw_set(ATHR_EG_ACL_CMD0_AND_ACTION,ATHR_EG_ACTION_HEAD); 

        athr_acl_rmw_set(ATHR_EG_ACL_CMD0_AND_ACTION,ATHR_EG_ACL_CMD0(rule->cmd0));
        athr_acl_rmw_set(ATHR_EG_ACL_CMD1234,ATHR_EG_ACL_CMD1(rule->cmd1));
        athr_acl_rmw_set(ATHR_EG_ACL_CMD1234,ATHR_EG_ACL_CMD2(rule->cmd2));
        athr_acl_rmw_set(ATHR_EG_ACL_CMD1234,ATHR_EG_ACL_CMD3(rule->cmd3));
        athr_acl_rmw_set(ATHR_EG_ACL_CMD1234,ATHR_EG_ACL_CMD4(rule->cmd4));
 
        athr_acl_reg_wr(ATHR_EG_ACL_OPERAND0,(rule->operand0));
        athr_acl_reg_wr(ATHR_EG_ACL_OPERAND1,(rule->operand1));

        memctrl |= ATHR_EG_ACL_ENTRY_ADDR(rule->entry_addr);

        if (rule->is_last) {
             memctrl |= ATHR_EG_ACL_TBL_WR;
             athr_acl_reg_wr(ATHR_EG_ACL_MEM_CTRL,(memctrl | ATHR_EG_ACL_LAST_ENTRY));
             while(!(athr_acl_reg_rd(ATHR_EG_ACL_MEM_CTRL) & ATHR_EG_ACL_ACK_REG)); /* Wait for Ack */
             while(!(athr_acl_reg_rd(ATHR_EG_ACL_MEM_CTRL) & ATHR_EG_ACL_RULE_MAP));
             athr_acl_rmw_clear(ATHR_EG_ACL_STATUS,ATHR_EG_ACL_DISABLE);
             //athr_acl_rmw_clear(ATHR_EG_ACL_STATUS,0x1 );	/* Added for Egress ACL fix verification */
             printk("Egress Rule map done\n");
        }
        else {
             memctrl |= ATHR_EG_ACL_TBL_WR;
             athr_acl_reg_wr(ATHR_EG_ACL_MEM_CTRL,(memctrl));
             while(!(athr_acl_reg_rd(ATHR_EG_ACL_MEM_CTRL) & ATHR_EG_ACL_ACK_REG)); /* Wait for Ack */

        }
        eg_reg_dump();
         athr_egress_acl_mac_enable_and_disable(ATHR_EG_ACL_MAC_ENABLE);
    }
    else if (rule->dir == ATHR_ACL_INGRESS_DIR)
    {
        ig_reg_clear();
        athr_ingress_acl_enable_and_disable(ATHR_IG_ACL_MAC_DISABLE);

	if (rule->entry_addr == 0) {
            //athr_acl_rmw_set(0xb9000230,(1 << 31));
            athr_acl_reg_wr(ATHR_IG_ACL_MEM_CTRL,ATHR_IG_ACL_INIT);
            while((athr_acl_reg_rd(ATHR_IG_ACL_MEM_CTRL) & ATHR_IG_ACL_INIT));
        }

        if (rule->g_rule == ATHR_ACL_GLOBAL_RULE_DROP) {
            memctrl |= ATHR_IG_ACL_GLOBAL_RULE_DROP;
            memctrl |= ATHR_IG_ACL_GLOBAL_RULE_VALID;
        }

        athr_acl_rmw_clear(ATHR_IG_ACL_CMD0_AND_ACTION,0xf); /* clear actions */
        athr_acl_rmw_set(ATHR_IG_ACL_CMD0_AND_ACTION,ATHR_IG_SET_RULE(rule->action));

        if (rule->is_link) {
            athr_acl_rmw_set(ATHR_IG_ACL_CMD0_AND_ACTION,ATHR_IG_ACL_NEP(rule->nep));
            athr_acl_rmw_set(ATHR_IG_ACL_CMD0_AND_ACTION,ATHR_IG_ACTION_LINKED);
        }

        if (rule->is_head)
            athr_acl_rmw_set(ATHR_IG_ACL_CMD0_AND_ACTION,ATHR_IG_ACTION_HEAD); 

        memctrl |= ATHR_IG_ACL_ENTRY_ADDR(rule->entry_addr);

        athr_acl_rmw_set(ATHR_IG_ACL_CMD0_AND_ACTION,ATHR_IG_ACL_CMD0(rule->cmd0));
        athr_acl_rmw_set(ATHR_IG_ACL_CMD1234,ATHR_IG_ACL_CMD1(rule->cmd1));
        athr_acl_rmw_set(ATHR_IG_ACL_CMD1234,ATHR_IG_ACL_CMD2(rule->cmd2));
        athr_acl_rmw_set(ATHR_IG_ACL_CMD1234,ATHR_IG_ACL_CMD3(rule->cmd3));
        athr_acl_rmw_set(ATHR_IG_ACL_CMD1234,ATHR_IG_ACL_CMD4(rule->cmd4));

        athr_acl_reg_wr(ATHR_IG_ACL_OPERAND0,(rule->operand0));
        athr_acl_reg_wr(ATHR_IG_ACL_OPERAND1,(rule->operand1));

        if (rule->is_last) {
             memctrl |= ATHR_IG_ACL_TBL_WR;
             athr_acl_reg_wr(ATHR_IG_ACL_MEM_CTRL,(memctrl | ATHR_IG_ACL_LAST_ENTRY));
             while(!(athr_acl_reg_rd(ATHR_IG_ACL_MEM_CTRL) & ATHR_IG_ACL_ACK_REG)); /* Wait for Ack */
             while(!(athr_acl_reg_rd(ATHR_IG_ACL_MEM_CTRL) & ATHR_IG_ACL_RULE_MAP));
             athr_acl_rmw_clear(ATHR_IG_ACL_STATUS,ATHR_IG_ACL_DISABLE);
        //     athr_acl_rmw_set(ATHR_IG_ACL_STATUS, ATHR_IG_STATUS_DEBUG);
             printk("Ingress Rule map done\n");
        }
        else {
             //athr_acl_rmw_set(ATHR_IG_ACL_MEM_CTRL,(memctrl |ATHR_IG_ACL_TBL_WR));
             athr_acl_reg_wr(ATHR_IG_ACL_MEM_CTRL,(memctrl |ATHR_IG_ACL_TBL_WR));
             while(!(athr_acl_reg_rd(ATHR_IG_ACL_MEM_CTRL) & ATHR_IG_ACL_ACK_REG)); /* Wait for Ack */
        }
//		athr_acl_rmw_clear(0xb9000230,0x80000000); */
        ig_reg_dump();
        athr_ingress_acl_enable_and_disable(ATHR_IG_ACL_MAC_ENABLE);
    }

    return 0;
}

void athr_acl_enable_tx(athr_gmac_t *mac,athr_gmac_desc_t *ds)
{
	if(mac->mac_unit == 0)
           ATHR_TX_HW_ACL_ENABLE(ds);
}
