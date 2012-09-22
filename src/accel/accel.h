/******************************************************************************
 *
 * Accelerometer Interface Include
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

#ifndef ACCEL_H_
#define ACCEL_H_

#define ACCEL_VERSION_MAJOR			1
#define ACCEL_VERSION_MINOR			0
#define ACCEL_VERSION_ASCII			"1.0"

#define ACCEL_DRIVER_TYPE			"ADXL345"

#define ACCEL_MANAGER_QUEUE_SIZE		5

#define ACCEL_RESPONSE_OK				TRUE
#define ACCEL_RESPONSE_ERROR			FALSE

#define ACCEL_READ						0b10000000
#define ACCEL_WRITE						0b00000000
#define ACCEL_SINGLE_BYTE				0b00000000
#define ACCEL_MULTIPLE_BYTES			0b01000000

#define ACCEL_REGISTER_DEVICE_ID		0x00	// Accelerometer Device ID
#define ACCEL_REGISTER_THRESH_TAP		0x1D	// Tap Threshold
#define ACCEL_REGISTER_OFSX				0x1E	// X-Axis Offset
#define ACCEL_REGISTER_OFSY				0x1F	// Y-Axis Offset
#define ACCEL_REGISTER_OFSZ				0x20	// Z-Axis Offset
#define ACCEL_REGISTER_DUR				0x21	// Tap Duration
#define ACCEL_REGISTER_LATENT			0x22	// Tap Latency
#define ACCEL_REGISTER_WINDOW			0x23	// Tap Window
#define ACCEL_REGISTER_THRESH_ACT		0x24	// Activity Threshold
#define ACCEL_REGISTER_THRESH_INACT		0x25	// Inactivity threshold
#define ACCEL_REGISTER_TIME_INACT		0x26	// Inactivity time
#define ACCEL_REGISTER_ACT_INACT_CTL	0x27	// Axis enable control for activity and inactivity detection
#define ACCEL_REGISTER_THRESH_FF		0x28	// Free-fall threshold
#define ACCEL_REGISTER_TIME_FF			0x29	// Free-fall time
#define ACCEL_REGISTER_TAP_AXES			0x2A	// Axis control for tap/double tap
#define ACCEL_REGISTER_ACT_TAP_STATUS	0x2B	// Source of tap/double tap
#define ACCEL_REGISTER_BW_RATE			0x2C	// Data rate and power mode control
#define ACCEL_REGISTER_POWER_CTL		0x2D	// Power-saving features control
#define ACCEL_REGISTER_INT_ENABLE		0x2E	// Interrupt enable control
#define ACCEL_REGISTER_INT_MAP			0x2F	// Interrupt mapping control
#define ACCEL_REGISTER_INT_SOURCE		0x30	// Source of interrupts
#define ACCEL_REGISTER_DATA_FORMAT		0x31	// Data format control
#define ACCEL_REGISTER_DATAX0			0x32	// X-Axis Data 0
#define ACCEL_REGISTER_DATAX1			0x33	// X-Axis Data 1
#define ACCEL_REGISTER_DATAY0			0x34	// Y-Axis Data 0
#define ACCEL_REGISTER_DATAY1			0x35	// Y-Axis Data 1
#define ACCEL_REGISTER_DATAZ0			0x36	// Z-Axis Data 0
#define ACCEL_REGISTER_DATAZ1			0x37	// Z-Axis Data 1
#define ACCEL_REGISTER_FIFO_CTL			0x38	// FIFO Control
#define ACCEL_REGISTER_FIFO_STATUS		0x39	// FIFO status

#define ACCEL_DUMMY_CMD					0xFF

#define ACCEL_DEVICE_ID					0xE5	// Expected device ID


#define ACCEL_DATA_RATE_3200HZ			0b1111
#define ACCEL_DATA_RATE_1600HZ			0b1110
#define ACCEL_DATA_RATE_800HZ			0b1101
#define ACCEL_DATA_RATE_400HZ			0b1100
#define ACCEL_DATA_RATE_200HZ			0b1011
#define ACCEL_DATA_RATE_100HZ			0b1010
#define ACCEL_DATA_RATE_50HZ			0b1001
#define ACCEL_DATA_RATE_25HZ			0b1000
#define ACCEL_DATA_RATE_12HZ			0b0111
#define ACCEL_DATA_RATE_6HZ				0b0110

// Power Control Masks
#define ACCEL_MASK_POWER_CTL_STANDBY	0b00000000
#define ACCEL_MASK_POWER_CTL_MEASURE	0b00001000

// DATA_FORMAT Masks
#define ACCEL_MASK_SELF_TEST_ENABLE		0b10000000
#define ACCEL_MASK_SELF_TEST_DISABLE	0b00000000

#define ACCEL_MASK_SPI_4_WIRE			0b00000000
#define ACCEL_MASK_SPI_3_WIRE			0b01000000

#define ACCEL_MASK_INT_NORMAL			0b00000000
#define ACCEL_MASK_INT_INVERT			0b00100000

#define ACCEL_MASK_RES_FULL				0b00001000
#define ACCEL_MASK_RES_10BIT			0b00000000

#define ACCEL_MASK_JUSTIFY_LEFT			0b00000100
#define ACCEL_MASK_JUSTIFY_RIGHT		0b00000000

#define ACCEL_MASK_RANGE_2G				0b00000000
#define ACCEL_MASK_RANGE_4G				0b00000001
#define ACCEL_MASK_RANGE_8G				0b00000010
#define ACCEL_MASK_RANGE_16G			0b00000011

// FIFO_CTL
#define ACCEL_MASK_FIFO_MODE_BYPASS		0b00000000	// FIFO is bypassed
#define ACCEL_MASK_FIFO_MODE_FIFO		0b01000000	// FIFO holds up to 32 values and then stops collecting data
#define ACCEL_MASK_FIFO_MODE_STREAM		0b10000000	// FIFO holds the last 32 data values. When the FIFO is full, the oldest data is overwritten
#define ACCEL_MASK_FIFO_MODE_TRIGGER	0b11000000	// When triggered by the trigger bit, the FIFO holds the last data samples before the trigger event and then continues to collect data until full

#define ACCEL_MASK_TRIGGER_INT1			0b00000000
#define ACCEL_MASK_TRIGGER_INT2			0b00100000

// FIFO_STATUS
#define ACCEL_MASK_FIFO_ENTRIES			0b00111111

#define ACCEL_SELF_TEST_SAMPLES			128			// Keep a power of two to make division easy

#define ACCEL_SENSITIVITY				4			// 3.9mg per LSB
#define ACCEL_SELF_TEST_SETTLE_TIME		2			// Time in milliseconds to allow the self-test to settle

#define ACCEL_SELF_TEST_X_MIN			51			// Counts
#define ACCEL_SELF_TEST_X_MAX			538			// Counts
#define ACCEL_SELF_TEST_Y_MIN			-538		// Counts
#define ACCEL_SELF_TEST_Y_MAX			-51			// Counts
#define ACCEL_SELF_TEST_Z_MIN			77			// Counts
#define ACCEL_SELF_TEST_Z_MAX			872			// Counts

#define ACCEL_READ_FIFO_INTERVAL		15			// Time in milliseconds to read the FIFO


enum tAccelSelfTest {
	ACCEL_SELF_TEST_ENABLE,
	ACCEL_SELF_TEST_DISABLE	
};


struct __attribute__ ((packed)) tAccelSample {
	signed short x;
	signed short y;
	signed short z;	
};


struct tAccelInitData {
	struct tAccelSample normal;
	struct tAccelSample test;
};

enum tAccelStatus {
	UNKNOWN,
	INCORRECT_DEVID,
	UNINITALIZED,
	CHECK_ID_FAILED,
	PERFORMING_INIT,
	SELF_TEST_FAILED,
	IDLE,
	SAMPLING
};

struct tAccelInfo {
	unsigned char available;
	unsigned char selfTestPassed;
	enum tAccelStatus status;
	struct tAccelInitData init;
	struct tAccelSample filteredData;
};

#define ACCEL_SAMPLES_PER_PAGE		42


struct __attribute__ ((packed)) tAccelDataPage {
	unsigned char error;
	unsigned char bufferOverrun;
	unsigned char reserved[2];
	
	struct tAccelSample samples[ACCEL_SAMPLES_PER_PAGE];
};


void accel_task_init( void );
void accel_task( void *pvParameters );
unsigned char accel_checkID( void );
unsigned char accel_setDataFormat( unsigned char format );
unsigned char accel_setDataRate( unsigned char rate );
unsigned char accel_setFIFOCtrl( unsigned char control );
unsigned char accel_setPowerCtrl( unsigned char control);
unsigned char accel_read( struct tAccelSample *sample );
unsigned char accel_readEntriesFIFO( void );
unsigned char accel_selfTest( enum tAccelSelfTest flag );
unsigned char accel_readRegister( unsigned char registerAddr );
unsigned char accel_flushFIFO( void );
unsigned char accel_performInit( struct tAccelInfo *info );

#endif /* ACCEL_H_ */