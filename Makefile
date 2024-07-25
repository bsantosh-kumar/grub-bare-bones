all:
	i686-elf-gcc -c -o kernel.o kernel.c -ffreestanding -O2 -Wall -Wextra -std=gnu99
	i686-elf-gcc -T linker.ld -o sanOS.bin -nostdlib -ffreestanding -O2 -lgcc kernel.o boot.o
	qemu-system-i386 -kernel sanOS.bin
qemu:
	qemu-system-i386 -kernel sanOS.bin 
