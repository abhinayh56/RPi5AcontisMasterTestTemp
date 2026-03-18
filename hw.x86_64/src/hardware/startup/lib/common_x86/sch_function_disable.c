/*
 * Copyright 2010,2023, BlackBerry Limited.
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

/*
 * sch_function_disable
 *
 * By disabling unused functions they will not be visible during the PCI scan.
 * This can speed up the boot process and will conserve PCI memory/IO space.
 *
 * All devices are assumed Bus 0 devices. By default all devices are enabled
 * after reset
 *
 * The following device/function/offsets exist in the SCH
 *
 * D30:F0,F1,F2 FD register offset 0xFC
 * D29:F0,F1,F2 FD register offset 0xFC, F7 FD register offset 0xC0
 * D28:F0,F1 FD register offset 0xFC
 * D27:F0 FD register offset 0xFC
 * D26:F0 FD register offset 0xFC
 * D2:F0 FD register offset 0xC4
 *
 * Unfortunately we cannot disable D31:F1 (PATA interface)
 *
 * Returns: !0 (true) if the function was successfully disabled otherwise 0 (false)
 *
 * IMPORTANT:
 * The PCI spec requires that a Function 0 device exist. If disabling a function
 * 0 device, all other functions for that device must also be disabled.
 * This routine does not enforce this so the caller must take precaution
*/
unsigned sch_function_disable(unsigned device, unsigned function)
{
	unsigned fd_offset;
	unsigned disabled;

	switch(device)
	{
		/* the following devices have their FD register at offset 0xFC */
		case 26:	// devices 26 and 27 have only function 0
		case 27:
			if (function > 0) return 0;
			/* Fall through */
		case 28:	// device 28 has functions 0 and 1
			if (function > 1) return 0;
			/* Fall through */
		case 30:	// device 30 has functions 0, 1 and 2
			if (function > 2) return 0;

			fd_offset = 0xFC;
			break;

		/* the following device has its FD register at offset 0xFC or 0xC0 */
		case 29:	// device 29 has functions 0, 1, 2 and 7
			if ((function > 2) && (function != 7)) return 0;
			fd_offset = (function == 7) ? 0xC0 : 0xFC;
			break;

		case 2:		// device 2 has function 0
			if (function > 0) return 0;
			fd_offset = 0xC4;
			break;

		default:
			return 0;
	}
	/* clear the PCI CMD register and disable the function */
	pci_write_cfg32(0, device, function, 0x4, 0);
	pci_write_cfg32(0, device, function, fd_offset, pci_read_cfg32(0, device, function, fd_offset) | 0x1);

	disabled = pci_read_cfg32(0, device, function, fd_offset) & 0x1;

	if (debug_flag > 0) kprintf("PCI D%u:F%u %s disabled\n", device, function, disabled ? "" : "NOT");
	return disabled;
}
