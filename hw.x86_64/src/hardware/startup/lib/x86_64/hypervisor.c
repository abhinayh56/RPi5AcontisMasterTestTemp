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

static enum {
    VIRT_VMX,
    VIRT_SVM,
    VIRT_UNKNOWN
} virt = VIRT_UNKNOWN;

/**
 * This function evaluates the provided hypervisor flags and returns the closest
 * set of flags that the hardware can actually do.
 * @param option_flags HYP_FLAG_* options that control setup for hypervisor
 * @returns closest options that the hardware can do
 */
hyp_flags_t
arch_hypervisor_validate_flags(hyp_flags_t  const option_flags) {
    if (option_flags == HYP_FLAG_INVALID) {
        crash("Unrecognized hypervisor option flag\n");
    }

    uint32_t regs[4];
    x86_cpuid1(0, NULL, &regs[0], &regs[2], &regs[1]);
    regs[3] = 0;
    const char * const brand = (void *)&regs[0];
    if (strcmp(brand, "GenuineIntel") == 0) {
        virt = VIRT_VMX;
    } else if (strcmp(brand, "AuthenticAMD") == 0) {
        virt = VIRT_SVM;
    } else {
        virt = VIRT_UNKNOWN;
    }

    if (option_flags & HYP_FLAG_ENABLED) {
        switch (virt) {
        case VIRT_VMX:
            // In the future, more nuanced control over hypervisor modes may be
            // required but for now, it will be a simple on/off switch
            x86_cpuid1(1, NULL, NULL, &regs[0], NULL);

            if ((regs[0] & X86_64_FEATURE2_VMX) == 0) {
                crash("VMX not supported\n");
            }
            break;

        case VIRT_SVM:
            x86_cpuid1(0x80000001, NULL, NULL, &regs[0], NULL);
            if ((regs[0] & X86_64_EXTFEAT_ECX_SVM) == 0U) {
                crash("SVM is not supported\n");
            }
            break;

        default:
            crash("Unsupported virtualization platform");
        }

        return HYP_FLAG_ENABLED;
    }

    return HYP_FLAG_DISABLED;
}


/**
 * This function is used to configure hypervisor support on all cores. It
 * expects to be called first on the bootcore, and then secondary cores.
 * hypervisor init will only succeed if all secondary cores can be configured
 * to match the bootcore.
 *
 * @param cpunum     to distinguish boot and secondary cores
 * @param hyp_flags  init flags
 */
void
arch_hypervisor_init(unsigned const cpunum, hyp_flags_t const hyp_flags) {
    switch(hyp_flags) {
    case HYP_FLAG_DISABLED:
        if (debug_flag && (cpunum == 0)) {
            kprintf("Hypervisor support disabled\n");
        }
        break;
    case HYP_FLAG_ENABLED:
        if (debug_flag && (cpunum == 0)) {
            kprintf("Hypervisor support enabled\n");
        }
        break;
    default:
        crash("Unknown required hypervisor mode\n");
        break;
    }
}

/**
 * Indicate whether we're doing AMD or Intel virtualization.
 * @param   sysp_hypinfo    hypinfo section of syspage
 */
void
arch_hypervisor_syspage_init(struct hypinfo_entry * const sysp_hypinfo)
{
    sysp_hypinfo->flags &= ~HYPINFO_VIRT_MASK;

    switch (virt) {
    case VIRT_VMX:
        sysp_hypinfo->flags |= HYPINFO_VIRT_SET(HYPINFO_VIRT_VMX);
        break;

    case VIRT_SVM:
        sysp_hypinfo->flags |= HYPINFO_VIRT_SET(HYPINFO_VIRT_SVM);
        break;

    default:
        sysp_hypinfo->flags |= HYPINFO_VIRT_MASK;
        break;
    }
}
