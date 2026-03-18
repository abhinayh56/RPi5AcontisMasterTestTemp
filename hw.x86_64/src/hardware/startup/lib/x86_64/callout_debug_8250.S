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
; This is a kernel hardware callout.
; It may be called from within an interrupt handler.
; This code MUST be position independant.
;
*/

#include <x86_64/callout.ah>

.globl patch_debug_8250
patch_debug_8250:
	add		%rdx, %rdi
	mov		DDI_BASE(%r8), %rcx
	mov		%rcx, sizeof_jmp+0(%rdi)
	movl	DDI_SHIFT(%r8), %ecx
	mov		%ecx, sizeof_jmp+8(%rdi)
	ret


#define PATCHED_PROLOGUE(x) \
	PROLOGUE(x); \
	mov x##_arg0(%rip), %rdx; \
	mov x##_arg1(%rip), %ecx; \
	mov $REG_LS, %eax; \
	shl %cl, %eax; \
	mov %eax, %ecx; \


/*
;
; void display_char(struct syspage_entry *sysp, char c)
;
; Display debug messages from kdebug or the kernel.
;
*/



		
CALLOUT_START(display_char_8250, 0, patch_debug_8250)
	PATCHED_PROLOGUE(disp)

	add   	%ecx,%edx
    pushfq   
    cli       
X$1:
    inb    %dx,%al
    testb  $LSR_TXRDY,%al
    je     X$1
    sub    %ecx,%edx
    mov    %esi,%eax		// character to output
    outb   %al,%dx
    popfq    
    ret       
CALLOUT_END(display_char_8250)
	
/*	
;
; int poll_key( volatile struct syspage_entry *)
;
; return a key value, or -1 if no key is available (for kdebug use)
;
*/

CALLOUT_START(poll_key_8250, 0, patch_debug_8250)
	PATCHED_PROLOGUE(poll)

	xor	      %eax,%eax
    addl      %ecx,%edx
    inb       %dx,%al
    testb     $LSR_RXRDY,%al
    je        X$2
    subl      %ecx,%edx
    inb       %dx,%al
    ret       
X$2:
    movl      $-1,%eax
    ret       
CALLOUT_END(poll_key_8250)
	

/*
;
; int break_detect( volatile struct syspage_entry *)
;
; return non-zero if a break condition is detected, 0 otherwise
;
*/
CALLOUT_START(break_detect_8250, 0, patch_debug_8250)
	PATCHED_PROLOGUE(brk)

	xor       %eax,%eax
    addl      %ecx,%edx
    inb       %dx,%al
    andb      $LSR_BI,%al
    je        X$3
    subl      %ecx,%edx
    inb       %dx,%al				// eat dummy character
    movb      $0x01,%al
X$3:
    ret       
CALLOUT_END(break_detect_8250)
