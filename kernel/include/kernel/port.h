
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

#ifndef _KERNEL_PORT_H
#define _KERNEL_PORT_H

static inline void io_wait(void)
{
    /* Port 0x80 is used for 'checkpoints' during POST. */
    /* The Linux kernel seems to think it is free for use :-/ */
    __asm__ __volatile__ ( "outb %b0, $0x80" : : "a"(0) );
}

static inline uint16_t inb (uint16_t port) {
	uint8_t ret;
	__asm__ __volatile__ ("inb %w1, %0" : "=a" (ret) : "dN" (port));
	return ret;
}

static inline void outb (uint16_t port, uint8_t data) {
	__asm__ __volatile__ ("outb %1, %w0" : : "dN" (port), "a" (data));
}

#endif
