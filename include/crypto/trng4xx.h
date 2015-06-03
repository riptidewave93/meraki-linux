/**
 *
 * Copyright (c) 2008 Loc Ho <spulijala@amcc.com>
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
 * @file trng4xx.h
 *
 * This file provides access to the AMCC SoC TRNG.
 *
 *
 */
#ifndef __TRNG4XX_H__
#define __TRNG4XX_H__

/**
 * TRNG Register and bit Definitions
 *
 */

/* Register TRNG4XX_STATUS bit definition */
#define TRNG4XX_STATUS_BUSY                0x00000001

/* Register TRNG4XX_CNTL bit definition */
#define TRNG4XX_CNTL_TST_RING_OUT          0x00000001
#define TRNG4XX_CNTL_TST_MODE              0x00000002
#define TRNG4XX_CNTL_TST_RUN               0x00000004
#define TRNG4XX_CNTL_TST_EN1               0x00000008
#define TRNG4XX_CNTL_TST_EN2               0x00000010
#define TRNG4XX_CNTL_DISABLE_ALARM         0x00000020
#define TRNG4XX_CNTL_CLOCK_ON              0x00000040
#define TRNG4XX_CNTL_SHORT_CYCLE           0x00000080
#define TRNG4XX_CNTL_TST_ALARM             0x00000100
#define TRNG4XX_CNTL_TST_LFSR              0x00000200
#define TRNG4XX_CNTL_RESET_LFSR            0x00000400
#define TRNG4XX_CNTL_BYPASS_TRNG           0x00000800
#define TRNG4XX_CNTL_POST_PROC_EN          0x00001000

/* Register TRNG4XX_CFG bit definition */
#define TRNG4XX_CFG_RING1_DELAY_SEL_SHIFT  0
#define TRNG4XX_CFG_RING1_DELAY_SEL_MASK   (7)
#define TRNG4XX_CFG_RING2_DELAY_SEL_SHIFT  3
#define TRNG4XX_CFG_RING2_DELAY_SEL_MASK   \
			(7 << TRNG4XX_CFG_RING2_DELAY_SEL_SHIFT)
#define TRNG4XX_CFG_RESET_CNT_SHIFT        6
#define TRNG4XX_CFG_RESET_CNT_SHIFT_MASK   \
			(0x3F << TRNG4XX_CFG_RING2_DELAY_SEL_SHIFT)

/* TRNG Register definition */
#define TRNG4XX_OUTPUT_ADDR       0x0000
#define TRNG4XX_STATUS_ADDR       0x0004
#define TRNG4XX_CNTL_ADDR         0x0008
#define TRNG4XX_CFG_ADDR          0x000C
#define TRNG4XX_ALARMCNT_ADDR     0x0010
#define TRNG4XX_AREG_ADDR         0x0014
#define TRNG4XX_BREG_ADDR         0x0018
#define TRNG4XX_XREG_0_ADDR       0x001C
#define TRNG4XX_XREG_1_ADDR       0x0020
#define TRNG4XX_XREG_2_ADDR       0x0024
#define TRNG4XX_LFSR1_L_ADDR      0x0028
#define TRNG4XX_LFSR1_H_ADDR      0x002C
#define TRNG4XX_LFSR2_L_ADDR      0x0030
#define TRNG4XX_LFSR2_H_ADDR      0x0034
#define TRNG4XX_KEY0_L_ADDR       0x0038
#define TRNG4XX_KEY0_H_ADDR       0x003C
#define TRNG4XX_KEY1_L_ADDR       0x0040
#define TRNG4XX_KEY1_H_ADDR       0x0044
#define TRNG4XX_IV_L_ADDR         0x0048
#define TRNG4XX_IV_H_ADDR         0x004C

struct hal_config;

/**
 * @brief   This function MUSTs be called to initialize the HW
 *          access function.
 * @param   hal_cfg        HW access configuration. If NULL,
 *                              default will be used.
 * @note    To load all access functions from HAL with default initial
 *              map address, pass a NULL as its parameter.
 */
int    trng4xx_config_set(struct hal_config *cfg);

/**
 * @brief   This function MUSTs be called to de-initialize the HW
 *          access function.
 */
int    trng4xx_config_clear(void);

/**
 * @brief   This functions reads from the TRNG registers.
 * @param   reg_addr    Device register offset
 * @param   data_val    An DWORD pointer to store the returned value
 */
inline int    trng4xx_hw_read32(u32 reg_addr, u32 *data_val);

/**
 * @brief   This functions writes to the TRNG registers.
 * @param   reg_addr    Device register offset
 * @param   data_val    An DWORD value to write
 */
inline int    trng4xx_hw_write32(u32 reg_addr, u32 data_val);

/**
 * @brief   This functions initializes the TRNG for operation.
 * @return  0 
 */
int 	trng4xx_hw_init(void);

/**
 * @brief   This functions de-initializes the TRNG.
 */
int 	trng4xx_hw_deinit(void);

/**
 * @brief   This functions retrieves an true random number.
 * @param   rand_val    A pointer to a DWORD to store the value
 * @return  0 if successfull. -EINPROGRESS if failed. This
 *          should not happen as this means the hardware still busy.
 */
int 	trng4xx_random(u32 *rand_val);

#endif
