/******************************************************************************
 *
 * LCD interface
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

#include <asf.h>
#include "drivers.h"
#include "menu.h"

#include FONT_SMALL_INCLUDE
#include FONT_LARGE_INCLUDE

// Lookup tables
const unsigned char pwmFadeTable[] = {
	0,   1,   1,   1,   1,   1,   1,   1,   1,
	1,   1,   1,   1,   1,   1,   1,   1,   1,
	1,   1,   1,   1,   1,   1,   1,   1,   1,
	1,   1,   1,   1,   1,   2,   2,   2,   2,
	2,   2,   2,   2,   2,   2,   2,   2,   2,
	2,   2,   2,   2,   2,   2,   3,   3,   3,
	3,   3,   3,   3,   3,   3,   3,   3,   3,
	3,   4,   4,   4,   4,   4,   4,   4,   4,
	4,   4,   4,   5,   5,   5,   5,   5,   5,
	5,   5,   6,   6,   6,   6,   6,   6,   6,
	7,   7,   7,   7,   7,   7,   8,   8,   8,
	8,   8,   8,   9,   9,   9,   9,   10,  10,
	10,  10,  10,  11,  11,  11,  11,  12,  12,
	12,  13,  13,  13,  13,  14,  14,  14,  15,
	15,  15,  16,  16,  16,  17,  17,  18,  18,
	18,  19,  19,  20,	20,  20,  21,  21,  22,
	22,  23,  23,  24,  24,  25,  26,  26,  27,
	27,  28,  29,  29,  30,  31,  31,  32,  33,
	33,  34,  35,  36,  36,  37,  38,  39,  40,
	41,  42,  42,  43,  44,  45,  46,  47,  48,
	50,  51,  52,  53,  54,  55,  57,  58,  59,
	60,  62,  63,  64,  66,  67,  69,  70,  72,
	74,  75,  77,  79,  80,  82,  84,  86,  88,	
	90,  91,  94,  96,  98,  100, 102, 104, 107,
	109, 111, 114, 116, 119, 122, 124, 127, 130,
	133, 136, 139, 142, 145, 148, 151, 155, 158,
	161, 165, 169, 172, 176, 180, 184, 188, 192,
	196, 201, 205, 210, 214, 219, 224, 229, 234,
	239, 244, 250, 255
};

const unsigned char hexLookup[] = {'0', '1', '2', '3', '4', '5', '6', '7', '8', '9', 'A', 'B', 'C', 'D', 'E', 'F'};


// Data Structs
struct tLCDTopBar topBar;

// Create task for FreeRTOS
void lcd_task_init( void ){
	xTaskCreate(lcd_gui_task, configTSK_GUI_TASK_NAME, configTSK_GUI_TASK_STACK_SIZE, NULL, configTSK_GUI_TASK_PRIORITY, NULL);
}

// LCD GUI Task
void lcd_gui_task( void *pvParameters ){
	
	unsigned char i = 0;
	struct tMenu mainMenu;
	
	lcd_reset();
	
	if( lcd_readID() != LCD_DEVICE_ID){
		// We broke the display!!
		while( TRUE ){
			asm("nop");
		}
	}
	
	lcd_init();
	
	lcd_fillRGB(COLOR_WHITE);
	
	topBar = lcd_createTopBar("traq|paq", COLOR_WHITE, COLOR_BLACK);
	
	mainMenu = menu_create("Main Menu", FONT_SMALL_POINTER);
	menu_addItem(&mainMenu, "Standard Session", 1);
	menu_addItem(&mainMenu, "Timed Moto", 2);
	menu_addItem(&mainMenu, "Settings", 3);
	menu_addItem(&mainMenu, "Review", 4);
	menu_addItem(&mainMenu, "About", 5);
	
	// Prevent backlight from turning on while screen is being initialize
	vTaskDelay( (portTickType)TASK_DELAY_MS(50) );
	
	lcd_fadeBacklightIn();
	
	
	while(1){
		vTaskDelay( (portTickType)TASK_DELAY_MS(1000) );
		menu_scrollDown(&mainMenu);
	}
}


void lcd_reset( void ){
	gpio_clr_gpio_pin(LCD_RESET);
	vTaskDelay( (portTickType)TASK_DELAY_MS(LCD_RESET_TIME) );
	gpio_set_gpio_pin(LCD_RESET);
	vTaskDelay( (portTickType)TASK_DELAY_MS(LCD_RESET_TIME) );
}


void lcd_init(void){
	lcd_writeCommand(LCD_CMD_INTERFACE_ENDIAN_CTRL);	// Pin Control (R606h)
	lcd_writeData(0x0000);								// Page 41 of SPFD5420A Datasheet
	
	vTaskDelay( (portTickType)TASK_DELAY_MS(LCD_SETUP_DELAY) );
	
	lcd_writeCommand(LCD_CMD_DISPLAY_CTRL1);			// Display Control 1 (R007h)
	lcd_writeData(0x0001);								// Page 16 of SPFD5420A Datasheet
	
	vTaskDelay( (portTickType)TASK_DELAY_MS(LCD_SETUP_DELAY) );
	
	lcd_writeCommand(LCD_CMD_POWER_CTRL6);				// Power Control 6(R110h)
	lcd_writeData(0x0001);								// Page 30 of SPFD5420A Datasheet
	
	vTaskDelay( (portTickType)TASK_DELAY_MS(LCD_SETUP_DELAY) );
	
	lcd_writeCommand(LCD_CMD_POWER_CTRL1);				// Power Control 1 (R100h)
	lcd_writeData(0x17B0);								// Page 26 of SPFD5420A Datasheet
	
	lcd_writeCommand(LCD_CMD_POWER_CTRL2);				// Power Control 2 (R101h)
	lcd_writeData(0x0147);								// Page 27 of SPFD5420A Datasheet
	
	lcd_writeCommand(LCD_CMD_POWER_CTRL3);				// Power Control 3 (R102h)
	lcd_writeData(0x019D);								// Page 28 of SPFD5420A Datasheet
	
	lcd_writeCommand(LCD_CMD_POWER_CTRL4);				// Power Control 4 (R103h)
	lcd_writeData(0x3600);								// Page 29 of SPFD5420A Datasheet
	
	lcd_writeCommand(LCD_CMD_VCOM1);					// NVM read data 2 (R281h)
	lcd_writeData(0x0010);								// Page 34 of SPFD5420A Datasheet
	
	vTaskDelay( (portTickType)TASK_DELAY_MS(LCD_SETUP_DELAY) );
	
	lcd_writeCommand(LCD_CMD_POWER_CTRL3);				// Power Control 3 (R102h)
	lcd_writeData(0x01BD);								// Page 28 of SPFD5420A Datasheet
	
	vTaskDelay( (portTickType)TASK_DELAY_MS(LCD_SETUP_DELAY) );

	//--------------- Power control 1~6 ---------------//
	lcd_writeCommand(LCD_CMD_POWER_CTRL1);				// Power Control 1 (R100h)
	lcd_writeData(0x16B0);								// Page 26 of SPFD5420A Datasheet
	
	lcd_writeCommand(LCD_CMD_POWER_CTRL2);				// Power Control 2 (R101h)
	lcd_writeData(0x0147);								// Page 27 of SPFD5420A Datasheet
	
	lcd_writeCommand(LCD_CMD_POWER_CTRL3);				// Power Control 3 (R102h)
	lcd_writeData(0x01BD);								// Page 28 of SPFD5420A Datasheet
	
	lcd_writeCommand(LCD_CMD_POWER_CTRL4);				// Power Control 4 (R103h)
	lcd_writeData(0x2d00);								// Page 29 of SPFD5420A Datasheet
	
	lcd_writeCommand(LCD_CMD_POWER_CTRL5);				// Power Control 5 (R107h)
	lcd_writeData(0x0000);								// Page 30 of SPFD5420A Datasheet
	
	lcd_writeCommand(LCD_CMD_POWER_CTRL6);				// Power Control 6(R110h)
	lcd_writeData(0x0001);								// Page 30 of SPFD5420A Datasheet
	
	//lcd_writeCommand(LCD_CMD_NVM_RD_WR);				// NVM read data 1 (R280h)
	//lcd_writeData(0x0000);							// Page 33 of SPFD5420A Datasheet
	
	lcd_writeCommand(LCD_CMD_VCOM1);					// NVM read data 2 (R281h)
	lcd_writeData(0x0006);								// Page 34 of SPFD5420A Datasheet
	
	lcd_writeCommand(LCD_CMD_VCOM2);					// NVM read data 3 (R282h)
	lcd_writeData(0x0000);								// Page 34 of SPFD5420A Datasheet


	lcd_writeCommand(LCD_CMD_BASE_IMG_SIZE);			// Base Image Number of Line (R400h)
	lcd_writeData(0x3500);								// Page 36 of SPFD5420A Datasheet
	
	lcd_writeCommand(LCD_CMD_BASE_IMG_CTRL);			// Base Image Display Control (R401h)
	lcd_writeData(LCD_BASE_IMAGE_DISP_CTRL);			// Page 39 of SPFD5420A Datasheet
	
	lcd_writeCommand(LCD_CMD_VERT_SCROLL_CTRL);			// Based Image Vertical Scroll Control (R404h)
	lcd_writeData(0);									// Page 40 of SPFD5420A Datasheet

	//--------------- Normal set ---------------//
	//lcd_writeCommand(LCD_CMD_ID_READ);					// ID Read Register (R000h)
	//lcd_writeData(0x0000);								// Page 13 of SPFD5420A Datasheet
	
	lcd_writeCommand(LCD_CMD_DRIVER_OUTPUT_CTRL);		// Driver Output Control Register (R001h)
	lcd_writeData(0x0100);								// Page 14 of SPFD5420A Datasheet
	
	lcd_writeCommand(LCD_CMD_LCD_DRIVE_WAVEFORM_CTRL);	// LCD Driving Waveform Control (R002h)
	lcd_writeData(0x0100);								// Page 14 of SPFD5420A Datasheet
	
	lcd_writeCommand(LCD_CMD_ENTRY_MODE);				// Entry Mode (R003h)
	lcd_writeData(LCD_ENTRY_MODE);						// Page 15 of SPFD5420A Datasheet

	//lcd_writeCommand(0x0007);							// Display Control 1 (R007h)
	//lcd_writeData(0x0000);							// Page 16 of SPFD5420A Datasheet
	
	//lcd_writeCommand(0x0008);							// Display Control 2 (R008h)
	//lcd_writeData(0x0808);							// Page 17 of SPFD5420A Datasheet

	lcd_writeCommand(LCD_CMD_LOW_PWR_CTRL1);			// Display Control 3 (R009h)
	lcd_writeData(0x0001);								// Page 18 of SPFD5420A Datasheet
	
	lcd_writeCommand(LCD_CMD_LOW_PWR_CTRL2);			// Low Power Control (R00Bh)
	lcd_writeData(0x0010);								// Page 19 of SPFD5420A Datasheet
	
	lcd_writeCommand(LCD_CMD_EXT_DISP_CTRL1);			// External Display Interface Control 1 (R00Ch)
	lcd_writeData(0x0001);								// Page 19 of SPFD5420A Datasheet
	
	lcd_writeCommand(LCD_CMD_EXT_DISP_CTRL2);			// External Display Interface Control 2 (R00Fh)
	lcd_writeData(0x0000);								// Page 20 of SPFD5420A Datasheet
	
	lcd_writeCommand(LCD_CMD_DISPLAY_CTRL1);			// Display Control 1 (R007h)
	lcd_writeData(0x0001);								// Page 16 of SPFD5420A Datasheet

	//--------------- Panel interface control 1~6 ---------------//
	lcd_writeCommand(LCD_CMD_PANEL_INT_CTRL1);			// Panel Interface Control 1 (R010h)
	lcd_writeData(0x0012);								// Page 20 of SPFD5420A Datasheet
	
	lcd_writeCommand(LCD_CMD_PANEL_INT_CTRL2);			// Panel Interface Control 2 (R011h)
	lcd_writeData(0x0202);								// Page 21 of SPFD5420A Datasheet
	
	lcd_writeCommand(LCD_CMD_PANEL_INT_CTRL3);			// Panel Interface control 3 (R012h)
	lcd_writeData(0x0300);								// Page 22 of SPFD5420A Datasheet
	
	lcd_writeCommand(LCD_CMD_PANEL_INT_CTRL4);			// Panel Interface control 4 (R020h)
	lcd_writeData(0x021E);								// Page 22 of SPFD5420A Datasheet
	
	lcd_writeCommand(LCD_CMD_PANEL_INT_CTRL5);			// Panel Interface Control 5 (021Rh)
	lcd_writeData(0x0202);								// Page 24 of SPFD5420A Datasheet
	
	lcd_writeCommand(LCD_CMD_PANEL_INT_CTRL6);			// Panel Interface Control 6 (R022h)
	lcd_writeData(0x0100);								// Page 25 of SPFD5420A Datasheet
	
	lcd_writeCommand(LCD_CMD_FRAME_MARKER_CTRL);		// Frame Marker Control (R090h)
	lcd_writeData(0x8000);								// Page 25 of SPFD5420A Datasheet
	
	vTaskDelay( (portTickType)TASK_DELAY_MS(LCD_SETUP_DELAY) );
	
	lcd_writeCommand(LCD_CMD_DISPLAY_CTRL1);			// Display Control 1 (R007h)
	lcd_writeData(0x0173);								// Page 16 of SPFD5420A Datasheet
	
	vTaskDelay( (portTickType)TASK_DELAY_MS(LCD_SETUP_DELAY) );
}

unsigned short lcd_readID(){
	lcd_writeCommand(LCD_CMD_ID_READ);
	
	return (*LCD_PARAM_ADDR);
}

void lcd_setCur(unsigned int x, unsigned int y){
	lcd_writeCommand(LCD_CMD_GRAM_VERT_ADDR_SET);	// GRAM Address Set (Vertical Address)
	lcd_writeData(x);								// Page 30 of SPFD5420A Datasheet
	lcd_writeCommand(LCD_CMD_GRAM_HORZ_ADDR_SET);	// GRAM Address Set (Horizontal Address)
	lcd_writeData(y);								// Page 30 of SPFD5420A Datasheet
	lcd_writeCommand(LCD_CMD_GRAM_RD_WR_DATA);		// Write Data to GRAM
}

void lcd_fillRGB(unsigned int data){
	unsigned int i,j;
	
	lcd_setCur(LCD_MIN_X, LCD_MAX_Y);
	
	for(i=0;i<432;i++){
		for(j=0;j<240;j++){
			lcd_writeData(data);
		}
	}
}

void lcd_displayImage(unsigned short *pixmap, unsigned short x_offset, unsigned short y_offset, unsigned short image_x, unsigned short image_y){
	unsigned int i,j;
	
	lcd_setCur(x_offset, y_offset);
	
	for(j=0; j < image_x; j++){
		for(i=0; i < image_y; i++){
			lcd_writeData(*pixmap);
			pixmap++;
		}
		lcd_setCur(x_offset + j, y_offset);
	}

}

void lcd_displayLargeText(unsigned short *pixmap, unsigned short x_offset, unsigned short y_offset, unsigned short image_x, unsigned short image_y){
	unsigned int i,j;
	unsigned short match;
	for(j=0; j < image_x; j++){
		for(i=0; i < image_y; i++){
			if(*pixmap != COLOR_WHITE){
				lcd_writeData(*pixmap);
			}else{
				lcd_setCur(x_offset + j, y_offset - i);
			}			
			pixmap++;
		}
		lcd_setCur(x_offset + j, y_offset);
	}

}

void lcd_writeText_8x16(char *lcd_string, const unsigned char *font_style, unsigned int origin_x, unsigned int origin_y, unsigned int fcolor){
	unsigned short x, y;
	unsigned int mask, xfont, yfont, font_size;
	const unsigned char *data;

	data = font_style;  // point to the start of the font table
	xfont = *data;  // get font x width
	data++;
	yfont = *data;  // get font y length
	data++;
	font_size = *data;  // get data bytes per font

	while(*lcd_string !='\0'){
		// point to character data in font table
		data =  (font_style + font_size) +			// header offset
		(font_size * (int)(*lcd_string - 32));		// character select

		for (y = (origin_y + yfont); y > origin_y; y--){
			mask = 0x01;
			for (x = (origin_x + xfont); x > origin_x ; x--){
				if (*data & mask){ // if pixel data then put dot
					lcd_setCur(x, y);
					lcd_writeData(fcolor);
				}
				mask <<= 1;
			}
		
			// Next row data
			data++;
		}
		
		// move to next character start pixel
		origin_x += xfont;
		lcd_string++;  // next character in string
	}		
}

void lcd_writeText_16x32(char *lcd_string, const unsigned char *font_style, unsigned int origin_x, unsigned int origin_y, unsigned int fcolor){
	unsigned short x, y;
	unsigned int mask, xfont, yfont, font_size;
	const unsigned short *data;

	//data = font_style;  // point to the start of the font table
	xfont = 16;//*data;  // get font x width
	//data++;
	yfont = 32;//*data;  // get font y length
	//data++;
	font_size = 64;//*data;  // get data bytes per font

	while(*lcd_string !='\0'){
		// point to character data in font table
		data =  (font_style) +			// header offset, (font_style + font_size) +	
		(font_size * (int)(*lcd_string - 32));		// character select

		for (y = (origin_y + yfont); y > origin_y; y--){
			mask = 0x01;
			for (x = (origin_x + xfont); x > origin_x ; x--){
				if (*data & mask){ // if pixel data then put dot
					lcd_setCur(x, y);
					lcd_writeData(fcolor);
				}
				mask <<= 1;
			}
		
			// Next row data
			data += 1;
		}
		
		// move to next character start pixel
		origin_x += xfont;
		lcd_string++;  // next character in string
	}		
}

void integer_to_hexascii(unsigned short number, unsigned char *string){
	*string = '0';
	string++;
	*string = 'x';
	string++;
	*string = hexLookup[(number >> 12) & 0xF];
	string++;
	*string = hexLookup[(number >>  8) & 0xF];
	string++;
	*string = hexLookup[(number >>  4) & 0xF];
	string++;
	*string = hexLookup[(number >>  0) & 0xF];
	string++;
	*string = '\0';
}




void lcd_drawFilledRectangle(unsigned short x1, unsigned short y1, unsigned short x2, unsigned short y2, unsigned short color){
	volatile int numPixels;
	
	numPixels = (x2 - x1 + 1) * (y1 - y2 + 1);		// Rectangle width * height
	
	// TRICKY: SPFD5420 definitions of horizontal and vertical are different from used in our application
	lcd_writeCommand(LCD_CMD_WINDOW_VERT_ADDR_START);
	lcd_writeData(x1);
	lcd_writeCommand(LCD_CMD_WINDOW_VERT_ADDR_END);
	lcd_writeData(x2);
	lcd_writeCommand(LCD_CMD_WINDOW_HORZ_ADDR_START);
	lcd_writeData(y2);
	lcd_writeCommand(LCD_CMD_WINDOW_HORZ_ADDR_END);
	lcd_writeData(y1);
	
	lcd_setCur(x1, y1);
	
	while(numPixels >= 0){
		lcd_writeData(color);
		numPixels -= 1;
	}
	
	// Reset GRAM drawing window
	lcd_writeCommand(LCD_CMD_WINDOW_VERT_ADDR_START);
	lcd_writeData(LCD_MIN_X);
	lcd_writeCommand(LCD_CMD_WINDOW_VERT_ADDR_END);
	lcd_writeData(LCD_MAX_X);
	lcd_writeCommand(LCD_CMD_WINDOW_HORZ_ADDR_START);
	lcd_writeData(LCD_MIN_Y);
	lcd_writeCommand(LCD_CMD_WINDOW_HORZ_ADDR_END);
	lcd_writeData(LCD_MAX_Y);
	
}

void lcd_updateBacklightDuty(unsigned char duty){
	#if(TRAQPAQ_HW_PWM_ENABLED)
	volatile avr32_pwm_channel_t pwm_channel_0 = {.ccnt = BACKLIGHT_PWM_ID };
		
	pwm_channel_0.CMR.calg = PWM_MODE_LEFT_ALIGNED;       	// Channel mode.
	pwm_channel_0.CMR.cpol = PWM_POLARITY_HIGH;            	// Channel polarity.
	pwm_channel_0.CMR.cpd = PWM_UPDATE_DUTY;              	// Not used the first time.
	pwm_channel_0.CMR.cpre = AVR32_PWM_CPRE_MCK;  			// Channel prescaler.
	pwm_channel_0.cprd = BACKLIGHT_PWM_MAX;  				// Channel period.
	
	pwm_channel_0.cdty = duty;
    pwm_channel_0.cupd = pwm_channel_0.cdty;

    pwm_async_update_channel(BACKLIGHT_PWM_CHANNEL, &pwm_channel_0);
	#endif
}	

void lcd_fadeBacklightIn(){
	#if(TRAQPAQ_HW_PWM_ENABLED)
	unsigned short i;
	
	for (i = 0; i < sizeof(pwmFadeTable); i++){
		lcd_updateBacklightDuty(pwmFadeTable[i]);
		vTaskDelay( (portTickType)TASK_DELAY_MS(BACKLIGHT_FADE_DELAY) );
	}
	#else
	gpio_set_gpio_pin(BACKLIGHT_PIN);
	#endif
}

void lcd_fadeBacklightOut(){
	#if(TRAQPAQ_HW_PWM_ENABLED)
	unsigned short i;

	for (i = sizeof(pwmFadeTable)-1; i > 0; i--){
		lcd_updateBacklightDuty(pwmFadeTable[i]);
		vTaskDelay( (portTickType)TASK_DELAY_MS(BACKLIGHT_FADE_DELAY) );
	}
	lcd_updateBacklightDuty(0);		// TODO: Need to figure out why this isn't being set by FOR loop above
	#else
	gpio_clr_gpio_pin(BACKLIGHT_PIN);
	#endif
}


void lcd_drawLine(unsigned short x1, unsigned short y1, unsigned short x2, unsigned short y2, unsigned short color){
	// Barrowed code
	
	// Compute deltas, ie. "width" and "height" of line, then
	// compute x and y direction, and make deltas positive for later use.
	int16_t xinc = 1; // Start off assuming direction is positive, ie. right.
	int16_t dx = x2 - x1;
	if (dx < 0){
	xinc = -1;
	dx = -dx;
	}
	int16_t yinc = 1; // Start off assuming direction is positive, ie. down.
	int16_t dy = y2 - y1;
	if (dy < 0){
	yinc = -1;
	dy = -dy;
	}

	// Set up current point.
	uint16_t x = x1;
	uint16_t y = y1;
	uint16_t i;
	// A "flat" line (dx>dy) is handled differently from a "steep" line (dx<dy).
	if (dx > dy) {
	// Walk along X, draw pixel, and step Y when required.
	int16_t e = dx >> 1;
	for ( i = 0; i <= dx; ++i) {
		lcd_setCur(x, y);
		lcd_writeData(color);
		// Sub-pixel "error" overflowed, so we step Y and reset the "error".
		if (e <= 0){
		e += dx;
		y += yinc;
		}
		// Walk one step along X.
		e -= dy;
		x += xinc;
	}
	} else {
	// Walk along Y, draw pixel, and step X when required.
	int16_t e = dy >> 1;
	for (i = 0; i <= dy; ++i) {
		lcd_setCur(x, y);
		lcd_writeData(color);
		// Sub-pixel "error" overflowed, so we step X and reset the "error".
		if (e <= 0){
		e += dy;
		x += xinc;
		}
		// Walk one step along Y.
		e -= dx;
		y += yinc;
	}
	}
}


struct tLCDProgressBar lcd_createProgressBar(unsigned short x1, unsigned short y1, unsigned short x2, unsigned short y2, unsigned short barcolor, unsigned short boxcolor, unsigned short backcolor){
	struct tLCDProgressBar progressBar;
	
	progressBar.start_x = x1;
	progressBar.start_y = y1;
	progressBar.end_x = x2;
	progressBar.end_y = y2;
	
	progressBar.color_bar = barcolor;
	progressBar.color_box = boxcolor;
	progressBar.color_background = backcolor;
	
	progressBar.position = progressBar.start_x + LCD_PROGRESSBAR_PADDING;
	
	progressBar.pixPercent = (progressBar.end_x - progressBar.start_x) / 100;
	
	lcd_drawLine(progressBar.start_x, progressBar.start_y, progressBar.end_x, progressBar.start_y, progressBar.color_box);
	lcd_drawLine(progressBar.end_x, progressBar.start_y, progressBar.end_x, progressBar.end_y, progressBar.color_box);
	lcd_drawLine(progressBar.end_x, progressBar.end_y, progressBar.start_x, progressBar.end_y, progressBar.color_box);
	lcd_drawLine(progressBar.start_x, progressBar.end_y, progressBar.start_x, progressBar.start_y, progressBar.color_box);
	
	return progressBar;
}

void lcd_updateProgressBar(struct tLCDProgressBar *progressBar, unsigned char percent){
	unsigned short newX = (progressBar->pixPercent * percent) + progressBar->start_x + LCD_PROGRESSBAR_PADDING;
	
	if(newX < progressBar->position){
		// We need to backtrack!
		lcd_drawFilledRectangle(newX, progressBar->start_y - LCD_PROGRESSBAR_PADDING, progressBar->position, progressBar->end_y + LCD_PROGRESSBAR_PADDING, progressBar->color_background);
	}else{
		// Draw bar as normal
		lcd_drawFilledRectangle(progressBar->position, progressBar->start_y - LCD_PROGRESSBAR_PADDING, newX, progressBar->end_y + LCD_PROGRESSBAR_PADDING, progressBar->color_bar);
	}

	progressBar->position = newX;
}


struct tLCDLabel lcd_createLabel(unsigned char *string, unsigned char *font_style, unsigned short x1, unsigned short y1, unsigned short width, unsigned short height, unsigned short textcolor, unsigned short backcolor){
	struct tLCDLabel label;
	
	label.start_x = x1;
	label.start_y = y1;
	label.width = width;
	label.height = height;
	
	label.color_text = textcolor;
	label.color_background = backcolor;
	label.font = font_style;
	
	lcd_writeText_8x16(string, label.font, label.start_x, label.start_y, label.color_text);
	
	return label;
}

void lcd_updateLabel(struct tLCDLabel * label, unsigned char *string) {
	lcd_drawFilledRectangle(label->start_x, label->start_y + label->height, label->start_x + label->width, label->start_y, label->color_background);
	lcd_writeText_8x16(string, label->font, label->start_x, label->start_y, label->color_text);
}

void lcd_scrollDisplay(unsigned short numLines){
	
	lcd_writeCommand(LCD_CMD_VERT_SCROLL_CTRL);		// Based Image Vertical Scroll Control (R404h)
	lcd_writeData(numLines);						// Page 40 of SPFD5420A Datasheet
}

struct tLCDTopBar lcd_createTopBar(unsigned char *string, unsigned short fcolor, unsigned short bcolor){
	struct tLCDTopBar topBar;
	
	topBar.fcolor = fcolor;
	topBar.bcolor = bcolor;
	
	topBar.battery.barcolor = LCD_BATTERY_BARCOLOR;
	topBar.battery.framecolor = LCD_BATTERY_FRAMECOLOR;
	topBar.battery.percent = 0;
	
	topBar.antenna.bars = 0;
	topBar.antenna.fcolor = LCD_ANTENNA_FCOLOR;
	topBar.antenna.bcolor = LCD_ANTENNA_BCOLOR;
	topBar.antenna.errorcolor = LCD_ANTENNA_ERRORCOLOR;
	
	// Draw bar background
	lcd_drawFilledRectangle(LCD_MIN_X, LCD_MAX_Y, LCD_MAX_X, LCD_MAX_Y - LCD_TOPBAR_THICKNESS, topBar.bcolor);
	
	// Write in the text
	lcd_writeText_8x16(string, FONT_SMALL_POINTER, LCD_TOPBAR_TEXT_XPADDING, LCD_MAX_Y - LCD_TOPBAR_TEXT_YPADDING, topBar.fcolor);
	
	// Draw battery
	lcd_drawLine(LCD_MAX_X - LCD_BATTERY_X_POS,
				LCD_MAX_Y - LCD_BATTERY_Y_POS,
				LCD_MAX_X - LCD_BATTERY_X_POS + LCD_BATTERY_WIDTH,
				LCD_MAX_Y - LCD_BATTERY_Y_POS,
				topBar.battery.framecolor);
				
	lcd_drawLine(LCD_MAX_X - LCD_BATTERY_X_POS + LCD_BATTERY_WIDTH,
				LCD_MAX_Y - LCD_BATTERY_Y_POS,
				LCD_MAX_X - LCD_BATTERY_X_POS + LCD_BATTERY_WIDTH,
				LCD_MAX_Y - LCD_BATTERY_Y_POS - LCD_BATTERY_HEIGHT,
				topBar.battery.framecolor);
				
	lcd_drawLine(LCD_MAX_X - LCD_BATTERY_X_POS,
				LCD_MAX_Y - LCD_BATTERY_Y_POS - LCD_BATTERY_HEIGHT,
				LCD_MAX_X - LCD_BATTERY_X_POS + LCD_BATTERY_WIDTH,
				LCD_MAX_Y - LCD_BATTERY_Y_POS - LCD_BATTERY_HEIGHT,
				topBar.battery.framecolor);
				
	lcd_drawLine(LCD_MAX_X - LCD_BATTERY_X_POS,
				LCD_MAX_Y - LCD_BATTERY_Y_POS - LCD_BATTERY_HEIGHT,
				LCD_MAX_X - LCD_BATTERY_X_POS,
				LCD_MAX_Y - LCD_BATTERY_Y_POS,
				topBar.battery.framecolor);
				
	lcd_drawFilledRectangle(LCD_MAX_X - LCD_BATTERY_X_POS + LCD_BATTERY_WIDTH,
							LCD_MAX_Y - LCD_BATTERY_HEIGHT + LCD_BATTERY_NIB_OFFSET,
							LCD_MAX_X - LCD_BATTERY_X_POS + LCD_BATTERY_WIDTH + LCD_BATTERY_NIB_WIDTH,
							LCD_MAX_Y - LCD_BATTERY_Y_POS - LCD_BATTERY_HEIGHT + LCD_BATTERY_NIB_OFFSET,
							topBar.battery.framecolor);
	
	
	// Draw Antenna
	lcd_updateAntenna(&topBar, 0);
	
	return topBar;
}

void lcd_updateTopBarText(unsigned char *string){
	// Draw bar background
	lcd_drawFilledRectangle(LCD_MIN_X, LCD_MAX_Y, LCD_MAX_X >> 1, LCD_MAX_Y - LCD_TOPBAR_THICKNESS, topBar.bcolor);
	
	// Write in the text
	lcd_writeText_8x16(string, FONT_SMALL_POINTER, LCD_TOPBAR_TEXT_XPADDING, LCD_MAX_Y - LCD_TOPBAR_TEXT_YPADDING, topBar.fcolor);
}

void lcd_updateAntenna(struct tLCDTopBar *topBar, unsigned char bars){
	unsigned short bar1, bar2, bar3, bar4;
	
	switch(bars){
		case(1):
			bar1 = topBar->antenna.fcolor;
			bar2 = topBar->antenna.bcolor;
			bar3 = topBar->antenna.bcolor;
			bar4 = topBar->antenna.bcolor;
			topBar->antenna.bars = 1;
			break;
			
		case(2):
			bar1 = topBar->antenna.fcolor;
			bar2 = topBar->antenna.fcolor;
			bar3 = topBar->antenna.bcolor;
			bar4 = topBar->antenna.bcolor;
			topBar->antenna.bars = 2;
			break;
			
		case(3):
			bar1 = topBar->antenna.fcolor;
			bar2 = topBar->antenna.fcolor;
			bar3 = topBar->antenna.fcolor;
			bar4 = topBar->antenna.bcolor;
			topBar->antenna.bars = 3;
			break;
			
		case(4):
			bar1 = topBar->antenna.fcolor;
			bar2 = topBar->antenna.fcolor;
			bar3 = topBar->antenna.fcolor;
			bar4 = topBar->antenna.fcolor;
			topBar->antenna.bars = 4;
			break;
			
		default:	// Catch zero and any other invalid crap
			bar1 = topBar->antenna.errorcolor;
			bar2 = topBar->antenna.errorcolor;
			bar3 = topBar->antenna.errorcolor;
			bar4 = topBar->antenna.errorcolor;
			topBar->antenna.bars = 0;
			break;
	}
	
	lcd_drawFilledRectangle(LCD_MAX_X - LCD_ANTENNA_X_POS,
							LCD_MAX_Y - LCD_ANTENNA_Y_POS + LCD_ANTENNA_BAR1_HEIGHT,
							LCD_MAX_X - LCD_ANTENNA_X_POS + LCD_ANTENNA_BAR_WIDTH,
							LCD_MAX_Y - LCD_ANTENNA_Y_POS,
							bar1);	// Bar 1: Small
							
	lcd_drawFilledRectangle(LCD_MAX_X - LCD_ANTENNA_X_POS + (LCD_ANTENNA_BAR_WIDTH + LCD_ANTENNA_BAR_PADDING),
							LCD_MAX_Y - LCD_ANTENNA_Y_POS + LCD_ANTENNA_BAR2_HEIGHT,
							LCD_MAX_X - LCD_ANTENNA_X_POS + (LCD_ANTENNA_BAR_WIDTH + LCD_ANTENNA_BAR_PADDING) + LCD_ANTENNA_BAR_WIDTH,
							LCD_MAX_Y - LCD_ANTENNA_Y_POS,
							bar2);	// Bar 2
							
	lcd_drawFilledRectangle(LCD_MAX_X - LCD_ANTENNA_X_POS + 2*(LCD_ANTENNA_BAR_WIDTH + LCD_ANTENNA_BAR_PADDING),
							LCD_MAX_Y - LCD_ANTENNA_Y_POS + LCD_ANTENNA_BAR3_HEIGHT,
							LCD_MAX_X - LCD_ANTENNA_X_POS + 2*(LCD_ANTENNA_BAR_WIDTH + LCD_ANTENNA_BAR_PADDING) + LCD_ANTENNA_BAR_WIDTH,
							LCD_MAX_Y - LCD_ANTENNA_Y_POS,
							bar3);	// Bar 3
							
	lcd_drawFilledRectangle(LCD_MAX_X - LCD_ANTENNA_X_POS + 3*(LCD_ANTENNA_BAR_WIDTH + LCD_ANTENNA_BAR_PADDING),
							LCD_MAX_Y - LCD_ANTENNA_Y_POS + LCD_ANTENNA_BAR4_HEIGHT,
							LCD_MAX_X - LCD_ANTENNA_X_POS + 3*(LCD_ANTENNA_BAR_WIDTH + LCD_ANTENNA_BAR_PADDING) + LCD_ANTENNA_BAR_WIDTH,
							LCD_MAX_Y - LCD_ANTENNA_Y_POS,
							bar4);	// Bar 4: Large
	
}

void lcd_updateBattery(struct tLCDTopBar *topBar, unsigned char percent){
	unsigned short newX = percent / 5;
	
	lcd_drawFilledRectangle(LCD_MAX_X - 26, LCD_MAX_Y - 6, LCD_MAX_X - 6, LCD_MAX_Y - 12, topBar->bcolor);	// Insides of battery
	if(percent >= LCD_BATTERY_GOOD){
		lcd_drawFilledRectangle(LCD_MAX_X - 26, LCD_MAX_Y - 6, LCD_MAX_X - 26 + newX, LCD_MAX_Y - 12, LCD_BATTERY_GOOD_COLOR);	// Insides of battery
		
	}else{
		lcd_drawFilledRectangle(LCD_MAX_X - 26, LCD_MAX_Y - 6, LCD_MAX_X - 26 + newX, LCD_MAX_Y - 12, LCD_BATTERY_BAD_COLOR);	// Insides of battery
	}
	
	topBar->battery.percent = percent;
	
}