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
 * We currently provide 2 default MTRR region layouts for 512M and 1G
 * respectively, this can be overridden by defining your own table in the BSP's
 * startup and passing it to poulsbo_config_wc(). See the
 * poulsbo_mtrr_regions_*.c files.
 */

/**
 * Returns 1 if the region passed in is the NULL region.
 * @note region must not be NULL
 */
static __inline__ int is_null_region(Poulsbo_MTRR_Region* region)
{
	return ((region->start == 0) && (region->size == 0) && (region->flags == 0));
}

/**
 * Sets the MTRR registers to the values of the table passed in the regions arg.
 * The regions table must be terminated with NULL_MTRR_REGION.
 */
void poulsbo_config_wc(Poulsbo_MTRR_Region* regions)
{
	unsigned regionIndex = 0;
	
	if (NULL != regions) {
		while (!is_null_region(&regions[regionIndex])) {
			if (regionIndex >= MAX_MTRR_VARIABLE_REGION_COUNT) {
				/* Detected too large or non NULL-terminated table */
				crash("Maximum number of MTRR regions (%d) exceeded, note that the table must be terminated with NULL_MTRR_REGION.\n", MAX_MTRR_VARIABLE_REGION_COUNT);
			}
			
			poulsbo_set_mtrr(regionIndex, regions[regionIndex].start, regions[regionIndex].size, regions[regionIndex].flags);
			regionIndex += 1;
		}
		
		if (regionIndex == 0) {
			kprintf("Warning: Empty regions table passed to poulsbo_config_wc(). If this is intentional, you shouldn't call poulsbo_config_wc().\n");
		}
	} else {
		/* Suspecious - warn the user */
		kprintf("Warning: NULL MTRR regions passed to poulsbo_config_wc(). If this is intentional, you shouldn't call poulsbo_config_wc().\n");
	}
}
