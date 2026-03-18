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
#include <x86/imps.h>


/*
 * ioapic_discover_imps
 * 
 * Discover available IOAPIC's using information in MP Configuration table.
 * 
 * This routine will go through the MP Configuration table and for every IOAPIC
 * registered, will add the IOAPIC using add_ioapic() based on its specified id.
 * The IOAPIC information can then be queried based on the IOAPIC ID.  
 * 
 * Returns: the number of IOAPIC's found
 * 
*/
unsigned ioapic_discover_imps(void)
{
	struct smp_fps *fps = smp_get_fps();
	unsigned n;
	unsigned num = 0;
	PADDR_T addr;
	ssize_t addr_incr;
	union
	{
		uint8_t 				type;
		struct smp_hdr			hdr;
		struct smp_ioapic		ioapic;
		struct smp_intsrc		intsrc;
		struct smp_lintsrc		lintsrc;
	} cfg;

	if ((fps == NULL) || (fps->config == 0))
	{
		if (debug_flag) kprintf("MP configuration table is missing.\n");
		return 0;
	}

	/* FIX ME - why can we not dereference fps directly ? */
	addr = (PADDR_T)fps->config;
	memmove(&cfg.hdr, (void *)(uintptr_t)addr, sizeof(cfg.hdr));

	addr = (PADDR_T)fps->config + sizeof(cfg.hdr);
	for(n = cfg.hdr.entry_count ; n > 0 ; --n)
	{
		memmove(&cfg, (void *)addr, sizeof(cfg));
		if (cfg.type == MP_IOAPIC)
		{
			if (debug_flag > 1) {
				kprintf("IMPS header found for IO-APIC %u @ %P [ver=%u, flags: %x]\n",
						cfg.ioapic.id, (paddr_t)cfg.ioapic.addr, cfg.ioapic.ver, cfg.ioapic.flags);
			}
			/* we only add the IOAPIC if it is marked enabled */
			if (cfg.ioapic.flags & 1)
			{
				add_ioapic(cfg.ioapic.id, cfg.ioapic.addr, -1, NULL);
				++num;
			}
		}

		if((addr_incr = imps_entry_size(cfg.type)) < 0)
		{
			kprintf("Unknown IMPS entry type %d.\n", cfg.type);
			break;
		}
		addr += imps_entry_size(cfg.type);
	}
	return num;
}

/*
 * ioapic_intsrc_imps
 * 
 * Retrieve the next interrupt source record from the MP Config table. The
 * caller can specify for which IOAPIC they are interested in by specifying an
 * <ioapic_id> >= 0.
 * 
 * To retrieve the first entry for <ioapic_id>, pass NULL for <start>
 * 
 * Returns: a token which can be passed as the <start> value in the next call
 * 			in order to retrieve the next interrupt source entry for <ioapic_id>
 * 			If the return value is NULL, there was no information and <*intsrc>
 * 			should be considered to contain garbage
 * 
 * Implementation Note
 * 
 * Currently the <start> value is interpreted as a count into the
 * cfg.hdr.entry_count number of entries in the MP Config table and we simply
 * skip over entries until we get to the <start> point.
 * I did not feel comfortable using the 'addr' as the cookie since I am not sure
 * why we do the memmove() yet and don't simply dereference 'fps'. Given that we
 * rescan, for 'fps', the 'addr' value may be stale on a subsequent call.
 * Regardless, the 'addr' is not enough either since we would also need to know
 * what the 'entry number' that we returned was in order to subtract it from 'n'
 * in the loop else we would overrun the table.
*/
static void *_ioapic_intsrc_imps(unsigned entry_type, int ioapic_id, void *start, struct smp_intsrc *intsrc);

void *ioapic_intsrc_imps(int ioapic_id, void *start, struct smp_intsrc *intsrc)
{
	return _ioapic_intsrc_imps(MP_INTSRC, ioapic_id, start, intsrc);
}
void *ioapic_lintsrc_imps(int ioapic_id, void *start, struct smp_intsrc *intsrc)
{
	return _ioapic_intsrc_imps(MP_LINTSRC, ioapic_id, start, intsrc);
}

static void *_ioapic_intsrc_imps(unsigned entry_type, int ioapic_id, void *start, struct smp_intsrc *intsrc)
{
	struct smp_fps *fps = smp_get_fps();
	unsigned n;
	uintptr_t num = 0;
	PADDR_T addr;
	ssize_t addr_incr;
	uintptr_t start_entry = (uintptr_t)start;
	union
	{
		uint8_t 				type;
		struct smp_hdr			hdr;
		struct smp_ioapic		ioapic;
		struct smp_intsrc		intsrc;
		struct smp_lintsrc		lintsrc;
	} cfg;

	if ((fps == NULL) || (fps->config == 0))
	{
		kprintf("MP configuration table is missing.\n");
		return 0;
	}

	/* FIX ME - why can we not dereference fps directly ? */
	addr = (PADDR_T)fps->config;
	memmove(&cfg.hdr, (void *)(uintptr_t)addr, sizeof(cfg.hdr));

	addr = (PADDR_T)fps->config + sizeof(cfg.hdr);
	for(n = cfg.hdr.entry_count ; n > 0 ; --n)
	{
		memmove(&cfg, (void *)addr, sizeof(cfg));
		if (cfg.type == entry_type)
		{
			++num;
			if (num > start_entry)
			{
				if (debug_flag > 1) {
					kprintf("IMPS header found for Interrupt Source\n");
				}

				if ((ioapic_id < 0) || (cfg.intsrc.dest_apic_id == ioapic_id))
				{
					if (intsrc != NULL)
					{
						*intsrc = cfg.intsrc;
						
						/* better not have reserved values or BIOS is screwed up */
						ASSERT(((intsrc->flags & 0xC) != 0x8) && ((intsrc->flags & 0x3) != 0x2));
						/* at this point we will resolve any bus defined trigger/polarity */
						if (((intsrc->flags & 0xC) == 0) || ((intsrc->flags & 0x3) == 0)) {
							imps_resolve_bus_int_flags(fps->config + sizeof(cfg.hdr), intsrc);
						}
					}
					return (void *)num;
				}
			}
		}
		if((addr_incr = imps_entry_size(cfg.type)) < 0)
		{
			kprintf("Unknown IMPS entry type %d.\n", cfg.type);
			return (void *)0;
		}
		addr += imps_entry_size(cfg.type);
	}
	return NULL;
}

/*
 * resolve_bus_int_flags
 * 
 * Search the MP Config table for <intsrc>->source_bus_id and resolve the trigger
 * mode and polarity of the interrupt identified by <intsrc>->source_bus_intr
 * 
 * <addr> is the start of the MP Config table and saves us having to rescan for it 
*/
void imps_resolve_bus_int_flags(PADDR_T addr, struct smp_intsrc *intsrc)
{
	unsigned n;
	ssize_t addr_incr;
	union
	{
		uint8_t 				type;
		struct smp_hdr			hdr;
		struct smp_bus			bus;
	} cfg;
	

	// Major kludge here. The cfg.hdr.entry_count wasn't being set
	// before the for loop, but this routine wasn't passed the fps pointer.
	// Since the 'addr' passes is +sizeof(cfg.hdr) away from what we
	// want, back it up to get the information we need
	memmove(&cfg.hdr, (void *)(addr - sizeof(cfg.hdr)), sizeof(cfg.hdr));

	if (debug_flag > 1) kprintf("Searching for bus id %u\n",intsrc->source_bus_id);
	for(n = cfg.hdr.entry_count ; n > 0 ; --n)
	{
		memmove(&cfg, (void *)addr, sizeof(cfg));
		
		if ((cfg.type == MP_BUS) && (cfg.bus.id == intsrc->source_bus_id))
		{
			if (debug_flag > 1) kprintf("found a bus entry for id %u\n", cfg.bus.id);
			if (memcmp(cfg.bus.bus_type, "PCI", sizeof("PCI") - 1) == 0)
			{
//kprintf("PCI bus interrupt flags changed from 0x%x to ", intsrc->flags);	
				/* PCI interrupts are active low, level */
				intsrc->flags = 0xF;
//kprintf("0x%x\n", intsrc->flags);
			}
			else if (memcmp(cfg.bus.bus_type, "EISA", sizeof("EISA") - 1) == 0)
			{
//kprintf("EISA bus interrupt flags changed from 0x%x to ", intsrc->flags);
				/* EISA has active low for level interrupts */
				if ((intsrc->flags & 0xC) == 0xC) intsrc->flags |= 0x3;
				/* FIX ME - what about edge triggered */
//kprintf("0x%x\n", intsrc->flags);
			}
			else if (memcmp(cfg.bus.bus_type, "ISA", sizeof("ISA") - 1) == 0)
			{
//kprintf("ISA bus interrupt flags changed from 0x%x to ", intsrc->flags);
				/* ISA has edge triggered interrupts (hopefully the polarity is set) */
				intsrc->flags = 0x4 | (intsrc->flags & 0x3);
//kprintf("0x%x\n", intsrc->flags);
			}
			else
			{
				kprintf("Unknown Bus: %c%c%c%c%c%c\n",
						cfg.bus.bus_type[0], cfg.bus.bus_type[1],
						cfg.bus.bus_type[2], cfg.bus.bus_type[3],
						cfg.bus.bus_type[4], cfg.bus.bus_type[5]);
				ASSERT(0);
			}
			return;
		}
		if((addr_incr = imps_entry_size(cfg.type)) < 0)
		{
			if (debug_flag > 1) kprintf("Unknown IMPS entry type %d.\n", cfg.type);
			break;
		}
		addr += imps_entry_size(cfg.type);
	}
	if (debug_flag > 1) kprintf("Not found\n");
}
