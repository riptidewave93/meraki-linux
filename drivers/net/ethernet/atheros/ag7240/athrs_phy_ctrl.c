#include <linux/version.h>
#include <linux/types.h>
#include <linux/netdevice.h>
#include <linux/etherdevice.h>
#include <generated/autoconf.h>
#include <linux/delay.h>
#include "athrs_ioctl.h"
#include "athrs_config.h"
#include "athrs_mac.h"

int  athr_mib_tx_stats(struct net_device *dev, struct ifreq *ifr, int phyBase)
{       
    
    athr_gmac_t *mac = (athr_gmac_t *) ATHR_MAC_PRIV(dev);
    athr_phy_ops_t *phy = mac->phy;

    struct eth_cfg_params *ethcfg;

    ethcfg = (struct eth_cfg_params *) ifr->ifr_data;
   
    ethcfg->txcntr.tx_broad	=  phy->read_mac_reg((athr_mibreg(ethcfg->portnum) | ATHR_PHY_TX_BROAD_REG));
    ethcfg->txcntr.tx_pause	=  phy->read_mac_reg((athr_mibreg(ethcfg->portnum) | ATHR_PHY_TX_PAUSE_REG));
    ethcfg->txcntr.tx_multi	=  phy->read_mac_reg((athr_mibreg(ethcfg->portnum) | ATHR_PHY_TX_MULTI_REG));
    ethcfg->txcntr.tx_underrun	=  phy->read_mac_reg((athr_mibreg(ethcfg->portnum) | ATHR_PHY_TX_MULTI_REG));
    ethcfg->txcntr.tx_64b	=  phy->read_mac_reg((athr_mibreg(ethcfg->portnum) | ATHR_PHY_TX_64B_REG));
    ethcfg->txcntr.tx_128b	=  phy->read_mac_reg((athr_mibreg(ethcfg->portnum) | ATHR_PHY_TX_128B_REG));
    ethcfg->txcntr.tx_256b	=  phy->read_mac_reg((athr_mibreg(ethcfg->portnum) | ATHR_PHY_TX_256B_REG));
    ethcfg->txcntr.tx_512b	=  phy->read_mac_reg((athr_mibreg(ethcfg->portnum) | ATHR_PHY_TX_512B_REG));
    ethcfg->txcntr.tx_1024b     =  phy->read_mac_reg((athr_mibreg(ethcfg->portnum) | ATHR_PHY_TX_1024B_REG));
    ethcfg->txcntr.tx_1518b     =  phy->read_mac_reg((athr_mibreg(ethcfg->portnum) | ATHR_PHY_TX_1518B_REG));
    ethcfg->txcntr.tx_maxb	=  phy->read_mac_reg((athr_mibreg(ethcfg->portnum) | ATHR_PHY_TX_MAXB_REG));
    ethcfg->txcntr.tx_oversiz	=  phy->read_mac_reg((athr_mibreg(ethcfg->portnum) | ATHR_PHY_TX_OVSIZE_REG));
    ethcfg->txcntr.tx_bytel   	=  phy->read_mac_reg((athr_mibreg(ethcfg->portnum) | ATHR_PHY_TX_TXBYTEL_REG));
    ethcfg->txcntr.tx_byteh  	=  phy->read_mac_reg((athr_mibreg(ethcfg->portnum) | ATHR_PHY_TX_TXBYTEU_REG));
    ethcfg->txcntr.tx_collision	=  phy->read_mac_reg((athr_mibreg(ethcfg->portnum) | ATHR_PHY_TX_COLL_REG));
    ethcfg->txcntr.tx_abortcol 	=  phy->read_mac_reg((athr_mibreg(ethcfg->portnum) | ATHR_PHY_TX_ABTCOLL_REG));
    ethcfg->txcntr.tx_multicol 	=  phy->read_mac_reg((athr_mibreg(ethcfg->portnum) | ATHR_PHY_TX_MLTCOL_REG));
    ethcfg->txcntr.tx_singalcol	=  phy->read_mac_reg((athr_mibreg(ethcfg->portnum) | ATHR_PHY_TX_SINGCOL_REG)); 
    ethcfg->txcntr.tx_execdefer	=  phy->read_mac_reg((athr_mibreg(ethcfg->portnum) | ATHR_PHY_TX_EXDF_REG));
    ethcfg->txcntr.tx_defer	=  phy->read_mac_reg((athr_mibreg(ethcfg->portnum) | ATHR_PHY_TX_DEF_REG));
    ethcfg->txcntr.tx_latecol	=  phy->read_mac_reg((athr_mibreg(ethcfg->portnum) | ATHR_PHY_TX_LATECOL_REG));
return 0;	
}
int athr_mib_rx_stats(struct net_device *dev, struct ifreq *ifr, int phyBase)
{
    athr_gmac_t *mac = (athr_gmac_t *) ATHR_MAC_PRIV(dev);
    athr_phy_ops_t *phy = mac->phy;

    struct eth_cfg_params *ethcfg;

    ethcfg = (struct eth_cfg_params *)ifr->ifr_data;

     ethcfg->rxcntr.rx_broad        = phy->read_mac_reg((athr_mibreg(ethcfg->portnum) | ATHR_PHY_RX_BROAD_REG));
     ethcfg->rxcntr.rx_pause        = phy->read_mac_reg((athr_mibreg(ethcfg->portnum) | ATHR_PHY_RX_PAUSE_REG));
     ethcfg->rxcntr.rx_multi        = phy->read_mac_reg((athr_mibreg(ethcfg->portnum) | ATHR_PHY_RX_MULTI_REG));
     ethcfg->rxcntr.rx_fcserr       = phy->read_mac_reg((athr_mibreg(ethcfg->portnum) | ATHR_PHY_RX_FCSERR_REG));
     ethcfg->rxcntr.rx_allignerr    = phy->read_mac_reg((athr_mibreg(ethcfg->portnum) | ATHR_PHY_RX_ALIGNERR_REG));
     ethcfg->rxcntr.rx_runt         = phy->read_mac_reg((athr_mibreg(ethcfg->portnum) | ATHR_PHY_RX_RUNT_REG));
     ethcfg->rxcntr.rx_frag         = phy->read_mac_reg((athr_mibreg(ethcfg->portnum) | ATHR_PHY_RX_FRAGMENT_REG));
     ethcfg->rxcntr.rx_64b          = phy->read_mac_reg((athr_mibreg(ethcfg->portnum) | ATHR_PHY_RX_RUNT_REG));
     ethcfg->rxcntr.rx_128b         = phy->read_mac_reg((athr_mibreg(ethcfg->portnum) | ATHR_PHY_RX_128B_REG));
     ethcfg->rxcntr.rx_256b         = phy->read_mac_reg((athr_mibreg(ethcfg->portnum) | ATHR_phy_RX_256B_REG));
     ethcfg->rxcntr.rx_512b         = phy->read_mac_reg((athr_mibreg(ethcfg->portnum) | ATHR_PHY_RX_512B_REG));
     ethcfg->rxcntr.rx_1024b        = phy->read_mac_reg((athr_mibreg(ethcfg->portnum) | ATHR_PHY_RX_1024B_REG));
     ethcfg->rxcntr.rx_1518b        = phy->read_mac_reg((athr_mibreg(ethcfg->portnum) | ATHR_PHY_RX_1518B_REG));
     ethcfg->rxcntr.rx_maxb         = phy->read_mac_reg((athr_mibreg(ethcfg->portnum) | ATHR_PHY_RX_MAXB_REG));
     ethcfg->rxcntr.rx_tool         = phy->read_mac_reg((athr_mibreg(ethcfg->portnum) | ATHR_PHY_RX_TOLO_REG));
     ethcfg->rxcntr.rx_goodbl       = phy->read_mac_reg((athr_mibreg(ethcfg->portnum) | ATHR_PHY_RX_GOODBL_REG));
     ethcfg->rxcntr.rx_goodbh       = phy->read_mac_reg((athr_mibreg(ethcfg->portnum) | ATHR_PHY_RX_GOODBU_REG));
     ethcfg->rxcntr.rx_overflow     = phy->read_mac_reg((athr_mibreg(ethcfg->portnum) | ATHR_PHY_RX_OVERFLW_REG));
     ethcfg->rxcntr.rx_badbl        = phy->read_mac_reg((athr_mibreg(ethcfg->portnum) | ATHR_PHY_RX_BADBL_REG));
     ethcfg->rxcntr.rx_badbu        = phy->read_mac_reg((athr_mibreg(ethcfg->portnum) | ATHR_PHY_RX_BADBU_REG));
           
          return 0;
}

int athr_forcephy_10M(struct net_device *dev, int phyBase, int phy_addr, int duplex)
{
    athr_gmac_t *mac = (athr_gmac_t *) ATHR_MAC_PRIV(dev);
    athr_phy_ops_t *phy = mac->phy;
    
	
    if (is_s26()) {
        /*power save off */
        phy->write_phy_reg(phyBase, phy_addr, ATHR_DEBUG_PORT_ADDRESS, 0x29);
        phy->write_phy_reg(phyBase, phy_addr, ATHR_DEBUG_PORT_DATA, 0x36c0);
        /*sleep off */
        phy->write_phy_reg(phyBase, phy_addr, ATHR_DEBUG_PORT_ADDRESS, 0xb);
        phy->write_phy_reg(phyBase, phy_addr, ATHR_DEBUG_PORT_DATA, 0x3c00);
    }
    
    phy->write_phy_reg(phyBase, phy_addr, ATHR_PHY_CONTROL,
                           (0x8000 | (duplex << 8)));

    return 0;
}

int athr_forcephy_100M(struct net_device *dev,int phyBase,  int phy_addr, int duplex)
{
    athr_gmac_t *mac = (athr_gmac_t *) ATHR_MAC_PRIV(dev);
    athr_phy_ops_t *phy = mac->phy;
    
    if (is_s26()) {	
        if (phy_addr % 2) {
            phy->write_phy_reg(phyBase, phy_addr, ATHR_PHY_FUNC_CONTROL, 0x820);
        } else {
        
            phy->write_phy_reg(phyBase, phy_addr, ATHR_PHY_FUNC_CONTROL, 0x800);
        }

        phy->write_phy_reg(phyBase, phy_addr, 0x1d, 0x29);
        phy->write_phy_reg(phyBase, phy_addr, 0x1e, 0x0);
        phy->write_phy_reg(phyBase, phy_addr, 0x10, 0xc60);
    }

    phy->write_phy_reg(phyBase, phy_addr, ATHR_PHY_CONTROL,
                       (0xa000 | (duplex << 8)));

    //mac->ops->check_link(mac,mac->mac_unit);
    return 0;
}

int athr_phy_ctrl(struct net_device *dev, struct ifreq *ifr, int cmd)
{
    int ret = 0;
    int i;
    int phyBase = 0;
    int flow_link_en = 0; 
    int phy_addr=0, duplex;
    int infmask = 0,portmask = 0;
    athr_gmac_t *mac = (athr_gmac_t *) ATHR_MAC_PRIV(dev);
    athr_phy_ops_t *phy = mac->phy;
    struct eth_cfg_params *ethcfg;
    ethcfg = (struct eth_cfg_params *) ifr->ifr_data;
	
    assert(ethcfg);
    assert(phy);

    phy_addr = ethcfg->portnum;
    
    infmask  = (1 << (6 + mac->mac_unit));

    if (phy_addr == 0x3f) 
        portmask = phy_addr;
    else 
        portmask = (1 << phy_addr);

    /* Sanity checks */
#if HYBRID_SWITCH_PORT6_USED
    if (!((phy->port_map & 0x7f) & portmask) 
#else            
    if (!((phy->port_map & 0x3f) & portmask) 
#endif        
           && !((phy->port_map & 0xc) & infmask))
    {
	printk("\nport_map:%x mask:%x\n",phy->port_map,portmask);
        return -EINVAL;
    }
   
    if (!(phy->read_phy_reg || phy->write_phy_reg))
        return -EINVAL;

    if (!(phy->read_mac_reg || phy->write_mac_reg))
        return -EINVAL;

    if (cmd != ATHR_PHY_CTRL_IOC)
        return -EINVAL;

    if (soc_is_ar7240()) {
        phyBase = 0;
    }
    else if(soc_is_ar7241() || soc_is_ar7242() || soc_is_ar934x()) {
        if (is_s17() || is_s16())
            phyBase = 0;
        else
            phyBase = 1;
    }

    cmd=ethcfg->cmd;

    switch (cmd) {

        case ATHR_PHY_RD:
            if (ethcfg->portnum != 0x3f) {
                ethcfg->val = phy->read_phy_reg(phyBase,
                                 ethcfg->portnum, ethcfg->phy_reg);
            } else {
                ethcfg->val = phy->read_mac_reg(ethcfg->phy_reg);
            }
            break;
        case ATHR_PHY_WR:
            if (ethcfg->portnum != 0x3f) {

                phy->write_phy_reg(phyBase,ethcfg->portnum,
                        ethcfg->phy_reg, ethcfg->val);
            } else {
                phy->write_mac_reg(ethcfg->phy_reg, ethcfg->val);
            }
            break;
        case ATHR_PHY_FORCE:
            duplex = ethcfg->duplex;
            
            if (ethcfg->phy_reg < ATHR_PHY_MAX) {
                if (ethcfg->val == 10) {

                    printk("Forcing 10Mbps %s on port:%d \n",
                           dup_str[ethcfg->duplex], (ethcfg->portnum));
                    athr_forcephy_10M(dev, phyBase, phy_addr, duplex);

                } else if (ethcfg->val == 100) {

                    printk("Forcing 100Mbps %s on port:%d \n",
                           dup_str[ethcfg->duplex], (ethcfg->portnum));
                    athr_forcephy_100M(dev, phyBase, phy_addr, duplex);

                } else if (ethcfg->val == 0) {
                    printk("Enabling Auto Neg on port:%d \n", (ethcfg->phy_reg));
                    phy->write_phy_reg(phyBase, ethcfg->portnum,
                                       ATHR_PHY_CONTROL, 0x9000);

                } else {
                    return -EINVAL;
		}
		printk("Phy status:%x\n", 
 			phy->read_phy_reg(phyBase,ethcfg->portnum,ATHR_PHY_CONTROL));

            } else
                return -EINVAL;        
            break;
        case ATHR_PHY_MIB :
            if (ethcfg->val == 1){
                 phy->write_mac_reg(ATHR_PHY_MIB_CTRL_REG,
                     (phy->read_mac_reg(ATHR_PHY_MIB_CTRL_REG) | ATHR_PHY_MIB_ENABLE));
                 printk("MIB counter enabled\n");
         
            }
            else if(ethcfg->val == 0){
                 phy->write_mac_reg(ATHR_PHY_MIB_CTRL_REG,
                      (phy->read_mac_reg(ATHR_PHY_MIB_CTRL_REG) & ~ATHR_PHY_MIB_ENABLE));
                 printk("MIB counter disabled\n");
            }
            else if(ethcfg->val == 0xff){
                 ret = phy->read_mac_reg(ATHR_PHY_MIB_CTRL_REG);
		 if(ret & ATHR_PHY_MIB_ENABLE)
                     printk("MIB module cntr set\n");
                 else
                     printk("MIB module cntr is not set\n"); 
	    }            
            else
               return -EINVAL;

            break;
        case ATHR_PORT_STATS:
        {
            int portnum;
            int link;
            portnum = ethcfg->portnum +1;

#if HYBRID_LINK_CHANGE_EVENT
            {
            int rc = 0;
            link = phy->is_up(mac->mac_unit, &rc);
            }
#else
            link  = phy->is_up(mac->mac_unit);
#endif
	    ethcfg->phy_st.link     = phy->is_alive(ethcfg->portnum);
            ethcfg->phy_st.speed    = phy->speed(mac->mac_unit, ethcfg->portnum);
            ethcfg->phy_st.duplex   = phy->is_fdx(mac->mac_unit, ethcfg->portnum);
            ethcfg->phy_st.txflctrl = ((phy->read_mac_reg(athr_flctrl_off(portnum )) >> 4 ) & 1);
            ethcfg->phy_st.rxflctrl = ((phy->read_mac_reg(athr_flctrl_off(portnum )) >> 5 ) & 1);
            		
         }
            break;
        case ATHR_PHY_STATS:
            athr_mib_tx_stats(dev, ifr, phyBase); 
            athr_mib_rx_stats(dev, ifr, phyBase); 
            break;
        case ATHR_FLOW_LINK_EN:
            if (ethcfg->val == 1) {
                phy->write_mac_reg(athr_flctrl_off(ethcfg->portnum),
                     (phy->read_mac_reg(athr_flctrl_off(ethcfg->portnum)) | (1 << 12)));

                printk("Flow link is enabled\n");
            } else if (ethcfg->val == 0) {
                phy->write_mac_reg(athr_flctrl_off(ethcfg->portnum),
                     (phy->read_mac_reg(athr_flctrl_off(ethcfg->portnum)) & ~(1 << 12)));

                printk("Flow link is disabled\n");
            }
            else {
                return -EINVAL;
            }
            break;
        case ATHR_PHY_RXFCTL:

            flow_link_en = (phy->read_mac_reg(athr_flctrl_off(ethcfg->portnum)) & (1 << 12));
                if (flow_link_en) {
                     phy->write_mac_reg(athr_flctrl_off(ethcfg->portnum),
                               (phy->read_mac_reg(athr_flctrl_off(ethcfg->portnum)) & ~(1<<12)));

                     printk("Flow link disabled.\n");
                                     }
            if (ethcfg->val == 1) {

                phy->write_mac_reg(athr_flctrl_off(ethcfg->portnum),
                     (phy->read_mac_reg(athr_flctrl_off(ethcfg->portnum)) | ATHR_PHY_RXFCTL_EN));


                printk ("RX Flow ctrl is enabled\n");

            } else if (ethcfg->val == 0) {

                phy->write_mac_reg(athr_flctrl_off(ethcfg->portnum),
                     (phy->read_mac_reg(athr_flctrl_off(ethcfg->portnum)) & ~ATHR_PHY_RXFCTL_EN));


                printk ("RX Flow ctrl is disabled\n");
            } else {
                return -EINVAL;
            } 
            break;
        case ATHR_PHY_TXFCTL:
            flow_link_en = (phy->read_mac_reg(athr_flctrl_off(ethcfg->portnum)) & (1 << 12));
                if (flow_link_en) {

                    phy->write_mac_reg(athr_flctrl_off(ethcfg->portnum),
                         (phy->read_mac_reg(athr_flctrl_off(ethcfg->portnum)) & ~(1<<12)));

                    printk("Flow link disabled.\n");
                }
            if (ethcfg->val == 1) {

                phy->write_mac_reg(athr_flctrl_off(ethcfg->portnum),
                     (phy->read_mac_reg(athr_flctrl_off(ethcfg->portnum)) | ATHR_PHY_TXFCTL_EN));


                printk ("TX Flow ctrl is enabled\n");

            } else if (ethcfg->val == 0) {
                phy->write_mac_reg(athr_flctrl_off(ethcfg->portnum),
                     (phy->read_mac_reg(athr_flctrl_off(ethcfg->portnum)) & ~ATHR_PHY_TXFCTL_EN));


                printk ("TX Flow ctrl is disabled\n");
            } else {
                return -EINVAL;
            }
            break;
            
        case ATHR_PORT_LINK:
            ethcfg->val = phy->is_alive(ethcfg->portnum);	    
            break;	    

        case ATHR_PHY_FLOW_CTRL:
            if (ethcfg->val == 1) {

                printk("TX & RX Flow ctrl enabled in the switch side....\n");

                for (i = 1; i < MAX_PHY; i++ ) {
                    phy->write_mac_reg(athr_flctrl_off(i), 0x2b0);
                }

            } else if (ethcfg ->val == 0) {

                printk("TX & RX Flow ctrl disabled in the switch side....\n");

                for (i = 1; i < MAX_PHY; i++ ) {
                    phy->write_mac_reg(athr_flctrl_off(i), 0x280);
                }


            } else {
                return -EINVAL;
            }
            break;
        default:
            break;
    }
    if (copy_to_user(ifr->ifr_data, ethcfg, sizeof(struct eth_cfg_params)))
        return -EFAULT;

    return 0;
}
