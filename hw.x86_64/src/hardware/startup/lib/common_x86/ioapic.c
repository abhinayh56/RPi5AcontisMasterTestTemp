/*
 * Copyright 2009,2025, BlackBerry Limited.
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
#include <x86/lapic.h>
#include <hw/acpi.h>

/* FIX ME - do we need the IOAPIC info in the syspage ? */
static struct ioapic_array_s
{
	PADDR_T paddr;
	void *vaddr;
	int gsi;	// Global System Interrupt assignment (-1 means unassigned)
	const ioapic_accessfnc_t *accessfncs;
	unsigned int autofill_profile_num;
} *ioapic_array = NULL;

static unsigned num_ioapics = 0;
/* normally IOAPIC's are numbered sequentially starting at the first ID after
 * the number of LAPIC's/CPU's. We won't assume this however */
static unsigned highest_id = 0;			// highest assigned IOAPIC id
static unsigned first_id = UINT_MAX;	// first assigned IOAPIC id

static __inline__ void *realloc_ram(void *p, size_t len)
{
	void *new_p = ws_alloc(len);
	/* this copies data beyond previous <p> but we have access and
	 * it will be cleared by the caller */
	if ((p != NULL) && (new_p != NULL))
	{
		memcpy(new_p, p, len);
	}
	return new_p;
}

/*
 * add IOAPIC address information to the ioapic_array.
 *
 * Typically, this function is called by either ioapic_discover_acpi() or
 * ioapic_discover_imps() as the ACPI/MP config tables are scanned respectively
 *
 * We allow an ioapic_id entry to be overridden if the caller specifies an
 * explicit <ioapic_id>
 *
 * The <gsi> parameter is only available from ACPI.
 *
 * The <accessfncs> parameter allows chip specific access functions
 *
 *
 * Note that it is not necessary that the tables be used to add IOAPIC's
*/
void add_ioapic(int ioapic_id, PADDR_T paddr, int gsi, ioapic_accessfnc_t *accessfncs)
{
	ASSERT(ioapic_id >= 0);	// for now we don't support an unspecified ioapic_id
	
	/* do not allow the same IOAPIC to be added twice (this is not a rewrite since ioapic_id < 0) */
	if ((ioapic_id < 0) && (ioapic_array != NULL))
	{
		unsigned i;
		for (i=0; i<num_ioapics; i++) {
			if (ioapic_array[i].paddr == paddr) {
				kprintf("IOAPIC %d @ paddr %P already added\n", i, (paddr_t)paddr);
				return;
			}
		}
	}

	ioapic_id = (ioapic_id < 0) ? num_ioapics : ioapic_id;

	/* determine whether or not the ioapic_array needs to be increased in size */
	if ((ioapic_id > highest_id) || (ioapic_array == NULL))
	{
		unsigned i;
		const unsigned start = (ioapic_array == NULL) ? 0 : highest_id + 1;  

		ioapic_array = realloc_ram(ioapic_array, sizeof(*ioapic_array) * (ioapic_id + 1));
		ASSERT(ioapic_array != NULL);

		/* clear out any unset entries */
		for (i=start; i<ioapic_id; i++)
		{
			ioapic_array[i].paddr = NULL_PADDR_STARTUP;
			ioapic_array[i].vaddr = NULL;
			ioapic_array[i].gsi = -1;
			ioapic_array[i].accessfncs = NULL;
			ioapic_array[i].autofill_profile_num = 0;
		}
		highest_id = ioapic_id;
	}
	if (ioapic_id < first_id) first_id = ioapic_id;

	/* paddr is used to access the IOAPIC in startup. vaddr is used by the callouts */
	ioapic_array[ioapic_id].paddr = paddr;
	ioapic_array[ioapic_id].vaddr = callout_memory_map(__PAGESIZE, paddr, PROT_READ|PROT_WRITE|PROT_NOCACHE);
	ASSERT(ioapic_array[ioapic_id].vaddr != NULL);
	ioapic_array[ioapic_id].gsi = gsi;
	ioapic_array[ioapic_id].accessfncs = accessfncs;
	ioapic_array[ioapic_id].autofill_profile_num = 6;	/* Default is profile 6 */

	++num_ioapics;

	if (debug_flag > 1) {
		kprintf("Added IO-APIC %d @ %P (vaddr: %v)\n", ioapic_id, (paddr_t)ioapic_array[ioapic_id].paddr, ioapic_array[ioapic_id].vaddr);
	}
}

/*
 * ioapic_get_num
 *
 * return the number of IOAPIC's
 *
 * IMPORTANT: the difference between the last assigned id and the first assigned
 *			  id IS NOT necessarily the number of IOAPIC's. Use ioapic_get_num()
 *			  to get the number of IOAPIC's
*/
unsigned ioapic_get_num(void)
{
	return num_ioapics;
}

/*
 * ioapic_get_id_last
 *
 * return the last assigned IOAPIC id.
 *
 * IMPORTANT: the difference between the last assigned id and the first assigned
 *			  id IS NOT necessarily the number of IOAPIC's. Use ioapic_get_num()
 *			  to get the number of IOAPIC's
*/
unsigned ioapic_get_id_last(void)
{
	return highest_id;	// will be zero if num_ioapics == 0
}

/*
 * ioapic_get_id_first
 *
 * return the last assigned IOAPIC id.
 *
 * IMPORTANT: the difference between the last assigned id and the first assigned
 *			  id IS NOT necessarily the number of IOAPIC's. Use ioapic_get_num()
 *			  to get the number of IOAPIC's
*/
unsigned ioapic_get_id_first(void)
{
	return (num_ioapics > 0) ? first_id : 0;
}

/*
 * ioapic_get_id_next
 *
 * return the next assigned IOAPIC id after <ioapic_id> or -1 on error
*/
int ioapic_get_id_next(unsigned ioapic_id)
{
	if (ioapic_id < first_id) ioapic_id = first_id;
	else ++ioapic_id;

	while ((ioapic_get_vaddr(ioapic_id) == NULL) && (ioapic_id <= highest_id)) {
		++ioapic_id;
	}

	return (ioapic_id > highest_id) ? -1 : ioapic_id;
}
/*
 * ioapic_get_gsibase
 *
 * return the assigned base Global System Interrupt vector for IOAPIC <ioapic_id>.
 * If -1 is returned, there was no assigned GSI base vector for that <ioapic_id>
 * and the implementation is free to assign a compatible value. Note that an
 * invalid <ioapic_id> will also return -1
*/
int ioapic_get_gsibase(unsigned ioapic_id)
{
	if ((ioapic_id > highest_id) || (ioapic_id < first_id)) return -1;
	else return ioapic_array[ioapic_id].gsi;
}

/*
 * ioapic_get_vaddr
 *
 * Return the virtual address of the IOAPIC identified by <ioapic_id> or NULL
 * on error
 *
 * Note: we do not want to call ioapic_find() here to scan for tables in case
 *		 startup does not want them to be used
*/
void *ioapic_get_vaddr(unsigned ioapic_id)
{
	if ((ioapic_id > highest_id) || (ioapic_id < first_id)) return NULL;
	else return (ioapic_array[ioapic_id].paddr == NULL_PADDR_STARTUP) ? NULL : ioapic_array[ioapic_id].vaddr;
}
/*
 * ioapic_get_paddr
 *
 * Return the physical address of the IOAPIC identified by <ioapic_id> or NULL_PADDR_STARTUP
 * on error
 *
 * Note: we do not want to call ioapic_find() here to scan for tables in case
 *		 startup does not want them to be used
*/
PADDR_T ioapic_get_paddr(unsigned ioapic_id)
{
	if ((ioapic_id > highest_id) || (ioapic_id < first_id)) return NULL_PADDR_STARTUP;
	else return ioapic_array[ioapic_id].paddr;
}

/*
 * ioapic_get_accessfnc
 * 
 * Return a pointer to the access functions associated with <ioapic_id>. If no
 * access functions are configured, NULL is returned
*/
const ioapic_accessfnc_t *ioapic_get_accessfnc(const unsigned ioapic_id)
{
	if ((ioapic_id > highest_id) || (ioapic_id < first_id)) return NULL;
	else return ioapic_array[ioapic_id].accessfncs;
}

/*
 * ioapic_set_accessfnc
 *
 * Set the access functions associated with <ioapic_id>.
 *
 * Returns: true (!0) or false (0) depending on whether the operation was
 *			successful
*/
unsigned ioapic_set_accessfnc(const unsigned ioapic_id, const ioapic_accessfnc_t * const ioapic_fnc)
{
	if ((ioapic_id > highest_id) || (ioapic_id < first_id)) return 0;
	else {
		ioapic_array[ioapic_id].accessfncs = ioapic_fnc;
		return 1;
	}
}

/*
 * ioapic_set_autofill_profile_num
 *
 * Set the autofill profile number to use for <ioapic_id>.
 *
 * Returns: true (!0) or false (0) depending on whether the operation was
 *			successful
*/
unsigned ioapic_set_autofill_profile_num(const unsigned ioapic_id, const unsigned int profile_num)
{
	if ((ioapic_id > highest_id) || (ioapic_id < first_id)) return 0;
	else {
		ioapic_array[ioapic_id].autofill_profile_num = profile_num;
		return 1;
	}
}

/*
 * ioapic_get_autofill_profile_num
 *
 * Return a pointer to the access functions associated with <ioapic_id>. If no
 * access functions are configured, NULL is returned
*/
unsigned int ioapic_get_autofill_profile_num(const unsigned ioapic_id)
{
	if ((ioapic_id > highest_id) || (ioapic_id < first_id)) return 0;
	else return ioapic_array[ioapic_id].autofill_profile_num;
}
