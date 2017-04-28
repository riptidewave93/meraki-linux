
# Makefile for Ar7240 gigabit MAC and Phys
#

export ATH_GMAC_AR934x=1

ifeq ($(BUILD_CONFIG),_ap123)
export ATH_PHY_SWAP=1
else
#To build hostapd 0.8.0 version for db12x and mi124
export BUILD_ATHR_HOSTAP=1
endif

export ATH_GMAC_RX_TASKLET=1
export ATH_GMAC_RX_PKT_CNT=100
export ATH_GMAC_DESC_SRAM=0

export ATH_GMAC1_RXFCTL=1
export ATH_GMAC1_TXFCTL=1

export ATH_GMAC0_RXFCTL=1
export ATH_GMAC0_TXFCTL=1

ifeq ($(BUILD_CONFIG),_ap123)
export ATH_GMAC0_TX_CNT=40
export ATH_GMAC1_TX_CNT=40
else
export ATH_GMAC0_TX_CNT=128
export ATH_GMAC1_TX_CNT=128
endif

export ATH_GMAC0_RX_CNT=128
export ATH_GMAC1_RX_CNT=128

export ATH_GMAC_HW_QOS=0

export ATH_GMAC_GE0_IS_CONNECTED=1
export ATH_GMAC_GE1_IS_CONNECTED=1

export ATH_GMAC_LEN_PER_TX_DS=1536
export ATH_GMAC_LOCATION=0xbfff0000

export ATH_GMAC0_RGMII=0
export ATH_GMAC0_GMII=0
export ATH_GMAC0_MII=0

export ATH_GMAC1_RGMII=0
export ATH_GMAC1_GMII=0
export ATH_GMAC1_MII=0

export ATH_SWITCH_ONLY_MODE=0
export ETH_CONFIG=_s17

ifeq ($(ETH_CONFIG2),_s17)
export ATH_GMAC_SUPPORT_DUAL_PHY=1
export ETH_CONFIG2=_s17
endif

ifneq ($(ETH_CONFIG),_s27)
export BUILD_EXT=$(ETH_CONFIG)$(ETH_CONFIG2)
endif

# For S17 H/W Accelerator
ifeq ($(ETH_CONFIG),_s17_hwaccel)
export BUILD_CONFIG=_routing
endif

# For db12x S27 H/W Accelerator
ifeq ($(BUILD_CONFIG),_hwaccel)
export ATH_GMAC_HW_ACL=1
export ATH_GMAC_HW_NAT=1
endif

# For ap123 S27 H/W Accelerator
ifeq ($(BUILD_CONFIG),_ap123_hwaccel)
export ATH_GMAC_HW_ACL=1
export ATH_GMAC_HW_NAT=1
endif

include $(src)/Makefile.inc
ifeq ($(ATH_GMAC_AR934x),1)

obj-$(CONFIG_AG7240)			:= athrs_gmac.o
athrs_gmac-objs				:= $(ACCEL-OBJS) $(PHY-OBJS) $(MAC-OBJS) athrs_mac.o athrs_mac_timer.o athrs_flowmac.o

else

obj-$(CONFIG_AG7240)			+= athrs_gmac.o
obj-phy-$(CONFIG_MACH_AR7240)		+= ag7240.o
obj-phy-$(CONFIG_AR7240_S26_PHY)	+= phys/ar7240_s26_phy.o
obj-phy-$(CONFIG_ATHRF1_PHY)            += phys/athrf1_phy.o
obj-phy-$(CONFIG_ATHRS27_PHY)           += phys/athrs27_phy.o
obj-phy-$(CONFIG_ATHRS16_PHY)		+= phys/athrs16_phy.o
obj-phy-$(CONFIG_AR7242_S16_PHY)	+= phys/athrs16_phy.o
obj-phy-$(CONFIG_AG7240_QOS)		+= athrs_qos.o vlan_igmp.o
obj-phy-$(CONFIG_AR7240_S26_VLAN_IGMP)	+= vlan_igmp.o
obj-phy-$(CONFIG_AR7240_S27_VLAN_IGMP)	+= vlan_igmp.o
obj-hw-nat-$(CONFIG_ATHRS_HW_NAT)	+= athrs_nf_nat.o athrs_hw_nat.o
obj-hw-acl-$(CONFIG_ATHRS_HW_ACL)	+= athrs_nf_acl.o athrs_hw_acl.o

obj-phy-$(CONFIG_SOC_AR934x) 	        += ag934x.o
obj-phy-$(CONFIG_MACH_HORNET)   	+= ag7240.o
obj-phy-$(CONFIG_AR8021_PHY)        	+= phys/ar8021_phy.o
obj-phy-$(CONFIG_ATHRS_VIR_PHY)        	+= phys/athrs_vir_phy.o

ifdef ATH_GMAC_TXQUEUELEN
EXTRA_CFLAGS = -DATH_GMAC_TXQUEUELEN=$$ATH_GMAC_TXQUEUELEN
else
# refer to ether_setup
EXTRA_CFLAGS = -DATH_GMAC_TXQUEUELEN=1000
endif


athrs_gmac-objs				:= $(obj-phy-y) athrs_mac.o athrs_mac_timer.o  athrs_phy_ctrl.o athrs_gmac_ctrl.o athrs_flowmac.o $(obj-hw-nat-y) $(obj-hw-acl-y)

endif

EXTRA_CFLAGS += -I$(src)/include -I$(src)/include/phys -I$(KERNELPATH)/arch/mips/include

ifdef FLOWMACDIR
EXTRA_CFLAGS+= -I${FLOWMACDIR}
endif

ifndef NO_PUSH_BUTTON
export NO_PUSH_BUTTON=1
endif

ifeq ($(strip ${NO_PUSH_BUTTON}), 1)
EXTRA_CFLAGS+= -DNO_PUSH_BUTTON=1
else
EXTRA_CFLAGS+= -DNO_PUSH_BUTTON=0
endif


