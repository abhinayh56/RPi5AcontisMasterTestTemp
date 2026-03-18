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

void *
callout_memory_map(unsigned size, paddr_t phys, unsigned prot_flags) {
	void	*p;
	int		pf;

	pf = X86_PREF(PTE_PRESENT);
	if(prot_flags & PROT_WRITE) pf |= X86_PREF(PTE_WRITE);
	if(prot_flags & PROT_USER) pf |= X86_PREF(PTE_USER);
	// We turn on both CD & WT bits to make sure we get a UC memory type
	// mapping on systems with PAT support - cpu_startup() reprograms PA2 to
	// be WC type
	if(prot_flags & PROT_NOCACHE) pf |= X86_PREF(PTE_CD) | X86_PREF(PTE_WT);

	p = (void *)x86_pref(map)(~(uintptr_t)0, phys, size, pf);
	return(p);
}
