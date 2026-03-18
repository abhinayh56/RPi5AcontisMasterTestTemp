/*
 * $QNXLicenseC:
 * Copyright 2008, QNX Software Systems.
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


#include "asmoff.def"
#include <x86_64/asm.h>

.text
//#define PRT_TRACE 1
//#define PRT(_c)   mov $_c, %al; out %al, (%dx)
#define PRT(_c)

/*
;
; We're starting an application processor in an SMP system. The processor
; is in real mode. Our mission, if we chose to accept it, is to get it
; into 32-bit protect mode and transfer control to an address (apstart32)
; in the startup code.
;
*/
.code16
	.globl	apstart16_start
	.globl	apstart16_end

.align 2
_aplink:
apstart16_start:
		cli
#ifdef PRT_TRACE
		mov   $0x2f8, %dx
#endif
		xor  %ax,%ax
		mov  %ax, %ss
		mov  %ax, %ds
		mov  %ax, %es
A_rbase = . + 2 - _aplink
		mov  $0x12345678, %ebx		// value patched by apstart_init()

		mov  %cr0, %eax
		and $~(X86_64_CR0_PG_BIT|X86_64_CR0_PE_BIT), %eax
		mov  %eax, %cr0

PRT('a')
		mov %cr4, %eax
		or $(X86_64_CR4_PAE|X86_64_CR4_PGE), %eax
		mov	%eax, %cr4

PRT('b')

A_proot = . + 2 - _aplink
		mov $0x12345678, %ecx		// value patched by apstart_init()
		mov %ecx, %cr3

PRT('c')
		mov $X86_64_MSR_EFER, %ecx
		rdmsr
		or $X86_64_EFER_LME, %eax
		and $~X86_64_EFER_NXE, %eax
		wrmsr

PRT('d')
		// convert transfer base address to seg:offset form
		mov		%ebx,%eax
		mov		%bx,%si
		shr		$4,%eax
		and		$0xf,%si
		mov		%ax,%ds
		lgdtl A_gdtp(%si)
PRT('e')
		lidtl A_idtp(%si)
PRT('f')
		mov %cr0, %eax
		or $(X86_64_CR0_PG_BIT|X86_64_CR0_PE_BIT), %eax
		mov %eax, %cr0
		// Make sure the paging enable has taken
		jmp flush
flush:
		.byte	0x66,0xea	// 16:32 far jump opcode
A_ljmp = . - _aplink
		.long	start64-_aplink	// patched by apstart_init()
		.short	0x1234			// patched by apstart_init()

.quad 0x55aadeadbeef
A_gdtp = .-_aplink
	.quad 1,2
A_idtp = .-_aplink
	.quad 3,4
A_initf = .-_aplink
	.quad 5

/*-bf:*/
.code64
start64:
	xor %ax,%ax
	mov %ax, %ss
	mov %ax, %ds
	mov %ax, %es
	jmp *A_initf(%ebx)

apstart16_end:
	.quad

STARTFUNC(apstart_init) /* void *relocated_addr, void (*initf)() */
	sgdt   A_gdtp(%rdi)
	sidt   A_idtp(%rdi)
	mov    %edi, A_rbase(%rdi)
	mov    %cr3, %rdx
	mov    %edx, A_proot(%rdi)
	mov    %cs, %ax
	mov    %ax, A_ljmp+4(%rdi)
	add    %edi, A_ljmp+0(%rdi)
	mov    %esi, A_initf(%rdi)
	mfence
	ret
ENDFUNC(apstart_init)
