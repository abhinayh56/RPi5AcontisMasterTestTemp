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

uint8_t		acpi_ignore;

int
cpu_handle_common_option(int opt) {
	switch(opt) {
	case 'B':
		acpi_ignore = 1;
		return 1;
	case 'E': ; // erratas
		int active = 1;
		const char *name = optarg;
		if(*name == '~') {
			active = 0;
			++name;
		}
		if(strcmp("meltdown", name) == 0) {
			meltdown_active = active;
		}
		return 1;
#if !defined(__X86_64__)
	case 'x':
		x86_cputype(); // get cpu_features set.
		if(cpu_features & X86_FEATURE_PAE) {
			unsigned eax;
			x86_cpuid1(0x80000000, &eax, NULL, NULL, NULL);
			if(eax >= 0x80000008) {
				x86_cpuid1(0x80000008, &eax, NULL, NULL, NULL);
				paddr_bits = eax & 0xff;
			} else {
				paddr_bits = 36;
			}
		}
		return 1;
#endif
	case 'z':
        // Deprecated:  it used to select a different source
        // for the system timer
		return 1;
	case 'Q':
		if (strcmp(optarg, "enable") == 0) {
			hypervisor_set_options(HYP_FLAG_ENABLED);
		} else if (strcmp(optarg, "disable") == 0) {
			hypervisor_set_options(HYP_FLAG_DISABLED);
		} else {
			hypervisor_set_options(HYP_FLAG_INVALID);
		}
		break;
	}
	return 0;
}
