/******************************************************************************
 *
 * Dataflash Manager
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


#ifndef DATAFLASH_MANAGER_REQUEST_H_
#define DATAFLASH_MANAGER_REQUEST_H_

#include "hal.h"
/*
#define FLASH_REQUEST_END_CURRENT_RECORD		0
#define FLASH_REQUEST_ADD_TRACK					1
#define FLASH_REQUEST_ADD_RECORDDATA			2
#define FLASH_REQUEST_ERASE_RECORD				3
#define FLASH_REQUEST_READ_RECORDTABLE			4
#define FLASH_REQUEST_READ_OTP					5
#define FLASH_REQUEST_SECTOR_ERASE				6
#define FLASH_REQUEST_BUSY						7
#define FLASH_REQUEST_CHIP_ERASE				8
#define FLASH_REQUEST_READ_RECORDDATA			9
#define FLASH_REQUEST_IS_FLASH_FULL				10
#define FLASH_REQUEST_USED_SPACE				11
#define DFMAN_REQUEST_UPDATE_DATE				12
#define FLASH_REQUEST_READ_TRACK				13
#define FLASH_REQUEST_ERASE_RECORDED_DATA		14
#define FLASH_REQUEST_WRITE_USER_PREFS			15
#define FLASH_REQUEST_SET_TRACK					16
#define FLASH_REQUEST_ERASE_TRACKS				17

#define FLASH_REQUEST_SHUTDOWN					18

#define FLASH_REQUEST_SET_DATESTAMP				19


#define DFMAN_STATUS_BUSY						0
#define DFMAN_STATUS_READY						1 */

enum tFlashCommand {
	FLASH_MGR_END_CURRENT_RECORD,
	FLASH_MGR_ADD_TRACK,
	FLASH_MGR_ADD_RECORD_DATA,
	FLASH_MGR_ERASE_RECORD,
	FLASH_MGR_READ_RECORDTABLE,
	FLASH_MGR_READ_OTP,
	FLASH_MGR_WRITE_OTP,
	FLASH_MGR_SECTOR_ERASE,
	FLASH_MGR_BUSY,
	FLASH_MGR_CHIP_ERASE,
	FLASH_MGR_READ_RECORDATA,
	FLASH_MGR_IS_FLASH_FULL,
	FLASH_MGR_USED_SPACE,
	FLASH_MGR_UPDATE_DATE,
	FLASH_MGR_READ_TRACK,
	FLASH_MGR_ERASE_RECORDED_DATA,
	FLASH_MGR_WRITE_USER_PREFS,
	FLASH_MGR_SET_TRACK,
	FLASH_MGR_ERASE_TRACKS,
	FLASH_MGR_REQUEST_SHUTDOWN,
	FLASH_MGR_SET_DATESTAMP,
	FLASH_MGR_READ_PAGE,
	FLASH_MGR_WRITE_PAGE
};

enum tFlashStatus {
	FLASH_STATUS_BUSY = 0,
	FLASH_STATUS_READY = 1
};

struct tFlashRequest {
	//unsigned char command;		// Define the request
	enum tFlashCommand command;		// Define the request
	
	unsigned char *pointer;		// Pointer for data to be written
	unsigned short length;		// Length of data to be written
	unsigned int index;			// Used for reading requests
	unsigned char resume;		// Flag to resume the calling task
	xTaskHandle handle;			// Handle of task to resume after completion
};


#define FLASH_MANAGER_QUEUE_SIZE		5							// Items to buffer in queue

#endif /* DATAFLASH_MANAGER_REQUEST_H_ */