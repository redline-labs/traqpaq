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
struct tAccelDevice accel;
struct tAccelData	accelData;

//--------------------------
// Functions
//--------------------------
void accel_task_init( void ){
	accel.available = FALSE;
	accel.status = UNKNOWN;
	
	//accelManagerQueue = xQueueCreate(FLASH_MANAGER_QUEUE_SIZE, sizeof(request));
	
	// Make sure the accel is in standby
	accel_setPowerCtrl( ACCEL_MASK_POWER_CTL_STANDBY );
	
	// Check if the accelerometer is responding
	if( !accel_checkID() ){
		debug_log(DEBUG_PRIORITY_WARNING, DEBUG_SENDER_ACCEL, "Incorrect accel ID");
	}
	
	xTaskCreate(accel_task, configTSK_ACCEL_TASK_NAME, configTSK_ACCEL_TASK_STACK_SIZE, NULL, configTSK_ACCEL_TASK_PRIORITY, configTSK_ACCEL_TASK_HANDLE);
}

void accel_task( void *pvParameters ){
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
	
	if( !accel_performInit( &accel ) ){
		debug_log(DEBUG_PRIORITY_WARNING, DEBUG_SENDER_ACCEL, "Init failed");
	}
	
	if( !accel.selfTestPassed ){
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
		accelData.filteredData.x = temp_x / entries;
		accelData.filteredData.y = temp_y / entries;
		accelData.filteredData.z = temp_z / entries;
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
		accel.available = TRUE;
		accel.status = UNINITALIZED;
		
		return ACCEL_RESPONSE_OK;
	}else{
		accel.available = FALSE;
		accel.status = INCORRECT_DEVID;
		
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
	
	if( (control & ACCEL_MASK_POWER_CTL_MEASURE) && (accel.status != PERFORMING_INIT) ){
		accel.status = SAMPLING;
	}else{
		accel.status = IDLE;
	}
	
	return ACCEL_RESPONSE_OK;
}

unsigned char accel_read( struct tAccelSample *sample ){
	unsigned char readCmd = ACCEL_READ | ACCEL_MULTIPLE_BYTES | ACCEL_REGISTER_DATAX0;
	
	// Even though its only 7 bytes, we will be calling this function frequently
	// PDCA is a little more efficient transferring the bytes out the SPI bus compared to SPI function calls
	
	spi_selectChip(ACCEL_SPI, ACCEL_SPI_NPCS);
	
	// Load up the PDCA channels
	pdca_load_channel(ACCEL_SPI_RX_PDCA_CHANNEL, sample, ACCEL_PCDA_READ_SIZE );
	pdca_load_channel(ACCEL_SPI_TX_PDCA_CHANNEL, &readCmd, ACCEL_PCDA_READ_SIZE ); // Use start of Flash as Dummy Bytes to Clock Out
	
	// Kick off the transfers, always Rx first
	pdca_enable(ACCEL_SPI_RX_PDCA_CHANNEL);
	pdca_enable(ACCEL_SPI_TX_PDCA_CHANNEL);
	
	// Wait for transfer to complete, too quick to put task to sleep ( ~13.8us )
	while( !(pdca_get_transfer_status(ACCEL_SPI_TX_PDCA_CHANNEL) & PDCA_TRANSFER_COMPLETE) );
	
	spi_unselectChip(ACCEL_SPI, ACCEL_SPI_NPCS);
	
	// Flip to big-endian
	sample->x = accel_flip_endian16(sample->x);
	sample->y = accel_flip_endian16(sample->y);
	sample->z = accel_flip_endian16(sample->z);
	
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

unsigned char accel_performInit( struct tAccelData *info ){
	unsigned char index, entries, i;
	struct tAccelSample sample;
	signed int x = 0, y = 0, z = 0, stx = 0, sty = 0, stz = 0, changeX = 0, changeY = 0, changeZ = 0;
	
	// Init accelInit data
	accel.status = PERFORMING_INIT;
	accel.selfTestPassed = FALSE;
	
	info->normal.x = 0;
	info->normal.y = 0;
	info->normal.z = 0;
	
	info->selfTest.x = 0;
	info->selfTest.y = 0;
	info->selfTest.z = 0;
	
	// Check to see if the accel is available on this board 
	if( !accel.available ){
		accel.status = INIT_FAILED;
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
	info->normal.x = x / ACCEL_SELF_TEST_SAMPLES;
	info->normal.y = y / ACCEL_SELF_TEST_SAMPLES;
	info->normal.z = z / ACCEL_SELF_TEST_SAMPLES;
	
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
	info->selfTest.x = stx / ACCEL_SELF_TEST_SAMPLES;
	info->selfTest.y = sty / ACCEL_SELF_TEST_SAMPLES;
	info->selfTest.z = stz / ACCEL_SELF_TEST_SAMPLES;
	
	// Disable the self-test pin and allow it to settle
	accel_setPowerCtrl( ACCEL_MASK_POWER_CTL_STANDBY );
	accel_selfTest( ACCEL_SELF_TEST_DISABLE );
	vTaskDelay( (portTickType)TASK_DELAY_MS( ACCEL_SELF_TEST_SETTLE_TIME ) );	// Allow time for normal readings to come back after self test is removed
	
	// Find the difference between the normal and self-test measurements
	changeX = info->selfTest.x - info->normal.x;
	changeY = info->selfTest.y - info->normal.y;
	changeZ = info->selfTest.z - info->normal.z;
	
	// Check the results against the limits in the datasheets
	if( (changeX >= ACCEL_SELF_TEST_X_MIN) && (changeX <= ACCEL_SELF_TEST_X_MAX) &&
		(changeY >= ACCEL_SELF_TEST_Y_MIN) && (changeY <= ACCEL_SELF_TEST_Y_MAX) &&
		(changeZ >= ACCEL_SELF_TEST_Z_MIN) && (changeZ <= ACCEL_SELF_TEST_Z_MAX) ){
		
		// Yes! We passed the self-test
		accel.selfTestPassed = TRUE;
		accel.status = IDLE;
	}else{
		
		// Damn, we failed.  Set the availability of the accel to not available.
		accel.selfTestPassed = FALSE;
		accel.available = FALSE;
		accel.status = SELF_TEST_FAILED;
	}
	
	return ACCEL_RESPONSE_OK; 
}