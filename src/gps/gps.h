/******************************************************************************
 *
 * GPS defines
 *
 * - Compiler:          GNU GCC for AVR32
 * - Supported devices: traq|paq hardware version 1.4
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

#define GPS_VERSION					"1.10"

#define gps_enable_interrupts()		GPS_USART->ier |= AVR32_USART_IER_RXRDY_MASK
#define gps_disable_interrupts()	GPS_USART->idr |= AVR32_USART_IER_RXRDY_MASK

#define GPS_RESET_TIME				100						// Time in milliseconds
#define GPS_RXD_QUEUE_SIZE			(GPS_MSG_MAX_STRLEN * 2)	// Number of items to buffer in Receive Queue
#define GPS_MANAGER_QUEUE_SIZE		5						// Number of items to buffer in Request 

#define GPS_WAIT_RXD_TIME			20						// Time (milliseconds) to wait for a received character

#define GPS_MSG_START_CHAR			'$'		
#define GPS_MSG_END_CHAR			0x0A					// ASCII for LF
#define GPS_MSG_CR					0x0D
#define GPS_MSG_MAX_STRLEN			120						// Each message is limited to 82 characters max, including '$', CR, and LF.... Some debug messages are longer than NMEA spec
#define GPS_DELIMITER_CHAR			','
#define GPS_CHECKSUM_CHAR			'*'
#define GPS_PERIOD					'.'

#define GPS_NULL					0

#define GPS_NORTH					'N'
#define GPS_SOUTH					'S'

#define GPS_EAST					'E'
#define GPS_WEST					'W'


#define THRESHOLD_DISTANCE_FEET		15			// Threshold (+/-) in feet for finish line gate
#define EARTH_RADIUS_FEET			20891000	// Approximate radius of earth in feet;
#define THRESHOLD_DISTANCE			((float)THRESHOLD_DISTANCE_FEET / (float)EARTH_RADIUS_FEET) // Do not modify, instead modify 'THRESHOLD_DISTANCE_FEET'
#define THRESHOLD_ANGLE				2250			// Threshold (+/-) in degrees for finish line gate, two assumed decimal places

#define RADIANS_CONVERSION			0.0174532925	// Value of (Pi / 180)

// Indices for pointing areas of each message
#define MAX_SIGNALS_SENTENCE		40				// Support for debug messages (specifically PMTK599 message)

#define TOKEN_MESSAGE_ID			0

#define MESSAGE_OFFSET_ID0			2
#define MESSAGE_OFFSET_ID1			3
#define MESSAGE_OFFSET_ID2			4
#define MESSAGE_OFFSET_ID3			5
#define MESSAGE_OFFSET_ID4			6

// GGA message from GPS receiver
#define ID_GGA_ID0					'G'
#define ID_GGA_ID1					'G'
#define ID_GGA_ID2					'A'

// RMC message from GPS receiver
#define ID_RMC_ID0					'R'
#define ID_RMC_ID1					'M'
#define ID_RMC_ID2					'C'

// Standard response from GPS receiver
#define ID_MTK001_ID0				'T'
#define ID_MTK001_ID1				'K'
#define ID_MTK001_ID2				'0'
#define ID_MTK001_ID3				'0'
#define ID_MTK001_ID4				'1'

// Startup message from GPS receiver
#define ID_MTK010_ID0				'T'
#define ID_MTK010_ID1				'K'
#define ID_MTK010_ID2				'0'
#define ID_MTK010_ID3				'1'
#define ID_MTK010_ID4				'0'

// Startup message from GPS receiver
#define ID_MTK011_ID0				'T'
#define ID_MTK011_ID1				'K'
#define ID_MTK011_ID2				'0'
#define ID_MTK011_ID3				'1'
#define ID_MTK011_ID4				'1'

// Debug 499 response from GPS receiver
#define ID_MTK599_ID0				'T'
#define ID_MTK599_ID1				'K'
#define ID_MTK599_ID2				'5'
#define ID_MTK599_ID3				'9'
#define ID_MTK599_ID4				'9'

// Debug 705 response from GPS receiver
#define ID_MTK705_ID0				'T'
#define ID_MTK705_ID1				'K'
#define ID_MTK705_ID2				'7'
#define ID_MTK705_ID3				'0'
#define ID_MTK705_ID4				'5'

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

#define TOKEN_PMTK010_SYSMSG		1
#define SYSMSG_OFFSET				2
#define PMTK010_SYSMSG_UNKNOWN		0x30
#define PMTK010_SYSMSG_STARTUP		0x31

#define TOKEN_PMTK011_SYSID			1

#define TOKEN_PMTK599_SERIAL_B0		3
#define TOKEN_PMTK599_SERIAL_B1		4
#define TOKEN_PMTK599_SERIAL_B2		5
#define TOKEN_PMTK599_SERIAL_B3		6

#define TOKEN_PMTK599_PARTNO_B0		12
#define TOKEN_PMTK599_PARTNO_B1		13
#define TOKEN_PMTK599_PARTNO_B2		14
#define TOKEN_PMTK599_PARTNO_B3		15
#define TOKEN_PMTK599_PARTNO_B4		16
#define TOKEN_PMTK599_PARTNO_B5		17
#define TOKEN_PMTK599_PARTNO_B6		18

#define TOKEN_PMTK705_SW_VERSION	1
#define TOKEN_PMTK705_SW_DATE		3

#define GPS_INFO_SW_VERSION_SIZE	9	// Number of characters of SW version string, AXN_x.xx, plus null character
#define GPS_INFO_SW_DATE_SIZE		9	// Number of characters of SW date string, YYYYMMDD, plus null character
#define GPS_INFO_PART_NUMBER_SIZE	8	// Number of characters of Part Number String, plus null character

#define GPS_MSG_TX_TIME				250
#define GPS_DEAD_STARTUP_TIME		500

#define GPS_BAUD_RATE_CHANGE_DELAY	100

#define GPS_UNKNOWN_MESSAGES_TOLERANCE	10
#define GPS_RESET_MAX_TRIES				3


enum tGpsCommand {
	GPS_MGR_REQUEST_DATE,
	GPS_MGR_REQUEST_START_RECORDING,
	GPS_MGR_REQUEST_STOP_RECORDING,
	GPS_MGR_REQUEST_SET_FINISH_POINT,
	GPS_MGR_REQUEST_CREATE_NEW_TRACK,
	GPS_MGR_REQUEST_SHUTDOWN,
	GPS_MGR_REQUEST_LATITUDE,
	GPS_MGR_REQUEST_LONGITUDE,
	GPS_MGR_REQUEST_COURSE,
	GPS_MGR_REQUEST_RECORD_STATUS,
	GPS_MGR_REQUEST_RECEIVER_INFO
};

struct tGPSRequest {
	enum tGpsCommand command;
	unsigned char data;
	unsigned int *pointer;
	unsigned char resume;		// Flag to resume the calling task
	xTaskHandle handle;			// Handle of task to resume after completion
};


struct tGPSPoint {
	signed int latitude;
	signed int longitude;
	unsigned short heading;
};

struct tGPSLine {
	unsigned short heading;
	
	signed int startLatitude;
	signed int startLongitude;
	
	signed int endLatitude;
	signed int endLongitude;
};

enum tGPSStatus {
	GPS_STATUS_UNKNOWN		= 0,
	GPS_STATUS_STARTED		= 1,
	GPS_STATUS_SICK			= 2,
	GPS_STATUS_DEAD			= 3
};

enum tGPSCmdResponse {
	GPS_NO_RESPONSE			= 0,
	GPS_INVALID_COMMAND		= 1,
	GPS_UNSUPPORTED_COMMAND	= 2,
	GPS_VALID_CMD_FAILED	= 3,
	GPS_VALID_CMD			= 4
};

struct tGPSError {
	unsigned char checksumErrors;
	unsigned char rmcMsgTimeouts;
	unsigned char ggaMsgTimeouts;
	unsigned char unrecognizedMsgs;
	unsigned char rxDataError;
	unsigned char resetCount;
};

struct tGPSLastCmd {
	unsigned int msgID;
	enum tGPSCmdResponse response;
};

struct tGPSInfo {
	unsigned int	serial_number;
	unsigned char	serial_number_valid;
	
	unsigned char	sw_version[GPS_INFO_SW_VERSION_SIZE];
	unsigned char	sw_version_valid;
	
	unsigned char	sw_date[GPS_INFO_SW_DATE_SIZE];
	unsigned char	sw_date_valid;
	
	unsigned char	part_number[GPS_INFO_PART_NUMBER_SIZE];
	unsigned char	part_number_valid;
	
	unsigned char	mode;
	unsigned char	satellites;
	unsigned char	record_flag;
	
	struct	tGPSPoint current_location;		// Last received position data
	enum	tGPSStatus status;				// GPS Receiver status
	struct	tGPSError error;				// Error counts
	struct	tGPSLastCmd lastCmd;			// Response from last sent command
};


#define deg2rad(x)			((x) * RADIANS_CONVERSION)
#define rad2deg(x)			((x) / RADIANS_CONVERSION)

#define incrementErrorCount(counter)		if(counter < 255) counter++

// Timeout in milliseconds for message timeout
#if( TRAQPAQ_UBLOX_GPS == TRUE )
	#define GPS_GGA_MSG_TIMEOUT				400
	#define GPS_RMC_MSG_TIMEOUT				400
#else
	#define GPS_GGA_MSG_TIMEOUT				200
	#define GPS_RMC_MSG_TIMEOUT				200
#endif

// Timer ID's
#define GPS_GGA_MSG_TIMER_ID		0
#define GPS_RMC_MSG_TIMER_ID		1
#define GPS_SWINFO_TIMER_ID			2
#define GPS_HWINFO_TIMER_ID			3
#define GPS_DEAD_TIMER_ID			4

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

unsigned char gps_intersection(signed int x1, signed int y1, signed int x2, signed int y2, signed int x3, signed int y3, signed int x4, signed int y4, unsigned short travelHeading, unsigned short finishHeading);
signed int gps_convert_to_decimal_degrees(signed int coordinate);
struct tGPSLine gps_find_finish_line(signed int latitude, signed int longitude, unsigned short heading);

void gps_set_messaging_rate(unsigned char rate);
void gps_set_messages( void );
void gps_cold_start( void );
void gps_warm_start( void );

void gps_send_request(enum tGpsCommand command, unsigned int *pointer, unsigned char data, unsigned char delay, unsigned char resume);
void gps_messageTimeout( xTimerHandle xTimer );
void gps_getReceiverInfo( xTimerHandle xTimer );
unsigned char gps_convertASCIIHex(unsigned char byte1, unsigned char byte2);
void gps_dead( xTimerHandle xTimer );
void gps_setSbasMode(unsigned char enableSBAS);

#endif /* GPS_H_ */
