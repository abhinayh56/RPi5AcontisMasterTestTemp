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
;
; This is code to cycle an AP into the kernel startup code for an SMP system.
; This code MUST be position independant.
;
*/

#include <x86_64/callout.ah>
		
/*
;
; RDI has a pointer to the kernel syspage pointer
; RSI has the cpu number
;
; As the kernel finishes initializing each processor, it will store the
; starting address of the AP initializing code into 'start_addr' 
; in the smp structure. Since XCHG is always an atomic operation, one
; and only one of the AP's whirling around in the loop here will pick
; up the address and jump to it.
;
*/
CALLOUT_START(smp_spin, 0, 0)
	xor     %rax,%rax
	xor     %rdx,%rdx
	movw    SYSPAGE_SMP(%rdi),%dx
	add     %rdx,%rdi
	mov 	%eax,SMP_PENDING(%rdi) // Tell startup next AP can go
1:
	xchg    SMP_START_ADDR(%rdi),%rax
	or      %rax,%rax
	je	1b
	cmp     $-1,%rax
	je      shutdown
    // Pass the CPU number to the kernel in RDI.
	mov     %rsi, %rdi
	jmp     *%rax
	
shutdown:		// kernel doesn't want us :-(, sniff, sniff.
    mov       %rax,SMP_START_ADDR(%rdi)	// shutdown next AP
    hlt       
    jmp       shutdown
CALLOUT_END(smp_spin)
