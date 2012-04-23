/******************************************************************************
 *
 * Memory Interface
 *
 * - Compiler:          GNU GCC for AVR32
 * - Supported devices: traq|paq hardware version 1.2
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

#include <asf.h>
#include "hal.h"

//--------------------------
// Queues
//--------------------------
xQueueHandle flashManagerQueue;

//--------------------------
// Structs
//--------------------------
struct tFlashOTP		flashOTP;
struct tFlashRequest	request;
struct tUserPrefs		userPrefs;
struct tFlashFlags		flashFlags;


void flash_task_init( void ){
	unsigned char i;
	
	flash_set_busy_flag();
	flash_clr_full_flag();
	
	flashManagerQueue = xQueueCreate(FLASH_MANAGER_QUEUE_SIZE, sizeof(request));
	
	flash_clr_wp();
	flash_clr_hold();
	
	// Check the dataflash device ID
	if( !flash_checkID() ){
		debug_log(DEBUG_PRIORITY_WARNING, DEBUG_SENDER_FLASH, "Incorrect Device ID");
	}
	
	// Read out the OTP registers
	flash_ReadOTP(OTP_START_INDEX, OTP_LENGTH, &flashOTP);
	
	// Check validity of OTP
	if( flash_calculate_otp_crc() != flashOTP.crc ){
		debug_log(DEBUG_PRIORITY_WARNING, DEBUG_SENDER_FLASH, "Invalid OTP CRC");
	}
	
	// Load user preferences!
	flash_ReadToBuffer(FLASH_ADDR_USERPREFS_START, sizeof(userPrefs), &userPrefs);
	if(flash_calculate_userPrefs_crc() != userPrefs.crc){
		// If CRC is bad, load defaults!
		debug_log(DEBUG_PRIORITY_WARNING, DEBUG_SENDER_FLASH, "Invalid User Prefs CRC");
		userPrefs.screenPWMMax = BACKLIGHT_DEFAULT_MAX;
		userPrefs.screenPWMMin = BACKLIGHT_DEFAULT_MIN;
		userPrefs.screenFadeTime = BACKLIGHT_DEFAULT_FADETIME;
		userPrefs.screenOffTime = BACKLIGHT_DEFAULT_OFFTIME;
	}
	
	// Finally schedule the dataflash task
	xTaskCreate(flash_task, configTSK_DATAFLASH_TASK_NAME, configTSK_DATAFLASH_TASK_STACK_SIZE, NULL, configTSK_DATAFLASH_TASK_PRIORITY, configTSK_DATAFLASH_TASK_HANDLE);
}

void flash_task( void *pvParameters ){
	unsigned char recordTableIndex = 0;	// Index of first empty record table
	unsigned char trackCount = 0;		// Count the number of tracks
	struct tRecordsEntry recordTable, prevRecordTable;
	struct tTracklist trackList;

	unsigned char flashIsFull = FALSE;
	
	debug_log(DEBUG_PRIORITY_INFO, DEBUG_SENDER_FLASH, "Task Started");
	
	flash_GlobalUnprotect();
	flash_WriteEnable();
	
	//--------------------------
	// Record Table Initialization
	//--------------------------
	while( recordTableIndex < RECORDS_TOTAL_POSSIBLE ){
		flash_ReadToBuffer(FLASH_ADDR_RECORDTABLE_START + (sizeof(recordTable) * recordTableIndex), sizeof(recordTable), &recordTable);
		if(recordTable.recordEmpty) break;
		recordTableIndex++;
	}
	
	// Figure out the start address of the new record table entry
	if(recordTableIndex == 0){
		// There is no previous record table entry
		recordTable.startAddress = FLASH_ADDR_RECORDDATA_START;
		recordTable.endAddress = FLASH_ADDR_RECORDDATA_START;
	}else{
		// Peek at the last available record
		flash_ReadToBuffer(FLASH_ADDR_RECORDTABLE_START + (sizeof(prevRecordTable) * (recordTableIndex - 1)), sizeof(prevRecordTable), &prevRecordTable);
		recordTable.startAddress = (prevRecordTable.endAddress + 1);
		recordTable.endAddress = recordTable.startAddress;
	}
	
	//--------------------------
	// Track Initialization
	//--------------------------
	trackCount = 0;
	
	/*while( trackCount < TRACKLIST_TOTAL_NUM ){
		flash_ReadToBuffer(FLASH_ADDR_TRACKLIST_START + (sizeof(trackList) * trackCount), sizeof(trackList), &trackList);
		if(trackList.isEmpty) break;
		trackCount++;
	}*/
	
	flash_set_wp();
	flash_set_hold();
	flash_clr_busy_flag();
	
	debug_log(DEBUG_PRIORITY_INFO, DEBUG_SENDER_FLASH, "Ready for requests");
	
	while(TRUE){
		xQueueReceive(flashManagerQueue, &request, portMAX_DELAY);
		
		flash_set_busy_flag();
		flash_clr_wp();
		flash_clr_hold();
		
		debug_log(DEBUG_PRIORITY_INFO, DEBUG_SENDER_FLASH, "Processing Request");
		
		switch(request.command){
			case(FLASH_REQUEST_END_CURRENT_RECORD):
				
				recordTable.recordEmpty = FALSE;
				flash_UpdateSector(FLASH_ADDR_RECORDTABLE_START + (sizeof(recordTable) * recordTableIndex), sizeof(recordTable), &recordTable);
				
				// Get the new record table ready!
				recordTableIndex++;
				recordTable.recordEmpty = TRUE;
				recordTable.startAddress = recordTable.endAddress; 
				recordTable.trackID = 0xFF;
					
				break;
				
			case(FLASH_REQUEST_ADD_RECORDDATA):
				if(recordTable.endAddress < FLASH_ADDR_RECORDDATA_END){
					flash_WriteFromBuffer(recordTable.endAddress, request.length, request.pointer);
					recordTable.endAddress += FLASH_PAGE_SIZE;
				}				
				break;
				
			case(FLASH_REQUEST_ERASE_RECORD):
				 // Not implemented yet!
				break;
				
			case(FLASH_REQUEST_READ_RECORDTABLE):
				flash_ReadToBuffer(FLASH_ADDR_RECORDTABLE_START + ( request.index * sizeof(recordTable) ), sizeof(recordTable), request.pointer);
				break;
				
			case(FLASH_REQUEST_READ_RECORDDATA):
				flash_ReadToBuffer(FLASH_ADDR_RECORDDATA_START + (request.index * FLASH_PAGE_SIZE), FLASH_PAGE_SIZE, request.pointer);
				break;
				
			case(FLASH_REQUEST_READ_TRACK):
				flash_ReadToBuffer(FLASH_ADDR_TRACKLIST_START + (request.index * sizeof(trackList)), sizeof(trackList), request.pointer);
				break;
				
			case(FLASH_REQUEST_ADD_TRACK):
				flash_UpdateSector(FLASH_ADDR_TRACKLIST_START + (trackCount * sizeof(trackList)), sizeof(trackList), request.pointer);
				trackCount++;
				break;
				
			case(FLASH_REQUEST_ERASE_TRACKS):
				flash_eraseTracks();
				trackCount = 0;
				break;
				
			case(FLASH_REQUEST_READ_OTP):
				flash_ReadOTP(request.index, request.length, request.pointer);
				break;
				
			case(FLASH_REQUEST_SECTOR_ERASE):
				flash_eraseBlock(FLASH_CMD_BLOCK_ERASE_4KB, (request.index * FLASH_4KB) );
				break;
				
			case(FLASH_REQUEST_BUSY):
				*(request.pointer) = flash_is_busy();
				break;
				
			case(FLASH_REQUEST_CHIP_ERASE):
				*(request.pointer) = flash_chipErase();
				recordTableIndex = 0;
				recordTable.startAddress = FLASH_ADDR_RECORDDATA_START;
				recordTable.endAddress = FLASH_ADDR_RECORDDATA_START;
				break;
				
			case(FLASH_REQUEST_IS_FLASH_FULL):
				*(request.pointer) = flashIsFull;
				break;
				
			case(FLASH_REQUEST_USED_SPACE):				
				*(request.pointer) = (((recordTable.endAddress - FLASH_ADDR_RECORDDATA_START) * 100) / (FLASH_ADDR_RECORDDATA_END - FLASH_ADDR_RECORDDATA_START));
				break;
				
			case(FLASH_REQUEST_ERASE_RECORDED_DATA):
				recordTableIndex = 0;
				recordTable.startAddress = FLASH_ADDR_RECORDDATA_START;
				recordTable.endAddress = FLASH_ADDR_RECORDDATA_START;

				flash_eraseRecordedData();
				break;
				
			case(FLASH_REQUEST_WRITE_USER_PREFS):
				userPrefs.crc = flash_calculate_userPrefs_crc();
				flash_UpdateSector(FLASH_ADDR_USERPREFS_START, sizeof(userPrefs), &userPrefs);
				break;
			
			case(FLASH_REQUEST_SET_TRACK):
				recordTable.trackID = (unsigned char)request.index;
				break;
				
			case(FLASH_REQUEST_SET_DATESTAMP):
				recordTable.datestamp = request.index;
				break;
				
			case(FLASH_REQUEST_SHUTDOWN):
				if(recordTable.startAddress != recordTable.endAddress){
					// Need to close current record
					recordTable.recordEmpty = FALSE;
					flash_UpdateSector(FLASH_ADDR_RECORDTABLE_START + (sizeof(recordTable) * recordTableIndex), sizeof(recordTable), &recordTable);
				}
				debug_log(DEBUG_PRIORITY_INFO, DEBUG_SENDER_FLASH, "Task shut down");
				wdt_send_request(WDT_REQUEST_DATAFLASH_SHUTDOWN_COMPLETE, NULL);
				vTaskSuspend(NULL);
				break;
		}
		
		flash_clr_wp();
		flash_clr_hold();
		flash_clr_busy_flag();
		
		// Resume requesting task if it has been suspended
		if(request.resume == TRUE){
			vTaskResume(request.handle);
			debug_log(DEBUG_PRIORITY_INFO, DEBUG_SENDER_FLASH, "Requesting task resumed");
		}
		debug_log(DEBUG_PRIORITY_INFO, DEBUG_SENDER_FLASH, "Completed Request");
		
	}
}


unsigned char flash_checkID(void){
	unsigned short spiResponse[3];
		
	spi_selectChip(DATAFLASH_SPI, DATAFLASH_SPI_NPCS);
	spi_write(DATAFLASH_SPI, DATAFLASH_CMD_READ_DEVICE_ID);
	
	spi_write(DATAFLASH_SPI, DATAFLASH_CMD_DUMMY);
	spi_read(DATAFLASH_SPI, &spiResponse[0]);
	
	spi_write(DATAFLASH_SPI, DATAFLASH_CMD_DUMMY);
	spi_read(DATAFLASH_SPI, &spiResponse[1]);
	
	spi_write(DATAFLASH_SPI, DATAFLASH_CMD_DUMMY);
	spi_read(DATAFLASH_SPI, &spiResponse[2]);

	spi_unselectChip(DATAFLASH_SPI, DATAFLASH_SPI_NPCS);
	
	if( (spiResponse[0] == DATAFLASH_MANUFACTURER_ID) & (spiResponse[1] == DATAFLASH_DEVICE_ID0) & (spiResponse[2] == DATAFLASH_DEVICE_ID1) ){
		return DATAFLASH_RESPONSE_OK;
	}else{
		return DATAFLASH_RESPONSE_FAILURE;
	}
}


union tDataflashStatus flash_readStatus(void){
	unsigned short spiResponse[2];
	union tDataflashStatus result;

	spi_selectChip(DATAFLASH_SPI, DATAFLASH_SPI_NPCS);
	spi_write(DATAFLASH_SPI, DATAFLASH_CMD_READ_STATUS);
	
	spi_write(DATAFLASH_SPI, DATAFLASH_CMD_DUMMY);
	spi_read(DATAFLASH_SPI, &spiResponse[0]);
	
	spi_write(DATAFLASH_SPI, DATAFLASH_CMD_DUMMY);
	spi_read(DATAFLASH_SPI, &spiResponse[1]);
	
	spi_unselectChip(DATAFLASH_SPI, DATAFLASH_SPI_NPCS);
	
	result.raw[0] = spiResponse[0];
	result.raw[1] = spiResponse[1];
	
	return result;
}


unsigned char flash_GlobalUnprotect(void){
	flash_WriteEnable();
	
	spi_selectChip(DATAFLASH_SPI, DATAFLASH_SPI_NPCS);
	spi_write(DATAFLASH_SPI, DATAFLASH_CMD_WRITE_STATUS1);
	spi_write(DATAFLASH_SPI, DATAFLASH_STATUS_GLOBAL_UNPROTECT);
	spi_unselectChip(DATAFLASH_SPI, DATAFLASH_SPI_NPCS);
	
	return DATAFLASH_RESPONSE_OK;
}


unsigned char flash_WriteEnable(void){
	spi_selectChip(DATAFLASH_SPI, DATAFLASH_SPI_NPCS);
	spi_write(DATAFLASH_SPI, DATAFLASH_CMD_WRITE_ENABLE);
	spi_unselectChip(DATAFLASH_SPI, DATAFLASH_SPI_NPCS);
	
	return DATAFLASH_RESPONSE_OK;
}


unsigned char flash_WriteDisable(void){
	spi_selectChip(DATAFLASH_SPI, DATAFLASH_SPI_NPCS);
	spi_write(DATAFLASH_SPI, DATAFLASH_CMD_WRITE_DISABLE);
	spi_unselectChip(DATAFLASH_SPI, DATAFLASH_SPI_NPCS);
	
	return DATAFLASH_RESPONSE_OK;
}


unsigned char flash_UpdateSector(unsigned long startAddress, unsigned short length, unsigned char *bufferPointer){
	unsigned char sectorBuffer[FLASH_4KB];
	unsigned long sectorAddress = startAddress & 0xFFFFF000; // Mask off address bits A11 through A0 for 4KB sector address
	unsigned short offset = startAddress & 0x0FFF;
	unsigned short i;
	
	union tDataflashStatus status;
	
	flash_ReadToBuffer(sectorAddress, FLASH_4KB, &sectorBuffer);
	
	// Erase the sector
	if( flash_eraseBlock(FLASH_CMD_BLOCK_ERASE_4KB, sectorAddress) == DATAFLASH_RESPONSE_FAILURE ){
		debug_log(DEBUG_PRIORITY_WARNING, DEBUG_SENDER_FLASH, "Erase Failed");
	}
	
	// Copy new data to buffer
	for(i = 0; i < length; i++){
		sectorBuffer[ i + offset ] = bufferPointer[i];
	}
	
	// Write new data back
	for(i = 0; i < FLASH_4KB; i += FLASH_PAGE_SIZE){
		if( flash_WriteFromBuffer(sectorAddress + i, FLASH_PAGE_SIZE, &(sectorBuffer[i])) == DATAFLASH_RESPONSE_FAILURE ){
			debug_log(DEBUG_PRIORITY_WARNING, DEBUG_SENDER_FLASH, "Write Failed");
		}
	}	
	
	return TRUE;
}


unsigned char flash_eraseTracks(){
	unsigned short i;
	unsigned char userPrefsBuffer[FLASH_ADDR_USERPREFS_END - FLASH_ADDR_USERPREFS_START + 1];
	
	// Copy the user prefs to buffer
	flash_ReadToBuffer(FLASH_ADDR_USERPREFS_START, FLASH_ADDR_USERPREFS_END - FLASH_ADDR_USERPREFS_START + 1, &userPrefsBuffer);
	
	// Erase the sector
	if( flash_eraseBlock(FLASH_CMD_BLOCK_ERASE_4KB, FLASH_ADDR_USERPREFS_START) == DATAFLASH_RESPONSE_FAILURE ){
		debug_log(DEBUG_PRIORITY_WARNING, DEBUG_SENDER_FLASH, "Erase Failed");
	}
	
	for(i = FLASH_ADDR_USERPREFS_START; i < FLASH_ADDR_USERPREFS_END; i += FLASH_PAGE_SIZE){
		if( flash_WriteFromBuffer(i, FLASH_PAGE_SIZE, &(userPrefsBuffer[i])) == DATAFLASH_RESPONSE_FAILURE ){
			debug_log(DEBUG_PRIORITY_WARNING, DEBUG_SENDER_FLASH, "Write Failed");
		}
	}
	
	return DATAFLASH_RESPONSE_OK;
}

unsigned char flash_eraseRecordedData(){
	unsigned short i;
	unsigned char sectorBuffer[FLASH_4KB];
	union tDataflashStatus status;
	
	flash_ReadToBuffer(FLASH_ADDR_USERPREFS_START, FLASH_4KB, &sectorBuffer);
	
	// Erase the flash
	if( flash_chipErase() == DATAFLASH_RESPONSE_FAILURE){
		debug_log(DEBUG_PRIORITY_WARNING, DEBUG_SENDER_FLASH, "Erase Failed");
	}

	// Write new data back
	for(i = 0; i < FLASH_4KB; i += FLASH_PAGE_SIZE){
		if( flash_WriteFromBuffer(FLASH_ADDR_USERPREFS_START + i, FLASH_PAGE_SIZE, &(sectorBuffer[i])) == DATAFLASH_RESPONSE_FAILURE ){
			debug_log(DEBUG_PRIORITY_WARNING, DEBUG_SENDER_FLASH, "Write Failed");
		}
	}	
	
	return TRUE;
}

unsigned char flash_ReadToBuffer(unsigned long startAddress, unsigned short length, unsigned char *bufferPointer){
	unsigned short dummyData;

	spi_selectChip(DATAFLASH_SPI, DATAFLASH_SPI_NPCS);
	
	spi_write(DATAFLASH_SPI, DATAFLASH_CMD_READ_ARRAY);
	spi_write(DATAFLASH_SPI, (startAddress >> 16) & 0xFF);
	spi_write(DATAFLASH_SPI, (startAddress >>  8) & 0xFF);
	spi_write(DATAFLASH_SPI, (startAddress & 0xFF) );
	spi_read(DATAFLASH_SPI, &dummyData);	// Dummy read required to clear the SPI->RDR register before enabling PDCA
	
	while( !spi_writeEndCheck(DATAFLASH_SPI) );
	
	pdca_load_channel(SPI_RX_PDCA_CHANNEL, bufferPointer, length);
	pdca_load_channel(SPI_TX_PDCA_CHANNEL, (void *)0x80000000, length); // Use start of Flash as Dummy Bytes to Clock Out
	
	pdca_enable(SPI_RX_PDCA_CHANNEL);
	pdca_enable(SPI_TX_PDCA_CHANNEL);
	
	while(pdca_get_transfer_status(SPI_TX_PDCA_CHANNEL) & PDCA_TRANSFER_COMPLETE){
		vTaskDelay( (portTickType)TASK_DELAY_MS( DATAFLASH_PDCA_CHECK_TIME ) );
	}
	
	spi_unselectChip(DATAFLASH_SPI, DATAFLASH_SPI_NPCS);
	
	return DATAFLASH_RESPONSE_OK;
}

unsigned char flash_WriteFromBuffer(unsigned long startAddress, unsigned short length, unsigned char *bufferPointer){
	unsigned char i;
	
	while( flash_is_busy() ){
		vTaskDelay( (portTickType)TASK_DELAY_MS( DATAFLASH_PROGRAM_TIME ) );
	}
	
	flash_WriteEnable();
	
	spi_selectChip(DATAFLASH_SPI, DATAFLASH_SPI_NPCS);
	
	spi_write(DATAFLASH_SPI, DATAFLASH_CMD_PAGE_PROGRAM);
	spi_write(DATAFLASH_SPI, (startAddress >> 16) & 0xFF);
	spi_write(DATAFLASH_SPI, (startAddress >>  8) & 0xFF);
	spi_write(DATAFLASH_SPI, (startAddress & 0xFF) );
	
	while( !spi_writeEndCheck(DATAFLASH_SPI) );
	
	pdca_load_channel(SPI_TX_PDCA_CHANNEL, bufferPointer, length);
	pdca_enable(SPI_TX_PDCA_CHANNEL);
	
	while(pdca_get_transfer_status(SPI_TX_PDCA_CHANNEL) & PDCA_TRANSFER_COMPLETE){
		vTaskDelay( (portTickType)TASK_DELAY_MS( DATAFLASH_PDCA_CHECK_TIME ) );
	}
	
	spi_unselectChip(DATAFLASH_SPI, DATAFLASH_SPI_NPCS);
	
	// Wait for dataflash to become ready again.
	vTaskDelay( (portTickType)TASK_DELAY_MS( DATAFLASH_PROGRAM_TIME ) );
	while( flash_is_busy() ){
		vTaskDelay( (portTickType)TASK_DELAY_MS( DATAFLASH_PROGRAM_TIME ) );
	}
		
	if(flash_operation_failed()){
		return DATAFLASH_RESPONSE_FAILURE;
	}
	
	return DATAFLASH_RESPONSE_OK;
}

unsigned char flash_ReadOTP(unsigned char startAddress, unsigned char length, unsigned char *bufferPointer){
	unsigned char i;
	unsigned short temp;
	
	spi_selectChip(DATAFLASH_SPI, DATAFLASH_SPI_NPCS);
	spi_write(DATAFLASH_SPI, DATAFLASH_CMD_READ_OTP);
	spi_write(DATAFLASH_SPI, 0x00);
	spi_write(DATAFLASH_SPI, 0x00);
	spi_write(DATAFLASH_SPI, startAddress);
	spi_write(DATAFLASH_SPI, DATAFLASH_CMD_DUMMY);
	spi_write(DATAFLASH_SPI, DATAFLASH_CMD_DUMMY);
	
	for(i = 0; i < length; i++){
		spi_write(DATAFLASH_SPI, DATAFLASH_CMD_DUMMY);
		spi_read(DATAFLASH_SPI, &temp);
		bufferPointer[i] = temp & 0xFF;
	}
	
	spi_unselectChip(DATAFLASH_SPI, DATAFLASH_SPI_NPCS);
	
	return DATAFLASH_RESPONSE_OK;
}

unsigned char flash_WriteOTP(unsigned char startAddress, unsigned char length, unsigned char *bufferPointer){
	unsigned char i;
	
	while( flash_is_busy() ){
		vTaskDelay( (portTickType)TASK_DELAY_MS( DATAFLASH_PROGRAM_TIME ) );
	}
	
	flash_WriteEnable();

	spi_selectChip(DATAFLASH_SPI, DATAFLASH_SPI_NPCS);
	spi_write(DATAFLASH_SPI, DATAFLASH_CMD_PROGRAM_OTP);
	spi_write(DATAFLASH_SPI, 0x00);
	spi_write(DATAFLASH_SPI, 0x00);
	spi_write(DATAFLASH_SPI, startAddress);
	
	for(i = 0; i < length; i++){
		spi_write(DATAFLASH_SPI, bufferPointer[i]);
	}
	
	spi_unselectChip(DATAFLASH_SPI, DATAFLASH_SPI_NPCS);
	
	return DATAFLASH_RESPONSE_OK;
}

unsigned char flash_eraseBlock(unsigned char blockSize, unsigned long startAddress){
	if( (blockSize == FLASH_CMD_BLOCK_ERASE_4KB) | (blockSize == DATAFLASH_CMD_BLOCK_ERASE_32KB) | (blockSize == DATAFLASH_CMD_BLOCK_ERASE_64KB) ){
		
		while( flash_is_busy() ){
			vTaskDelay( (portTickType)TASK_DELAY_MS( DATAFLASH_PROGRAM_TIME ) );
		}
		
		flash_WriteEnable();
		
		spi_selectChip(DATAFLASH_SPI, DATAFLASH_SPI_NPCS);
		
		spi_write(DATAFLASH_SPI, blockSize);
		spi_write(DATAFLASH_SPI, (startAddress >> 16) & 0xFF);
		spi_write(DATAFLASH_SPI, (startAddress >>  8) & 0xFF);
		spi_write(DATAFLASH_SPI, (startAddress >>  0) & 0xFF);
		
		spi_unselectChip(DATAFLASH_SPI, DATAFLASH_SPI_NPCS);
		
		// Wait for dataflash to become ready again.
		vTaskDelay( (portTickType)TASK_DELAY_MS( DATAFLASH_ERASE_TIME ) );
		while( flash_is_busy() ){
			vTaskDelay( (portTickType)TASK_DELAY_MS( DATAFLASH_ERASE_TIME ) );
		}
		
		if(flash_operation_failed()){
			return DATAFLASH_RESPONSE_FAILURE;
		}
		return DATAFLASH_RESPONSE_OK;
		
	}else{
		return DATAFLASH_RESPONSE_FAILURE;
		
	}
}

unsigned char flash_chipErase( void ){
	while( flash_is_busy() ){
		vTaskDelay( (portTickType)TASK_DELAY_MS( DATAFLASH_PROGRAM_TIME ) );
	}
	
	flash_WriteEnable();
	
	spi_selectChip(DATAFLASH_SPI, DATAFLASH_SPI_NPCS);
	spi_write(DATAFLASH_SPI, DATAFLASH_CMD_CHIP_ERASE);
	spi_unselectChip(DATAFLASH_SPI, DATAFLASH_SPI_NPCS);
	
	
	// Wait for dataflash to become ready again.
	vTaskDelay( (portTickType)TASK_DELAY_MS( DATAFLASH_ERASE_TIME ) );
	while( flash_is_busy() ){
		vTaskDelay( (portTickType)TASK_DELAY_MS( DATAFLASH_ERASE_TIME ) );
	}
		
	if(flash_operation_failed()){
		return DATAFLASH_RESPONSE_FAILURE;
	}
	return DATAFLASH_RESPONSE_OK;
}

unsigned char flash_powerDown( void ){
		while( flash_is_busy() ){
			vTaskDelay( (portTickType)TASK_DELAY_MS( DATAFLASH_PROGRAM_TIME ) );
		}
		
		spi_selectChip(DATAFLASH_SPI, DATAFLASH_SPI_NPCS);
		spi_write(DATAFLASH_SPI, DATAFLASH_CMD_DEEP_POWER_DOWN);
		spi_unselectChip(DATAFLASH_SPI, DATAFLASH_SPI_NPCS);
		
		return DATAFLASH_RESPONSE_OK;
}

unsigned char flash_wakeUp( void ){
	
		while( flash_is_busy() ){
			vTaskDelay( (portTickType)TASK_DELAY_MS( DATAFLASH_PROGRAM_TIME ) );
		}
		
		spi_selectChip(DATAFLASH_SPI, DATAFLASH_SPI_NPCS);
		spi_write(DATAFLASH_SPI, DATAFLASH_CMD_WAKEUP);
		spi_unselectChip(DATAFLASH_SPI, DATAFLASH_SPI_NPCS);
		
		return DATAFLASH_RESPONSE_OK;
}


unsigned char flash_is_busy( void ){
	union tDataflashStatus status;
	
	status = flash_readStatus();
	
	if( status.registers.BSY0 ){
		return TRUE;
	}else{
		return FALSE;
	}		
}

unsigned char flash_operation_failed( void ){
	union tDataflashStatus status;
	
	status = flash_readStatus();
	
	return status.registers.EPE;
}

unsigned short flash_calculate_otp_crc( void ){
	unsigned short crc = 0;
	unsigned char i;
	
	for(i = 0; i < OTP_SERIAL_LENGTH; i++){
		crc = update_crc_ccitt(crc, flashOTP.serial[i]);
	}
	
	crc = update_crc_ccitt(crc, flashOTP.pcb_rev);
	crc = update_crc_ccitt(crc, flashOTP.tester_id);
	crc = update_crc_ccitt(crc, flashOTP.reserved);
	
	return crc;
}

unsigned short flash_calculate_userPrefs_crc( void ){
	unsigned short crc = 0;

	crc = update_crc_ccitt(crc, userPrefs.screenPWMMax);
	crc = update_crc_ccitt(crc, userPrefs.screenPWMMin);
	crc = update_crc_ccitt(crc, userPrefs.screenFadeTime);
	crc = update_crc_ccitt(crc, userPrefs.screenOffTime);

	return crc;
}

unsigned char flash_send_request(unsigned char command, unsigned char *pointer, unsigned short length, unsigned int index, unsigned char resume, unsigned char delay){
	struct tFlashRequest request;
	
	request.command = command;
	request.pointer = pointer;
	request.length = length;
	request.index = index;
	request.resume = resume;
	
	if(resume == TRUE){
		request.handle = xTaskGetCurrentTaskHandle();
	}
	
	xQueueSend(flashManagerQueue, &request, delay);
	
	if(resume == TRUE){
		vTaskSuspend(NULL);
	}
	
	return TRUE;
}