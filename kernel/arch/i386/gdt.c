
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
 
#define GDT_SIZE 5

extern void installgdt();

// honestly, easier for me to do it this way than to try and deal with
// bitmask hell...
struct gdt_entry {
    uint16_t limit_low;
    uint16_t low;
    uint8_t middle;
    uint8_t access;
    uint8_t granularity;
    uint8_t high;
} __attribute__((packed));

// pointer + max limit - 1
struct gdt_ptr {
    uint16_t limit;
    uint32_t base;
} __attribute__((packed));

// global descriptor table
struct gdt_entry gdt[GDT_SIZE];
struct gdt_ptr gp;

void gdtset(int32_t num, uint64_t base, uint64_t limit, uint8_t access,
	uint8_t gran) {
    
	// haha just kidding welcome to bitmask hell
	gdt[num].low = (base & 0xFFFF);
    gdt[num].middle = (base >> 16) & 0xFF;
    gdt[num].high = (base >> 24) & 0xFF;
    gdt[num].limit_low = (limit & 0xFFFF);
    gdt[num].granularity = ((limit >> 16) & 0x0F);
    gdt[num].granularity |= (gran & 0xF0);
    gdt[num].access = access;
}

// called by kernel entry to install the gdt
void gdt_prepare() {
    // set up gdt pointer for lgdt instruction
    gp.limit = (sizeof(struct gdt_entry) * GDT_SIZE) - 1;
    gp.base = (uint32_t)&gdt;
    gdtset(0, 0, 0, 0, 0); // null descriptor
    gdtset(1, 0, 0xFFFFFFFF, 0x9A, 0xCF); // code segment - kernel space
    gdtset(2, 0, 0xFFFFFFFF, 0x92, 0xCF); // data segment - kernel space
    gdtset(3, 0, 0xFFFFFFFF, 0xFE, 0xC0); // code segment - user space
    gdtset(4, 0, 0xFFFFFFFF, 0xFE, 0xC0); // data segment - user space
    installgdt();
}
