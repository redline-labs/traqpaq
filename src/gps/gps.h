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
 * Copyright (c) 2011 Redline Electronics LLC.
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

#define GPS_MSG_START_CHAR			'$'		
#define GPS_MSG_END_CHAR			0x0A	// ASCII for LF
#define GPS_MSG_CR					0x0D
#define GPS_MSG_MAX_STRLEN			90		// Each message is limited to 82 characters max, including '$', CR, and LF
#define GPS_MSG_DELIMITER_CHAR		','
#define GPS_CHECKSUM_CHAR			'*'
#define GPS_PERIOD					'.'

#define GPS_MSG_ID_POS				4		// Look at the 5th character to identify NMEA message
#define GPS_MSG_ID_PMTK				'K'
#define GPS_MSG_ID_GGA				'G'
#define GPS_MSG_ID_GSV				'S'
#define GPS_MSG_ID_RMC				'M'

#define GPS_PACKET_START			7

#define GPS_NULL					0

#define GPS_MSGS_PER_INTERRUPT		15		// 3 Messages at 5Hz

#define GPS_BUFFER_SIZE				(GPS_MSGS_PER_INTERRUPT * GPS_MSG_MAX_STRLEN)

#define GPS_SIGNAL_MAX_LEN			12


typedef struct tGPSMsgGGA {		// Raw ASCII NMEA messages
	unsigned char utc[GPS_SIGNAL_MAX_LEN];		//TODO: Determine what to do with the period
	unsigned char latitude[GPS_SIGNAL_MAX_LEN];	//TODO: Determine what to do with the period
	unsigned char latitudeNorS[GPS_SIGNAL_MAX_LEN];
	unsigned char longitude[GPS_SIGNAL_MAX_LEN];	//TODO: Determine what do to with the period
	unsigned char longitudeEorW[GPS_SIGNAL_MAX_LEN];
	unsigned char quality[GPS_SIGNAL_MAX_LEN];
	unsigned char satelites[GPS_SIGNAL_MAX_LEN];
	unsigned char hdop[GPS_SIGNAL_MAX_LEN];
	unsigned char altitude[GPS_SIGNAL_MAX_LEN];
	//unsigned char altitudeUnits;
	//unsigned char geoidal[6];
	//unsigned char geoidalUnits;
	//unsigned char diffGPSDataAge[6];
	//unsigned char diffGPSID[7];
	//unsigned char checksum[3];	
};


typedef struct tGPSMsgGSV {		// Raw ASCII NMEA messages
	unsigned char mode;
	unsigned char currentMode;
	unsigned char PRNofSatellite[12][2];
	unsigned char pdop[3];
	unsigned char hdop[3];
	unsigned char vdop[3];
	unsigned char checksum[2];
};


typedef struct tGPSMsgRMC {		// Raw ASCII NMEA message
	//unsigned char utc[9];
	//unsigned char status;
	//unsigned char latitude[8];
	//unsigned char latitudeNorS;
	//unsigned char longitude[9];
	//unsigned char longitudeEorW;
	unsigned char speed[GPS_SIGNAL_MAX_LEN];
	unsigned char track[GPS_SIGNAL_MAX_LEN];
	//unsigned char date[6];
	//unsigned char magVar;
	//unsigned char magVarEorW;
	//unsigned char checksum[2];
};

// Struct for storing GPS messages as they come in
typedef struct tGPSRXDBuffer{
  // Message Complete - Used to determine if ISR has detected end of line for message
  unsigned char complete;

  // Message Valid - Use defines for determing validity of message
  unsigned char valid;

  unsigned char processed;

  // End position of
  unsigned char rawpos;

  // Raw, unfiltered message.  Keep variable type as int to be compatible with USART ISR
  unsigned char raw[GPS_MSG_MAX_STRLEN];
};





// Prototypes
//void gps_processMsg(struct tGPSRXDBuffer *GPSRXDBuffer, unsigned char index, struct tGPSMsgGGA *GPSMsgGGA, struct tGPSMsgGSV *GPSMsgGSV);

void gps_task_init( void );
void gps_task( void *pvParameters );
void gps_irq( void );

#endif /* GPS_H_ */
