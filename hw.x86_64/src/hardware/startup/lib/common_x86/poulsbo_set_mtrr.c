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

#include <startup.h>
#include "poulsbo_chipset.h"

/**
 * See section 11.11 of Intel® 64 and IA-32 Architectures Software Developer’s
 * Manual Vol 3A - 317080.pdf
 */
void poulsbo_set_mtrr(uint32_t reg, paddr32_t base, uint32_t size, uint32_t type)
{
    uint64_t    val;
    uint64_t    old_deftype;
    uint32_t    cr0val;
    uint32_t    old_cr0;
    uint32_t    old_cr4;

    old_cr0 = rdcr0();
    old_cr4 = rdcr4();

    cr0val = old_cr0 | 0x40000000;	// set CD
    cr0val &= ~0x20000000;	// clear NW
    ldcr0(cr0val);
    flushcache();

	ldcr4(old_cr4 & ~0x80);		// clear PGE

    old_deftype = rdmsr(defType_MSR);
    wrmsr(defType_MSR, old_deftype & 0xf300);

    val = base | type;
    wrmsr(physBase_MSR(reg), val);
    val = size ? (~(size - 1) | 0x800) : 0;
    wrmsr(physMask_MSR(reg), val);

    wrmsr(defType_MSR, old_deftype);
    flushcache();

    ldcr0(old_cr0);
    ldcr4(old_cr4);
}
