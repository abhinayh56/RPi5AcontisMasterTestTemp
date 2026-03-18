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
.text

// syspage_paddr, vaddr, rtn_off, rw_off, data, rtn
// RDI            RSI    RDX      RCX     R8    R9 
// the patch data is a pointer to a 'apic_patch_data_t'
// struct {
//		void *lapic_vaddr;
//		void *ioapic_vaddr;
//		uint32_t ioapic_cpu_intr_base;
// };
patch_lapic2:
	add		%rdx, %rdi	/* syspage_paddr+rtn_off */
	mov     0(%r8), %r8
	mov     %r8, sizeof_jmp+0(%rdi)
	add		%rcx, %rsi	/* rtn_off+rw_off*/
	mov     %rsi, sizeof_jmp+8(%rdi)
	ret

patch_lapic1:
	add		%rdx, %rdi	/* syspage_paddr+rtn_off */
	mov     0(%r8), %r8
	mov		%r8, sizeof_jmp+0(%rdi)
	ret

mask_data:
	.long	4


#define LVT_SPACING_SHIFT	4

/*
; do nothing stub which returns 0. Currently used for MSI mask/unmask/config
; callouts since we can't leave them NULL
*/
CALLOUT_START(interrupt_lapic_stub, 0, 0)
	xorl	%eax,%eax
	ret
CALLOUT_END(interrupt_lapic_stub)

/*
; 
; int interrupt_id()
;
; Returns: 0 - it's not a glitch   -1 - it's a glitch
;
*/
# next 2 lines are for backwards compatibility since the 'apic' name was public
.global interrupt_id_apic
interrupt_id_apic:
CALLOUT_START(interrupt_id_lapic, 0, 0)
	//Nothing to do
CALLOUT_END(interrupt_id_lapic)

/*
; 
; void interrupt_eoi
;
; Send an end-of-interrupt to the PIC.
;
*/
# next 2 lines are for backwards compatibility since the 'apic' name was public
.global interrupt_eoi_apic
interrupt_eoi_apic:
CALLOUT_START(interrupt_eoi_lapic, 0, patch_lapic1)
	PROLOGUE(int_eoi);
	mov 	int_eoi_arg0(%rip),%rdi		// get APIC base vaddr (patched)
	movl	$0,LAPIC_EOI(%rdi)		// Intel wants a zero written
CALLOUT_END(interrupt_eoi_lapic)

/*
;
; int interrupt_unmask(volatile struct syspage_entry *syspage, int intr)
;
; Unmask the passed hardware interrupt number. 
;
*/
# next 2 lines are for backwards compatibility since the 'apic' name was public
.global interrupt_unmask_apic
interrupt_unmask_apic:
CALLOUT_START(interrupt_unmask_lapic, mask_data, patch_lapic2)
	PROLOGUE(int_unmask)
	mov     int_unmask_arg0(%rip),%rdi		/* get APIC base vaddr (patched) */
	mov     int_unmask_arg1(%rip),%rax		/* get R/W mask vaddr (patched) */
	orl		%esi,%esi
	jl		1f
	movzx   %si, %rsi
	mov     %rsi, %rcx
	shl	$LVT_SPACING_SHIFT,%rsi
	andl	$~LVT_MASKED,LAPIC_LTR(%rdi,%rsi,1)
	mov 	$~1,%edx
	shl		%cl,%edx
	and 	%edx,0(%rax)
	jmp		2f
1:	
	// sync with mask
	mov 	0(%rax),%ecx
	mov 	$6,%eax
3:
	shr		$1,%ecx
	jc		4f
	andl	$~LVT_MASKED,LAPIC_LTR(%rdi)
	jmp		5f
4:	
	orl		$LVT_MASKED,LAPIC_LTR(%rdi)
5:
	add 	$(1<<LVT_SPACING_SHIFT),%rdi
	dec 	%eax
	jne		3b

2:
    xor     %eax,%eax
    ret       
CALLOUT_END(interrupt_unmask_lapic)

/*
;
; int interrupt_mask(volatile struct syspage_entry *syspage, int intr)
;
; Mask the passed hardware interrupt number.
;
*/
# next 2 lines are for backwards compatibility since the 'apic' name was public
.global interrupt_mask_apic
interrupt_mask_apic:
CALLOUT_START(interrupt_mask_lapic, mask_data, patch_lapic2)
	PROLOGUE(int_mask)
	mov    int_mask_arg0(%rip),%rdi		// get APIC base vaddr (patched)
	mov    int_mask_arg1(%rip),%rax		// get R/W mask vaddr (patched)
	or 		%esi,%esi
	jl		1f
	movzx   %si, %rsi
	mov     %rsi, %rcx     
	shl		$LVT_SPACING_SHIFT,%rsi
	orl		$LVT_MASKED,LAPIC_LTR(%rdi,%rsi,1)
	mov 	$1,%edx
	shl		%cl,%edx
	or 		%edx,0(%rax)
	jmp		2f
1:	
	// sync with mask
	mov 	0(%rax),%ecx
	mov 	$6,%eax
3:
	shr		$1,%ecx
	jc		4f
	andl	$~LVT_MASKED,LAPIC_LTR(%rdi)
	jmp		5f
4:	
	orl		$LVT_MASKED,LAPIC_LTR(%rdi)
5:
	add 	$(1<<LVT_SPACING_SHIFT),%rdi
	decl	%eax
	jne		3b

2:
    xor     %eax,%eax
    ret       
CALLOUT_END(interrupt_mask_lapic)

/*
;
; int interrupt_config(volatile struct syspage_entry *syspage,
                        struct intrinfo_entry *info, int intr)
;
; Return configuration information for interrupt
;
;
*/
# next 2 lines are for backwards compatibility since the 'apic' name was public
.global interrupt_config_apic
interrupt_config_apic:
CALLOUT_START(interrupt_config_lapic, 0, 0)
    xor     %eax,%eax
    ret
CALLOUT_END(interrupt_config_lapic)

/*
; 
; int interrupt_id()
;
; Returns: 0 - it's not a glitch   -1 - it's a glitch
;
*/
# next 2 lines are for backwards compatibility since the 'apic' name was public
.global interrupt_id_apic_spurious
interrupt_id_apic_spurious:
CALLOUT_START(interrupt_id_lapic_spurious, 0, 0)
	mov		  $-1,%eax		
CALLOUT_END(interrupt_id_lapic_spurious)

/*
; 
; void interrupt_eoi
;
; Send an end-of-interrupt to the PIC.
;
*/
# next 2 lines are for backwards compatibility since the 'apic' name was public
.global interrupt_eoi_apic_spurious
interrupt_eoi_apic_spurious:
CALLOUT_START(interrupt_eoi_lapic_spurious, 0, 0)
CALLOUT_END(interrupt_eoi_lapic_spurious)

/*
;
; int interrupt_unmask(volatile struct syspage_entry *syspage, int intr)
;
; Unmask the passed hardware interrupt number. 
;
*/
# next 2 lines are for backwards compatibility since the 'apic' name was public
.global interrupt_unmask_apic_spurious
interrupt_unmask_apic_spurious:
CALLOUT_START(interrupt_unmask_lapic_spurious, 0, 0)
    xorl      %eax,%eax
    ret       
CALLOUT_END(interrupt_unmask_lapic_spurious)

/*
;
; int interrupt_mask(volatile struct syspage_entry *syspage, int intr)
;
; Mask the passed hardware interrupt number.
;
*/
# next 2 lines are for backwards compatibility since the 'apic' name was public
.global interrupt_mask_apic_spurious
interrupt_mask_apic_spurious:
CALLOUT_START(interrupt_mask_lapic_spurious, 0, 0)
	mov		  $22,%eax		// return EINVAL
    ret       
CALLOUT_END(interrupt_mask_lapic_spurious)

/*
;
; int interrupt_config(volatile struct syspage_entry *syspage,
                        struct intrinfo_entry *info, int intr)
;
; Return configuration information for interrupt
;
*/
# next 2 lines are for backwards compatibility since the 'apic' name was public
.global interrupt_config_apic_spurious
interrupt_config_apic_spurious:
CALLOUT_START(interrupt_config_lapic_spurious, 0, 0)
    mov     $INTR_CONFIG_FLAG_DISALLOWED,%eax
    ret
CALLOUT_END(interrupt_config_lapic_spurious)
