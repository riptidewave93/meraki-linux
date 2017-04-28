/*
 * QCA955x init
 *
 * Contents of this file have been copied from Atheros LSDK-9.5.5.36/u-boot
 *
 */

#include <linux/serial_reg.h>
#include <linux/smp.h>
#include <linux/pci_regs.h>
#include <asm/addrspace.h>
#include <asm/byteorder.h>
#include <asm/page.h>
#include <asm/mipsregs.h>
#include <asm/r4kcache.h>
#include <asm/cacheops.h>
#include <asm/mach-ath79/ath79.h>
#include <asm/mach-ath79/ar71xx_regs.h>
#include "nandloader_common.h"

/*
 * PLL Config for different CPU/DDR/AHB frequencies
 */
#define CFG_PLL_720_600_200     0x01
#define CFG_PLL_720_680_240     0x02
#define CFG_PLL_720_600_240     0x03
#define CFG_PLL_680_680_226     0x04
#define CFG_PLL_720_600_300     0x05
#define CFG_PLL_400_400_200     0x06
#define CFG_PLL_560_450_220     0x07
#define CFG_PLL_550_400_200     0x08
#define CFG_PLL_550_600_200     0x09
#define CFG_PLL_FREQ    CFG_PLL_720_600_200
#define ATH_MEM_DDR1            2
#define ATH_MEM_DDR2            3

#include "955x.h"

#define ATH_MAX_DDR_SIZE		(256 * 1024 * 1024)
#define ATH_DDR_SIZE_INCR		(4 * 1024 * 1024)
#define CFG_DDR_REFRESH_VAL      0x4138

inline int
ath_ram_type(uint32_t bs)
{
	if (RST_BOOTSTRAP_DDR_SELECT_GET(bs)) {
		return ATH_MEM_DDR1;
	} else {
		return ATH_MEM_DDR2;
	}
}


#define CFG_DDR2_SCORPION_CAS_LATENCY	4

#ifdef CONFIG_TB614
#	define DDR_CONFIG2_SWAP_A26_A27_VAL	(0x1)
#else
#	define DDR_CONFIG2_SWAP_A26_A27_VAL	(0x0)
#endif

#if CFG_DDR2_SCORPION_CAS_LATENCY == 4
#define CFG_DDR2_CONFIG_VAL			DDR_CONFIG_CAS_LATENCY_MSB_SET(0x1) | \
						DDR_CONFIG_OPEN_PAGE_SET(0x1) | \
						DDR_CONFIG_CAS_LATENCY_SET(0x1) | \
						DDR_CONFIG_TMRD_SET(0xf) | \
						DDR_CONFIG_TRFC_SET(0x15) | \
						DDR_CONFIG_TRRD_SET(0x7) | \
						DDR_CONFIG_TRP_SET(0x9) | \
						DDR_CONFIG_TRCD_SET(0x9) | \
						DDR_CONFIG_TRAS_SET(0x1b)

#define CFG_DDR2_CONFIG2_VAL			DDR_CONFIG2_HALF_WIDTH_LOW_SET(0x1) | \
						DDR_CONFIG2_SWAP_A26_A27_SET(DDR_CONFIG2_SWAP_A26_A27_VAL) | \
						DDR_CONFIG2_GATE_OPEN_LATENCY_SET(0x8) | \
						DDR_CONFIG2_TWTR_SET(0x15) | \
						DDR_CONFIG2_TRTP_SET(0x9) | \
						DDR_CONFIG2_TRTW_SET(0xe) | \
						DDR_CONFIG2_TWR_SET(0x1) | \
						DDR_CONFIG2_CKE_SET(0x1) | \
						DDR_CONFIG2_CNTL_OE_EN_SET(0x1) | \
						DDR_CONFIG2_BURST_LENGTH_SET(0x8)

#define CFG_DDR2_CONFIG3_VAL			0x0000000a
#define CFG_DDR2_EXT_MODE_VAL			0x402
#define CFG_DDR2_MODE_VAL_INIT			0x143
#define CFG_DDR2_MODE_VAL			0x43
#define CFG_DDR2_TAP_VAL			0x10
#define CFG_DDR2_EN_TWL_VAL			0x0000167d
#define CFG_DDR2_RD_DATA_THIS_CYCLE_VAL_16	0xffff
#define CFG_DDR2_RD_DATA_THIS_CYCLE_VAL_32	0xff

#elif CFG_DDR2_SCORPION_CAS_LATENCY == 5

#define CFG_DDR2_CONFIG_VAL			DDR_CONFIG_CAS_LATENCY_MSB_SET(0x1) | \
						DDR_CONFIG_OPEN_PAGE_SET(0x1) | \
						DDR_CONFIG_CAS_LATENCY_SET(0x4) | \
						DDR_CONFIG_TMRD_SET(0xf) | \
						DDR_CONFIG_TRFC_SET(0x15) | \
						DDR_CONFIG_TRRD_SET(0x7) | \
						DDR_CONFIG_TRP_SET(0x9) | \
						DDR_CONFIG_TRCD_SET(0x9) | \
						DDR_CONFIG_TRAS_SET(0x1b)

#define CFG_DDR2_CONFIG2_VAL			DDR_CONFIG2_HALF_WIDTH_LOW_SET(0x1) | \
						DDR_CONFIG2_SWAP_A26_A27_SET(DDR_CONFIG2_SWAP_A26_A27_VAL) | \
						DDR_CONFIG2_GATE_OPEN_LATENCY_SET(0xb) | \
						DDR_CONFIG2_TWTR_SET(0x15) | \
						DDR_CONFIG2_TRTP_SET(0x9) | \
						DDR_CONFIG2_TRTW_SET(0xe) | \
						DDR_CONFIG2_TWR_SET(0x1) | \
						DDR_CONFIG2_CKE_SET(0x1) | \
						DDR_CONFIG2_CNTL_OE_EN_SET(0x1) | \
						DDR_CONFIG2_BURST_LENGTH_SET(0x8)

#define CFG_DDR2_CONFIG3_VAL			0x0000000a
#define CFG_DDR2_EXT_MODE_VAL			0x402
#define CFG_DDR2_MODE_VAL_INIT			0x153
#define CFG_DDR2_MODE_VAL			0x53
#define CFG_DDR2_TAP_VAL			0x10
#define CFG_DDR2_EN_TWL_VAL			0x00001e7d
#define CFG_DDR2_RD_DATA_THIS_CYCLE_VAL_16	0xffff
#define CFG_DDR2_RD_DATA_THIS_CYCLE_VAL_32	0xff
#endif

#define CFG_DDR1_CONFIG_VAL			DDR_CONFIG_OPEN_PAGE_SET(0x1) | \
						DDR_CONFIG_CAS_LATENCY_SET(0x7) | \
						DDR_CONFIG_TMRD_SET(0x5) | \
						DDR_CONFIG_TRFC_SET(0x7) | \
						DDR_CONFIG_TRRD_SET(0x4) | \
						DDR_CONFIG_TRP_SET(0x6) | \
						DDR_CONFIG_TRCD_SET(0x6) | \
						DDR_CONFIG_TRAS_SET(0x10)

#define CFG_DDR1_CONFIG2_VAL			DDR_CONFIG2_HALF_WIDTH_LOW_SET(0x1) | \
						DDR_CONFIG2_GATE_OPEN_LATENCY_SET(0x6) | \
						DDR_CONFIG2_TWTR_SET(0xe) | \
						DDR_CONFIG2_TRTP_SET(0x8) | \
						DDR_CONFIG2_TRTW_SET(0xe) | \
						DDR_CONFIG2_TWR_SET(0xd) | \
						DDR_CONFIG2_CKE_SET(0x1) | \
						DDR_CONFIG2_CNTL_OE_EN_SET(0x1) | \
						DDR_CONFIG2_BURST_LENGTH_SET(0x8)
#define CFG_DDR1_CONFIG3_VAL			0x0
#define CFG_DDR1_EXT_MODE_VAL			0x0
#define CFG_DDR1_MODE_VAL_INIT			0x133
#define CFG_DDR1_MODE_VAL			0x33
#define CFG_DDR1_RD_DATA_THIS_CYCLE_VAL_16	0xffff
#define CFG_DDR1_RD_DATA_THIS_CYCLE_VAL_32	0xff
#define CFG_DDR1_TAP_VAL			0x20

#define CFG_DDR_CTL_CONFIG			DDR_CTL_CONFIG_SRAM_TSEL_SET(0x1) | \
						DDR_CTL_CONFIG_GE0_SRAM_SYNC_SET(0x1) | \
						DDR_CTL_CONFIG_GE1_SRAM_SYNC_SET(0x1) | \
						DDR_CTL_CONFIG_USB_SRAM_SYNC_SET(0x1) | \
						DDR_CTL_CONFIG_PCIE_SRAM_SYNC_SET(0x1) | \
						DDR_CTL_CONFIG_WMAC_SRAM_SYNC_SET(0x1) | \
						DDR_CTL_CONFIG_MISC_SRC1_SRAM_SYNC_SET(0x1) | \
						DDR_CTL_CONFIG_MISC_SRC2_SRAM_SYNC_SET(0x1)

void
qca955x_init_ddr(void)
{
#if !defined(CONFIG_ATH_NAND_BR) && !defined(CONFIG_ATH_EMULATION)
	int		ddr_config, ddr_config2, ddr_config3, ext_mod, mod_val,
			mod_val_init, cycle_val, tap_val, type, ctl_config;
	uint32_t	bootstrap;

   ANNOUNCE_PROGRESS();
	bootstrap = ath_reg_rd(RST_BOOTSTRAP_ADDRESS);

	switch(type = ath_ram_type(bootstrap)) {
	case ATH_MEM_DDR2:
		ddr_config	= CFG_DDR2_CONFIG_VAL;
		ddr_config2	= CFG_DDR2_CONFIG2_VAL;
		ddr_config3	= CFG_DDR2_CONFIG3_VAL;
		ext_mod		= CFG_DDR2_EXT_MODE_VAL;
		mod_val_init	= CFG_DDR2_MODE_VAL_INIT;
		mod_val		= CFG_DDR2_MODE_VAL;
		tap_val		= CFG_DDR2_TAP_VAL;

		ath_reg_wr_nf(DDR_CONTROL_ADDRESS, 0x10);
		udelay(10);
		ath_reg_wr_nf(DDR_CONTROL_ADDRESS, 0x20);
		udelay(10);

		if (RST_BOOTSTRAP_DDR_WIDTH_GET(bootstrap)) {
			ctl_config =	CFG_DDR_CTL_CONFIG |
					DDR_CTL_CONFIG_PAD_DDR2_SEL_SET(0x1);

			cycle_val = CFG_DDR2_RD_DATA_THIS_CYCLE_VAL_32;
		} else {
			ctl_config =	CFG_DDR_CTL_CONFIG |
					DDR_CTL_CONFIG_PAD_DDR2_SEL_SET(0x1) |
					DDR_CTL_CONFIG_HALF_WIDTH_SET(0x1);

			cycle_val = CFG_DDR2_RD_DATA_THIS_CYCLE_VAL_16;
		}

		ctl_config |= CPU_DDR_SYNC_MODE;

		ath_reg_wr_nf(DDR_CTL_CONFIG_ADDRESS, ctl_config);

		udelay(10);
		break;
	case ATH_MEM_DDR1:
		ddr_config	= CFG_DDR1_CONFIG_VAL;
		ddr_config2	= CFG_DDR1_CONFIG2_VAL;
		ddr_config3	= CFG_DDR1_CONFIG3_VAL;
		ext_mod		= CFG_DDR1_EXT_MODE_VAL;
		mod_val_init	= CFG_DDR1_MODE_VAL_INIT;
		mod_val		= CFG_DDR1_MODE_VAL;
		tap_val		= CFG_DDR1_TAP_VAL;

		if (RST_BOOTSTRAP_DDR_WIDTH_GET(bootstrap)) {
                        ctl_config = CFG_DDR_CTL_CONFIG;
			cycle_val = CFG_DDR1_RD_DATA_THIS_CYCLE_VAL_32;
		} else {
			cycle_val = CFG_DDR1_RD_DATA_THIS_CYCLE_VAL_16;
                        ctl_config = 0;
		}

		ctl_config |= CPU_DDR_SYNC_MODE;

		ath_reg_wr_nf(DDR_CTL_CONFIG_ADDRESS, ctl_config);
		udelay(10);

		break;
	}

	ath_reg_wr_nf(DDR_RD_DATA_THIS_CYCLE_ADDRESS, cycle_val);
	udelay(100);
	ath_reg_wr_nf(DDR_BURST_ADDRESS, 0x74444444);
	udelay(100);
	ath_reg_wr_nf(DDR_BURST2_ADDRESS, 0x44444444);
	udelay(100);
	ath_reg_wr_nf(DDR_AHB_MASTER_TIMEOUT_MAX_ADDRESS, 0xfffff);
	udelay(100);
	ath_reg_wr_nf(DDR_CONFIG_ADDRESS, ddr_config);
	udelay(100);
	ath_reg_wr_nf(DDR_CONFIG2_ADDRESS, ddr_config2);
	udelay(100);
	ath_reg_wr(DDR_CONFIG_3_ADDRESS, ddr_config3);
	udelay(100);

	if (type == ATH_MEM_DDR2) {
		ath_reg_wr_nf(DDR2_CONFIG_ADDRESS, CFG_DDR2_EN_TWL_VAL);
		udelay(100);
	}

	ath_reg_wr_nf(DDR_CONFIG2_ADDRESS, ddr_config2 | 0x80);	// CKE Enable
	udelay(100);

	ath_reg_wr_nf(DDR_CONTROL_ADDRESS, 0x8);	// Precharge
	udelay(10);

	if (type == ATH_MEM_DDR2) {
		ath_reg_wr_nf(DDR_CONTROL_ADDRESS, 0x10);	// EMR2
		udelay(10);
		ath_reg_wr_nf(DDR_CONTROL_ADDRESS, 0x20);	// EMR3
		udelay(10);
	}

	if (type == ATH_MEM_DDR1 || type == ATH_MEM_DDR2) {
		ath_reg_wr_nf(DDR_EXTENDED_MODE_REGISTER_ADDRESS, CFG_DDR2_EXT_MODE_VAL); // EMR DLL enable, Reduced Driver Impedance control, Differential DQS disabled
		udelay(100);
		ath_reg_wr_nf(DDR_CONTROL_ADDRESS, 0x2); // EMR write
		udelay(10);
	}

	ath_reg_wr_nf(DDR_MODE_REGISTER_ADDRESS, mod_val_init);
	udelay(1000);

	ath_reg_wr_nf(DDR_CONTROL_ADDRESS, 0x1);	// MR Write
	udelay(10);

	ath_reg_wr_nf(DDR_CONTROL_ADDRESS, 0x8);	// Precharge
	udelay(10);

	ath_reg_wr_nf(DDR_CONTROL_ADDRESS, 0x4);	// Auto Refresh
	udelay(10);

	ath_reg_wr_nf(DDR_CONTROL_ADDRESS, 0x4);	// Auto Refresh
	udelay(10);

	// Issue MRS to remove DLL out-of-reset
	ath_reg_wr_nf(DDR_MODE_REGISTER_ADDRESS, mod_val);
	udelay(100);

	ath_reg_wr_nf(DDR_CONTROL_ADDRESS, 0x1); // MR write
	udelay(100);

	if (type == ATH_MEM_DDR2) {
		ath_reg_wr_nf(DDR_EXTENDED_MODE_REGISTER_ADDRESS, 0x782);
		udelay(100);

		ath_reg_wr_nf(DDR_CONTROL_ADDRESS, 0x2); // EMR write
		udelay(100);

		ath_reg_wr_nf(DDR_EXTENDED_MODE_REGISTER_ADDRESS, CFG_DDR2_EXT_MODE_VAL);
		udelay(100);

		ath_reg_wr_nf(DDR_CONTROL_ADDRESS, 0x2); // EMR write
		udelay(100);
	}

	ath_reg_wr_nf(DDR_REFRESH_ADDRESS, CFG_DDR_REFRESH_VAL);
	udelay(100);

        ath_reg_wr(TAP_CONTROL_0_ADDRESS, tap_val);
	ath_reg_wr(TAP_CONTROL_1_ADDRESS, tap_val);

	if (RST_BOOTSTRAP_DDR_WIDTH_GET(bootstrap)) {
		ath_reg_wr (TAP_CONTROL_2_ADDRESS, tap_val);
		ath_reg_wr (TAP_CONTROL_3_ADDRESS, tap_val);
	}

	if (type == ATH_MEM_DDR2) {
		ath_reg_wr(PMU1_ADDRESS, 0x633c8176);
		// Set DDR2 Voltage to 1.8 volts
		ath_reg_wr(PMU2_ADDRESS, PMU2_LDO_TUNE_SET(3) |
					 PMU2_PGM_SET(0x1));
	}

	/*
         * Based on SGMII validation for stucks, packet errors were  observed and it was
         * mostly due to noise pickup on SGMII lines. Switching regulator register is to
         * be programmed with proper setting to avoid such stucks.
	 */
	ath_reg_rmw_clear(PMU1_ADDRESS, (7<<1));
	ath_reg_rmw_set(PMU1_ADDRESS, (1<<3));

   ANNOUNCE_OK();
	return;
#else	// !nand flash and !emulation
	return;
#endif
}

static int
ath_local_write_config(int where, int size, uint32_t value)
{
   ath_reg_wr((ATH_PCI_CRP + where),value);
   return 0;
}

#ifdef PCIE2_APP_ADDRESS
static int
ath_local_write_config_rc2(int where, int size, uint32_t value)
{
   ath_reg_wr((0x18250000 + where),value);
   return 0;
}

void
pci_rc2_init_board (void)
{
   uint32_t cmd;

   ath_reg_rmw_set(RST_RESET2_ADDRESS, RST_RESET2_PCIE2_PHY_RESET_SET(1));
   udelay(10000);

   ath_reg_rmw_set(RST_RESET2_ADDRESS, RST_RESET2_PCIE2_RESET_SET(1));
   udelay(10000);

   ath_reg_rmw_clear(RST_MISC2_ADDRESS, RST_MISC2_PERSTN_RCPHY2_SET(1));
   udelay(10000);

   ath_reg_wr_nf(PCIE2_RESET_ADDRESS, 0); // Put endpoint in reset
   udelay(100000);

   ath_reg_rmw_set(RST_MISC2_ADDRESS, RST_MISC2_PERSTN_RCPHY2_SET(1));
   udelay(10000);

   ath_reg_rmw_clear(RST_RESET2_ADDRESS, RST_RESET_PCIE_PHY_RESET_SET(1));
   udelay(10000);

   ath_reg_rmw_clear(RST_RESET2_ADDRESS, RST_RESET_PCIE_RESET_SET(1));
   udelay(10000);

   ath_reg_wr_nf(PCIE2_APP_ADDRESS, PCIE2_APP_PCIE2_BAR_MSN_SET(1) |
               PCIE2_APP_CFG_BE_SET(0xf) |
               PCIE2_APP_SLV_RESP_ERR_MAP_SET(0x3f) |
               PCIE2_APP_LTSSM_ENABLE_SET(1));

   cmd = PCI_COMMAND_MEMORY | PCI_COMMAND_MASTER | PCI_COMMAND_INVALIDATE |
      PCI_COMMAND_PARITY | PCI_COMMAND_SERR | PCI_COMMAND_FAST_BACK;

   ath_local_write_config_rc2(PCI_COMMAND, 4, cmd);
   ath_local_write_config_rc2(0x20, 4, 0x1ff01000);
   ath_local_write_config_rc2(0x24, 4, 0x1ff01000);

   ath_reg_wr_nf(PCIE2_RESET_ADDRESS, 4); // Pull endpoint out of reset
   udelay(100000);

   /*
    * Check if the WLAN PCI-E H/W is present, If the
    * WLAN H/W is not present, skip the PCI platform
    * initialization code and return
    */
   if (((ath_reg_rd(PCIE2_RESET_ADDRESS)) & 0x1) == 0x0) {
      //prmsg("*** Warning *** : PCIe WLAN Module not found !!!\n");
      return;
   }
}
#endif

/******************************************************************************/
/*!
** \brief pci host initialization
**
** Sets up the PCI controller on the host. For AR7240 this may not be necessary,
** but this function is required for board support.
**
** We want a 1:1 mapping between PCI and DDR for inbound and outbound.
** The PCI<---AHB decoding works as follows:
**
** 8 registers in the DDR unit provide software configurable 32 bit offsets
** for each of the eight 16MB PCI windows in the 128MB. The offsets will be
** added to any address in the 16MB segment before being sent to the PCI unit.
**
** Essentially for any AHB address generated by the CPU,
** 1. the MSB four bits are stripped off, [31:28],
** 2. Bit 27 is used to decide between the lower 128Mb (PCI) or the rest of
**    the AHB space
** 3. Bits 26:24 are used to access one of the 8 window registers and are
**    masked off.
** 4. If it is a PCI address, then the WINDOW offset in the WINDOW register
**    corresponding to the next 3 bits (bit 26:24) is ADDED to the address,
**    to generate the address to PCI unit.
**
**     eg. CPU address = 0x100000ff
**         window 0 offset = 0x10000000
**         This points to lowermost 16MB window in PCI space.
**         So the resulting address would be 0x000000ff+0x10000000
**         = 0x100000ff
**
**         eg2. CPU address = 0x120000ff
**         WINDOW 2 offset = 0x12000000
**         resulting address would be 0x000000ff+0x12000000
**                         = 0x120000ff
**
** There is no translation for inbound access (PCI device as a master)
**
**  \return N/A
*/
void qca955x_init_pcie_plls(void)
{
   uint32_t cmd;

   // common for rc1 and rc2
   ath_reg_wr_nf(PCIE_PLL_DITHER_DIV_MAX_ADDRESS,
      PCIE_PLL_DITHER_DIV_MAX_EN_DITHER_SET(0x1) |
      PCIE_PLL_DITHER_DIV_MAX_USE_MAX_SET(0x1) |
      PCIE_PLL_DITHER_DIV_MAX_DIV_MAX_INT_SET(0x14) |
      PCIE_PLL_DITHER_DIV_MAX_DIV_MAX_FRAC_SET(0x3ff));

   ath_reg_wr_nf(PCIE_PLL_DITHER_DIV_MIN_ADDRESS,
      PCIE_PLL_DITHER_DIV_MIN_DIV_MIN_INT_SET(0x14));

   ath_reg_wr_nf(PCIE_PLL_CONFIG_ADDRESS,
      PCIE_PLL_CONFIG_REFDIV_SET(1) |
      PCIE_PLL_CONFIG_BYPASS_SET(1) |
      PCIE_PLL_CONFIG_PLLPWD_SET(1));
   udelay(10000);

   ath_reg_rmw_clear(PCIE_PLL_CONFIG_ADDRESS, PCIE_PLL_CONFIG_PLLPWD_SET(1));
   udelay(1000);
   ath_reg_rmw_clear(PCIE_PLL_CONFIG_ADDRESS, PCIE_PLL_CONFIG_BYPASS_SET(1));
   udelay(1000);

#ifdef PCIE2_APP_ADDRESS
   if (!(ath_reg_rd(RST_BOOTSTRAP_ADDRESS) & RST_BOOTSTRAP_PCIE_RC_EP_SELECT_MASK)) {
      pci_rc2_init_board();
      return;
   }
#endif

   ath_reg_rmw_set(RST_RESET_ADDRESS, RST_RESET_PCIE_PHY_RESET_SET(1));
   udelay(10000);

   ath_reg_rmw_set(RST_RESET_ADDRESS, RST_RESET_PCIE_RESET_SET(1));
   udelay(10000);

#ifdef PCIE2_APP_ADDRESS
   ath_reg_rmw_clear(RST_MISC2_ADDRESS, RST_MISC2_PERSTN_RCPHY_SET(1));
   udelay(10000);
#endif

   ath_reg_wr_nf(PCIE_RESET_ADDRESS, 0);  // Put endpoint in reset
   udelay(100000);

#ifdef PCIE2_APP_ADDRESS
   ath_reg_rmw_set(RST_MISC2_ADDRESS, RST_MISC2_PERSTN_RCPHY_SET(1));
   udelay(10000);
#endif

   ath_reg_rmw_clear(RST_RESET_ADDRESS, RST_RESET_PCIE_PHY_RESET_SET(1));
   udelay(10000);

   ath_reg_rmw_clear(RST_RESET_ADDRESS, RST_RESET_PCIE_RESET_SET(1));
   udelay(10000);

   ath_reg_wr_nf(PCIE_APP_ADDRESS, PCIE_APP_PCIE_BAR_MSN_SET(1) |
               PCIE_APP_CFG_BE_SET(0xf) |
               PCIE_APP_SLV_RESP_ERR_MAP_SET(0x3f) |
               PCIE_APP_LTSSM_ENABLE_SET(1));

   cmd = PCI_COMMAND_MEMORY | PCI_COMMAND_MASTER | PCI_COMMAND_INVALIDATE |
      PCI_COMMAND_PARITY | PCI_COMMAND_SERR | PCI_COMMAND_FAST_BACK;

   ath_local_write_config(PCI_COMMAND, 4, cmd);
   ath_local_write_config(0x20, 4, 0x1ff01000);
   ath_local_write_config(0x24, 4, 0x1ff01000);

   ath_reg_wr_nf(PCIE_RESET_ADDRESS, 4);  // Pull endpoint out of reset
   udelay(100000);

   /*
    * Check if the WLAN PCI-E H/W is present, If the
    * WLAN H/W is not present, skip the PCI platform
    * initialization code and return
    */
   if (((ath_reg_rd(PCIE_RESET_ADDRESS)) & 0x1) == 0x0) {
      //prmsg("*** Warning *** : PCIe WLAN Module not found !!!\n");
   }

#ifdef PCIE2_APP_ADDRESS
   pci_rc2_init_board();
#endif

   ath_reg_rmw_clear ( GPIO_OUT_FUNCTION1_ADDRESS, GPIO_OUT_FUNCTION1_ENABLE_GPIO_4_MASK);
}

void
show_soc_banner(void)
{
    ANNOUNCE_LITERAL("\n\n\nMeraki Atheros LinuxLoader MR18 built " __DATE__ " " __TIME__ "\n");
}
