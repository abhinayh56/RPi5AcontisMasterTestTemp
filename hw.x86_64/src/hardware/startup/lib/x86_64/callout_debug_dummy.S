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




/*
;
; This is a kernel hardware callout.
; It may be called from within an interrupt handler.
; This code MUST be position independant.
;
*/

#include <x86_64/callout.ah>

/*
;
; void display_char(struct syspage_entry *sysp, char c)
;
; Display debug messages from kdebug or the kernel.
;
*/
		
CALLOUT_START(display_char_dummy, 0, 0)
    ret       
CALLOUT_END(display_char_dummy)
	
/*	
;
; int poll_key( volatile struct syspage_entry *)
;
; return a key value, or -1 if no key is available (for kdebug use)
;
*/

CALLOUT_START(poll_key_dummy, 0, 0)
    mov      $-1,%rax
    ret       
CALLOUT_END(poll_key_dummy)
	

/*
;
; int break_detect( volatile struct syspage_entry *)
;
; return non-zero if a break condition is detected, 0 otherwise
;
*/
CALLOUT_START(break_detect_dummy, 0, 0)
    xor      %rax,%rax
    ret       
CALLOUT_END(break_detect_dummy)
