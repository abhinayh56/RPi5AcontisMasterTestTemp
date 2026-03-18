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
#include <x86/pci_misc.h>

/*
 * This file applies to all newer Intel platforms which have moved timer and
 * interrupt control to the ITSS device and includes, but is not limited to the
 * following SoC's
 *
 *		Apollo Lake/Broxton
 *		SkyLake/Kaby Lake
 *		Denverton
 *		Anderson Lake
*/

/*
 ===============================================================================
 * unhide_P2SB
 *
 * The P2SB device may be hidden preventing us from getting the base address of
 * the sideband message port in order to communicate with the ITSS.
 *
 * This function will look for the known P2SB devices and if they are not found
 * attempt to unhide them. We check first because the UEFI/ABL may have unhidden
 * them for use
 *
*/
static void unhide_P2SB(void)
{
	unsigned p2sb_devfunc;
	const unsigned devId = get_LPC_devid();

	switch(devId)
	{
		case INTEL_APOLLO_LAKE_LPC_DIDS:	// Apollo Lake
			p2sb_devfunc = PCI_DEVFUNC(13, 0);
			break;
		default:		// Skylake/Denverton/??
			p2sb_devfunc = PCI_DEVFUNC(31, 1);
			break;
	}
	unsigned ps2b_unhidden = 0;		/* false by default */
	const unsigned hidden = pci_read_cfg32(0, PCI_DEVNO(p2sb_devfunc), PCI_FUNCNO(p2sb_devfunc), 0) == 0xFFFFFFFF;

	if (hidden)
	{
		if (debug_flag > 1) {
			kprintf("Attempting to unhide B0:D%u:F%u\n", PCI_DEVNO(p2sb_devfunc), PCI_FUNCNO(p2sb_devfunc));
		}

		/*
		 * Note:
		 * we use the ECAM to access the byte address in order to unhide the
		 * P2SB device because our I/O based config space functions do not allow
		 * this
		 */
		paddr_t ecam_pbase;
		uint64_t ecam_size;
		const int ret = find_ecam_base(&ecam_pbase, &ecam_size);
		if (!ret && (ecam_pbase != NULL_PADDR))
		{
			void * const ecam_base = startup_memory_map(MEG(1), ecam_pbase, PROT_NOCACHE | PROT_READ | PROT_WRITE);
			if (ecam_base != NULL)
			{
				volatile _Uint32t * const reg_e0_p = (_Uint32t *)(((_Uintptrt)ecam_base) + (p2sb_devfunc << 12));
				/* write down bit 0 as an 8 bit access to offset 0xE1 */
				((volatile _Uint8t *)reg_e0_p)[0xE1] = (_Uint8t)~(1u << 0);
				/* this next read ensures the write completes */
				_Uint8t reg_e1 = ((volatile _Uint8t *)reg_e0_p)[0xE1];
				/* this check ensures the compiler does not optimize out the read operation */
				if ((reg_e1 & 1) != 0) kprintf("Unable to unhide P2SB\n");

				/* final check that the device is unhidden is to read the VID */
				const _Uint16t vid = ((volatile _Uint16t *)reg_e0_p)[0];
				ps2b_unhidden = (vid == 0xffff) ? 0 : 1;
				startup_memory_unmap(ecam_base);
			}
			else
			{
				kprintf("%s(): unable to map ECAM memory space\n", __FUNCTION__);
			}
		}
		else
		{
			kprintf("%s(): unable to find ECAM base\n", __FUNCTION__);
		}
	}

	if (debug_flag > 1) {
		kprintf("%s(): %s\n", __FUNCTION__, (hidden == 0) ? "not hidden" : ((ps2b_unhidden == 1) ? "ok" : "failed"));
	}
}

/*
 ===============================================================================
 * find_P2SB
 *
 * Adapted from itss_irq.c from the PCI server
 *
 * The P2SB has been assigned a class code of 5,0x80 (other memory controller).
 * Unfortunately, because of the rather ambiguous meaning of "Other Memory
 * Controller" this class code is also used by other devices as well.
 * On some ITSS based platforms, the P2SB has been at BDF 0:31:1 however we can't
 * rely on that because its 0:13:0 on Apollo Lake. What does seem to be consistent
 * in the docs that are available is that the P2SB has a single 16MB memory BAR
 * and so we will use that information in conjunction with the 5,0x80 class code
 * to determine the P2SB BDF.
 *
 * However before we can do any of that, we may need to unhide the P2SB device
 * because normally Intel hides it. That check will be done first
 *
*/
unsigned find_P2SB(void)
{
	static unsigned p2sb_devfunc = 0xFFFFFFFF;

	if (p2sb_devfunc == 0xFFFFFFFF)
	{
		unhide_P2SB();

		int cookie = 0;
		int bus = 0;
		unsigned devfunc;

		while ((cookie = pci_find_class(cookie, 5, 0x80, &bus, &devfunc)) != -1)
		{
			/* probe to get the BAR size */
			_Uint32t cur_bar = pci_read_cfg32(bus, PCI_DEVNO(devfunc), PCI_FUNCNO(devfunc), 0x10);

			if (cur_bar != 0xFFFFFFFF)
			{
				pci_write_cfg32(bus, PCI_DEVNO(devfunc), PCI_FUNCNO(devfunc), 0x10, 0xFFFFFFFF);
				_Uint32t bar = pci_read_cfg32(bus, PCI_DEVNO(devfunc), PCI_FUNCNO(devfunc), 0x10);

				if (bar != 0xFFFFFFFF)
				{
					/* we don't care if the BAR is 64 bit, just that its a 16 MB memory BAR */
					const unsigned is_mem_bar = (bar & 0x1) == 0;
					const _Uint32t bar_size = ~(bar & ~0xF) + 1;
					if (is_mem_bar && (bar_size == 0x1000000))
					{
						p2sb_devfunc = devfunc;
						if (debug_flag > 0) {
							kprintf("Found P2SB device at B%u:D%u:F%u\n", bus,
									PCI_DEVNO(p2sb_devfunc), PCI_FUNCNO(p2sb_devfunc));
						}
					}
				}
			}
			/* restore the BAR */
			pci_write_cfg32(bus, PCI_DEVNO(devfunc), PCI_FUNCNO(devfunc), 0x10, cur_bar);

			if (p2sb_devfunc != 0xFFFFFFFF) break;
		}
	}
	return p2sb_devfunc;
}
