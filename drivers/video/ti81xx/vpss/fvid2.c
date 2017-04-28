/*
 * linux/drivers/video/ti81xx/vpss/fvid2.c
 *
 * VPSS FVID2 driver for TI 81XX
 *
 * Copyright (C) 2009 TI
 * Author: Yihe Hu <yihehu@ti.com>
 *
 * This program is free software; you can redistribute it and/or modify it
 * under the terms of the GNU General Public License as published by the
 * Free Software Foundation; either version 2 of the License, or (at your
 * option) any later version.
 *
 * This program is distributed in the hope that it will be useful, but
 * WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License along
 * with this program; if not, write to the Free Software Foundation, Inc.,
 * 59 Temple Place - Suite 330, Boston, MA	02111-1307, USA.
 */
#define VPSS_SUBMODULE_NAME "FVID2"

#include <linux/module.h>
#include <linux/device.h>
#include <linux/interrupt.h>
#include <linux/err.h>
#include <linux/sched.h>
#include <linux/dma-mapping.h>
#include <linux/slab.h>
/* Syslink Module level headers */
#ifdef CONFIG_TI81XX_VPSS_SYSNLINK_NOTIFY
#include <ti/syslink/Std.h>
#include <ti/ipc/MultiProc.h>
#include <ti/ipc/Notify.h>
#else
#include <syslink/multiproc.h>
#include <syslink/notify.h>
#endif
#include "core.h"

struct vps_fvid2_ctrl {
	u32                                     firm_ver;
	u32                                     fvid2handle;
	u32                                     rmprocid;
	u32                                     notifyno;
	u32                                     lineid;
	struct list_head                        list;
	spinlock_t                              lock;
	struct vps_payload_info                 pinfo;
	struct vps_psrvfvid2createparams        *fcrprms;
	u32                                     fcrprms_phy;
	struct vps_psrvfvid2deleteparams        *fdltprms;
	u32                                     fdltprms_phy;
	struct vps_psrvfvid2controlparams       *fctrlprms;
	u32                                     fctrlprms_phy;
	struct vps_psrvfvid2queueparams         *fqprms;
	u32                                     fqprms_phy;
	struct vps_psrvfvid2dequeueparams       *fdqprms;
	u32                                     fdqprms_phy;
	struct vps_psrvcallback                 *cbprms;
	u32                                     cbprms_phy;
	struct vps_psrvcommandstruct            *createcmdprms;
	u32                                     createcmdprms_phy;
	struct vps_psrvcommandstruct            *ctrlcmdprms;
	u32                                     ctrlcmdprms_phy;
	struct vps_psrvcommandstruct		*qcmdprms;
	u32					qcmdprms_phy;
	struct vps_psrvcommandstruct		*dqcmdprms;
	u32					dqcmdprms_phy;;
	struct vps_psrverrorcallback            *ecbprms;
	u32                                     ecbprms_phy;
};

static unsigned long         vps_timeout = 0u;
static struct list_head      fvid2_list;
static u32                   procid = 2;
static u32                   fwversion;
struct mutex                 fmutex;
/*define the information used by the proxy running in M3*/
#define VPS_FVID2_RESERVED_NOTIFY	0x09
#define VPS_FVID2_M3_INIT_VALUE      (0xAAAAAAAA)
#define VPS_FVID2_PS_LINEID          0
#define CURRENT_VPS_FIRMWARE_VERSION        (0x01000137)

static inline int get_payload_size(void);
static inline void assign_payload_addr(struct vps_fvid2_ctrl *fctrl,
				       struct vps_payload_info *pinfo,
				       u32 *buf_offset);

static inline u32 time_diff(struct timeval stime, struct timeval etime)
{
	if (etime.tv_usec < stime.tv_usec)
		return (etime.tv_sec - 1 - stime.tv_sec) * 1000 +
			(1000000 - stime.tv_usec + etime.tv_usec) / 1000;
	else
		return (etime.tv_sec - stime.tv_sec) * 1000 +
			(etime.tv_usec - stime.tv_usec) / 1000;
}
static void vps_callback(u16 procid,
		  u16 lineid,
		  u32 eventno,
		  void *arg,
		  u32 payload)
{
	struct vps_psrvcallback *appcb;
	struct vps_fvid2_ctrl *fctrl = (struct vps_fvid2_ctrl *)arg;
	unsigned long flags;

	/*perform all kinds of sanity check*/
	if (payload == 0) {
		VPSSERR("Payload is empty.\n");
		return;
	}
	if (arg == NULL) {
		VPSSERR("empty callback arguments.\n");
		return ;
	}

	if (fctrl->cbprms_phy != payload) {
		VPSSERR("payload not matched\n");
		return;
	}
	appcb = fctrl->cbprms;
	if (appcb->appcallback == NULL) {
		VPSSERR("no callback registered\n");
		return;
	}


	if (eventno != fctrl->notifyno) {
		VPSSERR("received event id %d and expected id %d\n",
			eventno, fctrl->notifyno);
		return;
	}

	if (lineid != fctrl->lineid) {
		VPSSERR("received lineid %d and expected lineid %d\n",
			lineid, fctrl->lineid);
	}

	if (procid != fctrl->rmprocid) {
		VPSSERR("received from processor %d and  \
					expected from processor %d\n",
			procid, fctrl->rmprocid);
		return;
	}
	/*dispatch the callback*/
	spin_lock_irqsave(&fctrl->lock, flags);
	if (appcb->cbtype == VPS_FVID2_IO_CALLBACK)
		appcb->appcallback(arg, appcb->appdata, NULL);
	spin_unlock_irqrestore(&fctrl->lock, flags);

}
/*allocate fvid2 control for each handle*/
static struct vps_fvid2_ctrl *vps_alloc_fvid2_ctrl(void)
{
	u32 size, offset = 0;
	struct vps_fvid2_ctrl *fctrl = NULL;

	mutex_lock(&fmutex);
	fctrl = kzalloc(sizeof(struct vps_fvid2_ctrl), GFP_KERNEL);
	if (fctrl) {
		size = get_payload_size();
		fctrl->pinfo.vaddr = vps_sbuf_alloc(size,
						&fctrl->pinfo.paddr);
		if (!fctrl->pinfo.vaddr) {
			VPSSERR("alloc fvid2 share buffer failed\n");
			/*free fctrl*/
			kfree(fctrl);
			fctrl = NULL;
			goto exit;
		}
		fctrl->pinfo.size = PAGE_ALIGN(size);
		memset(fctrl->pinfo.vaddr, 0, fctrl->pinfo.size);
		assign_payload_addr(fctrl, &fctrl->pinfo, &offset);

		fctrl->rmprocid = procid;
		fctrl->lineid = VPS_FVID2_PS_LINEID;
		fctrl->firm_ver = fwversion;
		spin_lock_init(&fctrl->lock);
		list_add_tail(&fctrl->list, &fvid2_list);

	}
exit:
	mutex_unlock(&fmutex);
	return fctrl;
}
/*free existing fvid2 control*/
static void vps_free_fvid2_ctrl(struct vps_fvid2_ctrl *fctrl)
{
	mutex_lock(&fmutex);
	if (!list_empty(&fvid2_list)) {
		struct vps_fvid2_ctrl *curr, *next;
		list_for_each_entry_safe(curr, next, &fvid2_list, list) {
			if (fctrl == curr) {
				list_del(&fctrl->list);
				/*free the sbuf*/
				if (fctrl->pinfo.vaddr)
					vps_sbuf_free(fctrl->pinfo.paddr,
						fctrl->pinfo.vaddr,
						fctrl->pinfo.size);
				kfree(fctrl);
				break;
			}
		}
	}
	mutex_unlock(&fmutex);
}

static inline int vps_check_fvid2_ctrl(void *handle)
{
	int r = 1;
	mutex_lock(&fmutex);
	if (!list_empty(&fvid2_list)) {
		struct vps_fvid2_ctrl *curr;
		list_for_each_entry(curr, &fvid2_list, list) {
			if (curr == handle) {
				r = 0;
				break;
			}

		}
	}
	mutex_unlock(&fmutex);
	return r;
}

void *vps_fvid2_create(u32 drvid,
			u32 instanceid,
			void *createargs,
			void *createstatusargs,
			struct fvid2_cbparams *cbparams)
{

	struct vps_fvid2_ctrl *fctrl = NULL;
	int status;
	struct timeval stime, etime;
	u32 td = 0;

	fctrl = vps_alloc_fvid2_ctrl();
	if (fctrl == NULL)
		return NULL;

	/*assembel the create parameter structure*/
	fctrl->fcrprms->command = VPS_FVID2_CREATE;
	fctrl->fcrprms->hosttaskinstance = VPS_FVID2_TASK_TYPE_1,
	fctrl->fcrprms->createargs = createargs;
	fctrl->fcrprms->createstatusargs = createstatusargs;
	fctrl->fcrprms->drvid = drvid;
	fctrl->fcrprms->cbparams = cbparams;
	fctrl->fcrprms->instanceid = instanceid;
	fctrl->fcrprms->fvid2handle = (void *)VPS_FVID2_M3_INIT_VALUE;


	if (cbparams == NULL) {
		fctrl->fcrprms->ioreqcb = NULL;
		fctrl->fcrprms->errcb = NULL;
	} else {
		fctrl->fcrprms->ioreqcb =
			(struct vps_psrvcallback *)fctrl->cbprms_phy;
		fctrl->fcrprms->errcb =
			(struct vps_psrverrorcallback *)fctrl->ecbprms_phy;
	}

	fctrl->createcmdprms->cmdtype = VPS_FVID2_CMDTYPE_SIMPLEX;
	fctrl->createcmdprms->simplexcmdarg = (void *)fctrl->fcrprms_phy;
	/*set the event to M3*/
	#ifdef CONFIG_TI81XX_VPSS_SYSNLINK_NOTIFY
	status = Notify_sendEvent(fctrl->rmprocid,
				  fctrl->lineid,
				  VPS_FVID2_RESERVED_NOTIFY,
				  fctrl->createcmdprms_phy,
				  1);
	#else
	status = notify_send_event(fctrl->rmprocid,
				  fctrl->lineid,
				  VPS_FVID2_RESERVED_NOTIFY,
				  fctrl->createcmdprms_phy,
				  1);
	#endif
	if (status < 0) {
		VPSSERR("send create event failed with status 0x%0x\n",
			  status);
		goto exit;
	}
	do_gettimeofday(&stime);
	etime = stime;
	while ((fctrl->fcrprms->fvid2handle ==
		    (void *)VPS_FVID2_M3_INIT_VALUE)) {
		usleep_range(100, 300);
		/*time out check*/
		if (vps_timeout) {
			do_gettimeofday(&etime);
			td = time_diff(stime, etime);
			if (vps_timeout < td) {
				VPSSERR("create timeout\n");
				goto exit;

			}
		}
	}

	fctrl->notifyno = fctrl->fcrprms->syslnkntyno;
	fctrl->fvid2handle = (u32)fctrl->fcrprms->fvid2handle;
	VPSSDBG("Fvid2 handle 0x%08x with notifyno %d within %d ms\n",
		(u32)fctrl->fvid2handle,
		fctrl->notifyno,
		td);

	if (fctrl->fvid2handle == 0) {
		VPSSERR("create handle is NULL\n");
		goto exit;
	}
	/*register the callback if successfully*/
	if (cbparams != NULL) {
		#ifdef CONFIG_TI81XX_VPSS_SYSNLINK_NOTIFY
		status = Notify_registerEvent(fctrl->rmprocid,
					fctrl->lineid,
					fctrl->notifyno,
					(Notify_FnNotifyCbck)vps_callback,
					(void *)fctrl);
		#else
		status = notify_register_event(fctrl->rmprocid,
					fctrl->lineid,
					fctrl->notifyno,
					(notify_fn_notify_cbck)vps_callback,
					(void *)fctrl);
		#endif
		if (status < 0) {
			VPSSERR("register event status 0x%08x\n", status);
			/*since the registration is failed,
				delete from M3 as well*/
			vps_fvid2_delete(fctrl, NULL);
			return NULL;
		}
	}

	return (void *)fctrl;
exit:
	vps_free_fvid2_ctrl(fctrl);
	return NULL;

}
EXPORT_SYMBOL(vps_fvid2_create);

int vps_fvid2_delete(void *handle, void *deleteargs)
{

	struct vps_fvid2_ctrl *fctrl = (struct vps_fvid2_ctrl *)handle;
	int status;
	struct timeval stime, etime;
	u32 td = 0;
	int r;

	VPSSDBG("enter delete.\n");
	if (vps_check_fvid2_ctrl(handle))
		return -EINVAL;

	fctrl->fdltprms->command = VPS_FVID2_DELETE;
	fctrl->fdltprms->fvid2handle = (void *)fctrl->fvid2handle;
	fctrl->fdltprms->deleteargs = deleteargs;
	fctrl->fdltprms->returnvalue = VPS_FVID2_M3_INIT_VALUE;


	fctrl->createcmdprms->cmdtype = VPS_FVID2_CMDTYPE_SIMPLEX;
	fctrl->createcmdprms->simplexcmdarg = (void *)fctrl->fdltprms_phy;

	/*send event to proxy in M3*/
	#ifdef CONFIG_TI81XX_VPSS_SYSNLINK_NOTIFY
	status = Notify_sendEvent(fctrl->rmprocid,
				  fctrl->lineid,
				  fctrl->notifyno,
				  fctrl->createcmdprms_phy,
				  1);
	#else
	status = notify_send_event(fctrl->rmprocid,
				  fctrl->lineid,
				  fctrl->notifyno,
				  fctrl->createcmdprms_phy,
				  1);
	#endif
	if (status < 0) {
		VPSSERR("set delete event failed status 0x%08x\n", status);
		return -EINVAL;
	} else {
		do_gettimeofday(&stime);
		etime = stime;
		while ((fctrl->fdltprms->returnvalue ==
			VPS_FVID2_M3_INIT_VALUE)) {
			usleep_range(100, 300);
			/*time out check*/
			if (vps_timeout) {
				do_gettimeofday(&etime);
				td = time_diff(stime, etime);
				if (vps_timeout < td) {
					VPSSERR("delete time out\n");
					return -ETIMEDOUT;
				}
			}
		}

		VPSSDBG("delete event return %d within %d ms\n",
			 fctrl->fdltprms->returnvalue,
			 td);

	}
	if (fctrl->fcrprms->cbparams != NULL) {
		#ifdef CONFIG_TI81XX_VPSS_SYSNLINK_NOTIFY
		status = Notify_unregisterEvent(
					fctrl->rmprocid,
					fctrl->lineid,
					fctrl->notifyno,
					(Notify_FnNotifyCbck)vps_callback,
					(void *)fctrl);
		#else
		status = notify_unregister_event(
					fctrl->rmprocid,
					fctrl->lineid,
					fctrl->notifyno,
					(notify_fn_notify_cbck)vps_callback,
					(void *)fctrl);

		#endif
		if (status < 0)
			VPSSERR("unregister Event status 0x%08x\n", status);
	}

	r = fctrl->fdltprms->returnvalue;
	vps_free_fvid2_ctrl(fctrl);
	return r;

}
EXPORT_SYMBOL(vps_fvid2_delete);

int vps_fvid2_control(void *handle,
		      u32 cmd,
		      void *cmdargs,
		      void *cmdstatusargs)
{
	struct vps_fvid2_ctrl *fctrl = (struct vps_fvid2_ctrl *)handle;
	int status;
	struct timeval etime, stime;
	u32 td = 0;

	if (vps_check_fvid2_ctrl(handle))
		return -EINVAL;

	VPSSDBG("send control with cmd 0x%08x\n", cmd);
	/*assembel the structure*/
	fctrl->fctrlprms->command = VPS_FVID2_CONTROL;
	fctrl->fctrlprms->fvid2handle = (void *)fctrl->fvid2handle;
	fctrl->fctrlprms->cmd = cmd;
	fctrl->fctrlprms->cmdargs = cmdargs;
	fctrl->fctrlprms->cmdstatusargs = cmdstatusargs;
	fctrl->fctrlprms->returnvalue = VPS_FVID2_M3_INIT_VALUE;

	fctrl->ctrlcmdprms->cmdtype = VPS_FVID2_CMDTYPE_SIMPLEX;
	fctrl->ctrlcmdprms->simplexcmdarg = (void *)fctrl->fctrlprms_phy;
	/*send the event*/
	#ifdef CONFIG_TI81XX_VPSS_SYSNLINK_NOTIFY
	status = Notify_sendEvent(fctrl->rmprocid,
				  fctrl->lineid,
				  fctrl->notifyno,
				  fctrl->ctrlcmdprms_phy,
				  1);
	#else
	status = notify_send_event(fctrl->rmprocid,
				  fctrl->lineid,
				  fctrl->notifyno,
				  fctrl->ctrlcmdprms_phy,
				  1);
	#endif

	if (status < 0) {
		VPSSERR("send control with cmd 0x%08x status 0x%08x\n",
			  cmd, status);
		return -EINVAL;
	} else {
		do_gettimeofday(&stime);
		etime = stime;
		while (fctrl->fctrlprms->returnvalue ==
			VPS_FVID2_M3_INIT_VALUE) {

			usleep_range(100, 300);
			if (vps_timeout) {
				do_gettimeofday(&etime);
				td = time_diff(stime, etime);
				if (vps_timeout < td) {
					VPSSERR("contrl event 0x%x timeout\n",
						cmd);
					return -ETIMEDOUT;
				}
			}
		}
	}

	VPSSDBG("control event 0x%x return %d within %d ms.\n",
		cmd,
		fctrl->fctrlprms->returnvalue,
		td);

	return fctrl->fctrlprms->returnvalue;

}
EXPORT_SYMBOL(vps_fvid2_control);

int vps_fvid2_queue(void *handle,
		    struct fvid2_framelist *framelist,
		    u32 streamid)
{
	struct vps_fvid2_ctrl *fctrl = (struct vps_fvid2_ctrl *)handle;
	int status;
	struct timeval stime, etime;
	u32 td = 0;

	if (vps_check_fvid2_ctrl(handle)) {
		VPSSERR("Q handle 0x%08x\n", (u32)handle);
		return -EINVAL;
	}
	/*assemble the structure*/
	fctrl->fqprms->command = VPS_FVID2_QUEUE;
	fctrl->fqprms->fvid2handle = (void *)fctrl->fvid2handle;
	fctrl->fqprms->framelist = framelist;
	fctrl->fqprms->streamid = streamid;
	fctrl->fqprms->returnvalue = VPS_FVID2_M3_INIT_VALUE;

	fctrl->qcmdprms->cmdtype = VPS_FVID2_CMDTYPE_SIMPLEX;
	fctrl->qcmdprms->simplexcmdarg = (void *)fctrl->fqprms_phy;

	/* send event to proxy in M3*/
	#ifdef CONFIG_TI81XX_VPSS_SYSNLINK_NOTIFY
	status = Notify_sendEvent(fctrl->rmprocid,
				  fctrl->lineid,
				  fctrl->notifyno,
				  fctrl->qcmdprms_phy,
				  1);
	#else
	status = notify_send_event(fctrl->rmprocid,
				  fctrl->lineid,
				  fctrl->notifyno,
				  fctrl->qcmdprms_phy,
				  1);
	#endif
	if (status < 0) {
		VPSSERR("send Q event status 0x%08x\n", status);
		return -EINVAL;
	} else {
		do_gettimeofday(&stime);
		etime = stime;
		while (fctrl->fqprms->returnvalue ==
			VPS_FVID2_M3_INIT_VALUE) {
			usleep_range(100, 300);
			/*time out check*/
			if (vps_timeout) {
				do_gettimeofday(&etime);
				td = time_diff(stime, etime);
				if (vps_timeout < td) {
					VPSSERR("queue timeout\n");
					return -ETIMEDOUT;
				}
			}

		}
	}

	VPSSDBG("queue event return %d within %d ms.\n",
		fctrl->fqprms->returnvalue,
		td);

	return fctrl->fqprms->returnvalue;
}
EXPORT_SYMBOL(vps_fvid2_queue);

int vps_fvid2_dequeue(void *handle,
		      struct fvid2_framelist *framelist,
		      u32 stream_id,
		      u32 timeout)
{
	struct vps_fvid2_ctrl *fctrl = (struct vps_fvid2_ctrl *)handle;
	int status;
	struct timeval etime, stime;
	u32 td = 0;

	if (vps_check_fvid2_ctrl(handle)) {
		VPSSERR("DQ handle NULL.\n");
		return -EINVAL;
	}

	/*assembel the structure*/
	fctrl->fdqprms->command = VPS_FVID2_DEQUEUE;
	fctrl->fdqprms->framelist = framelist;
	fctrl->fdqprms->streamid = stream_id;
	fctrl->fdqprms->fvid2handle = (void *)fctrl->fvid2handle;
	fctrl->fdqprms->timeout = timeout;
	fctrl->fdqprms->returnvalue = VPS_FVID2_M3_INIT_VALUE;

	fctrl->dqcmdprms->cmdtype = VPS_FVID2_CMDTYPE_SIMPLEX;
	fctrl->dqcmdprms->simplexcmdarg = (void *)fctrl->fdqprms_phy;


	/* send event to proxy in M3*/
	#ifdef CONFIG_TI81XX_VPSS_SYSNLINK_NOTIFY
	status = Notify_sendEvent(fctrl->rmprocid,
				  fctrl->lineid,
				  fctrl->notifyno,
				  fctrl->dqcmdprms_phy,
				  1);
	#else
	status = notify_send_event(fctrl->rmprocid,
				  fctrl->lineid,
				  fctrl->notifyno,
				  fctrl->dqcmdprms_phy,
				  1);
	#endif

	if (status < 0) {
		VPSSERR("send DQ event status 0x%08x\n", status);
		return -EINVAL;
	} else {
		do_gettimeofday(&stime);
		etime = stime;
		while (fctrl->fdqprms->returnvalue ==
			VPS_FVID2_M3_INIT_VALUE) {
			usleep_range(100, 300);
			/*time out check*/
			if (vps_timeout) {
				do_gettimeofday(&etime);
				td = time_diff(stime, etime);
				if (vps_timeout < td) {
					VPSSDBG("dequeue timeout\n");
					return -ETIMEDOUT;
				}
			}
		}
	}

	VPSSDBG("dequeue event return %d within %d ms\n",
		fctrl->fdqprms->returnvalue,
		td);

	return fctrl->fdqprms->returnvalue;
}
EXPORT_SYMBOL(vps_fvid2_dequeue);


static int get_firmware_version(struct platform_device *pdev, u32 procid,
			u32 *version, u32 *status)
{
	struct vps_payload_info  pinfo;
	struct vps_psrvcommandstruct  *cmdstruct;
	u32    cmdstruct_phy;
	struct vps_psrvgetstatusvercmdparams *vps_verparams;
	u32    vps_verparams_phy;
	int r = -1, rtv;
	struct timeval stime, etime;
	u32 td = 0;
	/*get the M3 version number*/
	VPSSDBG("Handshake...\n");

	pinfo.size = PAGE_SIZE;
	pinfo.vaddr = vps_sbuf_alloc(pinfo.size, &pinfo.paddr);
	if (pinfo.vaddr == NULL) {
		VPSSERR("failed to allocate version cmd struct\n");
		return -EINVAL;
	}
	memset(pinfo.vaddr, 0, pinfo.size);
	/*assign to version command*/
	cmdstruct = (struct vps_psrvcommandstruct *)pinfo.vaddr;
	cmdstruct_phy = pinfo.paddr;

	vps_verparams = (struct vps_psrvgetstatusvercmdparams *)
		(pinfo.vaddr + sizeof(struct vps_psrvcommandstruct));
	vps_verparams_phy = pinfo.paddr +
			sizeof(struct vps_psrvcommandstruct);

	/*init the structure*/
	cmdstruct->cmdtype = VPS_FVID2_CMDTYPE_SIMPLEX;
	cmdstruct->simplexcmdarg = (void *)vps_verparams_phy;

	vps_verparams->command = VPS_FVID2_GET_FIRMWARE_VERSION;
	vps_verparams->returnvalue = VPS_FVID2_M3_INIT_VALUE;
	#ifdef CONFIG_TI81XX_VPSS_SYSNLINK_NOTIFY
	rtv = Notify_sendEvent(procid,
				 VPS_FVID2_PS_LINEID,
				 VPS_FVID2_RESERVED_NOTIFY,
				 cmdstruct_phy,
				 1);
	#else
	rtv = notify_send_event(procid,
				 VPS_FVID2_PS_LINEID,
				 VPS_FVID2_RESERVED_NOTIFY,
				 cmdstruct_phy,
				 1);
	#endif
	if (rtv < 0) {
		VPSSDBG("Failed to send version command to M3 %#x.\n",
			 rtv);
		r = -EINVAL;
		goto exit;
	} else {
		do_gettimeofday(&stime);
		etime = stime;
		while (vps_verparams->returnvalue ==
				VPS_FVID2_M3_INIT_VALUE) {
			usleep_range(100, 300);
			do_gettimeofday(&etime);
			td = time_diff(stime, etime);
			/*one second timeout*/
			if (1000  < td) {
				VPSSDBG("Get Firmware"
					" Version Timeout\n");
				r = -ETIMEDOUT;
				goto exit;
			}
		}
	}

	r = vps_verparams->returnvalue;
	if (version)
		*version = vps_verparams->version;
	if (status)
		*status = vps_verparams->status;
exit:
	/*release the memory*/
	vps_sbuf_free(pinfo.paddr, pinfo.vaddr, pinfo.size);
	return r;
}
static inline int get_payload_size(void)
{
	int size = 0;

	/*calculate the size of each FVID2 to allocate*/
	size  = sizeof(struct vps_psrvfvid2createparams);
	size += sizeof(struct vps_psrvfvid2deleteparams);
	size += sizeof(struct vps_psrvfvid2controlparams);
	size += sizeof(struct vps_psrvfvid2queueparams);
	size += sizeof(struct vps_psrvfvid2dequeueparams);
	size += sizeof(struct vps_psrvcallback);
	size += sizeof(struct vps_psrverrorcallback);
	size += sizeof(struct vps_psrvcommandstruct);
	size += sizeof(struct vps_psrvfvid2processframesparams);
	size += sizeof(struct vps_psrvfvid2getprocessedframesparams);

	return size;
}

static inline void assign_payload_addr(struct vps_fvid2_ctrl *fctrl,
				       struct vps_payload_info *pinfo,
				       u32 *buf_offset)
{

	/*assign the virt and phy address*/

	fctrl->fcrprms = (struct  vps_psrvfvid2createparams *)
			setaddr(pinfo,
				buf_offset,
				&fctrl->fcrprms_phy,
				sizeof(struct vps_psrvfvid2createparams));


	fctrl->fdltprms = (struct vps_psrvfvid2deleteparams *)
			setaddr(pinfo,
				buf_offset,
				&fctrl->fdltprms_phy,
				sizeof(struct vps_psrvfvid2deleteparams));

	fctrl->fctrlprms = (struct vps_psrvfvid2controlparams *)
			setaddr(pinfo,
				buf_offset,
				&fctrl->fctrlprms_phy,
				sizeof(struct vps_psrvfvid2controlparams));

	fctrl->fqprms = (struct vps_psrvfvid2queueparams *)
			setaddr(pinfo,
				buf_offset,
				&fctrl->fqprms_phy,
				sizeof(struct vps_psrvfvid2queueparams));

	fctrl->fdqprms = (struct vps_psrvfvid2dequeueparams *)
			setaddr(pinfo,
				buf_offset,
				&fctrl->fdqprms_phy,
				sizeof(struct vps_psrvfvid2dequeueparams));

	fctrl->cbprms = (struct vps_psrvcallback *)
			setaddr(pinfo,
				buf_offset,
				&fctrl->cbprms_phy,
				sizeof(struct vps_psrvcallback));

	fctrl->ecbprms = (struct vps_psrverrorcallback *)
			setaddr(pinfo,
				buf_offset,
				&fctrl->ecbprms_phy,
				sizeof(struct vps_psrverrorcallback));

	fctrl->createcmdprms = (struct vps_psrvcommandstruct *)
			setaddr(pinfo,
				buf_offset,
				&fctrl->createcmdprms_phy,
				sizeof(struct vps_psrvcommandstruct));
	fctrl->ctrlcmdprms = (struct vps_psrvcommandstruct *)
			setaddr(pinfo,
				buf_offset,
				&fctrl->ctrlcmdprms_phy,
				sizeof(struct vps_psrvcommandstruct));
	fctrl->qcmdprms = (struct vps_psrvcommandstruct *)
			setaddr(pinfo,
				buf_offset,
				&fctrl->qcmdprms_phy,
				sizeof(struct vps_psrvcommandstruct));
	fctrl->dqcmdprms = (struct vps_psrvcommandstruct *)
			setaddr(pinfo,
				buf_offset,
				&fctrl->dqcmdprms_phy,
				sizeof(struct vps_psrvcommandstruct));




}

int vps_fvid2_init(struct platform_device *pdev, u32 timeout)
{
	int i, r = 0;

	VPSSDBG("fvid2 init\n");
	vps_timeout = timeout;

	#ifdef CONFIG_TI81XX_VPSS_SYSNLINK_NOTIFY
	procid = MultiProc_getId("VPSS-M3");
	if (MultiProc_INVALIDID == procid) {
		VPSSERR("failed to get the M3DSS processor ID.\n");
		return -EINVAL;
	}

	#else
	procid = multiproc_get_id("VPSS-M3");
	if (MULTIPROC_INVALIDID == procid) {
		VPSSERR("failed to get the M3DSS processor ID.\n");
		return -EINVAL;
	}
	#endif

	INIT_LIST_HEAD(&fvid2_list);
	i = 0;
	do {
		/* this is kind of handshake to make sure
		 that M3 is ready to receive the command from A8,
		 10s is way enough to do*/
		r = get_firmware_version(pdev, procid, &fwversion, NULL);
		if (r) {
			VPSSDBG("Trying to get firmware version\n");
			msleep(50);
		}
	} while ((r != 0) && (++i < 200));

	if (r == 0) {
		if (fwversion != CURRENT_VPS_FIRMWARE_VERSION) {
			if (fwversion <
			     CURRENT_VPS_FIRMWARE_VERSION) {
				VPSSERR("M3 firmware version 0x%x is old,"
					"please update firmware version.\n",
					fwversion);
				r = -EINVAL;
				goto exit;
			}

			if (fwversion >
			    CURRENT_VPS_FIRMWARE_VERSION)
				VPSSERR("M3 firmware version 0x%x is newer,"
					"driver may not work properly.\n",
					fwversion);
		} else
			VPSSDBG("get firmware version 0x%08x.\n",
				fwversion);
	} else {
		r = -EINVAL;
		VPSSERR("Unable to get firmware version\n");
		goto exit;
	}

	mutex_init(&fmutex);
exit:
	if (r)
		vps_fvid2_deinit(pdev);
	return r;
}

void vps_fvid2_deinit(struct platform_device *pdev)
{
	VPSSDBG("fvid2 deinit\n");
	vps_timeout = 0;
	while (!list_empty(&fvid2_list)) {
		struct vps_fvid2_ctrl *cur, *next;
		list_for_each_entry_safe(cur, next, &fvid2_list, list)
			vps_free_fvid2_ctrl(cur);
	}
}
