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

#define gps_enable_rxrdy_isr()		GPS_USART->ier |= AVR32_USART_IER_RXRDY_MASK
#define gps_disable_rxrdy_isr()		GPS_USART->idr |= AVR32_USART_IER_RXRDY_MASK

#define gps_flip_endian4(little)	( ((little & 0xFF000000) >> 24) + ((little & 0x00FF0000) >> 8) + ((little & 0x0000FF00) << 8) + ((little & 0x000000FF) << 24) )
#define gps_flip_endian2(little)	( ((little & 0xFF00) >> 8) + ((little & 0x00FF) << 8) )

#define GPS_RESET_TIME				100						// Time in milliseconds
#define GPS_RXD_QUEUE_SIZE			100						// Number of items to buffer in Receive Queue
#define GPS_MANAGER_QUEUE_SIZE		5						// Number of items to buffer in Request 

#define GPS_WAIT_RXD_TIME			20						// Time (milliseconds) to wait for a received character
#define GPS_MSG_MAX_LENGTH			108

#define GPS_CHAR_SYNC1				0xB5
#define GPS_CHAR_SYNC2				0x62


#define THRESHOLD_DISTANCE_FEET		15			// Threshold (+/-) in feet for finish line gate
#define EARTH_RADIUS_FEET			20891000	// Approximate radius of earth in feet;
#define THRESHOLD_DISTANCE			((float)THRESHOLD_DISTANCE_FEET / (float)EARTH_RADIUS_FEET) // Do not modify, instead modify 'THRESHOLD_DISTANCE_FEET'
#define THRESHOLD_ANGLE				2250			// Threshold (+/-) in degrees for finish line gate, two assumed decimal places

#define RADIANS_CONVERSION			0.0174532925	// Value of (Pi / 180)

#define GPS_MSG_TX_TIME				250
#define GPS_DEAD_STARTUP_TIME		500

#define GPS_BAUD_RATE_CHANGE_DELAY	100

#define GPS_UNKNOWN_MESSAGES_TOLERANCE	10
#define GPS_RESET_MAX_TRIES				3


enum tGPSMessageClasses {
	UBX_CLASS_NAV	= 0x01,
	UBX_CLASS_RXM	= 0x02,
	UBX_CLASS_INF	= 0x04,
	UBX_CLASS_ACK	= 0x05,
	UBX_CLASS_CFG	= 0x06,
	UBX_CLASS_MON	= 0x0A,
	UBX_CLASS_AID	= 0x0B,
	UBX_CLASS_TIM	= 0x0D
}; 


#define UBX_NAV_CLOCK		0x22
#define UBX_NAV_DGPS		0x31
#define UBX_NAV_DOP			0x04
#define UBX_NAV_POSECEF		0x01
#define UBX_NAV_POSLLH		0x02
#define UBX_NAV_PVT			0x07
#define UBX_NAV_SBAS		0x32
#define UBX_NAV_SOL			0x06
#define UBX_NAV_STATUS		0x03
#define UBX_NAV_SVINFO		0x30
#define UBX_NAV_TIMEGPS		0x20
#define UBX_NAV_TIMEUTC		0x21
#define UBX_NAV_VELECEF		0x11
#define UBX_NAV_VELNED		0x12

#define UBX_MON_VER			0x04
#define UBX_MON_HW2			0x0B
#define UBX_MON_HW			0x09
#define UBX_MON_IO			0x02
#define UBX_MON_MSGPP		0x06
#define UBX_MON_RXBUF		0x07
#define UBX_MON_RXR			0x21
#define UBX_MON_TXBUF		0x08

#define UBX_CFG_ANT			0x13
#define UBX_CFG_CFG			0x09
#define UBX_CFG_DAT			0x06
#define UBX_CFG_GNSS		0x3E
#define UBX_CFG_INF			0x02
#define UBX_CFG_ITFM		0x39
#define UBX_CFG_MSG			0x01
#define UBX_CFG_NAV5		0x24
#define UBX_CFG_NAVX5		0x23
#define UBX_CFG_NMEA		0x17
#define UBX_CFG_NVS			0x22
#define UBX_CFG_PM2			0x3B
#define UBX_CFG_PRT			0x00
#define UBX_CFG_RATE		0x08
#define UBX_CFG_RINV		0x34
#define UBX_CFG_RST			0x04
#define UBX_CFG_RXM			0x11
#define UBX_CFG_SBAS		0x16
#define UBX_CFG_TP5			0x31
#define UBX_CFG_USB			0x1B

#define UBX_ACK_ACK			0x01
#define UBX_ACK_NAK			0x00

#define UBX_INF_DEBUG		0x04
#define UBX_INF_ERROR		0x00
#define UBX_INF_NOTICE		0x02
#define UBX_INF_TEST		0x03
#define UBX_INF_WARNING		0x01

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
	GPS_RESPONSE_NAK		= 0,
	GPS_RESPONSE_ACK		= 1,
	GPS_RESPONSE_UNKNOWN	= 2
};

enum tGPSStateMachine {
	GPS_STATE_UNKNOWN,
	GPS_STATE_SYNC1,
	GPS_STATE_SYNC2,
	GPS_STATE_CLASS,
	GPS_STATE_ID,
	GPS_STATE_LENGTH1,
	GPS_STATE_LENGTH2,
	GPS_STATE_PAYLOAD,
	GPS_STATE_XSUMA,
	GPS_STATE_XSUMB,
	GPS_STATE_RX_COMPLETE
};

struct tUbxNavPvtValidBitfield {
	unsigned reserved		: 5;	// Reserved bits
	unsigned fullyResolved	: 1;	// UTC Time of Day has been fully resolved
	unsigned validTime		: 1;	// Valid UTC Time of Day
	unsigned validDate		: 1;	// Valid UTC Date
};

struct tUbxNavPvtFlagsBitfield {
	unsigned reserved		: 3;	// Reserved bits
	unsigned psmState		: 3;	// Power Save Mode State
	unsigned diffSoln		: 1;	// Differential Corrections Applied
	unsigned gnssFixOk		: 1;	// A Valid Fix Achieved
};

struct tUbxNavPvt {
	unsigned int iTOW;		// Time of the week, milliseconds
	unsigned short year;	// Year (UTC)
	unsigned char month;	// Month (UTC)
	unsigned char day;		// Day (UTC)
	unsigned char hour;		// Hour (UTC)
	unsigned char minutes;	// Minutes (UTC)
	unsigned char seconds;	// Seconds (UTC)
	struct tUbxNavPvtValidBitfield valid;
	unsigned int tAcc;		// Time Accuracy Estimate (nanoseconds)
	signed int nano;		// Fraction of a second (nanoseconds)
	unsigned char fixType;	// GNSS Fix Type
	struct tUbxNavPvtFlagsBitfield flags;	// Fix Status Flags
	unsigned char reserved1;
	unsigned char numSV;	// Number of satellites used in Nav Solution
	signed int lon;			// Longitude (deg)
	signed int lat;			// Latitude (deg)
	signed int height;		// Height above ellipsoid (mm)
	signed int hMSL;		// Height above mean sea level (mm)
	unsigned int hAcc;		// Horizontal Accuracy Estimate (mm)
	unsigned int vAcc;		// Vertical Accuracy Estimate (mm)
	signed int velN;		// NED North Velocity (mm/s)
	signed int velE;		// NED East Velocity (mm/s)
	signed int velD;		// NED Down Velocity (mm/s)
	signed int gSpeed;		// 2D Ground Speed (mm/s)
	signed int heading;		// Heading of 2D motion (deg)
	unsigned int sAcc;		// Speed Accuracy Estimate (mm/s)
	unsigned int headingAcc;	// Heading Accuracy Estimate (deg)
	unsigned short pDOP;	// Position Dillution of Precision (0.01)
	unsigned short reserved2;
	unsigned int reserved3;
};

#define UBX_MON_VER_SW_VERSION_SIZE		30
#define UBX_MON_VER_HW_VERSION_SIZE		10
#define UBX_MON_VER_EXTENSION_SIZE		30

struct tUbxMonVer {
	unsigned char swVersion[30];
	unsigned char hwVersion[10];
	unsigned char extension1[UBX_MON_VER_EXTENSION_SIZE];
	unsigned char extension2[UBX_MON_VER_EXTENSION_SIZE];
};

#define UBX_CFG_USB_VENDOR_STRING_SIZE	32
#define UBX_CFG_USB_PRODUCT_STRING_SIZE	32
#define UBX_CFG_USB_SERIAL_NUMBER_SIZE	32

struct tUbxCfgUsb {
	unsigned short vendorID;
	unsigned short productID;
	unsigned short reserved1;
	unsigned short reserved2;
	unsigned short powerConsumption;
	unsigned short flags;
	unsigned char vendorString[UBX_CFG_USB_VENDOR_STRING_SIZE];
	unsigned char productString[UBX_CFG_USB_PRODUCT_STRING_SIZE];
	unsigned char serialNumber[UBX_CFG_USB_SERIAL_NUMBER_SIZE];
};

enum tUbxMonHW2CfgSource {
	CFG_SOURCE_ROM		= 114,
	CFG_SOURCE_OTP		= 111,
	CFG_SOURCE_IO		= 112,
	CFG_SOURCE_FLASH	= 102
};

struct tUbxMonHW2 {
	signed char ofsI;					// Imbalance of I-part of complex signal, scaled +/- 128
	unsigned char magI;					// Magnitude of I-part of complex signal
	signed char ofsQ;					// Imbalance of Q-part of complete signal, scale +/- 128
	unsigned char magQ;					// Magnitude of Q-part of complex signal
	unsigned char cfgSource;			// Source of low-level configuration
	unsigned char reserved0[3];			// Reserved
	unsigned int lowLevelCfg;			// Low-level configuration
	unsigned int reserved1[2];			// Reserved
	unsigned int postStatus;			// POST Status word
	unsigned int reserved2;				// Reserved
};

enum tUbxMonHWaPower {
	ANTENNA_POWER_OFF		= 0,
	ANTENNA_POWER_ON		= 1,
	ANTENNA_POWER_UNKNOWN	= 2
};

struct tUbxMonHWFlags {
	unsigned rtcCalib		: 1;
	unsigned safeBoot		: 1;
	unsigned jammingState	: 2;
	unsigned reserved		: 4;
};

#define UBX_MON_HW_VP_SIZE	25

struct tUbxMonHW {
	unsigned int pinSel;				// Mask of Pins Set as Peripheral/PIO
	unsigned int pinBank;				// Mask of Pins Set as Bank A/B
	unsigned int pinDir;				// Mask of Pins set as Input/Output
	unsigned int pinVal;				// Mask of Pins value Low/High
	unsigned short noisePerMS;			// Noise Level as measured by the GPS Core
	unsigned short agcCnt;				// AGC Monitor
	unsigned char aStatus;				// Status of the Antenna Supervisor State Machine
	unsigned char aPower;				// Current Power Status of Antenna
	unsigned char flags;				// Flags
	unsigned char reserved1;			// Reserved
	unsigned int usedMask;				// Mask of Pins that are used by the virtual pin manager
	unsigned char VP[UBX_MON_HW_VP_SIZE];	// Array of Pin Mappings for each of the physical pins
	unsigned char jamInd;				// CW Jamming Indicator, scaled (0 = no jamming, 255 = strong jamming)
	unsigned short reserved3;			// Reserved
	unsigned int pinIrq;				// Mask of pins value using the PIO IRQ
	unsigned int pullH;					// Mask of pins value using the PIO Pull High Resistor
	unsigned int pullL;					// Mask of pins value using the PIO Pull Low Resistor
};


struct tUbxAckAck {
	unsigned char clsID;				// Class ID of the acknowledged message
	unsigned char msgID;				// Message ID of the acknowledged message
};

struct tUbxAckNak {
	unsigned char clsID;				// Class ID of the acknowledged message
	unsigned char msgID;				// Message ID of the acknowledged message
};

union tUBXMessages {
	struct tUbxNavPvt NAV_PVT;
	struct tUbxMonVer MON_VER;
	struct tUbxMonHW  MON_HW;
	struct tUbxMonHW2 MON_HW2;
	struct tUbxCfgUsb CFG_USB;
	struct tUbxAckAck ACK_ACK;
	struct tUbxAckNak ACK_NAK;
	unsigned char raw[GPS_MSG_MAX_LENGTH];
};

struct tGPSMessage {
	enum tGPSMessageClasses class;
	unsigned char id;
	unsigned short length;
	unsigned short rxCount;
	union tUBXMessages messages;
	unsigned char xsumA;
	unsigned char xsumB;
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
	unsigned char id;
	unsigned char class;
	enum tGPSCmdResponse response;
};

struct tGPSDebug {
	signed char ofsI;	// Imbalance of I-part of complex signal
	unsigned char magI;	// Magnitude of I-part of complex signal
	signed char ofsQ;	// Imbalance of Q-part of complex signal
	unsigned char magQ;	// Magnitude of Q-part of complex signal
};

#define GPS_INFO_SW_VERSION_SIZE	30
#define GPS_INFO_SW_DATE_SIZE		10
#define GPS_INFO_HW_VERSION_SIZE	10
#define GPS_INFO_SERIALNO_SIZE		32

struct tGPSInfo {
	unsigned char	serial_number[GPS_INFO_SERIALNO_SIZE];
	unsigned char	serial_number_valid;
	
	unsigned char	sw_version[GPS_INFO_SW_VERSION_SIZE];
	unsigned char	sw_version_valid;
	
	unsigned char	sw_date[GPS_INFO_SW_DATE_SIZE];
	unsigned char	sw_date_valid;
	
	unsigned char	part_number[GPS_INFO_HW_VERSION_SIZE];
	unsigned char	part_number_valid;
	
	unsigned char	mode;
	unsigned char	satellites;
	unsigned char	record_flag;
	
	struct	tGPSPoint current_location;		// Last received position data
	enum	tGPSStatus status;				// GPS Receiver status
	struct	tGPSError error;				// Error counts
	struct	tGPSLastCmd lastCmd;			// Response from last sent command
	struct	tGPSDebug debug;				// Debug Info
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
