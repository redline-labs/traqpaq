/******************************************************************************
 *
 * Options -> Display -> Screen Brightness
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
	menu_addItem(&mainMenu, "100%",	255);
	menu_addItem(&mainMenu,  "80%",	70);
	menu_addItem(&mainMenu,  "60%",	46);
	menu_addItem(&mainMenu,  "40%",	26);
	menu_addItem(&mainMenu,  "20%",	10);
	
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
			backlight_updateDuty( menu_readCallback(&mainMenu) );
			lcd_force_redraw();
			lcd_change_screens( LCDFSM_DISPLAY );
			break;
			
		case(BUTTON_BACK):
			lcd_force_redraw();
			lcd_change_screens( LCDFSM_OPTIONS );
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