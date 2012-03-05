/******************************************************************************
 *
 * Battery Fuel Gauge
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

/* TODO: Add software framework include drivers below */
#include "asf.h"
#include "drivers.h"
#include "battery.h"

void fuel_task_init( void ){
	xTaskCreate(fuel_task, configTSK_FUEL_TASK_NAME, configTSK_FUEL_TASK_STACK_SIZE, NULL, configTSK_FUEL_TASK_PRIORITY, configTSK_FUEL_TASK_HANDLE);
}


void fuel_task( void *pvParameters ){
	unsigned short percent;
	signed short accumulated_current;
	
	struct tBatteryInfo batteryInfo;
	struct tFuelStatus fuelStatus;
	struct tFuelEEStatus fuelEEStatus;
	
	debug_log(DEBUG_PRIORITY_INFO, DEBUG_SENDER_FUEL, "Task Started");
	
	// Check the Status Registers
	fuel_read_register(FUEL_ADDRESS_PROTECTION_REGISTER, &fuelStatus, sizeof(fuelStatus));
	if( fuelStatus.COC ) debug_log(DEBUG_PRIORITY_WARNING, DEBUG_SENDER_FUEL, "Charge Overcurrent Detected");
	if( fuelStatus.DOC ) debug_log(DEBUG_PRIORITY_WARNING, DEBUG_SENDER_FUEL, "Discharge Overcurrent Detected");
	if( fuelStatus.OV ) debug_log(DEBUG_PRIORITY_WARNING, DEBUG_SENDER_FUEL, "Overvoltage Detected");
	if( fuelStatus.UV ) debug_log(DEBUG_PRIORITY_WARNING, DEBUG_SENDER_FUEL, "Undervoltage Detected");

	
	batteryInfo = fuel_readBatteryInfo();
	
	if( batteryInfo.crc != fuel_calculateBatteryInfoCRC(&batteryInfo) ){
		batteryInfo.capacity = BATTERY_CAPACITY_COUNTS;
		debug_log(DEBUG_PRIORITY_WARNING, DEBUG_SENDER_FUEL, "Battery Info CRC failed");
	}
	
	while(1){
		accumulated_current = fuel_read_current( FUEL_CURRENT_ACCUMULATED ) >> 2;
		
		if(accumulated_current < 0){
			accumulated_current = 0;
			fuel_updateAccumulatedCurrent(0);
			
		}else if(accumulated_current > batteryInfo.capacity){
			accumulated_current = batteryInfo.capacity;
			
		}
		
		// Format the data in percentage
		lcd_sendWidgetRequest(LCD_REQUEST_UPDATE_BATTERY, (accumulated_current * 100) / batteryInfo.capacity, pdFALSE);
		
		vTaskDelay( (portTickType)TASK_DELAY_MS(FUEL_UPDATE_RATE) );
	}
}


void fuel_read_register(unsigned char command, unsigned char *pointer, unsigned char length){
	twi_package_t packet;

	packet.chip = 			FUEL_ADDRESS;
	packet.addr_length = 	FUEL_ADDRESS_LENGTH;
	packet.addr = 			command;
	packet.length = 		length;
	packet.buffer = 		pointer;

	twi_master_read(FUEL_TWI, &packet);
}

void fuel_write_register(unsigned char command, unsigned char *pointer, unsigned char length){
	twi_package_t packet;

	packet.chip = 			FUEL_ADDRESS;
	packet.addr_length = 	FUEL_ADDRESS_LENGTH;
	packet.addr = 			command;
	packet.length = 		length;
	packet.buffer = 		pointer;

	twi_master_write(FUEL_TWI, &packet);
}

void fuel_write_command(unsigned char fcmd){
	twi_package_t packet;

	packet.chip = 			FUEL_ADDRESS;
	packet.addr_length = 	FUEL_ADDRESS_LENGTH;
	packet.addr = 			FUEL_FUNCTION_MADDR;
	packet.length = 		1;
	packet.buffer = 		&fcmd;

	twi_master_write(FUEL_TWI, &packet);
}



unsigned short fuel_read_voltage( void ){
	unsigned short battery_voltage;
	fuel_read_register( FUEL_ADDRESS_VOLTAGE_REGISTER_MSB, &battery_voltage, 2 );
	
	return battery_voltage >> FUEL_SHIFTRIGHT_VOLTAGE;
}


unsigned short fuel_read_current( unsigned char measurement ){
	unsigned short battery_current;
	
	if( measurement == FUEL_CURRENT_INSTANTANEOUS ){	// Read instantaneous current
		fuel_read_register(FUEL_ADDRESS_CURRENT_REGISTER_MSB, &battery_current, 2);
		battery_current = battery_current >> FUEL_SHIFTRIGHT_INSTANTANEOUS_CURRENT;
		
	}else{	// Read accumulated current
		fuel_read_register(FUEL_ADDRESS_ACCUM_CURRENT_REGISTER_MSB, &battery_current, 2);
		battery_current = battery_current >> FUEL_SHIFTRIGHT_ACCUMULATED_CURRENT;
	}
	
	return battery_current;
}


unsigned short fuel_read_temperature( void ){
	unsigned short battery_temperature;
	fuel_read_register(FUEL_ADDRESS_TEMPERATURE_REGISTER_MSB, &battery_temperature, 2);
	
	return ( battery_temperature >> FUEL_SHIFTRIGHT_TEMPERATURE );
}

unsigned short fuel_calculateBatteryInfoCRC( struct tBatteryInfo *batteryInfo ){
	unsigned short crc = 0;
	
	crc = update_crc_ccitt(crc, (batteryInfo->capacity >> 8) & 0xFF);
	crc = update_crc_ccitt(crc, (batteryInfo->capacity >> 0) & 0xFF);
	
	crc = update_crc_ccitt(crc, (batteryInfo->chargeCycles >> 8) & 0xFF);
	crc = update_crc_ccitt(crc, (batteryInfo->chargeCycles >> 0) & 0xFF);
	
	crc = update_crc_ccitt(crc, (batteryInfo->health >> 8) & 0xFF);
	crc = update_crc_ccitt(crc, (batteryInfo->health >> 0) & 0xFF);
	
	return crc;
}

void fuel_copyEEtoShadowRAM( void ){
	fuel_write_command(FUEL_FUNCTION_FCMD_RCALL_BLOCK0);
	fuel_write_command(FUEL_FUNCTION_FCMD_RCALL_BLOCK1);
	fuel_write_command(FUEL_FUNCTION_FCMD_RCALL_BLOCK2);
}

void fuel_copyShadowRAMtoEE( void ){
	fuel_write_command(FUEL_FUNCTION_FCMD_COPY_BLOCK0);
	while( fuel_isBusy() ){
		vTaskDelay( (portTickType)TASK_DELAY_MS(FUEL_WRITE_TO_EE_TIME) );
	}
	
	fuel_write_command(FUEL_FUNCTION_FCMD_COPY_BLOCK1);
	while( fuel_isBusy() ){
		vTaskDelay( (portTickType)TASK_DELAY_MS(FUEL_WRITE_TO_EE_TIME) );
	}
	
	fuel_write_command(FUEL_FUNCTION_FCMD_COPY_BLOCK2);
	while( fuel_isBusy() ){
		vTaskDelay( (portTickType)TASK_DELAY_MS(FUEL_WRITE_TO_EE_TIME) );
	}
}

void fuel_clearEELocks( void ){
	unsigned char flags = 0;
	fuel_write_register(FUEL_ADDRESS_EEPROM_REGISTER, &flags, sizeof(flags));
}

struct tBatteryInfo fuel_readBatteryInfo( void ){
	struct tBatteryInfo tempInfo;
	
	fuel_copyEEtoShadowRAM();
	fuel_read_register(FUEL_ADDRESS_EEPROM_BLOCK0_START, &tempInfo, sizeof(tempInfo) );
	
	return tempInfo;
}

void fuel_writeShadowRAM( void ){
	fuel_clearEELocks();
	fuel_copyShadowRAMtoEE();
}	
	

void fuel_updateAccumulatedCurrent(unsigned short value){
	fuel_write_register(FUEL_ADDRESS_ACCUM_CURRENT_REGISTER_MSB, &value, sizeof(value) );
}


unsigned char fuel_writeBatteryInfo(unsigned short capacity, unsigned short chargeCycles, unsigned short health){
	struct tBatteryInfo batteryInfo;
	
	batteryInfo.capacity = capacity;
	batteryInfo.chargeCycles = chargeCycles;
	batteryInfo.health = health;
	batteryInfo.crc = fuel_calculateBatteryInfoCRC(&batteryInfo);

	fuel_write_register(FUEL_ADDRESS_EEPROM_BLOCK0_START, &batteryInfo, sizeof(batteryInfo) );
	fuel_writeShadowRAM();
	fuel_copyEEtoShadowRAM();
	
	return TRUE;
}

unsigned char fuel_isBusy( void ){
	struct tFuelEEStatus fuelEEStatus;
	
	fuel_read_register(FUEL_ADDRESS_EEPROM_REGISTER, &fuelEEStatus, sizeof(fuelEEStatus) );
	
	return fuelEEStatus.EEC;
}