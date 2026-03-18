/*
 * $QNXLicenseC:
 * Copyright 2016, QNX Software Systems.
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
#include <sys/syspage.h>

/**
 * Macros - Intel
 */

#define INTEL_VENDOR_ID	"GenuineIntel"

/* Intel base clock frequencies in khz */
#define INTEL_BCLK_83MHZ	83000
#define INTEL_BCLK_100MHZ	100000
#define INTEL_BCLK_133MHZ	133330
#define INTEL_BCLK_167MHZ	166670

/* Intel MSR */
#define MSR_PLATFORM_INFO	0xce


/**
 * Macros - AMD
 */
#define AMD_VENDOR_ID	"AuthenticAMD"

#define GET_ECX_APIC_CORESIZE(reg)	(((reg) & (0x3 << 12)) >> 12)
#define GET_ECX_NC(reg)				((reg) & 0xef)
#define AMD_PMM7_CPULowPwrEn (1 << 0)
#define AMD_PMM7_NBLowPwrEn (1 << 1)

/**
 * Macros - Common
 */
#define NANO_SCALE	(-9)


/**
 * Functions - Intel
 */

/**
 * Get CPU base clock frequency in HZ, documented in
 * 64-ia-32-architectures-software-developer-system-programming-manual
 * @return	base clock frequency in HZ
 */
static uint64_t get_intel_bclk_hz()
{
	unsigned khz = 0;
	const uint32_t family_model = cpu_family_model();

	switch (family_model) {
		/* SandyBridge */
		case 0x062a:
		case 0x062d:
		/* IvyBridge */
		case 0x063a:
		case 0x063e:
		/* Haswell */
		case 0x063c:
		case 0x063f:
		case 0x0645:
		case 0x0646:
		/* Broadwell */
		case 0x063d:
		case 0x0647:
		case 0x064f:
		case 0x0656:
		/* Skylake */
		case 0x064e:
		case 0x065e:
			khz = INTEL_BCLK_100MHZ;
			break;

		default:
			khz = 0;
	}

	return (khz * 1000);
}

/**
 * Functions - AMD
 */
static int amdk8_num_processors(void)
{
	unsigned info[3];
	unsigned num_cpus, core_id_size, cores_per_cpu;

	num_cpus = lsp.syspage.p->num_cpu;
	if ( num_cpus == 1 ) {
		return 0;
	}

	(void)x86_cpuid(0x80000008, info);
	core_id_size = GET_ECX_APIC_CORESIZE(info[2]);
	if ( core_id_size == 0 ) {
		if ( !(cpu_features & X86_64_FEATURE_HTT) ) {
			return 1;
		} else {
			cores_per_cpu = GET_ECX_NC(info[2]) + 1;
		}
	} else {
		cores_per_cpu = (1 << core_id_size);
	}
	return num_cpus/cores_per_cpu;
}

static void fix_amdk8_tsc(void)
{
	uint8_t val;
	int		proc, num_procs;

	num_procs = amdk8_num_processors();
	for ( proc = 0; proc < num_procs; proc++ ) {

		val = pci_read_cfg8( 0, 0x18 + proc, 3, 0x87 );

		val &= ~(AMD_PMM7_CPULowPwrEn|AMD_PMM7_NBLowPwrEn);

		pci_write_cfg8(0, 0x18 + proc, 3, 0x87, val);
	}
}

void fix_tsc(void)
{
	char		id[CPU_VENDOR_ID_SIZE];
	unsigned	family;

	x86_cpuid( 0, id );
	id[CPU_VENDOR_ID_SIZE - 1] = '\0';
	family = cpu_family();
	if (cpu_tsc & X86_64_TSC_INVARIANT) {
		return;
	}

	if ( !strcmp( id, AMD_VENDOR_ID ) && family == 0xf ) {
		fix_amdk8_tsc();
	}
}

/**
 * Functions - Common
 */

/**
 * Calibrate the frequency of TSC in Hz, using the default timer
 * @return	TSC cycles per second (Hz)
 */
uint64_t calibrate_tsc(unsigned long ns)
{
	unsigned long ticks;
	unsigned start_tick;
	uint64_t init_diff;
	uint64_t tick_diff;
	uint64_t start_tsc;
	uint64_t end_tsc;
	uint64_t elapsed_time_nano;
	uint64_t cycles_per_sec;
	unsigned factor;
	unsigned scale_factor;

	ticks = timer_ns2tick(ns);
	start_tick = timer_start();

	/* preparation: wait for the beginning of a tick to sync with rdtsc */
	do {
		init_diff = timer_diff(start_tick);
	} while (init_diff == 0);
	ticks += init_diff;

	/* actual benchmark */
	rdtsc_ordered(&start_tsc);
	__asm__ __volatile__ (".align 16");
	do {
		tick_diff = timer_diff(start_tick);
	} while (tick_diff < ticks);

	rdtsc_ordered(&end_tsc);

	/* calculation based on end_tsc, start_tsc and tick_diff */
	tick_diff -= init_diff;	/* init_diff is measured prior to the beginning of benchmark */

	/* Please, refer to [A Note About Using qtime::timer_rate And qtime::timer_scale]
	 * in services/system/doc/clock.txt and services/system/ker/nano_clock.c:clock_start()
	 * and jiffy_to_ns() for the detail
	 */
    if (lsp.qtime.p->timer_scale < NANO_SCALE) {
        factor = (unsigned)(NANO_SCALE - lsp.qtime.p->timer_scale);
    } else {
        factor = (unsigned)(lsp.qtime.p->timer_scale - NANO_SCALE);
    }

    scale_factor = 1;
    while (factor != 0) {
        scale_factor *= 10;
        --factor;
    }

	if (lsp.qtime.p->timer_scale < NANO_SCALE) {	/* e.g. if nano_conversion_scale is -6, it means 'timer_scale' is femto */
		elapsed_time_nano = ((tick_diff * (lsp.qtime.p->timer_period)) + (scale_factor >> 1)) / scale_factor;	/* to nanoseconds */
	} else {	/* e.g if nano_conversion_scale is 3, it means 'timer_scale' is micro */
		elapsed_time_nano = (tick_diff * (lsp.qtime.p->timer_period)) * scale_factor;	/* to nanoseconds */
	}

	cycles_per_sec = ((end_tsc - start_tsc) * 1000000000) / elapsed_time_nano;

	return cycles_per_sec;
}

/**
 * Get invariant(constant) CPU TSC(Time Stamp Counter) based on MSR_PLATFORM_INFO
 * , documented in 64-ia-32-architectures-software-developer-system-programming-manual
 * @return	Invariant TSC = (Maximum Non-Turbo Ratio) * (Base clock).
 *			Otherwise, it returns 0 which implies the CPU TSC is not supported
 *			in the current implementation
 */
uint64_t get_invariant_tsc(void)
{
	char id[CPU_VENDOR_ID_SIZE];

	x86_cpuid(0, id);
	id[CPU_VENDOR_ID_SIZE - 1] = '\0';

	if (strcmp(id, INTEL_VENDOR_ID) == 0) {	/* Intel */
		volatile uint8_t non_turbo_ratio = 1;
		uint64_t bclk_hz = 0;

		/* get maximum non-turbo ratio from MSR */
		non_turbo_ratio = (rdmsr(MSR_PLATFORM_INFO) >> 8) & 0xff;
		bclk_hz = get_intel_bclk_hz();

		return (non_turbo_ratio * bclk_hz);
	} else if (strcmp(id, AMD_VENDOR_ID) == 0) {	/* AMD */
		/*According to AMD BIOS and Kernel Develper's Guide, The TSC increments at the P0 frequency */
		return 0;
	} else	{	/* Unknown */
		return 0;
	}

	return 0;
}
