#ifndef _DRV8836_H_
#define _DRV8836_H_

struct drv8836_platdata {
	char  label[10];
	int	gpio_Ain1;
	int	gpio_Ain2;
	int	gpio_Bin1;
	int	gpio_Bin2;
	int	pulse_per_step;
};

struct drv8836_platform_data {
	int	num_motors;
	struct drv8836_platdata *data;
};

#endif /* _DRV8836_H_ */
