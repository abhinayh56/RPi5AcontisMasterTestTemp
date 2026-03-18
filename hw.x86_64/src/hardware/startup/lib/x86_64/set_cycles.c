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
#include "common_x86/hv_guest.h"

//
// Initialize cycle_per_sec field in the qtime structure in the system page.
//
void
set_cycles_per_sec(struct qtime_entry *qtime) {
	if(cycles_freq != 0) {
		qtime->cycles_per_sec = cycles_freq;
		return;
	}

    // Give a chance to an hypervisor to tell us
    if (hvg_tsc_frequency(&qtime->cycles_per_sec)) {
        return;
    }

	/* If the processor supports invariant(constant) TSC,
	 * the TSC is based on MSR
	 * If the processor supports TSC, but not invariant
	 * the TSC is based on reading TSC
	 */
#if 0
	/* We disable invarant tsc rate from MSR because profiling
	 * gives us a better accuracy at the moment
	 */
	if (cpu_tsc & X86_TSC_INVARIANT) {
		qtime->cycles_per_sec = get_invariant_tsc();
	}
#endif
	if (qtime->cycles_per_sec == 0) {
		qtime->cycles_per_sec = calibrate_tsc(10000000);
	}
}
