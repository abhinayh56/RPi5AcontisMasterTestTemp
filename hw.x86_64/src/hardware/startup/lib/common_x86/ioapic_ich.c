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

// Intel ICH IOAPIC specific configuration



#include <startup.h>
#include <x86/lapic.h>
#include <hw/acpi.h>
#include <x86/imps.h>
#include "ich_chipset.h"
#include <x86/pci_misc.h>


typedef enum
{
	ich_ioapic_reg_ID			= 0x00,
	ich_ioapic_reg_VERSION		= 0x01,
	ich_ioapic_reg_REDIR_TBL_0	= 0x10,
	
} ich_ioapic_reg_index_e;

#define ich_ioapic_reg_REDIR_TBL(n)	(ich_ioapic_reg_REDIR_TBL_0 + (2 * (n)))

static uint64_t ioapic_read_entry(const PADDR_T ioapic_vaddr, unsigned entry_num);
static void ioapic_write_entry(const PADDR_T ioapic_vaddr, unsigned entry_num, uint64_t data);
static unsigned ioapic_num_entries(const PADDR_T ioapic_vaddr);
static unsigned ioapic_get_id(const PADDR_T ioapic_vaddr);
static void ioapic_set_id(const PADDR_T ioapic_vaddr, unsigned ioapic_id);
static uint32_t ioapic_read(const PADDR_T addr, unsigned reg_num);
static void ioapic_write(const PADDR_T addr, unsigned reg_num, uint32_t data);
static void enable_ioapic(void);
//static void disable_ioapic(void);
//static int update_paddr_offset(const PADDR_T ioapic_paddr);
static int check_paddr_offset(const PADDR_T ioapic_paddr);

#ifdef APIC_DEBUG
static unsigned ioapic_version(const PADDR_T ioapic_vaddr);
/* dump the IOAPIC registers */
static void dump_ioapic(const PADDR_T ioapic_paddr)
{
	unsigned i;

	kprintf("\nICH IOAPIC register dump\n-------------------------\n");
	kprintf("ID: %x\n", ioapic_get_id(ioapic_paddr));
	kprintf("VERSION: %x\n", ioapic_version(ioapic_paddr));
	kprintf("REDIR TABLE (%u entries)\n", ioapic_num_entries(ioapic_paddr)); 
	for (i=0; i<ioapic_num_entries(ioapic_paddr); i++) {
//		uint64_t redir_val = ioapic_read_entry(ioapic_paddr, i);
		if ((i % 3) == 0) kprintf("\n");
//		kprintf("[%u]: %L   ", i, redir_val);
		kprintf("[%u]: %x %x   ", i, ioapic_read(ioapic_paddr, ich_ioapic_reg_REDIR_TBL(i)+1),  ioapic_read(ioapic_paddr, ich_ioapic_reg_REDIR_TBL(i)));
	}
	kprintf("\n");
}
#define DUMP_IOAPIC(__paddr__)	dump_ioapic((__paddr__))
#else	/* APIC_DEBUG */
#define DUMP_IOAPIC(__paddr__)
#endif	/* APIC_DEBUG */

/*
 * configure
 * 
 * ICH Southbridge specific IOAPIC configuration. If <ioapic_cfg> is not NULL,
 * it must point to a <num> element array of 'ioapic_cfg_t' entries. There does
 * not need to be a 1:1 correspondence between the number of IOAPIC entries and
 * <num>. That is, the caller can specify only values for those interrupt
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
	unsigned const bsp_apic_id	= lapic_id();
	unsigned i;
	const PADDR_T ioapic_paddr = ioapic_get_paddr(ioapic_id);
	unsigned num_interrupts;
	unsigned num_configured = 0;

	if (ioapic_paddr == NULL_PADDR_STARTUP)
	{
		kprintf("Expected ICH IOAPIC %u not found\n", ioapic_id);
		return 0;
	}

	/*
	 * before enabling the IOAPIC, validate its physical address. Starting with
	 * the ICH8, the IOAPIC base address may have an offset applied via the OIC
	 * register. The offset value affects IOAPIC base address bits 15:12 (for
	 * the ICH8, ICH9 and ICH10 consumer) or 19:12 (for the ICH10 corporate).
	 * If required, and possible, the OIC register will be updated to reflect
	 * the physical address specified for <ioapic_id>. If the physical address
	 * is not possible, we return 0, essentially indicating a configuration error
	 */
	if (check_paddr_offset(ioapic_paddr) != 0) {
		if (debug_flag > 0) kprintf("IOAPIC ID%u @ %x is not within the OIC[ASEL] range\n", ioapic_id, ioapic_paddr);
		return 0;
	}

	/*
	 * Starting with ICH9, and to support I/O virtualization, the IOAPIC may be
	 * assigned a unique Bus/Device/Function number. We don't utilize this (yet)
	 * but will ensure it is set to its default value of B0:D31:F0
	 */
	if (ich_model(NULL) >= 9)
	{
		uint16_t val = GET_LPC_REG16(0x6c);
		if (val != 0x00F8U)
		{
			SET_LPC_REG16(0x6c, 0x00F8U);
			if (debug_flag) {
				kprintf("Restored IOAPIC B:D:F to 0:31:0 from %u:%u:%u\n",
						val >> 8, (val & 0xF8U) >> 3, val & 0x7);
			}
		}
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
		kprintf("Only %u ICH interrupts available, not %u\n", ioapic_num_entries(ioapic_paddr), num);
		return 0;
	}

	/* we now configure the IOAPIC based on the interrupt source information */
	ioapic_set_id(ioapic_paddr, ioapic_id);

	if (debug_flag > 1) kprintf("\nConfiguring %u IOAPIC entries\n", num);
	for(i=0; i<num; i++)
	{
		if (ioapic_cfg[i].intsrc.dest_apic_id == ioapic_id)
		{
			/* this table maps ioapic_cfg[i].intsrc.int_type types to ICH interrupt types */
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


static ioapic_accessfnc_t _ioapic_accessfncs_ich = 
{
	.num_entries = num_interrupts,
	.read_entry = read_entry,
	.write_entry = write_entry,
	.configure = configure,
};
ioapic_accessfnc_t *ioapic_accessfncs_ich = &_ioapic_accessfncs_ich;


/*
 * =============================================================================
 *                 M I S C   S U P P O R T   R O U T I N E S
 * =============================================================================
*/

/*
 * starting with the ICH8, Intel documents this for the OIC[AEN] bit
 *
 * 		NOTE: SW should read this register after modifying APIC enable bit prior
 * 			  to access to the IOxAPIC address range.
 *
 * This function ensures the read is done and not optimized out from the caller
*/
static volatile uint8_t read_oic_reg(volatile uint8_t *oic_reg)
{
	return *oic_reg;
}

static void enable_ioapic(void)
{
	/* IOAPIC enable is in bit 0 of the OIC register @ rcba + offset 0x31ff */
	volatile uint8_t *oic_reg = startup_memory_map(1, ich_get_rcba() + 0x31ff, PROT_READ|PROT_WRITE|PROT_NOCACHE);

//	kprintf("OIC is 0x%x\n", *oic_reg);
	*oic_reg |= 0x1;	// enable the IOAPIC
	read_oic_reg(oic_reg);
	startup_memory_unmap((void *)oic_reg);
}

#if 0
update_paddr_offset() has currently been substituted with check_paddr_offset().
As a consequence, disable_ioapic() is not required either

static void disable_ioapic(void)
{
	volatile uint8_t *oic_reg = startup_memory_map(1, ich_get_rcba() + 0x31ff, PROT_READ|PROT_WRITE|PROT_NOCACHE);

//	kprintf("OIC is 0x%x\n", *oic_reg);
	*oic_reg &= ~0x1;	// disable the IOAPIC
	read_oic_reg(oic_reg);
	startup_memory_unmap((void *)oic_reg);
}

/*
 * update_paddr_offset
 *
 * Ensure that the physical address programmed for the IOAPIC is consistent with the
 * OIC[ASEL] (address range select).
 *
 * For all ICH's prior to the ICH8, there is no ASEL field in the OIC register and the
 * offset is assumed to be 0x0
 * For the ICH8, ICH9 and consumer ICH10, the OIC register contains a 4 bit address
 * range select field in the 8 bit OIC register at offset 0x31FF.
 * For the corporate ICH10 and newer, the OIC register is 16 bits and the address
 * range select field is 8 bits and at offset 0x31FE (LE) and the upper 8 bits of the
 * OIC register is similar to pre ICH8 for AEN and CEN fields.
 *
 * return 0 on success, otherwise -1 (IOAPIC will be disabled in both cases)
*/
static int update_paddr_offset(const PADDR_T ioapic_paddr)
{
	unsigned offset_mask = 0xFU;	// 4 bits by default
	unsigned offset_shift = 4;		// 4 bit mask starts at bit 4 by default
	volatile uint8_t *oic_reg;
	uint16_t desired_offset;
	uint16_t actual_offset;
	unsigned ret;
	const unsigned ich_model_num = ich_model(NULL);

	disable_ioapic();	// leave it disabled upon return

	/* validate <ioapic_paddr> */
	if ((ioapic_paddr & 0xFEC00000U) != 0xFEC00000U) return -1;

	/* do this mapping after the disable() */
	oic_reg = startup_memory_map(2, ich_get_rcba() + 0x31fe, PROT_READ|PROT_WRITE|PROT_NOCACHE);
	ASSERT(oic_reg != NULL);
	++oic_reg;	// point to offset 0x31FF (we assume a non-corporate model)

	/* so far, it looks like any ICH reporting >= 10 has the same OIC layout as ICH10 corporate */
	if ((ich_model_num >= 10) || ich_corporate_model(ich_model_num))
	{
		offset_mask = 0xFFU;	// 8 bit base address offset
		offset_shift = 0;		// 8 bit mask starts at bit 0 by default
		--oic_reg;				// point back to 0x31FE
	}

	/* extract 'offset_mask' bits (15:12 or 19:12) of ioapic_paddr and shift down */
	desired_offset = (ioapic_paddr & (offset_mask << 12)) >> 12;

	/*
	 * for the ICH7 and earlier, there is no ASEL field in the OIC register and
	 * we cannot rely on the reserved bits being 0 so we don't even read the OIC
	 * register. A non-zero 'desired_offset' is therefore an error.
	 *
	 * Note that the SCH returns a model of 1. While technically the SCH should
	 * have its own implementation of this file, it is otherwise very similar to
	 * the ICH and we can just use this implementation for now relying on the
	 * model # of 1 to skip the OIC register read (which does not exist in the
	 * SCH) and return based on an expected desired_offset of 0 as with early ICH's
	*/
	if (ich_model_num < 8) return (desired_offset == 0) ? 0 : -1;

	actual_offset = *oic_reg & (offset_mask << offset_shift);
//kprintf("mask/shift: %x/%u\n", offset_mask, offset_shift);
//kprintf("actual/desired offsets: %x/%x\n", actual_offset, desired_offset);
	if (actual_offset != desired_offset) {
		*oic_reg &= ~(offset_mask << offset_shift);
		*oic_reg |= ((desired_offset << offset_shift) & offset_mask);
	}
//kprintf("OIC is 0x%x\n", ((uintptr_t)oic_reg & 1) ? *oic_reg : *(volatile uint16_t *)oic_reg);
	/* read it back again just to be sure and return the appropriate value */
	ret = ((*oic_reg & (offset_mask << offset_shift)) == desired_offset) ? 0 : -1;

	startup_memory_unmap((void *)oic_reg);

	return ret;
}
#endif

/*
* check_paddr_offset
*
* Ensure that the physical address programmed for the IOAPIC is consistent with the
* OIC[ASEL] (address range select).
*
* For all ICH's prior to the ICH8, there is no ASEL field in the OIC register and the
* offset is assumed to be 0x0
* For the ICH8, ICH9 and consumer ICH10, the OIC register contains a 4 bit address
* range select field in the 8 bit OIC register at offset 0x31FF.
* For the corporate ICH10 and newer, the OIC register is 16 bits and the address
* range select field is 8 bits and at offset 0x31FE (LE) and the upper 8 bits of the
* OIC register is similar to pre ICH8 for AEN and CEN fields.
*
* return 0 on success, otherwise -1
*/
static int check_paddr_offset(const PADDR_T ioapic_paddr)
{
	unsigned offset_mask = 0xFU;	// 4 bits by default
	unsigned offset_shift = 4;		// 4 bit mask starts at bit 4 by default
	volatile uint8_t *oic_reg;
	uint16_t desired_offset;
	uint16_t actual_offset;
	unsigned ret;
	const unsigned ich_model_num = ich_model(NULL);

	/* validate <ioapic_paddr> */
	if ((ioapic_paddr & 0xFEC00FFFU) != 0xFEC00000U) return -1;

	oic_reg = startup_memory_map(2, ich_get_rcba() + 0x31fe, PROT_READ|PROT_NOCACHE);
	ASSERT(oic_reg != NULL);
	++oic_reg;	// point to offset 0x31FF (we assume a non-corporate model)

	/* so far, it looks like any ICH reporting >= 10 has the same OIC layout as ICH10 corporate */
	if ((ich_model_num >= 10) || ich_corporate_model(ich_model_num))
	{
		offset_mask = 0xFFU;	// 8 bit base address offset
		offset_shift = 0;		// 8 bit mask starts at bit 0 (of 0x31FE)
		--oic_reg;				// point back to 0x31FE
	}

	/* extract 'offset_mask' bits (15:12 or 19:12) of ioapic_paddr and shift down */
	desired_offset = (ioapic_paddr & (offset_mask << 12)) >> 12;

	/*
	 * for the ICH7 and earlier, there is no ASEL field in the OIC register and
	 * we cannot rely on the reserved bits being 0 so we don't even read the OIC
	 * register. In this situation, if this is the first (or only) IOAPIC, a
	 * non-zero 'desired_offset' will be considered an error. For multi IOAPIC
	 * configurations, we won't be able to validate the paddr so we will just
	 * accept it. This situation exists in the Intel 3100 series server chipsets
	 * where the chipset (LPC) ID reports as an ICH6 (ie. no OIC[ASEL]) yet there
	 * is more than 1 IOAPIC.
	 *
	 * Note that the SCH/IOH return models # of < 8. While technically they should
	 * have their own implementation of this file, they are otherwise very similar
	 * to the ICH and we can use this implementation for now relying on the model #
	 * < 8 to skip the OIC register read (which doesn't exist in these chipsets)
	 * and return based on an expected desired_offset of 0 as with early ICH's
	*/
	if (ich_model_num < 8)
	{
		if (ioapic_get_id(ioapic_paddr) == ioapic_get_id_first()) {
			/* first or only IOAPIC */
			ret = (desired_offset == 0) ? 0 : -1;
		} else {
			if (debug_flag) kprintf("Extra IOAPIC paddr %x cannot be validated .. continue\n", ioapic_paddr);
			ret = 0;
		}
	}
	else
	{
		actual_offset = *oic_reg & (offset_mask << offset_shift);
//kprintf("mask/shift: %x/%u\n", offset_mask, offset_shift);
//kprintf("actual/desired offsets: %x/%x\n", actual_offset, desired_offset);
//kprintf("OIC is 0x%x\n", ((uintptr_t)oic_reg & 1) ? *oic_reg : *(volatile uint16_t *)oic_reg);
		ret = (actual_offset <= desired_offset) ? 0 : -1;
	}

	startup_memory_unmap((void *)oic_reg);
	return ret;
}

static unsigned ioapic_num_entries(const PADDR_T ioapic_paddr)
{
	ASSERT(ioapic_paddr != NULL_PADDR_STARTUP);
	return ((ioapic_read(ioapic_paddr, ich_ioapic_reg_VERSION) & 0x00FF0000) >> 16) + 1;
}

static unsigned ioapic_get_id(const PADDR_T ioapic_paddr)
{
	return (ioapic_read(ioapic_paddr, ich_ioapic_reg_ID) & 0x0F000000) >> 24;
}

static void ioapic_set_id(const PADDR_T ioapic_paddr, unsigned ioapic_id)
{
	/* set the id preserving all other bits */
	uint32_t id_reg = ioapic_read(ioapic_paddr, ich_ioapic_reg_ID) & ~0x0F000000;
	ioapic_write(ioapic_paddr, ich_ioapic_reg_ID, ((ioapic_id & 0xF) << 24) | id_reg);
	ASSERT(ioapic_get_id(ioapic_paddr) == ioapic_id);
}

#ifdef APIC_DEBUG
static unsigned ioapic_version(const PADDR_T ioapic_paddr)
{
	return ioapic_read(ioapic_paddr, ich_ioapic_reg_VERSION) & 0x000000FF;
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
	const unsigned reg_num = ich_ioapic_reg_REDIR_TBL(entry_num);
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
	const unsigned reg_num = ich_ioapic_reg_REDIR_TBL(entry_num);
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
