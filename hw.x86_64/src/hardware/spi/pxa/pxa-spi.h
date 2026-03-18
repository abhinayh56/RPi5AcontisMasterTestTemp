/*
 * Copyright (c) 2023, BlackBerry Limited.
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


#ifndef _PXA_SPI_H_
#define _PXA_SPI_H_

#include <atomic.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <dlfcn.h>
#include <errno.h>
#include <unistd.h>
#include <string.h>
#include <sys/slog.h>
#include <sys/slogcodes.h>
#include <sys/mman.h>
#include <sys/types.h>
#include <hw/inout.h>
#include <pci/pci.h>
#include <pci/pci_id.h>
#include <pci/pci_ccode.h>
#include <hw/io-spi.h>


#define SPI_MIN_WORD_WIDTH              (4)
#define SPI_MAX_WORD_WIDTH              (32)

#define DEFAULT_RX_THRESHOLD            (32)
#define DEFAULT_TX_THRESHOLD            (32)

#define DEFAULT_DMA_RX_THRESHOLD        (1)
#define DEFAULT_DMA_TX_THRESHOLD        (16)
#define DEFAULT_DMA_TX_LOW_THRESHOLD    (48)

#ifndef ARRAY_SIZE
#define ARRAY_SIZE(a)   (sizeof(a) / sizeof((a)[0]))
#endif

/* Intel Generic SPI Register Definitions */
#define SSCR0           (0x00)          /* SSP Control Register 0 */
#define SSCR1           (0x04)          /* SSP Control Register 1 */
#define SSSR            (0x08)          /* SSP Status Register */
#define SSITR           (0x0c)          /* SSP Interrupt Test Register */
#define SSDR            (0x10)          /* SSP Data Register */
#define SSTO            (0x28)          /* SSP Time-Out Register */
#define SSPSP           (0x2c)          /* SSP Programmable Serial Protocol Register */
#define SITF            (0x44)          /* SPI Transmit FIFO Register */
#define SIRF            (0x48)          /* SPI Receive FIFO Register */

/* SSCR0 */
#define SSCR0_MOD       (1 << 31)       /* Mode */
#define SSCR0_ACS       (1 << 30)       /* Audio Clock Select */
#define SSCR0_TIM       (1 << 23)       /* Transmit FIFO Under Run Interrupt Mask */
#define SSCR0_RIM       (1 << 22)       /* Receive FIFO Over Run Interrupt Mask */
#define SSCR0_NCS       (1 << 21)       /* Network Clock Select */
#define SSCR0_EDSS      (1 << 20)       /* Extended Data Size Select */
#define SSCR0_SCR       (0xfff << 8)    /* Serial Clock Rate */
#define SSCR0_SSE       (1 << 7)        /* Synchronous Serial Port Enable */
#define SSCR0_ECS       (1 << 6)        /* External Clock Select */
#define SSCR0_FRF       (3 << 4)        /* Frame Format */
#define SSCR0_DSS       (0xf << 0)      /* Data Size Select */

#define SSCR0_FrameFmt(x)   (((x) & 3) << 4)            /* Frame Format */
#define SSCR0_SCRate(x)     ((((x) - 1) & 0xfff) <<  8) /* Serial Clock Rate */
#define SSCR0_DSSize(x)     ((((x) - 1) & 0xf)   <<  0) /* Data Size Select */
#define SSCR0_SCR_MAX   (0xfff)

/* SSCR1 */
#define SSCR1_TTELP     (1 << 31)       /* TXD Tristate Enable on Last Phase */
#define SSCR1_TTE       (1 << 30)       /* TXD Tristate Enable */
#define SSCR1_EBCEI     (1 << 29)       /* Enable Bit Count Error Interrupt */
#define SSCR1_SCFR      (1 << 28)       /* Slave Clock Free Running */
#define SSCR1_ECRA      (1 << 27)       /* Enable Clock Request A */
#define SSCR1_ECRB      (1 << 26)       /* Enable Clock Request B */
#define SSCR1_SCLKDIR   (1 << 25)       /* SSP Serial Bit Rate Clock (SSPSCLK) Direction */
#define SSCR1_SFRMDIR   (1 << 24)       /* SSP Frame (SSPSFRM) Direction */
#define SSCR1_RWOT      (1 << 23)       /* Receive With Out Transmit */
#define SSCR1_TRAIL     (1 << 22)       /* Trailing Byte */
#define SSCR1_TSRE      (1 << 21)       /* Transmit Service Request Enable */
#define SSCR1_RSRE      (1 << 20)       /* Receive Service Request Enable */
#define SSCR1_TINTE     (1 << 19)       /* Receiver Time-out Interrupt Enable */
#define SSCR1_PINTE     (1 << 18)       /* Peripheral Trailing Byte Interrupts Enable */
#define SSCR1_IFS       (1 << 16)       /* Invert Frame Signal */
#define SSCR1_STRF      (1 << 15)       /* Select FIFO for EFWR */
#define SSCR1_EFWR      (1 << 14)       /* Enable FIFO Write/Read */
#define SSCR1_RFT       (0xf << 10)     /* Receive FIFO Trigger Threshold */
#define SSCR1_TFT       (0xf << 6)      /* Transmit FIFO Trigger Threshold */
#define SSCR1_MWDS      (1 << 5)        /* Microwire Transmit Data Size */
#define SSCR1_SPH       (1 << 4)        /* Motorola SPI SSPSCLK Phase Setting */
#define SSCR1_SPO       (1 << 3)        /* Motorola SPI SSPSCLK Polarity Setting */
#define SSCR1_LBM       (1 << 2)        /* Loop-Back Mode */
#define SSCR1_TIE       (1 << 1)        /* Transmit FIFO Interrupt Enable */
#define SSCR1_RIE       (1 << 0)        /* Receive FIFO Interrupt Enable */

#define SSCR1_RFThresh(x) ((((x) - 1) & 0xf) << 10)     /* Receive FIFO Trigger Threshold */
#define SSCR1_TFThresh(x) ((((x) - 1) & 0xf) << 6)      /* Transmit FIFO Trigger Threshold */

/* SSSR */
#define SSSR_BCE        (1 << 23)       /* Bit Count Error                     RW1C  */
#define SSSR_CSS        (1 << 22)       /* Clock Synchronization Status        RO    */
#define SSSR_TUR        (1 << 21)       /* Transmit FIFO Under Run             RW1C  */
#define SSSR_EOC        (1 << 20)       /* End of Chain                        RW1C  */
#define SSSR_TINT       (1 << 19)       /* Receiver Time-out Interrupt         RW1C  */
#define SSSR_PINT       (1 << 18)       /* Peripheral Trailing Byte Interrupt  RW1C  */
#define SSSR_RFL        (0xf << 12)     /* Receive FIFO Level                  RO    */
#define SSSR_TFL        (0xf << 8)      /* Transmit FIFO Level                 RO    */
#define SSSR_ROR        (1 << 7)        /* Receive FIFO Overrun                RW1C  */
#define SSSR_RFS        (1 << 6)        /* Receive FIFO Service Request        RO    */
#define SSSR_TFS        (1 << 5)        /* Transmit FIFO Service Request       RO    */
#define SSSR_BSY        (1 << 4)        /* SSP Busy                            RO    */
#define SSSR_RNE        (1 << 3)        /* Receive FIFO Not Empty              RO    */
#define SSSR_TNF        (1 << 2)        /* Transmit FIFO Not Full              RO    */

#define SSSR_RFLevel(x) ((((x) >> 12) & 0xf) + 1)       /* get Receive FIFO Level */
#define SSSR_TFLevel(x) (((x) >> 8)   & 0xf)            /* get Transmit FIFO Level */

/* SSITR */
#define SSITR_TROR      (1 << 7)        /* Test Receive FIFO Overrun */
#define SSITR_TRFS      (1 << 6)        /* Test Receive FIFO Service Request */
#define SSITR_TTFS      (1 << 5)        /* Test Transmit FIFO Service Request */

/* SSPSP */
#define SSPSP_FSRT      (1 << 25)       /* Frame Sync Relative Timing Bit */
#define SSPSP_DMYSTOP   (3 << 23)       /* Dummy Stop */
#define SSPSP_SFRMWDTH  (0x3f << 16)    /* Serial Frame Width */
#define SSPSP_SFRMDLY   (0x7f << 9)     /* Serial Frame Delay */
#define SSPSP_DMYSTRT   (3 << 7)        /* Dummy Start */
#define SSPSP_STRTDLY   (7 << 4)        /* Start Delay */
#define SSPSP_ETDS      (1 << 3)        /* End of Transfer Data State */
#define SSPSP_SFRMP     (1 << 2)        /* Serial Frame Polarity */
#define SSPSP_SCMODE    (3 << 0)        /* Serial Bit-rate Clock Mode */

/* SITF */
#define SITF_SITFL      (0x1ff << 16)   /* SPI Transmit FIFO Level */
#define SITF_LWMTF      (0xff << 8)     /* Low Water Mark Transmit FIFO */
#define SITF_HWMTF      (0xff << 0)     /* High Water Mark Transmit FIFO */

#define SITF_Low(x)     ((((x) - 1) & 0xff) << 8)       /* Low Water Mark Transmit FIFO */
#define SITF_High(x)    ((((x) - 1) & 0xff) << 0)       /* High Water Mark Transmit FIFO */

/* SIRF */
#define SIRF_SIRFL      (0x1ff << 8)    /* SPI Receive FIFO Level */
#define SIRF_WMRF       (0xff << 0)     /* Water Mark Receive FIFO */

#define SIRF_High(x)    ((((x) - 1) & 0xff) << 0)       /* Water Mark Receive FIFO */

#define ALL_SSCR1_INTERRUPTS        ( SSCR1_EBCEI \
                                    | SSCR1_TSRE  \
                                    | SSCR1_RSRE  \
                                    | SSCR1_TINTE \
                                    | SSCR1_PINTE \
                                    | SSCR1_TIE   \
                                    | SSCR1_RIE )

/* These RW1C status bits must be manually cleared */
#define ALL_SSSR_RW1C_STATUS_BITS   ( SSSR_BCE  \
                                    | SSSR_TUR  \
                                    | SSSR_EOC  \
                                    | SSSR_TINT \
                                    | SSSR_PINT \
                                    | SSSR_ROR )

#define SPI_DMA_TX_CH_BASE          0x800
#define SPI_DMA_RX_CH_BASE          0x858

/* SPI DMA CHANNEL OFFSETS */
#define SPI_DMA_SAR_LO              0x000
#define SPI_DMA_SAR_HI              0x004
#define SPI_DMA_DAR_LO              0x008
#define SPI_DMA_DAR_HI              0x00C
#define SPI_DMA_LLP_LO              0x010
#define SPI_DMA_LLP_HI              0x014
#define SPI_DMA_CTL_LO              0x018
#define SPI_DMA_CTL_HI              0x01C
#define SPI_DMA_SSTAT               0x020
#define SPI_DMA_DSTAT               0x028
#define SPI_DMA_SSTATAR_LO          0x030
#define SPI_DMA_SSTATAR_HI          0x034
#define SPI_DMA_DSTATAR_LO          0x038
#define SPI_DMA_DSTATAR_HI          0x03C
#define SPI_DMA_CFG_LO              0x040
#define SPI_DMA_CFG_HI              0x044
#define SPI_DMA_SGR                 0x048
#define SPI_DMA_DSR                 0x050

/* SPI DMA CONTROLLER OFFSETS */
#define SPI_DMA_CONTROLLER_BASE     0xAC0

#define SPI_DMA_RAWTFR              0xAC0
#define SPI_DMA_RAWBLOCK            0xAC8
#define SPI_DMA_RAWSRCTRAN          0xAD0
#define SPI_DMA_RAWDSTTRAN          0xAD8
#define SPI_DMA_RAWERR              0xAE0

#define SPI_DMA_STATUSTFR           0xAE8
#define SPI_DMA_STATUSBLOCK         0xAF0
#define SPI_DMA_STATUSSRCTRAN       0xAF8
#define SPI_DMA_STATUSDSTTRAN       0xB00
#define SPI_DMA_STATUSERR           0xB08

#define SPI_DMA_MASKTFR             0xB10
#define SPI_DMA_MASKBLOCK           0xB18
#define SPI_DMA_MASKSRCTRAN         0xB20
#define SPI_DMA_MASKDSTTRAN         0xB28
#define SPI_DMA_MASKERR             0xB30

#define SPI_DMA_CLEARTFR            0xB38
#define SPI_DMA_CLEARBLOCK          0xB40
#define SPI_DMA_CLEARSRCTRAN        0xB48
#define SPI_DMA_CLEARDSTTRAN        0xB50
#define SPI_DMA_CLEARERR            0xB58

#define SPI_DMA_STATUSINT           0xB60

#define SPI_DMA_DMACFGREG           0xB98

#define SPI_DMA_CHENREG             0xBA0

#define SPI_DMA_REMAP_LO            0x240
#define SPI_DMA_REMAP_HI            0x244

#define SPI_DEVICE_RESET            0x204

#define SPI_TX_BIT_COUNT            0x218
#define SPI_RX_BIT_COUNT            0x21C

#define SPI_DEVICE_CAPABILITIES     0x2FC

#define SPI_DMA_MEM_TO_DEV_TRANSFER 1
#define SPI_DMA_DEV_TO_MEM_TRANSFER 2

#define SPI0_DMA_TX_CHANNEL_CLASS   0x00
#define SPI0_DMA_TX_CHANNEL_WEIGHT  0x00

#define SPI0_DMA_RX_CHANNEL_CLASS   0x01
#define SPI0_DMA_RX_CHANNEL_WEIGHT  0x00

#define SPI_PIO_PORTION_HALF_SIZE   (30)
#define SPI_PIO_PORTION_SIZE        (2*SPI_PIO_PORTION_HALF_SIZE)

#define SPI_CS_MAX                  1

#define SPI_INTR_DONE               0
#define SPI_INTR_CONTINUE           1
#define SPI_INTR_ERR                2

enum {
    INTERRUPT_INVALID,
    INTERRUPT_FIRST,
    INTERRUPT_MIDDLE,
    INTERRUPT_LAST,
    INTERRUPT_DMA,
};

typedef struct
{
    spi_ctrl_t     *spi_ctrl;           /* The address of spi_ctrl structure */
    spi_bus_t      *bus;                /* The address of bus structure */

    pci_devhdl_t    pci;                /* PCI device handle */
    uint32_t        idx;                /* PCI device index */
    pci_vid_t       vid;                /* PCI Vendor ID */
    pci_did_t       did;                /* PCI Device ID */
    int             iid;                /* InterruptAttachEvent ID */

    pci_ba_val_t    pbase;              /* physical base address of device registers */
    uintptr_t       vbase;              /* virtual address of device registers */
    uint64_t        map_size;           /* size of device registers */

    void           *pbuf;              /* xfer buffer */
    uint32_t        xlen;               /* Full transaction length requested by client */
    uint32_t        tlen;               /* transmit counter (in data items) */
    uint32_t        rlen;               /* receive counter (in data items) */
    uint32_t        dlen;               /* word size */
    uint32_t        dscale;             /* right shift to convert from byte count to word count */
    uint64_t        xtime_us;           /* time for exchange in microseconds */
    bool            loopback;           /* loopback mode */

    uint32_t        cr0;                /* SSCR0 setting */
    uint32_t        cr1;                /* SSCR1 setting */
    uint32_t        intr_count;
    uint32_t        intr_processing;

    bool            dma_mode;           /* DMA transaction mode */
    bool            dma_active;         /* DMA is active */
    uint32_t        dma_rx_thresh;      /* Receive FIFO threshold - DMA mode */
    uint32_t        dma_tx_thresh;      /* Transmit FIFO threshold - DMA mode */
    uint32_t        dma_tx_low_thresh;	/* Transmit FIFO threshold (Low) - DMA mode */
    uint32_t        dma_xfer_count;
    uint32_t        dma_error_count;
    uint32_t        dma_error;
    int             typed_mem_fd;

    uint32_t        xfer_count;
    uint32_t        rx_thresh;          /* Receive FIFO threshold */
    uint32_t        tx_thresh;          /* Transmit FIFO threshold */
} pxa_spi_t;

static inline uint32_t pxa_read32(const pxa_spi_t *const spi, const uint32_t offset)
{
    volatile const uint32_t *const addr = (uint32_t*)(spi->vbase + offset);
    const uint32_t value = *addr;
    return value;
}

static inline void pxa_write32(const pxa_spi_t *const spi, const uint32_t offset, const uint32_t value)
{
    volatile uint32_t *const addr = (uint32_t*)(spi->vbase + offset);
    *addr = value;
}

static inline void pxa_read_fifo_uint8(const pxa_spi_t *const spi, uint8_t *const buffer, const uint32_t len)
{
    register uint32_t i;
    volatile const uint32_t *const addr = (uint32_t*)(spi->vbase + SSDR);
    for (i = 0; i < len; i++) {
        buffer[i] = (uint8_t) *addr;
    }
}

static inline void pxa_write_fifo_uint8(const pxa_spi_t *const spi, const uint8_t *const buffer, const uint32_t len)
{
    register size_t i;
    volatile uint32_t *const addr = (uint32_t*)(spi->vbase + SSDR);
    for (i = 0; i < len; i++) {
        *addr = buffer[i];
    }
}

static inline void pxa_read_fifo_uint16(const pxa_spi_t *const spi, uint16_t *const buffer, const uint32_t len)
{
    register size_t i;
    volatile const uint32_t *const addr = (uint32_t*)(spi->vbase + SSDR);
    for (i = 0; i < len; i++) {
        buffer[i] = (uint16_t) *addr;
    }
}

static inline void pxa_write_fifo_uint16(const pxa_spi_t *const spi, const uint16_t *const buffer, const uint32_t len)
{
    register size_t i;
    volatile uint32_t *const addr = (uint32_t*)(spi->vbase + SSDR);
    for (i = 0; i < len; i++) {
        *addr = buffer[i];
    }
}

static inline void pxa_read_fifo_uint32(const pxa_spi_t *const spi, uint32_t *const buffer, const uint32_t len)
{
    register size_t i;
    volatile const uint32_t *const addr = (uint32_t*)(spi->vbase + SSDR);
    for (i = 0; i < len; i++) {
        buffer[i] = *addr;
    }
}

static inline void pxa_write_fifo_uint32(const pxa_spi_t *const spi, const uint32_t *const buffer, const uint32_t len)
{
    register size_t i;
    volatile uint32_t *const addr = (uint32_t*)(spi->vbase + SSDR);
    for (i = 0; i < len; i++) {
        *addr = buffer[i];
    }
}

static inline unsigned get_field(const uint32_t reg_value, const unsigned start, const unsigned end)
{
    return (reg_value>>end)&((~(uint32_t)0)>>(31-start+end));
}

static inline void set_field(uint32_t *reg_value, const unsigned start, const unsigned end, const uint32_t field_value)
{
    register uint32_t mask = ((~(uint32_t)0)>>(31-start+end))<<end;
    *reg_value = (*reg_value&~mask) | ((field_value<<end)&mask);
}

/* Function proto */
int  pxa_cfg(const void *const hdl, spi_dev_t *spi_dev);
int  pxa_prepare_for_transfer(pxa_spi_t *spi, const spi_cfg_t *cfg, const uint32_t tnbytes, const uint32_t rnbytes);
int  pxa_attach_intr(pxa_spi_t *spi);
int  pxa_process_interrupts(pxa_spi_t *spi);
int  pxa_wait(pxa_spi_t *spi);
void pxa_device_reset(const pxa_spi_t *const spi);
void pxa_dump_spi_registers(const pxa_spi_t *const spi);
void pxa_dump_dma_registers(const pxa_spi_t *const spi);
void pxa_dump_spi_and_dma_registers(const pxa_spi_t *const spi);

/* Function interface for io-spi */
int  spi_init(spi_bus_t *bus);
void pxa_fini(void *const hdl);
void pxa_drvinfo(const void *const hdl, spi_drvinfo_t *info);
void pxa_devinfo(const void *const hdl, const spi_dev_t *const spi_dev, spi_devinfo_t *const info);
int  pxa_setcfg(const void *const hdl, spi_dev_t *spi_dev, const spi_cfg_t *const cfg);
int  pxa_xfer(void *const hdl, spi_dev_t *const spi_dev, uint8_t *const buf, const uint32_t tnbytes, const uint32_t rnbytes);
int  pxa_dmaxfer(void *const hdl, spi_dev_t *spi_dev, dma_addr_t *const addr, const uint32_t tnbytes, const uint32_t rnbytes);
int  pxa_dma_allocbuf(void *const hdl, dma_addr_t *addr, const uint32_t len);
int  pxa_dma_freebuf(void *const hdl, dma_addr_t *addr);

#endif /* _PXA_SPI_H_ */
