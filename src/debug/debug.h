/******************************************************************************
 *
 * Debug
 *
 * - Compiler:          GNU GCC for AVR32
 * - Supported devices: traq|paq hardware version 1.3
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


#ifndef DEBUG_H_
#define DEBUG_H_

#define DEBUG_CR					0x0D
#define DEBUG_LF					0x0A

#define DEBUG_MAX_STRLEN			40
#define DEBUG_QUEUE_SIZE			15


#define DEBUG_SENDER_WDT			0
#define DEBUG_SENDER_FLASH		1
#define DEBUG_SENDER_USB			2
#define DEBUG_SENDER_GPS			3
#define DEBUG_SENDER_LCD			4
#define DEBUG_SENDER_FUEL			5
#define DEBUG_SENDER_BUTTON			6
#define DEBUG_SENDER_CHARGE			7
#define DEBUG_SENDER_ADC			8
#define DEBUG_SENDER_PWM			9

#define DEBUG_SENDER_BATTERY_ACCUM	10
#define DEBUG_SENDER_BATTERY_VOLT	11

#define DEBUG_PRIORITY_INFO			0
#define DEBUG_PRIORITY_WARNING		1
#define DEBUG_PRIORITY_CRITICAL		2

struct tDebugLog {
	unsigned char priority;
	unsigned char sender;
	unsigned char text[DEBUG_MAX_STRLEN];
};


void debug_task_init( void );
void debug_task( void *pvParameters );
void debug_log(unsigned char priority, unsigned char sender, char *string);


#endif /* DEBUG_H_ */