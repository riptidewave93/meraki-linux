/*
 * linux/arch/arm/mach-omap2/devices.c
 *
 * OMAP2 platform device setup/initialization
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 */

#include <linux/kernel.h>
#include <linux/init.h>
#include <linux/platform_device.h>
#include <linux/io.h>
#include <linux/clk.h>
#include <linux/err.h>
#include <linux/davinci_emac.h>
#include <linux/cpsw.h>
#include <linux/ahci_platform.h>
#include <linux/delay.h>
#include <linux/can/platform/d_can.h>

#include <mach/hardware.h>
#include <mach/irqs.h>
#include <asm/mach-types.h>
#include <asm/mach/map.h>
#include <asm/pmu.h>
#include <asm/hardware/edma.h>

#include <plat/tc.h>
#include <plat/board.h>
#include <plat/mcbsp.h>
#include <mach/gpio.h>
#include <plat/mmc.h>
#include <plat/dma.h>
#include <plat/omap_hwmod.h>
#include <plat/omap_device.h>
#include <plat/asp.h>
#ifdef CONFIG_HAVE_PWM
#include <plat/pwm.h>
#endif
#include <plat/smartreflex.h>
#include <plat/ti81xx-vpss.h>
#include "mux.h"
#include "control.h"
#include "pcie-ti81xx.h"

#include "devices.h"
#include "plat/elm.h"
#if defined(CONFIG_VIDEO_OMAP2) || defined(CONFIG_VIDEO_OMAP2_MODULE)

static struct resource omap2cam_resources[] = {
	{
		.start		= OMAP24XX_CAMERA_BASE,
		.end		= OMAP24XX_CAMERA_BASE + 0xfff,
		.flags		= IORESOURCE_MEM,
	},
	{
		.start		= INT_24XX_CAM_IRQ,
		.flags		= IORESOURCE_IRQ,
	}
};

static struct platform_device omap2cam_device = {
	.name		= "omap24xxcam",
	.id		= -1,
	.num_resources	= ARRAY_SIZE(omap2cam_resources),
	.resource	= omap2cam_resources,
};
#endif

static struct resource omap3isp_resources[] = {
	{
		.start		= OMAP3430_ISP_BASE,
		.end		= OMAP3430_ISP_END,
		.flags		= IORESOURCE_MEM,
	},
	{
		.start		= OMAP3430_ISP_CCP2_BASE,
		.end		= OMAP3430_ISP_CCP2_END,
		.flags		= IORESOURCE_MEM,
	},
	{
		.start		= OMAP3430_ISP_CCDC_BASE,
		.end		= OMAP3430_ISP_CCDC_END,
		.flags		= IORESOURCE_MEM,
	},
	{
		.start		= OMAP3430_ISP_HIST_BASE,
		.end		= OMAP3430_ISP_HIST_END,
		.flags		= IORESOURCE_MEM,
	},
	{
		.start		= OMAP3430_ISP_H3A_BASE,
		.end		= OMAP3430_ISP_H3A_END,
		.flags		= IORESOURCE_MEM,
	},
	{
		.start		= OMAP3430_ISP_PREV_BASE,
		.end		= OMAP3430_ISP_PREV_END,
		.flags		= IORESOURCE_MEM,
	},
	{
		.start		= OMAP3430_ISP_RESZ_BASE,
		.end		= OMAP3430_ISP_RESZ_END,
		.flags		= IORESOURCE_MEM,
	},
	{
		.start		= OMAP3430_ISP_SBL_BASE,
		.end		= OMAP3430_ISP_SBL_END,
		.flags		= IORESOURCE_MEM,
	},
	{
		.start		= OMAP3430_ISP_CSI2A_REGS1_BASE,
		.end		= OMAP3430_ISP_CSI2A_REGS1_END,
		.flags		= IORESOURCE_MEM,
	},
	{
		.start		= OMAP3430_ISP_CSIPHY2_BASE,
		.end		= OMAP3430_ISP_CSIPHY2_END,
		.flags		= IORESOURCE_MEM,
	},
	{
		.start		= OMAP3630_ISP_CSI2A_REGS2_BASE,
		.end		= OMAP3630_ISP_CSI2A_REGS2_END,
		.flags		= IORESOURCE_MEM,
	},
	{
		.start		= OMAP3630_ISP_CSI2C_REGS1_BASE,
		.end		= OMAP3630_ISP_CSI2C_REGS1_END,
		.flags		= IORESOURCE_MEM,
	},
	{
		.start		= OMAP3630_ISP_CSIPHY1_BASE,
		.end		= OMAP3630_ISP_CSIPHY1_END,
		.flags		= IORESOURCE_MEM,
	},
	{
		.start		= OMAP3630_ISP_CSI2C_REGS2_BASE,
		.end		= OMAP3630_ISP_CSI2C_REGS2_END,
		.flags		= IORESOURCE_MEM,
	},
	{
		.start		= INT_34XX_CAM_IRQ,
		.flags		= IORESOURCE_IRQ,
	}
};

static struct platform_device omap3isp_device = {
	.name		= "omap3isp",
	.id		= -1,
	.num_resources	= ARRAY_SIZE(omap3isp_resources),
	.resource	= omap3isp_resources,
};

int omap3_init_camera(void *pdata)
{
	omap3isp_device.dev.platform_data = pdata;
	return platform_device_register(&omap3isp_device);
}
EXPORT_SYMBOL_GPL(omap3_init_camera);

static inline void omap_init_camera(void)
{
#if defined(CONFIG_VIDEO_OMAP2) || defined(CONFIG_VIDEO_OMAP2_MODULE)
	if (cpu_is_omap24xx())
		platform_device_register(&omap2cam_device);
#endif
}

#ifdef CONFIG_ARCH_TI814X

#define TI814X_D_CAN_RAM_BASE			0x1000
#define TI814X_D_CAN_NUM_MSG_OBJS		64
#define TI814X_CTL_DCAN_RAMINIT_OFFSET		0x644
#define TI814X_D_CAN_RAMINIT_START(n)		(0x1 << n)

static void d_can_hw_raminit(unsigned int instance)
{
	u32 val;

	/* Read the value */
	val = __raw_readl(TI81XX_CTRL_REGADDR(TI814X_CTL_DCAN_RAMINIT_OFFSET));

	/* Modify by setting "0" */
	val &= ~TI814X_D_CAN_RAMINIT_START(instance);
	__raw_writel(val, TI81XX_CTRL_REGADDR(TI814X_CTL_DCAN_RAMINIT_OFFSET));

	/* Reset to one */
	val |= TI814X_D_CAN_RAMINIT_START(instance);
	__raw_writel(val, TI81XX_CTRL_REGADDR(TI814X_CTL_DCAN_RAMINIT_OFFSET));

	/* Give some time delay for transition from 0 -> 1 */
	udelay(1);
}

static struct d_can_platform_data ti814x_evm_d_can0_pdata = {
	.d_can_offset		= 0,
	.d_can_ram_offset	= TI814X_D_CAN_RAM_BASE,
	.num_of_msg_objs	= TI814X_D_CAN_NUM_MSG_OBJS,
	.dma_support		= false,
	.parity_check		= false,
	.fck_name		= "dcan0_fck",
	.ick_name		= "dcan0_ick",
};

static struct resource ti814x_d_can0_resources[] = {
	{
		.start	= TI814X_D_CAN0_BASE,
		.end	= TI814X_D_CAN0_BASE + 0x3FFF,
		.flags	= IORESOURCE_MEM,
	},
	{
		.name	= "int0",
		.start	= TI814X_IRQ_DCAN0_INT0,
		.end	= TI814X_IRQ_DCAN0_INT0,
		.flags	= IORESOURCE_IRQ,
	},
	{
		.name	= "int1",
		.start	= TI814X_IRQ_DCAN0_INT1,
		.end	= TI814X_IRQ_DCAN0_INT1,
		.flags	= IORESOURCE_IRQ,
	},
};

static struct platform_device ti814x_d_can0_device = {
	.dev		= {
		.platform_data = &ti814x_evm_d_can0_pdata,
	},
	.name		= "d_can",
	.id		= -1,
	.num_resources	= ARRAY_SIZE(ti814x_d_can0_resources),
	.resource	= ti814x_d_can0_resources,
};

static void ti814x_d_can_init(unsigned int instance)
{
	omap_mux_init_signal("dcan0_tx.dcan0_tx", 0);
	omap_mux_init_signal("dcan0_rx.dcan0_rx", TI814X_INPUT_EN |
						TI814X_PULL_UP);
	d_can_hw_raminit(instance);
	platform_device_register(&ti814x_d_can0_device);
}
#endif

#if defined(CONFIG_OMAP_MBOX_FWK) || defined(CONFIG_OMAP_MBOX_FWK_MODULE)

#define MBOX_REG_SIZE   0x120

#ifdef CONFIG_ARCH_OMAP2
static struct resource omap2_mbox_resources[] = {
	{
		.start		= OMAP24XX_MAILBOX_BASE,
		.end		= OMAP24XX_MAILBOX_BASE + MBOX_REG_SIZE - 1,
		.flags		= IORESOURCE_MEM,
	},
	{
		.start		= INT_24XX_MAIL_U0_MPU,
		.flags		= IORESOURCE_IRQ,
		.name		= "dsp",
	},
	{
		.start		= INT_24XX_MAIL_U3_MPU,
		.flags		= IORESOURCE_IRQ,
		.name		= "iva",
	},
};
static int omap2_mbox_resources_sz = ARRAY_SIZE(omap2_mbox_resources);
#else
#define omap2_mbox_resources		NULL
#define omap2_mbox_resources_sz		0
#endif

#ifdef CONFIG_ARCH_OMAP3
static struct resource omap3_mbox_resources[] = {
	{
		.start		= OMAP34XX_MAILBOX_BASE,
		.end		= OMAP34XX_MAILBOX_BASE + MBOX_REG_SIZE - 1,
		.flags		= IORESOURCE_MEM,
	},
	{
		.start		= INT_24XX_MAIL_U0_MPU,
		.flags		= IORESOURCE_IRQ,
		.name		= "dsp",
	},
};
static int omap3_mbox_resources_sz = ARRAY_SIZE(omap3_mbox_resources);
#else
#define omap3_mbox_resources		NULL
#define omap3_mbox_resources_sz		0
#endif

#ifdef CONFIG_ARCH_OMAP4

#define OMAP4_MBOX_REG_SIZE	0x130
static struct resource omap4_mbox_resources[] = {
	{
		.start          = OMAP44XX_MAILBOX_BASE,
		.end            = OMAP44XX_MAILBOX_BASE +
					OMAP4_MBOX_REG_SIZE - 1,
		.flags          = IORESOURCE_MEM,
	},
	{
		.start          = OMAP44XX_IRQ_MAIL_U0,
		.flags          = IORESOURCE_IRQ,
		.name		= "mbox",
	},
};
static int omap4_mbox_resources_sz = ARRAY_SIZE(omap4_mbox_resources);
#else
#define omap4_mbox_resources		NULL
#define omap4_mbox_resources_sz		0
#endif

#ifdef CONFIG_ARCH_TI81XX

#define TI81XX_MBOX_REG_SIZE            0x144
static struct resource ti81xx_mbox_resources[] = {
	{
		.start          = TI81XX_MAILBOX_BASE,
		.end            = TI81XX_MAILBOX_BASE +
					TI81XX_MBOX_REG_SIZE - 1,
		.flags          = IORESOURCE_MEM,
	},
	{
		.start          = TI81XX_IRQ_MBOX,
		.flags          = IORESOURCE_IRQ,
		.name		= "mbox",
	},
};
static int ti81xx_mbox_resources_sz = ARRAY_SIZE(ti81xx_mbox_resources);
#else
#define ti81xx_mbox_resources		NULL
#define ti81xx_mbox_resources_sz	0
#endif

static struct platform_device mbox_device = {
	.name		= "omap-mailbox",
	.id		= -1,
};

static inline void omap_init_mbox(void)
{
	if (cpu_is_omap24xx()) {
		mbox_device.resource = omap2_mbox_resources;
		mbox_device.num_resources = omap2_mbox_resources_sz;
	} else if (cpu_is_omap34xx()) {
		mbox_device.resource = omap3_mbox_resources;
		mbox_device.num_resources = omap3_mbox_resources_sz;
	} else if (cpu_is_omap44xx()) {
		mbox_device.resource = omap4_mbox_resources;
		mbox_device.num_resources = omap4_mbox_resources_sz;
	} else if (cpu_is_ti81xx()) {
		mbox_device.resource = ti81xx_mbox_resources;
		mbox_device.num_resources = ti81xx_mbox_resources_sz;
	} else {
		pr_err("%s: platform not supported\n", __func__);
		return;
	}
	platform_device_register(&mbox_device);
}
#else
static inline void omap_init_mbox(void) { }
#endif /* CONFIG_OMAP_MBOX_FWK */

static inline void omap_init_sti(void) {}

#if defined(CONFIG_SND_SOC) || defined(CONFIG_SND_SOC_MODULE)

#if defined(CONFIG_ARCH_TI81XX)
struct platform_device ti81xx_pcm_device = {
	.name		= "davinci-pcm-audio",
	.id		= -1,
};

static void ti81xx_init_pcm(void)
{
	platform_device_register(&ti81xx_pcm_device);
}
#else
static struct platform_device omap_pcm = {
	.name	= "omap-pcm-audio",
	.id	= -1,
};

/*
 * OMAP2420 has 2 McBSP ports
 * OMAP2430 has 5 McBSP ports
 * OMAP3 has 5 McBSP ports
 * OMAP4 has 4 McBSP ports
 */
OMAP_MCBSP_PLATFORM_DEVICE(1);
OMAP_MCBSP_PLATFORM_DEVICE(2);
OMAP_MCBSP_PLATFORM_DEVICE(3);
OMAP_MCBSP_PLATFORM_DEVICE(4);
OMAP_MCBSP_PLATFORM_DEVICE(5);

static void omap_init_audio(void)
{
	platform_device_register(&omap_mcbsp1);
	platform_device_register(&omap_mcbsp2);
	if (cpu_is_omap243x() || cpu_is_omap34xx() || cpu_is_omap44xx()) {
		platform_device_register(&omap_mcbsp3);
		platform_device_register(&omap_mcbsp4);
	}
	if (cpu_is_omap243x() || cpu_is_omap34xx())
		platform_device_register(&omap_mcbsp5);

	platform_device_register(&omap_pcm);
}

#endif /* defined(CONFIG_ARCH_TI81XX) */

#else

#if defined(CONFIG_ARCH_TI81XX)
static inline void ti81xx_init_pcm(void) {}
#else
static inline void omap_init_audio(void) {}
#endif

#endif /* defined(CONFIG_SND_SOC) || defined(CONFIG_SND_SOC_MODULE) */

#if defined(CONFIG_SPI_OMAP24XX) || defined(CONFIG_SPI_OMAP24XX_MODULE)

#include <plat/mcspi.h>

#define OMAP2_MCSPI1_BASE		0x48098000
#define OMAP2_MCSPI2_BASE		0x4809a000
#define OMAP2_MCSPI3_BASE		0x480b8000
#define OMAP2_MCSPI4_BASE		0x480ba000

#define OMAP4_MCSPI1_BASE		0x48098100
#define OMAP4_MCSPI2_BASE		0x4809a100
#define OMAP4_MCSPI3_BASE		0x480b8100
#define OMAP4_MCSPI4_BASE		0x480ba100

#define TI81XX_MCSPI1_BASE		0x48030100
#define TI814X_MCSPI2_BASE		0x481A0100
#define TI814X_MCSPI3_BASE		0x481A2100
#define TI814X_MCSPI4_BASE		0x481A4100

static struct omap2_mcspi_platform_config omap2_mcspi1_config = {
	.num_cs		= 4,
};

static struct resource omap2_mcspi1_resources[] = {
	{
		.start		= OMAP2_MCSPI1_BASE,
		.end		= OMAP2_MCSPI1_BASE + 0xff,
		.flags		= IORESOURCE_MEM,
	},
	{
		//SPI0XEVT0: SPI0 Transmit 0
		.start		= 16,
		.end		= 16,
		.flags		= IORESOURCE_DMA,
	},
	{
		//SPI0REVT0: SPI0 Receive 0
		.start		= 17,
		.end		= 17,
		.flags		= IORESOURCE_DMA,
	},
	{
                //SPI0XEVT1: SPI0 Transmit 1
                .start          = 18,
                .end            = 18,
                .flags          = IORESOURCE_DMA,
        },
        {
                //SPI0REVT1: SPI0 Receive 1
                .start          = 19,
                .end            = 19,
                .flags          = IORESOURCE_DMA,
        },
	{
                //SPI0XEVT2: SPI0 Transmit 2
                .start          = 20,
                .end            = 20,
                .flags          = IORESOURCE_DMA,
        },
        {
                //SPI0REVT2: SPI0 Receive 2
                .start          = 21,
                .end            = 21,
                .flags          = IORESOURCE_DMA,
        },
	{
                //SPI0XEVT3: SPI0 Transmit 3
                .start          = 22,
                .end            = 22,
                .flags          = IORESOURCE_DMA,
        },
        {
                //SPI0REVT3: SPI0 Receive 3
                .start          = 23,
                .end            = 23,
                .flags          = IORESOURCE_DMA,
        },


};

static struct platform_device omap2_mcspi1 = {
	.name		= "omap2_mcspi",
	.id		= 1,
	.num_resources	= ARRAY_SIZE(omap2_mcspi1_resources),
	.resource	= omap2_mcspi1_resources,
	.dev		= {
		.platform_data = &omap2_mcspi1_config,
	},
};

static struct omap2_mcspi_platform_config omap2_mcspi2_config = {
	.num_cs		= 2,
};

static struct resource omap2_mcspi2_resources[] = {
	{
		.start		= OMAP2_MCSPI2_BASE,
		.end		= OMAP2_MCSPI2_BASE + 0xff,
		.flags		= IORESOURCE_MEM,
	},
	{
                //SPI1XEVT0: SPI0 Transmit 0
                .start          = 42,
                .end            = 42,
                .flags          = IORESOURCE_DMA,
        },
        {
                //SPI1REVT0: SPI0 Receive 0
                .start          = 43,
                .end            = 43,
                .flags          = IORESOURCE_DMA,
        },
	{
                //SPI1XEVT1: SPI0 Transmit 1
                .start          = 44,
                .end            = 44,
                .flags          = IORESOURCE_DMA,
        },
        {
                //SPI1REVT1: SPI0 Receive 1
                .start          = 45,
                .end            = 45,
                .flags          = IORESOURCE_DMA,
        },


};

static struct platform_device omap2_mcspi2 = {
	.name		= "omap2_mcspi",
	.id		= 2,
	.num_resources	= ARRAY_SIZE(omap2_mcspi2_resources),
	.resource	= omap2_mcspi2_resources,
	.dev		= {
		.platform_data = &omap2_mcspi2_config,
	},
};

#if defined(CONFIG_ARCH_OMAP2430) || defined(CONFIG_ARCH_OMAP3) || \
	defined(CONFIG_ARCH_OMAP4) || defined(CONFIG_ARCH_TI814X)
static struct omap2_mcspi_platform_config omap2_mcspi3_config = {
	.num_cs		= 2,
};

static struct resource omap2_mcspi3_resources[] = {
	{
	.start		= OMAP2_MCSPI3_BASE,
	.end		= OMAP2_MCSPI3_BASE + 0xff,
	.flags		= IORESOURCE_MEM,
	},
};

static struct platform_device omap2_mcspi3 = {
	.name		= "omap2_mcspi",
	.id		= 3,
	.num_resources	= ARRAY_SIZE(omap2_mcspi3_resources),
	.resource	= omap2_mcspi3_resources,
	.dev		= {
		.platform_data = &omap2_mcspi3_config,
	},
};
#endif

#if defined(CONFIG_ARCH_OMAP3) || defined(CONFIG_ARCH_OMAP4) || \
	defined(CONFIG_ARCH_TI814X)
static struct omap2_mcspi_platform_config omap2_mcspi4_config = {
	.num_cs		= 1,
};

static struct resource omap2_mcspi4_resources[] = {
	{
		.start		= OMAP2_MCSPI4_BASE,
		.end		= OMAP2_MCSPI4_BASE + 0xff,
		.flags		= IORESOURCE_MEM,
	},
};

static struct platform_device omap2_mcspi4 = {
	.name		= "omap2_mcspi",
	.id		= 4,
	.num_resources	= ARRAY_SIZE(omap2_mcspi4_resources),
	.resource	= omap2_mcspi4_resources,
	.dev		= {
		.platform_data = &omap2_mcspi4_config,
	},
};
#endif

#ifdef CONFIG_ARCH_TI81XX
static inline void ti81xx_mcspi_fixup(void)
{
	omap2_mcspi1_resources[0].start	= TI81XX_MCSPI1_BASE;
	omap2_mcspi1_resources[0].end	= TI81XX_MCSPI1_BASE + 0xff;

	if (cpu_is_ti814x()) {
		omap2_mcspi2_resources[0].start	= TI814X_MCSPI2_BASE;
		omap2_mcspi2_resources[0].end	= TI814X_MCSPI2_BASE + 0xff;
#ifdef CONFIG_ARCH_TI814X
		omap2_mcspi3_resources[0].start	= TI814X_MCSPI3_BASE;
		omap2_mcspi3_resources[0].end	= TI814X_MCSPI3_BASE + 0xff;
		omap2_mcspi4_resources[0].start	= TI814X_MCSPI4_BASE;
		omap2_mcspi4_resources[0].end	= TI814X_MCSPI4_BASE + 0xff;
#endif
	}
}
#else
static inline void ti81xx_mcspi_fixup(void)
{
}
#endif



#ifdef CONFIG_ARCH_OMAP4
static inline void omap4_mcspi_fixup(void)
{
	omap2_mcspi1_resources[0].start	= OMAP4_MCSPI1_BASE;
	omap2_mcspi1_resources[0].end	= OMAP4_MCSPI1_BASE + 0xff;
	omap2_mcspi2_resources[0].start	= OMAP4_MCSPI2_BASE;
	omap2_mcspi2_resources[0].end	= OMAP4_MCSPI2_BASE + 0xff;
	omap2_mcspi3_resources[0].start	= OMAP4_MCSPI3_BASE;
	omap2_mcspi3_resources[0].end	= OMAP4_MCSPI3_BASE + 0xff;
	omap2_mcspi4_resources[0].start	= OMAP4_MCSPI4_BASE;
	omap2_mcspi4_resources[0].end	= OMAP4_MCSPI4_BASE + 0xff;
}
#else
static inline void omap4_mcspi_fixup(void)
{
}
#endif

static inline void omap2_mcspi2_init(void)
{
	platform_device_register(&omap2_mcspi2);
}

#if defined(CONFIG_ARCH_OMAP2430) || defined(CONFIG_ARCH_OMAP3) || \
	defined(CONFIG_ARCH_OMAP4) || defined(CONFIG_ARCH_TI814X)
static inline void omap2_mcspi3_init(void)
{
	platform_device_register(&omap2_mcspi3);
}
#else
static inline void omap2_mcspi3_init(void)
{
}
#endif

#if defined(CONFIG_ARCH_OMAP3) || defined(CONFIG_ARCH_OMAP4) || \
	defined(CONFIG_ARCH_TI814X)
static inline void omap2_mcspi4_init(void)
{
	platform_device_register(&omap2_mcspi4);
}
#else
static inline void omap2_mcspi4_init(void)
{
}
#endif

static void omap_init_mcspi(void)
{
	if (cpu_is_omap44xx())
		omap4_mcspi_fixup();

	if (cpu_is_ti81xx())
		ti81xx_mcspi_fixup();

	platform_device_register(&omap2_mcspi1);

	if (!cpu_is_ti816x())
		omap2_mcspi2_init();

	if (cpu_is_omap2430() || cpu_is_omap343x() || cpu_is_omap44xx() ||
		cpu_is_ti814x())
		omap2_mcspi3_init();

	if (cpu_is_omap343x() || cpu_is_omap44xx() || cpu_is_ti814x())
		omap2_mcspi4_init();
}

#else
static inline void omap_init_mcspi(void) {}
#endif

#ifdef CONFIG_ARCH_TI81XX

struct omap_elm_platform_data  elm_platform_data;

static int omap_elm_init(struct omap_hwmod *oh, void *platform_data)
{
	struct omap_device *pdev;
	char *name = "omap2_elm";
	static int elm_num;

	elm_num++;

	pdev = omap_device_build(name, elm_num, oh, platform_data,
			sizeof(struct omap_elm_platform_data), NULL, 0, 0);
	return 0;
}

static void omap_init_elm(void)
{
	/* configure ELM also with same ecc mode as GPMC */
#ifdef CONFIG_MTD_NAND_OMAP_ECC_BCH16_CODE_HW
	elm_platform_data.ecc_opt = OMAP_ECC_BCH16_CODE_HW;
#elif defined(CONFIG_MTD_NAND_OMAP_ECC_BCH8_CODE_HW)
	elm_platform_data.ecc_opt = OMAP_ECC_BCH8_CODE_HW;
/* (not supported) #elif  defined(CONFIG_MTD_NAND_OMAP_ECC_BCH4_CODE_HW) */
/*			elm_platform_data.ecc_opt = OMAP_ECC_BCH4_CODE_HW; */
#else
	elm_platform_data.ecc_opt = OMAP_ECC_BCH8_CODE_HW;
#endif

	omap_hwmod_for_each_by_class("elm", omap_elm_init, &elm_platform_data);
}
#else
static void omap_init_elm(void) {}
#endif

static struct resource omap2_pmu_resource = {
	.start	= 3,
	.end	= 3,
	.flags	= IORESOURCE_IRQ,
};

static struct resource omap3_pmu_resource = {
	.start	= INT_34XX_BENCH_MPU_EMUL,
	.end	= INT_34XX_BENCH_MPU_EMUL,
	.flags	= IORESOURCE_IRQ,
};

static struct platform_device omap_pmu_device = {
	.name		= "arm-pmu",
	.id		= ARM_PMU_DEVICE_CPU,
	.num_resources	= 1,
};

static void omap_init_pmu(void)
{
	if (cpu_is_omap24xx())
		omap_pmu_device.resource = &omap2_pmu_resource;
	else if (cpu_is_omap34xx() || cpu_is_ti81xx())
		omap_pmu_device.resource = &omap3_pmu_resource;
	else
		return;

	platform_device_register(&omap_pmu_device);
}


#if defined(CONFIG_CRYPTO_DEV_OMAP_SHAM) || defined(CONFIG_CRYPTO_DEV_OMAP_SHAM_MODULE)

#ifdef CONFIG_ARCH_OMAP2
static struct resource omap2_sham_resources[] = {
	{
		.start	= OMAP24XX_SEC_SHA1MD5_BASE,
		.end	= OMAP24XX_SEC_SHA1MD5_BASE + 0x64,
		.flags	= IORESOURCE_MEM,
	},
	{
		.start	= INT_24XX_SHA1MD5,
		.flags	= IORESOURCE_IRQ,
	}
};
static int omap2_sham_resources_sz = ARRAY_SIZE(omap2_sham_resources);
#else
#define omap2_sham_resources		NULL
#define omap2_sham_resources_sz		0
#endif

#ifdef CONFIG_ARCH_OMAP3
static struct resource omap3_sham_resources[] = {
	{
		.start	= OMAP34XX_SEC_SHA1MD5_BASE,
		.end	= OMAP34XX_SEC_SHA1MD5_BASE + 0x64,
		.flags	= IORESOURCE_MEM,
	},
	{
		.start	= INT_34XX_SHA1MD52_IRQ,
		.flags	= IORESOURCE_IRQ,
	},
	{
		.start	= OMAP34XX_DMA_SHA1MD5_RX,
		.flags	= IORESOURCE_DMA,
	}
};
static int omap3_sham_resources_sz = ARRAY_SIZE(omap3_sham_resources);
#else
#define omap3_sham_resources		NULL
#define omap3_sham_resources_sz		0
#endif

static struct platform_device sham_device = {
	.name		= "omap-sham",
	.id		= -1,
};

static void omap_init_sham(void)
{
	if (cpu_is_omap24xx()) {
		sham_device.resource = omap2_sham_resources;
		sham_device.num_resources = omap2_sham_resources_sz;
	} else if (cpu_is_omap34xx()) {
		sham_device.resource = omap3_sham_resources;
		sham_device.num_resources = omap3_sham_resources_sz;
	} else {
		pr_err("%s: platform not supported\n", __func__);
		return;
	}
	platform_device_register(&sham_device);
}
#else
static inline void omap_init_sham(void) { }
#endif

#if defined(CONFIG_CRYPTO_DEV_OMAP_AES) || defined(CONFIG_CRYPTO_DEV_OMAP_AES_MODULE)

#ifdef CONFIG_ARCH_OMAP2
static struct resource omap2_aes_resources[] = {
	{
		.start	= OMAP24XX_SEC_AES_BASE,
		.end	= OMAP24XX_SEC_AES_BASE + 0x4C,
		.flags	= IORESOURCE_MEM,
	},
	{
		.start	= OMAP24XX_DMA_AES_TX,
		.flags	= IORESOURCE_DMA,
	},
	{
		.start	= OMAP24XX_DMA_AES_RX,
		.flags	= IORESOURCE_DMA,
	}
};
static int omap2_aes_resources_sz = ARRAY_SIZE(omap2_aes_resources);
#else
#define omap2_aes_resources		NULL
#define omap2_aes_resources_sz		0
#endif

#ifdef CONFIG_ARCH_OMAP3
static struct resource omap3_aes_resources[] = {
	{
		.start	= OMAP34XX_SEC_AES_BASE,
		.end	= OMAP34XX_SEC_AES_BASE + 0x4C,
		.flags	= IORESOURCE_MEM,
	},
	{
		.start	= OMAP34XX_DMA_AES2_TX,
		.flags	= IORESOURCE_DMA,
	},
	{
		.start	= OMAP34XX_DMA_AES2_RX,
		.flags	= IORESOURCE_DMA,
	}
};
static int omap3_aes_resources_sz = ARRAY_SIZE(omap3_aes_resources);
#else
#define omap3_aes_resources		NULL
#define omap3_aes_resources_sz		0
#endif

static struct platform_device aes_device = {
	.name		= "omap-aes",
	.id		= -1,
};

static void omap_init_aes(void)
{
	if (cpu_is_omap24xx()) {
		aes_device.resource = omap2_aes_resources;
		aes_device.num_resources = omap2_aes_resources_sz;
	} else if (cpu_is_omap34xx()) {
		aes_device.resource = omap3_aes_resources;
		aes_device.num_resources = omap3_aes_resources_sz;
	} else {
		pr_err("%s: platform not supported\n", __func__);
		return;
	}
	platform_device_register(&aes_device);
}

#else
static inline void omap_init_aes(void) { }
#endif

/*-------------------------------------------------------------------------*/

#if defined(CONFIG_ARCH_OMAP3) || defined(CONFIG_ARCH_OMAP4)

#define MMCHS_SYSCONFIG			0x0010
#define MMCHS_SYSCONFIG_SWRESET		(1 << 1)
#define MMCHS_SYSSTATUS			0x0014
#define MMCHS_SYSSTATUS_RESETDONE	(1 << 0)

static struct platform_device dummy_pdev = {
	.dev = {
		.bus = &platform_bus_type,
	},
};

/**
 * omap_hsmmc_reset() - Full reset of each HS-MMC controller
 *
 * Ensure that each MMC controller is fully reset.  Controllers
 * left in an unknown state (by bootloader) may prevent retention
 * or OFF-mode.  This is especially important in cases where the
 * MMC driver is not enabled, _or_ built as a module.
 *
 * In order for reset to work, interface, functional and debounce
 * clocks must be enabled.  The debounce clock comes from func_32k_clk
 * and is not under SW control, so we only enable i- and f-clocks.
 **/
static void __init omap_hsmmc_reset(void)
{
	u32 i, nr_controllers;
	struct clk *iclk, *fclk;

	if (cpu_is_omap242x() || cpu_is_ti81xx())
		return;

	nr_controllers = cpu_is_omap44xx() ? OMAP44XX_NR_MMC :
		(cpu_is_omap34xx() ? OMAP34XX_NR_MMC : OMAP24XX_NR_MMC);

	for (i = 0; i < nr_controllers; i++) {
		u32 v, base = 0;
		struct device *dev = &dummy_pdev.dev;

		switch (i) {
		case 0:
			base = OMAP2_MMC1_BASE;
			break;
		case 1:
			base = OMAP2_MMC2_BASE;
			break;
		case 2:
			base = OMAP3_MMC3_BASE;
			break;
		case 3:
			if (!cpu_is_omap44xx())
				return;
			base = OMAP4_MMC4_BASE;
			break;
		case 4:
			if (!cpu_is_omap44xx())
				return;
			base = OMAP4_MMC5_BASE;
			break;
		}

		if (cpu_is_omap44xx())
			base += OMAP4_MMC_REG_OFFSET;

		dummy_pdev.id = i;
		dev_set_name(&dummy_pdev.dev, "mmci-omap-hs.%d", i);
		iclk = clk_get(dev, "ick");
		if (IS_ERR(iclk))
			goto err1;
		if (clk_enable(iclk))
			goto err2;

		fclk = clk_get(dev, "fck");
		if (IS_ERR(fclk))
			goto err3;
		if (clk_enable(fclk))
			goto err4;

		omap_writel(MMCHS_SYSCONFIG_SWRESET, base + MMCHS_SYSCONFIG);
		v = omap_readl(base + MMCHS_SYSSTATUS);
		while (!(omap_readl(base + MMCHS_SYSSTATUS) &
			 MMCHS_SYSSTATUS_RESETDONE))
			cpu_relax();

		clk_disable(fclk);
		clk_put(fclk);
		clk_disable(iclk);
		clk_put(iclk);
	}
	return;

err4:
	clk_put(fclk);
err3:
	clk_disable(iclk);
err2:
	clk_put(iclk);
err1:
	printk(KERN_WARNING "%s: Unable to enable clocks for MMC%d, "
			    "cannot reset.\n",  __func__, i);
}
#else
static inline void omap_hsmmc_reset(void) {}
#endif

#if defined(CONFIG_MMC_OMAP) || defined(CONFIG_MMC_OMAP_MODULE) || \
	defined(CONFIG_MMC_OMAP_HS) || defined(CONFIG_MMC_OMAP_HS_MODULE)

static inline void omap2_mmc_mux(struct omap_mmc_platform_data *mmc_controller,
			int controller_nr)
{
	if (cpu_is_ti814x()) {
		omap_mux_init_signal("mmc1_sdwp", TI814X_PIN_INPUT_PULL_UP);
		omap_mux_init_signal("mmc1_sdcd", TI814X_PIN_INPUT_PULL_UP);

		omap_mux_init_signal("mmc1_clk", TI814X_PIN_OUTPUT_PULL_DOWN);
		omap_mux_init_signal("mmc1_cmd_mux0", TI814X_PIN_INPUT_PULL_UP);
		omap_mux_init_signal("mmc1_dat0", TI814X_PIN_INPUT_PULL_UP);
		omap_mux_init_signal("mmc1_dat1", TI814X_PIN_INPUT_PULL_UP);
		omap_mux_init_signal("mmc1_dat2", TI814X_PIN_INPUT_PULL_UP);
		omap_mux_init_signal("mmc1_dat3", TI814X_PIN_INPUT_PULL_UP);
	} else {
		if ((mmc_controller->slots[0].switch_pin > 0) && \
			(mmc_controller->slots[0].switch_pin < OMAP_MAX_GPIO_LINES)) {
			omap_mux_init_gpio(mmc_controller->slots[0].switch_pin,
						OMAP_PIN_INPUT_PULLUP);
		}
		if ((mmc_controller->slots[0].gpio_wp > 0) && \
			(mmc_controller->slots[0].gpio_wp < OMAP_MAX_GPIO_LINES)) {
			omap_mux_init_gpio(mmc_controller->slots[0].gpio_wp,
						OMAP_PIN_INPUT_PULLUP);
		}
	}

	if (cpu_is_omap2420() && controller_nr == 0) {
		omap_mux_init_signal("sdmmc_cmd", 0);
		omap_mux_init_signal("sdmmc_clki", 0);
		omap_mux_init_signal("sdmmc_clko", 0);
		omap_mux_init_signal("sdmmc_dat0", 0);
		omap_mux_init_signal("sdmmc_dat_dir0", 0);
		omap_mux_init_signal("sdmmc_cmd_dir", 0);
		if (mmc_controller->slots[0].caps & MMC_CAP_4_BIT_DATA) {
			omap_mux_init_signal("sdmmc_dat1", 0);
			omap_mux_init_signal("sdmmc_dat2", 0);
			omap_mux_init_signal("sdmmc_dat3", 0);
			omap_mux_init_signal("sdmmc_dat_dir1", 0);
			omap_mux_init_signal("sdmmc_dat_dir2", 0);
			omap_mux_init_signal("sdmmc_dat_dir3", 0);
		}

		/*
		 * Use internal loop-back in MMC/SDIO Module Input Clock
		 * selection
		 */
		if (mmc_controller->slots[0].internal_clock) {
			u32 v = omap_ctrl_readl(OMAP2_CONTROL_DEVCONF0);
			v |= (1 << 24);
			omap_ctrl_writel(v, OMAP2_CONTROL_DEVCONF0);
		}
	}

	if (cpu_is_omap34xx()) {
		if (controller_nr == 0) {
			omap_mux_init_signal("sdmmc1_clk",
				OMAP_PIN_INPUT_PULLUP);
			omap_mux_init_signal("sdmmc1_cmd",
				OMAP_PIN_INPUT_PULLUP);
			omap_mux_init_signal("sdmmc1_dat0",
				OMAP_PIN_INPUT_PULLUP);
			if (mmc_controller->slots[0].caps &
				(MMC_CAP_4_BIT_DATA | MMC_CAP_8_BIT_DATA)) {
				omap_mux_init_signal("sdmmc1_dat1",
					OMAP_PIN_INPUT_PULLUP);
				omap_mux_init_signal("sdmmc1_dat2",
					OMAP_PIN_INPUT_PULLUP);
				omap_mux_init_signal("sdmmc1_dat3",
					OMAP_PIN_INPUT_PULLUP);
			}
			if (mmc_controller->slots[0].caps &
						MMC_CAP_8_BIT_DATA) {
				omap_mux_init_signal("sdmmc1_dat4",
					OMAP_PIN_INPUT_PULLUP);
				omap_mux_init_signal("sdmmc1_dat5",
					OMAP_PIN_INPUT_PULLUP);
				omap_mux_init_signal("sdmmc1_dat6",
					OMAP_PIN_INPUT_PULLUP);
				omap_mux_init_signal("sdmmc1_dat7",
					OMAP_PIN_INPUT_PULLUP);
			}
		}
		if (controller_nr == 1) {
			/* MMC2 */
			omap_mux_init_signal("sdmmc2_clk",
				OMAP_PIN_INPUT_PULLUP);
			omap_mux_init_signal("sdmmc2_cmd",
				OMAP_PIN_INPUT_PULLUP);
			omap_mux_init_signal("sdmmc2_dat0",
				OMAP_PIN_INPUT_PULLUP);

			/*
			 * For 8 wire configurations, Lines DAT4, 5, 6 and 7 need to be muxed
			 * in the board-*.c files
			 */
			if (mmc_controller->slots[0].caps &
				(MMC_CAP_4_BIT_DATA | MMC_CAP_8_BIT_DATA)) {
				omap_mux_init_signal("sdmmc2_dat1",
					OMAP_PIN_INPUT_PULLUP);
				omap_mux_init_signal("sdmmc2_dat2",
					OMAP_PIN_INPUT_PULLUP);
				omap_mux_init_signal("sdmmc2_dat3",
					OMAP_PIN_INPUT_PULLUP);
			}
			if (mmc_controller->slots[0].caps &
							MMC_CAP_8_BIT_DATA) {
				omap_mux_init_signal("sdmmc2_dat4.sdmmc2_dat4",
					OMAP_PIN_INPUT_PULLUP);
				omap_mux_init_signal("sdmmc2_dat5.sdmmc2_dat5",
					OMAP_PIN_INPUT_PULLUP);
				omap_mux_init_signal("sdmmc2_dat6.sdmmc2_dat6",
					OMAP_PIN_INPUT_PULLUP);
				omap_mux_init_signal("sdmmc2_dat7.sdmmc2_dat7",
					OMAP_PIN_INPUT_PULLUP);
			}
		}

		/*
		 * For MMC3 the pins need to be muxed in the board-*.c files
		 */
	}

	if (cpu_is_ti816x()) {
		omap_mux_init_signal("mmc_pow", OMAP_PULL_ENA);
		omap_mux_init_signal("mmc_clk", OMAP_PIN_OUTPUT);
		omap_mux_init_signal("mmc_cmd", OMAP_PULL_UP);
		omap_mux_init_signal("mmc_dat0", OMAP_PULL_UP);
		omap_mux_init_signal("mmc_dat1_sdirq", OMAP_PULL_UP);
		omap_mux_init_signal("mmc_dat2_sdrw", OMAP_PULL_UP);
		omap_mux_init_signal("mmc_dat3", OMAP_PULL_UP);
		omap_mux_init_signal("mmc_sdcd", OMAP_PULL_ENA);
		omap_mux_init_signal("mmc_sdwp", OMAP_PULL_ENA);
	}
}

void __init omap2_init_mmc(struct omap_mmc_platform_data **mmc_data,
			int nr_controllers)
{
	int i;
	char *name;

	for (i = 0; i < nr_controllers; i++) {
		unsigned long base = 0;
		unsigned long size;
		unsigned int irq = 0;

		if (!mmc_data[i])
			continue;

		omap2_mmc_mux(mmc_data[i], i);

		switch (i) {
		case 0:
			if (!cpu_is_ti81xx()) {
				base = OMAP2_MMC1_BASE;
				irq = INT_24XX_MMC_IRQ;
			} else if (cpu_is_ti816x()) {
				base = TI816X_MMC1_BASE;
				irq = TI81XX_IRQ_SD;
			} else if (cpu_is_ti814x()) {
				base = TI814X_MMC1_BASE;
				irq = TI81XX_IRQ_SD;
			}
			break;
		case 1:
			if (cpu_is_ti814x()) {
				base = TI814X_MMC2_BASE;
				irq = TI814X_IRQ_SD1;
			} else {
				base = OMAP2_MMC2_BASE;
				irq = INT_24XX_MMC2_IRQ;
			}
			break;
		case 2:
			if (!cpu_is_omap44xx() && !cpu_is_omap34xx())
				return;
			base = OMAP3_MMC3_BASE;
			irq = INT_34XX_MMC3_IRQ;
			break;
		case 3:
			if (!cpu_is_omap44xx())
				return;
			base = OMAP4_MMC4_BASE;
			irq = OMAP44XX_IRQ_MMC4;
			break;
		case 4:
			if (!cpu_is_omap44xx())
				return;
			base = OMAP4_MMC5_BASE;
			irq = OMAP44XX_IRQ_MMC5;
			break;
		default:
			continue;
		}

		if (cpu_is_omap2420()) {
			size = OMAP2420_MMC_SIZE;
			name = "mmci-omap";
		} else if (cpu_is_omap44xx()) {
			if (i < 3)
				irq += OMAP44XX_IRQ_GIC_START;
			size = OMAP4_HSMMC_SIZE;
			name = "mmci-omap-hs";
		} else if (cpu_is_ti81xx()) {
			size = TI81XX_HSMMC_SIZE;
			name = "mmci-omap-hs";
		} else {
			size = OMAP3_HSMMC_SIZE;
			name = "mmci-omap-hs";
		}
		omap_mmc_add(name, i, base, size, irq, mmc_data[i]);
	};
}

#endif

/*-------------------------------------------------------------------------*/

#if defined(CONFIG_HDQ_MASTER_OMAP) || defined(CONFIG_HDQ_MASTER_OMAP_MODULE)
#if defined(CONFIG_ARCH_OMAP2430) || defined(CONFIG_ARCH_OMAP3430)
#define OMAP_HDQ_BASE	0x480B2000
#endif
static struct resource omap_hdq_resources[] = {
	{
		.start		= OMAP_HDQ_BASE,
		.end		= OMAP_HDQ_BASE + 0x1C,
		.flags		= IORESOURCE_MEM,
	},
	{
		.start		= INT_24XX_HDQ_IRQ,
		.flags		= IORESOURCE_IRQ,
	},
};
static struct platform_device omap_hdq_dev = {
	.name = "omap_hdq",
	.id = 0,
	.dev = {
		.platform_data = NULL,
	},
	.num_resources	= ARRAY_SIZE(omap_hdq_resources),
	.resource	= omap_hdq_resources,
};
static inline void omap_hdq_init(void)
{
	(void) platform_device_register(&omap_hdq_dev);
}
#else
static inline void omap_hdq_init(void) {}
#endif

/*---------------------------------------------------------------------------*/

#if defined(CONFIG_VIDEO_OMAP2_VOUT) || \
	defined(CONFIG_VIDEO_OMAP2_VOUT_MODULE)
#if defined(CONFIG_FB_OMAP2) || defined(CONFIG_FB_OMAP2_MODULE)
static struct resource omap_vout_resource[3 - CONFIG_FB_OMAP2_NUM_FBS] = {
};
#else
static struct resource omap_vout_resource[2] = {
};
#endif

static struct platform_device omap_vout_device = {
	.name		= "omap_vout",
	.num_resources	= ARRAY_SIZE(omap_vout_resource),
	.resource 	= &omap_vout_resource[0],
	.id		= -1,
};
static void omap_init_vout(void)
{
	if (platform_device_register(&omap_vout_device) < 0)
		printk(KERN_ERR "Unable to register OMAP-VOUT device\n");
}
#else
static inline void omap_init_vout(void) {}
#endif


#ifdef CONFIG_ARCH_TI816X

#define TI816X_EMAC1_BASE		(0x4A100000)
#define TI816X_EMAC2_BASE		(0x4A120000)
#define TI816X_EMAC_CNTRL_OFFSET	(0x0)
#define TI816X_EMAC_CNTRL_MOD_OFFSET	(0x900)
#define TI816X_EMAC_CNTRL_RAM_OFFSET	(0x2000)
#define TI816X_EMAC_MDIO_OFFSET		(0x800)
#define TI816X_EMAC_CNTRL_RAM_SIZE	(0x2000)
#define TI816X_EMAC1_HW_RAM_ADDR	(0x4A102000)
#define TI816X_EMAC2_HW_RAM_ADDR	(0x4A122000)

#define TI816X_EMAC_PHY_MASK		(0xF)
#define TI816X_EMAC_MDIO_FREQ		(1000000)

static struct mdio_platform_data ti816x_mdio_pdata = {
	.bus_freq       = TI816X_EMAC_MDIO_FREQ,
};

static struct resource ti816x_mdio_resources[] = {
	{
		.start  = TI816X_EMAC1_BASE + TI816X_EMAC_MDIO_OFFSET,
		.end	= TI816X_EMAC1_BASE + TI816X_EMAC_MDIO_OFFSET +
				SZ_256 - 1,
		.flags  = IORESOURCE_MEM,
	}
};

static struct platform_device ti816x_mdio_device = {
	.name           = "davinci_mdio",
	.id             = 0,
	.num_resources  = ARRAY_SIZE(ti816x_mdio_resources),
	.resource	= ti816x_mdio_resources,
	.dev.platform_data = &ti816x_mdio_pdata,
};

static struct emac_platform_data ti816x_emac1_pdata = {
	.rmii_en	=	0,
	.phy_id		=	"0:01",
	.gigabit_en	=	1,
};

static struct emac_platform_data ti816x_emac2_pdata = {
	.rmii_en	=	0,
	.phy_id		=	"0:02",
	.gigabit_en	=	1,
};

static struct resource ti816x_emac1_resources[] = {
	{
		.start	=	TI816X_EMAC1_BASE,
		.end	=	TI816X_EMAC1_BASE + 0x3FFF,
		.flags	=	IORESOURCE_MEM,
	},
	{
		.start	=	TI816X_IRQ_MACRXTHR0,
		.end	=	TI816X_IRQ_MACRXTHR0,
		.flags	=	IORESOURCE_IRQ,
	},
	{
		.start	=	TI816X_IRQ_MACRXINT0,
		.end	=	TI816X_IRQ_MACRXINT0,
		.flags	=	IORESOURCE_IRQ,
	},
	{
		.start	=	TI816X_IRQ_MACTXINT0,
		.end	=	TI816X_IRQ_MACTXINT0,
		.flags	=	IORESOURCE_IRQ,
	},
	{
		.start	=	TI816X_IRQ_MACMISC0,
		.end	=	TI816X_IRQ_MACMISC0,
		.flags	=	IORESOURCE_IRQ,
	},
};

static struct resource ti816x_emac2_resources[] = {
	{
		.start	=	TI816X_EMAC2_BASE,
		.end	=	TI816X_EMAC2_BASE + 0x3FFF,
		.flags	=	IORESOURCE_MEM,
	},
	{
		.start	=	TI816X_IRQ_MACRXTHR1,
		.end	=	TI816X_IRQ_MACRXTHR1,
		.flags	=	IORESOURCE_IRQ,
	},
	{
		.start	=	TI816X_IRQ_MACRXINT1,
		.end	=	TI816X_IRQ_MACRXINT1,
		.flags	=	IORESOURCE_IRQ,
	},
	{
		.start	=	TI816X_IRQ_MACTXINT1,
		.end	=	TI816X_IRQ_MACTXINT1,
		.flags	=	IORESOURCE_IRQ,
	},
	{
		.start	=	TI816X_IRQ_MACMISC1,
		.end	=	TI816X_IRQ_MACMISC1,
		.flags	=	IORESOURCE_IRQ,
	},
};
static struct platform_device ti816x_emac1_device = {
	.name	=	"davinci_emac",
	.id	=	0,
	.num_resources	=	ARRAY_SIZE(ti816x_emac1_resources),
	.resource	=	ti816x_emac1_resources,
};

static struct platform_device ti816x_emac2_device = {
	.name	=	"davinci_emac",
	.id	=	1,
	.num_resources	=	ARRAY_SIZE(ti816x_emac2_resources),
	.resource	=	ti816x_emac2_resources,
};

void ti816x_emac_mux(void)
{
	omap_mux_init_signal("gmii1_rxclk", OMAP_MUX_MODE1);
	omap_mux_init_signal("gmii1_rxd0", OMAP_MUX_MODE1);
	omap_mux_init_signal("gmii1_rxd1", OMAP_MUX_MODE1);
	omap_mux_init_signal("gmii1_rxd2", OMAP_MUX_MODE1);
	omap_mux_init_signal("gmii1_rxd3", OMAP_MUX_MODE1);
	omap_mux_init_signal("gmii1_rxd4", OMAP_MUX_MODE1);
	omap_mux_init_signal("gmii1_rxd5", OMAP_MUX_MODE1);
	omap_mux_init_signal("gmii1_rxd6", OMAP_MUX_MODE1);
	omap_mux_init_signal("gmii1_rxd7", OMAP_MUX_MODE1);
	omap_mux_init_signal("gmii1_rxdv", OMAP_MUX_MODE1);
	omap_mux_init_signal("gmii1_gtxclk", OMAP_MUX_MODE1);
	omap_mux_init_signal("gmii1_txd0", OMAP_MUX_MODE1);
	omap_mux_init_signal("gmii1_txd1", OMAP_MUX_MODE1);
	omap_mux_init_signal("gmii1_txd2", OMAP_MUX_MODE1);
	omap_mux_init_signal("gmii1_txd3", OMAP_MUX_MODE1);
	omap_mux_init_signal("gmii1_txd4", OMAP_MUX_MODE1);
	omap_mux_init_signal("gmii1_txd5", OMAP_MUX_MODE1);
	omap_mux_init_signal("gmii1_txd6", OMAP_MUX_MODE1);
	omap_mux_init_signal("gmii1_txd7", OMAP_MUX_MODE1);
	omap_mux_init_signal("gmii1_txen", OMAP_MUX_MODE1);
	omap_mux_init_signal("gmii1_txclk", OMAP_MUX_MODE1);
	omap_mux_init_signal("gmii1_col", OMAP_MUX_MODE1);
	omap_mux_init_signal("gmii1_crs", OMAP_MUX_MODE1);
	omap_mux_init_signal("gmii1_rxer", OMAP_MUX_MODE1);
}



void ti816x_ethernet_init(void)
{
	u32 mac_lo, mac_hi;

	mac_lo = omap_ctrl_readl(TI81XX_CONTROL_MAC_ID0_LO);
	mac_hi = omap_ctrl_readl(TI81XX_CONTROL_MAC_ID0_HI);
	ti816x_emac1_pdata.mac_addr[0] = mac_hi & 0xFF;
	ti816x_emac1_pdata.mac_addr[1] = (mac_hi & 0xFF00) >> 8;
	ti816x_emac1_pdata.mac_addr[2] = (mac_hi & 0xFF0000) >> 16;
	ti816x_emac1_pdata.mac_addr[3] = (mac_hi & 0xFF000000) >> 24;
	ti816x_emac1_pdata.mac_addr[4] = mac_lo & 0xFF;
	ti816x_emac1_pdata.mac_addr[5] = (mac_lo & 0xFF00) >> 8;

	ti816x_emac1_pdata.ctrl_reg_offset = TI816X_EMAC_CNTRL_OFFSET;
	ti816x_emac1_pdata.ctrl_mod_reg_offset = TI816X_EMAC_CNTRL_MOD_OFFSET;
	ti816x_emac1_pdata.ctrl_ram_offset = TI816X_EMAC_CNTRL_RAM_OFFSET;
	ti816x_emac1_pdata.ctrl_ram_size = TI816X_EMAC_CNTRL_RAM_SIZE;
	ti816x_emac1_pdata.version = EMAC_VERSION_2;
	ti816x_emac1_pdata.hw_ram_addr = TI816X_EMAC1_HW_RAM_ADDR;
	ti816x_emac1_pdata.interrupt_enable = NULL;
	ti816x_emac1_pdata.interrupt_disable = NULL;
	ti816x_emac1_device.dev.platform_data = &ti816x_emac1_pdata;
	platform_device_register(&ti816x_emac1_device);

	platform_device_register(&ti816x_mdio_device);
	clk_add_alias(NULL, dev_name(&ti816x_mdio_device.dev),
			NULL, &ti816x_emac1_device.dev);

	mac_lo = omap_ctrl_readl(TI81XX_CONTROL_MAC_ID1_LO);
	mac_hi = omap_ctrl_readl(TI81XX_CONTROL_MAC_ID1_HI);
	ti816x_emac2_pdata.mac_addr[0] = mac_hi & 0xFF;
	ti816x_emac2_pdata.mac_addr[1] = (mac_hi & 0xFF00) >> 8;
	ti816x_emac2_pdata.mac_addr[2] = (mac_hi & 0xFF0000) >> 16;
	ti816x_emac2_pdata.mac_addr[3] = (mac_hi & 0xFF000000) >> 24;
	ti816x_emac2_pdata.mac_addr[4] = mac_lo & 0xFF;
	ti816x_emac2_pdata.mac_addr[5] = (mac_lo & 0xFF00) >> 8;

	ti816x_emac2_pdata.ctrl_reg_offset = TI816X_EMAC_CNTRL_OFFSET;
	ti816x_emac2_pdata.ctrl_mod_reg_offset = TI816X_EMAC_CNTRL_MOD_OFFSET;
	ti816x_emac2_pdata.ctrl_ram_offset = TI816X_EMAC_CNTRL_RAM_OFFSET;
	ti816x_emac2_pdata.ctrl_ram_size = TI816X_EMAC_CNTRL_RAM_SIZE;
	ti816x_emac2_pdata.version = EMAC_VERSION_2;
	ti816x_emac2_pdata.hw_ram_addr = TI816X_EMAC2_HW_RAM_ADDR;
	ti816x_emac2_pdata.interrupt_enable = NULL;
	ti816x_emac2_pdata.interrupt_disable = NULL;
	ti816x_emac2_device.dev.platform_data = &ti816x_emac2_pdata;
	platform_device_register(&ti816x_emac2_device);

	ti816x_emac_mux();
}
#else
static inline void ti816x_ethernet_init(void) {}
#endif

#ifdef CONFIG_TI816X_SMARTREFLEX

/* smartreflex platform data */
#define TI816X_SR_HVT_CNTRL_OFFSET	0x06AC
#define TI816X_SR_SVT_CNTRL_OFFSET	0x06A8
#define TI816X_SR_HVT_ERR2VOLT_GAIN	0xD
#define TI816X_SR_SVT_ERR2VOLT_GAIN	0x12
#define TI816X_SR_HVT_ERR_MIN_LIMIT	0xF6
#define TI816X_SR_SVT_ERR_MIN_LIMIT	0xF8

/* Refer TRM to know the Err2VoltGain factor and MinError Limits
 * for different step sizes. Update this table for both the sensors
 * (HVT and SVT) according to your step size, default step size is
 * 15mV. Factors changing with step-size are e2v_gain, err_minlimit.
 * Don't forgot to change the step size in platform data structure,
 * ti816x_sr_pdata.
 */
static struct ti816x_sr_sdata sr_sensor_data[] = {
	{
		.efuse_offs	= TI816X_SR_HVT_CNTRL_OFFSET,
		.e2v_gain	= TI816X_SR_HVT_ERR2VOLT_GAIN,
		.err_minlimit	= TI816X_SR_HVT_ERR_MIN_LIMIT,
		.err_maxlimit	= 0x2,
		.err_weight	= 0x4,
		.senn_mod	= 0x1,
		.senp_mod	= 0x1,
	},
	{
		.efuse_offs	= TI816X_SR_SVT_CNTRL_OFFSET,
		.e2v_gain	= TI816X_SR_SVT_ERR2VOLT_GAIN,
		.err_minlimit	= TI816X_SR_SVT_ERR_MIN_LIMIT,
		.err_maxlimit	= 0x2,
		.err_weight	= 0x4,
		.senn_mod	= 0x1,
		.senp_mod	= 0x1,
	},
};

static struct ti816x_sr_platform_data ti816x_sr_pdata = {
	.vd_name		= "vdd_avs",
	.ip_type		= 2,
	.irq_delay		= 2000,
	.no_of_vds		= 1,
	.no_of_sens		= ARRAY_SIZE(sr_sensor_data),
	.vstep_size_uv		= 15000,
	.enable_on_init		= true,
	.sr_sdata		= sr_sensor_data,
};

static struct resource ti816x_sr_resources[] = {
	{
		.name	=	"sr_hvt",
		.start	=	TI816X_SR0_BASE,
		.end	=	TI816X_SR0_BASE + SZ_4K - 1,
		.flags	=	IORESOURCE_MEM,
	},
	{
		.name	=	"sr_hvt",
		.start	=	TI81XX_IRQ_SMRFLX0,
		.end	=	TI81XX_IRQ_SMRFLX0,
		.flags	=	IORESOURCE_IRQ,
	},
	{
		.name	=	"sr_svt",
		.start	=	TI816X_SR1_BASE,
		.end	=	TI816X_SR1_BASE + SZ_4K - 1,
		.flags	=	IORESOURCE_MEM,
	},
	{
		.name	=	"sr_svt",
		.start	=	TI81XX_IRQ_SMRFLX1,
		.end	=	TI81XX_IRQ_SMRFLX1,
		.flags	=	IORESOURCE_IRQ,
	},
};

/* VCORE for SR regulator init */
static struct platform_device ti816x_sr_device = {
	.name		= "smartreflex",
	.id		= -1,
	.num_resources	= ARRAY_SIZE(ti816x_sr_resources),
	.resource	= ti816x_sr_resources,
	.dev = {
		.platform_data = &ti816x_sr_pdata,
	},
};

static void __init ti816x_sr_init(void)
{
	if (platform_device_register(&ti816x_sr_device))
		printk(KERN_ERR "failed to register ti816x_sr device\n");
	else
		printk(KERN_INFO "registered ti816x_sr device\n");
}
#else
static inline void ti816x_sr_init(void) {}
#endif

#if defined(CONFIG_ARCH_TI81XX)

#define TI81XX_TPCC_BASE		0x49000000
#define TI81XX_TPTC0_BASE		0x49800000
#define TI81XX_TPTC1_BASE		0x49900000
#define TI81XX_TPTC2_BASE		0x49a00000
#define TI81XX_TPTC3_BASE		0x49b00000

#define TI81XX_SCM_BASE_EDMA		0x00000f90

static struct resource ti81xx_edma_resources[] = {
	{
		.name	= "edma_cc0",
		.start	= TI81XX_TPCC_BASE,
		.end	= TI81XX_TPCC_BASE + SZ_32K - 1,
		.flags	= IORESOURCE_MEM,
	},
	{
		.name	= "edma_tc0",
		.start	= TI81XX_TPTC0_BASE,
		.end	= TI81XX_TPTC0_BASE + SZ_1K - 1,
		.flags	= IORESOURCE_MEM,
	},
	{
		.name	= "edma_tc1",
		.start	= TI81XX_TPTC1_BASE,
		.end	= TI81XX_TPTC1_BASE + SZ_1K - 1,
		.flags	= IORESOURCE_MEM,
	},
	{
		.name	= "edma_tc2",
		.start	= TI81XX_TPTC2_BASE,
		.end	= TI81XX_TPTC2_BASE + SZ_1K - 1,
		.flags	= IORESOURCE_MEM,
	},
	{
		.name	= "edma_tc3",
		.start	= TI81XX_TPTC3_BASE,
		.end	= TI81XX_TPTC3_BASE + SZ_1K - 1,
		.flags	= IORESOURCE_MEM,
	},
	{
		.name	= "edma0",
		.start	= TI81XX_IRQ_EDMA_COMP,
		.flags	= IORESOURCE_IRQ,
	},
	{
		.name	= "edma0_err",
		.start	= TI81XX_IRQ_EDMA_ERR,
		.flags	= IORESOURCE_IRQ,
	},
};

static const s16 ti816x_dma_rsv_chans[][2] = {
	/* (offset, number) */
	{0, 4},
	{26, 6},
	{48, 4},
	{56, 8},
	{-1, -1}
};

static const s16 ti816x_dma_rsv_slots[][2] = {
	/* (offset, number) */
	{0, 4},
	{26, 6},
	{48, 4},
	{56, 8},
	{64, 127},
	{-1, -1}
};

/* Four Transfer Controllers on TI816X */
static const s8 ti816x_queue_tc_mapping[][2] = {
	/* {event queue no, TC no} */
	{0, 0},
	{1, 1},
	{2, 2},
	{3, 3},
	{-1, -1}
};

static const s8 ti816x_queue_priority_mapping[][2] = {
	/* {event queue no, Priority} */
	{0, 0},
	{1, 1},
	{2, 2},
	{3, 3},
	{-1, -1}
};

static struct edma_soc_info ti816x_edma_info[] = {
	{
		.n_channel		= 64,
		.n_region		= 5,	/* 0-2, 4-5 */
		.n_slot			= 512,
		.n_tc			= 4,
		.n_cc			= 1,
		.rsv_chans		= ti816x_dma_rsv_chans,
		.rsv_slots		= ti816x_dma_rsv_slots,
		.queue_tc_mapping	= ti816x_queue_tc_mapping,
		.queue_priority_mapping	= ti816x_queue_priority_mapping,
	},
};

static struct platform_device ti816x_edma_device = {
	.name		= "edma",
	.id		= -1,
	.dev = {
		.platform_data = ti816x_edma_info,
	},
	.num_resources	= ARRAY_SIZE(ti81xx_edma_resources),
	.resource	= ti81xx_edma_resources,
};

static const s16 ti814x_dma_rsv_chans[][2] = {
	/* (offset, number) */
	{0, 2},
	{14, 2},
	{26, 6},
	{48, 4},
	{56, 8},
	{-1, -1}
};

static const s16 ti814x_dma_rsv_slots[][2] = {
	/* (offset, number) */
	{0, 2},
	{14, 2},
	{26, 6},
	{48, 4},
	{56, 8},
	{64, 127},
	{-1, -1}
};

/* Four Transfer Controllers on TI814X */
static const s8 ti814x_queue_tc_mapping[][2] = {
	/* {event queue no, TC no} */
	{0, 0},
	{1, 1},
	{2, 2},
	{3, 3},
	{-1, -1}
};

static const s8 ti814x_queue_priority_mapping[][2] = {
	/* {event queue no, Priority} */
	{0, 0},
	{1, 1},
	{2, 2},
	{3, 3},
	{-1, -1}
};

static struct event_to_channel_map ti814x_xbar_event_mapping[] = {
	/* {xbar event no, Channel} */
	{1, -1},
	{2, -1},
	{3, -1},
	{4, -1},
	{5, -1},
	{6, -1},
	{7, -1},
	{8, -1},
	{9, -1},
	{10, -1},
	{11, -1},
	{12, -1},
	{13, -1},
	{14, -1},
	{15, -1},
	{16, -1},
	{17, -1},
	{18, -1},
	{19, -1},
	{20, -1},
	{21, -1},
	{22, -1},
	{23, -1},
	{24, -1},
	{25, -1},
	{26, -1},
	{27, -1},
	{28, -1},
	{29, -1},
	{30, -1},
	{31, -1},
	{-1, -1}
};

/**
 * map_xbar_event_to_channel - maps a crossbar event to a DMA channel
 * according to the configuration provided
 * @event: the event number for which mapping is required
 * @channel: channel being activated
 * @xbar_event_mapping: array that has the event to channel map
 *
 * Events that are routed by default are not mapped. Only events that
 * are crossbar mapped are routed to available channels according to
 * the configuration provided
 *
 * Returns zero on success, else negative errno.
 */
int map_xbar_event_to_channel(unsigned event, unsigned *channel,
			struct event_to_channel_map *xbar_event_mapping)
{
	unsigned ctrl = 0;
	unsigned xbar_evt_no = 0;
	unsigned val = 0;
	unsigned offset = 0;
	unsigned mask = 0;

	ctrl = EDMA_CTLR(event);
	xbar_evt_no = event - (edma_info[ctrl]->num_channels);

	if (event < edma_info[ctrl]->num_channels) {
		*channel = event;
	} else if (event < edma_info[ctrl]->num_events) {
		*channel = xbar_event_mapping[xbar_evt_no].channel_no;
		/* confirm the range */
		if (*channel < EDMA_MAX_DMACH)
			clear_bit(*channel, edma_info[ctrl]->edma_unused);
		mask = (*channel)%4;
		offset = (*channel)/4;
		offset *= 4;
		offset += mask;
		val = (unsigned)__raw_readl(TI81XX_CTRL_REGADDR(
					TI81XX_SCM_BASE_EDMA + offset));
		val = val & (~(0xFF));
		val = val | (xbar_event_mapping[xbar_evt_no].xbar_event_no);
		__raw_writel(val, TI81XX_CTRL_REGADDR(TI81XX_SCM_BASE_EDMA
								+ offset));
		return 0;
	} else {
		return -EINVAL;
	}

	return 0;
}
EXPORT_SYMBOL(map_xbar_event_to_channel);

static struct edma_soc_info ti814x_edma_info[] = {
	{
		.n_channel		= 64,
		.n_region		= 5,	/* 0-2, 4-5 */
		.n_slot			= 512,
		.n_tc			= 4,
		.n_cc			= 1,
		.rsv_chans		= ti814x_dma_rsv_chans,
		.rsv_slots		= ti814x_dma_rsv_slots,
		.queue_tc_mapping	= ti814x_queue_tc_mapping,
		.queue_priority_mapping	= ti814x_queue_priority_mapping,
		.is_xbar		= 1,
		.n_events		= 95,
		.xbar_event_mapping	= ti814x_xbar_event_mapping,
		.map_xbar_channel	= map_xbar_event_to_channel,
	},
};

static struct platform_device ti814x_edma_device = {
	.name		= "edma",
	.id		= -1,
	.dev = {
		.platform_data = ti814x_edma_info,
	},
	.num_resources	= ARRAY_SIZE(ti81xx_edma_resources),
	.resource	= ti81xx_edma_resources,
};

int __init ti81xx_register_edma(void)
{
	struct platform_device *pdev;
	static struct clk *edma_clk;

	if (cpu_is_ti816x())
		pdev = &ti816x_edma_device;
	else if (cpu_is_ti814x())
		pdev = &ti814x_edma_device;
	else {
		pr_err("%s: platform not supported\n", __func__);
		return -ENODEV;
	}

	edma_clk = clk_get(NULL, "tpcc_ick");
	if (IS_ERR(edma_clk)) {
		printk(KERN_ERR "EDMA: Failed to get clock\n");
		return -EBUSY;
	}
	clk_enable(edma_clk);
	edma_clk = clk_get(NULL, "tptc0_ick");
	if (IS_ERR(edma_clk)) {
		printk(KERN_ERR "EDMA: Failed to get clock\n");
		return -EBUSY;
	}
	clk_enable(edma_clk);
	edma_clk = clk_get(NULL, "tptc1_ick");
	if (IS_ERR(edma_clk)) {
		printk(KERN_ERR "EDMA: Failed to get clock\n");
		return -EBUSY;
	}
	clk_enable(edma_clk);
	edma_clk = clk_get(NULL, "tptc2_ick");
	if (IS_ERR(edma_clk)) {
		printk(KERN_ERR "EDMA: Failed to get clock\n");
		return -EBUSY;
	}
	clk_enable(edma_clk);
	edma_clk = clk_get(NULL, "tptc3_ick");
	if (IS_ERR(edma_clk)) {
		printk(KERN_ERR "EDMA: Failed to get clock\n");
		return -EBUSY;
	}
	clk_enable(edma_clk);


	return platform_device_register(pdev);
}
#if	!defined(CONFIG_MACH_DM385IPNC) && !defined(CONFIG_MACH_TI8148IPNC)
static void __init ti81xx_video_mux(void)
{
	if (cpu_is_ti816x()) {
		omap_mux_init_signal("vin1_d9",  OMAP_MUX_MODE2);
		omap_mux_init_signal("vin1_d10", OMAP_MUX_MODE2);
		omap_mux_init_signal("vin1_d11", OMAP_MUX_MODE2);
		omap_mux_init_signal("vin1_d12", OMAP_MUX_MODE2);
		omap_mux_init_signal("vin1_d13", OMAP_MUX_MODE2);
		omap_mux_init_signal("vin1_d14", OMAP_MUX_MODE1);
		omap_mux_init_signal("vin0_d20", OMAP_MUX_MODE1);
		omap_mux_init_signal("vin0_d21", OMAP_MUX_MODE1);
		omap_mux_init_signal("vin0_d22", OMAP_MUX_MODE1);
		omap_mux_init_signal("vin0_d23", OMAP_MUX_MODE1);
		omap_mux_init_signal("vin1_d4",  OMAP_MUX_MODE2);
		omap_mux_init_signal("vin1_d5",  OMAP_MUX_MODE2);
		omap_mux_init_signal("vin1_d6",  OMAP_MUX_MODE2);
		omap_mux_init_signal("vin1_d7",  OMAP_MUX_MODE2);
		omap_mux_init_signal("vin1_d8",  OMAP_MUX_MODE2);
		omap_mux_init_signal("vin1_d15", OMAP_MUX_MODE2);
		omap_mux_init_signal("vin0_d16", OMAP_MUX_MODE1);
		omap_mux_init_signal("vin0_d17", OMAP_MUX_MODE1);
		omap_mux_init_signal("vin0_d18", OMAP_MUX_MODE1);
		omap_mux_init_signal("vin0_d19", OMAP_MUX_MODE1);
		omap_mux_init_signal("vout0_r_cr0", OMAP_MUX_MODE1);
		omap_mux_init_signal("tsi5_data", OMAP_MUX_MODE1);
		omap_mux_init_signal("tsi5_bytstrt", OMAP_MUX_MODE1);
		omap_mux_init_signal("tsi5_pacval", OMAP_MUX_MODE1);
		omap_mux_init_signal("tsi5_pacerr", OMAP_MUX_MODE1);
		omap_mux_init_signal("vin1_clk1",   OMAP_MUX_MODE2);
		omap_mux_init_signal("vin0_hsync0", OMAP_MUX_MODE1);
		omap_mux_init_signal("vin0_vsync0", OMAP_MUX_MODE1);
		omap_mux_init_signal("vin0_fld0",   OMAP_MUX_MODE1);
		omap_mux_init_signal("vin0_de0",    OMAP_MUX_MODE1);
		omap_mux_init_signal("tsi7_dclk",    OMAP_MUX_MODE1);
		omap_mux_init_signal("tsi7_data",    OMAP_MUX_MODE1);
		omap_mux_init_signal("tsi7_bytstrt",    OMAP_MUX_MODE1);
		omap_mux_init_signal("tsi7_pacval",    OMAP_MUX_MODE1);
		omap_mux_init_signal("vout0_r_cr1",    OMAP_MUX_MODE1);
		omap_mux_init_signal("vin1_clk0",    OMAP_MUX_MODE2);
		omap_mux_init_signal("vin1_d0",    OMAP_MUX_MODE2);
		omap_mux_init_signal("vin1_d1",    OMAP_MUX_MODE2);
		omap_mux_init_signal("vin1_d2",    OMAP_MUX_MODE2);
		omap_mux_init_signal("vin1_d3",    OMAP_MUX_MODE2);
		omap_mux_init_signal("vout0_r_cr2",    OMAP_MUX_MODE0);
		omap_mux_init_signal("vout0_r_cr3",    OMAP_MUX_MODE0);
		omap_mux_init_signal("vout0_r_cr4",    OMAP_MUX_MODE0);
		omap_mux_init_signal("vout0_r_cr5",    OMAP_MUX_MODE0);
		omap_mux_init_signal("vout0_r_cr6",    OMAP_MUX_MODE0);
		omap_mux_init_signal("vout0_r_cr7",    OMAP_MUX_MODE0);
		omap_mux_init_signal("vout0_r_cr8",    OMAP_MUX_MODE0);
		omap_mux_init_signal("vout0_r_cr9",    OMAP_MUX_MODE0);
		omap_mux_init_signal("iic1_scl", OMAP_MUX_MODE0 | OMAP_PULL_UP);
		omap_mux_init_signal("iic1_sda", OMAP_MUX_MODE0 | OMAP_PULL_UP);

	} else if (cpu_is_ti811x()){
		/*Temporary hack, till we have interface
		 to configure pin mux for J5-Eco*/

		/*I2c2 - Pin Muxing*/
		omap_writel(0x60040, 0x48140A18);
		omap_writel(0x60040, 0x48140A1c);
		omap_writel(0x1, 0x48140924);
		omap_writel(0x1, 0x48140928);
		omap_writel(0x1, 0x481409a0);
		omap_writel(0x1, 0x481409a4);
		omap_writel(0x1, 0x48140a6c);
		omap_writel(0x1, 0x48140b8c);
		omap_writel(0x1, 0x48140b90);
		omap_writel(0x1, 0x48140ba8);

		/*Pin Muxing for Vout1*/
		omap_writel(0x1, 0x48140B2c);
		omap_writel(0x1, 0x481409f0);
		omap_writel(0x1, 0x48140b30);
		omap_writel(0x1, 0x48140b34);
		omap_writel(0x1, 0x48140B38);
		omap_writel(0x1, 0x48140B3C);
		omap_writel(0x1, 0x48140B40);
		omap_writel(0x1, 0x48140B44);
		omap_writel(0x1, 0x48140B48);
		omap_writel(0x1, 0x48140B4C);
		omap_writel(0x1, 0x48140B50);
		omap_writel(0x1, 0x48140B54);
		omap_writel(0x1, 0x48140B58);
		omap_writel(0x1, 0x48140B5C);
		omap_writel(0x1, 0x48140B60);
		omap_writel(0x1, 0x48140B64);
		omap_writel(0x1, 0x48140B68);
		omap_writel(0x1, 0x48140B6C);
		omap_writel(0x1, 0x48140B70);
		omap_writel(0x1, 0x48140B74);
		omap_writel(0x1, 0x48140B78);
		omap_writel(0x1, 0x48140B7C);
		omap_writel(0x1, 0x48140B80);
		omap_writel(0x1, 0x48140B84);
		omap_writel(0x1, 0x48140B88);
		omap_writel(0x1, 0x48140B8C);
		omap_writel(0x1, 0x48140B90);
		omap_writel(0x1, 0x48140B94);

	} else if (cpu_is_ti814x())	{
		/*VOUT0 DVO2 configuration*/
		omap_mux_init_signal("vout0_fid_mux1", OMAP_MUX_MODE0);
		omap_mux_init_signal("vout0_clk", OMAP_MUX_MODE0);
		omap_mux_init_signal("vout0_hsync", OMAP_MUX_MODE0);
		omap_mux_init_signal("vout0_vsync", OMAP_MUX_MODE0);
		//omap_mux_init_signal("vout0_avid", OMAP_MUX_MODE0);
		omap_mux_init_signal("vout0_b_cb_c2", OMAP_MUX_MODE0);
		omap_mux_init_signal("vout0_b_cb_c3", OMAP_MUX_MODE0);
		omap_mux_init_signal("vout0_b_cb_c4", OMAP_MUX_MODE0);
		omap_mux_init_signal("vout0_b_cb_c5", OMAP_MUX_MODE0);
		omap_mux_init_signal("vout0_b_cb_c6", OMAP_MUX_MODE0);
		omap_mux_init_signal("vout0_b_cb_c7", OMAP_MUX_MODE0);
		omap_mux_init_signal("vout0_b_cb_c8", OMAP_MUX_MODE0);
		omap_mux_init_signal("vout0_b_cb_c9", OMAP_MUX_MODE0);
		omap_mux_init_signal("vout0_g_y_yc2", OMAP_MUX_MODE0);
		omap_mux_init_signal("vout0_g_y_yc3", OMAP_MUX_MODE0);
		omap_mux_init_signal("vout0_g_y_yc4", OMAP_MUX_MODE0);
		omap_mux_init_signal("vout0_g_y_yc5", OMAP_MUX_MODE0);
		omap_mux_init_signal("vout0_g_y_yc6", OMAP_MUX_MODE0);
		omap_mux_init_signal("vout0_g_y_yc7", OMAP_MUX_MODE0);
		omap_mux_init_signal("vout0_g_y_yc8", OMAP_MUX_MODE0);
		omap_mux_init_signal("vout0_g_y_yc9", OMAP_MUX_MODE0);
		omap_mux_init_signal("vout0_r_cr2", OMAP_MUX_MODE0);
		omap_mux_init_signal("vout0_r_cr3", OMAP_MUX_MODE0);
		omap_mux_init_signal("vout0_r_cr4", OMAP_MUX_MODE0);
		omap_mux_init_signal("vout0_r_cr5", OMAP_MUX_MODE0);
		omap_mux_init_signal("vout0_r_cr6", OMAP_MUX_MODE0);
		omap_mux_init_signal("vout0_r_cr7", OMAP_MUX_MODE0);
		omap_mux_init_signal("vout0_r_cr8", OMAP_MUX_MODE0);
		omap_mux_init_signal("vout0_r_cr9", OMAP_MUX_MODE0);
#if 0
		/*HDMI I2C scl and I2C sda Function 2*/
		omap_mux_init_signal("hdmi_ddc_scl_mux0",
			TI814X_PULL_UP | TI814X_INPUT_EN);
		omap_mux_init_signal("hdmi_ddc_sda_mux0",
			TI814X_PULL_UP | TI814X_INPUT_EN);
		omap_mux_init_signal("hdmi_cec_mux0",
			TI814X_PULL_UP | TI814X_INPUT_EN);
		omap_mux_init_signal("hdmi_hpd_mux0",
			TI814X_INPUT_EN);
#endif
		/*I2C2 configuration functon 6*/
		omap_mux_init_signal("i2c2_scl_mux0",
			TI814X_PULL_UP | TI814X_INPUT_EN);
		omap_mux_init_signal("i2c2_sda_mux0",
			TI814X_PULL_UP | TI814X_INPUT_EN);

		/*VIN0 configuraiton*/
		omap_mux_init_signal("vin0_clk1",
				TI814X_PULL_DIS | TI814X_INPUT_EN);
		omap_mux_init_signal("vin0_de0_mux0", OMAP_MUX_MODE0);
		/* force pin pad of  vin0_de0_mux0 to 0*/
		omap_writel(0, 0x48140A18);
		omap_mux_init_signal("vin0_fld0_mux0",
				TI814X_PULL_DIS | TI814X_INPUT_EN);
		omap_mux_init_signal("vin0_clk0",
				TI814X_PULL_DIS | TI814X_INPUT_EN);
		omap_mux_init_signal("vin0_hsync0",
				TI814X_PULL_DIS | TI814X_INPUT_EN);
		omap_mux_init_signal("vin0_vsync0",
				TI814X_PULL_DIS | TI814X_INPUT_EN);
		omap_mux_init_signal("vin0_d0",
				TI814X_PULL_DIS | TI814X_INPUT_EN);
		omap_mux_init_signal("vin0_d1",
				TI814X_PULL_DIS | TI814X_INPUT_EN);
		omap_mux_init_signal("vin0_d2",
				TI814X_PULL_DIS | TI814X_INPUT_EN);
		omap_mux_init_signal("vin0_d3",
				TI814X_PULL_DIS | TI814X_INPUT_EN);
		omap_mux_init_signal("vin0_d4",
				TI814X_PULL_DIS | TI814X_INPUT_EN);
		omap_mux_init_signal("vin0_d5",
				TI814X_PULL_DIS | TI814X_INPUT_EN);
		omap_mux_init_signal("vin0_d6",
				TI814X_PULL_DIS | TI814X_INPUT_EN);
		omap_mux_init_signal("vin0_d7",
				TI814X_PULL_DIS | TI814X_INPUT_EN);
		omap_mux_init_signal("vin0_d8",
				TI814X_PULL_DIS | TI814X_INPUT_EN);
		omap_mux_init_signal("vin0_d9",
				TI814X_PULL_DIS | TI814X_INPUT_EN);
		omap_mux_init_signal("vin0_d10",
				TI814X_PULL_DIS | TI814X_INPUT_EN);
		omap_mux_init_signal("vin0_d11",
				TI814X_PULL_DIS | TI814X_INPUT_EN);
		omap_mux_init_signal("vin0_d12",
				TI814X_PULL_DIS | TI814X_INPUT_EN);
		omap_mux_init_signal("vin0_d13",
				TI814X_PULL_DIS | TI814X_INPUT_EN);
		omap_mux_init_signal("vin0_d14",
				TI814X_PULL_DIS | TI814X_INPUT_EN);
		omap_mux_init_signal("vin0_d15",
				TI814X_PULL_DIS | TI814X_INPUT_EN);
		omap_mux_init_signal("vin0_d16",
				TI814X_PULL_DIS | TI814X_INPUT_EN);
		omap_mux_init_signal("vin0_d17",
				TI814X_PULL_DIS | TI814X_INPUT_EN);
		omap_mux_init_signal("vin0_d18",
				TI814X_PULL_DIS | TI814X_INPUT_EN);
		omap_mux_init_signal("vin0_d19",
				TI814X_PULL_DIS | TI814X_INPUT_EN);
		omap_mux_init_signal("vin0_d20",
				TI814X_PULL_DIS | TI814X_INPUT_EN);
		omap_mux_init_signal("vin0_d21",
				TI814X_PULL_DIS | TI814X_INPUT_EN);
		omap_mux_init_signal("vin0_d22",
				TI814X_PULL_DIS | TI814X_INPUT_EN);
		omap_mux_init_signal("vin0_d23",
				TI814X_PULL_DIS | TI814X_INPUT_EN);
		omap_mux_init_signal("vin0_de0_mux1",
				TI814X_PULL_DIS | TI814X_INPUT_EN);
		omap_mux_init_signal("vin0_de1",
				TI814X_PULL_DIS | TI814X_INPUT_EN);
		omap_mux_init_signal("vin0_fld0_mux1",
				TI814X_PULL_DIS | TI814X_INPUT_EN);
		omap_mux_init_signal("vin0_fld1",
				TI814X_PULL_DIS | TI814X_INPUT_EN);

		/*VIN1 Configuration*/
		omap_mux_init_signal("vin1_hsync0",
				TI814X_PULL_DIS | TI814X_INPUT_EN);
		omap_mux_init_signal("vin1_clk1",
				TI814X_PULL_DIS | TI814X_INPUT_EN);
#if 0
		omap_mux_init_signal("vin1_vsync0",
				TI814X_PULL_DIS | TI814X_INPUT_EN);
		omap_mux_init_signal("vin1_fid0",
				TI814X_PULL_DIS | TI814X_INPUT_EN);
		omap_mux_init_signal("vin1_de0",
				TI814X_PULL_DIS | TI814X_INPUT_EN);
#endif
		omap_mux_init_signal("vin1_clk0",
				TI814X_PULL_DIS | TI814X_INPUT_EN);
		omap_mux_init_signal("vin1a_d0",
				TI814X_PULL_DIS | TI814X_INPUT_EN);
		omap_mux_init_signal("vin1a_d1",
				TI814X_PULL_DIS | TI814X_INPUT_EN);
		omap_mux_init_signal("vin1a_d2",
				TI814X_PULL_DIS | TI814X_INPUT_EN);
		omap_mux_init_signal("vin1a_d3",
				TI814X_PULL_DIS | TI814X_INPUT_EN);
		omap_mux_init_signal("vin1a_d4",
				TI814X_PULL_DIS | TI814X_INPUT_EN);
		omap_mux_init_signal("vin1a_d5",
				TI814X_PULL_DIS | TI814X_INPUT_EN);
		omap_mux_init_signal("vin1a_d6",
				TI814X_PULL_DIS | TI814X_INPUT_EN);
		omap_mux_init_signal("vin1a_d8",
				TI814X_PULL_DIS | TI814X_INPUT_EN);
		omap_mux_init_signal("vin1a_d9",
				TI814X_PULL_DIS | TI814X_INPUT_EN);
		omap_mux_init_signal("vin1a_d10",
				TI814X_PULL_DIS | TI814X_INPUT_EN);
		omap_mux_init_signal("vin1a_d11",
				TI814X_PULL_DIS | TI814X_INPUT_EN);
		omap_mux_init_signal("vin1a_d12",
				TI814X_PULL_DIS | TI814X_INPUT_EN);
		omap_mux_init_signal("vin1a_d13",
				TI814X_PULL_DIS | TI814X_INPUT_EN);
		omap_mux_init_signal("vin1a_d14",
				TI814X_PULL_DIS | TI814X_INPUT_EN);
		omap_mux_init_signal("vin1a_d15",
				TI814X_PULL_DIS | TI814X_INPUT_EN);
		omap_mux_init_signal("vin1a_d16",
				TI814X_PULL_DIS | TI814X_INPUT_EN);
		omap_mux_init_signal("vin1a_d17",
				TI814X_PULL_DIS | TI814X_INPUT_EN);
		omap_mux_init_signal("vin1a_d18",
				TI814X_PULL_DIS | TI814X_INPUT_EN);
		omap_mux_init_signal("vin1a_d19",
				TI814X_PULL_DIS | TI814X_INPUT_EN);
		omap_mux_init_signal("vin1a_d20",
				TI814X_PULL_DIS | TI814X_INPUT_EN);
		omap_mux_init_signal("vin1a_d21",
				TI814X_PULL_DIS | TI814X_INPUT_EN);
		omap_mux_init_signal("vin1a_d22",
				TI814X_PULL_DIS | TI814X_INPUT_EN);
		omap_mux_init_signal("vin1a_d23",
				TI814X_PULL_DIS | TI814X_INPUT_EN);
		omap_mux_init_signal("vin1a_d7",
				TI814X_PULL_DIS | TI814X_INPUT_EN);
		/*FIXME move to right place
		    set divider for SYSCLK10*/
		omap_writel(3, 0x48180324);
}
}
#endif

#else
static inline void ti81xx_register_edma(void) {}
#endif

#ifdef CONFIG_MTD_CFI
/* gpio1_22 controls the GPMC higher address lines a12 -25 */
#define GPIO_NOR	54

static void ti814x_nor_init(void)
{
	int error = 0;

	if (cpu_is_ti811x()) { /*TI811X */
		omap_mux_init_signal("gmii0_rxd4.gpmc_a_1_mux0",
					TI814X_PULL_DIS | TI814X_INPUT_EN);
		omap_mux_init_signal("gmii0_rxd5.gpmc_a_2_mux0",
					TI814X_PULL_DIS | TI814X_INPUT_EN);
		omap_mux_init_signal("gmii0_rxd6.gpmc_a_3_mux0",
					TI814X_PULL_DIS | TI814X_INPUT_EN);
		omap_mux_init_signal("gmii0_rxd7.gpmc_a_4_mux0",
					TI814X_PULL_DIS | TI814X_INPUT_EN);
		omap_mux_init_signal("gmii0_rxdv.gpmc_a_5_mux0",
					TI814X_PULL_DIS | TI814X_INPUT_EN);
		omap_mux_init_signal("gmii0_gtxclk.gpmc_a_6_mux0",
					TI814X_PULL_DIS | TI814X_INPUT_EN);
		omap_mux_init_signal("gmii0_txd0.gpmc_a_7_mux0",
					TI814X_PULL_DIS | TI814X_INPUT_EN);
		omap_mux_init_signal("gmii0_txd1.gpmc_a_8_mux0",
					TI814X_PULL_DIS | TI814X_INPUT_EN);
		omap_mux_init_signal("gmii0_txd2.gpmc_a_9_mux0",
					TI814X_PULL_DIS | TI814X_INPUT_EN);
		omap_mux_init_signal("gmii0_txd3.gpmc_a_10_mux0",
					TI814X_PULL_DIS | TI814X_INPUT_EN);
		omap_mux_init_signal("gmii0_txd4.gpmc_a_11_mux0",
					TI814X_PULL_DIS | TI814X_INPUT_EN);
		omap_mux_init_signal("gmii0_txd5.gpmc_a_12_mux0",
					TI814X_PULL_DIS | TI814X_INPUT_EN);
		omap_mux_init_signal("gmii0_txd6.gpmc_a_13_mux0",
					TI814X_PULL_DIS | TI814X_INPUT_EN);
		omap_mux_init_signal("gmii0_txd7.gpmc_a_14_mux0",
					TI814X_PULL_DIS | TI814X_INPUT_EN);
		omap_mux_init_signal("gmii0_txen.gpmc_a_15_mux0",
					TI814X_PULL_DIS | TI814X_INPUT_EN);
	} else { /*TI814X except J5Eco */
		omap_mux_init_signal("gpmc_ad0",
					TI814X_PULL_DIS | TI814X_INPUT_EN);
		omap_mux_init_signal("gpmc_ad1",
					TI814X_PULL_DIS | TI814X_INPUT_EN);
		omap_mux_init_signal("gpmc_ad2",
					TI814X_PULL_DIS | TI814X_INPUT_EN);
		omap_mux_init_signal("gpmc_ad3",
					TI814X_PULL_DIS | TI814X_INPUT_EN);
		omap_mux_init_signal("gpmc_ad4",
					TI814X_PULL_DIS | TI814X_INPUT_EN);
		omap_mux_init_signal("gpmc_ad5",
					TI814X_PULL_DIS | TI814X_INPUT_EN);
		omap_mux_init_signal("gpmc_ad6",
					TI814X_PULL_DIS | TI814X_INPUT_EN);
		omap_mux_init_signal("gpmc_ad7",
					TI814X_PULL_DIS | TI814X_INPUT_EN);
		omap_mux_init_signal("gpmc_ad8",
					TI814X_PULL_DIS | TI814X_INPUT_EN);
		omap_mux_init_signal("gpmc_ad9",
					TI814X_PULL_DIS | TI814X_INPUT_EN);
		omap_mux_init_signal("gpmc_ad10",
					TI814X_PULL_DIS | TI814X_INPUT_EN);
		omap_mux_init_signal("gpmc_ad11",
					TI814X_PULL_DIS | TI814X_INPUT_EN);
		omap_mux_init_signal("gpmc_ad12",
					TI814X_PULL_DIS | TI814X_INPUT_EN);
		omap_mux_init_signal("gpmc_ad13",
					TI814X_PULL_DIS | TI814X_INPUT_EN);
		omap_mux_init_signal("gpmc_ad14",
					TI814X_PULL_DIS | TI814X_INPUT_EN);
		omap_mux_init_signal("gpmc_ad15",
					TI814X_PULL_DIS | TI814X_INPUT_EN);
	}
	omap_mux_init_signal("mmc2_dat3.gpmc_a_1_mux1", TI814X_PULL_UP);
	omap_mux_init_signal("mmc2_dat2.gpmc_a_2_mux1", TI814X_PULL_UP);
	omap_mux_init_signal("mmc2_dat1.gpmc_a_3_mux1", TI814X_PULL_UP);
	omap_mux_init_signal("mmc2_dat0.gpmc_a_4_mux1", TI814X_PULL_UP);
	omap_mux_init_signal("vout1_g_y_yc1.gpmc_a_5_mux1", TI814X_PULL_UP);
	omap_mux_init_signal("vout1_g_y_yc0.gpmc_a_6_mux1", 0);
	omap_mux_init_signal("vout1_r_cr1.gpmc_a_7_mux1", 0);
	omap_mux_init_signal("vout1_r_cr0.gpmc_a_8_mux1", 0);
	omap_mux_init_signal("vout1_b_cb_c1.gpmc_a_9_mux1", 0);
	omap_mux_init_signal("vout1_b_cb_c0.gpmc_a_10_mux1", TI814X_PULL_UP);
	omap_mux_init_signal("vout1_fid.gpmc_a_11_mux1", 0);
	omap_mux_init_signal("vout0_fid_mux1.gpmc_a_12_mux1", 0);
	omap_mux_init_signal("vout1_g_y_yc2.gpmc_a_13_mux1", TI814X_PULL_UP);
	omap_mux_init_signal("vout1_r_cr3.gpmc_a_14_mux1", TI814X_PULL_UP);
	omap_mux_init_signal("vout1_r_cr2.gpmc_a_15_mux1", 0);
	omap_mux_init_signal("gpmc_a16", 0);
	omap_mux_init_signal("gpmc_a17", 0);
	omap_mux_init_signal("gpmc_a18", 0);
	omap_mux_init_signal("gpmc_a19", 0);
	omap_mux_init_signal("gpmc_a_20_mux0", TI814X_PULL_UP);
	omap_mux_init_signal("gpmc_a_21_mux0", 0);
	omap_mux_init_signal("gpmc_a_22_mux0", TI814X_PULL_UP);
	omap_mux_init_signal("gpmc_a_23_mux0", 0);

	if (cpu_is_ti811x()) { /*TI811X */
		omap_mux_init_signal("mmc2_dat7.gpmc_a_24_mux1", TI814X_PULL_UP);
		omap_mux_init_signal("mmc2_dat6.gpmc_a_25_mux1", 0);
	} else {
		omap_mux_init_signal("gpmc_cs2.gpmc_a_24_mux1", TI814X_PULL_UP);
		omap_mux_init_signal("gpmc_cs1.gpmc_a_25_mux1", 0);
	}

	omap_mux_init_signal("mmc2_dat4.gpio1_22", TI814X_PULL_UP);
	omap_mux_init_signal("gpmc_cs0", TI814X_PULL_UP);
	omap_mux_init_signal("gpmc_oen_ren", TI814X_PULL_UP);
	omap_mux_init_signal("gpmc_wen", TI814X_PULL_UP);
	omap_mux_init_signal("gpmc_clk", TI814X_PULL_UP);

	error = gpio_request(GPIO_NOR, "gpio_nor");
	if (error) {
		printk(KERN_ERR "%s: failed to request GPIO for NOR"
			": %d\n", __func__, error);
		return;
	}
	gpio_direction_output(GPIO_NOR, 0x0);
	gpio_export(GPIO_NOR, true);

}
#endif

/*-------------------------------------------------------------------------*/

#ifdef CONFIG_ARCH_TI814X
#define TI814X_CPSW_BASE		(0x4A100000)
#define TI814X_CPSW_MDIO_BASE		(0x4A100800)
#define TI811X_CPSW_MDIO_BASE		(0x4A101000)
#define	TI814X_CPSW_SS_BASE		(0x4A100900)
#define	TI811X_CPSW_SS_BASE		(0x4A101200)
#define TI814X_EMAC_MDIO_FREQ		(1000000)
/* Port Vlan IDs for Dual Mac Mode */
#define CPSW_PORT_VLAN_SLAVE_0		2
#define CPSW_PORT_VLAN_SLAVE_1		3
#define TI81XX_DEFAULT_VLAN		4000

static u64 cpsw_dma_mask = DMA_BIT_MASK(32);
/* TODO : Verify the offsets */
struct cpsw_slave_data cpsw_slaves[] = {
	{
		.slave_reg_ofs  = 0x50,
		.sliver_reg_ofs = 0x700,
		.phy_id		= "0:00",
		.dual_emac_reserved_vlan = CPSW_PORT_VLAN_SLAVE_0,
	},
	{
		.slave_reg_ofs  = 0x90,
		.sliver_reg_ofs = 0x740,
		.phy_id		= "0:01",
		.dual_emac_reserved_vlan = CPSW_PORT_VLAN_SLAVE_1,
	},
};

static struct cpsw_platform_data ti814x_cpsw_pdata = {
	.ss_reg_ofs		= 0x900,
	.channels		= 8,
	.cpdma_reg_ofs		= 0x100,
	.cpdma_sram_ofs         = 0x200,
	.slaves			= 2,
	.slave_data		= cpsw_slaves,
	.ale_reg_ofs		= 0x600,
	.ale_entries		= 1024,
	.host_port_reg_ofs      = 0x28,
	.hw_stats_reg_ofs       = 0x400,
	.cpts_reg_ofs		= 0x500,
	.bd_ram_ofs		= 0x2000,
	.bd_ram_size		= SZ_8K,
	.rx_descs               = 64,
	.mac_control            = BIT(5), /* MIIEN */
	.gigabit_en		= 1,
	.host_port_num		= 0,
	.no_bd_ram		= false,
	.default_vlan		= TI81XX_DEFAULT_VLAN,
};

static struct mdio_platform_data cpsw_mdio_pdata = {
	.bus_freq       = TI814X_EMAC_MDIO_FREQ,
};

static struct resource cpsw_mdio_resources[] = {
	{
		.start  = TI814X_CPSW_MDIO_BASE,
		.end    = TI814X_CPSW_MDIO_BASE + SZ_256 - 1,
		.flags  = IORESOURCE_MEM,
	},
};

static struct platform_device cpsw_mdio_device = {
	.name           = "davinci_mdio",
	.id             = 0,
	.num_resources  = ARRAY_SIZE(cpsw_mdio_resources),
	.resource       = cpsw_mdio_resources,
	.dev.platform_data = &cpsw_mdio_pdata,
};

static struct resource ti814x_cpsw_resources[] = {
	{
		.start  = TI814X_CPSW_BASE,
		.end    = TI814X_CPSW_BASE + SZ_2K - 1,
		.flags  = IORESOURCE_MEM,
	},
	{
		.start  = TI814X_CPSW_SS_BASE,
		.end    = TI814X_CPSW_SS_BASE + SZ_256 - 1,
		.flags  = IORESOURCE_MEM,
	},
	{
		.start	= TI814X_IRQ_GSWRXTHR0,
		.end	= TI814X_IRQ_GSWRXTHR0,
		.flags	= IORESOURCE_IRQ,
	},
	{
		.start	= TI814X_IRQ_GSWRXINT0,
		.end	= TI814X_IRQ_GSWRXINT0,
		.flags	= IORESOURCE_IRQ,
	},
	{
		.start	= TI814X_IRQ_GSWTXINT0,
		.end	= TI814X_IRQ_GSWTXINT0,
		.flags	= IORESOURCE_IRQ,
	},
	{
		.start	= TI814X_IRQ_GSWMISC0,
		.end	= TI814X_IRQ_GSWMISC0,
		.flags	= IORESOURCE_IRQ,
	},
};

static struct platform_device ti814x_cpsw_device = {
	.name		=	"cpsw",
	.id		=	0,
	.num_resources	=	ARRAY_SIZE(ti814x_cpsw_resources),
	.resource	=	ti814x_cpsw_resources,
	.dev		=	{
					.platform_data	   = &ti814x_cpsw_pdata,
					.dma_mask	   = &cpsw_dma_mask,
					.coherent_dma_mask = DMA_BIT_MASK(32),
				},
};

void ti814x_cpsw_mux(void)
{
#if 0 /* No pinmux for now */
	omap_mux_init_signal("gmii1_rxclk", OMAP_MUX_MODE1);
	omap_mux_init_signal("gmii1_rxd0", OMAP_MUX_MODE1);
	omap_mux_init_signal("gmii1_rxd1", OMAP_MUX_MODE1);
	omap_mux_init_signal("gmii1_rxd2", OMAP_MUX_MODE1);
	omap_mux_init_signal("gmii1_rxd3", OMAP_MUX_MODE1);
	omap_mux_init_signal("gmii1_rxd4", OMAP_MUX_MODE1);
	omap_mux_init_signal("gmii1_rxd5", OMAP_MUX_MODE1);
	omap_mux_init_signal("gmii1_rxd6", OMAP_MUX_MODE1);
	omap_mux_init_signal("gmii1_rxd7", OMAP_MUX_MODE1);
	omap_mux_init_signal("gmii1_rxdv", OMAP_MUX_MODE1);
	omap_mux_init_signal("gmii1_gtxclk", OMAP_MUX_MODE1);
	omap_mux_init_signal("gmii1_txd0", OMAP_MUX_MODE1);
	omap_mux_init_signal("gmii1_txd1", OMAP_MUX_MODE1);
	omap_mux_init_signal("gmii1_txd2", OMAP_MUX_MODE1);
	omap_mux_init_signal("gmii1_txd3", OMAP_MUX_MODE1);
	omap_mux_init_signal("gmii1_txd4", OMAP_MUX_MODE1);
	omap_mux_init_signal("gmii1_txd5", OMAP_MUX_MODE1);
	omap_mux_init_signal("gmii1_txd6", OMAP_MUX_MODE1);
	omap_mux_init_signal("gmii1_txd7", OMAP_MUX_MODE1);
	omap_mux_init_signal("gmii1_txen", OMAP_MUX_MODE1);
	omap_mux_init_signal("gmii1_txclk", OMAP_MUX_MODE1);
	omap_mux_init_signal("gmii1_col", OMAP_MUX_MODE1);
	omap_mux_init_signal("gmii1_crs", OMAP_MUX_MODE1);
	omap_mux_init_signal("gmii1_rxer", OMAP_MUX_MODE1);
#endif
}

#define S_INVALID_MAC	"deadbeaf"
/* Get Ethernet address from kernel boot params */
static unsigned char cpmac_eth_string[20] = S_INVALID_MAC;

/* string to hex conversion */
static unsigned char emac_str_to_hexnum(unsigned char c)
{
	if(c >= '0' && c <= '9')
		return c - '0';
	if(c >= 'a' && c <= 'f')
		return c - 'a' + 10;
	if(c >= 'A' && c <= 'F')
		return c - 'A' + 10;
	return 0;
}

/* string to ethernet address conversion */
static void emac_str_to_ethaddr(unsigned char *ea, unsigned char *str)
{
	int i;
	unsigned char num;

	for(i = 0; i < ETH_ALEN; i++) {
		if((*str == '.') || (*str == ':')) {
			str++;
		}
		num = emac_str_to_hexnum(*str) << 4;
		++str;
		num |= (emac_str_to_hexnum(*str));
		++str;
		ea[i] = num;
	}
}

static int cpmac_eth_setup(char *str)
{
	/* The first char passed from the bootloader is '=', so ignore it */
	strcpy(cpmac_eth_string, str + 1);

	printk("cpsw: kernel boot params Ethernet address: %s\n",
	       cpmac_eth_string);

	return 1;
}
__setup("eth", cpmac_eth_setup);

void ti814x_cpsw_init(void)
{
	u32 mac_lo, mac_hi;

	mac_lo = omap_ctrl_readl(TI81XX_CONTROL_MAC_ID0_LO);
	mac_hi = omap_ctrl_readl(TI81XX_CONTROL_MAC_ID0_HI);
	cpsw_slaves[0].mac_addr[0] = mac_hi & 0xFF;
	cpsw_slaves[0].mac_addr[1] = (mac_hi & 0xFF00) >> 8;
	cpsw_slaves[0].mac_addr[2] = (mac_hi & 0xFF0000) >> 16;
	cpsw_slaves[0].mac_addr[3] = (mac_hi & 0xFF000000) >> 24;
	cpsw_slaves[0].mac_addr[4] = mac_lo & 0xFF;
	cpsw_slaves[0].mac_addr[5] = (mac_lo & 0xFF00) >> 8;

	mac_lo = omap_ctrl_readl(TI81XX_CONTROL_MAC_ID1_LO);
	mac_hi = omap_ctrl_readl(TI81XX_CONTROL_MAC_ID1_HI);
	cpsw_slaves[1].mac_addr[0] = mac_hi & 0xFF;
	cpsw_slaves[1].mac_addr[1] = (mac_hi & 0xFF00) >> 8;
	cpsw_slaves[1].mac_addr[2] = (mac_hi & 0xFF0000) >> 16;
	cpsw_slaves[1].mac_addr[3] = (mac_hi & 0xFF000000) >> 24;
	cpsw_slaves[1].mac_addr[4] = mac_lo & 0xFF;
	cpsw_slaves[1].mac_addr[5] = (mac_lo & 0xFF00) >> 8;
	/* Change the mac address according to eth in bootargs
	   if it was assigned. */
	if(strcmp(S_INVALID_MAC, cpmac_eth_string) != 0)
		emac_str_to_ethaddr(ti814x_cpsw_pdata.mac_addr, cpmac_eth_string);
	else
		memcpy(ti814x_cpsw_pdata.mac_addr, cpsw_slaves[0].mac_addr, ETH_ALEN);

#if 0
	ti814x_cpsw_mux();
#endif
	if (!(cpu_is_ti811x() || cpu_is_dm385() ||
			(cpu_is_ti814x() && omap_rev() > TI8148_REV_ES1_0)))
		cpsw_slaves[0].phy_id = "0:01";

	if (cpu_is_ti811x()) {
		cpsw_slaves[0].slave_reg_ofs		= 0x200;
		cpsw_slaves[0].sliver_reg_ofs		= 0xd80;
		cpsw_slaves[1].slave_reg_ofs		= 0x300;
		cpsw_slaves[1].sliver_reg_ofs		= 0xdc0;

		ti814x_cpsw_pdata.ss_reg_ofs		= 0x1200;
		ti814x_cpsw_pdata.cpdma_reg_ofs		= 0x800;
		ti814x_cpsw_pdata.cpdma_sram_ofs	= 0xa00;
		ti814x_cpsw_pdata.ale_reg_ofs		= 0xd00;
		ti814x_cpsw_pdata.host_port_reg_ofs	= 0x108;
		ti814x_cpsw_pdata.hw_stats_reg_ofs	= 0x900;
		ti814x_cpsw_pdata.cpts_reg_ofs		= 0xc00;

		cpsw_mdio_resources[0].start	= TI811X_CPSW_MDIO_BASE;
		cpsw_mdio_resources[0].end	= TI811X_CPSW_MDIO_BASE
						  + SZ_256 - 1;

		ti814x_cpsw_resources[1].start  = TI811X_CPSW_SS_BASE;
		ti814x_cpsw_resources[1].end    = TI811X_CPSW_SS_BASE
						  + SZ_256 - 1;
	}

	platform_device_register(&cpsw_mdio_device);
	platform_device_register(&ti814x_cpsw_device);
	clk_add_alias(NULL, dev_name(&cpsw_mdio_device.dev),
			NULL, &ti814x_cpsw_device.dev);
}
#else
static inline void ti814x_cpsw_init(void) {}
#endif

#ifdef CONFIG_ARCH_TI81XX

#define REFCLK_LDO_INVALID	(0x10 << 16)
#define REFCLK_LDO_REF_VAL	(0x001A0020)

static void ti81xx_ethernet_init(void)
{
	if (cpu_is_ti816x())
		ti816x_ethernet_init();
	else
		ti814x_cpsw_init();
}

static inline void ti814x_sata_pllcfg(void)
{
	if (!cpu_is_ti814x())
		return;

	if (cpu_is_dm385()) {
		/*check for correct VSET value in REFCLK_LJCBLDO_CTRL register*/
		if ((omap_ctrl_readl(TI814X_REFCLK_LJCBLDO_CTRL) &
				(REFCLK_LDO_INVALID)) == REFCLK_LDO_INVALID) {

			omap_ctrl_writel(REFCLK_LDO_REF_VAL,
						TI814X_REFCLK_LJCBLDO_CTRL);
			udelay(2000);
		}
	}

	if ((cpu_is_ti814x()) && (!cpu_is_dm385())) {
		/* Configure 100Mhz clock source on DM814x */

		/* Configure SATA0 PLL -applies for TI814x*/
		omap_ctrl_writel(0x00000004, TI814X_CONTROL_SATA_PLLCFG0);
		udelay(100);
		/* cfgpll1  (for 100 MHz Operation) */
		omap_ctrl_writel(0x812C003C, TI814X_CONTROL_SATA_PLLCFG1);
		udelay(2000);
		omap_ctrl_writel(0x004008E0, TI814X_CONTROL_SATA_PLLCFG3);
		udelay(2000);
		/* wait for bias to be stable */
		omap_ctrl_writel(0x00000014, TI814X_CONTROL_SATA_PLLCFG0);
		udelay(850);
		omap_ctrl_writel(0x00000016, TI814X_CONTROL_SATA_PLLCFG0);
		udelay(60);
		/* cfgpll0 Replaced 0xC00000016 to 0x40000016 for 100MHz
		* Usage instead of 20MHz
		*/
		omap_ctrl_writel(0x40000016, TI814X_CONTROL_SATA_PLLCFG0);
		udelay(2000);

		/* cfgpll0 Replaced 0xC0007077 with 0x40007077 for
		* 100MHz Usage instead of 20MHz
		*/
		omap_ctrl_writel(0x40007077, TI814X_CONTROL_SATA_PLLCFG0);

		while (!(omap_ctrl_readl(TI814X_CONTROL_SATA_PLLSTATUS) & 0x1))
			cpu_relax();
	} else {
		/* Configure 20Mhz clock source on ti813x */
	}

}

static inline void ti814x_pcie_pllcfg(void)
{

	if (cpu_is_ti814x()) {
		/* TODO: Add bitfield macros for following */

		omap_ctrl_writel(0x00000002, TI814X_SERDES_REFCLK_CTL);
		omap_ctrl_writel(0x00000000, TI814X_CONTROL_PCIE_PLLCFG0);
		omap_ctrl_writel(0x00640000, TI814X_CONTROL_PCIE_PLLCFG1);
		omap_ctrl_writel(0x00000000, TI814X_CONTROL_PCIE_PLLCFG2);
		omap_ctrl_writel(0x004008E0, TI814X_CONTROL_PCIE_PLLCFG3);
		omap_ctrl_writel(0x0000609C, TI814X_CONTROL_PCIE_PLLCFG4);

		/* Configure SERDES misc bits - values as is from h/w */
		if (!cpu_is_dm385()) {
			/*
			 * TODO: MISCCFG write is not needed for PG2.x devices,
			 * to be tested with if{} part removed
			 */
			if (omap_rev() > TI8148_REV_ES1_0)
				omap_ctrl_writel(0x0000039E,
						TI814X_CONTROL_PCIE_MISCCFG);
			else
				omap_ctrl_writel(0x00000E7B,
						TI814X_CONTROL_SMA0);
		}

		udelay(50);
		omap_ctrl_writel(0x00000004, TI814X_CONTROL_PCIE_PLLCFG0);

		udelay(50);
		omap_ctrl_writel(0x00000014, TI814X_CONTROL_PCIE_PLLCFG0);

		udelay(50);
		omap_ctrl_writel(0x00000016, TI814X_CONTROL_PCIE_PLLCFG0);

		udelay(50);
		omap_ctrl_writel(0x30000016, TI814X_CONTROL_PCIE_PLLCFG0);

		udelay(50);
		omap_ctrl_writel(0x70007016, TI814X_CONTROL_PCIE_PLLCFG0);

		udelay(200);
		omap_ctrl_writel(0x70007017, TI814X_CONTROL_PCIE_PLLCFG0);

		while (!(omap_ctrl_readl(TI814X_CONTROL_PCIE_PLLSTATUS) & 0x1))
			cpu_relax();

	}

}
#endif

#if defined(CONFIG_SATA_AHCI_PLATFORM) || \
	defined(CONFIG_SATA_AHCI_PLATFORM_MODULE)

static int ti81xx_ahci_plat_init(struct device *dev, void __iomem *base);
static void ti81xx_ahci_plat_exit(struct device *dev);


static struct ahci_platform_data omap_sata0_pdata = {
	.init	= ti81xx_ahci_plat_init,
	.exit	= ti81xx_ahci_plat_exit,
};

static struct ahci_platform_data omap_sata1_pdata = {
	.init	= ti81xx_ahci_plat_init,
	.exit	= ti81xx_ahci_plat_exit,
};

static u64 omap_sata_dmamask = DMA_BIT_MASK(32);

/* SATA PHY control register offsets */
#define SATA_P0PHYCR_REG	0x178
#define SATA_P1PHYCR_REG	0x1F8

#define SATA_PHY_ENPLL(x)	((x) << 0)
#define SATA_PHY_MPY(x)		((x) << 1)
#define SATA_PHY_LB(x)		((x) << 5)
#define SATA_PHY_CLKBYP(x)	((x) << 7)
#define SATA_PHY_RXINVPAIR(x)	((x) << 9)
#define SATA_PHY_LBK(x)		((x) << 10)
#define SATA_PHY_RXLOS(x)	((x) << 12)
#define SATA_PHY_RXCDR(x)	((x) << 13)
#define SATA_PHY_RXEQ(x)	((x) << 16)
#define SATA_PHY_RXENOC(x)	((x) << 20)
#define SATA_PHY_TXINVPAIR(x)	((x) << 21)
#define SATA_PHY_TXCM(x)	((x) << 22)
#define SATA_PHY_TXSWING(x)	((x) << 23)
#define SATA_PHY_TXDE(x)	((x) << 27)

#define TI81XX_SATA_BASE	0x4A140000
#define DM385_SATA1_BASE	0x4A0AE000

/* These values are tried and tested and not expected to change.
 * Hence not using a macro to generate them.
 */
#define TI814X_SATA_PHY_CFGRX0_VAL	0x008FCC22
#define TI814X_SATA_PHY_CFGRX1_VAL	0x008E0500
#define TI814X_SATA_PHY_CFGRX2_VAL	0x7BDEF000
#define TI814X_SATA_PHY_CFGRX3_VAL	0x1F180B0F
#define TI814X_SATA_PHY_CFGTX0_VAL	0x01003622
#define TI814X_SATA_PHY_CFGTX1_VAL	0x40000002
#define TI814X_SATA_PHY_CFGTX2_VAL	0x00C201F8
#define TI814X_SATA_PHY_CFGTX3_VAL	0x073CE39E

#define TI813X_SATA_PHY_CFGRX0_VAL	0x00C7CC22
#define TI813X_SATA_PHY_CFGRX1_VAL	0x008E0500
#define TI813X_SATA_PHY_CFGRX2_VAL	0x7BDEF000
#define TI813X_SATA_PHY_CFGRX3_VAL	0x1F180B0F
#define TI813X_SATA_PHY_CFGTX0_VAL	0x01001622
#define TI813X_SATA_PHY_CFGTX1_VAL	0x40000002
#define TI813X_SATA_PHY_CFGTX2_VAL	0x00000000
#define TI813X_SATA_PHY_CFGTX3_VAL	0x073CE39E

static int ti81xx_ahci_plat_init(struct device *dev, void __iomem *base)
{
	unsigned int phy_val;
	int ret;
	struct clk *sata_clk;

	sata_clk = clk_get(dev, NULL);
	if (IS_ERR(sata_clk)) {
		pr_err("ahci : Failed to get SATA clock\n");
		return PTR_ERR(sata_clk);
	}

	if (!base) {
		pr_err("ahci : SATA reg space not mapped, PHY enable failed\n");
		ret = -ENOMEM;
		goto err;
	}

	ret = clk_enable(sata_clk);
	if (ret) {
		pr_err("ahci : Clock enable failed\n");
		goto err;
	}

	if (cpu_is_ti816x()) {
		phy_val = SATA_PHY_ENPLL(1) |
			SATA_PHY_MPY(8) |
			SATA_PHY_LB(0) |
			SATA_PHY_CLKBYP(0) |
			SATA_PHY_RXINVPAIR(0) |
			SATA_PHY_LBK(0) |
			SATA_PHY_RXLOS(1) |
			SATA_PHY_RXCDR(4) |
			SATA_PHY_RXEQ(1) |
			SATA_PHY_RXENOC(1) |
			SATA_PHY_TXINVPAIR(0) |
			SATA_PHY_TXCM(0) |
			SATA_PHY_TXSWING(7) |
			SATA_PHY_TXDE(0);

		writel(phy_val, base + SATA_P0PHYCR_REG);
		writel(phy_val, base + SATA_P1PHYCR_REG);
	} else if (cpu_is_ti814x()) {

		if (cpu_is_dm385()) {
			/* Configuring for 20Mhz clock source on TI813x */
			writel(TI813X_SATA_PHY_CFGRX0_VAL,
				base + TI814X_SATA_PHY_CFGRX0_OFFSET);
			writel(TI813X_SATA_PHY_CFGRX1_VAL,
				base + TI814X_SATA_PHY_CFGRX1_OFFSET);
			writel(TI813X_SATA_PHY_CFGRX2_VAL,
				base + TI814X_SATA_PHY_CFGRX2_OFFSET);
			writel(TI813X_SATA_PHY_CFGRX3_VAL,
				base + TI814X_SATA_PHY_CFGRX3_OFFSET);
			writel(TI813X_SATA_PHY_CFGTX0_VAL,
				base + TI814X_SATA_PHY_CFGTX0_OFFSET);
			writel(TI813X_SATA_PHY_CFGTX1_VAL,
				base + TI814X_SATA_PHY_CFGTX1_OFFSET);
			writel(TI813X_SATA_PHY_CFGTX2_VAL,
				base + TI814X_SATA_PHY_CFGTX2_OFFSET);
			writel(TI813X_SATA_PHY_CFGTX3_VAL,
				base + TI814X_SATA_PHY_CFGTX3_OFFSET);

		} else {
			/* Configuring for 100Mhz clock source on TI814x */
			writel(TI814X_SATA_PHY_CFGRX0_VAL,
				base + TI814X_SATA_PHY_CFGRX0_OFFSET);
			writel(TI814X_SATA_PHY_CFGRX1_VAL,
				base + TI814X_SATA_PHY_CFGRX1_OFFSET);
			writel(TI814X_SATA_PHY_CFGRX2_VAL,
				base + TI814X_SATA_PHY_CFGRX2_OFFSET);
			writel(TI814X_SATA_PHY_CFGRX3_VAL,
				base + TI814X_SATA_PHY_CFGRX3_OFFSET);
			writel(TI814X_SATA_PHY_CFGTX0_VAL,
				base + TI814X_SATA_PHY_CFGTX0_OFFSET);
			writel(TI814X_SATA_PHY_CFGTX1_VAL,
				base + TI814X_SATA_PHY_CFGTX1_OFFSET);
			writel(TI814X_SATA_PHY_CFGTX2_VAL,
				base + TI814X_SATA_PHY_CFGTX2_OFFSET);
			writel(TI814X_SATA_PHY_CFGTX3_VAL,
				base + TI814X_SATA_PHY_CFGTX3_OFFSET);
		}
	}

	return 0;
err:
	clk_put(sata_clk);
	return ret;
}

static void ti81xx_ahci_plat_exit(struct device *dev)
{
	struct clk *sata_clk;

	sata_clk = clk_get(dev, NULL);
	if (IS_ERR(sata_clk)) {
		pr_err("ahci : Failed to get SATA clock\n");
		return;
	}

	clk_disable(sata_clk);
	clk_put(sata_clk);
}

/* resources will be filled by soc specific init routine */
static struct resource omap_ahci0_resources[] = {
	{
		.start	= TI81XX_SATA_BASE,
		.end	= TI81XX_SATA_BASE + 0x10fff,
		.flags	= IORESOURCE_MEM,
	},
	{
		.start	= TI81XX_IRQ_SATA,
		.flags	= IORESOURCE_IRQ,
	}
};

static struct resource omap_ahci1_resources[] = {
	{
		.start	= DM385_SATA1_BASE,
		.end	= DM385_SATA1_BASE + 0x10fff,
		.flags	= IORESOURCE_MEM,
	},
	{
		.start	= DM385_IRQ_SATA1,
		.flags	= IORESOURCE_IRQ,
	}
};

static struct platform_device omap_ahci0_device = {
	.name	= "ahci",
	.id	= 0,
	.dev	= {
		.platform_data = &omap_sata0_pdata,
		.coherent_dma_mask	= DMA_BIT_MASK(32),
		.dma_mask		= &omap_sata_dmamask,
	},
	.num_resources	= ARRAY_SIZE(omap_ahci0_resources),
	.resource	= omap_ahci0_resources,
};

static struct platform_device omap_ahci1_device = {
	.name	= "ahci",
	.id	= 1,
	.dev	= {
		.platform_data = &omap_sata1_pdata,
		.coherent_dma_mask	= DMA_BIT_MASK(32),
		.dma_mask		= &omap_sata_dmamask,
	},
	.num_resources	= ARRAY_SIZE(omap_ahci1_resources),
	.resource	= omap_ahci1_resources,
};


static inline void omap_init_ahci(void)
{
	if ((cpu_is_ti81xx()) && (!cpu_is_ti811x())) {
		platform_device_register(&omap_ahci0_device);
		if (cpu_is_dm385())
			platform_device_register(&omap_ahci1_device);
	}
}
#else
static inline void omap_init_ahci(void) {}
#endif

#if defined(CONFIG_ARCH_TI81XX)
static struct resource dm385_mcasp_resource[] = {
	{
		.name = "mcasp",
		.start = TI81XX_ASP1_BASE,
		.end = TI81XX_ASP1_BASE + (SZ_1K * 12) - 1,
		.flags = IORESOURCE_MEM,
	},
	/* TX event */
	{
		.start = TI81XX_DMA_MCASP1_AXEVT,
		.end = TI81XX_DMA_MCASP1_AXEVT,
		.flags = IORESOURCE_DMA,
	},
	/* RX event */
	{
		.start = TI81XX_DMA_MCASP1_AREVT,
		.end = TI81XX_DMA_MCASP1_AREVT,
		.flags = IORESOURCE_DMA,
	},
};

static struct resource ti81xx_mcasp_resource[] = {
	{
		.name = "mcasp",
		.start = TI81XX_ASP2_BASE,
		.end = TI81XX_ASP2_BASE + (SZ_1K * 12) - 1,
		.flags = IORESOURCE_MEM,
	},
	/* TX event */
	{
		.start = TI81XX_DMA_MCASP2_AXEVT,
		.end = TI81XX_DMA_MCASP2_AXEVT,
		.flags = IORESOURCE_DMA,
	},
	/* RX event */
	{
		.start = TI81XX_DMA_MCASP2_AREVT,
		.end = TI81XX_DMA_MCASP2_AREVT,
		.flags = IORESOURCE_DMA,
	},
};

static struct platform_device ti81xx_mcasp_device = {
	.name = "davinci-mcasp",
};

void __init ti81xx_register_mcasp(int id, struct snd_platform_data *pdata)
{
	//if (machine_is_ti8168evm() || machine_is_ti8148evm() || machine_is_ti811xevm() || machine_is_ti8148ipnc()) {
	if (machine_is_ti8168evm() || machine_is_ti811xevm()) {
		ti81xx_mcasp_device.id = 2;
		ti81xx_mcasp_device.resource = ti81xx_mcasp_resource;
		ti81xx_mcasp_device.num_resources = ARRAY_SIZE(ti81xx_mcasp_resource);
	//} else if (machine_is_dm385evm() || machine_is_dm385ipnc()) {
	} else if (machine_is_dm385evm() || machine_is_dm385ipnc() || machine_is_ti8148evm() || machine_is_ti8148ipnc() ) {
		ti81xx_mcasp_device.id = 1;
		ti81xx_mcasp_device.resource = dm385_mcasp_resource;
		ti81xx_mcasp_device.num_resources = ARRAY_SIZE(dm385_mcasp_resource);
	} else {
		pr_err("%s: platform not supported\n", __func__);
		return;
	}

	/* TI811x AIC_MCLK is connected to SoC pin MCA2_AHCLKX */
	//omap_mux_init_signal("xref_clk2.mcasp2_ahclkx", 0);
	ti81xx_mcasp_device.dev.platform_data = pdata;
	platform_device_register(&ti81xx_mcasp_device);
}
#endif

#if defined(CONFIG_ARCH_TI81XX) && defined(CONFIG_PCI)
static struct ti81xx_pcie_data ti81xx_pcie_data = {
	.msi_irq_base	= MSI_IRQ_BASE,
	.msi_irq_num	= MSI_NR_IRQS,
};

static struct resource ti81xx_pcie_resources[] = {
	{
		/* Register space */
		.name		= "pcie-regs",
		.start		= TI816X_PCIE_REG_BASE,
		.end		= TI816X_PCIE_REG_BASE + SZ_16K - 1,
		.flags		= IORESOURCE_MEM,
	},
	{
		/* Non-prefetch memory */
		.name		= "pcie-nonprefetch",
		.start		= TI816X_PCIE_MEM_BASE,
		.end		= TI816X_PCIE_MEM_BASE + SZ_256M - 1,
		.flags		= IORESOURCE_MEM,
	},
	{
		/* IO window */
		.name		= "pcie-io",
		.start		= TI816X_PCIE_IO_BASE,
		.end		= TI816X_PCIE_IO_BASE + SZ_2M + SZ_1M - 1,
		.flags		= IORESOURCE_IO,
	},
	{
		/* Inbound memory window */
		.name		= "pcie-inbound0",
		.start		= PLAT_PHYS_OFFSET,
		.end		= PLAT_PHYS_OFFSET + SZ_2G - 1,
		.flags		= IORESOURCE_MEM,
	},
	{
		/* Legacy Interrupt */
		.name		= "legacy_int",
		.start		= TI81XX_IRQ_PCIINT0,
		.end		= TI81XX_IRQ_PCIINT0,
		.flags		= IORESOURCE_IRQ,
	},
#ifdef CONFIG_PCI_MSI
	{
		/* MSI Interrupt Line */
		.name		= "msi_int",
		.start		= TI81XX_IRQ_PCIINT1,
		.end		= TI81XX_IRQ_PCIINT1,
		.flags		= IORESOURCE_IRQ,
	},
#endif
};

static struct platform_device ti81xx_pcie_device = {
	.name		= "ti81xx_pcie",
	.id		= 0,
	.dev		= {
		.platform_data = &ti81xx_pcie_data,
	},
	.num_resources	= ARRAY_SIZE(ti81xx_pcie_resources),
	.resource	= ti81xx_pcie_resources,
};

static inline void ti81xx_init_pcie(void)
{
	if (!cpu_is_ti81xx())
		return;

	if (cpu_is_ti816x()) {
		omap_ctrl_writel(TI816X_PCIE_PLLMUX_25X |
				TI81XX_PCIE_DEVTYPE_RC,
				TI816X_CONTROL_PCIE_CFG);

		/* MSI clearing is "write 0 to clear" */
		ti81xx_pcie_data.msi_inv = 1;

		ti81xx_pcie_data.device_id = 0xb800;
	} else if (cpu_is_ti814x()) {

		/* TODO: Add bitfield macros for following */

		omap_ctrl_writel(0x00000002, TI814X_SERDES_REFCLK_CTL);
		omap_ctrl_writel(0x00000000, TI814X_CONTROL_PCIE_PLLCFG0);
		omap_ctrl_writel(0x00640000, TI814X_CONTROL_PCIE_PLLCFG1);
		omap_ctrl_writel(0x00000000, TI814X_CONTROL_PCIE_PLLCFG2);
		omap_ctrl_writel(0x004008E0, TI814X_CONTROL_PCIE_PLLCFG3);
		omap_ctrl_writel(0x0000609C, TI814X_CONTROL_PCIE_PLLCFG4);

		/* Configure SERDES misc bits - values as is from h/w */
		if (omap_rev() > TI8148_REV_ES1_0)
			omap_ctrl_writel(0x0000039E,
					TI814X_CONTROL_PCIE_MISCCFG);
		else
			omap_ctrl_writel(0x00000E7B, TI814X_CONTROL_SMA0);

		udelay(50);
		omap_ctrl_writel(0x00000004, TI814X_CONTROL_PCIE_PLLCFG0);

		udelay(50);
		omap_ctrl_writel(0x00000014, TI814X_CONTROL_PCIE_PLLCFG0);

		udelay(50);
		omap_ctrl_writel(0x00000016, TI814X_CONTROL_PCIE_PLLCFG0);

		udelay(50);
		omap_ctrl_writel(0x30000016, TI814X_CONTROL_PCIE_PLLCFG0);

		udelay(50);
		omap_ctrl_writel(0x70007016, TI814X_CONTROL_PCIE_PLLCFG0);

		udelay(200);
		omap_ctrl_writel(0x70007017, TI814X_CONTROL_PCIE_PLLCFG0);

		while (!(omap_ctrl_readl(TI814X_CONTROL_PCIE_PLLSTATUS) & 0x1))
			cpu_relax();

		omap_ctrl_writel(TI81XX_PCIE_DEVTYPE_RC,
				TI814X_CONTROL_PCIE_CFG);

		/*
		 * Force x1 lane as TI814X only supports x1 while the PCIe
		 * registers read x2 leading to wrong capability printed form
		 * PCIe configuration.
		 */
		ti81xx_pcie_data.force_x1 = 1;

		if (cpu_is_dm385())
			ti81xx_pcie_data.device_id = 0xb802;
		else
			ti81xx_pcie_data.device_id = 0xb801;
	}

	platform_device_register(&ti81xx_pcie_device);
}
#else
static inline void ti81xx_init_pcie(void) {}
#endif

#ifdef CONFIG_ARCH_TI814X
static inline void ti814x_enable_i2c2(void)
{
	struct clk *fclk;

	fclk = clk_get(NULL, "i2c3_fck");
	if (!IS_ERR(fclk))
		clk_enable(fclk);
	else
		printk(KERN_WARNING "clk get on i2c3 fck failed\n");
}
#endif

#ifdef CONFIG_ARCH_TI81XX
static struct resource ti81xx_rtc_resources[] = {
	{
		.start	= TI81XX_RTC_BASE,
		.end	= TI81XX_RTC_BASE + SZ_4K - 1,
		.flags	= IORESOURCE_MEM,
	},
	{ /* timer irq */
		.start	= TI81XX_IRQ_RTC,
		.end	= TI81XX_IRQ_RTC,
		.flags	= IORESOURCE_IRQ,
	},
	{ /* alarm irq */
		.start	= TI81XX_IRQ_RTC_ALARM,
		.end	= TI81XX_IRQ_RTC_ALARM,
		.flags	= IORESOURCE_IRQ,
	},
};

static struct platform_device ti81xx_rtc_device = {
	.name		= "omap_rtc",
	.id		= -1,
	.num_resources	= ARRAY_SIZE(ti81xx_rtc_resources),
	.resource	= ti81xx_rtc_resources,
};

#define KICK0_REG	0x6c
#define KICK1_REG	0x70
#define OSC_REG		0x54

#define KICK0_REG_VAL	0x83e70b13
#define KICK1_REG_VAL	0x95a4f1e0
#define RESET_VAL	BIT(5)

static int ti81xx_rtc_init(void)
{
	void __iomem *base;
	struct clk *clk;

	clk = clk_get(NULL, "rtc_c32k_fck");
	if (!clk) {
		pr_err("rtc : Failed to get RTC clock\n");
		return -1;
	}

	if (clk_enable(clk)) {
		pr_err("rtc: Clock Enable Failed\n");
		return -1;
	}

	base = ioremap(TI81XX_RTC_BASE, SZ_4K);

	if (WARN_ON(!base))
		return -ENOMEM;

	/* Unlock the rtc's registers */
	__raw_writel(KICK0_REG_VAL, base + KICK0_REG);
	__raw_writel(KICK1_REG_VAL, base + KICK1_REG);

	if (cpu_is_ti811x()) {
		/*
		 * Enable the 32K OSc
		 * TODO: Need a better way to handle this
		 * Since we want the clock to be running before mmc init
		 * we need to do it before the rtc probe happens
		 */
		__raw_writel(0x48, base + 0x54);
	} else {
		/* Reset the RTC */
		__raw_writel(RESET_VAL, base + OSC_REG);

		/*
		 * After setting the SW_RESET bit, RTC registers must
		 * not be accessed for 3 32kHz clock cycles
		 * (roughly 2200 OCP cycles).
		 */
		udelay(100);

		/*
		 * Unlock the rtc's registers again as the registers
		 * would have been locked due to reset
		 */
		__raw_writel(KICK0_REG_VAL, base + KICK0_REG);
		__raw_writel(KICK1_REG_VAL, base + KICK1_REG);
	}
	iounmap(base);

	return  platform_device_register(&ti81xx_rtc_device);
}
static void __init clkout2_enable(void)
{
	struct clk *ck_32;

	ck_32 = clk_get(NULL, "osc_32k_ck");
	if (IS_ERR(ck_32)) {
		pr_err("Cannot clk_get ck_32\n");
		return;
	}
	clk_enable(ck_32);
}
#endif

static int __init omap2_init_devices(void)
{
	/*
	 * please keep these calls, and their implementations above,
	 * in alphabetical order so they're easier to sort through.
	 */
	omap_hsmmc_reset();
#if !defined(CONFIG_ARCH_TI81XX)
	omap_init_audio();
#endif
	omap_init_camera();
	omap_init_mbox();
	omap_init_mcspi();
	omap_init_elm();
	omap_init_pmu();
	omap_hdq_init();
	omap_init_sti();
	omap_init_sham();
	omap_init_aes();
	omap_init_vout();
#ifdef CONFIG_ARCH_TI81XX
	if (cpu_is_ti814x()) {
		/* Init PCIe,SATA PLL here, before invoking respective init*/
#if !defined(CONFIG_MACH_DM385IPNC) && !defined(CONFIG_MACH_TI8148IPNC)
		ti814x_pcie_pllcfg();
		if(!cpu_is_ti811x())
			ti814x_sata_pllcfg();
#endif
	}

	ti81xx_ethernet_init();
#if !defined(CONFIG_MACH_DM385IPNC) && !defined(CONFIG_MACH_TI8148IPNC)
	ti81xx_init_pcie();
#endif
	ti81xx_register_edma();
	ti81xx_init_pcm();
	ti816x_sr_init();
#if !defined(CONFIG_MACH_DM385IPNC) && !defined(CONFIG_MACH_TI8148IPNC)
	ti81xx_video_mux();
#endif
#ifdef CONFIG_ARCH_TI814X
	ti814x_enable_i2c2();
	ti814x_d_can_init(0);
#endif
#if !defined(CONFIG_MACH_DM385IPNC) && !defined(CONFIG_MACH_TI8148IPNC)
#ifdef CONFIG_MTD_CFI
	ti814x_nor_init();
#endif
#endif
#endif
	// Penlin comment for debug 
	//omap_init_ahci();
#ifdef CONFIG_ARCH_TI81XX
	ti81xx_rtc_init();
	clkout2_enable();
#endif
	return 0;
}
arch_initcall(omap2_init_devices);

#if defined(CONFIG_OMAP_WATCHDOG) || defined(CONFIG_OMAP_WATCHDOG_MODULE)
struct omap_device_pm_latency omap_wdt_latency[] = {
	[0] = {
		.deactivate_func = omap_device_idle_hwmods,
		.activate_func   = omap_device_enable_hwmods,
		.flags		 = OMAP_DEVICE_LATENCY_AUTO_ADJUST,
	},
};

static int __init omap_init_wdt(void)
{
	int id = -1;
	struct omap_device *od;
	struct omap_hwmod *oh;
	char *oh_name;
	char *dev_name = "omap_wdt";

	if (cpu_is_ti814x())
		oh_name = "wd_timer1";
	else
		oh_name = "wd_timer2";

	if (!cpu_class_is_omap2())
		return 0;

	oh = omap_hwmod_lookup(oh_name);
	if (!oh) {
		pr_err("Could not look up wd_timer%d hwmod\n", id);
		return -EINVAL;
	}

	od = omap_device_build(dev_name, id, oh, NULL, 0,
				omap_wdt_latency,
				ARRAY_SIZE(omap_wdt_latency), 0);
	WARN(IS_ERR(od), "Cant build omap_device for %s:%s.\n",
				dev_name, oh->name);
	return 0;
}
subsys_initcall(omap_init_wdt);
#endif

#ifdef CONFIG_HAVE_PWM

#define PWM_PLATFROM_NAME "omap-pwm"

struct platform_device **omap_pwm_devices;

int __init omap_register_pwm_config(struct omap2_pwm_platform_config *config, int size)
{
	int ret = 0, i;
	if(!config || size < 0)
		return -EINVAL;
	omap_pwm_devices = (struct platform_device**) kzalloc(sizeof(struct platform_device*) * size, GFP_KERNEL);
	if(omap_pwm_devices == NULL)
		return -ENOMEM;
	for(i = 0; i < size; i++) {
		omap_pwm_devices[i] = platform_device_alloc(PWM_PLATFROM_NAME, i);
		if(!omap_pwm_devices[i]) {
			while(i-- > 0) {
				platform_device_unregister(omap_pwm_devices[i]);
				omap_pwm_devices[i] = NULL;
			}
			kfree(omap_pwm_devices);
			omap_pwm_devices = NULL;
			ret = -ENOMEM;
			break;
		}
		omap_pwm_devices[i]->dev.platform_data = &config[i];
		ret = platform_device_add(omap_pwm_devices[i]);
		if(ret) {
			platform_device_put(omap_pwm_devices[i]);
			omap_pwm_devices[i] = NULL;
			while(i-- > 0) {
				platform_device_unregister(omap_pwm_devices[i]);
				omap_pwm_devices[i] = NULL;
			}
			kfree(omap_pwm_devices);
			omap_pwm_devices = NULL;
			break;
		}
	}
	return ret;
}
EXPORT_SYMBOL_GPL(omap_register_pwm_config);
#endif

