/*
 * $QNXLicenseC:
 * Copyright 2007, 2008, QNX Software Systems.
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

// Module Description:  board specific interface

#include <pci/pci_id.h>
#include <sdhci_pci.h>


sdio_vendor_t	sdio_vendors[] = {
	{ .vid = SDIO_VENDOR_ID_WILDCARD, .name = "Generic", .chipsets = sdio_sdhci_product },
	{ .vid = PCI_VID_Intel_Corp, .name = "Intel", .chipsets = sdio_sdhci_intel_products },
	{ .vid = PCI_VID_O2Micro__Inc, .name = "O2 Micro", .chipsets = sdio_sdhci_o2_products },
	{ .vid = PCI_VID_Silicon_Image__Inc, .name = "CMD", .chipsets = sdio_sdhci_cmd_products },
    { .vid = 0, .name = NULL, .chipsets = NULL }
};
