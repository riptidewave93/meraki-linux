/**
 * APM PktDMA Test Driver
 *
 * Copyright (c) 2010 Applied Micro Circuits Corporation.
 * Author: Shasi Pulijala <spulijala@apm.com>
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License as
 * published by the Free Software Foundation; either version 2 of
 * the License, or (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place, Suite 330, Boston,
 * MA 02111-1307 USA
 *
 * PktDMA testing module.
 *
 */
#include <linux/kernel.h>
#include <linux/module.h>
#include <linux/proc_fs.h>
#include <asm/uaccess.h>
#include <asm/apm_pktdma.h>
#include <linux/kthread.h>

#include "apm_pktdma_access.h"
#include "apm_pktdma_core.h"
#include <asm/cacheflush.h>

#define DEBUG_DUMP_DATA

#ifndef HIDWORD
#define HIDWORD(v)		((u32) (((u64) (v)) >> 32))
#define LODWORD(v)		((u32) ((v) & 0xFFFFFFFF))
#define MAKE64(h,l)		(((u64) (h) << 32) | (l)) 
#endif

#define NUM_SLOTS	(2 * 1024)
#define PKT_SIZE	(64 * 1024)

#if !defined(DEBUG_DUMP_DATA)
#define DUMP_PKT(hdr, fmt, ...)
#else
#define DUMP_PKT(hdr, d, l)	\
	do { \
		print_hex_dump(KERN_INFO, hdr , \
			DUMP_PREFIX_ADDRESS, 16, 4,  d, l, 1); \
	} while(0);
#endif

int pktdma_stest_cb(struct  apm_pktdma_op_result *pktdma_result);

struct test_context {
	u32 magic;
	void *ptr;
	void *ptr1;
	void *ptr2;
	void *ptr3;
	
	void *saddr;
	void *daddr;
	u32 len;
	u8 free_buf;
	int xor_op;
};

int test_data = 0xBABEFACE;
struct dma_async_tx_descriptor test_depend_tx;

u32 *scatter_addr, *gather_addr[2], *multi_m2b_src;
int test_sg_size[] = /* {64, 64, 64, 64, 64, 512, 256, 64}; */
{(1024), (1024), (16 * 1024), (16 * 1024), (16 * 1024), (16 * 1024), (16 * 1024)};
int scatter_test_sg_size[] =
{ 256, 86, 64, 42, 128, 32, 512, 32, 32, 32, 256, 256, 256, 256, 256, 256};
int m2b_test_sg[] = {(1024), (1024), (1024), (1024), 512};

void test_cbf(void *data)
{
	printk(KERN_INFO "In async test callback function %X !!\n", *(int *) data);
}

static int dma_check_copy(u32 *sa, u32 *da, u32 byte_count)
{
	PKTDMA_DEBUG("Check: SA: %p, DA: %p", sa, da);
	PKTDMA_DEBUG("byte_count %d", byte_count);
	if (!memcmp(sa, da, byte_count)) {
		//dma_responses++;
		PKTDMA_ERR("Copy successful !!");
		return 0;
	} else {
		PKTDMA_ERR("Check: SA: %p, DA: %p", sa, da);
		printk(KERN_ERR "Copy failed !!\n");
		DUMP_PKT("sa", sa, byte_count);
		DUMP_PKT("da", da, byte_count);
		return -1;
	}
}

static void pktdma_fill_src(u32 *sa, char ch, u32 byte_count)
{
	memset(sa, ch, byte_count);
	return;
}

static int dma_check_copy_char(char *sa, char ch, u32 byte_count)
{
	int i;
	//PKTDMA_DEBUG("Check: SA: %p, ch: %c", sa, ch);
	//PKTDMA_DEBUG("byte_count %d", byte_count);
	for (i = 0; i < byte_count; i++) {
		if (*(sa++) != ch) {
			printk("Copy failed at 0x%p!!", sa);
			PKTDMA_DEBUG_DUMP("Dump failed from",
					  (void *)sa, 64);
			return -1;
		}
	}
	printk("Copy successful!!\n");
	return 0;
}

static void dma_fill_src_inc(u32 *sa, int init, u16 byte_count)
{
	int i;
	char *ptr = (char *) sa;
	for (i = 0; i < byte_count; i++) {
		*(ptr++) = init+i;
	}
	PKTDMA_DEBUG("Filled src %p with %d bytes !!", sa, byte_count);
	return;
}

/** For All M2M Operations */
static int pktdma_test_m2m_callback(struct  apm_pktdma_op_result *pktdma_result)
{
	struct test_context *ctx;
	int error;
	PKTDMA_DEBUG("In %s !!",__FUNCTION__);
	PKTDMA_DEBUG("Err: %d,", pktdma_result->err);

	ctx = (struct test_context *) pktdma_result->ctx;
#ifdef CONFIG_NOT_COHERENT_CACHE
	apm_pktdma_invalidate_dest(ctx->daddr, ctx->len);
#endif
	PKTDMA_DEBUG("printing src address = %p",
		     (u32 *) (ctx->saddr));
	PKTDMA_DEBUG("printing dest address = %p",
		     (u32 *) (ctx->daddr));
	
	dma_check_copy((u32 *) ctx->saddr, (u32 *) ctx->daddr, ctx->len);

	PKTDMA_DEBUG("ctx->len = %d\n", ctx->len);
	PKTDMA_DEBUG("Received context: %p", ctx);
	if (ctx->magic == 0xCAFEBABE) {
		if (ctx->ptr)
			kfree(ctx->ptr);
		if (ctx->saddr)
			kfree(ctx->saddr);
		if (ctx->daddr)
			kfree(ctx->daddr);
		kfree(ctx);
		PKTDMA_DEBUG("Freed all memory 4.");
	} else {
		PKTDMA_DEBUG("Bad context in m2m gather result !!");
		error = 1;
		return -1;
	}

	return 0;
	
}

/** For Packet to Memory - supports Gather and Direct */
static int pktdma_test_p2m_callback(struct apm_pktdma_op_result *dma_result)
{
	u32 rc = 0;
	struct test_context *ctx;
	PKTDMA_DEBUG("In %s !!",__FUNCTION__);
	PKTDMA_ERR("Result: %d",dma_result->err);

	ctx = (struct test_context *) dma_result->ctx;
#ifdef CONFIG_NOT_COHERENT_CACHE
	apm_pktdma_invalidate_dest(ctx->daddr, ctx->len);
#endif
	PKTDMA_DEBUG("Direct mode received context: %p", ctx);
	if (ctx->magic == 0xCAFEBABE) {
		dma_check_copy_char((void *)ctx->daddr, 'c', ctx->len);
		kfree((void *)ctx->daddr);
		if (ctx->ptr1)
			kfree(ctx->ptr1);
		if (ctx->ptr3)
			kfree(ctx->ptr3);
		if (ctx->ptr2)
			kfree(ctx->ptr2);

		kfree(ctx->ptr);
		kfree(ctx);
		PKTDMA_DEBUG( "Freed all memory 2.");
		return rc;
	} else {
		PKTDMA_DEBUG( "Bad context in m2m direct mode result !!");
		return -1;
	}
	return 0;
}

/** For Memory to buffer Gather and copy */
static int pktdma_test_m2b_callback(struct  apm_pktdma_op_result *iodma_result)
{
	struct test_context *ctx;
	int i, error;

	ctx = (struct test_context *) iodma_result->ctx;

	PKTDMA_DEBUG("In %s !!",__FUNCTION__);
	PKTDMA_DEBUG("Err: %d, Length: %d",
		    iodma_result->err, iodma_result->fp[0].fp_dlength);
	if (ctx->xor_op) {
		printk(KERN_INFO "XOR Result\n");
		DUMP_PKT("dst", iodma_result->fp[0].fp_dest_address, ctx->len);
		printk("\n");
		DUMP_PKT("src", ctx->saddr, ctx->len);
		if (ctx->saddr)
			kfree((void *)ctx->saddr);
		if (ctx->ptr)
			kfree((void *)ctx->ptr);
		if (ctx)
			kfree((void *)ctx);
		return 0;
	}
	if ( iodma_result->crc || iodma_result->checksum) {
		printk("Dumping Checksum 0x%08X or crc 0x%08X\n",
			    iodma_result->checksum, iodma_result->crc);
		dma_check_copy((void *)ctx->saddr,
				(void *)iodma_result->fp[0].fp_dest_address,
				ctx->len);
		goto err;
	}
	for (i = 0; i < (iodma_result->num_fp); i++) {
		if(!iodma_result->fp[i].fp_dest_address) {
			PKTDMA_ERR("Error Null Address returned");
			goto err;
		}

		dma_check_copy_char((char *)iodma_result->fp[i].fp_dest_address,
				     'y',
				     iodma_result->fp[i].fp_dlength);
	}
	for (i = 0; i < (iodma_result->num_fp); i++) {
		PKTDMA_DEBUG("freeing fp dest address = 0x%p\n",
			     iodma_result->fp[i].fp_dest_address);
		kfree((void *)iodma_result->fp[i].fp_dest_address);
	}
err:
	PKTDMA_DEBUG("Received context: %p", ctx);

	if (ctx->magic == 0xCAFEBABE) {
		if (ctx->saddr)
			kfree((void *)ctx->saddr);
		if (ctx->ptr)
			kfree((void *)ctx->ptr);
		if (ctx)
			kfree((void *)ctx);
		PKTDMA_DEBUG("Freed all memory 4.");
} else {
		PKTDMA_DEBUG("Bad context in m2b gather result !!");
		error = 1;
		return -1;
}

	return 0;
}
#define STRIDE_PKT_SIZE		500
static int dma_test_stride(struct apm_pktdma_op_result *dma_result)
{

	u32 rc = 0;
	struct test_context *ctx;
	PKTDMA_DEBUG("In %s !!\n",__FUNCTION__);
	PKTDMA_DEBUG("Result: %d\n",dma_result->err);

	ctx = (struct test_context *) dma_result->ctx;
	PKTDMA_DEBUG("Direct mode received context: %p\n", ctx);
	if (ctx->magic == 0xCAFEBABE) {
		DUMP_PKT("stride data", (void *)ctx->daddr, ctx->len);
		kfree(ctx->saddr);
		kfree(ctx->daddr);
		kfree(ctx->ptr);
		kfree(ctx);
		PKTDMA_DEBUG( "Freed all memory 2.\n");
		return rc;
	} else {
		PKTDMA_DEBUG( "Bad context in m2m direct mode result !!\n");
		return -1;
	}

	return 0;
}

int apm_pktdma_test_m2m_copy_stride(void)
{
	int rc = 0;
	u16 byte_count;
	u64 sa, da;
	u32 *src_data, *dest_data;
	struct test_context *ctx;
	struct apm_pktdma_m2m_params *dma_m2m;
	
	/* Test 2: m2m direct mode */
	dma_m2m = (struct apm_pktdma_m2m_params *)
			kzalloc(sizeof(struct apm_pktdma_m2m_params), GFP_KERNEL);
	if (!dma_m2m)
		return -1;
	ctx = (struct test_context *)
			kzalloc(sizeof(struct test_context), GFP_KERNEL);
	if (!ctx)
		return -1;
	PKTDMA_DRXTX("m2m direct mode context: %p", ctx);
	src_data = (u32 *) kzalloc(STRIDE_PKT_SIZE, GFP_KERNEL);
	if (!src_data)
		return -1;
	PKTDMA_DRXTX("src_data = %p", src_data);
	dest_data = (u32 *) kzalloc(STRIDE_PKT_SIZE, GFP_KERNEL);
	if (!dest_data)
		return -1;
	PKTDMA_DRXTX("dest_data = %p", dest_data);
	byte_count = STRIDE_PKT_SIZE;
	ctx->magic = 0xCAFEBABE;
	ctx->ptr = dma_m2m;
	ctx->saddr = (void *)src_data;
	ctx->daddr = (void *)dest_data;
	ctx->len = byte_count;
	dma_m2m->m2m_mode = IODMA_STRIDE;
	sa = __pa((u32)src_data);
	da = __pa((u32)dest_data);
	dma_m2m->sa = &sa;
	PKTDMA_DRXTX("src data = 0x%08X", (u32)src_data);
	dma_m2m->da = &da;
	dma_m2m->byte_count = &byte_count;

#if 1
	dma_m2m->str.dst_str_1_sz = 40;
	dma_m2m->str.src_str_1_sz = 40;
	
	dma_m2m->str.dst_str_sz = 60;
	dma_m2m->str.src_str_sz = 60;
	
	dma_m2m->str.dst_str_dis = 50;
	dma_m2m->str.src_str_dis = 50;
#else
	dma_m2m->str.dst_str_dis = 32;
	dma_m2m->str.dst_str_1_sz = 64;
	dma_m2m->str.dst_str_sz = 64;
	dma_m2m->str.src_str_dis = 32;
	dma_m2m->str.src_str_1_sz = 64;
	dma_m2m->str.src_str_sz = 64;
#endif
	pktdma_fill_src(src_data, 'a', byte_count);
	dma_m2m->cb = dma_test_stride;
	dma_m2m->context = ctx;
	PKTDMA_DEBUG("byte_count =  %d", byte_count);
	rc = apm_pktdma_m2m_xfer(dma_m2m);

	return rc;
}

/** Beggining of Test Code */
/** Memory to Buffer */

static int apm_pktdma_test_m2b_copy(int chanid)
{
	int rc = 0;
	struct test_context *ctx;
	struct apm_pktdma_m2b_params *iodma_m2b;
	u32 *src_data;
	u16 byte_count;
	u64 sa;

	/* Test 7: m2b direct mode with flyby */
	iodma_m2b = (struct apm_pktdma_m2b_params *) kzalloc(sizeof(struct
 			apm_pktdma_m2b_params), GFP_KERNEL);
	if (!iodma_m2b)
		return -ENOMEM;
	ctx = (struct test_context *) kzalloc(sizeof(struct test_context),
	GFP_KERNEL);
	if (!ctx)
		return -ENOMEM;
	src_data = (u32 *) kzalloc((1024), GFP_KERNEL);
	if (!src_data)
		return -ENOMEM;
	ctx->magic = 0xCAFEBABE;
	ctx->ptr = iodma_m2b;
	ctx->saddr = src_data;
	iodma_m2b->chid = chanid;
	
	iodma_m2b->m2b_mode = IODMA_COPY;
	//byte_count = (10 * 1024 + 512);
	byte_count = (1024);
	pktdma_fill_src(src_data, 'y', byte_count);
	sa = __pa(src_data);
	iodma_m2b->sa = (u64 *) &sa;
	iodma_m2b->byte_count = &byte_count;
	iodma_m2b->cb = pktdma_test_m2b_callback;
	iodma_m2b->context = ctx;
#ifdef CONFIG_NOT_COHERENT_CACHE
	apm_pktdma_flush_src(src_data, byte_count);
#endif
	rc = apm_pktdma_m2b_xfer(iodma_m2b);

	return rc;
}

#define GATHER_M2B_SIZE 4
#define GATHER_M2B_PKTSIZE	1024
/** Memory to BUffer Gather */
int apm_pktdma_test_m2b_gather(int chid)
{
	int rc = 0, i, j;
	struct test_context *ctx;
	struct apm_pktdma_m2b_params *iodma_m2b;
	u32 byte_count = 0;
	u16 byte_count_ptr[GATHER_M2B_SIZE];
	u64 sa_ptr1[GATHER_M2B_SIZE];

	/* Test 7: m2b direct mode with flyby */
	iodma_m2b = (struct apm_pktdma_m2b_params *) kzalloc(sizeof(struct
			apm_pktdma_m2b_params), GFP_KERNEL);
	if (!iodma_m2b)
		return -ENOMEM;
	ctx = (struct test_context *)
			kzalloc(sizeof(struct test_context), GFP_KERNEL);
	if (!ctx)
		return -ENOMEM;
	PKTDMA_DEBUG("m2b direct mode context: %p", ctx);

	byte_count = (GATHER_M2B_SIZE * GATHER_M2B_PKTSIZE);

	multi_m2b_src = (u32 *) kzalloc(byte_count, GFP_KERNEL);
	if (!multi_m2b_src)
		return -ENOMEM;
	ctx->magic = 0xCAFEBABE;
	ctx->ptr = iodma_m2b;
	ctx->saddr = multi_m2b_src;
	pktdma_fill_src(multi_m2b_src, 'y', byte_count);
	iodma_m2b->m2b_mode = IODMA_GATHER;
	iodma_m2b->sg_count = GATHER_M2B_SIZE;
	iodma_m2b->cb = pktdma_test_m2b_callback;
	
	PKTDMA_DEBUG("Setting the gather list for m2b");
	for (i = 0, j = 0; i < GATHER_M2B_SIZE; i++) {
		sa_ptr1[i] = __pa((u32)multi_m2b_src) + j;
		byte_count_ptr[i] = GATHER_M2B_PKTSIZE;
		j += GATHER_M2B_PKTSIZE;
	}
#ifdef CONFIG_NOT_COHERENT_CACHE
	apm_pktdma_flush_src(multi_m2b_src, byte_count);
#endif
	iodma_m2b->sa = sa_ptr1;
	iodma_m2b->byte_count = byte_count_ptr;
	iodma_m2b->context = ctx;
	iodma_m2b->chid = chid;
	rc = apm_pktdma_m2b_xfer(iodma_m2b);

	return 0;
}

#define GATHER_SIZE		2
/**  Memory to Memory Gather */
int apm_pktdma_test_m2m_copy_gather(int chanid)
{
	int rc = 0, i, j;
	u32 byte_count = 0;
	u64 da;
	u32 *dest_data;
	struct test_context *ctx;
	struct apm_pktdma_m2m_params *iodma_m2m;
	u16 byte_count_ptr[GATHER_SIZE];
	u64 sa_ptr1[GATHER_SIZE];

	/* Test 4: m2m pointer mode gather */
	iodma_m2m = (struct apm_pktdma_m2m_params *) kzalloc(sizeof(struct
			apm_pktdma_m2m_params), GFP_KERNEL);
	if (!iodma_m2m)
		return -ENOMEM;
	ctx = (struct test_context *)
			kzalloc(sizeof(struct test_context), GFP_KERNEL);
	if (!ctx)
		return -ENOMEM;
	PKTDMA_DEBUG("m2m gather context: %p", ctx);
	for (i = 0; i < GATHER_SIZE; i++) {
		byte_count += test_sg_size[i];
		PKTDMA_DEBUG("byte_count = %d", byte_count);
	}

	dest_data = (u32 *) kzalloc(byte_count, GFP_KERNEL);
	if (!dest_data)
		return -ENOMEM;
	gather_addr[0] = (u32 *) kzalloc(byte_count, GFP_KERNEL);
	if (!gather_addr[0])
		return -ENOMEM;
	PKTDMA_DEBUG("gather_addr = 0x%p\n", gather_addr[0]);
	ctx->magic = 0xCAFEBABE;
	ctx->ptr = iodma_m2m;
	ctx->saddr = (void *)gather_addr[0];
	ctx->daddr = dest_data;
	ctx->len = byte_count;
	pktdma_fill_src(gather_addr[0], 'y', byte_count);
	iodma_m2m->chid = chanid;
	iodma_m2m->m2m_mode = IODMA_GATHER;
	iodma_m2m->sg_count = GATHER_SIZE;
	da = __pa((u32)dest_data);

	PKTDMA_DEBUG("Setting the gather list for m2b");
	for (i = 0, j = 0; i < GATHER_SIZE; i++) {
		sa_ptr1[i] = __pa((u32)gather_addr[0]) + j;
		byte_count_ptr[i] = test_sg_size[i];
		j += test_sg_size[i];
	}
	iodma_m2m->sa = sa_ptr1;
	iodma_m2m->da = &da;
	iodma_m2m->byte_count = byte_count_ptr;
	iodma_m2m->cb = pktdma_test_m2m_callback;
	iodma_m2m->context = ctx;
	for ( i = 0; i < GATHER_SIZE ; i++)
		PKTDMA_DEBUG("src address =  0x%08X. len = %d",
			     (u32)sa_ptr1[i], byte_count_ptr[i]);
	PKTDMA_DEBUG("printing byte_count = %d", byte_count);
#ifdef CONFIG_NOT_COHERENT_CACHE
	apm_pktdma_flush_src(gather_addr[0], byte_count);
#endif	
	rc = apm_pktdma_m2m_xfer(iodma_m2m);
	
	return rc;
}

#define SCATTER_SIZE		2
#define SCATTER_DATA_SIZE	256

/** Memory to Memory Scatter */
int apm_pktdma_test_m2m_copy_scatter(int chanid)
{
	int rc = 0;
	u16 byte_count = 0;
	u64 sa;
	u32 *src_data;
	struct test_context *ctx;
	struct apm_pktdma_m2m_params *iodma_m2m;
	u64 da_ptr1[SCATTER_SIZE];
	u16 byte_count_ptr[SCATTER_SIZE];
	int i, j;

	/* Test 4: m2m pointer mode gather */
	iodma_m2m = (struct apm_pktdma_m2m_params *) kzalloc(sizeof(struct
			apm_pktdma_m2m_params), GFP_KERNEL);
	if (!iodma_m2m)
		return -ENOMEM;
	ctx = (struct test_context *)
			kzalloc(sizeof(struct test_context), GFP_KERNEL);
	if (!ctx)
		return -ENOMEM;
	PKTDMA_DEBUG("m2m scatter context: %p", ctx);
	for (i = 0; i < SCATTER_SIZE; i++) {
		byte_count += scatter_test_sg_size[i];
		PKTDMA_DEBUG("byte_count = %d", byte_count);
	}
	src_data = (u32 *) kzalloc(byte_count, GFP_KERNEL);
	if (!src_data)
		return -ENOMEM;

	scatter_addr = (u32 *) kzalloc((SCATTER_DATA_SIZE * SCATTER_SIZE) /* + 15 */,
	GFP_KERNEL);
	if (!scatter_addr)
		return -ENOMEM;
	ctx->magic = 0xCAFEBABE;
	ctx->ptr = iodma_m2m;
	ctx->saddr = src_data;
	ctx->ptr1 = da_ptr1;
	ctx->daddr = (void *)scatter_addr;
	ctx->ptr2 = byte_count_ptr;
	ctx->len = byte_count;
	PKTDMA_DEBUG("byte_count = %d", byte_count);
	pktdma_fill_src(src_data, 'x', byte_count);

	iodma_m2m->m2m_mode = IODMA_SCATTER;
	iodma_m2m->sg_count = SCATTER_SIZE;
	sa = __pa(src_data);

	PKTDMA_DEBUG("Setting the gather list for m2b");
	for (i = 0, j = 0; i < SCATTER_SIZE; i++) {
		da_ptr1[i] = __pa((u32)scatter_addr) + j;
		byte_count_ptr[i] = scatter_test_sg_size[i];
		j += scatter_test_sg_size[i];
	}
	iodma_m2m->sa = (u64 *) &sa;
	iodma_m2m->da = da_ptr1;
	iodma_m2m->byte_count = byte_count_ptr;
	iodma_m2m->cb = pktdma_test_m2m_callback;
	iodma_m2m->context = ctx;
	iodma_m2m->chid = chanid;
#ifdef CONFIG_NOT_COHERENT_CACHE
	apm_pktdma_flush_src(src_data, byte_count);
#endif	
	rc = apm_pktdma_m2m_xfer(iodma_m2m);
	
	return rc;
}

/** CRC and Checksum Tests */
static int apm_pktdma_test_m2b_gen_crc32(void)
{
	int rc = 0;
	struct test_context *ctx;
	struct apm_pktdma_m2b_params *iodma_m2b;
	u32 *src_data;
	u16 byte_count;
	u64 sa;

	/* Test 7: m2b direct mode with flyby */
	iodma_m2b = (struct apm_pktdma_m2b_params *) kzalloc(sizeof(struct
			apm_pktdma_m2b_params), GFP_KERNEL);
	if (!iodma_m2b)
		return -1;
	memset(iodma_m2b, 0, sizeof(struct
			apm_pktdma_m2b_params));
	ctx = (struct test_context *)
			kzalloc(sizeof(struct test_context), GFP_KERNEL);
	if (!ctx)
		return -1;
	
	PKTDMA_DEBUG("m2b direct mode context with crc: %p", ctx);
	src_data = (u32 *) kzalloc(32, GFP_KERNEL);
	if (!src_data)
		return -1;
	ctx->magic = 0xCAFEBABE;
	ctx->ptr = iodma_m2b;
	ctx->saddr = src_data;
	ctx->len = 32;

	iodma_m2b->m2b_mode = IODMA_COPY;
	byte_count = (32);
	dma_fill_src_inc(src_data, 1, byte_count);
	sa = __pa((u32)src_data);
	iodma_m2b->sa = (u64 *) &sa;
	iodma_m2b->byte_count = &byte_count;
	iodma_m2b->fb.fb_type = DMA_FBY_CRC32;
	iodma_m2b->fb.fb_byte_count = byte_count;
	iodma_m2b->fb.fb_seed_select = DMA_SEED_SELECT_DEFAULT;
	iodma_m2b->fb.fb_gen_check = DMA_CRC_CHKSUM_GEN;
	iodma_m2b->cb = pktdma_test_m2b_callback;
	iodma_m2b->context = ctx;
	iodma_m2b->fby = 1;

	rc = apm_pktdma_m2b_xfer(iodma_m2b);

	return rc;

}

int apm_pktdma_test_m2b_check_crc32(void)
{
	int rc = 0;
	struct test_context *ctx;
	struct apm_pktdma_m2b_params *iodma_m2b;
	//u32 *dest_data;
	u32 *src_data, src_crc_offset;
	u16 byte_count;
	u64 sa;
	u32 crc_value = 0;

	/* Test 7: m2b direct mode with flyby */
	iodma_m2b = (struct apm_pktdma_m2b_params *) kzalloc(sizeof(struct
			apm_pktdma_m2b_params), GFP_KERNEL);
	if (!iodma_m2b)
		return -ENOMEM;
	memset(iodma_m2b, 0, sizeof(struct
			apm_pktdma_m2b_params));
	ctx = (struct test_context *)
			kzalloc(sizeof(struct test_context),
				GFP_KERNEL);
	if (!ctx)
		return -ENOMEM;
	
	PKTDMA_DEBUG("m2b direct mode context: %p", ctx);
	src_data = (u32 *) kzalloc(32 + 4, GFP_KERNEL);
	if (!src_data)
		return -ENOMEM;
	ctx->magic = 0xCAFEBABE;
	ctx->ptr = iodma_m2b;
	ctx->saddr = (void *)src_data;

	iodma_m2b->m2b_mode = IODMA_COPY;
	dma_fill_src_inc(src_data, 1, byte_count);
	crc_value = 0x8E4ACB3F;
	src_crc_offset = (u32)src_data + byte_count;
	memcpy((void *)src_crc_offset, &crc_value, 4);
	sa = __pa((u32)src_data);
	iodma_m2b->sa = (u64 *) &sa;
	byte_count = 36;
	iodma_m2b->byte_count = &byte_count;
#if 1
	iodma_m2b->fb.fb_type = DMA_FBY_CRC32;
	iodma_m2b->fb.fb_byte_count = (byte_count - 4);
	iodma_m2b->fb.fb_seed_select = DMA_SEED_SELECT_DEFAULT;
	iodma_m2b->fb.fb_gen_check = DMA_CRC_CHKSUM_CHECK;
	iodma_m2b->cb = pktdma_test_m2b_callback;
	iodma_m2b->fby = 1;
#endif
	iodma_m2b->context = ctx;

	PKTDMA_DEBUG("Dumping in access.c:");

	rc = apm_pktdma_m2b_xfer(iodma_m2b);
	return rc;
}

/** Packet to Memory */
int apm_pktdma_test_p2m_copy(void)
{
	int rc = 0;
	struct test_context *ctx;
	struct apm_pktdma_p2m_params *iodma_p2m;
	u32 *dest_data;
	void* src_data;
	u64 da, sa;
	u16 byte_count;
	u8 fp_id;
	struct apm_pktdma_buf_info fp_ppc_buf;
	/* Test 6: p2m direct mode */
	iodma_p2m = (struct apm_pktdma_p2m_params *)
			kzalloc
			(sizeof(struct apm_pktdma_p2m_params),
			 GFP_KERNEL);
	if (!iodma_p2m)
		return -ENOMEM;
	ctx = (struct test_context *)
			kzalloc(sizeof(struct test_context),
				GFP_KERNEL);
	if (!ctx)
		return -ENOMEM;

	dest_data = (u32 *) kzalloc(64, GFP_KERNEL);
	if (!dest_data)
		return -ENOMEM;
	da = __pa((u32)dest_data);

	rc = apm_pktdma_get_fp(&fp_ppc_buf);
	if (rc) {
		PKTDMA_DEBUG("PPC Free pool alloc failed");
		return rc;
	}
	/* Convert to virtual address in Linux */
	src_data = fp_ppc_buf.addr;
	fp_id = fp_ppc_buf.fp_id;
	iodma_p2m->fp_id = &fp_id;
	
	byte_count = 64;
	sa = __pa(src_data);
	iodma_p2m->fp_sa = &sa;
	iodma_p2m->da = &da;
	ctx->len = byte_count;
	ctx->free_buf = 1;
	PKTDMA_DEBUG("msg buf len = 0x%08X",
			  (fp_ppc_buf.datalen & ~0xFFF));
	byte_count = ((fp_ppc_buf.datalen & ~0xFFF) | byte_count);
	iodma_p2m->byte_count = &byte_count;
	ctx->magic = 0xCAFEBABE;
	ctx->ptr = iodma_p2m;
	iodma_p2m->p2m_mode = IODMA_COPY;
	pktdma_fill_src(src_data, 'c', byte_count);
	iodma_p2m->cb = pktdma_test_p2m_callback;
	iodma_p2m->context = (void *)ctx;
	ctx->daddr = dest_data;
#ifdef CONFIG_NOT_COHERENT_CACHE
	apm_pktdma_flush_src(src_data, byte_count);
#endif
	rc = apm_pktdma_p2m_xfer(iodma_p2m);

	return rc;
}
#define GATHER_M2B_SIZE_XOR	2
#define SRC_XOR_PAGE_SZ		128
int apm_pktdma_test_m2b_xor(void)
{
	int rc = 0, j, k;
	struct test_context *ctx;
	struct apm_pktdma_m2b_params *iodma_m2b;
	u16 byte_count = 0;
	u16 len[GATHER_M2B_SIZE_XOR];
	u64 sa[GATHER_M2B_SIZE_XOR];
	u32 *src;

	iodma_m2b = (struct apm_pktdma_m2b_params *) kzalloc(sizeof(struct
			apm_pktdma_m2b_params), GFP_KERNEL);
	if (!iodma_m2b)
		return -ENOMEM;
	ctx = (struct test_context *) kzalloc(sizeof(struct test_context),
			GFP_KERNEL);
	if (!ctx) {
		kfree(iodma_m2b);
		return -ENOMEM;
	}
	src = kzalloc(SRC_XOR_PAGE_SZ * GATHER_M2B_SIZE_XOR, GFP_KERNEL);
	if (!src) {
		kfree(iodma_m2b);
		kfree(ctx);
		return -ENOMEM;
	}
	memset(src, 1, SRC_XOR_PAGE_SZ);
	memset((src + SRC_XOR_PAGE_SZ), 0, SRC_XOR_PAGE_SZ);

	for (j = 0, k = 0; j < GATHER_M2B_SIZE_XOR; j++) {
		sa[j] = __pa(src) + k;
		len[j] = SRC_XOR_PAGE_SZ;
		k += SRC_XOR_PAGE_SZ;
	}
	ctx->saddr = src;
	ctx->magic = 0xCAFEBABE;
	ctx->ptr = iodma_m2b;
	ctx->xor_op = 1;
	ctx->len = SRC_XOR_PAGE_SZ * GATHER_M2B_SIZE_XOR;

	iodma_m2b->m2b_mode = IODMA_GATHER;
	iodma_m2b->sg_count = GATHER_M2B_SIZE_XOR;
	iodma_m2b->sa = sa;
	iodma_m2b->byte_count = len;
	iodma_m2b->context = ctx;
	/* Setting XOR FBY Params */
	iodma_m2b->fb.fb_type = DMA_FBY_XOR_2SRC;
	iodma_m2b->fb.fb_byte_count = byte_count;
	iodma_m2b->fb.fb_seed_select = DMA_SEED_SELECT_DEFAULT;
	iodma_m2b->fb.fb_gen_check = DMA_CRC_CHKSUM_GEN;
	iodma_m2b->cb = pktdma_test_m2b_callback;
	iodma_m2b->context = ctx;
	iodma_m2b->fby = 1;
	
	rc = apm_pktdma_m2b_xfer(iodma_m2b);
	return rc;

}

static int apm_pktdma_test_m2m_direct_copy(int chid, int len)
{
	int rc = 0;
	u16 byte_count;
	u64 sa, da;
	u32 *src_data, *dest_data;
	struct test_context *ctx;
	struct apm_pktdma_m2m_params *dma_m2m;
	//struct dma_chan_info info;
	
	/* Test 2: m2m direct mode */
	dma_m2m = (struct apm_pktdma_m2m_params *)
			kzalloc(sizeof(struct apm_pktdma_m2m_params),
				GFP_KERNEL);
	if (!dma_m2m)
		return -ENOMEM;
	ctx = (struct test_context *)
			kzalloc(sizeof(struct test_context), GFP_KERNEL);
	if (!ctx)
		return -ENOMEM;
	PKTDMA_DEBUG("m2m direct mode context: %p", ctx);
	src_data = (u32 *) kzalloc(len, GFP_KERNEL);
	if (!src_data)
		return -ENOMEM;

	ctx->saddr = src_data;
	PKTDMA_DEBUG("src_data = %p", src_data);

	dest_data = (u32 *) kzalloc(len, GFP_KERNEL);
	if (!dest_data)
		return -ENOMEM;
	ctx->daddr = dest_data;
	PKTDMA_DEBUG("dest_data = %p", dest_data);
	byte_count = len;
	
	ctx->magic = 0xCAFEBABE;
	ctx->ptr = dma_m2m;
	ctx->len = byte_count;

	dma_m2m->m2m_mode = IODMA_COPY;
	sa = __pa(src_data);
	da = __pa(dest_data);
	dma_m2m->sa = &sa;
	dma_m2m->da = &da;
	dma_m2m->byte_count = &byte_count;

	dma_m2m->chid = /* info.chan_en[0] */ chid;

	pktdma_fill_src(src_data, 'a', byte_count);
	dma_m2m->cb = pktdma_test_m2m_callback;
	dma_m2m->context = ctx;
#ifdef CONFIG_NOT_COHERENT_CACHE
	apm_pktdma_flush_dest(src_data, byte_count);
	apm_pktdma_flush_src(dest_data, byte_count);
#endif	
	rc = apm_pktdma_m2m_xfer(dma_m2m);
	
	return rc;
}
/* PKTDMA Stress Test Code */
/* Enable GATHER_TEST to test sizes of 32K and above.
STEST_GATHER_SIZE = 2 (32K),
STEST_GATHER_SIZE = 4 (64K), Accordingly change the PKT SIZE
REV = 22 */

#define GATHER_TEST
#define STEST_GATHER_SIZE	4
struct op_item {
	u32 signature;
	int slot;
	int free;
	u8 *sa_ptr;
	u8 *da_ptr;	
	u8 *sa;
	u8 *da;
	u64 psa;
	u64 pda;		
	int chan_id;
	u16 byte_cnt;
	u32 ctr;
	u64 phy_sa[4];
	u16 len[4];
	u32 signature_end;
};

u32 op_inprogress;
u32 op_chk_mode;
u32 op_mode;

struct op_item op_slots[NUM_SLOTS];
u32 op_data_max;
u32 op_data_done;
u32 op_data_submitted;
u32 op_data_slot;
u64 op_start;
u64 op_end;
unsigned long j_end;
unsigned long j_start;
struct timeval value;
int test_m2m_submit(int chid, int slot);

void pktdma_free_buffer_ring2(void)
{
	int i;
	for (i = 0; i < NUM_SLOTS; i++) {
		kfree(op_slots[i].sa_ptr);
		kfree(op_slots[i].da_ptr);
	}
}

int pktdma_build_buffer_ring2(int chid)
{
	int i;
	int j;
	u8 *ptr;
#ifdef GATHER_TEST
	int k;
	void *saddr;
#endif
	struct dma_chan_info info;
	apm_pktdma_get_total_chan(&info);

	printk("Testing on chan:%d\n", info.chan_en[0]);

	for (i = 0; i < NUM_SLOTS; i++) {
		if (op_slots[i].sa_ptr == NULL) {
			op_slots[i].sa_ptr = kzalloc(4+PKT_SIZE+4, GFP_KERNEL);
			if (!op_slots[i].sa_ptr) {
				printk("memory allocation failed\n");
				return -ENOMEM;
			}
		}
		*(u32 *) op_slots[i].sa_ptr = 0xC00DF00D;
		*(u32 *) (op_slots[i].sa_ptr + 4 + PKT_SIZE) = 0xC00DF00D;
		op_slots[i].sa = op_slots[i].sa_ptr + 4;
		ptr = op_slots[i].sa;
		for (j = 0; j < PKT_SIZE; j++)
			ptr[j] = j+1;
		
		*(u32 *) op_slots[i].sa = 0;
		
#ifdef GATHER_TEST
		saddr = (void *)op_slots[i].sa;
		for (j = 0, k = 0; j < STEST_GATHER_SIZE; j++) {
			op_slots[i].phy_sa[j] = __pa(saddr) + k;
			op_slots[i].len[j] = (16 * 1024);
			k += (16 * 1024);
		}
#else
		op_slots[i].psa = __pa(op_slots[i].sa);
		op_slots[i].byte_cnt = PKT_SIZE;

#endif
		if (op_slots[i].da_ptr == NULL) {
			op_slots[i].da_ptr = kzalloc(4+PKT_SIZE+4, GFP_KERNEL);
			if (!op_slots[i].da_ptr) {
				printk("memory allocation failed\n");
				return -ENOMEM;
			}
		}
		*(u32 *) op_slots[i].da_ptr = 0xC00DF00D;
		*(u32 *) (op_slots[i].da_ptr + 4 + PKT_SIZE) = 0xC00DF00D;
		op_slots[i].da = op_slots[i].da_ptr + 4;
		
		op_slots[i].pda = __pa(op_slots[i].da);
		op_slots[i].slot = i;
		op_slots[i].ctr = 0;
		op_slots[i].free = 1;
		op_slots[i].chan_id = info.chan_en[0];
		op_slots[i].signature_end = 0xC00DF00D; 
		op_slots[i].signature = 0xC00DF00D;
	}
	return 0;
}

int test_m2m_cb(struct  apm_pktdma_op_result *pktdma_result)
{
	struct op_item *op;
	int rc;
	
	op = (struct op_item *) pktdma_result->ctx;
	if (op == NULL) {
		printk("Operation pointer is NULL\n");
		return 0;
	}
#if defined(CONFIG_NOT_COHERENT_CACHE)
	apm_pktdma_invalidate_dest(op->da, op->byte_cnt);
#endif
	if (op->signature != 0xC00DF00D || op->signature_end != 0xC00DF00D) {
		printk("Operation signature invalid 0x%08X 0x%08X\n", 
			op->signature, op->signature_end);
		goto err;
	}
#if 0	
	if (*(u32 *) op->da_ptr != 0xC00DF00D || 
	    *(u32 *) (op->da_ptr + 4 + PKT_SIZE) != 0xC00DF00D) {
		printk("Operation dest data signature invalid 0x%08X 0x%08X\n", 
			*(u32 *) op->da_ptr, 
			*(u32 *) (op->da_ptr + 4 + PKT_SIZE));
		DUMP_PKT("Dest ", (void *) op->da_ptr, 4+PKT_SIZE+4);
		goto err;
	}
	

	if (*(u32 *) op->sa_ptr != 0xC00DF00D || 
	    *(u32 *) (op->sa_ptr + 4 + PKT_SIZE) != 0xC00DF00D) {
		printk("Operation src data signature invalid 0x%08X 0x%08X\n", 
			*(u32 *) op->sa_ptr, 
			*(u32 *) (op->sa_ptr + 4 + PKT_SIZE));
		DUMP_PKT("Src ", (void *) op->sa_ptr, 4+PKT_SIZE+4);
		goto err;
	}
#endif		
	if (!op->sa || !op->da) {
		printk("Source/destination pointer NULL 0x%p 0x%p "
			"0x%02X.%08X 0x%02X.%08X\n",
			op->sa, op->da, 
			HIDWORD(op->psa), LODWORD(op->psa),
			HIDWORD(op->pda), LODWORD(op->pda));
		goto err;
	}
			
	if (pktdma_result->err)
		printk("Operation error 0x%08X\n", pktdma_result->err);
	
	if (op_chk_mode == 1) {
		rc = memcmp(op->sa, op->da, PKT_SIZE);
		if (rc != 0) {
			u64 phy1;
			u64 phy2;
			
			printk("Check failed operation 0x%p signature 0x%08X "
				"0x%08X counter 0x%08X\n", 
				op, op->signature, op->signature_end, op->ctr);
			
			phy1 = __pa(op->sa_ptr);
			phy2 = __pa(op->da_ptr);		
			printk("sa_ptr 0x%p 0x%02X.%08X psa 0x%02X.%08X " 
			       "da_ptr 0x%p 0x%02X.%08X pda 0x%02X.%08X\n",
				op->sa_ptr, 
				HIDWORD(phy1),
				LODWORD(phy1),
				HIDWORD(op->psa),
				LODWORD(op->psa),
				op->da_ptr, 
				HIDWORD(phy2),
				LODWORD(phy2),
				HIDWORD(op->pda),
				LODWORD(op->pda));
			
			DUMP_PKT("Src ", (void *) op->sa_ptr, 4+PKT_SIZE+4);
			DUMP_PKT("Dst ", (void *) op->da_ptr, 4+PKT_SIZE+4);
			printk("PktDMA m2m completed mode %d count %d max %d "
				"submitted %d slot %d\n", 
				op_mode, op_data_done, op_data_max, 
				op_data_submitted, op_data_slot);
			op_data_done = 0;	
			op_data_submitted = 0;	
			op_data_max = 0;	
			op_inprogress = 0;		
		}
	}
	
err:
	op->free = 1;
	++op_data_done;
	
	if (op_mode == 0) {
		if (op_inprogress && op_data_submitted < op_data_max)
			test_m2m_submit(op->chan_id, op->slot);
	}


	if (op_data_done % (100*1000) == 0) {
		printk(".");
	}

	if (op_data_done >= op_data_max && op_inprogress) {
		struct timeval value;
		op_end = get_jiffies_64();
		op_inprogress = 0;
		jiffies_to_timeval(op_end - op_start, &value);
		/* jiffies %lld %lld */
		printk("PKTDMA m2m completed mode %d count %d max %d "
				"submitted %d slot %d size %d \n",
			op_mode, op_data_done, op_data_max, op_data_submitted,
			op_data_slot, PKT_SIZE/*,  op_start, op_end */);
		//printk("smp_core0: %lld, smp_core1: %lld\n", p_dev.smp_core0, p_dev.smp_core1);
		printk("Time elapsed %u s %u us\n", 
			(unsigned int) value.tv_sec,
			(unsigned int) value.tv_usec);
		/* pktdma_free_buffer_ring2(); */
	}
		
	return 0;
}
int pkt_cnt_dma = 0;
int test_m2m_submit(int chid, int slot)
{
	int rc;
	struct apm_pktdma_m2m_params iodma_m2m;
	struct op_item *op;
#ifndef GATHER_TEST
	u64 saddr[1];
#endif
	u64 daddr[1];
	
	op = &op_slots[slot];	
	op->free = 0;	
	*(u32 *) op->sa = ++op->ctr;

	daddr[0] = __pa(op->da);

#if defined(CONFIG_NOT_COHERENT_CACHE)
	apm_pktdma_flush_src(op->sa, op->byte_cnt);
#endif

#ifdef GATHER_TEST
	iodma_m2m.m2m_mode = IODMA_GATHER;
	iodma_m2m.sg_count = STEST_GATHER_SIZE;
	iodma_m2m.sa = op->phy_sa;
	iodma_m2m.byte_count = op->len;
#else
	iodma_m2m.m2m_mode = IODMA_COPY;
	iodma_m2m.sg_count = 0;
	saddr[0] = __pa(op->sa);
	iodma_m2m.sa = saddr;
	iodma_m2m.byte_count = &op->byte_cnt;
#endif
	iodma_m2m.chid = op->chan_id;
	//iodma_m2m.chid = chid;
	iodma_m2m.da = daddr;
	iodma_m2m.context = (void *) op;
	iodma_m2m.cb = test_m2m_cb;
	
#if 0
	if (!((op->pda & 0x4) && (op->psa & 0x4))) {
		u64 phy1;
		u64 phy2;
		
		phy1 = __pa(op->sa_ptr);
		phy2 = __pa(op->da_ptr);		
		printk("sa_ptr 0x%p 0x%02X.%08X psa 0x%02X.%08X " 
			"da_ptr 0x%p 0x%02X.%08X pda 0x%02X.%08X\n",
			op->sa_ptr, 
			HIDWORD(phy1),
			LODWORD(phy1),
			HIDWORD(op->psa),
			LODWORD(op->psa),
			op->da_ptr, 
			HIDWORD(phy2),
			LODWORD(phy2),
			HIDWORD(op->pda),
			LODWORD(op->pda));
	}
#endif
	++op_data_submitted;
	
	rc = apm_pktdma_m2m_xfer(&iodma_m2m);
	if (rc != 0)
		printk("apm_pktdma_m2m_xfer error %d\n", rc);
	return rc;
}
int pkt_cnt_qm = 0;
int test_m2m(int loop)
{
	int rc;
	int i;
	int idx;
	//struct timeval value_end;

	if (op_inprogress) {
		printk("Operation m2m is already running\n");
		printk("mode %d count %d max %d submitted %d slot %d size %d\n", 
			op_mode, op_data_done, op_data_max, op_data_submitted,
			op_data_slot, PKT_SIZE);
		return 0;
	}
	
	op_mode = 1;		
	op_inprogress = 1;
	op_data_done = 0;
	op_data_submitted = 0;
	op_data_max = loop;
	if (op_data_max == 0)
		op_data_max = 0xFFFFFFFF;
	op_data_slot = NUM_SLOTS;
	if (op_data_max < op_data_slot)
		op_data_slot = op_data_max;

	printk("Testing m2m operation %d slots %d size %d\n", 
		op_data_max, op_data_slot, PKT_SIZE);
	
	/* Initialize the slot */
	if (pktdma_build_buffer_ring2(0) < 0)
		return -1;
	
	printk("Submitting %d operation\n", op_data_slot);
		
	op_start = get_jiffies_64();
	
	for (i = 0; i < op_data_slot; i++) {
		rc = test_m2m_submit(0, i);
		if (rc) {
			printk("m2m failed error %d\n", rc);
			return rc;
		}
		/* udelay(10000);Special Test case */
	}
		
	if (op_data_max <= op_data_slot)
		return 0;
		
	/* Continuous check for free slot */
	idx = 0;
/*	printk("Waiting for operation complete...\n"); */	
	while (op_data_submitted < op_data_max) {
		struct op_item *op;
		
		/* Find a free slot */
		for (i = 0; 
		     i < op_data_slot && 
		     op_data_submitted < op_data_max; 
		     i++) {
		        op = &op_slots[idx];
			if (!op->free) {
				idx = (idx + 1) % op_data_slot; 
				continue;
			}
			rc = test_m2m_submit(0, idx);
			idx = (idx + 1) % op_data_slot; 
			if (rc) {
				printk("m2m failed error %d\n", rc);
				return rc;
			}
		}
		/* udelay(100); Special Test case */
		/* Sleep for a little bit */
		schedule();
	}
#if 1
	if (op_data_done < op_data_max && op_inprogress) {
		op_end = get_jiffies_64();
		op_inprogress = 0;
		jiffies_to_timeval(op_end - op_start, &value);
		printk("\nTime elapsed %u s %u us\n",
		       (unsigned int) value.tv_sec,
		       (unsigned int) value.tv_usec);
		printk("op_data_done= %d, op_data_max = %d\n", op_data_done, op_data_max);
		printk("Packets sent out from DMA driver = %d\n", pkt_cnt_dma);
		printk("Packets sent out from QM driver = %d\n", pkt_cnt_qm);
	}
#endif

	return 0;
}

#if 0
static int test_m2m_different_sizes(void)
{
	struct dma_chan_info info;
	u32 test_times = 0;

	apm_pktdma_get_total_chan(&info);

	printk("Testing on chan:%d\n", info.chan_en[0]);
	while (1) {
		apm_pktdma_test_m2m_direct_copy(info.chan_en[0], 2012);
		apm_pktdma_test_m2m_direct_copy(info.chan_en[0], 710);
		apm_pktdma_test_m2m_direct_copy(info.chan_en[0], 2012);
		test_times++;
		if (test_times % (1000) == 0) {
			printk(".");
		}
		udelay(1000);
	}

	return 0;
}
#endif

static int apm_pktdma_driver_ioctl(struct inode *inode, struct file *file,
				  unsigned int cmd, unsigned long arg)
{
	return 0;
}
static int apm_pktdma_driver_open(struct inode *inode, struct file *file)
{
	return 0;
}

static int apm_pktdma_driver_release(struct inode *inode, struct file *file)
{
	return 0;
}

static int apm_pktdma_driver_write(struct file *file, const char __user
		*user_data, size_t size, loff_t *offset)
{
	int i;
	struct dma_chan_info info;
	int chid;
	
	apm_pktdma_get_total_chan(&info);
	chid = info.chan_en[0];
#if 0
	ptr = kzalloc(size,GFP_KERNEL);
	copy_from_user(ptr,buf,1);

	user_data = ptr[0];
	copy_from_user(ptr,buf + 2 ,size - 1);
	len = simple_strtoul(ptr,&ptr,0);
	printk("Len is %d\n",len);
#endif
	if (*user_data == '0') {
		printk(KERN_INFO "PktDMA Test Driver\n");
		return 0;
	}
	if (*user_data == '1') {
#if 1
		for (i = 0; i < info.max_chan; i++) {
			printk("Testing chan:%d\n", info.chan_en[i]);
			apm_pktdma_test_m2m_direct_copy(info.chan_en[i], 33);
		}
#else
		test_m2m_different_sizes();
#endif
	} else if (*user_data == '2') {
		apm_pktdma_test_m2m_copy_gather(chid);
		
	} else if (*user_data == '3') {
		apm_pktdma_test_m2m_copy_scatter(chid);
		
	} else if (*user_data == '4') {
		op_chk_mode = 0;
		test_m2m(512*1024);

	} else if (*user_data == '5') {
		apm_pktdma_test_m2b_copy(chid);
		
	} else if (*user_data == '6') {
		apm_pktdma_test_m2b_gather(chid);
		
	} else if (*user_data == '7') {
		apm_pktdma_test_m2m_copy_stride();
		
	} else if (*user_data == '8') {
		apm_pktdma_test_p2m_copy();
		
	} else if (*user_data == '9') {
		/* Testing CRC */
		apm_pktdma_test_m2b_gen_crc32();

	} else if (*user_data == 'A') {
		/* Testing Checksum */
		apm_pktdma_test_m2b_check_crc32();
		
	} else if (*user_data == 'x') {
		/* Testing XOR */
		apm_pktdma_test_m2b_xor();
	} else if (*user_data == 'p') {
		printk("Test DMA Oper:\n"
			"1 : m2m Copy\n"
				"2 : m2m Gather\n"
				"3 : m2m Scatter\n"
				"4 : Speed Test (m2m - 1K PKTSIZE)\n"
				"5 : m2b Copy\n"
				"6 : m2b Gather\n"
				"7 : Stride Test\n"
				"8 : p2m Copy\n"
				"9 : Gen CRC32\n"
				"A : Check crc32\n"
				"x : m2b xor\n");
	}

	return 1;
}


struct file_operations apm_pktdma_driver_fops = {
	.owner		= THIS_MODULE,
	.open		= apm_pktdma_driver_open,
	.release	= apm_pktdma_driver_release,
	.ioctl		= apm_pktdma_driver_ioctl,
	.write		= apm_pktdma_driver_write,
};

/**
 * Module init/exit
 *
 */
int __init apm_pktdma_test_int(void)
{
	struct proc_dir_entry *entry;
	
	memset(op_slots, 0, sizeof(op_slots));
	op_inprogress = 0;
	
	entry = create_proc_entry("apm_dma_test", 0, NULL);
	if (entry)
		entry->proc_fops = &apm_pktdma_driver_fops;
	else
		PKTDMA_ERR("init failed in proc entry !!");

	return 0;
}
		
void __exit apm_pktdma_test_exit(void)
{
	remove_proc_entry("apm_dma_test", NULL);
}
module_init(apm_pktdma_test_int);
module_exit(apm_pktdma_test_exit);

MODULE_AUTHOR("Shasi Pulijala <spulijala@apm.com>");
MODULE_DESCRIPTION("Test Driver for Mamba PKTDMA");
MODULE_LICENSE("GPL");
