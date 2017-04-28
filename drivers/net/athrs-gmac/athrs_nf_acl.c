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

#include "athrs_nf_acl.h"


int eg_global_rule = NF_ACCEPT;
int ig_global_rule = NF_ACCEPT;
//int eg_global_rule = NF_DROP;
//int ig_global_rule = NF_DROP;
//int athr_rmap_done = 0;

int eg_rule_cnt = 0;
int ig_rule_cnt = 0;

athr_nf_chain_t *cmd_chain;
athr_acl_rmap_t *rule_map;
athr_nf_cmd_t   *nf_cmd_list;

ath_acl_rule_t eg_tb[RULE_TB_SIZE];
ath_acl_rule_t ig_tb[RULE_TB_SIZE];


extern int athr_acl_add_rule(ath_acl_rule_t *rule);
extern int athr_acl_flush_tb(int dir);

extern athr_hw_acl_ops_t *hw_ops;
static athr_hw_acl_ops_t *ops = NULL;

char c_action[][10] = {"drop\0","accept\0"};
char c_dir[][10]={"","egress\0","ingress\0"};

struct xt_mtchk_param *mtpar;

void display_chain(athr_nf_chain_t *chain)
{
  printk("-----------------------------\n");
  printk("chain name:%s \n",chain->name);
  printk("direction :%s \n",c_dir[chain->dir]);
  printk("no of cmds:%d \n",chain->no_cmds);
  printk("nf cmd offset:%d\n",chain->kc_offset);
  printk("qol:%d\n",chain->qol);
  printk("-----------------------------\n");
 
}

void display_cmd(athr_nf_cmd_t *cmd)
{
  printk("-----------------------------\n");
  printk("Type of cmd:%d\n",cmd->cmd);
  printk("cmd direction:%s\n",c_dir[cmd->dir]);
  printk("action    :%s \n",c_action[cmd->action]);
  printk("operand0:%x\n",cmd->operand0);
  printk("operand1:%x\n",cmd->operand1);
  printk("proto:%x\n",cmd->proto);
  printk("qol:%d\n",cmd->qol);
  printk("-----------------------------\n");
}

void display_rule(athr_acl_rmap_t *r_map)
{
  printk("-----------------------------\n");
  printk("Type of cmd0:%d\n",r_map->cmds[0]);
  printk("Type of cmd1:%d\n",r_map->cmds[1]);
  printk("Type of cmd2:%d\n",r_map->cmds[2]);
  printk("Type of cmd3:%d\n",r_map->cmds[3]);
  printk("Type of cmd4:%d\n",r_map->cmds[4]);
  printk("cmd direction:%s\n",c_dir[r_map->dir]);
  printk("action    :%s:%d \n",c_action[r_map->action],r_map->action);
  printk("operand0:%x\n",r_map->operand0);
  printk("operand1:%x\n",r_map->operand1);
  printk("rem_bits:%x\n",r_map->rem_bits);
  printk("no of cmds:%x\n",r_map->cmd_no);
  printk("NEP:%d\n",r_map->nep);
  printk("TYPE:%x\n",r_map->type);
  printk("-----------------------------\n");
}


int athr_init_nf_chain(void)
{
    INIT_LIST_HEAD(&cmd_chain->list);
    return 0;
}

int athr_init_nf_cmd_list(athr_nf_cmd_t *cmd_nf_lst)
{
    INIT_LIST_HEAD(&cmd_nf_lst->list);
    return 0;
}

int athr_init_rule_map(athr_acl_rmap_t *rule_map)
{
    rule_map->cmd_no = 0;
    rule_map->rem_bits = 64;
    rule_map->action = -1;
    return 0;
}

int athr_add_cmd_to_chain(void *arg,athr_nf_cmd_t *cmd,int cmdno,int action)
{
    athr_nf_chain_t *chain = (athr_nf_chain_t *)arg;
    athr_nf_cmd_t *tmp_cmd;

    assert(chain->cmd_tb);
    
    cmd->kc_offset = cmdno;
    cmd->action = action;
    chain->kc_offset = cmdno;
    chain->no_cmds++;
    chain->qol += cmd->qol;
    
    if (list_empty(&chain->cmd_tb->list)) 
    {
         list_add_tail(&cmd->list,&chain->cmd_tb->list);
         return 0;
    }

    list_for_each_entry(tmp_cmd,&chain->cmd_tb->list,list) 
    {
         if (cmd->qol >= tmp_cmd->qol) {

           if (list_is_singular(&chain->cmd_tb->list)) {
              list_add(&cmd->list,&chain->cmd_tb->list);
           }
           else {
              list_insert(&tmp_cmd->list,&cmd->list);
           }

           return 0;
         }
         
    }
    list_add_tail(&cmd->list,&chain->cmd_tb->list);
    return 0;
}

int athr_is_cmd_in_chain(athr_nf_chain_t *chain,athr_nf_cmd_t *cmd)
{
    athr_nf_cmd_t *tmp_cmd;
    athr_nf_cmd_t *cmd_tb;

    if(!chain)
         return 0;

    cmd_tb = chain->cmd_tb;

    list_for_each_entry(tmp_cmd,&cmd_tb->list,list) 
    {
        if (tmp_cmd->cmd == cmd->cmd && tmp_cmd->operand0 == cmd->operand0 &&
            tmp_cmd->operand1 == cmd->operand1 && tmp_cmd->dir == cmd->dir &&
            tmp_cmd->proto == cmd->proto)
               return 1;
    }

    return 0;
}
    
int athrs_set_global_rule(int g_rule,int dir) 
{
    if (dir == ATHR_ACL_EGRESS_DIR) 
       eg_global_rule = g_rule;
    if (dir == ATHR_ACL_INGRESS_DIR) 
       eg_global_rule = g_rule;

    return 0;
}  

void* athr_get_cmd_chain(char *name,int dir,int off)
{
    athr_nf_chain_t *cmd_ch;

    if (!cmd_chain) 
      return NULL;

    list_for_each_entry(cmd_ch, &cmd_chain->list, list) {
        if (!strcmp(cmd_ch->name,name) && (cmd_ch->dir == dir)
            && (cmd_ch->kc_offset == off))
            return cmd_ch;
    }

    return NULL;
 
}

int athrs_create_cmd_chain(char *name, struct ipt_ip *ip, int *iptb_dir, int action, int off)
{
   athr_nf_chain_t *cmd_ch = NULL;
   int dir;
  
   assert(iptb_dir);
   assert(ip);
  
   if (strncmp(ip->iniface, ATHR_ETH_WAN_IF, strlen(ATHR_ETH_WAN_IF)) == 0)	/* If interface is supplied change the direction */
	*iptb_dir = 0x2;
   else if (strncmp(ip->outiface, ATHR_ETH_WAN_IF, strlen(ATHR_ETH_WAN_IF)) == 0)
	*iptb_dir = 0x1;

   dir = *iptb_dir;

   if (athr_get_cmd_chain(name,dir,off) != NULL)
       return 0;

   printk("created a %s chain\n",c_dir[dir]);
   if(!cmd_chain) /* create the first chain */
   {
       cmd_chain = (athr_nf_chain_t *)kmalloc(sizeof(athr_nf_chain_t),GFP_KERNEL);
       athr_init_nf_chain();
   }

   cmd_ch = (athr_nf_chain_t *)kmalloc(sizeof(athr_nf_chain_t),GFP_KERNEL);
   list_add_tail(&cmd_ch->list,&cmd_chain->list);

   cmd_ch->cmd_tb = kmalloc(sizeof(athr_nf_cmd_t),GFP_KERNEL);
   athr_init_nf_cmd_list(cmd_ch->cmd_tb);
   cmd_ch->dir = dir;
   strcpy(cmd_ch->name,name);
   cmd_ch->kc_offset = off;
   cmd_ch->no_cmds = 0;
   cmd_ch->qol = 0;

   return 0;
  
}

int athr_add_rule(athr_acl_rmap_t *r_map)
{
     ath_acl_rule_t rule;

     assert(r_map);

     while(r_map->cmd_no < 5) {
       r_map->cmds[r_map->cmd_no] = CMD_NOP; /* fill no ops */
       r_map->cmd_no++;
     }

     rule.cmd0 = r_map->cmds[0];
     rule.cmd1 = r_map->cmds[1];
     rule.cmd2 = r_map->cmds[2];
     rule.cmd3 = r_map->cmds[3];
     rule.cmd4 = r_map->cmds[4];
     rule.is_last = 0;
     rule.operand0 = r_map->operand0;
     rule.operand1 = r_map->operand1;
     rule.action  = r_map->action + 2;
     rule.dir     = r_map->dir;
     rule.is_head = 0;
     rule.is_link = 0;

     if (TYPE_OF(r_map->type) == R_HEAD)
        rule.is_head = 1;
 
     if (TYPE_OF(r_map->type) == R_LINK)
        rule.is_link = 1;

     if (r_map->dir == ATHR_ACL_EGRESS_DIR) {
        rule.g_rule = eg_global_rule;
        rule.entry_addr = eg_rule_cnt;
        
        if (r_map->nep) {
           rule.nep = eg_rule_cnt + 1;
           rule.is_link = 1;
        }
        else {
           rule.nep = 0;
           rule.is_link = 0;
        }
        memcpy(&eg_tb[eg_rule_cnt],&rule,sizeof(ath_acl_rule_t));
        eg_rule_cnt++;
    }

    if (r_map->dir == ATHR_ACL_INGRESS_DIR) {
        rule.g_rule = ig_global_rule;
        rule.entry_addr = ig_rule_cnt;

        if (r_map->nep) {
           rule.nep = ig_rule_cnt + 1;
           rule.is_link = 1;
        }
        else {
           rule.nep = 0;
           rule.is_link = 0;
        }

        memcpy(&ig_tb[ig_rule_cnt],&rule,sizeof(ath_acl_rule_t));
        ig_rule_cnt++;
    }

   // kfree(r_map);

   return 0;
}

int athr_set_rule(athr_acl_rmap_t *r_map,athr_nf_cmd_t *iptb_cmd , int tb_idx,int r_action,int nep)
{


    if (cmdopts[tb_idx].op_len == QOL_48BIT && (r_map->rem_bits == 64)
        && (r_map->action == r_action || r_map->action < 0)) { /* add as a rule if the cmd QOL is 48 bits */

         r_map->cmds[r_map->cmd_no] = iptb_cmd->cmd;
         r_map->operand0 = (iptb_cmd->operand0 & cmdopts[tb_idx].op1_mask);
         r_map->operand1 = (iptb_cmd->operand1 & cmdopts[tb_idx].op2_mask);
         r_map->rem_bits -= cmdopts[tb_idx].op_len;
         r_map->action = r_action;
         r_map->dir = iptb_cmd->dir;
         r_map->cmd_no++;
         r_map->nep = nep;
           
         if (iptb_cmd->is_last) {
            r_map->rem_bits = 0;
         }

         return 0;
    }
    else if (cmdopts[tb_idx].cmd_type >= CMD_L4_DST_PORT  &&  
             cmdopts[tb_idx].cmd_type <= CMD_L4_SRC_PORT2 &&
             (r_map->action == r_action || r_map->action < 0) &&
             (cmdopts[tb_idx].op_len <= r_map->rem_bits)) {  
   
         /* add as a rule if udp/tcp ports to be matched 32 + 8(AND) + 8*/

         unsigned char *ops = (char *)(&r_map->cmds[0]);
         int offset = 0,proto = -1;

         ops = ops + 8;

         if (r_map->rem_bits < 64 && !iptb_cmd->is_and) {   
            r_map->cmds[r_map->cmd_no] = cmdopts[20].cmd_type; /* append oR command */
            r_map->cmd_no++;
         }

         set_offset(offset,r_map->rem_bits);

         if (iptb_cmd->proto == IPPROTO_TCP) {
            //proto = 0x1;  /* Modified for TCP issue*/
            proto = 0x4;
         }
         else if (iptb_cmd->proto == IPPROTO_UDP) {
            proto = 0x2;
         }
         else if (iptb_cmd->proto == IPPROTO_ICMP) {
            proto  = (0x1);
         }
         else {
            printk("########## unknow protocol\n");
            return -1;

         }

         r_map->cmds[r_map->cmd_no] = CMD_TUI_L3_PROT;
         set_offset(offset,r_map->rem_bits);
         ops[offset] = proto;
         r_map->rem_bits -= 8;
         r_map->cmd_no++;
   
         r_map->cmds[r_map->cmd_no] = iptb_cmd->cmd;

         set_offset(offset,r_map->rem_bits);
         ops[offset] =  (iptb_cmd->operand0 & 0xff);
         r_map->rem_bits -= 8;

         set_offset(offset,r_map->rem_bits);
         ops[offset] =  ((iptb_cmd->operand0 & 0xff00) >> 8 ); /* first 16 bits */
         r_map->rem_bits -= 8;

         set_offset(offset,r_map->rem_bits);
         ops[offset] =  (iptb_cmd->operand1 & 0xff);
         r_map->rem_bits -= 8;

         set_offset(offset,r_map->rem_bits);
         ops[offset] =  ((iptb_cmd->operand1 & 0xff00) >> 8 ); /* Range */
         r_map->cmd_no++;
         r_map->rem_bits -= 8;
         
         r_map->action = r_action;
         r_map->dir = iptb_cmd->dir;
         r_map->nep = nep;
           
         if (iptb_cmd->is_last) {
            r_map->rem_bits = 0;
         }

         return 0;

    }
    else if ( (cmdopts[tb_idx].op_len <= r_map->rem_bits) && (r_map->cmd_no < 4) &&
             (r_map->action == r_action || r_map->action < 0)) { /* action */

         unsigned char *ops = (char *)(&r_map->cmds[0]);
         int offset = -1;
         ops = ops + 8;

         if (r_map->rem_bits < 64 && !iptb_cmd->is_and) {   
            r_map->cmds[r_map->cmd_no] = cmdopts[20].cmd_type; /* append oR command */
            r_map->cmd_no++;
         }

	 set_offset(offset,r_map->rem_bits);

         r_map->cmds[r_map->cmd_no] = iptb_cmd->cmd;
         r_map->cmd_no++;

         if (cmdopts[tb_idx].no_ops > 1) {
	     set_offset(offset,r_map->rem_bits);
             ops[offset] = iptb_cmd->operand0 & cmdopts[tb_idx].op1_mask;
             r_map->rem_bits -= 8;
             
	     set_offset(offset,r_map->rem_bits);
             ops[offset] =  iptb_cmd->operand1 & cmdopts[tb_idx].op1_mask;
             r_map->rem_bits -= 8;

             r_map->action = r_action;
             r_map->dir = iptb_cmd->dir;

             r_map->nep = nep;
               
             if (iptb_cmd->is_last) {
                r_map->rem_bits = 0;
             }
             return 0;
         }
         else if (cmdopts[tb_idx].no_ops == 1) {
	     set_offset(offset,r_map->rem_bits);
             ops[offset] = iptb_cmd->operand0 & cmdopts[tb_idx].op1_mask;
             r_map->rem_bits -= 8;

             r_map->action = r_action;
             r_map->dir = iptb_cmd->dir;

             r_map->nep = nep;
               
             if (iptb_cmd->is_last) {
                r_map->rem_bits = 0;
             }
             return 0;
         }
         else {
              printk("ERROR:No space to form a rule\n");
              return -1;
         }
    }
    assert(0);
    printk("ERROR:No space to form a rule\n");
    return -1;

}

/* The function will add a command to rule slot, for each rule slot  
 * in the list , check if space is available to accomdate this
 * command , else add the command to a new rule slot.
 *
 * After adding a rule to a slot append a OR command to resemble
 * iptables parsing of rules.
 *
 * On adding a cmd if the rule slot is full add the rule slot to the
 * rule table.
 *
 * @cmd_cnt - contains the no of commands per iptables filter entry
 */
int athr_form_rule(athr_nf_cmd_t *iptb_cmd , int tb_idx,int r_action,int nep,int type, int cmd_cnt)
{
    athr_acl_rmap_t *r_map = NULL;
    int ret = -1;
    int cmd_dir = iptb_cmd->dir;
    int *rule_cnt = (cmd_dir == ATHR_ACL_EGRESS_DIR) ? &eg_rule_cnt : &ig_rule_cnt;

    if (nep) {
        if (cmd_cnt > (RULE_TB_SIZE - *rule_cnt))
           return -2;
    }

    if (*rule_cnt >= RULE_TB_SIZE) { 
        return -1;
    }

    if (!rule_map) {
       rule_map = kmalloc(sizeof(athr_acl_rmap_t),GFP_KERNEL);
       memset(rule_map,0,sizeof(athr_acl_rmap_t));
       INIT_LIST_HEAD(&rule_map->list);
       athr_init_rule_map(rule_map);
    }

    list_for_each_entry(r_map, &rule_map->list, list) 
    {
        if (r_map->rem_bits != 0 && (cmdopts[tb_idx].op_len + 8) <= r_map->rem_bits &&
            r_map->action == r_action && r_map->dir == iptb_cmd->dir && r_map->type == type &&
            r_map->cmd_no < 4) {

             ret = athr_set_rule(r_map,iptb_cmd,tb_idx,r_action,nep);

             if (ret == 0) {
                *rule_cnt = *rule_cnt + 1;
               //  display_rule(r_map);
                 return 0;
             }
        }
            
    } /* End of list */
    
    /* if no space found in exsisting rules add cmd to a new rule slot */

    r_map = kmalloc(sizeof(athr_acl_rmap_t),GFP_KERNEL);
    memset(r_map,0,sizeof(athr_acl_rmap_t));
    athr_init_rule_map(r_map);
    r_map->type = type;
    list_add_tail(&r_map->list,&rule_map->list);
    ret = athr_set_rule(r_map,iptb_cmd,tb_idx,r_action,nep);

    if  (ret == 0) {
        *rule_cnt = *rule_cnt + 1;
     // display_rule(r_map);
        return 0;
    }
    return ret;

}

int athr_delete_chains(void)
{
 
    athr_nf_chain_t *cmd_ch;
    athr_nf_cmd_t *cmd_tb;  
    athr_nf_cmd_t *iptb_cmd; 
    void *free_list;
    void *cmd_free_list;

    printk("%s...\n",__func__);
    if (cmd_chain)
    {
	free_list = list_entry((&cmd_chain->list)->next, typeof(*cmd_ch), list);
	list_for_each_entry(cmd_ch, &cmd_chain->list, list)
        {  /* list through each chain */

            cmd_tb = cmd_ch->cmd_tb;

            ATHRS_ACL_LIST_ITERATE_AND_FREE(iptb_cmd, &cmd_tb->list, cmd_free_list);
            if (cmd_ch != free_list) {
		kfree (free_list);
	    }

	    free_list = cmd_ch;
            kfree (cmd_tb);
        }

	kfree (free_list);
        kfree (cmd_chain);
        cmd_chain = NULL;
    }
    return 0;
}


int athr_flush_rmaps(void)
{
    athr_acl_rmap_t *r_map;
    void *free_list;

    printk("%s...\n",__func__);

    if (rule_map)
    {
        ATHRS_ACL_LIST_ITERATE_AND_FREE(r_map, &rule_map->list, free_list);
        kfree (rule_map);
        rule_map = NULL;
    }

    return 0;
}

int athr_flush_rules(int flush_chains)
{
   int i = 0;

   printk("flush all rules\n");

   athr_acl_flush_tb(ATHR_ACL_EGRESS_DIR);
   athr_acl_flush_tb(ATHR_ACL_INGRESS_DIR);

   for (i = 0; i < eg_rule_cnt ; i++) {

       memset(&eg_tb[i],0,sizeof(ath_acl_rule_t));
   }

   for (i = 0; i < ig_rule_cnt ; i++) {

       memset(&ig_tb[i],0,sizeof(ath_acl_rule_t));
   }

   eg_rule_cnt = 0;
   ig_rule_cnt = 0;
   athr_flush_rmaps();

   if (flush_chains)
       athr_delete_chains();

   return 0;
}

int athr_commit_rules(void)
{
   int i = 0;

   printk("adding %d egress rules\n",eg_rule_cnt);
   printk("adding %d ingress rules\n",ig_rule_cnt);

   for (i = 0; i < eg_rule_cnt ; i++) {

       if (i == (eg_rule_cnt - 1)) {
            eg_tb[i].is_last = 1;
       }
#if 0 
       if (i == 0) {
         athr_acl_rmw_set(ATHR_GMAC_FREE_TIMER,ATHR_GMAC_TX_PAUSE);
         while((athr_acl_reg_rd(0xb90001a8) & 0x3ff))
	 {
		printk("Waiting ......\n");
	 }
       }
#endif
       athr_acl_add_rule(&eg_tb[i]);
   }

   athr_acl_rmw_clear(ATHR_GMAC_FREE_TIMER,ATHR_GMAC_TX_PAUSE);

   for (i = 0; i < ig_rule_cnt ; i++) {

#if 0
       if (i == 0)
            athr_acl_rmw_set(ETH_IG_NAT_STATUS,(0x1 << 31));
#endif
       if (i == (ig_rule_cnt - 1)) {
            ig_tb[i].is_last = 1;
       }

       athr_acl_add_rule(&ig_tb[i]);
   }
#if 0
   athr_acl_rmw_clear(ETH_IG_NAT_STATUS,(0x1 << 31));
#endif

  return 0;
}

int athr_rmap_to_acl_tb(void)
{
    athr_acl_rmap_t *r_map;

    list_for_each_entry(r_map, &rule_map->list, list) 
    {
      // display_rule(r_map);
       athr_add_rule(r_map);
    }
   
    athr_commit_rules();
 
    return 0;
       
}

int athr_process_chain(void)
{
 
    athr_nf_chain_t *cmd_ch;
    athr_nf_cmd_t *cmd_tb;  
    athr_nf_cmd_t *iptb_cmd; 

    int i = 0,rule_cnt = 0,nep = 0;
    int ret = 0;

    athr_flush_rules(0);				   /* Do not flush chains */
    list_for_each_entry(cmd_ch, &cmd_chain->list, list) {  /* list through each chain */
        int count = 0;
        int cmd_cnt = 1;
        int qol = 0;

        cmd_tb = cmd_ch->cmd_tb;

        rule_cnt = cmd_ch->qol / 64;
        if(rule_cnt % 64)
        //if(cmd_ch->qol % 64)
              rule_cnt++;
		
        list_for_each_entry(iptb_cmd,&cmd_tb->list,list) { /* list through each cmds in the chain */
            uint16_t type = 0;
             
             type = (type | (iptb_cmd->kc_offset << 8));
             for(i = 0; i<= ACL_CMD_MAX; i++) {
                 if (iptb_cmd->cmd == cmdopts[i].cmd_type) 
                 {
                     if(cmd_ch->no_cmds > 1) 
                     {
                          printk("1:rule cnt:%d qol:%d count:%d iptb_cmd->qol %d\n",rule_cnt,qol,count, iptb_cmd->qol);
                         //if (((qol + iptb_cmd->qol) > 64) && rule_cnt > 1) 
                         if (((qol + iptb_cmd->qol) >= 64) && rule_cnt > 1) {
                              qol = 0;
                              printk("rule count dec\n");
                              rule_cnt--;
                              count++;
                          }
                          printk("2:rule cnt:%d qol:%d count:%d\n",rule_cnt,qol,count);
                          iptb_cmd->is_and = 1;

                          if (rule_cnt > 1)
                              nep = 1;
                          else
                             nep = 0;

                          if (count == 0)
                             type |= R_HEAD;
                          else if (count < rule_cnt)
                             type |= R_LINK;
                          else
                             type |= R_END;

                          if (cmd_cnt == cmd_ch->no_cmds)
                             iptb_cmd->is_last = 1;

                          qol += iptb_cmd->qol;

                          ret = athr_form_rule(iptb_cmd,i,iptb_cmd->action,nep,type, cmd_ch->no_cmds);
                          cmd_cnt ++;
                     }
                     else 
                     {
			type |= R_HEAD;
                        ret = athr_form_rule(iptb_cmd,i,iptb_cmd->action,0, type, 1);
                     }
                }
            }
            if (ret) {
                break;
            }
        }
        if (ret == -1) {
            break;
        }
    }

    eg_rule_cnt = 0;
    ig_rule_cnt = 0;

    athr_rmap_to_acl_tb(); /* add all the rules to tables */
    return 0;

}

int athr_nf_parse_ip(struct ipt_ip *ip,athr_nf_cmd_t *cmd)
{
    int cmd_t = 0,proto = 0, range = 0;
    int i = 0;

    assert(ip);
#if 0
    if (ip->proto == IPPROTO_TCP) {
        proto = 1;
    }
    else if (ip->proto == IPPROTO_UDP) {
        proto = 2;
    }
    else if (ip->proto == IPPROTO_ICMP) {
        proto = 4;
    }
#endif

    if (ip->src.s_addr) {
        cmd_t = CMD_L3_IP_SRC_ADDR;
        cmd->operand0 = ip->src.s_addr;
        for (i = 0; i <=31 ; i++) 
            range +=  ((ip->smsk.s_addr >> i)& 0x1);
        cmd->operand1 = (range & 0x3f); /* range */
    }

    if (ip->dst.s_addr) {
        cmd_t = CMD_L3_IP_DST_ADDR;
        cmd->operand0 = ip->dst.s_addr;
        for (i = 0; i <=31 ; i++) {
            range +=  ((ip->dmsk.s_addr >> i)& 0x1);
	}
        cmd->operand1 = (range & 0x3f);
    }

    if ((proto) && (cmd_t == CMD_L3_IP_SRC_ADDR)) {
       cmd_t = CMD_L3_IP_SRC_PROT;
       cmd->operand1 |= ((ip->proto & 0x7) << 6);
    }

    if ((proto) && (cmd_t == CMD_L3_IP_DST_ADDR)) {
       cmd_t = CMD_L3_IP_DST_PROT;
       cmd->operand1 |= ((ip->proto & 0x7) << 6);
    }

    if (cmd_t == CMD_NOP) {
       return -1;
    }

    cmd->cmd = cmd_t ;
    cmd->qol = QOL_48BIT;
    return 0;

}

int athr_nf_parse_match(void *arg,athr_nf_cmd_t *cmd,char *name)
{
    int cmd_t = 0;

   
    if (strcmp(name,"mac") == 0) {

        struct xt_mac_info *mac = (struct xt_mac_info *)arg;

        cmd_t = CMD_SRC_MAC_ADDR;
        cmd->operand0 = ((mac->srcaddr[5] << 0)  |
                         (mac->srcaddr[4] << 8)  |
                         (mac->srcaddr[3] << 16) |
                         (mac->srcaddr[2] << 24));

        cmd->operand1 = ((mac->srcaddr[1] << 0) |
                         (mac->srcaddr[0] << 8));

        cmd->qol = QOL_48BIT;
        printk("invert:%d\n",mac->invert);

    }
    else if(strcmp(name,"tos") == 0) {

        struct xt_tos_match_info *tos = (struct xt_tos_match_info *)arg;

        cmd->operand0 = (tos->tos_value);
        //cmd->operand0 |= ((tos->tos_mask) << 8);
        cmd->operand1 = (tos->tos_mask);

        cmd_t = CMD_L3_IP_TOS;
        cmd->qol = QOL_16BIT;
    }
    else if(strcmp(name,"state") == 0) {

       struct xt_state_info *state =  (struct xt_state_info *)arg;
       
       cmd->operand0 = state->statemask;
       cmd_t = CMD_L4_CONN_STATE;
       cmd->qol = QOL_16BIT;
    }
    else {

         return -1;
    }

    cmd->cmd = cmd_t;

    return 0;

}


int athr_nf_parse_tcp(struct xt_tcp *tcp,athr_nf_cmd_t *cmd)
{
    int cmd_t = 0;

    assert(tcp);

    if (tcp->spts[0] > 0) {

        if (tcp->spts[1] !=0) 
            cmd_t = CMD_L4_SRC_PORT;
        else
           cmd_t = CMD_L4_SRC_PORT2;

        cmd->operand0 = (tcp->spts[0] & 0xffff); /* start */
        cmd->operand1 = (tcp->spts[1] & 0xffff); /* range */
    }
    else if (tcp->dpts[0] > 0) {

        if (tcp->dpts[1] !=0) 
            cmd_t = CMD_L4_DST_PORT;
        else
           cmd_t = CMD_L4_DST_PORT2;

        cmd->operand0 = (tcp->dpts[0] & 0xffff);
        cmd->operand1 = (tcp->dpts[1] & 0xffff);
    }
    else if (tcp->flg_mask > 0)
    {

      cmd_t = CMD_L4_TCP_FLAGS;
      //cmd->operand0 = tcp->flg_mask;
      cmd->operand0 = (tcp->flg_cmp & 0xff);
      cmd->operand1 = (tcp->flg_mask & 0xff);
    }

    cmd->cmd = cmd_t;
    cmd->qol = QOL_32BIT + 8;

    return 0;

}

int athr_nf_parse_udp(struct xt_udp *udp,athr_nf_cmd_t *cmd)
{
    int cmd_t = 0;
 
    assert(udp);

    if (udp->spts[0] > 0) {

        if (udp->spts[1] !=0) 
            cmd_t = CMD_L4_SRC_PORT;
        else
           cmd_t = CMD_L4_SRC_PORT2;

        cmd->operand0 = (udp->spts[0] & 0xffff);
        cmd->operand1 = (udp->spts[1] & 0xffff);
    }
    else if (udp->dpts[0] > 0) {

        if (udp->dpts[1] !=0) 
            cmd_t = CMD_L4_DST_PORT;
        else
           cmd_t = CMD_L4_DST_PORT2;

        cmd->operand0 = (udp->dpts[0] & 0xffff);
        cmd->operand1 = (udp->dpts[1] & 0xffff);
    }
    cmd->cmd = cmd_t ;
    cmd->qol = QOL_32BIT + 8;

    return 0;

}

int athr_nf_parse_icmp(struct ipt_icmp *icmp,athr_nf_cmd_t *cmd)
{
    int cmd_t = 0;

    assert(icmp);

   printk("code 0:%x\n",icmp->code[0]);
   printk("code 1:%x\n",icmp->code[1]);
   printk("type:%x\n",icmp->type);
   printk("invflags:%x\n",icmp->invflags);

    if (icmp->code[0] > 0)
    {
       cmd_t = CMD_L3_ICMP_CODE;
       cmd->operand0 = icmp->code[0];
       cmd->operand1 = icmp->code[1];
    }
    else {

       cmd_t = CMD_L3_ICMP_TYPE;
       cmd->operand0 = icmp->type;
       cmd->operand1 = 0;

    }
    cmd->cmd = cmd_t;
    if (cmd_t == CMD_L3_ICMP_CODE)
        cmd->qol = QOL_16BIT;
    else
        cmd->qol = QOL_8BIT;
    return 0;
}

void athr_invert_dir(athr_nf_cmd_t *cmd)
{
     if (cmd->dir == ATHR_ACL_INGRESS_DIR)
         cmd->dir = ATHR_ACL_EGRESS_DIR;
     else if (cmd->dir == ATHR_ACL_EGRESS_DIR)
         cmd->dir = ATHR_ACL_INGRESS_DIR;
}

int athr_nf_parse_cmds(char *name,void *mt_par,int cmdno,int dir,int action)
{
    athr_nf_cmd_t *cmd = NULL;
    athr_nf_chain_t *chain;
    int ret = 0, added_to_chain = -1;

    chain = athr_get_cmd_chain(name,dir,cmdno);
    
    assert(mt_par);
    assert(mtpar);

    memcpy(mtpar,mt_par,sizeof(struct xt_mtchk_param));

    if(!chain) {
        printk("ERROR:Unable to find chain:%s with dir:%x\n",name,dir);
        assert(0);
        return -2;
    }
 
    cmd = (athr_nf_cmd_t *)kmalloc(sizeof(athr_nf_cmd_t),GFP_KERNEL);
    memset(cmd,0,sizeof(athr_nf_cmd_t));

    assert(mtpar->match);

    cmd->dir = dir;

    printk("proto:%X\n", mtpar->match->proto);
    printk("name:%s\n", mtpar->match->name);

    ret = athr_nf_parse_ip((void *)mtpar->entryinfo,cmd);

    if (ret == 0 && !athr_is_cmd_in_chain(chain,cmd)) {
        printk("########## added IP command ###########\n");
        athr_add_cmd_to_chain(chain,cmd,cmdno,action);
        display_chain(chain);
        display_cmd(cmd);
        cmd = (athr_nf_cmd_t *)kmalloc(sizeof(athr_nf_cmd_t),GFP_KERNEL);
        memset(cmd,0,sizeof(athr_nf_cmd_t));
        cmd->dir = dir;
	added_to_chain = 0;
    }

    if (mtpar->match->proto == IPPROTO_TCP) {
        cmd->proto = mtpar->match->proto;
        ret = athr_nf_parse_tcp(mtpar->matchinfo,cmd);
    }
    else if (mtpar->match->proto == IPPROTO_UDP) {
        cmd->proto = mtpar->match->proto;
        ret = athr_nf_parse_udp(mtpar->matchinfo,cmd);
    }
    else if (mtpar->match->proto == IPPROTO_ICMP) {
        cmd->proto = mtpar->match->proto;
        ret = athr_nf_parse_icmp(mtpar->matchinfo,cmd);
    }
    else {
         ret = athr_nf_parse_match(mtpar->matchinfo,cmd,(char *)mtpar->match->name);
    }

    if(!cmd->cmd) {
        printk("Command unknow\n");
        chain->kc_offset = cmdno;
	kfree (cmd);
	cmd = NULL;
        return -1;
    }

    if (!athr_is_cmd_in_chain(chain,cmd)) {
        printk("########## added new command ###########\n");
        athr_add_cmd_to_chain(chain,cmd,cmdno,action);
        display_chain(chain);
        display_cmd(cmd);
	added_to_chain = 0;
    }

    if (added_to_chain != 0) {
	kfree (cmd);
	cmd = NULL;
    }

    return 0;
}

int athr_hw_acl_config(struct eth_cfg_params *cfg,int cmd,struct net_device *dev)
{
//      athr_gmac_t *mac = (athr_gmac_t *)ATHR_MAC_PRIV(dev);

      if (cfg->cmd == ATHR_ACL_COMMIT) {
	 if (cmd_chain)
             athr_process_chain();
         return 0;
      }

      if (cfg->cmd == ATHR_ACL_FLUSH) {
         athr_flush_rules(1);
         return 0;
      }

      return -1;
}

void athr_hw_acl_init(void)
{

    if (!ops) {
        ops = (athr_hw_acl_ops_t *)kmalloc(sizeof(athr_hw_acl_ops_t),GFP_KERNEL);

        ops->create_cmd_chain = athrs_create_cmd_chain;
        ops->get_cmd_chain = athr_get_cmd_chain;
        ops->nf_parse_cmds = athr_nf_parse_cmds;
        mtpar = kmalloc(sizeof(struct xt_mtchk_param), GFP_KERNEL);
        hw_ops = ops;
    }

    /* Work around - enable checksum*/
    athr_acl_rmw_set(0xb9000230,0x40);
    athr_acl_reg_wr(ATHR_EG_ACL_STATUS, (ATHR_EG_ACL_DISABLE | (1<<31)));
}  
