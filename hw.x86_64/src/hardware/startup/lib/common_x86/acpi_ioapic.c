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


/*
 * ioapic_discover_acpi
 *
 * Discover available IOAPIC's using information in ACPI tables.
 *
 * This routine will go through the ACPI tables and for every IOAPIC registered,
 * will add the IOAPIC using add_ioapic() based on its specified id. The IOAPIC
 * information can then be queried based on the IOAPIC ID.
 *
 * Returns: the number of IOAPIC's found
 *
*/
unsigned ioapic_discover_acpi(void)
{
	/* search acpi tables for IO-APIC information */
	unsigned len;
	paddr_t paddr;
	acpi_madt *madt;
	acpi_madt_generic *p;
	uintptr_t end;
	unsigned num = 0;
//	unsigned			num_headers = 0;

	paddr = acpi_find_table(ACPI_MADT_SIGNATURE, &len);
	if(paddr == NULL_PADDR) return 0;

	madt = startup_memory_map(len, paddr, PROT_READ);
	ASSERT(madt != NULL);

	end = (uintptr_t)madt + madt->hdr.Length;
	p = &madt->APIC[0];
	while((uintptr_t)p < end)
	{
		if (p->hdr.Type == ACPI_MADT_TYPE_IO_APIC)
		{
			unsigned ioapic_id = p->io_apic.IO_APIC_ID;
			const unsigned last_ioapic_id = ioapic_get_id_last();

			if (debug_flag > 1) {
				kprintf("ACPI header found for IO-APIC %u @ %P [base=%u]\n",
						ioapic_id, (paddr_t)p->io_apic.IO_APIC_Address, p->io_apic.Global_System_Interrupt_Base);
				if (ioapic_id > 0xf) {
					kprintf("IO-APIC id %u truncated to %u\n", ioapic_id, ioapic_id & 0xf);
				}
			}

			ioapic_id &= 0xf;

			if (ioapic_id == 0)
			{
				/*
				 * a value of zero is not valid for an IOAPIC id therefore it is
				 * likely because of the BIOS that the table contains this value. We cannot
				 * read from the hardware at this point so we will try and autogenerate
				 * a valid IOAPIC id based on the last IOAPIC added
				*/
				ioapic_id = last_ioapic_id + 1;
				if (debug_flag) kprintf("IO-APIC id 0 adjusted to %u\n", ioapic_id);
			}
			else if ((ioapic_id >= ioapic_get_id_first()) && (ioapic_id <= last_ioapic_id))
			{
				/*
				 * the IO-APIC ID has already been used so pick a new one. This can happen, for
				 * example, with multiple IO-APIC's and the first has an ACPI value of 0
				*/
				if (debug_flag) kprintf("IO-APIC ID %u already exists, adjusting to %u\n", ioapic_id, last_ioapic_id + 1);
				ioapic_id = last_ioapic_id + 1;
			}

			add_ioapic(ioapic_id, p->io_apic.IO_APIC_Address, p->io_apic.Global_System_Interrupt_Base, NULL);
			++num;
		}
//		++num_headers;
		p = (void *)((uintptr_t)p + p->hdr.Length);
	}
//	kprintf("checked %u headers\n", num_headers);
	startup_memory_unmap(madt);

	return num;
}

/*
 * ioapic_intsrc_acpi
 *
 * Retrieve the next interrupt source record from the ACPI table. The caller can
 * specify for which IOAPIC they are interested in by specifying an
 * <ioapic_id> >= 0.
 *
 * To retrieve the first entry for <ioapic_id>, pass NULL for <start>
 *
 * Returns: a token which can be passed as the <start> value in the next call
 * 			in order to retrieve the next interrupt source entry for <ioapic_id>
 * 			If the return value is NULL, there was no information and <*intsrc>
 *			should be considered to contain garbage
*/
void *ioapic_intsrc_acpi(int ioapic_id, void *start, acpi_madt_interrupt_source_overrides *intsrc)
{
	/* search acpi tables for Interrupt source override information */
	unsigned len;
	paddr_t paddr;
	acpi_madt *madt;
	acpi_madt_generic *p;
	uintptr_t end;

	paddr = acpi_find_table(ACPI_MADT_SIGNATURE, &len);
	if(paddr == NULL_PADDR) return 0;

	madt = startup_memory_map(len, paddr, PROT_READ);
	ASSERT(madt != NULL);

	end = (uintptr_t)madt + madt->hdr.Length;
	p = (start == NULL) ? &madt->APIC[0] : (acpi_madt_generic *)start;
	while((uintptr_t)p < end)
	{
		if (p->hdr.Type == ACPI_MADT_TYPE_INTERRUPT_SOURCE_OVERRIDE)
		{
			/* skip over entries before <start> */
			if ((void *)p > start)
			{
				unsigned found = 0;

				if (debug_flag > 1) {
					kprintf("ACPI header found for Interrupt source override: bus:%u, SrcIRQ:%u, GSI:%u, Flags:%x\n",
							p->interrupt_sources_override.Bus, p->interrupt_sources_override.Source,
							p->interrupt_sources_override.Global_System_Interrupt, p->interrupt_sources_override.Flags);
				}
				/* determine whether the override applies to this IOAPIC */
				if (ioapic_id >= 0)
				{
					const ioapic_accessfnc_t *ioapic_fnc = ioapic_get_accessfnc(ioapic_id);
					const unsigned gsi_first = ioapic_get_gsibase(ioapic_id);
					ASSERT(ioapic_fnc != NULL);
					const unsigned gsi_last = ioapic_fnc->num_entries(ioapic_id) + gsi_first - 1;

					found = (p->interrupt_sources_override.Global_System_Interrupt >= gsi_first) &&
							(p->interrupt_sources_override.Global_System_Interrupt <= gsi_last);
				}
				if ((ioapic_id < 0) || found)
				{
					if (intsrc != NULL)
					{
						*intsrc = p->interrupt_sources_override;

						/* better not have reserved values or BIOS is screwed up */
						ASSERT(((p->interrupt_sources_override.Flags & 0xC) != 0x8) && ((p->interrupt_sources_override.Flags & 0x3) != 0x2));
						/* at this point we will resolve any bus defined trigger/polarity */
						if (((p->interrupt_sources_override.Flags & 0xC) == 0) || ((p->interrupt_sources_override.Flags & 0x3) == 0)) {
							if (debug_flag > 2) kprintf("** NEED A RESOLVER for ACPI INTSRC FLAGS **\n");
							//imps_resolve_bus_int_flags(fps->config + sizeof(cfg.hdr), intsrc);
						}
					}
					startup_memory_unmap(madt);
					/* return the location of the next header */
					return (void *)((uintptr_t)p + p->hdr.Length);
				}
			}
		}
		p = (void *)((uintptr_t)p + p->hdr.Length);
	}
	startup_memory_unmap(madt);

	return NULL;
}
