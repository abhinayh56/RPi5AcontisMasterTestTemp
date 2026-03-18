ifelse(
#
# These are the default definitions for the x86_64 platform.
# Variants can override (re-define, undefine, or in some cases
# extend) macros in their definitions.m4 files.
#
# Extending would be self-referencing, like e.g. define(`__MACRO__', `...' __MACRO__)
# If __MACRO__ contains commas, however, it can be tricky to make it work,
# since m4 thinks those commas are options delimiters for the
# define function. It is doable, but error-prone, so proceed with caution.
#
)

## Boot parameters
define(`__LD_QNX__',            `ldqnx-64.so.2')
define(`__BOOT_ADDR__',         `0x2000000')
define(`__ARCH__',              `x86_64')
define(`__TYPE__',              `uefi')
define(`__COMPRESS_ATTR__',     `+compress')
define(`__PROCNTO_MODULES__',   `')
define(`__STARTUP__',           `startup-x86')
define(`__STARTUP_OPTS__',      `-D8250..115200')
define(`__PROCNTO__',           `procnto-smp-instr')
define(`__PROCNTO_OPTS__',      `')

## Console
define(`__CONSOLE__',           `/dev/ser1')

## add /root/.profile
define(`__ROOT_PROFILE__',      `')

## ENV profile, use to overwrite the common /etc/profile

## System Memory Management Unit (smmu)
ifdef(`__SMMU__', `
define(`__SMMU_USB_HOST_OPTS__', `-s smmu=on')
define(`__NET_CONF_FILE_NAME__', `iosock.conf')
define(`__NET_CONF_FILE_PATH__', `/etc/system/config/net')
define(`__NET_CONF_FILE_CONTENT__', `qnx.smmu="1"')
')

## Audio driver

## Block driver
define(`__BLOCK_DRVR__',        `devb-ahci, devb-eide, devb-nvme, devb-ram, devb-sdmmc')

define(`__DEVB_DRVR_START__', `
    ############################################################################################
    ## Block driver
    ############################################################################################
    ksh /proc/boot/blk-start.sh
')

## Network driver
define(`__NET_DRVR__',          `devs-vmx.so')
define(`__NET_OPTS__',          `-d vmx')
define(`__NET_DEV__',           `vmx0')

## USB host driver
define(`__USB_HOST_DRVR__',     `devu-hcd-ehci.so, devu-hcd-ohci.so, devu-hcd-uhci.so, devu-hcd-xhci.so')
define(`__USB_HOST_OPTS__',     `ifdef(`__SMMU_USB_HOST_OPTS__', `__SMMU_USB_HOST_OPTS__') -d xhci -d ehci -d ohci -d uhci')
define(`__USB_HOST_DEV__',      `/dev/usb/io-usb-otg')

## Persistent storge
define(`__PERSISTENT_STORAGE_DEVICE__', `/dev/umass0t179')
#define(`__PERSISTENT_STORAGE_MOUNT_POINT__', `/')
define(`__PERSISTENT_STORAGE_MOUNT_OPTS__', `-t qnx6 -o sync=optional')
#define(`__PERSISTENT_STORAGE_START__', `')
#define(`__PERSISTENT_STORAGE_FILES__', `')

## Serial driver
define(`__DEVC_DRVR__',         `devc-ser8250, devc-serusb, devc-serpci')
#define(`__DEVC_OPTS__',         `')
#define(`__DEVC_DEV__',          `')

define(`__DEVC_START__', `
    ############################################################################################
    ## Serial driver
    ############################################################################################
    display_msg "Starting serial driver ..."
    devc-ser8250 -e -b115200
    waitfor /dev/ser1
')

## CAN driver

## I2C driver

## SPI driver

## Random
#define(`__RANDOM_DRVR__',       `')
#define(`__RANDOM_DRVR_OPTS__',  `')

## DMA
#define(`__DMA_DRVR__',          `')

## RTC
define(`__RTC_DRVR__',          `rtc')
define(`__RTC_OPTS__',          `hw')

define(`__RTC_START__', `dnl
    ############################################################################################
    ## RTC utility
    ############################################################################################
    display_msg "Starting RTC ..."
    __RTC_DRVR__ __RTC_OPTS__
')

## WDT kick
#define(`__WDT_DRVR__',          `')
#define(`__WDT_OPTS__',          `')
#define(`__WDT_START__',         `')

## Device tree blob
#define(`__DTB_FILE__',          `')

## Customize script
define(`__CUSTOMIZE_SCRIPT_NAME__', `/scripts/board_startup.sh')
#define(`__CUSTOMIZE_SCRIPT_START__', `')
#define(`__CUSTOMIZE_SCRIPT_FILES__', `')

## PCI driver
define(`__PCI_HW_DRVR__',       `pci_hw-AMD_x86.so, pci_hw-Intel_x86.so')
define(`__PCI_HW_MODULE__',     `pci_hw-Intel_x86.so')
define(`__PCI_OPTS__',          `--aspace-enable')

# Enable the bus master for all PCI bridge devices
define(`__PCI_BRIDGE_BUS_MASTER_ENABLE__')

## Note: disable the PCI server in template script, And start it in BOARD_EARLY_START
## Because all drivers need PCI server
define(`__PCI_START__',         `dnl')

## Board specific files
define(`__BOARD_EARLY_START__', `
__PCI_COMMON_START__
')

define(`__BOARD_LATE_START__', `
    ############################################################################################
    ## Debug console
    ############################################################################################
    ksh /proc/boot/debug-console.sh &
')

define(`__DEBUG_CONSOLE_SCRIPT__', `dnl
################################################################################################
## Debug console
################################################################################################
debug-console.sh = {
#!/bin/ksh

USB_DEVICE_LIST=/dev/shmem/usb_device.list

## check if connect the USB-Serial Controller to your target
x=1
while [ $x -le 5 ]
do
    usb > $USB_DEVICE_LIST

    if  grep "USB-Serial" $USB_DEVICE_LIST > /dev/null  || \
        grep "USB UART"   $USB_DEVICE_LIST > /dev/null  || \
        grep "usb serial" $USB_DEVICE_LIST > /dev/null
    then
        if [ ! -e /dev/serusb1 ]; then
            echo "Starting USB serial driver ..."
            devc-serusb -e -b115200
            waitfor /dev/serusb1
            TERM=qansi on -t /dev/serusb1 /bin/ksh
        fi

        break
    fi

    x=$(( $x + 1 ))
    sleep 1;
done
}')

