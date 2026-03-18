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

#include "asmoff.def"
#include <x86_64/asm.h>


#define X86_64_XCPUID_LM (1<<29)

/* page sizes: */
#define PGSIZE_1G  (1<<30)
#define PGSIZE_2M  (1<<21)
#define PGSIZE_4K  (1<<12)

/* 1G pages are optional; only defined when  */
#define MAX_PAGE_SIZE  PGSIZE_2M



#define PAGEDIR_PERMS      (\
	X86_64_PTE_PRESENT |\
	X86_64_PTE_WRITE |\
	X86_64_PTE_USER \
)

#define BIGPAGE_PERMS (\
	X86_64_PTE_PRESENT |\
	X86_64_PTE_WRITE |\
	X86_64_PTE_USER |\
	X86_64_PTE_PS \
)

#define MEDPAGE_PERMS  BIGPAGE_PERMS

/*-
	these are for understanding; the pagefill code relies implicitly on
	their inter-relationship.
*/
#define PAGESIZE   4096
#define PTESIZE    8
#define NGIG       4
#define NDIRENT    (PAGESIZE/PTESIZE)
.text


.code32

STARTFUNC(fillpage) /*edx:eax = 64bit addr, edi = pageptr, ecx = incr */
next_pte:
	mov     %eax, 0(%edi)
	movl    %edx, 4(%edi)
	add		%ecx, %eax
	adc		$0, %edx
	add		$PTESIZE, %edi

	test	$PAGESIZE-1, %edi
	jnz		next_pte	
	ret
ENDFUNC(fillpage)

STARTFUNC(fillpages)
	push	%esi
	push	%edi
	xor 	%esi,%esi
	

/* PAGESIZE aligned pagemem */
	lea     pagemem, %edi
/* this is our cr3 value, and base page table; save it. */
	mov		%edi, %esi

/* set level 4 slot 0 to our level 3 page table */
	lea		PAGESIZE(%edi), %eax
	or		$PAGEDIR_PERMS, %eax
	mov		%eax, 0(%edi)
	lea		PAGESIZE(%edi), %edi

	mov		$pagemem_end - pagemem, %eax
#if MAX_PAGE_SIZE == PGSIZE_1G
	cmp		$2*PAGESIZE, %eax
	jb		not_enough_pagemem

/* fill level 3 with 1G pages starting at 0 */
	mov		$BIGPAGE_PERMS, %eax
	xor		%edx, %edx
	mov		$(1<<30), %ecx
	call	fillpage

/* zero out entries after first 4 in previous pagedir */
	lea		(-PAGESIZE+4*PTESIZE)(%edi), %edi
	xor		%eax, %eax
	xor		%edx, %edx
	xor		%ecx, %ecx
	call	fillpage
#elif MAX_PAGE_SIZE == PGSIZE_2M
	cmp		$6*PAGESIZE, %eax
	jb		not_enough_pagemem

	lea		PAGESIZE(%edi), %eax
	or		$PAGEDIR_PERMS, %eax
	xor		%edx, %edx
	mov		$PAGESIZE, %ecx
	call	fillpage

/* zero out entries after first 4 in previous pagedir */
	lea		(-PAGESIZE+4*PTESIZE)(%edi), %edi
	xor		%eax, %eax
	xor		%edx, %edx
	xor		%ecx, %ecx
	call	fillpage

	xor		%edx, %edx
	mov		$MEDPAGE_PERMS, %eax
	mov		$PGSIZE_2M, %ecx

	call	fillpage  /* 0x0000000000..0x40000000 */
	call	fillpage  /* 0x0040000000..0x7fffffff */
	call	fillpage  /* 0x0080000000..0xbfffffff */
	call	fillpage  /* 0x00c0000000..0xffffffff */


#else
	cmp		$35*PAGESIZE, %eax	/* 3+32*4k = 140k (64m mapped) */
	jb		not_enough_pagemem

#error "need a new startup to handle small only pages"
#error "to cover 4g linear, requires 8m of page tables"
#error "so startup either needs to be sparse, or manage that"

#endif

/* return the root dir */
	mov		%esi, %eax

	pop		%edi
	pop		%esi
	ret

not_enough_pagemem:
	movl	$-1, %eax
	pop		%edi
	pop		%esi
	ret

ENDFUNC(fillpages)

STARTFUNC(entry_32prot)
	pusha
// start msr_init
	/* EFER.LME = 1 */
	mov	$X86_64_MSR_EFER, %ecx
	rdmsr
	or	$X86_64_EFER_LME, %eax
/*-
	* Can't use NX bits, until setup is complete.  We have intermingled code
	* and data here.
	*/
	andl	$~X86_64_EFER_NXE, %eax
	wrmsr

	mov		%cr4, %eax
	or		$(X86_64_CR4_PAE), %eax
	mov		%eax, %cr4
// end msr_init

	call	fillpages
	// page root returned in EAX

// go big
#define CR0BITS (X86_64_CR0_PG_BIT | X86_64_CR0_PE_BIT | 0)
	mov		%cr0,%edx
	and		$~X86_64_CR0_PG_BIT, %edx
	mov		%edx, %cr0
	jmp		pager_disabled
pager_disabled:
	mov		%eax, %cr3		/* our page table */
	mov		%cr0, %eax
	or		$CR0BITS, %eax
	mov		%eax, %cr0
	jmp		compat_mode_start
compat_mode_start:
	/* load our new gdt */
	lgdt	gdtload

	popa
	ljmp	$8, $long_mode_start

.code64
long_mode_start:
	mov		%rax,save_rax
	xor		%rax, %rax
	mov		%ax, %ss
	mov		%ax, %ds
	mov		%ax, %es

#if 0
/*
first lets do a small sanity checkon the page tables: check the
PHYS mappings; hopefully, we don't step on any io devices...
 */
	mov    $0x2000, %rax
	shl    $32, %rax
	or     $0x06000000, %rax
	movq   (%rax), %r11
	add    $0x40000000, %rax
	movq   (%rax), %r11
	add    $0x40000000, %rax
	movq   (%rax), %r11
	add    $0x40000000, %rax
	movq   (%rax), %r11
/* ok, we should live. */
#endif	

jmptoproc:
	mov		save_rax,%rax
	jmp		_start

ENDFUNC(entry_32prot)

save_rax:	.quad	0

.p2align 3
gdtload:
	.word   gdt64end - gdt64
	.long   gdt64       # low
	.long   0           # high

.p2align 3
gdt64:
	.long   0           # null descriptor
	.long   0
	.long   0x00000000      # %cs
	.long   0x00209800
	.long   0x00000000      # %ds
	.long   0x00008000
gdt64end:

.align PAGESIZE
pagemem:
#if MAX_PAGE_SIZE == PGSIZE_1G
/* 1 for pml4, 1 for pml3 [ 512 1G pte ] */
	.zero   2*PAGESIZE
#elif MAX_PAGE_SIZE == PGSIZE_2M
/* 1 for pml4, 1 for pml3, 4 for pml2 [ 2048 2M pte ] */
	.zero   6*PAGESIZE
#else
#error "must cofigure MAX_PAGE_SIZE for initial map"
#endif
pagemem_end:
	.quad	0
