/*
 * $QNXLicenseC:
 * Copyright 2020, 2022, 2023, 2025 BlackBerry Limited.
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

#include <x86/pci_misc.h>
#include <x86/intel_misc.h>


/*
 * This file contains functions for determining and configuring the polarity
 * and trigger mode for IOAPIC interrupt inputs. These are NOT API's to read or
 * write the redirection table entry settings for interrupt disposition but
 * rather the settings for how the interrupt sources appear at the IOAPIC inputs
 *
 * These settings can also be statically configured by using a auto-configure
 * profile however this will not accommodate runtime changes to the settings and
 * if any are made, the ioapic_intsrc_cfg_check() is probably going to fail.
 *
 * During IOAPIC initialization (see init_intrinfo_ioapic in apic_io_intr.c),
 * the ioapic_*_get() functions will be used to check any sources which have not
 * been specified in the auto-config profiles (normally only the first 16 legacy
 * interrupts are specified) to ensure that the interrupt polarity and trigger
 * mode are set consistent with the chipset settings associated with those
 * interrupt sources
 *
 * Note that some chipsets do not implement controls for the interrupts
 * presented to the IOAPIC and in these cases the API will indicate this in
 * the value returned and default settings will be used (see apic_io_intr.c)
 *
 * If required, platform specific behaviour can be obtained by localizing this
 * file in the board specific directory and making appropriate edits
 *
 * If only a few polarity and/or trigger mode adjustments are needed to specific
 * entries beyond the legacy 16, these can be made just prior to calling
 * init_intrinfo_ioapic() in the board specific init_intrinfo.c
*/


/*
 ===============================================================================
 ioapic_intsrc_polarity_get()

 returns -1 for active low, +1 for active high, 0 for undetermined, unsupported
 or any other error

*/
int ioapic_intsrc_polarity_get(const unsigned ioapic_id, const unsigned ioapic_entry_num)
{
	int polarity = 0;	/* assume error */

	if (is_intel_chipset())
	{
		unsigned devid = get_LPC_devid();
		switch(devid)
		{
			case INTEL_APOLLO_LAKE_LPC_DIDS:
			case INTEL_DENVERTON_LPC_DIDS:
			case INTEL_ANDERSON_LAKE_LPC_DIDS:
			case INTEL_SKYLAKE_SKU_UY_LPC_DIDS:
			case INTEL_COFFEELAKE_LPC_DIDS:
			case INTEL_TIGERLAKE_LPC_DIDS:
			case INTEL_ELKHARTLAKE_LPC_DIDS:
			case INTEL_RAPTORLAKE_LPC_DIDS:
			case INTEL_ALDERLAKE_LPC_DIDS:
			case INTEL_AMSTONLAKE_LPC_DIDS:
			case INTEL_SKYLAKE_LPC_DIDS:
			case INTEL_SERIES_3xx_2xx_PCH_LPC_DIDS:
			case INTEL_SERIES_C62x_SUPER_SKUS_LPC_DIDS:
			case INTEL_SERIES_C62xA_SUPER_SKUS_LPC_DIDS:
			case INTEL_SERIES_C62x_PROD_SKUS_LPC_DIDS:
			case INTEL_SERIES_C62xA_PROD_SKUS_LPC_DIDS:
				polarity = itss_polarity_get(ioapic_id, ioapic_entry_num);
				break;

			default:
				break;
		}
	}
	if (debug_flag > 2)
	{
		kprintf("%s(%u, %u) returns %s\n", __FUNCTION__,
					ioapic_id, ioapic_entry_num,
					(polarity == -1) ? "LO" : ((polarity == 1) ? "HI" : "unknown/error"));
	}
	return polarity;
}

/*
 ===============================================================================
 ioapic_intsrc_polarity_set()

 returns the polarity set (-1 active low, +1 active high) or 0 for unsupported
 or any other error

*/
int ioapic_intsrc_polarity_set(const unsigned ioapic_id, const unsigned ioapic_entry_num, const int polarity)
{
	int new_polarity = 0;	/* assume error */

	if ((polarity == -1) || (polarity == 1))
	{
		if (is_intel_chipset())
		{
			unsigned devid = get_LPC_devid();
			switch(devid)
			{
				case INTEL_APOLLO_LAKE_LPC_DIDS:
				case INTEL_DENVERTON_LPC_DIDS:
				case INTEL_ANDERSON_LAKE_LPC_DIDS:
				case INTEL_SKYLAKE_SKU_UY_LPC_DIDS:
				case INTEL_COFFEELAKE_LPC_DIDS:
				case INTEL_TIGERLAKE_LPC_DIDS:
				case INTEL_ELKHARTLAKE_LPC_DIDS:
				case INTEL_RAPTORLAKE_LPC_DIDS:
				case INTEL_ALDERLAKE_LPC_DIDS:
				case INTEL_AMSTONLAKE_LPC_DIDS:
				case INTEL_SKYLAKE_LPC_DIDS:
				case INTEL_SERIES_3xx_2xx_PCH_LPC_DIDS:
				case INTEL_SERIES_C62x_SUPER_SKUS_LPC_DIDS:
				case INTEL_SERIES_C62xA_SUPER_SKUS_LPC_DIDS:
				case INTEL_SERIES_C62x_PROD_SKUS_LPC_DIDS:
				case INTEL_SERIES_C62xA_PROD_SKUS_LPC_DIDS:
					new_polarity = itss_polarity_set(ioapic_id, ioapic_entry_num, polarity);
					break;

				default:
					break;
			}
		}
	}
	if (debug_flag > 2)
	{
		kprintf("%s(%u, %u, %d) returns %s\n", __FUNCTION__,
					ioapic_id, ioapic_entry_num, polarity,
					(new_polarity == -1) ? "LO" : ((new_polarity == 1) ? "HI" : "unknown/error"));
	}
	return new_polarity;
}

/*
 ===============================================================================
 ioapic_intsrc_trigger_mode_get()

 returns 0 for level, 1 for edge and -1 for undetermined, unsupported or any
 other error

*/
int ioapic_intsrc_trigger_mode_get(const unsigned ioapic_id, const unsigned ioapic_entry_num)
{
	int trigger_mode = -1;	/* assume error */

	if (is_intel_chipset())
	{
		unsigned devid = get_LPC_devid();
		switch(devid)
		{
			case INTEL_APOLLO_LAKE_LPC_DIDS:
			case INTEL_DENVERTON_LPC_DIDS:
			case INTEL_ANDERSON_LAKE_LPC_DIDS:
			case INTEL_SKYLAKE_SKU_UY_LPC_DIDS:
			case INTEL_COFFEELAKE_LPC_DIDS:
			case INTEL_TIGERLAKE_LPC_DIDS:
			case INTEL_ELKHARTLAKE_LPC_DIDS:
			case INTEL_RAPTORLAKE_LPC_DIDS:
			case INTEL_ALDERLAKE_LPC_DIDS:
			case INTEL_AMSTONLAKE_LPC_DIDS:
			case INTEL_SKYLAKE_LPC_DIDS:
			case INTEL_SERIES_3xx_2xx_PCH_LPC_DIDS:
			case INTEL_SERIES_C62x_SUPER_SKUS_LPC_DIDS:
			case INTEL_SERIES_C62xA_SUPER_SKUS_LPC_DIDS:
			case INTEL_SERIES_C62x_PROD_SKUS_LPC_DIDS:
			case INTEL_SERIES_C62xA_PROD_SKUS_LPC_DIDS:
				trigger_mode = itss_trigger_mode_get(ioapic_id, ioapic_entry_num);
				break;

			default:
				break;
		}
	}
	if (debug_flag > 2)
	{
		kprintf("%s(%u, %u) returns %s\n", __FUNCTION__,
				ioapic_id, ioapic_entry_num,
				(trigger_mode == 0) ? "LEVEL" : ((trigger_mode == 1) ? "EDGE" : "unknown/error"));
	}
	return trigger_mode;
}

/*
 ===============================================================================
 ioapic_intsrc_trigger_mode_set()

 returns the trigger mode set (0 for level, 1 for edge) or -1 for unsupported
 or any other error

*/
int ioapic_intsrc_trigger_mode_set(const unsigned ioapic_id, const unsigned ioapic_entry_num, const unsigned trigger_mode)
{
	int new_trigger_mode = -1;

	if ((trigger_mode == 0) || (trigger_mode == 1))
	{
		if (is_intel_chipset())
		{
			unsigned devid = get_LPC_devid();
			switch(devid)
			{
				case INTEL_APOLLO_LAKE_LPC_DIDS:
				case INTEL_DENVERTON_LPC_DIDS:
				case INTEL_ANDERSON_LAKE_LPC_DIDS:
				case INTEL_SKYLAKE_SKU_UY_LPC_DIDS:
				case INTEL_COFFEELAKE_LPC_DIDS:
				case INTEL_TIGERLAKE_LPC_DIDS:
				case INTEL_ELKHARTLAKE_LPC_DIDS:
				case INTEL_RAPTORLAKE_LPC_DIDS:
				case INTEL_ALDERLAKE_LPC_DIDS:
				case INTEL_AMSTONLAKE_LPC_DIDS:
				case INTEL_SKYLAKE_LPC_DIDS:
				case INTEL_SERIES_3xx_2xx_PCH_LPC_DIDS:
				case INTEL_SERIES_C62x_SUPER_SKUS_LPC_DIDS:
				case INTEL_SERIES_C62xA_SUPER_SKUS_LPC_DIDS:
				case INTEL_SERIES_C62x_PROD_SKUS_LPC_DIDS:
				case INTEL_SERIES_C62xA_PROD_SKUS_LPC_DIDS:
					new_trigger_mode = itss_trigger_mode_set(ioapic_id, ioapic_entry_num, trigger_mode);
					break;

				default:
					break;
			}
		}
	}
	if (debug_flag > 2)
	{
		kprintf("%s(%u, %u, %d) returns %s\n", __FUNCTION__,
					ioapic_id, ioapic_entry_num, trigger_mode,
					(new_trigger_mode == 0) ? "LEVEL" : ((new_trigger_mode == 1) ? "EDGE" : "unknown/error"));
	}
	return new_trigger_mode;
}

/*
 ===============================================================================
 * ioapic_intsrc_cfg_check
 *
 * returns 0 (ok) or -1 (fail) based on whether the IO-APIC redirection table
 * entry interrupt source configuration settings are consistent with the chipset
 * interrupt source configuration (if they exist)
 *
*/
int ioapic_intsrc_cfg_check(const unsigned ioapic_id)
{
	int r = 0;	/* we'll assume ok unless we determine otherwise */
	const ioapic_accessfnc_t * const access_fn = ioapic_get_accessfnc(ioapic_id);

	if (access_fn == NULL) kprintf("%s(%u), invalid ID, no access functions\n", __FUNCTION__, ioapic_id);
	else
	{
		const unsigned num_entries = access_fn->num_entries(ioapic_id);

		if (debug_flag > 1)
		{
			kprintf("%s(%u), checking polarity settings for %u entries\n",
					__FUNCTION__, ioapic_id, num_entries);
		}
		unsigned i;
		for (i=0; i<num_entries; i++)
		{
			/* in the IOAPIC, 1 means active LO polarity, 0 means HI */
			const int ioapic_polarity = (((access_fn->read_entry(ioapic_id, i) >> 13) & 1) == 1) ? -1 : 1;
			const int chipset_polarity = ioapic_intsrc_polarity_get(ioapic_id, i);

			/* check them all anyway */
			if (chipset_polarity == 0) continue;
			/* log which ones are inconsistent and ensure a failed return code */
			else if (ioapic_polarity != chipset_polarity)
			{
				r = -1;
				if (debug_flag > 1)
				{
					static const char * const polarity_str[] = {[0] = "LO", [1] = "?", [2] = "HI"};
					kprintf("IOAPIC %u: entry %u, configured polarity is active %s, chipset is active %s\n",
							ioapic_id, i, polarity_str[ioapic_polarity + 1], polarity_str[chipset_polarity + 1]);
				}
			}
		}
		if (debug_flag > 1)
		{
			kprintf("%s(%u), checking trigger mode settings for %u entries\n",
					__FUNCTION__, ioapic_id, num_entries);
		}
		for (i=0; i<num_entries; i++)
		{
			const int ioapic_trigger_mode = (((access_fn->read_entry(ioapic_id, i) >> 15) & 1) == 1) ? 0 : 1;
			const int chipset_trigger_mode = ioapic_intsrc_trigger_mode_get(ioapic_id, i);

			/* check them all anyway */
			if (chipset_trigger_mode == -1) continue;
			/* log which ones are inconsistent and ensure a failed return code */
			else if (ioapic_trigger_mode != chipset_trigger_mode)
			{
				r = -1;
				if (debug_flag > 1)
				{
					static const char * const tmode_str[] = {[0] = "Level", [1] = "Edge"};
					kprintf("IOAPIC %u: entry %u, configured trigger mode is %s, chipset is %s\n",
							ioapic_id, i, tmode_str[ioapic_trigger_mode], tmode_str[chipset_trigger_mode]);
				}
			}
		}
	}
	return r;
}
