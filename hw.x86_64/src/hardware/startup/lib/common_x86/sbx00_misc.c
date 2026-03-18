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
#include "sbx00_chipset.h"
#include <x86/pci_misc.h>

/*
 * sbx00_pirq_remap_adjust
 *
 * This routine will undo any PIRQ re-mapping done by the BIOS so that the full
 * number of IOAPIC interrupts are usable (normally 16 thru 23). Only PIRQ's
 * which are not configured as GPIO's can be used although we unmap all of them
 * when using the IOAPIC as suggested in AMD's docs for the Pci_Intr_Data
 * register (@ 0xC00)
*/
void sbx00_pirq_remap_adjust(const unsigned did)
{
	static const uint8_t sb6_700_pirq_reg_offset[] =
	{
		0x0, 0x1, 0x2, 0x3,		// INT[A..D]
		0x9, 0xA, 0xB, 0xC,		// INT[E..H]
	};
	static const uint8_t sb800_pirq_reg_offset[] =
	{
		0x0, 0x1, 0x2, 0x3,		// INT[A..D]
		0x4, 0x5, 0x6, 0x7,		// INT[E..H]
	};
	const uint8_t * const pirq_reg_offset = (did == 0x4385) ? sb800_pirq_reg_offset : sb6_700_pirq_reg_offset;
	const unsigned pirq_table_size = (did == 0x4385) ? NUM_ELTS(sb800_pirq_reg_offset) : NUM_ELTS(sb6_700_pirq_reg_offset);
	const uint8_t pirq_val = (did == 0x4385) ? 0x80 : 0x0;
	unsigned i;

	if (debug_flag > 1) kprintf("SBx00 PIRQ route register layout\n");
	/* for all PIRQ's */

	for (i=0; i<pirq_table_size; i++)
	{
		out8(0xC00, pirq_reg_offset[i]);
		if (debug_flag > 1) {
			volatile const uint8_t pirq = in8(0xC01);
			const char s[] = {'A' + i, '\0'};
			kprintf("INT%s: %x, new pirq_val: 0x%x\n", s, pirq, pirq_val);
		}
		out8(0xC01, pirq_val);
	}
}
