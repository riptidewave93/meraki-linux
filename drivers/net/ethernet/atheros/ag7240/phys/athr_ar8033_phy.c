/*
 * Copyright (c) 2010, Atheros Communications Inc.
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

#include "athrs_phy.h"
#include "athrs_mac.h"

extern void athr_dcm_reset_seq(void);
static athr_phy_speed_t athrs_ar8033_speed(int ethUnit, unsigned int phyUnit);
static int athrs_ar8300_phyUnit_to_phyAddr(int phyUnit);

void
athrs_ar8033_mgmt_init(void)
{
#ifdef KPHACK
    uint32_t rddata;

    rddata = athr_reg_rd(GPIO_IN_ENABLE3_ADDRESS)&
             ~GPIO_IN_ENABLE3_MII_GE1_MDI_MASK;
    rddata |= GPIO_IN_ENABLE3_MII_GE1_MDI_SET(19);
    athr_reg_wr(GPIO_IN_ENABLE3_ADDRESS, rddata);
    
    ath_gpio_config_output(ATH_GPIO);
    ath_gpio_config_output(ATH_GPIO17);
    
    rddata = athr_reg_rd(GPIO_OUT_FUNCTION4_ADDRESS) &
             ~(GPIO_FUNCTION4_MASK);
                

    rddata |=(GPIO_FUNCTION4_ENABLE);
              
    athr_reg_wr(GPIO_OUT_FUNCTION4_ADDRESS, rddata);

    ath_mdio_gpio14();
#endif
}

int
athrs_ar8033_phy_setup(void  *arg)
{

    if (is_emu()) {
        athr_dcm_reset_seq();
    }
    
    return 0;
}

int 
athrs_ar8033_reg_init(void *arg)
{
  athr_gmac_t *mac   = (athr_gmac_t *) arg;

  athrs_ar8033_mgmt_init();
  if (is_emu()){ 
      athr_gmac_mii_write(mac->mac_unit,0x5, 0x9, 0x0);
  }
  athr_gmac_mii_write(mac->mac_unit,0x5, 0x1f, 0x101 );


#ifdef FORCE_10
  printk ("To advertise only 10Mpbps\n");
  athr_gmac_mii_write(mac->mac_unit,0x0, 0x16,0x0);
  athr_gmac_mii_write(mac->mac_unit,0x0, 0x4, 0x0040 |0x0020);
#endif


  printk("%s: Done\n",__func__);
   
  return 0;
}

unsigned int 
athrs_ar8033_phy_read(int ethUnit, unsigned int phy_addr, unsigned int reg_addr)
{
  
  return athr_gmac_mii_read(ethUnit,phy_addr,reg_addr);
 
}

void 
athrs_ar8033_phy_write(int ethUnit,unsigned int phy_addr, unsigned int reg_addr, unsigned int write_data)
{
    athr_gmac_mii_write(ethUnit,phy_addr,reg_addr,write_data);

}

void
athrs_ar8033_led_set(struct led_classdev *led_cdev, enum led_brightness brightness)
{
    unsigned int regval = 0;
    unsigned int phy_addr = 0;
    unsigned int phy_unit = 0;

    athr_gmac_t *mac = container_of(led_cdev, athr_gmac_t, led_dev);

    if(brightness > 0) {
        //turn on lan leds
        regval = 0x0005;
        for(phy_unit = AR8033_PHY_BASE; phy_unit <= AR8033_PHY_MAX; phy_unit++) {
            phy_addr = athrs_ar8300_phyUnit_to_phyAddr(phy_unit);

            athrs_ar8033_phy_write(mac->mac_unit,
                                   phy_addr,
                                   AR8033_MAN_LED_CTRL_REG,
                                   regval);
        }
    } else {
        //turn off lan leds
        for(phy_unit = AR8033_PHY_BASE; phy_unit <= AR8033_PHY_MAX; phy_unit++) {
            phy_addr = athrs_ar8300_phyUnit_to_phyAddr(phy_unit);

            //It's unclear why different register values are required depending
            //on the speed of the connection.
            if(athrs_ar8033_speed(mac->mac_unit, phy_unit) == ATHR_PHY_SPEED_1000T) {
                regval = 0x00CA;
            } else {
                regval = 0x00AA;
            }

            athrs_ar8033_phy_write(mac->mac_unit,
                                   phy_addr,
                                   AR8033_MAN_LED_CTRL_REG,
                                   regval);
        }
    }
}

static int
athrs_ar8300_phyUnit_to_phyAddr(int phyUnit)
{
   return phyUnit ? 0x0 : 0x3;
}

static int
athrs_ar8300_phy_is_link_alive(int phyUnit)
{
    uint16_t phyHwStatus;
    uint32_t  phyAddr = athrs_ar8300_phyUnit_to_phyAddr(phyUnit);

    phyHwStatus = athr_gmac_mii_read(AR8033_PHY_BASE, phyAddr, AR8033_PHY_SPEC_STATUS);

    if (phyHwStatus == 0xffff)
        return FALSE;

    if (phyHwStatus & ATHR_STATUS_LINK_PASS)
        return TRUE;

    return FALSE;
}

static int
athrs_ar8033_is_fdx(int ethUnit, unsigned int phyUnit)
{
    uint16_t  phyHwStatus;
    int       ii = 200;
    uint32_t  phyAddr = athrs_ar8300_phyUnit_to_phyAddr(phyUnit);

    if (athrs_ar8300_phy_is_link_alive(phyUnit)) {

        do {
            phyHwStatus = athr_gmac_mii_read (AR8033_PHY_BASE, phyAddr,
                                           AR8033_PHY_SPEC_STATUS);
		      if(phyHwStatus & AR8033_LINK_STATUS_RESOLVED)
			          break;
            mdelay(10);
        } while(--ii);

        if (phyHwStatus & ATHER_STATUS_FULL_DEPLEX)
            return TRUE;
    }

    return FALSE;
}

static athr_phy_speed_t
athrs_ar8033_speed(int ethUnit, unsigned int phyUnit)
{
    uint16_t  phyHwStatus;
    uint32_t  phyAddr = athrs_ar8300_phyUnit_to_phyAddr(phyUnit);
    int       ii = 200;
    athr_phy_speed_t phySpeed = ATHR_PHY_SPEED_10T;

    if (athrs_ar8300_phy_is_link_alive(phyUnit)) {

        do {
            phyHwStatus = athr_gmac_mii_read(AR8033_PHY_BASE, phyAddr,
                                          AR8033_PHY_SPEC_STATUS);
		      if(phyHwStatus & AR8033_LINK_STATUS_RESOLVED)
			        break;
            mdelay(10);
        }while(--ii);

        phyHwStatus = ((phyHwStatus & ATHER_STATUS_LINK_MASK) >>
                       ATHER_STATUS_LINK_SHIFT);

        switch(phyHwStatus) {
        case 0:
            phySpeed = ATHR_PHY_SPEED_10T;
            break;
        case 1:
            phySpeed = ATHR_PHY_SPEED_100T;
            break;
        case 2:
            phySpeed = ATHR_PHY_SPEED_1000T;
            break;
        default:
            DPRINTF(MODULE_NAME":Unknown speed read:%d! phyhwstat:%x\n",phyUnit,phyHwStatus);
        }
    }

    return phySpeed;
}

static int
athrs_ar8033_is_up(int ethUnit)
{
    uint16_t      phyHwStatus, phyHwControl;
    int           linkCount   = 0;
    uint32_t  phyAddr = athrs_ar8300_phyUnit_to_phyAddr(ethUnit);


    /* Check for reset complete */
    phyHwStatus = athr_gmac_mii_read(AR8033_PHY_BASE, phyAddr, AR8033_PHY_STATUS);
    phyHwControl = athr_gmac_mii_read(AR8033_PHY_BASE, phyAddr, AR8033_PHY_CONTROL);
    if (!AR8033_RESET_DONE(phyHwControl))
        return FALSE;

    /* Check for AutoNegotiation complete */
    phyHwControl = athr_gmac_mii_read(AR8033_PHY_BASE, phyAddr, AR8033_PHY_CONTROL);
    if ((!(phyHwControl & AR8033_CTRL_AUTONEGOTIATION_ENABLE))
         || AR8033_AUTONEG_DONE(phyHwStatus)) {

        phyHwStatus = athr_gmac_mii_read(AR8033_PHY_BASE, phyAddr,
                                   AR8033_PHY_SPEC_STATUS);

        if (phyHwStatus & ATHR_STATUS_LINK_PASS) {
           linkCount++;
        }
    }

    return (linkCount);
}

int athrs_ar8033_phy_register_ops(void *arg)
{
  athr_gmac_t *mac   = (athr_gmac_t *) arg;
  athr_phy_ops_t *ops = mac->phy;

  if(!ops)
     ops = kmalloc(sizeof(athr_phy_ops_t), GFP_KERNEL);

  memset(ops,0,sizeof(athr_phy_ops_t));

  ops->mac            =  mac;
  ops->is_up          =  athrs_ar8033_is_up;
  ops->is_alive       =  NULL;
  ops->speed          =  athrs_ar8033_speed;
  ops->is_fdx         =  athrs_ar8033_is_fdx;
  ops->ioctl          =  NULL;
  ops->setup          =  athrs_ar8033_phy_setup;
  ops->stab_wr        =  NULL;
  ops->link_isr       =  NULL;
  ops->en_link_intrs  =  NULL;
  ops->dis_link_intrs =  NULL;
  ops->read_phy_reg   =  athrs_ar8033_phy_read;
  ops->write_phy_reg  =  athrs_ar8033_phy_write;
  ops->read_mac_reg   =  NULL;
  ops->write_mac_reg  =  NULL;
  ops->init           =  athrs_ar8033_reg_init;
  ops->led_set        =  athrs_ar8033_led_set;

  mac->phy = ops;
  ops->port_map       =  0xa0;
  return 0;
}
