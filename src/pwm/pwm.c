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

// Lookup tables
const unsigned char pwmFadeTable[] = {
	0,   1,   1,   1,   1,   1,   1,   1,   1,
	1,   1,   1,   1,   1,   1,   1,   1,   1,
	1,   1,   1,   1,   1,   1,   1,   1,   1,
	1,   1,   1,   1,   1,   2,   2,   2,   2,
	2,   2,   2,   2,   2,   2,   2,   2,   2,
	2,   2,   2,   2,   2,   2,   3,   3,   3,
	3,   3,   3,   3,   3,   3,   3,   3,   3,
	3,   4,   4,   4,   4,   4,   4,   4,   4,
	4,   4,   4,   5,   5,   5,   5,   5,   5,
	5,   5,   6,   6,   6,   6,   6,   6,   6,
	7,   7,   7,   7,   7,   7,   8,   8,   8,
	8,   8,   8,   9,   9,   9,   9,   10,  10,
	10,  10,  10,  11,  11,  11,  11,  12,  12,
	12,  13,  13,  13,  13,  14,  14,  14,  15,
	15,  15,  16,  16,  16,  17,  17,  18,  18,
	18,  19,  19,  20,	20,  20,  21,  21,  22,
	22,  23,  23,  24,  24,  25,  26,  26,  27,
	27,  28,  29,  29,  30,  31,  31,  32,  33,
	33,  34,  35,  36,  36,  37,  38,  39,  40,
	41,  42,  42,  43,  44,  45,  46,  47,  48,
	50,  51,  52,  53,  54,  55,  57,  58,  59,
	60,  62,  63,  64,  66,  67,  69,  70,  72,
	74,  75,  77,  79,  80,  82,  84,  86,  88,	
	90,  91,  94,  96,  98,  100, 102, 104, 107,
	109, 111, 114, 116, 119, 122, 124, 127, 130,
	133, 136, 139, 142, 145, 148, 151, 155, 158,
	161, 165, 169, 172, 176, 180, 184, 188, 192,
	196, 201, 205, 210, 214, 219, 224, 229, 234,
	239, 244, 250, 255
};

extern xQueueHandle lcdButtonsManagerQueue;
xQueueHandle pwmManagerQueue;

struct tPWMDisplayStatus displayStatus;

extern struct tUserPrefs userPrefs;

void pwm_task_init( unsigned char mode ){
	pwmManagerQueue = xQueueCreate(1, sizeof(unsigned char));
	
	if(mode == TASK_MODE_NORMAL){
		xTaskCreate(pwm_task_normal, configTSK_PWM_TASK_NAME, configTSK_PWM_TASK_STACK_SIZE, NULL, configTSK_PWM_TASK_PRIORITY, configTSK_PWM_TASK_HANDLE);
	}else{
		xTaskCreate(pwm_task_usb, configTSK_PWM_TASK_NAME, configTSK_PWM_TASK_STACK_SIZE, NULL, configTSK_PWM_TASK_PRIORITY, configTSK_PWM_TASK_HANDLE);
	}		
}

void pwm_task_normal( void *pvParameters ){
	unsigned char flag;
	
	// Initialize the display status
	displayStatus.brightness = 0;
	displayStatus.displayOn = FALSE;
	displayStatus.displayFaded = FALSE;
	
	debug_log(DEBUG_PRIORITY_INFO, DEBUG_SENDER_PWM, "Task Started");
	
	// Turn Boost Converter On
	gpio_set_gpio_pin(PM_SHDN1);
	
	vTaskDelay( (portTickType)TASK_DELAY_MS(BOOST_CONVERTER_TURN_ON_TIME) );
	
	pwm_fadeBacklight(userPrefs.screenPWMMax);
	
	while( TRUE ){
		if( xQueueReceive(pwmManagerQueue, &flag, (portTickType)TASK_DELAY_S(userPrefs.screenFadeTime) ) == pdFALSE ){
			if(displayStatus.displayFaded == FALSE){
				pwm_fadeBacklight(userPrefs.screenPWMMin);
			}
			
		}else{
			if(displayStatus.displayFaded == TRUE){
				pwm_fadeBacklight(userPrefs.screenPWMMax);
			}			
		}
	}
}


void pwm_task_usb( void *pvParameters ){
	unsigned char flag;
	
	// Initialize the display status
	displayStatus.brightness = 0;
	displayStatus.displayOn = FALSE;
	displayStatus.displayFaded = FALSE;
	
	debug_log(DEBUG_PRIORITY_INFO, DEBUG_SENDER_PWM, "Task Started");
	
	while( TRUE ){
		if( xQueueReceive(pwmManagerQueue, &flag, (portTickType)TASK_DELAY_S(userPrefs.screenFadeTime) ) == pdFALSE ){
			if(displayStatus.displayOn == TRUE){
				pwm_fadeBacklight(BACKLIGHT_PWM_MIN);
				gpio_clr_gpio_pin(PM_SHDN1);
			}
			
		}else{
			if(displayStatus.displayOn == FALSE){
				gpio_set_gpio_pin(PM_SHDN1);
				pwm_fadeBacklight(userPrefs.screenPWMMax);
			}			
		}
	}
}

void pwm_updateBacklightDuty(unsigned char duty){
	#if(TRAQPAQ_HW_PWM_ENABLED)
	volatile avr32_pwm_channel_t pwm_channel_0 = {.ccnt = BACKLIGHT_PWM_ID };
		
	pwm_channel_0.CMR.calg = PWM_MODE_LEFT_ALIGNED;       	// Channel mode.
	pwm_channel_0.CMR.cpol = PWM_POLARITY_HIGH;            	// Channel polarity.
	pwm_channel_0.CMR.cpd = PWM_UPDATE_DUTY;              	// Not used the first time.
	pwm_channel_0.CMR.cpre = AVR32_PWM_CPRE_MCK;  			// Channel prescaler.
	pwm_channel_0.cprd = BACKLIGHT_PWM_MAX;  				// Channel period.
	
	pwm_channel_0.cdty = duty;
    pwm_channel_0.cupd = pwm_channel_0.cdty;

    pwm_async_update_channel(BACKLIGHT_PWM_CHANNEL, &pwm_channel_0);
	#endif
}

void pwm_fadeBacklight(unsigned char endValue){
	unsigned char i;
	
	if( displayStatus.brightness < endValue){
		// Fading backlight up
		for (i = displayStatus.brightness; i < endValue; i++){
			pwm_updateBacklightDuty(pwmFadeTable[i]);
			vTaskDelay( (portTickType)TASK_DELAY_MS(BACKLIGHT_FADE_DELAY) );
		}
	}else{
		// Fading backlight down
		for (i = displayStatus.brightness; i > endValue; i--){
			pwm_updateBacklightDuty(pwmFadeTable[i]);
			vTaskDelay( (portTickType)TASK_DELAY_MS(BACKLIGHT_FADE_DELAY) );
		}		
	}
	
	displayStatus.brightness = endValue;
	
	if(displayStatus.brightness == BACKLIGHT_PWM_MIN){
		displayStatus.displayOn = FALSE;
		displayStatus.displayFaded = FALSE;
		
	}else if(displayStatus.brightness == BACKLIGHT_PWM_MAX){
		displayStatus.displayOn = TRUE;
		displayStatus.displayFaded = FALSE;
		
	}else{
		displayStatus.displayOn = TRUE;
		displayStatus.displayFaded = TRUE;
	}
}


void pwm_send_request( void ){
	unsigned char flag;
	xQueueSend(pwmManagerQueue, &flag, 0);
}