/*
 * $QNXLicenseC:
 * Copyright 2023 BlackBerry Limited.
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

#define CPUID_FAMILY 1
#ifndef X86_64_CR4_OSFXSR
#define X86_64_CR4_OSFXSR (1<<9)
#endif
#ifndef X86_64_FEATURES2_XSAVE
#define X86_64_FEATURES2_XSAVE (1<<26)
#endif

/**
 bool early_cpu_setup(void) -- configure processor for C runtime needs.
 
 The C compiler and intrinsics want to use the xmm registers.
 Support for the xmm registers is tested and enabled here.

 @return 1(true)  if xmm support enabled.
 @return 0(false) if xmm support not enabled.
*/

STARTFUNC(early_cpu_setup)
    push %rbx
    mov  $CPUID_FAMILY, %eax
    cpuid
    pop %rbx
    testl $X86_64_FEATURES2_XSAVE, %ecx
    jz noxsave
    mov %cr4, %rax
    orq $X86_64_CR4_OSFXSR, %rax
    mov %rax, %cr4
    mov $1, %eax
    ret
noxsave:
    xor %eax, %eax
    ret
ENDFUNC(early_cpu_setup)
