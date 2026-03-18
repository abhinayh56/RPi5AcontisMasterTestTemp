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






//
// structures/macros for a multiboot spec system
//

#ifndef _X86_MULTIBOOT_H_INCLUDED
#define _X86_MULTIBOOT_H_INCLUDED

#ifndef __PLATFORM_H_INCLUDED
    #include <sys/platform.h>
#endif

struct mb_info {
	_Uint32t	flags;
	_Uint32t	mem_lower;
	_Uint32t	mem_upper;
	_Uint32t	boot_device;
	_Uint32t	cmdline;
	_Uint32t	mods_count;
	_Uint32t	mods_addr;
	union {
		struct {
			_Uint32t	tabsize;
			_Uint32t	strsize;
			_Uint32t	addr;
			_Uint32t	reserved;
		}	aout;
		struct {
			_Uint32t	num;
			_Uint32t	size;
			_Uint32t	addr;
			_Uint32t	shndx;
		}	elf;
	}		syms;
	_Uint32t	mmap_length;
	_Uint32t	mmap_addr;
	_Uint32t	drives_length;
	_Uint32t	drives_addr;
	_Uint32t	config_table;
	_Uint32t	boot_loader_name;
	_Uint32t	apm_table;
	_Uint32t	vbe_control_info;
	_Uint32t	vbe_mode_info;
	_Uint32t	vbe_mode;
	_Uint32t	vbe_interface_seg;
	_Uint32t	vbe_interface_off;
	_Uint32t	vbe_interface_len;
};

#define MB_SIGNATURE	0x2badb002

#define MB_INFO_FLAG_MEM_VALID			0x00000001
#define MB_INFO_FLAG_BOOTDEV_VALID		0x00000002
#define MB_INFO_FLAG_CMDLINE_VALID		0x00000004
#define MB_INFO_FLAG_MODS_VALID			0x00000008
#define MB_INFO_FLAG_AOUT_SYMS_VALID	0x00000010
#define MB_INFO_FLAG_ELF_SYMS_VALID		0x00000020
#define MB_INFO_FLAG_MMAP_VALID			0x00000040
#define MB_INFO_FLAG_DRIVES_VALID		0x00000080
#define MB_INFO_FLAG_CONFIG_VALID		0x00000100
#define MB_INFO_FLAG_BOOTLDNAME_VALID	0x00000200
#define MB_INFO_FLAG_APM_VALID			0x00000400
#define MB_INFO_FLAG_VBE_VALID			0x00000800

struct mb_info_mmap {
	_Uint32t	size; 
	_Uint32t	base_addr_low;
	_Uint32t	base_addr_high;
	_Uint32t	length_low;
	_Uint32t	length_high;
	_Uint32t	type;
};

#define MB_INFO_MMAP_TYPE_RAM	1

struct mb_info_drive {
	_Uint32t	size;
	_Uint8t		drive_number;
	_Uint8t		drive_mode;
	_Uint16t	drive_cylinders;
	_Uint8t		drive_heads;
	_Uint8t		drive_sectors;
	_Uint16t	drive_ports[1]; //variable sized;
};

#define MB_INFO_DRIVE_MODE_CHS	'0'
#define MB_INFO_DRIVE_MODE_LBA	'1'

struct mb_info_config {
	_Uint16t	version;
	_Uint16t	cseg;
	_Uint32t	offset;
	_Uint16t	cseg_16;
	_Uint16t	dseg;
	_Uint16t	flags;
	_Uint16t	cseg_len;
	_Uint16t	cseg_16_len;
	_Uint16t	dseg_len;
};
	
#endif

/* __SRCVERSION("multiboot.h $Rev: 778261 $"); */
