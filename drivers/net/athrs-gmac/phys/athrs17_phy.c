/*
 * This file is subject to the terms and conditions of the GNU General Public
 * License.  See the file "COPYING" in the main directory of this archive
 * for more details.
 *
 * Copyright © 2010 Atheros Communications, Inc.,  All Rights Reserved.
 */

/*
 * Manage the atheros ethernet PHY.
 *
 * All definitions in this file are operating system independent!
 */
#include "athrs_ioctl.h"
#include "athrs_phy.h"
#include "athrs_mac.h"
#include "athrs17_phy.h"
#ifdef S17_REGISTER_DUMP
#include <linux/proc_fs.h>
#endif

/* for PHY related debugging messages */
#undef S17_PHY_DEBUG
#ifdef S17_PHY_DEBUG
#define DPRINTF_PHY(_fmt,...) do {         \
                printk(MODULE_NAME":"_fmt, __VA_ARGS__);      \
} while (0)
#else
#define DPRINTF_PHY(_fmt,...) 
#endif

/* support SSDK full feature set, 
   for ssdk_sh, and HNAT on AR8327N/AR8328N */

#ifdef S17_FULL_FEATURE
#include "build_mode.h"
#include "ssdk_init.h"
sw_error_t
athrs17_phy_write(a_uint32_t dev_id, a_uint32_t phy_addr, a_uint32_t reg, a_uint16_t data)
{
    phy_reg_write(dev_id, phy_addr, reg, data);
    return 0;
}

sw_error_t
athrs17_phy_read(a_uint32_t dev_id, a_uint32_t phy_addr, a_uint32_t reg, a_uint16_t *data)
{
    *data = phy_reg_read(dev_id, phy_addr, reg);
    return 0;
}
#endif /* S17_FULL_FEATURE */

/* AR8327N/AR8328N HNAT feature */
#ifdef CONFIG_ATHRS17_HNAT

#define AOS_HEADER_TYPE_ARP 0x8

typedef struct {
	uint16_t ver:2;
	uint16_t pri:3;
	uint16_t type:5;
	uint16_t rev:2;
	uint16_t with_tag:1;
	uint16_t sport:3;
	uint16_t vid;
} aos_header_t;

void
aos_header_arp_in (struct sk_buff *skb)
{
	uint8_t offset = 12;
	uint16_t athr_tag = S17_HDR_VALUE;
	uint8_t header_len = 0;

	aos_header_t *athr_header;
	aos_header_t *skb_headp = (aos_header_t *) skb->head;
	if(*(uint16_t *)&skb->data[offset] == athr_tag){
	    athr_header = (aos_header_t *)&skb->data[offset+2];
	    header_len = 4;
	}
	else{
		athr_header = (aos_header_t *)&skb->data[offset];
		header_len = 2;
	}

	memcpy(skb_headp, athr_header, sizeof(aos_header_t));
	if(athr_header->with_tag){
		skb_headp->vid = 0xfff & *(uint16_t *)&skb->data[offset+header_len];
	} else {
		skb_headp->vid = 0;
	}
    
    memmove(&skb->data[header_len], &skb->data[0], offset);
    skb_pull(skb, header_len); /* remove athr header */
}

#endif /* CONFIG_ATHRS17_HNAT */

unsigned int s17_rd_phy(int ethUnit,unsigned int phy_addr, unsigned int reg_addr);
void s17_wr_phy(int ethUnit,unsigned int phy_addr, unsigned int reg_addr, unsigned int write_data);
static void athrs17_reg_rmw(unsigned int reg_addr, unsigned int reg_val);

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

#define S17_LAN_PORT_VLAN          1
#define S17_WAN_PORT_VLAN          2

#define ENET_UNIT_GE1 1  
#define ENET_UNIT_GE0 0 /* Connected to the switch */

#define TRUE    1
#define FALSE   0

#define S17_PHY0_ADDR   0x0
#define S17_PHY1_ADDR   0x1
#define S17_PHY2_ADDR   0x2
#define S17_PHY3_ADDR   0x3
#define S17_PHY4_ADDR   0x4
#define S17_IND_PHY 4

#define MODULE_NAME "ATHRS17"

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
#ifdef S17_8023AZ_FEATURE
    int azFeature;
#endif    
} athrPhyInfo_t;

/*
 * Per-PHY information, indexed by PHY unit number
 * MAC port 0 - CPU port 0x100
 * All ports are connected to GE0 of the SoC MAC
 * LAN/WAN seperation by VLAN tags (port 0-3 with VLANID 1; port 4 with VLANID 2
 */
static athrPhyInfo_t athrPhyInfo[] = {
    {TRUE,   /* phy port 0 -- MAC port 1 0x200 */
     FALSE,
     ENET_UNIT_GE0,
     0,
     S17_PHY0_ADDR,
     S17_LAN_PORT_VLAN
    },

    {TRUE,   /* phy port 1 -- MAC port 2 0x300 */
     FALSE,
     ENET_UNIT_GE0,
     0,
     S17_PHY1_ADDR,
     S17_LAN_PORT_VLAN
    },

    {TRUE,   /* phy port 2 -- MAC port 3 0x400 */
     FALSE,
     ENET_UNIT_GE0,
     0,
     S17_PHY2_ADDR, 
     S17_LAN_PORT_VLAN
    },

    {TRUE,   /* phy port 3 -- MAC port 4 0x500 */
     FALSE,
     ENET_UNIT_GE0,
     0,
     S17_PHY3_ADDR, 
     S17_LAN_PORT_VLAN
    },

    {TRUE,   /* phy port 4 -- WAN port or MAC port 5 0x600 */
     FALSE,
     ENET_UNIT_GE0,
     0,
     S17_PHY4_ADDR, 
     S17_WAN_PORT_VLAN   /* set as WAN port */
    },
    
    {FALSE,  /* phy port 5 -- CPU port (no RJ45 connector) */
     TRUE,
     ENET_UNIT_GE0,
     0,
     0x00, 
     S17_LAN_PORT_VLAN    /* Send to all ports */
    },
};

static uint8_t athr17_init_flag = 0;

#ifdef S17_REGISTER_DUMP
static int athrs17_seq_open(struct inode *inode, struct file *file); 

static const struct file_operations athrs17_seq_fops = {
  .owner          = THIS_MODULE,
  .open           = athrs17_seq_open,
  .read           = seq_read,
  .llseek         = seq_lseek,
  .release        = single_release_net,
};
#endif

#define S17_PHY_MAX 5

/* Range of valid PHY IDs is [MIN..MAX] */
#define S17_ID_MIN 0
#define S17_ID_MAX (S17_PHY_MAX-1)

/* Convenience macros to access myPhyInfo */
#define S17_IS_ENET_PORT(phyUnit) (athrPhyInfo[phyUnit].isEnetPort)
#define S17_IS_PHY_ALIVE(phyUnit) (athrPhyInfo[phyUnit].isPhyAlive)
#define S17_ETHUNIT(phyUnit) (athrPhyInfo[phyUnit].ethUnit)
#define S17_PHYBASE(phyUnit) (athrPhyInfo[phyUnit].phyBase)
#define S17_PHYADDR(phyUnit) (athrPhyInfo[phyUnit].phyAddr)
#define S17_VLAN_TABLE_SETTING(phyUnit) (athrPhyInfo[phyUnit].VLANTableSetting)


#define S17_IS_ETHUNIT(phyUnit, ethUnit) \
            (S17_IS_ENET_PORT(phyUnit) &&        \
            S17_ETHUNIT(phyUnit) == (ethUnit))

#define S17_IS_WAN_PORT(phyUnit) (!(S17_ETHUNIT(phyUnit)==ENET_UNIT_GE1))
            

int athrs17_reg_init(void *arg)
{
    int phy_addr; 
#ifdef S17_FULL_FEATURE /* SSDK full features */
    ssdk_init_cfg cfg;
    garuda_init_spec_cfg chip_spec_cfg;
    int rv; 
    
    memset(&cfg, 0, sizeof(ssdk_init_cfg));
    memset(&chip_spec_cfg, 0, sizeof(garuda_init_spec_cfg));
    
    cfg.cpu_mode = HSL_CPU_1;
    cfg.reg_mode = HSL_MDIO;
    cfg.nl_prot = 30;
    cfg.chip_type = CHIP_ISIS;
    cfg.chip_spec_cfg = &chip_spec_cfg;
    cfg.reg_func.mdio_set = athrs17_phy_write;
    cfg.reg_func.mdio_get = athrs17_phy_read;
    
    rv = ssdk_init(0, &cfg);
    if (rv == 0)
        printk("\n######## S17 SSDK init succeeded! ########\n");
    else
        printk("\n######## S17 SSDK init failed! (code: %d) ########\n", rv);
#endif /* S17_FULL_FEATURE */

#ifdef CONFIG_ATHRS17_HNAT
    /* header len 4 with type 0xaaaa */
    athrs17_reg_write(S17_HDRCTRL_REG, S17_HDRLENGTH_SEL | S17_HDR_VALUE);

    /*PORT0 TX_HEADER_MODE_0 all frame with header*/
    athrs17_reg_write(S17_P0HDRCTRL_REG, S17_TXHDR_MODE_ALL);
    
    /*cpu port en*/
    athrs17_reg_write(S17_GLOFW_CTRL0_REG, (athrs17_reg_read(S17_GLOFW_CTRL0_REG)|S17_CPU_PORT_EN));
    
    /* enable L3 offload */
    athrs17_reg_write(S17_MODULE_EN_REG, (athrs17_reg_read(S17_MODULE_EN_REG) | 0x4)); 
#endif
    
    /* configure the RGMII */
    athrs17_reg_write(S17_GLOFW_CTRL1_REG, \
                  S17_BROAD_DPALL | S17_MULTI_FLOOD_DPALL | S17_UNI_FLOOD_DPALL);
    athrs17_reg_write(S17_PWS_REG, S17_PWS_CHIP_AR8327);
    athrs17_reg_write(S17_P0PAD_MODE_REG, S17_MAC0_RGMII_EN | S17_MAC0_RGMII_TXCLK_DELAY | \
                  S17_MAC0_RGMII_RXCLK_DELAY | (1 << S17_MAC0_RGMII_TXCLK_SHIFT) | \
                  (1 << S17_MAC0_RGMII_RXCLK_SHIFT));
    athrs17_reg_write(S17_P6PAD_MODE_REG, S17_MAC6_RGMII_RXCLK_DELAY | \
                  (0 << S17_MAC6_RGMII_RXCLK_SHIFT));
    athrs17_reg_write(S17_P0STATUS_REG, S17_PORT_STATUS_DEFAULT);

    /* AR8327/AR8328 v1.0 fixup */
    if ((athrs17_reg_read(S17_MASK_CTRL_REG) & 0xff) == S17_CHIPID_V1_0)
    {
        for (phy_addr = 0x0; phy_addr <= S17_PHY_MAX; phy_addr++)
        {
          /* For 100M waveform */
          phy_reg_write(0, phy_addr, S17_DEBUG_PORT_ADDRESS, 0x0);
          phy_reg_write(0, phy_addr, S17_DEBUG_PORT_DATA, 0x02ea);
          /* Turn On Gigabit Clock */
          phy_reg_write(0, phy_addr, S17_DEBUG_PORT_ADDRESS, 0x3d);
          phy_reg_write(0, phy_addr, S17_DEBUG_PORT_DATA, 0x68a0);
        }
    }
		
		/* enable HOL by default */
    for (phy_addr = 0; phy_addr < S17_MAC_MAX; phy_addr++)
    {
       switch (phy_addr) {
       	case 0:
       	case 5:
       	case 6:
       		athrs17_reg_write(S17_PORT0_HOL_CTRL0 + phy_addr * 8, \
            S17_HOL_CTRL0_WAN);
          break;
        
        default:
        	athrs17_reg_write(S17_PORT0_HOL_CTRL0 + phy_addr * 8, \
            S17_HOL_CTRL0_LAN);
          break;
       }
        
        athrs17_reg_write(S17_PORT0_HOL_CTRL1 + phy_addr * 8, \
            S17_HOL_CTRL1_DEFAULT);
    }
#ifdef S17_8023AZ_FEATURE
    athrs17_reg_write(S17_EEE_CTRL_REG, S17_LPI_DISABLE_ALL); 
#endif
    
    athr17_init_flag = 1;

    printk ("%s:done\n",__func__);
    
    return 0;
}

/******************************************************************************
*
* athrs17_phy_is_link_alive - test to see if the specified link is alive
*
* RETURNS:
*    TRUE  --> link is alive
*    FALSE --> link is down
*/
BOOL
athrs17_phy_is_link_alive(int phyUnit)
{
    uint16_t phyHwStatus;
    uint32_t phyBase;
    uint32_t phyAddr;

    phyBase = S17_PHYBASE(phyUnit);
    phyAddr = S17_PHYADDR(phyUnit);
 
    phyHwStatus = phy_reg_read(phyBase, phyAddr, S17_PHY_SPEC_STATUS);

    //if phy is powered down, registers all read 0xffff
    if (phyHwStatus == 0xffff)
        return 0;

#ifdef S17_8023AZ_FEATURE

    if (athrPhyInfo[phyUnit].azFeature & S17_8023AZ_PHY_ENABLED)
    {
        if (phyHwStatus & S17_STATUS_LINK_PASS)
        {       
            if (!(athrs17_reg_read(S17_EEE_CTRL_REG) & (S17_LPI_DISABLE_P1 << (phyUnit*2))))
            {
                DPRINTF_PHY("[az] PHY %d linked with 802.3az feature ON\n", phyUnit);
                while (1)
                {
                    phyHwStatus = phy_reg_read(phyBase, phyAddr, S17_PHY_SPEC_STATUS);
                    if (phyHwStatus & S17_STATUS_RESOLVED) break; 
                }

                if (((phyHwStatus & S17_STATUS_LINK_MASK) >> S17_STATUS_LINK_SHIFT) == S17_STATUS_LINK_1000M)
                    DPRINTF_PHY("[az] link speed @ 1000 Mbps (%d)\n", phyUnit);
                else if (((phyHwStatus & S17_STATUS_LINK_MASK) >> S17_STATUS_LINK_SHIFT) == S17_STATUS_LINK_100M)
                    DPRINTF_PHY("[az] link speed @ 100 Mbps (%d)\n", phyUnit);
                else if (((phyHwStatus & S17_STATUS_LINK_MASK) >> S17_STATUS_LINK_SHIFT) == S17_STATUS_LINK_10M)
                    DPRINTF_PHY("[az] link speed @ 10 Mbps (%d)\n", phyUnit);
            }
            athrPhyInfo[phyUnit].azFeature |= S17_8023AZ_PHY_LINKED; 
            return TRUE; /* linked */
        }else /* not linked */
        {
            DPRINTF_PHY("[az] PHY %d disconnected from az state, disable LPI\n", phyUnit);
            athrs17_reg_write(S17_EEE_CTRL_REG, \
              athrs17_reg_read(S17_EEE_CTRL_REG) | (S17_LPI_DISABLE_P1 << (phyUnit*2)));
            
            DPRINTF_PHY("[az] Phy %d is NOT linked \n", phyUnit);
            athrs17_reg_write(S17_P1STATUS_REG + phyUnit * 4, \
                  S17_PORT_STATUS_AZ_DEFAULT); 
            DPRINTF_PHY("[az] write MAC Xon-Xoff regs (Phy: %d)\n", phyUnit); 
            athrs17_reg_write(S17_P1_FLCTL_REG + phyUnit * 4, \
                  S17_PORT_FLCTL_XOFF_DEFAULT);
            athrs17_reg_write(S17_P1_FLCTL_REG + phyUnit * 4, 0);
            athrs17_reg_write(S17_P1_FLCTL_REG + phyUnit * 4, \
                  S17_PORT_FLCTL_XOFF_DEFAULT);
            athrs17_reg_write(S17_P1_FLCTL_REG + phyUnit * 4, \
                  (S17_PORT_FLCTL_XON_DEFAULT | S17_PORT_FLCTL_XOFF_DEFAULT));
            athrs17_reg_write(S17_P1STATUS_REG + phyUnit * 4, \
                  (S17_TX_HALF_FLOW_EN | S17_LINK_EN | S17_FLOW_LINK_EN )); 
            
            /* unset the linked flag */
            athrPhyInfo[phyUnit].azFeature &= ~S17_8023AZ_PHY_ENABLED;
            athrPhyInfo[phyUnit].azFeature &= ~S17_8023AZ_PHY_LINKED; 
            
            return FALSE; 
        }
    }else /* ! S17_8023AZ_PHY_ENABLED */
    {
        if (phyHwStatus & S17_STATUS_LINK_PASS)
        {
            int azStatus; 
            
            DPRINTF_PHY("[az] phy %d linked\n", phyUnit);
            DPRINTF_PHY("[az] check for 802.3az feature (Phy: %d)\n", phyUnit);
            phy_reg_write(0, phyUnit, S17_MMD_CTRL_REG, 0x7);
            phy_reg_write(0, phyUnit, S17_MMD_DATA_REG, 0x8000);
            phy_reg_write(0, phyUnit, S17_MMD_CTRL_REG, 0x4007);
            azStatus = phy_reg_read(0, phyUnit, S17_MMD_DATA_REG); 
            if ((azStatus & 0x2) || (azStatus & 0x6))
            {
                DPRINTF_PHY("[az] link partner has 802.3az feature, enable LPI (Phy: %d)\n", phyUnit);
                athrs17_reg_write(S17_EEE_CTRL_REG, \
                    athrs17_reg_read(S17_EEE_CTRL_REG) & ~(S17_LPI_DISABLE_P1 << (phyUnit*2)));
                
                athrPhyInfo[phyUnit].azFeature |= S17_8023AZ_PHY_ENABLED;
            }else
            {
                DPRINTF_PHY("[az] link partner does not have 802.2az feature. az status: %.8x\n", azStatus); 
            }
            
            athrPhyInfo[phyUnit].azFeature |= S17_8023AZ_PHY_LINKED; 
            
            return TRUE; 
        }else
        {
            athrPhyInfo[phyUnit].azFeature &= ~S17_8023AZ_PHY_LINKED; 
            return FALSE; 
        }
    }
    
#else
    if (phyHwStatus & S17_STATUS_LINK_PASS)
        return TRUE;

    return FALSE;
#endif
}

/******************************************************************************
*
* athrs17_phy_setup - reset and setup the PHY associated with
* the specified MAC unit number.
*
* Resets the associated PHY port.
*
* RETURNS:
*    TRUE  --> associated PHY is alive
*    FALSE --> no LINKs on this ethernet unit
*/

#ifdef CONFIG_MERAKI_GLUON
int
athrs17_phy_setup(void *arg)
{
    (void)arg;
    return 1;
}
#else

int
athrs17_phy_setup(void *arg)
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
    for (phyUnit=0; phyUnit < S17_PHY_MAX; phyUnit++) {
        if (!S17_IS_ETHUNIT(phyUnit, ethUnit)) {
            continue;
        }

        foundPhy = TRUE;
        phyBase = S17_PHYBASE(phyUnit);
        phyAddr = S17_PHYADDR(phyUnit);
        
        phy_reg_write(phyBase, phyAddr, S17_AUTONEG_ADVERT,
                      S17_ADVERTISE_ALL);

        phy_reg_write(phyBase, phyAddr, S17_1000BASET_CONTROL,
                      S17_ADVERTISE_1000FULL);

        /* Reset PHYs*/
        phy_reg_write(phyBase, phyAddr, S17_PHY_CONTROL,
                      S17_CTRL_AUTONEGOTIATION_ENABLE 
                      | S17_CTRL_SOFTWARE_RESET);
#ifdef S17_8023AZ_FEATURE
        athrPhyInfo[phyUnit].azFeature = 0;
#endif

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
    for (phyUnit=0; (phyUnit < S17_PHY_MAX); phyUnit++) {
        if (!S17_IS_ETHUNIT(phyUnit, ethUnit)) {
            continue;
        }

        timeout=20;
        for (;;) {
            phyHwStatus = phy_reg_read(phyBase, phyAddr, S17_PHY_CONTROL);

            if (S17_RESET_DONE(phyHwStatus)) {
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
    for (phyUnit=0; phyUnit < S17_PHY_MAX; phyUnit++) {
        if (!S17_IS_ETHUNIT(phyUnit, ethUnit)) {
            continue;
        }
         
        if (athrs17_phy_is_link_alive(phyUnit)) {
            liveLinks++;
            S17_IS_PHY_ALIVE(phyUnit) = TRUE;
        } else {
            S17_IS_PHY_ALIVE(phyUnit) = FALSE;
        }

        DRV_PRINT(DRV_DEBUG_PHYSETUP,
            ("eth%d: Phy Specific Status=%4.4x\n",
            ethUnit, 
            phy_reg_read(S17_PHYBASE(phyUnit),
                         S17_PHYADDR(phyUnit),
                         S17_PHY_SPEC_STATUS)));
    }
    DPRINTF_PHY("Phy setup Complete\n");

#ifdef S17_8023AZ_FEATURE
    if (!athrPhyInfo[0].azFeature)
    {
        int phyUnit; 
        
        for (phyUnit = 0; phyUnit < S17_PHY_MAX; phyUnit ++)
        {
            int phyLink = phy_reg_read(0, phyUnit, S17_PHY_SPEC_STATUS); 
        
            if (phyLink & S17_STATUS_LINK_PASS)
            {
              DPRINTF_PHY("[az] Phy %d is linked \n", phyUnit);
              DPRINTF_PHY("[az] write MAC Xon-Xoff regs (%.8x)\n", S17_P1_FLCTL_REG + phyUnit*4); 
              athrs17_reg_write(S17_P1_FLCTL_REG + phyUnit * 4, \
                  S17_PORT_FLCTL_XOFF_DEFAULT);
              athrs17_reg_write(S17_P1_FLCTL_REG + phyUnit * 4, 0);
              athrs17_reg_write(S17_P1_FLCTL_REG + phyUnit * 4, \
                  S17_PORT_FLCTL_XOFF_DEFAULT);
              athrs17_reg_write(S17_P1_FLCTL_REG + phyUnit * 4, \
                  (S17_PORT_FLCTL_XON_DEFAULT | S17_PORT_FLCTL_XOFF_DEFAULT));
              DPRINTF_PHY("[az] restart auto negotiation (Phy: %d)\n", phyUnit); 
              phy_reg_write(0, phyUnit, S17_PHY_CONTROL, \
                  (S17_CTRL_AUTONEGOTIATION_ENABLE | S17_CTRL_RESTART_AUTONEGOTIATION));
            }else
            {
              DPRINTF_PHY("[az] Phy %d is NOT linked \n", phyUnit);
              athrs17_reg_write(S17_P1STATUS_REG + phyUnit * 4, \
                  S17_PORT_STATUS_AZ_DEFAULT); 
              DPRINTF_PHY("[az] write MAC Xon-Xoff regs (%.8x)\n", S17_P1_FLCTL_REG + phyUnit * 4); 
              athrs17_reg_write(S17_P1_FLCTL_REG + phyUnit * 4, \
                  S17_PORT_FLCTL_XOFF_DEFAULT);
              athrs17_reg_write(S17_P1_FLCTL_REG + phyUnit * 4, 0);
              athrs17_reg_write(S17_P1_FLCTL_REG + phyUnit * 4, \
                  S17_PORT_FLCTL_XOFF_DEFAULT);
              athrs17_reg_write(S17_P1_FLCTL_REG + phyUnit * 4, \
                  (S17_PORT_FLCTL_XON_DEFAULT | S17_PORT_FLCTL_XOFF_DEFAULT));
              athrs17_reg_write(S17_P1STATUS_REG + phyUnit * 4, \
                  (S17_TX_HALF_FLOW_EN | S17_LINK_EN | S17_FLOW_LINK_EN )); 
            }
            DPRINTF_PHY("[az] 802.3az init done.. \n");
        }
    }
    /* end of 802.3az feature */
#endif
  
    return (liveLinks > 0);
}
#endif //CONFIG_MERAKI_GLUON

/******************************************************************************
*
* athrs17_phy_is_fdx - Determines whether the phy ports associated with the
* specified device are FULL or HALF duplex.
*
* RETURNS:
*    1 --> FULL
*    0 --> HALF
*/
int
athrs17_phy_is_fdx(int ethUnit, int phyUnit)
{
    uint32_t  phyBase;
    uint32_t  phyAddr;
    uint16_t  phyHwStatus;
    int       ii = 200;

    if (ethUnit == ENET_UNIT_GE0)
        return TRUE;

    for (phyUnit=0; phyUnit < S17_PHY_MAX; phyUnit++) {
        if (!S17_IS_ETHUNIT(phyUnit, ethUnit)) {
            continue;
        }

        if (athrs17_phy_is_link_alive(phyUnit)) {

            phyBase = S17_PHYBASE(phyUnit);
            phyAddr = S17_PHYADDR(phyUnit);

            do {
                phyHwStatus = phy_reg_read (phyBase, phyAddr, 
                                               S17_PHY_SPEC_STATUS);
		if(phyHwStatus & S17_STATUS_RESOLVED)
			break;
                mdelay(10);
            } while(--ii);
            
            if (phyHwStatus & S17_STATUS_FULL_DEPLEX)
                return TRUE;
        }
    }

    return FALSE;
}

/******************************************************************************
*
* athrs17_phy_speed - Determines the speed of phy ports associated with the
* specified device.
*
* RETURNS:
*               ATHR_PHY_SPEED_10T, ATHR_PHY_SPEED_100TX;
*               ATHR_PHY_SPEED_1000T;
*/

int
athrs17_phy_speed(int ethUnit, int phyUnit)
{
    uint16_t  phyHwStatus;
    uint32_t  phyBase;
    uint32_t  phyAddr;
    int       ii = 200;
    athr_phy_speed_t phySpeed = 0;

    for (phyUnit=0; phyUnit < S17_PHY_MAX; phyUnit++) {
        if (!S17_IS_ETHUNIT(phyUnit, ethUnit)) {
            continue;
        }

        phyBase = S17_PHYBASE(phyUnit);
        phyAddr = S17_PHYADDR(phyUnit);
        phySpeed = ATHR_PHY_SPEED_10T;

        if (athrs17_phy_is_link_alive(phyUnit)) {

            do {
                phyHwStatus = phy_reg_read(phyBase, phyAddr, 
                                              S17_PHY_SPEC_STATUS);
		        if(phyHwStatus & S17_STATUS_RESOLVED)
			        break;
                mdelay(10);
            }while(--ii);
            
            phyHwStatus = ((phyHwStatus & S17_STATUS_LINK_MASK) >>
                           S17_STATUS_LINK_SHIFT);

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
                DPRINTF_PHY(MODULE_NAME":Unkown speed read:%d! phyhwstat:%x\n",phyUnit,phyHwStatus);
            }
        } 

        
        
    }

    if (ethUnit == ENET_UNIT_GE0)
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
athrs17_phy_is_up(int ethUnit)
{
    int           phyUnit;
    uint16_t      phyHwStatus, phyHwControl;
    athrPhyInfo_t *lastStatus;
    int           linkCount   = 0;
    int           lostLinks   = 0;
    int           gainedLinks = 0;
    uint32_t      phyBase;
    uint32_t      phyAddr;

    for (phyUnit=0; phyUnit < S17_PHY_MAX; phyUnit++) {
        if (!S17_IS_ETHUNIT(phyUnit, ethUnit)) {
            continue;
        }

        phyBase = S17_PHYBASE(phyUnit);
        phyAddr = S17_PHYADDR(phyUnit);

        lastStatus = &athrPhyInfo[phyUnit];

        if (lastStatus->isPhyAlive) { /* last known link status was ALIVE */
            phyHwStatus = phy_reg_read(phyBase, phyAddr, S17_PHY_SPEC_STATUS);

            /* See if we've lost link */
            if (phyHwStatus & S17_STATUS_LINK_PASS) {
                linkCount++;
            } else {
                lostLinks++;
                DRV_PRINT(DRV_DEBUG_PHYCHANGE,("\nenet%d port%d down\n",
                                               ethUnit, phyUnit));
                DPRINTF_PHY("\nEnet:%d port:%d down\n",
                                              ethUnit, phyUnit);

                /* Flush the port from ARL entry */
                athrs17_reg_rmw(S17_ATU_FUNC_REG,(phyUnit << 8)); 

                lastStatus->isPhyAlive = FALSE;
            }
        } else { /* last known link status was DEAD */
            /* Check for reset complete */
            phyHwStatus = phy_reg_read(phyBase, phyAddr, S17_PHY_STATUS);
            if (!S17_RESET_DONE(phyHwStatus))
                continue;

            phyHwControl = phy_reg_read(phyBase, phyAddr, S17_PHY_CONTROL);
            /* Check for AutoNegotiation complete */            
            if ((!(phyHwControl & S17_CTRL_AUTONEGOTIATION_ENABLE)) 
                 || S17_AUTONEG_DONE(phyHwStatus)) {
                phyHwStatus = phy_reg_read(phyBase, phyAddr, 
                                           S17_PHY_SPEC_STATUS);

                if (phyHwStatus & S17_STATUS_LINK_PASS) {
                gainedLinks++;
                linkCount++;
                DRV_PRINT(DRV_DEBUG_PHYCHANGE,("\nenet%d port%d up\n",
                                               ethUnit, phyUnit));
                DPRINTF_PHY("\nEnet:%d port%d up\n",ethUnit, phyUnit);
                lastStatus->isPhyAlive = TRUE;
                }
            }
        }
    }

    return (linkCount);

}

unsigned int
athrs17_reg_read(unsigned int reg_addr)
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
    phy_val = (uint16_t) ((reg_word_addr >> 8) & 0x3ff);  /* bit16-8 of reg address */
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
athrs17_reg_write(unsigned int reg_addr, unsigned int reg_val)
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
    phy_val = (uint16_t) ((reg_word_addr >> 8) & 0x3ff);  /* bit16-8 of reg address */
    phy_reg_write(0, phy_addr, phy_reg, phy_val);

    /* For S17 registers such as ARL and VLAN, since they include BUSY bit */
    /* in higher address, we should write the lower 16-bit register then the */
    /* higher one */

    /* write register in lower address */
    phy_addr = 0x10 | ((reg_word_addr >> 5) & 0x7); /* bit7-5 of reg address */
    phy_reg = (uint8_t) (reg_word_addr & 0x1f);   /* bit4-0 of reg address */
    phy_val = (uint16_t) (reg_val & 0xffff);
    phy_reg_write(0, phy_addr, phy_reg, phy_val); 
    
    /* read register in higher address */
    reg_word_addr++;
    phy_addr = 0x10 | ((reg_word_addr >> 5) & 0x7); /* bit7-5 of reg address */
    phy_reg = (uint8_t) (reg_word_addr & 0x1f);   /* bit4-0 of reg address */
    phy_val = (uint16_t) ((reg_val >> 16) & 0xffff);
    phy_reg_write(0, phy_addr, phy_reg, phy_val);
}

static void
athrs17_reg_rmw(unsigned int reg_addr, unsigned int reg_val)
{
	reg_val |= athrs17_reg_read(reg_addr);
        athrs17_reg_write(reg_addr,reg_val);
}

void athrs17_powersave_off(int phy_addr)
{
    s17_wr_phy(0, phy_addr,S17_DEBUG_PORT_ADDRESS,0x29);
    s17_wr_phy(0, phy_addr,S17_DEBUG_PORT_DATA,0x36c0);

}

void athrs17_sleep_off(int phy_addr)
{
    s17_wr_phy(0, phy_addr,S17_DEBUG_PORT_ADDRESS,0xb);
    s17_wr_phy(0, phy_addr,S17_DEBUG_PORT_DATA,0x3c00);
}

void athrs17_enable_link_intrs(void *arg)
{
    int phyUnit = 0, phyAddr = 0;
    /* Enable global PHY related interrupt */
    athrs17_reg_write(S17_GLOBAL_INTMASK1, S17_GLOBAL_INT_PHYMASK);

    for (phyUnit = 0; phyUnit < S17_PHY_MAX; phyUnit++) {
       phyAddr = S17_PHYADDR(phyUnit);
       phy_reg_write(0,phyAddr,S17_PHY_INTR_ENABLE, S17_PHY_LINK_INTRS);
    }
}

void athrs17_disable_link_intrs(void *arg) 
{
    int phyUnit = 0, phyAddr = 0;

    for (phyUnit = 0; phyUnit < S17_PHY_MAX; phyUnit++) {
        phyAddr = S17_PHYADDR(phyUnit);
        phy_reg_write(0,phyAddr,S17_PHY_INTR_ENABLE, 0x0);
    }
}

void athrs17_force_100M(int phyAddr,int duplex) 
{
    if(phyAddr%2) {
        s17_wr_phy(0, phyAddr, S17_PHY_SPEC_CONTROL,0x820);
    }
    else { 
        s17_wr_phy(0,phyAddr, S17_PHY_SPEC_CONTROL,0x800);
    }

    s17_wr_phy(0,phyAddr,S17_DEBUG_PORT_ADDRESS,0x29);
    s17_wr_phy(0,phyAddr,S17_DEBUG_PORT_DATA,0x0);
    s17_wr_phy(0,phyAddr,0x10,0xc60);
    s17_wr_phy(0,phyAddr,S17_PHY_CONTROL,(0xa000|(duplex << 8)));
}

void athrs17_force_10M(int phyAddr,int duplex) 
{

    athrs17_powersave_off(phyAddr);
    athrs17_sleep_off(phyAddr);

    s17_wr_phy(0, phyAddr,S17_PHY_CONTROL,(0x8000 |(duplex << 8)));
}

void ar934x_s17_intr(void *arg)
{
    int status = 0, intr_reg_val;
    uint32_t phyUnit = 0 ,phyBase = 0 ,phyAddr = 0;
    uint32_t phymask = 0x0;
    uint32_t linkDown = 0x0;
    athr_gmac_t *mac0 = athr_macs[0];


    athrs17_reg_write(S17_GLOBAL_INTMASK1, 0x0);

    intr_reg_val = athrs17_reg_read(S17_GLOBAL_INT1_REG);


    /* clear global link interrupt */
    athrs17_reg_write(S17_GLOBAL_INT1_REG, intr_reg_val);

    if (intr_reg_val & S17_GLOBAL_INT_PHYMASK) {

       for (phyUnit=0; phyUnit < S17_PHY_MAX; phyUnit++) {

           phyBase = S17_PHYBASE(phyUnit);
           phyAddr = S17_PHYADDR(phyUnit);
           status = phy_reg_read(0,phyAddr, S17_PHY_INTR_STATUS);

           if(status & S17_PHY_LINK_UP) {
               DPRINTF("LINK UP - Port %d:%x\n",phyAddr,status);
               phymask = (phymask | (1 << phyUnit));
           }
           if(status & S17_PHY_LINK_DOWN) {
               DPRINTF("LINK DOWN - Port %d:%x\n",phyAddr,status);
               phymask = (phymask | (1 << phyUnit));
               linkDown = (linkDown | (1 << phyUnit));
           }
           if(status & S17_PHY_LINK_DUPLEX_CHANGE) {
               DPRINTF("LINK DUPLEX CHANGE - Port %d:%x\n",phyAddr,status);
               phymask = (phymask | (1 << phyUnit));
           }
           if(status & S17_PHY_LINK_SPEED_CHANGE) {
               DPRINTF("LINK SPEED CHANGE %d:%x\n",phyAddr,status);
               phymask = (phymask | (1 << phyUnit));
           }
       }
       for (phyUnit=0; phyUnit < S17_PHY_MAX; phyUnit++) {
           if ((phymask >> phyUnit) & 0x1) {

               phyBase = S17_PHYBASE(phyUnit);
               phyAddr = S17_PHYADDR(phyUnit);

               status = phy_reg_read(0,phyAddr,S17_PHY_INTR_STATUS);

               if (!athrs17_phy_is_link_alive(phyUnit) && !((linkDown >> phyUnit) & 0x1))
                   continue;

               mac0->ops->check_link(mac0,phyUnit);
           }
       }
       
       athrs17_reg_write(S17_GLOBAL_INTMASK1, S17_GLOBAL_INT_PHYMASK);
   }
   else  {
      DPRINTF("Spurious link interrupt:%s,status:%x\n",__func__,status);
      athrs17_reg_write(S17_GLOBAL_INTMASK1, S17_GLOBAL_INT_PHYMASK);
   }
}

int
athrs17_ioctl(struct net_device *dev, void *args, int cmd)
{
        return 0;
}

unsigned int s17_rd_phy(int ethUnit,unsigned int phy_addr, unsigned int reg_addr)
{
      return (phy_reg_read(ethUnit,phy_addr,reg_addr));

}

void s17_wr_phy(int ethUnit,unsigned int phy_addr, unsigned int reg_addr, unsigned int write_data)
{
      phy_reg_write(ethUnit,phy_addr,reg_addr,write_data);          

}

#ifdef S17_REGISTER_DUMP
static int athrs17_seq_show(struct seq_file *file, void *v)
{
  int i; 
  
  seq_printf(file, "Atheros ETHROS S832xN test... \n");
  seq_printf(file, "---== REG ==---\t ---== VAL ==---\n");
  for (i = 0; i < 16; i++)
  {
    seq_printf(file, "   0x%.8x\t   0x%.8x\n", i*4, athrs17_reg_read(i*4));
  }
  
  /* for HNAT functions */
  seq_printf(file, "--=== Router MAC ==---\n");
  for (i = 0; i < 16; i++)
  {
    seq_printf(file, "    0x%.8x -- %.8x-%.8x-%.8x\n", 0x2000 + 16*i, \
        athrs17_reg_read(0x2000 + 16*i), athrs17_reg_read(0x2000 + 16*i + 4), \
        athrs17_reg_read(0x2000 + 16*i + 8));
    seq_printf(file, "    0x%.8x -- %.8x-%.8x-%.8x\n", 0x5a900 + 16*i, \
        athrs17_reg_read(0x5a900 + 16*i), athrs17_reg_read(0x5a900 + 16*i + 4), \
        athrs17_reg_read(0x5a900 + 16*i + 8));
  }
    
  return 0;
}

static int athrs17_seq_open(struct inode *inode, struct file *file)
{
    return single_open_net(inode, file, athrs17_seq_show);
}

static int athrs17_proc_init(struct net *net)
{
  if (!proc_net_fops_create(net, "athrS832xN", S_IRUGO, &athrs17_seq_fops))
      return -ENOMEM;
  
  printk("Created a proc entry for athrs832xN \n");

  return 1;
}
#endif

int athrs17_register_ops(void *arg)
{
  athr_gmac_t *mac   = (athr_gmac_t *) arg;
  athr_phy_ops_t *ops = mac->phy;

  if(!ops) 
     ops = kmalloc(sizeof(athr_phy_ops_t), GFP_KERNEL);

  memset(ops,0,sizeof(athr_phy_ops_t));

  ops->mac            =  mac;
  ops->is_up          =  athrs17_phy_is_up;
  ops->is_alive       =  athrs17_phy_is_link_alive;
  ops->speed          =  athrs17_phy_speed;
  ops->is_fdx         =  athrs17_phy_is_fdx;
  ops->ioctl          =  athrs17_ioctl;
  ops->setup          =  athrs17_phy_setup;
  ops->stab_wr        =  NULL;
  ops->link_isr       =  ar934x_s17_intr;
  ops->en_link_intrs  =  athrs17_enable_link_intrs;
  ops->dis_link_intrs =  athrs17_disable_link_intrs;
  ops->read_phy_reg   =  s17_rd_phy;
  ops->write_phy_reg  =  s17_wr_phy;
  ops->read_mac_reg   =  athrs17_reg_read;
  ops->write_mac_reg  =  athrs17_reg_write;
  ops->init           =  athrs17_reg_init;
  mac->phy            =  ops;

  if (mac->mac_unit == 0) {
        athr_macs[0]     =  mac;
        ops->arg_ad[0]   = (void *)mac;
        ops->port_map    = 0x7f;
  }
  else {
        athr_macs[1]    =  mac;
        ops->arg_ad[1]  = (void *)mac;
  }

#ifdef S17_REGISTER_DUMP
  /* register proc file system */
  athrs17_proc_init(&init_net); 
#endif

  return 0;
}
