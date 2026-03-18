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
#include "amd_misc.h"


/*
 * return true (!0) or false (0) based on whether we are running on a board with
 * an AMD chipset.
 *
 * For the purposes of configuring interrupts, we have an AMD chipset if we
 * can
 * 		- locate a PCI/ISA bridge device (aka LPC on new chipsets)
 * 		- the bridge device has a vendor id = 0x1002
 */
unsigned is_amd_chipset(void)
{
	int devfunc = get_LPC_pci_devfunc();
	uint16_t vendor_id;
	uint16_t device_id;

	if (debug_flag > 2) kprintf("amd LPC devfunc = %x\n", devfunc);
	
	if (devfunc < 0) return 0;	// false
	vendor_id = pci_read_cfg16(0, PCI_DEVNO(devfunc), PCI_FUNCNO(devfunc), 0);
	device_id = pci_read_cfg16(0, PCI_DEVNO(devfunc), PCI_FUNCNO(devfunc), 2);

	if (debug_flag > 2) kprintf("vendor id = %x\n", vendor_id);

	/* the INT[A..H] config and routing registers are not in the LPC device on
	 * AMD sb600/sb700 chipsets, they are in the SMBus module and ACPI block so
	 * we override the LPC devfunc with the PCI device for that so that the LPC
	 * get/set functions continue to work */
	if ((vendor_id == 0x1002) && ((device_id & 0xfff0) != 0x7800))
	{
		unsigned tmp_devfunc;
		if (pci_find_class(0, 0xc, 0x5, NULL, &tmp_devfunc) == -1) return 0;	// false
		set_LPC_pci_devfunc(tmp_devfunc);
	}

	return (vendor_id == 0x1002) || (vendor_id == 0x1022);
}

/*
 * return true (!0) or false (0) based on whether we are running on a board with
 * an AMD Ryzen processor
 */
unsigned is_amd_ryzen(void)
{
	static int ret = -1;

	/* do the check once */
	if (ret < 0)
	{
		ret = is_amd_chipset();
		if (ret != 0)
		{
			const uint32_t family = cpu_family();
			ret = (family >= 0x17);
		}
	}
	ASSERT(ret >= 0);
	return (unsigned)ret;
}
