#
# Makefile for Broadcom Home Networking Division (HND) shared driver code
#
# Copyright (C) 2013, Broadcom Corporation. All Rights Reserved.
# 
# Permission to use, copy, modify, and/or distribute this software for any
# purpose with or without fee is hereby granted, provided that the above
# copyright notice and this permission notice appear in all copies.
# 
# THE SOFTWARE IS PROVIDED "AS IS" AND THE AUTHOR DISCLAIMS ALL WARRANTIES
# WITH REGARD TO THIS SOFTWARE INCLUDING ALL IMPLIED WARRANTIES OF
# MERCHANTABILITY AND FITNESS. IN NO EVENT SHALL THE AUTHOR BE LIABLE FOR ANY
# SPECIAL, DIRECT, INDIRECT, OR CONSEQUENTIAL DAMAGES OR ANY DAMAGES
# WHATSOEVER RESULTING FROM LOSS OF USE, DATA OR PROFITS, WHETHER IN AN ACTION
# OF CONTRACT, NEGLIGENCE OR OTHER TORTIOUS ACTION, ARISING OUT OF OR IN
# CONNECTION WITH THE USE OR PERFORMANCE OF THIS SOFTWARE.
#
# $Id: Makefile,v 1.5 2008-05-02 22:49:54 $
#

SHARED   := ../src/shared

include $(src)/$(SHARED)/wl_config

## from linux dir ##########
export SRCBASE_hnd := $(src)/$(SHARED)/../
KBUILD_CFLAGS += -I$(SRCBASE_hnd)/include
KBUILD_AFLAGS += -I$(SRCBASE_hnd)/include
KBUILD_CFLAGS += -DBCMDRIVER -Dlinux

KBUILD_CFLAGS += -DCFG_NORTHSTAR -DCFG_SIM

#################################

EXTRA_CFLAGS += $(WLFLAGS) -DBCM5354 -DLINUX_HYBRID_POSTPROCESSING_REMOVAL -DBCMDBG_ERR

ifeq ($(CONFIG_BCM_IPROC_GMAC_SG),y)
EXTRA_CFLAGS    += -DBCMDMASGLISTOSL
endif

### for bcm5301x_otp.h
EXTRA_CFLAGS += -I$(src)/../../otp/


#HND_OBJS += $(src)/$(SHARED)/hndfwd.o
#hnd-objs += $(SHARED)/hndfwd.o
#EXTRA_CFLAGS += -DGMAC3

#ifneq ($(CONFIG_BCM947XX),y)
HND_OBJS += $(src)/$(SHARED)/nvramstubs.o
hnd-objs += $(SHARED)/nvramstubs.o
#endif

ifeq ($(CONFIG_MACH_HX4),y)
HND_OBJS += $(src)/$(SHARED)/bcmiproc_serdes.o
hnd-objs += $(SHARED)/bcmiproc_serdes.o
HND_OBJS += $(src)/$(SHARED)/bcmiproc_phy5461s.o
hnd-objs += $(SHARED)/bcmiproc_phy5461s.o
endif
ifeq ($(CONFIG_MACH_KT2),y)
HND_OBJS += $(src)/$(SHARED)/bcmiproc_serdes.o
hnd-objs += $(SHARED)/bcmiproc_serdes.o
HND_OBJS += $(src)/$(SHARED)/bcmiproc_phy5461s.o
hnd-objs += $(SHARED)/bcmiproc_phy5461s.o
endif

ifeq ($(CONFIG_MACH_HR2),y)
HND_OBJS += $(src)/$(SHARED)/bcmiproc_phy5221.o
hnd-objs += $(SHARED)/bcmiproc_phy5221.o
endif

ifeq ($(CONFIG_MACH_NSP),y)
HND_OBJS += $(src)/$(SHARED)/bcmiproc_robo_serdes.o
hnd-objs += $(SHARED)/bcmiproc_robo_serdes.o
endif

ifeq ($(CONFIG_ET_47XX),y)
HND_OBJS += $(src)/$(SHARED)/bcmrobo.o
hnd-objs += $(SHARED)/bcmrobo.o
endif

#ifdef HNDDMA
ifeq ($(HNDDMA),1)
HND_OBJS += $(src)/$(SHARED)/hnddma.o
hnd-objs += $(SHARED)/hnddma.o
endif
#endif

#ifdef BCMUTILS
ifeq ($(BCMUTILS),1)
HND_OBJS += $(src)/$(SHARED)/bcmutils.o
hnd-objs += $(SHARED)/bcmutils.o
endif
#endif

#ifdef SIUTILS
ifeq ($(SIUTILS),1)
HND_OBJS += $(src)/$(SHARED)/siutils.o
hnd-objs += $(SHARED)/siutils.o
HND_OBJS += $(src)/$(SHARED)/aiutils.o
hnd-objs += $(SHARED)/aiutils.o
ifeq ($(CONFIG_MACH_HX4),y)
HND_OBJS += $(src)/$(SHARED)/hx4_erom.o
hnd-objs += $(SHARED)/hx4_erom.o
endif
ifeq ($(CONFIG_MACH_HR2),y)
HND_OBJS += $(src)/$(SHARED)/hr2_erom.o
hnd-objs += $(SHARED)/hr2_erom.o
endif
ifeq ($(CONFIG_MACH_NSP),y)
HND_OBJS += $(src)/$(SHARED)/nsp_erom.o
hnd-objs += $(SHARED)/nsp_erom.o
endif
ifeq ($(CONFIG_MACH_KT2),y)
HND_OBJS += $(src)/$(SHARED)/kt2_erom.o
hnd-objs += $(SHARED)/kt2_erom.o
endif
endif
#endif /* SIUTILS */

#ifdef BCMSROM
ifeq ($(BCMSROM),1)
HND_OBJS += $(src)/$(SHARED)/bcmsrom.o
hnd-objs += $(SHARED)/bcmsrom.o
HND_OBJS += $(src)/$(SHARED)/bcmotp.o
hnd-objs += $(SHARED)/bcmotp.o
endif
#endif

#ifdef BCMOTP
ifeq ($(BCMOTP),1)
ifneq ($(BCMSROM),1)
HND_OBJS += $(src)/$(SHARED)/bcmotp.o
hnd-objs += $(SHARED)/bcmotp.o
endif
EXTRA_CFLAGS += -DBCMNVRAMR
endif
#endif

#ifdef OSLLX
# linux osl
ifeq ($(OSLLX),1)
HND_OBJS += $(src)/$(SHARED)/linux_osl.o
hnd-objs += $(SHARED)/linux_osl.o
endif
#endif

# BCMDMA32
ifeq ($(BCMDMA32),1)
EXTRA_CFLAGS += -DBCMDMA32
endif

obj-$(CONFIG_HND) := hnd.o

#ifeq ($(wildcard $(src)/$(SHARED)/bcmutils.c),)
#$(src)/$(SHARED)/bcmutils.o: $(src)/$(SHARED)/linux/bcmutils.o
#	cp $< $@
#endif

#ifeq ($(wildcard $(src)/$(SHARED)/bcmrobo.c),)
#$(src)/$(SHARED)/bcmrobo.o: $(src)/$(SHARED)/linux/bcmrobo.o
#	cp $< $@
#endif

#ifeq ($(wildcard $(src)/$(SHARED)/hnddma.c),)
#$(src)/$(SHARED)/hnddma.o: $(src)/$(SHARED)/linux/hnddma.o
#	cp $< $@
#endif

#ifeq ($(wildcard $(src)/$(SHARED)/linux_osl.c),)
#$(src)/$(SHARED)/linux_osl.o: $(src)/$(SHARED)/linux/linux_osl.o
#	cp $< $@
#endif

#ifeq ($(wildcard $(src)/$(SHARED)/nvramstubs.c),)
#$(src)/$(SHARED)/nvramstubs.o: $(src)/$(SHARED)/linux/nvramstubs.o
#	cp $< $@
#endif

ifeq ($(wildcard $(src)/$(SHARED)/bcmsrom.c),)
$(src)/$(SHARED)/bcmsrom.o: $(src)/$(SHARED)/linux/bcmsrom.o
	cp $< $@
endif

#ifeq ($(wildcard $(src)/$(SHARED)/bcmotp.c),)
#$(src)/$(SHARED)/bcmotp.o: $(src)/$(SHARED)/linux/bcmotp.o
#	cp $< $@
#endif

#ifeq ($(wildcard $(src)/$(SHARED)/bcm5301x_otp.c),)
#$(src)/$(SHARED)/bcmotp.o: $(src)/$(SHARED)/linux/bcm5301x_otp.o
#	cp $< $@
#endif

#ifeq ($(wildcard $(src)/$(SHARED)/hndfwd.c),)
#$(src)/$(SHARED)/hndfwd.o: $(src)/$(SHARED)/linux/hndfwd.o
#	cp $< $@
#endif

#ifeq ($(wildcard $(src)/$(SHARED)/bcmiproc_serdes.c),)
#$(src)/$(SHARED)/bcmiproc_serdes.o: $(src)/$(SHARED)/linux/bcmiproc_serdes.o
#	cp $< $@
#endif

#ifeq ($(wildcard $(src)/$(SHARED)/bcmiproc_phy5221.c),)
#$(src)/$(SHARED)/bcmiproc_phy5221.o: $(src)/$(SHARED)/linux/bcmiproc_phy5221.o
#	cp $< $@
#endif

#ifeq ($(wildcard $(src)/$(SHARED)/bcmiproc_phy5461s.c),)
#$(src)/$(SHARED)/bcmiproc_phy5461s.o: $(src)/$(SHARED)/linux/bcmiproc_phy5461s.o
#	cp $< $@
#endif

#ifeq ($(wildcard $(src)/$(SHARED)/bcmiproc_robo_serdes.c),)
#$(src)/$(SHARED)/bcmiproc_robo_serdes.o: $(src)/$(SHARED)/linux/bcmiproc_robo_serdes.o
#	cp $< $@
#endif

#$(src)/shared_ksyms.c: $(src)/shared_ksyms.sh $(HND_OBJS)
#	sh -e $< $(HND_OBJS) > $@

hnd-objs += shared_ksyms.o
