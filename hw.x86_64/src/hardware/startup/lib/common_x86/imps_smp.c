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
#include <x86/imps.h>


// Return the number of CPU's on the system.
unsigned
imps_smp_num_cpu(void) {
	struct smp_fps				*fps;
	int							n;
	PADDR_T						addr;
	unsigned					num_cpus;
	ssize_t						addr_incr;
	union {
		uint8_t 				type;
		struct smp_hdr			hdr;
		struct smp_processor	processor;
		struct smp_bus			bus;
		struct smp_ioapic		ioapic;
		struct smp_intsrc		intsrc;
		struct smp_lintsrc		lintsrc;
	} 						cfg;

	fps = smp_get_fps();
	if(fps == NULL) {
		return 1;
	}

	if(fps->config == 0) {
		kprintf("MP configuration table is missing.\n");
		return 1;
	}

	addr = (PADDR_T)fps->config;
	memmove(&cfg.hdr, (void *)addr, sizeof(cfg.hdr));
	if(debug_flag > 2) {
		kprintf("LAPIC addr: %P, #entries=%d\n", (paddr_t)cfg.hdr.lapic_addr, cfg.hdr.entry_count);
	}
	apic_set_paddr(cfg.hdr.lapic_addr);

	num_cpus = 0;
	addr = (PADDR_T)fps->config + sizeof(cfg.hdr);
	for(n = cfg.hdr.entry_count ; n ; --n) {
		memmove(&cfg, (void *)addr, sizeof(cfg));
		if(cfg.type == MP_PROCESSOR) {
			if(debug_flag > 2) {
				kprintf("LAPIC id: %w", cfg.processor.id);
				if(cfg.processor.flags & 0x02) {
					kprintf(" (BSP)");
				}
				kprintf("\n");
			}
			apic_add_cpuid(num_cpus, cfg.processor.id);
			++num_cpus;
		}
		if((addr_incr = imps_entry_size(cfg.type)) < 0) {
			kprintf("Unknown SMP entry type %d.\n", cfg.type);
			break;
		}
		addr += addr_incr;
	}
	
	/*
	*  Temp fix for Dell P4's. For some reason they decide about
	*  half way thru the startup that there isn't actually a processor.
	*  This should be revisted when we have time.
	*/
	if(num_cpus == 0) num_cpus = 1;

	return num_cpus;
}
