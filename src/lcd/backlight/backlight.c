/******************************************************************************
 *
 * PWM Control
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

/* TODO: Add software framework include drivers below */
#include "asf.h"
#include "drivers.h"
#include "dataflash/dataflash_layout.h"

xTimerHandle xBacklightFadeTimer;
xTimerHandle xBacklightDelayTimer;
xTimerHandle xBacklightOffTimer;

extern struct tUserPrefs userPrefs;
struct tDisplayStatus displayStatus;

void backlight_init( unsigned short delay ){
	displayStatus.isReady = FALSE;
	displayStatus.isOff = TRUE;
	displayStatus.isFaded = FALSE;
	
	xBacklightFadeTimer = xTimerCreate( "BacklightFade", userPrefs.screenFadeTime * configTICK_RATE_HZ, pdFALSE, NULL, backlight_fadeTimerCallback );
	xBacklightOffTimer = xTimerCreate( "BacklightOff", userPrefs.screenOffTime * configTICK_RATE_HZ, pdFALSE, NULL, backlight_offTimerCallback );
	xBacklightDelayTimer = xTimerCreate( "BacklightDelay", delay * portTICK_RATE_MS, pdFALSE, NULL, backlight_delayTimerCallback );
	
	xTimerStart( xBacklightDelayTimer, pdFALSE);
}


void backlight_updateDuty(unsigned char duty){
	avr32_pwm_channel_t pwm_channel = {.ccnt = BACKLIGHT_PWM_ID };
		
	pwm_channel.CMR.calg = PWM_MODE_LEFT_ALIGNED;       	// Channel mode.
	pwm_channel.CMR.cpol = PWM_POLARITY_HIGH;            	// Channel polarity.
	pwm_channel.CMR.cpd = PWM_UPDATE_DUTY;              	// Not used the first time.
	pwm_channel.CMR.cpre = AVR32_PWM_CPRE_MCK;  			// Channel prescaler.
	
    pwm_channel.cupd = duty;

    pwm_async_update_channel(BACKLIGHT_PWM_CHANNEL, &pwm_channel);
}

void backlight_resetTimer( void ){
	boost_converter_on();
	xTimerReset( xBacklightFadeTimer, 20 );
	xTimerReset( xBacklightOffTimer, 20 );
	backlight_updateDuty(userPrefs.screenPWMMax);
	
	displayStatus.isFaded = FALSE;
	displayStatus.isOff = FALSE;
	displayStatus.isReady = TRUE;
}


void backlight_fadeTimerCallback( void ){
	backlight_updateDuty(userPrefs.screenPWMMin);
	displayStatus.isFaded = TRUE;
	displayStatus.isOff = FALSE;
}

void backlight_offTimerCallback( void ){
	boost_converter_off();
	displayStatus.isFaded = FALSE;
	displayStatus.isOff = TRUE;
}

void backlight_delayTimerCallback( void ){
	boost_converter_on();
	backlight_updateDuty(userPrefs.screenPWMMax);
	
	xTimerStart( xBacklightFadeTimer, pdFALSE);
	xTimerStart( xBacklightOffTimer, pdFALSE);
	
	displayStatus.isOff = FALSE;
	displayStatus.isFaded = FALSE;
	displayStatus.isReady = TRUE;
}

void backlight_stopTimers( void ){
	xTimerStop(xBacklightFadeTimer, pdFALSE);
	xTimerStop(xBacklightOffTimer, pdFALSE);
	displayStatus.isReady = FALSE;
}