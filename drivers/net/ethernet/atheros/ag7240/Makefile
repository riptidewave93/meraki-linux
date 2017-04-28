
# Makefile for Ar7240 gigabit MAC and Phys
#

obj-$(CONFIG_AG7240)			+= ag7240_mod.o

obj-phy-y				+= ag7240.o
obj-phy-$(CONFIG_SOC_QCA955X) 	        += qca955x.o
obj-phy-y		 	        += athrs_sgmii.o
obj-phy-y		 	        += ag934x.o
obj-phy-$(CONFIG_AG7240_RGMII_PHY)      += phys/athrf1_phy.o
obj-phy-$(CONFIG_SOC_QCA955X)		+= phys/athr_ar8033_phy.o
obj-phy-$(CONFIG_SOC_QCA955X)		+= qca_soc_mac.o
obj-phy-$(CONFIG_AG7240_S26_PHY)	+= phys/ar7240_s26_phy.o
obj-phy-$(CONFIG_ATHRF1_PHY)            += phys/athrf1_phy.o
obj-phy-$(CONFIG_AG7240_ATHRS17_PHY)            += phys/athrs17_phy.o
obj-phy-$(CONFIG_ATHRS27_PHY)           += phys/athrs27_phy.o
obj-phy-$(CONFIG_ATHRS16_PHY)		+= phys/athrs16_phy.o
obj-phy-$(CONFIG_ATHRS16_PHY_CONNECT_GE0)		+= phys/athrs16_phy.o
obj-phy-$(CONFIG_AR7242_S16_PHY)	+= phys/athrs16_phy.o
obj-phy-$(CONFIG_AG7240_QOS)		+= athrs_qos.o vlan_igmp.o
obj-phy-$(CONFIG_AR7240_S26_VLAN_IGMP)	+= vlan_igmp.o
obj-phy-$(CONFIG_AR7240_S27_VLAN_IGMP)	+= vlan_igmp.o

obj-hw-nat-$(CONFIG_ATHRS_HW_NAT)	+= athrs_nf_nat.o athrs_hw_nat.o
obj-hw-acl-$(CONFIG_ATHRS_HW_ACL)	+= athrs_nf_acl.o athrs_hw_acl.o

obj-phy-$(CONFIG_SOC_AR934X) 	        += ag934x.o

obj-phy-$(CONFIG_SOC_AR934X) 	        += athrs_sgmii.o
obj-phy-$(CONFIG_SOC_AR934X)		+= ag7240.o

obj-phy-$(CONFIG_MACH_HORNET)   	+= ag7240.o
obj-phy-$(CONFIG_AR8021_PHY)        	+= phys/ar8021_phy.o 
obj-phy-$(CONFIG_ATHRS_VIR_PHY)        	+= phys/athrs_vir_phy.o 

ifeq ($(HYBRID_VLAN_IGMP), 1)
ifneq ($(findstring _s17,$(ETH_CONFIG2)_$(ETH_CONFIG)),)
PHY-OBJS	+= vlan_igmp_s17.o
else    
PHY-OBJS	+= athrs_qos.o
PHY-OBJS	+= vlan_igmp.o
endif
endif

ifdef ATH_GMAC_TXQUEUELEN
EXTRA_CFLAGS = -DATH_GMAC_TXQUEUELEN=$$ATH_GMAC_TXQUEUELEN
else
# refer to ether_setup
EXTRA_CFLAGS = -DATH_GMAC_TXQUEUELEN=1000
endif

ag7240_mod-objs				:= $(MAC-OBJS) $(obj-phy-y) $(obj-phy-m) athrs_mac.o athrs_mac_timer.o  athrs_phy_ctrl.o athrs_gmac_ctrl.o athrs_flowmac.o $(obj-hw-nat-y) $(obj-hw-acl-y)

ifeq ($(HYBRID_PLC_FILTER),1)
athrs_gmac-y += athrs_plc_filter.o
endif

EXTRA_CFLAGS += -I${src}/include -I${src}/include/phys -I$(KERNELPATH)/arch/mips/include

EXTRA_CFLAGS += -DCONFIG_MACH_AR7240 -DCONFIG_MACH_AR933x -DCONFIG_SOC_AR934X -DCONFIG_MACH_QCA955x

ifdef CONFIG_SOC_QCA955X
# This is necessary for QCA955x. See include/athrs_mac.h CONFIG_QCA955X_MAX_LEN_PER_TX_DS
EXTRA_CFLAGS += -DATHR_GMAC_SUPPORT_MULTIPLE_DS -DCONFIG_ATHRS_AR8033_PHY
endif

# AACHARYA
ifdef CONFIG_AG7240_S26_PHY
EXTRA_CFLAGS += -DCONFIG_AG7240_S26_PHY
endif
ifdef CONFIG_ATHRF1_PHY
EXTRA_CFLAGS += -DCONFIG_ATHRF1_PHY
endif
ifdef CONFIG_AG7240_GE1_IS_CONNECTED
EXTRA_CFLAGS += -DCONFIG_AG7240_GE1_IS_CONNECTED -DCONFIG_AG7240_GE1_MII
endif
ifdef CONFIG_AG7240_ATHRS17_PHY
EXTRA_CFLAGS += -DCONFIG_AG7240_ATHRS17_PHY
endif

ifdef FLOWMACDIR
EXTRA_CFLAGS+= -I${FLOWMACDIR}
endif
EXTRA_CFLAGS += -I${KERNELPATH}/../../../drivers/hybrid_bridge/os/linux
EXTRA_CFLAGS += -I${KERNELPATH}/../../../drivers/wlan/include/sys

ifndef NO_PUSH_BUTTON
export NO_PUSH_BUTTON=1
endif

ifeq ($(strip ${NO_PUSH_BUTTON}), 1)
EXTRA_CFLAGS+= -DNO_PUSH_BUTTON=1
else
EXTRA_CFLAGS+= -DNO_PUSH_BUTTON=0
endif


ifeq ($(strip ${AP136_BOOTROM_TGT}), 1)
EXTRA_CFLAGS+= -DAP136_BOOTROM_TGT
else
EXTRA_CFLAGS+= -UAP136_BOOTROM_TGT
endif

ifeq ($(strip ${AP136_BOOTROM_HOST}), 1)
EXTRA_CFLAGS+= -DAP136_BOOTROM_HOST
else
EXTRA_CFLAGS+= -UAP136_BOOTROM_HOST
endif

ifeq ($(strip ${HYBRID_VLAN_COMMUNICATE}), 1)
EXTRA_CFLAGS+= -DHYBRID_VLAN_COMMUNICATE=1
else
EXTRA_CFLAGS+= -DHYBRID_VLAN_COMMUNICATE=0
endif

ifeq ($(strip ${HYBRID_PATH_SWITCH}), 1)
EXTRA_CFLAGS+= -DATH_HY_PATH_SWITCH
endif

ifeq ($(strip ${HYBRID_PLC_FILTER}), 1)
EXTRA_CFLAGS+= -DHYBRID_PLC_FILTER
endif

ifeq ($(strip ${HYBRID_LINK_CHANGE_EVENT}), 1)
EXTRA_CFLAGS+= -DHYBRID_LINK_CHANGE_EVENT=1
else
EXTRA_CFLAGS+= -DHYBRID_LINK_CHANGE_EVENT=0
endif

ifeq ($(strip ${HYBRID_SWITCH_PORT6_USED}), 1)
EXTRA_CFLAGS+= -DHYBRID_SWITCH_PORT6_USED=1
else
EXTRA_CFLAGS+= -DHYBRID_SWITCH_PORT6_USED=0
endif

ifeq ($(strip ${HYBRID_APH126_128_S17_WAR}), 1)
EXTRA_CFLAGS+= -DHYBRID_APH126_128_S17_WAR=1
else
EXTRA_CFLAGS+= -DHYBRID_APH126_128_S17_WAR=0
endif


ifneq ($(strip ${ATHR_PORT1_LED_GPIO}), )
EXTRA_CFLAGS+= -DATHR_PORT1_LED_GPIO=${ATHR_PORT1_LED_GPIO}
endif

ifdef STAGING_DIR
ifneq ($(strip $(ATH_HEADER)),)
EXTRA_CFLAGS+= -DCONFIG_ATHEROS_HEADER_EN=1
endif
ifneq ($(strip $(CONFIG_AR7240_S26_VLAN_IGMP)),)
EXTRA_CFLAGS+= -DCONFIG_AR7240_S26_VLAN_IGMP=1
ifneq ($(strip $(CONFIG_QCAGMAC_ATH_SNOOPING_V6)),)
EXTRA_CFLAGS+= -DCONFIG_IPV6=1
endif
endif
ifneq ($(strip $(CONFIG_AR7240_S27_VLAN_IGMP)),)
EXTRA_CFLAGS+= -DCONFIG_AR7240_S27_VLAN_IGMP=1
ifneq ($(strip $(CONFIG_QCAGMAC_ATH_SNOOPING_V6)),)
EXTRA_CFLAGS+= -DCONFIG_IPV6=1
endif
endif
ifneq ($(strip $(CONFIG_AR7240_S17_VLAN_IGMP)),)
EXTRA_CFLAGS+= -DCONFIG_AR7240_S17_VLAN_IGMP=1
ifneq ($(strip $(CONFIG_QCAGMAC_ATH_SNOOPING_V6)),)
EXTRA_CFLAGS+= -DCONFIG_IPV6=1
endif
endif
endif

EXTRA_CFLAGS+= -g -ggdb

clean:
	rm -f *.o *.ko 
	rm -f phys/*.o *.ko
ifneq ($(ACCEL-OBJS),)
	rm -f hwaccels/*.o
endif
