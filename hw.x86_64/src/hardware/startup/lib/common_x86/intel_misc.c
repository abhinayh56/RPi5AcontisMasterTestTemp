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
#include <x86/intel_misc.h>



/*
 * return true (!0) or false (0) based on whether we are running on a board with
 * an Intel chipset.
 *
 * For the purposes of configuring interrupts, we have an Intel chipset if we
 * can
 * 		- locate a PCI/ISA bridge device (aka LPC on new chipsets)
 * 		- the bridge device has a vendor id = 0x8086
 */
unsigned is_intel_chipset(void)
{
	int devfunc = get_LPC_pci_devfunc();
	uint16_t vendor_id;

	if (devfunc < 0) return 0;	// false
	vendor_id = pci_read_cfg16(0, PCI_DEVNO(devfunc), PCI_FUNCNO(devfunc), 0);

	return (vendor_id == 0x8086);
}
