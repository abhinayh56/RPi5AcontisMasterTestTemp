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
#include <startup.h>
#include "ich_chipset.h"

void
ich_pci_irq_routing(void* rcba_vaddr, uint8_t* pirq_to_irq_map, uint16_t* pci_int_to_pirq_map, uint_t pci_dev_count)
{
	int i;
	volatile uint16_t* irqdesc;
	uint32_t gpiobase;
	uint32_t gpio_use_sel;
	
	/* 
	 * Confirm that GPIO[2-5] aren't enabled since they are multiplexed with
	 * PIRQ[E-H]
	 */
	gpiobase = pci_read_cfg32(LPC_BRIDGE_BDF, LPC_BRIDGE_GPIOBASE) & ~1;
	gpio_use_sel = in32(gpiobase + GPIO_USE_SEL);
	if (gpio_use_sel & 0x3C) {
		crash("PIRQ[E-H] unavailable since some of GPIO[2-5] are enabled: 0x%x", gpio_use_sel);
	}
	
	/* Remap our PIRQs[A-D] to different ISA lines */
	for (i = 0; i < 4; i++) {
		pci_write_cfg8(LPC_BRIDGE_BDF, LPC_BRIDGE_PIRQA+i, pirq_to_irq_map[i]);
	}
	
	/* Remap our PIRQs[E-H] to different ISA lines */
	for (i = 0; i < 4; i++) {
		pci_write_cfg8(LPC_BRIDGE_BDF, LPC_BRIDGE_PIRQE+i, pirq_to_irq_map[i+4]);
	}
	
	/* Remap our PCI INT[A-D] interrupts to different PIRQS */
	irqdesc = (uint16_t*)(((unsigned char*)rcba_vaddr) + ICH_PCI_DEVICE_ROUTE_CONFIG);
	for (i = 0; i < pci_dev_count; i++) {
		irqdesc[i] = pci_int_to_pirq_map[i];
	}
}
