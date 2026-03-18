/*
 * $QNXLicenseC:
 * Copyright 2007, 2008, QNX Software Systems. 
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
//
patch_display:
	add		%rdx, %rdi		// get rtn base pointer
	push	%rdi
	mov		$0x1000,%rdi			// get bios data area mapped
	mov		$0x0,%rsi
	mov		$(PROT_READ|PROT_WRITE),%rdx
	call	callout_memory_map
	mov		0(%rsp),%rdi
	mov		%rax,2(%rdi)
	mov		$0xa000,%rdi			// get screen data area mapped
	mov		$0xb0000,%rsi
	mov		$(PROT_READ|PROT_WRITE),%rdx
	call	callout_memory_map
	pop		%rdi
	mov		%rax,2+10(%rdi)
	ret
/*
;
; void display_char(struct syspage_entry *sysp, char c)
;
; Display debug messages from kdebug or the kernel.
;
*/
CALLOUT_START(display_char_vga, 0, patch_display)
	movabs	$0x1234567812345678,%r9		// bios data area
	movabs	$0x1234567812345678,%r10	// screen data area

#define BCOLS		0x44a
#define BROW		0x451
#define BCOL		0x450
	mov %esi, %ecx

    xor       %rdx,%rdx
    mov       BROW(%r9),%dl
	mov       %dl,%dh
    mov       BCOL(%r9),%dl
    mov       %cl,%al
    cmp       $0x0d,%al				// carriage return
    jne       X$1
    xor       %dl,%dl
    jmp       X$5
X$1:
    cmp       $0x0a,%al				// line feed
    jne       X$3
X$2:
    xor       %dl,%dl
    inc       %dh
    cmp       $0x18,%dh
    jle       X$5
    dec       %dh
    mov       %r10,%rdi
    movzbq    BCOLS(%r9),%rsi
    shl       $0x00000001,%rsi
    add       %rdi,%rsi
    xor       %eax,%eax
    mov       BCOLS(%r9),%al
    mov       $25,%ah
    mul       %ah
    mov       %eax,%ecx
    push      %rsi
    push      %rcx
    repe; movsw 				// scroll B/W buffer
    mov       $0x0720,%ax
    mov       BCOLS(%r9),%cl
    repe; stosw 				// blank bottom line of B/w buffer
    lea       0x8000(%r10),%rdi
    pop       %rcx
    pop       %rsi
    add       $0x8000,%rsi
    repe; movsw 				// scroll colour buffer
    mov       $0x0720,%ax
    mov       BCOLS(%r9),%cl
    repe; stosw 				// blank bottom line of colour buffer
    jmp       X$5
X$3:
    cmp       $0x0c,%al				// home
    jne       X$4
    xor       %rdx,%rdx
    jmp       X$5
X$4:
    push      %rax
    push      %rcx
    xor       %rax,%rax
    mov       BCOLS(%r9),%al
    mul       %dh
    shl       $0x00000001,%rax
    xor       %rcx,%rcx
    mov       %dl,%cl
    shl       $0x00000001,%rcx
    add       %ecx,%eax
    mov	      %r10,%rdi
    add       %rax,%rdi
    pop       %rcx
    pop       %rax
    mov       $0x07,%ah
    mov       %ax,(%rdi)
    mov       %ax,0x8000(%rdi)
    inc       %dl
    cmp       BCOLS(%r9),%dl
    jge       X$2
X$5:
    mov       %dl,BCOL(%r9)
	mov       %dh,%dl
    mov       %dl,BROW(%r9)
	xor       %rax,%rax
    mov       BCOLS(%r9),%al
    mul       %dh
	xor		  %rcx,%rcx
	mov 	  BCOL(%r9),%cl
	add		  %rax,%rcx
    mov       $0x3d4,%dx
	mov		  $14,%al
    out       %al,%dx
    inc       %dx
    mov       %ch,%al
    out       %al,%dx
	dec		  %dx
	mov		  $15,%al
    out       %al,%dx
    inc       %dx
    mov       %cl,%al
    out       %al,%dx
    ret       
CALLOUT_END(display_char_vga)
	
/*
;
; int poll_key( volatile struct syspage_entry *)
;
; return a key value, or -1 if no key is available (for kdebug use)
;
*/
CALLOUT_START(poll_key_pckbd, 0, 0)
	xorl	  %eax,%eax
	inb  	  $0x64,%al				// key code available?
    testb     $0x01,%al
    je        X$6
    inb       $0x60,%al
    xorb      $0x80,%al
    andl      $0x00000080,%eax		// ignore key-up codes
    je        X$6
    orl       $0x00000100,%eax		// indicate non-ascii character
    ret       
X$6:
    movl      $-1,%eax
    ret       
CALLOUT_END(poll_key_pckbd)
	
/*
;
; int break_detect( volatile struct syspage_entry *)
;
; return a 1 if a break condition is detected, 0 otherwise
;
*/
CALLOUT_START(break_detect_pckbd, 0, 0)
	xorl	%eax,%eax
	ret
CALLOUT_END(break_detect_pckbd)
