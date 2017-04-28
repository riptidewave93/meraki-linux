/*
 * linux/drivers/leds-pwm.c
 *
 * simple PWM based LED control
 *
 * Copyright 2009 Luotao Fu @ Pengutronix (l.fu@pengutronix.de)
 *
 * based on leds-gpio.c by Raphael Assenat <raph@8d.com>
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License version 2 as
 * published by the Free Software Foundation.
 */

#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/init.h>
#include <linux/platform_device.h>
#include <linux/fb.h>
#include <linux/leds.h>
#include <linux/err.h>
#include <linux/pwm.h>
#include <linux/leds_pwm.h>
#include <linux/slab.h>

struct led_pwm_data {
	struct led_classdev	cdev;
	struct pwm_device	*pwm;
	unsigned int 		active_low;
	unsigned int		period;
};

static void led_pwm_set(struct led_classdev *led_cdev,
	enum led_brightness brightness)
{
	struct led_pwm_data *led_dat =
		container_of(led_cdev, struct led_pwm_data, cdev);
	unsigned int max = led_dat->cdev.max_brightness;
	unsigned int period =  led_dat->period;

	if (brightness == 0) {
		pwm_config(led_dat->pwm, 0, period);
		pwm_disable(led_dat->pwm);
	} else {
		pwm_config(led_dat->pwm, brightness * period / max, period);
		pwm_enable(led_dat->pwm);
	}
}

static int led_pwm_create_of(struct platform_device *pdev,
			     struct led_pwm_data *priv)
{
	struct device_node *child;
	int ret;
	int num_leds=0;

	for_each_child_of_node(pdev->dev.of_node, child) {
		struct led_pwm_data *led_dat = &priv[num_leds];

		led_dat->cdev.name = of_get_property(child, "label",
						     NULL) ? : child->name;

		led_dat->pwm = devm_of_pwm_get(&pdev->dev, child, NULL);
		if (IS_ERR(led_dat->pwm)) {
			dev_err(&pdev->dev, "unable to request PWM for %s\n",
				led_dat->cdev.name);
			ret = PTR_ERR(led_dat->pwm);
			goto err;
		}
		/* Get the period from PWM core */
		led_dat->period = pwm_get_period(led_dat->pwm);

		led_dat->cdev.default_trigger = of_get_property(child,
						"linux,default-trigger", NULL);
		of_property_read_u32(child, "max-brightness",
				     &led_dat->cdev.max_brightness);

		led_dat->cdev.brightness_set = led_pwm_set;
		led_dat->cdev.brightness = LED_OFF;
		led_dat->cdev.flags |= LED_CORE_SUSPENDRESUME;

		ret = led_classdev_register(&pdev->dev, &led_dat->cdev);
		if (ret < 0) {
			dev_err(&pdev->dev, "failed to register for %s\n",
				led_dat->cdev.name);
			of_node_put(child);
			goto err;
		}
		num_leds++;
	}

	return 0;
err:
	while (num_leds--) {
		struct led_pwm_data *led_dat = &priv[num_leds];
		led_classdev_unregister(&led_dat->cdev);
	}

	return ret;
}


static int led_pwm_probe(struct platform_device *pdev)
{
	struct led_pwm_platform_data *pdata = pdev->dev.platform_data;
	struct led_pwm *cur_led;
	struct led_pwm_data *leds_data, *led_dat;
	int i, ret = 0;
	int count;

	if (pdata)
		count = pdata->num_leds;
	else
		count = of_get_child_count(pdev->dev.of_node);

	if (!count)
		return -EINVAL;

	leds_data = kzalloc(sizeof(struct led_pwm_data) * count,
				GFP_KERNEL);
	if (!leds_data)
		return -ENOMEM;

	if (pdata) {
	for (i = 0; i < pdata->num_leds; i++) {
		cur_led = &pdata->leds[i];
		led_dat = &leds_data[i];

		led_dat->pwm = pwm_request(cur_led->pwm_id,
				cur_led->name);
		if (IS_ERR(led_dat->pwm)) {
			ret = PTR_ERR(led_dat->pwm);
			dev_err(&pdev->dev, "unable to request PWM %d\n",
					cur_led->pwm_id);
			goto err;
		}

		led_dat->cdev.name = cur_led->name;
		led_dat->cdev.default_trigger = cur_led->default_trigger;
		led_dat->active_low = cur_led->active_low;
		led_dat->period = cur_led->pwm_period_ns;
		led_dat->cdev.brightness_set = led_pwm_set;
		led_dat->cdev.brightness = LED_OFF;
		led_dat->cdev.max_brightness = cur_led->max_brightness;
		led_dat->cdev.flags |= LED_CORE_SUSPENDRESUME;

		ret = led_classdev_register(&pdev->dev, &led_dat->cdev);
		if (ret < 0) {
			pwm_free(led_dat->pwm);
			goto err;
		}
	}
	} else {
		ret = led_pwm_create_of(pdev, leds_data);
		if (ret) {
			i=0;
			goto err;
		}
	}

	platform_set_drvdata(pdev, leds_data);

	return 0;

err:
	if (i > 0) {
		for (i = i - 1; i >= 0; i--) {
			led_classdev_unregister(&leds_data[i].cdev);
			pwm_free(leds_data[i].pwm);
		}
	}

	kfree(leds_data);

	return ret;
}

static int __devexit led_pwm_remove(struct platform_device *pdev)
{
	int i;
	struct led_pwm_platform_data *pdata = pdev->dev.platform_data;
	struct led_pwm_data *leds_data;

	leds_data = platform_get_drvdata(pdev);

	for (i = 0; i < pdata->num_leds; i++) {
		led_classdev_unregister(&leds_data[i].cdev);
		pwm_free(leds_data[i].pwm);
	}

	kfree(leds_data);

	return 0;
}

static const struct of_device_id of_pwm_leds_match[] = {
	{ .compatible = "pwm-leds", },
	{},
};
MODULE_DEVICE_TABLE(of, of_pwm_leds_match);

static struct platform_driver led_pwm_driver = {
	.probe		= led_pwm_probe,
	.remove		= __devexit_p(led_pwm_remove),
	.driver		= {
		.name	= "leds_pwm",
		.owner	= THIS_MODULE,
		.of_match_table = of_pwm_leds_match,
	},
};

module_platform_driver(led_pwm_driver);

MODULE_AUTHOR("Luotao Fu <l.fu@pengutronix.de>");
MODULE_DESCRIPTION("PWM LED driver for PXA");
MODULE_LICENSE("GPL");
MODULE_ALIAS("platform:leds-pwm");
