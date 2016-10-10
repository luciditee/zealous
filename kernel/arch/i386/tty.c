#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
#include <string.h>

#include <kernel/tty.h>
#include <kernel/port.h>

#include "vga.h"

static const size_t VGA_WIDTH = 80;
static const size_t VGA_HEIGHT = 25;
static uint16_t* const VGA_MEMORY = (uint16_t*) 0xC03FF000;

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
	uint8_t point = (y * VGA_WIDTH) + x;
	outb(0x3D4, 14);
	outb(0x3D5, point >> 8);
	outb(0x3D4, 15);
	outb(0x3D5, point);
}

void vgaterm_putentryat(unsigned char c, uint8_t color, size_t x, size_t y) {
	const size_t index = y * VGA_WIDTH + x;
	vgaterm_buffer[index] = vga_entry(c, color);
}

void vgaterm_putchar(char c) {
	unsigned char uc = c;
	vgaterm_putentryat(uc, vgaterm_color, vgaterm_column, vgaterm_row);
	size_t column = vgaterm_column + 1;
	size_t row = vgaterm_row;
	if (column == VGA_WIDTH) {
		column = 0;
		if (row == VGA_HEIGHT) 
			row = 0;
	}
	vgaterm_mvcursor(column, row);
	
	if (++vgaterm_column == VGA_WIDTH) {
		vgaterm_column = 0;
		if (++vgaterm_row == VGA_HEIGHT) {
			vgaterm_row = 0;
		}
	}
}

void vgaterm_write(const char* data, size_t size) {
	for (size_t i = 0; i < size; i++)
		vgaterm_putchar(data[i]);
}

void vgaterm_writestring(const char* data) {
	vgaterm_write(data, strlen(data));
}
