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
 * @file pka4xx_access.h
 *
 * This module provides access to the AMCC SoC PKA hardware under Linux.
 *
 *******************************************************************************
 */
#ifndef __PKA4XX_ACCESS_H__
#define __PKA4XX_ACCESS_H__

#include <linux/types.h>
#include <linux/interrupt.h>
#include <asm/errno.h>

#ifndef AHB_BASE_ADDR_LO
	/* FIXME */
#	define AHB_BASE_ADDR_LO	0
#endif

/* Debugging Flags */

#ifndef LL_EMERG
#define LL_EMERG           KERN_EMERG
#define LL_ALERT           KERN_ALERT
#define LL_CRIT            KERN_CRIT
#define LL_ERR             KERN_ERR
#define LL_WARNING         KERN_WARNING
#define LL_NOTICE          KERN_NOTICE
#define LL_INFO            KERN_INFO
#define LL_DEBUG           KERN_DEBUG
#define LL_EXTRADEBUG      KERN_DEBUG
#endif

#define PKA4XX_HDR	"PKA4XX: "
/* #define PKA4XX_DEBUG */

#if !defined(PKA4XX_DEBUG)
#	define LPRINTF(ll, fmt, ...)
#else
#       define LPRINTF(ll, fmt, ...)	\
		do { \
				printk(ll PKA4XX_HDR fmt "\n", ##__VA_ARGS__); \
		} while(0);
#endif

#ifndef RC_OK
	#define RC_OK                  0
	#define RC_INVALID_PARM        -EINVAL
	#define RC_NODEV               -ENODEV
	#define RC_NO_IMPLEMENTATION   -ENOSYS
	#define RC_ENOMEM              -ENOMEM
	#define RC_EINPROGRESS         -EINPROGRESS
	#define RC_EALREADY            -EALREADY
	#define RC_EBUSY	 	    -EBUSY
	#define RC_EIO		    -EIO

	/* Error code base specify to AMCC */
	#define RC_ERROR_BASE          5000
	#define RC_HWERROR             -(RC_ERROR_BASE+0)
	#define RC_FATAL               -(RC_ERROR_BASE+1)
#endif	/* RC_OK */

#ifndef ASSERT
	#define ASSERT(x)
#endif	/* ASSERT */

#endif
