/*
 * LED driver for WS2801
 *
 * Kevin Paul Herbert
 * Copyright (c) 2012, Meraki, Inc.
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License version 2 as
 * published by the Free Software Foundation.
 *
 */

#include <linux/kernel.h>
#include <linux/init.h>
#include <linux/platform_device.h>
#include <linux/leds.h>
#include <linux/workqueue.h>
#include <linux/delay.h>
#include <linux/leds-ws2801.h>

#include <asm/gpio.h>

#define MAX_NAME_LENGTH 24
#define NUM_COLORS 3

static const char *led_ws2801_colors[] = { "red", "green", "blue" };

struct led_ws2801_led_data {
	struct led_classdev cdev;
	struct led_ws2801_data *controller;
	enum led_brightness level;
	char name[MAX_NAME_LENGTH];
};

struct led_ws2801_data {
	unsigned cki;
	unsigned sdi;
	struct delayed_work work;
	struct led_ws2801_led_data *led_chain;
	int num_leds;
	const char *name;
	u32 ndelay;
	atomic_t pending;
};

static void led_ws2801_work(struct work_struct *work)
{
	struct led_ws2801_data	*controller =
		container_of(work, struct led_ws2801_data, work.work);
	struct led_ws2801_led_data *led;
	u8 bit;
	int index;

	for (index = 0; index < controller->num_leds; index++) {
		led = &controller->led_chain[index];
		for (bit = 0x80; bit; bit = bit >> 1) {
			gpio_set_value(controller->sdi,
				       (led->level & bit) != 0);
			local_irq_enable();
			gpio_set_value(controller->cki, 1);
			ndelay(controller->ndelay);
			local_irq_disable();
			gpio_set_value(controller->cki, 0);
			ndelay(controller->ndelay);
		}
	}
	local_irq_enable();
	atomic_set(&controller->pending, 1);
}

static void led_ws2801_set(struct led_classdev *led_cdev,
			   enum led_brightness value)
{
	struct led_ws2801_led_data *led_dat =
		container_of(led_cdev, struct led_ws2801_led_data, cdev);
	struct led_ws2801_data *controller = led_dat->controller;

	if (led_dat->level != value) {
		led_dat->level = value;
		if (atomic_dec_and_test(&controller->pending))
			schedule_delayed_work(&led_dat->controller->work,
					      (HZ/1000) + 1);
	}
}

static int __devinit led_ws2801_create(struct led_ws2801_data *controller,
				       struct device *parent,
				       int index,
				       enum led_brightness brightness,
				       const char *color)
{
	struct led_ws2801_led_data *led = &controller->led_chain[index];
	int ret;

	scnprintf(led->name, sizeof(led->name), "%s%d:%s",
		  controller->name, index / NUM_COLORS, color);
	led->cdev.name = led->name;
	led->cdev.brightness_set = led_ws2801_set;
	led->level = brightness;
	led->controller = controller;
	ret = led_classdev_register(parent, &led->cdev);
	if (ret < 0)
		goto err;

	return 0;

err:
	kfree(led);
	return ret;
}


static int __devinit
led_ws2801_create_chain(const struct led_ws2801_template *template,
			struct led_ws2801_data *controller,
			struct device *parent)
{
	int ret;
	int index;

	controller->cki = template->cki;
	controller->sdi = template->sdi;
	controller->num_leds = template->num_leds * 3;
	controller->name = template->name;
	controller->ndelay = template->ndelay;
	atomic_set(&controller->pending, 1);

	controller->led_chain = kzalloc(sizeof(struct led_ws2801_led_data) *
					controller->num_leds, GFP_KERNEL);

	if (!controller->led_chain) {
		return -ENOMEM;
	}

	ret = gpio_request(controller->cki, template->name);
	if (ret < 0)
		goto err_free_chain;

	ret = gpio_request(controller->sdi, template->name);
	if (ret < 0)
		goto err_ret_cki;

	ret = gpio_direction_output(controller->cki, 0);
	if (ret < 0)
		goto err_ret_sdi;

	ret = gpio_direction_output(controller->sdi, 0);
	if (ret < 0)
		goto err_ret_sdi;

	for (index = 0; index < controller->num_leds; index++) {
		ret = led_ws2801_create(controller, parent, index,
					template->init_brightness
					[index % NUM_COLORS],
					led_ws2801_colors[index % NUM_COLORS]);
		if (ret < 0)
			goto err_ret_sdi;
	}

	INIT_DELAYED_WORK(&controller->work, led_ws2801_work);
	schedule_delayed_work(&controller->work, 0);

	return 0;

err_ret_sdi:
	gpio_free(controller->sdi);
err_ret_cki:
	gpio_free(controller->cki);
err_free_chain:
	kfree(controller->led_chain);

	return ret;
}

static void led_ws2801_delete_chain(struct led_ws2801_data *controller)
{
	struct led_ws2801_led_data *led_chain;
	struct led_ws2801_led_data *led;
	int index;
	int num_leds;

	led_chain = controller->led_chain;
	controller->led_chain = 0;
	num_leds = controller->num_leds;
	controller->num_leds = 0;
	cancel_delayed_work_sync(&controller->work);

	for (index = 0; index < controller->num_leds; index++) {
		led = &led_chain[index];
		led_classdev_unregister(&led->cdev);
	}

	gpio_free(controller->cki);
	gpio_free(controller->sdi);

	kfree(led_chain);
}

static int __devinit led_ws2801_probe(struct platform_device *pdev)
{
	struct led_ws2801_platform_data *pdata = pdev->dev.platform_data;
	struct led_ws2801_data *controllers;
	int i, ret = 0;

	if (!pdata)
		return -EBUSY;

	controllers = kzalloc(sizeof(struct led_ws2801_data) *
			      pdata->num_controllers, GFP_KERNEL);
	if (!controllers)
		return -ENOMEM;

	for (i = 0; i < pdata->num_controllers; i++) {
		ret = led_ws2801_create_chain(&pdata->template[i],
					      &controllers[i],
					      &pdev->dev);
		if (ret < 0)
			goto err;
	}

	platform_set_drvdata(pdev, controllers);

	return 0;

err:
	for (i = i - 1; i >= 0; i--)
		led_ws2801_delete_chain(&controllers[i]);

	kfree(controllers);

	return ret;
}

static int __devexit led_ws2801_remove(struct platform_device *pdev)
{
	int i;
	struct led_ws2801_platform_data *pdata = pdev->dev.platform_data;
	struct led_ws2801_data *controllers;

	controllers = platform_get_drvdata(pdev);

	for (i = 0; i < pdata->num_controllers; i++)
		led_ws2801_delete_chain(&controllers[i]);

	kfree(controllers);

	return 0;
}

static struct platform_driver led_ws2801_driver = {
	.probe		= led_ws2801_probe,
	.remove		= __devexit_p(led_ws2801_remove),
	.driver		= {
		.name	= "leds-ws2801",
		.owner	= THIS_MODULE,
	},
};

static int __init led_ws2801_init(void)
{
	return platform_driver_register(&led_ws2801_driver);
}

static void __exit led_ws2801_exit(void)
{
	platform_driver_unregister(&led_ws2801_driver);
}

module_init(led_ws2801_init);
module_exit(led_ws2801_exit);

MODULE_AUTHOR("Kevin Paul Herbert <kph@meraki.net>")
MODULE_DESCRIPTION("WS2801 LED driver");
MODULE_LICENSE("GPL");
MODULE_ALIAS("platforms:leds-ws2801");
