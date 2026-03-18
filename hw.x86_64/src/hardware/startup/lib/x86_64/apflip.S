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






.text
	
	.extern		cpu_starting
	
	
   	// Flip the CPUID to the one indicated in cpu_starting and
	// then go back to sleep. Used to make sure the BSP is CPU 0.
	
	.globl		apflip
apflip:
	cli
    movw    $0x0008,%ax
    movw    %ax,%ds
    mov     cpu_starting(%rip),%eax
	mov 	$0xfee00020,%edx
	and		$0xf0ffffff,%edx
	shl		$24,%eax
	or		%edx,%eax
	movl	%eax,0xfee00020
	movl	$0,cpu_starting(%rip)
	hlt
