/**
 * AppliedMicro AM862xx SoC Security Driver
 *
 * Copyright (c) 2010 Applied Micro Circuits Corporation.
 * All rights reserved. Loc Ho <lho@apm.com>
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
 * @file   apm_sec_sa.c
 *
 * This file implements the security context assoicate format support
 * functions.
 *
 */
#include "apm_sec_csr.h"
#include "apm_sec_priv.h" 
#include "apm_sec_sa.h"
 
/* Table lookup for SA crypto length in byte (Crypto alg) */
unsigned char sec_sa_crypto_key_len_tbl[2][8] = {
	{0, 0, 0, 0, 0, 0, 0, 0},
	{8, 16, 24, 16, 0, 128 / 8, 192 / 8, 256 / 8}
};

/* Table lookup for SA Hash Digest length in byte (Hash alg, digest type) */
unsigned char sec_sa_hash_digest_len_tbl[4][8] = {
	/* Digest type 0 */
	{0, 0, 0, 0, 0, 0, 0, 0},
	/* Digest type 1 */
	{4 * 4, 5 * 4, 5 * 4, 8 * 4, 8 * 4, 16 * 4, 16 * 4, 0},
	/* Digest type 2 */
	{0, 12 * 4, 12 * 4, 12 * 4, 4 * 4, 0, 0, 4 * 4},
	/* Digest type 3 */
	{8 * 4, 10 * 4, 10 * 4, 256/8 * 2, 256/8 * 2, 512/8 * 2, 512/8 * 2, 0},
};

/* Table lookup for SA sequence number length in byte (seq01) */
unsigned char sec_sa_seq_len_tbl[4] = { 0, 4, 8, 8 };

/* Table lookup for SA mask length in byte (mask01) */
unsigned char sec_sa_mask_len_tbl[4] = { 0, 8, 8, 16 };

/* Security context IV length in bytes (iv03 from token of SA) */
unsigned char sec_sa_iv_len_tbl[16] = {
	0, 4, 4, 8,
	4, 8, 8, 12,
	4, 8, 8, 12,
	8, 12, 12, 16
};

unsigned int sec_sa_compute_key_len(unsigned char ca, unsigned char key)
{
	return 	sec_sa_crypto_key_len_tbl[key][ca];
}

unsigned int sec_sa_compute_digest_len(unsigned char ha, unsigned char dt)
{
	return sec_sa_hash_digest_len_tbl[dt][ha];
}

unsigned int sec_sa_compute_seq_len(unsigned char seq)
{
	return sec_sa_seq_len_tbl[seq];
}

unsigned int sec_sa_compute_iv_len(unsigned char iv03)
{
	return sec_sa_iv_len_tbl[iv03];
}

unsigned int sec_sa_compute_mask_len(unsigned char mask)
{
	return sec_sa_mask_len_tbl[mask];
}

unsigned int sec_sa_digest_len(struct sec_sa_ctrl *sa)
{
	return sec_sa_hash_digest_len_tbl[sa->digest_type][sa->hash_alg];
}

unsigned int sec_sa_key_len(struct sec_sa_ctrl *sa)
{
	return 	sec_sa_crypto_key_len_tbl[sa->key][sa->crypto_alg];
}

unsigned int sec_sa_seq_len(struct sec_sa_ctrl *sa)
{
        return sec_sa_seq_len_tbl[sa->seq];
}

unsigned int sec_sa_mask_len(struct sec_sa_ctrl *sa)
{
        return sec_sa_mask_len_tbl[sa->mask];
}

unsigned int sec_sa_iv_len(struct sec_sa_ctrl *sa)
{
        return sec_sa_iv_len_tbl[sa->iv03];
}

int sec_sa_set_aes_ccm_msg_len(unsigned char *block, unsigned int msglen,
				int csize)
{
	unsigned int data;

	memset(block, 0, csize);
	block += csize;

	if (csize >= 4)
		csize = 4;
	else if (msglen > (1 << (8 * csize)))
		return -1;

	data = cpu_to_be32(msglen);
	memcpy(block - csize, (unsigned char *) &data + 4 - csize, csize);

	return 0;
}

int sec_sa_set_aes_ccm_adata(unsigned char *adata, unsigned int a)
{
	int len = 0;

	/* add control info for associated data
	 * RFC 3610 and NIST Special Publication 800-38C
	 */
	if (a < 65280) {
		*(unsigned short *) adata = cpu_to_be16(a);
		len = 2;
	} else  {
		*(unsigned short *) adata   = cpu_to_be16(0xfffe);
		*(unsigned int *) &adata[2] = cpu_to_be32(a);
		*(unsigned short *) &adata[6] = 0;
		len = 6;
	}

	return len;
}

#ifdef APM_SEC_SATKNDEBUG
static void sec_sa_format(unsigned int *sa, int len, char *msg)
{
	int i;
	u8  *sa8 = (u8 *) sa;
		
	for (i = 0; i < len; i++) {
		if (i && (i % 4) == 0) {
			sprintf(msg, " %02X", sa8[i]);
			msg += 3;
		} else {
			sprintf(msg, "%02X", sa8[i]);
			msg += 2;
		}		
	}
	*msg = '\0';
}

void sec_sa_dump(unsigned int *sa, unsigned int sa_len)
{
	struct sec_sa_ctrl *sa_ctrl = (struct sec_sa_ctrl *) sa;
	int	i;
	int	len;
	char    msg[300];
		
	APMSEC_SATKNLOG("SA: 0x%p", sa);
	APMSEC_SATKNLOG("  Mask: %d (0x%02X) Seq: %d (0x%02X)", 
		sa_ctrl->mask, sa_ctrl->mask, sa_ctrl->seq, sa_ctrl->seq);
    	APMSEC_SATKNLOG("   SPI: %d", sa_ctrl->spi);
    	APMSEC_SATKNLOG("  Hash: %d-%d", 
    		sa_ctrl->hash_alg, sa_ctrl->digest_type);
    	APMSEC_SATKNLOG("Crypto: %d-%d", 
    		sa_ctrl->crypto_alg, sa_ctrl->cryptomode_feedback);
    	APMSEC_SATKNLOG("   ToP: %d", sa_ctrl->ToP);
    	APMSEC_SATKNLOG("IV Fmt: %d IV: %d%d%d%db", sa_ctrl->iv_format,
    		sa_ctrl->iv03 & 0x8 ? 1: 0, sa_ctrl->iv03 & 0x4 ? 1: 0,
		sa_ctrl->iv03 & 0x2 ? 1: 0, sa_ctrl->iv03 & 0x1 ? 1: 0);
	APMSEC_SATKNLOG("crypto/Hash store: %d/%d", 
		sa_ctrl->crypto_store, sa_ctrl->hash_store); 
	APMSEC_SATKNLOG("         enc hash: %d", sa_ctrl->enc_hash_result);
	APMSEC_SATKNLOG(" state select/i-j: %d/%d", 
		sa_ctrl->state_selection, sa_ctrl->ij_pointer);
	i = 2;
    	len = sec_sa_compute_key_len(sa_ctrl->crypto_alg, sa_ctrl->key); 
	if (len) {
		sec_sa_format(&sa[i], len, msg);
		i += len >> 2;
       		APMSEC_SATKNLOG("Key 0x%s", msg);
	}
    	switch (sa_ctrl->digest_type) {
	case SA_DIGEST_TYPE_INNER:
		len = sec_sa_compute_digest_len(sa_ctrl->hash_alg, 
					  	sa_ctrl->digest_type);
		sec_sa_format(&sa[i], len, msg);
		i += len >> 2;
       		APMSEC_SATKNLOG("Digest 0x%s", msg);
		break;
	case SA_DIGEST_TYPE_HMAC:
		len = sec_sa_compute_digest_len(sa_ctrl->hash_alg, 
					  	sa_ctrl->digest_type);
		sec_sa_format(&sa[i], len / 2, msg);
		i += (len >> 2) / 2;
      		APMSEC_SATKNLOG("Digest0 0x%s", msg);
		sec_sa_format(&sa[i], len / 2, msg);
		i += (len >> 2) / 2;
      		APMSEC_SATKNLOG("Digest1 0x%s", msg);
		break;
	case SA_DIGEST_TYPE_HASH:
        	switch (sa_ctrl->hash_alg) {
		case SA_HASH_ALG_XCBC128:
		case SA_HASH_ALG_XCBC192:
		case SA_HASH_ALG_XCBC256:
			len = sec_sa_compute_digest_len(sa_ctrl->hash_alg, 
							sa_ctrl->digest_type);
			sec_sa_format(&sa[i], len / 3, msg);
			APMSEC_SATKNLOG("Hash Key0 0x%s", msg);
			i += (len >> 2) / 3;
			sec_sa_format(&sa[i], len / 3, msg);
			APMSEC_SATKNLOG("Hash Key1 0x%s", msg);
			i += (len >> 2) / 3;
			sec_sa_format(&sa[i], len / 3, msg);
			APMSEC_SATKNLOG("Hash Key2 0x%s", msg);
			i += (len >> 2) / 3;
			break;
		case SA_HASH_ALG_GHASH:
		case SA_HASH_ALG_KASUMI_F9:
			len = sec_sa_compute_digest_len(sa_ctrl->hash_alg, 
							sa_ctrl->digest_type);
			sec_sa_format(&sa[i], len, msg);
			i += len >> 2;
			APMSEC_SATKNLOG("Digest0 0x%s", msg);
			break;
		}
		break;
	}
	if (sa_ctrl->spi) {
		sec_sa_format(&sa[i], 4, msg);
		i++;
		APMSEC_SATKNLOG("    SPI: 0x%s", msg);
	}
	if (sa_ctrl->seq) {
		len = sec_sa_compute_seq_len(sa_ctrl->seq);
		sec_sa_format(&sa[i], len, msg);
		i += len >> 2;
		APMSEC_SATKNLOG("SEQ 0x%s", msg);
	}
	if (sa_ctrl->mask) {
		len = sec_sa_compute_mask_len(sa_ctrl->mask);
		sec_sa_format(&sa[i], len, msg);
		i += len >> 2;
		APMSEC_SATKNLOG("MASK 0x%s", msg);
	}
	if (sa_ctrl->iv03) {
		len = sec_sa_compute_iv_len(sa_ctrl->iv03);
		sec_sa_format(&sa[i], len, msg);
		i += len >> 2;
		APMSEC_SATKNLOG(" IV 0x%s", msg);
	}
	if (sa_ctrl->crypto_alg == SA_CRYPTO_ALG_ARC4 &&
	    sa_ctrl->state_selection) {
		APMSEC_SATKNLOG(" ARC4 ptr 0x%08X", sa[i++]);
		APMSEC_SATKNLOG(" ARC4 i-j 0x%08X", sa[i++]);
	}
	APMSEC_DEBUG_DUMP("SA RAW: ", (void *) sa, sa_len);
    	if (i << 2 != sa_len)
        	APMSEC_SATKNLOG("SA length mismatch %d != %d (actual)", 
        		sa_len, i << 2);
}

void sec_sa_dumphw(void)
{
	u32 val;

	int i;

	for (i = 0; i < 4; i++) {
		apm_sec_rd32(EIP96_CORE_CSR_BLOCK, IPE_IV0_ADDR + i * 4, &val);
		APMSEC_SATKNLOG("HW IV[%d] 0x%08X", i, val); 
	}
#if 1	
	for (i = 0; i < 8; i++) {
		apm_sec_rd32(EIP96_CORE_CSR_BLOCK, IPE_KEY0_ADDR + i * 4, 
				&val);
		APMSEC_SATKNLOG("HW Key[%d] 0x%08X", i, val); 
	}
	for (i = 0; i < 8; i++) {
		apm_sec_rd32(EIP96_CORE_CSR_BLOCK, IPE_IN_DGST0_ADDR + i * 4, 
				&val);
		APMSEC_SATKNLOG("HW Inner Digest[%d] 0x%08X", i, val); 
	}
	for (i = 0; i < 8; i++) {
		apm_sec_rd32(EIP96_CORE_CSR_BLOCK, IPE_IN_DGST8_ADDR + i * 4, 
				&val);
		APMSEC_SATKNLOG("HW Inner Digest[%d] 0x%08X", i + 8, val); 
	}
	for (i = 0; i < 8; i++) {
		apm_sec_rd32(EIP96_CORE_CSR_BLOCK, IPE_OUT_DGST0_ADDR + i * 4, 
				&val);
		APMSEC_SATKNLOG("HW Outer Digest[%d] 0x%08X", i, val); 
	}
	for (i = 0; i < 8; i++) {
		apm_sec_rd32(EIP96_CORE_CSR_BLOCK, IPE_OUT_DGST8_ADDR + i * 4, 
				&val);
		APMSEC_SATKNLOG("HW Outer Digest[%d] 0x%08X", i + 8, val); 
	}
	for (i = 0; i < 8; i++) {
		apm_sec_rd32(EIP96_CORE_CSR_BLOCK, IPE_HASH_RES0_ADDR + i * 4, 
				&val);
		APMSEC_SATKNLOG("HW Hash Result[%d] 0x%08X", i, val);
	}
#endif
	apm_sec_rd32(EIP96_CORE_CSR_BLOCK, IPE_SPI_SSRC_ADDR, &val);
	APMSEC_SATKNLOG("SPI Number 0x%08X", val);
	
	apm_sec_rd32(EIP96_CORE_CSR_BLOCK, IPE_SN_ADDR , &val);
	APMSEC_SATKNLOG("Seq Number 0x%08X", val);

	apm_sec_rd32(EIP96_CORE_CSR_BLOCK, IPE_NXT_SPI_SSRC_ADDR, &val);
	APMSEC_SATKNLOG("SPI Number 0x%08X", val);
	
	apm_sec_rd32(EIP96_CORE_CSR_BLOCK, IPE_NXT_SN_ADDR , &val);
	APMSEC_SATKNLOG("Seq Number 0x%08X", val);
	
}
#endif
