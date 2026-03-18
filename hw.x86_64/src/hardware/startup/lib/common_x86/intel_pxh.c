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
#include "intel_pxh.h"



/*
 * find_pxh_ioapic
 *
 * Given the <ioapic_id>, determine whether or not it is an IOAPIC residing
 * within a PXH PCIe-PCI bridge
 *
 * In the PXH, the IO-APIC's appear in the PCI config space and so we can search
 * for their device ID's 0x0326 and 0x0327. Once found, we can determine whether
 * or not <ioapic_id> is in fact a PXH IO-APIC by comparing the physical address
 * for <ioapic_id> with the MBAR/ABAR configuration registers for the found
 * IO-APIC device ID's.
 *
 * Returns: true (!0) or false (0)
*/
unsigned find_pxh_ioapic(unsigned ioapic_id)
{
	const PADDR_T acpi_paddr = ioapic_get_paddr(ioapic_id);
	unsigned devfunc;
	int bus = -1;
	int cookie = 0;

	/* find all "System Peripheral" class devices (c/sc = 8/0) */
	while ((cookie = pci_find_class(cookie, 8, 0, &bus, &devfunc)) != -1)
	{
		/* found, make sure its a PXH IOAPIC */
		const uint16_t did = pci_read_cfg16(bus, PCI_DEVNO(devfunc), PCI_FUNCNO(devfunc), 0x2);
		if ((did == 0x0326) || (did == 0x0327))
		{
			/*
			 * found a PXH IOAPIC. Does it correspond to <ioapic_id> ?
			 * Since it is now permissible to relocate IO-APIC's anywhere
			 * within the 0-4GB address space, we check both the MBAR and the
			 * ABAR for a match with the ACPI established value
			*/
			const uint32_t mbar_paddr = pci_read_cfg32(bus, PCI_DEVNO(devfunc), PCI_FUNCNO(devfunc), 0x10);
			const uint16_t abar = pci_read_cfg16(bus, PCI_DEVNO(devfunc), PCI_FUNCNO(devfunc), 0x40);
			const uint32_t abar_paddr = (abar & (1u << 15)) ? 0xFEC00000u | ((abar & 0xFFFu) << 8) : 0;

			if ((acpi_paddr == mbar_paddr) || (acpi_paddr == abar_paddr)) return 1;
		}
		bus = -1;	// reset for next pass through loop
	}
	return 0;
}
