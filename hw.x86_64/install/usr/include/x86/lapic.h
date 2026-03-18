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

#ifndef __LAPIC_H__
#define __LAPIC_H__

#include <x86/imps.h>
#include <hw/acpi.h>

#define LAPIC_ID	0x0020
#define LAPIC_VER	0x0030
#define LAPIC_TPR	0x0080
#define LAPIC_APR	0x0090
#define LAPIC_PPR	0x00a0
#define LAPIC_EOI	0x00b0
#define LAPIC_LDR	0x00d0
#define LAPIC_DFR	0x00e0
#define LAPIC_SVR	0x00f0
#define LAPIC_SPIV	0x00f0
#define LAPIC_ESR	0x0280
#define LAPIC_ICRL	0x0300
#define LAPIC_ICRH	0x0310
#define LAPIC_LTR	0x0320
#define LAPIC_LTMR	0x0330
#define LAPIC_LPCR	0x0340
#define LAPIC_LL0R	0x0350
#define LAPIC_LL1R	0x0360
#define LAPIC_LER	0x0370
#define LAPIC_ICR	0x0380
#define LAPIC_CCR	0x0390
#define LAPIC_TDCR	0x03e0

#define ICR_INIT	0x00004500L
#define ICR_STARTUP	0x00004600L

#define LVR_VERSION_SHIFT			0
#define LVR_VERSION_MASK			0xff
#define LVR_MAX_LVT_SHIFT			16
#define LVR_MAX_LVT_MASK			(0xff << 16)

#define LVT_VECTOR_SHIFT			0
#define LVT_VECTOR_MASK				0xff
#define LVT_DELIVERY_MODE_SHIFT		8
#define LVT_DELIVERY_MODE_MASK		(0x7 << 8)
#define LVT_DELIVERY_MODE_FIXED		(0x0 << 8)
#define LVT_DELIVERY_MODE_SMI		(0x2 << 8)
#define LVT_DELIVERY_MODE_NMI		(0x4 << 8)
#define LVT_DELIVERY_MODE_ExtINT	(0x7 << 8)
#define LVT_DELIVERY_MODE_INIT		(0x5 << 8)
#define LVT_DELIVERY_STATUS			(1<<12)
#define LVT_POLARITY_LOW			(1<<13)
#define LVT_REMOTE_IRR				(1<<14)
#define LVT_TRIGGER_MODE_LEVEL		(1<<15)
#define LVT_MASKED					(1<<16)
#define LVT_TIMER_MODE_PERIODIC		(1<<17)

#define SVR_VECTOR_SHIFT			0
#define SVR_VECTOR_MASK				0xff
#define SVR_APIC_ENABLED			(1<<8)
#define SVR_FOCUS_DISABLED			(1<<9)
#define SVR_EOI_BROADCAST_DISABLED	(1<<12)

#define lapic_read(reg)			(*(volatile uint32_t *)((uintptr_t)lapic_startup_vaddr+(reg)))
#define lapic_write(reg, val)	(*(volatile uint32_t *)((uintptr_t)lapic_startup_vaddr+(reg)) = (val));

/* IOAPIC stuff */
typedef struct
{
	unsigned			vector;		// assigned vector
	unsigned			cpu_mask;	// set of processors eligible to receive this interrupt vector
	unsigned			masked;		// boolean (==0 it is unmasked, otherwise it is masked)
	struct smp_intsrc	intsrc;		// all of the other required info as defined in the MP Config table
} ioapic_cfg_t;

typedef struct
{
	unsigned (*num_entries)(const unsigned ioapic_id);
	unsigned (*configure)(const unsigned ioapic_id, const ioapic_cfg_t *ioapic_cfg, const int num);
	uint64_t (*read_entry)(const unsigned ioapic_id, const unsigned entry_num);
	int      (*write_entry)(const unsigned ioapic_id, const unsigned entry_num, const uint64_t data);
} ioapic_accessfnc_t;

void apic_add_as_entry(const int cpu, const paddr_t lapic_addr);
void *ioapic_intsrc_acpi(int ioapic_id, void *start, acpi_madt_interrupt_source_overrides *intsrc);
void apic_set_vaddr(void);

#endif	/* __LAPIC_H__ */

/* __SRCVERSION("lapic.h $Rev: 984519 $"); */
