/*
 * Texas Instruments Ethernet Switch Driver
 *
 * Copyright (C) 2010 Texas Instruments
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License as
 * published by the Free Software Foundation version 2.
 *
 * This program is distributed "as is" WITHOUT ANY WARRANTY of any
 * kind, whether express or implied; without even the implied warranty
 * of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 */
#include <linux/kernel.h>
#include <linux/io.h>
#include <linux/clk.h>
#include <linux/timer.h>
#include <linux/module.h>
#include <linux/platform_device.h>
#include <linux/if_ether.h>
#include <linux/etherdevice.h>
#include <linux/netdevice.h>
#include <linux/phy.h>
#include <linux/workqueue.h>
#include <linux/delay.h>
#include <linux/if_vlan.h>
#include <linux/net_tstamp.h>
#include <linux/ethtool.h>
#include <linux/cpsw.h>
#include <linux/net_switch_config.h>

#include "cpsw_ale.h"
#include "davinci_cpdma.h"

#define CPSW_DEBUG	(NETIF_MSG_HW		| NETIF_MSG_WOL		| \
			 NETIF_MSG_DRV		| NETIF_MSG_LINK	| \
			 NETIF_MSG_IFUP		| NETIF_MSG_INTR	| \
			 NETIF_MSG_PROBE	| NETIF_MSG_TIMER	| \
			 NETIF_MSG_IFDOWN	| NETIF_MSG_RX_ERR	| \
			 NETIF_MSG_TX_ERR	| NETIF_MSG_TX_DONE	| \
			 NETIF_MSG_PKTDATA	| NETIF_MSG_TX_QUEUED	| \
			 NETIF_MSG_RX_STATUS)

#define msg(level, type, format, ...)				\
do {								\
	if (netif_msg_##type(priv) && net_ratelimit())		\
		dev_##level(priv->dev, format, ## __VA_ARGS__);	\
} while (0)

#define CPSW_POLL_WEIGHT	64
#define CPSW_MIN_PACKET_SIZE	60
#define CPSW_MAX_PACKET_SIZE	(1500 + 14 + 4 + 4)
#define CPSW_USE_DEFAULT	0x0afbdce1  /**< Flag to indicate use of a
						default */

#define CPSW_PRIMAP(shift, priority)    (priority << (shift * 4))

#define CPSW_IRQ_QUIRK
#ifdef CPSW_IRQ_QUIRK
#define cpsw_enable_irq(priv)	\
	do {			\
		u32 i;		\
		for (i = 0; i < priv->num_irqs; i++) \
			enable_irq(priv->irqs_table[i]); \
	} while (0);
#define cpsw_disable_irq(priv)	\
	do {			\
		u32 i;		\
		for (i = 0; i < priv->num_irqs; i++) \
			disable_irq_nosync(priv->irqs_table[i]); \
	} while (0);
#else
#define cpsw_enable_irq(priv) do { } while (0);
#define cpsw_disable_irq(priv) do { } while (0);
#endif

#define CPSW_VER_1		0x19010a
#define CPSW_VER_2		0x19010c
#define cpsw_slave_reg(priv, slave, reg)				\
	(((priv)->cpsw_version == CPSW_VER_1) ?				\
	&(((struct cpsw_slave_regs_v1 *)((slave)->regs))->reg) :	\
	&(((struct cpsw_slave_regs_v2 *)((slave)->regs))->reg))

#if defined(CONFIG_VLAN_8021Q) || defined(CONFIG_VLAN_8021Q_MODULE)
#define VLAN_SUPPORT
#endif

#define CPSW_VLAN_AWARE			0x2

#define ALE_ALL_PORTS			0x7

#define PTP_ETHER_TYPE			0x88f7
#define CPTS_VERSION			0x4e8a0101
#define CPTS_TS_PUSH			0x0
#define CPTS_TS_ROLLOVER		(0x1 << 20)
#define CPTS_TS_HROLLOVER		(0x2 << 20)
#define CPTS_TS_HW_PUSH			(0x3 << 20)
#define CPTS_TS_ETH_RX			(0x4 << 20)
#define CPTS_TS_ETH_TX			(0x5 << 20)
#define CPSW_MSG_TYPE_EN		0xffff
#define CPSW_801_1Q_LTYPE		0x88f7
#define CPSW_SEQ_ID_OFS			0x1e

#define CPSW_V1_TS_RX_EN		BIT(0)
#define CPSW_V1_TS_TX_EN		BIT(4)
#define CPSW_V1_MSG_TYPE_OFS		16
#define CPSW_V1_SEQ_ID_OFS_SHIFT	16

#define CPSW_V2_TS_RX_EN		BIT(0)
#define CPSW_V2_TS_TX_EN		BIT(1)
#define CPSW_V2_TS_LTYPE_1_EN		BIT(2)
#define CPSW_V2_TS_LTYPE_2_EN		BIT(3)
#define CPSW_V2_TS_ANNEX_D_EN		BIT(4)
#define CPSW_V2_SEQ_ID_OFS_SHIFT	16

#define CPTS_FIFO_SIZE			32
#define CPTS_READ_TS_MAX_TRY		20
#define CPTL_CLK_FREQ			250000000 /*250MHz*/
#define DEFAULT_CPTS_CLK		CPTS_CLK_SEL_AUDIO
#define NANOSEC_CPTSCOUNT_CONV_SHIFT	2	/*1GHz/250Mhz=4 "1<<2 - 4"*/
#define NANOSEC_TO_CPTSCOUNT(_NS_)	(_NS_ >> NANOSEC_CPTSCOUNT_CONV_SHIFT)
#define CPTSCOUNT_TO_NANOSEC(_NS_)	(_NS_ << NANOSEC_CPTSCOUNT_CONV_SHIFT)

/* CPSW control module masks */
#define CPSW_INTPACEEN		(0x3 << 16)
#define CPSW_INTPRESCALE_MASK	(0x7FF << 0)
#define CPSW_CMINTMAX_CNT	63
#define CPSW_CMINTMIN_CNT	2
#define CPSW_CMINTMAX_INTVL	(1000 / CPSW_CMINTMIN_CNT)
#define CPSW_CMINTMIN_INTVL	((1000 / CPSW_CMINTMAX_CNT) + 1)

#define switchcmd(__cmd__)	((__cmd__)->cmd_data.switchcmd)
#define portcmd(__cmd__)	((__cmd__)->cmd_data.portcmd)
#define priocmd(__cmd__)	((__cmd__)->cmd_data.priocmd)

#ifdef CONFIG_TI_CPSW_DUAL_EMAC
#define cpsw_slave_phy_index(priv)	((priv)->emac_port)
#else
#define cpsw_slave_phy_index(priv)	0
#endif

static int debug_level;
module_param(debug_level, int, 0);
MODULE_PARM_DESC(debug_level, "cpsw debug level (NETIF_MSG bits)");

static int ale_ageout = 10;
module_param(ale_ageout, int, 0);
MODULE_PARM_DESC(ale_ageout, "cpsw ale ageout interval (seconds)");

static int rx_packet_max = CPSW_MAX_PACKET_SIZE;
module_param(rx_packet_max, int, 0);
MODULE_PARM_DESC(rx_packet_max, "maximum receive packet size (bytes)");

struct cpsw_ss_regs {
	u32	id_ver;
	u32	soft_reset;
	u32	control;
	u32	int_control;
	u32	rx_thresh_en;
	u32	rx_en;
	u32	tx_en;
	u32	misc_en;
	u32	mem_allign1[8];
	u32	rx_thresh_stat;
	u32	rx_stat;
	u32	tx_stat;
	u32	misc_stat;
	u32	mem_allign2[8];
	u32	rx_imax;
	u32	tx_imax;
};

struct cpsw_regs {
	u32	id_ver;
	u32	control;
	u32	soft_reset;
	u32	stat_port_en;
	u32	ptype;
	u32	soft_idle;
	u32	thru_rate;
	u32	gap_thresh;
	u32	tx_start_wds;
	u32	flow_control;
	u32	vlan_ltype;
	u32	ts_ltype;
};

struct cpsw_slave_regs_v1 {
	u32	max_blks;
	u32	blk_cnt;
	u32	flow_thresh;
	u32	port_vlan;
	u32	tx_pri_map;
	u32	ts_ctl;
	u32	ts_seq_ltype;
	u32	ts_vlan;
	u32	sa_lo;
	u32	sa_hi;
	/* Dummy resigters */
	u32	port_control;
	u32	ts_control;
	u32	ts_seq_mtype;
};

struct cpsw_slave_regs_v2 {
	u32	port_control;
	u32	ts_control;
	u32	max_blks;
	u32	blk_cnt;
	u32	flow_thresh;
	u32	port_vlan;
	u32	tx_pri_map;
	u32	ts_seq_mtype;
	u32	sa_lo;
	u32	sa_hi;
	/* Dummy resigters */
	u32	ts_ctl;
	u32	ts_seq_ltype;
	u32	ts_vlan;
};

struct cpsw_host_regs {
	u32	max_blks;
	u32	blk_cnt;
	u32	flow_thresh;
	u32	port_vlan;
	u32	tx_pri_map;
	u32	cpdma_tx_pri_map;
	u32	cpdma_rx_chan_map;
};

struct cpsw_sliver_regs {
	u32	id_ver;
	u32	mac_control;
	u32	mac_status;
	u32	soft_reset;
	u32	rx_maxlen;
	u32	__reserved_0;
	u32	rx_pause;
	u32	tx_pause;
	u32	__reserved_1;
	u32	rx_pri_map;
};

struct cpsw_hw_stats {
	u32	rxgoodframes;
	u32	rxbroadcastframes;
	u32	rxmulticastframes;
	u32	rxpauseframes;
	u32	rxcrcerrors;
	u32	rxaligncodeerrors;
	u32	rxoversizedframes;
	u32	rxjabberframes;
	u32	rxundersizedframes;
	u32	rxfragments;
	u32	__pad_0[2];
	u32	rxoctets;
	u32	txgoodframes;
	u32	txbroadcastframes;
	u32	txmulticastframes;
	u32	txpauseframes;
	u32	txdeferredframes;
	u32	txcollisionframes;
	u32	txsinglecollframes;
	u32	txmultcollframes;
	u32	txexcessivecollisions;
	u32	txlatecollisions;
	u32	txunderrun;
	u32	txcarriersenseerrors;
	u32	txoctets;
	u32	octetframes64;
	u32	octetframes65t127;
	u32	octetframes128t255;
	u32	octetframes256t511;
	u32	octetframes512t1023;
	u32	octetframes1024tup;
	u32	netoctets;
	u32	rxsofoverruns;
	u32	rxmofoverruns;
	u32	rxdmaoverruns;
};

struct cpsw_slave {
	void __iomem			*regs;
	struct cpsw_sliver_regs __iomem	*sliver;
	int				slave_num;
	u32				mac_control;
	struct cpsw_slave_data		*data;
	struct phy_device		*phy;
#ifdef CONFIG_TI_CPSW_DUAL_EMAC
	u32				port_vlan;
	struct net_device		*ndev;
	u32				open_stat;
#endif /* CONFIG_TI_CPSW_DUAL_EMAC */
};

/*
 * CPTS Regs
 */
struct cpts_regs {
	u32	id_ver;
	u32	control;
	u32	rftclk_sel;
	u32	ts_push;
	u32	ts_load_val;
	u32	ts_load_en;
	u32	mem_allign1[2];
	u32	intstat_raw;
	u32	intstat_masked;
	u32	int_enable;
	u32	mem_allign2;
	u32	event_pop;
	u32	event_low;
	u32	event_high;
};

/*
 * CPTS Events
 */
struct cpts_time_evts {
	uint32_t event_high;
	u64 ts;
};

struct cpts_evts_fifo {
	struct cpts_time_evts fifo[CPTS_FIFO_SIZE];
	u32 head, tail;
};
/*
 * Time Handle
 */
struct cpts_time_handle {
	struct cpts_evts_fifo tx_fifo;
	struct cpts_evts_fifo rx_fifo;
	bool enable_timestamping;
	int tshi;
	bool first_half;
	int freq;
};

struct cpsw_priv {
	spinlock_t			lock;
	struct platform_device		*pdev;
	struct net_device		*ndev;
	struct resource			*cpsw_res;
	struct resource			*cpsw_ss_res;
	struct napi_struct		napi;
#define napi_to_priv(napi)	container_of(napi, struct cpsw_priv, napi)
	struct device			*dev;
	struct cpsw_platform_data	data;
	struct cpsw_regs __iomem	*regs;
	struct cpsw_ss_regs __iomem	*ss_regs;
	struct cpsw_hw_stats __iomem	*hw_stats;
	struct cpsw_host_regs __iomem	*host_port_regs;

	struct cpts_regs __iomem	*cpts_reg;
	struct cpts_time_handle		*cpts_time;

	u8				port_state[3];
	u32				cpsw_version;
	u32				msg_enable;
	u32				coal_intvl;
	u32				bus_freq_mhz;
	struct net_device_stats		stats;
	int				rx_packet_max;
	int				host_port;
	struct clk			*clk;
	u8				mac_addr[ETH_ALEN];
	struct cpsw_slave		*slaves;
#define for_each_slave(priv, func, arg...)			\
	do {							\
		int idx;					\
		for (idx = 0; idx < (priv)->data.slaves; idx++)	\
			(func)((priv)->slaves + idx, ##arg);	\
	} while (0)
#define slave(priv, idx)		((priv)->slaves + idx)

	struct cpdma_ctlr		*dma;
	struct cpdma_chan		*txch, *rxch;
	struct cpsw_ale			*ale;

#ifdef CPSW_IRQ_QUIRK
	/* snapshot of IRQ numbers */
	u32 irqs_table[4];
	u32 num_irqs;
#endif
#ifdef VLAN_SUPPORT
	struct vlan_group *vlgrp;
#endif /* VLAN_SUPPORT */
#ifdef CONFIG_TI_CPSW_DUAL_EMAC
	u32				emac_port;
#endif /* CONFIG_TI_CPSW_DUAL_EMAC */
};

static int cpsw_set_coalesce(struct net_device *ndev,
			struct ethtool_coalesce *coal);

#if (defined(CONFIG_PTP_1588_CLOCK_CPTS) ||		\
	defined(CONFIG_PTP_1588_CLOCK_CPTS_MODULE))
DEFINE_SPINLOCK(cpts_time_lock);
static struct cpsw_priv *gpriv;
static u64 time_push;

static int cpts_time_evts_fifo_push(struct cpts_evts_fifo *fifo,
				struct cpts_time_evts *evt)
{

	fifo->fifo[fifo->tail].event_high = evt->event_high;
	fifo->fifo[fifo->tail].ts = CPTSCOUNT_TO_NANOSEC(evt->ts);

	fifo->tail++;
	if (fifo->tail >= CPTS_FIFO_SIZE)
		fifo->tail = 0;

	if (fifo->head == fifo->tail) {
		fifo->fifo[fifo->tail].event_high = 0;
		fifo->fifo[fifo->tail].ts = 0;
		fifo->head++;
		if (fifo->head >= CPTS_FIFO_SIZE)
			fifo->head = 0;
	}

	return 0;
}

static int cpts_time_evts_fifo_pop(struct cpts_evts_fifo *fifo,
				u32 evt_high, struct cpts_time_evts *evt)
{
	u32 i, ev_found = 0;

	if (fifo->head == fifo->tail)
		return -1;

	i = fifo->head;
	while (i != fifo->tail) {
		if (evt_high == fifo->fifo[i].event_high) {
			evt->event_high = fifo->fifo[i].event_high;
			evt->ts = fifo->fifo[i].ts;

			fifo->fifo[i].event_high = 0;
			fifo->fifo[i].ts = 0;
			ev_found = 1;
		}
		if (fifo->fifo[i].event_high == 0 && i == fifo->head) {
			fifo->head++;
			if (fifo->head >= CPTS_FIFO_SIZE)
				fifo->head = 0;
		}
		if (ev_found)
			return 0;
		i++;
		if (i >= CPTS_FIFO_SIZE)
			i = 0;
	}
	printk(KERN_DEBUG "cpts_time_evts_fifo_pop() evt %05x not found\n",
			evt_high);
	return 0;
}

static int cpts_isr(struct cpsw_priv *priv)
{
	unsigned long flags;

	while (__raw_readl(&priv->cpts_reg->intstat_raw) & 0x01) {
		u32	event_high;
		u32	event_tslo;
		u64 ts = 0;

		event_high = __raw_readl(&priv->cpts_reg->event_high);
		event_tslo = __raw_readl(&priv->cpts_reg->event_low);
		__raw_writel(0x01, &priv->cpts_reg->event_pop);

		if (priv->cpts_time->first_half &&
				event_tslo & 0x80000000) {
			/* this is misaligned ts */
			ts = (u64)(priv->cpts_time->tshi - 1);
		} else {
			ts = (u64)(priv->cpts_time->tshi);
		}
		ts = (ts << 32) | event_tslo;

		if ((event_high & 0xf00000) == CPTS_TS_PUSH) {
			/*Push TS to Read */
			time_push = ts;
		} else if ((event_high & 0xf00000) == CPTS_TS_ROLLOVER) {
			/* Roll over */
			spin_lock_irqsave(&cpts_time_lock, flags);
			priv->cpts_time->tshi++;
			priv->cpts_time->first_half = true;
			spin_unlock_irqrestore(&cpts_time_lock, flags);
		} else if ((event_high & 0xf00000) == CPTS_TS_HROLLOVER) {
			/* Half Roll Over */
			spin_lock_irqsave(&cpts_time_lock, flags);
			priv->cpts_time->first_half = false;
			spin_unlock_irqrestore(&cpts_time_lock, flags);
		} else if ((event_high & 0xf00000) == CPTS_TS_HW_PUSH) {
			/* HW TS Push */
			printk(KERN_ERR "CPTS_TS_HW_PUSH Not Utilised\n");
			/* TBD */
		} else if ((event_high & 0xf00000) == CPTS_TS_ETH_RX) {
			/* Ethernet Rx Ts */
			struct cpts_time_evts evt;

			evt.event_high = event_high & 0xfffff;
			evt.ts = event_tslo;

			evt.ts = ts;
			if (priv->cpts_time->enable_timestamping) {
				spin_lock_irqsave(&cpts_time_lock, flags);
				cpts_time_evts_fifo_push(
					&(priv->cpts_time->rx_fifo), &evt);
				spin_unlock_irqrestore(&cpts_time_lock, flags);
			}
		} else if ((event_high & 0xf00000) == CPTS_TS_ETH_TX) {
			/* Ethernet Tx Ts */
			struct cpts_time_evts evt;

			evt.event_high = event_high & 0xfffff;
			evt.ts = event_tslo;

			evt.ts = ts;
			if (priv->cpts_time->enable_timestamping) {
				spin_lock_irqsave(&cpts_time_lock, flags);
				cpts_time_evts_fifo_push(
					&(priv->cpts_time->tx_fifo), &evt);
				spin_unlock_irqrestore(&cpts_time_lock, flags);
			}
		} else {
			printk(KERN_ERR "Invalid CPTS Event type...\n");
		}

		cpdma_ctlr_eoi_statistics(priv->dma);
	}
	return 0;
}

int cpts_systime_write(u64 ns)
{
	if (gpriv == NULL) {
		printk(KERN_ERR "Device Error, No device found\n");
		return -ENODEV;
	}
	ns = NANOSEC_TO_CPTSCOUNT(ns);
	__raw_writel((u32)(ns & 0xffffffff), &gpriv->cpts_reg->ts_load_val);
	__raw_writel(0x1, &gpriv->cpts_reg->ts_load_en);
	gpriv->cpts_time->tshi = (u32)(ns >> 32);

	return 0;
}
EXPORT_SYMBOL(cpts_systime_write);

int cpts_systime_read(u64 *ns)
{
	int ret = 0;
	int i;

	if (gpriv == NULL) {
		printk(KERN_ERR "Device Error, No device found\n");
		return -ENODEV;
	}

	time_push = 0;
	__raw_writel(0x1, &gpriv->cpts_reg->ts_push);
	for (i = 0; i < 20; i++) {
		cpts_isr(gpriv);
		if (time_push)
			break;
	}
	if (i >= 20) {
		*ns = 0;
		ret = -EBUSY;
	} else
		*ns = CPTSCOUNT_TO_NANOSEC(time_push);

	return ret;
}
EXPORT_SYMBOL(cpts_systime_read);

static void cpts_rx_timestamp(struct cpsw_priv *priv,
			struct sk_buff *skb, u32 evt_high)
{
	struct cpts_time_evts evt = {0, 0};
	struct skb_shared_hwtstamps *shhwtstamps;
	unsigned long flags;

	spin_lock_irqsave(&cpts_time_lock, flags);
	cpts_time_evts_fifo_pop(&(priv->cpts_time->rx_fifo), evt_high, &evt);
	spin_unlock_irqrestore(&cpts_time_lock, flags);

	shhwtstamps = skb_hwtstamps(skb);
	memset(shhwtstamps, 0, sizeof(*shhwtstamps));
	shhwtstamps->hwtstamp = ns_to_ktime(evt.ts);
}

static void cpts_tx_timestamp(struct cpsw_priv *priv,
			struct sk_buff *skb, u32 evt_high)
{
	struct cpts_time_evts evt = {0, 0};
	struct skb_shared_hwtstamps shhwtstamps;
	unsigned long flags;

	spin_lock_irqsave(&cpts_time_lock, flags);
	cpts_time_evts_fifo_pop(&(priv->cpts_time->tx_fifo), evt_high, &evt);
	spin_unlock_irqrestore(&cpts_time_lock, flags);

	memset(&shhwtstamps, 0, sizeof(struct skb_shared_hwtstamps));
	shhwtstamps.hwtstamp = ns_to_ktime(evt.ts);
	skb_tstamp_tx(skb, &shhwtstamps);
}
#endif

static void cpsw_intr_enable(struct cpsw_priv *priv)
{
	__raw_writel(0xFF, &priv->ss_regs->tx_en);
	__raw_writel(0xFF, &priv->ss_regs->rx_en);

	cpdma_ctlr_int_ctrl(priv->dma, true);
	return;
}

static void cpsw_intr_disable(struct cpsw_priv *priv)
{
	__raw_writel(0, &priv->ss_regs->tx_en);
	__raw_writel(0, &priv->ss_regs->rx_en);

	cpdma_ctlr_int_ctrl(priv->dma, false);
	return;
}

void cpsw_tx_handler(void *token, int len, int status)
{
	struct sk_buff		*skb = token;
	struct net_device	*ndev = skb->dev;
	struct cpsw_priv	*priv = netdev_priv(ndev);
#if (defined(CONFIG_PTP_1588_CLOCK_CPTS) ||		\
		defined(CONFIG_PTP_1588_CLOCK_CPTS_MODULE))
	u32			evt_high = 0;
#endif

	if (unlikely(netif_queue_stopped(ndev)))
		netif_wake_queue(ndev);

#if (defined(CONFIG_PTP_1588_CLOCK_CPTS) ||		\
		defined(CONFIG_PTP_1588_CLOCK_CPTS_MODULE))
	if ((priv->cpts_time->enable_timestamping) &&
			((htons(*((unsigned short *)&skb->data[12]))) ==
			PTP_ETHER_TYPE)) {
		evt_high = (skb->data[14] & 0xf) << 16;
		evt_high |= htons(*((unsigned short *)&skb->data[44]));
		if (unlikely(__raw_readl(&priv->cpts_reg->intstat_raw) & 0x01))
			cpts_isr(priv);
		cpts_tx_timestamp(priv, skb, evt_high);
	}
#endif

	priv->stats.tx_packets++;
	priv->stats.tx_bytes += len;
	dev_kfree_skb_any(skb);
}

void cpsw_rx_handler(void *token, int len, int status)
{
	struct sk_buff		*skb = token;
	struct net_device	*ndev = skb->dev;
	struct cpsw_priv	*priv = netdev_priv(ndev);
	int			ret = 0;
#if (defined(CONFIG_PTP_1588_CLOCK_CPTS) ||		\
		defined(CONFIG_PTP_1588_CLOCK_CPTS_MODULE))
	u32			evt_high = 0;
#endif

#ifdef CONFIG_TI_CPSW_DUAL_EMAC
	if (CPDMA_RX_SOURCE_PORT(status) == 1) {
		ndev = priv->slaves[0].ndev;
		priv = netdev_priv(ndev);
		skb->dev = ndev;
	} else if (CPDMA_RX_SOURCE_PORT(status) == 2) {
		ndev = priv->slaves[1].ndev;
		priv = netdev_priv(ndev);
		skb->dev = ndev;
	}
#endif /* CONFIG_TI_CPSW_DUAL_EMAC */

	/* free and bail if we are shutting down */
	if (unlikely(!netif_running(ndev)) ||
			unlikely(!netif_carrier_ok(ndev))) {
		dev_kfree_skb_any(skb);
		return;
	}

	if (likely(status >= 0)) {
		skb_put(skb, len);

#if (defined(CONFIG_PTP_1588_CLOCK_CPTS) ||		\
	defined(CONFIG_PTP_1588_CLOCK_CPTS_MODULE))
		if ((priv->cpts_time->enable_timestamping) &&
				((htons(*((unsigned short *)&skb->data[12])))
				== PTP_ETHER_TYPE)) {
			evt_high = (skb->data[14] & 0xf) << 16;
			evt_high |= htons(*((unsigned short *)&skb->data[44]));
			if (unlikely(__raw_readl(&priv->cpts_reg->intstat_raw)
						 & 0x01))
				cpts_isr(priv);
			cpts_rx_timestamp(priv, skb, evt_high);
		}
#endif

		skb->protocol = eth_type_trans(skb, ndev);
		netif_receive_skb(skb);
		priv->stats.rx_bytes += len;
		priv->stats.rx_packets++;
		skb = NULL;
	}


	if (unlikely(!netif_running(ndev))) {
		if (skb)
			dev_kfree_skb_any(skb);
		return;
	}

	if (likely(!skb)) {
		skb = netdev_alloc_skb_ip_align(ndev, priv->rx_packet_max);
		if (WARN_ON(!skb))
			return;

		ret = cpdma_chan_submit(priv->rxch, skb, skb->data,
				skb_tailroom(skb), 0, GFP_KERNEL);
	}

	WARN_ON(ret < 0);

}

static irqreturn_t cpsw_interrupt(int irq, void *dev_id)
{
	struct cpsw_priv *priv = dev_id;

#if (defined(CONFIG_PTP_1588_CLOCK_CPTS) ||		\
		defined(CONFIG_PTP_1588_CLOCK_CPTS_MODULE))
	cpts_isr(priv);
#endif /* CONFIG_PTP_1588_CLOCK_CPTS */

	if (likely(netif_running(priv->ndev))) {
		cpsw_intr_disable(priv);
		cpsw_disable_irq(priv);
		napi_schedule(&priv->napi);
	}

#ifdef CONFIG_TI_CPSW_DUAL_EMAC
	else if (likely(netif_running(priv->slaves[1].ndev))) {
		struct cpsw_priv *priv_sl2 = netdev_priv(priv->slaves[1].ndev);
		cpsw_intr_disable(priv_sl2);
		cpsw_disable_irq(priv_sl2);
		napi_schedule(&priv_sl2->napi);
	}
#endif /* CONFIG_TI_CPSW_DUAL_EMAC */

	return IRQ_HANDLED;
}

static int cpsw_poll(struct napi_struct *napi, int budget)
{
	struct cpsw_priv	*priv = napi_to_priv(napi);
	int			num_tx, num_rx;

	num_tx = cpdma_chan_process(priv->txch, 128);
	num_rx = cpdma_chan_process(priv->rxch, budget);

	if (num_rx || num_tx)
		msg(dbg, intr, "poll %d rx, %d tx pkts\n", num_rx, num_tx);

	if (num_rx < budget) {
		napi_complete(napi);
		cpdma_ctlr_eoi(priv->dma);
		cpsw_intr_enable(priv);
		cpsw_enable_irq(priv);
	}

	return num_rx;
}

static inline void soft_reset(const char *module, void __iomem *reg)
{
	unsigned long timeout = jiffies + HZ;

	__raw_writel(1, reg);
	do {
		cpu_relax();
	} while ((__raw_readl(reg) & 1) && time_after(timeout, jiffies));

	WARN(__raw_readl(reg) & 1, "failed to soft-reset %s\n", module);
}

#define mac_hi(mac)	(((mac)[0] << 0) | ((mac)[1] << 8) |	\
			 ((mac)[2] << 16) | ((mac)[3] << 24))
#define mac_lo(mac)	(((mac)[4] << 0) | ((mac)[5] << 8))

static void cpsw_set_slave_mac(struct cpsw_slave *slave,
			       struct cpsw_priv *priv)
{
	writel(mac_hi(priv->mac_addr), cpsw_slave_reg(priv, slave, sa_hi));
	writel(mac_lo(priv->mac_addr), cpsw_slave_reg(priv, slave, sa_lo));
}

static inline u32 cpsw_get_slave_port(struct cpsw_priv *priv, u32 slave_num)
{
	if (priv->host_port == 0)
		return slave_num + 1;
	else
		return slave_num;
}

static void _cpsw_adjust_link(struct cpsw_slave *slave,
			      struct cpsw_priv *priv, bool *link)
{
	struct phy_device	*phy = slave->phy;
	u32			mac_control = 0;
	u32			slave_port;

	if (!phy)
		return;

	slave_port = cpsw_get_slave_port(priv, slave->slave_num);

	if (phy->link) {
		/* enable forwarding */
		cpsw_ale_control_set(priv->ale, slave_port,
			     ALE_PORT_STATE,
			     priv->port_state[slave_port]);

		mac_control = (priv->data.mac_control &
				~(BIT(0) | BIT(7) | BIT(18)));

		if (phy->speed == 10)
			mac_control |= BIT(18);
		else if (phy->speed == 100)
			mac_control |= BIT(15);
		else if (phy->speed == 1000)
			mac_control |= BIT(7);	/* GIGABITEN	*/
		if (phy->duplex)
			mac_control |= BIT(0);	/* FULLDUPLEXEN	*/
		*link = true;
	} else {
		cpsw_ale_control_set(priv->ale, slave_port,
			     ALE_PORT_STATE, ALE_PORT_STATE_DISABLE);
		mac_control = 0;
	}

	if (mac_control != slave->mac_control) {
		phy_print_status(phy);
		__raw_writel(mac_control, &slave->sliver->mac_control);
	}

	slave->mac_control = mac_control;
}

static void cpsw_adjust_link(struct net_device *ndev)
{
	struct cpsw_priv	*priv = netdev_priv(ndev);
	bool			link = false;

#ifdef CONFIG_TI_CPSW_DUAL_EMAC
	_cpsw_adjust_link(&priv->slaves[priv->emac_port], priv, &link);
#else /* CONFIG_TI_CPSW_DUAL_EMAC */
	for_each_slave(priv, _cpsw_adjust_link, priv, &link);
#endif /* CONFIG_TI_CPSW_DUAL_EMAC */

	if (link) {
		netif_carrier_on(ndev);
		if (netif_running(ndev))
			netif_wake_queue(ndev);
	} else {
		netif_carrier_off(ndev);
		netif_stop_queue(ndev);
	}
}

static inline int __show_stat(char *buf, int maxlen, const char* name, u32 val)
{
	static char *leader = "........................................";

	if (!val)
		return 0;
	else
		return snprintf(buf, maxlen, "%s %s %10d\n", name,
				leader + strlen(name), val);
}

static ssize_t cpsw_hw_stats_show(struct device *dev,
				     struct device_attribute *attr,
				     char *buf)
{
	struct net_device	*ndev = to_net_dev(dev);
	struct cpsw_priv	*priv = netdev_priv(ndev);
	int			len = 0;
	struct cpdma_chan_stats	dma_stats;

#define show_stat(x) do {						\
	len += __show_stat(buf + len, SZ_4K - len, #x,			\
			   __raw_readl(&priv->hw_stats->x));		\
} while (0)

#define show_dma_stat(x) do {						\
	len += __show_stat(buf + len, SZ_4K - len, #x, dma_stats.x);	\
} while (0)

	len += snprintf(buf + len, SZ_4K - len, "CPSW Statistics:\n");
	show_stat(rxgoodframes);	show_stat(rxbroadcastframes);
	show_stat(rxmulticastframes);	show_stat(rxpauseframes);
	show_stat(rxcrcerrors);		show_stat(rxaligncodeerrors);
	show_stat(rxoversizedframes);	show_stat(rxjabberframes);
	show_stat(rxundersizedframes);	show_stat(rxfragments);
	show_stat(rxoctets);		show_stat(txgoodframes);
	show_stat(txbroadcastframes);	show_stat(txmulticastframes);
	show_stat(txpauseframes);	show_stat(txdeferredframes);
	show_stat(txcollisionframes);	show_stat(txsinglecollframes);
	show_stat(txmultcollframes);	show_stat(txexcessivecollisions);
	show_stat(txlatecollisions);	show_stat(txunderrun);
	show_stat(txcarriersenseerrors); show_stat(txoctets);
	show_stat(octetframes64);	show_stat(octetframes65t127);
	show_stat(octetframes128t255);	show_stat(octetframes256t511);
	show_stat(octetframes512t1023);	show_stat(octetframes1024tup);
	show_stat(netoctets);		show_stat(rxsofoverruns);
	show_stat(rxmofoverruns);	show_stat(rxdmaoverruns);

	cpdma_chan_get_stats(priv->rxch, &dma_stats);
	len += snprintf(buf + len, SZ_4K - len, "\nRX DMA Statistics:\n");
	show_dma_stat(head_enqueue);	show_dma_stat(tail_enqueue);
	show_dma_stat(pad_enqueue);	show_dma_stat(misqueued);
	show_dma_stat(desc_alloc_fail);	show_dma_stat(pad_alloc_fail);
	show_dma_stat(runt_receive_buff); show_dma_stat(runt_transmit_buff);
	show_dma_stat(empty_dequeue);	show_dma_stat(busy_dequeue);
	show_dma_stat(good_dequeue);	show_dma_stat(teardown_dequeue);

	cpdma_chan_get_stats(priv->txch, &dma_stats);
	len += snprintf(buf + len, SZ_4K - len, "\nTX DMA Statistics:\n");
	show_dma_stat(head_enqueue);	show_dma_stat(tail_enqueue);
	show_dma_stat(pad_enqueue);	show_dma_stat(misqueued);
	show_dma_stat(desc_alloc_fail);	show_dma_stat(pad_alloc_fail);
	show_dma_stat(runt_receive_buff); show_dma_stat(runt_transmit_buff);
	show_dma_stat(empty_dequeue);	show_dma_stat(busy_dequeue);
	show_dma_stat(good_dequeue);	show_dma_stat(teardown_dequeue);

	return len;
}

DEVICE_ATTR(hw_stats, S_IRUGO, cpsw_hw_stats_show, NULL);

#ifdef CONFIG_TI_CPSW_DUAL_EMAC
static int cpsw_common_res_usage_stat(struct cpsw_priv *priv)
{
	u32 i;
	u32 usage_count = 0;

	for (i = 0; i < priv->data.slaves; i++)
		if (priv->slaves[i].open_stat)
			usage_count++;

	return usage_count;
}
#endif /* CONFIG_TI_CPSW_DUAL_EMAC */

static void cpsw_slave_open(struct cpsw_slave *slave, struct cpsw_priv *priv)
{
	char name[32];
	u32 slave_port;

	sprintf(name, "slave-%d", slave->slave_num);

	soft_reset(name, &slave->sliver->soft_reset);

	/* setup priority mapping */
	writel(0x76543210, &slave->sliver->rx_pri_map);
	writel(0x33221100, cpsw_slave_reg(priv, slave, tx_pri_map));

	/* setup max packet size, and mac address */
	writel(priv->rx_packet_max, &slave->sliver->rx_maxlen);
	cpsw_set_slave_mac(slave, priv);

	slave->mac_control = 0;	/* no link yet */

	slave_port = cpsw_get_slave_port(priv, slave->slave_num);

#ifdef CONFIG_TI_CPSW_DUAL_EMAC
	writel(slave->port_vlan, cpsw_slave_reg(priv, slave, port_vlan));
	cpsw_ale_add_vlan(priv->ale, slave->port_vlan,
			1 << slave_port | 1 << priv->host_port, 0,
			1 << slave_port | 1 << priv->host_port, 0);
	cpsw_ale_vlan_add_mcast(priv->ale, priv->ndev->broadcast,
			1 << slave_port | 1 << priv->host_port,
			slave->port_vlan, 0, 0);
	cpsw_ale_vlan_add_ucast(priv->ale, priv->mac_addr, priv->host_port,
			0, slave->port_vlan);
	/* Add VLAN id 0 with untagging to remove VLAN ID 0 on egress */
	cpsw_ale_add_vlan(priv->ale, 0,
			ALE_ALL_PORTS << priv->host_port,
			ALE_ALL_PORTS << priv->host_port, priv->host_port, 0);
#else /* !CONFIG_TI_CPSW_DUAL_EMAC */
	cpsw_ale_add_mcast(priv->ale, priv->ndev->broadcast,
			   1 << slave_port, 0, 0);
#endif /* CONFIG_TI_CPSW_DUAL_EMAC */

	priv->port_state[slave_port] = ALE_PORT_STATE_FORWARD;

	slave->phy = phy_connect(priv->ndev, slave->data->phy_id,
				 &cpsw_adjust_link, 0, slave->data->phy_if);
	if (IS_ERR(slave->phy)) {
		msg(err, ifup, "phy %s not found on slave %d\n",
		    slave->data->phy_id, slave->slave_num);
		slave->phy = NULL;
	} else {
		printk(KERN_ERR "\nCPSW phy found : id is : 0x%x\n",
			slave->phy->phy_id);
		phy_start(slave->phy);
	}
}

static void cpsw_init_host_port(struct cpsw_priv *priv)
{
	/* soft reset the controller and initialize ale */
	soft_reset("cpsw", &priv->regs->soft_reset);
	cpsw_ale_start(priv->ale);

#if defined(VLAN_SUPPORT) || defined(CONFIG_TI_CPSW_DUAL_EMAC)
	/* switch to vlan aware mode */
	cpsw_ale_control_set(priv->ale, priv->host_port, ALE_VLAN_AWARE, 1);
	__raw_writel(CPSW_VLAN_AWARE, &priv->regs->control);
#else /* !VLAN_SUPPORT and !CONFIG_TI_CPSW_DUAL_EMAC */
	/* switch to vlan unaware mode */
	cpsw_ale_control_set(priv->ale, priv->host_port, ALE_VLAN_AWARE, 0);
#endif /* defined(VLAN_SUPPORT) || defined(CONFIG_TI_CPSW_DUAL_EMAC) */

	/* setup host port priority mapping */
	__raw_writel(0x76543210, &priv->host_port_regs->cpdma_tx_pri_map);
	__raw_writel(0, &priv->host_port_regs->cpdma_rx_chan_map);

	cpsw_ale_control_set(priv->ale, priv->host_port,
			     ALE_PORT_STATE, ALE_PORT_STATE_FORWARD);

#ifndef CONFIG_TI_CPSW_DUAL_EMAC
	cpsw_ale_add_ucast(priv->ale, priv->mac_addr, priv->host_port, 0);
	/*ALE_SECURE);*/
	cpsw_ale_add_mcast(priv->ale, priv->ndev->broadcast,
			   1 << priv->host_port, 0, 0);
#endif /* CONFIG_TI_CPSW_DUAL_EMAC */
}

static int cpsw_ndo_open(struct net_device *ndev)
{
	struct cpsw_priv *priv = netdev_priv(ndev);
	int i, ret;
	u32 reg;

#ifdef CONFIG_TI_CPSW_DUAL_EMAC
	if (!cpsw_common_res_usage_stat(priv))
		cpsw_intr_disable(priv);
#else /* !CONFIG_TI_CPSW_DUAL_EMAC */
	cpsw_intr_disable(priv);
#endif /* CONFIG_TI_CPSW_DUAL_EMAC */

	netif_carrier_off(ndev);

	ret = clk_enable(priv->clk);
	if (ret < 0) {
		dev_err(priv->dev, "unable to turn on device clock\n");
		return ret;
	}

	ret = device_create_file(&ndev->dev, &dev_attr_hw_stats);
	if (ret < 0) {
		dev_err(priv->dev, "unable to add device attr\n");
		return ret;
	}

	if (priv->data.phy_control)
		(*priv->data.phy_control)(true);

	reg = readl(&priv->regs->id_ver);
	priv->cpsw_version = reg;

	msg(info, ifup, "initializing cpsw version %d.%d (%d)\n",
	    (reg >> 8 & 0x7), reg & 0xff, (reg >> 11) & 0x1f);

	/* initialize host and slave ports */
#ifdef CONFIG_TI_CPSW_DUAL_EMAC
	if (!cpsw_common_res_usage_stat(priv))
		cpsw_init_host_port(priv);
	cpsw_slave_open(&priv->slaves[priv->emac_port], priv);
#else /* !CONFIG_TI_CPSW_DUAL_EMAC */
	cpsw_init_host_port(priv);
	for_each_slave(priv, cpsw_slave_open, priv);
#endif /* CONFIG_TI_CPSW_DUAL_EMAC */

#if defined(VLAN_SUPPORT) && !defined(CONFIG_TI_CPSW_DUAL_EMAC)
	writel(priv->data.default_vlan, &priv->host_port_regs->port_vlan);
	writel(priv->data.default_vlan, cpsw_slave_reg(priv,
						&priv->slaves[0], port_vlan));
	writel(priv->data.default_vlan, cpsw_slave_reg(priv,
						&priv->slaves[0], port_vlan));
	cpsw_ale_add_vlan(priv->ale, priv->data.default_vlan,
			ALE_ALL_PORTS << priv->host_port,
			ALE_ALL_PORTS << priv->host_port,
			ALE_ALL_PORTS << priv->host_port, 0);
#endif

#ifdef CONFIG_TI_CPSW_DUAL_EMAC
	if (!cpsw_common_res_usage_stat(priv)) {
#endif /* CONFIG_TI_CPSW_DUAL_EMAC */
		/* setup tx dma to fixed prio and zero offset */
		cpdma_control_set(priv->dma, CPDMA_TX_PRIO_FIXED, 1);
		cpdma_control_set(priv->dma, CPDMA_RX_BUFFER_OFFSET, 0);

		/* disable priority elevation and enable statistics
		on all ports */
		__raw_writel(0, &priv->regs->ptype);

		/* enable statistics collection only on the host port */
		/*__raw_writel(BIT(priv->host_port),
				&priv->regs->stat_port_en);*/
		__raw_writel(0x7, &priv->regs->stat_port_en);

		if (WARN_ON(!priv->data.rx_descs))
			priv->data.rx_descs = 128;

		for (i = 0; i < priv->data.rx_descs; i++) {
			struct sk_buff *skb;

			ret = -ENOMEM;
			skb = netdev_alloc_skb_ip_align(priv->ndev,
							priv->rx_packet_max);
			if (!skb)
				break;
			ret = cpdma_chan_submit(priv->rxch, skb, skb->data,
					skb_tailroom(skb), 0, GFP_KERNEL);
			if (WARN_ON(ret < 0))
				break;
		}
		/* continue even if we didn't manage to submit
		all receive descs */
		msg(info, ifup, "submitted %d rx descriptors\n", i);
#ifdef CONFIG_TI_CPSW_DUAL_EMAC
	}
#endif /* CONFIG_TI_CPSW_DUAL_EMAC */

	/* Enable Interrupt pacing if configured */
	if (priv->coal_intvl != 0) {
		struct ethtool_coalesce coal;

		coal.rx_coalesce_usecs = (priv->coal_intvl << 4);
		cpsw_set_coalesce(ndev, &coal);
	}

	cpdma_ctlr_start(priv->dma);
	cpsw_intr_enable(priv);
	napi_enable(&priv->napi);
	cpdma_ctlr_eoi(priv->dma);

#if (defined(CONFIG_PTP_1588_CLOCK_CPTS) ||		\
		defined(CONFIG_PTP_1588_CLOCK_CPTS_MODULE))
	reg = __raw_readl(&priv->cpts_reg->id_ver);
	if (reg == CPTS_VERSION) {
		printk(KERN_ERR "Found CPTS and initializing...\n");
		/* Enable CPTS */
		__raw_writel(0x01, &priv->cpts_reg->control);
		/* Enable CPTS Interrupt */
		__raw_writel(0x01, &priv->cpts_reg->int_enable);
		/* Enable CPSW_SS Misc Interrupt */
		__raw_writel(0x10, &priv->ss_regs->misc_stat);
	} else {
		printk(KERN_ERR "Cannot find CPTS\n");
	}
#endif

#ifdef CONFIG_TI_CPSW_DUAL_EMAC
	priv->slaves[priv->emac_port].open_stat = true;
#endif /* CONFIG_TI_CPSW_DUAL_EMAC */

	return 0;
}

static void cpsw_slave_stop(struct cpsw_slave *slave, struct cpsw_priv *priv)
{
	if (!slave->phy)
		return;
	phy_stop(slave->phy);
	phy_disconnect(slave->phy);
	slave->phy = NULL;
}

static int cpsw_ndo_stop(struct net_device *ndev)
{
	struct cpsw_priv *priv = netdev_priv(ndev);

	msg(info, ifdown, "shutting down cpsw device\n");
	netif_stop_queue(priv->ndev);
	napi_disable(&priv->napi);
	netif_carrier_off(priv->ndev);

#ifdef CONFIG_TI_CPSW_DUAL_EMAC
	if (cpsw_common_res_usage_stat(priv) <= 1) {
#endif /* CONFIG_TI_CPSW_DUAL_EMAC */
		cpsw_intr_disable(priv);
		cpdma_ctlr_int_ctrl(priv->dma, false);
		cpdma_ctlr_stop(priv->dma);
		cpsw_ale_stop(priv->ale);
#ifdef CONFIG_TI_CPSW_DUAL_EMAC
	}
#endif /* CONFIG_TI_CPSW_DUAL_EMAC */

	device_remove_file(&ndev->dev, &dev_attr_hw_stats);

#ifdef CONFIG_TI_CPSW_DUAL_EMAC
	cpsw_slave_stop(&priv->slaves[priv->emac_port], priv);
#else /* CONFIG_TI_CPSW_DUAL_EMAC */
	for_each_slave(priv, cpsw_slave_stop, priv);
#endif /* CONFIG_TI_CPSW_DUAL_EMAC */

	if (priv->data.phy_control)
		(*priv->data.phy_control)(false);
	clk_disable(priv->clk);

#ifdef CONFIG_TI_CPSW_DUAL_EMAC
	priv->slaves[priv->emac_port].open_stat = false;
#endif /* CONFIG_TI_CPSW_DUAL_EMAC */

	return 0;
}

static netdev_tx_t cpsw_ndo_start_xmit(struct sk_buff *skb,
				       struct net_device *ndev)
{
	struct cpsw_priv *priv = netdev_priv(ndev);
	int ret;

	ndev->trans_start = jiffies;

	ret = skb_padto(skb, CPSW_MIN_PACKET_SIZE);
	if (unlikely(ret < 0)) {
		msg(err, tx_err, "packet pad failed");
		goto fail;
	}

#ifdef CONFIG_TI_CPSW_DUAL_EMAC
	if (ndev == priv->slaves[0].ndev) {
		ret = cpdma_chan_submit(priv->txch, skb, skb->data,
				skb->len, 1, GFP_KERNEL);
	} else {
		ret = cpdma_chan_submit(priv->txch, skb, skb->data,
				skb->len, 2, GFP_KERNEL);
	}
#else
	ret = cpdma_chan_submit(priv->txch, skb, skb->data,
				skb->len, 0, GFP_KERNEL);
#endif /* CONFIG_TI_CPSW_DUAL_EMAC */

	if (unlikely(ret != 0)) {
		msg(err, tx_err, "desc submit failed");
		goto fail;
	}

	return NETDEV_TX_OK;
fail:
	priv->stats.tx_dropped++;
	netif_stop_queue(ndev);
	return NETDEV_TX_BUSY;
}

#ifdef VLAN_SUPPORT

static void __cpsw_ndo_vlan_rx_add_vid(struct net_device *ndev,
		unsigned short vid)
{
	struct cpsw_priv *priv = netdev_priv(ndev);

	if (vid) {
#ifdef CONFIG_TI_CPSW_DUAL_EMAC
		cpsw_ale_add_vlan(priv->ale, vid,
				1 << priv->host_port |
				1 << (priv->emac_port + 1), 0,
				1 << priv->host_port |
				1 << (priv->emac_port + 1), 0);
		cpsw_ale_vlan_add_ucast(priv->ale, priv->mac_addr,
				priv->host_port, 0, vid);
		cpsw_ale_vlan_add_mcast(priv->ale, priv->ndev->broadcast,
				1 << priv->host_port |
				1 << (priv->emac_port + 1), vid, 0, 0);

#else /* !CONFIG_TI_CPSW_DUAL_EMAC */
		cpsw_ale_add_vlan(priv->ale, vid,
				ALE_ALL_PORTS << priv->host_port, 0,
				ALE_ALL_PORTS << priv->host_port, 0);
		cpsw_ale_vlan_add_ucast(priv->ale, priv->mac_addr,
				priv->host_port, 0, vid);
		cpsw_ale_vlan_add_mcast(priv->ale, priv->ndev->broadcast,
				ALE_ALL_PORTS << priv->host_port, vid, 0, 0);
#endif /* CONFIG_TI_CPSW_DUAL_EMAC */
	} else
		cpsw_ale_add_vlan(priv->ale, vid,
				ALE_ALL_PORTS << priv->host_port,
				ALE_ALL_PORTS << priv->host_port,
				ALE_ALL_PORTS << priv->host_port, 0);
}

static void cpsw_ndo_vlan_rx_add_vid(struct net_device *ndev,
		unsigned short vid)
{
	struct cpsw_priv *priv = netdev_priv(ndev);

	spin_lock(&priv->lock);
	printk(KERN_ERR "Adding vlanid %d to vlan filter\n", vid);
	__cpsw_ndo_vlan_rx_add_vid(ndev, vid);
	spin_unlock(&priv->lock);
}

static void cpsw_ndo_vlan_rx_register(struct net_device *ndev,
		struct vlan_group *grp)
{
	struct cpsw_priv *priv = netdev_priv(ndev);
	int i;

	spin_lock(&priv->lock);

	priv->vlgrp = grp;
	if (grp) {
		for (i = 0; i <= VLAN_VID_MASK; i++) {
			if (vlan_group_get_device(priv->vlgrp, i))
				__cpsw_ndo_vlan_rx_add_vid(ndev, i);
		}
	}
	spin_unlock(&priv->lock);
}

static void cpsw_ndo_vlan_rx_kill_vid(struct net_device *ndev,
		unsigned short vid)
{
	struct cpsw_priv *priv = netdev_priv(ndev);

	spin_lock(&priv->lock);
	printk(KERN_ERR "%s: removing vlanid %d from vlan filter\n",
			ndev->name, vid);
	vlan_group_set_device(priv->vlgrp, vid, NULL);
	cpsw_ale_del_vlan(priv->ale, vid, ALE_ALL_PORTS << priv->host_port);
	cpsw_ale_vlan_del_ucast(priv->ale, priv->mac_addr,
				priv->host_port, vid);
	cpsw_ale_vlan_del_mcast(priv->ale, priv->ndev->broadcast, 0, vid);
	spin_unlock(&priv->lock);
}
#endif /* VLAN_SUPPORT */

#if (defined(CONFIG_PTP_1588_CLOCK_CPTS) ||		\
		defined(CONFIG_PTP_1588_CLOCK_CPTS_MODULE))

#ifdef CONFIG_TI_CPSW_DUAL_EMAC

static int cpts_enable_l2_ts(struct cpsw_priv *priv, bool state)
{
	u32 val = 0;

	if (priv->cpsw_version == CPSW_VER_1) {
		if (state) {
			/* Enable TS */
			val = CPSW_V1_TS_RX_EN | CPSW_V1_TS_TX_EN |
				(CPSW_MSG_TYPE_EN << CPSW_V1_MSG_TYPE_OFS);
			writel(val, cpsw_slave_reg(priv,
					&priv->slaves[priv->emac_port],
					ts_ctl));

			val = (CPSW_SEQ_ID_OFS << CPSW_V1_SEQ_ID_OFS_SHIFT) |
				CPSW_801_1Q_LTYPE;
			writel(val, cpsw_slave_reg(priv,
					&priv->slaves[priv->emac_port],
					ts_seq_ltype));
		} else {
			/* Disable TS */
			writel(0, cpsw_slave_reg(priv,
					&priv->slaves[priv->emac_port],
					ts_ctl));
		}
	} else {
		if (state) {
			/* Enable TS */
			val = CPSW_V2_TS_ANNEX_D_EN | CPSW_V2_TS_LTYPE_1_EN
				| CPSW_V2_TS_RX_EN | CPSW_V2_TS_TX_EN;

			writel(val, cpsw_slave_reg(priv,
					&priv->slaves[priv->emac_port],
					port_control));

			val = (CPSW_SEQ_ID_OFS << CPSW_V2_SEQ_ID_OFS_SHIFT) |
				CPSW_MSG_TYPE_EN;
			writel(val, cpsw_slave_reg(priv,
					&priv->slaves[priv->emac_port],
					ts_seq_mtype));

			writel(CPSW_801_1Q_LTYPE, &priv->regs->ts_ltype);
		} else {
			/* Disable TS */
			writel(0, cpsw_slave_reg(priv, &priv->slaves[0],
						 port_control));
			writel(0, cpsw_slave_reg(priv, &priv->slaves[1],
						 port_control));

			writel(0, cpsw_slave_reg(priv, &priv->slaves[0],
						 ts_seq_mtype));
			writel(0, cpsw_slave_reg(priv, &priv->slaves[1],
						 ts_seq_mtype));
		}
	}

	return 0;
}

#else

static int cpts_enable_l2_ts(struct cpsw_priv *priv, bool state)
{
	u32 val = 0;

	if (priv->cpsw_version == CPSW_VER_1) {
		if (state) {
			/* Enable TS */
			val = CPSW_V1_TS_RX_EN | CPSW_V1_TS_TX_EN |
				(CPSW_MSG_TYPE_EN << CPSW_V1_MSG_TYPE_OFS);
			writel(val, cpsw_slave_reg(priv, &priv->slaves[0],
						   ts_ctl));
			writel(val, cpsw_slave_reg(priv, &priv->slaves[1],
						   ts_ctl));

			val = (CPSW_SEQ_ID_OFS << CPSW_V1_SEQ_ID_OFS_SHIFT) |
				CPSW_801_1Q_LTYPE;
			writel(val, cpsw_slave_reg(priv, &priv->slaves[0],
						   ts_seq_ltype));
			writel(val, cpsw_slave_reg(priv, &priv->slaves[1],
						   ts_seq_ltype));
		} else {
			/* Disable TS */
			writel(0, cpsw_slave_reg(priv, &priv->slaves[0],
						 ts_ctl));
			writel(0, cpsw_slave_reg(priv, &priv->slaves[1],
						 ts_ctl));
		}
	} else {
		if (state) {
			/* Enable TS */
			val = CPSW_V2_TS_ANNEX_D_EN | CPSW_V2_TS_LTYPE_1_EN
				| CPSW_V2_TS_RX_EN | CPSW_V2_TS_TX_EN;

			writel(val, cpsw_slave_reg(priv, &priv->slaves[0],
						   port_control));
			writel(val, cpsw_slave_reg(priv, &priv->slaves[1],
						   port_control));

			val = (CPSW_SEQ_ID_OFS << CPSW_V2_SEQ_ID_OFS_SHIFT) |
				CPSW_MSG_TYPE_EN;
			writel(val, cpsw_slave_reg(priv, &priv->slaves[0],
						   ts_seq_mtype));
			writel(val, cpsw_slave_reg(priv, &priv->slaves[1],
						   ts_seq_mtype));

			writel(CPSW_801_1Q_LTYPE, &priv->regs->ts_ltype);
		} else {
			/* Disable TS */
			writel(0, cpsw_slave_reg(priv, &priv->slaves[0],
						 port_control));
			writel(0, cpsw_slave_reg(priv, &priv->slaves[1],
						 port_control));

			writel(0, cpsw_slave_reg(priv, &priv->slaves[0],
						 ts_seq_mtype));
			writel(0, cpsw_slave_reg(priv, &priv->slaves[1],
						 ts_seq_mtype));
		}
	}

	return 0;
}

#endif

static int cpsw_hwtstamp_ioctl(struct net_device *ndev,
		struct ifreq *ifr, int cmd)
{
	struct hwtstamp_config config;
	struct cpsw_priv *priv = netdev_priv(ndev);

	if (copy_from_user(&config, ifr->ifr_data, sizeof(config)))
		return -EFAULT;

	pr_debug("%s config flag:0x%x, tx_type:0x%x, rx_filter:0x%x\n",
		__func__, config.flags, config.tx_type, config.rx_filter);

	/* reserved for future extensions */
	if (config.flags)
		return -EINVAL;

	if ((config.tx_type != HWTSTAMP_TX_OFF) &&
			(config.tx_type != HWTSTAMP_TX_ON))
		return -ERANGE;

	switch (config.rx_filter) {
	case HWTSTAMP_FILTER_NONE:
		/*
		 * Dont allow any timestamping
		 */
		break;

	case HWTSTAMP_FILTER_PTP_V1_L4_EVENT:
	case HWTSTAMP_FILTER_PTP_V1_L4_SYNC:
	case HWTSTAMP_FILTER_PTP_V1_L4_DELAY_REQ:
		dev_err(&ndev->dev, "PTP V1 L4 Timestamping is not supported!!!\n");
		config.rx_filter = HWTSTAMP_FILTER_NONE;
		config.tx_type = HWTSTAMP_TX_OFF;
		break;

	case HWTSTAMP_FILTER_PTP_V2_L4_EVENT:
	case HWTSTAMP_FILTER_PTP_V2_L4_SYNC:
	case HWTSTAMP_FILTER_PTP_V2_L4_DELAY_REQ:
		dev_err(&ndev->dev, "PTP V2 L4 Timestamping is not supported!!!\n");
		config.rx_filter = HWTSTAMP_FILTER_NONE;
		config.tx_type = HWTSTAMP_TX_OFF;
		break;

	case HWTSTAMP_FILTER_PTP_V2_L2_EVENT:
	case HWTSTAMP_FILTER_PTP_V2_L2_SYNC:
	case HWTSTAMP_FILTER_PTP_V2_L2_DELAY_REQ:

	case HWTSTAMP_FILTER_PTP_V2_EVENT:
		/* PTP v2/802.1AS, any layer, any kind of event packet */
	case HWTSTAMP_FILTER_PTP_V2_SYNC:
		/* PTP v2/802.1AS, any layer, Sync packet */
	case HWTSTAMP_FILTER_PTP_V2_DELAY_REQ:
		/* PTP v2/802.1AS, any layer, Delay_req packet */

		/* Enabling All PTP v2/802.1AS Event time stamping */
		config.rx_filter = HWTSTAMP_FILTER_PTP_V2_L2_EVENT;
		cpts_enable_l2_ts(priv, true);
		priv->cpts_time->enable_timestamping = true;
		dev_info(priv->dev, "Enabling PTP Time stamping...\n");
		break;
	default:
		return -ERANGE;
	}

	/* Enabling Time stamping is done only for Port 1 */
	if (config.tx_type == HWTSTAMP_TX_OFF &&
			config.rx_filter == HWTSTAMP_FILTER_NONE) {
		cpts_enable_l2_ts(priv, false);

		priv->cpts_time->enable_timestamping = false;
		dev_info(priv->dev, "Disabling PTP Time stamping...\n");
	}

	/* Empty Queue */
	priv->cpts_time->rx_fifo.head = 0;
	priv->cpts_time->tx_fifo.head = 0;
	priv->cpts_time->rx_fifo.tail = 0;
	priv->cpts_time->tx_fifo.tail = 0;

	return copy_to_user(ifr->ifr_data, &config, sizeof(config)) ?
			-EFAULT : 0;
}
#endif

#ifndef CONFIG_TI_CPSW_DUAL_EMAC
/**
 *  cpsw_set_priority_mapping
 *    This function configures the packet priority handling on each port.
 */
static int cpsw_set_priority_mapping(struct cpsw_priv *priv, int port,
	int rxpriority, int txpriority, int switchpriority)
{
	if (port == 0) {
		writel(CPSW_PRIMAP(rxpriority, txpriority),
		       &priv->host_port_regs->cpdma_tx_pri_map);
		writel(CPSW_PRIMAP(txpriority, switchpriority),
		       &priv->host_port_regs->tx_pri_map);
	} else {
		/* Configure sliver priority mapping registers */
		writel(CPSW_PRIMAP(rxpriority, txpriority),
		       &slave(priv, port-1)->sliver->rx_pri_map);
		writel(CPSW_PRIMAP(txpriority, switchpriority),
		       cpsw_slave_reg(priv, slave(priv, port-1), tx_pri_map));
	}
	return 0;
}

/**
 *    cpsw_rate_limit_tx
 *    Limits the number of Tx packets on a port for a 100ms time period.
 *    Rate limit can be done either on Tx or Rx for all the ports due to
 *    hardware limitation
 */
static int cpsw_rate_limit_tx(struct cpsw_ale *ale, u32 enable, u32 port,
				u32 addr_type, u32 limit)
{
	if (!enable) {
		cpsw_ale_control_set(ale, port, ALE_RATE_LIMIT, 0);
		return 0;
	}

	/* ALE prescale is set for 100 ms time */
	if (addr_type == ADDR_TYPE_BROADCAST)
		cpsw_ale_control_set(ale, port, ALE_PORT_BCAST_LIMIT, limit);
	else if (addr_type == ADDR_TYPE_MULTICAST)
		cpsw_ale_control_set(ale, port, ALE_PORT_MCAST_LIMIT, limit);
	else
		return -EFAULT;

	/* Enable the bit for rate limiting transmission */
	cpsw_ale_control_set(ale, port, ALE_RATE_LIMIT_TX, 1);
	cpsw_ale_control_set(ale, port, ALE_RATE_LIMIT, 1);

	return 0;
}

/**
 *    cpsw_rate_limit_rx
 *    Limits the number of Rx packets on a port for a 100ms time period.
 *    Rate limit can be done either on Tx or Rx for all the ports due to
 *    hardware limitation
 */
static int cpsw_rate_limit_rx(struct cpsw_ale *ale, u32 enable, u32 port,
				u32 addr_type, u32 limit)
{
	if (!enable) {
		cpsw_ale_control_set(ale, port, ALE_RATE_LIMIT, 0);
		return 0;
	}

	/* ALE prescale is set for 100 ms time */
	if (addr_type == ADDR_TYPE_BROADCAST)
		cpsw_ale_control_set(ale, port, ALE_PORT_BCAST_LIMIT, limit);
	else if (addr_type == ADDR_TYPE_MULTICAST)
		cpsw_ale_control_set(ale, port, ALE_PORT_MCAST_LIMIT, limit);
	else
		return -EFAULT;

	/* Disable the bit for rate limiting reception */
	cpsw_ale_control_set(ale, port, ALE_RATE_LIMIT_TX, 0);
	cpsw_ale_control_set(ale, port, ALE_RATE_LIMIT, 1);

	return 0;
}

int cpsw_rate_limit(struct cpsw_ale *ale, u32 enable, u32 direction, u32 port,
			u32 packet_type, u32 limit)
{

	if (port == 0) {
		/* For host port transmit/receive terminlogy is inverse
			of cpgmac port */
		if (direction)
			cpsw_rate_limit_rx(ale, enable, port,
					packet_type, limit);
		else
			cpsw_rate_limit_tx(ale, enable, port,
					packet_type, limit);
	} else {
		if (direction)
			cpsw_rate_limit_tx(ale, enable, port,
					packet_type, limit);
		else
			cpsw_rate_limit_rx(ale, enable, port,
					packet_type, limit);
	}
	return 0;
}

static int cpsw_config_dump(struct cpsw_priv *priv, u8 *buf, u32 size)
{
	int vlan_aware = 0;
	int out_len = 0;

	out_len += snprintf(buf, size, "Switch Configuarion\n");

	vlan_aware = __raw_readl(&priv->regs->control) & CPSW_VLAN_AWARE;

	out_len += snprintf(buf + out_len, size - out_len, "VLAN Aware Mode"
			"                              :  %s\n",
			(vlan_aware ? "Yes" : "No"));
	out_len += snprintf(buf + out_len, size - out_len, "ALE VLAN Aware "
			"Mode                          :  %s\n",
			(cpsw_ale_control_get(priv->ale, priv->host_port,
				ALE_VLAN_AWARE) ? "Yes" : "No"));
	out_len += snprintf(buf + out_len, size - out_len, "Unknown VLAN "
			"Members                         :  %u\n",
			cpsw_ale_control_get(priv->ale, 0,
				ALE_PORT_UNKNOWN_VLAN_MEMBER));
	out_len += snprintf(buf + out_len, size - out_len, "Unknown Multicast"
			" Flood Mask                 :  %u\n",
			cpsw_ale_control_get(priv->ale, 0,
				ALE_PORT_UNKNOWN_MCAST_FLOOD));
	out_len += snprintf(buf + out_len, size - out_len, "Unknown Registered"
			" Multicast Flood Mask      :  %u\n",
			cpsw_ale_control_get(priv->ale, 0,
				ALE_PORT_UNKNOWN_REG_MCAST_FLOOD));
	out_len += snprintf(buf + out_len, size - out_len, "Unknown VLAN Force"
			" Untagged Egress           :  %u\n",
			cpsw_ale_control_get(priv->ale, 0,
				ALE_PORT_UNTAGGED_EGRESS));

	out_len += snprintf(buf + out_len, size - out_len,
			"\nPort Configuration\n");
	out_len += snprintf(buf + out_len, size - out_len,
			"\t%-8s %-8s %-8s %s\n", "PORT", "PVID", "PRIORITY",
			"STATE");
	out_len += snprintf(buf + out_len, size - out_len,
			"\t---------------------------------\n");

	if (vlan_aware) {
		int port_vlan;
		char *port_state = NULL;

		switch (cpsw_ale_control_get(priv->ale, 0, ALE_PORT_STATE)) {
		case 0:
			port_state = "disabled";
			break;
		case 1:
			port_state = "blocked";
			break;
		case 2:
			port_state = "learn";
			break;
		case 3:
			port_state = "forward";
			break;
		}
		port_vlan = __raw_readl(&priv->host_port_regs->port_vlan);
		out_len += snprintf(buf + out_len, size - out_len,
			"\t%-8u %-8u %-8u %s\n", 0,
			port_vlan & 0xfff, (port_vlan > 13) & 0x7, port_state);

		switch (cpsw_ale_control_get(priv->ale, 1, ALE_PORT_STATE)) {
		case 0:
			port_state = "disabled";
			break;
		case 1:
			port_state = "blocked";
			break;
		case 2:
			port_state = "learn";
			break;
		case 3:
			port_state = "forward";
			break;
		}
		port_vlan = readl(cpsw_slave_reg(priv, &priv->slaves[0],
						 port_vlan));
		out_len += snprintf(buf + out_len, size - out_len,
			"\t%-8u %-8u %-8u %s\n", 1,
			port_vlan & 0xfff, (port_vlan > 13) & 0x7, port_state);

		switch (cpsw_ale_control_get(priv->ale, 2, ALE_PORT_STATE)) {
		case 0:
			port_state = "disabled";
			break;
		case 1:
			port_state = "blocked";
			break;
		case 2:
			port_state = "learn";
			break;
		case 3:
			port_state = "forward";
			break;
		}
		port_vlan = readl(cpsw_slave_reg(priv, &priv->slaves[0],
						 port_vlan));
		out_len += snprintf(buf + out_len, size - out_len,
			"\t%-8u %-8u %-8u %s\n", 2,
			port_vlan & 0xfff, (port_vlan > 13) & 0x7, port_state);
	} else {
		char *port_state = NULL;

		switch (cpsw_ale_control_get(priv->ale, 0, ALE_PORT_STATE)) {
		case 0:
			port_state = "disabled";
			break;
		case 1:
			port_state = "blocked";
			break;
		case 2:
			port_state = "learn";
			break;
		case 3:
			port_state = "forward";
			break;
		}
		out_len += snprintf(buf + out_len, size - out_len,
			"\t%-8u %-8s %-8s %s\n", 0,
			"-", "-", port_state);

		switch (cpsw_ale_control_get(priv->ale, 1, ALE_PORT_STATE)) {
		case 0:
			port_state = "disabled";
			break;
		case 1:
			port_state = "blocked";
			break;
		case 2:
			port_state = "learn";
			break;
		case 3:
			port_state = "forward";
			break;
		}
		out_len += snprintf(buf + out_len, size - out_len,
			"\t%-8u %-8s %-8s %s\n", 1,
			"-", "-", port_state);

		switch (cpsw_ale_control_get(priv->ale, 2, ALE_PORT_STATE)) {
		case 0:
			port_state = "disabled";
			break;
		case 1:
			port_state = "blocked";
			break;
		case 2:
			port_state = "learn";
			break;
		case 3:
			port_state = "forward";
			break;
		}
		out_len += snprintf(buf + out_len, size - out_len,
			"\t%-8u %-8s %-8s %s\n", 2,
			"-", "-", port_state);
	}

	return out_len;
}

static int cpsw_set_port_state(struct cpsw_priv *priv, int port,
			int port_state)
{
	int ret = -EFAULT;
	switch (port_state) {
	case PORT_STATE_DISABLED:
		ret = cpsw_ale_control_set(priv->ale,
			port, ALE_PORT_STATE,
			ALE_PORT_STATE_DISABLE);
		priv->port_state[port] = ALE_PORT_STATE_DISABLE;
		break;
	case PORT_STATE_BLOCKED:
		ret = cpsw_ale_control_set(priv->ale,
			port, ALE_PORT_STATE,
			ALE_PORT_STATE_BLOCK);
		priv->port_state[port] = ALE_PORT_STATE_BLOCK;
		break;
	case PORT_STATE_LEARN:
		ret = cpsw_ale_control_set(priv->ale,
			port, ALE_PORT_STATE,
			ALE_PORT_STATE_LEARN);
		priv->port_state[port] = ALE_PORT_STATE_LEARN;
		break;
	case PORT_STATE_FORWARD:
		ret = cpsw_ale_control_set(priv->ale,
			port, ALE_PORT_STATE,
			ALE_PORT_STATE_FORWARD);
		priv->port_state[port] = ALE_PORT_STATE_FORWARD;
		break;
	}
	return ret;
}

static int cpsw_switch_config_ioctl(struct net_device *ndev,
		struct ifreq *ifrq, int cmd)
{
	struct cpsw_priv *priv = netdev_priv(ndev);
	struct net_switch_config *switch_config;
	int ret = -EFAULT;

	/*
	* Only SIOCDEVPRIVATE is used as cmd argument and hence, there is no
	* switch statement required.
	* Function calls are based on switch_config.cmd
	*/
	if (cmd != SIOCSWITCHCONFIG)
		return ret;

	switch_config = kzalloc(sizeof(struct net_switch_config), GFP_KERNEL);
	if (copy_from_user(switch_config, (ifrq->ifr_data),
			sizeof(struct net_switch_config)))
		return ret;

	switch (switch_config->cmd) {
	case CONFIG_SWITCH_ADD_MULTICAST:
		if ((switchcmd(switch_config).untag_port <= 3) &&
				(switchcmd(switch_config).vid <= 4095) &&
				(switchcmd(switch_config).mem_port > 0) &&
				(switchcmd(switch_config).mem_port <= 7) &&
				is_multicast_ether_addr(
				switchcmd(switch_config).addr)) {
			if (switchcmd(switch_config).vid == 0)
				ret = cpsw_ale_add_mcast(priv->ale,
					switchcmd(switch_config).addr,
					switchcmd(switch_config).mem_port
						<< priv->host_port,
					switchcmd(switch_config).flag,
					switchcmd(switch_config).untag_port);
			else
				ret = cpsw_ale_vlan_add_mcast(priv->ale,
					switchcmd(switch_config).addr,
					switchcmd(switch_config).mem_port
						<< priv->host_port,
					switchcmd(switch_config).vid,
					switchcmd(switch_config).flag,
					switchcmd(switch_config).untag_port);
		} else {
			dev_err(priv->dev, "Invalid Switch config arguments\n");
			ret = -EFAULT;
		}
		break;

	case CONFIG_SWITCH_ADD_UNICAST:
		if ((switchcmd(switch_config).vid <= 4095) &&
				(switchcmd(switch_config).mem_port <= 2) &&
				is_valid_ether_addr(
					switchcmd(switch_config).addr)) {
			if (switchcmd(switch_config).vid == 0)
				ret = cpsw_ale_add_ucast(priv->ale,
					switchcmd(switch_config).addr,
					priv->host_port, 0);
			else
				ret = cpsw_ale_vlan_add_ucast(priv->ale,
					switchcmd(switch_config).addr,
					priv->host_port, 0,
					switchcmd(switch_config).vid);
		} else {
			dev_err(priv->dev, "Invalid Switch config arguments\n");
			ret = -EFAULT;
		}
		break;

	case CONFIG_SWITCH_ADD_OUI:
		if (!is_zero_ether_addr(switchcmd(switch_config).addr)) {
			ret = cpsw_ale_add_oui(priv->ale,
				switchcmd(switch_config).addr);
		} else {
			dev_err(priv->dev, "Invalid Switch config arguments\n");
			ret = -EFAULT;
		}
		break;

	case CONFIG_SWITCH_FIND_ADDR:
		if ((switchcmd(switch_config).vid <= 4095) &&
			!is_zero_ether_addr(switchcmd(switch_config).addr)) {
			ret = cpsw_ale_match_addr(priv->ale,
				switchcmd(switch_config).addr,
				switchcmd(switch_config).vid);
			if (ret >= 0) {
				switch_config->ret_type = ret;
				ret = copy_to_user(ifrq->ifr_data,
					switch_config,
					sizeof(struct net_switch_config)) ?
					-EFAULT : 0;
			}
		} else {
			dev_err(priv->dev, "Invalid Arguments\n");
			ret = -EFAULT;
		}
		break;

	case CONFIG_SWITCH_DEL_MULTICAST:
		if ((switchcmd(switch_config).vid <= 4095) &&
				is_multicast_ether_addr(
				switchcmd(switch_config).addr)) {
			if (switchcmd(switch_config).vid == 0)
				ret = cpsw_ale_del_mcast(priv->ale,
					switchcmd(switch_config).addr,
					switchcmd(switch_config).mem_port
						<< priv->host_port);
			else
				ret = cpsw_ale_vlan_del_mcast(priv->ale,
					switchcmd(switch_config).addr,
					switchcmd(switch_config).mem_port
						<< priv->host_port,
					switchcmd(switch_config).vid);
		} else {
			dev_err(priv->dev, "Invalid Arguments\n");
			ret = -EFAULT;
		}
		break;

	case CONFIG_SWITCH_DEL_UNICAST:
		if ((switchcmd(switch_config).vid <= 4095) &&
			is_valid_ether_addr(switchcmd(switch_config).addr)) {
			if (switchcmd(switch_config).vid == 0) {
				if (!memcmp(switchcmd(switch_config).addr,
						priv->mac_addr, 6)) {
					ret = -EPERM;
					break;
				}
				ret = cpsw_ale_del_ucast(priv->ale,
					switchcmd(switch_config).addr, 0);
			} else
				ret = cpsw_ale_del_ucast(priv->ale,
					switchcmd(switch_config).addr,
					switchcmd(switch_config).vid);
		} else {
			dev_err(priv->dev, "Invalid Arguments\n");
			ret = -EFAULT;
		}
		break;

	case CONFIG_SWITCH_ADD_VLAN:
		if ((switchcmd(switch_config).vid <= 4095) &&
				(switchcmd(switch_config).mem_port > 0) &&
				(switchcmd(switch_config).mem_port <= 7)) {
			ret = cpsw_ale_add_vlan(priv->ale,
				switchcmd(switch_config).vid,
				switchcmd(switch_config).mem_port
					<< priv->host_port,
				switchcmd(switch_config).untag_port,
				switchcmd(switch_config).reg_multi,
				switchcmd(switch_config).unreg_multi);
		} else {
			dev_err(priv->dev, "Invalid Arguments\n");
			ret = -EFAULT;
		}
		break;

	case CONFIG_SWITCH_FIND_VLAN:
		if (switchcmd(switch_config).vid <= 4095) {
			switch_config->ret_type = cpsw_ale_match_vlan(priv->ale,
				switchcmd(switch_config).vid);
			ret = copy_to_user(ifrq->ifr_data, switch_config,
				sizeof(struct net_switch_config)) ?
				-EFAULT : 0;
		} else {
			dev_err(priv->dev, "Invalid Arguments\n");
			ret = -EFAULT;
		}
		break;

	case CONFIG_SWITCH_DEL_VLAN:
		if ((switchcmd(switch_config).vid <= 4095) &&
				(switchcmd(switch_config).mem_port <= 7)) {
			ret = cpsw_ale_del_vlan(priv->ale,
				switchcmd(switch_config).vid,
				switchcmd(switch_config).mem_port
				<< priv->host_port);
		} else {
			dev_err(priv->dev, "Invalid Arguments\n");
			ret = -EFAULT;
		}
		break;

	case CONFIG_SWITCH_SET_PORT_VLAN_CONFIG:
	{
		int port_vlan = switchcmd(switch_config).vid |
				switchcmd(switch_config).prio_port << 13;

		if ((switchcmd(switch_config).vid <= 4095) &&
				(switchcmd(switch_config).port <= 2) &&
				(switchcmd(switch_config).prio_port <= 7)) {
			if (switchcmd(switch_config).CFI_port)
				port_vlan |= (1 << 12);

			if (switchcmd(switch_config).port == 0)
				writel(port_vlan,
					&priv->host_port_regs->port_vlan);
			else if (switchcmd(switch_config).port == 1)
				writel(port_vlan, cpsw_slave_reg(priv,
						&priv->slaves[0], port_vlan));
			else
				writel(port_vlan, cpsw_slave_reg(priv,
						&priv->slaves[1], port_vlan));
			ret = 0;
		} else {
			dev_err(priv->dev, "Invalid Arguments\n");
			ret = -EFAULT;
		}
		break;
	}

	case CONFIG_SWITCH_TIMEOUT:
		ret = cpsw_ale_set_ageout(priv->ale,
			switchcmd(switch_config).ale_timeout);
		break;

	case CONFIG_SWITCH_DUMP:
		ret = cpsw_ale_dump(priv->ale,
			switchcmd(switch_config).aledump,
			switch_config->cmd_data.buf, 4095);
		if (ret)
			ret = copy_to_user(ifrq->ifr_data, switch_config,
				sizeof(struct net_switch_config)) ?
				-EFAULT : 0;
		break;

	case CONFIG_SWITCH_SET_FLOW_CONTROL:
		if (portcmd(switch_config).port <= 7) {
			writel(portcmd(switch_config).port,
				&priv->regs->flow_control);
		} else {
			dev_err(priv->dev, "Invalid Arguments\n");
			ret = -EFAULT;
		}
		break;

	case CONFIG_SWITCH_SET_PRIORITY_MAPPING:
		if ((priocmd(switch_config).port <= 2) &&
			(priocmd(switch_config).prio_rx <= 7) &&
			(priocmd(switch_config).prio_tx <= 7) &&
			(priocmd(switch_config).prio_switch <= 3)) {
			ret = cpsw_set_priority_mapping(priv,
				priocmd(switch_config).port,
				priocmd(switch_config).prio_rx,
				priocmd(switch_config).prio_tx,
				priocmd(switch_config).prio_switch);
		} else {
			dev_err(priv->dev, "Invalid Arguments\n");
			ret = -EFAULT;
		}
		break;

	case CONFIG_SWITCH_PORT_STATISTICS_ENABLE:
		if (portcmd(switch_config).port <= 7) {
			writel(portcmd(switch_config).port,
				&priv->regs->stat_port_en);
			ret = 0;
		} else {
			dev_err(priv->dev, "Invalid Arguments\n");
			ret = -EFAULT;
		}
		break;

	case CONFIG_SWITCH_CONFIG_DUMP:
		cpsw_config_dump(priv, switch_config->cmd_data.buf, 4096);
		ret = copy_to_user(ifrq->ifr_data, switch_config,
			sizeof(struct net_switch_config)) ?
			-EFAULT : 0;
		break;

	case CONFIG_SWITCH_RATELIMIT:
		if ((portcmd(switch_config).port <= 2) &&
				(portcmd(switch_config).enable == 0))
			ret = cpsw_ale_control_set(priv->ale,
				portcmd(switch_config).port,
				ALE_RATE_LIMIT, 0);
		else if ((portcmd(switch_config).port <= 2) &&
				((portcmd(switch_config).addr_type
					== ADDR_TYPE_BROADCAST) ||
				(portcmd(switch_config).addr_type
					== ADDR_TYPE_MULTICAST)) &&
				(portcmd(switch_config).limit <= 255)) {
			ret = cpsw_rate_limit(priv->ale,
				portcmd(switch_config).enable,
				portcmd(switch_config).direction,
				portcmd(switch_config).port,
				portcmd(switch_config).addr_type,
				portcmd(switch_config).limit);
		} else {
			dev_err(priv->dev, "Invalid Arguments\n");
			ret = -EFAULT;
		}
		break;

	case CONFIG_SWITCH_VID_INGRESS_CHECK:
		if (portcmd(switch_config).port <= 2) {
			cpsw_ale_control_set(priv->ale,
				portcmd(switch_config).port,
				ALE_PORT_DROP_UNTAGGED, 1);
			cpsw_ale_control_set(priv->ale,
				portcmd(switch_config).port,
				ALE_PORT_DROP_UNKNOWN_VLAN, 1);
			ret = 0;
		} else {
			dev_err(priv->dev, "Invalid Arguments\n");
			ret = -EFAULT;
		}
		break;

	case CONFIG_SWITCH_ADD_UNKNOWN_VLAN_INFO:
		if ((portcmd(switch_config).port <= 7) &&
				(portcmd(switch_config).
					reg_multi_port_mask <= 7) &&
				(portcmd(switch_config).
					unknown_reg_multi_port_mask <= 7) &&
				(portcmd(switch_config).
					unknown_vlan_member <= 7)) {
			cpsw_ale_control_set(priv->ale, 0,
				ALE_PORT_UNTAGGED_EGRESS,
				portcmd(switch_config).port);
			cpsw_ale_control_set(priv->ale, 0,
				ALE_PORT_UNKNOWN_REG_MCAST_FLOOD,
				portcmd(switch_config).reg_multi_port_mask);
			cpsw_ale_control_set(priv->ale, 0,
				ALE_PORT_UNKNOWN_MCAST_FLOOD,
				portcmd(switch_config).
					unknown_reg_multi_port_mask);
			cpsw_ale_control_set(priv->ale, 0,
				ALE_PORT_UNKNOWN_VLAN_MEMBER,
				portcmd(switch_config).unknown_vlan_member);
			ret = 0;
		} else {
			dev_err(priv->dev, "Invalid Arguments\n");
			ret = -EFAULT;
		}
		break;

	case CONFIG_SWITCH_802_1:
	{
		char addr_802_1[6] = {0x01, 0x80, 0xc2, 0x0, 0x0, 0x03};
		if (portcmd(switch_config).enable)
			ret = cpsw_ale_add_mcast(priv->ale,
					addr_802_1, 0x7, 0, 0);
		else
			ret = cpsw_ale_add_mcast(priv->ale,
					addr_802_1, 0x3, 0, 0);
		break;
	}

	case CONFIG_SWITCH_MACAUTH:
		ret = cpsw_ale_control_set(priv->ale, priv->host_port,
			ALE_AUTH_ENABLE, 1);
		break;

	case CONFIG_SWITCH_SET_PORT_CONFIG:
	{
		struct ethtool_cmd ecmd;

		if (portcmd(switch_config).port == 1)
			if (priv->slaves[0].phy) {
				ecmd.phy_address = priv->slaves[0].phy->addr;
				ret = phy_ethtool_gset(priv->slaves[0].phy,
						&ecmd);
			} else {
				dev_err(priv->dev, "Phy not Found\n");
				ret = -EFAULT;
				break;
			}
		else if (portcmd(switch_config).port == 2)
			if (priv->slaves[1].phy) {
				ecmd.phy_address = priv->slaves[1].phy->addr;
				ret = phy_ethtool_gset(priv->slaves[1].phy,
						&ecmd);
			} else {
				dev_err(priv->dev, "Phy not Found\n");
				ret = -EFAULT;
				break;
			}
		else {
			dev_err(priv->dev, "Invalid Arguments\n");
			ret = -EFAULT;
			break;
		}

		if (((portcmd(switch_config).limit == 0) ||
			(portcmd(switch_config).limit == 10) ||
			(portcmd(switch_config).limit == 100) ||
			(portcmd(switch_config).limit == 1000))) {

			if (portcmd(switch_config).limit == 0)
				ecmd.autoneg = AUTONEG_ENABLE;
			else {
				ecmd.autoneg = AUTONEG_DISABLE;
				ecmd.speed =
					portcmd(switch_config).limit;
				if (portcmd(switch_config).direction
						== 1)
					ecmd.duplex = DUPLEX_FULL;
				else
					ecmd.duplex = DUPLEX_HALF;
			}

			if (portcmd(switch_config).port == 1)
				ret = phy_ethtool_sset(priv->slaves[0].phy,
					&ecmd);
			else
				ret = phy_ethtool_sset(priv->slaves[1].phy,
					&ecmd);
		} else {
			dev_err(priv->dev, "Invalid Arguments\n");
			ret = -EFAULT;
		}
		break;
	}

	case CONFIG_SWITCH_GET_PORT_CONFIG:
	{
		struct ethtool_cmd ecmd;

		if (portcmd(switch_config).port == 1)
			if (priv->slaves[0].phy) {
				ecmd.phy_address = priv->slaves[0].phy->addr;
				ret = phy_ethtool_gset(priv->slaves[0].phy,
						&ecmd);
			} else {
				dev_err(priv->dev, "Phy not Found\n");
				ret = -EFAULT;
				break;
			}
		else if (portcmd(switch_config).port == 2)
			if (priv->slaves[1].phy) {
				ecmd.phy_address = priv->slaves[1].phy->addr;
				ret = phy_ethtool_gset(priv->slaves[1].phy,
						&ecmd);
			} else {
				dev_err(priv->dev, "Phy not Found\n");
				ret = -EFAULT;
				break;
			}
		else {
			dev_err(priv->dev, "Invalid Arguments\n");
			ret = -EFAULT;
			break;
		}

		if (!ret) {
			portcmd(switch_config).limit = ecmd.speed;
			if (ecmd.duplex == DUPLEX_FULL)
				portcmd(switch_config).direction = 1;
			else
				portcmd(switch_config).direction = 0;
			ret = copy_to_user(ifrq->ifr_data, switch_config,
				sizeof(struct net_switch_config)) ?
				-EFAULT : 0;
		}

		break;
	}

	case CONFIG_SWITCH_PORT_STATE:
		if (portcmd(switch_config).port <= 2) {
			ret = cpsw_set_port_state(priv,
				portcmd(switch_config).port,
				portcmd(switch_config).port_state);
		} else {
			dev_err(priv->dev, "Invalid Arguments\n");
			ret = -EFAULT;
		}
		break;

	case CONFIG_SWITCH_RESET:
		ret = cpsw_ndo_stop(ndev);
		if (!ret)
			ret = cpsw_ndo_open(ndev);
		break;

	default:
		ret = -EOPNOTSUPP;
	}

	kfree(switch_config);
	return ret;
}
#endif /* CONFIG_TI_CPSW_DUAL_EMAC */

static int cpsw_ndo_do_ioctl(struct net_device *ndev, struct ifreq *ifrq,
		int cmd)
{
	if (!(netif_running(ndev)))
		return -EINVAL;

	switch (cmd) {
	case SIOCSHWTSTAMP:
#if (defined(CONFIG_PTP_1588_CLOCK_CPTS) ||		\
	defined(CONFIG_PTP_1588_CLOCK_CPTS_MODULE))
		return cpsw_hwtstamp_ioctl(ndev, ifrq, cmd);
#else
		return -EOPNOTSUPP;
#endif

	case SIOCSWITCHCONFIG:
#ifdef CONFIG_TI_CPSW_DUAL_EMAC
		return -EOPNOTSUPP;
#else
		return cpsw_switch_config_ioctl(ndev, ifrq, cmd);
#endif /* CONFIG_TI_CPSW_DUAL_EMAC */

	default:
		return -EOPNOTSUPP;
	}
	return 0;
}

static void cpsw_ndo_set_multicast_list(struct net_device *ndev)
{
	struct cpsw_priv *priv = netdev_priv(ndev);

	if (!netdev_mc_empty(ndev)) {
		struct netdev_hw_addr *ha;

		/* Clear all mcast from ALE */
		cpsw_ale_flush_multicast(priv->ale,
				ALE_ALL_PORTS << priv->host_port);

		/* program multicast address list into ALE register */
		netdev_for_each_mc_addr(ha, ndev) {
			cpsw_ale_add_mcast(priv->ale, (u8 *)ha->addr,
				ALE_ALL_PORTS << priv->host_port, 0, 0);
		}
	} else {
		/* Clear all mcast from ALE */
		cpsw_ale_flush_multicast(priv->ale,
				ALE_ALL_PORTS << priv->host_port);
	}
}

#ifdef CONFIG_TI_CPSW_DUAL_EMAC

static void cpsw_ndo_change_rx_flags(struct net_device *ndev, int flags)
{
	struct cpsw_priv *priv = netdev_priv(ndev);
	struct cpsw_ale *ale = priv->ale;

	if (flags & IFF_PROMISC) {
		if ((priv->slaves[0].ndev->flags & IFF_PROMISC) ||
				(priv->slaves[1].ndev->flags & IFF_PROMISC)) {
			/*
			 * Enabling promiscuous mode for one interface will be
			 * common for both the interface as the interface
			 * shares the same hardware resource.
			 */

			/* Enable Bypass */
			cpsw_ale_control_set(ale, 0, ALE_BYPASS, 1);

			dev_err(&ndev->dev, "promiscuity enabled\n");
		} else {
			/* Disable Bypass */
			cpsw_ale_control_set(ale, 0, ALE_BYPASS, 0);
			dev_err(&ndev->dev, "promiscuity disabled\n");
		}
	}

	/*
	 * The switch cannot filter multicast traffic unless it is configured
	 * in "VLAN Aware" mode.  Unfortunately, VLAN awareness requires a
	 * whole bunch of additional logic that this driver does not implement
	 * at present.
	 */
	if ((flags & IFF_ALLMULTI) && !(ndev->flags & IFF_ALLMULTI))
		dev_err(&ndev->dev, "multicast traffic cannot be filtered!\n");
}

#else

static void cpsw_ndo_change_rx_flags(struct net_device *ndev, int flags)
{
	/*
	 * The switch cannot operate in promiscuous mode without substantial
	 * headache.  For promiscuous mode to work, we would need to put the
	 * ALE in bypass mode and route all traffic to the host port.
	 * Subsequently, the host will need to operate as a "bridge", learn,
	 * and flood as needed.  For now, we simply complain here and
	 * do nothing about it :-)
	 */
	if ((flags & IFF_PROMISC) && (ndev->flags & IFF_PROMISC))
		dev_err(&ndev->dev, "promiscuity ignored!\n");

	/*
	 * The switch cannot filter multicast traffic unless it is configured
	 * in "VLAN Aware" mode.  Unfortunately, VLAN awareness requires a
	 * whole bunch of additional logic that this driver does not implement
	 * at present.
	 */
	if ((flags & IFF_ALLMULTI) && !(ndev->flags & IFF_ALLMULTI))
		dev_err(&ndev->dev, "multicast traffic cannot be filtered!\n");
}

#endif

static int cpsw_ndo_set_mac_address(struct net_device *ndev, void *p)
{
	struct cpsw_priv *priv = netdev_priv(ndev);
	struct sockaddr *addr = (struct sockaddr *)p;

	if (!is_valid_ether_addr(addr->sa_data))
		return -EADDRNOTAVAIL;

#ifdef CONFIG_TI_CPSW_DUAL_EMAC
	cpsw_ale_vlan_del_ucast(priv->ale, priv->mac_addr, priv->host_port,
				priv->slaves[priv->emac_port].port_vlan);
#else /* !CONFIG_TI_CPSW_DUAL_EMAC */
	cpsw_ale_del_ucast(priv->ale, priv->mac_addr, priv->host_port);
#endif /* CONFIG_TI_CPSW_DUAL_EMAC */

	memcpy(priv->mac_addr, addr->sa_data, ETH_ALEN);
	memcpy(ndev->dev_addr, priv->mac_addr, ETH_ALEN);

#ifdef CONFIG_TI_CPSW_DUAL_EMAC
	cpsw_ale_vlan_add_ucast(priv->ale, priv->mac_addr, priv->host_port,
				0, priv->slaves[priv->emac_port].port_vlan);
	/*ALE_SECURE);*/
	cpsw_set_slave_mac(&priv->slaves[priv->emac_port], priv);
#else /* !CONFIG_TI_CPSW_DUAL_EMAC */
	cpsw_ale_add_ucast(priv->ale, priv->mac_addr, priv->host_port,
			   0);
	/*ALE_SECURE);*/
	for_each_slave(priv, cpsw_set_slave_mac, priv);
#endif /* CONFIG_TI_CPSW_DUAL_EMAC */

	return 0;
}

static void cpsw_ndo_tx_timeout(struct net_device *ndev)
{
	struct cpsw_priv *priv = netdev_priv(ndev);

	msg(err, tx_err, "transmit timeout, restarting dma");
	priv->stats.tx_errors++;
	cpsw_intr_disable(priv);
	cpdma_ctlr_int_ctrl(priv->dma, false);
	cpdma_chan_stop(priv->txch);
	cpdma_chan_start(priv->txch);
	cpdma_ctlr_int_ctrl(priv->dma, true);
	cpsw_intr_enable(priv);
	cpdma_ctlr_eoi(priv->dma);
}

static struct net_device_stats *cpsw_ndo_get_stats(struct net_device *ndev)
{
	struct cpsw_priv *priv = netdev_priv(ndev);
	return &priv->stats;
}

#ifdef CONFIG_NET_POLL_CONTROLLER
static void cpsw_ndo_poll_controller(struct net_device *ndev)
{
	struct cpsw_priv *priv = netdev_priv(ndev);

	cpsw_intr_disable(priv);
	cpdma_ctlr_int_ctrl(priv->dma, false);
	cpsw_interrupt(ndev->irq, priv);
	cpdma_ctlr_int_ctrl(priv->dma, true);
	cpsw_intr_enable(priv);
	cpdma_ctlr_eoi(priv->dma);
}
#endif

static const struct net_device_ops cpsw_netdev_ops = {
	.ndo_open		= cpsw_ndo_open,
	.ndo_stop		= cpsw_ndo_stop,
	.ndo_start_xmit		= cpsw_ndo_start_xmit,
	.ndo_set_multicast_list	= cpsw_ndo_set_multicast_list,
	.ndo_change_rx_flags	= cpsw_ndo_change_rx_flags,
	.ndo_set_mac_address	= cpsw_ndo_set_mac_address,
	.ndo_do_ioctl		= cpsw_ndo_do_ioctl,
	.ndo_validate_addr	= eth_validate_addr,
	.ndo_tx_timeout		= cpsw_ndo_tx_timeout,
	.ndo_get_stats		= cpsw_ndo_get_stats,
#ifdef CONFIG_NET_POLL_CONTROLLER
	.ndo_poll_controller	= cpsw_ndo_poll_controller,
#endif
#ifdef VLAN_SUPPORT
	.ndo_vlan_rx_register	= cpsw_ndo_vlan_rx_register,
	.ndo_vlan_rx_add_vid	= cpsw_ndo_vlan_rx_add_vid,
	.ndo_vlan_rx_kill_vid	= cpsw_ndo_vlan_rx_kill_vid,
#endif /* VLAN_SUPPORT */
};

static void cpsw_get_drvinfo(struct net_device *ndev,
			     struct ethtool_drvinfo *info)
{
	struct cpsw_priv *priv = netdev_priv(ndev);
	strcpy(info->driver, "TI CPSW Driver v1.0");
	strcpy(info->version, "1.0");
	strcpy(info->bus_info, priv->pdev->name);
}

static u32 cpsw_get_msglevel(struct net_device *ndev)
{
	struct cpsw_priv *priv = netdev_priv(ndev);
	return priv->msg_enable;
}

static void cpsw_set_msglevel(struct net_device *ndev, u32 value)
{
	struct cpsw_priv *priv = netdev_priv(ndev);
	priv->msg_enable = value;
}

/**
 * cpsw_get_settings: Get EMAC settings
 * @ndev: CPSW network adapter
 * @ecmd: ethtool command
 *
 * Executes ethool get command
 *
 */
static int cpsw_get_settings(struct net_device *ndev,
			     struct ethtool_cmd *ecmd)
{
	struct cpsw_priv *priv = netdev_priv(ndev);
	int slave_no = cpsw_slave_phy_index(priv);

	if (priv->slaves[slave_no].phy)
		return phy_ethtool_gset(priv->slaves[slave_no].phy, ecmd);
	else
		return -EOPNOTSUPP;

}

/**
 * cpsw_set_settings: Set EMAC settings
 * @ndev: CPSW network adapter
 * @ecmd: ethtool command
 *
 * Executes ethool set command
 *
 */
static int cpsw_set_settings(struct net_device *ndev, struct ethtool_cmd *ecmd)
{
	struct cpsw_priv *priv = netdev_priv(ndev);
	int slave_no = cpsw_slave_phy_index(priv);

	if (priv->slaves[slave_no].phy)
		return phy_ethtool_sset(priv->slaves[slave_no].phy, ecmd);
	else
		return -EOPNOTSUPP;

}

/**
 * cpsw_get_coalesce : Get interrupt coalesce settings for this device
 * @ndev : CPSW network adapter
 * @coal : ethtool coalesce settings structure
 *
 * Fetch the current interrupt coalesce settings
 *
 */
static int cpsw_get_coalesce(struct net_device *ndev,
				struct ethtool_coalesce *coal)
{
	struct cpsw_priv *priv = netdev_priv(ndev);

	coal->rx_coalesce_usecs = priv->coal_intvl;
	return 0;

}

/**
 * cpsw_set_coalesce : Set interrupt coalesce settings for this device
 * @ndev : CPSW network adapter
 * @coal : ethtool coalesce settings structure
 *
 * Set interrupt coalesce parameters
 *
 */
static int cpsw_set_coalesce(struct net_device *ndev,
				struct ethtool_coalesce *coal)
{
	struct cpsw_priv *priv = netdev_priv(ndev);
	u32 int_ctrl;
	u32 num_interrupts = 0;
	u32 prescale = 0;
	u32 addnl_dvdr = 1;
	u32 coal_intvl = 0;

	if (!coal->rx_coalesce_usecs)
		return -EINVAL;

	coal_intvl = coal->rx_coalesce_usecs;

	int_ctrl =  __raw_readl(&priv->ss_regs->int_control);
	prescale = priv->bus_freq_mhz * 4;

	if (coal_intvl < CPSW_CMINTMIN_INTVL)
		coal_intvl = CPSW_CMINTMIN_INTVL;

	if (coal_intvl > CPSW_CMINTMAX_INTVL) {
		/*
		 * Interrupt pacer works with 4us Pulse, we can
		 * throttle further by dilating the 4us pulse.
		 */
		addnl_dvdr = CPSW_INTPRESCALE_MASK / prescale;

		if (addnl_dvdr > 1) {
			prescale *= addnl_dvdr;
			if (coal_intvl > (CPSW_CMINTMAX_INTVL * addnl_dvdr))
				coal_intvl = (CPSW_CMINTMAX_INTVL
						* addnl_dvdr);
		} else {
			addnl_dvdr = 1;
			coal_intvl = CPSW_CMINTMAX_INTVL;
		}
	}

	num_interrupts = (1000 * addnl_dvdr) / coal_intvl;

	int_ctrl |= CPSW_INTPACEEN;
	int_ctrl &= (~CPSW_INTPRESCALE_MASK);
	int_ctrl |= (prescale & CPSW_INTPRESCALE_MASK);
	__raw_writel(int_ctrl, &priv->ss_regs->int_control);

	__raw_writel(num_interrupts, &priv->ss_regs->rx_imax);
	__raw_writel(num_interrupts, &priv->ss_regs->tx_imax);

	printk(KERN_INFO "Set coalesce to %d usecs.\n", coal_intvl);
	priv->coal_intvl = coal_intvl;

	return 0;
}

static const struct ethtool_ops cpsw_ethtool_ops = {
	.get_drvinfo	= cpsw_get_drvinfo,
	.get_msglevel	= cpsw_get_msglevel,
	.set_msglevel	= cpsw_set_msglevel,
	.get_link	= ethtool_op_get_link,
	.get_settings	= cpsw_get_settings,
	.set_settings	= cpsw_set_settings,
	.get_coalesce	= cpsw_get_coalesce,
	.set_coalesce	= cpsw_set_coalesce,
};

static void cpsw_slave_init(struct cpsw_slave *slave, struct cpsw_priv *priv)
{
	void __iomem		*regs = priv->regs;
	int			slave_num = slave->slave_num;
	struct cpsw_slave_data	*data = priv->data.slave_data + slave_num;

	slave->data	= data;
	slave->regs	= regs + data->slave_reg_ofs;
	slave->sliver	= regs + data->sliver_reg_ofs;
#ifdef CONFIG_TI_CPSW_DUAL_EMAC
	slave->port_vlan = data->dual_emac_reserved_vlan;
#endif /* CONFIG_TI_CPSW_DUAL_EMAC */
}

#ifdef CONFIG_TI_CPSW_DUAL_EMAC

static inline void cpsw_deinit_slave_emac(struct cpsw_priv *priv)
{
	struct net_device *ndev = priv->slaves[1].ndev;

	unregister_netdev(ndev);
	free_netdev(ndev);
}

#else
#define cpsw_deinit_slave_emac(priv)
#endif

static int __devinit cpsw_probe(struct platform_device *pdev)
{
	struct cpsw_platform_data	*data = pdev->dev.platform_data;
	struct net_device		*ndev;
	struct cpsw_priv		*priv;
	struct cpdma_params		dma_params;
	struct cpsw_ale_params		ale_params;
	void __iomem			*regs;
	struct resource			*res;
	int ret = 0, i, k = 0;
#ifdef CONFIG_TI_CPSW_DUAL_EMAC
	/* Priv for second Interface */
	struct cpsw_priv		*priv_sl2;
#endif /* CONFIG_TI_CPSW_DUAL_EMAC */

	if (!data) {
		pr_err("cpsw: platform data missing\n");
		return -ENODEV;
	}

	ndev = alloc_etherdev(sizeof(struct cpsw_priv));
	if (!ndev) {
		pr_err("cpsw: error allocating net_device\n");
		return -ENOMEM;
	}

	platform_set_drvdata(pdev, ndev);
	priv = netdev_priv(ndev);
	spin_lock_init(&priv->lock);
	priv->data = *data;
	priv->pdev = pdev;
	priv->ndev = ndev;
	priv->dev  = &ndev->dev;
	priv->msg_enable = netif_msg_init(debug_level, CPSW_DEBUG);
	priv->rx_packet_max = max(rx_packet_max, 128);

	priv->cpts_time = kzalloc(sizeof(struct cpts_time_handle), GFP_KERNEL);
#if (defined(CONFIG_PTP_1588_CLOCK_CPTS) ||		\
		defined(CONFIG_PTP_1588_CLOCK_CPTS_MODULE))
	gpriv = priv;
#endif /* CONFIG_PTP_1588_CLOCK_CPTS */

	if (is_valid_ether_addr(data->mac_addr)) {
		memcpy(priv->mac_addr, data->mac_addr, ETH_ALEN);
		printk(KERN_INFO "Detected MACID=%x:%x:%x:%x:%x:%x\n",
			priv->mac_addr[0], priv->mac_addr[1],
			priv->mac_addr[2], priv->mac_addr[3],
			priv->mac_addr[4], priv->mac_addr[5]);
	} else {
		random_ether_addr(priv->mac_addr);
		printk(KERN_INFO "Random MACID=%x:%x:%x:%x:%x:%x\n",
			priv->mac_addr[0], priv->mac_addr[1],
			priv->mac_addr[2], priv->mac_addr[3],
			priv->mac_addr[4], priv->mac_addr[5]);
	}

	memcpy(ndev->dev_addr, priv->mac_addr, ETH_ALEN);

	priv->slaves = kzalloc(sizeof(struct cpsw_slave) * data->slaves,
			       GFP_KERNEL);
	if (!priv->slaves) {
		dev_err(priv->dev, "failed to allocate slave ports\n");
		ret = -EBUSY;
		goto clean_ndev_ret;
	}
	for (i = 0; i < data->slaves; i++)
		priv->slaves[i].slave_num = i;

	priv->clk = clk_get(&pdev->dev, NULL);
	if (IS_ERR(priv->clk)) {
		dev_err(priv->dev, "failed to get device clock\n");
	}

	priv->coal_intvl = 0;
	priv->bus_freq_mhz = clk_get_rate(priv->clk) / 1000000;

	priv->cpsw_res = platform_get_resource(pdev, IORESOURCE_MEM, 0);
	if (!priv->cpsw_res) {
		dev_err(priv->dev, "error getting i/o resource\n");
		ret = -ENOENT;
		goto clean_clk_ret;
	}

	if (!request_mem_region(priv->cpsw_res->start,
			resource_size(priv->cpsw_res), ndev->name)) {
		dev_err(priv->dev, "failed request i/o region\n");
		ret = -ENXIO;
		goto clean_clk_ret;
	}

	regs = ioremap(priv->cpsw_res->start, resource_size(priv->cpsw_res));
	if (!regs) {
		dev_err(priv->dev, "unable to map i/o region\n");
		goto clean_cpsw_iores_ret;
	}
	priv->regs = regs;
	priv->host_port = data->host_port_num;
	priv->host_port_regs = regs + data->host_port_reg_ofs;
	priv->hw_stats = regs + data->hw_stats_reg_ofs;

	priv->cpsw_ss_res = platform_get_resource(pdev, IORESOURCE_MEM, 1);
	if (!priv->cpsw_ss_res) {
		dev_err(priv->dev, "error getting i/o resource\n");
		ret = -ENOENT;
		goto clean_clk_ret;
	}

	if (!request_mem_region(priv->cpsw_ss_res->start,
			resource_size(priv->cpsw_ss_res), ndev->name)) {
		dev_err(priv->dev, "failed request i/o region\n");
		ret = -ENXIO;
		goto clean_clk_ret;
	}

	/* Init CPTS Regs offsets */
	priv->cpts_reg = regs + data->cpts_reg_ofs;

	regs = ioremap(priv->cpsw_ss_res->start,
				resource_size(priv->cpsw_ss_res));
	if (!regs) {
		dev_err(priv->dev, "unable to map i/o region\n");
		goto clean_cpsw_ss_iores_ret;
	}
	priv->ss_regs = regs;


	for_each_slave(priv, cpsw_slave_init, priv);

	memset(&dma_params, 0, sizeof(dma_params));
	dma_params.dev			= &pdev->dev;
	dma_params.dmaregs		= (void __iomem *)(((u32)priv->regs) +
						data->cpdma_reg_ofs);
	dma_params.rxthresh		= (void __iomem *)(((u32)priv->regs) +
						data->cpdma_reg_ofs + 0x0c0);
	dma_params.rxfree		= (void __iomem *)(((u32)priv->regs) +
						data->cpdma_reg_ofs + 0x0e0);
	dma_params.txhdp		= (void __iomem *)(((u32)priv->regs) +
						data->cpdma_sram_ofs);
	dma_params.rxhdp		= (void __iomem *)(((u32)priv->regs) +
						data->cpdma_sram_ofs + 0x020);
	dma_params.txcp			= (void __iomem *)(((u32)priv->regs) +
						data->cpdma_sram_ofs + 0x040);
	dma_params.rxcp			= (void __iomem *)(((u32)priv->regs) +
						data->cpdma_sram_ofs + 0x060);
	dma_params.num_chan		= data->channels;
	dma_params.has_soft_reset	= true;
	dma_params.min_packet_size	= CPSW_MIN_PACKET_SIZE;
	dma_params.desc_mem_size	= data->bd_ram_size;
	dma_params.desc_align		= 16;
	dma_params.has_ext_regs		= true;
	dma_params.desc_mem_phys	= data->no_bd_ram ? 0 :
			(u32 __force)priv->cpsw_res->start + data->bd_ram_ofs;
	dma_params.desc_hw_addr		= data->hw_ram_addr ?
				data->hw_ram_addr : dma_params.desc_mem_phys ;

	priv->dma = cpdma_ctlr_create(&dma_params);
	if (!priv->dma) {
		dev_err(priv->dev, "error initializing dma\n");
		ret = -ENOMEM;
		goto clean_iomap_ret;
	}

	priv->txch = cpdma_chan_create(priv->dma, tx_chan_num(0),
				       cpsw_tx_handler);
	priv->rxch = cpdma_chan_create(priv->dma, rx_chan_num(0),
				       cpsw_rx_handler);

	if (WARN_ON(!priv->txch || !priv->rxch)) {
		dev_err(priv->dev, "error initializing dma channels\n");
		ret = -ENOMEM;
		goto clean_dma_ret;
	}

	memset(&ale_params, 0, sizeof(ale_params));
	ale_params.dev			= &ndev->dev;
	ale_params.ale_regs		= (void *)((u32)priv->regs) +
						((u32)data->ale_reg_ofs);
	ale_params.ale_ageout		= ale_ageout;
	ale_params.ale_entries		= data->ale_entries;
	ale_params.ale_ports		= data->slaves;

	priv->ale = cpsw_ale_create(&ale_params);
	if (!priv->ale) {
		dev_err(priv->dev, "error initializing ale engine\n");
		ret = -ENODEV;
		goto clean_dma_ret;
	}

	ndev->irq = platform_get_irq(pdev, 0);
	if (ndev->irq < 0) {
		dev_err(priv->dev, "error getting irq resource\n");
		ret = -ENOENT;
		goto clean_ale_ret;
	}

	while ((res = platform_get_resource(priv->pdev, IORESOURCE_IRQ, k))) {
		for (i = res->start; i <= res->end; i++) {
			if (request_irq(i, cpsw_interrupt, IRQF_DISABLED,
					dev_name(&pdev->dev), priv)) {
				pr_err("error attaching irq\n");
				goto clean_ale_ret;
			}
			#ifdef CPSW_IRQ_QUIRK
			priv->irqs_table[k] = i;
			priv->num_irqs = k+1;
			#endif
		}
		k++;
	}

#ifdef VLAN_SUPPORT
	ndev->features |= NETIF_F_HW_VLAN_FILTER;
#endif /* VLAN_SUPPORT */

	ndev->flags |= IFF_ALLMULTI;	/* see cpsw_ndo_change_rx_flags() */

	ndev->netdev_ops = &cpsw_netdev_ops;
	SET_ETHTOOL_OPS(ndev, &cpsw_ethtool_ops);
	netif_napi_add(ndev, &priv->napi, cpsw_poll, CPSW_POLL_WEIGHT);

	/* register the network device */
	SET_NETDEV_DEV(ndev, &pdev->dev);
	ret = register_netdev(ndev);
	if (ret) {
		dev_err(priv->dev, "error registering net device\n");
		ret = -ENODEV;
		goto clean_irq_ret;
	}

	msg(notice, probe, "initialized device (regs %x, irq %d)\n",
	    priv->cpsw_res->start, ndev->irq);

#ifdef CONFIG_TI_CPSW_DUAL_EMAC
	priv->slaves[0].ndev = ndev;
	priv->emac_port = 0;

	ndev = alloc_etherdev(sizeof(struct cpsw_priv));
	if (!ndev) {
		pr_err("cpsw: error allocating net_device\n");
		return -ENOMEM;
	}

	priv_sl2 = netdev_priv(ndev);
	spin_lock_init(&priv_sl2->lock);
	priv_sl2->data = *data;
	priv_sl2->pdev = pdev;
	priv_sl2->ndev = ndev;
	priv_sl2->dev  = &ndev->dev;
	priv_sl2->msg_enable = netif_msg_init(debug_level, CPSW_DEBUG);
	priv_sl2->rx_packet_max = max(rx_packet_max, 128);
	priv_sl2->cpts_time = priv->cpts_time;

	/* Temprovary work around as both mac address are same in e-fuse */
	if (!memcmp(data->slave_data[0].mac_addr,
			data->slave_data[1].mac_addr, ETH_ALEN)) {
		printk(KERN_INFO "Both MAC IDs are same in e-fuse\n");
		random_ether_addr(data->slave_data[1].mac_addr);
	}

	if (is_valid_ether_addr(data->slave_data[1].mac_addr)) {
		memcpy(priv_sl2->mac_addr, data->slave_data[1].mac_addr,
					ETH_ALEN);
		printk(KERN_INFO "Detected MACID=%x:%x:%x:%x:%x:%x\n",
			priv_sl2->mac_addr[0], priv_sl2->mac_addr[1],
			priv_sl2->mac_addr[2], priv_sl2->mac_addr[3],
			priv_sl2->mac_addr[4], priv_sl2->mac_addr[5]);
	} else {
		random_ether_addr(priv_sl2->mac_addr);
		printk(KERN_INFO "Random MACID=%x:%x:%x:%x:%x:%x\n",
			priv_sl2->mac_addr[0], priv_sl2->mac_addr[1],
			priv_sl2->mac_addr[2], priv_sl2->mac_addr[3],
			priv_sl2->mac_addr[4], priv_sl2->mac_addr[5]);
	}
	memcpy(ndev->dev_addr, priv_sl2->mac_addr, ETH_ALEN);

	priv_sl2->slaves = priv->slaves;
	priv_sl2->clk = priv->clk;

	priv_sl2->coal_intvl = 0;
	priv_sl2->bus_freq_mhz = clk_get_rate(priv_sl2->clk) / 1000000;

	priv_sl2->cpsw_res = priv->cpsw_res;
	priv_sl2->regs = priv->regs;
	priv_sl2->host_port = priv->host_port;
	priv_sl2->host_port_regs = priv->host_port_regs;
	priv_sl2->hw_stats = priv->hw_stats;
	priv_sl2->cpsw_ss_res = priv->cpsw_ss_res;
	priv_sl2->ss_regs = priv->ss_regs;
	priv_sl2->dma = priv->dma;
	priv_sl2->txch = priv->txch;
	priv_sl2->rxch = priv->rxch;
	priv_sl2->ale = priv->ale;
	priv_sl2->cpts_reg = priv->cpts_reg;
	for (i = 0; i < priv->num_irqs; i++) {
		#ifdef CPSW_IRQ_QUIRK
		priv_sl2->irqs_table[i] = priv->irqs_table[i];
		priv_sl2->num_irqs = priv->num_irqs;
		#endif
	}

#ifdef VLAN_SUPPORT
	ndev->features |= NETIF_F_HW_VLAN_FILTER;
#endif /* VLAN_SUPPORT */
	ndev->flags |= IFF_ALLMULTI;	/* see cpsw_ndo_change_rx_flags() */

	ndev->netdev_ops = &cpsw_netdev_ops;
	SET_ETHTOOL_OPS(ndev, &cpsw_ethtool_ops);
	netif_napi_add(ndev, &priv_sl2->napi, cpsw_poll, CPSW_POLL_WEIGHT);

	/* register the network device */
	SET_NETDEV_DEV(ndev, &pdev->dev);
	ret = register_netdev(ndev);
	if (ret) {
		dev_err(priv->dev, "error registering net device\n");
		ret = -ENODEV;
		goto clean_irq_ret;
	}

	priv_sl2->emac_port = 1;
	priv->slaves[1].ndev = ndev;
#endif /* CONFIG_TI_CPSW_DUAL_EMAC */

	return 0;

clean_irq_ret:
	free_irq(ndev->irq, priv);
clean_ale_ret:
	cpsw_ale_destroy(priv->ale);
clean_dma_ret:
	cpdma_chan_destroy(priv->txch);
	cpdma_chan_destroy(priv->rxch);
	cpdma_ctlr_destroy(priv->dma);
clean_iomap_ret:
	iounmap(priv->regs);
clean_cpsw_ss_iores_ret:
	release_mem_region(priv->cpsw_ss_res->start,
				resource_size(priv->cpsw_ss_res));
clean_cpsw_iores_ret:
	release_mem_region(priv->cpsw_res->start,
				resource_size(priv->cpsw_res));
clean_clk_ret:
	clk_put(priv->clk);
	kfree(priv->cpts_time);
	kfree(priv->slaves);
clean_ndev_ret:
	free_netdev(ndev);
	return ret;
}

static int __devexit cpsw_remove(struct platform_device *pdev)
{
	struct net_device *ndev = platform_get_drvdata(pdev);
	struct cpsw_priv *priv = netdev_priv(ndev);
	int i;

	msg(notice, probe, "removing device\n");
	platform_set_drvdata(pdev, NULL);

	cpsw_deinit_slave_emac(priv);
	for (i = 0; i < priv->num_irqs; i++)
		free_irq(priv->irqs_table[i], priv);
	cpsw_ale_destroy(priv->ale);
	cpdma_chan_destroy(priv->txch);
	cpdma_chan_destroy(priv->rxch);
	cpdma_ctlr_destroy(priv->dma);
	iounmap(priv->regs);
	release_mem_region(priv->cpsw_res->start,
					resource_size(priv->cpsw_res));
	release_mem_region(priv->cpsw_ss_res->start,
					resource_size(priv->cpsw_ss_res));
	clk_put(priv->clk);
	kfree(priv->cpts_time);
	kfree(priv->slaves);
	unregister_netdev(ndev);
	free_netdev(ndev);

	return 0;
}

static int cpsw_suspend(struct device *dev)
{
	struct platform_device	*pdev = to_platform_device(dev);
	struct net_device	*ndev = platform_get_drvdata(pdev);

#ifdef CONFIG_TI_CPSW_DUAL_EMAC
	struct cpsw_priv *priv = netdev_priv(ndev);
	u32 i = 0;

	while (i < priv->data.slaves) {
		if (netif_running(priv->slaves[i].ndev))
			cpsw_ndo_stop(priv->slaves[i].ndev);
		i++;
	}
#else /* CONFIG_TI_CPSW_DUAL_EMAC */
	if (netif_running(ndev))
		cpsw_ndo_stop(ndev);
#endif /* CONFIG_TI_CPSW_DUAL_EMAC */

	return 0;
}

static int cpsw_resume(struct device *dev)
{
	struct platform_device	*pdev = to_platform_device(dev);
	struct net_device	*ndev = platform_get_drvdata(pdev);

#ifdef CONFIG_TI_CPSW_DUAL_EMAC
	struct cpsw_priv *priv = netdev_priv(ndev);
	u32 i = 0;

	while (i < priv->data.slaves) {
		if (netif_running(priv->slaves[i].ndev))
			cpsw_ndo_open(priv->slaves[i].ndev);
		i++;
	}
#else /* CONFIG_TI_CPSW_DUAL_EMAC */
	if (netif_running(ndev))
		cpsw_ndo_open(ndev);
#endif /* CONFIG_TI_CPSW_DUAL_EMAC */

	return 0;
}

static const struct dev_pm_ops cpsw_pm_ops = {
	.suspend	= cpsw_suspend,
	.resume		= cpsw_resume,
};

static struct platform_driver cpsw_driver = {
	.driver = {
		.name	 = "cpsw",
		.owner	 = THIS_MODULE,
		.pm	 = &cpsw_pm_ops,
	},
	.probe = cpsw_probe,
	.remove = __devexit_p(cpsw_remove),
};

static int __init cpsw_init(void)
{
	return platform_driver_register(&cpsw_driver);
}
late_initcall(cpsw_init);

static void __exit cpsw_exit(void)
{
	platform_driver_unregister(&cpsw_driver);
}
module_exit(cpsw_exit);

MODULE_LICENSE("GPL");
MODULE_DESCRIPTION("TI CPSW Ethernet driver");
