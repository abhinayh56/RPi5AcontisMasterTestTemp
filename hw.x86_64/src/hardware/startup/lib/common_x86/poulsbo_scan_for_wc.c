/*
 * $QNXLicenseC:
 * Copyright 2008, QNX Software Systems. 
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
#include "poulsbo_chipset.h"

/**
 * Helper function to poulsbo_scan_for_wc() for adding a graphics syspage entry.
 */
static void hwinfo_add_graphics(paddr32_t wc_base, uint32_t wc_size)
{
	hwi_add_device(HWI_ITEM_BUS_UNKNOWN, HWI_ITEM_DEVCLASS_MISC, "graphics", 0);
	if ((wc_base != 0) && (wc_size != 0)) {
		alloc_ram(wc_base, wc_size, 1);
		hwi_add_location(wc_base, wc_size, 0, hwi_find_as(wc_base, 0));
	}
}

/**
 * Look for a region marked as write-combining. It is assumed such a region is
 * reserved for the graphics driver, so stuff a syspage entry indicating so.
 */
void poulsbo_scan_for_wc(void)
{
	paddr32_t mtrr_base;
	uint32_t mtrr_size;
	uint32_t mtrr_type;
	uint32_t wc_base = 0;
	uint32_t wc_size = 0;
	int i;

	/* 
	 * Scan for any MTRR set as write-combining; if it's found, it's for
	 * graphics, so stuff a syspage entry with the base address and size.
	 */
	for(i = 0; i < MAX_MTRR_VARIABLE_REGION_COUNT; i++) {
		poulsbo_get_mtrr(i, &mtrr_base, &mtrr_size, &mtrr_type);
		if(mtrr_type == X86_64_MTRR_TYPE_WRITECOMBINING) {
			wc_base = mtrr_base;
			wc_size = mtrr_size;
			break; /* We only configure 1 WC region, no need to continue */
		}
	}
	
	hwinfo_add_graphics(wc_base, wc_size);
}
