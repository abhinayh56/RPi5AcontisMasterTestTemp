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

#ifndef ICH_CHIPSET_H_
#define ICH_CHIPSET_H_

#include <x86/pci_misc.h>
#include <x86/chipsets/ich.h>

/* IRQ Routing setup */
void ich_pci_irq_routing(void* rcba_vaddr, uint8_t* pirq_to_irq_map, uint16_t* pci_int_to_pirq_map, uint_t pci_dev_count);

// Miscellaneous ICH southbridge routines
unsigned ich_corporate_model(unsigned model);
unsigned ich_model(uint16_t *did_p);
unsigned ich_is_gpio(const unsigned pin);
void ich_pirq_remap_adjust(void);
void ich_pci_irq_routing(void* rcba_vaddr, uint8_t* pirq_to_irq_map, uint16_t* pci_int_to_pirq_map, uint_t pci_dev_count);

#define ich_get_rcba()	(uint32_t)(GET_LPC_REG32(LPC_BRIDGE_RCBA) & ~0x1)

#endif /* ICH_CHIPSET_H_ */
