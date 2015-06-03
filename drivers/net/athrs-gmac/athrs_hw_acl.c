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

#include "athrs_hw_acl.h"

//ath_acl_rule_t eg_tb[RULE_TB_SIZE];
//ath_acl_rule_t ig_tb[RULE_TB_SIZE];
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

       athr_acl_rmw_set(0xb90001b8, (1<<30));          /* Egress Fix (DMA PAUSE ENABLE) */
       while(athr_acl_reg_rd(0xb90001a8) & 0x3ff);     /* Wait for FIFO to clear out  */

       athr_acl_rmw_set(ATHR_EG_ACL_STATUS,ATHR_EG_ACL_DISABLE);

       athr_acl_reg_wr(ATHR_EG_ACL_MEM_CTRL,ATHR_EG_ACL_INIT);
       while((athr_acl_reg_rd(ATHR_EG_ACL_MEM_CTRL) & ATHR_EG_ACL_INIT));
       //athr_acl_reg_wr(ATHR_EG_ACL_STATUS,ATHR_EG_ACL_DISABLE); 
       athr_acl_rmw_clear(ATHR_EG_ACL_STATUS,ATHR_EG_ACL_DISABLE); 

       athr_acl_rmw_clear(0xb90001b8, (1<<30));          /* Egress Fix (DMA PAUSE DISABLE) */
       return 0;
    }
    if (dir == ATHR_ACL_INGRESS_DIR ) {

       athr_acl_rmw_set(ATHR_IG_ACL_STATUS,ATHR_IG_ACL_DISABLE);	/* Disable ACL before write */

       athr_acl_reg_wr(ATHR_IG_ACL_MEM_CTRL,ATHR_IG_ACL_INIT);
       while((athr_acl_reg_rd(ATHR_IG_ACL_MEM_CTRL) & ATHR_IG_ACL_INIT));
       //athr_acl_reg_wr(ATHR_IG_ACL_STATUS,ATHR_IG_ACL_DISABLE); 
       athr_acl_rmw_clear(ATHR_IG_ACL_STATUS,ATHR_IG_ACL_DISABLE); 
       return 0;
    }

    return -1;

}

int athr_acl_add_rule(ath_acl_rule_t *rule)
{
 
    unsigned int memctrl = 0x0;

    display_acl_rule(rule);

    if (rule->dir == ATHR_ACL_EGRESS_DIR)
    {
	eg_reg_clear();	/* Added */
	
	athr_acl_rmw_set(0xb90001b8, (1<<30));		/* Egress Fix (DMA PAUSE ENABLE) */
	while(athr_acl_reg_rd(0xb90001a8) & 0x3ff);     /* Wait for FIFO to clear out  */

        //athr_acl_reg_wr(ATHR_EG_ACL_STATUS,ATHR_EG_ACL_DISABLE); 
        athr_acl_rmw_set(ATHR_EG_ACL_STATUS,ATHR_EG_ACL_DISABLE); 

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
	athr_acl_rmw_clear(0xb90001b8, (1<<30));		/* Egress Fix (DMA PAUSE DISABLE) */
    }
    else if (rule->dir == ATHR_ACL_INGRESS_DIR)
    {
        ig_reg_clear();

	athr_acl_rmw_set(0xb9000230, (1<<31));		/* Ingress fix - IG_FE_DROP_ENABLE  */
	while((athr_acl_reg_rd(0xb90001a8)>>16) & 0x3ff);     /* Wait for FIFO to clear out  */
	printk("Fifo Lengths: 0x%x\n",athr_acl_reg_rd(0xb90001a8));

        athr_acl_reg_wr(ATHR_IG_ACL_STATUS,ATHR_IG_ACL_DISABLE);

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
	printk("Fifo Lengths: 0x%x\n",athr_acl_reg_rd(0xb90001a8));
	athr_acl_rmw_clear(0xb9000230, (1<<31));		/* Ingress fix - IG_FE_DROP_DISABLE  */
    }

    return 0;
}
#if 0
int athr_acl_query_rule(ath_acl_rule_t *rule)
{
   ath_acl_rule_t *rule_tb = NULL;

   if (rule->dir == ATHR_ACL_EGRESS_DIR)
   {
     rule_tb = (ath_acl_rule_t *)eg_tb;
   }
   else if (rule->dir == ATHR_ACL_INGRESS_DIR)
   {
     rule_tb = (ath_acl_rule_t *)ig_tb;
   }
  
   while(!rule_tb->is_last) 
   {
     if(!memcmp(rule,rule_tb,sizeof(ath_acl_rule_t)))
        return 1;
     rule_tb++;
   }
   return 0;
}

int athr_acl_get_slot(ath_acl_rule_t *rule,int dir)
{
   ath_acl_rule_t *rule_tb = NULL;

   if (dir == ATHR_ACL_EGRESS_DIR)
   {
     rule_tb = (ath_acl_rule_t *)eg_tb;
   }
   else if (dir == ATHR_ACL_INGRESS_DIR)
   {
     rule_tb = (ath_acl_rule_t *)ig_tb;
   }
  
   while(!rule_tb->is_last) 
   {
     if (rule_tb->cmd1 == CMD_NOP) {
        rule = rule_tb;
        return 1;
     }
     rule_tb++; 
   }
   return 0;
}
#endif

void athr_acl_enable_tx(athr_gmac_t *mac,athr_gmac_desc_t *ds)
{
	if(mac->mac_unit == 0)
           ATHR_TX_HW_ACL_ENABLE(ds);
}
