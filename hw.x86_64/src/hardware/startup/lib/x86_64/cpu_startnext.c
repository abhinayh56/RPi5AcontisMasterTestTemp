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


void
cpu_startnext(uintptr_t eip, unsigned cpu)
{
	void  (*f)(uintptr_t syspage, unsigned cpu);
	uintptr_t ksys;

	ksys  = (uintptr_t)lsp.system_private.p->kern_syspageptr;
/*-
   after we swap page tables, we are mainly unmapped.
   there is a gratuitous map of this page, but if happened to
   span, we are dead.
   Real solution is to code this in machine code as:
    start_procnto(cpu, gdtaddr, gdt_size-1, idtaddr, idt_size-1, ker_seg);
   which would advertise two labels:  start_procnto, end_start_procnto
   which unity pages could be setup for.
   Notably; kprintf only works if we are on a serial port.
 */
	x86_64_load_pgdir(cpu);
	x86_64_setgdt(gdt_size-1, gdt_ptr);
	x86_64_setidt(idt_size-1, idt_ptr);
	x86_64_setsegs(X86_64_KER_CS);
	f = (void *)eip;
	(*f)(ksys, cpu);
	/*NOTREACHED*/
}
