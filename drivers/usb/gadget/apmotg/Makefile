#
# Copyright (c) 2011, Applied Micro Circuits Corporation
# Author: Loc Ho <lho@apm.com>
#
# This program is free software; you can redistribute it and/or
# modify it under the terms of the GNU General Public License as
# published by the Free Software Foundation; either version 2 of
# the License, or (at your option) any later version.
#
# This program is distributed in the hope that it will be useful,
# but WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
# GNU General Public License for more details.
#
# You should have received a copy of the GNU General Public License
# along with this program; if not, write to the Free Software
# Foundation, Inc., 59 Temple Place, Suite 330, Boston,
# MA 02111-1307 USA
#
KBUILD_CPPFLAGS	+= -Dlinux

obj-$(CONFIG_USB_GADGET_APM_OTG) += apmotg.o

apmotg-objs 	:= dwc_otg_driver.o dwc_otg_attr.o
apmotg-objs	+= dwc_otg_cil.o dwc_otg_cil_intr.o
apmotg-objs 	+= dwc_otg_pcd.o dwc_otg_pcd_intr.o
apmotg-objs 	+= dwc_otg_hcd.o dwc_otg_hcd_intr.o dwc_otg_hcd_queue.o

apmotg-$(CONFIG_DWC_USE_PLB_DMA) += ppc4xx_dma.o

ifeq ($(CONFIG_USB_GADGET_APM_OTG_DEBUG),y)
EXTRA_CFLAGS += -DCONFIG_DWC_DEBUG
endif
