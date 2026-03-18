/*
 * $QNXLicenseC:
 * Copyright 2007, 2008, QNX Software Systems. 
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
#include "multiboot.h"

#define maplo(s,x) ((void *)(uintptr_t)(x))
#define unmaplo(p) 

//
// Initialize raminfo structure in the system page.
// This code is system dependant and may have to be
// changed by end users.
//
unsigned mem_scan;		// Global set via a command line option in main()

static void
my_add_ram(paddr64_t big_start, paddr64_t size) {
	paddr_t		end;
	paddr_t		start;

	start = big_start;
	end = start + size - 1;

	if(end >= start) {
		add_ram(start, end - start + 1);
	}

	// If we find memory above 64M then BIOS must be new so never scan
	if(mem_scan && (end > mem_scan)) {
		mem_scan = 0;
	}
}

//
// Memory located by QNX IPL.
//
static int 
ipl_ram() {
	int										found;
	const struct startup_info_mem_extended	*mem;
	paddr64_t								addr;
	paddr64_t								size;

	found = 0;
	mem = NULL;
	for (;;) {
		mem = find_startup_info(mem, STARTUP_INFO_MEM);
		if(mem == NULL) break;
		addr = mem->mem.addr;
		size = mem->mem.size;
		if(mem->mem.hdr.size >= sizeof(struct startup_info_mem_extended)) {
			addr |= (paddr64_t)mem->addr_hi << 32;
			size |= (paddr64_t)mem->size_hi << 32;
		}
		my_add_ram(addr, size);
		found = 1;
	} 

	return found;
} 

//
// Memory located by multiboot loader
//
static int
multiboot_ram() {
	struct mb_info	*info;
	int              r = 0;
	if ((unsigned)boot_regs->rax == MB_SIGNATURE) {
		info = maplo(sizeof *info, (unsigned)boot_regs->rbx);

		if(info->flags & MB_INFO_FLAG_MMAP_VALID) {
			struct mb_info_mmap *mm_info;
			unsigned len;

			len = info->mmap_length;
			if(len > 0) {
				mm_info = maplo(len, info->mmap_addr);
				do {
					unsigned		size;

					if(mm_info->type == MB_INFO_MMAP_TYPE_RAM) {
						my_add_ram(
							mm_info->base_addr_low | (paddr64_t)mm_info->base_addr_high << 32,
							mm_info->length_low | (paddr64_t)mm_info->length_high << 32
						);
					}
					size = mm_info->size + 4;
					mm_info = (void *)((uintptr_t)mm_info + size);
					len -= size;
				} while(len != 0);
				unmaplo(mm_info);
				r = 1;
			}
		}

		if((r == 0) && (info->flags & MB_INFO_FLAG_MEM_VALID)) {
			my_add_ram(0, info->mem_lower * 1024);
			my_add_ram(MEG(1), info->mem_upper * 1024);
			r = 1;
		}

        avoid_ram((uintptr_t)info, sizeof(*info));
        if ((info->flags & MB_INFO_FLAG_DRIVES_VALID) && (info->drives_length > 0)) {
            avoid_ram((uintptr_t)info->drives_addr, info->drives_length);
        }

		unmaplo(info);
	}
	return r;
}

void
init_raminfo() {
	// Note that add_ram() removes overlapping memory additions.
	if(ipl_ram() || multiboot_ram()) {
		if(mem_scan) {
			x86_scanmem(MEG(16), MEG(256)-1);
		}
	} else {
		x86_scanmem(0, MEG(16)-1);
	}
}
