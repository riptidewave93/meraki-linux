ifdef CONFIG_TI81XX_VPSS_SYSLINK_NOTIFY
INCS =	-I$(IPCDIR)/ \
	-I$(SYSLINK_ROOT)

EXTRA_CFLAGS += $(INCS)
endif


obj-$(CONFIG_TI81XX_VPSS) += vpss.o
vpss-y := core.o fvid2.o dctrl.o grpx.o sbuf.o system.o video.o capt.o
