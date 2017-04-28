#ifndef _ATHRS_QOPS_H
#define _ATHRS_QOPS_H

#include "athrs_ioctl.h"

#if defined(CONFIG_SOC_AR724X) || defined (CONFIG_SOC_AR934X) || defined (CONFIG_MACH_HORNET)
#define ENET_UNIT_LAN 1
#define ENET_UNIT_WAN 0

#include "athrs_mac.h"
#include "athrs_phy.h"
extern athr_gmac_t *athr_gmacs[2];

extern int set_mac_qops(void *mac);
extern uint32_t athr_fdb_add_qos(u_int8_t addr[], uint32_t prio,uint32_t portno);
extern uint32_t athr_ioctl_vlan_qos(uint32_t vlan_id, uint32_t vlan_pri, uint32_t *port_map);

static uint32_t
qos_gmac_reg_read(char *ad_name,uint32_t Reg) {

    athr_gmac_t *mac = athr_gmac_name2mac(ad_name);
    athr_phy_ops_t *phy = mac->phy;

    return (phy->read_mac_reg(Reg));

}

static void
qos_gmac_reg_write(char *ad_name,uint32_t Reg, uint32_t Val) 
{

    athr_gmac_t *mac = athr_gmac_name2mac(ad_name);
    athr_phy_ops_t *phy = mac->phy;

    printk(KERN_DEBUG "[%x] QoS Reg write %x=%x\n",mac->mac_unit,Reg,Val);

    phy->write_mac_reg(Reg,Val);
}

static void
qos_gmac_reg_rmw_set(char *ad_name,uint32_t Reg, uint32_t Val) 
{
    int tval = qos_gmac_reg_read(ad_name,Reg);

    qos_gmac_reg_write(ad_name,Reg,(tval | Val));
}

static void
qos_gmac_reg_rmw_clear(char *ad_name,uint32_t Reg, uint32_t Val) 
{
    int tval = qos_gmac_reg_read(ad_name,Reg);

    qos_gmac_reg_write(ad_name,Reg,(tval & ~Val));
}

static int 
qos_gmac_enable_mac_qos(char *ad_name) {

    athr_gmac_t *mac = athr_gmac_name2mac(ad_name);
    struct net_device *dev = mac->mac_dev;
    int val = 0;

#if (LINUX_VERSION_CODE >= KERNEL_VERSION(2,6,31))
    dev->netdev_ops->ndo_stop(dev);
#else
    dev->stop(dev);
#endif
    if(mac_has_flag(mac,ETH_SWONLY_MODE))
        return -1;

    if (mac->mac_unit == 1 ||
            (is_s16() && mac->mac_unit == 0)) {
        mac_set_flag(mac,ATHR_SWITCH_QOS);
        val = qos_gmac_reg_read(ad_name, ATHR_CTRL_PORT_0);
        if( (val & ATHR_HDR_EN) == 0){
            qos_gmac_reg_rmw_set(ad_name,ATHR_CTRL_PORT_0, ATHR_HDR_EN);
            mac->qos->qos_flag |= 0x1 << 0;
        } 

        val = qos_gmac_reg_read(ad_name, ATHR_CPU_PORT); 
        if( (val & ATHR_CPU_EN) == 0){
            qos_gmac_reg_rmw_set(ad_name,ATHR_CPU_PORT, ATHR_CPU_EN);
            mac->qos->qos_flag |= 0x1 << 1;
        } 
        qos_gmac_reg_rmw_set(ad_name,ATHR_QOS_MODE_REGISTER,ATHR_QOS_WEIGHTED);

#ifndef CONFIG_ATHEROS_HEADER_EN
        mac_set_flag(mac,ATHR_HEADER_ENABLED);

        if (is_s16())
            mac_set_flag(mac,ATHR_S16_HEADER);
        else if (is_s26())
            mac_set_flag(mac,ATHR_S26_HEADER);
        else if (is_s27())
            mac_set_flag(mac,ATHR_S27_HEADER);
#endif
        for (val = 1; val <= 4; val++){
            qos_gmac_reg_rmw_clear(ad_name,ATHR_PRI_CTRL_PORT_0 +
              (val * 256) , (ATHR_DA_PRI_EN|ATHR_TOS_PRI_EN|ATHR_VLAN_PRI_EN|ATHR_PORT_PRI_EN));
        }
    }
    else {
     /* WAN MAC setting */
        mac_set_flag(mac,WAN_QOS_SOFT_CLASS);
        qos_gmac_reg_rmw_clear(ad_name,ATHR_PRI_CTRL_PORT_0 +
               (5 * 256) , (ATHR_DA_PRI_EN|ATHR_TOS_PRI_EN|ATHR_VLAN_PRI_EN|ATHR_PORT_PRI_EN));
    }

    mac->qos->qos_cap |= (1 << mac->mac_unit);

#if (LINUX_VERSION_CODE >= KERNEL_VERSION(2,6,31))
    dev->netdev_ops->ndo_open(dev);
#else
    dev->open(dev);
#endif
    return 0;
}

static void 
qos_gmac_disable_mac_qos(char *ad_name) {

    athr_gmac_t *mac = athr_gmac_name2mac(ad_name);
    struct net_device *dev = mac->mac_dev;
    int val = 0;

#if (LINUX_VERSION_CODE >= KERNEL_VERSION(2,6,31))
    dev->netdev_ops->ndo_stop(dev);
#else
    dev->stop(dev);
#endif
    
    if (mac->mac_unit == 1) {
        if(mac->qos->qos_flag != 0){
            if( (0x1 & mac->qos->qos_flag) != 0)
                qos_gmac_reg_rmw_clear(ad_name,ATHR_CTRL_PORT_0 ,ATHR_HDR_EN);
            if( ( (0x1 << 1) & mac->qos->qos_flag) != 0)
                qos_gmac_reg_rmw_clear(ad_name,ATHR_CPU_PORT, ATHR_CPU_EN);
        }
        qos_gmac_reg_rmw_clear(ad_name,ATHR_QOS_MODE_REGISTER,ATHR_QOS_WEIGHTED);

#ifndef CONFIG_ATHEROS_HEADER_EN
        if (is_s16())
            mac_clear_flag(mac,ATHR_S16_HEADER);
        else if (is_s26())
            mac_clear_flag(mac,ATHR_S26_HEADER);
        else if (is_s27())
            mac_clear_flag(mac,ATHR_S27_HEADER);
#endif
        for (val = 1; val <= 4; val++){
            qos_gmac_reg_rmw_clear(ad_name,ATHR_PRI_CTRL_PORT_0 +
                 (val * 256) , (ATHR_DA_PRI_EN|ATHR_TOS_PRI_EN|ATHR_VLAN_PRI_EN|ATHR_PORT_PRI_EN));
        }
    }
    else {
        /* WAN MAC setting */
        mac_clear_flag(mac,WAN_QOS_SOFT_CLASS);
        qos_gmac_reg_rmw_clear(ad_name,ATHR_PRI_CTRL_PORT_0 +
                 (5 * 256) , (ATHR_DA_PRI_EN|ATHR_TOS_PRI_EN|ATHR_VLAN_PRI_EN|ATHR_PORT_PRI_EN));

    }
    mac->qos->qos_cap &= ~(1 << mac->mac_unit);
    mac->qos->qos_flag = 0;

#if (LINUX_VERSION_CODE >= KERNEL_VERSION(2,6,31))
    dev->netdev_ops->ndo_open(dev);
#else
    dev->open(dev);
#endif
}

struct ath_qops ath_qos_ops = {
    qos_gmac_reg_read,
    qos_gmac_reg_write,
    qos_gmac_reg_rmw_set,
    qos_gmac_reg_rmw_clear,
    qos_gmac_enable_mac_qos,
    qos_gmac_disable_mac_qos,
    0,
};

static inline int
get_unit(char *ad_name)
{
    athr_gmac_t *mac = athr_gmac_name2mac(ad_name);

    return (mac->mac_unit);
}

static inline struct ath_qops
*get_qmac(char *ad_name)
{
    athr_gmac_t *mac = athr_gmac_name2mac(ad_name);

    if (mac_has_flag(mac,ETH_SWONLY_MODE))
        mac = athr_gmac_unit2mac(1);

    return (mac->qos);
}

static inline  struct net_device 
*get_ndev(char *ad_name)
{
    athr_gmac_t *mac = athr_gmac_name2mac(ad_name);

    if (mac_has_flag(mac,ETH_SWONLY_MODE))
        mac = athr_gmac_unit2mac(1);

    return (mac->mac_dev);
}

 
#endif
#endif  //_ATHRS_QOS_H
