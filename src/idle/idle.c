/*
 * idle.c
 *
 * Created: 11/22/2011 6:14:56 PM
 *  Author: Ryan
 */ 

#include "asf.h"
#include "drivers.h"

// This hooks into the idle task of FreeRTOS
void vApplicationIdleHook(){
	while(1){
		// TODO: Investigate how deep of sleep we can use here
		pm_sleep(AVR32_PM_SMODE_FROZEN);
	}		
}