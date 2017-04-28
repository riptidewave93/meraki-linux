#include "athrs_mac.h"
#include "athrs_phy.h"
#include "955x.h"
#define MODULE_NAME "955x_SGMII"
#define SGMII_LINK_WAR_MAX_TRY 10


/*
 * Reads the sgmii status register
 * and gives the info @ ? speed the 
 * the SGMII link has negotiated with the phy.
 * 0 -10Mbps.
 * 1 -100Mbps.
 * 2 -1000Mbps.
 */
int athrs_sgmii_get_speed(athr_gmac_t *mac, athr_phy_speed_t  *rate)
{
    int phy_hw_status = 0x0;
    phy_hw_status = athr_reg_rd(SGMII_MAC_RX_CONFIG_ADDRESS);

    switch (SGMII_MAC_RX_CONFIG_SPEED_MODE_GET(phy_hw_status)) {
        case 0:
                return (*rate = ATHR_PHY_SPEED_10T);
                break;
        case 1:
                return (*rate = ATHR_PHY_SPEED_100T);
                break;
        case 2:
                return (*rate = ATHR_PHY_SPEED_1000T);
                break;
        default:
                return (*rate = ATHR_PHY_SPEED_UNKNOWN);
                break;
    }

   return ATHR_PHY_SPEED_UNKNOWN;

}

/*
 * Reads the SGMII status register and check if  
 * duplex bit is set.
 *
 *  Return 
 *       SGMII_FULL_DUPLEX if duplex (12 bit) is set.
 *       SGMII_HALF_DUPLEX if duplex bit is not set.
 */

int athr_gmac_sgmii_get_duplex(athr_gmac_t *mac, athr_gmac_duplex_t *duplex )
{
    int phy_hw_status = 0x0;

    phy_hw_status = athr_reg_rd(SGMII_MAC_RX_CONFIG_ADDRESS);


    if (SGMII_MAC_RX_CONFIG_DUPLEX_MODE_GET(phy_hw_status) == ATHR_SGMII_FULL_DUPLEX) {
        return (*duplex = ATHR_SGMII_FULL_DUPLEX);
    } else if (SGMII_MAC_RX_CONFIG_DUPLEX_MODE_GET(phy_hw_status) == ATHR_SGMII_HALF_DUPLEX) {
        return (*duplex = ATHR_SGMII_HALF_DUPLEX);
    }
    
   return 0;


}

/* 
 * Reads the SGMII sataus register and check if link
 * is alive bit is set.
 * Return:
 *        SGMII_LINK_IS_ALIVE;
 *        SGMII_LINK_IS_DEAD;
 */

int athr_sgmii_get_link_status(athr_gmac_t *mac, athr_gmac_sgmii_link_t *link)
{
    int phy_hw_status = 0x0;
    phy_hw_status = athr_reg_rd(SGMII_MAC_RX_CONFIG_ADDRESS);

    if (SGMII_MAC_RX_CONFIG_LINK_GET(phy_hw_status))
        return (*link = SGMII_LINK_IS_ALIVE);
    else
        return (*link = SGMII_LINK_IS_DEAD);
}

/*
 * SGMII SETUP
 */

void athr_gmac_sgmii_setup(athr_gmac_t *mac)
{
	uint32_t status = 0, count = 0;
	if (mac_has_flag(mac, ATHR_SGMII_FORCED)) {
		athr_reg_wr(MR_AN_CONTROL_ADDRESS, MR_AN_CONTROL_SPEED_SEL1_SET(1) |
				MR_AN_CONTROL_PHY_RESET_SET(1) |
				MR_AN_CONTROL_DUPLEX_MODE_SET(1));
		udelay(10);

		athr_reg_wr(SGMII_CONFIG_ADDRESS, SGMII_CONFIG_MODE_CTRL_SET(2) |
				SGMII_CONFIG_FORCE_SPEED_SET(1) |
				SGMII_CONFIG_SPEED_SET(2));

	} else {
		athr_reg_wr(SGMII_CONFIG_ADDRESS, SGMII_CONFIG_MODE_CTRL_SET(2));

		athr_reg_wr(MR_AN_CONTROL_ADDRESS, MR_AN_CONTROL_AN_ENABLE_SET(1) |
				MR_AN_CONTROL_PHY_RESET_SET(1));
	}

	/*
	 * SGMII reset sequence suggested by systems team.
	 */

	athr_reg_wr(SGMII_RESET_ADDRESS, SGMII_RESET_RX_CLK_N_RESET);

	athr_reg_wr(SGMII_RESET_ADDRESS, SGMII_RESET_HW_RX_125M_N_SET(1));

	athr_reg_wr(SGMII_RESET_ADDRESS, SGMII_RESET_HW_RX_125M_N_SET(1) |
			SGMII_RESET_RX_125M_N_SET(1));

	athr_reg_wr(SGMII_RESET_ADDRESS, SGMII_RESET_HW_RX_125M_N_SET(1) |
			SGMII_RESET_TX_125M_N_SET(1) |
			SGMII_RESET_RX_125M_N_SET(1));

	athr_reg_wr(SGMII_RESET_ADDRESS, SGMII_RESET_HW_RX_125M_N_SET(1) |
			SGMII_RESET_TX_125M_N_SET(1) |
			SGMII_RESET_RX_125M_N_SET(1) |
			SGMII_RESET_RX_CLK_N_SET(1));

	athr_reg_wr(SGMII_RESET_ADDRESS, SGMII_RESET_HW_RX_125M_N_SET(1) |
			SGMII_RESET_TX_125M_N_SET(1) |
			SGMII_RESET_RX_125M_N_SET(1) |
			SGMII_RESET_RX_CLK_N_SET(1) |
			SGMII_RESET_TX_CLK_N_SET(1));

	athr_reg_rmw_clear(MR_AN_CONTROL_ADDRESS, MR_AN_CONTROL_PHY_RESET_SET(1));

	/*
	 * WAR::Across resets SGMII link status goes to weird
	 * state.
	 * if 0xb8070058 (SGMII_DEBUG register) reads other then 0x1f or 0x10
	 * for sure we are in bad  state.
	 * Issue a PHY reset in MR_AN_CONTROL_ADDRESS to keep going.
	 */

	status = (athr_reg_rd(SGMII_DEBUG_ADDRESS) & 0xff);
	while (!(status == 0xf || status == 0x10)) {

		athr_reg_rmw_set(MR_AN_CONTROL_ADDRESS, MR_AN_CONTROL_PHY_RESET_SET(1));
		udelay(100);
		athr_reg_rmw_clear(MR_AN_CONTROL_ADDRESS, MR_AN_CONTROL_PHY_RESET_SET(1));
		if (count++ == SGMII_LINK_WAR_MAX_TRY) {
			printk (MODULE_NAME "Max resets limit reached exiting...\n");
			break;
		}
		status = (athr_reg_rd(SGMII_DEBUG_ADDRESS) & 0xff);
	}

	printk (MODULE_NAME "::%s  Done\n",__func__);

}


void
athr_gmac_sgmii_intr(ATHR_MAC_TASK_ARG)
{
    int isr, imr, status, count = 0;
    //ATHR_MAC_TASK_SGMII();
    
    isr = athr_reg_rd(SGMII_INTERRUPT_ADDRESS);
    imr = athr_reg_rd(SGMII_INTERRUPT_MASK_ADDRESS);

    if (isr & SGMII_LINK_MAC_CHANGE) {
        athr_reg_rmw_clear(SGMII_INTERRUPT_ADDRESS, SGMII_LINK_MAC_CHANGE);
        printk ("Sgmii Link mac change  isr::%x imr::%x\n",isr,imr);

        status = athr_reg_rd(SGMII_SERDES_ADDRESS);
        if (SGMII_SERDES_FIBER_SDO_GET(status)) {
            status = (athr_reg_rd(SGMII_DEBUG_ADDRESS) & 0xff);
            DPRINTF (MODULE_NAME "Link up\n");
            while (!(status == 0xf || status == 0x10)) {
                
                athr_reg_rmw_set(MR_AN_CONTROL_ADDRESS, MR_AN_CONTROL_PHY_RESET_SET(1));
                udelay(100);
                athr_reg_rmw_clear(MR_AN_CONTROL_ADDRESS, MR_AN_CONTROL_PHY_RESET_SET(1));
                if (count++ == SGMII_LINK_WAR_MAX_TRY) {
                        printk (MODULE_NAME "Max resets limit reached exiting...\n");
                        break;
                }
                status = (athr_reg_rd(SGMII_DEBUG_ADDRESS) & 0xff);
            }
        } else {
            DPRINTF (MODULE_NAME "Link Down:\n");
        }

        athr_reg_rmw_set(SGMII_INTERRUPT_MASK_ADDRESS, SGMII_LINK_MAC_CHANGE);
    }
}


