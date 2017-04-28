/**
 * AppliedMicro APM86xxx SoC Crypto Driver for 
 *
 * Copyright (c) 2010 Applied Micro Circuits Corporation.
 * All rights reserved. Loc Ho <lho@amcc.com>
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
 * @file apm_sec_alg.c
 *
 * This file implements the Linux crypto algorithms for APM APM86xxx SoC 
 * security driver.
 *
 */
#include <linux/kernel.h>
#include <linux/module.h>
#include <linux/moduleparam.h>
#include <linux/mod_devicetable.h>
#include <linux/interrupt.h>
#include <linux/spinlock_types.h>
#include <linux/highmem.h>
#include <linux/scatterlist.h>
#include <linux/interrupt.h>
#include <linux/crypto.h>
#include <linux/rtnetlink.h>
#include <linux/ip.h>
#include <linux/crypto.h>
#include <crypto/aead.h>
#include <crypto/algapi.h>
#include <crypto/des.h>
#include <crypto/authenc.h>
#include <crypto/hash.h>
#include <crypto/internal/hash.h>	

#include "apm_sec_priv.h"
#include "apm_sec_sa.h"
#include "apm_sec_tkn.h"
#include "apm_sec_alg.h"
#include "apm_sec_access.h"

/**
 * Support MD5 Hashing Algorithms
 *
 */

static int apm_sec_hash_alg_init(struct crypto_tfm *tfm, unsigned int sa_len, 
				unsigned char ha, unsigned char dt)
{
	struct apm_sec_session_ctx *session = crypto_tfm_ctx(tfm);
	int	rc;

	apm_sec_session_init(session);
	rc = apm_sec_create_sa_tkn_pool(session, sa_len, sa_len, 0,
					TKN_BASIC_MAX_LEN);
	if (rc != 0)
		goto err_nomem;

	crypto_ahash_set_reqsize(__crypto_ahash_cast(tfm),
                       		sizeof(struct apm_sec_req_ctx));
	
	APMSEC_SATKNLOG("alg %s init sa 0x%p sa_ptr 0x%p",
		tfm->__crt_alg->cra_name, session->sa, session->sa->sa_ptr);
	/* Setup SA */
	session->sa->sa_ptr->hash_alg    = ha;
	session->sa->sa_ptr->digest_type = dt;
	session->sa->sa_ptr->ToP	 = SA_TOP_HASH_OUTBOUND;
	
	return 0;

err_nomem:
	apm_sec_free_sa_tkn_pool(session);
	return rc;
}

static int apm_sec_md5_alg_init(struct crypto_tfm *tfm)
{
	return apm_sec_hash_alg_init(tfm, SA_MD5_LEN(0),
				SA_HASH_ALG_MD5, SA_DIGEST_TYPE_INIT);
}

static int apm_sec_compute_ipad_opad(struct crypto_tfm *tfm,
					const unsigned char *key,
					unsigned int keylen,
					unsigned char *ipad_opad)			
{
	struct crypto_shash 	*child_hash = NULL;
	struct crypto_tfm 	*child_tfm;
	unsigned char 	*input = NULL;
	unsigned char	*opad;
	unsigned char	*ipad;
	const char 	*alg_name;
	char 	*child_name;
	char	bigendian;
	int 	bs; 
	int 	ds, ctx_size;
	int	dl;
	int	rc;
	int	i;
	struct apm_sec_export sdesc;

	alg_name = crypto_tfm_alg_name(tfm);	
	if (strstr(alg_name, "md5") != 0) {
		child_name = "md5";
		bigendian  = 0;
	} else if (strstr(alg_name, "sha1") != 0) {
		child_name = "sha1";
		bigendian  = 0;
	} else if (strstr(alg_name, "sha224") != 0) {
		child_name = "sha224";
		bigendian  = 0;
	} else if (strstr(alg_name, "sha256") != 0) {
		child_name = "sha256";
		bigendian  = 0;
	} else if (strstr(alg_name, "sha384") != 0) {
		child_name = "sha384";
		bigendian  = 0;
	} else if (strstr(alg_name, "sha512") != 0) {
		child_name = "sha512";
		bigendian  = 0;
	} else {
		APMSEC_RXLOG("unsupported algorithm %s", alg_name);
		return -EINVAL;
	}
	
	child_hash = crypto_alloc_shash(child_name, 0, 0);
	if (IS_ERR(child_hash)) {
		rc = PTR_ERR(child_hash);
		APMSEC_ERR("failed to load transform for %s error %d",
				child_name, rc);
		goto err_alg;
	}	
	
	child_tfm = crypto_shash_tfm(child_hash);
	dl = ds = crypto_shash_digestsize(child_hash);
	bs = crypto_tfm_alg_blocksize(child_tfm);
	ctx_size = crypto_shash_descsize(child_hash);
	/* Allocate buffer for ipad/opad/key buffer */
	input = kzalloc(ctx_size * 2, GFP_KERNEL);
	if (input == NULL) {
		rc = -ENOMEM;
		goto err_alg_hash_key;
	}
	ipad = input;
	opad = input + ctx_size;
	
	APMSEC_DEBUG("hmac ipad/opad %s keylen %d bs %d ds %d",
			child_name, keylen, bs, ds);
	APMSEC_DEBUG_DUMP("hmac key: ", key, keylen);

	sdesc.shash.tfm = child_hash;
	sdesc.shash.flags    = 0;

	if (keylen > bs) {
		rc = crypto_shash_init(&sdesc.shash);
		if (rc < 0)
			goto err_alg_hash_key;
		rc = crypto_shash_update(&sdesc.shash, key, keylen);
		if (rc < 0)
			goto err_alg_hash_key;
		rc = crypto_shash_final(&sdesc.shash, ipad);
		keylen = ds;
	} else {
		memcpy(ipad, key, keylen);
	}
	if (strstr(child_name, "sha224") != 0)
		dl = 32;
	else if (strstr(child_name, "sha384") != 0)
		dl = 64;

	memset(ipad + keylen, 0, ctx_size - keylen);
	memcpy(opad, ipad, ctx_size);
	for (i = 0; i < bs; i++) {
		ipad[i] ^= 0x36;
		opad[i] ^= 0x5c;
	}

	crypto_shash_init(&sdesc.shash);
	crypto_shash_update(&sdesc.shash, ipad, bs);	
 	crypto_shash_export(&sdesc.shash, ipad);

 	/* We load the digest in byte stream array */
 	if (bigendian) {
		sec_sa_setbe((u32 *) ipad_opad, ipad, dl);
	} else if (dl < 64) {
		if (strstr(child_name, "md5") != 0)
			sec_sa_setle((u32 *) ipad_opad, ipad, dl);
		else
			sec_sa_setbe((u32 *) ipad_opad, ipad + 8, dl);
	} else {
		ipad += 16;
		sec_sa_setbe((u32 *) ipad_opad, ipad, dl);
	}

	crypto_shash_init(&sdesc.shash);
 	crypto_shash_update(&sdesc.shash, opad, bs);
	crypto_shash_export(&sdesc.shash, opad);
 	/* We load the digest in byte stream array */
 	if (bigendian) {
		sec_sa_setbe((u32 *) (ipad_opad + dl), opad, dl);
	} else if (dl < 64) {
		if (strstr(child_name, "md5") != 0)
			sec_sa_setle((u32 *) (ipad_opad + dl), opad, dl);
		else
			sec_sa_setbe((u32 *) (ipad_opad + dl), opad + 8, dl);
	} else {
		opad += 16;
		sec_sa_setbe((u32 *) (ipad_opad + dl), opad, dl);
	}
	
	APMSEC_DEBUG_DUMP("hmac ipad/opad digest: ", ipad_opad, dl * 2);

	rc = dl * 2;
	
err_alg_hash_key:
	crypto_free_shash(child_hash);
	
err_alg:
	if (input)
		kfree(input);	
	
	return rc;	
}

static int apm_sec_hash_hmac_setkey(struct crypto_ahash *hash,
				const unsigned char *key,
				unsigned int keylen,
				unsigned int sa_len,
				unsigned char ha,
				unsigned char dt)
{
	struct crypto_tfm	   *tfm = crypto_ahash_tfm(hash);
	struct apm_sec_session_ctx *session = crypto_tfm_ctx(tfm);
	int 	rc;

	/* As key length can change, we need to flush the cache sa/token */
	apm_sec_free_sa_tkn_pool(session);
	/* Re-create the SA/token pool */
	rc = apm_sec_create_sa_tkn_pool(session, sa_len, sa_len, 0, 
					  TKN_BASIC_MAX_LEN);
	if (rc != 0)
		goto err_nomem;
	/* Setup SA */
	session->sa->sa_ptr->hash_alg    = ha;
	session->sa->sa_ptr->digest_type = dt;
	session->sa->sa_ptr->ToP	 = SA_TOP_HASH_OUTBOUND;
	session->sa->sa_ptr->context_length = (session->sa_len >> 2) - 2;


	/* Load pre-computed inner and outer digest into SA */
	rc = apm_sec_compute_ipad_opad(tfm, key, keylen,				
				       (u8 *) SA_DIGEST(session->sa->sa_ptr));


	if (rc < 0)
		goto err_nomem;
		
	APMSEC_DEBUG("alg %s setkey", crypto_tfm_alg_name(tfm));
	return 0;

err_nomem:
	apm_sec_free_sa_tkn_pool(session);
	return rc;
}

static int apm_sec_md5_hmac_setkey(struct crypto_ahash *hash, 
				const unsigned char *key,
				unsigned int keylen)
{
	return apm_sec_hash_hmac_setkey(hash, key, keylen, SA_MD5_LEN(1), 
				SA_HASH_ALG_MD5, SA_DIGEST_TYPE_HMAC);
}

static int apm_sec_hash_init(struct ahash_request *req)
{
	struct apm_sec_session_ctx *session = crypto_tfm_ctx(req->base.tfm);
	struct apm_sec_req_ctx     *rctx = ahash_request_ctx(req);
	struct sec_tkn_input_hdr   *in_tkn;
	unsigned char 	new_tkn;
	int	rc;
	int	ds;

	/* In order to support continuous hashing, we need another SA
	   reserved for this req */
	rctx->tkn = NULL;
	rctx->sa  = apm_sec_sa_get(session);
	if (rctx->sa == NULL) {
		rc = -ENOMEM;
		goto err_nomem;
	}
	memcpy(rctx->sa->sa_ptr, session->sa->sa_ptr, session->sa_len);
	/* Now, reserve a token for this request and update fields as
	 * required
	 */
	rctx->tkn = apm_sec_tkn_get(session, &new_tkn);
	if (rctx->tkn == NULL) {
		rc = -ENOMEM;
		goto err_nomem;
	}
	in_tkn = TKN_CTX_INPUT_TKN(rctx->tkn);
	if (new_tkn) {
		ds = crypto_ahash_digestsize(__crypto_ahash_cast(req->base.tfm));
		rctx->tkn->input_tkn_len = sec_tkn_set_hash((u32 *) in_tkn,
				req->nbytes,
				(u32 *) session->sa->sa_ptr, 
				apm_sec_v2hwaddr(rctx->sa->sa_ptr), ds);
		BUG_ON(rctx->tkn->input_tkn_len > session->tkn_input_len);
	} else {
		sec_tkn_update_hash((u32 *) in_tkn, req->nbytes);
	}
	rctx->tkn->sa = rctx->sa;
	rctx->tkn->context = &req->base;
	/* NOTE: The source and destination addresses are transferred to
	         the QM message right before sending to QM hardware via
	         function apm_sec_loadbuffer2qmsg. */
	return 0;

err_nomem:
	if (rctx->tkn)
		apm_sec_tkn_free(session, rctx->tkn);
	if (rctx->sa)
		apm_sec_sa_free(session, rctx->sa);
	return rc;
}

static int apm_sec_hash_update(struct ahash_request *req)
{
	return apm_sec_setup_crypto(&req->base);
}

static int apm_sec_hash_final(struct ahash_request *req)
{
	struct apm_sec_session_ctx *session = crypto_tfm_ctx(req->base.tfm);
	struct apm_sec_req_ctx *rctx = ahash_request_ctx(req);

	/* NOTE: Token is freed when per an operation completed */
	if (rctx->sa) {
		apm_sec_sa_free(session, rctx->sa);
		rctx->sa = NULL;
	}
	return 0;
}

static int apm_sec_hash_digest(struct ahash_request *req)
{
	struct apm_sec_session_ctx *session = crypto_tfm_ctx(req->base.tfm);
	struct apm_sec_req_ctx *rctx = ahash_request_ctx(req);
	struct sec_tkn_input_hdr *in_tkn;
	unsigned char 	new_tkn;
	int	rc;
	int	nbytes;
	void *sa_align;
	u32 sahwaddr;

	APMSEC_TXLOG("hash digest %s nbytes %d",
		req->base.tfm->__crt_alg->cra_name, req->nbytes); 
		
	rctx->sa  = NULL;	/* No private SA */
	rctx->tkn = apm_sec_tkn_get(session, &new_tkn);
	if (rctx->tkn == NULL) {
		rc = -ENOMEM;
		goto errout;
	}
	in_tkn = TKN_CTX_INPUT_TKN(rctx->tkn);
	APMSEC_SATKNLOG("alg %s tkn 0x%p outtkn 0x%p intkn 0x%p",
		req->base.tfm->__crt_alg->cra_name, rctx->tkn, 
		TKN_CTX_RESULT_TKN(rctx->tkn), in_tkn);
	nbytes = req->nbytes ? req->nbytes : 
			       crypto_tfm_alg_blocksize(req->base.tfm);
	sa_align = SA_PTR_ALIGN(session->sa->sa_ptr);
	sahwaddr = apm_sec_v2hwaddr(sa_align);
#if defined (CONFIG_APM867xx)
	if (session->sa->sa_len <= 8)
		sahwaddr = 0;
#endif	
	if (new_tkn) {
		int ds = crypto_ahash_digestsize(
					__crypto_ahash_cast(req->base.tfm));
		rctx->tkn->input_tkn_len = sec_tkn_set_hash((u32 *) in_tkn,
				nbytes, (u32 *) session->sa->sa_ptr, sahwaddr,
				ds);
		BUG_ON(rctx->tkn->input_tkn_len > session->tkn_input_len);
	} else {
		sec_tkn_update_hash((u32 *) in_tkn, nbytes);
	}
	rctx->tkn->sa = session->sa;
	rctx->tkn->context = &req->base;
	/* NOTE: The source and destination addresses are transferred to
	         the QM message right before sending to QM hardware via
	         function apm_sec_loadbuffer2qmsg. */
	rc = apm_sec_setup_crypto(&req->base);
	if (rc == -EINPROGRESS)
		goto errout;
		
	if (rc != 0)
		APMSEC_ERR("hash digest failed error %d", rc);
	apm_sec_tkn_free(session, rctx->tkn);
 errout:

	return rc;	
}

/**
 * SHA1 and SHA2 Algorithm
 *
 */

static int apm_sec_sha1_alg_init(struct crypto_tfm *tfm)
{
	return apm_sec_hash_alg_init(tfm, SA_SHA1_LEN(0),
			SA_HASH_ALG_SHA1, SA_DIGEST_TYPE_INIT);
}

static int apm_sec_sha1_hmac_setkey(struct crypto_ahash *hash, 
				const unsigned char *key,
				unsigned int keylen)
{
	return apm_sec_hash_hmac_setkey(hash, key, keylen,
			SA_SHA1_LEN(1), SA_HASH_ALG_SHA1,
			SA_DIGEST_TYPE_HMAC);
}

static int apm_sec_sha2_alg_init(struct crypto_tfm *tfm)
{
	int ds = crypto_ahash_digestsize(__crypto_ahash_cast(tfm));
	unsigned char 	ha = 0;
	
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
	return apm_sec_hash_alg_init(tfm, SA_SHA2_LEN(ds, 0),
				       ha, SA_DIGEST_TYPE_INIT);
}

static int apm_sec_sha2_hmac_setkey(struct crypto_ahash *hash,
				const unsigned char *key,
				unsigned int keylen)
{
	int 		ds = crypto_ahash_digestsize(hash);
	unsigned char 	ha = 0;

	switch (ds) {
	default:
	case 256/8:
		ha = SA_HASH_ALG_SHA256;
		break;
	case 224/8:
		ha = SA_HASH_ALG_SHA224;
		ds = 32;
		break;
	case 512/8:
		ha = SA_HASH_ALG_SHA512;
		break;
	case 384/8:
		ha = SA_HASH_ALG_SHA384;
		ds = 64;
		break;
	}
	return apm_sec_hash_hmac_setkey(hash, key, keylen,
			SA_SHA2_LEN(ds, 1), ha, SA_DIGEST_TYPE_HMAC);
}

/*
 * For xcbc(aes), no support for init, update, and final hash functions
 *
 */
static int apm_sec_hash_noinit(struct ahash_request *req)
{	/* TBD */
	return -ENOSYS;
}

static int apm_sec_hash_noupdate(struct ahash_request *req)
{	/* TBD */
	return -ENOSYS;
}

static int apm_sec_hash_nofinal(struct ahash_request *req)
{	/* TBD */
	return -ENOSYS;
}

/**
 * 3DES/DES Functions
 *
 */
static int apm_sec_setkey_3des(struct crypto_ablkcipher *cipher,
				 const unsigned char 	*key,
				 unsigned int 	keylen,
				 unsigned char 	cm)
{
	struct crypto_tfm *tfm = crypto_ablkcipher_tfm(cipher);
	struct apm_sec_session_ctx *session = crypto_tfm_ctx(tfm);
	int 	rc = 0;
	
	if (keylen != 8 && keylen != 24) {
		crypto_ablkcipher_set_flags(cipher,
					CRYPTO_TFM_RES_BAD_KEY_LEN);
		return -EINVAL;
	}

	if (keylen == 8) {
		u32 tmp[32];
		rc = des_ekey(tmp, key);
		if (unlikely(rc == 0) &&
		    (tfm->crt_flags & CRYPTO_TFM_REQ_WEAK_KEY)) {
			crypto_ablkcipher_set_flags(cipher,
						    CRYPTO_TFM_RES_WEAK_KEY);
			return -EINVAL;
		}
	}

	/* As key length can change, we need to flush the cache sa/token */
	apm_sec_free_sa_tkn_pool(session);
	/* Re-create SA/token pool */
	rc = apm_sec_create_sa_tkn_pool(session, 
			keylen == 8 ? SA_DES_MAX_LEN : SA_3DES_MAX_LEN,
			keylen == 8 ? SA_DES_LEN(cm) : SA_3DES_LEN(cm), 
			1, TKN_BASIC_MAX_LEN);
	if (rc != 0)
		goto err_nomem;

	/* Setup SA */
	session->sa->sa_ptr->crypto_alg = keylen == 8 ? SA_CRYPTO_ALG_DES :
					                SA_CRYPTO_ALG_3DES;
	session->sa->sa_ptr->cryptomode_feedback = cm;
	session->sa->sa_ptr->key            = 1;
	session->sa->sa_ptr->context_length = (session->sa_len >> 2) - 2;
	/* session->sa->sa_ptr->iv_format = 0; */
	sec_sa_setbe(SA_KEY(session->sa->sa_ptr), key, keylen);
	if (cm == SA_CRYPTO_MODE_AES_3DES_CBC)
		session->sa->sa_ptr->iv03 = 0x3; /* Overwrite via PRNG/Tkn */
	APMSEC_DEBUG("alg %s setkey", crypto_tfm_alg_name(tfm));
	return 0;

err_nomem:
	apm_sec_free_sa_tkn_pool(session);
	return rc;
}

static inline int apm_sec_setkey_3des_cbc(struct crypto_ablkcipher *cipher,
					    const unsigned char *key, 
					    unsigned int keylen)
{
	return apm_sec_setkey_3des(cipher, key, keylen,
				     SA_CRYPTO_MODE_AES_3DES_CBC);
}

static inline int apm_sec_setkey_3des_ecb(struct crypto_ablkcipher *cipher,
					    const unsigned char *key, 
					    unsigned int keylen)
{
	return apm_sec_setkey_3des(cipher, key, keylen,
				     SA_CRYPTO_MODE_AES_3DES_ECB);
}

static inline int apm_sec_setkey_3des_cfb(struct crypto_ablkcipher *cipher,
					    const unsigned char *key, 
					    unsigned int keylen)
{
	return apm_sec_setkey_3des(cipher, key, keylen,
				     SA_CRYPTO_MODE_AES_3DES_CFB);
}

static inline int apm_sec_setkey_3des_ofb(struct crypto_ablkcipher *cipher,
					    const unsigned char *key, 
					    unsigned int keylen)
{
	return apm_sec_setkey_3des(cipher, key, keylen,
				     SA_CRYPTO_MODE_AES_3DES_OFB);
}

static inline int apm_sec_encrypt(struct ablkcipher_request *req)
{
	struct apm_sec_session_ctx *session = crypto_tfm_ctx(req->base.tfm);
	struct apm_sec_req_ctx *rctx = ablkcipher_request_ctx(req);
	struct sec_tkn_input_hdr *in_tkn;
	int		rc;
	unsigned char 	new_tkn;
	void *sa_align;

	if (session->sa->sa_ptr->cryptomode_feedback == SA_CRYPTO_MODE_AES_3DES_CBC && req->nbytes & 0x7)
		return -EINVAL;


	APMSEC_TXLOG("encrypt %s nbytes %d",
		req->base.tfm->__crt_alg->cra_name, req->nbytes);

#ifdef APM_SEC_DEBUG
	if (req->info == NULL &&
	    rctx->sa->sa_ptr->cryptomode_feedback == 
	    SA_CRYPTO_MODE_AES_3DES_CBC) {
		rc = -EINVAL;
		goto errout;
	}
#endif
	rctx->sa  = NULL;	/* No private SA */
	rctx->tkn = apm_sec_tkn_get(session, &new_tkn);
	if (rctx->tkn == NULL) {
		rc = -ENOMEM;
		goto errout;
	}
	in_tkn = TKN_CTX_INPUT_TKN(rctx->tkn);
	sa_align = SA_PTR_ALIGN(session->sa->sa_ptr);
	APMSEC_SATKNLOG("alg %s tkn 0x%p outtkn 0x%p intkn 0x%p",
		req->base.tfm->__crt_alg->cra_name, rctx->tkn, 
		TKN_CTX_RESULT_TKN(rctx->tkn), in_tkn);

	if (new_tkn) {
		rctx->tkn->input_tkn_len = sec_tkn_set_crypt(
				(u32 *) in_tkn,
				req->nbytes,
				apm_sec_v2hwaddr(sa_align),
				(u32 *) session->sa->sa_ptr,
				session->sa->sa_ptr->crypto_alg,
				session->sa->sa_ptr->cryptomode_feedback,
				SA_IV(session->sa->sa_ptr),
				req->info, 1,
				req->base.flags & CRYPTO_TFM_REQ_MERAKI_MODE);
		BUG_ON(rctx->tkn->input_tkn_len > session->tkn_input_len);
		rctx->tkn->sa = session->sa;
	} else {
		sec_tkn_update_crypt((u32 *) in_tkn, req->nbytes,
				(u32 *) session->sa->sa_ptr,
				session->sa->sa_ptr->crypto_alg,
				session->sa->sa_ptr->cryptomode_feedback,
				req->info, 1);
	}
	rctx->tkn->context	= &req->base;
	/* NOTE: The source and destination addresses are transferred to
	         the QM message right before sending to QM hardware via
	         function apm_sec_loadbuffer2qmsg. */
	
	rc = apm_sec_setup_crypto(&req->base);
	if (rc == -EINPROGRESS) {
		goto errout;
	}
		
	if (rc != 0)
		APMSEC_TXLOG("encrypt failed error %d", rc); 
	
	apm_sec_tkn_free(session, rctx->tkn);

 errout:
	return rc;
}

static inline int apm_sec_decrypt(struct ablkcipher_request *req)
{
	struct apm_sec_session_ctx *session = crypto_tfm_ctx(req->base.tfm);
	struct apm_sec_req_ctx *rctx = ablkcipher_request_ctx(req);
	struct sec_tkn_input_hdr *in_tkn;
	int		rc;
	unsigned char 	new_tkn;
	void *sa_align;

	if (session->sa->sa_ptr->cryptomode_feedback == SA_CRYPTO_MODE_AES_3DES_CBC && req->nbytes & 0x7)
		return -EINVAL;

	APMSEC_TXLOG("decrypt %s nbytes %d",
		req->base.tfm->__crt_alg->cra_name, req->nbytes); 
#ifdef APM_SEC_DEBUG
	if (req->info == NULL &&
	    rctx->sa->sa_ptr->cryptomode_feedback == 
	    SA_CRYPTO_MODE_AES_3DES_CBC) {
		rc = -EINVAL;
		goto errout;
	}
#endif
	rctx->sa  = NULL;	/* No private SA */
	rctx->tkn = apm_sec_tkn_get(session, &new_tkn);
	if (rctx->tkn == NULL) {
		rc = -ENOMEM;
		goto errout;
	}
	in_tkn = TKN_CTX_INPUT_TKN(rctx->tkn);
	sa_align = SA_PTR_ALIGN(session->sa->sa_ptr);
	APMSEC_SATKNLOG("alg %s tkn 0x%p outtkn 0x%p intkn 0x%p",
		req->base.tfm->__crt_alg->cra_name, rctx->tkn, 
		TKN_CTX_RESULT_TKN(rctx->tkn), in_tkn);
	if (new_tkn) {
		rctx->tkn->input_tkn_len = sec_tkn_set_crypt(
				(u32 *) in_tkn,
				req->nbytes,
				apm_sec_v2hwaddr(sa_align),
				(u32 *) session->sa->sa_ptr,
				session->sa->sa_ptr->crypto_alg,
				session->sa->sa_ptr->cryptomode_feedback,
				SA_IV(session->sa->sa_ptr), /* Nonce */
				req->info, 0,	            /* IV */
				req->base.flags & CRYPTO_TFM_REQ_MERAKI_MODE);
		BUG_ON(rctx->tkn->input_tkn_len > session->tkn_input_len);
		rctx->tkn->sa = session->sa;
	} else {
		sec_tkn_update_crypt((u32 *) in_tkn, req->nbytes,
				(u32 *) session->sa->sa_ptr,
				session->sa->sa_ptr->crypto_alg,
				session->sa->sa_ptr->cryptomode_feedback,
				req->info, 0);	/* IV */
	}
	rctx->tkn->context     = &req->base;
	/* NOTE: The source and destination addresses are transferred to
	         the QM message right before sending to QM hardware via
	         function apm_sec_loadbuffer2qmsg. */
	
	rc = apm_sec_setup_crypto(&req->base);
	if (rc == -EINPROGRESS)
		goto errout;
		
	if (rc != 0)
		APMSEC_TXLOG("decrypt failed error %d", rc); 
	
	apm_sec_tkn_free(session, rctx->tkn);
 errout:
	return rc;	
}

/**
 * AES Functions
 *
 */
static int apm_sec_setkey_aes(struct crypto_ablkcipher *cipher,
				const unsigned char *key, unsigned int keylen,
				unsigned char cm)
{
	struct crypto_tfm *tfm = crypto_ablkcipher_tfm(cipher);
	struct apm_sec_session_ctx *session = crypto_tfm_ctx(tfm);
	const unsigned char  *nonce = NULL;
	int 	rc;

	if (cm == SA_CRYPTO_MODE_AES_3DES_CTR ||
	    cm == SA_CRYPTO_MODE_AES_3DES_CTR_LOAD_REUSE) {
		if (keylen != 256/8+4 && keylen != 128/8+4 && 
		    keylen != 192/8+4) {
			crypto_ablkcipher_set_flags(cipher,
					CRYPTO_TFM_RES_BAD_KEY_LEN);
			return -1;
		}
		keylen -= 4;
		nonce = key + keylen;
	} else if (keylen != 256/8 && keylen != 128/8 && keylen != 192/8) {
		crypto_ablkcipher_set_flags(cipher, 
					CRYPTO_TFM_RES_BAD_KEY_LEN);
		return -1;
	}
	/* As key length can change, we need to flush the cache sa/token */
	apm_sec_free_sa_tkn_pool(session);
	/* Now, create SA, token and cache again */
	rc = apm_sec_create_sa_tkn_pool(session, SA_AES_MAX_LEN(cm),
			SA_AES_LEN(keylen, cm), 0, TKN_BASIC_MAX_LEN);
	if (rc != 0)
		goto err_nomem;

	/* Setup SA  */
	if (keylen >= 256 / 8)
		session->sa->sa_ptr->crypto_alg = SA_CRYPTO_ALG_AES256;
	else if (keylen >= 192 / 8)
		session->sa->sa_ptr->crypto_alg = SA_CRYPTO_ALG_AES192;
	else
		session->sa->sa_ptr->crypto_alg = SA_CRYPTO_ALG_AES128;
	session->sa->sa_ptr->cryptomode_feedback = cm;
	session->sa->sa_ptr->key		= 1;
	session->sa->sa_ptr->context_length	= (session->sa_len >> 2) - 2;
	sec_sa_setbe(SA_KEY(session->sa->sa_ptr), key, keylen);
	/* session->sa->sa_ptr->iv_format	= 0; */
	if (cm == SA_CRYPTO_MODE_AES_3DES_CTR ||
	    cm == SA_CRYPTO_MODE_AES_3DES_CTR_LOAD_REUSE) {
		//sec_sec_sa_setbe2le(SA_IV(session->sa->sa_ptr), *nonce);
		memcpy(SA_IV(session->sa->sa_ptr), nonce, 4);
		sec_sa_set32be(SA_IV(session->sa->sa_ptr) + 3, 1);
		/* IV0,1,2,3 in SA but will over written by token */
		session->sa->sa_ptr->iv03 = 0xF;
	} else if (cm == SA_CRYPTO_MODE_AES_3DES_CBC) {
		/* IV0,1,2,3 in SA but will over written by token */
		session->sa->sa_ptr->iv03 = 0xF;
	}
	
	APMSEC_DEBUG("alg %s setkey", crypto_tfm_alg_name(tfm));
	return 0;

err_nomem:
	apm_sec_free_sa_tkn_pool(session);

	return rc;
}

static inline int apm_sec_setkey_aes_ecb(struct crypto_ablkcipher *cipher,
					   const unsigned char *key, 
					   unsigned int keylen)
{
	return apm_sec_setkey_aes(cipher, key, keylen,
				    SA_CRYPTO_MODE_AES_3DES_ECB);
}

static inline int apm_sec_setkey_aes_cbc(struct crypto_ablkcipher *cipher,
					   const unsigned char *key, 
					   unsigned int keylen)
{
	return apm_sec_setkey_aes(cipher, key, keylen,
				    SA_CRYPTO_MODE_AES_3DES_CBC);
}

static inline int apm_sec_setkey_aes_ctr(struct crypto_ablkcipher *cipher,
					   const unsigned char *key, 
					   unsigned int keylen)
{
	return apm_sec_setkey_aes(cipher, key, keylen,
				    SA_CRYPTO_MODE_AES_3DES_CTR);
}

/**
 * SetAuthSize Functions
 *
 */
static int apm_sec_setauthsize_aes(struct crypto_aead *ciper,
				     unsigned int authsize)
{
	struct aead_tfm *tfm = crypto_aead_crt(ciper);

	switch (authsize) {
	case 8:
	case 12:
	case 16:
		break;
	default:
		return -EINVAL;
	}

	tfm->authsize = authsize;
	return 0;
}

static int apm_sec_setauthsize_aes_ccm(struct crypto_aead *ciper,
				     unsigned int authsize)
{
	struct aead_tfm *tfm = crypto_aead_crt(ciper);

	switch (authsize) {
	case 8:
	case 10:
	case 12:
	case 16:
		break;
	default:
		return -EINVAL;
	}

	tfm->authsize = authsize;
	return 0;
}

/**
 * XCBC Key Generation Rotuine
 *
 */
int apm_sec_xcbc_digest(const unsigned char *key,
				unsigned int keylen,
				unsigned char *ipad_opad)

{
	struct crypto_blkcipher *blk_tfm = NULL;
	struct blkcipher_desc   blk_desc;
	struct scatterlist 	sg[1];
	char 	block128[16];
	int j;
	int rc;

	/* Load K2, K3, and K1 digest */
	blk_tfm = crypto_alloc_blkcipher("ecb(aes)", 0, 0);
	if (IS_ERR(blk_tfm)) {
		APMSEC_ERR("failed to load helper transform for ecb(aes) "
				"error %ld", PTR_ERR(blk_tfm));
		return -EINVAL;
	}
	blk_desc.tfm   = blk_tfm;
	blk_desc.flags = 0;
	rc = crypto_blkcipher_setkey(blk_tfm, key, keylen);
	if (rc != 0) {
		APMSEC_ERR("failed to set helper transform key for ecb(aes) "
				"error %d", rc);
		rc = -EINVAL;
		goto err;
	}

	for (j = 2; j < 5; j++) {
		int i;
		for (i = 0; i < 128/8; i++)
			block128[i] = j == 4 ? 0x01 : j;
		sg_init_one(sg, block128, 128/8);
		rc = crypto_blkcipher_encrypt(&blk_desc, sg, sg, 128/8);
		if (rc != 0) {
			APMSEC_ERR("failed to encrypt hash key for "
					"xcbc key error %d", rc);
			rc = -ENOMEM;
			goto err;
		}
		APMSEC_DEBUG("xcbc key j %d", j);
 		APMSEC_DEBUG_DUMP("Each key: ", block128, 16);
 
  		sec_sa_setle((unsigned int *) ipad_opad, block128, 128/8);
 		ipad_opad += 16;
	}
err:
	if (blk_tfm)
		crypto_free_blkcipher(blk_tfm);
	return rc;

}

static int apm_sec_sha2_xcbc_setkey(struct crypto_ahash *hash,
				const unsigned char *key,
				unsigned int keylen)
{
	struct crypto_tfm    	*tfm = crypto_ahash_tfm(hash);
	struct apm_sec_session_ctx *session = crypto_tfm_ctx(tfm);
	int 	rc;

	if (keylen != 128/8) {
		crypto_ahash_set_flags(hash, CRYPTO_TFM_RES_BAD_KEY_LEN);
		return -1;
	}

	/* As key length can change, we need to flush the cache sa/token */
	apm_sec_free_sa_tkn_pool(session);
	/* Re-create the SA/token pool */
	rc = apm_sec_create_sa_tkn_pool(session, 
			SA_AES_XCBC_LEN, SA_AES_XCBC_LEN,
			0, TKN_BASIC_MAX_LEN);
	if (rc != 0)
		return -ENOMEM;

	session->sa->sa_ptr->hash_alg    = SA_HASH_ALG_XCBC128;
	session->sa->sa_ptr->digest_type = SA_DIGEST_TYPE_HASH;
	session->sa->sa_ptr->ToP	     = SA_TOP_HASH_OUTBOUND;
	session->sa->sa_ptr->context_length = (session->sa_len >> 2) - 2;

	/* Load pre-computed key value into SA */
	rc = apm_sec_xcbc_digest(key, keylen, 
				(u8 *) SA_DIGEST(session->sa->sa_ptr));
	
	APMSEC_DEBUG("alg %s setkey", crypto_tfm_alg_name(tfm));
	return rc;
}

/* ARC4 Functions */
static int __apm_sec_setkey_arc4(struct crypto_ablkcipher *cipher,
			const unsigned char *key, unsigned int keylen,
			unsigned char cm)
{
	struct crypto_tfm *tfm = crypto_ablkcipher_tfm(cipher);
	struct apm_sec_session_ctx *session = crypto_tfm_ctx(tfm);
	int rc;

	if (keylen < 5 || keylen > 16) {
		crypto_ablkcipher_set_flags(cipher, CRYPTO_TFM_RES_BAD_KEY_LEN);
		return -1;
	}
	/* As key length can change, we need to flush the cache sa/token */
	apm_sec_free_sa_tkn_pool(session);
	/* Re-create SA and token cache */
	rc = apm_sec_create_sa_tkn_pool(session, SA_ARC4_LEN(cm), 
			SA_ARC4_LEN(cm) + 
				(cm == SA_ARC4_STATEFULL ? 256 : 0),
			0, TKN_BASIC_MAX_LEN);
	if (rc != 0)
		goto err_nomem;

	sec_sa_setbe(SA_KEY(session->sa->sa_ptr), key, keylen);
	session->sa->sa_ptr->crypto_alg 	     = SA_CRYPTO_ALG_ARC4;
	session->sa->sa_ptr->cryptomode_feedback = keylen;
	session->sa->sa_ptr->state_selection     = cm;
	session->sa->sa_ptr->ij_pointer	     = cm;
	session->sa->sa_ptr->crypto_store	     = 1;
	session->sa->sa_ptr->key		     = 1;
	session->sa->sa_ptr->pkt_based_options   = 0;
	session->sa->sa_ptr->context_length = (session->sa_len >> 2) - 2;
	if (cm == SA_ARC4_STATEFULL) {
		int i;
		u32 j = 0;
		u32 k = 0;
		u32 *arc4_state = (u32 *) ((u8 *) session->sa->sa_ptr + 
					SA_ARC4_LEN(SA_ARC4_STATEFULL));
		sec_sa_set32le(SA_ARC4(session->sa->sa_ptr),
				apm_sec_v2hwaddr(arc4_state));
		sec_sa_set32le(SA_ARC4(session->sa->sa_ptr) + 1, 0);
		/* Initialize ARC4 state record */
		for (i = 0; i < 256; i++)
			arc4_state[i] = i;
		for (i = 0; i < 256; i++) {			
			u32 temp = arc4_state[i];
			j = (j + temp + key[k]) & 0xff;
			arc4_state[i] = arc4_state[j];
			arc4_state[j] = temp;
			if (++k >= keylen)
				k = 0;
		}
	}

	APMSEC_DEBUG("alg %s (state%s) setkey", crypto_tfm_alg_name(tfm), 
			cm == SA_ARC4_STATEFULL ? "full" : "less");
	return 0;

err_nomem:
	apm_sec_free_sa_tkn_pool(session);

	return rc;
}

static int apm_sec_setkey_arc4_stateless(struct crypto_ablkcipher *cipher,
				 const unsigned char * key, unsigned int keylen)
{
	return __apm_sec_setkey_arc4(cipher, key, keylen, 
					SA_ARC4_STATELESS);
}

static int apm_sec_setkey_arc4_statefull(struct crypto_ablkcipher *cipher,
				 const unsigned char * key, unsigned int keylen)
{
	return __apm_sec_setkey_arc4(cipher, key, keylen, 
					SA_ARC4_STATEFULL);
}

static inline int apm_sec_arc4_encrypt(struct ablkcipher_request *req)
{
	struct apm_sec_session_ctx *session  = crypto_tfm_ctx(req->base.tfm);
	struct apm_sec_req_ctx *rctx = ablkcipher_request_ctx(req);
	struct sec_tkn_input_hdr *in_tkn;
	unsigned char new_tkn;
	int rc;
	void *sa_align;

	rctx->sa  = NULL;
	rctx->tkn = apm_sec_tkn_get(session, &new_tkn);
	if (rctx->tkn == NULL)
		return -ENOMEM;
	in_tkn = TKN_CTX_INPUT_TKN(rctx->tkn);
	sa_align = SA_PTR_ALIGN(session->sa->sa_ptr);
	if (new_tkn) {
		rctx->tkn->input_tkn_len = sec_tkn_set_arc4_crypt(
					(u32 *) in_tkn,
					req->nbytes,
					apm_sec_v2hwaddr(sa_align),
					(u32 *) session->sa->sa_ptr, 1);
		BUG_ON(rctx->tkn->input_tkn_len > session->tkn_input_len);
		rctx->tkn->sa		= session->sa;
	} else {
		sec_tkn_update_arc4((u32 *) in_tkn, req->nbytes);
	}
	rctx->tkn->context	= &req->base;
	
	rc = apm_sec_setup_crypto(&req->base);
	if (rc == -EINPROGRESS)
		return rc; 
		
	if (rc != 0)
		APMSEC_TXLOG("encrypt failed error %d", rc); 
	
	apm_sec_tkn_free(session, rctx->tkn);
	return rc;
}

static inline int apm_sec_arc4_decrypt(struct ablkcipher_request *req)
{
	struct apm_sec_session_ctx *session  = crypto_tfm_ctx(req->base.tfm);
	struct apm_sec_req_ctx *rctx = ablkcipher_request_ctx(req);
	struct sec_tkn_input_hdr *in_tkn;
	unsigned char new_tkn;
	int rc;
	void *sa_align;

	rctx->sa  = NULL;
	rctx->tkn = apm_sec_tkn_get(session, &new_tkn);
	if (rctx->tkn == NULL)
		return -ENOMEM;
	in_tkn = TKN_CTX_INPUT_TKN(rctx->tkn);
	sa_align = SA_PTR_ALIGN(session->sa->sa_ptr);
	if (new_tkn) {
		rctx->tkn->input_tkn_len = sec_tkn_set_arc4_crypt(
				(u32 *) in_tkn,
				req->nbytes,
				apm_sec_v2hwaddr(sa_align),
				(u32 *) session->sa->sa_ptr, 0);
		BUG_ON(rctx->tkn->input_tkn_len > session->tkn_input_len);
		rctx->tkn->sa = session->sa;
	} else {
		sec_tkn_update_arc4((u32 *) in_tkn, req->nbytes);
	}
	rctx->tkn->context = &req->base;
	
	rc = apm_sec_setup_crypto(&req->base);	
	if (rc == -EINPROGRESS)
		return rc; 
		
	if (rc != 0)
		APMSEC_TXLOG("encrypt failed error %d", rc); 
	
	apm_sec_tkn_free(session, rctx->tkn);
	return rc;
}
int ccm_count = 0;
/**
 * Basic CCM Algorithm Routines
 *
 */
static int apm_sec_setkey_aes_ccm(struct crypto_aead *cipher, 
				const unsigned char *key,
				unsigned int keylen)
{
	struct crypto_tfm *tfm = crypto_aead_tfm(cipher);
	struct apm_sec_session_ctx *session = crypto_tfm_ctx(tfm);
	int rc = 0;

	if (keylen != 128/8 && keylen != 192/8 && keylen != 256/8) {
		crypto_aead_set_flags(cipher, CRYPTO_TFM_RES_BAD_KEY_LEN);
		return -EINVAL;
	}
	/* Key changed, musts release all pre-allocate token and sa */
	apm_sec_free_sa_tkn_pool(session);
	/* Re-create token and SA pool */
	rc = apm_sec_create_sa_tkn_pool(session, 
			SA_AES_CCM_MAX_LEN, SA_AES_CCM_LEN(keylen),
			0, TKN_AES_CCM_MAX);
	if (rc != 0)
		goto err_nomem;

	session->sa->sa_ptr->hash_alg   	= SA_HASH_ALG_XCBC128;
	session->sa->sa_ptr->crypto_alg 	= SA_CRYPTO_ALG_AES128;
	session->sa->sa_ptr->digest_type 	= SA_DIGEST_TYPE_HASH;
	session->sa->sa_ptr->key 		= 1;
	session->sa->sa_ptr->cryptomode_feedback =
				SA_CRYPTO_MODE_AES_3DES_CTR_LOAD_REUSE;
	session->sa->sa_ptr->spi		= 0;
	session->sa->sa_ptr->seq		= SA_SEQ_NO_COUNTER;
	session->sa->sa_ptr->mask		= SA_MASK_NONE;
	session->sa->sa_ptr->iv03		= 0xF;
	session->sa->sa_ptr->enc_hash_result= 1;
	session->sa->sa_ptr->context_length = (session->sa_len >> 2) - 2;

	sec_sa_setbe(SA_KEY(session->sa->sa_ptr), key, keylen);
	memset(SA_DIGEST(session->sa->sa_ptr), 0, 16 * 2);
	sec_sa_setle(SA_DIGEST(session->sa->sa_ptr) + 8, key, keylen);
	/* clear crypto block A0 */
	memset(SA_IV(session->sa->sa_ptr), 0, 16);
		
	return rc;

err_nomem:
	apm_sec_free_sa_tkn_pool(session);
	return rc;
}

static int apm_sec_encrypt_aes_ccm(struct aead_request *req)
{
	struct apm_sec_session_ctx *session = crypto_tfm_ctx(req->base.tfm);
	struct apm_sec_req_ctx *rctx = aead_request_ctx(req);
	struct crypto_aead 	 *aead = crypto_aead_reqtfm(req);
	struct sec_tkn_input_hdr *in_tkn;
	unsigned char	iv_B0[16];
	unsigned char	adata_length[8];
	u32		adata_length_length 	= 0;
	unsigned char	*adata 			= NULL;
	unsigned char	new_tkn;
	int		rc;
	void *sa_align;
#if defined(APM_SEC_TXDEBUG) || defined(APM_SEC_SATKNDEBUG)	
	struct crypto_alg 	*alg = req->base.tfm->__crt_alg;
#endif
	APMSEC_TXLOG("encrypt ccm %s nbytes %d", alg->cra_name, req->cryptlen);

	/* Support only AAD data size of 0, 8, 12, and 16 */
#ifdef APM_SEC_DEBUG
	if (req->assoclen != 0 && req->assoclen != 8 &&
	    req->assoclen != 12 && req->assoclen != 16) {
		APMSEC_TXLOG("Algorithm AES-CCM unsupported adata length %d",
			req->assoclen);
		return -EINVAL;
	}
	/* Support only counter field length of 2 and 4 bytes */
	if (req->iv[0] != 1 && req->iv[0] != 3) {
		APMSEC_TXLOG("algorithm AES-CCM unsupported "
				"counter length %d",
			req->iv[0] & 0x7);
		return -EINVAL;
	}
#endif
	/* Allocate SA and token for this request */
	rctx->tkn = apm_sec_tkn_get(session, &new_tkn);
	if (rctx->tkn == NULL)
		return -ENOMEM;
	rctx->sa  = apm_sec_sa_get(session);
	if (rctx->sa == NULL) {
		rc = -ENOMEM;
		goto err_enc_aes_ccm;
	}
	memcpy(rctx->sa->sa_ptr, session->sa->sa_ptr, session->sa_len);
	
	/* Form IV B0 salt and flags for hashing */	
	memcpy(iv_B0, req->iv, 16);
	iv_B0[0] |= (8 * ((crypto_aead_authsize(aead) - 2) / 2));
	if (req->assoclen)
		iv_B0[0] |= 64;
	rc = sec_sa_set_aes_ccm_msg_len(iv_B0 + 16 - (req->iv[0] + 1),
					req->cryptlen, req->iv[0] + 1);
	if (rc != 0) {
		rc = -EOVERFLOW;
		goto err_enc_aes_ccm;
	}

	/* Form adata length field and its length */
	if (req->assoclen)
		adata_length_length = sec_sa_set_aes_ccm_adata(adata_length,
							       req->assoclen);
	/* Note: rfc 3610 and NIST 800-38C require counter of
	 * zero to encrypt auth tag.
	 */
	memset(req->iv + 15 - req->iv[0], 0, req->iv[0] + 1);
	sec_sa_setbe(SA_IV(rctx->sa->sa_ptr), req->iv, 16);
	
	in_tkn = TKN_CTX_INPUT_TKN(rctx->tkn);
	if (req->assoclen) {
		adata = kmap_atomic(sg_page(&req->assoc[0])) +
						req->assoc[0].offset;
	}
	APMSEC_SATKNLOG("alg %s tkn 0x%p outtkn 0x%p intkn 0x%p",
			alg->cra_name, rctx->tkn,
			TKN_CTX_RESULT_TKN(rctx->tkn), in_tkn);
	sa_align = SA_PTR_ALIGN(rctx->sa->sa_ptr);
	if (new_tkn) {
		rctx->tkn->input_tkn_len = sec_tkn_set_crypt_aes_ccm(
					(u32 *) in_tkn,
					req->cryptlen,
					apm_sec_v2hwaddr(sa_align),
					(u32 *) rctx->sa->sa_ptr,
					crypto_aead_authsize(aead),
					(u32 *) iv_B0,
					adata_length, adata_length_length,
					adata, req->assoclen, 1);
	} else {
		sec_tkn_update_crypt_aes_ccm((u32 *) in_tkn,
					req->cryptlen,
					apm_sec_v2hwaddr(sa_align),
					(u32 *) rctx->sa->sa_ptr,
					crypto_aead_authsize(aead),
					(u32 *) iv_B0,
					adata_length, adata_length_length,
					adata, req->assoclen, 1);
	}
	if (adata)
		kunmap_atomic(adata);
	
	BUG_ON(rctx->tkn->input_tkn_len > session->tkn_input_len);
	session->sa_flush_done = 0;
	rctx->tkn->sa		= rctx->sa;
	rctx->tkn->context      = &req->base;
	rctx->tkn->dest_nbytes = req->cryptlen + crypto_aead_authsize(aead);
	/* NOTE: The source and destination addresses are transferred to
	         the QM message right before sending to QM hardware via
	         function apm_sec_loadbuffer2qmsg. */
	
	rc = apm_sec_setup_crypto(&req->base);
	if (rc == -EINPROGRESS)
		return rc; 
		
	if (rc != 0)
		APMSEC_TXLOG("encrypt ccm failed error %d", rc); 

err_enc_aes_ccm:
	if (rctx->tkn)
		apm_sec_tkn_free(session, rctx->tkn);
	if (rctx->sa)
		apm_sec_sa_free(session, rctx->sa);
	return rc;
}

static int apm_sec_decrypt_aes_ccm(struct aead_request *req)
{
	struct apm_sec_session_ctx *session = crypto_tfm_ctx(req->base.tfm);
	struct apm_sec_req_ctx *rctx = aead_request_ctx(req);
	struct crypto_aead *aead = crypto_aead_reqtfm(req);
	struct sec_tkn_input_hdr *in_tkn;
	unsigned char	iv_B0[16];
	unsigned char	adata_length[8];
	u32		adata_length_length 	= 0;
	unsigned char	*adata 			= NULL;
	unsigned char	new_tkn;
	int		rc;
	void *sa_align;
	
#if defined(APM_SEC_SATKNDEBUG)
	struct crypto_alg 	*alg = req->base.tfm->__crt_alg;
#endif
	APMSEC_TXLOG("decrypt ccm %s nbytes %d",
			req->base.tfm->__crt_alg->cra_name, req->cryptlen); 

#ifdef APM_SEC_DEBUG
	/* Support only AAD data size of 0, 8, 12, and 16 */
	if (req->assoclen != 0 && req->assoclen != 8 &&
	    req->assoclen != 12 && req->assoclen != 16) {
		APMSEC_RXLOG("algorithm AES-CCM unsupported adata "
				"length %d", req->assoclen);
		return -EINVAL;
	}
	/* Support only counter field length of 2 and 4 bytes */
	if (req->iv[0] != 1 && req->iv[0] != 3) {
		APMSEC_RXLOG("algorithm AES-CCM unsupported counter "
				"length %d", req->iv[0] & 0x7);
		return -EINVAL;
	}
#endif
	/* Allocate SA and token for this request */
	rctx->tkn = apm_sec_tkn_get(session, &new_tkn);
	if (rctx->tkn == NULL)
		return -ENOMEM;
	rctx->sa  = apm_sec_sa_get(session);
	if (rctx->sa == NULL) {
		rc = -ENOMEM;
		goto err_enc_aes_ccm;
	}
	memcpy(rctx->sa->sa_ptr, session->sa->sa_ptr, session->sa_len);

	/* Form IV B0 salt and flags for hashing */
	memcpy(iv_B0, req->iv, 16);
	/* Note: rfc 3610 and NIST 800-38C require counter of
	 * zero to encrypt auth tag.
	 */
	memset(req->iv + 15 - req->iv[0], 0, req->iv[0] + 1);
	sec_sa_setbe(SA_IV(rctx->sa->sa_ptr), req->iv, 16);

	/* Decrypt format control info per RFC 3610 and NIST Special Publication
	   800-38C */
	iv_B0[0] |= (8 * ((crypto_aead_authsize(aead) - 2) / 2));
	if (req->assoclen)
		iv_B0[0] |= 64;
	rc = sec_sa_set_aes_ccm_msg_len(iv_B0 + 16 - (req->iv[0] + 1),
				req->cryptlen - crypto_aead_authsize(aead), 
				req->iv[0] + 1);
	if (rc != 0) {
		rc = -EOVERFLOW;
		goto err_enc_aes_ccm;
	}
	/* Form adata length field and its length */
	if (req->assoclen)
		adata_length_length = sec_sa_set_aes_ccm_adata(adata_length,
							req->assoclen);

	in_tkn = TKN_CTX_INPUT_TKN(rctx->tkn);
	if (req->assoclen) {
		adata = kmap_atomic(sg_page(&req->assoc[0])) +
				req->assoc[0].offset;
	}
	APMSEC_SATKNLOG("alg %s tkn 0x%p outtkn 0x%p intkn 0x%p",
			alg->cra_name, rctx->tkn, 
			TKN_CTX_RESULT_TKN(rctx->tkn), in_tkn);
	sa_align = SA_PTR_ALIGN(rctx->sa->sa_ptr);
	if (new_tkn) {
		rctx->tkn->input_tkn_len = sec_tkn_set_crypt_aes_ccm(
					(u32 *) in_tkn,
					req->cryptlen,
					apm_sec_v2hwaddr(sa_align),
					(u32 *) rctx->sa->sa_ptr,
					crypto_aead_authsize(aead),
					(u32 *) iv_B0,
					adata_length, adata_length_length,
					adata, req->assoclen, 0);
		BUG_ON(rctx->tkn->input_tkn_len > session->tkn_input_len);
	} else {
		sec_tkn_update_crypt_aes_ccm((u32 *) in_tkn,
					req->cryptlen,
					apm_sec_v2hwaddr(sa_align),
					(u32 *) rctx->sa->sa_ptr,
					crypto_aead_authsize(aead),
					(u32 *) iv_B0,
					adata_length, adata_length_length,
					adata, req->assoclen, 0);
	}
	if (adata)
		kunmap_atomic(adata);
	session->sa_flush_done = 0;
	rctx->tkn->sa	       = rctx->sa;
	rctx->tkn->context     = &req->base;
	rctx->tkn->dest_nbytes  = req->cryptlen;

	rc = apm_sec_setup_crypto(&req->base);
	if (rc == -EINPROGRESS)
		return rc; 
		
	if (rc != 0)
		APMSEC_TXLOG(" decrypt ccm failed error %d",
				rc); 

err_enc_aes_ccm:
	if (rctx->tkn)
		apm_sec_tkn_free(session, rctx->tkn);
	if (rctx->sa)
		apm_sec_sa_free(session, rctx->sa);
	return rc;
}

static int apm_sec_givencrypt_aes_ccm(struct aead_givcrypt_request *req)
{	/* TBD */
	return -ENOSYS;
}

static int apm_sec_givdecrypt_aes_ccm(struct aead_givcrypt_request *req)
{	/* TBD */
	return -ENOSYS;
}

/**
 * GHASH Calculation Routine
 *
 */
int apm_compute_gcm_hash_key(struct crypto_tfm *tfm,
				const u8 * key, unsigned int keylen)
{
	struct apm_sec_session_ctx *session = crypto_tfm_ctx(tfm);
	struct scatterlist 	sg[1];
	struct crypto_blkcipher *aes_tfm = NULL;
	struct blkcipher_desc 	desc;
	int 	bs = crypto_tfm_alg_blocksize(tfm);
	unsigned char 	digest[bs];
	u32 	*sa_hash;
	int 	rc;

	/* Load pre-computed key value into SA */
	aes_tfm = crypto_alloc_blkcipher("ecb(aes)", 0, CRYPTO_ALG_ASYNC);
	if (IS_ERR(aes_tfm)) {
		rc = PTR_ERR(aes_tfm);
		APMSEC_ERR("failed to load transform for ecb(aes) error %d", 
				rc);
		return rc;
	}
	desc.tfm    = aes_tfm;
	desc.flags  = 0;
	
	rc = crypto_blkcipher_setkey(desc.tfm, key, keylen);
	if (rc) {
		APMSEC_ERR("failed to load gcm key error %d", rc);
		goto err_alg;
	}
	memset(digest, 0x00, 16);
	sg_init_one(&sg[0], digest, 16);
	rc = crypto_blkcipher_encrypt(&desc, sg, sg, bs);
	if (rc < 0) {
		APMSEC_ERR("failed to hash gcm key error %d", rc);
		goto err_alg;
	}
	sa_hash = SA_DIGEST(session->sa->sa_ptr);

	sec_sa_setle(sa_hash, digest, 16);

err_alg:
	if (aes_tfm)
		crypto_free_blkcipher(aes_tfm);

	return rc;
}

/**
 * Basic Algorithm AES-GCM Functions
 *
 */
static inline int apm_sec_setkey_aes_gcm(struct crypto_aead *cipher,
					   const unsigned char *key, 
					   unsigned int keylen)
{
	struct crypto_tfm *tfm = crypto_aead_tfm(cipher);
	struct apm_sec_session_ctx *session = crypto_tfm_ctx(tfm);
	int 	rc = 0;

	if (keylen != 128/8 && keylen != 192/8 && keylen != 256/8) {
		crypto_aead_set_flags(cipher, CRYPTO_TFM_RES_BAD_KEY_LEN);
		return -EINVAL;
	}
	/* As key length can change, we need to flush the cache sa/token */
	apm_sec_free_sa_tkn_pool(session);
	/* Re-create the SA/Token pool */	
	rc = apm_sec_create_sa_tkn_pool(session, 
					SA_AES_GCM_MAX_LEN, SA_AES_GCM_LEN(keylen),
			0, TKN_AES_GCM_MAX);
	if (rc != 0)
		goto err_nomem;

	/* Setup SA - Fill SA with proper value. For AES-GCM, we will
	   have an SA for each request */
	session->sa->sa_ptr->hash_alg   	= SA_HASH_ALG_GHASH;
	session->sa->sa_ptr->digest_type 	= SA_DIGEST_TYPE_HASH;
	session->sa->sa_ptr->key 		= 1;
	switch(keylen) {
	case 128/8:
		session->sa->sa_ptr->crypto_alg = SA_CRYPTO_ALG_AES128;
		break;
	case 192/8:
		session->sa->sa_ptr->crypto_alg = SA_CRYPTO_ALG_AES192;
		break;
	case 256/8:
		session->sa->sa_ptr->crypto_alg = SA_CRYPTO_ALG_AES256;
		break;
	}
	session->sa->sa_ptr->cryptomode_feedback = SA_CRYPTO_MODE_AES_3DES_CTR;

	session->sa->sa_ptr->mask	= SA_MASK_NONE;
	session->sa->sa_ptr->iv_format	= SA_IV_FORMAT_COUNTER_MODE;
	session->sa->sa_ptr->iv03	= 0x7;
	session->sa->sa_ptr->enc_hash_result= 1;
	session->sa->sa_ptr->context_length = (session->sa_len >> 2) - 2;

	sec_sa_setbe(SA_KEY(session->sa->sa_ptr), key, keylen);
	rc = apm_compute_gcm_hash_key(tfm, key, keylen);
	if (rc < 0)
		goto err;

	APMSEC_DEBUG("alg %s setkey", crypto_tfm_alg_name(tfm));
	return rc;

err_nomem:
err:
	apm_sec_free_sa_tkn_pool(session);
	APMSEC_DEBUG("alg %s setkey failed rc %d", 
			crypto_tfm_alg_name(tfm), rc);
	return rc;
}

static int apm_sec_encrypt_aes_gcm(struct aead_request *req)
{
	struct apm_sec_session_ctx *session = crypto_tfm_ctx(req->base.tfm);
	struct apm_sec_req_ctx *rctx = aead_request_ctx(req);
	struct crypto_aead *aead = crypto_aead_reqtfm(req);
	struct sec_tkn_input_hdr *in_tkn;
	unsigned char 	*adata;
	unsigned char	new_tkn;
	int icvsize, adata_flag;
	int rc;
	void *sa_align;
 	
	/* Allocate token and SA for this request */
	rctx->tkn = apm_sec_tkn_get(session, &new_tkn);
	if (rctx->tkn == NULL)
		return -ENOMEM;

	rctx->sa  = apm_sec_sa_get(session);
	if (rctx->sa == NULL) {
		rc = -ENOMEM;
		goto err_enc_aes_gcm;
	}

	/* Form SA - SPI, SEQ, and Nonce */
	memcpy(rctx->sa->sa_ptr, session->sa->sa_ptr, session->sa_len);
	sec_sa_setbe(SA_IV(rctx->sa->sa_ptr), req->iv, 12);
	if (apm_lsec_sg_scattered(req->assoclen, req->assoc)) {
		adata = kzalloc(req->assoclen, GFP_KERNEL);
		if (!adata) {
			rc = -ENOMEM;
			goto err_enc_aes_gcm;
		}
		apm_lsec_sg2buf(req->assoc, req->assoclen, adata);
		adata_flag = 0;
		/* Check if free as completion? */
	} else {
		adata = kmap_atomic(sg_page(&req->assoc[0])) +
				req->assoc[0].offset;
		adata_flag = 1;
	}

	icvsize = crypto_aead_authsize(aead);
	in_tkn = TKN_CTX_INPUT_TKN(rctx->tkn);
	sa_align = SA_PTR_ALIGN(rctx->sa->sa_ptr);
	APMSEC_SATKNLOG("gcm encrypt tkn 0x%p outtkn 0x%p intkn 0x%p"
			"nbytes = %d",
			rctx->tkn, 
			TKN_CTX_RESULT_TKN(rctx->tkn), in_tkn, req->cryptlen);
	if (new_tkn) {
		rctx->tkn->input_tkn_len = sec_tkn_set_crypt_aes_gcm(
				(u32 *) in_tkn,
				req->cryptlen,
				apm_sec_v2hwaddr(sa_align),
				(u32 *) session->sa->sa_ptr,
				adata, req->assoclen, icvsize, 1);
		BUG_ON(rctx->tkn->input_tkn_len > session->tkn_input_len);
	} else {
		rctx->tkn->input_tkn_len = sec_tkn_update_crypt_aes_gcm(
				(u32 *) in_tkn,
				req->cryptlen,
				apm_sec_v2hwaddr(sa_align),
				(u32 *) session->sa->sa_ptr,
				adata, req->assoclen, icvsize, 1);
	}
	session->sa_flush_done = 0;
	rctx->tkn->sa		= rctx->sa;
	rctx->tkn->context      = &req->base;
	rctx->tkn->dest_nbytes = req->cryptlen + icvsize;
	
	rc = apm_sec_setup_crypto(&req->base);
	if (adata_flag)
		kunmap_atomic(adata);
	else
		kfree(adata);
 	if (rc == -EINPROGRESS)
		return rc; 
	
	if (rc != 0)
		APMSEC_TXLOG("encrypt gcm failed error %d",
				rc); 
			
err_enc_aes_gcm:
	if (rctx->tkn)
		apm_sec_tkn_free(session, rctx->tkn);
	if (rctx->sa)
		apm_sec_sa_free(session, rctx->sa);
	return rc;
}

static int apm_sec_decrypt_aes_gcm(struct aead_request *req)
{
	struct apm_sec_session_ctx *session  = crypto_tfm_ctx(req->base.tfm);
	struct apm_sec_req_ctx *rctx = aead_request_ctx(req);
	struct sec_tkn_input_hdr *in_tkn;
	struct crypto_aead *aead = crypto_aead_reqtfm(req);
	unsigned char 	*adata;
	unsigned char	new_tkn;
	unsigned char icvsize;
	int	rc, adata_flag;
	void *sa_align;
	
	/* Allocate token and SA for this request */
	rctx->tkn = apm_sec_tkn_get(session, &new_tkn);
	if (rctx->tkn == NULL)
		return -ENOMEM;
	rctx->sa  = apm_sec_sa_get(session);
	if (rctx->tkn == NULL) {
		rc = -ENOMEM;
		goto err_enc_aes_gcm;
	}

	/* Form SA - SPI, SEQ, and Nonce */
	memcpy(rctx->sa->sa_ptr, session->sa->sa_ptr, session->sa_len);
	sec_sa_setbe(SA_IV(rctx->sa->sa_ptr), req->iv, 12);
	if (apm_lsec_sg_scattered(req->assoclen, req->assoc)) {
		adata = kzalloc(req->assoclen, GFP_KERNEL);
		if (!adata) {
			rc = -ENOMEM;
			goto err_enc_aes_gcm;
		}
		apm_lsec_sg2buf(req->assoc, req->assoclen, adata);
		adata_flag = 0;
		/* Check if free as completion? */
	} else {
		adata = kmap_atomic(sg_page(&req->assoc[0])) +
				req->assoc[0].offset;
		adata_flag = 1;
	}

	icvsize = crypto_aead_authsize(aead);
	in_tkn = TKN_CTX_INPUT_TKN(rctx->tkn);
	sa_align = SA_PTR_ALIGN(rctx->sa->sa_ptr);
	APMSEC_SATKNLOG("gcm decrypt tkn 0x%p outtkn 0x%p intkn 0x%p",
			rctx->tkn, 
			TKN_CTX_RESULT_TKN(rctx->tkn), in_tkn);
	if (new_tkn) {
		rctx->tkn->input_tkn_len = sec_tkn_set_crypt_aes_gcm(
				(u32 *) in_tkn,
				req->cryptlen,
				apm_sec_v2hwaddr(sa_align),
				(u32 *) session->sa->sa_ptr,
				adata, req->assoclen, icvsize, 0);
		BUG_ON(rctx->tkn->input_tkn_len > session->tkn_input_len);
	} else {
		rctx->tkn->input_tkn_len = sec_tkn_update_crypt_aes_gcm(
				(u32 *) in_tkn,
				req->cryptlen,
				apm_sec_v2hwaddr(sa_align),
				(u32 *) session->sa->sa_ptr,
				adata, req->assoclen, icvsize, 0);
	}
	session->sa_flush_done = 0;
	rctx->tkn->sa		= rctx->sa;
	rctx->tkn->context      = &req->base;
	rctx->tkn->dest_nbytes = req->cryptlen;
	rc = apm_sec_setup_crypto(&req->base);
	if (adata_flag)
		kunmap_atomic(adata);
	else
		kfree(adata);
	if (rc == -EINPROGRESS)
		return rc; 

	if (rc != 0)
		APMSEC_TXLOG("decrypt gcm failed error %d", rc); 

err_enc_aes_gcm:
	if (rctx->tkn)
		apm_sec_tkn_free(session, rctx->tkn);
	if (rctx->sa)
		apm_sec_sa_free(session, rctx->sa);
	return rc;
}

static int apm_sec_givencrypt_aes_gcm(struct aead_givcrypt_request *req)
{
	return -ENOSYS;
}

static int apm_sec_givdecrypt_aes_gcm(struct aead_givcrypt_request *req)
{
	return -ENOSYS;
}

#if 0
/**
 * Authenc(cbc(aes),hmac(md5)) IPSec Functions
 *
 */
static int apm_sec_setkey_cbc_hmac_authenc(struct crypto_aead *cipher,
				      	const unsigned char 	*key,
				      	unsigned int	keylen,
					unsigned char   ca,
				      	unsigned char	ha)
{
	struct crypto_tfm    *tfm = crypto_aead_tfm(cipher);
	struct apm_sec_session_ctx *ctx = crypto_tfm_ctx(tfm);
	struct rtattr 	     *rta = (void *) key;
	struct crypto_authenc_key_param *param;
	unsigned int enckeylen;
	unsigned int authkeylen;
	unsigned int sa_len;
	unsigned int sa_max_len;
	int rc = -EINVAL;

	if (!RTA_OK(rta, keylen))
		goto badkey;
	if (rta->rta_type != CRYPTO_AUTHENC_KEYA_PARAM)
		goto badkey;
	if (RTA_PAYLOAD(rta) < sizeof(*param))
		goto badkey;

	param     = RTA_DATA(rta);
	enckeylen = be32_to_cpu(param->enckeylen);

	key    += RTA_ALIGN(rta->rta_len);
	keylen -= RTA_ALIGN(rta->rta_len);

	authkeylen = keylen - enckeylen;

	if (keylen < enckeylen)
		goto badkey;
	if (enckeylen != 128/8 && enckeylen != 192/8 && enckeylen != 256/8 &&
	    enckeylen != 64/8)
		goto badkey;
	if (ca == SA_CRYPTO_ALG_RSVD) {
		switch (enckeylen) {
		case 128/8:
			ca = SA_CRYPTO_ALG_AES128;
			break;
		case 192/8:
			ca = SA_CRYPTO_ALG_AES192;
			break;
		case 256/8:
		default:
			ca = SA_CRYPTO_ALG_AES256;
			break;
		}
	}
	if (ca == SA_CRYPTO_ALG_DES) {
		sa_len = SA_AUTHENC_DES_LEN(0 /* IV using PRNG */,
					ha, SA_DIGEST_TYPE_HMAC);
		sa_max_len = SA_AUTHENC_DES_MAX_LEN(
					0 /* IV using PRNG */,
					ha, SA_DIGEST_TYPE_HMAC);
	} else if (ca == SA_CRYPTO_ALG_3DES) {
		sa_len = SA_AUTHENC_3DES_LEN(
					0 /* IV using PRNG */,
					ha, SA_DIGEST_TYPE_HMAC);
		sa_max_len = SA_AUTHENC_3DES_MAX_LEN(
					0 /* IV using PRNG */,
					ha, SA_DIGEST_TYPE_HMAC);
	} else {
		sa_len = SA_AUTHENC_AES_LEN(enckeylen, 
					0 /* IV using PRNG */,
					ha, SA_DIGEST_TYPE_HMAC);
		sa_max_len = SA_AUTHENC_AES_MAX_LEN(enckeylen, 
					0 /* IV using PRNG */,
					ha, SA_DIGEST_TYPE_HMAC);
	}
	
	/* As key length can change, we need to flush the cache sa/token */
	apm_sec_free_sa_tkn_pool(ctx);
	/* Re-create the SA/token pool */
	rc = apm_sec_create_sa_tkn_pool(ctx, sa_max_len, sa_len, 1,
					sec_tkn_AUTHENC_MAX_SIZE);
	if (rc != 0)
		goto err_nomem;

	APMSEC_DEBUG("authenc keylen %d authkeylen %d "
		"ca %d ha %d sa_len %d icvsize %d",
		enckeylen, authkeylen, ca, ha, session->sa_len,
		crypto_aead_authsize(cipher));
	APMSEC_DEBUGX_DUMP("authenc key: ", key + authkeylen, enckeylen);
	APMSEC_DEBUGX_DUMP("authenc auth key: ", key, authkeylen);
	/* Setup SA - NOTE: Control word is in host endian to allow easy
	 * variable reference. Control word in token is in little endian.
	 * Even though we setup both SA (inbound and outbound) only one
	 * can be used.
	 */
	memset(session->sa->sa_ptr, 0, session->sa_len);
	session->sa->sa_ptr->crypto_alg 	= ca;
	session->sa->sa_ptr->hash_alg   	= ha;
	session->sa->sa_ptr->digest_type 	= SA_DIGEST_TYPE_HMAC;
	session->sa->sa_ptr->key 		= 1;
	session->sa->sa_ptr->cryptomode_feedback = SA_CRYPTO_MODE_AES_3DES_CBC;
	session->sa->sa_ptr->ToP		= SA_TOP_ENCRYPT_HASH_OUTBOUND;
	session->sa->sa_ptr->spi		= 1;
	session->sa->sa_ptr->seq		= SA_SEQ_32BITS;
	session->sa->sa_ptr->iv03		= 0;	/* Use PRNG */
	session->sa->sa_ptr->context_length = (sa_len >> 2) - 2;
	sec_sa_setbe(SA_KEY(session->sa->sa_ptr), key + authkeylen, enckeylen);

	/* Pre-compute inner and outer digest using auth key */
	APMSEC_DEBUG("authenc sa 0x%p sa_ptr 0x%p sa_ib 0x%p sa_ib_ptr 0x%p",
		session->sa, session->sa->sa_ptr, session->sa_ib, session->sa_ib->sa_ptr);
	rc = apm_sec_compute_ipad_opad(tfm, key, authkeylen,
			(unsigned char *) SA_DIGEST(session->sa->sa_ptr)
			crypto_ahash_digestsize);
	APMSEC_DEBUGX_DUMP("sa: ", session->sa->sa_ptr, session->sa_len);
	/* Now copy the same one for inbound */
	memcpy(session->sa_ib->sa_ptr, session->sa->sa_ptr, session->sa_len);
	session->sa_ib->sa_ptr->ToP = SA_TOP_HASH_DECRYPT_INBOUND;
	APMSEC_DEBUGX_DUMP("sa_ib: ", session->sa_ib->sa_ptr, session->sa_len);

	APMSEC_DEBUG_DUMP("alg %s setkey", 
			crypto_tfm_alg_name(tfm));
	return rc;

err_nomem:
	apm_sec_free_sa_tkn_pool(ctx);
	return rc;

badkey:
	crypto_aead_set_flags(cipher, CRYPTO_TFM_RES_BAD_KEY_LEN);
	return rc;
}

static int apm_sec_setkey_aes_md5_authenc(struct crypto_aead *cipher,
				 	  const unsigned char 	*key,
				 	  unsigned int	keylen)
{
	return apm_sec_setkey_cbc_hmac_authenc(cipher, key, keylen, 
			SA_CRYPTO_ALG_RSVD, SA_HASH_ALG_MD5);
}

static int __apm_sec_encrypt_cbc_authenc(struct aead_givcrypt_request *givreq,
					 struct aead_request *req)
{
	struct apm_sec_session_ctx     *ctx;
	struct apm_sec_req_ctx *rctx;
	struct sec_tkn_input_hdr *in_tkn;
	struct crypto_aead 	 *aead;
	struct crypto_alg 	 *alg;
	unsigned char 	new_tkn;
	int		rc;
	int		icvsize;
	void 		*page;
	
	/* For givreq, ESP header is located in req->assoc. It is located
	 * into context SA once only. The IV will be generated
	 * and store in req->giv. We expect the location of req->giv is
	 * be immediately before input data. The seq number to be used is
	 * stored in givreq->seq. It will not be used as it is cached in
	 * context SA.
	 */
	if (givreq)
		req = &givreq->areq;

	ctx  = crypto_tfm_ctx(req->base.tfm);
	rctx = aead_request_ctx(req);
	aead = crypto_aead_reqtfm(req);
	alg  = req->base.tfm->__crt_alg;
	
#if defined(APM_SEC_TXDEBUG)	
	page = kmap_atomic(sg_page(&req->src[0]));
	APMSEC_TXLOG("IPE%d encrypt authenc %s nbytes %d src[0] 0x%p len %d", 
			alg->cra_name, req->cryptlen,
			page + req->src[0].offset, req->src[0].length);
	kunmap_atomic(page);		
	/* Check if location of IV if immediate before input */
	if (givreq) {
		void	*addr;
		page = kmap_atomic(sg_page(&req->src[0]));
		addr = page + req->src[0].offset - alg->cra_aead.ivsize;
		kunmap_atomic(page);		
		if (addr != givreq->giv) {
			APMSEC_ERR("IPE%d authenc giv 0x%p location not "
				"before input 0x%p", 
				givreq->giv, addr);
			return -EINVAL;
		}
	}
#endif
	if (*SA_SPI(session->sa->sa_ptr) == 0) {
		/* Load SPI. SEQ is assumed to start at 0. */
		struct ip_esp_hdr *esph;
		page = kmap_atomic(sg_page(req->assoc));
		esph = (struct ip_esp_hdr *) (page + req->assoc->offset);	
		sec_sa_setbe(SA_SPI(session->sa->sa_ptr), esph->spi);
		kunmap_atomic(page);
	}	
	
	rctx->sa  = NULL;
	rctx->tkn = apm_sec_tkn_get(ctx, &new_tkn);
	if (rctx->tkn == NULL)
		return -ENOMEM;
	icvsize = crypto_aead_authsize(aead);
	in_tkn  = TKN_CTX_INPUT_TKN(rctx->tkn);
	APMSEC_SATKNLOG("IPE%d tkn 0x%p outtkn 0x%p intkn 0x%p",
			rctx->tkn, 
			TKN_CTX_RESULT_TKN(rctx->tkn), in_tkn);
	if (new_tkn) {
		rctx->tkn->input_tkn_len = sec_tkn_set_encrypt_cbc_authenc(
					(u32 *) in_tkn,
					req->cryptlen,
					apm_sec_v2hwaddr(session->sa->sa_ptr),
					(u32 *) session->sa->sa_ptr,
					session->sa->sa_ptr->crypto_alg,
					icvsize,
					SA_SEQ_OFFSET(session->sa->sa_ptr));
		BUG_ON(rctx->tkn->input_tkn_len > ctx->tkn_input_len);
		rctx->tkn->devid 	= ctx->dev->dev_id;
		rctx->tkn->priority 	= apm_sec_COS_DEF; 
		rctx->tkn->sa	        = session->sa;
	} else {
		sec_tkn_update_encrypt_cbc_authenc((u32 *) in_tkn, 
					req->cryptlen);
	}
	
	rctx->tkn->context     = &req->base;
	rctx->tkn->src	       = NULL;
	if (apm_sec_sg_scattered(req->cryptlen, req->dst)) {
		rctx->tkn->result_type     = IPE_TKN_RESULT_AMEM;
		rctx->tkn->amem_result.len = alg->cra_aead.ivsize + 
					     req->cryptlen + icvsize;
		rctx->tkn->amem_result.offset = -alg->cra_aead.ivsize;
		rctx->tkn->amem_result.buf = kmalloc(rctx->tkn->mem_result.len,
							GFP_KERNEL);
		if (rctx->tkn->amem_result.buf == NULL) {
			rc = -ENOMEM;
			goto out_chk;
		}
		rc = apm_sec_setup_crypto(&req->base);
	} else {
		void *dpage;
		rctx->tkn->result_type 	  = IPE_TKN_RESULT_MMEM;
		rctx->tkn->mem_result.len = alg->cra_aead.ivsize + 
						req->cryptlen + icvsize;
		rctx->tkn->mem_result.offset = 0;
		dpage = kmap_atomic(sg_page(&req->dst[0]));
		rctx->tkn->mem_result.buf = dpage + req->dst[0].offset - 
					    alg->cra_aead.ivsize;
		rc = apm_sec_setup_crypto(&req->base);
		kunmap_atomic(dpage);		
	}
out_chk:
	if (rc == -EINPROGRESS || rc == -EAGAIN)
		return rc; 
		
	if (rc != 0)
		APMSEC_TXLOG("IPE%d encrypt authenc failed error %d", 
				rc); 
	
	if (rctx->tkn)
		apm_sec_tkn_free(ctx, rctx->tkn);
	return rc;
}

static int apm_sec_encrypt_cbc_authenc(struct aead_request *req)
{
	return __apm_sec_encrypt_cbc_authenc(NULL, req);
}

static int apm_sec_givencrypt_cbc_authenc(struct aead_givcrypt_request
						*givreq)
{
	return __apm_sec_encrypt_cbc_authenc(givreq, NULL);
}

static int apm_sec_decrypt_cbc_authenc(struct aead_request *req)
{
	struct apm_sec_session_ctx     *ctx  = crypto_tfm_ctx(req->base.tfm);
	struct apm_sec_req_ctx *rctx = aead_request_ctx(req);
	struct crypto_aead 	 *aead = crypto_aead_reqtfm(req);
	struct crypto_alg 	 *alg  = req->base.tfm->__crt_alg;
	struct sec_tkn_input_hdr *in_tkn;
	int		icv_len = crypto_aead_authsize(aead);
	unsigned char 	new_tkn;
	int		rc;
	void 		*page;
#if defined(APM_SEC_RXDEBUG)	
	void		  *ivaddr;
	struct ip_esp_hdr *esph;
#endif
	/* ESP header is located in req->assoc. It is cached in context SA
	 * and will be located once only. The seq number is stored in
	 * givreq->seq. The seq number is cached in context SA. The IV is
	 * located in req->giv. We expect the location of req->giv is be
	 * immediately before input data. In addition the input sequence
	 * number of the packet must be immediately before IV as well.
	 */

#if defined(APM_SEC_RXDEBUG)	
	APMSEC_TXLOG("IPE%d decrypt authenc %s nbytes %d", 
			alg->cra_name, req->cryptlen); 
	/* Check if location of IV if immediate before input */
	page   = kmap_atomic(sg_page(&req->src[0]));
	ivaddr = page + req->src[0].offset - alg->cra_aead.ivsize;
	kunmap_atomic(page);		
	if (ivaddr != req->iv) {
		APMSEC_ERR("IPE%d authenc giv 0x%p location not before "
			"input 0x%p", req->iv,
			page + req->src[0].offset);
		return -EINVAL;
	}
	page  = kmap_atomic(sg_page(req->assoc));
	esph  = page + req->assoc->offset;
	kunmap_atomic(page);		
	if (esph->enc_data != ivaddr) {
		APMSEC_ERR("IPE%d authenc esph 0x%p location not before "
			"iv 0x%p", esph, ivaddr);
		return -EINVAL;
	}	
#endif
	if (*SA_SPI(session->sa_ib->sa_ptr) == 0) {
		/* Fist time - Load SPI and SEQ */
		struct ip_esp_hdr *esph;
		page = kmap_atomic(sg_page(req->assoc));
		esph = (struct ip_esp_hdr *) (page + req->assoc->offset);	
		sec_sa_setbe(SA_SPI(session->sa_ib->sa_ptr), esph->spi);
		kunmap_atomic(page);	
	}
	
	rctx->sa  = NULL;
	rctx->tkn = apm_sec_tkn_get(ctx, &new_tkn);
	if (rctx->tkn == NULL)
		return -ENOMEM;
	in_tkn = TKN_CTX_INPUT_TKN(rctx->tkn);
	APMSEC_SATKNLOG("IPE%d tkn 0x%p outtkn 0x%p intkn 0x%p",
			rctx->tkn, 
			TKN_CTX_RESULT_TKN(rctx->tkn), in_tkn);
	if (new_tkn) {
		rctx->tkn->input_tkn_len = sec_tkn_set_decrypt_cbc_authenc(
				(u32 *) in_tkn,
				alg->cra_aead.ivsize + req->cryptlen,
				apm_sec_v2hwaddr(session->sa_ib->sa_ptr),
				(u32 *) session->sa_ib->sa_ptr,
				session->sa_ib->sa_ptr->crypto_alg,
				session->sa_ib->sa_ptr->hash_alg,
				session->sa_ib->sa_ptr->digest_type,
				icv_len,
				SA_SEQ_OFFSET(session->sa_ib->sa_ptr));
		BUG_ON(rctx->tkn->input_tkn_len > ctx->tkn_input_len);

		rctx->tkn->devid	= ctx->dev->dev_id;
		rctx->tkn->priority 	= apm_sec_COS_DEF; 
		rctx->tkn->sa	        = session->sa_ib;
	} else {
		sec_tkn_update_decrypt_cbc_authenc((u32 *) in_tkn,
					alg->cra_aead.ivsize + req->cryptlen, 
					session->sa_ib->sa_ptr->crypto_alg, 
					icv_len);
	}
	rctx->tkn->context     = &req->base;
	rctx->tkn->src	       = NULL;
	if (apm_sec_sg_scattered(req->cryptlen, req->dst)) {
		rctx->tkn->result_type     = IPE_TKN_RESULT_AMEM;
		rctx->tkn->amem_result.len = req->cryptlen;
		rctx->tkn->amem_result.buf = kmalloc(req->cryptlen, GFP_KERNEL);
		if (rctx->tkn->amem_result.buf == NULL) {
			rc = -ENOMEM;
			goto out_chk;
		} 
		req->cryptlen	   += alg->cra_aead.ivsize;
		req->src[0].offset -= alg->cra_aead.ivsize;
		req->src[0].length += alg->cra_aead.ivsize;	
		if (req->src == req->dst)
			rctx->tkn->amem_result.offset = alg->cra_aead.ivsize;
		else
			rctx->tkn->amem_result.offset = 0;
		rc = apm_sec_setup_crypto(&req->base);
	} else {
		void *dpage;
		rctx->tkn->result_type 	     = IPE_TKN_RESULT_MMEM;
		rctx->tkn->mem_result.len    = req->cryptlen;
		rctx->tkn->mem_result.offset = 0;
		dpage = kmap_atomic(sg_page(&req->dst[0]));
		rctx->tkn->mem_result.buf = dpage + req->dst[0].offset;
		/* Must be after above line as req->dst == req->src (may) */
		req->cryptlen 	   += alg->cra_aead.ivsize; 
		req->src[0].offset -= alg->cra_aead.ivsize;
		req->src[0].length += alg->cra_aead.ivsize;	
		rc = apm_sec_setup_crypto(&req->base);
		kunmap_atomic(dpage);		
	}
out_chk:
	if (rc == -EINPROGRESS || rc == -EAGAIN)
		return rc; 
		
	if (rc != 0)
		APMSEC_TXLOG("IPE%d decrypt ccm failed error %d", 
				rc); 
	
	if (rctx->tkn)
		apm_sec_tkn_free(ctx, rctx->tkn);
	return rc;
}

static int apm_sec_setkey_aes_sha1_authenc(struct crypto_aead *cipher,
				 	  const unsigned char 	*key,
				 	  unsigned int	keylen)
{
	return apm_sec_setkey_cbc_hmac_authenc(cipher, key, keylen,
			SA_CRYPTO_ALG_RSVD, SA_HASH_ALG_SHA1);
}

static int apm_sec_setkey_aes_sha256_authenc(struct crypto_aead *cipher,
				 	  const unsigned char 	*key,
				 	  unsigned int	keylen)
{
	return apm_sec_setkey_cbc_hmac_authenc(cipher, key, keylen, 
			SA_CRYPTO_ALG_RSVD, SA_HASH_ALG_SHA256);
}
#endif
/**
* Setkey Routines for Transport/Tunnel GCM,/AES(CTR) Algorithms
*
*/


static int __apm_sec_setkey_esp_ctr(struct crypto_aead *cipher,
				const u8 *key,
				unsigned int keylen, unsigned char ca,
				unsigned char ha,
				unsigned char gcm_flag)
{
	struct crypto_tfm    *tfm = crypto_aead_tfm(cipher);
	struct apm_sec_session_ctx *session = crypto_tfm_ctx(tfm);
	struct rtattr 	     *rta = (void *) key;
	struct esp_authenc_param {
		__be32 spi;
		__be32 seq;
		__be16 pad_block_size;
		__be16 encap_uhl;
		struct crypto_authenc_key_param authenc_param;
	} *param;
	unsigned int enckeylen;
	unsigned int authkeylen;
	unsigned char dt;
	int rc = -EINVAL;

	if (!RTA_OK(rta, keylen))
		goto badkey;
	
	param     = RTA_DATA(rta);
	enckeylen = be32_to_cpu(param->authenc_param.enckeylen);

	key    += RTA_ALIGN(rta->rta_len);
	keylen -= RTA_ALIGN(rta->rta_len);
	if (gcm_flag)
		enckeylen = keylen;
	authkeylen = keylen - enckeylen;

	if (keylen < enckeylen)
		goto badkey;
	
	if (enckeylen != 128/8 + 4 && enckeylen != 192/8 + 4 &&
		enckeylen != 256/8 + 4) {
		crypto_aead_set_flags(cipher, CRYPTO_TFM_RES_BAD_KEY_LEN);
		return -EINVAL;
	}

	enckeylen -= 4;	/* salt (nonce) is at last 4 bytes */

	switch (enckeylen) {
	case 128/8:
		ca = SA_CRYPTO_ALG_AES128;
		break;
	case 192/8:
		ca = SA_CRYPTO_ALG_AES192;
		break;
	case 256/8:
	default:
		ca = SA_CRYPTO_ALG_AES256;
		break;
	}
	/* As key length can change, we need to flush the cache sa/token */
	apm_sec_free_sa_tkn_pool(session);
	if (gcm_flag) {
		rc = apm_sec_create_sa_tkn_pool(session, 
			SA_AES_GCM_MAX_LEN, SA_AES_GCM_LEN(enckeylen),
			1, IPE_TKN_ESP_TRANSPORT_GCM_MAXSIZE);
	} else {
		dt = SA_DIGEST_TYPE_HMAC;
		rc = apm_sec_create_sa_tkn_pool(session,
			SA_AES_CTR_MAX_LEN,
			SA_AES_CTR_LEN(enckeylen, ha, dt),
			1, IPE_TKN_ESP_TRANSPORT_GCM_MAXSIZE);
	}

	if (rc != 0)
		goto err_nomem;

	memset(session->sa->sa_ptr, 0, session->sa_len);
	session->sa->sa_ptr->crypto_alg 	= ca;
	session->sa->sa_ptr->hash_alg   	= ha;
	if (gcm_flag)
		session->sa->sa_ptr->digest_type 	= SA_DIGEST_TYPE_HASH;
	else
		session->sa->sa_ptr->digest_type 	= SA_DIGEST_TYPE_HMAC;
	session->sa->sa_ptr->key 		= 1;
	session->sa->sa_ptr->cryptomode_feedback = SA_CRYPTO_MODE_AES_3DES_CTR;
	session->sa->sa_ptr->ToP		= SA_TOP_ENCRYPT_HASH_OUTBOUND;
	session->sa->sa_ptr->spi		= 1;
	session->sa->sa_ptr->seq		= SA_SEQ_32BITS;
	session->sa->sa_ptr->iv_format	= SA_IV_FORMAT_COUNTER_MODE;
	session->sa->sa_ptr->iv03		= 0x1;
	if (gcm_flag)
		session->sa->sa_ptr->enc_hash_result= 1;

	session->sa->sa_ptr->pad_type	= SA_PAD_TYPE_IPSEC;
	session->sa->sa_ptr->context_length = (session->sa_len >> 2) - 2;
#if 0
	sec_sa_setbe(SA_SPI(session->sa->sa_ptr), param->spi);
	sec_sa_setbe(SA_SEQ(session->sa->sa_ptr), param->seq);
#endif
	sec_sa_set32le(SA_SPI(session->sa->sa_ptr), param->spi);
	sec_sa_set32le(SA_SEQ(session->sa->sa_ptr), param->seq);

	session->pad_block_size = be16_to_cpu(param->pad_block_size);
	session->encap_uhl      = be16_to_cpu(param->encap_uhl);

	APMSEC_DEBUG_DUMP("enc key: ", key + authkeylen, enckeylen);
	APMSEC_DEBUG_DUMP("auth key: ", key, authkeylen);
	if (gcm_flag) {
		sec_sa_setbe(SA_KEY(session->sa->sa_ptr), key, enckeylen);
		/* Save salt into IV0; IV1 & IV2 will be filled later
		 * from request
		 */
		sec_sa_setbe(SA_IV(session->sa->sa_ptr), key + enckeylen, 4);
	} else {
		sec_sa_setbe(SA_KEY(session->sa->sa_ptr), key + authkeylen,
							enckeylen);
		/* Save salt into IV0; IV1 & IV2 will be filled
		 * later from request
		 */
		sec_sa_setbe(SA_IV(session->sa->sa_ptr), key + authkeylen
							+ enckeylen, 4);
	}
	APMSEC_DEBUG("esp sa 0x%p sa_ptr 0x%p sa_ib 0x%p sa_ib_ptr 0x%p",
		session->sa, session->sa->sa_ptr, session->sa_ib, session->sa_ib->sa_ptr); 
	if (gcm_flag)
		rc = apm_compute_gcm_hash_key(tfm,
					(unsigned char *) key, enckeylen);
	else
		rc = apm_sec_compute_ipad_opad(tfm, key, authkeylen,
					(u8 *) SA_DIGEST(session->sa->sa_ptr));
	if (rc < 0) {
		APMSEC_ERR("returned err in digest calc %d", rc)
		goto err_nomem;
	}
	
	APMSEC_DEBUGX_DUMP("sa: ", session->sa->sa_ptr, session->sa_len);
	/* Now copy the same one for inbound */
	memcpy(session->sa_ib->sa_ptr, session->sa->sa_ptr, session->sa_len);
	session->sa_ib->sa_ptr->ToP = SA_TOP_HASH_DECRYPT_INBOUND;
	APMSEC_DEBUGX_DUMP("sa_ib: ", session->sa_ib->sa_ptr, session->sa_len);

	APMSEC_DEBUG("alg %s setkey", 
			crypto_tfm_alg_name(tfm));
	return 0;

err_nomem:
	apm_sec_free_sa_tkn_pool(session);
	return rc;

badkey:
	crypto_aead_set_flags(cipher, CRYPTO_TFM_RES_BAD_KEY_LEN);
	return rc;
}

static unsigned char apm_sec_set_esp_ha(struct crypto_aead *cipher)
{
	struct crypto_tfm *tfm = crypto_aead_tfm(cipher);
	struct crypto_alg *alg  = tfm->__crt_alg;
	struct aead_alg aalg = alg->cra_aead;
	unsigned char ha;

	switch (aalg.maxauthsize) {
		default:
		case 128/8:
			ha = SA_HASH_ALG_MD5;
			break;
		case 160/8:
			ha = SA_HASH_ALG_SHA1;
			break;
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
	return ha;
}
static int __apm_sec_setkey_esp(struct crypto_aead *cipher, const u8 *key,
				  unsigned int keylen, unsigned char ca,
				  unsigned char ha)
{
	struct crypto_tfm    *tfm = crypto_aead_tfm(cipher);
	struct apm_sec_session_ctx *session = crypto_tfm_ctx(tfm);
	struct rtattr 	     *rta = (void *) key;
	struct esp_authenc_param {
		__be32 spi;
		__be32 seq;
		__be16 pad_block_size;
		__be16 encap_uhl;
		struct crypto_authenc_key_param authenc_param;
	} *param;
	unsigned int enckeylen;
	unsigned int authkeylen;
	unsigned int sa_len;
	unsigned int sa_max_len;
	int rc = -EINVAL;

	if (!RTA_OK(rta, keylen))
		goto badkey;
	if (rta->rta_type != CRYPTO_AUTHENC_KEYA_PARAM)
		goto badkey;
	if (RTA_PAYLOAD(rta) < sizeof(*param))
		goto badkey;

	param     = RTA_DATA(rta);
	enckeylen = be32_to_cpu(param->authenc_param.enckeylen);

	key    += RTA_ALIGN(rta->rta_len);
	keylen -= RTA_ALIGN(rta->rta_len);

	authkeylen = keylen - enckeylen;

	if (keylen < enckeylen)
		goto badkey;
	if (enckeylen != 128/8 && enckeylen != 192/8 && enckeylen != 256/8 &&
	    enckeylen != 64/8)
		goto badkey;
	if (ca == SA_CRYPTO_ALG_RSVD) {
		switch (enckeylen) {
		case 128/8:
			ca = SA_CRYPTO_ALG_AES128;
			break;
		case 192/8:
			ca = SA_CRYPTO_ALG_AES192;
			break;
		case 256/8:
		default:
			ca = SA_CRYPTO_ALG_AES256;
			break;
		}
	}
	if (ca == SA_CRYPTO_ALG_DES) {
		sa_len = SA_AUTHENC_DES_LEN(0 /* IV using PRNG */,
					ha, SA_DIGEST_TYPE_HMAC);
		sa_max_len = SA_AUTHENC_DES_MAX_LEN(
					0 /* IV using PRNG */,
					ha, SA_DIGEST_TYPE_HMAC);
	} else if (ca == SA_CRYPTO_ALG_3DES) {
		sa_len = SA_AUTHENC_3DES_LEN(
					0 /* IV using PRNG */,
					ha, SA_DIGEST_TYPE_HMAC);
		sa_max_len = SA_AUTHENC_3DES_MAX_LEN(
					0 /* IV using PRNG */,
					ha, SA_DIGEST_TYPE_HMAC);
	} else {
		sa_len = SA_AUTHENC_AES_LEN(enckeylen, 
					0 /* IV using PRNG */,
					ha, SA_DIGEST_TYPE_HMAC);
		sa_max_len = SA_AUTHENC_AES_MAX_LEN(enckeylen, 
					0 /* IV using PRNG */,
					ha, SA_DIGEST_TYPE_HMAC);
	}
	
	/* As key length can change, we need to flush the cache sa/token */
	apm_sec_free_sa_tkn_pool(session);
	/* Re-create the SA/token pool */
	/*FIXME*/
	/*  For now this size should work for both transport and Tunnel input and output since
	we do not pass the Outer IP header processing to the Engine */
	rc = apm_sec_create_sa_tkn_pool(session, sa_max_len, sa_len, 1,
					IPE_TKN_ESP_TRANSPORT_MAXSIZE);

	if (rc != 0)
		goto err_nomem;

	APMSEC_DEBUG("authenc keylen %d authkeylen %d "
		"ca %d ha %d sa_len %d icvsize %d",
		enckeylen, authkeylen, ca, ha, session->sa_len,
		crypto_aead_authsize(cipher));
	APMSEC_DEBUGX_DUMP("authenc key: ", key + authkeylen, enckeylen);
	APMSEC_DEBUGX_DUMP("authenc auth key: ", key, authkeylen);
	/* Setup SA - NOTE: Control word is in host endian to allow easy
	 * variable reference. Control word in token is in little endian.
	 * Even though we setup both SA (inbound and outbound) only one
	 * can be used.
	 */
	memset(session->sa->sa_ptr, 0, session->sa_len);
	session->sa->sa_ptr->crypto_alg 	= ca;
	session->sa->sa_ptr->hash_alg   	= ha;
	session->sa->sa_ptr->digest_type 	= SA_DIGEST_TYPE_HMAC;
	session->sa->sa_ptr->key 		= 1;
	session->sa->sa_ptr->cryptomode_feedback = SA_CRYPTO_MODE_AES_3DES_CBC;
	session->sa->sa_ptr->ToP		= SA_TOP_ENCRYPT_HASH_OUTBOUND;
	session->sa->sa_ptr->spi		= 1;
	session->sa->sa_ptr->seq		= SA_SEQ_32BITS;
	session->sa->sa_ptr->iv03		= 0;	/* Use PRNG */
	session->sa->sa_ptr->pad_type	= SA_PAD_TYPE_IPSEC;
	session->sa->sa_ptr->context_length = (sa_len >> 2) - 2;
	sec_sa_setbe(SA_KEY(session->sa->sa_ptr), key + authkeylen, enckeylen);
	sec_sa_set32le(SA_SPI(session->sa->sa_ptr), param->spi);
	sec_sa_set32le(SA_SEQ(session->sa->sa_ptr), param->seq);
	
	session->pad_block_size = be16_to_cpu(param->pad_block_size);
	session->encap_uhl      = be16_to_cpu(param->encap_uhl);

	/* Pre-compute inner and outer digest using auth key */
	APMSEC_DEBUG("SA 0x%p sa_ptr 0x%p sa_ib 0x%p sa_ib_ptr 0x%p",
		session->sa, session->sa->sa_ptr, session->sa_ib, session->sa_ib->sa_ptr);
	rc = apm_sec_compute_ipad_opad(tfm, key, authkeylen,
			(unsigned char *) SA_DIGEST(session->sa->sa_ptr));
	if (rc < 0)
		goto err_nomem;
	APMSEC_DEBUGX_DUMP("sa: ", session->sa->sa_ptr, session->sa_len);
	/* Now copy the same one for inbound */
	memcpy(session->sa_ib->sa_ptr, session->sa->sa_ptr, session->sa_len);
	session->sa_ib->sa_ptr->ToP = SA_TOP_HASH_DECRYPT_INBOUND;
	APMSEC_DEBUGX_DUMP("sa_ib: ", session->sa_ib->sa_ptr, session->sa_len);

	APMSEC_DEBUG("alg %s setkey", 
			crypto_tfm_alg_name(tfm));
	return 0;

err_nomem:
	apm_sec_free_sa_tkn_pool(session);
	return rc;

badkey:
	crypto_aead_set_flags(cipher, CRYPTO_TFM_RES_BAD_KEY_LEN);
	return rc;
}


static int apm_sec_setkey_esp(struct crypto_aead *cipher,
			      const u8 *key,
			      unsigned int keylen)
{
	unsigned char ha;
	
	ha = apm_sec_set_esp_ha(cipher);
	return __apm_sec_setkey_esp(cipher, key, keylen,
				    SA_CRYPTO_ALG_RSVD,
				    ha);
}
int glb_seq = 0;

/**
 * ESP Tunnel Encrypt/Decrypt Routine for all AES/DES/DES3/GCM/AES(CTR) Algs
 *
 */
static int _apm_sec_encrypt_esp_tunnel(struct aead_request *req, int gcm_flag)
{
	struct apm_sec_session_ctx     *session;
	struct apm_sec_req_ctx *rctx;
	struct sec_tkn_input_hdr *in_tkn;
	struct crypto_aead 	 *aead;
	struct crypto_alg 	 *alg;
	struct iphdr	*iph;
	unsigned char 	new_tkn;
	int		rc;
	int		icvsize;
	void 		*page;
	int		pdl;
	unsigned char	nh;
	int		clen;
	void *sa_align;

	session  = crypto_tfm_ctx(req->base.tfm);
	rctx = aead_request_ctx(req);
	aead = crypto_aead_reqtfm(req);
	alg  = req->base.tfm->__crt_alg;
	
#if defined(APM_SEC_TXDEBUG)
	APMSEC_TXLOG("encrypt esp tunnel %s nbytes %d",
		     alg->cra_name, req->cryptlen);
#endif
	rctx->sa  = NULL;
	rctx->tkn = apm_sec_tkn_get(session, &new_tkn);
	if (rctx->tkn == NULL)
		return -ENOMEM;
	icvsize = crypto_aead_authsize(aead);
	in_tkn  = TKN_CTX_INPUT_TKN(rctx->tkn);
	APMSEC_SATKNLOG("ESP Tunnel tkn 0x%p outtkn 0x%p intkn 0x%p",
			rctx->tkn,
			TKN_CTX_RESULT_TKN(rctx->tkn), in_tkn);
	/* Compute pad len and Next header byte */
	page = kmap_atomic(sg_page(&req->src[0]));
	iph   = ((struct iphdr *) (page + req->src[0].offset));
	nh = iph->protocol;
	clen = req->cryptlen;
	kunmap_atomic(page);
	
	pdl = ALIGN(clen + 2, ALIGN(crypto_aead_blocksize(aead), 4));
	if (session->pad_block_size)
		pdl = ALIGN(pdl, session->pad_block_size);
	pdl -= clen;
	sa_align = SA_PTR_ALIGN(session->sa->sa_ptr);
	/* sec_sa_set32le(SA_SEQ(session->sa->sa_ptr), (++glb_seq)); */
	if (new_tkn) {
		rctx->tkn->input_tkn_len = sec_tkn_set_espout_tunnel(
				(u32 *) in_tkn,
		req->cryptlen,
		apm_sec_v2hwaddr(sa_align),
		(u32 *) session->sa->sa_ptr,
		session->sa->sa_ptr->crypto_alg,
		session->sa->sa_ptr->cryptomode_feedback,
		pdl, gcm_flag, SA_SEQ_OFFSET(session->sa->sa_ptr));
	BUG_ON(rctx->tkn->input_tkn_len > session->tkn_input_len);
#if 0
		rctx->tkn->devid	= session->dev->dev_id;
		rctx->tkn->priority 	= apm_sec_COS_DEF;
#endif
		rctx->tkn->sa	        = session->sa;
	} else {
		sec_tkn_update_espout_tunnel((u32 *) in_tkn,
					      req->cryptlen,
					      session->sa->sa_ptr->cryptomode_feedback,
					      icvsize, pdl, gcm_flag);
	}
	
	rctx->tkn->context     = &req->base;
	rctx->tkn->dest_nbytes = sizeof(struct ip_esp_hdr) +
			alg->cra_aead.ivsize + pdl +
			req->cryptlen
			+ icvsize;

	/** For AES to work need to flush the entire ESP packet 
	Len of IP Header(20) + Len of SPI + Len of Seq + Len of IV(16) */
	rctx->tkn->esp_src_len = 20 /* Size of IP Header*/ +
			sizeof(struct ip_esp_hdr) +
			alg->cra_aead.ivsize;
#if 0
	printk("rctx->tkn->dest_nbytes = %d\n", rctx->tkn->dest_nbytes);
	printk("pdl = %d\n", pdl);
	printk("icvsize = %d\n", icvsize);
#endif
	rc = apm_sec_setup_crypto(&req->base);
	if (rc == -EINPROGRESS)
		return rc;
		
	if (rc != 0)
		APMSEC_TXLOG("encrypt esp tunnel failed error %d",
			     rc);
	
	if (rctx->tkn)
		apm_sec_tkn_free(session, rctx->tkn);
	return rc;
	
}

static int _apm_sec_decrypt_esp_tunnel(struct aead_request *req, int gcm_flag)
{
	struct apm_sec_session_ctx     *session  = crypto_tfm_ctx(req->base.tfm);
	struct apm_sec_req_ctx *rctx = aead_request_ctx(req);
	struct crypto_aead 	 *aead = crypto_aead_reqtfm(req);
#ifdef APM_SEC_TXDEBUG
	struct crypto_alg 	 *alg  = req->base.tfm->__crt_alg;
#endif
	struct sec_tkn_input_hdr *in_tkn;
	int		icv_len = crypto_aead_authsize(aead);
	unsigned char 	new_tkn;
	int		rc;
	int		ihl;
	void 		*page;
	void *sa_align;
	int ivsize;
	
	APMSEC_TXLOG("decrypt esp tunnel %s nbytes %d",
		     alg->cra_name, req->cryptlen);
	
	rctx->sa  = NULL;
	rctx->tkn = apm_sec_tkn_get(session, &new_tkn);
	if (rctx->tkn == NULL)
		return -ENOMEM;
	/* Extract IHL length */
	page = kmap_atomic(sg_page(&req->src[0]));
	ihl  = ((struct iphdr *) (page + req->src[0].offset))->ihl << 2;
	kunmap_atomic(page);
	sa_align = SA_PTR_ALIGN(session->sa_ib->sa_ptr);
	in_tkn = TKN_CTX_INPUT_TKN(rctx->tkn);
	APMSEC_SATKNLOG("tkn 0x%p outtkn 0x%p intkn 0x%p",
			rctx->tkn,
			TKN_CTX_RESULT_TKN(rctx->tkn), in_tkn);
	if (new_tkn) {
		rctx->tkn->input_tkn_len = sec_tkn_set_espin_tunnel(
				(u32 *) in_tkn,
		req->cryptlen,
		apm_sec_v2hwaddr(sa_align),
		(u32 *) session->sa_ib->sa_ptr,
		session->sa_ib->sa_ptr->crypto_alg,
		session->sa_ib->sa_ptr->cryptomode_feedback, gcm_flag,
		ihl, icv_len, SA_SEQ_OFFSET(session->sa_ib->sa_ptr));
		BUG_ON(rctx->tkn->input_tkn_len > session->tkn_input_len);
#if 0
		rctx->tkn->devid	= session->dev->dev_id;
		rctx->tkn->priority 	= apm_sec_COS_DEF;
#endif
		rctx->tkn->sa	        = session->sa_ib;
	} else {
		sec_tkn_update_espin_tunnel((u32 *) in_tkn,
					     req->cryptlen,
					     session->sa_ib->sa_ptr->cryptomode_feedback,
					     session->sa_ib->sa_ptr->crypto_alg,
					     icv_len, gcm_flag);
	}
	rctx->tkn->context     = &req->base;
	ivsize = (session->sa_ib->sa_ptr->crypto_alg == SA_CRYPTO_ALG_DES ||
			session->sa_ib->sa_ptr->crypto_alg == SA_CRYPTO_ALG_3DES)
			? 8 : 16;
	if (gcm_flag)
		ivsize = 8;
	rctx->tkn->dest_nbytes = req->cryptlen /* -  icv_len */ - 8 - ivsize;

	rc = apm_sec_setup_crypto(&req->base);
	if (rc == -EINPROGRESS)
		return rc;
		
	if (rc != 0)
		APMSEC_TXLOG("decrypt ccm failed error %d",
			     rc);
	
	if (rctx->tkn)
		apm_sec_tkn_free(session, rctx->tkn);
	return rc;
}
static int apm_sec_decrypt_esp_tunnel(struct aead_request *req)
{
	return _apm_sec_decrypt_esp_tunnel(req, 0);
}

static int apm_sec_encrypt_esp_tunnel(struct aead_request *req)
{
	return _apm_sec_encrypt_esp_tunnel(req, 0);
}

static int apm_sec_decrypt_esp_tunnel_gcm(struct aead_request *req)
{
	return _apm_sec_decrypt_esp_tunnel(req, 1);
}

static int apm_sec_encrypt_esp_tunnel_gcm(struct aead_request *req)
{
	return _apm_sec_encrypt_esp_tunnel(req, 1);
}

static int apm_sec_setkey_esp_gcm(struct crypto_aead *cipher,
				const u8 *key,
				unsigned int keylen)
{
	return __apm_sec_setkey_esp_ctr(cipher, key, keylen,
					  SA_CRYPTO_ALG_RSVD,
					  SA_HASH_ALG_GHASH, 1);
}

static int apm_sec_setkey_esp_aes_ctr(struct crypto_aead *cipher,
				const u8 *key,
				unsigned int keylen)
{
	unsigned char ha;
	ha = apm_sec_set_esp_ha(cipher);
	return __apm_sec_setkey_esp_ctr(cipher, key, keylen,
					SA_CRYPTO_ALG_RSVD,
					ha, 0);
}

static int apm_sec_setkey_esp_des3(struct crypto_aead *cipher,
				const unsigned char *key,
				unsigned int keylen)
{
	unsigned char ha;
	ha = apm_sec_set_esp_ha(cipher);
	return __apm_sec_setkey_esp(cipher, key, keylen,
					SA_CRYPTO_ALG_3DES,
					ha);

}

static int apm_sec_setkey_esp_des(struct crypto_aead *cipher,
				const u8 *key,
				unsigned int keylen)
{
	unsigned char ha;
	ha = apm_sec_set_esp_ha(cipher);

	return __apm_sec_setkey_esp(cipher, key, keylen,
				SA_CRYPTO_ALG_DES,
				ha);
}


/**
 * ESP Transport Encrypt/Decrypt Routine for AES/DES/DES3/AES(CTR) Algs
 *
 */
static int _apm_sec_encrypt_esp_transport(struct aead_request *req, int gcm_flag)
{
	struct apm_sec_session_ctx     *session;
	struct apm_sec_req_ctx *rctx;
	struct sec_tkn_input_hdr *in_tkn;
	struct crypto_aead 	 *aead;
	struct crypto_alg 	 *alg;
	struct iphdr	*iph;
	unsigned char 	new_tkn;
	int		rc;
	int		icvsize;
	void 		*page;
	int		pdl;
	unsigned char	nh;
	unsigned char	iphl;
	int		clen;
	void *sa_align;
	
	session  = crypto_tfm_ctx(req->base.tfm);
	rctx = aead_request_ctx(req);
	aead = crypto_aead_reqtfm(req);
	alg  = req->base.tfm->__crt_alg;
	
#if defined(APM_SEC_TXDEBUG)	
	APMSEC_TXLOG("IPE encrypt esp %s nbytes %d",
			alg->cra_name, req->cryptlen);
#endif
	rctx->sa  = NULL;
	rctx->tkn = apm_sec_tkn_get(session, &new_tkn);
	if (rctx->tkn == NULL)
		return -ENOMEM;
	icvsize = crypto_aead_authsize(aead);
	in_tkn  = TKN_CTX_INPUT_TKN(rctx->tkn);
	APMSEC_SATKNLOG("IPE tkn 0x%p outtkn 0x%p intkn 0x%p",
			rctx->tkn, 
			TKN_CTX_RESULT_TKN(rctx->tkn), in_tkn);
	/* Compute pad len and Next header byte */
	page = kmap_atomic(sg_page(&req->src[0]));
	iph   = ((struct iphdr *) (page + req->src[0].offset));
	nh = iph->protocol;
	iphl = iph->ihl << 2;
	APMSEC_DEBUG("icv_len = %d\n", icvsize);
	APMSEC_DEBUG("nh = %d\n", nh);
	clen = req->cryptlen - iphl;
	kunmap_atomic(page);
	sa_align = SA_PTR_ALIGN(session->sa->sa_ptr);
	pdl = ALIGN(clen + 2, ALIGN(crypto_aead_blocksize(aead), 4));
	if (session->pad_block_size)
		pdl = ALIGN(pdl, session->pad_block_size);
	pdl -= clen;
	if (new_tkn) {
		rctx->tkn->input_tkn_len = sec_tkn_set_espout_transport(
					(u32 *) in_tkn,
					req->cryptlen,
					apm_sec_v2hwaddr(sa_align),
					(u32 *) session->sa->sa_ptr,
					session->sa->sa_ptr->crypto_alg,
					pdl,
					icvsize,
					nh, iphl,
					SA_SEQ_OFFSET(session->sa->sa_ptr),
					session->sa->sa_ptr->cryptomode_feedback,
					gcm_flag);
		BUG_ON(rctx->tkn->input_tkn_len > session->tkn_input_len);
		rctx->tkn->sa	        = session->sa;
	} else {
		sec_tkn_update_espout_transport((u32 *) in_tkn, 
					req->cryptlen,
					session->sa_ib->sa_ptr->crypto_alg,
					pdl, nh, iphl, icvsize,
					session->sa->sa_ptr->cryptomode_feedback,
					gcm_flag);
	}
	
	rctx->tkn->context     = &req->base;
	rctx->tkn->dest_nbytes = sizeof(struct ip_esp_hdr) +
			alg->cra_aead.ivsize + pdl +
			req->cryptlen + icvsize;

	rc = apm_sec_setup_crypto(&req->base);
	if (rc == -EINPROGRESS)
		return rc;		
	if (rc != 0)
		APMSEC_TXLOG("IPE encrypt transport esp failed error %d",
				rc); 
	
	if (rctx->tkn)
		apm_sec_tkn_free(session, rctx->tkn);
	return rc;
}

static int _apm_sec_decrypt_esp_transport(struct aead_request *req, int gcm_flag)
{
	struct apm_sec_session_ctx     *session  = crypto_tfm_ctx(req->base.tfm);
	struct apm_sec_req_ctx *rctx = aead_request_ctx(req);
	struct crypto_aead 	 *aead = crypto_aead_reqtfm(req);
#ifdef APM_SEC_TXDEBUG	
	struct crypto_alg 	 *alg  = req->base.tfm->__crt_alg;
#endif
	struct sec_tkn_input_hdr *in_tkn;
	int		icv_len = crypto_aead_authsize(aead);
	unsigned char 	new_tkn;
	int		rc;
	int		ihl;
	void 		*page;
	int ivsize;
	void *sa_align;
	
	APMSEC_TXLOG("IPE decrypt esp %s nbytes %d",
			alg->cra_name, req->cryptlen); 
	
	rctx->sa  = NULL;
	rctx->tkn = apm_sec_tkn_get(session, &new_tkn);
	if (rctx->tkn == NULL)
		return -ENOMEM;
	/* Extract IHL length */
	page = kmap_atomic(sg_page(&req->src[0]));
	ihl  = ((struct iphdr *) (page + req->src[0].offset))->ihl << 2;	
	kunmap_atomic(page);
	sa_align = SA_PTR_ALIGN(session->sa_ib->sa_ptr);
	in_tkn = TKN_CTX_INPUT_TKN(rctx->tkn);
	APMSEC_SATKNLOG("IPE tkn 0x%p outtkn 0x%p intkn 0x%p",
			rctx->tkn, 
			TKN_CTX_RESULT_TKN(rctx->tkn), in_tkn);
	if (new_tkn) {
		rctx->tkn->input_tkn_len = sec_tkn_set_espin_transport(
				(u32 *) in_tkn,
				req->cryptlen,
				apm_sec_v2hwaddr(sa_align),
				(u32 *) session->sa_ib->sa_ptr,
				session->sa_ib->sa_ptr->crypto_alg,
				ihl, icv_len,
				SA_SEQ_OFFSET(session->sa_ib->sa_ptr),
				session->sa_ib->sa_ptr->cryptomode_feedback,
				gcm_flag);
		BUG_ON(rctx->tkn->input_tkn_len > session->tkn_input_len);
		rctx->tkn->sa	        = session->sa_ib;
	} else {
		sec_tkn_update_espin_transport((u32 *) in_tkn,
				req->cryptlen,
				session->sa_ib->sa_ptr->crypto_alg,
				ihl, icv_len,
				session->sa_ib->sa_ptr->cryptomode_feedback,
				gcm_flag);
	}
	rctx->tkn->context     = &req->base;
	ivsize = (session->sa_ib->sa_ptr->crypto_alg == SA_CRYPTO_ALG_DES ||
			session->sa_ib->sa_ptr->crypto_alg ==
			SA_CRYPTO_ALG_3DES
			|| session->sa_ib->sa_ptr->cryptomode_feedback ==
			SA_CRYPTO_MODE_AES_3DES_CTR) ? 8 : 16;
	rctx->tkn->dest_nbytes = req->cryptlen - icv_len - 8 - ivsize;

	rc = apm_sec_setup_crypto(&req->base);
	if (rc == -EINPROGRESS)
		return rc;	
	if (rc != 0)
		APMSEC_TXLOG("IPE decrypt failed error %d",
				rc); 
	
	if (rctx->tkn)
		apm_sec_tkn_free(session, rctx->tkn);
	return rc;
}

static int apm_sec_encrypt_esp_transport_gcm(struct aead_request *req)
{
	return _apm_sec_encrypt_esp_transport(req, 1);
}

static int apm_sec_encrypt_esp_transport(struct aead_request *req)
{
	return _apm_sec_encrypt_esp_transport(req, 0);
}

static int apm_sec_decrypt_esp_transport_gcm(struct aead_request *req)
{
	return _apm_sec_decrypt_esp_transport(req, 1);
}

static int apm_sec_decrypt_esp_transport(struct aead_request *req)
{
	return _apm_sec_decrypt_esp_transport(req, 0);
}
#if 0
/**
 * ESP Transport Encrypt/Decrypt Routine for GCM Algorithm
 *
 */
static int apm_sec_encrypt_esp_transport_gcm(struct aead_request *req)
{
	struct apm_sec_session_ctx     *ctx;
	struct apm_sec_req_ctx *rctx;
	struct sec_tkn_input_hdr *in_tkn;
	struct crypto_aead 	 *aead;
	struct crypto_alg 	 *alg;
	struct iphdr	*iph;
	unsigned char 	new_tkn;
	int		rc;
	int		icvsize;
	void 		*page;
	int		pdl;
	unsigned char	nh;
	int		clen;	
	
	ctx  = crypto_tfm_ctx(req->base.tfm);
	rctx = aead_request_ctx(req);
	aead = crypto_aead_reqtfm(req);
	alg  = req->base.tfm->__crt_alg;

#if defined(APM_SEC_TXDEBUG)	
	APMSEC_TXLOG("IPE%d encrypt esp %s nbytes %d", 
			alg->cra_name, req->cryptlen);
#endif
	rctx->sa  = NULL;
	rctx->tkn = apm_sec_tkn_get(ctx, &new_tkn);
	if (rctx->tkn == NULL)
		return -ENOMEM;
	icvsize = crypto_aead_authsize(aead);
	in_tkn  = TKN_CTX_INPUT_TKN(rctx->tkn);
	APMSEC_SATKNLOG("IPE%d tkn 0x%p outtkn 0x%p intkn 0x%p",
			rctx->tkn, 
			TKN_CTX_RESULT_TKN(rctx->tkn), in_tkn);
	/* Compute pad len and Next header byte */
	page = kmap_atomic(sg_page(&req->src[0]));
	iph   = ((struct iphdr *) (page + req->src[0].offset));
	nh = iph->protocol;
	clen = req->cryptlen - (iph->ihl << 2);
	kunmap_atomic(page);
	
	pdl = ALIGN(clen + 2, ALIGN(crypto_aead_blocksize(aead), 4));
	if (ctx->pad_block_size)
		pdl = ALIGN(pdl, ctx->pad_block_size);
	pdl -= clen;
	if (new_tkn) {
		rctx->tkn->input_tkn_len = sec_tkn_set_espout_transport_gcm(
					(u32 *) in_tkn,
					req->cryptlen,
					apm_sec_v2hwaddr(session->sa->sa_ptr),
					(u32 *) session->sa->sa_ptr,
					pdl,
					icvsize,
					nh,
					SA_SEQ_OFFSET(session->sa->sa_ptr));
		BUG_ON(rctx->tkn->input_tkn_len > ctx->tkn_input_len);
		rctx->tkn->devid 	= ctx->dev->dev_id;
		rctx->tkn->priority 	= apm_sec_COS_DEF;
		rctx->tkn->sa	        = session->sa;
	} else {
		sec_tkn_update_espout_transport_gcm((u32 *) in_tkn,
					req->cryptlen,
					session->sa_ib->sa_ptr->crypto_alg,
					pdl, nh, icvsize);
	}
	
	rctx->tkn->context     = &req->base;
	rctx->tkn->src	       = NULL;
	if (apm_sec_sg_scattered(req->cryptlen, req->dst)) {
		rctx->tkn->result_type     = IPE_TKN_RESULT_AMEM;
		rctx->tkn->amem_result.len = sizeof(struct ip_esp_hdr) +
						alg->cra_aead.ivsize + pdl +
						req->cryptlen + icvsize;
		rctx->tkn->amem_result.offset = 0;
		rctx->tkn->amem_result.buf = kmalloc(rctx->tkn->mem_result.len,
							GFP_KERNEL);
		if (rctx->tkn->amem_result.buf == NULL) {
			rc = -ENOMEM;
			goto out_chk;
		}
		rc = apm_sec_setup_crypto(&req->base);
	} else {
		void *dpage;
		rctx->tkn->result_type 	  = IPE_TKN_RESULT_MMEM;
		rctx->tkn->mem_result.len = sizeof(struct ip_esp_hdr) + 
						alg->cra_aead.ivsize + pdl +
						req->cryptlen + icvsize;
		rctx->tkn->mem_result.offset = 0;
		dpage = kmap_atomic(sg_page(&req->dst[0]));
		rctx->tkn->mem_result.buf = dpage + req->dst[0].offset;
		rc = apm_sec_setup_crypto(&req->base);
		kunmap_atomic(dpage);		
	}
out_chk:
	if (rc == -EINPROGRESS || rc == -EAGAIN)
		return rc; 
		
	if (rc != 0)
		APMSEC_TXLOG("IPE%d encrypt gcm esp failed error %d", 
				rc); 
	
	if (rctx->tkn)
		apm_sec_tkn_free(ctx, rctx->tkn);
	return rc;
}

static int apm_sec_decrypt_esp_transport_gcm(struct aead_request *req)
{
	struct apm_sec_session_ctx     *ctx  = crypto_tfm_ctx(req->base.tfm);
	struct apm_sec_req_ctx *rctx = aead_request_ctx(req);
	struct crypto_aead 	 *aead = crypto_aead_reqtfm(req);
	struct crypto_alg 	 *alg  = req->base.tfm->__crt_alg;

	struct sec_tkn_input_hdr *in_tkn;
	int icvsize = crypto_aead_authsize(aead);
	unsigned char 	new_tkn;
	int		rc;
	int		ihl;
	void 		*page;
	int result_len;
	int ivsize;
	
	APMSEC_TXLOG("IPE%d decrypt esp %s nbytes %d", 
			alg->cra_name, req->cryptlen); 
	
	rctx->sa  = NULL;
	rctx->tkn = apm_sec_tkn_get(ctx, &new_tkn);
	if (rctx->tkn == NULL)
		return -ENOMEM;
	/* Extract IHL length */
	page = kmap_atomic(sg_page(&req->src[0]));
	ihl  = ((struct iphdr *) (page + req->src[0].offset))->ihl << 2;	
	kunmap_atomic(page);
	
	in_tkn = TKN_CTX_INPUT_TKN(rctx->tkn);
	APMSEC_SATKNLOG("IPE%d tkn 0x%p outtkn 0x%p intkn 0x%p",
			rctx->tkn, 
			TKN_CTX_RESULT_TKN(rctx->tkn), in_tkn);
	if (new_tkn) {
		rctx->tkn->input_tkn_len = sec_tkn_set_espin_transport_gcm(
					(u32 *) in_tkn,
					req->cryptlen,
					apm_sec_v2hwaddr
					(session->sa_ib->sa_ptr),
					(u32 *) session->sa_ib->sa_ptr,
					session->sa_ib->sa_ptr->crypto_alg,
					ihl,
					icvsize,
					SA_SEQ_OFFSET(session->sa->sa_ptr));
		BUG_ON(rctx->tkn->input_tkn_len > ctx->tkn_input_len);
		rctx->tkn->devid	= ctx->dev->dev_id;
		rctx->tkn->priority 	= apm_sec_COS_DEF; 
		rctx->tkn->sa	        = session->sa_ib;
	} else {
		sec_tkn_update_espin_transport_gcm((u32 *) in_tkn,
					req->cryptlen, 
					session->sa_ib->sa_ptr->crypto_alg, 
					ihl, icvsize);
	}

	rctx->tkn->context     = &req->base;
	rctx->tkn->src	       = NULL;
	ivsize = alg->cra_aead.ivsize;

	result_len = req->cryptlen - icvsize - 8 - ivsize;

	if (apm_sec_sg_scattered(result_len, req->dst)) {
		rctx->tkn->result_type     = IPE_TKN_RESULT_AMEM;
		rctx->tkn->amem_result.len = result_len;
		rctx->tkn->amem_result.buf = kmalloc(result_len, GFP_KERNEL);
		if (rctx->tkn->amem_result.buf == NULL) {
			rc = -ENOMEM;
			goto out_chk;
		} 
		rctx->tkn->amem_result.offset = 0;
		rc = apm_sec_setup_crypto(&req->base);
	} else {
		void *dpage;
		rctx->tkn->result_type 	     = IPE_TKN_RESULT_MMEM;
		rctx->tkn->mem_result.len    = result_len;
		rctx->tkn->mem_result.offset = 0;
		dpage = kmap_atomic(sg_page(&req->dst[0]));
		rctx->tkn->mem_result.buf = dpage + req->dst[0].offset;
		rc = apm_sec_setup_crypto(&req->base);
		kunmap_atomic(dpage);		
	}
out_chk:
	if (rc == -EINPROGRESS || rc == -EAGAIN)
		return rc; 
		
	if (rc != 0)
		APMSEC_TXLOG("IPE%d decrypt ccm failed error %d", 
				rc); 
	
	if (rctx->tkn)
		apm_sec_tkn_free(ctx, rctx->tkn);
	return rc;
}

/**
 * AH Transport/Tunnel SetKey Routines
 *
 */

static int __apm_sec_setkey_ah(struct crypto_aead *cipher, const u8 *key,
				  	unsigned int keylen, unsigned char ha,
					unsigned char xcbc_flag)
{
	struct crypto_tfm    *tfm = crypto_aead_tfm(cipher);
	struct apm_sec_session_ctx *ctx = crypto_tfm_ctx(tfm);
	struct rtattr 	     *rta = (void *) key;
	struct ah_param {
		__be32 spi;
		__be32 seq;
	} *param;
	unsigned int authkeylen;
	unsigned int sa_len;
	unsigned int sa_max_len;
	int rc = -EINVAL;

	if (!RTA_OK(rta, keylen))
		goto badkey;
	if (rta->rta_type != CRYPTO_AUTHENC_KEYA_PARAM)
		goto badkey;
	if (RTA_PAYLOAD(rta) < sizeof(*param))
		goto badkey;

	param     = RTA_DATA(rta);

	key    += RTA_ALIGN(rta->rta_len);
	keylen -= RTA_ALIGN(rta->rta_len);

	authkeylen = keylen;

	if (xcbc_flag) {
		sa_len  = 8 + SA_compute_digest_len((ha),
			SA_DIGEST_TYPE_HASH) + 4 /* SPI */+
			SA_compute_seq_len
				(SA_SEQ_32BITS /* Assuming 32bit seq*/);

		sa_max_len  = 8 + 512/8 + SA_compute_digest_len((ha),
			SA_DIGEST_TYPE_HASH) + 4 /* SPI */+
			SA_compute_seq_len
				(SA_SEQ_32BITS /* Assuming 32bit seq*/);
	}
	else {
		sa_len  = 8 + SA_compute_digest_len((ha),
			SA_DIGEST_TYPE_HMAC) + 4 /* SPI */+
			SA_compute_seq_len
				(SA_SEQ_32BITS /* Assuming 32bit seq*/);
	
		sa_max_len  = 8 + 512/8 + SA_compute_digest_len((ha),
			SA_DIGEST_TYPE_HMAC) + 4 /* SPI */+
			SA_compute_seq_len
				(SA_SEQ_32BITS /* Assuming 32bit seq*/);
	}
	/* As key length can change, we need to flush the cache sa/token */
	apm_sec_free_sa_tkn_pool(ctx);
	/* Re-create the SA/token pool */
	rc = apm_sec_create_sa_tkn_pool(ctx, sa_max_len, sa_len, 1,
			IPE_TKN_AHOUT_TUNNEL_MAXSIZE);
	if (rc != 0)
		goto err_nomem;

	APMSEC_DEBUG("authenc authkeylen %d ha %d sa_len %d icvsize %d",
		authkeylen, ha, session->sa_len, crypto_aead_authsize(cipher));
	APMSEC_DEBUGX_DUMP("authenc auth key: ", key, authkeylen);
	/* Setup SA - NOTE: Control word is in host endian to allow easy
	 * variable reference. Control word in token is in little endian.
	 * Even though we setup both SA (inbound and outbound) only one
	 * can be used.
	 */
	memset(session->sa->sa_ptr, 0, session->sa_len);
	session->sa->sa_ptr->hash_alg   	= ha;
	if (xcbc_flag)
		session->sa->sa_ptr->digest_type 	= SA_DIGEST_TYPE_HASH;
	else
		session->sa->sa_ptr->digest_type 	= SA_DIGEST_TYPE_HMAC;
	session->sa->sa_ptr->ToP		= SA_TOP_HASH_OUTBOUND;
	session->sa->sa_ptr->spi		= 1;
	session->sa->sa_ptr->seq		= SA_SEQ_32BITS;
	session->sa->sa_ptr->iv03		= 0;	/* Use PRNG */
	session->sa->sa_ptr->context_length = (sa_len >> 2) - 2;

	sec_sa_setbe(SA_SPI(session->sa->sa_ptr), param->spi);
	sec_sa_setbe(SA_SEQ(session->sa->sa_ptr), param->seq);
	
	/* Pre-compute inner and outer digest using auth key */
	APMSEC_DEBUG("transport sa 0x%p sa_ptr 0x%p sa_ib 0x%p sa_ib_ptr 0x%p",
		session->sa, session->sa->sa_ptr, session->sa_ib, session->sa_ib->sa_ptr);
	if (xcbc_flag)
		rc = apm_sec_xcbc_digest(key, authkeylen,
			(unsigned char *) SA_DIGEST(session->sa->sa_ptr));
	else 
		rc = apm_sec_compute_ipad_opad(tfm, key, authkeylen,
			(unsigned char *) SA_DIGEST(session->sa->sa_ptr));
	if (rc)
		goto err_nomem;
	APMSEC_DEBUGX_DUMP("sa: ", session->sa->sa_ptr, session->sa_len);
	/* Now copy the same one for inbound */
	memcpy(session->sa_ib->sa_ptr, session->sa->sa_ptr, session->sa_len);
	session->sa_ib->sa_ptr->ToP = SA_TOP_HASH_INBOUND;
	APMSEC_DEBUGX_DUMP("sa_ib: ", session->sa_ib->sa_ptr, session->sa_len);

	APMSEC_DEBUG("alg %s setkey", 
			crypto_tfm_alg_name(tfm));
	return rc;

err_nomem:
	apm_sec_free_sa_tkn_pool(ctx);
	return rc;

badkey:
	crypto_aead_set_flags(cipher, CRYPTO_TFM_RES_BAD_KEY_LEN);
	return rc;
}

static int apm_sec_setkey_ah_md5(struct crypto_aead *cipher, const u8 *key,
				   unsigned int keylen)
{
	return __apm_sec_setkey_ah(cipher, key, keylen,
					SA_HASH_ALG_MD5, 0);
}

static int apm_sec_setkey_ah_sha1(struct crypto_aead *cipher, const u8 *key,
				   unsigned int keylen)
{
	return __apm_sec_setkey_ah(cipher, key, keylen,
					SA_HASH_ALG_SHA1, 0);
}

static int apm_sec_setkey_ah_xcbc(struct crypto_aead *cipher, const u8 *key,
				    unsigned int keylen)
{
	return __apm_sec_setkey_ah(cipher, key, keylen,
					SA_HASH_ALG_XCBC128, 1);
}

static int apm_sec_setkey_ah_sha2(struct crypto_aead *cipher,
				const unsigned char *key,
				unsigned int keylen)
{
	struct crypto_tfm *tfm = crypto_aead_tfm(cipher);
	struct crypto_alg *alg  = tfm->__crt_alg;
	struct aead_alg aalg = alg->cra_aead;

	unsigned char ha = 0;

	switch (aalg.maxauthsize) {
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
	return __apm_sec_setkey_ah(cipher, key, keylen, ha, 0);
}

/**
 * AH Transport/Tunnel Encrypt Routines
 *
 */
static int apm_sec_encrypt_ah_transport(struct aead_request *req)
{
	struct apm_sec_session_ctx     *ctx;
	struct apm_sec_req_ctx *rctx;
	struct sec_tkn_input_hdr *in_tkn;
	struct crypto_aead 	 *aead;
	struct crypto_alg 	 *alg;
	struct iphdr	*iph;
	unsigned char 	new_tkn;
	int		rc;
	int		icvsize;
	void 		*page;
	unsigned char	nh;
	unsigned short  rid;
	unsigned short iphl;	

	ctx  = crypto_tfm_ctx(req->base.tfm);
	rctx = aead_request_ctx(req);
	aead = crypto_aead_reqtfm(req);
	alg  = req->base.tfm->__crt_alg;
	
#if defined(APM_SEC_TXDEBUG)	
	APMSEC_TXLOG("IPE%d auth ah transport %s nbytes %d",
			alg->cra_name, req->cryptlen);
#endif
	rctx->sa  = NULL;
	rctx->tkn = apm_sec_tkn_get(ctx, &new_tkn);
	if (rctx->tkn == NULL)
		return -ENOMEM;
	icvsize = crypto_aead_authsize(aead);
	in_tkn  = TKN_CTX_INPUT_TKN(rctx->tkn);
	APMSEC_SATKNLOG("IPE%d tkn 0x%p outtkn 0x%p intkn 0x%p",
			rctx->tkn, 
			TKN_CTX_RESULT_TKN(rctx->tkn), in_tkn);
	/* Compute pad len and Next header byte */
	page = kmap_atomic(sg_page(&req->src[0]));
	iph   = ((struct iphdr *) (page + req->src[0].offset));
	iphl = iph->ihl << 2;
	nh = iph->protocol;
	rid = iph->id;
	kunmap_atomic(page);

	if (new_tkn) {
		rctx->tkn->input_tkn_len = ipe_tkn_set_ahout_transport(
					(u32 *) in_tkn,
					req->cryptlen,
					apm_sec_v2hwaddr(session->sa->sa_ptr),
					(u32 *) session->sa->sa_ptr, 0, 0,
					iphl, 0, nh, rid, 0, 0,
					icvsize,
					SA_SEQ_OFFSET(session->sa->sa_ptr));
		BUG_ON(rctx->tkn->input_tkn_len > ctx->tkn_input_len);
		rctx->tkn->devid 	= ctx->dev->dev_id;
		rctx->tkn->priority 	= apm_sec_COS_DEF; 
		rctx->tkn->sa	        = session->sa;
	} else {
		ipe_tkn_update_ahout_transport((u32 *) in_tkn,
					req->cryptlen,
					apm_sec_v2hwaddr
					(session->sa->sa_ptr),
					0, rid, iphl, 0, nh, 0,
					0, icvsize);
	}

	rctx->tkn->context     = &req->base;
	rctx->tkn->src	       = NULL;
	if (apm_sec_sg_scattered(req->cryptlen, req->dst)) {
		rctx->tkn->result_type     = IPE_TKN_RESULT_AMEM;
		rctx->tkn->amem_result.len = sizeof(struct ip_auth_hdr) +
						req->cryptlen + icvsize;
		rctx->tkn->amem_result.offset = 0;
		rctx->tkn->amem_result.buf = kmalloc(rctx->tkn->mem_result.len,
							GFP_KERNEL);
		if (rctx->tkn->amem_result.buf == NULL) {
			rc = -ENOMEM;
			goto out_chk;
		}
		rc = apm_sec_setup_crypto(&req->base);
	} else {
		void *dpage;
		rctx->tkn->result_type 	  = IPE_TKN_RESULT_MMEM;
		rctx->tkn->mem_result.len = sizeof(struct ip_auth_hdr) +
						req->cryptlen + icvsize;
		rctx->tkn->mem_result.offset = 0;
		dpage = kmap_atomic(sg_page(&req->dst[0]));
		rctx->tkn->mem_result.buf = dpage + req->dst[0].offset;
		rc = apm_sec_setup_crypto(&req->base);
		kunmap_atomic(dpage);
	}
out_chk:
	if (rc == -EINPROGRESS || rc == -EAGAIN)
		return rc; 
		
	if (rc != 0)
		APMSEC_TXLOG("IPE%d auth ah failed error %d",
				rc);

	if (rctx->tkn)
		apm_sec_tkn_free(ctx, rctx->tkn);
	return rc;
}

static int apm_sec_encrypt_ah_tunnel(struct aead_request *req)
{
	struct apm_sec_session_ctx     *ctx;
	struct apm_sec_req_ctx *rctx;
	struct sec_tkn_input_hdr *in_tkn;
	struct crypto_aead 	 *aead;
	struct crypto_alg 	 *alg;
	struct iphdr	*iph;
	unsigned char 	new_tkn;
	int		rc;
	int		icvsize;
	void 		*page;
	unsigned char	nh;
	unsigned short  rid;
	unsigned short iphl;
	unsigned int saddr;
	unsigned int daddr;	
	unsigned char tos;
	unsigned int result_len;
	unsigned short fragoff;
	ctx  = crypto_tfm_ctx(req->base.tfm);
	rctx = aead_request_ctx(req);
	aead = crypto_aead_reqtfm(req);
	alg  = req->base.tfm->__crt_alg;
	
	APMSEC_TXLOG("IPE%d auth ah tunnel %s nbytes %d",
			alg->cra_name, req->cryptlen);
	rctx->sa  = NULL;
	rctx->tkn = apm_sec_tkn_get(ctx, &new_tkn);
	if (rctx->tkn == NULL)
		return -ENOMEM;
	icvsize = crypto_aead_authsize(aead);
	in_tkn  = TKN_CTX_INPUT_TKN(rctx->tkn);
	APMSEC_SATKNLOG("IPE%d tkn 0x%p outtkn 0x%p intkn 0x%p",
			rctx->tkn, 
			TKN_CTX_RESULT_TKN(rctx->tkn), in_tkn);
	/* Compute Outer Header values to be sent to Hash Engine for ICV */
	page = kmap_atomic(sg_page(&req->dst[0]));
	iph   = ((struct iphdr *) (page + req->dst[0].offset));
	iphl = iph->ihl << 2;
	nh = iph->protocol;
	rid = iph->id;
	saddr = iph->saddr;
	daddr = iph->daddr;
	tos = iph->tos;
	fragoff = iph->frag_off;
	kunmap_atomic(page);

	if (new_tkn) {
		rctx->tkn->input_tkn_len = ipe_tkn_set_ahout_tunnel(
					(u32 *) in_tkn,
					req->cryptlen,
					apm_sec_v2hwaddr
					(session->sa->sa_ptr),
					(u32 *) session->sa->sa_ptr, 0, 0,
					iphl, nh, tos, saddr, daddr, rid,
					0, fragoff, icvsize, 
					SA_SEQ_OFFSET(session->sa->sa_ptr));
		BUG_ON(rctx->tkn->input_tkn_len > ctx->tkn_input_len);
		rctx->tkn->devid 	= ctx->dev->dev_id;
		rctx->tkn->priority 	= apm_sec_COS_DEF; 
		rctx->tkn->sa	        = session->sa;
	} else {
		ipe_tkn_update_ahout_tunnel((u32 *) in_tkn,
					req->cryptlen,
					0, 0, iphl, nh, tos,
					saddr, daddr, rid,
					fragoff, 0);
	}

	rctx->tkn->context     = &req->base;
	rctx->tkn->src	       = NULL;
	result_len = req->cryptlen + iphl + sizeof(struct ip_auth_hdr)
					+ icvsize;
	if (apm_sec_sg_scattered(req->cryptlen, req->dst)) {
		rctx->tkn->result_type     = IPE_TKN_RESULT_AMEM;
		rctx->tkn->amem_result.len = result_len;
		rctx->tkn->amem_result.offset = 0;
		rctx->tkn->amem_result.buf = kmalloc(rctx->tkn->mem_result.len,
							GFP_KERNEL);
		if (rctx->tkn->amem_result.buf == NULL) {
			rc = -ENOMEM;
			goto out_chk;
		}
		rc = apm_sec_setup_crypto(&req->base);
	} else {
		void *dpage;
		rctx->tkn->result_type 	  = IPE_TKN_RESULT_MMEM;
		rctx->tkn->mem_result.len = result_len;
		rctx->tkn->mem_result.offset = 0;
		dpage = kmap_atomic(sg_page(&req->dst[0]));
		rctx->tkn->mem_result.buf = dpage + req->dst[0].offset;
		rc = apm_sec_setup_crypto(&req->base);
		kunmap_atomic(dpage);
	}
out_chk:
	if (rc == -EINPROGRESS || rc == -EAGAIN)
		return rc; 
		
	if (rc != 0)
		APMSEC_TXLOG("IPE%d auth ah tunnel failed error %d",
				rc); 
	
	if (rctx->tkn)
		apm_sec_tkn_free(ctx, rctx->tkn);
	return rc;
}

/**
 * AH Transport/Tunnel Decrypt Routines
 *
 */
static int apm_sec_decrypt_ah_transport(struct aead_request *req)
{
	struct apm_sec_session_ctx     *ctx  = crypto_tfm_ctx(req->base.tfm);
	struct apm_sec_req_ctx *rctx = aead_request_ctx(req);
	struct crypto_aead 	 *aead = crypto_aead_reqtfm(req);
#ifdef APM_SEC_TXDEBUG	
	struct crypto_alg 	 *alg  = req->base.tfm->__crt_alg;
#endif
	struct sec_tkn_input_hdr *in_tkn;
	unsigned char 	new_tkn;
	int		rc;
	void 		*page;
	struct iphdr	*iph;
	struct ip_auth_hdr  *ah;
	int 		result_len;
	int 		icvsize;
	unsigned char	nh;
	unsigned short  rid;
	unsigned short  iphl;

	APMSEC_TXLOG("IPE%d decrypt ah transport %s nbytes %d",
			alg->cra_name, req->cryptlen); 
	
	rctx->sa  = NULL;
	rctx->tkn = apm_sec_tkn_get(ctx, &new_tkn);
	if (rctx->tkn == NULL)
		return -ENOMEM;
	/* Extract IHL length */
	icvsize = crypto_aead_authsize(aead);
	page = kmap_atomic(sg_page(&req->src[0]));
	iph  = ((struct iphdr *) (page + req->src[0].offset));
	iphl = iph->ihl << 2;
	rid = iph->id;
	ah	= (struct ip_auth_hdr *) (page + req->src[0].offset + iphl);
	nh = ah->nexthdr;
	kunmap_atomic(page);
	
	in_tkn = TKN_CTX_INPUT_TKN(rctx->tkn);
	APMSEC_SATKNLOG("IPE%d tkn 0x%p outtkn 0x%p intkn 0x%p",
			rctx->tkn, 
			TKN_CTX_RESULT_TKN(rctx->tkn), in_tkn);
	if (new_tkn) {
		rctx->tkn->input_tkn_len = ipe_tkn_set_ahin_transport(
					(u32 *) in_tkn,
					req->cryptlen,
					apm_sec_v2hwaddr
					(session->sa_ib->sa_ptr),
					(u32 *) session->sa_ib->sa_ptr, 0,
					iphl, 0, nh, rid, 0, 0,
					icvsize,
					SA_SEQ_OFFSET(session->sa_ib->sa_ptr));
		BUG_ON(rctx->tkn->input_tkn_len > ctx->tkn_input_len);
		rctx->tkn->devid	= ctx->dev->dev_id;
		rctx->tkn->priority 	= apm_sec_COS_DEF;
		rctx->tkn->sa	        = session->sa_ib;
	} else {
		ipe_tkn_update_ahin_transport((u32 *) in_tkn,
					req->cryptlen, 0, rid, iphl, 0, nh, 0,
					0, icvsize);
	}
	rctx->tkn->context     = &req->base;
	rctx->tkn->src	       = NULL;
	result_len = req->cryptlen - sizeof(struct ip_auth_hdr)
					- icvsize;
	if (apm_sec_sg_scattered(result_len, req->dst)) {
		rctx->tkn->result_type     = IPE_TKN_RESULT_AMEM;
		rctx->tkn->amem_result.len = result_len;
		rctx->tkn->amem_result.buf = kmalloc(result_len, GFP_KERNEL);
		if (rctx->tkn->amem_result.buf == NULL) {
			rc = -ENOMEM;
			goto out_chk;
		}
		rctx->tkn->amem_result.offset = 0;
		rc = apm_sec_setup_crypto(&req->base);
	} else {
		void *dpage;
		rctx->tkn->result_type 	     = IPE_TKN_RESULT_MMEM;
		rctx->tkn->mem_result.len    = result_len;
		rctx->tkn->mem_result.offset = 0;
		dpage = kmap_atomic(sg_page(&req->dst[0]));
		rctx->tkn->mem_result.buf = dpage + req->dst[0].offset;
		rc = apm_sec_setup_crypto(&req->base);
		kunmap_atomic(dpage);		
	}
out_chk:
	if (rc == -EINPROGRESS || rc == -EAGAIN)
		return rc; 
		
	if (rc != 0)
		APMSEC_TXLOG("IPE%d decrypt ah transport failed error %d",
				rc); 
	
	if (rctx->tkn)
		apm_sec_tkn_free(ctx, rctx->tkn);
	return rc;
}

static int apm_sec_decrypt_ah_tunnel(struct aead_request *req)
{
	struct apm_sec_session_ctx     *ctx  = crypto_tfm_ctx(req->base.tfm);
	struct apm_sec_req_ctx *rctx = aead_request_ctx(req);
	struct crypto_aead 	 *aead = crypto_aead_reqtfm(req);
#ifdef APM_SEC_TXDEBUG	
	struct crypto_alg 	 *alg  = req->base.tfm->__crt_alg;
#endif
	struct sec_tkn_input_hdr *in_tkn;
	unsigned char 	new_tkn;
	int		rc;
	void 		*page;
	struct iphdr	*iph;
	int 		result_len;
	int 		icvsize;
	unsigned short  rid;
	unsigned short iphl;

	APMSEC_TXLOG("IPE%d decrypt ah tunnel %s nbytes %d",
			alg->cra_name, req->cryptlen);
	
	rctx->sa  = NULL;
	rctx->tkn = apm_sec_tkn_get(ctx, &new_tkn);
	if (rctx->tkn == NULL)
		return -ENOMEM;
	/* Extract IHL length */
	icvsize = crypto_aead_authsize(aead);
	page = kmap_atomic(sg_page(&req->src[0]));
	iph  = ((struct iphdr *) (page + req->src[0].offset));
	iphl = iph->ihl << 2;
	rid = iph->id;
	kunmap_atomic(page);
	
	in_tkn = TKN_CTX_INPUT_TKN(rctx->tkn);
	APMSEC_SATKNLOG("IPE%d tkn 0x%p outtkn 0x%p intkn 0x%p",
			rctx->tkn, 
			TKN_CTX_RESULT_TKN(rctx->tkn), in_tkn);
	if (new_tkn) {
		rctx->tkn->input_tkn_len = ipe_tkn_set_ahin_tunnel(
					(u32 *) in_tkn,
					req->cryptlen,
					apm_sec_v2hwaddr(session->sa_ib->sa_ptr),
					(u32 *) session->sa_ib->sa_ptr, 0,
					0, iphl, rid, icvsize,
					SA_SEQ_OFFSET(session->sa_ib->sa_ptr));
		BUG_ON(rctx->tkn->input_tkn_len > ctx->tkn_input_len);
		rctx->tkn->devid	= ctx->dev->dev_id;
		rctx->tkn->priority 	= apm_sec_COS_DEF;
		rctx->tkn->sa	        = session->sa_ib;
	} else {
		ipe_tkn_update_ahin_tunnel((u32 *) in_tkn,
					req->cryptlen, 0, 0, iphl, rid);
	}
	rctx->tkn->context     = &req->base;
	rctx->tkn->src	       = NULL;
	result_len = req->cryptlen - sizeof(struct ip_auth_hdr)
			 		- icvsize - iphl;

	if (apm_sec_sg_scattered(result_len, req->dst)) {
		rctx->tkn->result_type     = IPE_TKN_RESULT_AMEM;
		rctx->tkn->amem_result.len = result_len;
		rctx->tkn->amem_result.buf = kmalloc(result_len, GFP_KERNEL);
		if (rctx->tkn->amem_result.buf == NULL) {
			rc = -ENOMEM;
			goto out_chk;
		}
		rctx->tkn->amem_result.offset = 0;
		rc = apm_sec_setup_crypto(&req->base);
	} else {
		void *dpage;
		rctx->tkn->result_type 	     = IPE_TKN_RESULT_MMEM;
		rctx->tkn->mem_result.len    = result_len;
		rctx->tkn->mem_result.offset = 0;
		dpage = kmap_atomic(sg_page(&req->dst[0]));
		rctx->tkn->mem_result.buf = dpage + req->dst[0].offset;
		rc = apm_sec_setup_crypto(&req->base);
		kunmap_atomic(dpage);		
	}
out_chk:
	if (rc == -EINPROGRESS || rc == -EAGAIN)
		return rc; 
		
	if (rc != 0)
		APMSEC_TXLOG("IPE%d decrypt ah tunnel failed error %d",
				rc); 
	
	if (rctx->tkn)
		apm_sec_tkn_free(ctx, rctx->tkn);
	return rc;
}
#endif

/*
 * Kasumi F8 Functions
 *
 */
static inline int apm_sec_setkey_kasumi(struct crypto_ablkcipher *cipher,
					     const unsigned char *key,
					     unsigned int keylen, int mode)
{
	struct crypto_tfm *tfm = crypto_ablkcipher_tfm(cipher);
	struct apm_sec_session_ctx *ctx = crypto_tfm_ctx(tfm);
	int rc;
	
	if (keylen != 16) {
		crypto_ablkcipher_set_flags(cipher, CRYPTO_TFM_RES_BAD_KEY_LEN);
		return -1;
	}

	rc = apm_sec_create_sa_tkn_pool(ctx, 
			SA_KASUMI_F8_LEN, SA_KASUMI_F8_LEN,
			0, TKN_BASIC_MAX_LEN);
	if (rc != 0)
		goto err_nomem;

	sec_sa_setbe(SA_KEY(ctx->sa->sa_ptr), key, keylen);
	ctx->sa->sa_ptr->crypto_alg 	     = SA_CRYPTO_ALG_KASUMI_F8;
	ctx->sa->sa_ptr->cryptomode_feedback = mode;
	ctx->sa->sa_ptr->key		     = 1;
	ctx->sa->sa_ptr->context_length = (ctx->sa_len >> 2) - 2;

	return 0;

err_nomem:
	apm_sec_free_sa_tkn_pool(ctx);

	return rc;
}

static inline int apm_sec_setkey_kasumi_f8(struct crypto_ablkcipher *cipher,
					const unsigned char *key,
					unsigned int keylen)
{
	return apm_sec_setkey_kasumi(cipher, key, keylen, SA_CRYPTO_MODE_KASUMI_F8);
}

static inline int apm_sec_setkey_kasumi_p(struct crypto_ablkcipher *cipher,
					   const unsigned char *key,
					   unsigned int keylen)
{
	return apm_sec_setkey_kasumi(cipher, key, keylen, SA_CRYPTO_MODE_KASUMI);
}

/* Kasumi F9 - Hash Algorithms */
static int apm_sec_kasumi_f9_setkey(struct crypto_ahash *hash,
				const unsigned char *key, unsigned int keylen)
{
	struct crypto_tfm	*tfm = crypto_ahash_tfm(hash);
	struct apm_sec_session_ctx	*ctx = crypto_tfm_ctx(tfm);
	int 	rc;
	int	direction;

	if (keylen != 16 + 1 /* 1 for direction */ ) {
		crypto_ahash_set_flags(hash, CRYPTO_TFM_RES_BAD_KEY_LEN);
		return -1;
	}
	direction = key[16] ? 1 : 0;
	keylen--;

	/* As direction can change, we need to flush the cache sa/token */
	apm_sec_free_sa_tkn_pool(ctx);
	/* Re-create the SA/token pool */
	rc = apm_sec_create_sa_tkn_pool(ctx,
			SA_KASUMI_F9_LEN, SA_KASUMI_F9_LEN,
			0, TKN_BASIC_MAX_LEN);
	if (rc != 0)
		goto err_nomem;
	ctx->sa->sa_ptr->ToP	     = SA_TOP_HASH_OUTBOUND;
	ctx->sa->sa_ptr->hash_alg    = SA_HASH_ALG_KASUMI_F9;
	ctx->sa->sa_ptr->digest_type = SA_DIGEST_TYPE_HASH;
	ctx->sa->sa_ptr->hash_store  = 1;
	ctx->sa->sa_ptr->context_length = (ctx->sa_len >> 2) - 2;
	ctx->sa->sa_ptr->kasumi_f9_direction = direction;
	ctx->sa->sa_ptr->cryptomode_feedback = 1;
	/* Load hash key */
	sec_sa_setbe(SA_DIGEST(ctx->sa->sa_ptr), key, keylen);

	APMSEC_DEBUG("alg %s setkey", 
			crypto_tfm_alg_name(tfm));
	return 0;

err_nomem:
	apm_sec_free_sa_tkn_pool(ctx);

	return rc;
}

#if 0
/*
 * AES-GCM (RFC4106) IPSec Authenc Routines
 *
 */
static int apm_sec_setkey_aes_gcm_rfc4106(struct crypto_aead *cipher,
					const unsigned char *key,
					unsigned int keylen)
{

	struct crypto_tfm    *tfm = crypto_aead_tfm(cipher);
	struct apm_sec_session_ctx *ctx = crypto_tfm_ctx(tfm);
	int 	rc;

	if (keylen != 128/8 + 4 && keylen != 192/8 + 4 &&
	    keylen != 256/8 + 4) {
		crypto_aead_set_flags(cipher, CRYPTO_TFM_RES_BAD_KEY_LEN);
		return -EINVAL;
	}

	keylen -= 4;	/* salt (nonce) is at last 4 bytes */
	
	rc = apm_sec_create_sa_tkn_pool(ctx, 
			SA_AES_GCM_MAX_LEN, SA_AES_GCM_LEN(keylen),
			0, TKN_BASIC_MAX_LEN);
	if (rc != 0)
		goto err_nomem;

	/* Setup SA - Fill SA with proper value. For AES-GCM, we will
	   have a SA for each request */
	memset(session->sa, 0, session->sa_len);
	session->sa->sa_ptr->hash_alg   	= SA_HASH_ALG_GHASH;
	session->sa->sa_ptr->digest_type 	= SA_DIGEST_TYPE_HASH;
	session->sa->sa_ptr->key 		= 1;
	switch(keylen) {
	case 128/8:
		session->sa->sa_ptr->crypto_alg 	= SA_CRYPTO_ALG_AES128;
		break;
	case 192/8:
		session->sa->sa_ptr->crypto_alg 	= SA_CRYPTO_ALG_AES192;
		break;
	case 256/8:
		session->sa->sa_ptr->crypto_alg 	= SA_CRYPTO_ALG_AES256;
		break;
	}
	session->sa->sa_ptr->cryptomode_feedback    =
			SA_CRYPTO_MODE_AES_3DES_CTR;

	session->sa->sa_ptr->spi		= 1;
	session->sa->sa_ptr->seq		= SA_SEQ_32BITS;
					/* assume 32-bits seq */
	session->sa->sa_ptr->iv_format	= SA_IV_FORMAT_COUNTER_MODE;
	session->sa->sa_ptr->iv03		= 0x7;
	session->sa->sa_ptr->mask		= SA_MASK_NONE;
	session->sa->sa_ptr->context_length = (session->sa_len >> 2) - 2;

	sec_sa_setle(SA_KEY(session->sa), key, keylen);
	/* Save salt into IV0; IV1 & IV2 will be filled later from request */
	sec_sa_setle(SA_IV(session->sa), key + keylen, 4);

	memcpy(session->sa_ib, session->sa, session->sa_len);
	session->sa_ib->sa_ptr->mask = SA_MASK_64;    /* assum 64-bit mask */
	
	APMSEC_DEBUG("alg %s setkey", 
			crypto_tfm_alg_name(tfm));
	return 0;

err_nomem:
	apm_sec_free_sa_tkn_pool(ctx);

	return rc;
}
#endif
/**
 * AES-CCM (RFC4309) IPSec Functions
 *
 */
static int apm_sec_setkey_esp_ccm(struct crypto_aead *cipher,
					const unsigned char *key,
					unsigned int keylen)
{
	struct crypto_tfm    *tfm = crypto_aead_tfm(cipher);
	struct apm_sec_session_ctx *session = crypto_tfm_ctx(tfm);
	struct rtattr 	     *rta = (void *) key;
	struct esp_authenc_param {
		__be32 spi;
		__be32 seq;
		__be16 pad_block_size;
		__be16 encap_uhl;
		struct crypto_authenc_key_param authenc_param;
	} *param;
	int 	rc = -EINVAL;

	if (!RTA_OK(rta, keylen))
		goto badkey;
	
	param     = RTA_DATA(rta);

	key    += RTA_ALIGN(rta->rta_len);
	keylen -= RTA_ALIGN(rta->rta_len);
		
	if (keylen != 128/8 + 3 && keylen != 192/8 + 3 &&
			keylen != 256/8 + 3)
		goto badkey;

	keylen -= 3;	/* salt value is 3 bytes */
	
	apm_sec_free_sa_tkn_pool(session);
	rc = apm_sec_create_sa_tkn_pool(session, SA_AES_ESP_CCM_MAX_LEN,
					  SA_AES_ESP_CCM_LEN(keylen),
					  1, TKN_AES_CCM_MAX);
	if (rc != 0)
		goto err_nomem;

	memset(session->sa->sa_ptr, 0, session->sa_len);
	switch(keylen) {
	case 128/8:
		session->sa->sa_ptr->crypto_alg 	= SA_CRYPTO_ALG_AES128;
		session->sa->sa_ptr->hash_alg   	= SA_HASH_ALG_XCBC128;
		break;
	case 192/8:
		session->sa->sa_ptr->crypto_alg 	= SA_CRYPTO_ALG_AES192;
		session->sa->sa_ptr->hash_alg   	= SA_HASH_ALG_XCBC192;
		break;
	case 256/8:
		session->sa->sa_ptr->crypto_alg 	= SA_CRYPTO_ALG_AES256;
		session->sa->sa_ptr->hash_alg   	= SA_HASH_ALG_XCBC256;
		break;
	}
	session->sa->sa_ptr->digest_type 	= SA_DIGEST_TYPE_HASH;
	session->sa->sa_ptr->key 		= 1;
	session->sa->sa_ptr->cryptomode_feedback =
				SA_CRYPTO_MODE_AES_3DES_CTR_LOAD_REUSE;
	session->sa->sa_ptr->iv03		= 0xF;

	session->sa->sa_ptr->ToP		= SA_TOP_HASH_ENCRYPT_OUTBOUND;
	session->sa->sa_ptr->spi		= 1;
	session->sa->sa_ptr->seq		= SA_SEQ_32BITS;
	session->sa->sa_ptr->enc_hash_result= 1;

	session->sa->sa_ptr->pad_type	= SA_PAD_TYPE_IPSEC;
	session->sa->sa_ptr->context_length = (session->sa_len >> 2) - 2;
	
	sec_sa_set32le(SA_SPI(session->sa->sa_ptr), param->spi);
	sec_sa_set32le(SA_SEQ(session->sa->sa_ptr), param->seq);
	
	session->pad_block_size = be16_to_cpu(param->pad_block_size);
	session->encap_uhl      = be16_to_cpu(param->encap_uhl);

	sec_sa_setbe(SA_KEY(session->sa->sa_ptr), key, keylen);
	/* Load K2, K3, and K1 digest */
	rc = apm_sec_xcbc_digest(key, keylen,
				(unsigned char *)
				SA_DIGEST(session->sa->sa_ptr));
	if (rc)
		goto err_nomem;
	/* Load salt (nonce) into IV0 */
	memcpy(SA_IV(session->sa->sa_ptr), key + keylen, 3);
	*((unsigned char *) SA_IV(session->sa->sa_ptr) + 3) = 0;
#if 0
	memcpy(SA_IV(session->sa->sa_ptr) + 1, 0xA1A2A3A4, 4);
	memcpy(SA_IV(session->sa->sa_ptr) + 2, 0xA5A6A7A8, 4);
#endif
	/* Now copy the same one for inbound */
	memcpy(session->sa_ib->sa_ptr, session->sa->sa_ptr, session->sa_len);
	session->sa_ib->sa_ptr->iv03	= 0x7;
	session->sa_ib->sa_ptr->ToP     = SA_TOP_DECRYPT_HASH_INBOUND;
	APMSEC_DEBUG("authenc sa 0x%p sa_ptr 0x%p sa_ib 0x%p sa_ib_ptr 0x%p",
		session->sa, session->sa->sa_ptr, session->sa_ib, session->sa_ib->sa_ptr);
	APMSEC_DEBUGX_DUMP("sa: ", session->sa->sa_ptr, session->sa_len);

	session->sa_ib->sa_ptr->ToP = SA_TOP_DECRYPT_HASH_INBOUND;
	APMSEC_DEBUGX_DUMP("sa_ib: ", session->sa_ib->sa_ptr, session->sa_len);

	APMSEC_DEBUG("alg %s setkey", 
			crypto_tfm_alg_name(tfm));
	return rc;

err_nomem:
	apm_sec_free_sa_tkn_pool(session);
	return rc;
badkey:
	crypto_aead_set_flags(cipher, CRYPTO_TFM_RES_BAD_KEY_LEN);
	return rc;
}

static int apm_sec_decrypt_esp_transport_ccm(struct aead_request *req)
{

	struct apm_sec_session_ctx     *session = crypto_tfm_ctx(req->base.tfm);
	struct apm_sec_req_ctx *rctx = aead_request_ctx(req);
	struct sec_tkn_input_hdr *in_tkn;
	struct crypto_aead 	 *aead = crypto_aead_reqtfm(req);
	struct crypto_alg 	 *alg  = req->base.tfm->__crt_alg;
	struct iphdr	*iph;
	unsigned char iv_B0[16];
	unsigned char adata_length[6];
	unsigned char 	new_tkn;
	int		rc;
	int		icvsize;
	int 		ivsize;
	void 		*page;
	int		pdl;
	unsigned char	nh;
	int		clen;
	void *sa_align;
	
	APMSEC_TXLOG("IPE decrypt esp %s nbytes %d",
			alg->cra_name, req->cryptlen);
	rctx->sa  = NULL;
	rctx->tkn = apm_sec_tkn_get(session, &new_tkn);
	if (rctx->tkn == NULL)
		return -ENOMEM;
	icvsize = crypto_aead_authsize(aead);
	in_tkn  = TKN_CTX_INPUT_TKN(rctx->tkn);
	APMSEC_SATKNLOG("IPE tkn 0x%p outtkn 0x%p intkn 0x%p",
			rctx->tkn, 
			TKN_CTX_RESULT_TKN(rctx->tkn), in_tkn);
	sa_align = SA_PTR_ALIGN(session->sa_ib->sa_ptr);
	/* Compute pad len and Next header byte */
	page = kmap_atomic(sg_page(&req->src[0]));
	iph   = ((struct iphdr *) (page + req->src[0].offset));
	nh = iph->protocol;
	clen = req->cryptlen - (iph->ihl << 2);
	kunmap_atomic(page);
	
	pdl = ALIGN(clen + 2, ALIGN(crypto_aead_blocksize(aead), 4));
	if (session->pad_block_size)
		pdl = ALIGN(pdl, session->pad_block_size);
	pdl -= clen;

	/* Form IV B0 salt and flags for hashing */
	//sec_tkn_setle(iv_B0, SA_IV(session->sa_ib->sa_ptr), 4);
	sec_tkn_setle((unsigned int *)iv_B0, (unsigned char *)
			SA_IV(session->sa_ib->sa_ptr), 4);
	memset(iv_B0 + 12, 0, 4);
	memset(adata_length, 0, 6);
	/* format control info per RFC 3610 and NIST Special Publication
	800-38C */
	iv_B0[0] |= 3;
	iv_B0[0] |= (8 * ((crypto_aead_authsize(aead) - 2) / 2));
	iv_B0[0] |= 64;
	/* Taking L length as 2 bytes*/
	ivsize = alg->cra_aead.ivsize;
	rctx->tkn->dest_nbytes = req->cryptlen - icvsize - 8 - ivsize;
	rc = sec_sa_set_aes_ccm_msg_len(iv_B0 + (16 - 4),
					84, 4);
	if (rc != 0) {
		rc = -EOVERFLOW;
		goto err_dec_aes_ccm;
	}
	sec_sa_set_aes_ccm_adata(adata_length, 8 /* SPI + Seq*/);
	rctx->tkn->input_tkn_len = sec_tkn_set_espin_transport_ccm(
					(u32 *) in_tkn,
					req->cryptlen,
					apm_sec_v2hwaddr(sa_align),
					(u32 *) session->sa_ib->sa_ptr,
					pdl,
					icvsize,
					nh, iv_B0, adata_length,
					SA_SEQ_OFFSET(session->sa_ib->sa_ptr));
	BUG_ON(rctx->tkn->input_tkn_len > session->tkn_input_len);
	rctx->tkn->sa	        = session->sa_ib;
	rctx->tkn->context     = &req->base;
	rc = apm_sec_setup_crypto(&req->base);
	if (rc == -EINPROGRESS)
		return rc;
err_dec_aes_ccm:
	if (rc != 0)
		APMSEC_TXLOG("IPE ccm decrypt esp failed error %d",
			     rc);
	if (rctx->tkn)
		apm_sec_tkn_free(session, rctx->tkn);
	return rc;
}

static int apm_sec_encrypt_esp_transport_ccm(struct aead_request *req)
{
	struct apm_sec_session_ctx     *session;
	struct apm_sec_req_ctx *rctx;
	struct sec_tkn_input_hdr *in_tkn;
	struct crypto_aead 	 *aead;
	struct crypto_alg 	 *alg;
	struct iphdr	*iph;
	unsigned char iv_B0[16];
	unsigned char adata_length[6];
	unsigned char 	new_tkn;
	int		rc;
	int		icvsize;
	void 		*page;
	int		pdl;
	unsigned char	nh;
	unsigned char	iphl;
	int		clen;	
	void *sa_align;

	session  = crypto_tfm_ctx(req->base.tfm);
	rctx = aead_request_ctx(req);
	aead = crypto_aead_reqtfm(req);
	alg  = req->base.tfm->__crt_alg;

	APMSEC_TXLOG("IPE encrypt esp %s nbytes %d",
			alg->cra_name, req->cryptlen);
	rctx->sa  = NULL;
	rctx->tkn = apm_sec_tkn_get(session, &new_tkn);
	if (rctx->tkn == NULL)
		return -ENOMEM;
	icvsize = crypto_aead_authsize(aead);
	in_tkn  = TKN_CTX_INPUT_TKN(rctx->tkn);
	APMSEC_SATKNLOG("IPE tkn 0x%p outtkn 0x%p intkn 0x%p",
			rctx->tkn, 
			TKN_CTX_RESULT_TKN(rctx->tkn), in_tkn);
	/* Compute pad len and Next header byte */
	page = kmap_atomic(sg_page(&req->src[0]));
	iph   = ((struct iphdr *) (page + req->src[0].offset));
	nh = iph->protocol;
	iphl = (iph->ihl << 2);
	clen = req->cryptlen - iphl;
	kunmap_atomic(page);
	
	pdl = ALIGN(clen + 2, ALIGN(crypto_aead_blocksize(aead), 4));
	if (session->pad_block_size)
		pdl = ALIGN(pdl, session->pad_block_size);
	pdl -= clen;
	sa_align = SA_PTR_ALIGN(session->sa->sa_ptr);
	/* Form IV B0 salt and flags for hashing */
	sec_tkn_setle((unsigned int *)iv_B0, (unsigned char *)
			SA_IV(session->sa->sa_ptr), 4);
	memset(iv_B0 + 12, 0, 4);
	memset(adata_length, 0, 6);
	/* format control info per RFC 3610 and NIST Special Publication
	800-38C */
	iv_B0[0] |= 3;
	iv_B0[0] |= (8 * ((crypto_aead_authsize(aead) - 2) / 2));
	iv_B0[0] |= 64;
	/* Taking L length as 2 bytes*/
	rc = sec_sa_set_aes_ccm_msg_len(iv_B0 + (16 - 4), req->cryptlen, 4);
	if (rc != 0) {
		rc = -EOVERFLOW;
		goto err_enc_aes_ccm;
	}
	sec_sa_set_aes_ccm_adata(adata_length, 8 /* SPI + Seq*/);
	

	rctx->tkn->input_tkn_len = sec_tkn_set_espout_transport_ccm(
				(u32 *) in_tkn,
				req->cryptlen,
				apm_sec_v2hwaddr(sa_align),
				(u32 *) session->sa->sa_ptr,
				pdl,
				icvsize,
				nh, iphl, iv_B0, adata_length,
				SA_SEQ_OFFSET(session->sa->sa_ptr));
	BUG_ON(rctx->tkn->input_tkn_len > session->tkn_input_len);
	rctx->tkn->sa	        = session->sa;

	rctx->tkn->context     = &req->base;
	rctx->tkn->dest_nbytes = sizeof(struct ip_esp_hdr) +
			alg->cra_aead.ivsize + pdl +
			req->cryptlen + icvsize;

	rc = apm_sec_setup_crypto(&req->base);
	if (rc == -EINPROGRESS)
		return rc; 
		
	if (rc != 0)
		APMSEC_TXLOG("IPE  encrypt esp failed error %d",
				rc); 
err_enc_aes_ccm:
	if (rctx->tkn)
		apm_sec_tkn_free(session, rctx->tkn);
	return rc;
}
#if 0
static int apm_sec_setkey_aes_xcbc_ipsec(struct crypto_aead *cipher,
				 	   const unsigned char 	*key,
				 	   unsigned int	keylen)
{
	struct crypto_tfm    *tfm = crypto_aead_tfm(cipher);
	struct apm_sec_session_ctx *ctx = crypto_tfm_ctx(tfm);
	struct rtattr 	     *rta = (void *) key;
	struct crypto_authenc_key_param *param;
	struct crypto_blkcipher *blk_tfm = NULL;
	struct blkcipher_desc   blk_desc;
	struct scatterlist 	sg[1];
	unsigned int enckeylen;
	unsigned int authkeylen;
	unsigned int j;
	unsigned int *digest;
	char 	block128[16];
	int 	rc = -EINVAL;

	if (!RTA_OK(rta, keylen))
		goto badkey;
	if (rta->rta_type != CRYPTO_AUTHENC_KEYA_PARAM)
		goto badkey;
	if (RTA_PAYLOAD(rta) < sizeof(*param))
		goto badkey;

	param     = RTA_DATA(rta);
	enckeylen = be32_to_cpu(param->enckeylen);

	key    += RTA_ALIGN(rta->rta_len);
	keylen -= RTA_ALIGN(rta->rta_len);

	authkeylen = keylen - enckeylen;

	if (keylen < enckeylen)
		goto badkey;
	if (keylen != 128/8 && keylen != 192/8 && keylen != 256/8)
		goto badkey;

	rc = apm_sec_create_sa_tkn_pool(ctx, 
			SA_AES_MAX_LEN(SA_CRYPTO_MODE_AES_3DES_CBC), 
			SA_AES_LEN(keylen, SA_CRYPTO_MODE_AES_3DES_CBC), 
			1, sec_tkn_IPSEC_MAX_SIZE);
	if (rc != 0)
		goto err_nomem;

	/* Setup SA */
	memset(session->sa->sa_ptr, 0, session->sa_len);
	session->sa->sa_ptr->crypto_alg 	= SA_keylen2aes_mode(keylen);
	session->sa->sa_ptr->hash_alg   	= SA_HASH_ALG_XCBC128;
	session->sa->sa_ptr->digest_type 	= SA_DIGEST_TYPE_HASH;
	session->sa->sa_ptr->key 		= 1;
	session->sa->sa_ptr->cryptomode_feedback = SA_CRYPTO_MODE_AES_3DES_CBC;
	session->sa->sa_ptr->iv03		= 0xF;
	session->sa->sa_ptr->context_length = (session->sa_len >> 2) - 2;
	sec_sa_setbe2le(SA_KEY(session->sa->sa_ptr), key, keylen);

	/* Load K2, K3, and K1 digest */
	blk_tfm = crypto_alloc_blkcipher("ecb(aes)", 0, 0);
	if (IS_ERR(blk_tfm)) {
		APMSEC_ERR("failed to load helper transform for ecb(aes) "
				"error %ld", PTR_ERR(blk_tfm));
		goto err_nomem;
	}
	blk_desc.tfm   = blk_tfm;
	blk_desc.flags = 0;
	rc = crypto_blkcipher_setkey(blk_tfm, key, keylen);
	if (rc != 0) {
		APMSEC_ERR("failed to set helper transform key for ecb(aes) "
				"error %d", rc);
		goto err_nomem;
	}
	digest = SA_DIGEST(session->sa->sa_ptr);
	for (j = 2; j < 5; j++) {
		int 	i;
		for (i = 0; i < 128/8; i++)
			block128[i] = j == 4 ? 0x01 : j;
		sg_init_one(sg, block128, 128/8);
		rc = crypto_blkcipher_encrypt(&blk_desc, sg, sg, 128/8);
		if (rc != 0) {
			APMSEC_ERR("failed to encrypt hash key error %d", 
					rc);
			goto err_nomem;
		}
		sec_sa_setbe2le(digest, block128, 128/8);
		digest += 4;
	}

	/* Load salt (nonce) into IV0 */
	memcpy(SA_IV(session->sa->sa_ptr), key + keylen, 3);
	*((unsigned char *) SA_IV(session->sa->sa_ptr) + 3) = 0;

	memcpy(session->sa_ib->sa_ptr, session->sa->sa_ptr, session->sa_len);

	APMSEC_DEBUG("alg %s setkey", alg->cra_name);
	return rc;

err_nomem:
	apm_sec_free_sa_tkn_pool(ctx);
	return rc;

badkey:
	crypto_aead_set_flags(cipher, CRYPTO_TFM_RES_BAD_KEY_LEN);
	return rc;
}

/*
 * MACSec Algorithms
 *
 *
 */
static inline int apm_sec_setkey_macsec(struct crypto_aead *cipher,
				const unsigned char *key, 
				unsigned int keylen)
{
	struct crypto_tfm    *tfm = crypto_aead_tfm(cipher);
	struct apm_sec_session_ctx *ctx = crypto_tfm_ctx(tfm);
	int 	rc = -EINVAL;

	/* Key length musts be 16-byte crypto and 16 bytes hash */ 
	if (keylen != 16 + 16) {
		crypto_aead_set_flags(cipher, CRYPTO_TFM_RES_BAD_KEY_LEN);
		return -EINVAL;
	}

	rc = apm_sec_create_sa_tkn_pool(ctx, SA_MACSEC_LEN, 
			SA_MACSEC_LEN, 1, sec_tkn_MACSEC_MAX_SIZE);
	if (rc != 0)
		goto err_nomem;

	/* Setup SA - Fill SA with proper values */
	session->sa->sa_ptr->hash_alg   	= SA_HASH_ALG_GHASH;
	session->sa->sa_ptr->digest_type 	= SA_DIGEST_TYPE_HASH;
	session->sa->sa_ptr->key 		= 1;
	session->sa->sa_ptr->crypto_alg 	= SA_CRYPTO_ALG_AES128;
	session->sa->sa_ptr->cryptomode_feedback = SA_CRYPTO_MODE_AES_3DES_CTR;
	session->sa->sa_ptr->seq		= SA_SEQ_32BITS;
	session->sa->sa_ptr->mask		= SA_MASK_NONE;
	session->sa->sa_ptr->iv_format	= SA_IV_FORMAT_COUNTER_MODE;
	session->sa->sa_ptr->iv03		= 0x7;
	session->sa->sa_ptr->context_length = (session->sa_len >> 2) - 2;
	sec_sa_setbe2le(SA_KEY(session->sa->sa_ptr), key, 16);
	sec_sa_setle(SA_DIGEST(session->sa->sa_ptr), key + 16, 16);
	SA_set32(SA_SEQ(session->sa->sa_ptr), 1);
	
	memcpy(session->sa_ib->sa_ptr, session->sa->sa_ptr, session->sa_len);
	session->sa_ib->sa_ptr->mask = SA_MASK_64;    /* assume 64-bit mask */
	
	APMSEC_DEBUG("alg %s setkey", alg->cra_name);
	return 0;

err_nomem:
	apm_sec_free_sa_tkn_pool(ctx);
	
	return rc;
}

static int apm_sec_setauthsize_macsec(struct crypto_aead *ciper,
				        unsigned int authsize)
{
	struct aead_tfm *tfm = crypto_aead_crt(ciper);

	if (authsize != 16)
		return -EINVAL;

	tfm->authsize = authsize;
	return 0;
}

static int apm_sec_encrypt_macsec(struct aead_request *req,
					unsigned int aes_mode)
{
	struct apm_sec_session_ctx     *ctx  = crypto_tfm_ctx(req->base.tfm);
	struct apm_sec_req_ctx *rctx = aead_request_ctx(req);
	struct sec_tkn_input_hdr	*in_tkn;
	unsigned char 	*adata = NULL;
	unsigned char	new_tkn;

	/* MACSec STI (4) and SCI (8) is stored in associate data field */
	if (req->assoclen != 12) {
		APMSEC_ERR("algorithm MACSec unsupported AAD length %d",
				req->assoclen);
		return -EINVAL;
	}

	/* Allocate token for this request */
	rctx->tkn = apm_sec_tkn_get(ctx, &new_tkn);
	if (rctx->tkn == NULL)
		return -ENOMEM;

	if (req->assoclen)
		adata = kmap_atomic(sg_page(req->assoc));
	in_tkn = TKN_CTX_INPUT_TKN(rctx->tkn);
	if (new_tkn) {
		rctx->tkn->input_tkn_len = sec_tkn_set_out_macsec(
					(u32 *) in_tkn,
					req->cryptlen,					
					apm_sec_v2hwaddr(rctx->sa),
					aes_mode,
					*(u32 *) adata,
					adata + 4);
		rctx->tkn->devid 	= ctx->dev->dev_id;
		rctx->tkn->priority 	= apm_sec_COS_DEF; 
		rctx->tkn->sa_len_dw 	= session->sa_len >> 2;
	} else {
		rctx->tkn->input_tkn_len = sec_tkn_update_out_macsec(
					(u32 *) in_tkn,
					req->cryptlen,
					*(u32 *) adata,
					adata + 4);
	}
	if (adata)
		kunmap_atomic(adata);
		
	return apm_sec_setup_crypto(&req->base);
}

static int apm_sec_decrypt_macsec(struct aead_request *req,
					unsigned int aes_mode)
{
	struct apm_sec_session_ctx     *ctx  = crypto_tfm_ctx(req->base.tfm);
	struct apm_sec_req_ctx *rctx = aead_request_ctx(req);
	struct sec_tkn_input_hdr	*in_tkn;
	unsigned char	new_tkn;

	/* Allocate token for this request */
	rctx->tkn = apm_sec_tkn_get(ctx, &new_tkn);
	if (rctx->tkn == NULL)
		return -ENOMEM;

	in_tkn = TKN_CTX_INPUT_TKN(rctx->tkn);
	if (new_tkn) {
		rctx->tkn->input_tkn_len = sec_tkn_set_in_macsec(
					(u32 *) in_tkn,
					req->cryptlen,
					apm_sec_v2hwaddr(rctx->sa->sa_ptr),
					rctx->sa->sa_ptr->mask,
					aes_mode);
		rctx->tkn->devid 	= ctx->dev->dev_id;
		rctx->tkn->priority 	= apm_sec_COS_DEF; 
		rctx->tkn->sa_len_dw 	= session->sa_len >> 2;
	} else {
		rctx->tkn->input_tkn_len = sec_tkn_update_in_macsec(
						(u32 *) in_tkn,
						req->cryptlen);
	}

	return apm_sec_setup_crypto(&req->base);
}

static int apm_sec_encrypt_macsec_gcm(struct aead_request *req)
{
	return apm_sec_encrypt_macsec(req, sec_tkn_AES_GCM);
}
static int apm_sec_decrypt_macsec_gcm(struct aead_request *req)
{
	return apm_sec_decrypt_macsec(req, sec_tkn_AES_GCM);
}

static int apm_sec_encrypt_macsec_gmac(struct aead_request *req)
{
	return apm_sec_encrypt_macsec(req, sec_tkn_AES_GMAC);
}
static int apm_sec_decrypt_macsec_gmac(struct aead_request *req)
{
	return apm_sec_decrypt_macsec(req, sec_tkn_AES_GMAC);
}

/*
 * For xcbc(aes), no support for init, update, and final hash functions
 *
 */
static int apm_sec_hash_noinit(struct ahash_request *req)
{       /* TBD */
        return -ENOSYS;
}

static int apm_sec_hash_noupdate(struct ahash_request *req)
{       /* TBD */
        return -ENOSYS;
}

static int apm_sec_hash_nofinal(struct ahash_request *req)
{       /* TBD */
        return -ENOSYS;
}
#endif
/*
 * Supported Crypto Algorithms
 *
 */
struct apm_sec_alg apm_sec_alg_tlb[] = {
	/* Basic Hash Alg */

	{.type = CRYPTO_ALG_TYPE_AHASH, .u.hash = {
         .halg.base      	= {
 	 .cra_name 		= "md5",
	 .cra_driver_name 	= "apm-md5",
	 .cra_flags 		= CRYPTO_ALG_TYPE_AHASH | CRYPTO_ALG_ASYNC,
	 .cra_blocksize 	= 64,	/* MD5 block size is 512-bits */
	 .cra_ctxsize 		= sizeof(struct apm_sec_session_ctx),
	 .cra_alignmask 	= 0,
	 .cra_priority 		= APM_SEC_CRYPTO_PRIORITY_IPSEC,
	 .cra_type 		= &crypto_ahash_type,
	 .cra_init 		= apm_sec_md5_alg_init },
	 .halg.digestsize 	= 16,	/* Digest is 128-bits */
	 .halg.statesize 	= sizeof(struct apm_sec_export),
	 .init 			= apm_sec_hash_init,
	 .update 		= apm_sec_hash_update,
	 .final 		= apm_sec_hash_final,
	 .digest 		= apm_sec_hash_digest,
	 }},

	{.type = CRYPTO_ALG_TYPE_AHASH, .u.hash = {
         .halg.base      	= {
	 .cra_name 		= "hmac(md5)",
	 .cra_driver_name 	= "apm-hmac-md5",
	 .cra_flags 		= CRYPTO_ALG_TYPE_AHASH | CRYPTO_ALG_ASYNC,
	 .cra_blocksize 	= 64,	/* MD5-HMAC block size is 512-bits */
	 .cra_ctxsize 		= sizeof(struct apm_sec_session_ctx),
	 .cra_alignmask 	= 0,
	 .cra_priority 		= APM_SEC_CRYPTO_PRIORITY_IPSEC,
	 .cra_type 		= &crypto_ahash_type },
	 .halg.digestsize 	= 16,	/* Digest is 128-bits */
	 .halg.statesize 	= sizeof(struct apm_sec_export),
	 .init 			= apm_sec_hash_init,
	 .update 		= apm_sec_hash_update,
	 .final 		= apm_sec_hash_final,
	 .digest 		= apm_sec_hash_digest,
	 .setkey 		= apm_sec_md5_hmac_setkey,
	 }},

	{.type = CRYPTO_ALG_TYPE_AHASH, .u.hash = {
         .halg.base      	= {
	 .cra_name 		= "sha1",
	 .cra_driver_name 	= "apm-sha1",
	 .cra_flags 		= CRYPTO_ALG_TYPE_AHASH | CRYPTO_ALG_ASYNC,
	 .cra_blocksize 	= 64,	/* SHA1-HMAC block size is 512-bits */
	 .cra_ctxsize 		= sizeof(struct apm_sec_session_ctx),
	 .cra_alignmask 	= 0,
	 .cra_priority 		= APM_SEC_CRYPTO_PRIORITY_IPSEC,
	 .cra_type   		= &crypto_ahash_type,
	 .cra_init 		= apm_sec_sha1_alg_init },
	 .halg.digestsize 	= 20,	/* Disgest is 160-bits */
	 .halg.statesize 	= sizeof(struct apm_sec_export),
	 .init   		= apm_sec_hash_init,
	 .update 		= apm_sec_hash_update,
	 .final  		= apm_sec_hash_final,
	 .digest 		= apm_sec_hash_digest,
	 }},

	{.type = CRYPTO_ALG_TYPE_AHASH, .u.hash = {
         .halg.base      	= {
	 .cra_name 		= "hmac(sha1)",
	 .cra_driver_name 	= "apm-hmac-sha1",
	 .cra_flags 		= CRYPTO_ALG_TYPE_AHASH | CRYPTO_ALG_ASYNC,
	 .cra_blocksize 	= 64,	/* SHA1-HMAC block size is 512-bits */
	 .cra_ctxsize 		= sizeof(struct apm_sec_session_ctx),
	 .cra_alignmask 	= 0,
	 .cra_priority 		= APM_SEC_CRYPTO_PRIORITY_IPSEC,
	 .cra_type   		= &crypto_ahash_type },
	 .halg.digestsize 	= 20,	/* Disgest is 160-bits */
	 .halg.statesize 	= sizeof(struct apm_sec_export),
	 .init   		= apm_sec_hash_init,
	 .update 		= apm_sec_hash_update,
	 .final  		= apm_sec_hash_final,
	 .digest 		= apm_sec_hash_digest,
	 .setkey 		= apm_sec_sha1_hmac_setkey,
	 }},
#if 0

	{.type = CRYPTO_ALG_TYPE_AHASH, .u.hash = {
         .halg.base      	= {
	 .cra_name 		= "sha224",
	 .cra_driver_name 	= "apm-sha224",
	 .cra_flags 		= CRYPTO_ALG_TYPE_AHASH | CRYPTO_ALG_ASYNC,
	 .cra_blocksize 	= 64,	/* SHA224-HMAC block size is 512-bits */
	 .cra_ctxsize 		= sizeof(struct apm_sec_session_ctx),
	 .cra_alignmask 	= 0,	/* Hardware requires 16 bytes aligned */
	 .cra_type 		= &crypto_ahash_type,
	 .cra_init 		= apm_sec_sha2_alg_init },	 
	 .halg.digestsize 	= 28,	/* Disgest is 224-bits */
	 .init   		= apm_sec_hash_init,
	 .update 		= apm_sec_hash_update,
	 .final  		= apm_sec_hash_final,
	 .digest 		= apm_sec_hash_digest,
	 }},

//	{.type = CRYPTO_ALG_TYPE_AHASH, .u.hash = {
//         .halg.base      	= {
//	 .cra_name 		= "hmac(sha224)",
//	 .cra_driver_name 	= "apm-hmac-sha224",
//	 .cra_flags 		= CRYPTO_ALG_TYPE_AHASH | CRYPTO_ALG_ASYNC,
//	 .cra_blocksize 	= 64,	/* SHA224-HMAC block size is 512-bits */
//	 .cra_ctxsize 		= sizeof(struct apm_sec_session_ctx),
//	 .cra_alignmask 	= 0,	/* Hardware requires 16 bytes aligned */
//	 .cra_type 		= &crypto_ahash_type },
//	 .halg.digestsize 	= 28,	/* Disgest is 224-bits */
//	 .init   		= apm_sec_hash_init,
//	 .update 		= apm_sec_hash_update,
//	 .final  		= apm_sec_hash_final,
//	 .digest 		= apm_sec_hash_digest,
//	 .setkey 		= apm_sec_sha2_hmac_setkey,
//	 }},

	{.type = CRYPTO_ALG_TYPE_AHASH, .u.hash = {
         .halg.base      	= {
	 .cra_name 		= "sha256",
	 .cra_driver_name 	= "apm-sha256",
	 .cra_flags 		= CRYPTO_ALG_TYPE_AHASH | CRYPTO_ALG_ASYNC,
	 .cra_blocksize 	= 64,	/* SHA256-HMAC block size is 512-bits */
	 .cra_ctxsize 		= sizeof(struct apm_sec_session_ctx),
	 .cra_alignmask 	= 0,	/* Hardware requires 16 bytes aligned */
	 .cra_type 		= &crypto_ahash_type,
	 .cra_init 		= apm_sec_sha2_alg_init },
	 .halg.digestsize 	= 32,	/* Disgest is 256-bits */
	 .init   		= apm_sec_hash_init,
	 .update 		= apm_sec_hash_update,
	 .final  		= apm_sec_hash_final,
	 .digest 		= apm_sec_hash_digest,
	 }},

//	{.type = CRYPTO_ALG_TYPE_AHASH, .u.hash = {
//         .halg.base      	= {
//	 .cra_name 		= "hmac(sha256)",
//	 .cra_driver_name 	= "apm-hmac-sha256",
//	 .cra_flags 		= CRYPTO_ALG_TYPE_AHASH | CRYPTO_ALG_ASYNC,
//	 .cra_blocksize 	= 64,	/* SHA256-HMAC block size is 512-bits */
//	 .cra_ctxsize 		= sizeof(struct apm_sec_session_ctx),
//	 .cra_alignmask 	= 0,	/* Hardware requires 16 bytes aligned */
//	 .cra_type 		= &crypto_ahash_type },
//	 .halg.digestsize 	= 32,	/* Disgest is 256-bits */
//	 .init   		= apm_sec_hash_init,
//	 .update 		= apm_sec_hash_update,
//	 .final  		= apm_sec_hash_final,
//	 .digest 		= apm_sec_hash_digest,
//	 .setkey 		= apm_sec_sha2_hmac_setkey,
//	 }},

	{.type = CRYPTO_ALG_TYPE_AHASH, .u.hash = {
         .halg.base      	= {
	 .cra_name 		= "sha384",
	 .cra_driver_name 	= "apm-sha384",
	 .cra_flags 		= CRYPTO_ALG_TYPE_AHASH | CRYPTO_ALG_ASYNC,
	 .cra_blocksize 	= 128,	/* SHA384-HMAC block size is 512-bits */
	 .cra_ctxsize 		= sizeof(struct apm_sec_session_ctx),
	 .cra_alignmask 	= 0,	/* Hardware requires 16 bytes aligned */
	 .cra_type 		= &crypto_ahash_type,
	 .cra_init 		= apm_sec_sha2_alg_init },
	 .halg.digestsize 	= 48,	/* Disgest is 384-bits */
	 .init   		= apm_sec_hash_init,
	 .update 		= apm_sec_hash_update,
	 .final  		= apm_sec_hash_final,
	 .digest 		= apm_sec_hash_digest,
	 }},
	 
//	{.type = CRYPTO_ALG_TYPE_AHASH, .u.hash = {
//         .halg.base      	= {
//	 .cra_name 		= "hmac(sha384)",
//	 .cra_driver_name 	= "apm-hmac-sha384",
//	 .cra_flags 		= CRYPTO_ALG_TYPE_AHASH | CRYPTO_ALG_ASYNC,
//	 .cra_blocksize 	= 128,	/* SHA384-HMAC block size is 512-bits */
//	 .cra_ctxsize 		= sizeof(struct apm_sec_session_ctx),
//	 .cra_alignmask 	= 0,	/* Hardware requires 16 bytes aligned */
//	 .cra_type 		= &crypto_ahash_type },
//	 .halg.digestsize 	= 48,	/* Disgest is 384-bits */
//	 .init   		= apm_sec_hash_init,
//	 .update 		= apm_sec_hash_update,
//	 .final  		= apm_sec_hash_final,
//	 .digest 		= apm_sec_hash_digest,
//	 .setkey 		= apm_sec_sha2_hmac_setkey,
//	 }},

	{.type = CRYPTO_ALG_TYPE_AHASH, .u.hash = {
         .halg.base      	= {
	 .cra_name 		= "sha512",
	 .cra_driver_name 	= "apm-sha512",
	 .cra_flags 		= CRYPTO_ALG_TYPE_AHASH | CRYPTO_ALG_ASYNC,
	 .cra_blocksize 	= 128,	/* SHA512-HMAC block size is 1024-bits*/
	 .cra_ctxsize 		= sizeof(struct apm_sec_session_ctx),
	 .cra_alignmask 	= 0,	/* Hardware requires 16 bytes aligned */
	 .cra_type 		= &crypto_ahash_type,
	 .cra_init 		= apm_sec_sha2_alg_init },
	 .halg.digestsize 	= 64,	/* Disgest is 512-bits */
	 .init   		= apm_sec_hash_init,
	 .update 		= apm_sec_hash_update,
	 .final  		= apm_sec_hash_final,
	 .digest 		= apm_sec_hash_digest,
	 }},

//	{.type = CRYPTO_ALG_TYPE_AHASH, .u.hash = {
//         .halg.base      	= {
//	 .cra_name 		= "hmac(sha512)",
//	 .cra_driver_name 	= "apm-hmac-sha512",
//	 .cra_flags 		= CRYPTO_ALG_TYPE_AHASH | CRYPTO_ALG_ASYNC,
//	 .cra_blocksize 	= 128,	/* SHA512-HMAC block size is 1024-bits*/
//	 .cra_ctxsize 		= sizeof(struct apm_sec_session_ctx),
//	 .cra_alignmask 	= 0,	/* Hardware requires 16 bytes aligned */
//	 .cra_type 		= &crypto_ahash_type },
//	 .halg.digestsize 	= 64,	/* Disgest is 512-bits */
//	 .init   		= apm_sec_hash_init,
//	 .update 		= apm_sec_hash_update,
//	 .final  		= apm_sec_hash_final,
//	 .digest 		= apm_sec_hash_digest,
//	 .setkey 		= apm_sec_sha2_hmac_setkey,
//	 }},

	/* Basic Crypto Alg */
	{.type = CRYPTO_ALG_TYPE_ABLKCIPHER, .u.cipher = {
         .cra_name 		= "ecb(des)",
	 .cra_driver_name 	= "apm-ecb-des",
	 .cra_flags 		= CRYPTO_ALG_TYPE_ABLKCIPHER | CRYPTO_ALG_ASYNC,
	 .cra_blocksize 	= 8,	/* 64-bit block */
	 .cra_ctxsize 		= sizeof(struct apm_sec_session_ctx),
	 .cra_alignmask 	= 0xF,	/* Hardware requires 16 bytes aligned */
	 .cra_type 		= &crypto_ablkcipher_type,
	 .cra_u 		= {.ablkcipher = {
	 .min_keysize 		= 8,	/* DES key is 64-bits */
	 .max_keysize 		= 8,	/* DES key is 64-bits */
	 .setkey 		= apm_sec_setkey_3des_ecb,
	 .encrypt 		= apm_sec_encrypt,
	 .decrypt 		= apm_sec_decrypt,
	 }}}},

	{.type = CRYPTO_ALG_TYPE_ABLKCIPHER, .u.cipher = {
         .cra_name 		= "cbc(des)",
	 .cra_driver_name 	= "apm-cbc-des",
	 .cra_flags 		= CRYPTO_ALG_TYPE_ABLKCIPHER | CRYPTO_ALG_ASYNC,
	 .cra_blocksize 	= 8,	/* 64-bits block */
	 .cra_ctxsize 		= sizeof(struct apm_sec_session_ctx),
	 .cra_alignmask 	= 0xF,	/* Hardware requires 16 bytes aligned */
	 .cra_type 		= &crypto_ablkcipher_type,
	 .cra_u 		= {.ablkcipher = {
	 .min_keysize 		= 8,	/* DES key is 64-bits */
	 .max_keysize 		= 8,	/* DES key is 64-bits */
	 .ivsize 		= 8,	/* IV size is 8 bytes */
	 .setkey 		= apm_sec_setkey_3des_cbc,
	 .encrypt 		= apm_sec_encrypt,
	 .decrypt 		= apm_sec_decrypt,
	 }}}},

	{.type = CRYPTO_ALG_TYPE_ABLKCIPHER, .u.cipher = {
         .cra_name 		= "cfb(des)",
	 .cra_driver_name 	= "apm-cfb-des",
	 .cra_flags 		= CRYPTO_ALG_TYPE_ABLKCIPHER | CRYPTO_ALG_ASYNC,
	 .cra_blocksize		= 8,	/* 64-bits block */
	 .cra_ctxsize 		= sizeof(struct apm_sec_session_ctx),
	 .cra_alignmask 	= 0xF,	/* Hardware requires 16 bytes aligned */
	 .cra_type 		= &crypto_ablkcipher_type,
	 .cra_u 		= {.ablkcipher = {
	 .min_keysize 		= 8,	/* DES key is 64-bits */
	 .max_keysize 		= 8,	/* DES key is 64-bits */
	 .setkey 		= apm_sec_setkey_3des_cfb,
	 .encrypt 		= apm_sec_encrypt,
	 .decrypt 		= apm_sec_decrypt,
	 }}}},
	{.type = CRYPTO_ALG_TYPE_ABLKCIPHER, .u.cipher = {
         .cra_name 		= "ofb(des)",
	 .cra_driver_name 	= "apm-ofb-des",
	 .cra_flags 		= CRYPTO_ALG_TYPE_ABLKCIPHER | CRYPTO_ALG_ASYNC,
	 .cra_blocksize 	= 8,	/* 64-bit block */
	 .cra_ctxsize 		= sizeof(struct apm_sec_session_ctx),
	 .cra_alignmask 	= 0xF,	/* Hardware requires 16 bytes aligned */
	 .cra_type 		= &crypto_ablkcipher_type,
	 .cra_u 		= {.ablkcipher = {
	 .min_keysize 		= 8,	/* DES key is 64-bits */
	 .max_keysize 		= 8,	/* DES key is 64-bits */
	 .setkey 		= apm_sec_setkey_3des_ofb,
	 .encrypt 		= apm_sec_encrypt,
	 .decrypt 		= apm_sec_decrypt,
	 }}}},
 
	{.type = CRYPTO_ALG_TYPE_ABLKCIPHER, .u.cipher = {
         .cra_name		= "cfb(des3_ede)",
	 .cra_driver_name	= "apm-cfb-des3",
	 .cra_flags 		= CRYPTO_ALG_TYPE_ABLKCIPHER | CRYPTO_ALG_ASYNC,
	 .cra_blocksize		= 8,	/* 64-bit block */
	 .cra_ctxsize 		= sizeof(struct apm_sec_session_ctx),
	 .cra_alignmask 	= 0xF,	/* Hardware requires 16 bytes aligned */
	 .cra_type 		= &crypto_ablkcipher_type,
	 .cra_u 		= {.ablkcipher = {
	 .min_keysize 		= 24,	/* 3DES key is 192-bits */
	 .max_keysize 		= 24,	/* 3DES key is 192-bits */
	 .setkey 		= apm_sec_setkey_3des_cfb,
	 .encrypt 		= apm_sec_encrypt,
	 .decrypt 		= apm_sec_decrypt,
	 }}}},
	{.type = CRYPTO_ALG_TYPE_ABLKCIPHER, .u.cipher = {
         .cra_name 		= "ofb(des3_ede)",
	 .cra_driver_name 	= "apm-ofb-des3",
	 .cra_flags 		= CRYPTO_ALG_TYPE_ABLKCIPHER | CRYPTO_ALG_ASYNC,
	 .cra_blocksize		= 8,	/* 64-bit block */
	 .cra_ctxsize 		= sizeof(struct apm_sec_session_ctx),
	 .cra_alignmask 	= 0xF,	/* Hardware requires 16 bytes aligned */
	 .cra_type 		= &crypto_ablkcipher_type,
	 .cra_u 		= {.ablkcipher = {
	 .min_keysize 		= 24,	/* 3DES key is 192-bits */
	 .max_keysize 		= 24,	/* 3DES key is 192-bits */
	 .setkey 		= apm_sec_setkey_3des_ofb,
	 .encrypt 		= apm_sec_encrypt,
	 .decrypt 		= apm_sec_decrypt,
	 }}}},

	{.type = CRYPTO_ALG_TYPE_ABLKCIPHER, .u.cipher = {
         .cra_name 		= "ecb(des3_ede)",
	 .cra_driver_name 	= "apm-ecb-des3",
	 .cra_flags 		= CRYPTO_ALG_TYPE_ABLKCIPHER | CRYPTO_ALG_ASYNC,
	 .cra_blocksize 	= 8,	/* 64-bit block */
	 .cra_ctxsize 		= sizeof(struct apm_sec_session_ctx),
	 .cra_alignmask 	= 0xF,	/* Hardware requires 16 bytes aligned */
	 .cra_type 		= &crypto_ablkcipher_type,
	 .cra_u 		= {.ablkcipher = {
	 .min_keysize 		= 24,	/* 3DES key is 192-bits */
	 .max_keysize 		= 24,	/* 3DES key is 192-bits */
	 .setkey 		= apm_sec_setkey_3des_ecb,
	 .encrypt 		= apm_sec_encrypt,
	 .decrypt 		= apm_sec_decrypt,
	 }}}},
#endif

	{.type = CRYPTO_ALG_TYPE_ABLKCIPHER, .u.cipher = {
         .cra_name 		= "cbc(des3_ede)",
	 .cra_driver_name 	= "apm-cbc-des3",
	 .cra_flags		= CRYPTO_ALG_TYPE_ABLKCIPHER | CRYPTO_ALG_ASYNC,
	 .cra_blocksize 	= 8,	/* 64-bit block */
	 .cra_ctxsize 		= sizeof(struct apm_sec_session_ctx),
	 .cra_alignmask 	= 0xF,	/* Hardware requires 16 bytes aligned */
	 .cra_priority 		= APM_SEC_CRYPTO_PRIORITY_IPSEC,
	 .cra_type 		= &crypto_ablkcipher_type,
	 .cra_u 		= {.ablkcipher = {
	 .min_keysize 		= 24,	/* 3DES key is 192-bits */
	 .max_keysize 		= 24,	/* 3DES key is 192-bits */
	 .ivsize 		= 8,		/* IV size is 8 bytes */
	 .setkey 		= apm_sec_setkey_3des_cbc,
	 .encrypt 		= apm_sec_encrypt,
	 .decrypt 		= apm_sec_decrypt,
	 }}}},

#if 0
	/* Crypto AES ECB, CBC, CTR, GCM, CCM, and GMAC modes */
	{.type = CRYPTO_ALG_TYPE_ABLKCIPHER, .u.cipher = {
         .cra_name 		= "ecb(aes)",
	 .cra_driver_name 	= "apm-ecb-aes",
	 .cra_flags 		= CRYPTO_ALG_TYPE_ABLKCIPHER | CRYPTO_ALG_ASYNC,
	 .cra_blocksize 	= 16,	/* 128-bits block */
	 .cra_ctxsize 		= sizeof(struct apm_sec_session_ctx),
	 .cra_alignmask 	= 0xF,	/* Hardware requires 16 bytes aligned */
	 .cra_type 		= &crypto_ablkcipher_type,
	 .cra_u 		= {.ablkcipher = {
	 .min_keysize 		= 16,	/* AES min key size is 128-bits */
	 .max_keysize 		= 32,	/* AES max key size is 256-bits */
	 .setkey 		= apm_sec_setkey_aes_ecb,
	 .encrypt 		= apm_sec_encrypt,
	 .decrypt 		= apm_sec_decrypt,
	 }}}},

	{.type = CRYPTO_ALG_TYPE_ABLKCIPHER, .u.cipher = {
         .cra_name 		= "apmcbc(aes)",
	 .cra_driver_name 	= "apm-cbc-aes",
	 .cra_flags 		= CRYPTO_ALG_TYPE_ABLKCIPHER | CRYPTO_ALG_ASYNC,
	 .cra_blocksize 	= 16,	/* 128-bits block */
	 .cra_ctxsize 		= sizeof(struct apm_sec_session_ctx),
	 .cra_alignmask 	= 0xF,	/* Hardware requires 16 bytes aligned */
	 .cra_type 		= &crypto_ablkcipher_type,
	 .cra_u 		= {.ablkcipher = {
	 .min_keysize 		= 16,	/* AES min key size is 128-bits */
	 .max_keysize 		= 32,	/* AES max key size is 256-bits */
	 .ivsize 		= 16,	/* IV size is 16 bytes */
	 .setkey 		= apm_sec_setkey_aes_cbc,
	 .encrypt 		= apm_sec_encrypt,
	 .decrypt 		= apm_sec_decrypt,
	 }}}},

	{.type = CRYPTO_ALG_TYPE_AHASH, .u.hash = {
         .halg.base      	= {
	 .cra_name 		= "xcbc(aes)",
	 .cra_driver_name	= "apm-xcbc-aes",
	 .cra_flags 		= CRYPTO_ALG_TYPE_AHASH | CRYPTO_ALG_ASYNC,
	 .cra_blocksize 	= 16,	/* XCBC block size is 128-bits */
	 .cra_ctxsize 		= sizeof(struct apm_sec_session_ctx),
	 .cra_alignmask 	= 0,	/* Hardware requires 16 bytes aligned */
	 .cra_type 		= &crypto_ahash_type },
	 .halg.digestsize 	= 16,	/* MAC (message auth code) is 128-bits*/
	 .init   		= apm_sec_hash_noinit,
	 .update 		= apm_sec_hash_noupdate,
	 .final  		= apm_sec_hash_nofinal,
	 .digest 		= apm_sec_hash_digest,
	 .setkey 		= apm_sec_sha2_xcbc_setkey,
	 }},
#endif

	{.type = CRYPTO_ALG_TYPE_ABLKCIPHER, .u.cipher = {
         .cra_name 		= "rfc3686(ctr(aes))",
	 .cra_driver_name 	= "apm-ctr-aes",
	 .cra_flags 		= CRYPTO_ALG_TYPE_ABLKCIPHER | CRYPTO_ALG_ASYNC,
	 .cra_blocksize 	= 16,	/* 128-bits block */
	 .cra_ctxsize 		= sizeof(struct apm_sec_session_ctx),
	 .cra_alignmask 	= 0xF,	/* Hardware requires 16 bytes aligned */
	 .cra_priority 		= APM_SEC_CRYPTO_PRIORITY_IPSEC,
	 .cra_type		= &crypto_ablkcipher_type,
	 .cra_u 		= {.ablkcipher = {
	 .min_keysize 		= 16,	/* AES min key size is 128-bits */
	 .max_keysize 		= 32,	/* AES max key size is 256-bits */
	 .ivsize 		= 16,		/* IV size is 16 bytes */
	 .setkey 		= apm_sec_setkey_aes_ctr,
	 .encrypt 		= apm_sec_encrypt,
	 .decrypt 		= apm_sec_decrypt,
	 }}}},

#if 0
	{.type = CRYPTO_ALG_TYPE_AEAD, .u.cipher = {
         .cra_name 		= "ccm(aes)",
	 .cra_driver_name 	= "apm-ccm-aes",
	 .cra_flags 		= CRYPTO_ALG_TYPE_AEAD | CRYPTO_ALG_ASYNC,
	 .cra_blocksize 	= 16,	/* 128-bits block */
	 .cra_ctxsize 		= sizeof(struct apm_sec_session_ctx),
	 .cra_alignmask 	= 0xF,	/* Hardware requires 16 bytes aligned */
	 .cra_type 		= &crypto_aead_type,
	 .cra_u 		= {.aead = {
	 .ivsize 		= 16,	/* IV size is 16 bytes */
	 .maxauthsize		= 16,	/* Max auth data size in bytes */
	 .setkey 		= apm_sec_setkey_aes_ccm,
	 .setauthsize		= apm_sec_setauthsize_aes_ccm,
	 .encrypt 		= apm_sec_encrypt_aes_ccm,
	 .decrypt 		= apm_sec_decrypt_aes_ccm,
	 .givencrypt		= apm_sec_givencrypt_aes_ccm,
	 .givdecrypt		= apm_sec_givdecrypt_aes_ccm,
	 }}}},

	/* Crypto ARC4 - stateless */
	{.type = CRYPTO_ALG_TYPE_ABLKCIPHER, .u.cipher = {
         .cra_name 		= "ecb(arc4)",
	 .cra_driver_name 	= "apm-arc4",
	 .cra_flags 		= CRYPTO_ALG_TYPE_ABLKCIPHER | CRYPTO_ALG_ASYNC,
	 .cra_blocksize 	= 1,	/* 8-bits block */
	 .cra_ctxsize 		= sizeof(struct apm_sec_session_ctx),
	 .cra_alignmask 	= 0xF,	/* Hardware requires 16 bytes aligned */
	 .cra_type 		= &crypto_ablkcipher_type,
	 .cra_u 		= {.ablkcipher = {
	 .min_keysize 		= 5,	/* ARC4 min key size is 40-bits */
	 .max_keysize 		= 16,	/* ARC4 max key size is 128-bits */
	 .setkey 		= apm_sec_setkey_arc4_stateless,
	 .encrypt 		= apm_sec_arc4_encrypt,
	 .decrypt 		= apm_sec_arc4_decrypt,
	 }}}},

	{.type = CRYPTO_ALG_TYPE_ABLKCIPHER, .u.cipher = {
         .cra_name 		= "f8(kasumi)",
	 .cra_driver_name 	= "apm-f8-kasumi",
	 .cra_flags 		= CRYPTO_ALG_TYPE_ABLKCIPHER | CRYPTO_ALG_ASYNC,
	 .cra_blocksize 	= 8,	/* 64-bits block */
	 .cra_ctxsize 		= sizeof(struct apm_sec_session_ctx),
	 .cra_alignmask 	= 0xF,	/* Hardware requires 16 byte aligned */
	 .cra_type 		= &crypto_ablkcipher_type,
	 .cra_u 		= {.ablkcipher = {
	 .min_keysize 		= 16,	/* Kasumi  min key size is 128-bits */
	 .max_keysize 		= 16,	/* Kasumi  max key size is 128-bits */
	 .ivsize 		= 8,	/* IV size is 8 bytes */
	 .setkey 		= apm_sec_setkey_kasumi_f8,
	 .encrypt 		= apm_sec_encrypt,
	 .decrypt 		= apm_sec_decrypt,
	 }}}},

	 {.type = CRYPTO_ALG_TYPE_ABLKCIPHER, .u.cipher = {
	 .cra_name 		= "kasumi",
	 .cra_driver_name 	= "apm-kasumi",
	 .cra_flags 		= CRYPTO_ALG_TYPE_ABLKCIPHER | CRYPTO_ALG_ASYNC,
	 .cra_blocksize 	= 8,	/* 64-bits block */
	 .cra_ctxsize 		= sizeof(struct apm_sec_session_ctx),
	 .cra_alignmask 	= 0xF,	/* Hardware requires 16 byte aligned */
	 .cra_type 		= &crypto_ablkcipher_type,
	 .cra_u 		= {.ablkcipher = {
	 .min_keysize 		= 16,	/* Kasumi  min key size is 128-bits */
	 .max_keysize 		= 16,	/* Kasumi  max key size is 128-bits */
	.setkey 		= apm_sec_setkey_kasumi_p,
	.encrypt 		= apm_sec_encrypt,
	.decrypt 		= apm_sec_decrypt,
	}}}},
	
	 {.type = CRYPTO_ALG_TYPE_AHASH, .u.hash = {
		 .halg.base = {
         .cra_name 		= "f9(kasumi)",
	 .cra_driver_name 	= "apm-f9-kasumi",
	 .cra_flags 		= CRYPTO_ALG_TYPE_AHASH | CRYPTO_ALG_ASYNC,
	 .cra_blocksize 	= 16,	/* XCBC block size is 128-bits */
	 .cra_ctxsize 		= sizeof(struct apm_sec_session_ctx),
	 .cra_alignmask		= 0,	/* Hardware requires 16 bytes aligned */
	 .cra_type 		= &crypto_ahash_type },
	 .halg.digestsize 		= 4,	/* MAC (message auth code) is 256-bits*/
	 .init   		= apm_sec_hash_init,
	 .update 		= apm_sec_hash_update,
	 .final  		= apm_sec_hash_final,
	 .digest 		= apm_sec_hash_digest,
	 .setkey 		= apm_sec_kasumi_f9_setkey,
	 }},
 
	/* Crypto ARC4 - statefull */
	{.type = CRYPTO_ALG_TYPE_ABLKCIPHER, .u.cipher = {
         .cra_name 		= "cbc(arc4)",
	 .cra_driver_name 	= "apm-arc4",
	 .cra_flags 		= CRYPTO_ALG_TYPE_BLKCIPHER | CRYPTO_ALG_ASYNC,
	 .cra_blocksize		= 1,	/* 8-bits block */
	 .cra_ctxsize 		= sizeof(struct apm_sec_session_ctx),
	 .cra_alignmask 	= 0xF,	/* Hardware requires 16 bytes aligned */
	 .cra_type		= &crypto_ablkcipher_type,
	 .cra_u 		= {.ablkcipher = {
	 .min_keysize 		= 5,	/* ARC4 min key size is 40-bits */
	 .max_keysize 		= 16,	/* ARC4 max key size is 128-bits */
	 .setkey 		= apm_sec_setkey_arc4_statefull,
	 .encrypt 		= apm_sec_arc4_encrypt,
	 .decrypt 		= apm_sec_arc4_decrypt,
	 }}}},

	 /* GCM Alg Decryption icv Failing  FIXME*/
	{ .type = CRYPTO_ALG_TYPE_AEAD, .u.cipher = {
	.cra_name 		= "gcm(aes)",
	.cra_driver_name 	= "apm-gcm-aes",
	.cra_flags 		= CRYPTO_ALG_TYPE_AEAD | CRYPTO_ALG_ASYNC,
	.cra_blocksize 		= 16,	/* 128-bits block */
	.cra_ctxsize 		= sizeof(struct apm_sec_session_ctx),
	.cra_alignmask 		= 0xF,	/* Hardware requires 16 bytes aligned */
	.cra_type 		= &crypto_aead_type,
	.cra_u 		= { .aead = {
	.ivsize 		= 16, /* IV size is 16 bytes */
	.maxauthsize		= 16, /* Max auth data size in bytes */
	.setkey 		= apm_sec_setkey_aes_gcm,
	.setauthsize		= apm_sec_setauthsize_aes,
	.encrypt 		= apm_sec_encrypt_aes_gcm,
	.decrypt 		= apm_sec_decrypt_aes_gcm,
	.givencrypt		= apm_sec_givencrypt_aes_gcm,
	.givdecrypt		= apm_sec_givdecrypt_aes_gcm,
			}
		}
	}},
#endif

#if 0
	 /* IPSec ESP Transport Mode Algorithms */
	 { .type = CRYPTO_ALG_TYPE_AEAD, .u.cipher = {
	 .cra_name 		= "transport(esp(authenc(hmac(md5),cbc(aes))))",
	 .cra_driver_name 	= "apm-transport-esp-hmac-md5-cbc-aes",
	 .cra_priority 		= APM_SEC_CRYPTO_PRIORITY_IPSEC,
	 .cra_flags 		= CRYPTO_ALG_TYPE_AEAD | CRYPTO_ALG_ASYNC,
	 .cra_blocksize 	= 16,	/* 128-bits block */
	 .cra_ctxsize 		= sizeof(struct apm_sec_session_ctx),
	 .cra_alignmask 	= 0xF,	/* Hardware requires 16 bytes aligned */
	 .cra_type 		= &crypto_aead_type,
	 .cra_module 		= THIS_MODULE,
	 .cra_u 		= {.aead = {
	 .ivsize 		= 16,	/* IV size for crypto */
	 .maxauthsize		= 16,	/* Max auth data size in bytes */
	 .setkey 		= apm_sec_setkey_esp,
	 .setauthsize		= apm_sec_setauthsize_aes,
	 .encrypt 		= apm_sec_encrypt_esp_transport,
	 .decrypt 		= apm_sec_decrypt_esp_transport,
	 }}}},

	 { .type = CRYPTO_ALG_TYPE_AEAD, .u.cipher = {
	 .cra_name 		= "transport(esp(authenc(hmac(sha1),cbc(aes))))",
	 .cra_driver_name 	= "apm-transport-esp-hmac-sha1-cbc-aes",
	 .cra_priority 		= APM_SEC_CRYPTO_PRIORITY_IPSEC,
	 .cra_flags 		= CRYPTO_ALG_TYPE_AEAD | CRYPTO_ALG_ASYNC,
	 .cra_blocksize 	= 16,	/* 128-bits block */
	 .cra_ctxsize 		= sizeof(struct apm_sec_session_ctx),
	 .cra_alignmask 	= 0xF,	/* Hardware requires 16 bytes aligned */
	 .cra_type 		= &crypto_aead_type,
	 .cra_module 		= THIS_MODULE,
	 .cra_u 		= {.aead = {
	 .ivsize 		= 16,	/* IV size for crypto */
	 .maxauthsize		= 20,	/* Max auth data size in bytes */
	 .setkey 		= apm_sec_setkey_esp,
	 .setauthsize		= apm_sec_setauthsize_aes,
	 .encrypt 		= apm_sec_encrypt_esp_transport,
	 .decrypt 		= apm_sec_decrypt_esp_transport,
	 }}}},
	
	 { .type = CRYPTO_ALG_TYPE_AEAD, .u.cipher = {
	 .cra_name 		= "transport(esp(authenc(hmac(sha256),cbc(aes))))",
	 .cra_driver_name 	= "apm-transport-esp-hmac-sha256-cbc-aes",
	 .cra_priority 		= APM_SEC_CRYPTO_PRIORITY_IPSEC,
	 .cra_flags 		= CRYPTO_ALG_TYPE_AEAD | CRYPTO_ALG_ASYNC,
	 .cra_blocksize 	= 16,	/* 128-bits block */
	 .cra_ctxsize 		= sizeof(struct apm_sec_session_ctx),
	 .cra_alignmask 	= 0xF,	/* Hardware requires 16 bytes aligned */
	 .cra_type 		= &crypto_aead_type,
	 .cra_module 		= THIS_MODULE,
	 .cra_u 		= {.aead = {
	 .ivsize 		= 16,	/* IV size for crypto */
	 .maxauthsize		= 32,	/* Max auth data size in bytes */
	 .setkey 		= apm_sec_setkey_esp,
	 .setauthsize		= apm_sec_setauthsize_aes,
	 .encrypt 		= apm_sec_encrypt_esp_transport,
	 .decrypt 		= apm_sec_decrypt_esp_transport,
	 }}}},

	 { .type = CRYPTO_ALG_TYPE_AEAD, .u.cipher = {
	 .cra_name 		= "transport(esp(authenc(hmac(sha224),cbc(aes))))",
	 .cra_driver_name 	= "apm-transport-esp-hmac-sha224-cbc-aes",
	 .cra_priority 		= APM_SEC_CRYPTO_PRIORITY_IPSEC,
	 .cra_flags 		= CRYPTO_ALG_TYPE_AEAD | CRYPTO_ALG_ASYNC,
	 .cra_blocksize 	= 16,	/* 128-bits block */
	 .cra_ctxsize 		= sizeof(struct apm_sec_session_ctx),
	 .cra_alignmask 	= 0xF,	/* Hardware requires 16 bytes aligned */
	 .cra_type 		= &crypto_aead_type,
	 .cra_module 		= THIS_MODULE,
	 .cra_u 		= {.aead = {
	 .ivsize 		= 16,	/* IV size for crypto */
	 .maxauthsize		= 28,	/* Max auth data size in bytes */
	 .setkey 		= apm_sec_setkey_esp,
	 .setauthsize		= apm_sec_setauthsize_aes,
	 .encrypt 		= apm_sec_encrypt_esp_transport,
	 .decrypt 		= apm_sec_decrypt_esp_transport,
	 }}}},

	 { .type = CRYPTO_ALG_TYPE_AEAD, .u.cipher = {
	 .cra_name 		= "transport(esp(authenc(hmac(sha512),cbc(aes))))",
	 .cra_driver_name 	= "apm-transport-esp-hmac-sha512-cbc-aes",
	 .cra_priority 		= APM_SEC_CRYPTO_PRIORITY_IPSEC,
	 .cra_flags 		= CRYPTO_ALG_TYPE_AEAD | CRYPTO_ALG_ASYNC,
	 .cra_blocksize 	= 16,	/* 128-bits block */
	 .cra_ctxsize 		= sizeof(struct apm_sec_session_ctx),
	 .cra_alignmask 	= 0xF,	/* Hardware requires 16 bytes aligned */
	 .cra_type 		= &crypto_aead_type,
	 .cra_module 		= THIS_MODULE,
	 .cra_u 		= {.aead = {
	 .ivsize 		= 16,	/* IV size for crypto */
	 .maxauthsize		= 32,	/* Max auth data size in bytes */
	 .setkey 		= apm_sec_setkey_esp,
	 .setauthsize		= apm_sec_setauthsize_aes,
	 .encrypt 		= apm_sec_encrypt_esp_transport,
	 .decrypt 		= apm_sec_decrypt_esp_transport,
	 }}}},

	 { .type = CRYPTO_ALG_TYPE_AEAD, .u.cipher = {
	 .cra_name 		= "transport(esp(authenc(hmac(md5),cbc(des))))",
	 .cra_driver_name 	= "apm-transport-esp-hmac-md5-cbc-des",
	 .cra_priority 		= APM_SEC_CRYPTO_PRIORITY_IPSEC,
	 .cra_flags 		= CRYPTO_ALG_TYPE_AEAD | CRYPTO_ALG_ASYNC,
	 .cra_blocksize 	= 8,	/* 128-bits block */
	 .cra_ctxsize 		= sizeof(struct apm_sec_session_ctx),
	 .cra_alignmask 	= 0xF,	/* Hardware requires 16 bytes aligned */
	 .cra_type 		= &crypto_aead_type,
	 .cra_module 		= THIS_MODULE,
	 .cra_u 		= {.aead = {
	 .ivsize 		= 8,	/* IV size for crypto */
	 .maxauthsize		= 16,	/* Max auth data size in bytes */
	 .setkey 		= apm_sec_setkey_esp_des,
	 .setauthsize		= apm_sec_setauthsize_aes,
	 .encrypt 		= apm_sec_encrypt_esp_transport,
	 .decrypt 		= apm_sec_decrypt_esp_transport,
	 }}}},

	 { .type = CRYPTO_ALG_TYPE_AEAD, .u.cipher = {
	 .cra_name 		= "transport(esp(authenc(hmac(sha1),cbc(des))))",
	 .cra_driver_name 	= "apm-transport-esp-hmac-sha1-cbc-des",
	 .cra_priority 		= APM_SEC_CRYPTO_PRIORITY_IPSEC,
	 .cra_flags 		= CRYPTO_ALG_TYPE_AEAD | CRYPTO_ALG_ASYNC,
	 .cra_blocksize 	= 8,	/* 128-bits block */
	 .cra_ctxsize 		= sizeof(struct apm_sec_session_ctx),
	 .cra_alignmask 	= 0xF,	/* Hardware requires 16 bytes aligned */
	 .cra_type 		= &crypto_aead_type,
	 .cra_module 		= THIS_MODULE,
	 .cra_u 		= {.aead = {
	 .ivsize 		= 8,	/* IV size for crypto */
	 .maxauthsize		= 20,	/* Max auth data size in bytes */
	 .setkey 		= apm_sec_setkey_esp_des,
	 .setauthsize		= apm_sec_setauthsize_aes,
	 .encrypt 		= apm_sec_encrypt_esp_transport,
	 .decrypt 		= apm_sec_decrypt_esp_transport,
	 }}}},

	 { .type = CRYPTO_ALG_TYPE_AEAD, .u.cipher = {
	 .cra_name 		= "transport(esp(authenc(hmac(sha224),cbc(des))))",
	 .cra_driver_name 	= "apm-transport-esp-hmac-sha224-cbc-des",
	 .cra_priority 		= APM_SEC_CRYPTO_PRIORITY_IPSEC,
	 .cra_flags 		= CRYPTO_ALG_TYPE_AEAD | CRYPTO_ALG_ASYNC,
	 .cra_blocksize 	= 8,	/* 64-bits block */
	 .cra_ctxsize 		= sizeof(struct apm_sec_session_ctx),
	 .cra_alignmask 	= 0xF,	/* Hardware requires 16 bytes aligned */
	 .cra_type 		= &crypto_aead_type,
	 .cra_module 		= THIS_MODULE,
	 .cra_u 		= {.aead = {
	 .ivsize 		= 8,	/* IV size for crypto */
	 .maxauthsize		= 28,	/* Max auth data size in bytes */
	 .setkey 		= apm_sec_setkey_esp_des,
	 .setauthsize		= apm_sec_setauthsize_aes,
	 .encrypt 		= apm_sec_encrypt_esp_transport,
	 .decrypt 		= apm_sec_decrypt_esp_transport,
	 }}}},

	 { .type = CRYPTO_ALG_TYPE_AEAD, .u.cipher = {
	 .cra_name 		= "transport(esp(authenc(hmac(sha256),cbc(des))))",
	 .cra_driver_name 	= "apm-transport-esp-hmac-sha256-cbc-des",
	 .cra_priority 		= APM_SEC_CRYPTO_PRIORITY_IPSEC,
	 .cra_flags 		= CRYPTO_ALG_TYPE_AEAD | CRYPTO_ALG_ASYNC,
	 .cra_blocksize 	= 8,	/* 128-bits block */
	 .cra_ctxsize 		= sizeof(struct apm_sec_session_ctx),
	 .cra_alignmask 	= 0xF,	/* Hardware requires 16 bytes aligned */
	 .cra_type 		= &crypto_aead_type,
	 .cra_module 		= THIS_MODULE,
	 .cra_u 		= {.aead = {
	 .ivsize 		= 8,	/* IV size for crypto */
	 .maxauthsize		= 32,	/* Max auth data size in bytes */
	 .setkey 		= apm_sec_setkey_esp_des,
	 .setauthsize		= apm_sec_setauthsize_aes,
	 .encrypt 		= apm_sec_encrypt_esp_transport,
	 .decrypt 		= apm_sec_decrypt_esp_transport,
	 }}}},

	 { .type = CRYPTO_ALG_TYPE_AEAD, .u.cipher = {
	 .cra_name 		= "transport(esp(authenc(hmac(md5),cbc(des3_ede))))",
	 .cra_driver_name 	= "apm-transport-esp-hmac-md5-cbc-des3",
	 .cra_priority 		= APM_SEC_CRYPTO_PRIORITY_IPSEC,
	 .cra_flags 		= CRYPTO_ALG_TYPE_AEAD | CRYPTO_ALG_ASYNC,
	 .cra_blocksize 	= 8,	/* 128-bits block */
	 .cra_ctxsize 		= sizeof(struct apm_sec_session_ctx),
	 .cra_alignmask 	= 0xF,	/* Hardware requires 16 bytes aligned */
	 .cra_type 		= &crypto_aead_type,
	 .cra_module 		= THIS_MODULE,
	 .cra_u 		= {.aead = {
	 .ivsize 		= 8,	/* IV size for crypto */
	 .maxauthsize		= 16,	/* Max auth data size in bytes */
	 .setkey 		= apm_sec_setkey_esp_des3,
	 .setauthsize		= apm_sec_setauthsize_aes,
	 .encrypt 		= apm_sec_encrypt_esp_transport,
	 .decrypt 		= apm_sec_decrypt_esp_transport,
	 }}}},

	 { .type = CRYPTO_ALG_TYPE_AEAD, .u.cipher = {
	 .cra_name 		= "transport(esp(authenc(hmac(sha1),cbc(des3_ede))))",
	 .cra_driver_name 	= "apm-transport-esp-hmac-sha1-cbc-des3",
	 .cra_priority 		= APM_SEC_CRYPTO_PRIORITY_IPSEC,
	 .cra_flags 		= CRYPTO_ALG_TYPE_AEAD | CRYPTO_ALG_ASYNC,
	 .cra_blocksize 	= 8,	/* 128-bits block */
	 .cra_ctxsize 		= sizeof(struct apm_sec_session_ctx),
	 .cra_alignmask 	= 0xF,	/* Hardware requires 16 bytes aligned */
	 .cra_type 		= &crypto_aead_type,
	 .cra_module 		= THIS_MODULE,
	 .cra_u 		= {.aead = {
	 .ivsize 		= 8,	/* IV size for crypto */
	 .maxauthsize		= 20,	/* Max auth data size in bytes */
	 .setkey 		= apm_sec_setkey_esp_des3,
	 .setauthsize		= apm_sec_setauthsize_aes,
	 .encrypt 		= apm_sec_encrypt_esp_transport,
	 .decrypt 		= apm_sec_decrypt_esp_transport,
	 }}}},

	 { .type = CRYPTO_ALG_TYPE_AEAD, .u.cipher = {
	 .cra_name 		= "transport(esp(authenc(hmac(sha256),cbc(des3_ede))))",
	 .cra_driver_name 	= "apm-transport-esp-hmac-sha256-cbc-des3",
	 .cra_priority 		= APM_SEC_CRYPTO_PRIORITY_IPSEC,
	 .cra_flags 		= CRYPTO_ALG_TYPE_AEAD | CRYPTO_ALG_ASYNC,
	 .cra_blocksize 	= 8,	/* 128-bits block */
	 .cra_ctxsize 		= sizeof(struct apm_sec_session_ctx),
	 .cra_alignmask 	= 0xF,	/* Hardware requires 16 bytes aligned */
	 .cra_type 		= &crypto_aead_type,
	 .cra_module 		= THIS_MODULE,
	 .cra_u 		= {.aead = {
	 .ivsize 		= 8,	/* IV size for crypto */
	 .maxauthsize		= 32,	/* Max auth data size in bytes */
	 .setkey 		= apm_sec_setkey_esp_des3,
	 .setauthsize		= apm_sec_setauthsize_aes,
	 .encrypt 		= apm_sec_encrypt_esp_transport,
	 .decrypt 		= apm_sec_decrypt_esp_transport,
	 }}}},
	
	 { .type = CRYPTO_ALG_TYPE_AEAD, .u.cipher = {
	 .cra_name 		= "transport(esp(authenc(hmac(md5),rfc3686(ctr(aes)))))",
	 .cra_driver_name 	= "apm-transport-esp-hmac-md5-ctr-aes",
	 .cra_priority 		= APM_SEC_CRYPTO_PRIORITY_IPSEC,
	 .cra_flags 		= CRYPTO_ALG_TYPE_AEAD | CRYPTO_ALG_ASYNC,
	 .cra_blocksize 	= 16,	/* 128-bits block */
	 .cra_ctxsize 		= sizeof(struct apm_sec_session_ctx),
	 .cra_alignmask 	= 0xF,	
	 .cra_type 		= &crypto_aead_type,
	 .cra_u 		= {.aead = {
	 .ivsize 		= 8,	/* IV size is 16 bytes */
	 .maxauthsize		= 16,	/* Max auth data size in bytes */
	 .setkey 		= apm_sec_setkey_esp_aes_ctr,
	 .setauthsize		= apm_sec_setauthsize_aes,
	 .encrypt 		= apm_sec_encrypt_esp_transport,
	 .decrypt 		= apm_sec_decrypt_esp_transport,
	 }}}},

	 { .type = CRYPTO_ALG_TYPE_AEAD, .u.cipher = {
	 .cra_name 		= "transport(esp(authenc(hmac(sha1),rfc3686(ctr(aes)))))",
	 .cra_driver_name 	= "apm-transport-esp-hmac-sha1-ctr-aes",
	 .cra_priority 		= APM_SEC_CRYPTO_PRIORITY_IPSEC,
	 .cra_flags 		= CRYPTO_ALG_TYPE_AEAD | CRYPTO_ALG_ASYNC,
	 .cra_blocksize 	= 16,	/* 128-bits block */
	 .cra_ctxsize 		= sizeof(struct apm_sec_session_ctx),
	 .cra_alignmask 	= 0xF,	
	 .cra_type 		= &crypto_aead_type,
	 .cra_u 		= {.aead = {
	 .ivsize 		= 8,	/* IV size is 16 bytes */
	 .maxauthsize		= 20,	/* Max auth data size in bytes */
	 .setkey 		= apm_sec_setkey_esp_aes_ctr,
	 .setauthsize		= apm_sec_setauthsize_aes,
	 .encrypt 		= apm_sec_encrypt_esp_transport,
	 .decrypt 		= apm_sec_decrypt_esp_transport,
	 }}}},

	 { .type = CRYPTO_ALG_TYPE_AEAD, .u.cipher = {
	 .cra_name 		= "transport(esp(rfc4106(gcm(aes))))",
	 .cra_driver_name 	= "apm-transport-esp-rfc4106-gcm-aes",
	 .cra_priority 		= APM_SEC_CRYPTO_PRIORITY_IPSEC,
	 .cra_flags 		= CRYPTO_ALG_TYPE_AEAD | CRYPTO_ALG_ASYNC,
	 .cra_blocksize 	= 16,	/* 128-bits block */
	 .cra_ctxsize 		= sizeof(struct apm_sec_session_ctx),
	 .cra_alignmask 	= 0xF,	/* Hardware requires 16 bytes aligned */
	 .cra_type 		= &crypto_aead_type,
	 .cra_module 		= THIS_MODULE,
	 .cra_u 		= {.aead = {
	 .ivsize 		= 8,	/* IV size for crypto */
	 .maxauthsize		= 16,	/* Max auth data size in bytes */
	 .setkey 		= apm_sec_setkey_esp_gcm,
	 .setauthsize		= apm_sec_setauthsize_aes,
	 .encrypt 		= apm_sec_encrypt_esp_transport_gcm,
	 .decrypt 		= apm_sec_decrypt_esp_transport_gcm,
	 }}}},

	 { .type = CRYPTO_ALG_TYPE_AEAD, .u.cipher = {
	 .cra_name 		= "transport(esp(rfc4309(ccm(aes))))",
	 .cra_driver_name 	= "apm-transport-esp-rfc4309-ccm-aes",
	 .cra_priority 		= APM_SEC_CRYPTO_PRIORITY_IPSEC,
	 .cra_flags 		= CRYPTO_ALG_TYPE_AEAD | CRYPTO_ALG_ASYNC,
	 .cra_blocksize 	= 16,	/* 128-bits block */
	 .cra_ctxsize 		= sizeof(struct apm_sec_session_ctx),
	 .cra_alignmask 	= 0xF,	/* Hardware requires 16 bytes aligned */
	 .cra_type 		= &crypto_aead_type,
	 .cra_module 		= THIS_MODULE,
	 .cra_u 		= {.aead = {
	 .ivsize 		= 8,	/* IV size for crypto */
	 .maxauthsize		= 16,	/* Max auth data size in bytes */
	 .setkey 		= apm_sec_setkey_esp_ccm,
	 .setauthsize		= apm_sec_setauthsize_aes_ccm,
	 .encrypt 		= apm_sec_encrypt_esp_transport_ccm,
	 .decrypt 		= apm_sec_decrypt_esp_transport_ccm,
	 }}}},

	/* IPsec Tunnel Mode ESP Algorithms*/
	{ .type = CRYPTO_ALG_TYPE_AEAD, .u.cipher = {
	 .cra_name 		= "tunnel(esp(authenc(hmac(md5),cbc(aes))))",
	 .cra_driver_name 	= "apm-tunnel-esp-hmac-md5-cbc-aes",
	 .cra_priority 		= APM_SEC_CRYPTO_PRIORITY_IPSEC,
	 .cra_flags 		= CRYPTO_ALG_TYPE_AEAD | CRYPTO_ALG_ASYNC,
	 .cra_blocksize 	= 16,	/* 128-bits block */
	 .cra_ctxsize 		= sizeof(struct apm_sec_session_ctx),
	 .cra_alignmask 	= 0xF,	
	 .cra_type 		= &crypto_aead_type,
	 .cra_u 		= {.aead = {
	 .ivsize 		= 16,	/* IV size is 16 bytes */
	 .maxauthsize		= 16,	/* Max auth data size in bytes */
	 .setkey 		= apm_sec_setkey_esp,
	 .setauthsize		= apm_sec_setauthsize_aes,
	 .encrypt 		= apm_sec_encrypt_esp_tunnel,
	 .decrypt 		= apm_sec_decrypt_esp_tunnel,
	 }}}},

	{ .type = CRYPTO_ALG_TYPE_AEAD, .u.cipher = {
	 .cra_name 		= "tunnel(esp(authenc(hmac(sha1),cbc(aes))))",
	 .cra_driver_name 	= "apm-tunnel-esp-hmac-sha1-cbc-aes",
	 .cra_priority 		= APM_SEC_CRYPTO_PRIORITY_IPSEC,
	 .cra_flags 		= CRYPTO_ALG_TYPE_AEAD | CRYPTO_ALG_ASYNC,
	 .cra_blocksize 	= 16,	/* 128-bits block */
	 .cra_ctxsize 		= sizeof(struct apm_sec_session_ctx),
	 .cra_alignmask 	= 0xF,	
	 .cra_type 		= &crypto_aead_type,
	 .cra_u 		= {.aead = {
	 .ivsize 		= 16,	/* IV size is 16 bytes */
	 .maxauthsize		= 20,	/* Max auth data size in bytes */
	 .setkey 		= apm_sec_setkey_esp,
	 .setauthsize		= apm_sec_setauthsize_aes,
	 .encrypt 		= apm_sec_encrypt_esp_tunnel,
	 .decrypt 		= apm_sec_decrypt_esp_tunnel,
	 }}}},

	 { .type = CRYPTO_ALG_TYPE_AEAD, .u.cipher = {
	 .cra_name 		= "tunnel(esp(authenc(hmac(sha224),cbc(aes))))",
	 .cra_driver_name 	= "apm-tunnel-esp-hmac-sha224-cbc-aes",
	 .cra_priority 		= APM_SEC_CRYPTO_PRIORITY_IPSEC,
	 .cra_flags 		= CRYPTO_ALG_TYPE_AEAD | CRYPTO_ALG_ASYNC,
	 .cra_blocksize 	= 16,	/* 128-bits block */
	 .cra_ctxsize 		= sizeof(struct apm_sec_session_ctx),
	 .cra_alignmask 	= 0xF,	
	 .cra_type 		= &crypto_aead_type,
	 .cra_u 		= {.aead = {
	 .ivsize 		= 16,	/* IV size is 16 bytes */
	 .maxauthsize		= 28,	/* Max auth data size in bytes */
	 .setkey 		= apm_sec_setkey_esp,
	 .setauthsize		= apm_sec_setauthsize_aes,
	 .encrypt 		= apm_sec_encrypt_esp_tunnel,
	 .decrypt 		= apm_sec_decrypt_esp_tunnel,
	 }}}},

	 { .type = CRYPTO_ALG_TYPE_AEAD, .u.cipher = {
	 .cra_name 		= "tunnel(esp(authenc(hmac(sha256),cbc(aes))))",
	 .cra_driver_name 	= "apm-tunnel-esp-hmac-sha256-cbc-aes",
	 .cra_priority 		= APM_SEC_CRYPTO_PRIORITY_IPSEC,
	 .cra_flags 		= CRYPTO_ALG_TYPE_AEAD | CRYPTO_ALG_ASYNC,
	 .cra_blocksize 	= 16,	/* 128-bits block */
	 .cra_ctxsize 		= sizeof(struct apm_sec_session_ctx),
	 .cra_alignmask 	= 0xF,	
	 .cra_type 		= &crypto_aead_type,
	 .cra_u 		= {.aead = {
	 .ivsize 		= 16,	/* IV size is 16 bytes */
	 .maxauthsize		= 32,	/* Max auth data size in bytes */
	 .setkey 		= apm_sec_setkey_esp,
	 .setauthsize		= apm_sec_setauthsize_aes,
	 .encrypt 		= apm_sec_encrypt_esp_tunnel,
	 .decrypt 		= apm_sec_decrypt_esp_tunnel,
	 }}}},

	 { .type = CRYPTO_ALG_TYPE_AEAD, .u.cipher = {
	 .cra_name 		= "tunnel(esp(authenc(hmac(sha384),cbc(aes))))",
	 .cra_driver_name 	= "apm-tunnel-esp-hmac-sha384-cbc-aes",
	 .cra_priority 		= APM_SEC_CRYPTO_PRIORITY_IPSEC,
	 .cra_flags 		= CRYPTO_ALG_TYPE_AEAD | CRYPTO_ALG_ASYNC,
	 .cra_blocksize 	= 16,	/* 128-bits block */
	 .cra_ctxsize 		= sizeof(struct apm_sec_session_ctx),
	 .cra_alignmask 	= 0xF,	
	 .cra_type 		= &crypto_aead_type,
	 .cra_u 		= {.aead = {
	 .ivsize 		= 16,	/* IV size is 16 bytes */
	 .maxauthsize		= 48,	/* Max auth data size in bytes */
	 .setkey 		= apm_sec_setkey_esp,
	 .setauthsize		= apm_sec_setauthsize_aes,
	 .encrypt 		= apm_sec_encrypt_esp_tunnel,
	 .decrypt 		= apm_sec_decrypt_esp_tunnel,
	 }}}},

	 { .type = CRYPTO_ALG_TYPE_AEAD, .u.cipher = {
	 .cra_name 		= "tunnel(esp(authenc(hmac(sha512),cbc(aes))))",
	 .cra_driver_name 	= "apm-tunnel-esp-hmac-sha512-cbc-aes",
	 .cra_priority 		= APM_SEC_CRYPTO_PRIORITY_IPSEC,
	 .cra_flags 		= CRYPTO_ALG_TYPE_AEAD | CRYPTO_ALG_ASYNC,
	 .cra_blocksize 	= 16,	/* 128-bits block */
	 .cra_ctxsize 		= sizeof(struct apm_sec_session_ctx),
	 .cra_alignmask 	= 0xF,	
	 .cra_type 		= &crypto_aead_type,
	 .cra_u 		= {.aead = {
	 .ivsize 		= 16,	/* IV size is 16 bytes */
	 .maxauthsize		= 64,	/* Max auth data size in bytes */
	 .setkey 		= apm_sec_setkey_esp,
	 .setauthsize		= apm_sec_setauthsize_aes,
	 .encrypt 		= apm_sec_encrypt_esp_tunnel,
	 .decrypt 		= apm_sec_decrypt_esp_tunnel,
	 }}}},

	 { .type = CRYPTO_ALG_TYPE_AEAD, .u.cipher = {
	 .cra_name 		= "tunnel(esp(authenc(hmac(md5),cbc(des3_ede))))",
	 .cra_driver_name 	= "apm-tunnel-esp-hmac-md5-cbc-des3",
	 .cra_priority 		= APM_SEC_CRYPTO_PRIORITY_IPSEC,
	 .cra_flags 		= CRYPTO_ALG_TYPE_AEAD | CRYPTO_ALG_ASYNC,
	 .cra_blocksize 	= 8,	/* 128-bits block */
	 .cra_ctxsize 		= sizeof(struct apm_sec_session_ctx),
	 .cra_alignmask 	= 0xF,	
	 .cra_type 		= &crypto_aead_type,
	 .cra_u 		= {.aead = {
	 .ivsize 		= 8,	/* IV size is 16 bytes */
	 .maxauthsize		= 16,	/* Max auth data size in bytes */
	 .setkey 		= apm_sec_setkey_esp_des3,
	 .setauthsize		= apm_sec_setauthsize_aes,
	 .encrypt 		= apm_sec_encrypt_esp_tunnel,
	 .decrypt 		= apm_sec_decrypt_esp_tunnel,
	 }}}},

	 { .type = CRYPTO_ALG_TYPE_AEAD, .u.cipher = {
	 .cra_name 		= "tunnel(esp(authenc(hmac(sha1),cbc(des3_ede))))",
	 .cra_driver_name 	= "apm-tunnel-esp-hmac-sha1-cbc-des3",
	 .cra_priority 		= APM_SEC_CRYPTO_PRIORITY_IPSEC,
	 .cra_flags 		= CRYPTO_ALG_TYPE_AEAD | CRYPTO_ALG_ASYNC,
	 .cra_blocksize 	= 8,	/* 128-bits block */
	 .cra_ctxsize 		= sizeof(struct apm_sec_session_ctx),
	 .cra_alignmask 	= 0xF,	
	 .cra_type 		= &crypto_aead_type,
	 .cra_u 		= {.aead = {
	 .ivsize 		= 8,	/* IV size is 16 bytes */
	 .maxauthsize		= 20,	/* Max auth data size in bytes */
	 .setkey 		= apm_sec_setkey_esp_des3,
	 .setauthsize		= apm_sec_setauthsize_aes,
	 .encrypt 		= apm_sec_encrypt_esp_tunnel,
	 .decrypt 		= apm_sec_decrypt_esp_tunnel,
	 }}}},

	 { .type = CRYPTO_ALG_TYPE_AEAD, .u.cipher = {
	 .cra_name 		= "tunnel(esp(authenc(hmac(sha224),cbc(des3_ede))))",
	 .cra_driver_name 	= "apm-tunnel-esp-hmac-sha224-cbc-des3",
	 .cra_priority 		= APM_SEC_CRYPTO_PRIORITY_IPSEC,
	 .cra_flags 		= CRYPTO_ALG_TYPE_AEAD | CRYPTO_ALG_ASYNC,
	 .cra_blocksize 	= 8,	/* 128-bits block */
	 .cra_ctxsize 		= sizeof(struct apm_sec_session_ctx),
	 .cra_alignmask 	= 0xF,	
	 .cra_type 		= &crypto_aead_type,
	 .cra_u 		= {.aead = {
	 .ivsize 		= 8,	/* IV size is 16 bytes */
	 .maxauthsize		= 28,	/* Max auth data size in bytes */
	 .setkey 		= apm_sec_setkey_esp_des3,
	 .setauthsize		= apm_sec_setauthsize_aes,
	 .encrypt 		= apm_sec_encrypt_esp_tunnel,
	 .decrypt 		= apm_sec_decrypt_esp_tunnel,
	 }}}},

	 { .type = CRYPTO_ALG_TYPE_AEAD, .u.cipher = {
	 .cra_name 		= "tunnel(esp(authenc(hmac(sha256),cbc(des3_ede))))",
	 .cra_driver_name 	= "apm-tunnel-esp-hmac-sha256-cbc-des3",
	 .cra_priority 		= APM_SEC_CRYPTO_PRIORITY_IPSEC,
	 .cra_flags 		= CRYPTO_ALG_TYPE_AEAD | CRYPTO_ALG_ASYNC,
	 .cra_blocksize 	= 8,	/* 128-bits block */
	 .cra_ctxsize 		= sizeof(struct apm_sec_session_ctx),
	 .cra_alignmask 	= 0xF,	
	 .cra_type 		= &crypto_aead_type,
	 .cra_u 		= {.aead = {
	 .ivsize 		= 8,	/* IV size is 16 bytes */
	 .maxauthsize		= 32,	/* Max auth data size in bytes */
	 .setkey 		= apm_sec_setkey_esp_des3,
	 .setauthsize		= apm_sec_setauthsize_aes,
	 .encrypt 		= apm_sec_encrypt_esp_tunnel,
	 .decrypt 		= apm_sec_decrypt_esp_tunnel,
	 }}}},

	 { .type = CRYPTO_ALG_TYPE_AEAD, .u.cipher = {
	 .cra_name 		= "tunnel(esp(authenc(hmac(sha384),cbc(des3_ede))))",
	 .cra_driver_name 	= "apm-tunnel-esp-hmac-sha384-cbc-des3",
	 .cra_priority 		= APM_SEC_CRYPTO_PRIORITY_IPSEC,
	 .cra_flags 		= CRYPTO_ALG_TYPE_AEAD | CRYPTO_ALG_ASYNC,
	 .cra_blocksize 	= 8,	/* 128-bits block */
	 .cra_ctxsize 		= sizeof(struct apm_sec_session_ctx),
	 .cra_alignmask 	= 0xF,	
	 .cra_type 		= &crypto_aead_type,
	 .cra_u 		= {.aead = {
	 .ivsize 		= 8,	/* IV size is 16 bytes */
	 .maxauthsize		= 48,	/* Max auth data size in bytes */
	 .setkey 		= apm_sec_setkey_esp_des3,
	 .setauthsize		= apm_sec_setauthsize_aes,
	 .encrypt 		= apm_sec_encrypt_esp_tunnel,
	 .decrypt 		= apm_sec_decrypt_esp_tunnel,
	 }}}},

	 { .type = CRYPTO_ALG_TYPE_AEAD, .u.cipher = {
	 .cra_name 		= "tunnel(esp(authenc(hmac(sha512),cbc(des3_ede))))",
	 .cra_driver_name 	= "apm-tunnel-esp-hmac-sha512-cbc-des3",
	 .cra_priority 		= APM_SEC_CRYPTO_PRIORITY_IPSEC,
	 .cra_flags 		= CRYPTO_ALG_TYPE_AEAD | CRYPTO_ALG_ASYNC,
	 .cra_blocksize 	= 8,	/* 128-bits block */
	 .cra_ctxsize 		= sizeof(struct apm_sec_session_ctx),
	 .cra_alignmask 	= 0xF,	
	 .cra_type 		= &crypto_aead_type,
	 .cra_u 		= {.aead = {
	 .ivsize 		= 8,	/* IV size is 16 bytes */
	 .maxauthsize		= 64,	/* Max auth data size in bytes */
	 .setkey 		= apm_sec_setkey_esp_des3,
	 .setauthsize		= apm_sec_setauthsize_aes,
	 .encrypt 		= apm_sec_encrypt_esp_tunnel,
	 .decrypt 		= apm_sec_decrypt_esp_tunnel,
	 }}}},

	 { .type = CRYPTO_ALG_TYPE_AEAD, .u.cipher = {
	 .cra_name 		= "tunnel(esp(authenc(hmac(md5),cbc(des))))",
	 .cra_driver_name 	= "apm-tunnel-esp-hmac-md5-cbc-des",
	 .cra_priority 		= APM_SEC_CRYPTO_PRIORITY_IPSEC,
	 .cra_flags 		= CRYPTO_ALG_TYPE_AEAD | CRYPTO_ALG_ASYNC,
	 .cra_blocksize 	= 8,	/* 128-bits block */
	 .cra_ctxsize 		= sizeof(struct apm_sec_session_ctx),
	 .cra_alignmask 	= 0xF,	
	 .cra_type 		= &crypto_aead_type,
	 .cra_u 		= {.aead = {
	 .ivsize 		= 8,	/* IV size is 16 bytes */
	 .maxauthsize		= 16,	/* Max auth data size in bytes */
	 .setkey 		= apm_sec_setkey_esp_des,
	 .setauthsize		= apm_sec_setauthsize_aes,
	 .encrypt 		= apm_sec_encrypt_esp_tunnel,
	 .decrypt 		= apm_sec_decrypt_esp_tunnel,
	 }}}},

	 { .type = CRYPTO_ALG_TYPE_AEAD, .u.cipher = {
	 .cra_name 		= "tunnel(esp(authenc(hmac(sha1),cbc(des))))",
	 .cra_driver_name 	= "apm-tunnel-esp-hmac-sha1-cbc-des",
	 .cra_priority 		= APM_SEC_CRYPTO_PRIORITY_IPSEC,
	 .cra_flags 		= CRYPTO_ALG_TYPE_AEAD | CRYPTO_ALG_ASYNC,
	 .cra_blocksize 	= 8,	/* 128-bits block */
	 .cra_ctxsize 		= sizeof(struct apm_sec_session_ctx),
	 .cra_alignmask 	= 0xF,	
	 .cra_type 		= &crypto_aead_type,
	 .cra_u 		= {.aead = {
	 .ivsize 		= 8,	/* IV size is 16 bytes */
	 .maxauthsize		= 20,	/* Max auth data size in bytes */
	 .setkey 		= apm_sec_setkey_esp_des,
	 .setauthsize		= apm_sec_setauthsize_aes,
	 .encrypt 		= apm_sec_encrypt_esp_tunnel,
	 .decrypt 		= apm_sec_decrypt_esp_tunnel,
	 }}}},

	 { .type = CRYPTO_ALG_TYPE_AEAD, .u.cipher = {
	 .cra_name 		= "tunnel(esp(authenc(hmac(sha224),cbc(des))))",
	 .cra_driver_name 	= "apm-tunnel-esp-hmac-sha224-cbc-des",
	 .cra_priority 		= APM_SEC_CRYPTO_PRIORITY_IPSEC,
	 .cra_flags 		= CRYPTO_ALG_TYPE_AEAD | CRYPTO_ALG_ASYNC,
	 .cra_blocksize 	= 8,	/* 128-bits block */
	 .cra_ctxsize 		= sizeof(struct apm_sec_session_ctx),
	 .cra_alignmask 	= 0xF,	
	 .cra_type 		= &crypto_aead_type,
	 .cra_u 		= {.aead = {
	 .ivsize 		= 8,	/* IV size is 16 bytes */
	 .maxauthsize		= 28,	/* Max auth data size in bytes */
	 .setkey 		= apm_sec_setkey_esp_des,
	 .setauthsize		= apm_sec_setauthsize_aes,
	 .encrypt 		= apm_sec_encrypt_esp_tunnel,
	 .decrypt 		= apm_sec_decrypt_esp_tunnel,
	 }}}},

	 { .type = CRYPTO_ALG_TYPE_AEAD, .u.cipher = {
	 .cra_name 		= "tunnel(esp(authenc(hmac(sha256),cbc(des))))",
	 .cra_driver_name 	= "apm-tunnel-esp-hmac-sha256-cbc-des",
	 .cra_priority 		= APM_SEC_CRYPTO_PRIORITY_IPSEC,
	 .cra_flags 		= CRYPTO_ALG_TYPE_AEAD | CRYPTO_ALG_ASYNC,
	 .cra_blocksize 	= 8,	/* 128-bits block */
	 .cra_ctxsize 		= sizeof(struct apm_sec_session_ctx),
	 .cra_alignmask 	= 0xF,	
	 .cra_type 		= &crypto_aead_type,
	 .cra_u 		= {.aead = {
	 .ivsize 		= 8,	/* IV size is 16 bytes */
	 .maxauthsize		= 32,	/* Max auth data size in bytes */
	 .setkey 		= apm_sec_setkey_esp_des,
	 .setauthsize		= apm_sec_setauthsize_aes,
	 .encrypt 		= apm_sec_encrypt_esp_tunnel,
	 .decrypt 		= apm_sec_decrypt_esp_tunnel,
	 }}}},

	{ .type = CRYPTO_ALG_TYPE_AEAD, .u.cipher = {
	 .cra_name 		= "tunnel(esp(authenc(hmac(sha384),cbc(des))))",
	 .cra_driver_name 	= "apm-tunnel-esp-hmac-sha384-cbc-des",
	 .cra_priority 		= APM_SEC_CRYPTO_PRIORITY_IPSEC,
	 .cra_flags 		= CRYPTO_ALG_TYPE_AEAD | CRYPTO_ALG_ASYNC,
	 .cra_blocksize 	= 8,	/* 128-bits block */
	 .cra_ctxsize 		= sizeof(struct apm_sec_session_ctx),
	 .cra_alignmask 	= 0xF,	
	 .cra_type 		= &crypto_aead_type,
	 .cra_u 		= {.aead = {
	 .ivsize 		= 8,	/* IV size is 16 bytes */
	 .maxauthsize		= 48,	/* Max auth data size in bytes */
	 .setkey 		= apm_sec_setkey_esp_des,
	 .setauthsize		= apm_sec_setauthsize_aes,
	 .encrypt 		= apm_sec_encrypt_esp_tunnel,
	 .decrypt 		= apm_sec_decrypt_esp_tunnel,
	 }}}},

	 { .type = CRYPTO_ALG_TYPE_AEAD, .u.cipher = {
	 .cra_name 		= "tunnel(esp(authenc(hmac(sha512),cbc(des))))",
	 .cra_driver_name 	= "apm-tunnel-esp-hmac-sha512-cbc-des",
	 .cra_priority 		= APM_SEC_CRYPTO_PRIORITY_IPSEC,
	 .cra_flags 		= CRYPTO_ALG_TYPE_AEAD | CRYPTO_ALG_ASYNC,
	 .cra_blocksize 	= 8,	/* 128-bits block */
	 .cra_ctxsize 		= sizeof(struct apm_sec_session_ctx),
	 .cra_alignmask 	= 0xF,	
	 .cra_type 		= &crypto_aead_type,
	 .cra_u 		= {.aead = {
	 .ivsize 		= 8,	/* IV size is 16 bytes */
	 .maxauthsize		= 32,	/* Max auth data size in bytes */
	 .setkey 		= apm_sec_setkey_esp_des,
	 .setauthsize		= apm_sec_setauthsize_aes,
	 .encrypt 		= apm_sec_encrypt_esp_tunnel,
	 .decrypt 		= apm_sec_decrypt_esp_tunnel,
	 }}}},

	 /* IPSec Algorithms */
	 {.type = CRYPTO_ALG_TYPE_AEAD, .u.cipher = {
	 .cra_name 		= "rfc4106(gcm(aes))",
	 .cra_driver_name 	= "apm-aes-ipsec",
	 .cra_priority 		= APM_SEC_CRYPTO_PRIORITY_IPSEC,
	 .cra_flags 		= CRYPTO_ALG_TYPE_AEAD | CRYPTO_ALG_ASYNC,
	 .cra_blocksize 	= 16,	/* 128-bits block */
	 .cra_ctxsize 		= sizeof(struct apm_sec_session_ctx),
	 .cra_alignmask 	= 0xF,	/* Hardware requires 16 bytes aligned */
	 .cra_type 		= &crypto_aead_type,
	 .cra_u 		= {.aead = {
	 .ivsize 		= 16,		/* IV size is 16 bytes */
	 .setkey 		= apm_sec_setkey_esp_gcm,
	 .setauthsize		= apm_sec_setauthsize_aes,
	 .encrypt 		= apm_sec_encrypt_esp_tunnel,
	 .decrypt 		= apm_sec_decrypt_esp_tunnel,
	 }}}},

	 { .type = CRYPTO_ALG_TYPE_AEAD, .u.cipher = {
	 .cra_name 		= "tunnel(esp(rfc4106(gcm(aes))))",
	 .cra_driver_name 	= "apm-tunnel-esp-rfc4106-gcm-aes",
	 .cra_priority 		= APM_SEC_CRYPTO_PRIORITY_IPSEC,
	 .cra_flags 		= CRYPTO_ALG_TYPE_AEAD | CRYPTO_ALG_ASYNC,
	 .cra_blocksize 	= 16,	/* 128-bits block */
	 .cra_ctxsize 		= sizeof(struct apm_sec_session_ctx),
	 .cra_alignmask 	= 0xF,	
	 .cra_type 		= &crypto_aead_type,
	 .cra_u 		= {.aead = {
	 .ivsize 		= 8,	/* IV size is 16 bytes */
	 .maxauthsize		= 16,	/* Max auth data size in bytes */
	 .setkey 		= apm_sec_setkey_esp_gcm,
	 .setauthsize		= apm_sec_setauthsize_aes,
	 .encrypt 		= apm_sec_encrypt_esp_tunnel_gcm,
	 .decrypt 		= apm_sec_decrypt_esp_tunnel_gcm,
	 }}}},

	 { .type = CRYPTO_ALG_TYPE_AEAD, .u.cipher = {
	 .cra_name 		= "tunnel(esp(authenc(hmac(md5),rfc3686(ctr(aes)))))",
	 .cra_driver_name 	= "apm-tunnel-esp-hmac-md5-ctr-aes",
	 .cra_priority 		= APM_SEC_CRYPTO_PRIORITY_IPSEC,
	 .cra_flags 		= CRYPTO_ALG_TYPE_AEAD | CRYPTO_ALG_ASYNC,
	 .cra_blocksize 	= 16,	/* 128-bits block */
	 .cra_ctxsize 		= sizeof(struct apm_sec_session_ctx),
	 .cra_alignmask 	= 0xF,	
	 .cra_type 		= &crypto_aead_type,
	 .cra_u 		= {.aead = {
	 .ivsize 		= 8,	/* IV size is 16 bytes */
	 .maxauthsize		= 16,	/* Max auth data size in bytes */
	 .setkey 		= apm_sec_setkey_esp_aes_ctr,
	 .setauthsize		= apm_sec_setauthsize_aes,
	 .encrypt 		= apm_sec_encrypt_esp_tunnel,
	 .decrypt 		= apm_sec_decrypt_esp_tunnel,
	 }}}},

	 { .type = CRYPTO_ALG_TYPE_AEAD, .u.cipher = {
	 .cra_name 		= "tunnel(esp(authenc(hmac(sha1),rfc3686(ctr(aes)))))",
	 .cra_driver_name 	= "apm-tunnel-esp-hmac-sha1-ctr-aes",
	 .cra_priority 		= APM_SEC_CRYPTO_PRIORITY_IPSEC,
	 .cra_flags 		= CRYPTO_ALG_TYPE_AEAD | CRYPTO_ALG_ASYNC,
	 .cra_blocksize 	= 16,	/* 128-bits block */
	 .cra_ctxsize 		= sizeof(struct apm_sec_session_ctx),
	 .cra_alignmask 	= 0xF,	
	 .cra_type 		= &crypto_aead_type,
	 .cra_u 		= {.aead = {
	 .ivsize 		= 8,	/* IV size is 16 bytes */
	 .maxauthsize		= 20,	/* Max auth data size in bytes */
	 .setkey 		= apm_sec_setkey_esp_aes_ctr,
	 .setauthsize		= apm_sec_setauthsize_aes,
	 .encrypt 		= apm_sec_encrypt_esp_tunnel,
	 .decrypt 		= apm_sec_decrypt_esp_tunnel,
	 }}}},

	{ .type = CRYPTO_ALG_TYPE_AEAD, .u.cipher = {
	 .cra_name 		= "tunnel(esp(authenc(hmac(sha224),rfc3686(ctr(aes)))))",
	 .cra_driver_name 	= "apm-tunnel-esp-hmac-sha224-ctr-aes",
	 .cra_priority 		= APM_SEC_CRYPTO_PRIORITY_IPSEC,
	 .cra_flags 		= CRYPTO_ALG_TYPE_AEAD | CRYPTO_ALG_ASYNC,
	 .cra_blocksize 	= 16,	/* 128-bits block */
	 .cra_ctxsize 		= sizeof(struct apm_sec_session_ctx),
	 .cra_alignmask 	= 0xF,	
	 .cra_type 		= &crypto_aead_type,
	 .cra_u 		= {.aead = {
	 .ivsize 		= 8,	/* IV size is 16 bytes */
	 .maxauthsize		= 28,	/* Max auth data size in bytes */
	 .setkey 		= apm_sec_setkey_esp_aes_ctr,
	 .setauthsize		= apm_sec_setauthsize_aes,
	 .encrypt 		= apm_sec_encrypt_esp_tunnel,
	 .decrypt 		= apm_sec_decrypt_esp_tunnel,
	 }}}},

	 { .type = CRYPTO_ALG_TYPE_AEAD, .u.cipher = {
	 .cra_name 		= "tunnel(esp(authenc(hmac(sha256),rfc3686(ctr(aes)))))",
	 .cra_driver_name 	= "apm-tunnel-esp-hmac-sha256-ctr-aes",
	 .cra_priority 		= APM_SEC_CRYPTO_PRIORITY_IPSEC,
	 .cra_flags 		= CRYPTO_ALG_TYPE_AEAD | CRYPTO_ALG_ASYNC,
	 .cra_blocksize 	= 16,	/* 128-bits block */
	 .cra_ctxsize 		= sizeof(struct apm_sec_session_ctx),
	 .cra_alignmask 	= 0xF,	
	 .cra_type 		= &crypto_aead_type,
	 .cra_u 		= {.aead = {
	 .ivsize 		= 8,	/* IV size is 16 bytes */
	 .maxauthsize		= 32,	/* Max auth data size in bytes */
	 .setkey 		= apm_sec_setkey_esp_aes_ctr,
	 .setauthsize		= apm_sec_setauthsize_aes,
	 .encrypt 		= apm_sec_encrypt_esp_tunnel,
	 .decrypt 		= apm_sec_decrypt_esp_tunnel,
	 }}}},

	 { .type = CRYPTO_ALG_TYPE_AEAD, .u.cipher = {
	 .cra_name 		= "tunnel(esp(authenc(hmac(sha384),rfc3686(ctr(aes)))))",
	 .cra_driver_name 	= "apm-tunnel-esp-hmac-sha384-ctr-aes",
	 .cra_priority 		= APM_SEC_CRYPTO_PRIORITY_IPSEC,
	 .cra_flags 		= CRYPTO_ALG_TYPE_AEAD | CRYPTO_ALG_ASYNC,
	 .cra_blocksize 	= 16,	/* 128-bits block */
	 .cra_ctxsize 		= sizeof(struct apm_sec_session_ctx),
	 .cra_alignmask 	= 0xF,	
	 .cra_type 		= &crypto_aead_type,
	 .cra_u 		= {.aead = {
	 .ivsize 		= 8,	/* IV size is 16 bytes */
	 .maxauthsize		= 48,	/* Max auth data size in bytes */
	 .setkey 		= apm_sec_setkey_esp_aes_ctr,
	 .setauthsize		= apm_sec_setauthsize_aes,
	 .encrypt 		= apm_sec_encrypt_esp_tunnel,
	 .decrypt 		= apm_sec_decrypt_esp_tunnel,
	 }}}},

	 { .type = CRYPTO_ALG_TYPE_AEAD, .u.cipher = {
	 .cra_name 		= "tunnel(esp(authenc(hmac(sha512),rfc3686(ctr(aes)))))",
	 .cra_driver_name 	= "apm-tunnel-esp-hmac-sha512-ctr-aes",
	 .cra_priority 		= APM_SEC_CRYPTO_PRIORITY_IPSEC,
	 .cra_flags 		= CRYPTO_ALG_TYPE_AEAD | CRYPTO_ALG_ASYNC,
	 .cra_blocksize 	= 16,	/* 128-bits block */
	 .cra_ctxsize 		= sizeof(struct apm_sec_session_ctx),
	 .cra_alignmask 	= 0xF,	
	 .cra_type 		= &crypto_aead_type,
	 .cra_u 		= {.aead = {
	 .ivsize 		= 8,	/* IV size is 16 bytes */
	 .maxauthsize		= 64,	/* Max auth data size in bytes */
	 .setkey 		= apm_sec_setkey_esp_aes_ctr,
	 .setauthsize		= apm_sec_setauthsize_aes,
	 .encrypt 		= apm_sec_encrypt_esp_tunnel,
	 .decrypt 		= apm_sec_decrypt_esp_tunnel,
	 }}}},
#endif

	 /* Linux provides init alg testing for all the algorithms above */
#if 0
	{ .type = CRYPTO_ALG_TYPE_AEAD, .u.cipher = {
		.cra_name 		= "rfc4309(ccm(aes))",
		.cra_driver_name 	= "apm-aes-ipsec",
		.cra_priority 		= APM_SEC_CRYPTO_PRIORITY_IPSEC,
		.cra_flags 		= CRYPTO_ALG_TYPE_AEAD | CRYPTO_ALG_ASYNC,
		.cra_blocksize 	= 16,	/* 128-bits block */
		.cra_ctxsize 		= sizeof(struct apm_sec_session_ctx),
		.cra_alignmask 	= 0xF,	/* Hardware requires 16 bytes aligned */
		.cra_type 		= &crypto_aead_type,
		.cra_u 		= {.aead = {
		.ivsize 		= 16,	/* IV size is 16 bytes */
		.maxauthsize		= 16,	/* Max auth data size in bytes */
		.setkey 		= apm_sec_setkey_esp_ccm,
		.setauthsize		= apm_sec_setauthsize_aes_ccm,
		.encrypt 		= apm_sec_encrypt_esp_transport_ccm,
		.decrypt 		= apm_sec_encrypt_esp_transport_ccm,
	}}}},
	
		/* IPSec combined hash and crypto Algorithms */
	{.cra_name 		= "authenc(hmac(md5),cbc(aes))",
		.cra_driver_name 	= "apm-authenc-hmac-md5-cbc-aes",
		.cra_priority 		= APM_SEC_CRYPTO_PRIORITY_IPSEC,
		.cra_flags 		= CRYPTO_ALG_TYPE_AEAD | CRYPTO_ALG_ASYNC,
		.cra_blocksize 	= 16,	/* 128-bits block */
		.cra_ctxsize 		= sizeof(struct apm_sec_session_ctx),
		.cra_alignmask 	= 0xF,	/* Hardware requires 16 bytes aligned */
		.cra_type 		= &crypto_aead_type,
		.cra_u 		= {.aead = {
		.ivsize 		= 16,	/* IV size is 16 bytes */
		.maxauthsize		= 16,	/* Max auth data size in bytes */
		.setkey 		= apm_sec_setkey_aes_md5_authenc,
		.setauthsize		= apm_sec_setauthsize_aes,
		.encrypt 		= apm_sec_encrypt_cbc_authenc,
		.decrypt 		= apm_sec_decrypt_cbc_authenc,
		.givencrypt		= apm_sec_givencrypt_cbc_authenc,
	}}},
	{.cra_name 		= "authenc(hmac(sha1),cbc(aes))",
		.cra_driver_name 	= "apm-authenc-hmac-sha1-cbc-aes",
		.cra_priority 		= APM_SEC_CRYPTO_PRIORITY_IPSEC,
		.cra_flags 		= CRYPTO_ALG_TYPE_AEAD | CRYPTO_ALG_ASYNC,
		.cra_blocksize 	= 16,	/* 128-bits block */
		.cra_ctxsize 		= sizeof(struct apm_sec_session_ctx),
		.cra_alignmask 	= 0xF,	/* Hardware requires 16 bytes aligned */
		.cra_type 		= &crypto_aead_type,
		.cra_u 		= {.aead = {
		.ivsize 		= 16,	/* IV size is 16 bytes */
		.maxauthsize		= 16,	/* Max auth data size in bytes */
		.setkey 		= apm_sec_setkey_aes_sha1_authenc,
		.setauthsize		= apm_sec_setauthsize_aes,
		.encrypt 		= apm_sec_encrypt_cbc_authenc,
		.decrypt 		= apm_sec_decrypt_cbc_authenc,
		.givencrypt		= apm_sec_givencrypt_cbc_authenc,
	}}},
	{.cra_name 		= "authenc(hmac(sha256),cbc(aes))",
		.cra_driver_name 	= "apm-authenc-hmac-sha256-cbc-aes",
		.cra_priority 		= APM_SEC_CRYPTO_PRIORITY_IPSEC,
		.cra_flags 		= CRYPTO_ALG_TYPE_AEAD | CRYPTO_ALG_ASYNC,
		.cra_blocksize 	= 16,	/* 128-bits block */
		.cra_ctxsize 		= sizeof(struct apm_sec_session_ctx),
		.cra_alignmask 	= 0xF,	/* Hardware requires 16 bytes aligned */
		.cra_type 		= &crypto_aead_type,
		.cra_u 		= {.aead = {
		.ivsize 		= 16,	/* IV size is 16 bytes */
		.maxauthsize		= 16,	/* Max auth data size in bytes */
		.setkey 		= apm_sec_setkey_aes_sha256_authenc,
		.setauthsize		= apm_sec_setauthsize_aes,
		.encrypt 		= apm_sec_encrypt_cbc_authenc,
		.decrypt 		= apm_sec_decrypt_cbc_authenc,
		.givencrypt		= apm_sec_givencrypt_cbc_authenc,
	}}},
	 /* IPsec Transport Mode AH Algorithms*/
	 {.cra_name 		= "transport(ah(hmac(md5)))",
	 .cra_driver_name 	= "apm-transport-ah-hmac-md5",
	 .cra_flags 		= CRYPTO_ALG_TYPE_AEAD | CRYPTO_ALG_ASYNC,
	 .cra_blocksize 	= 64,	/* MD5-HMAC block size is 512-bits */
	 .cra_ctxsize 		= sizeof(struct apm_sec_session_ctx),
	 .cra_alignmask 	= 0xF,	/* Hardware requires 16 bytes aligned */
	 .cra_type 		= &crypto_aead_type,
	 .cra_u 		= {.aead = {
	 .ivsize 		= 0,	/* IV size for crypto */
	 .maxauthsize		= 16,	/* Max auth data size in bytes */
	 .setkey 		= apm_sec_setkey_ah_md5,
	 .setauthsize		= apm_sec_setauthsize_aes,
	 .encrypt 		= apm_sec_encrypt_ah_transport,
	 .decrypt 		= apm_sec_decrypt_ah_transport,
	 }}},
	 {.cra_name 		= "transport(ah(hmac(sha1)))",
	 .cra_driver_name 	= "apm-transport-ah-hmac-sha1",
	 .cra_flags 		= CRYPTO_ALG_TYPE_AEAD | CRYPTO_ALG_ASYNC,
	 .cra_blocksize 	= 64,	/* MD5-HMAC block size is 512-bits */
	 .cra_ctxsize 		= sizeof(struct apm_sec_session_ctx),
	 .cra_alignmask 	= 0xF,	/* Hardware requires 16 bytes aligned */
	 .cra_type 		= &crypto_aead_type,
	 .cra_u 		= {.aead = {
	 .ivsize 		= 0,	/* IV size for crypto */
	 .maxauthsize		= 20,	/* Max auth data size in bytes */
	 .setkey 		= apm_sec_setkey_ah_sha1,
	 .setauthsize		= apm_sec_setauthsize_aes,
	 .encrypt 		= apm_sec_encrypt_ah_transport,
	 .decrypt 		= apm_sec_decrypt_ah_transport,
	 }}},
	 {.cra_name 		= "transport(ah(hmac(sha224)))",
	 .cra_driver_name 	= "apm-transport-ah-hmac-sha224",
	 .cra_flags 		= CRYPTO_ALG_TYPE_AEAD | CRYPTO_ALG_ASYNC,
	 .cra_blocksize 	= 64,	/* MD5-HMAC block size is 512-bits */
	 .cra_ctxsize 		= sizeof(struct apm_sec_session_ctx),
	 .cra_alignmask 	= 0xF,	/* Hardware requires 16 bytes aligned */
	 .cra_type 		= &crypto_aead_type,
	 .cra_u 		= {.aead = {
	 .ivsize 		= 0,	/* IV size for crypto */
	 .maxauthsize		= 28,	/* Max auth data size in bytes */
	 .setkey 		= apm_sec_setkey_ah_sha2,
	 .setauthsize		= apm_sec_setauthsize_aes,
	 .encrypt 		= apm_sec_encrypt_ah_transport,
	 .decrypt 		= apm_sec_decrypt_ah_transport,
	 }}},
	 {.cra_name 		= "transport(ah(hmac(sha256)))",
	 .cra_driver_name 	= "apm-transport-ah-hmac-sha224",
	 .cra_flags 		= CRYPTO_ALG_TYPE_AEAD | CRYPTO_ALG_ASYNC,
	 .cra_blocksize 	= 64,	/* MD5-HMAC block size is 512-bits */
	 .cra_ctxsize 		= sizeof(struct apm_sec_session_ctx),
	 .cra_alignmask 	= 0xF,	/* Hardware requires 16 bytes aligned */
	 .cra_type 		= &crypto_aead_type,
	 .cra_u 		= {.aead = {
	 .ivsize 		= 0,	/* IV size for crypto */
	 .maxauthsize		= 32,	/* Max auth data size in bytes */
	 .setkey 		= apm_sec_setkey_ah_sha2,
	 .setauthsize		= apm_sec_setauthsize_aes,
	 .encrypt 		= apm_sec_encrypt_ah_transport,
	 .decrypt 		= apm_sec_decrypt_ah_transport,
	 }}},
	 /* IPsec Tunnel Mode AH Algorithms*/
	 {.cra_name 		= "tunnel(ah(hmac(md5)))",
	 .cra_driver_name 	= "apm-tunnel-ah-hmac-md5",
	 .cra_flags 		= CRYPTO_ALG_TYPE_AEAD | CRYPTO_ALG_ASYNC,
	 .cra_blocksize 	= 64,	/* MD5-HMAC block size is 512-bits */
	 .cra_ctxsize 		= sizeof(struct apm_sec_session_ctx),
	 .cra_alignmask 	= 0xF,	/* Hardware requires 16 bytes aligned */
	 .cra_type 		= &crypto_aead_type,
	 .cra_u 		= {.aead = {
	.ivsize 		= 0,	/* IV size for crypto */
	 .maxauthsize		= 16,	/* Max auth data size in bytes */
	 .setkey 		= apm_sec_setkey_ah_md5,
	 .setauthsize		= apm_sec_setauthsize_aes,
	 .encrypt 		= apm_sec_encrypt_ah_tunnel,
	 .decrypt 		= apm_sec_decrypt_ah_tunnel,
	 }}},
	 {.cra_name 		= "tunnel(ah(hmac(sha224)))",
	 .cra_driver_name 	= "apm-tunnel-ah-hmac-sha224",
	 .cra_flags 		= CRYPTO_ALG_TYPE_AEAD | CRYPTO_ALG_ASYNC,
	 .cra_blocksize 	= 64,	/* MD5-HMAC block size is 512-bits */
	 .cra_ctxsize 		= sizeof(struct apm_sec_session_ctx),
	 .cra_alignmask 	= 0xF,	/* Hardware requires 16 bytes aligned */
	 .cra_type 		= &crypto_aead_type,
	 .cra_u 		= {.aead = {
	 .ivsize 		= 0,	/* IV size for crypto */
	 .maxauthsize		= 28,	/* Max auth data size in bytes */
	 .setkey 		= apm_sec_setkey_ah_sha2,
	 .setauthsize		= apm_sec_setauthsize_aes,
	 .encrypt 		= apm_sec_encrypt_ah_tunnel,
	 .decrypt 		= apm_sec_decrypt_ah_tunnel,
	 }}},
	 {.cra_name 		= "tunnel(ah(hmac(sha256)))",
	 .cra_driver_name 	= "apm-tunnel-ah-hmac-sha256",
	 .cra_flags 		= CRYPTO_ALG_TYPE_AEAD | CRYPTO_ALG_ASYNC,
	 .cra_blocksize 	= 64,	/* MD5-HMAC block size is 512-bits */
	 .cra_ctxsize 		= sizeof(struct apm_sec_session_ctx),
	 .cra_alignmask 	= 0xF,	/* Hardware requires 16 bytes aligned */
	 .cra_type 		= &crypto_aead_type,
	 .cra_u 		= {.aead = {
	 .ivsize 		= 0,	/* IV size for crypto */ 
	 .maxauthsize		= 32,	/* Max auth data size in bytes */
	 .setkey 		= apm_sec_setkey_ah_sha2,
	 .setauthsize		= apm_sec_setauthsize_aes,
	 .encrypt 		= apm_sec_encrypt_ah_tunnel,
	 .decrypt 		= apm_sec_decrypt_ah_tunnel,
	 }}},
	 {.cra_name 		= "tunnel(ah(hmac(sha384)))",
	 .cra_driver_name 	= "apm-tunnel-ah-hmac-sha384",
	 .cra_flags 		= CRYPTO_ALG_TYPE_AEAD | CRYPTO_ALG_ASYNC,
	 .cra_blocksize 	= 64,	/* MD5-HMAC block size is 512-bits */
	 .cra_ctxsize 		= sizeof(struct apm_sec_session_ctx),
	 .cra_alignmask 	= 0xF,	/* Hardware requires 16 bytes aligned */
	 .cra_type 		= &crypto_aead_type,
	 .cra_u 		= {.aead = {
	 .ivsize 		= 0,	/* IV size for crypto */
	 .maxauthsize		= 48,	/* Max auth data size in bytes */
	 .setkey 		= apm_sec_setkey_ah_sha2,
	 .setauthsize		= apm_sec_setauthsize_aes,
	 .encrypt 		= apm_sec_encrypt_ah_tunnel,
	 .decrypt 		= apm_sec_decrypt_ah_tunnel,
	 }}},
	 {.cra_name 		= "tunnel(ah(hmac(sha512)))",
	 .cra_driver_name 	= "apm-tunnel-ah-hmac-sha512",
	 .cra_flags 		= CRYPTO_ALG_TYPE_AEAD | CRYPTO_ALG_ASYNC,
	 .cra_blocksize 	= 64,	/* SHA512-HMAC block size is 512-bits */
	 .cra_ctxsize 		= sizeof(struct apm_sec_session_ctx),
	 .cra_alignmask 	= 0xF,	/* Hardware requires 16 bytes aligned */
	 .cra_type 		= &crypto_aead_type,
	 .cra_u 		= {.aead = {
	 .ivsize 		= 0,	/* IV size for crypto */
	 .maxauthsize		= 64,	/* Max auth data size in bytes */
	 .setkey 		= apm_sec_setkey_ah_sha2,
	 .setauthsize		= apm_sec_setauthsize_aes,
	 .encrypt 		= apm_sec_encrypt_ah_tunnel,
	 .decrypt 		= apm_sec_decrypt_ah_tunnel,
	 }}},
	 {.cra_name 		= "tunnel(ah(xcbc(aes)))",
	 .cra_driver_name 	= "apm-tunnel-ah-xcbc",
	 .cra_flags 		= CRYPTO_ALG_TYPE_AEAD | CRYPTO_ALG_ASYNC,
	 .cra_blocksize 	= 64,	/* XCBC block size is 512-bits */
	 .cra_ctxsize 		= sizeof(struct apm_sec_session_ctx),
	 .cra_alignmask 	= 0xF,	/* Hardware requires 16 bytes aligned */
	 .cra_type 		= &crypto_aead_type,
	 .cra_u 		= {.aead = {
	 .ivsize 		= 0,	/* IV size for crypto */
	 .maxauthsize		= 16,	/* Max auth data size in bytes */
	 .setkey 		= apm_sec_setkey_ah_xcbc,
	 .setauthsize		= apm_sec_setauthsize_aes,
	 .encrypt 		= apm_sec_encrypt_ah_tunnel,
	 .decrypt 		= apm_sec_decrypt_ah_tunnel,
	 }}},
	{.cra_name 		= "authenc(rfc3686(ctr(aes)),hmac(md5))",
	 .cra_driver_name 	= "apm-aes-md5-ipsec",
	 .cra_priority 		= APM_SEC_CRYPTO_PRIORITY_IPSEC,
	 .cra_flags 		= CRYPTO_ALG_TYPE_AEAD | CRYPTO_ALG_ASYNC,
	 .cra_blocksize 	= 16,	/* 128-bits block */
	 .cra_ctxsize 		= sizeof(struct apm_sec_session_ctx),
	 .cra_alignmask 	= 0xF,	/* Hardware requires 16 bytes aligned */
	 .cra_type 		= &crypto_aead_type,
	 .cra_u 		= {.aead = {
	 .ivsize 		= 16,	/* IV size is 16 bytes */
	 .maxauthsize		= 16,	/* Max auth data size in bytes */
	 .setkey 		= apm_sec_setkey_esp_aes_ctr,
	 .setauthsize		= apm_sec_setauthsize_aes,
	 .encrypt 		= apm_sec_encrypt_esp_transport,
	 .decrypt 		= apm_sec_decrypt_esp_transport,
	 }}},
	 

	{.cra_name 		= "authenc(rfc3686(ctr(aes)),hmac(sha1))",
	 .cra_driver_name 	= "apm-aes-md5-ipsec",
	 .cra_priority 		= APM_SEC_CRYPTO_PRIORITY_IPSEC,
	 .cra_flags 		= CRYPTO_ALG_TYPE_AEAD | CRYPTO_ALG_ASYNC,
	 .cra_blocksize 	= 16,	/* 128-bits block */
	 .cra_ctxsize 		= sizeof(struct apm_sec_session_ctx),
	 .cra_alignmask 	= 0xF,	/* Hardware requires 16 bytes aligned */
	 .cra_type 		= &crypto_aead_type,
	 .cra_u 		= {.aead = {
	 .ivsize 		= 16,	/* IV size is 16 bytes */
	 .maxauthsize		= 16,	/* Max auth data size in bytes */
	 .setkey 		= apm_sec_setkey_ctr_aes_sha1_ipsec,
	 .setauthsize		= apm_sec_setauthsize_aes,
	 .encrypt 		= apm_sec_encrypt_ctr_ipsec,
	 .decrypt 		= apm_sec_decrypt_ctr_ipsec,
	 .givencrypt		= apm_sec_givencrypt_ctr_ipsec,
	 }}},

	{.cra_name 		= "authenc(rfc3686(ctr(aes)),hmac(sha256))",
	 .cra_driver_name 	= "apm-aes-md5-ipsec",
	 .cra_priority 		= APM_SEC_CRYPTO_PRIORITY_IPSEC,
	 .cra_flags 		= CRYPTO_ALG_TYPE_AEAD | CRYPTO_ALG_ASYNC,
	 .cra_blocksize 	= 16,	/* 128-bits block */
	 .cra_ctxsize 		= sizeof(struct apm_sec_session_ctx),
	 .cra_alignmask 	= 0xF,	/* Hardware requires 16 bytes aligned */
	 .cra_type 		= &crypto_aead_type,
	 .cra_u 		= {.aead = {
	 .ivsize 		= 16,	/* IV size is 16 bytes */
	 .maxauthsize		= 16,	/* Max auth data size in bytes */
	 .setkey 		= apm_sec_setkey_ctr_aes_sha256_ipsec,
	 .setauthsize		= apm_sec_setauthsize_aes,
	 .encrypt 		= apm_sec_encrypt_ctr_ipsec,
	 .decrypt 		= apm_sec_decrypt_ctr_ipsec,
	 .givencrypt		= apm_sec_givencrypt_ctr_ipsec,
	 }}},
	
	{.cra_name 		= "authenc(rfc3686(ctr(aes)),xcbc(aes))",
	 .cra_driver_name 	= "apm-aes-md5-ipsec",
	 .cra_priority 		= APM_SEC_CRYPTO_PRIORITY_IPSEC,
	 .cra_flags 		= CRYPTO_ALG_TYPE_AEAD | CRYPTO_ALG_ASYNC,
	 .cra_blocksize 	= 16,	/* 128-bits block */
	 .cra_ctxsize 		= sizeof(struct apm_sec_session_ctx),
	 .cra_alignmask 	= 0xF,	/* Hardware requires 16 bytes aligned */
	 .cra_type 		= &crypto_aead_type,
	 .cra_u 		= {.aead = {
	 .ivsize 		= 16,	/* IV size is 16 bytes */
	 .maxauthsize		= 16,	/* Max auth data size in bytes */
	 .setkey 		= apm_sec_setkey_ctr_aes_xcbc_ipsec,
	 .setauthsize		= apm_sec_setauthsize_aes,
	 .encrypt 		= apm_sec_encrypt_ctr_ipsec,
	 .decrypt 		= apm_sec_decrypt_ctr_ipsec,
	 .givencrypt		= apm_sec_givencrypt_ctr_ipsec,
	 }}},

	{.cra_name 		= "macsec(gcm(aes))",
	 .cra_driver_name 	= "apm-macsec",
	 .cra_flags 		= CRYPTO_ALG_TYPE_AEAD | CRYPTO_ALG_ASYNC,
	 .cra_blocksize 	= 16,	/* 128-bits block */
	 .cra_ctxsize 		= sizeof(struct apm_sec_session_ctx),
	 .cra_alignmask 	= 0xF,	/* Hardware requires 16 bytes aligned */
	 .cra_type 		= &crypto_aead_type,
	 .cra_u 		= {.aead = {
	 .ivsize 		= 16,		/* IV size is 16 bytes */
	 .setkey 		= apm_sec_setkey_macsec,
	 .setauthsize		= apm_sec_setauthsize_macsec,
	 .encrypt 		= apm_sec_encrypt_macsec_gcm,
	 .decrypt 		= apm_sec_decrypt_macsec_gcm,
	 }}},
	{.cra_name 		= "macsec(gmac(aes))",
	 .cra_driver_name 	= "apm-macsec",
	 .cra_flags 		= CRYPTO_ALG_TYPE_AEAD | CRYPTO_ALG_ASYNC,
	 .cra_blocksize 	= 16,	/* 128-bits block */
	 .cra_ctxsize 		= sizeof(struct apm_sec_session_ctx),
	 .cra_alignmask 	= 0xF,	/* Hardware requires 16 bytes aligned */
	 .cra_type 		= &crypto_aead_type,
	 .cra_u 		= {.aead = {
	 .ivsize 		= 16,		/* IV size is 16 bytes */
	 .setkey 		= apm_sec_setkey_macsec,
	 .setauthsize		= apm_sec_setauthsize_macsec,
	 .encrypt 		= apm_sec_encrypt_macsec_gmac,
	 .decrypt 		= apm_sec_decrypt_macsec_gmac,
	 }}},
#endif
	/* Terminator */
	{.type = 0 }
};

