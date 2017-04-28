/*
 * This file is subject to the terms and conditions of the GNU General Public
 * License.  See the file "COPYING" in the main directory of this archive
 * for more details.
 *
 * Copyright © 2007 Atheros Communications, Inc.,  All Rights Reserved.
 */

/*
 * Manage the atheros ethernet PHY.
 *
 * All definitions in this file are operating system independent!
 */
#include "athrs_ioctl.h"
#include "athrs_phy.h"
#include "athrs_mac.h"
#include "athrs16_phy.h"
#include "athrs_phy_ctrl.h"

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

#define ATHR_LAN_PORT_VLAN          1
#define ATHR_WAN_PORT_VLAN          2


/*depend on connection between cpu mac and s16 mac*/
#if defined (CONFIG_PORT0_AS_SWITCH)
#define ENET_UNIT_LAN 0  
#define ENET_UNIT_WAN 1
#define CFG_BOARD_AP96 1

#elif defined (CONFIG_AG7240_GE0_IS_CONNECTED)
#define ENET_UNIT_LAN 0  
#define CFG_BOARD_PB45 0
#define CFG_BOARD_AP96 1

#else
#define ENET_UNIT_LAN 1  
#define ENET_UNIT_WAN 0
#define CFG_BOARD_PB45 1
#endif


#define TRUE    1
#define FALSE   0

#define ATHR_PHY0_ADDR   0x0
#define ATHR_PHY1_ADDR   0x1
#define ATHR_PHY2_ADDR   0x2
#define ATHR_PHY3_ADDR   0x3
#define ATHR_PHY4_ADDR   0x4
#define ATHR_IND_PHY 4

#define MODULE_NAME "ATHRS16"



extern char *dup_str[];
static athr_gmac_t *athr_macs[2];


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
 * Per-PHY information, indexed by PHY unit number
 * MAC port 0 - CPU port 0x100
 */
static athrPhyInfo_t athrPhyInfo[] = {
    {TRUE,   /* phy port 0 -- MAC port 1 0x200 */
     FALSE,
     ENET_UNIT_LAN,
     0,
     ATHR_PHY0_ADDR,
     ATHR_LAN_PORT_VLAN
    },

    {TRUE,   /* phy port 1 -- MAC port 2 0x300 */
     FALSE,
     ENET_UNIT_LAN,
     0,
     ATHR_PHY1_ADDR,
     ATHR_LAN_PORT_VLAN
    },

    {TRUE,   /* phy port 2 -- MAC port 3 0x400 */
     FALSE,
     ENET_UNIT_LAN,
     0,
     ATHR_PHY2_ADDR, 
     ATHR_LAN_PORT_VLAN
    },

    {TRUE,   /* phy port 3 -- MAC port 4 0x500 */
     FALSE,
     ENET_UNIT_LAN,
     0,
     ATHR_PHY3_ADDR, 
     ATHR_LAN_PORT_VLAN
    },

    {TRUE,   /* phy port 4 -- WAN port or MAC port 5 0x600 */
     FALSE,
     ENET_UNIT_LAN,//ENET_UNIT_WAN,
     0,
     ATHR_PHY4_ADDR, 
     ATHR_LAN_PORT_VLAN   /* Send to all ports */
    },
    
    {FALSE,  /* phy port 5 -- CPU port (no RJ45 connector) */
     TRUE,
     ENET_UNIT_LAN,
     0,
     0x00, 
     ATHR_LAN_PORT_VLAN    /* Send to all ports */
    },
};

static uint8_t athr16_init_flag = 0;

//#define ATHR_PHY_MAX (sizeof(ipPhyInfo) / sizeof(ipPhyInfo[0]))
#define ATHR_PHY_MAX 5

/* Range of valid PHY IDs is [MIN..MAX] */
#define ATHR_ID_MIN 0
#define ATHR_ID_MAX (ATHR_PHY_MAX-1)

/* Convenience macros to access myPhyInfo */
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

static void phy_mode_setup(void);

static void phy_mode_setup(void) 
{
#ifdef ATHRS16_VER_1_0
    printk("phy_mode_setup\n");

    /*work around for phy4 rgmii mode*/
    phy_reg_write(ATHR_PHYBASE(ATHR_IND_PHY), ATHR_PHYADDR(ATHR_IND_PHY), 29, 18);     
    phy_reg_write(ATHR_PHYBASE(ATHR_IND_PHY), ATHR_PHYADDR(ATHR_IND_PHY), 30, 0x480c);    

    /*rx delay*/ 
    phy_reg_write(ATHR_PHYBASE(ATHR_IND_PHY), ATHR_PHYADDR(ATHR_IND_PHY), 29, 0);     
    phy_reg_write(ATHR_PHYBASE(ATHR_IND_PHY), ATHR_PHYADDR(ATHR_IND_PHY), 30, 0x824e);  

    /*tx delay*/ 
    phy_reg_write(ATHR_PHYBASE(ATHR_IND_PHY), ATHR_PHYADDR(ATHR_IND_PHY), 29, 5);     
    phy_reg_write(ATHR_PHYBASE(ATHR_IND_PHY), ATHR_PHYADDR(ATHR_IND_PHY), 30, 0x3d47);    
#endif
}
void athrs16_force_100M(int phy ,int duplex)
{
    phy_reg_write(ATHR_PHYBASE(phy),ATHR_PHYADDR(phy),
                     ATHR_PHY_CONTROL,(0xa000|(duplex << 8)));
}
void athrs16_force_10M(int phy,int duplex)
{
    phy_reg_write(ATHR_PHYBASE(phy),ATHR_PHYADDR(phy),
                     ATHR_PHY_CONTROL,(0x8000 |(duplex << 8)));
}

static BOOL
athrs16_bist_test(void)
{
    uint32_t times = 2, regc = 0xa0, regr = 0xa4, data = 0, i;

    while (--times) {
        athrs16_reg_write(regc, 0x80700000);

        i = 0x1000;
        while(i--) {
            if(!(athrs16_reg_read(regc)&0x80000000))
                break;
            udelay(5);
        }

        if (0 == i)
            return FALSE;

        data = athrs16_reg_read(regc);

        if(!(data & 0xe000))
            break;
        
        if (!times || !(data & 0x40000000))
            return FALSE;
        
        athrs16_reg_write(regr, (data & 0x1fff) | 0x80000000);

    }

    /* test pass then reset switch */
    printk(MODULE_NAME ": resetting...\n");
    athrs16_reg_write(0x0, athrs16_reg_read(0x0)|0x80000000);

    i = 20;
    while(i--) {
        mdelay(100);
        if(!(athrs16_reg_read(0x0)&0x80000000))
            break;
    }

    if (i)
        printk(MODULE_NAME ": reset done.\n");
    else
        printk(MODULE_NAME ": reset fail.\n"); 

    return TRUE;
}

int athrs16_reg_init(void *arg)
{
    /* if using header for register configuration, we have to     */
    /* configure s16 register after frame transmission is enabled */
/*    if (athr16_init_flag)
        return;*/
    printk("Modified reg init\n");
    athrs16_bist_test();
    /*Power on strip mode setup*/
#if CFG_BOARD_PB45
    athrs16_reg_write(0x208, 0x2fd0001);  /*tx delay*/   
    athrs16_reg_write(0x108, 0x2be0001);  /*mac0 rgmii mode*/ 
#elif CFG_BOARD_AP96
    //athrs16_reg_write(0x8, 0x012e1bea);
    athrs16_reg_write(0x8, 0x01261be2);
#endif
    
    athrs16_reg_write(PORT_STATUS_REGISTER0, 0x7e);

   /* XXX FIX ME: S27 results in continious pause frames after few hours
    * running high traffic if flow control is enabled. This
    * might need a  RTL fix, disabling by default as a WAR.
    */

    athrs16_reg_write(PORT_STATUS_REGISTER1, 0x200);
    athrs16_reg_write(PORT_STATUS_REGISTER2, 0x200);
    athrs16_reg_write(PORT_STATUS_REGISTER3, 0x200);
    athrs16_reg_write(PORT_STATUS_REGISTER4, 0x200);
#if CFG_BOARD_PB45
    athrs16_reg_write(0x600, 0x200);
    printk("CFG Board PB45\n");
#elif CFG_BOARD_AP96
    //athrs16_reg_write(0x600, 0x0);
    printk("CFG board AP96\n");
    athrs16_reg_write(0x600, 0x200);
#endif

    athrs16_reg_write(FLD_MASK_REG, 0x003f003f);

#if defined(CONFIG_VLAN_8021Q) || defined(CONFIG_VLAN_8021Q_MODULE)        
#ifdef HEADER_EN        
    athrs16_reg_write(PORT_CONTROL_REGISTER0, 0x6804);
#else
    athrs16_reg_write(PORT_CONTROL_REGISTER0, 0x6004);
#endif

    athrs16_reg_write(PORT_CONTROL_REGISTER1, 0x6004);
    athrs16_reg_write(PORT_CONTROL_REGISTER2, 0x6004);
    athrs16_reg_write(PORT_CONTROL_REGISTER3, 0x6004);
    athrs16_reg_write(PORT_CONTROL_REGISTER4, 0x6004);    
    athrs16_reg_write(PORT_CONTROL_REGISTER5, 0x6004);    
#else
#ifdef HEADER_EN        
    athrs16_reg_write(PORT_CONTROL_REGISTER0, 0x4804);
#else
    athrs16_reg_write(PORT_CONTROL_REGISTER0, 0x4004);
#endif
#endif

#ifdef FULL_FEATURE
	hsl_dev_init(0, 2);
#endif

   /* Enable ARP packets to CPU port */
    athrs16_reg_write(ARL_TBL_CTRL_REG,(athrs16_reg_read(ARL_TBL_CTRL_REG) | 0x100000));
   /* Enable Broadcast packets to CPU port */
    athrs16_reg_write(FLD_MASK_REG,(athrs16_reg_read(FLD_MASK_REG) | S16_ENABLE_CPU_BROADCAST ));

    DPRINTF("S16 CPU_PORT_REGISTER :%x\n", athrs16_reg_read ( CPU_PORT_REGISTER ));
    DPRINTF("S16 PORT_STATUS_REGISTER0  :%x\n", athrs16_reg_read ( PORT_STATUS_REGISTER0 ));
    DPRINTF("S16 PORT_STATUS_REGISTER1  :%x\n", athrs16_reg_read ( PORT_STATUS_REGISTER1 ));
    DPRINTF("S16 PORT_STATUS_REGISTER2  :%x\n", athrs16_reg_read ( PORT_STATUS_REGISTER2 ));
    DPRINTF("S16 PORT_STATUS_REGISTER3  :%x\n", athrs16_reg_read ( PORT_STATUS_REGISTER3 ));
    DPRINTF("S16 PORT_STATUS_REGISTER4  :%x\n", athrs16_reg_read ( PORT_STATUS_REGISTER4 ));

    DPRINTF("S16 PORT_CONTROL_REGISTER0 :%x\n", athrs16_reg_read ( PORT_CONTROL_REGISTER0 ));
    DPRINTF("S16 PORT_CONTROL_REGISTER1 :%x\n", athrs16_reg_read ( PORT_CONTROL_REGISTER1 ));
    DPRINTF("S16 PORT_CONTROL_REGISTER2 :%x\n", athrs16_reg_read ( PORT_CONTROL_REGISTER2 ));
    DPRINTF("S16 PORT_CONTROL_REGISTER3 :%x\n", athrs16_reg_read ( PORT_CONTROL_REGISTER3 ));
    DPRINTF("S16 PORT_CONTROL_REGISTER4 :%x\n", athrs16_reg_read ( PORT_CONTROL_REGISTER4 ));

    athr16_init_flag = 1;
    printk("athrs16_reg_init complete*\n");


    if (is_ar934x()) {
       athr_reg_rmw_set(ATHR_GPIO_OE, 0x10);
       athr_reg_rmw_set(ATHR_GPIO_INT_ENABLE, 0x10);
       athr_reg_rmw_set(ATHR_GPIO_INT_TYPE, 0x10); /* level sensitive */
    }
    return 0;
}

/******************************************************************************
*
* athrs16_phy_is_link_alive - test to see if the specified link is alive
*
* RETURNS:
*    1  --> link is alive
*    0 --> link is down
*/
int
athrs16_phy_is_link_alive(int phyUnit)
{
    uint16_t phyHwStatus;
    uint32_t phyBase;
    uint32_t phyAddr;

    phyBase = ATHR_PHYBASE(phyUnit);
    phyAddr = ATHR_PHYADDR(phyUnit);
 
  

    phyHwStatus = phy_reg_read(phyBase, phyAddr, ATHR_PHY_SPEC_STATUS);

    if (phyHwStatus & ATHR_STATUS_LINK_PASS)
        return 1;

    return 0;
}

/******************************************************************************
*
* athrs16_phy_setup - reset and setup the PHY associated with
* the specified MAC unit number.
*
* Resets the associated PHY port.
*
* RETURNS:
*    TRUE  --> associated PHY is alive
*    FALSE --> no LINKs on this ethernet unit
*/

int
athrs16_phy_setup(void *arg)
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
    for (phyUnit=0; phyUnit < ATHR_PHY_MAX; phyUnit++) {
        if (!ATHR_IS_ETHUNIT(phyUnit, ethUnit)) {
            continue;
        }

        foundPhy = TRUE;
        phyBase = ATHR_PHYBASE(phyUnit);
        phyAddr = ATHR_PHYADDR(phyUnit);
        
        phy_reg_write(phyBase, phyAddr, ATHR_AUTONEG_ADVERT,
                      ATHR_ADVERTISE_ALL);

        phy_reg_write(phyBase, phyAddr, ATHR_1000BASET_CONTROL,
                      ATHR_ADVERTISE_1000FULL);

        /* Reset PHYs*/
        phy_reg_write(phyBase, phyAddr, ATHR_PHY_CONTROL,
                      ATHR_CTRL_AUTONEGOTIATION_ENABLE 
                      | ATHR_CTRL_SOFTWARE_RESET);

    }

    if (!foundPhy) {
        return FALSE; /* No PHY's configured for this ethUnit */
    }

    /*
     * After the phy is reset, it takes a little while before
     * it can respond properly.
     */
    mdelay(1000);


    /*
     * Wait up to 3 seconds for ALL associated PHYs to finish
     * autonegotiation.  The only way we get out of here sooner is
     * if ALL PHYs are connected AND finish autonegotiation.
     */
    for (phyUnit=0; (phyUnit < ATHR_PHY_MAX) /*&& (timeout > 0) */; phyUnit++) {
        if (!ATHR_IS_ETHUNIT(phyUnit, ethUnit)) {
            continue;
        }

        timeout=20;
        for (;;) {
            phyHwStatus = phy_reg_read(phyBase, phyAddr, ATHR_PHY_CONTROL);

            if (ATHR_RESET_DONE(phyHwStatus)) {
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
    for (phyUnit=0; phyUnit < ATHR_PHY_MAX; phyUnit++) {
        if (!ATHR_IS_ETHUNIT(phyUnit, ethUnit)) {
            continue;
        }
#if 0
	/* Enable RGMII */
	phy_reg_write(0,phyUnit,0x1d,0x12);
	phy_reg_write(0,phyUnit,0x1e,0x8);
	/* Tx delay on PHY */
	phy_reg_write(0,phyUnit,0x1d,0x5);
	phy_reg_write(0,phyUnit,0x1e,0x100);
        
	/* Rx delay on PHY */
	phy_reg_write(0,phyUnit,0x1d,0x0);
	phy_reg_write(0,phyUnit,0x1e,0x8000);
#endif
         


        if (athrs16_phy_is_link_alive(phyUnit)) {
            liveLinks++;
            ATHR_IS_PHY_ALIVE(phyUnit) = TRUE;
        } else {
            ATHR_IS_PHY_ALIVE(phyUnit) = FALSE;
        }

        DRV_PRINT(DRV_DEBUG_PHYSETUP,
            ("eth%d: Phy Specific Status=%4.4x\n",
            ethUnit, 
            phy_reg_read(ATHR_PHYBASE(phyUnit),
                         ATHR_PHYADDR(phyUnit),
                         ATHR_PHY_SPEC_STATUS)));
    }
    phy_mode_setup();   
    printk("Phy setup Complete\n"); 
  
    return (liveLinks > 0);
}

/******************************************************************************
*
* athrs16_phy_is_fdx - Determines whether the phy ports associated with the
* specified device are FULL or HALF duplex.
*
* RETURNS:
*    1 --> FULL
*    0 --> HALF
*/
int
athrs16_phy_is_fdx(int ethUnit, int phyUnit)
{
    uint32_t  phyBase;
    uint32_t  phyAddr;
    uint16_t  phyHwStatus;
    int       ii = 200;

    if (ethUnit == ENET_UNIT_LAN)
        return TRUE;

    for (phyUnit=0; phyUnit < ATHR_PHY_MAX; phyUnit++) {
        if (!ATHR_IS_ETHUNIT(phyUnit, ethUnit)) {
            continue;
        }

        if (athrs16_phy_is_link_alive(phyUnit)) {

            phyBase = ATHR_PHYBASE(phyUnit);
            phyAddr = ATHR_PHYADDR(phyUnit);

            do {
                phyHwStatus = phy_reg_read (phyBase, phyAddr, 
                                               ATHR_PHY_SPEC_STATUS);
		if(phyHwStatus & ATHR_STATUS_RESOVLED)
			break;
                mdelay(10);
            } while(--ii);
            
            if (phyHwStatus & ATHER_STATUS_FULL_DEPLEX)
                return TRUE;
        }
    }

    return FALSE;
}

/******************************************************************************
*
* athrs16_phy_speed - Determines the speed of phy ports associated with the
* specified device.
*
* RETURNS:
*               ATHR_PHY_SPEED_10T, ATHR_PHY_SPEED_100TX;
*               ATHR_PHY_SPEED_1000T;
*/

int
athrs16_phy_speed(int ethUnit, int phyUnit)
{
    uint16_t  phyHwStatus;
    uint32_t  phyBase;
    uint32_t  phyAddr;
    int       ii = 200;
    athr_phy_speed_t phySpeed = 0;

    for (phyUnit=0; phyUnit < ATHR_PHY_MAX; phyUnit++) {
        if (!ATHR_IS_ETHUNIT(phyUnit, ethUnit)) {
            continue;
        }

        phyBase = ATHR_PHYBASE(phyUnit);
        phyAddr = ATHR_PHYADDR(phyUnit);
        phySpeed = ATHR_PHY_SPEED_10T;

        if (athrs16_phy_is_link_alive(phyUnit)) {

            do {
                phyHwStatus = phy_reg_read(phyBase, phyAddr, 
                                              ATHR_PHY_SPEC_STATUS);
		        if(phyHwStatus & ATHR_STATUS_RESOVLED)
			        break;
                mdelay(10);
            }while(--ii);
            
            phyHwStatus = ((phyHwStatus & ATHER_STATUS_LINK_MASK) >>
                           ATHER_STATUS_LINK_SHIFT);

            switch(phyHwStatus) {
            case 0:
                printk(MODULE_NAME": speed read:10Mbps phyhwstat:%x\n",phyHwStatus);
                phySpeed = ATHR_PHY_SPEED_10T;
                break;
            case 1:
                phySpeed = ATHR_PHY_SPEED_100T;
                break;
            case 2:
                phySpeed = ATHR_PHY_SPEED_1000T;
                break;
            default:
                printk(MODULE_NAME":Unkown speed read:%d! phyhwstat:%x\n",phyUnit,phyHwStatus);
            }
        } 

        
        
    }

    if (ethUnit == ENET_UNIT_LAN)
         phySpeed = ATHR_PHY_SPEED_1000T;

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
athrs16_phy_is_up(int ethUnit)
{
    int           phyUnit;
    uint16_t      phyHwStatus, phyHwControl;
    athrPhyInfo_t *lastStatus;
    int           linkCount   = 0;
    int           lostLinks   = 0;
    int           gainedLinks = 0;
    uint32_t      phyBase;
    uint32_t      phyAddr;

    for (phyUnit=0; phyUnit < ATHR_PHY_MAX; phyUnit++) {
        if (!ATHR_IS_ETHUNIT(phyUnit, ethUnit)) {
            continue;
        }

        phyBase = ATHR_PHYBASE(phyUnit);
        phyAddr = ATHR_PHYADDR(phyUnit);

        lastStatus = &athrPhyInfo[phyUnit];

        if (lastStatus->isPhyAlive) { /* last known link status was ALIVE */
            phyHwStatus = phy_reg_read(phyBase, phyAddr, ATHR_PHY_SPEC_STATUS);

            /* See if we've lost link */
            if (phyHwStatus & ATHR_STATUS_LINK_PASS) {
                linkCount++;
            } else {
                lostLinks++;
                DRV_PRINT(DRV_DEBUG_PHYCHANGE,("\nenet%d port%d down\n",
                                               ethUnit, phyUnit));
                printk("\nenet%d port%d down\n",
                                              ethUnit, phyUnit);
                lastStatus->isPhyAlive = FALSE;
            }
        } else { /* last known link status was DEAD */
            /* Check for reset complete */
            phyHwStatus = phy_reg_read(phyBase, phyAddr, ATHR_PHY_STATUS);
            if (!ATHR_RESET_DONE(phyHwStatus))
                continue;

            phyHwControl = phy_reg_read(phyBase, phyAddr, ATHR_PHY_CONTROL);
            /* Check for AutoNegotiation complete */            
            if ((!(phyHwControl & ATHR_CTRL_AUTONEGOTIATION_ENABLE)) 
                 || ATHR_AUTONEG_DONE(phyHwStatus)) {
                phyHwStatus = phy_reg_read(phyBase, phyAddr, 
                                           ATHR_PHY_SPEC_STATUS);

                if (phyHwStatus & ATHR_STATUS_LINK_PASS) {
                gainedLinks++;
                linkCount++;
                DRV_PRINT(DRV_DEBUG_PHYCHANGE,("\nenet%d port%d up\n",
                                               ethUnit, phyUnit));
                printk("\nenet%d port%d up\n",ethUnit, phyUnit);
                lastStatus->isPhyAlive = TRUE;
                }
            }
        }
    }

    return (linkCount);

}

int athrs16_check_status(void)
{
     uint32_t cpu_port_st = athrs16_reg_read(0x100);
     uint32_t cpu_port_ctrl = athrs16_reg_read(0x104);

     if (((cpu_port_st & 0xffff) == 0x1080)
          && ((cpu_port_ctrl & 0xffff) == 0x4004)) {
	return 0; /* NOT OK */
     }
     return 1; /* OK */
}

unsigned int
athrs16_reg_read(unsigned int reg_addr)
{
    uint32_t reg_word_addr;
    uint32_t phy_addr, tmp_val, reg_val;
    uint16_t phy_val;
    uint8_t phy_reg;

    /* change reg_addr to 16-bit word address, 32-bit aligned */
    reg_word_addr = (reg_addr & 0xfffffffc) >> 1;

    /* configure register high address */
    phy_addr = 0x18;
    phy_reg = 0x0;
    phy_val = (uint16_t) ((reg_word_addr >> 8) & 0x1ff);  /* bit16-8 of reg address */
    phy_reg_write(0, phy_addr, phy_reg, phy_val);

    /* For some registers such as MIBs, since it is read/clear, we should */
    /* read the lower 16-bit register then the higher one */

    /* read register in lower address */
    phy_addr = 0x10 | ((reg_word_addr >> 5) & 0x7); /* bit7-5 of reg address */
    phy_reg = (uint8_t) (reg_word_addr & 0x1f);   /* bit4-0 of reg address */
    reg_val = (uint32_t) phy_reg_read(0, phy_addr, phy_reg);

    /* read register in higher address */
    reg_word_addr++;
    phy_addr = 0x10 | ((reg_word_addr >> 5) & 0x7); /* bit7-5 of reg address */
    phy_reg = (uint8_t) (reg_word_addr & 0x1f);   /* bit4-0 of reg address */
    tmp_val = (uint32_t) phy_reg_read(0, phy_addr, phy_reg);
    reg_val |= (tmp_val << 16);

    return reg_val;   
}

void
athrs16_reg_write(unsigned int reg_addr, unsigned int reg_val)
{
    uint32_t reg_word_addr;
    uint32_t phy_addr;
    uint16_t phy_val;
    uint8_t phy_reg;

    /* change reg_addr to 16-bit word address, 32-bit aligned */
    reg_word_addr = (reg_addr & 0xfffffffc) >> 1;

    /* configure register high address */
    phy_addr = 0x18;
    phy_reg = 0x0;
    phy_val = (uint16_t) ((reg_word_addr >> 8) & 0x1ff);  /* bit16-8 of reg address */
    phy_reg_write(0, phy_addr, phy_reg, phy_val);

    /* For some registers such as ARL and VLAN, since they include BUSY bit */
    /* in lower address, we should write the higher 16-bit register then the */
    /* lower one */

    /* read register in higher address */
    reg_word_addr++;
    phy_addr = 0x10 | ((reg_word_addr >> 5) & 0x7); /* bit7-5 of reg address */
    phy_reg = (uint8_t) (reg_word_addr & 0x1f);   /* bit4-0 of reg address */
    phy_val = (uint16_t) ((reg_val >> 16) & 0xffff);
    phy_reg_write(0, phy_addr, phy_reg, phy_val);

    /* write register in lower address */
    reg_word_addr--;
    phy_addr = 0x10 | ((reg_word_addr >> 5) & 0x7); /* bit7-5 of reg address */
    phy_reg = (uint8_t) (reg_word_addr & 0x1f);   /* bit4-0 of reg address */
    phy_val = (uint16_t) (reg_val & 0xffff);
    phy_reg_write(0, phy_addr, phy_reg, phy_val); 
}

void athrs16_enable_link_intrs(void *arg)
{
     int phyUnit = 0, phyAddr = 0;

    /* Enable global PHY link status interrupt */
    athrs16_reg_write(S16_GLOBAL_INTR_MASK_REG,PHY_LINK_CHANGE_REG);

    for (phyUnit = 0; phyUnit < ATHR_PHY_MAX; phyUnit++) {
       phyAddr = ATHR_PHYADDR(phyUnit);
       phy_reg_write(0,phyAddr,ATHR_PHY_INTR_ENABLE,PHY_LINK_INTRS);
    }

}

void athrs16_disable_link_intrs(void *arg) 
{
    int phyUnit = 0, phyAddr = 0;

    for (phyUnit = 0; phyUnit < ATHR_PHY_MAX; phyUnit++) {
        phyAddr = ATHR_PHYADDR(phyUnit);
        phy_reg_write(0,phyAddr,ATHR_PHY_INTR_ENABLE,0x0);
    }
}


void ar934x_s16_intr(void *arg)
{
    int status = 0, intr_reg_val;
    uint32_t phyUnit = 0 ,phyBase = 0 ,phyAddr = 0;
    uint32_t phymask = 0x0;
    uint32_t linkDown = 0x0;
    athr_gmac_t *mac0 = athr_macs[0];


    athrs16_reg_write(S16_GLOBAL_INTR_MASK_REG,0x0);

    intr_reg_val = athrs16_reg_read(S16_GLOBAL_INTR_REG);


    /* clear global link interrupt */
    athrs16_reg_write(S16_GLOBAL_INTR_REG,intr_reg_val);

    if (intr_reg_val & PHY_LINK_CHANGE_REG) {

       for (phyUnit=0; phyUnit < ATHR_PHY_MAX; phyUnit++) {

           phyBase = ATHR_PHYBASE(phyUnit);
           phyAddr = ATHR_PHYADDR(phyUnit);
           status = phy_reg_read(0,phyAddr,ATHR_PHY_INTR_STATUS);

           if(status & PHY_LINK_UP) {
               DPRINTF("LINK UP - Port %d:%x\n",phyAddr,status);
               phymask = (phymask | (1 << phyUnit));
           }
           if(status & PHY_LINK_DOWN) {
               DPRINTF("LINK DOWN - Port %d:%x\n",phyAddr,status);
               phymask = (phymask | (1 << phyUnit));
               linkDown = (linkDown | (1 << phyUnit));
           }
           if(status & PHY_LINK_DUPLEX_CHANGE) {
               DPRINTF("LINK DUPLEX CHANGE - Port %d:%x\n",phyAddr,status);
               phymask = (phymask | (1 << phyUnit));
           }
           if(status & PHY_LINK_SPEED_CHANGE) {
               DPRINTF("LINK SPEED CHANGE %d:%x\n",phyAddr,status);
               phymask = (phymask | (1 << phyUnit));
           }
       }
       for (phyUnit=0; phyUnit < ATHR_PHY_MAX; phyUnit++) {
           if ((phymask >> phyUnit) & 0x1) {

               phyBase = ATHR_PHYBASE(phyUnit);
               phyAddr = ATHR_PHYADDR(phyUnit);

               status = phy_reg_read(0,phyAddr,ATHR_PHY_INTR_STATUS);

               if (!athrs16_phy_is_link_alive(phyUnit) && !((linkDown >> phyUnit) & 0x1))
                   continue;

               mac0->ops->check_link(mac0,phyUnit);
           }
       }
       
       athrs16_reg_write(S16_GLOBAL_INTR_MASK_REG,PHY_LINK_CHANGE_REG);
   }
   else  {
      DPRINTF("Spurious link interrupt:%s,status:%x\n",__func__,status);
      athrs16_reg_write(S16_GLOBAL_INTR_MASK_REG,PHY_LINK_CHANGE_REG);
   }

}

unsigned int s16_rd_phy(int ethUnit,unsigned int phy_addr, unsigned int reg_addr)
{
      return (phy_reg_read(ethUnit,phy_addr,reg_addr));          

}

void s16_wr_phy(int ethUnit,unsigned int phy_addr, unsigned int reg_addr, unsigned int write_data)
{
      phy_reg_write(ethUnit,phy_addr,reg_addr,write_data);          

}

int athrs16_register_ops(void *arg)
{
  athr_gmac_t *mac   = (athr_gmac_t *) arg;
  athr_phy_ops_t *ops = mac->phy;

  if(!ops) 
     ops = kmalloc(sizeof(athr_phy_ops_t), GFP_KERNEL);

  memset(ops,0,sizeof(athr_phy_ops_t));

  ops->mac            =  mac;
  ops->is_up          =  athrs16_phy_is_up;
  ops->is_alive       =  athrs16_phy_is_link_alive;
  ops->speed          =  athrs16_phy_speed;
  ops->is_fdx         =  athrs16_phy_is_fdx;
  ops->ioctl          =  NULL;
  ops->setup          =  athrs16_phy_setup;
  ops->stab_wr        =  NULL;
  ops->link_isr       =  ar934x_s16_intr;
  ops->en_link_intrs  =  athrs16_enable_link_intrs;
  ops->dis_link_intrs =  athrs16_disable_link_intrs;
  ops->read_phy_reg   =  s16_rd_phy;
  ops->write_phy_reg  =  s16_wr_phy;
  ops->read_mac_reg   =  athrs16_reg_read;
  ops->write_mac_reg  =  athrs16_reg_write;
  ops->init           =  athrs16_reg_init;
  mac->phy            =  ops;

  if (mac->mac_unit == 0) {
     athr_macs[0]     =  mac;
     ops->arg_ad[0]   = (void *)mac;
     ops->port_map    = 0x7f;
  }
  else {
     athr_macs[1]    = mac;
     ops->arg_ad[1]  = (void *)mac;
  }

  return 0;
}

int athrs16_in_reset(void *arg)
{
     athr_gmac_t *mac = arg;
     athr_phy_ops_t *phy = mac->phy;
     struct net_device *dev = mac->mac_dev;
     int phyUnit = 0, in_reset = 0;
     int reg = 0;

     for (phyUnit=0; phyUnit < 6 ; phyUnit++) {

         reg = 0x100 + (phyUnit * 0x100);
         in_reset = phy->read_mac_reg(reg);


         if ((in_reset & (1 << 12))) {

            printk(KERN_DEBUG MODULE_NAME ":reg:%x phyUnit:%d"
                " in_rest:%X\n",reg,phyUnit,in_reset);
            printk(MODULE_NAME ": S16 in reset...\n");

            athr_stop_dev(dev);
            athr_open_dev(dev);

            return 1 ;

         }
     }
     return 0;
}
