/*****************************************************************************/
/*                                                                           */
/*  Name:           device_version.c                                                 */
/*  Autor:          hw.huang                                            */
/*  Description:    device version                                           */
/*  Project:        IPNC DM814x					                              */
/*                                                                           */
/*****************************************************************************/

/**************************************************************************
 * Included Files
 **************************************************************************/

#include <linux/kernel.h>
#include <linux/string.h>
#include <linux/init.h>
#include <linux/errno.h>
#include <linux/spinlock.h>
#include <linux/module.h>
#include <linux/proc_fs.h>
#include "control.h"
#include <linux/io.h>
#include "device_version.h"


#if CONFIG_PROC_FS

#define MAX_BUF		4096


int device_version_get( void )
{
	u32 id;
	u32 rev;

	id = omap_ctrl_readl(TI81XX_CONTROL_DEVICE_ID);
	rev = (id >> 28) & 0xF;

#ifdef CONFIG_MACH_TI8148IPNC
	/* PG2.1 devices should read 0x3 as chip rev
	 * Some PG2.1 devices have 0xc as chip rev
	 */
	if (0x3 == rev || 0xc == rev)
		return 1;	/*	PG2.1	*/
	else
		return 0;	/*	PG1.0	*/
#endif
	return rev;

}

static int proc_device_version(char *page, char **start, off_t off,
		       int count, int *eof, void *data)
{
	int len = 0;
	char buffer[MAX_BUF];
	
	len +=
	    sprintf(&buffer[len],"%d\n",device_version_get());

	if (count > len)
		count = len;
	memcpy(page, &buffer[off], count);

	*eof = 1;
	*start = NULL;
	return len;
}

int __init device_version_proc_client_create(void)
{
	create_proc_read_entry("device_version", 0, NULL, proc_device_version, NULL);

	
	return 0;
}

#else				/* CONFIG_PROC_FS */

#define device_version_proc_client_create() do {} while(0);

#endif				/* CONFIG_PROC_FS */

EXPORT_SYMBOL(device_version_get);

module_init(device_version_proc_client_create);
