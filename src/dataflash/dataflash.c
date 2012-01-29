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
#include "dataflash_layout.h"

void dataflash_task_init( void ){
	xTaskCreate(dataflash_task, configTSK_DATAFLASH_TASK_NAME, configTSK_DATAFLASH_TASK_STACK_SIZE, NULL, configTSK_DATAFLASH_TASK_PRIORITY, NULL);
}

unsigned char readBuffer[256];

void dataflash_task( void *pvParameters ){
	unsigned short i;
	
	if( !dataflash_checkID() ){
		debug_log("WARNING [DATAFLASH]: Incorrect device ID");
	}
	
	dataflash_GlobalUnprotect();
	dataflash_WriteEnable();
	
	if( dataflash_is_busy() ){
		debug_log("WARNING [DATAFLASH]: Busy response received");
	}
	
	
	while(TRUE){
		vTaskSuspend(NULL);
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


unsigned char dataflash_ReadToBuffer(unsigned long startAddress, unsigned short length, unsigned char *bufferPointer){	
	
	pdca_load_channel(SPI_TX_PDCA_CHANNEL, (void *)0x80000000, length); // Use start of Flash as Dummy Bytes to Clock Out
	pdca_load_channel(SPI_RX_PDCA_CHANNEL, bufferPointer, length);
	spi_selectChip(DATAFLASH_SPI, DATAFLASH_SPI_NPCS);
	
	spi_write(DATAFLASH_SPI, DATAFLASH_CMD_READ_ARRAY);
	spi_write(DATAFLASH_SPI, (startAddress >> 16) & 0xFF);
	spi_write(DATAFLASH_SPI, (startAddress >>  8) & 0xFF);
	spi_write(DATAFLASH_SPI, (startAddress >>  0) & 0xFF);
	
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
	
	pdca_load_channel(SPI_TX_PDCA_CHANNEL, bufferPointer, length); // Use start of Flash as Dummy Bytes to Clock Out
	spi_selectChip(DATAFLASH_SPI, DATAFLASH_SPI_NPCS);
	
	spi_write(DATAFLASH_SPI, DATAFLASH_CMD_PAGE_PROGRAM);
	spi_write(DATAFLASH_SPI, (startAddress >> 16) & 0xFF);
	spi_write(DATAFLASH_SPI, (startAddress >>  8) & 0xFF);
	spi_write(DATAFLASH_SPI, (startAddress >>  0) & 0xFF);
	
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