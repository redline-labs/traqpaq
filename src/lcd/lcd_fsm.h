/******************************************************************************
 *
 * LCD interface
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


#ifndef LCD_FSM_H_
#define LCD_FSM_H_

enum tLcdScreens {
	// ---------------------------------
	// Splash Screen
	// ---------------------------------
	LCDFSM_SPLASH,


	// ---------------------------------
	// Main Menu
	// ---------------------------------
	LCDFSM_MAINMENU,
	LCDFSM_RECORD_NEW_SESSION,
	LCDFSM_TIMED_MOTO,
	LCDFSM_REVIEW_SESSION,
	LCDFSM_OPTIONS,
	LCDFSM_HELP,

	// ---------------------------------
	// Main Menu -> Record New Session
	// ---------------------------------
	LCDFSM_SELECT_EXISTING_TRACK,
	LCDFSM_CREATE_NEW_TRACK,


	// ---------------------------------
	// Main Menu -> Timed Moto
	// ---------------------------------
	LCDFSM_LAP_BASED,
	LCDFSM_TIME_BASED,


	// ---------------------------------
	// Main Menu -> Timed Moto / Record New Session -> Start Record
	// ---------------------------------
	LCDFSM_START_RECORD,


	// ---------------------------------
	// Main Menu -> Options
	// ---------------------------------
	LCDFSM_OPTIONS_CREATE_NEW_TRACK,
	LCDFSM_MODIFY_EXISTING_TRACK,
	LCDFSM_DISPLAY,
	LCDFSM_DATE_AND_TIME,
	LCDFSM_OPTIONS_MEMORY,

	// ---------------------------------
	// Main Menu -> Options -> Create New Track
	// ---------------------------------
	LCDFSM_OPTIONS_NEW_TRACK_CREATED,


	// ---------------------------------
	// Main Menu -> Options -> Display
	// ---------------------------------
	LCDFSM_OPTIONS_BRIGHTNESS,
	LCDFSM_OPTIONS_SCREEN_TIMEOUT,

	// ---------------------------------
	// Main Menu -> Options -> Memory
	// ---------------------------------
	LCDFSM_OPTIONS_ERASE_RECORDS,
	LCDFSM_OPTIONS_ERASE_RECORDS_CONFIRM,
	LCDFSM_OPTIONS_VIEW_USAGE,

	LCDFSM_OPTIONS_ERASE_TRACKS,
	LCDFSM_OPTIONS_ERASE_TRACKS_CONFIRM,

	// ---------------------------------
	// Main Menu -> Options -> Create New Track
	// ---------------------------------


	// ---------------------------------
	// Main Menu -> Help
	// ---------------------------------
	LCDFSM_HOW_TO_USE,
	LCDFSM_ABOUT,
	LCDFSM_TECHNICAL_INFO,
	LCDFSM_DISCLAIMER,
	LCDFSM_MODULE_INFO,

	// ---------------------------------
	// Main Menu -> Review Session
	// ---------------------------------
	LCDFSM_REVIEW_DETAILED_INFO,

	// ---------------------------------
	// Power Off
	// ---------------------------------
	LCDFSM_POWEROFF,
	LCDFSM_POWEROFF_CONFIRMED,
	
};


#endif /* LCD_FSM_H_ */