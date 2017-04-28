#include <asm/setup.h>
#include <libfdt.h>
#ifdef CONFIG_ARCH_AMBARELLA
#include <plat/atag.h>
#endif

static int node_offset(void *fdt, const char *node_path)
{
	int offset = fdt_path_offset(fdt, node_path);
	if (offset == -FDT_ERR_NOTFOUND)
		offset = fdt_add_subnode(fdt, 0, node_path);
	return offset;
}

static int setprop(void *fdt, const char *node_path, const char *property,
		   uint32_t *val_array, int size)
{
	int offset = node_offset(fdt, node_path);
	if (offset < 0)
		return offset;
	return fdt_setprop(fdt, offset, property, val_array, size);
}

static int setprop_string(void *fdt, const char *node_path,
			  const char *property, const char *string)
{
	int offset = node_offset(fdt, node_path);
	if (offset < 0)
		return offset;
	return fdt_setprop_string(fdt, offset, property, string);
}

static int setprop_cell(void *fdt, const char *node_path,
			const char *property, uint32_t val)
{
	int offset = node_offset(fdt, node_path);
	if (offset < 0)
		return offset;
	return fdt_setprop_cell(fdt, offset, property, val);
}

/*
 * Convert and fold provided ATAGs into the provided FDT.
 *
 * REturn values:
 *    = 0 -> pretend success
 *    = 1 -> bad ATAG (may retry with another possible ATAG pointer)
 *    < 0 -> error from libfdt
 */
int atags_to_fdt(void *atag_list, void *fdt, int total_space)
{
	struct tag *atag = atag_list;
	uint32_t mem_reg_property[2 * NR_BANKS];
	int memcount = 0;
	int ret;

	/* make sure we've got an aligned pointer */
	if ((u32)atag_list & 0x3)
		return 1;

	/* if we get a DTB here we're done already */
	if (*(u32 *)atag_list == fdt32_to_cpu(FDT_MAGIC))
	       return 0;

	/* validate the ATAG */
	if (atag->hdr.tag != ATAG_CORE ||
	    (atag->hdr.size != tag_size(tag_core) &&
	     atag->hdr.size != 2))
		return 1;

	/* let's give it all the room it could need */
	ret = fdt_open_into(fdt, fdt, total_space);
	if (ret < 0)
		return ret;

	for_each_tag(atag, atag_list) {
		if (atag->hdr.tag == ATAG_CMDLINE) {
			setprop_string(fdt, "/chosen", "bootargs",
					atag->u.cmdline.cmdline);
		} else if (atag->hdr.tag == ATAG_MEM) {
			if (memcount >= sizeof(mem_reg_property)/4)
				continue;
			if (!atag->u.mem.size)
				continue;
			mem_reg_property[memcount++] = cpu_to_fdt32(atag->u.mem.start);
			mem_reg_property[memcount++] = cpu_to_fdt32(atag->u.mem.size);
		} else if (atag->hdr.tag == ATAG_INITRD2) {
			uint32_t initrd_start, initrd_size;
			initrd_start = atag->u.initrd.start;
			initrd_size = atag->u.initrd.size;
			setprop_cell(fdt, "/chosen", "linux,initrd-start",
					initrd_start);
			setprop_cell(fdt, "/chosen", "linux,initrd-end",
					initrd_start + initrd_size);
#ifdef CONFIG_ARCH_AMBARELLA
		} else if (atag->hdr.tag == ATAG_AMBARELLA_BST) {
			uint32_t bst[2];
			bst[0] = cpu_to_fdt32(atag->u.mem.start);
			bst[1] = cpu_to_fdt32(atag->u.mem.size);
			setprop(fdt, "/ambarella", "bst-reg", bst, 8);
		} else if (atag->hdr.tag == ATAG_AMBARELLA_BSB) {
			uint32_t bsb[2];
			bsb[0] = cpu_to_fdt32(atag->u.mem.start);
			bsb[1] = cpu_to_fdt32(atag->u.mem.size);
			setprop(fdt, "/ambarella", "bsb-reg", bsb, 8);
		} else if (atag->hdr.tag == ATAG_AMBARELLA_DSP) {
			uint32_t dsp[2];
			dsp[0] = cpu_to_fdt32(atag->u.mem.start);
			dsp[1] = cpu_to_fdt32(atag->u.mem.size);
			setprop(fdt, "/ambarella", "dsp-reg", dsp, 8);
		} else if (atag->hdr.tag == ATAG_AMBARELLA_HAL) {
			uint32_t hal[3];
			hal[0] = cpu_to_fdt32(atag->u.ramdisk.start);
			hal[1] = cpu_to_fdt32(atag->u.ramdisk.size);
			hal[2] = cpu_to_fdt32(atag->u.ramdisk.flags);
			setprop(fdt, "/ambarella", "hal-reg", hal, 12);
		} else if (atag->hdr.tag == ATAG_REVISION) {
			uint32_t rev;
			rev = cpu_to_fdt32(atag->u.revision.rev);
			setprop(fdt, "/ambarella", "board-id", &rev, 4);
#endif
		}
	}

	if (memcount)
		setprop(fdt, "/memory", "reg", mem_reg_property, 4*memcount);

	return fdt_pack(fdt);
}
