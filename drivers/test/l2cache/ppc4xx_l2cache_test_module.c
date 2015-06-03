/**************************************************************************** 
 *
 * Test driver for the PPC44x L2 Cache core.
 * This driver will print out the L2 Cache statistics to the /proc/l2cache
 *
 * Author: Adam Graham <agraham@amcc.com>
 * February 26, 2008
 *
 * Copyright 2008 Applied Micro Circuits Corporation
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License as
 * published by the Free Software Foundation; either version 2 of the
 * License, or (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA 02111-1307, USA.
 *
 ****************************************************************************
 *
 * Beware! The kernel PPC44x L2 Cache must be activated for this test.
 * The example should work on any PPC440EX/GT based board, 
 * maybe also on PPC44x (not yet tested).
 * To use this module:
 * 1. Build it as a kenel module
 * 2. insmod <module>
 *		cat /proc/l2cache/start    to start the l2 cache statistics
 *		cat /proc/l2cache/stop     to stop the l2 cache statistics
 *		cat /proc/l2cache/stats    to read the statistics
 * 3. rmmod <module>
 * Everything you want to know will be printed out
 *
 */

#include <linux/module.h>	/* needed by all modules		*/	
#include <linux/init.h>		/* needed for the module_xxx_macros	*/
#include <linux/fs.h>		/* needed for registering a device	*/
#include <linux/proc_fs.h>	/* needed for registering a device	*/
//#include "ppc4xx_l2cache_core.h" /* Driver Interface */
//#include "ppc4xx_l2cache_proc.h" /* ProcFS Interface */
#include <asm/dcr-regs.h> /* PPC4xx L2 Cache DCR defines */
#include <asm/dcr.h>
#include <asm/reg.h>




#define DRV_NAME	"ppc4xx_l2cache_test_module"
#define DRV_VERSION	"0.1"

MODULE_DESCRIPTION("PPC4xx L2 Cache test driver");
MODULE_AUTHOR("agraham@amcc.com");
MODULE_VERSION(DRV_VERSION);
MODULE_LICENSE("GPL");

uint32_t	ppc4xx_l2cache_base;		/* Set by ioremap function */

#define DCR_L2C_BASE 0x30

#define PPC4xx_L2CACHE_BASEADDR	0xefxxxxx;

/* PPC4xx L2 Cache core registers */
#define PPC4xx_L2CACHE_CONFIG		0x0
#define PPC4xx_L2CACHE_COMMAND		0x1
#define PPC4xx_L2CACHE_ADDRESS		0x2
#define PPC4xx_L2CACHE_DATA		0x3
#define PPC4xx_L2CACHE_STATUS		0x4
#define PPC4xx_L2CACHE_CORE_VERSION	0x5
#define PPC4xx_L2CACHE_SNOOP0		0x6
#define PPC4xx_L2CACHE_SNOOP1		0x7

/* Function Prototypes */
static void ppc4xx_l2cache_cleanup(void);
static int ppc4xx_l2cache_proc_start(char *, char **, off_t, int, int *, void *);
static int ppc4xx_l2cache_proc_stop(char *, char **, off_t, int, int *, void *);
static int ppc4xx_l2cache_proc_perf(char *, char **, off_t, int, int *, void *);
static int ppc4xx_l2cache_proc_query(char *, char **, off_t, int, int *, void *);
static int ppc4xx_l2cache_proc_regs(char *, char **, off_t, int, int *, void *);
int ppc4xx_l2cache_createProcEntry(void);
void ppc4xx_l2cache_removeProcEntry(void);
static u32 ppc4xx_l2cache_read_reg(u32);
static u32 ppc4xx_l2cache_read_config_reg(void);
static u32 ppc4xx_l2cache_read_status_reg(void);
static u32 ppc4xx_l2cache_diag(u32);

/*
 * Kernel Logging
 */
#define LOG(string, args...) \
	printk(KERN_INFO DRV_NAME ": " string "\n",##args)

#define ALERT(string, args...) \
	printk(KERN_ALERT DRV_NAME ": " string "\n",##args)

#define WARNING(string, args...) \
	printk(KERN_WARNING DRV_NAME ": WARNING, " string "\n",##args)

#define ERR(string, args...) \
	printk(KERN_ALERT DRV_NAME ": ERROR, " string "\n",##args)

#if defined(CONFIG_PPC4XX_L2CACHE_DEBUG)
#define DBG(string, args...) \DCRN_L2C0_CFG
	printk(KERN_INFO DRV_NAME ": " string "\n",##args)
#else
#define DBG(string, args...)   do { } while (0)
#endif

static inline u32 ppc4xx_l2cache_read_reg(u32 l2_reg)
{
	u32 reg;

	//reg = mfdcr(l2_reg);

	return reg;
}

static inline u32 ppc4xx_l2cache_read_config_reg(void)
{
	u32 reg;
	reg = mfdcr(DCR_L2C_BASE + DCRN_L2C0_CFG);
	printk("base 0x%08x\n", ppc4xx_l2cache_base); 
	return reg;
}

static inline u32 ppc4xx_l2cache_read_status_reg(void)
{
	u32 reg;
	reg = mfdcr(DCR_L2C_BASE + DCRN_L2C0_SR);
	return reg;
}

/*******************************************************************************
 * Static Variables:
 ******************************************************************************/
static struct proc_dir_entry *proc_driver = NULL; /* /proc/		     */
static struct proc_dir_entry *proc_regs   = NULL; /* /proc/driver/l2cache/   */
static struct proc_dir_entry *proc_start  = NULL; /* /proc/driver/l2cache/   */
static struct proc_dir_entry *proc_stop   = NULL; /* /proc/driver/l2cache/   */
static struct proc_dir_entry *proc_perf   = NULL; /* /proc/driver/l2cache/   */
static struct proc_dir_entry *proc_query  = NULL; /* /proc/driver/l2cache/   */

/* Issue L2C diagnostic command */
static inline u32 ppc4xx_l2cache_diag(u32 addr)
{
	asm volatile ("sync" ::: "memory");
	mtdcr(DCR_L2C_BASE + DCRN_L2C0_ADDR, addr);
	mtdcr(DCR_L2C_BASE + DCRN_L2C0_CMD, L2C_CMD_DIAG);

	while (!(mfdcr(DCR_L2C_BASE + DCRN_L2C0_SR) & L2C_SR_CC)) {
		;
	}
	asm volatile ("sync; isync" ::: "memory");

	return mfdcr(DCR_L2C_BASE + DCRN_L2C0_DATA);
}

static int ppc4xx_l2cache_proc_start(char *buffer, char **start_off,
					off_t offset, int buff_len, int *eof, void *data)
{
	int len  = 0;
	u32 sr = mfdcr(DCR_L2C_BASE + DCRN_L2C0_SR); 

#if defined(L2_TEST_DEBUG)
	len += sprintf(buffer + len,
		"PPC4xx L2 Cache Start L2C Performance counter.\n");
#endif

	if (sr & L2C_SR_PCS) {
		/* Start Performance monitor counters */
		mtdcr(DCR_L2C_BASE + DCRN_L2C0_CMD, L2C_CMD_STRC);
		len += sprintf(buffer + len,
			"PPC4xx L2 Cache Performance counter started.\n");
	} else {
		len += sprintf(buffer + len,
			"PPC4xx L2 Cache Performance counter is already started.\n");
	}

	return len;
}

static int ppc4xx_l2cache_proc_query(char *buffer, char **start_off,
					off_t offset, int buff_len, int *eof, void *data)
{
	int len  = 0;
	u32 sr = mfdcr(DCR_L2C_BASE + DCRN_L2C0_SR);

#if defined(L2_TEST_DEBUG)
	len += sprintf(buffer + len,
		"PPC4xx L2 Cache Query L2C Performance monitor.\n");
#endif

	if (sr & L2C_SR_PCS) {
		len += sprintf(buffer + len,
			"PPC4xx L2 Cache Performance counter is stopped.\n");
	} else {
		len += sprintf(buffer + len,
			"PPC4xx L2 Cache Performance counter is running.\n");
	}

	return len;
}

static int ppc4xx_l2cache_proc_stop(char *buffer, char **start_off,
					off_t offset, int buff_len, int *eof, void *data)
{
	int len  = 0;
	u32 sr = mfdcr(DCR_L2C_BASE + DCRN_L2C0_SR); 
	
#if defined(L2_TEST_DEBUG)
	len += sprintf(buffer + len,
		"PPC4xx L2 Cache Stop L2C Performance counter.\n");
#endif

	if (sr & L2C_SR_PCS) {
		len += sprintf(buffer + len,
			"PPC4xx L2 Cache Performance counter is already stopped.\n");
	} else {
		/* Start Performance monitor counters */
		mtdcr(DCR_L2C_BASE + DCRN_L2C0_CMD, L2C_CMD_STPC);
		len += sprintf(buffer + len,
			"PPC4xx L2 Cache Performance counter stopped.\n");
	}

	return len;
}

static int ppc4xx_l2cache_proc_perf(char *buffer, char **start_off,
					off_t offset, int buff_len, int *eof, void *data)
{
	u32 l2_data; 
	int len  = 0;
	u32 sr = mfdcr(DCR_L2C_BASE + DCRN_L2C0_SR); 

#if defined(L2_TEST_DEBUG)
	len += sprintf(buffer + len,
		"PPC4xx L2 Cache perf L2C read Performance Counter registers.\n");
#endif

	if (sr & L2C_SR_PCS) {
		len += sprintf(buffer + len,
			"PPC4xx L2 Cache Performance counter is not running.\n");
	} else {
		/* read diagnostics stats */
		l2_data = ppc4xx_l2cache_diag(0x22000000);
		len += sprintf(buffer + len,
			"PPC4xx L2 Cache Performance Hit Counter Read 0x%08x\n", l2_data);

		l2_data = ppc4xx_l2cache_diag(0x24000000);
		len += sprintf(buffer + len,
			"PPC4xx L2 Cache Performance Request Counter Read 0x%08x\n", l2_data);

		l2_data = ppc4xx_l2cache_diag(0x28000000);
		len += sprintf(buffer + len,
			"PPC4xx L2 Cache Performance Cycle Counter Read 0x%08x\n", l2_data);
	}
	
	return len;
}

static int ppc4xx_l2cache_proc_regs(char *buffer, char **start_off,
				    off_t offset, int buff_len, int *eof, void *data)
{
	int len  = 0;

#if defined(L2_TEST_DEBUG)
	len += sprintf(buffer + len,
		"PPC4xx L2 Cache Registers.\n");
#endif

	len += sprintf(buffer + len,
		"PPC4xx L2_CONFIG Register 0x%08x\n",
			ppc4xx_l2cache_read_config_reg());

	len += sprintf(buffer + len,
		"PPC4xx L2_STATUS Register 0x%08x\n",
			ppc4xx_l2cache_read_status_reg());

	return len;
}

int ppc4xx_l2cache_createProcEntry(void)
{
	proc_driver = proc_mkdir("driver/l2cache", NULL);


	if (proc_driver == NULL) {
		ERR("while creating proc directory");
		return -ENOMEM;
	}

	proc_regs = create_proc_entry("registers", 0, proc_driver);

	if (proc_regs == NULL) {
		ERR("while creating proc register file");
		return -ENOMEM;
	}else {
	  proc_regs->read_proc = ppc4xx_l2cache_proc_regs;
	}

	proc_start = create_proc_entry("start", 0, proc_driver);
	

	if (proc_start == NULL) {
		ERR("while creating proc start file");
		return -ENOMEM;
	}else {
	  proc_start->read_proc = ppc4xx_l2cache_proc_start;
	}


	proc_stop = create_proc_entry("stop", 0, proc_driver);

	if (proc_stop == NULL) {
		ERR("while creating proc stop file");
		return -ENOMEM;
	}else {
	  proc_stop->read_proc = ppc4xx_l2cache_proc_stop;
	}

	proc_perf  = create_proc_entry("performance", 0, proc_driver);

	if (proc_perf == NULL) {
		ERR("while creating proc perf file");
		return -ENOMEM;
	}else {
	  proc_perf->read_proc = ppc4xx_l2cache_proc_perf;
	}

	proc_query = create_proc_entry("query", 0, proc_driver);

	if (proc_query == NULL) {
		ERR("while creating proc query file");
		return -ENOMEM;
	}else {
	  proc_query->read_proc = ppc4xx_l2cache_proc_query;
	}
	
	return 0;
}

static int __init ppc4xx_l2cache_init_module(void)
{
	int retval = 0;

	LOG("initializing");
	LOG("version %s", DRV_VERSION);

	retval = ppc4xx_l2cache_createProcEntry();

	if (retval < 0) {
		ERR("while registering l2cache test driver.");
	}

	return retval;
}

module_init(ppc4xx_l2cache_init_module);

static void __exit ppc4xx_l2cache_cleanup_module(void)
{
        LOG("cleaning up");

        ppc4xx_l2cache_cleanup();
}

module_exit(ppc4xx_l2cache_cleanup_module);

static void
ppc4xx_l2cache_cleanup(void)
{
	ppc4xx_l2cache_removeProcEntry();
}

void
ppc4xx_l2cache_removeProcEntry(void)
{
        if (proc_regs)   remove_proc_entry("registers", proc_driver);
        if (proc_start)  remove_proc_entry("start", proc_driver);
        if (proc_stop)   remove_proc_entry("stop", proc_driver);
        if (proc_perf)   remove_proc_entry("performance", proc_driver);
        if (proc_query)  remove_proc_entry("query", proc_driver);
        if (proc_driver) remove_proc_entry("l2cache", NULL);
}
