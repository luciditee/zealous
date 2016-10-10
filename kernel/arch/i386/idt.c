
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
#include <kernel/port.h>

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
extern void irq_0();
extern void irq_1();
extern void irq_2();
extern void irq_3();
extern void irq_4();
extern void irq_5();
extern void irq_6();
extern void irq_7();
extern void irq_8();
extern void irq_9();
extern void irq_10();
extern void irq_11();
extern void irq_12();
extern void irq_13();
extern void irq_14();
extern void irq_15();

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

#define PIC1		0x20		/* IO base address for master PIC */
#define PIC2		0xA0		/* IO base address for slave PIC */
#define PIC1_COMMAND	PIC1
#define PIC1_DATA	(PIC1+1)
#define PIC2_COMMAND	PIC2
#define PIC2_DATA	(PIC2+1)

#define ICW1_ICW4	0x01		/* ICW4 (not) needed */
#define ICW1_SINGLE	0x02		/* Single (cascade) mode */
#define ICW1_INTERVAL4	0x04		/* Call address interval 4 (8) */
#define ICW1_LEVEL	0x08		/* Level triggered (edge) mode */
#define ICW1_INIT	0x10		/* Initialization - required! */
 
#define ICW4_8086	0x01		/* 8086/88 (MCS-80/85) mode */
#define ICW4_AUTO	0x02		/* Auto (normal) EOI */
#define ICW4_BUF_SLAVE	0x08		/* Buffered mode/slave */
#define ICW4_BUF_MASTER	0x0C		/* Buffered mode/master */
#define ICW4_SFNM	0x10		/* Special fully nested (not) */
 
/* reinitialize the PIC controllers, giving them specified vector offsets
   rather than 8h and 70h, as configured by default */
 
/*
arguments:
	offset1 - vector offset for master PIC
		vectors on the master become offset1..offset1+7
	offset2 - same for slave PIC: offset2..offset2+7
*/
void PIC_remap(int offset1, int offset2)
{
	unsigned char a1, a2;
 
	a1 = inb(PIC1_DATA);                        // save masks
	a2 = inb(PIC2_DATA);
 
	outb(PIC1_COMMAND, ICW1_INIT+ICW1_ICW4);  // starts the initialization sequence (in cascade mode)
	io_wait();
	outb(PIC2_COMMAND, ICW1_INIT+ICW1_ICW4);
	io_wait();
	outb(PIC1_DATA, offset1);                 // ICW2: Master PIC vector offset
	io_wait();
	outb(PIC2_DATA, offset2);                 // ICW2: Slave PIC vector offset
	io_wait();
	outb(PIC1_DATA, 4);                       // ICW3: tell Master PIC that there is a slave PIC at IRQ2 (0000 0100)
	io_wait();
	outb(PIC2_DATA, 2);                       // ICW3: tell Slave PIC its cascade identity (0000 0010)
	io_wait();
 
	outb(PIC1_DATA, ICW4_8086);
	io_wait();
	outb(PIC2_DATA, ICW4_8086);
	io_wait();
 
	outb(PIC1_DATA, a1);   // restore saved masks.
	outb(PIC2_DATA, a2);
}

void idt_prepare() {
	// set up idt
	idt_ptr.limit = (sizeof(struct interrupt_descriptor) * IDT_SIZE)-1;
	idt_ptr.base = (size_t) &idt;
	memset(&idt, 0, sizeof(struct interrupt_descriptor) * 256);
    PIC_remap (0x20, 0x28);
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
	idt_set(32, (uint32_t)irq_0, 0x08, 0x8E);
	idt_set(33, (uint32_t)irq_1, 0x08, 0x8E);
	idt_set(34, (uint32_t)irq_2, 0x08, 0x8E);
	idt_set(35, (uint32_t)irq_3, 0x08, 0x8E);
	idt_set(36, (uint32_t)irq_4, 0x08, 0x8E);
	idt_set(37, (uint32_t)irq_5, 0x08, 0x8E);
	idt_set(38, (uint32_t)irq_6, 0x08, 0x8E);
	idt_set(39, (uint32_t)irq_7, 0x08, 0x8E);
	idt_set(40, (uint32_t)irq_8, 0x08, 0x8E);
	idt_set(41, (uint32_t)irq_9, 0x08, 0x8E);
	idt_set(42, (uint32_t)irq_10, 0x08, 0x8E);
	idt_set(43, (uint32_t)irq_11, 0x08, 0x8E);
	idt_set(44, (uint32_t)irq_12, 0x08, 0x8E);
	idt_set(45, (uint32_t)irq_13, 0x08, 0x8E);
	idt_set(46, (uint32_t)irq_14, 0x08, 0x8E);
	idt_set(47, (uint32_t)irq_15, 0x08, 0x8E);
}

