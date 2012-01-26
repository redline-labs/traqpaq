/******************************************************************************
 *
 * Menu functions
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

#include "menu.h"
#include "lcd.h"
#include FONT_LARGE_INCLUDE

struct tMenu menu_init(){
	struct tMenu menu;
	menu.numItems = 0;		// Initialize number of items in menu
	return menu;
}

void menu_addItem(struct tMenu *menu, unsigned char *itemText, unsigned short actionCode) {
	strcpy(menu->item[menu->numItems].text, itemText);
	menu->item[menu->numItems].callback = actionCode;
	
	if(menu->numItems == 0){
		// Need to draw the selection box
		lcd_drawFilledRectangle(LCD_MIN_X,
								MENU_Y_START,
								LCD_MAX_X,
								MENU_Y_START - MENU_ROW_HEIGHT,
								COLOR_REDLINERED);
								
		lcd_writeText_16x32(menu->item[menu->numItems].text,
						FONT_LARGE_POINTER,
						MENU_TEXT_X_PADDING,
						MENU_Y_START - MENU_ROW_HEIGHT + MENU_TEXT_Y_PADDING,
						COLOR_WHITE);
						
		menu->selectedIndex = 0;
		menu->item[menu->numItems].visible = TRUE;
		menu->numItems += 1;
		
	}else if( (menu->numItems < MENU_MAX_ITEMS) && (menu->numItems <= MENU_ITEMS_PER_SCREEN) ){
		lcd_writeText_16x32(menu->item[menu->numItems].text,
					FONT_LARGE_POINTER,
					MENU_TEXT_X_PADDING,
					MENU_Y_START - ((menu->numItems + 1) * MENU_ROW_HEIGHT) + MENU_TEXT_Y_PADDING,
					COLOR_BLACK);
		
		menu->item[menu->numItems].visible = TRUE;
		menu->numItems += 1;
			
	}else if( menu->numItems < MENU_MAX_ITEMS){
		lcd_writeText_16x32(menu->item[menu->numItems].text,
					FONT_LARGE_POINTER,
					MENU_TEXT_X_PADDING,
					MENU_Y_START - ((menu->numItems + 1) * MENU_ROW_HEIGHT) + MENU_TEXT_Y_PADDING,
					COLOR_BLACK);
		
		menu->item[menu->numItems].visible = FALSE;
		menu->numItems += 1;
	}
}

void menu_scrollUp(struct tMenu *menu) {
	unsigned char newIndex;
	
	if(menu->selectedIndex == 0){
		newIndex = menu->numItems - 1;
	}else{
		newIndex = menu->selectedIndex - 1;
	}
	
	// Unselect previous item
	lcd_drawFilledRectangle(LCD_MIN_X,
							MENU_Y_START - (menu->selectedIndex * MENU_ROW_HEIGHT),
							LCD_MAX_X,
							MENU_Y_START - ((menu->selectedIndex + 1) * MENU_ROW_HEIGHT),
							COLOR_WHITE);
							
	lcd_writeText_16x32(menu->item[menu->selectedIndex].text,
				FONT_LARGE_POINTER,
				MENU_TEXT_X_PADDING,
				MENU_Y_START - ((menu->selectedIndex + 1) * MENU_ROW_HEIGHT) + MENU_TEXT_Y_PADDING,
				COLOR_BLACK);
	
	// Select new item
	lcd_drawFilledRectangle(LCD_MIN_X,
							MENU_Y_START - (newIndex * MENU_ROW_HEIGHT),
							LCD_MAX_X,
							MENU_Y_START - ((newIndex + 1) * MENU_ROW_HEIGHT),
							COLOR_REDLINERED);
							
	lcd_writeText_16x32(menu->item[newIndex].text,
				FONT_LARGE_POINTER,
				MENU_TEXT_X_PADDING,
				MENU_Y_START - ((newIndex + 1) * MENU_ROW_HEIGHT) + MENU_TEXT_Y_PADDING,
				COLOR_WHITE);
	
	menu->selectedIndex = newIndex;
}

void menu_scrollDown(struct tMenu *menu) {
	unsigned char newIndex;
	
	if(menu->selectedIndex == menu->numItems - 1){
		newIndex = 0;
	}else{
		newIndex = menu->selectedIndex + 1;
	}
	
	// Unselect previous item
	lcd_drawFilledRectangle(LCD_MIN_X,
							MENU_Y_START - (menu->selectedIndex * MENU_ROW_HEIGHT),
							LCD_MAX_X,
							MENU_Y_START - ((menu->selectedIndex + 1) * MENU_ROW_HEIGHT),
							COLOR_WHITE);
							
	lcd_writeText_16x32(menu->item[menu->selectedIndex].text,
				FONT_LARGE_POINTER,
				MENU_TEXT_X_PADDING,
				MENU_Y_START - ((menu->selectedIndex + 1) * MENU_ROW_HEIGHT) + MENU_TEXT_Y_PADDING,
				COLOR_BLACK);
	
	// Select new item
	lcd_drawFilledRectangle(LCD_MIN_X,
							MENU_Y_START - (newIndex * MENU_ROW_HEIGHT),
							LCD_MAX_X,
							MENU_Y_START - ((newIndex + 1) * MENU_ROW_HEIGHT),
							COLOR_REDLINERED);
							
	lcd_writeText_16x32(menu->item[newIndex].text,
				FONT_LARGE_POINTER,
				MENU_TEXT_X_PADDING,
				MENU_Y_START - ((newIndex + 1) * MENU_ROW_HEIGHT) + MENU_TEXT_Y_PADDING,
				COLOR_WHITE);
	
	menu->selectedIndex = newIndex;
}

unsigned short menu_readCallback(struct tMenu *menu){
	return menu->item[menu->selectedIndex].callback;
}

void menu_clear(struct tMenu *menu){
	lcd_drawFilledRectangle(LCD_MIN_X, MENU_Y_START, LCD_MAX_X, LCD_MIN_Y, COLOR_WHITE);
	menu->numItems = 0;
}