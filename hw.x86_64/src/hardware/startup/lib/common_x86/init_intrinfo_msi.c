/*
 * $QNXLicenseC:
 * Copyright 2009, QNX Software Systems. 
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

/*
 * Add entries for use as MSI/MSI-X interrupts 
*/

#include <startup.h>
#if defined(__X86_64__)
#include <x86_64/intr.h>
#else
#include <x86/intr.h>
#endif

extern struct callout_rtn	interrupt_id_lapic;
extern struct callout_rtn	interrupt_eoi_lapic;
extern struct callout_rtn	interrupt_lapic_stub;

static struct startup_intrinfo msi =
{
	.vector_base = 0,		// filled in below
	.num_vectors = 0,		// filled in below
	.cascade_vector = _NTO_INTR_SPARE,
	.cpu_intr_base = 0,		// filled in below
	.cpu_intr_stride = 1,
	.flags = INTR_FLAG_MSI,
	.id = {.genflags = 0, .size = 0, .rtn = &interrupt_id_lapic},
	.eoi = {.genflags = INTR_GENFLAG_NOGLITCH, .size = 0, .rtn = &interrupt_eoi_lapic},
	.mask = &interrupt_lapic_stub,
	.unmask = &interrupt_lapic_stub,
	.config = &interrupt_lapic_stub,
	.patch_data = NULL,		// filled in below
};

/*
 * init_intrinfo_msi
 * 
 * Initialize an intrinfo entry to use the remaining unassigned x86 vectors for
 * MSI.
 * 
 * If <vector_base> < 0, then the default X86[_64]_INTR_CLASS_MSI value will be used
 * otherwise MSI vectors will start at <vector_base>
 * 
 * Returns: the number of vectors configured
*/
static apic_patch_data_t patch_data;

int init_intrinfo_msi(int vector_base)
{
	if (vector_base < 0) vector_base = X86_PREF(INTR_CLASS_MSI);

	/* use every remaining vector upto the architecturally defined 254 (255 is reserved) */
	msi.vector_base = vector_base;
	msi.cpu_intr_base = find_next_cpu_intr();
	ASSERT(msi.cpu_intr_base < 255);
	msi.num_vectors = (254 - msi.cpu_intr_base) + 1;

	patch_data.lapic = lapic_callout_vaddr;
	msi.patch_data = &patch_data;

	add_interrupt(&msi);

	if (debug_flag)
	{
		kprintf( "MSI interrupt = 0x%x\n", X86_PREF(INTR_CLASS_MSI));
		kprintf( "MSI vector no = %d 0x%x\n", msi.cpu_intr_base, msi.cpu_intr_base );
		kprintf( "MSI vec count = %d\n", msi.num_vectors);
	}

	return msi.num_vectors;
}
