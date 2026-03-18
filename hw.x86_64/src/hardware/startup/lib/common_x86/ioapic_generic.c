/*
 * $QNXLicenseC:
 * Copyright 2010, 2020 QNX Software Systems.
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
 * Non Intel IOAPIC specific configuration
 *
 * This file currently does not do anything special for non Intel chipsets
 * because I don't have any docs but what it does not do is any of the Intel
 * specific manipulations that are done in ioapic_ich.c. Specifically, the
 * existence of an OIC register or equivalent (used to enable/disable the IOAPIC
 * among other things) is unknown so we must assume that the IOAPIC has already
 * been enabled by the BIOS
*/


#include <startup.h>
#include <x86/intr.h>
#include <x86/lapic.h>
#include <hw/acpi.h>
#include <x86/imps.h>


typedef enum
{
	ioapic_reg_ID			= 0x00,
	ioapic_reg_VERSION		= 0x01,
	ioapic_reg_REDIR_TBL_0	= 0x10,

} ioapic_reg_index_e;

#define ioapic_reg_REDIR_TBL(n)	(ioapic_reg_REDIR_TBL_0 + (2 * (n)))

static uint64_t ioapic_read_entry(const PADDR_T ioapic_vaddr, unsigned entry_num);
static void ioapic_write_entry(const PADDR_T ioapic_vaddr, unsigned entry_num, uint64_t data);
static unsigned ioapic_num_entries(const PADDR_T ioapic_vaddr);
static unsigned ioapic_get_id(const PADDR_T ioapic_vaddr);
static void ioapic_set_id(const PADDR_T ioapic_vaddr, unsigned ioapic_id);
static uint32_t ioapic_read(const PADDR_T addr, unsigned reg_num);
static void ioapic_write(const PADDR_T addr, unsigned reg_num, uint32_t data);
static void enable_ioapic(void);

#ifdef APIC_DEBUG
static unsigned ioapic_version(const PADDR_T ioapic_vaddr);
/* dump the IOAPIC registers */
static void dump_ioapic(const PADDR_T ioapic_paddr)
{
	unsigned i;

	kprintf("\nNon Intel IOAPIC register dump\n-------------------------\n");
	kprintf("ID: %x\n", ioapic_get_id(ioapic_paddr));
	kprintf("VERSION: %x\n", ioapic_version(ioapic_paddr));
	kprintf("REDIR TABLE (%u entries)\n", ioapic_num_entries(ioapic_paddr));
	for (i=0; i<ioapic_num_entries(ioapic_paddr); i++) {
//		uint64_t redir_val = ioapic_read_entry(ioapic_paddr, i);
		if ((i % 3) == 0) kprintf("\n");
//		kprintf("[%u]: %L   ", i, redir_val);
		kprintf("[%u]: %x %x   ", i, ioapic_read(ioapic_paddr, ioapic_reg_REDIR_TBL(i)+1),  ioapic_read(ioapic_paddr, ioapic_reg_REDIR_TBL(i)));
	}
	kprintf("\n");
}
#define DUMP_IOAPIC(__paddr__)	dump_ioapic((__paddr__))
#else	/* APIC_DEBUG */
#define DUMP_IOAPIC(__paddr__)
#endif	/* APIC_DEBUG */

/*
 * ioapic_configure_generic
 *
 * Non Intel Southbridge specific IOAPIC configuration. If <ioapic_cfg> is not
 * NULL, it must point to a <num> element array of 'ioapic_cfg_t' entries. There
 * does not need to be a 1:1 correspondence between the number of IOAPIC entries
 * and <num>. That is, the caller can specify only values for those interrupt
 * entries to be modified (as specified by 'ioapic_cfg[i].intsrc.dest_apic_intin')
 * however the caller must also ensure that the unspecified entries do not
 * conflict with any entries being modified.
 *
 * An entry in the ioapic_cfg[] array with an intsrc.dest_apic_id == 0 will be
 * ignored. This is to accommodate the array as returned from ioapic_auto_config()
 *
 * if <ioapic_cfg> == NULL and <num> == -1, all the IOAPIC entries will be
 * cleared and masked
 *
 * If <ioapic_cfg> is NULL and <num> != -1, then no interrupts will be configured.
 *
 * Returns:	the number of interrupts configured
 *
 * NOTE:
 *
 * The current crop of PCH bridges (Series 5/3400 series) utilize this file as well
*/
static unsigned configure(const unsigned ioapic_id, const ioapic_cfg_t *ioapic_cfg, const int num)
{
	unsigned i;
	const PADDR_T ioapic_paddr = ioapic_get_paddr(ioapic_id);
	unsigned num_interrupts;
	unsigned num_configured = 0;
	unsigned const bsp_apic_id	= lapic_id();

	if (ioapic_paddr == NULL_PADDR_STARTUP)
	{
		kprintf("Expected Non Intel IOAPIC %u not found\n", ioapic_id);
		return 0;
	}

	/* now we can enable the IOAPIC */
	enable_ioapic();

	num_interrupts = ioapic_num_entries(ioapic_paddr);

	if ((ioapic_cfg == NULL) && (num != -1)) return 0;
	else if ((ioapic_cfg == NULL) && (num == -1))
	{
		if (debug_flag > 1) kprintf("\nClearing %u IOAPIC entries\n", num_interrupts);
		/* mask each entry */
		for (i=0; i<num_interrupts; i++) {
			ioapic_write_entry(ioapic_paddr, i, (1ULL << 16));
		}
		DUMP_IOAPIC(ioapic_paddr);
		return num_interrupts; 
	}
	else if (num > (int)num_interrupts)
	{
		kprintf("Only %u interrupts available, not %u\n", ioapic_num_entries(ioapic_paddr), num);
		return 0;
	}

	/* we now configure the IOAPIC based on the interrupt source information */
	ioapic_set_id(ioapic_paddr, ioapic_id);

	if (debug_flag > 1) kprintf("\nConfiguring %u IOAPIC entries\n", num);
	for(i=0; i<num; i++)
	{
		if (ioapic_cfg[i].intsrc.dest_apic_id == ioapic_id)
		{
			/* this table maps ioapic_cfg[i].intsrc.int_type types to interrupt types */
			static const unsigned _inttype_tbl[] = {
					[IMPS_INTSRC_INTTYPE_FIX] = 0,
					[IMPS_INTSRC_INTTYPE_NMI] = 4,
					[IMPS_INTSRC_INTTYPE_SMI] = 2,
					[IMPS_INTSRC_INTTYPE_EXT] = 7,
			};
			const uint32_t int_mode = _inttype_tbl[ioapic_cfg[i].intsrc.int_type & 0x3];
			const uint64_t redir_entry =
					  ((uint64_t)bsp_apic_id << 56)					// Boot CPU gets all interrupts for now
					| ((ioapic_cfg[i].masked) ? (1ULL << 16) : 0)			// interrupt is masked ?
					| (((ioapic_cfg[i].intsrc.flags & IMPS_INTSRC_TRIG_MASK) == IMPS_INTSRC_TRIG_LEVEL) ? (1 << 15) : 0)	// trigger mode
					| (((ioapic_cfg[i].intsrc.flags & IMPS_INTSRC_ACTIVE_MASK) == IMPS_INTSRC_ACTIVE_LO) ? (1 << 13) : 0)	// polarity
					| (0ULL << 11)											// physical destination mode
					| (int_mode << 8)										// delivery mode
					| (ioapic_cfg[i].vector	& 0xFFU);						// assigned vector			

#if 0
		//there are no bus entries in the MP Config table so can't patch up the INT trigger/level
		/* we expect that the trigger mode and polarity is fully resolved to edge/level and high/low */
			ASSERT((ioapic_cfg[i].intsrc.flags & 0x5) == 0x5);
#endif

			if (debug_flag > 1) {
				kprintf("[%u] vec: %u, %s cpumask:%x, ID: %u, busid:%u, busintr:%u, dest intin:%u, type:%u, flags:%x\n", i,
						ioapic_cfg[i].vector, ioapic_cfg[i].masked ? " " : "*", ioapic_cfg[i].cpu_mask,
						ioapic_cfg[i].intsrc.dest_apic_id, ioapic_cfg[i].intsrc.source_bus_id,
						ioapic_cfg[i].intsrc.source_bus_intr, ioapic_cfg[i].intsrc.dest_apic_intin,
						ioapic_cfg[i].intsrc.int_type, ioapic_cfg[i].intsrc.flags & IMPS_INTSRC_FLAGS_MASK);
			}
			ioapic_write_entry(ioapic_paddr, ioapic_cfg[i].intsrc.dest_apic_intin, redir_entry);
			++num_configured;
		}
	}
	if (debug_flag > 1) kprintf("Interrupts routed to Boot CPU (APIC ID 0x%x) by default\n", bsp_apic_id);
	DUMP_IOAPIC(ioapic_paddr);

	return num_configured;
}

static unsigned num_interrupts(const unsigned ioapic_id)
{
	const PADDR_T ioapic_paddr = ioapic_get_paddr(ioapic_id);
	return (ioapic_paddr == NULL_PADDR_STARTUP) ? 0 : ioapic_num_entries(ioapic_paddr);
}

static uint64_t read_entry(const unsigned ioapic_id, const unsigned entry_num)
{
	const PADDR_T ioapic_paddr = ioapic_get_paddr(ioapic_id);

	if ((ioapic_paddr == NULL_PADDR_STARTUP) || (entry_num >= ioapic_num_entries(ioapic_paddr))) return 0;
	else return ioapic_read_entry(ioapic_paddr, entry_num);
}

/* return 0 on success, otherwise -1 */
static int write_entry(const unsigned ioapic_id, const unsigned entry_num, const uint64_t data)
{
	const PADDR_T ioapic_paddr = ioapic_get_paddr(ioapic_id);

	if ((ioapic_paddr == NULL_PADDR_STARTUP) || (entry_num >= ioapic_num_entries(ioapic_paddr))) {
		return -1;
	}
	ioapic_write_entry(ioapic_paddr, entry_num, data);
	return 0;
}


static ioapic_accessfnc_t _ioapic_accessfncs_generic =
{
	.num_entries = num_interrupts,
	.read_entry = read_entry,
	.write_entry = write_entry,
	.configure = configure,
};
ioapic_accessfnc_t *ioapic_accessfncs_generic = &_ioapic_accessfncs_generic;


/*
 * =============================================================================
 *                 M I S C   S U P P O R T   R O U T I N E S
 * =============================================================================
*/


static void enable_ioapic(void)
{
}

static unsigned ioapic_num_entries(const PADDR_T ioapic_paddr)
{
	ASSERT(ioapic_paddr != NULL_PADDR_STARTUP);
	return ((ioapic_read(ioapic_paddr, ioapic_reg_VERSION) & 0x00FF0000) >> 16) + 1;
}

static unsigned ioapic_get_id(const PADDR_T ioapic_paddr)
{
	return (ioapic_read(ioapic_paddr, ioapic_reg_ID) & 0x0F000000) >> 24;
}

static void ioapic_set_id(const PADDR_T ioapic_paddr, unsigned ioapic_id)
{
	/* set the id preserving all other bits */
	uint32_t id_reg = ioapic_read(ioapic_paddr, ioapic_reg_ID) & ~0x0F000000;
	ioapic_write(ioapic_paddr, ioapic_reg_ID, ((ioapic_id & 0xF) << 24) | id_reg);
	ASSERT(ioapic_get_id(ioapic_paddr) == ioapic_id);
}

#ifdef APIC_DEBUG
static unsigned ioapic_version(const PADDR_T ioapic_paddr)
{
	return ioapic_read(ioapic_paddr, ioapic_reg_VERSION) & 0x000000FF;
}
#endif	/* APIC_DEBUG */

#define IOAPIC_ENTRY_RESERVED_AND_RO_BITS \
		( \
			(((1ULL << (55 - 17 + 1)) - 1) << 17) |	/* bits [55..17] 0x7F FFFF FFFF */ \
			(1ULL << 12)							/* bit 12 */ \
		)
static uint64_t ioapic_read_entry(const PADDR_T ioapic_paddr, unsigned entry_num)
{
	// FIX ME - this function just does not work
	const unsigned reg_num = ioapic_reg_REDIR_TBL(entry_num);
	uint64_t data;

	data = ioapic_read(ioapic_paddr, reg_num + 1);	// MSW
	data <<= 32;
	data |= ioapic_read(ioapic_paddr, reg_num);		// LSW

	/* mask out reserved and RO bits */
	data &= ~IOAPIC_ENTRY_RESERVED_AND_RO_BITS;
	return data;
}


static void ioapic_write_entry(const PADDR_T ioapic_paddr, unsigned entry_num, uint64_t data)
{
	const unsigned reg_num = ioapic_reg_REDIR_TBL(entry_num);
	const uint64_t curr_redir_entry = ioapic_read_entry(ioapic_paddr, entry_num)
									& IOAPIC_ENTRY_RESERVED_AND_RO_BITS;

	/* preserve reserved and RO bits */
	data &= ~IOAPIC_ENTRY_RESERVED_AND_RO_BITS;
	data |= curr_redir_entry;

	/* write the MSW first in case the LSW has the mask bit clear */ 
	ioapic_write(ioapic_paddr, reg_num + 1, data >> 32);
	ioapic_write(ioapic_paddr, reg_num, data & 0xFFFFFFFFU);
}

/* can only ever read 32 bits at a time */
static uint32_t ioapic_read(const PADDR_T addr, unsigned reg_num)
{
	volatile uint32_t *const reg = startup_memory_map(0x20, addr, PROT_READ|PROT_WRITE|PROT_NOCACHE);
	reg[0] = reg_num & 0xFF;
	uint32_t const data = reg[4];
	startup_memory_unmap((void *)reg);
//kprintf("read %x from reg %u\n", data, reg_num);
	return data;
}


/* can only ever write 32 bits at a time */
static void ioapic_write(const PADDR_T addr, unsigned reg_num, uint32_t data)
{
	volatile uint32_t *const reg = startup_memory_map(0x20, addr, PROT_READ|PROT_WRITE|PROT_NOCACHE);
	reg[0] = reg_num & 0xFF;
	reg[4] = data;
	startup_memory_unmap((void *)reg);
//kprintf("write %x to reg %u\n", data, reg_num);
}
