/*
 * $QNXLicenseC:
 * Copyright 2018, QNX Software Systems.
 *
 * Licensed under the Apache License, Version 2.0 (the "License"). You
 * may not reproduce, modify or distribute this software except in
 * compliance with the License. You may obtain a copy of the License
 * at: http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" basis,
 * WITHOUT WARRANTIES OF ANY KIND, either express or implied.
 *
 * This file may contain contributions from others, either as
 * contributors under the License or as licensors under other terms.
 * Please review this entire file for other proprietary rights or license
 * notices, as well as the QNX Development Suite License Guide at
 * http://licensing.qnx.com/license-guide/ for other information.
 * $
 */


#include <x86_64/asm.h>

.text

// both x86_cpuid & x86_64_cpuid defined to make it easier
// on shared code
.global x86_cpuid
x86_cpuid:
STARTFUNC(x86_64_cpuid)
	push	%rbx
	mov %rdi, %rax       //ARG1 cpuid#
	cpuid
	test %rsi, %rsi		//ARG2 buffer
	jz  saved
	movl %ebx, 0(%rsi)
	movl %edx, 4(%rsi)
	movl %ecx, 8(%rsi)
saved:
	pop	%rbx
	ret
ENDFUNC(x86_64_cpuid);

STARTFUNC(x86_64_setidt)
	mov %di, -10(%rsp)
	mov %rsi, -8(%rsp)
	lidt -10(%rsp)
	ret
ENDFUNC(x86_64_setidt)


STARTFUNC(x86_64_setgdt)
	sub  $16, %rsp
	mov  %di, (%rsp)
	mov  %rsi, 2(%rsp)
	lgdt (%rsp)
	add  $16, %rsp
	ret
ENDFUNC(x86_64_setgdt)


STARTFUNC(x86_64_setsegs)
	sub  $16, %rsp
	xor  %rsi,%rsi
	mov  %si,%ds
	mov  %si,%es
	mov  %si,%ss
	mov  %rdi, 8(%rsp)
	lea  _cont(%rip), %rdx
	mov  %rdx, (%rsp)
	lretq	/* cleans up stack */
_cont:
	ret
ENDFUNC(x86_64_setgdt)
