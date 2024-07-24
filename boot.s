.set ALIGN, 1<<0 /*This means align loaded modules to page boundaries*/
.set MEMINFO, 1<<1 /*Asking the bootloader to provide the mmap*/
.set FLAGS, ALIGN | MEMINFO /*This is for the flag of multiboot header*/
.set MAGIC, 0x1BADB002 
.set CHECKSUM, -(MAGIC+FLAGS) 

.section .multiboot
.align 4 /*because bootloader search for magic number in 32 bit boundary*/
.long MAGIC
.long FLAGS
.long CHECKSUM

.section .bss
stack_bottom: /*stack grows downward, where the least address is in the bottom most address, so bottom will have the least address*/
.skip 16384
stack_top: /*Stack starts from stack_top and will be till stack_bottom, so this is the label that we are going to push into $esp */

.section .text
/*bootloader will start from this address, and we don't return from here, linker is going to call this '_start' function*/
.global _start
.type _start @function 
_start:
/*when we reach this point, we are in x86 protected mode , set by the bootloader. Processor state is same as specified in the multi-boot standard, paging is disabled, interrupts are disabled, it doesn't support standard header, like stdio.h. It is upto to the kernel to provide all of these. It has no security mechanism, no debugging, no safegaurds, it has complete control over the machine*/

/*We setup stack here*/
mov $stack_top, %esp
/*Floating point instructions, and instruction set extensions are not enabled yet, we have to initialize GDT and paging here */

/*Here we enter into high level kernel. Stack should be 16 bits aligned when we call the call method*/
call kernel_main 
/*after calling our kernel code, we disable all interrupts, and halt the system, if any interrupt does we jump back to hlt*/
cli
1: hlt
jmp 1b

/*we set the size of _start symbol for debugging purposes*/
.size _start, . - _start


