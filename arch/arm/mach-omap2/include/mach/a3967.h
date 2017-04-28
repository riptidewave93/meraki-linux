#ifndef _A3967_H_
#define _A3967_H_

struct a3967_platdata {
	int gpio_reset;
	int gpio_enable;
	int gpio_dir;
	int gpio_step;
	int gpio_ms1;
	int gpio_ms2;
	int gpio_sleep;
	int pulse_per_step;
	int clockwise_forward;
};

#endif /* _A3967_H_ */
