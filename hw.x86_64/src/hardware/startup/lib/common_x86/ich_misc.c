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

#include <startup.h>
#include "ich_chipset.h"
#include <x86/pci_misc.h>

/*
 * ich_corporate_model(void)
 *
 * Identify whether the specified ICH <model> is a corporate or consumer model.
 *
 * Returns !0 (true) if it is a corporate model, otherwise 0 (false)
*/
unsigned ich_corporate_model(unsigned model)
{
	/* At this time, only the ICH10 has a consumer/corporate model */
	if (model != 10) return 0;
	else
	{

		/*
		 * we read the 16bit Device Id (offset 0x2) for B0:D31:F0 (LPC). A value
		 * of 0x3A14 or 0x3A1A indicates a corporate ICH10 model, 0x3A16 and 0x3A18
		 * indicate consumer models. The ICH10 is the only ICH I know of that has a
		 * consumer/corporate flavour so we check the entire 16bit value
		*/
		uint16_t did;
		unsigned r = ich_model(&did);
		if (r == 0) {
			kprintf("WARNING: Cannot identify ICH model\n");
			return 0;
		}
		if ((debug_flag > 0) && ((did == 0x3A14) || (did == 0x3A1A))) kprintf("ICH10 corporate\n");
		return (did == 0x3A14) || (did == 0x3A1A);
	}
}

/*
 * ich_model
 *
 * In certain circumstances we need to know the ICH model. This routine will
 * decode the known ICH Device ID values and return an integer model # > 0.
 * Additionally, if <did_p> != NULL, the actual device Id will be returned.
 *
 * A value of 0, indicates the model # could not be determined. In this case
 * *<did_p> should be considered garbage.
*/
unsigned ich_model(uint16_t *did_p)
{
	const uint16_t did = get_LPC_devid();
	unsigned ich_num;

	if (did == 0xFFFFU) return 0;	// may not be an ICH chipset

	/* not found. Let's try and determine from the 'did' */
	ich_num = (did & 0x0F00) >> 8;
	if (debug_flag > 1) kprintf("ICH model: %u (%w)\n", ich_num, did);
	if (did_p != NULL) *did_p = did;
	return ich_num;
}


/*
 * ich_is_gpio
 *
 * Return true (!0) or false (0) depending on whether <pin> is configured as a
 * GPIO
*/
unsigned ich_is_gpio(const unsigned pin)
{
	volatile uint8_t gpio_enabled = GET_LPC_REG8(0x4c) & (1 << 4);	// check bit 4
	if (debug_flag > 1) kprintf("checking GPIO pin %u .. ", pin);
	if (gpio_enabled)
	{
		volatile const uint32_t gpio_base = GET_LPC_REG32(0x48) & 0xFFC0;
		volatile const uint32_t gpio_pin_use_31_0 = in32(gpio_base + 0);
		if (debug_flag > 1) kprintf("%s\n", (gpio_pin_use_31_0 & (1 << pin)) ? "yes" : "no");
		return (gpio_pin_use_31_0 & (1 << pin));
	}
	if (debug_flag > 1) kprintf("no\n");	// GPIO's are not enabled
	return 0;	// <pin> is not configured as a gpio
}

/*
 * ich_pirq_remap_adjust
 *
 * This routine will undo any PIRQ re-mapping done by the BIOS so that the full
 * number of IOAPIC interrupts are usable (normally 16 thru 23). Only PIRQ's
 * which are not configured as GPIO's can be adjusted
*/
void ich_pirq_remap_adjust(void)
{
	static const uint8_t pirq_reg_offset[] =
	{
		0x60, 0x61, 0x62, 0x63,		// PIRQ[A..D]
		0x68, 0x69, 0x6A, 0x6B,		// PIRQ[E..H]
	};
	unsigned i;

	if (debug_flag > 1) kprintf("ICH PIRQ route register layout\n");
	/* for all PIRQ's */
	for (i=0; i<NUM_ELTS(pirq_reg_offset); i++)
	{
		uint8_t pirq = GET_LPC_REG8(pirq_reg_offset[i]);
		if (debug_flag > 1) {const char s[] = {'A' + i, '\0'}; kprintf("PIRQ%s: %x\n", s, pirq);}

		if ((pirq & 0x80) == 0)
		{
#if 0
// it turns out that there is actually an internal PIRQ[E..H] signal path and an external PIRQ[E..H]
// signal path and that they must be OR'd internally. Internal chipset devices ALWAYS drive a PIRQ
// and if it happens to be PIRQ[E..H] this occurs regardless of the GPIO_USE_SEL setting for GPIO[2..5].
// The GPIO_USE_SEL setting for GPIO[2..5] only applies for the external PIRQ[E..H] signal path.
// This is entirely unclear in the docs but is consistent with observed behaviour when forcing the use
// of PIRQ[E..H] onto interrupts 20 - 23
			/* for PIRQ[E..H] which are shared with GPIO pins 2..5, we can only change them
			 * if they are not configured as GPIO's */
			if ((i < 4) || !ich_is_gpio(2+(i-4)))
#endif
			{
				/* leave the re-map IRQ in the lower bits so we know what it was */
				SET_LPC_REG8(pirq_reg_offset[i], pirq | 0x80);
			}
		}
	}
}
