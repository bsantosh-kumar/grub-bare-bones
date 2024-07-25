LINKER_FLAGS := -T linker.ld -nostdlib -lgcc -ffreestanding -O2
KERNEL_FLAGS := -c -ffreestanding -O2 -Wall -Wextra -std=gnu99

all: sanOS.bin

grub: sanOS.bin grub.cfg
	mkdir -p isodir/boot/grub/
	cp sanOS.bin isodir/boot/sanOS.bin
	cp grub.cfg isodir/boot/grub/grub.cfg 
	grub-mkrescue -o sanOS.iso isodir

sanOS.bin: kernel.o boot.o
	i686-elf-gcc $(LINKER_FLAGS) -o $@ $^

kernel.o: kernel.c
	i686-elf-gcc $(KERNEL_FLAGS) -o $@ $^ 
boot.o: boot.s
	i686-elf-as $< -o $@
qemu: sanOS.bin
	qemu-system-i386 -kernel $<
clean:
	rm -rf *.o *.bin
