/*
 * $QNXLicenseC: 
 * Copyright 2010, QNX Software Systems.  
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

#include <stdint.h>
#include <sys/types.h>

#ifndef __CHIPSETS_I945_H_INCLUDED
#define __CHIPSETS_I945_H_INCLUDED

/*
 * Memory (address) constants
 */
#define DEFAULT_PCIEXBAR	0xE0000000U /* PCI Enhanced Config Space base address */
#define DEFAULT_EPBAR		0xFED19000U /* Egress Port base address */
#define DEFAULT_DMIBAR		0xFED18000U /* Direct Media Interface register range base address */
#define DEFAULT_MCHBAR		0xFED14000U /* Memory Controller Hub base address */

#define BARENABLE	1

/* 
 * 945 GSE PCI configuration registers and offsets
 */
#define HOST_BRIDGE_BDF 0, 0, 0
 #define	HOST_BRIDGE_EPBAR 0x40
 #define	HOST_BRIDGE_MCHBAR 0x44
 #define	HOST_BRIDGE_PCIEXBAR 0x48
 #define	HOST_BRIDGE_DMIBAR 0x4C
 #define	HOST_BRIDGE_DEVEN 0x54
 #define	HOST_BRIDGE_PAM0 0x90
 #define	HOST_BRIDGE_PAM1 0x91
 #define	HOST_BRIDGE_PAM2 0x92
 #define	HOST_BRIDGE_PAM3 0x93
 #define	HOST_BRIDGE_PAM4 0x94
 #define	HOST_BRIDGE_PAM5 0x95
 #define	HOST_BRIDGE_PAM6 0x96
 #define	HOST_BRIDGE_TOLUD 0x9C
 #define	HOST_BRIDGE_TOM 0xA0

#define GRAPHICS0_DEV_BDF 0, 2, 0
#define GRAPHICS1_DEV_BDF 0, 2, 1
 #define	GRAPHICS_DEV_UPMC4 0xC1
 #define	GRAPHICS_DEV_GCFC 0xF0

/* 
 * Utility functions for reading within the MCH register space
 */
static inline uint64_t
mch_rd64(off_t offset)
{
	if ((offset & 7) == 0) {
		return *((volatile uint64_t*)(DEFAULT_MCHBAR + offset)); 
	}
	
	return 0;
}

static inline uint32_t
mch_rd32(off_t offset)
{
	if ((offset & 3) == 0) {
		return *((volatile uint32_t*)(DEFAULT_MCHBAR + offset)); 
	}
	
	return 0;
}

static inline uint16_t
mch_rd16(off_t offset)
{
	if ((offset & 1) == 0) {
		return *((volatile uint16_t*)(DEFAULT_MCHBAR + offset));
	}
	
	return 0;
}

static inline uint8_t
mch_rd8(off_t offset)
{
	return *((volatile uint8_t*)(DEFAULT_MCHBAR + offset));
}

/* 
 * Utility functions for writing to the MCH register space
 */
static inline void
mch_wr64(off_t offset, uint64_t val)
{
	if ((offset & 7) == 0) {
		*((volatile uint64_t*)(DEFAULT_MCHBAR + offset)) = val;
	}
}

static inline void
mch_wr32(off_t offset, uint32_t val)
{
	if ((offset & 3) == 0) {
		*((volatile uint32_t*)(DEFAULT_MCHBAR + offset)) = val;
	}
}

static inline void
mch_wr16(off_t offset, uint16_t val)
{
	if ((offset & 1) == 0) {
		*((volatile uint16_t*)(DEFAULT_MCHBAR + offset)) = val;
	}
}

static inline void
mch_wr8(off_t offset, uint8_t val)
{
	*((volatile uint8_t*)(DEFAULT_MCHBAR + offset)) = val;
}

/* 
 * Utility functions for masked writing to MCH registers
 */
static inline void
mch_reg_wr64(off_t offset, uint64_t mask, uint64_t val)
{
	mch_wr64(offset, ((mch_rd64(offset) & ~mask) | val));
}

static inline void
mch_reg_wr32(off_t offset, uint32_t mask, uint32_t val)
{
	mch_wr32(offset, ((mch_rd32(offset) & ~mask) | val));
}

static inline void
mch_reg_wr16(off_t offset, uint16_t mask, uint16_t val)
{
	mch_wr16(offset, ((mch_rd16(offset) & ~mask) | val));
}

static inline void
mch_reg_wr8(off_t offset, uint8_t mask, uint8_t val)
{
	mch_wr8(offset, ((mch_rd8(offset) & ~mask) | val));
}

/*
 * Utility functions for setting bits in MCH registers
 */
static inline void
mch_set64(off_t offset, uint64_t bits)
{
	mch_wr64(offset, mch_rd64(offset) | bits);
}

static inline void
mch_set32(off_t offset, uint32_t bits)
{
	mch_wr32(offset, mch_rd32(offset) | bits);
}

static inline void
mch_set16(off_t offset, uint16_t bits)
{
	mch_wr16(offset, mch_rd16(offset) | bits);
}

static inline void
mch_set8(off_t offset, uint8_t bits)
{
	mch_wr8(offset, mch_rd8(offset) | bits);
}

/*
 * Utility functions for clearing bits in MCH registers
 */
static inline void
mch_clear64(off_t offset, uint64_t bits)
{
	mch_wr64(offset, mch_rd64(offset) & ~bits);
}

static inline void
mch_clear32(off_t offset, uint32_t bits)
{
	mch_wr32(offset, mch_rd32(offset) & ~bits);
}

static inline void
mch_clear16(off_t offset, uint16_t bits)
{
	mch_wr16(offset, mch_rd16(offset) & ~bits);
}

static inline void
mch_clear8(off_t offset, uint8_t bits)
{
	mch_wr8(offset, mch_rd8(offset) & ~bits);
}

#endif /* __CHIPSETS_I945_H_INCLUDED */
