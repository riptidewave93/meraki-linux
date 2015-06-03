#
# Makefile for DWC_otg Highspeed USB controller driver
#

KBUILD_CPPFLAGS	+= -Dlinux 

obj-$(CONFIG_USB_DWC_OTG)	+= dwc_otg.o

dwc_otg-y := dwc_otg_driver.o dwc_otg_attr.o dwc_otg_cil.o \
	    dwc_otg_cil_intr.o dwc_otg_pcd.o dwc_otg_pcd_intr.o \
	    dwc_otg_hcd.o dwc_otg_hcd_intr.o dwc_otg_hcd_queue.o
dwc_otg-$(CONFIG_DWC_USE_PLB_DMA) += ppc4xx_dma.o
