/**
 * AppliedMicro APM862XX SoC Security Driver
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
 * @file   apm_sec_kn.h
 *
 * This file defines the secure token messages format. All token are in 
 * little endian format.
 *
 */
#ifndef __APM_SEC_TKN_H__
#define __APM_SEC_TKN_H__

#include "apm_sec_access.h"	/* for link list and data tytpe */

#define TKN_ALIGN16(x)		(((unsigned long) (x) + 15) & ~0xF)

struct sec_tkn_result_hdr {	/* All field is in little endian */
#if defined(__LITTLE_ENDIAN)
	unsigned int EXX	:15;	/* E0 - E14 */
	unsigned int pkt_len	:17;

	unsigned int L		:1;
	unsigned int N		:1;
	unsigned int C		:1;
	unsigned int B		:1;
	unsigned int hash_bytes	:4;
	unsigned int H		:1;
	unsigned int rsvd1	:12;
	unsigned int E15	:1;
	unsigned int bypass_data:4;
#else
	unsigned int pkt_len	:17;
	unsigned int EXX	:15;	/* E0 - E14 */

	unsigned int bypass_data:4;
	unsigned int E15	:1;
	unsigned int rsvd1	:16;
	unsigned int H		:1;
	unsigned int hash_bytes	:6;
	unsigned int B		:1;
	unsigned int C		:1;
	unsigned int N		:1;
	unsigned int L		:1;
#endif

	unsigned int out_pkt_ptr;	/**< Copied from input token */
#if defined(__LITTLE_ENDIAN)
	unsigned short rsvd2;
	unsigned char pad_length;
	unsigned char next_hdr_field;
#else
	unsigned char next_hdr_field;
	unsigned char pad_length;
	unsigned short rsvd2;
#endif
	unsigned int bypass_tkn_data[4];/**< Optional bypass data 0 - 4 */

} __attribute__((packed));

#define TKN_RESULT_HDR_MAX_LEN	\
		((sizeof(struct sec_tkn_result_hdr) + 15) & ~0xF)
#define TKN_RESULT_HDR_LEN	\
		(sizeof(struct sec_tkn_result_hdr) - 4*4)

#define TKN_RESULT_E7           0x0001
#define TKN_RESULT_E8           0x0002
#define TKN_RESULT_E9           0x0004
#define TKN_RESULT_E10          0x0008
#define TKN_RESULT_E11          0x0010
#define TKN_RESULT_E12          0x0020
#define TKN_RESULT_E13          0x0040
#define TKN_RESULT_E14          0x0080
#define TKN_RESULT_E1           0x0100
#define TKN_RESULT_E2           0x0200
#define TKN_RESULT_E3           0x0400
#define TKN_RESULT_E4           0x0800
#define TKN_RESULT_E5           0x1000
#define TKN_RESULT_E6           0x2000

/*
 * Variouse Index into token
 */
#define sec_tkn_sa_ptr_idx      3   /**< Index of token for SA pointer */
#define sec_tkn_ctrl0_idx       4   /**< Index of token for SA Control WORD0 */
#define sec_tkn_ctrl1_idx       5   /**< Index of token for SA Control WORD1 */
#define sec_tkn_1st_instr_idx   6   /**< Index of token for 1st instructions with SA control WORD0,1 */
/**
 * @struct sec_tkn_input_hdr
 * @brief   This is the minimum input token structure format.
 *
 */
struct sec_tkn_input_hdr {	/* All field is in little endian */
#if defined(__LITTLE_ENDIAN)
	unsigned int pkt_len:17;	/**< Input packet length */
	unsigned int rsvd1:1;
	unsigned int CT:2;	/**< Context Type - Set TKN_CT_XXX */
	unsigned int RC:2;	/**< Re-use context - See TKN_RC_XXX */
	unsigned int ToO:3;	/**< Type of output - See TKN_TOO_XXX */
	unsigned int C:1;	/**< Context Control words present if set to 1*/
	unsigned int IV:3;	/**< IV usage and selection */
	unsigned int U:1;	/**< Checksum present if set to 1 */
	unsigned int rsvd2:1;
	unsigned int rsvd3:1;
	unsigned int tkn_length:14;
	unsigned int disable_dup_ctx:1;
	unsigned int num_ctx_updates:4;
	unsigned int rsvd4:13;
#else
	unsigned int rsvd3:1;
	unsigned int rsvd2:1;
	unsigned int U:1;	/**< Checksum present if set to 1 */
	unsigned int IV:3;	/**< IV usage and selection.
				     See SafeNet IV register usage table. */
	unsigned int C:1;	/**< Context Control word present if set to 1 */
	unsigned int ToO:3;	/**< Type of output - See TKN_TOO_XXX */
	unsigned int RC:2;	/**< Re-use context - See TKN_RC_XXX */
	unsigned int CT:2;	/**< Context Type - Set TKN_CT_XXX */
	unsigned int rsvd1:1;
	unsigned int pkt_len:17;/**< Input packet length */
#endif
	unsigned int rsvd4:13;
	unsigned int num_ctx_updates:4;
	unsigned int rsvd5:1;
	unsigned int tkn_length:14;
	unsigned int rsvd6;
	unsigned int ctx_ptr;
	
} __attribute__((packed));

#define TKN_INPUT_HDR_LEN       (sizeof(struct sec_tkn_input_hdr))

/**
 * @brief   Security Token Context
 * @note
 * Due the the natural of security token variable length fields, it is better 
 * to operate on an array of bytes. Input token musts be 16-bytes align. Output
 * token musts be 16 bytes align. 
 *
 * Format of Input and Output Token:
 *
 * ------------------
 * - Software core  - software core variable - 16 bytes address aligned
 * - variables      -
 * ------------------
 * - Output Token   - see sec_tkn_result_hdr - 16 bytes address aligned
 * ------------------
 * - Input Token    - see sec_tkn_input_hdr - 16 bytes address aligned
 * -                - This is variable length. The begin of input token
 * -                - is a configurable value of X DWORD. At this moment, it
 * -                - is 32 bytes
 * ------------------
 *
 */
struct sec_tkn_ctx {
	struct list_head next;		/**< Next link for list chain purpose */
	unsigned char  sa_len_dw;	/**< # of SA DWORD */
	unsigned short input_tkn_len;	/**< Input token length in bytes */
	void *context;			/**< A pointer context */

	void *qmsg;
	int dest_nbytes;
	unsigned char  esp_src_len;	/**<calculated len to do the flsuh the esp pkt(for aes) */
	void *dest_mem;
	struct sec_sa_item *sa;		/**< SA pointer */
	void *result_tkn_ptr;		/**< Point to beginning of output
					     token (16-bytes aligned) */
	/* See note above */
	/* sec_tkn_result_hdr 	result_tkn; */
	/* sec_tkn_input_hdr_t  in_tkn; */
};

#define TKN_CTX_RESULT_TKN_COMPUTE(t)	\
		((void *) TKN_ALIGN16((unsigned char *) (t) + \
					 sizeof(struct sec_tkn_ctx)))
#define TKN_CTX_SIZE(in_tkn_size)   	\
		(TKN_ALIGN16(sizeof(struct sec_tkn_ctx)) + \
		 TKN_RESULT_HDR_MAX_LEN + TKN_ALIGN16(in_tkn_size))
#define TKN_CTX_RESULT_TKN(t)           \
		((struct sec_tkn_result_hdr *) (t)->result_tkn_ptr)
#define TKN_CTX_INPUT_TKN(t)    	\
		((struct sec_tkn_input_hdr *) \
		 ((char *) (t)->result_tkn_ptr + TKN_RESULT_HDR_MAX_LEN))
#define TKN_CTX_RESULT_TKN_OFFSET(t)    \
		((unsigned long) (t)->result_tkn_ptr - (unsigned long) (t))
#define TKN_CTX__INPUT_TKN_OFFSET(t)	\
		(TKN_CTX_TKN_RESULT_TKN_OFFSET(t) + TKN_RESULT_HDR_MAX_LEN)

#ifdef APM_SEC_SATKNDEBUG
void sec_tkn_dump(struct sec_tkn_ctx *tkn);
#endif

/**
 * @brief Create token header (first 4 DWORD's)
 * @param   t       A pointer to input token
 * @param   tl      Variable to store token length counter
 * @param   u       U field of token header. If set, checksum is supplied
 *			from token
 * @param   iv      IV field of token header. See SafeNet User Manual
 *			7.2.1.6 IV:Usage and Selection
 * @param   c       C field of token header. Context control WORD0,1 from
 *			token
 * @param   too     ToO field of token header. See IPE_TKN_TOO_XXX or
 *			SafeNet User Manual
 * @param   rc      RC field of token header. See IPE_TKN_RC_XXX or SafeNet
 *			User Manual
 * @param   ct      CT field of token header. 0 for single IPE. 1 for shared
 *			context.
 * @param   pl      Input packet length in bytes
 * @param   sa      SA address in 32-bit physical address format. See
 * 			function ipe_sa_va2indirectmap.
 * @note  Token header consists of:
 *          Token Control DWORD             - 1st DWORD
 *          Token input packet address      - 2nd DWORD which is always 0
 *          Token output packet address     - 3rd DWORD which is always 0
 *          Token context pointer address   - 4th DWORD which is physical
 *						address
 *
 *
 */
#define sec_tkn_set_hdr(t, tl, u, iv, c, too, rc, ct, pl, cu, tknlen, sa) \
{ \
	sec_tkn_set32le(&t[0], \
			  (u) << 29 | (iv) << 26 | (c) << 25 | (too) << 22 | \
		    	  (rc) << 20 | (ct) << 18 | (pl)); \
	sec_tkn_set32le(&t[1], \
			(cu) << 15 | (tknlen) );\
	/* SA (security association address */ \
	sec_tkn_set32le(&t[3], ((u32)sa)); \
	tl = 4; \
}

#define sec_tkn_update_hdr(t, u, iv, c, too, rc, ct, pl) 	\
	sec_tkn_set32le((t), \
			  (u) << 29 | (iv) << 26 | (c) << 25 | (too) << 22 | \
		    	  (rc) << 20 | (ct) << 18 | (pl))

/* Max # of byte in SA for basic encrypt/decrypt and hashing algorithms
 *
 * Crypto: Tkn Header, Input pointer, Output pointer, Context pointer,
 *         control W0, control W1, IV03 (CBC and CTR only),
 *	   two token instructions
 * Hash:   Tkn Header, Input pointer, Output pointer, Context pointer,
 *         control W0, control W1, 3 token instructionss
 */
#define TKN_BASIC_MAX_LEN	(6*4+6*4)
#define TKN_AES_GCM_MAX		(16*4 + 2*4)
#define TKN_AES_CCM_MAX		(4*25)

#define IPE_INTKN_AUTHENC_MAX_SIZE	((4+2+6)<<2)
#define IPE_TKN_ESP_TRANSPORT_MAXSIZE	((4+2+8) << 2)
#define IPE_TKN_ESP_TRANSPORT_GCM_MAXSIZE	((4+2+11) << 2)

/*
 * MACSec Token Templates
 */
#define IPE_INTKN_MACSEC_MAX_SIZE	((4+2+12)<<2)
#define IPE_INTKN_AES_CCM		1
#define IPE_INTKN_AES_GMAC    		2
#define IPE_INTKN_AES_GCM      		3
static inline void sec_tkn_set32le(unsigned int *tkn, unsigned int v)
{
	*tkn = cpu_to_le32(v);
}

static inline void sec_tkn_set32be(unsigned int *tkn, unsigned int v)
{
	*tkn = cpu_to_be32(v);
}

static inline void sec_tkn_setle(unsigned int *tkn, const unsigned char *buf,
				   int len)
{
	for (; len > 0; buf += 4, len -= 4) {
		*tkn++ = cpu_to_le32(*(unsigned int *) buf);
	}
}

static inline void sec_tkn_setbe2le(unsigned int *tkn, 
				    const unsigned char *buf, int len)
{
	/* Set network byte order (big endian) to little endian */
	/* We must swap regardless of system */
	for (; len > 0; buf += 4, len -= 4) {
#if defined(__LITTLE_ENDIAN)
		*tkn++ = cpu_to_be32(*(unsigned int *) buf);
#else
		*tkn++ = cpu_to_le32(*(unsigned int *) buf);
#endif
	}
}

static inline void sec_kn_seth2h(unsigned int *tkn,
				    const unsigned char *buf, int len)
{
	memcpy(tkn, buf, len);
}

static inline unsigned int sec_tkn_getle32h(unsigned int *tkn)
{
	return le32_to_cpu(*tkn);
}

static inline void sec_tkn_setbe(unsigned int *tkn,
				   const unsigned char *buf, int len)
{
	for (; len > 0; buf += 4, len -= 4) {
		*tkn++ = cpu_to_be32(*(unsigned int *) buf);
	}
}

static inline void sec_tkn_setle32h(unsigned int *in, unsigned int len,
				      unsigned int *out)
{
	for (; len > 0; len -= 4) {
		*out++ = le32_to_cpu(*(unsigned int *) in++);
	}
}

/**
 * @brief Create input token for basic hashing operation
 * @param t	A pointer to input token
 * @param pl	Payload byte length to operation
 * @param ctrl  Two control control WORD
 * @param sa	MSB 32-bit of a 36-bits physical address format. See function
 *			apm_sa_va2hw.
 * @param ds	Digest size in bytes
 * @return Return total number token byte required
 */
u16 sec_tkn_set_hash(u32 *t, u32 pl, u32 *ctrl, u32 sa, u16 ds);

/**
 * @brief Update input token for basic hashing operation
 * @param t	A pointer to input token
 * @param pl	Payload byte length to encrypt
 *
 */
void sec_tkn_update_hash(u32 *t, u32 pl);				  

/**
 * @brief Create input token for basic encrypt/decrypt operation
 * @param t	A pointer to input token
 * @param pl	Payload byte length to encrypt
 * @param sa	MSB 32-bit of a 36-bits physical address format. See function
 *			apm_sa_va2hw.
 * @param ca	Crypto algorithm to use. See SA_CRYPTO_ALG_XXX
 * @param cm	Crypto mode to use. See SA_CRYPTO_MODE_XXX
 * @param nonce Nonce value in SA format for CTR mode
 * @param iv	Pointer to IV byte stream (16 bytes) for CBC and CTR mode
 * @param ctrl  Two control control WORD
 * @return Return total number token byte required
 * @note For ECB, OFB, and CFB, there is no nonce or iv.
 *       For CBC, IV0 and IV1 is loaded from SA and override by token IV0
 *	 and IV1.
 *       For CTR, IV0 - IV3 is loaded from SA and override by token IV0 -
 *	 IV3.
 */
u16 sec_tkn_set_crypt(u32 *t, u32 pl, u32 sa, u32 *ctrl, u8 ca, u8 cm,
		u32 *nonce, u8 *iv, int enc, u32 meraki_mode);

/**
 * @brief Update input token for basic encrypt/decrypt operation
 * @param t	A pointer to input token
 * @param pl	Payload byte length to encrypt
 * @param iv	For CBC mode, pointer to IV byte stream (16 bytes)
 * @note For ECB, OFB, and CFB, there is no nonce or iv.
 *       For CBC, IV0 and IV1 is loaded from SA and override by token IV0
 *	 and IV1.
 *       For CTR, IV0 - IV3 is loaded from SA and override by token IV0 -
 *	 IV3.
 */
void sec_tkn_update_crypt(u32 *t, u32 pl, u32 *ctrl, u8 ca, u8 cm, u8 *iv,
			   int enc);

/**
 * @brief Create input token for gcm encrypt/decrypt operation
 * @param t	A pointer to input token
 * @param pl	Payload byte length to encrypt
 * @param sa	MSB 32-bit of a 36-bits physical address format. See function
 *			apm_sa_va2hw.
 * @param ca	Crypto algorithm to use. See SA_CRYPTO_ALG_XXX
 * @param cm	Crypto mode to use. See SA_CRYPTO_MODE_XXX
 * @param nonce Nonce value in SA format for CTR mode
 * @param iv	Pointer to IV byte stream (16 bytes) for CBC and CTR mode
 * @param ctrl  Two control control WORD
 * @return Return total number token byte required
 */
unsigned short sec_tkn_set_crypt_aes_gcm(unsigned int *t,
			unsigned int pl, unsigned int sa, unsigned int *ctrl,
			unsigned char *adata, unsigned char assoclen,
			unsigned char icv_len, int enc);
/**
 * @brief Update input token for  gcm encrypt/decrypt operation
 * @param t	A pointer to input token
 * @param pl	Payload byte length to encrypt
 * @param iv	For CBC mode, pointer to IV byte stream (16 bytes)
 */
unsigned short sec_tkn_update_crypt_aes_gcm(unsigned int *t,
		unsigned int pl,unsigned int sa, unsigned int  *ctrl,
		unsigned char *adata, unsigned char assoclen,
		unsigned char icv_len, int enc);
/**
 * @brief Create input token for ccm encrypt/decrypt operation
 * @param t	A pointer to input token
 * @param pl	Payload byte length to encrypt
 * @param sa	MSB 32-bit of a 36-bits physical address format. See function
 *			apm_sa_va2hw.
 * @param ca	Crypto algorithm to use. See SA_CRYPTO_ALG_XXX
 * @param cm	Crypto mode to use. See SA_CRYPTO_MODE_XXX
 * @param nonce Nonce value in SA format for CTR mode
 * @param iv	Pointer to IV byte stream (16 bytes) for CBC and CTR mode
 * @param ctrl  Two control control WORD
 * @return Return total number token byte required
 */
unsigned short sec_tkn_set_crypt_aes_ccm(unsigned int *t, unsigned int pl,
		unsigned int sa, unsigned int *ctrl, unsigned int icv_size,
		unsigned int *iv_B0, unsigned char *adata_length,
		unsigned int adata_length_length, unsigned char *adata,
		unsigned char assoclen, int enc);

/**
 * @brief Update input token for  gcm encrypt/decrypt operation
 * @param t	A pointer to input token
 * @param pl	Payload byte length to encrypt
 * @param iv	For CBC mode, pointer to IV byte stream (16 bytes)
 */
unsigned short sec_tkn_update_crypt_aes_ccm(unsigned int *t,
		unsigned int pl, unsigned int sa, unsigned int *ctrl,
		unsigned int icv_size, unsigned int *iv_B0,
		unsigned char *adata_length, unsigned int adata_length_length,
		unsigned char *adata, unsigned char assoclen, int enc);
/**
 * @brief Create input token for  arc4 encrypt/decrypt operation
 * @param t	A pointer to input token
 * @param pl	Payload byte length to encrypt
 * @param sa	MSB 32-bit of a 36-bits physical address format. See function
 *			apm_sa_va2hw.
 * @param ctrl  Two control control WORD
 * @return Return total number token byte required
 */

unsigned short sec_tkn_set_arc4_crypt(unsigned int *t, unsigned int pl,
				      unsigned int sa, unsigned int *ctrl, int enc);
/**
 * @brief Update input token for  gcm encrypt/decrypt operation
 * @param t	A pointer to input token
 * @param pl	Payload byte length to encrypt
 * @param iv	For CBC mode, pointer to IV byte stream (16 bytes)
 */
void sec_tkn_update_arc4(unsigned int *t, unsigned int pl);
/**
 * @brief Create ESP outbound token for tunnel mode
 *
 * @param   t       Token pointer (DWORD pointer)
 * @param   tl      Variable to store token length counter
 * @param   pl      Packet length in bytes
 * @param   sa      Security context association physical address (32-bits)
 * @param   ipv6    For IPv6, set to 1. For IPv4, set to 0
 * @param   ethl    Ethernet header length in bytes
 * @param   iphl    IP header field length in bytes
 * @param   oiphl   Outer IP header length in bytes
 * @param   oiph    Outer IP header array of byte to insert
 * @param   ca      Crypto alg (IPE_SA_CRYPTO_ALG_XXX)
 * @param   cm      Crypto mode (IPE_SA_CRYPTO_MODE_AES_XXX)
 * @param   ha      Hash alg (IPE_SA_HASH_ALG_XXX - no XCBC, no GHASH, no
 *		     KASUMI)
 * @param   esn     Enable extend seq num if 1 - 64-bit
 * @param   pdl     Padding length in bytes - IPSec padding length
 * @param   iv_prng Use PRNG for IV if 1
 * @param   gcm_icv_len For GCM mode, set ICV length to non-zero (valid=8,
 *		        12, 16)
 * @return Token length in byte
 *
 * @note Security Context (SA) Format:
 *      0xCTRLW0    - Control Word 0
 *      0xCTRLW1    - Control Word 1
 *      0xKEY_0-7   - Crypto Key 128, 192, and 256
 *      0xDIGEST0-1 - Hash Digest 0, 1 - depend on hash alg
 *      0xSPI       - SPI
 *      0xSEQ0-1    - Seq number and extend seq (See seq01)
 *      0xIV0-3     - IV 0,1,2,3 - depend on crypto alg
 */
unsigned short sec_tkn_set_espout_tunnel(unsigned int *t,
		unsigned int pl, unsigned int sa, unsigned int *ctrl,
		unsigned char ca, unsigned char cm, unsigned char pdl,
		unsigned char gcm_flag, unsigned char seq_offset);
/**
 * @brief Update ESP outbound token for tunnel mode
 *
 * @param   t       Token pointer (DWORD pointer)
 * @param   pl      Packet length in bytes
 * @param   ipv6    For IPv6, set to 1. For IPv4, set to 0
 * @param   ethl    Ethernet header length in bytes
 * @param   iphl    IP header field length in bytes
 * @param   oiphl   Outer IP header length in bytes
 * @param   oiph    Outer IP header array of byte to insert
 * @param   pdl     Padding length in bytes - IPSec padding length
 * @param   gcm_icv_len For GCM mode, set ICV length to non-zero (valid=8, 12,
 *			16)
 *
 */		
void sec_tkn_update_espout_tunnel(unsigned int *t, unsigned int pl,
		unsigned char cm, unsigned int icv_len, unsigned char pad_len,
		unsigned char gcm_flag);
/**
 * @brief Create ESP inbound token for tunnel mode
 *
 * @param   t       Token pointer (DWORD pointer)
 * @param   tl      Variable to store token length counter
 * @param   pl      Packet length in bytes
 * @param   sa      Security context association physical address (32-bits)
 * @param   ipv6    For IPv6, set to 1. For IPv4, set to 0
 * @param   ca      Crypto alg (IPE_SA_CRYPTO_ALG_XXX)
 * @param   cm      Crypto mode (IPE_SA_CRYPTO_MODE_AES_XXX)
 * @param   ha      Hash alg (IPE_SA_HASH_ALG_XXX - no XCBC, no GHASH, no
 *		     KASUMI)
 * @param   esn     Enable extend seq num if 1 - 64-bit
 * @param   mask01  SEQ number mask bit of SA CTRL WORD 0 (00=no mask;
 *		     01/10=Mask0,1; 11=Mask0,1,2,3)
 * @param   ethl    Ethernet header length in bytes
 * @param   oiphl   Outer IP header length in bytes
 * @param   pdl     Padding length in bytes - IPSec padding length
 * @param   iv_prng Use PRNG for IV if 1
 * @param   gcm_icv_len For GCM mode, set ICV length to non-zero (valid=8, 12,
 *			16)
 * @return Token length in byte
 *
 * @note Security Context (SA) Format:
 *      0xCTRLW0    - Control Word 0
 *      0xCTRLW1    - Control Word 1
 *      0xKEY_0-7   - Crypto Key 128, 192, and 256
 *      0xDIGEST0-1 - Hash Digest 0, 1 - depend on hash alg
 *      0xSPI       - SPI
 *      0xSEQ0-1    - Seq number and extend seq (See seq01)
 *      0xMASK0-3   - Seq number mask (see mask01)
 *      0xIV0-3     - IV 0,1,2,3 - depend on crypto alg
 */
 unsigned short sec_tkn_set_espin_tunnel(unsigned int *t,
		unsigned int pl, unsigned int sa, unsigned int *ctrl,
		unsigned char ca, unsigned char cm,
		unsigned char gcm_flag, unsigned int ihl,
		unsigned char icv_len, unsigned char seq_offset);

/**
  * @brief Update ESP outbound token for tunnel mode
  *
  * @param   t       Token pointer (DWORD pointer)
  * @param   pl      Packet length in bytes
  * @param   ipv6    For IPv6, set to 1. For IPv4, set to 0
  * @param   ethl    Ethernet header length in bytes
  * @param   oiphl   Outer IP header length in bytes
  * @param   gcm_icv_len For GCM mode, set ICV length to non-zero (valid=8, 12,
  *			16)
 */
 void sec_tkn_update_espin_tunnel(unsigned int *t, unsigned int pl,
				  unsigned char cm,
				  unsigned char ca, unsigned int icvsize,
				  unsigned int gcm_flag);
/**
 * @brief Create ESP outbound token for transport mode AES/DES/DES3/AES(CTR)
 *
 * @param   t       Token pointer (DWORD pointer)
 * @param   tl      Variable to store token length counter
 * @param   pl      Packet length in bytes
 * @param   sa      Security context association physical address (32-bits)
 * @param   ca      Crypto alg (IPE_SA_CRYPTO_ALG_XXX)
 * @param   pdl     Padding length in bytes - IPSec padding length
 * @param   icvsize Authentication length calculated for payload + esp data
 * @param   nh      Next Header value to be placed in the ESP trailer
 * @param   seq_offset Offset in the Control word where the upadted
			sequence is to be placed
 * @return Token length in byte
 *
 * @note Security Context (SA) Format:
 *      0xCTRLW0    - Control Word 0
 *      0xCTRLW1    - Control Word 1
 *      0xKEY_0-7   - Crypto Key 128, 192, and 256
 *      0xDIGEST0-1 - Hash Digest 0, 1 - depend on hash alg
 *      0xSPI       - SPI
 *      0xSEQ0-1    - Seq number and extend seq (See seq01)
 *      0xIV0-3     - IV 0,1,2,3 - depend on crypto alg
 *	This token set routine works for AES/DES/DES3/AES(CTR) Algorithms
 *
 */
unsigned short sec_tkn_set_espout_transport(unsigned int *t,
		unsigned int pl, unsigned int sa, unsigned int *ctrl,
		unsigned char ca, unsigned char pdl, unsigned char icvsize,
		unsigned char nh, unsigned char iphl, unsigned char seq_offset,
		unsigned char cm, int gcm_flag);
 
/**
  * @brief Update ESP outbound token for transport mode AES/DES/DES3/AES(CTR)
  *
  * @param   t       Token pointer (DWORD pointer)
  * @param   tl      Variable to store token length counter
  * @param   pl      Packet length in bytes
  * @param   ca      Crypto alg (IPE_SA_CRYPTO_ALG_XXX)
  * @param   pdl     Padding length in bytes - IPSec padding length
  * @param   icvsize Authentication length calculated for payload + esp data
  * @param   nh      Next Header value to be placed in the ESP trailer
  */
void sec_tkn_update_espout_transport(unsigned int *t, unsigned int pl,
		unsigned char ca, unsigned char pdl, unsigned char nh,
		unsigned char iphl, unsigned char icvsize,
		unsigned char cm, int gcm_flag);

/**
 * @brief Create ESP inbound token for transport mode AES/DES/DES3/AES(CTR)
 *
 * @param   t       Token pointer (DWORD pointer)
 * @param   tl      Variable to store token length counter
 * @param   pl      Packet length in bytes
 * @param   sa      Security context association physical address (32-bits)
 * @param   ca      Crypto alg (IPE_SA_CRYPTO_ALG_XXX)
 * @param   pdl     Padding length in bytes - IPSec padding length
 * @param   icvsize Authentication length verified for payload + esp data
 * @param   seq_offset Offset in the Control word where the upadted
 *			sequence is to be placed
 * @return Token length in byte
 *
 * @note Security Context (SA) Format:
 *      0xCTRLW0    - Control Word 0
 *      0xCTRLW1    - Control Word 1
 *      0xKEY_0-7   - Crypto Key 128, 192, and 256
 *      0xDIGEST0-1 - Hash Digest 0, 1 - depend on hash alg
 *      0xSPI       - SPI
 *      0xSEQ0-1    - Seq number and extend seq (See seq01)
 *      0xIV0-3     - IV 0,1,2,3 - depend on crypto alg
 *
 *	This token works for AES/DES/DES3/AES(CTR) Algorithms
 *
 */
unsigned short sec_tkn_set_espin_transport(unsigned int *t, unsigned int pl,
		unsigned int sa, unsigned int *ctrl, unsigned char ca,
		unsigned int ihl, unsigned char icv_len, 
		unsigned char seq_offset, unsigned char cm, int gcm_flag);
/**
  * @brief Update ESP inbound token for transport mode AES/DES/DES3/AES(CTR)
  *
  * @param   t       Token pointer (DWORD pointer)
  * @param   tl      Variable to store token length counter
  * @param   pl      Packet length in bytes
  * @param   ca      Crypto alg (IPE_SA_CRYPTO_ALG_XXX)
  * @param   icvsize Authentication length verified for payload + esp data
  */
 void sec_tkn_update_espin_transport(unsigned int *t,
			     unsigned int pl,
			     unsigned char ca,
			     unsigned short ihl,
			     unsigned char icv_len, unsigned char cm, int gcm_flag);
 /**
  * Outbound ESP Transport Update Token for CCM
  *
  */
 /**
  * Outbound ESP Transport Create Token for CCM
  *
  */
unsigned short sec_tkn_set_espout_transport_ccm(unsigned int *t,
		 unsigned int pl, unsigned int sa,
		 unsigned int *ctrl, unsigned char pdl,
		 unsigned char icvsize, unsigned char nh,
		 unsigned char iphl, unsigned char *iv_B0,
		 unsigned char *adata_length, unsigned char seq_offset);
unsigned short sec_tkn_update_espout_transport_ccm(unsigned int *t,
		 unsigned int pl, unsigned int sa, unsigned char pdl,
		 unsigned char icvsize, unsigned char nh, unsigned char iphl,
		 unsigned char *iv_B0, unsigned char *adata_length);
unsigned short sec_tkn_set_espin_transport_ccm(unsigned int *t,
		 unsigned int pl, unsigned int sa, unsigned int *ctrl,
		 unsigned char pdl, unsigned char icvsize,
		 unsigned char nh, unsigned char *iv_B0,
		 unsigned char *adata_length, unsigned char seq_offset);
#endif
