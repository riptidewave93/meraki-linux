/*
 * drivers/net/ibm_newemac/mal.c
 *
 * Memory Access Layer (MAL) support
 *
 * Copyright 2007 Benjamin Herrenschmidt, IBM Corp.
 *                <benh@kernel.crashing.org>
 *
 * Based on the arch/ppc version of the driver:
 *
 * Copyright (c) 2004, 2005 Zultys Technologies.
 * Eugene Surovegin <eugene.surovegin@zultys.com> or <ebs@ebshome.net>
 *
 * Based on original work by
 *      Benjamin Herrenschmidt <benh@kernel.crashing.org>,
 *      David Gibson <hermes@gibson.dropbear.id.au>,
 *
 *      Armin Kuster <akuster@mvista.com>
 *      Copyright 2002 MontaVista Softare Inc.
 *
 * This program is free software; you can redistribute  it and/or modify it
 * under  the terms of  the GNU General  Public License as published by the
 * Free Software Foundation;  either version 2 of the  License, or (at your
 * option) any later version.
 *
 */

#include <linux/delay.h>

#include "core.h"
#include <asm/dcr-regs.h>
#include <asm/ppc4xx_ocm.h>

static int mal_count;
#ifdef CONFIG_IBM_NEW_EMAC_INTR_COALESCE
static char *tx_coal_irqname[] = {
	"TX0 COAL",
	"TX1 COAL",
	"TX2 COAL",
	"TX3 COAL",
};
static char *rx_coal_irqname[] = {
	"RX0 COAL",
	"RX1 COAL",
	"RX2 COAL",
	"RX3 COAL",
};
#endif

int __devinit mal_register_commac(struct mal_instance	*mal,
				  struct mal_commac	*commac)
{
	unsigned long flags;

	spin_lock_irqsave(&mal->lock, flags);

	MAL_DBG(mal, "reg(%08x, %08x)" NL,
		commac->tx_chan_mask, commac->rx_chan_mask);

	/* Don't let multiple commacs claim the same channel(s) */
	if ((mal->tx_chan_mask & commac->tx_chan_mask) ||
	    (mal->rx_chan_mask & commac->rx_chan_mask)) {
		spin_unlock_irqrestore(&mal->lock, flags);
		printk(KERN_WARNING "mal%d: COMMAC channels conflict!\n",
		       mal->index);
		return -EBUSY;
	}

	if (list_empty(&mal->list))
		napi_enable(&mal->napi);
	mal->tx_chan_mask |= commac->tx_chan_mask;
	mal->rx_chan_mask |= commac->rx_chan_mask;
	list_add(&commac->list, &mal->list);

	spin_unlock_irqrestore(&mal->lock, flags);

	return 0;
}

void mal_unregister_commac(struct mal_instance	*mal,
		struct mal_commac *commac)
{
	unsigned long flags;

	spin_lock_irqsave(&mal->lock, flags);

	MAL_DBG(mal, "unreg(%08x, %08x)" NL,
		commac->tx_chan_mask, commac->rx_chan_mask);

	mal->tx_chan_mask &= ~commac->tx_chan_mask;
	mal->rx_chan_mask &= ~commac->rx_chan_mask;
	list_del_init(&commac->list);
	if (list_empty(&mal->list))
		napi_disable(&mal->napi);

	spin_unlock_irqrestore(&mal->lock, flags);
}

int mal_set_rcbs(struct mal_instance *mal, int channel, unsigned long size)
{
	BUG_ON(channel < 0 || channel >= mal->num_rx_chans ||
	       size > MAL_MAX_RX_SIZE);

	MAL_DBG(mal, "set_rbcs(%d, %lu)" NL, channel, size);

	if (size & 0xf) {
		printk(KERN_WARNING
		       "mal%d: incorrect RX size %lu for the channel %d\n",
		       mal->index, size, channel);
		return -EINVAL;
	}

	set_mal_dcrn(mal, MAL_RCBS(channel), size >> 4);
	return 0;
}

int mal_tx_bd_offset(struct mal_instance *mal, int channel)
{
	BUG_ON(channel < 0 || channel >= mal->num_tx_chans);

	return channel * NUM_TX_BUFF;
}

int mal_rx_bd_offset(struct mal_instance *mal, int channel)
{
	BUG_ON(channel < 0 || channel >= mal->num_rx_chans);
	return mal->num_tx_chans * NUM_TX_BUFF + channel * NUM_RX_BUFF;
}

void mal_enable_tx_channel(struct mal_instance *mal, int channel)
{
	unsigned long flags;

	spin_lock_irqsave(&mal->lock, flags);

	MAL_DBG(mal, "enable_tx(%d)" NL, channel);

	set_mal_dcrn(mal, MAL_TXCASR,
		     get_mal_dcrn(mal, MAL_TXCASR) | MAL_CHAN_MASK(channel));

	spin_unlock_irqrestore(&mal->lock, flags);
}

void mal_disable_tx_channel(struct mal_instance *mal, int channel)
{
	set_mal_dcrn(mal, MAL_TXCARR, MAL_CHAN_MASK(channel));

	MAL_DBG(mal, "disable_tx(%d)" NL, channel);
}

void mal_enable_rx_channel(struct mal_instance *mal, int channel)
{
	unsigned long flags;

	/*
	 * On some 4xx PPC's (e.g. 460EX/GT), the rx channel is a multiple
	 * of 8, but enabling in MAL_RXCASR needs the divided by 8 value
	 * for the bitmask
	 */
	if (!(channel % 8))
		channel >>= 3;

	spin_lock_irqsave(&mal->lock, flags);

	MAL_DBG(mal, "enable_rx(%d)" NL, channel);

	set_mal_dcrn(mal, MAL_RXCASR,
		     get_mal_dcrn(mal, MAL_RXCASR) | MAL_CHAN_MASK(channel));

	spin_unlock_irqrestore(&mal->lock, flags);
}

void mal_disable_rx_channel(struct mal_instance *mal, int channel)
{
	/*
	 * On some 4xx PPC's (e.g. 460EX/GT), the rx channel is a multiple
	 * of 8, but enabling in MAL_RXCASR needs the divided by 8 value
	 * for the bitmask
	 */
	if (!(channel % 8))
		channel >>= 3;

	set_mal_dcrn(mal, MAL_RXCARR, MAL_CHAN_MASK(channel));

	MAL_DBG(mal, "disable_rx(%d)" NL, channel);
}

void mal_poll_add(struct mal_instance *mal, struct mal_commac *commac)
{
	unsigned long flags;

	spin_lock_irqsave(&mal->lock, flags);

	MAL_DBG(mal, "poll_add(%p)" NL, commac);

	/* starts disabled */
	set_bit(MAL_COMMAC_POLL_DISABLED, &commac->flags);

	list_add_tail(&commac->poll_list, &mal->poll_list);

	spin_unlock_irqrestore(&mal->lock, flags);
}

void mal_poll_del(struct mal_instance *mal, struct mal_commac *commac)
{
	unsigned long flags;

	spin_lock_irqsave(&mal->lock, flags);

	MAL_DBG(mal, "poll_del(%p)" NL, commac);

	list_del(&commac->poll_list);

	spin_unlock_irqrestore(&mal->lock, flags);
}

/* synchronized by mal_poll() */
static inline void mal_enable_eob_irq(struct mal_instance *mal)
{
	MAL_DBG2(mal, "enable_irq" NL);

	// XXX might want to cache MAL_CFG as the DCR read can be slooooow
	set_mal_dcrn(mal, MAL_CFG, get_mal_dcrn(mal, MAL_CFG) | MAL_CFG_EOPIE);
}

/* synchronized by NAPI state */
static inline void mal_disable_eob_irq(struct mal_instance *mal)
{
	// XXX might want to cache MAL_CFG as the DCR read can be slooooow
	set_mal_dcrn(mal, MAL_CFG, get_mal_dcrn(mal, MAL_CFG) & ~MAL_CFG_EOPIE);

	MAL_DBG2(mal, "disable_irq" NL);
}

#ifdef CONFIG_IBM_NEW_EMAC_INTR_COALESCE

#if defined(CONFIG_460SX)
/* Set Tx fram count */
static inline void set_ic_txfthr(struct mal_instance *mal)
{
	int reg;
	int val = mal->coales_param[0].tx_count;

	reg = (val<<23) | (1<<22) ;
	
	SDR_WRITE(DCRN_SDR0_ICCRTX0, reg);	/* set counter */
	SDR_WRITE(DCRN_SDR0_ICCRTX0,(val<<23));	/* enable counter */

	SDR_WRITE(DCRN_SDR0_ICCRTX1, reg);        /* set counter */
        SDR_WRITE(DCRN_SDR0_ICCRTX1,(val<<23));  /* enable counter */

	SDR_WRITE(DCRN_SDR0_ICCRTX2, reg);        /* set counter */
        SDR_WRITE(DCRN_SDR0_ICCRTX2,(val<<23));  /* enable counter */

	SDR_WRITE(DCRN_SDR0_ICCRTX3, reg);        /* set counter */
        SDR_WRITE(DCRN_SDR0_ICCRTX3,(val<<23));  /* enable counter */


	mal->enet_coales_iccrtx = reg;
}
/* Set Rx fram count */
static inline void set_ic_rxfthr(struct mal_instance *mal)
{
	int reg;
	int val = mal->coales_param[0].rx_count;
	
	reg = (val<<23) | (1<<22) ;

	SDR_WRITE(DCRN_SDR0_ICCRRX0,reg);	/* set counter */
	SDR_WRITE(DCRN_SDR0_ICCRRX0,(val<<23));	/* enable counter */

	SDR_WRITE(DCRN_SDR0_ICCRRX1,reg);        /* set counter */
        SDR_WRITE(DCRN_SDR0_ICCRRX1,(val<<23));  /* enable counter */

	SDR_WRITE(DCRN_SDR0_ICCRRX2,reg);        /* set counter */
        SDR_WRITE(DCRN_SDR0_ICCRRX2,(val<<23));  /* enable counter */

	SDR_WRITE(DCRN_SDR0_ICCRRX3,reg);        /* set counter */
        SDR_WRITE(DCRN_SDR0_ICCRRX3,(val<<23));  /* enable counter */

	mal->enet_coales_iccrrx = reg;
}
#endif

inline void mal_enable_coal(struct mal_instance *mal)
{
	unsigned int val;
#if defined(CONFIG_405EX)
	/* Clear the counters */
	val = SDR0_ICC_FLUSH0 | SDR0_ICC_FLUSH1;
	mtdcri(SDR0, DCRN_SDR0_ICCRTX, val);
	mtdcri(SDR0, DCRN_SDR0_ICCRRX, val);

	/* Set Tx/Rx Timer values */
	mtdcri(SDR0, DCRN_SDR0_ICCTRTX0, CONFIG_IBM_NEW_EMAC_TX_COAL_TIMER);
	mtdcri(SDR0, DCRN_SDR0_ICCTRTX1, CONFIG_IBM_NEW_EMAC_TX_COAL_TIMER);
	mtdcri(SDR0, DCRN_SDR0_ICCTRRX0, CONFIG_IBM_NEW_EMAC_RX_COAL_TIMER);
	mtdcri(SDR0, DCRN_SDR0_ICCTRRX1, CONFIG_IBM_NEW_EMAC_RX_COAL_TIMER);

	/* Enable the Tx/Rx Coalescing interrupt */
	val = ((CONFIG_IBM_NEW_EMAC_TX_COAL_COUNT & COAL_FRAME_MASK)
			<< SDR0_ICC_FTHR0_SHIFT) |
		((CONFIG_IBM_NEW_EMAC_TX_COAL_COUNT & COAL_FRAME_MASK)
			<< SDR0_ICC_FTHR1_SHIFT);
	mtdcri(SDR0, DCRN_SDR0_ICCRTX, val);

	val = ((CONFIG_IBM_NEW_EMAC_RX_COAL_COUNT & COAL_FRAME_MASK)
			<< SDR0_ICC_FTHR0_SHIFT) |
		((CONFIG_IBM_NEW_EMAC_RX_COAL_COUNT & COAL_FRAME_MASK)
			<< SDR0_ICC_FTHR1_SHIFT);

	mtdcri(SDR0, DCRN_SDR0_ICCRRX, val);
#elif defined(CONFIG_APM82181)
	/* Clear the counters */
        val = SDR0_ICC_FLUSH;
        mtdcri(SDR0, DCRN_SDR0_ICCRTX0, val);
	mtdcri(SDR0, DCRN_SDR0_ICCRRX0, val);

	/* Set Tx/Rx Timer values */
        mtdcri(SDR0, DCRN_SDR0_ICCTRTX0,  mal->coales_param[0].tx_time);
        mtdcri(SDR0, DCRN_SDR0_ICCTRRX0,  mal->coales_param[0].rx_time);

	/* Enable the Tx/Rx Coalescing interrupt */	
	val = (mal->coales_param[0].tx_count & COAL_FRAME_MASK)
                        << SDR0_ICC_FTHR_SHIFT;
        mtdcri(SDR0, DCRN_SDR0_ICCRTX0, val);
        
	val = (mal->coales_param[0].rx_count & COAL_FRAME_MASK)
                        << SDR0_ICC_FTHR_SHIFT;
        mtdcri(SDR0, DCRN_SDR0_ICCRRX0, val);

#elif defined(CONFIG_460EX) || defined(CONFIG_460GT)
	/* Clear the counters */
	val = SDR0_ICC_FLUSH;
	mtdcri(SDR0, DCRN_SDR0_ICCRTX0, val);
	mtdcri(SDR0, DCRN_SDR0_ICCRTX1, val);
	mtdcri(SDR0, DCRN_SDR0_ICCRRX0, val);
	mtdcri(SDR0, DCRN_SDR0_ICCRRX1, val);
#if defined(CONFIG_460GT)
	mtdcri(SDR0, DCRN_SDR0_ICCRTX2, val);
	mtdcri(SDR0, DCRN_SDR0_ICCRTX3, val);
	mtdcri(SDR0, DCRN_SDR0_ICCRRX2, val);
	mtdcri(SDR0, DCRN_SDR0_ICCRRX3, val);
#endif

	/* Set Tx/Rx Timer values */
	mtdcri(SDR0, DCRN_SDR0_ICCTRTX0, CONFIG_IBM_NEW_EMAC_TX_COAL_TIMER);
	mtdcri(SDR0, DCRN_SDR0_ICCTRTX1, CONFIG_IBM_NEW_EMAC_TX_COAL_TIMER);
	mtdcri(SDR0, DCRN_SDR0_ICCTRRX0, CONFIG_IBM_NEW_EMAC_RX_COAL_TIMER);
	mtdcri(SDR0, DCRN_SDR0_ICCTRRX1, CONFIG_IBM_NEW_EMAC_RX_COAL_TIMER);
#if defined(CONFIG_460GT)
	mtdcri(SDR0, DCRN_SDR0_ICCTRTX2, CONFIG_IBM_NEW_EMAC_TX_COAL_TIMER);
	mtdcri(SDR0, DCRN_SDR0_ICCTRTX3, CONFIG_IBM_NEW_EMAC_TX_COAL_TIMER);
	mtdcri(SDR0, DCRN_SDR0_ICCTRRX2, CONFIG_IBM_NEW_EMAC_RX_COAL_TIMER);
	mtdcri(SDR0, DCRN_SDR0_ICCTRRX3, CONFIG_IBM_NEW_EMAC_RX_COAL_TIMER);
#endif

	/* Enable the Tx/Rx Coalescing interrupt */
	val = (CONFIG_IBM_NEW_EMAC_TX_COAL_COUNT & COAL_FRAME_MASK)
			<< SDR0_ICC_FTHR_SHIFT;
	mtdcri(SDR0, DCRN_SDR0_ICCRTX0, val);
	mtdcri(SDR0, DCRN_SDR0_ICCRTX1, val);
#if defined(CONFIG_460GT)
	mtdcri(SDR0, DCRN_SDR0_ICCRTX2, val);
	mtdcri(SDR0, DCRN_SDR0_ICCRTX3, val);
#endif

	val = (CONFIG_IBM_NEW_EMAC_RX_COAL_COUNT & COAL_FRAME_MASK)
			<< SDR0_ICC_FTHR_SHIFT;
	mtdcri(SDR0, DCRN_SDR0_ICCRRX0, val);
	mtdcri(SDR0, DCRN_SDR0_ICCRRX1, val);
#if defined(CONFIG_460GT)
	mtdcri(SDR0, DCRN_SDR0_ICCRRX2, val);
	mtdcri(SDR0, DCRN_SDR0_ICCRRX3, val);
#endif

#elif defined(CONFIG_460SX)
	mtdcri(SDR0, DCRN_SDR0_ICCTRTX0,  mal->coales_param[0].tx_time);
	mtdcri(SDR0, DCRN_SDR0_ICCTRTX1,  mal->coales_param[1].tx_time);
	mtdcri(SDR0, DCRN_SDR0_ICCTRTX2,  mal->coales_param[2].tx_time);
	mtdcri(SDR0, DCRN_SDR0_ICCTRTX3,  mal->coales_param[3].tx_time);

	mtdcri(SDR0, DCRN_SDR0_ICCTRRX0,  mal->coales_param[0].rx_time);
	mtdcri(SDR0, DCRN_SDR0_ICCTRRX1,  mal->coales_param[1].rx_time);
	mtdcri(SDR0, DCRN_SDR0_ICCTRRX2,  mal->coales_param[2].rx_time);
	mtdcri(SDR0, DCRN_SDR0_ICCTRRX3,  mal->coales_param[3].rx_time);

	set_ic_rxfthr(mal);
	set_ic_txfthr(mal);
#endif
	printk(KERN_INFO "MAL: Enabled Interrupt Coal TxCnt: %d RxCnt: %d\n",
		mal->coales_param[0].tx_count,
		mal->coales_param[0].rx_count);

	printk(KERN_INFO "                            TxTimer: %d RxTimer: %d\n",
		mal->coales_param[0].tx_time,
		mal->coales_param[0].rx_time);
}
#endif

static irqreturn_t mal_serr(int irq, void *dev_instance)
{
	struct mal_instance *mal = dev_instance;
	struct list_head *l;

	u32 esr = get_mal_dcrn(mal, MAL_ESR);

	/* Clear the error status register */
	set_mal_dcrn(mal, MAL_ESR, esr);

	MAL_DBG(mal, "SERR %08x" NL, esr);

	if (esr & MAL_ESR_EVB) {
		if (esr & MAL_ESR_DE) {
			/* We ignore Descriptor error,
			 * TXDE or RXDE interrupt will be generated anyway.
			 */
			return IRQ_HANDLED;
		}

		if (esr & MAL_ESR_PEIN) {
			/* PLB error, it's probably buggy hardware or
			 * incorrect physical address in BD (i.e. bug)
			 */
			if (net_ratelimit())
				printk(KERN_ERR
				       "mal%d: system error, "
				       "PLB (ESR = 0x%08x)\n",
				       mal->index, esr);
			return IRQ_HANDLED;
		}

		/* OPB error, it's probably buggy hardware or incorrect
		 * EBC setup
		 */
		if (net_ratelimit())
			printk(KERN_ERR
			       "mal%d: system error, OPB (ESR = 0x%08x)\n",
			       mal->index, esr);
	}


	list_for_each(l, &mal->poll_list) {
		struct mal_commac *mc =
			list_entry(l, struct mal_commac, poll_list);
		mc->ops->reset(mc->dev);
	}

	return IRQ_HANDLED;
}

static inline void mal_schedule_poll(struct mal_instance *mal)
{
	if (likely(napi_schedule_prep(&mal->napi))) {
		MAL_DBG2(mal, "schedule_poll" NL);
		mal_disable_eob_irq(mal);
		__napi_schedule(&mal->napi);
	} else
		MAL_DBG2(mal, "already in poll" NL);
}

static irqreturn_t mal_txeob(int irq, void *dev_instance)
{
	struct mal_instance *mal = dev_instance;

	u32 r = get_mal_dcrn(mal, MAL_TXEOBISR);

	MAL_DBG2(mal, "txeob %08x" NL, r);

	mal_schedule_poll(mal);
	set_mal_dcrn(mal, MAL_TXEOBISR, r);

#ifdef CONFIG_PPC_DCR_NATIVE
	if (mal_has_feature(mal, MAL_FTR_CLEAR_ICINTSTAT))
		mtdcri(SDR0, DCRN_SDR_ICINTSTAT,
				(mfdcri(SDR0, DCRN_SDR_ICINTSTAT) | ICINTSTAT_ICTX));
#endif

	return IRQ_HANDLED;
}

static irqreturn_t mal_rxeob(int irq, void *dev_instance)
{
	struct mal_instance *mal = dev_instance;

	u32 r = get_mal_dcrn(mal, MAL_RXEOBISR);

	MAL_DBG2(mal, "rxeob %08x" NL, r);

	mal_schedule_poll(mal);
	set_mal_dcrn(mal, MAL_RXEOBISR, r);

#ifdef CONFIG_PPC_DCR_NATIVE
	if (mal_has_feature(mal, MAL_FTR_CLEAR_ICINTSTAT))
		mtdcri(SDR0, DCRN_SDR_ICINTSTAT,
				(mfdcri(SDR0, DCRN_SDR_ICINTSTAT) | ICINTSTAT_ICRX));
#endif

	return IRQ_HANDLED;
}

#ifdef CONFIG_IBM_NEW_EMAC_INTR_COALESCE
static irqreturn_t mal_coal(int irq, void *dev_instance)
{
	struct mal_instance *mal = dev_instance;
	mal_schedule_poll(mal);
	return IRQ_HANDLED;
}
#endif

static irqreturn_t mal_txde(int irq, void *dev_instance)
{
	struct mal_instance *mal = dev_instance;

	u32 deir = get_mal_dcrn(mal, MAL_TXDEIR);
	set_mal_dcrn(mal, MAL_TXDEIR, deir);

	MAL_DBG(mal, "txde %08x" NL, deir);

	if (net_ratelimit())
		printk(KERN_ERR
		       "mal%d: TX descriptor error (TXDEIR = 0x%08x)\n",
		       mal->index, deir);

	return IRQ_HANDLED;
}

static irqreturn_t mal_rxde(int irq, void *dev_instance)
{
	struct mal_instance *mal = dev_instance;
	struct list_head *l;

	u32 deir = get_mal_dcrn(mal, MAL_RXDEIR);

	MAL_DBG(mal, "rxde %08x" NL, deir);

	list_for_each(l, &mal->list) {
		struct mal_commac *mc = list_entry(l, struct mal_commac, list);
		if (deir & mc->rx_chan_mask) {
			set_bit(MAL_COMMAC_RX_STOPPED, &mc->flags);
			mc->ops->rxde(mc->dev);
		}
	}

	mal_schedule_poll(mal);
	set_mal_dcrn(mal, MAL_RXDEIR, deir);

	return IRQ_HANDLED;
}

static irqreturn_t mal_int(int irq, void *dev_instance)
{
	struct mal_instance *mal = dev_instance;
	u32 esr = get_mal_dcrn(mal, MAL_ESR);

	if (esr & MAL_ESR_EVB) {
		/* descriptor error */
		if (esr & MAL_ESR_DE) {
			if (esr & MAL_ESR_CIDT)
				return mal_rxde(irq, dev_instance);
			else
				return mal_txde(irq, dev_instance);
		} else { /* SERR */
			return mal_serr(irq, dev_instance);
		}
	}
	return IRQ_HANDLED;
}

void mal_poll_disable(struct mal_instance *mal, struct mal_commac *commac)
{
	/* Spinlock-type semantics: only one caller disable poll at a time */
	while (test_and_set_bit(MAL_COMMAC_POLL_DISABLED, &commac->flags))
		msleep(1);

	/* Synchronize with the MAL NAPI poller */
	napi_synchronize(&mal->napi);
}

void mal_poll_enable(struct mal_instance *mal, struct mal_commac *commac)
{
	smp_wmb();
	clear_bit(MAL_COMMAC_POLL_DISABLED, &commac->flags);

	/* Feels better to trigger a poll here to catch up with events that
	 * may have happened on this channel while disabled. It will most
	 * probably be delayed until the next interrupt but that's mostly a
	 * non-issue in the context where this is called.
	 */
	napi_schedule(&mal->napi);
}

static int mal_poll(struct napi_struct *napi, int budget)
{
#if defined(CONFIG_IBM_EMAC_MAL_QOS_V404)
	int v;
#endif
	struct mal_instance *mal = container_of(napi, struct mal_instance, napi);
	struct list_head *l;
	int received = 0;
	unsigned long flags;

	MAL_DBG2(mal, "poll(%d)" NL, budget);
 again:
	/* Process TX skbs */
	list_for_each(l, &mal->poll_list) {
		struct mal_commac *mc =
			list_entry(l, struct mal_commac, poll_list);
		mc->ops->poll_tx(mc->dev);
	}

	/* Process RX skbs.
	 *
	 * We _might_ need something more smart here to enforce polling
	 * fairness.
	 */
	list_for_each(l, &mal->poll_list) {
		struct mal_commac *mc =
			list_entry(l, struct mal_commac, poll_list);
		int n;
		if (unlikely(test_bit(MAL_COMMAC_POLL_DISABLED, &mc->flags)))
			continue;
		n = mc->ops->poll_rx(mc->dev, budget);
		if (n) {
			received += n;
			budget -= n;
			if (budget <= 0)
				goto more_work; // XXX What if this is the last one ?
		}
	}

	/* We need to disable IRQs to protect from RXDE IRQ here */
	spin_lock_irqsave(&mal->lock, flags);
	__napi_complete(napi);
	mal_enable_eob_irq(mal);
	spin_unlock_irqrestore(&mal->lock, flags);

	/* Check for "rotting" packet(s) */
	list_for_each(l, &mal->poll_list) {
		struct mal_commac *mc =
			list_entry(l, struct mal_commac, poll_list);
		if (unlikely(test_bit(MAL_COMMAC_POLL_DISABLED, &mc->flags)))
			continue;
		if (unlikely(mc->ops->peek_rx(mc->dev) ||
			     test_bit(MAL_COMMAC_RX_STOPPED, &mc->flags))) {
			MAL_DBG2(mal, "rotting packet" NL);
			if (napi_reschedule(napi))
				mal_disable_eob_irq(mal);
			else
				MAL_DBG2(mal, "already in poll list" NL);

			if (budget > 0)
				goto again;
			else
				goto more_work;
		}
		mc->ops->poll_tx(mc->dev);
	}

#if defined(CONFIG_IBM_EMAC_MAL_QOS_V404)
	/* Process RX skbs QOS virtual channels.
	 * 
	 */
	for ( v = 1; v < MAX_VCHANS; v++ ) {
		list_for_each(l, &mal->poll_list) {
			struct mal_commac *mc = 
				list_entry(l, struct mal_commac, poll_list);
			struct emac_instance *dev = mc->dev;
			int n;
			if ( v >= dev->rx_vchans ) {
				continue;
			}
			n = mc->ops->poll_rx(dev->vdev[v],budget);			
			if (n) {
				received += n;
				budget -= n;
				if (budget <= 0) {				
					goto more_work;
				}
			}
		}
	
	}
#endif

 more_work:
	MAL_DBG2(mal, "poll() %d <- %d" NL, budget, received);
	return received;
}

static void mal_reset(struct mal_instance *mal)
{
	int n = 10;

	MAL_DBG(mal, "reset" NL);

	set_mal_dcrn(mal, MAL_CFG, MAL_CFG_SR);

	/* Wait for reset to complete (1 system clock) */
	while ((get_mal_dcrn(mal, MAL_CFG) & MAL_CFG_SR) && n)
		--n;

	if (unlikely(!n))
		printk(KERN_ERR "mal%d: reset timeout\n", mal->index);
}

int mal_get_regs_len(struct mal_instance *mal)
{
	return sizeof(struct emac_ethtool_regs_subhdr) +
	    sizeof(struct mal_regs);
}

void *mal_dump_regs(struct mal_instance *mal, void *buf)
{
	struct emac_ethtool_regs_subhdr *hdr = buf;
	struct mal_regs *regs = (struct mal_regs *)(hdr + 1);
	int i;

	hdr->version = mal->version;
	hdr->index = mal->index;

	regs->tx_count = mal->num_tx_chans;
	regs->rx_count = mal->num_rx_chans;

	regs->cfg = get_mal_dcrn(mal, MAL_CFG);
	regs->esr = get_mal_dcrn(mal, MAL_ESR);
	regs->ier = get_mal_dcrn(mal, MAL_IER);
	regs->tx_casr = get_mal_dcrn(mal, MAL_TXCASR);
	regs->tx_carr = get_mal_dcrn(mal, MAL_TXCARR);
	regs->tx_eobisr = get_mal_dcrn(mal, MAL_TXEOBISR);
	regs->tx_deir = get_mal_dcrn(mal, MAL_TXDEIR);
	regs->rx_casr = get_mal_dcrn(mal, MAL_RXCASR);
	regs->rx_carr = get_mal_dcrn(mal, MAL_RXCARR);
	regs->rx_eobisr = get_mal_dcrn(mal, MAL_RXEOBISR);
	regs->rx_deir = get_mal_dcrn(mal, MAL_RXDEIR);

	for (i = 0; i < regs->tx_count; ++i)
		regs->tx_ctpr[i] = get_mal_dcrn(mal, MAL_TXCTPR(i));

	for (i = 0; i < regs->rx_count; ++i) {
		regs->rx_ctpr[i] = get_mal_dcrn(mal, MAL_RXCTPR(i));
		regs->rcbs[i] = get_mal_dcrn(mal, MAL_RCBS(i));
	}
	return regs + 1;
}


static int __devinit mal_probe(struct of_device *ofdev,
			       const struct of_device_id *match)
{
	struct mal_instance *mal;
	int err = 0, i, bd_size;
	int index = mal_count++;
	unsigned int dcr_base;
	const u32 *prop;
	const char *str_prop;
	u32 cfg;
	unsigned long irqflags;
	irq_handler_t hdlr_serr, hdlr_txde, hdlr_rxde;
#ifdef CONFIG_IBM_NEW_EMAC_INTR_COALESCE
	int num_phys_chans;
	int coal_intr_index;
#endif

	mal = kzalloc(sizeof(struct mal_instance), GFP_KERNEL);
	if (!mal) {
		printk(KERN_ERR
		       "mal%d: out of memory allocating MAL structure!\n",
		       index);
		return -ENOMEM;
	}
	mal->index = index;
	mal->ofdev = ofdev;
	mal->version = of_device_is_compatible(ofdev->node, "ibm,mcmal2") ? 2 : 1;

	MAL_DBG(mal, "probe" NL);

	str_prop = of_get_property(ofdev->node, "descriptor-memory", NULL);
	if (str_prop && (!strcmp(str_prop,"ocm") || !strcmp(str_prop,"OCM"))) {
		printk(KERN_INFO
			"mal%d: descriptor-memory = %s\n", index, str_prop);
		mal->desc_memory = MAL_DESC_MEM_OCM;
	}

	prop = of_get_property(ofdev->node, "num-tx-chans", NULL);
	if (prop == NULL) {
		printk(KERN_ERR
		       "mal%d: can't find MAL num-tx-chans property!\n",
		       index);
		err = -ENODEV;
		goto fail;
	}
	mal->num_tx_chans = prop[0];

	prop = of_get_property(ofdev->node, "num-rx-chans", NULL);
	if (prop == NULL) {
		printk(KERN_ERR
		       "mal%d: can't find MAL num-rx-chans property!\n",
		       index);
		err = -ENODEV;
		goto fail;
	}
	mal->num_rx_chans = prop[0];

	dcr_base = dcr_resource_start(ofdev->node, 0);
	if (dcr_base == 0) {
		printk(KERN_ERR
		       "mal%d: can't find DCR resource!\n", index);
		err = -ENODEV;
		goto fail;
	}
	mal->dcr_host = dcr_map(ofdev->node, dcr_base, 0x100);
	if (!DCR_MAP_OK(mal->dcr_host)) {
		printk(KERN_ERR
		       "mal%d: failed to map DCRs !\n", index);
		err = -ENODEV;
		goto fail;
	}

	if (of_device_is_compatible(ofdev->node, "ibm,mcmal-405ez")) {
#if defined(CONFIG_IBM_NEW_EMAC_MAL_CLR_ICINTSTAT) && \
		defined(CONFIG_IBM_NEW_EMAC_MAL_COMMON_ERR)
		mal->features |= (MAL_FTR_CLEAR_ICINTSTAT |
				MAL_FTR_COMMON_ERR_INT);
#else
		printk(KERN_ERR "%s: Support for 405EZ not enabled!\n",
				ofdev->node->full_name);
		err = -ENODEV;
		goto fail;
#endif
	}

	mal->txeob_irq = irq_of_parse_and_map(ofdev->node, 0);
	mal->rxeob_irq = irq_of_parse_and_map(ofdev->node, 1);
	mal->serr_irq = irq_of_parse_and_map(ofdev->node, 2);

	if (mal_has_feature(mal, MAL_FTR_COMMON_ERR_INT)) {
		mal->txde_irq = mal->rxde_irq = mal->serr_irq;
	} else {
		mal->txde_irq = irq_of_parse_and_map(ofdev->node, 3);
		mal->rxde_irq = irq_of_parse_and_map(ofdev->node, 4);
	}

	if (mal->txeob_irq == NO_IRQ || mal->rxeob_irq == NO_IRQ ||
	    mal->serr_irq == NO_IRQ || mal->txde_irq == NO_IRQ ||
	    mal->rxde_irq == NO_IRQ) {
		printk(KERN_ERR
		       "mal%d: failed to map interrupts !\n", index);
		err = -ENODEV;
		goto fail_unmap;
	}

#ifdef CONFIG_IBM_NEW_EMAC_INTR_COALESCE
	/* Number of Tx channels is equal to Physical channels */
	/* Rx channels include Virtual channels so use Tx channels */
	BUG_ON(mal->num_tx_chans > MAL_MAX_PHYS_CHANNELS);
	num_phys_chans = mal->num_tx_chans;
	/* Older revs in 460EX and 460GT have coalesce bug in h/w */
#if defined(CONFIG_460EX) || defined(CONFIG_460GT)
	{
		unsigned int pvr;
		unsigned short min;
		pvr = mfspr(SPRN_PVR);
		min = PVR_MIN(pvr);
		if (min < 4) {
			printk(KERN_INFO "PVR %x Intr Coal disabled: H/W bug\n",
					pvr);
			mal->coalesce_disabled = 1;
		}
	}
#else
	mal->coalesce_disabled = 0;
#endif
	coal_intr_index = 5;

	/* If device tree doesn't Interrupt coal IRQ, fall back to EOB IRQ */
	for (i = 0; (i < num_phys_chans) && (mal->coalesce_disabled == 0) ; i++) {
		mal->txcoal_irq[i] = irq_of_parse_and_map(ofdev->node, coal_intr_index++);
		if (mal->txcoal_irq[i] == NO_IRQ) {
			printk(KERN_INFO "MAL: No device tree IRQ for TxCoal%d  - disabling coalescing\n", i);
			mal->coalesce_disabled = 1;
		}
	}
	for (i = 0; (i < num_phys_chans) && (mal->coalesce_disabled == 0); i++) {
		mal->rxcoal_irq[i] = irq_of_parse_and_map(ofdev->node, coal_intr_index++);
		if (mal->rxcoal_irq[i] == NO_IRQ) {
			printk(KERN_INFO "MAL: No device tree IRQ for RxCoal%d  - disabling coalescing\n", i);
			mal->coalesce_disabled = 1;
		}
	}
#endif

	INIT_LIST_HEAD(&mal->poll_list);
	INIT_LIST_HEAD(&mal->list);
	spin_lock_init(&mal->lock);

	init_dummy_netdev(&mal->dummy_dev);

	netif_napi_add(&mal->dummy_dev, &mal->napi, mal_poll,
		       CONFIG_IBM_NEW_EMAC_POLL_WEIGHT);

	/* Load power-on reset defaults */
	mal_reset(mal);

	/* Set the MAL configuration register */
	cfg = (mal->version == 2) ? MAL2_CFG_DEFAULT : MAL1_CFG_DEFAULT;
	cfg |= MAL_CFG_PLBB | MAL_CFG_OPBBL | MAL_CFG_LEA;

	/* Current Axon is not happy with priority being non-0, it can
	 * deadlock, fix it up here
	 */
	if (of_device_is_compatible(ofdev->node, "ibm,mcmal-axon"))
		cfg &= ~(MAL2_CFG_RPP_10 | MAL2_CFG_WPP_10);

	/* Apply configuration */
	set_mal_dcrn(mal, MAL_CFG, cfg);

	/* Allocate space for BD rings */
	BUG_ON(mal->num_tx_chans <= 0 || mal->num_tx_chans > 32);
	BUG_ON(mal->num_rx_chans <= 0 || mal->num_rx_chans > 32);

	bd_size = sizeof(struct mal_descriptor) *
		(NUM_TX_BUFF * mal->num_tx_chans +
		 NUM_RX_BUFF * mal->num_rx_chans);

	if (mal->desc_memory == MAL_DESC_MEM_OCM) {
		mal->bd_virt = ocm_alloc(&mal->bd_phys, bd_size, 4,
					OCM_NON_CACHED, "mal_descriptors");
		mal->bd_dma  = (u32)mal->bd_phys;
	}

	if (mal->bd_virt == NULL) {
		/* Allocate BD on SDRAM in case !MAL_DESC_MEM_OCM or failed OCM alloc */
		if (mal->desc_memory == MAL_DESC_MEM_OCM){
			printk(KERN_INFO
				"mal%d: failed OCM alloc, descriptor-memory = SDRAM\n", index);
			mal->desc_memory = MAL_DESC_MEM_SDRAM;
		}
		mal->bd_virt = dma_alloc_coherent(&ofdev->dev, bd_size,
						&mal->bd_dma, GFP_KERNEL);
	}


	if (mal->bd_virt == NULL) {
		printk(KERN_ERR
		       "mal%d: out of memory allocating RX/TX descriptors!\n",
		       index);
		err = -ENOMEM;
		goto fail_unmap;
	}

	memset(mal->bd_virt, 0, bd_size);
	for (i = 0; i < mal->num_tx_chans; ++i) {
		if (mal->desc_memory == MAL_DESC_MEM_OCM)
			set_mal_dcrn(mal, MAL_TXBADDR, (mal->bd_phys >> 32));

		set_mal_dcrn(mal, MAL_TXCTPR(i), mal->bd_dma +
			     sizeof(struct mal_descriptor) *
			     mal_tx_bd_offset(mal, i));
	}

	for (i = 0; i < mal->num_rx_chans; ++i) {
		if (mal->desc_memory == MAL_DESC_MEM_OCM)
			set_mal_dcrn(mal, MAL_RXBADDR, (u32)(mal->bd_phys >> 32));

		set_mal_dcrn(mal, MAL_RXCTPR(i), mal->bd_dma +
			     sizeof(struct mal_descriptor) *
			     mal_rx_bd_offset(mal, i));
	}

	if (mal_has_feature(mal, MAL_FTR_COMMON_ERR_INT)) {
		irqflags = IRQF_SHARED;
		hdlr_serr = hdlr_txde = hdlr_rxde = mal_int;
	} else {
		irqflags = 0;
		hdlr_serr = mal_serr;
		hdlr_txde = mal_txde;
		hdlr_rxde = mal_rxde;
	}

	err = request_irq(mal->serr_irq, hdlr_serr, irqflags, "MAL SERR", mal);
	if (err) {
		mal->serr_irq = NO_IRQ;
		goto failirq;
	}
	err = request_irq(mal->txde_irq, hdlr_txde, irqflags, "MAL TX DE", mal);
	if (err) {
		mal->txde_irq = NO_IRQ;
		goto failirq;
	}
	err = request_irq(mal->rxde_irq, hdlr_rxde, irqflags, "MAL RX DE", mal);
	if (err) {
		mal->rxde_irq = NO_IRQ;
		goto failirq;
	}
#ifdef CONFIG_IBM_NEW_EMAC_INTR_COALESCE
	for (i = 0; (i < num_phys_chans) && (mal->coalesce_disabled == 0); i++) {
		err = request_irq(mal->txcoal_irq[i],
					mal_coal, 0, tx_coal_irqname[i], mal);
		if (err) {
			printk(KERN_INFO "MAL: TxCoal%d ReqIRQ failed - disabling coalescing\n", i);
			mal->txcoal_irq[i] = NO_IRQ;
			mal->coalesce_disabled = 1;
			break;
		}
	}
	for (i = 0; (i < num_phys_chans) && (mal->coalesce_disabled == 0); i++) {
		err = request_irq(mal->rxcoal_irq[i],
					mal_coal, 0, rx_coal_irqname[i], mal);
		if (err) {
			printk(KERN_INFO "MAL: RxCoal%d ReqIRQ failed - disabling coalescing\n", i);
			mal->rxcoal_irq[i] = NO_IRQ;
			mal->coalesce_disabled = 1;
			break;
		}
	}

	/* Fall back to EOB IRQ if coalesce not supported */
	if (mal->coalesce_disabled) {
		/* Clean up any IRQs allocated for Coalescing */
		for (i = 0; i < num_phys_chans; i++) {
			if (mal->txcoal_irq[i] != NO_IRQ)
				free_irq(mal->txcoal_irq[i], mal);
			if (mal->rxcoal_irq[i] != NO_IRQ)
				free_irq(mal->rxcoal_irq[i], mal);
		}
#endif
		err = request_irq(mal->txeob_irq, mal_txeob, 0, "MAL TX EOB", mal);
		if (err) {
			mal->txeob_irq = NO_IRQ;
			goto failirq;
		}
	err = request_irq(mal->rxeob_irq, mal_rxeob, 0, "MAL RX EOB", mal);
		if (err) {
			mal->rxeob_irq = NO_IRQ;
			goto failirq;
		}
#ifdef CONFIG_IBM_NEW_EMAC_INTR_COALESCE
	}
#endif

	/* Enable all MAL SERR interrupt sources */
	if (mal->version == 2)
		set_mal_dcrn(mal, MAL_IER, MAL2_IER_EVENTS);
	else
		set_mal_dcrn(mal, MAL_IER, MAL1_IER_EVENTS);

#ifdef CONFIG_IBM_NEW_EMAC_INTR_COALESCE
	if (mal->coalesce_disabled == 0) {
		mal->coales_param[0].tx_count = (CONFIG_IBM_NEW_EMAC_TX_COAL_COUNT & COAL_FRAME_MASK);
		mal->coales_param[1].tx_count = (CONFIG_IBM_NEW_EMAC_TX_COAL_COUNT & COAL_FRAME_MASK);
		mal->coales_param[2].tx_count = (CONFIG_IBM_NEW_EMAC_TX_COAL_COUNT & COAL_FRAME_MASK);
		mal->coales_param[3].tx_count = (CONFIG_IBM_NEW_EMAC_TX_COAL_COUNT & COAL_FRAME_MASK);

		mal->coales_param[0].rx_count = (CONFIG_IBM_NEW_EMAC_RX_COAL_COUNT & COAL_FRAME_MASK);
		mal->coales_param[1].rx_count = (CONFIG_IBM_NEW_EMAC_RX_COAL_COUNT & COAL_FRAME_MASK);
		mal->coales_param[2].rx_count = (CONFIG_IBM_NEW_EMAC_RX_COAL_COUNT & COAL_FRAME_MASK);
		mal->coales_param[3].rx_count = (CONFIG_IBM_NEW_EMAC_RX_COAL_COUNT & COAL_FRAME_MASK);

		mal->coales_param[0].tx_time = CONFIG_IBM_NEW_EMAC_TX_COAL_TIMER;
		mal->coales_param[1].tx_time = CONFIG_IBM_NEW_EMAC_TX_COAL_TIMER;
		mal->coales_param[2].tx_time = CONFIG_IBM_NEW_EMAC_TX_COAL_TIMER;
		mal->coales_param[3].tx_time = CONFIG_IBM_NEW_EMAC_TX_COAL_TIMER;

		mal->coales_param[0].rx_time = CONFIG_IBM_NEW_EMAC_RX_COAL_TIMER;
		mal->coales_param[1].rx_time = CONFIG_IBM_NEW_EMAC_RX_COAL_TIMER;
		mal->coales_param[2].rx_time = CONFIG_IBM_NEW_EMAC_RX_COAL_TIMER;
		mal->coales_param[3].rx_time = CONFIG_IBM_NEW_EMAC_RX_COAL_TIMER;

		mal_enable_coal(mal);
}
#endif
	/* Enable EOB interrupt */
	mal_enable_eob_irq(mal);

	printk(KERN_INFO
	       "MAL v%d %s, %d TX channels, %d RX channels\n",
	       mal->version, ofdev->node->full_name,
	       mal->num_tx_chans, mal->num_rx_chans);

	/* Advertise this instance to the rest of the world */
	wmb();
	dev_set_drvdata(&ofdev->dev, mal);

	mal_dbg_register(mal);

	return 0;

 failirq:
	if (mal->serr_irq != NO_IRQ)
		free_irq(mal->serr_irq, mal);
	if (mal->txde_irq != NO_IRQ)
		free_irq(mal->txde_irq, mal);
	if (mal->rxde_irq != NO_IRQ)
	free_irq(mal->rxde_irq, mal);
#ifdef CONFIG_IBM_NEW_EMAC_INTR_COALESCE
	if (mal->coalesce_disabled == 0) {
		for (i = 0; i < num_phys_chans; i++) {
			if (mal->txcoal_irq[i] != NO_IRQ)
				free_irq(mal->txcoal_irq[i], mal);
			if (mal->rxcoal_irq[i] != NO_IRQ)
				free_irq(mal->rxcoal_irq[i], mal);
		}
	} else {
#endif
		if (mal->txeob_irq != NO_IRQ)
	free_irq(mal->txeob_irq, mal);
		if (mal->rxeob_irq != NO_IRQ)
			free_irq(mal->rxeob_irq, mal);
#ifdef CONFIG_IBM_NEW_EMAC_INTR_COALESCE
	}
#endif
	if (mal->desc_memory == MAL_DESC_MEM_OCM)
		ocm_free(mal->bd_virt);
	else
		dma_free_coherent(&ofdev->dev, bd_size,
				mal->bd_virt, mal->bd_dma);
 fail_unmap:
	dcr_unmap(mal->dcr_host, 0x100);
 fail:
	kfree(mal);

	return err;
}

static int __devexit mal_remove(struct of_device *ofdev)
{
	struct mal_instance *mal = dev_get_drvdata(&ofdev->dev);
#ifdef CONFIG_IBM_NEW_EMAC_INTR_COALESCE
	int	i;
	int	num_phys_chans;
#endif

	MAL_DBG(mal, "remove" NL);

	/* Synchronize with scheduled polling */
	napi_disable(&mal->napi);

	if (!list_empty(&mal->list)) {
		/* This is *very* bad */
		printk(KERN_EMERG
		       "mal%d: commac list is not empty on remove!\n",
		       mal->index);
		WARN_ON(1);
	}

	dev_set_drvdata(&ofdev->dev, NULL);

	if (mal->serr_irq != NO_IRQ)
	free_irq(mal->serr_irq, mal);
	if (mal->txde_irq != NO_IRQ)
	free_irq(mal->txde_irq, mal);
	if (mal->rxde_irq != NO_IRQ)
	free_irq(mal->rxde_irq, mal);
#ifdef CONFIG_IBM_NEW_EMAC_INTR_COALESCE
	num_phys_chans = mal->num_tx_chans;
	if (mal->coalesce_disabled == 0) {
		for (i = 0; i < num_phys_chans; i++) {
			if (mal->txcoal_irq[i] != NO_IRQ)
				free_irq(mal->txcoal_irq[i], mal);
			if (mal->rxcoal_irq[i] != NO_IRQ)
				free_irq(mal->rxcoal_irq[i], mal);
		}
	} else {
#endif
		if (mal->txeob_irq != NO_IRQ)
			free_irq(mal->txeob_irq, mal);
		if (mal->rxeob_irq != NO_IRQ)
	free_irq(mal->rxeob_irq, mal);
#ifdef CONFIG_IBM_NEW_EMAC_INTR_COALESCE
	}
#endif
	mal_reset(mal);

	mal_dbg_unregister(mal);

	if (mal->desc_memory == MAL_DESC_MEM_OCM)
		ocm_free(mal->bd_virt);
	else
	dma_free_coherent(&ofdev->dev,
			  sizeof(struct mal_descriptor) *
			  (NUM_TX_BUFF * mal->num_tx_chans +
			   NUM_RX_BUFF * mal->num_rx_chans), mal->bd_virt,
			  mal->bd_dma);
	kfree(mal);

	return 0;
}

static struct of_device_id mal_platform_match[] =
{
	{
		.compatible	= "ibm,mcmal",
	},
	{
		.compatible	= "ibm,mcmal2",
	},
	/* Backward compat */
	{
		.type		= "mcmal-dma",
		.compatible	= "ibm,mcmal",
	},
	{
		.type		= "mcmal-dma",
		.compatible	= "ibm,mcmal2",
	},
	{},
};

static struct of_platform_driver mal_of_driver = {
	.name = "mcmal",
	.match_table = mal_platform_match,

	.probe = mal_probe,
	.remove = mal_remove,
};

int __init mal_init(void)
{
	return of_register_platform_driver(&mal_of_driver);
}

void mal_exit(void)
{
	of_unregister_platform_driver(&mal_of_driver);
}
