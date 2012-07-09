/******************************************************************************
 *
 * Start Recording Session
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
	lastSecond = 0;
	lastMinute = 0;
	lastHour = 0;
	
	menu_clear(&mainMenu);
	
	// Keep out of peripheral box area!
	lcd_writeText_16x32("Recording!", FONT_LARGE_POINTER, LCD_MIN_X + 50, LCD_MAX_Y - LCD_TOPBAR_THICKNESS - 64, COLOR_BLACK);
	lcd_writeText_16x32("SELECT to stop!", FONT_LARGE_POINTER, LCD_MIN_X + 50, LCD_MAX_Y - LCD_TOPBAR_THICKNESS - 96, COLOR_BLACK);
	
	lapHourLabel = lcd_createLabel("00", FONT_LARGE_POINTER, LCD_MIN_X + 50, LCD_MAX_Y - LCD_TOPBAR_THICKNESS - 160, 32, 32, COLOR_BLACK, COLOR_WHITE);
	lapMinuteLabel = lcd_createLabel("00", FONT_LARGE_POINTER, LCD_MIN_X + 98, LCD_MAX_Y - LCD_TOPBAR_THICKNESS - 160, 32, 32, COLOR_BLACK, COLOR_WHITE);
	lapSecondLabel = lcd_createLabel("00", FONT_LARGE_POINTER, LCD_MIN_X + 146, LCD_MAX_Y - LCD_TOPBAR_THICKNESS - 160, 32, 32, COLOR_BLACK, COLOR_WHITE);
	lapMilliLabel = lcd_createLabel("0", FONT_LARGE_POINTER, LCD_MIN_X + 194, LCD_MAX_Y - LCD_TOPBAR_THICKNESS - 160, 16, 32, COLOR_BLACK, COLOR_WHITE);
	
	oldLapHourLabel = lcd_createLabel("00", FONT_LARGE_POINTER, LCD_MIN_X + 50, LCD_MAX_Y - LCD_TOPBAR_THICKNESS - 192, 32, 32, COLOR_BLACK, COLOR_WHITE);
	oldLapMinuteLabel = lcd_createLabel("00", FONT_LARGE_POINTER, LCD_MIN_X + 98, LCD_MAX_Y - LCD_TOPBAR_THICKNESS - 192, 32, 32, COLOR_BLACK, COLOR_WHITE);
	oldLapSecondLabel = lcd_createLabel("00", FONT_LARGE_POINTER, LCD_MIN_X + 146, LCD_MAX_Y - LCD_TOPBAR_THICKNESS - 192, 32, 32, COLOR_BLACK, COLOR_WHITE);
	oldLapMilliLabel = lcd_createLabel("0", FONT_LARGE_POINTER, LCD_MIN_X + 194, LCD_MAX_Y - LCD_TOPBAR_THICKNESS - 192, 16, 32, COLOR_BLACK, COLOR_WHITE);
	
	gps_send_request(GPS_REQUEST_START_RECORDING, NULL, NULL, pdFALSE, pdTRUE);
	backlight_stopTimers();
	
	lcd_redraw_complete();
}

if( xQueueReceive(lcdButtonsManagerQueue, &button, pdFALSE) == pdTRUE ){
	switch(button){
		
		// ---------------------------------
		// Short duration button presses
		// ---------------------------------
		case(BUTTON_UP):
			break;
			
		case(BUTTON_DOWN):
			break;
			
		case(BUTTON_SELECT):
			gps_send_request(GPS_REQUEST_STOP_RECORDING, NULL, NULL, pdFALSE, pdTRUE);
			backlight_resetTimer();
			lcd_force_redraw();
			lcd_change_screens( LCDFSM_MAINMENU );
			break;
			
		case(BUTTON_BACK):
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