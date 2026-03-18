/*
 * Copyright (c) 2023, BlackBerry Limited.
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
#include "common_x86/hv_guest.h"

enum hypervisor_host {
    HH_NOT_CHECKED,
    HH_NONE,
    HH_UNKNOWN,
    HH_QVM,
    HH_KVM,
    HH_QEMU,
    HH_XEN,
    HH_HYPERV,
    HH_VMWARE
} hypervisor_host = HH_NOT_CHECKED;
static uint32_t max_hh_cpuid;
static char hh_id_string[13];
static int hyperv_compatible = 0;
static int hyperv_freqs_support = 0;

#define HYPER_V_SIGNATURE               0x31237648U // 'Hv#1'
#define HYPERV_PART_PRIV_MASK_FREQS     (1U << 11)
#define HYPERV_FEAT_ID_EDX_TIMER_FREQS  (1U << 8)

#define CPUID_HYPERVISOR_BASE       0x40000000U
#define CPUID_HYPERV_NEUTRAL_IID    0x40000001U
#define CPUID_HYPERV_FEATURES       0x40000003U
#define CPUID_QVM_TSC_FREQ          0x40000002U
#define CPUID_HV_TIMING_INFO        0x40000010U

#define HV_X64_MSR_TSC_FREQUENCY    0x40000022U
#define HV_X64_MSR_APIC_FREQUENCY   0x40000023U

/**
 * Discover if we're under an hypervisor, and which.
 * @post ::hypervisor_host is set to the appropriate value
 */
static void
detect_hypervisor_host(void)
{
    if (hypervisor_host != HH_NOT_CHECKED) {
        return;
    }

    // CPUID.01H.ECX[31] is RES0 for real hardware.
    // AMD mentions that it's reserved for an hypervisor to indicate
    // guest status.
    uint32_t regs[3];
    x86_cpuid(1, regs);
    if ((regs[2] & (1U << 31)) == 0) {
        hypervisor_host = HH_NONE;
        return;
    }

    hypervisor_host = HH_UNKNOWN;

    // The range 4000_0000H through 4000_00FFH is reserved for hypervisor use
    max_hh_cpuid = x86_cpuid(CPUID_HYPERVISOR_BASE, regs);
    memcpy(hh_id_string, &regs[0], 4);
    memcpy(hh_id_string + 4, &regs[2], 4);
    memcpy(hh_id_string + 8, &regs[1], 4);
    hh_id_string[12] = '\0';

    if (debug_flag > 1) {
        kprintf("Max Hypervisor Host CPUID leaf 0x%x\nHypervisor Host signature %s\n",
            max_hh_cpuid, hh_id_string);
    }

    static struct {
        const char *signature;
        enum hypervisor_host host;
    } known_signatures[] = {
        { "QNXQVMBSQG",     HH_QVM },
        { "KVMKVMKVM",      HH_KVM },
        { "TCGTCGTCGTCG",   HH_QEMU },
        { "Microsoft Hv",   HH_HYPERV },
        { "XenVMMXenVMM",   HH_XEN },
        { "VMwareVMware",   HH_VMWARE },
    };

    for (uint32_t i = 0; i < sizeof(known_signatures)/sizeof(known_signatures[0]); ++i) {
        if (strcmp(hh_id_string, known_signatures[i].signature) == 0) {
            hypervisor_host = known_signatures[i].host;
            break;
        }
    }

    // Hyper-V compatible hypervisors have a signature in CPUID.4000_0001H.EAX
    // and a set of flags describing what they support in CPUID.4000_0003H
    if (max_hh_cpuid >= CPUID_HYPERV_NEUTRAL_IID) {
        uint32_t eax = x86_cpuid(CPUID_HYPERV_NEUTRAL_IID, NULL);
        if (eax == HYPER_V_SIGNATURE) {
            if (debug_flag > 1) {
                kprintf("Microsoft Hyper-V compliant Hypervisor Host\n");
            }
            hyperv_compatible = 1;
            if (max_hh_cpuid >= CPUID_HYPERV_FEATURES) {
                eax = x86_cpuid(CPUID_HYPERV_FEATURES, &regs);
                hyperv_freqs_support = ((eax & HYPERV_PART_PRIV_MASK_FREQS) != 0) &&
                    ((regs[2] & HYPERV_FEAT_ID_EDX_TIMER_FREQS) != 0);
            }
        }
    }
}

/**
 * Helper function to return a frequency value.
 * @param   type    a string that describe the frequency, for debug
 * @param   freq    pointer where to store the value
 * @param   value   value to set
 * @return always 1, to simplify error paths.
 */
static int
hvg_set_freq(const char * const type, uint64_t * const freq, uint64_t const value)
{
    *freq = value;
    if (debug_flag > 1) {
        kprintf("Using hypervisor-provided %s frequency of %L Hz\n",
            type, *freq);
    }
    return 1;
}

/**
 * Helper function to return a TSC frequency.
 * @param   freq    pointer where to store the value
 * @param   value   value to set
 */
static int
hvg_set_tsc(uint64_t * const freq, uint64_t const value)
{
    return hvg_set_freq("TSC", freq, value);
}

/**
 * Helper function to return a LAPIC timer frequency.
 * @param   freq    pointer where to store the value
 * @param   value   value to set
 */
static int
hvg_set_lapic(uint64_t * const freq, uint64_t const value)
{
    return hvg_set_freq("LAPIC", freq, value);
}

/**
 * Indicate the frequency of the TSC, if running as a guest.
 * @param   freq     pointer where to store the value
 * @return  1 if the value was set, 0 otherwise.
 */
int
hvg_tsc_frequency(uint64_t * const freq)
{
    detect_hypervisor_host();

    if (hyperv_compatible && hyperv_freqs_support) {
        return hvg_set_tsc(freq, rdmsr(HV_X64_MSR_TSC_FREQUENCY));
    }

    if (max_hh_cpuid >= CPUID_HV_TIMING_INFO) {
        uint32_t const tsc_freq = x86_cpuid(CPUID_HV_TIMING_INFO, NULL);
        if (tsc_freq > 0) {
            return hvg_set_tsc(freq, 1000UL * tsc_freq);
        }
    }

    if ((hypervisor_host == HH_QVM) &&
                (max_hh_cpuid >= CPUID_QVM_TSC_FREQ)) {
        // Reasonably recent versions of QVM indicate the TSC/LAPIC
        // frequency in a CPUID leaf.  No need to sample it.

        uint32_t regs[3];
        uint32_t const eax =  x86_cpuid(CPUID_QVM_TSC_FREQ, regs);
        return hvg_set_tsc(freq, (uint64_t)eax | ((uint64_t)regs[0] << 32));
    }

    return 0;
}

/**
 * Indicate the frequency of the LAPIC, if running as a guest.
 * @param   freq    pointer where to store the value
 * @return  1 if the value was set, 0 otherwise.
 */
int
hvg_lapic_frequency(uint64_t * const freq)
{
    detect_hypervisor_host();

    if (hyperv_compatible & hyperv_freqs_support) {
        return hvg_set_lapic(freq, rdmsr(HV_X64_MSR_APIC_FREQUENCY));
    }

    uint32_t regs[3];
    if (max_hh_cpuid >= CPUID_HV_TIMING_INFO) {
        x86_cpuid(CPUID_HV_TIMING_INFO, regs);
        if (regs[0] > 0) {
            return hvg_set_lapic(freq, 1000UL * regs[0]);
        }
    }

    if ((hypervisor_host == HH_QVM) &&
                (max_hh_cpuid >= CPUID_QVM_TSC_FREQ)) {
        // Reasonably recent versions of QVM indicate the TSC/LAPIC
        // frequency in a CPUID leaf.  No need to sample it.

        uint32_t const eax =  x86_cpuid(CPUID_QVM_TSC_FREQ, regs);
        return hvg_set_lapic(freq, (uint64_t)eax | ((uint64_t)regs[0] << 32));
    }

    if ((hypervisor_host == HH_KVM) || (hypervisor_host == HH_QEMU)) {
        // Hard-coded as 1 GHz
        return hvg_set_lapic(freq, 1000UL * 1000UL * 1000UL);
    }

    return 0;
}
