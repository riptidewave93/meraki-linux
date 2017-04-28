/**
 * AppliedMicro APM862xx SoC Ethernet Driver
 *
 * Copyright (c) 2010 Applied Micro Circuits Corporation.
 * All rights reserved. Keyur Chudgar <kchudgar@apm.com>
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.	 See the
 * GNU General Public License for more details.
 *
 * @file apm_enet_access.c
 *
 * This file implements driver for APM862xx SoC Ethernet subsystem
 *
 */
#include <linux/netdevice.h>
#include <net/ip.h>
#include <net/tcp.h>
#include <linux/if_vlan.h>
#include <linux/inetdevice.h>
#include <linux/in6.h>
#include <net/addrconf.h>
#include <asm/ipp.h>
#include <asm/cacheflush.h>
#if defined(CONFIG_SMP)
#include <asm/smp.h>
#else
#include <asm/apm86xxx_soc.h>
#endif

#include "apm_enet_mac.h"
#include "apm_enet_misc.h"
#include "apm_enet_access.h"
#ifdef CONFIG_INET_OFFLOAD
#include "apm_enet_offload.h"
#endif
#ifdef CONFIG_CLE_BRIDGE
#include "apm_cle_bridge.h"
#endif
#include "apm_enet_lro.h"

/* Define to enable Extended Link List Support */
#undef ENABLE_ELL	

#if defined(CONFIG_SLAB_HW)
extern int eth0_fp_id;
extern int eth1_fp_id;
extern int ppc_fp_id;
extern int ppc_fp_mbid;
extern int ppc_hw_buff_pool_init;
extern struct kmem_cache *virt_to_cache_ext(const void *obj);
extern int apm_enet_init_fp(int queue_id, int buff_size,
				int no_of_buffs, int qm_blk, int hdr_rsvd);
extern int apm_enet_rx_frame(struct apm_qm_msg_desc *rx_msg_desc);
#endif
struct net_device *devices[MAX_PORTS] = {0};
struct apm_data_priv *privinfo[MAX_PORTS];
static struct apm_enet_dev apm_enet_dev_struct = {0};
#if !defined (NOTIFICATION_OFF) && !defined (CONFIG_SLAB_HW)
atomic_t tx_completion_counter[MAX_PORTS] = {{0}};
#endif
/* Queue ID mapper to private structure for reverse lookup */
struct apm_enet_dev_base * qid2priv[MB_MAX_QUEUES];
static int ipp_notifier_registered = 0;

struct apm_enet_skb_cb {
	void *ell;	/* Extended link list */
#if !defined (NOTIFICATION_OFF) && !defined (CONFIG_SLAB_HW)
	u32 port_id;
#endif
};
#define APM_ENET_SKB_CB(__skb) 	\
	((struct apm_enet_skb_cb *)&((__skb)->cb[0]))

int apm_enet_wr32(struct apm_data_priv *priv, u8 block_id,
		u32 reg_offset, u32 value)
{
	u32 command_done;
	u8  indirect = 0;
	u32 count = 0;
	u32 base_addr_v;
	u32 addr_reg_offst, cmd_reg_offst, wr_reg_offst;
	u32 cmd_done_reg_offst;
	
	switch (block_id) {
	case BLOCK_ETH_MAC:
		if (likely(reg_offset < MII_MGMT_CONFIG_ADDR ||
				reg_offset > MII_MGMT_INDICATORS_ADDR))
			base_addr_v = priv->mac_base_addr_v;
		else
			base_addr_v = priv->enet_mii_base_addr_v;

		addr_reg_offst =
			base_addr_v + MAC_ADDR_REG_OFFSET; 
		cmd_reg_offst =
			base_addr_v + MAC_COMMAND_REG_OFFSET;
		wr_reg_offst =
			base_addr_v + MAC_WRITE_REG_OFFSET;
		cmd_done_reg_offst =
			base_addr_v + MAC_COMMAND_DONE_REG_OFFSET;
		indirect = 1;
		ENET_DEBUG_WR("MAC write\n");
		break;
		
	case BLOCK_ETH_STATS:
		addr_reg_offst =
			priv->stats_base_addr_v + STAT_ADDR_REG_OFFSET;
		cmd_reg_offst =
			priv->stats_base_addr_v + STAT_COMMAND_REG_OFFSET;
		wr_reg_offst =
			priv->stats_base_addr_v + STAT_WRITE_REG_OFFSET;
		cmd_done_reg_offst =
			priv->stats_base_addr_v + STAT_COMMAND_DONE_REG_OFFSET;
		indirect = 1;
		ENET_DEBUG_WR("STATS write\n");
		break;
	
	case BLOCK_ETH_GBL:
		addr_reg_offst = priv->eth_gbl_base_addr_v + reg_offset;
		ENET_DEBUG_WR("ENET Global CSR write\n");
		break;
		
	case BLOCK_ETH_FFDIV:
		addr_reg_offst = priv->eth_ffdiv_base_addr_v + reg_offset;
		ENET_DEBUG_WR("ENET FFDIV CSR write\n");
		break;
		
	case BLOCK_ETH_MAC_GBL:
		addr_reg_offst = priv->mac_gbl_base_addr_v + reg_offset;
		ENET_DEBUG_WR("ENET MAC Global CSR write\n");
		break;
		
	case BLOCK_ETH_PTP:
		addr_reg_offst = priv->eth_ptp_base_addr_v + reg_offset;
		ENET_DEBUG_WR("ENET PTP CSR write\n");
		break;
		
	case BLOCK_ETH_UNISEC:
		addr_reg_offst = priv->eth_unisec_base_addr_v + reg_offset;
		ENET_DEBUG_WR("ENET UNISEC CSR write\n");
		break;
		
	case BLOCK_ETH_DIAG:
		addr_reg_offst = priv->eth_diag_base_addr_v + reg_offset;
		ENET_DEBUG_WR("ETH DIAG CSR write\n");
		break;

	case BLOCK_ETH_QMI_SLAVE:
		addr_reg_offst = priv->eth_qmi_base_addr_v + reg_offset;
		ENET_DEBUG_WR("ENET QMI SLAVE CSR write\n");
		break;

	case  BLOCK_ETH_MACIP_IND:
		addr_reg_offst = priv->vpaddr_base + reg_offset;
		ENET_DEBUG_WR("ENET MACIP INDIRECT CSR write\n");
		break;
		
	default:
		printk(KERN_ERR 
			"Invalid block id %d write reg\n", block_id);
		return -1;
	}
	
	if (indirect) {
		/* Write the register offset in DCR */
		out_be32((void __iomem *) addr_reg_offst, reg_offset);
		ENET_DEBUG_WR("Indirect addr_reg_offst : 0x%X,"
				"value(reg_offset) 0x%X\n",
				addr_reg_offst, reg_offset);
		
		/* Write the data in the wrData register */
		out_be32((void __iomem *) wr_reg_offst, value);
		ENET_DEBUG_WR("Indirect wr_reg_offst: 0x%X value 0x%X\n",
			      wr_reg_offst, value);
		
		/* Invoke write command */
		out_be32((void __iomem *) cmd_reg_offst, WRITE0_WR(1));
		ENET_DEBUG_WR("Indirect cmd_reg_offst: 0x%X "
			      "value(cmd) 0x%X\n",
			      cmd_reg_offst,  WRITE0_WR(1) );
		
		/* check command done */
		while (1) {
			command_done =
				in_be32((void __iomem *) cmd_done_reg_offst);
			ENET_DEBUG_WR("Indirect cmd_done_reg_offst: 0x%X,"
					"command_done:0x%X \n",
					cmd_done_reg_offst, command_done);
			
			if (command_done)
				break;
   
			udelay(ACCESS_DELAY_TIMEMS);
			if(count++ > MAX_LOOP_POLL_CNT) {
				printk(KERN_ERR
					"Write failed for blk %d\n",block_id);
				return -1;
			}
		}
		/* Reset command reg */
		ENET_DEBUG_RD("Reset command reg[0X%08X] \n ", cmd_reg_offst);
		out_be32((void __iomem *) cmd_reg_offst, 0);
	} else {
		out_be32((void __iomem *) addr_reg_offst, value);
		ENET_DEBUG_WR("Direct write addr: 0x%X value 0x%X\n",
				addr_reg_offst, value);
	}
	return 0;
}

int apm_enet_rd32(struct apm_data_priv *priv, u8 block_id,
		u32 reg_offset, u32 *value)
{
	u8 indirect = 0;
	u32 command_done;
	u32 count = 0;
	u32 base_addr_v;
	u32 addr_reg_offst, cmd_reg_offst, rd_reg_offst;
	u32 cmd_done_reg_offst;

	switch (block_id) {
	case BLOCK_ETH_MAC:
		if (likely(reg_offset < MII_MGMT_CONFIG_ADDR ||
				reg_offset > MII_MGMT_INDICATORS_ADDR))
			base_addr_v = priv->mac_base_addr_v;
		else
			base_addr_v = priv->enet_mii_base_addr_v;

		addr_reg_offst =
			base_addr_v + MAC_ADDR_REG_OFFSET; 
		cmd_reg_offst =
			base_addr_v + MAC_COMMAND_REG_OFFSET;
		rd_reg_offst =
			base_addr_v + MAC_READ_REG_OFFSET;
		cmd_done_reg_offst =
			base_addr_v + MAC_COMMAND_DONE_REG_OFFSET;
		indirect = 1;
		ENET_DEBUG_RD("MAC read \n");
		break;
		
	case BLOCK_ETH_STATS:
		addr_reg_offst =
			priv->stats_base_addr_v + STAT_ADDR_REG_OFFSET;
		cmd_reg_offst =
			priv->stats_base_addr_v + STAT_COMMAND_REG_OFFSET;
		rd_reg_offst =
			priv->stats_base_addr_v + STAT_READ_REG_OFFSET;
		cmd_done_reg_offst =
		priv->stats_base_addr_v + STAT_COMMAND_DONE_REG_OFFSET;
		indirect = 1;
		ENET_DEBUG_RD("STATS read \n");
		break;
		
	case BLOCK_ETH_GBL:
		addr_reg_offst = priv->eth_gbl_base_addr_v + reg_offset;
		ENET_DEBUG_RD("ENET Global CSR read\n");
		break;
		
	case BLOCK_ETH_FFDIV:
		addr_reg_offst = priv->eth_ffdiv_base_addr_v + reg_offset;
		ENET_DEBUG_RD("ENET FFDIV CSR read\n");
		break;
		
	case BLOCK_ETH_MAC_GBL:
		addr_reg_offst = priv->mac_gbl_base_addr_v + reg_offset;
		ENET_DEBUG_RD("ENET MAC Global CSR read\n");
		break;
		
	case BLOCK_ETH_PTP:
		addr_reg_offst = priv->eth_ptp_base_addr_v + reg_offset;
		ENET_DEBUG_RD("ENET PTP CSR read\n");
		break;
		
	case BLOCK_ETH_UNISEC:
		addr_reg_offst = priv->eth_unisec_base_addr_v + reg_offset;
		ENET_DEBUG_RD("ENET UNISEC CSR read\n");
		break;
		
	case BLOCK_ETH_DIAG:
		addr_reg_offst = priv->eth_diag_base_addr_v + reg_offset;
		ENET_DEBUG_RD("ETH DIAG CSR read\n");
		break;

	case BLOCK_ETH_QMI_SLAVE:
		addr_reg_offst = priv->eth_qmi_base_addr_v + reg_offset;
		ENET_DEBUG_RD("ENET QMI SLAVE CSR read\n");
		break;

	case  BLOCK_ETH_MACIP_IND:
		addr_reg_offst = priv->vpaddr_base + reg_offset;
		ENET_DEBUG_RD("ENET MACIP INDIRECT CSR read\n");
		break;
		
	default:
		printk(KERN_ERR "Invalid blockid in read reg: %d\n", block_id);
		return -1;
	}
	
	if (indirect) {
		/* Write the MAC register offset in DCR */
		out_be32((void __iomem *) addr_reg_offst, reg_offset);
		ENET_DEBUG_RD("Indirect addr_reg_offst: 0x%X "
				"value(reg_offset) 0x%X\n",
				addr_reg_offst, reg_offset);
		
		/* Invoke read command */
		out_be32((void __iomem *) cmd_reg_offst, READ0_WR(1));
		ENET_DEBUG_RD("Indirect cmd_reg_offst: 0x%X "
				"value(cmd) 0x%X\n",
				cmd_reg_offst,	READ0_WR(1));
		
		/* Poll for command done */
		while (1) {
			command_done =
				in_be32((void __iomem *) cmd_done_reg_offst);
			ENET_DEBUG_RD("Indirect cmd_done_reg_offst: 0x%X "
					"command_done:0x%X \n",
					cmd_done_reg_offst, command_done);
			
			if (command_done)
				break;
			
			udelay(ACCESS_DELAY_TIMEMS);
			if(count++ > MAX_LOOP_POLL_CNT) {
				printk(KERN_ERR
					"Read failed for blk %d\n",block_id);
				return -1;
			}
		}
		
		*value = in_be32((void __iomem *) rd_reg_offst);
		ENET_DEBUG_RD("Indirect rd_reg_offst: 0x%X value read 0x%X\n",
				rd_reg_offst, *value);
		
		/* Reset command reg */
		ENET_DEBUG_RD("Reset command reg[0X%08X] \n ", cmd_reg_offst);
		out_be32((void __iomem *) cmd_reg_offst, 0);
	} else {
		*value = in_be32((void __iomem *) addr_reg_offst);
		ENET_DEBUG_RD("Direct read addr: 0x%X value: 0x%X\n",
				addr_reg_offst, *value);
	}
	return 0;
}

struct apm_enet_dev_base * apm_enet_qid2ctx(int qid)
{
	return qid2priv[qid];
}

int apm_enet_is_smp(void)
{
	return apm_enet_dev_struct.is_smp;
}

static int apm_enet_change_mtu(struct net_device *ndev, int new_mtu)
{
	struct apm_enet_dev_base *priv_dev = netdev_priv(ndev);
	struct apm_data_priv *priv = &priv_dev->priv;

	if (HW_MTU(new_mtu) < APM_ENET_MIN_MTU || HW_MTU(new_mtu) > APM_ENET_MAX_MTU) {
		printk(KERN_ERR "Invalid MTU: %d\n", new_mtu);
		return -EINVAL;
	}
	
	printk(KERN_INFO "changing MTU from %d to %d\n", ndev->mtu, new_mtu);
	
	if ( !netif_running(ndev)) {
		ndev->mtu = new_mtu;
		apm_gmac_change_mtu(priv, HW_MTU(new_mtu));
	} else {
		netif_stop_queue(ndev);
		apm_gmac_rx_disable(priv);
		apm_gmac_tx_disable(priv);

		ndev->mtu = new_mtu;
		apm_gmac_change_mtu(priv, HW_MTU(new_mtu));

		apm_gmac_rx_enable(priv);
		apm_gmac_tx_enable(priv);
		netif_start_queue(ndev);
	}
	return 0;
}

#if !defined(CONFIG_SLAB_HW) && !defined(CONFIG_DRIVER_POOL)
static int apm_enet_init_fp(int queue_id, int buff_size,
		 	int no_of_buffs, int qm_blk, int hdr_rsvd)
{
	int i;
	struct sk_buff *tmp_skb;
	phys_addr_t phy_addr;
	struct apm_qm_msg_desc fp_msg_desc;
	struct apm_qm_msg16 msg;
	int rc = 0;

	memset(&msg, 0, sizeof(msg));
	fp_msg_desc.msg = &msg;

	/* Common fields */ 
	msg.C = apm_enet_pkt_iscoherent();
	msg.BufDataLen = APM_PKT_POOL_SIZE;	/* Set buffer size */ 
	msg.FPQNum = queue_id;
	msg.RType = APM_QM_ETH_RTYPE;
	
	fp_msg_desc.qid = queue_id; 
	fp_msg_desc.mb_id = queue_id;
	fp_msg_desc.msg = &msg;

	/* Program all the free buffer pools */
	for (i = 0; i < no_of_buffs; i++) {
		tmp_skb = dev_alloc_skb(buff_size + hdr_rsvd);
		if (unlikely(!tmp_skb)) {
			printk(KERN_ERR 
				"Failed to allocate new skb size %d", 
				buff_size);
			return -ENOMEM;
		}
		skb_reserve(tmp_skb, hdr_rsvd);

		msg.UserInfo = (u32) tmp_skb;
		
		/* Convert to physical address and program free buffer pool */
		phy_addr = virt_to_phys(tmp_skb->data);
		msg.DataAddrMSB = (u32) (phy_addr >> 32);
		msg.DataAddrLSB = (u32) phy_addr;

#if defined CONFIG_NOT_COHERENT_CACHE && !defined CONFIG_APM86xxx_IOCOHERENT
		invalidate_dcache_range((u32) tmp_skb->data,
			(u32) tmp_skb->data + buff_size);
#endif 
		/* Fill with the new buffer address */
		if (qm_blk == 0) {
			if (unlikely((apm_qm_fp_dealloc_buf(&fp_msg_desc)) != 0)) {
				printk(KERN_ERR 
					"Can not allocate QM FP buffer\n");
				return -1;
			}
		} else if (!is_apm86xxx_lite()) {
			if (unlikely((apm_qml_fp_dealloc_buf(&fp_msg_desc)) != 0)) {
				printk(KERN_ERR 
					"Can not allocate QML FP buffer\n");
				return -1;
			}
		}
	}
	return rc;
}

static int inline apm_enet_cfg_buf_address(int queue_id, int size, int hdr_rsvd)
{
	struct apm_qm_msg_desc fp_msg_desc;
	struct apm_qm_msg16 msg;
	struct sk_buff *tmp_skb;
	phys_addr_t phy_addr;

	fp_msg_desc.msg = &msg;
	fp_msg_desc.qid = queue_id; 
	fp_msg_desc.mb_id = queue_id;

	tmp_skb = dev_alloc_skb(size + hdr_rsvd);
	if (unlikely(!tmp_skb)){
		printk(KERN_ERR "Failed to allocate new skb size %d\n",
			size);
		return -ENOMEM;
	}
	skb_reserve(tmp_skb, hdr_rsvd);

	phy_addr = virt_to_phys(tmp_skb->data);
	
	/* Program individual WORD to avoid use of memzero */
	((u32 *) &msg)[0] = apm_enet_pkt_iscoherent() << 23 | 
				(APM_PKT_POOL_SIZE << 8) | 
				(u32) (phy_addr >> 32);
	((u32 *) &msg)[1] = (u32) phy_addr;
	((u32 *) &msg)[2] = APM_QM_ETH_RTYPE << 24 | queue_id;
	((u32 *) &msg)[3] = (u32) tmp_skb;

#if defined CONFIG_NOT_COHERENT_CACHE && !defined CONFIG_APM86xxx_IOCOHERENT
	invalidate_dcache_range((u32) tmp_skb->data,
		(u32) tmp_skb->data + size);
#endif 
	/* Fill with the new buffer address */
	if (unlikely((apm_qm_fp_dealloc_buf(&fp_msg_desc)) != 0)) {
		kfree_skb(tmp_skb);
		printk(KERN_ERR "Can not replenish FP buffer\n");
		return -1;
	}
	return 0;
}

static int apm_enet_deinit_fp(int qid)
{
	struct apm_qm_qstate qstate;
	struct apm_qm_msg32 msg;
	struct apm_qm_msg_desc desc;
	int rc;
	int i;
	
	rc = apm_qm_qstate_rd(IP_BLK_QM, qid, &qstate); 
	if (rc < 0) 
		return rc;
	desc.qid = qid;
	desc.mb_id = qid;
	desc.msg = &msg;
	for (i = 0; i < qstate.nummsgs; i++) {
		rc = apm_qm_fp_alloc_buf(&desc);
		if (rc < 0)
			break;
		if (msg.msg16.UserInfo)
			kfree_skb((struct sk_buff *) msg.msg16.UserInfo);			
	}
	return 0;	
}
#endif

#if !defined(CONFIG_SLAB_HW)
#if 0
static void apm_enet_refill_fp(int qid, int size, u32 paddr_hi, u32 paddr_lo)
{
	struct apm_qm_msg_desc fp_msg_desc;
	struct apm_qm_msg16 msg;
#ifdef FLUSH_DCACHE
	void *data;
#endif	
	memset(&msg, 0, sizeof(msg));
	fp_msg_desc.msg = &msg;

	/* Common fields  */ 
	msg.C = apm_enet_pkt_iscoherent();
	msg.BufDataLen = APM_PKT_POOL_SIZE;	/* Set buffer size */ 
	msg.FPQNum = qid;
	msg.RType = APM_QM_ETH_RTYPE;
	msg.DataAddrMSB = paddr_hi;
	msg.DataAddrLSB = paddr_lo;

	fp_msg_desc.qid = qid; 
	fp_msg_desc.mb_id = qid;
#ifdef FLUSH_DCACHE
	data = phys_to_virt(((u64) paddr_hi << 32) | paddr_lo);
	invalidate_dcache_range((u32) data, (u32) data + size);
#endif 
	if (unlikely((apm_qm_fp_dealloc_buf(&fp_msg_desc)) != 0)) {
		printk(KERN_ERR "Can not replenish FP buffer\n");
	}
}
#endif
#endif

#ifdef ENET_LINK_POLL
static void apm_enet_link_down(struct net_device *ndev)
{
	struct apm_enet_dev_base *priv_dev = netdev_priv(ndev);
	struct apm_data_priv *priv = &priv_dev->priv;

	/* Disable Mac Rx and Tx */ 
	apm_gmac_rx_disable(priv);

	netif_carrier_off(ndev);
	/* Stop xmit queue */
	netif_stop_queue(ndev);
	
	apm_gmac_tx_disable(priv);
	priv_dev->link_status = 0;
	apm_enet_dev_struct.link_down_ref++;
	return;
}

static void apm_enet_link_up(struct net_device *ndev)
{
	struct apm_enet_dev_base *priv_dev = netdev_priv(ndev);
	struct apm_data_priv *priv = &priv_dev->priv;
	u32 speed;
	
	/* Start xmit queue */
	netif_carrier_on(ndev);
	netif_start_queue(ndev);

	speed = apm_get_link_speed(priv);
	priv_dev->link_status = apm_get_phy_link_status(priv);

	apm_gmac_init(priv, ndev->dev_addr, speed, HW_MTU(ndev->mtu), priv->crc);
	if (priv_dev->ipg > 0)
		apm_gmac_set_ipg(priv, priv_dev->ipg); 	

	PHY_PRINT("port:%d, phyid:%d, speed:%d, link_status: %d \n",
		  priv->port, priv->phy_addr, speed, priv_dev->link_status);
	priv_dev->link_status = 1;
	apm_enet_dev_struct.link_down_ref--;
	return;
}

static void apm_enet_link_poll(unsigned long data)
{
	struct net_device *ndev;
	struct apm_enet_dev_base *priv_dev;
	struct apm_data_priv *priv;
	u32 speed; 
	u32 link_status;
	int i;
	u32 poll_timer = HZ;

	for (i = 0; i < MAX_PORTS; i++) {
		ndev = devices[i];
		if(!ndev || !netif_running(ndev)) 
			continue;
		
		priv_dev = netdev_priv(ndev);
		priv = &priv_dev->priv;
		
		link_status = apm_get_phy_link_status(priv);
		speed = apm_get_link_speed(priv);

		PHY_PRINT("port:%d, phyid:%d, speed:%d, link_status: %d \n",
		   	priv->port, priv->phy_addr, speed, link_status);
	
		if (link_status == priv_dev->link_status)
			continue;
	
		if (!link_status) {
			apm_enet_link_down(ndev);
			printk(KERN_INFO "%s Link is down \n", ndev->name);
		}
		else {
			apm_enet_link_up(ndev);
			printk(KERN_INFO "%s Link is up with speed %d Mbps\n",
			    	ndev->name, speed);
		}
	}
	apm_enet_dev_struct.link_poll_timer.expires =
		jiffies + poll_timer;
	add_timer(&apm_enet_dev_struct.link_poll_timer);
	return;
}

static void apm_enet_start_link_poll(void)
{
	init_timer(&apm_enet_dev_struct.link_poll_timer);
	apm_enet_dev_struct.link_poll_timer.expires =
	     				jiffies + HZ/2;
	apm_enet_dev_struct.link_poll_timer.function = apm_enet_link_poll;
	add_timer(&apm_enet_dev_struct.link_poll_timer);
}
#endif

static inline int eth_hdr_len(const void *data)
{
	const struct ethhdr *eth = data;

	return eth->h_proto == htons(ETH_P_8021Q) ? VLAN_ETH_HLEN : ETH_HLEN;
}

#if !defined (NOTIFICATION_OFF) && !defined (CONFIG_SLAB_HW)
static inline int apm_qm_pull_tx_comp_msg(u32 mailbox)
{
	int count = 0;
	struct sk_buff *skb_to_free;
	int skbval;

	while ((skbval = apm_qm_pull_comp_msg(mailbox)) != -1) {
		/* Retrieve skb address */
		skb_to_free = (struct sk_buff *) skbval;
		if (likely(skb_to_free != NULL)) {
			u32 port_id = APM_ENET_SKB_CB(skb_to_free)->port_id;

			ENET_DEBUGRXTX("Freeing SKB in TX "
					"module: 0x%p\n", skb_to_free);
#if defined(ENABLE_ELL)
			if (APM_ENET_SKB_CB(skb_to_free)->ell != NULL)
				kfree(APM_ENET_SKB_CB(skb_to_free)->ell);
#endif
			FREE_SKB(skb_to_free);
			atomic_dec(&tx_completion_counter[port_id]);
		} else {
			printk(KERN_ERR "Error retriving msg from "
					"comp queue\n");
			break;
		}
		++count;
	}
	return count;
}
#endif

/* IP Forward Function */
#ifdef CONFIG_INET_OFFLOAD
static inline int apm_enet_start_xmit_ipfwd(u32 tx_qid, struct apm_qm_msg16 *tx_msg16, void *tx_data)
{
	struct apm_qm_msg_desc tx_msg_desc;
	struct apm_qm_msg16 *msg16;
	struct apm_qm_msg64 msg;
	struct apm_qm_msg_up8 *msg8_1;

	memset(&msg.msg32_1.msg16, 0, sizeof(msg.msg32_1.msg16));
	msg16 = &msg.msg32_1.msg16;
	msg8_1 = &msg.msg32_1.msgup8_1;

	msg16->BufDataLen = tx_msg16->BufDataLen;
	msg16->FPQNum = tx_msg16->FPQNum;
	msg16->DataAddrLSB = tx_msg16->DataAddrLSB;
	msg16->C = apm_enet_pkt_iscoherent();
	msg16->UserInfo = tx_msg16->UserInfo;

	/* HR = 1 HE = 1 H0Enq_Num = 200 */
	/* H0Info_msb = 0x18 (TYPE_SEL_WORK_MSG << 4 | TSO_INS_CRC_ENABLE << 3) */
	/* H0Info_lsb = 0x0140E145 (Enable IPProto Checksum, EthHdr 14, IPHdr 5, TCPHdr 5) */
	*(u64 *)(msg8_1) = 0x0300C8180140E145LL;

	tx_msg_desc.qid = tx_qid;
	tx_msg_desc.msg = &msg;
#if defined(CONFIG_NOT_COHERENT_CACHE)
	flush_dcache_range((u32) tx_data, (u32) tx_data + 32);
#endif 
	apm_qm_push_msg(&tx_msg_desc);
	return 0;
}
#endif

/* Packet transmit function */
static int apm_enet_start_xmit(struct sk_buff *skb, struct net_device *ndev)
{
	struct apm_enet_dev_base *priv_dev = netdev_priv(ndev);
	phys_addr_t paddr;
	struct apm_qm_msg_desc tx_msg_desc;
	struct apm_qm_msg64 msg;
#if !defined (NOTIFICATION_OFF) && !defined (CONFIG_SLAB_HW)
	u32 port_id = priv_dev->port_id;
#elif defined CONFIG_SLAB_HW
	struct kmem_cache *c;
	u8 is_eth_fp = 0;
	u8 is_ppc_fp = 0;
#endif
	struct apm_qm_msg16 *msg16;
	struct apm_qm_msg_up8 *msg8_1;
	u32 core_id = apm_processor_id();
	struct eth_queue_ids *eth_queues = &priv_dev->qm_queues[core_id];
	u32 nr_frags = skb_shinfo(skb)->nr_frags;

	if (skb->len > priv_dev->priv.hw_mtu) {
	    priv_dev->estats.tx_dropped++;
	    dev_kfree_skb(skb);
	    return NETDEV_TX_OK;
	}

	paddr = virt_to_phys((volatile void *) skb->data);

	ENET_DEBUGTX("TX Frame PADDR: 0x%010llx VADDR: 0x%p len: %d frag %d\n",
		paddr, skb->data, skb->len, nr_frags);

	/* Packet not fragmented */
	if (likely(nr_frags == 0)) {
		memset(&msg.msg32_1, 0, sizeof(msg.msg32_1));
		msg16 = &msg.msg32_1.msg16;
		msg8_1 = &msg.msg32_1.msgup8_1;
#if defined(ENABLE_ELL)
		APM_ENET_SKB_CB(skb)->ell = NULL;
#endif			
		/* Prepare QM message */
		msg16->BufDataLen = skb->len;
#if defined(CONFIG_SLAB_HW)
		msg16->BufDataLen &= 0xfff;
		msg16->BufDataLen |= APM_PKT_POOL_SIZE;
		c = virt_to_cache_ext(skb->head);
        	/* Check if allocated from ethernet free pool */
        	if (c->flags & __GFP_FREEPOOL_ETH0) {
			/* Ask Enet hardware to free up buffer after its done */
			msg16->FPQNum = eth0_fp_id;
			msg8_1->HR = 1;
			is_eth_fp = 1;
		} else if (c->flags & __GFP_FREEPOOL_ETH1) {
			/* Ask Enet hardware to free up buffer after its done */
			msg16->FPQNum = eth1_fp_id;
			msg8_1->HR = 1;
			is_eth_fp = 1;
		} else if (c->flags & __GFP_FREEPOOL_PPC) {
			is_ppc_fp = 1;
			msg8_1->HR = 1;
			msg16->FPQNum = ppc_fp_id;
		}
#endif
		msg16->DataAddrMSB = (u32) (paddr >> 32);
		msg16->DataAddrLSB = (u32) paddr;

		ENET_DUMP("TX ", skb->data, skb->len);
	} else if (nr_frags < 5) {
		struct apm_enet_msg_ext8 *ext_msg;
		int i;
#if defined(ENABLE_ELL)
		APM_ENET_SKB_CB(skb)->ell = NULL;
#endif			
		memset(&msg, 0, sizeof(msg));
		msg16 = &msg.msg32_1.msg16;
		msg8_1 = &msg.msg32_1.msgup8_1;
		ext_msg = (struct apm_enet_msg_ext8*) &msg.msg32_2;
		/* First Fragment */
		msg16->BufDataLen = skb_headlen(skb);
		msg16->DataAddrMSB = (u32) (paddr >> 32);
		msg16->DataAddrLSB = (u32) paddr;
		/* Its 64B msg */
		msg16->NV = 1;
		
		ENET_DUMP("TX ", skb->data, msg16->BufDataLen);

		for (i = 0; i < nr_frags; i++) {
			skb_frag_t *frag = &skb_shinfo(skb)->frags[i];
			void *addr;
			
			addr  = page_address(frag->page.p) + frag->page_offset;
			paddr = virt_to_phys(addr);
			ext_msg->NxtBufDataLength = frag->size;
			ext_msg->NxtDataAddrMSB = (u32) (paddr >> 32);
			ext_msg->NxtDataAddrLSB = (u32) paddr;
			ENET_DEBUGTX(
				"Frag[%d] len %d PADDR 0x%010llx VADDR 0x%p\n",
				i, frag->size, paddr, addr);
			ENET_DUMP("TX ", addr, frag->size);
#if defined(CONFIG_NOT_COHERENT_CACHE) 
			flush_dcache_range((u32) addr,
					(u32) addr + frag->size);
#endif 
			ext_msg++;
		}
		/* Terminate all remaining pointers */
		for ( ; i < 4; i++) {
			ext_msg->NxtBufDataLength = 0x7800;
			ext_msg++;
		}
	} else {
#if !defined(ENABLE_ELL)
		printk(KERN_ERR "Extended link list not enable\n");
		priv_dev->estats.tx_dropped++;
		dev_kfree_skb(skb);
		return NETDEV_TX_OK;
#else		
		struct apm_enet_msg_ext8 *ext_msg;
		struct apm_enet_msg_ext8 *ext_msg_ll;
		int i, j;
		u32 byte_cnt = 0;
		int ll_cnt = nr_frags - 3;
		u32 *tmp;
		if (unlikely(nr_frags > APM_ENET_MAX_FRAG)) {
			printk(KERN_ERR "fragments exceeding Ethernet HW\n");
			priv_dev->estats.tx_dropped++;
			dev_kfree_skb(skb);
			return NETDEV_TX_OK;
		}
		memset(&msg, 0, sizeof(msg));
		msg16 = &msg.msg32_1.msg16;
		msg8_1 = &msg.msg32_1.msgup8_1;
		ext_msg = (struct apm_enet_msg_ext8*) &msg.msg32_2;
		/* First Fragment */
		msg16->BufDataLen = skb_headlen(skb);
		msg16->DataAddrMSB = (u32) (paddr >> 32);
		msg16->DataAddrLSB = (u32) paddr;
		msg16->UserInfo = (u32) skb;
		/* Its 64B msg */
		msg16->NV = 1;
		/* It has LL */
		msg16->LL = 1;
				
		for (i = 0; i < 3; i++) {
			skb_frag_t *frag = &skb_shinfo(skb)->frags[i];
			void *addr;
			ext_msg->NxtBufDataLength = frag->size;
			addr = page_address(frag->page) + frag->page_offset;
			paddr = virt_to_phys(addr);			
			ext_msg->NxtDataAddrMSB = (u32) (paddr >> 32);
			ext_msg->NxtDataAddrLSB = (u32) paddr;
			ENET_DEBUGTX(
				"NxtBufDataLength:%d, NxtDataAddrLSB:%x\n",
				ext_msg->NxtBufDataLength, 
				ext_msg->NxtDataAddrLSB);
#if defined(CONFIG_NOT_COHERENT_CACHE) 
			flush_dcache_range((u32) addr,
					(u32) addr + frag->size);
#endif 
			ext_msg++;
		}
		ext_msg_ll = kmalloc(ll_cnt * sizeof(struct apm_enet_msg_ext8),
			GFP_KERNEL);
		if (ext_msg_ll == NULL) {
			printk(KERN_ERR 
				"Out of memory for HW Ethernet link list\n");
			priv_dev->estats.tx_dropped++;
			dev_kfree_skb(skb);
			return NETDEV_TX_OK;
		}
		APM_ENET_SKB_CB(skb)->ell = ext_msg_ll;	
		memset(ext_msg_ll, 0, ll_cnt * sizeof(struct apm_enet_msg_ext8));
		/* Info for LL buffers */
		paddr = virt_to_phys(ext_msg_ll);
		ext_msg->NxtDataAddrMSB = (u32) (paddr >> 32);
		ext_msg->NxtDataAddrLSB = (u32) paddr;
		for (j = 0, i = 3; i < nr_frags; i++, j++) {
			skb_frag_t *frag = &skb_shinfo(skb)->frags[i];
			void *addr;
			byte_cnt += frag->size;
			ext_msg_ll->NxtBufDataLength = frag->size;
			addr = page_address(frag->page) + frag->page_offset;
			paddr = virt_to_phys(addr);
			ext_msg_ll->NxtDataAddrMSB = (u32) (paddr >> 32);
			ext_msg_ll->NxtDataAddrLSB = (u32) paddr;
			ENET_DEBUGTX(
				"NxtBufDataLength:%d, NxtDataAddrLSB:%x\n",
				ext_msg_ll->NxtBufDataLength,
				ext_msg_ll->NxtDataAddrLSB);
#if defined(CONFIG_NOT_COHERENT_CACHE) 
			flush_dcache_range((u32) addr,
					(u32) addr + frag->size);
#endif 
			ext_msg_ll++;
		}
		tmp = (u32 *) ext_msg;
		*tmp = (byte_cnt << 12) | (ll_cnt << 4);
		ext_msg_ll -= ll_cnt;
		ENET_QMSG("LL msg ", ext_msg_ll, 8*ll_cnt);
#if defined(CONFIG_NOT_COHERENT_CACHE)
		flush_dcache_range((u32) ext_msg_ll, 
				(u32) ext_msg_ll + 
				ll_cnt*sizeof(struct apm_enet_msg_ext8));
#endif 
#endif
	}
	
	/* Common message Fields */
	msg16->C = apm_enet_pkt_iscoherent();
	msg16->UserInfo = (u32) skb;
	
	msg8_1->HE = 1;
	msg8_1->H0Enq_Num = eth_queues->comp_qid;
	/* Set TYPE_SEL for egress work message */
	if (!priv_dev->priv.crc)
		msg8_1->H0Info_msb = (TYPE_SEL_WORK_MSG << 4) | (TSO_INS_CRC_ENABLE << 3); //XXX there are HW problems tickled by this CRC calculation
	else
		msg8_1->H0Info_msb = (TYPE_SEL_WORK_MSG << 4);

	tx_msg_desc.qid = eth_queues->default_tx_qid;
	tx_msg_desc.msg = &msg;	

	/* Checksum offload processing */
	if (likely(ndev->features & NETIF_F_IP_CSUM) &&
	    likely(skb->protocol == htons(ETH_P_IP))) {
		struct iphdr *iph = ip_hdr(skb);
		int maclen;
		u32 ihl;
		
		if (unlikely(iph->frag_off & htons(IP_MF | IP_OFFSET))) 
			goto xmit;
		
		if (iph->protocol == IPPROTO_TCP) {
			int xhlen;
		
			maclen = eth_hdr_len(skb->data);
			ihl = ip_hdrlen(skb) >> 2;
			xhlen = tcp_hdrlen(skb)/4;
			msg8_1->H0Info_lsb |=
				(xhlen & TSO_TCP_HLEN_MASK) |
				((ihl & TSO_IP_HLEN_MASK) << 6) |
				((maclen & TSO_ETH_HLEN_MASK) << 12) |
				(TSO_CHKSUM_ENABLE << 22) |
				(TSO_IPPROTO_TCP << 24);
			ENET_DEBUGTX("Checksum Offload H0Info 0x%02X.%08X\n",
			    	msg8_1->H0Info_msb, msg8_1->H0Info_lsb);
	
			/* TCP Segmentation offload processing */	
			if (unlikely(ndev->features & NETIF_F_TSO)) {
				u32 mss = skb_shinfo(skb)->gso_size;	
				int mss_len = skb->len - maclen + ihl + xhlen;
				
				if (skb_headlen(skb) < maclen + ihl + xhlen) {
					/* HW requires all header resides
					   in the first buffer */
					printk(KERN_ERR 
						"Un-support header length "
						"location by Ethernet HW\n");
					priv_dev->estats.tx_dropped++;
					dev_kfree_skb(skb);
					return NETDEV_TX_OK;
				}
				if (mss && mss_len > mss) {		
					struct apm_data_priv *priv;

					priv = &priv_dev->priv;			
					if (mss != priv_dev->mss) {
						apm_enet_mss(priv, mss);
						priv_dev->mss = mss;							
					}
					msg8_1->H0Info_lsb |=
						((TSO_MSS_SZ0 & TSO_MSS_MASK) << 20) |
						((TSO_ENABLE & TSO_ENABLE_MASK) << 23);
			
					ENET_DEBUG_TSO(
						"TCP TSO H0Info 0x%02X.%08X",
						msg8_1->H0Info_msb, 
						msg8_1->H0Info_lsb);
				}
			}
		} else if (iph->protocol == IPPROTO_UDP) {
			maclen = eth_hdr_len(skb->data);
			ihl = (ip_hdrlen(skb) >> 2);
			msg8_1->H0Info_lsb |=
				(UDP_HDR_SIZE & TSO_TCP_HLEN_MASK) |
				((ihl & TSO_IP_HLEN_MASK) << 6) |
				((maclen & TSO_ETH_HLEN_MASK) << 12) |
				(TSO_CHKSUM_ENABLE << 22) |
				(TSO_IPPROTO_UDP << 24);
			ENET_DEBUGTX("Checksum Offload H0Info 0x%02X.%08X",
			msg8_1->H0Info_msb, msg8_1->H0Info_lsb);
		}
	}
xmit:
	/* Flush here as it is referenced multiple place */
#if defined(CONFIG_NOT_COHERENT_CACHE)
	if (likely(nr_frags == 0))
		flush_dcache_range((u32) skb->data, 
				(u32) skb->data + skb->len);
	else
		flush_dcache_range((u32) skb->data, 
				(u32) skb->data + skb_headlen(skb));
#endif
	ENET_DEBUGRXTX("TX CQID %d QID %d len %d\n",
		msg8_1->H0Enq_Num, tx_msg_desc.qid, msg16->BufDataLen);
	ENET_QMSG("TX msg ", tx_msg_desc.msg, msg16->NV ? 64 : 32);

#if !defined (NOTIFICATION_OFF) && !defined (CONFIG_SLAB_HW)
	APM_ENET_SKB_CB(skb)->port_id = port_id;
#endif
	apm_qm_push_msg(&tx_msg_desc);
	
	ndev->trans_start = jiffies;
	++priv_dev->stats.tx_packets;
	priv_dev->stats.tx_bytes += skb->len;

#if !defined (NOTIFICATION_OFF) && !defined (CONFIG_SLAB_HW)
	if (unlikely(atomic_inc_return(&tx_completion_counter[port_id]) > APM_COMPQ_THRES_HI)) {
		if (!netif_queue_stopped(priv_dev->ndev)) {
			netif_stop_queue(priv_dev->ndev);
		}
	}

#elif defined(CONFIG_SLAB_HW)
#if defined(FAM_BUF_POOL)
#else
#if defined(ENABLE_ELL)
	if (APM_ENET_SKB_CB(skb)->ell != NULL)
		kfree(APM_ENET_SKB_CB(skb)->ell);
#endif			
	if (is_eth_fp == 1 || is_ppc_fp == 1) 
		free_skb_control(skb); /* HW is freeing the data part */
	else
		FREE_SKB(skb);
#endif
#elif defined(NOTIFICATION_OFF)
#if defined(ENABLE_ELL)
	if (APM_ENET_SKB_CB(skb)->ell != NULL)
		kfree(APM_ENET_SKB_CB(skb)->ell);
#endif
	FREE_SKB(skb);
#endif

	return NETDEV_TX_OK;
}

#if !defined (NOTIFICATION_OFF) && !defined (CONFIG_SLAB_HW)
static void apm_enet_tx_completion_tasklet(unsigned long data)
{
	struct apm_qm_mb_tasklet *tx_completion =
			(struct apm_qm_mb_tasklet *)data;
	u32 port_id;
	u32 mailbox = tx_completion->mailbox;

	ENET_DEBUGRXTX("Checking completion queue %d mailbox %d\n",
	       tx_completion->queue, tx_completion->mailbox);
	apm_qm_pull_tx_comp_msg(mailbox);

	for (port_id = 0; port_id < MAX_PORTS; port_id++) {
		if (atomic_read(&tx_completion_counter[port_id]) < APM_COMPQ_THRES_LO) {
			struct apm_enet_dev_base *priv_dev = netdev_priv(devices[port_id]);
			if (likely(priv_dev) && netif_queue_stopped(priv_dev->ndev)) {
			    /* && likely(priv_dev->ndev->dev.power.status != DPM_OFF)) {*/
				netif_wake_queue(priv_dev->ndev);
			}
		}
	}

	apm_qm_enable_mb_irq(mailbox);
}
#endif

/* Process received frame */
#if !defined (CONFIG_SLAB_HW) && !defined (CONFIG_DRIVER_POOL)
static inline int apm_enet_rx_frame(struct apm_qm_msg_desc *rx_msg_desc)
{
	struct apm_qm_msg32 *msg = (struct apm_qm_msg32 *) rx_msg_desc->msg;
	struct apm_qm_msg16 *msg16 = &msg->msg16;
	struct sk_buff *skb;
	struct apm_enet_dev_base *priv_dev;
	struct apm_enet_msg_ext8 *ext_msg;
	u32 data_len;
	int i;
#if !defined(CONFIG_SMP) || defined(SMP_LOAD_BALANCE)
	u32 core_id = apm_processor_id();
#else
	u32 core_id = 0;		/* SMP - non-load balanced */
#endif
	/* Save variables locally from MailBox as we will lose the slot */
	u8 NV = msg16->NV;
	u32 BufDataLen = msg16->BufDataLen;
	u8 LErr = msg16->LErr;
	u32 UserInfo = msg16->UserInfo;
#ifdef ENET_DBGRXTX
	u32 DataAddrMSB = msg16->DataAddrMSB;
	u32 DataAddrLSB = msg16->DataAddrLSB;
#endif	

	ENET_DEBUGRXTX("RX frame QID %d Rtype %d\n", 
		rx_msg_desc->qid, msg16->RType);
	
	priv_dev = qid2priv[rx_msg_desc->qid];
#ifdef ENET_CHK
	if (priv_dev == NULL) {
		printk(KERN_ERR "private ptr for QID %d is NULL\n", 
			rx_msg_desc->qid);
		return -1;
	}
#endif
	
	/* Check for error, if packet received with error */
	/* FIXME - Work around to handle IP fragmented packets.
	   Use CLE instead to bypass HW checksum.
	*/
	if (unlikely((LErr) && LErr != 7)) {
		/* Parse error */
		apm_enet_parse_error(LErr);
		printk(KERN_ERR "ENET error code %d QID %d\n", 
			LErr, rx_msg_desc->qid);
		if (LErr == ENET_ING_CRC_ERR)
			goto err_refill_all;
		print_hex_dump(KERN_ERR, "QM Msg: ",
			DUMP_PREFIX_ADDRESS, 16, 4, msg, NV ? 64 : 32, 1);
		return -1;
	}
	
#ifdef ENET_CHK
	if (UserInfo == 0) {
		printk(KERN_ERR "ENET UserInfo is NULL for QID %d\n", rx_msg_desc->qid);
		return 0;
	}
#endif

	skb = (struct sk_buff *) UserInfo;

	/* prefetch data in cache */
	prefetch(skb->data - priv_dev->hdr_rsvd);
#if APM_ENET_SKB_RESERVE_SIZE > NET_IP_ALIGN
	prefetch(skb->data - NET_IP_ALIGN);
#endif

#ifdef ENET_CHK
	if (skb->head == NULL || skb->data == NULL) {
		ENET_DEBUG_ERR("PACKET CORRUPTION\n");
		return 0;
	}
#endif 	

	ENET_DEBUGRXTX("RX port %d SKB data VADDR: 0x%p PADDR: 0x%02X_%08X\n", 
		priv_dev->port_id, skb->data, DataAddrMSB, DataAddrLSB);

#ifdef CONFIG_INET_OFFLOAD
 	/* Check for CLE IPV4 Forwarding detection (MAGIC KEY1) */
	if (rx_msg_desc->is_msg16 == 0 && msg->msgup8_1.H0Info_msb == 0xcc) {
		struct ipv4_frwd_hdr {
			u16 da16;
			u32 da32;
			u16 sa16;
			u32 sa32;
			u8  res[10];
			u8  ttl;
			u8  protocol;
		} __attribute__ ((packed));
		struct ipv4_frwd_hdr *skb_data, *mac_data;

		/* CLE MAGIC KEY2 plus port number (0 or 1) to forward to */
		int port = msg->msgup8_2.H0Info_msb & 0x1;
		struct net_device *ndev = devices[port];
		struct apm_enet_dev_base *tx_priv_dev = netdev_priv(ndev);
		u32 tx_qid = tx_priv_dev->qm_queues[core_id].default_tx_qid;

		skb_data = (struct ipv4_frwd_hdr *) skb->data;
		mac_data = (struct ipv4_frwd_hdr *) ndev->dev_addr;
		/* Setting DA as per h0_info and h1_info */
		skb_data->da16 = (u16)msg->msgup8_1.H0Info_lsb;
		skb_data->da32 = (u32)msg->msgup8_2.H0Info_lsb;
		/* Setting SA as interface 'port' MAC Address */
		skb_data->sa16 = (u16)mac_data->da16;
		skb_data->sa32 = (u32)mac_data->da32;
		/* Decrementing TTL by 1*/
		skb_data->ttl--;
		if (unlikely(skb_data->protocol != IPPROTO_TCP &&
			skb_data->protocol != IPPROTO_UDP))
			ip_send_check((struct iphdr *)&skb_data->res[2]);

		msg16->BufDataLen = apm_qm_less_bufdatalen(msg16->BufDataLen, 4, &data_len);
		apm_enet_start_xmit_ipfwd(tx_qid, msg16, skb_data);

		++priv_dev->stats.rx_packets;
		priv_dev->stats.rx_bytes += data_len;

		ndev->trans_start = jiffies;
		++tx_priv_dev->stats.tx_packets;
		tx_priv_dev->stats.tx_bytes += data_len;

		return 0;
	}
#endif

	if (skb->len != 0) {
		printk(KERN_ERR "Corrupted QM msg skb 0x%p len %d\n", 
			skb, skb->len);
		print_hex_dump(KERN_INFO, "QM msg:",
			DUMP_PREFIX_ADDRESS, 16, 4, msg, NV ? 64 : 32, 1);	
		return 0;
	}
	
	if (likely(!NV)) {
		/* Strip off CRC as HW isn't doing this */ 
		data_len = apm_enet_rx_decode_len(BufDataLen) - 4;
		skb_put(skb, data_len);
		ENET_DEBUGRX("RX port %d SKB len %d\n", 
			priv_dev->port_id, data_len);
	} else {
		/* Multiple fragment */
		data_len = apm_enet_rx_decode_len(BufDataLen);
		skb_put(skb, data_len);
		ENET_DEBUGRX("RX port %d SKB multiple len %d\n", 
			priv_dev->port_id, data_len);

		/* Handle fragments */
		ext_msg = (struct apm_enet_msg_ext8 *) &msg[1];
		for (i = 0; i < 4 && 
			apm_enet_rx_decode_len(ext_msg->NxtBufDataLength); 
		     i++) {
			struct page *tmp_page;
			void *data;
			int frag_size;
			
			tmp_page = alloc_page(GFP_ATOMIC);
			if (!tmp_page) {
				kfree_skb(skb);	
				goto err_refill;
			}
			frag_size = apm_enet_rx_decode_len(
						ext_msg->NxtBufDataLength);
			if (i >= 3 || 
			    apm_enet_rx_decode_len(ext_msg[1].NxtBufDataLength) == 0)
				/* Strip off CRC as HW does not handle this */ 
				frag_size -= 4;
			data = phys_to_virt(
					((u64) ext_msg->NxtDataAddrMSB << 32) | 
					ext_msg->NxtDataAddrLSB);	
			memcpy(page_address(tmp_page), data, frag_size);
			skb_add_rx_frag(skb, i, tmp_page, 0, frag_size, PAGE_SIZE);
			ext_msg++;
		}
	}
	ENET_DUMP("RX ", (u8 *) skb->data, data_len);

	skb->dev = priv_dev->ndev;

	apm_enet_cfg_buf_address(priv_dev->qm_queues[core_id].rx_fp_qid, 
				APM_PKT_BUF_SIZE, priv_dev->hdr_rsvd);
	skb->protocol = eth_type_trans(skb, priv_dev->ndev);

#if 0
	/* Jumbo not support yet */	
	if (unlikely(NV)) {
		ext_msg = (struct apm_enet_msg_ext8 *) &msg[1];
		for (i = 0; i < 4 && 
			apm_enet_rx_decode_len(ext_msg->NxtBufDataLength); 
		     i++) {
			apm_enet_refill_fp(
				priv_dev->qm_queues[core_id].rx_fp_qid, 
				APM_PKT_BUF_SIZE,  
				ext_msg->NxtDataAddrMSB,
				ext_msg->NxtDataAddrLSB);
			ext_msg++;
		}
	}
#endif
	
#if defined(IPV4_RX_CHKSUM) || defined(IPV4_TSO)
	if (likely(priv_dev->flags & FLAG_RX_CSUM_ENABLED) &&
	    likely(skb->protocol == htons(ETH_P_IP))) {
		struct iphdr *iph = (struct iphdr *) skb->data;
		if (likely(!(iph->frag_off & htons(IP_MF | IP_OFFSET))) ||
	    	    likely(iph->protocol != IPPROTO_TCP && 
		    iph->protocol != IPPROTO_UDP)) {
			skb->ip_summed = CHECKSUM_UNNECESSARY;
		}
	}
#endif

#ifdef CONFIG_CLE_BR_EXT_FWD
	/* Check for Bridge Forwarding detection magic key */
	if (rx_msg_desc->is_msg16 == 0 && msg->msgup8_1.H0Info_msb == CLE_BR_MAGIC1) {
		if (msg->msgup8_2.H0Info_msb == CLE_BR_MAGIC2 ) {
			if (((apm_cle_br_fwd_directly
				(skb, rx_msg_desc))) == NETDEV_TX_OK) {
				return 0;
			}
		}
	}
#endif

	if (unlikely(in_irq())) {
		printk(":::Shouldn't have come here:::Giving"
		       "frame to stack in IRQ\n");
#ifdef DRIVER_LOOPBACK
		apm_enet_start_xmit(skb, ndev);
#if defined(NOTIFICATION_OFF)
		FREE_SKB(skb);
#endif
#elif defined DRIVER_DROP
		FREE_SKB(skb);
		++priv_dev->estats.rx_dropped_stack; 
#else
		ENET_DEBUGRXTX("Calling netif_rx\n");
		if (unlikely(netif_rx(skb) == NET_RX_DROP)) {
			++priv_dev->estats.rx_dropped_stack; 
		}
#endif
	} else {
#ifdef DRIVER_LOOPBACK
		apm_enet_start_xmit(skb, ndev);
#if defined(NOTIFICATION_OFF)
		FREE_SKB(skb);
#endif
#elif defined DRIVER_DROP
		FREE_SKB(skb);
		++priv_dev->estats.rx_dropped_stack;
#else
		netif_receive_skb(skb);
#endif
	}
	++priv_dev->stats.rx_packets;
	priv_dev->stats.rx_bytes += data_len;
	
	return 0;
	
err_refill_all:
	/* Retrieve skb address */
	skb = (struct sk_buff *) UserInfo;
	if (skb != NULL)
		FREE_SKB(skb);
	apm_enet_cfg_buf_address(priv_dev->qm_queues[core_id].rx_fp_qid, 
				APM_PKT_BUF_SIZE, priv_dev->hdr_rsvd);

err_refill:
#if 0
	/* Jumbo not support yet */	
	/* Multiple fragment */
	if (NV) {
		ext_msg = (struct apm_enet_msg_ext8 *) &msg[1];
		for (i = 0; i < 4 && 
			apm_enet_rx_decode_len(ext_msg->NxtBufDataLength); 
		     i++) {
			apm_enet_refill_fp(
				priv_dev->qm_queues[core_id].rx_fp_qid, 
				APM_PKT_BUF_SIZE,  
				ext_msg->NxtDataAddrMSB,
				ext_msg->NxtDataAddrLSB);
			ext_msg++;
		}
	}
#endif
	return -1;	
}
#endif

int apm_enet_rx_irq(struct apm_qm_msg_desc *rx_msg_desc)
{
	struct apm_enet_dev_base *priv_dev;

	priv_dev = qid2priv[rx_msg_desc->qid];
#ifdef ENET_CHK
	if (priv_dev == NULL) {
		printk(KERN_ERR "Context is NULL for QID %d MBoxID %d\n", 
			rx_msg_desc->qid, rx_msg_desc->mb_id);
		return -1;
	}
#endif

#ifdef CONFIG_NAPI
	ENET_DEBUGRXTX("Rx Frame interface %d in NAPI\n", priv_dev->port_id);
	
	/* Disable interrupts for this port */
	ENET_DEBUGRXTX("RX disable interrupt MBID %d\n",
		   rx_msg_desc->mb_id);
	apm_qm_disable_mb_irq(rx_msg_desc->mb_id);

	priv_dev->in_poll_rx_msg_desc.mb_id = rx_msg_desc->mb_id;
	priv_dev->in_poll_rx_msg_desc.qid = rx_msg_desc->qid;

	/* Tell system we have work to be done. */
	napi_schedule(&priv_dev->napi);
	return 1;
#else
	return apm_enet_rx_frame(rx_msg_desc);
#endif
}

static void apm_enet_reset_task(struct work_struct *work)
{
	struct apm_enet_dev_base *priv_dev = container_of(work, struct apm_enet_dev_base, reset_task);
	struct apm_data_priv *priv = &priv_dev->priv;
	struct net_device *ndev = priv_dev->ndev;

	priv_dev->estats.tx_wd_timeouts++;
	apm_gmac_full_reset(priv);
	ndev->netdev_ops->ndo_stop(ndev);
	ndev->netdev_ops->ndo_open(ndev);
}

static void apm_enet_timeout(struct net_device *ndev)
{
	struct apm_enet_dev_base *priv_dev = netdev_priv(ndev);
	schedule_work(&priv_dev->reset_task);
}

/* Will be called whenever the device will change state to 'Up' */
static int apm_enet_open(struct net_device *ndev)
{
	struct apm_enet_dev_base *priv_dev = netdev_priv(ndev);
	struct apm_data_enet_dev *enet_dev =
		(struct apm_data_enet_dev *)priv_dev;
	struct apm_data_priv *priv = &priv_dev->priv;
	u32 port_id = priv_dev->port_id;
	u32 speed;

	/* Initialize Eth subsystem if this is master core for this port */
	if (!enet_dev->hw_config)
		goto init_done;
	
	ENET_DEBUG("Master core config for port %d\n", priv_dev->port_id);
	if (apm_enet_dev_struct.master_cfg) {
		/* Interrupts are not per port, its per block */
		if ((request_irq(enet_dev->enet_err_irq, apm_enet_err_irq,
				IRQF_SHARED, ndev->name, ndev)) != 0) 
			printk(KERN_ERR "Failed to reg Enet Error IRQ %d\n",
			       enet_dev->enet_err_irq);
	
		if ((request_irq(enet_dev->enet_mac_err_irq,
				apm_enet_mac_err_irq, IRQF_SHARED,
				ndev->name, ndev)) != 0) 
			printk(KERN_ERR "Failed to reg Enet MAC Error"
				"IRQ %d\n",
				enet_dev->enet_mac_err_irq);
		
		if ((request_irq(enet_dev->enet_qmi_err_irq,
				apm_enet_qmi_err_irq,
				IRQF_SHARED, ndev->name, ndev)) != 0) 
			printk(KERN_ERR "Failed to reg Enet QMI Error"
				"IRQ %d\n",
				enet_dev->enet_qmi_err_irq);
	}

	/* Start Preclassifier Engine for this port */
	ENET_DEBUG("Start Preclassifier for port %d\n", port_id);
	apm_preclass_switch_tree(port_id, CLE_PTREE_DEFAULT, 0);

	/* Unmask interrupts */
	apm_enet_unmask_int(priv);

init_done:
#ifdef CONFIG_NAPI
	napi_enable(&priv_dev->napi);
#endif

	netif_start_queue(ndev);

	/* All system init is done for this port, initialize the MAC */
	ENET_DEBUG("Initialize MAC\n" );
	speed = apm_get_link_speed(priv);
	priv_dev->link_status = apm_get_phy_link_status(priv);
	if (!priv_dev->link_status)
		apm_enet_dev_struct.link_down_ref++;
	else {
		printk(KERN_INFO "%s Link is up with speed %d Mbps\n", 
			ndev->name, speed);
		netif_carrier_on(ndev);
	}
	apm_gmac_init(priv, ndev->dev_addr, speed, HW_MTU(ndev->mtu), priv->crc);
	if (priv_dev->ipg > 0)
		apm_gmac_set_ipg(priv, priv_dev->ipg); 	

#ifdef ENET_LINK_POLL
	/* Start Link status poll timer */
	if (!(apm_enet_dev_struct.link_poll_ref++)) 
		apm_enet_start_link_poll();
#endif 	
	return 0;
}

static int apm_enet_close(struct net_device *ndev)
{
	struct apm_enet_dev_base *priv_dev = netdev_priv(ndev);
	struct apm_data_enet_dev *enet_dev =
		(struct apm_data_enet_dev *) priv_dev;

	apm_enet_link_down(ndev);

#ifdef CONFIG_NAPI
	napi_disable(&priv_dev->napi);
#endif
	if (apm_enet_dev_struct.master_cfg) {
		free_irq(enet_dev->enet_err_irq, ndev);
		free_irq(enet_dev->enet_mac_err_irq, ndev);
		free_irq(enet_dev->enet_qmi_err_irq, ndev);
	}
	
#ifdef ENET_LINK_POLL	
	apm_enet_dev_struct.link_poll_ref -= 1;
	if (!(apm_enet_dev_struct.link_poll_ref)) 
		del_timer(&apm_enet_dev_struct.link_poll_timer);
#endif 
	return 0;
}

unsigned int find_port(char *device)
{
	int i;

	for(i = 0;i < MAX_PORTS; i++){
		if(!strcmp(device, devices[i]->name))
			return i;
	}
	return -1;
}

struct net_device *find_netdev(int port_id)
{

	if (port_id > MAX_PORTS)
		return NULL;

	return devices[port_id];
}

struct apm_data_priv * get_priv(void * dev_handle)
{
	struct net_device *ndev = (struct net_device *) dev_handle;
	struct apm_enet_dev_base *priv_dev = netdev_priv(ndev);
	return &priv_dev->priv;
}

static int apm_enet_qconfig(struct apm_enet_dev_base *priv_dev,
			int is_smp, int core_id, int port)
{
	struct apm_qm_qstate *comp_qstate;
	struct qm_cfg_qconfig qconfig;
	struct qm_cfg_qgroup  qgroup;
	unsigned long priv_data = (unsigned long)priv_dev;

	u16 hw_port_id = (port == GIGE_PORT0) ? IP_ETH0 : IP_ETH1;
	u16 core = (is_smp) ? 0 : core_id;
	int rc = 0;

	/* TODO: It default sets the tx_idx and rx_idx to zero */
	memset(priv_dev->qm_queues, 0,
	    	sizeof(struct eth_queue_ids) * MAX_CORES);
	apm_enet_qm_cfg_register(priv_data, port);

	for ( ; core < MAX_CORES; core++) {
		/* Allocate EGRESS work queues from PPCx to ETHx*/
		memset(&qconfig, QM_CFG_INVALID, sizeof(struct qm_cfg_qconfig));
		qconfig.ip    = QM_CFG_IP_QM;
		qconfig.dev   = hw_port_id;
		qconfig.ppc   = core;
		qconfig.dir   = QM_CFG_DIR_EGRESS;
		qconfig.qsize = QM_CFG_QSIZE_16KB;
		qconfig.thr   = 1;
		qconfig.qtype = QM_CFG_QTYPE_PQ;

		rc = apm_enet_add_qconfig(priv_data, &qconfig);
		if (rc != 0) {
			goto done;
		}

		if (qconfig.qtype == QM_CFG_QTYPE_VQ) {
			priv_dev->qm_queues[core].default_tx_qid =
				priv_dev->qm_queues[core].tx[0].qsel[0];
		} else {
			priv_dev->qm_queues[core].default_tx_qid =
				priv_dev->qm_queues[core].tx[0].qid;
		}

		/* Allocate INGRESS work queue from ETHx to PPCx */
		memset(&qconfig, QM_CFG_INVALID, sizeof(struct qm_cfg_qconfig));
		qconfig.ip    = QM_CFG_IP_QM;
		qconfig.dev   = hw_port_id;
		qconfig.ppc   = core;
		qconfig.dir   = QM_CFG_DIR_INGRESS;
		qconfig.qsize = QM_CFG_QSIZE_16KB;
		qconfig.thr   = 1;
		qconfig.qtype = QM_CFG_QTYPE_PQ;

		rc = apm_enet_add_qconfig(priv_data, &qconfig);
		if (rc != 0) {
			goto done;
		}

		if (qconfig.qtype == QM_CFG_QTYPE_VQ) {
			priv_dev->qm_queues[core].default_rx_qid =
				priv_dev->qm_queues[core].rx[0].qsel[0];
		} else {
			priv_dev->qm_queues[core].default_rx_qid =
				priv_dev->qm_queues[core].rx[0].qid;
		}

#ifdef PKT_LOSSLESS_WOL
		/* Allocate one ingress queue for WoL */
		if (port == GIGE_PORT0) {
			memset(&qconfig, QM_CFG_INVALID,
				sizeof(struct qm_cfg_qconfig));
			qconfig.ip    = QM_CFG_IP_QM;
			qconfig.dev   = QM_CFG_DEV_IP_IPP;
			qconfig.ppc   = core;
			qconfig.dir   = QM_CFG_DIR_INGRESS;
			qconfig.qsize = QM_CFG_QSIZE_16KB;
			qconfig.thr   = 1;
			qconfig.qtype = QM_CFG_QTYPE_PQ;

			if ((rc = qm_cfg_add_qconfig(&qconfig)) != 0) {
				printk(KERN_ERR "Error: could not allocate "
					"WoL ingress queues for "
					"ETH%d PPC=%d\n", port, core);
				goto done;
			}

			apm_enet_dev_struct.wol_qid = qconfig.qid;
		}
#endif
		/* Get completion queue info for ETHx to PPCx */
		comp_qstate = get_comp_queue(hw_port_id, core);
		priv_dev->qm_queues[core].comp_qid = comp_qstate->q_id;
		priv_dev->qm_queues[core].comp_mb = comp_qstate->mb_id;

		/* Save this for reverse lookup */
		qid2priv[comp_qstate->q_id] = priv_dev;

		/* Disable interrupts for completion queue mailboxes */
		apm_qm_disable_mb_irq(priv_dev->qm_queues[core].comp_mb);

		/* Allocate free pool for ETHx from PPCx */
		memset(&qconfig, QM_CFG_INVALID, sizeof(struct qm_cfg_qconfig));
		qconfig.ip    = QM_CFG_IP_QM;
		qconfig.dev   = hw_port_id;
		qconfig.ppc   = core;
		qconfig.dir   = QM_CFG_DIR_EGRESS;
		qconfig.qsize = QM_CFG_QSIZE_16KB;
		qconfig.thr   = 1;
		qconfig.qtype = QM_CFG_QTYPE_FP;

		if ((rc = qm_cfg_add_qconfig(&qconfig)) != 0) {
			printk(KERN_ERR "Error: could not allocate "
				"free pool for ETH%d PPC=%d\n", port, core);
			goto done;
		}

		priv_dev->qm_queues[core].rx_fp_qid = qconfig.qid;
		priv_dev->qm_queues[core].rx_fp_pbn = qconfig.pbn;
		priv_dev->qm_queues[core].rx_fp_mbid = qconfig.mbox;

		/* Allocate HW free pool for ETHx from PPCx */
		memset(&qconfig, QM_CFG_INVALID, sizeof(struct qm_cfg_qconfig));
		qconfig.ip    = QM_CFG_IP_QM;
		qconfig.dev   = hw_port_id;
		qconfig.ppc   = core;
		qconfig.dir   = QM_CFG_DIR_EGRESS;
		qconfig.qsize = QM_CFG_QSIZE_16KB;
		qconfig.thr   = 1;
		qconfig.qtype = QM_CFG_QTYPE_FP;

		if ((rc = qm_cfg_add_qconfig(&qconfig)) != 0) {
			printk(KERN_ERR "Error: could not allocate "
				"HW free pool for ETH%d PPC=%d\n", port, core);
			goto done;
		}

		priv_dev->qm_queues[core].hw_fp_qid = qconfig.qid;
		priv_dev->qm_queues[core].hw_fp_pbn = qconfig.pbn;

#if defined(CONFIG_SLAB_HW)
		if (port == GIGE_PORT0) {
			eth0_fp_id =  qconfig.qid;
			qid2priv[eth0_fp_id] = priv_dev;
			DEBG_HW_POOL("Got ETH0 free pool QID: %d\n", 
				eth0_fp_id);
			/* Allocate free pool for PPC0 from ETH */
			memset(&qconfig, QM_CFG_INVALID,
				sizeof(struct qm_cfg_qconfig));
			qconfig.ip    = QM_CFG_IP_QM;
			qconfig.dev   = QM_CFG_DEV_IP_PPC0;
			qconfig.ppc   = core;
			qconfig.dir   = QM_CFG_DIR_INGRESS;
			qconfig.qsize = QM_CFG_QSIZE_16KB;
			qconfig.thr   = 1;
			qconfig.qtype = QM_CFG_QTYPE_FP;

			if ((rc = qm_cfg_add_qconfig(&qconfig)) != 0) {
				printk(KERN_ERR "Error: could not allocate "
					"free pool for PPC%d\n", core);
				goto done;
			}

			ppc_fp_mbid = qconfig.mbox;
			ppc_fp_id = qconfig.qid;
			DEBG_HW_POOL("Got PPC%d free pool QID: %d MB ID: %d\n", 
				ppc_fp_id, ppc_fp_mbid);
			/* disable interrupts for ppc fp queue mailboxe */
			apm_qm_disable_mb_irq(ppc_fp_mbid);
		} else if (core == 0) {
			eth1_fp_id = qconfig.qid;
			qid2priv[eth1_fp_id] = priv_dev;
			DEBG_HW_POOL("Got ETH1 free pool QID: %d\n",
				eth1_fp_id);
		}
#endif		
	       ENET_DBG_Q("Port %d CPU%d CQID %d CMB %d FP %d FP PBN %d\n",
			port, core, 
			priv_dev->qm_queues[core].comp_qid,
			priv_dev->qm_queues[core].comp_mb, 
			priv_dev->qm_queues[core].rx_fp_qid,
			priv_dev->qm_queues[core].rx_fp_pbn);

		if (priv_dev->qm_queues[core].rx[0].qtype == QM_CFG_QTYPE_VQ) {
			int i;
			int count = priv_dev->qm_queues[core].rx[0].qcount;

			ENET_DBG_Q("RX VQ QID %d RX MB %d\n",
				priv_dev->qm_queues[core].rx[0].qid,
				priv_dev->qm_queues[core].rx[0].mbox);

			for (i = 0; i <  count; i++) {
				ENET_DBG_Q("RX QSEL%i QID %d\n", i,
				priv_dev->qm_queues[core].rx[0].qsel[i]);
			}
		} else {
			ENET_DBG_Q("RX QID %d RX MB %d\n",
				priv_dev->qm_queues[core].rx[0].qid,
				priv_dev->qm_queues[core].rx[0].mbox);
		}

		if (priv_dev->qm_queues[core].tx[0].qtype == QM_CFG_QTYPE_VQ) {
			int i;
			int count = priv_dev->qm_queues[core].tx[0].qcount;

			ENET_DBG_Q("TX VQ QID %d\n",
				priv_dev->qm_queues[core].tx[0].qid);

			for (i = 0; i <  count; i++) {
				ENET_DBG_Q("TX QSEL%i QID %d\n", i,
				priv_dev->qm_queues[core].tx[0].qsel[i]);
			}
		} else {
			ENET_DBG_Q("TX QID %d\n",
				priv_dev->qm_queues[core].tx[0].qid);
		}
			
		strcpy(qgroup.name,"default\0");
		qgroup.dev  = hw_port_id;
		qgroup.ppc  = core;
		qgroup.eqid = priv_dev->qm_queues[core].default_tx_qid;
		qgroup.iqid = priv_dev->qm_queues[core].default_rx_qid;
		qgroup.fqid = priv_dev->qm_queues[core].rx_fp_qid;
		qgroup.hqid = priv_dev->qm_queues[core].hw_fp_qid;
		qgroup.cqid = priv_dev->qm_queues[core].comp_qid;

		qm_cfg_add_qgroup(&qgroup);

#if !defined (NOTIFICATION_OFF) && !defined (CONFIG_SLAB_HW)
		apm_qm_enable_mb_irq(priv_dev->qm_queues[core].comp_mb);
#endif

		if (!is_smp) 
			break;
	}
done:
	return rc;
}

#ifdef CONFIG_NAPI
int apm_enet_poll(struct napi_struct *napi, int budget)
{
	struct apm_enet_dev_base *priv_dev =
	    container_of(napi, struct apm_enet_dev_base, napi);
	struct apm_qm_msg_desc in_poll_rx_msg_desc;
	int cnt = 0;
	struct apm_qm_msg64 msg;

	memcpy(&in_poll_rx_msg_desc,
	       &priv_dev->in_poll_rx_msg_desc, sizeof(struct apm_qm_msg_desc));
	in_poll_rx_msg_desc.msg = &msg;

	while(cnt < budget) {
		ENET_DEBUG("Retrieving more frames in NAPI poll\n"); 
		if (unlikely (apm_qm_pull_msg(&in_poll_rx_msg_desc) == -1)) {
			/* No more messages, enable int and return */
			napi_complete(napi);
			apm_qm_enable_mb_irq(in_poll_rx_msg_desc.mb_id);
			return cnt;
		} 
		apm_enet_rx_frame(&in_poll_rx_msg_desc);

		cnt++;
	};

	return cnt;
}
#endif

struct net_device_stats *apm_enet_stats(struct net_device *ndev)
{
	struct apm_enet_dev_base *priv_dev = netdev_priv(ndev);
	struct apm_data_priv *priv = &(priv_dev->priv);
	struct net_device_stats *nst = &priv_dev->nstats;
	struct apm_emac_stats *st = &priv_dev->stats;
	struct eth_detailed_stats detailed_stats;
	struct eth_rx_stat *rx_stats;
	struct eth_tx_stats *tx_stats;

	memset(&detailed_stats, 0, sizeof(struct eth_detailed_stats));

	rx_stats = &detailed_stats.rx_stats;
	tx_stats = &detailed_stats.tx_stats;

	local_irq_disable();
	
	nst->rx_packets = st->rx_packets;
	nst->rx_bytes = st->rx_bytes;
	nst->tx_packets = st->tx_packets;
	nst->tx_bytes = st->tx_bytes;

	apm_gmac_get_detailed_stats(priv, &detailed_stats);
	
	nst->rx_dropped += rx_stats->rx_drop_pkt_count;
	nst->tx_dropped += tx_stats->tx_drop_frm_count;
	nst->rx_errors += rx_stats->rx_fcs_err_count +
		rx_stats->rx_alignment_err_pkt_count +
		rx_stats->rx_frm_len_err_pkt_count +
		rx_stats->rx_code_err_pkt_count +
		rx_stats->rx_carrier_sense_err_pkt_count +
		rx_stats->rx_undersize_pkt_count + 
		rx_stats->rx_oversize_pkt_count;
	
	nst->tx_errors += tx_stats->tx_fcs_err_frm_count +
		tx_stats->tx_oversize_frm_count + 
		tx_stats->tx_undersize_frm_count;
	
	local_irq_enable();
	
	return (nst);
}

int apm_enet_set_mac_addr(struct net_device *ndev, void *p)
{
	struct apm_enet_dev_base *priv_dev = netdev_priv(ndev);
	struct apm_data_priv *priv = &(priv_dev->priv);
	struct sockaddr *addr = p;

	if (netif_running(ndev))
		return -EBUSY;

	memcpy(ndev->dev_addr, addr->sa_data, ndev->addr_len);
	apm_gmac_set_gmac_addr(priv, (unsigned char *)(ndev->dev_addr));
	return 0;
}

/* net_device_ops structure for data path ethernet */
static const struct net_device_ops apm_dnetdev_ops = {
	.ndo_open		= apm_enet_open,
	.ndo_stop		= apm_enet_close,
	.ndo_start_xmit		= apm_enet_start_xmit,
	.ndo_do_ioctl		= apm_enet_ioctl,
	.ndo_tx_timeout		= apm_enet_timeout,
	.ndo_get_stats		= apm_enet_stats,
	.ndo_change_mtu		= apm_enet_change_mtu,
	.ndo_set_mac_address	= apm_enet_set_mac_addr,
};

static int apm_enet_scu_init(int dev_id, int hw_cfg) 
{
	struct clk *clk;
	int rc = 0;
	static int init_done = 0;
	struct apm86xxx_pll_ctrl eth_pll;
	
	if (hw_cfg && !init_done) {
		/* Enable Ethernet PLL */
		eth_pll.clkf_val = 0x13;
		eth_pll.clkod_val = 0;
		eth_pll.clkr_val = 0;
		eth_pll.bwadj_val = 0x13;
		if ( !(enable_eth_pll(&eth_pll))) 
			apm86xxx_write_scu_reg(SCU_SOCDIV4_ADDR, 0x00040004);
		init_done = 1;
	}
	if (dev_id == GIGE_PORT0) {
		/* reset Eth0 */
		clk = clk_get(NULL, "enet0");
		if (IS_ERR(clk))
			return -ENODEV;
		clk_enable(clk);

		/* reset CLE0 */
		clk = clk_get(NULL, "cle0");
		if (IS_ERR(clk))
			return -ENODEV;
		clk_enable(clk);
	} else if (dev_id == GIGE_PORT1) {
		/* reset Eth1 */
		clk = clk_get(NULL, "enet1");
		if (IS_ERR(clk))
			return -ENODEV;
		clk_enable(clk);

		/* reset CLE1 */
		clk = clk_get(NULL, "cle1");
		if (IS_ERR(clk))
			return -ENODEV;
		clk_enable(clk);
	}
	return rc;
}

static int fwload_callback(struct notifier_block *self, unsigned long action,
				void *dev)
{
	int ret;
	struct ipp_net_stats netstats;

	if (action != IPP_FIRMWARE_LOADED)
		goto _ret_fwload_callback;

	/* Firmware loading complete */
	ret = get_ipp_features();
#ifdef CONFIG_APM_ENET_LRO
	if (ret & IPP_LRO_FW_MASK) {
		int i;

		for (i = 0; i < MAX_PORTS; i++) {
			struct apm_enet_dev_base *priv_dev;
			struct net_device *dev;

			if ((dev = find_netdev(i)) == NULL)
				continue;

			priv_dev = netdev_priv(dev);
			if (apm_enet_lro_init(priv_dev) == 0) {
#ifdef CONFIG_NAPI
				napi_enable(&priv_dev->lro_napi);
				netif_napi_add(ndev, &priv_dev->lro_napi, 
					apm_enet_lro_poll, 16); /* pallas: 512 is crazy */
#endif
			}
		}
	} else {
		printk("LRO feature is disabled in SlimPRO firmware\n");
	}
#endif

#ifdef CONFIG_APM_ENET_SLIMPRO_IPFW
	if (ret & IPP_NATOFFLOAD_FW_MASK) {
		int i;

		for (i = 0; i < MAX_PORTS; i++) {
			struct apm_enet_dev_base *priv_dev;
			struct net_device *dev;

			if ((dev = find_netdev(i)) == NULL)
				continue;

			priv_dev = netdev_priv(dev);
			apm_enet_slimpro_ipfw_init(priv_dev);
		}
	}
#endif

	if (get_ipp_netstats(&netstats) == 0)
		apm_enet_dev_struct.ipp_hw_mtu = netstats.hw_mtu;
	else
		apm_enet_dev_struct.ipp_hw_mtu = IPP_PKT_BUF_SIZE;
	apm_enet_dev_struct.ipp_hw_mtu = apm_enet_dev_struct.ipp_hw_mtu > 9600 ?
					9600 : apm_enet_dev_struct.ipp_hw_mtu;

	/* Send QMLite Init Msg */
	ret = ipp_send_user_msg(IPP_CONFIG_SET_HDLR,
				IPP_QMLITE_INIT_VAR,
				IPP_QMLITE_INIT_START,
				IPP_MSG_CONTROL_URG_BIT,
				IPP_MSG_PARAM_UNUSED);
	if (ret < 0) {
		printk(KERN_ERR "Failed to send QMLite "
				"Init Msg to IPP: %d\n", ret);
	}

	if (apm_cle_system_id != CORE_0)
		goto _ret_fwload_callback;

	ret = apm_preclass_init_wol_inter_tree(GIGE_PORT0);

	if (ret == APM_RC_ERROR) {
		printk("Preclass Wol Intermediate Tree init error\n");
		goto _ret_fwload_callback;
	}

	/* Set snptr0 for Wake-On-LAN */
	ret = ipp_send_user_msg(IPP_CONFIG_SET_HDLR,
				IPP_ENET0_CLE0_SNPTR0, (u8)ret,
				IPP_MSG_CONTROL_URG_BIT,
				IPP_MSG_PARAM_UNUSED);
	if (ret < 0) {
		printk(KERN_ERR "Failed to send SNPTR0 "
				"value to IPP: %d\n", ret);
	}

_ret_fwload_callback:
	return NOTIFY_OK;
}

static struct notifier_block fwload_nb = {
	.notifier_call = fwload_callback,
};

static int __devinit apm_enet_of_probe(struct platform_device *pdev)
{
	const u8 *ethaddr;
	const u32 *dev_id, *phy_id;
	int rc = 0;
	int dev_id_len, mac_addr_len;
	u32 enet_irq = 0, enet_mac_irq = 0, enet_qmi_irq = 0;
	u32 *hw_cfg, hw_cfg_len;
	u32 *mas_cfg, mas_cfg_len;
	u32 csr_addr_size;
	u32 port_addr = 0;
	u64 port_addr_p = 0;
	u32 port_id;
	u32 core_id = apm_processor_id();
	struct net_device *ndev;
	struct apm_enet_dev_base *priv_dev;
	struct apm_data_enet_dev *enet_dev;
	struct apm_data_priv *priv;
	struct device_node *np = pdev->dev.of_node;
	struct resource res;
	u32 *node_val;
	u32 node_len;
	u32 enet_mii_access_base_addr;
	u64 enet_gbl_base_addr_p;
	u32 enet_gbl_base_addr;

	ENET_DEBUG("Probing data path Ethernet device\n");

	mas_cfg = (u32 *) of_get_property(np, "master-cfg", &mas_cfg_len);
	if (mas_cfg == NULL) 
		apm_enet_dev_struct.master_cfg = 1;
	else
		apm_enet_dev_struct.master_cfg = *mas_cfg;
      
	/* Retrieve Enet Port CSR register address and size */
	if ((rc = of_address_to_resource(np, 0, &res)) != 0) {
		printk(KERN_ERR "Unable to retrive Enet csr addr from DTS\n");
		return rc;
	}
	port_addr_p = res.start;
	csr_addr_size = RES_SIZE(&res);
	port_addr = (u32) ioremap_nocache(port_addr_p, csr_addr_size);
	ENET_DEBUG("CSR PADDR: 0x%010llx VADDR: 0x%08X\n", 
		port_addr_p, port_addr);
		
	/* Retrieve Classifier CSR register address and size */
	if ((rc = of_address_to_resource(np, 1, &res)) != 0) {
		printk(KERN_ERR "Unable to retrive Classifier csr"
			"addr from DTS\n");
		return rc;
	}
	apm_class_base_addr_p = res.start;
	csr_addr_size = RES_SIZE(&res);
	apm_class_base_addr = (u32) ioremap_nocache(apm_class_base_addr_p,
						   csr_addr_size);
	ENET_DEBUG("Classifier PADDR: 0x%010llx VADDR: 0x%08X\n",
		apm_class_base_addr_p, apm_class_base_addr);

	/* Retrieve Enet Global CSR register address and size */
	if ((rc = of_address_to_resource(np, 2, &res)) != 0) {
		printk(KERN_ERR 
			"Unable to retrive Enet Global csr addr from DTS\n");
		return rc;
	}

	enet_gbl_base_addr_p = res.start;
	csr_addr_size = RES_SIZE(&res);
	enet_gbl_base_addr = (u32) ioremap_nocache(enet_gbl_base_addr_p,
						   csr_addr_size);
	
	ENET_DEBUG("Enet Global PADDR: 0x%010llx VADDR: 0x%08X\n",
		   enet_gbl_base_addr_p, enet_gbl_base_addr);

	/* Retrieve Enet MII accsess register address and size */
	if ((rc = of_address_to_resource(np, 3, &res)) != 0) {
		printk(KERN_ERR "Unable to retrive Enet MII access addr"
			"from DTS\n");
		return rc;
	}

	enet_mii_access_base_addr = (u32) ioremap_nocache(res.start,
							  RES_SIZE(&res));
	
	ENET_DEBUG("Enet MII Access PADDR: 0x%010llx  VADDR: 0x%08X\n",
		   res.start, enet_mii_access_base_addr);

	if (apm_enet_dev_struct.master_cfg) {
		/* Retrieve ENET Error IRQ number */
		if ((enet_irq = of_irq_to_resource(np, 0, NULL)) == NO_IRQ) {
			printk(KERN_ERR "Unable to retrive ENET Error"
			       "IRQ number from DTS\n");
			return -EINVAL;
		}
		ENET_DEBUG("Enet Error IRQ no: 0x%x\n", enet_irq);
		
		/* Retrieve ENET MAC Error IRQ number */
		if ((enet_mac_irq = of_irq_to_resource(np, 1,
							NULL)) == NO_IRQ) {
			printk(KERN_ERR "Unable to retrive ENET MAC Error IRQ"
				" number from DTS\n");
			return -EINVAL;
		}
		ENET_DEBUG("Enet MAC Error IRQ no: 0x%x\n", enet_mac_irq);
		
		/* Retrieve ENET QMI Error IRQ number */
		if ((enet_qmi_irq = of_irq_to_resource(np, 2,
					NULL)) == NO_IRQ) {
			printk(KERN_ERR "Unable to retrive ENET QMI Error IRQ"
				" number from DTS\n");
			return -EINVAL;
		}
		ENET_DEBUG("Enet QMI Error IRQ no: 0x%x\n", enet_qmi_irq);
	}
	
	ndev = alloc_etherdev(sizeof(struct apm_data_enet_dev));

	if (!ndev) {
		printk(KERN_ERR "Not able to allocate memory for netdev\n");
		return -ENOMEM;
	}

	priv_dev = (struct apm_enet_dev_base *) netdev_priv(ndev);
	priv_dev->ndev = ndev;
	priv_dev->pdev = pdev;
	priv_dev->node = pdev->dev.of_node;
	priv_dev->q_poll_timer_status = Q_POLL_TIMER_OFF;
	priv_dev->link_status = 0;

	INIT_WORK(&priv_dev->reset_task, apm_enet_reset_task);

	SET_NETDEV_DEV(ndev, &pdev->dev);
	dev_set_drvdata(&pdev->dev, priv_dev);

	enet_dev = (struct apm_data_enet_dev *) priv_dev;
	priv = &priv_dev->priv;

	priv->enet_write32 = apm_enet_wr32;
	priv->enet_read32 = apm_enet_rd32;

	priv->paddr_base = enet_gbl_base_addr_p;
	priv->vaddr_base = enet_gbl_base_addr;
	priv->ppaddr_base = port_addr_p;
	priv->vpaddr_base = port_addr;

	/* Per Port Indirect access */
	priv->mac_base_addr_v  = port_addr + BLOCK_ETH_MAC_OFFSET;
	priv->stats_base_addr_v = port_addr + BLOCK_ETH_STATS_OFFSET;
	/* Global direct access	 */
	priv->eth_gbl_base_addr_v =
		enet_gbl_base_addr + BLOCK_ETH_GBL_OFFSET;
	priv->eth_ffdiv_base_addr_v =
		enet_gbl_base_addr + BLOCK_ETH_FFDIV_OFFSET;
	priv->mac_gbl_base_addr_v  =
		enet_gbl_base_addr + BLOCK_ETH_MAC_GBL_OFFSET;
	priv->eth_ptp_base_addr_v =
		enet_gbl_base_addr + BLOCK_ETH_PTP_OFFSET  ;
	priv->eth_unisec_base_addr_v =
		enet_gbl_base_addr + BLOCK_ETH_UNISEC_OFFSET;
	priv->eth_diag_base_addr_v =
		enet_gbl_base_addr + BLOCK_ETH_DIAG_OFFSET;
	priv->eth_qmi_base_addr_v =
		enet_gbl_base_addr + BLOCK_ETH_QMI_SLAVE_OFFSET ;
	priv->enet_mii_base_addr_v = enet_mii_access_base_addr;

	/* Retrieve Device ID for this port */
	dev_id = (u32 *) of_get_property(np, "devid", &dev_id_len);
	if (dev_id == NULL || dev_id_len < sizeof(u32) ||
	    *dev_id >= MAX_PORTS) {
		printk(KERN_ERR "No device ID or invalid value in DTS\n");
		return -EINVAL;
	}
	port_id = *dev_id;

	/* Retrieve PHY ID for this port */
	phy_id = (u32 *) of_get_property(np, "phyid", &dev_id_len);
	if (phy_id == NULL || dev_id_len < sizeof(u32) ||
	    *phy_id > 0x1F) {
		printk(KERN_ERR "No phy ID or invalid value in DTS\n");
		return -EINVAL;
	}

	node_val = (u32 *) of_get_property(np, "rx-fifo-cnt", &node_len);
	if (node_val != NULL && node_len >= sizeof(u32) &&  
	    *node_val >= 32 && *node_val <= 512) 
		priv_dev->rx_buff_cnt = *node_val;
	else		 
		priv_dev->rx_buff_cnt = APM_NO_PKT_BUF;
	
	node_val = (u32 *) of_get_property(np, "hdr-rsvd", &node_len);
	if (node_val != NULL && node_len >= sizeof(u32) &&  
	    *node_val >= 0 && *node_val <= 512) 
		priv_dev->hdr_rsvd = *node_val;
	else		 
		priv_dev->hdr_rsvd = APM_ENET_SKB_RESERVE_SIZE;
	
	node_val = (u32 *) of_get_property(np, "ipg", &node_len);
	if (node_val != NULL && node_len >= sizeof(u32)) 
		priv_dev->ipg = *node_val;
	else		 
		priv_dev->ipg = 0;
	
	hw_cfg = (u32 *) of_get_property(np, "hw-cfg", &hw_cfg_len);
	if (hw_cfg == NULL) {
		enet_dev->hw_config = 1;
		apm_enet_dev_struct.is_smp = 1;
	} else {
		enet_dev->hw_config = *hw_cfg;
	}

	if ((rc = apm_enet_scu_init(port_id, enet_dev->hw_config)) != 0) {
		printk(KERN_ERR "Error in apm_enet_scu_init \n");
		return rc;
	}
	
	/* Enable Enet CLK, Reset Enet and CLE */
	if (apm_enet_dev_struct.is_smp || !core_id) {
		/* Remove ENET CSR memory shutdown */
		if ((rc = apm86xxx_disable_mem_shutdown(
		     	(u32 __iomem *)(priv->eth_diag_base_addr_v +
			    	ENET_CFG_MEM_RAM_SHUTDOWN_ADDR),
			     	ENET0_F2_MASK)) != 0) {
			printk(KERN_ERR
				"Failed to remove Eth CSR memory shutdown\n");
			return rc;
		}
		/* Remove CLE CSR memory shutdown */
		if ((rc = apm86xxx_disable_mem_shutdown(
		     	(u32 __iomem *)(apm_class_base_addr +
			     	APM_GLBL_DIAG_OFFSET +
			     	CLE_CFG_MEM_RAM_SHUTDOWN_ADDR),
			CLE_MASK)) != 0) {
			printk(KERN_ERR 
				"Failed to remove Classifier CSR "
				"memory shutdown\n");
			return rc;
		}
	}
		
	/* Fill in the driver function table */
	ndev->netdev_ops = &apm_dnetdev_ops;

#ifdef CONFIG_NAPI
	netif_napi_add(ndev, &priv_dev->napi, apm_enet_poll, 16); /* pallas: 512 is crazy */
#endif

#if defined(IPV4_TX_CHKSUM) || defined(IPV4_TSO)
	/* Enable TX IPV4 TCP/UDP HW checksum */
	ndev->features |= NETIF_F_IP_CSUM | NETIF_F_SG; 
#endif
#if defined(IPV4_RX_CHKSUM) || defined(IPV4_TSO)
	/* Enable RX IPV4 TCP/UDP HW checksum */
	priv_dev->flags |= FLAG_RX_CSUM_ENABLED;
#endif
#if defined(IPV4_TSO)
	ndev->features |= NETIF_F_TSO;
#endif
	priv_dev->mss = DEFAULT_TCP_MSS;

	SET_ETHTOOL_OPS(ndev, &apm_ethtool_ops);
#ifdef CONFIG_INET_OFFLOAD
	SET_ETHOFFLOAD_OPS(ndev, &apm_ethoffload_ops);
	apm_ethoffload_ops.ethoffload_interfaces++;
#endif

	priv->port = port_id;
	priv->phy_addr = *phy_id;
	priv->crc = 1;
	privinfo[port_id] = priv;
	priv_dev->port_id = port_id;
	
	/* disable Ethernet interface */
	apm_gmac_tx_disable(priv);
	apm_gmac_rx_disable(priv);

	if ((rc = register_netdev(ndev)) != 0) {
		printk(KERN_ERR "enet%d: failed to register net dev(%d)!\n",
		       priv_dev->port_id, rc);
		return rc;
	}
	
	ethaddr = (u8 *) of_get_property(np, "local-mac-address",
					 &mac_addr_len);
	if (ethaddr == NULL || mac_addr_len < ETH_ALEN)
		printk(KERN_ERR "Can't get Device MAC address\n");
	else
		memcpy(ndev->dev_addr, ethaddr, ETH_ALEN);
	
	devices[port_id] = ndev;
	ENET_DEBUG("%s: emac%d MAC %02x:%02x:%02x:%02x:%02x:%02x\n",
	       ndev->name, priv_dev->port_id,
	       ndev->dev_addr[0], ndev->dev_addr[1], ndev->dev_addr[2],
	       ndev->dev_addr[3], ndev->dev_addr[4], ndev->dev_addr[5]);

	/* QM configuration */
	if ((rc = apm_enet_qconfig(priv_dev, apm_enet_dev_struct.is_smp, core_id, port_id)) != 0) {
		printk(KERN_ERR "Error in QM configuration\n");
		return rc;
	}
	priv_dev->irq_en = 1;

	/* Configure free pool */
	if (apm_enet_dev_struct.is_smp) {
#ifdef SMP_LOAD_BALANCE
		apm_enet_init_fp(
			priv_dev->qm_queues[port_id % MAX_CORES].rx_fp_qid, 
			APM_PKT_BUF_SIZE, priv_dev->rx_buff_cnt, 0, 
			priv_dev->hdr_rsvd);
#else
		apm_enet_init_fp(priv_dev->qm_queues[0].rx_fp_qid, 
			APM_PKT_BUF_SIZE, priv_dev->rx_buff_cnt, 0,
			priv_dev->hdr_rsvd);
#endif
#ifdef PKT_LOSSLESS_WOL		
		/* configure intermediate free pool */
		apm_enet_init_fp(ENET_RX_FQ_TO_DDR, APM_PKT_BUF_SIZE, 10, 1,
				priv_dev->hdr_rsvd);
#endif		
	} else {
		apm_enet_init_fp(priv_dev->qm_queues[core_id].rx_fp_qid, 
			APM_PKT_BUF_SIZE, priv_dev->rx_buff_cnt, 0,
			priv_dev->hdr_rsvd);
#ifdef PKT_LOSSLESS_WOL		
		/* configure intermediate free pool */
		if (port == 0 && !is_apm86xxx_lite())
			apm_enet_init_fp(ENET_RX_FQ_TO_DDR, APM_PKT_BUF_SIZE, 
					10, 1, priv_dev->hdr_rsvd);
#endif		
	}

	apm_enet_init_fp(priv_dev->qm_queues[core_id].hw_fp_qid, 
		APM_PKT_BUF_SIZE, APM_HW_PKT_BUF, 0,
		priv_dev->hdr_rsvd);

	/* Initialize the core subsystem if not done already */
	if (apm_enet_dev_struct.master_cfg) {
		enet_dev->enet_err_irq = enet_irq;
		enet_dev->enet_mac_err_irq = enet_mac_irq;
		enet_dev->enet_qmi_err_irq = enet_qmi_irq;
	}

	if (!(apm_enet_dev_struct.enet_config_done)) {
		/* Register QM callback function for Ethernet */
		apm_qm_msg_rx_register(APM_QM_ETH_RTYPE, apm_enet_rx_frame);
#if !defined (NOTIFICATION_OFF) && !defined (CONFIG_SLAB_HW)
		for (rc = 0; rc < MAX_PORTS; rc++)
			atomic_set(&tx_completion_counter[rc], 0);
#ifdef CONFIG_SMP
		for (rc = 0; rc < MAX_CORES; rc++) {
			apm_qm_mb_tasklet_register(
				priv_dev->qm_queues[rc].comp_mb,
				priv_dev->qm_queues[rc].comp_qid,
				rc, tx_completion_counter,
				apm_enet_tx_completion_tasklet);
		}
#else
		apm_qm_mb_tasklet_register(
			priv_dev->qm_queues[core_id].comp_mb,
			priv_dev->qm_queues[core_id].comp_qid,
			core_id, tx_completion_counter,
			apm_enet_tx_completion_tasklet);
#endif
#endif
		/* For AMP Systems, we are partitioning cle resoures */
#ifndef CONFIG_SMP
		apm_cle_system_id = CORE_0;
		apm_cle_systems = 1;
#else
		apm_cle_system_id = mfspr(SPRN_PIR);
		apm_cle_systems = 1;
		for (rc = CORE_1; rc < MAX_SYSTEMS; rc++)
			if (cpu_enabled(rc))
				apm_cle_systems++;
#endif
		apm_enet_dev_struct.enet_config_done = 1;
	}

	apm_enet_add_sysfs(pdev->dev.driver);

	apm_gmac_access_phy(priv);
	rc = apm_get_link_speed(priv);
	apm_gmac_init(priv, ndev->dev_addr, rc, HW_MTU(ndev->mtu), priv->crc);

	/* Set InterPacket Gap if greater than 0 (existed in DTS) */
	if (priv_dev->ipg > 0)
		apm_gmac_set_ipg(priv, priv_dev->ipg); 	

	/* Register iPP Firmware Notifier */
	if (!ipp_notifier_registered) {
		register_ipp_fwload_notifier(&fwload_nb);
		ipp_notifier_registered = 1;
	}

	/* Disable Rx and Tx */ 
	apm_gmac_rx_disable(priv);
	apm_gmac_tx_disable(priv);

	/* Initialize PreClassifier Tree for this port */
	ENET_DEBUG("Initialize Preclassifier Tree for port %d core %d\n",
		port_id, core_id);
#ifdef SMP_LOAD_BALANCE
	apm_preclass_init(port_id, &priv_dev->qm_queues[port_id % MAX_CORES]);
#else
	if (apm_enet_dev_struct.is_smp)
		apm_preclass_init(port_id, &priv_dev->qm_queues[0]);
	else 
		apm_preclass_init(port_id, &priv_dev->qm_queues[core_id]);
#endif

	priv_dev->in_poll_rx_msg_desc.is_msg16 = 0;

#ifdef CONFIG_PM
	/* Init List for storing Ethernet RX MSG before going to suspend */
	INIT_LIST_HEAD(&priv_dev->head);
#endif
	return 0;
}

/* Called when module is unloaded */
static int __devexit apm_enet_of_remove(struct platform_device *pdev)
{
	struct apm_enet_dev_base *priv_dev = NULL;
	struct apm_data_enet_dev *enet_dev;
	struct apm_data_priv *priv;
	struct net_device *ndev;
	int i;
	static int fini_done = 0;

	if (!fini_done) {
		apm_enet_remove_sysfs(pdev->dev.driver);
		ENET_DEBUG("Unloading Ethernet driver\n");
		for (i = 0; i < MAX_PORTS; i++) {
			ndev = devices[i];
			if(!ndev)
				continue;

			priv_dev = netdev_priv(ndev);
			enet_dev = (struct apm_data_enet_dev *) priv_dev;
			priv = &priv_dev->priv;

			/* Stop any traffic and disable MAC */
			apm_gmac_rx_disable(priv);
			apm_gmac_tx_disable(priv);

			/* Release IRQ */
			if (apm_enet_dev_struct.master_cfg) {
				free_irq(enet_dev->enet_err_irq, priv_dev);
				free_irq(enet_dev->enet_mac_err_irq, priv_dev);
				free_irq(enet_dev->enet_qmi_err_irq, priv_dev);
			}
			/* Unregister netdev in kernel and free it */
			unregister_netdev(devices[i]);
			free_netdev(devices[i]);
			
#if !defined (CONFIG_SLAB_HW) 
			for (i = 0; i < MAX_CORES; i++) {
				int j;
				for (j = 0; j < MAX_PORTS; j++)
					if (priv_dev->qm_queues[i].rx_fp_qid > 0)
						apm_enet_deinit_fp(
							priv_dev->qm_queues[i].rx_fp_qid);
					if (priv_dev->qm_queues[i].hw_fp_qid > 0)
						apm_enet_deinit_fp(
							priv_dev->qm_queues[i].hw_fp_qid);
				}
#ifdef PKT_LOSSLESS_WOL		
			if (i == GIGE_PORT0 && !is_apm86xxx_lite())
				apm_enet_deinit_fp(ENET_RX_FQ_TO_DDR);
#endif		
#endif		
		}

		/* Unregister iPP Firmware Notifier */
		if (ipp_notifier_registered) {
			unregister_ipp_fwload_notifier(&fwload_nb);
			ipp_notifier_registered = 0;
		}
		fini_done = 1;
	}

	return 0;
}

/* Called when system is shutwon */
static void apm_enet_shutdown(struct platform_device *pdev)
{
	int rc = 0;
	struct apm_enet_dev_base *priv_dev;
	struct apm_data_priv *priv;
	int port;
	struct apm_qm_msg_desc rx_msg_desc;
	struct apm_qm_msg32 msg;
	struct eth_queue_ids *queues;
	int core_id = apm_processor_id();
	u32 no_rx_msg = 0;
	u32 data = 0;

#ifdef CONFIG_SMP
	if (!cpu_enabled(1))
		return rc;
#endif 	
	
	ENET_DEBUG("ENET Shutdown Starts... \n");
	memset(&rx_msg_desc, 0, sizeof(struct apm_qm_msg_desc));
	memset(&msg, 0, sizeof(struct apm_qm_msg32));
	rx_msg_desc.msg = &msg;
	
	priv_dev = dev_get_drvdata(&pdev->dev);
	priv = &priv_dev->priv;
	port = priv_dev->port_id;

	queues = &priv_dev->qm_queues[core_id];
	ENET_DEBUG("ENET SHUTDOWN Starts for port: %d...\n", port);
	/* Disable Ethernet Port RX Mac */
	ENET_DEBUG("Disable Enet Port%d Rx Mac \n", port);
	/* Disable Rx */ 
	apm_gmac_rx_disable(priv);
	/* Empty out the Rx Queue */
	/* FIXME: we should check rx_count here and loop through all queues */
	rx_msg_desc.mb_id = queues->rx[queues->rx_idx].mbox;
	rx_msg_desc.qid = queues->rx[queues->rx_idx].qid;
	rx_msg_desc.is_msg16 = 0;
	
	while ((rc = apm_qm_pull_msg(&rx_msg_desc)) == 0) {
		no_rx_msg++;	
	}
	ENET_DEBUG("Emptied: %d Rx Messsage\n", no_rx_msg);
	no_rx_msg = 0;

	do {
		rc = apm_enet_rd32(priv, BLOCK_ETH_QMI_SLAVE,
				   ENET_STSSSQMIWQBUFFER3_ADDR, &data);
		data = (data & 0X0000F000);
	} while (data);

	ENET_DEBUG("Disable Enet Port %d Tx Mac\n", port);
	apm_gmac_tx_disable(priv);

	/* Empty out the Comp Queue */
	memset(&rx_msg_desc, 0, sizeof(struct apm_qm_msg_desc));
	memset (&msg, 0, sizeof(struct apm_qm_msg32));
	rx_msg_desc.msg = &msg;
	
	rx_msg_desc.mb_id = queues->comp_mb;
	rx_msg_desc.qid = queues->comp_qid;
	rx_msg_desc.is_msg16 = 0;
	
	while ((rc = apm_qm_pull_msg(&rx_msg_desc)) == 0) {
		no_rx_msg++;	
	}
	ENET_DEBUG("Emptied: %d Comp Messsage\n", no_rx_msg);
	
	apm_qm_shutdown();

	ENET_DEBUG("Flush Enet Prefetch Buffer: %d\n", 
		queues->rx_fp_pbn - 0x20);
	apm_enet_fp_pb_flush(priv, queues->rx_fp_pbn - 0x20);
	ENET_DEBUG("Flush Enet HW Prefetch Buffer: %d\n", 
		queues->hw_fp_pbn - 0x20);
	apm_enet_fp_pb_flush(priv, queues->hw_fp_pbn - 0x20);
	ENET_DEBUG("ENET Shutdown Ends...\n");
}

static struct of_device_id apm_enet_match[] = {
	{.compatible = "apm, apm86xxx-enet",},
	{},
};

static struct platform_driver apm_enet_of_driver = {
	.driver = {
		.name = "apm86xxx-enet",
		.of_match_table = apm_enet_match,
	},
	.probe = apm_enet_of_probe,
	.remove = apm_enet_of_remove,
	.shutdown = apm_enet_shutdown,
};

static int __init apm_net_init(void)
{
	return platform_driver_register(&apm_enet_of_driver);
}

static void __exit apm_net_exit(void)
{
	platform_driver_unregister(&apm_enet_of_driver);
}

module_init(apm_net_init);
module_exit(apm_net_exit);

MODULE_AUTHOR("Keyur Chudgar <kchudgar@apm.com>");
MODULE_DESCRIPTION("APM862xx SoC Ethernet driver");
MODULE_LICENSE("GPL");
