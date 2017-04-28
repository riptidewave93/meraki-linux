#ifndef _GPIOC_H_
#define _GPIOC_H_

struct gpioc_gpio {
	int     gpio;
	unsigned long   initflags;
};

struct gpioc_export {
	struct gpioc_gpio   *gpios;
	int         ngpios;
	unsigned long       irqflags;
};

#endif /* _GPIOC_H_ */
