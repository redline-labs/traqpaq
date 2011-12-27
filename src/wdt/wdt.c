/*
 * wdt.c
 *
 * Created: 11/20/2011 6:17:35 PM
 *  Author: Ryan
 */ 

#include "asf.h"
#include "drivers.h"

void wdt_task_init(){
	xTaskCreate(wdt_task, configTSK_WATCHDOG_TASK_NAME, configTSK_WATCHDOG_TASK_STACK_SIZE, NULL, configTSK_WATCHDOG_TASK_PRIORITY, NULL);
}

void wdt_task(){
	portTickType xLastWakeTime;
	
	wdt_opt_t wdt_options = {
		.us_timeout_period = WATCHDOG_TIMEOUT_US  // TimeOut Value = 100ms
	};

	xLastWakeTime = xTaskGetTickCount();
	
	wdt_enable(&wdt_options);
	
	while(1){
		wdt_clear();
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