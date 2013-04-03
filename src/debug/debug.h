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

#define DEBUG_MAX_STRLEN			45
#define TASK_LIST_STRLEN			400
#define DEBUG_QUEUE_SIZE			15
#define DEBUG_PDCA_DELAY_TIME		2

enum tDebugSender {
	DEBUG_SENDER_WDT,
	DEBUG_SENDER_FLASH,
	DEBUG_SENDER_USB,
	DEBUG_SENDER_GPS,
	DEBUG_SENDER_LCD,
	DEBUG_SENDER_FUEL,
	DEBUG_SENDER_BUTTON,
	DEBUG_SENDER_CHARGE,
	DEBUG_SENDER_ADC,
	DEBUG_SENDER_PWM,
	DEBUG_SENDER_ACCEL,
	DEBUG_SENDER_BATTERY_ACCUM,
	DEBUG_SENDER_BATTERY_VOLT,
	DEBUG_SENDER_ACCELX,
	DEBUG_SENDER_ACCELY,
	DEBUG_SENDER_ACCELZ,
	DEBUG_SENDER_TASKLIST
};

enum tDebugPriority {
	DEBUG_PRIORITY_INFO,
	DEBUG_PRIORITY_WARNING,
	DEBUG_PRIORITY_CRITICAL,
	DEBUG_PRIORITY_TASK
};


struct tDebugLog {
	enum tDebugPriority priority;
	enum tDebugSender sender;
	unsigned short strLen;
	unsigned char text[DEBUG_MAX_STRLEN];
};


void debug_task_init( void );
void debug_task( void *pvParameters );
void debug_log(enum tDebugPriority priority, enum tDebugSender sender, char *string);


#endif /* DEBUG_H_ */