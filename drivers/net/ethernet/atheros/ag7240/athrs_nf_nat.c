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

#include <linux/autoconf.h>
#include <linux/types.h>
#include <linux/in.h>
#include <linux/ip.h>
#include <net/ip.h>
#include <linux/tcp.h>
#include <linux/icmp.h>
#include <linux/skbuff.h>
#include <linux/if_arp.h>
#include <linux/notifier.h>
#include <linux/inetdevice.h>
#include <linux/netdevice.h>
#include <linux/jhash.h>
#include <linux/netfilter/nf_conntrack_common.h>
#include <net/netfilter/nf_conntrack.h>


#include "athrs_mac.h"
#include "athrs_hw_nat.h"
#include "athrs_nf_nat.h"

void athr_nat_init(void);
void athr_nat_down(void);
extern spinlock_t athrs_nat_op_lock;

static int nat_on = 0;
static int nat_wan_addr_set;
static int nat_ingress_session_count;
static int nat_egress_session_count;
static unsigned long ath_hw_nat_ingress_pkt_count;
static unsigned long ath_hw_nat_egress_pkt_count;
static int flag = 0xffffffff;
extern struct net init_net;
static struct net *net_hwnat = &init_net;
static int hwnat_proc_entry = 0;

static int hw_nat_seq_open(struct inode *inode, struct file *file);

static const struct file_operations hw_nat_seq_fops = {
         .owner          = THIS_MODULE,
         .open           = hw_nat_seq_open,
         .read           = seq_read,
         .llseek         = seq_lseek,
         .release        = single_release_net,
};

/*To find the free index. If none of them free return -1*/
static int ffindex(void)
{
        int index;
        if(!(index = ffs(flag)))
        return -EINVAL;
        flag ^= (1U << (index-1));
        return (index-1);
}

void *gct_addr[32];
int athrs_nat_add_del_ops_frag(uint32_t key[], uint32_t info[], int , int );
int athrs_nat_lkp_frag(uint32_t , uint16_t , uint32_t *, int , int );

static unsigned long athr_get_ingress_pkt_count(void)
{
        return ath_hw_nat_ingress_pkt_count;
}

static uint32_t athr_get_wan_ip_addr(void)
{
        return athrs_wan_if_ip_addr;
}

static unsigned long athr_get_egress_pkt_count(void)
{
        return ath_hw_nat_egress_pkt_count;
}

int athr_ct_add_entry(uint32_t ip, uint16_t port, void *ct_addr, int dir)
{
	uint32_t key[2];
	uint32_t info[4];
	key[0] = ip;
	key[1] = port;
	if(dir == INGRESS) {
		info[0] = (uint32_t)ct_addr;
	} else {
	        int key = ffindex();
        	if(-EINVAL == key)
                	return -EINVAL;
		gct_addr[key] = ct_addr;
		info[0] = (uint32_t)key;
	}
#ifdef CONFIG_ATHRS_HW_NAT_DEBUG
	printk("athr_ct_add_entry: ip:%x, port=%u, ct_addr = %p dir = %d\n", ip, port, ct_addr, dir);
#endif
	return athrs_nat_add_del_ops_frag(key, info, dir, CMD_INS);
}

int athr_ct_lkp_entry(uint32_t ip, uint16_t port, void **ct_addr, int dir)
{
	int ret;
	uint32_t index = 100;
	if(dir == INGRESS) {
		ret = athrs_nat_lkp_frag(ip, port, (uint32_t *)ct_addr, dir, CMD_LKP);
	} else
	{
		ret = athrs_nat_lkp_frag(ip, port, &index, dir, CMD_LKP);
		if(index < 32) {
			*ct_addr = (void *)gct_addr[index];
//			printk("gct_addr[%d] = %x\n",index, (void *)gct_addr[index]);
		}
	}
#ifdef CONFIG_ATHRS_HW_NAT_DEBUG
	printk("athr_ct_lkp_entry: ip = %x, port = %u, ct_addr = %p dir = %d\n", ip, port, *ct_addr, dir);
#endif
	return ret;
}

int athr_ct_del_entry(uint32_t ip, uint16_t port, int dir)
{
	uint32_t key[2];
	key[0] = ip;
	key[1] = port;
#ifdef CONFIG_ATHRS_HW_NAT_DEBUG
	printk("athr_ct_del_entry: ip:%x, port=%u \n", ip, port);
#endif
	return athrs_nat_add_del_ops_frag(key, NULL, dir, CMD_DEL);
}
int athr_nat_session_count_check(int dir)
{

        if (dir == EGRESS) {
                if (nat_egress_session_count < ATHR_HW_NAT_EGRESS_MAX_ENTRIES)
                        return 0;

        }
        else if (dir == INGRESS) {
                if (nat_ingress_session_count < ATHR_HW_NAT_INGRESS_MAX_ENTRIES)
                        return 0;
        }

        return -1;
}

void athr_nat_session_nos_update(int dir, int op)
{
        spin_lock(&athrs_nat_op_lock);

        if (dir == EGRESS) {
                if (op == CMD_INS && nat_egress_session_count >= 0) {
                             nat_egress_session_count ++;
                }
                else if (op == CMD_DEL && nat_egress_session_count > 0) {
                             nat_egress_session_count --;
                }
        }
        else if (dir == INGRESS) {
                if (nat_ingress_session_count >= 0 &&  op == CMD_INS) {
                        nat_ingress_session_count ++;
                }
                else if (op == CMD_DEL && nat_ingress_session_count > 0) {
                             nat_ingress_session_count --;
                }
        }

        spin_unlock(&athrs_nat_op_lock);
}

static int athr_nat_netdev_event_handler(struct notifier_block *this,
                                           unsigned long event, void *ptr)
{
        struct net_device *dev = (struct net_device *)ptr;

        if (!net_eq(dev_net(dev), &init_net))
                return NOTIFY_DONE;

        if (dev->name == NULL || strncmp(dev->name, ATH_WAN_INTERFACE, 4))
                return NOTIFY_DONE;

        switch (event) {
                case NETDEV_UP:
                        break;

                case NETDEV_DOWN:
 //                       printk("%s: interface down..disabling nat\n", __func__);
//                        athr_nat_down();
                        break;
        }
        return NOTIFY_DONE;
}

static int athr_nat_inetaddr_event_handler(struct notifier_block *this,
                                           unsigned long event, void *ptr)
{
        struct in_ifaddr *ifa = (struct in_ifaddr *)ptr;
        struct net_device *dev = ifa->ifa_dev->dev;

        if (!net_eq(dev_net(dev), &init_net))
                return NOTIFY_DONE;

        if (dev->name == NULL || strncmp(dev->name, ATH_WAN_INTERFACE, 4))
                return NOTIFY_DONE;

        switch (event) {
                case NETDEV_UP:
                       if (ifa->ifa_address && ifa->ifa_address != htonl(0xFFFFFFFF)) {
                                athrs_wan_if_ip_addr = ifa->ifa_address;
                                printk("%s: setting global ip : "NIPQUAD_FMT" in nat\n",
                                       __func__, NIPQUAD(athrs_wan_if_ip_addr));
                                athr_reg_write_global_ip_address(athrs_wan_if_ip_addr);
                                nat_wan_addr_set = 1;
                        }
                        break;

                case NETDEV_DOWN:
                        printk("%s: interface down..disabling nat\n", __func__);
                        //athr_nat_down();
                        break;
        }
        return NOTIFY_DONE;
}

void athr_set_nat_wan_mac(unsigned char *devaddr)
{
        if (memcmp(athrs_wan_if_mac_addr, devaddr, ETH_ALEN))
                memcpy(athrs_wan_if_mac_addr, devaddr, ETH_ALEN);
}

static int athrs_wan_ipaddr_to_idx(uint32_t newip)
{
	return 0;
}
static int athrs_nat_ops(uint32_t oldip, uint16_t oldport,
			 uint32_t newip, uint16_t newport,
			 u_int8_t protocol, u_int8_t maniptype,
                         int cmd)
{
	ath_egrs_t egrs;
	ath_ingrs_t ingrs;
	int ret;

        if (cmd != CMD_LKP) {
                if (oldip == 0 || ((protocol != ICMP_PROT) && (oldport == 0)) || protocol == 0) {
                        printk("%s: oldip/port/proto invalid ip/port/proto combination\n", __func__);
                        printk("%s: oldip = 0x%x, port = %d, newip = 0x%x, port = %d prot = %d\n",
                               __func__, oldip, oldport, newip, newport, protocol);
                        return -1;
                }

                if (cmd == CMD_INS) {
                    if (newip == 0 || ((protocol != ICMP_PROT) && (newport == 0))) {
                            printk("%s: newip/port invalid ip/port combination\n", __func__);
                            return -1;
                    }
                }
        }


	switch (maniptype) {
	case EGRESS:
		egrs.prot_key = protocol;
		egrs.lan_ip_addr = oldip;
		egrs.lan_port = oldport;
		egrs.info.wan_ip_idx = athrs_wan_ipaddr_to_idx(newip);
		egrs.info.wan_port = newport;
                if (cmd == CMD_INS) {
#ifdef CONFIG_ATHRS_HW_NAT_DEBUG
                        printk("Egress : adding lan ip :"NIPQUAD_FMT":%d to "NIPQUAD_FMT":%d proto:%d\n",
                               NIPQUAD(oldip), oldport, NIPQUAD(newip), newport, protocol);
#endif
                }

                if (cmd == CMD_LKP) {
                        ret = athrs_nat_hwtbl_lkup((void *)&egrs, maniptype, cmd);
                }
                else
                {
                        ret = athrs_nat_hwtbl_ops((void *)&egrs, maniptype, cmd);
                }
		break;

	case INGRESS:
		ingrs.prot_key = protocol;
		ingrs.wan_ip_idx = athrs_wan_ipaddr_to_idx(oldip);
		ingrs.wan_port = oldport;
		ingrs.info.lan_ip_addr = newip;
		ingrs.info.lan_port = newport;

                if (cmd == CMD_INS) {
#ifdef CONFIG_ATHRS_HW_NAT_DEBUG
                        printk("Ingress : adding wan ip "NIPQUAD_FMT":%d to "NIPQUAD_FMT":%d proto:%d\n",
                        NIPQUAD(oldip), oldport, NIPQUAD(newip), newport, protocol);
#endif
                }

                /* Not really needed */
		memcpy(ingrs.info.lan_mac_addr, athrs_wan_if_mac_addr, ETH_ALEN);

                if (cmd == CMD_LKP) {
                        ret = athrs_nat_hwtbl_lkup((void *)&ingrs, maniptype, cmd);
                }
                else
                {
                        ret = athrs_nat_hwtbl_ops((void *)&ingrs, maniptype, cmd);
                }

		break;

	default:
		printk("%s: invalid maniptype\n", __func__);
		return -1;
	}
	return ret;
}

static int athr_hw_nat_add_entry(uint32_t oldip, uint16_t oldport,
			    uint32_t newip, uint16_t newport,
			    u_int8_t protocol, int maniptype)
{
	int ret;

        if (athr_nat_session_count_check(maniptype) != 0) {
                printk("%s: %s nat h/w table full \n", __func__,
                       (maniptype == EGRESS ? "Egress" : "Ingress"));
        }

	ret = athrs_nat_ops(oldip, oldport, newip,
			    newport, protocol, maniptype, CMD_INS);

        if (ret == 0) {
                athr_nat_session_nos_update(maniptype, CMD_INS);
        }
#ifdef CONFIG_ATHRS_HW_NAT_DEBUG
	printk("%s: athrs hw nat add ret value : %d\n", __func__, ret);
#endif
        return ret;

}

static void athr_hw_nat_del_entry(uint32_t oldip, uint16_t oldport,
			    u_int8_t protocol, int maniptype)
{
	int ret;
#ifdef CONFIG_ATHRS_HW_NAT_DEBUG
        printk("%s: lan ip :"NIPQUAD_FMT" port:%d  proto:%d maniptype=%d \n",
                       __func__, NIPQUAD(oldip), oldport, protocol, maniptype);
#endif

	ret = athrs_nat_ops(oldip, oldport, 0,
			    0, protocol, maniptype, CMD_DEL);

        if (ret == 0) {
                athr_nat_session_nos_update(maniptype, CMD_DEL);
        }
#ifdef CONFIG_ATHRS_HW_NAT_DEBUG
	printk("%s: athrs hw nat del ret value success : %d\n", __func__, ret);
#endif

}

static int athr_hw_nat_lookup_entry(uint32_t ipaddr, uint16_t port, u_int8_t maniptype,
				     u_int8_t protocol, uint32_t *newipaddr, uint16_t *newport)
{
	ath_egrs_t egrs;
	ath_ingrs_t ingrs;
	int ret;
        /*
         * Try the lookup on egress lut
         */
	switch (maniptype) {
	case EGRESS:
		egrs.prot_key = protocol;
		egrs.lan_ip_addr = ipaddr;
		egrs.lan_port = port;
                ret = athrs_nat_hwtbl_lkup((void *)&egrs, maniptype, CMD_LKP);
		*newipaddr = egrs.lan_ip_addr;
		*newport = egrs.lan_port;
		break;

	case INGRESS:
		ingrs.prot_key = protocol;
		ingrs.wan_ip_idx = athrs_wan_ipaddr_to_idx(ipaddr);
		ingrs.wan_port = port;
                ret = athrs_nat_hwtbl_lkup((void *)&ingrs, maniptype, CMD_LKP);
		*newipaddr = ingrs.info.lan_ip_addr;
		*newport = ingrs.info.lan_port;
		break;

	default:
		printk("%s: invalid maniptype\n", __func__);
		return -1;
	}

//	printk("%s: athrs hw nat lookup ret value : %d\n", __func__, ret);
	return ret;
}


void athr_nat_process_egress_pkts(int unit, struct sk_buff *skb,
                                  athr_gmac_desc_t *ds)
{
        struct iphdr *iph;
#ifdef CONFIG_ATHRS_HW_NAT_DEBUG
	struct tcphdr *tcp;
#endif
        struct nf_conn *ct;

        if ((unit == 0) && (skb->protocol == ETH_P_IP)) {
                iph = ip_hdr(skb);
                if (iph != NULL) {
                        /*
                         * Enable NAT for tcp/udp/icmp packets
                         * if we have a entry in the h/w nat lut
                         */
                        if ((iph->protocol == IPPROTO_TCP ||
                             iph->protocol == IPPROTO_UDP  ||
                             iph->protocol == IPPROTO_ICMP))  {
                                    //tcp = (struct tcphdr *)((void *)iph + iph->ihl*4);
                                    if (skb->ath_hw_nat_fw_flags == 2) {
#ifdef CONFIG_ATHRS_HW_NAT_DEBUG
                                            printk("NATED: Egress : IP src: %X dst: %X skb->ath_hw_nat_fw_flags: %d\n",
                                                   iph->saddr, iph->daddr, skb->ath_hw_nat_fw_flags);

                                            if (iph->protocol == IPPROTO_TCP) {
                                                    tcp = (struct tcphdr *)((void *)iph + iph->ihl*4);
                                                    printk("NATED: Egress : tcp src: %d tcp dst : %d syn:%d ack:%d fin:%d skb->ath_hw_nat_fw_flags: %d\n",
                                                           tcp->source, tcp->dest, tcp->syn, tcp->ack, tcp->fin, skb->ath_hw_nat_fw_flags);
                                            }
#endif
                                            ath_hw_nat_egress_pkt_count ++;
                                            ATHR_TX_HW_NAT_ENABLE(ds);
#ifdef CONFIG_ATHRS_HW_NAT_DEBUG
                                             if (iph->protocol == IPPROTO_UDP &&
                                                 (iph->frag_off & htons(IP_MF|IP_OFFSET)))
                                                 printk("%s: src addr : 0x%x, dst addr : 0x%x\n", __func__,
                                                        iph->saddr, iph->daddr);
#endif
                                    }
                                    else {
#ifdef CONFIG_ATHRS_HW_NAT_DEBUG
                                            printk("UNNATED: Egress : IP src: %X dst: %X skb->ath_hw_nat_fw_flags: %d\n",
                                                   iph->saddr,iph->daddr, skb->ath_hw_nat_fw_flags);

                                            if (iph->protocol == IPPROTO_TCP) {
                                                    //tcp = tcp_hdr(skb);
                                                    tcp = (struct tcphdr *)((void *)iph + iph->ihl*4);
                                                    printk("UNNATED: Egress : tcp src: %d tcp dst: %d syn:%d ack:%d psh:%d fin:%d\n",
                                                           tcp->source,tcp->dest,tcp->syn,tcp->ack,tcp->psh, tcp->fin);
                                            }
#endif
                                            ct = (struct nf_conn *)skb->nfct;

                                            if(ct) {
                                                    if (test_bit(IPS_ATHR_HW_NAT_ADDED_BIT, &ct->status)) {
                                                            if(iph->frag_off) {
                                                                    ath_hw_nat_egress_pkt_count ++;
                                                                    ATHR_TX_HW_NAT_ENABLE(ds);
                                                            } else {
                                                                    printk("%s: nf nat disable check failed %d %x\n", __func__,
                                                                           skb->ath_hw_nat_fw_flags, iph->frag_off);
                                                            }
                                                    }
                                            }

                                    }
                        }
                        else
                                printk("%s: non ip protocol\n", __func__);
                }
                else
                    printk("%s: null ip header\n",__func__);
         }
}

void athr_nat_process_ingress_pkts(int unit, struct sk_buff *skb,
                                   athr_gmac_desc_t *ds)
{
        struct iphdr *iph;
#ifdef CONFIG_ATHRS_HW_NAT_DEBUG
	struct tcphdr *tcp;
#endif

        if (unit == 0) {

                skb->ath_hw_nat_fw_flags = 0;

                if (skb->protocol == htons(ETH_P_IP)) {

                        /* based on skb_reset_network_header function */

                        iph = (struct iphdr *)(skb->data);

                        if (ATHR_RX_HW_NAT_STATUS(ds)) {
                                /*
                                 * if we have done the nat'ing in the hardware,
                                 * mark the skb, so that it won't go through the
                                 * netfilter nat
                                 */
#ifdef CONFIG_ATHRS_HW_NAT_DEBUG
                                printk("NATED: Ingress : IP src:%X dst:%X\n",iph->saddr,iph->daddr);

                                if (iph->protocol == IPPROTO_TCP) {
                                        tcp = (struct tcphdr *)(skb->data + sizeof(struct iphdr));
                                        printk("NATED: Ingress : tcp src: %d tcp dst : %d syn:%d ack:%d psh:%d fin:%d\n",
                                                tcp->source,tcp->dest,tcp->syn,tcp->ack,tcp->psh,tcp->fin);
                                }

#endif
                                skb->ath_hw_nat_fw_flags = 3;
				ath_hw_nat_ingress_pkt_count ++;
                        }
                        else
                        {
                                /* Unnated packet */
                                skb->ath_hw_nat_fw_flags = 100;
#ifdef CONFIG_ATHRS_HW_NAT_DEBUG
                                printk("UNNATED: Ingress : IP src:%X dst:%X\n",iph->saddr,iph->daddr);
                                if (iph->protocol == IPPROTO_TCP) {
                                        tcp = (struct tcphdr *)(skb->data + sizeof(struct iphdr));
                                        printk("UNNATED: Ingress : tcp src: %d tcp dst : %d syn:%d ack:%d psh:%d fin:%d\n",
                                                tcp->source,tcp->dest,tcp->syn,tcp->ack,tcp->psh,tcp->fin);
                                }
#endif
                        }
                }
        }
        else
        {
                /* set it to 1 to indicate that its a lan packet */
                skb->ath_hw_nat_fw_flags = 1;
        }
}

static void athr_nat_init_and_enable(int portdir)
{
	char dir[30];
	strcpy(dir, (portdir == INGRESS ? "Ingress" : "Egress"));

#if 1
        /*
         * For now disable nat lut timer, since we use conntrack
         * timer for inactive connections
         */

	athrs_lut_ageing_timer_disable(portdir);
#ifdef _UNUSED_
        athrs_lut_set_ageing_ticks(portdir, 0xffffffff);

        athrs_lut_set_ageing_timeout(portdir, 0xfff,
                                              0x3ff,
                                              0x3ff);
#endif
#endif
	/* Enable nat */
	athrs_nat_enable(portdir);
	printk("%s: %s nat enable done\n", __func__, dir);

	/* Init nat */
	athrs_lut_init(portdir);

	printk("%s: %s lut init done\n", __func__, dir);
#ifdef _UNUSED_
	athrs_frag_lut_init(portdir);
	printk("%s: %s frag lut init done\n", __func__, dir);
#endif
}

void athr_nat_proc_remove(void)
{
        proc_net_remove(net_hwnat, "hw_nat");
}

static int hw_nat_seq_show(struct seq_file *seq, void *v)
{
        seq_printf(seq, "\n");
        seq_printf(seq, "ETH_GLOBAL_IP_ADDR0    : 0x%X\n", ath_reg_rd(ATHR_WAN_IP_ADDR_REG));
        seq_printf(seq, "ETH_LCL_MAC_ADDR_DW0   : 0x%X\n", ath_reg_rd(ETH_LCL_MAC_ADDR_DW0));
        seq_printf(seq, "ETH_LCL_MAC_ADDR_DW1   : 0x%X\n", ath_reg_rd(ETH_LCL_MAC_ADDR_DW1));
        seq_printf(seq, "ETH_DST_MAC_ADDR_DW0   : 0x%X\n", ath_reg_rd(ETH_DST_MAC_ADDR_DW0));
        seq_printf(seq, "ETH_DST_MAC_ADDR_DW1   : 0x%X\n", ath_reg_rd(ETH_DST_MAC_ADDR_DW1));
        seq_printf(seq, "\n");

        seq_printf(seq, "EG_NAT_STATUS          : 0x%X\n", ath_reg_rd(ETH_EG_NAT_STATUS));
        seq_printf(seq, "EG_NAT_CNTR            : 0x%X\n", ath_reg_rd(ETH_EG_NAT_CNTR));
        seq_printf(seq, "EG_NAT_DBG             : 0x%X\n", ath_reg_rd(ETH_EG_NAT_DBG));
        seq_printf(seq, "EG_CPU_REQ             : 0x%X\n", ath_reg_rd(ATHR_EGRESS_LUT_REG));
        seq_printf(seq, "EG_AGER_TICKS          : 0x%X\n", ath_reg_rd(EG_AGER_TICKS));
        seq_printf(seq, "\n");

        seq_printf(seq, "IG_EG_NAT_STATUS       : 0x%X\n", ath_reg_rd(ETH_IG_NAT_STATUS));
        seq_printf(seq, "IG_AGER_FIFO_REG       : 0x%X\n", ath_reg_rd(ETH_IG_AGER_FIFO_REG));
        seq_printf(seq, "IG_NAT_CNTR            : 0x%X\n", ath_reg_rd(ETH_IG_NAT_CNTR));
        seq_printf(seq, "IG_NAT_DBG             : 0x%X\n", ath_reg_rd(ETH_IG_NAT_DBG));
        seq_printf(seq, "IG_CPU_REQ             : 0x%X\n", ath_reg_rd(ATHR_INGRESS_LUT_REG));
        seq_printf(seq, "IG_AGER_TICKS          : 0x%X\n", ath_reg_rd(IG_AGER_TICKS));
        seq_printf(seq, "\n");

        return 0;
}

static int hw_nat_seq_open(struct inode *inode, struct file *file)
{
        return single_open_net(inode, file, hw_nat_seq_show);
}

static int hw_nat_proc_init(struct net *net)
{
        if (!proc_net_fops_create(net, "hw_nat", S_IRUGO, &hw_nat_seq_fops))
                return -ENOMEM;

        printk("Created a proc entry [/proc/net/hw_nat] for h/w nat \n");

        return 1;
}

void athr_nat_init(void)
{

        athr_nat_init_and_enable(EGRESS);
        athr_nat_init_and_enable(INGRESS);

        printk("%s: Setting h/w nat router mac addr \n", __func__);
        athrs_hw_nat_set_router_mac_addr(athrs_wan_if_mac_addr);

        rcu_assign_pointer(athrs_hw_nat_add_entry_hook, athr_hw_nat_add_entry);
        rcu_assign_pointer(athrs_hw_nat_del_entry_hook, athr_hw_nat_del_entry);
        rcu_assign_pointer(athrs_hw_nat_lkup_entry_hook, athr_hw_nat_lookup_entry);
        rcu_assign_pointer(athrs_hw_nat_get_ingress_pkt_count_hook, athr_get_ingress_pkt_count);
        rcu_assign_pointer(athrs_hw_nat_get_egress_pkt_count_hook, athr_get_egress_pkt_count);
        rcu_assign_pointer(athrs_hw_nat_get_wan_ip_addr_hook, athr_get_wan_ip_addr);
        synchronize_rcu();
        printk("%s: athrs h/w nat hooks initialized\n", __func__);


        athrs_nat_op_lock_init();
        nat_on = 1;

        if (hwnat_proc_entry == 0)
                hwnat_proc_entry = hw_nat_proc_init(net_hwnat);

}

void athr_nat_down(void)
{
        athrs_nat_disable(EGRESS);
        athrs_nat_disable(INGRESS);

        rcu_assign_pointer(athrs_hw_nat_add_entry_hook, NULL);
        rcu_assign_pointer(athrs_hw_nat_del_entry_hook, NULL);
        rcu_assign_pointer(athrs_hw_nat_lkup_entry_hook, NULL);
        rcu_assign_pointer(athrs_hw_nat_get_ingress_pkt_count_hook, NULL);
        rcu_assign_pointer(athrs_hw_nat_get_egress_pkt_count_hook, NULL);
        rcu_assign_pointer(athrs_hw_nat_get_wan_ip_addr_hook, NULL);
        synchronize_rcu();

        printk("%s: athrs h/w nat hooks set to NULL\n", __func__);

}

void athr_reg_net_notifcations(void)
{
        register_netdevice_notifier(&athr_nat_netdev_notifier);
        register_inetaddr_notifier(&athr_nat_inetaddr_notifier);
}

void athr_unreg_net_notifcations(void)
{
        unregister_inetaddr_notifier(&athr_nat_inetaddr_notifier);
        unregister_netdevice_notifier(&athr_nat_netdev_notifier);
}
