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

#include <startup.h>
#include <x86/pci_misc.h>



void pcu_ilb_pirq_remap_adjust(void)
{
	unsigned i;
	const uint32_t ilb_base_reg = GET_LPC_REG32(0x50);
	const PADDR_T ilb_base = (ilb_base_reg & ~((1u << 9) - 1)) + 8;	// PIRQ registers start at offset 8
	volatile uint8_t *ilb_base_p = startup_memory_map(8, ilb_base, PROT_NOCACHE | PROT_READ | PROT_WRITE);

//kprintf("ilb_base_reg: %x, ilb_base: %P, ilb_base_p: %v\n", ilb_base_reg, ilb_base, ilb_base_p);
	if (debug_flag > 1) kprintf("PCU ILB PIRQ route register layout\n");
	/* for all PIRQ's */
	for (i=0; i<8; i++)
	{
		if (debug_flag > 1) {const char s[] = {'A' + i, '\0'}; kprintf("PIRQ%s: %x\n", s, ilb_base_p[i]);}

		if ((ilb_base_p[i] & 0x80) == 0)
		{
			/* leave the re-map IRQ in the lower bits so we know what it was */
			ilb_base_p[i] |= 0x80;
		}
//kprintf("ilb_base_p[%u] @ 0x%x = 0x%x\n", i, &ilb_base_p[i], ilb_base_p[i]);
	}
	startup_memory_unmap((void *)ilb_base_p);
}
