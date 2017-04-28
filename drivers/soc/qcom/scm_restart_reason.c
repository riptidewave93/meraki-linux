/* Copyright (c) 2015-2016, The Linux Foundation. All rights reserved.
 *
 * Permission to use, copy, modify, and/or distribute this software for any
 * purpose with or without fee is hereby granted, provided that the above
 * copyright notice and this permission notice appear in all copies.
 *
 * THE SOFTWARE IS PROVIDED "AS IS" AND THE AUTHOR DISCLAIMS ALL WARRANTIES
 * WITH REGARD TO THIS SOFTWARE INCLUDING ALL IMPLIED WARRANTIES OF
 * MERCHANTABILITY AND FITNESS. IN NO EVENT SHALL THE AUTHOR BE LIABLE FOR
 * ANY SPECIAL, DIRECT, INDIRECT, OR CONSEQUENTIAL DAMAGES OR ANY DAMAGES
 * WHATSOEVER RESULTING FROM LOSS OF USE, DATA OR PROFITS, WHETHER IN AN
 * ACTION OF CONTRACT, NEGLIGENCE OR OTHER TORTIOUS ACTION, ARISING OUT OF
 * OR IN CONNECTION WITH THE USE OR PERFORMANCE OF THIS SOFTWARE.
 *
 */

#include <linux/io.h>
#include <linux/kernel.h>
#include <linux/module.h>
#include <linux/moduleparam.h>
#include <linux/of.h>
#include <linux/of_address.h>
#include <linux/platform_device.h>
#include <linux/notifier.h>
#include <linux/reboot.h>
#include <soc/qcom/scm.h>

#define SET_MAGIC	0x1
#define CLEAR_MAGIC	0x0
#define SCM_CMD_TZ_CONFIG_HW_FOR_RAM_DUMP_ID	0x9
#define SCM_CMD_TZ_FORCE_DLOAD_ID		0x10
#define SCM_CMD_DLOAD_READ			0x12

static int restart_reason = -1;
static uint32_t dload;
#define DLOAD_MAGIC_COOKIE 0x10

/*
 * returns -1 if not yet initialized by scm_restart_reason_probe()
 * or 1 if hw watchdog bite triggered this reboot
 * otherwise 0
 */
int qcom_get_restart_reason(void)
{
	return restart_reason;
}
EXPORT_SYMBOL(qcom_get_restart_reason);

static void scm_restart_dload_mode_enable(void)
{
	unsigned int magic_cookie = SET_MAGIC;
	scm_call(SCM_SVC_BOOT, SCM_CMD_TZ_FORCE_DLOAD_ID, &magic_cookie,
		sizeof(magic_cookie), NULL, 0);
}

static void scm_restart_dload_mode_disable(void)
{
	unsigned int magic_cookie = CLEAR_MAGIC;
	scm_call(SCM_SVC_BOOT, SCM_CMD_TZ_FORCE_DLOAD_ID, &magic_cookie,
		sizeof(magic_cookie), NULL, 0);
}

static void scm_restart_sdi_disable(void)
{
	unsigned int clear_info[] = {
		1 /* Disable wdog debug */, 0 /* SDI enable*/, };
	scm_call(SCM_SVC_BOOT, SCM_CMD_TZ_CONFIG_HW_FOR_RAM_DUMP_ID,
		&clear_info, sizeof(clear_info), NULL, 0);
}

static int scm_restart_panic(struct notifier_block *this,
	unsigned long event, void *data)
{
	/* Don't report panic as hw watchdog bite reboot */
	scm_restart_dload_mode_disable();
	scm_restart_sdi_disable();

	return NOTIFY_DONE;
}

static struct notifier_block panic_nb = {
	.notifier_call = scm_restart_panic,
};

static int scm_restart_reason_reboot(struct notifier_block *nb,
				unsigned long action, void *data)
{
	scm_restart_sdi_disable();
	scm_restart_dload_mode_disable();

	return NOTIFY_DONE;
}

static struct notifier_block reboot_nb = {
	.notifier_call = scm_restart_reason_reboot,
};

static int scm_restart_reason_probe(struct platform_device *pdev)
{
	int ret;

	/*
	 * Preserve DLOAD value before reset below; read DLOAD (persists across
	 * reboots); will be DLOAD_MAGIC_COOKIE if hw watchdog bite triggered
	 */
	ret = scm_call(SCM_SVC_BOOT, SCM_CMD_DLOAD_READ, NULL, 0,
		       (void*)&dload, sizeof(dload));
	if (ret)
		dev_err(&pdev->dev, "failed to read DLOAD:%d\n", ret);
	if (dload && dload != DLOAD_MAGIC_COOKIE)
		dev_info(&pdev->dev, "unrecognized magic cookie:%#x\n", dload);
	restart_reason = !ret && dload == DLOAD_MAGIC_COOKIE;

	scm_restart_dload_mode_enable();

	ret = atomic_notifier_chain_register(&panic_notifier_list, &panic_nb);
	if (ret) {
		dev_err(&pdev->dev, "failed to setup download mode\n");
		goto fail;
	}

	ret = register_reboot_notifier(&reboot_nb);
	if (ret) {
		dev_err(&pdev->dev, "failed to setup reboot handler\n");
		atomic_notifier_chain_unregister(&panic_notifier_list,
								&panic_nb);
		goto fail;
	}

fail:
	return ret;
}

static int scm_restart_reason_remove(struct platform_device *pdev)
{
	atomic_notifier_chain_unregister(&panic_notifier_list, &panic_nb);
	unregister_reboot_notifier(&reboot_nb);
	return 0;
}

static const struct of_device_id scm_restart_reason_match_table[] = {
	{ .compatible = "qca,scm_restart_reason", },
	{}
};
MODULE_DEVICE_TABLE(of, scm_restart_reason_match_table);

static struct platform_driver scm_restart_reason_driver = {
	.probe      = scm_restart_reason_probe,
	.remove     = scm_restart_reason_remove,
	.driver     = {
		.name = "qca_scm_restart_reason",
		.of_match_table = scm_restart_reason_match_table,
	},
};

module_platform_driver(scm_restart_reason_driver);

MODULE_DESCRIPTION("QCA SCM Restart Reason Driver");
