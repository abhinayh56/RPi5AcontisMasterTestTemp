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



.text
	
	.extern		cpu_starting
	.extern		init_one_cpuinfo
	.extern		cpu_one_startup
	.extern		board_smp_adjust_num
	.extern		syspage_available
	.extern		smp_spin_vaddr
	.extern		cpu_startnext

	.globl		smp_start
smp_start:
    lea stk(%rip),%rsp
    call early_cpu_setup
    and %eax, %eax
    jz  smp_halt
    mov cpu_starting(%rip),%eax

    dec %eax
	mov %rax, %rdi
	call board_smp_adjust_num
	mov %rax,%rbx	/*save*/
	mov %rax,%rdi
	call cpu_one_startup
	mov %rbx,%rdi
    call init_one_cpuinfo
    mov  %rbx,%rdi		/* restore cpu number for lapic_configure() */
	call lapic_configure
    xor %eax,%eax
    mov %eax,cpu_starting(%rip)
	
	// Wait for the BSP to finish initializing the system page
1:
	mov 	syspage_available(%rip),%eax
    cmp 	%eax,%ebx
    jne     1b
    mov     %rbx, %rsi
    mov    smp_spin_vaddr(%rip), %rdi
    call    cpu_startnext	/* we don't come back from this */
smp_halt:
2:
	jmp     2

.bss
	.balign	16
	.space	8 * 512	/* enough? */
stk:
