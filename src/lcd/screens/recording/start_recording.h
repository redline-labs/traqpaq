/******************************************************************************
 *
 * Start Recording Session
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

if(lcd_redraw_required()){
	menu_clear(&mainMenu);
	
	gpsRequest.command = GPS_REQUEST_START_RECORDING;
	xQueueSend(gpsManagerQueue, &gpsRequest, 20);
	
	lcd_writeText_16x32("Recording!", FONT_LARGE_POINTER, LCD_MIN_X, LCD_MAX_Y - LCD_TOPBAR_THICKNESS - 64, COLOR_BLACK);
	lcd_writeText_16x32("SELECT to stop!", FONT_LARGE_POINTER, LCD_MIN_X, LCD_MAX_Y - LCD_TOPBAR_THICKNESS - 96, COLOR_BLACK);
	
	lcd_redraw_complete();
}

if( xQueueReceive(lcdButtonsManagerQueue, &button, 0) == pdTRUE ){
	switch(button){
		
		// ---------------------------------
		// Short duration button presses
		// ---------------------------------
		case(BUTTON_UP):
			asm("nop");
			break;
			
		case(BUTTON_DOWN):
			asm("nop");
			break;
			
		case(BUTTON_SELECT):
			gpsRequest.command = GPS_REQUEST_STOP_RECORDING;
			xQueueSend(gpsManagerQueue, &gpsRequest, 20);
			
			dataflashRequest.command = DFMAN_REQUEST_END_CURRENT_RECORD;
			dataflashRequest.resume = xTaskGetCurrentTaskHandle();
			xQueueSend(dataflashManagerQueue, &dataflashRequest, 20);
			vTaskSuspend(NULL);
		
			lcd_force_redraw();
			lcd_change_screens( LCDFSM_MAINMENU );
			break;
			
		case(BUTTON_BACK):
			asm("nop");
			break;
			
			
		// ---------------------------------
		// Long duration button presses
		// ---------------------------------
		case(BUTTON_LONG_UP):
			asm("nop");
			break;
			
		case(BUTTON_LONG_DOWN):
			asm("nop");
			break;
			
		case(BUTTON_LONG_SELECT):
			asm("nop");
			break;
			
		case(BUTTON_LONG_BACK):
			asm("nop");
			break;
	}
}