/******************************************************************************
 *
 * GPS Interface
 *
 * - Compiler:          GNU GCC for AVR32
 * - Supported devices: traq|paq hardware version 1.2
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
#include "math.h"
#include "lcd/itoa.h"
#include "string.h"

xQueueHandle gpsRxdQueue;
xQueueHandle gpsManagerQueue;

unsigned char rxBuffer[GPS_MSG_MAX_STRLEN];
unsigned char gpsTokens[MAX_SIGNALS_SENTENCE];

xTimerHandle xMessageTimer;
xTimerHandle xReceiverSWInfoTimer, xReceiverHWInfoTimer;

struct tGPSInfo gpsInfo;

__attribute__((__interrupt__)) static void ISR_gps_rxd(void){
	int rxd;
	
	usart_read_char(GPS_USART, &rxd);
	
	#if( TRAQPAQ_GPS_ECHO_MODE == TRUE )
	usart_putchar(DEBUG_USART, rxd);
	#endif
	
	xQueueSendFromISR(gpsRxdQueue, &rxd, pdFALSE);
}


void gps_task_init( void ){
	struct tGPSRequest request;
	
	#if( TRAQPAQ_GPS_EXTERNAL_LOGGING == FALSE )
	if(systemFlags.button.powerOnMethod == POWER_ON_MODE_BUTTON){
		gpsRxdQueue		= xQueueCreate( GPS_RXD_QUEUE_SIZE,     sizeof(int)     );
		gpsManagerQueue = xQueueCreate( GPS_MANAGER_QUEUE_SIZE, sizeof(request) );

		INTC_register_interrupt(&ISR_gps_rxd, AVR32_USART3_IRQ, AVR32_INTC_INT0);
	
		xTaskCreate(gps_task, configTSK_GPS_TASK_NAME, configTSK_GPS_TASK_STACK_SIZE, NULL, configTSK_GPS_TASK_PRIORITY, configTSK_GPS_TASK_HANDLE);
	}
	#else
	gpio_set_gpio_pin(GPS_RESET);
	debug_log(DEBUG_PRIORITY_INFO, DEBUG_SENDER_GPS, "External logging mode");
	#endif
}


void gps_task( void *pvParameters ){
	unsigned char i;
	unsigned int rxdChar;										// Temporary storage for received character queue
	unsigned char rxIndex = 0;									// Index in received character buffer
	
	unsigned char processedRMC = FALSE, processedGGA = FALSE;	// Flags for processed NMEA messages
	
	unsigned char processChecksum = FALSE;
	unsigned short calculatedChecksum = 0;
	
	unsigned char recordIndex = 0;								// Index in formatted data struct
	
	unsigned char oldMode = 0;
	
	unsigned int lapTime = 0, oldLapTime = 0;
	unsigned short oldCourse;
	
	unsigned int datestamp = 0;
	
	struct tRecordDataPage gpsData;							// Formatted GPS Data
	struct tGPSLine finishLine;								// Formatted coordinate pairs for "finish line"
	struct tTracklist trackList;
	
	struct tGPSRequest request;

	// Make sure the battery isn't low before continuing
	fuel_low_battery_check();
	
	debug_log(DEBUG_PRIORITY_INFO, DEBUG_SENDER_GPS, "Task Started");
	
	// Initialize information about the GPS receiver
	gpsInfo.serial_number_valid = FALSE;
	gpsInfo.sw_date_valid = FALSE;
	gpsInfo.sw_version_valid = FALSE;
	gpsInfo.part_number_valid = FALSE;
	
	gpsInfo.mode = 0;
	gpsInfo.satellites = 0;
	gpsInfo.record_flag = FALSE;
	gpsInfo.current_location.heading = 0;
	gpsInfo.current_location.latitude = 0;
	gpsInfo.current_location.longitude = 0;
	
	// Pull the GPS out of reset and enable the ISR
	gps_enable_interrupts();
	gps_reset();
	
	#if (TRAQPAQ_GPS_TTF_TEST_MODE == TRUE)
	gps_warm_start();
	#endif
	
	xMessageTimer = xTimerCreate( "gpsMessageTimer", (GPS_MSG_TIMEOUT / portTICK_RATE_MS), pdFALSE, 0, gps_messageTimeout );			// Start the GPS message timeout detection
	xReceiverSWInfoTimer = xTimerCreate( "GetReceiverSWInfoTimer", (GPS_MSG_TX_TIME / portTICK_RATE_MS), 1, NULL, gps_getReceiverSWInfo );	// Start the delay for requesting receiver info
	xReceiverHWInfoTimer = xTimerCreate( "GetReceiverHWInfoTimer", 2*(GPS_MSG_TX_TIME / portTICK_RATE_MS), 1, NULL, gps_getReceiverHWInfo );
	xTimerStart(xReceiverSWInfoTimer, pdFALSE);
	xTimerStart(xReceiverHWInfoTimer, pdFALSE);
	
	while(TRUE){
		// Check for pending requests
		if( xQueueReceive(gpsManagerQueue, &request, pdFALSE) == pdTRUE ){
			switch(request.command){
				case(GPS_MGR_REQUEST_START_RECORDING):
					//flash_send_request(FLASH_REQUEST_SET_DATESTAMP, NULL, NULL, datestamp, FALSE, pdFALSE);
					flash_send_request(FLASH_MGR_SET_DATESTAMP, NULL, NULL, datestamp, FALSE, pdFALSE);
					lapTime = 0;
					oldLapTime = 0xFFFFFFFF;
					gpsInfo.record_flag = TRUE;
					break;
					
				case(GPS_MGR_REQUEST_STOP_RECORDING):
					gpsInfo.record_flag = FALSE;
					recordIndex = 0;
					//flash_send_request(FLASH_REQUEST_END_CURRENT_RECORD, NULL, NULL, NULL, FALSE, 20);
					flash_send_request(FLASH_MGR_END_CURRENT_RECORD, NULL, NULL, NULL, FALSE, 20);
					break;
					
				case(GPS_MGR_REQUEST_SET_FINISH_POINT):
					// Load the track, and then tell the dataflash that we are using it
					//flash_send_request(FLASH_REQUEST_READ_TRACK, &trackList, sizeof(trackList), request.data, TRUE, 20);
					flash_send_request(FLASH_MGR_READ_TRACK, &trackList, sizeof(trackList), request.data, TRUE, 20);
					//flash_send_request(FLASH_REQUEST_SET_TRACK, NULL, NULL, request.data, FALSE, 20);
					flash_send_request(FLASH_MGR_SET_TRACK, NULL, NULL, request.data, FALSE, 20);
					finishLine = gps_find_finish_line(trackList.latitude, trackList.longitude, trackList.heading);
					break;
					
				case(GPS_MGR_REQUEST_CREATE_NEW_TRACK):
					itoa(gpsData.utc, &(trackList.name), 10, FALSE);
					trackList.heading = gpsInfo.current_location.heading;
					trackList.longitude = gpsInfo.current_location.longitude;
					trackList.latitude = gpsInfo.current_location.latitude;
					trackList.isEmpty = FALSE;
					trackList.reserved = 0xA5;
					//flash_send_request(FLASH_REQUEST_ADD_TRACK, &trackList, NULL, NULL, FALSE, pdFALSE);
					flash_send_request(FLASH_MGR_ADD_TRACK, &trackList, NULL, NULL, FALSE, pdFALSE);
					break;
					
				case(GPS_MGR_REQUEST_SHUTDOWN):
					xTimerStop(xMessageTimer, pdFALSE);
					gpio_clr_gpio_pin(GPS_RESET);	// Put the GPS into reset
					debug_log(DEBUG_PRIORITY_INFO, DEBUG_SENDER_GPS, "Task shut down");
					wdt_send_request(WDT_REQUEST_GPS_SHUTDOWN_COMPLETE, NULL);
					vTaskSuspend(NULL);
					break;
				
				case(GPS_MGR_REQUEST_LATITUDE):
					*(request.pointer) = gpsInfo.current_location.latitude;
					break;
					
				case(GPS_MGR_REQUEST_LONGITUDE):
					*(request.pointer) = gpsInfo.current_location.longitude;
					break;
					
				case(GPS_MGR_REQUEST_COURSE):
					*(request.pointer) = gpsInfo.current_location.heading;
					break;
					
				case(GPS_MGR_REQUEST_RECORD_STATUS):
					*(request.pointer) = gpsInfo.record_flag;
					break;
				
			}
			
			// Resume requesting task if it has been suspended
			if(request.resume == TRUE){
				vTaskResume(request.handle);
			}
		}
		
		// Service the received characters
		xQueueReceive(gpsRxdQueue, &rxdChar, portMAX_DELAY);
		
		if( rxdChar == GPS_MSG_END_CHAR ){
			// Reset the time since receiving a message
			xTimerReset(xMessageTimer, pdFALSE);
				
			if( gps_received_checksum() == calculatedChecksum ){
					
				gps_buffer_tokenize();
				
				//--------------------------
				// GGA Message Received
				//--------------------------
				if( (rxBuffer[gpsTokens[TOKEN_MESSAGE_ID] + MESSAGE_OFFSET_ID0] == ID_GGA_ID0) &
					(rxBuffer[gpsTokens[TOKEN_MESSAGE_ID] + MESSAGE_OFFSET_ID1] == ID_GGA_ID1) &
					(rxBuffer[gpsTokens[TOKEN_MESSAGE_ID] + MESSAGE_OFFSET_ID2] == ID_GGA_ID2) ){
					
					// Convert Time!
					gpsData.data[recordIndex].latitude	= atoi( &(	rxBuffer[gpsTokens[TOKEN_GGA_LATITUDE		]]) );
					gpsData.data[recordIndex].latitude	= gps_convert_to_decimal_degrees(gpsData.data[recordIndex].latitude);
					
					if( rxBuffer[gpsTokens[TOKEN_GGA_NORS]] == GPS_SOUTH){
						gpsData.data[recordIndex].latitude *= -1;
					}
					
					gpsData.data[recordIndex].longitude	= atoi( &(	rxBuffer[gpsTokens[TOKEN_GGA_LONGITUDE		]]) );
					gpsData.data[recordIndex].longitude = gps_convert_to_decimal_degrees(gpsData.data[recordIndex].longitude);
					
					if( rxBuffer[gpsTokens[TOKEN_GGA_EORW]] == GPS_WEST){
						gpsData.data[recordIndex].longitude *= -1;
					}
					
					gpsData.currentMode					= atoi( &(	rxBuffer[gpsTokens[TOKEN_GGA_QUALITY		]]) ) & 0xFFFF;
					gpsData.satellites					= atoi( &(	rxBuffer[gpsTokens[TOKEN_GGA_NUM_SATELLITES	]]) ) & 0xFF;
					gpsData.data[recordIndex].altitude	= atoi( &(	rxBuffer[gpsTokens[TOKEN_GGA_ALTITUDE		]]) ) & 0xFFFF;
					
					if(recordIndex == 0){
						gpsData.hdop					= atoi( &(	rxBuffer[gpsTokens[TOKEN_GGA_HDOP			]]) ) & 0xFFFF;
					}						
					
					// Determine if a lap was detected!
					gpsData.data[recordIndex].lapDetected = gps_intersection(gpsInfo.current_location.longitude,	gpsInfo.current_location.latitude,
																			gpsData.data[recordIndex].longitude,    gpsData.data[recordIndex].latitude,
																			finishLine.startLongitude,				finishLine.startLatitude,
																			finishLine.endLongitude,				finishLine.endLatitude);
																			 
					if( gpsData.data[recordIndex].lapDetected && gpsInfo.record_flag){
						if(lapTime <= oldLapTime){
							lcd_sendWidgetRequest(LCD_REQUEST_UPDATE_PERIPHERIAL, LCD_PERIPHERIAL_FASTER, pdFALSE);
						}else{
							lcd_sendWidgetRequest(LCD_REQUEST_UPDATE_PERIPHERIAL, LCD_PERIPHERIAL_SLOWER, pdFALSE);
						}
						
						oldLapTime = lapTime;
						lapTime = 0;
						
						// Update the displayed lap time
						lcd_sendWidgetRequest(LCD_REQUEST_UPDATE_OLDLAPTIME, oldLapTime, pdFALSE);
					}
					
					// Update the antenna widget
					if(oldMode != gpsData.currentMode){
						lcd_sendWidgetRequest(LCD_REQUEST_UPDATE_ANTENNA, gpsData.currentMode, pdFALSE);
						oldMode = gpsData.currentMode;
						
						#if (TRAQPAQ_GPS_TTF_TEST_MODE == TRUE)
						debug_log(DEBUG_PRIORITY_INFO, DEBUG_SENDER_GPS, "GPS Mode changed");
						#endif
					}
					
					// Save the last coordinates for detecting the intersection
					gpsInfo.current_location.longitude = gpsData.data[recordIndex].longitude;
					gpsInfo.current_location.latitude = gpsData.data[recordIndex].latitude;
					gpsInfo.current_location.heading = gpsData.data[recordIndex].heading;
						
					processedGGA = TRUE;
					
				}
				//--------------------------
				// RMC Message Received
				//--------------------------
				if( (rxBuffer[gpsTokens[TOKEN_MESSAGE_ID] + MESSAGE_OFFSET_ID0] == ID_RMC_ID0) &
					(rxBuffer[gpsTokens[TOKEN_MESSAGE_ID] + MESSAGE_OFFSET_ID1] == ID_RMC_ID1) &
					(rxBuffer[gpsTokens[TOKEN_MESSAGE_ID] + MESSAGE_OFFSET_ID2] == ID_RMC_ID2) ){
						
					// More Converting!!
					gpsData.data[recordIndex].speed		= atoi( &(	rxBuffer[gpsTokens[TOKEN_RMC_SPEED	]]) ) & 0xFFFF;
					gpsData.data[recordIndex].heading	= atoi( &(	rxBuffer[gpsTokens[TOKEN_RMC_TRACK	]]) ) & 0xFFFF;
					datestamp							= atoi( &(	rxBuffer[gpsTokens[TOKEN_RMC_DATE	]]) );
					
					if(recordIndex == 0){
						gpsData.utc						= atoi( &(	rxBuffer[gpsTokens[TOKEN_RMC_UTC	]]) );
					}
					
					processedRMC = TRUE;

				
				}else
				//--------------------------
				// PMTK001 Message Received
				//--------------------------
				if( (rxBuffer[gpsTokens[TOKEN_MESSAGE_ID] + MESSAGE_OFFSET_ID0] == ID_MTK001_ID0) &
					(rxBuffer[gpsTokens[TOKEN_MESSAGE_ID] + MESSAGE_OFFSET_ID1] == ID_MTK001_ID1) &
					(rxBuffer[gpsTokens[TOKEN_MESSAGE_ID] + MESSAGE_OFFSET_ID2] == ID_MTK001_ID2) &
					(rxBuffer[gpsTokens[TOKEN_MESSAGE_ID] + MESSAGE_OFFSET_ID3] == ID_MTK001_ID3) &
					(rxBuffer[gpsTokens[TOKEN_MESSAGE_ID] + MESSAGE_OFFSET_ID4] == ID_MTK001_ID4) ){
						
					switch( rxBuffer[gpsTokens[TOKEN_PMTK001_FLAG]] ){
						case(PMTK001_INVALID_CMD):
							debug_log(DEBUG_PRIORITY_WARNING, DEBUG_SENDER_GPS, "Invalid Command");
							break;
							
						case(PMTK001_UNSUPPORTED_CMD):
							debug_log(DEBUG_PRIORITY_WARNING, DEBUG_SENDER_GPS, "Unsupported Command");
							break;
							
						case(PMTK001_VALID_CMD_FAILED):
							debug_log(DEBUG_PRIORITY_WARNING, DEBUG_SENDER_GPS, "Command Failed");
							break;
							
						case(PMTK001_VALID_CMD):
							debug_log(DEBUG_PRIORITY_INFO, DEBUG_SENDER_GPS, "Command Succeeded");
							break;
					}
					
				}else
				//--------------------------
				// PMTK599 Message Received (Receiver Part Number and Serial Number)
				//--------------------------
				if( (rxBuffer[gpsTokens[TOKEN_MESSAGE_ID] + MESSAGE_OFFSET_ID0] == ID_MTK599_ID0) &
					(rxBuffer[gpsTokens[TOKEN_MESSAGE_ID] + MESSAGE_OFFSET_ID1] == ID_MTK599_ID1) &
					(rxBuffer[gpsTokens[TOKEN_MESSAGE_ID] + MESSAGE_OFFSET_ID2] == ID_MTK599_ID2) &
					(rxBuffer[gpsTokens[TOKEN_MESSAGE_ID] + MESSAGE_OFFSET_ID3] == ID_MTK599_ID3) &
					(rxBuffer[gpsTokens[TOKEN_MESSAGE_ID] + MESSAGE_OFFSET_ID4] == ID_MTK599_ID4) ){
					
					// Load up the serial number
					gpsInfo.serial_number = (gps_convertASCIIHex(rxBuffer[gpsTokens[TOKEN_PMTK599_SERIAL_B0]], rxBuffer[gpsTokens[TOKEN_PMTK599_SERIAL_B0]+1]) << 24) |
											(gps_convertASCIIHex(rxBuffer[gpsTokens[TOKEN_PMTK599_SERIAL_B1]], rxBuffer[gpsTokens[TOKEN_PMTK599_SERIAL_B1]+1]) << 16) |
											(gps_convertASCIIHex(rxBuffer[gpsTokens[TOKEN_PMTK599_SERIAL_B2]], rxBuffer[gpsTokens[TOKEN_PMTK599_SERIAL_B2]+1]) <<  8) |	
											(gps_convertASCIIHex(rxBuffer[gpsTokens[TOKEN_PMTK599_SERIAL_B3]], rxBuffer[gpsTokens[TOKEN_PMTK599_SERIAL_B3]+1]) <<  0);

					// Load up the part number... Goofy, it is transmitted as Hex Byte in ASCII
					gpsInfo.part_number[0] = gps_convertASCIIHex(rxBuffer[gpsTokens[TOKEN_PMTK599_PARTNO_B0]], rxBuffer[gpsTokens[TOKEN_PMTK599_PARTNO_B0]+1]);
					gpsInfo.part_number[1] = gps_convertASCIIHex(rxBuffer[gpsTokens[TOKEN_PMTK599_PARTNO_B1]], rxBuffer[gpsTokens[TOKEN_PMTK599_PARTNO_B1]+1]);
					gpsInfo.part_number[2] = gps_convertASCIIHex(rxBuffer[gpsTokens[TOKEN_PMTK599_PARTNO_B2]], rxBuffer[gpsTokens[TOKEN_PMTK599_PARTNO_B2]+1]);
					gpsInfo.part_number[3] = gps_convertASCIIHex(rxBuffer[gpsTokens[TOKEN_PMTK599_PARTNO_B3]], rxBuffer[gpsTokens[TOKEN_PMTK599_PARTNO_B3]+1]);
					gpsInfo.part_number[4] = gps_convertASCIIHex(rxBuffer[gpsTokens[TOKEN_PMTK599_PARTNO_B4]], rxBuffer[gpsTokens[TOKEN_PMTK599_PARTNO_B4]+1]);
					gpsInfo.part_number[5] = gps_convertASCIIHex(rxBuffer[gpsTokens[TOKEN_PMTK599_PARTNO_B5]], rxBuffer[gpsTokens[TOKEN_PMTK599_PARTNO_B5]+1]);
					gpsInfo.part_number[6] = gps_convertASCIIHex(rxBuffer[gpsTokens[TOKEN_PMTK599_PARTNO_B6]], rxBuffer[gpsTokens[TOKEN_PMTK599_PARTNO_B6]+1]);
					gpsInfo.part_number[7] = NULL;	// Terminate the string
					
					gpsInfo.serial_number_valid = TRUE;
					gpsInfo.part_number_valid = TRUE;
				
				}else
				//--------------------------
				// PMTK705 Message Received (Receiver Software Version and Date)
				//--------------------------
				if( (rxBuffer[gpsTokens[TOKEN_MESSAGE_ID] + MESSAGE_OFFSET_ID0] == ID_MTK705_ID0) &
					(rxBuffer[gpsTokens[TOKEN_MESSAGE_ID] + MESSAGE_OFFSET_ID1] == ID_MTK705_ID1) &
					(rxBuffer[gpsTokens[TOKEN_MESSAGE_ID] + MESSAGE_OFFSET_ID2] == ID_MTK705_ID2) &
					(rxBuffer[gpsTokens[TOKEN_MESSAGE_ID] + MESSAGE_OFFSET_ID3] == ID_MTK705_ID3) &
					(rxBuffer[gpsTokens[TOKEN_MESSAGE_ID] + MESSAGE_OFFSET_ID4] == ID_MTK705_ID4) ){
						
					strlcpy( &gpsInfo.sw_version, &rxBuffer[gpsTokens[TOKEN_PMTK705_SW_VERSION]], GPS_INFO_SW_VERSION_SIZE );
					strlcpy( &gpsInfo.sw_date, &rxBuffer[gpsTokens[TOKEN_PMTK705_SW_DATE]], GPS_INFO_SW_DATE_SIZE );
					
					gpsInfo.sw_version_valid = TRUE;
					gpsInfo.sw_date_valid = TRUE;
					asm("nop");
				}
				
				
				
				if(processedGGA && processedRMC){
					processedGGA = FALSE;
					processedRMC = FALSE;
						
					if(gpsInfo.record_flag){
						// Update lap time counter;
						lcd_sendWidgetRequest(LCD_REQUEST_UPDATE_LAPTIME, lapTime++, pdFALSE);
						
						recordIndex++;
						if(recordIndex == RECORD_DATA_PER_PAGE){
							//flash_send_request(FLASH_REQUEST_ADD_RECORDDATA, &gpsData, sizeof(gpsData), NULL, TRUE, 20);
							flash_send_request(FLASH_MGR_ADD_RECORD_DATA, &gpsData, sizeof(gpsData), NULL, TRUE, 20);
							recordIndex = 0;
						}
						
					}
				}  // ProcessedGGA and ProcessedRMC
				
			}else{
				// Invalid CRC received!
				debug_log(DEBUG_PRIORITY_WARNING, DEBUG_SENDER_GPS, "Invalid Checksum Received");
			}				
			
			// Reset the buffer and checksum
			rxIndex = 0;
			calculatedChecksum = 0;

		}else{
			
			//--------------------------
			// Keep a running CRC calculation
			//--------------------------
			if( rxdChar == GPS_CHECKSUM_CHAR ){
				processChecksum = FALSE;
			}
			
			if( processChecksum ){
				calculatedChecksum ^= rxdChar;
			}
			
			if( rxdChar == GPS_MSG_START_CHAR ){
				processChecksum = TRUE;
			}
			
			//--------------------------
			// Store the received character (skip periods!)
			//--------------------------
			if( (rxIndex < GPS_MSG_MAX_STRLEN) && (rxdChar != GPS_PERIOD) ){
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


unsigned short gps_received_checksum( void ){
	unsigned char index = 0;
	
	// Find the start of the NMEA checksum
	while( (index < GPS_MSG_MAX_STRLEN) && (rxBuffer[index] != GPS_CHECKSUM_CHAR) ){
		index++;
	}
	
	// Skip the GPS_CHECKSUM_CHAR
	index++;
	
	return gps_convertASCIIHex(rxBuffer[index], rxBuffer[index + 1]);	
}

unsigned char gps_convertASCIIHex(unsigned char byte1, unsigned char byte2){
	unsigned char converted;
	
	if(byte1 >= 'A'){
		byte1 += 10 - 'A';
	}else{
		byte1 -= '0';
	}
	
	if(byte2 >= 'A'){
		byte2 += 10 - 'A';
	}else{
		byte2 -= '0';
	}
	
	return ((byte1 & 0xF) << 4) + (byte2 & 0xF);
}


unsigned char gps_intersection(signed int x1, signed int y1, signed int x2, signed int y2, signed int x3, signed int y3, signed int x4, signed int y4){
	// (x1, y1) and (x2, y2) are points for line along traveled path
    // (x3, y3) and (y4, x3) are points for the threshold line
	
	// Temporary storage
	float ua, ub, denominator;
	
	// Calculate the denominator
    denominator = (x2 - x1)*(y4 - y3) - (y2 - y1)*(x4 - x3);
    
    ua = (float)((x4 - x3) * (y1 - y3) - (y4 - y3) * (x1 - x3)) / denominator;
    ub = (float)((x2 - x1) * (y1 - y3) - (y2 - y1) * (x1 - x3)) / denominator;

    // Calulate the point of intersection
    //long = x1+ua*(x2 - x1);
    //lat = y1+ua*(y2 - y1);
	
	if( (ua >= 0) && (ua <= 1) && (ub >= 0) && (ub <= 1) ){
		// Paths intersected!
		
		// Check course

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

struct tGPSLine gps_find_finish_line(signed int latitude, signed int longitude, unsigned short heading){
	float angle;
	int	vect;
	
	struct tGPSLine finish;
	
	// Add 90 degrees to heading, make sure it is between 0 and 360,
	// and finally shift the decimal back in
	angle = deg2rad(((heading + 900) % 3600) / 10);
	
	// Copy the heading on the point to the heading for the line
	finish.heading = heading;
	
	// Project the THRESHOLD_DISTANCE along the perpendicular angle
	vect = sin(angle) * THRESHOLD_DISTANCE * 60 * 1000000;
	finish.startLongitude	= longitude + vect;
	finish.endLongitude		= longitude - vect;
	
	vect = cos(angle) * THRESHOLD_DISTANCE * 60 * 1000000;
	finish.startLatitude	= latitude + vect;
	finish.endLatitude		= latitude - vect;
	
	return finish;
}

void gps_set_messaging_rate(unsigned char rate){
	
	switch(rate){
		case(GPS_MESSAGING_100MS):
			usart_write_line(GPS_USART, "$PMTK300,100,0,0,0,0*2C");
			break;
			
		case(GPS_MESSAGING_200MS):
			usart_write_line(GPS_USART, "$PMTK300,200,0,0,0,0*2F");
			break;
			
		case(GPS_MESSAGING_500MS):
			usart_write_line(GPS_USART, "$PMTK300,500,0,0,0,0*28");
			break;
			
		case(GPS_MESSAGING_1000MS):
			usart_write_line(GPS_USART, "$PMTK300,1000,0,0,0,0*1C");
			break;
	}
	
	usart_putchar(GPS_USART, GPS_MSG_CR);
	usart_putchar(GPS_USART, GPS_MSG_END_CHAR);
	
}

void gps_set_messages( void ){
	// Enable GGA and RMC messages only
	usart_write_line(GPS_USART, "$PMTK314,0,1,0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0*28");
	usart_putchar(GPS_USART, GPS_MSG_CR);
	usart_putchar(GPS_USART, GPS_MSG_END_CHAR);
}

void gps_cold_start( void ){
	// $PMTK103*30<CR><LF>
	usart_write_line(GPS_USART, "$PMTK103*30");
	usart_putchar(GPS_USART, GPS_MSG_CR);
	usart_putchar(GPS_USART, GPS_MSG_END_CHAR);
}

void gps_warm_start( void ){
	// $PMTK102*31<CR><LF>
	usart_write_line(GPS_USART, "$PMTK102*31");
	usart_putchar(GPS_USART, GPS_MSG_CR);
	usart_putchar(GPS_USART, GPS_MSG_END_CHAR);
}

void gps_send_request(enum tGpsCommand command, unsigned int *pointer, unsigned char data, unsigned char delay, unsigned char resume){
	struct tGPSRequest request;
	request.command = command;
	request.pointer = pointer;
	request.data = data;
	request.resume = resume;
	
	if(resume == TRUE){
		request.handle = xTaskGetCurrentTaskHandle();
	}
	
	xQueueSend(gpsManagerQueue, &request, delay);
	
	if(resume == TRUE){
		vTaskSuspend(NULL);
	}
}

void gps_messageTimeout( xTimerHandle xTimer ){
	debug_log(DEBUG_PRIORITY_CRITICAL, DEBUG_SENDER_GPS, "Message Timer Expired");
	debug_log(DEBUG_PRIORITY_WARNING, DEBUG_SENDER_GPS, "Setting Message Rate");
	gps_set_messaging_rate(GPS_MESSAGING_100MS);
}

void gps_getReceiverSWInfo( xTimerHandle xTimer ){
	// Software version and date	
	usart_write_line(GPS_USART, "$PMTK605*31");
	usart_putchar(GPS_USART, GPS_MSG_CR);
	usart_putchar(GPS_USART, GPS_MSG_END_CHAR);
}


void gps_getReceiverHWInfo( xTimerHandle xTimer ){
	// Reciever part number and serial number
	usart_write_line(GPS_USART, "$PMTK499,1C0,21*77");
	usart_putchar(GPS_USART, GPS_MSG_CR);
	usart_putchar(GPS_USART, GPS_MSG_END_CHAR);
}