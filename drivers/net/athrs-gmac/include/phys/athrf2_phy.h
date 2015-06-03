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

#ifndef _ATHRF2_PHY_H
#define _ATHRF2_PHY_H
#define BOOL int

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
#define ATHR_ADVERTISE_1000FULL              0x0200
#define ATHR_ADVERTISE_1000HALF		     0x0100

/* Phy Specific status fields */
#define ATHER_STATUS_LINK_MASK               0xC000
#define ATHER_STATUS_LINK_SHIFT              14
#define ATHER_STATUS_FULL_DUPLEX             0x2000
#define ATHR_STATUS_LINK_PASS                0x0400 
#define ATHR_LATCH_LINK_PASS                 0x0004 
#define ATHR_STATUS_RESOVLED                 0x0800

/*phy debug port  register */
#define ATHER_DEBUG_SERDES_REG               5

/* Serdes debug fields */
#define ATHER_SERDES_BEACON                  0x0100

#define OPERATIONAL_MODE_REG0                0x4

#define PHY_LINK_CHANGE_REG 		     0x4
#define PHY_LINK_UP 		             0x400
#define PHY_LINK_DOWN 		             0x800
#define PHY_LINK_DUPLEX_CHANGE 		     0x2000
#define PHY_LINK_SPEED_CHANGE		     0x4000
#define PHY_LINK_INTRS			     (PHY_LINK_UP | PHY_LINK_DOWN | PHY_LINK_DUPLEX_CHANGE | PHY_LINK_SPEED_CHANGE)


int athr_phy_is_up(int unit);
int athr_phy_is_fdx(int unit,int phyUnit);
int athr_phy_speed(int unit,int phyUnit);
BOOL athr_phy_setup(void  *arg);
BOOL athr_phy_is_link_alive(int phyUnit);
int athrsf2_register_ops(void *arg);


#endif
