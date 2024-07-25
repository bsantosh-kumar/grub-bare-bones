LINKER_FLAGS := -T linker.ld -nostdlib -lgcc -ffreestanding -O2
KERNEL_FLAGS := -c -ffreestanding -O2 -Wall -Wextra -std=gnu99
OS := sanOS
KERNEL := kernel
BOOT := boot


all: $(OS).bin

grub: $(OS).bin grub.cfg
	mkdir -p isodir/$(BOOT)/grub/
	cp $(OS).bin isodir/$(BOOT)/$(OS).bin
	cp grub.cfg isodir/$(BOOT)/grub/grub.cfg 
	grub-mkrescue -o $(OS).iso isodir

$(OS).bin: $(KERNEL).o $(BOOT).o
	i686-elf-gcc $(LINKER_FLAGS) -o $@ $^

$(KERNEL).o: $(KERNEL).c
	i686-elf-gcc $(KERNEL_FLAGS) -o $@ $^ 
$(BOOT).o: $(BOOT).s
	i686-elf-as $< -o $@
qemu-kernel: $(OS).bin
	qemu-system-i386 -kernel $<
qemu-grub: grub 
	qemu-system-i386 -cdrom $(OS).bin
clean:
	rm -rf *.o *.bin
