/*
 * Copyright (c) 2023, BlackBerry Limited.
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


#include "startup.h"
#include <hw/acpi.h>
#include <x86/intel_misc.h>

#define PCI_MAX_NUM_OF_BUSES	256U
#define PCI_MAX_NUM_OF_DEVICES	32U
#define PCI_MAX_NUM_OF_FUNCS	8U

/* ECAM info data type */
typedef struct ecam_info_s
{
	paddr_t base;
	uint64_t  size;
} ecam_info_t;

/*
 ===============================================================================
 * find_ecam_info_by_io_probe
 *
 * Adapted from the PCI server code
 * This function will try and determine the ECAM information by either reading
 * chipset specific registers based on the host bridge Device ID or by probing
 * well known register locations known to hold this information.
 * return 0 if ECAM information is properly set in ecam_info, otherwise return -1.
 *
 */
static int find_ecam_info_by_io_probe(ecam_info_t *const info) {
	paddr_t ecam_paddr = 0;
	int ret = -1;
	/*
		* some typical locations of and values for the ECAM address. Note
		* that BIOS's seem to be able to set this value since when the HPz820
		* BIOS was updated we found that the ECAM address was moved from
		* 0xE0000000 to 0xF0000000 but we could not find 0xF0000000 in any
		* bridge device location nor any documentation for this value in
		* the E5 specs.
		*/
	static const struct
	{
		unsigned cfg_offset;
		uint32_t addr_or_offset;
	} ecam_addr[] =
	{
		{.cfg_offset = 1, .addr_or_offset = 0x60},
		{.cfg_offset = 1, .addr_or_offset = 0x48},
		{.cfg_offset = 0, .addr_or_offset = 0xe0000000u},
		{.cfg_offset = 0, .addr_or_offset = 0xf0000000u},
		{.cfg_offset = 0, .addr_or_offset = 0x80000000u},
	};

	const uint32_t D0F0_vid_did = pci_read_cfg32(0, 0, 0, 0);

	if (D0F0_vid_did != 0xFFFFFFFFU) {
		for (unsigned i = 0; i < NUM_ELTS(ecam_addr); i++) {
			if (ecam_addr[i].cfg_offset) {
				if (debug_flag > 4) {
					kprintf("%s(): trying offset 0x%x for vid/did %x\n", __FUNCTION__, ecam_addr[i].addr_or_offset, D0F0_vid_did);
				}
				ecam_paddr = (paddr_t)pci_read_cfg32(0, 0, 0, (uint8_t)ecam_addr[i].addr_or_offset);
			} else {
				if (debug_flag > 4) {
					kprintf("%s(): trying addr 0x%x for vid/did %x\n", __FUNCTION__, ecam_addr[i].addr_or_offset, D0F0_vid_did);
				}
				ecam_paddr = (paddr_t)ecam_addr[i].addr_or_offset;
			}

			/* remove lower bits associated with attributes */
			ecam_paddr &= ~(paddr_t)7U;

			void *const p = startup_memory_map(MEG(1), ecam_paddr, PROT_NOCACHE | PROT_READ | PROT_WRITE);
			if (p != NULL) {
				if (*((uint32_t *)p) == D0F0_vid_did) {
					if (debug_flag > 4) {
						kprintf("%s(): found ecam base 0x%L\n", __FUNCTION__, ecam_paddr);
					}
					info->base = ecam_paddr;
					/* assuming the maximum address range is 256*32*8*4K since
					* there is no way to tell the size of configuration space for IO probe.
					*/
					info->size = PCI_MAX_NUM_OF_BUSES * PCI_MAX_NUM_OF_DEVICES * PCI_MAX_NUM_OF_FUNCS * KILO(4);
					ret = 0;
					startup_memory_unmap(p);
					break;
				} else {
					startup_memory_unmap(p);
				}
			} else {
				kprintf("Failed to map ECAM memory space\n");
			}
		}
	}
	return ret;
}

static void dump_acpi_table_hdr(const acpi_generic *const tbl, const unsigned len) {
	kprintf("ACPI table info: len=%u \n", len);
	kprintf("  signature = 0x%x len = 0x%x Revision: 0x%b Checksum: 0x%b\n",
			tbl->hdr.Signature, tbl->hdr.Length, tbl->hdr.Revision, tbl->hdr.Checksum);
	kprintf("  Oem ID : %b%b%b%b%b%b\n",
			tbl->hdr.OEMID[0], tbl->hdr.OEMID[1], tbl->hdr.OEMID[2],
			tbl->hdr.OEMID[3], tbl->hdr.OEMID[4], tbl->hdr.OEMID[5]);
}

/*
 ===============================================================================
 * find_ecam_info_by_io_acpi
 *
 * Adapted from AWS board specific code.
 * This function will try and determine the ECAM information by querying ACPI table(MCFG sub table).
 * return 0 if ECAM information is properly set in ecam_info, otherwise return -1.
 *
 */
static int find_ecam_info_by_acpi(ecam_info_t *const info) {
	unsigned len;
	int ret = -1;
	const paddr_t paddr = acpi_find_table(ACPI_MCFG_SIGNATURE, &len);
	if (paddr != NULL_PADDR) {
		acpi_mcfg *const mcfg = startup_memory_map(len, paddr, PROT_READ);
		if (mcfg != NULL) {
			const acpi_cfg_space *const p = &mcfg->Cfg_Space[0];
			if (debug_flag > 4) {
				dump_acpi_table_hdr((acpi_generic*)mcfg, len);
				kprintf("Base_address=%L\n", p->Base_Address); 
				kprintf("Start_PCI_Bus_Number=%u\n", p->Start_PCI_Bus_Number);
				kprintf("End_PCI_Bus_Number=%u\n", p->End_PCI_Bus_Number);
			}

			info->base =  p->Base_Address;
			info->size = (p->End_PCI_Bus_Number - p->Start_PCI_Bus_Number + 1U) * PCI_MAX_NUM_OF_DEVICES * PCI_MAX_NUM_OF_FUNCS * KILO(4);
			startup_memory_unmap(mcfg);
			ret = 0;
		} else {
			kprintf("Failed to map MCFG table\n");
		}
	} else {
		kprintf("ACPI table not found (0x%x)\n", ACPI_MCFG_SIGNATURE);
	}
	return ret;
}

/*
 ===============================================================================
 * find_ecam_info_by_io_acpi
 * 
 * This function will try ACPI method first. If it is failed, it will retry with
 * IO probing method for Intel CPU.
 *
 * Notes: This function will return ECAM base and size directly without querying
 * hardware once ecam_info structure is set by previous call.
 *
 */
int find_ecam_base(paddr_t *const ecam_base, uint64_t *const size) {

	static ecam_info_t ecam_info = {.base = NULL_PADDR, .size = 0ULL};
	int ret = 0;

	if ((ecam_info.base == NULL_PADDR) || (ecam_info.size == 0)) {
		ret = find_ecam_info_by_acpi(&ecam_info);
		if ((ret == -1) && is_intel_chipset()) {
			/* Try IO probe for Intel CPU if there is no ACPI table */
			ret = find_ecam_info_by_io_probe(&ecam_info);
		}
	}

	*ecam_base = ecam_info.base;
	*size = ecam_info.size;

	return ret;
}
