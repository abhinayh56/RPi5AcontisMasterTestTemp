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

//
// Default board specific SMP routines - use the Intel MultiProcessor
// Specification implementation.
//

// Return the number of CPU's on the system.
unsigned
board_smp_num_cpu(void) {
	unsigned	num;

	num = acpi_smp_num_cpu();
	if(num != 0) return num;
	return imps_smp_num_cpu();
}

// Perform board specific initialization of the smp_entry section of
// the system page.
void
board_smp_init(struct smp_entry *smp, unsigned num_cpus) {
	apic_smp_init(smp, num_cpus);
}

// Get the 'cpu' processor executing at the 'start' address.
int
board_smp_start(unsigned cpu, void (*start)(void)) {
	return apic_smp_start(cpu, start);
}

// Perform any necessary adjustment of the cpu number
unsigned
board_smp_adjust_num(unsigned cpu) {
	return apic_smp_adjust_num(cpu);
}
