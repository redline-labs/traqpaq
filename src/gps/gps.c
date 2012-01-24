/******************************************************************************
 *
 * GPS Interface
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

// Green = RXD on FT232
// Blue = TXD on FT232


#include "asf.h"
#include "drivers.h"


static xQueueHandle queueGPSrxd;
unsigned char rxBuffer[GPS_MSG_MAX_STRLEN];
unsigned char gpsTokens[MAX_SIGNALS_SENTENCE];

__attribute__((__interrupt__)) static void ISR_gps_rxd(void){
	int rxd;
	
	usart_read_char(GPS_USART, &rxd);
	xQueueSendFromISR(queueGPSrxd, &rxd, pdFALSE);
}


void gps_task_init( void ){
	queueGPSrxd = xQueueCreate(GPS_QUEUE_SIZE, sizeof(int));

	INTC_register_interrupt(&ISR_gps_rxd, AVR32_USART0_IRQ, AVR32_INTC_INT0);
	GPS_USART->ier = AVR32_USART_IER_RXRDY_MASK;
	
	xTaskCreate(gps_task, configTSK_GPS_TASK_NAME, configTSK_GPS_TASK_STACK_SIZE, NULL, configTSK_GPS_TASK_PRIORITY, NULL);
}


void gps_task( void *pvParameters ){
	int rxdChar;
	unsigned char rxIndex = 0;
	unsigned char i;
	
	gps_reset();
	
	while(TRUE){
		// Wait until we received a character
		if( xQueueReceive(queueGPSrxd, &rxdChar, portMAX_DELAY) == pdTRUE ){
			if( (rxdChar == GPS_MSG_END_CHAR) && gps_verify_checksum() ){
				gps_buffer_tokenize();
				
				//--------------------------
				// GGA Message Received
				//--------------------------
				if( (rxBuffer[gpsTokens[TOKEN_MESSAGE_ID] + MESSAGE_OFFSET_ID0] == ID_GGA_ID0) &
					(rxBuffer[gpsTokens[TOKEN_MESSAGE_ID] + MESSAGE_OFFSET_ID1] == ID_GGA_ID1) &
					(rxBuffer[gpsTokens[TOKEN_MESSAGE_ID] + MESSAGE_OFFSET_ID2] == ID_GGA_ID2) ){
					
					asm("nop");
					
					
					
					
					
				}else
				//--------------------------
				// RMC Message Received
				//--------------------------
				if( (rxBuffer[gpsTokens[TOKEN_MESSAGE_ID] + MESSAGE_OFFSET_ID0] == ID_RMC_ID0) &
					(rxBuffer[gpsTokens[TOKEN_MESSAGE_ID] + MESSAGE_OFFSET_ID1] == ID_RMC_ID1) &
					(rxBuffer[gpsTokens[TOKEN_MESSAGE_ID] + MESSAGE_OFFSET_ID2] == ID_RMC_ID2) ){
						
					asm("nop");
					
					
					
					
					
					
					
					
					
					
				}else{
					// Unrecognized Message!
					asm("nop");
					
				}
				
				rxIndex = 0;
				
			}else{
				// Store the data in the buffer
				if(rxBuffer < GPS_MSG_MAX_STRLEN)
				rxBuffer[rxIndex++] = (rxdChar & 0xFF);
			}
		}
	}
}


void gps_reset( void ){
	gpio_clr_gpio_pin(GPS_RESET);
	vTaskDelay( (portTickType)TASK_DELAY_MS(GPS_RESET_TIME) );
	gpio_set_gpio_pin(GPS_RESET);
	vTaskDelay( (portTickType)TASK_DELAY_MS(GPS_RESET_TIME) );
}


void gps_buffer_tokenize( void ){
	unsigned char index;
	unsigned char signalPosIndex = 0;

	// Find the start of the NMEA sentence
	while( (index < GPS_MSG_MAX_STRLEN) && (rxBuffer[index++] != GPS_MSG_START_CHAR) );
	
	// Store the location!
	gpsTokens[signalPosIndex++] = index;
				
				
	// Replace commas with null!
	while(index <= GPS_MSG_MAX_STRLEN){
		if(rxBuffer[index] == GPS_DELIMITER_CHAR){
			rxBuffer[index] = GPS_NULL;
			gpsTokens[signalPosIndex++] = index + 1;
		}
		index++;
	}
	
}


unsigned char gps_verify_checksum( void ){
	unsigned char index = 0;
	unsigned char crcCalculated = 0;	// Seed the CRC with zero
	unsigned char crcReceived;
	
	// Find the start of the NMEA sentence
	while( (index < GPS_MSG_MAX_STRLEN) && (rxBuffer[index++] != GPS_MSG_START_CHAR) );
	
	// Start calculating the checksum
	while( (index < GPS_MSG_MAX_STRLEN) && (rxBuffer[index] != GPS_CHECKSUM_CHAR) ){
			crcCalculated ^= rxBuffer[index++];
	}
	
	// Skip the GPS_CHECKSUM_CHAR
	index++;
				
	// Convert the received checksum
	if(rxBuffer[index] >= 'A'){
		rxBuffer[index] += 10 - 'A';
	}else{
		rxBuffer[index] -= '0';
	}
				
	if(rxBuffer[index+1] >= 'A'){
		rxBuffer[index+1] += 10 - 'A';
	}else{
		rxBuffer[index+1] -= '0';
	}
				
	crcReceived = ((rxBuffer[index] & 0xF) << 4) + (rxBuffer[index+1] & 0xF);
				
	// Compare!
	if( crcReceived == crcCalculated ){
		return TRUE;
	}
	
	return FALSE;
}