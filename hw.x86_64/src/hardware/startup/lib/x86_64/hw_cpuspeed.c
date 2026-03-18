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



unsigned long
x86_64_cpuspeed(unsigned cpunum) {
	if( lsp.cpuinfo.p[cpunum].flags & X86_64_CPU_RDTSC ) {
		if (lsp.qtime.p->cycles_per_sec != 0) {
			return lsp.qtime.p->cycles_per_sec / (1000 * 1000); // MHz
		}
		return (calibrate_tsc(1000000) / 1000000);	/* calibrate_tsc(1ms) / (10^conversion_scale(from Hz to MHz)) */
	}
	return 1;
}

// Defined to make it easier to use shared code
unsigned long
x86_cpuspeed(unsigned cpunum) {
	return x86_64_cpuspeed(cpunum);
}
