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

/*
 * PCI/LPC support functions
 *
 * Note that most of the routines use 'lpc' as the keyword for the PCI/ISA
 * bridge since it is easier to put in the name of functions
*/

#include <startup.h>
#include <x86/pci_misc.h>

static unsigned devfunc = ~0;

/*
 * get_LPC_pci_devfunc
 *
 * returns the PCI device/function number for a PCI device of class 'bridge' (6)
 * and subclass 'isa' (1) (ie. the PCI/ISA bridge device). On newer chipsets the
 * PCI/ISA bridge is also known as the LPC device.
 *
 * Returns: PCI_DEVFUNC(device, function) or -1 on error
 */
int get_LPC_pci_devfunc(void)
{
	static unsigned found_already = 0;

	if (!found_already)
	{
		unsigned tmp_devfunc = ~0;
		int r = pci_find_class(0, 6, 1, NULL, &tmp_devfunc);

		if (r < 0) return r;

		if (debug_flag) kprintf("pci_find_class() returned D%u:F%u\n", PCI_DEVNO(tmp_devfunc), PCI_FUNCNO(tmp_devfunc));
#if 0
		not valid for non Intel
		/* always expect the PCI/ISA (aka LPC) device to be BUS 0, FUNC 0. Device may change */
		if (PCI_FUNCNO(tmp_devfunc) != 0) return -1;
#endif

		found_already = 1;
		devfunc = tmp_devfunc;
	}
	return devfunc;
}

void set_LPC_pci_devfunc(unsigned df)
{
	devfunc = df;
}

/*
 * get_intel_LPC_devid
 *
 * returns the PCI device id for the PCI/ISA (aka LPC) device if found or ~0 on
 * error.
 * Different PCI/ISA devices have different routing registers/etc
 */
unsigned get_LPC_devid(void)
{
	int devfunc = get_LPC_pci_devfunc();
	uint16_t device_id;

	if (devfunc < 0) return ~0;

	device_id = pci_read_cfg16(0, PCI_DEVNO(devfunc), PCI_FUNCNO(devfunc), 2);
	return device_id;
}

uint8_t get_lpc_reg8(unsigned offset)
{
	int lpc_devfunc = get_LPC_pci_devfunc();
//printf("lpc_devfunc = D:%u,F:%u\n", PCI_DEVNO(lpc_devfunc), PCI_FUNCNO(lpc_devfunc));
	if (lpc_devfunc < 0) return ~0;
//printf("read 8 bits @ offset %x\n", offset);
	return pci_read_cfg8(0, PCI_DEVNO(lpc_devfunc), PCI_FUNCNO(lpc_devfunc), offset);
}

uint16_t get_lpc_reg16(unsigned offset)
{
	int lpc_devfunc = get_LPC_pci_devfunc();

	if (lpc_devfunc < 0) return ~0;
	return pci_read_cfg16(0, PCI_DEVNO(lpc_devfunc), PCI_FUNCNO(lpc_devfunc), offset);
}

uint32_t get_lpc_reg32(unsigned offset)
{
	int lpc_devfunc = get_LPC_pci_devfunc();

	if (lpc_devfunc < 0) return ~0;
	return pci_read_cfg32(0, PCI_DEVNO(lpc_devfunc), PCI_FUNCNO(lpc_devfunc), offset);
}

void set_lpc_reg8(unsigned offset, uint8_t val)
{
	int lpc_devfunc = get_LPC_pci_devfunc();

	if (lpc_devfunc < 0) return;
	pci_write_cfg8(0, PCI_DEVNO(lpc_devfunc), PCI_FUNCNO(lpc_devfunc), offset, val);
}

void set_lpc_reg16(unsigned offset, uint16_t val)
{
	int lpc_devfunc = get_LPC_pci_devfunc();

	if (lpc_devfunc < 0) return;
	pci_write_cfg16(0, PCI_DEVNO(lpc_devfunc), PCI_FUNCNO(lpc_devfunc), offset, val);
}

void set_lpc_reg32(unsigned offset, uint32_t val)
{
	int lpc_devfunc = get_LPC_pci_devfunc();

	if (lpc_devfunc < 0) return;
	pci_write_cfg32(0, PCI_DEVNO(lpc_devfunc), PCI_FUNCNO(lpc_devfunc), offset, val);
}

/*
 * pci_find_class
 *
 * The PCI server equivalent of this function locates all the devices of a
 * given class and subclass.
 *
 * Since we do not want to scan (enumerate) the PCI bus in startup, we instead
 * attempt to access a specific list of known devices. The devices can be of
 * any class/subclass and in fact all devices that may need to be looked up in
 * startup can be added to the list. Additionally, we first look through a list
 * of optional command line provided devices.
 *
 * Specifically, in order to support get_LPC_pci_devfunc(), all supported
 * PCI/ISA (aka LPC) bridge devices (class 6, subclass 1) should be in the list
 *
 * In order to search for multiple devices of the same class, we pass in
 * <cookie> == 0 on the first call to pci_find_class() and as long as -1 is
 * not returned by pci_find_class() we can then pass in the return value from
 * pci_find_class() as the first parameter in order to search for the next
 * device of the specified class.
 * 
 * 		ex.
 * 			cookie = 0;
 * 			while ((cookie = pci_find_class(cookie, ...)) != -1)
 * 			{
 * 				:
 * 			}
 *
 * Additionally, if <bus> != NULL and *<bus> >=0, then the caller can specify
 * a specific bus on which to look for the device class/subclass. This allows
 * the caller to cycle through as many buses as they desire instead of being
 * limited to the bus entries in the device_table[]. If the caller is interested
 * in the bus number on which a device is found but does not want to override the
 * "known" entries, then set *<bus> == -1. If using a loop such as in the example
 * above, setting *<bus> = -1 must be done for each iteration of the loop since
 * it will have been filled in with an actual value upon successful return
 *
 * IMPORTANT: whenever a new class/subclass pair is being searched for, or -1
 * 			  is returned, always pass 0 as the first parameter (cookie) in any
 * 			  new search
 *
 * Returns: on success, sets the *<bus> and *<devfunc> = PCI_DEVFUNC(device, function)
 * 			and returns a cookie for use in subsequent calls, otherwise it returns -1
 */

int pci_find_class(int cookie, unsigned class, unsigned subclass, int *bus, unsigned *devfunc)
{
	extern const pci_dev_tbl_t known_pci_device_table[];
	extern const unsigned int known_pci_device_table_size;
	extern const pci_dev_tbl_t *option_pci_device_table;
	extern const unsigned int option_pci_device_table_size;
	unsigned i;

	for (i=cookie; i<option_pci_device_table_size; i++)
	{
		pci_dev_tbl_t const *d = &option_pci_device_table[i];
		unsigned search_bus = ((bus != NULL) && (*bus >= 0)) ? *bus & 0xff : d->bus;

		if (pci_read_cfg16(search_bus, d->dev, d->func, 0) != 0xFFFFU)
		{
			/* check the class and subclass for the device */
			const uint16_t c = pci_read_cfg8(search_bus, d->dev, d->func, 0xb);
			const uint16_t sc = pci_read_cfg8(search_bus, d->dev, d->func, 0xa);
			/* check the class and subclass for the device */
			if ((c == class) && (sc == subclass))
			{
				if (devfunc != NULL) *devfunc = PCI_DEVFUNC(d->dev, d->func);
				if (bus != NULL) *bus = search_bus;
				return ++i;
			}
		}
	}
	for (i=cookie; i<known_pci_device_table_size; i++)
	{
		pci_dev_tbl_t const *d = &known_pci_device_table[i];
		unsigned search_bus = ((bus != NULL) && (*bus >= 0)) ? *bus & 0xff : d->bus;

		if (pci_read_cfg16(search_bus, d->dev, d->func, 0) != 0xFFFFU)
		{
			/* check the class and subclass for the device */
			const uint16_t c = pci_read_cfg8(search_bus, d->dev, d->func, 0xb);
			const uint16_t sc = pci_read_cfg8(search_bus, d->dev, d->func, 0xa);
			/* check the class and subclass for the device */
			if ((c == class) && (sc == subclass))
			{
				if (devfunc != NULL) *devfunc = PCI_DEVFUNC(d->dev, d->func);
				if (bus != NULL) *bus = search_bus;
				return ++i;
			}
		}
	}
	return -1;
}
