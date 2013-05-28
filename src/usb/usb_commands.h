/******************************************************************************
 *
 * USB Commands
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


#ifndef USB_COMMANDS_H_
#define USB_COMMANDS_H_

/////////////////////////////
// USB Tx Messages         //
/////////////////////////////
union tUsbTxMessages{
	struct __attribute__ ((packed)) tUsbTxReqVersions {
		unsigned char swVerMajor;
		unsigned char swVerMinor;
		unsigned char hwVer;
		unsigned char serialNumber[OTP_SERIAL_LENGTH];
		unsigned char testerId;
	} REQ_VERSIONS;

	struct __attribute__ ((packed)) tUsbTxReqBatteryInfo {
		unsigned short	batteryVoltage;
		unsigned short	batteryTemperature;
		signed short	instantCurrent;
		signed short	accumulatedCurrent;
	} REQ_BATTERY_INFO;

	struct __attribute__ ((packed)) tUsbTxCmdWriteBatteryInfo {
		unsigned char success;
	} CMD_WRITE_BATTERY_INFO;

	struct tTracklist REQ_READ_SAVED_TRACKS;

	struct tRecordsEntry CMD_READ_RECORD_TABLE;

	struct __attribute__ ((packed)) tUsbTxCmdReadRecordData {
		unsigned char data[FLASH_PAGE_SIZE]
	} CMD_READ_RECORD_DATA;

	struct __attribute__ ((packed)) tUsbTxCmdEraseRecordData {
		unsigned char success;
	} CMD_ERASE_RECORD_DATA;

	struct __attribute__ ((packed)) tUsbTxCmdWriteUserPrefs {
		unsigned char success;
	} CMD_WRITE_USER_PREFS;

	struct __attribute__ ((packed)) tUsbTxCmdWriteSavedTracks {
		unsigned char success;
	} CMD_WRITE_SAVED_TRACKS;

	struct __attribute__ ((packed)) tUsbTxCmdReadOtp {
		unsigned char data[FLASH_OTP_SIZE];
	} CMD_READ_OTP;

	struct __attribute__ ((packed)) tUsbTxCmdWriteOtp {
		unsigned char success;
	} CMD_WRITE_OTP;

	struct tADCvalues DBG_READ_ADC_VALUES;

	struct __attribute__ ((packed)) tUsbTxDbgFlashSectorErase {
		unsigned char success;
	} DBG_FLASH_SECTOR_ERASE;

	struct __attribute__ ((packed)) tUsbTxDbgFlashStatus {
		unsigned char busy;
		unsigned char isFull;
		unsigned char usedPercent;
	} DBG_FLASH_STATUS;

	struct __attribute__ ((packed)) tUsbTxDbgChipErase {
		unsigned char success;
	} DBG_FLASH_CHIP_ERASE;

	struct __attribute__ ((packed)) tUsbTxDbgArbRead {
		unsigned char data[FLASH_PAGE_SIZE]
	} DBG_FLASH_ARB_READ;

	struct __attribute__ ((packed)) tUsbTxDbgArbWrite {
		unsigned char success;
	} DBG_FLASH_ARB_WRITE;

	struct __attribute__ ((packed)) tUsbTxDbgGpsCurrentPosition {
		signed int latitude;
		signed int longitude;
		unsigned short course;
	} DBG_GPS_CURRENT_POSITION;

	struct __attribute__ ((packed)) tUsbTxDbgGpsCurrentMode {
		unsigned char mode;
		unsigned char satellites;
	} DBG_GPS_CURRENT_MODE;

	struct __attribute__ ((packed)) tUsbTxDbgGpsInfoSerialNo {
		unsigned char valid;
		unsigned char serialNumber[GPS_INFO_SERIALNO_SIZE];
	} DBG_GPS_SERIAL_NUMBER;

	struct __attribute__ ((packed)) tUsbTxDbgGpsInfoPartNo {
		unsigned char valid;
		unsigned char partNumber[GPS_INFO_HW_VERSION_SIZE];
	} DBG_GPS_PART_NUMBER;

	struct __attribute__ ((packed)) tUsbTxDbgGpsInfoSwVer {
		unsigned char valid;
		unsigned char swVer[GPS_INFO_SW_VERSION_SIZE];
	} DBG_GPS_SW_VER;

	struct __attribute__ ((packed)) tUsbTxDbgGpsInfoSwDate {
		unsigned char valid;
		unsigned char swDate[GPS_INFO_SW_DATE_SIZE];
	} DBG_GPS_SW_DATE;

	struct __attribute__ ((packed)) tUsbTxDbgGpsLastResponse {
		unsigned char class;
		unsigned char id;
		unsigned char response;
	} DBG_GPS_LAST_RESPONSE;

	struct __attribute__ ((packed)) tUsbTxDbgStartRecording {
		unsigned char success;
	} DBG_GPS_START_RECORDING;

	struct __attribute__ ((packed)) tUsbTxDbgStopRecording {
		unsigned char success;
	} DBG_GPS_STOP_RECORDING;

	struct __attribute__ ((packed)) tUsbTxDbgRecordingStatus {
		unsigned char status;
	} DBG_GPS_RECORDING_STATUS;

	struct __attribute__ ((packed)) tUsbTxDbgAccelGetStatus {
		unsigned char available;
		unsigned char selfTestPassed;
		enum tAccelStatus status;
	} DBG_ACCEL_GET_STATUS;

	struct __attribute__ ((packed)) tUsbTxDbgAccelFiltData {
		unsigned short x;
		unsigned short y;
		unsigned short z;
	} DBG_ACCEL_GET_FILT_DATA;

	struct __attribute__ ((packed)) tUsbTxDbgAccelNormData {
		unsigned short x;
		unsigned short y;
		unsigned short z;
	} DBG_ACCEL_GET_NORM_DATA;

	struct __attribute__ ((packed)) tUsbTxDbgAccelStData {
		unsigned short x;
		unsigned short y;
		unsigned short z;
	} DBG_ACCEL_GET_ST_DATA;

	struct __attribute__ ((packed)) tUsbTxDbgGpsLoadAid {
		unsigned char success;
	} DBG_GPS_LOAD_AID;

	struct __attribute__ ((packed)) tUsbTxTaskList {
		unsigned char success;
	} DBG_TASK_LIST;
	
	unsigned char raw[FLASH_PAGE_SIZE];
};

struct __attribute__ ((packed)) tUsbTxFrame {
	unsigned char command;
	unsigned short msgLength;
	union tUsbTxMessages message;
};




/////////////////////////////
// USB Rx Messages         //
/////////////////////////////
union tUsbRxMessages{
	struct __attribute__ ((packed)) tUsbRxReqVersions {
	} CMD_REQ_VERSIONS;

	struct __attribute__ ((packed)) tUsbRxReqBatteryInfo {
	} CMD_REQ_BATTERY_INFO;

	struct tBatteryInfo CMD_WRITE_BATTERY_INFO;

	struct __attribute__ ((packed)) tUsbRxCmdReadSavedTracks {
		unsigned short index;
	} REQ_READ_SAVED_TRACKS;

	struct __attribute__ ((packed)) tUsbRxReqReadRecordTable {
		unsigned char index;
	} CMD_READ_RECORD_TABLE;

	struct __attribute__ ((packed)) tUsbRxCmdReadRecordData {
		unsigned short length;
		unsigned short index;
	} CMD_READ_RECORD_DATA;

	struct __attribute__ ((packed)) tUsbRxCmdEraseRecordData {
	} CMD_ERASE_RECORD_DATA;

	struct __attribute__ ((packed)) tUsbRxCmdWriteUserPrefs {
		unsigned char	screenPWMMax;
		unsigned char	screenPWMMin;
		unsigned short	screenFadeTime;
		unsigned short	screenOffTime;
	} CMD_WRITE_USER_PREFS;

	struct __attribute__ ((packed)) tUsbRxCmdWriteSavedTracks {
		unsigned char name[TRACKLIST_MAX_STRLEN];
		
		signed int longitude;
		signed int latitude;
		
		unsigned short heading;
		unsigned char isEmpty;
		unsigned char reserved;
	} CMD_WRITE_SAVED_TRACKS;

	struct __attribute__ ((packed)) tUsbRxCmdReadOtp {
		unsigned char length;
		unsigned char index;
	} CMD_READ_OTP;

	struct __attribute__ ((packed)) tUsbRxCmdWriteOtp {
		unsigned char serial[OTP_SERIAL_LENGTH];
		unsigned char pcb_rev;
		unsigned char tester_id;
		unsigned char reserved;
		unsigned short crc;
	} CMD_WRITE_OTP;

	struct __attribute__ ((packed)) tUsbRxDbgReadAdcValues {
	} DBG_READ_ADC_VALUES;

	struct __attribute__ ((packed)) tUsbRxDbgFlashSectorErase {
		unsigned char index;
	} DBG_FLASH_SECTOR_ERASE;

	struct __attribute__ ((packed)) tUsbRxDbgFlashBusy {
	} DBG_FLASH_BUSY;

	struct __attribute__ ((packed)) tUsbRxDbgChipErase {
	} DBG_FLASH_CHIP_ERASE;

	struct __attribute__ ((packed)) tUsbRxDbgFlashFull {
	} DBG_FLASH_IS_FULL;

	struct __attribute__ ((packed)) tUsbRxDbgFlashUsedSpace {
	} DBG_FLASH_USED_SPACE;

	struct __attribute__ ((packed)) tUsbRxDbgArbRead {
		unsigned short length;
		unsigned short index;
	} DBG_FLASH_ARB_READ;

	struct __attribute__ ((packed)) tUsbRxDbgArbWrite {
		unsigned short index;
		unsigned char data[FLASH_PAGE_SIZE];
	} DBG_FLASH_ARB_WRITE;

	struct __attribute__ ((packed)) tUsbRxDbgGpsCurrentPosition {
	} DBG_GPS_CURRENT_POSITION;

	struct __attribute__ ((packed)) tUsbRxDbgGpsCurrentMode {
	} DBG_GPS_CURRENT_MODE;

	struct __attribute__ ((packed)) tUsbRxDbgGpsInfoSerialNo {
	} DBG_GPS_SERIAL_NUMBER;

	struct __attribute__ ((packed)) tUsbRxDbgGpsInfoPartNo {
	} DBG_GPS_PART_NUMBER;

	struct __attribute__ ((packed)) tUsbRxDbgGpsInfoSwVer {
	} DBG_GPS_SW_VER;

	struct __attribute__ ((packed)) tUsbRxDbgGpsInfoSwDate {
	} DBG_GPS_SW_DATE;

	struct __attribute__ ((packed)) tUsbRxDbgGpsLastResponse {
	} DBG_GPS_LAST_RESPONSE;

	struct __attribute__ ((packed)) tUsbRxDbgStartRecording {
	} DBG_GPS_START_RECORDING;

	struct __attribute__ ((packed)) tUsbRxDbgStopRecording {
	} DBG_GPS_STOP_RECORDING;

	struct __attribute__ ((packed)) tUsbRxDbgRecordingStatus {
	} DBG_GPS_RECORDING_STATUS;

	struct __attribute__ ((packed)) tUsbRxDbgAccelGetStatus {
	} DBG_ACCEL_GET_STATUS;

	struct __attribute__ ((packed)) tUsbRxDbgAccelFiltData {
	} DBG_ACCEL_GET_FILT_DATA;

	struct __attribute__ ((packed)) tUsbRxDbgAccelNormData {
	} DBG_ACCEL_GET_NORM_DATA;

	struct __attribute__ ((packed)) tUsbRxDbgAccelStData {
	} DBG_ACCEL_GET_ST_DATA;

	struct __attribute__ ((packed)) tUsbRxDbgGpsLoadAid {
	} DBG_GPS_LOAD_AID;

	struct __attribute__ ((packed)) tUsbRxTaskList {
	} DBG_TASK_LIST;
	
	unsigned char raw[FLASH_PAGE_SIZE];
};

struct __attribute__ ((packed)) tUsbRxFrame {
	unsigned char command;
	unsigned short msgLength;
	union tUsbRxMessages message;
};




#endif /* USB_COMMANDS_H_ */