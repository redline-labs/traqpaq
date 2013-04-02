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


/*__attribute__((__interrupt__)) static void ISR_debug_rxd(void){
	#if( TRAQPAQ_GPS_ECHO_MODE == TRUE )
	int rxd;
	
	usart_read_char(DEBUG_USART, &rxd);
	
	usart_putchar(GPS_USART, rxd);
	#endif
}*/



void debug_task_init( void ){
	#if( TRAQPAQ_DEBUG_ENABLED == TRUE )
	debugLogQueue = xQueueCreate(DEBUG_QUEUE_SIZE, sizeof(debugRequest));
	
	usart_write_line(DEBUG_USART, "------------------------------");
	usart_putchar(DEBUG_USART, DEBUG_CR);
	usart_putchar(DEBUG_USART, DEBUG_LF);
	usart_write_line(DEBUG_USART, "traq|paq Debug Interface"	  );
	usart_putchar(DEBUG_USART, DEBUG_CR);
	usart_putchar(DEBUG_USART, DEBUG_LF);
	usart_write_line(DEBUG_USART, "------------------------------");
	usart_putchar(DEBUG_USART, DEBUG_CR);
	usart_putchar(DEBUG_USART, DEBUG_LF);
	
	xTaskCreate(debug_task, configTSK_DEBUG_TASK_NAME, configTSK_DEBUG_TASK_STACK_SIZE, NULL, configTSK_DEBUG_TASK_PRIORITY, configTSK_DEBUG_TASK_HANDLE);
	#endif
}

void debug_task( void *pvParameters ) {
		
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
				
			case(DEBUG_SENDER_ACCEL):
				usart_write_line(DEBUG_USART, "[ACCEL]: ");
				break;
				
			case(DEBUG_SENDER_ACCELX):
				usart_write_line(DEBUG_USART, "[X]: ");
				break;
				
			case(DEBUG_SENDER_ACCELY):
				usart_write_line(DEBUG_USART, "[Y]: ");
				break;
				
			case(DEBUG_SENDER_ACCELZ):
				usart_write_line(DEBUG_USART, "[Z]: ");
				break;
			
		}
		
		pdca_load_channel(DEBUG_TX_PDCA_CHANNEL, &debugRequest.text, debugRequest.strLen);
		pdca_enable(DEBUG_TX_PDCA_CHANNEL);

		while( !(pdca_get_transfer_status(DEBUG_TX_PDCA_CHANNEL) & PDCA_TRANSFER_COMPLETE) ){
			vTaskDelay( (portTickType)TASK_DELAY_MS( DEBUG_PDCA_DELAY_TIME ) );
		}

		usart_putchar(DEBUG_USART, DEBUG_CR);
		usart_putchar(DEBUG_USART, DEBUG_LF);
		
	}
}

void debug_log(enum tDebugPriority priority, enum tDebugSender sender, char *string){
	#if( TRAQPAQ_DEBUG_ENABLED == TRUE )
	struct tDebugLog request;
	
	request.priority = priority;
	request.sender = sender;
	request.strLen = strlen(string);
	
	strlcpy(&request.text, string, DEBUG_MAX_STRLEN);
	
	xQueueSend(debugLogQueue, &request, pdFALSE);
	#endif
}