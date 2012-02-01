/******************************************************************************
 *
 * Memory Interface
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

#include <asf.h>
#include "drivers.h"
#include "dataflash_otp_layout.h"
#include "dataflash_manager_request.h"
#include "usb_task.h"
#include "usb_descriptors.h"
#include "dataflash_layout.h"


// Struct for holding USB serial number
S_usb_serial_number module_serial_number;

// Struct for holding serial number, HW Version, and Tester ID;
struct tDataflashOTP dataflashOTP;

// Queue for receiving requests into the dataflash manager
xQueueHandle dataflashManagerQueue;

struct tDataflashRequest request;

void dataflash_task_init( void ){
	unsigned char i;
	
	dataflashManagerQueue = xQueueCreate(DFMAN_QUEUE_SIZE, sizeof(request));
	
	// Check the dataflash device ID
	if( !dataflash_checkID() ){
		debug_log("WARNING [DATAFLASH]: Incorrect device ID");
	}
	
	// Read out the OTP registers
	dataflash_ReadOTP(OTP_START_INDEX, OTP_LENGTH, &dataflashOTP);
	
	module_serial_number.bLength = sizeof(module_serial_number);
	module_serial_number.bDescriptorType = STRING_DESCRIPTOR;
	
	// Check validity of OTP
	if( dataflash_calculate_otp_crc() == dataflashOTP.crc ){
		// Copy OTP serial number to USB Descriptor
		for(i = 0; i < OTP_SERIAL_LENGTH; i++){
			module_serial_number.wstring[i] = Usb_unicode( dataflashOTP.serial[i] );
		}
		
	}else{	// CRC verification failed
		// Copy null serial number to USB descriptor
		for(i = 0; i < OTP_SERIAL_LENGTH; i++){
			module_serial_number.wstring[i] = Usb_unicode('0');
		}
		
	}
	
	// Finally schedule the dataflash task
	xTaskCreate(dataflash_task, configTSK_DATAFLASH_TASK_NAME, configTSK_DATAFLASH_TASK_STACK_SIZE, NULL, configTSK_DATAFLASH_TASK_PRIORITY, configTSK_DATAFLASH_TASK_HANDLE);
}

void dataflash_task( void *pvParameters ){
	unsigned short i;
	volatile unsigned char recordTableIndex = 0;	// Index of first empty record table
	struct tRecordsEntry recordTable, prevRecordTable;
	
	dataflash_GlobalUnprotect();
	dataflash_WriteEnable();
	
	if( dataflash_is_busy() ){
		debug_log("WARNING [DATAFLASH]: Busy response received");
	}
	
	// Find the first empty record
	//while( (recordTableIndex < RECORDS_TOTAL_POSSIBLE) && (recordTable.recordEmpty) ){
	//	dataflash_ReadToBuffer(DATAFLASH_ADDR_RECORDTABLE_START + (sizeof(recordTable) * recordTableIndex), sizeof(recordTable), &recordTable);
	//	recordTableIndex++;
	//}
	
	
	// Figure out the start address of the new record table
	//if(recordTableIndex == 0){
		// The record table is empty!
		recordTable.startAddress = DATAFLASH_ADDR_RECORDDATA_START;
		recordTable.endAddress = DATAFLASH_ADDR_RECORDDATA_START;
	//}else{
		// Peek at the last available record
	//	dataflash_ReadToBuffer(DATAFLASH_ADDR_RECORDTABLE_START + (sizeof(prevRecordTable) * (recordTableIndex - 1)), sizeof(prevRecordTable), &prevRecordTable);
	//	recordTable.startAddress = (prevRecordTable.endAddress + 1) & 0xFFFFFF00;	// Align to page
	//	recordTable.endAddress = recordTable.startAddress;
	//}
	
	while(TRUE){
		xQueueReceive(dataflashManagerQueue, &request, portMAX_DELAY);
		
		switch(request.command){
			case(DFMAN_REQUEST_UPDATE_RECORDTABLE):
				recordTable.recordEmpty = FALSE;
				recordTable.trackID = 0xAA;
				
				dataflash_UpdateSector(DATAFLASH_ADDR_RECORDTABLE_START + (sizeof(recordTable) * recordTableIndex), sizeof(recordTable), &recordTable);
				
				// Get the new record table ready!
				recordTableIndex++;
				recordTable.recordEmpty = TRUE;
				recordTable.startAddress = recordTable.endAddress; 
				recordTable.trackID = 0xFF;
				
				break;
				
			case(DFMAN_REQUEST_UPDATE_TRACKLIST):
				asm("nop");
				break;
				
			case(DFMAN_REQUEST_ADD_RECORDDATA):
				dataflash_WriteFromBuffer(recordTable.endAddress, request.length, request.pointer);
				recordTable.endAddress += DATAFLASH_PAGE_SIZE;
				break;
				
			case(DFMAN_REQUEST_ERASE_RECORD):
				asm("nop");
				break;
				
			case(DFMAN_REQUEST_READ_RECORDTABLE):
				//dataflash_ReadToBuffer(DATAFLASH_ADDR_RECORDTABLE_START + ((request.index / RECORDS_ENTRY_PER_PAGE) * DATAFLASH_PAGE_SIZE), request.length, request.pointer);
				dataflash_ReadToBuffer(DATAFLASH_ADDR_RECORDTABLE_START, request.length, request.pointer);
				break;
				
			case(DFMAN_REQUEST_READ_RECORDDATA):
				dataflash_ReadToBuffer(DATAFLASH_ADDR_RECORDDATA_START + (request.index * DATAFLASH_PAGE_SIZE), request.length, request.pointer);
				break;
				
			case(DFMAN_REQUEST_READ_OTP):
				dataflash_ReadOTP(request.index, request.length, request.pointer);
				break;
				
			case(DFMAN_REQUEST_SECTOR_ERASE):
				dataflash_eraseBlock(DATAFLASH_CMD_BLOCK_ERASE_4KB, (request.index * DATAFLASH_4KB) );
				break;
				
			case(DFMAN_REQUEST_BUSY):
				*(request.pointer) = dataflash_is_busy();
				break;
				
			case(DFMAN_REQUEST_CHIP_ERASE):
				*(request.pointer) = dataflash_chipErase();
				break;
		}
		
		// Resume task if it has been suspended
		if(request.resume != NULL){
			vTaskResume(request.resume);
		}
		
	}
}


unsigned char dataflash_checkID(void){
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


union tDataflashStatus dataflash_readStatus(void){
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


unsigned char dataflash_GlobalUnprotect(void){
	dataflash_WriteEnable();
	
	spi_selectChip(DATAFLASH_SPI, DATAFLASH_SPI_NPCS);
	spi_write(DATAFLASH_SPI, DATAFLASH_CMD_WRITE_STATUS1);
	spi_write(DATAFLASH_SPI, DATAFLASH_STATUS_GLOBAL_UNPROTECT);
	spi_unselectChip(DATAFLASH_SPI, DATAFLASH_SPI_NPCS);
	
	return DATAFLASH_RESPONSE_OK;
}


unsigned char dataflash_WriteEnable(void){
	spi_selectChip(DATAFLASH_SPI, DATAFLASH_SPI_NPCS);
	spi_write(DATAFLASH_SPI, DATAFLASH_CMD_WRITE_ENABLE);
	spi_unselectChip(DATAFLASH_SPI, DATAFLASH_SPI_NPCS);
	
	return DATAFLASH_RESPONSE_OK;
}


unsigned char dataflash_WriteDisable(void){
	spi_selectChip(DATAFLASH_SPI, DATAFLASH_SPI_NPCS);
	spi_write(DATAFLASH_SPI, DATAFLASH_CMD_WRITE_DISABLE);
	spi_unselectChip(DATAFLASH_SPI, DATAFLASH_SPI_NPCS);
	
	return DATAFLASH_RESPONSE_OK;
}


unsigned char dataflash_UpdateSector(unsigned long startAddress, unsigned short length, unsigned char *bufferPointer){
	unsigned char sectorBuffer[DATAFLASH_4KB];
	unsigned long sectorAddress = startAddress & 0xFFFFF000; // Mask off address bits A11 through A0 for 4KB sector address
	unsigned short offset = startAddress & 0x0FFF;
	unsigned short i;
	
	dataflash_ReadToBuffer(sectorAddress, DATAFLASH_4KB, &sectorBuffer);
	
	// Wait for dataflash to become ready again.
	while( dataflash_is_busy() ){
		vTaskDelay( (portTickType)TASK_DELAY_MS( DATAFLASH_STATUS_CHECK_TIME ) );
	}
	
	dataflash_eraseBlock(DATAFLASH_CMD_BLOCK_ERASE_4KB, sectorAddress);
	
	// Copy new data to buffer
	for(i = 0; i < length; i++){
		sectorBuffer[ i + offset ] = bufferPointer[i];
	}
	
	while( dataflash_is_busy() ){
		vTaskDelay( (portTickType)TASK_DELAY_MS( DATAFLASH_ERASE_TIME ) );
	}

	for(i = 0; i < DATAFLASH_4KB; i += DATAFLASH_PAGE_SIZE){
		dataflash_WriteFromBuffer(sectorAddress + i, DATAFLASH_PAGE_SIZE, &(sectorBuffer[i]));
		
		// Wait for dataflash to become ready again.
		while( dataflash_is_busy() ){
			vTaskDelay( (portTickType)TASK_DELAY_MS( DATAFLASH_PROGRAM_TIME ) );
		}
	}	
	
	return TRUE;
}

unsigned char dataflash_ReadToBuffer(unsigned long startAddress, unsigned short length, unsigned char *bufferPointer){
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

unsigned char dataflash_WriteFromBuffer(unsigned long startAddress, unsigned short length, unsigned char *bufferPointer){
	unsigned char i;
	
	dataflash_WriteEnable();
	
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
	
	return DATAFLASH_RESPONSE_OK;
}

unsigned char dataflash_ReadOTP(unsigned char startAddress, unsigned char length, unsigned char *bufferPointer){
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

unsigned char dataflash_WriteOTP(unsigned char startAddress, unsigned char length, unsigned char *bufferPointer){
	unsigned char i;
	
	dataflash_WriteEnable();

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

unsigned char dataflash_eraseBlock(unsigned char blockSize, unsigned long startAddress){
	if( (blockSize == DATAFLASH_CMD_BLOCK_ERASE_4KB) | (blockSize == DATAFLASH_CMD_BLOCK_ERASE_32KB) | (blockSize == DATAFLASH_CMD_BLOCK_ERASE_64KB) ){
		dataflash_WriteEnable();
		
		spi_selectChip(DATAFLASH_SPI, DATAFLASH_SPI_NPCS);
		
		spi_write(DATAFLASH_SPI, blockSize);
		spi_write(DATAFLASH_SPI, (startAddress >> 16) & 0xFF);
		spi_write(DATAFLASH_SPI, (startAddress >>  8) & 0xFF);
		spi_write(DATAFLASH_SPI, (startAddress >>  0) & 0xFF);
		
		spi_unselectChip(DATAFLASH_SPI, DATAFLASH_SPI_NPCS);
		
		return DATAFLASH_RESPONSE_OK;
		
	}else{
		return DATAFLASH_RESPONSE_FAILURE;
		
	}
}

unsigned char dataflash_chipErase( void ){
		dataflash_WriteEnable();
	
		spi_selectChip(DATAFLASH_SPI, DATAFLASH_SPI_NPCS);
		spi_write(DATAFLASH_SPI, DATAFLASH_CMD_CHIP_ERASE);
		spi_unselectChip(DATAFLASH_SPI, DATAFLASH_SPI_NPCS);
		
		return DATAFLASH_RESPONSE_OK;
}

unsigned char dataflash_powerDown( void ){
		
		spi_selectChip(DATAFLASH_SPI, DATAFLASH_SPI_NPCS);
		spi_write(DATAFLASH_SPI, DATAFLASH_CMD_DEEP_POWER_DOWN);
		spi_unselectChip(DATAFLASH_SPI, DATAFLASH_SPI_NPCS);
		
		return DATAFLASH_RESPONSE_OK;
}

unsigned char dataflash_wakeUp( void ){
		
		spi_selectChip(DATAFLASH_SPI, DATAFLASH_SPI_NPCS);
		spi_write(DATAFLASH_SPI, DATAFLASH_CMD_WAKEUP);
		spi_unselectChip(DATAFLASH_SPI, DATAFLASH_SPI_NPCS);
		
		return DATAFLASH_RESPONSE_OK;
}


unsigned char dataflash_is_busy( void ){
	union tDataflashStatus status;
	
	status = dataflash_readStatus();
	
	if( status.registers.BSY0 ){
		return TRUE;
	}else{
		return FALSE;
	}		
}

unsigned short dataflash_calculate_otp_crc( void ){
	unsigned short crc = 0;
	unsigned char i;
	
	for(i = 0; i < OTP_SERIAL_LENGTH; i++){
		crc = update_crc_ccitt(crc, dataflashOTP.serial[i]);
	}
	
	crc = update_crc_ccitt(crc, dataflashOTP.pcb_rev);
	crc = update_crc_ccitt(crc, dataflashOTP.tester_id);
	crc = update_crc_ccitt(crc, dataflashOTP.reserved);
	
	return crc;
}