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
//#define MARVEL_FORCE_10 1

extern void athr_dcm_reset_seq(void);
void
athrs_marvel_mgmt_init(void)
{
    uint32_t rddata;

    rddata = athr_reg_rd(0xb8040050);
    rddata =  rddata & (~(0xff<<16));
    rddata = (rddata | (0x12<<16)); //GPIO 18-MDIO ,GPIO 7 -MDC
    athr_reg_wr(0xb8040050,rddata);
    
    rddata = athr_reg_rd(0xb8040000);
    rddata = (rddata &  ~(1<<7) & ~(1<<18)); //Output emable for mdc
    athr_reg_wr(0xb8040000,rddata);
    
    rddata = athr_reg_rd(0xb804003c);  //mdio_out _en for gpio mux
    rddata =  rddata & (~(0xff<<16));
    rddata = rddata | (0x20<<16) ;//GPIO 18-MDIO ,GPIO 7 -MDC
    athr_reg_wr(0xb804003c,rddata);
    
    rddata = athr_reg_rd(0xb8040030);  //mdc_out _en for gpio mux
    rddata =  rddata & (~(0xff<<24));
    rddata =  (0x21<<24) ;//GPIO 18-MDIO ,GPIO 7 -MDC
    athr_reg_wr(0xb8040030,rddata);

    printk("%s:Done.\n",__func__);
    
}

int
athrs_marvel_phy_setup(void  *arg)
{

    if (is_emu()) {
        athr_dcm_reset_seq();
#if 0
        athr_reg_wr(0xb804006c, 0x2);
        rddata = athr_reg_rd(0xb8040030);
        athr_reg_wr(0xb8040030, (rddata & ~(0xff<<16)) );

        rddata = athr_reg_rd(0xb8040000);

        athr_reg_wr(0xb8040000, (rddata | 0x4) );

        rddata = athr_reg_rd(0xb8040008);

        athr_reg_wr(0xb8040008, rddata|0x4);

        udelay(1000);

        athr_reg_wr(0xb8040008,rddata & ~(0x4));

        udelay(1000);

        printk("dcm reset done\n");
#endif

    }
    
    return 0;
}

int
athrs_marvel_phy_speed(int ethUnit,int phyUnit)
{
   int phy_hw_status = 0x0,speed;

   phy_hw_status = athr_reg_rd(0xb8070038);

   speed = ((phy_hw_status & (3 << 10)) >> 10);

   switch (speed) {
	case 0:
		return ATHR_PHY_SPEED_10T;
		break;
	case 1:
		return ATHR_PHY_SPEED_100T;
		break;
	case 2:
		return ATHR_PHY_SPEED_1000T;
		break;
	case 3:
	default:
		return ATHR_PHY_SPEED_UNKNOWN;
		break;
   }
		
   return ATHR_PHY_SPEED_UNKNOWN;
 
}

/******************************************************************************
*
* athrs_marvel_phy_is_fdx - Determines whether the phy ports associated with the
* specified device are FULL or HALF duplex.
*
* RETURNS:
*    1 --> FULL
*    0 --> HALF
*/
int
athrs_marvel_phy_is_fdx(int ethUnit,int phyUnit)
{
   int phy_hw_status = 0x0;

   phy_hw_status = athr_reg_rd(0xb8070038);

   if (phy_hw_status & (1 << 12))
       return 1;
   else
       return 0;
}

/******************************************************************************
*
* athr_marvel_phy_is_link_alive - test to see if the specified link is alive
*
* RETURNS:
*    TRUE  --> link is alive
*    FALSE --> link is down
*/
int
athrs_marvel_phy_is_link_alive(int phyUnit)
{
   int phy_hw_status = 0x0;

   phy_hw_status = athr_reg_rd(0xb8070038);

   if (phy_hw_status & (1 << 15))
       return 1;
   else
      return 0;
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
athrs_marvel_phy_is_up(int ethUnit)
{
   int phy_hw_status = 0x0;

   phy_hw_status = athr_reg_rd(0xb8070038);

   if (phy_hw_status & (1 << 15))
       return 1;
   else
      return 0;
}

int 
athrs_marvel_reg_init(void *arg)
{


  athrs_marvel_mgmt_init();

  phy_reg_write(0x1,0x0,0x16,0x1);
  phy_reg_write(0x1,0x0,0x1b,0x4);
  phy_reg_write(0x1,0x0,0x19,0xc000);

#ifdef MARVEL_FORCE_10
  printk ("To advertise only 10Mpbps\n");
  phy_reg_write(0x1,0x0, 0x16,0x0);
  phy_reg_write(0x1,0x0, 0x4, 0x0040 |0x0020);
// To write 0 to 1000baset control
 // phy_reg_write(0x1,0x0, 0x9, 0x0);
//  phy_reg_write(0x1,0x0, 0x0, 0x9000 );
#endif


  printk("%s: Done\n",__func__);
   
  return 0;
}

unsigned int 
athrs_marvel_phy_read(int ethUnit, unsigned int phy_addr, unsigned int reg_addr)
{
  
  return phy_reg_read(0x1,0x0,reg_addr);
 
}

void 
athrs_marvel_phy_write(int ethUnit,unsigned int phy_addr, unsigned int reg_addr, unsigned int write_data)
{
    phy_reg_write(0x1,0x0,reg_addr,write_data);

}
int athrs_marvel_phy_register_ops(void *arg)
{
  athr_gmac_t *mac   = (athr_gmac_t *) arg;
  athr_phy_ops_t *ops = mac->phy;

  if(!ops)
     ops = kmalloc(sizeof(athr_phy_ops_t), GFP_KERNEL);

  memset(ops,0,sizeof(athr_phy_ops_t));

  ops->mac            =  mac;
  ops->is_up          =  athrs_marvel_phy_is_up;
  ops->is_alive       =  athrs_marvel_phy_is_link_alive;
  ops->speed          =  athrs_marvel_phy_speed;
  ops->is_fdx         =  athrs_marvel_phy_is_fdx;
  ops->ioctl          =  NULL;
  ops->setup          =  athrs_marvel_phy_setup;
  ops->stab_wr        =  NULL;
  ops->link_isr       =  NULL;
  ops->en_link_intrs  =  NULL;
  ops->dis_link_intrs =  NULL;
  ops->read_phy_reg   =  athrs_marvel_phy_read;
  ops->write_phy_reg  =  athrs_marvel_phy_write;
  ops->read_mac_reg   =  NULL;
  ops->write_mac_reg  =  NULL;
  ops->init           =  athrs_marvel_reg_init;

  mac->phy = ops;
  return 0;
}
