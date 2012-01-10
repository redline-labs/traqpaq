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

void dataflash_task( void *pvParameters ){
	unsigned char checkID;
	
	if( !dataflash_checkID() ){
		// Dang!  We broke it!
		while(1);
	}
	
	dataflash_WriteEnable();
	dataflash_GlobalUnprotect();
	
	while(TRUE){
		asm("nop");
		vTaskDelay( (portTickType)TASK_DELAY_MS(1000) );
	}
}

unsigned char dataflash_checkID(void){
	unsigned char spiResponse[] = {DATAFLASH_CMD_READ_DEVICE_ID, 0, 0};
		
	spi_selectChip(DATAFLASH_SPI, DATAFLASH_SPI_NPCS);
	spi_write_packet(DATAFLASH_SPI, &spiResponse, 1);
	spi_read_packet(DATAFLASH_SPI, &spiResponse, sizeof(spiResponse));
	spi_unselectChip(DATAFLASH_SPI, DATAFLASH_SPI_NPCS);
	
	if( (spiResponse[0] == DATAFLASH_MANUFACTURER_ID) & (spiResponse[1] == DATAFLASH_DEVICE_ID0) & (spiResponse[2] == DATAFLASH_DEVICE_ID1) ){
		return DATAFLASH_RESPONSE_OK;
	}else{
		return DATAFLASH_RESPONSE_FAILURE;
	}
}



union tDataflashStatus dataflash_readStatus(void){
	unsigned char spiResponse[] = {DATAFLASH_CMD_READ_STATUS, 0};
	union tDataflashStatus result;

	spi_selectChip(DATAFLASH_SPI, DATAFLASH_SPI_NPCS);
	spi_write_packet(DATAFLASH_SPI, spiResponse, 1);
	spi_read_packet(DATAFLASH_SPI, spiResponse, sizeof(spiResponse));
	spi_unselectChip(DATAFLASH_SPI, DATAFLASH_SPI_NPCS);
	
	result.raw[0] = spiResponse[0];
	result.raw[1] = spiResponse[1];
	
	return result;
}


unsigned char dataflash_GlobalUnprotect(void){
	unsigned char spiResponse[] = {DATAFLASH_CMD_WRITE_STATUS1, DATAFLASH_STATUS_GLOBAL_UNPROTECT};
		
	spi_selectChip(DATAFLASH_SPI, DATAFLASH_SPI_NPCS);
	spi_write_packet(DATAFLASH_SPI, spiResponse, sizeof(spiResponse));
	spi_unselectChip(DATAFLASH_SPI, DATAFLASH_SPI_NPCS);
	
	return DATAFLASH_RESPONSE_OK;
}

unsigned char dataflash_WriteEnable(void){
	unsigned char spiResponse[] = {DATAFLASH_CMD_WRITE_ENABLE};
		
	spi_selectChip(DATAFLASH_SPI, DATAFLASH_SPI_NPCS);
	spi_write_packet(DATAFLASH_SPI, spiResponse, sizeof(spiResponse));
	spi_unselectChip(DATAFLASH_SPI, DATAFLASH_SPI_NPCS);
	
	return DATAFLASH_RESPONSE_OK;
}

unsigned char dataflash_WriteDisable(void){
	unsigned char spiResponse[] = {DATAFLASH_CMD_WRITE_DISABLE};
		
	spi_selectChip(DATAFLASH_SPI, DATAFLASH_SPI_NPCS);
	spi_write_packet(DATAFLASH_SPI, spiResponse, sizeof(spiResponse));
	spi_unselectChip(DATAFLASH_SPI, DATAFLASH_SPI_NPCS);
	
	return DATAFLASH_RESPONSE_OK;
}

unsigned char dataflash_ReadToBuffer(unsigned long startAddress, unsigned int length, unsigned char *bufferPointer){
	unsigned char spiResponse[] = {DATAFLASH_CMD_READ_ARRAY, startAddress >> 16, (startAddress >> 8) & 0xFF, startAddress & 0xFF};
	
	spi_selectChip(DATAFLASH_SPI, DATAFLASH_SPI_NPCS);
	spi_write_packet(DATAFLASH_SPI, spiResponse, sizeof(spiResponse));
	spi_read_packet(DATAFLASH_SPI, bufferPointer, length);
	spi_unselectChip(DATAFLASH_SPI, DATAFLASH_SPI_NPCS);
	
	return DATAFLASH_RESPONSE_OK;
}

unsigned char dataflash_WriteFromBuffer(unsigned long startAddress, unsigned int length, unsigned char *bufferPointer){
	unsigned char spiResponse[] = {DATAFLASH_CMD_PAGE_PROGRAM, (startAddress >> 16) & 0xFF, (startAddress >> 8) & 0xFF, startAddress & 0xFF};
	
	spi_selectChip(DATAFLASH_SPI, DATAFLASH_SPI_NPCS);
	spi_write_packet(DATAFLASH_SPI, spiResponse, sizeof(spiResponse));
	spi_write_packet(DATAFLASH_SPI, bufferPointer, length);
	spi_unselectChip(DATAFLASH_SPI, DATAFLASH_SPI_NPCS);
	
	return DATAFLASH_RESPONSE_OK;
}

unsigned char dataflash_ReadOTP(unsigned char startAddress, unsigned char length, unsigned char *bufferPointer){
	unsigned char spiResponse[] = {DATAFLASH_CMD_READ_OTP, 0x00, 0x00, startAddress, DATAFLASH_CMD_DUMMY, DATAFLASH_CMD_DUMMY};
	
	spi_selectChip(DATAFLASH_SPI, DATAFLASH_SPI_NPCS);
	spi_write_packet(DATAFLASH_SPI, spiResponse, sizeof(spiResponse));
	spi_read_packet(DATAFLASH_SPI, bufferPointer, length);
	spi_unselectChip(DATAFLASH_SPI, DATAFLASH_SPI_NPCS);
	
	return DATAFLASH_RESPONSE_OK;
}

unsigned char dataflash_WriteOTP(unsigned char startAddress, unsigned char length, unsigned char *bufferPointer){
	unsigned char spiResponse[] = {DATAFLASH_CMD_PROGRAM_OTP, 0x00, 0x00, startAddress};
	
	spi_selectChip(DATAFLASH_SPI, DATAFLASH_SPI_NPCS);
	spi_write_packet(DATAFLASH_SPI, spiResponse, sizeof(spiResponse));
	spi_write_packet(DATAFLASH_SPI, bufferPointer, length);
	spi_unselectChip(DATAFLASH_SPI, DATAFLASH_SPI_NPCS);
	
	return DATAFLASH_RESPONSE_OK;
}

unsigned char dataflash_eraseBlock(unsigned char blockSize, unsigned long startAddress){
	if( (blockSize == DATAFLASH_CMD_BLOCK_ERASE_4KB) | (blockSize == DATAFLASH_CMD_BLOCK_ERASE_32KB) | (blockSize == DATAFLASH_CMD_BLOCK_ERASE_64KB) ){
		unsigned char spiResponse[] = {blockSize, (startAddress >> 16) & 0xFF, (startAddress >> 8) & 0xFF, startAddress & 0xFF};
	
		spi_selectChip(DATAFLASH_SPI, DATAFLASH_SPI_NPCS);
		spi_write_packet(DATAFLASH_SPI, spiResponse, sizeof(spiResponse));
		spi_unselectChip(DATAFLASH_SPI, DATAFLASH_SPI_NPCS);
		
		return DATAFLASH_RESPONSE_OK;
		
	}else{
		return DATAFLASH_RESPONSE_FAILURE;
		
	}
}

unsigned char dataflash_chipErase(void){
		unsigned char spiResponse[] = {DATAFLASH_CMD_CHIP_ERASE};
	
		spi_selectChip(DATAFLASH_SPI, DATAFLASH_SPI_NPCS);
		spi_write_packet(DATAFLASH_SPI, spiResponse, sizeof(spiResponse));
		spi_unselectChip(DATAFLASH_SPI, DATAFLASH_SPI_NPCS);
		
		return DATAFLASH_RESPONSE_OK;
}

unsigned char dataflash_powerDown(void){
		unsigned char spiResponse[] = {DATAFLASH_CMD_DEEP_POWER_DOWN};
	
		spi_selectChip(DATAFLASH_SPI, DATAFLASH_SPI_NPCS);
		spi_write_packet(DATAFLASH_SPI, spiResponse, sizeof(spiResponse));
		spi_unselectChip(DATAFLASH_SPI, DATAFLASH_SPI_NPCS);
		
		return DATAFLASH_RESPONSE_OK;
}

unsigned char dataflash_wakeUp(void){
		unsigned char spiResponse[] = {DATAFLASH_CMD_WAKEUP};
	
		spi_selectChip(DATAFLASH_SPI, DATAFLASH_SPI_NPCS);
		spi_write_packet(DATAFLASH_SPI, spiResponse, sizeof(spiResponse));
		spi_unselectChip(DATAFLASH_SPI, DATAFLASH_SPI_NPCS);
		
		return DATAFLASH_RESPONSE_OK;
}