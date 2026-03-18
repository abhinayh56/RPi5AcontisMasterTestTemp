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

/*
 * find_top_cpu_intr
 * 
 * return the highest regsistered (via add_interrupt()) external CPU interrupt
 * or 0 if none have been registered yet 
*/
unsigned
find_top_cpu_intr() {
	struct intrinfo_entry		*intr;
	unsigned					max_intr;
	unsigned					curr_intr;
	unsigned					num;

	// Find the highest hardware interrupt vector so far.

	max_intr = 0;
	intr = lsp.intrinfo.p;
	for (num =lsp.intrinfo.size/sizeof(*intr); num != 0; --num)
	{
		curr_intr = intr->cpu_intr_base;
		if(intr->cpu_intr_stride != 0) {
			curr_intr += intr->cpu_intr_stride * (intr->num_vectors-1);
		}
		if(curr_intr > max_intr) max_intr = curr_intr;
		++intr;
	}
	return max_intr;
}

/*
 * find_next_cpu_intr
 * 
 * get the next usable external (hardware) CPU interrupt. This function does
 * what most of the users of find_top_cpu_intr() have to do anyway.
*/
unsigned
find_next_cpu_intr() {
	const unsigned top = find_top_cpu_intr();
	return (top == 0) ? FIRST_HARDWARE_INTERRUPT : top + 1;
}
