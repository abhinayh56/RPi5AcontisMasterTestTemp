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
 * See section 10.11 of Intel® 64 and IA-32 Architectures Software Developer’s
 * Manual - 317080.pdf
 */
void poulsbo_get_mtrr(uint32_t reg, paddr32_t *base, uint32_t *size, uint32_t *type)
{
    uint64_t    val;

    val = rdmsr(physMask_MSR(reg));
    if ((val & 0x800) == 0) {
        /* MTRR is disabled, so it is free */
        *base = 0;
        *size = 0;
        *type = 0;
        return;
    }

    *size = ~((val & 0xfffff000) - 1);

    val = rdmsr(physBase_MSR(reg));
    *base = (val & 0xFFFFF000UL);
    *type = (val & 0xFF);
}
