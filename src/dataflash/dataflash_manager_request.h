/******************************************************************************
 *
 * Dataflash Manager
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


#ifndef DATAFLASH_MANAGER_REQUEST_H_
#define DATAFLASH_MANAGER_REQUEST_H_

#include "drivers.h"



#define DFMAN_REQUEST_UPDATE_RECORDTABLE		0
#define DFMAN_REQUEST_UPDATE_TRACKLIST			1
#define DFMAN_REQUEST_ADD_RECORDDATA			2
#define DFMAN_REQUEST_ERASE_RECORD				3
#define DFMAN_REQUEST_READ_RECORDTABLE			4
#define DFMAN_REQUEST_READ_OTP					5
#define DFMAN_REQUEST_SECTOR_ERASE				6
#define DFMAN_REQUEST_BUSY						7
#define DFMAN_REQUEST_CHIP_ERASE				8
#define DFMAN_REQUEST_READ_RECORDDATA			9



#define DFMAN_STATUS_BUSY						0
#define DFMAN_STATUS_READY						1

typedef struct tDataflashRequest {
	unsigned char command;		// Define the request
	unsigned char *pointer;		// Pointer for data to be written
	unsigned short length;		// Length of data to be written
	unsigned long index;		// Used for reading requests

	xTaskHandle resume;			// Handle of task to resume after completion
};


#define DFMAN_QUEUE_SIZE		5							// Items to buffer in queue

#endif /* DATAFLASH_MANAGER_REQUEST_H_ */