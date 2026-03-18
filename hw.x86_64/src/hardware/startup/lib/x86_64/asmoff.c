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






/*
	This source file isn't included in the startup library.
	It's only used to build the asmoff.def file.

	CPU specific definitions
*/
#include "asmoff.h"
#include <x86/lapic.h>
#include <x86_64/context.h>

VALUE(LAPIC_EOI,			LAPIC_EOI);
VALUE(LAPIC_LTR,			LAPIC_LTR);
VALUE(LVT_MASKED,			LVT_MASKED);
VALUE(LAPIC_ICR,			LAPIC_ICR);
VALUE(LAPIC_CCR,			LAPIC_CCR);

VALUE(REG_RDI,		offsetof(X86_64_CPU_REGISTERS, rdi));
VALUE(REG_RSI,		offsetof(X86_64_CPU_REGISTERS, rsi));
VALUE(REG_RDX,		offsetof(X86_64_CPU_REGISTERS, rdx));
VALUE(REG_R10,		offsetof(X86_64_CPU_REGISTERS, r10));
VALUE(REG_R8,		offsetof(X86_64_CPU_REGISTERS, r8));
VALUE(REG_R9,		offsetof(X86_64_CPU_REGISTERS, r9));
VALUE(REG_RAX,		offsetof(X86_64_CPU_REGISTERS, rax));
VALUE(REG_RBX,		offsetof(X86_64_CPU_REGISTERS, rbx));
VALUE(REG_RBP,		offsetof(X86_64_CPU_REGISTERS, rbp));
VALUE(REG_RCX,		offsetof(X86_64_CPU_REGISTERS, rcx));
VALUE(REG_R11,		offsetof(X86_64_CPU_REGISTERS, r11));
VALUE(REG_R12,		offsetof(X86_64_CPU_REGISTERS, r12));
VALUE(REG_R13,		offsetof(X86_64_CPU_REGISTERS, r13));
VALUE(REG_R14,		offsetof(X86_64_CPU_REGISTERS, r14));
VALUE(REG_R15,		offsetof(X86_64_CPU_REGISTERS, r15));
VALUE(REG_RIP,		offsetof(X86_64_CPU_REGISTERS, rip));
VALUE(REG_CS,		offsetof(X86_64_CPU_REGISTERS, cs));
VALUE(REG_RFLAGS,	offsetof(X86_64_CPU_REGISTERS, rflags));
VALUE(REG_RSP,		offsetof(X86_64_CPU_REGISTERS, rsp));
VALUE(REG_SS,		offsetof(X86_64_CPU_REGISTERS, ss));

VALUE(X86_64_PTE_PRESENT,	X86_64_PTE_PRESENT);
VALUE(X86_64_PTE_WRITE,		X86_64_PTE_WRITE);
VALUE(X86_64_PTE_USER,		X86_64_PTE_USER);
VALUE(X86_64_PTE_PS,		X86_64_PTE_PS);

VALUE(X86_64_CR0_PG_BIT,	X86_64_CR0_PG_BIT);
VALUE(X86_64_CR0_PE_BIT,	X86_64_CR0_PE_BIT);

VALUE(X86_64_CR4_PAE,		X86_64_CR4_PAE);
VALUE(X86_64_CR4_PGE,		X86_64_CR4_PGE);

VALUE(X86_64_MSR_EFER,		X86_64_MSR_EFER);
VALUE(X86_64_EFER_LME,		X86_64_EFER_LME);
VALUE(X86_64_EFER_NXE,		X86_64_EFER_NXE);

VALUE(PROT_READ,			PROT_READ);
VALUE(PROT_WRITE,			PROT_WRITE);
