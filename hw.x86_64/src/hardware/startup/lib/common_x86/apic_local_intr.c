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
#if defined(__X86_64__)
#include <x86_64/intr.h>
#else
#include <x86/intr.h>
#endif
#include <x86/lapic.h>
#include <x86/imps.h>
#include "amd_misc.h"

//
// Initialize interrupt controller hardware & intrinfo structure in the
// system page.
// This code is hardware dependant and may have to be changed
// changed by end users.
//
// This version is for the Pentium local APIC


extern struct callout_rtn	interrupt_id_lapic;
extern struct callout_rtn	interrupt_eoi_lapic;
extern struct callout_rtn	interrupt_mask_lapic;
extern struct callout_rtn	interrupt_unmask_lapic;
extern struct callout_rtn	interrupt_config_lapic;

extern struct callout_rtn	interrupt_id_lapic_spurious;
extern struct callout_rtn	interrupt_eoi_lapic_spurious;
extern struct callout_rtn	interrupt_mask_lapic_spurious;
extern struct callout_rtn	interrupt_unmask_lapic_spurious;
extern struct callout_rtn	interrupt_config_lapic_spurious;

static struct startup_intrinfo lapic_intrs[] = {
	{
		.vector_base = X86_PREF(INTR_CLASS_APIC),
		.num_vectors = 0,		// filled in below
		.cascade_vector = _NTO_INTR_SPARE,
		.cpu_intr_base = 0,		// filled in below
		.cpu_intr_stride = 1, 
		.flags = 0,
		.id = {.genflags = INTR_GENFLAG_NOGLITCH, .size = 0, .rtn = &interrupt_id_lapic},
		.eoi = {.genflags = 0, .size = 0, .rtn = &interrupt_eoi_lapic},
		.mask = &interrupt_mask_lapic, 
		.unmask = &interrupt_unmask_lapic, 
		.config = &interrupt_config_lapic,
		.patch_data = NULL,
		.local_stride = 0x10,
	},
	{
		.vector_base = X86_PREF(INTR_CLASS_APIC)+0xffff,
		.num_vectors = 1,
		.cascade_vector = _NTO_INTR_SPARE,
		.cpu_intr_base = APIC_SPURIOUS_INTERRUPT,
		.cpu_intr_stride = 1, 
		.flags = 0,
		.id = {.genflags = 0, .size = 0, .rtn = &interrupt_id_lapic_spurious},
		.eoi = {.genflags = 0, .size = 0, .rtn = &interrupt_eoi_lapic_spurious},
		.mask = &interrupt_mask_lapic_spurious, 
		.unmask = &interrupt_unmask_lapic_spurious, 
		.config = &interrupt_config_lapic_spurious,
		.patch_data = NULL,
		.local_stride = 0x10,
	},
};

#ifdef APIC_DEBUG
/* dump some of the more interesting Local APIC registers */
static void 
dump_lapic(int cpu) {
	kprintf("\nLAPIC register dump (CPU %u)\n---------------------------\n", cpu);
	kprintf("Id: 0x%x\n", (lapic_read(LAPIC_ID) & 0xff000000) >> 24);
	/* directed EOI support is indicated by bit 24 of the Version register */
	kprintf("Version: %x, num LVT: %u\n", lapic_read(LAPIC_VER) & ~0x00ff0000, (lapic_read(LAPIC_VER) & 0x00ff0000) >> 16);
	kprintf("Logical Dest: %x\n", lapic_read(LAPIC_LDR));
	kprintf("Spurious IVEC: %x\n", lapic_read(LAPIC_SPIV));
	kprintf("Error Status: %x\n", lapic_read(LAPIC_ESR));
	kprintf("LVT CMCI: %x\n", lapic_read(0x2f0));
	kprintf("LVT ICR: %x %x\n", lapic_read(LAPIC_ICRH), lapic_read(LAPIC_ICRL));
	kprintf("LVT Timer: %x\n", lapic_read(LAPIC_LTR));
	kprintf("LVT Thermal Sensor: %x\n", lapic_read(LAPIC_LTMR));
	kprintf("LVT PerfMon: %x\n", lapic_read(LAPIC_LPCR));
	kprintf("LVT LINT0: %x\n", lapic_read(LAPIC_LL0R));
	kprintf("LVT LINT1: %x\n", lapic_read(LAPIC_LL1R));
	kprintf("LVT Error: %x\n", lapic_read(LAPIC_LER));
	kprintf("\n");
}
#define DUMP_LAPIC(__cpu__)	dump_lapic((__cpu__))
#else	/* APIC_DEBUG */
#define DUMP_LAPIC(__cpu__)
#endif	/* APIC_DEBUG */

/* 
 * lapic_configure
 * 
 * Even without configuring the LAPIC, it is set for Virtual Wire Mode
 * (ie. LINT0 is ExtINT and LINT1 is NMI). This is being done by the BIOS.
 * All we are doing when we pass -L is to additionally configure the timer,
 * thermal sensor, performance counter and other internal LVT entries
 * 
 * ???
 * 
 * There is Local APIC info in the ACPI tables. Why are we not looking at that?
 * 
 * For Symmetric I/O mode, need to get out of PIC mode by setting bits in IMCR
 * (see pg 3-7 of the Multiprocessor Spec v1.4 from Intel) however need to read
 * MP feature information bytes to determine if IMCR is implemented.
*/
void
lapic_configure(int cpu) {

	if (debug_flag) kprintf("lapic_configure(cpu=%d), lapic_id: 0x%x\n", cpu, lapic_id());

	if(lapic_callout_vaddr != NULL) {
		const unsigned num_lvt = ((lapic_read(LAPIC_VER) & LVR_MAX_LVT_MASK) >> LVR_MAX_LVT_SHIFT) + 1;
		unsigned hw_intr = lapic_intrs[0].cpu_intr_base;

		/* LAPIC timer */
		lapic_write(LAPIC_LTR,  (LVT_MASKED|LVT_TIMER_MODE_PERIODIC)|(hw_intr++));
		
		/* The next 2 registers are implementation dependent and we should not
		 * be writing to them unconditionally. There are 4 LVT entries defined
		 * in the Pentium family. P6 added the performance counter and P4/Xeon
		 * added the Thermal Monitor. If we expect only to run on processor
		 * with these registers we should ASSERT(num_lvt == 6)
		 * 
		 * NOTE: because of the way that the x86/intr.h interrupt defines for
		 * 		X86_INTR_APIC_* were done, and because we use the performance
		 * 		counters internally in cpu_perfregs.c and for the IDE
		 * 		(services/qconn), the order of assignment must be as follows
		 * 		to preserve backward compatibility. I sure hope we don't run
		 * 		across an LAPIC that does not implement these.
		*/
		if (num_lvt > 5) {
			/* Temperature */
			lapic_write(LAPIC_LTMR, (LVT_MASKED|LVT_DELIVERY_MODE_FIXED)|(hw_intr++));
		}
		if (num_lvt > 4) {
			/* Performance Counter */
			lapic_write(LAPIC_LPCR, (LVT_MASKED|LVT_DELIVERY_MODE_FIXED)|(hw_intr++));
		}

		/* LINT0 and LINT1 (in case a particular board wants to use them) */
		lapic_write(LAPIC_LL0R, (lapic_read(LAPIC_LL0R) & ~0xFFU) | hw_intr++);
		lapic_write(LAPIC_LL1R, (lapic_read(LAPIC_LL1R) & ~0xFFU) | hw_intr++);

		/* Error */
		lapic_write(LAPIC_LER,  (LVT_MASKED)|(hw_intr++));

		/* Spurious Vector */
		lapic_write(LAPIC_SVR,  SVR_APIC_ENABLED | APIC_SPURIOUS_INTERRUPT);

		/*
		 * we assume that this startup runs on machines implementing Virtual Wire Mode
		 * (should be all recent machines (its 2009) and that the LAPIC is effectively already
		 * configured in this mode. If this is not the case, we are in PIC mode and we
		 * need to get out of it by manipulating the IMCR register and since there is
		 * no code to do that we will assert
		*/
		/* FIX ME - it is possible that NMI could be connected to LINT0 so we need a better way of determining
		 * 			which LINT to mask (see ACPI Local NMI structure) */
		if (is_imcr_present()) {
			crash("IMCR register is implemented which means we are in unsupported PIC mode\n");
		} else if (cpu == 0) {
			/* EOI broadcast disable (bit 12 of the Version register) must not be disabled */
			ASSERT((lapic_read(LAPIC_SVR) & (1 << 12)) == 0);
		}

		DUMP_LAPIC(cpu);

		/* in all cases, the LINT0/LINT1 inputs for AP's must be masked
		 *
		 * For AMD Ryzen platform, The R/W LAPIC INT0/1 fields have the settings they do and cannot seem to be changed.
		 * More investigation is needed beyond just defeating the assertion.
		 */
		ASSERT((cpu == 0) || is_amd_ryzen() || (lapic_read(LAPIC_LL0R) & LVT_MASKED));
		ASSERT((cpu == 0) || is_amd_ryzen() || (lapic_read(LAPIC_LL1R) & LVT_MASKED));

#if 0
		/* FIX ME not sure if we care about the local interrupt sources from the MP Config table */
		{
			void *next = NULL;
			struct smp_intsrc intsrc;
			unsigned i = 0;
			kprintf("Search for LINTSRC's\n");
			while((next = ioapic_lintsrc_imps(-1, next, &intsrc)) != NULL)
			{
				kprintf("[%u] LAPIC %u intsrc: busid:%u, busintr:%u, dest intin:%u, type:%u, flags:%x\n", i++,
						intsrc.dest_apic_id, intsrc.source_bus_id, intsrc.source_bus_intr,
						intsrc.dest_apic_intin, intsrc.int_type, intsrc.flags & IMPS_INTSRC_FLAGS_MASK);
			}
		}
#endif
		lapic_intrs[0].num_vectors = hw_intr - lapic_intrs[0].cpu_intr_base;
	}
}

static apic_patch_data_t patch_data;

void
init_intrinfo_lapic(void) {
	unsigned	hw_intr;

	if(lapic_callout_vaddr != NULL) {
		hw_intr = find_next_cpu_intr();

		lapic_intrs[0].cpu_intr_base = hw_intr;
		if (debug_flag) kprintf("Base CPU interrupt for APIC is %u\n",hw_intr);

		patch_data.lapic = lapic_callout_vaddr;
		lapic_intrs[0].patch_data = &patch_data;
		lapic_intrs[1].patch_data = &patch_data;

		/* init_intrinfo_lapic() should only ever be called by the boot CPU (0)
		 * however there isn't a good way to test for that (apparently, cpu_starting
		 * should not be extern'd in) */
		lapic_configure(0);
		
		add_interrupt_array(lapic_intrs, sizeof(lapic_intrs));
	}
}
