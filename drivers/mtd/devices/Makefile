#
# linux/drivers/mtd/devices/Makefile
#

obj-$(CONFIG_MTD_DOC2000)	+= doc2000.o
obj-$(CONFIG_MTD_DOC2001)	+= doc2001.o
obj-$(CONFIG_MTD_DOC2001PLUS)	+= doc2001plus.o
obj-$(CONFIG_MTD_DOCPROBE)	+= docprobe.o
obj-$(CONFIG_MTD_DOCECC)	+= docecc.o
obj-$(CONFIG_MTD_SLRAM)		+= slram.o
obj-$(CONFIG_MTD_PHRAM)		+= phram.o
obj-$(CONFIG_MTD_PMC551)	+= pmc551.o
obj-$(CONFIG_MTD_MS02NV)	+= ms02-nv.o
obj-$(CONFIG_MTD_MTDRAM)	+= mtdram.o
obj-$(CONFIG_MTD_LART)		+= lart.o
obj-$(CONFIG_MTD_BLOCK2MTD)	+= block2mtd.o
obj-$(CONFIG_MTD_DATAFLASH)	+= mtd_dataflash.o
obj-$(CONFIG_MTD_M25P80)	+= m25p80.o
obj-$(CONFIG_MTD_AR7240_SPI_FLASH)	+= ar7240_flash.o
obj-$(CONFIG_MTD_AR7100_SPI_FLASH)  += ar7100_flash.o
ifeq ($(ATH_SST_FLASH), y)
CFLAGS_ar7240_flash.o += -DATH_SST_FLASH=1
endif
obj-$(CONFIG_MTD_ATH_FLASH)	+= ath_flash.o
ifeq ($(ATH_SST_FLASH), y)
CFLAGS_ath_flash.o += -DATH_SST_FLASH=1
endif
obj-$(CONFIG_MTD_MT29F)		+= mt29f.o
obj-$(CONFIG_MTD_SST25L)	+= sst25l.o
