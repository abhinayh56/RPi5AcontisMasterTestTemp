/*
 * $QNXLicenseC:
 * Copyright 2016, 2017, 2023, 2025 BlackBerry Limited.
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
#include <x86/intel_misc.h>
#include "amd_misc.h"


#define MSR_PLATFORM_INFO	0xCE
#define MSR_PLATFORM_ID		0x17
#define MSR_FSB_FREQ		0xCD
#define MSR_PERF_STATUS		0x198


typedef struct
{
//	const char * const manufacturer_str;
	_Uint32t family_model;
	unsigned(*clk_get)(const _Uint32t family_model);
	const char * const arch_str;
} base_freq_tbl_t;

static const base_freq_tbl_t *find_tbl_entry(const _Uint32t family_model);

/*
 ===============================================================================
 get_leaf_15_timer_freq

 returns the rate (in Hz) at which the LAPIC timer decrements using the CPUID
 0x15 leaf if supported

 A return value of 0 is considered invalid and should not be used

*/
static unsigned get_leaf_15_timer_freq(const _Uint32t family_model)
{
	struct {
		_Uint32t ebx;
		_Uint32t edx;
		_Uint32t ecx;
	} regs = {0, 0, 0};
	_Uint32t eax = x86_cpuid(0x15, (void *)&regs);

	if (debug_flag) {
		kprintf("CPUID:0x15, eax/ebx/edx/ecx: %x/%x/%x/%x\n", eax, regs.ebx, regs.edx, regs.ecx);
	}

	/*
	 * The Intel Developers Manual indicates that ECX, if not 0, contains the
	 * Core Xtal Freq in Hz yet on the ASROCK IMB-151D (BayTrail - Silvermont)
	 * the value is 0x100 (256) and the ratio is 1:1 so these values don't
	 * make sense. Because of this, I'm going to assume that a value less than
	 * 1 MHz is bogus. Could probably be higher but don't want to eliminate
	 * lower powered cores
	 */
	if ((regs.ecx > 0) && (regs.ecx < MEG(1)))
	{
		return 0;
	}
	else if ((regs.ecx == 0) && (regs.ebx != 0) && (eax != 0))
	{
		if (debug_flag) kprintf("TSC multiplier: %u\n", regs.ebx / eax);
		/*
		 * ratio enumerated but not core xtal freq. Get from Table based on uArch
		 * See Determining the Processor Base Frequency Chapter in the 64-ia-32
		 * developers manual. For the Sept 2016 version this is section 18.18.3
		 * on pg 18-106.
		 * 6th and 7th generation processors include Skylake, Kaby Lake, etc
		 */
		const base_freq_tbl_t *tbl_p = find_tbl_entry(family_model);
		if ((tbl_p != NULL) && (strcasecmp(tbl_p->arch_str, "Goldmont") == 0)) return 19200000;
		else return 24000000;
	}
	else
	{
		return regs.ecx;
	}
}

/*
 ===============================================================================
 get_silvermont_timer_freq

 returns the rate (in Hz) at which the LAPIC timer decrements for Silvermont
 and Airmont micro-architectures

 A return value of 0 is considered invalid and should not be used

*/
static unsigned get_silvermont_timer_freq(const _Uint32t family_model)
{
	static const unsigned fsb_tbl_x_mont[] =
	{
		[0] =  83333333, [1] = 100000000,
		[2] = 133333333, [3] = 166666666,
		[4] =  80000000, [5] =  93333333,
		[6] =  90000000, [7] =  88900000,
		[8] =  87500000,
		[9 ... 15] = 000000000,
	};
	const _Uint64t platform_id_msr = rdmsr(MSR_PLATFORM_ID);
	const _Uint64t fsb_freq_msr = rdmsr(MSR_FSB_FREQ);
	if (debug_flag) {
		kprintf("MSR(0xCD) = 0x%L, MSR(0x17) = 0x%L\n", fsb_freq_msr, platform_id_msr);
	}
	const _Uint64t platform_info_msr = rdmsr(MSR_PLATFORM_INFO);
	if (debug_flag) {
		kprintf("MSR_PLATFORM_INFO: 0x%L,TSC multiplier: %u\n", platform_info_msr, (platform_info_msr >> 8) & 0xFF);
	}

	const unsigned fsb_ratio_mask = 0x3F;
	const unsigned fsb_ratio = fsb_ratio_mask & (platform_id_msr >> 8);
	const unsigned fsb_freq = fsb_tbl_x_mont[fsb_freq_msr & 0xF];

	if (debug_flag) {
		kprintf("fsb_ratio: %u, fsb_freq = %u\n", fsb_ratio, fsb_freq);
	}

	return fsb_freq;
//	return (fsb_ratio * fsb_freq) / 10;
}

/*
 ===============================================================================
 get_goldmont_timer_freq

 returns the rate (in Hz) at which the LAPIC timer decrements for the Goldmont
 micro-architecture

 A return value of 0 is considered invalid and should not be used

*/
static unsigned get_goldmont_timer_freq(const _Uint32t family_model)
{
	unsigned base_freq = get_leaf_15_timer_freq(family_model);
	return base_freq;
}

/*
 ===============================================================================
 get_denverton_timer_freq

 returns the rate (in Hz) at which the LAPIC timer decrements for the Denverton
 SoC

 A return value of 0 is considered invalid and should not be used

*/
static unsigned get_denverton_timer_freq(const _Uint32t family_model)
{
    /*
        Intel 64 and IA-32 Architectures Software Developer’s Manual
        Intel Document Number: 325462, December 2017

        Section 10.5.4 APIC Timer
        The APIC timer frequency will be the processor’s bus clock or 
        CORE CRYSTAL CLOCK FREQUENCY (when TSC/core crystal clock ratio
        is enumerated in CPUID leaf 0x15) divided by the value specified
        in the divide configuration register.

        Denverton / Atom C3000 External Design Specification
        Intel Document Number: 558579, December 2017

        Section 1.2.4 Integrated Clock Generator
        The integrated clock generator requires two external crystals (32.768-kHz and 25-MHz)
        and eliminates the need for an external clock chip.

        Based on sections from Intel 64 and IA-32 Architectures Software Developer’s Manual
        and Denverton / Atom C3000 External Design Specification LAPIC timer frequency is set
        to 25MHz. If in the future listed above documents are changed it will be necessery
        to modify current function.
     */

    return 25000000;
}

/*
 ===============================================================================
 get_bridgewell_timer_freq

 returns the rate (in Hz) at which the LAPIC timer decrements for the "bridge"
 (ex. Sandy Bridge, Ivy Bridge) and "well" (ex. Haswell, Broadwell)
 micro-architectures

 A return value of 0 is considered invalid and should not be used

*/
static unsigned get_bridgewell_timer_freq(const _Uint32t family_model)
{
	const _Uint64t platform_info_msr = rdmsr(MSR_PLATFORM_INFO);
	if (debug_flag) {
		kprintf("MSR_PLATFORM_INFO: 0x%L,TSC multiplier: %u\n", platform_info_msr, (platform_info_msr >> 8) & 0xFF);
	}
	return 100000000;
}

/*
 ===============================================================================
 get_nehalem_timer_freq

 returns the rate (in Hz) at which the LAPIC timer decrements for the Nehalem
 micro-architecture

 A return value of 0 is considered invalid and should not be used

*/
static unsigned get_nehalem_timer_freq(const _Uint32t family_model)
{
	const _Uint64t platform_info_msr = rdmsr(MSR_PLATFORM_INFO);
	if (debug_flag) {
		kprintf("MSR_PLATFORM_INFO: 0x%L,TSC multiplier: %u\n", platform_info_msr, (platform_info_msr >> 8) & 0xFF);
	}
	return 133333333;
}

/*
 ===============================================================================
 get_core2_timer_freq

 returns the rate (in Hz) at which the LAPIC timer decrements for Silvermont
 and Airmont micro-architectures

 A return value of 0 is considered invalid and should not be used

*/
static unsigned get_core2_timer_freq(const _Uint32t family_model)
{
	static const unsigned fsb_tbl_core2[] =
	{
			[0] = 266666666, [1] = 133333333,
			[2] = 200000000, [3] = 166666666,
			[4] = 333333333, [5] = 100000000,
			[6] = 400000000, [7] = 333333333,
			[8 ... 15] = 000000000,
	};
	const _Uint64t fsb_freq_msr = rdmsr(MSR_FSB_FREQ);
	const unsigned fsb_freq = fsb_tbl_core2[fsb_freq_msr & 0x7];

	/* the following obtains the bus ratio multiplier for the TSC */
	const _Uint64t perf_status_msr = rdmsr(MSR_PERF_STATUS);
	unsigned bus_ratio = 0;
	if ((perf_status_msr & (1u << 31)) != 0)
	{
		bus_ratio = (perf_status_msr >> 40) & 0x1F;
	}
	else
	{
		const _Uint64t platform_id_msr = rdmsr(MSR_PLATFORM_ID);
		bus_ratio = (platform_id_msr >> 8) & 0x1F;
	}
	if (debug_flag) {
		kprintf("TSC multiplier: %u\n", bus_ratio);
	}

	return fsb_freq;
}

/*
 ===============================================================================
 get_alder_lake_timer_freq

 returns the rate (in Hz) at which the LAPIC timer decrements for the Alder Lake
 micro-architecture

 A return value of 0 is considered invalid and should not be used

*/
static unsigned get_alder_lake_timer_freq(const _Uint32t family_model)
{
	unsigned base_freq = get_leaf_15_timer_freq(family_model);
	return base_freq;
}

/*
 ===============================================================================
 get_amston_lake_timer_freq

 returns the rate (in Hz) at which the LAPIC timer decrements for the Amston Lake
 micro-architecture

 A return value of 0 is considered invalid and should not be used

*/
static uint32_t get_amston_lake_timer_freq(const _Uint32t family_model)
{
	uint32_t base_freq = get_leaf_15_timer_freq(family_model);
	return base_freq;
}

/*
 ===============================================================================
 Intel_family_model_ids

*/
static const base_freq_tbl_t Intel_family_model_ids[] =
{
	{0x0400, NULL, "486"},
	{0x0401, NULL, "486"},
	{0x0402, NULL, "486"},
	{0x0403, NULL, "486"},
	{0x0404, NULL, "486"},
	{0x0405, NULL, "486"},
	{0x0407, NULL, "486"},
	{0x0408, NULL, "486"},
	{0x0409, NULL, "486"},
	{0x0501, NULL, "Pentium"},
	{0x0502, NULL, "Pentium"},
	{0x0503, NULL, "Pentium"},
	{0x0504, NULL, "Pentium"},
	{0x0507, NULL, "Pentium"},
	{0x0508, NULL, "Pentium"},
	{0x0509, NULL, "Quark"},
	{0x0601, NULL, "Pentium Pro"},
	{0x0603, NULL, "Pentium II"},
	{0x0605, NULL, "Pentium II"},
	{0x0606, NULL, "Pentium II"},
	{0x0607, NULL, "Pentium III"},
	{0x0608, NULL, "Pentium III"},
	{0x0609, NULL, "Pentium M"},
	{0x060A, NULL, "Pentium III"},
	{0x060B, NULL, "Pentium III"},
	{0x060D, NULL, "Pentium M"},
	{0x060E, NULL, "Core (Yonah)"},
	{0x060F, get_core2_timer_freq, "Core 2"},
	{0x0615, NULL, "Pentium M"},
	{0x0616, get_core2_timer_freq, "Core 2"},
	{0x0617, get_core2_timer_freq, "Core 2 (Penryn)"},
	{0x061A, get_nehalem_timer_freq, "Nehalem"},
	{0x061C, NULL, "Bonnell"},
	{0x061D, get_core2_timer_freq, "Core 2 (Penryn) Xeon X74xx"},
	{0x061E, get_nehalem_timer_freq, "Nehalem"},
	{0x0625, get_nehalem_timer_freq, "Westmere"},
	{0x0626, NULL, "Bonnell"},
	{0x0627, NULL, "Bonnell Atom Z24xx"},
	{0x062A, get_bridgewell_timer_freq, "Sandy Bridge"},
	{0x062C, get_nehalem_timer_freq, "Westmere"},
	{0x062D, get_bridgewell_timer_freq, "Sandy Bridge"},
	{0x062E, get_nehalem_timer_freq, "Nehalem"},
	{0x062F, get_nehalem_timer_freq, "Westmere"},
	{0x0635, NULL, "Bonnell"},
	{0x0636, NULL, "Bonnell"},
	{0x0637, get_silvermont_timer_freq, "Silvermont"},
	{0x063A, get_bridgewell_timer_freq, "Ivy Bridge"},
	{0x063C, get_bridgewell_timer_freq, "Haswell"},
	{0x063D, get_bridgewell_timer_freq, "Broadwell"},
	{0x063E, get_bridgewell_timer_freq, "Ivy Bridge"},
	{0x063F, get_bridgewell_timer_freq, "Haswell"},
	{0x0645, get_bridgewell_timer_freq, "Haswell"},
	{0x0646, get_bridgewell_timer_freq, "Haswell"},
	{0x064A, get_silvermont_timer_freq, "Silvermont"},
	{0x064D, get_silvermont_timer_freq, "Silvermont"},
	{0x065C, get_goldmont_timer_freq, "Goldmont"},
	{0x065F, get_denverton_timer_freq, "Goldmont-Denverton"},
	{0x0697, get_alder_lake_timer_freq, "Alder Lake"},
	{0x06BE, &get_amston_lake_timer_freq, "Amston Lake"},
	{0x0B01, NULL, "Knights Corner"},
	{0x0F00, NULL, "Pentium 4"},
	{0x0F01, NULL, "Pentium 4"},
	{0x0F02, NULL, "Pentium 4"},
	{0x0F03, NULL, "Nocona"},
	{0x0F03, NULL, "Prescott"},
	{0x0F04, NULL, "Nocona"},
	{0x0F04, NULL, "Prescott"},
	{0x0F06, NULL, "Nocona"},
	{0x0F06, NULL, "Prescott"},
};

/*
AMD	04	03	Am486
AMD	04	07	Am486
AMD	04	08	Am486
AMD	04	09	Am486
AMD	04	0A	Am486
AMD	04	0E	Am5x86
AMD	04	0F	Am5x86
AMD	05	00	K5
AMD	05	01	K5
AMD	05	02	K5
AMD	05	03	K5
AMD	05	06	K6
AMD	05	07	K6
AMD	05	08	K6-2
AMD	05	09	K6-3
AMD	05	0D	K6-3
AMD	06	01	K7
AMD	06	02	K7
AMD	06	03	K7
AMD	06	04	K7
AMD	06	06	K7
AMD	06	07	K7
AMD	06	08	K7
AMD	06	0A	K7
AMD	0F	04	K8
AMD	0F	05	K8
AMD	0F	07	K8
AMD	0F	08	K8
AMD	0F	0B	K8
AMD	0F	0C	K8
AMD	0F	0E	K8
AMD	0F	0F	K8
AMD	0F	14	K8
AMD	0F	15	K8
AMD	0F	17	K8
AMD	0F	18	K8
AMD	0F	1B	K8
AMD	0F	1C	K8
AMD	0F	1F	K8
AMD	0F	21	K8
AMD	0F	23	K8
AMD	0F	24	K8
AMD	0F	25	K8
AMD	0F	27	K8
AMD	0F	2B	K8
AMD	0F	2C	K8
AMD	0F	2F	K8
AMD	0F	41	K8
AMD	0F	43	K8
AMD	0F	48	K8
AMD	0F	4B	K8
AMD	0F	4C	K8
AMD	0F	4F	K8
AMD	0F	5D	K8
AMD	0F	5F	K8
AMD	0F	68	K8
AMD	0F	6B	K8
AMD	0F	6C	K8
AMD	0F	6F	K8
AMD	0F	7C	K8
AMD	0F	7F	K8
AMD	0F	C1	K8
AMD	10	02	K10
AMD	10	04	K10
AMD	10	05	K10
AMD	10	06	K10
AMD	10	08	K10
AMD	10	09	K10
AMD	10	0A	K10
AMD	11	03	K8
AMD	12	01	K10
AMD	14	01	Bobcat
AMD	14	02	Bobcat
AMD	15	01	Bulldozer
AMD	15	02	Piledriver
AMD	15	10	Piledriver
AMD	15	13	Piledriver
AMD	15	30	Steamroller
AMD	16	00	Jaguar
AMD	16	30	Jaguar (Puma)
AMD	17	71	Ryzen
*/

/*
 ===============================================================================
 AMD_family_model_ids

*/
static const base_freq_tbl_t AMD_family_model_ids[] =
{

};

/*
Centaur	5	4	WinChip C6
Centaur	5	8	WinChip 2
Centaur	5	9	WinChip 3
Centaur	6	6	WinChip-based
Centaur	6	7	C3
Centaur	6	8	C3
Centaur	6	9	C3-2
Centaur	6	A	C7
Centaur	6	D	C7
Centaur	6	F	Isaiah?

*/
/*
 ===============================================================================
 find_tbl_entry

 Locate the 'base_freq_tbl_t' for <family_model> and return a pointer to it
 or NULL if <family_model> could not be found

*/
static const base_freq_tbl_t *find_tbl_entry(const _Uint32t family_model)
{
	const base_freq_tbl_t *tbl_entry_p = NULL;
	unsigned i;

	if (is_intel_chipset())
	{
		for (i=0; i<NUM_ELTS(Intel_family_model_ids); i++)
		{
			if (Intel_family_model_ids[i].family_model == family_model)
			{
				tbl_entry_p = &Intel_family_model_ids[i];
				break;
			}
		}
	}
	else if (is_amd_chipset())
	{
		for (i=0; i<NUM_ELTS(AMD_family_model_ids); i++)
		{
			if (AMD_family_model_ids[i].family_model == family_model)
			{
				tbl_entry_p = &AMD_family_model_ids[i];
				break;
			}
		}
	}
	else
	{
		kprintf("%s(): Unsupported Manufacturer\n");
	}
	return tbl_entry_p;
}

/*
 ===============================================================================
 get_processor_base_freq

 returns the rate (in Hz) of the processor

 A return value of 0 is considered invalid and should not be used

*/
unsigned get_processor_base_freq(void)
{
#if 0
	/* Debug stuff */
	struct {
		_Uint32t ebx;
		_Uint32t edx;
		_Uint32t ecx;
	} regs = {0, 0, 0};
	_Uint32t eax = x86_cpuid(0x15, (void *)&regs);
	kprintf("CPUID:0x15, eax/ebx/edx/ecx: %x/%x/%x/%x\n", eax, regs.ebx, regs.edx, regs.ecx);

	regs.ebx = regs.edx = regs.ecx = 0;
	eax = x86_cpuid(0x16, (void *)&regs);
	kprintf("CPUID:0x16, eax/ebx/edx/ecx: %x/%x/%x/%x\n", eax, regs.ebx, regs.edx, regs.ecx);
	const _Uint64t perf_status_msr = rdmsr(MSR_PERF_STATUS);
	const _Uint64t platform_id_msr = rdmsr(MSR_PLATFORM_ID);
	const _Uint64t platform_info_msr = rdmsr(MSR_PLATFORM_INFO);
	const _Uint64t fsb_freq_msr = 0;//rdmsr(MSR_FSB_FREQ);
	kprintf("MSR(0xCD) = 0x%L, MSR(0xCE) = 0x%L, MSR(0x198) = 0x%L, MSR(0x17) = 0x%L\n", fsb_freq_msr, platform_info_msr, perf_status_msr, platform_id_msr);
#endif

	unsigned base_freq = 0;
	const _Uint32t family_model = cpu_family_model();

	if (debug_flag) {
		kprintf("CPU Family/Model: 0x%x/0x%x\n", cpu_family(), cpu_model());
	}

	const base_freq_tbl_t * const tbl_p = find_tbl_entry(family_model);
	if (tbl_p != NULL)
	{
		if (debug_flag > 1) {
			kprintf("Found 0x%x, uArch: %s\n", family_model, tbl_p->arch_str);
		}
		if (tbl_p->clk_get != NULL)
		{
			base_freq = tbl_p->clk_get(family_model);
		}
		else
		{
			kprintf("No assigned clk_get() function\n");
		}
	}
	else
	{
		 if (debug_flag) {
			kprintf("Unable to find CPU Family/Model: 0x%x/0x%x, try CPUID leaf 0x15\n", cpu_family(), cpu_model());
		}
		base_freq = get_leaf_15_timer_freq(family_model);
	}
	return base_freq;
}
