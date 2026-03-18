/*
 * $QNXLicenseC:
 * Copyright 2008, 2020 QNX Software Systems.
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

#define NON_CACHE_BASE	((uintptr_t)0xffffff8000000000)

#define BIG_PAGE_MASK	(~((paddr_t)(MEG(2)-1)))

void *
startup_memory_map(unsigned size, paddr_t phys, unsigned prot_flags) {
	if(!(prot_flags & PROT_NOCACHE)) {
		// cachable mappings can use the unity mapping already established
		return (void *)phys;
	}

	uintptr_t	p = NON_CACHE_BASE + phys;
	if(p < NON_CACHE_BASE) {
		// wrapped :-(
		crash("physical address %P too high to handle", phys);
	}
	if(((p+size-1) & BIG_PAGE_MASK) != (p & BIG_PAGE_MASK)) {
		crash("can't handle mapping crossing 2M boundry");
	}

	paddr_t tbl_paddr = rdcr3() & ~(paddr_t)(__PAGESIZE-1);

	unsigned 	bit_pos;
	paddr_t		pte_paddr;
	for(bit_pos = 39; bit_pos >= 21; bit_pos -= 9) {
		static int first_time = 1;
		pte_paddr = tbl_paddr + ((p >> (bit_pos-3)) & (0x1ffu<<3));
		paddr_t pte = *(paddr_t *)pte_paddr;
		if(first_time) {
			// The very first time, we don't care what's in the PML4
			// entry - we want to change it to point at our set of tables
			first_time = 0;
			pte = 0;

			/* Write enable the Page Table that may have been disabled by UEFI */
			ldcr0(rdcr0() & ~X86_64_CR0_WP_BIT);
		}
		if(!(pte & X86_64_PTE_PRESENT)) {
			// page table doesn't exist yet, add it in
			pte = find_ram(__PAGESIZE, __PAGESIZE, 0, 0);
			if(pte == NULL_PADDR_STARTUP) {
				crash("no memory for startup page table");
			}
			avoid_ram(pte, __PAGESIZE);
			memset((void *)pte, 0, __PAGESIZE);
			pte |= (X86_64_PTE_PRESENT|X86_64_PTE_WRITE);
			*(paddr_t *)pte_paddr = pte;
		}
		tbl_paddr = pte & ~(paddr_t)(__PAGESIZE-1);
	}
	// We turn on both CD & WT bits to make sure we get a UC memory type
	// mapping on systems with PAT support - cpu_startup() reprograms PA2 to
	// be WC type
	*(paddr_t *)pte_paddr = (phys & BIG_PAGE_MASK)
		| (X86_64_PTE_PRESENT|X86_64_PTE_WRITE|X86_64_PTE_CD|X86_64_PTE_WT|X86_64_PTE_PS);
	ldcr3(rdcr3()); // invalidate the TLB
	return (void *)p;
}


void
startup_memory_unmap(void *p) {
	// Nothing to do
}
