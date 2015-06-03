/**
 * AMCC SoC PPC4xx Crypto Driver
 *
 * Copyright (c) 2008 Applied Micro Circuits Corporation.
 * All rights reserved. James Hsiao <jhsiao@amcc.com>
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
 * This file implements the Linux crypto algorithms.
 */

#include <linux/kernel.h>
#include <linux/interrupt.h>
#include <linux/spinlock_types.h>
#include <linux/highmem.h>
#include <linux/scatterlist.h>
#include <linux/crypto.h>
#include <linux/hash.h>
#include <crypto/internal/hash.h>
#include <crypto/aead.h>
#include <linux/dma-mapping.h>
#include <linux/pci.h>
#include <crypto/algapi.h>
#include <crypto/aes.h>
#include <crypto/des.h>
#include <crypto/sha.h>
#include <crypto/authenc.h>
#include <net/ip.h>

#include "crypto4xx_reg_def.h"
#include "crypto4xx_sa.h"
#include "crypto4xx_core.h"

#define DEBUG_CRYPTESP 1
#ifdef DEBUG_CRYPTESP
#	define ESP_PHD		print_hex_dump
#else
#	define ESP_PHD(arg...)
#endif

#ifdef DEBUG_CRYPTESP

#define ESP_PRINTK printk
#else
#define ESP_PRINTK(KERN_INFO  arg...)
#endif

void set_dynamic_sa_command_0(struct dynamic_sa_ctl *sa, u32 save_h,
			      u32 save_iv, u32 ld_h, u32 ld_iv, u32 hdr_proc,
			      u32 h, u32 c, u32 pad_type, u32 op_grp, u32 op,
			      u32 dir)
{
	sa->sa_command_0.w = 0;
	sa->sa_command_0.bf.save_hash_state = save_h;
	sa->sa_command_0.bf.save_iv = save_iv;
	sa->sa_command_0.bf.load_hash_state = ld_h;
	sa->sa_command_0.bf.load_iv = ld_iv;
	sa->sa_command_0.bf.hdr_proc = hdr_proc;
	sa->sa_command_0.bf.hash_alg = h;
	sa->sa_command_0.bf.cipher_alg = c;
	sa->sa_command_0.bf.pad_type = pad_type & 3;
	sa->sa_command_0.bf.extend_pad = pad_type >> 2;
	sa->sa_command_0.bf.op_group = op_grp;
	sa->sa_command_0.bf.opcode = op;
	sa->sa_command_0.bf.dir = dir;
}

void set_dynamic_sa_command_1(struct dynamic_sa_ctl *sa, u32 cm, u32 hmac_mc,
			      u32 cfb, u32 esn, u32 sn_mask, u32 mute,
			      u32 cp_pad, u32 cp_pay, u32 cp_hdr)
{
	sa->sa_command_1.bf.crypto_mode31 = (cm & 4) >> 2;
	sa->sa_command_1.bf.crypto_mode9_8 = (cm & 3);
	sa->sa_command_1.bf.feedback_mode = cfb,
	sa->sa_command_1.bf.sa_rev = 1;
	sa->sa_command_1.bf.hmac_muting = hmac_mc;
	sa->sa_command_1.bf.extended_seq_num = esn;
	sa->sa_command_1.bf.seq_num_mask = sn_mask;
	sa->sa_command_1.bf.mutable_bit_proc = mute;
	sa->sa_command_1.bf.copy_pad = cp_pad;
	sa->sa_command_1.bf.copy_payload = cp_pay;
	sa->sa_command_1.bf.copy_hdr = cp_hdr;
}

/** Table lookup for SA Hash Digest length and
 *  Hash Contents (based on Hash type)
 */
unsigned int crypto4xx_sa_hash_tbl[3][HASH_ALG_MAX_CNT] = {
	/* Hash Contents */
	{ SA_HASH128_CONTENTS, SA_HASH160_CONTENTS, SA_HASH256_CONTENTS,
	SA_HASH256_CONTENTS, SA_HASH512_CONTENTS, SA_HASH512_CONTENTS },
	/* Digest len */
	{4 * 4, 5 * 4, 7 * 4, 8 * 4, 12 * 4, 16 * 4},
	/* SA Length */
	{ SA_HASH128_LEN, SA_HASH160_LEN, SA_HASH256_LEN, SA_HASH256_LEN,
	SA_HASH512_LEN, SA_HASH512_LEN }
};

/** Table lookup for Hash Algorithms based on Hash type, used in
 *  crypto4xx_pre_compute_hmac()
 */
char *crypto4xx_hash_alg_map_tbl[HASH_ALG_MAX_CNT] = CRYPTO4XX_MAC_ALGS;

static void crypto4xx_sg_setbuf(unsigned char *data, size_t bufsize,
				struct scatterlist *sg, int sg_num)
{
	int remainder_of_page;
	int i = 0;

	sg_init_table(sg, sg_num);
	while (bufsize > 0 && i < sg_num) {
		sg_set_buf(&sg[i], data, bufsize);
		remainder_of_page = PAGE_SIZE - sg[i].offset;
		if (bufsize > remainder_of_page) {
			/* the buffer was split over multiple pages */
			sg[i].length = remainder_of_page;
			bufsize -= remainder_of_page;
			data += remainder_of_page;
		} else {
			bufsize = 0;
		}
		i++;
	}
}

int crypto4xx_pre_compute_hmac(struct crypto4xx_ctx *ctx,
			       void *key,
			       unsigned int keylen,
			       unsigned int bs,
			       unsigned char ha,
			       unsigned char digs)
{
	u8 *ipad = NULL;
	u8 *opad;
	struct crypto_hash *child_hash = NULL;
	struct hash_desc desc;
	struct scatterlist sg[1];
	struct scatterlist asg[2];
	struct crypto_tfm *child_tfm;
	char *child_name = NULL;
	int i, rc = 0;
	int ds;

	BUG_ON(ha >= HASH_ALG_MAX_CNT);
	child_name = crypto4xx_hash_alg_map_tbl[ha];
	child_hash = crypto_alloc_hash(child_name, 0, 0);
	if (IS_ERR(child_hash)) {
		rc = PTR_ERR(child_hash);
		printk(KERN_ERR "failed to load "
				"transform for %s error %d\n",
				child_name, rc);
		return rc;
	}

	ipad =  kmalloc(bs * 2, GFP_KERNEL);
	if (ipad == NULL) {
		crypto_free_hash(child_hash);
		return -ENOMEM;
	}

	opad = ipad + bs;
	child_tfm = crypto_hash_tfm(child_hash);
	ds = crypto_hash_digestsize(child_hash);
	desc.tfm = child_hash;
	desc.flags = 0;
	if (keylen > bs) {
		crypto4xx_sg_setbuf(key, keylen, asg, 2);
		rc = crypto_hash_init(&desc);
		if (rc < 0)
			goto err_alg_hash_key;
		rc = crypto_hash_update(&desc, asg, keylen);
		if (rc < 0)
			goto err_alg_hash_key;
		rc = crypto_hash_final(&desc, ipad);
		keylen = ds;
	} else {
		memcpy(ipad, key, keylen);
	}
	memset(ipad + keylen, 0, bs-keylen);
	memcpy(opad, ipad, bs);

	for (i = 0; i < bs; i++) {
		ipad[i] ^= 0x36;
		opad[i] ^= 0x5c;
	}

	sg_init_one(&sg[0], ipad, bs);
	rc = crypto_hash_init(&desc);
	if (rc < 0)
		goto err_alg_hash_key;
	rc = crypto_hash_update(&desc, sg, bs);
	if (rc < 0)
		goto err_alg_hash_key;

	if (ha == SA_HASH_ALG_SHA224)
		ds = SHA256_DIGEST_SIZE;
	else if (ha == SA_HASH_ALG_SHA384)
		ds = SHA512_DIGEST_SIZE;

	crypto_hash_partial(&desc, ipad);
	crypto4xx_memcpy_le(ctx->sa_in +
			get_dynamic_sa_offset_inner_digest(ctx), ipad, ds);

	sg_init_one(&sg[0], opad, bs);
	rc = crypto_hash_init(&desc);
	if (rc < 0)
		goto err_alg_hash_key;

	rc = crypto_hash_update(&desc, sg, bs);
	if (rc < 0)
		goto err_alg_hash_key;

	crypto_hash_partial(&desc, opad);
	crypto4xx_memcpy_le(ctx->sa_in +
			get_dynamic_sa_offset_outer_digest(ctx), opad, ds);

err_alg_hash_key:
	kfree(ipad);
	crypto_free_hash(child_hash);
	return rc;
}

int crypto4xx_pre_compute_ssl_mac(struct crypto4xx_ctx *ctx, 
				  void *key,
				  unsigned int keylen,
				  unsigned int bs,
				  unsigned char ha)

{
	u8 *ipad;
	u8 *opad;
	struct crypto_hash *child_hash = NULL;
	struct hash_desc desc;
	struct scatterlist sg[1];
	struct crypto_tfm *child_tfm;
	unsigned char *digest = NULL;
	int padsize = 0;
	char *child_name = NULL;
	int i, rc = 0;
	int ds;

	digest = kmalloc(bs, GFP_KERNEL);
	if (digest == NULL) {
		rc = -ENOMEM;
		goto err_nomem;
	}

	if (ha == SA_HASH_ALG_MD5) {
		child_name = "md5";
		padsize = 48;
	} else if (ha == SA_HASH_ALG_SHA1) {
		child_name = "sha1";
		padsize = 40;
	}

	child_hash = crypto_alloc_hash(child_name, 0, 0);
	if (IS_ERR(child_hash)) {
		rc = PTR_ERR(child_hash);
		printk(KERN_ERR
				"failed to load transform for %s error %d\n",
		child_name, rc);
		goto err_alg;
	}

	child_tfm   = crypto_hash_tfm(child_hash);
	ds  	    = crypto_hash_digestsize(child_hash);
	desc.tfm    = child_hash;
	desc.flags  = 0;
	
	if (keylen > bs) {
		sg_init_one(&sg[0], key, keylen);
		rc = crypto_hash_init(&desc);
		if (rc < 0)
			goto err_alg_hash_key;
		rc = crypto_hash_update(&desc, &sg[0], keylen);
		if (rc < 0)
			goto err_alg_hash_key;
		rc = crypto_hash_final(&desc, digest);
		key    = digest;
		keylen = ds;
	}

	ipad = kmalloc(bs * 4, GFP_KERNEL);
	if (ipad == NULL)
		goto err_nomem;
	
	memcpy(ipad, key, keylen);
	memset(ipad + keylen, 0, bs);
	opad = ipad + bs;
	memcpy(opad, ipad, bs);
	
	for (i = 0; i < bs; i++) {
		ipad[i] ^= 0x36;
		opad[i] ^= 0x5c;
	}
	
	sg_init_one(&sg[0], ipad, bs);
	rc = crypto_hash_init(&desc);
	if (rc < 0)
		goto err_alg_hash_key;
	rc = crypto_hash_update(&desc, sg, bs);
	if (rc < 0)
		goto err_alg_hash_key;

	crypto_hash_partial(&desc, digest);
	memcpy(ctx->sa_in + get_dynamic_sa_offset_inner_digest(ctx), digest, ds);

	sg_init_one(&sg[0], opad, bs);
	rc = crypto_hash_init(&desc);
	if (rc < 0)
		goto err_alg_hash_key;
	rc = crypto_hash_update(&desc, sg, bs);
	if (rc < 0)
		goto err_alg_hash_key;

	crypto_hash_partial(&desc, digest);
	memcpy(ctx->sa_in + get_dynamic_sa_offset_outer_digest(ctx), digest, ds);
	
err_alg_hash_key:
		crypto_free_hash(child_hash);
err_alg:
		kfree(digest);
err_nomem:
		return rc;
}

int crypto4xx_compute_gcm_hash_key_sw(struct crypto4xx_ctx *ctx,
				      const u8 *key,
				      unsigned int keylen)
{
	struct crypto_blkcipher *aes_tfm = NULL;
	struct blkcipher_desc 	desc;
	struct scatterlist sg[1];
	char src[16];
	int rc = 0;

	aes_tfm = crypto_alloc_blkcipher("ecb(aes)", 0, CRYPTO_ALG_ASYNC);
	if (IS_ERR(aes_tfm)) {
		printk(KERN_ERR "failed to load transform for %ld\n",
		       PTR_ERR(aes_tfm));
		rc = PTR_ERR(aes_tfm);
		return rc;
	}
	desc.tfm    = aes_tfm;
	desc.flags  = 0;

	memset(src, 0, 16);
	rc = crypto_blkcipher_setkey(aes_tfm, key, keylen);
	if (rc) {
		printk(KERN_ERR "setkey() failed flags=%x\n",
		       crypto_blkcipher_get_flags(aes_tfm));
		goto out;
	}

	sg_init_one(sg, src, 16);
	rc = crypto_blkcipher_encrypt(&desc, sg, sg, 16);
	if (rc)
		goto out;
	crypto4xx_memcpy_le(ctx->sa_in +
			get_dynamic_sa_offset_inner_digest(ctx), src, 16);

out:
	crypto_free_blkcipher(aes_tfm);
	return rc;
}

/**
 * 3DES/DES Functions
 *
 */
static int crypto4xx_setkey_3des(struct crypto_ablkcipher *cipher,
				 const u8 *key,
				 unsigned int keylen,
				 unsigned char cm,
				 unsigned char fb)
{
	struct crypto_tfm	*tfm = crypto_ablkcipher_tfm(cipher);
	struct crypto4xx_ctx	*ctx = crypto_tfm_ctx(tfm);
	struct crypto_alg	*alg = tfm->__crt_alg;
	struct crypto4xx_alg	*my_alg = crypto_alg_to_crypto4xx_alg(alg);
	struct dynamic_sa_ctl	*sa;
	int rc;

	ctx->dev = my_alg->dev;

	if (keylen != DES_KEY_SIZE && keylen != DES3_EDE_KEY_SIZE) {
		crypto_ablkcipher_set_flags(cipher,
			CRYPTO_TFM_RES_BAD_KEY_LEN);

		return -EINVAL;
	}

	if (keylen == DES_KEY_SIZE) {
		u32 tmp[32];
		rc = des_ekey(tmp, key);
		if (unlikely(rc == 0) &&
				  (tfm->crt_flags & CRYPTO_TFM_REQ_WEAK_KEY)) {
			crypto_ablkcipher_set_flags(cipher,
				CRYPTO_TFM_RES_WEAK_KEY);
			return -EINVAL;
		}
	}

	/* Create SA */
	if (ctx->sa_in_dma_addr || ctx->sa_out_dma_addr)
		crypto4xx_free_sa(ctx);

	rc = crypto4xx_alloc_sa(ctx, keylen == 8 ? SA_DES_LEN : SA_3DES_LEN);
	if (rc)
		return rc;
	/*
	 *  state record will state in base ctx, so iv and
	 *  hash result can be reused
	 *  also don't need to alloc each packet coming
	 */
	if (ctx->state_record_dma_addr == 0) {
		rc = crypto4xx_alloc_state_record(ctx);
		if (rc) {
			crypto4xx_free_sa(ctx);
			return rc;
		}
	}

	/* Setup SA */
	ctx->direction = DIR_INBOUND;
	ctx->hash_final = 0;

	sa = (struct dynamic_sa_ctl *) ctx->sa_in;
	set_dynamic_sa_command_0(sa, SA_NOT_SAVE_HASH, SA_NOT_SAVE_IV,
				 SA_LOAD_HASH_FROM_SA, SA_LOAD_IV_FROM_STATE,
				 SA_NO_HEADER_PROC, SA_HASH_ALG_NULL,
				 SA_CIPHER_ALG_DES,
				 SA_PAD_TYPE_ZERO, SA_OP_GROUP_BASIC,
				 SA_OPCODE_DECRYPT, DIR_INBOUND);

	set_dynamic_sa_command_1(sa, cm, SA_HASH_MODE_HASH,
				 fb, SA_EXTENDED_SN_OFF,
				 SA_SEQ_MASK_OFF, SA_MC_ENABLE,
				 SA_NOT_COPY_PAD, SA_COPY_PAYLOAD,
				 SA_NOT_COPY_HDR);

	if (keylen == DES_KEY_SIZE) {
		crypto4xx_memcpy_le(((struct dynamic_sa_des *) sa)->key,
				      key, keylen);
		((struct dynamic_sa_des *)sa)->ctrl.sa_contents =
				SA_DES_CONTENTS;
		sa->sa_command_0.bf.cipher_alg = SA_CIPHER_ALG_DES;
	} else {
		crypto4xx_memcpy_le(((struct dynamic_sa_3des *) sa)->key,
				      key, keylen);
		((struct dynamic_sa_3des *)sa)->ctrl.sa_contents =
				SA_3DES_CONTENTS;
		sa->sa_command_0.bf.cipher_alg = SA_CIPHER_ALG_3DES;
	}

	memcpy((void *)(ctx->sa_in +
			get_dynamic_sa_offset_state_ptr_field(ctx)),
			(void *)&ctx->state_record_dma_addr, 4);
	ctx->offset_to_sr_ptr = get_dynamic_sa_offset_state_ptr_field(ctx);
	ctx->is_hash = 0;
	sa->sa_command_0.bf.dir = DIR_INBOUND;
	memcpy(ctx->sa_out, ctx->sa_in, ctx->sa_len * 4);
	sa = (struct dynamic_sa_ctl *) ctx->sa_out;
	sa->sa_command_0.bf.dir = DIR_OUTBOUND;

	return 0;
}

int crypto4xx_setkey_3des_cfb(struct crypto_ablkcipher *cipher,
			      const u8 *key, unsigned int keylen)
{
	return crypto4xx_setkey_3des(cipher, key, keylen,
					CRYPTO_MODE_CFB,
					CRYPTO_FEEDBACK_MODE_8BIT_CFB);
}

int crypto4xx_setkey_3des_ofb(struct crypto_ablkcipher *cipher,
			      const u8 *key, unsigned int keylen)
{
	return crypto4xx_setkey_3des(cipher, key, keylen,
				       CRYPTO_MODE_OFB,
				       CRYPTO_FEEDBACK_MODE_64BIT_OFB);
}

int crypto4xx_setkey_3des_cbc(struct crypto_ablkcipher *cipher,
			      const u8 *key, unsigned int keylen)
{
	return crypto4xx_setkey_3des(cipher, key, keylen,
				     CRYPTO_MODE_CBC,
				     CRYPTO_FEEDBACK_MODE_NO_FB);
}

int crypto4xx_setkey_3des_ecb(struct crypto_ablkcipher *cipher,
			      const u8 *key, unsigned int keylen)
{
	return crypto4xx_setkey_3des(cipher, key, keylen,
				     CRYPTO_MODE_ECB,
				     CRYPTO_FEEDBACK_MODE_NO_FB);
}


int crypto4xx_encrypt(struct ablkcipher_request *req)
{
	struct crypto4xx_ctx *ctx = crypto_tfm_ctx(req->base.tfm);

	ctx->direction = DIR_OUTBOUND;
	ctx->hash_final = 0;
	ctx->is_hash = 0;
	ctx->pd_ctl = 0x1;

	return crypto4xx_build_pd(&req->base, ctx, req->src, req->dst,
				   req->nbytes, NULL, 0, req->info,
				  get_dynamic_sa_iv_size(ctx));
}

int crypto4xx_decrypt(struct ablkcipher_request *req)
{
	struct crypto4xx_ctx *ctx = crypto_tfm_ctx(req->base.tfm);

	ctx->hash_final = 0;
	ctx->is_hash = 0;
	ctx->pd_ctl = 0x1;
	ctx->direction = DIR_INBOUND;

	return crypto4xx_build_pd(&req->base, ctx, req->src, req->dst,
				   req->nbytes, NULL, 0, req->info,
				  get_dynamic_sa_iv_size(ctx));
}

int crypto4xx_encrypt_ctr(struct ablkcipher_request *req)
{
	struct crypto_ablkcipher *ablkcipher = crypto_ablkcipher_reqtfm(req);
	struct crypto4xx_ctx *ctx = crypto_tfm_ctx(req->base.tfm);

	ctx->hash_final = 0;
	ctx->is_hash = 0;
	ctx->pd_ctl = 0x1;
	ctx->direction = DIR_OUTBOUND;

	return crypto4xx_build_pd(&req->base, ctx, req->src, req->dst,
				  req->nbytes, NULL, 0,
				  req->info,
				  crypto_ablkcipher_ivsize(ablkcipher));
}

int crypto4xx_decrypt_ctr(struct ablkcipher_request *req)
{
	struct crypto_ablkcipher *ablkcipher = crypto_ablkcipher_reqtfm(req);
	struct crypto4xx_ctx *ctx = crypto_tfm_ctx(req->base.tfm);

	ctx->hash_final = 0;
	ctx->is_hash = 0;
	ctx->pd_ctl = 0x1;
	ctx->direction = DIR_INBOUND;

	return crypto4xx_build_pd(&req->base, ctx, req->src, req->dst,
				  req->nbytes, NULL, 0,
				  req->info,
				  crypto_ablkcipher_ivsize(ablkcipher));
}

/**
 * AES Functions
 */
static int crypto4xx_setkey_aes(struct crypto_ablkcipher *cipher,
				const u8 *key,
				unsigned int keylen,
				unsigned char cm,
				u8 fb)
{
	struct crypto_tfm *tfm = crypto_ablkcipher_tfm(cipher);
	struct crypto4xx_ctx *ctx = crypto_tfm_ctx(tfm);
	struct crypto_alg 	*alg = tfm->__crt_alg;
	struct crypto4xx_alg 	*my_alg = crypto_alg_to_crypto4xx_alg(alg);
	struct dynamic_sa_ctl *sa;
	int    rc;

	ctx->dev = my_alg->dev;

	if (keylen != AES_KEYSIZE_256 &&
		keylen != AES_KEYSIZE_192 && keylen != AES_KEYSIZE_128) {
		crypto_ablkcipher_set_flags(cipher,
				CRYPTO_TFM_RES_BAD_KEY_LEN);
		return -EINVAL;
	}

	/* Create SA */
	if (ctx->sa_in_dma_addr || ctx->sa_out_dma_addr)
		crypto4xx_free_sa(ctx);

	rc = crypto4xx_alloc_sa(ctx, SA_AES128_LEN + (keylen-16) / 4);
	if (rc)
		return rc;

	if (ctx->state_record_dma_addr == 0) {
		rc = crypto4xx_alloc_state_record(ctx);
		if (rc) {
			crypto4xx_free_sa(ctx);
			return rc;
		}
	}
	/* Setup SA */
	sa = (struct dynamic_sa_ctl *) ctx->sa_in;
	ctx->hash_final = 0;

	set_dynamic_sa_command_0(sa, SA_NOT_SAVE_HASH, SA_NOT_SAVE_IV,
				 SA_LOAD_HASH_FROM_SA, SA_LOAD_IV_FROM_STATE,
				 SA_NO_HEADER_PROC, SA_HASH_ALG_NULL,
				 SA_CIPHER_ALG_AES, SA_PAD_TYPE_ZERO,
				 SA_OP_GROUP_BASIC, SA_OPCODE_DECRYPT,
				 DIR_INBOUND);

	set_dynamic_sa_command_1(sa, cm, SA_HASH_MODE_HASH,
				 fb, SA_EXTENDED_SN_OFF,
				 SA_SEQ_MASK_OFF, SA_MC_ENABLE,
				 SA_NOT_COPY_PAD, SA_NOT_COPY_PAYLOAD,
				 SA_NOT_COPY_HDR);
	crypto4xx_memcpy_le(ctx->sa_in + get_dynamic_sa_offset_key_field(ctx),
			    key, keylen);
	sa->sa_contents = SA_AES_CONTENTS | (keylen << 2);
	sa->sa_command_1.bf.key_len = keylen >> 3;
	ctx->is_hash = 0;
	ctx->direction = DIR_INBOUND;
	memcpy(ctx->sa_in + get_dynamic_sa_offset_state_ptr_field(ctx),
			(void *)&ctx->state_record_dma_addr, 4);
	ctx->offset_to_sr_ptr = get_dynamic_sa_offset_state_ptr_field(ctx);

	memcpy(ctx->sa_out, ctx->sa_in, ctx->sa_len * 4);
	sa = (struct dynamic_sa_ctl *) ctx->sa_out;
	sa->sa_command_0.bf.dir = DIR_OUTBOUND;

	return 0;
}

int crypto4xx_setkey_aes_ecb(struct crypto_ablkcipher *cipher,
			     const u8 *key, unsigned int keylen)
{
	return crypto4xx_setkey_aes(cipher, key, keylen, CRYPTO_MODE_ECB,
				    CRYPTO_FEEDBACK_MODE_NO_FB);
}

int crypto4xx_setkey_aes_cbc(struct crypto_ablkcipher *cipher,
			     const u8 *key, unsigned int keylen)
{
	return crypto4xx_setkey_aes(cipher, key, keylen, CRYPTO_MODE_CBC,
				    CRYPTO_FEEDBACK_MODE_NO_FB);
}

int crypto4xx_setkey_aes_ctr(struct crypto_ablkcipher *cipher,
			     const u8 *key, unsigned int keylen)
{
	struct crypto_tfm	*tfm = crypto_ablkcipher_tfm(cipher);
	struct crypto4xx_ctx	*ctx = crypto_tfm_ctx(tfm);
	struct crypto_alg *alg = tfm->__crt_alg;
	struct crypto4xx_alg *my_alg = crypto_alg_to_crypto4xx_alg(alg);
	struct dynamic_sa_ctl *sa;
	u32 cnt = 1;
	int rc;
	u32 cm = CRYPTO_MODE_AES_CTR;

	ctx->dev   = my_alg->dev;

	keylen -= 4;
	/* Create SA */
	if (ctx->sa_in_dma_addr || ctx->sa_out_dma_addr)
		crypto4xx_free_sa(ctx);

	if (keylen != AES_KEYSIZE_256 &&
		   keylen != AES_KEYSIZE_192 && keylen != AES_KEYSIZE_128) {
		crypto_ablkcipher_set_flags(cipher,
					    CRYPTO_TFM_RES_BAD_KEY_LEN);
		return -EINVAL;
	}

	rc = crypto4xx_alloc_sa(ctx, SA_AES128_LEN + (keylen-16) / 4);
	if (rc)
		return rc;

	if (ctx->state_record_dma_addr == 0) {
		rc = crypto4xx_alloc_state_record(ctx);
		if (rc) {
			crypto4xx_free_sa(ctx);
			return rc;
		}
	}

	sa = (struct dynamic_sa_ctl *) ctx->sa_in;
	ctx->hash_final = 0;
	ctx->ctr_aes = 1;
	/* Setup SA */
	set_dynamic_sa_command_0(sa, SA_NOT_SAVE_HASH, SA_NOT_SAVE_IV,
				 SA_LOAD_HASH_FROM_SA, SA_LOAD_IV_FROM_STATE,
				 SA_NO_HEADER_PROC, SA_HASH_ALG_NULL,
				 SA_CIPHER_ALG_AES, SA_PAD_TYPE_ZERO,
				 SA_OP_GROUP_BASIC, SA_OPCODE_ENCRYPT,
				 DIR_INBOUND);
	set_dynamic_sa_command_1(sa, cm, SA_HASH_MODE_HASH,
				 CRYPTO_FEEDBACK_MODE_NO_FB,
				 SA_EXTENDED_SN_OFF, SA_SEQ_MASK_OFF,
				 SA_MC_ENABLE, SA_NOT_COPY_PAD,
				 SA_NOT_COPY_PAYLOAD,
				 SA_NOT_COPY_HDR);

	crypto4xx_memcpy_le(ctx->sa_in + get_dynamic_sa_offset_key_field(ctx),
			    key, keylen);
	sa->sa_contents = SA_AES_CONTENTS | (keylen << 2);
	sa->sa_command_1.bf.key_len = keylen >> 3;

	ctx->direction = DIR_INBOUND;
	memcpy(ctx->sa_in + get_dynamic_sa_offset_state_ptr_field(ctx),
	       (void *)&ctx->state_record_dma_addr, 4);
	ctx->offset_to_sr_ptr = get_dynamic_sa_offset_state_ptr_field(ctx);

	crypto4xx_memcpy_le(ctx->state_record, key + keylen, 4);
	crypto4xx_memcpy_le(ctx->state_record + 12, (void *)&cnt, 4);

	sa->sa_command_0.bf.dir = DIR_INBOUND;

	memcpy(ctx->sa_out, ctx->sa_in, ctx->sa_len * 4);
	sa = (struct dynamic_sa_ctl *) ctx->sa_out;
	sa->sa_command_0.bf.dir = DIR_OUTBOUND;

	return 0;
}

int crypto4xx_setkey_aes_cfb(struct crypto_ablkcipher *cipher,
					  const u8 *key, unsigned int keylen)
{
	return crypto4xx_setkey_aes(cipher, key, keylen, CRYPTO_MODE_CFB,
				    CRYPTO_FEEDBACK_MODE_128BIT_CFB);
}

int crypto4xx_setkey_aes_ofb(struct crypto_ablkcipher *cipher,
					  const u8 *key, unsigned int keylen)
{
	return crypto4xx_setkey_aes(cipher, key, keylen, CRYPTO_MODE_OFB,
				    CRYPTO_FEEDBACK_MODE_64BIT_OFB);
}

int crypto4xx_setkey_aes_icm(struct crypto_ablkcipher *cipher,
					  const u8 *key, unsigned int keylen)
{
	return crypto4xx_setkey_aes(cipher, key, keylen, CRYPTO_MODE_AES_ICM,
				    CRYPTO_FEEDBACK_MODE_NO_FB);
}

/**
 * AES-GCM Functions
 */
static inline int crypto4xx_aes_gcm_validate_keylen(unsigned int keylen)
{
	switch (keylen) {
	case 16:
	case 20:
	case 24:
	case 30:
	case 32:
	case 36:
		return 0;
	default:
		printk(KERN_ERR "crypto4xx_setkey_aes_gcm: "
				"ERROR keylen = 0x%08x\n", keylen);
		return -EINVAL;
	}
	return -EINVAL;
}

int crypto4xx_setkey_aes_gcm(struct crypto_aead *cipher,
				     const u8 *key, unsigned int keylen)

{
	struct crypto_tfm	*tfm = crypto_aead_tfm(cipher);
	struct crypto4xx_ctx	*ctx = crypto_tfm_ctx(tfm);
	struct crypto_alg	*alg = tfm->__crt_alg;
	struct crypto4xx_alg	*my_alg = crypto_alg_to_crypto4xx_alg(alg);
	struct dynamic_sa_ctl	*sa;
	int    rc = 0;

	u32 cm = 4;

	ctx->dev = my_alg->dev;

	if (crypto4xx_aes_gcm_validate_keylen(keylen) != 0) {
		printk(KERN_ERR "crypto4xx_setkey_aes_gcm:"
				"ERROR keylen = 0x%08x\n", keylen);
		crypto_aead_set_flags(cipher, CRYPTO_TFM_RES_BAD_KEY_LEN);
	    return -EINVAL;
	}

	if (ctx->sa_in_dma_addr || ctx->sa_out_dma_addr)
		 crypto4xx_free_sa(ctx);

	rc = crypto4xx_alloc_sa(ctx, SA_AES128_GCM_LEN + (keylen-16) / 4);
	if (rc)
		return rc;

	if (ctx->state_record_dma_addr == 0) {
		rc = crypto4xx_alloc_state_record(ctx);
		if (rc)
			goto err;
	}

	sa  = (struct dynamic_sa_ctl *) ctx->sa_in;

	sa->sa_contents = SA_AES_GCM_CONTENTS | (keylen << 2);
	sa->sa_command_1.bf.key_len = keylen >> 3;

	ctx->direction = DIR_INBOUND;
	crypto4xx_memcpy_le(ctx->sa_in + get_dynamic_sa_offset_key_field(ctx),
			    key, keylen);

	memcpy(ctx->sa_in + get_dynamic_sa_offset_state_ptr_field(ctx),
	       (void *)&ctx->state_record_dma_addr, 4);

	rc = crypto4xx_compute_gcm_hash_key_sw(ctx, key, keylen);
	if (rc) {
		printk(KERN_ERR "GCM hash key setting failed = %d\n", rc);
		goto err;
	}

	ctx->offset_to_sr_ptr = get_dynamic_sa_offset_state_ptr_field(ctx);
	ctx->is_gcm = 1;
	ctx->hash_final = 1;
	ctx->is_hash = 0;
	ctx->pd_ctl = 0x11;

	set_dynamic_sa_command_0(sa, SA_SAVE_HASH, SA_NOT_SAVE_IV,
				 SA_LOAD_HASH_FROM_SA, SA_LOAD_IV_FROM_STATE,
				 SA_NO_HEADER_PROC, SA_HASH_ALG_GHASH,
				 SA_CIPHER_ALG_AES, SA_PAD_TYPE_ZERO,
				 SA_OP_GROUP_BASIC, SA_OPCODE_HASH_DECRYPT,
				 DIR_INBOUND);

	sa->sa_command_1.bf.crypto_mode31 = (cm & 4) >> 2;
	sa->sa_command_1.bf.crypto_mode9_8 = (cm & 3);
	sa->sa_command_1.bf.feedback_mode = 0;

	sa->sa_command_1.bf.hash_crypto_offset = 0;
	sa->sa_command_1.bf.sa_rev = 1;
	sa->sa_command_1.bf.copy_payload = 1;

	sa->sa_command_1.bf.copy_pad = 0;
	sa->sa_command_1.bf.copy_hdr = 0;
	sa->sa_command_1.bf.mutable_bit_proc = 1;
	sa->sa_command_1.bf.seq_num_mask = 1;

	memcpy(ctx->sa_out, ctx->sa_in, ctx->sa_len * 4);
	sa = (struct dynamic_sa_ctl *) ctx->sa_out;
	sa->sa_command_0.bf.dir = DIR_OUTBOUND;
	sa->sa_command_0.bf.opcode = SA_OPCODE_ENCRYPT_HASH;
	
	return 0;
err:
	crypto4xx_free_sa(ctx);
	return rc;
}

int crypto4xx_encrypt_aes_gcm(struct aead_request *req)
{
	struct crypto_aead *aead = crypto_aead_reqtfm(req);
	struct crypto4xx_ctx *ctx  = crypto_tfm_ctx(req->base.tfm);

	ctx->direction = DIR_OUTBOUND;
	ctx->append_icv = 1;

	return crypto4xx_build_pd(&req->base, ctx, req->src, req->dst,
				  req->cryptlen, req->assoc, req->assoclen,
				  req->iv, crypto_aead_ivsize(aead));
}

int crypto4xx_decrypt_aes_gcm(struct aead_request *req)
{
	struct crypto_aead *aead = crypto_aead_reqtfm(req);
	struct crypto4xx_ctx *ctx  = crypto_tfm_ctx(req->base.tfm);
	int len = req->cryptlen - crypto_aead_authsize(aead);

	ctx->direction = DIR_INBOUND;
	ctx->append_icv = 0;
	return crypto4xx_build_pd(&req->base, ctx, req->src, req->dst,
				  len, req->assoc, req->assoclen,
				  req->iv, crypto_aead_ivsize(aead));
}

int crypto4xx_givencrypt_aes_gcm(struct aead_givcrypt_request *req)
{
	return -ENOSYS;
}

int crypto4xx_givdecrypt_aes_gcm(struct aead_givcrypt_request *req)
{
	return -ENOSYS;
}

/**
 * AES-CCM Functions
 */
int crypto4xx_setauthsize_aes(struct crypto_aead *ciper,
			      unsigned int authsize)
{
	struct aead_tfm *tfm = crypto_aead_crt(ciper);

	switch (authsize) {
	case 8:
	case 12:
	case 16:
	case 10:
		break;
	default:
		return -EINVAL;
	}

	tfm->authsize = authsize;
	return 0;
}

int crypto4xx_setkey_aes_ccm(struct crypto_aead *cipher, const u8 *key,
				unsigned int keylen)
{
	struct crypto_tfm	*tfm = crypto_aead_tfm(cipher);
	struct crypto4xx_ctx	*ctx = crypto_tfm_ctx(tfm);
	struct crypto_alg	*alg = tfm->__crt_alg;
	struct crypto4xx_alg	*my_alg = crypto_alg_to_crypto4xx_alg(alg);
	struct dynamic_sa_ctl	*sa;
	int rc = 0;

	ctx->dev = my_alg->dev;

	if (ctx->sa_in_dma_addr || ctx->sa_out_dma_addr)
		 crypto4xx_free_sa(ctx);

	rc = crypto4xx_alloc_sa(ctx, SA_AES128_CCM_LEN + (keylen-16) / 4);
	if (rc)
		return rc;

	if (ctx->state_record_dma_addr == 0) {
		rc = crypto4xx_alloc_state_record(ctx);
		if (rc) {
			crypto4xx_free_sa(ctx);
			return rc;
		}
	}

	/* Setup SA */
	sa  = (struct dynamic_sa_ctl *) ctx->sa_in;
	sa->sa_contents = SA_AES_CCM_CONTENTS | (keylen << 2);

	set_dynamic_sa_command_0(sa, SA_NOT_SAVE_HASH, SA_NOT_SAVE_IV,
				 SA_LOAD_HASH_FROM_SA, SA_LOAD_IV_FROM_STATE,
				 SA_NO_HEADER_PROC, SA_HASH_ALG_CBC_MAC,
				 SA_CIPHER_ALG_AES,
				 SA_PAD_TYPE_ZERO, SA_OP_GROUP_BASIC,
				 SA_OPCODE_HASH_DECRYPT, DIR_INBOUND);

	sa->sa_command_0.bf.digest_len = 0;
	sa->sa_command_1.bf.key_len = keylen >> 3;
	ctx->direction = DIR_INBOUND;
	ctx->append_icv = 0;
	ctx->is_gcm = 0;
	ctx->hash_final = 1;
	ctx->is_hash = 0;
	ctx->pd_ctl = 0x11;

	crypto4xx_memcpy_le(ctx->sa_in + get_dynamic_sa_offset_key_field(ctx),
			    key, keylen);
	memcpy(ctx->sa_in + get_dynamic_sa_offset_state_ptr_field(ctx),
	       (void *)&ctx->state_record_dma_addr, 4);
	ctx->offset_to_sr_ptr = get_dynamic_sa_offset_state_ptr_field(ctx);

	set_dynamic_sa_command_1(sa, CRYPTO_MODE_AES_CTR, SA_HASH_MODE_HASH,
				 CRYPTO_FEEDBACK_MODE_NO_FB, SA_EXTENDED_SN_OFF,
				 SA_SEQ_MASK_OFF, SA_MC_ENABLE,
				 SA_NOT_COPY_PAD, SA_COPY_PAYLOAD,
				 SA_NOT_COPY_HDR);

	memcpy(ctx->sa_out, ctx->sa_in, ctx->sa_len * 4);
	sa = (struct dynamic_sa_ctl *) ctx->sa_out;
	set_dynamic_sa_command_0(sa, SA_SAVE_HASH, SA_NOT_SAVE_IV,
				 SA_LOAD_HASH_FROM_SA, SA_LOAD_IV_FROM_STATE,
				 SA_NO_HEADER_PROC, SA_HASH_ALG_CBC_MAC,
				 SA_CIPHER_ALG_AES,
				 SA_PAD_TYPE_ZERO, SA_OP_GROUP_BASIC,
				 SA_OPCODE_ENCRYPT_HASH, DIR_OUTBOUND);
	set_dynamic_sa_command_1(sa, CRYPTO_MODE_AES_CTR, SA_HASH_MODE_HASH,
				 CRYPTO_FEEDBACK_MODE_NO_FB, SA_EXTENDED_SN_OFF,
				 SA_SEQ_MASK_OFF, SA_MC_ENABLE,
				 SA_NOT_COPY_PAD, SA_COPY_PAYLOAD,
				 SA_NOT_COPY_HDR);

	return 0;
}

int crypto4xx_encrypt_aes_ccm(struct aead_request *req)
{
	struct crypto4xx_ctx *ctx  = crypto_tfm_ctx(req->base.tfm);
	struct crypto_aead *aead = crypto_aead_reqtfm(req);
	struct dynamic_sa_ctl *sa;

	ctx->direction = DIR_OUTBOUND;

	sa = (struct dynamic_sa_ctl *) ctx->sa_out;
	if (req->assoclen)
		sa->sa_command_1.bf.hash_crypto_offset = req->assoclen >> 2;

	sa->sa_command_0.bf.digest_len = (crypto_aead_authsize(aead) >> 2);
	if ((req->iv[0] & 7) == 1)
		sa->sa_command_1.bf.crypto_mode9_8 = 1;

	ctx->append_icv = 1;
	return crypto4xx_build_pd(&req->base, ctx, req->src, req->dst,
				  req->cryptlen, req->assoc, req->assoclen,
				  req->iv, 16);
}

int crypto4xx_decrypt_aes_ccm(struct aead_request *req)
{
	struct crypto4xx_ctx   *ctx  = crypto_tfm_ctx(req->base.tfm);
	struct crypto_aead *aead = crypto_aead_reqtfm(req);
	struct dynamic_sa_ctl *sa;

	/* Support only counter field length of 2 and 4 bytes */
	if ((req->iv[0] & 0x7) != 1 && (req->iv[0] & 0x7) != 3) {
		printk(KERN_ERR "algorithm AES-CCM "
				"unsupported counter length %d\n",
			req->iv[0] & 0x7);
		return -EINVAL;
	}

	ctx->direction = DIR_INBOUND;
	sa = (struct dynamic_sa_ctl *) ctx->sa_in;

	sa->sa_command_0.bf.digest_len = (crypto_aead_authsize(aead) >> 2);
	if ((req->iv[0] & 7) == 1)
		sa->sa_command_1.bf.crypto_mode9_8 = 1;
	else
		sa->sa_command_1.bf.crypto_mode9_8 = 0;

	return crypto4xx_build_pd(&req->base, ctx, req->src, req->dst,
				  req->cryptlen, req->assoc, req->assoclen,
				  req->iv, 16);
}

int crypto4xx_givencrypt_aes_ccm(struct aead_givcrypt_request *req)
{
	return -ENOSYS;
}

int crypto4xx_givdecrypt_aes_ccm(struct aead_givcrypt_request *req)
{
	return -ENOSYS;
}

/**
 * Kasumi Functions
 *
 */
int crypto4xx_setkey_kasumi(struct crypto_ablkcipher *cipher,
				   const u8 *key,
				   unsigned int keylen,
				   unsigned char cm)
{
	struct crypto_tfm	*tfm = crypto_ablkcipher_tfm(cipher);
	struct crypto4xx_ctx	*ctx = crypto_tfm_ctx(tfm);
	struct crypto_alg	*alg = tfm->__crt_alg;
	struct crypto4xx_alg	*my_alg = crypto_alg_to_crypto4xx_alg(alg);
	struct dynamic_sa_ctl	*sa;
	u32 sa_len = 0;
	int rc;

	if (keylen != 16) {
		crypto_ablkcipher_set_flags(cipher, CRYPTO_TFM_RES_BAD_KEY_LEN);
		printk(KERN_ERR "%s: keylen fail\n", __func__);
		return -EINVAL;
	}

	ctx->dev = my_alg->dev;

	/* Create SA - SA is created here as the alg init function is
	 * common to many algorithm and it does not have the SA length
	 * as it is specify to an algorithm. See setkey function has
	 * to be called for encryption/decryption algorithm once,
	 * it is okay to do this here.
	 */
	if (ctx->sa_in_dma_addr || ctx->sa_out_dma_addr)
		 crypto4xx_free_sa(ctx);

	if (cm == CRYPTO_MODE_KASUMI)
		 sa_len = SA_KASUMI_LEN;
	else if (cm == CRYPTO_MODE_KASUMI_f8)
		 sa_len = SA_KASUMI_F8_LEN;

	rc = crypto4xx_alloc_sa(ctx, sa_len);
	if (rc)
		return rc;

	if (!ctx->state_record) {
		rc = crypto4xx_alloc_state_record(ctx);
		if (rc) {
			crypto4xx_free_sa(ctx);
			return rc;
		}
	}

	sa = (struct dynamic_sa_ctl *) ctx->sa_in;
	/* Setup SA - SA is a shared resource for request operation. As
	 * crypto alg and crypto mode can not be change, it should be
	 * ok to store them there. SA control words are not used by the
	 * hardware (configured in token instead), we use it to store
	 * software algorithm and mode selected.
	 */

	if (cm == CRYPTO_MODE_KASUMI) {
		sa->sa_contents = SA_KASUMI_CONTENTS;
		sa->sa_command_0.bf.cipher_alg = SA_CIPHER_ALG_KASUMI;
		sa->sa_command_0.bf.hash_alg = SA_HASH_ALG_NULL;
		sa->sa_command_0.bf.pad_type = 3; /* set to zero padding */
		sa->sa_command_0.bf.opcode = 0;
		sa->sa_command_1.bf.crypto_mode31 = (cm & 4) >> 2;
		sa->sa_command_1.bf.crypto_mode9_8 = (cm & 3);
		sa->sa_command_1.bf.feedback_mode = 0;
	} else {
		sa->sa_contents = SA_KASUMI_F8_CONTENTS;
		sa->sa_command_0.bf.cipher_alg = SA_CIPHER_ALG_KASUMI;
		sa->sa_command_0.bf.hash_alg = SA_HASH_ALG_NULL;
		sa->sa_command_0.bf.pad_type = 3;
		sa->sa_command_0.bf.load_iv = SA_LOAD_IV_FROM_STATE;
		sa->sa_command_0.bf.opcode = SA_OPCODE_ENCRYPT;
		sa->sa_command_1.bf.crypto_mode31 = (cm & 4) >> 2;;
		sa->sa_command_1.bf.crypto_mode9_8 = (cm & 3);
		sa->sa_command_1.bf.feedback_mode = 0;
		sa->sa_command_1.bf.mutable_bit_proc = 1;
	}

	ctx->direction = DIR_INBOUND;
	sa->sa_command_1.bf.sa_rev = 1;
	crypto4xx_memcpy_le(ctx->sa_in + get_dynamic_sa_offset_key_field(ctx),
			    key, keylen);
	ctx->is_hash = 0;

	memcpy(ctx->sa_in + get_dynamic_sa_offset_state_ptr_field(ctx),
	       (void *)&ctx->state_record_dma_addr, 4);
	ctx->offset_to_sr_ptr = get_dynamic_sa_offset_state_ptr_field(ctx);
	sa->sa_command_0.bf.dir = DIR_INBOUND;

	memcpy(ctx->sa_out, ctx->sa_in, ctx->sa_len * 4);
	sa = (struct dynamic_sa_ctl *) ctx->sa_out;
	sa->sa_command_0.bf.dir = DIR_OUTBOUND;

	return 0;
}

int crypto4xx_setkey_kasumi_p(struct crypto_ablkcipher *cipher,
					const u8 *key,
					unsigned int keylen)
{
	return crypto4xx_setkey_kasumi(cipher, key, keylen,
				       CRYPTO_MODE_KASUMI);
}

int crypto4xx_setkey_kasumi_f8(struct crypto_ablkcipher *cipher,
					     const u8 *key,
					     unsigned int keylen)
{
	return crypto4xx_setkey_kasumi(cipher, key, keylen,
				       CRYPTO_MODE_KASUMI_f8);
}

/**
 * Kasumi and Kasumi f8 work with number of bits.
 * The crypto engine can only take number bytes as source/destination length
 * User should round up bit number to byte number. When receive the result
 * packet and then mask off the extra bits in the last
 * byte.
 */
int crypto4xx_encrypt_kasumi(struct ablkcipher_request *req)
{
	struct crypto4xx_ctx *ctx  = crypto_tfm_ctx(req->base.tfm);
	ctx->direction = DIR_OUTBOUND;
	ctx->pd_ctl = 0x1;

	return crypto4xx_build_pd(&req->base, ctx, req->src, req->dst,
				  req->nbytes, NULL, 0, NULL, 0);
}

/**
 * Kasumi and Kasumi f8 work with number of bits.
 * The crypto engine can only take number bytes as source/destination length
 * User should round up bit number to byte number.
 * When receive the result packet and then mask off the extra bits in the last
 * byte.
 */
int crypto4xx_decrypt_kasumi(struct aead_request *req)
{
	struct crypto4xx_ctx *ctx  = crypto_tfm_ctx(req->base.tfm);

	ctx->pd_ctl = 0x1;
	ctx->direction = DIR_INBOUND;

	return crypto4xx_build_pd(&req->base, ctx, req->src, req->dst,
				  req->cryptlen, NULL, 0, NULL, 0);
}

/**
 * Kasumi and Kasumi f8 work with number of bits.
 * The crypto engine can only take number bytes as source/destination length
 * The user should round up bit number to byte number.
 * When receive the result packet and then mask
 * off the extra bits in the last byte.
 */
int crypto4xx_encrypt_kasumi_f8(struct ablkcipher_request *req)
{
	struct crypto4xx_ctx *ctx  = crypto_tfm_ctx(req->base.tfm);

	ctx->direction = DIR_OUTBOUND;
	ctx->is_hash = 0;
	ctx->pd_ctl = 0x1;

	return crypto4xx_build_pd(&req->base, ctx, req->src, req->dst,
				  req->nbytes, NULL, 0, req->info, 8);
}

/** Note:
 * Kasumi and Kasumi f8 work with number of bits.
 * The crypto engine can only take number bytes as source/destination length
 *  User should round up bit number to byte number.
 * When receive the result packet and then mask off the extra bits in the last
 * byte.
 */
int crypto4xx_decrypt_kasumi_f8(struct ablkcipher_request *req)
{
	struct crypto4xx_ctx *ctx  = crypto_tfm_ctx(req->base.tfm);

	ctx->direction = DIR_INBOUND;
	ctx->is_hash = 0;
	ctx->pd_ctl = 0x1;

	return crypto4xx_build_pd(&req->base, ctx, req->src, req->dst,
				  req->nbytes, NULL, 0, req->info, 8);
}

/**
 * ARC4 Functions
 *
 */
int crypto4xx_setkey_arc4(struct crypto_ablkcipher *cipher,
				 const u8 *key, unsigned int keylen)
{
	struct crypto_tfm	*tfm = crypto_ablkcipher_tfm(cipher);
	struct crypto4xx_ctx	*ctx = crypto_tfm_ctx(tfm);
	struct crypto_alg	*alg = tfm->__crt_alg;
	struct crypto4xx_alg	*my_alg = crypto_alg_to_crypto4xx_alg(alg);
	struct dynamic_sa_ctl	*sa = (struct dynamic_sa_ctl *) ctx->sa_in;
	int rc = 0;

	ctx->dev = my_alg->dev;

	/* Create SA */
	if (ctx->sa_in_dma_addr || ctx->sa_out_dma_addr)
		crypto4xx_free_sa(ctx);

	rc = crypto4xx_alloc_sa(ctx, SA_ARC4_LEN);
	if (rc)
		return rc;

#if 0
	crypto4xx_alloc_arc4_state_record(ctx);
		if (ctx->arc4_state_record == NULL) {
			crypto4xx_free_sa(ctx);
			return -ENOMEM;
	}
#endif
	if (ctx->arc4_state_record == NULL) {
		rc = crypto4xx_alloc_arc4_state_record(ctx);
		if (rc) {
			crypto4xx_free_sa(ctx);
			return -ENOMEM;
		}
	}
	/* Setup SA */
	ctx->sa_len = SA_ARC4_LEN;
	ctx->init_arc4 = 1;
	ctx->direction = DIR_INBOUND;

	sa = ctx->sa_in;
	memset(((struct dynamic_sa_arc4 *)sa)->key, 0, 16);

	crypto4xx_memcpy_le(((struct dynamic_sa_arc4 *)sa)->key, key, keylen);
	sa->sa_contents = SA_ARC4_CONTENTS;

	set_dynamic_sa_command_0(sa, SA_NOT_SAVE_HASH, SA_NOT_SAVE_IV,
				 SA_LOAD_HASH_FROM_SA, SA_LOAD_IV_FROM_STATE,
				 SA_NO_HEADER_PROC, SA_HASH_ALG_NULL,
				 SA_CIPHER_ALG_ARC4, SA_PAD_TYPE_ZERO,
				 SA_OP_GROUP_BASIC, SA_OPCODE_ENCRYPT,
				 DIR_INBOUND);

	set_dynamic_sa_command_1(sa, 0, SA_HASH_MODE_HASH,
				 CRYPTO_FEEDBACK_MODE_NO_FB,
				 SA_EXTENDED_SN_OFF, SA_SEQ_MASK_OFF,
				 SA_MC_ENABLE, SA_NOT_COPY_PAD,
				 SA_COPY_PAYLOAD, SA_NOT_COPY_HDR);

	sa->sa_command_1.bf.key_len = keylen;
	memcpy(sa + get_dynamic_sa_offset_arc4_state_ptr(ctx),
	       (void *)&ctx->arc4_state_record_dma_addr, 4);

	memcpy(ctx->sa_out, ctx->sa_in, ctx->sa_len * 4);
	sa = (struct dynamic_sa_ctl *) ctx->sa_out;
	sa->sa_command_0.bf.dir = DIR_OUTBOUND;


	return 0;
}

int crypto4xx_arc4_encrypt(struct ablkcipher_request *req)
{
	struct crypto4xx_ctx *ctx  = crypto_tfm_ctx(req->base.tfm);

	if (ctx->init_arc4) {
		ctx->init_arc4 = 0;
		ctx->pd_ctl = 9;
	} else {
		ctx->pd_ctl = 0x1;
	}

	return crypto4xx_build_pd(&req->base, ctx, req->src,
				  req->dst,
				  req->nbytes, NULL, 0, NULL, 0);
}

int crypto4xx_arc4_decrypt(struct ablkcipher_request *req)
{
	struct crypto4xx_ctx *ctx  = crypto_tfm_ctx(req->base.tfm);

	if (ctx->init_arc4) {
		ctx->init_arc4 = 0;
		ctx->pd_ctl = 9;
	} else {
		ctx->pd_ctl = 0x1;
	}

	return crypto4xx_build_pd(&req->base, ctx, req->src,
				  req->dst,
				  req->nbytes, NULL, 0, NULL, 0);
}

/**
 * Support MD5/SHA/HMAC Hashing Algorithms
 *
 */
static int crypto4xx_hash_alg_init(struct crypto_tfm *tfm,
				   unsigned int sa_len,
				   unsigned char ha,
				   unsigned char hm)
{
	struct crypto_alg	*alg = tfm->__crt_alg;
	struct crypto4xx_alg	*my_alg = crypto_alg_to_crypto4xx_alg(alg);
	struct crypto4xx_ctx	*ctx = crypto_tfm_ctx(tfm);
	struct dynamic_sa_ctl	*sa;
	int rc;

	ctx->dev   = my_alg->dev;
	ctx->is_hash = 1;
	ctx->hash_final = 0;

	/* Create SA */
	if (ctx->sa_in_dma_addr || ctx->sa_out_dma_addr)
		crypto4xx_free_sa(ctx);

	rc = crypto4xx_alloc_sa(ctx, sa_len);
	if (rc)
		return rc;

	
	if (ctx->state_record_dma_addr == 0) {
		crypto4xx_alloc_state_record(ctx);
		if (!ctx->state_record_dma_addr) {
			crypto4xx_free_sa(ctx);
			return -ENOMEM;
		}
	}
	
	crypto_ahash_set_reqsize(__crypto_ahash_cast(tfm),
				 sizeof(struct crypto4xx_ctx));
	sa = (struct dynamic_sa_ctl *) ctx->sa_in;

	
	/*
	 * Setup hash algorithm and hash mode
	 */
	set_dynamic_sa_command_0(sa, SA_SAVE_HASH, SA_NOT_SAVE_IV,
				 SA_NOT_LOAD_HASH, SA_LOAD_IV_FROM_SA,
				 SA_NO_HEADER_PROC, ha, SA_CIPHER_ALG_NULL,
				 SA_PAD_TYPE_ZERO, SA_OP_GROUP_BASIC,
				 SA_OPCODE_HASH, DIR_INBOUND);
	set_dynamic_sa_command_1(sa, 0, SA_HASH_MODE_HASH,
				 CRYPTO_FEEDBACK_MODE_NO_FB, SA_EXTENDED_SN_OFF,
				 SA_SEQ_MASK_OFF, SA_MC_ENABLE,
				 SA_NOT_COPY_PAD, SA_NOT_COPY_PAYLOAD,
				 SA_NOT_COPY_HDR);

	BUG_ON(ha >= HASH_ALG_MAX_CNT);
	sa->sa_contents = crypto4xx_sa_hash_tbl[0][ha];
	ctx->direction = DIR_INBOUND;
	memcpy(ctx->sa_in + get_dynamic_sa_offset_state_ptr_field(ctx),
		(void *)&ctx->state_record_dma_addr, 4);
	ctx->offset_to_sr_ptr = get_dynamic_sa_offset_state_ptr_field(ctx);

	return 0;
}

int crypto4xx_hash_init(struct ahash_request *req)
{
	struct crypto4xx_ctx *ctx = crypto_tfm_ctx(req->base.tfm);
	int ds;
	struct dynamic_sa_ctl *sa;

	sa = (struct dynamic_sa_ctl *) ctx->sa_in;
	ds = crypto_ahash_digestsize(
			__crypto_ahash_cast(req->base.tfm));
	sa->sa_command_0.bf.digest_len = ds >> 2;
	sa->sa_command_0.bf.load_hash_state = SA_LOAD_HASH_FROM_SA;
	ctx->is_hash = 1;
	ctx->direction = DIR_INBOUND;

	return 0;
}

int crypto4xx_hash_update(struct ahash_request *req)
{
	struct crypto4xx_ctx *ctx = crypto_tfm_ctx(req->base.tfm);

	ctx->is_hash = 1;
	ctx->hash_final = 0;
	ctx->pd_ctl = 0x11;
	ctx->direction = DIR_INBOUND;

	return crypto4xx_build_pd(&req->base, ctx, req->src,
				  (struct scatterlist *) req->result,
				  req->nbytes, NULL, 0, NULL, 0);
}

int crypto4xx_hash_final(struct ahash_request *req)
{
	return 0;
}

int crypto4xx_hash_digest(struct ahash_request *req)
{
	struct crypto4xx_ctx *ctx = crypto_tfm_ctx(req->base.tfm);

	ctx->hash_final = 1;
	ctx->pd_ctl = 0x11;
	ctx->direction = DIR_INBOUND;

	return crypto4xx_build_pd(&req->base, ctx, req->src,
				  (struct scatterlist *) req->result,
				  req->nbytes, NULL, 0, NULL, 0);
}

/**
 * SHA1 Algorithm
 */

int crypto4xx_md5_alg_init(struct crypto_tfm *tfm)
{
	return crypto4xx_hash_alg_init(tfm, SA_HASH128_LEN, SA_HASH_ALG_MD5,
				       SA_HASH_MODE_HASH);
}

int crypto4xx_hash_hmac_setkey(struct crypto_ahash *hash,
				      const u8 *key,
				      unsigned int keylen,
				      unsigned int sa_len,
				      unsigned char ha,
				      unsigned char hm,
				      unsigned int max_keylen)
{
	struct crypto_tfm	*tfm = crypto_ahash_tfm(hash);
	struct crypto_alg	*alg	 = tfm->__crt_alg;
	struct crypto4xx_alg	*my_alg   = crypto_alg_to_crypto4xx_alg(alg);
	struct crypto4xx_ctx	*ctx = crypto_tfm_ctx(tfm);
	struct dynamic_sa_ctl 	*sa;
	int bs 	= crypto_tfm_alg_blocksize(tfm);
	int ds 	= crypto_ahash_digestsize(hash);
	int rc;

	ctx->dev = my_alg->dev;

	if (keylen > max_keylen) {
		crypto_ahash_set_flags(hash, CRYPTO_TFM_RES_BAD_KEY_LEN);
		return -1;
	}

	if (ctx->sa_in_dma_addr || ctx->sa_out_dma_addr)
		crypto4xx_free_sa(ctx);

	/* Create SA */
	rc = crypto4xx_alloc_sa(ctx, sa_len);
	if (rc)
		return rc;

	if (ctx->state_record_dma_addr == 0) {
		rc = crypto4xx_alloc_state_record(ctx);
		if (rc)
			goto err;
	}

	sa = (struct dynamic_sa_ctl *) ctx->sa_in;

	/*
	 * Setup hash algorithm and hash mode
	 */
	set_dynamic_sa_command_0(sa, SA_SAVE_HASH, SA_NOT_SAVE_IV,
				 SA_NOT_LOAD_HASH, SA_LOAD_IV_FROM_SA,
				 SA_NO_HEADER_PROC,
				 ha, SA_CIPHER_ALG_NULL, SA_PAD_TYPE_ZERO,
				 SA_OP_GROUP_BASIC, SA_OPCODE_HASH,
				 DIR_INBOUND);
	set_dynamic_sa_command_1(sa, 0, hm,
				 CRYPTO_FEEDBACK_MODE_NO_FB,
				 SA_EXTENDED_SN_OFF,
				 SA_SEQ_MASK_OFF, SA_MC_ENABLE,
				 SA_NOT_COPY_PAD, SA_NOT_COPY_PAYLOAD,
				 SA_NOT_COPY_HDR);

	BUG_ON(ha >= HASH_ALG_MAX_CNT);
	sa->sa_contents = crypto4xx_sa_hash_tbl[0][ha];
	ctx->direction = DIR_INBOUND;
	memcpy((ctx->sa_in) + get_dynamic_sa_offset_state_ptr_field(ctx),
	       (void *)&ctx->state_record_dma_addr, 4);

	ctx->offset_to_sr_ptr = get_dynamic_sa_offset_state_ptr_field(ctx);
	rc = crypto4xx_pre_compute_hmac(ctx, (void *)key, keylen, bs, ha, ds);
	if (rc) {
		printk(KERN_ERR "Hmac Initial Digest Calculation failed\n");
		goto err;
	}

	ctx->hash_final = 1;
	ctx->is_hash = 1;

	memcpy(ctx->sa_out, ctx->sa_in, ctx->sa_len * 4);
	sa = (struct dynamic_sa_ctl *) ctx->sa_out;
	sa->sa_command_0.bf.dir = DIR_OUTBOUND;

	return 0;
err:
	crypto4xx_free_sa(ctx);
	return rc;
}

int crypto4xx_md5_hmac_setkey(struct crypto_ahash *hash, const u8 *key,
			      unsigned int keylen)
{
	return crypto4xx_hash_hmac_setkey(hash, key, keylen, SA_HASH128_LEN,
					  SA_HASH_ALG_MD5, SA_HASH_MODE_HMAC,
					  256);
}

/**
 * SHA1 and SHA2 Algorithm
 *
 */

int crypto4xx_sha1_alg_init(struct crypto_tfm *tfm)
{
	return crypto4xx_hash_alg_init(tfm, SA_HASH160_LEN, SA_HASH_ALG_SHA1,
				       SA_HASH_MODE_HASH);
}

int crypto4xx_sha1_hmac_setkey(struct crypto_ahash *hash, const u8 *key,
			       unsigned int keylen)
{
	return crypto4xx_hash_hmac_setkey(hash, key, keylen, SA_HASH160_LEN,
					  SA_HASH_ALG_SHA1, SA_HASH_MODE_HMAC,
					  256);
}

int crypto4xx_sha2_alg_init(struct crypto_tfm *tfm)
{
	int ds = crypto_ahash_digestsize(__crypto_ahash_cast(tfm));
	u8 ha;

	switch (ds) {
	default:
	case 256/8:
		ha = SA_HASH_ALG_SHA256;
		break;
	case 224/8:
		ha = SA_HASH_ALG_SHA224;
		break;
	case 512/8:
		ha = SA_HASH_ALG_SHA512;
		break;
	case 384/8:
		ha = SA_HASH_ALG_SHA384;
		break;
	}
	BUG_ON(ha >= HASH_ALG_MAX_CNT);

	return crypto4xx_hash_alg_init(tfm,
				       crypto4xx_sa_hash_tbl[2][ha], ha, 0);
}

int crypto4xx_sha2_hmac_setkey(struct crypto_ahash *hash,
				      const u8 *key,
				      unsigned int keylen)
{
	int ds = crypto_ahash_digestsize(hash);
	unsigned char ha;

	switch (ds) {
	default:
	case 256/8:
		ha = SA_HASH_ALG_SHA256;
		break;
	case 224/8:
		ha = SA_HASH_ALG_SHA224;
		break;
	case 512/8:
		ha = SA_HASH_ALG_SHA512;
		break;
	case 384/8:
		ha = SA_HASH_ALG_SHA384;
		break;
	}
	BUG_ON(ha >= HASH_ALG_MAX_CNT);

	return crypto4xx_hash_hmac_setkey(hash, key, keylen,
					  crypto4xx_sa_hash_tbl[2][ha],
					  ha,
					  SA_HASH_MODE_HMAC,
					  512);
}

/**
 * AES-XCBC-MAC Algorithm
 *
 */
int crypto4xx_xcbc_digest(const unsigned char *key,
			  unsigned int keylen,
			  u8 *sa_hash, int bs)
{
	struct scatterlist 	sg[1];
	struct crypto_blkcipher *aes_tfm = NULL;
	struct blkcipher_desc 	desc;
	int rc;
	u8 *digest;

	/* Load pre-computed key value into SA */
	aes_tfm = crypto_alloc_blkcipher("ecb(aes)", 0, CRYPTO_ALG_ASYNC);
	if (IS_ERR(aes_tfm)) {
		rc = PTR_ERR(aes_tfm);
		printk(KERN_ERR "failed to load transform"
				" for ecb(aes) error %d\n", rc);
		goto err_alg;
	}
	desc.tfm    = aes_tfm;
	desc.flags  = 0;
	rc = crypto_blkcipher_setkey(desc.tfm, key, keylen);
	if (rc) {
		printk(KERN_ERR  "failed to load key error %d\n", rc);
		goto err_alg;
	}
	digest =  kmalloc(16, GFP_KERNEL);
	if (digest == NULL) {
		rc = -ENOMEM;
		goto err_alg;
	}

	memset(digest, 0x01, bs);
	sg_init_one(&sg[0], digest, bs);
	rc = crypto_blkcipher_encrypt(&desc, sg, sg, bs);
	if (rc < 0) {
		printk(KERN_ERR "failed to hash key error %d\n", rc);
		goto err_alg;
	}

	crypto4xx_memcpy_le((void *) sa_hash, digest, bs);

	memset(digest, 0x02, bs);
	sg_init_one(&sg[0], digest, bs);
	rc = crypto_blkcipher_encrypt(&desc, sg, sg, bs);
	if (rc < 0) {
		printk(KERN_ERR "failed to hash key error %d\n", rc);
		goto err_alg;
	}

	sa_hash += 32;
	crypto4xx_memcpy_le((void *) sa_hash, digest, bs);

	memset(digest, 0x03, bs);
	sg_init_one(&sg[0], digest, bs);
	rc = crypto_blkcipher_encrypt(&desc, sg, sg, bs);
	if (rc < 0) {
		printk(KERN_ERR "failed to hash key error %d\n", rc);
		goto err_alg;
	}

	sa_hash += 16;
	crypto4xx_memcpy_le((void *) sa_hash, digest, bs);

	crypto_free_blkcipher(aes_tfm);

	return 0;
err_alg:
#if 0
	if (aes_tfm)
		crypto_free_blkcipher(aes_tfm);
#endif
	return rc;
}

int crypto4xx_xcbc_setkey(struct crypto_ahash *hash,
				      const u8 *key,
				      unsigned int keylen)
{
	struct crypto_tfm    	*tfm = crypto_ahash_tfm(hash);
	struct crypto4xx_ctx 	*ctx = crypto_tfm_ctx(tfm);
        struct crypto_alg	*alg     = tfm->__crt_alg;
        struct crypto4xx_alg	*my_alg   = crypto_alg_to_crypto4xx_alg(alg);
	int bs = crypto_tfm_alg_blocksize(tfm);
	struct dynamic_sa_ctl 	*sa;
	u8 *sa_hash;
	int rc = 0;

	ctx->dev = my_alg->dev;

	if (keylen != 128/8) {
		crypto_ahash_set_flags(hash, CRYPTO_TFM_RES_BAD_KEY_LEN);
		return -EINVAL;
	}

	if (ctx->sa_in_dma_addr || ctx->sa_out_dma_addr)
		 crypto4xx_free_sa(ctx);
	
	/* Create SA */
	rc = crypto4xx_alloc_sa(ctx, SA_AES128_XCBC_MAC_LEN);
	if (rc)
		return rc;

	if (ctx->state_record_dma_addr == 0) {
		rc = crypto4xx_alloc_state_record(ctx);
		if (rc) {
			rc = -ENOMEM;
			goto err;
		}
	}

	ctx->direction = DIR_INBOUND;
	sa = (struct dynamic_sa_ctl *) ctx->sa_in;
	/*
	 * Setup hash algorithm and hash mode
	 */
	sa->sa_contents = SA_AES128_XCBC_MAC_CONTENTS;
	set_dynamic_sa_command_0(sa, SA_SAVE_HASH, SA_NOT_SAVE_IV,
				 SA_NOT_LOAD_HASH, SA_LOAD_IV_FROM_SA,
				 SA_NO_HEADER_PROC,
				 SA_HASH_ALG_AES_XCBC_MAC_128,
				 SA_CIPHER_ALG_NULL, SA_PAD_TYPE_ZERO,
				 SA_OP_GROUP_BASIC, SA_OPCODE_HASH,
				 DIR_INBOUND);
	set_dynamic_sa_command_1(sa, 0, SA_HASH_MODE_HASH,
				 CRYPTO_FEEDBACK_MODE_NO_FB,
				 SA_EXTENDED_SN_OFF,
				 SA_SEQ_MASK_OFF, SA_MC_ENABLE,
				 SA_NOT_COPY_PAD, SA_NOT_COPY_PAYLOAD,
				 SA_NOT_COPY_HDR);
	crypto4xx_memcpy_le(ctx->sa_in + get_dynamic_sa_offset_key_field(ctx),
			    key, keylen);

	memcpy((void *)(ctx->sa_in +
			get_dynamic_sa_offset_state_ptr_field(ctx)),
		(void *)&ctx->state_record_dma_addr, 4);
	ctx->is_hash = 1;
	ctx->offset_to_sr_ptr = get_dynamic_sa_offset_state_ptr_field(ctx);
	sa_hash = (u8 *)(&(((struct dynamic_sa_aes128_xcbc_mac *)
			ctx->sa_in)->inner_digest));
	rc = crypto4xx_xcbc_digest(key, keylen, sa_hash, bs);
	if (rc) {
		printk(KERN_ERR "XCBC Digest Calculation Failed %d\n", rc);
		goto err;
	}

	ctx->is_hash = 1;
	ctx->hash_final = 1;
	ctx->pd_ctl = 0x11;

	ctx->direction = DIR_INBOUND;

	memcpy(ctx->sa_out, ctx->sa_in, ctx->sa_len * 4);
	sa = (struct dynamic_sa_ctl *) ctx->sa_out;
	sa->sa_command_0.bf.dir = DIR_OUTBOUND;

	return 0;
err:
	crypto4xx_free_sa(ctx);
	return rc;
}

/**
 * Kasumi F9 - Hash Algorithms
 *
 */
int crypto4xx_kasumi_f9_setkey(struct crypto_ahash *hash,
			       const u8 *key, unsigned int keylen)
{
	struct crypto_tfm	*tfm = crypto_ahash_tfm(hash);
	struct crypto4xx_ctx	*ctx = crypto_tfm_ctx(tfm);
        struct crypto_alg	*alg     = tfm->__crt_alg;
        struct crypto4xx_alg	*my_alg   = crypto_alg_to_crypto4xx_alg(alg);
	struct dynamic_sa_ctl	*sa;
	int 	rc;

	ctx->dev = my_alg->dev;

	if (keylen != 16) {
		crypto_ahash_set_flags(hash, CRYPTO_TFM_RES_BAD_KEY_LEN);
		return -EINVAL;
	}

	/* Create SA */
	if (ctx->sa_in_dma_addr || ctx->sa_out_dma_addr)
		 crypto4xx_free_sa(ctx);

	rc = crypto4xx_alloc_sa(ctx, SA_KASUMI_F9_LEN);
	if (rc)
		return rc;

	if (ctx->state_record_dma_addr == 0) {
		rc = crypto4xx_alloc_state_record(ctx);
		if (rc) {
			crypto4xx_free_sa(ctx);
			return rc;
		}
	}

	sa = (struct dynamic_sa_ctl *) ctx->sa_in;
	/*
	 * Setup hash algorithm and hash mode
	 */
	set_dynamic_sa_command_0(sa, SA_SAVE_HASH, SA_NOT_SAVE_IV,
				 SA_NOT_LOAD_HASH, SA_LOAD_IV_FROM_SA,
				 SA_NO_HEADER_PROC, SA_HASH_ALG_KASUMI_f9,
				 SA_CIPHER_ALG_NULL, SA_PAD_TYPE_ZERO,
				 SA_OP_GROUP_BASIC, SA_OPCODE_HASH,
				 DIR_INBOUND);
	set_dynamic_sa_command_1(sa, 0, SA_HASH_MODE_HASH,
				 CRYPTO_FEEDBACK_MODE_NO_FB, SA_EXTENDED_SN_OFF,
				 SA_SEQ_MASK_OFF, SA_MC_ENABLE,
				 SA_NOT_COPY_PAD, SA_NOT_COPY_PAYLOAD,
				 SA_NOT_COPY_HDR);
	sa->sa_contents = SA_KASUMI_F9_CONTENTS;

	ctx->direction = DIR_INBOUND;
	memcpy((void *)(ctx->sa_in +
			get_dynamic_sa_offset_state_ptr_field(ctx)),
			(void *)&ctx->state_record_dma_addr, 4);

	crypto4xx_memcpy_le(ctx->sa_in +
			get_dynamic_sa_offset_inner_digest(ctx), key, keylen);
	ctx->offset_to_sr_ptr = get_dynamic_sa_offset_state_ptr_field(ctx);
	ctx->is_hash = 1;
	ctx->hash_final = 1;
	ctx->pd_ctl = 0x11;
	ctx->bypass = 4;

	return 0;
}

int crypto4xx_kasumi_f9_digest(struct ahash_request *req)
{
	struct crypto4xx_ctx *ctx = crypto_tfm_ctx(req->base.tfm);
	struct scatterlist *src = req->src;
	struct dynamic_sa_ctl *sa;
	dma_addr_t addr;

	/*
	 * We have prepended count/fresh/direction/reserv total
	 * 16byte before the plaintext
	 * so, need to modify the length.
	 * We doing so, to make use of tcrypt.c's hash_test.
	 */
	sa = (struct dynamic_sa_ctl *) ctx->sa_in;

	addr = dma_map_page(NULL, sg_page(src), src->offset,
			    src->length, DMA_TO_DEVICE);
	crypto4xx_memcpy_le((void *)sa +
			get_dynamic_sa_offset_outer_digest(ctx),
			phys_to_virt(addr), 12);

	return crypto4xx_build_pd(&req->base, ctx, req->src,
				  (struct scatterlist *)req->result,
				  req->nbytes, NULL, 0, NULL, 0);
}
/** IPSEC Related Routines */

int crypto4xx_setkey_esp_tunnel(struct crypto_aead *cipher,
				const u8 *key,
				unsigned int keylen,
				u32 cipher_alg,
				u32 hashing,
				u32 c_mode,
				u32 sa_len,
				u32 sa_contents,
				u32 ds,
				u32 bypass,
				u32 hash_bs)

{
	struct crypto_tfm	*tfm = crypto_aead_tfm(cipher);
	struct crypto4xx_ctx	*ctx = crypto_tfm_ctx(tfm);
	struct crypto_alg	*alg = tfm->__crt_alg;
	struct crypto4xx_alg	*my_alg = crypto_alg_to_crypto4xx_alg(alg);
	struct rtattr		*rta = (void *) key;
	struct dynamic_sa_ctl	*sa;

	struct esp_authenc_param {
		__be32 spi;
		__be32 seq;
		__be16 pad_block_size;
		__be16 encap_uhl;
		struct crypto_authenc_key_param authenc_param;
	} *param;

	unsigned int enckeylen;
	unsigned int authkeylen;

	if (!RTA_OK(rta, keylen))
		goto badkey;

	if (rta->rta_type != CRYPTO_AUTHENC_KEYA_PARAM)
		goto badkey;

	if (RTA_PAYLOAD(rta) < sizeof(*param))
		goto badkey;

	ctx->dev  = my_alg->dev;

	param     = RTA_DATA(rta);

	ctx->spi = be32_to_cpu(param->spi);
	ctx->seq = be32_to_cpu(param->seq);
	ctx->pad_block_size = be16_to_cpu(param->pad_block_size);
	ctx->encap_uhl   = be16_to_cpu(param->encap_uhl);

	ESP_PRINTK(KERN_INFO    "%s: spi = 0x%08x, seq = %d, pad_size = %d, encap uhl = %d\n",__FUNCTION__,
		   ctx->spi, ctx->seq, ctx->pad_block_size, ctx->encap_uhl);

	enckeylen = be32_to_cpu(param->authenc_param.enckeylen);

	key    += RTA_ALIGN(rta->rta_len);
	keylen -= RTA_ALIGN(rta->rta_len);

	authkeylen = keylen - enckeylen;


	printk(KERN_INFO "%s: enckeylen = %d, authkeylen = %d\n",
		   __FUNCTION__, enckeylen, authkeylen);
#if 0
	ESP_PHD(KERN_CONT, "", DUMP_PREFIX_OFFSET,
		16, 1,
		(void*)key, authkeylen, false);
	
	ESP_PHD(KERN_CONT, "", DUMP_PREFIX_OFFSET,
		16, 1,
		(void*)key+authkeylen, enckeylen, false);
#endif
	/* Create SA */
	if (ctx->sa_in_dma_addr || ctx->sa_out_dma_addr) {
		crypto4xx_free_sa(ctx);
	}

	crypto4xx_alloc_sa(ctx, sa_len);
	if (!ctx->sa_in_dma_addr || !ctx->sa_out_dma_addr)
		goto err_nomem;

	if (!ctx->state_record) {
		crypto4xx_alloc_state_record(ctx);
		if (!ctx->state_record_dma_addr)
			goto err_nomem_sr;
	}

	ctx->direction = DIR_INBOUND;
	sa = (struct dynamic_sa_ctl *)(ctx->sa_in);
	/* 
	* Setup hash algorithm and hash mode
	*/
	sa->sa_command_0.w = 0;
	sa->sa_command_0.bf.hash_alg = hashing;
	sa->sa_command_0.bf.gather = 0;
	sa->sa_command_0.bf.save_hash_state = 1;
	sa->sa_command_0.bf.load_hash_state = 0;
	sa->sa_command_0.bf.cipher_alg = SA_CIPHER_ALG_NULL;
	sa->sa_command_0.bf.opcode = SA_OPCODE_HASH;
	sa->sa_command_0.bf.dir = DIR_INBOUND;
	sa->sa_command_1.w              = 0;
	sa->sa_command_1.bf.hmac_muting = 0;
	sa->sa_command_1.bf.sa_rev      = 1;
	sa->sa_contents = sa_contents;

	memcpy(ctx->sa_in + get_dynamic_sa_offset_state_ptr_field(ctx),
	       (void*)&(ctx->state_record_dma_addr),  4);

	crypto4xx_pre_compute_hmac(ctx, (void *)key, authkeylen,
				   hash_bs, hashing, ds);

	/*
	* Now, setup command for ESP
	*/
	sa->sa_command_0.bf.load_hash_state = 0;
	sa->sa_command_0.bf.save_hash_state = 0;
	sa->sa_command_0.bf.hdr_proc = 1;

	sa->sa_command_0.bf.load_iv = 2;
	sa->sa_command_0.bf.cipher_alg = cipher_alg;
	sa->sa_command_0.bf.op_group = SA_OP_GROUP_PROTOCOL;
	sa->sa_command_0.bf.opcode = SA_OPCODE_ESP;

	sa->sa_command_1.bf.hmac_muting = 0;

	
	printk(KERN_INFO "%s: keylen = %d, enckeylen = %d\n",
		   __FUNCTION__, keylen, enckeylen);
	
	if (cipher_alg == SA_CIPHER_ALG_AES) {
		if ( enckeylen == 16)
			sa->sa_command_1.bf.key_len = SA_AES_KEY_LEN_128;
		else if ( enckeylen == 24)
			sa->sa_command_1.bf.key_len = SA_AES_KEY_LEN_192;
		else
			sa->sa_command_1.bf.key_len = SA_AES_KEY_LEN_256;
	} else {
		sa->sa_command_1.bf.key_len = 0;
	}

	sa->sa_command_1.bf.crypto_mode31 = c_mode >> 2;
	sa->sa_command_1.bf.crypto_mode9_8 = c_mode & 3;
	sa->sa_command_1.bf.feedback_mode = 0;
	sa->sa_command_1.bf.copy_payload = 1;
	sa->sa_command_1.bf.copy_pad = 1;
	sa->sa_command_1.bf.copy_hdr = 1;

	sa->sa_command_1.bf.seq_num_mask = 1;
	sa->sa_command_1.bf.mutable_bit_proc = 0;

	sa->sa_command_0.bf.hdr_proc = 1;

	crypto4xx_memcpy_le(ctx->sa_in + get_dynamic_sa_offset_key_field(ctx),
			    key+authkeylen, enckeylen);

	memcpy(ctx->sa_in + get_dynamic_sa_offset_spi(ctx),
	       (void*)&(ctx->spi), 4);
	memcpy(ctx->sa_in + get_dynamic_sa_offset_seq_num(ctx),
	       (void*)&(ctx->seq), 4);

	/* 
	* Setup sa for inbound processing
	*/
	sa->sa_command_0.bf.dir = DIR_INBOUND;
	sa->sa_command_0.bf.load_iv  = 1;
	sa->sa_command_0.bf.hdr_proc = 1;

	sa->sa_command_1.bf.copy_pad = 1;
	sa->sa_command_1.bf.copy_hdr = 0;
	sa->sa_command_1.bf.hash_crypto_offset = 6;


	/* 
	* Setup sa for outbound processing
	*/
	memcpy(ctx->sa_out, ctx->sa_in, ctx->sa_len*4);
	sa = (struct dynamic_sa_ctl *)(ctx->sa_out);
	sa->sa_command_0.bf.dir = DIR_OUTBOUND;
	sa->sa_command_0.bf.load_iv = 3;
	sa->sa_command_0.bf.hdr_proc = 1;

	sa->sa_command_1.bf.hash_crypto_offset = 0;
	sa->sa_command_1.bf.copy_pad = 1;
	sa->sa_command_1.bf.copy_hdr = 1;

	ctx->bypass = bypass;
	ctx->authenc = 0;
	ctx->hash_final = 1;
	ctx->is_hash = 0;
	ctx->pad_ctl = param->pad_block_size/4;
	ctx->append_icv = 0;

	return 0;

err_nomem_sr:
		crypto4xx_free_sa(ctx);

err_nomem:
		return -ENOMEM;
badkey:
		ESP_PRINTK(KERN_INFO   KERN_ERR "%s: badkey\n",__FUNCTION__);
	return -EINVAL;
}

int crypto4xx_setkey_tunnel_esp_cbc_aes_md5(struct crypto_aead *cipher,
					    const u8 *key,
					    unsigned int keylen)
{
	ESP_PRINTK(KERN_INFO "%s: called\n", __FUNCTION__);
	return crypto4xx_setkey_esp_tunnel(cipher, key, keylen,
					   SA_CIPHER_ALG_AES,
					   SA_HASH_ALG_MD5, 1,
					   SA_ESP_MD5_SHA1_LEN,
					   SA_ESP_MD5_SHA1_CONTENTS, 16, 0, 64);
}

int crypto4xx_setkey_tunnel_esp_cbc_aes_sha1(struct crypto_aead *cipher,
					     const u8 *key,
					     unsigned int keylen)
{
	ESP_PRINTK(KERN_INFO "%s: called\n", __FUNCTION__);
	return crypto4xx_setkey_esp_tunnel(cipher, key, keylen,
					   SA_CIPHER_ALG_AES,
					   SA_HASH_ALG_SHA1, 1,
					   SA_ESP_MD5_SHA1_LEN,
					   SA_ESP_MD5_SHA1_CONTENTS, 20, 0, 64);
}

int crypto4xx_setkey_tunnel_esp_cbc_aes_sha224(struct crypto_aead *cipher,
					       const u8 *key,
					       unsigned int keylen)
{
	ESP_PRINTK(KERN_INFO "%s: called\n", __FUNCTION__);
	return crypto4xx_setkey_esp_tunnel(cipher, key, keylen,
					   SA_CIPHER_ALG_AES,
					   SA_HASH_ALG_SHA224, 1,
					   SA_ESP_SHA256_LEN,
					   SA_ESP_SHA256_CONTENTS, 28, 0, 64);
}

int crypto4xx_setkey_tunnel_esp_cbc_aes_sha256(struct crypto_aead *cipher,
					       const u8 *key,
					       unsigned int keylen)
{
	ESP_PRINTK(KERN_INFO "%s: called\n", __FUNCTION__);
	return crypto4xx_setkey_esp_tunnel(cipher, key, keylen,
					   SA_CIPHER_ALG_AES,
					   SA_HASH_ALG_SHA256, 1,
					   SA_ESP_SHA256_LEN,
					   SA_ESP_SHA256_CONTENTS, 32, 0, 64);
}

int crypto4xx_setkey_tunnel_esp_cbc_aes_sha384(struct crypto_aead *cipher,
					       const u8 *key,
					       unsigned int keylen)
{
	ESP_PRINTK(KERN_INFO "%s: called\n", __FUNCTION__);
	return crypto4xx_setkey_esp_tunnel(cipher, key, keylen,
					   SA_CIPHER_ALG_AES,
					   SA_HASH_ALG_SHA384, 1,
					   SA_ESP_SHA512_LEN,
					   SA_ESP_SHA512_CONTENTS, 48, 0, 128);
}

int crypto4xx_setkey_tunnel_esp_cbc_aes_sha512(struct crypto_aead *cipher,
					       const u8 *key,
					       unsigned int keylen)
{
	ESP_PRINTK(KERN_INFO "%s: called\n", __FUNCTION__);
	return crypto4xx_setkey_esp_tunnel(cipher, key, keylen,
					   SA_CIPHER_ALG_AES,
					   SA_HASH_ALG_SHA512, 1,
					   SA_ESP_SHA512_LEN,
					   SA_ESP_SHA512_CONTENTS, 64, 0, 128);
}
/** DES and 3DES Related IPSEC Algorithms */
int crypto4xx_setkey_tunnel_esp_cbc_des_md5(struct crypto_aead *cipher,
					    const u8 *key,
					    unsigned int keylen)
{
	ESP_PRINTK(KERN_INFO "%s: called\n", __FUNCTION__);
	return crypto4xx_setkey_esp_tunnel(cipher, key, keylen,
					   SA_CIPHER_ALG_DES,
					   SA_HASH_ALG_MD5, 1,
					   SA_ESP_DES_MD5_SHA1_LEN,
					   SA_ESP_DES_MD5_SHA1_CONTENTS,
					   16, 0, 64);
}

int crypto4xx_setkey_tunnel_esp_cbc_des_sha1(struct crypto_aead *cipher,
					     const u8 *key,
					     unsigned int keylen)
{
	ESP_PRINTK(KERN_INFO "%s: called\n", __FUNCTION__);
	return crypto4xx_setkey_esp_tunnel(cipher, key, keylen,
					   SA_CIPHER_ALG_DES,
					   SA_HASH_ALG_SHA1, 1,
					   SA_ESP_DES_MD5_SHA1_LEN,
					   SA_ESP_DES_MD5_SHA1_CONTENTS, 20, 0,
					   64);

}

int crypto4xx_setkey_tunnel_esp_cbc_des_sha224(struct crypto_aead *cipher,
					       const u8 *key,
					       unsigned int keylen)
{
	ESP_PRINTK(KERN_INFO "%s: called\n", __FUNCTION__);
	return crypto4xx_setkey_esp_tunnel(cipher, key, keylen,
					   SA_CIPHER_ALG_DES,
					   SA_HASH_ALG_SHA224, 1,
					   SA_ESP_SHA256_LEN,
					   SA_ESP_SHA256_CONTENTS,
					   28, 0, 64);
}

int crypto4xx_setkey_tunnel_esp_cbc_des_sha256(struct crypto_aead *cipher,
					       const u8 *key,
					       unsigned int keylen)
{
	ESP_PRINTK(KERN_INFO "%s: called\n", __FUNCTION__);
	return crypto4xx_setkey_esp_tunnel(cipher, key, keylen,
					   SA_CIPHER_ALG_DES,
					   SA_HASH_ALG_SHA256, 1,
					   SA_ESP_SHA256_LEN,
					   SA_ESP_SHA256_CONTENTS, 32,
					   0, 64);
}

int crypto4xx_setkey_tunnel_esp_cbc_des_sha384(struct crypto_aead *cipher,
					       const u8 *key,
					       unsigned int keylen)
{
	ESP_PRINTK(KERN_INFO "%s: called\n", __FUNCTION__);
	return crypto4xx_setkey_esp_tunnel(cipher, key, keylen,
					   SA_CIPHER_ALG_DES,
					   SA_HASH_ALG_SHA384, 1,
					   SA_ESP_SHA512_LEN,
					   SA_ESP_SHA512_CONTENTS,
					   48, 0, 128);
}

int crypto4xx_setkey_tunnel_esp_cbc_des_sha512(struct crypto_aead *cipher,
					       const u8 *key,
					       unsigned int keylen)
{
	ESP_PRINTK(KERN_INFO "%s: called\n", __FUNCTION__);
	return crypto4xx_setkey_esp_tunnel(cipher, key, keylen,
					   SA_CIPHER_ALG_DES,
					   SA_HASH_ALG_SHA512, 1,
					   SA_ESP_SHA512_LEN,
					   SA_ESP_SHA512_CONTENTS,
					   64, 0, 128);
}

int crypto4xx_setkey_tunnel_esp_cbc_3des_md5(struct crypto_aead *cipher,
					     const u8 *key,
					     unsigned int keylen)
{
	ESP_PRINTK(KERN_INFO "%s: called\n", __FUNCTION__);
	return crypto4xx_setkey_esp_tunnel(cipher, key, keylen,
					   SA_CIPHER_ALG_3DES,
					   SA_HASH_ALG_MD5, 1,
					   SA_ESP_3DES_MD5_SHA1_LEN,
					   SA_ESP_3DES_MD5_SHA1_CONTENTS,
					   16, 0, 64);
}
int crypto4xx_setkey_tunnel_esp_cbc_3des_sha1(struct crypto_aead *cipher,
		const u8 *key,
		unsigned int keylen)
{
	ESP_PRINTK(KERN_INFO "%s: called\n", __FUNCTION__);
	return crypto4xx_setkey_esp_tunnel(cipher, key, keylen,
					   SA_CIPHER_ALG_3DES,
					   SA_HASH_ALG_SHA1, 1,
					   SA_ESP_3DES_MD5_SHA1_LEN,
					   SA_ESP_3DES_MD5_SHA1_CONTENTS, 20,
					   0,
					   64);
}

int crypto4xx_setkey_tunnel_esp_cbc_3des_sha224(struct crypto_aead *cipher,
		const u8 *key,
		unsigned int keylen)
{
	ESP_PRINTK(KERN_INFO "%s: called\n", __FUNCTION__);
	return crypto4xx_setkey_esp_tunnel(cipher, key, keylen,
					   SA_CIPHER_ALG_3DES,
					   SA_HASH_ALG_SHA224, 1,
					   SA_ESP_SHA256_LEN,
					   SA_ESP_SHA256_CONTENTS,
					   28, 0, 64);
}

int crypto4xx_setkey_tunnel_esp_cbc_3des_sha256(struct crypto_aead *cipher,
		const u8 *key,
		unsigned int keylen)
{
	ESP_PRINTK(KERN_INFO "%s: called\n", __FUNCTION__);
	return crypto4xx_setkey_esp_tunnel(cipher, key, keylen,
					   SA_CIPHER_ALG_3DES,
					   SA_HASH_ALG_SHA256, 1,
					   SA_ESP_SHA256_LEN,
					   SA_ESP_SHA256_CONTENTS,
					   32, 0, 64);
}


int crypto4xx_setkey_tunnel_esp_cbc_3des_sha384(struct crypto_aead *cipher,
		const u8 *key,
		unsigned int keylen)
{
	ESP_PRINTK(KERN_INFO "%s: called\n", __FUNCTION__);
	return crypto4xx_setkey_esp_tunnel(cipher, key, keylen,
					   SA_CIPHER_ALG_3DES,
					   SA_HASH_ALG_SHA384,
					   1,
					   SA_ESP_SHA512_LEN,
					   SA_ESP_SHA512_CONTENTS,
					   48, 0, 128);
}

int crypto4xx_setkey_tunnel_esp_cbc_3des_sha512(struct crypto_aead *cipher,
		const u8 *key,
		unsigned int keylen)
{
	ESP_PRINTK(KERN_INFO "%s: called\n", __FUNCTION__);
	return crypto4xx_setkey_esp_tunnel(cipher, key, keylen,
				    SA_CIPHER_ALG_3DES,
				    SA_HASH_ALG_SHA512, 1,
				    SA_ESP_SHA512_LEN,
				    SA_ESP_SHA512_CONTENTS,
				    64, 0, 128);
}

int crypto4xx_encrypt_esp_tunnel(struct aead_givcrypt_request *givreq,
				 struct aead_request *req)
{

        struct crypto4xx_ctx *ctx;  
#if 0
	struct scatterlist *sg;
	struct iphdr *iph;
	void * daddr;
	struct dynamic_sa_ctl *sa;
#endif
	if (givreq)
	        req = &givreq->areq;

	ctx = crypto_tfm_ctx(req->base.tfm);

#if 0
	sg = req->dst;
	daddr = kmap_atomic(sg_page(sg), KM_SOFTIRQ1);
	iph = (struct iphdr *)(daddr + sg->offset - 20);
	ctx->next_hdr = (u32)(iph->protocol);
	kunmap_atomic(daddr, KM_SOFTIRQ1);


	ctx->next_hdr = 4;
	ctx->pd_ctl =( ctx->pad_ctl  << 24) + 0x11 + (ctx->next_hdr << 8); 
#endif
	ctx->hc_offset = 0;
	ctx->pd_ctl =( ctx->pad_ctl  << 24) + 0x411; 
	ctx->direction = DIR_OUTBOUND;

	return crypto4xx_build_pd(&req->base, ctx, req->src, req->dst,
				  req->cryptlen, NULL, 0, NULL, 0); 


}

int crypto4xx_decrypt_esp_tunnel(struct aead_request *req)
{
	struct crypto4xx_ctx *ctx  = crypto_tfm_ctx(req->base.tfm);

	ctx->pd_ctl =( ctx->pad_ctl << 24) + 0x11; 
	ctx->direction = DIR_INBOUND;
	ctx->hc_offset = 6;
	return crypto4xx_build_pd(&req->base, ctx, req->src, req->dst,
				  req->cryptlen, NULL, 0, NULL, 0); 

}
int crypto4xx_encrypt_esp_cbc(struct aead_request *req)
{
	return crypto4xx_encrypt_esp_tunnel(NULL, req);
}

int crypto4xx_givencrypt_esp_cbc(struct aead_givcrypt_request *req)
{
	return crypto4xx_encrypt_esp_tunnel(req, NULL);
}

int crypto4xx_decrypt_esp_cbc(struct aead_request *req)
{
	return crypto4xx_decrypt_esp_tunnel(req);
}

/** Setkey Routine for IPSEC for Transport */
int crypto4xx_setkey_esp(struct crypto_aead *cipher,
			 const u8 *key, unsigned int keylen, u32 cipher_alg,
			 u32 hashing, u32 c_mode, u32 sa_len,
			 u32 sa_contents, u32 ds,
			 u32 bypass, u32 hash_bs)

{
	struct crypto_tfm	*tfm = crypto_aead_tfm(cipher);
	struct crypto4xx_ctx	*ctx = crypto_tfm_ctx(tfm);
	struct crypto_alg	*alg = tfm->__crt_alg;
	struct crypto4xx_alg	*my_alg = crypto_alg_to_crypto4xx_alg(alg);
	struct rtattr		*rta = (void *) key;
	struct dynamic_sa_ctl	*sa;

	struct esp_authenc_param {
		__be32 spi;
		__be32 seq;
		__be16 pad_block_size;
		__be16 encap_uhl;
		struct crypto_authenc_key_param authenc_param;
	} *param;

	unsigned int enckeylen;
	unsigned int authkeylen;

	if (!RTA_OK(rta, keylen))
		goto badkey;
	if (rta->rta_type != CRYPTO_AUTHENC_KEYA_PARAM)
		goto badkey;

	if (RTA_PAYLOAD(rta) < sizeof(*param))
		goto badkey;

	ctx->dev  = my_alg->dev;

	param     = RTA_DATA(rta);

	ctx->spi = be32_to_cpu(param->spi);
	ctx->seq = be32_to_cpu(param->seq);
	ctx->pad_block_size = be16_to_cpu(param->pad_block_size);
	ctx->encap_uhl   = be16_to_cpu(param->encap_uhl);

	ESP_PRINTK(KERN_INFO "%s: spi = 0x%08x, seq = %d, pad_size = %d, encap uhl = %d\n",__FUNCTION__,
		   ctx->spi, ctx->seq, ctx->pad_block_size, ctx->encap_uhl);

	enckeylen = be32_to_cpu(param->authenc_param.enckeylen);

	key    += RTA_ALIGN(rta->rta_len);
	keylen -= RTA_ALIGN(rta->rta_len);

	authkeylen = keylen - enckeylen;

	ESP_PRINTK(KERN_INFO "%s: enckeylen = %d, authkeylen = %d\n",
		   __FUNCTION__, enckeylen, authkeylen);

	ESP_PHD(KERN_CONT, "", DUMP_PREFIX_OFFSET,
		16, 1,
		(void*)key, authkeylen, false);
	
	ESP_PHD(KERN_CONT, "", DUMP_PREFIX_OFFSET,
		16, 1,
		(void*)key+authkeylen, enckeylen, false);

	/* Create SA */
	if (ctx->sa_in_dma_addr || ctx->sa_out_dma_addr) {
		crypto4xx_free_sa(ctx);
	}

	crypto4xx_alloc_sa(ctx, sa_len);
	if (!ctx->sa_in_dma_addr || !ctx->sa_out_dma_addr)
		goto err_nomem;

	if (!ctx->state_record) {
		crypto4xx_alloc_state_record(ctx);
		if (!ctx->state_record_dma_addr)
			goto err_nomem_sr;
	}
	
	sa = (struct dynamic_sa_ctl *) ctx->sa_in;
	/* 
	* Setup hash algorithm and hash mode
	*/
	sa->sa_command_0.w = 0;
	sa->sa_command_0.bf.hash_alg = hashing;
	sa->sa_command_0.bf.gather = 0;
	sa->sa_command_0.bf.save_hash_state = 1;
	sa->sa_command_0.bf.load_hash_state = 0;
	sa->sa_command_0.bf.cipher_alg = SA_CIPHER_ALG_NULL;
	sa->sa_command_0.bf.opcode = SA_OPCODE_HASH;
	sa->sa_command_0.bf.dir = DIR_INBOUND;
	sa->sa_command_1.w              = 0;
	sa->sa_command_1.bf.hmac_muting = 0;
	sa->sa_command_1.bf.sa_rev      = 1;
	sa->sa_contents = sa_contents;

	ctx->direction = DIR_INBOUND;
	memcpy((ctx->sa_in) + get_dynamic_sa_offset_state_ptr_field(ctx),
		(void*)&(ctx->state_record_dma_addr),  4);

	crypto4xx_pre_compute_hmac(ctx, (void *)key, authkeylen,
				   hash_bs, hashing, ds);

	/*
	 * Now, setup command for ESP
	 */
	sa->sa_command_0.bf.load_hash_state = 0;
	sa->sa_command_0.bf.save_hash_state = 0;
	sa->sa_command_0.bf.hdr_proc = 1;


	sa->sa_command_0.bf.load_iv = 2;
	sa->sa_command_0.bf.cipher_alg = cipher_alg;
	sa->sa_command_0.bf.op_group = SA_OP_GROUP_PROTOCOL;
	sa->sa_command_0.bf.opcode = SA_OPCODE_ESP;

	sa->sa_command_1.bf.hmac_muting = 0;

	
	ESP_PRINTK(KERN_INFO "%s: keylen = %d, enckeylen = %d\n",
		   __FUNCTION__, keylen, enckeylen);
	
	if (cipher_alg == SA_CIPHER_ALG_AES) {
		if ( enckeylen == 16)
		{
			ESP_PRINTK(KERN_INFO "%s: AES 128\n", __FUNCTION__);
			sa->sa_command_1.bf.key_len = SA_AES_KEY_LEN_128;
		} else if ( enckeylen == 24){
			ESP_PRINTK(KERN_INFO "%s: AES 192\n", __FUNCTION__);
			sa->sa_command_1.bf.key_len = SA_AES_KEY_LEN_192;
		} else {
			ESP_PRINTK(KERN_INFO "%s: AES 256\n", __FUNCTION__);
			sa->sa_command_1.bf.key_len = SA_AES_KEY_LEN_256;
		}
	} else {
		sa->sa_command_1.bf.key_len = 0;
	}

	sa->sa_command_1.bf.crypto_mode31 = c_mode >> 2;
	sa->sa_command_1.bf.crypto_mode9_8 = c_mode & 3;
	sa->sa_command_1.bf.feedback_mode = 0;
	sa->sa_command_1.bf.copy_payload = 1;
	sa->sa_command_1.bf.copy_pad = 1;
	sa->sa_command_1.bf.copy_hdr = 1;

	sa->sa_command_1.bf.seq_num_mask = 1;
	sa->sa_command_1.bf.mutable_bit_proc = 0;

	sa->sa_command_0.bf.hdr_proc = 1;

	crypto4xx_memcpy_le((void*)(ctx->sa_in) + get_dynamic_sa_offset_key_field(ctx),
			     key+authkeylen, enckeylen);

	memcpy(ctx->sa_in + get_dynamic_sa_offset_spi(ctx),
	       (void*)&(ctx->spi),
	       4);
	memcpy(ctx->sa_in + get_dynamic_sa_offset_seq_num(ctx),
	       (void*)&(ctx->seq), 4);


	sa->sa_command_1.bf.copy_hdr = 0;
	sa->sa_command_1.bf.hash_crypto_offset = 6;

	sa->sa_command_0.bf.load_iv  = 1;
	sa->sa_command_0.bf.dir = DIR_INBOUND;

	memcpy(ctx->sa_out, ctx->sa_in, ctx->sa_len * 4);
	sa = (struct dynamic_sa_ctl *) ctx->sa_out;
	sa->sa_command_0.bf.dir = DIR_OUTBOUND;
	sa->sa_command_0.bf.load_iv = 3;
	sa->sa_command_1.bf.hash_crypto_offset = 0;
	sa->sa_command_1.bf.copy_hdr = 1;


	ctx->bypass = bypass;
	ctx->authenc = 0;
	ctx->hash_final = 1;
	ctx->is_hash = 0;
	ctx->pad_ctl = param->pad_block_size/4;
	ctx->append_icv = 0;

	return 0;

err_nomem_sr:
	crypto4xx_free_sa(ctx);

err_nomem:
	return -ENOMEM;
badkey:
	ESP_PRINTK(KERN_INFO "%s: badkey\n",__FUNCTION__);
	return -EINVAL;
}

/** Encrypt/Decrypt Routines for IPSEC for Transport */
int crypto4xx_encrypt_esp(struct aead_givcrypt_request *givreq,
					struct aead_request *req)
{
	struct crypto4xx_ctx *ctx;
	struct scatterlist *sg;
	struct iphdr *iph;
	void * saddr;
	
	if (givreq)
	        req = &givreq->areq;

	ctx = crypto_tfm_ctx(req->base.tfm);

	sg = req->src;
	saddr = kmap_atomic(sg_page(sg), KM_SOFTIRQ1);
	iph = (struct iphdr *)(saddr + sg->offset);
	ctx->next_hdr = (u32)(iph->protocol);
	iph->protocol = 0x32;

#if 1
	ESP_PHD(KERN_CONT, "", DUMP_PREFIX_OFFSET,
		16, 1,
		(void*)saddr+ sg->offset, sg->length, false);

	ESP_PRINTK(KERN_INFO "%s: next_hdr = %d\n",__FUNCTION__, ctx->next_hdr);
#endif	
	kunmap_atomic(saddr, KM_SOFTIRQ1);
	
	ctx->hc_offset = 0;
	ctx->pd_ctl = (ctx->pad_ctl  << 24) + 0x11; 
	ctx->direction = DIR_OUTBOUND;
	return crypto4xx_build_pd(&req->base, ctx, req->src, req->dst,
				  req->cryptlen, NULL, 0, NULL, 0); 

}

int crypto4xx_decrypt_esp(struct aead_request *req)
{
	struct crypto4xx_ctx *ctx  = crypto_tfm_ctx(req->base.tfm);
#if 1
	struct scatterlist *sg;
	void * saddr;

	sg = req->src;
	saddr = kmap_atomic(sg_page(sg), KM_SOFTIRQ1);
	ESP_PHD(KERN_CONT, "", DUMP_PREFIX_OFFSET,
		16, 1,
		(void*)saddr+sg->offset, sg->length, false);
	kunmap_atomic(saddr, KM_SOFTIRQ1);
#endif
	ctx->hc_offset = 0;
	ctx->pd_ctl =( ctx->pad_ctl << 24) + 0x11; 

	ctx->direction = DIR_INBOUND;

	return crypto4xx_build_pd(&req->base, ctx, req->src, req->dst,
				  req->cryptlen, NULL, 0, NULL, 0); 
}

/**AES Transport Related Algorithms for IPSEC */
int crypto4xx_setkey_transport_esp_cbc_aes_md5(struct crypto_aead *cipher,
					       const u8 *key,
					       unsigned int keylen)
{
	return crypto4xx_setkey_esp(cipher, key, keylen,
				    SA_CIPHER_ALG_AES,
				    SA_HASH_ALG_MD5, 1,
				    SA_ESP_MD5_SHA1_LEN,
				    SA_ESP_MD5_SHA1_CONTENTS, 16, 5, 64);
}

int crypto4xx_setkey_transport_esp_cbc_aes_sha1(struct crypto_aead *cipher,
		const u8 *key,
		unsigned int keylen)
{
	ESP_PRINTK(KERN_INFO "%s: called\n", __FUNCTION__);
	return crypto4xx_setkey_esp(cipher, key, keylen,
				    SA_CIPHER_ALG_AES,
				    SA_HASH_ALG_SHA1, 1,
				    SA_ESP_MD5_SHA1_LEN,
				    SA_ESP_MD5_SHA1_CONTENTS, 20, 5, 64);
}

int crypto4xx_setkey_transport_esp_cbc_aes_sha224(struct crypto_aead *cipher,
		const u8 *key,
		unsigned int keylen)
{
	ESP_PRINTK(KERN_INFO "%s: called\n", __FUNCTION__);
	return crypto4xx_setkey_esp(cipher, key, keylen,
				    SA_CIPHER_ALG_AES,
				    SA_HASH_ALG_SHA224, 1,
				    SA_ESP_SHA256_LEN,
				    SA_ESP_SHA256_CONTENTS, 28, 5, 64);
}

int crypto4xx_setkey_transport_esp_cbc_aes_sha256(struct crypto_aead *cipher,
		const u8 *key,
		unsigned int keylen)
{
	ESP_PRINTK(KERN_INFO "%s: called\n", __FUNCTION__);
	return crypto4xx_setkey_esp(cipher, key, keylen,
				    SA_CIPHER_ALG_AES,
				    SA_HASH_ALG_SHA256, 1,
				    SA_ESP_SHA256_LEN,
				    SA_ESP_SHA256_CONTENTS, 32, 5, 64);
}

int crypto4xx_setkey_transport_esp_cbc_aes_sha384(struct crypto_aead *cipher,
		const u8 *key,
		unsigned int keylen)
{
	ESP_PRINTK(KERN_INFO "%s: called\n", __FUNCTION__);
	return crypto4xx_setkey_esp(cipher, key, keylen,
				    SA_CIPHER_ALG_AES,
				    SA_HASH_ALG_SHA384, 1,
				    SA_ESP_SHA512_LEN,
				    SA_ESP_SHA512_CONTENTS, 48, 5, 128);
}

int crypto4xx_setkey_transport_esp_cbc_aes_sha512(struct crypto_aead *cipher,
		const u8 *key,
		unsigned int keylen)
{
	ESP_PRINTK(KERN_INFO "%s: called\n", __FUNCTION__);
	return crypto4xx_setkey_esp(cipher, key, keylen,
				    SA_CIPHER_ALG_AES,
				    SA_HASH_ALG_SHA512, 1,
				    SA_ESP_SHA512_LEN,
				    SA_ESP_SHA512_CONTENTS, 64, 5, 128);
}

/**DES Transport Related Algorithms for IPSEC */
int crypto4xx_setkey_transport_esp_cbc_des_md5(struct crypto_aead *cipher,
		const u8 *key,
		unsigned int keylen)
{
	ESP_PRINTK(KERN_INFO "%s: called\n", __FUNCTION__);
	return crypto4xx_setkey_esp(cipher, key, keylen,
				    SA_CIPHER_ALG_DES,
				    SA_HASH_ALG_MD5, 1,
				    SA_ESP_MD5_SHA1_LEN,
				    SA_ESP_MD5_SHA1_CONTENTS, 16, 5, 64);
}

int crypto4xx_setkey_transport_esp_cbc_des_sha1(struct crypto_aead *cipher,
		const u8 *key,
		unsigned int keylen)
{
	ESP_PRINTK(KERN_INFO "%s: called\n", __FUNCTION__);
	return crypto4xx_setkey_esp(cipher,
				    key,
				    keylen,
				    SA_CIPHER_ALG_DES,
				    SA_HASH_ALG_SHA1, 1,
				    SA_ESP_MD5_SHA1_LEN,
				    SA_ESP_MD5_SHA1_CONTENTS, 20, 5, 64);
}

int crypto4xx_setkey_transport_esp_cbc_des_sha224(struct crypto_aead *cipher,
		const u8 *key,
		unsigned int keylen)
{
	ESP_PRINTK(KERN_INFO "%s: called\n", __FUNCTION__);
	return crypto4xx_setkey_esp(cipher, key, keylen,
				    SA_CIPHER_ALG_DES,
				    SA_HASH_ALG_SHA224, 1,
				    SA_ESP_SHA256_LEN,
				    SA_ESP_SHA256_CONTENTS, 28, 5, 64);
}

int crypto4xx_setkey_transport_esp_cbc_des_sha256(struct crypto_aead *cipher,
		const u8 *key,
		unsigned int keylen)
{
	ESP_PRINTK(KERN_INFO "%s: called\n", __FUNCTION__);
	return crypto4xx_setkey_esp(cipher, key, keylen,
				    SA_CIPHER_ALG_DES,
				    SA_HASH_ALG_SHA256, 1,
				    SA_ESP_SHA256_LEN,
				    SA_ESP_SHA256_CONTENTS, 32, 5, 64);
}

int crypto4xx_setkey_transport_esp_cbc_des_sha384(struct crypto_aead *cipher,
		const u8 *key,
		unsigned int keylen)
{
	ESP_PRINTK(KERN_INFO "%s: called\n", __FUNCTION__);
	return crypto4xx_setkey_esp(cipher, key, keylen,
				    SA_CIPHER_ALG_DES,
				    SA_HASH_ALG_SHA384, 1,
				    SA_ESP_SHA512_LEN,
				    SA_ESP_SHA512_CONTENTS, 48, 5, 128);
}

int crypto4xx_setkey_transport_esp_cbc_des_sha512(struct crypto_aead *cipher,
		const u8 *key,
		unsigned int keylen)
{
	ESP_PRINTK(KERN_INFO "%s: called\n", __FUNCTION__);
	return crypto4xx_setkey_esp(cipher, key, keylen,
				    SA_CIPHER_ALG_DES,
				    SA_HASH_ALG_SHA512, 1,
				    SA_ESP_SHA512_LEN,
				    SA_ESP_SHA512_CONTENTS, 64, 5, 128);
}

/**3DES Transport Related Algorithms for IPSEC */
int crypto4xx_setkey_transport_esp_cbc_3des_md5(struct crypto_aead *cipher,
		const u8 *key,
		unsigned int keylen)
{
	ESP_PRINTK(KERN_INFO "%s: called\n", __FUNCTION__);
	return crypto4xx_setkey_esp(cipher, key, keylen,
				    SA_CIPHER_ALG_3DES,
				    SA_HASH_ALG_MD5, 1,
				    SA_ESP_MD5_SHA1_LEN,
				    SA_ESP_MD5_SHA1_CONTENTS, 16, 5, 64);
}
int crypto4xx_setkey_transport_esp_cbc_3des_sha1(struct crypto_aead *cipher,
		const u8 *key,
		unsigned int keylen)
{
	ESP_PRINTK(KERN_INFO "%s: called\n", __FUNCTION__);
	return crypto4xx_setkey_esp(cipher, key, keylen,
				    SA_CIPHER_ALG_3DES,
				    SA_HASH_ALG_SHA1, 1,
				    SA_ESP_MD5_SHA1_LEN,
				    SA_ESP_MD5_SHA1_CONTENTS, 20, 5, 64);
}


int crypto4xx_setkey_transport_esp_cbc_3des_sha224(struct crypto_aead *cipher,
		const u8 *key,
		unsigned int keylen)
{
	ESP_PRINTK(KERN_INFO "%s: called\n", __FUNCTION__);
	return crypto4xx_setkey_esp(cipher, key, keylen,
				    SA_CIPHER_ALG_3DES,
				    SA_HASH_ALG_SHA224, 1,
				    SA_ESP_SHA256_LEN,
				    SA_ESP_SHA256_CONTENTS, 28, 5, 64);
}

int crypto4xx_setkey_transport_esp_cbc_3des_sha256(struct crypto_aead *cipher,
		const u8 *key,
		unsigned int keylen)
{
	ESP_PRINTK(KERN_INFO "%s: called\n", __FUNCTION__);
	return crypto4xx_setkey_esp(cipher, key, keylen,
				    SA_CIPHER_ALG_3DES,
				    SA_HASH_ALG_SHA256, 1,
				    SA_ESP_SHA256_LEN,
				    SA_ESP_SHA256_CONTENTS, 32, 5,64);
}

int crypto4xx_setkey_transport_esp_cbc_3des_sha384(struct crypto_aead *cipher,
		const u8 *key,
		unsigned int keylen)
{
	ESP_PRINTK(KERN_INFO "%s: called\n", __FUNCTION__);
	return crypto4xx_setkey_esp(cipher, key, keylen,
				    SA_CIPHER_ALG_3DES,
				    SA_HASH_ALG_SHA384, 1,
				    SA_ESP_SHA512_LEN,
				    SA_ESP_SHA512_CONTENTS, 48, 5, 128);
}

int crypto4xx_setkey_transport_esp_cbc_3des_sha512(struct crypto_aead *cipher,
						const u8 *key,
						unsigned int keylen)
{
	ESP_PRINTK(KERN_INFO "%s: called\n", __FUNCTION__);
	return crypto4xx_setkey_esp(cipher, key, keylen,
				    SA_CIPHER_ALG_3DES,
				    SA_HASH_ALG_SHA512, 1,
				    SA_ESP_SHA512_LEN,
				    SA_ESP_SHA512_CONTENTS, 64, 5, 128);
}

int crypto4xx_encrypt_transport_esp_cbc(struct aead_request *req)
{
	return crypto4xx_encrypt_esp(NULL, req);
}

int crypto4xx_givencrypt_transport_esp_cbc(struct aead_givcrypt_request *req)
{
	return crypto4xx_encrypt_esp(req, NULL);
}

int crypto4xx_decrypt_transport_esp_cbc(struct aead_request *req)
{
	return crypto4xx_decrypt_esp(req);
}
/** Setkey and Encrypt/Decrypt Functions for Macsec */
int crypto4xx_setkey_macsec_gcm(struct crypto_aead *cipher,
				       const u8 *key,
				       unsigned int keylen)
{
	struct crypto_tfm	*tfm = crypto_aead_tfm(cipher);
	struct crypto4xx_ctx	*ctx = crypto_tfm_ctx(tfm);
	struct crypto_alg	*alg = tfm->__crt_alg;
	struct crypto4xx_alg	*my_alg = crypto_alg_to_crypto4xx_alg(alg);
	struct dynamic_sa_ctl	*sa;
	int rc;
	struct offload_param {
		__be32 spi;
		__be32 seq;
		__be32 iv_h;
		__be32 iv_l;
	} *param;

	
	ctx->dev = my_alg->dev;

	/* Create SA */
	if (ctx->sa_in_dma_addr || ctx->sa_out_dma_addr) {
		crypto4xx_free_sa(ctx);
	}

	crypto4xx_alloc_sa(ctx, SA_MACSEC_GCM_LEN );
	if (!ctx->sa_in_dma_addr || !ctx->sa_out_dma_addr)
		goto err_nomem;

	ctx->direction = DIR_INBOUND;
	
	if (!ctx->state_record) {
		crypto4xx_alloc_state_record(ctx);
		if (!ctx->state_record_dma_addr)
			goto err_nomem_sr;
	}
	
	param = (struct offload_param *) key;
	key += sizeof(struct offload_param);

	sa = (struct dynamic_sa_ctl *) ctx->sa_in;
	sa->sa_contents = SA_MACSEC_GCM_CONTENTS;
		
	keylen -= 16;
	rc = crypto4xx_compute_gcm_hash_key_sw(ctx, key, keylen);
	if (rc)
		goto err_nomem_sr;

	memcpy(ctx->sa_in + get_dynamic_sa_offset_state_ptr_field(ctx),
	       (void*)&(ctx->state_record_dma_addr),  4);

	ctx->offset_to_sr_ptr = get_dynamic_sa_offset_state_ptr_field(ctx);
	set_dynamic_sa_command_0(sa,
				 SA_SAVE_HASH,
				 SA_NOT_SAVE_IV,
				 SA_LOAD_HASH_FROM_SA,
				 SA_LOAD_IV_FROM_INPUT,
				 SA_HEADER_PROC,
				 SA_HASH_ALG_GHASH,
				 SA_CIPHER_ALG_AES,
				 SA_PAD_TYPE_ZERO,
				 SA_OP_GROUP_PROTOCOL,
				 SA_OPCODE_EXT_PROT_MACSEC,
				 DIR_INBOUND);

	set_dynamic_sa_command_1(sa, CRYPTO_MODE_AES_CTR,
				 SA_HASH_MODE_HASH,
				 CRYPTO_FEEDBACK_MODE_NO_FB,
				 SA_EXTENDED_SN_OFF,
				 SA_SEQ_MASK_ON,
				 SA_MC_ENABLE,
				 SA_NOT_COPY_PAD,
				 SA_COPY_PAYLOAD,
				 SA_COPY_HDR);

	
	sa->sa_command_1.bf.byte_offset = 1;
	sa->sa_command_1.bf.key_len = keylen >> 3;
	crypto4xx_memcpy_le(ctx->sa_in + get_dynamic_sa_offset_key_field(ctx),
			    key, keylen);

	/* 
	* Setup sa for inbound processing
	*/
	((struct dynamic_sa_macsec_gcm *) ctx->sa_in)->seq_mask[0] = 0x00000001;
	((struct dynamic_sa_macsec_gcm *) ctx->sa_in)->seq_mask[1] = 0x00000000;
	((struct dynamic_sa_macsec_gcm *) ctx->sa_in)->seq_mask[2] = 0x00000000;
	((struct dynamic_sa_macsec_gcm *) ctx->sa_in)->seq_mask[3] = 0x00000000;

	((struct dynamic_sa_macsec_gcm *) ctx->sa_in)->spi = cpu_to_le32(param->spi);
	((struct dynamic_sa_macsec_gcm *) ctx->sa_in)->seq = cpu_to_be32(param->seq);
	((struct dynamic_sa_macsec_gcm *) ctx->sa_in)->iv[0] = cpu_to_le32(param->iv_h);
	((struct dynamic_sa_macsec_gcm *) ctx->sa_in)->iv[1] = cpu_to_le32(param->iv_l);


	sa->sa_command_1.bf.copy_payload = 1;
	sa->sa_command_1.bf.copy_pad = 1;
	sa->sa_command_1.bf.copy_hdr = 1;
	sa->sa_command_1.bf.hash_crypto_offset = 0;

	/* 
	* Setup sa for outbound processing
	*/
	ctx->direction = DIR_OUTBOUND;
	memcpy(ctx->sa_out, ctx->sa_in, ctx->sa_len * 4);
	sa = (struct dynamic_sa_ctl *) ctx->sa_out;
	set_dynamic_sa_command_0(sa,
				 SA_SAVE_HASH,
				 SA_SAVE_IV,
				 SA_LOAD_HASH_FROM_SA,
				 SA_LOAD_IV_FROM_SA,
				 SA_HEADER_PROC,
				 SA_HASH_ALG_GHASH,
				 SA_CIPHER_ALG_AES,
				 0,
				 SA_OP_GROUP_PROTOCOL,
				 SA_OPCODE_EXT_PROT_MACSEC,
				 DIR_OUTBOUND);

	set_dynamic_sa_command_1(sa, CRYPTO_MODE_AES_CTR,
				 SA_HASH_MODE_HASH,
				 CRYPTO_FEEDBACK_MODE_NO_FB,
				 SA_EXTENDED_SN_OFF,
				 SA_SEQ_MASK_ON,
				 SA_MC_ENABLE,
				 SA_NOT_COPY_PAD,
				 SA_COPY_PAYLOAD,
				 SA_COPY_HDR);

	ctx->authenc = 0;
	ctx->hash_final = 1;
	ctx->is_hash = 0;
	ctx->bypass = 0;
	
	return 0;

err_nomem_sr:
		crypto4xx_free_sa(ctx);

err_nomem:
		return -ENOMEM;

}

int crypto4xx_encrypt_macsec(struct aead_request *req)
{
	struct crypto4xx_ctx *ctx  = crypto_tfm_ctx(req->base.tfm);
	ctx->direction = DIR_OUTBOUND;
	ctx->pd_ctl =( ctx->pad_ctl << 24) + 0x11;

	return crypto4xx_build_pd(&req->base, ctx, req->src, req->dst,
				   req->cryptlen, NULL, 0, NULL, 0);
}

int crypto4xx_decrypt_macsec(struct aead_request *req)
{
	struct crypto4xx_ctx *ctx  = crypto_tfm_ctx(req->base.tfm);
	struct crypto4xx_device *dev = ctx->dev;

	dev->macsec_decrypt_num++;
	ctx->pd_ctl =(ctx->pad_ctl << 24) + 0x11;
	ctx->direction = DIR_INBOUND;
	ctx->bypass = 0;

	return crypto4xx_build_pd(&req->base, ctx, req->src, req->dst,
				   req->cryptlen, NULL, 0, NULL, 0);
}

/** DTLS/SSL/TLS Related Algorithms */
static int crypto4xx_setkey_dtls(struct crypto_aead *cipher,
				 const u8 *key,
				 unsigned int keylen,
				 u32 hash_alg,
				 u32 cipher_alg,
				 u32 opcode,
				 u32 op_grp)
{
	struct crypto_tfm	*tfm = crypto_aead_tfm(cipher);
	struct crypto4xx_ctx	*ctx = crypto_tfm_ctx(tfm);
	struct crypto_alg	*alg = tfm->__crt_alg;
	struct crypto4xx_alg	*my_alg = crypto_alg_to_crypto4xx_alg(alg);
	int 	bs 	= crypto_tfm_alg_blocksize(tfm);
	struct rtattr		*rta = (void *) key;
	struct dynamic_sa_ctl	*sa;
	struct offload_param {
		__be32 spi;
		__be32 seq_h;
		__be32 seq_l;
		struct crypto_authenc_key_param authenc_param;
	} *param;

	unsigned int enckeylen;
	unsigned int authkeylen;
	int rc;

	if (!RTA_OK(rta, keylen))
		goto badkey;

	if (rta->rta_type != CRYPTO_AUTHENC_KEYA_PARAM)
		goto badkey;

	if (RTA_PAYLOAD(rta) < sizeof(*param))
		goto badkey;

	ctx->dev  = my_alg->dev;
	param     = RTA_DATA(rta);
	enckeylen = be32_to_cpu(param->authenc_param.enckeylen);

	key    += RTA_ALIGN(rta->rta_len);
	keylen -= RTA_ALIGN(rta->rta_len);
	authkeylen = keylen - enckeylen;

	/* Create SA */
	if (ctx->sa_in_dma_addr || ctx->sa_out_dma_addr)
		crypto4xx_free_sa(ctx);
	
	rc = crypto4xx_alloc_sa(ctx, SA_DTLS_LEN);
	if (rc)
		goto err_nomem;
	
	if (!ctx->state_record) {
		rc = crypto4xx_alloc_state_record(ctx);
		if (rc)
			goto err_nomem_sr;
	}
	
	ctx->direction = DIR_INBOUND;
	sa = (struct dynamic_sa_ctl *) ctx->sa_in;
	sa->sa_contents = SA_DTLS_CONTENTS;
	memcpy(ctx->sa_in + get_dynamic_sa_offset_state_ptr_field(ctx),
	       (void *)&(ctx->state_record_dma_addr), 4);

	set_dynamic_sa_command_0(sa,
				 SA_SAVE_HASH,
				 SA_NOT_SAVE_IV,
				 SA_LOAD_HASH_FROM_SA,
				 SA_LOAD_IV_FROM_INPUT,
				 SA_HEADER_PROC,
				 hash_alg,
				 cipher_alg,
				 SA_PAD_TYPE_DTLS,
				 op_grp,
				 opcode,
				 DIR_INBOUND);

	set_dynamic_sa_command_1(sa, CRYPTO_MODE_CBC,
				 SA_HASH_MODE_HASH,
				 CRYPTO_FEEDBACK_MODE_NO_FB,
				 SA_EXTENDED_SN_OFF,
				 SA_SEQ_MASK_OFF,
				 SA_MC_ENABLE,
				 SA_NOT_COPY_PAD,
				 SA_COPY_PAYLOAD,
				 SA_NOT_COPY_HDR);

	sa->sa_command_1.bf.mutable_bit_proc = 0;

	crypto4xx_pre_compute_hmac(ctx, (void *)key, authkeylen,  bs, hash_alg,
				   authkeylen);
	crypto4xx_memcpy_le((u32 *)((void *) sa +
			get_dynamic_sa_offset_key_field(ctx)),
	key + authkeylen, enckeylen);

	if (cipher_alg == SA_CIPHER_ALG_AES)
		sa->sa_command_1.bf.key_len = enckeylen >> 3;

	((struct dynamic_sa_dtls *) sa)->spi.w  = cpu_to_le32(param->spi);
	((struct dynamic_sa_dtls *) sa)->seq[1] = cpu_to_be32(param->seq_h);
	((struct dynamic_sa_dtls *) sa)->seq[0] = cpu_to_be32(param->seq_l);

	ctx->hash_final = 1;
	ctx->is_hash = 0;
	ctx->pad_ctl = 4;
	ctx->append_icv = 0;
	ctx->pd_ctl =( ctx->pad_ctl << 24) + 0x11;

	memcpy(ctx->sa_out, ctx->sa_in, ctx->sa_len * 4);
	sa = (struct dynamic_sa_ctl *) ctx->sa_out;

	set_dynamic_sa_command_0(sa, SA_NOT_SAVE_HASH, SA_NOT_SAVE_IV,
				 SA_LOAD_HASH_FROM_SA, /*SA_LOAD_IV_FROM_STATE */SA_GEN_IV,
				 SA_HEADER_PROC, hash_alg, cipher_alg,
				 SA_PAD_TYPE_DTLS, SA_OP_GROUP_EXTEND_PROTOCOL,
				 opcode, DIR_OUTBOUND);

	set_dynamic_sa_command_1(sa, CRYPTO_MODE_CBC, SA_HASH_MODE_HASH,
				 CRYPTO_FEEDBACK_MODE_NO_FB, SA_EXTENDED_SN_ON,
				 SA_SEQ_MASK_OFF, SA_MC_ENABLE,
				 SA_COPY_PAD, SA_COPY_PAYLOAD, SA_COPY_HDR);
	return 0;

err_nomem_sr:
		crypto4xx_free_sa(ctx);
err_nomem:
		return -ENOMEM;
	
badkey:
		printk("%s: badkey\n",__FUNCTION__);
	return -EINVAL;
}

int crypto4xx_setkey_dtls_aes_sha1(struct crypto_aead *cipher,
				   const u8 *key,
				   unsigned int keylen)
{
	return crypto4xx_setkey_dtls(cipher, key, keylen, SA_HASH_ALG_SHA1,
				     SA_CIPHER_ALG_AES, SA_OPCODE_DTLS,
				     SA_OP_GROUP_EXTEND_PROTOCOL);
}

int crypto4xx_setkey_dtls_des_sha1(struct crypto_aead *cipher,
					  const u8 *key,
					  unsigned int keylen)
{

	return crypto4xx_setkey_dtls(cipher, key, keylen, SA_HASH_ALG_SHA1,
				     SA_CIPHER_ALG_DES, SA_OPCODE_DTLS,
				     SA_OP_GROUP_EXTEND_PROTOCOL);
}

int crypto4xx_setkey_dtls_des3_sha1(struct crypto_aead *cipher,
					   const u8 *key,
					   unsigned int keylen)
{
	return crypto4xx_setkey_dtls(cipher, key, keylen, SA_HASH_ALG_SHA1,
				     SA_CIPHER_ALG_3DES,  SA_OPCODE_DTLS,
				     SA_OP_GROUP_EXTEND_PROTOCOL);
}

int crypto4xx_setkey_dtls_null_md5(struct crypto_aead *cipher,
					  const u8 *key,
					  unsigned int keylen)
{
	return crypto4xx_setkey_dtls(cipher, key, keylen, SA_HASH_ALG_MD5,
				     SA_CIPHER_ALG_NULL, SA_OPCODE_DTLS,
				     SA_OP_GROUP_EXTEND_PROTOCOL);
}

int crypto4xx_setkey_dtls_null_sha1(struct crypto_aead *cipher,
					   const u8 *key,
					   unsigned int keylen)
{
	return crypto4xx_setkey_dtls(cipher, key, keylen, SA_HASH_ALG_SHA1,
				     SA_CIPHER_ALG_NULL, SA_OPCODE_DTLS,
				     SA_OP_GROUP_EXTEND_PROTOCOL);
}
/** DTLS/SSL/TLS Related Encrypt/Decrypt Algorithms */

int crypto4xx_encrypt_dtls(struct aead_request *req)
{
	struct crypto4xx_ctx *ctx  = crypto_tfm_ctx(req->base.tfm);

	ctx->direction = DIR_OUTBOUND;

	return crypto4xx_build_pd(&req->base, ctx, req->src, req->dst,
				   req->cryptlen, NULL, 0, NULL, 0);
}

int crypto4xx_decrypt_dtls(struct aead_request *req)
{
	struct crypto4xx_ctx *ctx  = crypto_tfm_ctx(req->base.tfm);

	ctx->direction = DIR_INBOUND;

	return crypto4xx_build_pd(&req->base, ctx, req->src, req->dst,
				   req->cryptlen, NULL, 0, NULL, 0);
}
int tls;
/** Setkey Functions for SSL/TLS */

static int crypto4xx_setkey_ssl_tls(struct crypto_aead *cipher,
				    const u8 *key,
				    unsigned int keylen,
				    u32 hash_alg,
				    u32 cipher_alg,
				    u32 opcode,
				    u32 op_grp)
{
	struct crypto_tfm	*tfm = crypto_aead_tfm(cipher);
	struct crypto4xx_ctx	*ctx = crypto_tfm_ctx(tfm);
	struct crypto_alg	*alg = tfm->__crt_alg;
	struct crypto4xx_alg	*my_alg = crypto_alg_to_crypto4xx_alg(alg);
	int bs 	= crypto_tfm_alg_blocksize(tfm);
	struct rtattr		*rta = (void *) key;
	struct dynamic_sa_ctl	*sa;
	struct offload_param {
		__be32 spi;
		__be32 seq_h;
		__be32 seq_l;
		struct crypto_authenc_key_param authenc_param;
	} *param;

	unsigned int enckeylen;
	unsigned int authkeylen;
	int rc;

	if (!RTA_OK(rta, keylen))
		goto badkey;

	if (rta->rta_type != CRYPTO_AUTHENC_KEYA_PARAM)
		goto badkey;

	if (RTA_PAYLOAD(rta) < sizeof(*param))
		goto badkey;
	
	ctx->dev  = my_alg->dev;

	param     = RTA_DATA(rta);

	enckeylen = be32_to_cpu(param->authenc_param.enckeylen);

	key    += RTA_ALIGN(rta->rta_len);
	keylen -= RTA_ALIGN(rta->rta_len);

	authkeylen = keylen - enckeylen;

	/* Create SA */
	if (ctx->sa_in_dma_addr || ctx->sa_out_dma_addr)
		crypto4xx_free_sa(ctx);
	

	rc = crypto4xx_alloc_sa(ctx, SA_DTLS_LEN );
	if (rc)
		goto err_nomem;
	
	if (!ctx->state_record) {
		rc = crypto4xx_alloc_state_record(ctx);
		if (rc)
			goto err_nomem_sr;
	}
	
	ctx->direction = DIR_INBOUND;

	sa = (struct dynamic_sa_ctl *) ctx->sa_in;
	/*
	* Setup hash algorithm and hash mode
	*/
	sa->sa_contents = SA_DTLS_CONTENTS;

	((struct dynamic_sa_dtls *)sa)->state_ptr= (u32)ctx->state_record_dma_addr;

	ctx->offset_to_sr_ptr = get_dynamic_sa_offset_state_ptr_field(ctx);
	set_dynamic_sa_command_0(sa,
				 SA_SAVE_HASH,
				 SA_NOT_SAVE_IV,
				 SA_LOAD_HASH_FROM_SA,
				 SA_LOAD_IV_FROM_STATE,
				 SA_HEADER_PROC,
				 hash_alg,
				 cipher_alg,
				 SA_PAD_TYPE_TLS,
				 op_grp,
				 opcode,
				 DIR_INBOUND);

	set_dynamic_sa_command_1(sa, CRYPTO_MODE_CBC,
				 SA_HASH_MODE_HASH,
				 CRYPTO_FEEDBACK_MODE_NO_FB,
				 SA_EXTENDED_SN_OFF,
				 SA_SEQ_MASK_ON,
				 SA_MC_ENABLE,
				 SA_NOT_COPY_PAD,
				 SA_COPY_PAYLOAD,
				 SA_NOT_COPY_HDR);

	if (opcode == SA_OPCODE_SSL) {
		if (hash_alg == SA_HASH_ALG_SHA1) {
			memcpy(ctx->sa_in +
					get_dynamic_sa_offset_inner_digest(ctx),
			(void*)key, authkeylen);
			memcpy(ctx->sa_in +
					get_dynamic_sa_offset_outer_digest(ctx),
			(void*)key, authkeylen);
		} else if (hash_alg == SA_HASH_ALG_MD5) {
			crypto4xx_pre_compute_ssl_mac(ctx, (void*)key,
					authkeylen, bs,hash_alg);
		}
	} else {
		crypto4xx_pre_compute_hmac(ctx, (void *)key, authkeylen,  bs,
					   hash_alg, authkeylen);
		sa->sa_command_1.bf.hmac_muting = 1;
	}

	((struct dynamic_sa_dtls *) sa)->spi.w  = cpu_to_le32(param->spi);
	((struct dynamic_sa_dtls *) sa)->seq[1] = cpu_to_be32(param->seq_h);
	((struct dynamic_sa_dtls *) sa)->seq[0] = cpu_to_be32(param->seq_l);

	crypto4xx_memcpy_le((u32 *) ((void *) sa +
			get_dynamic_sa_offset_key_field(ctx)),
			key + authkeylen, enckeylen);

	if (cipher_alg == SA_CIPHER_ALG_AES)
		sa->sa_command_1.bf.key_len = enckeylen >> 3;

	ctx->hash_final = 1;
	ctx->is_hash = 0;
	ctx->pad_ctl = 4;
	ctx->append_icv = 0;
	ctx->pd_ctl =( ctx->pad_ctl << 24) + 0x11;
	ctx->direction = DIR_OUTBOUND;

	memcpy(ctx->sa_out, ctx->sa_in, ctx->sa_len * 4);

	sa = (struct dynamic_sa_ctl *) ctx->sa_out;
	set_dynamic_sa_command_0(sa,
				 SA_NOT_SAVE_HASH,
				 SA_NOT_SAVE_IV,
				 SA_LOAD_HASH_FROM_SA,
				 SA_LOAD_IV_FROM_STATE,
				 SA_HEADER_PROC,
				 hash_alg,
				 cipher_alg,
				 SA_PAD_TYPE_TLS,
				 op_grp, opcode,
				 DIR_OUTBOUND);
	set_dynamic_sa_command_1(sa,
				 CRYPTO_MODE_CBC,
				 SA_HASH_MODE_HASH,
				 CRYPTO_FEEDBACK_MODE_NO_FB,
				 SA_EXTENDED_SN_ON,
				 SA_SEQ_MASK_ON,
				 SA_MC_ENABLE,
				 SA_COPY_PAD,
				 SA_COPY_PAYLOAD,
				 SA_COPY_HDR);

	return 0;

err_nomem_sr:
		crypto4xx_free_sa(ctx);
err_nomem:
		return -ENOMEM;
badkey:
		printk("%s: badkey\n",__FUNCTION__);
	return -EINVAL;
}
extern int ssl_arc4;

int crypto4xx_setkey_ssl_tls_arc4(struct crypto_aead *cipher,
					 const u8 *key,
					 unsigned int keylen,
					 u32 hash_alg,
					 u32 cipher_alg,
					 u32 opcode,
					 u32 op_grp)
{
	struct crypto_tfm	*tfm = crypto_aead_tfm(cipher);
	struct crypto4xx_ctx	*ctx = crypto_tfm_ctx(tfm);
	struct crypto_alg	*alg = tfm->__crt_alg;
	struct crypto4xx_alg	*my_alg = crypto_alg_to_crypto4xx_alg(alg);
	int bs 	= crypto_tfm_alg_blocksize(tfm);
	struct rtattr		*rta = (void *) key;
	struct dynamic_sa_ctl	*sa;
	int i, j = 0, k = 0;
	u8 a;
	const u8 arc4_key[256];
	
	struct offload_param {
		__be32 spi;
		__be32 seq_h;
		__be32 seq_l;
		struct crypto_authenc_key_param authenc_param;
	} *param;

	unsigned int enckeylen;
	unsigned int authkeylen;
	int rc;
	
	if (!RTA_OK(rta, keylen))
		goto badkey;
	if (rta->rta_type != CRYPTO_AUTHENC_KEYA_PARAM)
		goto badkey;

	if (RTA_PAYLOAD(rta) < sizeof(*param))
		goto badkey;

	ctx->dev  = my_alg->dev;
	param     = RTA_DATA(rta);
	enckeylen = be32_to_cpu(param->authenc_param.enckeylen);

	key    += RTA_ALIGN(rta->rta_len);
	keylen -= RTA_ALIGN(rta->rta_len);

	authkeylen = keylen - enckeylen;
	/* Create SA */
	if (ctx->sa_in_dma_addr || ctx->sa_out_dma_addr) {
		crypto4xx_free_sa(ctx);
	}

	rc = crypto4xx_alloc_sa(ctx, SA_SSL_ARC4_LEN);
	if (rc)
		return rc;

	if (!ctx->state_record) {
		rc = crypto4xx_alloc_state_record(ctx);
		if (rc)
			goto err_nomem_sr;
	}
	
	if (ctx->arc4_state_record == NULL) {
		rc = crypto4xx_alloc_arc4_state_record(ctx);
		if (rc)
			goto err_nomem_arc4;
	}

	sa = (struct dynamic_sa_ctl *) ctx->sa_in;

	ctx->direction = DIR_INBOUND;
	ctx->init_arc4 = 1;
	sa->sa_contents = SA_SSL_ARC4_CONTENTS;

	memcpy(ctx->sa_in + get_dynamic_sa_offset_state_ptr_field(ctx),
	    (void *)&(ctx->state_record_dma_addr), 4);
	ctx->offset_to_sr_ptr = get_dynamic_sa_offset_state_ptr_field(ctx);
	set_dynamic_sa_command_0(sa,
				 SA_NOT_SAVE_HASH, SA_NOT_SAVE_IV,
				 SA_LOAD_HASH_FROM_SA,
				 SA_LOAD_IV_FROM_SA,
				 SA_HEADER_PROC,
				 hash_alg,
				 cipher_alg,
				 SA_PAD_TYPE_TLS,
				 op_grp,
				 opcode,
				 DIR_INBOUND);

	set_dynamic_sa_command_1(sa,
				 CRYPTO_MODE_CBC,
				 SA_HASH_MODE_HASH,
				 CRYPTO_FEEDBACK_MODE_NO_FB,
				 SA_EXTENDED_SN_ON,
				 SA_SEQ_MASK_OFF,
				 SA_MC_ENABLE,
				 SA_NOT_COPY_PAD,
				 SA_COPY_PAYLOAD,
				 SA_NOT_COPY_HDR);

	sa->sa_command_1.bf.arc4_stateful = 1;
	sa->sa_command_1.bf.save_arc4_state = 1;

	if (opcode == SA_OPCODE_SSL) {
		if (hash_alg == SA_HASH_ALG_SHA1) {
			crypto4xx_memcpy_le(ctx->sa_in +
					get_dynamic_sa_offset_inner_digest(ctx),
			(void*) key, authkeylen);
			crypto4xx_memcpy_le(ctx->sa_in +
					get_dynamic_sa_offset_outer_digest(ctx),
			(void*) key, authkeylen);
		} else if (hash_alg == SA_HASH_ALG_MD5) {
			crypto4xx_pre_compute_ssl_mac(ctx, (void*)key,
					authkeylen, bs, hash_alg);
		}
		sa->sa_command_1.bf.hmac_muting = 0;
	}  else {
		crypto4xx_pre_compute_hmac(ctx, (void*) key, authkeylen, bs,
					   hash_alg, authkeylen);
		sa->sa_command_1.bf.hmac_muting = 1;
	}

	((struct dynamic_sa_ssl_tls_arc4 *) sa)->arc4_state_ptr =
					(u32)ctx->arc4_state_record_dma_addr;
	
	/* Setting Key */
	crypto4xx_memcpy_le((u32 *) ((void *) sa +
			get_dynamic_sa_offset_key_field(ctx)),
	key + authkeylen, enckeylen);

	memcpy((u8 *)arc4_key, key + authkeylen, enckeylen);

	((struct dynamic_sa_ssl_tls_arc4 *) sa)->spi.w =
			cpu_to_le32(param->spi);
	((struct dynamic_sa_ssl_tls_arc4 *) sa)->seq[1] =
			cpu_to_be32(param->seq_h);
	((struct dynamic_sa_ssl_tls_arc4 *) sa)->seq[0] =
			cpu_to_be32(param->seq_l);

	/* For stateful mode we need to initialize the ARC4 state record */
	((struct dynamic_sa_ssl_tls_arc4 *) ctx->sa_in)->ij.i = 1;
	((struct dynamic_sa_ssl_tls_arc4 *) ctx->sa_in)->ij.j = 0;

	for (i = 0; i < 256; i++)
		((struct arc4_sr *) ctx->arc4_state_record)->arc4_state[i] = i;
	
	for (i = 0; i < 256; i++) {
		a = ((struct arc4_sr *) ctx->arc4_state_record)->arc4_state[i];
		j = (j + arc4_key[k] + a) & 0xff;
		((struct arc4_sr *) ctx->arc4_state_record)->arc4_state[i] =
				((struct arc4_sr *) ctx->arc4_state_record)->arc4_state[j];
		((struct arc4_sr *) ctx->arc4_state_record)->arc4_state[j] = a;
		if (++k >= enckeylen)
			k = 0;
	}
	
	ctx->hash_final = 1;
	ctx->is_hash = 0;
	ctx->pad_ctl = 4;
	ctx->append_icv = 0;
	ctx->direction = DIR_OUTBOUND;
	ctx->pd_ctl =( ctx->pad_ctl << 24) + 0x11;

	/* Setup SA command for outbound process */
	memcpy(ctx->sa_out, ctx->sa_in, ctx->sa_len * 4);
	sa = (struct dynamic_sa_ctl *) ctx->sa_out;
	set_dynamic_sa_command_0(sa,
				 SA_NOT_SAVE_HASH,
				 SA_NOT_SAVE_IV,
				 SA_LOAD_HASH_FROM_SA,
				 SA_LOAD_IV_FROM_SA,
				 SA_HEADER_PROC,
				 hash_alg,
				 cipher_alg,
				 SA_PAD_TYPE_TLS,
				 op_grp,
				 opcode,
				 DIR_OUTBOUND);
	
	set_dynamic_sa_command_1(sa,
				 CRYPTO_MODE_CBC,
				 SA_HASH_MODE_HASH,
				 CRYPTO_FEEDBACK_MODE_NO_FB,
				 SA_EXTENDED_SN_ON,
				 SA_SEQ_MASK_ON,
				 SA_MC_ENABLE,
				 SA_COPY_PAD,
				 SA_COPY_PAYLOAD,
				 SA_COPY_HDR);

	sa->sa_command_1.bf.arc4_stateful = 1;
	sa->sa_command_1.bf.save_arc4_state = 1;

	return 0;

err_nomem_arc4:
	crypto4xx_free_state_record(ctx);
err_nomem_sr:
	crypto4xx_free_sa(ctx);
	return -ENOMEM;
badkey:
	printk("%s: badkey\n",__FUNCTION__);
	return 0xffffffff;
}

int crypto4xx_setkey_ssl_aes_sha1(struct crypto_aead *cipher,
				  const u8 *key,
				  unsigned int keylen)
{
	return crypto4xx_setkey_ssl_tls(cipher, key, keylen, SA_HASH_ALG_SHA1,
					SA_CIPHER_ALG_AES, SA_OPCODE_SSL,
					SA_OP_GROUP_EXTEND_PROTOCOL);
}

int crypto4xx_setkey_ssl_des_sha1(struct crypto_aead *cipher,
					 const u8 *key,
					 unsigned int keylen)
{

	return crypto4xx_setkey_ssl_tls(cipher, key, keylen, SA_HASH_ALG_SHA1,
					SA_CIPHER_ALG_DES, SA_OPCODE_SSL,
					SA_OP_GROUP_EXTEND_PROTOCOL);
}
int crypto4xx_setkey_ssl_des3_sha1(struct crypto_aead *cipher,
					  const u8 *key,
					  unsigned int keylen)
{
	return crypto4xx_setkey_ssl_tls(cipher, key, keylen, SA_HASH_ALG_SHA1,
					SA_CIPHER_ALG_3DES, SA_OPCODE_SSL,
					SA_OP_GROUP_EXTEND_PROTOCOL);
}

int crypto4xx_setkey_ssl_arc4_sha1(struct crypto_aead *cipher,
					  const u8 *key,
					  unsigned int keylen)
{
	return crypto4xx_setkey_ssl_tls_arc4(cipher, key, keylen,
					     SA_HASH_ALG_SHA1,
					     SA_CIPHER_ALG_ARC4,
					     SA_OPCODE_SSL,
					     SA_OP_GROUP_EXTEND_PROTOCOL);
}

int crypto4xx_setkey_ssl_arc4_md5(struct crypto_aead *cipher,
				  const u8 *key,
				  unsigned int keylen)
{
	return crypto4xx_setkey_ssl_tls_arc4(cipher, key, keylen,
					     SA_HASH_ALG_MD5,
					     SA_CIPHER_ALG_ARC4, SA_OPCODE_SSL,
					     SA_OP_GROUP_EXTEND_PROTOCOL);
}

int crypto4xx_setkey_ssl_null_md5(struct crypto_aead *cipher,
				  const u8 *key,
				  unsigned int keylen)
{

	return crypto4xx_setkey_ssl_tls(cipher, key, keylen, SA_HASH_ALG_MD5,
					SA_CIPHER_ALG_NULL, SA_OPCODE_SSL,
					SA_OP_GROUP_EXTEND_PROTOCOL);
}

int crypto4xx_setkey_ssl_null_sha1(struct crypto_aead *cipher,
				   const u8 *key,
				   unsigned int keylen)
{
	return crypto4xx_setkey_ssl_tls(cipher, key, keylen, SA_HASH_ALG_SHA1,
					SA_CIPHER_ALG_NULL, SA_OPCODE_SSL,
					SA_OP_GROUP_EXTEND_PROTOCOL);
}

/** Encrypt Decrpt Functions for SSL-AES*/
int crypto4xx_encrypt_ssl_aes(struct aead_request *req)
{
	struct crypto4xx_ctx *ctx  = crypto_tfm_ctx(req->base.tfm);

	ctx->direction = DIR_OUTBOUND;
	return crypto4xx_build_pd(&req->base, ctx, req->src, req->dst,
				   req->cryptlen, NULL, 0,
				   req->iv, AES_BLOCK_SIZE);
}

int crypto4xx_decrypt_ssl_aes(struct aead_request *req)
{
	struct crypto4xx_ctx *ctx  = crypto_tfm_ctx(req->base.tfm);

	ctx->direction = DIR_INBOUND;

	return crypto4xx_build_pd(&req->base, ctx, req->src, req->dst,
				   req->cryptlen, NULL, 0,
				   req->iv, AES_BLOCK_SIZE);
}

/** Encrypt Decrpt Functions for SSL-DES*/
int crypto4xx_encrypt_ssl_des(struct aead_request *req)
{
	struct crypto4xx_ctx *ctx  = crypto_tfm_ctx(req->base.tfm);

	ctx->direction = DIR_OUTBOUND;

	return crypto4xx_build_pd(&req->base, ctx, req->src, req->dst,
				   req->cryptlen, NULL, 0,
				   req->iv, DES_BLOCK_SIZE);
}

int crypto4xx_decrypt_ssl_des(struct aead_request *req)
{
	struct crypto4xx_ctx *ctx  = crypto_tfm_ctx(req->base.tfm);

	ctx->direction = DIR_INBOUND;
	
	return crypto4xx_build_pd(&req->base, ctx, req->src, req->dst,
				   req->cryptlen, NULL, 0,
				   req->iv, DES_BLOCK_SIZE);
}

/** Encrypt Decyrpt Functions for SSL-NULL*/
int crypto4xx_encrypt_ssl_null(struct aead_request *req)
{
	struct crypto4xx_ctx *ctx  = crypto_tfm_ctx(req->base.tfm);

	ctx->direction = DIR_OUTBOUND;
	
	return crypto4xx_build_pd(&req->base, ctx, req->src, req->dst,
				   req->cryptlen, NULL, 0, NULL, 0);
}

int crypto4xx_decrypt_ssl_null(struct aead_request *req)
{
	struct crypto4xx_ctx *ctx  = crypto_tfm_ctx(req->base.tfm);

	ctx->direction = DIR_INBOUND;

	return crypto4xx_build_pd(&req->base, ctx, req->src, req->dst,
				   req->cryptlen, NULL, 0, NULL, 0);
}

int ssl_arc4 = 0;
/** Encrypt Decyrpt Functions for SSL- ARC4*/
int crypto4xx_encrypt_ssl_arc4(struct aead_request *req)
{
	struct crypto4xx_ctx *ctx  = crypto_tfm_ctx(req->base.tfm);
	int ret = 0;
	
	ctx->direction = DIR_OUTBOUND;
	ssl_arc4 = 1;
	ret =  crypto4xx_build_pd(&req->base, ctx, req->src, req->dst,
				   req->cryptlen, NULL, 0, NULL, 0);
	ssl_arc4 = 0;
	return ret;
}

int crypto4xx_decrypt_ssl_arc4(struct aead_request *req)
{
	int ret;
	struct crypto4xx_ctx *ctx  = crypto_tfm_ctx(req->base.tfm);
	ssl_arc4 = 1;
	ctx->direction = DIR_INBOUND;

	ret = crypto4xx_build_pd(&req->base, ctx, req->src, req->dst,
				   req->cryptlen, NULL, 0, NULL, 0);
	ssl_arc4 = 0;
	return ret;
}

/** TLS and TLS V1 Setkey Functions */
int crypto4xx_setkey_tls_aes_sha1(struct crypto_aead *cipher,
					 const u8 *key,
					 unsigned int keylen)
{
	return crypto4xx_setkey_ssl_tls(cipher, key, keylen, SA_HASH_ALG_SHA1,
					SA_CIPHER_ALG_AES, SA_OPCODE_TLS,
					SA_OP_GROUP_EXTEND_PROTOCOL);
}

int crypto4xx_setkey_tls_des_sha1(struct crypto_aead *cipher,
					 const u8 *key,
					 unsigned int keylen)
{
	return crypto4xx_setkey_ssl_tls(cipher, key, keylen, SA_HASH_ALG_SHA1,
					SA_CIPHER_ALG_DES, SA_OPCODE_TLS,
					SA_OP_GROUP_EXTEND_PROTOCOL);
}

int crypto4xx_setkey_tls_des3_sha1(struct crypto_aead *cipher,
					  const u8 *key,
					  unsigned int keylen)
{
	return crypto4xx_setkey_ssl_tls(cipher, key, keylen, SA_HASH_ALG_SHA1,
					SA_CIPHER_ALG_3DES, SA_OPCODE_TLS,
					SA_OP_GROUP_EXTEND_PROTOCOL);
}

int crypto4xx_setkey_tls_arc4_sha1(struct crypto_aead *cipher,
					  const u8 *key,
					  unsigned int keylen)
{
	return crypto4xx_setkey_ssl_tls_arc4(cipher, key, keylen,
					     SA_HASH_ALG_SHA1,
					     SA_CIPHER_ALG_ARC4, SA_OPCODE_TLS,
					     SA_OP_GROUP_EXTEND_PROTOCOL);
}

int crypto4xx_setkey_tls_arc4_md5(struct crypto_aead *cipher,
					 const u8 *key,
					 unsigned int keylen)
{
	return crypto4xx_setkey_ssl_tls_arc4(cipher, key, keylen,
					     SA_HASH_ALG_MD5,
					     SA_CIPHER_ALG_ARC4, SA_OPCODE_TLS,
					     SA_OP_GROUP_EXTEND_PROTOCOL);
}

int crypto4xx_setkey_tls_null_md5(struct crypto_aead *cipher,
					 const u8 *key,
					 unsigned int keylen)
{
	return crypto4xx_setkey_ssl_tls(cipher, key, keylen, SA_HASH_ALG_MD5,
					SA_CIPHER_ALG_NULL, SA_OPCODE_TLS,
					SA_OP_GROUP_EXTEND_PROTOCOL);
}
int crypto4xx_setkey_tls_null_sha1(struct crypto_aead *cipher,
					  const u8 *key,
					  unsigned int keylen)
{
	return crypto4xx_setkey_ssl_tls(cipher, key, keylen, SA_HASH_ALG_SHA1,
					SA_CIPHER_ALG_NULL, SA_OPCODE_TLS,
					SA_OP_GROUP_EXTEND_PROTOCOL);
}

int crypto4xx_setkey_tls1_1_aes_sha1(struct crypto_aead *cipher,
					    const u8 *key,
					    unsigned int keylen)
{
	tls = 1;
	return crypto4xx_setkey_ssl_tls(cipher, key, keylen, SA_HASH_ALG_SHA1,
					SA_CIPHER_ALG_AES, SA_OPCODE_TLS1_1,
					SA_OP_GROUP_EXTEND_PROTOCOL);
}

int crypto4xx_setkey_tls1_1_des_sha1(struct crypto_aead *cipher,
					    const u8 *key,
					    unsigned int keylen)
{
	return crypto4xx_setkey_ssl_tls(cipher, key, keylen, SA_HASH_ALG_SHA1,
					SA_CIPHER_ALG_DES, SA_OPCODE_TLS1_1,
					SA_OP_GROUP_EXTEND_PROTOCOL);
}

int crypto4xx_setkey_tls1_1_des3_sha1(struct crypto_aead *cipher,
					     const u8 *key,
					     unsigned int keylen)
{
	return crypto4xx_setkey_ssl_tls(cipher, key, keylen, SA_HASH_ALG_SHA1,
					SA_CIPHER_ALG_3DES, SA_OPCODE_TLS1_1,
					SA_OP_GROUP_EXTEND_PROTOCOL);
}

int crypto4xx_setkey_tls1_1_arc4_sha1(struct crypto_aead *cipher,
					     const u8 *key,
					     unsigned int keylen)
{
	return crypto4xx_setkey_ssl_tls_arc4(cipher, key, keylen,
					     SA_HASH_ALG_SHA1,
					     SA_CIPHER_ALG_ARC4, SA_OPCODE_TLS1_1,
					     SA_OP_GROUP_EXTEND_PROTOCOL);
}

int crypto4xx_setkey_tls1_1_arc4_md5(struct crypto_aead *cipher,
					    const u8 *key,
					    unsigned int keylen)
{
	return crypto4xx_setkey_ssl_tls_arc4(cipher, key, keylen,
					     SA_HASH_ALG_MD5,
					     SA_CIPHER_ALG_ARC4, SA_OPCODE_TLS1_1,
					     SA_OP_GROUP_EXTEND_PROTOCOL);
}

int crypto4xx_setkey_tls1_1_null_md5(struct crypto_aead *cipher,
					    const u8 *key,
					    unsigned int keylen)
{
	return crypto4xx_setkey_ssl_tls(cipher, key, keylen, SA_HASH_ALG_MD5,
					SA_CIPHER_ALG_NULL, SA_OPCODE_TLS1_1,
					SA_OP_GROUP_EXTEND_PROTOCOL);
}
int crypto4xx_setkey_tls1_1_null_sha1(struct crypto_aead *cipher,
					     const u8 *key,
					     unsigned int keylen)
{
	return crypto4xx_setkey_ssl_tls(cipher, key, keylen, SA_HASH_ALG_SHA1,
					SA_CIPHER_ALG_NULL, SA_OPCODE_TLS1_1,
					SA_OP_GROUP_EXTEND_PROTOCOL);
}

int crypto4xx_setkey_transport_esp_rfc4106_gcm(struct crypto_aead *cipher,
		const u8 *key,
		unsigned int keylen)
{
	struct crypto_tfm	*tfm = crypto_aead_tfm(cipher);
	struct crypto4xx_ctx	*ctx = crypto_tfm_ctx(tfm);
	struct crypto_alg	*alg = tfm->__crt_alg;
	struct crypto4xx_alg	*my_alg = crypto_alg_to_crypto4xx_alg(alg);
	struct rtattr		*rta = (void *) key;
	struct dynamic_sa_ctl	*sa;
	u32 rc;

	ctx->dev = my_alg->dev;
	struct esp_authenc_param {
		__be32 spi;
		__be32 seq;
		__be16 pad_block_size;
		__be16 encap_uhl;
		struct crypto_authenc_key_param authenc_param;
	} *param;

	unsigned int enckeylen;

	ESP_PRINTK("%s: keylen = %d\n",__FUNCTION__, keylen);
	ESP_PHD(KERN_CONT, "", DUMP_PREFIX_OFFSET,
		16, 1,
		(void*)key, keylen, false);

	if (!RTA_OK(rta, keylen)) {
		printk("%s: badkey 1\n",__FUNCTION__);
		goto badkey;
	}

	param = RTA_DATA(rta);
	enckeylen = be32_to_cpu(param->authenc_param.enckeylen);

	key += RTA_ALIGN(rta->rta_len);
	key += 4;
	//keylen -= RTA_ALIGN(rta->rta_len);
	keylen = keylen - sizeof(*param) - 4;
	
	ctx->spi = be32_to_cpu(param->spi);
	ctx->seq = be32_to_cpu(param->seq);
	ctx->pad_block_size = be16_to_cpu(param->pad_block_size);
	ctx->encap_uhl   = be16_to_cpu(param->encap_uhl);

	ESP_PRINTK("%s: spi = 0x%08x, seq = %d, pad_size = %d, encap uhl = %d\n",__FUNCTION__,
		   ctx->spi, ctx->seq, ctx->pad_block_size, ctx->encap_uhl);

	/* Create SA */
	if (ctx->sa_in_dma_addr || ctx->sa_out_dma_addr) {
		crypto4xx_free_sa(ctx);
	}

	rc = crypto4xx_alloc_sa(ctx, SA_ESP_GCM_LEN);
	if (rc)
		return rc;

	if (!ctx->state_record) {
		crypto4xx_alloc_state_record(ctx);
		if (!ctx->state_record_dma_addr)
			goto err_nomem_sr;
	}
	
	sa = (struct dynamic_sa_ctl *) ctx->sa_in;
	ctx->direction = DIR_INBOUND;
	sa->sa_contents = SA_ESP_GCM_CONTENTS;
	ctx->offset_to_sr_ptr = get_dynamic_sa_offset_state_ptr_field(ctx);

	rc = crypto4xx_compute_gcm_hash_key_sw(ctx, key, keylen);
	if (rc)
		goto err_nomem_sr;

	memcpy(ctx->sa_in + get_dynamic_sa_offset_state_ptr_field(ctx),
	       (void*)&(ctx->state_record_dma_addr),  4);

	crypto4xx_memcpy_le(ctx->sa_in + get_dynamic_sa_offset_key_field(ctx),
			    key, keylen);

	memcpy(ctx->sa_in + get_dynamic_sa_offset_spi(ctx),
	       (void*)&(ctx->spi), 4);
	memcpy(ctx->sa_in + get_dynamic_sa_offset_seq_num(ctx),
	       (void*)&(ctx->seq), 4);


	set_dynamic_sa_command_0(sa,
				 SA_NOT_SAVE_HASH,
				 SA_NOT_SAVE_IV,
				 SA_LOAD_HASH_FROM_SA,
				 SA_LOAD_IV_FROM_INPUT,
				 SA_HEADER_PROC,
				 SA_HASH_ALG_GHASH,
				 SA_CIPHER_ALG_AES,
				 0,
				 SA_OP_GROUP_PROTOCOL,
				 SA_OPCODE_ESP,
				 DIR_INBOUND);

	sa->sa_command_0.bf.digest_len = 3;
	set_dynamic_sa_command_1(sa, CRYPTO_MODE_AES_CTR,
				 SA_HASH_MODE_HASH,
				 CRYPTO_FEEDBACK_MODE_NO_FB,
				 SA_EXTENDED_SN_OFF,
				 SA_SEQ_MASK_ON,
				 SA_MC_ENABLE,
				 SA_COPY_PAD,
				 SA_COPY_PAYLOAD,
				 SA_NOT_COPY_HDR);

	sa->sa_command_1.bf.key_len = SA_AES_KEY_LEN_128;
	sa->sa_command_1.bf.hash_crypto_offset = 0;
	/* 
	* Setup sa for outbound processing
	*/
	memcpy(ctx->sa_out, ctx->sa_in, ctx->sa_len * 4);
	sa = (struct dynamic_sa_ctl *) ctx->sa_out;

	set_dynamic_sa_command_0(sa,
				 SA_NOT_SAVE_HASH,
				 SA_NOT_SAVE_IV,
				 SA_LOAD_HASH_FROM_SA,
				 //SA_LOAD_IV_FROM_SA,
				 SA_LOAD_IV_GEN_IV,
				 SA_HEADER_PROC,
				 SA_HASH_ALG_GHASH,
				 SA_CIPHER_ALG_AES,
				 0,
				 SA_OP_GROUP_PROTOCOL,
				 SA_OPCODE_ESP,
				 DIR_OUTBOUND);

	sa->sa_command_0.bf.digest_len = 3;
	set_dynamic_sa_command_1(sa, CRYPTO_MODE_AES_CTR,
				 SA_HASH_MODE_HASH,
				 CRYPTO_FEEDBACK_MODE_NO_FB,
				 SA_EXTENDED_SN_OFF,
				 SA_SEQ_MASK_ON,
				 SA_MC_ENABLE,
				 SA_COPY_PAD,
				 SA_COPY_PAYLOAD,
				 SA_COPY_HDR);

	sa->sa_command_1.bf.key_len = SA_AES_KEY_LEN_128;
	sa->sa_command_1.bf.hash_crypto_offset = 0;
	
	ctx->bypass = 5;
	ctx->authenc = 0;
	ctx->hash_final = 1;
	ctx->is_hash = 0;
	printk("param->pad_block_size = %d\n", param->pad_block_size);
	//ctx->pad_ctl = param->pad_block_size / 4;
	ctx->pad_ctl = 0x08;
	ctx->append_icv = 0;

	return 0;

err_nomem_sr:
		crypto4xx_free_sa(ctx);

	return -ENOMEM;
badkey:

		ESP_PRINTK("%s: badkey\n",__FUNCTION__);
	return -EINVAL;
}
