#include <linux/kernel.h>
#include <linux/module.h>
#include <linux/platform_device.h>
#include <linux/sysfs.h>

#include <asm/gpio.h>

#include "xlna.h"
#include "atheros.h"

#define GPIO0_XLNA_ENABLE 0x2e
#define GPIO1_XLNA_ENABLE 0x2f

#define GPIO_OFFSET(gpio) (gpio - ATHEROS_GPIO_BASE)
#define GPIO_FUNC_REG_OFFSET(offset) (ATH_GPIO_OUT_FUNCTION0 + ((offset / 4) * 4)) //round down to nearest four bytes
#define GPIO_FUNC_REG_SHIFT(data, offset) (data << ((offset % 4) * 8))

static int
xlna_enable(struct device *dev)
{
    struct xlna_platform_data *data = dev->platform_data;
    int req;

    if (data->enabled)
        return 0;

    if (GPIO_OFFSET(data->gpio0) >= ATHEROS_NUM_GPIOS) {
        printk("gpio %d is not a valid on-board gpio for the xlna\n", data->gpio0);
        return -EINVAL;
    }

    if (GPIO_OFFSET(data->gpio1) >= ATHEROS_NUM_GPIOS) {
        printk("gpio %d is not a valid on-board gpio for the xlna\n", data->gpio1);
        return -EINVAL;
    }

    req = gpio_request(data->gpio0, "xlna-gpio0");
    if (req < 0)
        goto errout_0;

    req = gpio_request(data->gpio1, "xlna-gpio1");
    if (req < 0)
        goto errout_1;

    req = gpio_direction_output(data->gpio0, 0);
    if (req < 0)
        goto errout_dir;

    req = gpio_direction_output(data->gpio1, 0);
    if (req < 0)
        goto errout_dir;

    ath_reg_rmw_clear(GPIO_FUNC_REG_OFFSET(GPIO_OFFSET(data->gpio0)), GPIO_FUNC_REG_SHIFT(0xff, GPIO_OFFSET(data->gpio0)));
    ath_reg_rmw_clear(GPIO_FUNC_REG_OFFSET(GPIO_OFFSET(data->gpio1)), GPIO_FUNC_REG_SHIFT(0xff, GPIO_OFFSET(data->gpio1)));

    ath_reg_rmw_set(GPIO_FUNC_REG_OFFSET(GPIO_OFFSET(data->gpio0)), GPIO_FUNC_REG_SHIFT(GPIO0_XLNA_ENABLE, GPIO_OFFSET(data->gpio0)));
    ath_reg_rmw_set(GPIO_FUNC_REG_OFFSET(GPIO_OFFSET(data->gpio1)), GPIO_FUNC_REG_SHIFT(GPIO1_XLNA_ENABLE, GPIO_OFFSET(data->gpio1)));

    data->enabled = 1;

    return 0;

 errout_dir:
    gpio_free(data->gpio0);
 errout_0:
    gpio_free(data->gpio1);
 errout_1:
    printk("failed to enable xlna. One or both gpios %d/%d are already reserved\n", data->gpio0, data->gpio1);

    data->enabled = 0;

    return req;
}

static int
xlna_disable(struct device *dev)
{
    struct xlna_platform_data *data = dev->platform_data;

    ath_reg_rmw_clear(GPIO_FUNC_REG_OFFSET(GPIO_OFFSET(data->gpio0)), GPIO_FUNC_REG_SHIFT(0xff, GPIO_OFFSET(data->gpio0)));
    ath_reg_rmw_clear(GPIO_FUNC_REG_OFFSET(GPIO_OFFSET(data->gpio1)), GPIO_FUNC_REG_SHIFT(0xff, GPIO_OFFSET(data->gpio1)));

    gpio_free(data->gpio0);
    gpio_free(data->gpio1);

    data->enabled = 0;

    return 0;
}

static ssize_t
xlna_enabled_read(struct device *dev, struct device_attribute *attr, char *buf)
{
    struct xlna_platform_data *data = dev->platform_data;

    return scnprintf(buf, PAGE_SIZE, "%d\n", data->enabled);
}

static ssize_t
xlna_enabled_write(struct device *dev, struct device_attribute *attr, const char *buf, size_t count)
{
    struct xlna_platform_data *data = dev->platform_data;

    if (count < 1)
        return 0;

    if (buf[0] == '1' && !data->enabled)
        xlna_enable(dev);
    else if (buf[0] == '0' && data->enabled)
        xlna_disable(dev);

    return 1;
}

static DEVICE_ATTR(enabled, S_IWUSR | S_IRUGO, xlna_enabled_read, xlna_enabled_write);

static int __devinit xlna_probe(struct platform_device *pdev)
{
    xlna_enable(&pdev->dev);

    return sysfs_create_file(&pdev->dev.kobj, &dev_attr_enabled.attr);
}

static int __devexit xlna_remove(struct platform_device *pdev)
{
    xlna_disable(&pdev->dev);

    sysfs_remove_file(&pdev->dev.kobj, &dev_attr_enabled.attr);

    return 0;
}

static struct platform_driver xlna_driver = {
    .probe = xlna_probe,
    .remove = __devexit_p(xlna_remove),
    .driver = {
        .name  = "ath-xlna",
        .owner = THIS_MODULE,
    },
};

static int __init xlna_init(void)
{
    return platform_driver_register(&xlna_driver);
}

static void __exit xlna_exit(void)
{
    platform_driver_unregister(&xlna_driver);
}

module_init(xlna_init);
module_exit(xlna_exit);
