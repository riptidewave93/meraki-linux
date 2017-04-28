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

#ifndef _ATHRS26_PHY_H
#define _ATHRS26_PHY_H
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
#define ATHR_ADVERTISE_1000HALF		      0x0100

/* Phy Specific status fields */
#define ATHER_STATUS_LINK_MASK                0xC000
#define ATHER_STATUS_LINK_SHIFT               14
#define ATHER_STATUS_FULL_DUPLEX              0x2000
#define ATHR_STATUS_LINK_PASS                 0x0400 
#define ATHR_LATCH_LINK_PASS                  0x0004 
#define ATHR_STATUS_RESOVLED                  0x0800

/*phy debug port  register */
#define ATHER_DEBUG_SERDES_REG                5

/* Serdes debug fields */
#define ATHER_SERDES_BEACON                   0x0100

#define S26_GLOBAL_INTR_REG                  0x10
#define S26_GLOBAL_INTR_MASK_REG             0x14


#define S26_ENABLE_CPU_BROADCAST             (1 << 26)

#define PHY_LINK_CHANGE_REG 		     0x4
#define PHY_LINK_UP 		             0x400
#define PHY_LINK_DOWN 		             0x800
#define PHY_LINK_DUPLEX_CHANGE 		     0x2000
#define PHY_LINK_SPEED_CHANGE		     0x4000
#define PHY_LINK_INTRS			     (PHY_LINK_UP | PHY_LINK_DOWN | PHY_LINK_DUPLEX_CHANGE | PHY_LINK_SPEED_CHANGE)

#undef S26_VER_1_0

int athrs26_reg_init(void *arg);
int athrs26_reg_init_lan(void *arg);
int athrs26_phy_is_up(int unit);
int athrs26_phy_is_fdx(int unit,int phyUnit);
int athrs26_phy_speed(int unit,int phyUnit);
void athrs26_phy_stab_wr(int phy_id, BOOL phy_up, int phy_speed);
int athrs26_phy_setup(void *arg);
int athrs26_mdc_check(void);
void athrs26_phy_off(void *arg);
void athrs26_phy_on(void *arg);
void athrs26_mac_speed_set(void *arg, int phy_speed);
int athrs26_register_ops(void *arg);
void athrs26_enable_link_intrs(void *arg);
void athrs26_disable_link_intrs(void *arg);
int athrs26_phy_is_link_alive(int phyUnit);
void athrs26_phy_stab_wr(int phy_id, int phy_up, int phy_speed);
unsigned int athrs26_reg_read(unsigned int s26_addr);
void athrs26_reg_write(unsigned int s26_addr, unsigned int s26_write_data);
void s26_wr_phy(int ethUnit,unsigned int phy_addr, unsigned int reg_addr, unsigned int write_data);
unsigned int s26_rd_phy(int ethUnit,unsigned int phy_addr, unsigned int reg_addr);
int athrs26_ioctl(struct net_device *dev,void *args, int cmd);

/*
 *  Atheros header defines
 */
#ifndef _ATH_HEADER_CONF
#define _ATH_HEADER_CONF

typedef enum {
    NORMAL_PACKET,
    RESERVED0,
    MIB_1ST,
    RESERVED1,
    RESERVED2,
    READ_WRITE_REG,
    READ_WRITE_REG_ACK,
    RESERVED3
} AT_HEADER_TYPE;

typedef struct {
    uint16_t    reserved0  :2;
    uint16_t    priority   :2;
    uint16_t    type       :4;
    uint16_t    broadcast  :1;
    uint16_t    from_cpu   :1;
    uint16_t    reserved1  :2;
    uint16_t    port_num   :4;
}at_header_t;

#define ATHR_HEADER_LEN 2

#endif // _ATH_HEADER_CONF
#endif
