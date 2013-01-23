/******************************************************************************
 *
 * Help -> Module Status
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

if(lcd_redraw_required()){
	menu_clear(&mainMenu);
	
	lcd_writeText_8x16("Mode: ", FONT_SMALL_POINTER, LCD_MIN_X + 5, LCD_MAX_Y - LCD_TOPBAR_THICKNESS - 20, COLOR_BLACK);
	lcd_writeText_8x16(itoa(gpsInfo.status, &tempString, 10, FALSE), FONT_SMALL_POINTER, LCD_MIN_X + 305, LCD_MAX_Y - LCD_TOPBAR_THICKNESS - 20, COLOR_RED);
	
	lcd_writeText_8x16("Satellites: ", FONT_SMALL_POINTER, LCD_MIN_X + 5, LCD_MAX_Y - LCD_TOPBAR_THICKNESS - 36, COLOR_BLACK);
	lcd_writeText_8x16(itoa(gpsInfo.satellites, &tempString, 10, FALSE), FONT_SMALL_POINTER, LCD_MIN_X + 305, LCD_MAX_Y - LCD_TOPBAR_THICKNESS - 36, COLOR_RED);
	
	lcd_writeText_8x16("Serial Number: ", FONT_SMALL_POINTER, LCD_MIN_X + 5, LCD_MAX_Y - LCD_TOPBAR_THICKNESS - 52, COLOR_BLACK);
	lcd_writeText_8x16(itoa(gpsInfo.serial_number, &tempString, 10, FALSE), FONT_SMALL_POINTER, LCD_MIN_X + 305, LCD_MAX_Y - LCD_TOPBAR_THICKNESS - 52, COLOR_RED);
	
	lcd_writeText_8x16("Part Number: ", FONT_SMALL_POINTER, LCD_MIN_X + 5, LCD_MAX_Y - LCD_TOPBAR_THICKNESS - 68, COLOR_BLACK);
	lcd_writeText_8x16(gpsInfo.part_number, FONT_SMALL_POINTER, LCD_MIN_X + 305, LCD_MAX_Y - LCD_TOPBAR_THICKNESS - 68, COLOR_RED);
	
	lcd_writeText_8x16("Software Version: ", FONT_SMALL_POINTER, LCD_MIN_X + 5, LCD_MAX_Y - LCD_TOPBAR_THICKNESS - 84, COLOR_BLACK);
	lcd_writeText_8x16(gpsInfo.sw_version, FONT_SMALL_POINTER, LCD_MIN_X + 305, LCD_MAX_Y - LCD_TOPBAR_THICKNESS - 84, COLOR_RED);
	
	lcd_writeText_8x16("Status: ", FONT_SMALL_POINTER, LCD_MIN_X + 5, LCD_MAX_Y - LCD_TOPBAR_THICKNESS - 100, COLOR_BLACK);
	lcd_writeText_8x16(itoa(gpsInfo.status, &tempString, 10, FALSE), FONT_SMALL_POINTER, LCD_MIN_X + 305, LCD_MAX_Y - LCD_TOPBAR_THICKNESS - 100, COLOR_RED);
	
	lcd_writeText_8x16("Checksum Errors: ", FONT_SMALL_POINTER, LCD_MIN_X + 5, LCD_MAX_Y - LCD_TOPBAR_THICKNESS - 116, COLOR_BLACK);
	lcd_writeText_8x16(itoa(gpsInfo.error.checksumErrors, &tempString, 10, FALSE), FONT_SMALL_POINTER, LCD_MIN_X + 305, LCD_MAX_Y - LCD_TOPBAR_THICKNESS - 116, COLOR_RED);
	
	lcd_writeText_8x16("GGA Msg Timeouts: ", FONT_SMALL_POINTER, LCD_MIN_X + 5, LCD_MAX_Y - LCD_TOPBAR_THICKNESS - 132, COLOR_BLACK);
	lcd_writeText_8x16(itoa(gpsInfo.error.ggaMsgTimeouts, &tempString, 10, FALSE), FONT_SMALL_POINTER, LCD_MIN_X + 305, LCD_MAX_Y - LCD_TOPBAR_THICKNESS - 132, COLOR_RED);
	
	lcd_writeText_8x16("RMC Msg Timeouts: ", FONT_SMALL_POINTER, LCD_MIN_X + 5, LCD_MAX_Y - LCD_TOPBAR_THICKNESS - 148, COLOR_BLACK);
	lcd_writeText_8x16(itoa(gpsInfo.error.rmcMsgTimeouts, &tempString, 10, FALSE), FONT_SMALL_POINTER, LCD_MIN_X + 305, LCD_MAX_Y - LCD_TOPBAR_THICKNESS - 148, COLOR_RED);
	
	lcd_writeText_8x16("Reset Count: ", FONT_SMALL_POINTER, LCD_MIN_X + 5, LCD_MAX_Y - LCD_TOPBAR_THICKNESS - 164, COLOR_BLACK);
	lcd_writeText_8x16(itoa(gpsInfo.error.resetCount, &tempString, 10, FALSE), FONT_SMALL_POINTER, LCD_MIN_X + 305, LCD_MAX_Y - LCD_TOPBAR_THICKNESS - 164, COLOR_RED);
	
	lcd_writeText_8x16("Rx Data Errors: ", FONT_SMALL_POINTER, LCD_MIN_X + 5, LCD_MAX_Y - LCD_TOPBAR_THICKNESS - 180, COLOR_BLACK);
	lcd_writeText_8x16(itoa(gpsInfo.error.rxDataError, &tempString, 10, FALSE), FONT_SMALL_POINTER, LCD_MIN_X + 305, LCD_MAX_Y - LCD_TOPBAR_THICKNESS - 180, COLOR_RED);
	
	lcd_writeText_8x16("Unrecognized Msg Errors: ", FONT_SMALL_POINTER, LCD_MIN_X + 5, LCD_MAX_Y - LCD_TOPBAR_THICKNESS - 196, COLOR_BLACK);
	lcd_writeText_8x16(itoa(gpsInfo.error.unrecognizedMsgs, &tempString, 10, FALSE), FONT_SMALL_POINTER, LCD_MIN_X + 305, LCD_MAX_Y - LCD_TOPBAR_THICKNESS - 196, COLOR_RED);
	
	lcd_redraw_complete();
	lcd_resetRedrawTimer();
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
			break;
			
		case(BUTTON_BACK):
			lcd_stopRedrawTimer();
			lcd_force_redraw();
			lcd_change_screens( LCDFSM_HELP );
			break;
			
			
		// ---------------------------------
		// Long duration button presses
		// ---------------------------------
		case(BUTTON_LONG_UP):
			break;
			
		case(BUTTON_LONG_DOWN):
			break;
			
		case(BUTTON_LONG_SELECT):
			lcd_stopRedrawTimer();
			lcd_force_redraw();
			lcd_change_screens( LCDFSM_POWEROFF );
			break;
			
		case(BUTTON_LONG_BACK):
			break;
	}
}