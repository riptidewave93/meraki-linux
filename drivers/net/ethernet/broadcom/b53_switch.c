/*
 * Initialize BCM5301x "RoboSwitch" at gmac probe through the
 * Switch Register Access Bridge (srab)
 *
 * Copyright (C) 2018 Cisco Meraki
 */

#include <linux/kernel.h>
#include "b53_regs.h"

#define B53_ALL_PORTS		0x1bf /* ports 0-8 except 6 */

/* extra B53_MGMT_PAGE registers */
#define B53_PPORT_CTRL		0x24
#define B53_PHY_PWR		0x4a
#define   PHY_OFF(n)		BIT(n)

/* extra B53_ARLCTRL_PAGE registers */
#define B53_ARL_MULTIPORT_CTL	0xe	/* Multiport control register */
#define B53_ARL_MULTIPORT_MATCH(n)  ((0x10 * (n)) + 0x10)
#define B53_ARL_MULTIPORT_MAP(n)  ((0x10 * (n)) + 0x18)


static void b53_srab_update64(void *regs, u8 page, u8 reg, u64 mask, u64 val)
{
    u64 v = 0;
    b53_srab_read64(regs, page, reg, &v);
    v &= ~mask;
    v |= val;
    b53_srab_write64(regs, page, reg, v);
}

static void b53_robo_config_vlan(void *regs)
{
    u64 mac = 0x00180a000102;  /* 00:18:0a:00:01:02 */

    /* enable 802.1Q, vlan learn mode (pg 0x34, reg 0) */
    b53_srab_update64(regs, B53_VLAN_PAGE, B53_VLAN_CTRL0, 0,
                      VC0_VLAN_EN | VC0_VID_CHK_EN | VC0_VID_HASH_VID);

    /* enable fwd and untag vlan maps for multicast (pg 0x34, reg 1) */
    b53_srab_update64(regs, B53_VLAN_PAGE, B53_VLAN_CTRL1, 0,
                      VC1_RX_MCST_FWD_EN | VC1_RX_MCST_UNTAG_EN);

    /* initialize mac addr index (pg 5, reg 2) */
    b53_srab_write64(regs, B53_ARLIO_PAGE, B53_MAC_ADDR_IDX, mac);

    /* set port 0 default vlan 0 (pg 0x34, reg 0x10) */
    b53_srab_write64(regs, B53_VLAN_PAGE, B53_VLAN_PORT_DEF_TAG(0), 0);

    /* set port 8 default vlan 0 (pg 0x34, reg 0x20) */
    b53_srab_write64(regs, B53_VLAN_PAGE, B53_VLAN_PORT_DEF_TAG(8), 0);
}

/* addr is the arl multiport to use [0-5] */
static void b53_fwd_ethtype(void *regs, unsigned int addr, u16 etype)
{
    /* set ethertype to fwd */
    u64 v = (u64)etype << 48;
    b53_srab_write64(regs, B53_ARLCTRL_PAGE, B53_ARL_MULTIPORT_MATCH(addr), v);

    /* enable fwd to all ports */
    b53_srab_update64(regs, B53_ARLCTRL_PAGE, B53_ARL_MULTIPORT_MAP(addr), 0,
                      B53_ALL_PORTS);

    /* enable ethertype-matching fwd only */
    b53_srab_update64(regs, B53_ARLCTRL_PAGE, B53_ARL_MULTIPORT_CTL,
                      3 << (addr * 2), 1 << (addr * 2));
}

static void b53_robo_enable(void *regs)
{
    /* enable managed mode (pg 0, reg 0xb) */
    b53_srab_update64(regs, B53_CTRL_PAGE, B53_SWITCH_MODE, 0, SM_SW_FWD_MODE);

    /* override port 7 status: set link on (pg 0, reg 0x5f) */
    b53_srab_update64(regs, B53_CTRL_PAGE, B53_GMII_PORT_OVERRIDE_CTRL(7), 0,
                      GMII_PO_LINK | GMII_PO_EN | GMII_PO_SPEED_2000M |
                      GMII_PO_RX_FLOW | GMII_PO_TX_FLOW);

    /* override cpu port status: set link pass (pg 0, reg 0x0e) */
    b53_srab_update64(regs, B53_CTRL_PAGE, B53_PORT_OVERRIDE_CTRL, 0,
                      PORT_OVERRIDE_LINK | PORT_OVERRIDE_EN |
                      PORT_OVERRIDE_SPEED_2000M | PORT_OVERRIDE_RX_FLOW |
                      PORT_OVERRIDE_TX_FLOW);

    /* disable BRCM header on cpu port (pg 2, reg 3) */
    b53_srab_update64(regs, B53_MGMT_PAGE, B53_BRCM_HDR, BRCM_HDR_P8_EN, 0);

    /* enable rx on cpu port (pg 0, reg 8) */
    b53_srab_update64(regs, B53_CTRL_PAGE, B53_PORT_CTRL(8), 0,
                      PORT_CTRL_RX_BCST_EN | PORT_CTRL_RX_MCST_EN |
                      PORT_CTRL_RX_UCST_EN);

    /* enable cpu port (pg 2, reg 0) */
    b53_srab_update64(regs, B53_MGMT_PAGE, B53_GLOBAL_CONFIG, 0,
                      GC_FRM_MGMT_PORT_MII);

    /* unprotect all ports (pg 0, reg 0x24) */
    b53_srab_update64(regs, B53_MGMT_PAGE, B53_PPORT_CTRL, B53_ALL_PORTS, 0);

    /* power-down PHYs 2-4 (pg 0, reg 0x4a) */
    b53_srab_update64(regs, B53_MGMT_PAGE, B53_PHY_PWR, 0,
                      PHY_OFF(2) | PHY_OFF(3) | PHY_OFF(4));
}

static void b53_reset_mib_counters(void *regs)
{
    /* set then clear mib reset bit (pg 2, reg 0x0) */
    u64 v = 0;
    b53_srab_read64(regs, B53_MGMT_PAGE, B53_GLOBAL_CONFIG, &v);
    v |= GC_RESET_MIB;
    b53_srab_write64(regs, B53_MGMT_PAGE, B53_GLOBAL_CONFIG, v);
    v &= ~GC_RESET_MIB;
    b53_srab_write64(regs, B53_MGMT_PAGE, B53_GLOBAL_CONFIG, v);
}

int bcm5301x_robo_probe(void *srab)
{
    b53_robo_config_vlan(srab);
    b53_robo_enable(srab);

    /* enable LLDP forwarding */
    b53_fwd_ethtype(srab, 0, 0x88cc);
    /* enable EAP forwarding */
    b53_fwd_ethtype(srab, 0, 0x888e);

    b53_reset_mib_counters(srab);
    return 0;
}
