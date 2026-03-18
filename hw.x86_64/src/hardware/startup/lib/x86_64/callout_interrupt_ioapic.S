/*
 * $QNXLicenseC:
 * Copyright 2009 QNX Software Systems. 
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
 * IOAPIC interrupts callouts
 *
 * This file handles 1 IOAPIC. You will need multiple callout entries for
 * multiple IOAPIC's or a change to these callouts
*/

#include <x86_64/callout.ah>

/*
 * Interrupts are always off in this code.
 *
 * Note that interrupt_id_* and interrupt_eoi_* are not actually
 * called by the kernel. For performance reasons, they are instead
 * copied into place and intermixed with other kernel code. As such,
 * they don't follow normal calling conventions. They should just fall out 
 * their bottoms when done rather than attempting to do a return 
 * instruction. The 'id' routine should return the interrupt level in
 * the RAX register. 
 * By turning on various INTR_GENFLAG_* bits in the intrinfo_entry,
 * the following values can be loaded before entry to this code.
 * 
 *		syspage_ptr			=> RBX
 *		intrinfo_entry		=> RCX
 *		cpu number 			=> RSI
 *		intr level number	=> RAX (only available in EOI routine)
 *		intr mask count		=> RDX (only available in EOI routine)
 *
 * If the INTR_FLAG_CPU_FAULT bit is on, the eoi routine should set
 * the RAX register to the following value to indicate the exception
 * that has occurred:
 * 		RAX = (fault_number << 16) | (signal_code << 8) | (signal_number)
 * Set RAX to zero if no exception has occured. If the exception is memory
 * related, set EDX to referenced address that caused the exception, zero
 * if there is no address.
 */

// syspage_paddr, vaddr, rtn_off, rw_off, data, rtn
// RDI            RSI    RDX      RCX     R8    R9
//
// the patch data is a pointer to a
// struct {
//		void *lapic_vaddr;
//		void *ioapic_vaddr;
//		uint32_t ioapic_cpu_intr_base;
// };
// which should, of course, be defined in an interface, and co-ordinated
// with mkasmoff.
// patches LAPIC_VADDR,IOAPIC_VADDR and ioapic_cpu_intr_base into eoi callout 
#define LAPIC_VADDR  0
#define IOAPIC_VADDR 8
#define INTR_BASE    16
patch_ioapic:
	add	%rdx, %rdi
	mov	LAPIC_VADDR(%r8), %rax
	mov	%rax, sizeof_jmp+LAPIC_VADDR(%rdi)
	mov	IOAPIC_VADDR(%r8), %rax
	mov	%rax, sizeof_jmp+IOAPIC_VADDR(%rdi)
	mov	INTR_BASE(%r8), %eax
	mov	%eax, sizeof_jmp+INTR_BASE(%rdi)
	ret


#define REDIR_SPACING_SHIFT	1
#define IOAPIC_REDIR	0x10
#define IOAPIC_EOI		0x40


/*
; 
; int interrupt_id()
;
; Returns: 0 - it's not a glitch   -1 - it's a glitch
;
*/
CALLOUT_START(interrupt_id_ioapic, 0, 0)
	//Nothing to do
CALLOUT_END(interrupt_id_ioapic)

/*
; 
; void interrupt_eoi
;
; Send an end-of-interrupt to the PIC.
;
*/
CALLOUT_START(interrupt_eoi_ioapic, 0, patch_ioapic)
	PROLOGUE(eoi)

	mov 	eoi_argbase+IOAPIC_VADDR(%rip),%rdx		// get IOAPIC base vaddr (patched)
	addl	eoi_argbase+INTR_BASE(%rip), %eax		// patched with ioapic_cpu_intr_base
	mov 	%eax,IOAPIC_EOI(%rdx)	// IOAPIC wants the vector written

	mov 	eoi_argbase+LAPIC_VADDR(%rip),%rdx		// get LAPIC base vaddr (patched)
	movl	$0,LAPIC_EOI(%rdx)		// LAPIC wants a zero written
CALLOUT_END(interrupt_eoi_ioapic)

/*
;
; int interrupt_unmask(volatile struct syspage_entry *syspage, int intr)
;
; Unmask the passed hardware interrupt number. 
;
*/
CALLOUT_START(interrupt_unmask_ioapic, 0, patch_ioapic)
	PROLOGUE(unmask)

	mov 	unmask_argbase+IOAPIC_VADDR(%rip),%rdx		// get IOAPIC base vaddr (patched)

	# get the entry number	
	shl		$REDIR_SPACING_SHIFT,%esi
	add 	$0x10, %esi

	# write it to the IDX register of the IOAPIC
	mov 	%esi,(%rdx)

	# read out the LSW of the entry, mask it and write it back
	movl	0x10(%rdx),%eax
	andl	$~0x10000,%eax
	movl	%eax,0x10(%rdx)

	xorl	%eax,%eax
	ret
CALLOUT_END(interrupt_unmask_ioapic)

/*
;
; int interrupt_mask(volatile struct syspage_entry *syspage, int intr)
;
; Mask the passed hardware interrupt number.
;
*/
CALLOUT_START(interrupt_mask_ioapic, 0, patch_ioapic)
	PROLOGUE(mask)
	mov	mask_argbase+IOAPIC_VADDR(%rip),%rdx		// get IOAPIC base vaddr (patched)

	# get the entry number	
	shl		$REDIR_SPACING_SHIFT,%esi
	addl	$0x10, %esi

	# write it to the IDX register of the IOAPIC
	movl	%esi,(%rdx)
	
	# read out the LSW of the entry, mask it and write it back
	movl	0x10(%rdx),%eax
	orl		$0x10000,%eax
	movl	%eax,0x10(%rdx)

	xorl	%eax,%eax
	ret
CALLOUT_END(interrupt_mask_ioapic)

/*
;
; int interrupt_config(volatile struct syspage_entry *syspage,
                        struct intrinfo_entry *info, int intr)
;
; Return configuration information for interrupt
;
;
*/
CALLOUT_START(interrupt_config_ioapic, 0, 0)
	xor		%eax,%eax
	ret
CALLOUT_END(interrupt_config_ioapic)
