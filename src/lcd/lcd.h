/******************************************************************************
 *
 * LCD Defines
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

#ifndef LCD_H_
#define LCD_H_

#include <conf_lcd.h>

extern struct tLCDTopBar topBar;

#define lcd_flagToASCII(flag)		(flag == TRUE ? "TRUE" : "FALSE")

#define LCD_VERSION					"1.00"

#define LCD_MIN_Y					0
#define LCD_MIN_X					0
#define LCD_MAX_Y					240
#define LCD_MAX_X					400

#define LCD_DEVICE_ID				0x5420

//-----------------------------------------------
// Font for GUI
// ----------------------------------------------
// 
// Un-comment only one of these pairs to specify
// a font to use for the GUI
//-----------------------------------------------

// Simple Font 8x16
#define FONT_SMALL_INCLUDE				"font/Simple8x16.h"
#define FONT_SMALL_POINTER				&FONT_SIMPLE8X16

// Bold Font, 16x32
#define FONT_LARGE_INCLUDE				"font/Simple16X32.h"
#define FONT_LARGE_POINTER				&FONT_SIMPLE16X32


//-----------------------------------------------
// Defines
//-----------------------------------------------
// Delays for Init
#define LCD_SETUP_DELAY				2

// GUI Elements
#define LCD_PROGRESSBAR_PADDING		3

#define LCD_MENU_MAX_ITEMS			20
#define LCD_MENU_ITEM_LENGTH		30
#define LCD_TOPBAR_TEXT_LENGTH		20
#define LCD_TOPBAR_THICKNESS		20		// Y-Axis "thickness" of top menu bar, in pixels
#define LCD_TOPBAR_TEXT_XPADDING	0		// Padding between left side and start of text on top menu bar, in pixels
#define LCD_TOPBAR_TEXT_YPADDING	18		// Space between top and bottom edge of text, in pixels

#define LCD_BATTERY_WIDTH			24
#define LCD_BATTERY_HEIGHT			10
#define LCD_BATTERY_INSIDE_PADDING	2
#define LCD_BATTERY_X_POS			28		// X position, leftmost part of battery, inside of the top bar
#define LCD_BATTERY_Y_POS			4		// Y Position, topmost part of battery, inside of the top bar
#define LCD_BATTERY_NIB_WIDTH		2		// X Width of Battery Nib
#define LCD_BATTERY_NIB_OFFSET		3		// Pixels to move inside of either battery frame... indirectly the width of the nib

#define LCD_ANTENNA_BAR1_HEIGHT		3		// Smallest bar
#define LCD_ANTENNA_BAR2_HEIGHT		6
#define LCD_ANTENNA_BAR3_HEIGHT		9
#define LCD_ANTENNA_BAR4_HEIGHT		12		// Largest Bar
#define LCD_ANTENNA_BAR_WIDTH		2		// Width of bars
#define LCD_ANTENNA_BAR_PADDING		2		// Space in between bars
#define LCD_ANTENNA_X_POS			55		// X Position, leftmost part of icon, inside of the top bar
#define LCD_ANTENNA_Y_POS			15		// Y Position, bottommost part of icon, inside of the top bar


#define LCD_BATTERY_BARCOLOR		COLOR_GREEN
#define LCD_BATTERY_FRAMECOLOR		COLOR_WHITE
#define LCD_BATTERY_GOOD			30			// Above 30%, show the battery is good
#define LCD_BATTERY_GOOD_COLOR		COLOR_GREEN
#define LCD_BATTERY_BAD_COLOR		COLOR_RED

#define LCD_ANTENNA_FCOLOR			COLOR_GREEN
#define LCD_ANTENNA_BCOLOR			COLOR_GREY
#define LCD_ANTENNA_ERRORCOLOR		COLOR_RED


// Color Definitions
// Bits: 15 14 13 12 11 10 9  8  7  6  5  4  3  2  1  0
//       B  B  B  B  B  G  G  G  G  G  G  R  R  R  R  R
#define COLOR_BLACK					0x0000
#define COLOR_BLUE					0b1111100000000000
#define COLOR_GREEN					0b0000011111100000
#define COLOR_RED					0b0000000000011111
#define COLOR_REDLINERED			0x291b
#define COLOR_WHITE					0xFFFF
#define COLOR_GREY					0b1000010000010000

// Macros for writing data
#define lcd_writeCommand(command)	*LCD_CMD_ADDR=command
#define lcd_writeData(data)			*LCD_PARAM_ADDR=data


// LCD Commands
#define LCD_CMD_ID_READ					0x0000
#define LCD_CMD_DRIVER_OUTPUT_CTRL		0x0001
#define LCD_CMD_LCD_DRIVE_WAVEFORM_CTRL	0x0002
#define LCD_CMD_ENTRY_MODE				0x0003
#define LCD_CMD_DISPLAY_CTRL1			0x0007
#define LCD_CMD_DISPLAY_CTRL2			0x0008
#define LCD_CMD_LOW_PWR_CTRL1			0x0009
#define LCD_CMD_LOW_PWR_CTRL2			0x000B
#define LCD_CMD_EXT_DISP_CTRL1			0x000C
#define LCD_CMD_EXT_DISP_CTRL2			0x000F
#define LCD_CMD_PANEL_INT_CTRL1			0x0010
#define LCD_CMD_PANEL_INT_CTRL2			0x0011
#define LCD_CMD_PANEL_INT_CTRL3			0x0012
#define LCD_CMD_PANEL_INT_CTRL4			0x0020
#define LCD_CMD_PANEL_INT_CTRL5			0x0021
#define LCD_CMD_PANEL_INT_CTRL6			0x0022
#define LCD_CMD_FRAME_MARKER_CTRL		0x0090
#define LCD_CMD_POWER_CTRL1				0x0100
#define LCD_CMD_POWER_CTRL2				0x0101
#define LCD_CMD_POWER_CTRL3				0x0102
#define LCD_CMD_POWER_CTRL4				0x0103
#define LCD_CMD_POWER_CTRL5				0x0107
#define LCD_CMD_POWER_CTRL6				0x0110
#define LCD_CMD_GRAM_HORZ_ADDR_SET		0x0200
#define LCD_CMD_GRAM_VERT_ADDR_SET		0x0201
#define LCD_CMD_GRAM_RD_WR_DATA			0x0202
#define LCD_CMD_WINDOW_HORZ_ADDR_START	0x0210
#define LCD_CMD_WINDOW_HORZ_ADDR_END	0x0211
#define LCD_CMD_WINDOW_VERT_ADDR_START	0x0212
#define LCD_CMD_WINDOW_VERT_ADDR_END	0x0213
#define LCD_CMD_NVM_RD_WR				0x0280
#define LCD_CMD_VCOM1					0x0281
#define LCD_CMD_VCOM2					0x0282
#define LCD_CMD_GAMMA_CTRL1				0x0300
#define LCD_CMD_GAMMA_CTRL2				0x0301
#define LCD_CMD_GAMMA_CTRL3				0x0302
#define LCD_CMD_GAMMA_CTRL4				0x0303
#define LCD_CMD_GAMMA_CTRL5				0x0304
#define LCD_CMD_GAMMA_CTRL6				0x0305
#define LCD_CMD_GAMMA_CTRL7				0x0306
#define LCD_CMD_GAMMA_CTRL8				0x0307
#define LCD_CMD_GAMMA_CTRL9				0x0308
#define LCD_CMD_GAMMA_CTRL10			0x0309
#define LCD_CMD_GAMMA_CTRL11			0x030A
#define LCD_CMD_GAMMA_CTRL12			0x030B
#define LCD_CMD_GAMMA_CTRL13			0x030C
#define LCD_CMD_GAMMA_CTRL14			0x030D
#define LCD_CMD_GAMMA_CTRL15			0x030E
#define LCD_CMD_GAMMA_CTRL16			0x030F
#define LCD_CMD_BASE_IMG_SIZE			0x0400
#define LCD_CMD_BASE_IMG_CTRL			0x0401
#define LCD_CMD_VERT_SCROLL_CTRL		0x0404
#define LCD_CMD_PAR_DISP1_POS			0x0500
#define LCD_CMD_PAR_DISP1_ADDR_START	0x0501
#define LCD_CMD_PAR_DISP1_ADDR_END		0x0502
#define LCD_CMD_PAR_DISP2_POS			0x0503
#define LCD_CMD_PAR_DISP2_ADDR_START	0x0504
#define LCD_CMD_PAR_DISP2_ADDR_END		0x0505
#define LCD_CMD_INTERFACE_ENDIAN_CTRL	0x0606
#define LCD_CMD_NVM_ACCESS_CTRL			0x06F0

/*							  TRI-REG
							  |DFM
							  ||Always set to 0
							  |||BGR
							  ||||Always set to 0
							  |||||Always set to 0
							  ||||||HWM
							  |||||||Always set to 0	
							  ||||||||ORG
							  |||||||||Always set to 0
							  ||||||||||ID1
							  |||||||||||ID0
							  ||||||||||||AM
							  |||||||||||||Always set to 0
							  ||||||||||||||EPF1
							  |||||||||||||||EPF0
							  ||||||||||||||||			*/
#define LCD_ENTRY_MODE		0b0000000000100000		

/*		 ___________________________________
		/ ______________________________    \
		||								|   |    R    >   B 
		||	^							|   | F  I    >   U
		||	|							|   | L  B    >   T
		||	Y+							|   | E  B    >   T
		|| 		X+						|   | X  O    >   O
		||		----->					|   |    N    >   N
		||______________________________|   |
		\___________________________________/
		 LCD Entry Mode: 0b0000000000110000
			Update Direction:  Increment Y
*/


/*		 ___________________________________
		/ ______________________________    \
		||								|   |    R    >   B 
		||	^							|   | F  I    >   U
		||	|							|   | L  B    >   T
		||	Y+							|   | E  B    >   T
		|| 		X+						|   | X  O    >   O
		||		----->					|   |    N    >   N
		||______________________________|   |
		\___________________________________/
		 LCD Entry Mode: 0b0000000000100000
			Update Direction:  Decrement Y
*/



/*									  Always set to 0
									  |Always set to 0
									  ||Always set to 0
									  |||Always set to 0
									  ||||Always set to 0
									  |||||Always set to 0
									  ||||||Always set to 0
									  |||||||Always set to 0	
									  ||||||||Always set to 0
									  |||||||||Always set to 0
									  ||||||||||Always set to 0
									  |||||||||||Always set to 0
									  ||||||||||||Always set to 0
									  |||||||||||||NDL
									  ||||||||||||||VLE
									  |||||||||||||||REV
									  ||||||||||||||||			*/
#define LCD_BASE_IMAGE_DISP_CTRL	0b0000000000000011


#define lcd_force_redraw()			redraw = TRUE
#define lcd_redraw_required()		(redraw == TRUE)
#define lcd_redraw_complete()		redraw = FALSE

#define lcd_change_screens(screen)	lcd_fsm = screen

#define LCD_TASK_SLEEP_TIME			20

unsigned short lcd_readID(void);
void lcd_task_init(void);
void lcd_gui_task( void *pvParameters );
void lcd_reset( void );
void lcd_init( void );
void lcd_fadeBacklightOut(void);
void lcd_fadeBacklightIn(void);
void lcd_updateBacklightDuty(unsigned char duty);

void lcd_setCur(unsigned int x, unsigned int y);

void lcd_writeText_8x16(char *lcd_string,const unsigned char *font_style, unsigned int origin_x, unsigned int origin_y, unsigned int fcolor);
void lcd_writeText_16x32(char *lcd_string,const unsigned char *font_style, unsigned int origin_x, unsigned int origin_y, unsigned int fcolor);

void lcd_fillRGB(unsigned int data);
void lcd_displayImage(unsigned short *pixmap, unsigned short x_offset, unsigned short y_offset, unsigned short image_x, unsigned short image_y);
void lcd_displayLargeText(unsigned short *pixmap, unsigned short x_offset, unsigned short y_offset, unsigned short image_x, unsigned short image_y);
void lcd_drawLine(unsigned short x1, unsigned short y1, unsigned short x2, unsigned short y2, unsigned short color);
void lcd_drawFilledRectangle(unsigned short x1, unsigned short y1, unsigned short x2, unsigned short y2, unsigned short color);

struct tLCDProgressBar lcd_createProgressBar(unsigned short x1, unsigned short y1, unsigned short x2, unsigned short y2, unsigned short barcolor, unsigned short boxcolor, unsigned short backcolor);
void lcd_updateProgressBar(struct tLCDProgressBar *progressBar, unsigned char percent);

struct tLCDLabel lcd_createLabel(unsigned char *string, unsigned char *font_style, unsigned short x1, unsigned short y1, unsigned short width, unsigned short height, unsigned short textcolor, unsigned short backcolor);
void lcd_updateLabel(struct tLCDLabel * label, unsigned char *string);

struct tLCDTopBar lcd_createTopBar(unsigned char *string, unsigned short fcolor, unsigned short bcolor);
void lcd_updateAntenna(struct tLCDTopBar *topBar, unsigned char bars);
void lcd_updateBattery(struct tLCDTopBar *topBar, unsigned char percent);
void lcd_updateTopBarText(unsigned char *string);

void lcd_scrollDisplay(unsigned short numLines);

typedef struct tLCDProgressBar {
	unsigned short start_x;
	unsigned short start_y;
	unsigned short end_x;
	unsigned short end_y;
	
	unsigned char pixPercent;
	
	unsigned short position;

	unsigned short color_box;
	unsigned short color_bar;
	unsigned short color_background;
};

typedef struct tLCDLabel {
	unsigned short start_x;
	unsigned short start_y;
	unsigned short width;
	unsigned short height;
	
	unsigned char *font;
	
	unsigned short color_text;
	unsigned short color_background;
};

typedef struct tLCDBattery {
	unsigned short barcolor;
	unsigned short framecolor;
	unsigned char percent;
};

typedef struct tLCDAntenna {
	unsigned short fcolor;	// Active bars on the icon
	unsigned short bcolor;	// Inactive bars on the icon
	unsigned short errorcolor;	// Color of bars when no reception
	unsigned char bars;		// Current number of bars shown
};

typedef struct tLCDTopBar {
	unsigned short bcolor;	// Background Color
	unsigned short fcolor;	// Foreground Color
	
	unsigned char text[LCD_TOPBAR_TEXT_LENGTH];
	
	struct tLCDBattery battery;
	struct tLCDAntenna antenna;
};

#define LCD_WIDGET_QUEUE_SIZE			5

#define LCD_REQUEST_UPDATE_BATTERY		0
#define LCD_REQUEST_UPDATE_CHARGE		1
#define LCD_REQUEST_UPDATE_USB			2
#define LCD_REQUEST_UPDATE_ANTENNA		3


typedef struct tLCDRequest {
	unsigned char action;
	unsigned char data;
};



#endif /* LCD_H_ */
