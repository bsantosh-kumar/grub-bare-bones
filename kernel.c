/*
to compile: 
	i686-elf-gcc -c kernel.c -o kernel.o -std=gnu99 -ffreestanding -O2 -Wall -Wextra
*/

#include<stdbool.h> //for bool data type
#include<stddef.h> //for size_t and NULL
#include<stdint.h> //for uintx_t and intx_t
/*All of the above headers are provided by the compiler, which are essential for OS development*/

#if defined(__linux__)
#error "You are not using cross compiler"
#endif
#if !defined(__i386__)
#error "You are not in ix86 compiler"
#endif

/*VGA colors*/
enum vga_color{
	VGA_COLOR_BLACK = 0,
	VAG_COLOR_BLUE = 1,
	VGA_COLOR_GREEN = 2,
	VAG_COLOR_CYAN = 3, 
	VGA_COLOR_RED = 4,
	VGA_COLOR_MAGENTA = 5, 
	VGA_COLOR_BROWN = 6, 
	VGA_COLOR_LIGHT_GREY = 7, 
	VGA_COLOR_DARK_GREY = 8, 
	VGA_COLOR_LIGHT_BLUE = 9, 
	VGA_COLOR_LIGHT_GREEN = 10, 
	VGA_COLOR_LIGHT_CYAN = 11, 
	VGA_COLOR_LIGHT_RED = 12, 
	VGA_COLOR_LIGHT_MAGENTA = 13, 
	VGA_COLOR_LIGHT_BROWN = 14, 
	VGA_COLOR_WHITE = 15
};

static inline uint8_t vga_entry_color(enum vga_color fg, enum vga_color bg){
	return fg | bg<<4;
}
static inline uint16_t vga_entry(unsigned char c, uint8_t color){
	return (uint16_t)c | (uint16_t)color<<8;
}
size_t strlen(const char *s){
	size_t size=0;
	while(s[size]) size++;
	return size;
}

static const uint8_t VGA_WIDTH = 80;
static const uint8_t VGA_HEIGHT = 25;
static uint16_t default_color=0;

/*these are not static because we might use them else where*/
uint16_t* terminal_buffer = 0;
size_t terminal_row = 0;
size_t terminal_col = 0;

static inline size_t get_index(size_t row,size_t col){
	return VGA_WIDTH*row + col;
}

void update_cursor_position(){
	if(++terminal_col==VGA_WIDTH){
		terminal_col = 0;
		if(++terminal_row==VGA_HEIGHT){
			terminal_row = 0;
		}
	}
}

void terminal_put_char(const char c){
	uint16_t currEntry = vga_entry(c,default_color);
	size_t idx = get_index(terminal_row, terminal_col);
	terminal_buffer[idx]=currEntry;
	update_cursor_position();
}

void terminal_put(const char *s, size_t len){
	for(size_t i=0;i<len;++i) 
		terminal_put_char(s[i]);
}

void terminal_put_string(const char *s){
	terminal_put(s,strlen(s));
}


void initalize_terminal(){
	default_color = vga_entry_color(VGA_COLOR_RED,VGA_COLOR_BLACK);
	terminal_buffer = (uint16_t*)0xb8000;
	for(uint8_t i=0;i<VGA_HEIGHT;++i){
		for(uint8_t j=0;j<VGA_WIDTH;++j){
			terminal_put_char(' ');	
		}
	}
}

void kernel_main(){
	initalize_terminal();
	terminal_put_string("Welcome to SanOS");
}
