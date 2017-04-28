/*
 * Parse RedBoot-style Flash Image System (FIS) tables and
 * produce a Linux partition array to match.
 *
 * Copyright © 2001      Red Hat UK Limited
 * Copyright © 2001-2010 David Woodhouse <dwmw2@infradead.org>
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
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA  02110-1301  USA
 *
 */

#include <linux/kernel.h>
#include <linux/slab.h>
#include <linux/init.h>
#include <linux/vmalloc.h>

#include <linux/mtd/mtd.h>
#include <linux/mtd/partitions.h>

#define REDBOOT_FIS_VERSION      (1)

// These will need to move somewhere else when the fisfs needs to use
// the following definitions.

#define REDBOOT_RFIS_VALID_MAGIC_LENGTH 10
#define REDBOOT_RFIS_VALID_MAGIC ".FisValid"  //exactly 10 bytes

#define REDBOOT_RFIS_VALID       (0xa5)
#define REDBOOT_RFIS_IN_PROGRESS (0xfd)
#define REDBOOT_RFIS_EMPTY       (0xff)

struct fis_valid_info
{
     char magic_name[REDBOOT_RFIS_VALID_MAGIC_LENGTH];
     unsigned char valid_flag[2]; //this should be safe for all alignment issues
     uint32_t	version_count;
};

struct fis_image_desc {
    union {
      unsigned char name[16];      // Null terminated name
      struct fis_valid_info valid_info;
    } u;
    uint32_t	  flash_base;    // Address within FLASH of image
    uint32_t	  mem_base;      // Address in memory where it executes
    uint32_t	  size;          // Length of image
    uint32_t	  entry_point;   // Execution entry point
    uint32_t	  data_length;   // Length of actual data
    unsigned char _pad[256-(16+7*sizeof(uint32_t))];
    uint32_t	  desc_cksum;    // Checksum over image descriptor
    uint32_t	  file_cksum;    // Checksum over image data
};

struct fis_list {
	struct fis_image_desc *img;
	struct fis_list *next;
};

static int primary_directory = CONFIG_MTD_REDBOOT_DIRECTORY_BLOCK;
module_param(primary_directory, int, 0);

#ifdef CONFIG_MTD_REDBOOT_REDUNDANT_FIS
static int redundant_directory = CONFIG_MTD_REDBOOT_REDUNDANT_BLOCK;
module_param(redundant_directory, int, 0);

#endif
static inline int redboot_checksum(struct fis_image_desc *img)
{
	/* RedBoot doesn't actually write the desc_cksum field yet AFAICT */
	return 1;
}

static int read_redboot_fis(struct mtd_info *master, int directory,
			    const char *which, struct fis_image_desc *buf)
{
	unsigned long offset;
	size_t retlen;
	int ret;

	if ( directory < 0 ) {
		offset = master->size + directory * master->erasesize;
		while (master->block_isbad && 
		       master->block_isbad(master, offset)) {
			if (!offset) {
			nogood:
			  printk(KERN_NOTICE "Failed to find a non-bad block to check for RedBoot %s partition table\n", which);
				return -EIO;
			}
			offset -= master->erasesize;
		}
	} else {
		offset = directory * master->erasesize;
		while (master->block_isbad && 
		       master->block_isbad(master, offset)) {
			offset += master->erasesize;
			if (offset == master->size)
				goto nogood;
		}
	}

	printk(KERN_NOTICE "Searching for RedBoot %s partition table in %s at offset 0x%lx\n",
	       which, master->name, offset);

	ret = master->read(master, offset,
			   master->erasesize, &retlen, (void *)buf);

	if (ret)
		return ret;

	if (retlen != master->erasesize) {
		return -EIO;
	}

	return 0;
}

static int parse_redboot_fis(struct mtd_info *master,
			     struct fis_image_desc *buf,
			     const char *which,
			     int *valid_magic_name, int *valid_magic_flag,
			     uint32_t *version_count)
{
	int i;
	int numslots;

	numslots = (master->erasesize / sizeof(struct fis_image_desc));

#ifdef CONFIG_MTD_REDBOOT_REDUNDANT_FIS
	/* If we are configured for redundant FIS, then check the
	 * validity magic entry first. This must be first in the
	 * directory.
	 */

#endif

	*valid_magic_name = -1;
	*valid_magic_flag = -1;
	*version_count = 0;

	for (i = 0; i < numslots; i++) {

		/*
		 * Check for the magic file .FisValid and the magic flags
		 */

		if (memcmp(buf[i].u.valid_info.magic_name,
			   REDBOOT_RFIS_VALID_MAGIC,
			   REDBOOT_RFIS_VALID_MAGIC_LENGTH) == 0) {
			if (*valid_magic_name != -1) {
				printk(KERN_WARNING "RedBoot .FisValid found "
				       "in %s slots %d and %d",
				       which, *valid_magic_name, i);
				continue;
			}
			*valid_magic_name = i;
			if ((buf[i].u.valid_info.valid_flag[0] ==
			     REDBOOT_RFIS_VALID) &&
			    (buf[i].u.valid_info.valid_flag[1] ==
			     REDBOOT_RFIS_VALID))
				*valid_magic_flag = true;
			*version_count = buf[i].u.valid_info.version_count;
			continue;
		}

		if (!memcmp(buf[i].u.name, "FIS directory", 14)) {
			/* This is apparently the FIS directory entry for the
			 * FIS directory itself.  The FIS directory size is
			 * one erase block; if the buf[i].size field is
			 * swab32(erasesize) then we know we are looking at
			 * a byte swapped FIS directory - swap all the entries!
			 * (NOTE: this is 'size' not 'data_length'; size is
			 * the full size of the entry.)
			 */

			/* RedBoot can combine the FIS directory and
			   config partitions into a single eraseblock;
			   we assume wrong-endian if either the swapped
			   'size' matches the eraseblock size precisely,
			   or if the swapped size actually fits in an
			   eraseblock while the unswapped size doesn't. */
			if (swab32(buf[i].size) == master->erasesize ||
			    (buf[i].size > master->erasesize
			     && swab32(buf[i].size) < master->erasesize)) {
				int j;
				/* Update numslots based on actual FIS directory size */
				numslots = swab32(buf[i].size) / sizeof (struct fis_image_desc);
				for (j = 0; j < numslots; ++j) {

					/* A single 0xff denotes a deleted entry.
					 * Two of them in a row is the end of the table.
					 */
					if (buf[j].u.name[0] == 0xff) {
						if (buf[j].u.name[1] == 0xff) {
							break;
						} else {
							continue;
						}
					}

					/* The unsigned long fields were written with the
					 * wrong byte sex, name and pad have no byte sex.
					 */
					swab32s(&buf[j].flash_base);
					swab32s(&buf[j].mem_base);
					swab32s(&buf[j].size);
					swab32s(&buf[j].entry_point);
					swab32s(&buf[j].data_length);
					swab32s(&buf[j].desc_cksum);
					swab32s(&buf[j].file_cksum);
				}
				swab32s(version_count);
			} else if (buf[i].size < master->erasesize) {
				/* Update numslots based on actual FIS directory size */
				numslots = buf[i].size / sizeof(struct fis_image_desc);
			}
			break;
		}
	}
	if (i == numslots) {
		/* Didn't find it */
		printk(KERN_NOTICE "No RedBoot %s partition table detected in %s\n",
		       which, master->name);
		return 0;
	}

	return numslots;

}

static int parse_redboot_partitions(struct mtd_info *master,
                             struct mtd_partition **pparts,
                             unsigned long fis_origin)
{
	int nrparts = 0;
	struct mtd_partition *parts;
	struct fis_image_desc *buf;
	int primary_numslots = 0;
	int primary_magic_found, primary_magic_valid, primary_version;
#ifdef CONFIG_MTD_REDBOOT_REDUNDANT_FIS
	struct fis_image_desc *redundant_buf;
	int redundant_numslots = 0;
	int redundant_magic_found, redundant_magic_valid, redundant_version;
	int use_redundant = false;
	int update_interrupted = false;
#endif
	struct fis_list *fl = NULL, *tmp_fl;
	int ret, i;
	char *names;
	char *nullname;
	int namelen = 0;
	int nulllen = 0;
	int numslots;
#ifdef CONFIG_MTD_REDBOOT_PARTS_UNALLOCATED
	static char nullstring[] = "unallocated";
#endif

	buf = vmalloc(master->erasesize);
	if (!buf)
		return -ENOMEM;
#ifdef CONFIG_MTD_REDBOOT_REDUNDANT_FIS
	redundant_buf = vmalloc(master->erasesize);
	if (!redundant_buf) {
		vfree(buf);
		return -ENOMEM;
	}
#endif /* CONFIG_MTD_REDBOOT_REDUNDANT_FIS */

	ret = read_redboot_fis(master, primary_directory, "primary", buf);
	if (!ret)
		primary_numslots = parse_redboot_fis(master, buf, "primary",
						     &primary_magic_found,
						     &primary_magic_valid,
						     &primary_version);

#ifdef CONFIG_MTD_REDBOOT_REDUNDANT_FIS
	ret = read_redboot_fis(master, redundant_directory, "redundant",
			       redundant_buf);
	if (!ret)
		redundant_numslots = parse_redboot_fis(master, redundant_buf,
						       "redundant",
						       &redundant_magic_found,
						       &redundant_magic_valid,
						       &redundant_version);

	if ((primary_numslots == 0) && (redundant_numslots != 0)) {
		printk(KERN_INFO "RedBoot primary FIS invalid, using redundant\n");
		use_redundant = true;
	} else if (redundant_magic_found < 0) {
		if (primary_numslots == 0) {
			ret = -ENOENT;
			goto out;
		}
		printk(KERN_INFO "RedBoot redundant FIS invalid, using primary\n");
		if (primary_version > 0)
			update_interrupted = true;
	} else if (primary_magic_found < 0) {
		if (redundant_numslots == 0) {
			ret = -ENOENT;
			goto out;
		}
		printk(KERN_INFO "RedBoot primary FIS magic not found, using redundant\n");
		use_redundant = true;
		if (redundant_version > 0)
			update_interrupted = true;
	} else if (redundant_magic_found < 0) {
		if (primary_numslots == 0) {
			ret = -ENOENT;
			goto out;
		}
		printk(KERN_INFO "RedBoot redundant FIS magic not found, using parimary\n");
		update_interrupted = true;
	} else if (redundant_version == (primary_version + 1)) {
		use_redundant = true;
	}

	if (update_interrupted)
		printk(KERN_WARNING "RedBoot FLASH update was interrupted\n");

	if (use_redundant) {
		vfree(buf);
		buf = redundant_buf;
		numslots = redundant_numslots;
		redundant_buf = NULL;
	} else {
		vfree(redundant_buf);
		redundant_buf = NULL;
		numslots = primary_numslots;
	}
#else /* CONFIG_MTD_REDUNDANT_FIS */
	if (!primary_numslots) {
		if (!ret)
			ret = -ENOENT;
		goto out;
	}
	numslots = primary_numslots;
#endif

	for (i = 0; i < numslots; i++) {
		struct fis_list *new_fl, **prev;

		if (memcmp(buf[i].u.valid_info.magic_name,
			   REDBOOT_RFIS_VALID_MAGIC,
			   REDBOOT_RFIS_VALID_MAGIC_LENGTH) == 0)
			continue;

		if (buf[i].u.name[0] == 0xff) {
			if (buf[i].u.name[1] == 0xff) {
				break;
			} else {
				continue;
			}
		}
		if (!redboot_checksum(&buf[i]))
			break;

		new_fl = kmalloc(sizeof(struct fis_list), GFP_KERNEL);
		namelen += strlen(buf[i].u.name)+1;
		if (!new_fl) {
			ret = -ENOMEM;
			goto out;
		}
		new_fl->img = &buf[i];
                if (fis_origin) {
                        buf[i].flash_base -= fis_origin;
                } else {
                        buf[i].flash_base &= master->size-1;
                }

		/* I'm sure the JFFS2 code has done me permanent damage.
		 * I now think the following is _normal_
		 */
		prev = &fl;
		while(*prev && (*prev)->img->flash_base < new_fl->img->flash_base)
			prev = &(*prev)->next;
		new_fl->next = *prev;
		*prev = new_fl;

		nrparts++;
	}
#ifdef CONFIG_MTD_REDBOOT_PARTS_UNALLOCATED
	if (fl->img->flash_base) {
		nrparts++;
		nulllen = sizeof(nullstring);
	}

	for (tmp_fl = fl; tmp_fl->next; tmp_fl = tmp_fl->next) {
		if (tmp_fl->img->flash_base + tmp_fl->img->size + master->erasesize <= tmp_fl->next->img->flash_base) {
			nrparts++;
			nulllen = sizeof(nullstring);
		}
	}
#endif
	parts = kzalloc(sizeof(*parts)*nrparts + nulllen + namelen, GFP_KERNEL);

	if (!parts) {
		ret = -ENOMEM;
		goto out;
	}

	nullname = (char *)&parts[nrparts];
#ifdef CONFIG_MTD_REDBOOT_PARTS_UNALLOCATED
	if (nulllen > 0) {
		strcpy(nullname, nullstring);
	}
#endif
	names = nullname + nulllen;

	i=0;

#ifdef CONFIG_MTD_REDBOOT_PARTS_UNALLOCATED
	if (fl->img->flash_base) {
	       parts[0].name = nullname;
	       parts[0].size = fl->img->flash_base;
	       parts[0].offset = 0;
		i++;
	}
#endif
	for ( ; i<nrparts; i++) {
		parts[i].size = fl->img->size;
		parts[i].offset = fl->img->flash_base;
		parts[i].name = names;

		strcpy(names, fl->img->u.name);
#ifdef CONFIG_MTD_REDBOOT_PARTS_READONLY
		if (!memcmp(names, "RedBoot", 8) ||
				!memcmp(names, "RedBoot config", 15) ||
				!memcmp(names, "FIS directory", 14)) {
			parts[i].mask_flags = MTD_WRITEABLE;
		}
#endif
		names += strlen(names)+1;

#ifdef CONFIG_MTD_REDBOOT_PARTS_UNALLOCATED
		if(fl->next && fl->img->flash_base + fl->img->size + master->erasesize <= fl->next->img->flash_base) {
			i++;
			parts[i].offset = parts[i-1].size + parts[i-1].offset;
			parts[i].size = fl->next->img->flash_base - parts[i].offset;
			parts[i].name = nullname;
		}
#endif
		tmp_fl = fl;
		fl = fl->next;
		kfree(tmp_fl);
	}
	ret = nrparts;
	*pparts = parts;
 out:
	while (fl) {
		struct fis_list *old = fl;
		fl = fl->next;
		kfree(old);
	}
	vfree(buf);
#ifdef CONFIG_MTD_REDBOOT_REDUNDANT_FIS
	if (redundant_buf)
		vfree(redundant_buf);
#endif /* CONFIG_MTD_REDBOOT_REDUNDANT_FIS */
	return ret;
}

static struct mtd_part_parser redboot_parser = {
	.owner = THIS_MODULE,
	.parse_fn = parse_redboot_partitions,
	.name = "RedBoot",
};

static int __init redboot_parser_init(void)
{
	return register_mtd_parser(&redboot_parser);
}

static void __exit redboot_parser_exit(void)
{
	deregister_mtd_parser(&redboot_parser);
}

module_init(redboot_parser_init);
module_exit(redboot_parser_exit);

MODULE_LICENSE("GPL");
MODULE_AUTHOR("David Woodhouse <dwmw2@infradead.org>");
MODULE_DESCRIPTION("Parsing code for RedBoot Flash Image System (FIS) tables");
