/******************************************************************************
 *
 * Charge Control
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

#ifndef PWM_H_
#define PWM_H_

#include "hal.h"

#define BACKLIGHT_TURN_ON_DELAY		200		// Time in milliseconds to delay turning on backlight initally

#define BACKLIGHT_DEFAULT_MAX		255		// Default MAX Screen Brightness
#define BACKLIGHT_DEFAULT_MIN		55		// Default MIN Screen Brightness
#define BACKLIGHT_DEFAULT_FADETIME	30		// Default Inactive Time in Seconds to Fade Screen
#define BACKLIGHT_DEFAULT_OFFTIME	120		// Default Inactive Time in Seconds to Turn Off Screen

#define boost_converter_on()		gpio_set_gpio_pin(PM_SHDN1)
#define boost_converter_off()		gpio_clr_gpio_pin(PM_SHDN1)

struct tDisplayStatusFlags{
	unsigned char isReady;
	unsigned char isFaded;
	unsigned char isOff;
};


void backlight_init( unsigned short delay );
void backlight_updateDuty(unsigned char duty);

void backlight_resetTimer( void );
void backlight_fadeTimerCallback( void );
void backlight_offTimerCallback( void );
void backlight_timerOffCallback( void );
void backlight_delayTimerCallback( void );
void backlight_stopTimers( void );

#endif /* PWM_H_ */