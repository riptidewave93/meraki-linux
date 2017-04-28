


extra-y := start.o main.o nand_flash.o printf.o lowlevel_init_934x.o lowlevel_init_934x-1.1.o bch.o malloc_compat.o

EXTRA_CFLAGS += -G 0 --include $(srctree)/$(src)/malloc_compat.h

LDFLAGS-LOADER = -T $(srctree)/$(src)/nandloader.lds

$(obj)/nandloader.elf: $(obj)/start.o $(obj)/main.o $(obj)/nand_flash.o $(obj)/printf.o $(obj)/lowlevel_init_934x.o $(obj)/lowlevel_init_934x-1.1.o $(obj)/bch.o $(obj)/malloc_compat.o
	$(LD) $(LDFLAGS-LOADER) -o $@ $^

$(obj)/nandloader-tmp.bin: $(obj)/nandloader.elf
	$(OBJCOPY) -O binary $< $@

checksum: $(obj)/nandloader-tmp.bin
	dd bs=4 skip=4 if=$(obj)/nandloader-tmp.bin | perl $(srctree)/$(src)/checksum.pl | dd bs=4 seek=2 count=2 of=$(obj)/nandloader-tmp.bin conv=notrunc

$(obj)/nandloader.bin: checksum
	mv $(obj)/nandloader-tmp.bin $(obj)/nandloader.bin

nandloader.bin: $(obj)/nandloader.bin

clean:
	rm -f nandloader.elf nandloader.bin $(extra-y)


