/******************************************************************************
 *
 * ADC Include
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

#ifndef ADC_H_
#define ADC_H_

#define ADC_VREF_SETTLE_TIME			1		// Time in milliseconds for VREF to settle
												// Testing shows VREF is settled in 200uS
	
#define ADC_SLEEP_TIME					5000	// Time in milliseconds for ADC task to sleep

struct tADCvalues {
	unsigned short main;
	unsigned short vcc;
	unsigned short vee;
};


void adc_task_init( void );
void adc_task( void *pvParameters );


#endif /* ADC_H_ */