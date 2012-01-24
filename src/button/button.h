/******************************************************************************
 *
 * Button Interface
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

#ifndef BUTTON_H_
#define BUTTON_H_

#define BUTTON_TIMER_INCREMENT		40			// Interval to check button status in milliseconds
#define BUTTON_LONG_PRESS_TIME		1000		// Threshold for qualifying a long-press in milliseconds

#define BUTTON_LONG_PRESS_TIMER_VALUE	BUTTON_LONG_PRESS_TIME/BUTTON_TIMER_INCREMENT

#define BUTTON_UP					0
#define BUTTON_DOWN					1
#define BUTTON_SELECT				2
#define BUTTON_BACK					3



void buttons_task_init( void );
void buttons_task( void *pvParameters );



#endif /* BUTTON_H_ */