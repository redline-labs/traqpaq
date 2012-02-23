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
#include "math.h"

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
	unsigned int rxdChar;
	unsigned char rxIndex = 0;	// Receive Character Index
	unsigned char processedRMC = FALSE, processedGGA = FALSE;
	unsigned char recordIndex = 0;
	
	signed int oldLatitude = 0;
	signed int oldLongitude = 0;

	struct tDataflashRequest dataflashRequest;
	struct tRecordDataPage gpsData;
	struct tGPSSetLine finishLine;
	struct tGPSSetPoint finishPoint;
	
	// Test finish line
	finishPoint.heading = 2666;
	finishPoint.longitude = -83453003;
	finishPoint.latitude = 42570383;
	
	finishLine = gps_find_finish_line(finishPoint);

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
					gpsData.data[recordIndex].latitude = gps_convert_to_decimal_degrees(gpsData.data[recordIndex].latitude);
					
					if( rxBuffer[gpsTokens[TOKEN_GGA_NORS]] == GPS_SOUTH){
						gpsData.data[recordIndex].latitude *= -1;
					}
					
					gpsData.data[recordIndex].longitude	= atoi( &(	rxBuffer[gpsTokens[TOKEN_GGA_LONGITUDE		]]) );
					gpsData.data[recordIndex].longitude = gps_convert_to_decimal_degrees(gpsData.data[recordIndex].longitude);
					
					if( rxBuffer[gpsTokens[TOKEN_GGA_EORW]] == GPS_WEST){
						gpsData.data[recordIndex].longitude *= -1;
					}
					
					gpsData.data[recordIndex].currentMode=atoi( &(	rxBuffer[gpsTokens[TOKEN_GGA_QUALITY		]]) ) & 0xFFFF;
					gpsData.data[recordIndex].satellites= atoi( &(	rxBuffer[gpsTokens[TOKEN_GGA_NUM_SATELLITES	]]) ) & 0xFF;
					gpsData.data[recordIndex].hdop		= atoi( &(	rxBuffer[gpsTokens[TOKEN_GGA_HDOP			]]) ) & 0xFFFF;
					gpsData.data[recordIndex].altitude	= atoi( &(	rxBuffer[gpsTokens[TOKEN_GGA_ALTITUDE		]]) ) & 0xFFFF;
					
					gpsData.data[recordIndex].lapDetected = gps_intersection(oldLongitude,							oldLatitude,
																			 gpsData.data[recordIndex].longitude,   gpsData.data[recordIndex].latitude,
																			 finishLine.startLongitude,				finishLine.startLatitude,
																			 finishLine.endLongitude,				finishLine.endLatitude);
					
					// Save the last coordinates for detecting the intersection
					oldLongitude = gpsData.data[recordIndex].longitude;
					oldLatitude = gpsData.data[recordIndex].latitude;
						
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


unsigned char gps_intersection(signed int x1, signed int y1, signed int x2, signed int y2, signed int x3, signed int y3, signed int x4, signed int y4){
	// (x1, y1) and (x2, y2) are points for line along traveled path
    // (x3, y3) and (y4, x3) are points for the threshold line
	
	// Temporary storage
	float ua, ub, denominator;
	
	// Calculate the denominator
    denominator = (x2 - x1)*(y4 - y3) - (y2 - y1)*(x4 - x3);
    
    ua = ((x4 - x3) * (y1 - y3) - (y4 - y3) * (x1 - x3)) / denominator;
    ub = ((x2 - x1) * (y1 - y3) - (y2 - y1) * (x1 - x3)) / denominator;

    // Calulate the point of intersection
    //long = x1+ua*(x2 - x1);
    //lat = y1+ua*(y2 - y1);
	
	if( (ua >= 0) && (ua <= 1) && (ub >= 0) && (ub <= 1) ){
		return true;
	}
	
	return false;
}

signed int gps_convert_to_decimal_degrees(signed int coordinate){
	unsigned int minutes;
	signed int degrees;
	
	minutes = (coordinate % 1000000);
	degrees = coordinate - minutes;
	
	degrees += ((minutes * 10) / 6);
	
	return degrees;
}

struct tGPSSetLine gps_find_finish_line(struct tGPSSetPoint point){
	// Storage for perpendicular angles to point heading
	float right, left;
	int vect;
	
	struct tGPSSetLine finish;
	
	right = deg2rad(((point.heading + 900) % 3600) / 10);
	left = deg2rad(((point.heading - 900) % 3600) / 10);
	
	finish.heading = point.heading;
	
	// Start Points
	vect = sin(right) * THRESHOLD_DISTANCE * 60 * 1000000;
	finish.startLongitude = vect + point.longitude;
	
	vect = cos(right) * THRESHOLD_DISTANCE * 60 * 1000000;
	finish.startLatitude = vect + point.latitude;
	
	// End Points
	vect = sin(left) * THRESHOLD_DISTANCE * 60 * 1000000;
	finish.endLongitude = vect + point.longitude;
	
	vect = cos(left) * THRESHOLD_DISTANCE * 60 * 1000000;
	finish.endLatitude = vect + point.latitude;
	
	return finish;
}