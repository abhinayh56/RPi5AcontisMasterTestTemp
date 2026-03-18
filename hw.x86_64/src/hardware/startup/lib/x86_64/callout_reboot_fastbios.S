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
;
; void reboot( volatile struct syspage_entry *, int abnormal )
;
; Reboot the machine
;
*/
CALLOUT_START(reboot_fastbios, 0, 0)
	test	  %esi, %esi
    jz        X$1
    xor       %rdx,%rdx
    movw      SYSPAGE_SYSTEM_PRIVATE(%rdi),%dx
    add       %rdx,%rdi
    testl     $SYSTEM_PRIVATE_FLAG_ABNORMAL_REBOOT,SP_PRIVATE_FLAGS(%rdi)
    je        X$2	// hang up if we don't want to do an abnormal reboot
	
X$1:

   	inb		  $0x92,%al
	and		  $0xfe,%al
   	outb	  %al,$0x92	// ensure reset bit is a zero
	mov		  %al,%bl
	inb		  $0x00,%al	// wait 2 microseconds
	inb		  $0x00,%al
	mov		  %bl,%al
	or		  $0x01,%al	// transition reset bit
   	outb	  %al,$0x92
	inb		  $0x00,%al	// wait 2 more microseconds
	inb		  $0x00,%al
	
X$2:

	cli
	// Some boards also look for a halt opcode.
    hlt       
    jmp       X$2
CALLOUT_END(reboot_fastbios)
