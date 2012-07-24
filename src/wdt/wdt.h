/******************************************************************************
 *
 * Watchdog defines
 *
 * - Compiler:          GNU GCC for AVR32
 * - Supported devices: traq|paq hardware version 1.2
 * - AppNote:			N/A
 *
 * - Last Author:		Ryan David ( ryan.david@redline-electronics.com )
 *
 *
 * Copyright (c) 2012 Redline Electronics LLC.
 *
 * This file is part of traq|paq.
 *
 * traq|paq is free software: you can redistribute it and/or modify it under
 * the terms of the GNU General Public License as published by the Free
 * Software Foundation, either version 3 of the License, or (at your option)
 * any later version.
 *
 * traq|paq is distributed in the hope that it will be useful, but WITHOUT ANY
 * WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS
 * FOR A PARTICULAR PURPOSE. See the GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License along
 * with traq|paq. If not, see http://www.gnu.org/licenses/.
 *
 ******************************************************************************/


#ifndef WDT_H_
#define WDT_H_

#define WATCHDOG_TIMEOUT_US			100000	// 100ms Watchdog Timeout
#define WATCHDOG_UPDATE_INTERVAL_MS	90		// 90ms Watchdog clear interval

#define WATCHDOG_QUEUE_SIZE			5

enum tWatchdogCommand {
	WDT_REQUEST_POWEROFF,
	WDT_REQUEST_DATAFLASH_SHUTDOWN_COMPLETE,
	WDT_REQUEST_GPS_SHUTDOWN_COMPLETE,
	WDT_REQUEST_LCD_SHUTDOWN_COMPLETE,
	WDT_REQUEST_USB_SHUTDOWN_COMPLETE,
	WDT_REQUEST_FUEL_SHUTDOWN_COMPLETE,
};

struct tWatchdogRequest {
	enum tWatchdogCommand command;
	unsigned char data;
};

void wdt_task_init(void);
void wdt_task( void *pvParameters );

unsigned char wdt_triggered(void);
unsigned char wdt_send_request(enum tWatchdogCommand command, unsigned char data);


#endif /* WDT_H_ */