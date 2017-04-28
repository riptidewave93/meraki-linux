/*
 *
 * Video Display Header file for TI81XX VPSS
 *
 * Copyright (C) 2012 TI
 * Author: Bikash Kund <bkund@ti.com>
 *
 * This file is from TI81XX VPSS M3 Driver
 * This program is free software; you can redistribute it and/or modify it
 * under the terms of the GNU General Public License as published by the
 * Free Software Foundation; either version 2 of the License, or (at your
 * option) any later version.
 *
 * This program is distributed in the hope that it will be useful, but
 * WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License along
 * with this program; if not, write to the Free Software Foundation, Inc.,
 * 59 Temple Place - Suite 330, Boston, MA      02111-1307, USA.
 */

/**
 *  \file vps_cfgDei.h
 *
 *  \brief VPSS - De-interlacer Configurations options interface
 */

#ifndef __LINUX_VPS_CFG_DEI_H
#define __LINUX_VPS_CFG_DEI_H


/* ========================================================================== */
/*                         Structure Declarations                             */
/* ========================================================================== */

/**
 *  struct Vps_DeiHqConfig
 *  \brief High quality DEI configuration.
 */
struct vps_deihqconfig {

	u32                  bypass;
	/*  DEI should be bypassed or not.
	*   1. For interlaced input and interlaced output from DEI/VIP Scalar,
	*   the DEI could be operated in two modes.
	*      a. DEI in bypass mode and the respective scalar in non-
	*      interlacing mode (interlaced input DEI, interlaced output
	*      from Scalar).
	*      In this case,
	*      DEI bypass = TRUE
	*      Scalar bypass = TRUE/FALSE depending up on whether scaling is
	*      required or not.
	*      b. DEI in deinterlacing mode - converts interlaced input to
	*      progressive output to the scalar. Scalar in interlacing mode -
	*      converts the progressive input from DEI to interlaced output.
	*      In this case,
	*      DEI bypass = FALSE
	*      Scalar bypass = FALSE
	*   2. For progressive input and interlaced output, Scalar will be in
	*      interlacing mode.
	*      DEI bypass = TRUE
	*      Scalar bypass = FALSE
	*   3. For progressive input and progressive output,
	*      DEI bypass = TRUE
	*      Scalar bypass = TRUE/FALSE depending up on whether scaling is
	*      required or not.
	*   4. For interlaced input and progressive output,
	*      DEI bypass = FALSE
	*      Scalar bypass = TRUE/FALSE depending up on whether scaling is
	*      required or not.
	*/
	u32                  inpmode;
	/**< Interpolation mode. For valid values see #Vps_DeiHqEdiMode. */
	u32                  tempinpenable;
	/**< 3D processing (temporal interpolation). */
	u32                  tempcnpchromaEnable;
	/**< 3D processing for chroma. */
	u32                  spatmaxbypass;
	/**< Bypass spatial maximum filtering. */
	u32                  tempmaxbypass;
	/**< Bypass temporal maximum filtering. */
	u32                  fldmode;
	/**< Motion Detection (MDT) mode. Determines how many previous fields
	*	are used in motion detection.
	*	For valid values see #Vps_DeiHqFldMode. */
	u32                  lcmodeenable;
	/**< Low cost mode disables logics that reduces flicker and produces
	*	smooth motion in areas of detail. It is recommended to use
	*	Advanced mode, so should be set to false. */
	u32                  mvstmenable;
	/**< Enable MVSTM loop. MVSTM should be enabled for the Advanced
	*	mode but it is optional for the low cost mode. */
	u32                  tnrenable;
	/**< Enable TNR for both Luma as well as for Chroma.
	*       If the DEI bypass flag is TRUE (as in the case of progressive
	*       inputs), the driver will internally set the DEI in non-bypass
	*       mode when TNR needs to be enabled. */
	u32                  snrenable;
	/**< Enable Snr for both Luma as well as for Chroma. */
	u32                  sktenable;
	/**< Enable Skin Tone Detection. */
	u32                  chromaedienable;
	/**< Edge adaptive interpolation for chroma. */
};

/**
 *  struct Vps_DeiConfig
 *  \brief DEI configuration.
 */
struct vps_deiconfig {
	u32                  bypass;
	/*  DEI should be bypassed or not.
	*   1. For interlaced input and interlaced output from DEI/VIP Scalar,
	*   the DEI could be operated in two modes.
	*      a. DEI in bypass mode and the respective scalar in non-
	*      interlacing mode (interlaced input DEI, interlaced output
	*      from Scalar).
	*      In this case,
	*      DEI bypass = TRUE
	*      Scalar bypass = TRUE/FALSE depending up on whether scaling is
	*      required or not.
	*      b. DEI in deinterlacing mode - converts interlaced input to
	*      progressive output to the scalar. Scalar in interlacing mode -
	*      converts the progressive input from DEI to interlaced output.
	*      In this case,
	*      DEI bypass = FALSE
	*      Scalar bypass = FALSE
	*   2. For progressive input and interlaced output, Scalar will be in
	*      interlacing mode.
	*      DEI bypass = TRUE
	*      Scalar bypass = FALSE
	*   3. For progressive input and progressive output,
	*      DEI bypass = TRUE
	*      Scalar bypass = TRUE/FALSE depending up on whether scaling is
	*      required or not.
	*   4. For interlaced input and progressive output,
	*      DEI bypass = FALSE
	*      Scalar bypass = TRUE/FALSE depending up on whether scaling is
	*      required or not.
	*/
	u32                  inpmode;
	/**< Interpolation mode. For valid values see #Vps_DeiEdiMode. */
	u32                  tempinpenable;
	/**< 3D processing (temporal interpolation). */
	u32                  tempInpchromaenable;
	/**< 3D processing for chroma. */
	u32                  spatmaxbypass;
	/**< Bypass spatial maximum filtering. */
	u32                  tempmaxbypass;
	/**< Bypass temporal maximum filtering. */
};


#endif

/* @} */
