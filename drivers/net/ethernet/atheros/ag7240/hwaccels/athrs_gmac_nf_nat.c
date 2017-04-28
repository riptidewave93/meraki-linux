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
#include <net/netfilter/nf_nat.h>
#include <net/netfilter/nf_conntrack_l3proto.h>
#include <net/netfilter/nf_conntrack_l4proto.h>
#include <net/netfilter/nf_conntrack_helper.h>
#include <net/netfilter/nf_conntrack_core.h>

/* TIME */ 
#include <linux/time.h>
#include<asm/asm.h>



#include <linux/seq_file.h>

#include "athrs_mac.h"
#include "athrs_gmac_nf_nat.h"
#include "athrs_gmac_hw_nat.h"

#define NAT_ADD_ENTRY   0x1
#define NAT_DEL_ENTRY   0x2

#define NAT_EGRESS      EGRESS
#define NAT_INGRESS     INGRESS

#define NAT_PROTO_TCP           0x1
#define NAT_PROTO_UDP           0x2
#define NAT_PROTO_ICMP          0x3

extern spinlock_t athrs_nat_op_lock;
extern uint32_t hash_conntrack(const struct nf_conntrack_tuple *tuple);
static athr_nat_ops_t *hw_nat_api;
uint8_t athrs_wan_if_mac_addr[ETH_ALEN] = { 0, 0, 0, 0, 0, 0 };
uint32_t athrs_wan_if_ip_addr[4] = { 0, 0, 0, 0 };

#ifdef CONFIG_ATHRS_HW_NAT_DEBUG
void display_ct(const struct nf_conn *ct_print);
static void tuple_print1 (const struct nf_conntrack_tuple *tuple,char dir);
#endif 
int athrs_nat_add_del_ops(uint32_t *key, uint32_t *info, int portdir, int cmd,
				uint8_t pkt_type);
extern int athr_hwnat_print_tuple(struct seq_file *s, struct nf_conn *ct, uint8_t pkt_cnt);
extern struct net init_net;
static struct net *net_hwnat = &init_net;

static struct kmem_cache *nf_hwnat_cachep __read_mostly;
static struct hlist_nulls_head *hwnat_mod_hash;
static int    hwnat_mod_hash_vmalloc;
static int hwnat_proc_entry = 0;

static int hw_nat_seq_open(struct inode *inode, struct file *file);
static void nf_nat_seq_stop(struct seq_file *s, void *v);
static void *nf_nat_seq_next(struct seq_file *s, void *v, loff_t *pos);
static void *nf_nat_seq_start(struct seq_file *seq, loff_t *pos);
static int nf_nat_seq_show(struct seq_file *s, void *v);
static int nf_nat_seq_open(struct inode *inode, struct file *file);

static int athr_nat_netdev_event_handler(struct notifier_block *this,
                                           unsigned long event, void *ptr);

static int athr_nat_inetaddr_event_handler(struct notifier_block *this,
                                           unsigned long event, void *ptr);

static struct notifier_block athr_nat_inetaddr_notifier = {
            .notifier_call = athr_nat_inetaddr_event_handler,
};

static struct notifier_block athr_nat_netdev_notifier = {
            .notifier_call = athr_nat_netdev_event_handler,
};

struct nf_nat_iter_state {
        struct seq_net_private p;
        unsigned int bucket;
};

static const struct file_operations hw_nat_seq_fops = {
         .owner          = THIS_MODULE,
         .open           = hw_nat_seq_open,
         .read           = seq_read,
         .llseek         = seq_lseek,
         .release        = single_release_net,
};

static const struct seq_operations nf_nat_seq_ops = {
        .start = nf_nat_seq_start,
        .next  = nf_nat_seq_next,
        .stop  = nf_nat_seq_stop,
        .show  = nf_nat_seq_show
};

static const struct file_operations nf_nat_seq_fops = {
         .owner          = THIS_MODULE,
         .open           = nf_nat_seq_open,
         .read           = seq_read,
         .llseek         = seq_lseek,
         .release        = seq_release_net,
};

#ifdef CONFIG_ATHRS_HW_NAT_DEBUG
static void print_athr_tuple(athr_tuple *tup1, const char *dir)
{
	 printk("%s :: TUPLE: oldip 0x%x newip 0x%x oldport 0x%x newport 0x%x \n", dir, tup1->oldip, tup1->newip, tup1->oldport, tup1->newport);
}
static void tuple_print (const struct nf_conntrack_tuple *tuple, const char *func)
{
        printk("%s: Src Port = %u, Src IP = %x Dst: Port = %u, IP = %x Protocol = 0x%x\n", \
                func, tuple->src.u.all, tuple->src.u3.ip, \
                tuple->dst.u.all, tuple->dst.u3.ip, tuple->dst.protonum);
}

static void tuple_print1 (const struct nf_conntrack_tuple *tuple,char dir)
{
        char dir1[30];
        strcpy(dir1, (dir ? "REPLEY" : "ORG"));
        printk(" DIR =%s Src Port = %u, Src IP = %x Dst: Port = %u, IP = %x Protocol = 0x%x\n", \
        dir1,tuple->src.u.all, tuple->src.u3.ip, \
        tuple->dst.u.all, tuple->dst.u3.ip, tuple->dst.protonum);
}


void display_ct(const struct nf_conn *ct_print)
{
	printk("ct_print->status = 0x%X  \n ", ct_print->status);
	tuple_print1(&ct_print->tuplehash[IP_CT_DIR_ORIGINAL].tuple,0);
	tuple_print1(&ct_print->tuplehash[IP_CT_DIR_REPLY].tuple,1);
}
#endif
static void athr_get_wan_ip_addr(uint32_t *wan_ip, uint8_t index)
{
        if (index >= 0 && index <= 3)
                *wan_ip = athrs_wan_if_ip_addr[index];
}

static int athr_nat_netdev_event_handler(struct notifier_block *this,
                                           unsigned long event, void *ptr)
{
        struct net_device *dev = (struct net_device *)ptr;
        int wan_index = -1;

        if (!net_eq(dev_net(dev), &init_net))
                return NOTIFY_DONE;

        if (dev->name == NULL || strncmp(dev->name, ATH_WAN_INTERFACE, 4))
                return NOTIFY_DONE;

        sscanf(dev->name,ATH_WAN_INTERFACE":%d", &wan_index);

	if (wan_index == -1)
                wan_index = 0;
        else
        {
            if (wan_index > 3)
                    return NOTIFY_DONE;
        }
        switch (event) {
                case NETDEV_UP:
                                hw_nat_api->set_wan_ip_addr(athrs_wan_if_ip_addr[wan_index], wan_index);
                        break;

                case NETDEV_DOWN:
 //                       printk("%s: interface down..disabling nat\n", __func__);
//                        hw_nat_api->nat_down(); 
                        break;
        }
        return NOTIFY_DONE;
}

static void athr_set_nat_wan_ip(uint32_t wan_ip, int wan_index)
{
        athr_reg_write_global_ip_address(wan_ip, wan_index);
}

static int athr_nat_inetaddr_event_handler(struct notifier_block *this,
                                           unsigned long event, void *ptr)
{
        struct in_ifaddr *ifa = (struct in_ifaddr *)ptr;
        struct net_device *dev = ifa->ifa_dev->dev;
        int wan_index = -1;

        if (!net_eq(dev_net(dev), &init_net))
                return NOTIFY_DONE;

        if (dev->name == NULL || strncmp(dev->name, ATH_WAN_INTERFACE, 4))
                return NOTIFY_DONE;

        //sscanf(dev->name,"%*[^1-9]%d", &wan_index);
        sscanf(ifa->ifa_label, ATH_WAN_INTERFACE":%d", &wan_index);

#ifdef CONFIG_ATHRS_HW_NAT_DEBUG
        printk("%s: wan_ifname : %s ifa_label : %s wan_index = %d \n", __func__, dev->name, ifa->ifa_label, wan_index);
#endif

        if (wan_index == -1)
                wan_index = 0;
        else
        {
            if (wan_index > 3)
                    return NOTIFY_DONE;
        }

        switch (event) {
                case NETDEV_UP:
                       if (ifa->ifa_address && ifa->ifa_address != htonl(0xFFFFFFFF)) {
                                athrs_wan_if_ip_addr[wan_index] = ifa->ifa_address;
                                printk("%s: setting global ip : "NIPQUAD_FMT" index : %d in nat reg\n",
                                       __func__, NIPQUAD(athrs_wan_if_ip_addr[wan_index]), wan_index);
                                hw_nat_api->set_wan_ip_addr(athrs_wan_if_ip_addr[wan_index], wan_index);
                        }
                        break;

                case NETDEV_DOWN:
                        printk("%s: interface down..disabling nat\n", __func__);
                        //hw_nat_api->nat_down();
                        break;
        }
        return NOTIFY_DONE;
}


static void athr_set_nat_wan_mac(unsigned char *devaddr)
{
        if (memcmp(athrs_wan_if_mac_addr, devaddr, ETH_ALEN))
                memcpy(athrs_wan_if_mac_addr, devaddr, ETH_ALEN);
}

static void athr_nat_init_and_enable(int portdir)
{
	char dir[30];
	strcpy(dir, (portdir == INGRESS ? "Ingress" : "Egress"));

        /* Disable Aging timer */ 
  	athrs_lut_ageing_timer_disable(portdir);

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
        proc_net_remove(net_hwnat, "nf_nat");
        hwnat_proc_entry = 0;
}

static int hw_nat_seq_show(struct seq_file *seq, void *v)
{
	int cnt=0;
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
	seq_printf(seq, "IG_ACL_CNT_GRP         :\n");
	for(cnt=1;cnt<IG_ACL_CNT_MAX;cnt++)	
	{ 
		seq_printf(seq,"GRP%02d :0x%08X   ",cnt,ath_reg_rd(ATHR_IG_ACL_MEM_CTRL+ (4*cnt)));
		if(cnt % 4 == 0)
        		seq_printf(seq, "\n");
	} 
	seq_printf(seq, "EG_ACL_CNT_GRP         :\n");
	for(;cnt<EG_ACL_CNT_MAX;cnt++)	
	{ 
		seq_printf(seq,"GRP%02d :0x%08X   ",(cnt-16),ath_reg_rd(ATHR_IG_ACL_MEM_CTRL+ (4*cnt)));
		if(cnt % 4 == 0)
        		seq_printf(seq, "\n");
	} 
        seq_printf(seq, "\n");

        return 0;
}

static struct hlist_nulls_node *nf_nat_get_first(struct seq_file *seq)
{
        struct net *net = seq_file_net(seq);
        struct nf_nat_iter_state *st = seq->private;
        struct hlist_nulls_node *n;

        for (st->bucket = 0;
             st->bucket < nf_conntrack_htable_size;
             st->bucket++) {
                n = rcu_dereference(net->ct.hash[st->bucket].first);
                if (!is_a_nulls(n))
                        return n;
        }
        return NULL;
}

static struct hlist_nulls_node *nf_nat_get_next(struct seq_file *seq,
                                      struct hlist_nulls_node *head)
{
        struct net *net = seq_file_net(seq);
        struct nf_nat_iter_state *st = seq->private;

        head = rcu_dereference(head->next);
        while (is_a_nulls(head)) {
                if (likely(get_nulls_value(head) == st->bucket)) {
                        if (++st->bucket >= nf_conntrack_htable_size)
                                return NULL;
                }
                head = rcu_dereference(net->ct.hash[st->bucket].first);
        }
        return head;
}

static struct hlist_nulls_node *nf_nat_get_idx(struct seq_file *seq, loff_t pos)
{
        struct hlist_nulls_node *head = nf_nat_get_first(seq);

        if (head)
                while (pos && (head = nf_nat_get_next(seq, head)))
                        pos--;
        return pos ? NULL : head;
}


static void *nf_nat_seq_start(struct seq_file *seq, loff_t *pos)
        __acquires(RCU)
{
        rcu_read_lock();
        return nf_nat_get_idx(seq, *pos);
}

static void *nf_nat_seq_next(struct seq_file *s, void *v, loff_t *pos)
{
        (*pos)++;
        return nf_nat_get_next(s, v);
}

static void nf_nat_seq_stop(struct seq_file *s, void *v)
        __releases(RCU)
{
        rcu_read_unlock();
}

static int nf_nat_seq_show(struct seq_file *s, void *v)
{
        struct nf_conntrack_tuple_hash *hash = v;
        struct nf_conn *ct = nf_ct_tuplehash_to_ctrack(hash);
        const struct nf_conntrack_l3proto *l3proto;
        const struct nf_conntrack_l4proto *l4proto;
        int ret = 0;

        NF_CT_ASSERT(ct);
        if (unlikely(!atomic_inc_not_zero(&ct->ct_general.use)))
                return 0;


        /* we only want to print DIR_ORIGINAL */
        if (NF_CT_DIRECTION(hash))
                goto release;
        if (nf_ct_l3num(ct) != AF_INET)
                goto release;

        l3proto = __nf_ct_l3proto_find(nf_ct_l3num(ct));
        NF_CT_ASSERT(l3proto);
        l4proto = __nf_ct_l4proto_find(nf_ct_l3num(ct), nf_ct_protonum(ct));
        NF_CT_ASSERT(l4proto);

        ret = -ENOSPC;
        if (seq_printf(s, "%-8s %u %ld ",
                      l4proto->name, nf_ct_protonum(ct),
                      timer_pending(&ct->timeout)
                      ? (long)(ct->timeout.expires - jiffies)/HZ : 0) != 0)
                goto release;
        if (l4proto->print_conntrack && l4proto->print_conntrack(s, ct))
                goto release;

        if (print_tuple(s, &ct->tuplehash[IP_CT_DIR_ORIGINAL].tuple,
                        l3proto, l4proto))
                goto release;

        if (!(test_bit(IPS_SEEN_REPLY_BIT, &ct->status)))
                if (seq_printf(s, "[UNREPLIED] "))
                        goto release;

        if (print_tuple(s, &ct->tuplehash[IP_CT_DIR_REPLY].tuple,
                        l3proto, l4proto))
                goto release;

        if (test_bit(IPS_ASSURED_BIT, &ct->status))
                if (seq_printf(s, "[ASSURED] "))
                        goto release;

        if (seq_printf(s, "use=%u", atomic_read(&ct->ct_general.use)))
                goto release;

        if (test_bit(IPS_ATHR_HW_NAT_ADDED_BIT, &ct->status)) {

            if (athr_hwnat_print_tuple(s, ct, 0))
                goto release;
        }

        if (seq_printf(s, "\n"))
                goto release;


        ret = 0;
release:
        nf_ct_put(ct);
        return ret;
}

static int hw_nat_seq_open(struct inode *inode, struct file *file)
{
        return single_open_net(inode, file, hw_nat_seq_show);
}

static int nf_nat_seq_open(struct inode *inode, struct file *file)
{
        return seq_open_net(inode, file, &nf_nat_seq_ops,
                            sizeof(struct nf_nat_iter_state));
}

static int athr_nat_proc_init(struct net *net)
{
        if (!proc_net_fops_create(net, "nf_nat", S_IRUGO, &nf_nat_seq_fops))
                return -ENOMEM;

        printk("Created a proc entry [/proc/net/nf_nat] for h/w nat \n");
        if (!proc_net_fops_create(net, "hw_nat", S_IRUGO, &hw_nat_seq_fops))
                return -ENOMEM;

        printk("Created a proc entry [/proc/net/hw_nat] for h/w nat \n");


        return 1;
}

static struct nf_conntrack_tuple_hash *
__nf_conntrack_find_hwnat(struct net *net, const struct nf_conntrack_tuple *tuple)
{
	struct nf_conntrack_tuple_hash *h;
	struct hlist_nulls_node *n;
	unsigned int hash = hash_conntrack(tuple);
#ifdef CONFIG_ATHRS_HW_NAT_DEBUG
        int cnt = 0, i = 0;
#endif

	/* Disable BHs the entire time since we normally need to disable them
	 * at least once for the stats anyway.
	 */
	local_bh_disable();
begin:
	hlist_nulls_for_each_entry_rcu(h, n, &hwnat_mod_hash[hash], hnnode) {

		if (nf_ct_tuple_equal(tuple, &h->tuple)) {
			local_bh_enable();
			return h;
		}
#ifdef CONFIG_ATHRS_HW_NAT_DEBUG
                if (cnt > 60) {
                        if (i == 0) {
                            dump_stack();
                            //tuple_print(tuple, __func__);
                            i++;
                        }
                }
                else
                        cnt++;
#endif
	}
	/*
	 * if the nulls value we got at the end of this lookup is
	 * not the expected one, we must restart lookup.
	 * We probably met an item that was moved to another chain.
	 */
	if (get_nulls_value(n) != hash) {
		goto begin;
	}
	local_bh_enable();

	return NULL;
}

static void corrupt_tuple(struct net *net, struct nf_conn *ct,
                   enum ip_conntrack_dir dir,
                   uint32_t ip, uint16_t port,
                   int protocol)
{
	uint32_t repl_hash;
        struct nf_ct_hwnat *nf_hwnat;

	hlist_nulls_del_rcu(&ct->tuplehash[dir].hnnode);

        nf_hwnat = (struct nf_ct_hwnat *)ct->hwnat_priv;
        nf_hwnat->tuplehash = ct->tuplehash[dir];

        repl_hash = hash_conntrack(&nf_hwnat->tuplehash.tuple);
        hlist_nulls_add_head_rcu(&nf_hwnat->tuplehash.hnnode,
                                 &hwnat_mod_hash[repl_hash]);

	if (protocol != NAT_PROTO_ICMP)
		ct->tuplehash[dir].tuple.dst.u.all = port;

	if (dir == IP_CT_DIR_REPLY)
		ct->tuplehash[dir].tuple.dst.u3.ip = ip;
	/*
	* After changing the tuple we've rehash the
	* tuple, so that it sits in the correct hash
	* bucket list
	*/
	repl_hash = hash_conntrack(&ct->tuplehash[dir].tuple);
	hlist_nulls_add_head_rcu(&ct->tuplehash[dir].hnnode,
	                         &net->ct.hash[repl_hash]);

}

static void revert_tuple(struct net *net, struct nf_conn *ct,
                  enum ip_conntrack_dir dir,
                  uint32_t ip, uint16_t port)
{

	struct nf_conntrack_tuple_hash *h;
	uint32_t repl_hash;
        struct nf_ct_hwnat *nf_hwnat;


        nf_hwnat = (struct nf_ct_hwnat *)ct->hwnat_priv;
	h = __nf_conntrack_find_hwnat(net,
                                      &nf_hwnat->tuplehash.tuple);
	if (h) {
	    hlist_nulls_del_rcu(&nf_hwnat->tuplehash.hnnode);
	}

	hlist_nulls_del_rcu(&ct->tuplehash[dir].hnnode);
	ct->tuplehash[dir].tuple.dst.u3.ip = ip;
	ct->tuplehash[dir].tuple.dst.u.all = port;
	/*
	* After changing the tuple we've rehash the
	* tuple, so that it sits in the correct hash
	* bucket list
	*/
	repl_hash = hash_conntrack(&ct->tuplehash[dir].tuple);
	hlist_nulls_add_head_rcu(&ct->tuplehash[dir].hnnode,
				 &net->ct.hash[repl_hash]);
}

static void athr_alloc_hwnat_priv(struct nf_conn *ct)
{
        struct nf_ct_hwnat *nf_hwnat;

        nf_hwnat = kmem_cache_alloc(nf_hwnat_cachep, GFP_ATOMIC);
        if (nf_hwnat == NULL) {
                printk("nf_hwnat_alloc: Can't alloc ct tuplehash.\n");
                    //atomic_dec(&net->ct.count);
                return;
        }
        ct->hwnat_priv = nf_hwnat;
}

static void athr_swap_tuples(athr_tuple *tup1, athr_tuple *tup2, uint8_t dir)
{
        tup2->oldip = tup1->newip;
        tup2->newip = tup1->oldip;
        tup2->oldport = tup1->newport;
        tup2->newport = tup1->oldport;
        tup2->maniptype = dir;
}

static void athr_modify_tb(struct nf_conn *ct, int op, athr_tuple tup1)
{
	int ret = -1;
	uint32_t ipaddr;
	uint16_t port;
        struct nf_ct_hwnat *nf_hwnat;
        uint8_t dir = tup1.maniptype;
        uint8_t protocol = tup1.protocol;
        athr_tuple tup2 = tup1;

	if (op == NAT_ADD_ENTRY) {

		if ((dir == NAT_EGRESS) ||
                    (protocol == NAT_PROTO_TCP) ||
                    (protocol == NAT_PROTO_ICMP) ||
                    (test_bit(IPS_ATHR_HW_NAT_IS_UDP_BIT, &ct->status) &&
                    (dir == NAT_INGRESS))) {

                        nf_hwnat = (struct nf_ct_hwnat *)ct->hwnat_priv;
                        nf_hwnat->tuple[0].src.u3.ip = tup1.oldip;
                        nf_hwnat->tuple[0].dst.u3.ip = tup1.newip;
                        nf_hwnat->tuple[0].src.u.all = tup1.oldport;
                        nf_hwnat->tuple[0].dst.u.all = tup1.newport;
                        nf_hwnat->tuple[0].dst.protonum = protocol;
                        nf_hwnat->tuple[0].dst.dir = NAT_EGRESS;

                        if (tup2.maniptype == NAT_INGRESS)
                                tup2.maniptype = NAT_EGRESS;

			ret = hw_nat_api->lkp_entry(tup2, &ipaddr, &port);

			if (ret) {
				ret = 0;
                                ret |= hw_nat_api->add_entry(tup2);
#ifdef TEST 
				printk(" EGRESS ADD ret = %x addcnt = %d  \n",ret,++egress_addcnt);
#endif 

			}
		}

		if (((dir == NAT_INGRESS) ||
                     (protocol == NAT_PROTO_TCP) ||
                     (protocol == NAT_PROTO_ICMP) ||
                     (test_bit(IPS_ATHR_HW_NAT_IS_UDP_BIT, &ct->status) &&
                     (dir == NAT_EGRESS))) &&
                     (!test_bit(IPS_ATHR_HW_NAT_IS_ONLY_EGRESS_BIT, &ct->status))) {

                        nf_hwnat = (struct nf_ct_hwnat *)ct->hwnat_priv;
			nf_hwnat->tuple[1].src.u3.ip = tup1.newip;
                        nf_hwnat->tuple[1].dst.u3.ip = tup1.oldip;
                        nf_hwnat->tuple[1].src.u.all = tup1.newport;
                        nf_hwnat->tuple[1].dst.u.all = tup1.oldport;
                        nf_hwnat->tuple[1].dst.protonum = tup1.protocol;
                        nf_hwnat->tuple[1].dst.dir = NAT_INGRESS;

                        athr_swap_tuples(&tup1, &tup2, NAT_INGRESS);

			ret = hw_nat_api->lkp_entry(tup2, &ipaddr, &port);

			if (ret) {
				ret = 0;
                                ret |= hw_nat_api->add_entry(tup2);
#ifdef TEST 
				printk(" ING ADD ret = %x addcnt =%d  \n",ret,++ingress_addcnt);
#endif 
			}

		}

		if (ret == 0) {
			set_bit(IPS_ATHR_HW_NAT_ADDED_BIT, &ct->status);
			return;
		}
                else
		{
			printk("Entry add failed:%d\n",ret);
                        kmem_cache_free(nf_hwnat_cachep, ct->hwnat_priv);
                        ct->hwnat_priv = NULL;
                        return;
		}
	}
	else if (op == NAT_DEL_ENTRY) {
                struct nf_ct_hwnat *nf_hwnat = (struct nf_ct_hwnat *)ct->hwnat_priv;
		struct nf_conntrack_tuple temp =
				nf_hwnat->tuplehash.tuple;

		struct nf_conntrack_tuple_hash *h;
                struct net *net = nf_ct_net(ct);
		enum ip_conntrack_dir c_dir;

                if (nf_hwnat->newport) {
                        tup1.newport = nf_hwnat->newport;
                        tup1.newip = nf_hwnat->newip;
                }

		if (dir == NAT_INGRESS)
     			c_dir = IP_CT_DIR_ORIGINAL;
		else
     			c_dir = IP_CT_DIR_REPLY;

// 		printk("RESTORE: dir %d newip 0x%x newport 0x%x\n", dir, nf_hwnat->newip, nf_hwnat->newport);
 		revert_tuple(net, ct, c_dir, nf_hwnat->newip, nf_hwnat->newport);
 		clear_bit(IPS_ATHR_HW_NAT_ADDED_BIT, &ct->status);

		h = __nf_conntrack_find_hwnat(net, &temp);
		if (!h) {
			if ((dir == NAT_EGRESS) ||
                            (protocol == NAT_PROTO_TCP) ||
                            (protocol == NAT_PROTO_ICMP) ||
                            test_bit(IPS_ATHR_HW_NAT_IS_UDP_BIT, &ct->status)) {

              //              printk(" 111 Direction = %d  maintype =%d \n  ",dir,tup1.maniptype);
                              tup1.maniptype=NAT_EGRESS;
#ifdef TEST 
			      printk(" EGRESS DEL   addcnd = %d delcnt = %d  \n",egress_addcnt,++egress_delcnt);
#endif 
        		    hw_nat_api->del_entry(tup1);
			}

			if ((dir == NAT_INGRESS) ||
                            (protocol == NAT_PROTO_TCP) ||
                            (protocol == NAT_PROTO_ICMP) ||
                            test_bit(IPS_ATHR_HW_NAT_IS_UDP_BIT, &ct->status)) {

				if(test_bit(IPS_ATHR_HW_NAT_IS_UDP_BIT, &ct->status))
					clear_bit(IPS_ATHR_HW_NAT_IS_UDP_BIT, &ct->status);

                                athr_swap_tuples(&tup1, &tup2, NAT_INGRESS);
                             
                        //      printk(" 222  Direction = %d  maintype =%d \n ",dir,tup2.maniptype);

			 //    printk(" saved ip 0x%x saved port 0x%x\n", nf_hwnat->tuplehash.tuple.dst.u3.ip, nf_hwnat->tuplehash.tuple.dst.u.all);
			      tup2.oldip = nf_hwnat->tuplehash.tuple.dst.u3.ip;
		            if (protocol != NAT_PROTO_ICMP)
				    tup2.oldport = nf_hwnat->tuplehash.tuple.dst.u.all;
			  //	printk(" INGRESS DEL \n");
#ifdef TEST 
				printk(" ING DEL addcnt= %d delcnt= %d  \n",ingress_addcnt,++ingress_delcnt);
#endif 

        			hw_nat_api->del_entry(tup2);
			}

		}

	}
}

static void athr_mangle_conntrack_snat(struct nf_conn *ct, int op,
				int protocol, uint8_t dir)
{
        struct net *net = nf_ct_net(ct);
        struct nf_ct_hwnat *nf_hwnat;
        athr_tuple tup1;

        if (dir == NAT_EGRESS) {

                if (ct->hwnat_priv == NULL)
                    athr_alloc_hwnat_priv(ct);

                nf_hwnat = (struct nf_ct_hwnat *)ct->hwnat_priv;
                /* Fetch egress tuple details */
                tup1.oldip = ct->tuplehash[IP_CT_DIR_ORIGINAL].tuple.src.u3.ip;
                tup1.oldport = ct->tuplehash[IP_CT_DIR_ORIGINAL].tuple.src.u.all;
                tup1.dstip = ct->tuplehash[IP_CT_DIR_REPLY].tuple.src.u3.ip;
                tup1.dstport = ct->tuplehash[IP_CT_DIR_REPLY].tuple.src.u.all;
                tup1.newip = ct->tuplehash[IP_CT_DIR_REPLY].tuple.dst.u3.ip;
                tup1.protocol = protocol;
                tup1.maniptype = dir;
                nf_hwnat->dstip = tup1.dstip;
                nf_hwnat->dstport = tup1.dstport;


                if (protocol == NAT_PROTO_ICMP) {
                        if (nf_hwnat->newport == 0xffff)
                                tup1.newport = ct->tuplehash[IP_CT_DIR_REPLY].tuple.dst.u.all;
                        else
                                tup1.newport = tup1.oldport;
                }
                else
                {
                        tup1.newport = ct->tuplehash[IP_CT_DIR_REPLY].tuple.dst.u.all;
                }

                /*
                 * Remove the reply tuple from old hash list
                 * since we are going to change the tuple details
                 */

                if (op == NAT_ADD_ENTRY) {
                        if ((protocol != NAT_PROTO_UDP) ||
                           ((protocol == NAT_PROTO_UDP) &&
                           test_bit(IPS_ATHR_HW_NAT_IS_UDP_BIT, &ct->status))) {

                                if (tup1.oldport != tup1.newport) { /* port range */
                                        nf_hwnat->newport = tup1.newport;
                                        nf_hwnat->newip = tup1.newip;
                                }
                                else
                                {
                                        nf_hwnat->newport = 0;
                                        nf_hwnat->newip = 0;
                                }


                                corrupt_tuple(net, ct, IP_CT_DIR_REPLY,
                                              tup1.oldip, tup1.oldport, protocol);

                        }
                }
        }
        else
        {
                printk("ERROR:Unknow direction %s:%d\n",__func__,__LINE__);
                return;
        }

	athr_modify_tb(ct, op, tup1);
}

static void athr_mangle_conntrack_dnat(struct nf_conn *ct, int op,int protocol, uint8_t dir) 
{
        athr_tuple tup1;
	struct net *net = nf_ct_net(ct);
        struct nf_ct_hwnat *nf_hwnat;
	int ret;
	uint32_t ipaddr;
	uint16_t port;

        if (dir == NAT_INGRESS) {

                if (ct->hwnat_priv == NULL)
                    athr_alloc_hwnat_priv(ct);

                nf_hwnat = (struct nf_ct_hwnat *)ct->hwnat_priv;
		/* Fetch egress tuple details */
		tup1.oldip = ct->tuplehash[IP_CT_DIR_REPLY].tuple.src.u3.ip;
		tup1.oldport  = ct->tuplehash[IP_CT_DIR_REPLY].tuple.src.u.all;
                tup1.dstip = ct->tuplehash[IP_CT_DIR_ORIGINAL].tuple.src.u3.ip;
                tup1.dstport = ct->tuplehash[IP_CT_DIR_ORIGINAL].tuple.src.u.all;
		tup1.newip = ct->tuplehash[IP_CT_DIR_ORIGINAL].tuple.dst.u3.ip;
                tup1.protocol = protocol;
                tup1.maniptype = dir;
                nf_hwnat->dstip = tup1.dstip;
                nf_hwnat->dstport = tup1.dstport;


		if (protocol == NAT_PROTO_ICMP) {
			tup1.newport = tup1.oldport;
		}
		else
		{
			tup1.newport = ct->tuplehash[IP_CT_DIR_ORIGINAL].tuple.dst.u.all;
		}

#if  1 
		nf_hwnat->newip = tup1.newip;
                nf_hwnat->newport = tup1.newport;
//  	        printk(" WHILE ADDING  H/W new IP and Port = %X %d \n ", tup1.newip,tup1.newport);  
#endif 


		if (op == NAT_ADD_ENTRY) {
                        ret = hw_nat_api->lkp_entry(tup1, &ipaddr, &port);
                        if ((ret != 0) ||
                            test_bit(IPS_ATHR_SW_NAT_SKIPPED_BIT, &ct->status)) {

                                corrupt_tuple(net, ct, IP_CT_DIR_ORIGINAL,
                                              tup1.oldip, tup1.oldport, protocol);

                                if (test_bit(IPS_ATHR_SW_NAT_SKIPPED_BIT, &ct->status)) {
                                        clear_bit(IPS_ATHR_SW_NAT_SKIPPED_BIT, &ct->status);
                                        return ;
                                }
                        }
		}
	}
	else
        {
		printk("ERROR:Unknow direction %s:%d\n",__func__,__LINE__);
		return;
	}

	athr_modify_tb(ct, op, tup1);
}

static void athr_nat_add_or_del_tuple(struct nf_conn *ct, int op)
{
	uint8_t protocol;
	uint8_t maniptype = 0;
	unsigned long stat1, stat2, algstat;
        struct nf_conn_help *help = NULL;

	protocol = nf_ct_protonum(ct);
	if (op == NAT_ADD_ENTRY) {
		if (test_bit(IPS_ATHR_HW_NAT_ADDED_BIT, &ct->status)) {
			if(protocol != IPPROTO_UDP)
				return;
		}
		if ((protocol != IPPROTO_ICMP) &&
			(protocol != IPPROTO_TCP ) && (protocol != IPPROTO_UDP)) {
			return;
		}
	}
        else if (op == NAT_DEL_ENTRY &&
		!test_bit(IPS_ATHR_HW_NAT_ADDED_BIT, &ct->status)) {
		return;
	}

	stat1 = (ct->status & IPS_NAT_MASK);
	stat2 = (ct->status & IPS_NAT_DONE_MASK);

        help = nfct_help(ct);
        algstat = help? help->helper ? 1: 0:0;

	/*
	 * Remove the h/w entry only if the ct is natted and not a alg pkt
	 */
	if (stat1 && stat2 && !algstat &&
	    (nf_ct_l3num(ct) == AF_INET)) {

		protocol = nf_ct_protonum(ct);

		if (protocol == IPPROTO_TCP) {
			protocol = NAT_PROTO_TCP;
		}
		else if (protocol == IPPROTO_UDP) {
			protocol = NAT_PROTO_UDP;
		}
		else if (protocol == IPPROTO_ICMP) {
			protocol = NAT_PROTO_ICMP;
		}
		else
		{
			printk("%s: invalid protocol id %d\n", __func__, protocol);
			return;
		}


		if (test_bit(IPS_SRC_NAT_BIT, &ct->status) &&
			test_bit(IPS_SRC_NAT_DONE_BIT, &ct->status)) {
			maniptype = NAT_EGRESS;
			athr_mangle_conntrack_snat(ct,op,protocol,maniptype);
		}
                else if (test_bit(IPS_DST_NAT_BIT, &ct->status) &&
		         test_bit(IPS_DST_NAT_DONE_BIT, &ct->status)) {
                             maniptype = NAT_INGRESS;


                             athr_mangle_conntrack_dnat(ct,op,protocol,maniptype);
		}

 		if (unlikely(maniptype != NAT_EGRESS && maniptype != NAT_INGRESS))
		{

			printk("%s: invalid maniptype \n", __func__);
			return;
		}

        }
#ifdef CONFIG_ATHRS_HW_NAT_DEBUG
    else {
		printk("Cond failed in add_del stat1=%lX stat2=%lX algstat=%lX nf_ct_l3num(ct)=%u\n\n", \
			 stat1, stat2, algstat, nf_ct_l3num(ct));
	}
#endif
}

#if 1
static struct nf_conn *athr_nf_find_get(struct net *net, struct nf_conntrack_tuple *tuple,
                                        __u32 ath_hw_nat_flag, struct nf_conntrack_tuple_hash **th)
{
    uint32_t temp;
    struct nf_conntrack_tuple_hash *h = *th;
    uint8_t i;

    h = nf_conntrack_find_get(net, tuple);
    if (!h) {
        if (ath_hw_nat_flag == 3) {
            temp = tuple->dst.u3.ip;
            h = NULL;

            for (i=0; i < 4; i++) {
                    athr_get_wan_ip_addr(&tuple->dst.u3.ip, i);

                    if (tuple->dst.u3.ip != 0) {

                            h = nf_conntrack_find_get(net, tuple);
                            if (h) {
                                    *th = h;
                                    return nf_ct_tuplehash_to_ctrack(h);
                            }
                    }
            }

            if (!h) {
                    tuple->dst.u3.ip = temp;
                    h = *th;
            }

        }
        *th = h;
        return NULL;
    } else {
        *th = h;
        return nf_ct_tuplehash_to_ctrack(h);
    }
}
#endif

static void athr_nf_nat_delete_tuple(struct nf_conn *ct)
{
        athr_nat_add_or_del_tuple(ct, NAT_DEL_ENTRY);

        if (ct->hwnat_priv) {
            kmem_cache_free(nf_hwnat_cachep, ct->hwnat_priv);
            ct->hwnat_priv = NULL;
        }
}

static void athr_nf_nat_process_ops(struct sk_buff *skb, struct nf_conn *ct,
                         enum ip_conntrack_info ctinfo, u_int8_t protonum)
{
        u_int8_t protocol;
        struct nf_conn_help *help = NULL;
	
	if (test_bit(IPS_ATHR_HW_SRC_NAT_L2NOSUPPORT_BIT, &ct->status) ||
	    test_bit(IPS_ATHR_HW_DST_NAT_L2NOSUPPORT_BIT, &ct->status)) 
	{
		return;
	}

        protocol = nf_ct_protonum(ct);

	if (test_bit(IPS_ATHR_HW_NAT_ADDED_BIT, &ct->status) && (skb->ath_hw_nat_fw_flags == 0) ) {
                skb->ath_hw_nat_fw_flags = 2;
                return;
         }
        if (test_bit(IPS_ATHR_HW_NAT_ADDED_BIT, &ct->status) &&
           (skb->ath_hw_nat_fw_flags == 1 || skb->ath_hw_nat_fw_flags == 100)) {

            if (skb->ath_hw_nat_fw_flags == 1) {
                skb->ath_hw_nat_fw_flags = 2;
                return;
            }

            if ((protonum == IPPROTO_UDP) &&
                (test_bit(IPS_SEEN_REPLY_BIT, &ct->status) ||
                 test_bit(IPS_ASSURED_BIT, &ct->status))) {

                if(!test_bit(IPS_ATHR_HW_NAT_IS_UDP_BIT, &ct->status)) {

                    if(skb->ath_hw_nat_fw_flags == 100)
                        skb->ath_hw_nat_fw_flags = 1;
                    else if (skb->ath_hw_nat_fw_flags == 1 &&
                             test_bit(IPS_DST_NAT_BIT, &ct->status) &&
                             test_bit(IPS_DST_NAT_DONE_BIT, &ct->status)) {
                                     skb->ath_hw_nat_fw_flags = 2;
                    }
                    set_bit(IPS_ATHR_HW_NAT_IS_UDP_BIT, &ct->status);
                    athr_nat_add_or_del_tuple(ct, NAT_ADD_ENTRY);
                }
            }
        }
        else if (ctinfo == IP_CT_ESTABLISHED) {
            unsigned long algstat;

            athr_nat_add_or_del_tuple(ct, NAT_ADD_ENTRY);

            if(skb->ath_hw_nat_fw_flags == 3
                    && test_bit(IPS_ATHR_SW_NAT_SKIPPED_BIT, &ct->status)
                    && (protonum != IPPROTO_UDP)) {

                    if (protonum == IPPROTO_TCP) {
                            protonum = NAT_PROTO_TCP;
                    }
                    else if (protonum == IPPROTO_ICMP) {
                            protonum = NAT_PROTO_ICMP;
                    }
                    else
                    {
                            printk("%s: invalid protocol id %d\n", __func__, protonum);
                                    return;
                    }

                    help = nfct_help(ct);
                    algstat = help ? help->helper ? 1: 0:0;

                    if(!algstat && (nf_ct_l3num(ct) == AF_INET))
                            athr_mangle_conntrack_dnat(ct, NAT_ADD_ENTRY, protonum, NAT_INGRESS);

                            set_bit(IPS_DST_NAT_BIT, &ct->status);
                            set_bit(IPS_DST_NAT_DONE_BIT, &ct->status);
            }

            if (test_bit(IPS_ATHR_HW_NAT_ADDED_BIT, &ct->status)) {

                    if (!(test_bit(IPS_DST_NAT_DONE_BIT, &ct->status) &&
                          test_bit(IPS_DST_NAT_BIT, &ct->status)))
                        skb->ath_hw_nat_fw_flags = 2;
            }
        }
        else {
            if(protonum == IPPROTO_UDP) {

                if (skb->ath_hw_nat_fw_flags != 3 &&
                    (!test_bit(IPS_ATHR_HW_NAT_ADDED_BIT, &ct->status))) {

                    if (test_bit(IPS_SEEN_REPLY_BIT, &ct->status) ||
                        test_bit(IPS_ASSURED_BIT, &ct->status)) {

                        if (!test_bit(IPS_ATHR_HW_NAT_IS_UDP_BIT, &ct->status)) {
                            set_bit(IPS_ATHR_HW_NAT_IS_UDP_BIT, &ct->status);
                        }
                    }

                    athr_nat_add_or_del_tuple(ct, NAT_ADD_ENTRY);

                    if (test_bit(IPS_ATHR_HW_NAT_ADDED_BIT, &ct->status)) {

                        if (!(test_bit(IPS_DST_NAT_DONE_BIT, &ct->status) &&
                              test_bit(IPS_DST_NAT_BIT, &ct->status)) &&
                            skb->ath_hw_nat_fw_flags != 100) {
                            /* Condition added to solve UDP issue */
                            skb->ath_hw_nat_fw_flags = 2;
                        }
                    }
                }
                else if (skb->ath_hw_nat_fw_flags == 3) {

                    if (test_bit(IPS_ATHR_SW_NAT_SKIPPED_BIT, &ct->status) ||
                        (test_bit(IPS_DST_NAT_BIT, &ct->status) &&
                         test_bit(IPS_DST_NAT_DONE_BIT, &ct->status))) {

                        if(!test_bit(IPS_ATHR_HW_NAT_ADDED_BIT, &ct->status)) {
                            set_bit(IPS_ATHR_HW_NAT_ADDED_BIT, &ct->status);
                            //printk("Calling add_hw_nat_hash\n");
                            //add_hw_nat_hash(net, ct, IP_CT_DIR_ORIGINAL);
                        }
                    }
                }
            } else {
                if(skb->ath_hw_nat_fw_flags == 3) {

                    if (!test_bit(IPS_ATHR_HW_NAT_ADDED_BIT, &ct->status) &&
                        test_bit(IPS_ATHR_SW_NAT_SKIPPED_BIT, &ct->status)) {
                        set_bit(IPS_ATHR_HW_NAT_ADDED_BIT, &ct->status);
                    }
                }
            }
        }
}

static void athr_nf_ct_alter_port(struct nf_conntrack_tuple curr_tuple,
                           struct nf_conntrack_tuple new_tuple, struct nf_conn *ct)
{
        struct nf_ct_hwnat *nf_hwnat;

        if(curr_tuple.dst.protonum == IPPROTO_ICMP) {

                if (ct->hwnat_priv == NULL)
                    athr_alloc_hwnat_priv(ct);

                    nf_hwnat = (struct nf_ct_hwnat *)ct->hwnat_priv;
                    if(curr_tuple.src.u.icmp.id != new_tuple.src.u.icmp.id)
                        nf_hwnat->newport = 0xffff;
        }
}

static int athr_nf_ct_tuple_taken(const struct nf_conntrack_tuple *tuple,
                            const struct nf_conn *ignored_conntrack)
{
        unsigned int hash;
        struct hlist_nulls_node *n;
        struct nf_conntrack_tuple_hash *h;
        struct nf_conntrack_tuple temp = *tuple;



        rcu_read_lock_bh();
        hash = hash_conntrack(&temp);
        hlist_nulls_for_each_entry_rcu(h, n, &hwnat_mod_hash[hash], hnnode) {

                if (nf_ct_tuplehash_to_ctrack(h) != ignored_conntrack &&
                    nf_ct_tuple_equal(&temp, &h->tuple)) {
                        //NF_CT_STAT_INC(net, found);
                        rcu_read_unlock_bh();
                     return 1;
                }
                //NF_CT_STAT_INC(net, searched);
        }
        rcu_read_unlock_bh();
        return 0;
}

static void athr_nf_ct_process_ops(struct sk_buff *skb, struct nf_conn *ct,
                                   enum ip_conntrack_info ctinfo, u_int8_t protonum)
{
    if (skb != NULL) {
        athr_nf_nat_process_ops(skb, ct, ctinfo, protonum);
    } else if (skb == NULL && ctinfo == 0) {
        athr_nf_nat_delete_tuple(ct);
    }
}

static void athr_nf_ct_check_layer2if(struct sk_buff *skb, struct nf_conn *ct,
                                   enum nf_nat_manip_type maniptype)
{
	if(!skb || !(skb->dev) || !ct)
	{
		return;
	}

	if(strncmp(skb->dev->name, "ppp", 3) == 0)
	{
		if(maniptype == IP_NAT_MANIP_SRC)
		{
			set_bit(IPS_ATHR_HW_SRC_NAT_L2NOSUPPORT_BIT, &ct->status);
		}
		else if(maniptype == IP_NAT_MANIP_DST)
		{
			set_bit(IPS_ATHR_HW_DST_NAT_L2NOSUPPORT_BIT, &ct->status);
		}
	}
}

void athr_nat_down(void)
{
        athrs_nat_disable(EGRESS);
        athrs_nat_disable(INGRESS);

        if (hw_nat_api->init_done == 1) {

                nf_ct_free_hashtable(hwnat_mod_hash, hwnat_mod_hash_vmalloc,
                                     nf_conntrack_htable_size);

                rcu_assign_pointer(athr_nat_sw_ops->nf_process_nat, NULL);
                rcu_assign_pointer(athr_nat_sw_ops->nf_find_get, NULL);
                rcu_assign_pointer(athr_nat_sw_ops->get_wan_ipaddr, NULL);
                rcu_assign_pointer(athr_nat_sw_ops->nf_tuple_taken, NULL);
                rcu_assign_pointer(athr_nat_sw_ops->nf_alter_port, NULL);
                rcu_assign_pointer(athr_nat_sw_ops->check_layer2if, NULL);

                kfree(athr_nat_sw_ops);
                athr_nat_sw_ops = NULL;
        }

}

void athr_nat_init(void)
{

	printk(" %s \n",__func__); 

        if (hw_nat_api->init_done == 1) {
            athr_nat_down();
        }

        if (!athr_nat_sw_ops) {
            athr_nat_sw_ops = (athr_nf_nat_ops_t *)kzalloc(sizeof(athr_nf_nat_ops_t), GFP_KERNEL);

            rcu_assign_pointer(athr_nat_sw_ops->nf_process_nat, athr_nf_ct_process_ops);
            rcu_assign_pointer(athr_nat_sw_ops->nf_find_get, athr_nf_find_get);
            rcu_assign_pointer(athr_nat_sw_ops->get_wan_ipaddr, athr_get_wan_ip_addr);
            rcu_assign_pointer(athr_nat_sw_ops->nf_tuple_taken, athr_nf_ct_tuple_taken);
            rcu_assign_pointer(athr_nat_sw_ops->nf_alter_port, athr_nf_ct_alter_port);
            rcu_assign_pointer(athr_nat_sw_ops->check_layer2if, athr_nf_ct_check_layer2if);
        }

        athr_nat_init_and_enable(EGRESS);
        athr_nat_init_and_enable(INGRESS);

        printk("%s: Setting h/w nat router mac addr \n", __func__);
        athrs_hw_nat_set_router_mac_addr(athrs_wan_if_mac_addr);

        athrs_nat_op_lock_init();

        hwnat_mod_hash = nf_ct_alloc_hashtable(&nf_conntrack_htable_size,
                                               &hwnat_mod_hash_vmalloc, 1);
        if (!hwnat_mod_hash) {
                printk(KERN_ERR "Unable to create hwnat_mod_hash\n");
                goto err_hw_hash;
        }

        net_hwnat->ct.sysctl_checksum = 0;

        if (!nf_hwnat_cachep) {
            nf_hwnat_cachep = kmem_cache_create("nf_hwnat",
                                                sizeof(struct nf_ct_hwnat),
                                                0, SLAB_DESTROY_BY_RCU, NULL);
            if (!nf_hwnat_cachep) {
                printk(KERN_ERR "Unable to create nf_ct_hwnat slab cache\n");
                goto err_cache;
            }
        }

        if (hwnat_proc_entry == 0) {
                hwnat_proc_entry = athr_nat_proc_init(net_hwnat);
                if (hwnat_proc_entry != 1)
                        goto err_proc;
        }

        hw_nat_api->init_done = 1;
        return;

err_proc:
        kmem_cache_destroy(nf_hwnat_cachep);
        nf_hwnat_cachep = NULL;
err_cache:
        nf_ct_free_hashtable(hwnat_mod_hash, hwnat_mod_hash_vmalloc,
                             nf_conntrack_htable_size);
err_hw_hash:
        rcu_assign_pointer(athr_nat_sw_ops->nf_process_nat, NULL);
        rcu_assign_pointer(athr_nat_sw_ops->nf_find_get, NULL);
        rcu_assign_pointer(athr_nat_sw_ops->get_wan_ipaddr, NULL);
        rcu_assign_pointer(athr_nat_sw_ops->nf_tuple_taken, NULL);
        rcu_assign_pointer(athr_nat_sw_ops->nf_alter_port, NULL);
        rcu_assign_pointer(athr_nat_sw_ops->check_layer2if, NULL);

        kfree(athr_nat_sw_ops);
        athr_nat_sw_ops = NULL;
        return;
}

static void athr_nat_cleanup(void)
{
        kmem_cache_destroy(nf_hwnat_cachep);
        nf_hwnat_cachep = NULL;
        hw_nat_api = NULL;
}

static void athr_reg_net_notifcations(void)
{
        register_netdevice_notifier(&athr_nat_netdev_notifier);
        register_inetaddr_notifier(&athr_nat_inetaddr_notifier);
}

static void athr_unreg_net_notifcations(void)
{
        unregister_inetaddr_notifier(&athr_nat_inetaddr_notifier);
        unregister_netdevice_notifier(&athr_nat_netdev_notifier);
}

void *athrs_gmac_register_nat_ops(void)
{
        if (!hw_nat_api) {
            hw_nat_api = (athr_nat_ops_t *)kzalloc(sizeof(athr_nat_ops_t), GFP_KERNEL);

            hw_nat_api->init                 = athr_nat_init;
            hw_nat_api->down                 = athr_nat_down;
            hw_nat_api->add_entry            = athr_hw_nat_add_entry;
            hw_nat_api->del_entry            = athr_hw_nat_del_entry;
            hw_nat_api->lkp_entry            = athr_hw_nat_lookup_entry;
            hw_nat_api->set_wan_mac_addr     = athr_set_nat_wan_mac;
            hw_nat_api->set_wan_ip_addr      = athr_set_nat_wan_ip;
            hw_nat_api->reg_net_notifier     = athr_reg_net_notifcations;
            hw_nat_api->unreg_net_notifier   = athr_unreg_net_notifcations;
            hw_nat_api->proc_init            = athr_nat_proc_init;
            hw_nat_api->proc_remove          = athr_nat_proc_remove;
            hw_nat_api->cleanup              = athr_nat_cleanup;
            hw_nat_api->init_done            = 0;               /*Set this in athr_nat_init func() */
        }

        return hw_nat_api;
}
