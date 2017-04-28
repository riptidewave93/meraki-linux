#include <linux/serial_reg.h>
#include <linux/smp.h>
#include <asm/addrspace.h>
#include <asm/byteorder.h>
#include <asm/page.h>
#include <asm/mipsregs.h>
#include <asm/r4kcache.h>
#include <asm/cacheops.h>
#include <asm/mach-ath79/ath79.h>

#include "config.h"
#include "934x.h"
#include "nandloader_common.h"

/*
 *  Initialize the ddr controller.  This code is borrowed from u-boot. It is
 *  the u-boot code with all the non-gluon cases removed.
 */
void
ar934x_init_ddr(void)
{
    int ddr_config, ddr_config2, ext_mod, mod_val,
        mod_val_init, cycle_val, tap_val, reg32;

#define CFG_934X_DDR2_CONFIG_VAL        0xc7d48cd0
#define CFG_934X_DDR2_MODE_VAL_INIT     0x133
#define CFG_934X_DDR2_EXT_MODE_VAL_INIT 0x382
#define CFG_934X_DDR2_EXT_MODE_VAL      0x402
#define CFG_934X_DDR2_MODE_VAL          0x33
#define CFG_934X_DDR2_CONFIG2_VAL       0x9dd0e6a8
#define CFG_934X_DDR2_EN_TWL_VAL        0xe59
#define CFG_934X_DDR2_TAP_VAL           0x10012
#define CFG_DDR2_RD_DATA_THIS_CYCLE_VAL 0xff

    ANNOUNCE_PROGRESS();

    ddr_config          = CFG_934X_DDR2_CONFIG_VAL;
    ddr_config2         = CFG_934X_DDR2_CONFIG2_VAL;
    ext_mod             = CFG_934X_DDR2_EXT_MODE_VAL;
    mod_val_init        = CFG_934X_DDR2_MODE_VAL_INIT;
    mod_val             = CFG_934X_DDR2_MODE_VAL;
    cycle_val           = CFG_DDR2_RD_DATA_THIS_CYCLE_VAL;
    tap_val             = CFG_934X_DDR2_TAP_VAL;

    ath_reg_wr_nf(ATH_DDR_DDR2_CONFIG, CFG_934X_DDR2_EN_TWL_VAL);
    udelay(100);
    ath_reg_wr_nf(ATH_DDR_CONTROL, 0x10);
    udelay(10);
    ath_reg_wr_nf(ATH_DDR_CONTROL, 0x20);
    udelay(10);
    ath_reg_wr_nf(ATH_DDR_CTL_CONFIG, (1 << 6));
    udelay(10);

    ath_reg_wr_nf(ATH_DDR_CONFIG, ddr_config);
    udelay(100);
    ath_reg_wr_nf(ATH_DDR_CONFIG2, ddr_config2 | 0x80);
    udelay(100);
    ath_reg_wr_nf(ATH_DDR_CONTROL, 0x8);
    udelay(10);

    ath_reg_wr_nf(ATH_DDR_MODE, mod_val_init);
    udelay(1000);

    ath_reg_wr_nf(ATH_DDR_CONTROL, 0x1);
    udelay(10);

    ath_reg_wr_nf(ATH_DDR_EXT_MODE, CFG_934X_DDR2_EXT_MODE_VAL_INIT);
    udelay(100);
    ath_reg_wr_nf(ATH_DDR_CONTROL, 0x2);
    udelay(10);
    ath_reg_wr_nf(ATH_DDR_EXT_MODE, ext_mod);

    udelay(100);
    ath_reg_wr_nf(ATH_DDR_CONTROL, 0x2);
    udelay(10);
    ath_reg_wr_nf(ATH_DDR_CONTROL, 0x8);
    udelay(10);
    ath_reg_wr_nf(ATH_DDR_MODE, mod_val);
    udelay(100);
    ath_reg_wr_nf(ATH_DDR_CONTROL, 0x1);
    udelay(10);
    ath_reg_wr_nf(ATH_DDR_REFRESH, 0x4270);
    udelay(100);

    ath_reg_wr (ATH_DDR_TAP_CONTROL0, tap_val);
    ath_reg_wr (ATH_DDR_TAP_CONTROL1, tap_val);
    ath_reg_wr (ATH_DDR_TAP_CONTROL2, tap_val);
    ath_reg_wr (ATH_DDR_TAP_CONTROL3, tap_val);

    ath_reg_wr_nf(ATH_DDR_RD_DATA_THIS_CYCLE, cycle_val);
    udelay(100);
    ath_reg_wr_nf(ATH_DDR_BURST, 0x74444444);
    udelay(100);
    ath_reg_wr_nf(ATH_DDR_BURST2, 0x222);
    udelay(100);
    ath_reg_wr_nf(ATH_AHB_MASTER_TIMEOUT, 0xfffff);
    udelay(100);

    reg32 = ath_reg_rd(ATH_RESET);
    reg32 = reg32 &  ~(1 << 27);
    ath_reg_wr_nf(ATH_RESET, reg32);

    ANNOUNCE_OK();
}

void
ar934x_init_usb_phy(void)
{
    ANNOUNCE_PROGRESS();
    ath_reg_rmw_set(ATH_RESET, RST_RESET_USB_PHY_SUSPEND_OVERRIDE_SET(1));
    udelay(1000);

    ath_reg_rmw_clear(ATH_RESET, RST_RESET_USB_HOST_RESET_SET(1));
    udelay(1000);

    ath_reg_rmw_clear(ATH_RESET, RST_RESET_USB_PHY_RESET_SET(1));
    udelay(1000);

    ath_reg_rmw_clear(ATH_RESET, RST_RESET_USB_PHY_ARESET_SET(1));
    ANNOUNCE_OK();
}

void
ar934x_init_pcie_plls(void)
{
    uint32_t cmd = 0;

    ANNOUNCE_PROGRESS();
    if (!(ath_reg_rd(ATH_BOOTSTRAP_REG) & ATH_REF_CLK_40)) {
        ath_reg_wr_nf(AR934X_PCIE_PLL_DITHER_DIV_MAX,
                      PCIE_PLL_DITHER_DIV_MAX_EN_DITHER_SET(0) |
                      PCIE_PLL_DITHER_DIV_MAX_USE_MAX_SET(1) |
                      PCIE_PLL_DITHER_DIV_MAX_DIV_MAX_INT_SET(0x20) |
                      PCIE_PLL_DITHER_DIV_MAX_DIV_MAX_FRAC_SET(0));
    }

    ath_reg_rmw_set(ATH_RESET, RST_RESET_PCIE_RESET_MASK); // core in reset
    udelay(10000);
    ath_reg_rmw_set(ATH_RESET, RST_RESET_PCIE_PHY_RESET_MASK);// phy in reset
    udelay(10000);
    ath_reg_rmw_clear(RST_MISC2_ADDRESS, RST_MISC2_PERSTN_RCPHY_SET(1)); // pci phy analog in reset
    udelay(10000);
    ath_reg_wr(0x180f0000, 0x1ffc0); // ltssm is disabled
    udelay(100);
    ath_reg_wr_nf(ATH_PCI_LCL_RESET, 0); // End point in reset
    udelay(100000);

    if ((ath_reg_rd(ATH_REV_ID) & 0xf) == 0) {
        ath_reg_wr_nf(AR934X_PCIE_PLL_CONFIG,
                      PCIE_PLL_CONFIG_REFDIV_SET(1) |
                      PCIE_PLL_CONFIG_BYPASS_SET(1) |
                      PCIE_PLL_CONFIG_PLLPWD_SET(1));
        udelay(10000);
        ath_reg_wr_nf(AR934X_PCIE_PLL_CONFIG,
                      PCIE_PLL_CONFIG_REFDIV_SET(1) |
                      PCIE_PLL_CONFIG_BYPASS_SET(1) |
                      PCIE_PLL_CONFIG_PLLPWD_SET(0));
        udelay(1000);
        ath_reg_wr_nf(AR934X_PCIE_PLL_CONFIG,
                      ath_reg_rd(AR934X_PCIE_PLL_CONFIG) &
                      (~PCIE_PLL_CONFIG_BYPASS_SET(1)));
        udelay(1000);
    } else {
        ath_reg_wr_nf(AR934X_PCIE_PLL_CONFIG,
                      PCIE_PLL_CONFIG_REFDIV_SET(2) |
                      PCIE_PLL_CONFIG_BYPASS_SET(1) |
                      PCIE_PLL_CONFIG_PLLPWD_SET(1));
        udelay(10000);

        if (!(ath_reg_rd(ATH_BOOTSTRAP_REG) & ATH_REF_CLK_40)) {
            ath_reg_wr_nf(0xb8116c00, (0x5 << 27) | (160 << 18) | 0);
        } else {
            ath_reg_wr_nf(0xb8116c00, (0x2 << 27) | (0x28 << 18) | 0);
        }
        do {
            ath_reg_wr_nf(0xb8116c04, (0x1 << 30) | (0x4 << 26) | (0x32 << 19) | (1 << 16) | (3 << 13) | (0x1e << 7));
            ath_reg_wr_nf(0xb8116c08, (6 << 23));
            udelay(10000);
            ath_reg_wr_nf(0xb8116c04, (0x1 << 30) | (0x4 << 26) | (0x32 << 19) | (3 << 13) | (0x1e << 7));

            ath_reg_rmw_clear(KSEG1ADDR(PCIe_DPLL3_ADDRESS), PCIe_DPLL3_DO_MEAS_SET(1));
            ath_reg_rmw_set(KSEG1ADDR(PCIe_DPLL3_ADDRESS), PCIe_DPLL3_DO_MEAS_SET(1));

            ath_reg_wr(0xb804000c, 1 << 2);

            udelay(1000);

            while (((cmd = ath_reg_rd(PCIe_DPLL4_ADDRESS)) & PCIe_DPLL4_MEAS_DONE_SET(1)) == 0) {
                udelay(10);
            }

            udelay(10000);

        } while ((cmd = PCIe_DPLL3_SQSUM_DVC_GET(ath_reg_rd(PCIe_DPLL3_ADDRESS))) >= 0x40000);

        ath_reg_rmw_clear(AR934X_PCIE_PLL_CONFIG, PCIE_PLL_CONFIG_PLLPWD_SET(1));
        udelay(10000);
        ath_reg_rmw_clear(AR934X_PCIE_PLL_CONFIG, PCIE_PLL_CONFIG_BYPASS_SET(1));
        udelay(10000);
    }
    ath_reg_rmw_set(RST_MISC2_ADDRESS, RST_MISC2_PERSTN_RCPHY_SET(1)); // pci phy analog out of reset
    udelay(10000);

    ath_reg_rmw_clear(ATH_RESET, RST_RESET_PCIE_PHY_RESET_MASK); // phy out of reset
    udelay(10000);

    ath_reg_rmw_clear(ATH_RESET, RST_RESET_PCIE_RESET_MASK); // core out of reset
    udelay(1000);

    if ((soc_is_ar7241() || soc_is_ar7242() || is_wasp())) {
        ath_reg_wr(0x180f0000, 0x1ffc1); // ltssm enable
    } else {
        ath_reg_wr(0x180f0000, 0x1);
    }
    udelay(100000);

    ath_reg_wr_nf(ATH_PCI_LCL_RESET, 4); // EP out of reset
    ANNOUNCE_OK();
}

void
show_soc_banner(void)
{
    ANNOUNCE_LITERAL("\n\n\nMeraki Atheros LinuxLoader built " __DATE__ " " __TIME__ "\n");
}
