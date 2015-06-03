/*
 *	Copyright(c) 2007 Yuri Tikhonov <yur@emcraft.com>
 *
 *	Developed for DENX Software Engineering GmbH
 *
 *	Asynchronous RAID-6 recovery calculations ASYNC_TX API.
 *
 *	based on async_xor.c code written by:
 *		Dan Williams <dan.j.williams@intel.com>
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
 * this program; if not, write to the Free Software Foundation, Inc., 51
 * Franklin St - Fifth Floor, Boston, MA 02110-1301 USA.
 *
 */
#include <linux/kernel.h>
#include <linux/interrupt.h>
#include <linux/dma-mapping.h>
#include <linux/raid/xor.h>
#include <linux/async_tx.h>

#include "../drivers/md/raid6.h"

/**
 * async_r6_dd_recov - attempt to calculate two data misses using dma engines.
 * @disks: number of disks in the RAID-6 array
 * @bytes: size of strip
 * @faila: first failed drive index
 * @failb: second failed drive index
 * @ptrs: array of pointers to strips (last two must be p and q, respectively)
 * @flags: ASYNC_TX_ACK, ASYNC_TX_DEP_ACK
 * @depend_tx: depends on the result of this transaction.
 * @cb: function to call when the operation completes
 * @cb_param: parameter to pass to the callback routine
 */
struct dma_async_tx_descriptor *
async_r6_dd_recov(int disks, size_t bytes, int faila, int failb,
	struct page **ptrs, enum async_tx_flags flags,
	struct dma_async_tx_descriptor *depend_tx,
	dma_async_tx_callback cb, void *cb_param)
{
	struct dma_async_tx_descriptor *tx = NULL;
	struct page *lptrs[disks];
	unsigned char lcoef[disks-4];
	int i = 0, k = 0, fc = -1;
	uint8_t bc[2];
	dma_async_tx_callback lcb = NULL;
	void *lcb_param = NULL;

	/* Assume that failb > faila */
	if (faila > failb) {
		fc = faila;
		faila = failb;
		failb = fc;
	}

	/* Try to compute missed data asynchronously. */
	if (disks == 4) {
		/*
		 * Pxy and Qxy are zero in this case so we already have
		 * P+Pxy and Q+Qxy in P and Q strips respectively.
		 */
		tx = depend_tx;
		lcb = cb;
		lcb_param = cb_param;
		goto do_mult;
	}

	/*
	 * (1) Calculate Qxy and Pxy:
	 * Qxy = A(0)*D(0) + ... + A(n-1)*D(n-1) + A(n+1)*D(n+1) + ... +
	 *	 A(m-1)*D(m-1) + A(m+1)*D(m+1) + ... + A(disks-1)*D(disks-1),
	 * where n = faila, m = failb.
	 */
	for (i = 0, k = 0; i < disks - 2; i++) {
		if (i != faila && i != failb) {
			lptrs[k] = ptrs[i];
			lcoef[k] = raid6_gfexp[i];
			k++;
		}
	}

	lptrs[k] = ptrs[faila];
	lptrs[k+1] = ptrs[failb];
	tx = async_pq(lptrs, lcoef, 0, k, bytes,
		      ASYNC_TX_PQ_ZERO_P | ASYNC_TX_PQ_ZERO_Q |
		      ASYNC_TX_ASYNC_ONLY, depend_tx, NULL, NULL);
	if (!tx) {
		/* Here may go to the synchronous variant */
		if (flags & ASYNC_TX_ASYNC_ONLY)
			return NULL;
		goto ddr_sync;
	}

	/*
	 * The following operations will 'damage' P/Q strips;
	 * so now we condemned to move in an asynchronous way.
	 */

	/* (2) Calculate Q+Qxy */
	lptrs[0] = ptrs[failb];
	lptrs[1] = ptrs[disks-1];
	lptrs[2] = NULL;
	tx = async_pq(lptrs, NULL, 0, 1, bytes, ASYNC_TX_DEP_ACK,
		      tx, NULL, NULL);

	/* (3) Calculate P+Pxy */
	lptrs[0] = ptrs[faila];
	lptrs[1] = ptrs[disks-2];
	lptrs[2] = NULL;
	tx = async_pq(lptrs, NULL, 0, 1, bytes, ASYNC_TX_DEP_ACK,
		      tx, NULL, NULL);

do_mult:
	/*
	 * (4) Compute (P+Pxy) * Bxy. Compute (Q+Qxy) * Cxy. XOR them and get
	 *  faila.
	 * B = (2^(y-x))*((2^(y-x) + {01})^(-1))
	 * C = (2^(-x))*((2^(y-x) + {01})^(-1))
	 * B * [p] + C * [q] -> [failb]
	 */
	bc[0] = raid6_gfexi[failb-faila];
	bc[1] = raid6_gfinv[raid6_gfexp[faila]^raid6_gfexp[failb]];

	lptrs[0] = ptrs[disks - 2];
	lptrs[1] = ptrs[disks - 1];
	lptrs[2] = NULL;
	lptrs[3] = ptrs[failb];
	tx = async_pq(lptrs, bc, 0, 2, bytes,
		      ASYNC_TX_PQ_ZERO_Q | ASYNC_TX_DEP_ACK,
		      tx, NULL, NULL);

	/* (5) Compute failed Dy using recovered [failb] and P+Pnm in [p] */
	lptrs[0] = ptrs[disks-2];
	lptrs[1] = ptrs[failb];
	lptrs[2] = ptrs[faila];
	lptrs[3] = NULL;
	tx = async_pq(lptrs, NULL, 0, 2, bytes,
		      ASYNC_TX_PQ_ZERO_P | ASYNC_TX_DEP_ACK,
		      tx, lcb, lcb_param);

	if (disks == 4)
		return tx;

	/* (6) Restore the parities back */
	flags |= ASYNC_TX_DEP_ACK;

	memcpy(lptrs, ptrs, (disks - 2) * sizeof(struct page *));
	lptrs[disks - 2] = ptrs[disks-2];
	lptrs[disks - 1] = ptrs[disks-1];
	return async_gen_syndrome(lptrs, 0, disks - 2, bytes, flags,
				  tx, cb, cb_param);

ddr_sync:
	{
		void **sptrs = (void **)lptrs;
		/*
		 * Failed to compute asynchronously, do it in
		 * synchronous manner
		 */

		/* wait for any prerequisite operations */
		async_tx_quiesce(&depend_tx);

		i = disks;
		while (i--)
			sptrs[i] = kmap(ptrs[i]);
		raid6_2data_recov(disks, bytes, faila, failb, sptrs);
		i = disks;
		while (i--)
			kunmap(ptrs[i]);

		async_tx_sync_epilog(cb, cb_param);
	}

	return tx;
}
EXPORT_SYMBOL_GPL(async_r6_dd_recov);

/**
 * async_r6_dp_recov - attempt to calculate one data miss using dma engines.
 * @disks: number of disks in the RAID-6 array
 * @bytes: size of strip
 * @faila: failed drive index
 * @ptrs: array of pointers to strips (last two must be p and q, respectively)
 * @flags: ASYNC_TX_ACK, ASYNC_TX_DEP_ACK
 * @depend_tx: depends on the result of this transaction.
 * @cb: function to call when the operation completes
 * @cb_param: parameter to pass to the callback routine
 */
struct dma_async_tx_descriptor *
async_r6_dp_recov(int disks, size_t bytes, int faila, struct page **ptrs,
	enum async_tx_flags flags, struct dma_async_tx_descriptor *depend_tx,
	dma_async_tx_callback cb, void *cb_param)
{
	struct dma_async_tx_descriptor *tx = NULL;
	struct page *lptrs[disks];
	unsigned char lcoef[disks-2];
	int i = 0, k = 0;

	/* Try compute missed data asynchronously. */

	/*
	 * (1) Calculate Qn + Q:
	 * Qn = A(0)*D(0) + .. + A(n-1)*D(n-1) + A(n+1)*D(n+1) + ..,
	 *  where n = faila;
	 * then subtract Qn from Q and place result to Pn.
	 */
	for (i = 0; i < disks - 2; i++) {
		if (i != faila) {
			lptrs[k] = ptrs[i];
			lcoef[k++] = raid6_gfexp[i];
		}
	}
	lptrs[k] = ptrs[disks-1]; /* Q-parity */
	lcoef[k++] = 1;

	lptrs[k] = NULL;
	lptrs[k+1] = ptrs[disks-2];

	tx = async_pq(lptrs, lcoef, 0, k, bytes,
		      ASYNC_TX_PQ_ZERO_Q | ASYNC_TX_ASYNC_ONLY,
		      depend_tx, NULL, NULL);
	if (!tx) {
		if (flags & ASYNC_TX_ASYNC_ONLY)
			return NULL;
		goto dpr_sync;
	}

	/*
	 * (2) Compute missed Dn:
	 * Dn = (Q + Qn) * [A(n)^(-1)]
	 */
	lptrs[0] = ptrs[disks-2];
	lptrs[1] = NULL;
	lptrs[2] = ptrs[faila];
	return async_pq(lptrs, (u8 *)&raid6_gfexp[faila ? 255-faila : 0], 0, 1,
			bytes, ASYNC_TX_DEP_ACK | ASYNC_TX_PQ_ZERO_Q,
			tx, cb, cb_param);

dpr_sync:
	{
		void **sptrs = (void **) lptrs;
		/*
		 * Failed to compute asynchronously, do it in
		 * synchronous manner
		 */

		/* wait for any prerequisite operations */
		async_tx_quiesce(&depend_tx);

		i = disks;
		while (i--)
			sptrs[i] = kmap(ptrs[i]);
		raid6_datap_recov(disks, bytes, faila, (void *)sptrs);
		i = disks;
		while (i--)
			kunmap(ptrs[i]);

		async_tx_sync_epilog(cb, cb_param);
	}

	return tx;
}
EXPORT_SYMBOL_GPL(async_r6_dp_recov);

static int __init async_r6recov_init(void)
{
	return 0;
}

static void __exit async_r6recov_exit(void)
{
	do { } while (0);
}

module_init(async_r6recov_init);
module_exit(async_r6recov_exit);

MODULE_AUTHOR("Yuri Tikhonov <yur@emcraft.com>");
MODULE_DESCRIPTION("asynchronous RAID-6 recovery api");
MODULE_LICENSE("GPL");
