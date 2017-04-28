/*
 * arch/arm/plat-ambarella/include/plat/crypto.h
 *
 * Author: Anthony Ginger <hfjiang@ambarella.com>
 *
 * Copyright (C) 2004-2010, Ambarella, Inc.
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
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA 02111-1307 USA
 *
 */

#ifndef __PLAT_AMBARELLA_CRYPTO_H
#define __PLAT_AMBARELLA_CRYPTO_H

/* ==========================================================================*/
#define AMBARELLA_CRYPTO_ALIGNMENT	(16)
#define AMBARELLA_CRA_PRIORITY		(300)
#define AMBARELLA_COMPOSITE_PRIORITY	(400)

#define AMBA_HW_ENCRYPT_CMD		(0)
#define AMBA_HW_DECRYPT_CMD		(1)

/* ==========================================================================*/
#ifndef __ASSEMBLER__

#if (CHIP_REV == I1 || CHIP_REV == AMB_S2)
#define CRYPTO_DATA_64BIT (1)
#define CRYPTO_MODE_SWITCH (0)
#elif (CHIP_REV == A7)
#define CRYPTO_DATA_64BIT (1)
#define CRYPTO_MODE_SWITCH (1)
#else
#define CRYPTO_DATA_64BIT (0)
#define CRYPTO_MODE_SWITCH (0)
#endif

#if (CRYPTO_DATA_64BIT == 0)
#define	CRYPT_A_INPUT1	CRYPT_A_INPUT_96_REG
#define	CRYPT_A_INPUT2	NULL
#define	CRYPT_A_OPCODE	CRYPT_A_OPCODE_REG
#define	CRYPT_D_INPUT1	CRYPT_D_INPUT_HI_REG
#define	CRYPT_D_INPUT2	NULL
#define	CRYPT_D_OPCODE	CRYPT_D_OPCODE_REG
#else
#define	CRYPT_A_INPUT1	CRYPT_A_INPUT_ENC_96_REG
#define	CRYPT_A_INPUT2	CRYPT_A_INPUT_DEC_96_REG
#define	CRYPT_A_OPCODE	NULL
#define	CRYPT_D_INPUT1	CRYPT_D_INPUT_ENC_HI_REG
#define	CRYPT_D_INPUT2	CRYPT_D_INPUT_DEC_HI_REG
#define	CRYPT_D_OPCODE	NULL
#endif

#if (CRYPT_SUPPORT_MD5_SHA1 == 1)
#define	CRYPT_MD5_SHA1_READY_INPUT	CRYPT_MD5_SHA1_READY_INPUT_REG
#define	CRYPT_MD5_INIT_31_0		CRYPT_MD5_INIT_31_0_REG
#define	CRYPT_MD5_INPUT_31_0		CRYPT_MD5_INPUT_31_0_REG
#define	CRYPT_MD5_OUTPUT_READY		CRYPT_MD5_OUTPUT_READY_REG
#define	CRYPT_MD5_OUTPUT_31_0		CRYPT_MD5_OUTPUT_31_0_REG
#define	CRYPT_SHA1_INIT_31_0		CRYPT_SHA1_INIT_31_0_REG
#define	CRYPT_SHA1_INPUT_31_0		CRYPT_SHA1_INPUT_31_0_REG
#define	CRYPT_SHA1_OUTPUT_READY		CRYPT_SHA1_OUTPUT_READY_REG
#define	CRYPT_SHA1_OUTPUT_31_0		CRYPT_SHA1_OUTPUT_31_0_REG
#define	CRYPT_MD5_INT_EN		CRYPT_MD5_INT_EN_REG
#define	CRYPT_SHA1_INT_EN		CRYPT_SHA1_INT_EN_REG
/*** ONLY A7 has this REG **/
#if (CRYPT_SUPPORT_NON_BINARY == 1)
#define	CRYPT_BINARY_COMP		CRYPT_BINARY_COMP_REG
#else
#define	CRYPT_BINARY_COMP		NULL
#endif
/****/
#else
#define	CRYPT_MD5_SHA1_READY_INPUT	NULL
#define	CRYPT_MD5_INIT_31_0		NULL
#define	CRYPT_MD5_INPUT_31_0		NULL
#define	CRYPT_MD5_OUTPUT_READY		NULL
#define	CRYPT_MD5_OUTPUT_31_0		NULL
#define	CRYPT_SHA1_INIT_31_0		NULL
#define	CRYPT_SHA1_INPUT_31_0		NULL
#define	CRYPT_SHA1_OUTPUT_READY		NULL
#define	CRYPT_SHA1_OUTPUT_31_0		NULL
#define	CRYPT_MD5_INT_EN		NULL
#define	CRYPT_SHA1_INT_EN		NULL
#define	CRYPT_BINARY_COMP		NULL
#endif



#define  INDEPENDENT_MD5 1


struct ambarella_platform_crypto_info{
	u32	mode_switch;    /*a7 support mode switch between  "Binary Compatibility Mode" and "Non-Binary Compatibility Mode" */
				/* TODO: And now, haven't support a7 mode switch,the a7 is default mode ( non-binary compatible mode, do not need opcode)*/
				/* For iONE,this flag is 0, and default mode is non-binary compatible mode, do not need opcode*/
				/* For a5s, this flag is 0, and default mode is binary compatibility mode,  need opcode*/
				/* For a7,  this flag is 1, and default mode is non-binary compatible mode, do not need opcode*/
				/* 1: support switch*/
				/* 0: do not support switch */
/*******AES&DES START***************************/
	u32	binary_mode;    /* 1: binary compatibility mode need opcode*/
				/* 0: non-binary compatible mode do not need opcode*/
	u32	data_swap;      /*swap from little to big endian 0:do not need swap  1:need swap */
	u32	reg_64;         /*64 bit register,get the 64bit address*/
				/* 0: 32-bit ,get the 32bit register address ,for a5s*/
				/* 1: 64-bit ,get the 64bit register address ,for ione */
/*******AES&DES END***************************/
	u32	md5_sha1;       /* 0:do not support md5 or sha1*/
				/* 1:support md5 and sha1*/
/*******MD5&SHA1 START***************************/
	u32	md5_sha1_64bit; /* 0:32 bit ,read and write align at 32bit*/
				/* 1:64 bit ,read and write align at 64bit*/
/*******MD5&SHA1 END***************************/
	u32	exception;/*used for some special exception.*/
					/*INDEPENDENT_MD5 :  for s2 cortex, MD5 irq is seperated from sha1 irq */
};

typedef struct md5_digest_s
{
	u32 digest_0;
	u32 digest_32;

	u32 digest_64;
	u32 digest_96;

	u32 digest_128;
	u32 digest_160;

	u32 digest_192;
	u32 digest_224;

	u32 digest_256;
	u32 digest_288;

	u32 digest_320;
	u32 digest_352;

	u32 digest_384;
	u32 digest_416;

	u32 digest_448;
	u32 digest_480;
} md5_digest_t;

typedef struct md5_data_s
{
	u32 data[16];
} md5_data_t;

typedef struct sha1_digest_s
{
	u32 digest_0;
	u32 digest_32;
	u32 digest_64;
	u32 digest_96;
	u32 digest_128;
	u32 digest_padding;

} sha1_digest_t;

typedef struct sha1_data_s
{
	u32 data[16];
} sha1_data_t;


/* ==========================================================================*/
extern struct platform_device  ambarella_crypto;

#endif /* __ASSEMBLER__ */
/* ==========================================================================*/

#endif

