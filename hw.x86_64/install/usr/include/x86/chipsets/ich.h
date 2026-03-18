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

#ifndef __CHIPSETS_ICH_H_INCLUDED
#define __CHIPSETS_ICH_H_INCLUDED

#define DEFAULT_RCBA 0xFED1C000
#define RCBA_ENABLE 1
#define DEFAULT_PMBASE 0x400
#define DEFAULT_GPIOBASE 0x480

#define ICH_ROOT_COMPLEX_SIZE (16*1024)

/* 
 * ICH PCI configuration registers and offsets
 */
#define LPC_BRIDGE_BDF 0, 31, 0
 #define	LPC_BRIDGE_PMBASE 0x40
 #define	LPC_BRIDGE_ACPI_CNTL 0x44
 #define	LPC_BRIDGE_GPIOBASE 0x48
 #define	LPC_BRIDGE_GPIOCNTL 0x4C
 #define	LPC_BRIDGE_PIRQA 0x60
 #define	LPC_BRIDGE_PIRQB 0x61
 #define	LPC_BRIDGE_PIRQC 0x62
 #define	LPC_BRIDGE_PIRQD 0x63
 #define	LPC_BRIDGE_PIRQE 0x68
 #define	LPC_BRIDGE_PIRQF 0x69
 #define	LPC_BRIDGE_PIRQG 0x6A
 #define	LPC_BRIDGE_PIRQH 0x6B
 #define	LPC_SIRQ_CNTL 0x64
 #define	LPC_BRIDGE_IO_DEC_RANGES 0x80
 #define	LPC_BRIDGE_ENABLES 0x82
 #define	LPC_BRIDGE_GEN_DEC_RANGES 0x84
 #define	LPC_BRIDGE_GEN_PMCON1 0xA0
 #define	LPC_BRIDGE_GEN_PMCON2 0xA2
 #define	LPC_BRIDGE_GEN_PMCON3 0xA4
 #define	LPC_BRIDGE_CX_STATE_CNF 0xA9
 #define	LPC_BRIDGE_MISC_FNC 0xAD
 #define	LPC_BRIDGE_GPIO_ROUTE 0xB8
 #define	LPC_BRIDGE_FWH_DEC_EN1 0xD8
 #define	LPC_BRIDGE_BIOS_CNTL 0xDC
 #define	LPC_BRIDGE_RCBA 0xF0

#define SMBUS_CTRLR_BDF 0, 31, 3
 #define	SMBUS_CTRLR_SMB_BASE 0x20
 #define	SMBUS_CTRLR_HOST_CFG 0x40

/* SMBus I/O registers and offsets. See Ref#307013 Section 14.2 */
#define	SMB_IO_BASE			0x500	/* SMBUS Host Control Register offset */
#define SMB_REG_HSTS		0x00	/* SMBUS Host Status Register offset */
 #define	SMB_REG_HSTS_CLEAR_BITS		0x1E		/* HSTS[4:1] = Bits to be cleared before / after an SMBus command */
 #define	SMB_REG_HSTS_STATUS_BITS	0x0E		/* HSTS[3:1] = Bits indicating command completion (pass or fail) */
 #define	SMB_REG_HSTS_FAILED_BITS	0x0C		/* HSTS[3:2] = Bits indicating command failure */	
#define SMB_REG_HCTL		0x02	/* SMBUS Host Control Register offset */
 #define	SMB_REG_HCTL_BYTE_DATA		(1 << 3)	/* HCTL[3] = Byte data command (TSA + Command + Data0) */
 #define	SMB_REG_HCTL_START			(1 << 6)	/* HCTL[6] = Start transaction */
#define SMB_REG_CMD			0x03	/* SMBUS Command Register */
#define SMB_REG_TSA			0x04	/* SMBUS Transmit Slave Address offset */
#define SMB_REG_HD0			0x05	/* SMBUS Data 0 Register */

/* GPIO registers and offsets. */
#define GPIO_USE_SEL 0x00
#define GPIO_SEL 0x04
#define GPIO_LVL 0x0C
#define GPIO_BLINK 0x18
#define GPIO_SER_BLINK 0x1C
#define GPIO_SB_STS 0x20
#define GPIO_SB_DATA 0x24
#define GPIO_INV 0x2C
#define GPIO_USE_SEL2 0x30
#define GPIO_SEL2 0x34
#define GPIO_LVL2 0x38

#define SATA_HOST_BDF 0, 31, 2
#define HDA_DEV_BDF 0, 27, 0
#define UHCI0_DEV_BDF 0, 29, 0
#define UHCI1_DEV_BDF 0, 29, 1
#define UHCI2_DEV_BDF 0, 29, 2
#define UHCI3_DEV_BDF 0, 29, 3
#define EHCI_DEV_BDF 0, 29, 7
 #define	EHCI_DEV_ACCESS_CTRL 0x80

/* ICH RCRB Registers and offsets (based at DEFAULT_RCBA) */
#define ICH_FD 0x3418 /* 32bits */
#define ICH_GCS 0x3410 /* 32bits */
#define ICH_PCI_DEVICE_ROUTE_CONFIG 0x3140 /* 5 x 16bits */

/* ISA IRQ helpers */
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

/* PIRQ label to offset map */
enum pirq_label {
	pirq_a = 0,
	pirq_b = 1,
	pirq_c = 2,
	pirq_d = 3,
	pirq_e = 4,
	pirq_f = 5,
	pirq_g = 6,
	pirq_h = 7,
	pirq_reserved_start = 8,
	pirq_reserved_end = 15,
};

/* 
 * Utility functions for reading within the Root-Complex register space
 */
static uint64_t inline
rcrb_rd64(off_t offset)
{
	if ((offset & 7) == 0) {
		return *((volatile uint64_t*)(DEFAULT_RCBA + offset)); 
	}
	
	return 0;
}

static uint32_t inline
rcrb_rd32(off_t offset)
{
	if ((offset & 3) == 0) {
		return *((volatile uint32_t*)(DEFAULT_RCBA + offset)); 
	}
	
	return 0;
}

static uint16_t inline
rcrb_rd16(off_t offset)
{
	if ((offset & 1) == 0) {
		return *((volatile uint16_t*)(DEFAULT_RCBA + offset));
	}
	
	return 0;
}

static uint8_t inline
rcrb_rd8(off_t offset)
{
	return *((volatile uint8_t*)(DEFAULT_RCBA + offset));
}

/* 
 * Utility functions for writing to the Root-Complex register space
 */
static void inline
rcrb_wr64(off_t offset, uint64_t val)
{
	if ((offset & 7) == 0) {
		*((volatile uint64_t*)(DEFAULT_RCBA + offset)) = val;
	}
}

static void inline
rcrb_wr32(off_t offset, uint32_t val)
{
	if ((offset & 3) == 0) {
		*((volatile uint32_t*)(DEFAULT_RCBA + offset)) = val;
	}
}

static void inline
rcrb_wr16(off_t offset, uint16_t val)
{
	if ((offset & 1) == 0) {
		*((volatile uint16_t*)(DEFAULT_RCBA + offset)) = val;
	}
}

static void inline
rcrb_wr8(off_t offset, uint8_t val)
{
	*((volatile uint8_t*)(DEFAULT_RCBA + offset)) = val;
}

/* 
 * Utility functions for masked writing to Root-Complex registers
 */
static void inline
rcrb_reg_wr64(off_t offset, uint64_t mask, uint64_t val)
{
	rcrb_wr64(offset, ((rcrb_rd64(offset) & ~mask) | val));
}

static void inline
rcrb_reg_wr32(off_t offset, uint32_t mask, uint32_t val)
{
	rcrb_wr32(offset, ((rcrb_rd32(offset) & ~mask) | val));
}

static void inline
rcrb_reg_wr16(off_t offset, uint16_t mask, uint16_t val)
{
	rcrb_wr16(offset, ((rcrb_rd16(offset) & ~mask) | val));
}

static void inline
rcrb_reg_wr8(off_t offset, uint8_t mask, uint8_t val)
{
	rcrb_wr8(offset, ((rcrb_rd8(offset) & ~mask) | val));
}

/*
 * Utility functions for setting bits in Root-Complex registers
 */
static void inline
rcrb_set64(off_t offset, uint64_t bits)
{
	rcrb_wr64(offset, rcrb_rd64(offset) | bits);
}

static void inline
rcrb_set32(off_t offset, uint32_t bits)
{
	rcrb_wr32(offset, rcrb_rd32(offset) | bits);
}

static void inline
rcrb_set16(off_t offset, uint16_t bits)
{
	rcrb_wr16(offset, rcrb_rd16(offset) | bits);
}

static void inline
rcrb_set8(off_t offset, uint8_t bits)
{
	rcrb_wr8(offset, rcrb_rd8(offset) | bits);
}

/*
 * Utility functions for clearing bits in Root-Complex registers
 */
static void inline
rcrb_clear64(off_t offset, uint64_t bits)
{
	rcrb_wr64(offset, rcrb_rd64(offset) & ~bits);
}

static void inline
rcrb_clear32(off_t offset, uint32_t bits)
{
	rcrb_wr32(offset, rcrb_rd32(offset) & ~bits);
}

static void inline
rcrb_clear16(off_t offset, uint16_t bits)
{
	rcrb_wr16(offset, rcrb_rd16(offset) & ~bits);
}

static void inline
rcrb_clear8(off_t offset, uint8_t bits)
{
	rcrb_wr8(offset, rcrb_rd8(offset) & ~bits);
}

#endif /* __CHIPSETS_ICH_H_INCLUDED */
