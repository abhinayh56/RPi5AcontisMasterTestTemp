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

#include "x86/chipsets/poulsbo.h"

/* Helpers for accessing the LPC configuration space */
#define GET_LPC_REG32(_offset) pci_read_cfg32(PCI_LPC_BUS_NO,PCI_LPC_DEVICE_NO,PCI_LPC_FUNCTION_NO,(_offset))
#define GET_LPC_REG16(_offset) pci_read_cfg16(PCI_LPC_BUS_NO,PCI_LPC_DEVICE_NO,PCI_LPC_FUNCTION_NO,(_offset))
#define GET_LPC_REG8(_offset) pci_read_cfg8(PCI_LPC_BUS_NO,PCI_LPC_DEVICE_NO,PCI_LPC_FUNCTION_NO,(_offset))
#define SET_LPC_REG32(_offset, _val) pci_write_cfg32(PCI_LPC_BUS_NO,PCI_LPC_DEVICE_NO,PCI_LPC_FUNCTION_NO,(_offset),(_val))
#define SET_LPC_REG16(_offset, _val) pci_write_cfg16(PCI_LPC_BUS_NO,PCI_LPC_DEVICE_NO,PCI_LPC_FUNCTION_NO,(_offset),(_val))
#define SET_LPC_REG8(_offset, _val) pci_write_cfg8(PCI_LPC_BUS_NO,PCI_LPC_DEVICE_NO,PCI_LPC_FUNCTION_NO,(_offset),(_val))

/* Helpers for Intel PIRQ/Poulsbo interrupt routing */
#define poulsbo_get_rcba() (uint32_t)(GET_LPC_REG32(LPC_REG_RCBA) & ~0x1)
#define poulsbo_set_rcba(_addr, _enable) SET_LPC_REG32(LPC_REG_RCBA,((_addr)| ((_enable) ? 0x1 : 0x0)))
#define poulsbo_get_pirqx_map(_x) (uint8_t)(GET_LPC_REG8(LPC_REG_PIRQ_BASE+((_x) & 0x7)))
#define poulsbo_set_pirqx_map(_x, _isa_irq) (SET_LPC_REG8(LPC_REG_PIRQ_BASE+((_x) & 0x7),(_isa_irq) & 0xf))

#define PIRQ_ISA_IRQ(_pirq) (_pirq & 0xf)
#define PIRQ_ISA_ENABLED(_pirq) (!(_pirq & 0x80))

/* Helpers for PCI INTn to PIRQ interrupt routing */
#define IXR_PIRQ_REG(_inta,_intb,_intc,_intd) \
	(uint16_t)((((_intd) & 0xf) << 12) | (((_intc) & 0xf) << 8) | (((_intb) & 0xf) << 4) | ((_inta) & 0xf))
#define IXR_PIRQ_INTA_DEFAULT pirq_a
#define IXR_PIRQ_INTB_DEFAULT pirq_b
#define IXR_PIRQ_INTC_DEFAULT pirq_c
#define IXR_PIRQ_INTD_DEFAULT pirq_d
#define IXR_PIRQ_ALL_DEFAULT IXR_PIRQ_REG(IXR_PIRQ_INTA_DEFAULT,IXR_PIRQ_INTB_DEFAULT,IXR_PIRQ_INTC_DEFAULT,IXR_PIRQ_INTD_DEFAULT)

/* Edge/Level Control Register location */
#define ELCR_MASTER (uint16_t)0x4d0
#define ELCR_SLAVE (uint16_t)0x4d1
#define ISA_LEVEL_TRIG(i0,i1,i2,i3,i4,i5,i6,i7)	(uint8_t)(((i7) << 7) | ((i6)<<6) | ((i5)<<5) | ((i4)<<4) | ((i3)<<3) | ((i2)<<2) | ((i1)<<1) | (i0))

#define MAX_MTRR_VARIABLE_REGION_COUNT 8

/**
 * Struct used to describe entries into the MTRR configuration table, which is
 * used to describe how the 8 variable range MTRRs should be configured.
 * 
 * Note that the choice of paddr32_t (as opposed to paddr64_t) was done simply
 * because the syspage entry used by devg-poulsbo has always been 32bits.
 */
typedef struct _Poulsbo_MTRR_Region {
	paddr32_t start;
	uint32_t size;
	uint32_t flags;
} Poulsbo_MTRR_Region;

/**
 * The MTRR configuration table must be NULL terminated with NULL_MTRR_REGION
 */
#define NULL_MTRR_REGION { 0, 0, 0 }

/* IRQ Routing setup */
void poulsbo_pci_irq_routing(void* rcba_vaddr, uint8_t* pirq_to_irq_map, uint16_t* pci_int_to_pirq_map);

/* MTRR Helpers */
void poulsbo_set_mtrr(uint32_t reg, paddr32_t base, uint32_t size, uint32_t type);
void poulsbo_get_mtrr(uint32_t reg, paddr32_t *base, uint32_t *size, uint32_t *type);

/* Routines to setup a write-combining range for use by the graphics driver */
void poulsbo_config_wc(Poulsbo_MTRR_Region* regions);
void poulsbo_scan_for_wc(void);
