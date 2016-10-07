#include <stdio.h>

#include <kernel/tty.h>
#include <kernel/gdt.h>

void kernel_main(void) {
	terminal_initialize();
	gdt_prepare();
	printf("Hello, kernel World!\n");
}
