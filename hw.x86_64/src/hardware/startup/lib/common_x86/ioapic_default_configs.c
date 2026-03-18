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
 * This file contains Intel defined APIC configuration tables (see pg 5-6 of
 * Intel's Multiprocessor Specification v1.4) and a function to fill in an
 * ioapic_cfg_t array with the specified configurations. This is normally used
 * in a BIOS/EFI based system however it can also be used to establish a "known"
 * standard configuration when IOAPIC's are manually configured as well (such as
 * in a fastboot scenario with no BIOS) 
*/

#include <startup.h>
#include <x86/lapic.h>
#include <x86/intel_misc.h>


#define INTEL_DEFAULT_CONFIG_6_INITIALIZER \
	{	/* 8259 IOUT (.source_bus_intr does not apply since not a FIXED interrupt) */ \
		.intsrc = {.dest_apic_intin = 0, \
			.flags = IMPS_INTSRC_ACTIVE_HI|IMPS_INTSRC_TRIG_EDGE, \
			/* normally, the INTTYPE for this would be EXT. However, there
			 * are some boards (so far with Nvidia chipsets) that connect
			 * the 8254 timer to INTPIN0 instead of 2. So we program this
			 * pin as FIXed so that if necessary we can simply use IRQ 0
			 * instead of 2 and the IOAPIC will already be properly configured.
			 * This should not negatively impact properly connected chipsets
			 * since INTPIN0 would otherwise normally be IOUT from the 8259
			 * and won't be connected to
			.int_type = IMPS_INTSRC_INTTYPE_EXT, .source_bus_id = 0,
			*/ \
			.int_type = IMPS_INTSRC_INTTYPE_FIX, .source_bus_id = 0, \
		}, \
	}, \
	{	/* Legacy IRQ1 (Keyboard Buffer Full) */ \
		.intsrc = {.dest_apic_intin = 1, .source_bus_intr = 1, \
			.flags = IMPS_INTSRC_ACTIVE_HI|IMPS_INTSRC_TRIG_EDGE, \
			.int_type = IMPS_INTSRC_INTTYPE_FIX, .source_bus_id = 0, \
		}, \
	}, \
	{	/* Legacy IRQ0 (8254 timer) */ \
		.intsrc = {.dest_apic_intin = 2, .source_bus_intr = 0, \
			.flags = IMPS_INTSRC_ACTIVE_HI|IMPS_INTSRC_TRIG_EDGE, \
			.int_type = IMPS_INTSRC_INTTYPE_FIX, .source_bus_id = 0, \
		}, \
	}, \
	{	/* Legacy IRQ3 */ \
		.intsrc = {.dest_apic_intin = 3, .source_bus_intr = 3, \
			.flags = IMPS_INTSRC_ACTIVE_HI|IMPS_INTSRC_TRIG_EDGE, \
			.int_type = IMPS_INTSRC_INTTYPE_FIX, .source_bus_id = 0, \
		}, \
	}, \
	{	/* Legacy IRQ4 */ \
		.intsrc = {.dest_apic_intin = 4, .source_bus_intr = 4, \
			.flags = IMPS_INTSRC_ACTIVE_HI|IMPS_INTSRC_TRIG_EDGE, \
			.int_type = IMPS_INTSRC_INTTYPE_FIX, .source_bus_id = 0, \
		}, \
	}, \
	{	/* Legacy IRQ5 */ \
		.intsrc = {.dest_apic_intin = 5, .source_bus_intr = 5, \
			.flags = IMPS_INTSRC_ACTIVE_HI|IMPS_INTSRC_TRIG_EDGE, \
			.int_type = IMPS_INTSRC_INTTYPE_FIX, .source_bus_id = 0, \
		}, \
	}, \
	{	/* Legacy IRQ6 */ \
		.intsrc = {.dest_apic_intin = 6, .source_bus_intr = 6, \
			.flags = IMPS_INTSRC_ACTIVE_HI|IMPS_INTSRC_TRIG_EDGE, \
			.int_type = IMPS_INTSRC_INTTYPE_FIX, .source_bus_id = 0, \
		}, \
	}, \
	{	/* Legacy IRQ7 */ \
		.intsrc = {.dest_apic_intin = 7, .source_bus_intr = 7, \
			.flags = IMPS_INTSRC_ACTIVE_HI|IMPS_INTSRC_TRIG_EDGE, \
			.int_type = IMPS_INTSRC_INTTYPE_FIX, .source_bus_id = 0, \
		}, \
	}, \
	{	/* Legacy IRQ8 (RTC) */ \
		.intsrc = {.dest_apic_intin = 8, .source_bus_intr = 8, \
			.flags = IMPS_INTSRC_ACTIVE_HI|IMPS_INTSRC_TRIG_EDGE,\
			.int_type = IMPS_INTSRC_INTTYPE_FIX, .source_bus_id = 0, \
		}, \
	}, \
	{	/* Legacy IRQ9 */ \
		.intsrc = {.dest_apic_intin = 9, .source_bus_intr = 9, \
			.flags = IMPS_INTSRC_ACTIVE_HI|IMPS_INTSRC_TRIG_EDGE, \
			.int_type = IMPS_INTSRC_INTTYPE_FIX, .source_bus_id = 0, \
		}, \
	}, \
	{	/* Legacy IRQ10 */ \
		.intsrc = {.dest_apic_intin = 10, .source_bus_intr = 10, \
			.flags = IMPS_INTSRC_ACTIVE_HI|IMPS_INTSRC_TRIG_EDGE, \
			.int_type = IMPS_INTSRC_INTTYPE_FIX, .source_bus_id = 0, \
		}, \
	}, \
	{	/* Legacy IRQ11 */ \
		.intsrc = {.dest_apic_intin = 11, .source_bus_intr = 11, \
			.flags = IMPS_INTSRC_ACTIVE_HI|IMPS_INTSRC_TRIG_EDGE, \
			.int_type = IMPS_INTSRC_INTTYPE_FIX, .source_bus_id = 0, \
		}, \
	}, \
	{	/* Legacy IRQ12 */ \
		.intsrc = {.dest_apic_intin = 12, .source_bus_intr = 12, \
			.flags = IMPS_INTSRC_ACTIVE_HI|IMPS_INTSRC_TRIG_EDGE, \
			.int_type = IMPS_INTSRC_INTTYPE_FIX, .source_bus_id = 0, \
		}, \
	}, \
	{	/* Legacy IRQ13 */ \
		.intsrc = {.dest_apic_intin = 13, .source_bus_intr = 13, \
			.flags = IMPS_INTSRC_ACTIVE_HI|IMPS_INTSRC_TRIG_EDGE, \
			.int_type = IMPS_INTSRC_INTTYPE_FIX, .source_bus_id = 0, \
		}, \
	}, \
	{	/* Legacy IRQ14 */ \
		.intsrc = {.dest_apic_intin = 14, .source_bus_intr = 14, \
			.flags = IMPS_INTSRC_ACTIVE_HI|IMPS_INTSRC_TRIG_EDGE, \
			.int_type = IMPS_INTSRC_INTTYPE_FIX, .source_bus_id = 0, \
		}, \
	}, \
	{	/* Legacy IRQ15 */ \
		.intsrc = {.dest_apic_intin = 15, .source_bus_intr = 15, \
			.flags = IMPS_INTSRC_ACTIVE_HI|IMPS_INTSRC_TRIG_EDGE, \
			.int_type = IMPS_INTSRC_INTTYPE_FIX, .source_bus_id = 0, \
		}, \
	}

#define INTEL_APL_CONFIG_INITIALIZER \
	{	/* 8259 IOUT (.source_bus_intr does not apply since not a FIXED interrupt) */ \
		.intsrc = {.dest_apic_intin = 0, \
			.flags = IMPS_INTSRC_ACTIVE_HI|IMPS_INTSRC_TRIG_EDGE, \
			/* normally, the INTTYPE for this would be EXT. However, there
			 * are some boards (so far with Nvidia chipsets) that connect
			 * the 8254 timer to INTPIN0 instead of 2. So we program this
			 * pin as FIXed so that if necessary we can simply use IRQ 0
			 * instead of 2 and the IOAPIC will already be properly configured.
			 * This should not negatively impact properly connected chipsets
			 * since INTPIN0 would otherwise normally be IOUT from the 8259
			 * and won't be connected to
			.int_type = IMPS_INTSRC_INTTYPE_EXT, .source_bus_id = 0,
			*/ \
			.int_type = IMPS_INTSRC_INTTYPE_FIX, .source_bus_id = 0, \
		}, \
	}, \
	{	/* Legacy IRQ1 (Keyboard Buffer Full) */ \
		.intsrc = {.dest_apic_intin = 1, .source_bus_intr = 1, \
			.flags = IMPS_INTSRC_ACTIVE_HI|IMPS_INTSRC_TRIG_EDGE, \
			.int_type = IMPS_INTSRC_INTTYPE_FIX, .source_bus_id = 0, \
		}, \
	}, \
	{	/* Legacy IRQ0 (8254 timer) */ \
		.intsrc = {.dest_apic_intin = 2, .source_bus_intr = 0, \
			.flags = IMPS_INTSRC_ACTIVE_HI|IMPS_INTSRC_TRIG_EDGE, \
			.int_type = IMPS_INTSRC_INTTYPE_FIX, .source_bus_id = 0, \
		}, \
	}, \
	{	/* Legacy IRQ3 */ \
		.intsrc = {.dest_apic_intin = 3, .source_bus_intr = 3, \
			.flags = IMPS_INTSRC_ACTIVE_LO|IMPS_INTSRC_TRIG_LEVEL, \
			.int_type = IMPS_INTSRC_INTTYPE_FIX, .source_bus_id = 0, \
		}, \
	}, \
	{	/* Legacy IRQ4 */ \
		.intsrc = {.dest_apic_intin = 4, .source_bus_intr = 4, \
			.flags = IMPS_INTSRC_ACTIVE_LO|IMPS_INTSRC_TRIG_LEVEL, \
			.int_type = IMPS_INTSRC_INTTYPE_FIX, .source_bus_id = 0, \
		}, \
	}, \
	{	/* Legacy IRQ5 */ \
		.intsrc = {.dest_apic_intin = 5, .source_bus_intr = 5, \
			.flags = IMPS_INTSRC_ACTIVE_LO|IMPS_INTSRC_TRIG_LEVEL,\
			.int_type = IMPS_INTSRC_INTTYPE_FIX, .source_bus_id = 0, \
		}, \
	}, \
	{	/* Legacy IRQ6 */ \
		.intsrc = {.dest_apic_intin = 6, .source_bus_intr = 6, \
			.flags = IMPS_INTSRC_ACTIVE_LO|IMPS_INTSRC_TRIG_LEVEL, \
			.int_type = IMPS_INTSRC_INTTYPE_FIX, .source_bus_id = 0, \
		}, \
	}, \
	{	/* Legacy IRQ7 */ \
		.intsrc = {.dest_apic_intin = 7, .source_bus_intr = 7, \
			.flags = IMPS_INTSRC_ACTIVE_LO|IMPS_INTSRC_TRIG_LEVEL, \
			.int_type = IMPS_INTSRC_INTTYPE_FIX, .source_bus_id = 0, \
		}, \
	}, \
	{	/* Legacy IRQ8 (RTC) */ \
		.intsrc = {.dest_apic_intin = 8, .source_bus_intr = 8, \
			.flags = IMPS_INTSRC_ACTIVE_HI|IMPS_INTSRC_TRIG_EDGE, \
			.int_type = IMPS_INTSRC_INTTYPE_FIX, .source_bus_id = 0, \
		}, \
	}, \
	{	/* Legacy IRQ9 */ \
		.intsrc = {.dest_apic_intin = 9, .source_bus_intr = 9, \
			.flags = IMPS_INTSRC_ACTIVE_LO|IMPS_INTSRC_TRIG_LEVEL, \
			.int_type = IMPS_INTSRC_INTTYPE_FIX, .source_bus_id = 0, \
		}, \
	}, \
	{	/* Legacy IRQ10 */ \
		.intsrc = {.dest_apic_intin = 10, .source_bus_intr = 10, \
			.flags = IMPS_INTSRC_ACTIVE_LO|IMPS_INTSRC_TRIG_LEVEL, \
			.int_type = IMPS_INTSRC_INTTYPE_FIX, .source_bus_id = 0, \
		}, \
	}, \
	{	/* Legacy IRQ11 */ \
		.intsrc = {.dest_apic_intin = 11, .source_bus_intr = 11, \
			.flags = IMPS_INTSRC_ACTIVE_LO|IMPS_INTSRC_TRIG_LEVEL, \
			.int_type = IMPS_INTSRC_INTTYPE_FIX, .source_bus_id = 0, \
		}, \
	}, \
	{	/* Legacy IRQ12 */ \
		.intsrc = {.dest_apic_intin = 12, .source_bus_intr = 12, \
			.flags = IMPS_INTSRC_ACTIVE_HI|IMPS_INTSRC_TRIG_EDGE, \
			.int_type = IMPS_INTSRC_INTTYPE_FIX, .source_bus_id = 0, \
		}, \
	}, \
	{	/* Legacy IRQ13 */ \
		.intsrc = {.dest_apic_intin = 13, .source_bus_intr = 13, \
			.flags = IMPS_INTSRC_ACTIVE_LO|IMPS_INTSRC_TRIG_LEVEL, \
			.int_type = IMPS_INTSRC_INTTYPE_FIX, .source_bus_id = 0, \
		}, \
	}, \
	{	/* Legacy IRQ14 */ \
		.intsrc = {.dest_apic_intin = 14, .source_bus_intr = 14, \
			.flags = IMPS_INTSRC_ACTIVE_LO|IMPS_INTSRC_TRIG_LEVEL, \
			.int_type = IMPS_INTSRC_INTTYPE_FIX, .source_bus_id = 0, \
		}, \
	}, \
	{	/* Legacy IRQ15 */ \
		.intsrc = {.dest_apic_intin = 15, .source_bus_intr = 15, \
			.flags = IMPS_INTSRC_ACTIVE_LO|IMPS_INTSRC_TRIG_LEVEL, \
			.int_type = IMPS_INTSRC_INTTYPE_FIX, .source_bus_id = 0, \
		}, \
	}

static const ioapic_cfg_t Intel_Default_Config_6[] =
{
	/*
	 * Note that it is '.dest_apic_intin' which determines the IOAPIC input not
	 * the order of the array entries. The .source_bus_intr corresponds to the IRQ.
	 * The .source_bus_id is 0 corresponding to ISA for all entries
	 *
	 * As a minimum the following fields MUST be explicitly set
	 *	.dest_apic_intin
	 *	.source_bus_intr (except as noted)
	 *	.source_bus_id
	 *	.flags
	 *	.int_type
	*/
	INTEL_DEFAULT_CONFIG_6_INITIALIZER,
};

static const ioapic_cfg_t Intel_APL_Config[] =
{
	/*
	 * Note that it is '.dest_apic_intin' which determines the IOAPIC input not
	 * the order of the array entries. The .source_bus_intr corresponds to the IRQ.
	 * The .source_bus_id is 0 corresponding to ISA for all entries
	 *
	 * As a minimum the following fields MUST be explicitly set
	 *	.dest_apic_intin
	 *	.source_bus_intr (except as noted)
	 *	.source_bus_id
	 *	.flags
	 *	.int_type
	*/
	INTEL_APL_CONFIG_INITIALIZER,
};

/*
 * This profile has been created to provide the default initialization values
 * for an extended IO-APIC configuration for interrupts beyond the default 24.
 * As such, it provides initialization values for the first 16 legacy interrupts
 * identical to Intel_Default_Config_6[] (default profile 6), values for the 8
 * dedicated legacy PCI INTPIN interrupts (PIRQ's) and values for the remaining
 * interrupts up to and including 119.
 */
static ioapic_cfg_t Intel_Extended_Level_Hi[] =
{
	/* 16 legacy 8259 IRQ's */
	INTEL_DEFAULT_CONFIG_6_INITIALIZER,

	/* 8 legacy PCI INTPIN PIRQs */
	[16 ... 23] =
	{
		/*
		 * in order to allow us to use a single initializer for all entries,
		 * set both the 'dest_apic_intin' and 'source_bus_intr' fields to 0.
		 * This condition will be caught in ioapic_autofill_defaults() and
		 * the values initialized to the index in which they reside, which for
		 * the extended values is currently the correct value to use
		 */
		.intsrc = {.dest_apic_intin = 0, .source_bus_intr = 0,
			.flags = IMPS_INTSRC_ACTIVE_LO|IMPS_INTSRC_TRIG_LEVEL,
			.int_type = IMPS_INTSRC_INTTYPE_FIX, .source_bus_id = 0,
		},
	},

	/* remaining IO-APIC interrupt sources */
	[24 ... 119] =
	{
		/*
		 * in order to allow us to use a single initializer for all entries,
		 * set both the 'dest_apic_intin' and 'source_bus_intr' fields to 0.
		 * This condition will be caught in ioapic_autofill_defaults() and
		 * the values initialized to the index in which they reside, which for
		 * the extended values is the correct value
		 */
		.intsrc = {.dest_apic_intin = 0, .source_bus_intr = 0,
			.flags = IMPS_INTSRC_ACTIVE_HI|IMPS_INTSRC_TRIG_LEVEL,
			.int_type = IMPS_INTSRC_INTTYPE_FIX, .source_bus_id = 0,
		},
	},
};

/*
 * This profile is exactly the same as Intel_Extended_Level_Hi[] except for
 * Apollo Lake
 */
static ioapic_cfg_t Intel_APL_Extended_Level_Hi[] =
{
	/* 16 legacy 8259 IRQ's */
	INTEL_APL_CONFIG_INITIALIZER,

	/* 8 legacy PCI INTPIN PIRQs */
	[16 ... 23] =
	{
		/*
		 * in order to allow us to use a single initializer for all entries,
		 * set both the 'dest_apic_intin' and 'source_bus_intr' fields to 0.
		 * This condition will be caught in ioapic_autofill_defaults() and
		 * the values initialized to the index in which they reside, which for
		 * the extended values is currently the correct value to use
		 */
		.intsrc = {.dest_apic_intin = 0, .source_bus_intr = 0,
			.flags = IMPS_INTSRC_ACTIVE_LO|IMPS_INTSRC_TRIG_LEVEL,
			.int_type = IMPS_INTSRC_INTTYPE_FIX, .source_bus_id = 0,
		},
	},

	/* remaining IO-APIC interrupt sources */
	[24 ... 119] =
	{
		/*
		 * in order to allow us to use a single initializer for all entries,
		 * set both the 'dest_apic_intin' and 'source_bus_intr' fields to 0.
		 * This condition will be caught in ioapic_autofill_defaults() and
		 * the values initialized to the index in which they reside, which for
		 * the extended values is the correct value
		 */
		.intsrc = {.dest_apic_intin = 0, .source_bus_intr = 0,
			.flags = IMPS_INTSRC_ACTIVE_HI|IMPS_INTSRC_TRIG_LEVEL,
			.int_type = IMPS_INTSRC_INTTYPE_FIX, .source_bus_id = 0,
		},
	},
};

/*
 * This profile can be used if you don't want to auto-fill anything and instead
 * have all interrupts (including the 16 legacy and 8 PCI legacy) be set using
 * the interrupt polarity and trigger registers. These registers either need to
 * exist and be set correctly or alternatively, you can override the functions
 * in ioapic_intsrc_disposition.c (see comments in that file for how to do it)
 * and return the desired IOAPIC polarity and trigger mode settings directly.
 * If none of this makes sense, don't use this profile or your asking for
 * trouble
 */
static ioapic_cfg_t Empty[] =
{

};

static const struct
{
	const unsigned num;
	const ioapic_cfg_t * const config;
} Intel_Default_Config[] =
{
	[IOAPIC_PROFILE_UNUSED] = {.num = 0, .config = NULL},	// this entry not used (only wastes 8/12 bytes)
	[IOAPIC_PROFILE_APL] = {.num = NUM_ELTS(Intel_APL_Config), .config = Intel_APL_Config},
	[IOAPIC_PROFILE_6] = {.num = NUM_ELTS(Intel_Default_Config_6), .config = Intel_Default_Config_6},
	[IOAPIC_PROFILE_EXTENDED_LEVEL_HI] = {.num = NUM_ELTS(Intel_Extended_Level_Hi), .config = Intel_Extended_Level_Hi},
	[IOAPIC_PROFILE_APL_EXTENDED_LEVEL_HI] = {.num = NUM_ELTS(Intel_APL_Extended_Level_Hi), .config = Intel_APL_Extended_Level_Hi},
	[IOAPIC_PROFILE_EMPTY] = {.num = 0, .config = Empty},
};

/*
 * ioapic_autofill_defaults
 *
 * Fill <num> elements of the <ioapic_cfg> array with default values based on the
 * default <profile_num> configuration
 *
 * Returns: the number of elements filled or -1 on error
*/
int ioapic_autofill_defaults(const unsigned ioapic_id, ioapic_cfg_t *ioapic_cfg, const unsigned num, unsigned profile_num)
{
	if ((profile_num < ioapic_profile_t_first) || (profile_num >= NUM_ELTS(Intel_Default_Config)))
	{
		if (debug_flag) kprintf("Unknown profile %u\n", profile_num);
		return -1;
	}
	else if ((num > Intel_Default_Config[profile_num].num) && (debug_flag))
	{
		kprintf("Autofill: %u of requested %u entries available for profile %u\n",
				Intel_Default_Config[profile_num].num, num, profile_num);
	}

	/* don't put an entry in the table unless its got useful information */
	ASSERT(Intel_Default_Config[profile_num].config != NULL);

	const int n = (num > Intel_Default_Config[profile_num].num) ? Intel_Default_Config[profile_num].num : num;
	if (n > 0)
	{
		unsigned i;
		for (i=0; i<n; i++)
		{
			ioapic_cfg[i].intsrc = Intel_Default_Config[profile_num].config[i].intsrc;
			/* assign the entries to <ioapic_id> */
			ioapic_cfg[i].intsrc.dest_apic_id = ioapic_id;

#if 0
kprintf("Checking entry %u, dest_apic_intin/source_bus_intr/flags : %u/%u/0x%x\n", i,
			ioapic_cfg[i].intsrc.dest_apic_intin, ioapic_cfg[i].intsrc.source_bus_intr,ioapic_cfg[i].intsrc.flags);
#endif

			if ((ioapic_cfg[i].intsrc.dest_apic_intin == 0) && (ioapic_cfg[i].intsrc.source_bus_intr == 0))
			{
				ioapic_cfg[i].intsrc.dest_apic_intin = i;
//				ioapic_cfg[i].intsrc.source_bus_intr = i;
				if (debug_flag > 3)
				{
					kprintf("Uninitialized dest_apic_intin/source_bus_intr for profile %u updated to %u/%u\n",
							profile_num, ioapic_cfg[i].intsrc.dest_apic_intin, ioapic_cfg[i].intsrc.source_bus_intr);
				}
			}
		}
	}

	return n;
}
