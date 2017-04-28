/**
 * AppliedMicro APM86xxx SoC Security Driver
 *
 * Copyright (c) 2010 Applied Micro Circuits Corporation.
 * All rights reserved. Loc Ho <lho@apm.com>
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * @file apm_sec_alg.h
 *
 * This file defines header for the algorithms to use with Linux CryptoAPI.
 *
 */
#ifndef __APM_SEC_ALG_H__
#define __APM_SEC_ALG_H__

#include <linux/crypto.h>
#include <crypto/algapi.h>

#define APM_SEC_CRYPTO_PRIORITY		300
#define APM_SEC_CRYPTO_PRIORITY_IPSEC	350

#define crypto_alg_to_apm_sec_alg(x)  	\
		container_of(x, struct apm_sec_alg, alg)

/**
 * @struct apm_sec_req_ctx
 * @param tkn	Token assigned to this request
 * @param sa	SA assigned to this request
 *
 */
struct apm_sec_req_ctx {
	struct sec_tkn_ctx *tkn;
	struct sec_sa_item *sa;
};

struct apm_sec_export {
    struct shash_desc shash;
    char ctx[256];
};

struct apm_sec_alg {
	struct list_head  entry;
        u32 type;
        union {
                struct crypto_alg cipher;
                struct ahash_alg hash;
        } u;
};

extern struct apm_sec_alg apm_sec_alg_tlb[];

#endif
