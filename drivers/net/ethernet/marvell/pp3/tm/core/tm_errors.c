/*******************************************************************************
Copyright (C) Marvell International Ltd. and its affiliates

This software file (the "File") is owned and distributed by Marvell
International Ltd. and/or its affiliates ("Marvell") under the following
alternative licensing terms.  Once you have made an election to distribute the
File under one of the following license alternatives, please (i) delete this
introductory statement regarding license alternatives, (ii) delete the two
license alternatives that you have not elected to use and (iii) preserve the
Marvell copyright notice above.


********************************************************************************
Marvell GPL License Option

If you received this File from Marvell, you may opt to use, redistribute and/or
modify this File in accordance with the terms and conditions of the General
Public License Version 2, June 1991 (the "GPL License"), a copy of which is
available along with the File in the license.txt file or by writing to the Free
Software Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA 02111-1307 or
on the worldwide web at http://www.gnu.org/licenses/gpl.txt.

THE FILE IS DISTRIBUTED AS-IS, WITHOUT WARRANTY OF ANY KIND, AND THE IMPLIED
WARRANTIES OF MERCHANTABILITY OR FITNESS FOR A PARTICULAR PURPOSE ARE EXPRESSLY
DISCLAIMED.  The GPL License provides additional details about this warranty
disclaimer.
*******************************************************************************/

#include "tm_errors.h"
#include "tm_errcodes.h"
#include "set_hw_registers.h"


/**
 */
int tm_sched_get_errors(tm_handle hndl, struct tm_error_info *info)
{
	int rc;
	DECLARE_TM_CTL_PTR(ctl, hndl)
	CHECK_TM_CTL_PTR(ctl)
	rc = get_hw_sched_errors(ctl, info);
	if (rc)
		return TM_HW_SCHED_GET_ERRORS_FAILED;
	return rc;
}


/**
 */
int tm_drop_get_errors(tm_handle hndl, struct tm_error_info *info)
{
	int rc;
	DECLARE_TM_CTL_PTR(ctl, hndl)
	CHECK_TM_CTL_PTR(ctl)

	rc = get_hw_drop_errors(ctl, info);
	if (rc)
		rc = TM_HW_DROP_GET_ERRORS_FAILED;
	return rc;
}

