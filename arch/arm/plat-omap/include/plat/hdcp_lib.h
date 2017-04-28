/*
 * hdcp_lib.h
 *
 * HDCP interface DSS driver setting for TI's OMAP4 family of processor.
 * Copyright (C) 2010-2011 Texas Instruments Incorporated - http://www.ti.com/
 * Authors: Fabrice Olivero
 *	Fabrice Olivero <f-olivero@ti.com>
 *
 * This program is free software; you can redistribute it and/or modify it
 * under the terms of the GNU General Public License version 2 as published by
 * the Free Software Foundation.
 *
 * This program is distributed in the hope that it will be useful, but WITHOUT
 * ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or
 * FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Public License for
 * more details.
 *
 * You should have received a copy of the GNU General Public License along with
 * this program.  If not, see <http://www.gnu.org/licenses/>.
 */
/* Rev history:
 * Sujith Shivalingappa <sujith.s@ti.com> Added init routines
 *		added to support TI814x and TI816x
 *		Clean up - removed unused / un-required strucutres / defines
 */
#ifndef _HDCP_LIB_H_
#define _HDCP_LIB_H_

#include <linux/mutex.h>
#include <linux/workqueue.h>
#include <linux/init.h>
#include <linux/kernel.h>
#include <linux/io.h>
#include <linux/slab.h>
#include <linux/fs.h>

#include <linux/ti81xxhdmi.h>

/* Configuration control for this library */
/* Auto RI mode is to be supported */
#define _9032_AUTO_RI_
/* Auto BCAP polling is to be supported */
#define _9032_BCAP_
/* Toggel Repeater bit, before updating with correct value.
	This workaround is not required for normal operation. i.e this macro
	should be defined */
#undef _9032_AN_STOP_FIX_
#ifdef DEBUG
#define DDC_DBG			/* Log DDC data */
#undef POWER_TRANSITION_DBG	/* Add wait loops to allow testing DSS power
				 * transition during HDCP */
#endif

/***************************/
/* Definitions             */
/***************************/

/* Status / error codes */
#define HDCP_OK				0
#define HDCP_DDC_ERROR			1
#define HDCP_AUTH_FAILURE		2
#define HDCP_AKSV_ERROR			3
#define HDCP_3DES_ERROR			4
#define HDCP_SHA1_ERROR			5
#define HDCP_DRIVER_ERROR		6
#define HDCP_CANCELLED_AUTH		7

#define HDCP_INFINITE_REAUTH		0x100
#define HDCP_MAX_DDC_ERR		5

/* FIXME: should be 300ms delay between HDMI start frame event and HDCP enable
 * (to respect 7 VSYNC delay in 24 Hz)
 */
#define HDCP_ENABLE_DELAY		300
#define HDCP_R0_DELAY			110
#define HDCP_KSV_TIMEOUT_DELAY		5000
#define HDCP_REAUTH_DELAY		100

/* DDC access timeout in ms */
#define HDCP_DDC_TIMEOUT		500
#define HDCP_STOP_FRAME_BLOCKING_TIMEOUT	(2*HDCP_DDC_TIMEOUT)

/* Defined Twice - Fixme */
#define DESHDCP_KEY_SIZE 		160
struct hdcp_encrypt_control {
	uint32_t in_key[DESHDCP_KEY_SIZE];
	uint32_t *out_key;
};

/***************************/
/* HW specific definitions */
/***************************/

#define DSS_SS_FROM_L3__DESHDCP 0x58007000

/* DESHDCP registers */
#define DESHDCP__DHDCP_CTRL   0x020
#define DESHDCP__DHDCP_DATA_L 0x024
#define DESHDCP__DHDCP_DATA_H 0x028

/* DESHDCP CTRL bits */
#define DESHDCP__DHDCP_CTRL__DIRECTION_POS_F 2
#define DESHDCP__DHDCP_CTRL__DIRECTION_POS_L 2

#define DESHDCP__DHDCP_CTRL__OUTPUT_READY_POS_F 0
#define DESHDCP__DHDCP_CTRL__OUTPUT_READY_POS_L 0


/* HDMI CORE SYSTEM base address */
/*-------------------------------*/
#define HDMI_IP_CORE_SYSTEM 			0x400
#define HDMI_IP_CORE_SYSTEM__DCTL		0x034
#define HDMI_IP_CORE_SYSTEM__HDCP_CTRL		0x03C
#define HDMI_IP_CORE_SYSTEM__BKSV0		0x040
#define HDMI_IP_CORE_SYSTEM__AN0		0x054
#define HDMI_IP_CORE_SYSTEM__AKSV0		0x074
#define HDMI_IP_CORE_SYSTEM__R1			0x088
#define HDMI_IP_CORE_SYSTEM__R2			0x08C
#define HDMI_IP_CORE_SYSTEM__RI_CMD		0x09C
#define HDMI_IP_CORE_SYSTEM__RI_STAT		0x098
#define HDMI_IP_CORE_SYSTEM__INTR2		0x1C8
#define HDMI_IP_CORE_SYSTEM__INTR3		0x1CC
#define HDMI_IP_CORE_SYSTEM__INT_UNMASK2	0x1D8
#define HDMI_IP_CORE_SYSTEM__INT_UNMASK3	0x1DC
#define HDMI_IP_CORE_SYSTEM__SHA_CTRL		0x330
#define HDMI_IP_CORE_SYSTEM__INTR2__BCAP	0x80
#define HDMI_IP_CORE_SYSTEM__INTR3__RI_ERR	0xF0

/* HDMI CORE AV base address */
/*---------------------------*/

#define HDMI_CORE_AV_BASE		0x900
#ifndef HDMI_CORE_AV_HDMI_CTRL
#define HDMI_CORE_AV_HDMI_CTRL		0x0BC
#define HDMI_CORE_AV_PB_CTRL2		0x0FC
#define	HDMI_CORE_AV_CP_BYTE1		0x37C
#endif
#define HDMI_CORE_AV_HDMI_CTRL__HDMI_MODE	0x01

/***********************/
/* HDCP DDC addresses  */
/***********************/

#define DDC_BKSV_ADDR		0x00
#define DDC_Ri_ADDR		0x08
#define DDC_AKSV_ADDR		0x10
#define DDC_AN_ADDR		0x18
#define DDC_V_ADDR		0x20
#define DDC_BCAPS_ADDR		0x40
#define DDC_BSTATUS_ADDR	0x41
#define DDC_KSV_FIFO_ADDR	0x43

#define DDC_BKSV_LEN		5
#define DDC_Ri_LEN		2
#define DDC_AKSV_LEN		5
#define DDC_AN_LEN		8
#define DDC_V_LEN		20
#define DDC_BCAPS_LEN		1
#define DDC_BSTATUS_LEN		2

#define DDC_BIT_REPEATER	6

#define DDC_BSTATUS0_MAX_DEVS	0x80
#define DDC_BSTATUS0_DEV_COUNT	0x7F
#define DDC_BSTATUS1_MAX_CASC	0x08

/* Enum for repeating and non repeating sink */
enum hdcp_repeater {
	HDCP_RECEIVER = 0,
	HDCP_REPEATER = 1
};

/* Ecryption state, turn on and off */
enum encryption_state {
	HDCP_ENC_OFF = 0x0,
	HDCP_ENC_ON  = 0x1
};

/* Mute - used directly to enable / disable mute */
enum av_mute {
	AV_MUTE_SET = 0x01,
	AV_MUTE_CLEAR = 0x10
};

/* Structure that defines the instace of library - Caller of this library
	should initialize these members */
struct hdcp_lib_inst {
	int is_initialized;
	void __iomem *hdmi_wp_base_addr;
	void __iomem *deshdcp_base_addr;
	int pending_disable;
	spinlock_t spinlock;
};

/***************************/
/* Function prototypes     */
/***************************/

/* 3DES */
int hdcp_3des_load_key(uint32_t *deshdcp_encrypted_key);
void hdcp_3des_encrypt_key(struct hdcp_encrypt_control *enc_ctrl,
			   uint32_t out_key[DESHDCP_KEY_SIZE]);

/* IP control */
void hdcp_lib_read_bksv(u8 *ksv_data);
int hdcp_lib_disable(void);
int hdcp_lib_step1_start(void);
int hdcp_lib_step1_r0_check(u8 *metadata);
int hdcp_lib_step2(void);
int hdcp_lib_irq(void);
void hdcp_lib_auto_ri_check(bool state);
void hdcp_lib_auto_bcaps_rdy_check(bool state);
void hdcp_lib_set_av_mute(enum av_mute av_mute_state);
void hdcp_lib_set_encryption(enum encryption_state enc_state);
u8 hdcp_lib_check_repeater_bit_in_tx(void);
int hdcp_lib_set_pending_disable(void);
int hdcp_lib_clear_pending_disable(void);
int hdcp_lib_get_sha_data(struct hdcp_sha_in *sha);

/* Init library */
int hdcp_lib_init(void);
int hdcp_lib_de_init(void);

#endif /* _HDCP_LIB_H_ */


/***************************/
/* Macros for accessing HW */
/***************************/

#define WR_REG_32(base, offset, val)	__raw_writel(val, base + offset)
#define RD_REG_32(base, offset)		__raw_readl(base + offset)


#undef FLD_MASK
#define FLD_MASK(start, end)	(((1 << (start - end + 1)) - 1) << (end))
#undef FLD_VAL
#define FLD_VAL(val, start, end) (((val) << end) & FLD_MASK(start, end))
#define FLD_GET(val, start, end) (((val) & FLD_MASK(start, end)) >> (end))
#define FLD_MOD(orig, val, start, end) \
	(((orig) & ~FLD_MASK(start, end)) | FLD_VAL(val, start, end))

#define WR_FIELD_32(base, offset, start, end, val) \
	WR_REG_32(base, offset, FLD_MOD(RD_REG_32(base, offset), val, \
		  start, end))

#define RD_FIELD_32(base, offset, start, end) \
	((RD_REG_32(base, offset) & FLD_MASK(start, end)) >> (end))


