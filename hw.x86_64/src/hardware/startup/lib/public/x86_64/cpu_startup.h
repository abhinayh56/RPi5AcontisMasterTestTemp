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




#include <x86_64/inline.h>
#include <x86_64/priv.h>
#include <x86_64/cpu.h>

#include <x86/lapic.h>


/********************************************************************
*********************************************************************

	These definitions are required for the system independent code.

*********************************************************************
********************************************************************/

#define CPU_SYSPAGE_TYPE	SYSPAGE_X86_64

/*-
 * These cannot be syscall instructions; the amd64 syscall
 * messes a couple of registers, which both the trampoline
 * function and the kernel take into account.
 */
#define KERCALL_SEQUENCE(name)	uint8_t name[] = {		\
					0xcd, 0x28,		/* int $0x28 */		\
					0xcd, 0x20,		/* int $0x20 */		\
}

struct cpu_local_syspage {
	SYSPAGE_TYPED_SECTION(paddr_t, pgdir);
};

#define BOOTSTRAPS_RUN_ONE_TO_ONE	0

/*-
 * 0000..03ff -> bios data area
 * 2000..3fff -> ipl page tables [ if 1G ]
 * 2000..7fff -> ipl page tables [ if 2M ]
 */
#define CPU_MEM_AVOID_START		0
#define CPU_MEM_AVOID_SIZE		KILO(640)

#define CPU_COMMON_OPTIONS_STRING	"Q:BE:x"

#define CPU_PADDR_BITS			52

/********************************************************************
*********************************************************************

	Everything below is specific to the X86_64.

*********************************************************************
********************************************************************/

// Architecture prefixes
#define X86_PREF(__n) X86_64_##__n
#define x86_pref(__n) x86_64_##__n

/* see public/x86_64/intr.h for a description of the GDT layout */
/* first 32 entries */ \
/* TSS for PROCESSORS_MAX CPU's */ \

#define GDT_SIZE \
		( \
			(X86_64_TSS_SEL0) + \
			(2*sizeof(X86_64_GATE) * PROCESSORS_MAX) \
		)

#define FREE_VADDR	X86_64_AS_SLOT2VA(X86_64_AS_STARTUP)

/*CPUID 8.01H (Extended Processor Features) ECX */
#ifndef X86_64_EXTFEAT_ECX_SVM
#define X86_64_EXTFEAT_ECX_SVM		X86_BIT(2)
#endif

//
// Function prototypes
//


void x86_64_init_4m_region(int global);

uintptr_t x86_64_map(uintptr_t vaddr,paddr_t paddr,size_t size,int flags);
uintptr_t x86_64_4m_map(uintptr_t vaddr,paddr_t paddr,size_t size,int flags);
void x86_64_init_pgdir(void);
void x86_64_load_pgdir(unsigned cpu);

void x86_64_print_pagetables(unsigned verbose);

unsigned long x86_64_cpuspeed(unsigned cputype);

unsigned x86_64_cputype(void);
uint32_t x86_64_cpuid(int, uint32_t *);  /* 3+retval */
int x86_64_cpuid_string(char *buf,int max);

void x86_64_setidt(unsigned lim, uintptr_t base);
void x86_64_setgdt(unsigned lim, uintptr_t base);
void x86_64_setsegs(unsigned newcs);


void x86_64_v86_add_mapping(paddr_t start, paddr_t len);

//
// Variable prototypes
//
extern unsigned				gdt_size;
extern unsigned				idt_size;
extern uintptr_t            gdt_ptr;
extern uintptr_t            idt_ptr;

#include <x86/x86both_startup.h>

/* __SRCVERSION("cpu_startup.h $Rev: 984519 $"); */
