/*
 * Cisco Meraki MS350 platform definition file
 *
 * Copyright (C) 2015 Cisco Systems, Inc.
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
#include <linux/meraki/cisco-mps.h>
#include <linux/i2c/pca954x.h>
#include <linux/platform_data/pca953x.h>
#include <linux/platform_data/leds-lp55xx.h>
#include <linux/leds.h>
#include <linux/gpio.h>
#include <linux/input.h>
#include <linux/gpio_keys.h>
#include <asm/reboot.h>
#include <linux/bcm59121.h>

/* Described on the MS350 schematic */
enum bigpony_board_id {
    BOARD_ID_MS350_48FP = 0,
    BOARD_ID_MS350_48LP = 1,
    BOARD_ID_MS350_48 = 2,
    BOARD_ID_MS350_24P = 3,
    BOARD_ID_MS350_24 = 4,
};

static struct kobject* bigpony_kobj;
static int bigpony_board_id = -1;

#define BIGPONY_SOC_GPIO_BASE 196
#define BIGPONY_SOC_SUS_GPIO_BASE 228
#define BIGPONY_EXT_GPIO_BASE 100

#define BIGPONY_SOC_GPIO(x) (BIGPONY_SOC_GPIO_BASE+(x))
#define BIGPONY_SOC_SUS_GPIO(x) (BIGPONY_SOC_SUS_GPIO_BASE+(x))
#define BIGPONY_EXT_GPIO(x) (BIGPONY_EXT_GPIO_BASE+(x))

/******** meraki_config *******/

#define POE_I2C_ADDRESS 0x20
#define POE_I2C_BUS_NUMBER 1

#define POE_GPIO_DISABLE BIGPONY_SOC_GPIO(25)      /* PoE Disable GPIO pin */
#define POE_GPIO_RESET   BIGPONY_SOC_SUS_GPIO(19)    /* PoE Reset GPIO pin */

static struct bcm59121_platform_data bigpony_bcm59121_data = {
    .port_base          =       1, /* Match numbers on faceplate */
    .gpio_disable       =       POE_GPIO_DISABLE,
    .gpio_reset         =       POE_GPIO_RESET,
    .therm_domain       =       "system",
    .temp_warm          =       80,
    .power_budget       =       0,
    .fast_state_detect  =       true,
    .logic_low          =       true,
};

static struct at24_platform_data bigpony_config_eeprom_plat_data = {
    .byte_len = 1024,
    .page_size = 8,
    .flags = 0,
    .setup = NULL,
    .context = NULL
};

static struct meraki_config_platform_data bigpony_meraki_config_plat_data = {
    .eeprom_data = &bigpony_config_eeprom_plat_data,
    .write_protect_gpio = -1,
    .write_protect_active_low = false
};

#ifndef CONFIG_MERAKI_BIGPONY_BOOTKERNEL
static struct pca953x_platform_data bigpony_gpiox_plat_data = {
    .gpio_base = BIGPONY_EXT_GPIO_BASE,
    .irq_base = -1,
};
#endif

static struct platform_device bigpony_meraki_config_device = {
    .name = "meraki-config",
    .id = -1,
    .dev = {
        .platform_data = &bigpony_meraki_config_plat_data,
    }
};

/********* Cisco MPS config ********/

static struct cisco_mps_platform_data bigpony_cisco_mps_plat_data[] = {
    {
        .present_pin = BIGPONY_SOC_GPIO(5),
        .power_good_pin = BIGPONY_SOC_SUS_GPIO(10),
        .control_i2c_address = 0x5a,
        .data_i2c_address = 0x52,
        .i2c_adapter_id = 10,
    },
    {
        .present_pin = BIGPONY_SOC_GPIO(10),
        .power_good_pin = BIGPONY_SOC_SUS_GPIO(5),
        .control_i2c_address = 0x5b,
        .data_i2c_address = 0x53,
        .i2c_adapter_id = 11,
    },
};

static struct platform_device bigpony_cisco_mps_device[] = {
    {
        .name = "cisco-mps",
        .id = 1,
        .dev = {
            .platform_data = &bigpony_cisco_mps_plat_data[0],
        },
    },
    {
        .name = "cisco-mps",
        .id = 2,
        .dev = {
            .platform_data = &bigpony_cisco_mps_plat_data[1],
        },
    },
};

/********* Orange and white power LEDs *********/

static struct gpio_led bigpony_gpio_leds[] = {
    {
        .name = "power:white",
        .gpio = BIGPONY_SOC_SUS_GPIO(17),
        .default_state = LEDS_GPIO_DEFSTATE_OFF,
    },
    {
        .name = "power:orange",
        .gpio = BIGPONY_SOC_GPIO(4),
        .active_low = 1,
        .default_state = LEDS_GPIO_DEFSTATE_ON,
    },
};

static struct gpio_led_platform_data bigpony_gpio_led_plat_data = {
    .num_leds = ARRAY_SIZE(bigpony_gpio_leds),
    .leds = bigpony_gpio_leds,
};

static struct platform_device bigpony_gpio_led_device = {
    .name = "leds-gpio",
    .id = -1,
    .dev = {
        .platform_data = &bigpony_gpio_led_plat_data,
    },
};

/********* Restore button *********/


static struct gpio_keys_button bigpony_gpio_buttons[] = {
    {
        .code = KEY_RESTART,
        .gpio = BIGPONY_SOC_SUS_GPIO(4),
        .active_low = 1,
        .desc = "restore",
        .type = EV_KEY
    },
};

static struct gpio_keys_platform_data bigpony_gpio_keys_plat_data = {
    .buttons = bigpony_gpio_buttons,
    .nbuttons = ARRAY_SIZE(bigpony_gpio_buttons),
    .poll_interval = 60,
    .rep = 1
};

static struct platform_device bigpony_gpio_keys_device = {
    .name = "gpio-keys-polled",
    .id = -1,
    .dev = {
        .platform_data = &bigpony_gpio_keys_plat_data,
    },
};

/********* Platform device config ********/

static struct platform_device* bigpony_devices[] __initdata = {
    &bigpony_meraki_config_device,
    &bigpony_gpio_led_device,
    &bigpony_gpio_keys_device,
    &bigpony_cisco_mps_device[0],
    &bigpony_cisco_mps_device[1],
};


/********* I2C0 **********/
#ifndef CONFIG_MERAKI_BIGPONY_BOOTKERNEL
static const struct i2c_board_info bigpony_i2c0_board_info[] = {
    {
        I2C_BOARD_INFO("lm75", 0x48),
    },
    {
        I2C_BOARD_INFO("pca9555", 0x20),
        .platform_data = &bigpony_gpiox_plat_data
    },
};
#endif


/********* I2C1 *********/
/********* I2C mux (PCA9547) *********/

static struct pca954x_platform_mode bigpony_i2cmux_plat_modes[] = {
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


static struct pca954x_platform_data bigpony_i2cmux_plat_data = {
    .modes = bigpony_i2cmux_plat_modes,
    .num_modes = ARRAY_SIZE(bigpony_i2cmux_plat_modes),
};

/********* Tri-color LED controller (LP5521) ********/

static struct lp55xx_led_config bigpony_lp5521_channels[] = {
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
};

static struct lp55xx_platform_data bigpony_lp5521_plat_data = {
    .led_config = bigpony_lp5521_channels,
    .num_channels = ARRAY_SIZE(bigpony_lp5521_channels),
    .clock_mode = LP55XX_CLOCK_INT,
    .enable_gpio = BIGPONY_SOC_GPIO(24),
};

static const struct i2c_board_info bigpony_i2c1_board_info[] = {
    {
        I2C_BOARD_INFO("24c8", 0x54),
        .platform_data = &bigpony_config_eeprom_plat_data,
    },
    {
        I2C_BOARD_INFO("pca9547", 0x71),
        .platform_data = &bigpony_i2cmux_plat_data
    },
    {
        I2C_BOARD_INFO("bcm59121", POE_I2C_ADDRESS),
        .platform_data = &bigpony_bcm59121_data,
    },
};

#ifndef CONFIG_MERAKI_BIGPONY_BOOTKERNEL
static const struct i2c_board_info bigpony_i2c14_board_info[] = {
    {
        I2C_BOARD_INFO("pmbus", 0x10),
    },
};

static const struct i2c_board_info bigpony_i2c15_board_info[] = {
    {
        I2C_BOARD_INFO("lm75", 0x49),
    },
};

static const struct i2c_board_info bigpony_i2c16_board_info[] = {
    {
        I2C_BOARD_INFO("lm75", 0x4a),
    },
};
#endif

static const struct i2c_board_info bigpony_i2c17_board_info[] = {
    {
        I2C_BOARD_INFO("lp5521", 0x32),
        .platform_data = &bigpony_lp5521_plat_data,
    }
};

static const struct gpio bigpony_reset_gpios[] = {
    {
        .gpio = BIGPONY_SOC_GPIO(1),
        .flags = GPIOF_OUT_INIT_HIGH,
        .label = "rst_emmc",
    },
    {
        .gpio = BIGPONY_SOC_SUS_GPIO(3),
        .flags = GPIOF_OUT_INIT_HIGH,
        .label = "rst_mac",
    },
    {
        .gpio = BIGPONY_SOC_SUS_GPIO(20),
        .flags = GPIOF_OUT_INIT_HIGH,
        .label = "rst_phy1-3",
    },
    {
        .gpio = BIGPONY_SOC_SUS_GPIO(21),
        .flags = GPIOF_OUT_INIT_HIGH,
        .label = "rst_phy4-6",
    },
    {
        .gpio = BIGPONY_SOC_SUS_GPIO(22),
        .flags = GPIOF_OUT_INIT_HIGH,
        .label = "rst_bcm54616s",
    },
    {
        .gpio = BIGPONY_SOC_SUS_GPIO(27),
        .flags = GPIOF_OUT_INIT_HIGH,
        .label = "rst_phy7",
    },
};

static const struct gpio bigpony_board_id_gpios[] = {
    {
        .gpio = BIGPONY_SOC_GPIO(21),
        .flags = GPIOF_IN,
        .label = "bd_id0",
    },
    {
        .gpio = BIGPONY_SOC_GPIO(22),
        .flags = GPIOF_IN,
        .label = "bd_id1",
    },
    {
        .gpio = BIGPONY_SOC_GPIO(23),
        .flags = GPIOF_IN,
        .label = "bd_id2",
    },
};

static void bigpony_restart(void)
{
    printk(KERN_NOTICE "bigpony_restart\n");
    gpio_set_value(BIGPONY_SOC_SUS_GPIO(18), 0);
}

static void bigpony_add_i2c_devices(int busnum, const struct i2c_board_info* info,
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

static ssize_t board_id_show(struct kobject* kobj,
                             struct kobj_attribute* attr,
                             char* buf)
{
    return snprintf(buf, PAGE_SIZE, "%d\n", bigpony_board_id);
}

static struct kobj_attribute board_id_attr =
    __ATTR(board_id, S_IRUGO, board_id_show, NULL);

static int bigpony_sysfs_init(void)
{
    int ret;

    if (!bigpony_kobj)
        bigpony_kobj = kobject_create_and_add("bigpony", firmware_kobj);
    if (!bigpony_kobj) {
        printk(KERN_WARNING "kobject_create_and_add bigpony failed\n");
        return -EINVAL;
    }

    ret = sysfs_create_file(bigpony_kobj, &board_id_attr.attr);
    if (ret) {
        printk(KERN_WARNING "sysfs_create_file board_id failed\n");
        return ret;
    }

    return 0;
}

static int __init bigpony_lateinit(void)
{
    int ret;

    if ((ret = gpio_request_one(BIGPONY_SOC_SUS_GPIO(18),
                                GPIOF_OPEN_DRAIN | GPIOF_OUT_INIT_HIGH,
                                "master reset signal")) < 0)
        printk(KERN_WARNING "Unable to request master reset GPIO, error = %d\n", ret);
    else
        machine_ops.emergency_restart = bigpony_restart;

    if ((ret = gpio_request_array(bigpony_reset_gpios, ARRAY_SIZE(bigpony_reset_gpios))) < 0)
        printk(KERN_WARNING "Unable to request peripheral reset GPIOs, error = %d\n", ret);

    if ((ret = gpio_request_array(bigpony_board_id_gpios, ARRAY_SIZE(bigpony_board_id_gpios))) < 0) {
        printk(KERN_WARNING "Unable to request board id GPIOs, error = %d\n", ret);
    } else {        int val;
        int i;

        bigpony_board_id = 0;
        for (i = 0; i < ARRAY_SIZE(bigpony_board_id_gpios); i++) {
            val = gpio_get_value(bigpony_board_id_gpios[i].gpio);
            if (val < 0) {
                bigpony_board_id = -1;
                break;
            }
            bigpony_board_id |= (val << i);
        }

        if (bigpony_board_id < 0)
            printk(KERN_WARNING "Unable to read board id gpios!");
    }

    bigpony_sysfs_init();

#ifndef CONFIG_MERAKI_BIGPONY_BOOTKERNEL
    bigpony_add_i2c_devices(0, bigpony_i2c0_board_info, ARRAY_SIZE(bigpony_i2c0_board_info));
#endif
    bigpony_add_i2c_devices(1, bigpony_i2c1_board_info, ARRAY_SIZE(bigpony_i2c1_board_info));
    platform_add_devices(bigpony_devices, ARRAY_SIZE(bigpony_devices));
#ifndef CONFIG_MERAKI_BIGPONY_BOOTKERNEL
    bigpony_add_i2c_devices(14, bigpony_i2c14_board_info, ARRAY_SIZE(bigpony_i2c14_board_info));
    bigpony_add_i2c_devices(15, bigpony_i2c15_board_info, ARRAY_SIZE(bigpony_i2c15_board_info));
    bigpony_add_i2c_devices(16, bigpony_i2c16_board_info, ARRAY_SIZE(bigpony_i2c16_board_info));
#endif
    bigpony_add_i2c_devices(17, bigpony_i2c17_board_info, ARRAY_SIZE(bigpony_i2c17_board_info));

    printk(KERN_INFO "Cisco Meraki MS350 late init\n");

    return 0;
}
late_initcall(bigpony_lateinit);


MODULE_AUTHOR("Stephen Segal <stsegal@cisco.com>");
MODULE_DESCRIPTION("Cisco Meraki MS350 platform definition");
MODULE_LICENSE("GPL");
