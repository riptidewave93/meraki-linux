#include <linux/kernel.h>
#include <linux/errno.h>
#include <linux/init.h>
#include <linux/module.h>
#include <linux/slab.h>
#include <linux/bitops.h>
#include <asm/byteorder.h>

uint8_t* malloc_base = NULL;
uint8_t* malloc_brk = NULL;

void *my_kmalloc(size_t size) {
    if (!malloc_base)
        return NULL;

    malloc_brk -= size;

    return malloc_brk;
}

void *memset(void *vp, int c, size_t l)
{
    char *p = (char*)vp;
    while (l--)
        *p++ = c;

    return vp;
}

void *my_kzalloc(size_t size) {
    void *r = my_kmalloc(size);
    memset(r, 0, size);
    return r;
}

//need to prevent link errors

void *kmem_cache_alloc(struct kmem_cache *a, gfp_t b) {
    return NULL;
}

#if defined(CONFIG_SLAB)
struct cache_sizes malloc_sizes[0];
#endif
