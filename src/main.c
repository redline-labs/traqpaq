/******************************************************************************
 *
 * Main Application for traq|paq
 *
 * - Compiler:          GNU GCC for AVR32
 * - Supported devices: traq|paq hardware version 1.4
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

struct tSystemFlags systemFlags;

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
	// Schedule the debug task
	//--------------------------
	debug_task_init();
	
	
	//--------------------------
	// Power On Method
	//--------------------------
	if( !gpio_get_pin_value(GPIO_BUTTON2) ){
		#if( TRAQPAQ_NORMAL_MODE_ON_USB )
		debug_log(DEBUG_PRIORITY_INFO, DEBUG_SENDER_BUTTON, "Forcing normal mode on USB powerup");
		systemFlags.button.powerOnMethod = POWER_ON_MODE_BUTTON;
		main_supply_on();
		#else
		debug_log(DEBUG_PRIORITY_INFO, DEBUG_SENDER_BUTTON, "Powered on via button");
		systemFlags.button.powerOnMethod = POWER_ON_MODE_BUTTON;
		#endif
		
	}else{
		debug_log(DEBUG_PRIORITY_INFO, DEBUG_SENDER_BUTTON, "Powered on via USB");
		systemFlags.button.powerOnMethod = POWER_ON_MODE_BUTTON;
		main_supply_on();
	}
	
	
	//--------------------------
	// Schedule the rest of the tasks
	//--------------------------
	buttons_task_init();
	//fuel_task_init();
	flash_task_init();
	//lcd_task_init();
	usb_task_init();
	wdt_task_init();
	gps_task_init();
	//accel_task_init();


	//--------------------------
	// Start the scheduler!
	//--------------------------	
	Enable_global_interrupt();
	vTaskStartScheduler();
	
	
	//--------------------------
	// Should never get to here!
	//--------------------------
	while (TRUE);	
}