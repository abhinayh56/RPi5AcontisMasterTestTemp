/*
 * $QNXLicenseC:
 * Copyright 2008, QNX Software Systems. 
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

#define PCI_CONFADDR 0xcf8
#define PCI_CONFDATA 0xcfc

void pci_write_cfg32(uint8_t bus, uint8_t dev_number, uint8_t func_number, uint8_t reg_offset, uint32_t value)
{
	uint32_t pci_addr;
	
	if (dev_number > 31) {
		crash("Invalid PCI device number: %d, should be <= 31\n", dev_number);
	}
	
	if (func_number > 7) {
		crash("Invalid PCI function number: %d, should be <= 7\n", func_number);
	}
	
	/* 
	 * PCI Address breakdown:
	 * bit31 = 1
	 * bit30:24 = 0 (reserved)
	 * bit23:16 = bus (0-255)
	 * bit15:11 = device (0-31)
	 * bit10:8 = function (0-7)
	 * bit7:0 = register (0-255)
	 */
	pci_addr = bus;
	pci_addr <<= 5;
	pci_addr |= dev_number;
	pci_addr <<= 3;
	pci_addr |= func_number;
	pci_addr <<= 8;
	pci_addr |= reg_offset;
	pci_addr |= 0x80000000;
	
	out32(PCI_CONFADDR, pci_addr);
	out32(PCI_CONFDATA, value);
}
