/********************************************************
 Name          : charge.c
 Author        : Ryan David
 Copyright     : Copyright 2011 Redline Electronics
 Description   : EVK1101 template
 **********************************************************/

/* TODO: Add software framework include drivers below */
#include "asf.h"
#include "drivers.h"

void charge_task_init( void ){
	xTaskCreate(charge_task, configTSK_CHARGE_TASK_NAME, configTSK_CHARGE_TASK_STACK_SIZE, NULL, configTSK_CHARGE_TASK_PRIORITY, NULL);
}

void charge_task( void ){
	unsigned char chargeState;
	
	while( TRUE ){
		chargeState = charge_state();
		vTaskDelay( (portTickType)TASK_DELAY_MS(500) );
	}
}

void charge_setRate(unsigned char rate){
	if(rate == CHARGE_RATE_HIGH){
		#if(TRAQPAQ_HW_USB_FASTCHG_ENABLED)
		gpio_set_gpio_pin(CHARGE_RATE);
		#endif
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