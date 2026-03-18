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

uintptr_t gdt_ptr;
uintptr_t idt_ptr;

//
// Initialize CPU specific pointers in the system page.
// This code is hardware independant and should not have to be
// changed by end users.
//

void
cpu_init_syspage_memory() {
}


struct syspage_entry *
cpu_alloc_syspage_memory(PADDR_T *cpupage, PADDR_T *syspage, unsigned spsize) {
	struct syspage_entry		*sp = lsp.syspage.p;
	struct system_private_entry	*private;
	unsigned					size;
	unsigned					cpsize;
	PADDR_T						syspage_paddr;

	#define SP_OFFSET(field)	PTR_DIFF(lsp.cpu.field.p, sp)
	#define INIT_ENTRY(_cpu,_field)	\
		sp->un._cpu._field.entry_size = lsp.cpu._cpu##_##_field.size;	\
		sp->un._cpu._field.entry_off = SP_OFFSET(_cpu##_##_field)

	spsize = ROUND(spsize,sizeof(uint64_t));
	cpsize = sp->num_cpu * sizeof(struct cpupage_entry);

	// The +1 is to turn the maximum interrupt into the number of interrupts.
	idt_size = find_top_cpu_intr() + 1;

	//Round up to next multiple of 0x10 and add 2 so that we have
	//room for the IPI vectors at the top end of IDT (higher prio in LAPIC).
	idt_size = ROUND(idt_size, 0x10) + 2;
	if(idt_size > 256) idt_size = 256;
	idt_size *= sizeof(X86_64_GATE);
		
	//
	// Allocate the system page (and cpupage entries) and save it away.
	// The system page must be 4K aligned in a virtual system.
	//
	size = spsize + cpsize + (GDT_SIZE + idt_size);

	syspage_paddr = calloc_ram(size, lsp.system_private.p->pagesize);
	if(syspage_paddr == NULL_PADDR_STARTUP) {
		crash("could not allocate 0x%x bytes for syspage/cpupage\n", size );
	}
	// Map the system page mapped twice in virtual space. Once R/W for the
	// kernel and once R/O for users
	private = lsp.system_private.p;

	private->user_syspageptr = TOPTR(x86_64_map(~0L,syspage_paddr,size, X86_64_PTE_PRESENT | X86_64_PTE_USER));
	private->kern_syspageptr = TOPTR(x86_64_map(~0L,syspage_paddr,size, X86_64_PTE_PRESENT | X86_64_PTE_WRITE));
	private->user_cpupageptr = TOPTR((uintptr_t)private->user_syspageptr + spsize);
	private->kern_cpupageptr = TOPTR((uintptr_t)private->kern_syspageptr + spsize);
	private->cpupage_spacing = sizeof(struct cpupage_entry);

	*syspage = syspage_paddr;
	*cpupage = syspage_paddr + spsize ;

	spsize += cpsize;

	gdt_ptr = (uintptr_t)private->kern_syspageptr + spsize;
	gdt_paddr = syspage_paddr + spsize;
	gdt_size  = GDT_SIZE;
	spsize += GDT_SIZE;

	idt_ptr = (uintptr_t)private->kern_syspageptr + spsize;
	idt_paddr = syspage_paddr + spsize;
	spsize += idt_size;

	init_gdt(idt_size);

	return((void *)syspage_paddr);
}

void
cpu_write_syspage_memory(PADDR_T sysp_paddr, unsigned sysp_size, unsigned callout_size) {
}

void
cpu_syspage_remap(uintptr_t vaddr, paddr_t paddr, size_t size, int flags)
{
	elf_map(vaddr, paddr, size, flags);
}
