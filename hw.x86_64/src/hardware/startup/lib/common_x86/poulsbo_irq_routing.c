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
#include "poulsbo_chipset.h"

#ifdef POULSBO_STARTUP_DEBUG

#define DEBUG_PRINT_IRQS(desc, rcba_vaddr) { \
	kprintf("%s:\n", desc); \
	poulsbo_irq_debug(rcba_vaddr); \
	kprintf("======================\n"); \
}

static char* pirq_label_str[16] = {
	"pirqA","pirqB","pirqC","pirqD","pirqE","pirqF","pirqG","pirqH","OPPS"
};

static char* pirq_alpha[] = {"PIRQA","PIRQB","PIRQC","PIRQD","PIRQE","PIRQF","PIRQG","PIRQH",NULL};

static void decode_irqdesc(uint16_t desc)
{
	uint8_t a,b,c,d;

	a = desc & 0xf;
	b = (desc >>4) & 0xf;
	c = (desc >>8) & 0xf;
	d = (desc >>12) & 0xf;

	kprintf("%s %s %s %s\n", pirq_label_str[a],pirq_label_str[b],pirq_label_str[c],pirq_label_str[d]);
}
	
void poulsbo_irq_debug(char* rcba)
{
	uint16_t *irqdesc;
	int i;
	uint8_t pirq;

	for(i=pirq_a ; i <= pirq_h ; i++) {
		pirq = poulsbo_get_pirqx_map(i);
		kprintf("%s: %x -> ISA irq %d\n",pirq_alpha[i],pirq, PIRQ_ISA_IRQ(pirq));
	}

	irqdesc = (uint16_t*)(rcba + PCI_DEVICE_ROUTE_CONFIG_OFFSET);
	kprintf("LPC: %x\n", irqdesc[irc_lpc]);
	decode_irqdesc(irqdesc[irc_lpc]);
	kprintf("SDIO: %x\n", irqdesc[irc_sdio]);
	decode_irqdesc(irqdesc[irc_sdio]);
	kprintf("USB Host: %x\n", irqdesc[irc_usb_host]);
	decode_irqdesc(irqdesc[irc_usb_host]);
	kprintf("PCI Ex: %x\n", irqdesc[irc_pci_exp]);
	decode_irqdesc(irqdesc[irc_pci_exp]);
	kprintf("HD Audio: %x\n", irqdesc[irc_hd_audio]);
	decode_irqdesc(irqdesc[irc_hd_audio]);
	kprintf("USB Target: %x\n", irqdesc[irc_usb_target]);
	decode_irqdesc(irqdesc[irc_usb_target]);
	kprintf("VGA: %x\n", irqdesc[irc_vga]);
	decode_irqdesc(irqdesc[irc_vga]);
}

#else /* POULSBO_STARTUP_DEBUG */

#define DEBUG_PRINT_IRQS(desc, rcba_vaddr) {}

#endif /* POULSBO_STARTUP_DEBUG */

/**
 * This function configures the IRQ routing for the Poulsbo devices. It sets the
 * INTn to PIRQ map as well as the PIRQ to IRQ map.
 */
void poulsbo_pci_irq_routing(void* rcba_vaddr, uint8_t* pirq_to_irq_map, uint16_t* pci_int_to_pirq_map)
{
	int i;
	volatile uint16_t* irqdesc;
	
	DEBUG_PRINT_IRQS("Before Routing", rcba_vaddr);
	
	/* Remap our PIRQs to different ISA lines */
	for (i = pirq_a; i <= pirq_h; i++) {
		poulsbo_set_pirqx_map(i, pirq_to_irq_map[i]);
	}
	
	/* Remap our PCI INT [A-D] interrupts to different PIRQS */
	irqdesc = (uint16_t*)(((unsigned char*)rcba_vaddr) + PCI_DEVICE_ROUTE_CONFIG_OFFSET);
	for (i = irc_lpc; i < irc_invalid; i++) {
		irqdesc[i] = pci_int_to_pirq_map[i];
	}
	
	DEBUG_PRINT_IRQS("After Routing", rcba_vaddr);
}
