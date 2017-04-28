/*
 * arch/arm/plat-ambarella/generic/spi.c
 *
 * Author: Anthony Ginger <hfjiang@ambarella.com>
 *
 * Copyright (C) 2004-2010, Ambarella, Inc.
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA 02111-1307 USA
 *
 */

#include <linux/kernel.h>
#include <linux/init.h>
#include <linux/platform_device.h>
#include <linux/dma-mapping.h>
#include <linux/moduleparam.h>
#include <linux/clk.h>

#include <mach/hardware.h>
#include <plat/spi.h>
#include <plat/clk.h>

/* ==========================================================================*/
#ifdef MODULE_PARAM_PREFIX
#undef MODULE_PARAM_PREFIX
#endif
#define MODULE_PARAM_PREFIX	"ambarella_config."

/* ==========================================================================*/
#if defined(CONFIG_PLAT_AMBARELLA_SUPPORT_HAL)
#else
static struct clk gclk_ssi = {
	.parent		= NULL,
	.name		= "gclk_ssi",
	.rate		= 0,
	.frac_mode	= 0,
	.ctrl_reg	= PLL_REG_UNAVAILABLE,
	.pres_reg	= PLL_REG_UNAVAILABLE,
	.post_reg	= CG_SSI_REG,
	.frac_reg	= PLL_REG_UNAVAILABLE,
	.ctrl2_reg	= PLL_REG_UNAVAILABLE,
	.ctrl3_reg	= PLL_REG_UNAVAILABLE,
	.lock_reg	= PLL_REG_UNAVAILABLE,
	.lock_bit	= 0,
	.divider	= 0,
	.max_divider	= (1 << 24) - 1,
	.extra_scaler	= 0,
	.ops		= &ambarella_rct_scaler_ops,
};

static struct clk *ambarella_ssi_register_clk(void)
{
	struct clk *pgclk_ssi = NULL;
	struct clk *pgclk_apb = NULL;

	pgclk_ssi = clk_get(NULL, "gclk_ssi");
	if (IS_ERR(pgclk_ssi)) {
		pgclk_apb = clk_get(NULL, "gclk_apb");
		if (IS_ERR(pgclk_apb)) {
			BUG();
		}
		gclk_ssi.parent = pgclk_apb;
		ambarella_register_clk(&gclk_ssi);
		pgclk_ssi = &gclk_ssi;
		pr_info("SYSCLK:PWM[%lu]\n", clk_get_rate(pgclk_ssi));
	}

	return pgclk_ssi;
}
#endif

static void ambarella_ssi_set_pll(void)
{
#if (CHIP_REV == I1) || (CHIP_REV == A8)
	u32 freq_hz = 54000000;
#elif (CHIP_REV == AMB_S2)
	u32 freq_hz = 60000000;
#else
	u32 freq_hz = 13500000;
#endif
#if defined(CONFIG_PLAT_AMBARELLA_SUPPORT_HAL)
	amb_set_ssi_clock_frequency(HAL_BASE_VP, freq_hz);
#else
	clk_set_rate(ambarella_ssi_register_clk(), freq_hz);
#endif
}

static u32 ambarella_ssi_get_pll(void)
{
#if defined(CONFIG_PLAT_AMBARELLA_SUPPORT_HAL)
	return (u32)amb_get_ssi_clock_frequency(HAL_BASE_VP);
#else
	return clk_get_rate(ambarella_ssi_register_clk());
#endif
}

#if (SPI_MASTER_INSTANCES >= 2)
#if defined(CONFIG_PLAT_AMBARELLA_SUPPORT_HAL)
#else
static struct clk gclk_ssi2 = {
	.parent		= NULL,
	.name		= "gclk_ssi2",
	.rate		= 0,
	.frac_mode	= 0,
	.ctrl_reg	= PLL_REG_UNAVAILABLE,
	.pres_reg	= PLL_REG_UNAVAILABLE,
	.post_reg	= CG_SSI2_REG,
	.frac_reg	= PLL_REG_UNAVAILABLE,
	.ctrl2_reg	= PLL_REG_UNAVAILABLE,
	.ctrl3_reg	= PLL_REG_UNAVAILABLE,
	.lock_reg	= PLL_REG_UNAVAILABLE,
	.lock_bit	= 0,
	.divider	= 0,
	.max_divider	= (1 << 24) - 1,
	.extra_scaler	= 0,
	.ops		= &ambarella_rct_scaler_ops,
};

static struct clk *ambarella_ssi2_register_clk(void)
{
	struct clk *pgclk_ssi2 = NULL;
	struct clk *pgclk_apb = NULL;

	pgclk_ssi2 = clk_get(NULL, "gclk_ssi2");
	if (IS_ERR(pgclk_ssi2)) {
		pgclk_apb = clk_get(NULL, "gclk_apb");
		if (IS_ERR(pgclk_apb)) {
			BUG();
		}
		gclk_ssi2.parent = pgclk_apb;
		ambarella_register_clk(&gclk_ssi2);
		pgclk_ssi2 = &gclk_ssi2;
		pr_info("SYSCLK:PWM[%lu]\n", clk_get_rate(pgclk_ssi2));
	}

	return pgclk_ssi2;
}
#endif

static void ambarella_ssi2_set_pll(void)
{
#if (CHIP_REV == I1) || (CHIP_REV == A8)
	u32 freq_hz = 54000000;
#elif (CHIP_REV == AMB_S2)
	u32 freq_hz = 60000000;
#else
	u32 freq_hz = 13500000;
#endif
#if defined(CONFIG_PLAT_AMBARELLA_SUPPORT_HAL)
	amb_set_ssi2_clock_frequency(HAL_BASE_VP, freq_hz);
#else
	clk_set_rate(ambarella_ssi2_register_clk(), freq_hz);
#endif
}

static u32 ambarella_ssi2_get_pll(void)
{
#if defined(CONFIG_PLAT_AMBARELLA_SUPPORT_HAL)
	return (u32)amb_get_ssi2_clock_frequency(HAL_BASE_VP);
#else
	return clk_get_rate(ambarella_ssi2_register_clk());
#endif
}
#endif

#if (SPI_AHB_INSTANCES >= 1)
#if defined(CONFIG_PLAT_AMBARELLA_SUPPORT_HAL)
#else
static struct clk gclk_ssi_ahb = {
	.parent		= NULL,
	.name		= "gclk_ssi_ahb",
	.rate		= 0,
	.frac_mode	= 0,
	.ctrl_reg	= PLL_REG_UNAVAILABLE,
	.pres_reg	= PLL_REG_UNAVAILABLE,
	.post_reg	= CG_SSI_AHB_REG,
	.frac_reg	= PLL_REG_UNAVAILABLE,
	.ctrl2_reg	= PLL_REG_UNAVAILABLE,
	.ctrl3_reg	= PLL_REG_UNAVAILABLE,
	.lock_reg	= PLL_REG_UNAVAILABLE,
	.lock_bit	= 0,
	.divider	= 0,
	.max_divider	= (1 << 24) - 1,
	.extra_scaler	= 0,
	.ops		= &ambarella_rct_scaler_ops,
};

static struct clk *ambarella_ssi_ahb_register_clk(void)
{
	struct clk *pgclk_ssi_ahb = NULL;
	struct clk *pgclk_apb = NULL;

	pgclk_ssi_ahb = clk_get(NULL, "gclk_ssi_ahb");
	if (IS_ERR(pgclk_ssi_ahb)) {
		pgclk_apb = clk_get(NULL, "gclk_apb");
		if (IS_ERR(pgclk_apb)) {
			BUG();
		}
		gclk_ssi_ahb.parent = pgclk_apb;
		ambarella_register_clk(&gclk_ssi_ahb);
		pgclk_ssi_ahb = &gclk_ssi_ahb;
		pr_info("SYSCLK:PWM[%lu]\n", clk_get_rate(pgclk_ssi_ahb));
	}

	return pgclk_ssi_ahb;
}
#endif

static void ambarella_ssi_ahb_set_pll(void)
{
#if defined(CONFIG_PLAT_AMBARELLA_SUPPORT_HAL)
	//amb_set_ssi_ahb_clock_frequency(HAL_BASE_VP, freq_hz);
#else
	u32 freq_hz = 54000000;
	clk_set_rate(ambarella_ssi_ahb_register_clk(), freq_hz);
#endif
}

static u32 ambarella_ssi_ahb_get_pll(void)
{
#if defined(CONFIG_PLAT_AMBARELLA_SUPPORT_HAL)
	//return (u32)amb_get_ssi_ahb_clock_frequency(HAL_BASE_VP);
	return 0;
#else
	return clk_get_rate(ambarella_ssi_ahb_register_clk());
#endif
}
#endif

/* ==========================================================================*/
void ambarella_spi_cs_activate(struct ambarella_spi_cs_config *cs_config)
{
	u8			cs_pin, cs_high;

	if (cs_config->bus_id >= SPI_MASTER_INSTANCES ||
		cs_config->cs_id >= cs_config->cs_num)
		return;

	cs_pin	= cs_config->cs_pins[cs_config->cs_id];
	cs_high	= cs_config->cs_high[cs_config->cs_id];

	if (!cs_high) {
		ambarella_gpio_set(cs_pin, 0);
	} else {
		ambarella_gpio_set(cs_pin, 1);
	}
}

void ambarella_spi_cs_deactivate(struct ambarella_spi_cs_config *cs_config)
{
	u8			cs_pin, cs_high;

	if (cs_config->bus_id >= SPI_MASTER_INSTANCES ||
		cs_config->cs_id >= cs_config->cs_num)
		return;

	cs_pin	= cs_config->cs_pins[cs_config->cs_id];
	cs_high	= cs_config->cs_high[cs_config->cs_id];

	if (!cs_high) {
		ambarella_gpio_set(cs_pin, 1);
	} else {
		ambarella_gpio_set(cs_pin, 0);
	}
}

struct resource ambarella_spi0_resources[] = {
	[0] = {
		.start	= SPI_BASE,
		.end	= SPI_BASE + 0x0FFF,
		.flags	= IORESOURCE_MEM,
	},
	[1] = {
		.start	= SSI_IRQ,
		.end	= SSI_IRQ,
		.flags	= IORESOURCE_IRQ,
	},
};

int ambarella_spi0_cs_pins[] = {
	SSI0_EN0, SSI0_EN1, SSIO_EN2, SSIO_EN3,
	SSI_4_N, SSI_5_N, SSI_6_N, SSI_7_N
};
#if defined(CONFIG_AMBARELLA_SYS_SPI_CALL)
AMBA_SPI_CS_PINS_PARAM_CALL(0, ambarella_spi0_cs_pins, 0644);
#endif
int ambarella_spi0_cs_high[] = {
	0, 0, 0, 0,
	0, 0, 0, 0,
};
#if defined(CONFIG_AMBARELLA_SYS_SPI_CALL)
AMBA_SPI_CS_HIGH_PARAM_CALL(0, ambarella_spi0_cs_high, 0644);
#endif
struct ambarella_spi_platform_info ambarella_spi0_platform_info = {
	.support_dma		= 0,
#if (SPI_MASTER_INSTANCES == 5 )
	.fifo_entries		= 64,
#else
	.fifo_entries		= 16,
#endif
	.cs_num			= ARRAY_SIZE(ambarella_spi0_cs_pins),
	.cs_pins		= ambarella_spi0_cs_pins,
	.cs_high		= ambarella_spi0_cs_high,
	.cs_activate		= ambarella_spi_cs_activate,
	.cs_deactivate		= ambarella_spi_cs_deactivate,
	.rct_set_ssi_pll	= ambarella_ssi_set_pll,
	.get_ssi_freq_hz	= ambarella_ssi_get_pll,
};

struct platform_device ambarella_spi0 = {
	.name		= "ambarella-spi",
	.id		= 0,
	.resource	= ambarella_spi0_resources,
	.num_resources	= ARRAY_SIZE(ambarella_spi0_resources),
	.dev		= {
		.platform_data		= &ambarella_spi0_platform_info,
		.dma_mask		= &ambarella_dmamask,
		.coherent_dma_mask	= DMA_BIT_MASK(32),
	}
};

#if (SPI_MASTER_INSTANCES >= 2)
#if (CHIP_REV == AMB_S2)
struct resource ambarella_spi1_resources[] = {
	[0] = {
		.start	= AHB64_BASE + 0x4000,
		.end	= AHB64_BASE + 0x4000 + 0x0FFF,
		.flags	= IORESOURCE_MEM,
	},
	[1] = {
		.start	= SSI_MASTER_IRQ,
		.end	= SSI_MASTER_IRQ,
		.flags	= IORESOURCE_IRQ,
	},
};
#else
struct resource ambarella_spi1_resources[] = {
	[0] = {
		.start	= SPI2_BASE,
		.end	= SPI2_BASE + 0x0FFF,
		.flags	= IORESOURCE_MEM,
	},
	[1] = {
		.start	= SSI2_IRQ,
		.end	= SSI2_IRQ,
		.flags	= IORESOURCE_IRQ,
	},
};
#endif

int ambarella_spi1_cs_pins[] = {SSI2_0EN, -1, -1, -1, -1, -1, -1, -1};
#if defined(CONFIG_AMBARELLA_SYS_SPI_CALL)
AMBA_SPI_CS_PINS_PARAM_CALL(1, ambarella_spi1_cs_pins, 0644);
#endif
int ambarella_spi1_cs_high[] = {
	0, 0, 0, 0,
	0, 0, 0, 0,
};
#if defined(CONFIG_AMBARELLA_SYS_SPI_CALL)
AMBA_SPI_CS_HIGH_PARAM_CALL(1, ambarella_spi1_cs_high, 0644);
#endif
struct ambarella_spi_platform_info ambarella_spi1_platform_info = {
	.support_dma		= 0,
	.fifo_entries		= 16,
	.cs_num			= ARRAY_SIZE(ambarella_spi1_cs_pins),
	.cs_pins		= ambarella_spi1_cs_pins,
	.cs_high		= ambarella_spi1_cs_high,
	.cs_activate		= ambarella_spi_cs_activate,
	.cs_deactivate		= ambarella_spi_cs_deactivate,
	.rct_set_ssi_pll	= ambarella_ssi2_set_pll,
	.get_ssi_freq_hz	= ambarella_ssi2_get_pll,
};

struct platform_device ambarella_spi1 = {
	.name		= "ambarella-spi",
	.id		= 1,
	.resource	= ambarella_spi1_resources,
	.num_resources	= ARRAY_SIZE(ambarella_spi1_resources),
	.dev		= {
		.platform_data		= &ambarella_spi1_platform_info,
		.dma_mask		= &ambarella_dmamask,
		.coherent_dma_mask	= DMA_BIT_MASK(32),
	}
};
#endif

#if (SPI_MASTER_INSTANCES >= 3)
struct resource ambarella_spi2_resources[] = {
	[0] = {
		.start	= SPI3_BASE,
		.end	= SPI3_BASE + 0x0FFF,
		.flags	= IORESOURCE_MEM,
	},
	[1] = {
		.start	= SSI3_IRQ,
		.end	= SSI3_IRQ,
		.flags	= IORESOURCE_IRQ,
	},
};

int ambarella_spi2_cs_pins[] = {
	SSI3_EN0, SSI3_EN1, SSI3_EN2, SSI3_EN3,
	SSI3_EN4, SSI3_EN5, SSI3_EN6, SSI3_EN7,
};
#if defined(CONFIG_AMBARELLA_SYS_SPI_CALL)
AMBA_SPI_CS_PINS_PARAM_CALL(2, ambarella_spi2_cs_pins, 0644);
#endif
int ambarella_spi2_cs_high[] = {
	0, 0, 0, 0,
	0, 0, 0, 0,
};
#if defined(CONFIG_AMBARELLA_SYS_SPI_CALL)
AMBA_SPI_CS_HIGH_PARAM_CALL(2, ambarella_spi2_cs_high, 0644);
#endif
struct ambarella_spi_platform_info ambarella_spi2_platform_info = {
	.support_dma		= 0,
#if (SPI_MASTER_INSTANCES == 5)
	.fifo_entries		= 64,
#else
	.fifo_entries		= 16,
#endif
	.cs_num			= ARRAY_SIZE(ambarella_spi2_cs_pins),
	.cs_pins		= ambarella_spi2_cs_pins,
	.cs_high		= ambarella_spi2_cs_high,
	.cs_activate		= ambarella_spi_cs_activate,
	.cs_deactivate		= ambarella_spi_cs_deactivate,
	.rct_set_ssi_pll	= ambarella_ssi2_set_pll,
	.get_ssi_freq_hz	= ambarella_ssi2_get_pll,
};

struct platform_device ambarella_spi2 = {
	.name		= "ambarella-spi",
	.id		= 2,
	.resource	= ambarella_spi2_resources,
	.num_resources	= ARRAY_SIZE(ambarella_spi2_resources),
	.dev		= {
		.platform_data		= &ambarella_spi2_platform_info,
		.dma_mask		= &ambarella_dmamask,
		.coherent_dma_mask	= DMA_BIT_MASK(32),
	}
};
#endif

#if (SPI_MASTER_INSTANCES >= 5)
struct resource ambarella_spi3_resources[] = {
	[0] = {
		.start	= SPI4_BASE,
		.end	= SPI4_BASE + 0x0FFF,
		.flags	= IORESOURCE_MEM,
	},
	[1] = {
		.start	= SSI4_IRQ,
		.end	= SSI4_IRQ,
		.flags	= IORESOURCE_IRQ,
	},
};

int ambarella_spi3_cs_pins[] = {SSI4_EN0, -1, -1, -1, -1, -1, -1, -1};
#if defined(CONFIG_AMBARELLA_SYS_SPI_CALL)
AMBA_SPI_CS_PINS_PARAM_CALL(3, ambarella_spi3_cs_pins, 0644);
#endif
int ambarella_spi3_cs_high[] = {
	0, 0, 0, 0,
	0, 0, 0, 0,
};
#if defined(CONFIG_AMBARELLA_SYS_SPI_CALL)
AMBA_SPI_CS_HIGH_PARAM_CALL(3, ambarella_spi3_cs_high, 0644);
#endif
struct ambarella_spi_platform_info ambarella_spi3_platform_info = {
	.support_dma		= 0,
	.fifo_entries		= 16,
	.cs_num			= ARRAY_SIZE(ambarella_spi3_cs_pins),
	.cs_pins		= ambarella_spi3_cs_pins,
	.cs_high		= ambarella_spi3_cs_high,
	.cs_activate		= ambarella_spi_cs_activate,
	.cs_deactivate		= ambarella_spi_cs_deactivate,
	.rct_set_ssi_pll	= ambarella_ssi2_set_pll,
	.get_ssi_freq_hz	= ambarella_ssi2_get_pll,
};

struct platform_device ambarella_spi3 = {
	.name		= "ambarella-spi",
	.id		= 3,
	.resource	= ambarella_spi3_resources,
	.num_resources	= ARRAY_SIZE(ambarella_spi3_resources),
	.dev		= {
		.platform_data		= &ambarella_spi3_platform_info,
		.dma_mask		= &ambarella_dmamask,
		.coherent_dma_mask	= DMA_BIT_MASK(32),
	}
};

struct resource ambarella_spi4_resources[] = {
	[0] = {
		.start	= SSI_DMA_BASE,
		.end	= SSI_DMA_BASE + 0x0FFF,
		.flags	= IORESOURCE_MEM,
	},
	[1] = {
		.start	= SSI_AHB_IRQ,
		.end	= SSI_AHB_IRQ,
		.flags	= IORESOURCE_IRQ,
	},
};

int ambarella_spi4_cs_pins[] = {SSI_AHB_EN0, -1, -1, -1, -1, -1, -1, -1};
#if defined(CONFIG_AMBARELLA_SYS_SPI_CALL)
AMBA_SPI_CS_PINS_PARAM_CALL(4, ambarella_spi4_cs_pins, 0644);
#endif
int ambarella_spi4_cs_high[] = {
	0, 0, 0, 0,
	0, 0, 0, 0,
};
#if defined(CONFIG_AMBARELLA_SYS_SPI_CALL)
AMBA_SPI_CS_HIGH_PARAM_CALL(4, ambarella_spi4_cs_high, 0644);
#endif
struct ambarella_spi_platform_info ambarella_spi4_platform_info = {
	.support_dma		= 0,
#if (SPI_MASTER_INSTANCES == 5)
	.fifo_entries		= 32,
#else
	.fifo_entries		= 16,
#endif
	.cs_num			= ARRAY_SIZE(ambarella_spi4_cs_pins),
	.cs_pins		= ambarella_spi4_cs_pins,
	.cs_high		= ambarella_spi4_cs_high,
	.cs_activate		= ambarella_spi_cs_activate,
	.cs_deactivate		= ambarella_spi_cs_deactivate,
#if (SPI_AHB_INSTANCES >= 1)
	.rct_set_ssi_pll	= ambarella_ssi_ahb_set_pll,
	.get_ssi_freq_hz	= ambarella_ssi_ahb_get_pll,
#else
	.rct_set_ssi_pll	= ambarella_ssi2_set_pll,
	.get_ssi_freq_hz	= ambarella_ssi2_get_pll,
#endif
};

struct platform_device ambarella_spi4 = {
	.name		= "ambarella-spi",
	.id		= 4,
	.resource	= ambarella_spi4_resources,
	.num_resources	= ARRAY_SIZE(ambarella_spi4_resources),
	.dev		= {
		.platform_data		= &ambarella_spi4_platform_info,
		.dma_mask		= &ambarella_dmamask,
		.coherent_dma_mask	= DMA_BIT_MASK(32),
	}
};
#endif

#if ( SPI_SLAVE_INSTANCES >= 1 )
struct resource ambarella_spi_slave_resources[] = {
	[0] = {
		.start	= SPI_SLAVE_BASE,
		.end	= SPI_SLAVE_BASE + 0x0FFF,
		.flags	= IORESOURCE_MEM,
	},
	[1] = {
		.start	= SSI_SLAVE_IRQ,
		.end	= SSI_SLAVE_IRQ,
		.flags	= IORESOURCE_IRQ,
	},
};

struct platform_device ambarella_spi_slave = {
	.name		= "ambarella-spi-slave",
	.id		= 0,
	.resource	= ambarella_spi_slave_resources,
	.num_resources	= ARRAY_SIZE(ambarella_spi_slave_resources),
	.dev		= {
		.platform_data		= NULL,
		.dma_mask		= &ambarella_dmamask,
		.coherent_dma_mask	= DMA_BIT_MASK(32),
	}
};
#endif

