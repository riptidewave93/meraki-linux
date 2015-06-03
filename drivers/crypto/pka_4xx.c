/*******************************************************************************
 *
 * Copyright (c) 2008 Loc Ho <lho@amcc.com>
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 *
 * Detail Description:
 * This file defines ioctl structures for the Linux CryptoAPI interface. It
 * provides user space applications accesss into the Linux CryptoAPI
 * functionalities.
 *
 * @file pka4xx.c
 *
 * This file provides access and implementation of the high layer API to the
 * PKA registers.
 *
 *******************************************************************************
 */
#include  <asm/delay.h>
#include  <asm/dcr-native.h>
#include  <linux/irq.h>
#include "pka_4xx_access.h"
#include <crypto/pka_4xx.h>
#include "pka_4xx_firmware.h"

/**
 * PKA Functions
 *
 */
/* # of time to poll for synchronous operation */
#define PKA4XX_POLL_DONE_MAX_CNT	5000

#define PKA4XX_CSR_WRITE_RETURN(a, v)     \
	do { \
		rc = pka4xx_csr_hw_write32((a), (v)); \
		if (rc != RC_OK) \
		return rc; \
	} while(0);

#define PKA4XX_CSR_READ_RETURN(a, v)     \
	do { \
		rc = pka4xx_csr_hw_read32((a), (v)); \
		if (rc != RC_OK) \
		return rc; \
	} while(0);


#define PKA_ALIGN(x, a)		do {	\
					(x) += ((a)-1); \
					(x) &= ~((a)-1); \
				} while(0);
#define PKA_ALIGN_RVAL(x, a)	(((x) + ((a)-1)) & (~((a)-1)))

static u32 pkt_firmware_sizedw		= PKA_FIRMWARE_1_3_SIZEDW;
static const u32 *pka_firmware		= pka_firmware_1_3;

u32 msg_buf[20][10];
int msg_idx;

u32 pka4xx_pkcp_set_vec(u32 vecA_cnt,
		       u32 *vecA,
		       u32 vecB_cnt,
		       u32 *vecB)
{
	u32 addr;
	int rc, i;
	u32 val32;

	addr  = PKA_RAM_ADDR;
	/* Set PKA RAM address and load input A - multiplicand */
	PKA4XX_CSR_WRITE_RETURN(PKA_ALENGTH_ADDR, vecA_cnt);
	PKA4XX_CSR_WRITE_RETURN(PKA_APTR_ADDR, addr >> 2);
	PKA4XX_CSR_READ_RETURN(PKA_APTR_ADDR, &val32);
	PKA4XX_CSR_READ_RETURN(PKA_ALENGTH_ADDR, &val32);
	for(i = 0; i < vecA_cnt; i++, addr += 4) {
		PKA4XX_CSR_WRITE_RETURN(addr, vecA[i]);
		PKA4XX_CSR_READ_RETURN(addr, &val32);
		LPRINTF(LL_INFO, "addr %08X val %08X", addr, val32);
	}
	PKA_ALIGN(addr, 8);	/* Align 8-byte */
	/* Align 8-byte but use 2 as it is DWORD */
	/* Set PKA RAM address and load for input B - multiplier  */
	PKA4XX_CSR_WRITE_RETURN(PKA_BLENGTH_ADDR, vecB_cnt);
	PKA4XX_CSR_WRITE_RETURN(PKA_BPTR_ADDR, addr >> 2);
	PKA4XX_CSR_READ_RETURN(PKA_BPTR_ADDR, &val32);
	PKA4XX_CSR_READ_RETURN(PKA_BLENGTH_ADDR, &val32);
	for(i = 0; i < vecB_cnt; i++, addr += 4) {
		PKA4XX_CSR_WRITE_RETURN(addr, vecB[i]);
		PKA4XX_CSR_READ_RETURN(addr, &val32);
		LPRINTF(LL_INFO,
			"addr %08X val %08X", addr, val32);
	}
	PKA_ALIGN(addr, 8);	/* Align 8-byte */
	/* Set PKA RAM address for output C - product */
	PKA4XX_CSR_WRITE_RETURN(PKA_CPTR_ADDR, addr >> 2);

	return addr;
}

u32 pka4xx_addsub_set_vec(u32 input_cnt,
			  u32 *addendA,
			  u32 *subtrahend,
			  u32 *addendC)
{
	u32 addr;
	int rc, i;
	
	addr  = PKA_RAM_ADDR;
	/* Set PKA RAM address and load input A - addendA */
	PKA4XX_CSR_WRITE_RETURN(PKA_ALENGTH_ADDR, input_cnt);
	PKA4XX_CSR_WRITE_RETURN(PKA_APTR_ADDR, addr >> 2);
	for(i = 0; i < input_cnt; i++, addr += 4)
		PKA4XX_CSR_WRITE_RETURN(addr, addendA[i]);
	PKA_ALIGN(addr, 8);	/* Align 8-byte */
	/* Set PKA RAM address and load input B - subtrahend */
	PKA4XX_CSR_WRITE_RETURN(PKA_BPTR_ADDR, addr >> 2);
	for(i = 0; i < input_cnt; i++, addr += 4)
		PKA4XX_CSR_WRITE_RETURN(addr, subtrahend[i]);
	PKA_ALIGN(addr, 8);	/* Align 8-byte */
	/* Set PKA RAM address and load input C - addendC */
	PKA4XX_CSR_WRITE_RETURN(PKA_CPTR_ADDR, addr >> 2);
	for(i = 0; i < input_cnt; i++, addr += 4)
		PKA4XX_CSR_WRITE_RETURN(addr, addendC[i]);
	PKA_ALIGN(addr, 8);	/* Align 8-byte */
	/* Set PKA RAM address for output - result */
	PKA4XX_CSR_WRITE_RETURN(PKA_DPTR_ADDR, addr >> 2);

	return addr;
}


u32 pka4xx_shift_set_vec(u32 input_cnt,
			 u32 *input,
			 u8  shift)
{
	u32 addr;
	int rc, i;
	
	addr  = PKA_RAM_ADDR;
	/* Set PKA RAM address and load input A - input */
	PKA4XX_CSR_WRITE_RETURN(PKA_ALENGTH_ADDR, input_cnt);
	PKA4XX_CSR_WRITE_RETURN(PKA_APTR_ADDR, addr >> 2);
	for(i = 0; i < input_cnt; i++, addr += 4)
		PKA4XX_CSR_WRITE_RETURN(addr, input[i]);
	PKA_ALIGN(addr, 8);	/* Align 8-byte */
	/* Set shift value */
	PKA4XX_CSR_WRITE_RETURN(PKA_SHIFT_ADDR, shift);
	/* Set PKA RAM address for output - result */
	PKA4XX_CSR_WRITE_RETURN(PKA_CPTR_ADDR, addr >> 2);
	/* Save callback for asynchronous operation */

	return addr;
}

u32 pka4xx_expmod_crt_set_vec(u32 exp_len,
			      u32 *expP,
			      u32 *expQ,
			      u32 mod_inverse_len,
			      u32 *modP,
			      u32 *modQ,
			      u32 *inverseQ,
			      u32 *input)
{
	u32 addr;
	u32 oaddr_start = 0x00000000;
	u32 Daddr;
	int i, rc;
	u32 val32;

	addr  = PKA_RAM_ADDR + (oaddr_start << 2);
	
	PKA4XX_CSR_WRITE_RETURN(PKA_ALENGTH_ADDR, exp_len);
	PKA4XX_CSR_WRITE_RETURN(PKA_APTR_ADDR, addr >> 2);
	for(i = 0; i < exp_len; i++, addr += 4) {
		PKA4XX_CSR_WRITE_RETURN(addr, expP[i]);
		PKA4XX_CSR_READ_RETURN(addr, &val32);
                LPRINTF(LL_INFO, "addr 0x%08X expP val 0x%08X\n",
			addr, val32);
	}
	PKA_ALIGN(addr, 8);	/* Align 8-byte */

	for(i = 0; i < exp_len; i++, addr += 4) {
		PKA4XX_CSR_WRITE_RETURN(addr, expQ[i]);
		PKA4XX_CSR_READ_RETURN(addr, &val32);
		LPRINTF(LL_INFO, "addr 0x%08X expQ val 0x%08X\n",
			addr, val32);
	}
	PKA_ALIGN(addr, 8);	/* Align 8-byte */

	/* Set PKA RAM address and load input modP and modQ */
	PKA4XX_CSR_WRITE_RETURN(PKA_BLENGTH_ADDR, mod_inverse_len);
	PKA4XX_CSR_WRITE_RETURN(PKA_BPTR_ADDR, addr >> 2);
	for(i = 0; i < mod_inverse_len; i++, addr += 4) {
		PKA4XX_CSR_WRITE_RETURN(addr, modP[i]);
		PKA4XX_CSR_READ_RETURN(addr, &val32);
                LPRINTF(LL_INFO, "addr 0x%08X modP val 0x%08X\n",
			addr, val32);
	}
	addr += 8;/*mm */		/* Require 1 extra DWORD */
	PKA_ALIGN(addr, 8);	/* Align 8-byte */

	for(i = 0; i < mod_inverse_len; i++, addr += 4) {
		PKA4XX_CSR_WRITE_RETURN(addr, modQ[i]);
		PKA4XX_CSR_READ_RETURN(addr, &val32);
                LPRINTF(LL_INFO, "addr 0x%08X mod Q val 0x%08X\n",
			addr, val32);
	}
	addr += 4;		/* Require 1 extra DWORD */
	PKA_ALIGN(addr, 8);	/* Align 8-byte */

	/* Set PKA RAM address and load input inverseQ */
	PKA4XX_CSR_WRITE_RETURN(PKA_CPTR_ADDR, addr >> 2);
	for(i = 0; i < mod_inverse_len; i++, addr += 4) {
		PKA4XX_CSR_WRITE_RETURN(addr, inverseQ[i]);
		PKA4XX_CSR_READ_RETURN(addr, &val32);
		LPRINTF(LL_INFO, "addr 0x%08X invQ val 0x%08X\n",
			addr, val32);
	}
	PKA_ALIGN(addr, 8);	/* Align 8-byte */

	/* Set PKA RAM address for output - result */
	PKA4XX_CSR_WRITE_RETURN(PKA_DPTR_ADDR, addr >> 2);
	Daddr = addr;
	for(i = 0; i < (mod_inverse_len<<1); i++, addr += 4) {
		PKA4XX_CSR_WRITE_RETURN(addr, input[i]);
		PKA4XX_CSR_READ_RETURN(addr, &val32);
		LPRINTF(LL_INFO, "addr 0x%08X input val 0x%08X\n",
		       addr, val32);
	}

	return Daddr;
}

u32  pka4xx_expmod_set_vec(u32 base_mod_cnt, u32 *base,
			   u32 *modulus,
			   u32 exponent_cnt,
			   u32 *exponent)
{
	u32 addr;
	u32 oaddr_start = 0x00000000;
	u32 val32;
	int rc, i;
	
	addr  = PKA_RAM_ADDR + (oaddr_start << 2);

	PKA4XX_CSR_WRITE_RETURN(PKA_ALENGTH_ADDR, exponent_cnt);
	PKA4XX_CSR_WRITE_RETURN(PKA_APTR_ADDR, addr >> 2);
	for(i = 0; i < exponent_cnt; i++, addr += 4) {
		PKA4XX_CSR_WRITE_RETURN(addr, exponent[i]);
		PKA4XX_CSR_READ_RETURN(addr, &val32);
		LPRINTF(LL_INFO, "addr 0x%08X A val 0x%08X",
			addr, val32);
	}
	PKA_ALIGN(addr, 8);	/* Align 8-byte */
	/* Set PKA RAM address and load input B - modulus */
	PKA4XX_CSR_WRITE_RETURN(PKA_BLENGTH_ADDR, base_mod_cnt);
	PKA4XX_CSR_WRITE_RETURN(PKA_BPTR_ADDR, addr >> 2);
	for(i = 0; i < base_mod_cnt; i++, addr += 4) {
		PKA4XX_CSR_WRITE_RETURN(addr, modulus[i]);
		PKA4XX_CSR_READ_RETURN(addr, &val32);
		LPRINTF(LL_INFO, "addr 0x%08X B val 0x%08X",
			addr, val32);
	}
	addr += 4;		/* Require 1 extra DWORD */
	PKA_ALIGN(addr, 8);	/* Align 8-byte */
	/* Set PKA RAM address and load input C - base */
	PKA4XX_CSR_WRITE_RETURN(PKA_CPTR_ADDR, addr >> 2);
	for(i = 0; i < base_mod_cnt; i++, addr += 4) {
		PKA4XX_CSR_WRITE_RETURN(addr, base[i]);
		PKA4XX_CSR_READ_RETURN(addr, &val32);
		LPRINTF(LL_INFO, "addr 0x%08X C val 0x%08X",
			addr, val32);
	}
	addr += 4;		/* Require 1 extra DWORD */
	PKA_ALIGN(addr, 8);	/* Align 8-byte */
	/* Set PKA RAM address for output - result */
	PKA4XX_CSR_WRITE_RETURN(PKA_DPTR_ADDR, addr >> 2);

	return addr;
}

void pka4xx_process_completed_event (struct pka4xx_op *op)
{
	int status = RC_OK;
	pka4xx_cb callback = NULL;

	callback = op->cb;
	op->cb = NULL;
	if (callback)
		(*callback)(op->ctx, status);
}

void pka4xx_tasklet_cb (unsigned long data)
{
	struct list_head *pos;
	struct list_head *tmp;
	
	unsigned long flags;

	spin_lock_irqsave(&pka_get_ctx()->lock, flags);
	
	list_for_each_safe(pos, tmp, &pka_get_ctx()->completed_event_queue) {
		struct pka4xx_op *item;
		item = list_entry(pos, struct pka4xx_op, next);
		list_del(pos);
		spin_unlock_irqrestore(&pka_get_ctx()->lock,flags);
		pka4xx_process_completed_event(item);
		spin_lock_irqsave(&pka_get_ctx()->lock, flags);
	}
	
	spin_unlock_irqrestore(&pka_get_ctx()->lock,flags);
}

static u8  pka4xx_pending_op(void)
{
	return pka_get_ctx()->op_head != pka_get_ctx()->op_tail;
}

static struct pka4xx_op * pka4xx_get_op_item(void)
{
	u32 tail;

	if (pka_get_ctx()->op_tail == PKA4XX_PENDING_OP_MAX-1)
		tail = 0;
	else
		tail = pka_get_ctx()->op_tail + 1;

	if (tail == pka_get_ctx()->op_head) {
		printk(LL_ERR "No free descriptor available for operation "
				 "queuing\n");
		return NULL;
	}
	return &pka_get_ctx()->op[pka_get_ctx()->op_tail];
}

static int pka4xx_start_op(struct pka4xx_op *op, int interrupt_mode)
{
	int	rc;
	u8 	restart = 0;
	u32 Daddr;
	u32 Caddr, addr;
	u32 val32;

	if (!interrupt_mode) {
		restart = !(pka_get_ctx()->op_head != pka_get_ctx()->op_tail);
	
		if (pka_get_ctx()->op_tail == PKA4XX_PENDING_OP_MAX-1)
			pka_get_ctx()->op_tail = 0;
		else
			pka_get_ctx()->op_tail++;
	}
	
	if (restart || interrupt_mode) {
		switch(op->opcode) {
		case 0:	/* Canceled */
			return RC_OK;
		case PKA_FUNCTION_DIV:
			/* Passing to pka4xx_div_set_vec the order of
			 * dividend_cnt, dividend, divisor_cnt, divisor
			 */
			LPRINTF(LL_INFO, "Starting async Div PKA operation \n");
			Caddr = pka4xx_pkcp_set_vec(op->async_pkcp.vecA_cnt,
					op->async_pkcp.vecA,
					op->async_pkcp.vecB_cnt,
					op->async_pkcp.vecB);
			op->ramC_addr = Caddr;
			addr = Caddr;
			addr += (op->async_pkcp.vecB_cnt + 1) * 4;
			PKA_ALIGN(addr, 8);	/* Align 8-byte */
			/* Select PKA RAM address for output D - quotient */
			PKA4XX_CSR_WRITE_RETURN(PKA_DPTR_ADDR, addr >> 2);
			PKA4XX_CSR_READ_RETURN(PKA_DPTR_ADDR, &val32);
			op->ramD_addr = addr;
			break;
		case PKA_FUNCTION_MUL:
		case PKA_FUNCTION_MOD:
		case PKA_FUNCTION_ADD:
		case PKA_FUNCTION_SUB:
		case PKA_FUNCTION_COMPARE:
			Caddr = pka4xx_pkcp_set_vec(op->async_pkcp.vecA_cnt,
					       op->async_pkcp.vecA,
					       op->async_pkcp.vecB_cnt,
					       op->async_pkcp.vecB);
			op->ramC_addr = Caddr;
			break;
		case PKA_FUNCTION_ADDSUB:
			LPRINTF(LL_INFO, "Starting async ADDSUB PKA operation\n");
			Daddr = pka4xx_addsub_set_vec(op->async_pkcp.vecA_cnt,
					op->async_pkcp.vecA,
					op->async_pkcp.vecB,
					op->async_pkcp.vec_addsub_C);
			op->ramD_addr = Daddr;
			break;
		case PKA_FUNCTION_RSHIFT:
		case PKA_FUNCTION_LSHIFT:
			Caddr = pka4xx_shift_set_vec(op->async_pkcp.vecA_cnt,
					op->async_pkcp.vecA,
					op->async_pkcp.shift_val);
			op->ramC_addr = Caddr;
			break;
		case PKA_FUNCTION_SEQOP_EXPMOD_ACT2:
		case PKA_FUNCTION_SEQOP_EXPMOD_ACT4:
		case PKA_FUNCTION_SEQOP_EXPMOD_VAR:
			Daddr = pka4xx_expmod_set_vec
					(op->async_expmod.base_mod_cnt,
					op->async_expmod.base,
					op->async_expmod.modulus,
					op->async_expmod.exp_cnt,
					op->async_expmod.exp);
			op->ramD_addr = Daddr;
			break;
		case PKA_FUNCTION_SEQOP_EXPMOD_CRT:
			/* No pending operation before adding this operation
			* id restart = 1
			*/
			Daddr = pka4xx_expmod_crt_set_vec
					(op->async_expmod_crt.exp_len,
					op->async_expmod_crt.expP,
					op->async_expmod_crt.expQ,
					op->async_expmod_crt.mod_inverse_len,
					op->async_expmod_crt.modP,
					op->async_expmod_crt.modQ,
					op->async_expmod_crt.inverseQ,
					op->async_expmod_crt.input);
			op->ramD_addr = Daddr;
			break;
		default:
			printk(LL_ERR "No operation in async mode\n");
			return RC_OK;
		}
		if (op->opcode == PKA_FUNCTION_SEQOP_EXPMOD_VAR ||
				  op->opcode == PKA_FUNCTION_SEQOP_EXPMOD_CRT) {
			PKA4XX_CSR_WRITE_RETURN(PKA_SHIFT_ADDR, op->resultC_cnt);
		}
		PKA4XX_CSR_WRITE_RETURN(PKA_FUNCTION_ADDR,
					PKA_FUNCTION_RUN | op->opcode);
	}
	return RC_OK;
}

irqreturn_t pka4xx_irq_handler(int irq, void * id)
{
	int	rc;
	u32     i;
	u32	val;
	struct pka4xx_op *op;
	struct pka4xx_op *next_op;
	unsigned long flags;

	if (!pka4xx_pending_op()) {
		LPRINTF(LL_INFO,
			"No pending op in pka4xx_irq_handler !!\n");
		return 0;
	}
	op = &pka_get_ctx()->op[pka_get_ctx()->op_head];
	switch(op->opcode) {
	case 0:	/* Canceled */
		op->cb = NULL;
		break;
	case PKA_FUNCTION_COMPARE:
		PKA4XX_CSR_READ_RETURN(PKA_COMPARE_ADDR, &val);
		if (val & PKA_COMPARE_EQUAL)
			*op->resultC_addr = 0;
		else if (val & PKA_COMPARE_LESSTHAN)
			*op->resultC_addr = -1;
		else
			*op->resultC_addr = 1;
		break;
	case PKA_FUNCTION_SEQOP_EXPMOD_ACT2:
	case PKA_FUNCTION_SEQOP_EXPMOD_ACT4:
	case PKA_FUNCTION_SEQOP_EXPMOD_VAR:
	case PKA_FUNCTION_SEQOP_EXPMOD_CRT:
		for(i = 0; i < op->resultD_cnt; op->ramD_addr += 4) {
			pka4xx_csr_hw_read32(op->ramD_addr,
						&op->resultD_addr[i]);
			msg_buf[msg_idx][i] = op->resultD_addr[i];
			LPRINTF(LL_INFO, "res expmod  0x%08x",
				msg_buf[msg_idx][i]);
			i++;
		}
		break;
	case PKA_FUNCTION_ADDSUB:
		for(i = 0; i < op->resultD_cnt; op->ramD_addr += 4)
		pka4xx_csr_hw_read32(op->ramD_addr,
				&op->resultD_addr[i++]);
		break;	
	case PKA_FUNCTION_DIV:
		for(i = 0; i < op->resultC_cnt; op->ramC_addr += 4)
			pka4xx_csr_hw_read32(op->ramC_addr,
					&op->resultC_addr[i++]);
		for(i = 0; i < op->resultD_cnt; op->ramD_addr += 4)
			pka4xx_csr_hw_read32(op->ramD_addr,
					&op->resultD_addr[i++]);
		break;
	default:
		for(i = 0; i < op->resultC_cnt; op->ramC_addr += 4)
		pka4xx_csr_hw_read32(op->ramC_addr,
					&op->resultC_addr[i++]);
		break;
	}

	if (pka_get_ctx()->op_head == PKA4XX_PENDING_OP_MAX - 1)
		pka_get_ctx()->op_head  = 0;
	else 
		pka_get_ctx()->op_head =
			(pka_get_ctx()->op_head + 1) % PKA4XX_PENDING_OP_MAX;
	
	next_op = &pka_get_ctx()->op[pka_get_ctx()->op_head];
	
	spin_lock_irqsave(&pka_get_ctx()->lock, flags);
	list_add_tail(&op->next, &pka_get_ctx()->completed_event_queue);
	spin_unlock_irqrestore(&pka_get_ctx()->lock,flags);
	
	if (!pka4xx_pending_op()) {
		LPRINTF(LL_INFO, "No pending op in pka4xx_irq_handler\n");
		tasklet_schedule(&pka_get_ctx()->tasklet);
		return IRQ_HANDLED;
	}
	pka4xx_start_op(next_op, 1);
	tasklet_schedule(&pka_get_ctx()->tasklet);
	
	return IRQ_HANDLED;
}

static int pka4xx_wait2complete(void)
{
	int rc;
	u32 val;
	u32 tried = 0;

	do {
		udelay(1);
		PKA4XX_CSR_READ_RETURN(PKA_FUNCTION_ADDR, &val);
		if (!(val & PKA_FUNCTION_RUN)) {
			return RC_OK;
		}
		tried++;
	} while (tried < PKA4XX_POLL_DONE_MAX_CNT);

	LPRINTF(LL_INFO "Returning busy after tried count = %d", tried);
	return RC_EBUSY;
}

int  pka4xx_mul(pka4xx_cb cb, void *ctx, u32 *op_id,
		u32 multiplicand_cnt, u32 *multiplicand,
		u32 multiplier_cnt, u32 *multiplier,
		u32 *product)
{
	int  rc;
	u32 addr;		/* Address of PKA RAM */
	struct pka4xx_op *pka_op;
	u32 i;

#ifdef PPR_PKA_DEBUG
	if (multiplicand_cnt > PKA4XX_VECTOR_MAXSIZE ||
	    multiplier_cnt > PKA4XX_VECTOR_MAXSIZE)
		return RC_INVALID_PARM;
#endif
	if (cb == NULL) {
		if (pka4xx_pending_op())
			return RC_EBUSY;
		pka_op 	     = NULL;
	} else {
		pka_op = pka4xx_get_op_item();
		if (pka_op == NULL)
			return RC_EBUSY;
	}

	/* Save callback for asynchronous operation */
	if (!cb) {
		addr = pka4xx_pkcp_set_vec(multiplicand_cnt, multiplicand,
					  multiplier_cnt, multiplier);
		/* Start the operation */
		PKA4XX_CSR_WRITE_RETURN(PKA_FUNCTION_ADDR,
					PKA_FUNCTION_RUN | PKA_FUNCTION_MUL);
		rc = pka4xx_wait2complete();
		
		if (rc != RC_OK)
			return rc;
		multiplicand_cnt += multiplier_cnt;
		for(i = 0; i < multiplicand_cnt; i++) {
			PKA4XX_CSR_READ_RETURN(addr, &product[i]);
			LPRINTF(LL_INFO, "result addr 0x%08x value 0x%08x",
						addr, product[i]);
			addr += 4;
		}
		return RC_OK;
	}
	/* Asynchronous operation */
	pka_op->opcode	       = PKA_FUNCTION_MUL;
	pka_op->cb             = cb;
	pka_op->ctx            = ctx;
	pka_op->resultC_cnt    = multiplicand_cnt+multiplier_cnt;
	pka_op->resultC_addr   = product;
	pka_op->async_pkcp.vecA_cnt = multiplicand_cnt;
	pka_op->async_pkcp.vecA = multiplicand;
	pka_op->async_pkcp.vecB_cnt = multiplier_cnt;
	pka_op->async_pkcp.vecB = multiplier;
	
	if (op_id)
		*op_id = pka_op->id;
	pka4xx_start_op(pka_op, 0);
	return RC_EINPROGRESS;
}

int  pka4xx_div(pka4xx_cb cb, void *ctx, u32 *op_id,
		u32 dividend_cnt, u32 *dividend,
		u32 divisor_cnt, u32 *divisor,
		u32 *remainder, u32 *quotient)
{
	int  rc;
	u32 addr;		/* Address of PKA RAM */
	struct pka4xx_op *pka_op;
	u32 resultC_addr;
	u32 resultD_addr;
	u32 i;
        u32 val32;

#ifdef PPR_PKA_DEBUG
	if (dividend_cnt > PKA4XX_VECTOR_MAXSIZE ||
	    divisor_cnt > PKA4XX_VECTOR_MAXSIZE ||
	    divisor_cnt > dividend_cnt)
		return RC_INVALID_PARM;
#endif
	if (cb == NULL) {
		if (pka4xx_pending_op())
			return RC_EBUSY;
		pka_op 	     = NULL;
	} else {
		pka_op = pka4xx_get_op_item();
		if (pka_op == NULL)
			return RC_EBUSY;
	}

	/* Save callback for asynchronous operation */
	if (!cb) {
		resultC_addr = pka4xx_pkcp_set_vec(dividend_cnt, dividend,
				divisor_cnt, divisor);
		addr = resultC_addr;
		addr += (divisor_cnt + 1) * 4;
		PKA_ALIGN(addr, 8);	/* Align 8-byte */
		/* Select PKA RAM address for output D - quotient */
		PKA4XX_CSR_WRITE_RETURN(PKA_DPTR_ADDR, addr >> 2);
		PKA4XX_CSR_READ_RETURN(PKA_DPTR_ADDR, &val32);
		resultD_addr = addr;
		
		/* Start the operation */
		PKA4XX_CSR_WRITE_RETURN(PKA_FUNCTION_ADDR,
					PKA_FUNCTION_RUN | PKA_FUNCTION_DIV);
		rc = pka4xx_wait2complete();
		if (rc != RC_OK)
			return rc;
		for(i = 0; i < divisor_cnt; i++) {
			PKA4XX_CSR_READ_RETURN(resultC_addr, &remainder[i]);
                        LPRINTF(LL_INFO, "C remaider : 0x%08x",
				remainder[i]);
			resultC_addr += 4;
		}
		dividend_cnt -= divisor_cnt;
		for(i = 0; i <= dividend_cnt /* Use = for + 1 */; ) {
			PKA4XX_CSR_READ_RETURN(resultD_addr,
					&quotient[i++]);
			resultD_addr += 4;
		}

		return RC_OK;
	}
	/* Setting params for Asynchronous operation */
	pka_op->opcode	       = PKA_FUNCTION_DIV;
	pka_op->cb             = cb;
	pka_op->ctx            = ctx;
	pka_op->resultC_cnt    = divisor_cnt;
	pka_op->resultD_cnt    = dividend_cnt-divisor_cnt+1;
	pka_op->resultC_addr   = remainder;
	pka_op->resultD_addr   = quotient;
	pka_op->async_pkcp.vecA_cnt = dividend_cnt;
	pka_op->async_pkcp.vecA = dividend;
	pka_op->async_pkcp.vecB_cnt = divisor_cnt;
	pka_op->async_pkcp.vecB = divisor;

	if (op_id)
		*op_id = pka_op->id;
	pka4xx_start_op(pka_op, 0);
	return RC_EINPROGRESS;
}

int  pka4xx_mod(pka4xx_cb cb, void *ctx, u32 *op_id,
		u32 dividend_cnt, u32 *dividend,
		u32 divisor_cnt, u32 *divisor,
		u32 *remainder)
{
	int  rc;
	u32 addr;		/* Address of PKA RAM */
	struct pka4xx_op *pka_op;
	u32 i;

#ifdef PPR_PKA_DEBUG
	if (dividend_cnt > PKA4XX_VECTOR_MAXSIZE ||
	    divisor_cnt > PKA4XX_VECTOR_MAXSIZE)
		return RC_INVALID_PARM;
#endif
	if (cb == NULL) {
		if (pka4xx_pending_op())
			return RC_EBUSY;
		pka_op 	     = NULL;
	} else {
		pka_op = pka4xx_get_op_item();
		if (pka_op == NULL)
			return RC_EBUSY;
	}

	/* Save callback for asynchronous operation */
	if (!cb) {
		addr = pka4xx_pkcp_set_vec(dividend_cnt, dividend,
					  divisor_cnt, divisor);
		/* Start the operation */
		PKA4XX_CSR_WRITE_RETURN(PKA_FUNCTION_ADDR,
					PKA_FUNCTION_RUN | PKA_FUNCTION_MOD);
		rc = pka4xx_wait2complete();
		if (rc != RC_OK)
			return rc;
		for(i = 0; i < divisor_cnt; ) {
			PKA4XX_CSR_READ_RETURN(addr, &remainder[i++]);
			addr += 4;
		}
		return RC_OK;
	}
	/* Asynchronous operation */
	pka_op->opcode         = PKA_FUNCTION_MOD;
	pka_op->cb             = cb;
	pka_op->ctx            = ctx;
	pka_op->resultC_cnt    = divisor_cnt;
	pka_op->resultC_addr   = remainder;
	pka_op->async_pkcp.vecA_cnt = dividend_cnt;
	pka_op->async_pkcp.vecA = dividend;
	pka_op->async_pkcp.vecB_cnt = divisor_cnt;
	pka_op->async_pkcp.vecB = divisor;
	if (op_id)
		*op_id = pka_op->id;
	pka4xx_start_op(pka_op, 0);
	return RC_EINPROGRESS;
}

int  pka4xx_add(pka4xx_cb cb, void *ctx, u32 *op_id,
		u32 addendA_cnt, u32 *addendA,
		u32 addendB_cnt, u32 *addendB, u32 *sum)
{
	int  rc;
	u32 addr;		/* Address of PKA RAM */
	struct pka4xx_op *pka_op;
	u32 result_len;
	u32 i;

#ifdef PPR_PKA_DEBUG
	if (addendA_cnt > PKA4XX_VECTOR_MAXSIZE ||
	    addendB_cnt > PKA4XX_VECTOR_MAXSIZE)
		return RC_INVALID_PARM;
#endif
	result_len   = addendA_cnt > addendB_cnt ? (addendA_cnt+1) :
						   (addendB_cnt+1); 
	if (cb == NULL) {
		if (pka4xx_pending_op())
			return RC_EBUSY;
		pka_op = NULL;
	} else {
		pka_op = pka4xx_get_op_item();
		if (pka_op == NULL)
			return RC_EBUSY;
	}

	/* Save callback for asynchronous operation */
	if (!cb) {
		addr = pka4xx_pkcp_set_vec(addendA_cnt, addendA,
					  addendB_cnt, addendB);
		/* Start the operation */
		PKA4XX_CSR_WRITE_RETURN(PKA_FUNCTION_ADDR,
					PKA_FUNCTION_ADD | PKA_FUNCTION_RUN);
		rc = pka4xx_wait2complete();
		if (rc != RC_OK)
			return rc;
		for(i = 0; i < result_len; ) {
			PKA4XX_CSR_READ_RETURN(addr, &sum[i++]);
			addr += 4;
		}
		LPRINTF(LL_INFO, "result = %d,addr = 0x%08x",
			*sum, (unsigned int)addr);
		return RC_OK;
	}
	/* Asynchronous operation */
	pka_op->opcode         = PKA_FUNCTION_ADD;
	pka_op->cb             = cb;
	pka_op->ctx            = ctx;
	pka_op->resultC_cnt    = result_len;
	pka_op->resultC_addr   = sum;
	pka_op->async_pkcp.vecA_cnt = addendA_cnt;
	pka_op->async_pkcp.vecA = addendA;
	pka_op->async_pkcp.vecB_cnt = addendB_cnt;
	pka_op->async_pkcp.vecB = addendB;
	
	if (op_id)
		*op_id = pka_op->id;
	pka4xx_start_op(pka_op, 0);
	return RC_EINPROGRESS;
}

int  pka4xx_sub(pka4xx_cb cb, void *ctx, u32 *op_id,
		u32 minuend_cnt, u32 *minuend,
		u32 subtrahend_cnt, u32 *subtrahend, u32 *difference)
{
	int  rc;
	u32 addr;		/* Address of PKA RAM */
	struct pka4xx_op *pka_op;
	u32 result_len;
	u32 i;

#ifdef PPR_PKA_DEBUG
	if (minuend_cnt > PKA4XX_VECTOR_MAXSIZE ||
	    subtrahend_cnt > PKA4XX_VECTOR_MAXSIZE)
		return RC_INVALID_PARM;
#endif
	result_len   = minuend_cnt > subtrahend_cnt ? minuend_cnt :
						      subtrahend_cnt;
	if (cb == NULL) {
		if (pka4xx_pending_op())
			return RC_EBUSY;
		pka_op 	     = NULL;
	} else {
		pka_op = pka4xx_get_op_item();
		if (pka_op == NULL)
			return RC_EBUSY;
	}

	/* Save callback for asynchronous operation */
	if (!cb) {
		addr = pka4xx_pkcp_set_vec(minuend_cnt, minuend,
					  subtrahend_cnt, subtrahend);
		/* Start the operation */
		PKA4XX_CSR_WRITE_RETURN(PKA_FUNCTION_ADDR,
					PKA_FUNCTION_SUB | PKA_FUNCTION_RUN);
		rc = pka4xx_wait2complete();
		if (rc != RC_OK)
			return rc;
		for(i = 0; i < result_len; ) {
			PKA4XX_CSR_READ_RETURN(addr, &difference[i++]);
			addr += 4;
		}
		return RC_OK;
	}
	/* Asynchronous operation */
	pka_op->opcode         = PKA_FUNCTION_SUB;
	pka_op->cb             = cb;
	pka_op->ctx            = ctx;
	pka_op->resultC_cnt    = result_len;
	pka_op->resultC_addr   = difference;
	pka_op->async_pkcp.vecA_cnt = minuend_cnt;
	pka_op->async_pkcp.vecA = minuend;
	pka_op->async_pkcp.vecB_cnt = subtrahend_cnt;
	pka_op->async_pkcp.vecB = subtrahend;
	
	if (op_id)
		*op_id = pka_op->id;
	pka4xx_start_op(pka_op, 0);

	return RC_EINPROGRESS;
}

int  pka4xx_addsub(pka4xx_cb cb, void *ctx, u32 *op_id,
		   u32 input_cnt, u32 *addendA,
		   u32 *addendC, u32 *subtrahend, u32 *result)
{
	int  rc;
	u32 addr;		/* Address of PKA RAM */
	struct pka4xx_op * pka_op;
	u32 i;

#ifdef PPR_PKA_DEBUG
	if (input_cnt > PKA4XX_VECTOR_MAXSIZE)
		return RC_INVALID_PARM;
#endif
	if (cb == NULL) {
		if (pka4xx_pending_op())
			return RC_EBUSY;
		pka_op 	     = NULL;
	} else {
		pka_op = pka4xx_get_op_item();
		if (pka_op == NULL)
			return RC_EBUSY;
	}

	/* Save callback for asynchronous operation */
	if (!cb) {
		addr = pka4xx_addsub_set_vec(input_cnt, addendA,
					     subtrahend, addendC);
		/* Start the operation */
		PKA4XX_CSR_WRITE_RETURN(PKA_FUNCTION_ADDR,
					PKA_FUNCTION_ADDSUB | PKA_FUNCTION_RUN);
		rc = pka4xx_wait2complete();
		if (rc != RC_OK)
			return rc;
		for(i = 0; i <= input_cnt /* Use = for + 1 */; ) {
			PKA4XX_CSR_READ_RETURN(addr, &result[i++]);
			addr += 4;
		}
		return RC_OK;
	}
	/* Asynchronous operation */
	pka_op->opcode         = PKA_FUNCTION_ADDSUB;
	pka_op->cb             = cb;
	pka_op->ctx            = ctx;
	pka_op->resultD_cnt    = input_cnt+1;
	pka_op->resultD_addr   = result;
	pka_op->async_pkcp.vecA_cnt = input_cnt;
	pka_op->async_pkcp.vecA = addendA;
	pka_op->async_pkcp.vecB_cnt = 0;
	pka_op->async_pkcp.vecB = subtrahend;
	pka_op->async_pkcp.vec_addsub_C = addendC;

	if (op_id)
		*op_id = pka_op->id;
	pka4xx_start_op(pka_op, 0);

	return RC_EINPROGRESS;
}

int  pka4xx_rshift(pka4xx_cb cb, void *ctx, u32 *op_id,
		   u32 input_cnt, u32 *input,
		   u8  shift, u32 *result)
{
	int  rc;
	u32 addr;		/* Address of PKA RAM */
	struct pka4xx_op *pka_op;
	u32 i;

#ifdef PPR_PKA_DEBUG
	if (input_cnt > PKA4XX_VECTOR_MAXSIZE)
		return RC_INVALID_PARM;
#endif
	if (cb == NULL) {
		if (pka4xx_pending_op())
			return RC_EBUSY;
		pka_op 	     = NULL;
	} else {
		pka_op = pka4xx_get_op_item();
		if (pka_op == NULL)
			return RC_EBUSY;
	}

	/* Save callback for asynchronous operation */
	if (!cb) {
		addr = pka4xx_shift_set_vec(input_cnt, input, shift);
		/* Start the operation */
		PKA4XX_CSR_WRITE_RETURN(PKA_FUNCTION_ADDR,
					PKA_FUNCTION_RSHIFT | PKA_FUNCTION_RUN);
		rc = pka4xx_wait2complete();
		if (rc != RC_OK)
			return rc;
		for(i = 0; i < input_cnt;) {
			PKA4XX_CSR_READ_RETURN(addr, &result[i++]);
			addr += 4;
		}
		return RC_OK;
	}
	/* Asynchronous operation */
	pka_op->opcode         = PKA_FUNCTION_RSHIFT;
	pka_op->cb             = cb;
	pka_op->ctx            = ctx;
	pka_op->resultC_cnt    = input_cnt;
	pka_op->resultC_addr   = result;
	pka_op->async_pkcp.vecA_cnt = input_cnt;
	pka_op->async_pkcp.vecA = input;
	pka_op->async_pkcp.shift_val = shift;

	if (op_id)
		*op_id = pka_op->id;
	pka4xx_start_op(pka_op, 0);

	return RC_EINPROGRESS;
}

int  pka4xx_lshift(pka4xx_cb cb, void *ctx,
		   u32 *op_id, u32 input_cnt,
		   u32 *input, u8  shift,
		   u32 *result)
{
	int  rc;
	u32 addr;		/* Address of PKA RAM */
	struct pka4xx_op *pka_op;
	u32 result_len;
	u32 i;

#ifdef PPR_PKA_DEBUG
	if (input_cnt > PKA4XX_VECTOR_MAXSIZE)
		return RC_INVALID_PARM;
#endif
	result_len = shift == 0 ? input_cnt : (input_cnt+1);
	if (cb == NULL) {
		if (pka4xx_pending_op())
			return RC_EBUSY;
		pka_op 	     = NULL;
	} else  {
		pka_op = pka4xx_get_op_item();
		if (pka_op == NULL)
			return RC_EBUSY;
	}

	/* Save callback for asynchronous operation */
	if (!cb) {
		addr = pka4xx_shift_set_vec(input_cnt, input, shift);
		/* Start the operation */
		PKA4XX_CSR_WRITE_RETURN(PKA_FUNCTION_ADDR,
					PKA_FUNCTION_LSHIFT | PKA_FUNCTION_RUN);
		rc = pka4xx_wait2complete();
		if (rc != RC_OK)
			return rc;
		for(i = 0; i < result_len; ) {
			PKA4XX_CSR_READ_RETURN(addr, &result[i++]);
			addr += 4;
		}
		return RC_OK;
	}
	/* Asynchronous operation */
	pka_op->opcode         = PKA_FUNCTION_LSHIFT;
	pka_op->cb             = cb;
	pka_op->ctx            = ctx;
	pka_op->resultC_cnt    = result_len;
	pka_op->resultC_addr   = result;
	pka_op->async_pkcp.vecA_cnt = input_cnt;
	pka_op->async_pkcp.vecA = input;
	pka_op->async_pkcp.shift_val = shift;

	if (op_id)
		*op_id = pka_op->id;
	pka4xx_start_op(pka_op, 0);

	return RC_EINPROGRESS;
}

int  pka4xx_compare(pka4xx_cb cb, void *ctx, u32 *op_id,
		    u32 input1_cnt, u32 *input1,
		    u32 input2_cnt, u32 *input2,
		    int *result)
{
	int  rc;
	struct pka4xx_op *pka_op;
	u32 val;

#ifdef PPR_PKA_DEBUG
	if (input1_cnt > PKA4XX_VECTOR_MAXSIZE ||
	    input2_cnt > PKA4XX_VECTOR_MAXSIZE)
		return RC_INVALID_PARM;
#endif
	if (cb == NULL) {
		if (pka4xx_pending_op())
			return RC_EBUSY;
		pka_op 	     = NULL;
	} else {
		pka_op = pka4xx_get_op_item();
		if (pka_op == NULL)
			return RC_EBUSY;
	}

	/* Save callback for asynchronous operation */
	if (!cb) {
		pka4xx_pkcp_set_vec(input1_cnt, input1, input2_cnt,
				       input2);
		/* Start the operation */
		PKA4XX_CSR_WRITE_RETURN(PKA_FUNCTION_ADDR,
				PKA_FUNCTION_COMPARE | PKA_FUNCTION_RUN);
		rc = pka4xx_wait2complete();
		if (rc != RC_OK)
			return rc;
		PKA4XX_CSR_READ_RETURN(PKA_COMPARE_ADDR, &val);
		if (val & PKA_COMPARE_EQUAL)
			*result = 0;
		else if (val & PKA_COMPARE_LESSTHAN)
			*result = -1;
		else
			*result = 1;
		return RC_OK;
	}
	/* Asynchronous operation */
	pka_op->opcode         = PKA_FUNCTION_COMPARE;
	pka_op->cb             = cb;
	pka_op->ctx            = ctx;
	pka_op->resultC_cnt    = 1;
	pka_op->resultC_addr   = (u32 *)result;
	pka_op->async_pkcp.vecA_cnt = input1_cnt;
	pka_op->async_pkcp.vecA = input1;
	pka_op->async_pkcp.vecB_cnt = input2_cnt;
	pka_op->async_pkcp.vecB = input2;

	if (op_id)
		*op_id = pka_op->id;
	pka4xx_start_op(pka_op, 0);

	return RC_EINPROGRESS;
}

int  pka4xx_expmod(pka4xx_cb cb, void *ctx, u32 *op_id,
		   u8  odd_pwr_cnt,
		   u32 base_mod_cnt, u32 *base,
		   u32 *modulus,
		   u32 exponent_cnt, u32 *exponent,
		   u32 *result)
{
	int  rc;
	u32 addr;		/* Address of PKA RAM */
	struct pka4xx_op * pka_op;
	u32 cmd;
	u32 i;

#ifdef PPR_PKA_DEBUG
	if (odd_pwr_cnt > 16 || odd_pwr_cnt == 0 ||
	    base_mod_cnt > PKA4XX_VECTOR_MAXSIZE ||
	    exponent_cnt > PKA4XX_VECTOR_MAXSIZE)
		return RC_INVALID_PARM;
#endif

	if (cb == NULL) {
		if (pka4xx_pending_op())
			return RC_EBUSY;
		pka_op 	     = NULL;
	} else {
		pka_op = pka4xx_get_op_item();
		if (pka_op == NULL)
			return RC_EBUSY;
	}

	/* Start the operation */
	if (odd_pwr_cnt == 2) {
		cmd = PKA_FUNCTION_SEQOP_EXPMOD_ACT2;
	} else if (odd_pwr_cnt == 8) {
		cmd = PKA_FUNCTION_SEQOP_EXPMOD_ACT4;
	} else {
		PKA4XX_CSR_WRITE_RETURN(PKA_SHIFT_ADDR, odd_pwr_cnt);
		cmd = PKA_FUNCTION_SEQOP_EXPMOD_VAR;
	}
	/* Save callback for asynchronous operation */
	if (!cb) {
		addr = pka4xx_expmod_set_vec(base_mod_cnt, base, modulus,
					     exponent_cnt, exponent);
		PKA4XX_CSR_WRITE_RETURN(PKA_FUNCTION_ADDR,
				     cmd | PKA_FUNCTION_RUN);
		rc = pka4xx_wait2complete();
		if (rc != RC_OK)
			return rc;
		for(i = 0; i < base_mod_cnt; i++) {
			PKA4XX_CSR_READ_RETURN(addr, &result[i]);
			LPRINTF(LL_INFO, "output = 0x%08x ",
				result[i]);
			addr += 4;
		}	
		return RC_OK;
		
	}
	/* Asynchronous operation */
	pka_op->opcode         = cmd;
	pka_op->cb             = cb;
	pka_op->ctx            = ctx;
	pka_op->resultC_cnt    = odd_pwr_cnt; /* Save odd power cnt in here */
	pka_op->resultD_cnt    = base_mod_cnt;
	pka_op->resultC_addr   = NULL;
	pka_op->resultD_addr   = result;
	pka_op->async_expmod.base = base;
	pka_op->async_expmod.exp = exponent;
	pka_op->async_expmod.modulus = modulus;
	pka_op->async_expmod.base_mod_cnt = base_mod_cnt;
	pka_op->async_expmod.exp_cnt = exponent_cnt;

	if (op_id)
		*op_id = pka_op->id;
	pka4xx_start_op(pka_op, 0);

	return RC_EINPROGRESS;
}

int pka4xx_expmod_crt(pka4xx_cb cb, void *ctx, u32 *op_id,
		      u8  odd_pwr_cnt,
		      u32 exp_len, u32 *expP, u32 *expQ,
		      u32 mod_inverse_len, u32 *modP, u32 *modQ,
		      u32 *inverseQ, u32 *input,
		      u32 *result)
{
	int  rc;
	struct pka4xx_op *pka_op;
	u32 i;
        u32 Daddr;

#ifdef PPR_PKA_DEBUG
	if (exp_len > PKA4XX_VECTOR_MAXSIZE ||
	    mod_inverse_len > PKA4XX_VECTOR_MAXSIZE ||
	    odd_pwr_cnt > 16)
		return RC_INVALID_PARM;
#endif
	if (cb == NULL) {
		if (pka4xx_pending_op())
			return RC_EBUSY;
		pka_op 	     = NULL;
	} else {
		pka_op = pka4xx_get_op_item();
		if (pka_op == NULL)
			return RC_EBUSY;
	}

	if (!cb) {
		Daddr = pka4xx_expmod_crt_set_vec(exp_len, expP, expQ,
						mod_inverse_len,
						modP, modQ,
						inverseQ, input);
	} else {
		/* Asynchronous operation */
		pka_op->opcode         = PKA_FUNCTION_SEQOP_EXPMOD_CRT;
		pka_op->cb             = cb;
		pka_op->ctx            = ctx;
		pka_op->resultD_cnt    = mod_inverse_len<<1;
		pka_op->resultC_cnt    = odd_pwr_cnt; /* Use result C cnt for pwr cnt */
		pka_op->resultD_addr   = result;
		pka_op->async_expmod_crt.expP = expP;
		pka_op->async_expmod_crt.expQ = expQ;
		pka_op->async_expmod_crt.modP = modP;
		pka_op->async_expmod_crt.modQ = modQ;
		pka_op->async_expmod_crt.inverseQ = inverseQ;
		pka_op->async_expmod_crt.exp_len = exp_len;
		pka_op->async_expmod_crt.mod_inverse_len = mod_inverse_len;
		pka_op->async_expmod_crt.input = input;
	}
						
	/* Save callback for asynchronous operation */
	if (!cb) {
		/* Start the operation */
		PKA4XX_CSR_WRITE_RETURN(PKA_SHIFT_ADDR, odd_pwr_cnt);
		PKA4XX_CSR_WRITE_RETURN(PKA_FUNCTION_ADDR,
			PKA_FUNCTION_SEQOP_EXPMOD_CRT | PKA_FUNCTION_RUN);
		rc = pka4xx_wait2complete();
		if (rc != RC_OK)
			return rc;
		for(i = 0; i < (mod_inverse_len<<1); i++) {
			PKA4XX_CSR_READ_RETURN(Daddr, &result[i]);
                        LPRINTF(LL_INFO, "D addr : 0x%08x val 0x%08x",
				Daddr, result[i]);
			Daddr += 4;
		}
		return RC_OK;
	}

	if (op_id)
		*op_id = pka_op->id;
	pka4xx_start_op(pka_op, 0);

	return RC_EINPROGRESS;
}

int pka4xx_hw_init(void)
{
	int rc;
	u32 i;
	int result;
	u32 prog_addr;

	printk(LL_INFO "Initializing PKA...\n");

	/* Initialize context variable */
	for(i = 0; i < PKA4XX_PENDING_OP_MAX; i++) {
		pka_get_ctx()->op[i].id     = i+1;
		pka_get_ctx()->op[i].opcode = 0;
	}
	INIT_LIST_HEAD(&pka_get_ctx()->completed_event_queue);

	/* Load PKA firmware */
	LPRINTF(LL_INFO, "Loading PKA firmware PKA RAM Addr: 0x%08X size "
			"(DW): %d...",
			pka_get_ctx()->csr_paddr,
			pkt_firmware_sizedw);

	/* Put PKA Sequencer into reset to access firmware area */
	rc = pka4xx_csr_hw_write32(PKA_SEQ_CTRL_ADDR, PKA_SEQ_CTRL_RESET);
	if (rc != RC_OK) {
		LPRINTF(LL_ERR,
			"Failed to put PKA Sequencer into reset error 0x%08X",
			rc);
		return rc;
	}
	/* Now, load the firmware */
	prog_addr = PKA_PROGRAM_ADDR;
	for(i = 0; i < pkt_firmware_sizedw; i++, prog_addr += 4) {
		rc = pka4xx_csr_hw_write32(prog_addr, pka_firmware[i]);
		
		if (rc != RC_OK) {
			LPRINTF(LL_ERR,
				"Failed to load PKA firmware error 0x%08X", rc);
			return rc;
		}
	}
	/* Put PKA Sequencer into normal operation */
	rc = pka4xx_csr_hw_write32(PKA_SEQ_CTRL_ADDR, 0);
	if (rc != RC_OK) {
		LPRINTF(LL_ERR,
			"Failed to put PKA Sequencer into reset error 0x%08X",
			rc);
		return rc;
	}

	/* Register for interrupt */
	tasklet_init(&pka_get_ctx()->tasklet,
		      pka4xx_tasklet_cb, (unsigned long)pka_get_ctx()->op);

	result = request_irq(pka_get_ctx()->irq, pka4xx_irq_handler,
			     0, "PKA", NULL);
	if (result != 0)
		return result;

	set_irq_type(pka_get_ctx()->irq, IRQ_TYPE_EDGE_RISING);
	/* Comment this out to enable interrupt mode -- Now doing only polling mode */
	/* disable_irq(pka_get_ctx()->irq); */
	
	return RC_OK;
}

int 	pka4xx_hw_deinit(void)
{
	disable_irq(pka_get_ctx()->irq);
	free_irq(pka_get_ctx()->irq, NULL);
	return RC_OK;
}
