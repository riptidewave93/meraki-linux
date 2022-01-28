/*
 * Cisco Meraki Monsters platform definition file
 *
 * Copyright (C) 2016 Cisco Systems, Inc.
 *
 * This program is free software; you can redistribute  it and/or modify it
 * under  the terms of  the GNU General  Public License as published by the
 * Free Software Foundation;  either version 2 of the  License, or (at your
 * option) any later version.
 */

#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/i2c.h>
#include <linux/platform_device.h>
#include <linux/platform_data/at24.h>
#include <linux/meraki/meraki_config.h>
#include <linux/platform_data/pca953x.h>
#include <linux/platform_data/leds-lp55xx.h>
#include <linux/meraki/cisco-mps.h>
#include <linux/i2c/pca954x.h>
#include <linux/leds.h>
#include <linux/gpio.h>
#include <linux/input.h>
#include <linux/gpio_keys.h>
#include <linux/dmi.h>
#include <asm/reboot.h>
#include <asm/io.h>
#include <linux/meraki/sfp_user.h>
#include <linux/meraki/cisco-act.h>
#include <linux/delay.h>
#include <linux/mfd/core.h>
#include <linux/pci.h>

#define MONSTERS_SOC_GPIO_BASE 436
#define MONSTERS_EXT0_GPIO_BASE 100

#define MONSTERS_EXT_GPIO_BASE(ext_id) (100 + (20*(ext_id)))

#define MONSTERS_SOC_GPIO(x) (MONSTERS_SOC_GPIO_BASE+(x))
#define MONSTERS_EXT_GPIO(ext_id, gpio) (MONSTERS_EXT_GPIO_BASE(ext_id)+(gpio))

static void (*old_native_machine_emergency_restart)(void);

static struct at24_platform_data monsters_config_eeprom_plat_data = {
    .byte_len = 1024,
    .page_size = 16,
    .flags = 0,
    .setup = NULL,
    .context = NULL
};

static struct meraki_config_platform_data monsters_meraki_config_plat_data = {
    .eeprom_data = &monsters_config_eeprom_plat_data,
    .write_protect_gpio = -1,
};

static struct platform_device monsters_meraki_config_device = {
    .name = "meraki-config",
    .id = -1,
    .dev = {
        .platform_data = &monsters_meraki_config_plat_data,
    }
};

/* Security FPGA */
static struct resource monsters_fpga_resource[] = {
    {
        .flags = IORESOURCE_MEM,

        /* Addresses specified by FPGA Engineer. This is standard TPM
           address space
        */
        .start = 0xfed40800,
        .end = 0xfed40800 + 0x3ff,
    },
};
static struct cisco_act_platform_data monsters_fpga_plat_data = {
    .write_buf_size = 0x700,
    .chip_family = TAM_FAMILY_FPGA,
    .software_reset = true,
    .mboot = {
        .enabled = false,
        .reg = 0x00000000,
        .mask = 0x00,
        .active_low = false,
    },
};
static struct mfd_cell lpc_ich_tam_cell = {
    .name = "tam_ich",
    .num_resources = ARRAY_SIZE(monsters_fpga_resource),
    .resources = monsters_fpga_resource,
    .ignore_resource_conflicts = true,
    .platform_data = &monsters_fpga_plat_data,
    .pdata_size = sizeof(struct cisco_act_platform_data),
};

/* PSU */

static struct cisco_mps_platform_data monsters_cisco_mps_plat_data[] = {
    {
        .present_pin = MONSTERS_SOC_GPIO(69),
        .power_good_pin = MONSTERS_SOC_GPIO(0),
        .control_i2c_address = 0x5a,
        .data_i2c_address = 0x52,
        .i2c_adapter_id = 32,
    },
    {
        .present_pin = MONSTERS_SOC_GPIO(5),
        .power_good_pin = MONSTERS_SOC_GPIO(31),
        .control_i2c_address = 0x5b,
        .data_i2c_address = 0x53,
        .i2c_adapter_id = 33,
    },
};

static struct platform_device monsters_cisco_mps_device[] = {
    {
        .name = "cisco-mps",
        .id = 1,
        .dev = {
            .platform_data = &monsters_cisco_mps_plat_data[0],
        },
    },
    {
        .name = "cisco-mps",
        .id = 2,
        .dev = {
            .platform_data = &monsters_cisco_mps_plat_data[1],
        },
    },
};

static struct gpio_led monsters_gpio_leds[] = {
    {
        .name = "power:orange",
        .gpio = MONSTERS_SOC_GPIO(50),
        .default_state = LEDS_GPIO_DEFSTATE_ON,
        .active_low = 1,
    },
    {
        .name = "ha:green",
        .gpio = MONSTERS_SOC_GPIO(15),
        .default_state = LEDS_GPIO_DEFSTATE_OFF,
    },
    {
        .name = "ha:orange",
        .gpio = MONSTERS_SOC_GPIO(57),
        .default_state = LEDS_GPIO_DEFSTATE_OFF,
    },
    {
        .name = "usb:green",
        .gpio = MONSTERS_SOC_GPIO(26),
        .default_state = LEDS_GPIO_DEFSTATE_OFF,
    },
    {
        .name = "usb:orange",
        .gpio = MONSTERS_SOC_GPIO(24),
        .default_state = LEDS_GPIO_DEFSTATE_OFF,
    },

    /*
     * The hwmon sets up fan 1 to be furthest from power supplies
     *
     * So when looking at the hardware, it is:
     *
     * PSU PSU FAN3 FAN2 FAN1 (M2/M3 do not have FAN1)
     */
    {
        .name = "fan3:orange",
        .gpio = MONSTERS_EXT_GPIO(3, 4),
        .default_state = LEDS_GPIO_DEFSTATE_OFF,
        .active_low = 1,
    },
    {
        .name = "fan2:orange",
        .gpio = MONSTERS_EXT_GPIO(3, 2),
        .default_state = LEDS_GPIO_DEFSTATE_OFF,
        .active_low = 1,
    },
    {
        .name = "fan1:orange",
        .gpio = MONSTERS_EXT_GPIO(3, 0),
        .default_state = LEDS_GPIO_DEFSTATE_OFF,
        .active_low = 1,
    },
    {
        .name = "fan3:green",
        .gpio = MONSTERS_EXT_GPIO(3, 5),
        .default_state = LEDS_GPIO_DEFSTATE_OFF,
        .active_low = 1,
    },
    {
        .name = "fan2:green",
        .gpio = MONSTERS_EXT_GPIO(3, 3),
        .default_state = LEDS_GPIO_DEFSTATE_OFF,
        .active_low = 1,
    },
    {
        .name = "fan1:green",
        .gpio = MONSTERS_EXT_GPIO(3, 15),
        .default_state = LEDS_GPIO_DEFSTATE_OFF,
        .active_low = 1,
    },
};

static struct gpio_led_platform_data monsters_gpio_led_plat_data = {
    .num_leds = ARRAY_SIZE(monsters_gpio_leds),
    .leds = monsters_gpio_leds,
};

static struct platform_device monsters_gpio_led_device = {
    .name = "leds-gpio",
    .id = -1,
    .dev = {
        .platform_data = &monsters_gpio_led_plat_data,
    },
};


/********* Restore button *********/

static struct gpio_keys_button monsters_gpio_buttons[] = {
    {
        .code = KEY_RESTART,
        .gpio = MONSTERS_SOC_GPIO(52),
        .active_low = 1,
        .desc = "restore",
        .type = EV_KEY,
    },
};

static struct gpio_keys_platform_data monsters_gpio_keys_plat_data = {
    .buttons = monsters_gpio_buttons,
    .nbuttons = ARRAY_SIZE(monsters_gpio_buttons),
    .poll_interval = 60,
    .rep = 1,
};

static struct platform_device monsters_gpio_keys_device = {
    .name = "gpio-keys-polled",
    .id = -1,
    .dev = {
        .platform_data = &monsters_gpio_keys_plat_data,
    },
};

/****************** SFP ******************/

static struct sfp_user_platform_data monsters_sfp_user_plat_data[] = {
    { // PORT 19, MONSTERS LABELED PORT 22
        .id = 1,
        .gpios = {
              [SFP_USER_GPIO_TX_DIS ] = MONSTERS_EXT_GPIO(0, 0),
              [SFP_USER_GPIO_RX_LOS ] = MONSTERS_EXT_GPIO(0, 1),
              [SFP_USER_GPIO_MOD_ABS] = MONSTERS_EXT_GPIO(0, 2),
        },
        .i2c_adapter_id = 10,
    },
    { // PORT 20, MONSTERS LABELED PORT 21
        .id = 2,
        .gpios = {
              [SFP_USER_GPIO_TX_DIS ] = MONSTERS_EXT_GPIO(0, 3),
              [SFP_USER_GPIO_RX_LOS ] = MONSTERS_EXT_GPIO(0, 4),
              [SFP_USER_GPIO_MOD_ABS] = MONSTERS_EXT_GPIO(0, 5),
        },
        .i2c_adapter_id = 11,
    },
    { // PORT 21, MONSTERS LABELED PORT 19
        .id = 3,
        .gpios = {
              [SFP_USER_GPIO_TX_DIS ] = MONSTERS_EXT_GPIO(0, 6),
              [SFP_USER_GPIO_RX_LOS ] = MONSTERS_EXT_GPIO(0, 7),
              [SFP_USER_GPIO_MOD_ABS] = MONSTERS_EXT_GPIO(0, 10),
        },
        .i2c_adapter_id = 12,
    },
    { // PORT 22, MONSTERS LABELED PORT 26
        .id = 4,
        .gpios = {
              [SFP_USER_GPIO_TX_DIS ] = MONSTERS_EXT_GPIO(0, 11),
              [SFP_USER_GPIO_RX_LOS ] = MONSTERS_EXT_GPIO(0, 12),
              [SFP_USER_GPIO_MOD_ABS] = MONSTERS_EXT_GPIO(0, 13),
        },
        .i2c_adapter_id = 14,
    },
    { // PORT 23, MONSTERS LABELED PORT 20
        .id = 5,
        .gpios = {
              [SFP_USER_GPIO_TX_DIS ] = MONSTERS_EXT_GPIO(0, 14),
              [SFP_USER_GPIO_RX_LOS ] = MONSTERS_EXT_GPIO(0, 15),
              [SFP_USER_GPIO_MOD_ABS] = MONSTERS_EXT_GPIO(0, 8),
        },
        .i2c_adapter_id = 13,
    },
    { // PORT 24, MONSTERS LABELED PORT 25
        .id = 6,
        .gpios = {
              [SFP_USER_GPIO_TX_DIS ] = MONSTERS_EXT_GPIO(1, 0),
              [SFP_USER_GPIO_RX_LOS ] = MONSTERS_EXT_GPIO(1, 1),
              [SFP_USER_GPIO_MOD_ABS] = MONSTERS_EXT_GPIO(1, 2),
        },
        .i2c_adapter_id = 15,
    },
    { // PORT 25, MONSTERS LABELED PORT 24
        .id = 7,
        .gpios = {
              [SFP_USER_GPIO_TX_DIS ] = MONSTERS_EXT_GPIO(1, 3),
              [SFP_USER_GPIO_RX_LOS ] = MONSTERS_EXT_GPIO(1, 4),
              [SFP_USER_GPIO_MOD_ABS] = MONSTERS_EXT_GPIO(1, 5),
        },
        .i2c_adapter_id = 16,
    },
    { // PORT 26, MONSTERS LABELED PORT 11
        .id = 8,
        .gpios = {
              [SFP_USER_GPIO_TX_DIS ] = MONSTERS_EXT_GPIO(1, 6),
              [SFP_USER_GPIO_RX_LOS ] = MONSTERS_EXT_GPIO(1, 7),
              [SFP_USER_GPIO_MOD_ABS] = MONSTERS_EXT_GPIO(1, 10),
        },
        .i2c_adapter_id = -1,
    },
    { // PORT 11, MONSTERS LABELED PORT 12
        .id = 9,
        .gpios = {
              [SFP_USER_GPIO_TX_DIS ] = MONSTERS_EXT_GPIO(1, 11),
              [SFP_USER_GPIO_RX_LOS ] = MONSTERS_EXT_GPIO(1, 12),
              [SFP_USER_GPIO_MOD_ABS] = MONSTERS_EXT_GPIO(1, 13),
        },
        .i2c_adapter_id = -1,
    },
    { // PORT 12, MONSTERS LABELED PORT 23
        .id = 10,
        .gpios = {
              [SFP_USER_GPIO_TX_DIS ] = MONSTERS_EXT_GPIO(1, 14),
              [SFP_USER_GPIO_RX_LOS ] = MONSTERS_EXT_GPIO(1, 15),
              [SFP_USER_GPIO_MOD_ABS] = MONSTERS_EXT_GPIO(1, 8),
        },
        .i2c_adapter_id = 17,
    },
    { // PORT 13, MONSTERS LABELED PORT 13
        .id = 11,
        .gpios = {
              [SFP_USER_GPIO_TX_DIS ] = MONSTERS_EXT_GPIO(2, 0),
              [SFP_USER_GPIO_RX_LOS ] = MONSTERS_EXT_GPIO(2, 1),
              [SFP_USER_GPIO_MOD_ABS] = MONSTERS_EXT_GPIO(2, 2),
        },
        .i2c_adapter_id = -1,
    },
    { // PORT 14, MONSTERS LABELED PORT 14
        .id = 12,
        .gpios = {
              [SFP_USER_GPIO_TX_DIS ] = MONSTERS_EXT_GPIO(2, 3),
              [SFP_USER_GPIO_RX_LOS ] = MONSTERS_EXT_GPIO(2, 4),
              [SFP_USER_GPIO_MOD_ABS] = MONSTERS_EXT_GPIO(2, 5),
        },
        .i2c_adapter_id = -1,
    },
    { // PORT 15, MONSTERS LABELED PORT 16
        .id = 13,
        .gpios = {
              [SFP_USER_GPIO_TX_DIS ] = MONSTERS_EXT_GPIO(2, 6),
              [SFP_USER_GPIO_RX_LOS ] = MONSTERS_EXT_GPIO(2, 7),
              [SFP_USER_GPIO_MOD_ABS] = MONSTERS_EXT_GPIO(2, 10),
        },
        .i2c_adapter_id = -1,
    },
    { // PORT 16, MONSTERS LABELED PORT 17
        .id = 14,
        .gpios = {
              [SFP_USER_GPIO_TX_DIS ] = MONSTERS_EXT_GPIO(2, 11),
              [SFP_USER_GPIO_RX_LOS ] = MONSTERS_EXT_GPIO(2, 12),
              [SFP_USER_GPIO_MOD_ABS] = MONSTERS_EXT_GPIO(2, 13),
        },
        .i2c_adapter_id = -1,
    },
    { // PORT 17, MONSTERS LABELED PORT 15
        .id = 15,
        .gpios = {
              [SFP_USER_GPIO_TX_DIS ] = MONSTERS_EXT_GPIO(2, 14),
              [SFP_USER_GPIO_RX_LOS ] = MONSTERS_EXT_GPIO(2, 15),
              [SFP_USER_GPIO_MOD_ABS] = MONSTERS_EXT_GPIO(2, 8),
        },
        .i2c_adapter_id = -1,
    },
    { // PORT 18, MONSTERS PORT 1 (UPLINK)
        .id = 16,
        .gpios = {
              [SFP_USER_GPIO_TX_DIS ] = MONSTERS_EXT_GPIO(3, 6),
              [SFP_USER_GPIO_RX_LOS ] = MONSTERS_EXT_GPIO(3, 7),
              [SFP_USER_GPIO_MOD_ABS] = MONSTERS_EXT_GPIO(3, 10),
        },
        .i2c_adapter_id = 30,
    },
    { // PORT 1, MONSTERS PORT 2 (UPLINK)
        .id = 17,
        .gpios = {
              [SFP_USER_GPIO_TX_DIS ] = MONSTERS_EXT_GPIO(3, 11),
              [SFP_USER_GPIO_RX_LOS ] = MONSTERS_EXT_GPIO(3, 12),
              [SFP_USER_GPIO_MOD_ABS] = MONSTERS_EXT_GPIO(3, 13),
        },
        .i2c_adapter_id = 31,
    },
    { // PORT 2, MONSTERS PORT 18
        .id = 18,
        .gpios = {
              [SFP_USER_GPIO_TX_DIS ] = MONSTERS_EXT_GPIO(3, 14),
              // FIXME the schematic is wrong for this one.
              //[SFP_USER_GPIO_RX_LOS ] = MONSTERS_EXT_GPIO(3, 15),
              [SFP_USER_GPIO_MOD_ABS] = MONSTERS_EXT_GPIO(3, 8),
        },
        .i2c_adapter_id = -1,
    },
};

static struct platform_device monsters_sfp_user_plat_devices[] = {
    {
        .name = "sfp-user",
        .id = 1,
        .dev = {
            .platform_data = &monsters_sfp_user_plat_data[0],
        },
    },
    {
        .name = "sfp-user",
        .id = 2,
        .dev = {
            .platform_data = &monsters_sfp_user_plat_data[1],
        },
    },
    {
        .name = "sfp-user",
        .id = 3,
        .dev = {
            .platform_data = &monsters_sfp_user_plat_data[2],
        },
    },
    {
        .name = "sfp-user",
        .id = 4,
        .dev = {
            .platform_data = &monsters_sfp_user_plat_data[3],
        },
    },
    {
        .name = "sfp-user",
        .id = 5,
        .dev = {
            .platform_data = &monsters_sfp_user_plat_data[4],
        },
    },
    {
        .name = "sfp-user",
        .id = 6,
        .dev = {
            .platform_data = &monsters_sfp_user_plat_data[5],
        },
    },
    {
        .name = "sfp-user",
        .id = 7,
        .dev = {
            .platform_data = &monsters_sfp_user_plat_data[6],
        },
    },
    {
        .name = "sfp-user",
        .id = 8,
        .dev = {
            .platform_data = &monsters_sfp_user_plat_data[7],
        },
    },
    {
        .name = "sfp-user",
        .id = 9,
        .dev = {
            .platform_data = &monsters_sfp_user_plat_data[8],
        },
    },
    {
        .name = "sfp-user",
        .id = 10,
        .dev = {
            .platform_data = &monsters_sfp_user_plat_data[9],
        },
    },
    {
        .name = "sfp-user",
        .id = 11,
        .dev = {
            .platform_data = &monsters_sfp_user_plat_data[10],
        },
    },
    {
        .name = "sfp-user",
        .id = 12,
        .dev = {
            .platform_data = &monsters_sfp_user_plat_data[11],
        },
    },
    {
        .name = "sfp-user",
        .id = 13,
        .dev = {
            .platform_data = &monsters_sfp_user_plat_data[12],
        },
    },
    {
        .name = "sfp-user",
        .id = 14,
        .dev = {
            .platform_data = &monsters_sfp_user_plat_data[13],
        },
    },
    {
        .name = "sfp-user",
        .id = 15,
        .dev = {
            .platform_data = &monsters_sfp_user_plat_data[14],
        },
    },
    {
        .name = "sfp-user",
        .id = 16,
        .dev = {
            .platform_data = &monsters_sfp_user_plat_data[15],
        },
    },
    {
        .name = "sfp-user",
        .id = 17,
        .dev = {
            .platform_data = &monsters_sfp_user_plat_data[16],
        },
    },
    {
        .name = "sfp-user",
        .id = 18,
        .dev = {
            .platform_data = &monsters_sfp_user_plat_data[17],
        },
    },
};



/********* Platform device config ********/

static struct platform_device* monsters_devices[] __initdata = {
    &monsters_meraki_config_device,
    &monsters_gpio_led_device,
    &monsters_gpio_keys_device,
    &monsters_cisco_mps_device[0],
    &monsters_cisco_mps_device[1],
    &monsters_sfp_user_plat_devices[0],
    &monsters_sfp_user_plat_devices[1],
    &monsters_sfp_user_plat_devices[2],
    &monsters_sfp_user_plat_devices[3],
    &monsters_sfp_user_plat_devices[4],
    &monsters_sfp_user_plat_devices[5],
    &monsters_sfp_user_plat_devices[6],
    &monsters_sfp_user_plat_devices[7],
    &monsters_sfp_user_plat_devices[8],
    &monsters_sfp_user_plat_devices[9],
    &monsters_sfp_user_plat_devices[10],
    &monsters_sfp_user_plat_devices[11],
    &monsters_sfp_user_plat_devices[12],
    &monsters_sfp_user_plat_devices[13],
    &monsters_sfp_user_plat_devices[14],
    &monsters_sfp_user_plat_devices[15],
    &monsters_sfp_user_plat_devices[16],
    &monsters_sfp_user_plat_devices[17],
};

/********* Tri-color LED controller (LP5562) ********/

static struct lp55xx_led_config monsters_lp5562_channels[] = {
    {
        .name = "tricolor0:red",
        .chan_nr = 0,
        .led_current = 150,
        .max_current = 255,
    },
    {
        .name = "tricolor0:green",
        .chan_nr = 1,
        .led_current = 150,
        .max_current = 255,
    },
    {
        .name = "tricolor0:blue",
        .chan_nr = 2,
        .led_current = 150,
        .max_current = 255,
    },
    {
        .name = "power:white",
        .chan_nr = 3,
        .led_current = 150,
        .max_current = 225,
    },
};

static struct lp55xx_platform_data monsters_lp5562_plat_data = {
    .led_config = monsters_lp5562_channels,
    .num_channels = ARRAY_SIZE(monsters_lp5562_channels),
    .clock_mode = LP55XX_CLOCK_INT,
    .enable_gpio = -1,
};

static struct pca954x_platform_mode monsters_i2cmux_plat_modes1[] = {
    {
        .adap_id = 10,
        .deselect_on_exit = 1,
    },
    {
        .adap_id = 11,
        .deselect_on_exit = 1,
    },
    {
        .adap_id = 12,
        .deselect_on_exit = 1,
    },
    {
        .adap_id = 13,
        .deselect_on_exit = 1,
    },
    {
        .adap_id = 14,
        .deselect_on_exit = 1,
    },
    {
        .adap_id = 15,
        .deselect_on_exit = 1,
    },
    {
        .adap_id = 16,
        .deselect_on_exit = 1,
    },
    {
        .adap_id = 17,
        .deselect_on_exit = 1,
    },
};

static struct pca954x_platform_mode monsters_i2cmux_plat_modes2[] = {
    {
        .adap_id = 20,
        .deselect_on_exit = 1,
    },
    {
        .adap_id = 21,
        .deselect_on_exit = 1,
    },
    {
        .adap_id = 22,
        .deselect_on_exit = 1,
    },
    {
        .adap_id = 23,
        .deselect_on_exit = 1,
    },
    {
        .adap_id = 24,
        .deselect_on_exit = 1,
    },
    {
        .adap_id = 25,
        .deselect_on_exit = 1,
    },
    {
        .adap_id = 26,
        .deselect_on_exit = 1,
    },
    {
        .adap_id = 27,
        .deselect_on_exit = 1,
    },
};

static struct pca954x_platform_mode monsters_i2cmux_plat_modes3[] = {
    {
        .adap_id = 30,
        .deselect_on_exit = 1,
    },
    {
        .adap_id = 31,
        .deselect_on_exit = 1,
    },
    {
        .adap_id = 32,
        .deselect_on_exit = 1,
    },
    {
        .adap_id = 33,
        .deselect_on_exit = 1,
    },
    {
        .adap_id = 34,
        .deselect_on_exit = 1,
    },
    {
        .adap_id = 35,
        .deselect_on_exit = 1,
    },
    {
        .adap_id = 36,
        .deselect_on_exit = 1,
    },
    {
        .adap_id = 37,
        .deselect_on_exit = 1,
    },
};

static struct pca954x_platform_data monsters_i2cmux_plat_data1 = {
    .modes = monsters_i2cmux_plat_modes1,
    .num_modes = ARRAY_SIZE(monsters_i2cmux_plat_modes1),
};

static struct pca954x_platform_data monsters_i2cmux_plat_data2 = {
    .modes = monsters_i2cmux_plat_modes2,
    .num_modes = ARRAY_SIZE(monsters_i2cmux_plat_modes2),
};

static struct pca954x_platform_data monsters_i2cmux_plat_data3 = {
    .modes = monsters_i2cmux_plat_modes3,
    .num_modes = ARRAY_SIZE(monsters_i2cmux_plat_modes3),
};

static struct pca953x_platform_data monsters_gpiox_plat_data[] = {
    {
        .gpio_base = MONSTERS_EXT_GPIO_BASE(0),
        .irq_base = 0,
    },
    {
        .gpio_base = MONSTERS_EXT_GPIO_BASE(1),
        .irq_base = 0,
    },
    {
        .gpio_base = MONSTERS_EXT_GPIO_BASE(2),
        .irq_base = 0,
    },
    {
        .gpio_base = MONSTERS_EXT_GPIO_BASE(3),
        .irq_base = 0,
    },
};

static const struct i2c_board_info monsters_i2c0_board_info[] = {
    {
        I2C_BOARD_INFO("lp5562", 0x30),
        .platform_data = &monsters_lp5562_plat_data,
    },
    {
        I2C_BOARD_INFO("24c08", 0x54),
        .platform_data = &monsters_config_eeprom_plat_data,
    },
    {
        I2C_BOARD_INFO("pca9548", 0x70),
        .platform_data = &monsters_i2cmux_plat_data1,
    },
    {
        I2C_BOARD_INFO("pca9548", 0x71),
        .platform_data = &monsters_i2cmux_plat_data2,
    },
    {
        I2C_BOARD_INFO("pca9548", 0x72),
        .platform_data = &monsters_i2cmux_plat_data3,
    },
};

// I2C expanders

static const struct i2c_board_info monsters_i2c34_board_info[] = {
    {
        I2C_BOARD_INFO("pca9555", 0x20),
        .platform_data = &monsters_gpiox_plat_data[0],
    },
};

static const struct i2c_board_info monsters_i2c35_board_info[] = {
    {
        I2C_BOARD_INFO("pca9555", 0x21),
        .platform_data = &monsters_gpiox_plat_data[1],
    },
};

static const struct i2c_board_info monsters_i2c36_board_info[] = {
    {
        I2C_BOARD_INFO("pca9555", 0x22),
        .platform_data = &monsters_gpiox_plat_data[2],
    },
};

static const struct i2c_board_info monsters_i2c37_board_info[] = {
    {
        I2C_BOARD_INFO("pca9555", 0x25),
        .platform_data = &monsters_gpiox_plat_data[3],
    },
};

static void monsters_add_i2c_devices(int busnum, const struct i2c_board_info* info,
                                    unsigned count)
{
    int i;
    struct i2c_adapter* i2c;

    i2c = i2c_get_adapter(busnum);
    if (i2c) {
        for (i = 0; i < count; i++) {
            if (i2c_new_device(i2c, &info[i]) == NULL) {
                printk(KERN_ERR "i2c_new_device failed (i2c%d, %s, %02X)", busnum,
                       info[i].type, info[i].addr);
            }
        }
        i2c_put_adapter(i2c);
    } else {
        printk(KERN_CRIT "i2c%d not found", busnum);
    }
}

#define GPIO_BASE 0x500

#define GPIO_USE_SEL (GPIO_BASE + 0x0)
#define GPIO_LVL (GPIO_BASE + 0x0C)
#define GPIO_USE_SEL2 (GPIO_BASE + 0x30)

#define GPIO_25_SHIFT (0x1 << 25)
#define GPIO_26_SHIFT (0x1 << 26)
#define GPIO_44_SHIFT (0x1 << 12)

static void setup_gpio_pins(void)
{
    unsigned int io_mem;

    io_mem = inl_p(GPIO_USE_SEL);
    io_mem |= GPIO_25_SHIFT;
    io_mem |= GPIO_26_SHIFT;
    outl_p(io_mem, GPIO_USE_SEL);

    io_mem = inl_p(GPIO_LVL);
    io_mem &= ~(GPIO_25_SHIFT); // Turn GPIO 25 to 0, so PSU1 is online
    outl_p(io_mem, GPIO_LVL);

    io_mem = inl_p(GPIO_USE_SEL2);
    io_mem |= (GPIO_44_SHIFT); // Turn on GPIO 44 DERST_n for FPGA reset
    outl_p(io_mem, GPIO_USE_SEL2);
}

/*
    monsters_restart determines if we need to do a special IAP restart via
    FPGA, or go through the native machine restart. We saved a function
    pointer to the original emergency_restart function to call if there is
    no need for an IAP restart.

    IAP restart is required after an FPGA or TAM image field upgrade.

*/

static void monsters_restart(void)
{
    struct device* dev;
    uint32_t magic_reset_val = 0x0000003a;
    uint32_t status_register = 0x0;
    uint32_t host_register = 0x0;
    uint16_t counter = 0;
    const uint16_t MAX_COUNTER = 90;

    /* Grab a reference to the LPC driver responsible for FPGA transactions.
    */
    dev = find_act_device_by_name("tam_ich");

    if (!dev)
    {
        printk("tam_ich driver not found\n");
        old_native_machine_emergency_restart();
    }

    /* Check if we need to do IAP reset due to FPGA Upgrade. This
       flag ought or ought not have been set in the "tam" utility executable
    */
    act_reg_read(dev, FPGA_HOST_STORAGE_ADDR_START + FPGA_HOST_IAP_TRIGGER_OFFSET,
                 (uint8_t*)&host_register, 0x4);

    if (host_register & 0x2) {
        /* If so, write 0x3a to register 0x2000 to reset board. This restart
           process takes roughtly three minutes.
        */
        printk("Upgrading Security Chip, this should take 3 minutes\n");
        act_reg_write(dev, 0x2000, (uint8_t*)&magic_reset_val, 0x4);

        /* IAP reset first starts by authenticating the image in eSPI. This
           takes about 5 to 10 seconds. Bit 18 of the Test and Status register
           is set upon writing 0x3a to reg 0x2000, and is cleared when
           finished. If succcessful, the system will halt and we will not see
           this bit clear.
        */
        do {
            /* Don't get caught in an infinite loop, break after a certain
               number of tries.
            */
            if(counter++ > MAX_COUNTER) {
                break;
            }

            mdelay(1000);
            act_reg_read(dev, 0x200c, (uint8_t*)&status_register, 0x4);
        }
        while(status_register & (1<<18));

        /* If after a small arbitrary delay the system is not halted and
           bit 18 is cleared, then upgrade must have failed
        */
        printk("Security Chip Upgrade Failed\n");
    }

    /* If we got here, we are either not triggering IAP reset, or we did and
       it failed. In either case, run through our native reset function
    */
    old_native_machine_emergency_restart();
}

static int monsters_match_name(struct device *dev, void *data)
{
    const char *name = data;
    if(dev && dev->driver)
    {
       return sysfs_streq(name, dev->driver->name);
    }
    return 0;
}

static int __init monsters_lateinit(void)
{
    struct device* dev;
    const char *vendor, *product;
    vendor = dmi_get_system_info(DMI_SYS_VENDOR);
    if (!vendor || strcmp(vendor, "Cisco"))
            goto getout;

    product = dmi_get_system_info(DMI_PRODUCT_NAME);
    if (!product || strcmp(product, "Monsters"))
            goto getout;

    setup_gpio_pins();

    monsters_add_i2c_devices(0, monsters_i2c0_board_info, ARRAY_SIZE(monsters_i2c0_board_info));
    monsters_add_i2c_devices(34, monsters_i2c34_board_info, ARRAY_SIZE(monsters_i2c34_board_info));
    monsters_add_i2c_devices(35, monsters_i2c35_board_info, ARRAY_SIZE(monsters_i2c35_board_info));
    monsters_add_i2c_devices(36, monsters_i2c36_board_info, ARRAY_SIZE(monsters_i2c36_board_info));
    monsters_add_i2c_devices(37, monsters_i2c37_board_info, ARRAY_SIZE(monsters_i2c37_board_info));
    platform_add_devices(monsters_devices, ARRAY_SIZE(monsters_devices));

    /* This driver is on the LPC bus and should be added to lpc_ich via MFD */
    dev = bus_find_device(&pci_bus_type, NULL, "lpc_ich", monsters_match_name);
    if (!dev) {
        printk("cisco-act Failed to find lpc_ich\n");
    }
    else {
        mfd_add_devices(dev, PLATFORM_DEVID_NONE, &lpc_ich_tam_cell, 1, NULL, 0, NULL);
    }

    /* Save the original emergency_restart fn pointer to call on later */
    old_native_machine_emergency_restart = machine_ops.emergency_restart;
    machine_ops.emergency_restart = monsters_restart;

    printk(KERN_INFO "Cisco Meraki Monsters Init\n");

getout:
    return 0;
}

late_initcall(monsters_lateinit);

MODULE_AUTHOR("Miles Meraki <miles@meraki.com>");
MODULE_DESCRIPTION("Cisco Meraki Monsters platform definition");
MODULE_LICENSE("GPL");
