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

#define gps_enable_interrupts()		GPS_USART->ier |= AVR32_USART_IER_RXRDY_MASK
#define gps_disable_interrupts()	GPS_USART->idr |= AVR32_USART_IER_RXRDY_MASK

#define GPS_RESET_TIME				100						// Time in milliseconds
#define GPS_RXD_QUEUE_SIZE			GPS_MSG_MAX_STRLEN	// Number of items to buffer in Receive Queue
#define GPS_MANAGER_QUEUE_SIZE		5						// Number of items to buffer in Request Queue

#define GPS_WAIT_RXD_TIME			20						// Time (milliseconds) to wait for a received character

#define GPS_MSG_START_CHAR			'$'		
#define GPS_MSG_END_CHAR			0x0A					// ASCII for LF
#define GPS_MSG_CR					0x0D
#define GPS_MSG_MAX_STRLEN			82						// Each message is limited to 82 characters max, including '$', CR, and LF
#define GPS_DELIMITER_CHAR			','
#define GPS_CHECKSUM_CHAR			'*'
#define GPS_PERIOD					'.'

#define GPS_NULL					0

#define GPS_NORTH					'N'
#define GPS_SOUTH					'S'

#define GPS_EAST					'E'
#define GPS_WEST					'W'


#define THRESHOLD_DISTANCE_FEET		25			// Threshold (+/-) in feet for finish line gate
#define EARTH_RADIUS_FEET			20891000	// Approximate radius of earth in feet;
#define THRESHOLD_DISTANCE			THRESHOLD_DISTANCE_FEET / EARTH_RADIUS_FEET // Do not modify, instead modify 'THRESHOLD_DISTANCE_FEET'

#define RADIANS_CONVERSION			0.0174532925	// Value of (Pi / 180)

// Indices for pointing areas of each message
#define MAX_SIGNALS_SENTENCE		20

#define TOKEN_MESSAGE_ID			0

#define MESSAGE_OFFSET_ID0			2
#define MESSAGE_OFFSET_ID1			3
#define MESSAGE_OFFSET_ID2			4
#define MESSAGE_OFFSET_ID3			5
#define MESSAGE_OFFSET_ID4			6

#define ID_GGA_ID0					'G'
#define ID_GGA_ID1					'G'
#define ID_GGA_ID2					'A'

#define ID_RMC_ID0					'R'
#define ID_RMC_ID1					'M'
#define ID_RMC_ID2					'C'

#define ID_MTK001_ID0				'T'
#define ID_MTK001_ID1				'K'
#define ID_MTK001_ID2				'0'
#define ID_MTK001_ID3				'0'
#define ID_MTK001_ID4				'1'

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

#define TOKEN_PMTK001_CMD			1
#define TOKEN_PMTK001_FLAG			2

#define PMTK001_INVALID_CMD			0x30
#define PMTK001_UNSUPPORTED_CMD		0x31
#define PMTK001_VALID_CMD_FAILED	0x32
#define PMTK001_VALID_CMD			0x33


typedef struct tGPSRequest {
	unsigned char command;
	unsigned int *pointer;
};

#define GPS_REQUEST_DATE			0
#define GPS_REQUEST_START_RECORDING	1
#define GPS_REQUEST_STOP_RECORDING	2


typedef struct tGPSSetPoint {
	signed int latitude;
	signed int longitude;
	unsigned short heading;
};

typedef struct tGPSSetLine {
	unsigned short heading;
	
	signed int startLatitude;
	signed int startLongitude;
	
	signed int endLatitude;
	signed int endLongitude;
};


#define deg2rad(x)			((x) * RADIANS_CONVERSION)
#define rad2deg(x)			((x) / RADIANS_CONVERSION)

#define gps_resetTimer()			LastUpdateTime = xTaskGetTickCount()
#define gps_did_time_expire(ms)		((xTaskGetTickCount() - LastUpdateTime) >= ((ms) /  portTICK_RATE_MS))			// Check to see if timer expired
#define GPS_RMC_TIMEOUT				900		// Timeout in milliseconds since receiving a RMC message

#define GPS_MODE_NO_FIX				0
#define GPS_MODE_2D_FIX				1
#define GPS_MODE_3D_FIX				2

#define GPS_MESSAGING_100MS			0
#define GPS_MESSAGING_200MS			1
#define GPS_MESSAGING_500MS			2
#define GPS_MESSAGING_1000MS		3

// Prototypes
void gps_task_init( void );
void gps_task( void *pvParameters );
void gps_reset( void );

void gps_buffer_tokenize( void );
unsigned short gps_received_checksum( void );

unsigned char gps_intersection(signed int x1, signed int y1, signed int x2, signed int y2, signed int x3, signed int y3, signed int x4, signed int y4);
signed int gps_convert_to_decimal_degrees(signed int coordinate);
struct tGPSSetLine gps_find_finish_line(struct tGPSSetPoint point);

void gps_set_messaging_rate(unsigned char rate);
void gps_set_messages( void );

#endif /* GPS_H_ */
