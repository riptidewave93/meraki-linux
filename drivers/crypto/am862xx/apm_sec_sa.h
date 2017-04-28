/**
 * AppliedMicro APM862xx SoC Security Driver 
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
 * @file   apm_sec_sa.h
 *
 * This file defines the security context assoicate format. Security 
 * association is touched by hardware and will require DMA'able memory.
 *
 */
#ifndef __APM_SEC_SA_H__
#define __APM_SEC_SA_H__

#include "apm_sec_access.h"		/* for link list */

#define SA_ALIGN16(x)			(((x) + 15) & ~0xF)

#define SA_TOP_NULL_OUTBOUND	    	0x00
#define SA_TOP_NULL_INBOUND             0x01
#define SA_TOP_HASH_OUTBOUND            0x02
#define SA_TOP_HASH_INBOUND             0x03
#define SA_TOP_CRYPTO_OUTBOUND          0x04
#define SA_TOP_CRYPTO_INBOUND           0x05
#define SA_TOP_ENCRYPT_HASH_OUTBOUND    0x06
#define SA_TOP_DECRYPT_HASH_INBOUND     0x07
#define SA_TOP_HASH_ENCRYPT_OUTBOUND    0x0E
#define SA_TOP_HASH_DECRYPT_INBOUND     0x0F

/* Type 0, 1, and 3 */
#define SA_HASH_ALG_MD5                 0x00
#define SA_HASH_ALG_SHA1_SSL_MAC        0x01
#define SA_HASH_ALG_SHA1                0x02
#define SA_HASH_ALG_SHA256              0x03
#define SA_HASH_ALG_SHA224              0x04
#define SA_HASH_ALG_SHA512              0x05
#define SA_HASH_ALG_SHA384              0x06
#define SA_HASH_ALG_RSVD                0x07
#define SA_HASH_ALG_NULL                SA_HASH_ALG_RSVD

#define SA_DIGEST_TYPE_INIT             0x00
#define SA_DIGEST_TYPE_INNER            0x01
#define SA_DIGEST_TYPE_HASH             0x02
#define SA_DIGEST_TYPE_HMAC             0x03

#define SA_CRYPTO_ALG_DES               0x00
#define SA_CRYPTO_ALG_ARC4              0x01
#define SA_CRYPTO_ALG_3DES              0x02
#define SA_CRYPTO_ALG_KASUMI_F8         0x03
#define SA_CRYPTO_ALG_RSVD              0x04
#define SA_CRYPTO_ALG_AES128            0x05
#define SA_CRYPTO_ALG_AES192            0x06
#define SA_CRYPTO_ALG_AES256            0x07

/* Use internally to select no crypto */
#define SA_CRYPTO_ALG_NULL              SA_CRYPTO_ALG_RSVD

/* Type 2 */
#define SA_HASH_ALG_XCBC128             0x01
#define SA_HASH_ALG_XCBC192             0x02
#define SA_HASH_ALG_XCBC256             0x03
#define SA_HASH_ALG_GHASH               0x04
#define SA_HASH_ALG_KASUMI_F9           0x07
#define SA_HASH_ALG_XNULL               0x00

#define SA_SEQ_NO_COUNTER               0x00
#define SA_SEQ_32BITS                   0x01
#define SA_SEQ_48BITS                   0x02
#define SA_SEQ_64BITS                   0x03

#define SA_MASK_NONE                    0x00
#define SA_MASK_64                      0x01
#define SA_MASK_32                      0x02
#define SA_MASK_128                     0x03

#define SA_CRYPTO_MODE_AES_3DES_ECB              0x00
#define SA_CRYPTO_MODE_AES_3DES_CBC              0x01
#define SA_CRYPTO_MODE_AES_3DES_CTR              0x02
#define SA_CRYPTO_MODE_AES_3DES_ICM              0x03
#define SA_CRYPTO_MODE_AES_3DES_OFB              0x04
#define SA_CRYPTO_MODE_AES_3DES_CFB              0x05
#define SA_CRYPTO_MODE_AES_3DES_CTR_LOAD_REUSE   0x06
#define SA_CRYPTO_MODE_AES_3DES_ICM_LOAD_REUSE   0x07
#define SA_CRYPTO_MODE_AES_3DES_BLOCK_SIZE       0x00
#define SA_CRYPTO_MODE_KASUMI                    0x00
#define SA_CRYPTO_MODE_KASUMI_F8                 0x01

#define SA_IV_FORMAT_FULL_IV_MODE       0x00
#define SA_IV_FORMAT_COUNTER_MODE       0x01
#define SA_IV_FORMAT_ORIG_SEQ_NUM_MODE  0x02
#define SA_IV_FORMAT_INCR_SEQ_NUM_MODE  0x03

#define SA_PAD_TYPE_ZERO                0x00
#define SA_PAD_TYPE_PKCS7               0x01
#define SA_PAD_TYPE_CONSTANT            0x02
#define SA_PAD_TYPE_RTP                 0x03
#define SA_PAD_TYPE_IPSEC               0x04
#define SA_PAD_TYPE_TLS                 0x05
#define SA_PAD_TYPE_SSL                 0x06
#define SA_PAD_TYPE_IPSEC_NO_SEQ_CHECK  0x07

#define SA_PBO_RESTART_HASH		0x01
#define SA_PBO_NOFINISH_HASH		0x02
#define SA_PBO_INIT_COUNTER		0x04
#define SA_PBO_INIT_ARC4		0x08

#define SA_ARC4_STATELESS		0
#define SA_ARC4_STATEFULL		1

struct sec_sa_ctrl {
	/* Hardware is little endian and this structure is represented
	   in little endian format */
#if defined(__LITTLE_ENDIAN)
	unsigned int ToP		:4;
	unsigned int pkt_based_options	:4;
	unsigned int context_length	:8;
	unsigned int key		:1;
	unsigned int crypto_alg		:3;
	unsigned int rsvd1		:1;
	unsigned int digest_type	:2;
	unsigned int hash_alg		:3;
	unsigned int rsvd2		:1;
	unsigned int spi		:1;
	unsigned int seq		:2;
	unsigned int mask		:2;

	unsigned int cryptomode_feedback:5;
	unsigned int iv03		:4;
	unsigned int digest_cnt		:1;
	unsigned int iv_format		:2;
	unsigned int crypto_store	:1;
	unsigned int rsvd3		:1;
	unsigned int pad_type		:3;
	unsigned int enc_hash_result	:1;
	unsigned int kasumi_f9_direction:1;
	unsigned int hash_store		:1;
	unsigned int ij_pointer		:1;
	unsigned int state_selection	:1;
	unsigned int seq_nbr_store	:1;
	unsigned int disable_mask_update:1;
	unsigned int resvd4		:7;
	unsigned int address_mode	:1;
#else
	unsigned int mask		:2;
	unsigned int seq		:2;
	unsigned int spi		:1;
	unsigned int rsvd2		:1;
	unsigned int hash_alg		:3;
	unsigned int digest_type	:2;
	unsigned int rsvd1		:1;
	unsigned int crypto_alg		:3;
	unsigned int key		:1;
	unsigned int context_length	:8;
	unsigned int pkt_based_options	:4;
	unsigned int ToP		:4;

	unsigned int address_mode	:1;
	unsigned int resvd4		:7;
	unsigned int disable_mask_update:1;
	unsigned int seq_nbr_store	:1;
	unsigned int state_selection	:1;
	unsigned int ij_pointer		:1;
	unsigned int hash_store		:1;
	unsigned int kasumi_f9_direction:1;
	unsigned int enc_hash_result	:1;
	unsigned int pad_type		:3;
	unsigned int rsvd3		:1;
	unsigned int crypto_store	:1;
	unsigned int iv_format		:2;
	unsigned int digest_cnt		:1;
	unsigned int iv03		:4;
	unsigned int cryptomode_feedback:5;
#endif
} __attribute__ ((packed));

struct sec_sa_item {
	struct list_head next;
	unsigned int	sa_len;
	/* Point to a location which is 16 bytes aligned - 8 */
	struct sec_sa_ctrl *sa_ptr;
	/* unsigned char rsvd[8]; */ 	/* We reserved 8 bytes to make SA to align
				   at 16 bytes - 8 bytes as the SA
				   two control WORD's are skipped. */
	unsigned int sahw[0];	/* Variable length SA following */
	
} __attribute__((aligned(16)));

#define SA_PTR_COMPUTE_ALIGN(sa) \
		((u32) ((unsigned char *) (sa)  + \
		(sizeof(struct sec_sa_item))))
#define SA_PTR_COMPUTE(sa)	\
		((void *) ((unsigned char *) \
		TKN_ALIGN16(SA_PTR_COMPUTE_ALIGN(sa)) + 8))

#define SA_PTR_ALIGN(sa)	\
		((void *) ((unsigned char *) (sa) + 8))
#define SA_PTR_HW(sa)		\
		((void *) ((unsigned char *) (sa) - 8))

#define SA_KEY_IDX		2
#define SA_KEY_OFFSET       	(SA_KEY_IDX << 2)
#define SA_KEY(sa)		(&((unsigned int *) (sa))[SA_KEY_IDX])
#define SA_DIGEST(sa)	 	\
		(&((unsigned int *) (sa))[SA_DIGEST_OFFSET(sa)])
#define SA_SPI_OFFSET(sa)  	\
		(SA_DIGEST_OFFSET(sa) + \
		(sec_sa_digest_len((struct sec_sa_ctrl *) (sa)) >> 2))
#define SA_IV_OFFSET(sa)    	\
		(SA_MASK_OFFSET(sa) + \
			(sec_sa_mask_len((struct sec_sa_ctrl *) (sa)) >> 2))
#define SA_IV(sa)		\
		(&((unsigned int *) (sa))[SA_IV_OFFSET(sa)])
#define SA_DIGEST_OFFSET(sa)	\
		(SA_KEY_IDX + \
		(sec_sa_key_len((struct sec_sa_ctrl *) (sa)) >> 2))
#define SA_SPI(sa)		\
		(&((unsigned int *) (sa))[SA_SPI_OFFSET(sa)])
#define SA_SEQ_OFFSET(sa)   \
		(SA_SPI_OFFSET(sa) + \
		(((struct sec_sa_ctrl *) (sa))->spi?1:0))
#define SA_SEQ(sa)  		\
		(&((unsigned int *) (sa))[SA_SEQ_OFFSET(sa)])
#define SA_MASK_OFFSET(sa)	\
		(SA_SEQ_OFFSET(sa) + \
		(sec_sa_seq_len((struct sec_sa_ctrl *) (sa)) >> 2))
#define SA_ARC4_OFFSET(sa)	\
		(SA_IV_OFFSET(sa) + \
		(sec_sa_iv_len((struct sec_sa_ctrl *) (sa)) >> 2))
#define SA_ARC4(sa)		\
		(&((unsigned int *) (sa))[SA_ARC4_OFFSET(sa)])

#define SA_MD5_LEN(hmac)	(2*4+4*4*((hmac)?2:0))
#define SA_SHA1_LEN(hmac)	(2*4+5*4*((hmac)?2:0))
#define SA_SHA2_LEN(ds, hmac)	(2*4+((ds)*((hmac)?2:0)))
#define SA_AES_XCBC_LEN		(2*4+12*4)
#define SA_KASUMI_F9_LEN	(2*4+4*4)
#define SA_AES_CCM_MAX_LEN	(2*4+512/8+4*4*3+4*4)
#define SA_AES_CCM_LEN(kl)	(2*4+(kl)+4*4*3+4*4)
#define SA_AES_ESP_CCM_MAX_LEN	(2*4+512/8+3*4*4+4*4+2*4)
#define SA_AES_ESP_CCM_LEN(kl)	(2*4+(kl)+3*4*4+4*4+2*4)
#define SA_AES_CCM_MAX_LEN	(2*4+512/8+4*4*3+4*4)
#define SA_AES_CCM_LEN(kl)	(2*4+(kl)+4*4*3+4*4)
#define SA_AES_GCM_MAX_LEN	(2*4+512/8+4*4+3*4)
#define SA_AES_GCM_LEN(kl)	(2*4+(kl)+4*4+3*4)
#define SA_DES_MAX_LEN		(sizeof(struct sec_sa_des))
#define SA_DES_LEN(cm)		\
		(sizeof(struct sec_sa_des) - \
		 ((cm) == SA_CRYPTO_MODE_AES_3DES_CBC ? 0 : 8)) 
#define SA_3DES_MAX_LEN		(sizeof(struct sec_sa_3des))
#define SA_3DES_LEN(cm)		\
		(sizeof(struct sec_sa_3des) - \
		 ((cm) == SA_CRYPTO_MODE_AES_3DES_CBC ? 0 : 8))		
#define SA_AES_MAX_LEN(cm)	\
		(2*4+512/8+ \
		 (((cm) == SA_CRYPTO_MODE_AES_3DES_CBC || \
		   (cm) == SA_CRYPTO_MODE_AES_3DES_CTR || \
		   (cm) == SA_CRYPTO_MODE_AES_3DES_CTR_LOAD_REUSE || \
		   (cm) == SA_CRYPTO_MODE_AES_3DES_ICM || \
		   (cm) == SA_CRYPTO_MODE_AES_3DES_ICM_LOAD_REUSE) \
		   ? 16 : 0))
#define SA_AES_LEN(kl, cm)	\
		(2*4+(kl)+ \
		 (((cm) == SA_CRYPTO_MODE_AES_3DES_CBC || \
		   (cm) == SA_CRYPTO_MODE_AES_3DES_CTR || \
		   (cm) == SA_CRYPTO_MODE_AES_3DES_CTR_LOAD_REUSE || \
		   (cm) == SA_CRYPTO_MODE_AES_3DES_ICM || \
		   (cm) == SA_CRYPTO_MODE_AES_3DES_ICM_LOAD_REUSE) \
		   ? 16 : 0))
#define SA_KASUMI_F8_LEN		(2*4+4*4)
#define SA_ARC4_LEN(cm)			(2*4+4*4+((cm)?8:0))
#define SA_AES_CTR_LEN(kl, ha, dt)	(2*4+(kl)+ \
		sec_sa_compute_digest_len((ha), (dt))+3*4)
#define SA_AES_CTR_MAX_LEN		(2*4+512/8+\
		sec_sa_compute_digest_len((ha), (dt))+3*4)
#define SA_MACSEC_LEN			(2*4+4*4+4*4+4+4*4+3*4)

struct sec_sa_des {
	struct sec_sa_ctrl ctrl;
	unsigned int key[2];
	unsigned int iv[2];	/* for CBC mode */
} __attribute__ ((packed));

struct sec_sa_3des {
	struct sec_sa_ctrl ctrl;
	unsigned int key[6];
	unsigned int iv[2];	/* for CBC mode */
} __attribute__ ((packed));

#ifdef APM_SEC_SATKNDEBUG
void sec_sa_dump(unsigned int *sa, unsigned int sa_len);
void sec_sa_dumphw(void);
#endif

unsigned int sec_sa_compute_seq_len(unsigned char seq);
unsigned int sec_sa_compute_mask_len(unsigned char mask);
unsigned int sec_sa_compute_iv_len(unsigned char iv03);
unsigned int sec_sa_compute_digest_len(unsigned char ha, unsigned char dt);
unsigned int sec_sa_digest_len(struct sec_sa_ctrl *sa);
unsigned int sec_sa_key_len(struct sec_sa_ctrl *sa);
unsigned int sec_sa_seq_len(struct sec_sa_ctrl *sa);
unsigned int sec_sa_mask_len(struct sec_sa_ctrl *sa);
unsigned int sec_sa_iv_len(struct sec_sa_ctrl *sa);

static inline void sec_sa_setbe(unsigned int *sa, const unsigned char *buf, 
				int len)
{	/* SA is in big endian */
	for (; len > 0; buf += 4, len -= 4) {
		*sa++ = cpu_to_be32(*(unsigned int *) buf);
	}
}

static inline void sec_sa_setle(unsigned int *sa, const unsigned char *buf, 
				int len)
{	/* SA is in little endian */
	for (; len > 0; buf += 4, len -= 4) {
		*sa++ = cpu_to_le32(*(unsigned int *) buf);
	}
}

static inline void sec_sa_set32be(unsigned int *sa, unsigned int val)
{
	*sa = cpu_to_be32(val);
}

static inline void sec_sa_set32le(unsigned int *sa, unsigned int val)
{
	*sa = cpu_to_le32(val);
}

/**
 * @brief Load message length into CCM first block
 *
 */
int sec_sa_set_aes_ccm_msg_len(unsigned char *block, unsigned int msglen,
				int csize);

/**
 * @brief Load adata into CCM first block
 *
 */
int sec_sa_set_aes_ccm_adata(unsigned char *adata, unsigned int a);

/**
 * @brief Compute algorthm mode by key length
 */
static inline int sec_sa_keylen2aes_mode(int kl)
{
	switch (kl) {
	default:
	case 128/8:
		return SA_CRYPTO_ALG_AES128;
	case 192/8:
		return SA_CRYPTO_ALG_AES192;
	case 256/8:
		return SA_CRYPTO_ALG_AES256;
	}
}

/* NOTE: To be ported */

#define SA_AUTHENC_DES_LEN(cm, ha, dt)  \
		(SA_DES_LEN(cm) + \
		sec_sa_compute_digest_len((ha), (dt)) + 4 + 4)
#define SA_AUTHENC_DES_MAX_LEN(cm, ha, dt)  \
		(SA_DES_MAX_LEN + \
		 sec_sa_compute_digest_len((ha), (dt)) + 4 + 4)
#define SA_AUTHENC_3DES_LEN(cm, ha, dt)  \
		(SA_3DES_LEN(cm) + \
		sec_sa_compute_digest_len((ha), (dt)) + 4 + 4)
#define SA_AUTHENC_3DES_MAX_LEN(cm, ha, dt)  \
		(SA_3DES_MAX_LEN + \
		sec_sa_compute_digest_len((ha), (dt)) + 4 + 4)
#define SA_AUTHENC_AES_LEN(kl, cm, ha, dt)  \
		(SA_AES_LEN(kl, cm) + \
		 sec_sa_compute_digest_len((ha), (dt)) + 4 + 4)
#define SA_AUTHENC_AES_MAX_LEN(kl, cm, ha, dt)  \
		(SA_AES_MAX_LEN(cm) + \
		 sec_sa_compute_digest_len((ha), (dt)) + 4 + 4)

static inline void ipe_sa_seth2h(unsigned int *sa, const unsigned char *buf, 
				 int len)
{
	memcpy(sa, buf, len);
}

#endif
