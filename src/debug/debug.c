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

#include "asf.h"
#include "hal.h"
#include "string.h"

xQueueHandle debugLogQueue;

struct tDebugLog debugRequest;


__attribute__((__interrupt__)) static void ISR_debug_rxd(void){
	#if( TRAQPAQ_GPS_ECHO_MODE == TRUE )
	int rxd;
	
	usart_read_char(DEBUG_USART, &rxd);
	
	usart_putchar(GPS_USART, rxd);
	#endif
}



void debug_task_init( void ){
	#if( TRAQPAQ_DEBUG_ENABLED == TRUE )
	debugLogQueue = xQueueCreate(DEBUG_QUEUE_SIZE, sizeof(debugRequest));
	
	xTaskCreate(debug_task, configTSK_DEBUG_TASK_NAME, configTSK_DEBUG_TASK_STACK_SIZE, NULL, configTSK_DEBUG_TASK_PRIORITY, configTSK_DEBUG_TASK_HANDLE);
	#endif
}

void debug_task( void *pvParameters ) {
	usart_write_line(DEBUG_USART, "------------------------------");
	usart_putchar(DEBUG_USART, DEBUG_CR);
	usart_putchar(DEBUG_USART, DEBUG_LF);
	usart_write_line(DEBUG_USART, "traq|paq Debug Interface"	  );
	usart_putchar(DEBUG_USART, DEBUG_CR);
	usart_putchar(DEBUG_USART, DEBUG_LF);
	usart_write_line(DEBUG_USART, "------------------------------");
	usart_putchar(DEBUG_USART, DEBUG_CR);
	usart_putchar(DEBUG_USART, DEBUG_LF);
	
	#if (TRAQPAQ_GPS_ECHO_MODE == TRUE)
	INTC_register_interrupt(&ISR_debug_rxd, AVR32_USART2_IRQ, AVR32_INTC_INT0);
	#endif
	
	while( TRUE ){
		xQueueReceive(debugLogQueue, &debugRequest, portMAX_DELAY);
		
		switch(debugRequest.priority){
			case(DEBUG_PRIORITY_INFO):
				usart_write_line(DEBUG_USART, "INFO ");
				break;
				
			case(DEBUG_PRIORITY_WARNING):
				usart_write_line(DEBUG_USART, "WARNING ");
				break;
			
			case(DEBUG_PRIORITY_CRITICAL):
				usart_write_line(DEBUG_USART, "CRITICAL ");
				break;
		}
		
		
		switch(debugRequest.sender){
			case(DEBUG_SENDER_WDT):
				usart_write_line(DEBUG_USART, "[WDT]: ");
				break;
				
			case(DEBUG_SENDER_FLASH):
				usart_write_line(DEBUG_USART, "[FLASH]: ");
				break;
				
			case(DEBUG_SENDER_USB):
				usart_write_line(DEBUG_USART, "[USB]: ");
				break;
				
			case(DEBUG_SENDER_GPS):
				usart_write_line(DEBUG_USART, "[GPS]: ");
				break;
				
			case(DEBUG_SENDER_LCD):
				usart_write_line(DEBUG_USART, "[LCD]: ");
				break;
				
			case(DEBUG_SENDER_FUEL):
				usart_write_line(DEBUG_USART, "[FUEL]: ");
				break;
				
			case(DEBUG_SENDER_BUTTON):
				usart_write_line(DEBUG_USART, "[BUTTON]: ");
				break;
				
			case(DEBUG_SENDER_CHARGE):
				usart_write_line(DEBUG_USART, "[CHARGE]: ");
				break;
				
			case(DEBUG_SENDER_ADC):
				usart_write_line(DEBUG_USART, "[ADC]: ");
				break;
				
			case(DEBUG_SENDER_PWM):
				usart_write_line(DEBUG_USART, "[PWM]: ");
				break;
				
			case(DEBUG_SENDER_BATTERY_ACCUM):
				usart_write_line(DEBUG_USART, "[ACCUM]: ");
				break;
				
			case(DEBUG_SENDER_BATTERY_VOLT):
				usart_write_line(DEBUG_USART, "[VOLT]: ");
				break;
			
		}
		
		usart_write_line(DEBUG_USART, &(debugRequest.text));
		usart_putchar(DEBUG_USART, DEBUG_CR);
		usart_putchar(DEBUG_USART, DEBUG_LF);
		
	}
}

void debug_log(unsigned char priority, unsigned char sender, char *string){
	#if( TRAQPAQ_DEBUG_ENABLED == TRUE )
	struct tDebugLog request;
	
	request.priority = priority;
	request.sender = sender;
	
	strlcpy(&request.text, string, DEBUG_MAX_STRLEN);
	
	xQueueSend(debugLogQueue, &request, pdFALSE);
	#endif
}