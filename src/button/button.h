/******************************************************************************
 *
 * Button Interface
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

#ifndef BUTTON_H_
#define BUTTON_H_

#define BUTTON_TIMER_INCREMENT		50			// Interval to check button status in milliseconds
#define BUTTON_LONG_PRESS_TIME		1000		// Threshold for qualifying a long-press in milliseconds

#define BUTTON_LONG_PRESS_TIMER_VALUE	BUTTON_LONG_PRESS_TIME/BUTTON_TIMER_INCREMENT


// Assign these by bits so we can use masks in the LCD task
#define BUTTON_UP					0b00000001
#define BUTTON_DOWN					0b00000010
#define BUTTON_SELECT				0b00000100
#define BUTTON_BACK					0b00001000

#define BUTTON_LONG_PRESS_MASK		0b10000000

#define BUTTON_LONG_UP				(BUTTON_UP		| BUTTON_LONG_PRESS_MASK)
#define BUTTON_LONG_DOWN			(BUTTON_DOWN	| BUTTON_LONG_PRESS_MASK)
#define BUTTON_LONG_SELECT			(BUTTON_SELECT	| BUTTON_LONG_PRESS_MASK)
#define BUTTON_LONG_BACK			(BUTTON_BACK	| BUTTON_LONG_PRESS_MASK)

#define POWER_ON_MODE_BUTTON		0
#define POWER_ON_MODE_USB			1

struct tButtonStatusFlags{
	unsigned char powerOnMethod;
};


void buttons_task_init();
void buttons_task_normal( void *pvParameters );
void buttons_task_usb( void *pvParameters );



#endif /* BUTTON_H_ */