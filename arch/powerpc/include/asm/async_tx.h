/*
 * Copyright(c) 2008 DENX Engineering. All rights reserved.
 *
 * Author: Yuri Tikhonov <yur@emcraft.com>
 *
 * This program is free software; you can redistribute it and/or modify it
 * under the terms of the GNU General Public License as published by the Free
 * Software Foundation; either version 2 of the License, or (at your option)
 * any later version.
 *
 * This program is distributed in the hope that it will be useful, but WITHOUT
 * ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or
 * FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Public License for
 * more details.
 *
 * You should have received a copy of the GNU General Public License along with
 * this program; if not, write to the Free Software Foundation, Inc., 59
 * Temple Place - Suite 330, Boston, MA  02111-1307, USA.
 *
 * The full GNU General Public License is included in this distribution in the
 * file called COPYING.
 */
#ifndef _PPC_ASYNC_TX_H_
#define _PPC_ASYNC_TX_H_

#if defined(CONFIG_440SPe) || defined(CONFIG_440SP) 
extern int ppc440spe_adma_estimate (struct dma_chan *chan,
	enum dma_transaction_type cap, struct page **src_lst,
	int src_cnt, size_t src_sz);
#define ppc_adma_estimate(chan, cap, src_lst, src_cnt, src_sz) \
	ppc440spe_adma_estimate(chan, cap, src_lst, src_cnt, src_sz)
#elif  defined(CONFIG_460EX) || defined(CONFIG_460GT)
extern int ppc460ex_adma_estimate (struct dma_chan *chan,
	enum dma_transaction_type cap, struct page **src_lst,
	int src_cnt, size_t src_sz);
#define ppc_adma_estimate(chan, cap, src_lst, src_cnt, src_sz) \
	ppc460ex_adma_estimate(chan, cap, src_lst, src_cnt, src_sz)
#elif defined(CONFIG_APM82181)
extern int apm82181_adma_estimate (struct dma_chan *chan,
        enum dma_transaction_type cap, struct page **src_lst,
        int src_cnt, size_t src_sz);
#define ppc_adma_estimate(chan, cap, src_lst, src_cnt, src_sz) \
        apm82181_adma_estimate(chan, cap, src_lst, src_cnt, src_sz)
#endif

struct ppc_dma_chan_ref {
	struct dma_chan *chan;
	struct list_head node;
};

extern struct list_head ppc_adma_chan_list;

/**
 * ppc_async_tx_find_best_channel - find a channel with the maximum rank for the
 *	transaction type given (the rank of the operation is the value
 *	returned by the device_estimate method).
 * @cap: transaction type
 * @src_lst: array of pointers to sources for the transaction
 * @src_cnt: number of arguments (size of the srcs array)
 * @src_sz: length of the each argument pointed by srcs
 */
static inline struct dma_chan *
ppc_async_tx_find_best_channel (enum dma_transaction_type cap,
	struct page **src_lst, int src_cnt, size_t src_sz)
{
	struct dma_chan *best_chan = NULL;
	struct ppc_dma_chan_ref *ref;
	int best_rank = -1;

	list_for_each_entry(ref, &ppc_adma_chan_list, node)
		if (dma_has_cap(cap, ref->chan->device->cap_mask)) {
			int rank;

			rank = ppc_adma_estimate (ref->chan,
				cap, src_lst, src_cnt, src_sz);
			if (rank > best_rank) {
				best_rank = rank;
				best_chan = ref->chan;
			}
		}

	return best_chan;
}

#define async_tx_find_channel(dep, type, dst, dst_count, src, src_count, len) \
	ppc_async_tx_find_best_channel(type, src, src_count, len)

#endif
