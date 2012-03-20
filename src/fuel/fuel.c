/******************************************************************************
 *
 * Battery Fuel Gauge
 *
 * - Compiler:          GNU GCC for AVR32
 * - Supported devices: traq|paq hardware version 1.2
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
#include "hal.h"
#include "battery.h"
#include "lcd/itoa.h"
#include "adc.h"
#include "charge.h"

xQueueHandle fuelManagerQueue;

void fuel_task_init( void ){
	// Initialize the Fuel system flags
	systemFlags.fuel.lowBattery = TRUE;
	
	xTaskCreate(fuel_task, configTSK_FUEL_TASK_NAME, configTSK_FUEL_TASK_STACK_SIZE, NULL, configTSK_FUEL_TASK_PRIORITY, configTSK_FUEL_TASK_HANDLE);
}


void fuel_task( void *pvParameters ){
	unsigned short percent;
	signed short accumulated_current;
	unsigned char oldChargeStatus;
	
	struct tBatteryInfo		batteryInfo;
	struct tFuelStatus		fuelStatus;
	struct tFuelEEStatus	fuelEEStatus;
	struct tADCvalues		adcValues;
	struct tFuelRequest		request;
	
	#if (TRAQPAQ_BATTERY_TEST_MODE == TRUE)
	unsigned char tempString[10];
	unsigned short voltage;
	#endif
	
	debug_log(DEBUG_PRIORITY_INFO, DEBUG_SENDER_FUEL, "Task Started");
	
	// Check to see if the battery is good
	while ( fuel_read_voltage() <= FUEL_VOLTAGE_POWERUP_LIMIT ){
		vTaskDelay( (portTickType)TASK_DELAY_MS( FUEL_LOW_BATTERY_CHECK_TIME ) );
	}		
	
	// Set the flag that the battery is good!
	systemFlags.fuel.lowBattery = FALSE;
	
	
	fuelManagerQueue = xQueueCreate(FUEL_QUEUE_SIZE, sizeof(request));
	
	// ---------------------------------
	// Check the Fuel Status registers
	// ---------------------------------
	fuel_read_register(FUEL_ADDRESS_PROTECTION_REGISTER, &fuelStatus, sizeof(fuelStatus));
	if( fuelStatus.COC ) debug_log(DEBUG_PRIORITY_WARNING, DEBUG_SENDER_FUEL, "Charge Overcurrent Detected");
	if( fuelStatus.DOC ) debug_log(DEBUG_PRIORITY_WARNING, DEBUG_SENDER_FUEL, "Discharge Overcurrent Detected");
	if( fuelStatus.OV ) debug_log(DEBUG_PRIORITY_WARNING, DEBUG_SENDER_FUEL, "Overvoltage Detected");
	if( fuelStatus.UV ) debug_log(DEBUG_PRIORITY_WARNING, DEBUG_SENDER_FUEL, "Undervoltage Detected");

	
	// ---------------------------------
	// Initialize battery info
	// ---------------------------------
	batteryInfo = fuel_readBatteryInfo();
	
	if( batteryInfo.crc != fuel_calculateBatteryInfoCRC(&batteryInfo) ){
		batteryInfo.capacity = BATTERY_CAPACITY_COUNTS;
		debug_log(DEBUG_PRIORITY_WARNING, DEBUG_SENDER_FUEL, "Battery Info CRC failed");
	}
	
	
	// ---------------------------------
	// Initialize charge state
	// ---------------------------------
	oldChargeStatus = charge_state();
	lcd_sendWidgetRequest(LCD_REQUEST_UPDATE_CHARGE, oldChargeStatus, pdFALSE);
	
	
	while(1){
		// Turn on ADC reference and allow it to settle
		gpio_set_gpio_pin(ADC_VREF_EN);
		
		// ---------------------------------
		// Read accumulated current
		// ---------------------------------
		accumulated_current = fuel_read_current( FUEL_CURRENT_ACCUMULATED );
		#if (TRAQPAQ_BATTERY_TEST_MODE == TRUE)
			debug_log(DEBUG_PRIORITY_INFO, DEBUG_SENDER_BATTERY_ACCUM, itoa(accumulated_current, &tempString, 10, FALSE));
			
			voltage = fuel_read_voltage();
			debug_log(DEBUG_PRIORITY_INFO, DEBUG_SENDER_BATTERY_VOLT, itoa(voltage, &tempString, 10, FALSE));
		#endif
		
		if(accumulated_current < 0){
			accumulated_current = 0;
			fuel_updateAccumulatedCurrent(0);
			
		}else if(accumulated_current > batteryInfo.capacity){
			accumulated_current = batteryInfo.capacity;
			
		}

		lcd_sendWidgetRequest(LCD_REQUEST_UPDATE_BATTERY, (accumulated_current * 100) / batteryInfo.capacity, pdFALSE);
		
		
		// ---------------------------------
		// Read supply voltages
		// ---------------------------------
		adc_start(ADC);
		adcValues.main	= adc_get_value(ADC, ADC_3V3_CHANNEL);
		adcValues.vcc	= adc_get_value(ADC, ADC_VCC_CHANNEL);
		adcValues.vee	= adc_get_value(ADC, ADC_VEE_CHANNEL);
		gpio_clr_gpio_pin(ADC_VREF_EN);
		
		
		// ---------------------------------
		// Check if battery low
		// ---------------------------------
		if ( fuel_read_voltage() <= FUEL_VOLTAGE_POWERDOWN_LIMIT ){
			wdt_send_request(WDT_REQUEST_POWEROFF, NULL);
		}		
		
		
		// ---------------------------------
		// Read charge status
		// ---------------------------------
		if( oldChargeStatus != charge_state() ){
			oldChargeStatus = charge_state();
			lcd_sendWidgetRequest(LCD_REQUEST_UPDATE_CHARGE, oldChargeStatus, pdFALSE);
		}			
		
		
		// ---------------------------------
		// Check for any requests!
		// ---------------------------------
		if( xQueueReceive(fuelManagerQueue, &request, (portTickType)TASK_DELAY_MS(FUEL_UPDATE_RATE)) == pdTRUE ){
			switch(request.command){
				case(FUEL_REQUEST_SHUTDOWN):
					debug_log(DEBUG_PRIORITY_INFO, DEBUG_SENDER_FUEL, "Task shut down");
					wdt_send_request(WDT_REQUEST_FUEL_SHUTDOWN_COMPLETE, NULL);
					vTaskSuspend(NULL);
					break;
			}
		}
		
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
	
	battery_voltage = battery_voltage >> FUEL_SHIFTRIGHT_VOLTAGE;
	
	return battery_voltage;
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

void charge_setRate(unsigned char rate){
	if(rate == CHARGE_RATE_HIGH){
		gpio_set_gpio_pin(CHARGE_RATE);
	}else{
		gpio_clr_gpio_pin(CHARGE_RATE);
	}
}

unsigned char charge_powerGood( void ){
	return gpio_get_pin_value(CHARGE_PG);
}

unsigned char charge_state( void ){
	return (( gpio_get_pin_value(CHARGE_STAT1) << 2 ) |  ( gpio_get_pin_value(CHARGE_STAT2) << 1 ) | ( gpio_get_pin_value(CHARGE_PG) << 0 ));
}

unsigned char fuel_send_request(unsigned char command, unsigned char data){
	struct tFuelRequest request;
	
	request.command = command;
	request.data = data;
	
	return xQueueSend(fuelManagerQueue, &request, portMAX_DELAY);
}