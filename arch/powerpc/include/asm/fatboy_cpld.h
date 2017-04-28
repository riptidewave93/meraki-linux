/*
 * Meraki MS420 series platform header
 *
 * Copyright (C) 2013 Cisco Systems, Inc.
 *
 * This program is free software; you can redistribute  it and/or modify it
 * under  the terms of  the GNU General  Public License as published by the
 * Free Software Foundation;  either version 2 of the  License, or (at your
 * option) any later version.
 */

#ifndef FATBOY_CPLD_H
#define FATBOY_CPLD_H

enum fatboy_cpld_reg {
    FATBOY_CPLD_SYSTEM_MODEL           = 0x00,
    FATBOY_CPLD_PSU1_STATUS            = 0x01,
    FATBOY_CPLD_PSU2_STATUS            = 0x02,
    FATBOY_CPLD_SYSTEM_STATUS          = 0x03,
    /* 0x04 reserved */
    FATBOY_CPLD_SFP_INT_STATUS         = 0x05,
    FATBOY_CPLD_PHY_INT_STATUS_1       = 0x06,
    FATBOY_CPLD_PHY_INT_STATUS_2       = 0x07,
    FATBOY_CPLD_OTHERS_INT_STATUS      = 0x08,
    FATBOY_CPLD_CODE_REV               = 0x09,
    FATBOY_CPLD_VOLTAGE_MARGIN_CTRL_1  = 0x0A,
    FATBOY_CPLD_VOLTAGE_MARGIN_CTRL_2  = 0x0B,
    /* 0x0C reserved */
    /* 0x0D reserved */
    FATBOY_CPLD_WDOG_CTRL              = 0x0E,
    FATBOY_CPLD_POWER_CTRL             = 0x0F,
    FATBOY_CPLD_RESET_CTRL_1           = 0x10,
    FATBOY_CPLD_RESET_CTRL_2           = 0x11,
    FATBOY_CPLD_INT_MASK_CTRL          = 0x12,
    FATBOY_CPLD_SYS_LED_CTRL           = 0x13,
    FATBOY_CPLD_MISC_CTRL              = 0x14,
    /* 0x15 reserved */
};


/* Watchdog stuff */

/* Write-only bit. Write 1 to ping the watchdog */
#define FATBOY_CPLD_WDOG_CTRL_KICK_MASK    0x01
/* Read-write bit. Set to 1 to enable the watchdog, 0 to disable */
#define FATBOY_CPLD_WDOG_CTRL_ENABLE_MASK  0x02
/* Read-write bits. Value is related to the programmed timeout of the
   watchdog. Refer to the driver for valid values. */
#define FATBOY_CPLD_WDOG_CTRL_TIMEOUT_MASK 0x3C
/* Read-once bit. Value indicates whether or not the watchdog
   fired. Clears on read. */
#define FATBOY_CPLD_WDOG_CTRL_FIRED_MASK   0x80

#define FATBOY_CPLD_WDOG_CTRL_KICK_SET(reg_data, new_val)       \
    ((reg_data) = (((reg_data) & ~FATBOY_CPLD_WDOG_CTRL_KICK_MASK) |    \
                   ((new_val) & FATBOY_CPLD_WDOG_CTRL_KICK_MASK)))

#define FATBOY_CPLD_WDOG_CTRL_ENABLE_GET(reg_data) \
    (((reg_data) & FATBOY_CPLD_WDOG_CTRL_ENABLE_MASK) >> 1)

#define FATBOY_CPLD_WDOG_CTRL_ENABLE_SET(reg_data, new_val)     \
    ((reg_data) = (((reg_data) & ~FATBOY_CPLD_WDOG_CTRL_ENABLE_MASK) |  \
                   (((new_val) << 1) & FATBOY_CPLD_WDOG_CTRL_ENABLE_MASK)))

#define FATBOY_CPLD_WDOG_CTRL_TIMEOUT_GET(reg_data) \
    (((reg_data) & FATBOY_CPLD_WDOG_CTRL_TIMEOUT_MASK) >> 2)

#define FATBOY_CPLD_WDOG_CTRL_TIMEOUT_SET(reg_data, new_val) \
    ((reg_data) = (((reg_data) & ~FATBOY_CPLD_WDOG_CTRL_TIMEOUT_MASK) | \
                   (((new_val) << 2) & FATBOY_CPLD_WDOG_CTRL_TIMEOUT_MASK)))
#define FATBOY_CPLD_WDOG_CTRL_FIRED(reg_data) \
    (((reg_data) & FATBOY_CPLD_WDOG_CTRL_FIRED_MASK) >> 7)

extern int fatboy_cpld_read(enum fatboy_cpld_reg reg, u8* data);
extern int fatboy_cpld_write_mask(enum fatboy_cpld_reg reg, u8 data, u8 mask);
extern int fatboy_cpld_write(enum fatboy_cpld_reg reg, u8 data);

#endif /* FATBOY_CPLD_H */
