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

#include "athrf1_phy.h"
#include "athrs_phy.h"
#include "athrs_mac.h"

#define MODULE_NAME "ATHRSF1_PHY"

#define ATHR_LAN_PORT_VLAN          1
#define ATHR_WAN_PORT_VLAN          2
#define ENET_UNIT_LAN 1
#define ENET_UNIT_WAN 0

#define TRUE    1
#define FALSE   0

#define ATHR_PHY_MAX 5
#define ATHR_PHY0_ADDR   0x0
#define ATHR_PHY1_ADDR   0x1
#define ATHR_PHY2_ADDR   0x2
#define ATHR_PHY3_ADDR   0x3
#define ATHR_PHY4_ADDR   0x4

/*
 * Track per-PHY port information.
 */
typedef struct {
    BOOL   isEnetPort;       /* normal enet port */
    BOOL   isPhyAlive;       /* last known state of link */
    int    ethUnit;          /* MAC associated with this phy port */
    uint32_t phyBase;
    uint32_t phyAddr;          /* PHY registers associated with this phy port */
    uint32_t VLANTableSetting; /* Value to be written to VLAN table */
} athrPhyInfo_t;

/*
 * Per-PHY information, indexed by PHY unit number.
 */

static athrPhyInfo_t athrPhyInfo[] = {

    {TRUE,   /* port 1 -- LAN port 1 */
     FALSE,
     ENET_UNIT_LAN,
     0,
     ATHR_PHY0_ADDR,
     ATHR_LAN_PORT_VLAN
    },

    {TRUE,   /* port 2 -- LAN port 2 */
     FALSE,
     ENET_UNIT_LAN,
     0,
     ATHR_PHY1_ADDR,
     ATHR_LAN_PORT_VLAN
    },

    {TRUE,   /* port 3 -- LAN port 3 */
     FALSE,
     ENET_UNIT_LAN,
     0,
     ATHR_PHY2_ADDR,
     ATHR_LAN_PORT_VLAN
    },

    {TRUE,   /* port 4 --  LAN port 4 */
     FALSE,
     ENET_UNIT_LAN,
     0,
     ATHR_PHY3_ADDR,
     ATHR_LAN_PORT_VLAN   /* Send to all ports */
    },

    {TRUE,  /* port 5 -- WAN Port 5 */
     FALSE,
     ENET_UNIT_WAN,
     0,
     ATHR_PHY0_ADDR,
     ATHR_LAN_PORT_VLAN    /* Send to all ports */
    },

    {FALSE,   /* port 0 -- cpu port 0 */
     TRUE,
     ENET_UNIT_LAN,
     0,
     0x00,
     ATHR_LAN_PORT_VLAN
    },

};

#define ATHR_IS_ENET_PORT(phyUnit) (athrPhyInfo[phyUnit].isEnetPort)
#define ATHR_IS_PHY_ALIVE(phyUnit) (athrPhyInfo[phyUnit].isPhyAlive)
#define ATHR_ETHUNIT(phyUnit) (athrPhyInfo[phyUnit].ethUnit)
#define ATHR_PHYBASE(phyUnit) (athrPhyInfo[phyUnit].phyBase)
#define ATHR_PHYADDR(phyUnit) (athrPhyInfo[phyUnit].phyAddr)
#define ATHR_VLAN_TABLE_SETTING(phyUnit) (athrPhyInfo[phyUnit].VLANTableSetting)

#define ATHR_IS_ETHUNIT(phyUnit, ethUnit) \
            (ATHR_IS_ENET_PORT(phyUnit) &&        \
            ATHR_ETHUNIT(phyUnit) == (ethUnit))

#define ATHR_IS_WAN_PORT(phyUnit) (!(ATHR_ETHUNIT(phyUnit)==ENET_UNIT_LAN))
 
/* Forward references */
BOOL athr_phy_is_link_alive(int phyUnit);
unsigned int athrf1_rd_phy(int ethUnit,unsigned int phy_addr, unsigned int reg_addr);
void athrf1_wr_phy(int ethUnit,unsigned int phy_addr, unsigned int reg_addr, unsigned int write_data);
unsigned int last_phy_speed; 

void athr_enable_linkIntrs(int ethUnit) 
{
    return;
}

void athr_disable_linkIntrs(int ethUnit) 
{
	return;
}
void athr_auto_neg(int ethUnit,int phyUnit) 
{ 
    int timeout = 0;
    uint16_t phyHwStatus;
    
    if(!is_emu()) {
       phy_reg_write(ethUnit, phyUnit , ATHR_PHY_CONTROL, ATHR_CTRL_AUTONEGOTIATION_ENABLE | ATHR_CTRL_SOFTWARE_RESET);
       phy_reg_write(ethUnit, phyUnit , ATHR_AUTONEG_ADVERT, ATHR_ADVERTISE_ALL);
       phy_reg_write(ethUnit, phyUnit , ATHR_1000BASET_CONTROL, (ATHR_ADVERTISE_1000FULL));
       printk("ATHR_AUTONEG_ADVERT:%X\n",phy_reg_read(ethUnit, phyUnit,ATHR_AUTONEG_ADVERT));
       printk("ATHR_1000BASET_CONTROL:%X\n",phy_reg_read(ethUnit, phyUnit,ATHR_1000BASET_CONTROL));
       printk("ATHR_PHY_CONTROL:%X\n",phy_reg_read(ethUnit, phyUnit,ATHR_PHY_CONTROL));
    }
    else {
       phy_reg_write(ethUnit, phyUnit , ATHR_AUTONEG_ADVERT, ATHR_ADVERTISE_ALL);
       /* Do not advertise 1000 */
       phy_reg_write(ethUnit, phyUnit , ATHR_1000BASET_CONTROL,0x0);
       phy_reg_write(ethUnit, phyUnit , ATHR_PHY_CONTROL, ATHR_CTRL_AUTONEGOTIATION_ENABLE | ATHR_CTRL_SOFTWARE_RESET);
    }

   /*
     * Wait up to 3 seconds for ALL associated PHYs to finish
     * autonegotiation.  The only way we get out of here sooner is
     * if ALL PHYs are connected AND finish autonegotiation.
     */
    timeout=20;
    for (;;) {
        phyHwStatus = phy_reg_read(ethUnit, phyUnit, ATHR_PHY_CONTROL);

        if (ATHR_RESET_DONE(phyHwStatus)) {
            printk(MODULE_NAME": Port %d, Neg Success\n", phyUnit);
            break;
        }
        if (timeout == 0) {
            printk(MODULE_NAME": Port %d, Negogiation timeout\n", phyUnit);
            break;
        }
        if (--timeout == 0) {
            printk(MODULE_NAME": Port %d, Negogiation timeout\n", phyUnit);
            break;
        }

        mdelay(150);
    }

    printk(MODULE_NAME": unit %d phy addr %x ", ethUnit, phyUnit);
}

/******************************************************************************
*
* athr_phy_is_link_alive - test to see if the specified link is alive
*
* RETURNS:
*    TRUE  --> link is alive
*    FALSE --> link is down
*/
BOOL
athr_phy_is_link_alive(int ethUnit)
{
	uint16_t phyHwStatus;
	uint32_t phyBase;
	uint32_t phyAddr;
        int phyUnit = 0;

	phyBase = ATHR_PHYBASE(phyUnit);
	phyAddr = ATHR_PHYADDR(phyUnit);
	phyHwStatus = phy_reg_read(ethUnit, phyAddr, ATHR_PHY_SPEC_STATUS);

	if (phyHwStatus & ATHR_STATUS_LINK_PASS) {
		return TRUE;
	}

	return FALSE;
}

/******************************************************************************
*
* athr_phy_setup - reset and setup the PHY associated with
* the specified MAC unit number.
*
* Resets the associated PHY port.
*
* RETURNS:
*    TRUE  --> associated PHY is alive
*    FALSE --> no LINKs on this ethernet unit
*/

BOOL
athr_phy_setup(void *arg)
{
	int       phyUnit = 0;
	int       liveLinks = 0;
        athr_gmac_t *mac = (athr_gmac_t *)arg;
        int ethUnit = mac->mac_unit;
#ifdef DISABLE_8023AZ
        int azStatus, phyAddr = 0;
#endif
	athr_auto_neg(ethUnit,phyUnit);

	if (athr_phy_is_link_alive(phyUnit)) {
		liveLinks++;
		ATHR_IS_PHY_ALIVE(phyUnit) = TRUE;
	} else {
		ATHR_IS_PHY_ALIVE(phyUnit) = FALSE;
	}

#ifdef DISABLE_8023AZ
	printk("checking for the az feature of 8035... \n");  
        athrf1_wr_phy(phyUnit, phyAddr, MMD_CTRL_REG, MMD_FUNC_ADDR | 3); 
        athrf1_wr_phy(phyUnit, phyAddr, MMD_DATA_REG, 0x14);
        athrf1_wr_phy(phyUnit, phyAddr, MMD_CTRL_REG, MMD_FUNC_DATA | 3);
        azStatus = athrf1_rd_phy(phyUnit, phyAddr, MMD_DATA_REG); 
        if (azStatus & 0x6)
        {
	    printk("Disabling 802.3az feature... \n"); 

	    athrf1_wr_phy(phyUnit, phyAddr, MMD_CTRL_REG, MMD_FUNC_ADDR | 0x7);
	    athrf1_wr_phy(phyUnit, phyAddr, MMD_DATA_REG, 0x3c);
	    athrf1_wr_phy(phyUnit, phyAddr, MMD_CTRL_REG, MMD_FUNC_DATA | 0x7);
	    athrf1_wr_phy(phyUnit, phyAddr, MMD_DATA_REG, 0x0);
	    /* restart auto-nego */
	    printk("Restart auto-negotiation\n"); 
	    athrf1_wr_phy(phyUnit, phyAddr, 0, 0x1200);
        }
#endif        
	return (liveLinks > 0);
}

/******************************************************************************
*
* athr_phy_is_fdx - Determines whether the phy ports associated with the
* specified device are FULL or HALF duplex.
*
* RETURNS:
*    1 --> FULL
*    0 --> HALF
*/
int
athr_phy_is_fdx(int ethUnit,int phyUnit)
{
    uint32_t  phyBase;
    uint32_t  phyAddr;
    uint16_t  phyHwStatus;
    int       ii = 200;

    phyUnit = 0; /* F1e has only one phyUnit */

    if (athr_phy_is_link_alive(phyUnit)) {

         phyBase = ATHR_PHYBASE(phyUnit);
         phyAddr = ATHR_PHYADDR(phyUnit);

         do {
                phyHwStatus = phy_reg_read(ethUnit, ATHR_PHYADDR(phyUnit),ATHR_PHY_SPEC_STATUS);
                mdelay(10);
          } while((!(phyHwStatus & ATHR_STATUS_RESOVLED)) && --ii);
          
          if (phyHwStatus & ATHER_STATUS_FULL_DUPLEX) {
                return TRUE;
          }
    }
    return FALSE;
}

/******************************************************************************
*
* athr_phy_speed - Determines the speed of phy ports associated with the
* specified device.
*
* RETURNS:
*               ATHR_PHY_SPEED_10T, ATHR_PHY_SPEED_100T;
*               ATHR_PHY_SPEED_1000T;
*/

int
athr_phy_speed(int ethUnit,int phyUnit)
{
    uint16_t  phyHwStatus;
    uint32_t  phyBase;
    uint32_t  phyAddr;
    int       ii = 200;
    
 
    phyUnit = 0; /* F1phy  has only one phyUnit */


    if (athr_phy_is_link_alive(phyUnit)) {

        phyBase = ATHR_PHYBASE(phyUnit);
        phyAddr = ATHR_PHYADDR(phyUnit);
        do {
            phyHwStatus = phy_reg_read(ethUnit, ATHR_PHYADDR(phyUnit),ATHR_PHY_SPEC_STATUS);
            mdelay(10);
        } while((!(phyHwStatus & ATHR_STATUS_RESOVLED)) && --ii);

        phyHwStatus = ((phyHwStatus & ATHER_STATUS_LINK_MASK) >>
                       ATHER_STATUS_LINK_SHIFT);

        switch(phyHwStatus) {
        case 0:
            if (last_phy_speed != phyHwStatus)
            {
    	       phy_reg_write(ethUnit, ATHR_PHYADDR(phyUnit), ATHR_DEBUG_PORT_ADDRESS, 0x5);
               phy_reg_write(ethUnit, ATHR_PHYADDR(phyUnit), ATHR_DEBUG_PORT_DATA, 0x147);
               last_phy_speed = phyHwStatus;
            }
            return ATHR_PHY_SPEED_10T;
        case 1:
            if (last_phy_speed != phyHwStatus)
            {
    	       phy_reg_write(ethUnit, ATHR_PHYADDR(phyUnit), ATHR_DEBUG_PORT_ADDRESS, 0x5);
               phy_reg_write(ethUnit, ATHR_PHYADDR(phyUnit), ATHR_DEBUG_PORT_DATA, 0x147);
               last_phy_speed = phyHwStatus;
            }
            return ATHR_PHY_SPEED_100T;
            
        case 2:
            if (last_phy_speed != phyHwStatus)
            {
    	        phy_reg_write(ethUnit, ATHR_PHYADDR(phyUnit), ATHR_DEBUG_PORT_ADDRESS, 0x5);
    	        phy_reg_write(ethUnit, ATHR_PHYADDR(phyUnit), ATHR_DEBUG_PORT_DATA, 0x0);
    	        last_phy_speed = phyHwStatus;
            }
            return ATHR_PHY_SPEED_1000T;
            
        default:
            printk("Unkown speed read!\n");
        }
    }
    if (last_phy_speed != phyHwStatus)
    {
        phy_reg_write(ethUnit, ATHR_PHYADDR(phyUnit), ATHR_DEBUG_PORT_ADDRESS, 0x0);
        phy_reg_write(ethUnit, ATHR_PHYADDR(phyUnit), ATHR_DEBUG_PORT_DATA, 0x14e);
        last_phy_speed = phyHwStatus;
    }

    //printk("athr_phy_speed: link down, returning 10t\n");
    return ATHR_PHY_SPEED_10T;
}

/*****************************************************************************
*
* athr_phy_is_up -- checks for significant changes in PHY state.
*
* A "significant change" is:
*     dropped link (e.g. ethernet cable unplugged) OR
*     autonegotiation completed + link (e.g. ethernet cable plugged in)
*
* When a PHY is plugged in, phyLinkGained is called.
* When a PHY is unplugged, phyLinkLost is called.
*/

int
athr_phy_is_up(int ethUnit)
{
    int           phyUnit;
    uint16_t      phyHwStatus, phyHwControl;
    athrPhyInfo_t *lastStatus;
    int           linkCount   = 0;
    int           lostLinks   = 0;
    int           gainedLinks = 0;
    uint32_t      phyBase;
    uint32_t      phyAddr;

    for (phyUnit=0; phyUnit < 1; phyUnit++) {

        phyBase = ATHR_PHYBASE(phyUnit);
        phyAddr = ATHR_PHYADDR(phyUnit);

        lastStatus = &athrPhyInfo[phyUnit];

        if (lastStatus->isPhyAlive) { /* last known link status was ALIVE */

             phyHwStatus = phy_reg_read(ethUnit, ATHR_PHYADDR(phyUnit),ATHR_PHY_SPEC_STATUS);

            /* See if we've lost link */
            if (phyHwStatus & ATHR_STATUS_LINK_PASS) { /* check realtime link */
                linkCount++;
            } else {
                phyHwStatus = phy_reg_read(ethUnit, ATHR_PHYADDR(phyUnit),ATHR_PHY_STATUS);
            /* If realtime failed check link in latch register before
	     * asserting link down.
             */
                if (phyHwStatus & ATHR_LATCH_LINK_PASS)     
                   linkCount++;
		else
                    lostLinks++;
                lastStatus->isPhyAlive = FALSE;
            }
        } else { /* last known link status was DEAD */

            /* Check for reset complete */

                phyHwStatus = phy_reg_read(ethUnit, ATHR_PHYADDR(phyUnit),ATHR_PHY_STATUS);

            if (!ATHR_RESET_DONE(phyHwStatus))
                continue;

                phyHwControl = phy_reg_read(ethUnit, ATHR_PHYADDR(phyUnit),ATHR_PHY_CONTROL);

            /* Check for AutoNegotiation complete */ 
           
            if ((!(phyHwControl & ATHR_CTRL_AUTONEGOTIATION_ENABLE)) 
                 || ATHR_AUTONEG_DONE(phyHwStatus)) {
                    phyHwStatus = phy_reg_read(ethUnit, ATHR_PHYADDR(phyUnit),ATHR_PHY_SPEC_STATUS);

                    if (phyHwStatus & ATHR_STATUS_LINK_PASS) {
                        gainedLinks++;
                        linkCount++;
                        lastStatus->isPhyAlive = TRUE;
                   }  
            }
        }
    }
    return (linkCount);

}
/* Place holders */

static int 
athr_reg_init(void *arg)
{
   return 0;
}
#if 0
int
athrf1e_ioctl(struct net_device *dev, void *args, int cmd)
{

    struct ifreq * ifr = (struct ifreq *) args;
    struct eth_cfg_params *ethcfg;
    athr_gmac_t *mac = ATHR_MAC_PRIV(dev);

    ethcfg = (struct eth_cfg_params *)ifr->ifr_data;

     if(cmd  == RD_PHY) {
            ethcfg->val = phy_reg_read(mac->mac_unit, 0,ethcfg->phy_reg);
    }
    else if(cmd  == WR_PHY) {
            phy_reg_write(mac->mac_unit,0,ethcfg->phy_reg,ethcfg->val);
    }
    else
        return -EINVAL;

    return 0;
}
#endif


unsigned int athrf1_rd_phy(int ethUnit,unsigned int phy_addr, unsigned int reg_addr)
{
		/* force unit if on AR934x board */
    if (is_ar934x()) ethUnit = 0;
    	
    return (phy_reg_read(ethUnit, phy_addr, reg_addr));
}

void athrf1_wr_phy(int ethUnit,unsigned int phy_addr, unsigned int reg_addr, unsigned int write_data)
{
		/* force unit if on AR934x board */
    if (is_ar934x()) ethUnit = 0;
    	
    phy_reg_write(ethUnit, phy_addr, reg_addr, write_data);
}

int athrsf1_register_ops(void *arg)
{
  athr_gmac_t *mac   = (athr_gmac_t *) arg;
  athr_phy_ops_t *ops = mac->phy;

  if(!ops)
     ops = kmalloc(sizeof(athr_phy_ops_t), GFP_KERNEL);

  memset(ops,0,sizeof(athr_phy_ops_t));

  ops->mac            =  mac;
  ops->is_up          =  athr_phy_is_up;
  ops->is_alive       =  athr_phy_is_link_alive;
  ops->speed          =  athr_phy_speed;
  ops->is_fdx         =  athr_phy_is_fdx;
  ops->ioctl          =  NULL;
  ops->setup          =  athr_phy_setup;
  ops->stab_wr        =  NULL;
  ops->link_isr       =  NULL;
  ops->en_link_intrs  =  NULL;
  ops->dis_link_intrs =  NULL;
  ops->read_phy_reg   =  athrf1_rd_phy;
  ops->write_phy_reg  =  athrf1_wr_phy;
  ops->read_mac_reg   =  NULL;
  ops->write_mac_reg  =  NULL;
  ops->init           =  athr_reg_init;

  ops->port_map       =  0x1;
  
  mac->phy = ops;
  
  return 0;
}

