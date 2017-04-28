/*
 *  Meraki small form-factor pluggable (SFP/SFP+) transceiver socket driver
 *
 *  Copyright (C) 2013 Cisco Systems, Inc.
 *
 *  This program is free software; you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation; version 2 of the License.
 */

#include <linux/module.h>
#include <linux/init.h>
#include <linux/gpio.h>
#include <linux/interrupt.h>
#include <linux/of.h>
#include <linux/of_gpio.h>
#include <linux/idr.h>
#include <linux/i2c.h>
#include <linux/meraki/sfp.h>
#include <linux/workqueue.h>
#include <linux/slab.h>
#include <asm/cmpxchg.h>

#define SFP_MAX_COUNT    48
#define SFP_RETRY_DELAY  1000  // milliseconds
enum {
    SFP_GPIO_RS0 = 0,
    SFP_GPIO_RS1 = 1,
    SFP_GPIO_ABS = 2,
    SFP_GPIO_LOS = 3,

    SFP_GPIO_COUNT
};

static const char* sfp_gpio_label_templates[SFP_GPIO_COUNT] = {
    "sfp%u_rs0",
    "sfp%u_rs1",
    "sfp%u_abs",
    "sfp%u_los",
};

static struct class sfp_xcvr_class = {
    .name = "sfp"
};

struct sfp_dev
{
    struct i2c_client* i2c;
    unsigned int port;
    int gpios[SFP_GPIO_COUNT];
    char sfp_gpio_labels[SFP_GPIO_COUNT][15];
    int abs_irq;
    bool init_done;
    bool detect;
    uint8_t i2c_retries;
    char name[20];
    struct sfp_data eeprom;
    struct delayed_work retry_work;
    struct device* sfp_xcvr_dev;
};

static struct sfp_dev sfp_devs[SFP_MAX_COUNT];
static LIST_HEAD(sfp_notify_list);
static DEFINE_MUTEX(sfp_notify_mutex);
static DECLARE_RWSEM(sfp_eeprom_rwsem);


static ssize_t sfp_show_port(struct device *dev, struct device_attribute *attr, char *buf)
{
    struct i2c_client* client = to_i2c_client(dev);
    struct sfp_dev* sfp = i2c_get_clientdata(client);

    return scnprintf(buf, PAGE_SIZE, "%u\n", sfp->port);
}
static DEVICE_ATTR(port, 0444, sfp_show_port, NULL);

static ssize_t sfp_show_detect(struct device* dev, struct device_attribute* attr, char* buf)
{
    struct i2c_client* client = to_i2c_client(dev);
    struct sfp_dev* sfp = i2c_get_clientdata(client);

    return scnprintf(buf, PAGE_SIZE, "%u\n", (unsigned)sfp->detect);
}
static DEVICE_ATTR(detect, 0444, sfp_show_detect, NULL);

static ssize_t sfp_sysfs_eeprom_read(struct file* file, struct kobject* kobj,
                                     struct bin_attribute* bin_attr,
                                     char* buf, loff_t off, size_t count)
{
    struct sfp_dev* sfp = dev_get_drvdata(container_of(kobj, struct device, kobj));

    if (off > sizeof(sfp->eeprom))
        return 0;
    if (off + count > sizeof(sfp->eeprom))
        count = sizeof(sfp->eeprom) - off;

    memcpy(buf, ((uint8_t*)&sfp->eeprom) + off, count);

    return count;
}

int register_sfp_notify(struct sfp_notify* notify)
{
    int ret = 0;

    if (!notify)
        return -EINVAL;

    mutex_lock(&sfp_notify_mutex);
    if (!try_module_get(notify->owner))
        ret = -EINVAL;
    else
        list_add(&notify->list, &sfp_notify_list);
    mutex_unlock(&sfp_notify_mutex);

    return ret;
}
EXPORT_SYMBOL(register_sfp_notify);

int unregister_sfp_notify(struct sfp_notify* notify)
{
    int ret = -EINVAL;
    struct list_head* pos;

    if (!notify)
        return -EINVAL;

    mutex_lock(&sfp_notify_mutex);
    list_for_each(pos, &sfp_notify_list) {
        struct sfp_notify* curr = list_entry(pos, struct sfp_notify, list);
        if (curr == notify) {
            list_del(pos);
            module_put(curr->owner);
            ret = 0;
            break;
        }
    }
    mutex_unlock(&sfp_notify_mutex);

    return ret;
}
EXPORT_SYMBOL(unregister_sfp_notify);

int get_sfp_data(unsigned int port, struct sfp_data* data)
{
    int ret = 0;

    if (!data || port > SFP_MAX_COUNT || port == 0)
        return -EINVAL;

    if (sfp_devs[port-1].port == 0)
        return -ENODEV;

    down_read(&sfp_eeprom_rwsem);
    if (sfp_devs[port-1].sfp_xcvr_dev)
        memcpy(data, &sfp_devs[port-1].eeprom, sizeof(*data));
    else
        ret = -ENODEV;
    up_read(&sfp_eeprom_rwsem);

    return ret;
}
EXPORT_SYMBOL(get_sfp_data);

static int read_sfp_eeprom(struct sfp_dev* dev)
{
    uint8_t offset = 0;
    int ret;
    struct i2c_msg msgs[] = {
        { 0, 0, 1, &offset },
        { 0, I2C_M_RD, sizeof(struct sfp_data), (uint8_t*)&dev->eeprom }
    };

    msgs[0].addr = dev->i2c->addr;
    msgs[1].addr = dev->i2c->addr;

    ret = i2c_transfer(dev->i2c->adapter, msgs, 2);
    if (ret != 2) {
        dev_err(&dev->i2c->dev, "Unable to communicate with SFP on port %u\n", dev->port);
        return (ret < 0 ? ret : -EIO);
    }

    return 0;
}

static void send_notify(unsigned int port, bool insert)
{
    struct list_head* pos;

    mutex_lock(&sfp_notify_mutex);
    list_for_each(pos, &sfp_notify_list) {
        struct sfp_notify* notify = list_entry(pos, struct sfp_notify, list);

        if (insert) {
            if (notify->insert)
                notify->insert(port, notify->user_data);
        } else {
            if (notify->remove)
                notify->remove(port, notify->user_data);
        }
    }
    mutex_unlock(&sfp_notify_mutex);
}

static void sfp_device_release(struct device* dev)
{
    kfree(dev);
}

static struct device_type sfp_device_type = {
    .release = sfp_device_release,
};


static struct bin_attribute eeprom_attr = {
    .attr = {
        .name = "eeprom",
        .mode = 0444,
    },
    .size = sizeof(struct sfp_data),
    .read = sfp_sysfs_eeprom_read
};


static int create_sfp_xcvr_device(struct sfp_dev* dev)
{
    int ret = 0;

    dev->sfp_xcvr_dev = kzalloc(sizeof(*dev->sfp_xcvr_dev), GFP_KERNEL);
    if (!dev->sfp_xcvr_dev)
        return -ENOMEM;

    dev->sfp_xcvr_dev->class = &sfp_xcvr_class;
    dev->sfp_xcvr_dev->type = &sfp_device_type;
    dev->sfp_xcvr_dev->parent = &dev->i2c->dev;
    dev_set_name(dev->sfp_xcvr_dev, "sfp%u", dev->port);

    ret = device_register(dev->sfp_xcvr_dev);
    if (ret < 0) {
        dev_err(&dev->i2c->dev, "Unable to create device: error %d.\n", ret);
        kfree(dev->sfp_xcvr_dev);
        dev->sfp_xcvr_dev = NULL;
        return ret;
    }

    ret = device_create_bin_file(dev->sfp_xcvr_dev, &eeprom_attr);
    if (ret < 0) {
        dev_err(dev->sfp_xcvr_dev, "Unable to create eeprom bin file: error %d.\n", ret);
        device_unregister(dev->sfp_xcvr_dev);
        return ret;
    }

    dev_set_drvdata(dev->sfp_xcvr_dev, dev);

    return 0;
}

static void destroy_sfp_xcvr_device(struct sfp_dev* dev)
{
    device_remove_bin_file(dev->sfp_xcvr_dev, &eeprom_attr);
    device_unregister(dev->sfp_xcvr_dev);
    dev->sfp_xcvr_dev = NULL;
}

static void handle_insert(struct sfp_dev* dev)
{
    down_write(&sfp_eeprom_rwsem);
    if (read_sfp_eeprom(dev) == 0) {
        uint8_t csum = 0;
        uint8_t* p;

        for (p = (uint8_t*)&dev->eeprom;
             p < (uint8_t*)&dev->eeprom.base_cc;
             p++)
            csum += *p;

        if (csum == dev->eeprom.base_cc) {
            csum = 0;
            for (p = (uint8_t*)&dev->eeprom.base_cc + 1;
                 p < (uint8_t*)&dev->eeprom.ext_cc;
                 p++)
                csum += *p;

            if (csum == dev->eeprom.ext_cc)
                create_sfp_xcvr_device(dev);
        }
    }
    up_write(&sfp_eeprom_rwsem);

    if (dev->sfp_xcvr_dev) {
        send_notify(dev->port, true);
    } else if (++(dev->i2c_retries) < 2) {
        schedule_delayed_work(&dev->retry_work, msecs_to_jiffies(SFP_RETRY_DELAY));
    }
}

static void sfp_work(struct work_struct* work)
{
    struct delayed_work* delayed_work = to_delayed_work(work);
    struct sfp_dev* dev = container_of(delayed_work, struct sfp_dev, retry_work);

    // It's unlikely, but not impossible for dev->detect to be false when entering
    // this function.  If it does happen, it's because we got scheduled and started
    // to run before the removal interrupt handler canceled us.  In that case,
    // we do nothing.
    if (likely(dev->detect)) {
        handle_insert(dev);
    }
}

static irqreturn_t sfp_threaded_irq_handler(int irq, void* devid)
{
    struct sfp_dev* dev = (struct sfp_dev*)devid;

    while (!dev->init_done)
        cpu_relax();

    if (irq == dev->abs_irq) {
        bool detect = !(gpio_get_value_cansleep(dev->gpios[SFP_GPIO_ABS]));

        // Nothing changed...spurious interrupt.
        if (detect == dev->detect)
            return IRQ_NONE;

        dev->detect = detect;
        // Make sure there's no retry work pending for this port
        cancel_delayed_work_sync(&dev->retry_work);

        if (detect) {
            dev->i2c_retries = 0;
            handle_insert(dev);
        } else {
            // Only send a remove if we had originally send an insert.
            if (dev->sfp_xcvr_dev) {
                down_write(&sfp_eeprom_rwsem);
                destroy_sfp_xcvr_device(dev);
                up_write(&sfp_eeprom_rwsem);
                send_notify(dev->port, false);
            }
        }

        return IRQ_HANDLED;
    }

    return IRQ_NONE;
}
static struct device_attribute *sfp_attrs[] = {
    &dev_attr_port,
    &dev_attr_detect,
};

static int sfp_make_sysfs_nodes(struct device *dev)
{
    int ret, i;

    for (i = 0; i < ARRAY_SIZE(sfp_attrs); ++i) {
        ret = device_create_file(dev, sfp_attrs[i]);
        if (ret < 0)
            goto fail;
    }

    return 0;

fail:
    while (i > 0)
        device_remove_file(dev, sfp_attrs[--i]);
    return ret;
}

static void sfp_unmake_sysfs_nodes(struct device *dev)
{
    int i = ARRAY_SIZE(sfp_attrs);
    while (i > 0)
        device_remove_file(dev, sfp_attrs[--i]);

    device_remove_bin_file(dev, &eeprom_attr);
}


static int __devinit sfp_probe(struct i2c_client* client,
                               const struct i2c_device_id* id)
{
    int i;
    int ret;
    unsigned int port;

    if (of_property_read_u32_array(client->dev.of_node, "port", &port, 1) < 0) {
        dev_err(&client->dev, "Unable to find port property\n");
        return -EINVAL;
    }

    if (port > SFP_MAX_COUNT) {
        dev_err(&client->dev, "Port %u is too high\n", port);
        return -EINVAL;
    }

    if (cmpxchg(&sfp_devs[port-1].port, 0, port) != 0) {
        dev_err(&client->dev, "Port %u already assigned\n", port);
        return -EINVAL;
    }

    for (i = 0; i < SFP_GPIO_COUNT; i++) {
        sfp_devs[port-1].gpios[i] = of_get_gpio(client->dev.of_node, i);
        if (sfp_devs[port-1].gpios[i] < 0) {
            dev_err(&client->dev, "Unable to find gpio index %d for port %u\n", i, port);
            ret = -EINVAL;
            goto err;
        }
    }

    sfp_devs[port-1].abs_irq = of_irq_to_resource(client->dev.of_node, 0, NULL);
    if (sfp_devs[port-1].abs_irq < 0) {
        dev_err(&client->dev, "Unable to find ABS interrupt resource for port %u\n", port);
        ret = -EINVAL;
        goto err;
    }

    for (i = 0; i < SFP_GPIO_COUNT; i++) {
        sprintf(sfp_devs[port-1].sfp_gpio_labels[i], sfp_gpio_label_templates[i], port);
        ret = devm_gpio_request(&client->dev, (unsigned)(sfp_devs[port-1].gpios[i]),
                                sfp_devs[port-1].sfp_gpio_labels[i]);
        if (ret) {
            dev_err(&client->dev, "Unable to request gpio %d (%s)\n", sfp_devs[port-1].gpios[i],
                    sfp_devs[port-1].sfp_gpio_labels[i]);
            goto err;
        }

        ret = gpio_direction_input(sfp_devs[port-1].gpios[i]);
        if (ret) {
            dev_err(&client->dev, "Unable to set direction of gpio %d (%s)\n", sfp_devs[port-1].gpios[i],
                    sfp_devs[port-1].sfp_gpio_labels[i]);
            goto err;
        }
    }

    i2c_set_clientdata(client, &sfp_devs[port-1]);
    sfp_devs[port-1].i2c = client;
    INIT_DELAYED_WORK(&sfp_devs[port-1].retry_work, sfp_work);
    sfp_devs[port-1].i2c_retries = 0;
    sfp_devs[port-1].init_done = false;
    sprintf(sfp_devs[port-1].name, "sfp%u", port);

    if (sfp_make_sysfs_nodes(&client->dev) < 0) {
        dev_err(&client->dev, "Failed to create sysfs nodes\n");
        goto gpio_free_err;
    }

    ret = request_threaded_irq(sfp_devs[port-1].abs_irq,
                               NULL,
                               sfp_threaded_irq_handler,
                               IRQF_TRIGGER_RISING | IRQF_TRIGGER_FALLING | IRQF_ONESHOT,
                               sfp_devs[port-1].name,
                               &sfp_devs[port-1]);

    if (ret) {
        dev_err(&client->dev, "Failed to request irq %d for port %u]\n", sfp_devs[port-1].abs_irq, port);
        goto sysfs_free_err;
    }

    sfp_devs[port-1].detect = !gpio_get_value_cansleep(sfp_devs[port-1].gpios[SFP_GPIO_ABS]);

    if (sfp_devs[port-1].detect) {
        handle_insert(&sfp_devs[port-1]);
    }
    sfp_devs[port-1].init_done = true;

    return 0;

sysfs_free_err:
    sfp_unmake_sysfs_nodes(&client->dev);

gpio_free_err:
    memset(sfp_devs[port-1].gpios, 0, sizeof(sfp_devs[port-1].gpios));

    i2c_set_clientdata(client, NULL);
    sfp_devs[port-1].i2c = NULL;

err:
    xchg(&sfp_devs[port-1].port, 0);

    return ret;
}

static int sfp_remove(struct i2c_client* client)
{
    struct sfp_dev* dev = i2c_get_clientdata(client);


    free_irq(dev->abs_irq, dev);
    cancel_delayed_work_sync(&dev->retry_work);
    sfp_unmake_sysfs_nodes(&client->dev);
    memset(dev->gpios, 0, sizeof(dev->gpios));

    i2c_set_clientdata(client, NULL);
    dev->i2c = NULL;
    xchg(&dev->port, 0);

    return 0;
}

static const struct i2c_device_id sfp_id[] = {
    { "sfp-socket", 0 },
    {}
};

static struct i2c_driver sfp_driver = {
    .driver = {
        .name = "sfp-socket",
        .owner = THIS_MODULE,
    },
    .probe = sfp_probe,
    .remove = sfp_remove,
    .id_table = sfp_id,
};

static int __init sfp_init(void)
{
    int ret;

    memset(sfp_devs, 0, sizeof(sfp_devs));
    ret = class_register(&sfp_xcvr_class);
    if (ret < 0)
        return ret;

    return i2c_add_driver(&sfp_driver);
}
module_init(sfp_init);

static void __exit sfp_exit(void)
{
    i2c_del_driver(&sfp_driver);
    class_unregister(&sfp_xcvr_class);
}
module_exit(sfp_exit);

MODULE_AUTHOR("Stephen Segal <stsegal@cisco.com>");
MODULE_DESCRIPTION("Meraki SFP/SFP+ socket driver");
MODULE_LICENSE("GPL");
