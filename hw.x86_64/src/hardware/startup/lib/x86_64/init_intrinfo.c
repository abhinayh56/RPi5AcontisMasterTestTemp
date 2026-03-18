/*
 * $QNXLicenseC:
 * Copyright 2016, 2020, 2022, 2023, 2025 BlackBerry Limited.
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
#include <x86/intr.h>
#include <ich_chipset.h>
#include <sch_chipset.h>
#include <piix4_chipset.h>
#include <sbx00_chipset.h>
#include <x86/intel_misc.h>
#include "amd_misc.h"
#include "intel_pxh.h"

extern int init_intrinfo_msi(int vector_base);
static void disable_8259(void);

/*
 * init_intrinfo
 *
 * Default x86 interrupt configuration. You can do your own APIC configuration
 * by implementing a board specific version of this file.
 *
*/
void init_intrinfo(void)
{
	extern ioapic_accessfnc_t *ioapic_accessfncs_generic;
	extern ioapic_accessfnc_t *ioapic_accessfncs_ich;
	extern ioapic_accessfnc_t *ioapic_accessfncs_ioh;
	extern ioapic_accessfnc_t *ioapic_accessfncs_amd;
	unsigned ioapic_id;
	unsigned num_ioapics = ioapic_find();
	const ioapic_accessfnc_t *accessFn = NULL;
	unsigned autofill_profile_num = IOAPIC_PROFILE_DEFAULT;
	int vector_base = (acpi_ignore == 1) ? _NTO_INTR_CLASS_EXTERNAL : -1;

	disable_8259();

	/* set up Local APIC */
	init_intrinfo_lapic();

	/*
	 * the following logic check is for specific chipsets and most of it can be
	 * removed for boards in which the chipset is known. This will save a lot of
	 * unnecessary code from being brought in
	 */
	if (is_intel_chipset())
	{
		/* undo any PIRQ legacy re-mapping to enable the use of corresponding IOAPIC INTPINs */
		unsigned devid = get_LPC_devid();

		if (debug_flag) kprintf("Intel IOAPIC config\n");

		switch(devid)
		{
			case INTEL_LEGACY_PIIX4_LPC_DIDS:	// legacy PIIX4 chipset (includes current VMWare)
			{
				piix4_pirq_remap_adjust();
				/* we use the generic access functions to avoid accesses to
				 * ICH registers that don't exist in the PIIX4 */
				accessFn = ioapic_accessfncs_generic;
				break;
			}
			case INTEL_BAYTRAIL_LPC_DIDS:	// Bay Trail (the PCU contains the iLB which contains the LPC)
			case INTEL_RANGELEY_LPC_DIDS:	// Rangeley-based systems
				accessFn = ioapic_accessfncs_ioh;
				pcu_ilb_pirq_remap_adjust();
				break;
			case INTEL_APOLLO_LAKE_LPC_DIDS:	// Apollo Lake/Broxton
				accessFn = ioapic_accessfncs_generic;
				autofill_profile_num = IOAPIC_PROFILE_APL;
				/* no ACPI tables so specify the vector base */
				vector_base = _NTO_INTR_CLASS_EXTERNAL;
				itss_pirq_remap_adjust();
				break;
			case INTEL_SKYLAKE_SKU_UY_LPC_DIDS:	// Skylake U and Y SKU's
			case INTEL_SKYLAKE_LPC_DIDS:		// Skylake full featured (A, B, H, Q, Z SKU's)
			case INTEL_DENVERTON_LPC_DIDS:		// Denverton
			case INTEL_ANDERSON_LAKE_LPC_DIDS:	// Ash Creek Falls (ACF)
			case INTEL_COFFEELAKE_LPC_DIDS:		// Coffeelake
			case INTEL_TIGERLAKE_LPC_DIDS:		// Tigerlake
			case INTEL_ELKHARTLAKE_LPC_DIDS:	// Elkhartlake
			case INTEL_RAPTORLAKE_LPC_DIDS:		// Raptorlake
			case INTEL_ALDERLAKE_LPC_DIDS:		// Alderlake
			case INTEL_AMSTONLAKE_LPC_DIDS:		// Amstonlake
			case INTEL_SERIES_C62x_SUPER_SKUS_LPC_DIDS:	// C62x Series PCH Super SKUs
			case INTEL_SERIES_C62xA_SUPER_SKUS_LPC_DIDS:	// C62xA Series PCH Super SKUs
			case INTEL_SERIES_C62x_PROD_SKUS_LPC_DIDS:	// C62x Series PCH Production SKUs
			case INTEL_SERIES_C62xA_PROD_SKUS_LPC_DIDS:	// C62xA Series PCH Production SKUs
				accessFn = ioapic_accessfncs_generic;
				itss_pirq_remap_adjust();
				break;
			case INTEL_POULSBO_SCH_LPC_DIDS:	// SCH (poulsbo)
			case INTEL_TUNNEL_CREEK_LPC_DIDS:	// IOH (Tunnel Creek)
				accessFn = ioapic_accessfncs_ioh;
				sch_pirq_remap_adjust();
				break;
			case INTEL_TOLAPAI_LPC_DIDS:
				accessFn = ioapic_accessfncs_ioh;
				ich_pirq_remap_adjust();
				break;
			default:		// ICH variant
			{
				accessFn = ioapic_accessfncs_ich;	// assume ICH IO-APIC's be default

				/* if it doesn't look like an ICH, we're done */
				if (ich_model(NULL) == 0) {
					kprintf("Unknown Intel chipset id: %x\n", devid);
					crash("startup aborted");
				}
				else if (num_ioapics > 1)
				{
					/*
					 * some boards with more than 1 IO-APIC don't necessarily
					 * have more than 1 ICH. Some server boards utilize Intel's
					 * PXH which depending on the model, may contain 1 or 2
					 * PCIe-PCI bridges and 1 or 2 IO-APIC's (1 per bridge). The
					 * 'ioapic_accessfncs_ich' cannot be used for the PXH IO-APIC's
					 * because the addressing is not restricted as it is in some
					 * ICH's. For those IO-APIC's we use 'ioapic_accessfncs_generic'.
					*/
					ioapic_id = ioapic_get_id_first();
					while (ioapic_id != -1u)
					{
						accessFn = find_pxh_ioapic(ioapic_id) ? ioapic_accessfncs_generic : ioapic_accessfncs_ich;
						if (debug_flag) kprintf("IO-APIC ID%u on %s\n", ioapic_id, (accessFn == ioapic_accessfncs_generic) ? "PXH" : "ICH");
						ioapic_set_accessfnc(ioapic_id, accessFn);
						ioapic_id = ioapic_get_id_next(ioapic_id);
					}
					accessFn = NULL;	// make sure our settings are not overridden
				}
				ich_pirq_remap_adjust();
				break;
			}
		}
	}
	else if (is_amd_chipset())
	{
		const unsigned devid = get_LPC_devid();
		const unsigned is_ryzen = is_amd_ryzen();

		accessFn = ioapic_accessfncs_amd;

		if (debug_flag) kprintf("%s chipset %x\n", is_ryzen ? "AMD Ryzen" : "AMD", devid);

		if (!is_ryzen)
		{
			switch(devid)
			{
				case AMD_SB600_SB700_ACPI_DIDS:	// sb600/sb700 SMbus module and ACPI block DevId
					sbx00_pirq_remap_adjust(devid);
					break;
				default:
					break;
			}
		}
	}
	else
	{
		/* non Intel (Via, Nvidia, ??) */
		accessFn = ioapic_accessfncs_generic;
		if (debug_flag) kprintf("Unknown vendor chipset, Generic IOAPIC config\n");
	}

	if (debug_flag) kprintf("Found %u IOAPIC's ...\n", num_ioapics);

	ioapic_id = ioapic_get_id_first();
	while (num_ioapics-- > 0)
	{
		/*
		 * we use a standard IOAPIC configuration by default however we do use
		 * the access functions setup above. Custom configurations can be made
		 * by providing a board specific implementation of the init_intrinfo_ioapic()
		 * function.
		 * -1 for the <vector_base> arg lets the base vector be discovered from the ACPI tables.
		*/
		if (accessFn != NULL) ioapic_set_accessfnc(ioapic_id, accessFn);
		ioapic_set_autofill_profile_num(ioapic_id, autofill_profile_num);
		init_intrinfo_ioapic(vector_base, ioapic_id);

		/*
		 * On some platforms, there are polarity and/or trigger mode override
		 * configuration registers for the IOAPIC inputs so make sure that they
		 * are consistent with the current IOAPIC configuration. This function
		 * also works for the case where these registers don't exist so should
		 * always call it
		 */
		const int r = ioapic_intsrc_cfg_check(ioapic_id);
		if (r != 0) kprintf("** WARNING: IOAPIC interrupt config check failed\n");
		/*
		 * By default, we do not ASSERT a successful ioapic_intsrc_cfg_check().
		 * This is to allow the system to boot to a usable state and allow developers
		 * to poke around should the above message be displayed. This is generally
		 * safe because the interrupts are masked until such a time as device drivers
		 * are started. However it is recommended that once the interrupt configuration
		 * has been established and confirmed correct that the ASSERT be added back in
		 * in order to hard stop should something change as incorrect settings can lead
		 * to a kernel crash with an "Out of interrupt events" message
		 ASSERT(r == 0);
		 */

		ioapic_id = ioapic_get_id_next(ioapic_id);
	}

	/* use the remaining vectors for MSI */
	init_intrinfo_msi(-1);
}

/* put the 8259's in a known state with all interrupts masked */
static void disable_8259(void)
{
	/* configure the 8259 to only allow for IRQ0 (8254 timer) */
	out8(0x20, 0x11); io_safe();		// ICW1 - cascaded 8259, edge triggered, ICW4 required
	out8(0x21, APIC_SPURIOUS_INTERRUPT); io_safe();		// ICW2 - vector address
	/* not sure why but ICW3 seems to be required even though docs say its not when single 8259 set in ICW1 */
	out8(0x21, 0x0); io_safe();			// ICW3 - not required (single 8259)
	out8(0x21, 0x01); io_safe();		// ICW4 - normal nested, non-buffered, normal EOI, 8086 mode

	out8(0x21, 0xff); io_safe();		// OCW1 - mask everything (including slave 8259)
	out8(0xA1, 0xff); io_safe();		// OCW1 - paranoid mask everything in slave 8259

	// Make sure both NMI sources are masked
	out8(0x61,(in8(0x61) & 0x3) | 0x0c);
}
