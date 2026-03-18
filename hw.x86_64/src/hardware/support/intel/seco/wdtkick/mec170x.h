/*
 * Copyright (c) 2025, BlackBerry Limited. All rights reserved.
 *
 * BlackBerry Limited and its licensors retain all intellectual property and
 * proprietary rights in and to this software and related documentation. Any
 * use, reproduction, disclosure or distribution of this software and related
 * documentation without an express license agreement from BlackBerry Limited
 * is strictly prohibited.
 */

#ifndef MEC170X_H__
#define MEC170X_H__

#include <threads.h>
#include <stdio.h>
#include <devctl.h>
#include <sys/iomsg.h>
#include <sys/iomgr.h>

/*signal pin for WDT */

#define WDT_TRIG_PIN		 0x50	/* If 0xFF not used */

#define MAX_DELAY  		0x7FFFFFFFU
#define MAX_EVENT_TIMEOUT  	0x7FFFFFFFU
#define MAX_RESET_TIMEOUT  	0x7FFFFFFFU

/* Watch Dog Event */
#define WTD_MODE_SIGNAL				0x01
#define WTD_MODE_SYSTEM_RESET		0x02
#define WTD_MODE_PWRBTN_1SEC		0x03
#define WTD_MODE_PWRBTN_4SEC		0x04
#define WTD_MODE_COLD_RESET			0x05
#define WTD_MODE_WARM_RESET			0x06

/* Watch Dog Timeout/Delay time unit */
#define WTD_MODE_IN_MSEC				0x00
#define WTD_MODE_DELAY_IN_MINUTES		0x80
#define WTD_MODE_TIMEOUT_IN_MINUTES		0x40

/* Watch Dog trigger type */
#define WTD_MODE_ENABLE_HW_TRIGGER		0x20
#define WTD_MODE_ENABLE_SW_TRIGGER		0x10

/* Watchdog Event mode*/

#define WDT_EVENT_NONE           0
#define WDT_EVENT_PWRBTN_PULSE   1
#define WDT_EVENT_PWRBTN_FORCE   2
#define WDT_EVENT_RESET          3

#define WDTKICK_OPTIONS     "p:d:e:r:s:"
#define UNSUPPORTED			0XFF

static int32_t config_wdt(int fd_wdt, uint8_t mode_Event,
        uint8_t mode_Timeout_unit, uint8_t mode_Trigger_Type);

static int32_t start_wdt(int fd_wdt, uint32_t delayTimeout_msec,
        uint32_t eventTimeout_msec, uint32_t resetTimeout_msec);
static void fd_cleanup(int32_t fd_wdt);

#endif

