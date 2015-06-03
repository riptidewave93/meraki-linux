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

#include "athrs_hw_nat.h"

spinlock_t athrs_nat_op_lock;
spinlock_t athrs_lut_op_lock;
static int nat_fail_status_cnt = 0;
extern void athr_nat_init(void);
extern void athr_nat_down(void);

void athrs_nat_op_lock_init(void)
{
      spin_lock_init(&athrs_nat_op_lock);
      spin_lock_init(&athrs_lut_op_lock);
}

static void athr_nat_frag_lut_reg_clear_status(int portdir)
{
      /* Clear the req, status regs to 0 before the operation */
      athr_reg_write_frag_lut(IG_FRAG_CPU_REQ_REG, 0, portdir);
      athr_reg_write_frag_lut(IG_FRAG_CPU_REQ_STAT_REG, 0, portdir);
}

static void athr_nat_lut_reg_clear_status(int portdir)
{
      /* Clear the req, status regs to 0 before the operation */
      athr_reg_write_lut(EG_CPU_REQ_REG, 0, portdir);
      athr_reg_write_lut(EG_CPU_REQ_STAT_REG, 0, portdir);
}

static void check_and_restart_nat(void)
{
    if (nat_fail_status_cnt == 3) {
        athr_nat_down();
        athr_nat_init();
        nat_fail_status_cnt = 0;
    }
    else
        nat_fail_status_cnt ++;
}

static int athrs_nat_hwtbl_ops_status_frag(int cmd,int dir)
{
   int cnt = 1000, res = 0;
   volatile int reg;

   cmd &= CMD_MASK;

   reg = (dir == INGRESS ? IG_FRAG_CPU_REQ_STAT_REG: EG_FRAG_CPU_REQ_STAT_REG);

   while (cnt != 0) {
       res = athr_reg_read_frag_lut(reg, dir);
       if (res & STAT_REQ_DONE) {
//           printk("%s: req done bit is set, lut op success cnt = %d\n", __func__, cnt);
           res = athr_reg_read_frag_lut(reg, dir);
           break;
       }
       cnt --;
   }

   if (cmd == CMD_INS) {
       if ((res & STAT_REQ_DONE) == STAT_REQ_DONE) {
            if (res & STAT_INSERT)
                return SUCCESS;
            else if (res & STAT_BKT_FULL)
                return -EBKT_FULL;
            else if (res & STAT_BIN_FULL)
                return -EBIN_FULL;
            else if (res & STAT_DUPLICATE)
                return -EDUP_KEY;
       }
       else
       {
           printk("%s: lut op CMD_INS req not done\n", __func__);
           check_and_restart_nat();
       }
   }
   else if (cmd == CMD_DEL || cmd == CMD_LKP)
   {
       if ((res & STAT_CMD_ACK) == STAT_CMD_ACK) {
          if ((res & STAT_DATA_FND) == STAT_DATA_FND)
             return SUCCESS;
          else
             return -ENODATA;
       }
       else
       {
           printk("%s: lut op CMD_DEL|CMD_LKP req not done\n", __func__);
           check_and_restart_nat();
       }
   }
   else
       printk("%s: lut op req bit not set\n", __func__);

   return -EINVAL;
}

static int athrs_nat_hwtbl_ops_status(int cmd,int dir)
{
   int cnt = 1000, reg, res = 0;

   cmd &= CMD_MASK;

   reg = (dir == INGRESS ? IG_CPU_REQ_STAT_REG : EG_CPU_REQ_STAT_REG);

   while (cnt != 0) {
       res = athr_reg_read_lut(reg, dir);
       if (res & STAT_REQ_DONE) {
//           printk("%s: req done bit is set, lut op success cnt = %d\n", __func__, cnt);
           res = athr_reg_read_lut(reg, dir);
           break;
       }
       cnt --;
   }

   if (cmd == CMD_INS) {
       if ((res & STAT_REQ_DONE) == STAT_REQ_DONE) {
            if (res & STAT_INSERT)
                return SUCCESS;
            else if (res & STAT_BKT_FULL)
                return -EBKT_FULL;
            else if (res & STAT_BIN_FULL)
                return -EBIN_FULL;
            else if (res & STAT_DUPLICATE)
                return -EDUP_KEY;
       }
       else
       {
           printk("%s: lut op CMD_INS req not done\n", __func__);
           check_and_restart_nat();
       }
   }
   else if (cmd == CMD_DEL || cmd == CMD_LKP)
   {
       if ((res & STAT_CMD_ACK) == STAT_CMD_ACK) {
          if ((res & STAT_DATA_FND) == STAT_DATA_FND)
             return SUCCESS;
          else
             return -ENODATA;
       }
       else
       {
           printk("%s: lut op CMD_DEL|CMD_LKP req not done\n", __func__);
           check_and_restart_nat();
       }
   }
   else
       printk("%s: lut op req bit not set\n", __func__);

   return -EINVAL;
}

int athrs_nat_lkp_frag(uint32_t ip, uint16_t port, uint32_t *ct_addr, int portdir, int cmd)
{

	int ret;
	spin_lock(&athrs_lut_op_lock);
	if(portdir == EGRESS) {
		athr_reg_write_frag_lut(EG_FRAG_KEY_DW0_REG, ip, portdir);
		athr_reg_write_frag_lut(EG_FRAG_KEY_DW1_REG, port, portdir);

		athr_nat_frag_lut_reg_clear_status(portdir);

		athr_reg_write_frag_lut(EG_FRAG_CPU_REQ_REG,
					(EG_REQ_ENABLE(1) | 
					EG_LUT_INIT(0) |
					EG_LUT_CMD(cmd & CMD_MASK)), portdir);

		ret = athrs_nat_hwtbl_ops_status_frag(cmd,portdir);
		if(!ret) {
			*ct_addr = (uint32_t)athr_reg_read_frag_lut(EG_FRAG_CPU_REQ_INFO_DW0, portdir);
		}
	} else
	{
		athr_reg_write_frag_lut(IG_FRAG_KEY_DW0_REG, ip, portdir);
		athr_reg_write_frag_lut(IG_FRAG_KEY_DW1_REG, port, portdir);

		athr_nat_frag_lut_reg_clear_status(portdir);

		athr_reg_write_frag_lut(IG_FRAG_CPU_REQ_REG,
					(IG_REQ_ENABLE(1) | 
					IG_LUT_INIT(0) |
					IG_LUT_CMD(cmd & CMD_MASK)), portdir);

		ret = athrs_nat_hwtbl_ops_status_frag(cmd,portdir);
		if(!ret) {
			*ct_addr = (uint32_t)athr_reg_read_frag_lut(IG_FRAG_CPU_REQ_INFO_DW0, portdir);
		}
	}
	spin_unlock(&athrs_lut_op_lock);
//	printk("athrs_nat_lkp_frag: ip = %x, port = %u, ct_addr = %x dir = %d, ret = %d\n", ip, port, *ct_addr, portdir, ret);
	return ret;
}

int athrs_nat_add_del_ops_frag(uint32_t key[], uint32_t info[], int portdir, int cmd)
{
	int ret = -1;

	spin_lock(&athrs_lut_op_lock);

	if (portdir == EGRESS) {
		if (cmd == CMD_INS) {
		    athr_reg_write_frag_lut(EG_FRAG_INFO_DW0_REG,
		                      info[0], portdir);
		}
		athr_reg_write_frag_lut(EG_FRAG_KEY_DW0_REG, key[0], portdir);
		athr_reg_write_frag_lut(EG_FRAG_KEY_DW1_REG, key[1], portdir);

		athr_nat_frag_lut_reg_clear_status(portdir);

		athr_reg_write_frag_lut(EG_FRAG_CPU_REQ_REG,
		                    (EG_PKT_TYPE(1) |
		                     EG_REQ_ENABLE(1) |
		                     EG_LUT_INIT(0) |
		                     EG_LUT_CMD(cmd & CMD_MASK)), portdir);

		ret = athrs_nat_hwtbl_ops_status_frag(cmd,portdir);
    //            printk("%s %d ret = %x\n", __func__, __LINE__, ret);
	}
   	else if (portdir == INGRESS) {
		if (cmd == CMD_INS) {
			athr_reg_write_frag_lut(IG_FRAG_INFO_DW0_REG, info[0], portdir);
//			athr_reg_write_frag_lut(IG_FRAG_INFO_DW1_REG, info[1], portdir);
//			athr_reg_write_frag_lut(IG_INFO_DW2_REG, info[2], portdir);
//			athr_reg_write_frag_lut(IG_INFO_DW3_REG, info[3], portdir);
		}

		athr_reg_write_frag_lut(IG_FRAG_KEY_DW0_REG, key[0], portdir);
		athr_reg_write_frag_lut(IG_FRAG_KEY_DW1_REG, key[1], portdir);
		athr_nat_frag_lut_reg_clear_status(portdir);

		athr_reg_write_frag_lut(IG_FRAG_CPU_REQ_REG,
						(IG_PKT_TYPE(1) |
						 IG_REQ_ENABLE(1) |
						 IG_LUT_INIT(0) |
						 IG_LUT_CMD(cmd & CMD_MASK)), portdir);

		ret = athrs_nat_hwtbl_ops_status_frag(cmd, portdir);
      //          printk("%s %d ret = %x\n", __func__, __LINE__, ret);
	}

   	spin_unlock(&athrs_lut_op_lock);
	return ret;
}


int athrs_nat_add_del_ops(uint32_t key[2], uint32_t info[4], int portdir, int cmd,
				uint8_t pkt_type)
{
	int ret = -1;

   	spin_lock(&athrs_nat_op_lock);

     	if (portdir == EGRESS) {
	     if (cmd == CMD_INS) {
	         athr_reg_write_lut(EG_INFO_INS_DW0_REG,
	                           info[0], portdir);
	     }
	     athr_reg_write_lut(EG_KEY_DW0_REG, key[0], portdir);
	     athr_reg_write_lut(EG_KEY_DW1_REG, key[1], portdir);

	     athr_nat_lut_reg_clear_status(portdir);

	     athr_reg_write_lut(EG_CPU_REQ_REG,
	                         (EG_PKT_TYPE(pkt_type) |
	                          EG_REQ_ENABLE(1) |
	                          EG_LUT_INIT(0) |
	                          EG_LUT_CMD(cmd & CMD_MASK)), portdir);

	     ret = athrs_nat_hwtbl_ops_status(cmd,portdir);
	}
   	else if (portdir == INGRESS) {
                if (cmd == CMD_INS) {
                    athr_reg_write_lut(IG_INFO_DW0_REG, info[0], portdir);
                    athr_reg_write_lut(IG_INFO_DW1_REG, info[1], portdir);

                    athr_reg_write_lut(IG_INFO_DW2_REG, info[2], portdir);
                    athr_reg_write_lut(IG_INFO_DW3_REG, info[3], portdir);
                }

                athr_reg_write_lut(IG_KEY_DW0_REG, key[0], portdir);
	        athr_nat_lut_reg_clear_status(portdir);

                athr_reg_write_lut(IG_CPU_REQ_REG,
                                  (IG_PKT_TYPE(pkt_type) |
                                   IG_REQ_ENABLE(1) |
                                   IG_LUT_INIT(0) |
                                   IG_LUT_CMD(cmd & CMD_MASK)), portdir);

                ret = athrs_nat_hwtbl_ops_status(cmd, portdir);
	}

   	spin_unlock(&athrs_nat_op_lock);
        return ret;
}

int
athrs_nat_hwtbl_ops(void *args, int portdir,int cmd)
{
   ath_egrs_t *egrs;
   ath_ingrs_t *ingrs;
   int ret = -1;
   //unsigned long flags;

   //spin_lock_irqsave(&athrs_nat_op_lock, flags);
   spin_lock(&athrs_nat_op_lock);

   if (portdir == EGRESS) {
       egrs =  (ath_egrs_t *)args;
       if (is_supp(egrs->prot_key)) {
          if (!is_icmp(egrs->prot_key)) { // For TCP/UDP
              if (cmd == CMD_INS) {
                  athr_reg_write_lut(EG_INFO_INS_DW0_REG,
                                    (EG_INFO_WAN_PORT(egrs->info.wan_port)|
                                     EG_INFO_WAN_IP_IDX(egrs->info.wan_ip_idx) |
                                     EG_INFO_CONN_STAT(egrs->info.l2_con_st)), portdir);
              }

              athr_reg_write_lut(EG_KEY_DW0_REG, (EG_KEY_DW0_LAN_ADDR(egrs->lan_ip_addr) |
                                 EG_KEY_DW0_PROT(egrs->prot_key) |
                                 EG_KEY_DW0_LAN_PORT(egrs->lan_port)), portdir);
          }

          else { // For ICMP Protocol
              if (cmd == CMD_INS) {
                  athr_reg_write_lut(EG_INFO_INS_DW0_REG,
                                    (EG_INFO_WAN_PORT(egrs->info.wan_icmp_id) |
                                     EG_INFO_WAN_IP_IDX(egrs->info.wan_ip_idx) |
                                     EG_INFO_CONN_STAT(egrs->info.l2_con_st)), portdir);
              }

              athr_reg_write_lut(EG_KEY_DW0_REG, (EG_KEY_DW0_LAN_ADDR(egrs->lan_ip_addr) |
                                 EG_KEY_DW0_PROT(egrs->prot_key) |
                                 EG_KEY_DW0_LAN_PORT(egrs->lan_icmp_id)), portdir);
          }

          athr_reg_write_lut(EG_KEY_DW1_REG, EG_KEY_DW1_LAN_ADDR(egrs->lan_ip_addr),
                             portdir);


          athr_nat_lut_reg_clear_status(portdir);

          athr_reg_write_lut(EG_CPU_REQ_REG,
                              (EG_PKT_TYPE(egrs->prot_key) |
                               EG_REQ_ENABLE(1) |
                               EG_LUT_INIT(0) |
                               EG_LUT_CMD(cmd & CMD_MASK)), portdir);

          ret = athrs_nat_hwtbl_ops_status(cmd,portdir);

       }
   }
   else if (portdir == INGRESS) {
     uint32_t lan_mac_ms;
     uint16_t lan_mac_ls;

     ingrs = (ath_ingrs_t *)args;
     lan_mac_ls = (ingrs->info.lan_mac_addr[5] | (ingrs->info.lan_mac_addr[4] << 8));
     lan_mac_ms = (ingrs->info.lan_mac_addr[3] | (ingrs->info.lan_mac_addr[2] << 8) |
                  (ingrs->info.lan_mac_addr[1] << 16) | (ingrs->info.lan_mac_addr[0] << 24));

        if (is_supp(ingrs->prot_key)) {
            if (!is_icmp(ingrs->prot_key)) {
                if (cmd == CMD_INS) {
                    athr_reg_write_lut(IG_INFO_DW0_REG,IG_INFO_DW0_LAN_IP(ingrs->info.lan_ip_addr), portdir);
                    athr_reg_write_lut(IG_INFO_DW1_REG,
					(IG_INFO_DW1_LAN_PORT(ingrs->info.lan_port) |
					IG_INFO_DW1_LAN_MAC_LS(lan_mac_ls)), portdir);

                    athr_reg_write_lut(IG_INFO_DW2_REG,IG_INFO_DW2_LAN_MAC_MS(lan_mac_ms), portdir);
                    athr_reg_write_lut(IG_INFO_DW3_REG,IG_INFO_DW3_CONN_STAT(ingrs->info.l2_con_st), portdir);
                }

                athr_reg_write_lut(IG_KEY_DW0_REG,
                                  (IG_KEY_DW0_WAN_PORT(ingrs->wan_port) |
                                   IG_KEY_DW0_PROT(ingrs->prot_key) |
                                   IG_KEY_DW0_WAN_IP_IDX(ingrs->wan_ip_idx)), portdir);
           }
           else {
                if (cmd == CMD_INS) {
                    athr_reg_write_lut(IG_INFO_DW0_REG,IG_INFO_DW0_LAN_IP(ingrs->info.lan_ip_addr), portdir);
                    athr_reg_write_lut(IG_INFO_DW1_REG,
                                      (IG_INFO_DW1_LAN_PORT(ingrs->info.lan_icmp_id) |
                                       IG_INFO_DW1_LAN_MAC_LS(lan_mac_ls)), portdir);

                    athr_reg_write_lut(IG_INFO_DW2_REG,IG_INFO_DW2_LAN_MAC_MS(lan_mac_ms), portdir);
                    athr_reg_write_lut(IG_INFO_DW3_REG,IG_INFO_DW3_CONN_STAT(ingrs->info.l2_con_st), portdir);
                }

                athr_reg_write_lut(IG_KEY_DW0_REG,
                                  (IG_KEY_DW0_WAN_PORT(ingrs->wan_icmp_id) |
                                   IG_KEY_DW0_PROT(ingrs->prot_key) |
                                   IG_KEY_DW0_WAN_IP_IDX(ingrs->wan_ip_idx)), portdir);

          }

          athr_nat_lut_reg_clear_status(portdir);

          athr_reg_write_lut(IG_CPU_REQ_REG,
                            (IG_PKT_TYPE(ingrs->prot_key) |
                             IG_REQ_ENABLE(1) |
                             IG_LUT_INIT(0) |
                             IG_LUT_CMD(cmd & CMD_MASK)), portdir);

          ret = athrs_nat_hwtbl_ops_status(cmd,portdir);
      }
   }

   //spin_unlock_irqrestore(&athrs_nat_op_lock, flags);
   spin_unlock(&athrs_nat_op_lock);
   return ret;
}

int
athrs_nat_hwtbl_lkup(void *args, int portdir)
{
   ath_egrs_t *egrs;
   ath_ingrs_t *ingrs;
   int ret = -1;
   //unsigned long flags;
   unsigned int info[2]; 
   int cmd = CMD_LKP;

   //spin_lock_irqsave(&athrs_nat_op_lock, flags);
   spin_lock(&athrs_nat_op_lock);

   if (portdir == EGRESS) {
       egrs =  (ath_egrs_t *)args;
       if (is_supp(egrs->prot_key)) {

            if (!is_icmp(egrs->prot_key)) {
                  athr_reg_write_lut(EG_KEY_DW0_REG, (EG_KEY_DW0_LAN_ADDR(egrs->lan_ip_addr) |
                                     EG_KEY_DW0_PROT(egrs->prot_key) |
                                     EG_KEY_DW0_LAN_PORT(egrs->lan_port)), portdir);
            }
            else {
                  athr_reg_write_lut(EG_KEY_DW0_REG, (EG_KEY_DW0_LAN_ADDR(egrs->lan_ip_addr) |
                                     EG_KEY_DW0_PROT(egrs->prot_key) |
                                     EG_KEY_DW0_LAN_PORT(egrs->lan_icmp_id)), portdir);

            }

            athr_reg_write_lut(EG_KEY_DW1_REG, EG_KEY_DW1_LAN_ADDR(egrs->lan_ip_addr), 
                                 portdir);

            athr_nat_lut_reg_clear_status(portdir);

            athr_reg_write_lut(EG_CPU_REQ_REG,
                               (EG_REQ_ENABLE(1) |
                                EG_LUT_CMD(cmd & CMD_MASK)), portdir);

            ret = athrs_nat_hwtbl_ops_status(cmd,portdir);

            info[0] = athr_reg_read_lut(EG_INFO_INS_DW0_REG, portdir);

            if ((info[0] & 3000) != 0) {
//		printk("Invalid WAN IP index \n");
            }
	    egrs->lan_port = (info[0] & 0xffff);
       }
   }
   else if (portdir == INGRESS) {
        ingrs = (ath_ingrs_t *)args;

		if (is_supp(ingrs->prot_key)) {

			if (!is_icmp(ingrs->prot_key)) {
			     athr_reg_write_lut(IG_KEY_DW0_REG,
			                       (IG_KEY_DW0_WAN_PORT(ingrs->wan_port) |
			                        IG_KEY_DW0_PROT(ingrs->prot_key)     |
			                        IG_KEY_DW0_WAN_IP_IDX(ingrs->wan_ip_idx)), portdir);
			}
			else {
			     athr_reg_write_lut(IG_KEY_DW0_REG,
			                       (IG_KEY_DW0_WAN_PORT(ingrs->wan_icmp_id) |
			                        IG_KEY_DW0_PROT(ingrs->prot_key)        |
			                        IG_KEY_DW0_WAN_IP_IDX(ingrs->wan_ip_idx)),
			                        portdir);
			}

			athr_nat_lut_reg_clear_status(portdir);

			athr_reg_write_lut(IG_CPU_REQ_REG, (IG_REQ_ENABLE(1) |
							IG_LUT_CMD(cmd & CMD_MASK)),
                                                        portdir);

                        ret = athrs_nat_hwtbl_ops_status(cmd,portdir);
	  		info[0] = athr_reg_read_lut(IG_INFO_DW0_REG, portdir);
	  		info[1] = athr_reg_read_lut(IG_INFO_DW1_REG, portdir);
	  		ingrs->info.lan_ip_addr = info[0];
	  		ingrs->info.lan_port = (info[1] & 0xffff);
		}
   	}

   //spin_unlock_irqrestore(&athrs_nat_op_lock, flags);
   spin_unlock(&athrs_nat_op_lock);

   return ret;
}

void athrs_hwtbl_set_wan_ip_address(u32 ipaddr)
{
        athr_reg_write_global_ip_address(ipaddr);
}
