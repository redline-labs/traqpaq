/******************************************************************************
 *
 * LCD interface
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

#include <asf.h>
#include "hal.h"
#include "menu.h"
#include "control_fsm.h"
#include "string.h"
#include "itoa.h"

#include "images/traqpaq.h"

#include FONT_SMALL_INCLUDE
#include FONT_LARGE_INCLUDE

// Lookup Table
const unsigned char hexLookup[] = {'0', '1', '2', '3', '4', '5', '6', '7', '8', '9', 'A', 'B', 'C', 'D', 'E', 'F'};

extern struct tFlashOTP flashOTP;

volatile unsigned short id;

// Queues
xQueueHandle lcdWidgetsManagerQueue;
xQueueHandle lcdButtonsManagerQueue;
xTimerHandle xPeripherialTimer;

// Create task for FreeRTOS
void lcd_task_init( void ){
	if(systemFlags.button.powerOnMethod == POWER_ON_MODE_BUTTON){
		xPeripherialTimer = xTimerCreate( "PeripheralTimer", LCD_PERIPHERIAL_FADE_TIME * configTICK_RATE_HZ, pdFALSE, NULL, lcd_clearPeripheral );
		xTaskCreate(lcd_gui_task_normal, configTSK_GUI_TASK_NAME, configTSK_GUI_TASK_STACK_SIZE, NULL, configTSK_GUI_TASK_PRIORITY, configTSK_GUI_TASK_HANDLE);
		
	}else{
		xTaskCreate(lcd_gui_task_usb, configTSK_GUI_TASK_NAME, configTSK_GUI_TASK_STACK_SIZE, NULL, configTSK_GUI_TASK_PRIORITY, configTSK_GUI_TASK_HANDLE);
	}		
}

// LCD GUI Task
void lcd_gui_task_normal( void *pvParameters ){
	unsigned char i = 0;
	
	struct tLCDRequest request;
	struct tLCDTopBar topBar;
	struct tMenu mainMenu;
	struct tLCDProgressBar progressBar;
	struct tLCDLabel lapHourLabel, lapMinuteLabel, lapSecondLabel, lapMilliLabel;
	struct tLCDLabel oldLapHourLabel, oldLapMinuteLabel, oldLapSecondLabel, oldLapMilliLabel;
	
	struct tTracklist trackList;
	struct tRecordsEntry recordTable;
	struct tRecordDataPage recordData;
	
	unsigned char tempString[20];
	unsigned char responseU8;
	unsigned int responseU32;
	unsigned int screenArgs;
	
	unsigned char lastHour, lastMinute, lastSecond;
	
	unsigned char button;
	unsigned short lcd_fsm = LCDFSM_MAINMENU;		// Useful for testing new screens!
	unsigned char redraw = TRUE;
	
	portTickType tickCount;
	
	lcdWidgetsManagerQueue = xQueueCreate(LCD_WIDGET_QUEUE_SIZE, sizeof(request));
	lcdButtonsManagerQueue	= xQueueCreate(LCD_BUTTON_QUEUE_SIZE, sizeof(unsigned char));
	
	// Make sure the battery isn't low before continuing
	fuel_low_battery_check();
	
	debug_log(DEBUG_PRIORITY_INFO, DEBUG_SENDER_LCD, "Task Started");
	
	lcd_reset();
	
	id = lcd_readID();
	
	if( lcd_readID() != LCD_DEVICE_ID){
		debug_log(DEBUG_PRIORITY_WARNING, DEBUG_SENDER_LCD, "Invalid Device ID");
	}
	
	lcd_init();
	
	// Create GUI element
	topBar = lcd_createTopBar("Josh's traq|paq", COLOR_WHITE, COLOR_BLACK);
	mainMenu = menu_init();
	
	backlight_init(BACKLIGHT_TURN_ON_DELAY);
	
	while(1){
		// See if a widget needs to be updated
		if( xQueueReceive(lcdWidgetsManagerQueue, &request, (LCD_TASK_SLEEP_TIME / portTICK_RATE_MS) ) == pdTRUE ){
			switch(request.action){
				case(LCD_REQUEST_UPDATE_BATTERY):
					lcd_updateBattery(&topBar, request.data);
					break;
					
				case(LCD_REQUEST_UPDATE_ANTENNA):
					switch(request.data){
						case(GPS_MODE_NO_FIX):
							lcd_updateAntenna(&topBar, 0);
							break;
							
						case(GPS_MODE_2D_FIX):
							lcd_updateAntenna(&topBar, 2);
							break;
							
						case(GPS_MODE_3D_FIX):
							lcd_updateAntenna(&topBar, 4);
							break;
					}
					
					break;
					
				case(LCD_REQUEST_UPDATE_CHARGE):
					switch(request.data){
						case(CHARGE_STATUS_SHUTDOWN):
							lcd_updateCharge(&topBar, COLOR_RED);
							break;
							
						case(CHARGE_STATUS_STANDBY):
							lcd_updateCharge(&topBar, COLOR_GREY);
							break;
						
						case(CHARGE_STATUS_COMPLETE):
							lcd_updateCharge(&topBar, COLOR_WHITE);
							break;
							
						case(CHARGE_STATUS_CHARGING):
							lcd_updateCharge(&topBar, COLOR_GREEN);
							break;
					}
					break;
					
				case(LCD_REQUEST_UPDATE_PERIPHERIAL):
					switch(request.data){
						case(LCD_PERIPHERIAL_SLOWER):
							lcd_drawPeripheralBox(LCD_PERIPHERIAL_SLOWER_COLOR);
							break;
						
						case(LCD_PERIPHERIAL_SAME):
							lcd_drawPeripheralBox(LCD_PERIPHERIAL_SAME_COLOR);
							break;
							
						case(LCD_PERIPHERIAL_FASTER):
							lcd_drawPeripheralBox(LCD_PERIPHERIAL_FASTER_COLOR);
							break;
					}

					lcd_resetPeripheralTimer();
					break;
					
				case(LCD_REQUEST_UPDATE_LAPTIME):
					lcd_updateLapTimer( request.data, &lapHourLabel, &lapMinuteLabel, &lapSecondLabel, &lapMilliLabel );
					break;
					
				case(LCD_REQUEST_UPDATE_OLDLAPTIME):
					lcd_updateLapTimer( request.data, &oldLapHourLabel, &oldLapMinuteLabel, &oldLapSecondLabel, &oldLapMilliLabel );
					break;
			}
		}
		
		// See if screen needs to be updated
		switch(lcd_fsm){
			// ---------------------------------
			// Splash Screen
			// ---------------------------------
			case(LCDFSM_SPLASH):
				//#include "screens/splash.h"
				break;
			
			// ---------------------------------
			// Main Menu
			// ---------------------------------
			case(LCDFSM_MAINMENU):
				#include "screens/main_menu.h"
				break;
				
			case(LCDFSM_RECORD_NEW_SESSION):
				#include "screens/record_new_session.h"
				break;
				
			case(LCDFSM_TIMED_MOTO):
				#include "screens/timed_moto.h"
				break;
				
			case(LCDFSM_REVIEW_SESSION):
				#include "screens/review_session.h"
				break;
				
			case(LCDFSM_OPTIONS):
				#include "screens/options.h"
				break;
				
			case(LCDFSM_HELP):
				#include "screens/help.h"
				break;
				
				
				
				
			// ---------------------------------
			// Main Menu -> Options
			// ---------------------------------
			case(LCDFSM_OPTIONS_CREATE_NEW_TRACK):
				#include "screens/options/create_new_track.h"
				break;
				
			case(LCDFSM_OPTIONS_NEW_TRACK_CREATED):
				#include "screens/options/new_track_created.h"
				break;
			
			case(LCDFSM_MODIFY_EXISTING_TRACK):
				#include "screens/options/modify_existing_track.h"
				break;
				
			case(LCDFSM_DISPLAY):
				#include "screens/options/display.h"
				break;
				
			case(LCDFSM_DATE_AND_TIME):
				#include "screens/options/date_and_time.h"
				break;
				
			case(LCDFSM_OPTIONS_MEMORY):
				#include "screens/options/memory.h"
				break;
				
				
			// ---------------------------------
			// Main Menu -> Options -> Display
			// ---------------------------------
			case(LCDFSM_OPTIONS_BRIGHTNESS):
				#include "screens/options/screen_brightness.h"
				break;
				
			case(LCDFSM_OPTIONS_SCREEN_TIMEOUT):
				#include "screens/options/screen_timeout.h"
				break;
			
			
			// ---------------------------------
			// Main Menu -> Options -> Erase
			// ---------------------------------
			case(LCDFSM_OPTIONS_ERASE_RECORDS):
				#include "screens/options/memory_erase.h"
				break;
				
			case(LCDFSM_OPTIONS_ERASE_RECORDS_CONFIRM):
				#include "screens/options/memory_erase_confirm.h"
				break;
				
			case(LCDFSM_OPTIONS_ERASE_TRACKS):
				#include "screens/options/memory_erase_tracks.h"
				break;
				
			case(LCDFSM_OPTIONS_ERASE_TRACKS_CONFIRM):
				#include "screens/options/memory_erase_tracks_confirm.h"
				break;
				
			// ---------------------------------
			// Main Menu -> Options -> View Memory Usage
			// ---------------------------------
			case(LCDFSM_OPTIONS_VIEW_USAGE):
				#include "screens/options/view_usage.h"
				break;

			
			// ---------------------------------
			// Main Menu -> Record New Session
			// ---------------------------------
			case(LCDFSM_SELECT_EXISTING_TRACK):
				#include "screens/normalmoto/select_existing_track.h"
				break;
				
			
			// ---------------------------------
			// Main Menu -> Timed Moto
			// ---------------------------------
			case(LCDFSM_LAP_BASED):
				#include "screens/timedmoto/timed_moto_laps.h"
				break;
				
			case(LCDFSM_TIME_BASED):
				#include "screens/timedmoto/timed_moto_time.h"
				break;
			
			
			// ---------------------------------
			// Main Menu -> Timed Moto / Record New Session -> Start Record
			// ---------------------------------
			case(LCDFSM_START_RECORD):
				#include "screens/normalmoto/start_recording.h"
				break;
				
				
			// ---------------------------------
			// Main Menu -> Help
			// ---------------------------------
			case(LCDFSM_TECHNICAL_INFO):
				#include "screens/help/technical_info.h"
				break;
				
			case(LCDFSM_MODULE_INFO):
				#include "screens/help/module_info.h"
				break;
			
			
			// ---------------------------------
			// Main Menu -> Review Session -> Detailed Info
			// ---------------------------------
			case(LCDFSM_REVIEW_DETAILED_INFO):
				#include "screens/review/detailed_info.h"
				break;
				
			// ---------------------------------
			// Main Menu -> Review Session -> Detailed Info
			// ---------------------------------
			case(LCDFSM_POWEROFF):
				#include "screens/poweroff.h"
				break;
				
			case(LCDFSM_POWEROFF_CONFIRMED):
				#include "screens/poweroff_confirmed.h"
				break;
				
				
			
			// ---------------------------------
			// Error screen - should never reach
			// ---------------------------------
			default:
				#include "screens/unknown_menu.h"
				break;
			
			
		}
	}
}



void lcd_gui_task_usb( void *pvParameters ){
	unsigned char i = 0;
	
	struct tLCDRequest request;
	
	struct tLCDProgressBar progressBar;
	struct tLCDLabel chargingStatus;
	
	lcdWidgetsManagerQueue = xQueueCreate(LCD_WIDGET_QUEUE_SIZE, sizeof(request));
	
	debug_log(DEBUG_PRIORITY_INFO, DEBUG_SENDER_LCD, "Task Started");
	
	lcd_reset();
	
	if( lcd_readID() != LCD_DEVICE_ID){
		debug_log(DEBUG_PRIORITY_WARNING, DEBUG_SENDER_LCD, "Invalid Device ID");
	}
	
	lcd_init();
	
	progressBar = lcd_createProgressBar(50, 100, 350, 50, COLOR_BLACK, COLOR_REDLINERED, COLOR_WHITE);
	chargingStatus = lcd_createLabel("Init", FONT_SMALL_POINTER, 50, 105, 100, 16, COLOR_BLACK, COLOR_WHITE);
	
	while(1){
		// See if a widget needs to be updated
		xQueueReceive(lcdWidgetsManagerQueue, &request, portMAX_DELAY);
		
		switch(request.action){
			case(LCD_REQUEST_UPDATE_BATTERY):
				lcd_updateProgressBar(&progressBar, request.data);
				break;
					
			case(LCD_REQUEST_UPDATE_CHARGE):
				switch(request.data){
					case(CHARGE_STATUS_SHUTDOWN):
						lcd_updateLabel(&chargingStatus, "Shutdown");
						break;
							
					case(CHARGE_STATUS_STANDBY):
						lcd_updateLabel(&chargingStatus, "Standby");
						break;
						
					case(CHARGE_STATUS_COMPLETE):
						lcd_updateLabel(&chargingStatus, "Complete");
						break;
							
					case(CHARGE_STATUS_CHARGING):
						lcd_updateLabel(&chargingStatus, "Charging");
						break;
				}
				break;					
		}
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
	
	// Clear the screen
	lcd_fillRGB(COLOR_WHITE);
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
	
	lcd_setCur(LCD_MAX_X - x_offset, LCD_MAX_Y - y_offset);
	
	for(j=image_x; j > 0; j--){
		for(i=image_y; i > 0; i--){
			lcd_writeData(*pixmap);
			pixmap++;
		}
		lcd_setCur(LCD_MAX_X - x_offset + j, LCD_MAX_Y - y_offset);
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
			for (x = (origin_x + xfont); x > origin_x; x--){
				if (*data & mask){ // if pixel data then put dot
					lcd_setCur(LCD_MAX_X - x, LCD_MAX_Y - y);
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
					lcd_setCur(LCD_MAX_X - x, LCD_MAX_Y-y);
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
	int numPixels;
	unsigned short newX1, newX2, newY1, newY2;
	
	numPixels = (x2 - x1 + 1) * (y1 - y2 + 1);		// Rectangle width * height
	
	// Flip coordinates 180deg (HW rev 1.3 screen flip)
	newX1 = LCD_MAX_X - x2;
	newX2 = LCD_MAX_X - x1;
	newY1 = LCD_MAX_Y - y2;
	newY2 = LCD_MAX_Y - y1;
	
	// TRICKY: SPFD5420 definitions of horizontal and vertical are different from used in our application
	lcd_writeCommand(LCD_CMD_WINDOW_VERT_ADDR_START);
	lcd_writeData(newX1);
	lcd_writeCommand(LCD_CMD_WINDOW_VERT_ADDR_END);
	lcd_writeData(newX2);
	lcd_writeCommand(LCD_CMD_WINDOW_HORZ_ADDR_START);
	lcd_writeData(newY2);
	lcd_writeCommand(LCD_CMD_WINDOW_HORZ_ADDR_END);
	lcd_writeData(newY1);
	
	lcd_setCur(newX1, newY1);
	
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


void lcd_drawLine(unsigned short x1, unsigned short y1, unsigned short x2, unsigned short y2, unsigned short color){
	// Borrowed code
	
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
			lcd_setCur(LCD_MAX_X - x, LCD_MAX_Y - y);
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
			lcd_setCur(LCD_MAX_X - x, LCD_MAX_Y - y);
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
	
	if(font_style == FONT_LARGE_POINTER){
		lcd_writeText_16x32(string, label.font, label.start_x, label.start_y, label.color_text);
	}else{
		lcd_writeText_8x16(string, label.font, label.start_x, label.start_y, label.color_text);
	}	
	return label;
}

void lcd_updateLabel(struct tLCDLabel * label, unsigned char *string) {
	lcd_drawFilledRectangle(label->start_x, label->start_y + label->height, label->start_x + label->width, label->start_y, label->color_background);
	
	if(label->font == FONT_LARGE_POINTER){
		lcd_writeText_16x32(string, label->font, label->start_x, label->start_y, label->color_text);
	}else{
		lcd_writeText_8x16(string, label->font, label->start_x, label->start_y, label->color_text);
	}
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
	//lcd_drawFilledRectangle(LCD_MIN_X, LCD_MIN_Y + LCD_TOPBAR_THICKNESS, LCD_MAX_X, LCD_MIN_Y, topBar.bcolor);
	
	// Write in the text
	lcd_writeText_8x16(string, FONT_SMALL_POINTER, LCD_TOPBAR_TEXT_XPADDING, LCD_MAX_Y - LCD_TOPBAR_TEXT_YPADDING, topBar.fcolor);
	//lcd_writeText_8x16(string, FONT_SMALL_POINTER, LCD_MIN_X, LCD_MIN_Y, COLOR_WHITE);
	
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

void lcd_updateCharge(struct tLCDTopBar *topBar, unsigned short state){
	lcd_drawFilledRectangle(LCD_MAX_X - LCD_CHARGE_X_POS,
							LCD_MAX_Y - LCD_CHARGE_Y_POS + LCD_CHARGE_HEIGHT,
							LCD_MAX_X - LCD_CHARGE_X_POS + LCD_CHARGE_WIDTH,
							LCD_MAX_Y - LCD_CHARGE_Y_POS,
							state);
}

void lcd_drawPeripheralBox(unsigned short color){
	// Vertical left most line
	lcd_drawFilledRectangle(LCD_MIN_X,
							LCD_MAX_Y - LCD_TOPBAR_THICKNESS - 1,
							LCD_MIN_X + LCD_PERIPHERIAL_THICKNESS,
							LCD_MIN_Y,
							color);
							
	// Vertical right most line
	lcd_drawFilledRectangle(LCD_MAX_X - LCD_PERIPHERIAL_THICKNESS,
							LCD_MAX_Y - LCD_TOPBAR_THICKNESS - 1,
							LCD_MAX_X,
							LCD_MIN_Y,
							color);
							
	// Horizontal top most line
	lcd_drawFilledRectangle(LCD_MIN_X,
							LCD_MAX_Y - LCD_TOPBAR_THICKNESS - 1,
							LCD_MAX_X,
							LCD_MAX_Y - LCD_TOPBAR_THICKNESS - LCD_PERIPHERIAL_THICKNESS,
							color);
							
	// Horizontal bottom most line
	lcd_drawFilledRectangle(LCD_MIN_X,
							LCD_MIN_Y + LCD_PERIPHERIAL_THICKNESS,
							LCD_MAX_X,
							LCD_MIN_Y,
							color);
}


unsigned char lcd_sendWidgetRequest(unsigned char action, unsigned int data, unsigned char delay){
	struct tLCDRequest request;
	
	request.action = action;
	request.data = data;
	
	return xQueueSend(lcdWidgetsManagerQueue, &request, delay);
}

unsigned char lcd_sendButtonRequest(unsigned char button){
	return xQueueSend(lcdButtonsManagerQueue, &button, pdFALSE);
}

void lcd_clearPeripheral( void ){
	lcd_drawPeripheralBox(COLOR_WHITE);
}

void lcd_resetPeripheralTimer( void ){
	xTimerReset( xPeripherialTimer, 20 );
}

void lcd_updateLapTimer( unsigned int ticks, struct tLCDLabel *hours, struct tLCDLabel *minutes, struct tLCDLabel *seconds, struct tLCDLabel *milli ){
	static unsigned int lastHour, lastMinute, lastSecond;
	unsigned char tempString[3];	// Two for the digits and one for the null character
	
	lcd_updateLabel(milli, itoa( (ticks % 10), &tempString, 10, FALSE));
					
	ticks = ticks / 10;		// Lop off the milliseconds
	if(ticks != lastSecond){
		lcd_updateLabel(seconds, itoa( ticks % 60, &tempString, 10, TRUE));
		lastSecond = ticks;
	}
					
	ticks = ticks / 60;		// Lop off the seconds
	if(ticks != lastMinute){
		lcd_updateLabel(minutes, itoa( ticks % 60, &tempString, 10, TRUE));
		lastMinute = ticks;	
	}
					
	ticks = ticks / 60;		// Lop off the minutes
	if(ticks != lastHour){
		lcd_updateLabel(hours, itoa( ticks, &tempString, 10, TRUE));
		lastMinute = ticks;	
	}
}