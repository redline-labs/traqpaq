/******************************************************************************
 *
 * Main Application for traq|paq
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

// ------------------------------------------------------------
// Includes
// ------------------------------------------------------------
#include "asf.h"
#include "drivers.h"

#include "conf_usb.h"
#include "usb_task.h"
#include "device_template_task.h"


// ------------------------------------------------------------
// Main
// ------------------------------------------------------------
int main( void ){
	unsigned short temp = 0;
	
	// Initialization ---------------------------------------------
	Disable_global_interrupt();
	Disable_global_exception();
	
	wdt_disable();
	board_init();
	
	// Check to see if we got reset from a watchdog timeout
	if( wdt_triggered() ){
		// Dang! we did!
		asm("nop");
	}
	
	// Schedule Tasks ---------------------------------------------
	#if( TRAQPAQ_HW_EBI_ENABLED )
	lcd_task_init();
	#endif
	
	#if( TRAQPAQ_HW_USB_ENABLED )
	//usb_task_init();
	//device_template_task_init();
	#endif
	
	#if( TRAQPAQ_HW_SPI_ENABLED )
	//dataflash_task_init();
	#endif
	
	#if( TRAQPAQ_HW_TWI_ENABLED )
	//fuel_task_init();
	#endif
	
	#if( TRAQPAQ_HW_USART_ENABLED )
	//gps_task_init();
	#endif
	
	#if( TRAQPAQ_HW_WDT_ENABLED )
	//wdt_task_init();
	#endif
	
	//charge_task_init();

	// Start the scheduler! ---------------------------------------
	vTaskStartScheduler();

	asm("nop");
	
	while (true);		// Should never reach this!
}