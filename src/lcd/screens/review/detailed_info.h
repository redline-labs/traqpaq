/******************************************************************************
 *
 * Review Session -> Detailed Info
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
	
	//flash_send_request(FLASH_REQUEST_READ_RECORDTABLE, &recordTable, NULL, screenArgs, TRUE, 20);
	flash_send_request(FLASH_MGR_READ_RECORDTABLE, &recordTable, NULL, screenArgs, TRUE, 20);
	//flash_send_request(FLASH_REQUEST_READ_TRACK, &trackList, NULL, recordTable.trackID, TRUE, 20);
	flash_send_request(FLASH_MGR_READ_TRACK, &trackList, NULL, recordTable.trackID, TRUE, 20);
	lcd_writeText_16x32("Track", FONT_LARGE_POINTER, LCD_MIN_X, LCD_MAX_Y-LCD_TOPBAR_THICKNESS - 32, COLOR_BLACK);
	lcd_writeText_16x32(trackList.name, FONT_LARGE_POINTER, LCD_MIN_X + 112, LCD_MAX_Y-LCD_TOPBAR_THICKNESS - 32, COLOR_BLACK);
	
	//itoa(recordData.data[0].utc, &tempString, 10)
	
	lcd_redraw_complete();
}

if( xQueueReceive(lcdButtonsManagerQueue, &button, 0) == pdTRUE ){
	switch(button){
		
		// ---------------------------------
		// Short duration button presses
		// ---------------------------------
		case(BUTTON_UP):
			menu_scrollUp(&mainMenu);
			break;
			
		case(BUTTON_DOWN):
			menu_scrollDown(&mainMenu);
			break;
			
		case(BUTTON_SELECT):
			break;
			
		case(BUTTON_BACK):
			lcd_force_redraw();
			lcd_change_screens( LCDFSM_REVIEW_SESSION );
			break;
			
			
		// ---------------------------------
		// Long duration button presses
		// ---------------------------------
		case(BUTTON_LONG_UP):
			break;
			
		case(BUTTON_LONG_DOWN):
			break;
			
		case(BUTTON_LONG_SELECT):
			lcd_force_redraw();
			lcd_change_screens( LCDFSM_POWEROFF );
			break;
			
		case(BUTTON_LONG_BACK):
			break;
	}
}