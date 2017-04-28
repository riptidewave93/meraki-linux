
#include <linux/serial_reg.h>
#include <linux/smp.h>
#include <asm/addrspace.h>
#include <asm/byteorder.h>
#include <asm/page.h>
#include <asm/mipsregs.h>
#include <asm/r4kcache.h>
#include <asm/cacheops.h>

#include <atheros.h>

#define ATH_DDR_CTL_CONFIG          ATH_DDR_CTL_BASE+0x108
#define ATH_DDR_DDR2_CONFIG         ATH_DDR_CTL_BASE+0xb8
#define ATH_DDR_BURST               ATH_DDR_CTL_BASE+0xc4
#define ATH_DDR_BURST2              ATH_DDR_CTL_BASE+0xc8
#define ATH_AHB_MASTER_TIMEOUT      ATH_DDR_CTL_BASE+0xcc

#define ATH_DDR_CTL_CONFIG_HALF_WIDTH (1 << 1)

#define ATH_DDR2_CONFIG_DDR2_EN        1

#define ATH_DDR_CONTROL_EMR3S         (1 << 5)
#define ATH_DDR_CONTROL_EMR2S         (1 << 4)
#define ATH_DDR_CONTROL_PREA          (1 << 3)
#define ATH_DDR_CONTROL_REF           (1 << 2)
#define ATH_DDR_CONTROL_EMRS          (1 << 1)
#define ATH_DDR_CONTROL_MRS           (1 << 0)

#define ATH_DDR_REFRESH_ENABLE        (1 << 14)
#define ATH_DDR_REFRESH_PERIOD        (0x3fff)

extern int printf(const char *fmt, ...);
extern void ar934x_lowlevel_init(void);

struct _uart_ptr {
	volatile uint32_t rbr_thr;
	volatile uint32_t ier;
	volatile uint32_t iir_fcr;
	volatile uint32_t lcr;
	volatile uint32_t mcr;
	volatile uint32_t lsr;
	volatile uint32_t msr;
	volatile uint32_t scr;
	volatile uint32_t usr;
};

#define UART_BASE ((struct _uart_ptr *)(KSEG1ADDR(ATH_UART_BASE)))

#define ANNOUNCE_LITERAL(str) do { static char strptr[] = {str}; uart_puts(strptr); } while (0);
#define ANNOUNCE_PROGRESS() do { uart_puts(__func__); } while (0);
#define ANNOUNCE_OK() ANNOUNCE_LITERAL(" ok\n")

u32  ref_clock_rate = 0;

static inline void
init_xtimer(void)
{
    if ((ath_reg_rd(ATH_BOOTSTRAP_REG) & ATH_REF_CLK_40)) {
        ref_clock_rate = (40 * 1000000);
    } else {
        ref_clock_rate = (25 * 1000000);
    }
}

static inline u32
mips_count_get(void)
{
    u32 count;
    asm volatile ("mfc0 %0, $9" : "=r" (count) :);
    return count;
}

void
udelay(unsigned long usec)
{
    ulong tmo;
    ulong start = mips_count_get();

    tmo = usec * (ref_clock_rate / 1000000);
    while ((ulong)((mips_count_get() - start)) < tmo)
        /*NOP*/;
}


static inline void
init_uart(void)
{
    u32 div = (ref_clock_rate) / (16 * 115200);

    UART_BASE->lcr = UART_LCR_DLAB;
    UART_BASE->rbr_thr = div & 0xff;
    UART_BASE->ier = div >> 8;
    UART_BASE->lcr = UART_LCR_WLEN8;
}

static void
uart_putc(char c)
{
    if (c == '\n')
	uart_putc('\r');	/* CR before LF, I mean handle newline */

    while ((UART_BASE->lsr & UART_LSR_THRE) == 0) ;

    UART_BASE->rbr_thr = c;
}

static void
uart_puts(const char *str)
{
    while (*str)
    uart_putc(*str++);
}

/*
 * Just walk memory 4MB at a time until we run wrap around
 */
uint32_t
ddr_find_size(void)
{
#define DDR_SIZE_INCR (4*1024*1024)

    uint8_t *p = (uint8_t *)KSEG1, pat = 0x77;
    uint32_t i;

    *p = pat;

    for (i = 1; ; i++) {
        /*
         * When we finally reach the end of RAM we wrap back to the
         * beginnig.  Thus, (p + i * DDR_SIZE_INCR) and p are
         * referencing the same piece of memory.
         */
        *(p + i * DDR_SIZE_INCR) = (uint8_t)(i);
        if (*p != pat) {
            break;
        }
    }

    return (i*DDR_SIZE_INCR);
}

/*
 *  Initialize the ddr controller.  This code is borrowed from u-boot. It is
 *  the u-boot code with all the non-gluon cases removed.
 */
static void
init_ddr(void)
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

//given an int containing the (I|D)S (I|D)L (I|D)A bits in the least significant positions, return the size
static inline void
get_cache_size(u_int32_t *isize, u_int32_t *dsize)
{
    u_int32_t conf1 = read_c0_config1();

    int ia = (conf1 & MIPS_CONF1_IA) >> 16,
        il = (conf1 & MIPS_CONF1_IL) >> 19,
        is = (conf1 & MIPS_CONF1_IS) >> 22,
        da = (conf1 & MIPS_CONF1_DA) >> 7,
        dl = (conf1 & MIPS_CONF1_DL) >> 10,
        ds = (conf1 & MIPS_CONF1_DS) >> 13;

    (*isize) = (64 << is) * (2 << il) * (ia + 1);
    (*dsize) = (64 << ds) * (2 << dl) * (da + 1);
}

static inline void
init_dram_uncached(u_int32_t *uncached_ram, u_int32_t cache_size)
{
    register u_int32_t *addr;

    ANNOUNCE_PROGRESS();

    for (addr = uncached_ram; addr < uncached_ram + cache_size; addr += 16) {
        addr[0] = 0;
        addr[1] = 0;
        addr[2] = 0;
        addr[3] = 0;
        addr[4] = 0;
        addr[5] = 0;
        addr[6] = 0;
        addr[7] = 0;
        addr[8] = 0;
        addr[9] = 0;
        addr[10] = 0;
        addr[11] = 0;
        addr[12] = 0;
        addr[13] = 0;
        addr[14] = 0;
        addr[15] = 0;
    }

    ANNOUNCE_OK();
}


static inline void
init_icache(u_int32_t *cached_ram, u_int32_t cache_size, u_int32_t line_size)
{
    register u_int32_t *addr;

    ANNOUNCE_PROGRESS();

    write_c0_taglo(0);       /* Ensure valid bit clear and parity consistent */

    for (addr = cached_ram; addr < cached_ram + cache_size; addr += line_size) {
        cache_op(Index_Store_Tag_I, addr); /* Clear tag to invalidate */
        cache_op(Fill, addr);              /* Fill so parity is correct */
        cache_op(Index_Store_Tag_I, addr); /* Invalidate to be safe */
    }

    ANNOUNCE_OK();
}

static inline void
init_dcache(u_int32_t *cached_ram, u_int32_t cache_size, u_int32_t line_size)
{
    register u_int32_t *addr;
    register u_int32_t dummy;

    ANNOUNCE_PROGRESS();

    write_c0_taglo(0);       /* Ensure valid bit clear and parity consistent */

    for (addr = cached_ram; addr < cached_ram + cache_size; addr += line_size) {
        cache_op(Index_Store_Tag_D, addr); /* Clear tag to invalidate */
    }

    for (addr = cached_ram; addr < cached_ram + cache_size; addr += line_size) {
        dummy = *(volatile u_int32_t *)addr; /* Read from each cache line */
    }

    for (addr = cached_ram; addr < cached_ram + cache_size; addr += line_size) {
        cache_op(Index_Store_Tag_D, addr); /* Invalidate to be safe */
    }

    ANNOUNCE_OK();
}

static inline void
enable_caches(void)
{
    u_int32_t config;

    ANNOUNCE_PROGRESS();

    config = read_c0_config();
    config = (config & ~CONF_CM_CMASK) | CONF_CM_CACHABLE_NONCOHERENT;
    write_c0_config(config);

    ANNOUNCE_OK();
}

static inline void
init_caches(void)
{
    u_int32_t isize, dsize;

    get_cache_size(&isize, &dsize);
    printf("D-cache size: %dK\n", isize >> 10);
    printf("I-cache size: %dK\n", dsize >> 10);

    init_dram_uncached((u_int32_t *)KSEG1, isize > dsize ? isize : dsize);
    init_icache((u_int32_t *)KSEG1, isize, 32/(sizeof (u_int32_t)));
    init_dcache((u_int32_t *)KSEG1, dsize, 32/(sizeof (u_int32_t)));
    enable_caches();
}

static inline void
init_usb_phy(void)
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

static inline void
init_pcie_plls(void)
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

    if ((is_ar7241() || is_ar7242() || is_wasp())) {
        ath_reg_wr(0x180f0000, 0x1ffc1); // ltssm enable
    } else {
        ath_reg_wr(0x180f0000, 0x1);
    }
    udelay(100000);

    ath_reg_wr_nf(ATH_PCI_LCL_RESET, 4); // EP out of reset
    ANNOUNCE_OK();
}

extern int nand_flash_init(void);
extern int nand_read(unsigned int, unsigned int, uint8_t *);
extern int nand_load_fw(void);
extern void printf_init(void(*)(char c));

static int
test_memory(void* addr)
{
    volatile register u_int32_t *ram = (volatile uint32_t *)addr;
    register int count;

    ANNOUNCE_PROGRESS();
    ram[0] = 0;
    ram[1] = ~0;
    ram[2] = 0x5a5a5a5a;
    ram[3] = 0xa5a5a5a5;
    ram[4] = 0xfffefffe;

#define RAMCHECK(off, val) do { if (ram[off] != val) { printf("failed RAM BORKED: (%p) 0x%x != 0x%x\n", &ram[off], ram[off], val); return -1; } } while(0)

    for (count = 0; count < 5; count++) {
        RAMCHECK(0, 0);
        RAMCHECK(1, ~0);
        RAMCHECK(2, 0x5a5a5a5a);
        RAMCHECK(3, 0xa5a5a5a5);
        RAMCHECK(4, 0xfffefffe);
    }

    ANNOUNCE_OK();
    return 0;
}

void
loader_entry(void)
{
    init_xtimer();
    init_uart();
    printf_init(uart_putc);
    ANNOUNCE_LITERAL("\n\n\nMeraki Atheros LinuxLoader built " __DATE__ " " __TIME__ "\n");

    ar934x_lowlevel_init();

    init_ddr();
    if (test_memory((void*)KSEG1))
        goto errout;

    init_caches();
    if (test_memory((void*)KSEG0))
        goto errout;

    //initialize the malloc base.  Reserve the last meg for phram stuff
    malloc_base = (uint8_t*)(KSEG0 + ddr_find_size() - (1024*1024));
    malloc_brk = malloc_base;

    //if the usb phys is still in reset, nand writes fail
    init_usb_phy();
    //initialize pcie
    init_pcie_plls();

    ANNOUNCE_LITERAL("nand_flash_init");
    if(nand_flash_init())
        goto errout;

    ANNOUNCE_LITERAL(" ok\n");

    nand_load_fw();

 errout:
    printf("error booting\n");
    while (1);

}
