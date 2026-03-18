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

//
// Initialize asinfo structure in the system page.
// This code is hardware dependant and may have to be changed
// changed by end users.
//

void
init_asinfo(unsigned mem) {
	(void)as_add(0, MEG(16)-1, AS_ATTR_NONE, "isa", mem);

	as_add(MEG(96), GIG(4)-MEG(1)-1, AS_ATTR_DEV, "device", mem);
	as_add(GIG(4)-MEG(1), GIG(4)-1, AS_ATTR_ROM, "rom", mem);
}
