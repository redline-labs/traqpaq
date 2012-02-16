/******************************************************************************
 *
 * GPS defines
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

#ifndef GPS_H_
#define GPS_H_

#define GPS_VERSION					"1.00"

#define GPS_RESET_TIME				100		// Time in milliseconds
#define GPS_RXD_QUEUE_SIZE			20		// Number of items to buffer in Receive Queue
#define GPS_MANAGER_QUEUE_SIZE		5		// Number of items to buffer in Request Queue

#define GPS_WAIT_RXD_TIME			20		// Time (milliseconds) to wait for a received character

#define GPS_MSG_START_CHAR			'$'		
#define GPS_MSG_END_CHAR			0x0A	// ASCII for LF
#define GPS_MSG_CR					0x0D
#define GPS_MSG_MAX_STRLEN			82		// Each message is limited to 82 characters max, including '$', CR, and LF
#define GPS_DELIMITER_CHAR			','
#define GPS_CHECKSUM_CHAR			'*'
#define GPS_PERIOD					'.'

#define GPS_NULL					0



// Indices for pointing areas of each message
#define MAX_SIGNALS_SENTENCE		20

#define TOKEN_MESSAGE_ID			0

#define MESSAGE_OFFSET_ID0			2
#define MESSAGE_OFFSET_ID1			3
#define MESSAGE_OFFSET_ID2			4

#define ID_GGA_ID0					'G'
#define ID_GGA_ID1					'G'
#define ID_GGA_ID2					'A'

#define ID_RMC_ID0					'R'
#define ID_RMC_ID1					'M'
#define ID_RMC_ID2					'C'

#define TOKEN_GGA_UTC				1
#define TOKEN_GGA_LATITUDE			2
#define TOKEN_GGA_NORS				3
#define TOKEN_GGA_LONGITUDE			4
#define TOKEN_GGA_EORW				5
#define TOKEN_GGA_QUALITY			6
#define TOKEN_GGA_NUM_SATELLITES	7
#define TOKEN_GGA_HDOP				8
#define TOKEN_GGA_ALTITUDE			9
#define TOKEN_GGA_ALTITUDE_UNITS	10
#define TOKEN_GGA_GEO_SEP			11
#define TOKEN_GGA_GEO_SEP_UNITS		12
#define TOKEN_GGA_DIGREF_DATA_AGE	13
#define TOKEN_GGA_DIGREF_STATION_ID	14

#define TOKEN_RMC_UTC				1
#define TOKEN_RMC_STATUS			2
#define TOKEN_RMC_LATITUDE			3
#define TOKEN_RMC_NORS				4
#define TOKEN_RMC_LONGITUDE			5
#define TOKEN_RMC_EORW				6
#define TOKEN_RMC_SPEED				7
#define TOKEN_RMC_TRACK				8
#define TOKEN_RMC_DATE				9
#define TOKEN_RMC_MAG_VAR			10
#define TOKEN_RMC_MAG_VAR_EORW		11
#define TOKEN_RMC_UNKNOWN			12


typedef struct tGPSRequest {
	unsigned char command;
	unsigned int *pointer;
};

#define GPS_REQUEST_DATE			0
#define GPS_REQUEST_START_RECORDING	1
#define GPS_REQUEST_STOP_RECORDING	2


// Prototypes
//void gps_processMsg(struct tGPSRXDBuffer *GPSRXDBuffer, unsigned char index, struct tGPSMsgGGA *GPSMsgGGA, struct tGPSMsgGSV *GPSMsgGSV);

void gps_task_init( void );
void gps_task( void *pvParameters );
void gps_reset( void );

void gps_buffer_tokenize( void );
unsigned char gps_verify_checksum( void );

#endif /* GPS_H_ */
