
ifeq ($(ATH_GMAC_AR934x),1)

MAC-OBJS += ag934x.o athrs_gmac_ctrl.o athrs_phy_ctrl.o

ifeq ($(ETH_CONFIG),_s17_hwaccel)
override ETH_CONFIG := _s17
export ATH_GMAC_HW_NAT=1
endif

ifeq ($(ETH_CONFIG2),_s17_hwaccel)
export ATH_GMAC_SUPPORT_DUAL_PHY=1
override ETH_CONFIG2 := _s17
export ATH_GMAC_HW_NAT=1
endif

ifeq ($(ETH_CONFIG),_s16)
PHY-OBJS += phys/athrs16_phy.o
ATH_GMAC_GE1_IS_CONNECTED=0
ATH_GMAC0_RGMII=1
endif

ifeq ($(ETH_CONFIG),_s17)
PHY-OBJS += phys/athrs17_phy.o
ATH_GMAC0_RGMII=1
ATH_GMAC_GE1_IS_CONNECTED=0

ifeq ($(CONFIG_NETFILTER),y)
CONFIG_NETFILTER_TMP=1
endif

ifneq (, $(findstring 1, $(ATH_GMAC_HW_NAT)$(HYBRID_VLAN_IGMP)))
ifneq (, $(findstring 1, $(CONFIG_NETFILTER_TMP)$(HYBRID_VLAN_IGMP)))
PHY-OBJS += phys/ssdk_ks_km.a
else
PHY-OBJS += phys/ssdk_ks_km_o.a
endif

SSDK_PRJ_PATH=$(ENETDIR)/s17_ssdk/board/ssdk_header

EXTRA_CFLAGS += -I$(SSDK_PRJ_PATH)/include \
                -I$(SSDK_PRJ_PATH)/include/common \
                -I$(SSDK_PRJ_PATH)/include/fal \
                -I$(SSDK_PRJ_PATH)/include/hsl \
                -I$(SSDK_PRJ_PATH)/include/hsl/isis \
                -I$(SSDK_PRJ_PATH)/include/sal/os \
                -I$(SSDK_PRJ_PATH)/include/sal/sd \
                -I$(SSDK_PRJ_PATH)/include/sal/sd/linux/hydra_howl \
                -I$(SSDK_PRJ_PATH)/include/sal/sd/linux/uk_interface \
                -I$(SSDK_PRJ_PATH)/include/init
endif
endif
ifeq ($(CONFIG_FLOWMAC), y)
EXTRA_CFLAGS += -I$(FLOWMAC_DIR)
endif
ifeq ($(ETH_CONFIG),_f1e)
PHY-OBJS += phys/athrf1_phy.o
ATH_GMAC_GE1_IS_CONNECTED=0
ATH_GMAC0_RGMII=1
endif

ifeq ($(ETH_CONFIG),_f2e)
PHY-OBJS += phys/athrf2_phy.o
ATH_GMAC_GE1_IS_CONNECTED=0
ATH_GMAC0_RGMII=1
endif


ifeq ($(ETH_CONFIG),_vir)
PHY-OBJS += phys/athrs_vir_phy.o
ATH_GMAC_HW_QOS=0 
ATH_GMAC_GE1_IS_CONNECTED=0
ATH_GMAC0_RGMII=1
endif

ifeq ($(ETH_CONFIG),_s27)
PHY-OBJS += phys/athrs27_phy.o
ATH_GMAC0_MII=1
ATH_GMAC1_RGMII=1
endif 

ifeq ($(CONFIG_AR7240_S27_VLAN_IGMP),y)
PHY-OBJS += vlan_igmp.o
endif 

ifeq ($(ATH_GMAC_SUPPORT_DUAL_PHY),1)
ATH_GMAC_GE1_IS_CONNECTED=1
ATH_GMAC0_GMII=0
ATH_GMAC0_MII=0

ifeq ($(ETH_CONFIG2),_s16)
PHY-OBJS += phys/athrs16_phy.o
ATH_GMAC0_RGMII=1
endif

ifeq ($(ETH_CONFIG2),_s17)
PHY-OBJS += phys/athrs17_phy.o
ifeq ($(HYBRID_VLAN_IGMP), 1)
PHY-OBJS += phys/ssdk_ks_km.a
SSDK_PRJ_PATH=$(ENETDIR)/s17_ssdk/board/ssdk_header
EXTRA_CFLAGS += -I$(SSDK_PRJ_PATH)/include \
                -I$(SSDK_PRJ_PATH)/include/init \
                -I$(SSDK_PRJ_PATH)/include/common \
                -I$(SSDK_PRJ_PATH)/include/fal \
                -I$(SSDK_PRJ_PATH)/include/hsl \
                -I$(SSDK_PRJ_PATH)/include/hsl/isis \
                -I$(SSDK_PRJ_PATH)/include/sal/os \
                -I$(SSDK_PRJ_PATH)/include/sal/sd \
                -I$(SSDK_PRJ_PATH)/include/sal/sd/linux/hydra_howl \
                -I$(SSDK_PRJ_PATH)/include/sal/sd/linux/uk_interface \
                -I$(SSDK_PRJ_PATH)/include/init
endif
ATH_GMAC0_RGMII=1
endif

ifeq ($(ETH_CONFIG2),_f1e)
PHY-OBJS += phys/athrf1_phy.o
ATH_GMAC0_RGMII=1
endif

ifeq ($(ETH_CONFIG2),_f2e)
PHY-OBJS += phys/athrf2_phy.o
ATH_GMAC0_RGMII=1
endif

endif # ATH_GMAC_SUPPORT_DUAL_PHY
ifeq ($(ATH_GMAC_HW_NAT),1)
ifneq ($(ETH_CONFIG),_s17)
EXTRA_CFLAGS += -I ${ENETDIR}/include/hwaccels
ACCEL-OBJS += hwaccels/athrs_hw_accels.o hwaccels/athrs_gmac_nf_nat.o hwaccels/athrs_gmac_hw_nat.o hwaccels/athrs_nat.o
endif
endif

ifeq ($(ATH_GMAC_HW_ACL),1)
ifneq ($(ETH_CONFIG),_s17)
EXTRA_CFLAGS += -I ${ENETDIR}/include/hwaccels
ACCEL-OBJS += hwaccels/athrs_hw_accels.o hwaccels/athrs_gmac_nf_acl.o hwaccels/athrs_gmac_hw_acl.o hwaccels/athrs_acl.o
endif
endif
endif # ATH_GMAC_AR934x


MAC-OBJS += ag7240.o

ifeq ($(GMAC_QCA955x),1)

MAC-OBJS += qca955x.o athrs_gmac_ctrl.o athrs_phy_ctrl.o

ifeq ($(ATH_CFG_SGMII), 1)
MAC-OBJS += athrs_sgmii.o
endif

ifeq ($(ETH_CONFIG),_vir)
PHY-OBJS += phys/athrs_vir_phy.o
endif

ifeq ($(ETH_CONFIG),_ar8033)
PHY-OBJS += phys/athr_ar8033_phy.o
ATH_GMAC_HW_QOS=0
endif

ifeq ($(ETH_CONFIG2),_ar8033)
PHY-OBJS += phys/athr_ar8033_phy.o
ATH_GMAC_HW_QOS=0
endif

ifeq ($(ETH_CONFIG2),_vir)
PHY-OBJS += phys/athrs_vir_phy.o
endif

ifeq ($(ETH_CONFIG),_s17)
PHY-OBJS += phys/athrs17_phy.o
endif

ifeq ($(ETH_CONFIG2),_s17)
PHY-OBJS += phys/athrs17_phy.o
ifeq ($(HYBRID_VLAN_IGMP), 1)
PHY-OBJS += phys/ssdk_ks_km.a
SSDK_PRJ_PATH=$(ENETDIR)/s17_ssdk/board/ssdk_header
EXTRA_CFLAGS += -I$(SSDK_PRJ_PATH)/include \
                -I$(SSDK_PRJ_PATH)/include/init \
                -I$(SSDK_PRJ_PATH)/include/common \
                -I$(SSDK_PRJ_PATH)/include/fal \
                -I$(SSDK_PRJ_PATH)/include/hsl \
                -I$(SSDK_PRJ_PATH)/include/hsl/isis \
                -I$(SSDK_PRJ_PATH)/include/sal/os \
                -I$(SSDK_PRJ_PATH)/include/sal/sd \
                -I$(SSDK_PRJ_PATH)/include/sal/sd/linux/hydra_howl \
                -I$(SSDK_PRJ_PATH)/include/sal/sd/linux/uk_interface \
                -I$(SSDK_PRJ_PATH)/include/init
endif
endif


ifeq ($(ATH_GMAC_SOC_HW_NAT),1)
EXTRA_CFLAGS += -I ${ENETDIR}/include/hwaccels
ACCEL-OBJS += hwaccels/athrs_hw_accels.o hwaccels/athrs_gmac_nf_nat.o hwaccels/athrs_gmac_955x_hw_nat.o hwaccels/athrs_nat.o
endif

ifeq ($(ATH_GMAC_SOC_HW_ACL),1)
EXTRA_CFLAGS += -I ${ENETDIR}/include/hwaccels
ACCEL-OBJS += hwaccels/athrs_hw_accels.o hwaccels/athrs_gmac_nf_acl.o hwaccels/athrs_gmac_955x_hw_acl.o hwaccels/athrs_acl.o
endif
endif

ifeq ($(ATH_GMAC_HW_QOS),1)
ifneq ($(ETH_CONFIG),_s17)
ifneq ($(ETH_CONFIG2),_s17)
ACCEL-OBJS += athrs_qos.o vlan_igmp.o
endif
endif
endif

enet_config.h:
ifeq ($(ATH_GMAC_AR934x),1)

	@> include/config.h
	@echo "/*" > include/config.h
	@echo "* Automatically generated make config: don't edit" >> include/config.h
	@echo "*/" >> include/config.h

ifeq ($(ATH_GMAC_SUPPORT_DUAL_PHY),1)
	@ echo "#define CONFIG_ATHR_SUPPORT_DUAL_PHY 1" >>include/config.h
ifeq ($(ETH_CONFIG2),_s16)
	@echo "#define CONFIG_ATHRS16_PHY 1" >>include/config.h
endif
ifeq ($(ETH_CONFIG2),_s17)
	@echo "#define CONFIG_ATHRS17_PHY 1" >>include/config.h
endif
ifeq ($(ETH_CONFIG2),_f1e)
	@echo "#define CONFIG_ATHRF1_PHY 1" >>include/config.h
endif
endif # ATH_GMAC_SUPPORT_DUAL_PHY

ifeq ($(ETH_CONFIG),_s16)
	@echo "#define CONFIG_ATHRS16_PHY 1" >>include/config.h
endif
ifeq ($(ETH_CONFIG),_s17)
	@echo "#define CONFIG_ATHRS17_PHY 1" >>include/config.h
endif
ifeq ($(ETH_CONFIG),_f1e)
	@echo "#define CONFIG_ATHRF1_PHY 1" >>include/config.h
endif
ifeq ($(ETH_CONFIG),_f2e)
	@echo "#define CONFIG_ATHRF2_PHY 1" >>include/config.h
endif
ifeq ($(ETH_CONFIG),_vir)
	@echo "#define CONFIG_ATHRS_VIR_PHY 1" >>include/config.h
endif
ifeq ($(ETH_CONFIG),_s27)
	@echo "#define CONFIG_ATHRS27_PHY 1" >>include/config.h
endif 
ifeq ($(HYBRID_VLAN_IGMP), 1)
	@echo "#define CONFIG_ATHR_VLAN_IGMP 1" >>include/config.h
ifneq ($(findstring _s17,$(ETH_CONFIG2)_$(ETH_CONFIG)),)
	@echo "#define CONFIG_SWITCH_S17 1" >>include/config.h
	@echo "#define S17_FULL_FEATURE 1" >>include/config.h
else
	@echo "#define CONFIG_SWITCH_S27 1" >>include/config.h
endif
endif
	@echo "#define CONFIG_AG7240_LEN_PER_TX_DS $(ATH_GMAC_LEN_PER_TX_DS)" >> include/config.h
	@echo "#define CONFIG_AG7240_NUMBER_TX_PKTS  $(ATH_GMAC0_TX_CNT)" >>include/config.h
	@echo "#define CONFIG_AG7240_NUMBER_RX_PKTS   $(ATH_GMAC0_RX_CNT)" >>include/config.h
	@echo "#define CONFIG_AG7240_NUMBER_TX_PKTS_1 $(ATH_GMAC1_TX_CNT)" >>include/config.h
	@echo "#define CONFIG_AG7240_NUMBER_RX_PKTS_1 $(ATH_GMAC1_RX_CNT)" >>include/config.h
	@echo "#define CONFIG_AG7240_MAC_LOCATION $(ATH_GMAC_LOCATION)" >>include/config.h

ifeq ($(ATH_GMAC0_RXFCTL),1)
	@echo "#define CONFIG_GMAC0_RXFCTL 1" >>include/config.h
endif

ifeq ($(ATH_GMAC0_TXFCTL),1)
	@echo "#define CONFIG_GMAC0_TXFCTL 1" >>include/config.h
endif

ifeq ($(ATH_GMAC1_RXFCTL),1)
	@echo "#define CONFIG_GMAC1_RXFCTL 1" >>include/config.h
endif

ifeq ($(ATH_GMAC1_TXFCTL),1)
	@echo "#define CONFIG_GMAC1_TXFCTL 1" >>include/config.h
endif
ifeq ($(ATH_GMAC_GE0_IS_CONNECTED),1)
	@echo "#define CONFIG_AG7240_GE0_IS_CONNECTED 1" >> include/config.h
endif
ifeq ($(ATH_GMAC_GE1_IS_CONNECTED),1)
	@echo "#define CONFIG_AG7240_GE1_IS_CONNECTED 1" >> include/config.h
endif

ifeq ($(ATH_GMAC0_RGMII),1)
	@echo "#define CONFIG_AG7240_GE0_RGMII 1" >> include/config.h
endif
ifeq ($(ATH_GMAC0_GMII),1)
	@echo "#define CONFIG_AG7240_GE0_GMII 1" >> include/config.h
endif
ifeq ($(ATH_GMAC0_MII),1)
	@echo "#define CONFIG_AG7240_GE0_MII 1" >> include/config.h
endif

ifeq ($(ATH_GMAC1_RGMII),1)
	@echo "#define CONFIG_AG7240_GE1_RGMII 1" >> include/config.h
endif
ifeq ($(ATH_GMAC1_GMII),1)
	@echo "#define CONFIG_AG7240_GE1_GMII 1" >> include/config.h
endif
ifeq ($(ATH_GMAC1_MII),1)
	@echo "#define CONFIG_AG7240_GE1_MII 1" >> include/config.h
endif
ifeq ($(ATH_SWITCH_ONLY_MODE),1)
	@echo "#define CONFIG_ATHR_SWITCH_ONLY_MODE 1" >> include/config.h
endif
ifeq ($(ATH_GMAC_DESC_SRAM),1)
	@echo "#define CONFIG_ATHR_DESC_SRAM 1" >> include/config.h
endif
ifeq ($(ATH_VIR_PHY_DELAY),1)
	@echo "#define CONFIG_VIR_XMII_CFG $(ATH_VIR_PHY_CFG)" >> include/config.h
endif
ifeq ($(ATH_PHY_SWAP),1)
	@echo "#define CONFIG_ATHR_PHY_SWAP 1" >> include/config.h
endif


ifeq ($(ATH_GMAC_HW_NAT),1)
	@echo "#define CONFIG_ATHRS_HW_NAT  1" >>include/config.h
ifeq ($(ETH_CONFIG),_s17)
	@echo "#define S17_FULL_FEATURE 1" >>include/config.h
	@echo "#define CONFIG_ATHRS17_HNAT 1" >>include/config.h
endif
ifeq ($(ETH_CONFIG),_s27)
	@echo "#define CONFIG_ATHRS_GMAC_HW_NAT  1" >>include/config.h
endif
endif

ifeq ($(ATH_GMAC_HW_ACL),1)
	@echo "#define CONFIG_ATHRS_HW_ACL 1" >>include/config.h
ifeq ($(ETH_CONFIG),_s27)
	@echo "#define CONFIG_ATHRS_GMAC_HW_ACL 1" >>include/config.h
endif
endif
endif # ATH_GMAC_AR934x

ifeq ($(GMAC_QCA955x),1)

	@> include/config.h
	@echo "/*" > include/config.h
	@echo " * Automatically generated make config: don't edit" >> include/config.h
	@echo " */" >> include/config.h

ifeq ($(ATH_GMAC_SUPPORT_DUAL_PHY),1)
	@ echo "#define CONFIG_ATHR_SUPPORT_DUAL_PHY 1" >>include/config.h
ifeq ($(ETH_CONFIG2),_s17)
	@echo "#define ATHR_GMAC1_MII_PHY  ATHR_S17_PHY" >>include/config.h
	@echo "#define CONFIG_ATHRS17_PHY 1" >>include/config.h
endif

ifeq ($(ETH_CONFIG2),_ar8033)
	@echo "#define ATHR_GMAC1_MII_PHY ATHR_AR8033_PHY" >>include/config.h
	@echo "#define CONFIG_ATHRS_AR8033_PHY 1" >>include/config.h
ifeq ($(ATH_MDC_GPIO),1)
	@echo "#define ATH_MDC_GPIO       1"      >>include/config.h
endif 
endif # _ar8033
endif # ATH_GMAC_SUPPORT_DUAL_PHY

ifeq ($(ETH_CONFIG),_s17)
	@echo "#define ATHR_GMAC0_MII_PHY ATHR_S17_PHY" >>include/config.h
ifneq (,$(findstring $(BOARD_TYPE),ap132 ap136 ap135 ap136-p2p))
	@echo "#define ATHR_GMAC1_MII_PHY ATHR_S17_PHY" >>include/config.h
endif

ifneq (,$(findstring $(BOARD_TYPE),board955x-offload-target))
ifneq (,$(findstring $(BUILD_CONFIG),_ap136 _ap132 _ap135))
	@echo "#define ATHR_GMAC1_MII_PHY ATHR_S17_PHY" >>include/config.h
endif
endif

ifneq (,$(findstring $(BOARD_TYPE),board955x-offload-host))
ifneq (,$(findstring $(BUILD_CONFIG),_ap135_bootrom_pcie))
	@echo "#define ATHR_GMAC1_MII_PHY ATHR_S17_PHY" >>include/config.h
endif
endif

	@echo "#define CONFIG_ATHRS17_PHY 1" >>include/config.h

endif

ifeq ($(ETH_CONFIG),_ar8033)
	@echo "#define ATHR_GMAC0_MII_PHY ATHR_AR8033_PHY" >>include/config.h
	@echo "#define CONFIG_ATHRS_AR8033_PHY 1" >>include/config.h
ifeq ($(ATH_MDC_GPIO),1)
	@echo "#define ATH_MDC_GPIO       1"      >>include/config.h
endif
endif # _ar8033

ifeq ($(ETH_CONFIG),_vir)
	@echo "#define ATHR_GMAC0_MII_PHY ATHR_VIR_PHY" >>include/config.h
	@echo "#define CONFIG_ATHRS_VIR_PHY 1" >>include/config.h

endif
ifeq ($(ETH_CONFIG2),_vir)
	@echo "#define ATHR_GMAC1_MII_PHY ATHR_VIR_PHY" >>include/config.h
	@echo "#define CONFIG_ATHRS_VIR_PHY 1" >>include/config.h
endif
ifeq ($(ATH_CFG_SGMII), 1)
	@echo "#define CONFIG_ATHRS_GMAC_SGMII 1" >>include/config.h
endif

ifeq ($(ATH_SGMII_LINK_INTR),1)
	@echo "#define CONFIG_ATH_SGMII_LINK_INTR 1" >> include/config.h
endif

ifneq ($(findstring ATHR_RGMII,$(ATH_GMAC0_MII)),)
	@echo "#define ATHR_GMAC0_MII ATHR_RGMII" >> include/config.h
endif
ifneq ($(findstring ATHR_SGMII,$(ATH_GMAC0_MII)),)
	@echo "#define ATHR_GMAC0_MII ATHR_SGMII" >> include/config.h
endif

ifneq ($(findstring ATHR_MII,$(ATH_GMAC0_MII)),)
	@echo "#define ATHR_GMAC0_MII ATHR_MII" >> include/config.h
	@echo "#define GMAC0_MII 1" >> include/config.h
endif

ifneq ($(findstring ATHR_SGMII,$(ATH_GMAC1_MII)),)
	@echo "#define ATHR_GMAC1_MII ATHR_SGMII" >> include/config.h
endif
ifneq ($(findstring ATHR_RGMII,$(ATH_GMAC1_MII)),)
	@echo "#define ATHR_GMAC1_MII ATHR_RGMII" >> include/config.h
endif
ifneq ($(findstring ATHR_MII,$(ATH_GMAC1_MII)),)
	@echo "#define ATHR_GMAC1_MII ATHR_MII" >> include/config.h
endif

ifeq ($(ATH_GMAC_GE0_IS_CONNECTED),1)
	@echo "#define ATH_GE0_IS_CONNECTED 1" >> include/config.h
endif
ifeq ($(ATH_GMAC_GE1_IS_CONNECTED),1)
	@echo "#define ATH_GE1_IS_CONNECTED 1" >> include/config.h
endif

ifeq ($(ATH_SLAVE_CONNECTED),1)
	@echo "#define CONFIG_SCO_SLAVE_CONNECTED 1" >> include/config.h
endif

ifeq ($(S17_SWMAC6_CONNECTED),1)
	@echo "#define CONFIG_S17_SWMAC6_CONNECTED 1" >> include/config.h
endif

ifeq ($(ATH_SWITCH_ONLY_MODE),1)
	@echo "#define CONFIG_ATHR_SWITCH_ONLY_MODE 1" >> include/config.h
endif

ifeq ($(ATH_S17_PHY0_WAN),1)
	@echo "#define CONFIG_ATH_S17_WAN 1" >> include/config.h
endif

ifeq ($(ATH_S17_MAC0_SGMII),1)
	@echo "#define ATH_S17_MAC0_SGMII 1" >> include/config.h
endif

ifeq ($(ATH_RGMII_CAL),1)
	@echo "#define ATH_RGMII_CAL 1" >> include/config.h
endif

ifeq ($(ATH_GMAC_DESC_SRAM),1)
	@echo "#define CONFIG_ATHR_DESC_SRAM 1" >> include/config.h
endif

ifeq ($(ATH_SGMII_FORCED),1)
	@echo "#define ATH_SGMII_FORCED_MODE 1" >> include/config.h
endif
	@echo "#define CONFIG_ATH_LEN_PER_TX_DS $(ATH_GMAC_LEN_PER_TX_DS)" >> include/config.h
	@echo "#define CONFIG_ATH_NUMBER_TX_PKTS  $(ATH_GMAC0_TX_CNT)" >>include/config.h
	@echo "#define CONFIG_ATH_NUMBER_RX_PKTS   $(ATH_GMAC0_RX_CNT)" >>include/config.h
	@echo "#define CONFIG_ATH_NUMBER_TX_PKTS_1 $(ATH_GMAC1_TX_CNT)" >>include/config.h
	@echo "#define CONFIG_ATH_NUMBER_RX_PKTS_1 $(ATH_GMAC1_RX_CNT)" >>include/config.h
	@echo "#define CONFIG_ATH_MAC_LOCATION $(ATH_GMAC_LOCATION)" >>include/config.h
ifdef ATH_GMAC_TXQUEUELEN
	@echo "#define ATH_GMAC_TXQUEUELEN $$ATH_GMAC_TXQUEUELEN" >> include/config.h
else
	# refer to ether_setup
	@echo "#define ATH_GMAC_TXQUEUELEN 1000" >> include/config.h
endif


ifeq ($(ATH_GMAC_HW_NAT),1)
	@echo "#define CONFIG_ATHRS_HW_NAT  1" >>include/config.h
ifeq ($(ETH_CONFIG),_s17)
	@echo "#define S17_FULL_FEATURE 1" >>include/config.h
	@echo "#define CONFIG_ATHRS17_HNAT 1" >>include/config.h
endif
endif

ifeq ($(ATH_GMAC_SOC_HW_NAT),1)
	@echo "#define CONFIG_ATHRS_GMAC_HW_NAT 1" >>include/config.h
endif

ifeq ($(ATH_GMAC_SOC_HW_ACL),1)
	@echo "#define CONFIG_ATHRS_GMAC_HW_ACL 1" >>include/config.h
endif

endif #END QCA955x

ifeq ($(ATH_GMAC_HW_QOS),1)
	@echo "#define CONFIG_ATHRS_QOS 1" >>include/config.h
endif

ifeq ($(ATH_GMAC_RX_TASKLET),1)
	@echo "#define CONFIG_ATHR_RX_TASK 1" >>include/config.h
	@echo "#define CONFIG_ATHR_RX_PKTS_CNT $(ATH_GMAC_RX_PKT_CNT)" >>include/config.h
endif

ifdef ATH_GMAC_TXQUEUELEN
	@echo "#define ATH_GMAC_TXQUEUELEN $$ATH_GMAC_TXQUEUELEN" >> include/config.h
else
	# refer to ether_setup
	@echo "#define ATH_GMAC_TXQUEUELEN 1000" >> include/config.h
endif

ifeq ($(strip ${ATH_CAL_NAND_FLASH}), 1)
	echo "#define ATH_CAL_NAND_FLASH 1"	    >>include/config.h
	echo "#define ATH_CAL_NAND_PARTITION "\"$(strip ${ATH_CAL_NAND_PARTITION})\"	    >>include/config.h
endif

ifeq ($(ETHDEBUG),1)
	@echo "#define ETHDEBUG_ENABLED $(ETHDEBUG)" >>include/config.h
else
	@echo "#define ETHDEBUG_ENABLED 0" >>include/config.h
endif


