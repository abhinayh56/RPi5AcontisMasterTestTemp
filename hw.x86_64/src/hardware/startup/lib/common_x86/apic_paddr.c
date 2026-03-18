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
#include <x86/lapic.h>

PADDR_T		lapic_paddr = NULL_PADDR_STARTUP;
void		*lapic_callout_vaddr;
void		*lapic_startup_vaddr;


unsigned
lapic_id(void) {
	if(lapic_startup_vaddr == NULL) {
		// Nobody's set the lapic register location, assume that there's
		// only one CPU and the id is zero.
		return 0;
	}
	return (lapic_read(LAPIC_ID) & 0xff000000u) >> 24;
}

/*
 * add an AS entry to /memory/ for the local APIC(s)
 *
 * Because the LAPIC addr can be modified on a per CPU basis, this API accommodates
 * that however currently all CPU's will have the same LAPIC address.
 *
 * The syntax for the AS entry will be as follows.
 * If all CPU's do not have the exact same LAPIC address then there must exist
 * an AS entry for each CPU of the following form
 *
 *		/memory/lapic.<cpu> where 0 =< cpu < # cpu's
 *
 * If all CPU's have the same LAPIC address (the current behaviour) then a single
 * entry of the form is all that's required
 *
 *		/memorylapic
 */
void
apic_add_as_entry(const int cpu, const paddr_t lapic_addr)
{
	char entry_name[] = {"lapic.xxx"};

	ASSERT(cpu < 999);

	if (cpu >= 0) {
		ksprintf(&entry_name[5], ".%u", cpu);
	} else {
		entry_name[5] = '\0';
	}

	unsigned off = as_find(AS_NULL_OFF, "memory", NULL);

	if (off != AS_NULL_OFF) {
		off= as_add(lapic_addr, lapic_addr + 0x3EF, AS_ATTR_DEV, entry_name, off);
	}

	if (debug_flag)
	{
		if (off != AS_NULL_OFF) {
			kprintf("Added /memory/%s\n", entry_name);
		} else {
			kprintf("Unable to add AS entry for LAPIC\n");
		}
	}
}

void
apic_set_paddr(PADDR_T p) {
	lapic_paddr = p;
	// Have to set this here because we're going to need the
	// startup vaddr before apic_set_vaddr() gets called
	lapic_startup_vaddr = startup_memory_map(__PAGESIZE, lapic_paddr, PROT_READ|PROT_WRITE|PROT_NOCACHE);

	apic_add_as_entry(-1, p);
}


void
apic_set_vaddr(void) {
	if(lapic_paddr == NULL_PADDR_STARTUP) {
		// discovering the number of CPU's tells us where the local APIC is
		if(acpi_smp_num_cpu() == 0) {
			imps_smp_num_cpu();
		}	
	}
	if(lapic_paddr != NULL_PADDR_STARTUP) {
		// Have to set this here because apic_set_paddr() gets called  before
		// callout_memory_map() can be used.
		lapic_callout_vaddr = callout_memory_map(__PAGESIZE, lapic_paddr, PROT_READ|PROT_WRITE|PROT_NOCACHE);
	}
//kprintf("set lapic vaddr s/c %v/%v => %P\n", lapic_startup_vaddr, lapic_callout_vaddr, (paddr_t)lapic_paddr);
}
