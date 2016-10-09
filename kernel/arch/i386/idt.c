
/*
 * Copyright 2016 Zealous Development Team.
 *
 * Licensed under the Apache License, Version 2.0 (the "License"); you may not
 * use this file except in compliance with the License. You may obtain a copy
 * of the License at
 *
 * http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS, WITHOUT
 * WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied. See the
 * License for the specific language governing permissions and limitations
 * under the License.
 *
 */

#include <stdint.h>
#include <string.h>

#define IDT_SIZE 256

extern void installidt();
extern void ex_divbyzero();
extern void ex_debug();
extern void ex_nonmaskint();
extern void ex_breakpoint();
extern void ex_overflow();
extern void ex_outofbounds();
extern void ex_badopcode();
extern void ex_nofpu();
extern void ex_doublefault();
extern void ex_fpuoverrun();
extern void ex_badtss();
extern void ex_badsegment();
extern void ex_stackfault();
extern void ex_gpf();
extern void ex_pagefault();
extern void ex_badtss();
extern void ex_fpufault();
extern void ex_alignment();
extern void ex_mcheck();
extern void ex_reserved1();
extern void ex_reserved2();
extern void ex_reserved3();
extern void ex_reserved4();
extern void ex_reserved5();
extern void ex_reserved6();
extern void ex_reserved7();
extern void ex_reserved8();
extern void ex_reserved9();
extern void ex_reserved10();
extern void ex_reserved11();
extern void ex_reserved12();
extern void ex_reserved13();

struct interrupt_descriptor {
	uint16_t lower_offset;
	uint16_t segment;
	uint8_t padding;
	uint8_t flags;
	uint16_t upper_offset;
} __attribute__((packed));

struct idt_pointer {
	uint16_t limit;
	size_t base;
} __attribute__((packed));

struct interrupt_descriptor idt[IDT_SIZE];
struct idt_pointer idt_ptr;

void idt_set(uint8_t index, uint64_t base, uint16_t selector, uint8_t flags) {
	idt[index].lower_offset = (base & 0xFFFF);
	idt[index].upper_offset = (base >> 16) & 0xFFFF;
	idt[index].segment = selector;
	idt[index].padding = 0;
	idt[index].flags = flags;
}

void idt_prepare() {
	// set up idt
	idt_ptr.limit = (sizeof(struct interrupt_descriptor) * IDT_SIZE)-1;
	idt_ptr.base = (size_t) &idt;
	memset(&idt, 0, sizeof(struct interrupt_descriptor) * 256);
	installidt();
	
	// set up exceptions
	idt_set(0, (uint32_t)ex_divbyzero, 0x08, 0x8E);
	idt_set(1, (uint32_t)ex_debug, 0x08, 0x8E);
	idt_set(2, (uint32_t)ex_nonmaskint, 0x08, 0x8E);
	idt_set(3, (uint32_t)ex_breakpoint, 0x08, 0x8E);
	idt_set(4, (uint32_t)ex_overflow, 0x08, 0x8E);
	idt_set(5, (uint32_t)ex_outofbounds, 0x08, 0x8E);
	idt_set(6, (uint32_t)ex_badopcode, 0x08, 0x8E);
	idt_set(7, (uint32_t)ex_nofpu, 0x08, 0x8E);
	idt_set(8, (uint32_t)ex_doublefault, 0x08, 0x8E);
	idt_set(9, (uint32_t)ex_fpuoverrun, 0x08, 0x8E);
	idt_set(10, (uint32_t)ex_badtss, 0x08, 0x8E);
	idt_set(11, (uint32_t)ex_badsegment, 0x08, 0x8E);
	idt_set(12, (uint32_t)ex_stackfault, 0x08, 0x8E);
	idt_set(13, (uint32_t)ex_gpf, 0x08, 0x8E);
	idt_set(14, (uint32_t)ex_pagefault, 0x08, 0x8E);
	idt_set(15, (uint32_t)ex_badtss, 0x08, 0x8E);
	idt_set(16, (uint32_t)ex_fpufault, 0x08, 0x8E);
	idt_set(17, (uint32_t)ex_alignment, 0x08, 0x8E);
	idt_set(18, (uint32_t)ex_mcheck, 0x08, 0x8E);
	idt_set(19, (uint32_t)ex_reserved1, 0x08, 0x8E);
	idt_set(20, (uint32_t)ex_reserved2, 0x08, 0x8E);
	idt_set(21, (uint32_t)ex_reserved3, 0x08, 0x8E);
	idt_set(22, (uint32_t)ex_reserved4, 0x08, 0x8E);
	idt_set(23, (uint32_t)ex_reserved5, 0x08, 0x8E);
	idt_set(24, (uint32_t)ex_reserved6, 0x08, 0x8E);
	idt_set(25, (uint32_t)ex_reserved7, 0x08, 0x8E);
	idt_set(26, (uint32_t)ex_reserved8, 0x08, 0x8E);
	idt_set(27, (uint32_t)ex_reserved9, 0x08, 0x8E);
	idt_set(28, (uint32_t)ex_reserved10, 0x08, 0x8E);
	idt_set(29, (uint32_t)ex_reserved11, 0x08, 0x8E);
	idt_set(30, (uint32_t)ex_reserved12, 0x08, 0x8E);
	idt_set(31, (uint32_t)ex_reserved13, 0x08, 0x8E);
}

