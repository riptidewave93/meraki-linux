/*
* DesignWare HS OTG controller driver
* Copyright (C) 2006 Synopsys, Inc.
* Portions Copyright (C) 2010 Applied Micro Circuits Corporation.
*
* This program is free software: you can redistribute it and/or
* modify it under the terms of the GNU General Public License
* version 2 as published by the Free Software Foundation.
*
* This program is distributed in the hope that it will be useful
* but WITHOUT ANY WARRANTY; without even the implied warranty of
* MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
* GNU General Public License version 2 for more details.
*
* You should have received a copy of the GNU General Public License
* along with this program; if not, see http://www.gnu.org/licenses
* or write to the Free Software Foundation, Inc., 51 Franklin Street,
* Suite 500, Boston, MA 02110-1335 USA.
*
* Based on Synopsys driver version 2.60a
* Modified by Mark Miesfeld <mmiesfeld@apm.com>
* Modified by Stefan Roese <sr@denx.de>, DENX Software Engineering
* Modified by Chuck Meade <chuck@theptrgroup.com>
*
* THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
* AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING BUT NOT LIMITED TO THE
* IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
* ARE DISCLAIMED. IN NO EVENT SHALL SYNOPSYS, INC. BE LIABLE FOR ANY DIRECT,
* INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY OR CONSEQUENTIAL DAMAGES
* (INCLUDING BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
* LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND
* ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY OR TORT
* (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF
* THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
*
*/

#if !defined(__DWC_OTG_ATTR_H__)
#define __DWC_OTG_ATTR_H__

/** @file
 * This file contains the interface to the Linux device attributes.
 */
extern struct device_attribute dev_attr_regoffset;
extern struct device_attribute dev_attr_regvalue;

extern struct device_attribute dev_attr_mode;
extern struct device_attribute dev_attr_hnpcapable;
extern struct device_attribute dev_attr_srpcapable;
extern struct device_attribute dev_attr_hnp;
extern struct device_attribute dev_attr_srp;
extern struct device_attribute dev_attr_buspower;
extern struct device_attribute dev_attr_bussuspend;
extern struct device_attribute dev_attr_busconnected;
extern struct device_attribute dev_attr_gotgctl;
extern struct device_attribute dev_attr_gusbcfg;
extern struct device_attribute dev_attr_grxfsiz;
extern struct device_attribute dev_attr_gnptxfsiz;
extern struct device_attribute dev_attr_gpvndctl;
extern struct device_attribute dev_attr_ggpio;
extern struct device_attribute dev_attr_guid;
extern struct device_attribute dev_attr_gsnpsid;
extern struct device_attribute dev_attr_devspeed;
extern struct device_attribute dev_attr_enumspeed;
extern struct device_attribute dev_attr_hptxfsiz;
extern struct device_attribute dev_attr_hprt0;

void dwc_otg_attr_create (struct device *dev);
void dwc_otg_attr_remove (struct device *dev);

#endif
