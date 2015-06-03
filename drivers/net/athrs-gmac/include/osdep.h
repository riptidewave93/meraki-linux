#ifndef __OSDEP_H
#define __OSDEP_H

#include <linux/stddef.h>
#include <linux/version.h>
#if (LINUX_VERSION_CODE >= KERNEL_VERSION(2,6,31))
#include <linux/autoconf.h>
#else
#include <linux/config.h>
#endif
#include <linux/module.h>
#include <linux/types.h>
#include <asm/byteorder.h>
#include <linux/init.h>
#include <linux/errno.h>
#include <linux/kernel.h>
#include <linux/netdevice.h>
#include <linux/etherdevice.h>
#include <linux/skbuff.h>
#include <linux/delay.h>
#include <linux/timer.h>
#include <linux/interrupt.h>
#include <linux/dma-mapping.h>
#include <linux/bitops.h>
#include <asm/irq.h>
#include <asm/io.h>
#include <net/sch_generic.h>
#include <net/inet_ecn.h>                /* XXX for TOS */


#if (LINUX_VERSION_CODE >= KERNEL_VERSION(2,6,31))


#define athr_stop_dev(dev)           dev->netdev_ops->ndo_stop(dev);
#define athr_open_dev(dev)           dev->netdev_ops->ndo_open(dev);
#define ATHR_MAC_NETPRIV(mac,dev)    (mac = netdev_priv(dev))
#define ATHR_MAC_PRIV(dev)           (netdev_priv(dev))

#ifndef CONFIG_ATHR_RX_TASK
#define athr_napi_is_enabled(mac)    (mac->napi.state == NAPI_STATE_SCHED) 
#define athr_napi_is_disabled(mac)   (mac->napi.state == NAPI_STATE_DISABLE) 
#define athr_napi_del(mac)           netif_napi_del(&mac->napi)

#define athr_napi_disable(mac)  	 \
do {					 \
	if (!athr_napi_is_disabled(mac)) \
            napi_disable(&mac->napi); 	 \
} while (0)

#define athr_napi_enable(mac)	         \
do {					 \
	if (!athr_napi_is_enabled(mac))  \
            napi_enable(&mac->napi); 	 \
} while (0)

#else

#define athr_napi_disable(xyz)	/* nothing */
#define athr_napi_enable(xyz)   /* nothing */
#define athr_napi_del(xyz)      /* nothing */

#endif

#define athr_mac_dma_cache_sync(b, c)	        \
do {						\
            dma_cache_sync(NULL, (void *)b,     \
            c, DMA_TO_DEVICE);	        \
} while (0)

#define ATHR_MAC_ISR_ARGS int cpl, void *dev_id
#define athr_mac_rx_sched_prep(m, d)	napi_schedule_prep(&m->napi)
#define __athr_mac_rx_sched(m, d)	__napi_schedule(&m->napi)
#define athr_mac_rx_sched(m)		napi_schedule(&m->napi)
#define athr_mac_cache_inv(d, s)	        \
do {						\
        dma_cache_sync(NULL, (void *)d,     \
        s, DMA_FROM_DEVICE);	        \
} while (0)
#define ATHR_MAC_TASK_ARG	struct work_struct *ws


#define ATHR_MAC_TASK_MAC()	                \
	athr_gmac_t *mac = (athr_gmac_t *)	\
		container_of(ws, athr_gmac_t, mac_tx_timeout)

#define ATHR_MAC_NAPI_MAC()	                \
	athr_gmac_t *mac = (athr_gmac_t *)	\
		container_of(napi, athr_gmac_t, napi)
#define ATHR_GET_DEV() \
	struct net_device *dev = mac->mac_dev;

#define __ATHR_MAC_RX_COMPLETE(mac)	__napi_complete(&mac->napi)
#define ATHR_MAC_RX_COMPLETE(mac)	napi_complete(&mac->napi)
#define ATHR_MAC_RX_RESCHED(mac)	napi_reschedule(&mac->napi)
#define ATHR_NAPI_CHECK_STATUS() 			   \
do {				   			   \
        if ((status & ATHR_GMAC_RX_STATUS_PKT_RCVD) == 0)  \
            goto done;					   \
} while (0)

#define ATHR_NAPI_HANDLE_RX_NOT_DONE() 			\
do { 							\
	ATHR_MAC_RX_COMPLETE(mac);			\
	ATHR_MAC_RX_RESCHED(mac);			\
}while (0)

#define ATHR_NAPI_HANDLE_RX_DONE()				\
do {							\
        spin_lock_irqsave(&mac->mac_lock, flags);       \
        __ATHR_MAC_RX_COMPLETE(mac);			\
        athr_gmac_intr_enable_recv(mac);		\
        spin_unlock_irqrestore(&mac->mac_lock, flags);	\
} while (0)

#define ATHR_MAC_UPDATE_QUOTA_BUDGET(quota, budget, work)  /* Do nothing */
#define ATHR_MAC_GET_MAX_WORK(budget, dev) budget
#define  IF_RX_STATUS_DONE(ret,ATHR_GMAC_RX_STATUS_DONE,work_done,budget) \
    if (likely(ret == ATHR_GMAC_RX_STATUS_DONE) && (work_done < budget))

#define ATHR_MAC_INIT_WORK(m, f)	INIT_WORK(&m->mac_tx_timeout, (void *)f)
#define ATHR_MAC_ETHERDEV_SZ	sizeof(athr_gmac_t)
#define ATHR_MAC_IRQF_DISABLED	IRQF_DISABLED


#define ATHR_MAC_POLL() \
   athr_gmac_poll(struct napi_struct *napi, int budget)

#define ATHR_MAC_SET_DEV_NET_OPS(dev, mac,                      \
                                 athr_gmac_get_stats,           \
                                 athr_gmac_open,                \
                                 athr_gmac_stop,                \
                                 athr_gmac_hard_start,          \
                                 athr_gmac_do_ioctl,            \
                                 athr_gmac_poll,                \
                                 athr_gmac_tx_timeout,          \
                                 ATHR_MAC_NAPI_WEIGHT,          \
                                 tx_qlen)                       \
do {                                                            \
        athr_gmac_net_ops.ndo_open      = athr_gmac_open,       \
        athr_gmac_net_ops.ndo_stop      = athr_gmac_stop,       \
        athr_gmac_net_ops.ndo_start_xmit= athr_gmac_hard_start, \
        athr_gmac_net_ops.ndo_get_stats = athr_gmac_get_stats,  \
        athr_gmac_net_ops.ndo_tx_timeout= athr_gmac_tx_timeout, \
        athr_gmac_net_ops.ndo_do_ioctl  = athr_gmac_do_ioctl,   \
        athr_gmac_net_ops.ndo_set_mac_address = eth_mac_addr,   \
        dev->tx_queue_len = tx_qlen;                            \
        dev->netdev_ops = (const struct net_device_ops *)       \
                                &athr_gmac_net_ops;             \
        netif_napi_add(dev, &mac->napi,                         \
                        athr_gmac_poll,                         \
                        ATHR_MAC_NAPI_WEIGHT);                  \
} while(0)

#else /* #if (LINUX_VERSION_CODE >= KERNEL_VERSION(2,6,31)) */

/* For PRE linux 2.6.31 kernel */

#define ATHR_MAC_NETPRIV(mac,dev) 				\
do { 						  		\
 mac = kmalloc(sizeof(athr_gmac_t), GFP_KERNEL);    		\
        if (!mac)						\
        {							\
            printk(MODULE_NAME ": unable to allocate mac\n");	\
            free_netdev(dev);					\
            return 1;						\
        }							\
} while (0)
#define ATHR_MAC_PRIV(dev) (dev->priv)
#define athr_napi_disable(xyz)	/* nothing */
#define athr_napi_enable(xyz)   /* nothing */
#define athr_napi_del(xyz)      /* nothing */
#define athr_stop_dev(dev)      dev->stop(dev);
#define athr_open_dev(dev)      dev->open(dev);

#define athr_mac_dma_cache_sync(b, c)	        \
do {						\
        dma_cache_wback((unsigned long)b, c);	\
} while (0)

#define ATHR_MAC_ISR_ARGS               int cpl, void *dev_id, struct pt_regs *regs
#define athr_mac_rx_sched_prep(m, d)	netif_rx_schedule_prep(d)
#define __athr_mac_rx_sched(m, d)	__netif_rx_schedule(d)
#define athr_mac_rx_sched(m)	        netif_rx_schedule(m->mac_dev)
#define athr_mac_cache_inv(d, s)                \
do {    				        \
        dma_cache_inv((unsigned long)d, s);	\
} while (0)

#define ATHR_MAC_TASK_ARG	athr_gmac_t *mac

#define ATHR_MAC_TASK_MAC()
#define ATHR_MAC_NAPI_MAC()	                \
        athr_gmac_t *mac = ATHR_MAC_PRIV(dev)
#define ATHR_GET_DEV()
#define ATHR_MAC_RX_COMPLETE(mac)       netif_rx_complete(mac->mac_dev)
#define __ATHR_MAC_RX_COMPLETE(mac)     netif_rx_complete(mac->mac_dev)
#define ATHR_MAC_RX_RESCHED(mac)        /* nothing */
#define ATHR_NAPI_CHECK_STATUS()        /* nothing */
#define ATHR_NAPI_HANDLE_RX_NOT_DONE()	/* nothing */
#define ATHR_NAPI_HANDLE_RX_DONE()				\
do {							\
        __ATHR_MAC_RX_COMPLETE(mac);			\
        spin_lock_irqsave(&mac->mac_lock, flags);       \
        athr_gmac_intr_enable_recv(mac);		\
        spin_unlock_irqrestore(&mac->mac_lock, flags);	\
} while (0)


#define ATHR_MAC_UPDATE_QUOTA_BUDGET(quota, budget, work_done)  \
do {                            \
        quota -= work_done;     \
        *budget -= work_done;   \
} while(0)

#define ATHR_MAC_GET_MAX_WORK(budget, dev)    \
    min(*budget, dev->quota)
#define  IF_RX_STATUS_DONE(ret,ATHR_GMAC_RX_STATUS_DONE,work_done,budget) \
    if (likely(ret == ATHR_GMAC_RX_STATUS_DONE))

#define ATHR_MAC_INIT_WORK(m, f)	INIT_WORK(&m->mac_tx_timeout, (void *)f, m)
#define ATHR_MAC_ETHERDEV_SZ	0
#define ATHR_MAC_IRQF_DISABLED	0

#define ATHR_MAC_POLL() \
   athr_gmac_poll(struct net_device *dev, int *budget)

#define ATHR_MAC_SET_DEV_NET_OPS(dev, mac,              \
                                 athr_gmac_get_stats,   \
                                 athr_gmac_open,        \
                                 athr_gmac_stop,        \
                                 athr_gmac_hard_start,  \
                                 athr_gmac_do_ioctl,    \
                                 athr_gmac_poll,        \
                                 athr_gmac_tx_timeout,  \
                                 ATHR_MAC_NAPI_WEIGHT,  \
                                 tx_qlen)               \
do {                                                    \
        dev->get_stats       =  athr_gmac_get_stats;    \
        dev->open            =  athr_gmac_open;         \
        dev->stop            =  athr_gmac_stop;         \
        dev->hard_start_xmit =  athr_gmac_hard_start;   \
        dev->do_ioctl        =  athr_gmac_do_ioctl;     \
        dev->poll            =  athr_gmac_poll;         \
        dev->weight          =  ATHR_MAC_NAPI_WEIGHT;   \
        dev->tx_timeout      =  athr_gmac_tx_timeout;   \
        dev->priv            =  mac;                    \
        dev->tx_queue_len    =  tx_qlen;                \
} while(0)

#endif /* #if (LINUX_VERSION_CODE >= KERNEL_VERSION(2,6,31)) */

/*
 * Deduce if tasklets are available.  If not then
 * fall back to using the immediate work queue.
 */
#include <linux/interrupt.h>
#ifdef DECLARE_TASKLET          /* native tasklets */
#define tq_struct tasklet_struct
#define ATH_INIT_TQUEUE(a,b,c)      tasklet_init((a),(b),(unsigned long)(c))
#define ATH_SCHEDULE_TQUEUE(a,b)    tasklet_schedule((a))
typedef unsigned long TQUEUE_ARG;
#define mark_bh(a)
#else                   /* immediate work queue */
#define ATH_INIT_TQUEUE(a,b,c)      INIT_TQUEUE(a,b,c)
#define ATH_SCHEDULE_TQUEUE(a,b) do {       \
    *(b) |= queue_task((a), &tq_immediate); \
} while(0)
typedef void *TQUEUE_ARG;
#define tasklet_disable(t)  do { (void) t; local_bh_disable(); } while (0)
#define tasklet_enable(t)   do { (void) t; local_bh_enable(); } while (0)
#endif /* !DECLARE_TASKLET */

#include <linux/sched.h>
#if LINUX_VERSION_CODE < KERNEL_VERSION(2,5,41)
#include <linux/tqueue.h>
#define ATH_WORK_THREAD            tq_struct
#define ATH_SCHEDULE_TASK(t)        schedule_task((t))
#define ATH_INIT_SCHED_TASK(t, f, d)    do { memset((t),0,sizeof(struct tq_struct)); \
                        (t)->routine = (void (*)(void*)) (f); \
                        (t)->data=(void *) (d); } while (0)
#define ATH_FLUSH_TASKS            flush_scheduled_tasks
#else
#include <linux/workqueue.h>
#define ATH_SCHEDULE_TASK(t)        schedule_work((t))
//#define ATH_INIT_SCHED_TASK(t, f, d)    (DECLARE_WORK((t), (f), (d)))
#define ATH_INIT_SCHED_TASK(t, f, d)    do { memset(((void *) (t)),0,sizeof(struct work_struct)); \
        PREPARE_WORK((t),((void (*)(void*))(f)),((void *) (d))); } while (0)
#define ATH_WORK_THREAD            work_struct
#define    ATH_FLUSH_TASKS            flush_scheduled_work
#endif /* KERNEL_VERSION < 2.5.41 */


#endif /* #ifndef __OSDEP_H */
