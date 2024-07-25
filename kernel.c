/*
to compile:
	i686-elf-gcc -c kernel.c -o kernel.o -std=gnu99 -ffreestanding -O2 -Wall -Wextra
*/

#include <stdbool.h> //for bool data type
#include <stddef.h>	 //for size_t and NULL
#include <stdint.h>	 //for uintx_t and intx_t
/*All of the above headers are provided by the compiler, which are essential for OS development*/

#if defined(__linux__)
#error "You are not using cross compiler"
#endif
#if !defined(__i386__)
#error "You are not in ix86 compiler"
#endif

/*VGA colors*/
enum vga_color
{
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

static inline uint8_t vga_entry_color(enum vga_color fg, enum vga_color bg)
{
	return fg | bg << 4;
}
static inline uint16_t vga_entry(unsigned char c, uint8_t color)
{
	return (uint16_t)c | (uint16_t)color << 8;
}
size_t strlen(const char *s)
{
	size_t size = 0;
	while (s[size])
		size++;
	return size;
}

static const uint8_t VGA_WIDTH = 80;
static const uint8_t VGA_HEIGHT = 25;
static uint16_t default_color = 0;

/*these are not static because we might use them else where*/
uint16_t *terminal_buffer = 0;
size_t terminal_row = 0;
size_t terminal_col = 0;

static inline size_t get_index(size_t row, size_t col)
{

	return VGA_WIDTH * row + col;
}
void shift_terminal(size_t shift_no)
{
	if (shift_no > VGA_WIDTH * VGA_HEIGHT)
		return;
	size_t total_len = VGA_WIDTH * VGA_HEIGHT;
	for (size_t i = shift_no; i < total_len; i++)
	{
		terminal_buffer[i - shift_no] = terminal_buffer[i];
	}
	for (size_t i = total_len - shift_no; i < total_len; i++)
		terminal_buffer[i] = vga_entry(' ', default_color);
}
/*
	TODO: Make the function work for pos>80*25
	The following function assumes that pos is small value not as big as
	80*25

*/
size_t ceil(size_t n, size_t d)
{
	if (n % d == 0)
		return n / d;
	else
		return n / d + 1;
}
void update_cursor_position(int pos)
{

	if (pos == -1)
	{ // this means new line
		terminal_col = 0;
		if (++terminal_row == VGA_HEIGHT)
		{
			terminal_row--;
			shift_terminal(VGA_WIDTH);
		}
		return;
	}
	if (pos < 0) // this shouldn't happen
		return;
	size_t num = get_index(terminal_row, terminal_col) + pos;
	size_t indx = num % (VGA_WIDTH * VGA_HEIGHT);
	terminal_row = indx / VGA_WIDTH;
	terminal_col = indx % VGA_WIDTH;
	if (num < VGA_HEIGHT * VGA_WIDTH)
		return;
	terminal_row = VGA_HEIGHT - 1;
	num -= (VGA_HEIGHT * VGA_WIDTH - 1);
	shift_terminal((ceil(num, VGA_WIDTH)) * VGA_WIDTH);
	for (size_t i = 0; i < terminal_col; i++)
	{
		size_t idx = get_index(terminal_row, i);
		terminal_buffer[idx] = vga_entry(' ', default_color);
	}
}
/*This checks for whitespace, if yes then moves the cursor accordingly*/
static bool check_space(const char c)
{
	if (c == '\n')
	{
		update_cursor_position(-1);
		return true;
	}
	else
		return false;
}

void terminal_put_char(const char c)
{
	// based on whitespace character we get skip printing
	if (check_space(c))
		return;
	size_t idx = get_index(terminal_row, terminal_col);
	uint16_t currEntry = vga_entry(c, default_color);
	terminal_buffer[idx] = currEntry;
	update_cursor_position(1);
}

void terminal_put(const char *s, size_t len)
{
	for (size_t i = 0; i < len; ++i)
		terminal_put_char(s[i]);
}

void terminal_put_string(const char *s)
{
	terminal_put(s, strlen(s));
}

void initalize_terminal()
{
	terminal_row = 0;
	terminal_col = 0;
	default_color = vga_entry_color(VGA_COLOR_RED, VGA_COLOR_BLACK);
	terminal_buffer = (uint16_t *)0xb8000;
	for (uint8_t i = 0; i < VGA_HEIGHT; ++i)
	{
		for (uint8_t j = 0; j < VGA_WIDTH; ++j)
		{
			terminal_put_char(' ');
		}
	}
}

void kernel_main()
{
	initalize_terminal();
	//	terminal_put_string("Welcome to SanOS\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\nn\n\n\n\n\nn\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\nSomething here\n Somethere else here");

	terminal_put_string("1 Welcome to SanOS Welcome to SanOS Welcome to SanOS Welcome to SanOS Welcome t\n");
	terminal_put_string("2 Welcome to SanOS Welcome to SanOS Welcome to SanOS Welcome to SanOS Welcome t\n");
	terminal_put_string("3 Welcome to SanOS Welcome to SanOS Welcome to SanOS Welcome to SanOS Welcome t\n");
	terminal_put_string("4 Welcome to SanOS Welcome to SanOS Welcome to SanOS Welcome to SanOS Welcome t\n");
	terminal_put_string("5 Welcome to SanOS Welcome to SanOS Welcome to SanOS Welcome to SanOS Welcome t\n");
	terminal_put_string("6 Welcome to SanOS Welcome to SanOS Welcome to SanOS Welcome to SanOS Welcome t\n");
	terminal_put_string("7 Welcome to SanOS Welcome to SanOS Welcome to SanOS Welcome to SanOS Welcome t\n");
	terminal_put_string("8 Welcome to SanOS Welcome to SanOS Welcome to SanOS Welcome to SanOS Welcome t\n");
	terminal_put_string("9 Welcome to SanOS Welcome to SanOS Welcome to SanOS Welcome to SanOS Welcome t\n");
	terminal_put_string("10 Welcome to SanOS Welcome to SanOS Welcome to SanOS Welcome to SanOS Welcome \n");
	terminal_put_string("11 Welcome to SanOS Welcome to SanOS Welcome to SanOS Welcome to SanOS Welcome \n");
	terminal_put_string("12 Welcome to SanOS Welcome to SanOS Welcome to SanOS Welcome to SanOS Welcome \n");
	terminal_put_string("13 Welcome to SanOS Welcome to SanOS Welcome to SanOS Welcome to SanOS Welcome \n");
	terminal_put_string("14 Welcome to SanOS Welcome to SanOS Welcome to SanOS Welcome to SanOS Welcome \n");
	terminal_put_string("15 Welcome to SanOS Welcome to SanOS Welcome to SanOS Welcome to SanOS Welcome \n");
	terminal_put_string("16 Welcome to SanOS Welcome to SanOS Welcome to SanOS Welcome to SanOS Welcome \n");
	terminal_put_string("17 Welcome to SanOS Welcome to SanOS Welcome to SanOS Welcome to SanOS Welcome \n");
	terminal_put_string("18 Welcome to SanOS Welcome to SanOS Welcome to SanOS Welcome to SanOS Welcome \n");
	terminal_put_string("19 Welcome to SanOS Welcome to SanOS Welcome to SanOS Welcome to SanOS Welcome \n");
	terminal_put_string("20 Welcome to SanOS Welcome to SanOS Welcome to SanOS Welcome to SanOS Welcome \n");
	terminal_put_string("21 Welcome to SanOS Welcome to SanOS Welcome to SanOS Welcome to SanOS Welcome \n");
	terminal_put_string("22 Welcome to SanOS Welcome to SanOS Welcome to SanOS Welcome to SanOS Welcome \n");
	terminal_put_string("23 Welcome to SanOS Welcome to SanOS Welcome to SanOS Welcome to SanOS Welcome \n");
	terminal_put_string("24 Welcome to SanOS Welcome to SanOS Welcome to SanOS Welcome to SanOS Welcome \n");
	terminal_put_string("25 Welcome to SanOS Welcome to SanOS Welcome to SanOS Welcome to SanOS Welcome \n");
	terminal_put_string("26 Welcome to SanOS Welcome to SanOS Welcome to SanOS Welcome to SanOS Welcome \0");
	terminal_put_string("26 Welcome to SanOS Welcome to SanOS Welcome to SanOS Welcome to SanOS Welcome \0");
	//  terminal_put_string("26 Welcome to SanOS Welcome to SanOS Welcome to SanOS Welcome to SanOS Welcome \n");
}
