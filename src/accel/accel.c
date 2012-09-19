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

//--------------------------
// Queues
//--------------------------


//--------------------------
// Structs
//--------------------------


//--------------------------
// Functions
//--------------------------
void accel_task_init( void ){
	unsigned short response;
	
	// Check if the accelerometer is responding
	if( !accel_checkID() ){
		debug_log(DEBUG_PRIORITY_WARNING, DEBUG_SENDER_ACCEL, "Incorrect accel ID - Exiting task");
	}
}


unsigned char accel_checkID( void ){
	unsigned short response;
	
	spi_selectChip(ACCEL_SPI, ACCEL_SPI_NPCS);
	spi_write(ACCEL_SPI, ACCEL_REGISTER_DEVICE_ID);
	
	spi_write(ACCEL_SPI, ACCEL_DUMMY_CMD);
	spi_read(ACCEL_SPI, &response);
	
	spi_unselectChip(ACCEL_SPI, ACCEL_SPI_NPCS);
	
	return ( response == ACCEL_DEVICE_ID );
}