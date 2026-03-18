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


#define NTRAP 256
#define SSIZE 256       /* trampoline stack size. */
#define _NGDT 32
#define NTRAP 256

#define KERN_CODE_MAX	0xFFFFFFFF
#define KERN_DATA_MAX	0xFFFFFFFF



/* size: Long or Compat */
#define _LONG        (X86_64_64BIT|X86_64_GRAN)
#define _COMPAT      (X86_64_32BIT|X86_64_GRAN)

#define _CODE (X86_64_PRESENT|\
               X86_64_SEGMENT|\
			   X86_64_EXECUTABLE|\
			   X86_64_READABLE|\
			   X86_64_ACCESSED)

#define _DATA (X86_64_PRESENT|\
               X86_64_SEGMENT|\
			   X86_64_DATA|\
			   X86_64_WRITABLE|\
			   X86_64_ACCESSED)

#define _SYS (X86_64_PRESENT | X86_64_SYSTEM)

#define _CODESEG(P)  (_CODE | _LONG   | X86_64_DPL(P))
#define _C32SEG(P)   (_CODE | _COMPAT | X86_64_DPL(P))
#define _DATASEG(P)  (_DATA | _COMPAT | X86_64_DPL(P))

#define _TSSDESC     (_SYS | X86_64_SYSTYPE(X86_64_TSS64))
#define _LDTDESC     (_SYS | X86_64_SYSTYPE(X86_64_LDT))
#define _INTDESC     (_SYS | X86_64_SYSTYPE(X86_64_INTR_GATE))



typedef union gdt {
	uint32_t         w[2];
	uint16_t         h[4];
} seg_t;

typedef union gate {
	uint32_t       w[4];
	uint16_t       h[8];
} gate_t;


paddr_t		gdt_paddr;
unsigned    gdt_size;
paddr_t     idt_paddr;
unsigned    idt_size;


/*- 
 Although intel lists 10 priorities of exceptions, there really are fewer
 sources to concern our selves with:
     machine_check         ERR_STACK
	 data breakpoint       DEBUG_STACK
	 nmi                   NMI_STACK
	 breakpoint            DEBUG_STACK
 Because the early and late portions of kernel entry where the stack is
 vulnerable, has interrupts disabled, and should never suffer from other
 faults.   The above ones are out of its control.
*/
static const char trapstack[]= {
	[1]  = X86_64_DEBUG_STACK,     /* DEBUG */
	[3]  = X86_64_DEBUG_STACK,     /* DEBUG */

	[2]  = X86_64_NMI_STACK,       /* NMI */

	[0]  = X86_64_TRAP_STACK,      /* divide by zero */
	[4]  = X86_64_TRAP_STACK,		/* overflow */
	[5]  = X86_64_TRAP_STACK,      /* bound */
	[6]  = X86_64_TRAP_STACK,      /* invalid opcode */ 
	[7]  = X86_64_TRAP_STACK,      /* device not avail */ 
	[8]  = X86_64_TRAP_STACK,      /* nested fault */ 
	[13] = X86_64_TRAP_STACK,      /* general protection */ 
	[14] = X86_64_TRAP_STACK,      /* page fault */ 
	[16] = X86_64_TRAP_STACK,      /* FP error */ 
	[17] = X86_64_TRAP_STACK,      /* alignment */ 
	[19] = X86_64_TRAP_STACK,      /* SIMD FP ERR */ 

	[9]  = X86_64_ERR_STACK,       /* copro overrun - deprecated */ 
	[10] = X86_64_ERR_STACK,       /* invalid tss */ 
	[11] = X86_64_ERR_STACK,       /* segment not present */ 
	[12] = X86_64_ERR_STACK,       /* stack fault */ 
	[15] = X86_64_ERR_STACK,       /* invalid exception */
	[18] = X86_64_ERR_STACK,       /* Machine Check*/ 
	[20] = X86_64_ERR_STACK,      /* reserved */ 
	[21] = X86_64_ERR_STACK,      /* reserved */ 
	[22] = X86_64_ERR_STACK,      /* reserved */ 
	[23] = X86_64_ERR_STACK,      /* reserved */ 
	[24] = X86_64_ERR_STACK,      /* reserved */ 
	[25] = X86_64_ERR_STACK,      /* reserved */ 
	[26] = X86_64_ERR_STACK,      /* reserved */ 
	[27] = X86_64_ERR_STACK,      /* reserved */ 
	[28] = X86_64_ERR_STACK,      /* reserved */ 
	[29] = X86_64_ERR_STACK,      /* reserved */ 
	[30] = X86_64_ERR_STACK,      /* reserved */ 
	[31] = X86_64_ERR_STACK      /* reserved */ 
};

#define trap_stack(x)  ((x) < NUM_ELTS(trapstack) ? trapstack[x] : X86_64_TRAP_STACK)


static void
mkseg(uint32_t *g, uint32_t type)
{
	g[0] = 0xffff;
	g[1] = 0xf0000|type;
}

static void
mkdesc(uint32_t *g, uint32_t type, uintptr_t r, size_t sz)
{
	sz -= 1; /* to become a lim */
	g[0] =   ((r&0xffff) << 16) | (uint16_t)sz;
	g[1] =   (r & 0xff000000) | (sz&0xf0000) | type | ((r>>16) & 0xff);
	g[2] =   (r>>32);
	g[3] =   0x0;
}

static void
mkgate(gate_t *g, uint32_t type, uint64_t f, unsigned stk)
{
	g->w[0] = (uint16_t)(f & 0xffff) | (X86_64_KER_CS << 16);
	g->w[1] = ((uint32_t)f&0xffff0000) |type | (stk&0xff);
	g->w[2] = (uint32_t)(f>>32);
	g->w[3] = 0;
}

static void
cpuinit_gdt(seg_t *sysgdt, unsigned nseg)
{
	memset(sysgdt, 0, nseg*sizeof(*sysgdt));

	mkdesc(sysgdt[X86_64_LDT_SEL/8].w, _LDTDESC,
		(uintptr_t)sysgdt, nseg*sizeof(*sysgdt));
	mkseg(sysgdt[X86_64_KER_CS/8].w, _CODESEG(0));
	mkseg(sysgdt[X86_64_SYS_CS/8].w, _CODESEG(1));
	mkseg(sysgdt[X86_64_USR_CS/8].w, _CODESEG(3));

/*-
  The amd sysref says data seg priv is ignored;
  Bochs + VMW both insist upon it.
*/
	mkseg(sysgdt[X86_64_KER_SS/8].w, _DATASEG(0));
	mkseg(sysgdt[X86_64_SYS_SS/8].w, _DATASEG(1));
	mkseg(sysgdt[X86_64_USR_SS/8].w, _DATASEG(3));
}

static void
cpuinit_idt(gate_t *idt, unsigned ntrap)
{
	unsigned        i;
	for (i=0; i < ntrap; i++) {
		mkgate(idt+i, _INTDESC, 0, trap_stack(i));
	}
}

void
init_gdt(unsigned idt_size)
{
	cpuinit_gdt((seg_t *)TOPTR(gdt_paddr), gdt_size/sizeof(seg_t));
	cpuinit_idt((gate_t *)TOPTR(idt_paddr), idt_size/sizeof(gate_t));
}
