/*
 *  Cisco Meraki MS420 PSU driver
 *
 *  Copyright (C) 2013 Cisco Systems, Inc.
 *
 *  This program is free software; you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation; either version 2 of the License, or
 *  (at your option) any later version.
 *
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with this program; if not, write to the Free Software
 *  Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA.
 */

#include <linux/module.h>
#include <linux/init.h>
#include <linux/power_supply.h>
#include <linux/i2c.h>
#include <linux/of.h>
#include <linux/of_i2c.h>
#include <linux/workqueue.h>
#include <linux/power_supply.h>
#include <linux/sysfs.h>
#include <asm/fatboy_cpld.h>

#define MS420_PSU_PMBUS_ADDR_OFFSET   4
#define MS420_PSU_POLL_DELAY          2000  // milliseconds

#define MS420_EEPROM_FIELD_MAX_LEN    25
#define MS420_EEPROM_SIZE             256
#define MS420_EEPROM_READ_SIZE        32

struct ms420_psu_data {
    uint32_t index;
    struct i2c_client* eeprom_client;
    struct i2c_client* pmbus_client;
    struct delayed_work poll_work;
    struct power_supply psy;
    bool present;
    bool power_good;
    uint8_t eeprom_data[MS420_EEPROM_SIZE];
    char manufacturer[MS420_EEPROM_FIELD_MAX_LEN];
    char model_name[MS420_EEPROM_FIELD_MAX_LEN];
    char serial[MS420_EEPROM_FIELD_MAX_LEN];
};


static ssize_t eeprom_read(struct file* file, struct kobject* kobj,
                           struct bin_attribute* bin_attr,
                           char* buf, loff_t off, size_t count)
{
    struct i2c_client* client = to_i2c_client(container_of(kobj, struct device, kobj));
    struct ms420_psu_data* data = i2c_get_clientdata(client);

    if (off > sizeof(data->eeprom_data))
        return 0;
    if (off + count > sizeof(data->eeprom_data))
        count = sizeof(data->eeprom_data) - off;

    memcpy(buf, data->eeprom_data + off, count);

    return count;
}

static bool ms420_psu_verify_checksum(uint8_t* buf, int count)
{
    int i;
    uint8_t sum = 0;

    for (i = 0; i < count; i++)
        sum += buf[i];

    return !sum;
}


static void ms420_psu_clear_eeprom_cache(struct ms420_psu_data* data)
{
    memset(data->eeprom_data, 0, sizeof(data->eeprom_data));
    memset(data->manufacturer, 0, sizeof(data->manufacturer));
    memset(data->model_name, 0, sizeof(data->model_name));
    memset(data->serial, 0, sizeof(data->serial));
}

static int ms420_psu_get_next_eeprom_field(uint8_t* buf, int idx, char* contents)
{
    int len, next_idx;

    if (buf[idx] == 0xC1)
        return 0;
    len = buf[idx] & 0x3F;
    next_idx = idx + len + 1;
    if (next_idx > MS420_EEPROM_SIZE)
        return -EINVAL;

    if (contents)
        memcpy(contents,
               buf + idx + 1,
               (len < (MS420_EEPROM_FIELD_MAX_LEN-1)) ? len : MS420_EEPROM_FIELD_MAX_LEN-1);

    return next_idx;
}

static int ms420_psu_read_eeprom(struct ms420_psu_data* data)
{
    int idx = 0;
    int prod_data;
    int len;
    int32_t ret;

    ms420_psu_clear_eeprom_cache(data);

    while (idx < MS420_EEPROM_SIZE) {
        len = (MS420_EEPROM_READ_SIZE < (MS420_EEPROM_SIZE - idx) ?
               MS420_EEPROM_READ_SIZE : (MS420_EEPROM_SIZE - idx));
        ret = i2c_smbus_read_i2c_block_data(data->eeprom_client, idx, len, data->eeprom_data + idx);
        if (ret < 0) {
            dev_err(&data->eeprom_client->dev, "EEPROM I2C read failed with error %d\n", ret);
            return -EIO;
        }
        idx += ret;

        if (ret < len)
            break;
    }

    if (!ms420_psu_verify_checksum(data->eeprom_data, 8)) {
        dev_err(&data->eeprom_client->dev, "Common header checksum bad\n");
        return -EINVAL;
    }

    if ((data->eeprom_data[0] & 0x0F) != 1) {
        dev_err(&data->eeprom_client->dev, "Invalid spec version %d\n", data->eeprom_data[0] & 0x0F);
        return -EINVAL;
    }

    prod_data = (data->eeprom_data[4] * 8);
    if (prod_data + 1 > MS420_EEPROM_READ_SIZE) {
        dev_err(&data->eeprom_client->dev, "Product info area offset %d invalid\n", prod_data);
        return -EINVAL;
    }
    len = data->eeprom_data[prod_data + 1] * 8;

    if (len > (MS420_EEPROM_SIZE - prod_data)) {
        dev_err(&data->eeprom_client->dev, "Product info area too big (%d)\n", len);
        return -EINVAL;
    }

    if (!ms420_psu_verify_checksum(data->eeprom_data + prod_data, len)) {
        // Some CompuWare PSUs have an incorrect length, so try adding an additional 8.
        len += 8;
        if (len > (MS420_EEPROM_SIZE - prod_data)) {
            dev_err(&data->eeprom_client->dev, "Product info area checksum bad\n");
            return -EINVAL;
        }
        if (!ms420_psu_verify_checksum(data->eeprom_data + prod_data, len)) {
            dev_err(&data->eeprom_client->dev, "Product info area checksum bad\n");
            return -EINVAL;
        }
    }

    if ((data->eeprom_data[prod_data] & 0x0F) != 1) {
        dev_err(&data->eeprom_client->dev, "Product info spec version %d invalid\n", data->eeprom_data[prod_data] & 0x0F);
        return -EINVAL;
    }

    idx = ms420_psu_get_next_eeprom_field(data->eeprom_data, prod_data + 3, data->manufacturer);
    if (idx == 0)
        return 0;
    else if (idx < 0) {
        dev_err(&data->eeprom_client->dev, "Unable to parse product manufacturer field\n");
        return idx;
    }

    idx = ms420_psu_get_next_eeprom_field(data->eeprom_data, idx, data->model_name);
    if (idx == 0)
        return 0;
    else if (idx < 0) {
        dev_err(&data->eeprom_client->dev, "Unable to parse product name field\n");
        return idx;
    }

    idx = ms420_psu_get_next_eeprom_field(data->eeprom_data, idx, NULL);
    if (idx == 0)
        return 0;
    else if (idx < 0) {
        dev_err(&data->eeprom_client->dev, "Unable to parse product model field\n");
        return idx;
    }

    idx = ms420_psu_get_next_eeprom_field(data->eeprom_data, idx, NULL);
    if (idx == 0)
        return 0;
    else if (idx < 0) {
        dev_err(&data->eeprom_client->dev, "Unable to parse product version field\n");
        return idx;
    }

    idx = ms420_psu_get_next_eeprom_field(data->eeprom_data, idx, data->serial);
    if (idx == 0)
        return 0;
    else if (idx < 0) {
        dev_err(&data->eeprom_client->dev, "Unable to parse product version field\n");
        return idx;
    }

    return 0;
}

static void ms420_psu_poll(struct work_struct* work)
{
    struct delayed_work* delayed_work = to_delayed_work(work);
    struct ms420_psu_data* data = container_of(delayed_work,
                                               struct ms420_psu_data,
                                               poll_work);
    uint8_t val;
    enum fatboy_cpld_reg reg;
    int ret;
    struct i2c_board_info pmbus_i2c_info = {
        I2C_BOARD_INFO("pmbus", data->eeprom_client->addr + MS420_PSU_PMBUS_ADDR_OFFSET)
    };

    reg = (data->index == 1 ? FATBOY_CPLD_PSU1_STATUS : FATBOY_CPLD_PSU2_STATUS);
    ret = fatboy_cpld_read(reg, &val);
    if (ret < 0) {
        dev_err(&data->eeprom_client->dev, "Unable to read PSU %d status",
                data->index);
        goto done;
    }

    data->present = ~val & 0x01;
    data->power_good = val & 0x02;

    if (data->present && !data->pmbus_client) {
        if (ms420_psu_read_eeprom(data) < 0) {
            dev_err(&data->eeprom_client->dev, "Failed to read EEPROM on PSU %d\n", data->index);
            goto done;
        }
        data->pmbus_client = i2c_new_device(data->eeprom_client->adapter, &pmbus_i2c_info);
        if (data->pmbus_client) {
            dev_info(&data->eeprom_client->dev, "PSU %d inserted\n", data->index);
            ret = sysfs_create_link(&data->eeprom_client->dev.kobj, &data->pmbus_client->dev.kobj, "pmbus");
            if (ret < 0)
                dev_warn(&data->eeprom_client->dev, "Failed to create sysfs link to pmbus driver with error %d\n", ret);
        } else
            dev_err(&data->eeprom_client->dev, "Failed to initialize PMBus for PSU %d\n", data->index);
    } else if (!data->present && data->pmbus_client) {
        ms420_psu_clear_eeprom_cache(data);
        sysfs_remove_link(&data->eeprom_client->dev.kobj, "pmbus");
        i2c_unregister_device(data->pmbus_client);
        data->pmbus_client = NULL;
    }

done:
    schedule_delayed_work(delayed_work, msecs_to_jiffies(MS420_PSU_POLL_DELAY));
}


static int ms420_psu_get_property(struct power_supply *psy,
                                  enum power_supply_property psp,
                                  union power_supply_propval *val)
{
    struct ms420_psu_data* data = container_of(psy, struct ms420_psu_data, psy);

    switch (psp) {
    case POWER_SUPPLY_PROP_PRESENT:
        val->intval = data->present;
        break;
    case POWER_SUPPLY_PROP_ONLINE:
        val->intval = data->power_good;
        break;
    case POWER_SUPPLY_PROP_MANUFACTURER:
        val->strval = data->manufacturer;
        break;
    case POWER_SUPPLY_PROP_MODEL_NAME:
        val->strval = data->model_name;
        break;
    case POWER_SUPPLY_PROP_SERIAL_NUMBER:
        val->strval = data->serial;
        break;
    default:
        return -ENOSYS;
    };

    return 0;
}

static void ms420_psu_external_power_changed(struct power_supply *psy)
{
    return;
}

static void ms420_psu_set_charged(struct power_supply *psy)
{
    return;
}

static struct bin_attribute eeprom_attr = {
    .attr = {
        .name = "eeprom",
        .mode = 0444,
    },
    .size = MS420_EEPROM_SIZE,
    .read = eeprom_read
};

static enum power_supply_property ms420_psu_props[] = {
    POWER_SUPPLY_PROP_PRESENT,
    POWER_SUPPLY_PROP_ONLINE,
    POWER_SUPPLY_PROP_MODEL_NAME,
    POWER_SUPPLY_PROP_MANUFACTURER,
    POWER_SUPPLY_PROP_SERIAL_NUMBER
};


static int __devinit ms420_psu_probe(struct i2c_client* client,
                                      const struct i2c_device_id* id)
{
    struct ms420_psu_data* data;
    int ret;

    data = devm_kzalloc(&client->dev, sizeof(struct ms420_psu_data),
                        GFP_KERNEL);
    if (!data)
        return -ENOMEM;

    if (of_property_read_u32_array(client->dev.of_node, "index", &data->index, 1) < 0) {
        dev_err(&client->dev, "Unable to find index property\n");
        return -EINVAL;
    }

    if (data->index != 1 && data->index != 2) {
        dev_err(&client->dev, "index must be 1 or 2\n");
        return -EINVAL;
    }

    data->psy.name = devm_kzalloc(&client->dev, sizeof(char) * 20, GFP_KERNEL);
    if (!data->psy.name)
        return -ENOMEM;

    sprintf((char*)data->psy.name, "ms420-psu.%u", data->index);
    data->psy.type = POWER_SUPPLY_TYPE_MAINS;
    data->psy.properties = ms420_psu_props;
    data->psy.num_properties = ARRAY_SIZE(ms420_psu_props);
    data->psy.supplied_to = NULL;
    data->psy.num_supplicants = 0;
    data->psy.get_property = ms420_psu_get_property;
    data->psy.external_power_changed = ms420_psu_external_power_changed;
    data->psy.set_charged = ms420_psu_set_charged;
    data->psy.use_for_apm = 0;

    ret = power_supply_register(&client->dev, &data->psy);
    if (ret < 0)
        return ret;

    ret = device_create_bin_file(&client->dev, &eeprom_attr);
    if (ret < 0) {
        power_supply_unregister(&data->psy);
        return ret;
    }

    data->eeprom_client = client;

    i2c_set_clientdata(client, data);
    INIT_DELAYED_WORK(&data->poll_work, ms420_psu_poll);
    ms420_psu_poll(&data->poll_work.work);

    dev_info(&client->dev, "PSU %d driver initialized\n", data->index);

    return 0;
}

static int ms420_psu_remove(struct i2c_client* client)
{
    struct ms420_psu_data* data = i2c_get_clientdata(client);

    cancel_delayed_work(&data->poll_work);
    power_supply_unregister(&data->psy);
    i2c_unregister_device(data->pmbus_client);
    i2c_set_clientdata(client, NULL);

    return 0;
}

static struct i2c_device_id ms420_psu_id[] = {
    { "ms420-psu", 0 },
    {}
};

static struct i2c_driver ms420_psu_driver = {
    .driver = {
        .name = "ms420-psu",
        .owner = THIS_MODULE,
    },
    .probe = ms420_psu_probe,
    .remove = ms420_psu_remove,
    .id_table = ms420_psu_id,
};

module_i2c_driver(ms420_psu_driver);

MODULE_AUTHOR("Stephen Segal <stsegal@cisco.com>");
MODULE_DESCRIPTION("Cisco Meraki MS420 PSU driver");
MODULE_LICENSE("GPL");
