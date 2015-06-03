/*
 * Copyright (c) 2010, Atheros Communications Inc.
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

#include "athrs_phy.h"
#include "athrs_mac.h"

int
athrs_vir_phy_setup(void  *arg)
{
    return 0;
}

int
athrs_vir_phy_speed(int ethUnit,int phyUnit)
{
    return ATHR_PHY_SPEED_1000T;
}

/******************************************************************************
*
* athrs_vir_phy_is_fdx - Determines whether the phy ports associated with the
* specified device are FULL or HALF duplex.
*
* RETURNS:
*    1 --> FULL
*    0 --> HALF
*/
int
athrs_vir_phy_is_fdx(int ethUnit,int phyUnit)
{
    return 1;
}

/******************************************************************************
*
* athr_vir_phy_is_link_alive - test to see if the specified link is alive
*
* RETURNS:
*    TRUE  --> link is alive
*    FALSE --> link is down
*/
int
athrs_vir_phy_is_link_alive(int phyUnit)
{
    return 1;
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
athrs_vir_phy_is_up(int ethUnit)
{
   return 1;
}

static int 
athrs_vir_reg_init(void *arg)
{
   return 0;
}

int athrs_vir_phy_register_ops(void *arg)
{
  athr_gmac_t *mac   = (athr_gmac_t *) arg;
  athr_phy_ops_t *ops = mac->phy;

  if(!ops)
     ops = kmalloc(sizeof(athr_phy_ops_t), GFP_KERNEL);

  memset(ops,0,sizeof(athr_phy_ops_t));

  ops->mac            =  mac;
  ops->is_up          =  athrs_vir_phy_is_up;
  ops->is_alive       =  athrs_vir_phy_is_link_alive;
  ops->speed          =  athrs_vir_phy_speed;
  ops->is_fdx         =  athrs_vir_phy_is_fdx;
  ops->ioctl          =  NULL;
  ops->setup          =  athrs_vir_phy_setup;
  ops->stab_wr        =  NULL;
  ops->link_isr       =  NULL;
  ops->en_link_intrs  =  NULL;
  ops->dis_link_intrs =  NULL;
  ops->read_phy_reg   =  NULL;
  ops->write_phy_reg  =  NULL;
  ops->read_mac_reg   =  NULL;
  ops->write_mac_reg  =  NULL;
  ops->init           =  athrs_vir_reg_init;

  mac->phy = ops;
  return 0;
}
