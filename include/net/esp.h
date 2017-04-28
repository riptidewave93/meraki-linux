#ifndef _NET_ESP_H
#define _NET_ESP_H

#include <linux/skbuff.h>

struct crypto_aead;

struct esp_data {
	/* 0..255 */
	int padlen;

	/* Confidentiality & Integrity */
	struct crypto_aead *aead;
#if CONFIG_INET_ESP_NR_REQ_CACHE > 0
#define ESP_NFRAGS_CACHE	10 /* total number of frag for cache */
#define ESP_REQ_CACHE_MAX	CONFIG_INET_ESP_NR_REQ_CACHE
	void		  *req_cache[ESP_REQ_CACHE_MAX];
	atomic_t	   req_cache_cnt;
	int		   req_cache_head;
	int		   req_cache_tail;
#endif
	int		   req_cache_size; /* request size without frag sg */
};

extern void *pskb_put(struct sk_buff *skb, struct sk_buff *tail, int len);

struct ip_esp_hdr;

static inline struct ip_esp_hdr *ip_esp_hdr(const struct sk_buff *skb)
{
	return (struct ip_esp_hdr *)skb_transport_header(skb);
}

#endif
