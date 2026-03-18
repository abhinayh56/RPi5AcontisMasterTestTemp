/*
 * $QNXLicenseC:
 * Copyright 2016, QNX Software Systems.
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

/**
 *
 * The family includes both the 4 bit (f)amily ID as well as the 8 bit extended
 * (F)amily ID. The model includes both the 4 bit (m)odel ID as well as the 4
 * bit extended (M)odel ID
 *
 * family = sum of 'family' and 'extended family'
 * model = sum of 'model' and 4-bit left shifted 'extended model'
 *
 * Documented in Intel 64 and IA-32 Architectures Developer's Manual: Vol. 2A
 *    MSb                                     LSb
 *      xxxx FFFF FFFF MMMM xxxx ffff mmmm xxxx
 *
 * @return It returns the display family and display model encoded into a 32 bit value
 *
 */
uint32_t cpu_family_model(void) {
	static uint32_t family_model;

	if (family_model == 0) {
		const uint32_t cpuid = x86_cpuid(1, 0);
		uint32_t family = (cpuid >> 8) & 0xF;
		uint32_t model = (cpuid >> 4) & 0xF;

		if ((family == 0xF) || (family == 0x6)) {
			model += ((cpuid >> 12) & 0xF0);
			if (family == 0xF) family += ((cpuid >> 20) & 0xFF);
		}
		family_model = (family << 8) | model;
	}

	return family_model;
}

/**
 * It returns display family encoded into the maximum 9 LSB bits
 * @return	CPU family in the maximum 9 LSB bits
 */
uint32_t cpu_family(void) {
	return ((cpu_family_model() >> 8) & 0x1ff);	/* 0x1FF is from the consideration of the max of (family + extended family) */
}

/**
 * It returns display model encoded into 8 LSB bits
 * @return	CPU model in 8 LSB bits
 */
uint32_t cpu_model(void) {
	return (cpu_family_model() & 0xff);
}
