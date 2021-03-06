//macros taken from atheros code drop, u-boot/include/ar934x_soc.h
#define CPU_PLL_CONFIG_NINT_VAL_40              CPU_PLL_CONFIG_NINT_SET(14)
#define CPU_PLL_CONFIG_REF_DIV_VAL              CPU_PLL_CONFIG_REFDIV_SET(1)
#define CPU_PLL_CONFIG_RANGE_VAL                CPU_PLL_CONFIG_RANGE_SET(1)
#define CPU_PLL_CONFIG_OUT_DIV_VAL1             CPU_PLL_CONFIG_OUTDIV_SET(1)
#define CPU_PLL_CONFIG_OUT_DIV_VAL2             CPU_PLL_CONFIG_OUTDIV_SET(0)
#define CPU_PLL_CONFIG_NFRAC                    CPU_PLL_DITHER_NFRAC_MIN_SET(0) | CPU_PLL_DITHER_NFRAC_MAX_SET(0)

#define DDR_PLL_CONFIG_NINT_VAL_40              DDR_PLL_CONFIG_NINT_SET(12)
#define DDR_PLL_CONFIG_REF_DIV_VAL              DDR_PLL_CONFIG_REFDIV_SET(1)
#define DDR_PLL_CONFIG_RANGE_VAL                DDR_PLL_CONFIG_RANGE_SET(1)
#define DDR_PLL_CONFIG_OUT_DIV_VAL1             DDR_PLL_CONFIG_OUTDIV_SET(1)
#define DDR_PLL_CONFIG_OUT_DIV_VAL2             DDR_PLL_CONFIG_OUTDIV_SET(0)
#define DDR_PLL_CONFIG_NFRAC                    DDR_PLL_DITHER_NFRAC_MIN_SET(0) | DDR_PLL_DITHER_NFRAC_MAX_SET(0)

#define CPU_PLL_NFRAC_MIN_SET                   CPU_PLL_DITHER_NFRAC_MIN_SET(20) | CPU_PLL_DITHER_NFRAC_MAX_SET(20)

#define CPU_DDR_CLOCK_CONTROL_AHB_DIV_VAL	CPU_DDR_CLOCK_CONTROL_AHB_POST_DIV_SET(1)
#define CPU_DDR_CLOCK_CONTROL_AHB_CLK_DDR	CPU_DDR_CLOCK_CONTROL_AHBCLK_FROM_DDRPLL_SET(1)
#define CPU_DDR_CLOCK_CONTROL_DDR_CLK_DDR	CPU_DDR_CLOCK_CONTROL_DDRCLK_FROM_DDRPLL_SET(1)
#define CPU_DDR_CLOCK_CONTROL_CPU_CLK_CPU	CPU_DDR_CLOCK_CONTROL_CPUCLK_FROM_CPUPLL_SET(1)
#define CPU_DDR_CLOCK_CONTROL_DDR_POST_DIV	CPU_DDR_CLOCK_CONTROL_DDR_POST_DIV_SET(0)
#define CPU_DDR_CLOCK_CONTROL_CPU_POST_DIV	CPU_DDR_CLOCK_CONTROL_CPU_POST_DIV_SET(0)
