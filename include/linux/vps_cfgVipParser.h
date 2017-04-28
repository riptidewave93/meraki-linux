/*
 *
 * Capture VIP Parser configuration file for TI 81XX VPSS
 *
 * Copyright (C) 2011 TI
 * Author: Hardik Shah <hardik.shah@ti.com>
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

#ifndef _VPS_CFG_VIP_PARSER_H
#define _VPS_CFG_VIP_PARSER_H

#ifdef __KERNEL__

/** \brief Use this when a particular value for VIP parser config
  is dont care */
#define VPS_VIP_VALUE_DONT_CARE     (-1)

/**
 *  \brief Enum for Control Channel Selection
 *  It describes channels numbers from extract control code and Vertical
 *  Ancillary Data
 */
enum Vps_VipCtrlChanSel {
	VPS_VIP_CTRL_CHAN_SEL_7_0 = 0,
	/**< Use data[7:0] to extract control codes and Vertical
	  Ancillary Data */

	VPS_VIP_CTRL_CHAN_SEL_15_8,
	/**< Use data[15:8] to extract control codes and Vertical
	  Ancillary Data */

	VPS_VIP_CTRL_CHAN_SEL_23_16,
	/**< Use data[23:16] to extract control codes and Vertical
	  Ancillary Data
	 */

	VPS_VIP_CTRL_CHAN_DONT_CARE = -1
		/**< Value is dont care */

};

/**
 *  \brief It is used only for Discrete Sync
 */
enum Vps_VipLineCaptureStyle {
	VPS_VIP_LINE_CAPTURE_STYLE_HSYNC = 0,
	/**< Use HSYNC style linecapture */

	VPS_VIP_LINE_CAPTURE_STYLE_ACTVID,
	/**< Use ACTVID style line capture */

	VPS_VIP_LINE_CAPTURE_STYLE_DONT_CARE = -1
	/**< Value is dont care */

};

/**
 *  \brief It is only used for Discrete Sync
 */
enum Vps_VipFidDetectMode {
	VPS_VIP_FID_DETECT_MODE_PIN = 0,
	/**< Take FID from pin */

	VPS_VIP_FID_DETECT_MODE_VSYNC,
	/**< FID is determined by VSYNC skew */

	VPS_VIP_FID_DETECT_MODE_DONT_CARE = -1
	/**< Value is dont care */

};

/**
 *  \brief VIP Polarity
 */
enum Vps_VipPolarity {
	VPS_VIP_POLARITY_LOW = 0,
	/**< low Polarity */

	VPS_VIP_POLARITY_HIGH,
	/**< high Polarity */

	VPS_VIP_POLARITY_DONT_CARE = -1
	/**< Value is dont care */

};

/**
 *  \brief Pixel clock edge polarity
 */
enum Vps_VipPixClkEdgePol {
	VPS_VIP_PIX_CLK_EDGE_POL_RISING = 0,
	/**< Rising Edge is active PIXCLK edge */

	VPS_VIP_PIX_CLK_EDGE_POL_FALLING,
	/**< Falling Edge is active PIXCLK edge */

	VPS_VIP_PIX_CLK_EDGE_POL_DONT_CARE = -1
	/**< Value is dont care */

};

/**
 *  \brief VIP Field ID
 */
enum Vps_VipFid {
	VPS_VIP_FID_EVEN = 0,
	/**< Even field id */

	VPS_VIP_FID_ODD,
	/**< Odd field id */

	VPS_VIP_FID_DONT_CARE = -1
	/**< Value is dont care */

};

/**
 *  \brief It gives the imformation in 8b interface mode from where to extract
 *  Vertical Ancillary data
 */
enum Vps_VipAncChSel8b {
	VPS_VIP_ANC_CH_SEL_8B_LUMA_SIDE = 0,
	/**< Extract 8b Mode Vertical Ancillary Data from Luma Sites */

	VPS_VIP_ANC_CH_SEL_8B_CHROMA_SIDE,
	/**< Extract 8b Mode Vertical Ancillary Data from Chroma Sites */

	VPS_VIP_ANC_CH_SEL_DONT_CARE = -1
	/**< Value is dont care */

};

/**
 *  \brief In embedded sync for 2x/4x mux mode there are two way to extract
 *  soruce number, one is from least significant nibble of the XV/fvh codeword
 *  and other is least significant nibble of a horizontal blanking pixel value
 */
enum Vps_VipSrcNumPos {
	VPS_VIP_SRC_NUM_POS_LS_NIBBLE_OF_CODEWORD = 0,
	/**< srcnum is in the least significant nibble of the XV/fvh codeword */

	VPS_VIP_SRC_NUM_POS_LS_NIBBLE_OF_HOR_BLNK_PIX,
	/**< srcnum is in the least significant nibble of a horizontal blanking
	 * pixelvalue
	 */

	VPS_VIP_SRC_NUM_POS_DONT_CARE = -1
	/**< Value is dont care */

};

/* ========================================================================== */
/*                         Structure Declarations                             */
/* ========================================================================== */

/**
 *  \brief Configuration parameter for VIP Instance
 */
struct Vps_VipConfig {
	u32 clipActive;
	/**< FALSE: Do not clip active Data
	 *   TRUE : Clip active Data
	 */

	u32 clipBlank;
	/**< FALSE: Do not clip blanking Data
	 *   TRUE : Clip blanking Data
	 */

};

/**
 *  \brief Configuration parameter specific to Discrete Sync mode
 */
struct Vps_VipDiscreteConfig {
	u32 fidSkewPostCnt;
	/**< Post count value when using vsync skew in FID determination */

	u32 fidSkewPreCnt;
	/**< Pre count value when using vsync skew in FID determination */

	u32 lineCaptureStyle;
	/**< For valid values see #Vps_VipLineCaptureStyle */

	u32 fidDetectMode;
	/**< For valid values see #Vps_VipFidDetectMode */

	u32 actvidPol;
	/**< For valid values see #Vps_VipPolarity */

	u32 vsyncPol;
	/**< For valid values see #Vps_VipPolarity */

	u32 hsyncPol;
	/**< For valid values see #Vps_VipPolarity */

};

/**
 *  \brief Configuration parameter specific to Embedded Sync only
 */
struct Vps_VipEmbConfig {
	u32 errCorrEnable;
	/**< TRUE: Error Correction enable, FALSE: disabled */

	u32 srcNumPos;
	/**< For valid values see #Vps_VipSrcNumPos */

	u32 isMaxChan3Bits;
	/**< FALSE: Use all 5 bits of the field for 32 total sources,
	 *   TRUE : Use only bits 2:0 of this field to support
	 *          the TVP5158's 8 sources, with the two upper
	 *          most bits reserved
	 */
};

/**
 *  \brief Configuration parameter for a VIP Port
 */
struct Vps_VipPortConfig {
	u32 ctrlChanSel;
	/**< For valid values see #Vps_VipCtrlChanSel */

	u32 ancChSel8b;
	/**< For valid values see #Vps_VipAncChSel8b */

	u32 pixClkEdgePol;
	/**< For valid values see #Vps_VipPixClkEdgePol */

	u32 invertFidPol;
	/**< FALSE: Keep FID as found, TRUE: Invert Value of FID */

	struct Vps_VipEmbConfig       embConfig;
	/**< Configuration parameter specific to Embedded Sync mode */

	struct Vps_VipDiscreteConfig  disConfig;
	/**< Configuration parameter specific to Discrete Sync mode */

};

#endif /* #ifdef _KERNEL_H_ */
#endif /* #ifdef _VPS_CFG_VIP_PARSER_H */
