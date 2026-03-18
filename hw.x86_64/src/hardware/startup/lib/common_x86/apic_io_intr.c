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
#if defined(__X86_64_)
#include <x86_64/intr.h>
#else
#include <x86/intr.h>
#endif
#include <x86/lapic.h>
#include <x86/imps.h>
#include <drvr/hwinfo.h>

extern struct callout_rtn	interrupt_eoi_lapic;

extern struct callout_rtn	interrupt_id_ioapic;
extern struct callout_rtn	interrupt_eoi_ioapic;
extern struct callout_rtn	interrupt_mask_ioapic;
extern struct callout_rtn	interrupt_unmask_ioapic;
extern struct callout_rtn	interrupt_config_ioapic;

static struct startup_intrinfo ioapic_intrs[] =
{
	[0 ... 7] =
	{
		.vector_base = 0,		// filled in below
		.num_vectors = 0,		// filled in below
		.cascade_vector = _NTO_INTR_SPARE,
		.cpu_intr_base = 0,		// filled in below
		.cpu_intr_stride = 1,
		.flags = 0,
		.id = {.genflags = INTR_GENFLAG_NOGLITCH, .size = 0, .rtn = &interrupt_id_ioapic},
		/* note that for the EOI, we normally do not use the directed method. If we do,
		 * then all LAPIC SVR's that have interrupts sent to them from IO-APIC's must
		 * have bit 12 set */
		.eoi = {.genflags = 0, .size = 0, .rtn = &interrupt_eoi_lapic},
		.mask = &interrupt_mask_ioapic,
		.unmask = &interrupt_unmask_ioapic,
		.config = &interrupt_config_ioapic,
		.patch_data = NULL,
	}
};

static unsigned find_cpu_intr_base(const ioapic_cfg_t *ioapic_cfg, const unsigned num);
static int find_highest_entry(const ioapic_cfg_t *ioapic_cfg, const unsigned num);
static ioapic_cfg_t *find_free_entry(ioapic_cfg_t *ioapic_cfg, const unsigned num);

/* if not specified we assume an Intel ICH style IOAPIC. A board specific implementation
 * can override the access functions used (for example if it contains something other
 * than an Intel ICH bridge implementation of the IOAPIC) but still use this default
 * init_intrinfo_ioapic() routine by calling ioapic_find() and then ioapic_set_accessfnc()
 * for the specific IOAPIC id prior to calling this init_intrinfo_ioapic() routine (see
 * boards/apic/init_intrinfo.c). Of course you can always just implement your own
 * init_intrinfo_ioapic() routine as well ... your choice.
*/
extern const ioapic_accessfnc_t * ioapic_accessfncs_ich;

/*
 * my lack of x86 assembler savvy prevents me from creating a patch routine with
 * this structure as an automatic so its stays as a (file) global.
*/
static apic_patch_data_t  apic_patch_data[NUM_ELTS(ioapic_intrs)];

/*
 * init_intrinfo_ioapic
 *
 * This is the default IOAPIC interrupt controller initialization routine for a
 * standard Intel Chipset configuration.
 * It expects
 * 		- 1 IOAPIC with ID=2 (it will ignore any others)
 * 		- ACPI/MP configuration tables
 * 		- the IOAPIC register set as per the Intel ICH southbridge specs (unless
 * 		  otherwise specified)
 *
 * A single parameter, <vector_base> is used as the first vector to use for
 * the block of interrupts. Together with the return value, the caller can
 * determine the next value to use in additional board specific init_intrinfo_*()
 * calls. This vector refers to the external vector used in InterruptAttach()
 * calls and not the internal CPU vector.
 *
 * For non ACPI/MP Config table or multiple/alternative IOAPIC implementations,
 * you need to implement this function in a board specific file.
 *
 * Returns: the number of vectors configured
*/
int init_intrinfo_ioapic(int vector_base, const int _ioapic_id)
{
	static unsigned ioapic_cnt = 0;	// used as index into ioapic_intrs[]
	const unsigned num_ioapics = ioapic_find();	// will scan ACPI/MP tables (if not already done)
	const unsigned ioapic_id = (_ioapic_id < 0) ? ioapic_get_id_first() : _ioapic_id;
	const int ioapic_base_vector = ioapic_get_gsibase(ioapic_id);
	const ioapic_accessfnc_t *ioapic_accessfncs = ioapic_get_accessfnc(ioapic_id);
	unsigned num_interrupts;
	unsigned num_interrupts_configured;
	int r;
	unsigned i;
	ioapic_cfg_t *ioapic_cfg;

	if (num_ioapics == 0) return 0;
	if (debug_flag) kprintf("configuring IOAPIC id:%u\n", ioapic_id);

	if (ioapic_cnt >= NUM_ELTS(ioapic_intrs)) {
		crash("\n%u IOAPIC's is more than we currently support\nUse startup-bios/pci-bios\nHalt!\n", ioapic_cnt);
	}

	if (vector_base < 0)
	{
		if (ioapic_base_vector >= 0) {
			vector_base = ioapic_base_vector;
		}
		else
		{
			kprintf("A vector base was not specified and one could not be found in the ACPI tables\n");
			return 0;
		}
	}
	else if ((ioapic_base_vector >= 0) && (vector_base != ioapic_base_vector))
	{
		/* warn of mismatch but we use 'vector_base' regardless */
		kprintf("Specified IVEC base=%u conflicts with ACPI GSI value=%u, using %u\n", vector_base, ioapic_base_vector, vector_base);
	}

	/* if the access functions have not been specified, setup the ICH default */
	if (ioapic_accessfncs == NULL) {
		if (debug_flag) {
			kprintf("No access functions found for IOAPIC id %u, using ICH defaults\n", ioapic_id);
		}
		ioapic_set_accessfnc(ioapic_id, ioapic_accessfncs = ioapic_accessfncs_ich);
	}

	num_interrupts = ioapic_accessfncs->num_entries(ioapic_id);
	ASSERT(num_interrupts > 0);

	ioapic_cfg = ws_alloc(num_interrupts * sizeof(*ioapic_cfg));
	ASSERT(ioapic_cfg != NULL);

	/* fill in the ioapic_cfg_t structure from BIOS settings */
	num_interrupts_configured = ioapic_auto_config(ioapic_id, ioapic_cfg, num_interrupts);

	/*
	 * auto config will take care of the standard 16 interrupts. Any remaining are
	 * assigned to IOAPIC INTIN pins as follows
	 * 		PIRQA - PIRQD --> INTIN16 - INTIN19 respectively
	 *		PIRQE - PIRQH --> INTIN20 - INTIN23 respectively
	 * so we create entries for them in order to reserve the vectors. We do this
	 * even for PIRQ[H:E] which could be configured as GPIO's since they could be
	 * changed back to interrupts and we want to make sure that the vectors are
	 * available.
	*/
	if (num_interrupts_configured < num_interrupts)
	{
		const unsigned num = num_interrupts - num_interrupts_configured;
		const unsigned ioapic_next_ivec = find_cpu_intr_base(ioapic_cfg, num_interrupts) + num_interrupts_configured;
		const unsigned first_intin = num_interrupts_configured;
		for (i=0; i<num; i++)
		{
			ioapic_cfg_t *p = find_free_entry(ioapic_cfg, num_interrupts);
			if (p == NULL) break;
			memset(p, 0, sizeof(*p));
			p->vector = ioapic_next_ivec + i;
			p->cpu_mask = (1 << 0);			// CPU 0 only
			p->masked = 1;					// masked by default
			p->intsrc.dest_apic_id = ioapic_id;
			p->intsrc.dest_apic_intin = first_intin + i;
			p->intsrc.int_type = IMPS_INTSRC_INTTYPE_FIX;
			const int polarity = ioapic_intsrc_polarity_get(ioapic_id, p->intsrc.dest_apic_intin);
			const int trigger_mode = ioapic_intsrc_trigger_mode_get(ioapic_id, p->intsrc.dest_apic_intin);
			/* if polarity is undetermined (ie. 0) assume active low (previous behaviour) */
			_Uint16t flags = (polarity <= 0) ? IMPS_INTSRC_ACTIVE_LO : IMPS_INTSRC_ACTIVE_HI;
			/* if trigger is undetermined (ie. -1) assume level (previous behaviour) */
			flags |= (trigger_mode <= 0) ? IMPS_INTSRC_TRIG_LEVEL : IMPS_INTSRC_TRIG_EDGE;
			p->intsrc.flags = flags;
			++num_interrupts_configured;
		}
	}

	ioapic_accessfncs->configure(ioapic_id, NULL, -1);	// clear and mask all IOAPIC entries
	r = ioapic_accessfncs->configure(ioapic_id, ioapic_cfg, num_interrupts);
	/* make sure all requested interrupt sources are configured */
	ASSERT(r == num_interrupts_configured);

	/* we must set 'num_vectors' to the highest configured IOAPIC entry (+1 because
	 * 0 based) used since our interrupt infrastructure expects this. This may leave
	 * used vectors but nothing currently we can do since that's how the OS expects
	 * the callouts */
	ioapic_intrs[ioapic_cnt].num_vectors = find_highest_entry(ioapic_cfg, num_interrupts) + 1;
	ASSERT(ioapic_intrs[ioapic_cnt].num_vectors > 0);
	ioapic_intrs[ioapic_cnt].vector_base = vector_base;
	ioapic_intrs[ioapic_cnt].cpu_intr_base = find_cpu_intr_base(ioapic_cfg, num_interrupts);
	/* set up the patch data */
	apic_patch_data[ioapic_cnt].ioapic = ioapic_get_vaddr(ioapic_id);
	apic_patch_data[ioapic_cnt].lapic = lapic_callout_vaddr;
	apic_patch_data[ioapic_cnt].ioapic_cpu_intr_base = ioapic_intrs[ioapic_cnt].cpu_intr_base;
	ioapic_intrs[ioapic_cnt].patch_data = (void *)&apic_patch_data[ioapic_cnt];
//	kprintf("apic_patch_data[ioapic_cnt] @ %x is %x %x\n", &apic_patch_data[ioapic_cnt], apic_patch_data[ioapic_cnt].ioapic, apic_patch_data[ioapic_cnt].lapic);

	/* check whether or not directed EOI's will be used and if so adjust the eoi callout.
	 * Note that the INTR_GENFLAG_LOAD_LEVEL flag requires procnto fix PR71374 */
	if (lapic_read(LAPIC_SVR) & SVR_EOI_BROADCAST_DISABLED)
	{
		ioapic_intrs[ioapic_cnt].eoi.rtn = &interrupt_eoi_ioapic;
		ioapic_intrs[ioapic_cnt].eoi.genflags = INTR_GENFLAG_LOAD_LEVEL;
		ioapic_intrs[ioapic_cnt].eoi.size = 0;
		if (debug_flag) kprintf("Directed EOI enabled\n");
	}

	add_interrupt(&ioapic_intrs[ioapic_cnt]);
	/* after a successful IOAPIC configuration, shut off LAPIC LINT0 */
	/* NOTE - need to do this on all processors though if we are going to support interrupt routing */
	/* NOTE - it is possible that NMI could be connected to LINT0 so we need a better way of determining
	 * 			which LINT to mask (see ACPI Local NMI structure) */
	lapic_write(LAPIC_LL0R, lapic_read(LAPIC_LL0R) | LVT_MASKED);

	if (debug_flag) {
		kprintf("IOAPIC %u: successfully configured %u vectors from %u to %u\n",
				ioapic_id,
				ioapic_intrs[ioapic_cnt].num_vectors, ioapic_intrs[ioapic_cnt].vector_base,
				ioapic_intrs[ioapic_cnt].vector_base + ioapic_intrs[ioapic_cnt].num_vectors - 1);
	}
	const unsigned num_configured = ioapic_intrs[ioapic_cnt].num_vectors;
	++ioapic_cnt;
	return num_configured;
}

/*
 * find_cpu_intr_base
 *
 * return the lowest numbered ioapic_cfg[].vector in the array
*/
static unsigned find_cpu_intr_base(const ioapic_cfg_t *ioapic_cfg, const unsigned num)
{
	unsigned i;
	unsigned base_vector = UINT_MAX;

	ASSERT(num > 0);
	for (i=0; i<num; i++) {
		if (ioapic_cfg[i].vector < base_vector) {
			base_vector = ioapic_cfg[i].vector;
		}
	}
	return base_vector;
}

/*
 * find_highest_entry
 *
 * return the highest configured ioapic_cfg[] entry. -1 means error
*/
static int find_highest_entry(const ioapic_cfg_t *ioapic_cfg, const unsigned num)
{
	int i = num;

	ASSERT(num > 0);
	while(--i >= 0) {
		if (ioapic_cfg[i].intsrc.dest_apic_id != 0) return i;
	}
	return -1;
}

/*
 * find_free_entry
 *
 * return a pointer to the first available ioapic_cfg[] entry. NULL means error
*/
static ioapic_cfg_t *find_free_entry(ioapic_cfg_t *ioapic_cfg, const unsigned num)
{
	unsigned i;

	ASSERT(num > 0);
	for (i=0; i<num; i++) {
		if (ioapic_cfg[i].intsrc.dest_apic_id == 0) return &ioapic_cfg[i];
	}
	return NULL;
}
