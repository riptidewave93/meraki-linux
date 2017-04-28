/**
 * @cond COPYRIGHT_INFORMATION
 *
 * Copyright (C) 2010 AppliedMicro Confidential Information
 *
 * All Rights Reserved.
 *
 * THIS WORK CONTAINS PROPRIETARY INFORMATION WHICH IS THE PROPERTY OF
 * AppliedMicro AND IS SUBJECT TO THE TERMS OF NON-DISCLOSURE AGREEMENT
 * BETWEEN AppliedMicro AND THE COMPANY USING THIS FILE.
 *
 * @version 1.0
 * @author Anup Patel (apatel@amcc.com)
 *
 * @endcond
 *
 * @file apm_ipp_imageinfo.h
 * @brief iPP runtime code image header format
 *
 * This module defines the iPP runtime code image header format. This header
 * image is used to identify the iPP runtime code on persistent storage 
 * device.
 */
#ifndef _IPP_IMAGEINFO_H__
#define _IPP_IMAGEINFO_H__

/** @name iPP IRAM macros */
/*@{*/
#ifdef IPP_EXTERNAL_BOOT
#define IPP_IRAM_START		0x00000000 /**< iPP IRAM start address */
#else
#ifdef CONFIG_APM862xx
#define IPP_IRAM_START		0x00002000 /**< iPP IRAM start address */
#else
#define IPP_IRAM_START		0x00004000 /**< iPP IRAM start address */
#endif
#endif
/*@}*/

/** @name IPP Image Signture */
#define IPP_IMAGE_SIGNATURE		0x43434D41

/* iPP ROM image information */
#define IPP_ROM_TYPE			0x01
#define IPP_ROM_MAJOR			1
#define IPP_ROM_MINOR			0
#define IPP_ROM_LOADADDR		0x00000000

/** @name iPP RUNTIME image information */
/*@{*/
#define IPP_RUNTIME_TYPE		0x02
#define IPP_RUNTIME_MAJOR		1
#define IPP_RUNTIME_MINOR		0
#define IPP_RUNTIME_LOADADDR		IPP_IRAM_START
/*@}*/

/** @name iPP EXTERNAL image information */
/*@{*/
#define IPP_EXTERNAL_TYPE		0x03
#define IPP_EXTERNAL_MAJOR		1
#define IPP_EXTERNAL_MINOR		0
#define IPP_EXTERNAL_LOADADDR		0x00000000
/*@}*/

#ifndef IPP_LINKER_FLAG

/** @name iPP useful macro for image header */
/*@{*/
#define IPP_DECLARE_IMAGEINFO(type,major,minor,loadaddr,size) __attribute__ ((section(".info"))) ipp_imageinfo_t ipp_imginfo = {IPP_IMAGE_SIGNATURE,type,major,minor,loadaddr,size};
#define IPP_IMAGEINFO_SIGNATURE			(ipp_imginfo.image_signature)
#define IPP_IMAGEINFO_TYPE			(ipp_imginfo.image_type)
#define IPP_IMAGEINFO_MAJOR			(ipp_imginfo.image_major)
#define IPP_IMAGEINFO_MINOR			(ipp_imginfo.image_minor)
#define IPP_IMAGEINFO_LOADADDR			(ipp_imginfo.image_loadaddr)
#define IPP_IMAGEINFO_SIZE			(ipp_imginfo.image_size)
/*@}*/

/** @name PPC useful macro for image header */
/*@{*/
#define IPP_IMAGEINFO_OFFSET			0xF0
#define IPP_IMAGEINFO_POINTER(base)		((ipp_imageinfo_t *)(base + IPP_IMAGEINFO_OFFSET))
#define IPP_IMAGEINFO_PTR_SIGNATURE(ptr)	(ptr->image_signature)
#define IPP_IMAGEINFO_PTR_TYPE(ptr)		(ptr->image_type)
#define IPP_IMAGEINFO_PTR_MAJOR(ptr)		(ptr->image_major)
#define IPP_IMAGEINFO_PTR_MINOR(ptr)		(ptr->image_minor)
#define IPP_IMAGEINFO_PTR_LOADADDR(ptr)		(ptr->image_loadaddr)
#define IPP_IMAGEINFO_PTR_SIZE(ptr)		(ptr->image_size)
/*@}*/

/** 
 * iPP image header information 
 *
 * This header is appended to all runtime image binary. It is appended right 
 * before the code entry point. This structure is aligned to 16 bytes and
 * checked by ROM code/EXT Code. \n
 * WARNING: Don't change structure without understanding implications \n
 *          Don't INCREASE THIS STRUCTURE Beyond 16 bytes without changing 
 *           Code execution address
 */
struct ipp_imageinfo {
	unsigned int image_signature; 	/**< Signature of iPP image */
	unsigned short image_type; 	/**< Image type (ROM, RUNTIME, or EXTERNAL) */
	unsigned char image_major; 	/**< Major version number of image */
	unsigned char image_minor; 	/**< Minor version number of image */
	unsigned int image_loadaddr; 	/**< Address where the image is to be loaded */
	unsigned int image_size; 	/**< Size of image in bytes */
}__attribute__((packed));

/** Typename for iPP image header structure */
typedef struct ipp_imageinfo ipp_imageinfo_t;

#endif /* IPP_LINKER_FLAG */

#endif /* _IPP_IMAGEINFO_H__ */

