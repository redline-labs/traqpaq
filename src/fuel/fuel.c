/********************************************************
 Name          : fuel.c
 Author        : Ryan David
 Copyright     : Copyright 2011 Redline Electronics
 Description   : EVK1101 template
 **********************************************************/

/* TODO: Add software framework include drivers below */
#include "asf.h"
#include "drivers.h"

// Add reference to the application top bar
extern struct tLCDTopBar topBar;

void fuel_task_init( void ){
	xTaskCreate(fuel_task, configTSK_FUEL_TASK_NAME, configTSK_FUEL_TASK_STACK_SIZE, NULL, configTSK_FUEL_TASK_PRIORITY, NULL);
}


void fuel_task( void ){
	unsigned short current;
	
	while(1){
		current = ( fuel_read_current( FUEL_CURRENT_ACCUMULATED ) - 0x8000 ) >> 3;
		lcd_updateBattery(&topBar, current);

		vTaskDelay( (portTickType)TASK_DELAY_MS(500) );
	}
}


void fuel_read_register(unsigned char command, unsigned short *pointer){
	twi_package_t packet;

	packet.chip = 			FUEL_ADDRESS;
	packet.addr_length = 	FUEL_ADDRESS_LENGTH;
	packet.addr = 			command;
	packet.length = 		FUEL_RESPONSE_LENGTH;
	packet.buffer = 		pointer;

	twi_master_read(FUEL_TWI, &packet);
}

/*void fuel_write_register(unsigned char command, unsigned char *pointer, unsigned char length){
	twi_package_t packet;

	packet.chip = 			FUEL_ADDRESS;
	packet.addr_length = 	FUEL_ADDRESS_LENGTH;
	packet.addr = 			command;
	packet.length = 		length;
	packet.buffer = 		pointer;

	twi_master_write(FUEL_TWI, &packet);
}*/

/*void fuel_write_command(unsigned char fcmd){
	twi_package_t packet;

	packet.chip = 			FUEL_ADDRESS;
	packet.addr_length = 	FUEL_ADDRESS_LENGTH;
	packet.addr = 			FUEL_FUNCTION_MADDR;
	packet.length = 		1;
	packet.buffer = 		&fcmd;

	twi_master_read(FUEL_TWI, &packet);
}*/



unsigned short fuel_read_voltage(void){
	unsigned short battery_voltage;
	fuel_read_register(FUEL_ADDRESS_VOLTAGE_REGISTER_MSB, &battery_voltage);
	
	return battery_voltage >> FUEL_SHIFTRIGHT_VOLTAGE;
}


unsigned short fuel_read_current(unsigned char measurement){
	unsigned short battery_current;
	
	if(measurement == FUEL_CURRENT_INSTANTANEOUS){
		fuel_read_register(FUEL_ADDRESS_CURRENT_REGISTER_MSB, &battery_current);
		battery_current = battery_current >> FUEL_SHIFTRIGHT_INSTANTANEOUS_CURRENT;
		
	}else if(measurement == FUEL_CURRENT_ACCUMULATED){
		fuel_read_register(FUEL_ADDRESS_ACCUM_CURRENT_REGISTER_MSB, &battery_current);
		battery_current = battery_current >> FUEL_SHIFTRIGHT_ACCUMULATED_CURRENT;
	}
	
	return battery_current;
}


unsigned short fuel_read_temperature(void){
	unsigned short battery_temperature;
	fuel_read_register(FUEL_ADDRESS_TEMPERATURE_REGISTER_MSB, &battery_temperature);
	
	return ( battery_temperature >> FUEL_SHIFTRIGHT_TEMPERATURE );
}

/*struct tFuelStatus fuel_read_status(void){
	struct tFuelStatus structuredStatus;
	
	fuel_read_register(FUEL_ADDRESS_PROTECTION_REGISTER, &structuredStatus);

	return structuredStatus;
}

struct tFuelEEStatus fuel_read_EEstatus(void){
	struct tFuelEEStatus structuredEEStatus;
	
	fuel_read_register(FUEL_ADDRESS_EEPROM_REGISTER, &structuredEEStatus);

	return structuredEEStatus;
}*/

/*void fuel_write_batteryInfo(struct tBatteryInfo *battery){
	fuel_write_register(FUEL_ADDRESS_EEPROM_BLOCK0_START, battery, sizeof(battery));
}*/

/*void fuel_read_batteryInfo(struct tBatteryInfo *battery){
	twi_package_t packet;

	packet.chip = 			FUEL_ADDRESS;
	packet.addr_length = 	FUEL_ADDRESS_LENGTH;
	packet.addr = 			FUEL_ADDRESS_EEPROM_BLOCK0_START;
	packet.length = 		32;
	packet.buffer = 		battery;

	twi_master_read(FUEL_TWI, &packet);
}*/