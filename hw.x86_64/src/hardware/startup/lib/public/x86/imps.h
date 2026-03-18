/*
 * $QNXLicenseC:
 * Copyright 2009 QNX Software Systems. 
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

#ifndef _IMPS_H_
#define _IMPS_H_


//#include <_pack1.h>

struct smp_fps {
	_Uint32t	signature;
	_Paddr32t	config;
	_Uint8t		length;
	_Uint8t		spec_rev;
	_Uint8t		checksum;
	_Uint8t		features[5];
};

struct smp_hdr {
	_Uint32t	signature;
	_Uint16t	base_length;
	_Uint8t		spec_rev;
	_Uint8t		checksum;
	_Uint8t		oem_id[8];
	_Uint8t		prod_id[12];
	_Paddr32t	oem_table_ptr;
	_Uint16t	oem_table_size;
	_Uint16t	entry_count;
	_Paddr32t	lapic_addr;
	_Uint16t	extended_length;
	_Uint8t		extended_checksum;
	_Uint8t		reserved[1];
};


#define MP_PROCESSOR	0
#define MP_BUS			1
#define MP_IOAPIC		2
#define MP_INTSRC		3
#define MP_LINTSRC		4
struct smp_processor {
	_Uint8t		type;			// Type 0
	_Uint8t		id;
	_Uint8t		ver;
	_Uint8t		flags;
	_Uint32t	signature;
	_Uint32t	features;
	_Uint8t		reserved[8];
};


struct smp_bus {
	_Uint8t		type;			// Type 1
	_Uint8t		id;
	_Uint8t		bus_type[6];
};

struct smp_ioapic {
	_Uint8t		type;			// Type 2
	_Uint8t		id;
	_Uint8t		ver;
	_Uint8t		flags;
	_Uint32t	addr;
};

struct smp_intsrc {
	_Uint8t		type;			// Type 3
	_Uint8t		int_type;
	_Uint16t	flags;
	_Uint8t		source_bus_id;
	_Uint8t		source_bus_intr;
	_Uint8t		dest_apic_id;
	_Uint8t		dest_apic_intin;
};

struct smp_lintsrc {
	_Uint8t		type;			// Type 4
	_Uint8t		int_type;
	_Uint16t	flags;
	_Uint8t		source_bus_id;
	_Uint8t		source_bus_intr;
	_Uint8t		dest_apic_id;
	_Uint8t		dest_apic_intin;
};

/* INTSRC defines for interrupt attributes */
#define IMPS_INTSRC_ACTIVE_LO		(3U << 0)
#define IMPS_INTSRC_ACTIVE_HI		(1U << 0)
#define IMPS_INTSRC_ACTIVE_MASK		(IMPS_INTSRC_ACTIVE_LO | IMPS_INTSRC_ACTIVE_HI)
#define IMPS_INTSRC_TRIG_LEVEL		(3U << 2)
#define IMPS_INTSRC_TRIG_EDGE		(1U << 2)
#define IMPS_INTSRC_TRIG_MASK		(IMPS_INTSRC_TRIG_LEVEL | IMPS_INTSRC_TRIG_EDGE)
#define IMPS_INTSRC_INTTYPE_FIX	0
#define IMPS_INTSRC_INTTYPE_NMI	1		
#define IMPS_INTSRC_INTTYPE_SMI	2
#define IMPS_INTSRC_INTTYPE_EXT	3

#define IMPS_INTSRC_FLAGS_MASK		(IMPS_INTSRC_ACTIVE_MASK | IMPS_INTSRC_TRIG_MASK)


extern struct smp_fps *smp_get_fps(void);
extern ssize_t imps_entry_size(unsigned type);
extern void *ioapic_intsrc_imps(int ioapic_id, void *start, struct smp_intsrc *intsrc);
extern void *ioapic_lintsrc_imps(int ioapic_id, void *start, struct smp_intsrc *intsrc);
extern unsigned is_imcr_present(void);
extern void imps_resolve_bus_int_flags(PADDR_T addr, struct smp_intsrc *intsrc);

#endif	/* _IMPS_H_ */
