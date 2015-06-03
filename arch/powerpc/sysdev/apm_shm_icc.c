/**
 * APM APM86xxx Shared Memory based Intercore communication
 *
 * Copyright (c) 2010, Applied Micro Circuits Corporation
 * Auther: Vivek Mahajan <vmahajan@apm.com>
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License as
 * published by the Free Software Foundation; either version 2 of
 * the License, or (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place, Suite 330, Boston,
 * MA 02111-1307 USA
 *
 * This is the Linux OS layer for Shared Memory based intercore communication.
 *
 */

#include <linux/of_platform.h>
#include <linux/proc_fs.h>
#include <linux/interrupt.h>
#include <asm/apm_shm_icc.h>
#include <asm/uaccess.h>

#define PFX 			"APMICC: "
#define APM_SHM_ICC_STR		"APM86xxx shmem ICC"
#define APM_SHM_ICC_NAME	"apm_shm_icc"

static unsigned int virq = 0;
static icc_suspend_cb_func_t apm_icc_user_suspend = NULL;
static icc_resume_cb_func_t apm_icc_user_resume = NULL;

int apm_icc_register_suspend(icc_suspend_cb_func_t suspend_cb_func)
{
        apm_icc_user_suspend = suspend_cb_func;
        return 0;
}
EXPORT_SYMBOL(apm_icc_register_suspend);

int apm_icc_register_resume(icc_resume_cb_func_t resume_cb_func)
{
        apm_icc_user_resume = resume_cb_func;
        return 0;
}
EXPORT_SYMBOL(apm_icc_register_resume);

void apm_shm_icc_test_resp(void)
{
	pr_info(PFX "In Async test callback\n");
}

static ssize_t apm_shm_icc_write(struct file *file, const char __user * buf,
				size_t count, loff_t * ppos)
{
	int sz, da, rc;
	char *bulk_data;
	char *udata;
	char *msg;
	char *buffer, *ptr;
	unsigned char code;
	unsigned char tmp;
	unsigned int i = 0, j = 0;
	unsigned int cpuid = mfspr(SPRN_PIR);
	struct apm_icc_user_tx_msg tx_msg;

	if (!buf || count > PAGE_SIZE)
		return -EINVAL;

	bulk_data = kzalloc(BULK_SIZE, GFP_KERNEL);
	if (!bulk_data) {
		pr_err(PFX "bulk data allocation failure!!\n");
		return -ENOMEM;
	}

	udata = kzalloc(ASYNC_SIZE, GFP_KERNEL);
	if (!udata) {
		pr_err(PFX "Async data allocation failure!!\n");
		kfree(bulk_data);
		return -ENOMEM;
	}

	rc = -ENOMEM;
	buffer = (char *)__get_free_page(GFP_KERNEL);
	if (!buffer)
		goto out;

	rc = -EFAULT;
	if (copy_from_user(buffer, buf, count))
		goto out;

	rc = -EINVAL;
	if (count < PAGE_SIZE)
		buffer[count] = '\0';
	else if (buffer[PAGE_SIZE - 1])
		goto out;

	code = simple_strtoul(buffer, &buffer, 0);

	if ((code == ICC_CMD_TX) || (code == ICC_ASYNC_MSG_TX)) {
		/*
		 * bash# echo "1 data 1 2 3 4 5" > /proc/apm_shm_icc
		 * bash# echo "3 data 1 2 3" > /proc/apm_shm_icc
		 */
		if (!strncmp("data", buffer + 1, 4)) {
			ptr = buffer + 6;
			udata[j++] = (char)simple_strtoul(ptr, &ptr, 0);
			while (1) {
				tmp = (char)simple_strtoul(ptr + 1, &ptr, 0);
				if (!*ptr)
					break;
				udata[j++] = tmp;
			}
		} else {
			pr_err(PFX "Data not provided!!\n");
			goto out;
		}
	} else if ((code == ICC_SYNC_DATA_TX) || (code == ICC_ASYNC_DATA_TX)) {
		/*
		 * bash# echo "7 bulk-data 1 2 3" > /proc/apm_shm_icc
		 * bash# echo "8 bulk-data 1 2 3 data 1 2" > /proc/apm_shm_icc
		 */
		if (!strncmp("bulk-data", buffer + 1, 9)) {
			ptr = buffer + 11;
			bulk_data[i++] = (char)simple_strtoul(ptr, &ptr, 0);
			while (1) {
				tmp = (char)simple_strtoul(ptr + 1, &ptr, 0);
				if (!*ptr)
					break;
				bulk_data[i++] = tmp;
				if (*ptr == 'd')
					break;
			}
			if (code == ICC_ASYNC_DATA_TX) {
				ptr += 5;
				udata[j++] = (char)simple_strtoul(ptr, &ptr, 0);
				while (1) {
					tmp = (char)simple_strtoul(ptr + 1,
								   &ptr, 0);
					if (!*ptr)
						break;
					udata[j++] = tmp;
				}
			}
		} else {
			pr_err(PFX "Bulk data undefined!!\n");
			goto out;
		}
	}

	switch (code) {
	case ICC_CMD_TX:
		tx_msg.data_size = j;
		tx_msg.user_data = udata;
		tx_msg.msg_type = ICC_MSG_CMD;
		tx_msg.msg_sub_type = ICC_CMD_TEST;
		rc = apm_icc_send_cmd(!cpuid, &tx_msg);
		if (rc)
			pr_err(PFX "ICC_CMD_TEST error!!\n");
		break;

	case ICC_SYNC_MSG_TX_REQ:
		msg = kzalloc(SYNC_MSG_SIZE, GFP_KERNEL);
		if (!msg) {
			pr_err(PFX "msg allocation failure!!\n");
			rc = -ENOMEM;
			goto out;
		}
		sz = SYNC_MSG_SIZE;
		pr_info("Sync msg.. [%d %d]\n", msg[0], msg[1]);

		rc = apm_icc_send_sync_msg(!cpuid, ICC_REQ_TEST, msg, &sz);
		if (rc)
			pr_err(PFX "apm_icc_send_sync_msg() error!!\n");
		else
			pr_info("Sync test msg ok.. [%d %d]\n", msg[0], msg[1]);

		kfree(msg);
		break;

	case ICC_ASYNC_MSG_TX:
		rc = apm_icc_send_async_msg_req(!cpuid, ICC_REQ_TEST, udata, j,
					    apm_shm_icc_test_resp);
		if (rc)
			pr_err(PFX "Async msg send error!!\n");
		break;

	case ICC_ASYNC_MSG_RX:
		rc = apm_icc_get_async_msg(!cpuid);
		if (rc)
			pr_err(PFX "Async msg get error!!\n");
		break;

	case ICC_SYNC_MSG_TX_HPTR:
		da = 128;
		sz = sizeof(da);
		rc = apm_icc_send_sync_msg(!cpuid, ICC_REQ_SEND_HPTR, (char *)&da,
				       &sz);
		if (rc)
			pr_err(PFX "Sync msg send error - SEND_HPTR !!\n");
		else
			pr_info(PFX "Got heap ptr [0x%08x]\n", da);
		break;

	case ICC_SYNC_MSG_TX_FPTR:
		da = APM_QM_FPID;
		sz = sizeof(da);
		rc = apm_icc_send_sync_msg(!cpuid, ICC_REQ_SEND_FPTR, (char *)&da,
				       &sz);
		if (rc)
			pr_err(PFX "Sync msg send error - SEND_FPTR !!\n");
		else
			pr_info(PFX "Got FP ptr [0x%08x]\n", da);
		break;

	case ICC_SYNC_DATA_TX:
		if (i < MIN_BULK_SIZE) {
			pr_err(PFX "Bulk data size should be at least 32!!\n");
			break;
		}
		rc = apm_icc_send_data(!cpuid, ICC_REQ_TEST, bulk_data, i,
				   apm_shm_icc_test_resp);
		if (rc)
			pr_err(PFX "Data send error !!\n");

		break;

	case ICC_ASYNC_DATA_TX:
		if (i - 1 < MIN_BULK_SIZE) {
			pr_err(PFX "Bulk data size should be at least 32!!\n");
			break;
		}
		tx_msg.data_size = j;
		tx_msg.user_data = udata;
		tx_msg.msg_type = ICC_MSG_AREQ;
		tx_msg.msg_sub_type = ICC_REQ_TEST;
		tx_msg.local = (void *)apm_shm_icc_test_resp;
		tx_msg.callback = (void *)apm_shm_icc_test_resp;
		tx_msg.tx_bulk_ptr = (int)bulk_data;
		tx_msg.tx_bulk_sz = i - 1;
		tx_msg.core = !cpuid;
		rc = apm_icc_send_async_data_req(!cpuid, &tx_msg);
		if (rc)
			pr_err(PFX "Bulk Data send error ||\n");
		break;

	case ICC_LCL_STATS:
#if defined(ICC_STATS_ENABLE)
		apm_icc_print_local_info();
#endif
		break;

	case ICC_RMT_STATS:
#if defined(ICC_STATS_ENABLE)
		apm_icc_print_remote_info(!cpuid);
#endif
		break;
	default:
		pr_err(PFX "Unsupported command 0x%x\n", code);
		break;
	}

	if (!rc)
		rc = count;
out:
	free_page((unsigned long)buffer);
	if (udata)
		kfree(udata);
	if (bulk_data)
		kfree(bulk_data);
	return rc;
}

static const struct file_operations apm_shm_icc_fops = {
	.write = apm_shm_icc_write,
};

static int apm_shm_icc_suspend(struct of_device *op, pm_message_t state)
{
	disable_irq_nosync(virq);
	if(apm_icc_user_suspend)
		apm_icc_user_suspend();
	return 0;
}

static int apm_shm_icc_resume (struct of_device *op)
{
	enable_irq(virq);
	if(apm_icc_user_resume)
		apm_icc_user_resume();
	return 0;
}

static irqreturn_t __icc_msg_interrupt(int interrupt, void *dev)
{
	apm_icc_msg_interrupt(dev);
	return IRQ_HANDLED;
}

static int __devinit apm_shm_icc_probe(struct of_device *dev,
				      const struct of_device_id *match)
{
	struct resource irq;
	struct proc_dir_entry *entry;
	unsigned int cpuid = mfspr(SPRN_PIR);

	if (apm_icc_init()) {
		dev_err(&dev->dev, "ICC init failed\n");
		return -EINVAL;
	}

	if (of_irq_to_resource(dev->node, 0, &irq) == NO_IRQ) {
		dev_err(&dev->dev, "Missing IRQ entry for core=%d\n", cpuid);
		return -EINVAL;
	}

	virq = irq_create_mapping(NULL, (u32) irq.start);
	if (virq == NO_IRQ) {
		dev_err(&dev->dev, "Map irq=%d failed on core=%d\n",
			(u32) irq.start, cpuid);
		return -EINVAL;
	}

	if (request_irq(virq, __icc_msg_interrupt, IRQF_DISABLED | IRQF_PERCPU,
			"shm-icc", (void *)(!cpuid))) {
		dev_err(&dev->dev, "request_irq fails virq=%d\n", virq);
		irq_dispose_mapping(virq);
		return -EFAULT;
	}

	entry = create_proc_entry(APM_SHM_ICC_NAME, 0, NULL);
	if (!entry) {
		dev_err(&dev->dev, "create_proc_entry failed\n");
		free_irq(virq, (void *)(!cpuid));
		irq_dispose_mapping(virq);
		return -EFAULT;
	}

	entry->proc_fops = &apm_shm_icc_fops;
	dev_info(&dev->dev, "%s initialized.\n", APM_SHM_ICC_STR);
	apm_shm_icc_probe_done = 1;
	return 0;
}

static int __devexit apm_shm_icc_remove(struct of_device *dev)
{
	BUG_ON(!virq);

	free_irq(virq, (void *)(!mfspr(SPRN_PIR)));
	irq_dispose_mapping(virq);

	dev_info(&dev->dev, "%s unloaded\n", APM_SHM_ICC_STR);
	return 0;
}

static struct of_device_id apm_shm_icc_match[] = {
	{
		.compatible = "apm,apm86xxx-shm-icc",
	},
	{},
};

MODULE_DEVICE_TABLE(of, apm_shm_icc_match);

static struct of_platform_driver apm_shm_icc_driver = {
	.name = "apm86xxx-shm-icc",
	.match_table = apm_shm_icc_match,
	.probe = apm_shm_icc_probe,
	.remove = __devexit_p(apm_shm_icc_remove),
	.suspend = apm_shm_icc_suspend,
	.resume = apm_shm_icc_resume,
};

static int __init apm_shm_icc_init(void)
{
	return of_register_platform_driver(&apm_shm_icc_driver);
}

static void __exit apm_shm_icc_exit(void)
{
	of_unregister_platform_driver(&apm_shm_icc_driver);
}

module_init(apm_shm_icc_init);
module_exit(apm_shm_icc_exit);

MODULE_DESCRIPTION(APM_SHM_ICC_STR);
MODULE_AUTHOR("Vivek Mahajan <vmahajan@apm.com>");
MODULE_LICENSE("GPL v2");
