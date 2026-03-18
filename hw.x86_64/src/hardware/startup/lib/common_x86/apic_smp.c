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
#include <x86/lapic.h>

#define write_code(d, s, l)		memmove((void *)(d), (void *)(s), l)

extern uint8_t				apstart16_start[];
extern uint8_t				apstart16_end[];

static uint8_t		cpuid_remap[PROCESSORS_MAX];

extern void   apstart_init(PADDR_T  addr, void (*f)(void));



#include <_pack1.h>

struct far_jmp {
	uint8_t			opcode;
	unsigned short	off;
	unsigned short	seg;
};


static void
smp_cmd(int id, unsigned cmd) {
	unsigned val;
	int n;

//kprintf("smp_cmd: lapic v/p = %v/%P id %x cmd %x\n", lapic_startup_vaddr, (paddr_t)lapic_paddr, id, cmd);
	lapic_write(LAPIC_ESR, 0);	// Clear any accumulated errors.

	// Set target APIC ID
	val = (((unsigned)id) << 24) | (lapic_read(LAPIC_ICRH) & 0x00ffffff);
	lapic_write(LAPIC_ICRH, val);

	// Send command
	val = cmd | (lapic_read(LAPIC_ICRL) & 0xfff33000);
	lapic_write(LAPIC_ICRL, val);

	for(n = 10000 ; n ; --n) {
		val = lapic_read(LAPIC_ICRL);
		if((val & 0x1000) == 0) {
			return;
		}
	}
	crash("smp_cmd(%x,%x) timedout\n", id, cmd);
}


static void
pause(int ms) {
	// Can't use timer hardware - CPU coming up reads/writes it as well
	// and gets confused. Maybe use the time stamp counter, if present.
	static volatile long cnt;

	while(ms--) {
		for(cnt = 10000 ; cnt ; --cnt) {
		}
	}
}


void
apic_add_cpuid(unsigned num_cpus, unsigned id) {
	cpuid_remap[num_cpus] = id;
}


// Perform board specific initialization of the smp_entry section of
// the system page.
void
apic_smp_init(struct smp_entry *smp, unsigned num_cpus) {
	// Nothing to do
}


// Get the 'cpu' processor executing at the 'start' address.
int
apic_smp_start(unsigned cpu, void (*start)(void)) {
	unsigned			size;
	PADDR_T				apstart;
	unsigned 			id;

	if(cpu == 1) {
		//
		// First time in
		//

		unsigned i;
		struct cpuinfo_entry *cep = lsp.cpuinfo.p;

		unsigned const bsp_apic_id	= lapic_id();
		if(cpuid_remap[0] != bsp_apic_id) {
			// We expect the BSP to be CPU 0, swap when that's not the
			// case
			for(i = 1; cpuid_remap[i] != bsp_apic_id; ++i) {
				if(i >= lsp.syspage.p->num_cpu) {
					crash("Could not find BSP APIC ID %x\n", bsp_apic_id);
				}
			}
			cpuid_remap[i] = cpuid_remap[0];
			cpuid_remap[0] = bsp_apic_id;
		}
		for(i = 0; i < lsp.syspage.p->num_cpu; ++i, ++cep) {
			cep->smp_hwcoreid = cpuid_remap[i];
			if (debug_flag > 0) {
				kprintf("%s(%d), CPU %u, cep->smp_hwcoreid = 0x%x\n", __FUNCTION__, cpu, i, cep->smp_hwcoreid);
			}
		}


		/*
			Find a space in low memory where we can put the 16 bit startup
			code. We only need it while this function is executing, so
			we don't need to bother actually allocating it.
		*/
		size = (uintptr_t)apstart16_end - (uintptr_t)apstart16_start;
		apstart = find_ram(size, 1, 0, 0);
		if(apstart >= 0x100000) {
			if (debug_flag > 0) {
				kprintf("No low memory for AP startup; using bootloader\n");
			}
			apstart = 0x600;
		}
		write_code(apstart, apstart16_start, size);

		// We cream the first few real mode interrupt vectors to get a page aligned
		// addr to jump to. Variable jmp must be declared volatile to prevent the
		// gcc optimizer from detecting a write through a NULL pointer and generating
		// a trap instruction.
		struct far_jmp volatile	* volatile jmp = (void *)0;
		jmp->opcode = 0xea;	/* JMP FAR direct */
		jmp->off = apstart & 0x000f;
		jmp->seg = apstart >> 4;
		if (debug_flag > 0) {
			kprintf("apstart_init(%P,%v) [size=0x%x]\n", apstart, start, size);
		}
		apstart_init(apstart, start);

		// Enable the local APIC
		lapic_write(LAPIC_SVR, lapic_read(LAPIC_SVR) | SVR_APIC_ENABLED);
	}

	id = cpuid_remap[cpu];

	// Send Init command and wait at least 10 msec.
	smp_cmd(id, ICR_INIT);
	pause(10);

	// Send startup command at vector 00 (spec says do it twice).
	smp_cmd(id, ICR_STARTUP);
	pause(1);
	smp_cmd(id, ICR_STARTUP);
	return 1;
}


// Perform any necessary adjustment of the cpu number
unsigned
apic_smp_adjust_num(unsigned cpu) {
	// nothing to do
	return cpu;
}
