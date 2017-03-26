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

#ifndef _KERNEL_SDEBUG_H
#define _KERNEL_SDEBUG_H

#include <stdint.h>
#include <kernel/port.h>
#include <string.h>

static inline void sdebug_putc(char c) {
	outb(0x3F8, (uint8_t)c);
}

static inline void sdebug_puts(const char* str, size_t length) {
	for (size_t i = 0; i < length; i++) {
		sdebug_putc(str[i]);
	}
}

static inline void sdebug_putln(const char* str, size_t length) {
	for (size_t i = 0; i < length; i++) {
		sdebug_putc(str[i]);
	}
	
	sdebug_putc('\r');
	sdebug_putc('\n');
}

#endif
