/*
 * Copyright (c) 2008, Atheros Communications Inc.
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
 */


#ifndef _ATHR_PB_H
#define _ATHR_PB_H

extern int32_t unregister_simple_config_callback(char *name);
extern int32_t register_simple_config_callback (char *name, void *callback, void *arg1, void *arg2);

/*
 * Handler for front panel push button interrupt
 */
static inline irqreturn_t
pushbutton_intr (int cpl, void *dev_id, struct pt_regs *regs, void *push_time)
{
    struct net_device *dev = dev_id;
    static char *tag = "EthPUSH-BUTTON.indication";
    union iwreq_data wrqu;
    char buf[128];

    /*
    ** Notify Push Button event
    */

    printk("Pushbutton Notify on %s for %d sec(s)\n",dev->name, *(u_int32_t *)push_time);
    snprintf(buf, sizeof(buf), "%s Push dur=%d", tag, *(u_int32_t *)push_time);
    memset(&wrqu, 0, sizeof(wrqu));
    wrqu.data.length = strlen(buf);
    wireless_send_event(dev, IWEVCUSTOM, &wrqu, buf);
    return IRQ_HANDLED;
}

#if NO_PUSH_BUTTON
#define is_push_button()  0
#else
#define is_push_button()  1
#endif

#endif // ATHR_PB_H
