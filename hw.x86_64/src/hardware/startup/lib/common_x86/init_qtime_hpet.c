/*
 * $QNXLicenseC:
 * Copyright 2008, 2020, 2022, 2023, 2025 BlackBerry Limited.
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
 * Previously this was used to initialize the HPET device on a system. As of
 * SDP8, we are making exclusive use of the LAPIC timer instead. This code
 * is now used to disable any available HPET devices, in the scenarios where
 * they are found to cause boot issues due to spurious interrupts coming out of
 * startup (HW-24239).
 */

#include <startup.h>
#include <x86/pci_misc.h>
#include <x86/intel_misc.h>
#include "ich_chipset.h"
#include "hw/acpi.h"

/* macro to access an HPET register at offset <o> */
#define HPET(o)			hpet[(o)/sizeof(*hpet)]

static volatile uint64_t *hpet;
static paddr_t hpet_paddr = NULL_PADDR;

static int
find_hpet_acpi(void) {
	unsigned			len;
	paddr_t				paddr = NULL_PADDR;

	// Try using ACPI tables to locate the HPET block
	for( ;; ) {
		paddr = acpi_find_table_next(ACPI_HPET_SIGNATURE, &len, paddr);
		if(paddr == NULL_PADDR) break;

		const acpi_hpet *const tbl = startup_memory_map(len, paddr, PROT_READ);
		unsigned const as = tbl->BASE_ADDRESS.Address_Space_ID;
		paddr_t chip_paddr = tbl->BASE_ADDRESS.Address.sm;
		startup_memory_unmap((void *)tbl);
		if(as == ACPI_GAS_ASID_SYSTEM_MEMORY) {
			// We're good to go....
			hpet_paddr = chip_paddr;
			return 1;
		}
	}
	return 0;
}

static int
find_hpet_chipset(void) {

	/* On ICH's the HPET address space can be at 1 of 4 locations and must be enabled. This
	 * is controlled through the HPTC registers on ICH's 6 thru 10 and in the LPC GEN_CNTL
	 * register of the ICH5. On SCH (poulsbo) and E6xx (Tunnel Creek), the location of the
	 * HPET's is fixed at 0xFED00000 (ie. there is no HPTC register).
	 * For Apollo Lake, the HPET can also be located at 1 of 4 locations however the device
	 * in which that information is contained is not the same as for the ICH's
	 */
	int found = 0;

	if (is_intel_chipset())
	{
		switch(get_LPC_devid())
		{
			case INTEL_LEGACY_PIIX4_LPC_DIDS:
			{
				// PIIX4
				break;
			}
			case INTEL_POULSBO_SCH_LPC_DIDS:	// SCH (poulsbo)
			case INTEL_TUNNEL_CREEK_LPC_DIDS:	// Tunnel Creek
			case INTEL_BAYTRAIL_LPC_DIDS:		// Baytrail
			case INTEL_RANGELEY_LPC_DIDS:		// Rangeley-based systems
			case INTEL_TOLAPAI_LPC_DIDS:		// Tolapai
			{
				hpet_paddr = 0xFED00000U;
				found = 1;
				break;
			}
			case INTEL_APOLLO_LAKE_LPC_DIDS:	// Apollo Lake
			case INTEL_SKYLAKE_SKU_UY_LPC_DIDS:	// Skylake U and Y SKU's
			case INTEL_SKYLAKE_LPC_DIDS:		// Skylake full featured (A, B, H, Q, Z SKU's)
			case INTEL_DENVERTON_LPC_DIDS:		// Denverton
			case INTEL_ANDERSON_LAKE_LPC_DIDS:      // Anderson Lake
			case INTEL_COFFEELAKE_LPC_DIDS:
			case INTEL_TIGERLAKE_LPC_DIDS:
			case INTEL_ELKHARTLAKE_LPC_DIDS:
			case INTEL_RAPTORLAKE_LPC_DIDS:
			case INTEL_ALDERLAKE_LPC_DIDS:
			case INTEL_AMSTONLAKE_LPC_DIDS:
			case INTEL_SERIES_C62x_SUPER_SKUS_LPC_DIDS:	// C62x Series PCH Super SKUs
			case INTEL_SERIES_C62xA_SUPER_SKUS_LPC_DIDS:	// C62xA Series PCH Super SKUs
			case INTEL_SERIES_C62x_PROD_SKUS_LPC_DIDS:	// C62x Series PCH Production SKUs
			case INTEL_SERIES_C62xA_PROD_SKUS_LPC_DIDS:	// C62xA Series PCH Production SKUs
			{
				unsigned p2sb_devfunc = find_P2SB();

				if (p2sb_devfunc != 0xFFFFFFFF)
				{
					uint8_t hptc = pci_read_cfg8(0, PCI_DEVNO(p2sb_devfunc), PCI_FUNCNO(p2sb_devfunc), 0x60);
					hpet_paddr = (paddr_t)(((hptc & 0x3) * 0x1000) + 0xFED00000);
					if ((hptc & (1u << 7)) == 0)
					{
						hptc |= (1u << 7);		// enable decode of the HPET address range
						pci_write_cfg8(0, PCI_DEVNO(p2sb_devfunc), PCI_FUNCNO(p2sb_devfunc), 0x60, hptc);
					}
					if (debug_flag > 1) {
						kprintf("htpc [0x%x] = 0x%x\n", hptc, ((hptc & 0x3) * 0x1000) + 0xFED00000);
					}
					found = 1;
				}
				break;
			}
			default:
			{
				const unsigned ich = ich_model(NULL);

				if (ich == 5)
				{
					paddr32_t gen_paddr = pci_read_cfg32(0, 31, 0, 0xD0);
					volatile uint32_t *gen_cntl = startup_memory_map(KILO(1), gen_paddr, PROT_READ | PROT_WRITE | PROT_NOCACHE);
					hpet_paddr = (paddr_t)((((*gen_cntl & 0x18000) >> 15) * 0x1000) + 0xFED00000);
					*gen_cntl |= (1U << 17);		// enable decode of the HPET address range
					startup_memory_unmap((void *)gen_cntl);
					found = 1;
				}
				else if (ich > 5)
				{
					// ICH 6 - 10
					paddr32_t gen_paddr = ((pci_read_cfg32(0, 31, 0, 0xF0) & ~0x1) + 0x3404);
					volatile uint32_t *hptc = startup_memory_map(KILO(1), gen_paddr, PROT_READ | PROT_WRITE | PROT_NOCACHE);
					hpet_paddr = (paddr_t)(((*hptc & 0x3) * 0x1000) + 0xFED00000);
					*hptc |= (1U << 7);		// enable decode of the HPET address range
					if (debug_flag > 1) {
						kprintf("htpc [%x] @ %v = %x\n", gen_paddr, hptc, *hptc);
					}
					startup_memory_unmap((void *)hptc);
					found = 1;
				}
				break;
			}
		}
	}
	return found;
}

/*
 * init_qtime_disable_hpet
 *
 * Used simply to disable the HPET, and clear out any pending interrupts, such that it doesn't interfere
 * with subsequent operation (refer to https://jira.bbqnx.net/browse/HW-24239). This should be removed
 * once a longer-term solution is in place.
 *
 * Returns: 0 on success, otherwise -1 on any error
 *
*/
int
init_qtime_disable_hpet() {
	int r = -1;

	if(!find_hpet_acpi() && !find_hpet_chipset()) {
		return -1;	// no HPET
	}
	hpet = (volatile uint64_t *)startup_memory_map(KILO(1), hpet_paddr, PROT_READ | PROT_WRITE | PROT_NOCACHE);
	if (debug_flag > 1) {
		kprintf("hpet @ %P, proceeding to disable (LAPIC timer will be used instead)\n", hpet_paddr);
	}

	/* clear GEN_CONF[ENABLE_CNF] to stop the timer(s) and disable all timer interrupts */
	HPET(0x10) &= ~(1U << 0);
	r = 0;	// success

	startup_memory_unmap((void *)hpet);
	return r;
}
