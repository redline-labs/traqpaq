/******************************************************************************
 *
 * Memory Layout
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


#ifndef DATAFLASH_LAYOUT_H_
#define DATAFLASH_LAYOUT_H_

#define DATAFLASH_LAYOUT_VERSION	"1.00"

#define DATAFLASH_PAGE_SIZE		256


typedef struct tRecordsEntry {
	unsigned record_complete	:1;  
	unsigned buffer_overrun		:1;	
	unsigned gps_overrun		:1;	
	unsigned crc_error			:1;
	unsigned mem_full			:1;							
	unsigned reserved			:3;

	unsigned long datestamp;
	unsigned long startAddress;
	unsigned long endAddress;
	unsigned char trackID;
	unsigned char reserved[2];
};	// tRecordsEntry - 16 Bytes

#define RECORD_ENTRY_SIZE		sizeof(tRecordsEntry)
#define RECORDS_TOTAL_POSSIBLE	64
#define RECORDS_ENTRY_PER_PAGE	16

typedef struct tRecordsEntryPage {
	struct tRecordsEntry record[RECORDS_ENTRY_PER_PAGE];
};

/*
unsigned long latitude;
unsigned short latitude_decimal;
unsigned long longitude;
unsigned short longitude_decimal;

*/

typedef struct tRecordData {
	unsigned char latitude[10];
	unsigned char NorS;
	unsigned char longitude[10];
	unsigned char EorW;
	unsigned char speed[5];
	unsigned char altitude[5]
}; // 32 bytes

#define RECORD_DATA_PER_PAGE	8

typedef struct tRecordDataPage {
	struct tRecordData data[RECORD_DATA_PER_PAGE];
};

#define DATAFLASH_ADDR_RECORDTABLE_START		0x00000000	// Size is exactly 1024B
#define DATAFLASH_ADDR_RECORDTABLE_END			0x000003FF

#define DATAFLASH_ADDR_RECORDDATA_START			0x00000400	// Size is remainder of 16Mb
#define DATAFLASH_ADDR_RECORDDATA_END			0x001FFFFF

// Define the the number of record samples available in Dataflash space alloted
//#define RECORDSDATA_TOTAL_POSSIBLE (DATAFLASH_ADDR_RECORDDATA_END - DATAFLASH_ADDR_RECORDDATA_START) / RECORD_PAGE_SIZE


#endif /* DATAFLASH_LAYOUT_H_ */