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
apm_enet-objs := apm_enet_mac.o		\
		apm_enet_misc.o		\
		apm_enet_eee.o          \
		apm_cle_access.o 	\
		apm_preclass_base.o   	\
		apm_preclass_api.o    	\
                apm_avl_base.o        	\
                apm_avl_api.o         	\
                apm_enet_tools.o      	\
		apm_enet_hwpool.o      	\
		apm_cle_cfg.o           \
		apm_cle_mgr.o           \
		apm_enet_ioctl.o	\
		apm_enet_qm_cfg.o	\
		apm_enet_access.o      	\
		apm_enet_sec.o

ifdef CONFIG_CLE_BRIDGE
apm_enet-objs += apm_cle_bridge.o
endif


obj-$(CONFIG_APM_NET) += apm_enet.o
obj-$(CONFIG_APM_ENET_LRO) += apm_enet_lro.o       
obj-$(CONFIG_APM_ENET_SLIMPRO_IPFW) += apm_enet_slimpro_ipfw.o       
obj-$(CONFIG_APM_VNET) += apm_vnet.o
obj-$(CONFIG_INET_OFFLOAD) += apm_enet_offload.o
