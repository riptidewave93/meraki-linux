/* Copyright (c) 2016-2017, The Linux Foundation. All rights reserved.
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License version 2 and
 * only version 2 as published by the Free Software Foundation.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 */

#include <linux/delay.h>
#include <linux/module.h>
#include <linux/of.h>
#include <linux/of_device.h>
#include <linux/pm_wakeup.h>
#include <linux/rwsem.h>
#include <linux/suspend.h>
#include <soc/qcom/ramdump.h>
#include <soc/qcom/subsystem_notif.h>

#include "debug.h"
#include "main.h"
#include "pci.h"
#include "qmi.h"

#define CNSS_DUMP_FORMAT_VER		0x11
#define CNSS_DUMP_MAGIC_VER_V2		0x42445953
#define CNSS_DUMP_NAME			"CNSS_WLAN"
#define WLAN_RECOVERY_DELAY		1000
#define FILE_SYSTEM_READY		1
#define FW_READY_TIMEOUT		5000
#define CNSS_EVENT_PENDING		2989
#define MAX_NUMBER_OF_SOCS 4

static struct cnss_plat_data *plat_env[MAX_NUMBER_OF_SOCS];
static int plat_env_index;

static DECLARE_RWSEM(cnss_pm_sem);

static bool fbc_bypass;
#ifdef CONFIG_CNSS2_DEBUG
module_param(fbc_bypass, bool, S_IRUSR | S_IWUSR);
MODULE_PARM_DESC(fbc_bypass,
		 "Bypass firmware download when loading WLAN driver");
#endif

static bool qmi_bypass;
#ifdef CONFIG_CNSS2_DEBUG
module_param(qmi_bypass, bool, S_IRUSR | S_IWUSR);
MODULE_PARM_DESC(qmi_bypass, "Bypass QMI from platform driver");
#endif

extern int wlfw_service_instance_id;
static bool enable_waltest;
#ifdef CONFIG_CNSS2_DEBUG
module_param(enable_waltest, bool, S_IRUSR | S_IWUSR);
MODULE_PARM_DESC(enable_waltest, "Enable to handle firmware waltest");
#endif

static struct cnss_fw_files FW_FILES_QCA6174_FW_3_0 = {
	"qwlan30.bin", "bdwlan30.bin", "otp30.bin", "utf30.bin",
	"utfbd30.bin", "epping30.bin", "evicted30.bin"
};

static struct cnss_fw_files FW_FILES_DEFAULT = {
	"qwlan.bin", "bdwlan.bin", "otp.bin", "utf.bin",
	"utfbd.bin", "epping.bin", "evicted.bin"
};

struct cnss_driver_event {
	struct list_head list;
	enum cnss_driver_event_type type;
	bool sync;
	struct completion complete;
	int ret;
	void *data;
};

static enum cnss_dev_bus_type cnss_get_dev_bus_type(struct device *dev)
{
	if (!dev)
		return CNSS_BUS_NONE;

	if (!dev->bus)
		return CNSS_BUS_NONE;

	if (memcmp(dev->bus->name, "pci", 3) == 0)
		return CNSS_BUS_PCI;
	else if (memcmp(dev->bus->name, "platform", 8) == 0)
		return CNSS_BUS_AHB;
	else
		return CNSS_BUS_NONE;
}

static void cnss_set_plat_priv(struct platform_device *plat_dev,
			       struct cnss_plat_data *plat_priv)
{
	if (plat_env_index >= MAX_NUMBER_OF_SOCS) {
		printk("ERROR: No space to allocate save the plat_priv\n");
		return;
	}
	plat_env[plat_env_index++] = plat_priv;
}

static struct cnss_plat_data *cnss_get_plat_priv_by_device_id(int id)
{
	int i;

	for (i = 0; i < plat_env_index; i++) {
		if (plat_env[i]->device_id == id)
			return plat_env[i];
	}
	return NULL;
}

static struct cnss_plat_data *cnss_get_plat_priv(struct platform_device
						 *plat_dev)
{
	int i;

	if (!plat_dev)
		return NULL;

	for (i = 0; i < plat_env_index; i++) {
		if (plat_env[i]->plat_dev == plat_dev)
			return plat_env[i];
	}
	return NULL;
}

void *cnss_bus_dev_to_bus_priv(struct device *dev)
{
	if (!dev)
		return NULL;

	switch (cnss_get_dev_bus_type(dev)) {
	case CNSS_BUS_PCI:
		return cnss_get_pci_priv(to_pci_dev(dev));
	case CNSS_BUS_AHB:
		return NULL;
	default:
		return NULL;
	}
}

struct cnss_plat_data *cnss_bus_dev_to_plat_priv(struct device *dev)
{
	void *bus_priv;

	if (!dev)
		return cnss_get_plat_priv(NULL);

	bus_priv = cnss_bus_dev_to_bus_priv(dev);
	if (!bus_priv && cnss_get_dev_bus_type(dev) == CNSS_BUS_PCI)
		return NULL;

	switch (cnss_get_dev_bus_type(dev)) {
	case CNSS_BUS_PCI:
		return cnss_pci_priv_to_plat_priv(bus_priv);
	case CNSS_BUS_AHB:
		return cnss_get_plat_priv(to_platform_device(dev));
	default:
		return NULL;
	}
}

static int cnss_pm_notify(struct notifier_block *b,
			  unsigned long event, void *p)
{
	switch (event) {
	case PM_SUSPEND_PREPARE:
		down_write(&cnss_pm_sem);
		break;
	case PM_POST_SUSPEND:
		up_write(&cnss_pm_sem);
		break;
	}

	return NOTIFY_DONE;
}

static struct notifier_block cnss_pm_notifier = {
	.notifier_call = cnss_pm_notify,
};

void cnss_lock_pm_sem(void)
{
	down_read(&cnss_pm_sem);
}
EXPORT_SYMBOL(cnss_lock_pm_sem);

void cnss_release_pm_sem(void)
{
	up_read(&cnss_pm_sem);
}
EXPORT_SYMBOL(cnss_release_pm_sem);

int cnss_get_fw_files_for_target(struct cnss_fw_files *pfw_files,
				 u32 target_type, u32 target_version)
{
	if (!pfw_files)
		return -ENODEV;

	switch (target_version) {
	case QCA6174_REV3_VERSION:
	case QCA6174_REV3_2_VERSION:
		memcpy(pfw_files, &FW_FILES_QCA6174_FW_3_0, sizeof(*pfw_files));
		break;
	default:
		memcpy(pfw_files, &FW_FILES_DEFAULT, sizeof(*pfw_files));
		cnss_pr_err("Unknown target version, type: 0x%X, version: 0x%X",
			    target_type, target_version);
		break;
	}

	return 0;
}
EXPORT_SYMBOL(cnss_get_fw_files_for_target);

int cnss_request_bus_bandwidth(int bandwidth, struct cnss_plat_data *plat_priv)
{
	int ret;
	struct cnss_bus_bw_info *bus_bw_info;

	if (!plat_priv)
		return -ENODEV;

	bus_bw_info = &plat_priv->bus_bw_info;
	if (!bus_bw_info->bus_client)
		return -ENOSYS;

	switch (bandwidth) {
	case CNSS_BUS_WIDTH_NONE:
	case CNSS_BUS_WIDTH_LOW:
	case CNSS_BUS_WIDTH_MEDIUM:
	case CNSS_BUS_WIDTH_HIGH:
		ret = msm_bus_scale_client_update_request(
			bus_bw_info->bus_client, bandwidth);
		if (!ret)
			bus_bw_info->current_bw_vote = bandwidth;
		else
			cnss_pr_err("Could not set bus bandwidth: %d, err = %d\n",
				    bandwidth, ret);
		break;
	default:
		cnss_pr_err("Invalid bus bandwidth: %d", bandwidth);
		ret = -EINVAL;
	}

	return ret;
}
EXPORT_SYMBOL(cnss_request_bus_bandwidth);

int cnss_get_platform_cap(struct cnss_platform_cap *cap)
{
	struct cnss_plat_data *plat_priv = cnss_bus_dev_to_plat_priv(NULL);

	if (!plat_priv)
		return -ENODEV;

	if (cap)
		*cap = plat_priv->cap;

	return 0;
}
EXPORT_SYMBOL(cnss_get_platform_cap);

int cnss_get_soc_info(struct device *dev, struct cnss_soc_info *info)
{
	int ret;
	struct cnss_plat_data *plat_priv = cnss_bus_dev_to_plat_priv(dev);
	void *bus_priv = cnss_bus_dev_to_bus_priv(dev);

	if (!plat_priv)
		return -ENODEV;

	ret = cnss_pci_get_bar_info(bus_priv, &info->va, &info->pa);
	if (ret)
		return ret;

	return 0;
}
EXPORT_SYMBOL(cnss_get_soc_info);

void cnss_set_driver_status(enum cnss_driver_status driver_status)
{
	struct cnss_plat_data *plat_priv = cnss_bus_dev_to_plat_priv(NULL);

	if (!plat_priv)
		return;

	plat_priv->driver_status = driver_status;
}
EXPORT_SYMBOL(cnss_set_driver_status);

void cnss_request_pm_qos(u32 qos_val)
{
	struct cnss_plat_data *plat_priv = cnss_bus_dev_to_plat_priv(NULL);

	if (!plat_priv)
		return;

	pm_qos_add_request(&plat_priv->qos_request, PM_QOS_CPU_DMA_LATENCY,
			   qos_val);
}
EXPORT_SYMBOL(cnss_request_pm_qos);

void cnss_remove_pm_qos(void)
{
	struct cnss_plat_data *plat_priv = cnss_bus_dev_to_plat_priv(NULL);

	if (!plat_priv)
		return;

	pm_qos_remove_request(&plat_priv->qos_request);
}
EXPORT_SYMBOL(cnss_remove_pm_qos);

u8 *cnss_common_get_wlan_mac_address(struct device *dev, uint32_t *num)
{
	struct cnss_plat_data *plat_priv = cnss_bus_dev_to_plat_priv(dev);
	struct cnss_wlan_mac_info *wlan_mac_info;
	struct cnss_wlan_mac_addr *addr;

	if (!plat_priv)
		goto out;

	wlan_mac_info = &plat_priv->wlan_mac_info;
	if (!wlan_mac_info->is_wlan_mac_set) {
		cnss_pr_info("Platform driver doesn't have any MAC address!\n");
		goto out;
	}

	addr = &wlan_mac_info->wlan_mac_addr;
	*num = addr->no_of_mac_addr_set;

	return &addr->mac_addr[0][0];
out:
	*num = 0;
	return NULL;
}
EXPORT_SYMBOL(cnss_common_get_wlan_mac_address);

int cnss_wlan_enable(struct device *dev,
		     struct cnss_wlan_enable_cfg *config,
		     enum cnss_driver_mode mode,
		     const char *host_version)
{
	struct cnss_plat_data *plat_priv = cnss_bus_dev_to_plat_priv(dev);
	struct wlfw_wlan_cfg_req_msg_v01 req;
	u32 i;
	int ret;

	if (!plat_priv)
		return 0;

	if (plat_priv->device_id == QCA6174_DEVICE_ID)
		return 0;

	if (qmi_bypass)
		return 0;

	if (mode == CNSS_CALIBRATION)
		goto skip_cfg;

	if (!config || !host_version) {
		cnss_pr_err("Invalid config or host_version pointer\n");
		return -EINVAL;
	}

	cnss_pr_dbg("Mode: %d, config: %pK, host_version: %s\n",
		    mode, config, host_version);

	if (mode == CNSS_WALTEST || mode == CNSS_CCPM)
		goto skip_cfg;

	memset(&req, 0, sizeof(req));

	req.host_version_valid = 1;
	strlcpy(req.host_version, host_version,
		QMI_WLFW_MAX_STR_LEN_V01 + 1);

	req.tgt_cfg_valid = 1;
	if (config->num_ce_tgt_cfg > QMI_WLFW_MAX_NUM_CE_V01)
		req.tgt_cfg_len = QMI_WLFW_MAX_NUM_CE_V01;
	else
		req.tgt_cfg_len = config->num_ce_tgt_cfg;
	for (i = 0; i < req.tgt_cfg_len; i++) {
		req.tgt_cfg[i].pipe_num = config->ce_tgt_cfg[i].pipe_num;
		req.tgt_cfg[i].pipe_dir = config->ce_tgt_cfg[i].pipe_dir;
		req.tgt_cfg[i].nentries = config->ce_tgt_cfg[i].nentries;
		req.tgt_cfg[i].nbytes_max = config->ce_tgt_cfg[i].nbytes_max;
		req.tgt_cfg[i].flags = config->ce_tgt_cfg[i].flags;
	}

	req.svc_cfg_valid = 1;
	if (config->num_ce_svc_pipe_cfg > QMI_WLFW_MAX_NUM_SVC_V01)
		req.svc_cfg_len = QMI_WLFW_MAX_NUM_SVC_V01;
	else
		req.svc_cfg_len = config->num_ce_svc_pipe_cfg;
	for (i = 0; i < req.svc_cfg_len; i++) {
		req.svc_cfg[i].service_id = config->ce_svc_cfg[i].service_id;
		req.svc_cfg[i].pipe_dir = config->ce_svc_cfg[i].pipe_dir;
		req.svc_cfg[i].pipe_num = config->ce_svc_cfg[i].pipe_num;
	}

#ifdef CONFIG_CNSS2_SHADOW_REGISTER
	req.shadow_reg_v2_valid = 1;
	if (config->num_shadow_reg_v2_cfg >
	    QMI_WLFW_MAX_NUM_SHADOW_REG_V2_V01)
		req.shadow_reg_v2_len = QMI_WLFW_MAX_NUM_SHADOW_REG_V2_V01;
	else
		req.shadow_reg_v2_len = config->num_shadow_reg_v2_cfg;

	memcpy(req.shadow_reg_v2, config->shadow_reg_v2_cfg,
	       sizeof(struct wlfw_shadow_reg_v2_cfg_s_v01)
	       * req.shadow_reg_v2_len);
#else
	req.shadow_reg_v2_valid = 0;
#endif
	if (mode != QMI_WLFW_CALIBRATION_V01) {
		ret = cnss_wlfw_wlan_cfg_send_sync(plat_priv, &req);
		if (ret)
			goto out;
	}

skip_cfg:
	ret = cnss_wlfw_wlan_mode_send_sync(plat_priv, mode);
out:
	return ret;
}
EXPORT_SYMBOL(cnss_wlan_enable);

int cnss_wlan_disable(struct device *dev, enum cnss_driver_mode mode)
{
	struct cnss_plat_data *plat_priv = cnss_bus_dev_to_plat_priv(dev);

	if (!plat_priv)
		return 0;

	if (plat_priv->device_id == QCA6174_DEVICE_ID)
		return 0;

	if (qmi_bypass)
		return 0;

	return cnss_wlfw_wlan_mode_send_sync(plat_priv, QMI_WLFW_OFF_V01);
}
EXPORT_SYMBOL(cnss_wlan_disable);

static int cnss_fw_mem_ready_hdlr(struct cnss_plat_data *plat_priv)
{
	int ret;

	if (!plat_priv)
		return -ENODEV;

	set_bit(CNSS_FW_MEM_READY, &plat_priv->driver_state);

	ret = cnss_wlfw_tgt_cap_send_sync(plat_priv);
	if (ret)
		goto out;

	ret = cnss_wlfw_bdf_dnld_send_sync(plat_priv);
	if (ret)
		goto out;

	if (plat_priv->device_id == QCA8074_DEVICE_ID)
		ret = cnss_wlfw_m3_dnld_send_sync(plat_priv);
out:
	return ret;
}

int cnss_is_fw_ready(struct device *dev)
{
	struct cnss_plat_data *plat_priv = cnss_bus_dev_to_plat_priv(dev);

	if (test_bit(CNSS_FW_READY, &plat_priv->driver_state))
		return 1;
	return 0;
}
EXPORT_SYMBOL(cnss_is_fw_ready);

int cnss_is_cold_boot_cal_done(struct device *dev)
{
	struct cnss_plat_data *plat_priv = cnss_bus_dev_to_plat_priv(dev);

	if (test_bit(CNSS_COLD_BOOT_CAL_DONE, &plat_priv->driver_state))
		return 1;
	return 0;
}
EXPORT_SYMBOL(cnss_is_cold_boot_cal_done);

static int cnss_fw_ready_hdlr(struct cnss_plat_data *plat_priv)
{
	struct cnss_pci_data *pci_priv;
	int ret = 0;

	if (!plat_priv)
		return -ENODEV;

	set_bit(CNSS_FW_READY, &plat_priv->driver_state);

	pci_priv = plat_priv->bus_priv;
	if (!pci_priv)
		return -ENODEV;

#ifdef CONFIG_CNSS2_CALIBRATION_SUPPORT
	if (plat_priv->driver_status == CNSS_LOAD_UNLOAD)
		complete(&plat_priv->fw_ready_event);
	else
		ret = cnss_wlfw_wlan_mode_send_sync(plat_priv,
						    QMI_WLFW_CALIBRATION_V01);
#endif
	return ret;
}

static int cnss_cold_boot_cal_done_hdlr(struct cnss_plat_data *plat_priv)
{
	struct cnss_pci_data *pci_priv;

	if (!plat_priv)
		return -ENODEV;

	set_bit(CNSS_COLD_BOOT_CAL_DONE, &plat_priv->driver_state);

	pci_priv = plat_priv->bus_priv;
	if (!pci_priv)
		return -ENODEV;

	cnss_wlfw_wlan_mode_send_sync(plat_priv, QMI_WLFW_OFF_V01);
	cnss_pci_stop_mhi(pci_priv);
	cnss_suspend_pci_link(pci_priv);
	cnss_power_off_device(plat_priv);

	return 0;
}

static char *cnss_driver_event_to_str(enum cnss_driver_event_type type)
{
	switch (type) {
	case CNSS_DRIVER_EVENT_SERVER_ARRIVE:
		return "SERVER_ARRIVE";
	case CNSS_DRIVER_EVENT_SERVER_EXIT:
		return "SERVER_EXIT";
	case CNSS_DRIVER_EVENT_REQUEST_MEM:
		return "REQUEST_MEM";
	case CNSS_DRIVER_EVENT_FW_MEM_READY:
		return "FW_MEM_READY";
	case CNSS_DRIVER_EVENT_FW_READY:
		return "FW_READY";
	case CNSS_DRIVER_EVENT_COLD_BOOT_CAL_DONE:
		return "COLD_BOOT_CAL_DONE";
	case CNSS_DRIVER_EVENT_MAX:
		return "EVENT_MAX";
	}

	return "UNKNOWN";
};

static void cnss_driver_event_work(struct work_struct *work)
{
	struct cnss_plat_data *plat_priv =
		container_of(work, struct cnss_plat_data, event_work);
	struct cnss_driver_event *event;
	unsigned long flags;
	int ret = 0;

	if (!plat_priv)
		return;

	spin_lock_irqsave(&plat_priv->event_lock, flags);

	while (!list_empty(&plat_priv->event_list)) {
		event = list_first_entry(&plat_priv->event_list,
					 struct cnss_driver_event, list);
		list_del(&event->list);
		spin_unlock_irqrestore(&plat_priv->event_lock, flags);

		cnss_pr_dbg("Processing driver event: %s%s(%d), state: 0x%lx\n",
			    cnss_driver_event_to_str(event->type),
			    event->sync ? "-sync" : "", event->type,
			    plat_priv->driver_state);

		switch (event->type) {
		case CNSS_DRIVER_EVENT_SERVER_ARRIVE:
			ret = cnss_wlfw_server_arrive(plat_priv);
			break;
		case CNSS_DRIVER_EVENT_SERVER_EXIT:
			ret = cnss_wlfw_server_exit(plat_priv);
			break;
		case CNSS_DRIVER_EVENT_REQUEST_MEM:
			ret = cnss_pci_alloc_fw_mem(plat_priv);
			if (ret)
				break;
			ret = cnss_wlfw_respond_mem_send_sync(plat_priv);
			break;
		case CNSS_DRIVER_EVENT_FW_MEM_READY:
			ret = cnss_fw_mem_ready_hdlr(plat_priv);
			break;
		case CNSS_DRIVER_EVENT_FW_READY:
			ret = cnss_fw_ready_hdlr(plat_priv);
			break;
		case CNSS_DRIVER_EVENT_COLD_BOOT_CAL_DONE:
			ret = cnss_cold_boot_cal_done_hdlr(plat_priv);
			break;
		default:
			cnss_pr_err("Invalid driver event type: %d",
				    event->type);
			kfree(event);
			continue;
		}

		spin_lock_irqsave(&plat_priv->event_lock, flags);
		if (event->sync) {
			event->ret = ret;
			complete(&event->complete);
			continue;
		}
		spin_unlock_irqrestore(&plat_priv->event_lock, flags);

		kfree(event);

		spin_lock_irqsave(&plat_priv->event_lock, flags);
	}
	spin_unlock_irqrestore(&plat_priv->event_lock, flags);
}

static void cnss_recovery_work_func(struct work_struct *work)
{
	struct cnss_recovery_work_t *ctx =
		container_of(work, struct cnss_recovery_work_t, work);

	cnss_self_recovery(ctx->dev, ctx->reason);
}

int cnss_driver_event_post(struct cnss_plat_data *plat_priv,
			   enum cnss_driver_event_type type,
			   bool sync, void *data)
{
	struct cnss_driver_event *event;
	unsigned long flags;
	int gfp = GFP_KERNEL;
	int ret = 0;

	if (!plat_priv)
		return -ENODEV;

	cnss_pr_dbg("Posting event: %s(%d)%s, state: 0x%lx\n",
		    cnss_driver_event_to_str(type), type,
		    sync ? "-sync" : "", plat_priv->driver_state);

	if (type >= CNSS_DRIVER_EVENT_MAX) {
		cnss_pr_err("Invalid Event type: %d, can't post", type);
		return -EINVAL;
	}

	if (in_interrupt() || irqs_disabled())
		gfp = GFP_ATOMIC;

	event = kzalloc(sizeof(*event), gfp);
	if (!event)
		return -ENOMEM;

	event->type = type;
	event->data = data;
	init_completion(&event->complete);
	event->ret = CNSS_EVENT_PENDING;
	event->sync = sync;

	spin_lock_irqsave(&plat_priv->event_lock, flags);
	list_add_tail(&event->list, &plat_priv->event_list);
	spin_unlock_irqrestore(&plat_priv->event_lock, flags);

	queue_work(plat_priv->event_wq, &plat_priv->event_work);

	if (!sync)
		return ret;

	ret = wait_for_completion_interruptible(&event->complete);

	cnss_pr_dbg("Completed event: %s(%d), state: 0x%lx, ret: %d/%d\n",
		    cnss_driver_event_to_str(type), type,
		    plat_priv->driver_state, ret, event->ret);

	spin_lock_irqsave(&plat_priv->event_lock, flags);
	if (ret == -ERESTARTSYS && event->ret == CNSS_EVENT_PENDING) {
		event->sync = false;
		spin_unlock_irqrestore(&plat_priv->event_lock, flags);
		return ret;
	}
	spin_unlock_irqrestore(&plat_priv->event_lock, flags);

	ret = event->ret;
	kfree(event);

	return ret;
}

int cnss_power_up(struct device *dev)
{
	int ret = 0;
	void *bus_priv = cnss_bus_dev_to_bus_priv(dev);
	struct cnss_plat_data *plat_priv = cnss_bus_dev_to_plat_priv(dev);

	ret = cnss_power_on_device(plat_priv);
	if (ret) {
		cnss_pr_err("Failed to power on device, err = %d\n", ret);
		goto err_power_on;
	}

	ret = cnss_resume_pci_link(bus_priv);
	if (ret) {
		cnss_pr_err("Failed to resume PCI link, err = %d\n", ret);
		goto err_resume_link;
	}

	return 0;
err_resume_link:
	cnss_power_off_device(plat_priv);
err_power_on:
	return ret;
}
EXPORT_SYMBOL(cnss_power_up);

int cnss_power_down(struct device *dev)
{
	int ret;
	void *bus_priv = cnss_bus_dev_to_bus_priv(dev);
	struct cnss_plat_data *plat_priv = cnss_bus_dev_to_plat_priv(dev);

	if (!bus_priv || !plat_priv)
		return -ENODEV;

	cnss_request_bus_bandwidth(CNSS_BUS_WIDTH_NONE, plat_priv);
	cnss_pci_set_monitor_wake_intr(bus_priv, false);
	cnss_pci_set_auto_suspended(bus_priv, 0);

	ret = cnss_suspend_pci_link(bus_priv);
	if (ret)
		cnss_pr_err("Failed to suspend PCI link, err = %d\n", ret);

	cnss_power_off_device(plat_priv);

	return 0;
}
EXPORT_SYMBOL(cnss_power_down);

void cnss_unregister_qca8074_cb(struct cnss_plat_data *plat_priv)
{
	void *handler = plat_priv->esoc_info.modem_notify_handler;

	if (handler) {
		/* unregister atomic handler first */
		subsys_notif_unregister_atomic_notifier(handler,
			&plat_priv->modem_atomic_nb);
		subsys_notif_unregister_notifier(handler, &plat_priv->modem_nb);
		plat_priv->esoc_info.modem_notify_handler = NULL;
	}
}

static int cnss_qca8074_notifier_atomic_nb(struct notifier_block *nb,
	unsigned long code,
	void *ss_handle)
{
	struct cnss_plat_data *plat_priv =
		container_of(nb, struct cnss_plat_data, modem_atomic_nb);
	struct cnss_pci_data *pci_priv = plat_priv->bus_priv;
	struct cnss_wlan_driver *driver_ops;

	driver_ops = plat_priv->driver_ops;

	if (code == SUBSYS_PREPARE_FOR_FATAL_SHUTDOWN)
		driver_ops->fatal(plat_priv->plat_dev, plat_priv->plat_dev_id);

	return NOTIFY_OK;
}

static int cnss_qca8074_notifier_nb(struct notifier_block *nb,
				  unsigned long code,
				  void *ss_handle)
{
	struct cnss_plat_data *plat_priv =
		container_of(nb, struct cnss_plat_data, modem_nb);
	struct cnss_pci_data *pci_priv = plat_priv->bus_priv;
	struct cnss_wlan_driver *driver_ops;

	driver_ops = plat_priv->driver_ops;

	if (code == SUBSYS_AFTER_POWERUP) {
		driver_ops->probe(plat_priv->plat_dev, plat_priv->plat_dev_id);
	} else if (code == SUBSYS_BEFORE_SHUTDOWN) {
		driver_ops->remove(plat_priv->plat_dev);
	} else if (code == SUBSYS_RAMDUMP_NOTIFICATION) {
		driver_ops->reinit(plat_priv->plat_dev, plat_priv->plat_dev_id);
		return NOTIFY_DONE;
	}

	return NOTIFY_OK;
}

void *cnss_register_qca8074_cb(struct cnss_plat_data *plat_priv)
{
	struct cnss_subsys_info *subsys_info;
	void *ss_handle = NULL;

	subsys_info = &plat_priv->subsys_info;
	plat_priv->modem_nb.notifier_call = cnss_qca8074_notifier_nb;
	ss_handle = subsys_notif_register_notifier(
		subsys_info->subsys_desc.name, &plat_priv->modem_nb);
	/* register the atomic notifier as well */
	if (ss_handle) {
		plat_priv->modem_atomic_nb.notifier_call =
			cnss_qca8074_notifier_atomic_nb;
		subsys_notif_register_atomic_notifier(ss_handle,
			&plat_priv->modem_atomic_nb);
	}

	return ss_handle;
}

int cnss_wlan_register_driver(struct cnss_wlan_driver *driver_ops)
{
	int ret, i;
	struct cnss_plat_data *plat_priv;
	struct cnss_subsys_info *subsys_info;
	struct cnss_esoc_info *esoc_info;

	for (i = 0; i < plat_env_index; i++) {
		plat_priv = plat_env[i];

		if (!plat_priv)
			continue;

		if (plat_priv->device_id != QCA8074_DEVICE_ID ||
				strncmp(driver_ops->name, "pld_ahb", 7) != 0)
			continue;

		if (!plat_priv) {
			cnss_pr_err("plat_priv is NULL!\n");
			ret = -ENODEV;
			goto out;
		}

		if (plat_priv->driver_ops) {
			cnss_pr_err("Driver has already registered!\n");
			ret = -EEXIST;
			goto out;
		}

		plat_priv->driver_status = CNSS_LOAD_UNLOAD;
		plat_priv->driver_ops = driver_ops;

		if (plat_priv->device_id == QCA8074_DEVICE_ID) {
			cnss_register_subsys(plat_priv);
			esoc_info = &plat_priv->esoc_info;
			esoc_info->modem_notify_handler =
					cnss_register_qca8074_cb(plat_priv);
		}

		subsys_info = &plat_priv->subsys_info;
		subsys_info->subsys_handle =
			subsystem_get(subsys_info->subsys_desc.name);
		if (!subsys_info->subsys_handle) {
			ret = -EINVAL;
			goto reset_ctx;
		} else if (IS_ERR(subsys_info->subsys_handle)) {
			ret = PTR_ERR(subsys_info->subsys_handle);
			goto reset_ctx;
		}

		plat_priv->driver_status = CNSS_INITIALIZED;
	}

	return 0;
reset_ctx:
	cnss_pr_err("Failed to get subsystem, err = %d\n", ret);
	plat_priv->driver_status = CNSS_UNINITIALIZED;
	plat_priv->driver_ops = NULL;
out:
	return ret;
}
EXPORT_SYMBOL(cnss_wlan_register_driver);

void cnss_wlan_unregister_driver(struct cnss_wlan_driver *driver_ops)
{
	struct cnss_plat_data *plat_priv;
	struct cnss_subsys_info *subsys_info;
	struct cnss_wlan_driver *ops;
	int i;

	for (i = 0; i < plat_env_index; i++) {
		plat_priv = plat_env[i];

		if (!plat_priv) {
			cnss_pr_err("plat_priv is NULL!\n");
			return;
		}

		if (plat_priv->device_id != QCA8074_DEVICE_ID ||
				strncmp(driver_ops->name, "pld_ahb", 7) != 0)
			continue;
		plat_priv->driver_status = CNSS_LOAD_UNLOAD;
		ops = plat_priv->driver_ops;

		subsys_info = &plat_priv->subsys_info;
		if (subsys_info->subsys_handle &&
		    !subsys_info->subsystem_put_in_progress) {
			subsys_info->subsystem_put_in_progress = true;
			subsystem_put(subsys_info->subsys_handle);
			subsys_info->subsystem_put_in_progress = false;
		} else {
			ops->remove(plat_priv->plat_dev);
		}

		subsys_info->subsys_handle = NULL;
		cnss_unregister_qca8074_cb(plat_priv);
		plat_priv->driver_ops = NULL;
		plat_priv->driver_status = CNSS_UNINITIALIZED;
	}
}
EXPORT_SYMBOL(cnss_wlan_unregister_driver);

void  *cnss_subsystem_get(struct device *dev)
{
	struct cnss_plat_data *plat_priv;
	struct cnss_subsys_info *subsys_info;
	plat_priv = cnss_get_plat_priv_by_device_id(QCA8074_DEVICE_ID);

	if (!plat_priv) {
		return NULL;
	}
	subsys_info = &plat_priv->subsys_info;

	if (subsys_info->subsys_handle) {
		printk("Error: subsys handle is not NULL");
		return NULL;
	}
	subsys_info->subsys_handle =
				subsystem_get(subsys_info->subsys_desc.name);
	return subsys_info->subsys_handle;
}
EXPORT_SYMBOL(cnss_subsystem_get);

void cnss_subsystem_put(struct device *dev)
{
	struct cnss_plat_data *plat_priv;
	struct cnss_subsys_info *subsys_info;

	plat_priv = cnss_get_plat_priv_by_device_id(QCA8074_DEVICE_ID);

	if (!plat_priv)
		return;

	subsys_info = &plat_priv->subsys_info;
	if (subsys_info->subsys_handle &&
	    !subsys_info->subsystem_put_in_progress) {
		subsys_info->subsystem_put_in_progress = true;
		subsystem_put(subsys_info->subsys_handle);
		subsys_info->subsystem_put_in_progress = false;
		subsys_info->subsys_handle = NULL;
	}
}
EXPORT_SYMBOL(cnss_subsystem_put);

static int cnss_get_resources(struct cnss_plat_data *plat_priv)
{
	int ret;

	ret = cnss_get_vreg(plat_priv);
	if (ret) {
		cnss_pr_err("Failed to get vreg, err = %d\n", ret);
		goto out;
	}

	ret = cnss_get_pinctrl(plat_priv);
	if (ret) {
		cnss_pr_err("Failed to get pinctrl, err = %d\n", ret);
		goto out;
	}

	return 0;
out:
	return ret;
}

static void cnss_put_resources(struct cnss_plat_data *plat_priv)
{
}

static int cnss_modem_notifier_nb(struct notifier_block *nb,
				  unsigned long code,
				  void *ss_handle)
{
	struct cnss_plat_data *plat_priv =
		container_of(nb, struct cnss_plat_data, modem_nb);
	struct cnss_pci_data *pci_priv = plat_priv->bus_priv;
	struct cnss_esoc_info *esoc_info;
	struct cnss_wlan_driver *driver_ops;

	cnss_pr_dbg("Modem notifier: event %lu\n", code);

	if (!pci_priv)
		return NOTIFY_DONE;

	esoc_info = &plat_priv->esoc_info;

	if (SUBSYS_AFTER_POWERUP == code)
		esoc_info->modem_current_status = 1;
	else if (SUBSYS_BEFORE_SHUTDOWN == code)
		esoc_info->modem_current_status = 0;
	else
		return NOTIFY_DONE;

	driver_ops = plat_priv->driver_ops;
	if (!driver_ops || !driver_ops->modem_status)
		return NOTIFY_DONE;

	driver_ops->modem_status(pci_priv->pci_dev,
				 esoc_info->modem_current_status);

	return NOTIFY_OK;
}

static int cnss_register_esoc(struct cnss_plat_data *plat_priv)
{
	int ret = 0;
	struct device *dev;
	struct cnss_esoc_info *esoc_info;
	struct esoc_desc *esoc_desc;
	const char *client_desc;

	dev = &plat_priv->plat_dev->dev;
	esoc_info = &plat_priv->esoc_info;

	esoc_info->notify_modem_status =
		of_property_read_bool(dev->of_node,
				      "qcom,notify-modem-status");

	if (esoc_info->notify_modem_status)
		goto out;

	ret = of_property_read_string_index(dev->of_node, "esoc-names", 0,
					    &client_desc);
	if (ret) {
		cnss_pr_dbg("esoc-names is not defined in DT, skip!\n");
	} else {
		esoc_desc = devm_register_esoc_client(dev, client_desc);
		if (IS_ERR_OR_NULL(esoc_desc)) {
			ret = PTR_RET(esoc_desc);
			cnss_pr_err("Failed to register esoc_desc, err = %d\n",
				    ret);
			goto out;
		}
		esoc_info->esoc_desc = esoc_desc;
	}

	plat_priv->modem_nb.notifier_call = cnss_modem_notifier_nb;
	esoc_info->modem_current_status = 0;
	esoc_info->modem_notify_handler =
		subsys_notif_register_notifier(esoc_info->esoc_desc ?
					       esoc_info->esoc_desc->name :
					       "modem", &plat_priv->modem_nb);
	if (IS_ERR(esoc_info->modem_notify_handler)) {
		ret = PTR_ERR(esoc_info->modem_notify_handler);
		cnss_pr_err("Failed to register esoc notifier, err = %d\n",
			    ret);
		goto unreg_esoc;
	}

	return 0;
unreg_esoc:
	if (esoc_info->esoc_desc)
		devm_unregister_esoc_client(dev, esoc_info->esoc_desc);
out:
	return ret;
}

static void cnss_unregister_esoc(struct cnss_plat_data *plat_priv)
{
	struct device *dev;
	struct cnss_esoc_info *esoc_info;

	dev = &plat_priv->plat_dev->dev;
	esoc_info = &plat_priv->esoc_info;

	if (esoc_info->notify_modem_status)
		subsys_notif_unregister_notifier(esoc_info->
						 modem_notify_handler,
						 &plat_priv->modem_nb);
	if (esoc_info->esoc_desc)
		devm_unregister_esoc_client(dev, esoc_info->esoc_desc);
}

static int cnss_qca6174_powerup(struct cnss_plat_data *plat_priv)
{
	int ret = 0;
	struct cnss_pci_data *pci_priv = plat_priv->bus_priv;

	if (!pci_priv) {
		cnss_pr_err("pci_priv is NULL!\n");
		return -ENODEV;
	}

	if (!plat_priv->driver_ops) {
		cnss_pr_err("driver_ops is NULL!\n");
		return -EINVAL;
	}

	ret = cnss_power_on_device(plat_priv);
	if (ret) {
		cnss_pr_err("Failed to power on device, err = %d\n", ret);
		goto out;
	}

	ret = cnss_resume_pci_link(pci_priv);
	if (ret) {
		cnss_pr_err("Failed to resume PCI link, err = %d\n", ret);
		goto power_off;
	}

	if (plat_priv->driver_status == CNSS_LOAD_UNLOAD) {
		ret = plat_priv->driver_ops->probe(pci_priv->pci_dev,
						   pci_priv->pci_device_id);
		if (ret) {
			cnss_pr_err("Failed to probe host driver, err = %d\n",
				    ret);
			goto suspend_link;
		}
	} else if (plat_priv->driver_status == CNSS_RECOVERY) {
		ret = plat_priv->driver_ops->reinit(pci_priv->pci_dev,
						    pci_priv->pci_device_id);
		if (ret) {
			cnss_pr_err("Failed to reinit host driver, err = %d\n",
				    ret);
			goto suspend_link;
		}
		plat_priv->driver_status = CNSS_INITIALIZED;
	} else {
		cnss_pr_err("Driver state is not correct to power up!\n");
		ret = -EINVAL;
		goto suspend_link;
	}

	return 0;
suspend_link:
	cnss_suspend_pci_link(pci_priv);
power_off:
	cnss_power_off_device(plat_priv);
out:
	return ret;
}

static int cnss_qca6174_shutdown(struct cnss_plat_data *plat_priv)
{
	int ret = 0;
	struct cnss_pci_data *pci_priv = plat_priv->bus_priv;

	if (!pci_priv)
		return -ENODEV;

	if (!plat_priv->driver_ops)
		return -EINVAL;

	if (plat_priv->driver_status == CNSS_LOAD_UNLOAD) {
		cnss_request_bus_bandwidth(CNSS_BUS_WIDTH_NONE, plat_priv);
		plat_priv->driver_ops->remove(pci_priv->pci_dev);
		cnss_pci_set_monitor_wake_intr(pci_priv, false);
		cnss_pci_set_auto_suspended(pci_priv, 0);
	} else {
		plat_priv->driver_status = CNSS_RECOVERY;
		plat_priv->driver_ops->shutdown(pci_priv->pci_dev);
	}

	ret = cnss_suspend_pci_link(pci_priv);
	if (ret)
		cnss_pr_err("Failed to suspend PCI link, err = %d\n", ret);

	cnss_power_off_device(plat_priv);

	return ret;
}

static void cnss_qca6174_crash_shutdown(struct cnss_plat_data *plat_priv)
{
	struct cnss_pci_data *pci_priv = plat_priv->bus_priv;

	if (!plat_priv->driver_ops)
		return;

	plat_priv->driver_ops->crash_shutdown(pci_priv->pci_dev);
}

static int cnss_qca6290_powerup(struct cnss_plat_data *plat_priv)
{
	int ret = 0;
	struct cnss_pci_data *pci_priv = plat_priv->bus_priv;

	if (!pci_priv) {
		cnss_pr_err("pci_priv is NULL!\n");
		return -ENODEV;
	}

	if (!plat_priv->driver_ops) {
		cnss_pr_err("driver_ops is NULL!\n");
		return -EINVAL;
	}

	ret = cnss_power_on_device(plat_priv);
	if (ret) {
		cnss_pr_err("Failed to power on device, err = %d\n", ret);
		goto out;
	}

	ret = cnss_resume_pci_link(pci_priv);
	if (ret) {
		cnss_pr_err("Failed to resume PCI link, err = %d\n", ret);
		goto power_off;
	}

	if (fbc_bypass)
		goto bypass_fbc;

	ret = cnss_pci_start_mhi(pci_priv);
	if (ret) {
		cnss_pr_err("Failed to start MHI, err = %d\n", ret);
		goto suspend_link;
	}

bypass_fbc:
	if (qmi_bypass)
		goto skip_fw_ready;

	reinit_completion(&plat_priv->fw_ready_event);
	ret = wait_for_completion_timeout(&plat_priv->fw_ready_event,
					  msecs_to_jiffies(FW_READY_TIMEOUT));

	if (ret == 0) {
		cnss_pr_err("Timeout for FW ready event!\n");
		ret = -EBUSY;
		goto stop_mhi;
	}

skip_fw_ready:
	if (enable_waltest) {
		cnss_pr_dbg("Firmware waltest is enabled.\n");
		ret = 0;
		goto out;
	}

	if (plat_priv->driver_status == CNSS_LOAD_UNLOAD) {
		ret = plat_priv->driver_ops->probe(pci_priv->pci_dev,
						   pci_priv->pci_device_id);
		if (ret) {
			cnss_pr_err("Failed to probe host driver, err = %d\n",
				    ret);
			goto stop_mhi;
		}
	} else if (plat_priv->driver_status == CNSS_RECOVERY) {
		ret = plat_priv->driver_ops->reinit(pci_priv->pci_dev,
						    pci_priv->pci_device_id);
		if (ret) {
			cnss_pr_err("Failed to reinit host driver, err = %d\n",
				    ret);
			goto stop_mhi;
		}
		plat_priv->driver_status = CNSS_INITIALIZED;
	} else {
		cnss_pr_err("Driver state is not correct to power up!\n");
		ret = -EINVAL;
		goto stop_mhi;
	}

	set_bit(CNSS_DRIVER_PROBED, &plat_priv->driver_state);

	return 0;

stop_mhi:
	if (!fbc_bypass)
		cnss_pci_stop_mhi(pci_priv);
suspend_link:
	cnss_suspend_pci_link(pci_priv);
power_off:
	cnss_power_off_device(plat_priv);
out:
	return ret;
}

static int cnss_qca6290_shutdown(struct cnss_plat_data *plat_priv)
{
	int ret = 0;
	struct cnss_pci_data *pci_priv = plat_priv->bus_priv;

	if (!pci_priv)
		return -ENODEV;

	if (!plat_priv->driver_ops)
		return -EINVAL;

	if (enable_waltest)
		goto bypass_driver_remove;

	if (plat_priv->driver_status == CNSS_LOAD_UNLOAD) {
		cnss_request_bus_bandwidth(CNSS_BUS_WIDTH_NONE, plat_priv);
		plat_priv->driver_ops->remove(pci_priv->pci_dev);
		cnss_pci_set_monitor_wake_intr(pci_priv, false);
		cnss_pci_set_auto_suspended(pci_priv, 0);
	} else {
		plat_priv->driver_status = CNSS_RECOVERY;
		plat_priv->driver_ops->shutdown(pci_priv->pci_dev);
	}

	clear_bit(CNSS_DRIVER_PROBED, &plat_priv->driver_state);

bypass_driver_remove:
	if (!fbc_bypass)
		cnss_pci_stop_mhi(pci_priv);

	ret = cnss_suspend_pci_link(pci_priv);
	if (ret)
		cnss_pr_err("Failed to suspend PCI link, err = %d\n", ret);

	cnss_power_off_device(plat_priv);

	return ret;
}

static int cnss_powerup(const struct subsys_desc *subsys_desc)
{
	int ret = 0;
	struct cnss_plat_data *plat_priv = dev_get_drvdata(subsys_desc->dev);

	if (!plat_priv) {
		cnss_pr_err("plat_priv is NULL!\n");
		return -ENODEV;
	}

	switch (plat_priv->device_id) {
	case QCA6174_DEVICE_ID:
		ret = cnss_qca6174_powerup(plat_priv);
		break;
	case QCA6290_DEVICE_ID:
		ret = cnss_qca6290_powerup(plat_priv);
		break;
	default:
		cnss_pr_err("Unknown device_id found: 0x%lx\n",
			    plat_priv->device_id);
		ret = -ENODEV;
	}

	return ret;
}

static int cnss_shutdown(const struct subsys_desc *subsys_desc, bool force_stop)
{
	int ret = 0;
	struct cnss_plat_data *plat_priv = dev_get_drvdata(subsys_desc->dev);

	if (!plat_priv) {
		cnss_pr_err("plat_priv is NULL!\n");
		return -ENODEV;
	}

	switch (plat_priv->device_id) {
	case QCA6174_DEVICE_ID:
		ret = cnss_qca6174_shutdown(plat_priv);
		break;
	case QCA6290_DEVICE_ID:
		ret = cnss_qca6290_shutdown(plat_priv);
		break;
	default:
		cnss_pr_err("Unknown device_id found: 0x%lx\n",
			    plat_priv->device_id);
		ret = -ENODEV;
	}

	return ret;
}

static int cnss_ramdump(int enable, const struct subsys_desc *subsys_desc)
{
	int ret = 0;
	struct cnss_plat_data *plat_priv = dev_get_drvdata(subsys_desc->dev);
	struct cnss_ramdump_info *ramdump_info;
	struct ramdump_segment segment;

	if (!plat_priv) {
		cnss_pr_err("plat_priv is NULL!\n");
		return -ENODEV;
	}

	ramdump_info = &plat_priv->ramdump_info;
	if (!ramdump_info->ramdump_size)
		return -EINVAL;

	if (!enable)
		return 0;

	switch (plat_priv->device_id) {
	case QCA6174_DEVICE_ID:
		memset(&segment, 0, sizeof(segment));
		segment.v_address = ramdump_info->ramdump_va;
		segment.size = ramdump_info->ramdump_size;
		ret = do_ramdump(ramdump_info->ramdump_dev, &segment, 1);
		break;
	case QCA6290_DEVICE_ID:
		break;
	default:
		cnss_pr_err("Unknown device_id found: 0x%lx\n",
			    plat_priv->device_id);
		ret = -ENODEV;
	}

	return ret;
}

void *cnss_get_virt_ramdump_mem(unsigned long *size)
{
	struct cnss_plat_data *plat_priv = cnss_bus_dev_to_plat_priv(NULL);
	struct cnss_ramdump_info *ramdump_info;

	if (!plat_priv)
		return NULL;

	ramdump_info = &plat_priv->ramdump_info;
	*size = ramdump_info->ramdump_size;

	return ramdump_info->ramdump_va;
}
EXPORT_SYMBOL(cnss_get_virt_ramdump_mem);

void cnss_device_crashed(void)
{
	struct cnss_plat_data *plat_priv = cnss_bus_dev_to_plat_priv(NULL);
	struct cnss_subsys_info *subsys_info;

	if (!plat_priv)
		return;

	subsys_info = &plat_priv->subsys_info;
	if (subsys_info->subsys_device) {
		subsys_set_crash_status(subsys_info->subsys_device, true);
		subsystem_restart_dev(subsys_info->subsys_device);
	}
}
EXPORT_SYMBOL(cnss_device_crashed);

static void cnss_crash_shutdown(const struct subsys_desc *subsys_desc)
{
	struct cnss_plat_data *plat_priv = dev_get_drvdata(subsys_desc->dev);

	if (!plat_priv) {
		cnss_pr_err("plat_priv is NULL!\n");
		return;
	}

	switch (plat_priv->device_id) {
	case QCA6174_DEVICE_ID:
		cnss_qca6174_crash_shutdown(plat_priv);
		break;
	case QCA6290_DEVICE_ID:
		break;
	default:
		cnss_pr_err("Unknown device_id found: 0x%lx\n",
			    plat_priv->device_id);
	}
}

int cnss_self_recovery(struct device *dev,
		       enum cnss_recovery_reason reason)
{
	struct cnss_plat_data *plat_priv = cnss_bus_dev_to_plat_priv(dev);
	struct cnss_subsys_info *subsys_info;

	if (!plat_priv) {
		cnss_pr_err("plat_priv is NULL!\n");
		return -EINVAL;
	}

	if (!plat_priv->plat_dev) {
		cnss_pr_err("plat_dev is NULL!\n");
		return -EINVAL;
	}

	if (!plat_priv->driver_ops) {
		cnss_pr_err("Driver is not registered yet!\n");
		return -EINVAL;
	}

	if (plat_priv->driver_status == CNSS_RECOVERY) {
		cnss_pr_err("Recovery is already in progress!\n");
		return -EINVAL;
	}

	if (plat_priv->driver_status == CNSS_LOAD_UNLOAD) {
		cnss_pr_err("Driver load or unload is in progress!\n");
		return -EINVAL;
	}

	subsys_info = &plat_priv->subsys_info;
	plat_priv->recovery_count++;
	plat_priv->driver_status = CNSS_RECOVERY;
#if CONFIG_CNSS2_PM
	pm_stay_awake(dev);
#endif
	cnss_shutdown(&subsys_info->subsys_desc, false);
	udelay(WLAN_RECOVERY_DELAY);
	cnss_powerup(&subsys_info->subsys_desc);
#if CONFIG_CNSS2_PM
	pm_relax(dev);
#endif
	plat_priv->driver_status = CNSS_INITIALIZED;

	return 0;
}
EXPORT_SYMBOL(cnss_self_recovery);

void cnss_schedule_recovery(struct device *dev,
			    enum cnss_recovery_reason reason)
{
	struct cnss_plat_data *plat_priv = cnss_bus_dev_to_plat_priv(dev);
	struct cnss_recovery_work_t *work = &plat_priv->cnss_recovery_work;

	work->dev = dev;
	work->reason = reason;
	queue_work(plat_priv->event_wq, &work->work);
}
EXPORT_SYMBOL(cnss_schedule_recovery);

int cnss_register_subsys(struct cnss_plat_data *plat_priv)
{
	int ret = 0;
	struct cnss_subsys_info *subsys_info;

	subsys_info = &plat_priv->subsys_info;

	switch (plat_priv->device_id) {
	case QCA6174_DEVICE_ID:
		subsys_info->subsys_desc.name = "AR6320";
		break;
	case QCA6290_DEVICE_ID:
		subsys_info->subsys_desc.name = "QCA6290";
		break;
	case QCA8074_DEVICE_ID:
		subsys_info->subsys_desc.name = "q6v5-wcss";
		return 0;
	default:
		cnss_pr_err("Unknown device ID: 0x%lx\n", plat_priv->device_id);
		ret = -ENODEV;
		goto out;
	}

	subsys_info->subsys_desc.owner = THIS_MODULE;
	subsys_info->subsys_desc.powerup = cnss_powerup;
	subsys_info->subsys_desc.shutdown = cnss_shutdown;
	subsys_info->subsys_desc.ramdump = cnss_ramdump;
	subsys_info->subsys_desc.crash_shutdown = cnss_crash_shutdown;
	subsys_info->subsys_desc.dev = &plat_priv->plat_dev->dev;

	subsys_info->subsys_device = subsys_register(&subsys_info->subsys_desc);
	if (IS_ERR(subsys_info->subsys_device)) {
		ret = PTR_ERR(subsys_info->subsys_device);
		cnss_pr_err("Failed to register subsys, err = %d\n", ret);
		goto out;
	}

	return 0;
out:
	return ret;
}

void cnss_unregister_subsys(struct cnss_plat_data *plat_priv)
{
	struct cnss_subsys_info *subsys_info;

	if (plat_priv->device_id == QCA8074_DEVICE_ID)
		return;

	subsys_info = &plat_priv->subsys_info;
	subsys_unregister(subsys_info->subsys_device);
}

static int cnss_init_dump_entry(struct cnss_plat_data *plat_priv)
{
	struct cnss_ramdump_info *ramdump_info;
	struct msm_dump_entry dump_entry;

	ramdump_info = &plat_priv->ramdump_info;
	ramdump_info->dump_data.addr = ramdump_info->ramdump_pa;
	ramdump_info->dump_data.len = ramdump_info->ramdump_size;
	ramdump_info->dump_data.version = CNSS_DUMP_FORMAT_VER;
	ramdump_info->dump_data.magic = CNSS_DUMP_MAGIC_VER_V2;
	strlcpy(ramdump_info->dump_data.name, CNSS_DUMP_NAME,
		sizeof(ramdump_info->dump_data.name));
	dump_entry.id = MSM_DUMP_DATA_CNSS_WLAN;
	dump_entry.addr = virt_to_phys(&ramdump_info->dump_data);

	return msm_dump_data_register(MSM_DUMP_TABLE_APPS, &dump_entry);
}

static int cnss_qca6174_register_ramdump(struct cnss_plat_data *plat_priv)
{
	int ret = 0;
	struct device *dev;
	struct cnss_subsys_info *subsys_info;
	struct cnss_ramdump_info *ramdump_info;
	u32 ramdump_size = 0;

	dev = &plat_priv->plat_dev->dev;
	subsys_info = &plat_priv->subsys_info;
	ramdump_info = &plat_priv->ramdump_info;

	if (of_property_read_u32(dev->of_node, "qcom,wlan-ramdump-dynamic",
				 &ramdump_size) == 0) {
		ramdump_info->ramdump_va = dma_alloc_coherent(dev, ramdump_size,
			&ramdump_info->ramdump_pa, GFP_KERNEL);

		if (ramdump_info->ramdump_va)
			ramdump_info->ramdump_size = ramdump_size;
	}

	cnss_pr_dbg("ramdump va: %pK, pa: %pa\n",
		    ramdump_info->ramdump_va, &ramdump_info->ramdump_pa);

	if (ramdump_info->ramdump_size == 0) {
		cnss_pr_info("Ramdump will not be collected");
		goto out;
	}

	ret = cnss_init_dump_entry(plat_priv);
	if (ret < 0) {
		cnss_pr_err("Failed to setup dump table, err = %d\n", ret);
		goto free_ramdump;
	}

	ramdump_info->ramdump_dev = create_ramdump_device(
		subsys_info->subsys_desc.name, subsys_info->subsys_desc.dev);
	if (!ramdump_info->ramdump_dev) {
		cnss_pr_err("Failed to create ramdump device!");
		ret = -ENOMEM;
		goto free_ramdump;
	}

	return 0;
free_ramdump:
	dma_free_coherent(dev, ramdump_info->ramdump_size,
			  ramdump_info->ramdump_va, ramdump_info->ramdump_pa);
out:
	return ret;
}

static void cnss_qca6174_unregister_ramdump(struct cnss_plat_data *plat_priv)
{
	struct device *dev;
	struct cnss_ramdump_info *ramdump_info;

	dev = &plat_priv->plat_dev->dev;
	ramdump_info = &plat_priv->ramdump_info;

	if (ramdump_info->ramdump_dev)
		destroy_ramdump_device(ramdump_info->ramdump_dev);

	if (ramdump_info->ramdump_va)
		dma_free_coherent(dev, ramdump_info->ramdump_size,
				  ramdump_info->ramdump_va,
				  ramdump_info->ramdump_pa);
}

int cnss_register_ramdump(struct cnss_plat_data *plat_priv)
{
	int ret = 0;

	switch (plat_priv->device_id) {
	case QCA6174_DEVICE_ID:
		ret = cnss_qca6174_register_ramdump(plat_priv);
		break;
	case QCA6290_DEVICE_ID:
		break;
	default:
		cnss_pr_err("Unknown device ID: 0x%lx\n", plat_priv->device_id);
		ret = -ENODEV;
		break;
	}
	return ret;
}

void cnss_unregister_ramdump(struct cnss_plat_data *plat_priv)
{
	switch (plat_priv->device_id) {
	case QCA6174_DEVICE_ID:
		cnss_qca6174_unregister_ramdump(plat_priv);
		break;
	case QCA6290_DEVICE_ID:
		break;
	default:
		cnss_pr_err("Unknown device ID: 0x%lx\n", plat_priv->device_id);
		break;
	}
}

static int cnss_register_bus_scale(struct cnss_plat_data *plat_priv)
{
	int ret = 0;
	struct cnss_bus_bw_info *bus_bw_info;

	bus_bw_info = &plat_priv->bus_bw_info;

	bus_bw_info->bus_scale_table =
		msm_bus_cl_get_pdata(plat_priv->plat_dev);
	if (bus_bw_info->bus_scale_table)  {
		bus_bw_info->bus_client =
			msm_bus_scale_register_client(
				bus_bw_info->bus_scale_table);
		if (!bus_bw_info->bus_client) {
			cnss_pr_err("Failed to register bus scale client!\n");
			ret = -EINVAL;
			goto out;
		}
	}

	return 0;
out:
	return ret;
}

static void cnss_unregister_bus_scale(struct cnss_plat_data *plat_priv)
{
	struct cnss_bus_bw_info *bus_bw_info;

	bus_bw_info = &plat_priv->bus_bw_info;

	if (bus_bw_info->bus_client)
		msm_bus_scale_unregister_client(bus_bw_info->bus_client);
}

static ssize_t cnss_fs_ready_store(struct device *dev,
				   struct device_attribute *attr,
				   const char *buf,
				   size_t count)
{
	int fs_ready = 0;
	struct cnss_plat_data *plat_priv = dev_get_drvdata(dev);

	if (sscanf(buf, "%du", &fs_ready) != 1)
		return -EINVAL;

	cnss_pr_dbg("File system is ready, fs_ready is %d, count is %zu\n",
		    fs_ready, count);

	if (qmi_bypass) {
		cnss_pr_dbg("QMI is bypassed.\n");
		return count;
	}

	if (!plat_priv) {
		cnss_pr_err("plat_priv is NULL!\n");
		return count;
	}

	switch (plat_priv->device_id) {
	case QCA6290_DEVICE_ID:
		break;
	default:
		cnss_pr_err("Not supported for device ID 0x%lx\n",
			    plat_priv->device_id);
		return count;
	}

	if (fs_ready == FILE_SYSTEM_READY)
		cnss_pci_start_mhi(plat_priv->bus_priv);

	return count;
}

static DEVICE_ATTR(fs_ready, S_IWUSR | S_IWGRP, NULL, cnss_fs_ready_store);

static int cnss_create_sysfs(struct cnss_plat_data *plat_priv)
{
	int ret = 0;

	ret = device_create_file(&plat_priv->plat_dev->dev, &dev_attr_fs_ready);
	if (ret) {
		cnss_pr_err("Failed to create device file, err = %d\n", ret);
		goto out;
	}

	return 0;
out:
	return ret;
}

static void cnss_remove_sysfs(struct cnss_plat_data *plat_priv)
{
	device_remove_file(&plat_priv->plat_dev->dev, &dev_attr_fs_ready);
}

static int cnss_event_work_init(struct cnss_plat_data *plat_priv)
{
	spin_lock_init(&plat_priv->event_lock);
	plat_priv->event_wq = alloc_workqueue("cnss_driver_event",
					      0, 0);
	if (!plat_priv->event_wq) {
		cnss_pr_err("Failed to create event workqueue!\n");
		return -EFAULT;
	}

	INIT_WORK(&plat_priv->event_work, cnss_driver_event_work);
	INIT_LIST_HEAD(&plat_priv->event_list);
	INIT_WORK(&plat_priv->cnss_recovery_work.work,
		  cnss_recovery_work_func);
	init_completion(&plat_priv->fw_ready_event);

	return 0;
}

static void cnss_event_work_deinit(struct cnss_plat_data *plat_priv)
{
	complete_all(&plat_priv->fw_ready_event);
	destroy_workqueue(plat_priv->event_wq);
}

static const struct platform_device_id cnss_platform_id_table[] = {
	{ .name = "qca6174", .driver_data = QCA6174_DEVICE_ID, },
	{ .name = "qca6290", .driver_data = QCA6290_DEVICE_ID, },
	{ .name = "qca8074", .driver_data = QCA8074_DEVICE_ID, },
};

static const struct of_device_id cnss_of_match_table[] = {
	{
		.compatible = "qcom,cnss",
		.data = (void *)&cnss_platform_id_table[0]},
	{
		.compatible = "qcom,cnss-qca6290",
		.data = (void *)&cnss_platform_id_table[1]},
	{
		.compatible = "qcom,cnss-qca8074",
		.data = (void *)&cnss_platform_id_table[2]},
	{ },
};
MODULE_DEVICE_TABLE(of, cnss_of_match_table);

static int cnss_probe(struct platform_device *plat_dev)
{
	int ret = 0;
	struct cnss_plat_data *plat_priv;
	const struct of_device_id *of_id;
	const struct platform_device_id *device_id;

	if (cnss_get_plat_priv(plat_dev)) {
		cnss_pr_err("Driver is already initialized!\n");
		ret = -EEXIST;
		goto out;
	}
	of_id = of_match_device(cnss_of_match_table, &plat_dev->dev);
	if (!of_id || !of_id->data) {
		cnss_pr_err("Failed to find of match device!\n");
		ret = -ENODEV;
		goto out;
	}

	device_id = of_id->data;

	plat_priv = devm_kzalloc(&plat_dev->dev, sizeof(*plat_priv),
				 GFP_KERNEL);
	if (!plat_priv) {
		ret = -ENOMEM;
		goto out;
	}

	plat_priv->plat_dev = plat_dev;
	plat_priv->device_id = device_id->driver_data;
	plat_priv->plat_dev_id = device_id;

	switch(plat_priv->device_id) {
		case QCA6290_DEVICE_ID:
			wlfw_service_instance_id = WLFW_SERVICE_INS_ID_V01_QCA6290;
			break;
		case QCA8074_DEVICE_ID:
			wlfw_service_instance_id = WLFW_SERVICE_INS_ID_V01_QCA8074;
			break;
		default:
			cnss_pr_err("No such device id %d\n",device_id);
			return -ENODEV;
	}
	cnss_set_plat_priv(plat_dev, plat_priv);
	platform_set_drvdata(plat_dev, plat_priv);

#ifdef CONFIG_CNSS2_PM
	ret = cnss_get_resources(plat_priv);
	if (ret)
		goto reset_ctx;

	ret = cnss_power_on_device(plat_priv);
	if (ret)
		goto free_res;
#endif


#ifdef CONFIG_CNSS2_PCI_DRIVER
	ret = cnss_pci_init(plat_priv);
	if (ret)
		goto power_off;
#endif

#ifdef CONFIG_CNSS2_PM
	ret = cnss_register_esoc(plat_priv);
	if (ret)
		goto deinit_pci;

	ret = cnss_register_bus_scale(plat_priv);
	if (ret)
		goto unreg_esoc;
#endif

	ret = cnss_create_sysfs(plat_priv);
	if (ret)
		goto unreg_bus_scale;

	ret = cnss_event_work_init(plat_priv);
	if (ret)
		goto remove_sysfs;

	ret = cnss_qmi_init(plat_priv);
	if (ret)
		goto deinit_event_work;
#ifdef CONFIG_CNSS2_PM
	register_pm_notifier(&cnss_pm_notifier);
#endif

	cnss_pr_info("Platform driver probed successfully.\n");

	return 0;

deinit_event_work:
	cnss_event_work_deinit(plat_priv);
remove_sysfs:
	cnss_remove_sysfs(plat_priv);
unreg_bus_scale:
	cnss_unregister_bus_scale(plat_priv);
unreg_esoc:
	cnss_unregister_esoc(plat_priv);
deinit_pci:
	cnss_pci_deinit(plat_priv);
power_off:
	cnss_power_off_device(plat_priv);
free_res:
	cnss_put_resources(plat_priv);
reset_ctx:
	platform_set_drvdata(plat_dev, NULL);
	cnss_set_plat_priv(plat_dev, NULL);
out:
	return ret;
}

static int cnss_remove(struct platform_device *plat_dev)
{
	struct cnss_plat_data *plat_priv = platform_get_drvdata(plat_dev);
#ifdef CONFIG_CNSS2_PM
	unregister_pm_notifier(&cnss_pm_notifier);
#endif
	cnss_qmi_deinit(plat_priv);
	cnss_event_work_deinit(plat_priv);
	cnss_remove_sysfs(plat_priv);
	cnss_unregister_bus_scale(plat_priv);
	cnss_unregister_esoc(plat_priv);
	cnss_pci_deinit(plat_priv);
	cnss_put_resources(plat_priv);
	platform_set_drvdata(plat_dev, NULL);

	return 0;
}

static struct platform_driver cnss_platform_driver = {
	.probe  = cnss_probe,
	.remove = cnss_remove,
	.driver = {
		.name = "cnss2",
		.owner = THIS_MODULE,
		.of_match_table = cnss_of_match_table,
	},
};

static int __init cnss_initialize(void)
{
	int ret = 0;

	cnss_debug_init();
	ret = platform_driver_register(&cnss_platform_driver);
	if (ret)
		cnss_debug_deinit();

	return ret;
}

static void __exit cnss_exit(void)
{
	platform_driver_unregister(&cnss_platform_driver);
	cnss_debug_deinit();
}

module_init(cnss_initialize);
module_exit(cnss_exit);

MODULE_LICENSE("GPL v2");
MODULE_DESCRIPTION("CNSS2 Platform Driver");
