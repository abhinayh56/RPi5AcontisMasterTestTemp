/*
 * Copyright 2010, 2022, 2023, 2025 BlackBerry Limited.
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
#include <x86/intel_misc.h>

/*
 * This file applies to all newer Intel platforms which have moved timer and
 * interrupt control to the ITSS device and includes, but is not limited to the
 * following SoC's
 *
 *		Apollo Lake/Broxton
 *		SkyLake/Kaby Lake
 *		Denverton
*/


/*
 ===============================================================================
 * get_itss_port_id
 *
 * Returns the ITSS message port ID. I cannot find docs for this anywhere but
 * by trial and error, the following seems to work.
 *
 * In the future I may add a scan of some sort
 *
*/
static unsigned get_itss_port_id(void)
{
	const unsigned devId = get_LPC_devid();
	unsigned port_id;

	switch(devId)
	{
		/*
		 * not sure about the Skylake U and Y SKU's as I don't have hardware to
		 * test but we'll assume the same port ID for now
		 */
		case INTEL_SKYLAKE_SKU_UY_LPC_DIDS:	// Skylake U and Y SKU's
		case INTEL_SKYLAKE_LPC_DIDS:		// Skylake full featured (A, B, H, Q, Z SKU's)
		case INTEL_ANDERSON_LAKE_LPC_DIDS:		// Anderson Lake
		case INTEL_COFFEELAKE_LPC_DIDS:
		case INTEL_TIGERLAKE_LPC_DIDS:
		case INTEL_ELKHARTLAKE_LPC_DIDS:
		case INTEL_RAPTORLAKE_LPC_DIDS:
		case INTEL_ALDERLAKE_LPC_DIDS:
		case INTEL_AMSTONLAKE_LPC_DIDS:
		case INTEL_SERIES_C62x_SUPER_SKUS_LPC_DIDS:
		case INTEL_SERIES_C62xA_SUPER_SKUS_LPC_DIDS:
		case INTEL_SERIES_C62x_PROD_SKUS_LPC_DIDS:
		case INTEL_SERIES_C62xA_PROD_SKUS_LPC_DIDS:
			port_id = 0xC4 << 16;
			break;
		default:
			port_id = 0xD0 << 16;
			break;
	}
	return port_id;
}

/*
 ===============================================================================
 * get_itss_base_addr
 *
 * The base address for the ITSS unit is obtained from the P2SB device BAR. That
 * in conjunction with a port ID is used to access the device.
 *
 * If the P2SB device cannot be found, a fixed base address will be used
 *
*/
paddr_t get_itss_base_addr(void)
{
	static paddr_t itss_base_addr = NULL_PADDR;

	if (itss_base_addr == NULL_PADDR)
	{
		const int bus = 0;
		const unsigned p2sb_devfunc = find_P2SB();

		if (p2sb_devfunc != 0xFFFFFFFF)
		{
			const _Uint32t bar_l = pci_read_cfg32(bus, PCI_DEVNO(p2sb_devfunc), PCI_FUNCNO(p2sb_devfunc), 0x10);
			if (bar_l != 0xFFFFFFFF)
			{
#if _PADDR_BITS > 32
				const _Uint32t bar_h = pci_read_cfg32(bus, PCI_DEVNO(p2sb_devfunc), PCI_FUNCNO(p2sb_devfunc), 0x14);
				itss_base_addr = ((paddr_t)bar_h << 32) | (bar_l & ~0xF);
#else
				itss_base_addr = bar_l & ~0xF;
#endif
			}
		}
		else
		{
			const unsigned devId = get_LPC_devid();

			switch(devId)
			{
				case INTEL_APOLLO_LAKE_LPC_DIDS:	// Apollo Lake
					itss_base_addr = 0xD0000000;
					break;
				case INTEL_DENVERTON_LPC_DIDS:		// Denverton
				case INTEL_SKYLAKE_SKU_UY_LPC_DIDS:	// Skylake U and Y SKU's
				case INTEL_SKYLAKE_LPC_DIDS:		// Skylake full featured (A, B, H, Q, Z SKU's)
				case INTEL_COFFEELAKE_LPC_DIDS:
				case INTEL_TIGERLAKE_LPC_DIDS:
				case INTEL_ELKHARTLAKE_LPC_DIDS:
				case INTEL_RAPTORLAKE_LPC_DIDS:
				case INTEL_ALDERLAKE_LPC_DIDS:
				case INTEL_AMSTONLAKE_LPC_DIDS:
				case INTEL_SERIES_C62x_SUPER_SKUS_LPC_DIDS:
				case INTEL_SERIES_C62xA_SUPER_SKUS_LPC_DIDS:
				case INTEL_SERIES_C62x_PROD_SKUS_LPC_DIDS:
				case INTEL_SERIES_C62xA_PROD_SKUS_LPC_DIDS:
					itss_base_addr = 0xFD000000;
					break;
				default:
					break;
			}
			if (debug_flag > 1) kprintf("No P2SB device found, using default value of 0x%P\n", itss_base_addr);
		}
		itss_base_addr |= get_itss_port_id();
	}
	return itss_base_addr;
}

/*
 ===============================================================================
 * itss_pirq_remap_adjust
 *
*/
void itss_pirq_remap_adjust(void)
{
	const paddr_t itss_base_addr = get_itss_base_addr();

	if (itss_base_addr == NULL_PADDR) kprintf("Unable to check ITSS PIRQ remap registers\n");
	else
	{
		volatile _Uint8t *itss_base_p = startup_memory_map(8, itss_base_addr + 0x3100, PROT_NOCACHE | PROT_READ | PROT_WRITE);

		//kprintf("itss_base_addr: %x, itss_base_p: %x\n", itss_base_addr, itss_base_p);
		if (debug_flag > 1) kprintf("ITSS PIRQ route register layout\n");
		/* for all PIRQ's */
		unsigned i;
		for (i=0; i<8; i++)
		{
			if (debug_flag > 1) {const char s[] = {'A' + i, '\0'}; kprintf("PIRQ%s: %x\n", s, itss_base_p[i]);}

			if ((itss_base_p[i] & 0x80) == 0)
			{
				/* leave the re-map IRQ in the lower bits so we know what it was */
				itss_base_p[i] |= 0x80;
			}
			//kprintf("itss_base_p[%u] @ 0x%x = 0x%x\n", i, &itss_base_p[i], itss_base_p[i]);
		}
		startup_memory_unmap((void *)itss_base_p);
	}
}

/* polarity control API's */

static unsigned polarity_ctrl_valid = 0;
/* layout of the Interrupt Polarity Control registers in supporting chipsets */
typedef struct __attribute__((packed, aligned(4)))
{
	volatile _Uint32t ipc[4];
} ipc_t;
static ipc_t *polarity_ctrl = NULL;
#define BITS_PER_ENTRY	(sizeof(polarity_ctrl->ipc[0]) * 8)

/*
 ===============================================================================
 * itss_ioapic_polarity_get
 *
 * returns -1 for active low, +1 for active high, 0 for undetermined,
 * unsupported or any other error
 *
*/
int itss_polarity_get(const unsigned ioapic_id, const unsigned ioapic_entry_num)
{
	if (polarity_ctrl == NULL)
	{
		/* we do this code block once to get access to the polarity registers
		 * and validate that they are implemented */
		const paddr_t itss_base_addr = get_itss_base_addr();

		if (itss_base_addr == NULL_PADDR) kprintf("Unable to check ITSS polarity registers\n");
		else
		{
			volatile _Uint32t *itss_base_p = startup_memory_map(sizeof(polarity_ctrl->ipc), itss_base_addr + 0x3200, PROT_NOCACHE | PROT_READ);
			if (itss_base_p != NULL)
			{
				polarity_ctrl = (ipc_t *)itss_base_p;

				if (debug_flag > 4)
				{
					kprintf("%s, itss_base_addr: %P mapped @ %v\n",
								__FUNCTION__, itss_base_addr, itss_base_p);
				}

				unsigned zero_count = 0;
				unsigned i;
				for (i=0; i<NUM_ELTS(polarity_ctrl->ipc); i++)
				{
					if (polarity_ctrl->ipc[i] == 0) ++zero_count;
					if (debug_flag > 4) kprintf("IPC%u: 0x%x, ", i, polarity_ctrl->ipc[i]);
				}
				if (debug_flag > 4) kprintf("\n");

				/* if every register is 0, we'll consider them not implemented
				 * because we know that at least inputs 16 thru 23 MUST be non-zero */
				if (zero_count < i) polarity_ctrl_valid = 1;
			}
		}
	}
	int polarity = 0;	/* assume error */
	if (polarity_ctrl_valid == 1)
	{
		const ioapic_accessfnc_t * const access_fn = ioapic_get_accessfnc(ioapic_id);
		if (access_fn == NULL) kprintf("%s, invalid ID %u, no access functions\n", __FUNCTION__, ioapic_id);
		else if (ioapic_entry_num < access_fn->num_entries(ioapic_id))
		{
			if (polarity_ctrl != NULL)
			{
				const unsigned entry_idx = ioapic_entry_num / BITS_PER_ENTRY;
				const unsigned bit_idx = ioapic_entry_num % BITS_PER_ENTRY;
				const unsigned polarity_bit = (polarity_ctrl->ipc[entry_idx] >> bit_idx) & 1;

				if (debug_flag > 4)
				{
					kprintf("%s(%u, %u), checking entry %u, bit %u, 0x%x, setting %u\n",
							__FUNCTION__, ioapic_id, ioapic_entry_num, entry_idx, bit_idx,
							polarity_ctrl->ipc[entry_idx], polarity_bit);
				}
				/* in these registers, 1 means LO polarity, 0 means HI */
				polarity = (polarity_bit == 1) ? -1 : 1;
			}
			else
			{
				kprintf("%s(%u, %u), polarity_ctrl is NULL\n", __FUNCTION__,
						ioapic_id, ioapic_entry_num);
			}
		}
		else if (debug_flag > 2)
		{
			kprintf("%s(%u, %u), Invalid ioapic_entry_num\n", __FUNCTION__,
						ioapic_id, ioapic_entry_num);
		}
	}
	else if (debug_flag > 2)
	{
		kprintf("%s(%u, %u), No valid IPC registers found\n", __FUNCTION__,
					ioapic_id, ioapic_entry_num);
	}
	if (debug_flag > 3)
	{
		kprintf("%s(%u, %u) returns %d\n", __FUNCTION__, ioapic_id,
					ioapic_entry_num, polarity);
	}
	return polarity;
}

/*
 ===============================================================================
 * itss_polarity_set
 *
 * returns the polarity set (-1 active low, +1 active high) or 0 for unsupported
 * or any other error
 *
*/
int itss_polarity_set(const unsigned ioapic_id, const unsigned ioapic_entry_num, const int polarity)
{
	int polarity_set = 0;

	if ((polarity == -1) || (polarity == 1))
	{
		/*
		 * this call sets up access to the registers and gets the current polarity
		 * If it fails for any reason, we're done
		 */
		polarity_set = itss_polarity_get(ioapic_id, ioapic_entry_num);

		if (polarity_set != polarity)
		{
			ASSERT((polarity_set == -1) || (polarity_set == 1));
			const unsigned entry_idx = ioapic_entry_num / BITS_PER_ENTRY;
			const unsigned bit_idx = ioapic_entry_num % BITS_PER_ENTRY;

			/* in these registers, 1 means LO polarity, 0 means HI */
			if (polarity == -1) polarity_ctrl->ipc[entry_idx] |= (1u << bit_idx);
			else polarity_ctrl->ipc[entry_idx] &= ~(1u << bit_idx);

			/* re-read the value from the register */
			polarity_set = itss_polarity_get(ioapic_id, ioapic_entry_num);
		}
	}
	if (debug_flag > 3)
	{
		kprintf("%s(%u, %u, %d) returns %d\n", __FUNCTION__,
					ioapic_id, ioapic_entry_num, polarity, polarity_set);
	}
	return polarity_set;
}

/*
 ===============================================================================
 * itss_trigger_mode_get()
 *
 * returns 0 for level, 1 for edge and -1 for undetermined, unsupported or any
 * other error
 *
*/
int itss_trigger_mode_get(const unsigned ioapic_id, const unsigned ioapic_entry_num)
{
	int trigger_mode = -1;

	if (debug_flag > 3)
	{
		kprintf("%s(%u, %u) returns %d\n", __FUNCTION__, ioapic_id,
					ioapic_entry_num, trigger_mode);
	}
	return trigger_mode;
}

/*
 ===============================================================================
 * itss_trigger_mode_set()
 *
 * returns the trigger mode set (0 for level, 1 for edge) or -1 for unsupported
 * or any other error
 *
*/
int itss_trigger_mode_set(const unsigned ioapic_id, const unsigned ioapic_entry_num, const unsigned trigger_mode)
{
	int new_trigger_mode = -1;

	if (debug_flag > 3)
	{
		kprintf("%s(%u, %u, %d) returns %d\n", __FUNCTION__,
					ioapic_id, ioapic_entry_num, trigger_mode, new_trigger_mode);
	}
	return new_trigger_mode;
}
