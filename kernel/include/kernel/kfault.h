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
 
#ifndef _KERNEL_KFAULT_H
#define _KERNEL_KFAULT_H

const char* kernel_exceptions[] = {
	"Division by zero",
	"Debug",
	"Non-maskable interrupt",
	"Breakpoint",
	"Overflow",
	"Out-of-bounds",
	"Invalid opcode",
	"No FPU",
	"Double fault",
	"FPU segment overrun",
	"Bad TSS",
	"Segment not present",
	"Stack fault",
	"General protection fault",
	"Page fault",
	"Unknown interrupt",
	"FPU fault",
	"Alignment check",
	"Machine check",
	"Reserved 1",
	"Reserved 2",
	"Reserved 3",
	"Reserved 4",
	"Reserved 5",
	"Reserved 6",
	"Reserved 7",
	"Reserved 8",
	"Reserved 9",
	"Reserved 10",
	"Reserved 11",
	"Reserved 12",
	"Reserved 13",
};

#endif
