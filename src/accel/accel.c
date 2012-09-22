/******************************************************************************
 *
 * Accelerometer Interface
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

#include <asf.h>
#include "hal.h"
#include "lcd/itoa.h"

//--------------------------
// Queues
//--------------------------
xQueueHandle accelManagerQueue;

//--------------------------
// Structs
//--------------------------
struct tAccelInfo accelInfo;

//--------------------------
// Functions
//--------------------------
void accel_task_init( void ){
	accelInfo.available = FALSE;
	accelInfo.status = UNKNOWN;
	
	flashManagerQueue = xQueueCreate(FLASH_MANAGER_QUEUE_SIZE, sizeof(request));
	
	// Make sure the accel is in standby
	accel_setPowerCtrl( ACCEL_MASK_POWER_CTL_STANDBY );
	
	// Check if the accelerometer is responding
	if( !accel_checkID() ){
		debug_log(DEBUG_PRIORITY_WARNING, DEBUG_SENDER_ACCEL, "Incorrect accel ID");
	}
	
	xTaskCreate(accel_task, configTSK_ACCEL_TASK_NAME, configTSK_ACCEL_TASK_STACK_SIZE, NULL, configTSK_ACCEL_TASK_PRIORITY, configTSK_ACCEL_TASK_HANDLE);
}

void accel_task( void *pvParameters ){
	struct tAccelDataPage accelData;
	struct tAccelSample sample;
	unsigned char i, entries;
	signed int temp_x, temp_y, temp_z;
	unsigned char tempString[10];
	unsigned recordFlag;
	
	debug_log(DEBUG_PRIORITY_INFO, DEBUG_SENDER_ACCEL, "Starting accel task...");
	
	recordFlag = 0;
	
	// Set the data format
	accel_setDataFormat(ACCEL_MASK_SELF_TEST_DISABLE	|
						ACCEL_MASK_SPI_4_WIRE			|
						ACCEL_MASK_INT_NORMAL			|
						ACCEL_MASK_RES_FULL				|
						ACCEL_MASK_JUSTIFY_RIGHT		|
						ACCEL_MASK_RANGE_16G			);
	
	// Set data rate
	accel_setDataRate( ACCEL_DATA_RATE_1600HZ );
	
	// Set up the FIFO
	accel_setFIFOCtrl( ACCEL_MASK_FIFO_MODE_FIFO );
	
	if( !accel_performInit( &accelInfo ) ){
		debug_log(DEBUG_PRIORITY_WARNING, DEBUG_SENDER_ACCEL, "Init failed");
	}
	
	if( !accelInfo.selfTestPassed ){
		debug_log(DEBUG_PRIORITY_WARNING, DEBUG_SENDER_ACCEL, "Self-test failed");
	}
	
	debug_log(DEBUG_PRIORITY_INFO, DEBUG_SENDER_ACCEL, "Starting measurements");
	accel_setPowerCtrl( ACCEL_MASK_POWER_CTL_MEASURE );
	
	
	
	while( TRUE ){
		temp_x = 0;
		temp_y = 0;
		temp_z = 0;
		
		vTaskDelay( (portTickType)TASK_DELAY_MS( ACCEL_READ_FIFO_INTERVAL ) );
		
		entries = accel_readEntriesFIFO();
		
		for(i = 0; i < entries; i++){
			accel_read(&sample);
			
			temp_x += sample.x;
			temp_y += sample.y;
			temp_z += sample.z;
		};
		
		// Update info with a semi-filtered reading
		accelInfo.filteredData.x = temp_x / entries;
		accelInfo.filteredData.y = temp_y / entries;
		accelInfo.filteredData.z = temp_z / entries;
	};
}


unsigned char accel_checkID( void ){
	unsigned short response;
	
	spi_selectChip(ACCEL_SPI, ACCEL_SPI_NPCS);
	spi_write(ACCEL_SPI, ACCEL_READ | ACCEL_SINGLE_BYTE | ACCEL_REGISTER_DEVICE_ID);
	
	spi_write(ACCEL_SPI, ACCEL_DUMMY_CMD);
	spi_read(ACCEL_SPI, &response);
	
	spi_unselectChip(ACCEL_SPI, ACCEL_SPI_NPCS);
	
	if( response == ACCEL_DEVICE_ID ){
		accelInfo.available = TRUE;
		accelInfo.status = UNINITALIZED;
		
		return ACCEL_RESPONSE_OK;
	}else{
		accelInfo.available = FALSE;
		accelInfo.status = INCORRECT_DEVID;
		
		return ACCEL_RESPONSE_ERROR;
	}
}


unsigned char accel_setDataFormat( unsigned char format ){
	spi_selectChip(ACCEL_SPI, ACCEL_SPI_NPCS);
	
	spi_write(ACCEL_SPI, ACCEL_WRITE | ACCEL_SINGLE_BYTE | ACCEL_REGISTER_DATA_FORMAT);
	spi_write(ACCEL_SPI, format);
	
	spi_unselectChip(ACCEL_SPI, ACCEL_SPI_NPCS);
	
	return ACCEL_RESPONSE_OK;
}

unsigned char accel_setDataRate( unsigned char rate ){
	spi_selectChip(ACCEL_SPI, ACCEL_SPI_NPCS);
	
	spi_write(ACCEL_SPI, ACCEL_WRITE | ACCEL_SINGLE_BYTE | ACCEL_REGISTER_BW_RATE);
	spi_write(ACCEL_SPI, rate);
	
	spi_unselectChip(ACCEL_SPI, ACCEL_SPI_NPCS);
	
	return ACCEL_RESPONSE_OK;
}


unsigned char accel_setFIFOCtrl( unsigned char control ){
	spi_selectChip(ACCEL_SPI, ACCEL_SPI_NPCS);
	
	spi_write(ACCEL_SPI, ACCEL_WRITE | ACCEL_SINGLE_BYTE | ACCEL_REGISTER_FIFO_CTL);
	spi_write(ACCEL_SPI, control);
	
	spi_unselectChip(ACCEL_SPI, ACCEL_SPI_NPCS);
	
	return ACCEL_RESPONSE_OK;
}

unsigned char accel_setPowerCtrl( unsigned char control ){
	spi_selectChip(ACCEL_SPI, ACCEL_SPI_NPCS);
	
	spi_write(ACCEL_SPI, ACCEL_WRITE | ACCEL_SINGLE_BYTE | ACCEL_REGISTER_POWER_CTL);
	spi_write(ACCEL_SPI, control);
	
	spi_unselectChip(ACCEL_SPI, ACCEL_SPI_NPCS);
	
	if( (control & ACCEL_MASK_POWER_CTL_MEASURE) && (accelInfo.status != PERFORMING_INIT) ){
		accelInfo.status = SAMPLING;
	}else{
		accelInfo.status = IDLE;
	}
	
	return ACCEL_RESPONSE_OK;
}

unsigned char accel_read( struct tAccelSample *sample ){
	unsigned short x1, x2, y1, y2, z1, z2;
	spi_selectChip(ACCEL_SPI, ACCEL_SPI_NPCS);
	
	spi_write(ACCEL_SPI, ACCEL_READ | ACCEL_MULTIPLE_BYTES | ACCEL_REGISTER_DATAX0);
	
	spi_write(ACCEL_SPI, ACCEL_DUMMY_CMD);
	spi_read(ACCEL_SPI, &x1);
	
	spi_write(ACCEL_SPI, ACCEL_DUMMY_CMD);
	spi_read(ACCEL_SPI, &x2);
	
	spi_write(ACCEL_SPI, ACCEL_DUMMY_CMD);
	spi_read(ACCEL_SPI, &y1);
	
	spi_write(ACCEL_SPI, ACCEL_DUMMY_CMD);
	spi_read(ACCEL_SPI, &y2);
	
	spi_write(ACCEL_SPI, ACCEL_DUMMY_CMD);
	spi_read(ACCEL_SPI, &z1);
	
	spi_write(ACCEL_SPI, ACCEL_DUMMY_CMD);
	spi_read(ACCEL_SPI, &z2);
	
	spi_unselectChip(ACCEL_SPI, ACCEL_SPI_NPCS);
	
	sample->x = (x2 << 8) + x1;
	sample->y = (y2 << 8) + y1;
	sample->z = (z2 << 8) + z1;
	
	return ACCEL_RESPONSE_OK;
}


unsigned char accel_readEntriesFIFO( void ){
	unsigned short entries;
	
	spi_selectChip(ACCEL_SPI, ACCEL_SPI_NPCS);
	spi_write(ACCEL_SPI, ACCEL_READ | ACCEL_SINGLE_BYTE | ACCEL_REGISTER_FIFO_STATUS);
	
	spi_write(ACCEL_SPI, ACCEL_DUMMY_CMD);
	spi_read(ACCEL_SPI, &entries);
	
	spi_unselectChip(ACCEL_SPI, ACCEL_SPI_NPCS);
	
	return (unsigned char)(entries & ACCEL_MASK_FIFO_ENTRIES);
}

unsigned char accel_selfTest( enum tAccelSelfTest flag ){
	unsigned char data_format;
	
	data_format = accel_readRegister( ACCEL_REGISTER_DATA_FORMAT );
	
	if( flag == ACCEL_SELF_TEST_ENABLE ){
		data_format |= ACCEL_MASK_SELF_TEST_ENABLE;
	}else{
		data_format &= ~ACCEL_MASK_SELF_TEST_ENABLE;
	}
	
	spi_selectChip(ACCEL_SPI, ACCEL_SPI_NPCS);
	spi_write(ACCEL_SPI, ACCEL_WRITE | ACCEL_SINGLE_BYTE | ACCEL_REGISTER_DATA_FORMAT);
	spi_write(ACCEL_SPI, data_format);
	spi_unselectChip(ACCEL_SPI, ACCEL_SPI_NPCS);
	
	return ACCEL_RESPONSE_OK;
};

unsigned char accel_readRegister( unsigned char registerAddr ){
	unsigned short response;
	
	spi_selectChip(ACCEL_SPI, ACCEL_SPI_NPCS);
	spi_write(ACCEL_SPI, ACCEL_READ | ACCEL_SINGLE_BYTE | registerAddr);
	
	spi_write(ACCEL_SPI, ACCEL_DUMMY_CMD);
	spi_read(ACCEL_SPI, &response);
	
	spi_unselectChip(ACCEL_SPI, ACCEL_SPI_NPCS);
	
	return (unsigned char)(response & 0xFF);
}

unsigned char accel_flushFIFO( void ){
	unsigned char i, entries;
	struct tAccelSample sample;
	
	entries = accel_readEntriesFIFO();
	
	for(i = 0; i < entries; i++){
		accel_read(&sample);
	}
	
	return ACCEL_RESPONSE_OK;
}

unsigned char accel_performInit( struct tAccelInfo *info ){
	unsigned char index, entries, i;
	struct tAccelSample sample;
	signed int x = 0, y = 0, z = 0, stx = 0, sty = 0, stz = 0, changeX = 0, changeY = 0, changeZ = 0;
	
	// Init accelInit data
	info->status = PERFORMING_INIT;
	info->selfTestPassed = FALSE;
	
	info->init.normal.x = 0;
	info->init.normal.y = 0;
	info->init.normal.z = 0;
	
	info->init.test.x = 0;
	info->init.test.y = 0;
	info->init.test.z = 0;
	
	// Check to see if the accel is available on this board 
	if( !info->available ){
		return ACCEL_RESPONSE_ERROR;
	}
	
	// Start measurement mode
	accel_setPowerCtrl( ACCEL_MASK_POWER_CTL_MEASURE );
	
	// Flush the buffer
	accel_flushFIFO();
	
	// Start reading pre-self-test samples
	index = 0;
	while( index < ACCEL_SELF_TEST_SAMPLES ){
		vTaskDelay( (portTickType)TASK_DELAY_MS( ACCEL_READ_FIFO_INTERVAL ) );
		
		entries = accel_readEntriesFIFO();
		
		for(i = 0; i < entries; i++){
			
			// We could have more samples in the FIFO than we really need
			if( index++ < ACCEL_SELF_TEST_SAMPLES ){
				
				accel_read(&sample);
				
				x += sample.x;
				y += sample.y;
				z += sample.z;
				
			}else{
				break;
			}
			
		}
	}
	
	// Average out the samples
	info->init.normal.x = x / ACCEL_SELF_TEST_SAMPLES;
	info->init.normal.y = y / ACCEL_SELF_TEST_SAMPLES;
	info->init.normal.z = z / ACCEL_SELF_TEST_SAMPLES;
	
	// Latch the self-test and allow it to settle
	accel_selfTest( ACCEL_SELF_TEST_ENABLE );
	vTaskDelay( (portTickType)TASK_DELAY_MS( ACCEL_SELF_TEST_SETTLE_TIME ) );
	
	// Flush the buffer
	accel_flushFIFO();
	
	// Start reading pre-self-test samples
	index = 0;
	while( index < ACCEL_SELF_TEST_SAMPLES ){
		vTaskDelay( (portTickType)TASK_DELAY_MS( ACCEL_READ_FIFO_INTERVAL ) );
		
		entries = accel_readEntriesFIFO();
		
		for(i = 0; i < entries; i++){
			// We could have more samples in the FIFO than we really need
			if( index++ < ACCEL_SELF_TEST_SAMPLES ){
				accel_read(&sample);
				
				stx += sample.x;
				sty += sample.y;
				stz += sample.z;
				
			}else{
				break;
			}
		}
	}
	
	// Average out the samples
	info->init.test.x = stx / ACCEL_SELF_TEST_SAMPLES;
	info->init.test.y = sty / ACCEL_SELF_TEST_SAMPLES;
	info->init.test.z = stz / ACCEL_SELF_TEST_SAMPLES;
	
	// Disable the self-test pin and allow it to settle
	accel_setPowerCtrl( ACCEL_MASK_POWER_CTL_STANDBY );
	accel_selfTest( ACCEL_SELF_TEST_DISABLE );
	vTaskDelay( (portTickType)TASK_DELAY_MS( ACCEL_SELF_TEST_SETTLE_TIME ) );	// Allow time for normal readings to come back after self test is removed
	
	// Find the difference between the normal and self-test measurements
	changeX = info->init.test.x - info->init.normal.x;
	changeY = info->init.test.y - info->init.normal.y;
	changeZ = info->init.test.z - info->init.normal.z;
	
	// Check the results against the limits in the datasheets
	if( (changeX >= ACCEL_SELF_TEST_X_MIN) && (changeX <= ACCEL_SELF_TEST_X_MAX) &&
		(changeY >= ACCEL_SELF_TEST_Y_MIN) && (changeY <= ACCEL_SELF_TEST_Y_MAX) &&
		(changeZ >= ACCEL_SELF_TEST_Z_MIN) && (changeZ <= ACCEL_SELF_TEST_Z_MAX) ){
		
		// Yes! We passed the self-test
		info->selfTestPassed = TRUE;
		info->status = IDLE;
	}else{
		
		// Damn, we failed.  Set the availability of the accel to not available.
		info->selfTestPassed = FALSE;
		info->available = FALSE;
		info->status = SELF_TEST_FAILED;
	}
	
	return ACCEL_RESPONSE_OK; 
}