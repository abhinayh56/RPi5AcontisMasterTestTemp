/*
 * $QNXLicenseC:
 * Copyright 2016, 2020, QNX Software Systems.
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
#include <hw/uefi.h>
#include <x86/intel_misc.h>
#include <x86/pci_misc.h>


const struct debug_device debug_devices[] = {

	{	"console",
		{	"",
			NULL
		},
		init_vga,
		put_vga,
		{	&display_char_vga,
			&poll_key_pckbd,
		}
	},
	{	"uefi_console",
		{	"",
			NULL
		},
		uefi_io_init,
		uefi_print_char,
		{
			&display_char_dummy,
			&poll_key_dummy,
			&break_detect_dummy,
		}
	},
	{	"8250",
		{	"0x3f8^0.57600.1843200.16",
			"0x2f8^0.57600.1843200.16"
		},
		init_8250,
		put_8250,
		{	&display_char_8250,
			&poll_key_8250,
			&break_detect_8250,
		}
	},
	{	"8250_pci_io",
		{	"0:24:2:0^0.115200.1843200.16"
		},
		init_8250_pci,
		put_8250,
		{	&display_char_8250,
			&poll_key_8250,
			&break_detect_8250,
		}
	},
};

//
// This startup program must execute out of RAM. It performs 2 functions.
//
// 1. It gathers information about the system and places it in a structure
//    called the system page. The kernel references this structure to
//    determine everything it needs to know about the system. This structure
//    is also available to user programs (read only if protection is on)
//    via _syspage_ptr->.
//
// 2. It (optionally) turns on the MMU and starts the next program
//    in the image file system.
//
int
main(int argc, char **argv, char **envv)
{
	int opt;
	int reserve = 1;
	size_t shadow_rom = 0;

	while((opt = getopt(argc, argv, COMMON_OPTIONS_STRING "bzp:s:")) != -1) {
		switch(opt) {
		case 'b':
			reserve = 0;
			break;
		case 's':
			shadow_rom = getsize(optarg, 0);
			if(shadow_rom == 0) shadow_rom = 1;
			break;
		case 'p':		// add devices to the known PCI device table
		{
			/*
			 * this option is intended to allow the pci_device_table[] to be
			 * extended to accommodate systems for which we either have not
			 * come across a device or which a bus, device or function number
			 * has changed. An example would be a PXH IO-APIC which is on a
			 * different bus than that on which it is known to be on the
			 * Supermicro PDSM4+/PDSME+
			 * The format of the this option is Bx:Dy:Fz where x, y and z are
			 * the bus, device and functions numbers. The -p option may be
			 * given multiple times in order to add multiple devices
			*/
			char *s = optarg;
			char *p;
			const char fmt_char[] = {'B', 'D', 'F'};
			const char fmt_sep[] = {':', ':', '\0'};
			unsigned int fmt_value[3];
			unsigned int idx = 0;

			while (*s != '\0')
			{
				if (*s != fmt_char[idx]) break;	// bad format
				else if ((p = strchr(s, fmt_sep[idx])) == NULL) break;	// bad format
				else
				{
					*p = '\0';
					if (++s == p) break;	// bad format (no digits)
					fmt_value[idx] = strtoul(s, NULL, 0);
					s = p + 1;
				}
				if (++idx == 3)
				{
					/* properly constructed option. Add to the table */
					add_known_pci_device(fmt_value[0], fmt_value[1], fmt_value[2]);
					break;
				}
			}
			break;
		}
		default:
			handle_common_option(opt);
			break;
		}
	}

	//
	// Initialization path for UEFI
	//
	if (is_uefi_boot())
	{
		// Initialize the UEFI environment
		uefi_init();

		// Initialize debugging output (kprintf won't work before this point)
		select_debug(&debug_devices[1], sizeof(debug_devices) - sizeof(debug_devices[0]));

		kprintf("\nUEFI boot\n");
		if (acpi_ignore) kprintf("Ignoring ACPI tables\n");

		// Collect information on all free RAM in the system with
		// the EFI service routine GetMemoryMap
		init_raminfo_uefi();
	}

	//
	// Initialization path for non-UEFI/UEFI+CSM
	//
	else
	{
		// Initialize debugging output (kprintf won't work before this point)
		select_debug(debug_devices, sizeof(debug_devices));

		kprintf("\nnon UEFI or UEFI+CSM boot\n");
		if (acpi_ignore) kprintf("Ignoring ACPI tables\n");

		// Collect information on all free RAM in the system gathered by
		// the bootloader or from a memory scan
		init_raminfo();
	}

	//
	// We normally stay away from the 1st 4K of memory. It is needed
	// to implement virtual 86 mode. If this capability is not needed
	// then this memory does not need to be allocated making it available.
	//
	if(reserve) alloc_ram(0L, 4096, 1);

	/*
	 * Enable Hypervisor if requested (and possible)
	 */
	hypervisor_init(0);

	//
	// Find out how many AP's we've got (must be done before init_mmu).
	//
	init_smp();

	//
	// In a virtual system we need to initialize the page tables
	//
	if(shdr->flags1 & STARTUP_HDR_FLAGS1_VIRTUAL) init_mmu();

	if(shadow_rom != 0) x86_pcbios_shadow_rom(0xc0000, shadow_rom);

	// ---------------------------------------------------------------------//
	//                                                                      //
	// The following routines have hardware or system dependencies which    //
	// may need to be changed on a non-PC compatible system. The default    //
	// routines assume a BIOS is present. If you do not have a BIOS you     //
	// will need to replace these routines with ones of your own.           //
	//                                                                      //
	init_hwinfo();
	init_intrinfo();
	init_qtime();
	init_cacheattr();
	init_cpuinfo();
	init_reboot_callout();

	add_typed_string(_CS_MACHINE, "x86pc");
	//LATER: add_typed_string(_CS_HW_PROVIDER, Do_a_ROM_scan());
	//LATER: add_typed_string(_CS_HW_SERIAL, Do_a_ROM_scan());
	//                                                                      //
	// ---------------------------------------------------------------------//

	//
	// Load the bootstrap executables in the image filesystem and
	// initialize various syspage pointers. This must be the *last*
	// initialization done before transfering to the next program.
	//
	init_system_private();

	//
	// This is real handy for debugging a new version of this startup
	// program. Commenting this line out will save a whack of code.
	//
	print_syspage();
	return 0;
}
