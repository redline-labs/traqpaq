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
#include "dataflash/dataflash_manager_request.h"

xQueueHandle gpsRxdQueue;
extern xQueueHandle dataflashManagerQueue;
extern xQueueHandle lcdWidgetsManagerQueue;

struct tLCDRequest lcdRequest;
struct tGPSRequest gpsRequest;

unsigned char rxBuffer[GPS_MSG_MAX_STRLEN];
unsigned char gpsTokens[MAX_SIGNALS_SENTENCE];

unsigned char recordFlag = FALSE;


__attribute__((__interrupt__)) static void ISR_gps_rxd(void){
	int rxd;
	usart_read_char(GPS_USART, &rxd);
	xQueueSendFromISR(gpsRxdQueue, &rxd, pdFALSE);
}


void gps_task_init( void ){
	gpsRxdQueue		= xQueueCreate(GPS_RXD_QUEUE_SIZE, sizeof(int));
	
	INTC_register_interrupt(&ISR_gps_rxd, AVR32_USART3_IRQ, AVR32_INTC_INT0);
	
	xTaskCreate(gps_task, configTSK_GPS_TASK_NAME, configTSK_GPS_TASK_STACK_SIZE, NULL, configTSK_GPS_TASK_PRIORITY, configTSK_GPS_TASK_HANDLE);
}


void gps_task( void *pvParameters ){
	int rxdChar;
	unsigned char rxIndex = 0;	// Receive Character Index
	unsigned char processedRMC = FALSE, processedGGA = FALSE;
	unsigned char recordIndex = 0;
	unsigned int datestamp;		// Received Datestamp
	
	unsigned int oldtime = 0;
	
	struct tDataflashRequest dataflashRequest;
	struct tRecordDataPage gpsData;

	// Pull the GPS out of reset
	GPS_USART->ier = AVR32_USART_IER_RXRDY_MASK;
	gps_reset();
	
	while(TRUE){
		
		xQueueReceive(gpsRxdQueue, &rxdChar, portMAX_DELAY);
		
		if( rxdChar == GPS_MSG_END_CHAR ){
			if( gps_verify_checksum() ){
					
				gps_buffer_tokenize();
				
				//--------------------------
				// GGA Message Received
				//--------------------------
				if( (rxBuffer[gpsTokens[TOKEN_MESSAGE_ID] + MESSAGE_OFFSET_ID0] == ID_GGA_ID0) &
					(rxBuffer[gpsTokens[TOKEN_MESSAGE_ID] + MESSAGE_OFFSET_ID1] == ID_GGA_ID1) &
					(rxBuffer[gpsTokens[TOKEN_MESSAGE_ID] + MESSAGE_OFFSET_ID2] == ID_GGA_ID2) ){
					
					// Convert Time! Wooo!
					gpsData.data[recordIndex].latitude	= atoi( &(	rxBuffer[gpsTokens[TOKEN_GGA_LATITUDE		]]) );
					gpsData.data[recordIndex].NorS		=			rxBuffer[gpsTokens[TOKEN_GGA_NORS			]];
					gpsData.data[recordIndex].longitude	= atoi( &(	rxBuffer[gpsTokens[TOKEN_GGA_LONGITUDE		]]) );
					gpsData.data[recordIndex].EorW		=			rxBuffer[gpsTokens[TOKEN_GGA_EORW			]];
					gpsData.data[recordIndex].currentMode=atoi( &(	rxBuffer[gpsTokens[TOKEN_GGA_QUALITY		]]) ) & 0xFFFF;
					gpsData.data[recordIndex].satellites= atoi( &(	rxBuffer[gpsTokens[TOKEN_GGA_NUM_SATELLITES	]]) ) & 0xFF;
					gpsData.data[recordIndex].hdop		= atoi( &(	rxBuffer[gpsTokens[TOKEN_GGA_HDOP			]]) ) & 0xFFFF;
					gpsData.data[recordIndex].altitude	= atoi( &(	rxBuffer[gpsTokens[TOKEN_GGA_ALTITUDE		]]) ) & 0xFFFF;
						
					processedGGA = TRUE;
					
				}else
				//--------------------------
				// RMC Message Received
				//--------------------------
				if( (rxBuffer[gpsTokens[TOKEN_MESSAGE_ID] + MESSAGE_OFFSET_ID0] == ID_RMC_ID0) &
					(rxBuffer[gpsTokens[TOKEN_MESSAGE_ID] + MESSAGE_OFFSET_ID1] == ID_RMC_ID1) &
					(rxBuffer[gpsTokens[TOKEN_MESSAGE_ID] + MESSAGE_OFFSET_ID2] == ID_RMC_ID2) ){
						
					// More Converting!!
					gpsData.data[recordIndex].utc		= atoi( &(	rxBuffer[gpsTokens[TOKEN_RMC_UTC	]]) );
					gpsData.data[recordIndex].speed		= atoi( &(	rxBuffer[gpsTokens[TOKEN_RMC_SPEED	]]) ) & 0xFFFF;
					gpsData.data[recordIndex].course	= atoi( &(	rxBuffer[gpsTokens[TOKEN_RMC_TRACK	]]) ) & 0xFFFF;
						
					// Lets grab the datestamp while we are at it
					datestamp							= atoi( &(	rxBuffer[gpsTokens[TOKEN_RMC_DATE	]]) );

					processedRMC = TRUE;
					
				}
				
				
				if(processedGGA && processedRMC){
					processedGGA = FALSE;
					processedRMC = FALSE;
						
					if(recordFlag){
						recordIndex++;
				
						if(recordIndex == RECORD_DATA_PER_PAGE){
							// Need to write data!							
							dataflashRequest.command = DFMAN_REQUEST_ADD_RECORDDATA;
							dataflashRequest.pointer = &gpsData;
							dataflashRequest.length = sizeof(gpsData);
							dataflashRequest.resume = xTaskGetCurrentTaskHandle();
							xQueueSend(dataflashManagerQueue, &dataflashRequest, 20);
							vTaskSuspend(NULL);						

							recordIndex = 0;
						}
						
					}else{
						recordIndex = 0;
					}
				}  // ProcessedGGA and ProcessedRMC
				
			}	// GPS Verify Checksum
						
			rxIndex = 0;

		}else if( (rxIndex < GPS_MSG_MAX_STRLEN) && (rxdChar != GPS_PERIOD) ){
				rxBuffer[rxIndex++] = (rxdChar & 0xFF);
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
	unsigned char index = 0;
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