/*
 *  Meraki machine autodetector and common support
 *
 *  Author: Kevin Paul Herbert <kph@meraki.net>
 *  Copyright (c) 2013 Meraki, Inc.
 *
 *  This program is free software; you can redistribute it and/or modify it
 *  under the terms of the GNU General Public License version 2 as published
 *  by the Free Software Foundation.
 */

#include <linux/init.h>
#include <linux/pci.h>
#include <linux/mtd/mtd.h>
#include <linux/mtd/partitions.h>
#include <linux/crypto.h>
#include <crypto/hash.h>
#include <crypto/sha.h>

#include "machtypes.h"
#include "pci.h"

struct meraki_magic {
	unsigned long magic;
	unsigned long header_length;
	long kernel_length;
	unsigned char kernel_sha1[SHA1_DIGEST_SIZE];
	unsigned long magic2;
	unsigned long squashfs_length;
	unsigned char squashfs_sha1[SHA1_DIGEST_SIZE];
};

#define MERAKI_MAGIC    0x8e73ed8a

static bool __init calc_mtd_sha1(struct mtd_info *mtd, char *parent,
				 char *section,
				 unsigned long skip,
				 unsigned long size, char *sha1_calc,
				 char *sha1_expected,
				 struct shash_desc *desc)
{
	int ret, retlen;
	unsigned long offset;
	int blocksize = crypto_shash_blocksize(desc->tfm);
	unsigned char databuf[blocksize];

	pr_info("%s: checking %s in %s\n", __func__, section, parent);
	crypto_shash_init(desc);

	for (offset = 0; offset < size; offset += blocksize) {
		if (offset + blocksize > size)
			blocksize = size - offset;
		ret = mtd_read(mtd, skip + offset, blocksize, &retlen, databuf);
		if ((ret < 0) || (retlen != blocksize)) {
			pr_err("%s: Unable to read part data from %s\n",
			       __func__, mtd->name);
			return false;
		}
		crypto_shash_update(desc, databuf, blocksize);
	}
	crypto_shash_final(desc, sha1_calc);

	if (memcmp(sha1_calc, sha1_expected, SHA1_DIGEST_SIZE)) {
		pr_err("%s: %s sha1sum mismatch in %s\n", __func__,
		       section, parent);
		return false;
	}
	return true;
}

static struct mtd_info __init *check_meraki_partition(char *parent,
						      unsigned long *offset,
						      char *kernel_sha1,
						      char *squashfs_sha1,
						      struct shash_desc *desc,
						      bool *ruined)
{
	struct mtd_info *mtd;
	struct meraki_magic header;
	int ret, retlen;
	unsigned long kernel_length;

	mtd = get_mtd_device_nm(parent);
	if (!mtd) {
		pr_err("%s: Unable to find %s\n", __func__, parent);
		goto out_err;
	}

	pr_info("%s: checking %s, this may take a while\n", __func__, parent);

	ret = mtd_read(mtd, 0, sizeof(header), &retlen, (u_char *)&header);
	if ((ret < 0) || (retlen != sizeof(header))) {
		pr_err("%s: Unable to read header from %s\n", __func__, parent);
		goto out_mtd;
	}

	if (header.magic != MERAKI_MAGIC) {
		pr_err("%s: Header magic is %lx in %s\n", __func__,
		       header.magic, parent);
		goto out_mtd;
	}

	if (header.magic2 != MERAKI_MAGIC) {
		pr_err("%s: Header magic2 is %lx in %s\n", __func__,
		       header.magic2, parent);
		goto out_mtd;
	}

	if (header.kernel_length < 0) {
		kernel_length = -header.kernel_length - 1;
		*ruined = 1;
	} else {
		*ruined = 0;
		kernel_length = header.kernel_length;
	}

	if (!kernel_length) {
		pr_err("%s: Data length is zero in %s\n", __func__, parent);
		goto out_mtd;
	}

	if (!calc_mtd_sha1(mtd, parent, "kernel", header.header_length,
			   header.kernel_length, kernel_sha1,
			   header.kernel_sha1, desc))
		goto out_mtd;

	*offset = header.header_length + header.kernel_length;

	if (!calc_mtd_sha1(mtd, parent, "squashfs", *offset,
			   header.squashfs_length,
			   squashfs_sha1, header.squashfs_sha1, desc))
		goto out_mtd;

	return mtd;

out_mtd:
	put_mtd_device(mtd);
out_err:

	return NULL;
}

static void __init add_root_partition(char *root, char *parent,
				      struct mtd_info *mtd,
				      unsigned long offset)
{
	int ret;

	ret = mtd_add_partition(mtd, root, offset, MTDPART_SIZ_FULL);
	if (ret < 0) {
		pr_err("%s: unable to add %s to %s, err = %d\n",
		       __func__, root, parent, ret);
	} else {
		mtd->flags &= ~MTD_WRITEABLE; /* Write protect parent */
	}
}

void __init meraki_late_init(void)
{
	char *part_names[2] = { "part1", "part2" };
	unsigned char kernel_sha1[2][SHA1_DIGEST_SIZE];
	unsigned char squashfs_sha1[2][SHA1_DIGEST_SIZE];
	unsigned long offset[2];
	struct mtd_info *mtd[2];
	bool ruined[2];
	int bootpart = -1;
	struct crypto_shash *shash;
	struct shash_desc *desc = NULL;
	int part;

	shash = crypto_alloc_shash("sha1", 0, 0);
	if (IS_ERR(shash)) {
		pr_err("%s: unable to allocate shash, err = %ld\n",
		       __func__, PTR_ERR(shash));
		return;
	}

	desc = kzalloc(sizeof(*desc) + crypto_shash_descsize(shash),
		       GFP_KERNEL);

	if (!desc) {
		pr_err("%s: Unable to allocate desc\n", __func__);
		return;
	}

	desc->tfm = shash;
	desc->flags = CRYPTO_TFM_REQ_MAY_SLEEP;

	for (part = 0; part <= 1; part++) {

		mtd[part] = check_meraki_partition(part_names[part],
						   &offset[part],
						   kernel_sha1[part],
						   squashfs_sha1[part],
						   desc, &ruined[part]);

		pr_info("%s: root %s is %svalid and %sruined\n", __func__,
			part_names[part], mtd[part] ? "" : "in",
			ruined[part] ? "" : "not ");
	}
	if (mtd[0] && !ruined[0]) {
		/*
		 * Part1 is valid. Use it, unless part2 contains the
		 * same data, in which case, use that.
		 */
		bootpart = 0;
		if (mtd[1]) {
			if ((strncmp(kernel_sha1[0], kernel_sha1[1],
				     SHA1_DIGEST_SIZE) == 0) &&
			    (strncmp(squashfs_sha1[0], squashfs_sha1[1],
				     SHA1_DIGEST_SIZE) == 0)) {
				bootpart = 1;
				pr_info("%s: "
					"Picked %s as both are indentical\n",
					__func__,
					part_names[bootpart]);
			}
		} else {
			pr_info("%s: picked %s because its the only option\n",
				__func__, part_names[bootpart]);
		}
	} else {
		/* If we have only mtd1, use that */
		if (mtd[1]) {
			bootpart = 1;
			pr_info("%s: picked %s because its the only option\n",
				__func__, part_names[bootpart]);
		}
	}

	if ((bootpart < 0) && mtd[0]) {
		bootpart = 0;	/* If only MTD0 exists, override ruined */
		pr_info("%s: picked %s because its the only option\n",
			__func__, part_names[bootpart]);
	}

	if (bootpart < 0) {
		pr_err("%s: unable to find a good root\n", __func__);
	} else {
		pr_info("%s: selected root is %s\n", __func__,
			part_names[bootpart]);
		add_root_partition("root", part_names[bootpart], mtd[bootpart],
			offset[bootpart]);
	}

	if (mtd[0])
		put_mtd_device(mtd[0]);
	if (mtd[1])
		put_mtd_device(mtd[1]);

	kfree(desc);
	crypto_free_shash(shash);
}

static void __init meraki_init(void)
{
	ath79_register_pci();	/* Register PCI - rest will be sorted later */
}

MIPS_MACHINE(ATH79_MACH_MERAKI, "Meraki",
	     "Meraki Access Point", meraki_init);
