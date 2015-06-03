#ifndef _ATHRS16_PHY_H
#define _ATHRS16_PHY_H
#include "athrs_phy_ctrl.h"

#define ATHR_RESET_DONE(phy_control)                   \
    (((phy_control) & (ATHR_CTRL_SOFTWARE_RESET)) == 0)
    
/* Phy status fields */
#define ATHR_STATUS_AUTO_NEG_DONE                   0x0020

#define ATHR_AUTONEG_DONE(ip_phy_status)                   \
    (((ip_phy_status) &                                  \
        (ATHR_STATUS_AUTO_NEG_DONE)) ==                    \
        (ATHR_STATUS_AUTO_NEG_DONE))

/* Link Partner ability */
#define ATHR_LINK_100BASETX_FULL_DUPLEX       0x0100
#define ATHR_LINK_100BASETX                   0x0080
#define ATHR_LINK_10BASETX_FULL_DUPLEX        0x0040
#define ATHR_LINK_10BASETX                    0x0020

/* Advertisement register. */
#define ATHR_ADVERTISE_NEXT_PAGE              0x8000
#define ATHR_ADVERTISE_ASYM_PAUSE             0x0800
#define ATHR_ADVERTISE_PAUSE                  0x0400
#define ATHR_ADVERTISE_100FULL                0x0100
#define ATHR_ADVERTISE_100HALF                0x0080  
#define ATHR_ADVERTISE_10FULL                 0x0040  
#define ATHR_ADVERTISE_10HALF                 0x0020  

#define ATHR_ADVERTISE_ALL (ATHR_ADVERTISE_ASYM_PAUSE | ATHR_ADVERTISE_PAUSE | \
                            ATHR_ADVERTISE_10HALF | ATHR_ADVERTISE_10FULL | \
                            ATHR_ADVERTISE_100HALF | ATHR_ADVERTISE_100FULL)
                       
/* 1000BASET_CONTROL */
#define ATHR_ADVERTISE_1000FULL               0x0200

/* Phy Specific status fields */
#define ATHER_STATUS_LINK_MASK                0xC000
#define ATHER_STATUS_LINK_SHIFT               14
#define ATHER_STATUS_FULL_DEPLEX              0x2000
#define ATHR_STATUS_LINK_PASS                 0x0400 
#define ATHR_STATUS_RESOVLED                  0x0800

/*phy debug port  register */
#define ATHER_DEBUG_SERDES_REG                5

/* Serdes debug fields */
#define ATHER_SERDES_BEACON                   0x0100

/* S16 CSR Registers */

#define S16_CPU_PORT_REGISTER                 0x0078
#define S16_MDIO_CTRL_REGISTER                0x0098

#define S16_GLOBAL_INTR_REG                   0x10
#define S16_GLOBAL_INTR_MASK_REG              0x14
#define S16_PWR_ON_STRAP_REG                  0x8


#define S16_ENABLE_CPU_BROADCAST              (1 << 26)

#define PHY_LINK_CHANGE_REG 	              0x4
#define PHY_LINK_UP 		              0x400
#define PHY_LINK_DOWN 		              0x800
#define PHY_LINK_DUPLEX_CHANGE 	              0x2000
#define PHY_LINK_SPEED_CHANGE	              0x4000
#define PHY_LINK_INTRS		              (PHY_LINK_UP | PHY_LINK_DOWN | PHY_LINK_DUPLEX_CHANGE | PHY_LINK_SPEED_CHANGE)

/*add feature define here*/
//#define FULL_FEATURE

#ifdef CONFIG_ATHRS16_PHY
#undef HEADER_REG_CONF
#undef HEADER_EN
#endif

int athrs16_reg_init(void *arg);
int athrs16_phy_is_up(int ethUnit);
int athrs16_phy_is_fdx(int ethUnit, int phyUnit);
int athrs16_phy_speed(int ethUnit, int phyUnit);
int athrs16_phy_is_link_alive(int phyUnit);
int athrs16_phy_setup(void *arg);
unsigned int athrs16_reg_read(unsigned int reg_addr);
void athrs16_reg_write(unsigned int reg_addr, unsigned int reg_val);
int athrs16_ioctl(struct net_device *dev, void *args, int cmd);

int athrs16_register_ops(void *arg);
int athrs16_in_reset(void *arg);
#endif


