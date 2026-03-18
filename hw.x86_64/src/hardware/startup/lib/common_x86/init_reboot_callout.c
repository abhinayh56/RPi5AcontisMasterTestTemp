/*
 * $QNXLicenseC:
 * Copyright 2014, QNX Software Systems.
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
#include <hw/acpi.h>

#define REBOOT_REG_ADDR_MASK	0xFFFFULL

// reboot arguments
static reboot_patch_args_t	reboot_arg; 

const struct callout_slot reboot_call = {CALLOUT_SLOT(reboot,)};

void init_reboot_callout(void) 
{
	unsigned			len;
	paddr_t				paddr;
	acpi_fadt			*fadt;
	void 				*call;

	// By default the reset register is 0xCF9, reset value is 0xE
	reboot_arg.reg_addr = (paddr_t)0xCF9;
	reboot_arg.value = 0x0E;
	reboot_arg.reg_width = sizeof(uint8_t);
	
	// Scan for ACPI table
	paddr = acpi_find_table(ACPI_FADT_SIGNATURE, &len);

	if (paddr != NULL_PADDR)
	{
		fadt = startup_memory_map(len, paddr, PROT_READ);

		// If ACPI support reset register
		if (fadt->Flags & ACPI_FADT_FLAGS_RESET_REG_SUP) 
		{
			if (fadt->RESET_REG.Address.sio & ~REBOOT_REG_ADDR_MASK)
			{
				// If the ACPI address is more than 16bit, report a warning, 
				// since we only use port with 16 bit width address
				kprintf("ACPI address 0x%L is more than 16 bit width, use 0xCF9 port instead\n", fadt->RESET_REG.Address.sio);
			}
			else
			{
				// Only use the lower 16 bit address
				reboot_arg.reg_addr = (paddr_t)(fadt->RESET_REG.Address.sio & REBOOT_REG_ADDR_MASK);
				// Only use the lower 8 bit value is used.
				reboot_arg.value = (uint32_t)(fadt->RESET_VALUE & 0xff);
				if (debug_flag) kprintf("found ACPI reset reg addr/value of %P/%x\n", reboot_arg.reg_addr, reboot_arg.value);
			}
		}
		startup_memory_unmap(fadt);
	}

	// Add reboot callout
	call = add_callout(reboot_call.offset, reboot_call.callout);
	callout_register_data(call, (void *)&reboot_arg);
	if (debug_flag) kprintf("using reset reg addr/value of %P/%x\n", reboot_arg.reg_addr, reboot_arg.value);
}
