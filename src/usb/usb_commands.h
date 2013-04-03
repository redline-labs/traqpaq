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

// Standard header
struct __attribute__ ((packed)) tUsbStdHeader {
	unsigned char command;
	unsigned short msgLength;
};

/////////////////////////////
// USB Tx Messages         //
/////////////////////////////
struct __attribute__ ((packed)) tUsbTxCmdReqApplVer {
	unsigned char swVerMajor;
	unsigned char swVerMinor;
};

struct __attribute__ ((packed)) tUsbTxCmdReqHwVer {
	unsigned char hwVer;
};

struct __attribute__ ((packed)) tUsbTxCmdReqSerialNumber {
	unsigned char serialNumber[OTP_SERIAL_LENGTH];
};

struct __attribute__ ((packed)) tUsbTxCmdReqTesterId {
	unsigned char testerId;
}; 

struct __attribute__ ((packed)) tUsbTxCmdReqBatteryVoltage {
	unsigned short batteryVoltage;
};

struct __attribute__ ((packed)) tUsbTxCmdReqBatteryTemperature {
	unsigned short batteryTemperature;
};

struct __attribute__ ((packed)) tUsbTxCmdReqBatteryInstant{
	signed short instantCurrent;
};

struct __attribute__ ((packed)) tUsbTxCmdReqBatteryAccumulated{
	signed short accumulatedCurrent;	
};

struct __attribute__ ((packed)) tUsbTxCmdReqBatteryUpdate{
	unsigned short success;
};

struct __attribute__ ((packed)) tUsbTxCmdWriteBatteryInfo {
	unsigned char success;
};

struct __attribute__ ((packed)) tUsbTxCmdReadSavedTracks {
	unsigned char name[TRACKLIST_MAX_STRLEN];
	
	signed int longitude;
	signed int latitude;
	
	unsigned short heading;
	unsigned char isEmpty;
	unsigned char reserved;
};

struct __attribute__ ((packed)) tUsbTxCmdReadRecordTable {
	unsigned char recordEmpty;
	unsigned char trackID;
	unsigned char reserved1[2];

	unsigned int datestamp;
	unsigned int startAddress;
	unsigned int endAddress;
};

struct __attribute__ ((packed)) tUsbTxCmdReadRecordData {
	unsigned char data[FLASH_PAGE_SIZE]
};

struct __attribute__ ((packed)) tUsbTxCmdEraseRecordData {
	unsigned char success;
};

struct __attribute__ ((packed)) tUsbTxCmdWriteUserPrefs {
	unsigned char success;
};

struct __attribute__ ((packed)) tUsbTxCmdWriteSavedTracks {
	unsigned char success;
};

struct __attribute__ ((packed)) tUsbTxCmdReadOtp {
	unsigned char data[FLASH_OTP_SIZE];
};

struct __attribute__ ((packed)) tUsbTxCmdWriteOtp {
	unsigned char success;
};

struct __attribute__ ((packed)) tUsbTxDbgReadAdcValues {
	unsigned short main;
	unsigned short vcc;
	unsigned short vee;
};

struct __attribute__ ((packed)) tUsbTxDbgFlashSectorErase {
	unsigned char success;
};

struct __attribute__ ((packed)) tUsbTxDbgFlashBusy {
	unsigned char busy;
};

struct __attribute__ ((packed)) tUsbTxDbgChipErase {
	unsigned char success;
};

struct __attribute__ ((packed)) tUsbTxDbgFlashFull {
	unsigned char isFull;
};

struct __attribute__ ((packed)) tUsbTxDbgFlashUsedSpace {
	unsigned char usedPercent;
};

struct __attribute__ ((packed)) tUsbTxDbgArbRead {
	unsigned char data[FLASH_PAGE_SIZE]
};

struct __attribute__ ((packed)) tUsbTxDbgArbWrite {
	unsigned char success;
};

struct __attribute__ ((packed)) tUsbTxDbgGpsCurrentPosition {
	signed int latitude;
	signed int longitude;
	unsigned short course;
};

struct __attribute__ ((packed)) tUsbTxDbgGpsCurrentMode {
	unsigned char mode;
	unsigned char satellites;
};

struct __attribute__ ((packed)) tUsbTxDbgGpsInfoSerialNo {
	unsigned char valid;
	unsigned char serialNumber[GPS_INFO_SERIALNO_SIZE];
};

struct __attribute__ ((packed)) tUsbTxDbgGpsInfoPartNo {
	unsigned char valid;
	unsigned char partNumber[GPS_INFO_HW_VERSION_SIZE];
};

struct __attribute__ ((packed)) tUsbTxDbgGpsInfoSwVer {
	unsigned char valid;
	unsigned char swVer[GPS_INFO_SW_VERSION_SIZE];
};

struct __attribute__ ((packed)) tUsbTxDbgGpsInfoSwDate {
	unsigned char valid;
	unsigned char swDate[GPS_INFO_SW_DATE_SIZE];
};

struct __attribute__ ((packed)) tUsbTxDbgGpsLastResponse {
	unsigned char class;
	unsigned char id;
	unsigned char response;
};

struct __attribute__ ((packed)) tUsbTxDbgStartRecording {
	unsigned char success;
};

struct __attribute__ ((packed)) tUsbTxDbgStopRecording {
	unsigned char success;
};

struct __attribute__ ((packed)) tUsbTxDbgRecordingStatus {
	unsigned char status;
};

struct __attribute__ ((packed)) tUsbTxDbgAccelGetStatus {
	unsigned char status;
};

struct __attribute__ ((packed)) tUsbTxDbgAccelFiltData {
	unsigned short x;
	unsigned short y;
	unsigned short z;
};

struct __attribute__ ((packed)) tUsbTxDbgAccelNormData {
	unsigned short x;
	unsigned short y;
	unsigned short z;
};

struct __attribute__ ((packed)) tUsbTxDbgAccelStData {
	unsigned short x;
	unsigned short y;
	unsigned short z;
};

struct __attribute__ ((packed)) tUsbTxDbgGpsLoadAid {
	unsigned char success;
};

struct __attribute__ ((packed)) tUsbTxTaskList {
	unsigned char success;
};



union tUsbTxMessages{
	struct tUsbTxCmdReqApplVer				CMD_REQ_APPL_VER;
	struct tUsbTxCmdReqHwVer				CMD_REQ_HARDWARE_VER;
	struct tUsbTxCmdReqSerialNumber			CMD_REQ_SERIAL_NUMBER;
	struct tUsbTxCmdReqTesterId				CMD_REQ_TESTER_ID;
	struct tUsbTxCmdReqBatteryVoltage		CMD_REQ_BATTERY_VOLTAGE;
	struct tUsbTxCmdReqBatteryTemperature	CMD_REQ_BATTERY_TEMPERATURE;
	struct tUsbTxCmdReqBatteryInstant		CMD_REQ_BATTERY_INSTANT;
	struct tUsbTxCmdReqBatteryAccumulated	CMD_REQ_BATTERY_ACCUMULATED;
	struct tUsbTxCmdReqBatteryUpdate		CMD_REQ_BATTERY_UPDATE;
	struct tUsbTxCmdWriteBatteryInfo		CMD_REQ_WRITE_BATTERY_INFO;
	struct tUsbTxCmdReadSavedTracks			CMD_READ_SAVED_TRACKS;
	struct tUsbTxCmdReadRecordTable			CMD_READ_RECORD_TABLE;
	struct tUsbTxCmdReadRecordData			CMD_READ_RECORD_DATA;
	struct tUsbTxCmdEraseRecordData			CMD_ERASE_RECORD_DATA;
	struct tUsbTxCmdWriteUserPrefs			CMD_WRITE_USER_PREFS;
	struct tUsbTxCmdWriteSavedTracks		CMD_WRITE_SAVED_TRACKS;
	struct tUsbTxCmdReadOtp					CMD_READ_OTP;
	struct tUsbTxCmdWriteOtp				CMD_WRITE_OTP;
	struct tUsbTxDbgReadAdcValues			DBG_READ_ADC_VALUES;
	struct tUsbTxDbgFlashSectorErase		DBG_FLASH_SECTOR_ERASE;
	struct tUsbTxDbgFlashBusy				DBG_FLASH_BUSY;
	struct tUsbTxDbgChipErase				DBG_FLASH_CHIP_ERASE;
	struct tUsbTxDbgFlashFull				DBG_FLASH_IS_FULL;
	struct tUsbTxDbgFlashUsedSpace			DBG_FLASH_USED_SPACE;
	struct tUsbTxDbgArbRead					DBG_FLASH_ARB_READ;
	struct tUsbTxDbgArbWrite				DBG_FLASH_ARB_WRITE;
	struct tUsbTxDbgGpsCurrentPosition		DBG_GPS_CURRENT_POSITION;
	struct tUsbTxDbgGpsCurrentMode			DBG_GPS_CURRENT_MODE;
	struct tUsbTxDbgGpsInfoSerialNo			DBG_GPS_SERIAL_NUMBER;
	struct tUsbTxDbgGpsInfoPartNo			DBG_GPS_PART_NUMBER;
	struct tUsbTxDbgGpsInfoSwVer			DBG_GPS_SW_VER;
	struct tUsbTxDbgGpsInfoSwDate			DBG_GPS_SW_DATE;
	struct tUsbTxDbgGpsLastResponse			DBG_GPS_LAST_RESPONSE;
	struct tUsbTxDbgStartRecording			DBG_GPS_START_RECORDING;
	struct tUsbTxDbgStopRecording			DBG_GPS_STOP_RECORDING;
	struct tUsbTxDbgRecordingStatus			DBG_GPS_RECORDING_STATUS;
	struct tUsbTxDbgAccelGetStatus			DBG_ACCEL_GET_STATUS;
	struct tUsbTxDbgAccelFiltData			DBG_ACCEL_GET_FILT_DATA;
	struct tUsbTxDbgAccelNormData			DBG_ACCEL_GET_NORM_DATA;
	struct tUsbTxDbgAccelStData				DBG_ACCEL_GET_ST_DATA;
	struct tUsbTxDbgGpsLoadAid				DBG_GPS_LOAD_AID;
	struct tUsbTxTaskList					DBG_TASK_LIST;
	
	unsigned char							raw[FLASH_PAGE_SIZE];
};

struct __attribute__ ((packed)) tUsbTxFrame {
	unsigned char command;
	unsigned short msgLength;
	union tUsbTxMessages message;
};












/////////////////////////////
// USB Rx Messages         //
/////////////////////////////
struct __attribute__ ((packed)) tUsbRxCmdReqApplVer {
};

struct __attribute__ ((packed)) tUsbRxCmdReqHwVer {
};

struct __attribute__ ((packed)) tUsbRxCmdReqSerialNumber {
};

struct __attribute__ ((packed)) tUsbRxCmdReqTesterId {
};

struct __attribute__ ((packed)) tUsbRxCmdReqBatteryVoltage {
};

struct __attribute__ ((packed)) tUsbRxCmdReqBatteryTemperature {
};

struct __attribute__ ((packed)) tUsbRxCmdReqBatteryInstant{
};

struct __attribute__ ((packed)) tUsbRxCmdReqBatteryAccumulated{
};

struct __attribute__ ((packed)) tUsbRxCmdReqBatteryUpdate{
};

struct __attribute__ ((packed)) tUsbRxCmdWriteBatteryInfo {
};

struct __attribute__ ((packed)) tUsbRxCmdReadSavedTracks {
	unsigned short index;
};

struct __attribute__ ((packed)) tUsbRxCmdReadRecordTable {
	unsigned char length;
	unsigned char index;
};

struct __attribute__ ((packed)) tUsbRxCmdReadRecordData {
	unsigned short length;
	unsigned short index;
};

struct __attribute__ ((packed)) tUsbRxCmdEraseRecordData {
};

struct __attribute__ ((packed)) tUsbRxCmdWriteUserPrefs {
	unsigned char	screenPWMMax;
	unsigned char	screenPWMMin;
	unsigned short	screenFadeTime;
	unsigned short	screenOffTime;
	unsigned short  crc;
};

struct __attribute__ ((packed)) tUsbRxCmdWriteSavedTracks {
	unsigned char name[TRACKLIST_MAX_STRLEN];
	
	signed int longitude;
	signed int latitude;
	
	unsigned short heading;
	unsigned char isEmpty;
	unsigned char reserved;
};

struct __attribute__ ((packed)) tUsbRxCmdReadOtp {
	unsigned char length;
	unsigned char index;
};

struct __attribute__ ((packed)) tUsbRxCmdWriteOtp {
	unsigned char serial[OTP_SERIAL_LENGTH];
	unsigned char pcb_rev;
	unsigned char tester_id;
	unsigned char reserved;
	unsigned short crc;
};

struct __attribute__ ((packed)) tUsbRxDbgReadAdcValues {
};

struct __attribute__ ((packed)) tUsbRxDbgFlashSectorErase {
	unsigned char index;
};

struct __attribute__ ((packed)) tUsbRxDbgFlashBusy {
};

struct __attribute__ ((packed)) tUsbRxDbgChipErase {
};

struct __attribute__ ((packed)) tUsbRxDbgFlashFull {
};

struct __attribute__ ((packed)) tUsbRxDbgFlashUsedSpace {
};

struct __attribute__ ((packed)) tUsbRxDbgArbRead {
	unsigned short length;
	unsigned short index;
};

struct __attribute__ ((packed)) tUsbRxDbgArbWrite {
	unsigned short index;
	unsigned char data[FLASH_PAGE_SIZE];
};

struct __attribute__ ((packed)) tUsbRxDbgGpsCurrentPosition {
};

struct __attribute__ ((packed)) tUsbRxDbgGpsCurrentMode {
};

struct __attribute__ ((packed)) tUsbRxDbgGpsInfoSerialNo {
};

struct __attribute__ ((packed)) tUsbRxDbgGpsInfoPartNo {
};

struct __attribute__ ((packed)) tUsbRxDbgGpsInfoSwVer {
};

struct __attribute__ ((packed)) tUsbRxDbgGpsInfoSwDate {
};

struct __attribute__ ((packed)) tUsbRxDbgGpsLastResponse {
};

struct __attribute__ ((packed)) tUsbRxDbgStartRecording {
};

struct __attribute__ ((packed)) tUsbRxDbgStopRecording {
};

struct __attribute__ ((packed)) tUsbRxDbgRecordingStatus {
};

struct __attribute__ ((packed)) tUsbRxDbgAccelGetStatus {
};

struct __attribute__ ((packed)) tUsbRxDbgAccelFiltData {
};

struct __attribute__ ((packed)) tUsbRxDbgAccelNormData {
};

struct __attribute__ ((packed)) tUsbRxDbgAccelStData {
};

struct __attribute__ ((packed)) tUsbRxDbgGpsLoadAid {
};

struct __attribute__ ((packed)) tUsbRxTaskList {
};


union tUsbRxMessages{
	struct tUsbRxCmdReqApplVer				CMD_REQ_APPL_VER;
	struct tUsbRxCmdReqHwVer				CMD_REQ_HARDWARE_VER;
	struct tUsbRxCmdReqSerialNumber			CMD_REQ_SERIAL_NUMBER;
	struct tUsbRxCmdReqTesterId				CMD_REQ_TESTER_ID;
	struct tUsbRxCmdReqBatteryVoltage		CMD_REQ_BATTERY_VOLTAGE;
	struct tUsbRxCmdReqBatteryTemperature	CMD_REQ_BATTERY_TEMPERATURE;
	struct tUsbRxCmdReqBatteryInstant		CMD_REQ_BATTERY_INSTANT;
	struct tUsbRxCmdReqBatteryAccumulated	CMD_REQ_BATTERY_ACCUMULATED;
	struct tUsbRxCmdReqBatteryUpdate		CMD_REQ_BATTERY_UPDATE;
	struct tUsbRxCmdWriteBatteryInfo		CMD_REQ_WRITE_BATTERY_INFO;
	struct tUsbRxCmdReadSavedTracks			CMD_READ_SAVED_TRACKS;
	struct tUsbRxCmdReadRecordTable			CMD_READ_RECORD_TABLE;
	struct tUsbRxCmdReadRecordData			CMD_READ_RECORD_DATA;
	struct tUsbRxCmdEraseRecordData			CMD_ERASE_RECORD_DATA;
	struct tUsbRxCmdWriteUserPrefs			CMD_WRITE_USER_PREFS;
	struct tUsbRxCmdWriteSavedTracks		CMD_WRITE_SAVED_TRACKS;
	struct tUsbRxCmdReadOtp					CMD_READ_OTP;
	struct tUsbRxCmdWriteOtp				CMD_WRITE_OTP;
	struct tUsbRxDbgReadAdcValues			DBG_READ_ADC_VALUES;
	struct tUsbRxDbgFlashSectorErase		DBG_FLASH_SECTOR_ERASE;
	struct tUsbRxDbgFlashBusy				DBG_FLASH_BUSY;
	struct tUsbRxDbgChipErase				DBG_FLASH_CHIP_ERASE;
	struct tUsbRxDbgFlashFull				DBG_FLASH_IS_FULL;
	struct tUsbRxDbgFlashUsedSpace			DBG_FLASH_USED_SPACE;
	struct tUsbRxDbgArbRead					DBG_FLASH_ARB_READ;
	struct tUsbRxDbgArbWrite				DBG_FLASH_ARB_WRITE;
	struct tUsbRxDbgGpsCurrentPosition		DBG_GPS_CURRENT_POSITION;
	struct tUsbRxDbgGpsCurrentMode			DBG_GPS_CURRENT_MODE;
	struct tUsbRxDbgGpsInfoSerialNo			DBG_GPS_SERIAL_NUMBER;
	struct tUsbRxDbgGpsInfoPartNo			DBG_GPS_PART_NUMBER;
	struct tUsbRxDbgGpsInfoSwVer			DBG_GPS_SW_VER;
	struct tUsbRxDbgGpsInfoSwDate			DBG_GPS_SW_DATE;
	struct tUsbRxDbgGpsLastResponse			DBG_GPS_LAST_RESPONSE;
	struct tUsbRxDbgStartRecording			DBG_GPS_START_RECORDING;
	struct tUsbRxDbgStopRecording			DBG_GPS_STOP_RECORDING;
	struct tUsbRxDbgRecordingStatus			DBG_GPS_RECORDING_STATUS;
	struct tUsbRxDbgAccelGetStatus			DBG_ACCEL_GET_STATUS;
	struct tUsbRxDbgAccelFiltData			DBG_ACCEL_GET_FILT_DATA;
	struct tUsbRxDbgAccelNormData			DBG_ACCEL_GET_NORM_DATA;
	struct tUsbRxDbgAccelStData				DBG_ACCEL_GET_ST_DATA;
	struct tUsbRxDbgGpsLoadAid				DBG_GPS_LOAD_AID;
	struct tUsbRxTaskList					DBG_TASK_LIST;
	
	unsigned char							raw[FLASH_PAGE_SIZE];
};

struct __attribute__ ((packed)) tUsbRxFrame {
	unsigned char command;
	unsigned short msgLength;
	union tUsbRxMessages message;
};




#endif /* USB_COMMANDS_H_ */