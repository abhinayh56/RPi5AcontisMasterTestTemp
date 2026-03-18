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

#ifndef __X86_CHIPSETS_POULSBO_H_INCLUDED
#define __X86_CHIPSETS_POULSBO_H_INCLUDED

/* PCI space defines */
#define PCI_DEVICE_ROUTE_CONFIG_OFFSET 0x3140
#define PCI_LPC_BUS_NO 0
#define PCI_LPC_DEVICE_NO 31
#define PCI_LPC_FUNCTION_NO	0

/* Intel LPC defines */
#define LPC_REG_VID			0x0
#define LPC_REG_DID			0x2
#define LPC_REG_GPIO_BASE	0x44
#define LPC_REG_PIRQ_BASE	0x60
#define LPC_REG_PIRQA		0x60
#define LPC_REG_PIRQB		0x61
#define LPC_REG_PIRQC		0x62
#define LPC_REG_PIRQD		0x63
#define LPC_REG_PIRQE		0x64
#define LPC_REG_PIRQF		0x65
#define LPC_REG_PIRQG		0x66
#define LPC_REG_PIRQH		0x67
#define LPC_REG_RCBA		0xF0
#define LPC_REG_MANF_ID		0xF8

#define POULSBO_ROOT_COMPLEX_SIZE (16*1024)

/* MSR defines */
#define defType_MSR         0x2FF
#define physBase_MSR(reg)   (0x200 + 2 * (reg))
#define physMask_MSR(reg)   (0x200 + 2 * (reg) + 1)

enum irc_device_interface_offsets {
	irc_lpc = 0,
	irc_sdio = 1,
	irc_usb_host = 2,
	irc_pci_exp = 3,
	irc_hd_audio = 4,
	irc_usb_target = 5,
	irc_vga = 6,
	irc_invalid = 7,
};

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

#endif /* __X86_CHIPSETS_POULSBO_H_INCLUDED */
