/*
 * $QNXLicenseC:
 * Copyright 2008, 2024, BlackBerry Limited.
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
#include "multiboot.h"
#include "amd_misc.h"

//
// Initialize hwinfo structure in the system page.
// This code is hardware dependant and may have to be changed
// changed by end users.
//
//


static inline uint16_t
get_port(uintptr_t const base, unsigned const off)
{
	uint16_t * const ptr = (uint16_t *)base + off;
	return *ptr;
}

static void
add_diskinfo(unsigned bus) {
	struct hwi_diskgeometry			*di;

	if(find_startup_info(NULL, STARTUP_INFO_DISK) != NULL) {
		const struct startup_info_disk	*disk;

		hwi_alloc_item(HWI_TAG_INFO(group), HWI_ITEM_DEVCLASS_DISK, bus);
		disk = NULL;
		for( ;; ) {
			disk = find_startup_info(disk, STARTUP_INFO_DISK);
			if(disk == NULL) break;
			di = hwi_alloc_tag(HWI_TAG_INFO(diskgeometry));
			di->disknumber = disk->drive;
			di->sectorsize = 9;
			di->heads = disk->heads;
			di->cyls = disk->cylinders;
			di->sectors = disk->sectors;
			if(disk->blocks != 0) {
				di->nblocks = disk->blocks;
			} else {
				di->nblocks = (unsigned long)di->heads * di->cyls * di->sectors;
			}
		}
	} else if(ACCESS_REG(boot_regs,ax) == MB_SIGNATURE) {
		// Check for Multiboot style information....
		struct mb_info	*info = (void *)ACCESS_REG(boot_regs,bx);

		if(info->flags & MB_INFO_FLAG_DRIVES_VALID) {
			unsigned				len = info->drives_length;

			if(len > 0) {
				struct mb_info_drive	*dr_info = (void *)(uintptr_t)info->drives_addr;

				hwi_alloc_item(HWI_TAG_INFO(group), HWI_ITEM_DEVCLASS_DISK, bus);
				do {
					di = hwi_alloc_tag(HWI_TAG_INFO(diskgeometry));
					di->disknumber = dr_info->drive_number;
					di->sectorsize = 9;
					di->heads = dr_info->drive_heads;
					di->cyls = dr_info->drive_cylinders;
					di->sectors = dr_info->drive_sectors;
					di->nblocks = (unsigned long)di->heads * di->cyls * di->sectors;
	
					len -= dr_info->size;
   					dr_info = (void *)((uintptr_t)dr_info + dr_info->size);
				} while(len != 0);
			}
		}
	}
}

void
init_hwinfo() {
	const struct startup_info_box	*sbox;
	char							*bus;
	unsigned						root;
	hwi_tag							*tag;
	unsigned						i;
	unsigned						bus_owner;
	
	root = hwi_find_item(HWI_NULL_OFF, HWI_ITEM_ROOT_HW, NULL);
	bus = HWI_ITEM_BUS_ISA;
	sbox = find_startup_info(NULL, STARTUP_INFO_BOX);
   	if(sbox != NULL) {
		switch(sbox->bustype) {
		case 1:
			bus = HWI_ITEM_BUS_MCA;
			break;
		case 2:
			tag = hwi_alloc_item(HWI_TAG_INFO(bus), HWI_ITEM_BUS_PCI, root);
			hwi_alloc_item(HWI_TAG_INFO(device), "bios", hwi_tag2off(tag));
			break;
		case 3:
			bus = HWI_ITEM_BUS_EISA;
			break;
		}
	}
	bus_owner = hwi_tag2off(hwi_alloc_item(HWI_TAG_INFO(bus), bus, root));
	add_diskinfo(bus_owner);

	// Add keyboard device
	hwi_add_device(bus, HWI_ITEM_DEVCLASS_MISC, "pckbd", 0);
	hwi_add_irq(1);
	hwi_add_location(0x60, 5, 0, hwi_find_as(0x60, 0));

	// Add serial device(s)
	for(i = 0; i < 4; ++i) {
		uint16_t	port;

		port = get_port(0x400, i);
		if(port == 0) break;
		if(i == 0) {
			hwi_add_device(bus, HWI_ITEM_DEVCLASS_SERIAL, "8250", 0);
		}
		hwi_add_irq(4 - (i & 1));
		hwi_add_location(port, 7, 0, hwi_find_as(port, 0));
	}

	// Add parallel device(s)
	for(i = 0; i < 3; ++i) {
		uint16_t	port;

		port = get_port(0x408, i);
		if(port == 0) break;
		if(i == 0) {
			hwi_add_device(bus, HWI_ITEM_DEVCLASS_PARALLEL, "centronics", 0);
		}
		hwi_add_location(port, 7, 0, hwi_find_as(port, 0));
	}

	// Keep the allocator away from where things may be installed
	hwi_add_device(HWI_ITEM_BUS_UNKNOWN, HWI_ITEM_DEVCLASS_MISC, "in-use", 0);
	hwi_add_location(0, 0x200, 0, hwi_find_as(0, 0));
	hwi_add_location(KILO(64)-0x100, 0x100, 0, hwi_find_as(0, 0)); //buggy
	hwi_add_location(KILO(640), 0x34000, 0, hwi_find_as(KILO(640), 1));
	hwi_add_location(0xd6000, 0x2a000, 0, hwi_find_as(0xd6000, 1));
	hwi_add_irq(14);
	hwi_add_irq(15);

	/* for AMD Ryzen processors, add an MSR entry for the contents of MSR(0xC0010058) */
	if (is_amd_ryzen())
	{
		const unsigned hwi_msr_off = hwidev_add("MSR", 0, HWI_NULL_OFF);
		if (hwi_msr_off != HWI_NULL_OFF)
		{
			const _Uint64t msr_val = rdmsr(0xC0010058);
			/* use the 'base' field for the MSR value and the 'len' field for the MSR */
			hwitag_add_location(hwi_msr_off, msr_val, 0xC0010058, 0, 0);
			if (debug_flag > 2) kprintf("added HWI tag for MSR(0xC0010058) = %L\n", msr_val);
		}
	}

	paddr_t ecam_pbase;
	uint64_t ecam_size;
	const int ret = find_ecam_base(&ecam_pbase, &ecam_size);
	if (!ret && (ecam_pbase != NULL_PADDR) && ecam_size)
	{
		/* add syspage asinfo entries for PCIe ECAM base address */
		const unsigned off = as_find(AS_NULL_OFF, "memory", NULL);
		as_add64(ecam_pbase, ecam_pbase + (paddr_t)ecam_size - 1U, AS_ATTR_DEV, "ecam", off);
		if (debug_flag > 1)
		{
			kprintf("%s: add ecam: %L - %L\r\n",
				__FUNCTION__, ecam_pbase, ecam_pbase + (paddr_t)ecam_size - 1U);
		}
	}

}
