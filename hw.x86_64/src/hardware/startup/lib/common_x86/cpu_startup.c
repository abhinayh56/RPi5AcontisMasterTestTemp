/*
 * Copyright (c) 2008, 2014-2016, 2018, 2022, BlackBerry Limited.
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
 */





#include <startup.h>
#include <x86/intel_microcode.h>

X86_PREF(CPU_REGISTERS)	*boot_regs;
int						nx_supported;
int						meltdown_active = -1; // unspecified

//
// Perform CPU specific startup initialization.
// This code is hardware independant and should not have to be changed
// changed by end users.
//
//

/*
 * is_intel_cpu
 *
 * return whether or not this is an Intel CPU
 */
static int
is_intel_cpu(void) {
	struct {
		_Uint32t ebx;
		_Uint32t edx;
		_Uint32t ecx;
	} regs;
	x86_cpuid(0, (void *)&regs);

//	kprintf("CPUID[0]: eax:0x%x, ebx:0x%x, edx:0x%x, ecx:0x%x\n", x86_cpuid(0, 0), regs.ebx, regs.edx, regs.ecx);

	/* "GenuineIntel" as per Architecture reference manual */
	if ((regs.ebx == 0x756e6547) &&
		(regs.edx == 0x49656e69) &&
		(regs.ecx == 0x6c65746e)) {
		return 1;	// true
	} else {
		return 0;	// false
	}
}

static uint32_t
current_intel_microcode_rev(void)
{
    // See 9.11.7 in 325384-067US May 2018
    wrmsr(X86_IA32_BIOS_SIGN_ID, 0);
    x86_cpuid(1, NULL);
    return (uint32_t)(rdmsr(X86_IA32_BIOS_SIGN_ID) >> 32);
}

static void
update_intel_microcode(int const cpu)
{
    const struct intel_microcode_update **mcupdates = intel_microcode_updates();
    if (mcupdates == NULL) return;

    // If the running startup provides updates, then it knows that the CPU supports
    // the MSRs used below.  OTOH, those are supported since the first P6, so it's
    // been quite a while.
    _Uint32t const sig = x86_cpuid(1, NULL);
    _Uint32t const pi = (unsigned)((rdmsr(X86_IA32_PLATFORM_ID) >> 50) & 0x0000000000000007);
    uint32_t const cur_rev = current_intel_microcode_rev();
    while (*mcupdates != NULL) {
        if (((*mcupdates)->processor_sig == sig) &&
            (((*mcupdates)->processor_flags & (1 << pi)) != 0) &&
            ((*mcupdates)->revision > cur_rev)) {
            uintptr_t const addr = (uintptr_t)&(*mcupdates)->data;
            wrmsr(X86_IA32_BIOS_UPDT_TRIG, addr);

            // Now check if the update was successful
            uint32_t const new_rev = current_intel_microcode_rev();
            if (new_rev != (*mcupdates)->revision) {
                kprintf("Failed to update microcode (%x != %x)\n", new_rev,
                    (*mcupdates)->revision);
            } else {
                if (debug_flag) {
                    kprintf("CPU%d: Updated microcode to revision %x.\n", cpu, new_rev);
                }
            }
            break;
        }
        ++mcupdates;
    };
}

void
cpu_one_startup(int cpu) {
	x86_cputype(); // get the CPU features set

	if(cpu_features & X86_64_FEATURE_PAT) {
		uint64_t pat;

		// We need to modify the default PAT table
		// entry 2 will select WC (0x1) instead of the default UC- (0x7)
		// entry 2 resides in bits 16-18

		pat = rdmsr(X86_64_MSR_PAT);
		pat &= ~((uint64_t)X86_64_PAT_MASK << 16);
		pat |= ((uint64_t)X86_64_PAT_WC << 16);
		wrmsr(X86_64_MSR_PAT, pat);

#if defined(__X86_64__)
		// For X86_64, the paging unit is already turned on, so we have to
		// clear out any uncachable PTE's that are in the TLB. Do that
		// by clearing and setting the CR4[PGE] bit since that also
		// invalidates any global mappings that might be hanging around
		uint64_t const cr4 = rdcr4();
		ldcr4(cr4 & ~(uint64_t)X86_64_CR4_PGE);
		ldcr4(cr4 | X86_64_CR4_PGE);
#endif
	}

	unsigned	flags;
	uint32_t	info[3];

	flags = x86_cpuid(0x80000000, 0);
	if(flags >= 0x80000001) {
		x86_cpuid(0x80000001, info);
		if(info[1] & (1 << 20)) {
			// Chip supports NX, turn it on
			wrmsr(X86_64_MSR_EFER, rdmsr(X86_64_MSR_EFER) | X86_64_EFER_NXE);
			nx_supported = 1;
		}
	}

	// check to ensure that MISC_ENABLE[22] (Limit CPUID Max Val which limits
	// some of x86 instructions causing kernel faults) has not been
	// set in the BIOS. If it has, we undo that limitation
	//
	// Caveats:
	//
	// If this is a really old processor (that does not support cpuid > 3),
	// MISC_ENABLE[22] does not even exist so testing for it on that platform
	// may lead to an incorrect decision and perhaps even a fault if the result
	// of the decision is to write to the MSR.
	//
	// Furthermore, in order to determine whether or not the MISC_ENABLE[22]
	// (ie. MSR[1A0].22) does exist, the recommended procedure is for the BIOS
	// to issue CPUID.(EAX=0H) before setting the bit and if EAX[7..0] is > 3
	// then the bit is supported and if its <= 3 it is not. The problem is that
	// if the bit is already set in the BIOS, we get back <=3 thus rendering the
	// CPUID.(EAX=0H) check useless in determining the existence of the
	// MISC_ENABLE[22] bit.
	//
	// We mitigate against these 2 caveats with the is_intel_cpu() and
	// cpu_family_model() checks. If this is not an Intel CPU, we don't go any
	// further with the procedure since we cannot assume anything about Display
	// Model and Family values for non Intel devices. If it is an Intel CPU, the
	// next thing we try to determine is whether or not MSR[1A0] bit 22 is
	// supported and we use the Display Family/Model (DF) information to
	// determine that as documented in the "Introduced as Architectural MSR"
	// column of the "IA-32 Architectural MSR's" Table 35-2 (starting on pg 35-2
	// Vol C of the Sep 2013 version of the Architectural Reference Manual).
	//
	// Now, there's still one complication. This column lists the DF at which a
	// MSR feature was introduced but unfortunately the DF numbers do not
	// increase numerically in conjunction with processor releases making a
	// numerical check for >= 0xF03 incorrect. For example, the Sandy Bridge
	// G850 processor (on which the entire CPUID problem was originally
	// uncovered) reports a DF of 0x62A which is numerically < 0xF03 but was
	// released much later than the Pentiums that report a Family of 0xF.
	// Therefore, some additional qualification will is required and I am going
	// to rely on Table 35-1 (just before aforementioned table 35-2) which lists
	// the families and their DF numbers in what looks to be, and what I hope is
	// a descending order of release date (ie. newest at the top, oldest at the
	// bottom). Using the documented "introduced as" value for MSR[1A0].22 of
	// 0xF03, it would then seem reasonable to conclude that for Family 6
	// processors (the only Family used so far that is newer than Family 0xF),
	// everything greater than 0x609 would have MSR[1A0].22 supported. Given
	// that the numeric anomaly appears to be Family 0xF, it would then seem
	// reasonable to conclude that only Family 0xF is special (and potentially
	// older than a Family 6 or any other Family value) and that the following
	// logic is sufficient assuming Intel doesn't release a new Family with a
	// value less than 6
	//
	//		MSR[1A0].22 support = ((family 0xF) && (DF >= 0xF03)) ||
	//								((family != 0xF) && (DF > 0x609))
	//
	// And as if we needed one more complication, the documented max basic cpuid
	// value for many of Intel's processors is < 0xD however it is believed that
	// if X86_FEATURE2_XSAVE is set (ie. ECX[26] is set after CPUID.(EAX=1H))
	// then issuing CPUID.(EAX=0DH, ECX=0) should be valid so no explicit check
	// for a maximum cpuid value >3 <=0xD will be made
	//
	if(is_intel_cpu()) {
		const _Uint32t family_model = cpu_family_model();
		const _Uint32t model = cpu_model();
		const _Uint32t family = cpu_family();
		const int msr_1A0_bit_22_exist = ((family == 0xF) && (model >= 0x03)) ||
										 ((family != 0xF) && (family_model > 0x609));

		if(msr_1A0_bit_22_exist) {
			volatile _Uint32t msr = rdmsr(0x1A0);
			if((msr & (1u << 22)) != 0) {
				msr &= ~(1u << 22);
				wrmsr(0x1A0, msr);
			}
		}
#ifdef AUTO_ENABLE_MELTDOWN
		if(meltdown_active < 0) {
			meltdown_active = 1;
		}
#endif
        update_intel_microcode(cpu);
	}
}

void
cpu_startup() {
	cpu_one_startup(0);
}
