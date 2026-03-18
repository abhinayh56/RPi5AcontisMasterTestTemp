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
#include <x86/imps.h>

//
// Intel MultiProcessor Specification Routines
//

#define MPFPS_SIGNATURE	('_' | ('M'<<8) | ((long)'P'<<16) | ((long)'_'<<24))

#define MPFPS_ADDR1	((PADDR_T) (640L*1024-1024))
#define MPFPS_ADDR2	((PADDR_T) 16L* *(unsigned short *)0x40e)
#define MPFPS_ADDR3	((PADDR_T) 0xf0000)


static const uint8_t entry_size[] = {
	sizeof(struct smp_processor),
	sizeof(struct smp_bus),
	sizeof(struct smp_ioapic),
	sizeof(struct smp_intsrc),
	sizeof(struct smp_lintsrc),
};


//
// Search for the "MP Floating Pointer Structure". It returns the offset
// of the structure. The structure is always on a 16 byte boundry and
// is 16 bytes long. It is located in the first megabyte of memory.
//
// Both start and length will be 16 byte aligned.
//
static struct smp_fps *
smp_scan_fps(PADDR_T beg, PADDR_T end) {
	static struct smp_fps buf;

	if(debug_flag > 4) {
		kprintf("Scan for MP from %P to %P\n", (paddr_t)beg, (paddr_t)end);
	}

	while(beg < end) {
		memmove(&buf, (void *)beg, sizeof(buf));
		if(buf.signature == MPFPS_SIGNATURE  &&
		  (buf.length == 1)  &&
		  (buf.spec_rev == 1  ||  buf.spec_rev == 4)  &&
		  (calc_cksum(&buf, 16) == 0)) {
			if(debug_flag > 4) {
				kprintf("Found MP Floating Structure Pointer at %P\n", (paddr_t)beg);
			}
			return &buf;
		}
		beg += 16;
	}

	return NULL;
}


//
// Look in the 3 places the spec says the pointer structure can be.
// Note: We assume 640K of base memory. It is possible a system may
//       be compliant with only 512K. However, it is unlikely.
//
struct smp_fps *
smp_get_fps(void) {
	struct smp_fps	*addr;

	addr = smp_scan_fps(MPFPS_ADDR1, MPFPS_ADDR1+1024);
	if(addr != NULL) return addr;
	addr = smp_scan_fps(MPFPS_ADDR2, MPFPS_ADDR2+1024);
	if(addr != NULL) return addr;
	addr = smp_scan_fps(MPFPS_ADDR3, MPFPS_ADDR3+0x10000);

	return addr;
}

ssize_t imps_entry_size(unsigned type)
{
	return (type >= NUM_ELTS(entry_size)) ? -1 : entry_size[type];
}


/*
 * As noted in Intel's Multiprocessor Specification v1.4 (pg 4-4)
 * 
 * MP FEATURE INFORMATION BYTE 2
 * Bits 0-6: Reserved for future MP definitions.
 * Bit 7: IMCRP.
 * When the IMCR presence bit is set, the IMCR is present and PIC Mode is
 * implemented; otherwise, Virtual Wire Mode is implemented.
 * 
 * Returns: true (!0) or false (0) depending on whether the IMCR register is
 * 			present or not
*/

unsigned is_imcr_present(void)
{
	struct smp_fps *fps = smp_get_fps();
	
	if (fps == NULL) return 0;	// false
	return ((fps->features[1] & 0x8) == 0x8);	// bytes are numbered 1 - 5
}
