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


#ifndef CONTROL_FSM_H_
#define CONTROL_FSM_H_

// ---------------------------------
// Main Menu
// ---------------------------------
#define LCDFSM_MAINMENU					000
#define LCDFSM_RECORD_NEW_SESSION		010
#define LCDFSM_TIMED_MOTO				020
#define LCDFSM_REVIEW_SESSION			030
#define LCDFSM_OPTIONS					040
#define LCDFSM_HELP						050

// ---------------------------------
// Main Menu -> Record New Session
// ---------------------------------
#define LCDFSM_SELECT_EXISTING_TRACK	100
#define LCDFSM_CREATE_NEW_TRACK			110


// ---------------------------------
// Main Menu -> Timed Moto
// ---------------------------------
#define LCDFSM_LAP_BASED				200
#define LCDFSM_TIME_BASED				210


// ---------------------------------
// Main Menu -> Timed Moto / Record New Session -> Start Record
// ---------------------------------
#define LCDFSM_START_RECORD				300



// ---------------------------------
// Main Menu -> Review Session
// ---------------------------------


// ---------------------------------
// Main Menu -> Options
// ---------------------------------
#define LCDFSM_OPTIONS_CREATE_NEW_TRACK	400
#define LCDFSM_MODIFY_EXISTING_TRACK	410
#define LCDFSM_DISPLAY					420
#define LCDFSM_DATE_AND_TIME			430

// ---------------------------------
// Main Menu -> Options -> Create New Track
// ---------------------------------
#define LCDFSM_OPTIONS_NEW_TRACK_CREATED 401


// ---------------------------------
// Main Menu -> Options -> Display
// ---------------------------------
#define LCDFSM_OPTIONS_BRIGHTNESS		421
#define LCDFSM_OPTIONS_SCREEN_TIMEOUT	422

// ---------------------------------
// Main Menu -> Options -> Date and Time
// ---------------------------------


// ---------------------------------
// Main Menu -> Help
// ---------------------------------
#define LCDFSM_HOW_TO_USE				500
#define LCDFSM_ABOUT					510
#define LCDFSM_TECHNICAL_INFO			520
#define LCDFSM_DISCLAIMER				530
#define LCDFSM_MODULE_INFO				540




#endif /* CONTROL_FSM_H_ */