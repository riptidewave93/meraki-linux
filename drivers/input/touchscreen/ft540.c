/*
 * drivers/input/touchscreen/ft540.c
 *
 * Copyright (C) 2004-2012, Ambarella, Inc.
 *	Zhenwu Xue <zwxue@ambarella.com>
 *
 *
 *  This program is free software; you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License version 2 as
 *  published by the Free Software Foundation.
 */

#include <linux/module.h>
#include <linux/slab.h>
#include <linux/delay.h>
#include <linux/input.h>
#include <linux/interrupt.h>
#include <linux/i2c.h>
#include <linux/i2c/ft540.h>
#include <asm/io.h>

#ifdef	CONFIG_DEBUG_TOUCHSCREEN
#define FT_DEBUG(format, arg...)	printk(format , ## arg)
#else
#define FT_DEBUG(format, arg...)
#endif

#define LOCATION_SHIFT	10
#define	MAX_Z		16
#define MAX_FINGERS	5

typedef enum {
	FT_CURRENT_KEY			= 0x01,
	FT_FINGER_NUM			= 0x02,
	FT_X1_HI,
	FT_X1_LO,
	FT_Y1_HI,
	FT_Y1_LO,
	FT_Z1_HI,
	FT_Z1_LO,
	FT_X2_HI,
	FT_X2_LO,
	FT_Y2_HI,
	FT_Y2_LO,
	FT_Z2_HI,
	FT_Z2_LO,
	FT_X3_HI,
	FT_X3_LO,
	FT_Y3_HI,
	FT_Y3_LO,
	FT_Z3_HI,
	FT_Z3_LO,
	FT_X4_HI,
	FT_X4_LO,
	FT_Y4_HI,
	FT_Y4_LO,
	FT_Z4_HI,
	FT_Z4_LO,
	FT_X5_HI,
	FT_X5_LO,
	FT_Y5_HI,
	FT_Y5_LO,
	FT_Z5_HI,
	FT_Z5_LO,

	FT_THGROUP			= 0x80,
	FT_THPEAK,
	FT_THCAL,
	FT_THWATER,
	FT_THTEMP,
	FT_THDIFF,
	FT_CTRL,
	FT_TIMEENTERMONITOR,
	FT_PERIODACTIVE,
	FT_PERIODMONITOR,
	FT_HEIGHT_B,
	FT_MAX_FRAME,
	FT_DIST_MOVE,
	FT_DIST_POINT,
	FT_FEG_FRAME,
	FT_SINGLE_CLICK_OFFSET,
	FT_DOUBLE_CLICK_TIME_MIN,
	FT_SINGLE_CLICK_TIME,
	FT_LEFT_RIGHT_OFFSET2,
	FT_UP_DOWN_OFFSET,
	FT_DISTANCE_LEFT_RIGHT,
	FT_DISTANCE_UP_DOWN,
	FT_ZOOM_DIS_SQR,
	FT_RADIAN_VALUE,
	FT_MAX_X_HI,
	FT_MAX_X_LO,
	FT_MAX_Y_HI,
	FT_MAX_Y_LO,
	FT_K_X_HI,
	FT_K_X_LO,
	FT_K_Y_HI,
	FT_K_Y_LO,
	FT_AUTO_CLB_MODE,
	FT_LIB_VERSION_H,
	FT_LIB_VERSION_L,
	FT_CIPHER,
	FT_MODE4,
	FT_PMODE,	/* Power Consume Mode */
	FT_FIRMID,
	FT_STATE,
	FT_FT5201ID,
	FT_ERR,
	FT_CLB,
} ft540_sub_addr_t;

enum {
	FT_UPPER_LEFT	= 0,
	FT_UPPER_RIGHT,
	FT_LOWER_LEFT,
	FT_LOWER_RIGHT,
};

#define NUM_DATA			32

struct ft540 {
	char				phys[32];
	struct input_dev		*input;
	struct i2c_client		*client;
	struct workqueue_struct 	*workqueue;
	struct work_struct		report_worker;
	u8				reg_data[NUM_DATA];
	u8				lights_enabled[4];
	int				irq;
	struct ft540_fix_data		fix;
	int				(*get_pendown_state)(void);
	void				(*clear_penirq)(void);
};

static void ft540_update_lights_status(struct ft540 *ft)
{
	u32	afsel, dir, data;

	afsel	= amba_readl(GPIO1_BASE + 0x18);
	dir	= amba_readl(GPIO1_BASE + 0x04);
	data	= amba_readl(GPIO1_BASE + 0x00);

	//GPIO45
	if (!(afsel & 0x00002000) && (dir & 0x00002000) && (data & 0x00002000)) {
		ft->lights_enabled[FT_LOWER_RIGHT] = 1;
	} else {
		ft->lights_enabled[FT_LOWER_RIGHT] = 0;
	}

	//GPIO46
	if (!(afsel & 0x00004000) && (dir & 0x00004000) && (data & 0x00004000)) {
		ft->lights_enabled[FT_UPPER_RIGHT] = 1;
	} else {
		ft->lights_enabled[FT_UPPER_RIGHT] = 0;
	}

	//GPIO50
	if (!(afsel & 0x00040000) && (dir & 0x00040000) && (data & 0x00040000)) {
		ft->lights_enabled[FT_LOWER_LEFT] = 1;
	} else {
		ft->lights_enabled[FT_LOWER_LEFT] = 0;
	}

	//GPIO51
	if (!(afsel & 0x00080000) && (dir & 0x00080000) && (data & 0x00080000)) {
		ft->lights_enabled[FT_UPPER_LEFT] = 1;
	} else {
		ft->lights_enabled[FT_UPPER_LEFT] = 0;
	}
}

static inline int ft540_read_all(struct ft540 *ft)
{
	if (i2c_smbus_read_i2c_block_data(ft->client,
		0, NUM_DATA, ft->reg_data) != NUM_DATA) {
		printk("I2C Error: %s\n", __func__);
		return -EIO;
	} else {
		return 0;
	}
}

static void ft540_send_event(struct ft540 *ft)
{
	struct input_dev	*input = ft->input;
	u8			i;
	static int		prev_touch = 0;
	int			curr_touch = 0, real_touch = 0;
	static int		touch_seq = 0;
	static int		prev_home = 0, prev_menu = 0, prev_back = 0;
	int			curr_home = 0, curr_menu = 0, curr_back = 0;
	int			event = 0;

	curr_touch = ft->reg_data[FT_FINGER_NUM] & 0x07;

	switch (ft->reg_data[FT_CURRENT_KEY]) {
	case 1:
		if (ft->lights_enabled[FT_UPPER_RIGHT]) {
			curr_home = 1;
		}
		break;
	case 4:
		if (ft->lights_enabled[FT_LOWER_RIGHT]) {
			curr_home = 1;
		}
		break;
	case 7:
		if (ft->lights_enabled[FT_LOWER_LEFT]) {
			curr_home = 1;
		}
		break;
	case 10:
		if (ft->lights_enabled[FT_UPPER_LEFT]) {
			curr_home = 1;
		}
		break;

	case 2:
		if (ft->lights_enabled[FT_UPPER_RIGHT]) {
			curr_menu = 1;
		}
		break;
	case 5:
		if (ft->lights_enabled[FT_LOWER_RIGHT]) {
			curr_menu = 1;
		}
		break;
	case 8:
		if (ft->lights_enabled[FT_LOWER_LEFT]) {
			curr_menu = 1;
		}
		break;
	case 11:
		if (ft->lights_enabled[FT_UPPER_LEFT]) {
			curr_menu = 1;
		}
		break;

	case 3:
		if (ft->lights_enabled[FT_UPPER_RIGHT]) {
			curr_back = 1;
		}
		break;
	case 6:
		if (ft->lights_enabled[FT_LOWER_RIGHT]) {
			curr_back = 1;
		}
		break;
	case 9:
		if (ft->lights_enabled[FT_LOWER_LEFT]) {
			curr_back = 1;
		}
		break;
	case 12:
		if (ft->lights_enabled[FT_UPPER_LEFT]) {
			curr_back = 1;
		}
		break;

	default:
		break;
	}

	for (i = 0; i < curr_touch; i++) {
		u8	xh, xl, yh, yl;
		u32	x, y;

		xh	= ft->reg_data[FT_X1_HI + 6 * i] & 0x0f;
		xl	= ft->reg_data[FT_X1_LO + 6 * i] & 0xff;
		yh	= ft->reg_data[FT_Y1_HI + 6 * i] & 0x0f;
		yl	= ft->reg_data[FT_Y1_LO + 6 * i] & 0xff;
		x	= (xh << 8) | xl;
		y	= (yh << 8) | yl;

		FT_DEBUG("Finger%d Raw: (%d, %d)\n", i, x, y);

		//Android KEYs
		if (x > 1024) {
			//HOME
			if (ft->lights_enabled[FT_LOWER_RIGHT] && y <= LOCATION_SHIFT) {
				curr_home = 1;
			}
			if (ft->lights_enabled[FT_UPPER_RIGHT] && y >= 305 - LOCATION_SHIFT && y <= 305 + LOCATION_SHIFT) {
				curr_home = 1;
			}
			if (ft->lights_enabled[FT_UPPER_LEFT] && y >= 455 - LOCATION_SHIFT && y <= 455 + LOCATION_SHIFT) {
				curr_home = 1;
			}
			if (ft->lights_enabled[FT_LOWER_LEFT] && y >= 765 - LOCATION_SHIFT && y <= 765 + LOCATION_SHIFT) {
				curr_home = 1;
			}

			//MENU
			if (ft->lights_enabled[FT_LOWER_RIGHT] && y >= 70 - LOCATION_SHIFT && y <= 70 + LOCATION_SHIFT) {
				curr_menu = 1;
			}
			if (ft->lights_enabled[FT_UPPER_RIGHT] && y >= 260 - LOCATION_SHIFT && y <= 260 + LOCATION_SHIFT) {
				curr_menu = 1;
			}
			if (ft->lights_enabled[FT_UPPER_LEFT] && y >= 405 - LOCATION_SHIFT && y <= 405 + LOCATION_SHIFT) {
				curr_menu = 1;
			}
			if (ft->lights_enabled[FT_LOWER_LEFT] && y >= 700 - LOCATION_SHIFT && y <= 700 + LOCATION_SHIFT) {
				curr_menu = 1;
			}

			//BACK
			if (ft->lights_enabled[FT_LOWER_RIGHT] && y >= 120 - LOCATION_SHIFT && y <= 120 + LOCATION_SHIFT) {
				curr_back = 1;
			}
			if (ft->lights_enabled[FT_UPPER_RIGHT] && y >= 220 - LOCATION_SHIFT && y <= 220 + LOCATION_SHIFT) {
				curr_back = 1;
			}
			if (ft->lights_enabled[FT_UPPER_LEFT] && y >= 360 - LOCATION_SHIFT && y <= 360 + LOCATION_SHIFT) {
				curr_back = 1;
			}
			if (ft->lights_enabled[FT_LOWER_LEFT] && y >= 650 - LOCATION_SHIFT && y <= 650 + LOCATION_SHIFT) {
				curr_back = 1;
			}
		} else {
			real_touch++;

			if (real_touch == 1) {
				touch_seq++;
			}

			if (touch_seq % 3 == 1) {
				if (x < ft->fix.x_min) {
					x = ft->fix.x_min;
				}
				if (x > ft->fix.x_max) {
					x = ft->fix.x_max;
				}
				if (y < ft->fix.y_min) {
					y = ft->fix.y_min;
				}
				if (y > ft->fix.y_max) {
					y = ft->fix.y_max;
				}

				if (ft->fix.x_invert) {
					x = ft->fix.x_max - x + ft->fix.x_min;
				}
				if (ft->fix.y_invert) {
					y = ft->fix.y_max - y + ft->fix.y_min;
				}

				event = 1;
				if (real_touch == 1) {
					input_report_abs(input, ABS_PRESSURE, MAX_Z);
					input_report_abs(input, ABS_X, x);
					input_report_abs(input, ABS_Y, y);
				}

				input_report_abs(input, ABS_MT_TOUCH_MAJOR, MAX_Z);
				input_report_abs(input, ABS_MT_POSITION_X, x);
				input_report_abs(input, ABS_MT_POSITION_Y, y);
				input_mt_sync(input);
				FT_DEBUG("Finger%d Calibrated: (%d, %d)\n", i, x, y);
			}
		}

	}

	/* Button Pressed */
	if (!prev_touch && real_touch) {
		event = 1;
		input_report_key(input, BTN_TOUCH, real_touch);
	}

	/* Button Released */
	if (prev_touch && !real_touch) {
		event = 1;
		touch_seq = 0;
		input_report_abs(input, ABS_PRESSURE, 0);
		input_report_key(input, BTN_TOUCH, 0);
		input_report_abs(input, ABS_MT_TOUCH_MAJOR, 0);
		input_mt_sync(input);
	}

	//HOME
	if (!prev_home && curr_home) {
		event = 1;
		input_report_key(input, KEY_HOME, 1);
		FT_DEBUG("HOME Pressed\n");
	}
	if (prev_home && !curr_home) {
		event = 1;
		input_report_key(input, KEY_HOME, 0);
		FT_DEBUG("HOME Released\n");
	}

	//MENU
	if (!prev_menu && curr_menu) {
		event = 1;
		input_report_key(input, KEY_MENU, 1);
		FT_DEBUG("MENU Pressed\n");
	}
	if (prev_menu && !curr_menu) {
		event = 1;
		input_report_key(input, KEY_MENU, 0);
		FT_DEBUG("MENU Released\n");
	}

	//ESC
	if (!prev_back && curr_back) {
		event = 1;
		input_report_key(input, KEY_ESC, 1);
		FT_DEBUG("BACK Pressed\n");
	}
	if (prev_back && !curr_back) {
		event = 1;
		input_report_key(input, KEY_ESC, 0);
		FT_DEBUG("ESC Released\n");
	}

	if (event) {
		input_sync(input);
	}

	prev_touch = real_touch;
	prev_home = curr_home;
	prev_menu = curr_menu;
	prev_back = curr_back;
}

static irqreturn_t ft540_irq(int irq, void *handle)
{
	struct ft540 *ft = handle;

	disable_irq_nosync(irq);

	if (ft->clear_penirq) {
		ft->clear_penirq();
	}

	queue_work(ft->workqueue, &ft->report_worker);

	return IRQ_HANDLED;
}

static void ft540_report_worker(struct work_struct *work)
{
	struct ft540	*ft;

	ft = container_of(work, struct ft540, report_worker);

	if (ft->get_pendown_state()) {
		ft540_read_all(ft);
		ft540_update_lights_status(ft);
		ft540_send_event(ft);
	}

	enable_irq(ft->irq);
}

static int ft540_probe(struct i2c_client *client,
	const struct i2c_device_id *id)
{
	struct input_dev 		*input_dev;
	struct ft540 			*ft;
	struct ft540_platform_data	*pdata;
	int				err;

	pdata = client->dev.platform_data;
	if (!pdata) {
		dev_err(&client->dev, "platform data is required!\n");
		return -EINVAL;
	}
	pdata->init_platform_hw();

	if (!i2c_check_functionality(client->adapter,
		I2C_FUNC_SMBUS_WRITE_BYTE_DATA | I2C_FUNC_SMBUS_WRITE_BYTE |
		I2C_FUNC_SMBUS_READ_BYTE))
		return -EIO;

	ft = kzalloc(sizeof(struct ft540), GFP_KERNEL);
	input_dev = input_allocate_device();
	if (!ft || !input_dev) {
		err = -ENOMEM;
		goto err_free_mem;
	}

	ft->client = client;
	i2c_set_clientdata(client, ft);
	ft->input = input_dev;
	ft->get_pendown_state = pdata->get_pendown_state;
	ft->clear_penirq = pdata->clear_penirq;
	snprintf(ft->phys, sizeof(ft->phys),
		 "%s/input0", dev_name(&client->dev));

	ft->fix	= pdata->fix[FT540_FAMILY_0];

	input_dev->name = "Focal Tech FT540 Touchscreen";
	input_dev->phys = ft->phys;
	input_dev->id.bustype = BUS_I2C;
	set_bit(EV_SYN, input_dev->evbit);
	set_bit(EV_KEY, input_dev->evbit);
	set_bit(BTN_TOUCH, input_dev->keybit);
	set_bit(KEY_HOME, input_dev->keybit);
	set_bit(KEY_MENU, input_dev->keybit);
	set_bit(KEY_ESC, input_dev->keybit);
	set_bit(EV_ABS, input_dev->evbit);

	input_set_abs_params(input_dev, ABS_PRESSURE, 0, MAX_Z, 0, 0);
	input_set_abs_params(input_dev, ABS_X, ft->fix.x_min, ft->fix.x_max, 0, 0);
	input_set_abs_params(input_dev, ABS_Y, ft->fix.y_min, ft->fix.y_max, 0, 0);

	input_set_abs_params(input_dev, ABS_MT_TOUCH_MAJOR, 0, MAX_Z, 0, 0);
	input_set_abs_params(input_dev, ABS_MT_POSITION_X, ft->fix.x_min, ft->fix.x_max, 0, 0);
	input_set_abs_params(input_dev, ABS_MT_POSITION_Y, ft->fix.y_min, ft->fix.y_max, 0, 0);

	ft->workqueue = create_singlethread_workqueue("ft540");
	INIT_WORK(&ft->report_worker, ft540_report_worker);

	ft->irq = client->irq;
	err = request_irq(ft->irq, ft540_irq, IRQF_TRIGGER_FALLING,
			client->dev.driver->name, ft);
	if (err < 0) {
		dev_err(&client->dev, "irq %d busy?\n", ft->irq);
		goto err_free_mem;
	}

	err = input_register_device(input_dev);
	if (err)
		goto err_free_irq;

	return 0;

 err_free_irq:
	free_irq(ft->irq, ft);
 err_free_mem:
	input_free_device(input_dev);
	kfree(ft);
	return err;
}

static int ft540_remove(struct i2c_client *client)
{
	struct ft540			*ft = i2c_get_clientdata(client);
	struct ft540_platform_data	*pdata = client->dev.platform_data;

	pdata->exit_platform_hw();
	destroy_workqueue(ft->workqueue);
	free_irq(ft->irq, ft);
	input_unregister_device(ft->input);
	kfree(ft);

	return 0;
}

static struct i2c_device_id ft540_idtable[] = {
	{ "ft540", 0 },
	{ }
};

MODULE_DEVICE_TABLE(i2c, ft540_idtable);

static struct i2c_driver ft540_driver = {
	.driver = {
		.owner	= THIS_MODULE,
		.name	= "ft540"
	},
	.id_table	= ft540_idtable,
	.probe		= ft540_probe,
	.remove		= ft540_remove,
};

static int __init ft540_init(void)
{
	return i2c_add_driver(&ft540_driver);
}

static void __exit ft540_exit(void)
{
	i2c_del_driver(&ft540_driver);
}

module_init(ft540_init);
module_exit(ft540_exit);

MODULE_AUTHOR("Zhenwu Xue <zwxue@ambarella.com>");
MODULE_DESCRIPTION("Focal Tech FT540 TouchScreen Driver");
MODULE_LICENSE("GPL");
