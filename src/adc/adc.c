/******************************************************************************
 *
 * ADC
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

#include <asf.h>
#include "hal.h"

void adc_task_init( void ){
	xTaskCreate(adc_task, configTSK_ADC_TASK_NAME, configTSK_ADC_TASK_STACK_SIZE, NULL, configTSK_ADC_TASK_PRIORITY, configTSK_ADC_TASK_HANDLE);
}

// ADC Task
void adc_task( void *pvParameters ){
	struct tADCvalues adcValues;
	
	debug_log(DEBUG_PRIORITY_INFO, DEBUG_SENDER_ADC, "Task Started");
	
	while(TRUE){
		// Turn on ADC VREF and allow it to settle
		gpio_set_gpio_pin(ADC_VREF_EN);
		vTaskDelay( (portTickType)TASK_DELAY_MS(ADC_VREF_SETTLE_TIME) );
	
		// Start conversions
		adc_start(ADC);
		
		adcValues.main	= adc_get_value(ADC, ADC_3V3_CHANNEL);
		adcValues.vcc	= adc_get_value(ADC, ADC_VCC_CHANNEL);
		adcValues.vee	= adc_get_value(ADC, ADC_VEE_CHANNEL);
		
		// Turn off reference
		gpio_clr_gpio_pin(ADC_VREF_EN);
		
		// Sleep!
		vTaskDelay( (portTickType)TASK_DELAY_MS(ADC_SLEEP_TIME) );
	}
	
}	