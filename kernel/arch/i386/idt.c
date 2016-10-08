
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
	idt_ptr.limit = (sizeof(struct interrupt_descriptor) * IDT_SIZE)-1;
	idt_ptr.base = (size_t) &idt;
	memset(&idt, 0, sizeof(struct interrupt_descriptor) * 256);
	
	// TODO: interrupt service routines go here.
	
	installidt();
}
