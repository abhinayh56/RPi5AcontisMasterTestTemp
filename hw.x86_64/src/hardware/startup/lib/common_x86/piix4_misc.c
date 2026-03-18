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
#include "piix4_chipset.h"
#include <x86/pci_misc.h>

/*
 * piix4_pirq_remap_adjust
 *
 * This routine will undo any PIRQ re-mapping done by the BIOS so that the full
 * number of IOAPIC interrupts are usable (normally 16 thru 19).
 *
*/
void piix4_pirq_remap_adjust(void)
{
	static const uint8_t pirq_reg_offset[] =
	{
		0x60, 0x61, 0x62, 0x63,		// PIRQ[A..D]
	};
	unsigned i;

	if (debug_flag > 1) kprintf("PIIX4 PIRQ route register layout\n");
	/* for all PIRQ's */
	for (i=0; i<NUM_ELTS(pirq_reg_offset); i++)
	{
		uint8_t pirq = GET_LPC_REG8(pirq_reg_offset[i]);
		if (debug_flag > 1) {const char s[] = {'A' + i, '\0'}; kprintf("PIRQ%s: %x\n", s, pirq);}

		if ((pirq & 0x80) == 0)
		{
			/* leave the re-map IRQ in the lower bits so we know what it was */
			SET_LPC_REG8(pirq_reg_offset[i], pirq | 0x80);
		}
	}
}
