/*
 * $QNXLicenseC:
 * Copyright 2007, 2008,2025, BlackBerry Limited.
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
	Create CPU ID string
*/

#include <startup.h>

struct type {
	char type;
	char *desc;
} __attribute__((packed));

struct desc {
	char family;
	char model;
	char *part;
} __attribute__((packed));

struct ider {
	char		id[CPU_VENDOR_ID_SIZE];
	char		*man;
	struct desc *list;
} __attribute__((packed));

static struct desc intel[] = {
	{6,		6,		"Pentium Celeron"},
	{6,		7,		"Pentium III"},
	{6,		8,		"Pentium III"},
	{6,		9,		"Pentium/Celeron M"},
	{6,		10,		"Pentium III Xeon"},
	{6,		11,		"Pentium III"},
	{6,		13,		"Pentium/Celeron M"},
	{6,		14,		"Core Solo/Duo"},
	{6,		15,		"Core 2/Xeon"},
	{6,		22,		"Celeron"},
	{6,		23,		"Core 2 Extreme/Xeon"},
	{6,		26,		"Core i7"},	/* Bloomfield */
	{6,		27,		"Xeon MP"},
	{6,		28,		"Atom"},
	{6,		37,		"Core i7/5/3"},	/* Westmere */
	{6,		38,		"E6xx"},
	{6,		42,		"Core i7/5/3"},	/* SandyBridge */
	{6,		45,		"Core i7/5/3"},	/* SandyBridge */
	{6,		58,		"Core i7/5/3"},	/* IvyBridge */
	{6,		60,		"Core i7/5/3"},	/* Haswell */
	{6,		61,		"Core M"},	/* Broadwell */
	{6,		62,		"Core i7"},	/* IvyBridge */
	{6,		63,		"Core i7"},	/* Haswell */
	{6,		69,		"Core i7/5/3"},	/* Haswell */
	{6,		70,		"Core i7/5"},	/* Haswell */
	{6,		71,		"Core M"},	/* Broadwell */
	{6,		78,		"Core i7/5/3"},	/* Skylake */
	{6,		94,		"Core i7/5/3"},	/* Skylake */
	{15,	0,		"Pentium 4"},
	{15,	1,		"Pentium 4"},
	{15,	2,		"Pentium 4"},
	{15,	3,		"Pentium 4"},
	{15,	4,		"Pentium 4"},
	{15,	6,		"Pentium 4"},
	{0,		0,		0}
};

static struct desc amd[] = {
	{4,		3,		"AMD486DX2WT"},
	{4,		7,		"AMD486DX2WB"},
	{4,		8,		"AMD486DX4"},
	{4,		9,		"AMD486DX4WB"},
	{5,		0,		"AMD-K5"},
	{5,		1,		"AMD-K5"},
	{5,		2,		"AMD-K5"},
	{5,		3,		"AMD-K5"},
	{5,		6,		"AMD-K6"},
	{5,		7,		"AMD-K6"},
	{5,		8,		"AMD-K6-2"},
	{5,		9,		"AMD-K6-III"},
	{0x17,	0x71,	"AMD-Zen2-Ryzen-3xxx"},
	{0,		0,		0}
};

static struct desc umc[] = {
	{4,		1,		"U5D"},
	{4,		2,		"U5S"},
	{0,		0,		0}
};

static struct desc cyrix[] = {
	{5,		2,		"6x86"},
	{6,		0,		"6x86MX"},
	{0,		0,		0}
};

static struct desc idt[] = {
	{5,		4,		"WinChip C6"},
	{0,		0,		0}
};

static struct ider ids[] = {
	{"GenuineIntel",  "Intel", 	intel},
	{"UMC UMC UMC ",  "UMC",	umc},
	{"AuthenticAMD",  "AMD",	amd},
	{"CyrixInstead",  "Cyrix",	cyrix},
	{"CentaurHauls",  "IDT",	idt},
	{{0}, 0, 0}
};

static struct type types[] = {
	{0,		""},
	{0,		"OD"},
	{0,		"Dual"},
	{0,		0}
};

static char *
stradd(char *dst, char *src) {
	while ((*dst++ = *src++) != 0)
	{};
	return dst-1;
}

static char *
decadd(char *dst, int v) {
	int is_3_digit = 0;			// check if the value is a three-digit-number

	if (v >= 100) {
		*dst++ = '0' + (v / 100);
		v %= 100;
		is_3_digit = 1;			// the value is a three-digit-number
	}

	if (v >= 10) {
		*dst++ = '0' + (v / 10);
		v %= 10;
	}
	else {
		if (is_3_digit != 0) {	// set the second digit to 0
			*dst++ = '0';
		}
	}

	*dst++ = v + '0';
	*dst = 0;
	return dst;
}

static char *
hexadd(char *dst, unsigned short v) {
	register int		bit;

	for(bit = 12; bit >= 0; bit -= 4) {
		register char		c = ((v >> bit) & 0x0f) + '0';

		*dst++ = c > '9' ? c + ('A' - '9' - 1) : c;
	}
	*dst = 0;
	return dst;
}


int
x86_64_cpuid_string (char *buf, int max) {
	union {
		uint32_t    regs[3];
		char		id[CPU_VENDOR_ID_SIZE];
	}  idregs;
	char			*t, *man, *part;
	struct desc		*list;
	struct ider		*idp;
	struct type		*typ;
	unsigned		max_id;
	int				type, family, model, step, c;

	if ((max_id = x86_64_cpuid(0, idregs.regs)) < 1) {
		return -1;
	}

	family = cpu_family();
	model = cpu_model();
	c = x86_64_cpuid(1, 0);
	step   =  c & 0xf;
	type = (c >> 12) & 0x3;

	idregs.id[CPU_VENDOR_ID_SIZE - 1] = 0;
	man = idregs.id;
	part = 0;
	list = 0;
	t = 0;
	for (idp = ids; idp->man; idp++) {
		if (memcmp(idregs.id, idp->id, sizeof idp->id) == 0) {
			man  = idp->man;
			list = idp->list;
			break;
		}
	}

	for (typ = types; typ->desc; typ++) {
		if (typ->type == type) {
			t = typ->desc;
			break;
		}
	}
	while (list && list->family) {
		if (list->family == family && list->model == model) {
			part = list->part;
			break;
		}
		list++;
	}
	if (part)
		buf = stradd(buf, part);
	else {
		buf = stradd(buf, man);
		*buf++ = ' ';
		*buf++ = family + '0';
		buf = stradd(buf, "86");
	}
	if (t && *t) {
		*buf++ = '-';
		buf = stradd(buf, t);
	}
	if (part) {
		buf = stradd(buf, " Stepping ");
		buf = decadd(buf, step);
	} else {
		buf = stradd(buf, " F");
		buf = decadd(buf, family);
		*buf++ = 'M';
		buf = decadd(buf, model);
		*buf++ = 'S';
		buf = decadd(buf, step);
	}
	if (idp->list == intel) {
		if((cpu_features & X86_64_FEATURE_SN) && max_id >= 3) {
			struct sernum {
				uint32_t reserved;
				uint32_t middle;
				uint32_t lower;
			}					ser = {.reserved = 0, .middle = 0, .lower = 0};

			x86_64_cpuid(3, &ser.reserved);
			*buf++ = ' ';
			buf = hexadd(buf, c >> 16);
			*buf++ = '-';
			buf = hexadd(buf, c);
			*buf++ = '-';
			buf = hexadd(buf, ser.middle >> 16);
			*buf++ = '-';
			buf = hexadd(buf, ser.middle);
			*buf++ = '-';
			buf = hexadd(buf, ser.lower >> 16);
			*buf++ = '-';
			buf = hexadd(buf, ser.lower);
		}
		if((cpu_features & X86_64_FEATURE_SEP) && (family == 6) && ((model == 1) || ((model < 3) && (step < 3)))) {
			// Broken Pentium Pro or Pentium 2. Disable feature bit
			// and make sure the CS MSR is set to zero so apps fault if
			// they try to use it.
			cpu_features &= ~X86_64_FEATURE_SEP;
			wrmsr(0x174, 0);
		}
	}
	return 0;
}


// Define to make it easier to use shared code
int
x86_cpuid_string (char *buf, int max) {
	return x86_64_cpuid_string(buf, max);
}
