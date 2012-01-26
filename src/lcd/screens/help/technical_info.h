/******************************************************************************
 *
 * Help -> Technical Info
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
	
	lcd_writeText_8x16("Software Version", FONT_SMALL_POINTER, LCD_MIN_X + 5, LCD_MAX_Y - LCD_TOPBAR_THICKNESS - 20, COLOR_BLACK);
	lcd_writeText_8x16(TRAQPAQ_SW_LEVEL_ASCII, FONT_SMALL_POINTER, LCD_MIN_X + 307, LCD_MAX_Y - LCD_TOPBAR_THICKNESS - 20, COLOR_RED);
	
	lcd_writeText_8x16("Software Build Date", FONT_SMALL_POINTER, LCD_MIN_X + 5, LCD_MAX_Y - LCD_TOPBAR_THICKNESS - 36, COLOR_BLACK);
	lcd_writeText_8x16(TRAQPAQ_SW_BUILD_DATE, FONT_SMALL_POINTER, LCD_MIN_X + 307, LCD_MAX_Y - LCD_TOPBAR_THICKNESS - 36, COLOR_RED);
	
	lcd_writeText_8x16("CPU Frequency", FONT_SMALL_POINTER, LCD_MIN_X + 5, LCD_MAX_Y - LCD_TOPBAR_THICKNESS - 52, COLOR_BLACK);
	lcd_writeText_8x16(APPL_CPU_SPEED_ASCII, FONT_SMALL_POINTER, LCD_MIN_X + 307, LCD_MAX_Y - LCD_TOPBAR_THICKNESS - 52, COLOR_RED);
	
	lcd_writeText_8x16("TRAQPAQ_HW_WDT_ENABLED", FONT_SMALL_POINTER, LCD_MIN_X + 5, LCD_MAX_Y - LCD_TOPBAR_THICKNESS - 68, COLOR_BLACK);
	lcd_writeText_8x16(lcd_flagToASCII(TRAQPAQ_HW_WDT_ENABLED), FONT_SMALL_POINTER, LCD_MIN_X + 307, LCD_MAX_Y - LCD_TOPBAR_THICKNESS - 68, COLOR_RED);
	
	lcd_writeText_8x16("TRAQPAQ_HW_PWM_ENABLED", FONT_SMALL_POINTER, LCD_MIN_X + 5, LCD_MAX_Y - LCD_TOPBAR_THICKNESS - 84, COLOR_BLACK);
	lcd_writeText_8x16(lcd_flagToASCII(TRAQPAQ_HW_PWM_ENABLED), FONT_SMALL_POINTER, LCD_MIN_X + 307, LCD_MAX_Y - LCD_TOPBAR_THICKNESS - 84, COLOR_RED);
	
	lcd_writeText_8x16("TRAQPAQ_HW_ADC_ENABLED", FONT_SMALL_POINTER, LCD_MIN_X + 5, LCD_MAX_Y - LCD_TOPBAR_THICKNESS - 100, COLOR_BLACK);
	lcd_writeText_8x16(lcd_flagToASCII(TRAQPAQ_HW_ADC_ENABLED), FONT_SMALL_POINTER, LCD_MIN_X + 307, LCD_MAX_Y - LCD_TOPBAR_THICKNESS - 100, COLOR_RED);
	
	lcd_writeText_8x16("TRAQPAQ_HW_GPS_ENABLED", FONT_SMALL_POINTER, LCD_MIN_X + 5, LCD_MAX_Y - LCD_TOPBAR_THICKNESS - 116, COLOR_BLACK);
	lcd_writeText_8x16(lcd_flagToASCII(TRAQPAQ_HW_GPS_ENABLED), FONT_SMALL_POINTER, LCD_MIN_X + 307, LCD_MAX_Y - LCD_TOPBAR_THICKNESS - 116, COLOR_RED);
	
	lcd_writeText_8x16("TRAQPAQ_HW_SPI_ENABLED", FONT_SMALL_POINTER, LCD_MIN_X + 5, LCD_MAX_Y - LCD_TOPBAR_THICKNESS - 132, COLOR_BLACK);
	lcd_writeText_8x16(lcd_flagToASCII(TRAQPAQ_HW_SPI_ENABLED), FONT_SMALL_POINTER, LCD_MIN_X + 307, LCD_MAX_Y - LCD_TOPBAR_THICKNESS - 132, COLOR_RED);
	
	lcd_writeText_8x16("TRAQPAQ_HW_TWI_ENABLED", FONT_SMALL_POINTER, LCD_MIN_X + 5, LCD_MAX_Y - LCD_TOPBAR_THICKNESS - 148, COLOR_BLACK);
	lcd_writeText_8x16(lcd_flagToASCII(TRAQPAQ_HW_TWI_ENABLED), FONT_SMALL_POINTER, LCD_MIN_X + 307, LCD_MAX_Y - LCD_TOPBAR_THICKNESS - 148, COLOR_RED);
	
	lcd_writeText_8x16("TRAQPAQ_HW_EXINT_ENABLED", FONT_SMALL_POINTER, LCD_MIN_X + 5, LCD_MAX_Y - LCD_TOPBAR_THICKNESS - 164, COLOR_BLACK);
	lcd_writeText_8x16(lcd_flagToASCII(TRAQPAQ_HW_EXINT_ENABLED), FONT_SMALL_POINTER, LCD_MIN_X + 307, LCD_MAX_Y - LCD_TOPBAR_THICKNESS - 164, COLOR_RED);
	
	lcd_writeText_8x16("TRAQPAQ_HW_USB_ENABLED", FONT_SMALL_POINTER, LCD_MIN_X + 5, LCD_MAX_Y - LCD_TOPBAR_THICKNESS - 180, COLOR_BLACK);
	lcd_writeText_8x16(lcd_flagToASCII(TRAQPAQ_HW_USB_ENABLED), FONT_SMALL_POINTER, LCD_MIN_X + 307, LCD_MAX_Y - LCD_TOPBAR_THICKNESS - 180, COLOR_RED);
	
	lcd_writeText_8x16("TRAQPAQ_HW_DEBUG_ENABLED", FONT_SMALL_POINTER, LCD_MIN_X + 5, LCD_MAX_Y - LCD_TOPBAR_THICKNESS - 196, COLOR_BLACK);
	lcd_writeText_8x16(lcd_flagToASCII(TRAQPAQ_HW_DEBUG_ENABLED), FONT_SMALL_POINTER, LCD_MIN_X + 307, LCD_MAX_Y - LCD_TOPBAR_THICKNESS - 196, COLOR_RED);
	
	lcd_writeText_8x16("TRAQPAQ_HW_PLL_ENABLED", FONT_SMALL_POINTER, LCD_MIN_X + 5, LCD_MAX_Y - LCD_TOPBAR_THICKNESS - 212, COLOR_BLACK);
	lcd_writeText_8x16(lcd_flagToASCII(TRAQPAQ_HW_PLL_ENABLED), FONT_SMALL_POINTER, LCD_MIN_X + 307, LCD_MAX_Y - LCD_TOPBAR_THICKNESS - 212, COLOR_RED);
	
	
	lcd_redraw_complete();
}

if( xQueueReceive(queueLCDmenu, &button, 0) == pdTRUE ){
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
			lcd_force_redraw();
			lcd_change_screens( LCDFSM_HELP );
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