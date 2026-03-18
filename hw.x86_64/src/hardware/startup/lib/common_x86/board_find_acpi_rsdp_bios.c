/*
 * $QNXLicenseC:
 * Copyright 2007, 2008, QNX Software Systems.
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
#include <hw/acpi.h>

static void *
find_rsdp(paddr_t paddr, unsigned size) {
	acpi_rsdp	*root;
	paddr_t		end;

	end = paddr + size - sizeof(*root);
	do {
		root = startup_memory_map(sizeof(acpi_rsdp), paddr, PROT_READ);
		if(memcmp(root->v1.Signature, ACPI_RSDP_SIGNATURE, sizeof(root->v1.Signature)) == 0) {
			unsigned len = sizeof(root->v1);
			if(root->v1.Revision > ACPI_RSDP_REVISION_1_0) {
				len = root->Length;
			}
			if(calc_cksum(root, len) == 0) {
				static int recorded_table = 0;
				if(!recorded_table) {
					recorded_table = 1;
					as_add(paddr, paddr+len-1, AS_ATTR_RAM,
						"acpi_rsdp", as_find(AS_NULL_OFF, "memory", NULL));
				}
				return root;
			}
		}
		startup_memory_unmap(root);
		paddr += 16;
	} while(paddr < end);
	return NULL;
}

void *
board_find_acpi_rsdp_bios(void) {
	paddr_t				ebda;
	uint16_t			*p;
	acpi_rsdp			*root;

	// Safety value for buggy BIOS's.
	if(acpi_ignore) return NULL;

	p = startup_memory_map(0x2, 0x40e, PROT_READ);
	ebda = *p << 4;
	startup_memory_unmap(p);

	if(ebda != 0) {
		root = find_rsdp(ebda, 0x1024);
		if(root != NULL) return root;
	}
	return find_rsdp(0xe0000, 0xfffff - 0xe0000);
}
