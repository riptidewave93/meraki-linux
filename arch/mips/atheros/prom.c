#include <linux/serial_reg.h>
#include <linux/init.h>
#include <linux/kernel.h>
#include <linux/string.h>
#include <linux/mm.h>
#include <linux/bootmem.h>

#include <asm/bootinfo.h>
#include <asm/addrspace.h>

#include <atheros.h>

int __ath_flash_size;

#ifdef CONFIG_EARLY_PRINTK
struct _uart_ptr {
	volatile uint32_t rbr_thr;
	volatile uint32_t ier;
	volatile uint32_t iir_fcr;
	volatile uint32_t lcr;
	volatile uint32_t mcr;
	volatile uint32_t lsr;
	volatile uint32_t msr;
	volatile uint32_t scr;
	volatile uint32_t usr;
};

#define UART_BASE ((struct _uart_ptr *)(KSEG1ADDR(ATH_UART_BASE)))

void __init prom_putchar(char c)
{
    while ((UART_BASE->lsr & UART_LSR_THRE) == 0) ;

    UART_BASE->rbr_thr = c;
}

#endif

void __init prom_init(void)
{
	int memsz = 0x2000000, argc = fw_arg0, i;
	char **arg = (char **)fw_arg1;

	printk("flash_size passed from bootloader = %d\n", (int)fw_arg3);
	__ath_flash_size = fw_arg3;

	/*
	 * if user passes kernel args, ignore the default one
	 */
	if (argc > 1) {
		arcs_cmdline[0] = '\0';

		for (i = 1; i < argc; i++)
			printk("arg %d: %s\n", i, arg[i]);

		/*
		 * arg[0] is "g", the rest is boot parameters
		 */
		for (i = 1; i < argc; i++) {
			if (strlen(arcs_cmdline) + strlen(arg[i] + 1)
			    >= sizeof(arcs_cmdline))
				break;
			strcat(arcs_cmdline, arg[i]);
			strcat(arcs_cmdline, " ");
		}
	}

	// mips_machgroup = MACH_GROUP_ATHEROS;

	mips_machtype = CONFIG_ATH_MACH_TYPE;

	/*
	 * By default, use all available memory. You can override this
	 * to use, say, 8MB by specifying "mem=8M" as an argument on the
	 * linux bootup command line.
	 */
	add_memory_region(0, memsz, BOOT_MEM_RAM);
}

void __init prom_free_prom_memory(void)
{
}
