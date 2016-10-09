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
 
#include <kernel/kfault.h>
#include <kernel/tty.h>
#include <stdio.h>
#include <stdint.h>

struct kregisters {
	uint32_t gs, fs, es, ds;
	uint32_t edi, esi, ebp, esp, ebx, edx, ecx, eax;
	uint32_t interrupt, errorcode;
	uint32_t eip, cs, eflags, uesp, ss;
};

void kfault(struct kregisters *reg) {
	if (reg->interrupt < 32) {
		printf(kernel_exceptions[reg->interrupt]);
		printf(" exception, halting system");
		for(;;);
	}
}
