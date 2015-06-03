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

#include "athrs_phy.h"
#include "athrs_mac.h"
#include "athrs27_phy.h"

/* PHY selections and access functions */

typedef enum {
    PHY_SRCPORT_INFO, 
    PHY_PORTINFO_SIZE,
} PHY_CAP_TYPE;

typedef enum {
    PHY_SRCPORT_NONE,
    PHY_SRCPORT_VLANTAG, 
    PHY_SRCPORT_TRAILER,
} PHY_SRCPORT_TYPE;

#define DRV_LOG(DBG_SW, X0, X1, X2, X3, X4, X5, X6)
#define DRV_MSG(x,a,b,c,d,e,f)
#define DRV_PRINT(DBG_SW,X)

#define S27_LAN_PORT_VLAN          1
#define S27_WAN_PORT_VLAN          2
#define ENET_UNIT_LAN 1
#define ENET_UNIT_WAN 0

#define TRUE    1
#define FALSE   0

#define S27_PHY0_ADDR   0x0
#define S27_PHY1_ADDR   0x1
#define S27_PHY2_ADDR   0x2
#define S27_PHY3_ADDR   0x3
#define S27_PHY4_ADDR   0x4

#define MODULE_NAME "ATHRS27"
MODULE_LICENSE("Dual BSD/GPL");

#ifdef S27_PHY_DEBUG
#define DPRINTF_PHY(_fmt,...) do {         \
                printk(MODULE_NAME":"_fmt, __VA_ARGS__);      \
} while (0)
#else
#define DPRINTF_PHY(_fmt,...) 
#endif

extern int phy_in_reset;
extern int athr_gmac_check_link(athr_gmac_t *mac,int phyUnit);

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
#ifdef S27_8023AZ_FEATURE
    int azFeature;
#endif
} athrPhyInfo_t;

static athr_gmac_t *athr_macs[2];
/*
 * Per-PHY information, indexed by PHY unit number.
 */
static athrPhyInfo_t athrPhyInfo[] = {

    {TRUE,   /* port 1 -- LAN port 1 */
     FALSE,
#ifdef CONFIG_ATHR_PHY_SWAP
     ENET_UNIT_WAN,
#else
     ENET_UNIT_LAN,
#endif
     1,
     S27_PHY0_ADDR,
     S27_LAN_PORT_VLAN
    },

    {TRUE,   /* port 2 -- LAN port 2 */
     FALSE,
     ENET_UNIT_LAN,
     1,
     S27_PHY1_ADDR, 
     S27_LAN_PORT_VLAN
    },

    {TRUE,   /* port 3 -- LAN port 3 */
     FALSE,
     ENET_UNIT_LAN,
     1,
     S27_PHY2_ADDR, 
     S27_LAN_PORT_VLAN
    },

    {TRUE,   /* port 4 --  LAN port 4 */
     FALSE,
     ENET_UNIT_LAN,     
     1,
     S27_PHY3_ADDR, 
     S27_LAN_PORT_VLAN   /* Send to all ports */
    },
    
    {TRUE,  /* port 5 -- WAN Port 5 */
     FALSE,
#ifdef CONFIG_ATHR_PHY_SWAP
     ENET_UNIT_LAN,
#else
     ENET_UNIT_WAN,
#endif
     1,
     S27_PHY4_ADDR, 
     S27_LAN_PORT_VLAN    /* Send to all ports */
    },

    {FALSE,   /* port 0 -- cpu port 0 */
     TRUE,
     ENET_UNIT_LAN,
     1,
     0x00,
     S27_LAN_PORT_VLAN
    },

};

static uint8_t athr27_init_flag = 0,athr27_init_flag1 = 0;
static DECLARE_WAIT_QUEUE_HEAD (hd_conf_wait);

#define S27_PHY_MAX 5

/* Range of valid PHY IDs is [MIN..MAX] */
#define S27_ID_MIN 0
#define S27_ID_MAX (S27_PHY_MAX-1)

/* Convenience macros to access myPhyInfo */
#define S27_IS_ENET_PORT(phyUnit) (athrPhyInfo[phyUnit].isEnetPort)
#define S27_IS_PHY_ALIVE(phyUnit) (athrPhyInfo[phyUnit].isPhyAlive)
#define S27_ETHUNIT(phyUnit) (athrPhyInfo[phyUnit].ethUnit)
#define S27_PHYBASE(phyUnit) (athrPhyInfo[phyUnit].phyBase)
#define S27_PHYADDR(phyUnit) (athrPhyInfo[phyUnit].phyAddr)
#define S27_VLAN_TABLE_SETTING(phyUnit) (athrPhyInfo[phyUnit].VLANTableSetting)


#define S27_IS_ETHUNIT(phyUnit, ethUnit) \
            (S27_IS_ENET_PORT(phyUnit) &&        \
            S27_ETHUNIT(phyUnit) == (ethUnit))

#define S27_IS_WAN_PORT(phyUnit) (!(S27_ETHUNIT(phyUnit)==ENET_UNIT_LAN))

void athrs27_powersave_off(int phy_addr)
{
    s27_wr_phy(1,phy_addr,S27_DEBUG_PORT_ADDRESS,0x29);
    s27_wr_phy(1,phy_addr,S27_DEBUG_PORT_DATA,0x36c0);

}

void athrs27_sleep_off(int phy_addr)
{
    s27_wr_phy(1,phy_addr,S27_DEBUG_PORT_ADDRESS,0xb);
    s27_wr_phy(1,phy_addr,S27_DEBUG_PORT_DATA,0x3c00);
}

void athrs27_enable_link_intrs(void *arg)
{
     athr_gmac_t *mac = (athr_gmac_t *)arg;
     int phyUnit = 0, phyAddr = 0;

    /* Enable global PHY link status interrupt */
    athrs27_reg_write(S27_GLOBAL_INTR_MASK_REG,S27_LINK_CHANGE_REG);

    if (mac_has_flag(mac,ETH_SWONLY_MODE)) {
        if (mac->mac_unit == 1) {
            for (phyUnit = 0; phyUnit < S27_PHY_MAX; phyUnit++) {
                phyAddr = S27_PHYADDR(phyUnit);
                s27_wr_phy(1,phyAddr,S27_PHY_INTR_ENABLE,S27_LINK_INTRS);
            }
        }
        return ;
    }

    if (mac->mac_unit == ENET_UNIT_WAN) {
        s27_wr_phy(1,S27_PHY4_ADDR,S27_PHY_INTR_ENABLE,S27_LINK_INTRS);
    }
    else {
        for (phyUnit = 0; phyUnit < S27_PHY_MAX - 1; phyUnit++) {
            phyAddr = S27_PHYADDR(phyUnit);
            s27_wr_phy(1,phyAddr,S27_PHY_INTR_ENABLE,S27_LINK_INTRS);
        }
    }
}

void athrs27_disable_link_intrs(void *arg) 
{
     athr_gmac_t *mac = (athr_gmac_t *)arg;
     int phyUnit = 0, phyAddr = 0;

     if (mac_has_flag(mac,ETH_SWONLY_MODE)) {
         if (mac->mac_unit == 1) {
             for (phyUnit = 0; phyUnit < S27_PHY_MAX; phyUnit++) {
                 phyAddr = S27_PHYADDR(phyUnit);
                 s27_wr_phy(1,phyAddr,S27_PHY_INTR_ENABLE,0x0);
             }
         }
         return ;
     }

     if (mac->mac_unit == ENET_UNIT_WAN) {
         s27_wr_phy(1,S27_PHY4_ADDR,S27_PHY_INTR_ENABLE,0x0);
     } 
     else {
         for (phyUnit = 0; phyUnit < S27_PHY_MAX - 1; phyUnit++) {
             phyAddr = S27_PHYADDR(phyUnit);
             s27_wr_phy(1,phyAddr,S27_PHY_INTR_ENABLE,0x0);
         }
     }
}


void athrs27_force_100M(int phyAddr,int duplex) 
{
   /*
    *  Force MDI and MDX to alternate ports 
    *  Phy 0,2 and 4 -- MDI
    *  Phy 1 and 3 -- MDX
    */

    if(phyAddr%2) {
        s27_wr_phy(1,phyAddr,S27_PHY_FUNC_CONTROL,0x820);
    }
    else { 
        s27_wr_phy(1,phyAddr,S27_PHY_FUNC_CONTROL,0x800);
    }

    s27_wr_phy(1,phyAddr,S27_DEBUG_PORT_ADDRESS,0x29);
    s27_wr_phy(1,phyAddr,S27_DEBUG_PORT_DATA,0x0);
    s27_wr_phy(1,phyAddr,0x10,0xc60);
    s27_wr_phy(1,phyAddr,S27_PHY_CONTROL,(0xa000|(duplex << 8)));
}

void athrs27_force_10M(int phyAddr,int duplex) 
{

    athrs27_powersave_off(phyAddr);
    athrs27_sleep_off(phyAddr);

    s27_wr_phy(1,phyAddr,S27_PHY_CONTROL,(0x8000 |(duplex << 8)));
}


/* 
 * internal init register settings for WAN side
 * (MAC5 if non-swapped)
 */

int athrs27_reg_init(void *arg)
{
    uint32_t rd_data;
    athr_gmac_t *mac = (athr_gmac_t *)arg;

    /* if using header for register configuration, we have to     */
    /* configure s27 register after frame transmission is enabled */
    if (athr27_init_flag)
        return 0;
    
    athrs27_reg_rmw(S27_OPMODE_REG1, S27_PHY4_MII_EN);  /* Set WAN port is connected to GE0 */

#if defined(S27_FORCE_100M)
    athrs27_force_100M(S27_PHY4_ADDR,1);
#elif  defined(S27_FORCE_10M)
    athrs27_force_10M(S27_PHY4_ADDR,1);
#else
    s27_wr_phy(1,S27_PHY4_ADDR,S27_PHY_CONTROL,0x9000);


#endif

    /* Disable WAN mac inside S27 */
    athrs27_reg_write(S27_PORT_STATUS_REGISTER5,0x0);

    /* Enable WAN mac inside S27 */
    if (mac_has_flag(mac,ETH_SWONLY_MODE)) 
        athrs27_reg_write(S27_PORT_STATUS_REGISTER5,0x200);

    /* Enable MDIO Access if PHY is Powered-down */
    s27_wr_phy(1,S27_PHY4_ADDR,S27_DEBUG_PORT_ADDRESS,0x3);
    rd_data = s27_rd_phy(1,S27_PHY4_ADDR,S27_DEBUG_PORT_DATA);
    s27_wr_phy(1,S27_PHY4_ADDR,S27_DEBUG_PORT_ADDRESS,0x3);
    s27_wr_phy(1,S27_PHY4_ADDR,S27_DEBUG_PORT_DATA,(rd_data & 0xfffffeff) );

    if (mac_has_flag(mac, ATHR_S27_HEADER))
        athrs27_reg_write(S27_PORT_CONTROL_REGISTER0, \
          (S27_LEARN_EN | S27_PORT_MODE_FWD | S27_HEADER_EN));
    else
        athrs27_reg_write(S27_PORT_CONTROL_REGISTER0, \
          (S27_LEARN_EN | S27_PORT_MODE_FWD));


    athr27_init_flag = 1;

    DPRINTF(MODULE_NAME":OPERATIONAL_MODE_REG0:%x\n",athrs27_reg_read(S27_OPMODE_REG0));
    DPRINTF(MODULE_NAME":REG 0x4-->:%x\n",athrs27_reg_read(0x4));
    DPRINTF(MODULE_NAME":REG 0x2c-->:%x\n",athrs27_reg_read(0x2c));
    DPRINTF(MODULE_NAME":REG 0x8-->:%x\n",athrs27_reg_read(0x8));

    return 0;
}

/* 
 * LAN side init register settings
 * (MAC1 if non-swapped)
 */
int athrs27_reg_init_lan(void *arg)
{
    int i = 60;
    int       phyUnit; 
    uint32_t  phyBase = 0;
    BOOL      foundPhy = FALSE;
    uint32_t  phyAddr = 0;
    uint32_t  queue_ctrl_reg = 0;
    uint32_t rd_data;
    athr_gmac_t *mac = (athr_gmac_t *)arg;

    /* if using header for register configuration, we have to     */
    /* configure s27 register after frame transmission is enabled */
    if (athr27_init_flag1)
        return 0;
    
    /* reset switch */
    printk(MODULE_NAME ": resetting s27\n");
    athrs27_reg_write(0x0, athrs27_reg_read(0x0)|0x80000000);

    while(i--) {
        mdelay(100);
        if(!(athrs27_reg_read(0x0)&0x80000000))
            break;
    }
    printk(MODULE_NAME ": s27 reset done\n");

    athrs27_reg_write(S27_PORT_STATUS_REGISTER0,0x4e);

   /* XXX FIX ME: S27 results in continious pause frames after few hours
    * running high traffic if flow control is enabled. This
    * might need a  RTL fix, disabling by default as a WAR.
    */

    /* Enable flow control on CPU port */
    athrs27_reg_write(S27_PORT_STATUS_REGISTER0, 
                     (athrs27_reg_read(S27_PORT_STATUS_REGISTER0) | 0x30));
  
    DPRINTF("Port status register read 2:%X\n",athrs27_reg_read(0x100));

       
    athrs27_reg_rmw(S27_OPMODE_REG0, S27_MAC0_MAC_GMII_EN);  /* Set GMII mode */

    if (is_emu() || is_ar934x()) {
       athrs27_reg_rmw(0x2c,((1<<26)| (1<<16) | 0x1));
    } 

    for (phyUnit=0; phyUnit < S27_PHY_MAX; phyUnit++) {

        if ((S27_ETHUNIT(phyUnit) == ENET_UNIT_WAN) &&
            !mac_has_flag(mac,ETH_SWONLY_MODE))
            continue;

        foundPhy = TRUE;
        phyBase = S27_PHYBASE(phyUnit);
        phyAddr = S27_PHYADDR(phyUnit);

#if defined(S27_FORCE_100M)
        athrs27_force_100M(phyAddr,1); 
#elif defined(S27_FORCE_10M)
        athrs27_force_10M(phyAddr,1); 
#else
        s27_wr_phy(1,phyAddr,S27_PHY_CONTROL,0x9000);

        /* Class A setting for 100M */
        s27_wr_phy(1,phyAddr, S27_DEBUG_PORT_ADDRESS, 5);
        s27_wr_phy(1,phyAddr, S27_DEBUG_PORT_DATA, (s27_rd_phy(1,phyAddr, S27_DEBUG_PORT_DATA)&((~2)&0xffff)));
#endif

        /* Enable MDIO Access if PHY is Powered-down */
        s27_wr_phy(1,phyAddr,S27_DEBUG_PORT_ADDRESS,0x3);
        rd_data = s27_rd_phy(1,phyAddr,S27_DEBUG_PORT_DATA);
        s27_wr_phy(1,phyAddr,S27_DEBUG_PORT_ADDRESS,0x3);
        s27_wr_phy(1,phyAddr,S27_DEBUG_PORT_DATA,(rd_data & 0xfffffeff) );

        /* Change HOL settings 
         * 25: PORT_QUEUE_CTRL_ENABLE.
         * 24: PRI_QUEUE_CTRL_EN.
         */
        queue_ctrl_reg = (0x100 * phyUnit) + 0x218 ;
        athrs27_reg_write(queue_ctrl_reg,0x032b5555);
       
        DPRINTF("S27 S27_PHY_FUNC_CONTROL (%d):%x\n",phyAddr,
            s27_rd_phy(1,phyAddr,S27_PHY_FUNC_CONTROL));
        DPRINTF("S27 PHY ID  (%d) :%x\n",phyAddr,
            s27_rd_phy(1,phyAddr,S27_PHY_ID1));
        DPRINTF("S27 PHY CTRL  (%d) :%x\n",phyAddr,
            s27_rd_phy(1,phyAddr,S27_PHY_SPEC_STATUS));
        DPRINTF("S27 ATHR PHY STATUS  (%d) :%x\n",phyAddr,
            s27_rd_phy(1,phyAddr,S27_PHY_STATUS));
    }

    /* initialization for port status */
    athrs27_reg_write(S27_PORT_STATUS_REGISTER1, S27_PORT_STATUS_DEFAULT);  /* LAN - 1 */
    athrs27_reg_write(S27_PORT_STATUS_REGISTER2, S27_PORT_STATUS_DEFAULT);  /* LAN - 2 */
    athrs27_reg_write(S27_PORT_STATUS_REGISTER3, S27_PORT_STATUS_DEFAULT);  /* LAN - 3 */
    athrs27_reg_write(S27_PORT_STATUS_REGISTER4, S27_PORT_STATUS_DEFAULT);  /* LAN - 4 */

    if (is_emu()) {
        athrs27_reg_write(S27_PORT_STATUS_REGISTER1, 0x4C);  /* LAN - 1 */
        athrs27_reg_write(S27_PORT_STATUS_REGISTER2, 0x4c);  /* LAN - 2 */
        athrs27_reg_write(S27_PORT_STATUS_REGISTER3, 0x4c);  /* LAN - 3 */
        athrs27_reg_write(S27_PORT_STATUS_REGISTER4, 0x4c);  /* LAN - 4 */
    }

    /* if the ATHR header is enabled */
    if (mac_has_flag(mac, ATHR_S27_HEADER))
        athrs27_reg_write(S27_PORT_CONTROL_REGISTER0, \
            (S27_LEARN_EN | S27_PORT_MODE_FWD| S27_HEADER_EN));
    else 
        athrs27_reg_write(S27_PORT_CONTROL_REGISTER0, \
            (S27_LEARN_EN | S27_PORT_MODE_FWD));

    /* Tag Priority Mapping */
    athrs27_reg_write(S27_TAGPRI_REG, S27_TAGPRI_DEFAULT);

    /* Enable Broadcast packets to CPU port */
    athrs27_reg_write(S27_FLD_MASK_REG,(athrs27_reg_read(S27_FLD_MASK_REG) | 
                           S27_ENABLE_CPU_BCAST_FWD ));

    /* Turn on back pressure */
    athrs27_reg_write(S27_PORT_STATUS_REGISTER0,
        (athrs27_reg_read(S27_PORT_STATUS_REGISTER0) | 0x80));
    athrs27_reg_write(S27_PORT_STATUS_REGISTER1,
        (athrs27_reg_read(S27_PORT_STATUS_REGISTER1) | 0x80));
    athrs27_reg_write(S27_PORT_STATUS_REGISTER2,
        (athrs27_reg_read(S27_PORT_STATUS_REGISTER2) | 0x80));
    athrs27_reg_write(S27_PORT_STATUS_REGISTER3,
        (athrs27_reg_read(S27_PORT_STATUS_REGISTER3) | 0x80));
    athrs27_reg_write(S27_PORT_STATUS_REGISTER4,
        (athrs27_reg_read(S27_PORT_STATUS_REGISTER4) | 0x80));

    DPRINTF("S27 S27_CPU_PORT_REGISTER :%x\n", athrs27_reg_read ( S27_CPU_PORT_REGISTER ));
    DPRINTF("S27 PORT_STATUS_REGISTER0  :%x\n", athrs27_reg_read ( S27_PORT_STATUS_REGISTER0 ));
    DPRINTF("S27 PORT_STATUS_REGISTER1  :%x\n", athrs27_reg_read ( S27_PORT_STATUS_REGISTER1 ));
    DPRINTF("S27 PORT_STATUS_REGISTER2  :%x\n", athrs27_reg_read ( S27_PORT_STATUS_REGISTER2 ));
    DPRINTF("S27 PORT_STATUS_REGISTER3  :%x\n", athrs27_reg_read ( S27_PORT_STATUS_REGISTER3 ));
    DPRINTF("S27 PORT_STATUS_REGISTER4  :%x\n", athrs27_reg_read ( S27_PORT_STATUS_REGISTER4 ));

    DPRINTF("S27 PORT_CONTROL_REGISTER0 :%x\n", athrs27_reg_read ( S27_PORT_CONTROL_REGISTER0 ));
    DPRINTF("S27 PORT_CONTROL_REGISTER1 :%x\n", athrs27_reg_read ( S27_PORT_CONTROL_REGISTER1 ));
    DPRINTF("S27 PORT_CONTROL_REGISTER2 :%x\n", athrs27_reg_read ( S27_PORT_CONTROL_REGISTER2 ));
    DPRINTF("S27 PORT_CONTROL_REGISTER3 :%x\n", athrs27_reg_read ( S27_PORT_CONTROL_REGISTER3 ));
    DPRINTF("S27 PORT_CONTROL_REGISTER4 :%x\n", athrs27_reg_read ( S27_PORT_CONTROL_REGISTER4 ));

    /* Disable WAN mac (MAC4) inside S27 */
    athrs27_reg_write(S27_PORT_STATUS_REGISTER5, 0x0);
    /*enable igmp/mld protocol packets copy to cpu port*/
    athrs27_reg_write(S27_CPU_PORT_REGISTER, athrs27_reg_read(S27_CPU_PORT_REGISTER)|(1 << 8));

    if(mac_has_flag(mac,ATHR_S27_HEADER))
        /* Set CPU port0 to Atheros Header Enable. */
        athrs27_reg_write(S27_PORT_CONTROL_REGISTER0, \
         athrs27_reg_read(S27_PORT_CONTROL_REGISTER0)|(S27_HEADER_EN));

    /* If s27 is is switch only mode, clear the PHY4_MII enable bit */
    if (mac_has_flag(mac,ETH_SWONLY_MODE)) {
        athrs27_reg_write(S27_OPMODE_REG1, 0x0);
        athrs27_reg_write(S27_PORT_STATUS_REGISTER5, S27_PORT_STATUS_DEFAULT);
    }

    athr27_init_flag1 = 1;
  
    return 0;
}

/******************************************************************************
*
* athrs27_phy_is_link_alive - test to see if the specified link is alive
*
* RETURNS:
*    TRUE  --> link is alive
*    FALSE --> link is down
*/
BOOL
athrs27_phy_is_link_alive(int phyUnit)
{
    uint16_t phyHwStatus;
    uint32_t phyBase;
    uint32_t phyAddr;
#ifdef S27_8023AZ_FEATURE
    uint32_t azStatus;
    athr_gmac_t *mac = athr_macs[S27_ETHUNIT(phyUnit)];
#endif

    phyBase = S27_PHYBASE(phyUnit);
    phyAddr = S27_PHYADDR(phyUnit);
    phyHwStatus = s27_rd_phy(1, phyAddr, S27_PHY_SPEC_STATUS);

#ifdef S27_8023AZ_FEATURE
    if (athrPhyInfo[phyUnit].azFeature & S27_8023AZ_PHY_ENABLED)
    {
        if (phyHwStatus & S27_STATUS_LINK_PASS)
        {
            if (athrs27_reg_read(S27_LPI_CTRL_PORT_1 + 0x100*phyUnit) & S27_LPI_ENABLED)
            {
                DPRINTF_PHY("(az)Port %d is linked with az feature\n", phyUnit);
                
                while (1)
                {
                    phyHwStatus = s27_rd_phy(1, phyAddr, S27_PHY_SPEC_STATUS);
                    if (phyHwStatus & S27_STATUS_RESOLVED) break;
                }
                
                if ((phyHwStatus >> S27_STATUS_LINK_SHIFT) == S27_STATUS_LINK_100M) /* linked @ 100M */
                {
                    DPRINTF_PHY("(az)Port %d is linked @ 100Mbps \n", phyUnit); 
                }else if ((phyHwStatus >> S27_STATUS_LINK_SHIFT) == S27_STATUS_LINK_10M) /* linked @ 10M */
                {
                    DPRINTF_PHY("(az)Port %d is linked @ 10Mbps \n", phyUnit); 
                }else
                {
                    DPRINTF_PHY("(az)Port %d in linked with unknown speed... \n", phyUnit);
                }
            }
            athrPhyInfo[phyUnit].azFeature |= S27_8023AZ_PHY_LINKED;
            return TRUE; /* linked */
        }else /* not linked */
        {
            DPRINTF_PHY("(az)Phy port %d is disconnedted from az partner\n", phyUnit);
            DPRINTF_PHY("(az)Turn off switch MAC LPI feature (%d)\n", phyUnit); 
            athrs27_reg_write(S27_LPI_CTRL_PORT_1 + 0x100*phyUnit, \
                (athrs27_reg_read(S27_LPI_CTRL_PORT_1 + 0x100*phyUnit) & ~S27_LPI_ENABLED)); 
            /* send the link pulse packet */
            DPRINTF_PHY("(az)Send link pulse packet (Phy: %d)\n", phyUnit);
                
            athrs27_reg_write(S27_PORT_STATUS_REGISTER1 + 0x100*phyUnit, 0x7d);
                
            if ( (S27_ETHUNIT(phyUnit) == ENET_UNIT_WAN) && \
                     mac_has_flag(mac, ETH_SWONLY_MODE))
            {
                DPRINTF_PHY("(az)AR934x GMAC5 with phyUnit: %d\n", phyUnit); 
            	athrs27_reg_write(S27_FLCTL_REG1, 0x162020);
              	athrs27_reg_write(S27_FLCTL_REG1, 0x160020);
               	athrs27_reg_write(S27_FLCTL_REG1, 0x162020);
               	athrs27_reg_write(S27_FLCTL_REG1, 0x16162020);
            }else
            {
               	athrs27_reg_write(S27_FLCTL_REG1, 0x16002020);
               	athrs27_reg_write(S27_FLCTL_REG1, 0x16002000);
               	athrs27_reg_write(S27_FLCTL_REG1, 0x16002020);
               	athrs27_reg_write(S27_FLCTL_REG1, 0x16162020);
            }
                
            athrs27_reg_write(S27_PORT_STATUS_REGISTER1 + 0x100*phyUnit, 0x1280);
            mdelay(200); 
            
            DPRINTF_PHY("(az)LPI register %.8x = %.8x\n", S27_LPI_CTRL_PORT_1 + 0x100*phyUnit,
                    athrs27_reg_read(S27_LPI_CTRL_PORT_1 + 0x100*phyUnit));
                
            /* unset az flags */
            athrPhyInfo[phyUnit].azFeature &= ~S27_8023AZ_PHY_ENABLED;
            athrPhyInfo[phyUnit].azFeature &= ~S27_8023AZ_PHY_LINKED; 
            return FALSE; 
        }
    }else  /* no AZ... yet */ 
    {
        if (phyHwStatus & S27_STATUS_LINK_PASS) 
        {
            DPRINTF_PHY("(az)Port %d is linked @ mode %x \n", phyUnit, phyHwStatus >> S27_STATUS_LINK_SHIFT); 
            s27_wr_phy(1, phyUnit, S27_MMD_CTRL_REG, S27_MMD_FUNC_ADDR | 0x7);
            s27_wr_phy(1, phyUnit, S27_MMD_DATA_REG, 0x8000);
            s27_wr_phy(1, phyUnit, S27_MMD_CTRL_REG, S27_MMD_FUNC_DATA | 0x7);
            azStatus = s27_rd_phy(1, phyUnit, S27_MMD_DATA_REG);
            
            if (azStatus & 0x2)
            {
                DPRINTF_PHY("[az]Port %d link partner has 802.3az feature, enable LPI\n", phyUnit);
                DPRINTF_PHY("(az)Turn on switch MAC LPI feature (phy: %d) \n", phyUnit); 
                mdelay(200); 
                athrs27_reg_write(S27_LPI_CTRL_PORT_1 + 0x100*phyUnit, \
                          S27_LPI_ENABLED | S27_LPI_WAKEUP_TIMER); 
                          
                athrPhyInfo[phyUnit].azFeature |= S27_8023AZ_PHY_ENABLED;
            }else
            {
                DPRINTF_PHY("[az]Port %d link partner does not have 802.2az feature. az status: %.8x\n", phyUnit, azStatus);
            }
            athrPhyInfo[phyUnit].azFeature |= S27_8023AZ_PHY_LINKED; 

            return TRUE;
        }else
        {
            athrPhyInfo[phyUnit].azFeature &= ~S27_8023AZ_PHY_LINKED;
            return FALSE;
        }
    }
#else
    if (phyHwStatus & S27_STATUS_LINK_PASS) 
        return TRUE;

    return FALSE;
#endif /* S27_8023AZ_FEATURE */
    
}

/******************************************************************************
*
* athrs27_phy_setup - reset and setup the PHY associated with
* the specified MAC unit number.
*
* Resets the associated PHY port.
*
* RETURNS:
*    TRUE  --> associated PHY is alive
*    FALSE --> no LINKs on this ethernet unit
*/

int
athrs27_phy_setup(void *arg)
{
    int       phyUnit;
    uint16_t  phyHwStatus;
    uint16_t  timeout;
    int       liveLinks = 0;
    uint32_t  phyBase = 0;
    BOOL      foundPhy = FALSE;
    uint32_t  phyAddr = 0;
    athr_gmac_t *mac = (athr_gmac_t *)arg;
    int ethUnit = mac->mac_unit;


    /* See if there's any configuration data for this enet */
    /* start auto negogiation on each phy */
    for (phyUnit=0; phyUnit < S27_PHY_MAX; phyUnit++) {

        foundPhy = TRUE;
        phyBase = S27_PHYBASE(phyUnit);
        phyAddr = S27_PHYADDR(phyUnit);

        if (!S27_IS_ETHUNIT(phyUnit, ethUnit)) {
            continue;
        }
        if (!is_emu()) {
           s27_wr_phy(1,phyAddr, S27_AUTONEG_ADVERT,S27_ADVERTISE_ALL);

           s27_wr_phy(1, phyAddr, S27_PHY_CONTROL,S27_CTRL_AUTONEGOTIATION_ENABLE
                         | S27_CTRL_SOFTWARE_RESET);
        }
        else {
      
           if(S27_ETHUNIT(phyUnit) == ENET_UNIT_WAN) {
               s27_wr_phy(1,phyAddr, S27_AUTONEG_ADVERT,S27_ADVERTISE_ALL);
               s27_wr_phy(1,phyAddr,0x9, 0x0); //donot advertise 1000Mbps mode
               s27_wr_phy(1, phyAddr, S27_PHY_CONTROL,0x0);
               s27_wr_phy(1, phyAddr, S27_PHY_CONTROL,S27_CTRL_AUTONEGOTIATION_ENABLE
                         | S27_CTRL_SOFTWARE_RESET);
           }
           else {

               s27_wr_phy(1,phyAddr, S27_AUTONEG_ADVERT,(S27_ADVERTISE_ASYM_PAUSE | S27_ADVERTISE_PAUSE |
                            S27_ADVERTISE_10HALF | S27_ADVERTISE_10FULL));
               s27_wr_phy(1,phyAddr,0x9, 0x0); //donot advertise 1000Mbps mode
               s27_wr_phy(1, phyAddr, S27_PHY_CONTROL,0x0);
               s27_wr_phy(1, phyAddr, S27_PHY_CONTROL,S27_CTRL_AUTONEGOTIATION_ENABLE
                         | S27_CTRL_SOFTWARE_RESET);
           }
       }
       DPRINTF("%s S27_PHY_CONTROL %d :%x\n",__func__,phyAddr,
          s27_rd_phy(1,phyAddr,S27_PHY_CONTROL));
       DPRINTF("%s S27_PHY_SPEC_STAUS %d :%x\n",__func__,phyAddr,
          s27_rd_phy(1,phyAddr,S27_PHY_SPEC_STATUS));
    }

    if (!foundPhy) {
        return FALSE; /* No PHY's configured for this ethUnit */
    }

    /*
     * After the phy is reset, it takes a little while before
     * it can respond properly.
     */
    if (ethUnit == ENET_UNIT_LAN)
        mdelay(1000);
    else
        mdelay(3000);

    /*
     * Wait up to 3 seconds for ALL associated PHYs to finish
     * autonegotiation.  The only way we get out of here sooner is
     * if ALL PHYs are connected AND finish autonegotiation.
     */
    for (phyUnit=0; (phyUnit < S27_PHY_MAX) /*&& (timeout > 0) */; phyUnit++) {

        if ((S27_ETHUNIT(phyUnit) == ENET_UNIT_WAN) &&
                !mac_has_flag(mac,ETH_SWONLY_MODE))
            continue;

        timeout=20;
        for (;;) {
            phyHwStatus =  s27_rd_phy(1,phyAddr, S27_PHY_CONTROL);

            if (S27_RESET_DONE(phyHwStatus)) {
                DRV_PRINT(DRV_DEBUG_PHYSETUP,
                          ("Port %d, Neg Success\n", phyUnit));
                break;
            }
            if (timeout == 0) {
                DRV_PRINT(DRV_DEBUG_PHYSETUP,
                          ("Port %d, Negogiation timeout\n", phyUnit));
                break;
            }
            if (--timeout == 0) {
                DRV_PRINT(DRV_DEBUG_PHYSETUP,
                          ("Port %d, Negogiation timeout\n", phyUnit));
                break;
            }

            mdelay(150);
        }
        /* extend the cable length */
        s27_wr_phy(1, phyUnit, S27_DEBUG_PORT_ADDRESS, 0x14);
        s27_wr_phy(1, phyUnit, S27_DEBUG_PORT_DATA, 0xf52); 
        
       /* Force Class A setting phys */
        s27_wr_phy(1, phyUnit, S27_DEBUG_PORT_ADDRESS, 4);
        s27_wr_phy(1, phyUnit, S27_DEBUG_PORT_DATA, 0xebbb); 
        s27_wr_phy(1, phyUnit, S27_DEBUG_PORT_ADDRESS, 5);
        s27_wr_phy(1, phyUnit, S27_DEBUG_PORT_DATA, 0x2c47);
    }

    /*
     * All PHYs have had adequate time to autonegotiate.
     * Now initialize software status.
     *
     * It's possible that some ports may take a bit longer
     * to autonegotiate; but we can't wait forever.  They'll
     * get noticed by mv_phyCheckStatusChange during regular
     * polling activities.
     */
    for (phyUnit=0; phyUnit < S27_PHY_MAX; phyUnit++) {
        if (!S27_IS_ETHUNIT(phyUnit, ethUnit)) {
            continue;
        }

        if (athrs27_phy_is_link_alive(phyUnit)) {
            liveLinks++;
            S27_IS_PHY_ALIVE(phyUnit) = TRUE;
        } else {
            S27_IS_PHY_ALIVE(phyUnit) = FALSE;
        }
        DRV_PRINT(DRV_DEBUG_PHYSETUP,
            ("eth%d: Phy Specific Status=%4.4x\n",
            ethUnit, 
            s27_rd_phy(1,S27_PHYADDR(phyUnit),S27_PHY_SPEC_STATUS)));
       
    }
#ifdef S27_8023AZ_FEATURE
    if (!athrPhyInfo[0].azFeature)
    {
        /* Enable 802.3az feature */
        for (phyUnit = 0; phyUnit < S27_PHY_MAX; phyUnit++) {
            /* do not turn on the WAN port 802.3az feature */
            if ( (S27_ETHUNIT(phyUnit) == ENET_UNIT_WAN) && \
                 !mac_has_flag(mac,ETH_SWONLY_MODE) ) 
                 continue; 
            
            DPRINTF_PHY("(az)Enabling 802.3az feature for port %d\n", phyUnit);
            s27_wr_phy(1, phyUnit, S27_MMD_CTRL_REG, S27_MMD_FUNC_ADDR | 0x3);
            s27_wr_phy(1, phyUnit, S27_MMD_DATA_REG, 0x8007);
            s27_wr_phy(1, phyUnit, S27_MMD_CTRL_REG, S27_MMD_FUNC_DATA | 0x3);
            s27_wr_phy(1, phyUnit, S27_MMD_DATA_REG, 0x30);
            s27_wr_phy(1, phyUnit, S27_MMD_CTRL_REG, S27_MMD_FUNC_ADDR | 0x7);
            s27_wr_phy(1, phyUnit, S27_MMD_DATA_REG, 0x3c);
            s27_wr_phy(1, phyUnit, S27_MMD_CTRL_REG, S27_MMD_FUNC_DATA | 0x7);
            s27_wr_phy(1, phyUnit, S27_MMD_DATA_REG, 0x2);
            DPRINTF_PHY("(az)Send restart auto-nego for port %d\n", phyUnit);         
            s27_wr_phy(1, phyUnit, S27_PHY_CONTROL, \
                (S27_CTRL_AUTONEGOTIATION_ENABLE | S27_CTRL_RESTART_AUTONEGOTIATION));
                
        }
        DPRINTF_PHY("(az)End of az enabling ... (%d)\n", ethUnit); 
    }
    /* end of 802.3az feature */
#endif

    if (mac_has_flag(mac, ETH_SOFT_LED)) {
        athrs27_reg_write(0x80, 1 << 30);
    }

    return (liveLinks > 0);
}

/******************************************************************************
*
* athrs27_phy_is_fdx - Determines whether the phy ports associated with the
* specified device are FULL or HALF duplex.
*
* RETURNS:
*    1 --> FULL
*    0 --> HALF
*/
int
athrs27_phy_is_fdx(int ethUnit,int phyUnit)
{
    uint32_t    phyBase;
    uint32_t    phyAddr;
    uint16_t    phyHwStatus;
    int         ii = 200;
    athr_gmac_t *mac = athr_macs[ethUnit];

    for (phyUnit=0; phyUnit < S27_PHY_MAX; phyUnit++) {
        if (!S27_IS_ETHUNIT(phyUnit, ethUnit) &&
            !mac_has_flag(mac,ETH_SWONLY_MODE)) {
            continue;
        }

        if (athrs27_phy_is_link_alive(phyUnit)) {

            phyBase = S27_PHYBASE(phyUnit);
            phyAddr = S27_PHYADDR(phyUnit);

            do {
                phyHwStatus = s27_rd_phy (1, phyAddr, 
                                               S27_PHY_SPEC_STATUS);
		        if(phyHwStatus & S27_STATUS_RESOLVED)
			        break;
                mdelay(10);
            } while(--ii);
            if (phyHwStatus & S27_STATUS_FULL_DUPLEX) {
                return TRUE;
            }
        }
    }

    return FALSE;
}

/******************************************************************************
*
* athrs27_phy_speed - Determines the speed of phy ports associated with the
* specified device.
*
* RETURNS:
*               S27_PHY_SPEED_10T, AG7240_PHY_SPEED_100T;
*               S27_PHY_SPEED_1000T;
*/

int
athrs27_phy_speed(int ethUnit,int phyUnit)
{
    uint16_t  phyHwStatus;
    uint32_t  phyBase;
    uint32_t  phyAddr;
    int       ii = 200;
    athr_phy_speed_t phySpeed = ATHR_PHY_SPEED_10T;
    athr_gmac_t *mac = athr_macs[ethUnit];

    for (phyUnit=0; phyUnit < S27_PHY_MAX; phyUnit++) {
        if (!S27_IS_ETHUNIT(phyUnit, ethUnit) &&
            !mac_has_flag(mac,ETH_SWONLY_MODE)) {
            continue;
        }

        phyBase = S27_PHYBASE(phyUnit);
        phyAddr = S27_PHYADDR(phyUnit);

        if (athrs27_phy_is_link_alive(phyUnit)) {

            do {
                phyHwStatus = s27_rd_phy(1, phyAddr, 
                                              S27_PHY_SPEC_STATUS);
		        if(phyHwStatus & S27_STATUS_RESOLVED)
			        break;
                mdelay(10);
            }while(--ii);

            phyHwStatus = ((phyHwStatus & S27_STATUS_LINK_MASK) >>
                           S27_STATUS_LINK_SHIFT);

            phySpeed = ATHR_PHY_SPEED_10T;

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
                printk("Unkown speed read!\n");
            }
        }


        
    }

    return phySpeed;

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
athrs27_phy_is_up(int ethUnit)
{

    uint16_t      phyHwStatus, phyHwControl;
    athrPhyInfo_t *lastStatus;
    int           linkCount   = 0;
    int           lostLinks   = 0;
    int           gainedLinks = 0;
    uint32_t      phyBase;
    uint32_t      phyAddr;
    int           phyUnit;
    athr_gmac_t   *mac = athr_macs[ethUnit];

    for (phyUnit=0; phyUnit < S27_PHY_MAX; phyUnit++) {
        if (!S27_IS_ETHUNIT(phyUnit, ethUnit) &&
            !mac_has_flag(mac,ETH_SWONLY_MODE)) {
            continue;
        }

        phyBase = S27_PHYBASE(phyUnit);
        phyAddr = S27_PHYADDR(phyUnit);

        lastStatus = &athrPhyInfo[phyUnit];

        if (lastStatus->isPhyAlive) { /* last known link status was ALIVE */
            phyHwStatus = s27_rd_phy(1, phyAddr, S27_PHY_SPEC_STATUS);

            /* See if we've lost link */
            if (phyHwStatus & S27_STATUS_LINK_PASS) {
                linkCount++;
            } else {
                lostLinks++;
                DRV_PRINT(DRV_DEBUG_PHYCHANGE,("\nenet%d port%d down\n",
                                               ethUnit, phyUnit));
                printk("enet%d port%d down\n",ethUnit, phyUnit);
                lastStatus->isPhyAlive = FALSE;
            }
        } else { /* last known link status was DEAD */
            /* Check for reset complete */
	    if (is_emu())  
	    {
	        phyHwStatus = s27_rd_phy(1, phyAddr, S27_PHY_STATUS);
	        if(phyAddr % 2) {
                    s27_wr_phy(1,phyAddr,S27_PHY_FUNC_CONTROL,0x820);
                }		
                else {
                    s27_wr_phy(1,phyAddr,S27_PHY_FUNC_CONTROL,0x800);
                }

	        if((phyHwStatus & 0x4)==0)
	        {
	           s27_wr_phy(1,phyAddr,0x9,0x0);
	           if(phyAddr !=0x4)
	               s27_wr_phy(1,phyAddr,0x4,0x41);
	           s27_wr_phy(1,phyAddr,0x0,0x9000);
	        }
	    }
		
	    phyHwStatus = s27_rd_phy(1, phyAddr, S27_PHY_STATUS);
            if (!S27_RESET_DONE(phyHwStatus))
                continue;

            phyHwControl = s27_rd_phy(1, phyAddr, S27_PHY_CONTROL);
            /* Check for AutoNegotiation complete */
            if ((!(phyHwControl & S27_CTRL_AUTONEGOTIATION_ENABLE)) 
                 || S27_AUTONEG_DONE(phyHwStatus)) {
                phyHwStatus = s27_rd_phy(1, phyAddr, 
                                           S27_PHY_SPEC_STATUS);

                if (phyHwStatus & S27_STATUS_LINK_PASS) {
                gainedLinks++;
                linkCount++;
                printk("enet%d port%d up\n",ethUnit, phyUnit);
                DRV_PRINT(DRV_DEBUG_PHYCHANGE,("\nenet%d port%d up\n",
                                               ethUnit, phyUnit));
                lastStatus->isPhyAlive = TRUE;
                }
            }
        }
    }
    return (linkCount);

}

unsigned int athrs27_reg_read(unsigned int s27_addr)
{
    unsigned int addr_temp;
    unsigned int s27_rd_csr_low, s27_rd_csr_high, s27_rd_csr;
    unsigned int data,unit = 0;
    unsigned int phy_address, reg_address;

    addr_temp = s27_addr >>2;
    data = addr_temp >> 7;

    phy_address = 0x1f;
    reg_address = 0x10;

    if (is_ar7240()) {
        unit = 0;
    } 
    else if(is_ar7241() || is_ar7242() || is_ar934x()) {
        unit = 1;
    }

    phy_reg_write(unit,phy_address, reg_address, data);

    phy_address = (0x17 & ((addr_temp >> 4) | 0x10));
    reg_address = ((addr_temp << 1) & 0x1e);
    s27_rd_csr_low = (uint32_t) phy_reg_read(unit,phy_address, reg_address);

    reg_address = reg_address | 0x1;
    s27_rd_csr_high = (uint32_t) phy_reg_read(unit,phy_address, reg_address);
    s27_rd_csr = (s27_rd_csr_high << 16) | s27_rd_csr_low ;

    return(s27_rd_csr);
}

void athrs27_reg_write(unsigned int s27_addr, unsigned int s27_write_data)
{
    unsigned int addr_temp;
    unsigned int data;
    unsigned int phy_address, reg_address,unit = 0;

    addr_temp = (s27_addr ) >>2;
    data = addr_temp >> 7;

    phy_address = 0x1f;
    reg_address = 0x10;

    if (is_ar7240()) {
        unit = 0;
    }
    else if(is_ar7241() || is_ar7242() || is_ar934x()) {
        unit = 1;
    }

    phy_reg_write(unit,phy_address, reg_address, data);

    phy_address = (0x17 & ((addr_temp >> 4) | 0x10));

    reg_address = (((addr_temp << 1) & 0x1e) | 0x1);
    data = (s27_write_data >> 16) & 0xffff;
    phy_reg_write(unit,phy_address, reg_address, data);

    reg_address = ((addr_temp << 1) & 0x1e);
    data = s27_write_data  & 0xffff;
    phy_reg_write(unit,phy_address, reg_address, data);
}

void athrs27_reg_rmw(unsigned int s27_addr, unsigned int s27_write_data)
{
    int val = athrs27_reg_read(s27_addr);
    athrs27_reg_write(s27_addr,(val | s27_write_data));
}

unsigned int s27_rd_phy(int ethUnit,unsigned int phy_addr, unsigned int reg_addr)
{
     unsigned int rddata, i = 100;
     unsigned long flags; 
     
     if(phy_addr >= S27_PHY_MAX) {
         DPRINTF("%s:Error invalid Phy Address:0x%x\n",__func__,phy_addr);
         return -1 ;
     }

     local_irq_save(flags); 

    /* MDIO_CMD is set for read */
    rddata = athrs27_reg_read(S27_MDIO_CTRL_REGISTER);
    rddata = (rddata & 0x0) | (reg_addr << S27_MDIO_REG_ADDR) 
              | (phy_addr << S27_MDIO_PHY_ADDR) | S27_MDIO_CMD_RD
              | S27_MDIO_MASTER | S27_MDIO_BUSY ;

    athrs27_reg_write(S27_MDIO_CTRL_REGISTER, rddata);

    rddata = athrs27_reg_read(S27_MDIO_CTRL_REGISTER);
    rddata = rddata & S27_MDIO_BUSY;

    /* Check MDIO_BUSY status */
    while(rddata && --i){
        rddata = athrs27_reg_read(S27_MDIO_CTRL_REGISTER);
        rddata = rddata & S27_MDIO_BUSY;
    }

    if(i <= 0)
      printk("ERROR:%s failed:phy:%d reg:%X rd_data:%X\n",
                __func__,phy_addr,reg_addr,rddata);
    /* Read the data from phy */

    rddata = athrs27_reg_read(S27_MDIO_CTRL_REGISTER);
    rddata = rddata & 0xffff;
    
    local_irq_restore(flags); 
    
    return(rddata);
}

void s27_wr_phy(int ethUnit,unsigned int phy_addr, unsigned int reg_addr, unsigned int write_data)
{
    unsigned int rddata,i = 100;
    unsigned long flags; 
    
    if(phy_addr >= S27_PHY_MAX) {
        DPRINTF("%s:Error invalid Phy Address:0x%x\n",__func__,phy_addr);
        return ;
    }

    local_irq_save(flags);
    
    /* MDIO_CMD is set for read */
    rddata = athrs27_reg_read(S27_MDIO_CTRL_REGISTER);
    rddata = (rddata & 0x0) | (write_data & 0xffff)
               | (reg_addr << S27_MDIO_REG_ADDR) 
               | (phy_addr << S27_MDIO_PHY_ADDR) | S27_MDIO_CMD_WR
               | S27_MDIO_MASTER | S27_MDIO_BUSY;


    athrs27_reg_write(S27_MDIO_CTRL_REGISTER, rddata);


    rddata = athrs27_reg_read(S27_MDIO_CTRL_REGISTER);
    rddata = rddata & S27_MDIO_BUSY;

    /* Check MDIO_BUSY status */
    while(rddata && --i){
        rddata = athrs27_reg_read(S27_MDIO_CTRL_REGISTER);
        rddata = rddata & S27_MDIO_BUSY;
    }
    
    local_irq_restore(flags); 
    
    if(i <= 0)
      printk("ERROR:%s failed:phy:%d reg%X\n",__func__,phy_addr,reg_addr);
}

void ar7240_s27_intr(void *arg)
{
    int status = 0, intr_reg_val;
    uint32_t phyUnit = 0 ,phyBase = 0 ,phyAddr = 0;
    uint32_t phymask = 0x0;
    uint32_t linkDown = 0x0;
    athr_gmac_t *mac0 = athr_macs[0];
    athr_gmac_t *mac1 = athr_macs[1];


    athrs27_reg_write(S27_GLOBAL_INTR_MASK_REG,0x0);

    intr_reg_val = athrs27_reg_read(S27_GLOBAL_INTR_REG);


    /* clear global link interrupt */
    athrs27_reg_write(S27_GLOBAL_INTR_REG,intr_reg_val);

    if (intr_reg_val & S27_LINK_CHANGE_REG) {

       for (phyUnit=0; phyUnit < S27_PHY_MAX; phyUnit++) {

           phyBase = S27_PHYBASE(phyUnit);
           phyAddr = S27_PHYADDR(phyUnit);
           status = s27_rd_phy(1,phyAddr,S27_PHY_INTR_STATUS);

           if(status & S27_LINK_UP) {
               DPRINTF("LINK UP - Port %d:%x\n",phyAddr,status);
               phymask = (phymask | (1 << phyUnit));
           }
           if(status & S27_LINK_DOWN) {
               DPRINTF("LINK DOWN - Port %d:%x\n",phyAddr,status);
               phymask = (phymask | (1 << phyUnit));
               linkDown = (linkDown | (1 << phyUnit));
           }
           if(status & S27_LINK_DUPLEX_CHANGE) {
               DPRINTF("LINK DUPLEX CHANGE - Port %d:%x\n",phyAddr,status);
               phymask = (phymask | (1 << phyUnit));
           }
           if(status & S27_LINK_SPEED_CHANGE) {
               DPRINTF("LINK SPEED CHANGE %d:%x\n",phyAddr,status);
               phymask = (phymask | (1 << phyUnit));
           }
       }
       for (phyUnit=0; phyUnit < S27_PHY_MAX; phyUnit++) {
           if ((phymask >> phyUnit) & 0x1) {

               phyBase = S27_PHYBASE(phyUnit);
               phyAddr = S27_PHYADDR(phyUnit);

               status = s27_rd_phy(1,phyAddr,S27_PHY_INTR_STATUS);

               if (!athrs27_phy_is_link_alive(phyUnit) && !((linkDown >> phyUnit) & 0x1))
                        continue;

               if(S27_ETHUNIT(phyUnit) == ENET_UNIT_WAN) {
                    if (mac_has_flag(mac1,ETH_SWONLY_MODE)) 
                        mac1->ops->check_link(mac1,phyUnit); 
                    else if (mac_has_flag(mac1,ATHR_DUAL_PHY)) 
                        continue; 
                    else 
                        mac0->ops->check_link(mac0,phyUnit); 
               }
               else {
                    mac1->ops->check_link(mac1,phyUnit);
               }
           }
       }
       athrs27_reg_write(S27_GLOBAL_INTR_MASK_REG,S27_LINK_CHANGE_REG);
   }
   else  {
      DPRINTF("Spurious link interrupt:%s,status:%x\n",__func__,status);
      athrs27_reg_write(S27_GLOBAL_INTR_MASK_REG,S27_LINK_CHANGE_REG);
   }

}

int athrs27_register_ops(void *arg)
{
  athr_gmac_t *mac   = (athr_gmac_t *) arg;
  athr_phy_ops_t *ops = mac->phy;

  if(!ops) 
     ops = kmalloc(sizeof(athr_phy_ops_t), GFP_KERNEL);

  memset(ops,0,sizeof(athr_phy_ops_t));

  ops->mac            =  mac;
  ops->is_up          =  athrs27_phy_is_up;
  ops->is_alive       =  athrs27_phy_is_link_alive;
  ops->speed          =  athrs27_phy_speed;
  ops->is_fdx         =  athrs27_phy_is_fdx;
  ops->ioctl          =  NULL;
  ops->setup          =  athrs27_phy_setup;
  ops->stab_wr        =  NULL;
  ops->link_isr       =  ar7240_s27_intr;
  ops->en_link_intrs  =  athrs27_enable_link_intrs;
  ops->dis_link_intrs =  athrs27_disable_link_intrs;
  ops->read_phy_reg   =  s27_rd_phy;
  ops->write_phy_reg  =  s27_wr_phy;
  ops->read_mac_reg   =  athrs27_reg_read;
  ops->write_mac_reg  =  athrs27_reg_write;

  if (mac->mac_unit == 0) {
     ops->init        = athrs27_reg_init;
     athr_macs[0]     = mac;
     ops->arg_ad[0]   = (void *)mac;
#ifdef CONFIG_ATHR_PHY_SWAP
		 ops->port_map    = 0x61; /* port 5,6, and 0 */
#else     
     ops->port_map    = 0x70; /*set GE0 and port - 4,5, 6 - eth0*/
#endif

     if(athr_macs[1])
     {
        ops->arg_ad[1]  = athr_macs[1];
        athr_macs[1]->phy->arg_ad[0] = mac;
     }
  }
  else {
     ops->init        = athrs27_reg_init_lan;
     athr_macs[1]     = mac;
     ops->arg_ad[1]   =(void *)mac;

     if(athr_macs[0])
     {
        ops->arg_ad[0]  = athr_macs[0];
        athr_macs[0]->phy->arg_ad[1] = mac;
     }

     if (mac_has_flag(mac,ETH_SWONLY_MODE))
        ops->port_map  = 0xbf; /* set GE1 port 0-4 5 - cpu port 7 - eth1 */ 
     else 
#ifdef CONFIG_ATHR_PHY_SWAP
        ops->port_map  = 0xbe; 
#else     	
        ops->port_map  = 0xaf; /* set GE1 port 0-3 and 5 7 - eth1*/ 
#endif
  }
  mac->phy = ops;

  return 0;
}
