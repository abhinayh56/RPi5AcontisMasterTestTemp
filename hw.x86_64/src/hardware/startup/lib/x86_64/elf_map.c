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

#define PAGEBITS 	((uint64_t)(__PAGESIZE-1))
#define PADDR_SIZE	(0x200000)
#define PADDR_MASK	((uint64_t)(PADDR_SIZE-1))


int
load_elf64mmu_4m(paddr_t addr, Elf64_Ehdr *hdr, Elf64_Phdr *phdr)
{
	static paddr_t	paddr_base;
	static uintptr_t	vaddr_base;
	uintptr_t vaddr;
	paddr_t daddr;
	int					i;

	for(i = 0; i < hdr->e_phnum; ++i, ++phdr)  {
		if (phdr->p_type != PT_LOAD) {
			continue;
		}
		vaddr  = phdr->p_vaddr & ~PAGEBITS;
		daddr = phdr->p_paddr & ~PAGEBITS;

		if ((vaddr & PADDR_MASK) != (daddr & PADDR_MASK)
		    || (phdr->p_memsz != phdr->p_filesz)) {
			if (vaddr_base) { 
			/* Unmap so we can remap using 4k pages*/
				(void)x86_64_4m_map(vaddr_base, 0, PADDR_SIZE, 0);
			}
			return 0;
		}
		if(vaddr_base == 0) {
			unsigned bits;

			// First time around, set up 4m mapping
			vaddr_base = vaddr & ~PADDR_MASK;
			paddr_base = daddr & ~PADDR_MASK;
			bits = X86_64_PTE_PRESENT|X86_64_PTE_WRITE|X86_64_PTE_PS;
//kprintf("load_elf64mmu - no vaddr_base %d\n", __LINE__);
			x86_64_4m_map(vaddr_base, paddr_base, PADDR_SIZE, bits);
		} else if ((vaddr & ~PADDR_MASK) != vaddr_base) {
			crash("executables don't fit in one 4m region");
		}
	}
//kprintf("load_elf64mmu - returning 1 %d\n", __LINE__);
	return 1;
}

int
load_elf32mmu_4m(paddr_t addr, Elf32_Ehdr *hdr, Elf32_Phdr *phdr)
{
	return 0;
}

/*
 Convert a set of ELF permission bits to X86 specific ones and
 map in the specified memory region with the given permissions.
*/
void
elf_map(uintptr_t vaddr, PADDR_T paddr, size_t size, int flags)
{
	int	pf = X86_64_PTE_PRESENT;
	if(flags & PF_W) pf |= X86_64_PTE_WRITE;
	if(!(flags & PF_X)) pf |= X86_64_PTE_NX;
	(void)x86_64_map(vaddr, paddr, size, pf);
}
