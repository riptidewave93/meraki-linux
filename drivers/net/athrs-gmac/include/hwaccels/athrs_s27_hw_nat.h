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

#ifndef _ATHR_S27_HW_NAT_H
#define _ATHR_S27_HW_NAT_H

#include "athrs_mac.h"

#define pl() printk("%s:%d\n",__func__,__LINE__)


#define EGRESS 		      0x1
#define INGRESS		      0x2

#define TCP_PROT              0x1
#define UDP_PROT	      0x2
#define ICMP_PROT             0x3
#define UNSUPPORTED           0x0

#define is_supp(x)            (x > UNSUPPORTED && x <= ICMP_PROT )
#define is_icmp(x)            (x == ICMP_PROT)

#define CMD_LKP               0x2
#define CMD_INS               0x3
#define CMD_DEL               0x4
#define CMD_MASK              0x7

/* Return Status */

#define SUCCESS 	     0
#define EBKT_FULL            0x1
#define EBIN_FULL            0x2
#define EDUP_KEY             0x3
#define ENO_DATA              0x4

/* Register Defines */

#define EG_CPU_REQ_REG	              0x0	   /* Reg to insert/delete/lookup an entry in LUT */
#define EG_PKT_TYPE(x)	              (x << 5)
#define EG_REQ_ENABLE(x)              (x << 4)
#define EG_LUT_INIT(x)	              (x << 3)
#define EG_LUT_CMD(x)	              (x << 0)

#define EG_INFO_INS_DW0_REG           0x8         /* Used  to programm the 24 bits of INFO */
#define EG_INFO_STAT_DW0_REG          0xc         /* INFO match found for delete/lkp */
#define EG_INFO_WAN_PORT(x)           (x << 0)    /* INFO data elements */
#define EG_INFO_WAN_IP_IDX(x)         (x << 16)   /* WAN IP ADDR select */
#define EG_INFO_CONN_STAT(x)          (x << 19)

#define EG_KEY_DW0_REG                0x10
#define EG_KEY_DW1_REG                0x14
#define EG_KEY_DW1_LAN_PORT(x)        (x << 0)
#define EG_KEY_DW1_PROT(x)            (x << 16)
#define EG_KEY_DW0_LAN_PORT(x)        (x << 0)
#define EG_KEY_DW0_PROT(x)            (x << 16)
#define EG_KEY_DW0_LAN_ADDR(x)        ((x & 0x3fff) << 18)
#define EG_KEY_DW1_LAN_ADDR(x)        ((x >> 14) & 0x3ffff)

#define IG_CPU_REQ_REG                0x0
#define IG_PKT_TYPE(x)                (x << 5)
#define IG_REQ_ENABLE(x)              (x << 4)
#define IG_LUT_INIT(x)                (x << 3)
#define IG_LUT_CMD(x)                 (x << 0)

#define IG_INFO_DW0_REG               0x8         /* Used  to programm the 24 bits of INFO */
#define IG_INFO_DW1_REG               0xc
#define IG_INFO_DW2_REG               0x10
#define IG_INFO_DW3_REG               0x14
#define IG_INFO_STAT_DW0_REG          0x18        /* INFO match found for delete/lkp */
#define IG_INFO_STAT_DW1_REG          0x1c
#define IG_INFO_STAT_DW2_REG          0x20
#define IG_INFO_STAT_DW3_REG          0x24

#define IG_INFO_DW0_LAN_IP(x)         (x << 0)
#define IG_INFO_DW1_LAN_PORT(x)       (x << 0)
#define IG_INFO_DW1_LAN_MAC_LS(x)     (x << 16)
#define IG_INFO_DW2_LAN_MAC_MS(x)     (x << 0)
#define IG_INFO_DW3_CONN_STAT(x)      (x << 0)

#define IG_KEY_DW0_REG                0x28
#define IG_KEY_DW0_WAN_PORT(x)        (x << 0)    /* WAN port Number */
#define IG_KEY_DW0_PROT(x)            (x << 16)   /* Protocol key */
#define IG_KEY_DW0_WAN_IP_IDX(x)      (x << 18)   /* WAN IP ADDR select */

/* Status registers */
#define EG_CPU_REQ_STAT_REG           0x4
#define IG_CPU_REQ_STAT_REG           0x4

/* 
 * Egress Fragment registers 
 */
#define EG_FRAG_KEY_DW0_REG           0x10
#define EG_FRAG_KEY_DW1_REG           0x14
#define EG_FRAG_INFO_DW0_REG          0x08
#define EG_FRAG_CPU_REQ_REG           0x00
#define EG_FRAG_CPU_REQ_STAT_REG      0x04
#define EG_FRAG_CPU_REQ_INFO_DW0      0x0c
#define EG_FRAG_AGER_FIFO_REG         0x20
#define EG_FRAG_AGER_TICK_REG         0x3c
#define EG_FRAG_AGER_TIME_OUT_REG     0x40

/* 
 * Ingress Fragment registers 
 */

#define IG_FRAG_KEY_DW0_REG           0x20
#define IG_FRAG_KEY_DW1_REG           0x24
#define IG_FRAG_CPU_REQ_REG           0x00
#define IG_FRAG_CPU_REQ_STAT_REG      0x04
#define IG_FRAG_INFO_DW0_REG          0x08
#define IG_FRAG_INFO_DW1_REG          0x0c
#define IG_FRAG_INFO_DW2_REG          0x10
#define IG_FRAG_CPU_REQ_INFO_DW0      0x14
#define IG_FRAG_CPU_REQ_INFO_DW1      0x18
#define IG_FRAG_CPU_REQ_INFO_DW2      0x1c
#define IG_FRAG_AGER_FIFO_REG         0x30
#define IG_FRAG_AGER_TICK_REG         0x58
#define IG_FRAG_AGER_TIME_OUT_REG     0x5c


/*
 * Timeout registers
 */
#define EG_AGER_TICKS                 0x5c
#define EG_AGER_TIME_OUT              0x60
#define IG_AGER_TICKS                 0x1070
#define IG_AGER_TIME_OUT              0x1074


/* LUT OP status values */
#define STAT_BKT_FULL                 (1 << 6)
#define STAT_REQ_DONE                 (1 << 5)
#define STAT_INSERT	              (1 << 4 | 1 << 0)
#define STAT_BIN_FULL                 (1 << 3)
#define STAT_DUPLICATE                (1 << 4 | 1 << 2 | 1 << 0)
#define STAT_DATA_FND                 (1 << 1)
#define STAT_CMD_ACK                  (1 << 0)

typedef struct {
   uint8_t prot_key      	  : 3;	   /* 1:TCP_PROT 2:UDP_PROT 3:ICMP_PORT 0:UNSUPP */
   uint32_t lan_ip_addr;		   /* Source IP addr */
   union {
      uint16_t lan_icmp_id;		   /* ICMP_PROT */
      uint16_t lan_port;                   /* TCP/UDP */
   };
   struct  {
      uint8_t l2_con_st          : 5;	   /* Conection Status */
      uint8_t wan_ip_idx         : 3;      /* Points to WAN IP address */
      union {
            uint16_t wan_port;             /* ICMP ID / PORT */
            uint16_t wan_icmp_id;
      };
   }info;

}ath_egrs_t;

typedef struct {
   uint8_t prot_key              : 3;
   uint8_t wan_ip_idx            : 2;
   union {
      uint16_t wan_icmp_id;		   /* ICMP_PROT */
      uint16_t wan_port;                   /* TCP/UDP */
   };
   struct  {
      uint8_t l2_con_st          : 5;
      uint8_t lan_mac_addr[6];
      uint32_t lan_ip_addr;
      union {
         uint16_t lan_port;
         uint16_t lan_icmp_id;
      };
   } info;

}ath_ingrs_t;

#define ATH_NAT_TCP_TIMEOUT     2000 /* 2 Mins */
#define ATH_NAT_UDP_TIMEOUT     300 /* 30 secs */
#define ATH_NAT_ICMP_TIMEOUT    300 /* 30 secs */


#define athr_reg_read_frag_lut(off, dir) \
    athr_reg_rd((dir == INGRESS ?     \
    ATHR_INGRESS_LUT_FRAG_REG:  \
    ATHR_EGRESS_LUT_FRAG_REG) | off)

#define athr_reg_write_frag_lut(off, val, dir)  \
    athr_reg_wr((dir == INGRESS ?     \
    ATHR_INGRESS_LUT_FRAG_REG : \
    ATHR_EGRESS_LUT_FRAG_REG) | off, val)



#define athr_reg_read_lut(off, dir) \
    athr_reg_rd((dir == INGRESS ?     \
    ATHR_INGRESS_LUT_REG :  \
    ATHR_EGRESS_LUT_REG) | off)

#define athr_reg_write_lut(off, val, dir)  \
    athr_reg_wr((dir == INGRESS ?     \
    ATHR_INGRESS_LUT_REG : \
    ATHR_EGRESS_LUT_REG) | off, val)

#define athr_reg_write_global_ip_address(addr, wan_index) \
               athr_reg_wr(ATHR_WAN_IP_ADDR_REG + (wan_index * 4), addr)

/*
 * Enable nat and disable L2 SA & DA mac addr edits
 * if l2 mac edits are needed, then write 0x0 instead
 * of 0xc
 */
#define athrs_nat_enable(portdir)  \
    athr_reg_wr(((portdir == INGRESS)?  \
                   ETH_IG_NAT_STATUS : \
                   ETH_EG_NAT_STATUS), \
		   ((portdir == INGRESS)? \
		   0xc:0xc))

#define athrs_nat_disable(portdir)  \
    athr_reg_wr(((portdir == INGRESS)?  \
                   ETH_IG_NAT_STATUS : \
                   ETH_EG_NAT_STATUS), 0x1)

#define athrs_lut_ageing_timer_disable(portdir) \
do { \
    athr_reg_wr(((portdir == INGRESS)?  \
                   ETH_IG_AGER_FIFO_REG: \
                   ETH_EG_AGER_FIFO_REG), 0x4); \
} while(0)

#define athrs_lut_frag_ageing_timer_disable(portdir) \
do { \
    athr_reg_wr(((portdir == INGRESS)?  \
                   (ATHR_INGRESS_LUT_FRAG_REG + IG_FRAG_AGER_FIFO_REG): \
                   (ATHR_EGRESS_LUT_FRAG_REG + EG_FRAG_AGER_FIFO_REG)), 0x4); \
} while(0)



#define athrs_lut_init(portdir) \
    athr_reg_write_lut(0, (1 << 3), portdir)


#define athrs_frag_lut_init(portdir) \
    athr_reg_write_frag_lut(0, (1 << 3), portdir)

#define athrs_hw_nat_set_router_mac_addr(addr) \
    athr_reg_wr(ETH_LCL_MAC_ADDR_DW0,\
                  ((addr[2] << 24) | (addr[3] << 16)| \
                   (addr[4] << 8)  | (addr[5]))); \
    athr_reg_wr(ETH_LCL_MAC_ADDR_DW1,\
                  ((addr[0] << 8) | addr[1]))

#define athrs_hw_nat_set_next_hop_mac_addr(addr) \
    athr_reg_wr(ETH_DST_MAC_ADDR_DW0,\
                  ((addr[2] << 24) | (addr[3] << 16)| \
                   (addr[4] << 8)  | (addr[5]))); \
    athr_reg_wr(ETH_DST_MAC_ADDR_DW1,\
                  ((addr[0] << 8) | addr[1]))

#define athrs_lut_set_ageing_timeout(portdir, tcp, udp, icmp) \
    athr_reg_wr(((portdir == INGRESS)?        \
                   IG_AGER_TIME_OUT:            \
                   EG_AGER_TIME_OUT),           \
                   ((icmp << 22) | (udp << 12) |  \
                    tcp))

#define athrs_lut_set_ageing_ticks(portdir, ticks) \
    athr_reg_wr(((portdir == INGRESS) ? \
                   IG_AGER_TICKS:         \
                   EG_AGER_TICKS), ticks)

#define TX_NAT_ENABLE 5
#define TX_NAT_STATUS 3

#define RX_NAT_STATUS 5
#define RX_CKSUM_STATUS 4

#define ATHR_TX_HW_NAT_ENABLE(ds) (ds->res1 |= (1 << TX_NAT_ENABLE))
#define ATHR_TX_HW_NAT_STATUS(ds) (ds->res1 &  (1 << TX_NAT_STATUS))

#define ATHR_RX_HW_NAT_STATUS(ds) (ds->res1 &  (1 << RX_NAT_STATUS))
#define ATHR_RX_CKSUM(ds)         (ds->res1 &  (1 << RX_CKSUM_STATUS))

#endif
