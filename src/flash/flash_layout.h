/******************************************************************************
 *
 * Memory Layout
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


#ifndef DATAFLASH_LAYOUT_H_
#define DATAFLASH_LAYOUT_H_

#define FLASH_LAYOUT_VERSION		0x0120
#define FLASH_LAYOUT_VERSION_ASCII	"1.20"

#define FLASH_PAGE_SIZE		256


struct __attribute__ ((packed)) tRecordsEntry {
	unsigned char recordEmpty;
	unsigned char trackID;
	unsigned char reserved1[2];

	unsigned int datestamp;
	unsigned int startAddress;
	unsigned int endAddress;
};	// tRecordsEntry - 16 Bytes

#define RECORD_ENTRY_SIZE		sizeof(tRecordsEntry)
#define RECORDS_TOTAL_POSSIBLE	256
#define RECORDS_ENTRY_PER_PAGE	16

struct tRecordsEntryPage {
	struct tRecordsEntry record[RECORDS_ENTRY_PER_PAGE];
};

struct __attribute__ ((packed)) tRecordData {
	signed int latitude;
	signed int longitude;
	
	unsigned char lapDetected;
	unsigned char reserved;
	unsigned short altitude; 
	
	unsigned short speed;
	unsigned short heading;
}; // 16 bytes

#define RECORD_DATA_PER_PAGE	15

struct __attribute__ ((packed)) tRecordDataPage {
	unsigned int utc;
	
	unsigned short hdop;
	unsigned char currentMode;
	unsigned char satellites;
	
	unsigned char reserved[8];
	
	struct tRecordData data[RECORD_DATA_PER_PAGE];
}; // 256 bytes




struct tUserPrefs {
	unsigned char	screenPWMMax;		// Max Screen Brightness ( 8-bit PWM value; 0 - 255 )
	unsigned char	screenPWMMin;		// Min Screen Brightness ( 8-bit PWM value; 0 - 255 )
	unsigned short	screenFadeTime;		// Inactive time for module until screen fades darker
	unsigned short	screenOffTime;		// Inactive time for module until screen turns off
	unsigned short  crc;
};


#define TRACKLIST_MAX_STRLEN		20

struct __attribute__ ((packed)) tTracklist {
	unsigned char name[TRACKLIST_MAX_STRLEN];
	
	signed int longitude;
	signed int latitude;
	
	unsigned short heading;
	unsigned char isEmpty;
	unsigned char reserved;
}; // 32 Bytes

#define TRACKLIST_TOTAL_NUM					120		// Maximum number of tracks able to be stored

// Flash Memory Layout for Atmel AT25DF161
#define FLASH_AT25DF161_USERPREFS_START		0x00000000
#define FLASH_AT25DF161_USERPREFS_END		0x000000FF

#define FLASH_AT25DF161_TRACKLIST_START		0x00000100
#define FLASH_AT25DF161_TRACKLIST_END		0x00000FFF	// Align to 4KB sector

#define FLASH_AT25DF161_RECORDTABLE_START	0x00001000
#define FLASH_AT25DF161_RECORDTABLE_END		0x00001FFF	// Align to 4KB sector

#define FLASH_AT25DF161_RECORDDATA_START	0x00002000	// Size is remainder of flash
#define FLASH_AT25DF161_RECORDDATA_END		0x001FFFFF	// Dataflash End Address


// Flash Memory Layout for Atmel AT25DF321
#define FLASH_AT25DF321_USERPREFS_START		0x00000000
#define FLASH_AT25DF321_USERPREFS_END		0x000000FF

#define FLASH_AT25DF321_TRACKLIST_START		0x00000100
#define FLASH_AT25DF321_TRACKLIST_END		0x00000FFF	// Align to 4KB sector

#define FLASH_AT25DF321_RECORDTABLE_START	0x00001000
#define FLASH_AT25DF321_RECORDTABLE_END		0x00001FFF	// Align to 4KB sector

#define FLASH_AT25DF321_RECORDDATA_START	0x00002000	// Size is remainder of flash
#define FLASH_AT25DF321_RECORDDATA_END		0x003FFFFF	// Dataflash End Address

#endif /* DATAFLASH_LAYOUT_H_ */