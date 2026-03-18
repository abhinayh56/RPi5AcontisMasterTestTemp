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


#include <startup.h>
#include <x86/lapic.h>
#include <hw/acpi.h>
#include <x86/intel_misc.h>	/* for IOAPIC_PROFILE_EMPTY which is not really Intel specific */

static unsigned ioapic_acpi_config(const unsigned ioapic_id, ioapic_cfg_t *ioapic_cfg, const unsigned num);
static unsigned ioapic_imps_config(const unsigned ioapic_id, ioapic_cfg_t *ioapic_cfg, const unsigned num);

/*
 * ioapic_auto_config
 * 
 * This function will fill in the <ioapic_cfg> structure with no more than
 * <num> entries for the IOAPIC identified by <ioapic_id>. The actual number
 * entered will be returned.
 * 
 * The <ioapic_cfg> array is filled in from ACPI/MP COnfig tables. ACPI is used
 * first to select one of the Intel "default configurations" and then any interrupt
 * source overrides are applied. If no ACPI information can be found, the MP
 * Configuration interrupt sources are used.
 * 
 * IMPORTANT
 * Because this function is designed to extract the configuration from ACPI/MP
 * tables, it requires a BIOS/EFI or similar loader to set them up.
 * 
 * Startup can manually configure any IOAPIC's by filling out an 'ioapic_cfg_t'
 * structure and then calling an appropriate ioapic_configure_*() routine based
 * on the type of IOAPIC implemented
 * 
 * Returns: On success, <num> or fewer entries will be placed into the
 * 			<ioapic_cfg> array and the actual number will be returned. If more
 * 			than <num> entries could have been configured, the 2's complement of
 * 			the remaining entries is returned.
 * 			Ex. <num> == 10 indicating that <ioapic_cfg> has space for 10 entries
 * 				but it is determined that information for 12 entries exist, then
 * 				10 will be provided and -2 will be returned. If 8 entries exist
 * 				then 8 will be returned.
 * 			IMPORTANT: if the return value is < <num>, the <ioapic_cfg> array may
 * 						be sparsely populated. That is, there may not have been
 * 						an interrupt defined for each entry in numerical order.
 * 						In this case, the ioapic_cfg[].intsrc.apic_id will be 0
 * 						indicating the entry is invalid but the caller should find
 * 						valid entries (ie. ioapic_cfg[].intsrc.apic_id == ioapic_id)
 * 						equivalent to the return value.
*/
unsigned ioapic_auto_config(const unsigned ioapic_id, ioapic_cfg_t *ioapic_cfg, const unsigned num)
{
	const unsigned ioapic_first_ivec = find_next_cpu_intr();
	unsigned num_interrupts;
	unsigned i;
	

	/* validate <ioapic_id> */
	ASSERT((ioapic_id >= ioapic_get_id_first()) && (ioapic_id <= ioapic_get_id_last()));

	/* this ensures that entries are invalid to start (ie. ioapic_cfg[].intsrc.apic_id == 0) */
	memset(ioapic_cfg, 0, sizeof(*ioapic_cfg) * num);

	/* initialize CPU vectors and routing */
	for (i=0; i<num; i++)
	{
		ioapic_cfg[i].vector = ioapic_first_ivec + i;
		ioapic_cfg[i].cpu_mask = (1 << 0);	// CPU 0 only
		ioapic_cfg[i].masked = 1;	// masked by default
	}
	const unsigned int profile_num = ioapic_get_autofill_profile_num(ioapic_id);
	if (profile_num == IOAPIC_PROFILE_EMPTY) num_interrupts = 0;
	else
	{
		num_interrupts = ioapic_acpi_config(ioapic_id, ioapic_cfg, num);
		if (num_interrupts == 0) {
			num_interrupts = ioapic_imps_config(ioapic_id, ioapic_cfg, num);
		}
	}
	return num_interrupts;
}

static unsigned ioapic_acpi_config(const unsigned ioapic_id, ioapic_cfg_t *ioapic_cfg, const unsigned num)
{
	extern void *ioapic_intsrc_acpi(int ioapic_id, void *start, acpi_madt_interrupt_source_overrides *intsrc);
	unsigned num_interrupts;
	unsigned i;
	void *next = NULL;
	acpi_madt_interrupt_source_overrides intsrc;
	const unsigned int profile_num = ioapic_get_autofill_profile_num(ioapic_id);

	num_interrupts = ioapic_autofill_defaults(ioapic_id, ioapic_cfg, num, profile_num);

	if (debug_flag > 2) kprintf("Search for ACPI INTSRC's\n");
	i = 0;
	while((next = ioapic_intsrc_acpi(ioapic_id, next, &intsrc)) != NULL)
	{
		const unsigned gsi_base = ioapic_get_gsibase(ioapic_id);
		const unsigned entry = intsrc.Global_System_Interrupt - gsi_base;
		unsigned j, idx;

		if (debug_flag > 2) {
			kprintf("[%u] IOAPIC %u intsrc: busid:%u, busintr:%u, gsi:%u, flags:%x\n", i++,
					ioapic_id, intsrc.Bus, intsrc.Source, intsrc.Global_System_Interrupt,
					intsrc.Flags & IMPS_INTSRC_FLAGS_MASK);
		}

		/* make sure this override applies to <ioapic_id> (protects 'entry' as well) */
		ASSERT(intsrc.Global_System_Interrupt >= gsi_base);

		/*
		 * we can't assume that auto fill put the entries into the ioapic_cfg array
		 * in order. At the end of the loop, we have either found the ioapic_cfg
		 * index for the entry to override, or we have found the first available
		 * ioapic_cfg index at which to add a new entry.
		*/
		for (j=0, idx=num; j<num; j++) {
			if ((ioapic_cfg[j].intsrc.dest_apic_id == 0) && (idx > j)) idx = j;
			if ((ioapic_cfg[j].intsrc.dest_apic_id != 0) && (ioapic_cfg[j].intsrc.dest_apic_intin == entry)) break;
		}
		if (j < num) idx = j;	// override
		else ++num_interrupts;	// new entry

		ASSERT(idx < num);
		/* either found an entry to override (idx == j) or no space */
		ASSERT((idx == j) || (ioapic_cfg[idx].intsrc.dest_apic_id == 0));

		if (debug_flag > 2) kprintf("Overriding pin %u intsrc @ idx %u\n", entry, idx);
		ioapic_cfg[idx].intsrc.dest_apic_id = ioapic_id;	// in case its a new entry
		ioapic_cfg[idx].intsrc.dest_apic_intin = entry;
		ioapic_cfg[idx].intsrc.int_type = IMPS_INTSRC_INTTYPE_FIX;
		ioapic_cfg[idx].intsrc.source_bus_id = intsrc.Bus;
		ioapic_cfg[idx].intsrc.source_bus_intr = intsrc.Source;
		ioapic_cfg[idx].intsrc.flags = intsrc.Flags & IMPS_INTSRC_FLAGS_MASK;
	}
	
	return num_interrupts;
}

static unsigned ioapic_imps_config(const unsigned ioapic_id, ioapic_cfg_t *ioapic_cfg, const unsigned num)
{
	unsigned num_interrupts = 0;
	unsigned i;
	void *next = NULL;
	struct smp_intsrc intsrc;

	/* 
	 * there can be more entries in the MP Config table than there are pins
	 * (and hence entries) on the IOAPIC. This is because interrupts can,
	 * and often are, shared between multiple devices. This is reflected in
	 * multiple entries in the table for the same pin. This is ok, however
	 * the settings for these entries (edge/level, polarity, type, etc) must
	 * be the same or this is considered a MP Config table error which will
	 * be asserted on.
	 * 
	 * FIX ME
	 * Note that multiple entries will have to be added to the HWINFO section
	 * in order to convey the information to interested drivers 
	*/
	if (debug_flag > 2) kprintf("Search for IMPS INTSRC's\n");
	i = 0;
	while((next = ioapic_intsrc_imps(ioapic_id, next, &intsrc)) != NULL)
	{
		if (debug_flag > 2) {
			kprintf("[%u] IOAPIC %u intsrc: busid:%u, busintr:%u, dest intin:%u, type:%u, flags:%x\n", i++,
					intsrc.dest_apic_id, intsrc.source_bus_id, intsrc.source_bus_intr,
					intsrc.dest_apic_intin, intsrc.int_type, intsrc.flags & IMPS_INTSRC_FLAGS_MASK);
		}

		ASSERT(intsrc.dest_apic_id == ioapic_id);
		
		/* we can only add entries for interrupts requested */
		if (intsrc.dest_apic_intin < num)
		{
			/* an IOAPIC can never have an ID of 0 so we use this to indicate an unconfigured entry */
			if (ioapic_cfg[intsrc.dest_apic_intin].intsrc.dest_apic_id != 0)
			{
//				kprintf("Ignoring duplicate pin %u intsrc\n", intsrc.dest_apic_intin);
				/* a duplicate entry for a pin has been found. Make sure its not different */
				ASSERT(intsrc.int_type == ioapic_cfg[intsrc.dest_apic_intin].intsrc.int_type);
				ASSERT((intsrc.flags & IMPS_INTSRC_FLAGS_MASK) == ioapic_cfg[intsrc.dest_apic_intin].intsrc.flags);
			}
			else
			{
//				kprintf("Adding pin %u intsrc\n", intsrc.dest_apic_intin);
				ioapic_cfg[intsrc.dest_apic_intin].intsrc = intsrc;
				ioapic_cfg[intsrc.dest_apic_intin].intsrc.flags &= IMPS_INTSRC_FLAGS_MASK;
				++num_interrupts;
			}
		}
	}
	
	return num_interrupts;
}
