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

#include <x86_64/asm.h>
#include "asmoff.def"


.text

#define		STACK_SIZE	11000

#define	SAVE_IP_OFF	0x00
#define	SAVE_CS_OFF	0x08
#define	SAVE_FL_OFF	0x010
#define	SAVE_SP_OFF	0x18
#define	SAVE_SS_OFF	0x20

	.extern		_main
	.extern		boot_regs
	.extern		entry_32prot
	.globl		stack_addr_lo
	.globl		stack_addr_hi

	.globl		_cstart_
	.globl	_start

entry_start:
	.quad	entry_32prot
	.short	. - entry_start
	.short	8
	.long	0xffacb5bd
_cstart_:
_start:
	xchg   %bx,%bx
	mov    %rax,boot_regs(%rip) //save EAX temporarily
	movw   %ds,%ax
	movw   %ax,%es
	movw   %ax,%ss
	lea    (stack_top-(REG_SS+8))(%rip), %rax
	mov    %rbx, REG_RBX(%rax)
	mov    %rcx, REG_RCX(%rax)
	mov    %rdx, REG_RDX(%rax)
	mov    %rsi, REG_RSI(%rax)
	mov    %rdi, REG_RDI(%rax)
	mov    %rbp, REG_RBP(%rax)
	mov    %rsp, REG_RSP(%rax)
	mov    %r8,  REG_R8 (%rax)
	mov    %r9,  REG_R9 (%rax)
	mov    %r10, REG_R10(%rax)
	mov    %r11, REG_R11(%rax)
	mov    %r12, REG_R12(%rax)
	mov    %r13, REG_R13(%rax)
	mov    %r14, REG_R14(%rax)
	mov    %r15, REG_R15(%rax)
	mov    %rax, %rsp
	mov    boot_regs(%rip), %rdx  // recover rax
	mov    %rdx, REG_RAX(%rax)
	mov    %rax, boot_regs(%rip)
	cld
	pushfq
	pop    %rax
	mov    $0x10, %ah    /* SET IOPL=1, clear NT,TF,IF,DF,OF */
	mov    %rax, (REG_RFLAGS)(%rsp)
	push   %rax
	popfq
    call early_cpu_setup
    test %eax, %eax
    jz   not_supported
/* 386 version does a privity change first, not sure why... */
	and		$~0xf,%rsp		// Align stack to 16 byte boundry as per ABI
	call   _main
not_supported:
1:
    jmp       1b

.align	8	// Align the stack to 8 bytes... (size of signature)
.globl	boot_args
boot_args:
	.ascii	  "ddpvbskr"	// signature for mkifs
stack_addr_lo:
	.space	  STACK_SIZE
stack_addr_hi:
stack_top:
