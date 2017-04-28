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

#include "athrs_mac.h"

#ifdef CONFIG_ATHRS_GMAC_HW_ACL
#include "athrs_gmac_nf_acl.h"
#endif

#ifdef CONFIG_ATHRS_GMAC_HW_NAT
#include  "athrs_gmac_nf_nat.h"
#endif

int intr_igcnt=0;
int intr_egcnt=0;
athr_nat_ager_intr(ATHR_MAC_ISR_ARGS)
{
    unsigned int intstatus=0x0;
    unsigned int lutstatus=0x0;
    unsigned int  info=0x0;
    struct timeval time1; 
    do_gettimeofday(&time1); 
   //  printk(" ISR: TIME : Sec%d usec=%d  \n ",time1.tv_sec,time1.tv_usec);
    /* read the ager  interrupt  status bit1 ->Egress lut  ,bit3 -> ingress lut */ 
    intstatus=athr_reg_rd(ATHR_LUTS_AGER_INTR);
    printk("  Ager Interrupt Status = %X \n",intstatus);
     /* Interrupt trigerred from Egres LUT */ 
    if(intstatus & 0x2)
    {
       printk(" ****** EG   Interrupt CNT = %d \n",++intr_egcnt); 
       lutstatus = athr_reg_rd(0xb8080038);
       printk(" EGRESS LUT Ager FIFO Signal = 0x%x \n",lutstatus);

       athr_reg_rmw_set(ETH_EG_AGER_FIFO_REG, 0x1);
 
       lutstatus = athr_reg_rd(0xb8080038);
       printk(" EGRESS LUT Ager FIFO Signal = 0x%x \n",lutstatus);
      /* read the deleted info from Ager FIFO */ 
       info = athr_reg_rd(0xb8080028);
       printk(" Egress  AGER Key DW0 = 0x%x \n",info);  
       info = athr_reg_rd(0xb808002c);
       printk(" Egress AGER Key DW1 = 0x%x \n",info);  
       info = athr_reg_rd(0xb8080030);
       printk(" Egress Key DW2 = %X \n",info);  
       info = athr_reg_rd(0xb8080034);
       printk(" Egress Key DW3  = %X \n ",info);  
      
       ath_reg_rmw_clear(ATHR_LUTS_AGER_INTR, 0x2);
       athr_reg_rmw_clear(ETH_EG_AGER_FIFO_REG, 0x1);
    } 
    if(intstatus & 0x8)
    { 

      printk(" ****** IG   Interrupt CNT = %d \n",++intr_igcnt); 
    //  lutstatus=athr_reg_rd(0xb808103c);
    //   printk(" Ingress Key DW2 = %X \n",lutstatus);
      lutstatus=athr_reg_rd(0xb8081040);
      printk(" Ingress AGER FIFO  = %X \n",lutstatus);
      athr_reg_rmw_set(ETH_IG_AGER_FIFO_REG, 0x1);

      lutstatus=athr_reg_rd(0xb8081040);
      printk(" Ingress AGER FIFO  = %X \n",lutstatus);
      /* read the deleted info from Ager FIFO */ 
      lutstatus=athr_reg_rd(0xb8081034);
      printk(" Ingress Key DW0 = %X \n",lutstatus);
      lutstatus=athr_reg_rd(0xb8081038);
      printk(" Ingress Key DW1 = %X \n",lutstatus);
      lutstatus=athr_reg_rd(0xb8081070);
      printk(" Ingress Key DW2 = %X \n",lutstatus);


      lutstatus=athr_reg_rd(0xb8081040);
      printk(" Ingress AGER FIFO  = %X \n",lutstatus);

      ath_reg_rmw_clear(ATHR_LUTS_AGER_INTR, 0x8);
      athr_reg_rmw_clear(ETH_IG_AGER_FIFO_REG, 0x1);
     //  ath_reg_rmw_clear(ATHR_LUTS_AGER_INTR, 0x8);
    } 

      /* Clear the LUT_AGER interrupt -write 1 to BIT 13  */ 
      athr_reg_rmw_set(ATH_MISC_INT_STATUS, (1<<13));
      return IRQ_HANDLED;
}

int athr_nat_ager_intr_init( )
{ 
      int st;
      st = request_irq(ATH_MISC_IRQ_NAT_AGER, athr_nat_ager_intr, ATHR_MAC_IRQF_DISABLED, "nat ager intr", NULL);
      if (st < 0)
      {
          printk(": request irq %d failed %d\n", ATH_MISC_IRQ_NAT_AGER, st);
          return 1;
      } 
} 

/* 
***********************************************************
*********AGER TIMEOUT  -TIMER VALUE CALCULATION **********
***********************************************************
1 tick in .025 us
1 us 1/.025 = 40 ticks
in 1 us 40 ticks
1 msec 40000 
for 10 msec 400000
for 100 msec 4000000
 
ath_reg_wr(IG_AGER_TICK, 0x3d0900); - 100msec
ath_reg_wr(IG_AGER_TIME_OUT, ( (0xA <<22)|(0xA<<12)|(0xA)) ); - 10
 
so that 10 * 100msec = 1 sec timeout
*/ 

int athr_nat_ager_lut_init( int dir )
{ 
#define ETH_EGRESS_LUT_ADDRESS  0xb8080000
#define EG_AGER_TICK_ADDRESS    0x005c
#define EG_AGER_TICK            ETH_EGRESS_LUT_ADDRESS + EG_AGER_TICK_ADDRESS
#define EG_AGER_TIMEOUT_ADDRESS    0x0060
#define EG_AGER_TIMEOUT           ETH_EGRESS_LUT_ADDRESS + EG_AGER_TIMEOUT_ADDRESS

#define ETH_INGRESS_LUT_ADDRESS 0xb8081000
#define IG_AGER_TICK_ADDRESS    0x0068
#define IG_AGER_TICK            ETH_INGRESS_LUT_ADDRESS + IG_AGER_TICK_ADDRESS
#define IG_AGER_TIME_OUT_ADDRESS  0x006c
#define IG_AGER_TIMEOUT          ETH_INGRESS_LUT_ADDRESS + IG_AGER_TIME_OUT_ADDRESS

	if( portdir == INGRESS ) {
		printk("IG :  TICK = %08X  TIMEOUT = %08X \n",IG_AGER_TICK,IG_AGER_TIMEOUT); 
		/* 1sec Timeout Value */ 
		ath_reg_wr(IG_AGER_TICK, 0x3d0900);
 	        ath_reg_wr(IG_AGER_TIMEOUT, ( (0xA <<22)|(0xA<<12)|(0xA)) );
	}
	if( portdir == EGRESS ) {
		printk(" TICK = %08X  TIMEOUT = %08X \n",EG_AGER_TICK,EG_AGER_TIMEOUT); 
		/* 1sec Timeout Value */ 
		ath_reg_wr(EG_AGER_TICK, 0x3d0900);
		ath_reg_wr(EG_AGER_TIMEOUT, ( (0xA <<22)|(0xA<<12)|(0xA)) );
	}
} 

void athr_test_ingress_aging_interrupt()
{
 uint32_t key[4]; 
 uint32_t info[4]; 
 int iLoopcnt=0; 
 int ret=0; 
 int cnt=0x0;  

 for(iLoopcnt=0;iLoopcnt<500;iLoopcnt++ ) 
   { 
           // int delay = 0xfffffffe; 
           key[0] = 0xaaaa5555+ iLoopcnt; 
           key[1] = 0xbbbb6666+ iLoopcnt; 
           key[2] = 0x11112222+ iLoopcnt; 
           key[3] = 0x33334444+ iLoopcnt; 
           info[0] = 0xcccc7777 +iLoopcnt; 
           info[1] = 0xdddd8888 +iLoopcnt;
           info[2] = 0xeeee9999 +iLoopcnt;
           info[3] = 0xffffaaaa+iLoopcnt; 
           ret=athrs_nat_add_del_ops(key,info,INGRESS,CMD_INS,0x3);
	   if(ret)
		  break;	
           printk(" CMD_INS %d = ret = %d \n",iLoopcnt,ret);
//	   do_gettimeofday(&time1); 
//	   printk(" TIME : Sec%d usec=%d  \n ",time1.tv_sec,time1.tv_usec);
           mdelay(10);  
           /* printk(" Entry : %d :: ret = %d key[0] = %X,key[1]=%X info[0]= %X info[1] = %X info[2] = %X info[3] = %X \n ",
								iLoopcnt,ret,key[0],key[1],info[0],info[1],info[2],info[3]); */ 
  } 
} 
#ifdef IPROUTE_TEST
/* the below test code to test the IPROTE LUT which is integerate with bridge code 
 br_hw_lut_insert,lookip,delete should be exportd from br_hw_lut.c file */  
int br_hw_lut_insert(const unsigned char *);
int br_hw_lut_lookup(const unsigned char *);
int br_hw_lut_modify(const unsigned char *);
int br_hw_lut_delete(const unsigned char *);
void  LUTStress(void); 
void  LUTStress_del(void);

void  LUTStress()
{
	int  key=0,iLoopcnt=0;
	unsigned long uldata[2]; 
        uldata[1] = 0xeeddbbcc; 
        uldata[0] = 0xAA550000; 

	for(iLoopcnt=0;iLoopcnt<64;iLoopcnt++)	{
	      key=br_hw_lut_insert( (const unsigned char *) &uldata[0]); 
  	      uldata[0]++ ; 
	//    printk(" Insert  key = %d  uldata %08X  \n",key,uldata[0]);
         } 
	 uldata[1] = 0xeeddbbcc; 
         uldata[0] = 0xAA550000; 

	for(iLoopcnt=0;iLoopcnt<64;iLoopcnt++)	{
	      key=br_hw_lut_lookup( (const unsigned char *) &uldata[0]); 
	      uldata[0]++ ; 
	 //   printk(" LKP =  key = %d  uldata %08X  \n",key,uldata[0]);
         } 
} 
void  LUTStress_del()
{
	int  key=0,iLoopcnt=0;
	unsigned long uldata[2]; 

        uldata[1] = 0xeeddbbcc; 
        uldata[0] = 0xAA550000; 

	for(iLoopcnt=0;iLoopcnt<64;iLoopcnt++)	{ 
	     key=br_hw_lut_delete( (const unsigned char *) &uldata[0]); 
	     uldata[0]++ ; 
	     // printk(" Delete key = %d  uldata %08X  \n",key,uldata[0]);
         } 
} 
/* End of IPROUTE Lut test code */ 
#endif  
