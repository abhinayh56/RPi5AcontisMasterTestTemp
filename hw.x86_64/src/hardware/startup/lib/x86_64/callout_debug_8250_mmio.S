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

// syspage_paddr, vaddr, rtn_off, rw_off, data, rtn
// RDI            RSI    RDX      RCX     R8    R9

.globl patch_debug_8250_mmio
patch_debug_8250_mmio:
	add		%rdx, %rdi		// get rtn base pointer
	push	%rdi
    // patch base
	mov		DDI_BASE(%r8), %rsi
	mov		$0x20, %rdi		// register size
//	mov		$(PROT_READ|PROT_WRITE|PROT_NOCACHE), %rdx
	mov		$0xb00, %rdx
	call	callout_memory_map
	mov		0(%rsp),%rdi
	mov		%rax,2(%rdi)
// FIX ME
// Patch shift

	pop		%rdi
	ret


/*
;
; void display_char(struct syspage_entry *sysp, char c)
;
; Display debug messages from kdebug or the kernel.
;
*/
CALLOUT_START(display_char_8250_mmio, 0, patch_debug_8250_mmio)
	movabs	$0x1234567812345678,%r9		// base address, patched
    pushfq   
    cli
X$1:
    movb    20(%r9),%al
    testb   $LSR_TXRDY,%al
    je      X$1
    movb    %sil,0(%r9)		// character to output
    popfq    
    ret       
CALLOUT_END(display_char_8250_mmio)
	
/*	
;
; int poll_key( volatile struct syspage_entry *)
;
; return a key value, or -1 if no key is available (for kdebug use)
;
*/
CALLOUT_START(poll_key_8250_mmio, 0, patch_debug_8250_mmio)
	movabs	$0x1234567812345678,%r9		// base address, patched

    movb    20(%r9),%al
    testb   $LSR_RXRDY,%al
    je      X$2
    movzbl  0(%r9),%eax
    ret       
X$2:
    movl    $-1,%eax
    ret       
CALLOUT_END(poll_key_8250_mmio)
	

/*
;
; int break_detect( volatile struct syspage_entry *)
;
; return non-zero if a break condition is detected, 0 otherwise
;
*/
CALLOUT_START(break_detect_8250_mmio, 0, patch_debug_8250_mmio)
	movabs	$0x1234567812345678,%r9		// base address, patched

    movb    20(%r9),%al
    andl    $LSR_BI,%eax
    je      X$3
    movb    0(%r9),%dl         // eat dummy character
X$3:
    ret       
CALLOUT_END(break_detect_8250_mmio)
