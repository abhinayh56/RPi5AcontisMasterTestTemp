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
#include <hw/acpi.h>

// Return the number of CPU's on the system, using the ACPI information.
unsigned
acpi_smp_num_cpu(void) {
	unsigned			num_cpus = 0;
	unsigned			len;
	paddr_t				paddr;
	acpi_madt			*madt;
	acpi_madt_generic	*p;
	uintptr_t			end;
	paddr_t				lapic;

	paddr = acpi_find_table(ACPI_MADT_SIGNATURE, &len);
	// Tell the upper level if we've got no ACPI on this board.
	if(paddr == NULL_PADDR) return 0;

	madt = startup_memory_map(len, paddr, PROT_READ);

	/* set the LAPIC address now so its available during apic_add_cpuid() */
	lapic = madt->Local_APIC_Address;

	end = (uintptr_t)madt + madt->hdr.Length;
	p = &madt->APIC[0];
	while((uintptr_t)p < end) {
		switch(p->hdr.Type) {
		case ACPI_MADT_TYPE_PROCESSOR_LOCAL_APIC:
			if(p->processor_local_apic.Flags & ACPI_MADT_PLA_FLAGS_ENABLED) {
				if(debug_flag > 2) {
					kprintf("LAPIC id: %w\n", p->processor_local_apic.APIC_ID);
				}
				apic_add_cpuid(num_cpus, p->processor_local_apic.APIC_ID);
				++num_cpus;
			}
			break;
		case ACPI_MADT_TYPE_LOCAL_APIC_ADDRESS_OVERRIDE:
			lapic = p->local_apic_address_override.Local_APIC_Address;
			break;
		}
		p = (void *)((uintptr_t)p + p->hdr.Length);
	}
	if(debug_flag > 2) {
		kprintf("LAPIC addr: 0x%P\n", lapic);
	}
	apic_set_paddr(lapic);

	startup_memory_unmap(madt);

	/* if no APIC ID's found (shouldn't happen), unset the apic paddr */
	if (num_cpus == 0) {
		apic_set_paddr(NULL_PADDR_STARTUP);
	}
	return num_cpus;
}
