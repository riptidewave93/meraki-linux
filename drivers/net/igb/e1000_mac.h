/*******************************************************************************

  Intel(R) Gigabit Ethernet Linux driver
  Copyright(c) 2007-2012 Intel Corporation.

  This program is free software; you can redistribute it and/or modify it
  under the terms and conditions of the GNU General Public License,
  version 2, as published by the Free Software Foundation.

  This program is distributed in the hope it will be useful, but WITHOUT
  ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or
  FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Public License for
  more details.

  You should have received a copy of the GNU General Public License along with
  this program; if not, write to the Free Software Foundation, Inc.,
  51 Franklin St - Fifth Floor, Boston, MA 02110-1301 USA.

  The full GNU General Public License is included in this distribution in
  the file called "COPYING".

  Contact Information:
  e1000-devel Mailing List <e1000-devel@lists.sourceforge.net>
  Intel Corporation, 5200 N.E. Elam Young Parkway, Hillsboro, OR 97124-6497

*******************************************************************************/

#ifndef _E1000_MAC_H_
#define _E1000_MAC_H_

/*
 * Functions that should not be called directly from drivers but can be used
 * by other files in this 'shared code'
 */
void e1000_igb_init_mac_ops_generic(struct e1000_igb_hw *hw);
void e1000_igb_null_mac_generic(struct e1000_igb_hw *hw);
s32  e1000_igb_null_ops_generic(struct e1000_igb_hw *hw);
s32  e1000_igb_null_link_info(struct e1000_igb_hw *hw, u16 *s, u16 *d);
bool e1000_igb_null_mng_mode(struct e1000_igb_hw *hw);
void e1000_igb_null_update_mc(struct e1000_igb_hw *hw, u8 *h, u32 a);
void e1000_igb_null_write_vfta(struct e1000_igb_hw *hw, u32 a, u32 b);
void e1000_igb_null_rar_set(struct e1000_igb_hw *hw, u8 *h, u32 a);
s32  e1000_igb_blink_led_generic(struct e1000_igb_hw *hw);
s32  e1000_igb_check_for_copper_link_generic(struct e1000_igb_hw *hw);
s32  e1000_igb_check_for_fiber_link_generic(struct e1000_igb_hw *hw);
s32  e1000_igb_check_for_serdes_link_generic(struct e1000_igb_hw *hw);
s32  e1000_igb_cleanup_led_generic(struct e1000_igb_hw *hw);
s32  e1000_igb_config_fc_after_link_up_generic(struct e1000_igb_hw *hw);
s32  e1000_igb_disable_pcie_master_generic(struct e1000_igb_hw *hw);
s32  e1000_igb_force_mac_fc_generic(struct e1000_igb_hw *hw);
s32  e1000_igb_get_auto_rd_done_generic(struct e1000_igb_hw *hw);
s32  e1000_igb_get_bus_info_pcie_generic(struct e1000_igb_hw *hw);
void e1000_igb_set_lan_id_single_port(struct e1000_igb_hw *hw);
s32  e1000_igb_get_hw_semaphore_generic(struct e1000_igb_hw *hw);
s32  e1000_igb_get_speed_and_duplex_copper_generic(struct e1000_igb_hw *hw, u16 *speed,
					       u16 *duplex);
s32  e1000_igb_get_speed_and_duplex_fiber_serdes_generic(struct e1000_igb_hw *hw,
						     u16 *speed, u16 *duplex);
s32  e1000_igb_id_led_init_generic(struct e1000_igb_hw *hw);
s32  e1000_igb_led_on_generic(struct e1000_igb_hw *hw);
s32  e1000_igb_led_off_generic(struct e1000_igb_hw *hw);
void e1000_igb_update_mc_addr_list_generic(struct e1000_igb_hw *hw,
				       u8 *mc_addr_list, u32 mc_addr_count);
s32  e1000_igb_set_fc_watermarks_generic(struct e1000_igb_hw *hw);
s32  e1000_igb_setup_fiber_serdes_link_generic(struct e1000_igb_hw *hw);
s32  e1000_igb_setup_led_generic(struct e1000_igb_hw *hw);
s32  e1000_igb_setup_link_generic(struct e1000_igb_hw *hw);
s32  e1000_igb_write_8bit_ctrl_reg_generic(struct e1000_igb_hw *hw, u32 reg,
				       u32 offset, u8 data);

u32  e1000_igb_hash_mc_addr_generic(struct e1000_igb_hw *hw, u8 *mc_addr);

void e1000_igb_clear_hw_cntrs_base_generic(struct e1000_igb_hw *hw);
void e1000_igb_clear_vfta_generic(struct e1000_igb_hw *hw);
void e1000_igb_init_rx_addrs_generic(struct e1000_igb_hw *hw, u16 rar_count);
void e1000_igb_pcix_mmrbc_workaround_generic(struct e1000_igb_hw *hw);
void e1000_igb_put_hw_semaphore_generic(struct e1000_igb_hw *hw);
void e1000_igb_rar_set_generic(struct e1000_igb_hw *hw, u8 *addr, u32 index);
s32  e1000_igb_check_alt_mac_addr_generic(struct e1000_igb_hw *hw);
void e1000_igb_reset_adaptive_generic(struct e1000_igb_hw *hw);
void e1000_igb_set_pcie_no_snoop_generic(struct e1000_igb_hw *hw, u32 no_snoop);
void e1000_igb_update_adaptive_generic(struct e1000_igb_hw *hw);
void e1000_igb_write_vfta_generic(struct e1000_igb_hw *hw, u32 offset, u32 value);

#endif
