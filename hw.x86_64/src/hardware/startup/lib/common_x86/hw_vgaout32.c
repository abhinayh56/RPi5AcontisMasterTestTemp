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

#define MONBUF			((uintptr_t)0xb0000)
#define COLBUF			((uintptr_t)0xb8000)

#define BIOS_COL		(*(uint8_t *)0x450)
#define BIOS_ROW		(*(uint8_t *)0x451)
#define BIOS_COLUMNS	(*(uint16_t *)0x44a)

void
init_vga(unsigned channel, const char *init, const char *defaults) {
	//Nothing happening
}

static void
scroll(unsigned char *dp, unsigned ncols) {
	memmove(dp, dp + 2*ncols, 2*24*ncols);
	dp += 24*2*ncols;
	while(ncols != 0) {
		*dp++ = 0x20;
		*dp++ = 0x07;
		--ncols;
	}
}

void
put_vga(int c) {
	unsigned char 	*dp;
	unsigned 		row, col, ncols, nrows;
	unsigned		off;

	col = BIOS_COL;
	row = BIOS_ROW;

	ncols = BIOS_COLUMNS;
	if(ncols != 40  &&  ncols != 80) ncols = 80;
	nrows = 25;

	switch(c) {
	case '\n':
		++row;
		break;
	case '\r':
		col = 0;
		break;
	case 0x0c:
		row = col = 0;
		break;
	default:
		off = (row * ncols + col) * 2;
		dp = (unsigned char *) (MONBUF + off);
		dp[0] = c;
		dp[1] = 0x0f;
		dp = (unsigned char *) (COLBUF + off);
		dp[0] = c;
		dp[1] = 0x0f;
		++col;
		break;
	}

	if (col >= ncols) {
		++row;
		col = 0;
	}

	if(row >= nrows) {
		--row;
		scroll((unsigned char *)MONBUF, ncols);
		scroll((unsigned char *)COLBUF, ncols);
	}

	BIOS_COL = col;
	BIOS_ROW = row;
}		
