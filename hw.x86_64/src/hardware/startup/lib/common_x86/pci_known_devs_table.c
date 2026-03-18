/*
 * Copyright 2010,2025, BlackBerry Limited.
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

/*
 * table of known devices that can be searched for by class/subclass. This table
 * exists because we don't enumerate the PCI bus in startup. This table lets
 * specific devices be found by class/subclass as required by other bits of
 * startup software. Only the pci_find_class() function uses this table however
 * it is in a separate file such that it can be added to without modifying
 * other source. Additionally, the -p command line option will allow this table
 * to be supplemented with unknown devices and/or known devices which reside on
 * different buses than those in the table below. This is accommodated via the
 * option_pci_device_table[]
*/

#include <startup.h>
#include <x86/pci_misc.h>


const pci_dev_tbl_t known_pci_device_table[] =
{
	/* the following entries are for known LPC devices */
	{.bus=0, .dev=31, .func=0},	// Intel ICH/PCH/SCH chipsets
	{.bus=0, .dev=7, .func=0},	// Intel PIIX4 chipset (vmware)
	{.bus=0, .dev=20, .func=3},	// AMD SB600/SB700 LPC (class 6, subclass 1)
	{.bus=0, .dev=20, .func=0},	// AMD SB600/SB700 SMbus/ACPI (where the PIRQ route registers actually are)

	/* the following entries are for IO-APIC(s) in a PXH PCIe-PCI bridge */
	{.bus=1, .dev=0, .func=1},	// PXH PCIe-PCI (single/dual) bridge. Bus 1 on Supermicro PDSM4+/PDSME+
	{.bus=1, .dev=0, .func=3},	// PXH PCIe-PCI (dual) bridge. Bus 1 on Supermicro PDSM4+/PDSME+

	{.bus=0, .dev=31, .func=1},	// Denverton/Skylake P2SB
	{.bus=0, .dev=13, .func=0},	// Apollo Lake P2SB
};

const unsigned int known_pci_device_table_size = NUM_ELTS(known_pci_device_table);

pci_dev_tbl_t *option_pci_device_table = NULL;
unsigned int option_pci_device_table_size = 0;

static __inline__ void *realloc_ram(void *p, size_t len)
{
	void *new_p = ws_alloc(len);
	/* this copies data beyond previous <p> but we have access and
	 * it will be cleared by the caller */
	if ((p != NULL) && (new_p != NULL))
	{
		memcpy(new_p, p, len);
	}
	return new_p;
}

void add_known_pci_device(int bus, int device, int function)
{
	/* avoid duplicates with the known table. Duplicates on the command line is user error */
	unsigned int i;
	for (i=0; i<known_pci_device_table_size; i++)
	{
		const pci_dev_tbl_t *p = &known_pci_device_table[i];
		if ((bus == p->bus) && (device == p->dev) && (function == p->func)) {
			return;
		}
	}

	option_pci_device_table = realloc_ram(option_pci_device_table, ++option_pci_device_table_size * sizeof(*option_pci_device_table));
	if (option_pci_device_table == NULL)
	{
		kprintf("add_known_pci_device: realloc_ram() failed!\n");
		return;
	}

	option_pci_device_table[option_pci_device_table_size - 1].bus = bus;
	option_pci_device_table[option_pci_device_table_size - 1].dev = device;
	option_pci_device_table[option_pci_device_table_size - 1].func = function;

	/*
	 * note that since this function is currently called at option processing
	 * time, it likely will never display unless the startup initializes debug
	 * before processing all the command line options, something typically only
	 * done with fastboot startup's
	*/
	if (debug_flag > 0)
	{
		kprintf("added B%u:D%u:F%u to known device table\n",
				option_pci_device_table[option_pci_device_table_size - 1].bus,
				option_pci_device_table[option_pci_device_table_size - 1].dev,
				option_pci_device_table[option_pci_device_table_size - 1].func);
	}
}
