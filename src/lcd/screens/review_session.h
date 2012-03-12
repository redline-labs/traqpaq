/******************************************************************************
 *
 * Review Session
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
	
	responseU8 = 0;
	
	/*while( responseU8 < RECORDS_TOTAL_POSSIBLE ){
		dataflash_send_request(DFMAN_REQUEST_READ_RECORDTABLE, &recordTable, NULL, responseU8, TRUE, 20);
		if(recordTable.recordEmpty) break;
		
		dataflash_send_request(DFMAN_REQUEST_READ_RECORDDATA, &recordData, NULL, (recordTable.startAddress - DATAFLASH_ADDR_RECORDDATA_START)/DATAFLASH_PAGE_SIZE, TRUE, pdFALSE);
		
		menu_addItem(&mainMenu, itoa(recordData.data[0].utc, &tempString, 10), responseU8);
		
		responseU8++;
	}*/
	
	if(responseU8 == 0){
		menu_addItem(&mainMenu, "No Items",	LCDFSM_MAINMENU);
	}		
	
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
			lcd_force_redraw();
			//screenArgs = menu_readCallback(&mainMenu);
			//lcd_change_screens( LCDFSM_REVIEW_DETAILED_INFO );
			lcd_change_screens( LCDFSM_MAINMENU );
			break;
			
		case(BUTTON_BACK):
			lcd_force_redraw();
			lcd_change_screens( LCDFSM_MAINMENU );
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