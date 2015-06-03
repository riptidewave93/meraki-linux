#include <crypto/aead.h>
#include <crypto/authenc.h>
#include <linux/err.h>
#include <linux/module.h>
#include <net/ip.h>
#include <net/xfrm.h>
#include <net/esp.h>
#include <linux/scatterlist.h>
#include <linux/kernel.h>
#include <linux/pfkeyv2.h>
#include <linux/rtnetlink.h>
#include <linux/slab.h>
#include <linux/spinlock.h>
#include <linux/in6.h>
#include <net/icmp.h>
#include <net/protocol.h>
#include <net/udp.h>
#include <linux/highmem.h>

//#define DEBUG_ESP
#ifdef DEBUG_ESP
#	define ESP_DUMP_PKT		print_hex_dump
#	define ESP_PRINTK		printk
#else
#	define ESP_DUMP_PKT(arg...)
#	define ESP_PRINTK(arg...)	
#endif

struct esp_skb_cb {
	struct xfrm_skb_cb xfrm;
	void *req_ctx;
	struct esp_data *esp;	
	char flags;
};

#define ESP_SKB_CB(__skb) ((struct esp_skb_cb *)&((__skb)->cb[0]))

struct esp_offload_param {
	unsigned short encap_sport;
	unsigned short encap_dport;
	unsigned char nh;
	unsigned char pad_len;
} __attribute__((packed));

#define ESP_OFFLOAD_INFO_SIZE	ALIGN(sizeof(struct esp_offload_param), 4)

/*
 * Allocate an AEAD request structure with extra space for SG, IV, and 16
 * bytes info data for HW offload.
 *
 * For alignment considerations the IV is placed at the front, followed
 * by the request and finally the SG list.
 *
 * TODO: Use spare space in skb for this where possible.
 */

static int esp_req_ctx_size(struct crypto_aead *aead, int nfrags)
{
	unsigned int len;

	len  = ESP_OFFLOAD_INFO_SIZE;
	len += crypto_aead_ivsize(aead);
	len += crypto_aead_alignmask(aead) &
		~(crypto_tfm_ctx_alignment() - 1);
	len = ALIGN(len, crypto_tfm_ctx_alignment());
	
	len += sizeof(struct aead_givcrypt_request) + crypto_aead_reqsize(aead);
	len = ALIGN(len, __alignof__(struct scatterlist));

	len += sizeof(struct scatterlist) * nfrags;

	return len;
}

static void *esp_alloc_req_ctx( struct esp_skb_cb *esp_skb, 
				struct crypto_aead *aead,
				int nfrags)
{
	void 		*ctx;
	unsigned int 	len;
	struct esp_data *esp = esp_skb->esp;

#if CONFIG_INET_ESP_NR_REQ_CACHE > 0
	if (nfrags <= ESP_NFRAGS_CACHE) {
		esp_skb->flags |= 0x01;
		if (atomic_read(&esp->req_cache_cnt)) {
			ctx = esp->req_cache[esp->req_cache_head];
			esp->req_cache_head = (esp->req_cache_head + 1) %
					ESP_REQ_CACHE_MAX;
			atomic_dec(&esp->req_cache_cnt);
			return ctx;
		}
		len  = esp->req_cache_size +
			sizeof(struct scatterlist) * ESP_NFRAGS_CACHE;
		ctx = kmalloc(len, GFP_ATOMIC);
	} else 	{
		esp_skb->flags &= ~0x01;
		len  = esp->req_cache_size +
				sizeof(struct scatterlist) * nfrags;
		ctx = kmalloc(len, GFP_ATOMIC);
	}
#else
	len  = esp->req_cache_size + 
		sizeof(struct scatterlist) * nfrags;
	ctx = kmalloc(len, GFP_ATOMIC);		
#endif
	return ctx;
}

static void esp_free_req_ctx(struct esp_skb_cb *esp_skb)
{
#if CONFIG_INET_ESP_NR_REQ_CACHE > 0
	struct esp_data *esp = esp_skb->esp;
			     
	if (esp_skb->flags & 0x01) {
		if (atomic_read(&esp->req_cache_cnt) < ESP_REQ_CACHE_MAX) {
			esp->req_cache[esp->req_cache_tail] = esp_skb->req_ctx;
			esp->req_cache_tail = (esp->req_cache_tail + 1) %
					ESP_REQ_CACHE_MAX;
			atomic_inc(&esp->req_cache_cnt);
			return;
		}
	}
#endif
	kfree(esp_skb->req_ctx);
}

static inline void *esp_tmp_offload_info(void *tmp)
{
	return tmp;
}

static inline u8 *esp_tmp_iv(struct crypto_aead *aead, void *tmp)
{
	return crypto_aead_ivsize(aead) ?
	       PTR_ALIGN((u8 *) tmp + ESP_OFFLOAD_INFO_SIZE,
	       		 crypto_aead_alignmask(aead) + 1) :
			 ((u8 *) tmp + ESP_OFFLOAD_INFO_SIZE);
}

static inline struct aead_givcrypt_request *esp_tmp_givreq(
	struct crypto_aead *aead, u8 *iv)
{
	struct aead_givcrypt_request *req;

	req = (void *)PTR_ALIGN(iv + crypto_aead_ivsize(aead),
				crypto_tfm_ctx_alignment());
	ESP_PRINTK("printing ivsize = %d\n", crypto_aead_ivsize(aead));
	aead_givcrypt_set_tfm(req, aead);
	return req;
}

static inline struct aead_request *esp_tmp_req(struct crypto_aead *aead, u8 *iv)
{
	struct aead_request *req;

	req = (void *)PTR_ALIGN(iv + crypto_aead_ivsize(aead),
				crypto_tfm_ctx_alignment());
	aead_request_set_tfm(req, aead);
	return req;
}

static inline struct scatterlist *esp_req_sg(struct crypto_aead *aead,
					     struct aead_request *req)
{
	return (void *)ALIGN((unsigned long)(req + 1) +
			     crypto_aead_reqsize(aead),
			     __alignof__(struct scatterlist));
}

static inline struct scatterlist *esp_givreq_sg(
	struct crypto_aead *aead, struct aead_givcrypt_request *req)
{
	return (void *)ALIGN((unsigned long)(req + 1) +
			     crypto_aead_reqsize(aead),
			     __alignof__(struct scatterlist));
}

static void esp_output_done(struct crypto_async_request *base, int err)
{
	struct sk_buff    *skb = base->data;

	ESP_DUMP_PKT(KERN_DEBUG, "ESP output done sw: ", DUMP_PREFIX_ADDRESS,
		     16, 4, skb->data, skb->len - skb->data_len, 1);

	esp_free_req_ctx(ESP_SKB_CB(skb));
	xfrm_output_resume(skb, err);
}

static int esp_output_sw(struct xfrm_state *x, struct sk_buff *skb)
{
	struct ip_esp_hdr *esph;
	struct crypto_aead *aead;
	struct aead_givcrypt_request *req;
	struct scatterlist *sg;
	struct scatterlist *asg;
	struct esp_data *esp;
	struct sk_buff *trailer;
	void *tmp;
	int err;
	u8  *iv;
	u8  *tail;
	int blksize;
	int clen;
	int alen;
	int nfrags;

	/* skb is pure payload to encrypt */

	err = -ENOMEM;

	ESP_DUMP_PKT(KERN_DEBUG, "ESP output sw: ", DUMP_PREFIX_ADDRESS,
		     16, 4, skb->data, skb->len - skb->data_len, 1);
	
	/* Round to block size */
	clen = skb->len;

	esp = x->data;
	aead = esp->aead;
	alen = crypto_aead_authsize(aead);

	blksize = ALIGN(crypto_aead_blocksize(aead), 4);
	ESP_PRINTK("printing blocksize = %d\n", crypto_aead_blocksize(aead));
	ESP_PRINTK("clen = %d\n", clen);
	clen = ALIGN(clen + 2, blksize);
	ESP_PRINTK("clen after = %d\n", clen);

	if (esp->padlen)
		clen = ALIGN(clen, esp->padlen);

	if ((err = skb_cow_data(skb, clen - skb->len + alen, &trailer)) < 0)
		goto error;
	nfrags = err;

	ESP_SKB_CB(skb)->esp = esp;
	
	tmp = esp_alloc_req_ctx(ESP_SKB_CB(skb), aead, nfrags + 1);
	if (!tmp)
		goto error;

	iv = esp_tmp_iv(aead, tmp);
	req = esp_tmp_givreq(aead, iv);
	asg = esp_givreq_sg(aead, req);
	sg = asg + 1;

	/* Fill padding... */
	tail = skb_tail_pointer(trailer);
	do {
		int i;
		for (i=0; i<clen-skb->len - 2; i++)
			tail[i] = i + 1;
	} while (0);
	tail[clen - skb->len - 2] = (clen - skb->len) - 2;
	tail[clen - skb->len - 1] = *skb_mac_header(skb);

	pskb_put(skb, trailer, clen - skb->len + alen);

	skb_push(skb, -skb_network_offset(skb));
	esph = ip_esp_hdr(skb);
	*skb_mac_header(skb) = IPPROTO_ESP;

	/* this is non-NULL only with UDP Encapsulation */
	if (x->encap) {
		struct xfrm_encap_tmpl *encap = x->encap;
		struct udphdr *uh;
		__be32 *udpdata32;
		__be16 sport, dport;
		int encap_type;

		spin_lock_bh(&x->lock);
		sport = encap->encap_sport;
		dport = encap->encap_dport;
		encap_type = encap->encap_type;
		spin_unlock_bh(&x->lock);

		uh = (struct udphdr *)esph;
		uh->source = sport;
		uh->dest = dport;
		uh->len = htons(skb->len - skb_transport_offset(skb));
		uh->check = 0;

		switch (encap_type) {
		default:
		case UDP_ENCAP_ESPINUDP:
			esph = (struct ip_esp_hdr *)(uh + 1);
			break;
		case UDP_ENCAP_ESPINUDP_NON_IKE:
			udpdata32 = (__be32 *)(uh + 1);
			udpdata32[0] = udpdata32[1] = 0;
			esph = (struct ip_esp_hdr *)(udpdata32 + 2);
			break;
		}

		*skb_mac_header(skb) = IPPROTO_UDP;
	}
	esph->spi = x->id.spi;
	esph->seq_no = htonl(XFRM_SKB_CB(skb)->seq.output);

	sg_init_table(sg, nfrags);
	skb_to_sgvec(skb, sg,
		     esph->enc_data + crypto_aead_ivsize(aead) - skb->data,
		     clen + alen);
	sg_init_one(asg, esph, sizeof(*esph));

	aead_givcrypt_set_callback(req, 0, esp_output_done, skb);
	aead_givcrypt_set_crypt(req, sg, sg, clen, iv);
	aead_givcrypt_set_assoc(req, asg, sizeof(*esph));
	aead_givcrypt_set_giv(req, esph->enc_data, 
			      XFRM_SKB_CB(skb)->seq.output);

	ESP_SKB_CB(skb)->req_ctx = tmp;
		
	err = crypto_aead_givencrypt(req);
	if (err == -EINPROGRESS)
		goto error;

	if (err == -EBUSY)
		err = NET_XMIT_DROP;
	
	ESP_DUMP_PKT(KERN_DEBUG, "ESP output sw: ", DUMP_PREFIX_ADDRESS,
		     16, 4, skb->data, skb->len - skb->data_len, 1);
	
	esp_free_req_ctx(ESP_SKB_CB(skb));

error:
	return err;
}

static int esp_output_done2_hw(struct crypto_async_request *base, int err)
{
	struct sk_buff    *skb = base->data;
	void           	  *esp_req = ESP_SKB_CB(skb)->req_ctx;
	struct esp_offload_param *param;

	ESP_DUMP_PKT(KERN_DEBUG, "ESP output done2 hw 1: ", DUMP_PREFIX_ADDRESS,
		     16, 4, skb->data, skb->len - skb->data_len, 1);

	if (err >= 0) {
		err = 0;
	}
	else {
		if ( err < 0)
			goto out;
	}
        param = esp_tmp_offload_info(esp_req);
	if (param->encap_sport) {
		/* UDP Encapsulation - Fill in dynamic fields */
                struct iphdr  *iph = ip_hdr(skb);
                int           ihl  = iph->ihl << 2;
                struct udphdr *uh  = (struct udphdr *) (((u8 *) iph) + ihl);
		uh->source = param->encap_sport;
		uh->dest   = param->encap_dport;
		*skb_mac_header(skb) = IPPROTO_UDP;
        } else {
	        *skb_mac_header(skb) = IPPROTO_ESP;
	}
	ESP_DUMP_PKT(KERN_DEBUG, "ESP output done2 hw 2: ", DUMP_PREFIX_ADDRESS,
		     16, 4, skb->data, skb->len - skb->data_len, 1);

out:
	esp_free_req_ctx(ESP_SKB_CB(skb));
	return err;
}

static void esp_output_done_hw(struct crypto_async_request *base, int err)
{
	struct sk_buff *skb = base->data;

	xfrm_output_resume(skb, esp_output_done2_hw(base, err));
}

static int esp_output_hw(struct xfrm_state *x, struct sk_buff *skb)
{
	struct crypto_aead  *aead;
	struct aead_request *req;
	struct scatterlist  *sg;
	struct scatterlist  *dsg;
	struct esp_data     *esp;
	struct sk_buff      *trailer;
	struct esp_offload_param *param;
	void *esp_req;
	int  err;
	u8   *iv;
	int  clen;
	int  alen;
	int  pad_len;
	int  nfrags;

	/* For ESP transport mode, skb.data is at IP header. skb.len
	   includes IP header and payload. skb network header, transport
	   header, and mac headers pointer are updated by transport module
	   code.

	  Input:
	  --------------------------------------------
	  | Network Hdr| Transport Hdr| IP | Payload |
	  --------------------------------------------
	                              ^
	      			      |
	     			      skb.data

	  For ESP tunnel mode, outer IP header is formed by tunnel module.
	  skb network header, transport header, and mac header pointer are
	  updated by tunnel module code.

   	  Input:
	  -----------------------------------------------------
	  | Outer IP | reserved1  | inner IP Header | Payload |
	  -----------------------------------------------------
	                         ^
				 |
				 skb.data

	  Outer IP   - Formed by tunnel mode code
	  reserved1* - space reserved for UDP, ESP, SEQ, and IV
	*/
	
	ESP_DUMP_PKT(KERN_DEBUG, "ESP output hw: ", DUMP_PREFIX_ADDRESS,
		     16, 4, skb->data, skb->len - skb->data_len, 1);

	err  = -ENOMEM;

	esp  = x->data;
	aead = esp->aead;
	alen = crypto_aead_authsize(aead);

	/* Compute pad length to expand skb tail for padding by HW */
	if (x->props.mode == XFRM_MODE_TUNNEL) {
		clen = skb->len;
	} else {
		 struct iphdr *iph = (struct iphdr *) skb->data;
		 clen = skb->len - (iph->ihl << 2); 
	}

	ESP_PRINTK("clen = %d\n", clen);
	pad_len = ALIGN(clen + 2, ALIGN(crypto_aead_blocksize(aead), 4));
	if (esp->padlen)
		pad_len = ALIGN(pad_len, esp->padlen);
	ESP_PRINTK("printing crypto_aead_blocksize(aead) = %d\n",
		   crypto_aead_blocksize(aead));
	pad_len -= clen;

	ESP_PRINTK("pad_len = %d\n", pad_len);
	/* Expand tailer to include padding and ICV */
	//printk("calling skb_cow_data 1\n");
	if ((err = skb_cow_data(skb, pad_len + alen, &trailer)) < 0)
		goto error;
	nfrags = err;

	ESP_SKB_CB(skb)->esp     = esp;

	esp_req = esp_alloc_req_ctx(ESP_SKB_CB(skb), aead, nfrags * 2);
	if (!esp_req)
		goto error;

	param = esp_tmp_offload_info(esp_req);
	iv    = esp_tmp_iv(aead, param);		/* Not used */
	req   = esp_tmp_req(aead, iv);
	sg    = esp_req_sg(aead, req);
	dsg   = sg + nfrags;

	/* Setup SG */
	sg_init_table(sg, nfrags);
	skb_to_sgvec(skb, sg, 0, skb->len);

	/* Prepare SKB to include padded length and ESP header */
	clen = skb->len;
	pskb_put(skb, trailer, pad_len + alen);
	skb_push(skb, x->props.header_len);
	sg_init_table(dsg, nfrags);
        if (x->props.mode == XFRM_MODE_TUNNEL)
	       skb_to_sgvec(skb, dsg, 20, skb->len - 20);
        else 
	       skb_to_sgvec(skb, dsg, 0, skb->len);

	ESP_PRINTK("in esp_output hw, pad_len = %d, aen = %d, skblen = %d\n",
		   pad_len, alen, skb->len);
	ESP_DUMP_PKT(KERN_DEBUG, "ESP output hw: ", DUMP_PREFIX_ADDRESS,
		     16, 4, skb->data, skb->len - skb->data_len, 1);
	/* This is non-NULL only with UDP Encapsulation.
	   UDP Encapsulation is done by hardware. Port of UDP encapsulation
	   info are passed to hardware offload driver at ESP init time. Dynamic
	   info per a packet are saved and filled in after hardware offload
         */
	if (x->encap) {
		struct xfrm_encap_tmpl *encap = x->encap;

		spin_lock_bh(&x->lock);
		param->encap_sport = encap->encap_sport;
		param->encap_dport = encap->encap_dport;
		spin_unlock_bh(&x->lock);
      	} else {
		param->encap_sport = 0;
	}

	/* Format input parameters for hardware ESP offload algorithm.
	   For ESP transport mode, IP header and payload are passed as
	   the source sg. All associate data such as SPI, seq and etc,
	   are loaded at init time.

	   Input Src:
	   ----------------------------------------
	   | reserved1 | IP | Payload | reserved2 |
	   ----------------------------------------
	               ^
	               |
	               start if source sg
	               
	   ----------------------------------------
	   | reserved1 | IP | Payload | reserved2 |
	   ----------------------------------------
           ^
           |
           start if destination sg
	               
	   reserved1 - space reserved for header (UDP ENCAP, ESP, & IV)
	   reserved2 - space reserved for padding + ICV

	   Output:
	   -------------------------------------------------------------
	   | IP | UDP ENCAP | ESP | IV | Payload | IPSec Padding | ICV |
	   -------------------------------------------------------------
	   UDP ENCAP if NAT

	   For ESP tunnel mode, IP header and payload is passed as
	   the source sg. All associate data such as SPI, seq and etc,
	   are loaded at init time. The outer IP is formed by tunnel module.

	   Input:
	   ----------------------------------------------------
	   | Outer IP | reserved1 | inner IP Header | Payload |
	   ----------------------------------------------------
	   			  ^
	   			  |
	   			  start of source sg
	   
	   ----------------------------------------------------
	   | Outer IP | reserved1 | inner IP Header | Payload |
	   ----------------------------------------------------
		      ^           (inner IP header not moved)
           	      |
           	      start if destination sg               
	   
	   Outer IP  - formed by tunnel mode code
	   reserved1 - space reserved for UDP, ESP, SEQ, and IV
	   inner IP Header - Start of sg. length is inner IP Header + payload

	   Output:
	   ------------------------------------------------------------------
	   |Outer IP|UDP ENCAP|SPI+SEQ|IV|Inner IP|Payload|IPSec Padding|ICV|
	   ------------------------------------------------------------------
	   Outer IP  - Formed by tunnel mode code
	   UDP ENCAP if NAT

	*/
	aead_request_set_callback(req, 0, esp_output_done_hw, skb);
	aead_request_set_crypt(req, sg, dsg, clen, iv);

	ESP_SKB_CB(skb)->req_ctx = esp_req;

	err = crypto_aead_encrypt(req);
	if (err == -EINPROGRESS)
		goto error;

	if (err == -EBUSY)
		err = NET_XMIT_DROP;

	err = esp_output_done2_hw(&req->base, err);

error:
	return err;
}

static int esp_output(struct xfrm_state *x, struct sk_buff *skb)
{

	if (x->alg_flags & XFRM_ALGO_FLAGS_OFFLOAD_ESP)
		return esp_output_hw(x, skb);
	else
		return esp_output_sw(x, skb);
}

static int esp_input_done2(struct sk_buff *skb, int err)
{
	struct iphdr *iph;
	struct xfrm_state  *x    = xfrm_input_state(skb);
	struct esp_data    *esp  = ESP_SKB_CB(skb)->esp;
	struct crypto_aead *aead = esp->aead;
	int alen = crypto_aead_authsize(aead);
	int hlen = sizeof(struct ip_esp_hdr) + crypto_aead_ivsize(aead);
	int elen = skb->len - hlen;
	int ihl;
	u8 nexthdr[2];
	int padlen;
	
        esp_free_req_ctx(ESP_SKB_CB(skb));

	if (unlikely(err))
		goto out;

	if (skb_copy_bits(skb, skb->len-alen-2, nexthdr, 2))
		BUG();

	ESP_DUMP_PKT(KERN_DEBUG, "ESP input sw done 1: ", DUMP_PREFIX_ADDRESS,
		     16, 4, skb->data, skb->len - skb->data_len, 1);


	err = -EINVAL;
	padlen = nexthdr[0];

	ESP_PRINTK("padlen in sw = %d, alen = %d, elen = %d\n",
		   padlen, alen, elen);
	if (padlen + 2 + alen >= elen)
		goto out;

	/* ... check padding bits here. Silly. :-) */

	iph = ip_hdr(skb);
	ihl = iph->ihl * 4;

	if (x->encap) {
		struct xfrm_encap_tmpl *encap = x->encap;
		struct udphdr *uh = (void *)(skb_network_header(skb) + ihl);

		/*
		 * 1) if the NAT-T peer's IP or port changed then
		 *    advertize the change to the keying daemon.
		 *    This is an inbound SA, so just compare
		 *    SRC ports.
		 */
		if (iph->saddr != x->props.saddr.a4 ||
		    uh->source != encap->encap_sport) {
			xfrm_address_t ipaddr;

			ipaddr.a4 = iph->saddr;
			km_new_mapping(x, &ipaddr, uh->source);

			/* XXX: perhaps add an extra
			 * policy check here, to see
			 * if we should allow or
			 * reject a packet from a
			 * different source
			 * address/port.
			 */
		}

		/*
		 * 2) ignore UDP/TCP checksums in case
		 *    of NAT-T in Transport Mode, or
		 *    perform other post-processing fixes
		 *    as per draft-ietf-ipsec-udp-encaps-06,
		 *    section 3.1.2
		 */
		if (x->props.mode == XFRM_MODE_TRANSPORT)
			skb->ip_summed = CHECKSUM_UNNECESSARY;
	}

	pskb_trim(skb, skb->len - alen - padlen - 2);
	__skb_pull(skb, hlen);
	skb_set_transport_header(skb, -ihl);

	err = nexthdr[1];
	/* RFC4303: Drop dummy packets without any error */
	if (err == IPPROTO_NONE)
		err = -EINVAL;
	
	ESP_DUMP_PKT(KERN_DEBUG, "ESP input sw done: ", DUMP_PREFIX_ADDRESS,
		     16, 4, skb->data, skb->len - skb->data_len, 1);

out:
	return err;
}

static void esp_input_done(struct crypto_async_request *base, int err)
{
	struct sk_buff *skb = base->data;

	xfrm_input_resume(skb, esp_input_done2(skb, err));
}

/*
 * Note: detecting truncated vs. non-truncated authentication data is very
 * expensive, so we only support truncated data, which is the recommended
 * and common case.
 */
static int esp_input_sw(struct xfrm_state *x, struct sk_buff *skb)
{
	struct ip_esp_hdr *esph;
	struct esp_data *esp = x->data;
	struct crypto_aead *aead = esp->aead;
	struct aead_request *req;
	struct sk_buff *trailer;
	int elen = skb->len - sizeof(*esph) - crypto_aead_ivsize(aead);
	int nfrags;
	void *tmp;
	u8 *iv;
	struct scatterlist *sg;
	struct scatterlist *asg;
	int err = -EINVAL;

	ESP_DUMP_PKT(KERN_DEBUG, "ESP input sw: ", DUMP_PREFIX_ADDRESS,
		     16, 4, skb->data, skb->len - skb->data_len, 1);
	
	if (!pskb_may_pull(skb, sizeof(*esph) + crypto_aead_ivsize(aead)))
		goto out;

	if (elen <= 0)
		goto out;

	if ((err = skb_cow_data(skb, 0, &trailer)) < 0)
		goto out;
	nfrags = err;

	err = -ENOMEM;
	
	ESP_SKB_CB(skb)->esp     = esp;
	
	tmp = esp_alloc_req_ctx(ESP_SKB_CB(skb), aead, nfrags + 1);
	if (!tmp)
		goto out;

	ESP_SKB_CB(skb)->req_ctx = tmp;
	iv  = esp_tmp_iv(aead, tmp);
	req = esp_tmp_req(aead, iv);
	asg = esp_req_sg(aead, req);
	sg  = asg + 1;

	skb->ip_summed = CHECKSUM_NONE;

	esph = (struct ip_esp_hdr *)skb->data;

	/* Get ivec. This can be wrong, check against another impls. */
	iv = esph->enc_data;

	sg_init_table(sg, nfrags);
	skb_to_sgvec(skb, sg, sizeof(*esph) + crypto_aead_ivsize(aead), elen);
	sg_init_one(asg, esph, sizeof(*esph));

	aead_request_set_callback(req, 0, esp_input_done, skb);
	aead_request_set_crypt(req, sg, sg, elen, iv);
	aead_request_set_assoc(req, asg, sizeof(*esph));
	
	err = crypto_aead_decrypt(req);
	if (err == -EINPROGRESS)
		goto out;

	err = esp_input_done2(skb, err);

out:
	return err;
}

static int esp_input_done2_hw(struct sk_buff *skb, int err)
{
	struct iphdr       *iph;
	struct xfrm_state  *x    = xfrm_input_state(skb);
	struct esp_data    *esp  = ESP_SKB_CB(skb)->esp;	
	struct crypto_aead *aead = esp->aead;
	int alen = crypto_aead_authsize(aead);
	int hlen = sizeof(struct ip_esp_hdr) + crypto_aead_ivsize(aead);
	int elen = skb->len - hlen;
	int ihl;
	u8  nexthdr[2];
	int padlen;
	void *esp_req = ESP_SKB_CB(skb)->req_ctx;
	
	ESP_DUMP_PKT(KERN_INFO, "ESP input done 0 hw: ", DUMP_PREFIX_ADDRESS,
		     16, 4, skb->data, skb->len - skb->data_len, 1);

	if (err >= 0) {
		if ( err == 0) {
			if (skb_copy_bits(skb, skb->len-alen-2, nexthdr, 2))
				BUG();
		}
		else {
			nexthdr[0] = (unsigned char)(err >>  8);
			nexthdr[1] = (unsigned char)(err >>  0);
		}	
	}	
	else {
		goto out;
	}

	ESP_DUMP_PKT(KERN_INFO, "ESP input done 1 hw: ", DUMP_PREFIX_ADDRESS,
		     16, 4, skb->data, skb->len - skb->data_len, 1);

	ESP_PRINTK("alen = %d\n", alen);
	err    = -EINVAL;
	padlen = nexthdr[0]/*  + 2 */;
	ESP_PRINTK("printing padlen = %d\n", padlen);

	if (padlen + alen > elen)
		goto out;

	ESP_PRINTK("now calculating iph\n");
	iph = ip_hdr(skb);
	ihl = iph->ihl * 4;

	
	if (x->encap) {
		struct xfrm_encap_tmpl   *encap = x->encap;
		struct esp_offload_param *param = esp_tmp_offload_info(esp_req);
		/*
		 * 1) if the NAT-T peer's IP or port changed then
		 *    advertize the change to the keying daemon.
		 *    This is an inbound SA, so just compare
		 *    SRC ports.
		 */
		if (iph->saddr != x->props.saddr.a4 ||
		    param->encap_sport != encap->encap_sport) {
			xfrm_address_t ipaddr;

			ipaddr.a4 = iph->saddr;
			km_new_mapping(x, &ipaddr, param->encap_sport);

			/* XXX: perhaps add an extra
			 * policy check here, to see
			 * if we should allow or
			 * reject a packet from a
			 * different source
			 * address/port.
			 */
		}

		/*
		 * 2) ignore UDP/TCP checksums in case
		 *    of NAT-T in Transport Mode, or
		 *    perform other post-processing fixes
		 *    as per draft-ietf-ipsec-udp-encaps-06,
		 *    section 3.1.2
		 */
		if (x->props.mode == XFRM_MODE_TRANSPORT)
			skb->ip_summed = CHECKSUM_UNNECESSARY;
	}

	pskb_trim(skb, skb->len - alen - padlen);
	__skb_pull(skb, hlen);
	skb_set_transport_header(skb, -ihl);

	err = nexthdr[1];
	
	ESP_DUMP_PKT(KERN_INFO, "ESP input done 2 hw: ", DUMP_PREFIX_ADDRESS,
		     16, 4, skb->data, skb->len - skb->data_len, 1);

	/* RFC4303: Drop dummy packets without any error */
	if (err == IPPROTO_NONE)
		err = -EINVAL;
out:
	esp_free_req_ctx(ESP_SKB_CB(skb));
	return err;
}

static void esp_input_done_hw(struct crypto_async_request *base, int err)
{
	struct sk_buff *skb = base->data;

	xfrm_input_resume(skb, esp_input_done2_hw(skb, err));
}

static int esp_input_hw(struct xfrm_state *x, struct sk_buff *skb)
{
	struct ip_esp_hdr   *esph;
	struct esp_data     *esp  = x->data;
	struct crypto_aead  *aead = esp->aead;
	struct aead_request *req;
	struct sk_buff      *trailer;
	int  elen = skb->len - sizeof(*esph) - crypto_aead_ivsize(aead);
	int  nfrags;
	void *esp_req;
	u8   *iv;
	struct scatterlist       *sg;
	struct scatterlist       *dsg;
	struct esp_offload_param *param;
	int  err = -EINVAL;
	int  hdr_len;
	int  clen;
	int  src_len;

	ESP_DUMP_PKT(KERN_DEBUG, "ESP input hw: ", DUMP_PREFIX_ADDRESS,
		     16, 4, skb->data, skb->len - skb->data_len, 1);


	/* For ESP transport mode, skb.data is at ESP header. skb.len
	   includes IP header and payload. skb network header, transport
	   header, and mac header is updated by transport module code.

	   Input:
	   -------------------------------------------------------------
	   | IP Hdr | UDP ENCAP | ESP | IV | Payload | IPSec Pad | ICV |
	   -------------------------------------------------------------
	                        ^
	                        |
	                        skb.data and length start here
	   For ESP tunnel mode,

   	   Input:
	   ----------------------------------------------------------
	   |Outer IP|UDP ENCAP|ESP|IV|inner IP|Payload|IPSec Pad|ICV|
	   ----------------------------------------------------------
	                      ^
			      |
			      skb.data and length start here
	*/
	if (!pskb_may_pull(skb, sizeof(*esph)))
		goto out;

	if (elen <= 0)
		goto out;

	if ((err = skb_cow_data(skb, 0, &trailer)) < 0)
		goto out;
	nfrags = err;

	err = -ENOMEM;

	ESP_SKB_CB(skb)->esp     = esp;
	
	esp_req = esp_alloc_req_ctx(ESP_SKB_CB(skb), aead, nfrags * 2);
	if (!esp_req)
		goto out;

	ESP_SKB_CB(skb)->req_ctx = esp_req;
	param = esp_tmp_offload_info(esp_req);
	iv    = esp_tmp_iv(aead, param);
	req   = esp_tmp_req(aead, iv);
	sg    = esp_req_sg(aead, req);
	dsg   = sg + nfrags;

	skb->ip_summed = CHECKSUM_NONE;

	/* This is non-NULL only with UDP Encapsulation.
	   UDP Encapsulation is done after ESP processing. Save info as
	   required as hardware offload driver can override these info. */
	if (x->encap) {
		struct iphdr  *iph = ip_hdr(skb);
		struct udphdr *uh;
		uh = (struct udphdr *) ((u8 *) iph + iph->ihl * 4);
		param->encap_sport = uh->source;
	}

	/* Setup SG */
	sg_init_table(sg, nfrags);
	hdr_len = skb_network_offset(skb);
	if (x->props.mode == XFRM_MODE_TUNNEL) {
		clen = skb->len;
		skb_to_sgvec(skb, sg, 0, clen);
	} else {
		clen = -hdr_len + skb->len;
		skb_to_sgvec(skb, sg, hdr_len, clen);
	}
	src_len = clen;

	sg_init_table(dsg, nfrags);
        if (x->props.mode == XFRM_MODE_TUNNEL) {	
		clen -= hdr_len + x->props.header_len;
		skb_to_sgvec(skb, dsg, hdr_len + x->props.header_len, clen);
	} else {
		clen -= crypto_aead_authsize(aead);
		clen -= (x->props.header_len);
		 /* clen -= crypto_aead_ivsize(aead);
		clen += hdr_len; */
		skb_to_sgvec(skb, dsg, hdr_len + x->props.header_len, clen);

	}

	/* For ESP transport mode:

	   Input:
	   --------------------------------------------------------------------
	   |IP|UDP ENCAP|ESP|IV| Payload |                    IPSec Pad | ICV |
	   --------------------------------------------------------------------
	   ^
	   |
	   start of source SG 

	   -------------------------------------------------------------
	   | reserved |     IP | Payload | Pad | Pad length | Next hdr |
	   -------------------------------------------------------------
	              ^	       (Payload position not moved)
	              |
	              start of destination SG 

	   Output:
	   -------------------------------------------------------------
	   | reserved |     IP | Payload | Pad | Pad length | Next hdr |
	   -------------------------------------------------------------
	   IP header is moved right before payload field. Payload not moved.

	   For ESP tunnel mode:

	   Input:
	   --------------------------------------------------------------
	   |Out IP|UDP ENCAP|ESP | IV | In IP | Payload | IPSec Pad |ICV|
	   --------------------------------------------------------------
	   ESP - start of SG and length

	   Output:
	   ---------------------------------------------------------------------
	   | reserved                 | In IP | Payload |Pad |Pad len|Next Hder|
	   ---------------------------------------------------------------------
 	   reserved is removed header but inner IP and payload are left at
	   same position

	*/
	aead_request_set_callback(req, 0, esp_input_done_hw, skb);
	aead_request_set_crypt(req, sg, dsg, src_len, iv);

	err = crypto_aead_decrypt(req);
	if (err == -EINPROGRESS)
		goto out;

	err = esp_input_done2_hw(skb, err);

out:
	return err;
}

static int esp_input(struct xfrm_state *x, struct sk_buff *skb)
{
	if (x->alg_flags & XFRM_ALGO_FLAGS_OFFLOAD_ESP)
		return esp_input_hw(x, skb);
	else
		return esp_input_sw(x, skb);
}

static u32 esp4_get_mtu(struct xfrm_state *x, int mtu)
{
	struct esp_data *esp = x->data;
	u32 blksize = ALIGN(crypto_aead_blocksize(esp->aead), 4);
	u32 align = max_t(u32, blksize, esp->padlen);
	u32 rem;

	mtu -= x->props.header_len + crypto_aead_authsize(esp->aead);
	rem = mtu & (align - 1);
	mtu &= ~(align - 1);

	switch (x->props.mode) {
	case XFRM_MODE_TUNNEL:
		break;
	default:
	case XFRM_MODE_TRANSPORT:
		/* The worst case */
		mtu -= blksize - 4;
		mtu += min_t(u32, blksize - 4, rem);
		break;
	case XFRM_MODE_BEET:
		/* The worst case. */
		mtu += min_t(u32, IPV4_BEET_PHMAXLEN, rem);
		break;
	}

	return mtu - 2;
}

static void esp4_err(struct sk_buff *skb, u32 info)
{
	struct net *net = dev_net(skb->dev);
	struct iphdr *iph = (struct iphdr*)skb->data;
	struct ip_esp_hdr *esph = (struct ip_esp_hdr*)(skb->data+(iph->ihl<<2));
	struct xfrm_state *x;

	if (icmp_hdr(skb)->type != ICMP_DEST_UNREACH ||
	    icmp_hdr(skb)->code != ICMP_FRAG_NEEDED)
		return;

	x = xfrm_state_lookup(net, (xfrm_address_t *)&iph->daddr, esph->spi, IPPROTO_ESP,
AF_INET);	if (!x)
		return;
	NETDEBUG(KERN_DEBUG "pmtu discovery on SA ESP/%08x/%08x\n",
		 ntohl(esph->spi), ntohl(iph->daddr));
	xfrm_state_put(x);
}

static void esp_destroy(struct xfrm_state *x)
{
	struct esp_data *esp = x->data;

	if (!esp)
		return;

	crypto_free_aead(esp->aead);

#if CONFIG_INET_ESP_NR_REQ_CACHE > 0
	ESP_PRINTK("esp->req_cache_head = %d\n", esp->req_cache_head);
	ESP_PRINTK("req_cache_cnt = %d\n", atomic_read(&esp->req_cache_cnt));
	esp->req_cache_head = 0;
	/* Delete request cache */
	while ((atomic_dec_return(&esp->req_cache_cnt)) > 0) {
	if (esp->req_cache[esp->req_cache_head])
		kfree(esp->req_cache[esp->req_cache_head]);
	esp->req_cache_head = (esp->req_cache_head + 1) %
			ESP_REQ_CACHE_MAX;
	}
#endif
	
	kfree(esp);
}

static int esp_init_aead(struct xfrm_state *x)
{
	struct esp_data *esp = x->data;
	struct crypto_aead *aead;
	int err;
	char alg_name[CRYPTO_MAX_ALG_NAME];
	char *key;
	int  key_len;
	struct rtattr *rta;
	struct esp_param {
		__be32 spi;
		__be32 seq;
		__be16 pad_block_size;
		__be16 encap_uhl;
	} *param;

	switch (x->props.mode) {
	case XFRM_MODE_TUNNEL:
		snprintf(alg_name, ARRAY_SIZE(alg_name),
			"tunnel(esp(%s))", x->aead->alg_name);
		x->alg_flags |= XFRM_ALGO_FLAGS_OFFLOAD_TUNNEL
				| XFRM_ALGO_FLAGS_OFFLOAD_ESP;
		break;
	case XFRM_MODE_TRANSPORT:
		snprintf(alg_name, ARRAY_SIZE(alg_name),
			"transport(esp(%s))", x->aead->alg_name);
		x->alg_flags |= XFRM_ALGO_FLAGS_OFFLOAD_TRANPORT
				| XFRM_ALGO_FLAGS_OFFLOAD_ESP;
		break;
	default:
		strncpy(alg_name, x->aead->alg_name, ARRAY_SIZE(alg_name));
		break;
	}
	aead = crypto_alloc_aead(alg_name, 0, 0);
	if (IS_ERR(aead) && (x->alg_flags & XFRM_ALGO_FLAGS_OFFLOAD_ESP)) {
		x->alg_flags &= ~(XFRM_ALGO_FLAGS_OFFLOAD_TUNNEL
				  | XFRM_ALGO_FLAGS_OFFLOAD_TRANPORT
				  | XFRM_ALGO_FLAGS_OFFLOAD_ESP);
		aead = crypto_alloc_aead(x->aead->alg_name, 0, 0);
	}
	err = PTR_ERR(aead);
	if (IS_ERR(aead))
		goto error;

	esp->aead = aead;

	if (x->alg_flags & XFRM_ALGO_FLAGS_OFFLOAD_ESP) {
		/* For esp offload, we must load esp offload parameters
		   via setkey function. */
		key_len = RTA_SPACE(sizeof(*param)) +
        		  ((x->aead->alg_key_len + 7) / 8);
		key = kmalloc(key_len, 0);
		rta = (void *) key;
		rta->rta_type = CRYPTO_AUTHENC_KEYA_PARAM;
		rta->rta_len  = RTA_LENGTH(sizeof(*param));
		param = RTA_DATA(rta);
		param->spi = cpu_to_be32(x->id.spi);
		param->seq = cpu_to_be32(x->replay.oseq);
		if (x->encap) {
			int encap_type;

			spin_lock_bh(&x->lock);
			encap_type = x->encap->encap_type;
			spin_unlock_bh(&x->lock);

			switch (encap_type) {
			default:
			case UDP_ENCAP_ESPINUDP:
				param->encap_uhl = cpu_to_be16(sizeof(struct udphdr));
				break;
			case UDP_ENCAP_ESPINUDP_NON_IKE:
				param->encap_uhl = cpu_to_be16(sizeof(struct udphdr) +
							 sizeof(__be32)*2);
				break;
			}
		} else {
			param->encap_uhl = 0;
		}
		param->pad_block_size = cpu_to_be16(esp->padlen);
		memcpy(key + RTA_SPACE(sizeof(*param)),
		       x->aead->alg_key,
		       (x->aead->alg_key_len + 7) / 8);
	} else {
		key_len = (x->aead->alg_key_len + 7) / 8;
		key     = x->aead->alg_key;
	}

	err = crypto_aead_setkey(aead, key, key_len);
	if (key != x->aead->alg_key)
		kfree(key);
	if (err)
		goto error;

	err = crypto_aead_setauthsize(aead, x->aead->alg_icv_len / 8);
	if (err)
		goto error;

error:
	return err;
}

static int esp_init_authenc(struct xfrm_state *x)
{
	struct esp_data *esp = x->data;
	struct crypto_aead *aead;
	struct rtattr *rta;
	char *key;
	char *p;
	char authenc_name[CRYPTO_MAX_ALG_NAME];
	unsigned int keylen;
	int err;
	struct esp_authenc_param {
		__be32 spi;
		__be32 seq;
		__be16 pad_block_size;
		__be16 encap_uhl;
		struct crypto_authenc_key_param authenc_param;
							/* Must be last */
	} *esp_param = NULL;
	struct crypto_authenc_key_param *param = NULL;

	err = -EINVAL;
	if (x->ealg == NULL)
		goto error;

	err = -ENAMETOOLONG;

	switch (x->props.mode) {
	case XFRM_MODE_TUNNEL:
		if (snprintf(authenc_name, CRYPTO_MAX_ALG_NAME,
			"tunnel(esp(authenc(%s,%s)))",
			x->aalg ? x->aalg->alg_name : "digest_null",
		     	x->ealg->alg_name) >= CRYPTO_MAX_ALG_NAME)
			goto error;
		x->alg_flags |= XFRM_ALGO_FLAGS_OFFLOAD_TUNNEL
				| XFRM_ALGO_FLAGS_OFFLOAD_ESP;
		break;
	case XFRM_MODE_TRANSPORT:
		if (snprintf(authenc_name, CRYPTO_MAX_ALG_NAME,
			"transport(esp(authenc(%s,%s)))",
			x->aalg ? x->aalg->alg_name : "digest_null",
		     	x->ealg->alg_name) >= CRYPTO_MAX_ALG_NAME)
			goto error;
		x->alg_flags |= XFRM_ALGO_FLAGS_OFFLOAD_TRANPORT
				| XFRM_ALGO_FLAGS_OFFLOAD_ESP;
		break;
	default:
		if (snprintf(authenc_name, CRYPTO_MAX_ALG_NAME,
			"authenc(%s,%s)",
		     	x->aalg ? x->aalg->alg_name : "digest_null",
		     	x->ealg->alg_name) >= CRYPTO_MAX_ALG_NAME)
			goto error;
		break;
	}

	aead = crypto_alloc_aead(authenc_name, 0, 0);
	if (IS_ERR(aead) && (x->alg_flags & XFRM_ALGO_FLAGS_OFFLOAD_ESP)) {
		x->alg_flags &= ~(XFRM_ALGO_FLAGS_OFFLOAD_TUNNEL
				  | XFRM_ALGO_FLAGS_OFFLOAD_TRANPORT
				  | XFRM_ALGO_FLAGS_OFFLOAD_ESP);
		if (snprintf(authenc_name, CRYPTO_MAX_ALG_NAME,
			"authenc(%s,%s)",
		     	x->aalg ? x->aalg->alg_name : "digest_null",
		     	x->ealg->alg_name) >= CRYPTO_MAX_ALG_NAME)
			goto error;
		aead = crypto_alloc_aead(authenc_name, 0, 0);
	}
	err = PTR_ERR(aead);
	if (IS_ERR(aead))
		goto error;

	esp->aead = aead;

	keylen = (x->aalg ? (x->aalg->alg_key_len + 7) / 8 : 0) +
		 (x->ealg->alg_key_len + 7) / 8;
	if (x->alg_flags & XFRM_ALGO_FLAGS_OFFLOAD_ESP)
		keylen += RTA_SPACE(sizeof(*esp_param));
	else
		keylen += RTA_SPACE(sizeof(*param));
	err = -ENOMEM;
	key = kmalloc(keylen, GFP_KERNEL);
	if (!key)
		goto error;

	p = key;
	rta = (void *)p;
	rta->rta_type = CRYPTO_AUTHENC_KEYA_PARAM;
	if (x->alg_flags & XFRM_ALGO_FLAGS_OFFLOAD_ESP) {
		rta->rta_len = RTA_LENGTH(sizeof(*esp_param));
		esp_param = RTA_DATA(rta);
		p += RTA_SPACE(sizeof(*esp_param));
	} else {
		rta->rta_len = RTA_LENGTH(sizeof(*param));
		param = RTA_DATA(rta);
		p += RTA_SPACE(sizeof(*param));
	}

	if (x->aalg) {
		struct xfrm_algo_desc *aalg_desc;

		memcpy(p, x->aalg->alg_key, (x->aalg->alg_key_len + 7) / 8);
		p += (x->aalg->alg_key_len + 7) / 8;

		aalg_desc = xfrm_aalg_get_byname(x->aalg->alg_name, 0);
		BUG_ON(!aalg_desc);

		err = -EINVAL;
		if (aalg_desc->uinfo.auth.icv_fullbits/8 !=
		    crypto_aead_authsize(aead)) {
			NETDEBUG(KERN_INFO "ESP: %s digestsize %u != %hu\n",
				 x->aalg->alg_name,
				 crypto_aead_authsize(aead),
				 aalg_desc->uinfo.auth.icv_fullbits/8);
			goto free_key;
		}

		err = crypto_aead_setauthsize(
			aead, aalg_desc->uinfo.auth.icv_truncbits / 8);
		if (err)
			goto free_key;
	}

	if (x->alg_flags & XFRM_ALGO_FLAGS_OFFLOAD_ESP) {
		esp_param->authenc_param.enckeylen =
				cpu_to_be32((x->ealg->alg_key_len + 7) / 8);
		/* For esp offload, we must load esp offload parameters
		   via setkey function. */
		esp_param->spi = cpu_to_be32(x->id.spi);
		esp_param->seq = cpu_to_be32(x->replay.oseq);
		if (x->encap) {
			int encap_type;

			spin_lock_bh(&x->lock);
			encap_type = x->encap->encap_type;
			spin_unlock_bh(&x->lock);

			switch (encap_type) {
			default:
			case UDP_ENCAP_ESPINUDP:
				esp_param->encap_uhl =
					cpu_to_be16(sizeof(struct udphdr));
				break;
			case UDP_ENCAP_ESPINUDP_NON_IKE:
				esp_param->encap_uhl =
					cpu_to_be16(sizeof(struct udphdr) +
					sizeof(__be32)*2);
				break;
			}
		} else {
			esp_param->encap_uhl = 0;
		}
		esp_param->pad_block_size = cpu_to_be16(esp->padlen);
	} else {
		param->enckeylen = cpu_to_be32((x->ealg->alg_key_len + 7) / 8);
	}
	memcpy(p, x->ealg->alg_key, (x->ealg->alg_key_len + 7) / 8);

	err = crypto_aead_setkey(aead, key, keylen);

free_key:
	kfree(key);

error:
	return err;
}

static int esp_init_state(struct xfrm_state *x)
{
	struct esp_data *esp;
	struct crypto_aead *aead;
	u32 align;
	int err;

	esp = kzalloc(sizeof(*esp), GFP_KERNEL);
	if (esp == NULL)
		return -ENOMEM;

	x->data = esp;

	if (x->aead)
		err = esp_init_aead(x);
	else
		err = esp_init_authenc(x);

	if (err)
		goto error;

	aead = esp->aead;
#if CONFIG_INET_ESP_NR_REQ_CACHE > 0
	atomic_set(&esp->req_cache_cnt, 0);
	esp->req_cache_head = 0;
	esp->req_cache_tail = 0;
#endif	
	esp->req_cache_size = esp_req_ctx_size(aead, 0);
	esp->padlen = 0;

	x->props.header_len = sizeof(struct ip_esp_hdr) +
			      crypto_aead_ivsize(aead);
	if (x->props.mode == XFRM_MODE_TUNNEL)
		x->props.header_len += sizeof(struct iphdr);
	else if (x->props.mode == XFRM_MODE_BEET && x->sel.family != AF_INET6)
		x->props.header_len += IPV4_BEET_PHMAXLEN;
	if (x->encap) {
		struct xfrm_encap_tmpl *encap = x->encap;

		switch (encap->encap_type) {
		default:
			goto error;
		case UDP_ENCAP_ESPINUDP:
			x->props.header_len += sizeof(struct udphdr);
			break;
		case UDP_ENCAP_ESPINUDP_NON_IKE:
			x->props.header_len += sizeof(struct udphdr) + 2 * sizeof(u32);
			break;
		}
	}

	align = ALIGN(crypto_aead_blocksize(aead), 4);
	if (esp->padlen)
		align = max_t(u32, align, esp->padlen);
	x->props.trailer_len = align + 1 + crypto_aead_authsize(esp->aead);

error:
	return err;
}

static const struct xfrm_type esp_type =
{
	.description	= "ESP4",
	.owner		= THIS_MODULE,
	.proto	     	= IPPROTO_ESP,
	.flags		= XFRM_TYPE_REPLAY_PROT,
	.init_state	= esp_init_state,
	.destructor	= esp_destroy,
	.get_mtu	= esp4_get_mtu,
	.input		= esp_input,
	.output		= esp_output
};

static const struct net_protocol esp4_protocol = {
	.handler	=	xfrm4_rcv,
	.err_handler	=	esp4_err,
	.no_policy	=	1,
	.netns_ok	=	1,
};

static int __init esp4_init(void)
{
	if (xfrm_register_type(&esp_type, AF_INET) < 0) {
		printk(KERN_INFO "ip esp init: can't add xfrm type\n");
		return -EAGAIN;
	}
	if (inet_add_protocol(&esp4_protocol, IPPROTO_ESP) < 0) {
		printk(KERN_INFO "ip esp init: can't add protocol\n");
		xfrm_unregister_type(&esp_type, AF_INET);
		return -EAGAIN;
	}
	return 0;
}

static void __exit esp4_fini(void)
{
	if (inet_del_protocol(&esp4_protocol, IPPROTO_ESP) < 0)
		printk(KERN_INFO "ip esp close: can't remove protocol\n");
	if (xfrm_unregister_type(&esp_type, AF_INET) < 0)
		printk(KERN_INFO "ip esp close: can't remove xfrm type\n");
}

module_init(esp4_init);
module_exit(esp4_fini);
MODULE_LICENSE("GPL");
MODULE_ALIAS_XFRM_TYPE(AF_INET, XFRM_PROTO_ESP);
