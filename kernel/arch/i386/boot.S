#
# Copyright 2016 Zealous Development Team.
#
# Licensed under the Apache License, Version 2.0 (the "License"); you may not
# use this file except in compliance with the License. You may obtain a copy
# of the License at
#
# http://www.apache.org/licenses/LICENSE-2.0
#
# Unless required by applicable law or agreed to in writing, software
# distributed under the License is distributed on an "AS IS" BASIS, WITHOUT
# WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied. See the
# License for the specific language governing permissions and limitations
# under the License.
#

.set ALIGN,    1<<0             # align to page
.set MEMINFO,  1<<1             # provide memory map
.set FLAGS,    ALIGN | MEMINFO  # multiboot flag
.set MAGIC,    0x1BADB002       # bootloader looks for this in the image to indicate this a kernel
.set CHECKSUM, -(MAGIC + FLAGS) # checksum of the magic flag

.section .multiboot
.align 4
.long MAGIC
.long FLAGS
.long CHECKSUM

.section .bootstrap_stack, "aw", @nobits
stack_bottom:
.skip 16384 # 16 KiB
stack_top:

.section .bss, "aw", @nobits
	.align 4096
boot_page_directory:
	.skip 4096
boot_page_table1:
	.skip 4096

.section .text
.global _start
.type _start, @function
_start:
	movl $(boot_page_table1 - 0xC0000000), %edi
	movl $0, %esi
	movl $1023, %ecx

1:
	cmpl $(_kernel_start - 0xC0000000), %esi
	jl 2f
	cmpl $(_kernel_end - 0xC0000000), %esi
	jge 3f

	movl %esi, %edx
	orl $0x003, %edx
	movl %edx, (%edi)

2:
	addl $4096, %esi
	addl $4, %edi
	loop 1b

3:
	movl $(0x000B8000 | 0x003), boot_page_table1 - 0xC0000000 + 1023 * 4
	movl $(boot_page_table1 - 0xC0000000 + 0x003), boot_page_directory - 0xC0000000 + 0
	movl $(boot_page_table1 - 0xC0000000 + 0x003), boot_page_directory - 0xC0000000 + 768 * 4
	movl $(boot_page_directory - 0xC0000000), %ecx
	movl %ecx, %cr3
	movl %cr0, %ecx
	orl $0x80010000, %ecx
	movl %ecx, %cr0
	lea 4f, %ecx
	jmp *%ecx

4:
	movl $0, boot_page_directory + 0
	movl %cr3, %ecx
	movl %ecx, %cr3
	mov $stack_top, %esp
	call gdt_prepare
	call idt_prepare
	
	sti
	call vgaterm_initialize
	call kernel_main
	cli
	
1:	hlt
	jmp 1b

# global descriptor table
.global installgdt
.type installgdt, @function
installgdt:
	cli
	lgdt (gp)
	mov $0x10, %ax
	mov %ax, %ds
	mov %ax, %es
	mov %ax, %fs
	mov %gs, %ax
	mov %ss, %ax
	ret
	
.global installidt
.type installidt, @function
installidt:
	lidt (idt_ptr)
	ret
	
