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
 * This file defines the security context
 * assoicate format.
 */

#ifndef __CRYPTO4XX_SA_H__
#define __CRYPTO4XX_SA_H__

#define AES_IV_SIZE				16

/**
 * Contents of Dynamic Security Association (SA) with all possible fields
 */
union dynamic_sa_contents {
	struct {
		u32 arc4_state_ptr:1;
		u32 arc4_ij_ptr:1;
		u32 state_ptr:1;
		u32 iv3:1;
		u32 iv2:1;
		u32 iv1:1;
		u32 iv0:1;
		u32 seq_num_mask3:1;
		u32 seq_num_mask2:1;
		u32 seq_num_mask1:1;
		u32 seq_num_mask0:1;
		u32 seq_num1:1;
		u32 seq_num0:1;
		u32 spi:1;
		u32 outer_size:5;
		u32 inner_size:5;
		u32 key_size:4;
		u32 cmd_size:4;
	} bf;
	u32 w;
} __attribute__((packed));

#define SA_OPCODE_ESP                   	0
#define SA_OPCODE_AH                    	1
#define SA_OPCODE_SSL                   	4
#define SA_OPCODE_TLS                   	5
#define SA_OPCODE_SRTP                  	7
#define SA_OPCODE_DTLS                  	1
#define SA_OPCODE_TLS1_1                	6

#define SA_OP_GROUP_BASIC               	0
#define SA_OP_GROUP_PROTOCOL            	1
#define SA_OP_GROUP_EXTEND_PROTOCOL     	3

#define SA_OPCODE_EXT_PROT_DTLS         	1
#define SA_OPCODE_EXT_PROT_MACSEC       	2
#define SA_OPCODE_EXT_PROT_SSL          	4
#define SA_OPCODE_EXT_PROT_TLS10        	5
#define SA_OPCODE_EXT_PROT_TLS11        	6

#define DIR_OUTBOUND				0
#define DIR_INBOUND				1
#define SA_OPCODE_ENCRYPT			0
#define SA_OPCODE_DECRYPT			0
#define SA_OPCODE_ENCRYPT_HASH          	1
#define SA_OPCODE_HASH_DECRYPT			1
#define SA_OPCODE_HASH				3
#define SA_OPCODE_HASH_ENCRYPT          	4
#define SA_OPCODE_DECRYPT_HASH			4

#define SA_OPCODE_ESP                   	0
#define SA_OPCODE_AH                    	1
#define SA_OPCODE_SSL                   	4
#define SA_OPCODE_TLS                   	5
#define SA_OPCODE_SRTP                  	7
#define SA_OPCODE_DTLS                  	1
#define SA_OPCODE_TLS1_1                	6

#define SA_CIPHER_ALG_DES			0
#define SA_CIPHER_ALG_3DES			1
#define SA_CIPHER_ALG_ARC4			2
#define SA_CIPHER_ALG_AES			3
#define SA_CIPHER_ALG_KASUMI			4
#define SA_CIPHER_ALG_NULL			15

#define SA_HASH_ALG_MD5				0
#define SA_HASH_ALG_SHA1			1
#define SA_HASH_ALG_SHA224              	2
#define SA_HASH_ALG_SHA256              	3
#define SA_HASH_ALG_SHA384              	4
#define SA_HASH_ALG_SHA512              	5
#define HASH_ALG_MAX_CNT			6
#define SA_HASH_ALG_AES_XCBC_MAC_128    	8
#define SA_HASH_ALG_KASUMI_f9           	9
#define SA_HASH_ALG_GHASH			12
#define SA_HASH_ALG_GMAC        		13
#define SA_HASH_ALG_CBC_MAC			14
#define SA_HASH_ALG_NULL			15

#define SA_LOAD_HASH_FROM_SA			0
#define SA_LOAD_HASH_FROM_STATE			2
#define SA_NOT_LOAD_HASH			3
#define SA_LOAD_IV_FROM_SA			0
#define SA_LOAD_IV_FROM_INPUT			1
#define SA_LOAD_IV_FROM_STATE			2
#define SA_LOAD_IV_GEN_IV			3

#define SA_PAD_TYPE_CONSTANT			2
#define SA_PAD_TYPE_ZERO			3
#define SA_PAD_TYPE_TLS				5
#define SA_PAD_TYPE_DTLS			5
#define SA_NOT_SAVE_HASH			0
#define SA_SAVE_HASH				1
#define SA_NOT_SAVE_IV				0
#define SA_SAVE_IV				1
#define SA_GEN_IV				3

#define SA_HEADER_PROC				1
#define SA_NO_HEADER_PROC			0

#define SA_HASH_ALG_MD5_DIGEST_SIZE		16
#define SA_HASH_ALG_SHA1_DIGEST_SIZE		20
#define SA_HASH_ALG_SHA224_DIGEST_SIZE		28
#define SA_HASH_ALG_SHA256_DIGEST_SIZE		32
#define SA_HASH_ALG_SHA384_DIGEST_SIZE		48
#define SA_HASH_ALG_SHA512_DIGEST_SIZE		64

#define CRYPTO4XX_CRYPTO_PRIORITY_IPSEC		300

#define CRYPTO4XX_MAC_ALGS	{ "md5", "sha1", \
				"sha224", "sha256", "sha384", "sha512" }
union sa_command_0 {
	struct {
		u32 scatter:1;
		u32 gather:1;
		u32 save_hash_state:1;
		u32 save_iv:1;
		u32 load_hash_state:2;
		u32 load_iv:2;
		u32 digest_len:4;
		u32 hdr_proc:1;
		u32 extend_pad:1;
		u32 stream_cipher_pad:1;
		u32 rsv:1;
		u32 hash_alg:4;
		u32 cipher_alg:4;
		u32 pad_type:2;
		u32 op_group:2;
		u32 dir:1;
		u32 opcode:3;
	} bf;
	u32 w;
} __attribute__((packed));

#define CRYPTO_MODE_ECB				0
#define CRYPTO_MODE_KASUMI			0
#define CRYPTO_MODE_CBC				1
#define CRYPTO_MODE_OFB                  	2
#define CRYPTO_MODE_CFB                  	3
#define CRYPTO_MODE_AES_CTR              	4
#define CRYPTO_MODE_KASUMI_f8            	4
#define CRYPTO_MODE_AES_ICM              	5

#define CRYPTO_FEEDBACK_MODE_NO_FB		0
#define CRYPTO_FEEDBACK_MODE_64BIT_OFB		0
#define CRYPTO_FEEDBACK_MODE_8BIT_CFB		1
#define CRYPTO_FEEDBACK_MODE_1BIT_CFB		2
#define CRYPTO_FEEDBACK_MODE_128BIT_CFB		3

#define SA_AES_KEY_LEN_128			2
#define SA_AES_KEY_LEN_192			3
#define SA_AES_KEY_LEN_256			4

#define SA_REV2					1
/*
 * The follow defines bits sa_command_1
 * In Basic hash mode  this bit define simple hash or hmac.
 * In IPsec mode, this bit define muting control.
 */
#define SA_HASH_MODE_HASH			0
#define SA_HASH_MODE_HMAC			1
#define SA_MC_ENABLE				0
#define SA_MC_DISABLE				1
#define SA_NOT_COPY_HDR				0
#define SA_COPY_HDR				1
#define SA_NOT_COPY_PAD				0
#define SA_COPY_PAD				1
#define SA_NOT_COPY_PAYLOAD			0
#define SA_COPY_PAYLOAD				1
#define SA_EXTENDED_SN_OFF			0
#define SA_EXTENDED_SN_ON			1
#define SA_SEQ_MASK_OFF				0
#define SA_SEQ_MASK_ON				1

union sa_command_1 {
	struct {
		u32 crypto_mode31:1;
		u32 save_arc4_state:1;
		u32 arc4_stateful:1;
		u32 key_len:5;
		u32 hash_crypto_offset:8;
		u32 sa_rev:2;
		u32 byte_offset:1;
		u32 hmac_muting:1;
		u32 feedback_mode:2;
		u32 crypto_mode9_8:2;
		u32 extended_seq_num:1;
		u32 seq_num_mask:1;
		u32 mutable_bit_proc:1;
		u32 ip_version:1;
		u32 copy_pad:1;
		u32 copy_payload:1;
		u32 copy_hdr:1;
		u32 rsv1:1;
	} bf;
	u32 w;
} __attribute__((packed));

struct dynamic_sa_ctl {
	u32 sa_contents;
	union sa_command_0 sa_command_0;
	union sa_command_1 sa_command_1;
} __attribute__((packed));

/**
 * State Record for Security Association (SA)
 */
struct  sa_state_record {
	u32 save_iv[4];
	u32 save_hash_byte_cnt[2];
	u32 save_digest[16];
} __attribute__((packed));

/**
 * Arc4 State Record for Security Association (SA)
 */
struct arc4_sr {
	u32 arc4_state[64];
} __attribute__((packed));

/**
 * Security Association (SA) for DES
 */
struct dynamic_sa_des {
	struct dynamic_sa_ctl  ctrl;
	u32 key[2];
	u32 iv[2];
	u32 state_ptr;
	u32 reserved;
} __attribute__((packed));
#define SA_DES_LEN		(sizeof(struct dynamic_sa_des)/4)
#define SA_DES_CONTENTS         0x26000022

/**
 * Security Association (SA) for 3DES
 */
struct dynamic_sa_3des {
	struct dynamic_sa_ctl ctrl;
	u32 key[6];
	u32 iv[2]; /* for CBC, OFC, and CFB mode */
	u32 state_ptr;
	u32 reserved;
} __attribute__((packed));
#define SA_3DES_LEN		(sizeof(struct dynamic_sa_3des)/4)
#define SA_3DES_CONTENTS        0x26000062

/**
 * Security Association (SA) for AES128
 *
 */
struct dynamic_sa_aes128 {
	struct dynamic_sa_ctl	ctrl;
	u32 key[4];
	u32 iv[4]; /* for CBC, OFC, and CFB mode */
	u32 state_ptr;
	u32 reserved;
} __attribute__((packed));
#define SA_AES128_LEN		(sizeof(struct dynamic_sa_aes128)/4)
#define SA_AES128_CONTENTS	0x3e000042

/**
 * Security Association (SA) for AES192
 */
struct dynamic_sa_aes192 {
	struct dynamic_sa_ctl ctrl;
	u32 key[6];
	u32 iv[4]; /* for CBC, OFC, and CFB mode */
	u32 state_ptr;
	u32 reserved;
} __attribute__((packed));
#define SA_AES192_LEN		(sizeof(struct dynamic_sa_aes192)/4)
#define SA_AES192_CONTENTS	0x3e000062

/**
 * Security Association (SA) for AES256
 */
struct dynamic_sa_aes256 {
	struct dynamic_sa_ctl ctrl;
	u32 key[8];
	u32 iv[4]; /* for CBC, OFC, and CFB mode */
	u32 state_ptr;
	u32 reserved;
} __attribute__((packed));

#define SA_AES256_LEN		(sizeof(struct dynamic_sa_aes256)/4)
#define SA_AES256_CONTENTS	0x3e000082
#define SA_AES_CONTENTS		0x3e000002

/**
 * Security Association (SA) for HASH128: HMAC-MD5
 */
struct dynamic_sa_hash128 {
	struct dynamic_sa_ctl ctrl;
	u32 inner_digest[4];
	u32 outer_digest[4];
	u32 state_ptr;
	u32 reserved;
} __attribute__((packed));
#define SA_HASH128_LEN		(sizeof(struct dynamic_sa_hash128)/4)
#define SA_HASH128_CONTENTS     0x20008402

/**
 * Security Association (SA) for HASH160: HMAC-SHA1
 */
struct dynamic_sa_hash160 {
	struct dynamic_sa_ctl ctrl;
	u32 inner_digest[5];
	u32 outer_digest[5];
	u32 state_ptr;
	u32 reserved;
} __attribute__((packed));
#define SA_HASH160_LEN		(sizeof(struct dynamic_sa_hash160)/4)
#define SA_HASH160_CONTENTS     0x2000a502

/**
 * Security Association (SA) for HASH256: HMAC-SHA224, HMAC-SHA256
 */
struct dynamic_sa_hash256 {
	struct dynamic_sa_ctl ctrl;
	u32 inner_digest[8];
	u32 outer_digest[8];
	u32 state_ptr;
	u32 reserved;
} __attribute__((packed));
#define SA_HASH256_LEN		(sizeof(struct dynamic_sa_hash256)/4)
#define SA_HASH256_CONTENTS     0x20010802

/*
 * Security Association (SA) for HASH512: HMAC-SHA512
 */
struct dynamic_sa_hash512 {
	struct dynamic_sa_ctl ctrl;
	u32 inner_digest[16];
	u32 outer_digest[16];
	u32 state_ptr;
	u32 reserved;
} __attribute__((packed));
#define SA_HASH512_LEN		(sizeof(struct dynamic_sa_hash512)/4)
#define SA_HASH512_CONTENTS     0x20021002

/**
 * Security Association (SA) for AES128_XCBC_MAC
 */
struct dynamic_sa_aes128_xcbc_mac {
	struct dynamic_sa_ctl ctrl;
	u32 key[4];
	u32 inner_digest[8];
	u32 outer_digest[8];
	u32 iv[4]; /* for CBC, OFC, and CFB mode */
	u32 state_ptr;
	u32 reserved;
} __attribute__((packed));
#define SA_AES128_XCBC_MAC_LEN	(sizeof(struct dynamic_sa_aes128_xcbc_mac)/4)
#define SA_AES128_XCBC_MAC_CONTENTS     0x3e010842

/**
 * Security Association (SA) for AES128_GCM
 */
struct dynamic_sa_aes128_gcm {
	struct dynamic_sa_ctl ctrl;
	u32 key[4];
	u32 inner_digest[4];
	u32 outer_digest[4];
	u32 spi;
	u32 seq;
	u32 iv[4]; /* for CBC, OFC, and CFB mode */
	u32 state_ptr;
	u32 reserved;
} __attribute__((packed));
#define SA_AES128_GCM_LEN	(sizeof(struct dynamic_sa_aes128_gcm)/4)
#define SA_AES128_GCM_CONTENTS          0x3e0c8442

/**
 * Security Association (SA) for AES192_XCBC_MAC
 */
struct dynamic_sa_aes192_xcbc_mac {
	struct dynamic_sa_ctl ctrl;
	u32 key[6];
	u32 inner_digest[8];
	u32 outer_digest[8];
	u32 iv[4]; /* for CBC, OFC, and CFB mode */
	u32 state_ptr;
	u32 reserved;
} __attribute__((packed));
#define SA_AES192_XCBC_MAC_LEN	(sizeof(struct dynamic_sa_aes192_xcbc_mac)/4)
#define SA_AES192_XCBC_MAC_CONTENTS     0x3e010862

/**
 * Security Association (SA) for AES192_GCM
 */
struct dynamic_sa_aes192_gcm {
	struct dynamic_sa_ctl ctrl;
	u32 key[6];
	u32 inner_digest[4];
	u32 outer_digest[4];
	u32 spi;
	u32 seq;
	u32 iv[4]; /* for CBC, OFC, and CFB mode */
	u32 state_ptr;
	u32 reserved;
} __attribute__((packed));
#define SA_AES192_GCM_LEN	(sizeof(struct dynamic_sa_aes192_gcm)/4)
#define SA_AES192_GCM_CONTENTS          0x3e0c8462


/**
 * Security Association (SA) for AES256_XCBC_MAC
 */
struct dynamic_sa_aes256_xcbc_mac {
	struct dynamic_sa_ctl ctrl;
	u32 key[8];
	u32 inner_digest[8];
	u32 outer_digest[8];
	u32 iv[4]; /* for CBC, OFC, and CFB mode */
	u32 state_ptr;
	u32 reserved;
} __attribute__((packed));
#define SA_AES256_XCBC_MAC_LEN	(sizeof(struct dynamic_sa_aes256_xcbc_mac)/4)
#define SA_AES256_XCBC_MAC_CONTENTS     0x3e010882

/**
 * Security Association (SA) for AES256_GCM
 */
struct dynamic_sa_aes256_gcm {
	struct dynamic_sa_ctl ctrl;
	u32 key[8];
	u32 inner_digest[4];
	u32 outer_digest[4];
	u32 spi;
	u32 seq;
	u32 iv[4]; /* for CBC, OFC, and CFB mode */
	u32 state_ptr;
	u32 reserved;
} __attribute__((packed));
#define SA_AES256_GCM_LEN	(sizeof(struct dynamic_sa_aes256_gcm)/4)
#define SA_AES256_GCM_CONTENTS          0x3e0c8482
#define SA_AES_GCM_CONTENTS          0x3e0c8402

/**
 * Security Association (SA) for Kasumi
 */
struct dynamic_sa_kasumi {
	struct dynamic_sa_ctl ctrl;
	u32 key[4];
	u32 state_ptr;
	u32 reserved;
} __attribute__((packed));
#define SA_KASUMI_LEN		(sizeof(struct dynamic_sa_kasumi)/4)
#define SA_KASUMI_CONTENTS              0x20000042

/**
 * Security Association (SA) for Kasumi f8
 */
struct dynamic_sa_kasumi_f8 {
	struct dynamic_sa_ctl ctrl;
	u32 key[4];
	u32 iv[2];
	u32 state_ptr;
	u32 reserved;
} __attribute__((packed));
#define SA_KASUMI_F8_LEN	(sizeof(struct dynamic_sa_kasumi_f8)/4)
#define SA_KASUMI_F8_CONTENTS           0x26000042

#define KASUMI_BLOCK_SIZE 8
#define KASUMI_KEY_SIZE   16

/**
 * Security Association (SA) for Kasumi f8
 */
struct dynamic_sa_kasumi_f9 {
	struct dynamic_sa_ctl ctrl;
	u32 inner_digest[4];
	u32 outter_digest[3];
	u32 state_ptr;
	u32 reserved;
} __attribute__((packed));
#define SA_KASUMI_F9_LEN	(sizeof(struct dynamic_sa_kasumi_f9)/4)
#define SA_KASUMI_F9_CONTENTS           0x20006402

/**
 * Security Association (SA) for AES256 CCM
 */
struct dynamic_sa_aes256_ccm {
	struct dynamic_sa_ctl ctrl;
	u32 key[8];
	u32 iv[4]; /* for CBC, OFC, and CFB mode */
	u32 state_ptr;
	u32 reserved;
} __attribute__((packed));
#define SA_AES256_CCM_LEN	(sizeof(struct dynamic_sa_aes256_ccm)/4)
#define SA_AES256_CCM_CONTENTS      0x3e000082
#define SA_AES_CCM_CONTENTS      0x3e000002

/**
 * Security Association (SA) for AES192 CCM
 */
struct dynamic_sa_aes192_ccm {
	struct dynamic_sa_ctl ctrl;
	u32 key[6];
	u32 iv[4]; /* for CBC, OFC, and CFB mode */
	u32 state_ptr;
	u32 reserved;
} __attribute__((packed));
#define SA_AES192_CCM_LEN           (sizeof(struct dynamic_sa_aes192_ccm)/4)
#define SA_AES192_CCM_CONTENTS      0x3e000062

/**
 * Security Association (SA) for AES128 CCM
 */
struct dynamic_sa_aes128_ccm {
	struct dynamic_sa_ctl ctrl;
	u32 key[4];
	u32 iv[4]; /* for CBC, OFC, and CFB mode */
	u32 state_ptr;
	u32 reserved;
} __attribute__((packed));
#define SA_AES128_CCM_LEN	(sizeof(struct dynamic_sa_aes128_ccm)/4)
#define SA_AES128_CCM_CONTENTS      0x3e000042

/**
 * Security Association (SA) for ARC4
 */
struct arc4_ij_ptr {
	u32 rsv:16;
	u32 j:8;
	u32 i:8;
} __attribute__((packed));

struct dynamic_sa_arc4 {
	struct dynamic_sa_ctl ctrl;
	u32 key[4];
	struct arc4_ij_ptr ij;
	u32 arc4_state_ptr;
	u32 reserved;
} __attribute__((packed));

#define SA_ARC4_LEN		(sizeof(struct dynamic_sa_arc4)/4)
#define SA_ARC4_CONTENTS        0xc0000042

/**
 * Security Association (SA) for IPsec ESP md5 or ESP sha1
 */
struct dynamic_sa_esp_md5_sha1
{
	struct dynamic_sa_ctl ctrl;
	u32 key[8];
	u32 inner_digest[5];
	u32 outter_digest[5];
	u32 spi;
	u32 seq[2];
	u32 seq_mask[4];
	u32 iv[4];
	u32 state_ptr;
	u32 reserved;
} __attribute__((packed));

#define SA_ESP_MD5_SHA1_LEN	sizeof(struct dynamic_sa_esp_md5_sha1)/4
#define SA_ESP_MD5_SHA1_CONTENTS        0x3ffca582

struct dynamic_sa_esp_des_md5_sha1
{
	struct dynamic_sa_ctl ctrl;
	u32 key[2];
	u32 inner_digest[5];
	u32 outter_digest[5];
	u32 spi;
	u32 seq[2];
	u32 seq_mask[4];
	u32 iv[4];
	u32 state_ptr;
	u32 reserved;
} __attribute__((packed));

#define SA_ESP_DES_MD5_SHA1_LEN		sizeof(struct dynamic_sa_esp_des_md5_sha1)/4
#define SA_ESP_DES_MD5_SHA1_CONTENTS        0x3ffca522

/**
 * Security Association (SA) for IPsec ESP 3des md5 sha1
 */
struct dynamic_sa_esp_3des_md5_sha1
{
	struct dynamic_sa_ctl ctrl;
	u32 key[6];
	u32 inner_digest[5];
	u32 outter_digest[5];
	u32 spi;
	u32 seq[2];
	u32 seq_mask[4];
	u32 iv[4];
	u32 state_ptr;
	u32 reserved;
} __attribute__((packed));

#define SA_ESP_3DES_MD5_SHA1_LEN		sizeof(struct dynamic_sa_esp_3des_md5_sha1)/4
#define SA_ESP_3DES_MD5_SHA1_CONTENTS        0x3ffca562

/**
 * Security Association (SA) for IPsec ESP sha512
 */
struct dynamic_sa_esp_sha512
{
	struct dynamic_sa_ctl ctrl;
	u32 key[8];
	u32 inner_digest[16];
	u32 outter_digest[16];
	u32 spi;
	u32 seq[2];
	u32 seq_mask[4];
	u32 iv[4];
	u32 state_ptr;
	u32 reserved;
} __attribute__((packed));

#define SA_ESP_SHA512_LEN		sizeof(struct dynamic_sa_esp_sha512)/4
#define SA_ESP_SHA512_CONTENTS        0x3ffe1082


/**
 * Security Association (SA) for IPsec ESP gcm
 */
struct dynamic_sa_esp_gcm
{
	struct dynamic_sa_ctl ctrl;
	u32 key[4];
	u32 inner_digest[4];
	u32 outter_digest[4];
	u32 spi;
	u32 seq[2];
	u32 seq_mask[4];
	u32 iv[4];
	u32 state_ptr;
	u32 reserved;
} __attribute__((packed));

#define SA_ESP_GCM_LEN		sizeof(struct dynamic_sa_esp_gcm)/4
#define SA_ESP_GCM_CONTENTS        0x3ffc8442
/**
 * Security Association (SA) for IPsec ESP aes sha256
 */
struct dynamic_sa_esp_sha256
{
	struct dynamic_sa_ctl ctrl;
	u32 key[8];
	u32 inner_digest[8];
	u32 outter_digest[8];
	u32 spi;
	u32 seq[2];
	u32 seq_mask[4];
	u32 iv[4];
	u32 state_ptr;
	u32 reserved;
} __attribute__((packed));

#define SA_ESP_SHA256_LEN      sizeof(struct dynamic_sa_esp_sha256)/4
#define SA_ESP_SHA256_CONTENTS        0x3ffd0882

/**
 * Security Association (SA) for MACsec GCM
 */
struct dynamic_sa_macsec_gcm
{
	struct dynamic_sa_ctl ctrl;
	u32 key[4];
	u32 inner_digest[4];
	u32 spi;
	u32 seq;
	u32 seq_mask[4];
	u32 iv[2];
	u32 state_ptr;
	u32 reserved;
} __attribute__((packed));

#define SA_MACSEC_GCM_LEN	      sizeof(struct dynamic_sa_macsec_gcm)/4
#define SA_MACSEC_GCM_CONTENTS        0x27ec0442

/**
 * Security Association (SA) for DTLS
 */
union dynamic_sa_dtls_spi
{
	struct {
		u32 rsv:8;
		u32 version:16;
		u32 type:8;
	}bf;
	u32 w;
}__attribute__((packed));

struct dynamic_sa_dtls
{
	struct dynamic_sa_ctl ctrl;
	u32 key[8];
	u32 inner_digest[5];
	u32 outter_digest[5];
	union dynamic_sa_dtls_spi spi;
	u32 seq[2];
	u32 seq_mask[4];
	u32 iv[4];
	u32 state_ptr;
	u32 reserved;
} __attribute__((packed));

#define SA_DTLS_LEN		sizeof(struct dynamic_sa_dtls)/4
#define SA_DTLS_CONTENTS        0x3ffca582

/**
 *
 * Security Association (SA) for SSL/TLS ARC4
 */
 struct dynamic_sa_ssl_tls_arc4
{
	struct dynamic_sa_ctl ctrl;
	u32 key[4];
	u32 inner_digest[5];
	u32	outter_digest[5];
	union dynamic_sa_dtls_spi spi;
	u32 seq[2];
	u32 seq_mask[4];
	u32 iv[4];
	u32 state_ptr;
	struct arc4_ij_ptr ij;
	u32 arc4_state_ptr;
} __attribute__((packed));

//typedef struct dynamic_sa_ssl_tls_arc4 dynamic_sa_ssl_tls_arc4_t;
#define SA_SSL_ARC4_LEN		sizeof(struct dynamic_sa_ssl_tls_arc4)/4
#define SA_SSL_ARC4_CONTENTS        0xfffca542

#endif
