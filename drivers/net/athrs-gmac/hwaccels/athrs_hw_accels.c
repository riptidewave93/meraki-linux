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

#include "athrs_mac.h"

#ifdef CONFIG_ATHRS_S27_HW_ACL
#include "athrs_s27_nf_acl.h"
#endif

#ifdef CONFIG_ATHRS_S27_HW_NAT
#include  "athrs_s27_nf_nat.h"
#endif

#if 0
static irqreturn_t
athr_nat_ager_intr(ATHR_MAC_ISR_ARGS)
{
    //printk("NAT AGER INTERRUPT !!!\n");


    athr_reg_wr(ATHR_LUTS_AGER_INTR, 0);

    athr_reg_wr(ETH_EG_AGER_FIFO_REG, 0);
    athr_reg_wr(ETH_EG_AGER_FIFO_REG, 1);
    athr_reg_wr(ATHR_LUTS_AGER_INTR, 0);
    athr_reg_wr(ATH_MISC_INT_STATUS, (1<<13));

    return IRQ_HANDLED;
}
#endif

inline int
athr_reg_hwaccels(void *arg)
{
      athr_gmac_t *mac = (athr_gmac_t *)arg;
      athr_hw_accels_t *hw_accels = mac->hw_accels;
//      int st;

      if (!hw_accels) {
           hw_accels = (athr_hw_accels_t *)kzalloc(sizeof(athr_hw_accels_t), GFP_KERNEL);
           mac->hw_accels = hw_accels;
      }

#ifdef CONFIG_ATHRS_S27_HW_NAT
      hw_accels->nat = athrs27_register_nat_ops();
      hw_accels->nat->set_wan_mac_addr(mac->mac_dev->dev_addr);
#if 0
      st = request_irq(ATH_MISC_IRQ_NAT_AGER, athr_nat_ager_intr, ATHR_MAC_IRQF_DISABLED, "nat ager intr", NULL);
      if (st < 0)
      {
          printk(": request irq %d failed %d\n", ATH_MISC_IRQ_NAT_AGER, st);
          return 1;
      }
#endif
#endif
#ifdef CONFIG_ATHRS_S27_HW_ACL
      hw_accels->acl = athrs27_register_acl_ops();
#endif
   return 0;
}

inline int
athr_setup_hwaccels(void *arg)
{
      athr_gmac_t *mac = (athr_gmac_t *)arg;
      athr_hw_accels_t *hw_accels = mac->hw_accels;
#if defined(CONFIG_ATHRS_S27_HW_NAT)
      hw_accels->nat->init();
#endif
#ifdef CONFIG_ATHRS_S27_HW_ACL
      hw_accels->acl->init();
#endif
   return 0;
}

inline int
athr_setup_hwaccels_notifiers(void *arg)
{
      athr_gmac_t *mac = (athr_gmac_t *)arg;
#if CONFIG_ATHRS_S27_HW_NAT
      athr_hw_accels_t *hw_accels = mac->hw_accels;
#endif
#ifdef CONFIG_ATHRS_S27_HW_NAT
      hw_accels->nat->reg_net_notifier();
#endif
   return 0;
}

inline int
athr_clean_hwaccels(void *arg)
{
      athr_gmac_t *mac = (athr_gmac_t *)arg;
      athr_hw_accels_t *hw_accels = mac->hw_accels;
#ifdef CONFIG_ATHRS_S27_HW_NAT
      hw_accels->nat->unreg_net_notifier();
      hw_accels->nat->proc_remove();
      hw_accels->nat->down();

      hw_accels->nat->cleanup();
      kfree(hw_accels->nat);
      hw_accels->nat = NULL;

#endif
#ifdef CONFIG_ATHRS_S27_HW_ACL
      hw_accels->acl->cleanup();
      kfree(hw_accels->acl);
      hw_accels->acl = NULL;

#endif
      kfree(hw_accels);
      hw_accels = NULL;
      mac->hw_accels = NULL;
   return 0;
}

