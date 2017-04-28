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

#include <linux/version.h>
#if (LINUX_VERSION_CODE >= KERNEL_VERSION(2,6,31))
#include <linux/autoconf.h>
#else
#include <linux/config.h>
#endif
#include <linux/types.h>
#include <linux/netdevice.h>
#include <linux/etherdevice.h>
#include <linux/delay.h>
#include <linux/if_pppox.h>
#include <linux/igmp.h>
#include <linux/ip.h>

#include "athrs_mac.h"
#include "athrs_phy.h"
#include "athrs_ioctl.h"

#define TRUE    1
#define FALSE   0

#define ATHR_OK   0
#define ATHR_ERR  1
#define ATHR_FULL 2

#define ATHR_MAX_PORT_ID 5 /* lan ports and cpu port, exclude cpu port */
#define ATHR_MAX_PORTS_ID 31 /* The maxsize is 0x11111 */

#define ATHR_PORT_ID_CHECK(port_id) \
do { \
    if (ATHR_MAX_PORT_ID < port_id) \
        return ATHR_ERR; \
} while (0)

#define ATHR_PORTS_ID_CHECK(port_id) \
do { \
    if (ATHR_MAX_PORTS_ID < port_id) \
        return ATHR_ERR; \
} while (0)


#define PORT_CONTROL_REG_ADDR(port_id) (0x104 + 0x100 * port_id)

athr_phy_ops_t *phy;
extern int athr_gmac_get_link_st(athr_gmac_t *mac, int *link, int *fdx,
                athr_phy_speed_t *speed,int phyUnit);

uint32_t
athr_port_igmps_status_set(uint32_t port_id, BOOL enable)
{
    uint32_t data;
    uint32_t addr;

    ATHR_PORT_ID_CHECK(port_id);

    addr = PORT_CONTROL_REG_ADDR(port_id);

    data = phy->read_mac_reg(addr);
    if (TRUE == enable) {
        data |= (0x1 << 10);
    } else if (FALSE == enable) {
        data &= (~(0x1 << 10));
    } else {
        return ATHR_ERR;
    }
    phy->write_mac_reg(addr, data);

    return ATHR_OK;
}




#define ATHR_VLAN_ID_CHECK(vlan_id) \
do { \
    if ((4095 < vlan_id) || (0 > vlan_id)) \
        return ATHR_ERR; \
} while (0)

#define VLAN_LOAD_ENTRY     2
#define VLAN_PURGE_ENTRY    3
#define VLAN_FIND_ENTRY     6

#define ATHR_VTF0_ADDR    0x40
#define ATHR_VTF1_ADDR    0x44

static uint32_t
_athr_vlan_commit(uint32_t op)
{
    uint32_t vt_busy = 1, i = 0x1000, vt_full, reg0;

    reg0  = phy->read_mac_reg(ATHR_VTF0_ADDR);
    reg0 |= ((0x7 & op) | 0x8);
    phy->write_mac_reg(ATHR_VTF0_ADDR, reg0);

    while (vt_busy && --i) {
        reg0    = phy->read_mac_reg(ATHR_VTF0_ADDR);
        vt_busy = reg0 & 0x8;
        udelay(5);
    }

    /* hardware can't complete the operation */
    if (0 == i) {
        return ATHR_ERR;
    }

    vt_full = reg0 & 0x10;
    if (vt_full) {
        /* write one to clear full voilation bit */
        reg0 = 0x10;
        phy->write_mac_reg(ATHR_VTF0_ADDR, reg0);
        return ATHR_FULL;
    }

    return ATHR_OK;
}

static uint32_t
_athr_vlan_read(uint32_t vlan_id, uint32_t * port_bmap)
{
    uint32_t reg0 = 0, reg1 = 0, ret;

    ATHR_VLAN_ID_CHECK(vlan_id);

    reg0 |= ((0xfff & vlan_id) << 16);
    phy->write_mac_reg(ATHR_VTF0_ADDR, reg0);

    ret = _athr_vlan_commit(VLAN_FIND_ENTRY);
    if (ATHR_OK != ret) {
        return ATHR_ERR;
    }

    reg0 = phy->read_mac_reg(ATHR_VTF0_ADDR);
    reg1 = phy->read_mac_reg(ATHR_VTF1_ADDR);

    if (reg1 & 0x800) {
        *port_bmap = reg1 & 0x3f;
        return ATHR_OK;
    } else {
        return ATHR_ERR;
    }
}

uint32_t
athr_vlan_create(uint32_t vlan_id)
{
    uint32_t reg0 = 0, reg1 = 0;
    uint32_t ret;

    ATHR_VLAN_ID_CHECK(vlan_id);

    reg0 |= (vlan_id << 16);
    reg1 |= (0x1 << 11);
    phy->write_mac_reg(ATHR_VTF0_ADDR, reg0);
    phy->write_mac_reg(ATHR_VTF1_ADDR, reg1);

    ret = _athr_vlan_commit(VLAN_LOAD_ENTRY);
    if (ATHR_OK != ret) {
        return ATHR_ERR;
    }

    return ATHR_OK;
}

uint32_t
athr_vlan_delete(uint32_t vlan_id)
{
    uint32_t reg0 = 0;
    uint32_t ret;

    ATHR_VLAN_ID_CHECK(vlan_id);

    reg0 |= (vlan_id << 16);
    phy->write_mac_reg(ATHR_VTF0_ADDR, reg0);

    ret = _athr_vlan_commit(VLAN_PURGE_ENTRY);
    if (ATHR_OK != ret) {
        return ATHR_ERR;
    }

    return ATHR_OK;
}

uint32_t
athr_vlan_query(uint32_t vlan_id, uint32_t * port_bmap)
{
    ATHR_VLAN_ID_CHECK(vlan_id);

    if (!port_bmap) {
        return ATHR_ERR;
    }

    return _athr_vlan_read(vlan_id, port_bmap);
}

uint32_t
athr_vlan_member_add(uint32_t vlan_id, uint32_t port_id)
{
    uint32_t port_bmap, reg0 = 0, reg1 = 0;
    uint32_t ret;

    ATHR_VLAN_ID_CHECK(vlan_id);
    ATHR_PORT_ID_CHECK(port_id);

    ret = _athr_vlan_read(vlan_id, &port_bmap);
    if (ATHR_OK != ret) {
        /* the vlan entry has not been created */
        return ATHR_ERR;
    }

    port_bmap |= (0x1 << port_id);
    reg0 |= (vlan_id << 16);
    reg1 |= ((0x1 << 11) | port_bmap);
    phy->write_mac_reg(ATHR_VTF0_ADDR, reg0);
    phy->write_mac_reg(ATHR_VTF1_ADDR, reg1);

    ret = _athr_vlan_commit(VLAN_LOAD_ENTRY);
    if (ATHR_FULL == ret) {
        /* hardware will set full bit when update vlan entry */
        return ATHR_OK;
    }

    return ret;
}

uint32_t
athr_vlan_member_del(uint32_t vlan_id, uint32_t port_id)
{
    uint32_t port_bmap, reg0 = 0, reg1 = 0;
    uint32_t ret;

    ATHR_VLAN_ID_CHECK(vlan_id);
    ATHR_PORT_ID_CHECK(port_id);

    ret = _athr_vlan_read(vlan_id, &port_bmap);
    if (ATHR_OK != ret) {
        /* the vlan entry has not been created */
        return ATHR_ERR;
    }

    port_bmap &= (~(0x1 << port_id));
    reg0 |= (vlan_id << 16);
    reg1 |= ((0x1 << 11) | port_bmap);
    phy->write_mac_reg(ATHR_VTF0_ADDR, reg0);
    phy->write_mac_reg(ATHR_VTF1_ADDR, reg1);

    ret = _athr_vlan_commit(VLAN_LOAD_ENTRY);
    if (ATHR_FULL == ret) {
        /* hardware will set full bit when update vlan entry */
        return ATHR_OK;
    }

    return ret;
}

#define PORT_BASE_VLAN_ADDR(port_id) (0x108 + 0x100 * port_id)

uint32_t
athr_port_default_vid_set(uint32_t port_id, uint32_t vlan_id)
{
    uint32_t addr, data;

    ATHR_PORT_ID_CHECK(port_id);
    ATHR_VLAN_ID_CHECK(vlan_id);

    addr = PORT_BASE_VLAN_ADDR(port_id);
    data = phy->read_mac_reg(addr);
    data &= 0xfffff000;
    data |= vlan_id;
    phy->write_mac_reg(addr, data);

    return ATHR_OK;
}


uint32_t
athr_port_egvlanmode_set(uint32_t port_id, port_1q_egmode_t mode)
{
    uint32_t addr, data;

    ATHR_PORT_ID_CHECK(port_id);

    addr = PORT_CONTROL_REG_ADDR(port_id);

    data = phy->read_mac_reg(addr);
    data &= 0xfffffcff;
    if (PORT_EG_UNMODIFIED == mode) {
        /* need't set value */
    } else if (PORT_EG_UNTAGGED == mode) {
        data |= (0x1 << 8);
    } else if (PORT_EG_TAGGED == mode) {
        data |= (0x2 << 8);
    } else {
        /* hardware not support value */
        return ATHR_ERR;
    }
    phy->write_mac_reg(addr, data);

    return ATHR_OK;
}

/* Don't forget to enable the ports once the driver has completed the startup.*/
uint32_t
athr_port_1qmode_set(uint32_t port_id, BOOL enable)
{
    uint32_t addr, data;

    ATHR_PORT_ID_CHECK(port_id);

    addr = PORT_BASE_VLAN_ADDR(port_id);

    data = phy->read_mac_reg(addr);
    data &= 0x3fffffff;
    if (TRUE == enable) {
        data |= (0x3 << 30);
    } else if (FALSE == enable) {
        /* needn't set value */
    } else {
        return ATHR_ERR;
    }
    phy->write_mac_reg(addr, data);

    return ATHR_OK;
}

#define ATHR_ATF0_ADDR          0x50
#define ATHR_ATF1_ADDR          0x54
#define ATHR_ATF2_ADDR          0x58

#define ARL_FLUSH_ALL             1
#define ARL_LOAD_ENTRY            2
#define ARL_PURGE_ENTRY           3
#define ARL_FLUSH_ALL_UNLOCK      4
#define ARL_LOOKUP_ENTRY          7

static uint32_t
_athr_fdb_commit(uint32_t op)
{
    uint32_t busy = 1, i = 0x1000;
    uint32_t reg0, full_vio;

    reg0 = phy->read_mac_reg(ATHR_ATF0_ADDR);
    reg0 |= ((op & 0x7) | 0x8);
    phy->write_mac_reg(ATHR_ATF0_ADDR, reg0);

    while (busy && --i) {
        reg0 = phy->read_mac_reg(ATHR_ATF0_ADDR);
        busy = reg0 & 0x8;
        udelay(5);
    }

    if (0 == i) {
        return ATHR_ERR;
    }

    full_vio = reg0 & 0x1000; 
    if (full_vio) {
        /* must clear AT_FULL_VOI bit */
        reg0 = 0x1000;
        phy->write_mac_reg(ATHR_ATF0_ADDR, reg0);

        if (ARL_LOAD_ENTRY == op) {
            return ATHR_FULL;
        } else {
            return ATHR_ERR;
        }
    }

    return ATHR_OK;
}

static void
_athr_fdb_fill_addr(mac_addr_t addr, uint32_t * reg0, uint32_t * reg1)
{
    *reg1 |= (addr.uc[0] << 24);
    *reg1 |= (addr.uc[1] << 16);
    *reg1 |= (addr.uc[2] << 8);
    *reg1 |= addr.uc[3];

    *reg0 |= (addr.uc[4] << 24);
    *reg0 |= (addr.uc[5] << 16);
}

static BOOL
_athr_fdb_is_zeroaddr(mac_addr_t addr)
{
    uint32_t i;

    for (i = 0; i < 6; i++) {
        if (addr.uc[i]) {
            return FALSE;
        }
    }

    return TRUE;
}

uint32_t
athr_fdb_add(mac_addr_t addr, uint32_t port_bmap, BOOL sa_drop)
{
    uint32_t reg0 = 0, reg1 = 0, reg2 = 0;

    if (TRUE == _athr_fdb_is_zeroaddr(addr)) {
        return ATHR_ERR;
    }

    _athr_fdb_fill_addr(addr, &reg0, &reg1);

    reg2 |= (0xf << 16);
    if (TRUE == sa_drop) {
        reg2 |= (0x1 << 14);
    }
    reg2 |= (port_bmap & 0x3f);

    phy->write_mac_reg(ATHR_ATF0_ADDR, reg0);
    phy->write_mac_reg(ATHR_ATF1_ADDR, reg1);
    phy->write_mac_reg(ATHR_ATF2_ADDR, reg2);

    return _athr_fdb_commit(ARL_LOAD_ENTRY);
}

uint32_t 
athr_fdb_add_qos(u_int8_t addr[], uint32_t prio,uint32_t portno)
{
    uint32_t reg0 = 0, reg1 = 0, reg2 = 0;

    reg1 |= (addr[0] << 24);
    reg1 |= (addr[1] << 16);
    reg1 |= (addr[2] << 8);
    reg1 |= addr[3];

    reg0 |= (addr[4] << 24);
    reg0 |= (addr[5] << 16);

    reg2 |= (0xf << 16);
    reg2 |= (0x1 << 12);
    reg2 |= (prio << 10);
    reg2 |= ((1 << portno) & 0x3f);
    printk("reg0 %x, reg1 %x, reg2 %x\n", reg0, reg1, reg2);
    phy->write_mac_reg(ATHR_ATF0_ADDR, reg0);
    phy->write_mac_reg(ATHR_ATF1_ADDR, reg1);
    phy->write_mac_reg(ATHR_ATF2_ADDR, reg2);

    return _athr_fdb_commit(ARL_LOAD_ENTRY);
}


uint32_t
athr_fdb_del(mac_addr_t addr)
{
    uint32_t reg0 = 0, reg1 = 0, reg2 = 0;

    if (TRUE == _athr_fdb_is_zeroaddr(addr)) {
        return ATHR_ERR;
    }

    _athr_fdb_fill_addr(addr, &reg0, &reg1);

    phy->write_mac_reg(ATHR_ATF0_ADDR, reg0);
    phy->write_mac_reg(ATHR_ATF1_ADDR, reg1);
    phy->write_mac_reg(ATHR_ATF2_ADDR, reg2);

    return _athr_fdb_commit(ARL_PURGE_ENTRY);
}

uint32_t
athr_fdb_flush(BOOL f_static)
{
    if (f_static) {
        return _athr_fdb_commit(ARL_FLUSH_ALL);
    } else {
        return _athr_fdb_commit(ARL_FLUSH_ALL_UNLOCK);
    }
}

uint32_t 
athr_fdb_getentry(mac_addr_t addr)
{
    uint32_t reg0 = 0, reg1 = 0, reg2 = 0,ret = 0;
    int drop=0,valid=0,port= -1;

    if (TRUE == _athr_fdb_is_zeroaddr(addr)) {
        printk("ret = %d .\n",ret);
        return 0;
    }

    _athr_fdb_fill_addr(addr, &reg0, &reg1);

    phy->write_mac_reg(ATHR_ATF0_ADDR, reg0);
    phy->write_mac_reg(ATHR_ATF1_ADDR, reg1);
    phy->write_mac_reg(ATHR_ATF2_ADDR, reg2);

    ret = _athr_fdb_commit(ARL_LOOKUP_ENTRY);
    
    if(ret != ATHR_OK){
        printk("ret = %d .\n",ret);
        return 0;
    }

    reg0 = phy->read_mac_reg(ATHR_ATF0_ADDR);
    reg1 = phy->read_mac_reg(ATHR_ATF1_ADDR);
    reg2 = phy->read_mac_reg(ATHR_ATF2_ADDR);

    addr.uc[0] = reg1 >> 24;
    addr.uc[1] = (reg1 >> 16)&0xff;
    addr.uc[2] = (reg1 >> 8)&0xff;
    addr.uc[3] = reg1 & 0xff;

    addr.uc[4] = reg0 >> 24;
    addr.uc[5] = (reg0 >> 16)&0xff;    

    port = reg2&0x3f;
    drop = (reg2 >> 14)&1;
    valid = (reg2 >> 16)&0xf;

    printk("Lookup reg %8x %8x %8x mac %x.%x.%x.%x.%x.%x,port %d,drop %d,valid %d.\n",reg0,reg1,reg2,addr.uc[0],addr.uc[1],addr.uc[2],
        addr.uc[3],addr.uc[4],addr.uc[5],port,drop,valid);
    
    if(reg1==0&&(reg0&0xffff0000)==0&&port==0) 
        return 0;
    else
        return 1; 
}

/*============================================================================================= 
 * The following functions are designed for the ioctl calls.
 *============================================================================================*/

/* [athr_ioctl_vlan_addports or athr_ioctl_vlan_addports] port_id: bit0--->port0,bit1--->port1.*/
uint32_t
athr_ioctl_vlan_addports(uint32_t vlan_id, uint32_t port_id)
{
    uint32_t port_bmap, reg0 = 0, reg1 = 0;
    uint32_t ret;

    ATHR_VLAN_ID_CHECK(vlan_id);
    ATHR_PORTS_ID_CHECK(port_id);

    ret = _athr_vlan_read(vlan_id, &port_bmap);
    if (ATHR_OK != ret) {
        /* the vlan entry has not been created */
        ret = athr_vlan_create(vlan_id);
        if (ATHR_OK != ret) return ATHR_ERR;
    }

    port_bmap |= port_id;
    reg0 |= (vlan_id << 16);
    reg1 |= ((0x1 << 11) | port_bmap);
    phy->write_mac_reg(ATHR_VTF0_ADDR, reg0);
    phy->write_mac_reg(ATHR_VTF1_ADDR, reg1);

    ret = _athr_vlan_commit(VLAN_LOAD_ENTRY);
    if (ATHR_FULL == ret) {
        /* hardware will set full bit when update vlan entry */
        return ATHR_OK;
    }

    return ret;

}

uint32_t
athr_ioctl_vlan_qos(uint32_t vlan_id, uint32_t vlan_pri, uint32_t *port_map)
{
    uint32_t port_bmap, reg0 = 0, reg1 = 0;
    uint32_t ret;

    ATHR_VLAN_ID_CHECK(vlan_id);

    ret = _athr_vlan_read(vlan_id, &port_bmap);
    if (ATHR_OK != ret) {
        /* the vlan entry has not been created */
        ret = athr_vlan_create(vlan_id);
        if (ATHR_OK != ret) return ATHR_ERR;
    }
    *port_map = port_bmap;
    reg0 |= (0x1 << 31) |(vlan_pri << 28)| (vlan_id << 16);
    reg1 |= ((0x1 << 11) | port_bmap);
    printk("reg0 %x, reg1 %x\n", reg0, reg1);
    phy->write_mac_reg(ATHR_VTF0_ADDR, reg0);
    phy->write_mac_reg(ATHR_VTF1_ADDR, reg1);

    ret = _athr_vlan_commit(VLAN_LOAD_ENTRY);
    if (ATHR_FULL == ret) {
        /* hardware will set full bit when update vlan entry */
        return ATHR_OK;
    }
    return ret;

}
// port_id: bit0--->port0,bit1--->port1.
uint32_t
athr_ioctl_vlan_delports(uint32_t vlan_id, uint32_t port_id)
{
    uint32_t port_bmap, reg0 = 0, reg1 = 0;
    uint32_t ret;

    ATHR_VLAN_ID_CHECK(vlan_id);
    ATHR_PORTS_ID_CHECK(port_id);

    ret = _athr_vlan_read(vlan_id, &port_bmap);
    if (ATHR_OK != ret) {
        /* the vlan entry has not been created */
        return ATHR_ERR;
    }
    
    port_bmap &= (~port_id);
    reg0 |= (vlan_id << 16);
    reg1 |= ((0x1 << 11) | port_bmap);
    phy->write_mac_reg(ATHR_VTF0_ADDR, reg0);
    phy->write_mac_reg(ATHR_VTF1_ADDR, reg1);

    ret = _athr_vlan_commit(VLAN_LOAD_ENTRY);
    if (ATHR_FULL == ret) {
        /* hardware will set full bit when update vlan entry */
        return ATHR_OK;
    }
   
    // Delete the vlan id if no ports exists in the vlan. 
    if(port_bmap == 0) 
    {
        ret = athr_vlan_delete(vlan_id);
        if (ATHR_OK != ret) return ATHR_ERR;
    }

    return ret;

}

uint32_t
athr_ioctl_set_igmp(uint32_t vlan_id, BOOL enable)
{
    uint32_t port_bmap;
    uint32_t ret;
    uint32_t i = 0;
    uint32_t port = 0;

    ATHR_VLAN_ID_CHECK(vlan_id);

    ret = _athr_vlan_read(vlan_id, &port_bmap);
    if (ATHR_OK != ret) {
        /* the vlan entry has not been created */
        return ATHR_ERR;
    }

    for(i=1;i<5;i++)
    {
        port = (port_bmap >>i)&0x1;
        if(port) 
        {   
            ret = athr_port_igmps_status_set(i,enable);
            if (ATHR_OK != ret) return ATHR_ERR;
        }
    }    
    return ATHR_OK;
}

uint32_t
athr_ioctl_enable_vlan(void)
{
    int i;
    /* Enable ports [0-4](phy0-3 and cpu port0) */
    for(i=0;i<5;i++)
        athr_port_1qmode_set(i,1);

    return ATHR_OK;
}

uint32_t
athr_ioctl_disable_vlan(void)
{
    int i;
    /* Disable ports [0-4](phy0-3 and cpu port0) */
    for(i=0;i<5;i++)
        athr_port_1qmode_set(i,0);

    return ATHR_OK;
}

void
athr_clear_multi(void)
{
#if defined(CONFIG_IPV6)
    phy->write_mac_reg(0x2c, phy->read_mac_reg(0x2c)&0xffc1ffff);
#else
    phy->write_mac_reg(0x2c, phy->read_mac_reg(0x2c)&0xffc0ffff);
#endif
}

void
athr_set_multi(void)
{
    phy->write_mac_reg(0x2c,phy->read_mac_reg(0x2c)|0x3f0000);    
}

void
athr_set_vlan_phy_ops(void *arg)
{
   athr_gmac_t *mac = arg;

   phy = mac->phy;
   assert(phy);
}

extern int ignore_packet_inspection;
void 
athr_filter_igmp_pkg(athr_gmac_t *mac, struct sk_buff *skb)
{
   /* 
     * 01:00:5e:X:X:X is a multicast MAC address.
     */
    mac_addr_t addr;

    if ((mac_has_flag(mac,ATHR_S26_HEADER) || mac_has_flag(mac,ATHR_S27_HEADER)) && (mac->mac_unit == 1))
    {
	/*default for s26*/
	int dmac_offset = 2;
	int head_offset = 0;
	if(mac_has_flag(mac,ATHR_S27_HEADER))
	{
	    dmac_offset = 0;
	    head_offset = 13;
	}
        int offset = 14;
        if((skb->data[dmac_offset]==0x33)&&(skb->data[dmac_offset+1]==0x33)){ /* multicast mac address */
            /* 
             * If vlan, calculate the offset of the vlan header. 
             */
            if((skb->data[14]==0x81)&&(skb->data[15]==0x00)){
                offset +=4;             
            }
            /* 
             * If pppoe exists, calculate the offset of the pppoe header. 
             */
            if((skb->data[offset]==0x88)&&((skb->data[offset+1]==0x63)||(skb->data[offset+1]==0x64))){
                offset += sizeof(struct pppoe_hdr);             
            }
            /* 
             * Parse IP layer.  
             */
            if((skb->data[offset]==0x86)&&(skb->data[offset+1]==0xdd)){
                offset +=2;
                offset += sizeof(struct ipv6hdr);
                
                if((skb->data[4]==0xff)&&(skb->data[offset]==0x87)){ /* Neighbor solicitation */
                    addr.uc[0] = 0x33;
                    addr.uc[1] = 0x33;
                    addr.uc[2] = 0xff;
                    addr.uc[3] = skb->data[dmac_offset+3];
                    addr.uc[4] = skb->data[dmac_offset+4];
                    addr.uc[5] = skb->data[dmac_offset+5];
                    athr_fdb_add(addr, 0x1f, 0);
                    printk("athr_mac::S26_ARL_ADD,mac:[%x.%x.%x.%x.%x.%x] port[%x] drop %d\n",
                        addr.uc[0],addr.uc[1],addr.uc[2],addr.uc[3],addr.uc[4],addr.uc[5],0x1f,0);
                }else if(skb->data[offset]==0x3a){ 
                    /* 0x3a for ICMPv6 */
                    offset += 8;
                    if((skb->data[offset]==0x82)|| (skb->data[offset]==0x8f)){
                        /* 0x82 Multicast Listener Query Message, 0x8f Multicast Listener Report Message */
                        struct icmp6hdr *igh;
                        igh =(struct icmp6hdr *)(skb->data + offset); 
                        igh->icmp6_cksum = 0x7d50|(skb->data[head_offset] & 0xf);
                    }
                }
            }
        }
        if(ignore_packet_inspection){
            if((skb->data[dmac_offset]==0x01)&&(skb->data[dmac_offset+1]==0x00)&&(skb->data[dmac_offset+2]==0x5e)){
    
                /* 
                 * If vlan, calculate the offset of the vlan header. 
                 */
                if((skb->data[14]==0x81)&&(skb->data[15]==0x00)){
                    offset +=4;             
                }

                /* 
                 * If pppoe exists, calculate the offset of the pppoe header. 
                 */
                if((skb->data[offset]==0x88)&&((skb->data[offset+1]==0x63)||(skb->data[offset+1]==0x64))){
                    offset += sizeof(struct pppoe_hdr);             
                }

                /* 
                 * Parse IP layer.  
                 */
                if((skb->data[offset]==0x08)&&(skb->data[offset+1]==0x00)){
                    struct iphdr * iph;
                    struct igmphdr *igh;

                    offset +=2;
                    iph =(struct iphdr *)(skb->data+offset);
                    offset += (iph->ihl<<2);
                    igh =(struct igmphdr *)(skb->data + offset); 

                    /* 
                     * Sanity Check and insert port information into igmp checksum. 
                     */
                    if((iph->ihl>4)&&(iph->version==4)&&(iph->protocol==2)){
                        igh->csum = 0x7d50|(skb->data[head_offset] & 0xf);
                        printk("[eth1] Type %x port %x.\n",igh->type, skb->data[head_offset] & 0xf);
                    }

                }
            }
        }
     }
}
int athr_vlan_ctrls(struct net_device *dev, struct ifreq *ifr, int cmd)
{
    struct eth_cfg_params *ethcfg = (struct eth_cfg_params *) ifr->ifr_data;
    int ret = 0;
    int tmp = 0;
    athr_gmac_t *mac = (athr_gmac_t *) ATHR_MAC_PRIV(dev);
    athr_phy_ops_t *phy = mac->phy;
    athr_gmac_t *mac0 = (athr_gmac_t *)phy->arg_ad[0];
    struct arl_struct *arl = (struct arl_struct *) (&ethcfg->vlanid);
    unsigned int vlan_value = ethcfg->val;
    unsigned short vlan_id = vlan_value >> 16;
    unsigned short mode = vlan_value >> 16;
    unsigned short vlan_port = vlan_value & 0x1f;
    unsigned int flag = 0;

    cmd = ethcfg->cmd;

    switch (cmd) {
        case ATHR_PACKET_FLAG:
            set_packet_inspection_flag(vlan_value);
            break;

        case ATHR_VLAN_ADDPORTS:
            if (vlan_id > 4095)
                return -EINVAL;

            printk("athr_gmac::ADD_PORT vid = %d ports=%x.\n", vlan_id,
                   vlan_port);

            ret = athr_ioctl_vlan_addports(vlan_id, vlan_port);
            break;

        case ATHR_VLAN_DELPORTS:
            if (vlan_id > 4095)
                return -EINVAL;

            printk("athr_gmac::vlan_DEL_PORT vid = %d ports=%x.\n", vlan_id,
                   vlan_port);

            ret = athr_ioctl_vlan_delports(vlan_id, vlan_port);
            break;

        case ATHR_VLAN_SETTAGMODE:
            ret = athr_port_egvlanmode_set(vlan_port, mode);
            break;

        case ATHR_VLAN_SETDEFAULTID:
            if (vlan_id > 4095)
                return -EINVAL;

            printk("athr_gmac::VLAN_SETDEFAULTID vid = %d portno=%d.\n", vlan_id,
                   vlan_port);

            ret = athr_port_default_vid_set(vlan_port, vlan_id);
            break;

        case ATHR_VLAN_ENABLE:
            athr_ioctl_enable_vlan();
            break;

        case ATHR_VLAN_DISABLE:
            athr_ioctl_disable_vlan();
            break;

        case ATHR_IGMP_ON_OFF:
            tmp = vlan_value & (0x1 << 7);
            vlan_port &= ~(0x1 << 7);

            if (vlan_port > 4)
                return -EINVAL;

            if (tmp != 0) {
                printk("athr_gmac::Enable IGMP snooping in port no %x.\n",
                     vlan_port);
                ret = athr_port_igmps_status_set(vlan_port, 1);
            } else {
                printk("athr_gmac::Disable IGMP snooping in port no %x.\n",
                     vlan_port);
                ret = athr_port_igmps_status_set(vlan_port, 0);
            }

            break;
        case ATHR_LINK_GETSTAT:
            if (vlan_port > 4) { /* if port=WAN */
                int fdx, phy_up;
                athr_phy_speed_t speed;

                athr_gmac_get_link_st(mac0, &phy_up, &fdx, &speed, 4);
                ifr->ifr_ifru.ifru_ivalue = (speed << 16 | fdx << 8 | phy_up);

                printk("athr_gmac::LINK_GETSTAT portno WAN is %x.\n",
                     ifr->ifr_ifru.ifru_ivalue);

            } else if (vlan_port > 0) {
                flag = phy->is_alive(vlan_port - 1);
                ifr->ifr_ifru.ifru_ivalue = flag;

                printk("athr_gmac::LINK_GETSTAT portno %d is %s.\n",
                     vlan_port, flag ? "up" : "down");

            } else {
                ifr->ifr_ifru.ifru_ivalue = 1;
            }
            /* PHY 0-4 <---> port 1-5 in user space. */
            break;
        case ATHR_ARL_ADD:
            ret = athr_fdb_add(arl->mac_addr, arl->port_map, arl->sa_drop);

            printk("athr_gmac::ARL_ADD,mac:[%x.%x.%x.%x.%x.%x] port[%x] drop %d\n",
                 arl->mac_addr.uc[0], arl->mac_addr.uc[1], arl->mac_addr.uc[2],
                 arl->mac_addr.uc[3], arl->mac_addr.uc[4], arl->mac_addr.uc[5],
                 arl->port_map, arl->sa_drop);
            break;
        case ATHR_ARL_DEL:
            ret = athr_fdb_del(arl->mac_addr);

            printk("athr_gmac::ARL_DEL mac:[%x.%x.%x.%x.%x.%x].\n",
                   arl->mac_addr.uc[0], arl->mac_addr.uc[1], arl->mac_addr.uc[2],
                   arl->mac_addr.uc[3], arl->mac_addr.uc[4], arl->mac_addr.uc[5]);
            break;
        case ATHR_MCAST_CLR:
            if (!vlan_value)
                athr_clear_multi();
            else
                athr_set_multi();

            printk("athr_gmac::_MCAST_CLR --- %s.\n",
                   vlan_value ? "enable Multicast" : "disable Multicast");
            break;
        default:
            break;
    }

    return ret;
}
