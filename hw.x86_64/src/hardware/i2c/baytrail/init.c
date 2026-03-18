/*
 * Copyright (c) 2015, 2022, BlackBerry Limited.
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
 */


#include <pci/pci.h>
#include <pci/cap_pcie.h>
#include <pci/pci_ccode.h>
#include <pci/cap_msi.h>
#include <pci/pci_id.h>
#include <sys/slog.h>
#include <sys/slogcodes.h>
#include "proto.h"

#define ARRAY_SIZE(a)   (sizeof(a) / sizeof((a)[0]))

#define BUSY_RETRY (3)

uint32_t i2c_reg_read32(baytrail_dev_t* const dev, const uint32_t offset)
{
    return *(volatile uint32_t *)(dev->vbase + offset);
}

void i2c_reg_write32(baytrail_dev_t* const dev, const uint32_t offset, const uint32_t value)
{
    *(volatile uint32_t *)(dev->vbase + offset) = value;
}

int baytrail_i2c_enable(baytrail_dev_t* const dev, const unsigned enable)
{
    const pci_bdf_t bdf = pci_bdf(dev->pci);
    int timeout = 1000;

    do {
        i2c_reg_write32(dev, DW_IC_ENABLE, enable);
        if ((i2c_reg_read32(dev, DW_IC_ENABLE_STATUS) & DW_IC_ENABLE_STATUS_ENABLE_MASK) == enable) {
            return 0;
        }

        /*
         * Wait 10 times the signaling period of the highest I2C transfer
         * supported by the driver (for 400KHz this is 25us).
         */
        usleep(25);
    } while (timeout--);

    logerr(bdf, "%s: timeout in %sabling I2C adapter\n", __FUNCTION__, enable ? "en" : "dis");
    return -ETIMEDOUT;
}

struct did_clock_info {
    pci_did_t     device_ID;
    unsigned long ic_clk_value;
};

/*
    The Broxton does not have a DW_IC_CLOCK_PARAMS register.
    The Broxton IC_CLK is 133 MHz.
*/
static
const struct did_clock_info did_clock_khz[] =
{
    { .device_ID = 0x5AAC, .ic_clk_value = DW_IC_CLOCK_PARAMS_133MHZ }, // I2C 0  B0:D22:F0  Broxton Apollo Lake
    { .device_ID = 0x5AAE, .ic_clk_value = DW_IC_CLOCK_PARAMS_133MHZ }, // I2C 1  B0:D22:F1
    { .device_ID = 0x5AB0, .ic_clk_value = DW_IC_CLOCK_PARAMS_133MHZ }, // I2C 2  B0:D22:F2
    { .device_ID = 0x5AB2, .ic_clk_value = DW_IC_CLOCK_PARAMS_133MHZ }, // I2C 3  B0:D22:F3
    { .device_ID = 0x5AB4, .ic_clk_value = DW_IC_CLOCK_PARAMS_133MHZ }, // I2C 4  B0:D23:F0
    { .device_ID = 0x5AB6, .ic_clk_value = DW_IC_CLOCK_PARAMS_133MHZ }, // I2C 5  B0:D23:F1
    { .device_ID = 0x5AB8, .ic_clk_value = DW_IC_CLOCK_PARAMS_133MHZ }, // I2C 6  B0:D23:F2
    { .device_ID = 0x5ABA, .ic_clk_value = DW_IC_CLOCK_PARAMS_133MHZ }, // I2C 7  B0:D23:F3

    { .device_ID = 0x4BB9, .ic_clk_value = DW_IC_CLOCK_PARAMS_100MHZ }, // I2C 0  B0:D27:F0 Elkhart Lake PSE I2C devices
    { .device_ID = 0x4BBA, .ic_clk_value = DW_IC_CLOCK_PARAMS_100MHZ }, // I2C 1  B0:D27:F1
    { .device_ID = 0x4BBB, .ic_clk_value = DW_IC_CLOCK_PARAMS_100MHZ }, // I2C 2  B0:D27:F2
    { .device_ID = 0x4BBC, .ic_clk_value = DW_IC_CLOCK_PARAMS_100MHZ }, // I2C 3  B0:D27:F3
    { .device_ID = 0x4BBD, .ic_clk_value = DW_IC_CLOCK_PARAMS_100MHZ }, // I2C 4  B0:D27:F4
    { .device_ID = 0x4BBE, .ic_clk_value = DW_IC_CLOCK_PARAMS_100MHZ }, // I2C 5  B0:D27:F5
    { .device_ID = 0x4BBF, .ic_clk_value = DW_IC_CLOCK_PARAMS_100MHZ }, // I2C 6  B0:D27:F6

    { .device_ID = 0x4B78, .ic_clk_value = DW_IC_CLOCK_PARAMS_133MHZ }, // I2C 0  B0:D21:F0 Elkhart Lake SIO I2C devices
    { .device_ID = 0x4B79, .ic_clk_value = DW_IC_CLOCK_PARAMS_133MHZ }, // I2C 1  B0:D21:F1
    { .device_ID = 0x4B7A, .ic_clk_value = DW_IC_CLOCK_PARAMS_133MHZ }, // I2C 2  B0:D21:F2
    { .device_ID = 0x4B7B, .ic_clk_value = DW_IC_CLOCK_PARAMS_133MHZ }, // I2C 3  B0:D21:F3
    { .device_ID = 0x4B4B, .ic_clk_value = DW_IC_CLOCK_PARAMS_133MHZ }, // I2C 4  B0:D25:F0
    { .device_ID = 0x4B4C, .ic_clk_value = DW_IC_CLOCK_PARAMS_133MHZ }, // I2C 5  B0:D25:F1
    { .device_ID = 0x4B44, .ic_clk_value = DW_IC_CLOCK_PARAMS_133MHZ }, // I2C 6  B0:D16:F0
    { .device_ID = 0x4B45, .ic_clk_value = DW_IC_CLOCK_PARAMS_133MHZ }, // I2C 7  B0:D16:F1
};

static
unsigned long clock_khz_for_did(const pci_did_t did)
{
    unsigned i;
    for (i = 0; i < ARRAY_SIZE(did_clock_khz); i++) {
        if (did == did_clock_khz[i].device_ID) {
            return did_clock_khz[i].ic_clk_value / 1000;
        }
    }
    return 0;
}

static uint32_t i2c_dw_scl_hcnt(const uint32_t ic_clk, const uint32_t tsymbol, const uint32_t tf, const int cond, const int offset)
{
    /*
     * DesignWare I2C core doesn't seem to have solid strategy to meet
     * the tHD;STA timing spec. Configuring _HCNT based on tHIGH spec
     * will result in violation of the tHD;STA spec.
     */
    if (cond) {
        /*
         * Conditional expression:
         *
         *   IC_[FS]S_SCL_HCNT + (1+4+3) >= IC_CLK * tHIGH
         *
         * This is based on the DW manuals, and represents an ideal
         * configuration.  The resulting I2C bus speed will be
         * faster than any of the others.
         *
         * If your hardware is free from tHD;STA issue, try this one.
         */
        return (ic_clk * tsymbol + 500000) / 1000000 - 8 + (unsigned)offset;
    }
    else {
        /*
         * Conditional expression:
         *
         *   IC_[FS]S_SCL_HCNT + 3 >= IC_CLK * (tHD;STA + tf)
         *
         * This is just experimental rule; the tHD;STA period turned
         * out to be proportinal to (_HCNT + 3).  With this setting,
         * we could meet both tHIGH and tHD;STA timing specs.
         *
         * If unsure, you'd better to take this alternative.
         *
         * The reason why we need to take into account "tf" here,
         * is the same as described in i2c_dw_scl_lcnt().
         */
        return (ic_clk * (tsymbol + tf) + 500000) / 1000000 - 3 + (unsigned)offset;
    }
}

static uint32_t i2c_dw_scl_lcnt(const uint32_t ic_clk, const uint32_t tlow, const uint32_t tf, const int offset)
{
    /*
     * Conditional expression:
     *
     *   IC_[FS]S_SCL_LCNT + 1 >= IC_CLK * (tlow + tf)
     *
     * DW I2C core starts counting the SCL CNTs for the LOW period
     * of the SCL clock (tlow) as soon as it pulls the SCL line.
     * In order to meet the tLOW timing spec, we need to take into
     * account the fall time of SCL signal (tf).  Default tf value
     * should be 0.3 us, for safety.
     */
    return ((ic_clk * (tlow + tf) + 500000) / 1000000) - 1 + (unsigned)offset;
}

int baytrail_i2c_bus_active(baytrail_dev_t* const dev)
{
    /* Wait Bus Idel */
    int busy_retry = BUSY_RETRY;
    int ret;
    const pci_bdf_t bdf = pci_bdf(dev->pci);

    ret = baytrail_i2c_wait_bus_not_busy(dev);
    while ((ret != 0) && (busy_retry >= 0)) {

        logerr(bdf, "Busy detected. retry=%d", busy_retry);
        if (busy_retry > 0) {
            busy_retry--;
        }
        else {
            baytrail_i2c_reset(dev);
            logfyi(bdf, "One last try to check for busy...");
        }
        ret = baytrail_i2c_wait_bus_not_busy(dev);
    }
    if (ret != 0) {
        logerr(bdf, "Can't get out of busy.");
        return I2C_STATUS_BUSY;
    }
    if (busy_retry < BUSY_RETRY) {
        logfyi(bdf, "Successfully unstuck from busy.");
    }

    /* Disable the adapter */
    if (0 != baytrail_i2c_enable(dev, DW_IC_ENABLE_STATUS_DISABLE)) {
        baytrail_i2c_reset(dev);
        return I2C_STATUS_BUSY;
    }

    /* Set slave address */
    if (dev->slave_addr_fmt == I2C_ADDRFMT_7BIT) {
        i2c_reg_write32(dev, DW_IC_TAR, dev->slave_addr);
    }
    else {
        i2c_reg_write32(dev, DW_IC_TAR, dev->slave_addr | DW_IC_TAR_10BITADDR_MASTER);
    }

    /* set speed and master mode */
    i2c_reg_write32(dev, DW_IC_CON, dev->master_cfg);

    /* Enforce disabled interrupts (due to HW issues) */
    i2c_reg_write32(dev, DW_IC_INTR_MASK, 0);

    /* Enable the adapter */
    if (0 != baytrail_i2c_enable(dev, DW_IC_ENABLE_STATUS_ENABLE)) {
        baytrail_i2c_reset(dev);
        return I2C_STATUS_BUSY;
    }

    return 0;
}

static
const pci_did_t supported_Intel_DIDs[] =
{
    0x5AAC,  // I2C 0  B0:D22:F0  Broxton Apollo Lake
    0x5AAE,  // I2C 1  B0:D22:F1
    0x5AB0,  // I2C 2  B0:D22:F2
    0x5AB2,  // I2C 3  B0:D22:F3

    0x5AB4,  // I2C 4  B0:D23:F0  Broxton Apollo Lake
    0x5AB6,  // I2C 5  B0:D23:F1
    0x5AB8,  // I2C 6  B0:D23:F2
    0x5ABA,  // I2C 7  B0:D23:F3

    0x0F41,  // I2C 1  B0:D24:F1  Bloomfield Hills
    0x0F42,  // I2C 2  B0:D24:F2
    0x0F43,  // I2C 3  B0:D24:F3
    0x0F44,  // I2C 4  B0:D24:F4
    0x0F45,  // I2C 5  B0:D24:F5
    0x0F46,  // I2C 6  B0:D24:F6
    0x0F47,  // I2C 7  B0:D24:F7

    0x4BB9,  // I2C 0  B0:D27:F0  Elkhart Lake PSE I2C devices
    0x4BBA,  // I2C 0  B0:D27:F1
    0x4BBB,  // I2C 0  B0:D27:F2
    0x4BBC,  // I2C 0  B0:D27:F3
    0x4BBD,  // I2C 0  B0:D27:F4
    0x4BBE,  // I2C 0  B0:D27:F5
    0x4BBF,  // I2C 0  B0:D27:F6

    0x4B78,  // I2C 0  B0:D21:F0  Elkhart Lake SIO I2C devices
    0x4B79,  // I2C 1  B0:D21:F1
    0x4B7A,  // I2C 2  B0:D21:F2
    0x4B7B,  // I2C 3  B0:D21:F3
    0x4B4B,  // I2C 4  B0:D25:F0
    0x4B4C,  // I2C 5  B0:D25:F1
    0x4B44,  // I2C 6  B0:D16:F0
    0x4B45,  // I2C 7  B0:D16:F1
};

static
int already_attached(const pci_err_t err)
{
    return (err == PCI_ERR_ATTACH_EXCLUSIVE)
        || (err == PCI_ERR_ATTACH_SHARED)
        || (err == PCI_ERR_ATTACH_OWNED);
}

static
int match_device(baytrail_dev_t *dev)
{
    pci_bdf_t bdf = PCI_BDF_NONE;
    pci_err_t err = PCI_ERR_ENODEV;

    dev->pci = NULL;

    if ((dev->vid != PCI_VID_ANY) && (dev->did != PCI_DID_ANY)) {
        // attempt to find the specified device
        bdf = pci_device_find(dev->idx, dev->vid, dev->did, PCI_CCODE_ANY);
        if (bdf != PCI_BDF_NONE) {
            dev->pci = pci_device_attach(bdf, pci_attachFlags_EXCLUSIVE_OWNER, &err);

            if (dev->pci) {
                logfyi(bdf, "attached specified VID %04x DID %04x @ idx %u",
                        dev->vid, dev->did, dev->idx);
                return 0;
            }
        }
    }
    else if ((dev->vid == PCI_VID_Intel_Corp) && (dev->did == PCI_DID_ANY)) {
        // attempt to find a supported Intel device
        unsigned i;
        for (i = 0; i < ARRAY_SIZE(supported_Intel_DIDs); i++) {
            dev->did = supported_Intel_DIDs[i];

            for (dev->idx = 0; ; dev->idx++) {
                bdf = pci_device_find(dev->idx, dev->vid, dev->did, PCI_CCODE_ANY);
                if (bdf == PCI_BDF_NONE) break;

                dev->pci = pci_device_attach(bdf, pci_attachFlags_EXCLUSIVE_OWNER, &err);

                if (dev->pci) {
                    logfyi(bdf, "attached supported VID %04x DID %04x @ idx %u",
                            dev->vid, dev->did, dev->idx);
                    return 0;
                }

                if (already_attached(err)) {
                    continue;  // search for the next possible idx/vid/did
                }
                else {
                    logerr(bdf, "failed VID %04x DID %04x @ idx %u: %s",
                            dev->vid, dev->did, dev->idx, pci_strerror(err));
                    return -1;
                }
            }
        }
    } else {
        // Do nothing
    }

    logerr(bdf, "failed VID %04x DID %04x @ idx %u: %s",
            dev->vid, dev->did, dev->idx, pci_strerror(err));
    return -1;
}

static
int map_in_device_registers(baytrail_dev_t *dev)
{
    const pci_bdf_t bdf = pci_bdf(dev->pci);
    pci_err_t rc;

    // Attempt to reset the device function
    {
        rc = pci_device_reset(dev->pci, pci_resetType_e_FUNCTION);
        if ((rc != PCI_ERR_OK) && (rc != PCI_ERR_ENOTSUP)) {
            logerr(bdf, "pci_device_reset failed: %d %s", rc, strerror(rc));
            return -1;
        }
    }

    // Obtain the device base address
    {
        pci_ba_t ba[6];
        int_t nba = (int_t)ARRAY_SIZE(ba);

        rc = pci_device_read_ba(dev->pci, &nba, ba, pci_reqType_e_UNSPECIFIED);
        if (rc != PCI_ERR_OK) {
            logerr(bdf, "pci_device_read_ba failed: %d %s", rc, strerror(rc));
            return -1;
        }

        int_t i;
        for (i = 0; i < nba; i++) {
            if ((ba[i].type == pci_asType_e_MEM)
             && ((ba[i].attr & pci_asAttr_e_EXPANSION_ROM) == 0)) {
                break;
            }
        }

        if (i >= nba) {
            logerr(bdf, "address space not found");
            return -1;
        }
        else {
            const void* const hw_addr = mmap_device_memory(NULL, ba[i].size, PROT_READ|PROT_WRITE|PROT_NOCACHE, MAP_SHARED, ba[i].addr);
            if (hw_addr == MAP_FAILED) {
                logerr(bdf, "mmap_device_memory failed");
                return -1;
            }

            dev->vbase    = (uintptr_t)hw_addr;
            dev->pbase    = ba[i].addr;
            dev->map_size = ba[i].size;

            if (dev->verbose) {
                logfyi(bdf, "mmap_device_memory size %u @ %08x to %p\n",
                        (unsigned)dev->map_size, (unsigned)dev->pbase, (void*)dev->vbase);
            }
        }
    }

    // Obtain the device interrupt number
    if (-1 == dev->irq) {
        const int_t msi_cap_idx = pci_device_find_capid(bdf, CAPID_MSI);
        if (msi_cap_idx >= 0) {
            slogf(_SLOG_SETCODE (_SLOGC_CHAR, 0), _SLOG_INFO, "Found MSI capability msi_cap_idx=%d", msi_cap_idx);

            dev->msi = NULL;
            rc = pci_device_read_cap(bdf, &dev->msi, (uint_t)msi_cap_idx);

            if (rc == PCI_ERR_OK) {
                /* this driver currently only supports 1 IRQ for all interrupt sources */
                rc = cap_msi_set_nirq(dev->pci, dev->msi, 1);
                if (rc == PCI_ERR_OK) {
                   rc = pci_device_cfg_cap_enable(dev->pci, pci_reqType_e_MANDATORY, dev->msi);
                }

                if (rc == PCI_ERR_OK) {
                    slogf(_SLOG_SETCODE (_SLOGC_CHAR, 0), _SLOG_INFO,
											"MSI's successfully enabled for 1 IRQ");
                }
                else {
                    pci_device_cfg_cap_disable(dev->pci, pci_reqType_e_MANDATORY, dev->msi);
                    dev->msi = NULL;
                    slogf(_SLOG_SETCODE (_SLOGC_CHAR, 0), _SLOG_ERROR,
											"MSI capability enable failed, %s", pci_strerror(rc));
                }
            }
            else {
                slogf(_SLOG_SETCODE (_SLOGC_CHAR, 0), _SLOG_ERROR,
										"Unable to read MSI capability, %s", pci_strerror(rc));

                dev->msi = NULL;
            }
        }
        else {
            slogf(_SLOG_SETCODE (_SLOGC_CHAR, 0), _SLOG_INFO, "MSI's not supported");
        }

        /* Get IRQ */
        int_t       nirq = 1;
        pci_irq_t   irq;
        rc = pci_device_read_irq(dev->pci, &nirq, &irq);

        if ((rc != PCI_ERR_OK) || (nirq != 1)) {
            rc = (nirq != 1) ? PCI_ERR_IRQ_NOT_AVAIL : rc;
            logerr(bdf, "pci_device_read_irq failed: %d Unable to determine IRQ", rc);
            return -1;
        }

        logfyi(bdf, "connected to IRQ %u", irq);

        dev->irq = irq;
    }

    return 0;
}

static
void cleanup(baytrail_dev_t *dev)
{
    if (dev->pci) {
        const pci_bdf_t bdf = pci_bdf(dev->pci);

        // unmap the device registers
        if (dev->vbase) {
            const int rc = munmap_device_memory((void*)dev->vbase, dev->map_size);
            if (rc == -1) {
                logerr(bdf, "munmap_device_memory failed");
            }
            dev->vbase = 0;
        }
        if (NULL != dev->msi) {
            pci_device_cfg_cap_disable(dev->pci, pci_reqType_e_MANDATORY, dev->msi);
            free(dev->msi);
            dev->msi = NULL;
        }
        // detach from the PCI server
        const pci_err_t rc = pci_device_detach(dev->pci);
        if (rc != PCI_ERR_OK) {
            logerr(bdf, "pci_device_detach failed: %d %s", rc, strerror(rc));
        }
        dev->pci = NULL;
    }

    free(dev);  // free device object
}

static void baytrail_i2c_set_fifo_threshold(baytrail_dev_t *dev)
{
    uint32_t reg, tx_fifo_depth, rx_fifo_depth;

    reg = i2c_reg_read32(dev, DW_IC_COMP_PARAM_1);
    tx_fifo_depth = ((reg >> DW_IC_COMP_PARAM_1_TX_DEPTH_F) & 0xff) + 1;
    rx_fifo_depth = ((reg >> DW_IC_COMP_PARAM_1_RX_DEPTH_F) & 0xff) + 1;

    dev->fifo_depth = min(tx_fifo_depth, rx_fifo_depth);

    i2c_reg_write32(dev, DW_IC_TX_TL, dev->fifo_depth / 2);
    i2c_reg_write32(dev, DW_IC_RX_TL, dev->fifo_depth / 2);
}

void* baytrail_i2c_init(int argc, char *argv[])
{
    baytrail_dev_t *dev;
    uint32_t reg;

    if (ThreadCtl(_NTO_TCTL_IO, NULL) == -1) {
        perror("ThreadCtl");
        return NULL;
    }

    dev = calloc(1, sizeof(*dev));  // allocate device object
    if (!dev) {
        return NULL;
    }

    if (baytrail_i2c_parseopts(dev, argc, argv) == -1) {
        goto fail_cleanup;
    }

    if (match_device(dev)) {
        goto fail_cleanup;
    }

    if (map_in_device_registers(dev)) {
        goto fail_cleanup;
    }

    /* check I2C component type*/
    reg = i2c_reg_read32(dev, DW_IC_COMP_TYPE);
    if (reg != DW_IC_COMP_TYPE_VALUE) {
        logerr(pci_bdf(dev->pci), "%s: Unknown I2C component type: 0x%08x", __FUNCTION__, reg);
        goto fail_cleanup;
    }

    /*
     * Attach interrupt
     */
    struct sigevent intrevent;

    SIGEV_INTR_INIT(&intrevent);

    dev->iid = InterruptAttachEvent(dev->irq, &intrevent, _NTO_INTR_FLAGS_TRK_MSK);
    if (dev->iid == -1) {
        logerr(pci_bdf(dev->pci), "%s: InterruptAttachEvent", __FUNCTION__);
        goto fail_cleanup;
    }

    /* Disable the I2C adapter */
    baytrail_i2c_enable(dev, DW_IC_ENABLE_STATUS_DISABLE);

    baytrail_i2c_init_registers(dev);

    return dev;

fail_cleanup:
    cleanup(dev);

    return NULL;
}

void baytrail_i2c_init_registers(baytrail_dev_t *dev)
{
    uint32_t reg;

    /* Set clock_khz for the support device */
    if (dev->clock_khz == 0) {
        dev->clock_khz = (uint32_t)clock_khz_for_did(dev->did);
    }

    /* Get I2C input clock */
    if (dev->clock_khz == 0) {
        if (i2c_reg_read32(dev, DW_IC_CLOCK_PARAMS)) {
            dev->clock_khz = DW_IC_CLOCK_PARAMS_133MHZ / 1000;
        }
        else {
            dev->clock_khz = DW_IC_CLOCK_PARAMS_100MHZ / 1000;
        }
    }

    /* Set SCL timing parameters for standard-mode */
    /* Intel Recommended setting; SF Case # 00168315 */
    if (dev->fixed_scl) {
        dev->ss_hcnt = 580;
        dev->ss_lcnt = 730;
    }
    else{
        dev->ss_hcnt = i2c_dw_scl_hcnt(dev->clock_khz,
                    4700,   // tHD;STA = tHIGH  = 4.7 us
                    300,    // scl_falling_time = 0.3 us
                    0,      // 0: DW default, 1: Ideal
                    0);     // No offset
        dev->ss_lcnt = i2c_dw_scl_lcnt(dev->clock_khz,
                    4700,   // tLOW = 4.7 us
                    300,    // scl_falling_time = 0.3 us
                    0);     // No offset
    }

    logfyi(pci_bdf(dev->pci), "i2c-baytrail DW_IC_SS_SCL_HCNT %08x ", dev->ss_hcnt);
    logfyi(pci_bdf(dev->pci), "i2c-baytrail DW_IC_SS_SCL_LCNT %08x ", dev->ss_lcnt);

    i2c_reg_write32(dev, DW_IC_SS_SCL_HCNT, dev->ss_hcnt);
    i2c_reg_write32(dev, DW_IC_SS_SCL_LCNT, dev->ss_lcnt);

    /* Set SCL timing parameters for fast-mode */
    /* Intel Recommended setting; SF Case # 00168315 */
    if (dev->fixed_scl) {
        dev->fs_hcnt = 100;
        dev->fs_lcnt = 214;
    }
    else {
        dev->fs_hcnt = i2c_dw_scl_hcnt(dev->clock_khz,
                    600,    // tHD;STA = tHIGH  = 0.6 us
                    300,    // scl_falling_time = 0.3 us
                    0,      // 0: DW default, 1: Ideal
                    0);     // No offset
        dev->fs_lcnt = i2c_dw_scl_lcnt(dev->clock_khz,
                    1300,   // tLOW = 1.3 us
                    300,    // scl_falling_time = 0.3 us
                    0);     // No offset
    }

    logfyi(pci_bdf(dev->pci), "i2c-baytrail DW_IC_FS_SCL_HCNT %08x ", dev->fs_hcnt);
    logfyi(pci_bdf(dev->pci), "i2c-baytrail DW_IC_FS_SCL_LCNT %08x ", dev->fs_lcnt);

    i2c_reg_write32(dev, DW_IC_FS_SCL_HCNT, dev->fs_hcnt);
    i2c_reg_write32(dev, DW_IC_FS_SCL_LCNT, dev->fs_lcnt);

    /* Configure SDA Hold Time */
    reg = i2c_reg_read32(dev, DW_IC_COMP_VERSION);
    if (reg >= DW_IC_SDA_HOLD_MIN_VERS) {

        /* Intel Recommended setting; SF Case # 00168315 */
        if (dev->fixed_scl) {
            const uint32_t sda_hold_time = 28;
            i2c_reg_write32(dev, DW_IC_SDA_HOLD, sda_hold_time);

            logfyi(pci_bdf(dev->pci), "i2c-baytrail Intel:DW_IC_SDA_HOLD %08x ", sda_hold_time);
        }
        else {

            /* The I2C-bus specification requires at least 300 ns for the SDA hold time */
            const uint32_t sda_hold_time = max(dev->sda_hold_time,
                        (dev->clock_khz * SDA_HOLD_TIME_VALUE_ns + 500000) / 1000000);
            if (sda_hold_time < dev->fs_lcnt) {
                i2c_reg_write32(dev, DW_IC_SDA_HOLD, sda_hold_time);

                logfyi(pci_bdf(dev->pci), "i2c-baytrail DW_IC_SDA_HOLD %08x ", sda_hold_time);
            }
            else {
                logerr(pci_bdf(dev->pci), "%s: SDA hold time %u too large for SCL_LCNT %u",
                        __FUNCTION__, sda_hold_time, dev->fs_lcnt);
            }
        }
    }
    else {
        logerr(pci_bdf(dev->pci), "%s: Hardware too old to adjust SDA hold time!", __FUNCTION__);
    }

    /* Configure Tx/Rx FIFO threshold */
    baytrail_i2c_set_fifo_threshold(dev);

    /* set default bus speed */
    baytrail_i2c_set_bus_speed(dev, 100000, NULL);

    /* Clear and Disable i2c Interrupt */
    i2c_reg_read32(dev, DW_IC_CLR_INTR);
    i2c_reg_write32(dev, DW_IC_INTR_MASK, 0);
}

void baytrail_i2c_fini(void *hdl)
{
    baytrail_dev_t* const dev = hdl;

    /* hdl can be NULL if some error occurred during initialization. */
    if (hdl == NULL) {
        return;
    }

    baytrail_i2c_enable(dev, DW_IC_ENABLE_STATUS_DISABLE);

    InterruptDetach(dev->iid);

    cleanup(dev);
}
