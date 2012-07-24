/******************************************************************************
 *
 * Watchdog Routines
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
#include "asf.h"
#include "hal.h"

xQueueHandle wdtManagerQueue;

void wdt_task_init(){
	xTaskCreate(wdt_task, configTSK_WATCHDOG_TASK_NAME, configTSK_WATCHDOG_TASK_STACK_SIZE, NULL, configTSK_WATCHDOG_TASK_PRIORITY, configTSK_WATCHDOG_TASK_HANDLE);
}

void wdt_task( void *pvParameters ){
	portTickType xLastWakeTime;
	wdt_opt_t wdt_options;
	struct tWatchdogRequest request;
	unsigned char gpsShutdown = FALSE, usbShutdown = FALSE, dataflashShutdown = FALSE, fuelShutdown = FALSE;
	
	debug_log(DEBUG_PRIORITY_INFO, DEBUG_SENDER_WDT, "Task Started");
	
	wdtManagerQueue = xQueueCreate(WATCHDOG_QUEUE_SIZE, sizeof(request));
	
	if( wdt_triggered() ){
		debug_log(DEBUG_PRIORITY_WARNING, DEBUG_SENDER_WDT, "Reset from watchdog");
	}
	
	xLastWakeTime = xTaskGetTickCount();
	
	wdt_options.us_timeout_period = WATCHDOG_TIMEOUT_US;
	wdt_enable(&wdt_options);
	
	while(1){
		wdt_clear();
		
		if( xQueueReceive(wdtManagerQueue, &request, pdFALSE) == pdTRUE ){
			switch(request.command){
				case(WDT_REQUEST_POWEROFF):
					debug_log(DEBUG_PRIORITY_INFO, DEBUG_SENDER_WDT, "Shutdown requested");
					fuel_send_request(FUEL_MGR_REQUEST_SHUTDOWN, NULL, NULL, NULL, NULL);
					//flash_send_request(FLASH_REQUEST_SHUTDOWN, NULL, NULL, NULL, NULL, NULL);
					flash_send_request(FLASH_MGR_REQUEST_SHUTDOWN, NULL, NULL, NULL, NULL, NULL);
					gps_send_request(GPS_MGR_REQUEST_SHUTDOWN, NULL, NULL, NULL, pdFALSE);
					
					debug_log(DEBUG_PRIORITY_INFO, DEBUG_SENDER_WDT, "Going down!");
					wdt_clear();	// Kick the watchdog one more time to allow debug messages to be sent
					vTaskSuspend(NULL);
					break;
			}
		}
		
		vTaskDelayUntil( &xLastWakeTime, ( WATCHDOG_UPDATE_INTERVAL_MS / portTICK_RATE_MS ) );
	}
}

unsigned char wdt_triggered(void){
	reset_cause_t resetCause;
	
	resetCause = reset_cause_get_causes();
	
	if(resetCause &= CHIP_RESET_CAUSE_WDT){
		return TRUE;
	}else{
		return FALSE;
	}
}

unsigned char wdt_send_request(enum tWatchdogCommand command, unsigned char data){
	struct tWatchdogRequest request;
	
	request.command = command;
	request.data = data;
	
	return xQueueSend(wdtManagerQueue, &request, portMAX_DELAY);
}