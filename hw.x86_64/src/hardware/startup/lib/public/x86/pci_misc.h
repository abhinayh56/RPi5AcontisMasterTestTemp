/*
 * $QNXLicenseC: 
 * Copyright 2010, QNX Software Systems.  
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

#ifndef PCI_MISC_H_
#define PCI_MISC_H_

typedef struct
{
	unsigned bus;
	unsigned dev;
	unsigned func;
} pci_dev_tbl_t;

extern int pci_find_class(int cookie, unsigned class, unsigned subclass, int *bus, unsigned *devfunc);
extern int get_LPC_pci_devfunc(void);
extern void set_LPC_pci_devfunc(unsigned);
extern unsigned get_LPC_devid(void);
extern uint8_t get_lpc_reg8(unsigned offset);
extern uint16_t get_lpc_reg16(unsigned offset);
extern uint32_t get_lpc_reg32(unsigned offset);
extern void set_lpc_reg8(unsigned offset, uint8_t val);
extern void set_lpc_reg16(unsigned offset, uint16_t val);
extern void set_lpc_reg32(unsigned offset, uint32_t val);
extern void add_known_pci_device(int bus, int device, int function);

#define GET_LPC_REG8(o)			get_lpc_reg8((o))
#define GET_LPC_REG16(o)		get_lpc_reg16((o))
#define GET_LPC_REG32(o)		get_lpc_reg32((o))
#define SET_LPC_REG8(o, v)		set_lpc_reg8((o), (v))
#define SET_LPC_REG16(o, v)		set_lpc_reg16((o), (v))
#define SET_LPC_REG32(o, v)		set_lpc_reg32((o), (v))

#define	PCI_DEVNO(devfunc)		((devfunc) >> 3)
#define	PCI_FUNCNO(devfunc)		((devfunc) & 7)
#define	PCI_DEVFUNC(dev,func)	(((dev)<<3)|(func))

#define SUB_VENDOR_ID_OFFSET 0x2c    //offsetof(struct _pci_config_regs, Sub_Vendor_ID) 
#define SUB_SYSTEM_ID_OFFSET 0x2e    //offsetof(struct _pci_config_regs, Sub_System_ID) 


#endif /* PCI_MISC_H_ */
