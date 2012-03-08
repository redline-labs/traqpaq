/******************************************************************************
 *
 * Charge Control
 *
 * - Compiler:          GNU GCC for AVR32
 * - Supported devices: traq|paq hardware version 1.1
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

/* TODO: Add software framework include drivers below */
#include "asf.h"
#include "hal.h"

void charge_task_init( void ){
	xTaskCreate(charge_task, configTSK_CHARGE_TASK_NAME, configTSK_CHARGE_TASK_STACK_SIZE, NULL, configTSK_CHARGE_TASK_PRIORITY, configTSK_CHARGE_TASK_HANDLE);
}

void charge_task( void *pvParameters ){

	debug_log(DEBUG_PRIORITY_INFO, DEBUG_SENDER_CHARGE, "Task Started");
	
	while( TRUE ){
		lcd_sendWidgetRequest(LCD_REQUEST_UPDATE_CHARGE, charge_state(), pdFALSE);
		vTaskDelay( (portTickType)TASK_DELAY_MS(2500) );
	}
}

void charge_setRate(unsigned char rate){
	if(rate == CHARGE_RATE_HIGH){
		#if(TRAQPAQ_HW_USB_FASTCHG_ENABLED)
		gpio_set_gpio_pin(CHARGE_RATE);
		#endif
	}else{
		gpio_clr_gpio_pin(CHARGE_RATE);
	}
}

unsigned char charge_powerGood( void ){
	return gpio_get_pin_value(CHARGE_PG);
}

unsigned char charge_state( void ){
	return (( gpio_get_pin_value(CHARGE_STAT1) << 2 ) |  ( gpio_get_pin_value(CHARGE_STAT2) << 1 ) | ( gpio_get_pin_value(CHARGE_PG) << 0 ));
}