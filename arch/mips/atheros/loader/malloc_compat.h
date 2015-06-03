#include <linux/kernel.h>
#include <linux/errno.h>
#include <linux/init.h>
#include <linux/module.h>
#include <linux/slab.h>
#include <linux/bitops.h>
#include <asm/byteorder.h>

//hijack kernel allocators in .c files we compile, but not in header definitions included above
#define kmalloc(s, sec) my_kmalloc(s)
#define kzalloc(s, sec) my_kzalloc(s)
#define kfree(s)

extern void *my_kmalloc(size_t);
extern void *my_kzalloc(size_t);

extern uint8_t* malloc_base;
extern uint8_t* malloc_brk;


