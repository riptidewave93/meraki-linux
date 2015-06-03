#
# Makefile for multiplexer I2C chip drivers.

obj-$(CONFIG_I2C_MUX_GPIO)	+= gpio-i2cmux.o
obj-$(CONFIG_I2C_GPIO_SCL)	+= i2c-gpio-scl.o

ifeq ($(CONFIG_I2C_DEBUG_BUS),y)
EXTRA_CFLAGS += -DDEBUG
endif
