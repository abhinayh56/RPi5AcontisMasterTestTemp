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

#define peek32(a)	(*(uint32_t *)(a))
#define poke32(a,v)	(*(uint32_t *)(a) = (v))

void
x86_scanmem(PADDR_T beg, PADDR_T end) {
	PADDR_T		addr;
	unsigned long	n, nbytes;

	// We assume the RAM in the image is in is OK so we add it and step over.
	if(full_ram_paddr > beg  &&  shdr->ram_size < end) {
		nbytes = ROUNDPG(shdr->ram_size);
		beg = full_ram_paddr + nbytes;
		add_ram(0, beg);
	}

	// Scan memory putting in markers at 4K page boundries.
	for(addr = beg, n = 0 ; addr < end ; addr += 4096, ++n) {
		poke32(addr, ~0L);
		if(peek32(addr) != ~0L) continue;
		poke32(addr, 0L);
		if(peek32(addr) != 0L) continue;
		poke32(addr, 0xfecaefbeL);	// "beefcafe" in hex
		poke32(addr + 4, n);
	}

	// Re-scan looking for markers of memory which can be added.
	for(addr = beg, n = 0 ; addr < end ;) {
		PADDR_T		addr1;
		unsigned long	n1;

		if(peek32(addr) == 0xfecaefbe  &&  peek32(addr + 4) == n) {
			poke32(addr, 0L); poke32(addr + 4, 0L);
			for(addr1 = addr + 4096, n1 = n + 1; addr1 < end ; addr1 += 4096, ++n1) {
				if(peek32(addr1) != 0xfecaefbe  ||  peek32(addr1 + 4) != n1) {
					break;
				}
				poke32(addr1, 0L); poke32(addr1 + 4, 0L);
			}

			//
			// Because it is so common we check for VGA video memory and skip it.
			// Note that if memory is contiguious before b0000 we will find it.
			//
			if(!(addr >= 0xb0000L  &&  addr <= 0xbffff)) {
				nbytes = addr1 - addr;

				add_ram(addr, nbytes);
			}

			addr = addr1;
			n = n1;
			continue;
		}

		addr += 4096;	// Next page
		++n;
	}
}
