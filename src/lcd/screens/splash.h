/******************************************************************************
 *
 * Main Menu
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
	
	// Set up when the splash screen duration
	tickCount = xTaskGetTickCount() + (portTickType)TASK_DELAY_MS(LCD_SPLASH_SCREEN_TIME);
	
	lcd_displayImage(&traqpaq_logo, 350, 65, TRAQPAQ_LOGO_X, TRAQPAQ_LOGO_Y);
	
	lcd_redraw_complete();
}

if( xTaskGetTickCount() >= tickCount ){
	// Flush the buttons buffer
	xQueueReceive(lcdButtonsManagerQueue, &button, pdFALSE);

	lcd_force_redraw();
	lcd_change_screens( LCDFSM_MAINMENU );
}