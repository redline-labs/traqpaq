/******************************************************************************
 *
 * Battery Fuel Gauge Include
 *
 * - Compiler:          GNU GCC for AVR32
 * - Supported devices: traq|paq hardware version 1.1
 * - AppNote:			N/A
 *
 * - Last Author:		Ryan David ( ryan.david@redline-electronics.com )
 *
 *
 * Copyright (c) 2011 Redline Electronics LLC.
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

#ifndef FUEL_H_
#define FUEL_H_

#define FUEL_VERSION				"1.00"

#define BATTERY_SUPPLIER_STRLEN		10
#define BATTERY_PARTNUMBER_STRLEN	10

#define FUEL_ADDRESS_PROTECTION_REGISTER		0x00
#define FUEL_ADDRESS_STATUS_REGISTER			0x01
#define FUEL_ADDRESS_RESERVED00					0x02
#define FUEL_ADDRESS_RESERVED01					0x03
#define FUEL_ADDRESS_RESERVED02					0x04
#define FUEL_ADDRESS_RESERVED03					0x05
#define FUEL_ADDRESS_RESERVED04					0x06
#define FUEL_ADDRESS_EEPROM_REGISTER			0x07
#define FUEL_ADDRESS_SPECIAL_FEATURE_REGISTER	0x08
#define FUEL_ADDRESS_RESERVED05					0x09
#define FUEL_ADDRESS_RESERVED06					0x0A
#define FUEL_ADDRESS_RESERVED07					0x0B
#define FUEL_ADDRESS_VOLTAGE_REGISTER_MSB		0x0C
#define FUEL_ADDRESS_VOLTAGE_REGISTER_LSB		0x0D
#define FUEL_ADDRESS_CURRENT_REGISTER_MSB		0x0E
#define FUEL_ADDRESS_CURRENT_REGISTER_LSB		0x0F
#define FUEL_ADDRESS_ACCUM_CURRENT_REGISTER_MSB	0x10
#define FUEL_ADDRESS_ACCUM_CURRENT_REGISTER_LSB	0x11
#define FUEL_ADDRESS_RESERVED08					0x12
#define FUEL_ADDRESS_RESERVED09					0x13
#define FUEL_ADDRESS_RESERVED10					0x14
#define FUEL_ADDRESS_RESERVED11					0x15
#define FUEL_ADDRESS_RESERVED12					0x16
#define FUEL_ADDRESS_RESERVED13					0x17
#define FUEL_ADDRESS_TEMPERATURE_REGISTER_MSB	0x18
#define FUEL_ADDRESS_TEMPERATURE_REGISTER_LSB	0x19
#define FUEL_ADDRESS_RESERVED14					0x1A
#define FUEL_ADDRESS_RESERVED15					0x1B
#define FUEL_ADDRESS_RESERVED16					0x1C
#define FUEL_ADDRESS_RESERVED17					0x1D
#define FUEL_ADDRESS_RESERVED18					0x1E
#define FUEL_ADDRESS_RESERVED19					0x1F
#define FUEL_ADDRESS_EEPROM_BLOCK0_START		0x20
#define FUEL_ADDRESS_EEPROM_BLOCK1_START		0x30
#define FUEL_ADDRESS_EEPROM_BLOCK2_START		0x40
#define FUEL_ADDRESS_FUNCTION_COMMAND_REGISTER	0xFE

#define FUEL_CURRENT_INSTANTANEOUS				0
#define FUEL_CURRENT_ACCUMULATED				1

#define FUEL_FUNCTION_MADDR						0x0FE										
#define FUEL_FUNCTION_COMMAND_BLOCK0			0x42
#define FUEL_FUNCTION_COMMAND_BLOCK1			0x43
#define FUEL_FUNCTION_COMMAND_BLOCK2			0x48		

#define FUEL_MASK_PROTECT_OV					0b10000000	// Overvoltage Flag
#define FUEL_MASK_PROTECT_UV					0b01000000	// Undervoltage Flag
#define FUEL_MASK_PROTECT_COC					0b00100000	// Charge Overcurrent Flag
#define FUEL_MASK_PROTECT_DOC					0b00010000	// Discharge Overcurrent Flag
#define FUEL_MASK_PROTECT_CC					0b00001000	// CC Pin Mirror
#define FUEL_MASK_PROTECT_DC					0b00000100	// DC Pin Mirror
#define FUEL_MASK_PROTECT_CE					0b00000010	// Charge Enable
#define FUEL_MASK_PROTECT_DE					0b00000001	// Discharge Enable
#define FUEL_MASK_STATUS_PMOD					0b00100000	// Sleep Mode Enable
#define FUEL_MASK_EEPROM_EEC					0b10000000	// EEPROM Copy Flag
#define FUEL_MASK_EEPROM_LOCK					0b01000000	// EEPROM Lock Enable
#define FUEL_MASK_EEPROM_BL2					0b00000100	// Block 2 Lock Flag
#define FUEL_MASK_EEPROM_BL1					0b00000010	// Block 1 Lock Flag
#define FUEL_MASK_EEPROM_BL0					0b00000001	// Block 0 Lock Flag
#define FUEL_MASK_SPECIAL_PS					0b10000000	// PS Pin Latch
#define FUEL_MASK_SPECIAL_SAWE					0b00000010	// Slave Address Write Enable

#define FUEL_SHIFTRIGHT_VOLTAGE					5
#define FUEL_SHIFTRIGHT_INSTANTANEOUS_CURRENT	3
#define FUEL_SHIFTRIGHT_ACCUMULATED_CURRENT		0
#define FUEL_SHIFTRIGHT_TEMPERATURE				5

#define FUEL_LIMIT_CURRENT_BLOFF_UPPER			0xFF80	// 9.3mA
#define FUEL_LIMIT_CURRENT_BLOFF_LOWER			0xFF30	// 15.6mA
#define FUEL_LIMIT_CURRENT_BLON_UPPER			0xF7FF	// 160mA
#define FUEL_LIMIT_CURRENT_BLON_LOWER			0xF57F	// 210mA
#define FUEL_LIMIT_TEMPERATURE_UPPER			0x1C00	// TODO: Find real temperature limit
#define FUEL_LIMIT_TEMPERATURE_LOWER			0x1B00
#define FUEL_LIMIT_ACUM_UPPER					0x9000	// TODO: Find real accumulated current limit
#define FUEL_LIMIT_ACUM_LOWER					0x7000

#define FUEL_LIMIT_VOLTAGE_LOWER				0x0265	// 2.99 Volts
#define FUEL_LIMIT_VOLTAGE_UPPER				0x035F	// 4.21 Volts


typedef struct tFuelStatus{ 
	// Protection Register - 8 bits
    unsigned OV		: 1;	// Overvoltage Flag
	unsigned UV		: 1;	// Undervoltage Flag
	unsigned COC	: 1;	// Charge Overcurrent Flag
	unsigned DOC	: 1;	// Discharge Overcurrent Flag
	unsigned CC		: 1;	// CC Pin Mirror (Active Low)
	unsigned DC		: 1;	// DC Pin Mirror (Active Low)
	unsigned CE		: 1;	// Charge Enable - 1 enables charging, 0 disables charging
	unsigned DE		: 1;	// Discharge Enable - 1 enables discharging, 0 disables discharging
	   
	// Status Register - 8 bits
	unsigned Res1	: 2;	// Reserved bits
	unsigned PMOD	: 1;	// Sleep Mode Enable
	unsigned Res0	: 5;	// Reserved bits
};

typedef struct tFuelBatteryStatus{
	unsigned short accumCurrent;
	unsigned short voltage;
	unsigned short instCurrent;
	unsigned short temperature;
};


typedef struct tFuelEEStatus{
	// EEPROM Register
	unsigned EEC	: 1;	// EEPROM Copy Flag
	unsigned LOCK	: 1;	// EEPROM Lock Enable
	unsigned Res2	: 3;	// Reserved bits
	unsigned BL2	: 1;	// EEPROM Block 2 Lock Flag
	unsigned BL1	: 1;	// EEPROM Block 1 Lock Flag
	unsigned BL0	: 1;	// EEPROM Block 0 Lock Flag
	
	// Special Feature Register
	unsigned PS		: 1;	// PS Pin Latch (Active Low)
	unsigned Res1	: 5;	// Reserved bits
	unsigned SAWE	: 1;	// Slave Address Write Enable
	unsigned Res0	: 1;	// Reserved bit
};

typedef struct tBatteryInfo{
	unsigned char supplier[BATTERY_SUPPLIER_STRLEN];
	unsigned char partNumber[BATTERY_PARTNUMBER_STRLEN];
	unsigned short capacity;		// Capacity of Battery in mA
	unsigned char useFastCharge;	// Allow USB Fast Charging for battery
	unsigned short minVoltage;		// Min Voltage Threshold
	unsigned short chargeCycles;	// Number of Charge Cycles
	unsigned char health;			// Health of battery - default to FF
	unsigned char status;			// Status of battery - default to FF
	unsigned char reserved;			// Reserved - default to FF
	unsigned short crc;				// CRC of tBatteryInfo
};	// tBatteryInfo - 32 Bytes


void fuel_task_init( void );
void fuel_task( void );
void fuel_read_register(unsigned char command, unsigned short *pointer);
unsigned short fuel_read_voltage(void);
unsigned short fuel_read_current(unsigned char measurement);
unsigned short fuel_read_temperature(void);
struct tFuelStatus fuel_read_status(void);
struct tFuelEEStatus fuel_read_EEstatus(void);
//void fuel_write_batteryInfo(struct tBatteryInfo *battery);
//void fuel_write_command(unsigned char fcmd);
//void fuel_read_batteryInfo(struct tBatteryInfo *battery);

#endif /* FUEL_H_ */
