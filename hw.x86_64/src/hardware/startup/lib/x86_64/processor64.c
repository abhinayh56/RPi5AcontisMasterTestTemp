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

/*-
 * PGDEBUG_ENABLED controls printing progress messages as ptes are created.
 * DEBUG_PAGETABLES controls dumping the constructed mappings when they are
 *                  completed.
 */

#define ANY_VADDR   (~0L)

typedef uint64_t	Pte_t;

#define NPTE_PER_TAB   512L
#define _1GPAGE   (1L<<30)
#define _2MPAGE   (1L<<21)
#define _4KPAGE   (1L<<12)


#define PAGE_DIR_LEVELS   4
static int pscale[PAGE_DIR_LEVELS] = {
	39,
	30,
	21,
	12
};



paddr_t   page_tables;
static Pte_t  page_dir;

#define PGCONTROL_BITS ((1UL<<63)|(0xfffUL))

#define is_big(p)	(((p) & X86_64_PTE_PS) != 0)
#define is_dir(p)	(!is_big(p))
#define is_mapped(p)	(((p) & X86_64_PTE_PRESENT) != 0)

static uintptr_t	free_addr = FREE_VADDR;


/* extract the base physical address from the pte */
static Pte_t *
pte_pa(Pte_t pte)
{
	pte &= ~PGCONTROL_BITS;
	return (Pte_t *)pte;
}

/* return the size of the virtual space mapped at this depth */
static uintptr_t
pdepth_size(unsigned depth)
{
	return 1L<<pscale[depth];
}

/*
 * Break out the index elements from a va to make a path.
 */
static void
va_path(uintptr_t p, uint32_t *path)
{
	unsigned i;
	for (i=0; i < PAGE_DIR_LEVELS; i++) {
		path[i] = (p >> pscale[i]) & (NPTE_PER_TAB-1);
	}
}


/*-
 Return a pointer to the Pte_t closest to mapping this address.
 Store, in *size, the virtual space covered by this level of Pte.
 The pointer is always valid; but the Pte_t it points to isn't necessarily.

*/
static Pte_t *
getptep(uintptr_t vaddr, paddr_t *size)
{
	unsigned depth;
	Pte_t  *pde = &page_dir;
	uint32_t   index[4];

	va_path(vaddr, index);

	depth = 0;
	pde = pte_pa(*pde) + index[depth];
	if (is_mapped(*pde)) { /* no 0.5T pages... yet? */
		pde = pte_pa(*pde) + index[++depth];
		if (is_mapped(*pde) && is_dir(*pde)) {
			pde = pte_pa(*pde) + index[++depth];
			if (is_mapped(*pde) && is_dir(*pde)) {
				pde = pte_pa(*pde) + index[++depth];
			}
		}
	}
	*size = pdepth_size(depth);
	return pde;
}

/*-
 * map pages handles various page sizes, and deleting regions.
 */
static uintptr_t
_map_pages(uintptr_t vaddr, paddr_t paddr, size_t size, int flags,
	size_t pgsize)
{
	int64_t		v;

	if (vaddr == ANY_VADDR) {
		/* don't care where the memory is, use next free vaddr */
		vaddr = free_addr;
		free_addr += size;
	}

	if ((vaddr|paddr|size) & (pgsize-1)) {
		kprintf("%s: unaligned mapping: V=%v, P=%P, Size=%v cannot map %v\n",
			__FUNCTION__, vaddr, paddr, (uintptr_t)size, (uintptr_t)pgsize);
		crash("Cannot continue\n");
	}

	if(meltdown_active <= 0) {
		flags |= X86_64_PTE_GLOBAL;
	}
	for (v = 0; v < size; v += pgsize) {
		Pte_t	*pde;
		size_t psize = pgsize;
		do {
			pde = getptep(vaddr+v, &psize);
			if (is_mapped(*pde)) {
				if (is_mapped(flags) && ((paddr_t)pte_pa(*pde) != (paddr + v))) {
					kprintf("%s: overlapped mapping at %v, pte = %L\n",
						__FUNCTION__, vaddr+v, *pde);
					kprintf("  was mapping [%v..%v] -> [%P..%P]\n",
						vaddr+v, vaddr+v-1+psize, paddr+v, paddr+v-1+psize);
					crash("Cannot continue\n");
				} else {
					break; /* Its already invalid or a remap*/
				}
			}
			if (psize < pgsize) {
				kprintf("%s: vaddr %v %v-size pages; size %v table found\n",
					__FUNCTION__, vaddr+v, (uintptr_t)pgsize, (uintptr_t)psize);
				kprintf("  was mapping [%v..%v] -> [%P..%P]\n",
					vaddr, vaddr+v-1, paddr, paddr+v-1);
				crash("Cannot continue\n");
			}
			if (psize > pgsize) {
				*pde = calloc_ram(__PAGESIZE, __PAGESIZE)
						| (X86_64_PTE_WRITE|X86_64_PTE_USER|X86_64_PTE_PRESENT);
			}
		} while (psize > pgsize);
		*pde = (paddr+v) | (size_t)flags;
	}
	return vaddr;
}

/*
 * one for each supported page size
 */
static uintptr_t
_map4k(uintptr_t vaddr, paddr_t paddr, size_t size, int flags)
{
	flags &= ~X86_64_PTE_PS;
	return _map_pages(vaddr, paddr, size, flags, _4KPAGE);
}

static uintptr_t
_map2m(uintptr_t vaddr, paddr_t paddr, size_t size, int flags)
{
	flags |= X86_64_PTE_PS;
	return _map_pages(vaddr, paddr, size, flags, _2MPAGE);
}

#if 0 // not used right now
static uintptr_t
_map1g(uintptr_t vaddr, paddr_t paddr, size_t size, int flags)
{
	flags |= X86_64_PTE_PS;
	return _map_pages(vaddr, paddr, size, flags, _1GPAGE);
}
#endif


/*
  amd64: 63.........................0
			                 |--11..0     offset
			            |---20..12        level0
			        |---29..21            level1
			   |----38..30                level2
		   |---47..39                     level3


Below, we allocate a single level3 table (page_tables+__PAGESIZE * 5),
which we point the first entry of at page_tables+__PAGESIZE*4.
At page_tables+__PAGESIZE*4 (level2), we put in 4 entries, which point to
page_tables+0
page_tables+__PAGESIZE
page_tables+__PAGESIZE*2
page_tables+__PAGESIZE*3

*/


void
x86_64_init_pgdir()
{
	unsigned		num;
	paddr_t		paddr;
	unsigned		i;
	unsigned		pd_size;
	uintptr_t   pgdir;
	Pte_t      *pdir;

	num = lsp.syspage.p->num_cpu;
	set_syspage_section(&lsp.cpu.pgdir, sizeof(paddr_t) * num);
	pd_size = __PAGESIZE;
	page_tables = paddr = calloc_ram(pd_size, pd_size);
	page_dir = page_tables;
	pdir = (Pte_t *)paddr;
	pdir[NPTE_PER_TAB-1] = (uintptr_t)(pdir)
								| (X86_64_PTE_WRITE|X86_64_PTE_PRESENT);
	pgdir = x86_64_map(ANY_VADDR, paddr, pd_size,
				 X86_64_PTE_PRESENT | X86_64_PTE_WRITE);

	/* All cpus share initial page table. */
	for (i = 0; i < num; ++i) {
		lsp.cpu.pgdir.p[i] = pgdir;
	}
}

void
x86_64_load_pgdir(unsigned cpu) {
	paddr_t		pg_tbl;

	pg_tbl = (paddr_t)page_dir;
	ldpgdir(pg_tbl);
}


uintptr_t
x86_64_map(uintptr_t vaddr, paddr_t paddr, size_t size, int flags)
{
	unsigned const 	off = paddr & (paddr_t)0xfff;

	size = ROUNDPG(size + off);
	return _map4k(vaddr, paddr - off, size, flags) + off;
}


uintptr_t
x86_64_4m_map(uintptr_t vaddr, paddr_t paddr, size_t size, int flags)
{
	uintptr_t  p;
	p =  _map2m(vaddr, paddr, size, flags);
	return p;
}



paddr_t
elf_vaddr_to_paddr(uintptr_t vaddr)
{
	paddr_t	paddr;
	paddr_t	size;
	Pte_t	*pte;

	size = 0;
	pte = getptep(vaddr, &size);
	if (pte == NULL) {
		kprintf("no pte for %v\n", (vaddr));
		return -1LL;
	}
	paddr = *pte & ~(size-1);
	paddr += vaddr & (size-1);
	return paddr;
}

#ifdef __SRCVERSION

#endif
