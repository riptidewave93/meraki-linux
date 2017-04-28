#
# Makefile for Ar7100 gigebit MAC and Phys
#
obj-$(CONFIG_AG7100)			+=	ag7100_mod.o

obj-phy-$(CONFIG_AG7100_VITESSE_PHY)	+=	vsc_phy.o
obj-phy-$(CONFIG_AG7100_ICPLUS_PHY)	+=	ipPhy.o
obj-phy-$(CONFIG_AG7100_REALTEK_PHY)	+=	rtPhy.o
obj-phy-$(CONFIG_AG7100_ADM6996FC_PHY)	+=	adm_phy.o
obj-phy-$(CONFIG_AG7100_ATHRF1_PHY) 	+=	athrf1_phy.o
obj-phy-$(CONFIG_AG7100_ATHRS16_PHY)	+=	athrs16_phy.o
obj-phy-$(CONFIG_AG7100_ATHRS26_PHY) 	+=	athrs26_phy.o
obj-phy-$(CONFIG_AG7100_GENERIC_PHY)    +=	generic_phy.o
obj-phy-$(CONFIG_AG7100_VITESSE_8601_PHY) +=	vsc8601_phy.o
obj-phy-$(CONFIG_AG7100_VITESSE_8601_7395_PHY) += generic_i2c.o generic_spi.o vsc73xx.o

ag7100_mod-objs			:=	$(obj-phy-y) ag7100.o

