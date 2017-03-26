#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
#include <string.h>

#include <kernel/tty.h>
#include <kernel/port.h>

#include "vga.h"

static const size_t VGA_WIDTH = 80;
static const size_t VGA_HEIGHT = 25;
static uint16_t* const VGA_MEMORY = (uint16_t*) 0xC03FF000; // 3FF000 offset to 0xC0000000, or kernel position

static size_t vgaterm_row;
static size_t vgaterm_column;
static uint8_t vgaterm_color;
static uint16_t* vgaterm_buffer;

void vgaterm_initialize(void) {
	vgaterm_row = 0;
	vgaterm_column = 0;
	vgaterm_color = vga_entry_color(VGA_COLOR_LIGHT_GREY, VGA_COLOR_BLACK);
	vgaterm_buffer = VGA_MEMORY;
	vgaterm_cls();
}

void vgaterm_cls() {
	for (size_t y = 0; y < VGA_HEIGHT; y++) {
		for (size_t x = 0; x < VGA_WIDTH; x++) {
			const size_t index = y * VGA_WIDTH + x;
			vgaterm_buffer[index] = vga_entry(' ', vgaterm_color);
		}
	}
	vgaterm_mvcursor(0,0);
}

void vgaterm_setcolor(uint8_t color) {
	vgaterm_color = color;
}

void vgaterm_mvcursor(size_t x, size_t y) {
	uint16_t point = (y * VGA_WIDTH) + x;
	outb(0x3D4, 0x0F); // low port
	outb(0x3D5, (unsigned char)(point&0xFF));
	outb(0x3D4, 0x0E);
	outb(0x3D5, (unsigned char)((point>>8)&0xFF));
}

void vgaterm_scroll() {
	// I chose to use separate loops for this because it ends up being faster than clearing and
	// redrawing the entire screen for each line
	// Clear first row
	for (size_t r = 0; r < VGA_WIDTH; r++) {
		vgaterm_buffer[r] = vga_entry(' ', vgaterm_color);
	}
	
	// Iterate all subsequent rows and copy their contents to the previous one.
	for (size_t y = 1; y < VGA_HEIGHT; y++) {
		for (size_t x = 0; x < VGA_WIDTH; x++) {
			const size_t thisRow = (y * VGA_WIDTH) + x;
			const size_t previousRow = (y-1) * VGA_WIDTH + x;
			vgaterm_buffer[previousRow] = vgaterm_buffer[thisRow];
		}
	}
	
	// Blank final row and set cursor position to beginning of bottom line
	for (size_t z = 0; z < VGA_WIDTH; z++) {
		const size_t finalIndex = (VGA_HEIGHT-1) * VGA_WIDTH + z;
		vgaterm_buffer[finalIndex] = vga_entry(' ', vgaterm_color);
	}
	
	vgaterm_column = 0;
	vgaterm_row = VGA_HEIGHT-1;
	vgaterm_mvcursor(0, vgaterm_row);
}

void vgaterm_putentryat(unsigned char c, uint8_t color, size_t x, size_t y) {
	const size_t index = y * VGA_WIDTH + x;
	vgaterm_buffer[index] = vga_entry(c, color);
}

void vgaterm_putchar(char c) {
	unsigned char uc = c;
	size_t next;
		
	switch (uc) {
		case 0: // ignore null bytes
		case '\v': // ignore vertical tabs, they serve no purpose on a vgaterm
			return;
		case '\n': // line feed, falls through to carriage return
			if (++vgaterm_row == VGA_HEIGHT) {
				vgaterm_scroll();
				
			}
		case '\r': // carriage return
			vgaterm_column = 0;
			vgaterm_mvcursor(0, vgaterm_row);
			return;
		case '\t': // horizontal tab
			next = vgaterm_column + 1;
			while (next % VGATERM_TAB_STOP != 0) {
				next++;
				if (next >= VGA_WIDTH) {
					if (++vgaterm_row == VGA_HEIGHT) {
						vgaterm_scroll();
					}
					next = 0;
					break;
				}
			}
			
			vgaterm_column = next;
			vgaterm_mvcursor(vgaterm_column, vgaterm_row);
			return;
		break;
		case '\b': // backspace
			if (vgaterm_column == 0) {
				if (vgaterm_row == 0) return;
				vgaterm_column = VGA_WIDTH - 1;
				vgaterm_row--;
			} else {
				vgaterm_column--;
			}
				
			vgaterm_mvcursor(vgaterm_column, vgaterm_row);
			return;
		break;
	}
	
	vgaterm_putentryat(uc, vgaterm_color, vgaterm_column, vgaterm_row);
	
	
	if (++vgaterm_column == VGA_WIDTH) {
		vgaterm_column = 0;
		if (++vgaterm_row == VGA_HEIGHT) {
			vgaterm_scroll();
		}
	}
	
	vgaterm_mvcursor(vgaterm_column, vgaterm_row);
}

void vgaterm_write(const char* data, size_t size) {
	for (size_t i = 0; i < size; i++)
		vgaterm_putchar(data[i]);
}

void vgaterm_writestring(const char* data) {
	vgaterm_write(data, strlen(data));
}
