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
.align 4

.global ex_divbyzero
.type ex_divbyzero, @function
ex_divbyzero:
	cli
	push $0
	push $0
	jmp isr_stub
	
.global ex_debug
.type ex_debug, @function
ex_debug:
	cli
	push $0
	push $1
	jmp isr_stub

.global ex_nonmaskint
.type ex_nonmaskint, @function
ex_nonmaskint:
	cli
	push $0
	push $2
	jmp isr_stub

.global ex_breakpoint
.type ex_breakpoint, @function
ex_breakpoint:
	cli
	push $0
	push $3
	jmp isr_stub
	
.global ex_overflow
.type ex_overflow, @function
ex_overflow:
	cli
	push $0
	push $4
	jmp isr_stub
	
.global ex_outofbounds
.type ex_outofbounds, @function
ex_outofbounds:
	cli
	push $0
	push $5
	jmp isr_stub
	
.global ex_badopcode
.type ex_badopcode, @function
ex_badopcode:
	cli
	push $0
	push $6
	jmp isr_stub
	
.global ex_nofpu
.type ex_nofpu, @function
ex_nofpu:
	cli
	push $0
	push $7
	jmp isr_stub
	
.global ex_doublefault
.type ex_doublefault, @function
ex_doublefault:
	cli
	push $8
	jmp isr_stub
	
.global ex_fpuoverrun
.type ex_fpuoverrun, @function
ex_fpuoverrun:
	cli
	push $0
	push $9
	jmp isr_stub
	
.global ex_badtss
.type ex_badtss, @function
ex_badtss:
	cli
	push $10
	jmp isr_stub

.global ex_badsegment
.type ex_badsegment, @function
ex_badsegment:
	cli
	push $11
	jmp isr_stub

.global ex_stackfault
.type ex_stackfault, @function
ex_stackfault:
	cli
	push $12
	jmp isr_stub

.global ex_gpf
.type ex_gpf, @function
ex_gpf:
	cli
	push $13
	jmp isr_stub

.global ex_pagefault
.type ex_pagefault, @function
ex_pagefault:
	cli
	push $14
	jmp isr_stub

.global ex_unknown
.type ex_unknown, @function
ex_unknown:
	cli
	push $0
	push $15
	jmp isr_stub
	
.global ex_fpufault
.type ex_fputfault, @function
ex_fpufault:
	cli
	push $0
	push $16
	jmp isr_stub
	
.global ex_alignment
.type ex_alignment, @function
ex_alignment:
	cli
	push $0
	push $17
	jmp isr_stub
	
.global ex_mcheck
.type ex_mcheck, @function
ex_mcheck:
	cli
	push $0
	push $18
	jmp isr_stub
	
.global ex_reserved1
.type ex_reserved1, @function
ex_reserved1:
	cli
	push $0
	push $19
	jmp isr_stub
	
.global ex_reserved2
.type ex_reserved2, @function
ex_reserved2:
	cli
	push $0
	push $20
	jmp isr_stub
	
.global ex_reserved3
.type ex_reserved3, @function
ex_reserved3:
	cli
	push $0
	push $21
	jmp isr_stub
	
.global ex_reserved4
.type ex_reserved4, @function
ex_reserved4:
	cli
	push $0
	push $22
	jmp isr_stub
	
.global ex_reserved5
.type ex_reserved5, @function
ex_reserved5:
	cli
	push $0
	push $23
	jmp isr_stub
	
.global ex_reserved6
.type ex_reserved6, @function
ex_reserved6:
	cli
	push $0
	push $24
	jmp isr_stub
	
.global ex_reserved7
.type ex_reserved7, @function
ex_reserved7:
	cli
	push $0
	push $25
	jmp isr_stub
	
.global ex_reserved8
.type ex_reserved8, @function
ex_reserved8:
	cli
	push $0
	push $26
	jmp isr_stub
	
.global ex_reserved9
.type ex_reserved9, @function
ex_reserved9:
	cli
	push $0
	push $27
	jmp isr_stub
	
.global ex_reserved10
.type ex_reserved10, @function
ex_reserved10:
	cli
	push $0
	push $28
	jmp isr_stub
	
.global ex_reserved11
.type ex_reserved11, @function
ex_reserved11:
	cli
	push $0
	push $29
	jmp isr_stub
	
.global ex_reserved12
.type ex_reserved12, @function
ex_reserved12:
	cli
	push $0
	push $30
	jmp isr_stub
	
.global ex_reserved13
.type ex_reserved13, @function
ex_reserved13:
	cli
	push $0
	push $31
	jmp isr_stub
	
isr_stub:
	pusha
	push %ds
	push %es
	push %fs
	push %gs
	mov $0x10, %ax
	mov %ax, %ds
	mov %ax, %es
	mov %ax, %fs
	mov %ax, %gs
	mov %esp, %eax
	push %eax
	mov kfault, %eax
	call *%eax
	pop %eax
	pop %gs
	pop %fs
	pop %es
	pop %ds
	popa
	add $8, %esp
	iret
	
	
