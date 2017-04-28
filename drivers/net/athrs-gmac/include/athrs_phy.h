/*
 * Copyright (c) 2008, Atheros Communications Inc.
 *
 * Permission to use, copy, modify, and/or distribute this software for any
 * purpose with or without fee is hereby granted, provided that the above
 * copyright notice and this permission notice appear in all copies.
 *
 * THE SOFTWARE IS PROVIDED "AS IS" AND THE AUTHOR DISCLAIMS ALL WARRANTIES
 * WITH REGARD TO THIS SOFTWARE INCLUDING ALL IMPLIED WARRANTIES OF
 * MERCHANTABILITY AND FITNESS. IN NO EVENT SHALL THE AUTHOR BE LIABLE FOR
 * ANY SPECIAL, DIRECT, INDIRECT, OR CONSEQUENTIAL DAMAGES OR ANY DAMAGES
 * WHATSOEVER RESULTING FROM LOSS OF USE, DATA OR PROFITS, WHETHER IN AN
 * ACTION OF CONTRACT, NEGLIGENCE OR OTHER TORTIOUS ACTION, ARISING OUT OF
 * OR IN CONNECTION WITH THE USE OR PERFORMANCE OF THIS SOFTWARE.
 */

#ifndef _ATHR_PHY_H
#define _ATHR_PHY_H

#include <linux/types.h>
#include <linux/spinlock_types.h>
#include <linux/workqueue.h>
#include <asm/system.h>
#include <linux/netdevice.h>

#include "athrs_vlan_igmp.h"

#if defined(CONFIG_MACH_AR7240)||defined(CONFIG_MACH_HORNET)
#include "ag7240.h"
#include "ar7240.h"
#endif

#if defined(CONFIG_SOC_AR934x)
#include "ag934x.h"
#include "atheros.h"
#if defined(CONFIG_ATHRS27_PHY) && defined(CONFIG_ATHR_PHY_SWAP)
#define CONFIG_ETH_SOFT_LED 1
#endif
#endif

#ifdef CONFIG_MACH_HORNET
#define CONFIG_ETH_SOFT_LED 1
#endif

#if defined(CONFIG_MACH_AR7100)
#include "ag7100.h"
#include "ar7100.h"
#define is_ar7100() 1
#endif

#ifdef CONFIG_AR9100
#define is_ar9100() 1
#endif

/* MAC version defines */

#ifndef is_wasp
#define is_wasp() 0
#endif
#ifndef is_ar934x
#define is_ar934x() 0
#endif
#ifndef is_ar7100
#define is_ar7100() 0
#endif
#ifndef is_ar7240
#define is_ar7240() 0
#endif
#ifndef is_ar7241
#define is_ar7241() 0
#endif
#ifndef is_ar7242
#define is_ar7242() 0
#endif
#ifndef is_ar9100
#define is_ar9100() 0
#endif
#ifndef is_emu
#define is_emu() 0
#endif
#ifndef is_ar933x 
#define is_ar933x() 0
#endif
#define is_s26()  0
#define is_s27()  0
#define is_f1e()  0
#define is_f2e()  0
#define is_s16()  0
#define is_s17()  0
#define is_8021() 0
#define is_vir_phy() 0

#if defined(CONFIG_AR7240_S26_PHY) || defined(CONFIG_ATHRS26_PHY)
#include "ar7240_s26_phy.h"
#undef is_s26 
#define is_s26() 1
#endif

#ifdef CONFIG_ATHRS27_PHY
#include "athrs27_phy.h"
#undef is_s27 
#ifdef CONFIG_ATHR_SUPPORT_DUAL_PHY
#define is_s27()(mac->mac_unit == 1)
#else
#define is_s27()(1)
#endif
#endif

#ifdef CONFIG_ATHRF1_PHY
#include "athrf1_phy.h"
#undef is_f1e
#define is_f1e() 1
#endif

#ifdef CONFIG_ATHRF2_PHY
#include "athrf2_phy.h"
#undef is_f2e 
#define is_f2e() 1
#endif

#if defined(CONFIG_ATHRS16_PHY) || defined(CONFIG_AR7242_S16_PHY)
#include "athrs16_phy.h"
#undef is_s16 
#define is_s16() (mac->mac_unit == 0)
#endif

#if defined(CONFIG_ATHRS17_PHY)
#include "athrs17_phy.h"
#undef is_s17 
#undef is_s16 
#define is_s17() (mac->mac_unit == 0)
#define is_s16() (mac->mac_unit == 0)
#endif

#ifdef CONFIG_AR8021_PHY
#include "ar8021_phy.h"
#undef is_8021 
#define is_8021() 1
#endif
 
#ifdef CONFIG_ATHRS_VIR_PHY
#include "athrs_vir_phy.h"
#undef is_vir_phy 
#define is_vir_phy() 1
#endif

#ifdef CONFIG_HORNET_EMULATION
#define is_ar9330_emu() 1
#else
#define is_ar9330_emu() 0
#endif

#ifdef CONFIG_AR7240_EMULATION
#define is_ar7240_emu() 1
#else
#define is_ar7240_emu() 0
#endif

typedef struct {
    int  (*is_up)                (int ethUnit);
    int  (*is_alive)             (int phyUnit);
    int  (*speed)                (int ethUnit,int phyUnit);
    int  (*is_fdx)               (int ethUnit,int phyUnit);
    int  (*ioctl)                (struct net_device *dev,void *arg, int cmd);
    int  (*init)                 (void *arg);
    int  (*setup)                (void *arg); 
    void (*stab_wr)              (int phyUnit,int phy_up,int speed);
    void (*link_isr)             (void * arg);
    void (*en_link_intrs)        (void *arg);
    void (*dis_link_intrs)       (void *arg);
    unsigned int (*read_phy_reg) (int ethUnit,unsigned int phyUnit,unsigned int reg);
    void (*write_phy_reg)        (int ethUnit,unsigned int phyUnit,unsigned int reg,unsigned int val);
    unsigned int (*read_mac_reg) (unsigned int reg);
    void (*write_mac_reg)        (unsigned int reg,unsigned int val);
    void                         *mac;
    uint8_t                      port_map; /* Bit 0,1,2,3,4  corresponds to ports 0,1,2,3,4 
                                              Bit - 5 GE0 Bit -6 GE1 Bit - 7 Reserved */
    void 			*arg_ad[2];
}athr_phy_ops_t;
static inline int
athr_gmac_phy_attach(void *arg,int unit)
{
#if defined(CONFIG_AR7240_S26_PHY) || defined(CONFIG_ATHRS26_PHY)
   if (is_ar7240() || is_ar7241() || (is_ar7242() && unit == 1) || is_ar7100() || is_ar933x()) {
      athrs26_register_ops(arg);
   }
#endif
#if defined(CONFIG_AR7242_RGMII_PHY) || defined(CONFIG_ATHRF1_PHY)
   if((is_ar7242() && unit == 0) || is_ar7100()) {
      athrsf1_register_ops(arg);
   }
#endif
#ifdef CONFIG_ATHRF2_PHY
     athrsf2_register_ops(arg);
#endif
#if defined(CONFIG_ATHRS16_PHY) || defined(CONFIG_AR7242_S16_PHY)
   if((is_ar7242() && unit == 0) || is_ar7100() || (is_ar934x() && unit == 0)){
      athrs16_register_ops(arg);
   }
#endif
#if defined(CONFIG_ATHRS17_PHY)
      athrs17_register_ops(arg);
#endif
#ifdef CONFIG_ATHRS27_PHY
#ifdef CONFIG_ATHR_SUPPORT_DUAL_PHY
   if (is_ar934x() && unit) {
#else
   if (is_ar934x()) {
#endif
      printk("mac:%d Registering S27....\n",unit);
      athrs27_register_ops(arg);
   }
#endif
#ifdef CONFIG_ATHRF1_PHY
   if(is_ar934x() && unit == 0){
      athrsf1_register_ops(arg);
   }
#endif
#if defined(CONFIG_HORNET_EMULATION) && defined(CONFIG_AR8021_PHY)
    if(1) {
       ar8021_register_ops(arg);
    }
#endif
#ifdef CONFIG_ATHRS_VIR_PHY
   if (is_ar934x() && unit == 0){
	printk("Registering Virtual Phy....\n");
        athrs_vir_phy_register_ops(arg);
    }
#endif
   return 0;
}

static inline int athr_chk_phy_in_rst(void *arg)
{

#if defined(CONFIG_ATHRS16_PHY) || defined(CONFIG_AR7242_S16_PHY)
     return (athrs16_in_reset(arg));
#else
     return 0;
#endif

}

#endif
