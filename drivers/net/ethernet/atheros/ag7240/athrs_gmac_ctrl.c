#include <linux/version.h>
#include <linux/types.h>
#include <linux/netdevice.h>
#include <linux/etherdevice.h>
#include <linux/delay.h>
#include "athrs_ioctl.h"
#include "athrs_mac.h"
#if ETHDEBUG_ENABLED
#include "ethdebug.h"
extern struct eth_dbg_params eth_vi_dbg_params;
extern u_int32_t eth_rx_drop;
#endif
#define MAX_SIZ  10
#define MAX_VAL_S26 0x7ff
extern int frame_sz;
static char st_str[][MAX_SIZ] = { "disabled","enabled" };
static void 
athr_gmac_rx_stats(athr_gmac_t *mac, struct ifreq *ifr)
{
    struct eth_cfg_params *ethcfg;
    ethcfg = (struct eth_cfg_params *) ifr->ifr_data;


    ethcfg->rxmac.byte_cntr 		= athr_gmac_reg_rd(mac,ATHR_GMAC_RX_BYTES_CNTR);

    ethcfg->rxmac.pkt_cntr		= athr_gmac_reg_rd(mac,ATHR_GMAC_RX_PKT_CNTR);  

    ethcfg->rxmac.fcserr_cntr		= athr_gmac_reg_rd(mac,ATHR_GMAC_RX_CRC_ERR_CNTR);  

    ethcfg->rxmac.mcast_cntr		= athr_gmac_reg_rd(mac,ATHR_GMAC_RX_MULT_CNTR);  

    ethcfg->rxmac.bcast_cntr		= athr_gmac_reg_rd(mac,ATHR_GMAC_RX_RBCA_CNTR);  

    ethcfg->rxmac.ctrlframe_cntr	= athr_gmac_reg_rd(mac,ATHR_GMAC_RX_RXCF_CNTR);  

    ethcfg->rxmac.pausefr_cntr		= athr_gmac_reg_rd(mac,ATHR_GMAC_RX_RXPF_CNTR);  
 
    ethcfg->rxmac.unknownop_cntr	= athr_gmac_reg_rd(mac,ATHR_GMAC_RX_RXUO_CNTR);  
 
    ethcfg->rxmac.allignerr_cntr	= athr_gmac_reg_rd(mac,ATHR_GMAC_RX_FRM_ERR_CNTR);  

    ethcfg->rxmac.framelerr_cntr	= athr_gmac_reg_rd(mac,ATHR_GMAC_RX_LEN_ERR_CNTR);  

    ethcfg->rxmac.codeerr_cntr		= athr_gmac_reg_rd(mac,ATHR_GMAC_RX_CODE_ERR_CNTR);  

    ethcfg->rxmac.carriersenseerr_cntr	= athr_gmac_reg_rd(mac,ATHR_GMAC_RX_CRS_ERR_CNTR);  

    ethcfg->rxmac.underszpkt_cntr 	= athr_gmac_reg_rd(mac,ATHR_GMAC_RX_RUND_CNTR);  

    ethcfg->rxmac.ozpkt_cntr		= athr_gmac_reg_rd(mac,ATHR_GMAC_RX_OVL_ERR_CNTR);  

    ethcfg->rxmac.fragment_cntr		= athr_gmac_reg_rd(mac,ATHR_GMAC_RX_RFRG_CNTR);  

    ethcfg->rxmac.jabber_cntr		= athr_gmac_reg_rd(mac,ATHR_GMAC_RX_RJBR_CNTR);  

    ethcfg->rxmac.rcvdrop_cntr		= athr_gmac_reg_rd(mac,ATHR_GMAC_RX_DROP_CNTR);  
    ethcfg->rxmac.rxoverfl              = mac->eth_overfl_cntr;
}

static void 
athr_gmac_tx_stats(athr_gmac_t *mac, struct ifreq *ifr) 
{
    struct eth_cfg_params *ethcfg;
    ethcfg = (struct eth_cfg_params *) ifr->ifr_data;



    ethcfg->txmac.pkt_cntr	 	= athr_gmac_reg_rd(mac, ATHR_GMAC_TX_PKT_CNTR);

    ethcfg->txmac.byte_cntr	 	= athr_gmac_reg_rd(mac, ATHR_GMAC_TX_BYTES_CNTR);

    ethcfg->txmac.mcast_cntr		= athr_gmac_reg_rd(mac, ATHR_GMAC_TX_MULT_CNTR);

    ethcfg->txmac.bcast_cntr		= athr_gmac_reg_rd(mac, ATHR_GMAC_TX_BRD_CNTR);

    ethcfg->txmac.pctrlframe_cntr	= athr_gmac_reg_rd(mac, ATHR_GMAC_TX_PCTRL_CNTR);

    ethcfg->txmac.deferal_cntr		= athr_gmac_reg_rd(mac, ATHR_GMAC_TX_DFRL_CNTR);		

    ethcfg->txmac.excess_deferal_cntr	= athr_gmac_reg_rd(mac, ATHR_GMAC_TX_TEDF_CNTR);		

    ethcfg->txmac.single_col_cntr	= athr_gmac_reg_rd(mac, ATHR_GMAC_TX_TSCL_CNTR);		

    ethcfg->txmac.multi_col_cntr	= athr_gmac_reg_rd(mac, ATHR_GMAC_TX_TMCL_CNTR);		

    ethcfg->txmac.late_col_cntr		= athr_gmac_reg_rd(mac, ATHR_GMAC_TX_TLCL_CNTR);		

    ethcfg->txmac.excess_col_cntr	= athr_gmac_reg_rd(mac, ATHR_GMAC_TX_TXCL_CNTR);		

    ethcfg->txmac.total_col_cntr	= athr_gmac_reg_rd(mac, ATHR_GMAC_TX_TNCL_CNTR);		

    ethcfg->txmac.honored_cntr		= athr_gmac_reg_rd(mac, ATHR_GMAC_TX_TPFH_CNTR);		

    ethcfg->txmac.dropframe_cntr 	= athr_gmac_reg_rd(mac, ATHR_GMAC_TX_DROP_CNTR);

    ethcfg->txmac.jabberframe_cntr	= athr_gmac_reg_rd(mac, ATHR_GMAC_TX_TJBR_CNTR);		

    ethcfg->txmac.fcserr_cntr		= athr_gmac_reg_rd(mac, ATHR_GMAC_TX_CRC_ERR_CNTR);

    ethcfg->txmac.ctrlframe_cntr	= athr_gmac_reg_rd(mac, ATHR_GMAC_TX_TXCF_CNTR);		

    ethcfg->txmac.oz_frame_cntr		= athr_gmac_reg_rd(mac, ATHR_GMAC_TX_TOVR_CNTR);		

    ethcfg->txmac.us_frame_cntr		= athr_gmac_reg_rd(mac, ATHR_GMAC_TX_TUND_CNTR);		

    ethcfg->txmac.frag_frame_cntr	= athr_gmac_reg_rd(mac, ATHR_GMAC_TX_TFRG_CNTR);		
}

int 
athr_gmac_ctrl(struct net_device *dev, struct ifreq *ifr, int cmd)
{
    athr_gmac_t *mac = (athr_gmac_t *) ATHR_MAC_PRIV(dev);

    athr_phy_ops_t *phy = mac->phy;
    struct eth_cfg_params *ethcfg;
#if ETHDEBUG_ENABLED
    struct eth_dbg_params *edp;
    struct eth_params *ep;
    int i=0,j=0;
    char str[] = "ethdebug";
#endif

    ethcfg = (struct eth_cfg_params *) ifr->ifr_data;
    cmd=ethcfg->cmd;

    switch (cmd) {
        case ATHR_GMAC_TX_FLOW_CTRL:
            if (ethcfg->val == 1){
                athr_gmac_reg_rmw_set(mac, ATHR_GMAC_CFG1,
                                      ATHR_GMAC_CFG1_TX_FCTL);
            }
            else if (ethcfg->val == 0){
                athr_gmac_reg_rmw_clear(mac, ATHR_GMAC_CFG1,
                                        ATHR_GMAC_CFG1_TX_FCTL);
            }
            else
                return -EINVAL;
            
            printk("TXFCTL %s in Mac:%d\n", st_str[ethcfg->val], mac->mac_unit);
            break;
        case ATHR_GMAC_RX_FLOW_CTRL:
            if (ethcfg->val == 1){
                athr_gmac_reg_rmw_set(mac, ATHR_GMAC_CFG1,
                                      ATHR_GMAC_CFG1_RX_FCTL);
            }
            else if (ethcfg->val == 0){
                athr_gmac_reg_rmw_clear(mac, ATHR_GMAC_CFG1,
                                        ATHR_GMAC_CFG1_RX_FCTL);
            }
            else
                return -EINVAL;

            printk("RXFCTL %s in Mac:%d\n", st_str[ethcfg->val], mac->mac_unit);
            break;
        case ATHR_GMAC_FLOW_CTRL:
            if (ethcfg->val == 1) {
                printk("TX & RX flow ctrl enabled in GE0 & GE1 ....\n");

                athr_reg_rmw_set(0xb9000000, 
                     (ATHR_GMAC_CFG1_RX_FCTL | ATHR_GMAC_CFG1_TX_FCTL)); 

                if (is_s26() || is_s27()) {
                    athr_reg_rmw_set(0xba000000, 
                         (ATHR_GMAC_CFG1_RX_FCTL | ATHR_GMAC_CFG1_TX_FCTL)); 
                }


            }else if (ethcfg->val == 0) {

                printk("TX & RX flow ctrl disabled in GE0 & GE1 ....\n");

                    athr_reg_rmw_clear(0xb9000000, 
                         (ATHR_GMAC_CFG1_RX_FCTL | ATHR_GMAC_CFG1_TX_FCTL)); 

                if (is_s26() || is_s27()) {
                    athr_reg_rmw_clear(0xba000000, 
                         (ATHR_GMAC_CFG1_RX_FCTL | ATHR_GMAC_CFG1_TX_FCTL)); 
                }

            }else 
                return -EINVAL;
            break;
        case ATHR_GMAC_DMA_CHECK:
            if (ethcfg->val == 1) {
                mac_set_flag(mac, CHECK_DMA_STATUS);
                athr_stop_dev(dev);
                athr_open_dev(dev);
            } else if (ethcfg->val == 0) {
                mac_clear_flag(mac, CHECK_DMA_STATUS);
                athr_stop_dev(dev);
                athr_open_dev(dev);
            } else {
                return -EINVAL;
            }
            printk("DMA STATUS %s in Mac:%d\n",
                   st_str[ethcfg->val], mac->mac_unit);
            break;
        case ATHR_GMAC_SW_ONLY_MODE:
            if (ethcfg->val == 1) {
                athr_reg_rmw_set(ATHR_GMAC_ETH_CFG, ATHR_GMAC_ETH_CFG_SW_ONLY_MODE);
                athr_stop_dev(dev);
                athr_open_dev(dev);
            } else if (ethcfg->val == 0) {
                athr_reg_rmw_clear(ATHR_GMAC_ETH_CFG, ATHR_GMAC_ETH_CFG_SW_ONLY_MODE);
                athr_stop_dev(dev);
                athr_open_dev(dev);
            } else {

                return -EINVAL;
            }
            printk("SW_ONLY_MODE %s in Mac:%d\n",
                   st_str[ethcfg->val], mac->mac_unit);
            break;
        case ATHR_GMAC_STATS:
            athr_gmac_tx_stats (mac,ifr);
            athr_gmac_rx_stats (mac,ifr);
            break;
        case ATHR_JUMBO_FRAME:
            if (ethcfg->val == 1) {
                athr_gmac_reg_rmw_set(mac, ATHR_GMAC_CFG2,
                      ATHR_GMAC_CFG2_HUGE_FRAME_EN);

                mac_set_flag(mac,ATHR_JUMBO_FR);

            } else if(ethcfg->val == 0) {
                athr_gmac_reg_rmw_clear(mac, ATHR_GMAC_CFG2,
                     ATHR_GMAC_CFG2_HUGE_FRAME_EN);

                mac_clear_flag(mac,ATHR_JUMBO_FR);

            } else {
                return -EINVAL;
            }
            printk("Jumbo Frame %s in Mac:%d\n",
                  st_str[ethcfg->val], mac->mac_unit);
            break;
        case ATHR_FRAME_SIZE_CTL:
            if (is_s16()) {
                frame_sz = ethcfg->val;
            } else if (is_s26()) {
                if (ethcfg->val >= 0x7FF) {
                    ethcfg->val = MAX_VAL_S26;
                    frame_sz = ethcfg->val;
                }
            }
            else
                frame_sz = ethcfg->val;
            printk("Jumbo Frame sz val:%x\n",ethcfg->val);
            athr_gmac_reg_wr(mac, ATHR_GMAC_MAX_PKTLEN,ethcfg->val);
            athr_stop_dev(dev);
            athr_open_dev(dev);
            if (soc_is_qca955x()) {
                athr_gmac_reg_rmw_set(mac, DMA_RESET_OFFSET, (1 << 28) | (1 << 29));
            }


            if(!is_f1e())
            {
                if (phy->write_mac_reg == NULL)
                    return -EINVAL; 

                if (is_s16() && is_s17())
                    phy->write_mac_reg(0x78, (phy->read_mac_reg(0x78)& ~0x3FFF) | ethcfg->val);
                else if(is_s16())
                    phy->write_mac_reg(0x30, (phy->read_mac_reg(0x30)& ~0x3FFF) | ethcfg->val);
                else if(is_s26())
                    phy->write_mac_reg(0x30, (phy->read_mac_reg(0x30)& ~0x7FF)  | ethcfg->val);
                else
                    phy->write_mac_reg(0x30, (phy->read_mac_reg(0x30)& ~0x3FFF) | ethcfg->val);
            }
            break;
             
        
#if ETHDEBUG_ENABLED
        case ATHR_DBG_CONFIG:
            printk("%s config :\n",str);
            ep = (struct eth_params *)ethcfg;
            edp = (struct eth_dbg_params *)&ep->val;
            eth_vi_dbg_params.vi_dbg.rxseq_num_bytes = edp->vi_dbg.rxseq_num_bytes;
            eth_vi_dbg_params.vi_dbg.rxseq_offset = edp->vi_dbg.rxseq_offset-ETH_HLEN;
            eth_vi_dbg_params.vi_num_streams = edp->vi_num_streams;
            eth_vi_dbg_params.vi_num_markers = edp->vi_num_markers;
            for (i = 0; i < eth_vi_dbg_params.vi_num_streams; i++) 
            {
                for (j = 0; j < eth_vi_dbg_params.vi_num_markers; j++)   
                {
                    eth_vi_dbg_params.vi_dbg.markers[i][j].offset    = edp->vi_dbg.markers[i][j].offset-ETH_HLEN;
                    eth_vi_dbg_params.vi_dbg.markers[i][j].num_bytes = edp->vi_dbg.markers[i][j].num_bytes;
                    eth_vi_dbg_params.vi_dbg.markers[i][j].match     = edp->vi_dbg.markers[i][j].match;
                }

            }
            eth_vi_dbg_params.vi_rx_seq_max =  edp->vi_rx_seq_max;
            eth_vi_dbg_params.vi_dbg_cfg = edp->vi_dbg_cfg;
#if notyet
            printk(">>> dumping debug params \n");
            printk("\n Rxseqnumber offset : %04x and size: %d bytes",eth_vi_dbg_params.vi_dbg.rxseq_offset,eth_vi_dbg_params.vi_dbg.rxseq_num_bytes);
            printk("\n Numbers of streams: %d",eth_vi_dbg_params.vi_num_streams);
            printk("\n Numbers of markers: %d",eth_vi_dbg_params.vi_num_markers);
            for (i = 0; i < eth_vi_dbg_params.vi_num_streams; i++)  
            {
                printk("\n -- Stream %d configuration -- ",i+1);  
                for (j = 0; j < eth_vi_dbg_params.vi_num_markers; j++)   
                    printk("\nmarker%d: Offset: %04x Size: %d Match: %08x",  j+1,eth_vi_dbg_params.vi_dbg.markers[i][j].offset, eth_vi_dbg_params.vi_dbg.markers[i][j].num_bytes, eth_vi_dbg_params.vi_dbg.markers[i][j].match);
            }
            printk("\n Max Seq number: %08x",eth_vi_dbg_params.vi_rx_seq_max);
            printk("\n Debug cfg: %d \n",eth_vi_dbg_params.vi_dbg_cfg);  
#endif        

            break;
        case ATHR_DBG_RESTART: 
            printk("%s restart :\n",str);
            eth_vi_dbg_params.vi_dbg_restart = 1;
            break;
        case ATHR_DBG_STATS:
            printk("%s stats:\n",str);
            //rxdrops = (u_int32_t *)ifr->ifr_data;
            ep = (struct eth_params *)ethcfg;
            ep->val = eth_rx_drop;
            break;
        case ATHR_DBG_ENABLE:  
            printk("%s enable:\n",str);
            ep = (struct eth_params *)ethcfg;
            eth_vi_dbg_params.vi_dbg_cfg = ep->val;
            if (eth_vi_dbg_params.vi_dbg_cfg)
                eth_vi_dbg_params.vi_dbg_restart = 1;
            break;
#endif

        default:
             break;
    }
    if (copy_to_user(ifr->ifr_data, ethcfg, sizeof(struct eth_cfg_params)))
        return -EFAULT;

    return 0;
}
