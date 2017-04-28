

extra-y := $(obj)/start.o $(obj)/main.o $(obj)/nand_flash.o $(obj)/printf.o  $(obj)/bch.o $(obj)/malloc_compat.o $(obj)/sha1.o

extra-$(CONFIG_SOC_QCA955X) += $(obj)/lowlevel_init_955x.o $(obj)/955x.o
extra-$(CONFIG_SOC_AR934X) += $(obj)/lowlevel_init_934x.o $(obj)/lowlevel_init_934x-1.1.o $(obj)/934x.o

$(obj)/nandloader.elf: $(extra-y)
	$(LD) $(LDFLAGS-LOADER) -o $@ $^


EXTRA_CFLAGS += -G 0 --include $(srctree)/$(src)/malloc_compat.h

LDFLAGS-LOADER = -T $(srctree)/$(src)/nandloader.lds

$(obj)/nandloader-tmp.bin: $(obj)/nandloader.elf
	$(OBJCOPY) -O binary $< $@

checksum: $(obj)/nandloader-tmp.bin
	dd bs=4 skip=4 if=$(obj)/nandloader-tmp.bin | perl $(srctree)/$(src)/checksum.pl | dd bs=4 seek=2 count=2 of=$(obj)/nandloader-tmp.bin conv=notrunc

$(obj)/nandloader.bin: checksum
	mv $(obj)/nandloader-tmp.bin $(obj)/nandloader.bin

nandloader.bin: $(obj)/nandloader.bin

clean:
	rm -f nandloader.elf nandloader.bin $(extra-y)


