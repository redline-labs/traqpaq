/******************************************************************************
 *
 * Main Menu -> Options -> Memory -> Erase Confirmed
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

if(lcd_redraw_required()){
	menu_clear(&mainMenu);
	
	lcd_writeText_16x32("Working...", FONT_LARGE_POINTER, 50, LCD_MAX_Y - LCD_TOPBAR_THICKNESS - 64, COLOR_BLACK);
	flash_send_request(FLASH_REQUEST_ERASE_RECORDED_DATA, NULL, NULL, NULL, TRUE, pdFALSE);
	lcd_writeText_16x32("Done!", FONT_LARGE_POINTER, 50, LCD_MAX_Y - LCD_TOPBAR_THICKNESS - 96, COLOR_GREEN);
	
	lcd_redraw_complete();
}

if( xQueueReceive(lcdButtonsManagerQueue, &button, 0) == pdTRUE ){
	switch(button){
		
		// ---------------------------------
		// Short duration button presses
		// ---------------------------------
		case(BUTTON_UP):
			break;
			
		case(BUTTON_DOWN):
			break;
			
		case(BUTTON_SELECT):
			lcd_force_redraw();
			lcd_change_screens( LCDFSM_OPTIONS_MEMORY );
			break;
			
		case(BUTTON_BACK):
			lcd_force_redraw();
			lcd_change_screens( LCDFSM_OPTIONS_MEMORY );
			break;
			
			
		// ---------------------------------
		// Long duration button presses
		// ---------------------------------
		case(BUTTON_LONG_UP):
			break;
			
		case(BUTTON_LONG_DOWN):
			break;
			
		case(BUTTON_LONG_SELECT):
			break;
			
		case(BUTTON_LONG_BACK):
			break;
	}
}