/*
 *
 * CPROC header file for TI81XX VPSS
 *
 * Copyright (C) 2010 TI
 * Author: Yihe Hu <yihehu@ti.com>
 *
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
 * 59 Temple Place - Suite 330, Boston, MA	02111-1307, USA.
 */
#ifndef __LINUX_VPS_CFGCPROC_H
#define __LINUX_VPS_CFGCPROC_H

/* ========================================================================== */
/*                             Include Files                                  */
/* ========================================================================== */

/* None */

/* ========================================================================== */
/*                           Macros & Typedefs                                */
/* ========================================================================== */


/** \brief Num of coefficients in each set */
#define VPS_CPROC_NUM_COEFF           (3u)

/**
 *  \brief Enum for specifying per-defined CSC co-effs
 */
enum  vps_cprocciecammode {
	VPS_CPROC_CIECAM_MODE_BT601 = 0,
	/**< BT601 CIECAM and Inverse CIECAM coefficient */

	VPS_CPROC_CIECAM_MODE_BT709,
	/**< BT709 CIECAM and Inverse CIECAM coefficient */

	VPS_CPROC_CIECAM_MODE_NONE,
	/**< User provided CIECAM and Inverse CIECAM coefficient */
};

/* ========================================================================== */
/*                         Structure Declarations                             */
/* ========================================================================== */

/**
 *  struct Vps_CscConfig
 *  \brief Configuration parameters for CSC.
 */
struct vps_cprocciecamcfg {
	u32              multcoeff[VPS_CPROC_NUM_COEFF][VPS_CPROC_NUM_COEFF];
	/**< Multiplication coefficients in the format
		A0, B0, C0 in the first row,
		A1, B1, C1 in the second row,
		and A2, B2, C2 in the third row. */
	u32              s, t, a, cz;

	u32              gy, gub, guy, gvg, vgr;
} ;

struct vps_cprocuciecamcfg {
	u32              multcoeff[VPS_CPROC_NUM_COEFF][VPS_CPROC_NUM_COEFF];
	/**< Multiplication coefficients in the format
		A0, B0, C0 in the first row,
		A1, B1, C1 in the second row,
		and A2, B2, C2 in the third row. */
	u32              a, cz, s, x0;
} ;

struct vps_cprocconfig {
	u32                        ciecammode;
	/*check vps_cprocciecammode for the right value*/
	struct vps_cprocciecamcfg  ciecamcfg;
	struct vps_cprocuciecamcfg iciecamcfg;
};


/* ========================================================================== */
/*                          Function Declarations                             */
/* ========================================================================== */



#endif
