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
#include "sch_chipset.h"
#include <x86/pci_misc.h>

/*
 * sch_is_gpio
 *
 * Return true (!0) or false (0) depending on whether <pin> is configured as a
 * GPIO
*/
unsigned sch_is_gpio(const unsigned pin)
{
	volatile uint8_t gpio_enabled = GET_LPC_REG8(0x44) & (1U << 31);	// check bit 31
	if (debug_flag > 1) kprintf("checking GPIO pin %u .. ", pin);
	if (gpio_enabled)
	{
		volatile const uint32_t gpio_base = GET_LPC_REG32(0x44) & 0x0000FFF0U;
		/* GPIO's 0-9 are in core well (@ 0x0), CPIO's 10 - 13 are in resume well (@ 0x20) */
		volatile const uint32_t gpio_pin_use = in32(gpio_base + 0) | (in32(gpio_base + 0x20) << 10);

		if (debug_flag > 1) kprintf("%s\n", (gpio_pin_use & (1U << pin)) ? "yes" : "no");
		return (gpio_pin_use & (1U << pin));
	}
	if (debug_flag > 1) kprintf("no\n");	// GPIO's are not enabled
	return 0;	// <pin> is not configured as a gpio
}

/*
 * sch_pirq_remap_adjust
 *
 * This routine will undo any PIRQ re-mapping done by the BIOS so that the full
 * number of IOAPIC interrupts are usable (normally 16 thru 23).
 * Note that the SCH does not share PIRQ's with GPIO's as with the ICH's
*/
void sch_pirq_remap_adjust(void)
{
	static const uint8_t pirq_reg_offset[] =
	{
		0x60, 0x61, 0x62, 0x63,		// PIRQ[A..D]
		0x64, 0x65, 0x66, 0x67,		// PIRQ[E..H]
	};
	unsigned i;

	if (debug_flag > 1) kprintf("SCH PIRQ route register layout\n");
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
