/****************************************************************************
 * cryptodev.h
 *
 * Linux CryptoAPI user space interface module
 *
 * Copyright (c) 2008 Shasi Pulijala <spulijala@amcc.com>
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
 ****************************************************************************
 */

#ifndef __CRYPTODEV_H__
#define __CRYPTODEV_H__

/**
 * @struct session_op
 * @brief ioctl parameter to create a session
 *
 *
 */
struct session_op {
	__u16		algo_size;
	__u16		key_size;	/* cipher key length*/
	__u16		hmackey_size;	/* mac key length*/
	__u16		icv_size;	/* authsize (ccm, gcm)*/
	__u8		data[0];
};

struct pka_op {
	__u8 		pka_alg;
	struct iovec 	iov[5];
	__u16 		num_iovecs;
};
/**
 * @struct key_op
 * @brief ioctl parameter to change the session key
 *
 */
struct key_op {
	__u16		ksize;
	__u8		data[0];
};

/**
 * @struct crypt_op
 * @brief ioctl parameter to request a crypt/decrypt operation
 * against a session
 *
 *
 */
#define CRYPTO_MAX_DATA_LEN		(64*1024 - 1)
struct crypt_op {
#define COP_NONE	0
#define COP_ENCRYPT	1
#define COP_DECRYPT	2
	__u16		op;		/* i.e. COP_ENCRYPT */
	__u16		flags;
	__u16		iv_size;
	__u16 		assoc_size;
	__u8		data[0];	/* must be big enough for chosen MAC */
};

/* create crypto session */
#define CIOCGSESSION    _IOWR('c', 101, struct session_op)
/* change crypto key */
#define CIOCKEY    	_IOWR('c', 102, struct key_op)
#define CIOCFINAL    	_IOWR('c', 105, int)

#define CIOCPKA		_IOWR('c', 107, struct pka_op)
#endif
