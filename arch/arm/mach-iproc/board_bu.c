/*
 * Copyright (C) 2013, Broadcom Corporation. All Rights Reserved.
 * 
 * Permission to use, copy, modify, and/or distribute this software for any
 * purpose with or without fee is hereby granted, provided that the above
 * copyright notice and this permission notice appear in all copies.
 * 
 * THE SOFTWARE IS PROVIDED "AS IS" AND THE AUTHOR DISCLAIMS ALL WARRANTIES
 * WITH REGARD TO THIS SOFTWARE INCLUDING ALL IMPLIED WARRANTIES OF
 * MERCHANTABILITY AND FITNESS. IN NO EVENT SHALL THE AUTHOR BE LIABLE FOR ANY
 * SPECIAL, DIRECT, INDIRECT, OR CONSEQUENTIAL DAMAGES OR ANY DAMAGES
 * WHATSOEVER RESULTING FROM LOSS OF USE, DATA OR PROFITS, WHETHER IN AN ACTION
 * OF CONTRACT, NEGLIGENCE OR OTHER TORTIOUS ACTION, ARISING OUT OF OR IN
 * CONNECTION WITH THE USE OR PERFORMANCE OF THIS SOFTWARE.
 */

#include <linux/version.h>
#include <linux/init.h>
#include <linux/device.h>
#include <linux/platform_device.h>
#include <linux/interrupt.h>
#include <linux/dma-mapping.h>
#include <linux/clkdev.h>
#include <asm/hardware/gic.h>


#include <linux/i2c.h>
#include <linux/delay.h>
#include <linux/irq.h>
#include <linux/input.h>
#include <linux/i2c/tsc2007.h>
#include <linux/spi/spi.h>
#include <mach/hardware.h>
#include <asm/mach/arch.h>
#include <asm/mach-types.h>
#include <mach/sdio_platform.h>
#include <mach/iproc.h>
#include <mach/common.h>
#include <asm/io.h>
#include <mach/io_map.h>
#include <mach/reg_utils.h>
#include <linux/pwm.h>
#include <linux/amba/bus.h>


#include "northstar.h"
#include "common.h"

#if defined(CONFIG_IPROC_SD) || defined(CONFIG_IPROC_SD_MODULE)
#define IPROC_SDIO_PA   	IPROC_SDIO3_REG_BASE
#define SDIO_CORE_REG_SIZE	0x10000
#define BSC_CORE_REG_SIZE	0x1000
#define SDIO_IDM_IDM_RESET_CONTROL  (0x16800)
#define IPROC_SDIO_IRQ		(177)
#endif

#if defined(CONFIG_MACH_NSP)
#define SATA_M0_IDM_IO_CONTROL_DIRECT_VA   HW_IO_PHYS_TO_VIRT(SATA_M0_IDM_IO_CONTROL_DIRECT)
#define SATA_M0_IDM_IDM_RESET_CONTROL_VA   HW_IO_PHYS_TO_VIRT(SATA_M0_IDM_IDM_RESET_CONTROL)
#define SATA_TOP_CTRL_BUS_CTRL_VA   HW_IO_PHYS_TO_VIRT(SATA_TOP_CTRL_BUS_CTRL)
#define SATA3_PCB_UPPER_REG15_VA   HW_IO_PHYS_TO_VIRT(SATA3_PCB_UPPER_REG15)
#define SATA3_PCB_UPPER_REG0_VA   HW_IO_PHYS_TO_VIRT(SATA3_PCB_UPPER_REG0)
#define SATA3_PCB_UPPER_REG1_VA   HW_IO_PHYS_TO_VIRT(SATA3_PCB_UPPER_REG1)
#define SATA3_PCB_UPPER_REG11_VA   HW_IO_PHYS_TO_VIRT(SATA3_PCB_UPPER_REG11)
#define SATA3_PCB_UPPER_REG5_VA   HW_IO_PHYS_TO_VIRT(SATA3_PCB_UPPER_REG5)
#define BCM_INT_SATA 190
#endif

extern void request_idm_timeout_interrupts(void);
extern irqreturn_t idm_timeout_handler(int val, void *ptr);

#if defined(CONFIG_IPROC_OTP)
extern void*	bcm5301x_otp_init(void);
extern int	bcm5301x_otp_exit(void);
extern int	bcm5301x_otp_read_dword(void *oh, uint wn, u32 *data);
#endif

/* This is the main reference clock 25MHz from external crystal */
static struct clk clk_ref = {
	.name = "Refclk",
	.rate = 25 * 1000000,   /* run-time override */
	.fixed = 1,
	.type  = 0,
};


static struct clk_lookup board_clk_lookups[] = {
	{
	.con_id         = "refclk",
	.clk            = &clk_ref,
	}
};

extern void __init northstar_timer_init(struct clk *clk_ref);

#if defined(CONFIG_IPROC_SD) || defined(CONFIG_IPROC_SD_MODULE)
/* sdio */
static struct sdio_platform_cfg sdio_platform_data = {
	.devtype = SDIO_DEV_TYPE_SDMMC,
};
static struct resource sdio_resources[] = {
	[0] = {
		.start	= IPROC_SDIO_PA,
		.end	= IPROC_SDIO_PA + BSC_CORE_REG_SIZE - 1,
		.flags	= IORESOURCE_MEM,
	},
	[1] = {
		.start	= IPROC_SDIO_IRQ,
		.end	= IPROC_SDIO_IRQ,
		.flags	= IORESOURCE_IRQ,
	}
};

static struct platform_device board_sdio_device = {
	.name		=	"iproc-sdio",
	.id		=	0,
	.dev		=	{
		.platform_data	= &sdio_platform_data,
	},
	.num_resources  = ARRAY_SIZE(sdio_resources),
  	.resource	= sdio_resources,
};

static void setup_sdio(void)
{
    void __iomem *idm_base;
    struct platform_device *sdio_plat_dev[1];    
    idm_base = (void __iomem *)IPROC_IDM_REGISTER_VA;
	printk("%s: %d %08x\n", __FUNCTION__, __LINE__, idm_base + SDIO_IDM_IDM_RESET_CONTROL);
    __raw_writel(0, idm_base + SDIO_IDM_IDM_RESET_CONTROL);
    sdio_plat_dev[0] = &board_sdio_device;
    platform_add_devices(sdio_plat_dev, 1);

}
#endif /* CONFIG_IPROC_SD || CONFIG_IPROC_SD_MODULE */

#if defined(CONFIG_PWM_IPROC) || defined(CONFIG_PWM_IPROC_MODULE)
static struct resource iproc_pwm_resources = {
	.start	= IPROC_CCB_PWM_CTL,
	.end	= IPROC_CCB_PWM_CTL + SZ_4K - 1,
	.flags	= IORESOURCE_MEM,
};

static struct platform_device board_pwm_device = {
	.name		= "iproc_pwmc",
	.id	        = -1,
  	.resource	= &iproc_pwm_resources,
  	.num_resources  = 1,
};
static struct pwm_lookup board_pwm_lookup[] = {
    PWM_LOOKUP("iproc_pwmc", 0,"iproc_pwmc","pwm-0"),
    PWM_LOOKUP("iproc_pwmc", 1,"iproc_pwmc","pwm-1"),        
    PWM_LOOKUP("iproc_pwmc", 2,"iproc_pwmc","pwm-2"),
    PWM_LOOKUP("iproc_pwmc", 3,"iproc_pwmc","pwm-3"),

};

#endif /* CONFIG_PWM_IPROC || CONFIG_PWM_IPROC_MODULE */

#if defined(CONFIG_IPROC_WDT) || defined(CONFIG_IPROC_WDT_MODULE)
/* watchdog */
static struct resource wdt_resources[] = {
	[0] = {
		.start	= IPROC_CCA_REG_BASE,
		.end	= IPROC_CCA_REG_BASE + 0x1000 - 1,
		.flags	= IORESOURCE_MEM,
	},
};

static struct platform_device board_wdt_device = {
	.name		=	"iproc_wdt",
	.id		    =	-1,
	.num_resources  = ARRAY_SIZE(wdt_resources),
  	.resource	= wdt_resources,
};
#endif /* CONFIG_IPROC_WDT || CONFIG_IPROC_WDT_MODULE */

#if defined(CONFIG_IPROC_CCB_WDT) || defined(CONFIG_IPROC_CCB_WDT_MODULE)
static AMBA_APB_DEVICE(sp805_wdt, "sp805-wdt", 0x00141805, 
								IPROC_CCB_WDT_REG_BASE, { }, NULL);
#endif

#if defined(CONFIG_IPROC_CCB_TIMER) || defined(CONFIG_IPROC_CCB_TIMER_MODULE)
static struct resource ccb_timer_resources[] = {
	[0] = {
		.start	= IPROC_CCB_TIM0_REG_VA,
		.end	= IPROC_CCB_TIM0_REG_VA + 0x1000 - 1,
		.flags	= IORESOURCE_MEM,
	},
	[1] = {
		.start	= IPROC_CCB_TIM1_REG_VA,
		.end	= IPROC_CCB_TIM1_REG_VA + 0x1000 - 1,
		.flags	= IORESOURCE_MEM,
	},
	[2] = {
		.start	= IPROC_CCB_TIMER_INT_START,
		.end	= IPROC_CCB_TIMER_INT_START + IPROC_CCB_TIMER_INT_COUNT - 1,
		.flags	= IORESOURCE_IRQ,
	},
};

static struct platform_device board_timer_device = {
	.name		=	"iproc_ccb_timer",
	.id		    =	-1,
	.num_resources  = ARRAY_SIZE(ccb_timer_resources),
  	.resource	= ccb_timer_resources,
};
#endif /* CONFIG_IPROC_CCB_TIMER || CONFIG_IPROC_CCB_TIMER_MODULE */

#if defined(CONFIG_IPROC_FA2)
#if defined(CONFIG_MACH_NSP)
static struct resource fa2_resources[] = {
    [0] = {
        .start  = CTF_CONTROL_REG, /* Macro is in socregs_nsp.h */
        .end    = CTF_CONTROL_REG + SZ_1K - 1,
        .flags  = IORESOURCE_MEM,
    },
    [1] = {
        .start  = 178, 
        .end    = 178,
        .flags  = IORESOURCE_IRQ,
    }
};
#endif
#endif /* CONFIG_IPROC_FA2 */

#ifdef CONFIG_I2C_IPROC
/* SMBus (I2C/BSC) block */

#if defined(CONFIG_MACH_HX4) || defined(CONFIG_MACH_KT2)
/* Helix4 */
static struct resource smbus_resources[] = {
    [0] = {
        .start  = ChipcommonB_SMBus_Config, /* Macro is in socregs_hx4.h */
        .end    = ChipcommonB_SMBus_Config + SZ_4K - 1,
        .flags  = IORESOURCE_MEM,
    },
    [1] = {
        .start  = 127, 
        .end    = 127,
        .flags  = IORESOURCE_IRQ,
    }
};

static struct resource smbus_resources1[] = {
    [0] = {
        .start  = ChipcommonB_SMBus1_SMBus_Config, /* Macro is in socregs_hx4.h */
        .end    = ChipcommonB_SMBus1_SMBus_Config + SZ_4K - 1,
        .flags  = IORESOURCE_MEM
    },
    [1] = {
        .start  = 128, /* macro in irqs.h (plat-iproc) */
        .end    = 128,
        .flags  = IORESOURCE_IRQ,
    }
};
#elif defined(CONFIG_MACH_NSP)
/* Northstar plus */
static struct resource smbus_resources[] = {
    [0] = {
        .start  = ChipcommonB_SMBus_Config, /* Macro is in socregs_hx4.h */
        .end    = ChipcommonB_SMBus_Config + SZ_4K - 1,
        .flags  = IORESOURCE_MEM,
    },
    [1] = {
        .start  = 121, 
        .end    = 121,
        .flags  = IORESOURCE_IRQ,
    }
};
#else
/* Northstar */
static struct resource smbus_resources[] = {
    [0] = {
        .start  = CCB_SMBUS_START, /* Define this macro is socregs.h, or
                                    in iproc_regs.h */
        .end    = CCB_SMBUS_START + SZ_4K - 1,
        .flags  = IORESOURCE_MEM,
    },
    [1] = {
        .start  = BCM_INT_ID_CCB_SMBUS, /* macro in irqs.h (plat-iproc) */
        .end    = BCM_INT_ID_CCB_SMBUS,
        .flags  = IORESOURCE_IRQ,
    }
};
#endif

/* Common to Northstar, Helix4 */
static struct platform_device board_smbus_device = {
    .name= "iproc-smb",
    .id = 0,
    .dev= {
        .platform_data  = NULL, /* Can be defined, if reqd */
    },
    .num_resources = ARRAY_SIZE(smbus_resources),
    .resource = smbus_resources,
};
#if defined(CONFIG_MACH_HX4) || defined(CONFIG_MACH_KT2)
static struct platform_device board_smbus_device1 = {
    .name= "iproc-smb",
    .id = 1,
    .dev= {
        .platform_data  = NULL, /* Can be defined, if reqd */
    },
    .num_resources = ARRAY_SIZE(smbus_resources1),
    .resource = smbus_resources1,
};

#endif /* CONFIG_MACH_HX4 */

#endif /* CONFIG_I2C_IPROC */

#if defined(CONFIG_IPROC_FA2)
#if defined(CONFIG_MACH_NSP)
static struct platform_device board_fa2_device = {
    .name= "fa2",
    .id = 0,
    .dev= {
        .platform_data  = NULL, /* Can be defined, if reqd */
    },
    .num_resources = ARRAY_SIZE(fa2_resources),
    .resource = fa2_resources,
};
#endif
#endif /* CONFIG_IPROC_FA2 */

#ifdef CONFIG_IPROC_USB3H
static struct resource bcm_xhci_resources[] = {
        [0] = {
                .start  = USB30_BASE,
                .end    = USB30_BASE + SZ_4K - 1,
                .flags  = IORESOURCE_MEM,
        },
        [1] = {
                .start  = BCM_INT_ID_USB3H2CORE_USB2_INT0,
                .end    = BCM_INT_ID_USB3H2CORE_USB2_INT0,
                .flags  = IORESOURCE_IRQ,
        },
};

static u64 xhci_dmamask = DMA_BIT_MASK(32);

static struct platform_device bcm_xhci_device = {
	.name		=	"bcm-xhci",
	.id		=	0,
	.dev		=	{
//		 .platform_data	= &xhci_platform_data,
		 .dma_mask       = &xhci_dmamask,
		 .coherent_dma_mask = DMA_BIT_MASK(32),
	},
  	.resource	= bcm_xhci_resources,
	.num_resources  = ARRAY_SIZE(bcm_xhci_resources),
};
#endif /* CONFIG_IPROC_USB3 */

#ifdef CONFIG_USB_EHCI_BCM

static u64 ehci_dmamask = DMA_BIT_MASK(32);

static struct resource usbh_ehci_resource[] = {
	[0] = {
		.start = IPROC_USB20_REG_BASE,
		.end = IPROC_USB20_REG_BASE + 0x0FFF,
		.flags = IORESOURCE_MEM,
	},
	[1] = {
		.start = BCM_INT_ID_USB2H2CORE_USB2_INT,
		.end = BCM_INT_ID_USB2H2CORE_USB2_INT,
		.flags = IORESOURCE_IRQ,
	},
};

static struct platform_device usbh_ehci_device =
{
	.name = "bcm-ehci",
	.id = 0,
	.resource = usbh_ehci_resource,
	.num_resources = ARRAY_SIZE(usbh_ehci_resource),
	.dev = {
		.dma_mask = &ehci_dmamask,
		.coherent_dma_mask = DMA_BIT_MASK(32),
	},
};
#endif

#ifdef CONFIG_USB_OHCI_BCM

static u64 ohci_dmamask = DMA_BIT_MASK(32);

static struct resource usbh_ohci_resource[] = {
	[0] = {
		.start = IPROC_USB20_REG_BASE + 0x1000,
		.end = IPROC_USB20_REG_BASE + 0x1000 + 0x0FFF,
		.flags = IORESOURCE_MEM,
	},
	[1] = {
		.start = BCM_INT_ID_USB2H2CORE_USB2_INT,
		.end = BCM_INT_ID_USB2H2CORE_USB2_INT,
		.flags = IORESOURCE_IRQ,
	},
};

static struct platform_device usbh_ohci_device =
{
	.name = "bcm-ohci",
	.id = 0,
	.resource = usbh_ohci_resource,
	.num_resources = ARRAY_SIZE(usbh_ohci_resource),
	.dev = {
		.dma_mask = &ohci_dmamask,
		.coherent_dma_mask = DMA_BIT_MASK(32),
	},
};
#endif

#ifdef CONFIG_SATA_AHCI_PLATFORM
static struct resource bcm_sata_resources[] = {
        [0] = {
                .start  = SATA_AHCI_GHC_HBA_CAP,
                .end    = SATA_AHCI_GHC_HBA_CAP + SZ_4K - 1,
                .flags  = IORESOURCE_MEM,
        },
        [1] = {
                .start  = BCM_INT_SATA,
                .end    = BCM_INT_SATA,
                .flags  = IORESOURCE_IRQ,
        },
};
static u64 sata_dmamask = DMA_BIT_MASK(32);

static struct platform_device bcm_sata_device = {
	.name		=	"strict-ahci",
	.id		=	0,
	.dev		=	{
		 .dma_mask       = &sata_dmamask,
		 .coherent_dma_mask = DMA_BIT_MASK(32),
	},
  	.resource	= bcm_sata_resources,
	.num_resources  = ARRAY_SIZE(bcm_sata_resources),
};
#endif

#if defined(CONFIG_MACH_NSP)
void config_AHCI( void )
{
	volatile unsigned int sata_clk_enable;
	volatile unsigned int bustopcfg;

	printk("\nConfigure AHCI ...\n");
	sata_clk_enable = __raw_readl(SATA_M0_IDM_IO_CONTROL_DIRECT_VA);
    sata_clk_enable |= 0x1;
    __raw_writel( sata_clk_enable, SATA_M0_IDM_IO_CONTROL_DIRECT_VA);
    sata_clk_enable = __raw_readl(SATA_M0_IDM_IO_CONTROL_DIRECT_VA);
    udelay(1000);


    /* Reset core */
    __raw_writel(0x0, SATA_M0_IDM_IDM_RESET_CONTROL_VA);
    udelay(1000);
    sata_clk_enable = __raw_readl(SATA_M0_IDM_IDM_RESET_CONTROL_VA);
    bustopcfg = __raw_readl(SATA_TOP_CTRL_BUS_CTRL_VA);
    bustopcfg &= ~ (( 3 << 2) | ( 3 << 4 ));
    bustopcfg |= (( 2 << 2) | ( 2 << 4 ));//| ( 2<< 6 ));
    //bustopcfg |= ( ( 0x2 << 8 ) | ( 0x2 << 17 ) );
    __raw_writel(bustopcfg, SATA_TOP_CTRL_BUS_CTRL_VA);
}
void configure_SATA_PHY ( void )
{
	unsigned int i;
	printk("\nConfigure PHY ...\n");

	__raw_writel(0x0150,SATA3_PCB_UPPER_REG15_VA);
	__raw_writel( 0xF6F6, SATA3_PCB_UPPER_REG0_VA);
	__raw_writel( 0x2e96, SATA3_PCB_UPPER_REG1_VA);

	__raw_writel(0x0160,SATA3_PCB_UPPER_REG15_VA);
	__raw_writel( 0xF6F6, SATA3_PCB_UPPER_REG0_VA);
	__raw_writel( 0x2e96, SATA3_PCB_UPPER_REG1_VA);

	//access SATA PLL
	__raw_writel(0x0050,SATA3_PCB_UPPER_REG15_VA);
	//Audio PLL 0x8B
	i = __raw_readl(SATA3_PCB_UPPER_REG11_VA);
	i &= ~ (( 0x1f) << 9 );
	i |= ( 0xC << 9);
	__raw_writel( i, SATA3_PCB_UPPER_REG11_VA);

	//Sequence for restarting PLL. Please try after changing the divider.
	//'PLL_CapCtrl[10] = 1, PLL_CapCtrl[7:0] = F0h
	//SATA3_PLL: PLL Register Bank address 0x50

	//set register SATA3_PLL_capControl ( 0x85 )
	i = __raw_readl(SATA3_PCB_UPPER_REG5_VA);
	i =  ( i | 0x4f0 ) & 0xFF0;
	__raw_writel( i, SATA3_PCB_UPPER_REG5_VA);

	//'PLL_Ctrl[13:12] = 11
	//Set register SATA3_PLL_CONTROL ( 0x81 )
	i = __raw_readl(SATA3_PCB_UPPER_REG1_VA);
	i |= 0x3000;
	__raw_writel( i, SATA3_PCB_UPPER_REG1_VA);

	//'PLL_ReStart
	i = __raw_readl(SATA3_PCB_UPPER_REG1_VA);
	i &= 0x7FFF;
	__raw_writel( i, SATA3_PCB_UPPER_REG1_VA);
	mdelay(100);
	i = __raw_readl(SATA3_PCB_UPPER_REG1_VA);
	i |= 0x8000;
	__raw_writel( i, SATA3_PCB_UPPER_REG1_VA);
	mdelay(1000);
	__raw_writel(0x0000,SATA3_PCB_UPPER_REG15_VA);
	i = __raw_readl(SATA3_PCB_UPPER_REG1_VA);
}
#endif


void __init board_map_io(void)
{

	/*
	 * Install clock sources in the lookup table.
	 */
	clkdev_add_table(board_clk_lookups,
			ARRAY_SIZE(board_clk_lookups));

	/* Map machine specific iodesc here */
	northstar_map_io();
}

#ifdef CONFIG_SATA_AHCI_PLATFORM
static struct platform_device *board_sata_device[] __initdata = {
	&bcm_sata_device,
};
#endif

static struct platform_device *board_devices[] __initdata = {
#ifdef CONFIG_I2C_IPROC
    &board_smbus_device,
#if defined(CONFIG_MACH_HX4) || defined(CONFIG_MACH_KT2)
    &board_smbus_device1,
#endif
#endif /* CONFIG_I2C_IPROC */

#if defined(CONFIG_IPROC_FA2)
#if defined(CONFIG_MACH_NSP)
    &board_fa2_device,
#endif
#endif /* FA+ */

#if defined(CONFIG_IPROC_CCB_TIMER) || defined(CONFIG_IPROC_CCB_TIMER_MODULE)
    &board_timer_device,
#endif /* CONFIG_IPROC_CCB_TIMER || CONFIG_IPROC_CCB_TIMER_MODULE */
#if defined(CONFIG_IPROC_WDT) || defined(CONFIG_IPROC_WDT_MODULE)
    &board_wdt_device,
#endif /* CONFIG_IPROC_WDT || CONFIG_IPROC_WDT_MODULE */
#if defined(CONFIG_PWM_IPROC) || defined(CONFIG_PWM_IPROC_MODULE)
	&board_pwm_device,
#endif /* CONFIG_PWM_IPROC || CONFIG_PWM_IPROC_MODULE */
#ifdef CONFIG_IPROC_USB3H
	&bcm_xhci_device,
#endif
#ifdef CONFIG_USB_EHCI_BCM
	&usbh_ehci_device,
#endif
#ifdef CONFIG_USB_OHCI_BCM
	&usbh_ohci_device,
#endif
#ifdef CONFIG_DMAC_PL330
	&pl330_dmac_device,
#endif
};

static struct amba_device *amba_devs[] __initdata = {
#if defined(CONFIG_IPROC_CCB_WDT) || defined(CONFIG_IPROC_CCB_WDT_MODULE)
	&sp805_wdt_device,
#endif
};

static void __init board_add_devices(void)
{
	int i;

	platform_add_devices(board_devices, ARRAY_SIZE(board_devices));
//    if (iproc_get_chipid() == 53010) {
//    }
	for (i = 0; i < ARRAY_SIZE(amba_devs); i++) {
		amba_device_register(amba_devs[i], &iomem_resource);
	}
}

#ifdef CONFIG_SATA_AHCI_PLATFORM
static void __init board_add_sata_device(void)
{
	platform_add_devices(board_sata_device, 1);
}
#endif


/* SPI device info of GSIO(SPI) interface */
static struct spi_board_info bcm5301x_spi_device[] = {
	{
		.modalias = "spidev",
		.platform_data = NULL,
		.controller_data = NULL,
		.max_speed_hz = 2 * 1000 * 1000,
		.bus_num = 0,
		.chip_select = 0,
		.mode = SPI_MODE_0,
	},
};

void __init board_timer_init(void)
{
	northstar_timer_init(&clk_ref);
}

struct sys_timer board_timer = {
	.init   = board_timer_init,
};


void __init board_init(void)
{
	printk(KERN_DEBUG "board_init: Enter\n");

	/*
	 * Add common platform devices that do not have board dependent HW
	 * configurations
	 */
	uint32_t	sata_enable=0;
	board_add_common_devices(&clk_ref);
#ifndef CONFIG_MACH_NS
	/* register IDM timeout interrupt handler */
	request_idm_timeout_interrupts();
#endif

#if defined(CONFIG_IPROC_OTP)
	/* read otp row 0xd to figure if sata is enabled */

	void		*oh;
	
	oh = bcm5301x_otp_init();
	if (oh != NULL)
	{
		bcm5301x_otp_read_dword(oh, 0xd, &sata_enable);
		printk("%s: %d %08x\n", __FUNCTION__, __LINE__, sata_enable);
		if ((sata_enable & 0x40000000) == 0x40000000)
		{
			config_AHCI();
			configure_SATA_PHY();
		}
		bcm5301x_otp_exit();
	}
	else
		printk("%s: %d bcm5301x_otp_init failed\n", __FUNCTION__, __LINE__);

#endif /* CONFIG_IPROC_OTP */

	board_add_devices();

#ifdef CONFIG_SATA_AHCI_PLATFORM
	if ((sata_enable & 0x40000000) == 0x40000000)
		board_add_sata_device();
#endif

#if defined(CONFIG_IPROC_SD) || defined(CONFIG_IPROC_SD_MODULE)   
	/* only bcm53012 support sdio */
	if ((__REG32(IPROC_IDM_REGISTER_VA + 0xd500) & 0xc) == 0x0) {
		setup_sdio();
	}
#endif

#if defined(CONFIG_PWM_IPROC) || defined(CONFIG_PWM_IPROC_MODULE)
        __raw_writel(0xf, IPROC_CCB_GPIO_REG_VA + IPROC_GPIO_CCB_AUX_SEL);
    	pwm_add_table(board_pwm_lookup, ARRAY_SIZE(board_pwm_lookup));
#endif  

	/* Register SPI device info */
	spi_register_board_info(bcm5301x_spi_device, 
	ARRAY_SIZE(bcm5301x_spi_device));


	printk(KERN_DEBUG "board_init: Leave\n");
}

MACHINE_START(IPROC, "Broadcom iProc")

// 	Used micro9 as a reference.  Micro9 removed these two fields,
//	and replaced them with a call to ep93xx_map_io(), which in turn
// 	calls iotable_init().  Northstar appears to have an equivalent
//	init (refer to northstar_io_desc[] array, in io_map.c
	.map_io = board_map_io,
	.init_irq = iproc_init_irq,
        .handle_irq     = gic_handle_irq,
	.timer  = &board_timer,
	.init_machine = board_init,
	.smp = smp_ops(iproc_smp_ops),
MACHINE_END
