/*
 * Copyright (c) 2025, BlackBerry Limited. All rights reserved.
 *
 * BlackBerry Limited and its licensors retain all intellectual property and
 * proprietary rights in and to this software and related documentation. Any
 * use, reproduction, disclosure or distribution of this software and related
 * documentation without an express license agreement from BlackBerry Limited
 * is strictly prohibited.
 */

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <devctl.h>
#include <stdio.h>
#include <errno.h>
#include <fcntl.h>
#include <sys/iomgr.h>
#include <sys/iomsg.h>
#include "mec170x.h"
#include <sys/slogcodes.h>
#include <sys/slog.h>
#include <sys/procmgr.h>
#include <secpol/secpol.h>
#include <pthread.h>
#include <hw/ecm_mec170x.h>

int main(int argc, char *argv[]) {

    int32_t opt = 0;
    int32_t priority = -1;
    uint32_t event_timeout = 30000; /* Watchdog timeout interval in milliseconds to trigger an event*/
    uint32_t initial_delay = 100; /* Initial delay for the watchdog timer in milliseconds to start event.This delay is only for First cycle*/
    uint32_t reset_timeout = 29000; /* Timeout interval in milliseconds to trigger a reset to system  */
    uint32_t sleep_period = 29000; /* default period between each kick in milliseconds */

    opt = getopt(argc, argv, WDTKICK_OPTIONS);
    errno = EOK;
    while (opt != -1) {
        switch ((char) opt) {
        case 'p':
            priority = (int32_t) strtol(optarg, NULL, 0);
            if ((errno != EOK) || (priority <= 1) || (priority >= 254)) {
                slogf(_SLOG_SETCODE(_SLOGC_CHAR, _SLOG_ERROR), _SLOG_ERROR,
                        "Invalid priority parameter");
                exit( EXIT_FAILURE);
            }
            break;

        case 'd':
            initial_delay = (uint32_t) strtoul(optarg, NULL, 0);
            if ((errno != EOK) || (initial_delay >= MAX_DELAY)) {
                slogf(_SLOG_SETCODE(_SLOGC_CHAR, _SLOG_ERROR), _SLOG_INFO,
                        "Invalid initial_delay parameter");
                exit( EXIT_FAILURE);
            }
            break;

        case 'e':
            event_timeout = (uint32_t) strtoul(optarg, NULL, 0);
            if ((errno != EOK) || (event_timeout >= MAX_EVENT_TIMEOUT)) {
                slogf(_SLOG_SETCODE(_SLOGC_CHAR, _SLOG_ERROR), _SLOG_INFO,
                        "Invalid event-timeout parameter");
                exit( EXIT_FAILURE);
            }
            break;

        case 'r':
            reset_timeout = (uint32_t) strtoul(optarg, NULL, 0);
            if ((errno != EOK) || (reset_timeout >= MAX_RESET_TIMEOUT)) {
                slogf(_SLOG_SETCODE(_SLOGC_CHAR, _SLOG_ERROR), _SLOG_INFO,
                        "Invalid reset timeout parameter");
                exit( EXIT_FAILURE);
            }
            break;

        case 's':
            sleep_period = (uint32_t) strtoul(optarg, NULL, 0);
            if (errno != EOK) {
                slogf(_SLOG_SETCODE(_SLOGC_CHAR, _SLOG_ERROR), _SLOG_ERROR,
                        "Invalid sleep-period parameter");
                exit( EXIT_FAILURE);
            }
            break;

        default:
            slogf(_SLOG_SETCODE(_SLOGC_CHAR, _SLOG_ERROR), _SLOG_ERROR,
                    "Default value is selected");
            break;
        }

        opt = getopt(argc, argv, WDTKICK_OPTIONS);
    }

    if (sleep_period >= event_timeout) {
        slogf(_SLOG_SETCODE(_SLOGC_CHAR, _SLOG_ERROR), _SLOG_ERROR,
                "Kick time is bigger or equal to timeout the watchdog will timeout before being kicked");
        exit( EXIT_FAILURE);
    }

    if (procmgr_daemon( EXIT_SUCCESS,
    PROCMGR_DAEMON_NOCLOSE | PROCMGR_DAEMON_NODEVNULL) == -1) {
        slogf(_SLOG_SETCODE(_SLOGC_CHAR, _SLOG_ERROR), _SLOG_ERROR,
                "procmgr_daemon failed ");
        exit( EXIT_FAILURE);
    }

    if (priority > 0) {
        if (pthread_setschedprio(pthread_self(), priority) != EOK) {
            slogf(_SLOG_SETCODE(_SLOGC_CHAR, _SLOG_ERROR), _SLOG_ERROR,
                    "Can't change priority");
            exit( EXIT_FAILURE);
        }
    }

    /* Open the ECM module to access the watchdog */

    int32_t fd = open(MEC_CHIP_PATHNAME, O_WRONLY);
    if (fd == -1) {
        slogf(_SLOG_SETCODE(_SLOGC_CHAR, 0), _SLOG_ERROR,
                "Failed to open the ECM node");
        exit( EXIT_FAILURE);
    }

    /* Stop the watchdog Timer */
    if (devctl(fd, DCMD_ECM_WDT_STOP, NULL, 0, NULL) == EOK) {
        slogf(_SLOG_SETCODE(_SLOGC_CHAR, _SLOG_INFO), _SLOG_INFO,
                "watchdog Timer stopped");
    } else {
        slogf(_SLOG_SETCODE(_SLOGC_CHAR, _SLOG_ERROR), _SLOG_ERROR,
                "Failed to stop watchdog Timer");
        fd_cleanup(fd);
        exit( EXIT_FAILURE);
    }

    /*Configure the watchdog Timer */
    if (config_wdt(fd, WTD_MODE_SYSTEM_RESET, WTD_MODE_IN_MSEC,
    WTD_MODE_ENABLE_SW_TRIGGER) == EOK) {
        slogf(_SLOG_SETCODE(_SLOGC_CHAR, _SLOG_INFO), _SLOG_INFO,
                "watchdog Timer configuration Done");
    } else {
        slogf(_SLOG_SETCODE(_SLOGC_CHAR, _SLOG_ERROR), _SLOG_ERROR,
                "Failed to configure watchdog Timer");
        fd_cleanup(fd);
        exit( EXIT_FAILURE);
    }

    /*Start the watchdog Timer */

    if (start_wdt(fd, initial_delay, event_timeout, reset_timeout) == 0) {
        slogf(_SLOG_SETCODE(_SLOGC_CHAR, _SLOG_INFO), _SLOG_INFO,
                "watchdog Timer Enabled\n");
    } else {
        slogf(_SLOG_SETCODE(_SLOGC_CHAR, _SLOG_INFO), _SLOG_ERROR,
                "Failed to enable the watchdog Timer");
        fd_cleanup(fd);
        exit( EXIT_FAILURE);
    }

    (void) secpol_transition_type(NULL, NULL, 0);

    /* Reload the Watchdog Timer within given Event-timeout */
    while (true) {
        if (devctl(fd, DCMD_ECM_WDT_TRIGGER, NULL, 0, NULL) != EOK) {
            slogf(_SLOG_SETCODE(_SLOGC_CHAR, _SLOG_INFO), _SLOG_INFO,
                    "watchdog Timer reloaded failed");
            break;
        }
        delay(sleep_period);
    }
    fd_cleanup(fd);
    return EXIT_FAILURE;
}

static int32_t config_wdt(int fd_wdt, uint8_t mode_Event,
        uint8_t mode_Timeout_unit, uint8_t mode_Trigger_Type) {

    wdt_access_ut wdtCfg;
    wdtCfg.signalPin = WDT_TRIG_PIN;

    switch (mode_Trigger_Type) {
    case WTD_MODE_ENABLE_SW_TRIGGER:
        wdtCfg.refreshPin = 0xFF; /* If 0xFF not used */
        break;

    case WTD_MODE_ENABLE_HW_TRIGGER:
        wdtCfg.refreshPin = UNSUPPORTED; /* If 0xFF not used */
        break;
    default:
        (void) slogf(_SLOG_SETCODE(_SLOGC_CHAR, _SLOG_INFO), _SLOG_INFO,
                "Trigger is not supported");
        break;
    }

    wdtCfg.config.eventConfig = WDT_EVENT_NONE; /* Event on first timeout */

    switch (mode_Event) {
    case WTD_MODE_SIGNAL:
        wdtCfg.config.resetConfig = WDT_EVENT_NONE; /* Event on second timeout */
        break;
    case WTD_MODE_PWRBTN_1SEC:
        wdtCfg.config.resetConfig = WDT_EVENT_PWRBTN_PULSE; /* Event on second timeout */
        break;
    case WTD_MODE_PWRBTN_4SEC:
        wdtCfg.config.resetConfig = WDT_EVENT_PWRBTN_FORCE; /* Event on second timeout */
        break;
    case WTD_MODE_SYSTEM_RESET:
        wdtCfg.config.resetConfig = WDT_EVENT_RESET; /* Event on second timeout */
        break;
    case WTD_MODE_COLD_RESET:
    case WTD_MODE_WARM_RESET:
    default:
        (void) slogf(_SLOG_SETCODE(_SLOGC_CHAR, _SLOG_INFO), _SLOG_INFO,
                "mode_Event not supported");
        break;
    }

    switch (mode_Timeout_unit) {
    case WTD_MODE_IN_MSEC:
        break;
    case WTD_MODE_TIMEOUT_IN_MINUTES:
    default:
        (void) slogf(_SLOG_SETCODE(_SLOGC_CHAR, _SLOG_INFO), _SLOG_INFO,
                "Mode_Timeout_unit not supported");
        break;
    }

    int32_t rc = devctl(fd_wdt, DCMD_ECM_WDT_CONFIG, &wdtCfg, sizeof(wdtCfg),
            NULL);
    if (rc == EOK) {
        (void) slogf(_SLOG_SETCODE(_SLOGC_CHAR, _SLOG_INFO), _SLOG_INFO,
                "watchdog configuration is successful");
    } else {
        (void) slogf(_SLOG_SETCODE(_SLOGC_CHAR, _SLOG_ERROR), _SLOG_ERROR,
                "watchdog configuration is failed");
    }
    return 0;
}

static int32_t start_wdt(int fd_wdt, uint32_t delayTimeout_msec,
        uint32_t eventTimeout_msec, uint32_t resetTimeout_msec) {

    wdt_access_ut wdtstart;
    wdtstart.delayTimeout = delayTimeout_msec;
    wdtstart.eventTimeout = eventTimeout_msec;
    wdtstart.resetTimeout = resetTimeout_msec;

    int32_t rc = devctl(fd_wdt, DCMD_ECM_WDT_START, &wdtstart, sizeof(wdtstart),
    NULL);

    if (rc == EOK) {
        (void) slogf(_SLOG_SETCODE(_SLOGC_CHAR, _SLOG_INFO), _SLOG_INFO,
                "watchdog started successful");
    } else {
        (void) slogf(_SLOG_SETCODE(_SLOGC_CHAR, _SLOG_ERROR), _SLOG_ERROR,
                "watchdog start failed");
        return EXIT_FAILURE;
    }

    return 0;
}

static void fd_cleanup(int32_t fd_wdt) {
    if (fd_wdt >= 0) {
        if (close(fd_wdt) != 0) {
            (void) slogf(_SLOG_SETCODE(_SLOGC_CHAR, _SLOG_ERROR), _SLOG_ERROR,
                    "Failed to close File Descriptor");
        } else {
            (void) slogf(_SLOG_SETCODE(_SLOGC_CHAR, _SLOG_ERROR), _SLOG_ERROR,
                    "closed File Descriptor");
        }
    }

}
