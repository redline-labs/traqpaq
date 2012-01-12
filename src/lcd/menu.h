/******************************************************************************
 *
 * Menu defines
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


#ifndef MENU_H_
#define MENU_H_

#include "string.h"

#define MENU_MAX_ITEM_STRLEN		20										// Max number of characters for menu item text
#define MENU_MAX_ITEMS				5										// Max number of items per menu

#define MENU_ROW_HEIGHT				40										// Height of menu items
#define MENU_Y_START				(LCD_MAX_Y-LCD_TOPBAR_THICKNESS-1)		// Y Position to use as top of menu items
#define MENU_TEXT_X_PADDING			6										// X Padding for menu item text
#define MENU_TEXT_Y_PADDING			5										// Y Padding for menu item text

struct tMenu menu_create(unsigned char *title, unsigned char *font);
void menu_addItem(struct tMenu *menu, unsigned char *itemText, unsigned char actionCode);
void menu_scrollUp(struct tMenu *menu);
void menu_scrollDown(struct tMenu *menu);

typedef struct tMenuItem {
	unsigned char text[MENU_MAX_ITEM_STRLEN];
	unsigned char callback; //TO DO: Implement call backs
};

typedef struct tMenu {
	unsigned char numItems;
	unsigned char selectedIndex;
	unsigned char title[MENU_MAX_ITEM_STRLEN];
	struct tMenuItem item[MENU_MAX_ITEMS];
};


#endif /* MENU_H_ */