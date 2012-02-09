/******************************************************************************
 *
 * Button interface
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

#include <asf.h>
#include "drivers.h"
#include "lcd/menu.h"

static xQueueHandle buttonPress;
extern xQueueHandle queueLCDmenu;


//------------------------------
// ISR's
//------------------------------
__attribute__((__interrupt__)) static void ISR_button0(void) {
	unsigned char button = BUTTON_DOWN;
	xQueueSendFromISR(buttonPress, &button, pdFALSE);
	eic_clear_interrupt_line(&AVR32_EIC, EXTINT_BUTTON0);
}  

__attribute__((__interrupt__)) static void ISR_button1(void) {
	unsigned char button = BUTTON_UP;
	xQueueSendFromISR(buttonPress, &button, pdFALSE);
	eic_clear_interrupt_line(&AVR32_EIC, EXTINT_BUTTON1);
}  

__attribute__((__interrupt__)) static void ISR_button2(void) {
	unsigned char button = BUTTON_SELECT;
	xQueueSendFromISR(buttonPress, &button, pdFALSE);
	eic_clear_interrupt_line(&AVR32_EIC, EXTINT_BUTTON2);
}  

__attribute__((__interrupt__)) static void ISR_button3(void) {
	unsigned char button = BUTTON_BACK; 
	xQueueSendFromISR(buttonPress, &button, pdFALSE);
	eic_clear_interrupt_line(&AVR32_EIC, EXTINT_BUTTON3);
}  


//------------------------------
// Functions
//------------------------------
void buttons_task_init( void ){
	buttonPress = xQueueCreate(1, sizeof(unsigned char));
	
	INTC_register_interrupt(&ISR_button0, EXTINT_BUTTON0_IRQ, EXTINT_BUTTON0); 
	INTC_register_interrupt(&ISR_button1, EXTINT_BUTTON1_IRQ, EXTINT_BUTTON1); 
	INTC_register_interrupt(&ISR_button2, EXTINT_BUTTON2_IRQ, EXTINT_BUTTON2); 
	INTC_register_interrupt(&ISR_button3, EXTINT_BUTTON3_IRQ, EXTINT_BUTTON3); 
	
	xTaskCreate(buttons_task, configTSK_BUTTONS_TASK_NAME, configTSK_BUTTONS_TASK_STACK_SIZE, NULL, configTSK_BUTTONS_TASK_PRIORITY, configTSK_BUTTONS_TASK_HANDLE);
}


void buttons_task( void *pvParameters ){
	unsigned char button;			// Storage for queue - Holds the ID of the button pressed
	unsigned short timer;			// Timer for how long a button was pressed
	unsigned char buttonStatus;		// Storage for status of the buttons
	
	while(1){
		timer = 0;
		buttonStatus = 1;
		
		// Wait for button press - suspends task
		xQueueReceive(buttonPress, &button, portMAX_DELAY);
		
		// Disable EXTINT interrupts until we are ready to process them again
		eic_disable_interrupt_lines(&AVR32_EIC, (1<<EXTINT_BUTTON0) | (1<<EXTINT_BUTTON1) | (1<<EXTINT_BUTTON2) | (1<<EXTINT_BUTTON3));

		// See how long the button is being pressed
		while( buttonStatus ){
			vTaskDelay( (portTickType)TASK_DELAY_MS(BUTTON_TIMER_INCREMENT) );
			buttonStatus = gpio_get_pin_value(GPIO_BUTTON0) | gpio_get_pin_value(GPIO_BUTTON1) | gpio_get_pin_value(GPIO_BUTTON2) | gpio_get_pin_value(GPIO_BUTTON3);
			timer++;
		}
			
		if(timer >= BUTTON_LONG_PRESS_TIMER_VALUE){
			// Qualified a long button press
			switch(button){
				case(BUTTON_UP):
					button = BUTTON_LONG_UP;
					xQueueSend(queueLCDmenu, &button, portMAX_DELAY);
					break;
					
				case(BUTTON_DOWN):
					button = BUTTON_LONG_DOWN;
					xQueueSend(queueLCDmenu, &button, portMAX_DELAY);
					break;
					
				case(BUTTON_BACK):
					button = BUTTON_LONG_BACK;
					xQueueSend(queueLCDmenu, &button, portMAX_DELAY);
					break;
					
				case(BUTTON_SELECT):
					button = BUTTON_LONG_SELECT;
					//xQueueSend(queueLCDmenu, &button, portMAX_DELAY);
					gpio_clr_gpio_pin(PM_ENABLE);
					break;
			}
			
		}else{
			// Normal short button press
			xQueueSend(queueLCDmenu, &button, portMAX_DELAY);
		}						
			
		// Clear any pending EXTINT interrupts and re-enable them
		eic_clear_interrupt_lines(&AVR32_EIC, (1<<EXTINT_BUTTON0) | (1<<EXTINT_BUTTON1) | (1<<EXTINT_BUTTON2) | (1<<EXTINT_BUTTON3));
		eic_enable_interrupt_lines(&AVR32_EIC, (1<<EXTINT_BUTTON0) | (1<<EXTINT_BUTTON1) | (1<<EXTINT_BUTTON2) | (1<<EXTINT_BUTTON3));
	}			
			
}
