/*
 * record_structure.h
 *
 * Created: 6/26/2011 11:13:36 PM
 *  Author: Ryan
 */ 


#ifndef DATAFLASH_LAYOUT_H_
#define DATAFLASH_LAYOUT_H_

#define DATAFLASH_LAYOUT_VERSION	"1.00"

#define DATAFLASH_PAGE_SIZE		256


typedef struct tRecordsEntry {
	unsigned char status;  // TODO: Need to create union individual bools for each of the status bits
							// bool complete
							// bool gps_buffer_overrun
							// bool crc_error
							// bool mem_full							
							// bool reserved0
							// bool reserved1
							// bool reserved2
							// bool reserved3
	unsigned long datestamp;
	unsigned long startAddress;
	unsigned long endAddress;
	unsigned char trackID;
	unsigned char reserved0;
	unsigned char reserved1;
};	// tRecordsEntry - 16 Bytes

#define RECORD_ENTRY_SIZE		sizeof(tRecordsEntry)
#define RECORDS_TOTAL_POSSIBLE	64						// 128 total possible entries in table
#define RECORDS_TABLE_SIZE		RECORD_ENTRY_SIZE * RECORDS_TOTAL_POSSIBLE	


typedef struct tRecordData {
	unsigned long latitude;
	unsigned long longitude;
	unsigned short speed;
	unsigned short course;
	unsigned long altitude;
};	// tRecordData - 16 Bytes

typedef struct tRecordPageInfo{
	unsigned long timestamp;
	unsigned long fixdata[3];	//TODO: Seperate this into more detailed variables.
};	// tRecordPageInfo - 16 Bytes

//#define RECORD_DATA_SIZE		sizeof(tRecordData)
//#define RECORD_PAGE_INFO_SIZE	sizeof(tRecordPageInfo)
#define RECORDS_PER_PAGE		15//(DATAFLASH_PAGE_SIZE - RECORD_PAGE_INFO_SIZE) / RECORD_DATA_SIZE

typedef struct tRecordPage{
	struct tRecordPageInfo info;
	struct tRecordData sample[RECORDS_PER_PAGE];
};

#define RECORD_PAGE_SIZE	sizeof(tRecordPage)



typedef struct tUITheme{
	// ------------------
	// Background colors for lap faster or slower
	// ------------------
	unsigned short lapFaster;
	unsigned short lapSimilar;
	unsigned short lapSlower;
	unsigned short lapNormal;
	
	// ------------------
	// UI Theme elements
	// ------------------
	unsigned short wallpaperForeground;
	unsigned short wallpaperBackground;
	
	unsigned short titlebarForeground;
	unsigned short titlebarBackground;
	
	unsigned short titlebarIconForeground;
	unsigned short titlebarIconBackground;
	
	unsigned short mainForeground;
	unsigned short mainBackground;
	
	unsigned short errorForeground;
	unsigned short errorBackground;
	
	unsigned short analyticsForeground;
	unsigned short analyticsBackground;
	
};	// tUITheme - 32 Bytes


typedef struct tUserPrefs{
	unsigned char screenBrightness;
	unsigned char screenTimeout;
	unsigned char devicePowerOff;
	unsigned char gpsPowerOff;
	unsigned char autosuggestTracks;	
	unsigned char timedMotos;
	unsigned char colorFeedback;
	// TODO: Fill in any remaining user preferences
	
	struct tUITheme colors;
	
	unsigned short crc;		// CRC of entire user prefs section
};	// tUserPrefs - 41 Bytes

#define USERPREFS_SIZE		sizeof(tUserPrefs)

#define TRACK_INFO_NAME_LENGTH	20

typedef struct tTrackInfo{
	unsigned char id;
	unsigned char name[TRACK_INFO_NAME_LENGTH];
	unsigned long latitude;
	unsigned long longitude;
	unsigned short course;
	unsigned char reserved;
};	// tTrackInfo - 32 Bytes

#define TRACK_INFO_SIZE		sizeof(tTrackInfo);
#define TRACK_TOTAL_SAVED	32	// Number of tracks total to save


#define DATAFLASH_ADDR_USERPREFS_START			0x00000000	// Size is only 41B, but align to page
#define DATAFLASH_ADDR_USERPREFS_END			0x000000FF
#define DATAFLASH_ADDR_SAVEDTRACKS_START		0x00000100	// Size is exactly 1024B
#define DATAFLASH_ADDR_SAVEDTRACKS_END			0x000004FF
#define DATAFLASH_ADDR_RECORDTABLE_START		0x00000500	// Size is exactly 1024B
#define DATAFLASH_ADDR_RECORDTABLE_END			0x000008FF
#define DATAFLASH_ADDR_RECORDDATA_START			0x00000900	// Size is remainder of 16Mb
#define DATAFLASH_ADDR_RECORDDATA_END			0x001FFFFF

// Define the the number of record samples available in Dataflash space alloted
#define RECORDSDATA_TOTAL_POSSIBLE (DATAFLASH_ADDR_RECORDDATA_END - DATAFLASH_ADDR_RECORDDATA_START) / RECORD_PAGE_SIZE


#endif /* DATAFLASH_LAYOUT_H_ */