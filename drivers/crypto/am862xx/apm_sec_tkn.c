/**
 * AppliedMicro APM862xx SoC Crypto Driver
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
 * @file   apm_sec_tkn.c
 *
 * This file implements the security token for common operation such as 
 * for ESP, AH, SRTP, simple hash, simple encrypt, simple decrypt, 
 * Kasumi F9, ARC4, CTS L3 ESP, SSL, DTLS, MACsec, and RTCP. 
 * All samples are taken from SafeNet User Manual v1.2. 
 *
 * A note about endianess - Big endian and little endian token doesn't mix
 * well. You should process in the endianess of you processor and then
 * call the appropriate functions to swap the entire 32-bit value.
 *
 */
#include "apm_sec_tkn.h"
#include "apm_sec_sa.h"

/**
 *
 * Basic Encrypt/Decrypt Token Template
 *
 * Table lookup for Basic IV fields:
 *	[crypto alg][crypto mode][token IV, context IV format, context IV03]
 */
static unsigned char basic_iv_map_tbl[8][8][3] = {
{			/* SA_CRYPTO_ALG_DES */
	{ /* SA_CRYPTO_MODE_AES_3DES_ECB - all zero */ },
	{ /* SA_CRYPTO_MODE_AES_3DES_CBC */ 6, 0, 0x3},
	{ /* SA_CRYPTO_MODE_AES_3DES_CTR - all zero */ },
	{ /* SA_CRYPTO_MODE_AES_3DES_ICM - all zero */ },
	{ /* SA_CRYPTO_MODE_AES_3DES_OFB - */6, 0, 0x3  },
	{ /* SA_CRYPTO_MODE_AES_3DES_CFB - */ 6, 0, 0x3},
	{ /* SA_CRYPTO_MODE_AES_3DES_CTR_LOAD_REUSE - all zero */ },
	{ /* SA_CRYPTO_MODE_AES_3DES_ICM_LOAD_REUSE - all zero */ },
},
{			/* SA_CRYPTO_ALG_ARC4 */
	{ /* SA_CRYPTO_MODE_AES_3DES_ECB - all zero */ },
	{ /* SA_CRYPTO_MODE_AES_3DES_CBC - all zero */ },
	{ /* SA_CRYPTO_MODE_AES_3DES_CTR - all zero */ },
	{ /* SA_CRYPTO_MODE_AES_3DES_ICM - all zero */ },
	{ /* SA_CRYPTO_MODE_AES_3DES_OFB - all zero */ },
	{ /* SA_CRYPTO_MODE_AES_3DES_CFB - all zero */ },
	{ /* SA_CRYPTO_MODE_AES_3DES_CTR_LOAD_REUSE - all zero */ },
	{ /* SA_CRYPTO_MODE_AES_3DES_ICM_LOAD_REUSE - all zero */ },
},
{			/* SA_CRYPTO_ALG_3DES */
	{ /* SA_CRYPTO_MODE_AES_3DES_ECB - all zero */ },
	{ /* SA_CRYPTO_MODE_AES_3DES_CBC */ 6, 0, 0x3},
	{ /* SA_CRYPTO_MODE_AES_3DES_CTR - all zero */ },
	{ /* SA_CRYPTO_MODE_AES_3DES_ICM - all zero */ },
	{ /* SA_CRYPTO_MODE_AES_3DES_OFB -*/6, 0, 0x3 },
	{ /* SA_CRYPTO_MODE_AES_3DES_CFB -*/6, 0, 0x3 },
	{ /* SA_CRYPTO_MODE_AES_3DES_CTR_LOAD_REUSE - all zero */ },
	{ /* SA_CRYPTO_MODE_AES_3DES_ICM_LOAD_REUSE - all zero */ },
},
{			/* SA_CRYPTO_ALG_KASUMI_F8 */
	{ /* KASUMI    */ 6, 0, 0 },
	{ /* KASUMI F8 */ 6, 0, 0 },
	{ /* Reserved - all zero */ },
	{ /* Reserved - all zero */ },
	{ /* Reserved - all zero */ },
	{ /* Reserved - all zero */ },
	{ /* Reserved - all zero */ },
	{ /* Reserved - all zero */ },
},
{			/* SA_CRYPTO_ALG_RSVD */
	{ /* SA_CRYPTO_MODE_AES_3DES_ECB - all zero */ },
	{ /* SA_CRYPTO_MODE_AES_3DES_CBC - all zero */ },
	{ /* SA_CRYPTO_MODE_AES_3DES_CTR - all zero */ },
	{ /* SA_CRYPTO_MODE_AES_3DES_ICM - all zero */ },
	{ /* SA_CRYPTO_MODE_AES_3DES_OFB - all zero */ },
	{ /* SA_CRYPTO_MODE_AES_3DES_CFB - all zero */ },
	{ /* SA_CRYPTO_MODE_AES_3DES_CTR_LOAD_REUSE - all zero */ },
	{ /* SA_CRYPTO_MODE_AES_3DES_ICM_LOAD_REUSE - all zero */ },
},
{			/* SA_CRYPTO_ALG_AES128 */
	{ /* SA_CRYPTO_MODE_AES_3DES_ECB - all zero */ },
	{ /* SA_CRYPTO_MODE_AES_3DES_CBC */ 7, 0, 0xF},
	{ /* SA_CRYPTO_MODE_AES_3DES_CTR */ 7, 0, 0xF},
	{ /* SA_CRYPTO_MODE_AES_3DES_ICM */ 7, 0, 0xF},
	{ /* SA_CRYPTO_MODE_AES_3DES_OFB - */7, 0, 0xF  },
	{ /* SA_CRYPTO_MODE_AES_3DES_CFB - */7, 0, 0xF },
	{ /* SA_CRYPTO_MODE_AES_3DES_CTR_LOAD_REUSE */ 7, 0, 0xF},
	{ /* SA_CRYPTO_MODE_AES_3DES_ICM_LOAD_REUSE */ 7, 0, 0xF}
},
{			/* SA_CRYPTO_ALG_AES192 */
	{ /* SA_CRYPTO_MODE_AES_3DES_ECB - all zero */ },
	{ /* SA_CRYPTO_MODE_AES_3DES_CBC */ 7, 0, 0xF},
	{ /* SA_CRYPTO_MODE_AES_3DES_CTR */ 7, 0, 0xF},
	{ /* SA_CRYPTO_MODE_AES_3DES_ICM */ 7, 0, 0xF},
	{ /* SA_CRYPTO_MODE_AES_3DES_OFB - */6, 0, 0xF },
	{ /* SA_CRYPTO_MODE_AES_3DES_CFB - */6, 0, 0xF },
	{ /* SA_CRYPTO_MODE_AES_3DES_CTR_LOAD_REUSE */ 7, 0, 0xF},
	{ /* SA_CRYPTO_MODE_AES_3DES_ICM_LOAD_REUSE */ 7, 0, 0xF}
},
{			/* SA_CRYPTO_ALG_AES256 */
	{ /* SA_CRYPTO_MODE_AES_3DES_ECB - all zero */ },
	{ /* SA_CRYPTO_MODE_AES_3DES_CBC */ 7, 0, 0xF},
	{ /* SA_CRYPTO_MODE_AES_3DES_CTR */ 7, 0, 0xF},
	{ /* SA_CRYPTO_MODE_AES_3DES_ICM */ 7, 0, 0xF},
	{ /* SA_CRYPTO_MODE_AES_3DES_OFB - */6, 0, 0xF },
	{ /* SA_CRYPTO_MODE_AES_3DES_CFB - */6, 0, 0xF},
	{ /* SA_CRYPTO_MODE_AES_3DES_CTR_LOAD_REUSE */ 7, 0, 0xF},
	{ /* SA_CRYPTO_MODE_AES_3DES_ICM_LOAD_REUSE */ 7, 0, 0xF}
}
};

#ifdef APM_SEC_SATKNDEBUG
void sec_tkn_dump(struct sec_tkn_ctx *tkn)
{
	APMSEC_SATKNLOG("Tkn: 0x%p", TKN_CTX_INPUT_TKN(tkn));
	APMSEC_DEBUG_DUMP("Tkn Raw:", (void *) TKN_CTX_INPUT_TKN(tkn), 
			tkn->input_tkn_len);
}
#endif

u16 sec_tkn_set_hash(u32 *t, u32 pl, u32 *ctrl, u32 sa, u16 ds)
{
	u16 tl;

	/* Create token header */
#if defined (CONFIG_APM862xx)
	sec_tkn_set_hdr(t,tl, 0,0,1,0,0,0,pl,0,8 /* token WORD */, SA_PTR_HW(sa))
#else
	sec_tkn_set_hdr(t,tl, 0,0,1,0,0,0,pl,0,8 /* token WORD */, sa ? SA_PTR_HW(sa) : NULL)
#endif
	/* Add control control WORD0 & WORD1 */
	sec_tkn_set32le(&t[tl++], ctrl[0]);
        sec_tkn_set32le(&t[tl++], ctrl[1]);
	/* Create instructions */
	sec_tkn_set32le(&t[tl++], 0x02920000 | pl);
	sec_tkn_set32le(&t[tl++], 0x21E60000 | ds);
	return tl << 2;
}

void sec_tkn_update_hash(u32 *t, u32 pl)
{
	sec_tkn_update_hdr(t, 0, 0, 1, 0, 0, 0, pl);
	sec_tkn_set32le(&t[sec_tkn_1st_instr_idx], 0x02920000 | pl);
}
inline unsigned short sec_tkn_set_crypt(unsigned int *t, unsigned int pl,
			unsigned int sa, unsigned int  *ctrl, unsigned char ca,
			unsigned char cm, unsigned int  *nonce, 
			unsigned char *iv, int enc, unsigned int meraki_mode)
{
	unsigned short tl = 4;		/* Token length in byte */
	unsigned short tlen;

	/* Create context control WORD0 and WORD1 */
	sec_tkn_set32le(&t[tl++], ctrl[0] | (enc ?
			SA_TOP_CRYPTO_OUTBOUND : SA_TOP_CRYPTO_INBOUND));
	sec_tkn_set32le(&t[tl++], ctrl[1]);
	/* Create instructions */
	if (cm == SA_CRYPTO_MODE_AES_3DES_CBC 
		/* || cm == SA_CRYPTO_MODE_KASUMI_F8 */) {
		int ivs = ca == SA_CRYPTO_ALG_DES ||
			  ca == SA_CRYPTO_ALG_3DES ||
			  ca == SA_CRYPTO_ALG_KASUMI_F8 ? 8 : 16;
		sec_tkn_setbe(&t[tl], iv, ivs); /* IV is in byte stream */
		tl += ivs >> 2;
	} else if (cm == SA_CRYPTO_MODE_AES_3DES_CTR ||
			cm == SA_CRYPTO_MODE_AES_3DES_CTR_LOAD_REUSE) {
		int ivs = 8;
		sec_tkn_setbe(&t[tl++],  (unsigned char *)nonce, 4);
		sec_tkn_setbe(&t[tl], iv, ivs); /* IV is in byte stream */
		tl += ivs >> 2;
		sec_tkn_set32be(&t[tl++], !meraki_mode);	  /* IV is in byte stream */
	}
	sec_tkn_set32le(&t[tl++], 0x0D960000 | pl);
	/* Create token header */
	sec_tkn_set_hdr(t, tlen, 0, basic_iv_map_tbl[ca][cm][0],
			1, 0, 0, 0, pl, 0, tl, SA_PTR_HW(sa));
	return tl << 2;
}

inline void sec_tkn_update_crypt(unsigned int *t,
				     unsigned int pl,
				     unsigned int  *ctrl,
				     unsigned char ca,
				     unsigned char cm,
				     unsigned char *iv, int enc)
{
	sec_tkn_update_hdr(t, 0, basic_iv_map_tbl[ca][cm][0], 1, 0, 0, 0, pl);
	
	sec_tkn_set32le(&t[sec_tkn_ctrl0_idx], 
			 ctrl[0] | (enc ? SA_TOP_CRYPTO_OUTBOUND
					: SA_TOP_CRYPTO_INBOUND));
	
	if (cm == SA_CRYPTO_MODE_AES_3DES_CBC 
		/* || cm == SA_CRYPTO_MODE_KASUMI_F8 */) {
		int ivs = ca == SA_CRYPTO_ALG_DES ||
			  ca == SA_CRYPTO_ALG_3DES ||
			  ca == SA_CRYPTO_ALG_KASUMI_F8 ? 8 : 16;
		sec_tkn_setbe(&t[sec_tkn_1st_instr_idx], iv, ivs);
		sec_tkn_set32le(&t[sec_tkn_1st_instr_idx + (ivs >> 2)],
			0x0D960000 | pl);
	} else if (cm == SA_CRYPTO_MODE_AES_3DES_CTR ||
		   cm == SA_CRYPTO_MODE_AES_3DES_CTR_LOAD_REUSE) {
		int ivs = 8;
		sec_tkn_setbe(&t[sec_tkn_1st_instr_idx + 1], iv, ivs);
		sec_tkn_set32le(&t[sec_tkn_1st_instr_idx + 2 + (ivs >> 2)],
			0x0D960000 | pl);
	} else {
		sec_tkn_set32le(&t[sec_tkn_1st_instr_idx], 0x0D960000 | pl);
	}
}

unsigned short sec_tkn_set_crypt_aes_gcm(unsigned int *t,
				unsigned int pl, unsigned int sa, unsigned int *ctrl,
				unsigned char *adata, unsigned char assoclen,
				unsigned char icv_len, int enc)
{
	unsigned short tl = 4;		/* Length of token in DWORD */
	unsigned short tlen;

	/* SA includes control word, key, GHash key, SPI, SEQ, and IV03 */
	sec_tkn_set32le(&t[tl++], ctrl[0] | (enc ?
			SA_TOP_ENCRYPT_HASH_OUTBOUND :
					SA_TOP_HASH_DECRYPT_INBOUND));
	sec_tkn_set32le(&t[tl++], ctrl[1]);
	/* Insert Adata to hash */
	if (enc) {
		if (assoclen) {
			sec_tkn_set32le(&t[tl++], 0x2AD80000 | assoclen);
			sec_tkn_setbe(&t[tl], adata, assoclen);
			tl += assoclen >> 2;
		}
	} else {
		if (assoclen) {
			sec_tkn_set32le(&t[tl++], 0x2AD80000 | assoclen);
			sec_tkn_setbe(&t[tl], adata, assoclen);
			tl += assoclen >> 2;
		} else {
			sec_tkn_set32le(&t[tl++], 0x0B000000);
		}
	}
	/* Schedule remove of 16 bytes of encrypted ICV */
	sec_tkn_set32le(&t[tl++], 0xA0800000);
	/* Insert 16 bytes of 0 to CRYPTO for remove by above instr */
	sec_tkn_set32le(&t[tl++], 0x25000010);
	/* Move payload to CRYPTO, HASH, and OUTPUT */
	if (enc) {
		sec_tkn_set32le(&t[tl++], 0x0F020000 | pl);
		/* Append hash result to OUTPUT */
		sec_tkn_set32le(&t[tl++], 0x21E60000 | icv_len);
	} else {
		/* Move payload to CRYPTO, HASH, and OUTPUT */
		sec_tkn_set32le(&t[tl++], 0x0F020000 | (pl - icv_len));
		/* Retrieve ICV from input for verification */
		sec_tkn_set32le(&t[tl++], 0x40E60000 | icv_len);
		/* Verification ICV FIXME*/
		/* Verification of ICV fails for GCM.. This is a bug and needs to be fixed */
		sec_tkn_set32le(&t[tl++], 0xD0070000 | icv_len);
	}
	/* Create token header */
	sec_tkn_set_hdr(t, tlen, 0, 0, 1, 0, 0, 0, pl, 0, tl, SA_PTR_HW(sa))

	return tl << 2;
}
		
unsigned short sec_tkn_update_crypt_aes_gcm(unsigned int *t,
		unsigned int pl,
		unsigned int sa,
		unsigned int  *ctrl,
		unsigned char *adata, unsigned char assoclen,
		unsigned char icv_len, int enc)
{
	unsigned short tl;		/* Length of token in DWORD */
	
	/* Update header */
	sec_tkn_update_hdr(t, 0, 0, 1, 0, 0, 0, pl);
	sec_tkn_set32le(&t[sec_tkn_sa_ptr_idx], (u32)(SA_PTR_HW(sa)));
	sec_tkn_set32le(&t[sec_tkn_ctrl0_idx],
			 ctrl[0] | (enc ? SA_TOP_ENCRYPT_HASH_OUTBOUND:
					 SA_TOP_HASH_DECRYPT_INBOUND));
	/* Insert Adata to hash */
	tl = sec_tkn_1st_instr_idx;
	if (enc) {
		if (assoclen) {
			sec_tkn_set32le(&t[tl++], 0x2AD80000 | assoclen);
			sec_tkn_setbe(&t[tl], adata, assoclen);
			tl += assoclen >> 2;
		}
	} else {
		if (assoclen) {
			sec_tkn_set32le(&t[tl++], 0x2AD80000 | assoclen);
			sec_tkn_setbe(&t[tl], adata, assoclen);
			tl += assoclen >> 2;
			tl += assoclen >> 2;
		} else {
			sec_tkn_set32le(&t[tl++], 0x0B000000);
		}
	}
	/* Schedule remove of 16 bytes of encrypted ICV */
	tl++;
	tl++;
	/* Move payload to CRYPTO, HASH, and OUTPUT */
	if (enc) {
		sec_tkn_set32le(&t[tl++], 0x0F020000 | pl);
		/* Append hash result to OUTPUT */
		sec_tkn_set32le(&t[tl++], 0x21E60000 | icv_len);
	} else {
		/* Move payload to CRYPTO, HASH, and OUTPUT */
		sec_tkn_set32le(&t[tl++], 0x0F020000 | (pl - icv_len));
		/* Retrieve ICV for verification */
		sec_tkn_set32le(&t[tl++], 0x40E60000 | icv_len);
		/* Verification ICV */
		sec_tkn_set32le(&t[tl++], 0xD0070000 | icv_len);
	}
	return tl << 2;
}

unsigned short sec_tkn_set_decrypt(unsigned int *t, unsigned int pl,
		unsigned int sa, unsigned int *ctrl, unsigned char ca,
		unsigned char cm, unsigned int *nonce, unsigned char *iv)
{
	/* FIXME */
	return 0;
}

void sec_tkn_update_decrypt(unsigned int *t, unsigned int pl,
	        unsigned int *ctrl, unsigned char ca, unsigned char cm,
		unsigned char *iv)
{
	/* FIXME */
}

/**
 *
 * AES-CCM Basic Algorithm Token Templates
 *
 */
unsigned short sec_tkn_set_crypt_aes_ccm(unsigned int *t, unsigned int pl,
		unsigned int sa, unsigned int *ctrl, unsigned int icv_size,
		unsigned int *iv_B0, unsigned char *adata_length,
		unsigned int adata_length_length, unsigned char *adata,
		unsigned char assoclen, int enc)
{
	unsigned short tl = 4;		/* Length of token in DWORD */
	unsigned short tlen;
	int hash_pad;

	/* SA includes control word, key, digest key, and IV */
	sec_tkn_set32le(&t[tl++], ctrl[0] | (enc ? SA_TOP_HASH_ENCRYPT_OUTBOUND :
			SA_TOP_DECRYPT_HASH_INBOUND));
	sec_tkn_set32le(&t[tl++], ctrl[1]);
	/* Insert B0 Vector + adata length field, adata from token to hash */
	sec_tkn_set32le(&t[tl++], 0x22D80000 | (16 + adata_length_length));
	sec_tkn_setbe(&t[tl], (u8 *) iv_B0, 16);
	tl += 4;
	/* Insert adata length field from Token to hash */
	if (adata_length_length == 2) {
		sec_tkn_set32be(&t[tl++], ((u32) *(u16 *) adata_length) << 16);
	} else {
		sec_tkn_set32be(&t[tl++], *(u32 *) adata_length);
		sec_tkn_set32be(&t[tl++],
				   ((u32) *(u16 *) (adata_length + 4)) << 16);
	}
	/* Insert adata to hash from token */
	sec_tkn_set32le(&t[tl++], 0x22D80000 | assoclen);
	sec_tkn_setbe(&t[tl], adata, assoclen);
	tl += assoclen >> 2;
	/* Padding to Hash Engine */
	hash_pad = 16 - ((adata_length_length + assoclen) % 16);
	sec_tkn_set32le(&t[tl++], 0x22000000 | hash_pad);
	/* Schedule remove of S0 at offset of 0 from output */
	sec_tkn_set32le(&t[tl++], 0xA0820000);
	/* Insert block of zero to generate S0 vector */
	sec_tkn_set32le(&t[tl++], 0x25000010);
	/* Pad hash to block size */
	hash_pad = 16 - ((enc ? pl : (pl - icv_size)) % 16);
	if (hash_pad) {
		/* Move payload to CRYPTO, HASH, and OUTPUT */
		sec_tkn_set32le(&t[tl++], 0x0F000000 | (enc ? pl : (pl - icv_size)));
		/* Move paddin to HASH */
		sec_tkn_set32le(&t[tl++], 0x22020000 | hash_pad);
	} else {
		/* Move payload to CRYPTO, HASH, and OUTPUT */
		sec_tkn_set32le(&t[tl++], 0x0F020000 | (enc ? pl : (pl - icv_size)));
	}
	/* Insert ICV to OUTPUT */
	if (enc) {
		sec_tkn_set32le(&t[tl++], 0x21E60000 | icv_size);
	}
	else {
		/* Retrieve ICV for verification */
		sec_tkn_set32le(&t[tl++], 0x40E60000 | icv_size);
		/* Verification ICV */
		sec_tkn_set32le(&t[tl++], 0xD0070000 | icv_size);
	}
	sec_tkn_set_hdr(t, tlen, 0, 0, 1, 0, 0, 0, pl, 0, tl, SA_PTR_HW(sa))
			
	return tl << 2;			/* Convert to byte unit */
}

unsigned short sec_tkn_update_crypt_aes_ccm(unsigned int *t,
		unsigned int pl, unsigned int sa, unsigned int *ctrl,
		unsigned int icv_size, unsigned int *iv_B0,
		unsigned char *adata_length, unsigned int adata_length_length,
		unsigned char *adata, unsigned char assoclen, int enc)
{
	unsigned short tl;		/* Length of token in DWORD */
	int 	hash_pad;
	
	/* Update header */
	sec_tkn_update_hdr(t, 0, 0, 1, 0, 0, 0, pl);
	sec_tkn_set32le(&t[sec_tkn_sa_ptr_idx], (u32)SA_PTR_HW(sa));
	sec_tkn_set32le(&t[sec_tkn_ctrl0_idx],
			   ctrl[0] | (enc ? SA_TOP_ENCRYPT_HASH_OUTBOUND:
					   SA_TOP_DECRYPT_HASH_INBOUND));
	tl = sec_tkn_1st_instr_idx;
	/* Insert B0 Vector + adata length field, adata from token to hash */
	sec_tkn_set32le(&t[tl++], 0x22D80000 | (16 + adata_length_length));
	sec_tkn_setbe(&t[tl], (u8 *) iv_B0, 16);
	tl += 4;
	/* Insert adata length field from Token to hash */
	if (adata_length_length == 2) {
		sec_tkn_set32be(&t[tl++], ((u32) *(u16 *) adata_length) << 16);
	} else {
		sec_tkn_set32be(&t[tl++], *(u32 *) adata_length);
		sec_tkn_set32be(&t[tl++],
				   ((u32) *(u16 *) (adata_length + 4)) << 16);
	}
	/* Insert adata to hash from token */
	sec_tkn_set32le(&t[tl++], 0x22D80000 | assoclen);
	sec_tkn_setbe(&t[tl], adata, assoclen);
	tl += assoclen >> 2;
	/* Padding to Hash Engine */
	hash_pad = 16 - ((adata_length_length + assoclen) % 16);
	sec_tkn_set32le(&t[tl++], 0x22000000 | hash_pad);
	/* Schedule remove of S0 at offset of 0 from output */
	sec_tkn_set32le(&t[tl++], 0xA0820000);
	/* Insert block of zero to generate S0 vector */
	sec_tkn_set32le(&t[tl++], 0x25000010);
	/* Move payload to CRYPTO, HASH, and OUTPUT */
	sec_tkn_set32le(&t[tl++], 0x0F000000 | (enc ? pl : (pl - icv_size)));
	/* Pad hash to block size */
	hash_pad = 16 - ((enc ? pl : (pl - icv_size)) % 16);
	if (hash_pad) {
		/* Move payload to CRYPTO, HASH, and OUTPUT */
		sec_tkn_set32le(&t[tl++], 0x0F000000 | (enc ? pl : (pl - icv_size)));
		/* Move paddin to HASH */
		sec_tkn_set32le(&t[tl++], 0x22020000 | hash_pad);
	} else {
		/* Move payload to CRYPTO, HASH, and OUTPUT */
		sec_tkn_set32le(&t[tl++], 0x0F020000 | (enc ? pl : (pl - icv_size)));
	}
	if (enc) {
		/* Insert ICV to OUTPUT */
		sec_tkn_set32le(&t[tl++], 0x21E60000 | icv_size);
	} else {
		/* Retrieve ICV for verification */
		sec_tkn_set32le(&t[tl++], 0x40E60000 | icv_size);
		/* Verification ICV */
		sec_tkn_set32le(&t[tl++], 0xD0070000 | icv_size);
	}

	return tl << 2;			/* Convert to byte unit */
}

/**
 * ARC4 Basic Algorithm Token Creation and Update Routines
 *
 */
unsigned short sec_tkn_set_arc4_crypt(unsigned int *t, unsigned int pl,
				unsigned int sa, unsigned int *ctrl, int enc)
{
	unsigned short tl = 4;
	unsigned short tlen;
	/* Create token header */
	
	/* Create context control WORD0 and WORD1 */
	sec_tkn_set32le(&t[tl++], ctrl[0] | (enc ? SA_TOP_CRYPTO_OUTBOUND :
			SA_TOP_CRYPTO_INBOUND));
	sec_tkn_set32le(&t[tl++], ctrl[1]);
	/* Move data to crypto from input */
	sec_tkn_set32le(&t[tl++], 0x0D960000 | pl);
	if (ctrl[1] & 0x00100000) {
		/* statefull */
		/* Write back ARC4 state to context */
		sec_tkn_set32le(&t[tl++], 0xE1D61800);
		/* Write back ARC4 i-j pointer to context */
		sec_tkn_set32le(&t[tl++], 0xE1CE1800 | 8);
	}
	sec_tkn_set_hdr(t, tlen, 0, 0, 1, 0, 0x3, 0, pl, 0, tl, SA_PTR_HW(sa))
	return tl << 2;
}

void sec_tkn_update_arc4(unsigned int *t, unsigned int pl)
{	/* Update header for new payload length */
	sec_tkn_update_hdr(t, 0, 0, 1, 0, 0x3, 0, pl);
	/* Update move data to crypto from input */
	sec_tkn_set32le(&t[sec_tkn_1st_instr_idx], 0x0D960000 | pl);
}

/**
 * ESP Token Templates in Tunnel Mode
 *
 */
inline unsigned short sec_tkn_set_espout_tunnel(unsigned int *t,
		unsigned int pl, unsigned int sa, unsigned int *ctrl,
		unsigned char ca, unsigned char cm, unsigned char pdl,
		unsigned char gcm_flag, unsigned char seq_offset)
{
	unsigned short tl = 4;	/* Length of token in DWORD */
	unsigned short tlen;
	unsigned char  ivsize = (ca == SA_CRYPTO_ALG_DES ||
			ca == SA_CRYPTO_ALG_3DES) ? 8 : 16;
	int oiphl = 20;
	int ipv6 = 0;
	int ethl = 0;
	/* Create context control WORD0 and WORD1 */
	sec_tkn_set32le(&t[tl++], ctrl[0]);
	sec_tkn_set32le(&t[tl++], ctrl[1]);

	/* Create token instructions - Process ESP header */
	switch (cm) {
		case SA_CRYPTO_MODE_AES_3DES_CBC:
		/* Insert ESP header to HASH and OUTPUT - SPI, SEQ, and ICV
			(IV0,IV1) */
			sec_tkn_set32le(&t[tl++], 0x23900000 | (4 + 4 + ivsize));
			break;
		case SA_CRYPTO_MODE_AES_3DES_CTR:
		case SA_CRYPTO_MODE_AES_3DES_CTR_LOAD_REUSE:
			if (gcm_flag) {
			/* Insert ESP header to HASH and OUTPUT -
				SPI and SEQ */
				sec_tkn_set32le(&t[tl++], 0x2B900000 | (4 + 4));
				/* Schuled remove of 16 bytes of encrypted ICV */
				/* FIXME For GCM is it (oiphl + 4 + 4 + 8) + 20 ?*/
				sec_tkn_set32le(&t[tl++], 0xA0820000 |
						(oiphl + 4 + 4 + 8));
				/* Insert ESP header to OUTPUT - ICV (IV1, IV2) */
				sec_tkn_set32le(&t[tl++], 0x21A80000 | 8);
			/* Insert 16 bytes of 0 to CRYPTO for remove by
				above instr */
				sec_tkn_set32le(&t[tl++], 0x25000010);
			} else {
			/* Insert ESP header to HASH and OUTPUT -
				SPI and SEQ */
				sec_tkn_set32le(&t[tl++],0x23900000 | (4 + 4));
			/* Insert ESP header to HASH and OUTPUT - ICV (IV1,
				IV2) */
				sec_tkn_set32le(&t[tl++],0x23A80000 | 8);
			}
			break;
	}
	/* Create token instructions - Process payload */
	if (ipv6) {
		/* FIXME - */
	} else {
		if (gcm_flag)
			/* Insert 16 bytes of 0 to CRYPTO for remove by above instr */
			sec_tkn_set32le(&t[tl++], 0x25000010);
		/* Increment TTL, set next header and total length and
		move to CRYPTO, HASH, and OUTPUT */
		sec_tkn_set32le(&t[tl++], 0x67010000 | ((pl) - (ethl)));
		/* Move remainder IP header and payload to CRYPTO, HASH, and
		OUTPUT */
		sec_tkn_set32le(&t[tl++],0x07000000 | ((pl) - (ethl) - 12));
	}
	/* Add padding to HASH, CRYPTO, and OUTPUT */
	sec_tkn_set32le(&t[tl++], 0x2F220000 | (0x04 << 9) | pdl);
	/* Create token instructions - Process ICV */
	/* Append hash result to OUTPUT */
	sec_tkn_set32le(&t[tl++], 0x21E60000 |
			((gcm_flag) ? gcm_flag : 12));

	sec_tkn_set32le(&t[tl++], 0xE12E1800 | ((seq_offset - 2) + 0x20));
	/* Create token header */

	sec_tkn_set_hdr(t, tlen, 0, 1 /* Source IV from PRNG */, 1, 0, 0, 0,
			pl, 1, tl, SA_PTR_HW(sa))

	return tl << 2;		/* Convert to byte unit */

}

inline void sec_tkn_update_espout_tunnel(unsigned int *t, unsigned int pl,
		unsigned char cm, unsigned int icv_len, unsigned char pad_len,
		unsigned char gcm_flag)
{
	/* Set the variable to first instruction */
	int tl = sec_tkn_1st_instr_idx;
	int ethl = 0;
	int oiphl = 20;
	int ipv6 = 0;

	sec_tkn_update_hdr(t, 0, 1, 1, 0, 0, 0, pl);
	
	switch (cm) {
		case SA_CRYPTO_MODE_AES_3DES_CBC:
			tl++;
			break;
		case SA_CRYPTO_MODE_AES_3DES_CTR:
		case SA_CRYPTO_MODE_AES_3DES_CTR_LOAD_REUSE:
			if (gcm_flag) {
				tl++;
				/* Schuled remove of 16 bytes of encrypted ICV */
				sec_tkn_set32le(&t[tl++], 0xA0800000 |
						(oiphl + 4 + 4 + 8));
			}
			tl += 2;
			break;
	}
	/* Create token instructions - Process payload */
	if (ipv6) {
		/* FIXME - */
	} else {
		/* Increment TTL, set next header and total
		length and move to CRYPTO, HASH,and OUTPUT */
		sec_tkn_set32le(&t[tl++], 0x67010000 | ((pl) - (ethl)));
		/* Move remainder IP header and
		payload to CRYPTO, HASH, and OUTPUT */
		sec_tkn_set32le(&t[tl++], 0x07000000 |
				((pl) - (ethl) - icv_len));
	}
	sec_tkn_set32le(&t[tl++], 0x2F220000 | (0x04 << 9) | pad_len);
	/* Add padding to HASH, CRYPTO, and OUTPUT */
	sec_tkn_set32le(&t[tl++], 0x21E60000 |
			((gcm_flag) ? gcm_flag : icv_len));
}

inline unsigned short sec_tkn_set_espin_tunnel(unsigned int *t,
		unsigned int pl, unsigned int sa, unsigned int *ctrl,
		unsigned char ca, unsigned char cm,
		unsigned char gcm_flag, unsigned int ihl,
		unsigned char icv_len, unsigned char seq_offset)
{
	unsigned short tl = 4;	/* Length of token in DWORD */
	unsigned short tlen;
	int oiphl = 20;
	unsigned char  ivsize = (ca == SA_CRYPTO_ALG_DES ||
			ca == SA_CRYPTO_ALG_3DES || cm ==
			SA_CRYPTO_MODE_AES_3DES_CTR) ? 8 : 16;

	/* Create context control WORD0 and WORD1 */
	sec_tkn_set32le(&t[tl++], ctrl[0]);
	sec_tkn_set32le(&t[tl++], ctrl[1]);

	/* Create token instructions - ESP AAD processing */
	switch (cm) {
		case SA_CRYPTO_MODE_AES_3DES_CBC:
		/* Retrieve ESP header to HASH -
			SPI, SEQ, and ICV (IV0,IV1) from INPUT */
			sec_tkn_set32le(&t[tl++], 0x42900000 | (4 + 4 + ivsize));
			break;
		case SA_CRYPTO_MODE_AES_3DES_CTR:
		case SA_CRYPTO_MODE_AES_3DES_CTR_LOAD_REUSE:
			if (gcm_flag) {
				sec_tkn_set32le(&t[tl++], 0x4B900000 | (4 + 4));
			/* Retrieve ESP header to HASH -
				SPI and SEQ - from INPUT */
				sec_tkn_set32le(&t[tl++], 0xA0800000 |
						(oiphl + 4 + 4 + 8));
				/* Schuled remove of 16 bytes of encrypted ICV */
				/* Retrieve ESP header IV1 (IV1, IV2) from INPUT */
				sec_tkn_set32le(&t[tl++], 0x40A80000 | 8);
			/* Insert 16 bytes of 0 to CRYPTO
				for remove by above instr */
				sec_tkn_set32le(&t[tl++], 0x25000010);
			} else {
				/* Retrieve ESP header to HASH - SPI and SEQ */
				sec_tkn_set32le(&t[tl++], 0x42900000 | (4 + 4));
				/* Retrieve ESP header to HASH - ICV (IV1, IV2) */
				sec_tkn_set32le(&t[tl++], 0x42A80000 | 8);
			}
			break;
	}
	/* Create token instructions - Payload processing */
	sec_tkn_set32le(&t[tl++], 0x0F020000 |
			(pl - 8 - ivsize - icv_len));
	/* Move payload to HASH, CRYPTO, and OUTPUT */
	/* Create token instructions - ICV processing */
	sec_tkn_set32le(&t[tl++], 0x40E60000 | icv_len);
	/* Retrieve ICV from packet for verification */
	sec_tkn_set32le(&t[tl++], 0xDD070000 | icv_len);
	/* Update context seq number */
	sec_tkn_set32le(&t[tl++], 0xE32E1800 | ((seq_offset - 2) + 0x20));

	/* Create token header */
	sec_tkn_set_hdr(t, tlen, 0, 0, 1, 5 /*ToO=101b*/, 0,
			0, pl, 1, tl, SA_PTR_HW(sa))

	return tl << 2;		/* Convert to byte unit */
}

inline void sec_tkn_update_espin_tunnel(unsigned int *t, unsigned int pl,
					unsigned char cm,
					unsigned char ca,
					unsigned int icvsize,
					unsigned int gcm_flag)
{
	int tl = sec_tkn_1st_instr_idx;
	int oiphl = 20;
	unsigned char  ivsize = (ca == SA_CRYPTO_ALG_DES ||
			ca == SA_CRYPTO_ALG_3DES || cm ==
			SA_CRYPTO_MODE_AES_3DES_CTR) ? 8 : 16;

	sec_tkn_update_hdr(t, 0, 0, 1, 5 /*ToO=101b*/, 0, 0, pl);
	switch (cm) {
		case SA_CRYPTO_MODE_AES_3DES_CBC:
			tl++;
			break;
		case SA_CRYPTO_MODE_AES_3DES_CTR:
		case SA_CRYPTO_MODE_AES_3DES_CTR_LOAD_REUSE:
			if (gcm_flag) {
				tl++;
				/* Schuled remove of 16 bytes of encrypted ICV */
				sec_tkn_set32le(&t[tl++], 0xA0800000 |
						(oiphl + 4 + 4 + 8));
			}
			tl += 2;
			break;
	}
	/* Move payload to HASH, CRYPTO, and OUTPUT */
	sec_tkn_set32le(&t[tl++], 0x0F020000 | (pl - 4 - 4 - ivsize -
			(gcm_flag ? gcm_flag : icvsize)));
}

/**
 * Outbound ESP Transport Create Token  AES/DES/DES3/AES(CTR)
 *
 */
inline unsigned short sec_tkn_set_espout_transport(unsigned int *t,
		unsigned int pl, unsigned int sa, unsigned int *ctrl,
		unsigned char ca, unsigned char pdl,
		unsigned char icvsize, unsigned char nh,
		unsigned char iphl, unsigned char seq_offset,
		unsigned char cm, int gcm_flag)
{
	unsigned short tl = 4;	/* Length of token in DWORD */
	unsigned short tlen;
	unsigned char  ivsize = (ca == SA_CRYPTO_ALG_DES ||
			ca == SA_CRYPTO_ALG_3DES || cm ==
			SA_CRYPTO_MODE_AES_3DES_CTR) ? 8 : 16;

	/* Create context control WORD0 and WORD1 */
	sec_tkn_set32le(&t[tl++], ctrl[0]);
	sec_tkn_set32le(&t[tl++], ctrl[1]);
	/* Move IP header to output with protocol replaced with 0x32 */
	sec_tkn_set32le(&t[tl++], 0x69320000 | (8+ivsize+pl+pdl+icvsize));
	/* Move remainder IPv4 header to output */
	sec_tkn_set32le(&t[tl++], 0x01000000 | (iphl - 12));
	/* Insert ESP header to HASH & output - SPI, SEQ, & IV -- Insert */
	if(cm == SA_CRYPTO_MODE_AES_3DES_CTR) {
		if (gcm_flag) {
			sec_tkn_set32le(&t[tl++], 0x2B900000 | (4 + 4));
			/* Schuled remove of 16 bytes of encrypted ICV:
			Ofsset of IP Header + SPI SEQ + IVSIZE */
			/** STAT bit is set here */
			sec_tkn_set32le(&t[tl++], 0xA0820000 | (iphl + 8 + 8));
			/* Insert IV to Output */
			sec_tkn_set32le(&t[tl++], 0x21A80000 | 8);
		}
		else {
			sec_tkn_set32le(&t[tl++], 0x23900000 | (4 + 4));
			/* Insert IV to Output */
			sec_tkn_set32le(&t[tl++], 0x23A80000 | 8);
		}
	}
	else {
		sec_tkn_set32le(&t[tl++], 0x23900000 | (4 + 4 + ivsize));
	}
	if (gcm_flag)
		/* Insert 16 bytes of 0 to CRYPTO for remove by above instr */
		sec_tkn_set32le(&t[tl++], 0x25000010);

	/* Move payload to CRYPTO, HASH, and OUTPUT  -- Direction */
	sec_tkn_set32le(&t[tl++], 0x07000000 | (pl - iphl));
	/* Add padding to CRYPTO, HASH, and OUTPUT -- Insert */
	sec_tkn_set32le(&t[tl++], 0x2F220000 | (nh << 9) | pdl);
	/* Append hash result to OUTPUT  -- Insert */
	sec_tkn_set32le(&t[tl++], 0x21E60000 | icvsize);
	/* Update context seq number -- Context Access*/
	sec_tkn_set32le(&t[tl++], 0xE12E1800 | ((seq_offset - 2) + 0x20));

	/* Create token header */
	sec_tkn_set_hdr(t, tlen, 0, 1 /* Source IV from PRNG */, 1, 0, 0, 0,
			pl, 1, tl, SA_PTR_HW(sa))


	return tl << 2;		/* Convert to byte unit */
}


/**
 * Outbound ESP Transport Update Token AES/DES/DES3/AES(CTR)
 *
 */
inline void sec_tkn_update_espout_transport(unsigned int *t,
		unsigned int pl, unsigned char ca, unsigned char pdl,
		unsigned char nh, unsigned char iphl, unsigned char icvsize,
		unsigned char cm, int gcm_flag)
{
	unsigned char  ivsize = (ca == SA_CRYPTO_ALG_DES ||
			ca == SA_CRYPTO_ALG_3DES || cm ==
			SA_CRYPTO_MODE_AES_3DES_CTR) ? 8 : 16;
	int instr_index = 0;
	
	sec_tkn_update_hdr(t, 0, 1, 1, 0, 0, 0, pl);
	/* Move IP header to output with protocol replaced with 0x32 */
	sec_tkn_set32le(&t[sec_tkn_1st_instr_idx],
			   0x69320000 | (8+ivsize+pl+pdl+icvsize));
	/* Move payload to CRYPTO, HASH, and OUTPUT */
	if (cm == SA_CRYPTO_MODE_AES_3DES_CTR) {
		if (gcm_flag)
			instr_index = sec_tkn_1st_instr_idx + 6;
		else
			instr_index = sec_tkn_1st_instr_idx + 4;
		sec_tkn_set32le(&t[instr_index],
				   0x07000000 | (pl - iphl));
		/* Add padding to CRYPTO, HASH, and OUTPUT */
		sec_tkn_set32le(&t[++instr_index],
				   0x2F220000 | (nh << 9) | pdl);
	}
	else {
		sec_tkn_set32le(&t[sec_tkn_1st_instr_idx + 3],
				   0x07000000 | (pl - iphl));
		/* Add padding to CRYPTO, HASH, and OUTPUT */
		sec_tkn_set32le(&t[sec_tkn_1st_instr_idx + 4],
				   0x2F220000 | (nh << 9) | pdl);

	}
}
/**
 * Inbound ESP Transport Create Token for AES/DES/DES3/AES(CTR)
 *
 */
inline unsigned short sec_tkn_set_espin_transport(unsigned int *t,
			unsigned int pl, unsigned int sa, unsigned int *ctrl,
			unsigned char ca, unsigned int ihl, 
			unsigned char icv_len, unsigned char seq_offset,
			unsigned char cm, int gcm_flag)
{
	unsigned short tl = 4;	/* Length of token in DWORD */
	unsigned short tlen;
	unsigned char  ivsize = (ca == SA_CRYPTO_ALG_DES ||
			ca == SA_CRYPTO_ALG_3DES || cm ==
			SA_CRYPTO_MODE_AES_3DES_CTR) ? 8 : 16;

	/* Create context control WORD0 and WORD1 */
	sec_tkn_set32le(&t[tl++], ctrl[0]);
	sec_tkn_set32le(&t[tl++], ctrl[1]);
	/* Process the IP header first 3 DWORD */
	sec_tkn_set32le(&t[tl++], 0x61000000);
	/* Move remainder IPv4 header to output */
	sec_tkn_set32le(&t[tl++], 0x01000000 | (ihl-12));
	/* Retrieve ESP header to HASH/context - SPI, SEQ, and IV */
	if (cm == SA_CRYPTO_MODE_AES_3DES_CTR) {
		if (gcm_flag) {
			sec_tkn_set32le(&t[tl++], 0x4A900000 | (4 + 4));
			/* Schuled remove of 16 bytes of encrypted ICV:
			Ofsset of IP Header */
			sec_tkn_set32le(&t[tl++], 0xA0820000 | ihl);
			sec_tkn_set32le(&t[tl++], 0x40A80000 | 8);
			/* Insert 16 bytes of 0 to CRYPTO for remove by above instr */
			sec_tkn_set32le(&t[tl++], 0x25000010);
		} else {
			sec_tkn_set32le(&t[tl++], 0x42900000 | (4 + 4));
			sec_tkn_set32le(&t[tl++], 0x42A80000 | 8);
		}
	}
	else {
		sec_tkn_set32le(&t[tl++], 0x42900000 | (4 + 4 + ivsize));
	}

	/* Move payload to CRYPTO, HASH, and OUTPUT */
	sec_tkn_set32le(&t[tl++], 0x0F020000 |
			  (pl - ihl - 8 - ivsize - icv_len));
	/* Retrieve ICV to context for verification */
	sec_tkn_set32le(&t[tl++], 0x40E60000 | icv_len);
	/* Update NH, Check sum, and protocol */
	sec_tkn_set32le(&t[tl++], 0xAE050002 | (7 << 19));

	/* Verify IPSec padding and hash result */
	sec_tkn_set32le(&t[tl++], 0xD5070000 | icv_len);

	/* Update context seq number */
	sec_tkn_set32le(&t[tl++], 0xE12E1800 | ((seq_offset - 2) + 0x20));

	/* Create token header */
	sec_tkn_set_hdr(t, tlen, 0, 0, 1, 5 /*ToO=101b*/, 0, 0, pl,
			1, tl, SA_PTR_HW(sa))

	return tl << 2;		/* Convert to byte unit */
}

/**
 * Inbound ESP Transport Update Token for AES/DES/DES3/AES(CTR)
 *
 */
void sec_tkn_update_espin_transport(unsigned int *t,
			unsigned int pl, unsigned char ca, unsigned short ihl,
			unsigned char icv_len, unsigned char cm, int gcm_flag)
{	
	unsigned char  ivsize = (ca == SA_CRYPTO_ALG_DES ||
			ca == SA_CRYPTO_ALG_3DES || cm ==
			SA_CRYPTO_MODE_AES_3DES_CTR) ? 8 : 16;

	sec_tkn_update_hdr(t, 0, 0, 1, 5 /*ToO=101b*/, 0, 0, pl);
	/* Move remainder IPv4 header to output */
	sec_tkn_set32le(&t[sec_tkn_1st_instr_idx + 1], 0x01000000 | (ihl-12));
	if (cm == SA_CRYPTO_MODE_AES_3DES_CTR) {
		if (gcm_flag)
			sec_tkn_set32le(&t[sec_tkn_1st_instr_idx + 6], 0x0F020000 |
					(pl - ihl - 8 - ivsize - icv_len));
		else 
			sec_tkn_set32le(&t[sec_tkn_1st_instr_idx + 4], 0x0F020000 |
					(pl - ihl - 8 - ivsize - icv_len));
	} else {
		sec_tkn_set32le(&t[sec_tkn_1st_instr_idx + 3], 0x0F020000 |
			  	(pl - ihl - 8 - ivsize - icv_len));
	}
	/* Move payload to CRYPTO, HASH, and OUTPUT */
}

/**
 * Outbound ESP Transport Create Token for CCM
 *
 */
inline unsigned short sec_tkn_set_espout_transport_ccm(unsigned int *t,
		unsigned int pl, unsigned int sa,
		unsigned int *ctrl, unsigned char pdl,
		unsigned char icvsize, unsigned char nh,
		unsigned char iphl, unsigned char *iv_B0,
		unsigned char *adata_length, unsigned char seq_offset)
{
	unsigned short tl = 4;	/* Length of token in DWORD */
	unsigned short tlen;
	unsigned char  ivsize = 8;
	int hash_pad;
	
	/* Create context control WORD0 and WORD1 */
	sec_tkn_set32le(&t[tl++], ctrl[0]);
	sec_tkn_set32le(&t[tl++], ctrl[1]);

	/* Move IP header to output with protocol replaced with 0x32 TTL Dec*/
	sec_tkn_set32le(&t[tl++], 0x69320000 | (8+ivsize+pl+pdl+icvsize));
	/* Move remainder IPv4 header to output */
	sec_tkn_set32le(&t[tl++], 0x01000000 | (iphl - 12));
	/*Insert Flag+Salt to Hash*/
	sec_tkn_set32le(&t[tl++], 0x22D80000 | 4);
	sec_tkn_setle(&t[tl++], &iv_B0[0], 4);
	/* Insert IV to Hash */
	sec_tkn_set32le(&t[tl++], 0x22A80000 | ivsize);
	/*Adata length and Message length fields*/
	sec_tkn_set32le(&t[tl++], 0x22D80000 | (4 + 2));
	//sec_tkn_setle(&t[tl++], (iv_B0 + 12), 4);
	sec_tkn_setle(&t[tl++], (iv_B0 + 12), 4);
	sec_tkn_setle(&t[tl++], adata_length, 2);

	/* Insert ESP header to HASH & output - SPI, SEQ, &  No IV -- Insert */
	sec_tkn_set32le(&t[tl++], 0x23900000 | (4 + 4));
	/* Hash Padding length* 16 - spi - seq*/
	sec_tkn_set32le(&t[tl++], 0x22000000 | 6);
	/* Insert IV to Hash */
	sec_tkn_set32le(&t[tl++], 0x21A80000 | ivsize);
	/* Schuled remove of 16 bytes of encrypted ICV:
	Ofsset of IP Header + SPI SEQ */
	sec_tkn_set32le(&t[tl++], 0xA0820000 | (iphl + 8 + ivsize));
	/* Insert 16 bytes of 0 to CRYPTO for remove by above instr */
	sec_tkn_set32le(&t[tl++], 0x25000010);
	/* Move payload to CRYPTO, HASH, and OUTPUT  -- Direction */
	sec_tkn_set32le(&t[tl++], 0x07000000 | (pl - iphl));
	/* Add padding to CRYPTO, HASH, and OUTPUT -- Insert */
	sec_tkn_set32le(&t[tl++], 0x2F220000 | (nh << 9) | pdl);
	hash_pad = (pl - iphl) % 16;
	if (hash_pad)
		sec_tkn_set32le(&t[tl++], 0x22020000 | hash_pad);
	/* Append hash result to OUTPUT  -- Insert */
	sec_tkn_set32le(&t[tl++], 0x21E60000 | icvsize);
	/* Update context seq number -- Context Access*/
	sec_tkn_set32le(&t[tl++], 0xE12E1800 | ((seq_offset - 2) + 0x20));
	/* Create token header */
	/* FIXME Changed the source IV from PRNG to all from Context */
	sec_tkn_set_hdr(t, tlen, 0, 0 /* Source IV from PRNG */, 1, 0, 0, 0,
			pl, 1, tl, SA_PTR_HW(sa))

			return tl << 2;		/* Convert to byte unit */
}

/**
 * Inbound ESP Transport Update Token for CCM
 *
 */
inline unsigned short sec_tkn_set_espin_transport_ccm(unsigned int *t,
		unsigned int pl, unsigned int sa, unsigned int *ctrl,
		unsigned char pdl, unsigned char icvsize,
		unsigned char nh, unsigned char *iv_B0,
		unsigned char *adata_length, unsigned char seq_offset)
{
	unsigned short tl = 4;	/* Length of token in DWORD */
	unsigned short tlen;
	unsigned char  ivsize = 8;
	unsigned int plength;
	int hash_pad;
	
	/* Create context control WORD0 and WORD1 */
	sec_tkn_set32le(&t[tl++], ctrl[0]);
	sec_tkn_set32le(&t[tl++], ctrl[1]);
	/* Process the IP header first 3 DWORD */
	sec_tkn_set32le(&t[tl++], 0x69000000);
	/* Move remainder IPv4 header to output */
	sec_tkn_set32le(&t[tl++], 0x01000000 | (20 - 12));
	/* Retrieve ESP header SPI SEQ  */
	sec_tkn_set32le(&t[tl++], 0x40900000 | (4 + 4));
	/*Insert Flag, Salt to Hash*/
	sec_tkn_set32le(&t[tl++], 0x22D80000 | 4);
	sec_tkn_setle(&t[tl++], &iv_B0[0], 4);
	/*Retrieve IV */
	sec_tkn_set32le(&t[tl++], 0x42A80000 | ivsize);
	/*Adata length and Message length fields*/
	sec_tkn_set32le(&t[tl++], 0x22D80000 | (4 + 2));
	sec_tkn_setle(&t[tl++], iv_B0 + 12, 4);
	sec_tkn_setle(&t[tl++], adata_length, 2);

	/* Insert ESP header to HASH & output -
	SPI, SEQ, &  No IV -- Insert */
	sec_tkn_set32le(&t[tl++], 0x22900000 | (4 + 4));
	/* Hash Padding length* 16 - spi - seq*/
	sec_tkn_set32le(&t[tl++], 0x22000000 | 6);
	/* Schuled remove of 16 bytes of encrypted ICV:
	Ofsset of IP Header + SPI SEQ */
	sec_tkn_set32le(&t[tl++], 0xA0820000 | 0x14);
	/* Insert 16 bytes of 0 to CRYPTO for remove by above instr */
	sec_tkn_set32le(&t[tl++], 0x25000010);
	/* Move payload to CRYPTO, HASH, and OUTPUT */
	plength = pl - 20 - 8 - ivsize - icvsize;
	sec_tkn_set32le(&t[tl++], 0x0F000000 | plength);
	hash_pad = plength % 16;
	if (hash_pad)
		sec_tkn_set32le(&t[tl++], 0x22020000 | hash_pad);
	/* Retrieve ICV to context for verification */
	sec_tkn_set32le(&t[tl++], 0x40E60000 | icvsize);
	/* Update NH, Check sum, and protocol */
	sec_tkn_set32le(&t[tl++], 0xAE050002 | (7 << 19));
	/* Verify IPSec padding and hash result */
	sec_tkn_set32le(&t[tl++], 0xD5070000 | icvsize);
	/* Update context seq number */
	sec_tkn_set32le(&t[tl++], 0xE12E1800 | ((seq_offset - 2) + 0x20));

	sec_tkn_set_hdr(t, tlen, 0, 0, 1, 5 /*ToO=101b*/, 0, 0, pl,
			1, tl, SA_PTR_HW(sa))

			return tl << 2;		/* Convert to byte unit */
	
}
