/*
 *  Meraki MR18 Access Point board support
 *
 *  Author: Abhinav Acharya <abhinav.acharya@meraki.net>
 *  Copyright (c) 2013 Meraki, Inc.
 *
 *  This program is free software; you can redistribute it and/or modify it
 *  under the terms of the GNU General Public License version 2 as published
 *  by the Free Software Foundation.
 */

#include <linux/module.h>
#include <linux/types.h>
#include <linux/init.h>
#include <linux/kernel.h>
#include <linux/string.h>
#include <linux/init.h>
#include <linux/platform_device.h>
#include <linux/mtd/mtd.h>
#include <linux/mtd/partitions.h>
#include <linux/spi/flash.h>
#include <linux/pci.h>
#include <asm/mips_machine.h>
#include <linux/ath9k_platform.h>
#include <asm/mach-ath79/ar71xx_regs.h>
#include "common.h"

#include "ath79.h"
#include "machtypes.h"
#include "pci.h"
#include "dev-wmac.h"

#define MOD_NAME	"mr18-radio-init"
#define MOD_VERSION	"0.1"
#define MOD_AUTHOR	"Meraki"
#define MOD_DESC	"Meraki MR18 Radio Init"

static struct ath9k_platform_data mr18_ath9k_wlan0_data;
static struct ath9k_platform_data mr18_ath9k_wlan1_data;
static struct ath9k_platform_data mr18_ath9k_wlan2_data;

#define MR18_WLAN0_CALDATA_OFFSET (0x1000)
#define MR18_WLAN1_CALDATA_OFFSET (0x5000)
#define MR18_WLAN2_CALDATA_OFFSET (0x9000)

static int __init mr18_init_radios(void)
{
	struct mtd_info *mtd;
	int retlen, ret;
	struct pci_dev *wlan1, *wlan2;

	if ((mips_machtype != ATH79_MACH_MERAKI) || !soc_is_qca955x())
		return -ENODEV;

	/* Clear RTC reset (Needed by AHB Radio) */
	ath79_device_reset_clear(QCA955X_RESET_RTC);

	mtd = get_mtd_device_nm("caldata");
	if (IS_ERR(mtd)) {
		pr_err("%s: Unable to find calibration data, err = %lx\n",
		       __func__, PTR_ERR(mtd));
		return -EIO;
	}

   /* on-chip wlan 2.4G */
   ret = mtd_read(mtd, MR18_WLAN0_CALDATA_OFFSET,
                   sizeof(mr18_ath9k_wlan0_data.eeprom_data), &retlen,
                   (unsigned char *)mr18_ath9k_wlan0_data.eeprom_data);
   if ((ret < 0) || (retlen != sizeof(mr18_ath9k_wlan0_data.eeprom_data))) {
            pr_err("%s: Unable to read %s caldata, err=%d len=%d\n",
                   __func__, "AHB WLAN", ret, retlen);
            ret = -EIO;
            goto err_nowan0;
   }
   ath79_register_wmac((unsigned char *)mr18_ath9k_wlan0_data.eeprom_data);

   /* PCIe wlan 5G */
   wlan1 = pci_get_device(0x168c, 0x0033, NULL);
   if (!wlan1) {
           pr_err("%s: %s PCI device disappeared\n", __func__, "WLAN1");
           ret = -ENODEV;
           goto err_nowan0;
   }

   ret = mtd_read(mtd, MR18_WLAN1_CALDATA_OFFSET,
                  sizeof(mr18_ath9k_wlan1_data.eeprom_data),&retlen,
                  (unsigned char *)mr18_ath9k_wlan1_data.eeprom_data);
   if ((ret < 0) ||
       (retlen != sizeof(mr18_ath9k_wlan1_data.eeprom_data))) {
           pr_err("%s: Unable to read %s caldata, err=%d len=%d\n",
                  __func__, "PCI WLAN", ret, retlen);
           ret = -EIO;
           goto err_nowan1;
   } else {
           wlan1->dev.platform_data = &mr18_ath9k_wlan1_data;
   }

   /* PCIe scanning radio */
	wlan2 = pci_get_device(0x168c, 0x0033, wlan1);
	if (!wlan2) {
		pr_err("%s: %s PCI device disappeared\n", __func__, "WLAN2");
		ret = -ENODEV;
		goto err_nowan1;
	}

   ret = mtd_read(mtd, MR18_WLAN2_CALDATA_OFFSET,
                  sizeof(mr18_ath9k_wlan2_data.eeprom_data),&retlen,
                  (unsigned char *)mr18_ath9k_wlan2_data.eeprom_data);
   if ((ret < 0) ||
       (retlen != sizeof(mr18_ath9k_wlan2_data.eeprom_data))) {
           pr_err("%s: Unable to read %s caldata, err=%d len=%d\n",
                  __func__, "PCI WLAN", ret, retlen);
           ret = -EIO;
           goto err_nowan2;
   } else {
           wlan2->dev.platform_data = &mr18_ath9k_wlan2_data;
   }

	put_mtd_device(mtd);

	pci_dev_put(wlan2);
	pci_dev_put(wlan1);

	return 0;

err_nowan2:
	pci_dev_put(wlan2);
err_nowan1:
	pci_dev_put(wlan1);
err_nowan0:
	put_mtd_device(mtd);
	return ret;
}

static void __exit mr18_cleanup_radios(void)
{
   return;
}

module_init(mr18_init_radios);
module_exit(mr18_cleanup_radios);

MODULE_LICENSE("GPL");
MODULE_AUTHOR(MOD_AUTHOR);
MODULE_DESCRIPTION(MOD_DESC);
MODULE_ALIAS("platform:" MOD_NAME);
