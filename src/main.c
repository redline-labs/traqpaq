/******************************************************************************
 *
 * Main Application for traq|paq
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

// ------------------------------------------------------------
// Main
// ------------------------------------------------------------
int main( void ){	
	//--------------------------
	// Initialization
	//--------------------------
	wdt_disable();
	Disable_global_exception();
	Disable_global_interrupt();
	
	INTC_init_interrupts();
	board_init();
	
	
	//--------------------------
	// Schedule the tasks
	//--------------------------
	#if( TRAQPAQ_DEBUG_ENABLED )
	debug_task_init();
	#endif
	
	#if( TRAQPAQ_NORMAL_MODE_ON_USB == FALSE )
	if( !gpio_get_pin_value(GPIO_BUTTON2) ){
		
		debug_log(DEBUG_PRIORITY_INFO, DEBUG_SENDER_EXTINT, "Powered on via USB");
		
		flash_task_init();
		lcd_task_init(TASK_MODE_USB);
		usb_task_init();
		fuel_task_init();
		buttons_task_init(TASK_MODE_USB);
		
	}else{
	#endif

		main_supply_on();
		
		#if( TRAQPAQ_NORMAL_MODE_ON_USB )
		debug_log(DEBUG_PRIORITY_INFO, DEBUG_SENDER_EXTINT, "Forcing normal mode on USB powerup");
		#else
		debug_log(DEBUG_PRIORITY_INFO, DEBUG_SENDER_EXTINT, "Powered on via button");
		#endif
		
		flash_task_init();
		lcd_task_init(TASK_MODE_NORMAL);
		usb_task_init();
		fuel_task_init();
		buttons_task_init(TASK_MODE_NORMAL);
		wdt_task_init();
		
		#if( TRAQPAQ_GPS_EXTERNAL_LOGGING )
		gpio_set_gpio_pin(GPS_RESET);
		#else
		gps_task_init();
		#endif
		
	#if( TRAQPAQ_NORMAL_MODE_ON_USB == FALSE )
	}
	#endif

	//--------------------------
	// Start the scheduler!
	//--------------------------
	Enable_global_interrupt();
	vTaskStartScheduler();
	
	
	//--------------------------
	// Should never get to here!
	//--------------------------
	debug_log(DEBUG_PRIORITY_CRITICAL, DEBUG_SENDER_WDT, "Scheduler Failed");
	while (TRUE);	
}