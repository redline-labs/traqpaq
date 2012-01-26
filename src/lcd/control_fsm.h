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

// Main Menu
#define LCDFSM_MAINMENU					0
#define LCDFSM_RECORD_NEW_SESSION		1
#define LCDFSM_TIMED_MOTO				2
#define LCDFSM_REVIEW_SESSION			3
#define LCDFSM_OPTIONS					4
#define LCDFSM_HELP						5

// Main Menu -> Record New Session
#define LCDFSM_SELECT_EXISTING_TRACK	100
#define LCDFSM_CREATE_NEW_TRACK			101


// Main Menu -> Timed Moto
#define LCDFSM_LAP_BASED				200
#define LCDFSM_TIME_BASED				201


// Main Menu -> Review Session


// Main Menu -> Options
#define LCDFSM_OPTIONS_CREATE_NEW_TRACK	400
#define LCDFSM_MODIFY_EXISTING_TRACK	401
#define LCDFSM_DISPLAY					402
#define LCDFSM_DATE_AND_TIME			403


// Main Menu -> Help
#define LCDFSM_HOW_TO_USE				500
#define LCDFSM_ABOUT					501
#define LCDFSM_DISCLAIMER				502




#endif /* CONTROL_FSM_H_ */