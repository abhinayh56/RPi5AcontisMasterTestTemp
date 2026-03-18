/*
 * $QNXLicenseC:
 * Copyright 2009, QNX Software Systems. 
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


/*
 * ioapic_find
 * 
 * return the number of IOAPIC's available as per the ACPI and/or MP Configuration
 * tables.
 * 
 * Note that it is not necessary that the tables be used to add IOAPIC's but if
 * this function is called, they will be checked. If you don't want to use the
 * tables and they do exist, don't call this function ... add IOAPIC's manually
 * by calling add_ioapic()
*/
unsigned ioapic_find(void)
{
	static unsigned been_called = 0;	// avoid rescans if called multiple times

	if (!been_called)
	{
		unsigned lcl_num_ioapics = ioapic_discover_acpi();
		if (lcl_num_ioapics == 0)
		{
			lcl_num_ioapics = ioapic_discover_imps();
			if (debug_flag) {
				if (lcl_num_ioapics > 0) kprintf("Added %d %s from IMPS tables\n", lcl_num_ioapics, lcl_num_ioapics > 1 ? "IOAPIC's" : "IOAPIC");
				else kprintf("NO IOAPIC's\n");
			}
		} else if (debug_flag) {
			kprintf("Added %d %s from ACPI tables\n", lcl_num_ioapics, lcl_num_ioapics > 1 ? "IOAPIC's" : "IOAPIC");
		}
		been_called = 1;
	}
	
	/*
	 * we return the ioapic_get_num() as determined by add_ioapic() which will
	 * account for all calls including those from ioapic_discover_acpi/imps()
	 * which may have rewritten or been disallowed because of specific add_ioapic()
	 * calls from a board specific startup. Bottom line, ioapic_get_num() always
	 * represents the actual number of IOAPIC's
	*/
	return ioapic_get_num();
}
