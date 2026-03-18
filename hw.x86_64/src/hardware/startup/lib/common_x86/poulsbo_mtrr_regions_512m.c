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
 * To properly configure the MTRRs, each MTRR region must be naturally aligned. So, 
 * we'll configure the MTRRs as follows for 512M of DRAM
 * -region 0 = 256M, writeback
 * -region 1 = 128M, writeback
 * -region 2 = 64M, writeback
 * -region 3 = 64M, write-combining
 */
Poulsbo_MTRR_Region poulsbo_mtrr_regions_512m[] = {
	{
		MEG(0),   /* start */
		MEG(256), /* size */
		X86_64_MTRR_TYPE_WRITEBACK
	},
	{
		MEG(256), /* start */
		MEG(128), /* size */
		X86_64_MTRR_TYPE_WRITEBACK
	},
	{
		MEG(384), /* start */
		MEG(64),  /* size */
		X86_64_MTRR_TYPE_WRITEBACK
	},
	{
		MEG(448), /* start */
		MEG(64),  /* size */
		X86_64_MTRR_TYPE_WRITECOMBINING
	},
	
	/* NULL terminate the table */
	NULL_MTRR_REGION
};
