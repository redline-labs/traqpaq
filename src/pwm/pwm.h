/******************************************************************************
 *
 * Charge Control
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

#ifndef PWM_H_
#define PWM_H_

#define BACKLIGHT_FADE_DELAY		1

#define BACKLIGHT_DEFAULT_MAX		255		// Default MAX Screen Brightness
#define BACKLIGHT_DEFAULT_MIN		175		// Default MIN Screen Brightness
#define BACKLIGHT_DEFAULT_FADETIME	5		// Default Inactive Time in Seconds to Fade Screen
#define BACKLIGHT_DEFAULT_OFFTIME	30		// Default Inactive Time in Seconds to Turn Off Screen


typedef struct tPWMDisplayStatus {
	unsigned char displayOn;
	unsigned char displayFaded;
	unsigned char brightness;
};

void pwm_task_init( void );
void pwm_task( void *pvParameters );

void pwm_fadeBacklight(unsigned char endValue);
void pwm_updateBacklightDuty(unsigned char duty);


#endif /* PWM_H_ */