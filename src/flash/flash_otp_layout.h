/******************************************************************************
 *
 * Dataflash Layout for OTP registers
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


#ifndef FLASH_OTP_LAYOUT_H_
#define FLASH_OTP_LAYOUT_H_

#define FLASH_OTP_LAYOUT_VERSION	0x0100
#define FLASH_OTP_LAYOUT_VERSION_ASCII	"1.00"
#define FLASH_OTP_SIZE			128		// Bytes

#define OTP_START_INDEX			0
#define OTP_END_INDEX			17
#define OTP_LENGTH				18

#define OTP_SERIAL_INDEX		0
#define OTP_SERIAL_LENGTH		13

#define OTP_PCB_REV_INDEX		13
#define OTP_TESTER_ID_INDEX		14
#define OTP_RESERVED_INDEX		15

#define OTP_CRC_MSB_INDEX		16
#define OTP_CRC_LSB_INDEX		17

#define OTP_RESERVED_DATA		0xAA


struct __attribute__ ((packed)) tFlashOTP {
	unsigned char serial[OTP_SERIAL_LENGTH];
	unsigned char pcb_rev;
	unsigned char tester_id;
	unsigned char reserved;
	unsigned short crc;
};		// 18 Bytes

#endif /* FLASH_OTP_LAYOUT_H_ */