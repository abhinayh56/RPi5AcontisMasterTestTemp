/*
 * Copyright (c) 2007, 2022, BlackBerry Limited.
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#include <startup.h>

//
// Initialize cpuinfo structures in the system page.
// This code is hardware independent and should not have to be changed
// changed by end users.
//

/**
 * Determines if the chip can fully support PCIDs.
 * Some x86_64 processors support PCID, but lack support for the INVPCID
 * instruction. In that case we cannot properly use PCIDs, and consequently turn
 * off support for the feature.
 */
static int
check_pcid_support(unsigned const flags)
{
    int has_pcid;
    int has_invpcid;

    if ((flags & X86_64_CPU_PCID) == 0) {
        // CPU does not support PCIDs at all.
        has_pcid = 0;
        has_invpcid = 0;
    } else {
        has_pcid = 1;

        // Query for invpcid support.
        unsigned    eax;
        unsigned    ebx;
        unsigned    ecx;
        unsigned    edx;
        x86_cpuid2(7, 0, &eax, &ebx, &ecx, &edx);
        if (ebx & (1 << 10)) {
            has_invpcid = 1;
        } else {
            if (debug_flag > 0) {
                kprintf("No INVPCID, turning off PCID support\n");
            }
            has_invpcid = 0;
        }
    }

    if (debug_flag > 1) {
        kprintf("PCID: %s INVPCID: %s\n", has_pcid ? "Yes" : "No",
                has_invpcid ? "Yes" : "No");
    }

    return has_invpcid;
}

void
init_one_cpuinfo(unsigned cpunum) {
	struct cpuinfo_entry	*cpu;
	unsigned long			flags = 0;
	unsigned				name;
	unsigned				cpu_type;
	char					buf[128];

	/*
	 * Ensure that the smp_hwcoreid is set properly. For x86/x86_64 this is
	 * done in apic_smp_start(). The problem is that apic_smp_start() won't
	 * be called for a single core configuration or if -P1 is provided on the
	 * startup command line
	 */
	struct cpuinfo_entry *cep = lsp.cpuinfo.p;
	const unsigned this_cpu_lapic_id = lapic_id();

	if (debug_flag > 0) kprintf("**checking smp_hwcoreid for cpu %u\n", cpunum);
	if (cep[cpunum].smp_hwcoreid != this_cpu_lapic_id) {
		kprintf("**Correcting CPU %u smp_hwcoreid from 0x%x to 0x%x\n",
					cpunum, cep[cpunum].smp_hwcoreid, this_cpu_lapic_id);
		cep[cpunum].smp_hwcoreid = this_cpu_lapic_id;
	}

	//
	// Run an instruction maze to determine the cpu type.
	//
	cpu_type = x86_cputype();

	//
	// Set cpu->name
	//
	if(x86_cpuid_string(buf, sizeof buf) == -1) {
		ultoa_end(cpu_type,buf,10);
	}
	name = add_string(buf);
	cpu = &lsp.cpuinfo.p[cpunum];
	cpu->name = name;
	cpu->cpu = cpu_type;
	if(cpu_features_set) {
		flags |= X86_64_CPU_CPUID;
		if(cpu_features & X86_64_FEATURE_TSC) flags |= X86_64_CPU_RDTSC;
		if(cpu_features & X86_64_FEATURE_MMX) flags |= X86_64_CPU_MMX;
		if(cpu_features & X86_64_FEATURE_CMOV) flags |= X86_64_CPU_CMOV;
		if(cpu_features & X86_64_FEATURE_FPU) flags |= CPU_FLAG_FPU;
		if(cpu_features & X86_64_FEATURE_PSE) flags |= X86_64_CPU_PSE;
		if(cpu_features & X86_64_FEATURE_PAE) flags |= X86_64_CPU_PAE;
		if(cpu_features & X86_64_FEATURE_MTRR) flags |= X86_64_CPU_MTRR;
		if(cpu_features & X86_64_FEATURE_PAT) flags |= X86_64_CPU_PAT;
		if(cpu_features & X86_64_FEATURE_PGE) flags |= X86_64_CPU_PGE;
		if(cpu_features & X86_64_FEATURE_SEP) flags |= X86_64_CPU_SEP;
		if(cpu_features & X86_64_FEATURE_FXSR) flags |= X86_64_CPU_FXSR;
		if(cpu_features & X86_64_FEATURE_SIMD) flags |= X86_64_CPU_SIMD;
		if(cpu_features & X86_64_FEATURE_SSE2) flags |= X86_64_CPU_SSE2;
		if(cpu_features2 & X86_64_FEATURE2_AVX) flags |= X86_64_CPU_AVX;
		if(cpu_features2 & X86_64_FEATURE2_XSAVE) flags |= X86_64_CPU_XSAVE;
		if(cpu_features2 & X86_64_FEATURE2_PCID) flags |= X86_64_CPU_PCID;
		if(cpu_features2 & X86_64_FEATURE2_RDRAND) flags |= X86_64_CPU_RDRAND;
		if(cpu_features3 & X86_64_FEATURE3_RDSEED) flags |= X86_64_CPU_RDSEED;
		if(nx_supported) {
			flags |= X86_64_CPU_NX;
		}
#if defined(__X86_64__)
		else {
			crash("No-execute page permission support has been been disabled. Check your BIOS settings");
		}
#endif
		fix_tsc();
		if(check_pcid_support(flags) == 0) {
			flags &= ~X86_64_CPU_PCID;
		}
		if (cpu_tsc & X86_64_TSC_INVARIANT) flags |= X86_64_CPU_INVARIANT_TSC;
	} else {
#if defined(__X86_64__)
		crash("expecting CPUID support");
#else
		if(x86_fputype()) flags |= CPU_FLAG_FPU;
#endif
	}
	if(cpu->cpu >= 486) {
		flags |= X86_64_CPU_WP | X86_64_CPU_INVLPG | X86_64_CPU_BSWAP;
	}
	if(cpu->cpu >= 586) {
		unsigned cr4 = rdcr4();

		// Allow machine check exceptions, but we're not initializing
		// rest of the machine check architecture at this time.
		if(cpu_features & X86_64_FEATURE_MCE)  cr4 |= X86_64_CR4_MCE;
		if(flags & X86_64_CPU_PGE)  cr4 |= X86_64_CR4_PGE;
		if(flags & X86_64_CPU_PSE)  cr4 |= X86_64_CR4_PSE;
		if((flags & X86_64_CPU_PAE) && (paddr_bits > 32))  cr4 |= X86_64_CR4_PAE;
		if(flags & X86_64_CPU_FXSR)  cr4 |= X86_64_CR4_OSFXSR;
		ldcr4(cr4);
	}

	if(shdr->flags1 & STARTUP_HDR_FLAGS1_VIRTUAL) flags |= CPU_FLAG_MMU;

	//
	// Set cpu->flags.
	//
	cpu->flags = flags;

	cpu->speed = (cpu_freq != 0) ? (cpu_freq / 1000000) : x86_cpuspeed(cpunum);
}

void
init_cpuinfo() {
	unsigned				num;
	unsigned				i;
	struct cpuinfo_entry	*cpu;

	num = lsp.syspage.p->num_cpu;
	cpu = set_syspage_section(&lsp.cpuinfo, sizeof(*lsp.cpuinfo.p) * num);
	for(i = 0; i < num; ++i) {
		cpu[i].ins_cache = system_icache_idx;
		cpu[i].data_cache = system_dcache_idx;
	}
	init_one_cpuinfo(0);
}
