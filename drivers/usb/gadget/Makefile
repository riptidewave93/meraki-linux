#
# USB peripheral controller drivers
#
ifeq ($(CONFIG_USB_GADGET_DEBUG),y)
	EXTRA_CFLAGS		+= -DDEBUG
endif

obj-$(CONFIG_USB_DUMMY_HCD)	+= dummy_hcd.o
obj-$(CONFIG_USB_NET2280)	+= net2280.o
obj-$(CONFIG_USB_AMD5536UDC)	+= amd5536udc.o
obj-$(CONFIG_USB_PXA25X)	+= pxa25x_udc.o
obj-$(CONFIG_USB_PXA27X)	+= pxa27x_udc.o
obj-$(CONFIG_USB_IMX)		+= imx_udc.o
obj-$(CONFIG_USB_GOKU)		+= goku_udc.o
obj-$(CONFIG_USB_OMAP)		+= omap_udc.o
obj-$(CONFIG_USB_LH7A40X)	+= lh7a40x_udc.o
obj-$(CONFIG_USB_S3C2410)	+= s3c2410_udc.o
obj-$(CONFIG_USB_AT91)		+= at91_udc.o
obj-$(CONFIG_USB_ATMEL_USBA)	+= atmel_usba_udc.o
obj-$(CONFIG_USB_FSL_USB2)	+= fsl_usb2_udc.o
fsl_usb2_udc-objs		:= fsl_udc_core.o
ifeq ($(CONFIG_ARCH_MXC),y)
fsl_usb2_udc-objs		+= fsl_mx3_udc.o
endif
obj-$(CONFIG_USB_M66592)	+= m66592-udc.o
obj-$(CONFIG_USB_R8A66597)	+= r8a66597-udc.o
obj-$(CONFIG_USB_FSL_QE)	+= fsl_qe_udc.o
obj-$(CONFIG_USB_CI13XXX)	+= ci13xxx_udc.o
obj-$(CONFIG_USB_S3C_HSOTG)	+= s3c-hsotg.o
###obj-$(CONFIG_USB_GADGET_APM_OTG) += apmotg/
obj-$(CONFIG_USB_LANGWELL)	+= langwell_udc.o
obj-$(CONFIG_USB_AR9130)	+= ar9130_udc.o
obj-$(CONFIG_USB_AR9130_OTG)	+= ar9130_otg.o
obj-$(CONFIG_USB_ATH)		+= ath_udc.o
obj-$(CONFIG_USB_ATH_OTG)	+= ath_otg.o

###obj-$(CONFIG_USB_DWC_OTG)	+= dwc_otg/

#
# USB gadget drivers
#
g_zero-objs			:= zero.o
g_audio-objs			:= audio.o
g_ether-objs			:= ether.o
g_serial-objs			:= serial.o
g_midi-objs			:= gmidi.o
gadgetfs-objs			:= inode.o
g_file_storage-objs		:= file_storage.o
g_printer-objs			:= printer.o
g_cdc-objs			:= cdc2.o

obj-$(CONFIG_USB_ZERO)		+= g_zero.o
obj-$(CONFIG_USB_AUDIO)		+= g_audio.o
obj-$(CONFIG_USB_ETH)		+= g_ether.o
obj-$(CONFIG_USB_GADGETFS)	+= gadgetfs.o
obj-$(CONFIG_USB_FILE_STORAGE)	+= g_file_storage.o
obj-$(CONFIG_USB_G_SERIAL)	+= g_serial.o
obj-$(CONFIG_USB_G_PRINTER)	+= g_printer.o
obj-$(CONFIG_USB_MIDI_GADGET)	+= g_midi.o
obj-$(CONFIG_USB_CDC_COMPOSITE) += g_cdc.o

KBUILD_CPPFLAGS	+= -Dlinux

obj-$(CONFIG_USB_DWC_OTG) += dwc_otg.o
DWC_OTG_OBJS := dwc_otg_driver.o dwc_otg_attr.o dwc_otg_cil.o dwc_otg_cil_intr.o dwc_otg_pcd.o dwc_otg_pcd_intr.o dwc_otg_hcd.o dwc_otg_hcd_intr.o dwc_otg_hcd_queue.o
dwc_otg-objs := $(addprefix dwc_otg/, $(DWC_OTG_OBJS))
dwc_otg-$(CONFIG_DWC_USE_PLB_DMA) += dwc_otg/ppc4xx_dma.o

EXTRA_CFLAGS += -DDWC_LINUX
#obj-$(CONFIG_USB_DWC_OTG) += apm_otg2.o
APM_OTG2_OBJS := dwc_cc.o dwc_common_linux.o dwc_crypto.o dwc_dh.o dwc_mem.o dwc_modpow.o dwc_notifier.o dwc_otg_attr.o dwc_otg_cfi.o dwc_otg_cil_intr.o dwc_otg_cil.o dwc_otg_driver.o dwc_otg_hcd_ddma.o dwc_otg_hcd_intr.o dwc_otg_hcd_linux.o dwc_otg_hcd.o dwc_otg_hcd_queue.o dwc_otg_pcd_intr.o dwc_otg_pcd_linux.o dwc_otg_pcd.o
apm_otg2-objs := $(addprefix apm_otg2/, $(APM_OTG2_OBJS))

ifeq ($(CONFIG_USB_GADGET_APM_OTG_DEBUG),y)
EXTRA_CFLAGS += -DCONFIG_DWC_DEBUG
endif

obj-$(CONFIG_USB_GADGET_APM_OTG) += apm_otg.o
APM_OTG_OBJS := dwc_otg_driver.o dwc_otg_attr.o dwc_otg_cil.o dwc_otg_cil_intr.o dwc_otg_pcd.o dwc_otg_pcd_intr.o dwc_otg_hcd.o dwc_otg_hcd_intr.o dwc_otg_hcd_queue.o
apm_otg-objs := $(addprefix apmotg/, $(APM_OTG_OBJS))
apm_otg-$(CONFIG_DWC_USE_PLB_DMA) += apmotg/ppc4xx_dma.o

