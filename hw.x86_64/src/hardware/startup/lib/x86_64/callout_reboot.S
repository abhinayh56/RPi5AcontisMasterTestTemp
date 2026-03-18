/*
 * $QNXLicenseC:
 * Copyright 2010, QNX Software Systems. 
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
 *
 * This is a kernel hardware callout.
 * It may be called from within an interrupt handler.
 * This code MUST be position independant.
 *
 */

#include <x86_64/callout.ah>

/*
 *
 * reboot callout patcher
 * Input argument:
 * syspage_paddr, vaddr, rtn_off, rw_off, data, rtn
 *      %rdi      %rsi    %rdx     %rcx   %r8   %r9
 *
 * Data structure to be patched
 * typedef struct
 * {
 *        uint32_t reg_width;	// the width of the register (1 byte, 2 bytes, 4 bytes) 
 *        uint32_t value;	// the value to reset the board 
 *        paddr_t  reg_addr;	// the physical address of reset register 
 * } reboot_patch_args_t;
 *
 * Reboot procedure:
 * 1. First try output the reset value to the reset register. If the board 
 *    support ACPI reset, then the reset register and reset value are obtained 
 *    from ACPI table. Otherwise, use the default 0xCF9 port and reset value 
 *    0xF;
 * 2. If 1 doesn't work, then try keyboard reset;
 * 3. If 1, 2 don't work, then try triple-fault reset;
 * 4. If 1, 2, 3 don't work, then enter an infinite spin.
 *
 */

patch_reboot:
    addq    %rdi,%rdx		// routine paddr in %rdx

    movq    8(%r8),%rax		// get the reset register address
    movq    %rax,(patch1-startlabel+2)(%rdx) // patch address

    movb    4(%r8),%al		// get the reset value
    movb    %al,(patch2-startlabel+1)(%edx)  // patch value

    ret       


/*
 *
 * void reboot( volatile struct syspage_entry *, int abnormal )
 *
 * Reboot the machine
 *
 */
CALLOUT_START(reboot, 0, patch_reboot)
startlabel:
    cmpq      $0x0,%rsi
    je        shutdown

    // to test if need to hangup
    xorq      %rdx,%rdx
    movw      SYSPAGE_SYSTEM_PRIVATE(%rdi),%dx
    addq      %rdi,%rdx
    testl     $SYSTEM_PRIVATE_FLAG_ABNORMAL_REBOOT,SP_PRIVATE_FLAGS(%rdx)
    je        spin		// halt if we don't want to do an abnormal reboot

shutdown:
patch1:
    movq      $0x1234567812345678, %rdx	// reset register (patched)
patch2:
    movb      $0x56,%al					// reset value (patch)

    outb      %al,%dx		// set reset value on reset register

    // we expect a reset here. if failed, go next step  

    // fall through to keyboard reset if the above doesn't work
kbdreset:
    // wait for keyboard controller ready
    inb       $0x64,%al
    testb     $0x02,%al
    jne       kbdreset

    // send keyboard shutdown command
    movb      $0xfe,%al
    outb      %al,$0x64

    // we expect a reset here. if failed, go next step

    // try triple-fault reset as the last way if the all the above don't work
triplefault:
    sub       $16,%rsp
    movq      $0,0(%rsp)	// setup empty idt table
    movq      $0,8(%rsp)
    lidt      0(%rsp)		// load idt table
    int3				// issue interrupt

    // we expect a reset here. if failed go next step  

spin:
    cli

    // Some boards also look for a halt opcode.
    hlt       
    jmp       spin

CALLOUT_END(reboot)
