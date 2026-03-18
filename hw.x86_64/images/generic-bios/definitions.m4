include(`../common-definitions.m4')

# Variant-specific defines/un-defines/overrides can go here
define(`__TYPE__',              `bios')
define(`__STARTUP_OPTS__',      `')
## Console
define(`__CONSOLE__',           `/dev/con1')

define(`__BOARD_LATE_START__', `
    ############################################################################################
    ## Debug console
    ############################################################################################
    ksh /proc/boot/debug-console.sh &

    ############################################################################################
    ## Input service
    ############################################################################################
    display_msg "Starting input service ..."
    io-hid -d usb
    waitfor /dev/io-hid/io-hid

    ############################################################################################
    ## Consoles and shells
    ############################################################################################
    display_msg "Starting consoles ..."
    devc-con-hid -n8
    waitfor /dev/con1
    waitfor /dev/con2
    waitfor /dev/con3
    waitfor /dev/con4
    waitfor /dev/con5
    waitfor /dev/con6
    waitfor /dev/con7
    waitfor /dev/con8

    reopen /dev/con2
    [+session] sh &

    reopen /dev/con3
    [+session] sh &

    reopen /dev/con4
    [+session] sh &

    reopen /dev/con5
    [+session] sh &

    reopen /dev/con6
    [+session] sh &

    reopen /dev/con7
    [+session] sh &

    reopen /dev/con8
    [+session] sh &
')

define(`__BOARD_FILES__', `
################################################################################################
## Libraries for io-hid drivers
################################################################################################
/lib/dll/devh-usb.so=devh-usb.so
/lib/dll/devh-ps2ser.so=devh-ps2ser.so

################################################################################################
## Console and io-hid drivers
################################################################################################
/sbin/devc-con=devc-con
/sbin/devc-con-hid=devc-con-hid
/sbin/io-hid=io-hid

__DEBUG_CONSOLE_SCRIPT__

################################################################################################
## Script for launching the block driver
################################################################################################
blk-start.sh={
#!/bin/ksh

PCI_DEVICE_LIST=/dev/shmem/pci_device.list
STD_NULL=/dev/null

## supported list for the EIDE block devices
EIDE_8086_7111=8086/7111

## supported list for the SDMMC block devices
SDMMC_8086_5acc=8086/5acc

## check if there is the pci device list
if [ ! -r $PCI_DEVICE_LIST ]; then
    pci-tool -v > $PCI_DEVICE_LIST
fi

# redirect stderr to slog2
exec 2> /dev/console

# check if there is the NVMe device in the list
if  grep "Non-volatile Memory Subsystem (NVMe Interface IO Controller)" $PCI_DEVICE_LIST > $STD_NULL
then
    echo "Starting NVMe block driver ..."
    devb-nvme cam ifdef(`__SMMU__', `pnp,smmu=on', `pnp') disk name=nvme
    waitfor /dev/nvme0 3
fi

# check if there is the SATA device in the list
if  grep "SATA Mass Storage Controller (AHCI Interface)" $PCI_DEVICE_LIST > $STD_NULL
then
    echo "Starting AHCI block driver ..."
    devb-ahci cam ifdef(`__SMMU__', `pnp,smmu=on', `pnp') disk name=sata
    waitfor /dev/sata0 3
fi

# check if there is the EIDE device in the list
if grep $EIDE_8086_7111 $PCI_DEVICE_LIST > $STD_NULL
then
    echo "Starting EIDE block driver ..."
    devb-eide cam ifdef(`__SMMU__', `pnp,smmu=on', `pnp') disk name=eide
    waitfor /dev/eide0 3
fi

# check if there is the SDMMC device in the list
if  grep $SDMMC_8086_5acc $PCI_DEVICE_LIST > $STD_NULL
then
    echo "Starting SDMMC block driver ..."
    devb-sdmmc cam ifdef(`__SMMU__', `pnp,smmu=on', `pnp') sdio idx=0,vid=0x8086,did=0x5acc disk name=sdmmc
    waitfor /dev/sdmmc0 3
fi
}

################################################################################################
## END OF BUILD SCRIPT
################################################################################################
')

