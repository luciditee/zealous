; expose entry point to linker
global _loader
extern kmain

; grub multiboot
MODULEALIGN equ	1<<0
MEMINFO		equ 1<<1
FLAGS		equ MODULEALIGN | MEMINFO
MAGIC		equ	  0x1BADB002
CHECKSUM	equ -(MAGIC + FLAGS)

KERNEL_VBASE	equ 0xC0000000
KERNEL_PAGE		equ (KERNEL_VBASE >> 22)

; identity-map the first 4mb of 32-bit address space to ensure kernel code will execute
section .data
align 0x1000
BootPageDirectory:
	dd 0x00000083
	times (KERNEL_PAGE - 1) dd 0
	dd 0x00000083
	times (1024 - KERNEL_PAGE - 1) dd 0
	
section .text
align 4
MultiBootHeader:
	dd MAGIC
	dd FLAGS
	dd CHECKSUM

; 16k stack for kernel space
STACKSIZE equ 0x4000

; linker offset
loader equ (_loader - 0xC0000000)
global loader

; the fun part
_loader:
	; load identity map into cr3
	mov ecx, (BootPageDirectory - KERNEL_VBASE)
	mov cr3, ecx 
	
	; set page size to 4mb
	mov ecx, cr4
	or ecx, 0x00000010 
	mov cr4, ecx
	
	; enable paging
	mov ecx, cr0
	or ecx, 0x80000000
	mov cr0, ecx
	
	lea ecx, [StartInHigherHalf] 
	jmp ecx ; jump to first instruction in the higher-half bootstrap

StartInHigherHalf:
	; okay, we made it into the kernel, go ahead and unmap the id map
	mov dword [BootPageDirectory], 0
	invlpg [0]
	
	; paging is enabled, welcome to higher-half land
	mov esp, stack+STACKSIZE
	push eax
	push ebx
	call kmain ; call kernel
	
	; long jump to hang if kernel returns
	lea ecx, [Hang]
	jmp ecx
	
; should the kernel main return, this ensures the processor stays unusable until reset
Hang:
	hlt
	cli
	jmp Hang

section .bss
align 32
stack:
	resb STACKSIZE
